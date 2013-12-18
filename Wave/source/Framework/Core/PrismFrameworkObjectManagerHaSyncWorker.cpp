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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Leifang Hu                                             *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkObjectManagerHaSyncWorker.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Core/VersionInformation.h"
#include "Framework/Core/VcsClusterConfiguration.h"
#include "Framework/Core/StartupFileConfiguration.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManagerBackupMessage.h"
#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "Framework/Database/DatabaseObjectManagerRestoreMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Core/Wave.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/LocationManagement/Location.h"
#include "Framework/Shutdown/PrismFinalizeWorker.h"
#include "Framework/Utils/TraceUtils.h"
#include "Version/PrismVersion.h"
#include "Framework/Messaging/HaPeer/HaPeerMessageTransportObjectManager.h"
#include "Framework/Messaging/HaPeer/HaPeerMessageReceiverObjectManager.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerUpdateInstanceIdMesssage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerLiveSyncEnableMessages.h"
#include "Framework/ObjectRelationalMapping/DatabaseSchema.h"
#include "Framework/ObjectRelationalMapping/OrmToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"

#include <cerrno>
#include <sstream>

namespace WaveNs
{

static UI32                   s_offSetForHaSyncValidationResults                                     = 0x0000FFFF;

PrismFrameworkObjectManagerHaSyncWorker::PrismFrameworkObjectManagerHaSyncWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager), 
      m_firmwareVersion (""), 
      m_syncDumpComplete (false),
      m_firmwareVersionMatch (false),
      m_syncState (OUT_OF_SYNC),
      m_standbySyncState (OUT_OF_SYNC),
      m_syncServicedAsStandby (false),
      m_isDbDropNeeded (false),
      m_dbSchemaType (NOT_CONVERTIBLE_SCHEMA),
      m_myHaVersion (RUNNING_CFG_SYNC_SAME_VERSION),
      m_peerHaVersion (UNKNOWN_VERSION)
{
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_START_HA_SYNC_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::startHaSyncDumpHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_INIT_HA_IPADDR_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::initHaIpAddressHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_HA_SYNC_CONFIGURE_STANDBY, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_HA_SYNC_UPDATE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::startHaSyncUpdateHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_GET_FIRMWARE_VERSION, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::getFirmwareVersionHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_END_HA_SYNC_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::endHaSyncHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_START_CCMD_HA_SYNC_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::startCcmdHaSyncDumpHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CCMD_HA_SYNC_UPDATE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::startCcmdHaSyncUpdateHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_SET_SYNC_STATE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::setSyncStateHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_GET_SYNC_STATE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerHaSyncWorker::getSyncStateHandler));
}

PrismFrameworkObjectManagerHaSyncWorker::~PrismFrameworkObjectManagerHaSyncWorker ()
{
}

PrismFrameworkObjectManagerHaSyncWorker *PrismFrameworkObjectManagerHaSyncWorker::getInstance ()
{
    static PrismFrameworkObjectManagerHaSyncWorker *pPrismFrameworkObjectManagerHaSyncWorker = (PrismFrameworkObjectManager::getInstance ())->m_pPrismFrameworkObjectManagerHaSyncWorker;
    WaveNs::prismAssert (NULL != pPrismFrameworkObjectManagerHaSyncWorker, __FILE__, __LINE__);
    return pPrismFrameworkObjectManagerHaSyncWorker;
}

PrismMessage *PrismFrameworkObjectManagerHaSyncWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case FRAMEWORK_OBJECT_MANAGER_HA_SYNC_CONFIGURE_STANDBY:
            pPrismMessage = new PrismHaSyncConfigureStandbyMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_HA_SYNC_UPDATE :
            pPrismMessage = new FrameworkObjectManagerHaSyncUpdateMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CCMD_HA_SYNC_UPDATE_MESSAGE :
            pPrismMessage = new FrameworkObjectManagerCcmdHaSyncUpdateMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_GET_FIRMWARE_VERSION :
            pPrismMessage = new FrameworkObjectManagerGetFirmwareVersionMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}


ResourceId PrismFrameworkObjectManagerHaSyncWorker::connectToHaPeer (const string &peerIpAddress, const SI32 &port)
{
    static const UI32 maximumNumberOfRetriesToConnectToARemoteLocation  = 10;
    static const UI32 maximumNumberOfSecondsToWaitToConnect             = 10;

	PrismFrameworkObjectManager *pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
    LocationBase *pThisLocation = pPrismFrameworkObjectManager->getThisLocation ();

    SI32 status = pThisLocation->connectToHaPeer (peerIpAddress, port, maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect);
    return (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::initializeHaPeerServerCommunications ()
{
    static const UI32 retryCount = 120;
                 UI32 i          = 0;

    PrismFrameworkObjectManager* pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
    LocationBase *pThisLocation = pPrismFrameworkObjectManager->getThisLocation ();

    if (NULL != pThisLocation)
    {
        ServerStreamingSocketStatus status = SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_BIND;

        while (SERVER_STREAMING_SOCKET_SUCCESS != status)
        {
            status = pThisLocation->initializeHaPeerServerCommunications ();
            i++;

            if (SERVER_STREAMING_SOCKET_SUCCESS != status)
            {
                if (i >= retryCount)
                {
                    if (SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_BIND == status)
                    {
                        trace (TRACE_LEVEL_ERROR, "Could not bind to Local TCP/IP Socket.  Please make sure that there is no other instance of this program is currently active.");
                    }
                    else if (SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_LISTEN == status)
                    {
                        trace (TRACE_LEVEL_ERROR, "Could not listen on a Local TCP/IP Socket.  Please make sure that there is no other instance of this programis currently active.");
                    }
                    else
                    {
                        trace (TRACE_LEVEL_ERROR, "Server Socket error.  Please make sure that there is no other instance of this program is currently active.");
                    }

                    // For now, if we cannot start Server communications we exit.

                    exit (2);
                }
                else
                {
                    trace (TRACE_LEVEL_WARN, "Still Waiting for TCP/IP Layer to allow us to bind on Server Socket ...");

                    prismSleep (1);
                }
            }
            else
            {
                trace (TRACE_LEVEL_SUCCESS, "Succeeded in binding on Server Socket.");
                return;
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::initializeHaPeerServerCommunications : This Locations is not configured yet.");

        prismAssert (false, __FILE__, __LINE__);
    }
}

bool PrismFrameworkObjectManagerHaSyncWorker::acceptHaPeerConnection (ServerStreamingSocket &newSocket)
{
    bool successfullyAcceptedNewConnection = false;

    PrismFrameworkObjectManager* pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
    LocationBase *pThisLocation = pPrismFrameworkObjectManager->getThisLocation ();

    if (NULL != pThisLocation)
    {
        successfullyAcceptedNewConnection = pThisLocation->acceptHaPeerConnection (newSocket);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::acceptHaPeerConnection : This Locations is not configured yet.");

        prismAssert (false, __FILE__, __LINE__);
    }

    return (successfullyAcceptedNewConnection);
}

void PrismFrameworkObjectManagerHaSyncWorker::disconnectFromHaPeer (const string &ipAddress, const SI32 &port, const bool &closePeerServerSocket)
{
    PrismFrameworkObjectManager *pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
    LocationBase                *pThisLocation                = pPrismFrameworkObjectManager->getThisLocation ();

    if (closePeerServerSocket)
    {
        HaPeerMessageReceiverObjectManager::closePeerServerStreamingSocketAndRemoveHaPeerMessageReceiverThreadFromCache (ipAddress, port);
    }
    pThisLocation->invalidateClientStreamingSocketForHaPeer ();

    (HaPeerMessageTransportObjectManager::getInstance ())->replyToRemoteMessagesPendingOnLocation (1, WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
}

void PrismFrameworkObjectManagerHaSyncWorker::initHaIpAddressHandler (FrameworkObjectManagerInitHaIpAddressMessage *pFrameworkObjectManagerInitHaIpAddressMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::initHaIpAddressStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFrameworkObjectManagerInitHaIpAddressMessage, this, sequencerSteps,sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void PrismFrameworkObjectManagerHaSyncWorker::initHaIpAddressStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: initHaIpAddressStep");

    FrameworkObjectManagerInitHaIpAddressMessage *pFrameworkObjectManagerInitHaIpAddressMessage = dynamic_cast<FrameworkObjectManagerInitHaIpAddressMessage *>(pPrismLinearSequencerContext->getPPrismMessage());

    string localHaIp = pFrameworkObjectManagerInitHaIpAddressMessage->getLocalHaIpAddress();

    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager:: local HA Ip:") + localHaIp);

    PrismFrameworkObjectManager* pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
    LocationBase *pThisLocation = pPrismFrameworkObjectManager->getThisLocation ();

    pThisLocation->resetIpAddressForHaInterface(localHaIp);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::startHaSyncDumpHandler (FrameworkObjectManagerStartHaSyncMessage *pFrameworkObjectManagerStartHaSyncMessage)
{

    if (FrameworkToolKit::getSecondaryClusterFormationFlag ())
    {
        if (true == (PrismFrameworkObjectManager::getInstance ())->getNeedNotifyClusterReadyState ())
        {
            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::startHaSyncDumpHandler: Need to restart sync dump after cluster formation complete. Skip this sync start trigger."));
            reply (pFrameworkObjectManagerStartHaSyncMessage);
            return;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::startHaSyncDumpHandler: Postponing HA sync as cluster formation is in progress"));
            postponeMessageHandling (pFrameworkObjectManagerStartHaSyncMessage);
            return;
        }
    }

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::connectToHaPeerStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCollectValidationDataStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateStandbyMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetVcsClusterDataStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetConfigurationFileStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::setSyncCompletionStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncFailedStep),
    };

    StartHaSyncDumpContext *pStartHaSyncDumpContext = new StartHaSyncDumpContext (pFrameworkObjectManagerStartHaSyncMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pStartHaSyncDumpContext->setHaPeerIp (pFrameworkObjectManagerStartHaSyncMessage->getPeerHaIpAddress ());
    pStartHaSyncDumpContext->setHaPeerPort (pFrameworkObjectManagerStartHaSyncMessage->getPeerHaIpPort ());

    trace (TRACE_LEVEL_PERF_START, "HA Sync Dump.");

    pStartHaSyncDumpContext->holdAll ();
    pStartHaSyncDumpContext->start ();

}

void PrismFrameworkObjectManagerHaSyncWorker::connectToHaPeerStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    SI32 status = FRAMEWORK_SUCCESS;
    ResourceId ret = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: connectToHaPeerStep");

    FrameworkObjectManagerStartHaSyncMessage *pFrameworkObjectManagerStartHaSyncMessage = dynamic_cast<FrameworkObjectManagerStartHaSyncMessage *>(pStartHaSyncDumpContext->getPPrismMessage());

    // Set the context 
    UI32 sizeOfBuffer = 0;
    UI32 * pContext = reinterpret_cast<UI32 *> (pFrameworkObjectManagerStartHaSyncMessage->findBuffer (CONTEXT_INFO, sizeOfBuffer));

    if (pContext != NULL)
    {
        pStartHaSyncDumpContext->setContextInfo (*pContext);
    }
    else
    {
        pStartHaSyncDumpContext->setContextInfo (UNKNOWN_CONTEXT_INFO);
    }

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::connectToHaPeerStep : context = ") + pStartHaSyncDumpContext->getContextInfo ());

    // Set the sync state to OUT_OF_SYNC at the beginning of the sync operation
    setSyncState (OUT_OF_SYNC);

    string peerHaIp = pFrameworkObjectManagerStartHaSyncMessage->getPeerHaIpAddress();
    SI32 peerHaPort = pFrameworkObjectManagerStartHaSyncMessage->getPeerHaIpPort();

    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManagerHaSyncWorker:: peer HA Ip:") + peerHaIp + string(" port:") + peerHaPort);

    disconnectFromHaPeer (peerHaIp, peerHaPort, true);

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker:: connectToHaPeer - disconnect from peer HA Ip:") + peerHaIp + string(" port:") + peerHaPort);

    if ((status = connectToHaPeer(peerHaIp, peerHaPort)) != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: connectToHaPeer failed");

        ret = WAVE_MESSAGE_ERROR;
    }
    pStartHaSyncDumpContext->executeNextStep (ret);
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncCollectValidationDataStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::collectHaSyncValidationDataStep");

    vector<PrismServiceId>  prismServiceIds;
    UI32                    numberOfPrismServiceIds;
    UI32                    i;
    ResourceId              status                       = WAVE_MESSAGE_SUCCESS;
    vector<PrismServiceId> &prismServiceIdsToCommunicate = pStartHaSyncDumpContext->getPrismServiceIdsToCommunicate ();

    PrismThread::getListOfServiceIds (prismServiceIds);
    numberOfPrismServiceIds = prismServiceIds.size ();

    for (i = 0; i < numberOfPrismServiceIds; i++)
    {
        trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::haSyncCollectHaSyncValidationDataStep : Collecting Validation Information from Service : ") + prismServiceIds[i]);

        if (true == (isServiceToBeExcludedInHaSyncCommunications (prismServiceIds[i])))
        {
            continue;
        }

        // Collect the list of services so that we use the list later.

        prismServiceIdsToCommunicate.push_back (prismServiceIds[i]);

        WaveObjectManagerHaSyncCollectValidationDataMessage message (prismServiceIds[i]);

        status  = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_COULD_NOT_COLLECT_VALIDATION_DATA;

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::CollectHaSyncValidationDataStep : Failed to obtain Validation Details for Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Status : " + FrameworkToolKit::localize (status));

            break;
        }

        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_COULD_NOT_COLLECT_VALIDATION_DATA;

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::CollectHaSyncValidationDataStep : Failed to obtain Validation Details for Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Completion Status : " + FrameworkToolKit::localize (status));

            break;
        }

        void *pData = NULL;
        UI32  size  = 0;

        message.getValidationDetails (pData, size);

        if ((0 != size) && (NULL != pData))
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::CollectHaSyncValidationDataStep: Obtained Validation Data for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            pStartHaSyncDumpContext->addValidationDetailsForService (prismServiceIds[i], pData, size);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::CollectHaSyncValidationDataStep : No Validation details are obtained for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));
        }
    }

    pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionStep");

    FrameworkObjectManagerGetFirmwareVersionMessage * pFrameworkObjectManagerGetFirmwareVersionMessage = new FrameworkObjectManagerGetFirmwareVersionMessage ();

    string firmwareVersion = PrismFrameworkObjectManager::getInstance ()->getFirmwareVersion ();
    LocationId haPeerLocationId = 1;

    pFrameworkObjectManagerGetFirmwareVersionMessage->addBuffer (FIRMWARE_VERSION, firmwareVersion.size (), (void*) &firmwareVersion, false);

    // Pass the database schema information to the standby MM
    DatabaseSchema databaseSchema;
    string serializedData;

    if (FRAMEWORK_SUCCESS != (OrmToolKit::getDatabaseSchema (databaseSchema)))
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionStep: failed to get databaseSchema object from the db.")); 
        pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR);    
        return;
    }

    string peerHaIp = pStartHaSyncDumpContext->getHaPeerIp ();
    SI32 peerHaPort = pStartHaSyncDumpContext->getHaPeerPort();

    UI8 serializationType = FrameworkToolKit::getSerializationTypeForHaPeer (peerHaIp, peerHaPort);
    databaseSchema.serialize2 (serializedData, serializationType);

    pFrameworkObjectManagerGetFirmwareVersionMessage->addBuffer (ACTIVE_DB_SCHEMA_OBJECT, serializedData.size (), (void *) (serializedData.c_str ()), false);

    UI32 haSyncVersion = m_myHaVersion;
    pFrameworkObjectManagerGetFirmwareVersionMessage->addBuffer (ACTIVE_HA_SYNC_VERSION, sizeof (UI32), (void *) &haSyncVersion, false); 

    WaveMessageStatus status = send (pFrameworkObjectManagerGetFirmwareVersionMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback), pStartHaSyncDumpContext, 60000, haPeerLocationId);

    if (status != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionStep: failed to get firmware version from standby MM : ") + FrameworkToolKit::localize (status));

        pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR_HAPEER_MESSAGE_FAILED);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionStep: succeeded in getting firmware version from standby MM");
    }
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerGetFirmwareVersionMessage *pFrameworkObjectManagerGetFirmwareVersionMessage, void *pContext) 
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback");

    StartHaSyncDumpContext *pStartHaSyncDumpContext = reinterpret_cast<StartHaSyncDumpContext *> (pContext);

    if (NULL != pFrameworkObjectManagerGetFirmwareVersionMessage)
    {
        if (FRAMEWORK_SUCCESS != frameworkStatus)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback : PrismHaSyncConfigureStandbyMessage Completion Status : " + FrameworkToolKit::localize (frameworkStatus));

            pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR_HAPEER_MESSAGE_FAILED);
            return;
        }

            // if the firmware version query times out, reply that sync dump failed
        ResourceId status = pFrameworkObjectManagerGetFirmwareVersionMessage->getCompletionStatus ();

        if (status != WAVE_MESSAGE_SUCCESS)
        {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback: Failed to retrieve firmware version from standby MM");

                pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR_STANDBY_SYNC_FAILED);
                return;
        }

        string firmwareVersionFromActive = PrismFrameworkObjectManager::getInstance ()->getFirmwareVersion ();

        if (firmwareVersionFromActive.compare (pFrameworkObjectManagerGetFirmwareVersionMessage->getFirmwareVersion ()) == 0)
        {
            setFirmwareVersionMatch (true);

            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback: firmware version from standby MM matches");
        }
        else
        {
            setFirmwareVersionMatch (false);

            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback: firmware version from standby MM does not match. Standby firmware version: " + pFrameworkObjectManagerGetFirmwareVersionMessage->getFirmwareVersion ());
        }

        // Get the database dump type 
        UI32 sizeOfBuffer = 0;
        UI32 * pDbDumpType = reinterpret_cast<UI32 *> (pFrameworkObjectManagerGetFirmwareVersionMessage->findBuffer (DB_DUMP_TYPE, sizeOfBuffer));
    
        if (pDbDumpType != NULL)
        {
            m_dbSchemaType = *pDbDumpType;

            if (m_dbSchemaType == CONVERTIBLE_SCHEMA)
            {
                m_isDbDropNeeded = true;
            }
            else
            {
                m_isDbDropNeeded = false;
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionCallback: database dump type not found");

            m_isDbDropNeeded = false;
            m_dbSchemaType = NOT_CONVERTIBLE_SCHEMA;
            m_peerHaVersion = STARTUP_CFG_SYNC;
        }

        sizeOfBuffer = 0;
        UI32 * pPeerHaVersion = reinterpret_cast<UI32 *> (pFrameworkObjectManagerGetFirmwareVersionMessage->findBuffer (STANDBY_HA_SYNC_VERSION, sizeOfBuffer));
        if (pPeerHaVersion != NULL)
        {
            m_peerHaVersion = *pPeerHaVersion;
        }
        else
        {
            m_peerHaVersion = UNKNOWN_VERSION;
        }

        delete (pFrameworkObjectManagerGetFirmwareVersionMessage);
    }

    // DB Sync is always part of Sync Dump, but only if the firmware version matches or the database schema is convertible
    if (((pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_CONTEXT) && (true == getFirmwareVersionMatch())) ||
        ((pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_ISSU_CONTEXT) && (m_dbSchemaType != NOT_CONVERTIBLE_SCHEMA)))
    {
        // Set the context from a regular sync dump to a blocking sync dump (configuration
        // is blocked and the DB is paused on the active MM during the sync dump) for
        // chassis firmwaredownload and sync dumps with matching firmware version 
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::Same firmware version or ISSU DB convertible, dcmd running DB sync");

        pStartHaSyncDumpContext->setIsDbSyncRequired (true);
        pStartHaSyncDumpContext->setContextInfo (SYNC_DUMP_WITH_PAUSE_DB_CONTEXT);
    }
    else if ((pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_CONTEXT) && (false == getFirmwareVersionMatch()) && (m_dbSchemaType != NOT_CONVERTIBLE_SCHEMA))
    {
         trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::Sync dump firmware verison diff but DB convertible, dcmd snapshot DB sync");

         pStartHaSyncDumpContext->setIsDbSyncRequired (true);
    }
    else if (((pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_CONTEXT) && (true == getFirmwareVersionMatch())) ||
             ((pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_ISSU_CONTEXT) && (m_dbSchemaType != NOT_CONVERTIBLE_SCHEMA)))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::Same firmware version or ISSU DB convertible, ccmd running DB sync");

        pStartHaSyncDumpContext->setIsDbSyncRequired (true);
        pStartHaSyncDumpContext->setContextInfo (CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT);
    }
    else if ((pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_CONTEXT) && (false == getFirmwareVersionMatch()) && (m_dbSchemaType != NOT_CONVERTIBLE_SCHEMA))
    {
         trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::Sync dump firmware verison diff but DB convertible, ccmd snapshot DB sync");

         pStartHaSyncDumpContext->setIsDbSyncRequired (true);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: DB not convertible, no DB sync");

        pStartHaSyncDumpContext->setIsDbSyncRequired (false);
        // In case ISSU context, but DB conversion is not feasible, treat as regular SYNC_DUMP_CONTEXT as there is no difference in this case
        if (pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_ISSU_CONTEXT)
        {
            pStartHaSyncDumpContext->setContextInfo (SYNC_DUMP_CONTEXT);
        }
        else if (pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_ISSU_CONTEXT)
        {
            pStartHaSyncDumpContext->setContextInfo (CCMD_SYNC_DUMP_CONTEXT);
        }
    }

    pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateStandbyMessageStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateStandbyMessageStep");

    string prismVersionString = PrismVersion::getVersionString ();
    LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();

    PrismHaSyncConfigureStandbyMessage *pMessage = new PrismHaSyncConfigureStandbyMessage (prismVersionString, locationRole);
    prismAssert (NULL != pMessage, __FILE__, __LINE__);

    UI32 context = pStartHaSyncDumpContext->getContextInfo ();

    pMessage->addBuffer (CONTEXT_INFO, sizeof (UI32), (void *) &context, false);

    pStartHaSyncDumpContext->setStandbyMessage (pMessage);
    pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncGetVcsClusterDataStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    UI32 contextInfo = pStartHaSyncDumpContext->getContextInfo ();

    if ((contextInfo == SYNC_DUMP_CONTEXT) || (contextInfo == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) || (contextInfo == VCS_CLUSTER_CONF_CHANGE_CONTEXT))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetVcsClusterDataStep");

        VcsClusterConfiguration vcsClusterConfiguration;

        PrismFrameworkObjectManager::getInstance ()->getVcsClusterConfigData (&vcsClusterConfiguration, contextInfo);

        PrismHaSyncConfigureStandbyMessage *pMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pStartHaSyncDumpContext->getStandbyMessage ());

        string serializedData;

        string peerHaIp = pStartHaSyncDumpContext->getHaPeerIp();
        SI32 peerHaPort = pStartHaSyncDumpContext->getHaPeerPort();

        UI8 serializationType = FrameworkToolKit::getSerializationTypeForHaPeer (peerHaIp, peerHaPort);
        vcsClusterConfiguration.serialize2 (serializedData, serializationType);

        pMessage->addBuffer (VCS_CONFIG, serializedData.size (), (void *) (serializedData.c_str()), false);
    }

    pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{

    UI32 contextInfo = pStartHaSyncDumpContext->getContextInfo ();

    if ((contextInfo == SYNC_DUMP_CONTEXT) || (contextInfo == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) || (contextInfo == COPY_RUNNING_TO_STARTUP_CONTEXT) || (contextInfo == COPY_DEFAULT_TO_STARTUP_CONTEXT) || (contextInfo == COPY_FILE_TO_STARTUP_CONTEXT))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep");
        
        StartupFileConfiguration startupFileConfiguration;
        int pathsize = 0;
        char fileName [256];
    
        startupFileConfiguration.setStartupFileType (FrameworkToolKit::getStartupFileType ());
    
        pathsize = readlink (FrameworkToolKit::getStartupFileName().c_str(), fileName, sizeof(fileName) - 1);
        if (-1 == pathsize)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep: readlink failed"));
            pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }

        fileName [pathsize] = '\0';
        startupFileConfiguration.setStartupFileName (fileName);
    
        UI32 startupFileType = startupFileConfiguration.getStartupFileType ();
        string startupFileName = startupFileConfiguration.getStartupFileName ();
    
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep: startupFile ") + startupFileName + " " + startupFileType);

        if (startupFileType == WAVE_PERSISTENCE_REGULAR_FILE)
	    {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep: WAVE_PERSISTENCE_REGULAR_FILE ");

            std::ifstream startupFile (startupFileName.c_str ());
            if (startupFile)
            {
               std::stringstream ss;
               // this copies the entire contents of the file into the string stream
               ss << startupFile.rdbuf ();
               // get the string out of the string stream
               string contents = ss.str ();
               startupFileConfiguration.setStartupFile (contents);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep: Couldn't open " + startupFileName);
    
                pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
	    }
        else if (startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep: startup WAVE_PERSISTENCE_DEFAULT_FILE");
        }
        else if (startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep: startup WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE");
        }
	    else
        {
            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep: startupfile type unknown ") + startupFileType);
        }

        PrismHaSyncConfigureStandbyMessage *pMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pStartHaSyncDumpContext->getStandbyMessage ());

        string serializedData;

        string peerHaIp = pStartHaSyncDumpContext->getHaPeerIp();
        SI32 peerHaPort = pStartHaSyncDumpContext->getHaPeerPort();

        UI8 serializationType = FrameworkToolKit::getSerializationTypeForHaPeer (peerHaIp, peerHaPort);
        startupFileConfiguration.serialize2 (serializedData, serializationType);

        pMessage->addBuffer (STARTUP_FILE_OBJECT, serializedData.size (), (void *) (serializedData.c_str()), false);
    }

    pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncGetConfigurationFileStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetConfigurationFileStep");

    PrismFrameworkConfiguration prismFrameworkConfiguration;
    string serializedData;

    prismFrameworkConfiguration.initializeConfiguration ();

    string peerHaIp = pStartHaSyncDumpContext->getHaPeerIp();
    SI32 peerHaPort = pStartHaSyncDumpContext->getHaPeerPort();

    UI8 serializationType = FrameworkToolKit::getSerializationTypeForHaPeer (peerHaIp, peerHaPort);
    prismFrameworkConfiguration.serialize2 (serializedData, serializationType);

    bool cfgValidity = false;

    ResourceId frameworkStatus = FrameworkToolKit::getPrismConfigurationValidity (cfgValidity);

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        pStartHaSyncDumpContext->executeNextStep (frameworkStatus);
        return;
    }

    PrismHaSyncConfigureStandbyMessage *pMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pStartHaSyncDumpContext->getStandbyMessage ());

    pMessage->addBuffer (CFG_OBJECT, serializedData.size (), (void *) (serializedData.c_str()), false);

    pMessage->addBuffer (CFG_VALIDITY, sizeof (bool), (void *) &cfgValidity, false);

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetConfigurationFileStep: Successfully added the cfg file");

    pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateDatabaseDumpStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (pStartHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: createDatabaseDumpStep");
    
        string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName ());

        bool pauseDatabase = false;

        if ((pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) ||
            (pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT))
        {
            pauseLocalPersistence();
            pauseDatabase = true;
        }

        DatabaseObjectManagerBackupMessage message (backupFileName, pauseDatabase);
        message.setBackupAllSchema (true);

        if (m_isDbDropNeeded == false)
        {
            message.setDataOnlyBackup (true);
        }

        status  = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::createDatabaseDumpStep : Could not send message to backup database.  Status : " + FrameworkToolKit::localize (status));

            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            if (pauseDatabase)
            {
                pStartHaSyncDumpContext->setIsDbResumeRequired (true);
            }

            status = message.getCompletionStatus ();
    
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::createDatabaseDumpStep : Message to backup database failed.  Completion Status: " + FrameworkToolKit::localize (status));

                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                 trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createDatabaseDumpStep : Successfully backed up the database.");
            }
        }
    }

    pStartHaSyncDumpContext->executeNextStep (status);
}
 
//  TODO - disabling validation for now
void PrismFrameworkObjectManagerHaSyncWorker::haSyncGetValidationDetailsStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetValidationDetailsStep : Starting ...");

    vector<PrismServiceId> &prismServiceIds                = pStartHaSyncDumpContext->getPrismServiceIdsVector ();
    vector<void *>         &validationDetailsVector        = pStartHaSyncDumpContext->getValidationDetailsVector ();
    vector<UI32>           &validationDetailsSizesVector   = pStartHaSyncDumpContext->getValidationDetailsSizesVector ();

    UI32                   numberOfPrismServiceIds        = prismServiceIds.size ();
    UI32                   numberOfValidationDetails      = validationDetailsVector.size ();
    UI32                   numberOfValidationDetailsSizes = validationDetailsSizesVector.size ();
    UI32                   j                              = 0;

    prismAssert (numberOfPrismServiceIds == numberOfValidationDetails, __FILE__, __LINE__);
    prismAssert (numberOfPrismServiceIds == numberOfValidationDetailsSizes, __FILE__, __LINE__);

    PrismHaSyncConfigureStandbyMessage *pMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pStartHaSyncDumpContext->getStandbyMessage ());

    for (j = 0; j < numberOfPrismServiceIds; j++)
    {
         if ((0 != validationDetailsSizesVector[j]) && (NULL != validationDetailsVector[j]))
         {
              pMessage->addBuffer (prismServiceIds[j], validationDetailsSizesVector[j], validationDetailsVector[j]);
         }
         else
         {
              trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::haSyncGetValidationDetailsStep : We could not have added an entry with 0 size and/or NULL validation buffer.");

              prismAssert (false, __FILE__, __LINE__);
         }
    }
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: sendDatabaseDumpStep");
    SI32        sizeOfBackupFile       = 0;
    char       *pDatabaseBackupBuffer  = NULL;
    LocationId  haPeerLocationId       = 1;

    PrismHaSyncConfigureStandbyMessage *pMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pStartHaSyncDumpContext->getStandbyMessage ());

    // TODO: logic to be completed in next check-in
    if (pStartHaSyncDumpContext->getIsDbSyncRequired ()) 
    {
        createDatabaseBackupBuffer(pDatabaseBackupBuffer, sizeOfBackupFile);

        if (NULL != pDatabaseBackupBuffer)
        {
             pMessage->addBuffer (pMessage->getDatabaseBackupBufferTag (), sizeOfBackupFile, pDatabaseBackupBuffer, false);
             pMessage->addBuffer (DB_DUMP_TYPE, sizeof (UI32), (void *) &m_dbSchemaType, false);
        }
        else
        {
             trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::haSyncSendDatabaseDumpStep : No Database Backup is being sent to the location.");
        }
    }

    WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpCallback), pStartHaSyncDumpContext, 420000, haPeerLocationId);

    if (status != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerHaSyncWorker:: failed to sendDatabaseDump") + FrameworkToolKit::localize (status));

        if (pStartHaSyncDumpContext->getIsDbResumeRequired ())
        {
            resumeDatabase ();
        }

        pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR_HAPEER_MESSAGE_FAILED);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: succeeded to sendDatabaseDump");
    }
}

void PrismFrameworkObjectManagerHaSyncWorker::createDatabaseBackupBuffer(char * &pDatabaseBackupBuffer, SI32 &sizeOfBackupFile)
{
    ifstream                            databaseBackupFile;

    string sBackupFilename = FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + FrameworkToolKit::getDatabaseBackupFileName ();
    databaseBackupFile.open (sBackupFilename.c_str (), ios::binary);
    databaseBackupFile.seekg (0, ios::end);
    sizeOfBackupFile = databaseBackupFile.tellg ();

    prismAssert (0 != sizeOfBackupFile, __FILE__, __LINE__);

    if (0 < sizeOfBackupFile)
    {
        pDatabaseBackupBuffer = new char[sizeOfBackupFile];

        prismAssert (NULL != pDatabaseBackupBuffer, __FILE__, __LINE__);

        databaseBackupFile.seekg (0, ios::beg);
        databaseBackupFile.read (pDatabaseBackupBuffer, sizeOfBackupFile);
    }

    databaseBackupFile.close ();
}

void PrismFrameworkObjectManagerHaSyncWorker::resumeDatabase ()
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::resumeDatabase : Starting ...");

    PrismResumeObjectManagerMessage message (DatabaseObjectManager::getPrismServiceId ());

    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::resumeDatabase : Could not resume Database.  Status : " + FrameworkToolKit::localize (status));

        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::resumeDatabase : Could not resume Database.  Completion Status : " + FrameworkToolKit::localize (status));

            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::resumeDatabase : Successfully resumed Database.");
        }
    }
    resumeLocalPersistence();
}

//  TODO - disabling validation for now
void PrismFrameworkObjectManagerHaSyncWorker::haSyncGetValidationResultsStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::haSyncGetValidationResultsStep : Starting ...");

    PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pStartHaSyncDumpContext->getStandbyMessage ());

    if (NULL != pPrismHaSyncConfigureStandbyMessage)
    {
        vector<PrismServiceId> &prismServiceIdsToCommunicate = pStartHaSyncDumpContext->getPrismServiceIdsToCommunicate ();
        UI32                    numberOfPrismServices        = prismServiceIdsToCommunicate.size ();
        UI32                    i                            = 0;

        for (i = 0; i < numberOfPrismServices; i++)
        {
            void *pValidationResults    = NULL;
            UI32  validationResultsSize = 0;

            pValidationResults = pPrismHaSyncConfigureStandbyMessage->transferBufferToUser (s_offSetForHaSyncValidationResults + prismServiceIdsToCommunicate[i], validationResultsSize);

            pStartHaSyncDumpContext->addValidationResultsForService (prismServiceIdsToCommunicate[i], pValidationResults, validationResultsSize);
        }
    }

    pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpCallback (FrameworkStatus frameworkStatus, PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage, void *pContext) 
{
    StartHaSyncDumpContext      *pStartHaSyncDumpContext     = reinterpret_cast<StartHaSyncDumpContext *> (pContext);

    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        completionStatus = pPrismHaSyncConfigureStandbyMessage->getCompletionStatus ();
            
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpCallback : PrismHaSyncConfigureStandbyMessage Completion Status : " + FrameworkToolKit::localize (completionStatus));
    }
    else
    {
        completionStatus = WAVE_MESSAGE_ERROR_HAPEER_MESSAGE_FAILED;

        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpCallback : PrismHaSyncConfigureStandbyMessage Completion Status : " + FrameworkToolKit::localize (frameworkStatus));
    }

    if (NULL != pPrismHaSyncConfigureStandbyMessage)
    {
        delete pPrismHaSyncConfigureStandbyMessage;
    }

    if (completionStatus != WAVE_MESSAGE_SUCCESS)
    {
       completionStatus = WAVE_MESSAGE_ERROR_STANDBY_SYNC_FAILED;

       if (pStartHaSyncDumpContext->getIsDbResumeRequired ())
       {
           resumeDatabase ();
       }
    }

    if (0 == (pStartHaSyncDumpContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        pStartHaSyncDumpContext->executeNextStep (completionStatus);
    }
}

bool PrismFrameworkObjectManagerHaSyncWorker::isServiceToBeExcludedInHaSyncCommunications (const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId               ()) == prismServiceId)           ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}


void PrismFrameworkObjectManagerHaSyncWorker::setSyncCompletionStatusStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

    if ((true == getFirmwareVersionMatch()) && 
        ((pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) ||
         (pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT)))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::setSyncCompletionStatusStep Enabling live sync");
        enableLiveSync ();
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: live sync can't be supported due to mismatched version");
        FrameworkToolKit::disableLiveSync ();
    }

    if (pStartHaSyncDumpContext->getIsDbResumeRequired ())
    {
        resumeDatabase ();
    }

    // If all the previous steps were successful, then set the sync dump completion status to true
    setSyncDumpComplete (true);

    if ((pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) ||
        (pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT))
    {
        setSyncState (IN_SYNC);
    }

    if ((pStartHaSyncDumpContext->getContextInfo () == SYNC_DUMP_CONTEXT) ||
        (pStartHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_CONTEXT))
    {
        if (false == pStartHaSyncDumpContext->getIsDbSyncRequired ())
        {
            // This is the case DB conversion in regular sync dump or ISSU failed. So no DB was synced to standby MM.
            // But we still proceed with the sync dump with startup file and VCS config. Mark out of sync as running config not in sync

           trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: Schema conversion failed return failure for sync dump");
           setSyncState (OUT_OF_SYNC);

           //if This is not a 4.0-> 3.0 downgrade case then return failure for sync dump as running DB sync is expected 
           //Otherwise proceed with downgrade flow which need success return on sync dump

           if (STARTUP_CFG_SYNC != m_peerHaVersion)
           {
               completionStatus = WAVE_MESSAGE_ERROR_SCHEMA_CONVERSION_FAILED;
           }
        }
        else 
        {
           // With DB sync complete, we will return success to FSS for sync dump result and mark active and standby in sync.
           // but since live sync is not supported in this case, as soon as active MM has update in DB, will mark standby as out of sync.

           setSyncState (IN_SYNC);
        }
    }

    trace (TRACE_LEVEL_PERF_END, "HA Sync Dump.");

    FrameworkToolKit::setSyncFailureNotified (false);
    pStartHaSyncDumpContext->executeNextStep (completionStatus);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyHandler (PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyHandler: Starting ...");

    // Set the context 
    UI32 sizeOfBuffer = 0;
    UI32 *pContext    = reinterpret_cast<UI32 *> (pPrismHaSyncConfigureStandbyMessage->findBuffer (CONTEXT_INFO, sizeOfBuffer));
    UI32 context      = UNKNOWN_CONTEXT_INFO;
    
    ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext = NULL;

    if (pContext != NULL)
    {
        context = *pContext;
    }

    if (context == SYNC_DUMP_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateVcsConfigStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::standbyConnectToHaPeerStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbySetInSyncStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateVcsConfigStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbySetInSyncStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == COPY_RUNNING_TO_STARTUP_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyRunningStartupStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == COPY_DEFAULT_TO_STARTUP_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == COPY_FILE_TO_STARTUP_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == VCS_CLUSTER_CONF_CHANGE_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateVcsConfigStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == CCMD_SYNC_DUMP_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbySetInSyncStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbySetInSyncStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == CCMD_COPY_RUNNING_TO_STARTUP_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep),
            //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };

        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (context == COPY_RUNNING_TO_STARTUP_OPTIMIZATION_CONTEXT)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::copyRunningStartupStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
        };
    
        pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pPrismHaSyncConfigureStandbyMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyHandler : unknown context");
    }

	prismAssert (NULL != pReceiveHaSyncDumpContext, __FILE__, __LINE__);
	
    pReceiveHaSyncDumpContext->setContextInfo (context);

    string  peerIpAddress = "";
    SI32    peerPort      = 0;
  
    ResourceId status = FrameworkToolKit::getHaPeerIpAddressAndPort (peerIpAddress, peerPort);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyHandler : could not find haPeer ipAddress and port.");
    }
    else
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyHandler : find haPeer ipAddress and port as [ %s:%d ]", peerIpAddress.c_str(), peerPort);
        pReceiveHaSyncDumpContext->setHaPeerIp  (peerIpAddress);
        pReceiveHaSyncDumpContext->setHaPeerPort(peerPort);
    }

    setSyncServicedAsStandby (true);

    pReceiveHaSyncDumpContext->start ();
}

void PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::removePreviousDatabaseBackupFile");

        string commandToRemoveDatabaseBackupFile = string ("rm -rf " + FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ());
        system (commandToRemoveDatabaseBackupFile.c_str ());
    }

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep");

    PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ());

    // Determine whether DB sync is needed
	UI32   sizeOfTheDatabaseBackupFromActive  = 0;

	char   *pBuffer = reinterpret_cast<char *> (pPrismHaSyncConfigureStandbyMessage->findBuffer (pPrismHaSyncConfigureStandbyMessage->getDatabaseBackupBufferTag (), sizeOfTheDatabaseBackupFromActive));

    UI32 context = pReceiveHaSyncDumpContext->getContextInfo ();

    if ((NULL != pBuffer) && ((context == SYNC_DUMP_CONTEXT) || (context == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) || (context == COPY_RUNNING_TO_STARTUP_CONTEXT) || (context == CCMD_SYNC_DUMP_CONTEXT) || (context == CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) || (context == CCMD_COPY_RUNNING_TO_STARTUP_CONTEXT)))
    {
        pReceiveHaSyncDumpContext->setIsDbSyncRequired (true);
	}
    else
	{
        pReceiveHaSyncDumpContext->setIsDbSyncRequired (false);
	}

    // Get the database dump type 
    UI32 sizeOfBuffer = 0; 
    UI32 * pDbDumpType = reinterpret_cast<UI32 *> (pPrismHaSyncConfigureStandbyMessage->findBuffer (DB_DUMP_TYPE, sizeOfBuffer));

    if (pDbDumpType != NULL)
    {    
        m_dbSchemaType = *pDbDumpType;

        if (m_dbSchemaType == CONVERTIBLE_SCHEMA)
        {
            m_isDbDropNeeded = true;
        }    
        else
        {    
            m_isDbDropNeeded = false;
        }    
    }    
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateVersionStep: database dump type not found");
        m_isDbDropNeeded = false;
        m_dbSchemaType = NOT_CONVERTIBLE_SCHEMA;
    }

    PrismFrameworkObjectManager::getInstance ()->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_NOT_ATTEMPTED);

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateServicesStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateServiceStep");

    vector<PrismServiceId> prismServiceIds;
    UI32                   numberOfPrismServiceIds;
    UI32                   i;
    LocationId             thisLocationId           = FrameworkToolKit::getThisLocationId ();
    ResourceId             status                   = WAVE_MESSAGE_SUCCESS;

    PrismThread::getListOfServiceIds (prismServiceIds);
    numberOfPrismServiceIds = prismServiceIds.size ();

    for (i = 0; i < numberOfPrismServiceIds; i++)
    {
        trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManagerHaSyncWorker::configureStandbyValidateServicesStep : Validating Service : ") + prismServiceIds[i]);

        if (true == (isServiceToBeExcludedInHaSyncCommunications (prismServiceIds[i])))
        {
            continue;
        }

        PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ());
        void                                  *pValidationData                        = NULL;
        UI32                                   size                                   = 0;

        pValidationData = pPrismHaSyncConfigureStandbyMessage->transferBufferToUser (prismServiceIds[i], size);

        WaveObjectManagerHaSyncValidateDataMessage message (prismServiceIds[i]);

        // If we have got some validation data then attach it and send it to the service.

        if ((NULL != pValidationData) && (0 != size))
        {
            trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::configureStandbyValidateServicesStep : Sending Validation details to Service : ") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            message.setValidationDetails (pValidationData, size, true);
        }

        status = sendSynchronously (&message, thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_SERVICE_VALIDATION_FAILED;

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::configureStandbyValidateServicesStep : Failed to Validate Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Status : " + FrameworkToolKit::localize (status));

            break;
        }

        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_SERVICE_VALIDATION_FAILED;

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::configureStandbyValidateServicesStep : Failed to Validate Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Completion Status : " + FrameworkToolKit::localize (status));

            break;
        }

        void *pValidationResults = NULL;

        message.getValidationResults (pValidationResults, size);

        if ((0 != size) && (NULL != pValidationResults))
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureStandbyValidateServicesStep : Obtained Validation Results for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            pPrismHaSyncConfigureStandbyMessage->addBuffer (prismServiceIds[i] + s_offSetForHaSyncValidationResults, size, pValidationResults, true);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureStandbyValidateServicesStep : No Validation Results are obtained for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));
        }
    }

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyShutdownServicesStep");

        PrismFrameworkObjectManager* pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
        PrismFinalizeWorker* pPrismFinalizeWorker = pPrismFrameworkObjectManager->getPFinalizeWorker();

        status = pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_STANDBY_CONFIGURE);
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateVcsConfigStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateVcsConfigStep");

    PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ());

    // Set vcs configuration
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    UI32 sizeOfData = 0;

    char * pVcsClusterData = reinterpret_cast<char *> (pPrismHaSyncConfigureStandbyMessage->findBuffer (VCS_CONFIG, sizeOfData));

    if (pVcsClusterData != NULL)
    {
        UI32 context = pReceiveHaSyncDumpContext->getContextInfo ();

	    string serializedData (pVcsClusterData);

        VcsClusterConfiguration vcsClusterConfiguration;
   
        string  peerHaIp            = pReceiveHaSyncDumpContext->getHaPeerIp();
        SI32    peerHaPort          = pReceiveHaSyncDumpContext->getHaPeerPort();
        UI8     serializationType   = FrameworkToolKit::getSerializationTypeForHaPeer (peerHaIp, peerHaPort);

	    vcsClusterConfiguration.loadFromSerializedData2 (serializedData, serializationType);

        status = PrismFrameworkObjectManager::getInstance ()->setVcsClusterConfigData (&vcsClusterConfiguration, context);
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep");

    PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ());

    UI32 sizeOfStartupData = 0;

    char * pStartupData = reinterpret_cast<char *> (pPrismHaSyncConfigureStandbyMessage->findBuffer (STARTUP_FILE_OBJECT, sizeOfStartupData));

    if (pStartupData != NULL)
    {
	    string serializedData (pStartupData);

    	StartupFileConfiguration startupFileConfiguration;
   
        string  peerHaIp            = pReceiveHaSyncDumpContext->getHaPeerIp();
        SI32    peerHaPort          = pReceiveHaSyncDumpContext->getHaPeerPort();
        UI8     serializationType   = FrameworkToolKit::getSerializationTypeForHaPeer (peerHaIp, peerHaPort);

   	    startupFileConfiguration.loadFromSerializedData2 (serializedData, serializationType);
    
	    bool isPrevStartupDbValid = FrameworkToolKit::getIsStartupValid ();
        UI32 startupFileType = startupFileConfiguration.getStartupFileType ();
        
	    WaveNs::trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: setup startup file to type:") + startupFileType + " filename:" + string(startupFileConfiguration.getStartupFileName ()));
    
        if (startupFileType == WAVE_PERSISTENCE_REGULAR_FILE)
        {
            WaveNs::tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: startup WAVE_PERSISTENCE_REGULAR_FILE");
   
            ofstream configurationFileToWrite (FrameworkToolKit::getStartupBackupFileName ().c_str ());

            if (true == (configurationFileToWrite.is_open ()))
            {
                string startupFile = startupFileConfiguration.getStartupFile ();

                configurationFileToWrite << startupFile;
                configurationFileToWrite.close ();

                string cmdString = "/bin/mv " + FrameworkToolKit::getStartupBackupFileName () + " " + startupFileConfiguration.getStartupFileName ();
                vector<string> output;

                SI32 retStatus = FrameworkToolKit::systemCommandOutput(cmdString.c_str (), output);

                if (0 != retStatus)
                {
                    if (false == output.empty ())
                    {
                        trace (TRACE_LEVEL_WARN, string ("PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep:: copying startup backup file to startup file failed. Error : ") + output[0].c_str ());
                    }

                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: Failed to sync startup file : " + (startupFileConfiguration.getStartupFileName ()));

                    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR);
                    return;
                }
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: Cannot open file : " + (startupFileConfiguration.getStartupFileName ()));
   
                pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
	    }
	    else if (startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE)
	    {
		    WaveNs::tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: startup WAVE_PERSISTENCE_DEFAULT_FILE");
	    }
	    else if (startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE)
	    {
		    WaveNs::tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: startup WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE");
   
		    startupFileType = WAVE_PERSISTENCE_DEFAULT_FILE;	// For Persistence manager to be handled properly
	    }
	    else
	    {
		    WaveNs::tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: startupfile type unknown %d", startupFileType);
	    }
   
	    PersistenceLocalObjectManagerSetStartupFileMessage *pPersistenceLocalObjectManagerSetStartupFileMessage = new PersistenceLocalObjectManagerSetStartupFileMessage(startupFileType, startupFileConfiguration.getStartupFileName ());

	    status = sendSynchronously (pPersistenceLocalObjectManagerSetStartupFileMessage);
   
	    if (WAVE_MESSAGE_SUCCESS != status)
	    {
		    WaveNs::trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: send SetStartupFileMessage failed - status: ") + FrameworkToolKit::localize(status));
	    }
	    else
	    {
		    ResourceId completionStatus = pPersistenceLocalObjectManagerSetStartupFileMessage->getCompletionStatus ();
   
		    if (WAVE_MESSAGE_SUCCESS != completionStatus)
		    {
			    WaveNs::trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: SetStartupFileMessage complete with error\n");
		    }
	    }

	    delete (pPersistenceLocalObjectManagerSetStartupFileMessage);

	    FrameworkToolKit::savePrismConfiguration(false);

	    if (isPrevStartupDbValid == true)
	    {
		    WaveNs::trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: need to trigger reboot since prev DB valid.\n");
		    bool isValid = false;
		    FrameworkToolKit::setIsStartupValid (isValid);
		    FrameworkToolKit::savePrismConfiguration(false);
            WaveNs::trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: Beginning System Call SYNC");
            sync ();
            WaveNs::trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdateStartupFileStep: Ending System Call SYNC");
	    }
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::standbyConnectToHaPeerStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::standbyConnectToHaPeerStep");

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyBootServicesPrePhaseStep");

		PrismFrameworkObjectManager* pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
		PrismFrameworkObjectManagerInitializeWorker* pPrismInitializeWorker = pPrismFrameworkObjectManager->getPInitializeWorker();

		ResourceId status = pPrismInitializeWorker->startPrismServices (WAVE_BOOT_HASTANDBY, WAVE_BOOT_PHASE_PRE_PHASE);

		if (WAVE_MESSAGE_SUCCESS != status)
		{
			trace (TRACE_LEVEL_FATAL, "HaSyncWorker::configureStandbyBootServicesPrePhaseStep : pre phase failed.");

			prismAssert (false, __FILE__, __LINE__);
		}
    }

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep");

        // Remove the .cfg files.  Then, in case DB restores fails, Dcmd will come up as first time boot after HA failover
        vector<string>  output;

        string cmd = "rm -rf " + FrameworkToolKit::getProcessInitialWorkingDirectory () + "/*.cfg";
        FrameworkToolKit::systemCommandOutput (cmd, output);
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep: Beginning System Call SYNC");
        sync();
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep: Ending System Call SYNC");

        PrismMessage *pMessage = NULL;

        if (m_isDbDropNeeded)
        {
            string sqlToDropSchema = (OrmRepository::getInstance ())->generateSqlToDropSchemaFromOrmDatabase (OrmRepository::getWaveCurrentSchema ());
            sqlToDropSchema += (OrmRepository::getInstance ())->generateSqlToDropSchemaFromOrmDatabase (OrmRepository::getWaveStartSchema ());

            pMessage = new DatabaseObjectManagerExecuteTransactionMessage (sqlToDropSchema);
        }
        else
        {
		    pMessage = new DatabaseObjectManagerEmptyMessage ();

            (dynamic_cast <DatabaseObjectManagerEmptyMessage *> (pMessage))->addToSchemasToBeEmptied (OrmRepository::getWaveStartSchema ());
            (dynamic_cast <DatabaseObjectManagerEmptyMessage *> (pMessage))->addToSchemasToBeEmptied (OrmRepository::getWaveCurrentSchema ());
        }

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep: Sending DatabaseObjectManagerEmptyMessage");

		ResourceId status  = sendSynchronously (pMessage);

		if (WAVE_MESSAGE_SUCCESS != status)
		{
			trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep : Could not send message to empty database.  Status : " + FrameworkToolKit::localize (status));

			prismAssert (false, __FILE__, __LINE__);
		}
		else
		{
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep: DatabaseObjectManagerEmptyMessage successfully sent.");

			status = pMessage->getCompletionStatus ();

			if (WAVE_MESSAGE_SUCCESS != status)
			{
				trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyEmptyDatabaseStep : Message to empty database failed.  Completion Status : " +FrameworkToolKit::localize (status));

				prismAssert (false, __FILE__, __LINE__);
			}
		}

        delete (pMessage);
    }

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep");

		PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ());

		UI32   sizeOfTheDatabaseBackupFromActive  = 0;

		char   *pBuffer = reinterpret_cast<char *> (pPrismHaSyncConfigureStandbyMessage->findBuffer (pPrismHaSyncConfigureStandbyMessage->getDatabaseBackupBufferTag (), sizeOfTheDatabaseBackupFromActive));

		if (NULL != pBuffer)
		{
			string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName3 ());

			ofstream databaseBackupFile;

			databaseBackupFile.open (backupFileName.c_str (), ios::binary);
			databaseBackupFile.write (pBuffer, sizeOfTheDatabaseBackupFromActive);
			databaseBackupFile.close ();

			// Request the Database service to restore itself from the backup file that we created based on the buffer we obtained from the primary.

            DatabaseObjectManagerRestoreMessage message (backupFileName);

            if (m_isDbDropNeeded == true)
            {
                message.setRestoreSchema (true);
            }
            else
            {
                message.setDataOnlyRestore (true);
            }

            message.setSaveConfiguration (false);

			ResourceId status  = sendSynchronously (&message);

			if (WAVE_MESSAGE_SUCCESS != status)
			{
				trace (TRACE_LEVEL_FATAL, "HaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep : Could not send message to restore database.Status : " + FrameworkToolKit::localize (status));

				prismAssert (false, __FILE__, __LINE__);
			}
			else
			{
				status = message.getCompletionStatus ();

				if (WAVE_MESSAGE_SUCCESS != status)
				{
					trace (TRACE_LEVEL_FATAL, "HaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep : Message to restore database failed.  Completion Status : " + FrameworkToolKit::localize (status));

				    prismAssert (false, __FILE__, __LINE__);
				}
				else
				{
					trace (TRACE_LEVEL_INFO, "HaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep : SUCCESSFULLY Synced up local database with that of the Active MM.");
				}
			}
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, "HaSyncWorker::configureStandbyLoadDatabaseFromActiveDatabaseStep : No Database backup has been received ???");
		}
    }

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if ((pReceiveHaSyncDumpContext->getIsDbSyncRequired ()) && m_isDbDropNeeded)
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep");

        status = PrismFrameworkObjectManager::getInstance ()->upgradeDatabase ();

		if (FRAMEWORK_SUCCESS != status)
		{
            PrismFrameworkObjectManager::getInstance ()->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_FAILED);
			trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyConvertDatabaseStep : Could not upgrade the database.  Status : " + FrameworkToolKit::localize (status));

			prismAssert (false, __FILE__, __LINE__);
		}
        else
		{
            PrismFrameworkObjectManager::getInstance ()->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_SUCCEEDED);
            status = WAVE_MESSAGE_SUCCESS;
		}
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep");

        PrismHaSyncConfigureStandbyMessage *pPrismHaSyncConfigureStandbyMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ());

        UI32 sizeOfCfgData = 0;

        char *pCfgData = reinterpret_cast<char *> (pPrismHaSyncConfigureStandbyMessage->findBuffer (CFG_OBJECT, sizeOfCfgData));

        if (pCfgData != NULL)
        {
        	PrismFrameworkConfiguration prismFrameworkConfiguration;
        	string serializedData (pCfgData);

            string  peerHaIp            = pReceiveHaSyncDumpContext->getHaPeerIp();
            SI32    peerHaPort          = pReceiveHaSyncDumpContext->getHaPeerPort();
            UI8     serializationType   = FrameworkToolKit::getSerializationTypeForHaPeer (peerHaIp, peerHaPort);

        	prismFrameworkConfiguration.loadFromSerializedData2(serializedData, serializationType);

            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep: successfully found cfg data");

            status = setFrameworkConfigurationFromConfigurationFile (prismFrameworkConfiguration);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::loadPrismConfigurationStep: cfg data not found.");

            status = WAVE_MESSAGE_ERROR;
        }
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

ResourceId PrismFrameworkObjectManagerHaSyncWorker::setFrameworkConfigurationFromConfigurationFile (const PrismFrameworkConfiguration &prismFrameworkConfiguration)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

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

void PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::savePrismConfigurationStep");

        FrameworkToolKit::savePrismConfiguration(false);
    }

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{

    ResourceId  status          = WAVE_MESSAGE_SUCCESS;
    bool        cfgValidity     = false;
    UI32        sizeOfBuffer    = 0;

    PrismHaSyncConfigureStandbyMessage *pMessage = reinterpret_cast<PrismHaSyncConfigureStandbyMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ()); 

    bool *cfgValidityPtr = reinterpret_cast<bool *> (pMessage->findBuffer (CFG_VALIDITY, sizeOfBuffer));

    if (NULL == cfgValidityPtr)
    {
        cfgValidity = true; 
    }
    else
    {
        cfgValidity = (*cfgValidityPtr);
    }

    if (false == cfgValidity)
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep: remove prism configuration file.");
        status = FrameworkToolKit::changePrismConfigurationValidity (false);
    }
    else
    { 
        if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyUpdatePrismConfigurationFileStep: save prism configuration file.");
            status = FrameworkToolKit::changePrismConfigurationValidity (true);
        }
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbyServicesPostPhaseStep");

		PrismFrameworkObjectManager* pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
		PrismFrameworkObjectManagerInitializeWorker* pInitializeWorker = pPrismFrameworkObjectManager->getPInitializeWorker();

		ResourceId status = pInitializeWorker->startPrismServices (WAVE_BOOT_HASTANDBY, WAVE_BOOT_PHASE_POST_PHASE);

		// Instead of asserting, rollback to last known good state.

		if (WAVE_MESSAGE_SUCCESS != status)
		{
			trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker:: post phase failed.");

			prismAssert (false, __FILE__, __LINE__);
		}
    }

    PrismFrameworkObjectManager::getInstance ()->setDbConversionStatus (DATABASE_SCHEMA_CONVERSION_NOT_ATTEMPTED);
    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep");

        if ((pReceiveHaSyncDumpContext->getContextInfo () == SYNC_DUMP_CONTEXT) || (pReceiveHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_CONTEXT)) 
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep: Copy startup schema not needed for snapshot DB sync");
            FrameworkToolKit::setIsStartupValid (true);
        }
        else if ((pReceiveHaSyncDumpContext->getContextInfo () == COPY_RUNNING_TO_STARTUP_CONTEXT) || (pReceiveHaSyncDumpContext->getContextInfo () == CCMD_COPY_RUNNING_TO_STARTUP_CONTEXT))
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep : Copy startup schema not needed for copy running start.");
   
            FrameworkToolKit::setIsStartupValid (true);
        }
        else if ((pReceiveHaSyncDumpContext->getContextInfo () == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) || (pReceiveHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT))
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep : Copy startup schema not needed for running DB sync");
   
            FrameworkToolKit::setIsStartupValid (true);
        }
        else
        {
            // Currently this part of code is not executed under any context for HA_VERSION of RUNNING_CFG_SYNC_SAME_VERSION or above
            string sourceSchema = OrmRepository::getWaveStartSchema ();
            string destSchema = OrmRepository::getWaveCurrentSchema ();

            DatabaseObjectManagerExecuteCopySchemaMessage message (sourceSchema, destSchema);
    
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep : source schema " + sourceSchema + ", dest schema " + destSchema);
        
            status = sendSynchronously (&message);

            if (status != WAVE_MESSAGE_SUCCESS)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep : Could not send message to Persistence Service.  Status : " + FrameworkToolKit::localize (status));
            }
            else
            {
                status = message.getCompletionStatus ();
        
                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep : Failed to execute the transaction.  Completion Status : " + FrameworkToolKit::localize (status));
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::copyStartupSchemaStep : Succeeded.  Setting IsStartupValid to true.");
        
	                FrameworkToolKit::setIsStartupValid (true);
                }
            }
        }
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep");

        PersistenceLocalObjectManagerUpdateInstanceIdMesssage updateInstanceIdMessage;

        status  = sendSynchronously (&updateInstanceIdMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep : Could not send message to update instance Id.  Status : " + FrameworkToolKit::localize (status));

            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            status = updateInstanceIdMessage.getCompletionStatus ();
        
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::updateInstanceIdStep : Not able to update Instance Id  Completion Status : " + FrameworkToolKit::localize (status));

                prismAssert (false, __FILE__, __LINE__);
            }
        }
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::copyRunningStartupStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    ResourceId schemaStatus = ConfigFileManagementToolKit::copyRunningToStartup ();

    ResourceId fileStatus = ConfigFileManagementToolKit::copyRunningToStartupFile ();

    if (schemaStatus != WAVE_MESSAGE_SUCCESS || fileStatus != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerHaSyncWorker::copyRunningStartupStep : schemaStatus: ") + FrameworkToolKit::localize (schemaStatus) + "fileStatus: " +  FrameworkToolKit::localize (fileStatus));

        if (schemaStatus != WAVE_MESSAGE_SUCCESS)
        {
            status = schemaStatus;
        }
        else
        {
            status = fileStatus;
        }
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    pReceiveHaSyncDumpContext->executeNextStep (status);
}

void PrismFrameworkObjectManagerHaSyncWorker::configureStandbySetInSyncStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::configureStandbySetInSyncStep");

    if ((pReceiveHaSyncDumpContext->getContextInfo () == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT) ||
        (pReceiveHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_WITH_PAUSE_DB_CONTEXT))
    {
        trace (TRACE_LEVEL_INFO, "Standby Running Config In Sync with Active MM");
        setStandbySyncState (IN_SYNC);
    }
    else if ((pReceiveHaSyncDumpContext->getContextInfo () == SYNC_DUMP_CONTEXT) ||
             (pReceiveHaSyncDumpContext->getContextInfo () == CCMD_SYNC_DUMP_CONTEXT))
    {
        if (false == pReceiveHaSyncDumpContext->getIsDbSyncRequired ())
        {
            // This is the case DB conversion in regular sync dump or ISSU failed. So no DB was synced to standby MM.
            // But we still proceed with the sync dump with startup file and VCS config. Mark out of sync as running config not in sync

           trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: NO DB sync supported for this case");
           setStandbySyncState (OUT_OF_SYNC);
        }
        else
        {
           // With the inital sync dump success, mark standby in sync with active. Since live sync is not supported, active MM will
           // notify FSS/HASM on sync update failure. Then they will trigger chassis reboot instead of MM failover.
           trace (TRACE_LEVEL_INFO, "Standby Running Config In Sync with Active MM");
           setStandbySyncState (IN_SYNC);
        }
    }

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::startHaSyncUpdateHandler (FrameworkObjectManagerHaSyncUpdateMessage *pFrameworkObjectManagerHaSyncUpdateMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncUpdateValidationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateStandbyMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetVcsClusterDataStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetStartupDataStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetConfigurationFileStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncFailedStep),
    };

    StartHaSyncDumpContext *pStartHaSyncDumpContext = new StartHaSyncDumpContext (pFrameworkObjectManagerHaSyncUpdateMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pStartHaSyncDumpContext->holdAll ();
    pStartHaSyncDumpContext->start ();

}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncUpdateValidationStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::haSyncUpdateValidationStep");

    if (getSyncDumpComplete ())
    {
        FrameworkObjectManagerHaSyncUpdateMessage *pFrameworkObjectManagerHaSyncUpdateMessage = dynamic_cast<FrameworkObjectManagerHaSyncUpdateMessage *>(pStartHaSyncDumpContext->getPPrismMessage());

        // Set the context 
        UI32 sizeOfBuffer = 0;
        UI32 * pContext = reinterpret_cast<UI32 *> (pFrameworkObjectManagerHaSyncUpdateMessage->findBuffer (CONTEXT_INFO, sizeOfBuffer));
    
        if (pContext != NULL)
        {
            pStartHaSyncDumpContext->setContextInfo (*pContext);
        }
        else
        {
            pStartHaSyncDumpContext->setContextInfo (UNKNOWN_CONTEXT_INFO);
        }

        UI32 context = pStartHaSyncDumpContext->getContextInfo ();

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::haSyncUpdateValidationStep : context = ") + pStartHaSyncDumpContext->getContextInfo ());

        if ((FrameworkToolKit::getIsStartupValid ()) && (m_dbSchemaType != NOT_CONVERTIBLE_SCHEMA) && (context == COPY_RUNNING_TO_STARTUP_CONTEXT))
        {
            pStartHaSyncDumpContext->setIsDbSyncRequired (pFrameworkObjectManagerHaSyncUpdateMessage->getIsDbSyncRequired ());
        }
        else
        {
            pStartHaSyncDumpContext->setIsDbSyncRequired (false);
        }

        pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR_OPERATION_NOT_SUPPORTED);
    }
}

void PrismFrameworkObjectManagerHaSyncWorker::getFirmwareVersionHandler (FrameworkObjectManagerGetFirmwareVersionMessage *pFrameworkObjectManagerGetFirmwareVersionMessage)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::getFirmwareVersionHandler: Starting ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep),
    };

    ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext = new ReceiveHaSyncDumpContext (pFrameworkObjectManagerGetFirmwareVersionMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pReceiveHaSyncDumpContext->start ();

}

void PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep (ReceiveHaSyncDumpContext *pReceiveHaSyncDumpContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep: Starting ...");

    FrameworkObjectManagerGetFirmwareVersionMessage *pFrameworkObjectManagerGetFirmwareVersionMessage = reinterpret_cast<FrameworkObjectManagerGetFirmwareVersionMessage *> (pReceiveHaSyncDumpContext->getPPrismMessage ());

    UI32 sizeOfBuffer = 0;
    UI32 * pPeerHaVersion = reinterpret_cast<UI32 *> (pFrameworkObjectManagerGetFirmwareVersionMessage->findBuffer (ACTIVE_HA_SYNC_VERSION, sizeOfBuffer));
    if (pPeerHaVersion != NULL)
    {
        m_peerHaVersion = *pPeerHaVersion;
    }
    else
    {
        m_peerHaVersion = UNKNOWN_VERSION;
    }

    string fwVersion = PrismFrameworkObjectManager::getInstance ()->getFirmwareVersion ();
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep: Firmware version: " + fwVersion);
    pFrameworkObjectManagerGetFirmwareVersionMessage->setFirmwareVersion (fwVersion);

    UI32 sizeOfDatabaseSchemaFromActive = 0;
    char *pDbSchema = reinterpret_cast<char *> (pFrameworkObjectManagerGetFirmwareVersionMessage->findBuffer (ACTIVE_DB_SCHEMA_OBJECT, sizeOfDatabaseSchemaFromActive));
        
    DatabaseSchema databaseSchema;
    string         serializedData;

    if (FRAMEWORK_SUCCESS != (OrmToolKit::getDatabaseSchema (databaseSchema)))    
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep: failed to get DatabaseSchema object from db."));
        pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR);      
        return;
    }

    databaseSchema.serialize2 (serializedData);

    UI32 databaseDumpType = NOT_CONVERTIBLE_SCHEMA;

    if (NULL != pDbSchema)
    {
        DatabaseSchema databaseSchemaFromActive;
       	string         serializedDataFromActive (pDbSchema);

        databaseSchemaFromActive.loadFromSerializedData2(serializedDataFromActive);

        if (databaseSchemaFromActive == databaseSchema)
        {
            m_isDbDropNeeded = false;
            databaseDumpType = SAME_SCHEMA;

            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep: Database dump type = SAME_SCHEMA (") + SAME_SCHEMA + ")");
        }
        else
        {
            if (databaseSchemaFromActive.isConvertibleTo (databaseSchema))
            {
                m_isDbDropNeeded = true;
                databaseDumpType = CONVERTIBLE_SCHEMA;
                trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep: Database dump type = CONVERTIBLE_SCHEMA (") + CONVERTIBLE_SCHEMA + ")");
            }
            else 
            {
                trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::sendFirmwareVersionStep: Database dump type = NOT_CONVERTIBLE_SCHEMA (") + NOT_CONVERTIBLE_SCHEMA + ")");
            }
        }
    }
    else
    {
        m_peerHaVersion = STARTUP_CFG_SYNC;
    }

    pFrameworkObjectManagerGetFirmwareVersionMessage->addBuffer (STANDBY_DB_SCHEMA_OBJECT, serializedData.size (), (void *) (serializedData.c_str()), false);
    pFrameworkObjectManagerGetFirmwareVersionMessage->addBuffer (DB_DUMP_TYPE, sizeof (UI32), (void *) &databaseDumpType, false);

    pFrameworkObjectManagerGetFirmwareVersionMessage->addBuffer (STANDBY_HA_SYNC_VERSION, sizeof (UI32), (void *) &m_myHaVersion, false);

    pReceiveHaSyncDumpContext->setPPrismMessage (pFrameworkObjectManagerGetFirmwareVersionMessage);

    setStandbySyncState (OUT_OF_SYNC);

    pReceiveHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManagerHaSyncWorker::startCcmdHaSyncDumpHandler (FrameworkObjectManagerStartCcmdHaSyncMessage *pFrameworkObjectManagerStartCcmdHaSyncMessage)
{
    if (FrameworkToolKit::getSecondaryClusterFormationFlag ())
    {
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::startCcmdHaSyncDumpHandler: Postponing HA sync as cluster formation is in progress"));
        postponeMessageHandling (pFrameworkObjectManagerStartCcmdHaSyncMessage);
        return;
    }

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::ccmdConnectToHaPeerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncValidateVersionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateStandbyMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetConfigurationFileStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::setSyncCompletionStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncFailedStep),
    };

    StartHaSyncDumpContext *pStartHaSyncDumpContext = new StartHaSyncDumpContext (pFrameworkObjectManagerStartCcmdHaSyncMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pStartHaSyncDumpContext->setHaPeerIp (pFrameworkObjectManagerStartCcmdHaSyncMessage->getPeerHaIpAddress ());
    pStartHaSyncDumpContext->setHaPeerPort (pFrameworkObjectManagerStartCcmdHaSyncMessage->getPeerHaIpPort ());
    pStartHaSyncDumpContext->holdAll ();
    pStartHaSyncDumpContext->start ();

}

void PrismFrameworkObjectManagerHaSyncWorker::ccmdConnectToHaPeerStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    SI32 status = FRAMEWORK_SUCCESS;
    ResourceId ret = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: ccmdConnectToHaPeerStep");

    FrameworkObjectManagerStartCcmdHaSyncMessage *pFrameworkObjectManagerStartCcmdHaSyncMessage = dynamic_cast<FrameworkObjectManagerStartCcmdHaSyncMessage *>(pStartHaSyncDumpContext->getPPrismMessage());

    // Set the context 
    UI32 sizeOfBuffer = 0;
    UI32 * pContext = reinterpret_cast<UI32 *> (pFrameworkObjectManagerStartCcmdHaSyncMessage->findBuffer (CONTEXT_INFO, sizeOfBuffer));

    if (pContext != NULL)
    {
        pStartHaSyncDumpContext->setContextInfo (*pContext);
    }
    else
    {
        pStartHaSyncDumpContext->setContextInfo (UNKNOWN_CONTEXT_INFO);
    }

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::ccmdConnectToHaPeerStep : context = ") + pStartHaSyncDumpContext->getContextInfo ());

    // Set the sync state to OUT_OF_SYNC at the beginning of the sync operation
    setSyncState (OUT_OF_SYNC);

    string peerHaIp = pFrameworkObjectManagerStartCcmdHaSyncMessage->getPeerHaIpAddress();
    SI32 peerHaPort = pFrameworkObjectManagerStartCcmdHaSyncMessage->getPeerHaIpPort();

    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManagerHaSyncWorker:: peer HA Ip:") + peerHaIp + string(" port:") + peerHaPort);

    disconnectFromHaPeer (peerHaIp, peerHaPort, true);

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker:: ccmdConnectToHaPeer - disconnect from peer HA Ip:") + peerHaIp + string(" port:") + peerHaPort);

    if ((status = connectToHaPeer(peerHaIp, peerHaPort)) != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker:: ccmdConnectToHaPeer failed");

        ret = WAVE_MESSAGE_ERROR;
    }
    pStartHaSyncDumpContext->executeNextStep (ret);
}

void PrismFrameworkObjectManagerHaSyncWorker::ccmdHaSyncUpdateValidationStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::ccmdHaSyncUpdateValidationStep");

    if (getSyncDumpComplete ())
    {
        FrameworkObjectManagerCcmdHaSyncUpdateMessage *pFrameworkObjectManagerCcmdHaSyncUpdateMessage = dynamic_cast<FrameworkObjectManagerCcmdHaSyncUpdateMessage *>(pStartHaSyncDumpContext->getPPrismMessage());

        // Set the context 
        UI32 sizeOfBuffer = 0;
        UI32 * pContext = reinterpret_cast<UI32 *> (pFrameworkObjectManagerCcmdHaSyncUpdateMessage->findBuffer (CONTEXT_INFO, sizeOfBuffer));
    
        if (pContext != NULL)
        {
            pStartHaSyncDumpContext->setContextInfo (*pContext);
        }
        else
        {
            pStartHaSyncDumpContext->setContextInfo (UNKNOWN_CONTEXT_INFO);
        }

        UI32 context = pStartHaSyncDumpContext->getContextInfo ();

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::ccmdHaSyncUpdateValidationStep : context = ") + pStartHaSyncDumpContext->getContextInfo ());

        if ((m_dbSchemaType != NOT_CONVERTIBLE_SCHEMA) && (context == CCMD_COPY_RUNNING_TO_STARTUP_CONTEXT))
        {
            pStartHaSyncDumpContext->setIsDbSyncRequired (true);
        }
        else
        {
            pStartHaSyncDumpContext->setIsDbSyncRequired (false);
        }

        pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pStartHaSyncDumpContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void PrismFrameworkObjectManagerHaSyncWorker::startCcmdHaSyncUpdateHandler (FrameworkObjectManagerCcmdHaSyncUpdateMessage *pFrameworkObjectManagerCcmdHaSyncUpdateMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::ccmdHaSyncUpdateValidationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateStandbyMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncGetConfigurationFileStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncCreateDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncSendDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManagerHaSyncWorker::haSyncFailedStep),
    };

    StartHaSyncDumpContext *pStartHaSyncDumpContext = new StartHaSyncDumpContext (pFrameworkObjectManagerCcmdHaSyncUpdateMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pStartHaSyncDumpContext->holdAll ();
    pStartHaSyncDumpContext->start ();

}

bool PrismFrameworkObjectManagerHaSyncWorker::getSyncDumpComplete ()
{
    bool syncDumpComplete;

    m_syncDumpCompleteMutex.lock ();

    syncDumpComplete = m_syncDumpComplete;

    m_syncDumpCompleteMutex.unlock ();

    return (syncDumpComplete);
}

void PrismFrameworkObjectManagerHaSyncWorker::setSyncDumpComplete (const bool &syncDumpComplete)
{
    m_syncDumpCompleteMutex.lock ();

    m_syncDumpComplete = syncDumpComplete;

    m_syncDumpCompleteMutex.unlock ();
}

bool PrismFrameworkObjectManagerHaSyncWorker::getFirmwareVersionMatch ()
{
      return (m_firmwareVersionMatch);
}

void PrismFrameworkObjectManagerHaSyncWorker::setFirmwareVersionMatch (const bool &firmwareVersionMatch)
{
    m_firmwareVersionMatch = firmwareVersionMatch;
}

void PrismFrameworkObjectManagerHaSyncWorker::endHaSyncHandler (FrameworkObjectManagerEndHaSyncMessage *pFrameworkObjectManagerEndHaSyncMessage)
{
    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::endHaSyncHandler : Starting ..."));

    // Set sync dump complete to false and disable live sync
    setSyncDumpComplete (false);
    setSyncState (OUT_OF_SYNC);
    FrameworkToolKit::disableLiveSync ();
    FrameworkToolKit::setSyncFailureNotified (false);

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManagerHaSyncWorker::endHaSyncHandler : stop sync dump and live sync"));

    return;
}

void PrismFrameworkObjectManagerHaSyncWorker::enableLiveSync ()
{
    bool isLiveSyncEnabled = true;

    PersistenceLocalObjectManagerSetLiveSyncEnableMessage *pPersistenceLocalObjectManagerSetLiveSyncEnableMessage = new PersistenceLocalObjectManagerSetLiveSyncEnableMessage (isLiveSyncEnabled);

    ResourceId status = sendSynchronously (pPersistenceLocalObjectManagerSetLiveSyncEnableMessage);
   
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManagerHaSyncWorker::enableLiveSync: send PersistenceLocalObjectManagerSetLiveSyncEnableMessage failed - status: ") + FrameworkToolKit::localize(status));
    }
    else
    {
        ResourceId completionStatus = pPersistenceLocalObjectManagerSetLiveSyncEnableMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerHaSyncWorker::enableLiveSync: PersistenceLocalObjectManagerSetLiveSyncEnableMessage complete with error\n");
        }
    }
 
    return; 
}

UI32 PrismFrameworkObjectManagerHaSyncWorker::getSyncState ()
{
    UI32 syncState;

    m_syncStateMutex.lock ();

    syncState = m_syncState;

    m_syncStateMutex.unlock ();

    return (syncState);
}

void PrismFrameworkObjectManagerHaSyncWorker::setSyncState (const UI32 &syncState)
{
    m_syncStateMutex.lock ();

    m_syncState = syncState;

    m_syncStateMutex.unlock ();
}

UI32 PrismFrameworkObjectManagerHaSyncWorker::getStandbySyncState ()
{
    UI32 standbySyncState;

    m_standbySyncStateMutex.lock ();

    standbySyncState = m_standbySyncState;

    m_standbySyncStateMutex.unlock ();

    return (standbySyncState);
}

void PrismFrameworkObjectManagerHaSyncWorker::setStandbySyncState (const UI32 &standbySyncState)
{
    m_standbySyncStateMutex.lock ();

    m_standbySyncState = standbySyncState;

    m_standbySyncStateMutex.unlock ();
}

bool PrismFrameworkObjectManagerHaSyncWorker::getSyncServicedAsStandby ()
{
    bool syncServicedAsStandby;

    m_syncServicedAsStandbyMutex.lock ();

    syncServicedAsStandby = m_syncServicedAsStandby;

    m_syncServicedAsStandbyMutex.unlock ();

    return (syncServicedAsStandby);
}

void PrismFrameworkObjectManagerHaSyncWorker::setSyncServicedAsStandby (const bool &syncServicedAsStandby)
{
    m_syncServicedAsStandbyMutex.lock ();

    m_syncServicedAsStandby = syncServicedAsStandby;

    m_syncServicedAsStandbyMutex.unlock ();
}

void PrismFrameworkObjectManagerHaSyncWorker::setSyncStateHandler (FrameworkObjectManagerSetSyncStateMessage *pFrameworkObjectManagerSetSyncStateMessage)
{
    UI32 syncState = pFrameworkObjectManagerSetSyncStateMessage->getSyncState ();
   
    setSyncState (syncState);

    reply (pFrameworkObjectManagerSetSyncStateMessage);
}

void PrismFrameworkObjectManagerHaSyncWorker::getSyncStateHandler (FrameworkObjectManagerGetSyncStateMessage *pFrameworkObjectManagerGetSyncStateMessage)
{
    pFrameworkObjectManagerGetSyncStateMessage->setSyncState (getSyncState ());

    reply (pFrameworkObjectManagerGetSyncStateMessage);
}

void PrismFrameworkObjectManagerHaSyncWorker::pauseLocalPersistence ()
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::pauseLocalPersistence : Starting ...");

    PrismPauseObjectManagerMessage message (PersistenceLocalObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::pauseLocalPersistence : Could not pause Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::pauseLocalPersistence : Could not pause Persistence.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void PrismFrameworkObjectManagerHaSyncWorker::resumeLocalPersistence ()
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerHaSyncWorker::resumeLocalPersistence : Starting ...");

    PrismResumeObjectManagerMessage message (PersistenceLocalObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::resumeLocalPersistence : Could not resume Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManagerHaSyncWorker::resumeLocalPersistence : Could not resume Persistence.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManagerHaSyncWorker::resumeDatabase : Successfully resumed Local Persistence.");
}

void PrismFrameworkObjectManagerHaSyncWorker::haSyncFailedStep (StartHaSyncDumpContext *pStartHaSyncDumpContext)
{
    if ((pStartHaSyncDumpContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_STANDBY_SYNC_FAILED) ||
        (pStartHaSyncDumpContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_HAPEER_MESSAGE_FAILED))
    {
        setSyncDumpComplete (false);
        setSyncState (OUT_OF_SYNC);
        FrameworkToolKit::disableLiveSync ();
    }
    PrismFrameworkObjectManagerHaSyncWorker::prismLinearSequencerFailedStep (pStartHaSyncDumpContext);
}


}
