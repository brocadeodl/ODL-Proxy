/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkConfigurationWorker.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Core/PrismFrameworkConfigurationContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManagerHaSyncWorker.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerLiveSyncEnableMessages.h"
#include <sys/file.h>


namespace WaveNs
{

PrismFrameworkConfigurationWorker::PrismFrameworkConfigurationWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    setPrismConfigurationValidity( true );
    if ( WAVE_MGMT_INTF_ROLE_SERVER == (FrameworkToolKit::getManagementInterfaceRole ()) )
    {
        //we dont want any waveclient to use flock mechanism to change configuration
        // only server is expected to change configuration
        createLockFileForConfigurationFile();
    }

    addOperationMap (FRAMEWORK_OBJECT_MANAGER_SYNC_CONFIGURATION_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkConfigurationWorker::syncPrismConfigurationMessageHandler));
}

PrismFrameworkConfigurationWorker::~PrismFrameworkConfigurationWorker ()
{
}

PrismMessage *PrismFrameworkConfigurationWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case FRAMEWORK_OBJECT_MANAGER_SYNC_CONFIGURATION_MESSAGE:
            pPrismMessage = new FrameworkObjectManagerSyncConfigurationMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

ResourceId PrismFrameworkConfigurationWorker::savePrismConfiguration (const string &prismConfigurationFile, const bool &syncToStandby)
{
    PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext = NULL;

    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::validateStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::initializeConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::displayConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::saveConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::sendConfigurationToStandbyStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::prismSynchronousLinearSequencerFailedStep)
    };

    pPrismFrameworkConfigurationContext = new PrismFrameworkConfigurationContext (NULL, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), prismConfigurationFile);

    pPrismFrameworkConfigurationContext->setSyncToStandby (syncToStandby);

    ResourceId status = pPrismFrameworkConfigurationContext->execute ();

    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::validateStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{
    if ( true ==  FrameworkToolKit::isNodeZeroized ()) 
    {
        trace (TRACE_LEVEL_ERROR,"PrismFrameworkConfigurationWorker::validateStep: Failed to save cfg file as node is zeroized.");

        return (WAVE_MESSAGE_ERROR);
    }
    
    if ( false == getPrismConfigurationValidity() )
    {

        if (true == pPrismFrameworkConfigurationContext->getSyncToStandby ())
        {
            trace (TRACE_LEVEL_INFO,"PrismFrameworkConfigurationWorker::validateStep: CFG file is invalid and will not save the configuration, but still sync over in-memory CFG content to standby" );
        }
        else
        {
            trace (TRACE_LEVEL_INFO,"PrismFrameworkConfigurationWorker::validateStep: CFG file is invalid and will not save the configuration" );
            return (WAVE_MESSAGE_ERROR);
        }

    }
    return (WAVE_MESSAGE_SUCCESS) ;
}

ResourceId PrismFrameworkConfigurationWorker::initializeConfigurationStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{
    PrismFrameworkConfiguration &prismFrameworkConfigurationObject = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();

    return (prismFrameworkConfigurationObject.initializeConfiguration ());
}

ResourceId PrismFrameworkConfigurationWorker::saveConfigurationStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (true == getPrismConfigurationValidity())
    {
        status = configurationWithFlock (SAVE_CONFIGURATION, pPrismFrameworkConfigurationContext);
    }
    
    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::sendConfigurationToStandbyStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
   
    if (pPrismFrameworkConfigurationContext->getSyncToStandby () == true) 
    {

        if (FrameworkToolKit::getIsLiveSyncEnabled () == false)
        {
            if (FrameworkToolKit::getSyncState () == IN_SYNC)
            {
                // DB previously in sync from sync dump. but live transaction sync not supported. So set out of sync and notify on sync update fail
                trace (TRACE_LEVEL_WARN, "PrismFrameworkConfigurationWorker::sendConfigurationToStandbyStep: Live sync disabled. Notify sync update failure on first update after sync dump.");
                FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CLUSTER_STATUS_HASYNC_NOT_SUPPORTED);
            }
            return (WAVE_MESSAGE_SUCCESS);
        }

        trace (TRACE_LEVEL_INFO, "PrismFrameworkConfigurationWorker::sendConfigurationToStandbyStep : Syncing configuration to standby");

        FrameworkObjectManagerSyncConfigurationMessage *pFrameworkObjectManagerSyncConfigurationMessage = new FrameworkObjectManagerSyncConfigurationMessage ();

        pFrameworkObjectManagerSyncConfigurationMessage->setPrismFrameworkConfigurationFileName (PrismFrameworkObjectManager::getConfigurationFileName ());
       
        bool cfgValidity = getPrismConfigurationValidity(); 

        PrismFrameworkConfiguration &prismFrameworkConfigurationObject = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();

        string serializedConfigurationObject;

        prismFrameworkConfigurationObject.serialize2 (serializedConfigurationObject);

        pFrameworkObjectManagerSyncConfigurationMessage->addBuffer (CFG_OBJECT, serializedConfigurationObject.size (), (void *) (serializedConfigurationObject.c_str()), false);

        pFrameworkObjectManagerSyncConfigurationMessage->addBuffer (CFG_VALIDITY, sizeof (bool), (void *) &cfgValidity, false);

        status  = sendSynchronously (pFrameworkObjectManagerSyncConfigurationMessage, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkConfigurationWorker::sendConfigurationToStandbyStep : Could not send message to sync configuration.  Status : " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_STATUS_HASYNC_FAILED);
        }
        else
        {
             status = pFrameworkObjectManagerSyncConfigurationMessage->getCompletionStatus ();
     
             if (WAVE_MESSAGE_SUCCESS != status)
             {
                 trace (TRACE_LEVEL_FATAL, "PrismFrameworkConfigurationWorker::sendConfigurationToStandbyStep : Message to sync configuration failed.  Completion Status : " + FrameworkToolKit::localize (status));
                 FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_STATUS_HASYNC_FAILED); 
             }
             else
             {
                 trace (TRACE_LEVEL_INFO, "PrismFrameworkConfigurationWorker::sendConfigurationToStandbyStep: FrameworkObjectManagerSyncConfigurationMessage successfully sent.");
             }
        }

        delete (pFrameworkObjectManagerSyncConfigurationMessage);
    }

    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::loadPrismConfiguration (const string &prismConfigurationFile)
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::loadConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::displayConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::prismSynchronousLinearSequencerFailedStep)
    };

    PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext = new PrismFrameworkConfigurationContext (NULL, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), prismConfigurationFile);

    ResourceId status = pPrismFrameworkConfigurationContext->execute ();

    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::loadConfigurationStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{
    string                      &prismFrameworkConfigurationFileName = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationFileName ();
    PrismFrameworkConfiguration &prismFrameworkConfigurationObject   = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    if (WAVE_MGMT_INTF_ROLE_SERVER != (FrameworkToolKit::getManagementInterfaceRole ()))
    {
        // Wave client need not use flock mechanism to change configuration
        status = prismFrameworkConfigurationObject.loadConfiguration (prismFrameworkConfigurationFileName);
    }
    else
    {
        status = configurationWithFlock (LOAD_CONFIGURATION, pPrismFrameworkConfigurationContext );
    }
    
    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::displayConfigurationStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{
    PrismFrameworkConfiguration &prismFrameworkConfigurationObject   = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();

    prismFrameworkConfigurationObject.display ();

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismFrameworkConfigurationWorker::changePrismConfigurationValidity ( const bool &validity)
{
    
    setPrismConfigurationValidity(validity);
    ResourceId status = WAVE_MESSAGE_SUCCESS;
     
    if ( validity == false )
    {
        //Since we remove Cfg file need to acquire flock
        status = configurationWithFlock (CHANGE_CONFIGURATION_VALIDITY);
    }
    else
    {
        status = savePrismConfiguration( PrismFrameworkObjectManager::getConfigurationFileName(), false);        
        trace (TRACE_LEVEL_INFO, "PrismFrameworkConfigurationWorker::changePrismConfigurationValidity : Configuration File is valid now ");
    }
    
    return status;
    
}

void PrismFrameworkConfigurationWorker::setPrismConfigurationValidity(const bool &validity)
{
    m_prismConfigurationValidityMutex.lock();
    
    m_isPrismConfigurationValid = validity;
    
    m_prismConfigurationValidityMutex.unlock();
}

bool PrismFrameworkConfigurationWorker::getPrismConfigurationValidity( )
{
    m_prismConfigurationValidityMutex.lock();

    bool validity = m_isPrismConfigurationValid ;

    m_prismConfigurationValidityMutex.unlock();
    
    return validity;
}

void PrismFrameworkConfigurationWorker::createLockFileForConfigurationFile()
{
    string cmdToCreateLockFile = "/bin/touch " + PrismFrameworkObjectManager::getLockFileForConfigurationFile(); 
    vector<string>  output;
    SI32 cmdStatus = 0;
    cmdStatus = FrameworkToolKit::systemCommandOutput ( cmdToCreateLockFile, output );

    if ( cmdStatus != 0 )
    {   
        if (0 < (output.size ()))
        {
            trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::createLockFileForConfigurationFile : cmdToCreateLockFile = ")+ cmdToCreateLockFile + " failed with error message : " + output[0]);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::createLockFileForConfigurationFile : cmdToCreateLockFile = ")+ cmdToCreateLockFile + " failed.");
        }
    }
}

ResourceId PrismFrameworkConfigurationWorker::configurationWithFlock ( const UI32 &operationType, PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{   

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    FILE *pFile ;
    string lockFilename = PrismFrameworkObjectManager::getLockFileForConfigurationFile();
    SI32 cmdStatus = 0;

    //Acquire flock for all operation types
    pFile = fopen (lockFilename.c_str(),"w");
    
    if (pFile == NULL)
    {   
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::configurationWithFlock : Unable to open the lockfile --> ") + lockFilename.c_str() );
        return (WAVE_MESSAGE_ERROR);
    }
    
    if (0 != flock( fileno(pFile), LOCK_EX ))
    {   
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::configurationWithFlock : Unable to secure the lock on Configurationlockfile --> ") + lockFilename.c_str() );  
        fclose( pFile );
        return (WAVE_MESSAGE_ERROR);
    }

    trace (TRACE_LEVEL_DEBUG, string("PrismFrameworkConfigurationWorker::configurationWithFlock : Acquired lock on Configurationlockfile --> ") + lockFilename.c_str() );

    switch ( operationType )
    {
     
        case CHANGE_CONFIGURATION_VALIDITY :
                                    {      //This is used to invalidate the configuration                              
                                        string          cmdToDeleteCFG  = "/bin/rm -rf " + PrismFrameworkObjectManager::getConfigurationFileName();
                                        vector<string>  output;

                                        cmdStatus = FrameworkToolKit::systemCommandOutput (cmdToDeleteCFG, output);
                                        if ( cmdStatus != 0 )
                                        {   
                                            trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::configurationWithFlock : cmdToDeleteCFG = ")+ cmdToDeleteCFG + " failed with error message : " + output[0]);
                                            status = WAVE_MESSAGE_ERROR;
                                        }
                                        break;
                                    }

        case SAVE_CONFIGURATION :       
                                    {
                                        if ( pPrismFrameworkConfigurationContext == NULL  )
                                        {
                                            trace (TRACE_LEVEL_ERROR,"PrismFrameworkConfigurationWorker::configurationWithFlock : pPrismFrameworkConfigurationContext for Save is Null");
                                            status = WAVE_MESSAGE_ERROR;
                                            break ;                    
                                        }                        
                                        string   &prismFrameworkConfigurationFileName = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationFileName ();
                                        PrismFrameworkConfiguration &prismFrameworkConfigurationObject   = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();                                               
                                        status = prismFrameworkConfigurationObject.saveConfiguration (prismFrameworkConfigurationFileName);
                                        break;
                                    }

        case LOAD_CONFIGURATION :
                                    {
                                        if ( pPrismFrameworkConfigurationContext == NULL  )
                                        {
                                            trace (TRACE_LEVEL_ERROR,"PrismFrameworkConfigurationWorker::configurationWithFlock : pPrismFrameworkConfigurationContext for Load is Null");
                                            status = WAVE_MESSAGE_ERROR;
                                            break ;
                                        }
                                        string   &prismFrameworkConfigurationFileName = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationFileName ();
                                        PrismFrameworkConfiguration &prismFrameworkConfigurationObject   = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();      
                                        status = prismFrameworkConfigurationObject.loadConfiguration (prismFrameworkConfigurationFileName);
                                        break;
                                    }
        default :
                                        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::configurationWithFlock : Unknown operation type = ") + operationType );
                                        status = WAVE_MESSAGE_ERROR;
                                        break;                                                         
                            
    }
    // Since the operation is complete release the flock
    flock( fileno(pFile), LOCK_UN );
    trace (TRACE_LEVEL_DEBUG, string("PrismFrameworkConfigurationWorker::configurationWithFlock : released lock on Configurationlockfile --> ") + lockFilename.c_str() );
    fclose( pFile );
    
    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::syncPrismConfigurationMessageHandler (FrameworkObjectManagerSyncConfigurationMessage *pFrameworkObjectManagerSyncConfigurationMessage)
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::resetStartupFileStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::displayConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::setInMemoryConfiguration),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::updatePrismConfigurationFileStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismFrameworkConfigurationWorker::prismSynchronousLinearSequencerFailedStep)
    };

    string prismConfigurationFile = pFrameworkObjectManagerSyncConfigurationMessage->getPrismFrameworkConfigurationFileName ();

    PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext = new PrismFrameworkConfigurationContext (pFrameworkObjectManagerSyncConfigurationMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), prismConfigurationFile);

    // set cfg object in context

    PrismFrameworkConfiguration &prismFrameworkConfigurationObject = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();

    UI32 sizeOfBuffer = 0;

    char *serialzedConfigurationObject = reinterpret_cast<char *> (pFrameworkObjectManagerSyncConfigurationMessage->findBuffer (CFG_OBJECT, sizeOfBuffer));

    string serializedData (serialzedConfigurationObject);

    prismFrameworkConfigurationObject.loadFromSerializedData2 (serializedData);

    ResourceId status = pPrismFrameworkConfigurationContext->execute ();

    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::updatePrismConfigurationFileStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{

    ResourceId  status              = WAVE_MESSAGE_SUCCESS;
    UI32        sizeOfBuffer        = 0;
    bool        cfgValidity         = false;

    FrameworkObjectManagerSyncConfigurationMessage *pMessage = reinterpret_cast<FrameworkObjectManagerSyncConfigurationMessage *> (pPrismFrameworkConfigurationContext->getPPrismMessage ());
    
    bool *cfgValidityPtr = reinterpret_cast<bool *> (pMessage->findBuffer (CFG_VALIDITY, sizeOfBuffer));

    if (NULL == cfgValidityPtr)
    {
        // Normally we shouldn't come here because in live sync, both MMs should have same firmware version.
        // To be safe, reset cfg validity.

        cfgValidity = true;
    }
    else
    {
        cfgValidity = (*cfgValidityPtr);
    }

    if (false == cfgValidity)
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkConfigurationWorker::updatePrismConfigurationFileStep: remove prism configuration file.");
        status = FrameworkToolKit::changePrismConfigurationValidity (false);
    }
    else
    {      
        trace (TRACE_LEVEL_INFO, "PrismFrameworkConfigurationWorker::updatePrismConfigurationFileStep: save prism configuration file.");
        status = FrameworkToolKit::changePrismConfigurationValidity (true);
    }
    
    return (status);
} 

ResourceId PrismFrameworkConfigurationWorker::resetStartupFileStep (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    PrismFrameworkConfiguration &prismFrameworkConfigurationObject = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();

    prismFrameworkConfigurationObject.setIsStartupValid (FrameworkToolKit::getIsStartupValid ());
    prismFrameworkConfigurationObject.setStartupFileName (FrameworkToolKit::getStartupFileName ());
    prismFrameworkConfigurationObject.setStartupFileType (FrameworkToolKit::getStartupFileType ());

    return (status);
}

ResourceId PrismFrameworkConfigurationWorker::setInMemoryConfiguration (PrismFrameworkConfigurationContext *pPrismFrameworkConfigurationContext)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    PrismFrameworkConfiguration &prismFrameworkConfiguration = pPrismFrameworkConfigurationContext->getPrismFrameworkConfigurationObject ();

    LocationBase *pThisLocation = (PrismFrameworkObjectManager::getInstance ())->getThisLocation ();

    prismAssert (NULL != pThisLocation, __FILE__, __LINE__);

    PrismFrameworkObjectManager::updateIpAddressForThisLocation (prismFrameworkConfiguration.getThisLocationIpAddress ());

    FrameworkToolKit::setLastUsedLocationId (prismFrameworkConfiguration.getLastUsedLocationId ());

    pThisLocation->setLocationRole             (static_cast<LocationRole> (prismFrameworkConfiguration.getThisLocationRole ()));
    pThisLocation->setLocationId               (prismFrameworkConfiguration.getThisLocationLocationId ());
    pThisLocation->setClusterPrimaryLocationId (prismFrameworkConfiguration.getClusterPrimaryLocationId ());
    pThisLocation->setClusterPrimaryPort       (prismFrameworkConfiguration.getClusterPrimaryPort ());

    UI32 numberOfKnownLocations = prismFrameworkConfiguration.getKnownLocationIpAddresses ().size ();
    UI32 i                      = 0;

    pThisLocation->removeAllKnownLocations ();

    for (i = 0; i < numberOfKnownLocations; i++)
    {
        if (pThisLocation->isAKnownLocation ((prismFrameworkConfiguration.getKnownLocationLocationIds ())[i]) != true)
        {
            pThisLocation->addKnownLocation ((prismFrameworkConfiguration.getKnownLocationLocationIds ())[i], (prismFrameworkConfiguration.getKnownLocationIpAddresses ())[i], (prismFrameworkConfiguration.getKnownLocationPorts ())[i]);
        }
    }

    UI32 numberOfKnownSubLocations = prismFrameworkConfiguration.getKnownSubLocationIpAddress ().size ();

    for (i = 0; i < numberOfKnownSubLocations; i++)
    {
        pThisLocation->addSubLocation ((prismFrameworkConfiguration.getKnownSubLocationLocationIds ())[i], (prismFrameworkConfiguration.getKnownSubLocationIpAddress ())[i], (prismFrameworkConfiguration.getKnownSubLocationPorts ())[i]);
    }

    status = WAVE_MESSAGE_SUCCESS;

    return (status);
}

}
