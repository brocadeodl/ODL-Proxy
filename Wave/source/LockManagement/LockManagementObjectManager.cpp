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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "LockManagement/LockManagementObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "LockManagement/LockManagementObjectManagerTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "LockManagement/LockManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Shell/ShellDebug.h"

namespace WaveNs
{

LockManagementObjectManager::LockManagementObjectManager ()
    : WaveObjectManager (getClassName ())
{
    LockManagedObject lockManagedObject (this);
    lockManagedObject.setupOrm ();
    
    addManagedClass (LockManagedObject::getClassName ());

    addOperationMap (LOCK_MANAGEMENT_OBJECT_MANAGER_ACQUIRE_LOCK, reinterpret_cast<PrismMessageHandler> (&LockManagementObjectManager::executeAcquireLockMessageHandler));
    addOperationMap (LOCK_MANAGEMENT_OBJECT_MANAGER_RELEASE_LOCK, reinterpret_cast<PrismMessageHandler> (&LockManagementObjectManager::executeReleaseLockMessageHandler));
}

LockManagementObjectManager::~LockManagementObjectManager ()
{
}

string LockManagementObjectManager::getClassName ()
{
    return ("Lock Management Service");
}

LockManagementObjectManager *LockManagementObjectManager::getInstance ()
{
    static LockManagementObjectManager *pLockManagementObjectManager = new LockManagementObjectManager ();

    WaveNs::prismAssert (NULL != pLockManagementObjectManager, __FILE__, __LINE__);

    return (pLockManagementObjectManager);
}

PrismServiceId LockManagementObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

WaveManagedObject *LockManagementObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((LockManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new LockManagedObject(this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "LockManagementObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

PrismMessage *LockManagementObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case LOCK_MANAGEMENT_OBJECT_MANAGER_ACQUIRE_LOCK:
            pPrismMessage = new LockManagementObjectManagerAcquireLockMessage (); 
            break;

        case LOCK_MANAGEMENT_OBJECT_MANAGER_RELEASE_LOCK:
            pPrismMessage = new LockManagementObjectManagerReleaseLockMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("LockManagementObjectManager::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (pPrismMessage);
}

void LockManagementObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
    ResourceId     status     = WAVE_MESSAGE_SUCCESS;

    if ((bootReason != WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT) && (bootReason != WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT))
    {
        vector<WaveManagedObject *> *pResults = querySynchronously (LockManagedObject::getClassName ());

        prismAssert (NULL != pResults, __FILE__, __LINE__);
        
        UI32 numberOfLocks = pResults->size ();

        if (numberOfLocks > 0)
        {
            startTransaction ();
    
            for (UI32 i = 0; i < numberOfLocks; i++)
            {
                delete ((*pResults)[i]);
            }

            status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "Not able to Remove Lock Managed Object, System is in inconsistent state");
            }
            else
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
        }

        pResults->clear ();
        delete pResults;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void LockManagementObjectManager::executeAcquireLockMessageHandler (LockManagementObjectManagerAcquireLockMessage *pLockManagementObjectManagerAcquireLockMessage)
{
    trace (TRACE_LEVEL_INFO, "LockManagementObjectManager::executeAcquireLockMessageHandler : Entering...");

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::validateAcquireLockStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::setServiceStringFromAcquireMessageStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::createLockManagedObjectInDataBaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    LockManagementMessagingContext *pLockManagementMessagingContext = new LockManagementMessagingContext (reinterpret_cast<PrismMessage*> (pLockManagementObjectManagerAcquireLockMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pLockManagementMessagingContext->execute ();
    trace (TRACE_LEVEL_INFO, "LockManagementObjectManager::executeAcquireLockMessageHandler : Finished acquire message processing");
}

ResourceId LockManagementObjectManager::validateAcquireLockStep (LockManagementMessagingContext *pLockManagementMessagingContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    //Check if the node is ready to accept all commands.
    if((false == FrameworkToolKit::isNodeReadyForAllCommands()) && (!FrameworkToolKit::isConfigReplayInProgress ()))
    {
        status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_ERROR, "LockManagementObjectManager::validateAcquireLockStep : Node is not ready for all commands");
    }

    return (status);
}

ResourceId LockManagementObjectManager::setServiceStringFromAcquireMessageStep (LockManagementMessagingContext *pLockManagementMessagingContext)
{
    LockManagementObjectManagerAcquireLockMessage  *pLockManagementObjectManagerAcquireLockMessage = reinterpret_cast<LockManagementObjectManagerAcquireLockMessage *> (pLockManagementMessagingContext->getPPrismMessage ());
    string                                          serviceString = pLockManagementObjectManagerAcquireLockMessage->getServiceString ();
    
    pLockManagementMessagingContext->setServiceString (serviceString);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep (LockManagementMessagingContext *pLockManagementMessagingContext)
{
    WaveManagedObjectSynchronousQueryContext        synchronousQueryContext (LockManagedObject::getClassName ());
    ResourceId                                      status = WAVE_MESSAGE_SUCCESS;
    string                                          serviceString = pLockManagementMessagingContext->getServiceString ();
    
    synchronousQueryContext.addAndAttribute (new AttributeString (serviceString, "serviceString"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_INFO, string ("LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep : Cannot find lock with serviceString = ") + serviceString);
        }
        else if (1 == numberOfResults)
        {
            trace (TRACE_LEVEL_INFO, string ("LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep : Lock present for serviceString =") + serviceString);
            LockManagedObject *pLockManagedObject = dynamic_cast<LockManagedObject *> ((*pResults)[0]);
            
            if (pLockManagedObject)
            {
                pLockManagementMessagingContext->setPLockManagedObject (dynamic_cast<LockManagedObject *> ((*pResults)[0]));
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, string ("LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep: No element found from the query"));
                prismAssert (false, __FILE__, __LINE__);
            }

        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep: There can be only one lock #no. of locks found = ")+numberOfResults+ "for serviceString = " + serviceString);
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep : System failure pResults is NULL");
        prismAssert (false, __FILE__, __LINE__);
    }
 
    pResults->clear ();
    delete pResults;

    return (status);
}

ResourceId LockManagementObjectManager::createLockManagedObjectInDataBaseStep (LockManagementMessagingContext *pLockManagementMessagingContext)
{
    ResourceId                                      status = WAVE_MESSAGE_ERROR;
    LockManagementObjectManagerAcquireLockMessage  *pLockManagementObjectManagerAcquireLockMessage = reinterpret_cast<LockManagementObjectManagerAcquireLockMessage *> (pLockManagementMessagingContext->getPPrismMessage ());
    string                                          serviceString = pLockManagementObjectManagerAcquireLockMessage->getServiceString ();
    LocationId                                      locationId = pLockManagementObjectManagerAcquireLockMessage->getSenderLocationId ();
    PrismServiceId                                  serviceId = pLockManagementObjectManagerAcquireLockMessage->getSenderServiceCode ();

    if (NULL == pLockManagementMessagingContext->getPLockManagedObject ())
    {

        startTransaction ();

        LockManagedObject *pLockManagedObject = new LockManagedObject (this, locationId, serviceString, serviceId);

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "LockManagementObjectManager::createLockManagedObjectInDataBaseStep: Could not create LockManagedObject in database");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("LockManagementObjectManager::createLockManagedObjectInDataBaseStep: Lock successfully acquired for serviceString = ") + serviceString);
            status = WAVE_MESSAGE_SUCCESS;
        }

        if (pLockManagedObject)
        {
            delete (pLockManagedObject);
            pLockManagedObject = NULL;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LockManagementObjectManager::createLockManagedObjectInDataBaseStep:Could not allocate memory");
            prismAssert (false, __FILE__, __LINE__);
        }
        

    }
    else 
    {
        trace (TRACE_LEVEL_INFO, string ("LockManagementObjectManager::createLockManagedObjectInDataBaseStep: Lock already acquired for serviceString = ") + serviceString);
    }

    return (status);
}

void LockManagementObjectManager::executeReleaseLockMessageHandler (LockManagementObjectManagerReleaseLockMessage *pLockManagementObjectManagerReleaseLockMessage)
{
    trace (TRACE_LEVEL_INFO, "LockManagementObjectManager::executeReleaseLockMessageHandler: Entering...");

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::setServiceStringFromReleaseMessageStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::queryLockManagedObjectFromServiceStringStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::deleteLockManagedObjectFromDataBaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&LockManagementObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    LockManagementMessagingContext *pLockManagementMessagingContext = new LockManagementMessagingContext (reinterpret_cast<PrismMessage*> (pLockManagementObjectManagerReleaseLockMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pLockManagementMessagingContext->execute ();
    trace (TRACE_LEVEL_INFO, "LockManagementObjectManager::executeReleaseLockMessageHandler: Finished release message processing");

}

ResourceId LockManagementObjectManager::setServiceStringFromReleaseMessageStep (LockManagementMessagingContext *pLockManagementMessagingContext)
{
    LockManagementObjectManagerReleaseLockMessage  *pLockManagementObjectManagerReleaseLockMessage = reinterpret_cast<LockManagementObjectManagerReleaseLockMessage *> (pLockManagementMessagingContext->getPPrismMessage ());
    string                                          serviceString = pLockManagementObjectManagerReleaseLockMessage->getServiceString ();

    pLockManagementMessagingContext->setServiceString (serviceString);
    
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId LockManagementObjectManager::deleteLockManagedObjectFromDataBaseStep (LockManagementMessagingContext *pLockManagementMessagingContext)
{
    ResourceId                                      status = WAVE_MESSAGE_ERROR;
    LockManagementObjectManagerReleaseLockMessage  *pLockManagementObjectManagerReleaseLockMessage = reinterpret_cast<LockManagementObjectManagerReleaseLockMessage *> (pLockManagementMessagingContext->getPPrismMessage ());
    string                                          serviceString = pLockManagementObjectManagerReleaseLockMessage->getServiceString ();
    LockManagedObject                              *pLockManagedObject = pLockManagementMessagingContext->getPLockManagedObject ();

    if (pLockManagedObject)
    {
        startTransaction ();

        delete (pLockManagedObject);

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("LockManagementObjectManager::deleteLockManagedObjectFromDataBaseStep: Could not delete LockManagedObject in database with serviceString = ") + serviceString);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("LockManagementObjectManager::deleteLockManagedObjectFromDataBaseStep: Lock successfully released for serviceString = ") + serviceString);
            status = WAVE_MESSAGE_SUCCESS;
        }

        //Since the LockManagedObject is deleted set the pointer in context to NULL
        //Here destructor will not help as the MO has already been deleted as part of deletion from the database, but the MO pointer in the context is still not set to NULL
        //Do not remove this will lead to double deletion.
        pLockManagementMessagingContext->setPLockManagedObject (NULL);
    }

    return (status);
}

void LockManagementObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId                      status = WAVE_MESSAGE_ERROR;
    UI32                            noOfLocks = 0;

    // query Lock Management Managed Object from DB and save in memory
    vector<WaveManagedObject *> *pResults = querySynchronously (LockManagedObject::getClassName ());

    if (pResults)
    {
        noOfLocks = pResults->size ();

        for (UI32 lockNumber = 0; lockNumber < noOfLocks; lockNumber++)
        {
            LockManagedObject *pLockManagedObject = dynamic_cast<LockManagedObject *> ((*pResults)[lockNumber]);
			prismAssert (NULL != pLockManagedObject, __FILE__, __LINE__);
			
            for (UI32 failedLocationNumber = 0; failedLocationNumber < failedLocationIds.size (); failedLocationNumber++)
            {
                if (failedLocationIds[failedLocationNumber] == pLockManagedObject->getLocationId ())
                {
                    startTransaction ();

                    delete (pLockManagedObject);
                    pLockManagedObject = NULL;

                    status = commitTransaction ();

                    if (FRAMEWORK_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_FATAL, string ("LockManagementObjectManager::failover: Could not delete LockManagedObject in database with Location id =") + failedLocationIds[failedLocationNumber]);
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, string ("LockManagementObjectManager::failover: Deleted the lock from database with locationId = ") + failedLocationIds[failedLocationNumber]);

                        // Since the pointer has already been deleted set this to NULL
                        (*pResults)[lockNumber] = NULL;
                        
                        // The lock in the database has been deleted because the node has failed over.
                        // break here to check the next row in the table.
                        break;
                    }
                }
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("LockManagementObjectManager::failover: System failure. pResults is NULL"));
        prismAssert (false, __FILE__, __LINE__);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pFailoverAsynchronousContext->callback ();    
}

}
