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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Types/Types.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Core/PrismFrameworkServiceIndependentMessages.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Timer/TimerMessages.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Messaging/MessageFactory/PrismMessageFactory.h"
#include "Framework/Persistence/PersistenceObjectManagerExecuteTransactionMessage.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectModel/WaveManagedObjectQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessageForCount.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForFileReplayEnd.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForMultiPartitionCleanup.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForUpgradePhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForDebugInformation.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataWorker.h"
#include "Framework/ObjectModel/PrismPostbootWorker.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveSendToClientsContext.h"
#include "Framework/ObjectModel/ObjectTracker/ObjectTracker.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/DistributedLog/DistributedLogAddLogEntryMessage.h"
#include "Framework/Messaging/Local/MessageHistory.h"
#include "Framework/Core/Wave.h"
#include "Framework/ObjectModel/WaveManagedObjectUpdateWorker.h"
#include "Framework/ObjectModel/WaveSendMulticastContext.h"
#include "Framework/ObjectModel/WaveManagedObjectCreateWorker.h"
#include "Framework/ObjectModel/WaveDebugInformationWorker.h"
#include "Framework/ObjectModel/WaveObjectManagerStatisticsTracker.h"
#include "Framework/ObjectModel/WaveManagedObjectDeleteWorker.h"
#include "Framework/ObjectModel/WaveClientDataObjectGetWorker.h"
#include "Cluster/ClusterMessages.h"
#include "Framework/ObjectModel/ServiceIndependentMessageHandlerMap.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaDifferenceInfo.h"
#include "Framework/ObjectModel/PrismLinearSequencerContextForUpgradePhase.h"
#include "Cluster/MultiPartition/Global/WavePartitionManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/ObjectModel/WaveManagedObjectAsynchronousPluginContext.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "ManagementInterface/ClientInterface/UnifiedClientUpdateMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCreateMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientDeleteMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientPostbootMessage.h"
#include "WaveManagedObjectToolKit.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "ManagementInterface/ClientInterface/UnifiedClientNotifyMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientBackendDetails.h"
#include "Framework/ObjectModel/PrismLinearSequencerContextForShutdownPhase.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/Attributes/AttributeUC.h"
#include "Framework/Database/DatabaseObjectManagerCreateInMemoryManagedObjectMessage.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "Framework/Messaging/Remote/InterLocationMulticastMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerDelayedTransactionMessages.h"
#include "Framework/ObjectModel/WaveObjectManagerCommitTransactionContext.h"
#include "Framework/Persistence/WaveConfigManagedObject.h"
#include "Framework/CliBlockManagement/CliBlockManagementToolKit.h"
#include "Framework/CliBlockManagement/CliBlockMessage.h"
#include "Framework/Persistence/WaveConfigurationAttributes.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerRepository.h"
#include "Framework/ObjectModel/WaveDeliverBrokerPublishMessageWorker.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBroker.h"

#include <time.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <stdarg.h>
#include <sstream>

namespace WaveNs
{

static PrismMutex                          s_prismObjectManagerMutex;
static PrismServiceId                      s_nextAvailablePrismServiceId = 0;
static PrismMutex                          s_enabledServicesMutex;
static map<PrismServiceId, PrismServiceId> s_enabledServices;
static PrismMutex                          s_mutexForAddingEventListener;
static PrismMutex                          s_supportedEventsMutex;
static map<string, WaveObjectManager *>    s_allManagedClassesAndOwnersMap;
static PrismMutex                          s_allManagedClassesAndOwnersMapMutex;

/*
 * Handling Backtrace
 */


static size_t                               s_bactraceFrameCount = 512;
static size_t                               s_demangleOutputLength = 512;
static size_t                               s_backtraceMessageBufferLength = 4096;
static size_t                               s_bactraceMessageLength = 0;

static void                                 **s_pBackTraceCalledSite =  new void *[s_bactraceFrameCount];
static string                               s_tempBackTraceBuffer;
static vector<string>                       firstLevelTokens;
static vector<string>                       secondLevelTokens;
static char                                 *s_backTraceMessageBuffer= new char [ 4096 + 1];
static char                                 *s_pDemangleOutPutBuffer = new char[ 512 + 1];
static char                                 *s_crashForOMMessage= new char [512 + 1];


map<string, PrismElement *>       WaveObjectManager::m_ownersForCreatingManagedObjectInstances;
PrismMutex                        WaveObjectManager::m_createManagedObjectInstanceWrapperMutex;
map <string, PrismServiceId>      WaveObjectManager::m_serviceStringServiceIdMap;
PrismMutex                        WaveObjectManager::m_serviceStringServiceIdMapMutex;

map<string, map<string, string> > WaveObjectManager::m_clientsListeningForCreateByManagedObject;
map<string, map<string, string> > WaveObjectManager::m_clientsListeningForUpdateByManagedObject;
map<string, map<string, string> > WaveObjectManager::m_clientsListeningForDeleteByManagedObject;
map<string, map<string, string> > WaveObjectManager::m_managedObjectsForCreateByClient;
map<string, map<string, string> > WaveObjectManager::m_managedObjectsForUpdateByClient;
map<string, map<string, string> > WaveObjectManager::m_managedObjectsForDeleteByClient;
PrismMutex                        WaveObjectManager::m_clientsListeningMutex;
PrismMutex                        WaveObjectManager::m_postponedMessageQueueMutex;

WaveServiceMode WaveObjectManager::m_waveServiceLaunchMode = WAVE_SERVICE_ACTIVE;

WaveObjectManager::PrismMessageResponseContext::PrismMessageResponseContext (PrismMessage *pPrismMessage, PrismElement *pPrismMessageSender, PrismMessageResponseHandler pPrismMessageSenderCallback, void *pPrismMessageSenderContext)
    : m_pPrismMessage                  (pPrismMessage),
      m_pPrismMessageSender            (pPrismMessageSender),
      m_pPrismMessageSenderCallback    (pPrismMessageSenderCallback),
      m_pPrismMessageSenderContext     (pPrismMessageSenderContext),
      m_isMessageTimedOut              (false),
      m_pInputMessageInResponseContext (NULL),
      m_isTimerStarted                 (false),
      m_timerHandle                    (0)
{
}

void WaveObjectManager::PrismMessageResponseContext::executeResponseCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, bool isMessageRecalled)
{
    if (((PrismMessageResponseHandler) NULL) == m_pPrismMessageSenderCallback)
    {
        delete m_pPrismMessage;
    }
    else
    {
        // We treat timed out messages differently.  If a message is timed out we sinply mark this context
        // to indicate the time out and execute the user callback with a Null Message pointer.

        if (FRAMEWORK_TIME_OUT !=frameworkStatus)
        {
            // We take pPrismMessage input so that multiple replies with messages copies is also possible.  But the current
            // methodology that we decided to follow is this:  If there are multiple replies for a message then the message
            // must not be deleted unpon receiving intermediate replies.  It must be deleted  only after receiving the last
            // reply.  Though we have the infra structure to support multiple replies with message copies, currently we do
            // not copy the message for the purpose of mulitple replies.  So when the response comes back it must always be
            // the original message.  The following prismAssert encforces that.

            // Update to the above comment (at the time of implementing support for multiple replies feature):
            // The below enforcement is only done in case of last reply.

            if (true == (pPrismMessage->getIsLastReply ()))
            {
                WaveNs::prismAssert (pPrismMessage == m_pPrismMessage, __FILE__, __LINE__);

                if (pPrismMessage != m_pPrismMessage)
                {
                    return;
                }
            }

            // If we decide to support the mutiple replies using message copies then the above prismAssert and if block must be removed.

            // If the context was previously marked with timed out status, then we must not executethe user context.
            // We must simply delete the message.

            if (false == (getIsMessageTimedOut ()))
            {
                (m_pPrismMessageSender->*m_pPrismMessageSenderCallback) (frameworkStatus, pPrismMessage, m_pPrismMessageSenderContext);
            }
            else
            {
                delete pPrismMessage;
            }
        }
        else
        {
            setIsMessageTimedOut (true);

            if (true == isMessageRecalled)
            {
                (m_pPrismMessageSender->*m_pPrismMessageSenderCallback) (frameworkStatus, pPrismMessage, m_pPrismMessageSenderContext);
            }
            else
            {
                (m_pPrismMessageSender->*m_pPrismMessageSenderCallback) (frameworkStatus, NULL, m_pPrismMessageSenderContext);
            }
        }
    }
}

void WaveObjectManager::PrismMessageResponseContext::executeResponseCallback (FrameworkStatus frameworkStatus)
{
    executeResponseCallback (frameworkStatus, m_pPrismMessage);
}

void WaveObjectManager::PrismMessageResponseContext::setIsMessageTimedOut (bool isMessageTimedOut)
{
    m_isMessageTimedOut = isMessageTimedOut;
}

bool WaveObjectManager::PrismMessageResponseContext::getIsMessageTimedOut ()
{
    return (m_isMessageTimedOut);
}

PrismMessage *WaveObjectManager::PrismMessageResponseContext::getPPrismMessage ()
{
    return (m_pPrismMessage);
}

PrismMessage*WaveObjectManager::PrismMessageResponseContext::getPInputMessageInResponseContext ()
{
    return (m_pInputMessageInResponseContext);
}

void WaveObjectManager::PrismMessageResponseContext::setPInputMessageInResponseContext (PrismMessage *pPrismMessage)
{
    m_pInputMessageInResponseContext = pPrismMessage;
}

bool WaveObjectManager::PrismMessageResponseContext::getIsTimerStarted () const
{
    return (m_isTimerStarted);
}

void WaveObjectManager::PrismMessageResponseContext::setIsTimerStarted (const bool &isTimerStarted)
{
    m_isTimerStarted = isTimerStarted;
}

TimerHandle WaveObjectManager::PrismMessageResponseContext::getTimerHandle () const
{
    return (m_timerHandle);
}

void WaveObjectManager::PrismMessageResponseContext::setTimerHandle (const TimerHandle &timerHandle)
{
    m_timerHandle = timerHandle;
}

WaveObjectManager::PrismOperationMapContext::PrismOperationMapContext (PrismElement *pPrismElement, PrismMessageHandler pPrismMessageHandler)
    : m_pPrismElementThatHandlesTheMessage (pPrismElement),
      m_pPrismMessageHandler (pPrismMessageHandler)
{
}

void WaveObjectManager::PrismOperationMapContext::executeMessageHandler (PrismMessage *&pPrismMessage)
{
    (m_pPrismElementThatHandlesTheMessage->*m_pPrismMessageHandler) (pPrismMessage);
}

WaveObjectManager::PrismEventMapContext::PrismEventMapContext (PrismElement *pPrismElement, PrismEventHandler pPrismEventHandler)
    : m_pPrismElementThatHandlesTheEvent (pPrismElement),
      m_pPrismEventHandler (pPrismEventHandler)
{
}

void WaveObjectManager::PrismEventMapContext::executeEventHandler (const PrismEvent *&pPrismEvent)
{
    (m_pPrismElementThatHandlesTheEvent->*m_pPrismEventHandler) (pPrismEvent);
}

WaveObjectManager::PrismEventListenerMapContext::PrismEventListenerMapContext (const PrismServiceId &eventListenerServiceId, const LocationId &eventListenerLocationId)
    : m_eventListenerSericeId (eventListenerServiceId),
      m_eventListenerLocationId (eventListenerLocationId)
{
}

WaveObjectManager::PrismEventListenerMapContext::~PrismEventListenerMapContext ()
{
}

PrismServiceId WaveObjectManager::PrismEventListenerMapContext::getEventListenerServiceId () const
{
    return (m_eventListenerSericeId);
}

LocationId WaveObjectManager::PrismEventListenerMapContext::getEventListenerLocationId () const
{
    return (m_eventListenerLocationId);
}

WaveObjectManager::WaveManagedObjectForUpdate::~WaveManagedObjectForUpdate ()
{
}

WaveObjectManager::WaveManagedObjectUpdateSingle::WaveManagedObjectUpdateSingle (WaveManagedObject *updateObject)
    : m_pUpdateWaveManagedObject (updateObject)
{
}

WaveObjectManager::WaveManagedObjectUpdateSingle::~WaveManagedObjectUpdateSingle ()
{
}

void WaveObjectManager::WaveManagedObjectUpdateSingle::getSqlForUpdate (string &sql)
{
    m_pUpdateWaveManagedObject->getSqlForUpdate (sql, OrmRepository::getWaveCurrentSchema ());
}

WaveObjectManager::WaveManagedObjectUpdateMultiple::WaveManagedObjectUpdateMultiple (WaveManagedObjectSynchronousQueryContextForUpdate * pWaveManagedObjectSynchronousQueryContextForUpdate)
    : m_pWaveManagedObjectSynchronousQueryContextForUpdate (pWaveManagedObjectSynchronousQueryContextForUpdate)
{
}

WaveObjectManager::WaveManagedObjectUpdateMultiple::~WaveManagedObjectUpdateMultiple ()
{
    delete m_pWaveManagedObjectSynchronousQueryContextForUpdate;
}

void WaveObjectManager::WaveManagedObjectUpdateMultiple::getSqlForUpdate (string &sql)
{
    m_pWaveManagedObjectSynchronousQueryContextForUpdate->getSqlForUpdate (sql);
}

WaveObjectManager::WaveManagedObjectForDelete::~WaveManagedObjectForDelete ()
{
}

WaveObjectManager::WaveManagedObjectDeleteSingle::WaveManagedObjectDeleteSingle (ObjectId deleteObjectId)
    : m_deleteObjectId (deleteObjectId)
{
}

WaveObjectManager::WaveManagedObjectDeleteSingle::~WaveManagedObjectDeleteSingle ()
{
}

void WaveObjectManager::WaveManagedObjectDeleteSingle::getSqlForDelete (string &sql)
{
    (OrmRepository::getInstance ())->getSqlForDelete (m_deleteObjectId, sql, OrmRepository::getWaveCurrentSchema ());
}

WaveObjectManager::WaveManagedObjectDeleteMultiple::WaveManagedObjectDeleteMultiple (WaveManagedObjectSynchronousQueryContextForDeletion* pWaveManagedObjectSynchronousQueryContextForDeletion)
    : m_pWaveManagedObjectSynchronousQueryContextForDeletion (pWaveManagedObjectSynchronousQueryContextForDeletion),
      m_deleteOnlyCompositions (false),
      m_partialDeleteFromVectorComposition (false),
      m_parentClassName (""),
      m_compositionName ("")
{
}

WaveObjectManager::WaveManagedObjectDeleteMultiple::WaveManagedObjectDeleteMultiple (
        WaveManagedObjectSynchronousQueryContextForDeletion* pWaveManagedObjectSynchronousQueryContextForDeletion,
        bool deleteOnlyCompositions, set<string> relationsSet)
    : m_pWaveManagedObjectSynchronousQueryContextForDeletion (pWaveManagedObjectSynchronousQueryContextForDeletion),
      m_deleteOnlyCompositions (deleteOnlyCompositions),
      m_relationsSet (relationsSet),
      m_partialDeleteFromVectorComposition (false),
      m_parentClassName (""),
      m_compositionName ("")
{
}

WaveObjectManager::WaveManagedObjectDeleteMultiple::WaveManagedObjectDeleteMultiple (
        WaveManagedObjectSynchronousQueryContextForDeletion* pWaveManagedObjectSynchronousQueryContextForDeletion,
        bool partialDeleteFromVectorComposition, string parentClassName, string compositionName)
    : m_pWaveManagedObjectSynchronousQueryContextForDeletion (pWaveManagedObjectSynchronousQueryContextForDeletion),
      m_deleteOnlyCompositions (false),
      m_partialDeleteFromVectorComposition (partialDeleteFromVectorComposition),
      m_parentClassName (parentClassName),
      m_compositionName (compositionName)
{
}

WaveObjectManager::WaveManagedObjectDeleteMultiple::~WaveManagedObjectDeleteMultiple ()
{
    delete m_pWaveManagedObjectSynchronousQueryContextForDeletion;
}

void WaveObjectManager::WaveManagedObjectDeleteMultiple::getSqlForDelete (string &sql)
{
    if (m_partialDeleteFromVectorComposition) {
        m_pWaveManagedObjectSynchronousQueryContextForDeletion->getSqlForPartialDeleteFromVectorCompositions (sql, m_parentClassName, m_compositionName);
    }
    else if (m_deleteOnlyCompositions) {
        m_pWaveManagedObjectSynchronousQueryContextForDeletion->getSqlForDeletingCompositions(sql, m_relationsSet);
    }
    else {
        m_pWaveManagedObjectSynchronousQueryContextForDeletion->getSqlForDelete (sql);
    }

    WaveNs::trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectDeleteMultiple::getSqlForDelete: SQL :" + sql);
}

WaveObjectManager::WaveBrokerPublishMessageHandlerContext::WaveBrokerPublishMessageHandlerContext (WaveBrokerPublishMessageHandler waveBrokerPublishMessageHandler, PrismElement *pSubscriber)
    : m_waveBrokerPublishMessageHandler (waveBrokerPublishMessageHandler),
      m_pSubscriber                     (pSubscriber)
{
}

WaveObjectManager::WaveBrokerPublishMessageHandlerContext::~WaveBrokerPublishMessageHandlerContext ()
{
}

WaveBrokerPublishMessageHandler WaveObjectManager::WaveBrokerPublishMessageHandlerContext::getWaveBrokerPublishMessageHandler ()
{
    return (m_waveBrokerPublishMessageHandler);
}

PrismElement *WaveObjectManager::WaveBrokerPublishMessageHandlerContext::getPSubscriber ()
{
    return (m_pSubscriber);
}

WaveObjectManager::WaveObjectManager (const string &objectManagerName, const UI32 &stackSize, const vector<UI32> *pCpuAffinityVector)
    : PrismElement                          (this),
      m_name                                (objectManagerName),
      m_pAssociatedPrismThread              (NULL),
      m_isEnabled                           (false),
      m_isTransactionOn                     (false),
      m_pAssociatedVirtualWaveObjectManager (NULL),
      m_allowAutomaticallyUnlistenForEvents (true),
      m_pInputMessage                       (NULL),
      m_messageHistoryState                 (false),
      m_messageHistoryMaxSize               (30),
      m_messageHistoryNextSlotInVector      (0),
      m_messageHistoryLogInsideSend         (true),
      m_messageHistoryLogInsideReply        (true),
      m_messageHistoryLogInsideHandleMessage(true),
      m_serviceId                           (0)
{
    if (NULL != pCpuAffinityVector)
    {
        m_cpuAffinityVector = *pCpuAffinityVector;
    }

    m_waveServiceMode = m_waveServiceLaunchMode;

    UI32 currentPrismServiceId = 0;

    if (true != (canInstantiateServiceAtThisTime (objectManagerName)))
    {
        // Cannot use trace and other Prism facilities here.  Framework is not even instantiated.

        cerr << "WaveObjectManager::WaveObjectManager : Please make sure that the PrismFrameworkObjectManager is the first Object Manager that gets instantated." << endl;
        cerr << "                                         Trying to instantiate Service : " << objectManagerName << endl;
        cerr << "CANOT CONTINUE.  EXITING ..." << endl;
        assert (0);
    }

    s_prismObjectManagerMutex.lock ();
    s_nextAvailablePrismServiceId++;
    m_serviceId = currentPrismServiceId = s_nextAvailablePrismServiceId;
    s_prismObjectManagerMutex.unlock ();

    PrismThread *pAssociatedPrismThread = NULL;

    if (WAVE_SERVICE_ACTIVE == m_waveServiceMode)
    {
        pAssociatedPrismThread = new PrismThread (m_serviceId, this, m_name, stackSize, &m_cpuAffinityVector);
    }

    setAssociatedPrismThread (pAssociatedPrismThread);

    m_traceClientId = TraceObjectManager::addClient (FrameworkToolKit::getDefaultTraceLevel (), m_name);

    if (0 == m_traceClientId)
    {
        cerr << "Could not initialize tracing for " << m_name << ".  Cannot proceed." << endl;
        assert (0);
    }

    m_pWaveObjectManagerStatisticsTracker = new WaveObjectManagerStatisticsTracker ();
    prismAssert (NULL != m_pWaveObjectManagerStatisticsTracker, __FILE__, __LINE__);

    m_pWaveManagedObjectLoadOperationalDataWorker = new WaveManagedObjectLoadOperationalDataWorker (this);
    prismAssert (NULL != m_pWaveManagedObjectLoadOperationalDataWorker, __FILE__, __LINE__);

    m_pPrismPostbootWorker = new PrismPostbootWorker (this);
    prismAssert (NULL != m_pPrismPostbootWorker, __FILE__, __LINE__);

    m_pWaveManagedObjectUpdateWorker = new WaveManagedObjectUpdateWorker (this);
    prismAssert (NULL != m_pWaveManagedObjectUpdateWorker, __FILE__, __LINE__);

    m_pWaveManagedObjectCreateWorker = new WaveManagedObjectCreateWorker (this);
    prismAssert (NULL != m_pWaveManagedObjectCreateWorker, __FILE__, __LINE__);

    m_pWaveDebugInformationWorker = new WaveDebugInformationWorker (this);
    prismAssert (NULL != m_pWaveDebugInformationWorker, __FILE__, __LINE__);


    m_pWaveManagedObjectDeleteWorker = new WaveManagedObjectDeleteWorker (this);
    prismAssert (NULL != m_pWaveManagedObjectDeleteWorker, __FILE__, __LINE__);

    m_pWaveClientDataObjectGetWorker = new WaveClientDataObjectGetWorker (this);
    prismAssert (NULL != m_pWaveClientDataObjectGetWorker, __FILE__, __LINE__);

    m_pWaveDeliverBrokerPublishMessageWorker = new WaveDeliverBrokerPublishMessageWorker(this);
    prismAssert (NULL != m_pWaveDeliverBrokerPublishMessageWorker, __FILE__, __LINE__);

    addOperationMap (WAVE_OBJECT_MANAGER_INITIALIZE,                             reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::initializeHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_LISTEN_FOR_EVENTS,                      reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::listenForEventsHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_INSTALL,                                reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::installHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_ENABLE,                                 reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::enableHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_UPGRADE,                                reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::upgradeHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_BOOT,                                   reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::bootHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_SHUTDOWN,                               reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::shutdownHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_UNINSTALL,                              reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::uninstallHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_DISABLE,                                reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::disableHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_UNINITIALIZE,                           reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::uninitializeHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_DESTRUCT,                               reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::destructHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_HAINSTALL,                              reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::hainstallHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_HABOOT,                                 reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::habootHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_HEARTBEAT_FAILURE,                      reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::heartbeatFailureHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_EXTERNAL_STATE_SYNCHRONIZATION,         reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::externalStateSynchronizationHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_CONFIG_REPLAY_END,                      reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::configReplayEndHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_FILE_REPLAY_END,                        reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::fileReplayEndHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_SLOT_FAILOVER,                          reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::slotFailoverHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_PARTITION_CLEANUP,                      reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::multiPartitionCleanupHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_TIMER_EXPIRED,                          reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::timerExpiredHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_CREATE_CLUSTER_COLLECT_VALIDATION_DATA, reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::clusterCreateCollectValidationDataHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_CREATE_CLUSTER_VALIDATE,                reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::clusterCreateValidateHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_CREATE_CLUSTER_SEND_VALIDATION_RESULTS, reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::clusterCreateSendValidationResultsHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_HA_SYNC_COLLECT_VALIDATION_DATA,        reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::haSyncCollectValidationDataHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_HA_SYNC_VALIDATE_DATA,                  reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::haSyncValidateDataHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_HA_SYNC_SEND_VALIDATION_RESULTS,        reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::haSyncSendValidationResultsHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_FAILOVER,                               reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::failoverHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_PAUSE,                                  reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::pauseHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_RESUME,                                 reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::resumeHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_REGISTER_EVENT_LISTENER,                reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::registerEventListenerHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_SET_CPU_AFFINITY,                       reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::setCpuAffinityHandler));

    addOperationMap (WAVE_OBJECT_MANAGER_BACKEND_SYNC_UP,                        reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::backendSyncUpHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_MESSAGE_HISTORY_CONFIG,                 reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::waveObjectManagerMessageHistoryConfigMessageHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_MESSAGE_HISTORY_DUMP,                   reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::waveObjectManagerMessageHistoryDumpMessageHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_FIPS_ZEROIZE,                           reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::zeroizeHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_DATABASE_SANITY_CHECK,                  reinterpret_cast<PrismMessageHandler> (&WaveObjectManager::databaseSanityCheckHandler));

    if (WAVE_SERVICE_ACTIVE == m_waveServiceMode)
    {
        WaveThreadStatus status = pAssociatedPrismThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "Could not start the thread corresponding to this object manager.");
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

WaveObjectManager::WaveObjectManager (const WaveObjectManager &prismObjectManager)
    : PrismElement (this),
      m_isTransactionOn (false)
{
    trace (TRACE_LEVEL_FATAL, "WaveObjectManager::WaveObjectManager : Copy constructing WaveObjectManager does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);
}

WaveObjectManager::~WaveObjectManager ()
{
    if (NULL != m_pWaveManagedObjectLoadOperationalDataWorker)
    {
        delete (m_pWaveManagedObjectLoadOperationalDataWorker);
    }

    if (NULL != m_pPrismPostbootWorker)
    {
        delete (m_pPrismPostbootWorker);
    }

    if (NULL != m_pWaveManagedObjectUpdateWorker)
    {
        delete m_pWaveManagedObjectUpdateWorker;
    }

    if (NULL != m_pWaveManagedObjectDeleteWorker)
    {
        delete m_pWaveManagedObjectDeleteWorker;
    }

    if (NULL != m_pWaveManagedObjectCreateWorker)
    {
        delete m_pWaveManagedObjectCreateWorker;
    }

    if (NULL != m_pWaveDebugInformationWorker)
    {
        delete m_pWaveDebugInformationWorker;
    }

    if (NULL != m_pWaveClientDataObjectGetWorker)
    {
        delete m_pWaveClientDataObjectGetWorker;
    }

    if (NULL != m_pWaveDeliverBrokerPublishMessageWorker)
    {
        delete m_pWaveDeliverBrokerPublishMessageWorker;
    }

    // delete MessageHistory objects in m_messageHistoryVector (if any)

    UI32 messageHistoryVectorSize = m_messageHistoryVector.size ();
    UI32 i                        = 0;

    for (i = 0; i < messageHistoryVectorSize; i++)
    {
        delete m_messageHistoryVector[i];
    }

    // Clean up WaveObjectManager Statistics

    if (NULL != m_pWaveObjectManagerStatisticsTracker)
    {
        m_pWaveObjectManagerStatisticsTracker->clearAllManagedObjectStatistics ();

        delete (m_pWaveObjectManagerStatisticsTracker);
    }

    UI32 numberOfRemainingWorkers = m_workers.size ();

    for (i  = 0; i < numberOfRemainingWorkers; i++)
    {
        delete m_workers[i];
    }

    m_workers.clear ();

    TraceObjectManager::removeClient (m_name);
}

string WaveObjectManager::getName () const
{
    return (m_name);
}

WaveObjectManager &WaveObjectManager::operator = (const WaveObjectManager &prismObjectManager)
{
    trace (TRACE_LEVEL_FATAL, "WaveObjectManager::operator = : Assigning to a WaveObjectManager does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);

    return (*this);
}

void WaveObjectManager::addOperationMap (UI32 operationCode, PrismMessageHandler pPrismMessageHandler, PrismElement *pPrismElement)
{
    if (NULL == pPrismElement)
    {
        pPrismElement = this;
    }

    map<UI32, PrismOperationMapContext *>::iterator element = m_operationsMap.find (operationCode);
    map<UI32, PrismOperationMapContext *>::iterator end     = m_operationsMap.end ();

    if (end != element)
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::addOperationMap : OperationMap already found for this operation code : ") + operationCode);
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_operationsMap[operationCode] = new PrismOperationMapContext (pPrismElement, pPrismMessageHandler);

    m_createMessageInstanceWrapperMutex.lock ();

    map<UI32, PrismElement *>::iterator element1    = m_ownersForCreatingMessageInstances.find (operationCode);
    map<UI32, PrismElement *>::iterator endElement1 = m_ownersForCreatingMessageInstances.end ();

    if (endElement1 != element1)
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::addOperationMap : Trying to set duplicate Owner.  Owner for \"") + operationCode + string ("\" was already set."));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_ownersForCreatingMessageInstances[operationCode] = pPrismElement;
    }

    m_createMessageInstanceWrapperMutex.unlock ();
}

void WaveObjectManager::addServiceIndependentOperationMap (UI32 operationCode, WaveServiceIndependentMessageHandler pWaveServiceIndependentMessageHandler)
{
    ServiceIndependentMessageHandlerMap::addOperationMap (getServiceId (), operationCode, pWaveServiceIndependentMessageHandler);
}

void WaveObjectManager::addServiceIndependentOperationMap (UI32 operationCode, ManagementInterfaceServiceIndependentMessageHandler pManagementInterfaceServiceIndependentMessageHandler)
{
    ServiceIndependentMessageHandlerMap::addOperationMap (getServiceId (), operationCode, pManagementInterfaceServiceIndependentMessageHandler);
}

void WaveObjectManager::removeServiceIndependentOperationMap (UI32 operationCode)
{
    ServiceIndependentMessageHandlerMap::removeOperationMap (getServiceId (), operationCode);
}

void WaveObjectManager::removeOperationMap (const UI32 &operationCode)
{
    map<UI32, PrismOperationMapContext *>::iterator element = m_operationsMap.find (operationCode);
    map<UI32, PrismOperationMapContext *>::iterator end     = m_operationsMap.end ();

    if (end == element)
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::removeOperationMap : OperationMap is NOT found for this operation code : ") + operationCode);
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    delete (element->second);
    m_operationsMap.erase (element);

    m_createMessageInstanceWrapperMutex.lock ();

    map<UI32, PrismElement *>::iterator element1    = m_ownersForCreatingMessageInstances.find (operationCode);
    map<UI32, PrismElement *>::iterator endElement1 = m_ownersForCreatingMessageInstances.end ();

    if (endElement1 == element1)
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::removeOperationMap : Trying to remove non-existant Owner.  Owner for \"") + operationCode + string ("\" was NOT set."));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_ownersForCreatingMessageInstances.erase (element1);
    }

    m_createMessageInstanceWrapperMutex.unlock ();
}

void WaveObjectManager::addEventListener (const UI32 &eventOperationCode, const PrismServiceId &listenerPrismServiceId, const LocationId &listenerLocationId)
{
    // This member function can be called from multiple threads.  So we need to make it thread safe.

    s_mutexForAddingEventListener.lock ();

    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator element = m_eventListenersMap.find (eventOperationCode);
    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator end     = m_eventListenersMap.end ();

    if (end == element)
    {
        m_eventListenersMap[eventOperationCode] = new vector<PrismEventListenerMapContext *>;
    }

    (m_eventListenersMap[eventOperationCode])->push_back (new PrismEventListenerMapContext (listenerPrismServiceId, listenerLocationId));

    trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::addEventListener : Number Of Event Liusteners For Event : ") + eventOperationCode + string (" are : ") + (m_eventListenersMap[eventOperationCode])->size () + " on Service " + m_name);

    s_mutexForAddingEventListener.unlock ();
}

void WaveObjectManager::getEventListeners (const UI32 &eventOperationCode, vector<PrismEventListenerMapContext *> &eventListeners)
{
    s_mutexForAddingEventListener.lock ();

    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator element = m_eventListenersMap.find (eventOperationCode);
    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator end     = m_eventListenersMap.end ();

    if (element != end)
    {
        vector<PrismEventListenerMapContext *>::iterator element1 = (element->second)->begin ();
        vector<PrismEventListenerMapContext *>::iterator end1     = (element->second)->end ();

        while (element1 != end1)
        {
            PrismEventListenerMapContext *pContext     = *element1;
            PrismEventListenerMapContext *pTempContext = new PrismEventListenerMapContext (pContext->getEventListenerServiceId (), pContext->getEventListenerLocationId ());

            eventListeners.push_back (pTempContext);

            element1++;
        }
    }

    s_mutexForAddingEventListener.unlock ();
}

void WaveObjectManager::removeEventListener (const UI32 &eventOperationCode, const PrismServiceId &listenerPrismServiceId, const LocationId &listenerLocationId)
{
    // This member function can be called from multiple threads.  So we need to make it thread safe.

    s_mutexForAddingEventListener.lock ();

    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator element = m_eventListenersMap.find (eventOperationCode);
    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator end     = m_eventListenersMap.end ();

    if (end != element)
    {
        vector<PrismEventListenerMapContext *>::iterator element1 = (*(element->second)).begin ();
        vector<PrismEventListenerMapContext *>::iterator end1     = (*(element->second)).end ();

        while (element1 != end1)
        {
            PrismEventListenerMapContext *pTempContext = *element1;

            if ((listenerPrismServiceId == (pTempContext->getEventListenerServiceId ())) && (listenerLocationId == (pTempContext->getEventListenerLocationId ())))
            {
                delete pTempContext;

                (*(element->second)).erase (element1);
                trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::removeEventListener : Removing an event Listener : ServiceId : ") + listenerPrismServiceId + " : LocationId : " + listenerLocationId);

                break;
            }

            element1++;
        }
    }

    s_mutexForAddingEventListener.unlock ();
}

void WaveObjectManager::addEventType (const UI32 &eventOperationCode)
{
    s_supportedEventsMutex.lock ();

    m_supportedEvents[eventOperationCode] = eventOperationCode;

    s_supportedEventsMutex.unlock ();
}

void WaveObjectManager::listenForEvent (PrismServiceId prismServiceCode, UI32 sourceOperationCode, PrismEventHandler pPrismEventHandler, PrismElement *pPrismElement, const LocationId &sourceLocationId)
{
    PrismServiceId prismServiceId = prismServiceCode;

    if (NULL == pPrismElement)
    {
        pPrismElement = this;
    }

    // If we are listening for WAVE_OBJECT_MANAGER_ANY_EVENT then we do not care about the service code.  We treat that we are listening to
    // null service.

    if (WAVE_OBJECT_MANAGER_ANY_EVENT == sourceOperationCode)
    {
        prismServiceId = 0;
    }

    LocationId thisLocationId      = FrameworkToolKit::getThisLocationId ();
    LocationId effectiveLocationId = sourceLocationId;

    // FIXME : declare a NullLocationId instead of using 0

    // We will change the effectiveLocationId only if the event is not WAVE_OBJECT_MANAGER_ANY_EVENT

    if (WAVE_OBJECT_MANAGER_ANY_EVENT != sourceOperationCode)
    {
        if (0 == effectiveLocationId)
        {
            if (true != (FrameworkToolKit::isALocalService (prismServiceId)))
            {
                effectiveLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
            }
        }
    }

    if (0 == effectiveLocationId)
    {
        effectiveLocationId = thisLocationId;
    }
    // Add an event map so that when the event arrives we can execute the corresponding event handler.

    map <LocationId, map<UI32, map<UI32, PrismEventMapContext *> *> *>::iterator element = m_eventsMap.find (effectiveLocationId);
    map <LocationId, map<UI32, map<UI32, PrismEventMapContext *> *> *>::iterator end     = m_eventsMap.end ();

    if (end == element)
    {
        m_eventsMap[effectiveLocationId] = new map<UI32, map<UI32, PrismEventMapContext *> *>;
    }

    map<UI32, map<UI32, PrismEventMapContext *> *>::iterator element1 = (m_eventsMap[effectiveLocationId])->find (prismServiceId);
    map<UI32, map<UI32, PrismEventMapContext *> *>::iterator end1     = (m_eventsMap[effectiveLocationId])->end ();

    if (end1 == element1)
    {
        (*(m_eventsMap[effectiveLocationId]))[prismServiceId] = new map<UI32, PrismEventMapContext *>;
    }

    // FIXME : currently if same entry is added multiple times, only the final entry remains in effect.  May be we ned to assert if we encounter mutiple additions.

    (*((*(m_eventsMap[effectiveLocationId]))[prismServiceId]))[sourceOperationCode] = new PrismEventMapContext (pPrismElement, pPrismEventHandler);

    // Update the event source service that we are interested to receive the particular events.
    // If the ObjectManager exists locally (meaning event source exists on this location) we simply execute a method on the source object manager.
    // Otherwise we send a message to the remote event source object manager.

    if (thisLocationId == effectiveLocationId)
    {
        WaveObjectManager *pWaveObjectManager = NULL;

        pWaveObjectManager = PrismMessageFactory::getWaveObjectManagerForEvent (prismServiceId, sourceOperationCode);

        // The remote transport service listens for any events.  The way it achieves listeneing for any event is that it sytart listening
        // for any event that is generated by the null service.  So the remote transport service does not have to specify a valid service with an object manager.

        if (((0 != prismServiceId) || (WAVE_OBJECT_MANAGER_ANY_EVENT != sourceOperationCode)) && (NULL == pWaveObjectManager))
        {
            trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::listenForEvent : There is no Object Manager to generate the specified event. ServiceId = ") + prismServiceId + ", Source OperationCode = " + sourceOperationCode + ".");
            prismAssert (false, __FILE__, __LINE__);
            return;
        }

        // Now add the listener to the object manager.

        if (NULL != pWaveObjectManager)
        {
            pWaveObjectManager->addEventListener (sourceOperationCode, getServiceId (), FrameworkToolKit::getThisLocationId ());
        }
    }
    else
    {
        // If the event that we are listening to is anything other than WAVE_OBJECT_MANAGER_ANY_EVENT then only send the remote message.
        // If the event to listen is WAVE_OBJECT_MANAGER_ANY_EVENT then it can be never on a remote location.  This event is only used
        // by the remote transport service to accept all events so that it can transport to remote location.

        if ((WAVE_OBJECT_MANAGER_ANY_EVENT == sourceOperationCode) && (thisLocationId != effectiveLocationId))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::listenForEvent : We can never listen for WAVE_OBJECT_MANAGER_ANY_EVENT from remote locations.");
            prismAssert (false, __FILE__, __LINE__);
        }

        if (WAVE_OBJECT_MANAGER_ANY_EVENT != sourceOperationCode)
        {
            WaveObjectManagerRegisterEventListenerMessage *pMessage = new WaveObjectManagerRegisterEventListenerMessage (prismServiceId, sourceOperationCode, getServiceId (), thisLocationId);

            WaveMessageStatus status = sendOneWay (pMessage);

            prismAssert (WAVE_MESSAGE_SUCCESS == status, __FILE__, __LINE__);
        }
    }
}

void WaveObjectManager::unlistenEvents ()
{
    map<LocationId, map<UI32, map<UI32, PrismEventMapContext *> *> *>::iterator element               = m_eventsMap.begin ();
    map<LocationId, map<UI32, map<UI32, PrismEventMapContext *> *> *>::iterator end                   = m_eventsMap.end ();
    LocationId                                                                  eventSourceLocationId = 0;
    PrismServiceId                                                              eventSourceServiceId  = 0;
    UI32                                                                        eventOperationCode    = 0;

    while (element != end)
    {
        eventSourceLocationId = element->first;

        map<UI32, map<UI32, PrismEventMapContext *> *>::iterator element1 = (*(element->second)).begin ();
        map<UI32, map<UI32, PrismEventMapContext *> *>::iterator end1     = (*(element->second)).end ();

        while (element1 != end1)
        {
            eventSourceServiceId = element1->first;

            map<UI32, PrismEventMapContext *>::iterator element2 = (*(element1->second)).begin ();
            map<UI32, PrismEventMapContext *>::iterator end2     = (*(element1->second)).end ();

            while (element2 != end2)
            {
                eventOperationCode = element2->first;

                trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::unlistenEvents : Event : LocationId : ") + eventSourceLocationId + " ServiceId : " + eventSourceServiceId + " OperationCode : " + eventOperationCode);

                const LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();

                if (thisLocationId == eventSourceLocationId)
                {
                    WaveObjectManager *pWaveObjectManager = NULL;

                    pWaveObjectManager = PrismMessageFactory::getWaveObjectManagerForEvent (eventSourceServiceId, eventOperationCode);

                    // The remote transport service listens for any events.  The way it achieves listeneing for any event is that it sytart listening
                    // for any event that is generated by the null service.  So the remote transport service does not have to specify a valid service with an object manager.

                    if (((0 != eventSourceServiceId) || (WAVE_OBJECT_MANAGER_ANY_EVENT != eventOperationCode)) && (NULL == pWaveObjectManager))
                    {
                        trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::unlistenEvents : There is no Object Manager to generate the specified event. ServiceId = ") + eventSourceServiceId + ", Source OperationCode = " + eventOperationCode + ".");
                        prismAssert (false, __FILE__, __LINE__);
                        return;
                    }

                    // Now add the listener to the object manager.

                    if (NULL != pWaveObjectManager)
                    {
                        pWaveObjectManager->removeEventListener (eventOperationCode, getServiceId (), FrameworkToolKit::getThisLocationId ());
                    }
                }
                else
                {
                    // FIXME : sagar : send remote message to remove event listener.
                }

                delete element2->second;

                element2++;
            }

            delete element1->second;

            element1++;
        }

        delete element->second;

        element++;
    }

    m_eventsMap.clear ();

    trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::unlistenEvents : Finished unlistening all the events for this service.");
}

void WaveObjectManager::addResponseMap (UI32 prismMessageId, PrismMessageResponseContext *pPrismMessageResponseContext)
{
    m_responsesMapMutex.lock ();
    m_responsesMap[prismMessageId] = pPrismMessageResponseContext;
    m_responsesMapMutex.unlock ();
}

void WaveObjectManager::addManagedClass (const string &managedClassName, PrismElement *pOwnerForInstantiation)
{
    if (false == (isManagedClassSupported (managedClassName)))
    {
        if (true == OrmRepository::isManagedClassAView (managedClassName))
        {
            m_managedViews[managedClassName] = managedClassName;
        }
        else
        {
            m_managedClasses[managedClassName] = managedClassName;
        }

//        m_managedClasses[managedClassName] = managedClassName;
        addOwnerForManagedClass (managedClassName, this);

        if (NULL != m_pAssociatedVirtualWaveObjectManager)
        {
            m_pAssociatedVirtualWaveObjectManager->addManagedClassWithoutOwnership (managedClassName);
        }

        m_createManagedObjectInstanceWrapperMutex.lock ();

        map<string, PrismElement *>::iterator element    = m_ownersForCreatingManagedObjectInstances.find (managedClassName);
        map<string, PrismElement *>::iterator endElement = m_ownersForCreatingManagedObjectInstances.end ();

        if (endElement != element)
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addManagedClass : Trying to set duplicate Owner.  Owner for \"" + managedClassName + "\" was already set.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            if (NULL == pOwnerForInstantiation)
            {
                pOwnerForInstantiation = this;
            }

            m_ownersForCreatingManagedObjectInstances[managedClassName] = pOwnerForInstantiation;
        }

        m_createManagedObjectInstanceWrapperMutex.unlock ();

    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addManagedClass : Entry for Managed Class " + managedClassName + " already exists.  Duplicate entries are not allowed.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveObjectManager::addManagedClassWithoutOwnership (const string &managedClassName)
{
    if (false == (isManagedClassSupported (managedClassName)))
    {
        if (true == OrmRepository::isManagedClassAView (managedClassName))
        {
            m_managedViews[managedClassName] = managedClassName;
        }
        else
        {
            m_managedClasses[managedClassName] = managedClassName;
        }

//         m_managedClasses[managedClassName] = managedClassName;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addManagedClassWithoutOwnership : Entry for Managed Class " + managedClassName + " already exists.  Duplicate entries are not allowed.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveObjectManager::addOwnerForManagedClass (const string &managedClassName, WaveObjectManager *pWaveObjectManager)
{
    s_allManagedClassesAndOwnersMapMutex.lock ();

    map<string, WaveObjectManager *>::iterator element = s_allManagedClassesAndOwnersMap.find (managedClassName);
    map<string, WaveObjectManager *>::iterator end     = s_allManagedClassesAndOwnersMap.end ();

    if (element == end)
    {
        s_allManagedClassesAndOwnersMap[managedClassName] = pWaveObjectManager;
    }
    else
    {
        WaveObjectManager *pExistingOwner = element->second;

        WaveNs::trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addOwnerForManagedClass : Trying add a Duplicate Owner for Managed Class : " + managedClassName + ", New Owner : " + pWaveObjectManager->getName () + ", Existing Owner : " + pExistingOwner->getName ());
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    s_allManagedClassesAndOwnersMapMutex.unlock ();
}

WaveObjectManager *WaveObjectManager::getOwnerForManagedClass (const string &managedClassName)
{
    WaveObjectManager *pWaveObjectManager = NULL;

    s_allManagedClassesAndOwnersMapMutex.lock ();

    map<string, WaveObjectManager *>::iterator element = s_allManagedClassesAndOwnersMap.find (managedClassName);
    map<string, WaveObjectManager *>::iterator end     = s_allManagedClassesAndOwnersMap.end ();

    if (element != end)
    {
        pWaveObjectManager = element->second;
    }

    s_allManagedClassesAndOwnersMapMutex.unlock ();

    return (pWaveObjectManager);
}

WaveObjectManager::PrismMessageResponseContext *WaveObjectManager::removeResponseMap (UI32 prismMessageId)
{
    PrismMessageResponseContext *pPrismMessageResponseContext = NULL;

    m_responsesMapMutex.lock ();

    map<UI32, PrismMessageResponseContext *>::iterator element = m_responsesMap.find (prismMessageId);
    map<UI32, PrismMessageResponseContext *>::iterator end     = m_responsesMap.end ();

    if (end != element)
    {
        pPrismMessageResponseContext = element->second;

        m_responsesMap.erase (element);
    }

    m_responsesMapMutex.unlock ();

    return (pPrismMessageResponseContext);
}

WaveObjectManager::PrismOperationMapContext *WaveObjectManager::getPrismMessageHandler (UI32 operationCode, UI32 messageHandlerServiceCode, UI32 thisServiceId)
{
    PrismOperationMapContext *pTemp = NULL;

    if (messageHandlerServiceCode == thisServiceId)
    {
        pTemp = m_operationsMap[operationCode];
    }

    // If we could not find the exeact match then look for WAVE_OBJECT_MANAGER_ANY_OPCODE opcode.
    // Because, e know that this specific opcode supports any PrismMessage or its derivation.

    if (NULL == pTemp)
    {
        pTemp = m_operationsMap[WAVE_OBJECT_MANAGER_ANY_OPCODE];
    }

    return (pTemp);
}

PrismMessage *WaveObjectManager::getPInputMesage () const
{
    return (m_pInputMessage);
}

WaveObjectManager::PrismEventMapContext *WaveObjectManager::getPrismEventHandler (const LocationId &eventSourceLocationId, const PrismServiceId &eventSourceServiceId, const UI32 &eventOperationCode)
{
    // If the eventOperationCode is WAVE_OBJECT_MANAGER_ANY_EVENT we do not care about the service and the corresponding service must have registered with null service id for listeneing.

    PrismServiceId        effectiveEventSourceServiceId = (WAVE_OBJECT_MANAGER_ANY_EVENT == eventOperationCode) ? 0 : eventSourceServiceId;
    PrismEventMapContext *pTemp                         = (*((*(m_eventsMap[eventSourceLocationId]))[effectiveEventSourceServiceId]))[eventOperationCode];

    return (pTemp);
}

void WaveObjectManager::setAssociatedPrismThread (PrismThread *pAssociatedPrismThread)
{
    if (NULL != m_pAssociatedPrismThread)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::setAssociatedPrismThread : This Object Manager has already been associated with another Prism Thread.");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_pAssociatedPrismThread = pAssociatedPrismThread;

    // FIXME : sagar : The following seems to be obsolete code and it is not needed anymore.
    //                 After confirming that this code is not required (perform extended tests)
    //                 remove the code and also remove the corresponding static member variable declarations in the .h file.

//    if (NULL != pAssociatedPrismThread)
//    {
//        m_registeredPrismServicesMutex.lock ();
//        m_registeredPrismServices.insert (m_registeredPrismServices.end (), pAssociatedPrismThread->getPrismServiceId ());
//        m_registeredPrismServicesMutex.unlock ();
//    }
}

bool WaveObjectManager::isOperationCodeSupported (UI32 operationCode)
{
    if (NULL != (getPrismMessageHandler (operationCode)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool WaveObjectManager::isEventOperationCodeSupported (UI32 eventOperationCode)
{
    s_supportedEventsMutex.lock ();

    map<UI32, UI32>::iterator element          = m_supportedEvents.find (eventOperationCode);
    map<UI32, UI32>::iterator end              = m_supportedEvents.end ();
    bool                      isEventSupported = false;

    if (end != element)
    {
        isEventSupported = true;
    }

    s_supportedEventsMutex.unlock ();

    return (isEventSupported);
}

bool WaveObjectManager::isEventOperationCodeSupportedForListening (const LocationId &eventSourceLocationId, const PrismServiceId &eventSourceServiceId, const UI32 &eventOperationCode)
{
    if (NULL != (getPrismEventHandler (eventSourceLocationId, eventSourceServiceId, eventOperationCode)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool WaveObjectManager::isManagedClassSupported (const string &managedClass)
{
    map<string, string>::iterator element = m_managedClasses.find (managedClass);
    map<string, string>::iterator end     = m_managedClasses.end ();

    if (end != element)
    {
        return (true);
    }
    else
    {
        map<string, string>::iterator element1  = m_managedViews.find (managedClass);
        map<string, string>::iterator end1      = m_managedViews.end ();

        if (end1 != element1)
        {
            return (true);
        }
        else
        {
            return (false);
        }

//        return (false);
    }
}

WaveObjectManager::PrismMessageResponseContext *WaveObjectManager::getResponseContext (UI32 prismMessageId)
{
    PrismMessageResponseContext *pPrismMessageResponseContext = NULL;

    m_responsesMapMutex.lock ();

    map<UI32, PrismMessageResponseContext *>::iterator element = m_responsesMap.find (prismMessageId);
    map<UI32, PrismMessageResponseContext *>::iterator end     = m_responsesMap.end ();

    if (end != element)
    {
        pPrismMessageResponseContext = element->second;
    }

    m_responsesMapMutex.unlock ();

    return (pPrismMessageResponseContext);
}

bool WaveObjectManager::isAKnownMessage (UI32 prismMessageId)
{
    // Please refer to the comment in the send method just above the corresponding locking
    // method.  The corresponssing lock line looks just like the line below

    m_sendReplyMutexForResponseMap.lock ();

    if (NULL != (getResponseContext (prismMessageId)))
    {
        m_sendReplyMutexForResponseMap.unlock ();
        return (true);
    }
    else
    {
        m_sendReplyMutexForResponseMap.unlock ();
        return (false);
    }
}

void WaveObjectManager::handlePrismMessage (PrismMessage *pPrismMessage)
{
    PrismOperationMapContext *pPrismOperationMapContext = getPrismMessageHandler (pPrismMessage->getOperationCode (), pPrismMessage->getServiceCode (), getServiceId ());

    if (NULL != pPrismOperationMapContext)
    {
        prismAssert (NULL == m_pInputMessage, __FILE__, __LINE__);
        m_pInputMessage = pPrismMessage;

        addMessageToMessageHistoryCalledFromHandle (pPrismMessage);

        pPrismOperationMapContext->executeMessageHandler (pPrismMessage);
        m_pInputMessage = NULL;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::handlePrismMessage : This type of message is not handled by this Object Manager.");
        prismAssert (false, __FILE__, __LINE__);
        pPrismMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_OPERATION_NOT_SUPPORTED);
        reply (pPrismMessage);
    }
}

void WaveObjectManager::handlePrismEvent (const PrismEvent *&pPrismEvent)
{
    PrismEventMapContext *pPrismEventMapContext = getPrismEventHandler (pPrismEvent->getSenderLocationId (), pPrismEvent->getServiceCode (), pPrismEvent->getOperationCode ());

    if (NULL != pPrismEventMapContext)
    {
        prismAssert (NULL == m_pInputMessage, __FILE__, __LINE__);

        m_pInputMessage = NULL;   // currently we don't support from event because it is const.

        pPrismEventMapContext->executeEventHandler (pPrismEvent);

        m_pInputMessage = NULL;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::handlePrismEvent : This type of event is not handled by this Object Manager.");
        prismAssert (false, __FILE__, __LINE__);
        //pPrismEvent->setCompletionStatus (WAVE_MESSAGE_ERROR_OPERATION_NOT_SUPPORTED); // Noone examines the completionStatus for events.

        // We must reply to the event so that it decrements the reference count for listeners.

        reply (pPrismEvent);
    }
}

WaveMessageStatus WaveObjectManager::postToRemoteLocation (InterLocationMulticastMessage *pPrismMessage, set<LocationId> locationsToSent)
{
    ResourceId status = ((PrismFrameworkObjectManager::getInstance ())->getThisLocation ())->postToRemoteLocation (pPrismMessage, locationsToSent);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
         return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        return (WAVE_MESSAGE_ERROR);
    }
}

WaveMessageStatus WaveObjectManager::postToRemoteLocation (PrismMessage *pPrismMessage)
{
    string         tempString;
    LocationId     destinationLocationId;
    WaveMessageType messageType            = pPrismMessage->getType ();

    // Make sure that the message has been prepared for serialization.

//    pPrismMessage->prepareForSerialization ();

//    pPrismMessage->serialize (tempString);

    if (WAVE_MESSAGE_TYPE_REQUEST == messageType)
    {
        destinationLocationId = pPrismMessage->getReceiverLocationId ();
    }
    else if (WAVE_MESSAGE_TYPE_RESPONSE == messageType)
    {
        destinationLocationId = pPrismMessage->getSenderLocationId ();
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::postToRemoteLocation : Currently, we can only post Request Messages and their Response Messages across locations.");
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_UNKNOWN_TYPE);
    }

//    ResourceId status = ((PrismFrameworkObjectManager::createInstance ())->getThisLocation ())->postToRemoteLocation (tempString, destinationLocationId);
    ResourceId status = ((PrismFrameworkObjectManager::getInstance ())->getThisLocation ())->postToRemoteLocation (pPrismMessage, destinationLocationId);


    if(WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER == status)
    {
    return WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER;
    }
    else if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION);
    }
    else
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
}

WaveMessageStatus WaveObjectManager::postToHaPeerLocation (PrismMessage *pPrismMessage)
{
    ResourceId status = ((PrismFrameworkObjectManager::getInstance ())->getThisLocation ())->postToHaPeerLocation (pPrismMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION);
    }
    else
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
}

WaveMessageStatus WaveObjectManager::send (PrismMessage *pPrismMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext, UI32 timeOutInMilliSeconds, LocationId locationId, PrismElement *pPrismMessageSender)
{
    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::send : Trying to send a message with NULL message.");
        return (WAVE_MESSAGE_ERROR_NULL_MESSAGE);
    }

    if ((NULL == pPrismMessage) || (((PrismMessageResponseHandler) NULL) == pPrismMessageCallback))
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::send : Trying to send a message with NULL callback.  If you do not want to register a callback send it as a one way message.");
        return (WAVE_MESSAGE_ERROR_NULL_CALLBACK);
    }

    PrismThread *pPrismThread = NULL;

    LocationId thisLocationId      = FrameworkToolKit::getThisLocationId ();
    LocationId effectiveLocationId = locationId;

    // FIXME : declare a NullLocationId instead of using 0

    if (0 == effectiveLocationId)
    {
        if (true != (FrameworkToolKit::isALocalService (pPrismMessage->getServiceCode ())))
        {
            effectiveLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
            LocationId myLocationId = FrameworkToolKit::getMyLocationId ();
            if (effectiveLocationId == myLocationId)
            {
                effectiveLocationId = 0;
            }
        }
    }

    if ((0 != effectiveLocationId) && (effectiveLocationId != thisLocationId) && (false == FrameworkToolKit::isAKnownLocation (effectiveLocationId)) && (1 != effectiveLocationId))

    {
        trace (TRACE_LEVEL_ERROR, string("WaveObjectManager::send: Location is invalid: Loc : ") + locationId);
        return (WAVE_MESSAGE_ERROR);
    }

    if ((0 == effectiveLocationId) || (thisLocationId == effectiveLocationId))
    {
        pPrismThread = PrismThread::getPrismThreadForServiceId (pPrismMessage->getServiceCode ());
    }
    else if (1 == effectiveLocationId)
    {
        pPrismThread = PrismThread::getPrismThreadForMessageHaPeerTransport ();
    }
    else
    {
        pPrismThread = PrismThread::getPrismThreadForMessageRemoteTransport ();
    }

    if (NULL == pPrismThread)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::send : No Service registered to accept this service Id ") + pPrismMessage->getServiceCode () + ".");
        return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE);
    }

    if (false == (pPrismThread->hasWaveObjectManagers ()))
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::send : Service identified.  But there are no Prism Object Managers registered to process any kind of requests.");
        return (WAVE_MESSAGE_ERROR_NO_OMS_FOR_SERVICE);
    }

    // Set this so that the message can be returned after getting processed.

    pPrismMessage->m_senderServiceCode = getServiceId ();

    // Store the receiver LocationId.

    UI32 prismMessageId = pPrismMessage->getMessageId ();

    pPrismMessage->m_receiverLocationId = (effectiveLocationId != 0) ? effectiveLocationId : thisLocationId;

    if (NULL != (getResponseContext (prismMessageId)))
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::send : This message (with id : ") + prismMessageId + "was already registered.");
        return (WAVE_MESSAGE_ERROR_DUPLICATE_MESSAGE_SEND);
    }

    addMessageToMessageHistoryCalledFromSend (pPrismMessage);

    // The following lock is used so that the response for the submitted message does not arrive before we finish adding the messages
    // Response context.  If the message response arrive before we add the response context, we will fail the response delivery.
    // This does not happen in normal cases since the same thread is sumbitting the messages and the same thread picks up the response
    // for processing.  But in some cases, Multiple threads  use the send of the same object manager's send.  In this case the threads
    // are typically different the the object manager's thread.  But the reply is received on the obkject manager's thread.  If there is
    // no  co-ordination between the send and reply threads for the same message, it is possible that we can receive a response before
    // the message response context has been added to the response map.  It is very rare but we have already seen it happen under heavy
    // stress on the system.

    m_sendReplyMutexForResponseMap.lock ();

    if ((NULL != m_pInputMessage) && (m_pAssociatedPrismThread->getId () == pPrismMessage->getPrismMessageCreatorThreadId ()))
    {
        // Propagate message flags from Inoming Message to Outgoing Message

        pPrismMessage->setIsConfigurationChange                 (m_pInputMessage->getIsConfigurationChange ());
        pPrismMessage->setIsConfigurationTimeChange             (m_pInputMessage->getIsConfigurationTimeChange ());
        pPrismMessage->setTransactionCounter                    (m_pInputMessage->getTransactionCounter ());

        if (false == pPrismMessage->getIsPartitionNameSetByUser ())
        {
            pPrismMessage->setPartitionName                         (m_pInputMessage->getPartitionName ());                         // Propagate the Partition name from Input Message.
        }
        else
        {
            // pass
            // Do not overwrite the partition name with partitionName in m_pInputMessage.
        }

        pPrismMessage->setPartitionLocationIdForPropagation     (m_pInputMessage->getPartitionLocationIdForPropagation ());
        pPrismMessage->setIsPartitionContextPropagated          (m_pInputMessage->getIsPartitionContextPropagated ());

        pPrismMessage->setIsALastConfigReplay                   (m_pInputMessage->getIsALastConfigReplay ());
        pPrismMessage->addXPathStringsVectorForTimestampUpdate  (m_pInputMessage->getXPathStringsVectorForTimestampUpdate ());  // if receiver service is "management interface", should this be skipped ?

        // This is required because sendToWaveCluster also uses the send method. As send and sendToWaveCluster will be having same m_pInputMessage with
        // the flags always set to false. Even if for surrogating message sendToWaveCluster sets the flag, it will be otherwise cleared off here.

        if (false == pPrismMessage->getIsMessageBeingSurrogatedFlag ())
        {
            pPrismMessage->setIsMessageBeingSurrogatedFlag  (m_pInputMessage->getIsMessageBeingSurrogatedFlag ());
            pPrismMessage->setSurrogatingForLocationId      (m_pInputMessage->getSurrogatingForLocationId ());
        }
    }

    if ((true == pPrismMessage->getIsAConfigurationIntent ()) && (true == PersistenceLocalObjectManager::getLiveSyncEnabled ()))
    {
        // Send a configuration intent to the HA peer

        ResourceId configurationIntentStatus = sendOneWayForStoringConfigurationIntent (pPrismMessage);

        if (WAVE_MESSAGE_SUCCESS == configurationIntentStatus)
        {
            pPrismMessage->setIsConfigurationIntentStored (true);
        }
        else
        {
            // Do not penalize the actual configuration.  Flag an error for now.

            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::send : Failed to store the configuration intent on HA peer for messageId : ") + pPrismMessage->getMessageId () + ", handled by service code : " + pPrismMessage->getServiceCode () + ".");

            pPrismMessage->setIsConfigurationIntentStored (false);
        }
    }

    WaveMessageStatus status = pPrismThread->submitMessage (pPrismMessage);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        // Now store the details related to callback so that we can call the appropriate callback when the reply to this message arrives.

        PrismMessageResponseContext *pPrismMessageResponseContext = new PrismMessageResponseContext (pPrismMessage, pPrismMessageSender != NULL ? pPrismMessageSender : this, pPrismMessageCallback, pPrismMessageContext);

        if (m_pAssociatedPrismThread->getId () == pPrismMessage->getPrismMessageCreatorThreadId ())
        {
            pPrismMessageResponseContext->setPInputMessageInResponseContext (m_pInputMessage);
        }

        prismAssert (NULL != pPrismMessageResponseContext, __FILE__, __LINE__);

        if (NULL == pPrismMessageResponseContext)
        {
            status = WAVE_MESSAGE_ERROR_MEMORY_EXHAUSTED;
        }
        else
        {
            addResponseMap (prismMessageId, pPrismMessageResponseContext);
        }

        // If user requested for a message timeout then start the timer.

        if (0 != timeOutInMilliSeconds)
        {
            TimerHandle timerHandle;

            ResourceId timeStatus = startTimer (timerHandle, timeOutInMilliSeconds, reinterpret_cast<PrismTimerExpirationHandler> (&WaveObjectManager::sendTimerExpiredCallback), reinterpret_cast<void *> (prismMessageId));

            if (FRAMEWORK_SUCCESS != timeStatus)
            {
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                pPrismMessageResponseContext->setIsTimerStarted (true);
                pPrismMessageResponseContext->setTimerHandle (timerHandle);
            }
        }
    }

    m_sendReplyMutexForResponseMap.unlock ();

    return (status);
}

void WaveObjectManager::sendTimerExpiredCallback (TimerHandle timerHandle, void *pContext)
{
    UI32 prismMessageId = reinterpret_cast<ULI> (pContext);

    handlePrismMessageResponseWhenTimeOutExpired (FRAMEWORK_TIME_OUT, prismMessageId);
}

WaveMessageStatus WaveObjectManager::sendOneWay (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    PrismThread *pPrismThread        = NULL;
    LocationId   thisLocationId      = FrameworkToolKit::getThisLocationId ();
    LocationId   effectiveLocationId = locationId;

    // FIXME : sagar : replace the 0 with NullLocationId

    if (0 == effectiveLocationId)
    {
        if (true != (FrameworkToolKit::isALocalService (pPrismMessage->getServiceCode ())))
        {
            effectiveLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
        }
    }

    if ((0 == effectiveLocationId) || (thisLocationId == effectiveLocationId))
    {
        pPrismThread = PrismThread::getPrismThreadForServiceId (pPrismMessage->getServiceCode ());
    }
    else if (1 == effectiveLocationId)
    {
        pPrismThread = PrismThread::getPrismThreadForMessageHaPeerTransport ();
    }
    else
    {
        pPrismThread = PrismThread::getPrismThreadForMessageRemoteTransport ();
    }

    if (NULL == pPrismThread)
    {
        //trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendOneWay : No Service registered to accept this service Id ") + pPrismMessage->getServiceCode () + ".");
        return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE);
    }

    if (false == (pPrismThread->hasWaveObjectManagers ()))
    {
        //trace (TRACE_LEVEL_ERROR, "WaveObjectManager::sendOneWay : Service identified.  But there are no Prism Object Managers registered to process any kind of reqquests.");
        return (WAVE_MESSAGE_ERROR_NO_OMS_FOR_SERVICE);
    }

    // Set this so the message can be returned.  In fact one way messages can never be returned.  This is set so that
    // in case we need to refer it at the receiver end.

    pPrismMessage->m_senderServiceCode = m_pAssociatedPrismThread->getPrismServiceId ();

    // Store the receiver LocationId.

    pPrismMessage->m_receiverLocationId = (effectiveLocationId != 0) ? effectiveLocationId : thisLocationId;

    // Set the field to indictae the message is a one way message so that when the receiver replies, the framework will
    // not attempt to deliver it back to the original sender.  It will simply destroy the message.

    pPrismMessage->setIsOneWayMessage (true);

    if ((NULL != m_pInputMessage) && (m_pAssociatedPrismThread->getId () == pPrismMessage->getPrismMessageCreatorThreadId ()))
    {
        // Propagate message flags from Inoming Message to Outgoing Message

        pPrismMessage->setIsConfigurationChange                 (m_pInputMessage->getIsConfigurationChange ());
        pPrismMessage->setIsConfigurationTimeChange             (m_pInputMessage->getIsConfigurationTimeChange ());
    }

    addMessageToMessageHistoryCalledFromSend (pPrismMessage);

    WaveMessageStatus status = pPrismThread->submitMessage (pPrismMessage);

    return (status);
}

WaveMessageStatus WaveObjectManager::sendOneWayToFront (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    PrismThread *pPrismThread        = NULL;
    LocationId   thisLocationId      = FrameworkToolKit::getThisLocationId ();
    LocationId   effectiveLocationId = locationId;

    // FIXME : sagar : replace the 0 with NullLocationId

    if (0 == effectiveLocationId)
    {
        if (true != (FrameworkToolKit::isALocalService (pPrismMessage->getServiceCode ())))
        {
            effectiveLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
        }
    }

    if ((0 == effectiveLocationId) || (thisLocationId == effectiveLocationId))
    {
        pPrismThread = PrismThread::getPrismThreadForServiceId (pPrismMessage->getServiceCode ());
    }
    else if (1 == effectiveLocationId)
    {
        pPrismThread = PrismThread::getPrismThreadForMessageHaPeerTransport ();
    }
    else
    {
        pPrismThread = PrismThread::getPrismThreadForMessageRemoteTransport ();
    }

    if (NULL == pPrismThread)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendOneWayToFront : No Service registered to accept this service Id ") + pPrismMessage->getServiceCode () + ".");
        return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE);
    }

    if (false == (pPrismThread->hasWaveObjectManagers ()))
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::sendOneWayToFront : Service identified.  But there are no Prism Object Managers registered to process any kind of reqquests.");
        return (WAVE_MESSAGE_ERROR_NO_OMS_FOR_SERVICE);
    }

    // Set this so the message can be returned.  Infact one way messages can never be returned.  This is set so that
    // in case we need to refer it at the receiver end.

    pPrismMessage->m_senderServiceCode = m_pAssociatedPrismThread->getPrismServiceId ();

    // Store the receiver LocationId.

    pPrismMessage->m_receiverLocationId = (effectiveLocationId != 0) ? effectiveLocationId : thisLocationId;

    // Set the field to indictae the message is a one way message so that when the receiver replies, the framework will
    // not attempt to deliver it back to the original sender.  It will simply destroy the message.

    pPrismMessage->setIsOneWayMessage (true);

    addMessageToMessageHistoryCalledFromSend (pPrismMessage);

    WaveMessageStatus status = pPrismThread->submitMessageAtFront (pPrismMessage);

    return (status);
}

WaveMessageStatus WaveObjectManager::sendSynchronously (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    // NOTICE :
    // In this method the order of lock, wait and unlock are very very important.
    // It has been coded so that there will be no dead locks.
    // If you are modifying the code - DON'T DO IT.  But if you must modify it,
    // PLEASE PLEASE make sure that the existing behavior is not broken.

    PrismThread *pPrismThread        = NULL;
    LocationId   thisLocationId      = FrameworkToolKit::getThisLocationId ();
    LocationId   effectiveLocationId = locationId;

    // FIXME : sagar : replace the 0 with NullLocationId

    if (0 == effectiveLocationId)
    {
        if (true != (FrameworkToolKit::isALocalService (pPrismMessage->getServiceCode ())))
        {
            effectiveLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
            LocationId myLocationId = FrameworkToolKit::getMyLocationId ();
            if (effectiveLocationId == myLocationId)
            {
                effectiveLocationId = 0;
            }
        }
    }

    if ((0 == effectiveLocationId) || (thisLocationId == effectiveLocationId))
    {

        // NOTE : In vCenter feature we have a separate thread spawned by the ObjectManager which sends message to the object manager. So revert back this particular check
        /*if (pPrismMessage->getServiceCode () == m_serviceId)
        {
            string  serviceName = FrameworkToolKit::getServiceNameById (m_serviceId);
            UI32    operationCode      = pPrismMessage->getOperationCode ();
            trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::sendSynchronously : Service [" + serviceName + "] cannot send message [serviceCode=" + m_serviceId + ", operationCode=" + operationCode + "] synchronously to itself."));
            return (WAVE_MESSAGE_ERROR);
        }*/

        pPrismThread = PrismThread::getPrismThreadForServiceId (pPrismMessage->getServiceCode ());

    }
    else if (1 == effectiveLocationId)
    {
        pPrismThread = PrismThread::getPrismThreadForMessageHaPeerTransport ();
    }
    else
    {
        pPrismThread = PrismThread::getPrismThreadForMessageRemoteTransport ();
    }

    if (NULL == pPrismThread)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendSynchronously : No Service registered to accept this service Id ") + pPrismMessage->getServiceCode () + ".");
        return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE);
    }

    if (false == (pPrismThread->hasWaveObjectManagers ()))
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::sendSynchronously : Service identified.  But there are no Prism Object Managers registered to process any kind of reqquests.");
        return (WAVE_MESSAGE_ERROR_NO_OMS_FOR_SERVICE);
    }

    // Set this so the message can be returned.  Infact synchonously sent messages can never be returned.  This is set so that
    // in case we need to refer it at the receiver end.

    pPrismMessage->m_senderServiceCode = m_pAssociatedPrismThread->getPrismServiceId ();

    // Set the filed to indictae the message is a synchronously sent message so that when the receiver replies, the framework will
    // not attempt to deliver it back to the original sender.  It will simply unlock the sending thread.

    pPrismMessage->setIsSynchronousMessage (true);

    // The following (PrismMutex creation and locking it) is done by the sending thread:
    // Now Create a PrismMutex and a corresponding PrismCondition.  Use the standard Mutex-Condition combination
    // logic to synchronize the sender thread and the receiver thread.  The Mutex-Condition logic is very similar
    // to the POSIX equivalents.
    // Store the created PrismMutex and PrismCondition in the PrismMessage.  The receiver thread needs to use
    // them when it does a reply on the message after processing it.

    PrismMutex synchronizingMutex;
    PrismCondition synchronizingCondition (&synchronizingMutex);

    pPrismMessage->setPSynchronizingMutex (&synchronizingMutex);
    pPrismMessage->setPSynchronizingCondition (&synchronizingCondition);

    // Now lock the synchronizing mutex.  So that receiver thread cannot acquire it.

    synchronizingMutex.lock ();

    // Store the receiver LocationId.

    pPrismMessage->m_receiverLocationId = (effectiveLocationId != 0) ? effectiveLocationId : thisLocationId;

    if ((true == pPrismMessage->getIsALastConfigReplay ()) && (m_pAssociatedPrismThread->getPrismServiceId () == PrismFrameworkObjectManager::getPrismServiceId ()))
    {
        // This case is the initial start of the Last Config Replay where the PrismFrameworkPostPersistentBootWorker (PrismFrameworkObjectManager) triggers a last config replay.  We do not want to propagate message flags from the input message to output message here.  Otherwise, the last configuration replayed intent will have incorrect propagated flags.  Only the sendSynchronously () API needs this special handling since triggering the last config replay should only be synchronously replayed.
    }
    else
    {
        if ((NULL != m_pInputMessage) && (m_pAssociatedPrismThread->getId () == pPrismMessage->getPrismMessageCreatorThreadId ()))
        {
            // Propagate message flags from Inoming Message to Outgoing Message

            pPrismMessage->setIsConfigurationChange                 (m_pInputMessage->getIsConfigurationChange ());
            pPrismMessage->setIsConfigurationTimeChange             (m_pInputMessage->getIsConfigurationTimeChange ());
            pPrismMessage->setTransactionCounter                    (m_pInputMessage->getTransactionCounter ());

            if (false == pPrismMessage->getIsPartitionNameSetByUser ())
            {
                pPrismMessage->setPartitionName                         (m_pInputMessage->getPartitionName ());                         // Propagate the Partition name from Input Message.
            }
            else
            {
                // pass
                // Do not overwrite the partition name with partitionName in m_pInputMessage.
            }

            pPrismMessage->setPartitionLocationIdForPropagation     (m_pInputMessage->getPartitionLocationIdForPropagation ());
            pPrismMessage->setIsPartitionContextPropagated          (m_pInputMessage->getIsPartitionContextPropagated ());

            pPrismMessage->setIsALastConfigReplay                   (m_pInputMessage->getIsALastConfigReplay ());
            pPrismMessage->addXPathStringsVectorForTimestampUpdate  (m_pInputMessage->getXPathStringsVectorForTimestampUpdate ());  // if receiver service is "management interface", should this be skipped ?

            // This is required because sendToWaveCluster also uses the send method. As send and sendToWaveCluster will be having same m_pInputMessage with
            // the flags always set to false. Even if for surrogating message sendToWaveCluster sets the flag, it will be otherwise cleared off here.

            if (false == pPrismMessage->getIsMessageBeingSurrogatedFlag ())
            {
                pPrismMessage->setIsMessageBeingSurrogatedFlag  (m_pInputMessage->getIsMessageBeingSurrogatedFlag ());
                pPrismMessage->setSurrogatingForLocationId      (m_pInputMessage->getSurrogatingForLocationId ());
            }
        }
    }

    addMessageToMessageHistoryCalledFromSend (pPrismMessage);

    if ((true == pPrismMessage->getIsAConfigurationIntent ()) && (true == PersistenceLocalObjectManager::getLiveSyncEnabled ()))
    {
        // Send a configuration intent to the HA peer

        ResourceId configurationIntentStatus = sendOneWayForStoringConfigurationIntent (pPrismMessage);

        if (WAVE_MESSAGE_SUCCESS == configurationIntentStatus)
        {
            pPrismMessage->setIsConfigurationIntentStored (true);
        }
        else
        {
            // Do not penalize the actual configuration.  Flag an error for now.

            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendSynchronously : Failed to store the configuration intent on HA peer for messageId : ") + pPrismMessage->getMessageId () + ", message operation code : " + pPrismMessage->getOperationCode () + ", handled by service code : " + pPrismMessage->getServiceCode () + ".");

            pPrismMessage->setIsConfigurationIntentStored (false);
        }
    }

    // Now submit the message to the revceiver thread.  Even if the receiver thread finishes processing
    // the message before we return from the method below, it is perfectly OK.  The receiver thread
    // may finish processing but cannot reply to the message as we are holding the synchronizing mutex.

    WaveMessageStatus status = pPrismThread->submitMessage (pPrismMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // For some reason the message could not be submitted.  We cannot proceed.  Release the acquired
        // Synchronizing mutex and return an error to the caller.

        synchronizingMutex.unlock ();

        if (true == pPrismMessage->getIsConfigurationIntentStored ())
        {
            // Remove configuration intent

            ResourceId configurationIntentStatus = sendOneWayForRemovingConfigurationIntent (pPrismMessage->getMessageId ());

            if (WAVE_MESSAGE_SUCCESS != configurationIntentStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendSynchronously : Failed to remove the configuration intent from HA peer for messageId : ") + pPrismMessage->getMessageId () + ", message operation code : " + pPrismMessage->getOperationCode () + ", handled by service code : " + pPrismMessage->getServiceCode () + ".");
            }
        }

        return (status);
    }

    // Now let us wait for the receiver thread to signal us to resume.  The receiver thread could not
    // have called the resume untill after we call the following wait method as the receiver thread will
    // be waiting to acquire the synchronizing mutex.  It is possible that the receiver can finish processing
    // the message before we return from the above staement but it cannot reply (signal us to continue) untill
    // after we call the following statement (wait is invoked).

    synchronizingCondition.wait ();

    // Now the receiver thread will be able to acquire the synchronizing mutex (since it will be released
    // as part of the above wait call).  So the receiver will be able to do a reply to the message.  The receiver
    // does a resume on the synchronizing condition in the reply processing.  That causes us to get out of
    // our wait above and also atomically locks the synchronizing mutex again.

    // The following code (unlock) also is executed by the sending thread.

    synchronizingMutex.unlock ();

    // By now we ensured that the receiver actually prpocessed the message and replied to it.  This reply is not actually
    // submiting the message back to the original sender.
    // But the receiver thread simply signalled us to continue after finishing processing the message.
    // We can return now.
    // The caller will examine the completion status on the message to obtain the status of completion of processing
    // the message.

    if ((NULL != m_pInputMessage) && (m_pAssociatedPrismThread->getId () == pPrismMessage->getPrismMessageCreatorThreadId ()))
    {
        m_pInputMessage->appendNestedSql (pPrismMessage->getNestedSql ());
    }

    if (true == pPrismMessage->getIsConfigurationIntentStored ())
    {
        // Remove configuration intent

        ResourceId configurationIntentStatus = sendOneWayForRemovingConfigurationIntent (pPrismMessage->getMessageId ());

        if (WAVE_MESSAGE_SUCCESS != configurationIntentStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendSynchronously : Failed to remove the configuration intent from HA peer for messageId : ") + pPrismMessage->getMessageId () + ", handled by service code : " + pPrismMessage->getServiceCode () + ".");
        }
    }

    return (status);
}

WaveMessageStatus WaveObjectManager::recallButDoNotDeleteResponseMap (PrismMessage *pPrismMessage)
{
    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::recall : Trying to recall a NULL message.");
        return (WAVE_MESSAGE_ERROR_NULL_MESSAGE);
    }

    PrismThread *pPrismThread        = NULL;
    LocationId   thisLocationId      = FrameworkToolKit::getThisLocationId ();
    LocationId   locationId          = pPrismMessage->getReceiverLocationId ();
    LocationId   effectiveLocationId = locationId;

    // FIXME : sagar : replace the 0 with NullLocationId

    if (0 == effectiveLocationId)
    {
        if (true != (FrameworkToolKit::isALocalService (pPrismMessage->getServiceCode ())))
        {
            effectiveLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
        }
    }

    if ((0 == effectiveLocationId) || (thisLocationId == effectiveLocationId))
    {
        pPrismThread = PrismThread::getPrismThreadForServiceId (pPrismMessage->getServiceCode ());
    }
    else
    {
        pPrismThread = PrismThread::getPrismThreadForMessageRemoteTransport ();
    }

    if (NULL == pPrismThread)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::recall : No Service registered to accept this service Id ") + pPrismMessage->getServiceCode () + ".");
        return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE);
    }

    return (pPrismThread->recallMessage (pPrismMessage));
}

WaveMessageStatus WaveObjectManager::recall (PrismMessage *pPrismMessage)
{
    WaveMessageStatus recallStatus = recallButDoNotDeleteResponseMap (pPrismMessage);

    if (WAVE_MESSAGE_SUCCESS == recallStatus)
    {
        PrismMessageResponseContext *pPrismMessageResponseContext = NULL;

        pPrismMessageResponseContext = removeResponseMap (pPrismMessage->getMessageId ());

        prismAssert (NULL != pPrismMessageResponseContext, __FILE__, __LINE__);

        if (NULL != pPrismMessageResponseContext)
        {
            delete pPrismMessageResponseContext;
        }
    }

    return (recallStatus);
}

void WaveObjectManager::trace (TraceLevel traceLevel, const string &stringToTrace)
{
    if (true == (TraceObjectManager::isTracingRequired (traceLevel, getTraceClientId ())))
    {
        TraceObjectManager::traceDirectly (getTraceClientId (), traceLevel, stringToTrace, true, false);
    }
}

void WaveObjectManager::tracePrintf (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...)
{
    if (true == (TraceObjectManager::isTracingRequired (traceLevel, getTraceClientId ())))
    {
        // We cannot use any tracing statement here for obvious reasons (recursion!!!)

        va_list variableArguments;

        va_start (variableArguments, pFormat);

        TraceObjectManager::traceDirectly (getTraceClientId (), traceLevel, addNewLine, suppressPrefix, pFormat, variableArguments);

        va_end (variableArguments);
    }
}

void WaveObjectManager::tracePrintf (TraceLevel traceLevel, const char * const pFormat, ...)
{
    if (true == (TraceObjectManager::isTracingRequired (traceLevel, getTraceClientId ())))
    {
        // We cannot use any tracing statement here for obvious reasons (recursion!!!)

        va_list variableArguments;

        va_start (variableArguments, pFormat);

        TraceObjectManager::traceDirectly (getTraceClientId (), traceLevel, pFormat, variableArguments);

        va_end (variableArguments);
    }
}

void WaveObjectManager::tracePrintf (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, va_list &variableArguments)
{
    if (true == (TraceObjectManager::isTracingRequired (traceLevel, getTraceClientId ())))
    {
        // We cannot use any tracing statement here for obvious reasons (recursion!!!)

        TraceObjectManager::traceDirectly (getTraceClientId (), traceLevel, addNewLine, suppressPrefix, pFormat, variableArguments);
    }
}

void WaveObjectManager::tracePrintf (TraceLevel traceLevel, const char * const pFormat, va_list &variableArguments)
{
    if (true == (TraceObjectManager::isTracingRequired (traceLevel, getTraceClientId ())))
    {
        // We cannot use any tracing statement here for obvious reasons (recursion!!!)

        TraceObjectManager::traceDirectly (getTraceClientId (), traceLevel, pFormat, variableArguments);
    }
}

WaveMessageStatus WaveObjectManager::reply (PrismMessage *pPrismMessage)
{
    // First check if we need to really deliver the reply.
    // If the message was sent synchronously simply resume the sender thread and return.
    // As part of resuming the sender thread we need to first lock the corresponding synchronining
    // PrismMutex and then resume the sender thread and then unlock the PrismMutex.
    // Then the sending thread automatically resumes processing.
    // If the message was sent as a one way message, simply destroy it.
    // Do not attempt to deliver it back to the original sender.

    if (true == (pPrismMessage->getIsSynchronousMessage ()))
    {
        prismAssert (true == (pPrismMessage->getIsLastReply ()), __FILE__, __LINE__);

        addMessageToMessageHistoryCalledFromReply (pPrismMessage);

        (pPrismMessage->getPSynchronizingMutex ())->lock ();
        (pPrismMessage->getPSynchronizingCondition ())->resume ();
        (pPrismMessage->getPSynchronizingMutex ())->unlock ();

        m_pInputMessage = NULL;

        return (WAVE_MESSAGE_SUCCESS);
    }
    else if (true == (pPrismMessage->getIsOneWayMessage ()))
    {
        prismAssert (true == (pPrismMessage->getIsLastReply ()), __FILE__, __LINE__);

        addMessageToMessageHistoryCalledFromReply (pPrismMessage);

        m_pInputMessage = NULL;
        delete pPrismMessage;
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        PrismMessage *pTempPrismMessage = pPrismMessage;

        if (false == (pPrismMessage->getIsLastReply ()))
        {
            pTempPrismMessage = pPrismMessage->clone ();

            pTempPrismMessage->setMessageId (pPrismMessage->getMessageId ());
            pTempPrismMessage->setMessageIdAtOriginatingLocation (pPrismMessage->getMessageIdAtOriginatingLocation ());
            pTempPrismMessage->setOriginalMessageId              (pPrismMessage->getOriginalMessageId ());
            pTempPrismMessage->setWaveClientMessageId            (pPrismMessage->getWaveClientMessageId ());
            pTempPrismMessage->setSenderServiceCode              (pPrismMessage->getSenderServiceCode ());
        }

        PrismThread *pPrismThread = NULL;

        pPrismThread = PrismThread::getPrismThreadForServiceId (pTempPrismMessage->getSenderServiceCode ());

        if (NULL == pPrismThread)
        {
            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::reply : No Service registered to accept reply with this service Id ") + pTempPrismMessage->getSenderServiceCode () + ". How did we receive this message in the first place.  May be the service went down after submitting the request.  Dropping and destroying the message.");

            if (true == (pTempPrismMessage->getIsLastReply ()))
            {
               m_pInputMessage = NULL;
            }

            delete pTempPrismMessage;
            return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE_RESPONSE);
        }

        pTempPrismMessage->setType (WAVE_MESSAGE_TYPE_RESPONSE);

        if (true == (pTempPrismMessage->getIsLastReply ()))
        {
            m_pInputMessage = NULL;
        }

        addMessageToMessageHistoryCalledFromReply (pTempPrismMessage);

        pPrismThread->submitReplyMessage (pTempPrismMessage);

        return (WAVE_MESSAGE_SUCCESS);
    }
}

void WaveObjectManager::handlePrismMessageResponse (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, bool isMessageRecalled)
{
    UI32                         prismMessageId               = pPrismMessage->getMessageId ();
    bool                         isLastReply                  = pPrismMessage->getIsLastReply ();
    PrismMessageResponseContext *pPrismMessageResponseContext = NULL;

    addMessageToMessageHistoryCalledFromHandle (pPrismMessage);

    //if (((FRAMEWORK_TIME_OUT != frameworkStatus) && (true == isLastReply)) || ((FRAMEWORK_TIME_OUT == frameworkStatus) && (true == isMessageRecalled)))
    if (true == isLastReply)
    {
        pPrismMessageResponseContext = removeResponseMap (prismMessageId);
    }
    else
    {
        pPrismMessageResponseContext = getResponseContext (prismMessageId);
    }

    if (NULL != pPrismMessageResponseContext)
    {
        if (true == (pPrismMessageResponseContext->getIsTimerStarted ()))
        {
            deleteTimer (pPrismMessageResponseContext->getTimerHandle ());
        }

        prismAssert (NULL == m_pInputMessage, __FILE__, __LINE__);

        if (false == (pPrismMessageResponseContext->getIsMessageTimedOut ()))
        {
            m_pInputMessage = pPrismMessageResponseContext->getPInputMessageInResponseContext ();
        }

        if (NULL != m_pInputMessage)
        {
            m_pInputMessage->appendNestedSql (pPrismMessage->getNestedSql ());
        }

        if (true == isLastReply && true == pPrismMessage->getIsConfigurationIntentStored ())
        {
            // Remove configuration intent

            ResourceId configurationIntentStatus = sendOneWayForRemovingConfigurationIntent (pPrismMessage->getMessageId ());

            if (WAVE_MESSAGE_SUCCESS != configurationIntentStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::handlePrismMessageResponse : Failed to remove the configuration intent from HA peer for messageId : ") + pPrismMessage->getMessageId () + ", handled by service code : " + pPrismMessage->getServiceCode () + ".");
            }
        }

        pPrismMessageResponseContext->executeResponseCallback (frameworkStatus, pPrismMessage, isMessageRecalled);

        m_pInputMessage = NULL;

        //if (((FRAMEWORK_TIME_OUT != frameworkStatus) && (true == isLastReply)) || ((FRAMEWORK_TIME_OUT == frameworkStatus) && (true == isMessageRecalled)))
        if (true == isLastReply)
        {
            delete pPrismMessageResponseContext;
        }
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveObjectManager::handlePrismMessageResponseWhenTimeOutExpired (FrameworkStatus frameworkStatus, UI32 prismMessageId)
{
    PrismMessageResponseContext *pPrismMessageResponseContext = getResponseContext (prismMessageId);

    if (NULL != pPrismMessageResponseContext)
    {
        pPrismMessageResponseContext->setIsTimerStarted (false);
        pPrismMessageResponseContext->setTimerHandle    (0);

        PrismMessage      *pPrismMessage     = pPrismMessageResponseContext->getPPrismMessage ();
        WaveMessageStatus  status            = recallButDoNotDeleteResponseMap (pPrismMessage);
        PrismMessage      *pTempInputMessage = m_pInputMessage;

        m_pInputMessage = NULL;

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            pPrismMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_RECALLED_DUE_TO_TIME_OUT);
            handlePrismMessageResponse (frameworkStatus, pPrismMessage, true); // Indicate that message has been recalled
        }
        else
        {
            pPrismMessageResponseContext->executeResponseCallback (frameworkStatus);
        }

        m_pInputMessage = pTempInputMessage;
    }
    else
    {
        // Nothing to be done.  The response must have arrived earlier.
    }
}

WaveMessageStatus WaveObjectManager::broadcast (PrismEvent *pPrismEvent)
{
    UI32                                    eventOperationCode     = pPrismEvent->getOperationCode ();
    vector<PrismEventListenerMapContext *> *pEventListeners;
    UI32                                    numberOfEventListeners = 0;
    UI32                                    i                      = 0;
    WaveMessageStatus                      status                 = WAVE_MESSAGE_SUCCESS;

    s_mutexForAddingEventListener.lock ();

    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator element = m_eventListenersMap.find (eventOperationCode);
    map<UI32, vector<PrismEventListenerMapContext *> *>::iterator end     = m_eventListenersMap.end ();

    if (element == end)
    {
        s_mutexForAddingEventListener.unlock ();

        // delete mem if we don't have any listeners
        // DO NOT CALL reply(pPrismEvent) AS THIS MAY SEND REPLY FOR PrismMessage!!!
        if (0 == (pPrismEvent->decrementReferenceCountForEventNotifications ()))
        {
            delete pPrismEvent;
        }

        return (WAVE_MESSAGE_SUCCESS);
    }

    pEventListeners        = element->second;
    numberOfEventListeners = pEventListeners->size ();

    trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::broadcast : Number Of Event Listeners for Event : ") + eventOperationCode + string (" are ") + numberOfEventListeners + string (" on Service : ") + m_name);

    pPrismEvent->m_senderServiceCode = m_pAssociatedPrismThread->getPrismServiceId ();

    // Set the field to indictae the message is a one way message so that when the receiver replies, the framework will
    // not attempt to deliver it back to the original sender.  It will simply destroy the event.

    pPrismEvent->setIsOneWayMessage (true);

    // Set the number of event listeners for the event.  This will be used to reference count and delete it when all the listeners are done.

    pPrismEvent->setReferenceCountForEventNotifications (numberOfEventListeners);

    for (i = 0; i < numberOfEventListeners; i++)
    {
        PrismEventListenerMapContext *pPrismEventListenerContext = (*pEventListeners)[i];

        prismAssert (NULL != pPrismEventListenerContext, __FILE__, __LINE__);

        PrismThread    *pPrismThread               = NULL;
        LocationId      thisLocationId             = FrameworkToolKit::getThisLocationId ();
        PrismServiceId  listenerServiceId          = pPrismEventListenerContext->getEventListenerServiceId ();
        LocationId      effectiveLocationId        = pPrismEventListenerContext->getEventListenerLocationId ();

        // FIXME : sagar : replace the 0 with NullLocationId

        if (0 == effectiveLocationId)
        {
            if (true != (FrameworkToolKit::isALocalService (listenerServiceId)))
            {
                effectiveLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
            }
        }

        if ((0 == effectiveLocationId) || (thisLocationId == effectiveLocationId))
        {
            pPrismThread = PrismThread::getPrismThreadForServiceId (listenerServiceId);
        }
        else
        {
            pPrismThread = PrismThread::getPrismThreadForMessageRemoteTransport ();
        }

        if (NULL == pPrismThread)
        {
            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::broadcast : No Service registered to accept this service Id ") + listenerServiceId + ".");
            reply (pPrismEvent);
            continue;
        }

        if (false == (pPrismThread->hasWaveObjectManagers ()))
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::broadcast : Service identified.  But there are no Prism Object Managers registered to process any kind of requests.");
            reply (pPrismEvent);
            continue;
        }

        // Store the receiver LocationId.
        // Though we store the locationid, this gets overwritten in the next iteraton of the loop.

        pPrismEvent->m_receiverLocationId = (effectiveLocationId != 0) ? effectiveLocationId : thisLocationId;

        status = pPrismThread->submitEvent (pPrismEvent);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // Reply so that the event reference count decrements and eventually gets deleted.

            reply (pPrismEvent);
        }
    }

    s_mutexForAddingEventListener.unlock ();

    return (WAVE_MESSAGE_SUCCESS);
}

WaveMessageStatus WaveObjectManager::reply (const PrismEvent *&pPrismEvent)
{
    // When all the listeners are done with the event, delete the event.

    if (0 == (pPrismEvent->decrementReferenceCountForEventNotifications ()))
    {
        delete pPrismEvent;
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::initializeHandler (PrismInitializeObjectManagerMessage *pInitializeMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::initializeInitializeWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::initializeInitializeSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pInitializeMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::initializeInitializeWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::initializeInitializeWorkersStep : Entering ...");

    PrismInitializeObjectManagerMessage *pPrismInitializeObjectManagerMessage = dynamic_cast<PrismInitializeObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pPrismInitializeObjectManagerMessage, __FILE__, __LINE__);

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::initializeInitializeWorkersStepCallback), pPrismLinearSequencerContext);

        prismAssert (NULL != pWaveAsynchronousContextForBootPhases, __FILE__, __LINE__);

        pWaveAsynchronousContextForBootPhases->setBootReason (pPrismInitializeObjectManagerMessage->getReason ());

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->initialize (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::initializeInitializeWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::initializeInitializeWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::initializeInitializeWorkersStepCallback : Initializing one worker failed.");

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
        //prismAssert (false, __FILE__, __LINE__);
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::initializeInitializeSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::initializeInitializeSelfStep : Entering ...");

    PrismInitializeObjectManagerMessage *pPrismInitializeObjectManagerMessage = dynamic_cast<PrismInitializeObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pPrismInitializeObjectManagerMessage, __FILE__, __LINE__);

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::initializeInitializeSelfStepCallback), pPrismLinearSequencerContext);

    prismAssert (NULL != pWaveAsynchronousContextForBootPhases, __FILE__, __LINE__);

    pWaveAsynchronousContextForBootPhases->setBootReason (pPrismInitializeObjectManagerMessage->getReason ());

    initialize (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::initializeInitializeSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::initializeInitializeSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::listenForEventsHandler (PrismListenForEventsObjectManagerMessage *pListenForEventsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::listenForEventsWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::listenForEventsSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pListenForEventsMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::listenForEventsWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::listenForEventsWorkersStep : Entering ...");

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::listenForEventsWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->listenForEvents (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::listenForEventsWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::listenForEventsWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::listenForEventsWorkersStepCallback : Initializing one worker failed.");

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
        //prismAssert (false, __FILE__, __LINE__);
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::listenForEventsSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::listenForEventsSelfStep : Entering ...");

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::listenForEventsSelfStepCallback), pPrismLinearSequencerContext);

    listenForEvents (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::listenForEventsSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::listenForEventsSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::installHandler (PrismInstallObjectManagerMessage *pInstallMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::installInstallWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::installInstallSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pInstallMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}


void WaveObjectManager::installInstallWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::installInstallWorkersStep : Entering ...");

    PrismInstallObjectManagerMessage *pPrismInstallObjectManagerMessage = reinterpret_cast<PrismInstallObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32                              numberOfWorkers                   = m_workers.size ();
    UI32                              i                                 = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::installInstallWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForBootPhases->setBootReason (pPrismInstallObjectManagerMessage->getReason ());

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->install (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::installInstallWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::installInstallWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                         = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::installInstallWorkersStepCallback : Installing one worker failed.");

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
        //prismAssert (false, __FILE__, __LINE__);
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::installInstallSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::installInstallSelfStep : Entering ...");

    PrismInstallObjectManagerMessage     *pPrismInstallObjectManagerMessage     = reinterpret_cast<PrismInstallObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::installInstallSelfStepCallback), pPrismLinearSequencerContext);

    pWaveAsynchronousContextForBootPhases->setBootReason (pPrismInstallObjectManagerMessage->getReason ());

    install (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::installInstallSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::installInstallSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                         = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::install : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::enableHandler (PrismEnableObjectManagerMessage *pEnableMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::enableEnableWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::enableEnableSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pEnableMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::enableEnableWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::enableEnableWorkersStep : Entering ...");

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::enableEnableWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->enable (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::enableEnableWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::enableEnableWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::enableEnableWorkersStepCallback : Enabling one worker failed.");
        //prismAssert (false, __FILE__, __LINE__);
        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::enableEnableSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::enableEnableSelfStep : Entering ...");

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::enableEnableSelfStepCallback), pPrismLinearSequencerContext);

    enable (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::enableEnableSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::enableEnableSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                         = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    // Indicate that now we are ready to accept Messages to provide services.

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        setIsEnabled (true);

        // Add it to the services that are enabled.

        PrismServiceId thisServiceId = getServiceId ();

        addServiceToEnabledServicesList (thisServiceId);
    }

    delete pWaveAsynchronousContextForBootPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::enable (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::enable : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::upgradeHandler (PrismUpgradeObjectManagerMessage *pUpgradeMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::upgradeDefaultValueStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::upgradeUpgradeWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::upgradeUpgradeSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase = new PrismLinearSequencerContextForUpgradePhase (pUpgradeMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContextForUpgradePhase->start ();
}

void WaveObjectManager::upgradeDefaultValueStep (PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::upgradeDefaultValueStep : Entering ...");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    OrmRepository                     *pOrmRepository       = OrmRepository::getInstance ();
    ManagedObjectSchemaInfoRepository& schemaInfoRepository  = pOrmRepository->getMOSchemaInfoRepository ();

    string defaultValueSql;

    vector <string> newManagedObjects;
    vector <string> removedManagedObjects;
    //vector <string> changedManagedObjects;
    map<string, ModifiedManagedObjectSchemaDifference*> modifiedManagedObjects;
    map<string, string>::iterator managedClassIterator;

    ManagedObjectSchemaDifferenceInfo *pMOSchemaDifference = NULL;
    DatabaseStandaloneTransaction      dbStandaloneTransaction;

    for (managedClassIterator=m_managedClasses.begin (); managedClassIterator!=m_managedClasses.end (); managedClassIterator++)
    {
        pMOSchemaDifference = schemaInfoRepository.findMOInSchemaDifferenceInfo (managedClassIterator->second);

        if (NULL != pMOSchemaDifference)
        {
            if (true == pMOSchemaDifference->getIsNewManagedObject ())
            {
                tracePrintf (TRACE_LEVEL_INFO, "WaveObjectManager::upgradeDefaultValueStepccccc : New MO : %s \n", (pMOSchemaDifference->getManagedObjectName()).c_str());
                newManagedObjects.push_back (pMOSchemaDifference->getManagedObjectName());
                continue;
            }
            else
            {
                tracePrintf (TRACE_LEVEL_INFO, "WaveObjectManager::upgradeDefaultValueStep : Changed MO : %s \n", (pMOSchemaDifference->getManagedObjectName()).c_str());
                //changedManagedObjects.push_back (pMOSchemaDifference->getManagedObjectName());
                ModifiedManagedObjectSchemaDifference *pDiff = schemaInfoRepository.getModifiedManagedObjectSchemaDifference (managedClassIterator->second);

                modifiedManagedObjects[pMOSchemaDifference->getManagedObjectName()] = pDiff;
            }

            // Query the managed object to find out if instances of the MO are there in the database
            bool instancesExist = false;
            //status = querySynchronouslyForCount (pMOSchemaDifference->getManagedObjectName (), numberOfInstances);

            bool transactionStatus = dbStandaloneTransaction.checkIfTableHasEntriesInDb (pMOSchemaDifference->getManagedObjectName (), OrmRepository::getWaveCurrentSchema (), instancesExist);

            if (true == transactionStatus)
            {
                if (false == instancesExist)
                {
                    tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "WaveObjectManager::upgradeDefaultValueStep : No MO Instance found for %s\n",pMOSchemaDifference->getManagedObjectName ().c_str());
                }
                else
                {
                    pMOSchemaDifference->setIsManagedObjectInstancePresentInDb (true);

                    if (pMOSchemaDifference->getNumberOfAddedFields () > 0)
                    {
                        string sqlForAddedFields, sqlString;

                        pMOSchemaDifference->getSqlForAddedFields (sqlForAddedFields);
                        sqlForAddedFields.replace (0,1,"");

                        sqlString += pMOSchemaDifference->getManagedObjectName () + " SET ";
                        sqlString += sqlForAddedFields;
                        sqlString += ";";

                        defaultValueSql += "UPDATE WaveCurrent." + sqlString;
                        defaultValueSql += "UPDATE WaveStart." + sqlString;
                    }
                }
            }
        }
        else if (schemaInfoRepository.isTableRemovedFromSchema (managedClassIterator->second))
        {
            trace (TRACE_LEVEL_INFO, "WaveObjectManager::upgradeDefaultValueStep: deleted MO in new firmware : " + managedClassIterator->second);
            removedManagedObjects.push_back (managedClassIterator->second);
        }
    }

    if (defaultValueSql != "")
    {
        /*
        PersistenceObjectManagerExecuteTransactionMessage message (defaultValueSql);

        status = sendSynchronously (&message);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::upgradeDefaultValueStep: Could not send message to Persistence Service.  Status : " + FrameworkToolKit::localize (status));
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "WaveObjectManager::upgradeDefaultValueStep : Failed to execute the transaction.  Completion Status : " + FrameworkToolKit::localize (status));
                status = WAVE_MESSAGE_ERROR;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "WaveObjectManager::upgradeDefaultValueStep : Successfully set the default values ");
            }
        }
        */

        if (!dbStandaloneTransaction.execute (defaultValueSql))
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::upgradeDefaultValueStep : Error in applying SQL to database");
            status = FRAMEWORK_STATUS_CONVERSION_FAILURE;
        }

    }

    if (false == FrameworkToolKit::isSchemaConversionDisabledByUser())
    {
        OMSpecificSchemaChangeInfoForUpgrade* pSchemaChangeInfo = new OMSpecificSchemaChangeInfoForUpgrade ();

        if (NULL == pSchemaChangeInfo)
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::upgradeDefaultValueStep: failed to allocate memory for OMSpecificSchemaChangeInfoForUpgrade.");
            prismAssert (false, __FILE__, __LINE__);
        }
        pSchemaChangeInfo->setObjectManagerName         (m_name);
        pSchemaChangeInfo->setAddedManagedObjectNames   (newManagedObjects);
        pSchemaChangeInfo->setRemovedManagedObjectNames (removedManagedObjects);
        pSchemaChangeInfo->setModifiedManagedObjectInfo (modifiedManagedObjects);

        pPrismLinearSequencerContextForUpgradePhase->setSchemaChangeInfo (pSchemaChangeInfo);
    }


    pPrismLinearSequencerContextForUpgradePhase->executeNextStep (status);
}

void WaveObjectManager::checkIfUpgradeIsNecessary (const map<string, ModifiedManagedObjectSchemaDifference*>& modifiedManagedObjects)
{
    trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::checkIfUpgradeIsNecessary: entered..");

    if (0 == modifiedManagedObjects.size())
    {
       return;
    }

    map<string, ModifiedManagedObjectSchemaDifference*>::const_iterator iter = modifiedManagedObjects.begin();

    FrameworkToolKit::initializeConsolePrintFunction ();

    for (;iter!=modifiedManagedObjects.end(); iter++)
    {
        trace (TRACE_LEVEL_INFO, "WaveObjectManager::checkIfUpgradeIsNecessary: modified MO name \n" + iter->first);

        ModifiedManagedObjectSchemaDifference* pDiff = iter->second;

        vector<RelationshipInfoFromSchemaDifference>  addedRelations = pDiff->getAddedRelations ();
        vector<RelationshipInfoFromSchemaDifference>  changedRelations = pDiff->getChangedRelations ();

        if (0 != changedRelations.size())
        {
            FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------");
            tracePrintf (TRACE_LEVEL_WARN, true, true, "ObjectManager : [%s] needs to implement upgrade function mandatorily atleast for changedRelations:", m_name.c_str());

            FrameworkToolKit::consolePrint ("Modified MO name : ");
            FrameworkToolKit::consolePrint (iter->first);
            FrameworkToolKit::consolePrint ("ObjectManager Name : ");
            FrameworkToolKit::consolePrint (m_name);
            FrameworkToolKit::consolePrint ("needs to implement upgrade function mandatorily atleast for changed Relations fields:");

            for (vector<RelationshipInfoFromSchemaDifference>::const_iterator it = changedRelations.begin(); it != changedRelations.end(); it++)
            {
                tracePrintf (TRACE_LEVEL_WARN, true, true, "\t%s",it->getRelationName().c_str());

                FrameworkToolKit::consolePrint (it->getRelationName());
            }
            FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------");
        }

        if (0 != addedRelations.size())
        {
            for (vector<RelationshipInfoFromSchemaDifference>::const_iterator it = addedRelations.begin(); it != addedRelations.end(); it++)
            {
                if (ORM_RELATION_UML_TYPE_COMPOSITION == it->getRelationUmlType () &&
                    ORM_RELATION_TYPE_ONE_TO_ONE == it->getRelationType () &&
                    false == it->getCanBeEmpty ())
                {

                    FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------");
                    tracePrintf (TRACE_LEVEL_WARN, true, true, "ObjectManager : [%s] needs to implement upgrade function mandatorily atleast for added 1-1 Compositions with canBeEmpty set to false:", m_name.c_str());

                    FrameworkToolKit::consolePrint ("Modified MO name : ");
                    FrameworkToolKit::consolePrint (iter->first);
                    FrameworkToolKit::consolePrint ("ObjectManager Name : ");
                    FrameworkToolKit::consolePrint (m_name);
                    FrameworkToolKit::consolePrint ("needs to implement upgrade function mandatorily atleast for added 1-1 Composition fields with canBeEmpty flag set to false:");

                    tracePrintf (TRACE_LEVEL_WARN, true, true, "\t%s",it->getRelationName().c_str());

                    FrameworkToolKit::consolePrint (it->getRelationName());
                    FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------");
                }
            }
        }

    }
}

void WaveObjectManager::upgradeUpgradeWorkersStep (PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::upgradeUpgradeWorkersStep : Entering ...");

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContextForUpgradePhase);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase = new WaveAsynchronousContextForUpgradePhase (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::upgradeUpgradeWorkersStepCallback), pPrismLinearSequencerContextForUpgradePhase);

/*
         pWaveAsynchronousContextForUpgradePhase->setChangedManagedObjects (pPrismLinearSequencerContextForUpgradePhase->getChangedManagedObjects ());
         pWaveAsynchronousContextForUpgradePhase->setNewManagedObjects (pPrismLinearSequencerContextForUpgradePhase->getNewManagedObjects ());
*/
        pWaveAsynchronousContextForUpgradePhase->setSchemaChangeInfo (pPrismLinearSequencerContextForUpgradePhase->getSchemaChangeInfo());

        ++(*pPrismLinearSequencerContextForUpgradePhase);
        m_workers[i]->upgrade (pWaveAsynchronousContextForUpgradePhase);
    }

    --(*pPrismLinearSequencerContextForUpgradePhase);
    pPrismLinearSequencerContextForUpgradePhase->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::upgradeUpgradeWorkersStepCallback (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::upgradeUpgradeWorkersStepCallback : Entering ...");

    PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase   = reinterpret_cast<PrismLinearSequencerContextForUpgradePhase *> (pWaveAsynchronousContextForUpgradePhase->getPCallerContext ());
    ResourceId                 status                           = pWaveAsynchronousContextForUpgradePhase->getCompletionStatus ();

    --(*pPrismLinearSequencerContextForUpgradePhase);

    delete pWaveAsynchronousContextForUpgradePhase;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::upgradeUpgradeWorkersStepCallback : Enabling one worker failed.");
    }

    pPrismLinearSequencerContextForUpgradePhase->executeNextStep (status);
}

void WaveObjectManager::upgradeUpgradeSelfStep (PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::upgradeUpgradeSelfStep : Entering ...");

    WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase = new WaveAsynchronousContextForUpgradePhase (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::upgradeUpgradeSelfStepCallback), pPrismLinearSequencerContextForUpgradePhase);

/*
    pWaveAsynchronousContextForUpgradePhase->setChangedManagedObjects (pPrismLinearSequencerContextForUpgradePhase->getChangedManagedObjects ());
    pWaveAsynchronousContextForUpgradePhase->setNewManagedObjects (pPrismLinearSequencerContextForUpgradePhase->getNewManagedObjects ());
*/
    pWaveAsynchronousContextForUpgradePhase->setSchemaChangeInfo (pPrismLinearSequencerContextForUpgradePhase->getSchemaChangeInfo());

    upgrade (pWaveAsynchronousContextForUpgradePhase);
}

void WaveObjectManager::upgradeUpgradeSelfStepCallback (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::upgradeUpgradeSelfStepCallback : Entering ...");

    PrismLinearSequencerContextForUpgradePhase *pPrismLinearSequencerContextForUpgradePhase   = reinterpret_cast<PrismLinearSequencerContextForUpgradePhase *> (pWaveAsynchronousContextForUpgradePhase->getPCallerContext ());
    ResourceId                 status                         = pWaveAsynchronousContextForUpgradePhase->getCompletionStatus ();

    // Indicate that now we are ready to accept Messages to provide services.

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::upgradeUpgradeWorkersStepCallback : Upgrading one service failed.");

    }

    delete pWaveAsynchronousContextForUpgradePhase;
    pPrismLinearSequencerContextForUpgradePhase->executeNextStep (status);
}

void WaveObjectManager::upgrade (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::upgrade : Entering ...");
#if 0
    if (true == pWaveAsynchronousContextForUpgradePhase->checkFromVersionString ("2.1.0"))
    {
        vector<string> changedMO = pWaveAsynchronousContextForUpgradePhase->getChangedManagedObjects();

    }
#endif
    OMSpecificSchemaChangeInfoForUpgrade* pSchemaChangeInfo = pWaveAsynchronousContextForUpgradePhase->getSchemaChangeInfo ();
    checkIfUpgradeIsNecessary (pSchemaChangeInfo->getModifiedManagedObjectInfo ());
    pWaveAsynchronousContextForUpgradePhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForUpgradePhase->callback ();
}

void WaveObjectManager::bootHandler (PrismBootObjectManagerMessage *pBootMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::bootBootWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::bootBootSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pBootMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::bootBootWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::bootBootWorkersStep : Entering ...");

    PrismBootObjectManagerMessage *pPrismBootObjectManagerMessage = reinterpret_cast<PrismBootObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::bootBootWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForBootPhases->setBootReason (pPrismBootObjectManagerMessage->getReason ());
        pWaveAsynchronousContextForBootPhases->setRollBackFlag (pPrismBootObjectManagerMessage->getRollBackFlag ());

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->boot (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::bootBootWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::bootBootWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::bootBootWorkersStepCallback : Booting one worker failed.");
        //prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::bootBootSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::bootBootSelfStep : Entering ...");

//    checkMessageAttributesInSerialization ();

    PrismBootObjectManagerMessage *pPrismBootObjectManagerMessage = reinterpret_cast<PrismBootObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::bootBootSelfStepCallback), pPrismLinearSequencerContext);

    pWaveAsynchronousContextForBootPhases->setBootReason (pPrismBootObjectManagerMessage->getReason ());
    pWaveAsynchronousContextForBootPhases->setRollBackFlag (pPrismBootObjectManagerMessage->getRollBackFlag ());

    boot (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::bootBootSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::bootBootSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;

    // If we are a user specific task OM, then We must not be registered with Framework any more events since we cannot receive the events any way after this point.

    if (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (getServiceId ())))
    {
        // However some special OMs would like to listen to the BOOT Completion event and then unlisten for themselves.  If we unlisten at the boot phase itself,
        // they will never get a chance to receive the boot completion event.

        if (true == (getAllowAutomaticallyUnlistenForEvents ()))
        {
            unlistenEvents ();
        }
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::boot : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::hainstallHandler (PrismHaInstallObjectManagerMessage *pHaInstallMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::hainstallInstallWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::hainstallInstallSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pHaInstallMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}


void WaveObjectManager::hainstallInstallWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::hainstallInstallWorkersStep : Entering ...");

    PrismHaInstallObjectManagerMessage *pPrismHaInstallObjectManagerMessage = reinterpret_cast<PrismHaInstallObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32                              numberOfWorkers                   = m_workers.size ();
    UI32                              i                                 = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::hainstallInstallWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForBootPhases->setBootReason (pPrismHaInstallObjectManagerMessage->getReason ());

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->hainstall (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::hainstallInstallWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::hainstallInstallWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                         = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::hainstallInstallWorkersStepCallback : Installing one worker failed.");
        //prismAssert (false, __FILE__, __LINE__);
        // FIXME : sagar : handle Worker installation failures here.
        //                 Typically there is nothing we can do except simply proceeding.
        //                 May be, we can tun off the services provided by the worker.
        //                 But we must not return here.  The execution of next step on the PrismLinearSequencerContext
        //                 is crucial.
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::hainstallInstallSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::hainstallInstallSelfStep : Entering ...");

    PrismHaInstallObjectManagerMessage     *pPrismHaInstallObjectManagerMessage     = reinterpret_cast<PrismHaInstallObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::hainstallInstallSelfStepCallback), pPrismLinearSequencerContext);

    pWaveAsynchronousContextForBootPhases->setBootReason (pPrismHaInstallObjectManagerMessage->getReason ());

    hainstall (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::hainstallInstallSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::hainstallInstallSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                         = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::hainstall (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::hainstall : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::habootHandler (PrismHaBootObjectManagerMessage *pHaBootMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::habootBootWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::habootBootSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pHaBootMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::habootBootWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::habootBootWorkersStep : Entering ...");

    PrismHaBootObjectManagerMessage *pPrismHaBootObjectManagerMessage = reinterpret_cast<PrismHaBootObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::habootBootWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForBootPhases->setBootReason (pPrismHaBootObjectManagerMessage->getReason ());

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->boot (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::habootBootWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::habootBootWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::habootBootWorkersStepCallback : Booting one worker failed.");
        //prismAssert (false, __FILE__, __LINE__);
        // FIXME : sagar : handle Worker boot failures here.
        //                 Typically there is nothing we can do except simply proceeding.
        //                 May be, we can tun off the services provided by the worker.
        //                 But we must not return here.  The execution of next step on the PrismLinearSequencerContext
        //                 is crucial.
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::habootBootSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::habootBootSelfStep : Entering ...");

    PrismHaBootObjectManagerMessage *pPrismHaBootObjectManagerMessage = reinterpret_cast<PrismHaBootObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::habootBootSelfStepCallback), pPrismLinearSequencerContext);

    pWaveAsynchronousContextForBootPhases->setBootReason (pPrismHaBootObjectManagerMessage->getReason ());

    haboot (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::habootBootSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::habootBootSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;

    // If we are a user specific task OM, then We must not be registered with Framework any more events since we cannot receive the events any way after this point.

    if (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (getServiceId ())))
    {
        // However some special OMs would like to listen to the BOOT Completion event and then unlisten for themselves.  If we unlisten at the boot phase itself,
        // they will never get a chance to receive the boot completion event.

        if (true == (getAllowAutomaticallyUnlistenForEvents ()))
        {
            unlistenEvents ();
        }
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haboot : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

ResourceId WaveObjectManager::ValidateAllOwnedManagedClassNames( const vector<string> &managedObjectNames )
{
    // Check for all the MO names for which this service is the owner
    ResourceId status =  WAVE_MESSAGE_SUCCESS ;

    if ( true == managedObjectNames.empty() )
    {
        trace (TRACE_LEVEL_ERROR, string("WaveObjectManager::ValidateAllOwnedManagedClassNames : Input set is empty for Service : ") + this->getName());
        return (WAVE_MESSAGE_ERROR);
    }

    for ( UI32 i = 0; i < managedObjectNames.size(); i++ )
    {
        WaveObjectManager * pWaveObjectManager = NULL ;
        pWaveObjectManager = getOwnerForManagedClass( managedObjectNames[i] );
        prismAssert(NULL != pWaveObjectManager,__FILE__, __LINE__);

        if ( ( this != pWaveObjectManager ) && ( m_pAssociatedVirtualWaveObjectManager != pWaveObjectManager ) )
        {
            // Neither this service nor the associated Global service is the owner
            trace (TRACE_LEVEL_ERROR, string("WaveObjectManager::ValidateAllOwnedManagedClassNames : Service : ") + this->getName() + " or its associated Global service does not own MO : " + managedObjectNames[i].c_str());
            return (WAVE_MESSAGE_ERROR);
        }
    }
    return (status);
}

void WaveObjectManager::populatePostbootMap()
{
    // override this function to populate the map
    return;
}

void  WaveObjectManager::setPostbootMap(const map<string, vector<string> > &postbootManagedObjectNames)
{
    m_postbootManagedObjectNames = postbootManagedObjectNames ;
}

void  WaveObjectManager::getPostbootMap(map<string, vector<string> > &postbootManagedObjectNames) const
{
    postbootManagedObjectNames = m_postbootManagedObjectNames;
}

void WaveObjectManager::getManageObjectNamesFromPostbootMap ( const string passName, vector<string> &managedObjectNamesInPass )
{
    //get GlobalManagedObject names from Global service for this pass
    if ( NULL != m_pAssociatedVirtualWaveObjectManager )
    {
        m_pAssociatedVirtualWaveObjectManager->getManageObjectNamesFromPostbootMap( passName, managedObjectNamesInPass );
    }

    map<string, vector<string> >::iterator elementManagedObjectMap = m_postbootManagedObjectNames.find(passName);

    if ( true == m_postbootManagedObjectNames.empty() )
    {
        trace (TRACE_LEVEL_INFO, string("WaveObjectManager::getManageObjectNamesFromPostbootMap : Postboot Map is empty for all pass, pass : ") + passName.c_str() + (" service :") + this->getName() );
    }
    else if ( m_postbootManagedObjectNames.end() != elementManagedObjectMap )
    {
        // Push LocalManagedObject names to the end
        managedObjectNamesInPass.insert( managedObjectNamesInPass.end(), (elementManagedObjectMap->second).begin(), (elementManagedObjectMap->second).end());
    }
}

ResourceId WaveObjectManager::postbootValidateAllOwnedManagedClassNames (const string passName, vector<string> &managedObjectNamesInPass)
{
    ResourceId status =  WAVE_MESSAGE_SUCCESS ;

    //map<passName, set<MONames> >
    getManageObjectNamesFromPostbootMap( passName, managedObjectNamesInPass );

    if ( true == managedObjectNamesInPass.empty() )
    {
        trace (TRACE_LEVEL_ERROR, string("WaveObjectManager::postbootValidateAllOwnedManagedClassNames : Postboot Map for service : ") + this->getName() + (" has no entry for Pass : " ) + passName);
        status =  WAVE_MESSAGE_ERROR ;
    }

    if ( WAVE_MESSAGE_SUCCESS == status )
    {
        //it only validates the MO names specified in xml
        //if the MO is not owned by this service, it will return error
        //if validation fails expectation is not to proceed
        status  = ValidateAllOwnedManagedClassNames( managedObjectNamesInPass );
    }

    return (status);
}

void WaveObjectManager::postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase)
{
    ResourceId status =  WAVE_MESSAGE_SUCCESS ;

    trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::postboot : Entering with pass#:") + pWaveAsynchronousContextForPostbootPhase->getPassNum());

    vector<string> managedObjectNamesInPass;

    //calling virtual function for postboot validation
    status = postbootValidate (pWaveAsynchronousContextForPostbootPhase);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        status = postbootValidateAllOwnedManagedClassNames( pWaveAsynchronousContextForPostbootPhase->getPassName(), managedObjectNamesInPass);

        if ( WAVE_MESSAGE_SUCCESS == status )
        {
            // For each MO we need to call the respective function which will populate the backEndMap

            for ( UI32 j = 0; j < managedObjectNamesInPass.size(); j++ )
            {
                bool isLocalManagedObject = OrmRepository::isALocalManagedObject ( managedObjectNamesInPass[j] );

                WaveManagedObjectSynchronousQueryContext synchronousQueryContext   ( managedObjectNamesInPass[j] );

                if ( true == isLocalManagedObject )
                {
                    trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::postboot : Postboot for localMO "));
                    synchronousQueryContext.addAndAttribute (new AttributeObjectId (FrameworkToolKit::getThisWaveNodeObjectId(), "ownerWaveNodeObjectId"));
                }

                //Get the MO object to call its respective function

                vector<WaveManagedObject *> *pWaveManagedObjectsInPass = querySynchronously ( &synchronousQueryContext );

                prismAssert (NULL != pWaveManagedObjectsInPass, __FILE__, __LINE__);

                if ( 0 == pWaveManagedObjectsInPass->size())
                {
                    trace(TRACE_LEVEL_DEVEL, string("WaveObjectManager::postboot : ManagedObject -> " + (managedObjectNamesInPass[j]) + " not found in DB, passName: " + pWaveAsynchronousContextForPostbootPhase->getPassName()));
                    delete pWaveManagedObjectsInPass;
                    continue;
                }

                trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::postboot : service name is ") + this->getName() + string(" MO Name is ") + managedObjectNamesInPass[j].c_str());

                GetHardwareConfigurationDetailsForPostbootContext *pGetHardwareConfigurationDetailsForPostbootContext = new GetHardwareConfigurationDetailsForPostbootContext ((managedObjectNamesInPass[j] ), pWaveAsynchronousContextForPostbootPhase->getPassName(), pWaveManagedObjectsInPass, this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::postbootCallback), pWaveAsynchronousContextForPostbootPhase);

                pGetHardwareConfigurationDetailsForPostbootContext->setCompletionStatus ( WAVE_MESSAGE_SUCCESS );

                ((*pWaveManagedObjectsInPass)[0])->getHardwareConfigurationDetailsForPostboot( pGetHardwareConfigurationDetailsForPostbootContext );

                status = pGetHardwareConfigurationDetailsForPostbootContext->getCompletionStatus();

                // Do not access any pointers beyond this
                delete pGetHardwareConfigurationDetailsForPostbootContext;
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector( pWaveManagedObjectsInPass );

                if ( WAVE_MESSAGE_SUCCESS != status )
                {
                    trace (TRACE_LEVEL_FATAL,string("WaveObjectManager::postboot : Since the Completion status for ManagedObject ") + (managedObjectNamesInPass[j]) + (" is not success, it will not continue ") );
                    break;
                }

            } //for loop based on MO name

        } // Validation is success

        else
        {
            trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::postboot : Validation Failed for service ") + this->getName());

            //treating as success because of JMP being part of postboot
            status = WAVE_MESSAGE_SUCCESS ;
        }
    }

    else if (status == WAVE_MESSAGE_SKIP_POSTBOOT)
    {
        //skipping postboot for this service
        status = WAVE_MESSAGE_SUCCESS;
    }
       trace (TRACE_LEVEL_INFO, string ("WaveObjectManager::postboot : returned ") + status) ;
    pWaveAsynchronousContextForPostbootPhase->setCompletionStatus (status);
    pWaveAsynchronousContextForPostbootPhase->callback ();
}

void WaveObjectManager::postbootCallback(GetHardwareConfigurationDetailsForPostbootContext *pGetHardwareConfigurationDetailsForPostbootContext)
{
    string managedObjectName = (pGetHardwareConfigurationDetailsForPostbootContext->getManagedObjectName());

    if ( WAVE_MESSAGE_SUCCESS != pGetHardwareConfigurationDetailsForPostbootContext->getCompletionStatus() )
    {
        trace (TRACE_LEVEL_ERROR, string("WaveObjectManager::postbootCallBack : Completion status for ManagedObject ") + managedObjectName.c_str() + (" is not success from the calling-function"));
        return ;
    }

    if ( true == pGetHardwareConfigurationDetailsForPostbootContext->getIsUpdateBackend () )
    {
        ResourceId status =  WAVE_MESSAGE_SUCCESS ;
        // If any backend update is required then continue
        // Each MO function decides and sets the backEnd update flag appropriately

        //map <UI32, string>                    groupCode and BackendNamesMap;
        //map <UI32, vector <string> >          groupCode and AttributeNamesMap;
        //map<UI32, bool>                       groupCode and typeFlag, it will not have case-groups
        //map<UI32, map<UI32, vector<UI32> > >  choice-group and its corresponding Usertags,Case-Groups;
        // pass the context since it already has the map
        status = sendSynchronouslyForBackEndAttributesMap( pGetHardwareConfigurationDetailsForPostbootContext );

        if ( WAVE_MESSAGE_SUCCESS != status )
        {
            trace (TRACE_LEVEL_ERROR,string("WaveObjectManager::postbootCallBack : sendSynchronously For BackEndAttributesMap failed for ManagedObject ") + managedObjectName.c_str());
        }

        pGetHardwareConfigurationDetailsForPostbootContext->setCompletionStatus (status);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string("WaveObjectManager::postbootCallBack : UpdateBackend is false for ManagedObject" ) + managedObjectName.c_str());
    }
    return ;
}

bool WaveObjectManager::checkForDuplicationInBackEndAttributesMap ( GetHardwareConfigurationDetailsForPostbootContext * pGetHardwareConfigurationDetailsForPostbootContext  )
{
    map <UI32, vector <string> > groupCodeAttributes = pGetHardwareConfigurationDetailsForPostbootContext->getGroupCodeAttributeGroupMap();
    map <UI32, vector <string> >::iterator elementgroupCodeAttributes = groupCodeAttributes.begin();
    map <UI32, vector <string> >::iterator endgroupCodeAttributes     = groupCodeAttributes.end();

    set<string> attributeNames;

    while ( endgroupCodeAttributes != elementgroupCodeAttributes )
    {
        vector<string> attributeNamesInGroup = elementgroupCodeAttributes->second;

        for ( UI32 i = 0; i < attributeNamesInGroup.size(); i++ )
        {
            if ( 0 != attributeNames.count( attributeNamesInGroup[i] ) )
            {
                trace(TRACE_LEVEL_ERROR, string("WaveObjectManager::checkForDuplicationInBackEndAttributesMap : Duplicate attribute is ") + (attributeNamesInGroup[i]));
                return (true) ;
            }
            else
            {
                attributeNames.insert( attributeNamesInGroup[i] );
            }
        }
        elementgroupCodeAttributes++;
    }

    return (false);
}

ResourceId WaveObjectManager::sendSynchronouslyForBackEndAttributesMap( GetHardwareConfigurationDetailsForPostbootContext * pGetHardwareConfigurationDetailsForPostbootContext )
{
    map <UI32, bool> groupCodeChoiceFlag;
    pGetHardwareConfigurationDetailsForPostbootContext->getGroupCodeChoiceFlag( groupCodeChoiceFlag );

    string managedObjectName = pGetHardwareConfigurationDetailsForPostbootContext->getManagedObjectName();
    ResourceId status =  WAVE_MESSAGE_SUCCESS ;

    if ( true == checkForDuplicationInBackEndAttributesMap( pGetHardwareConfigurationDetailsForPostbootContext ) )
    {
        trace(TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : Duplicate attribute is present for MO ") + managedObjectName);
        prismAssert (false, __FILE__, __LINE__);
    }

    if ( false == groupCodeChoiceFlag.empty() )
    {
        vector<WaveManagedObject *>* pWaveManagedObjectsInPass = NULL;

        pWaveManagedObjectsInPass = pGetHardwareConfigurationDetailsForPostbootContext->getWaveManagedObjects( );
        prismAssert (NULL != pWaveManagedObjectsInPass, __FILE__, __LINE__);

        if ( 0 == pWaveManagedObjectsInPass->size())
        {
            // Since the MO in DB is empty there is nothing to be sent
            // Should we treat this as error

            tracePrintf (TRACE_LEVEL_ERROR, "WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : MO -> %s not found in DB", (managedObjectName).c_str());
            return (status) ;
        }

        vector<string> managedObjectKeysName;

        OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (managedObjectName, managedObjectKeysName);

        // All the keys name for this MO
        set<string> keyAttributeNames( managedObjectKeysName.begin(),managedObjectKeysName.end() );
        pGetHardwareConfigurationDetailsForPostbootContext->setKeyAttributeNames (keyAttributeNames);
        managedObjectKeysName.clear();

        vector<WaveManagedObject *>::iterator managedObjectIterator;

        // Since we may be in a recursive call doing postboot for composite child, save the parent keys
        vector<Attribute *> prefixAttributes = pGetHardwareConfigurationDetailsForPostbootContext->getAttributes();

        // For each row of the MO
        for ( managedObjectIterator = pWaveManagedObjectsInPass->begin (); managedObjectIterator != pWaveManagedObjectsInPass->end(); managedObjectIterator++ )
        {
            WaveManagedObject* pWaveManagedObject = (*managedObjectIterator);
            pGetHardwareConfigurationDetailsForPostbootContext->setPOperateOnWaveManagedObject( pWaveManagedObject );

            vector<Attribute *> keyAttributes;

            //First populate all keys
            set<string>::iterator elementKeyAttributeNames = keyAttributeNames.begin();
            set<string>::iterator endKeyAttributeNames     = keyAttributeNames.end();

            while ( endKeyAttributeNames != elementKeyAttributeNames )
            {
                Attribute* pAttr = pWaveManagedObject->getAttributeByName ( *elementKeyAttributeNames );

                if ( NULL == pAttr  )
                {
                    trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : MO " + managedObjectName + " does not have attribute " + (*elementKeyAttributeNames).c_str()));
                    prismAssert (false, __FILE__, __LINE__);
                }

                AttributeType attrType = pAttr->getAttributeType ();

                //Clone to get new Attribute with same values
                Attribute* pClone = pAttr->clone();
                prismAssert (pClone != NULL, __FILE__, __LINE__);

                if ( (AttributeType::AttributeTypeObjectId == attrType ) || ( AttributeType::AttributeTypeObjectIdVector == attrType ) )
                {
                    Attribute* pAssociatedAttribute = NULL ;

                    trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : MO " + managedObjectName + " has associated key " + pClone->getAttributeName()));
                    getAssociatedAttributeClone( managedObjectName, pClone, pAssociatedAttribute );

                    if ( NULL != pAssociatedAttribute)
                    {
                        // replace the key name for associated attribute
                        pAssociatedAttribute->setAttributeName( pClone->getAttributeName() );
                        keyAttributes.push_back( pAssociatedAttribute );
                    }
                    delete pClone;
                    pClone = NULL;

                }
                else if ( ( AttributeType::AttributeTypeComposition == attrType ) || ( AttributeType::AttributeTypeCompositionVector == attrType ) )
                {
                    trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : MO " + managedObjectName + " has composite key " + pClone->getAttributeName()));
                    delete pClone;
                    pClone = NULL;

                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    bool isAdded = false;

                    if ( false == pClone->isCurrentValueSameAsDefault()  )
                    {
                        //Send only if the value is not same as default
                        // Its a simple attribute
                        AttributeUC * pAttributeUC = dynamic_cast<AttributeUC *> (pClone);

                        if ( NULL == pAttributeUC )
                        {
                            keyAttributes.push_back( pClone );
                            isAdded = true;
                        }
                        else
                        {
                            // UC type attribute, send only if user has configured the value for it
                            if ( true == pAttributeUC->getIsUserConfigured() )
                            {
                                trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : user has configured attribute ") + pClone->getAttributeName () );
                                keyAttributes.push_back( pClone );
                                isAdded = true;
                            }
                        }
                    }

                    if ( false == isAdded )
                    {
                        // we did not add the attribute, so delete it here
                        delete (pClone);
                    }
                }
                elementKeyAttributeNames++;
            }// for each key attribute

            map<UI32, bool>::iterator elementGroupCodeChoiceFlag = groupCodeChoiceFlag.begin();
            map<UI32, bool>::iterator endGroupCodeChoiceFlag = groupCodeChoiceFlag.end();

            // For each groupId
            while ( endGroupCodeChoiceFlag != elementGroupCodeChoiceFlag )
            {
                pGetHardwareConfigurationDetailsForPostbootContext->setGroupCode ( elementGroupCodeChoiceFlag->first );

                if ( false == prefixAttributes.empty() )
                {
                    // Merge prefix and keys. Set merged list in the context
                    vector<Attribute *> temp = prefixAttributes;
                    temp.insert( temp.end(), keyAttributes.begin(), keyAttributes.end() );

                    pGetHardwareConfigurationDetailsForPostbootContext->setAttributeVector( temp );
                    temp.clear();
                }
                else
                {
                    pGetHardwareConfigurationDetailsForPostbootContext->setAttributeVector( keyAttributes );
                }

                if ( true == elementGroupCodeChoiceFlag->second )
                {
                    postbootForChoiceGroup(pGetHardwareConfigurationDetailsForPostbootContext);

                    status = pGetHardwareConfigurationDetailsForPostbootContext->getCompletionStatus();

                    if( WAVE_MESSAGE_SUCCESS != status )
                    {
                        trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : postboot of MO " + managedObjectName + " failed for choice-group - ") +  elementGroupCodeChoiceFlag->first );
                        return (status) ;
                    }
                }
                else
                {
                    string clientName = pGetHardwareConfigurationDetailsForPostbootContext->getClientNameForGroup ( elementGroupCodeChoiceFlag->first ) ;
                    prismAssert (false == clientName.empty(), __FILE__, __LINE__);

                    pGetHardwareConfigurationDetailsForPostbootContext->setClientName( clientName);
                    trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : MO " + managedObjectName + ", groupcode is ") + pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode());
                    status = sendSynchronouslyForAGroup( pGetHardwareConfigurationDetailsForPostbootContext );

                    if( WAVE_MESSAGE_SUCCESS != status )
                    {
                        trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : postboot of MO " + managedObjectName + " failed for Normal-group - ") +  elementGroupCodeChoiceFlag->first );
                        return (status) ;
                    }
                }

                elementGroupCodeChoiceFlag++;
            }// For each group

            // Reset the row pointer to NULL
            pGetHardwareConfigurationDetailsForPostbootContext->setPOperateOnWaveManagedObject(NULL);

            // Release key values since we need to read it from the next row of the MO
            WaveManagedObjectToolKit::releaseMemoryOfAttributeVector( keyAttributes );

        }// For each MO row

    }// Group Vector is not empty
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, "WaveObjectManager::sendSynchronouslyForBackEndAttributesMap : GroupCodes Vector is empty for MO -> %s ", managedObjectName.c_str());
        //status = WAVE_MESSAGE_ERROR ;
    }

    return (status) ;

}

void WaveObjectManager::postbootForChoiceGroup ( GetHardwareConfigurationDetailsForPostbootContext * pGetHardwareConfigurationDetailsForPostbootContext )
{
    ResourceId status =  WAVE_MESSAGE_SUCCESS ;

    string managedObjectName = pGetHardwareConfigurationDetailsForPostbootContext->getManagedObjectName();

    UI32 groupCode = pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode();

    WaveManagedObject* pWaveManagedObject = NULL;
    pWaveManagedObject = pGetHardwareConfigurationDetailsForPostbootContext->getPOperateOnWaveManagedObject();
    prismAssert ( NULL != pWaveManagedObject, __FILE__, __LINE__);

    vector<string> choiceGroupAttribute = pGetHardwareConfigurationDetailsForPostbootContext->getAttributeNamesForGroup( groupCode );
    //Removing the below assert for now since the group currently has more than 1 attribute
    // We expect only the attribute which has the choice-usertag value
    //prismAssert ( 1 == choiceGroupAttribute.size(), __FILE__, __LINE__);

    vector<UI32> caseGroups;

    for ( UI32 j = 0; j < choiceGroupAttribute.size(); j++ )
    {
        Attribute* pAttr = pWaveManagedObject->getAttributeByName ( choiceGroupAttribute[j] );

        if ( NULL == pAttr  )
        {
            trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::postbootForChoiceGroup : MO " + managedObjectName + " does not have attribute " + choiceGroupAttribute[j].c_str()));
            prismAssert (false, __FILE__, __LINE__);
        }

        AttributeUI32 *pAttrUI32 = dynamic_cast<AttributeUI32 *>(pAttr);

        if ( NULL == pAttrUI32 )
        {
            trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::postbootForChoiceGroup : MO " + managedObjectName + ", does not have UI32 type userTag for Choice-attribute " + choiceGroupAttribute[j].c_str()));
        }
        else
        {
            // We got the first UI32 type attribute, its value must be userTag
            pGetHardwareConfigurationDetailsForPostbootContext->getCaseGroupsForChoiceGroup( groupCode, pAttrUI32->getValue(), caseGroups );
            break;
        }
    }


    if ( false == caseGroups.empty() )
    {
        // Since we may be in a recursive call, save the parent keys
        vector<Attribute *> prefixAttributes = pGetHardwareConfigurationDetailsForPostbootContext->getAttributes();

        for ( UI32 i = 0; i < caseGroups.size(); i++ )
        {
            pGetHardwareConfigurationDetailsForPostbootContext->setAttributeVector( prefixAttributes );

            pGetHardwareConfigurationDetailsForPostbootContext->setGroupCode ( caseGroups[i] );

            if ( false == pGetHardwareConfigurationDetailsForPostbootContext->isAChoiceGroup( caseGroups[i] ) )
            {
                string clientName = pGetHardwareConfigurationDetailsForPostbootContext->getClientNameForGroup ( caseGroups[i] ) ;
                prismAssert (false == clientName.empty(), __FILE__, __LINE__);

                pGetHardwareConfigurationDetailsForPostbootContext->setClientName( clientName);
                trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::postbootForChoiceGroup : MO " + managedObjectName + ", groupcode is ") + pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode());
                status = sendSynchronouslyForAGroup( pGetHardwareConfigurationDetailsForPostbootContext );

                if( WAVE_MESSAGE_SUCCESS != status )
                {
                    trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::postbootForChoiceGroup : Postboot failed for MO " + managedObjectName + ", case-groupcode - ") + pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode());
                    return ;
                }
            }
            else
            {
                postbootForChoiceGroup(pGetHardwareConfigurationDetailsForPostbootContext);

                status = pGetHardwareConfigurationDetailsForPostbootContext->getCompletionStatus();

                if( WAVE_MESSAGE_SUCCESS != status )
                {
                    trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::postbootForChoiceGroup : Postboot failed for MO " + managedObjectName + ", choiceGroup - ") + pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode() );
                    return ;
                }
            }
        }
    }
}

ResourceId WaveObjectManager::sendSynchronouslyForAGroup( GetHardwareConfigurationDetailsForPostbootContext * pGetHardwareConfigurationDetailsForPostbootContext )
{
    ResourceId status =  WAVE_MESSAGE_SUCCESS ;
    string managedObjectName = pGetHardwareConfigurationDetailsForPostbootContext->getManagedObjectName();
    trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::sendSynchronouslyForAGroup : MO " + managedObjectName + ", groupcode is ") + pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode());

    string clientName;
    pGetHardwareConfigurationDetailsForPostbootContext->getClientName( clientName);

    // This flag is used to avoid sending data from the parent MO for a group having composite attribute
    // For a composite attribute we do postboot for child.
    bool compositeChild = false;

    vector<string> groupAttributeNames = pGetHardwareConfigurationDetailsForPostbootContext->getAttributeNamesForGroup( pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode() );

    WaveManagedObject* pWaveManagedObject = NULL;
    pWaveManagedObject = pGetHardwareConfigurationDetailsForPostbootContext->getPOperateOnWaveManagedObject();

    prismAssert ( NULL != pWaveManagedObject, __FILE__, __LINE__);

    // This captures values for all the non-key attributes in a group
    vector<Attribute *> attributeList;

    // below flag is used to send specifically the group which has only keys specified in it
    // Groups where there are no attributes to be sent after filtering will not be sent
    bool keyGroup = false;

    //For each attribute
    for ( UI32 i = 0 ; i < groupAttributeNames.size(); i++ )
    {
        if ( true == compositeChild )
        {
            trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForAGroup : MO " + managedObjectName + ", Within a group You cannot include attributes following composite attribute. AttributeName is ") + groupAttributeNames[i].c_str() + (", Asserting ...") );
            prismAssert (false, __FILE__, __LINE__);
        }

        if ( true == pGetHardwareConfigurationDetailsForPostbootContext->isKeyAttribute( groupAttributeNames[i] ) )
        {
            // If its a key then we have already populated it
            trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::sendSynchronouslyForAGroup : MO " + managedObjectName + " already processed key attribute " + groupAttributeNames[i].c_str()));
            keyGroup = true;
            continue;
        }

        Attribute* pAttr = pWaveManagedObject->getAttributeByName ( groupAttributeNames[i] );

        if ( NULL == pAttr  )
        {
            trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForAGroup : MO " + managedObjectName + " does not have attribute " + groupAttributeNames[i].c_str()));
            prismAssert (false, __FILE__, __LINE__);
        }

        AttributeType attrType = pAttr->getAttributeType ();

        //Clone to get new Attribute with same values
        Attribute* pClone = pAttr->clone();
        prismAssert (pClone != NULL, __FILE__, __LINE__);

        if (  (AttributeType::AttributeTypeObjectId != attrType ) && ( AttributeType::AttributeTypeObjectIdVector != attrType )
               && ( AttributeType::AttributeTypeComposition != attrType ) && ( AttributeType::AttributeTypeCompositionVector != attrType ) )
        {
            bool isAdded = false;

            if ( false == pClone->isCurrentValueSameAsDefault() )
            {
                //Send only if the value is not same as default
                // Its a simple attribute
                AttributeUC * pAttributeUC = dynamic_cast<AttributeUC *> (pClone);

                if ( NULL == pAttributeUC )
                {
                    trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::sendSynchronouslyForAGroup : Not of type UC, dynamiccast is returning NULL for ") + pClone->getAttributeName () );
                    attributeList.push_back( pClone );
                    isAdded = true;
                }
                else
                {
                    // UC type attribute, send only if user has configured the value for it
                    if ( true == pAttributeUC->getIsUserConfigured() )
                    {
                        trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::sendSynchronouslyForAGroup : user has configured attribute ") + pClone->getAttributeName () );
                        attributeList.push_back( pClone );
                        isAdded = true;
                    }
                }
            }

            if ( false == isAdded )
            {
                delete (pClone);
            }
        }
        else
        {
            if ( (AttributeType::AttributeTypeObjectId == attrType ) || ( AttributeType::AttributeTypeObjectIdVector == attrType ) )
            {
                // Attribute has association relation
                Attribute*  pAssociatedAttribute = NULL ;
                getAssociatedAttributeClone( managedObjectName, pClone, pAssociatedAttribute );

                if ( NULL != pAssociatedAttribute)
                {
                    // replace the key name for associated attribute
                    pAssociatedAttribute->setAttributeName( pClone->getAttributeName() );
                    attributeList.push_back( pAssociatedAttribute );
                }
            }
            else
            {
                if ( false == attributeList.empty() )
                {
                    // append all the group attributes to the end of parent and its own keys
                    vector<Attribute *> temp = pGetHardwareConfigurationDetailsForPostbootContext->getAttributes();
                    temp.insert( temp.end(), attributeList.begin(), attributeList.end() );

                    pGetHardwareConfigurationDetailsForPostbootContext->setAttributeVector( temp );
                    temp.clear();
                }

                // do postboot for child
                postbootForCompositeChild( pClone, pGetHardwareConfigurationDetailsForPostbootContext);

                compositeChild = true ;

                // After postboot for compositechild check for status
                status = pGetHardwareConfigurationDetailsForPostbootContext->getCompletionStatus();
                if( WAVE_MESSAGE_SUCCESS != status )
                {
                    trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForAGroup : postboot for composite child of parent MO " + managedObjectName + " failed"));
                }
            }

            // release memory from clone only incase of association/composition
            delete pClone ;
        }

    }// for each attribute in a group

    // After populating all the attributes
    if ( (WAVE_MESSAGE_SUCCESS == status ) && ( true != compositeChild ))
    {
        // If there was no failure seen and this group does not have a composite attribute
        // then send attributes to backend

        {// Testing begin
            // This block is for debug use. Enable DEVEL log and monitor the attributes being sent
            vector< Attribute *> keys = pGetHardwareConfigurationDetailsForPostbootContext->getAttributes();

            vector< Attribute *>::iterator element = keys.begin();
            vector< Attribute *>::iterator end     = keys.end();
            while ( end != element )
            {
                Attribute * att = ( *element) ;
                string value;
                att->toString( value );
                trace (TRACE_LEVEL_DEVEL, string("Name : ") + att->getAttributeName () + (" Value : ") + value);
                element++;
            }

            element = attributeList.begin();
            end     = attributeList.end();
            while ( end != element )
            {
                Attribute * att = ( *element) ;
                string value;
                att->toString( value );
                trace (TRACE_LEVEL_DEVEL, string("Name : ") + att->getAttributeName () + (" Value : ") + value);
                element++;
            }

            // End of Test prints for debug
        }// Testing end

        if ( false == attributeList.empty() || true == keyGroup )
        {
            // allow only keyGroup or if attributes in the group are not empty after filtering
            vector<Attribute *> merged = pGetHardwareConfigurationDetailsForPostbootContext->getAttributes();
            merged.insert( merged.end(), attributeList.begin(), attributeList.end() );

            pGetHardwareConfigurationDetailsForPostbootContext->setAttributeVector( merged );
            merged.clear();

            UnifiedClientPostbootMessage *pMessage = new UnifiedClientPostbootMessage ( pGetHardwareConfigurationDetailsForPostbootContext );
            prismAssert(NULL != pMessage ,__FILE__, __LINE__);

            status = sendSynchronouslyToWaveClient( clientName, pMessage );

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForAGroup : Error while sending MO " + managedObjectName +  " to client " + clientName  + " with status : " + FrameworkToolKit::localize (status)));
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus ())
                {
                    status = pMessage->getCompletionStatus ();
                    trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::sendSynchronouslyForAGroup :Error while sending MO " + managedObjectName +  " to client " + clientName  + " with Completionstatus : " + FrameworkToolKit::localize (status)));
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, string("WaveObjectManager::sendSynchronouslyForAGroup : MO " + managedObjectName + " successfully sent to client " + clientName ));
                }
            }
            delete pMessage;

            WaveManagedObjectToolKit::releaseMemoryOfAttributeVector( attributeList );

            // Remove the deleted pointers from context by intializing it with empty vector
            vector<Attribute *> emptyVector;
            pGetHardwareConfigurationDetailsForPostbootContext->setAttributeVector( emptyVector );
        }
        else
        {
            if ( false == groupAttributeNames.empty() )
            {
                trace (TRACE_LEVEL_INFO, string("WaveObjectManager::sendSynchronouslyForAGroup : MO " + managedObjectName + " after filtering has no attributes to be sent in group - ") + pGetHardwareConfigurationDetailsForPostbootContext->getGroupCode());
            }
        }
    }

    return (status) ;
}

void WaveObjectManager::postbootForCompositeChild( Attribute *pAttribute, GetHardwareConfigurationDetailsForPostbootContext *pGetHardwareConfigurationDetailsForPostbootContext)
{
    AttributeType attrType = pAttribute->getAttributeType ();

    vector<WaveManagedObject *>  *pResults;

    ResourceId status =  WAVE_MESSAGE_SUCCESS ;

    if (( AttributeType::AttributeTypeComposition != attrType ) && ( AttributeType::AttributeTypeCompositionVector != attrType ))
    {
        trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::postbootForCompositeChild : Wrong attribute type"));
        prismAssert( false, __FILE__, __LINE__);
    }

    // get all pointers to composed MO
    pResults = pAttribute->getComposedManagedObject();
    prismAssert( NULL != pResults, __FILE__, __LINE__);

    if ( 0 == (*pResults).size() )
    {
        trace (TRACE_LEVEL_INFO, string("WaveObjectManager::postbootForCompositeChild : DB is empty for CompositeVector child of ParentMO  ") + (pGetHardwareConfigurationDetailsForPostbootContext->getManagedObjectName()) );
        return ;
    }
    else
    {
        // this means the size is atleast more than 1
        // for attributecomposition the size will be 1, and  we need to check if MO pointer is NULL
        if ( (AttributeType::AttributeTypeComposition == attrType) && (NULL == ((*pResults)[0])) )
        {
            trace (TRACE_LEVEL_INFO, string("WaveObjectManager::postbootForCompositeChild : DB is empty for simple Composite child of ParentMO  ") + (pGetHardwareConfigurationDetailsForPostbootContext->getManagedObjectName()) );
            return ;
        }
    }

    string managedObjectName = ((*pResults)[0])->getObjectClassName ();

    //create a new context for composite child
    GetHardwareConfigurationDetailsForPostbootContext *pNewGetHardwareConfigurationDetailsForPostbootContext = new GetHardwareConfigurationDetailsForPostbootContext( managedObjectName, pGetHardwareConfigurationDetailsForPostbootContext->getPassName(), pResults, this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::postbootCallback), pGetHardwareConfigurationDetailsForPostbootContext);

    vector<string> parentManagedobjectNames;
    vector<string>::iterator parentManagedobjectNamesIterator;

    // Get all parent MO names and the corresponding GroupCodes
    pGetHardwareConfigurationDetailsForPostbootContext->getParentManagedObjectNames( parentManagedobjectNames );

    parentManagedobjectNamesIterator = parentManagedobjectNames.begin();

    // Append the current parent MO name and its groupcode
    parentManagedobjectNamesIterator = parentManagedobjectNames.insert(parentManagedobjectNamesIterator, pGetHardwareConfigurationDetailsForPostbootContext->getManagedObjectName());

    // set parent MO names and the corresponding GroupCodes in the new context
    pNewGetHardwareConfigurationDetailsForPostbootContext->setParentManagedObjectNames( parentManagedobjectNames );

    // set all parent keys
    pNewGetHardwareConfigurationDetailsForPostbootContext->setAttributeVector( pGetHardwareConfigurationDetailsForPostbootContext->getAttributes());

    pNewGetHardwareConfigurationDetailsForPostbootContext->setCompletionStatus ( WAVE_MESSAGE_SUCCESS );

    // Invoke the breeze function
    ((*pResults)[0])->getHardwareConfigurationDetailsForPostboot(pNewGetHardwareConfigurationDetailsForPostbootContext);

    status = pNewGetHardwareConfigurationDetailsForPostbootContext->getCompletionStatus();

    // Do not access any pointers beyond this
    delete pNewGetHardwareConfigurationDetailsForPostbootContext;

    if ( WAVE_MESSAGE_SUCCESS != status )
    {
        trace (TRACE_LEVEL_FATAL,string("WaveObjectManager::postbootForCompositeChild : Since the Completion status for ManagedObject ") + (managedObjectName ) + (" is not success, we will not continue ") );
        pGetHardwareConfigurationDetailsForPostbootContext->setCompletionStatus( status );
    }

    return ;
}

void WaveObjectManager::getAssociatedAttributeClone( string parentClassName, Attribute *pAttribute, Attribute *&pAssociatedAttribute )
{
    string  associatedClassName ;
    vector <ObjectId> associatedObjectIds ;
    AttributeType attrType = pAttribute->getAttributeType ();

    vector<WaveManagedObject *>  *pResults;

    if (AttributeType::AttributeTypeObjectId == attrType )
    {
        AttributeObjectIdAssociation *pAttributeObjectIdAssociation = dynamic_cast<AttributeObjectIdAssociation *> (pAttribute);
        if ( NULL == pAttributeObjectIdAssociation  )
        {
            trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::getAssociatedAttributeClone : Dynamic cast for Association returned null"));
            prismAssert( false, __FILE__, __LINE__);
        }
        associatedObjectIds.push_back(pAttributeObjectIdAssociation->getValue());
        associatedClassName = pAttributeObjectIdAssociation->getAssociatedTo ();
    }
    else
    {
        AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation = dynamic_cast<AttributeObjectIdVectorAssociation *> (pAttribute);
        if ( NULL == pAttributeObjectIdVectorAssociation )
        {
            trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::getAssociatedAttributeClone : Dynamic cast for Vector Association returned null"));
            prismAssert( false, __FILE__, __LINE__);
        }
        associatedObjectIds = pAttributeObjectIdVectorAssociation->getValue();
        associatedClassName = pAttributeObjectIdVectorAssociation->getAssociatedTo ();
    }

    if ( parentClassName == associatedClassName )
    {
        trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::getAssociatedAttributeClone : ParentMO Name and associatedChildMO Name for ManagedObject ") + parentClassName + (" are same. Assert here"));
        prismAssert( false, __FILE__, __LINE__);
    }

    // We support only simple key in associated ManagedObject
    // Assert for other types
    prismAssert( 1 == associatedObjectIds.size(), __FILE__, __LINE__);

    if ( ObjectId::NullObjectId  == associatedObjectIds[0] )
    {
        trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::getAssociatedAttributeClone : ParentMO Name- ") + parentClassName + (", has empty associatedChildMO -") + associatedClassName );
        pAssociatedAttribute = NULL ;
        return ;
    }

    vector<string> keyAttributeName;

    OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (associatedClassName, keyAttributeName);
    prismAssert( 1 == keyAttributeName.size() ,__FILE__, __LINE__);

    pResults = querySynchronously (associatedClassName, associatedObjectIds);
    prismAssert( NULL != pResults, __FILE__, __LINE__);

    // Additional check. We support only simple key in associated ManagedObject
    // Assert for other types
    prismAssert(1 == (*pResults).size() ,__FILE__, __LINE__);

    WaveManagedObject* pWaveManagedObject = (*pResults)[0];

    Attribute *pRelatedAttribute = pWaveManagedObject->getAttributeByName ( keyAttributeName[0]);
    prismAssert( NULL != pRelatedAttribute, __FILE__, __LINE__);

    attrType = pRelatedAttribute->getAttributeType ();

    if ( (AttributeType::AttributeTypeObjectId == attrType ) || ( AttributeType::AttributeTypeObjectIdVector == attrType ) )
    {
        trace (TRACE_LEVEL_DEVEL, string("WaveObjectManager::getAssociatedAttributeClone : MO " + associatedClassName + " has associated key " + pRelatedAttribute->getAttributeName()));
        getAssociatedAttributeClone( associatedClassName, pRelatedAttribute, pAssociatedAttribute );
    }
    else if ( ( AttributeType::AttributeTypeComposition == attrType ) || ( AttributeType::AttributeTypeCompositionVector == attrType ) )
    {
        trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::getAssociatedAttributeClone : MO " + associatedClassName + " has composite key " + pRelatedAttribute->getAttributeName()));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        // Do we have to filter duplicate value and UC here
        pAssociatedAttribute = pRelatedAttribute->clone();
        prismAssert( NULL != pAssociatedAttribute, __FILE__, __LINE__);

    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    return ;
}

void WaveObjectManager::shutdownHandler (PrismShutdownObjectManagerMessage *pShutdownMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::shutdownShutdownWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::shutdownShutdownSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pShutdownMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::shutdownShutdownWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::shutdownShutdownWorkersStep : Entering ...");

    UI32 numberOfWorkers                            = m_workers.size ();
    UI32 i                                          = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::shutdownShutdownWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->shutdown (pWaveAsynchronousContextForShutDownPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::shutdownShutdownWorkersStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::shutdownShutdownWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForShutDownPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::shutdownShutdownWorkersStepCallback : Shutdowning one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::shutdownShutdownSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::shutdownShutdownSelfStep : Entering ...");

    WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::shutdownShutdownSelfStepCallback), pPrismLinearSequencerContext);

    shutdown (pWaveAsynchronousContextForShutDownPhases);
}

void WaveObjectManager::shutdownShutdownSelfStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::shutdownShutdownSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                   status                       = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForShutDownPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}


void WaveObjectManager::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    if (false == m_backendAttributeMap.empty())
    {
        map<string, map<string, UnifiedClientBackendDetails*> >::iterator backendShutdownMapIterator = m_backendAttributeMap.find("shutdown");
        if (backendShutdownMapIterator != m_backendAttributeMap.end())
        {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::notifyStep),
                reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContextForShutdownPhase *pPrismLinearSequencerContext = new PrismLinearSequencerContextForShutdownPhase (pWaveAsynchronousContextForShutDownPhases,  this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

            pPrismLinearSequencerContext->setBackendMap(backendShutdownMapIterator->second);
            pPrismLinearSequencerContext->start ();
        }
    }
    else
    {
        pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForShutDownPhases->callback ();
    }

}

void WaveObjectManager::notifyStep (PrismLinearSequencerContextForShutdownPhase *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "WaveObjectManager::notifyStep : Entering ...");

    ResourceId status =  WAVE_MESSAGE_SUCCESS ;

    map<string, UnifiedClientBackendDetails*> backendMap = pPrismLinearSequencerContext->getBackendMap();

    if (false == backendMap.empty ())
    {
        map<string, UnifiedClientBackendDetails*>::iterator backendNotifyValueMapIteratorBegin = backendMap.find("notify");

        if (backendNotifyValueMapIteratorBegin != backendMap.end())
        {
            string TaskName = backendNotifyValueMapIteratorBegin->first;

            UnifiedClientBackendDetails *backendDetails = dynamic_cast<UnifiedClientBackendDetails *> (backendNotifyValueMapIteratorBegin->second);
            string ClientName = backendDetails->getClientName();
            UI32   BackendValue = backendDetails->getBackendNotifyValue();

            trace (TRACE_LEVEL_INFO, string("WaveObjectManager::notifyStep  : TaskName is ") + TaskName + string(" Client Name is ")+ ClientName + string(" Backend value is ") + BackendValue);

            //Create Notify Message
            UnifiedClientNotifyMessage *pMessage = new UnifiedClientNotifyMessage ();
            prismAssert(NULL != pMessage ,__FILE__, __LINE__);

            //Set the BackendNotifyValue in message
            pMessage->setbackendNotifyValue (BackendValue);

            // send to backend Client
            status = sendSynchronouslyToWaveClient (ClientName,pMessage);
            // Check the send Status
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::notifyStep: Error while sending Message to client " + ClientName  + " with status : " + FrameworkToolKit::localize (status)));
            }
            else
            {
                // check the completionStatus
                if (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus ())
                {
                    status = pMessage->getCompletionStatus ();
                    trace (TRACE_LEVEL_FATAL, string("WaveObjectManager::notifyStep: Error while sending Message to client" + ClientName + " with Completionstatus : " + FrameworkToolKit::localize (status)));
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, string("WaveObjectManager::notifyStep : Succesufully send message to client " + ClientName  + " with Completionstatus : " + FrameworkToolKit::localize (status)));
                }
            }

            delete pMessage;
        }//BackendMAp contains notify entry
    }//if(BackendMap is not empty)

    pPrismLinearSequencerContext->executeNextStep (status);
}

void  WaveObjectManager::setBackendAttributeMap(const map<string, map<string,UnifiedClientBackendDetails*> > &backendAttributeMap)
{
    m_backendAttributeMap = backendAttributeMap ;
}

void  WaveObjectManager::getBackendAttributeMap(map<string, map<string,UnifiedClientBackendDetails*> > &backendAttributeMap) const
{
    backendAttributeMap = m_backendAttributeMap;
}

void WaveObjectManager::uninstallHandler (PrismUninstallObjectManagerMessage *pUninstallMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::uninstallUninstallWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::uninstallUninstallSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pUninstallMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::uninstallUninstallWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninstallUninstallWorkersStep : Entering ...");

    UI32 numberOfWorkers                            = m_workers.size ();
    UI32 i                                          = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::uninstallUninstallWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->uninstall (pWaveAsynchronousContextForShutDownPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::uninstallUninstallWorkersStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninstallUninstallWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForShutDownPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::uninstallUninstallWorkersStepCallback : Shutdowning one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::uninstallUninstallSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninstallUninstallSelfStep : Entering ...");

        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::uninstallUninstallSelfStepCallback), pPrismLinearSequencerContext);

    uninstall (pWaveAsynchronousContextForShutDownPhases);
}

void WaveObjectManager::uninstallUninstallSelfStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninstallUninstallSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                   status                       = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForShutDownPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::uninstall (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninstall : Entering ...");

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void WaveObjectManager::disableHandler (PrismDisableObjectManagerMessage *pDisableMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::disableUnlistenEventsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::disableDisableWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::disableDisableSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pDisableMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::disableUnlistenEventsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::disableUnlistenEventsStep : Entering ...");

    unlistenEvents ();

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::disableDisableWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::disableDisableWorkersStep : Entering ...");

    UI32 numberOfWorkers                            = m_workers.size ();
    UI32 i                                          = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::disableDisableWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->disable (pWaveAsynchronousContextForShutDownPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::disableDisableWorkersStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::disableDisableWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForShutDownPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::disableDisableWorkersStepCallback : Disabling one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::disableDisableSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::disableDisableSelfStep : Entering ...");

        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::disableDisableSelfStepCallback), pPrismLinearSequencerContext);

    disable (pWaveAsynchronousContextForShutDownPhases);
}

void WaveObjectManager::disableDisableSelfStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::disableDisableSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                   status                       = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    // Cancel any outstanding timers for this service

    ResourceId deleteTimersStatus = deleteAllTimersForService ();

    if (FRAMEWORK_SUCCESS != deleteTimersStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::disableDisableSelfStepCallback : Deleting all timers for service failed : ") + FrameworkToolKit::localize (deleteTimersStatus));
    }

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        setIsEnabled (false);

        // Add it to the services that are enabled.

        PrismServiceId thisServiceId = getServiceId ();

        removeServiceFromEnabledServicesList (thisServiceId);

        vector<PrismMessage *> incomingMessages;
        UI32                   numberOfIncomingMessages = 0;
        UI32                   i                        = 0;

        m_pAssociatedPrismThread->emptyIncomingMessageQueuesForDisable (incomingMessages);
        numberOfIncomingMessages = incomingMessages.size ();

        trace (TRACE_LEVEL_DEBUG, string ("Replying to all ") + numberOfIncomingMessages + " pending messages with service disabling error.");

        for (i = 0; i < numberOfIncomingMessages; i++)
        {
            PrismMessage *pPrismMessage = incomingMessages[i];

            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

            pPrismMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_SERVICE_DISABLING);
            reply (pPrismMessage);
        }
    }

    delete pWaveAsynchronousContextForShutDownPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::disable : Entering ...");

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void WaveObjectManager::uninitializeHandler (PrismUninitializeObjectManagerMessage *pUninitializeMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::uninitializeUninitializeWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::uninitializeUninitializeSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pUninitializeMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::uninitializeUninitializeWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninitializeUninitializeWorkersStep : Entering ...");

    UI32 numberOfWorkers                            = m_workers.size ();
    UI32 i                                          = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::uninitializeUninitializeWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->uninitialize (pWaveAsynchronousContextForShutDownPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::uninitializeUninitializeWorkersStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninitializeUninitializeWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForShutDownPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::uninitializeUninitializeWorkersStepCallback : Shutdowning one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::uninitializeUninitializeSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninitializeUninitializeSelfStep : Entering ...");

        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::uninitializeUninitializeSelfStepCallback), pPrismLinearSequencerContext);

    uninitialize (pWaveAsynchronousContextForShutDownPhases);
}

void WaveObjectManager::uninitializeUninitializeSelfStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninitializeUninitializeSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                   status                       = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForShutDownPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::uninitialize (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::uninitialize : Entering ...");

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void WaveObjectManager::destructHandler (PrismDestructObjectManagerMessage *pDestructMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::destructDestructWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::destructDestructSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pDestructMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::destructDestructWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::destructDestructWorkersStep : Entering ...");

    UI32 numberOfWorkers                            = m_workers.size ();
    UI32 i                                          = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::destructDestructWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->destruct (pWaveAsynchronousContextForShutDownPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::destructDestructWorkersStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::destructDestructWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForShutDownPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::destructDestructWorkersStepCallback : Destructing one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::destructDestructSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::destructDestructSelfStep : Entering ...");

    WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::destructDestructSelfStepCallback), pPrismLinearSequencerContext);

    destruct (pWaveAsynchronousContextForShutDownPhases);
}

void WaveObjectManager::destructDestructSelfStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::destructDestructSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                   status                       = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForShutDownPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::destruct (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::destruct : Entering ...");

    if (NULL != m_pAssociatedPrismThread)
    {
        m_pAssociatedPrismThread->requestForThreadTermination ();
    }

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void WaveObjectManager::heartbeatFailureHandler (PrismHeartbeatFailureObjectManagerMessage *pHeartbeatFailureMessage)
{
    m_pInputMessage = NULL; // Behave as if this message handler got called from nowhere.
                            // This is done so that the heartbeatFailure virtual function impementers are free to do async sends in their code still reply back to this even before async sends get back their replies.

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::heartbeatFailureWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::heartbeatFailureSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pHeartbeatFailureMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::heartbeatFailureWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::heartbeatFailureWorkersStep : Entering ...");

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::heartbeatFailureWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->heartBeatFailure (pPrismAsynchronousContext);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::heartbeatFailureWorkersStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::heartbeatFailureWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                 status                     = pPrismAsynchronousContext->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pPrismAsynchronousContext;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::heartbeatFailureWorkersStepCallback : Sending heartbeatFailure for one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::heartbeatFailureSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::heartbeatFailureSelfStep : Entering ...");

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::heartbeatFailureSelfStepCallback), pPrismLinearSequencerContext);

    heartbeatFailure (pPrismAsynchronousContext);
}

void WaveObjectManager::heartbeatFailureSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::heartbeatFailureSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                 status                     = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;

    // If we are a user specific task OM, then We must not be registered with Framework any more events since we cannot receive the events any way after this point.

    if (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (getServiceId ())))
    {
        // However some special OMs would like to listen to the BOOT Completion event and then unlisten for themselves.  If we unlisten at the boot phase itself,
        // they will never get a chance to receive the boot completion event.

        if (true == (getAllowAutomaticallyUnlistenForEvents ()))
        {
            unlistenEvents ();
        }
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::heartbeatFailure (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::heartbeatFailure : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveObjectManager::configReplayEndHandler (PrismConfigReplayEndObjectManagerMessage *pConfigReplayEndMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::configReplayEndWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::configReplayEndSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pConfigReplayEndMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::configReplayEndWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::configReplayEndWorkersStep : Entering ...");

    /* Currently since the ConfigReplayEndObjectManagerMessage doesnt contain any data members inside it,
     * we are not using message that is passed into this function. When this expands, we need to use this
     * message and pass it onto the WaveAsynchronousContextForConfigReplayEnd.
     * Please refer externalStateSynchronization for the method to use it.
    */

    //PrismConfigReplayEndObjectManagerMessage *pPrismConfigReplayEndObjectManagerMessage = reinterpret_cast<PrismConfigReplayEndObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd = new WaveAsynchronousContextForConfigReplayEnd (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::configReplayEndWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->configReplayEnd(pWaveAsynchronousContextForConfigReplayEnd);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::configReplayEndWorkersStepCallback (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::configReplayEndWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForConfigReplayEnd->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForConfigReplayEnd->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForConfigReplayEnd;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::configReplayEndWorkersStepCallback : Sending configReplayEnd for one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::configReplayEndSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::configReplayEndSelfStep : Entering ...");

    /* Currently since the ConfigReplayEndObjectManagerMessage doesnt contain any data members inside it,
     * we are not using message that is passed into this function. When this expands, we need to use this
     * message and pass it onto the WaveAsynchronousContextForConfigReplayEnd.
     * Please refer externalStateSynchronization for the method to use it.
    */
    //PrismConfigReplayEndObjectManagerMessage *pPrismConfigReplayEndObjectManagerMessage = reinterpret_cast<PrismConfigReplayEndObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd = new WaveAsynchronousContextForConfigReplayEnd (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::configReplayEndSelfStepCallback), pPrismLinearSequencerContext);

    configReplayEnd (pWaveAsynchronousContextForConfigReplayEnd);
}

void WaveObjectManager::configReplayEndSelfStepCallback (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::configReplayEndSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForConfigReplayEnd->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForConfigReplayEnd->getCompletionStatus ();

    delete pWaveAsynchronousContextForConfigReplayEnd;

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
{
    tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "WaveObjectManager::configReplayEnd : Entering ... ");

    pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForConfigReplayEnd->callback ();
}

void WaveObjectManager::slotFailoverHandler (PrismSlotFailoverObjectManagerMessage *pSlotFailoverMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::slotFailoverWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::slotFailoverSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSlotFailoverMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::slotFailoverWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::slotFailoverEndWorkersStep : Entering ...");

    PrismSlotFailoverObjectManagerMessage *pPrismSlotFailoverObjectManagerMessage = reinterpret_cast<PrismSlotFailoverObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover = new WaveAsynchronousContextForSlotFailover (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::slotFailoverWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForSlotFailover->setSlotNumber (pPrismSlotFailoverObjectManagerMessage->getSlotNumber () );

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->slotFailover (pWaveAsynchronousContextForSlotFailover);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::slotFailoverWorkersStepCallback (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::slotFailoverWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForSlotFailover->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForSlotFailover->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForSlotFailover;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::slotFailoverWorkersStepCallback : Sending slotFailover for one worker failed.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::slotFailoverSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::slotFailoverSelfStep : Entering ...");

    PrismSlotFailoverObjectManagerMessage *pPrismSlotFailoverObjectManagerMessage = reinterpret_cast<PrismSlotFailoverObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover = new WaveAsynchronousContextForSlotFailover (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::slotFailoverSelfStepCallback), pPrismLinearSequencerContext);
    pWaveAsynchronousContextForSlotFailover->setSlotNumber (pPrismSlotFailoverObjectManagerMessage->getSlotNumber () );

    slotFailover (pWaveAsynchronousContextForSlotFailover);
}

void WaveObjectManager::slotFailoverSelfStepCallback (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::slotFailoverSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForSlotFailover->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForSlotFailover->getCompletionStatus ();

    delete pWaveAsynchronousContextForSlotFailover;

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::slotFailover (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
{
    tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "WaveObjectManager::slotFailover : Entering ... ");

    pWaveAsynchronousContextForSlotFailover->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForSlotFailover->callback ();
}

void WaveObjectManager::multiPartitionCleanupHandler (PrismMultiPartitionCleanupObjectManagerMessage *pMultiPartitionCleanupMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::multiPartitionCleanupWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::multiPartitionCleanupSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMultiPartitionCleanupMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::multiPartitionCleanupWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::multiPartitionCleanupEndWorkersStep : Entering ...");

    PrismMultiPartitionCleanupObjectManagerMessage *pPrismMultiPartitionCleanupObjectManagerMessage = reinterpret_cast<PrismMultiPartitionCleanupObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    bool isPartialCleanup   = pPrismMultiPartitionCleanupObjectManagerMessage->getIsPartialCleanup ();
    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup = new WaveAsynchronousContextForMultiPartitionCleanup (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::multiPartitionCleanupWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForMultiPartitionCleanup->setPartitionName (pPrismMultiPartitionCleanupObjectManagerMessage->getPartitionName () );
        pWaveAsynchronousContextForMultiPartitionCleanup->setOwnerPartitionManagedObjectId (pPrismMultiPartitionCleanupObjectManagerMessage->getOwnerPartitionManagedObjectId ());

        ++(*pPrismLinearSequencerContext);

        if (false == isPartialCleanup)
        {
            m_workers[i]->multiPartitionCleanup (pWaveAsynchronousContextForMultiPartitionCleanup);
        }
        else
        {
            pWaveAsynchronousContextForMultiPartitionCleanup->setPartialCleanupTag (pPrismMultiPartitionCleanupObjectManagerMessage->getPartialCleanupTag ());
            m_workers[i]->multiPartitionPartialCleanup (pWaveAsynchronousContextForMultiPartitionCleanup);
        }
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::multiPartitionCleanupWorkersStepCallback (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::multiPartitionCleanupWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForMultiPartitionCleanup->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForMultiPartitionCleanup->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForMultiPartitionCleanup;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::multiPartitionCleanupWorkersStepCallback : Sending multiPartitionCleanup for one worker failed.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::multiPartitionCleanupSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::multiPartitionCleanupSelfStep : Entering ...");

    PrismMultiPartitionCleanupObjectManagerMessage *pPrismMultiPartitionCleanupObjectManagerMessage = reinterpret_cast<PrismMultiPartitionCleanupObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    bool isPartialCleanup   = pPrismMultiPartitionCleanupObjectManagerMessage->getIsPartialCleanup ();

    WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup = new WaveAsynchronousContextForMultiPartitionCleanup (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::multiPartitionCleanupSelfStepCallback), pPrismLinearSequencerContext);

    pWaveAsynchronousContextForMultiPartitionCleanup->setPartitionName (pPrismMultiPartitionCleanupObjectManagerMessage->getPartitionName () );
    pWaveAsynchronousContextForMultiPartitionCleanup->setOwnerPartitionManagedObjectId (pPrismMultiPartitionCleanupObjectManagerMessage->getOwnerPartitionManagedObjectId ());

    if (false == isPartialCleanup)
    {
        multiPartitionCleanup (pWaveAsynchronousContextForMultiPartitionCleanup);
    }
    else
    {
        pWaveAsynchronousContextForMultiPartitionCleanup->setPartialCleanupTag (pPrismMultiPartitionCleanupObjectManagerMessage->getPartialCleanupTag ());
        multiPartitionPartialCleanup (pWaveAsynchronousContextForMultiPartitionCleanup);
    }
}

void WaveObjectManager::multiPartitionCleanupSelfStepCallback (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::multiPartitionCleanupSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForMultiPartitionCleanup->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForMultiPartitionCleanup->getCompletionStatus ();

    delete pWaveAsynchronousContextForMultiPartitionCleanup;

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::multiPartitionCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
{
    tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "WaveObjectManager::multiPartitionCleanup : Entering ... ");

    ObjectId ownerPartitionManagedObjectId (pWaveAsynchronousContextForMultiPartitionCleanup->getOwnerPartitionManagedObjectId ());

    bool isLocalService = isALocalPrismService ();

    startTransaction();

    map<string, string>::iterator itr      = m_managedClasses.begin ();
    map<string, string>::iterator endItr   = m_managedClasses.end ();

    string managedClassName;

    for (; itr != endItr; itr++)
    {
        managedClassName = itr->first;

      /*  bool isManagedView = OrmRepository::isManagedClassAView (managedClassName);
        if (true == isManagedView)
        {
            continue;
        }*/

        bool isLocalManagedObject = OrmRepository::isALocalManagedObject (managedClassName);

        if ((false == isLocalService) && (true == isLocalManagedObject))
        {
            continue;
        }

        WaveManagedObjectSynchronousQueryContextForDeletion waveManagedObjectSynchronousQueryContextForDeletion (managedClassName);
        if (true == isLocalManagedObject)
        {
            ObjectId ownerWaveNodeObjectId = FrameworkToolKit::getObjectIdForLocationId (FrameworkToolKit::getThisLocationId ());
            waveManagedObjectSynchronousQueryContextForDeletion.addAndAttribute (new AttributeObjectId (ownerWaveNodeObjectId, "ownerWaveNodeObjectId"));
        }
        waveManagedObjectSynchronousQueryContextForDeletion.addAndAttribute (new AttributeObjectId (ownerPartitionManagedObjectId, "ownerPartitionManagedObjectId"));

        deleteWaveManagedObject (&waveManagedObjectSynchronousQueryContextForDeletion);
    }

    ResourceId status =  commitTransaction();

    if (FRAMEWORK_SUCCESS == status)
    {
        pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (status);
    }

    pWaveAsynchronousContextForMultiPartitionCleanup->callback ();
}

void WaveObjectManager::multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
{
    tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "WaveObjectManager::multiPartitionPartialCleanup : Entering ... ");

    prismAssert (true == pWaveAsynchronousContextForMultiPartitionCleanup->getIsPartialCleanup (), __FILE__, __LINE__);

    pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForMultiPartitionCleanup->callback ();
}

void WaveObjectManager::fileReplayEndHandler (PrismFileReplayEndObjectManagerMessage *pFileReplayEndMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::fileReplayEndWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::fileReplayEndSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFileReplayEndMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::fileReplayEndWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::fileReplayEndWorkersStep : Entering ...");

    /* Currently since the FileReplayEndObjectManagerMessage doesnt contain any data members inside it,
     * we are not using message that is passed into this function. When this expands, we need to use this
     * message and pass it onto the WaveAsynchronousContextForFileReplayEnd.
     * Please refer externalStateSynchronization for the method to use it.
    */

    //PrismFileReplayEndObjectManagerMessage *pPrismFileReplayEndObjectManagerMessage = reinterpret_cast<PrismFileReplayEndObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd = new WaveAsynchronousContextForFileReplayEnd (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::fileReplayEndWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->fileReplayEnd(pWaveAsynchronousContextForFileReplayEnd);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::fileReplayEndWorkersStepCallback (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::fileReplayEndWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForFileReplayEnd->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForFileReplayEnd->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForFileReplayEnd;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::fileReplayEndWorkersStepCallback : Sending fileReplayEnd for one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::fileReplayEndSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::fileReplayEndSelfStep : Entering ...");

    /* Currently since the FileReplayEndObjectManagerMessage doesnt contain any data members inside it,
     * we are not using message that is passed into this function. When this expands, we need to use this
     * message and pass it onto the WaveAsynchronousContextForFileReplayEnd.
     * Please refer externalStateSynchronization for the method to use it.
    */
    //PrismFileReplayEndObjectManagerMessage *pPrismFileReplayEndObjectManagerMessage = reinterpret_cast<PrismFileReplayEndObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd = new WaveAsynchronousContextForFileReplayEnd (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::fileReplayEndSelfStepCallback), pPrismLinearSequencerContext);

    fileReplayEnd (pWaveAsynchronousContextForFileReplayEnd);
}

void WaveObjectManager::fileReplayEndSelfStepCallback (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::fileReplayEndSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForFileReplayEnd->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForFileReplayEnd->getCompletionStatus ();

    delete pWaveAsynchronousContextForFileReplayEnd;

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::fileReplayEnd (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd)
{
    tracePrintf (TRACE_LEVEL_INFO, false, false,  "WaveObjectManager::fileReplayEnd : Entering ... ");

    pWaveAsynchronousContextForFileReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForFileReplayEnd->callback ();
}

void WaveObjectManager::externalStateSynchronizationHandler (PrismExternalStateSynchronizationObjectManagerMessage *pExternalStateSynchronizationMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::externalStateSynchronizationWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::externalStateSynchronizationSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pExternalStateSynchronizationMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::externalStateSynchronizationWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::externalStateSynchronizationWorkersStep : Entering ...");

    PrismExternalStateSynchronizationObjectManagerMessage *pPrismExternalStateSynchronizationObjectManagerMessage = reinterpret_cast<PrismExternalStateSynchronizationObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization = new WaveAsynchronousContextForExternalStateSynchronization (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::externalStateSynchronizationWorkersStepCallback), pPrismLinearSequencerContext);
        pWaveAsynchronousContextForExternalStateSynchronization->setFssStageNumber (pPrismExternalStateSynchronizationObjectManagerMessage->getFssStageNumber ());
        pWaveAsynchronousContextForExternalStateSynchronization->setServiceType    (pPrismExternalStateSynchronizationObjectManagerMessage->getServiceType());

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->externalStateSynchronization(pWaveAsynchronousContextForExternalStateSynchronization);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::externalStateSynchronizationWorkersStepCallback (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::externalStateSynchronizationWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForExternalStateSynchronization->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForExternalStateSynchronization->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForExternalStateSynchronization;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::externalStateSynchronizationWorkersStepCallback : Sending externalStateSynchronization for one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::externalStateSynchronizationSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::externalStateSynchronizationSelfStep : Entering ...");

    PrismExternalStateSynchronizationObjectManagerMessage *pPrismExternalStateSynchronizationObjectManagerMessage = reinterpret_cast<PrismExternalStateSynchronizationObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization = new WaveAsynchronousContextForExternalStateSynchronization (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::externalStateSynchronizationSelfStepCallback), pPrismLinearSequencerContext);

    pWaveAsynchronousContextForExternalStateSynchronization->setFssStageNumber (pPrismExternalStateSynchronizationObjectManagerMessage->getFssStageNumber ());
    pWaveAsynchronousContextForExternalStateSynchronization->setServiceType    (pPrismExternalStateSynchronizationObjectManagerMessage->getServiceType());

    externalStateSynchronization (pWaveAsynchronousContextForExternalStateSynchronization);
}

void WaveObjectManager::externalStateSynchronizationSelfStepCallback (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::externalStateSynchronizationSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForExternalStateSynchronization->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForExternalStateSynchronization->getCompletionStatus ();

    delete pWaveAsynchronousContextForExternalStateSynchronization;

    // If we are a user specific task OM, then We must not be registered with Framework any more events since we cannot receive the events any way after this point.

    if (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (getServiceId ())))
    {
        // However some special OMs would like to listen to the BOOT Completion event and then unlisten for themselves.  If we unlisten at the boot phase itself,
        // they will never get a chance to receive the boot completion event.

        if (true == (getAllowAutomaticallyUnlistenForEvents ()))
        {
            unlistenEvents ();
        }
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
{
    tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "WaveObjectManager::externalStateSynchronization : Entering ... ");

    pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForExternalStateSynchronization->callback ();
}

void WaveObjectManager::addWorker (WaveWorker *pWaveWorker)
{
    m_workersMutex.lock ();

    if (NULL != pWaveWorker)
    {
        m_workers.insert (m_workers.end (), pWaveWorker);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addWorker : Trying to add a NULL worker to this manager.  Will not add.");
        prismAssert (false, __FILE__, __LINE__);
        m_workersMutex.unlock ();
        return;
    }

    m_workersMutex.unlock ();
}

void WaveObjectManager::removeWorker (WaveWorker *pWaveWorker)
{
    m_workersMutex.lock ();

    if (NULL != pWaveWorker)
    {
        vector<WaveWorker*>::iterator element    = m_workers.begin ();
        vector<WaveWorker*>::iterator endElement = m_workers.end   ();
        bool                          found      = false;

        while (element != endElement)
        {
            if (pWaveWorker == (*element))
            {
                m_workers.erase (element);
                found = true;
                break;
            }

            element++;
        }

        if (false == found)
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::removeWorker : Trying to remove an invalid Worker.");
            prismAssert (false, __FILE__, __LINE__);
            m_workersMutex.unlock ();
            return;
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addWorker : Trying to remove a NULL worker from this manager.  Will not remove.");
        prismAssert (false, __FILE__, __LINE__);
        m_workersMutex.unlock ();
        return;
    }

    m_workersMutex.unlock ();
}

PrismServiceId WaveObjectManager::getServiceId ()
{
    return (m_serviceId);
}

TraceClientId WaveObjectManager::getTraceClientId ()
{
    return (m_traceClientId);
}

void WaveObjectManager::holdMessages ()
{
    m_pAssociatedPrismThread->holdMessages ();
}

void WaveObjectManager::holdHighPriorityMessages ()
{
    m_pAssociatedPrismThread->holdHighPriorityMessages ();
}

void WaveObjectManager::holdEvents ()
{
    m_pAssociatedPrismThread->holdEvents ();
}

void WaveObjectManager::holdAll ()
{
    m_pAssociatedPrismThread->holdAll ();
}

void WaveObjectManager::unholdMessages ()
{
    m_pAssociatedPrismThread->unholdMessages ();
}

void WaveObjectManager::unholdHighPriorityMessages ()
{
    m_pAssociatedPrismThread->unholdHighPriorityMessages ();
}

void WaveObjectManager::unholdEvents ()
{
    m_pAssociatedPrismThread->unholdEvents ();
}

void WaveObjectManager::unholdAll ()
{
    m_pAssociatedPrismThread->unholdAll ();
}

bool WaveObjectManager::getIsEnabled ()
{
    bool isEnabled = false;

    m_isEnabledMutex.lock ();
    isEnabled = m_isEnabled;
    m_isEnabledMutex.unlock ();

    return (isEnabled);
}

void WaveObjectManager::setIsEnabled (bool isEnabled)
{
    m_isEnabledMutex.lock ();
    m_isEnabled = isEnabled;
    m_isEnabledMutex.unlock ();
}

bool WaveObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
{
    switch (operationCode)
    {
        case WAVE_OBJECT_MANAGER_INITIALIZE :
        case WAVE_OBJECT_MANAGER_ENABLE :
        case WAVE_OBJECT_MANAGER_UNINITIALIZE:
        case WAVE_OBJECT_MANAGER_DESTRUCT:
        case WAVE_OBJECT_MANAGER_DATABASE_SANITY_CHECK:
            return (true);
        default :
            return (false);
    }
}

bool WaveObjectManager::isEventAllowedBeforeEnabling (const UI32 &eventOperationCode)
{
    // At present we do not allow any events to be passed to any service with out enabling the service.

    return (false);
}

bool WaveObjectManager::canInstantiateServiceAtThisTime (const string &prismServiceName)
{
    // This method ensures that no other service gets instantiated before the Framework Service itself gets instantiated.

    if ("Prism Framework" == prismServiceName)
    {
        return (true);
    }
    else
    {
        if (true == (PrismFrameworkObjectManager::getIsInstantiated ()))
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
}

void WaveObjectManager::prismAssert (bool isAssertNotRequired, const char *pFileName, UI32 lineNumber)
{
    // Here also like in the trace method, We cannot use any tracing statement here for obvious reasons (recursion!!!)
    if (false == isAssertNotRequired)
    {

        string stringToTrace (pFileName);

        stringToTrace = stringToTrace + " : " + lineNumber;

        trace (TRACE_LEVEL_FATAL, stringToTrace);

        SI32             numberOfSymbols       = backtrace (s_pBackTraceCalledSite, s_bactraceFrameCount);
        char           **pBackTraceSymbols     = backtrace_symbols (s_pBackTraceCalledSite, numberOfSymbols);
        SI32             i                     = 0;
        SI32             demangleStatus        = 0;
        SI32             indexOm               = -1;

        s_bactraceMessageLength = snprintf (s_backTraceMessageBuffer, s_backtraceMessageBufferLength, "\n\r");



        for (i = 0; i < numberOfSymbols; i++)
        {
            if ( NULL != strstr(pBackTraceSymbols[i], "prismAssert") )
            {
                continue;
            }

            if ( NULL != strstr(pBackTraceSymbols[i], "sigSegvHandler") )
            {
                continue;
            }
            if ( (indexOm == -1) && ((NULL != strstr(pBackTraceSymbols[i], "ObjectManager")) || (NULL != strstr(pBackTraceSymbols[i],"Worker"))))
            {
                indexOm = i;
            }
            s_bactraceMessageLength += snprintf (s_backTraceMessageBuffer + s_bactraceMessageLength, s_backtraceMessageBufferLength, "%s\n\r", pBackTraceSymbols[i]);
        }

        if (WAVE_MGMT_INTF_ROLE_SERVER == (FrameworkToolKit::getManagementInterfaceRole ()))
        {
            trace (TRACE_LEVEL_FATAL, "Mangled:");
            trace (TRACE_LEVEL_FATAL, "________");
            trace (TRACE_LEVEL_FATAL, s_backTraceMessageBuffer);
        }

        snprintf (s_crashForOMMessage, s_demangleOutputLength, "Crashed  in Unknown OM/Worker");

        /*
         * Reset the Message for Demangled Message
         */
        s_bactraceMessageLength = snprintf (s_backTraceMessageBuffer, s_backtraceMessageBufferLength, "\n\r");
        for (i = 0; i < numberOfSymbols; i++)
        {
            s_tempBackTraceBuffer = pBackTraceSymbols[i];
            if ( NULL != strstr(pBackTraceSymbols[i], "prismAssert") )
            {
                continue;
            }

            if ( NULL != strstr(pBackTraceSymbols[i], "sigSegvHandler") )
            {
                continue;
            }

            tokenize (s_tempBackTraceBuffer, firstLevelTokens, '(');

            if (2 <= (firstLevelTokens.size ()))
            {
                s_tempBackTraceBuffer = firstLevelTokens[1];

                tokenize (s_tempBackTraceBuffer, secondLevelTokens, '+');

                if (1 <= (secondLevelTokens.size ()))
                {
                    memset (s_pDemangleOutPutBuffer, 0, s_demangleOutputLength);

                    char *pDemangledBuffer = abi::__cxa_demangle (secondLevelTokens[0].c_str (), s_pDemangleOutPutBuffer, &s_demangleOutputLength, &demangleStatus);

                    if (0 == demangleStatus)
                    {
                        if ( i == indexOm )
                        {
                            snprintf (s_crashForOMMessage, s_demangleOutputLength, "** Crashed in OM/Worker (%s)", pDemangledBuffer);
                        }
                        s_bactraceMessageLength  += snprintf (s_backTraceMessageBuffer + s_bactraceMessageLength, s_backtraceMessageBufferLength, "%s\n\r", pDemangledBuffer);

                    }
                    else
                    {
                        s_bactraceMessageLength += snprintf (s_backTraceMessageBuffer + s_bactraceMessageLength, s_backtraceMessageBufferLength, "%s\n\r", secondLevelTokens[0].c_str());
                    }
                }
                else
                {
                    s_bactraceMessageLength += snprintf (s_backTraceMessageBuffer + s_bactraceMessageLength, s_backtraceMessageBufferLength, "%s\n\r",  pBackTraceSymbols[i]);
                }
            }
            else
            {
                s_bactraceMessageLength += snprintf (s_backTraceMessageBuffer + s_bactraceMessageLength, s_backtraceMessageBufferLength, "%s\n\r",  pBackTraceSymbols[i]);
            }
        }

        if (WAVE_MGMT_INTF_ROLE_SERVER == (FrameworkToolKit::getManagementInterfaceRole ()))
        {
            trace (TRACE_LEVEL_FATAL, "");
            trace (TRACE_LEVEL_FATAL, "De-Mangled (for quick lookup purposes only, use mangled output to get more information):");
            trace (TRACE_LEVEL_FATAL, "*******************************************************************************************************");
            trace (TRACE_LEVEL_FATAL, s_crashForOMMessage);
            trace (TRACE_LEVEL_FATAL, "*******************************************************************************************************");

            trace (TRACE_LEVEL_FATAL, "___________");

            trace (TRACE_LEVEL_FATAL, s_backTraceMessageBuffer);
        }

        // In case of Wave server application running as a daemon, following function will print backtrace details on console.
        FrameworkToolKit::initializeConsolePrintFunction ();
        FrameworkToolKit::consolePrint ("********************************************************************************************************");
        FrameworkToolKit::consolePrint (s_crashForOMMessage);
        FrameworkToolKit::consolePrint ("********************************************************************************************************");
        FrameworkToolKit::consolePrint (s_backTraceMessageBuffer);

        delete[] s_pBackTraceCalledSite;
        free (pBackTraceSymbols);

        assert (0);
    }
}

void WaveObjectManager::pingHandler (PrismPingObjectManagerMessage *pPingMessage)
{
    pPingMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pPingMessage);
}

ResourceId WaveObjectManager::startTimer (TimerHandle &timerHandle, timeval &startInterval, timeval &periodicInterval, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext, PrismElement *pPrismTimerSender)
{
    timeval             startTime;

    timerHandle = 0;

    if (0 != gettimeofday (&startTime, NULL))
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::startTimer : error getting time.");
        return (FRAMEWORK_TIMER_CAN_NOT_START);
    }

    if ((0 > startInterval.tv_sec) || (0 > startInterval.tv_usec) || ((0 == startInterval.tv_sec) && (0 == startInterval.tv_usec)) || (1000000 <= startInterval.tv_usec))
    {
        return (FRAMEWORK_TIMER_INVALID_START_INTERVAL);
    }

    if ((0 > periodicInterval.tv_sec) || (0 > periodicInterval.tv_usec) || (1000000 <= periodicInterval.tv_usec))
    {
        return (FRAMEWORK_TIMER_INVALID_PERIODIC_INTERVAL);
    }

    if (((PrismTimerExpirationHandler) NULL) == pPrismTimerExpirationCallback)
    {
        return (FRAMEWORK_TIMER_INVALID_CALLBACK);
    }

    TimerObjectManagerAddTimerMessage   *pStartTimerMessage = new TimerObjectManagerAddTimerMessage (startInterval, periodicInterval, startTime, pPrismTimerExpirationCallback, pPrismTimerExpirationContext, (NULL == pPrismTimerSender) ? this : pPrismTimerSender);

    if (NULL == pStartTimerMessage)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::addTimer : Error allocating add Timer Msg.");
        return (FRAMEWORK_TIMER_NO_MEMORY);
    }

    WaveMessageStatus status = sendSynchronously (pStartTimerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::startTimer : add timer message failed.");
        delete (pStartTimerMessage);

        return (FRAMEWORK_TIMER_CAN_NOT_START);
    }

    if (TIMER_SUCCESS == pStartTimerMessage->getCompletionStatus ())
    {
        timerHandle = pStartTimerMessage->getTimerId ();
        delete (pStartTimerMessage);

        return (FRAMEWORK_SUCCESS);
    }
    else
    {
        timerHandle = 0;
        delete (pStartTimerMessage);

        return (FRAMEWORK_TIMER_CAN_NOT_START);
    }
}

ResourceId WaveObjectManager::startTimer (TimerHandle &timerHandle, UI32 timeInMilliSeconds, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext, PrismElement *pPrismTimerSender)
{
    timeval startInterval;
    timeval periodicInterval;

    startInterval.tv_sec     = timeInMilliSeconds / 1000;
    startInterval.tv_usec    = (timeInMilliSeconds % 1000) * 1000;
    periodicInterval.tv_sec  = 0;
    periodicInterval.tv_usec = 0;

    return (startTimer (timerHandle, startInterval, periodicInterval, pPrismTimerExpirationCallback, pPrismTimerExpirationContext, pPrismTimerSender));
}

void WaveObjectManager::timerExpiredHandler (PrismTimerExpiredObjectManagerMessage *pTimerExpiredMessage)
{
    prismAssert (NULL != pTimerExpiredMessage, __FILE__, __LINE__);
    prismAssert (NULL != pTimerExpiredMessage->getTimerSender (), __FILE__, __LINE__);
    prismAssert (((PrismTimerExpirationHandler) NULL) != (pTimerExpiredMessage->getTimerExpirationCallback ()), __FILE__, __LINE__);

    TimerHandle                     timerHandle             = pTimerExpiredMessage->getTimerId ();
    PrismElement                   *pSender                 = pTimerExpiredMessage->getTimerSender ();
    PrismTimerExpirationHandler     pTimerExpirationHandler = pTimerExpiredMessage->getTimerExpirationCallback ();
    void                           *pContext                = pTimerExpiredMessage->getTimerExpirationContext();

    reply (pTimerExpiredMessage);

    (pSender->*(pTimerExpirationHandler)) (timerHandle, pContext);
}

ResourceId WaveObjectManager::deleteTimer (TimerHandle timerHandle)
{
    ResourceId returnCode;

    TimerObjectManagerDeleteTimerMessage *pDeleteTimerMessage = new TimerObjectManagerDeleteTimerMessage (timerHandle);

    if (NULL == pDeleteTimerMessage)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::deleteTimer : Error allocating delete Timer Msg.");
        return (FRAMEWORK_TIMER_NO_MEMORY);
    }

    WaveMessageStatus status = sendSynchronously (pDeleteTimerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::deleteTimer : delete timer message failed.");
        delete (pDeleteTimerMessage);
        return (FRAMEWORK_TIMER_CAN_NOT_DELETE);
    }
    else
    {
        returnCode = pDeleteTimerMessage->getCompletionStatus ();

        delete (pDeleteTimerMessage);

        if (TIMER_SUCCESS != returnCode)
        {
            return (FRAMEWORK_TIMER_CAN_NOT_DELETE);
        }
        else
        {
            returnCode = FRAMEWORK_SUCCESS;
        }

        return (returnCode);
    }
}

ResourceId WaveObjectManager::deleteAllTimersForService ()
{
    ResourceId returnStatus = FRAMEWORK_SUCCESS;

    TimerObjectManagerDeleteAllTimersForServiceMessage *pDeleteAllTimersForServiceMessage = new TimerObjectManagerDeleteAllTimersForServiceMessage ();

    if (NULL != pDeleteAllTimersForServiceMessage)
    {
        WaveMessageStatus sendStatus = sendOneWay (pDeleteAllTimersForServiceMessage);

        if (WAVE_MESSAGE_SUCCESS == sendStatus)
        {
            returnStatus = FRAMEWORK_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::deleteAllTimersForService : Delete all timers for service message failed.");

            returnStatus = FRAMEWORK_TIMER_CAN_NOT_DELETE;

            delete pDeleteAllTimersForServiceMessage;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::deleteAllTimersForService : Error allocating delete all timers for service message.");

        returnStatus = FRAMEWORK_TIMER_NO_MEMORY;
    }

    return (returnStatus);
}

UI32 WaveObjectManager::getNumberOfPendingTimerExpirationMessages ()
{
    return (m_pAssociatedPrismThread->getNumberOfPendingTimerExpirationMessages ());
}

UI32 WaveObjectManager::getNumberOfPendingNormalMessages ()
{
    return (m_pAssociatedPrismThread->getNumberOfPendingNormalMessages ());
}

UI32 WaveObjectManager::getNumberOfPendingHighPriorityMessages ()
{
    return (m_pAssociatedPrismThread->getNumberOfPendingHighPriorityMessages ());
}

bool WaveObjectManager::isALocalPrismService ()
{
    return (false);
}

void WaveObjectManager::addServiceToEnabledServicesList (const PrismServiceId &prismServiceId)
{
    s_enabledServicesMutex.lock ();
    s_enabledServices[prismServiceId] = prismServiceId;
    s_enabledServicesMutex.unlock ();
}

void WaveObjectManager::removeServiceFromEnabledServicesList (const PrismServiceId &prismServiceId)
{
    s_enabledServicesMutex.lock ();

    map<PrismServiceId, PrismServiceId>::iterator element = s_enabledServices.find (prismServiceId);
    map<PrismServiceId, PrismServiceId>::iterator end     = s_enabledServices.end ();

    if (element != end)
    {
        s_enabledServices.erase (element);
    }

    s_enabledServicesMutex.unlock ();
}

void WaveObjectManager::getListOfEnabledServices (vector<PrismServiceId> &enabledServices)
{
    s_enabledServicesMutex.lock ();

    map<PrismServiceId, PrismServiceId>::iterator element = s_enabledServices.begin ();
    map<PrismServiceId, PrismServiceId>::iterator end     = s_enabledServices.end ();

    enabledServices.clear ();

    while (end != element)
    {
        enabledServices.push_back (element->second);
        element++;
    }

    s_enabledServicesMutex.unlock ();
}

bool WaveObjectManager::isServiceEnabled (const PrismServiceId &prismServiceId)
{
    s_enabledServicesMutex.lock ();

    map<PrismServiceId, PrismServiceId>::iterator element   = s_enabledServices.find (prismServiceId);
    map<PrismServiceId, PrismServiceId>::iterator end       = s_enabledServices.end ();
    bool                                          isEnabled = false;

    if (element != end)
    {
        isEnabled = true;
    }
    else
    {
        isEnabled = false;
    }

    s_enabledServicesMutex.unlock ();

    return (isEnabled);
}

void WaveObjectManager::clusterCreateCollectValidationDataHandler (WaveObjectManagerCollectValidationDataMessage *pWaveObjectManagerCollectValidationDataMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::clusterCreateCollectValidationDataWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::clusterCreateCollectValidationDataSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerCollectValidationDataMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::clusterCreateCollectValidationDataWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::clusterCreateCollectValidationDataWorkersStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
}

void WaveObjectManager::clusterCreateCollectValidationDataSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateCollectValidationDataSelfStep : Entering ...");

    WaveObjectManagerCollectValidationDataMessage *pMessage = reinterpret_cast<WaveObjectManagerCollectValidationDataMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::clusterCreateCollectValidationDataSelfStepCallback), pPrismLinearSequencerContext);

    clusterCreateCollectValidationData (pMessage, pPrismAsynchronousContext);
}

void WaveObjectManager::clusterCreateCollectValidationDataSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateCollectValidationDataSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateCollectValidationData : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveObjectManager::clusterCreateValidateHandler (WaveObjectManagerValidateClusterCreationMessage *pWaveObjectManagerValidateClusterCreationMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::clusterCreateValidateWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::clusterCreateValidateSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerValidateClusterCreationMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::clusterCreateValidateWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::clusterCreateValidateWorkersStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
}

void WaveObjectManager::clusterCreateValidateSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateCollectValidationDataSelfStep : Entering ...");

    WaveObjectManagerValidateClusterCreationMessage *pMessage = reinterpret_cast<WaveObjectManagerValidateClusterCreationMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::clusterCreateValidateSelfStepCallback), pPrismLinearSequencerContext);

    clusterCreateValidate (pMessage, pPrismAsynchronousContext);
}

void WaveObjectManager::clusterCreateValidateSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateCollectValidationDataSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateValidate : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveObjectManager::clusterCreateSendValidationResultsHandler (WaveObjectManagerSendValidationResultsMessage *pWaveObjectManagerSendValidationResultsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::clusterCreateSendValidationResultsWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::clusterCreateSendValidationResultsSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerSendValidationResultsMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::clusterCreateSendValidationResultsWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::clusterCreateSendValidationResultsWorkersStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
}

void WaveObjectManager::clusterCreateSendValidationResultsSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateSendValidationResultsSelfStep : Entering ...");

    WaveObjectManagerSendValidationResultsMessage *pMessage = reinterpret_cast<WaveObjectManagerSendValidationResultsMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::clusterCreateSendValidationResultsSelfStepCallback), pPrismLinearSequencerContext);

    clusterCreateSendValidationResults (pMessage, pPrismAsynchronousContext);
}

void WaveObjectManager::clusterCreateSendValidationResultsSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateCollectValidationDataSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::clusterCreateSendValidationResults (WaveObjectManagerSendValidationResultsMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::clusterCreateSendValidationResults : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveObjectManager::haSyncCollectValidationDataHandler (WaveObjectManagerHaSyncCollectValidationDataMessage *pWaveObjectManagerHaSyncCollectValidationDataMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::haSyncCollectValidationDataWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::haSyncCollectValidationDataSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerHaSyncCollectValidationDataMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::haSyncCollectValidationDataWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::haSyncCollectValidationDataSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncCollectValidationDataSelfStep : Entering ...");

    WaveObjectManagerHaSyncCollectValidationDataMessage *pMessage = reinterpret_cast<WaveObjectManagerHaSyncCollectValidationDataMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::haSyncCollectValidationDataSelfStepCallback), pPrismLinearSequencerContext);

    haSyncCollectValidationData (pMessage, pPrismAsynchronousContext);
}

void WaveObjectManager::haSyncCollectValidationDataSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncCollectValidationDataSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::haSyncCollectValidationData (WaveObjectManagerHaSyncCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::HaSyncCollectValidationData : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveObjectManager::haSyncValidateDataHandler (WaveObjectManagerHaSyncValidateDataMessage *pWaveObjectManagerHaSyncValidateDataMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::haSyncValidateDataWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::haSyncValidateDataSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerHaSyncValidateDataMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::haSyncValidateDataWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::haSyncValidateDataSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncCollectValidationDataSelfStep : Entering ...");

    WaveObjectManagerHaSyncValidateDataMessage *pMessage = reinterpret_cast<WaveObjectManagerHaSyncValidateDataMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::haSyncValidateDataSelfStepCallback), pPrismLinearSequencerContext);

    haSyncValidateData (pMessage, pPrismAsynchronousContext);
}

void WaveObjectManager::haSyncValidateDataSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncValidateDataSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::haSyncValidateData (WaveObjectManagerHaSyncValidateDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncValidateData : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveObjectManager::haSyncSendValidationResultsHandler (WaveObjectManagerHaSyncSendValidationResultsMessage *pWaveObjectManagerHaSyncSendValidationResultsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::haSyncSendValidationResultsWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::haSyncSendValidationResultsSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerHaSyncSendValidationResultsMessage, this, sequencerSteps, sizeof
(sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::haSyncSendValidationResultsWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::haSyncSendValidationResultsSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncSendValidationResultsSelfStep : Entering ...");

    WaveObjectManagerHaSyncSendValidationResultsMessage *pMessage = reinterpret_cast<WaveObjectManagerHaSyncSendValidationResultsMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::haSyncSendValidationResultsSelfStepCallback), pPrismLinearSequencerContext);

    haSyncSendValidationResults (pMessage, pPrismAsynchronousContext);
}

void WaveObjectManager::haSyncSendValidationResultsSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncSendValidationResultsSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::haSyncSendValidationResults (WaveObjectManagerHaSyncSendValidationResultsMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::haSyncSendValidationResults : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveObjectManager::getDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::getDebugInformation : Entering ...");

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

void WaveObjectManager::resetDebugInformation (WaveAsynchronousContextForDebugInformation *pWaveAsynchronousContextForDebugInformation)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::resetDebugInformation : Entering ...");

    pWaveAsynchronousContextForDebugInformation->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForDebugInformation->callback ();
}

void WaveObjectManager::failoverHandler (PrismFailoverObjectManagerMessage *pPrismFailoverObjectManagerMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::failoverWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::failoverSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPrismFailoverObjectManagerMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::failoverWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::failoverWorkersStep : Entering ...");

    UI32                               numberOfWorkers = m_workers.size ();
    UI32                               i               = 0;
    PrismFailoverObjectManagerMessage *pMessage        = reinterpret_cast<PrismFailoverObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    FrameworkObjectManagerFailoverReason failoverReason                     = pMessage->getFailoverReason ();
    vector<LocationId>                   failedLocationIds                  = pMessage->getFailedLocationIds ();
    bool                                 isPrincipalChangedWithThisFailover = pMessage->getIsPrincipalChangedWithThisFailover ();

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        FailoverAsynchronousContext *pFailoverAsynchronousContext = new FailoverAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::failoverWorkersStepCallback), pPrismLinearSequencerContext, failoverReason, failedLocationIds, isPrincipalChangedWithThisFailover);

        ++(*pPrismLinearSequencerContext);
        //m_workers[i]->failover (pMessage->getFailoverReason (), pMessage->getFailedLocationIds (), pPrismAsynchronousContext);
        m_workers[i]->failover (pFailoverAsynchronousContext);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::failoverWorkersStepCallback (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::failoverWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pFailoverAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pFailoverAsynchronousContext->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pFailoverAsynchronousContext;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::failoverWorkersStepCallback : Failover one worker failed.");
        prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::failoverSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::failoverSelfStep : Entering ...");

    PrismFailoverObjectManagerMessage *pMessage = reinterpret_cast<PrismFailoverObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    FrameworkObjectManagerFailoverReason failoverReason                     = pMessage->getFailoverReason ();
    vector<LocationId>                   failedLocationIds                  = pMessage->getFailedLocationIds ();
    bool                                 isPrincipalChangedWithThisFailover = pMessage->getIsPrincipalChangedWithThisFailover ();

    FailoverAsynchronousContext *pFailoverAsynchronousContext = new FailoverAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::failoverSelfStepCallback), pPrismLinearSequencerContext, failoverReason, failedLocationIds, isPrincipalChangedWithThisFailover);

    //failover (pMessage->getFailoverReason (), pMessage->getFailedLocationIds (), pPrismAsynchronousContext);
    failover (pFailoverAsynchronousContext);
}

void WaveObjectManager::failoverSelfStepCallback (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::failoverSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pFailoverAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pFailoverAsynchronousContext->getCompletionStatus ();

    delete pFailoverAsynchronousContext;
    pPrismLinearSequencerContext->executeNextStep (status);
}
/*
void WaveObjectManager::failover (FrameworkObjectManagerFailoverReason failoverReason, vector<LocationId> failedLocationIds, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::failover : Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}
*/
void WaveObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::failover : Entering ...");

    pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pFailoverAsynchronousContext->callback ();
}

void WaveObjectManager::pauseHandler (PrismPauseObjectManagerMessage *pPrismPauseObjectManagerMessage)
{
    holdAll ();

    pPrismPauseObjectManagerMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pPrismPauseObjectManagerMessage);
}

void WaveObjectManager::resumeHandler (PrismResumeObjectManagerMessage *pPrismResumeObjectManagerMessage)
{
    unholdAll ();

    pPrismResumeObjectManagerMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pPrismResumeObjectManagerMessage);
}

PrismMessage *WaveObjectManager::createMessageInstance (const UI32 &operationCode)
{
    trace (TRACE_LEVEL_ERROR, "WaveObjectManager::createMessageInstance : NOT IMPLEMENTED.  RETURNS NULL BY DEFAULT.");
    return (NULL);
}

PrismMessage *WaveObjectManager::createMessageInstanceWrapper (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    m_createMessageInstanceWrapperMutex.lock ();

    map<UI32, PrismElement *>::iterator element    = m_ownersForCreatingMessageInstances.find (operationCode);
    map<UI32, PrismElement *>::iterator endElement = m_ownersForCreatingMessageInstances.end ();

    if (endElement != element)
    {
        pPrismMessage = (m_ownersForCreatingMessageInstances[operationCode])->createMessageInstance (operationCode);

        if (NULL == pPrismMessage)
        {
            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::createMessageInstanceWrapper : Owner for \"") + operationCode + string ("\" has not implemented dynamically creating the instance of this Message Type.  Implement this functionality to proceed further."));
        }
    }
    else
    {
        map<UI32, PrismElement *>::iterator element1    = m_ownersForCreatingMessageInstances.find (WAVE_OBJECT_MANAGER_ANY_OPCODE);
        map<UI32, PrismElement *>::iterator endElement1 = m_ownersForCreatingMessageInstances.end  ();

        if (endElement1 != element1)
        {
            pPrismMessage = (m_ownersForCreatingMessageInstances[WAVE_OBJECT_MANAGER_ANY_OPCODE])->createMessageInstance (operationCode);

            if (NULL == pPrismMessage)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::createMessageInstanceWrapper : Owner for \"") + operationCode + string ("\" via WAVE_OBJECT_MANAGER_ANY_OPCODE has not implemented dynamically creating the instance of this Message Type.  Implement this functionality to proceed further."));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::createMessageInstanceWrapper : There is no owner registered for \"") + operationCode + string ("\" or generic WAVE_OBJECT_MANAGER_ANY_OPCODE.  Please register a owner for this Managed Object Type to proceed further.  Aslo, please implement createManagedObjectInstance on the owner."));
        }
    }

    m_createMessageInstanceWrapperMutex.unlock ();

    return (pPrismMessage);
}

WaveManagedObject *WaveObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    trace (TRACE_LEVEL_ERROR, "WaveObjectManager::createManagedObjectInstance : NOT IMPLEMENTED.  RETURNS NULL BY DEFAULT.");
    return (NULL);
}

WaveManagedObject *WaveObjectManager::createManagedObjectInstanceWrapper (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    m_createManagedObjectInstanceWrapperMutex.lock ();

    map<string, PrismElement *>::iterator element    = m_ownersForCreatingManagedObjectInstances.find (managedClassName);
    map<string, PrismElement *>::iterator endElement = m_ownersForCreatingManagedObjectInstances.end ();

    if (endElement != element)
    {
        pWaveManagedObject = (m_ownersForCreatingManagedObjectInstances[managedClassName])->createManagedObjectInstance (managedClassName);

        if (NULL == pWaveManagedObject)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::createManagedObjectInstanceWrapper : Owner for \"" + managedClassName + "\" has not implemented dynamically creating the instance of this Managed Object Type.  Implement this functionality to proceed further.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::createManagedObjectInstanceWrapper : There is no owner registered for \"" + managedClassName + "\".  Please register a owner for this Managed Object Type to proceed further.  Aslo, please implement createManagedObjectInstance on the owner.");
    }

    m_createManagedObjectInstanceWrapperMutex.unlock ();

    return (pWaveManagedObject);
}

PrismEvent *WaveObjectManager::createEventInstance (const UI32 &eventOperationCode)
{
    trace (TRACE_LEVEL_ERROR, "WaveObjectManager::createEventInstance : NOT IMPLEMENTED.  RETURNS NULL BY DEFAULT.");
    return (NULL);
}

void WaveObjectManager::registerEventListenerHandler (WaveObjectManagerRegisterEventListenerMessage *pWaveObjectManagerRegisterEventListenerMessage)
{
    const UI32           operationCodeToListenFor = pWaveObjectManagerRegisterEventListenerMessage->getOperationCodeToListenFor ();
    const PrismServiceId listenerPrismServiceId   = pWaveObjectManagerRegisterEventListenerMessage->getListenerPrismServiceId ();
    const LocationId     listenerLocationId       = pWaveObjectManagerRegisterEventListenerMessage->getListenerLocationId ();

    addEventListener (operationCodeToListenFor, listenerPrismServiceId, listenerLocationId);

    pWaveObjectManagerRegisterEventListenerMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pWaveObjectManagerRegisterEventListenerMessage);
}

void WaveObjectManager::startTransaction ()
{
    if (true == m_isTransactionOn)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::startTransaction : Transaction is already in progress.");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_isTransactionOn = true;

    if (NULL != m_pInputMessage)
    {
        m_pInputMessage->setTransactionCounter ((m_pInputMessage->getTransactionCounter ()) + 1);
    }

}

bool WaveObjectManager::isTransactionInProgress ()
{
    return (m_isTransactionOn);
}

void WaveObjectManager::trackObjectCreatedDuringCurrentTransaction (WaveManagedObject *pWaveManagedObject)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported ((pWaveManagedObject->getObjectClassName ()))))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::trackObjectCreatedDuringCurrentTransaction : This Object Manager " + m_name + " does not support Managed Object of type " + pWaveManagedObject->getObjectClassName ());
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            m_objectsCreatedDuringCurrentTransaction.push_back (pWaveManagedObject);
        }
    }
}

void WaveObjectManager::trackObjectDeletedDuringCurrentTransaction (WaveManagedObject *pWaveManagedObject)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported ((pWaveManagedObject->getObjectClassName ()))))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::trackObjectDeletedDuringCurrentTransaction : This Object Manager " + m_name + " does not support Managed Object of type " + pWaveManagedObject->getObjectClassName ());
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            m_objectsDeletedIndividuallyDuringCurrentTransaction.push_back (pWaveManagedObject->getObjectId ());
            m_objectsDeletedDuringCurrentTransaction.push_back (new WaveManagedObjectDeleteSingle (pWaveManagedObject->getObjectId ()));
        }
    }
}

void WaveObjectManager::setupPartitionForMOsBeingCommitted (vector<WaveManagedObject *> *&partitionManagedObjectsForGarbageCollection)
{
    // 1. Add propagated partition name (if non-empty) to set of partition names referenced in current transaction.

    string propagatedPartitionName;

    if (NULL != m_pInputMessage)
    {
        propagatedPartitionName = m_pInputMessage->getPartitionName ();

        if (false == propagatedPartitionName.empty ())
        {
            addPartitionNameToSetOfPartitionNamesReferencedInCurrentTransaction (propagatedPartitionName);
        }
    }

    // 2. Get the set of partition names referenced in current transaction.

    set<string> setOfPartitionNamesReferencedInCurrentTransaction = getSetOfPartitionNamesReferencedInCurrentTransaction ();

    // 3. Get Object ids of corresponding WavePartitionManagedObject s, and store them in a map.

    map<string, ObjectId> mapOfPartitionNameToWavePartitionManagedObjectId;

    if (0 < setOfPartitionNamesReferencedInCurrentTransaction.size ())
    {
        // TODO : Following query can be removed later,  once -
        // clusterwide unique (distributed) (in-memory) cache of WavePartitionManagedObjects is available.

        WaveManagedObjectSynchronousQueryContext partitionQueryContext (WavePartitionManagedObject::getClassName());

        set<string>::iterator setItr = setOfPartitionNamesReferencedInCurrentTransaction.begin ();
        set<string>::iterator setEnd = setOfPartitionNamesReferencedInCurrentTransaction.end ();

        for (; setItr != setEnd; setItr++)
        {
            partitionQueryContext.addOrAttribute (new AttributeString (*(setItr), "partitionName"));
        }

        vector<WaveManagedObject *> *pPartitionResults = querySynchronously(&partitionQueryContext);

        if (NULL != pPartitionResults)
        {
            UI32                        numberOfResults             = pPartitionResults->size ();
            WavePartitionManagedObject *pWavePartitionManagedObject = NULL;

            for (UI32 i = 0; i < numberOfResults; i++)
            {
                pWavePartitionManagedObject = dynamic_cast<WavePartitionManagedObject *> ((*pPartitionResults)[i]);
                prismAssert (NULL != pWavePartitionManagedObject, __FILE__, __LINE__);

                mapOfPartitionNameToWavePartitionManagedObjectId[pWavePartitionManagedObject->getPartitionName ()] = pWavePartitionManagedObject->getObjectId ();
            }
        }

        // 3.. Store queried vector of WavePartitionManagedObject pointers in "partitionManagedObjectsForGarbageCollection"
        //     It will be deleted by caller function (i.e. commitTransaction), after closing current transaction.

        partitionManagedObjectsForGarbageCollection = pPartitionResults;
    }

    // 4. Reset the set of partition names referenced in current transaction.

    setOfPartitionNamesReferencedInCurrentTransaction.clear ();

    // 5. Setup Partition Object ids in created (new) Managed Objects.

    if (0 < mapOfPartitionNameToWavePartitionManagedObjectId.size ())
    {
        UI32 numberOfObjectsCreatedDuringCurrentTransaction = m_objectsCreatedDuringCurrentTransaction.size ();

        for (UI32 i = 0; i < numberOfObjectsCreatedDuringCurrentTransaction; i++)
        {
            WaveManagedObject *pWaveManagedObject = m_objectsCreatedDuringCurrentTransaction[i];

            string partitionName = "";

            if (true == pWaveManagedObject->getIsPartitionNameSetByUser ())
            {
                // 5.1 User has explicitly set a partition name.
                partitionName = pWaveManagedObject->getUserSpecifiedPartitionName ();
            }
            else
            {
                // 5.2 User has not set a partition name, so user propagated Partition name.
                partitionName = propagatedPartitionName;
            }

            map<string,ObjectId>::iterator itr      = mapOfPartitionNameToWavePartitionManagedObjectId.find (partitionName);
            map<string,ObjectId>::iterator endItr   = mapOfPartitionNameToWavePartitionManagedObjectId.end ();

            // 5.3 Note : if partitionName is empty, then, as expected, NullObjectId will get set.

            ObjectId ownerPartitionManagedObjectId;

            if (endItr != itr)
            {
                ownerPartitionManagedObjectId = itr->second;
            }

            pWaveManagedObject->setOwnerPartitionManagedObjectId (ownerPartitionManagedObjectId);
        }
    }
}

ResourceId WaveObjectManager::commitTransaction (WaveObjectManagerCommitTransactionContext *pWaveObjectManagerCommitTransactionContext)
{
    if (false == m_isTransactionOn)
    {
        trace (TRACE_LEVEL_FATAL , "WaveObjectManager::commitTransaction : There is no transaction in progress.");
        prismAssert (false, __FILE__, __LINE__);
        return (FRAMEWORK_ERROR);
    }

    UI32   numberOfManagedObjectDeleteAtTheBeginingOfTransaction    = m_managedObjectsDeletedAtTheBeginingOfTransaction.size ();
    UI32   numberOfObjectsCreated                                   = m_objectsCreatedDuringCurrentTransaction.size ();
    UI32   numberOfCompositionEntriesCreated                        = m_compositionEntriesCreatedDuringCurrentTrasnaction.size ();
    UI32   numberOfAssociationEntriesCreated                        = m_associationEntriesCreatedDuringCurrentTrasnaction.size ();
    UI32   numberOfRelationshipEntriesCreated                       = m_relationshipEntriesCreatedDuringCurrentTransaction.size ();
    UI32   numberOfObjectsUpdatedIndividually                       = m_objectsUpdatedIndividuallyDuringCurrentTransaction.size ();
    UI32   numberOfObjectsUpdated                                   = m_objectsUpdatedDuringCurrentTransaction.size ();
    UI32   numberOfObjectsDeletedIndividually                       = m_objectsDeletedIndividuallyDuringCurrentTransaction.size ();
    UI32   numberOfObjectsDeleted                                   = m_objectsDeletedDuringCurrentTransaction.size ();
    UI32   numberOfCompositionEntriesDeleted                        = m_compositionEntriesDeletedDuringCurrentTransaction.size ();
    UI32   numberOfAssociationEntriesDeleted                        = m_associationEntriesDeletedDuringCurrentTrasnaction.size ();
    UI32   numberOfRelationshipEntriesDeleted                       = m_relationshipEntriesDeletedDuringCurrentTransaction.size ();
    UI32   i                                                        = 0;
    string sql;
    string nestedSql                                                = "";
    UI32   numberOfConfigsAddedOrUpdated                            = 0;
    UI32   transactionCounter                                       = 0;
    bool   isADelayedCommitTransaction                              = false;
    ResourceId status;

    // multi-partition support.

    // Note : setupPartitionForMOsBeingCommitted queries few WavePartitionManagedObjects, and stores them in following vector.
    //        These Managed Objects are to be deleted after closing current transaction. (CLEANUP1)
    vector<WaveManagedObject *> *partitionManagedObjectsForGarbageCollection = NULL;

    setupPartitionForMOsBeingCommitted (partitionManagedObjectsForGarbageCollection);

    // First update ORM Relations on the managed Objects for all creates and updates.
    for (i = 0; i < numberOfManagedObjectDeleteAtTheBeginingOfTransaction; i++)
    {
        OrmRepository::getSqlToDeleteAllEntriesInTable (m_managedObjectsDeletedAtTheBeginingOfTransaction [i], sql);
    }

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        (m_objectsCreatedDuringCurrentTransaction[i])->updateOrmRelations ();
    }

    for (i = 0; i < numberOfObjectsUpdatedIndividually; i++)
    {
        (m_objectsUpdatedIndividuallyDuringCurrentTransaction[i])->updateOrmRelations ();
    }

    // Also set the keypath strings for the objects.
    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        (m_objectsCreatedDuringCurrentTransaction[i])->updateKeyString ();
    }

    for (i = 0; i < numberOfObjectsUpdatedIndividually; i++)
    {
        (m_objectsUpdatedIndividuallyDuringCurrentTransaction[i])->updateKeyString ();
    }

    // now proceed to generate SQL int he following order
    //     1.  Creates
    //     2.  Updates
    //     3.  Deletes

    for (i = 0; i < numberOfObjectsCreated; i++)
    {
        (m_objectsCreatedDuringCurrentTransaction[i])->getSqlForInsert (sql, OrmRepository::getWaveCurrentSchema ());

        UI32 createdManagedObjectTableId = ((m_objectsCreatedDuringCurrentTransaction[i])->getObjectId ()).getClassId ();

        m_pWaveObjectManagerStatisticsTracker->incrementManagedObjectsCreated (OrmRepository::getTableNameById (createdManagedObjectTableId));
    }

    for (i = 0; i < numberOfCompositionEntriesCreated; i++)
    {
        m_compositionEntriesCreatedDuringCurrentTrasnaction[i].getSqlForInsertIntoParent (sql);

        m_pWaveObjectManagerStatisticsTracker->incrementCompositionsCreated (m_compositionEntriesCreatedDuringCurrentTrasnaction[i].getCompositionTableName ());
    }

    for (i = 0; i < numberOfAssociationEntriesCreated; i++)
    {
        m_associationEntriesCreatedDuringCurrentTrasnaction[i].getSqlForInsertIntoParent (sql);

        m_pWaveObjectManagerStatisticsTracker->incrementAssociationsCreated (m_associationEntriesCreatedDuringCurrentTrasnaction[i].getAssociationTableName ());
    }

    for (i = 0; i < numberOfRelationshipEntriesCreated; i++)
    {
        m_relationshipEntriesCreatedDuringCurrentTransaction[i].getSqlForInsertIntoParent (sql);

        if (ORM_RELATION_TYPE_ONE_TO_ONE == m_relationshipEntriesCreatedDuringCurrentTransaction[i].getRelationType ())
        {
            m_pWaveObjectManagerStatisticsTracker->incrementManagedObjectsUpdated (m_relationshipEntriesCreatedDuringCurrentTransaction[i].getParentClassName ());
        }
        else
        {
            if ((ORM_RELATION_UML_TYPE_ASSOCIATION == m_relationshipEntriesCreatedDuringCurrentTransaction[i].getRelationUmlType ()) || (ORM_RELATION_UML_TYPE_AGGREGATION == m_relationshipEntriesCreatedDuringCurrentTransaction[i].getRelationUmlType ()))
            {
                m_pWaveObjectManagerStatisticsTracker->incrementAssociationsCreated (m_relationshipEntriesCreatedDuringCurrentTransaction[i].getRelationshipTableName ());
            }
            else
            {
                m_pWaveObjectManagerStatisticsTracker->incrementCompositionsCreated  (m_relationshipEntriesCreatedDuringCurrentTransaction[i].getRelationshipTableName ());
            }
        }
    }

    for (i = 0; i < numberOfObjectsUpdatedIndividually; i++)
    {

        UI32 updatedManagedObjectTableId = ((m_objectsUpdatedIndividuallyDuringCurrentTransaction[i])->getObjectId ()).getClassId ();

        m_pWaveObjectManagerStatisticsTracker->incrementManagedObjectsUpdated (OrmRepository::getTableNameById (updatedManagedObjectTableId));

        trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::commitTransaction : Updated : " + (m_objectsUpdatedIndividuallyDuringCurrentTransaction[i])->getObjectClassName () + " : " + (m_objectsUpdatedIndividuallyDuringCurrentTransaction[i])->getUserDefinedKeyCombinationValue ());
    }

    for (i = 0; i < numberOfObjectsUpdated; i++)
    {
        (m_objectsUpdatedDuringCurrentTransaction[i])->getSqlForUpdate (sql);
    }

    for (i = 0; i < numberOfObjectsDeletedIndividually; i++)
    {
        UI32 deletedManagedObjectTableId = (m_objectsDeletedIndividuallyDuringCurrentTransaction[i]).getClassId ();

        m_pWaveObjectManagerStatisticsTracker->incrementManagedObjectsDeleted (OrmRepository::getTableNameById (deletedManagedObjectTableId));
    }

    for (i = 0; i < numberOfObjectsDeleted; i++)
    {
        (m_objectsDeletedDuringCurrentTransaction[i])->getSqlForDelete (sql);
    }

    for (i = 0; i < numberOfCompositionEntriesDeleted; i++)
    {
        m_compositionEntriesDeletedDuringCurrentTransaction[i].getSqlForDeleteFromParent (sql);

        m_pWaveObjectManagerStatisticsTracker->incrementCompositionsDeleted (m_compositionEntriesDeletedDuringCurrentTransaction[i].getCompositionTableName ());
    }

    for (i = 0; i < numberOfAssociationEntriesDeleted; i++)
    {
        m_associationEntriesDeletedDuringCurrentTrasnaction[i].getSqlForDeleteFromParent (sql);

        m_pWaveObjectManagerStatisticsTracker->incrementAssociationsDeleted (m_associationEntriesDeletedDuringCurrentTrasnaction[i].getAssociationTableName ());
    }

    for (i = 0; i < numberOfRelationshipEntriesDeleted; i++)
    {
        m_relationshipEntriesDeletedDuringCurrentTransaction[i].getSqlForDeleteFromParent (sql);

        if (ORM_RELATION_TYPE_ONE_TO_ONE == m_relationshipEntriesDeletedDuringCurrentTransaction[i].getRelationType ())
        {
            m_pWaveObjectManagerStatisticsTracker->incrementManagedObjectsUpdated (m_relationshipEntriesDeletedDuringCurrentTransaction[i].getParentClassName ());
        }
        else
        {
            if ((ORM_RELATION_UML_TYPE_ASSOCIATION == m_relationshipEntriesDeletedDuringCurrentTransaction[i].getRelationUmlType ()) || (ORM_RELATION_UML_TYPE_AGGREGATION == m_relationshipEntriesDeletedDuringCurrentTransaction[i].getRelationUmlType ()))
            {
                m_pWaveObjectManagerStatisticsTracker->incrementAssociationsDeleted (m_relationshipEntriesDeletedDuringCurrentTransaction[i].getRelationshipTableName ());
            }
            else
            {
                m_pWaveObjectManagerStatisticsTracker->incrementCompositionsDeleted  (m_relationshipEntriesDeletedDuringCurrentTransaction[i].getRelationshipTableName ());
            }
        }
    }

    m_managedObjectsDeletedAtTheBeginingOfTransaction.clear ();
    m_objectsCreatedDuringCurrentTransaction.clear ();
    m_compositionEntriesCreatedDuringCurrentTrasnaction.clear ();
    m_associationEntriesCreatedDuringCurrentTrasnaction.clear ();
    m_relationshipEntriesCreatedDuringCurrentTransaction.clear ();
    m_objectsUpdatedIndividuallyDuringCurrentTransaction.clear ();

    for (i = 0; i < numberOfObjectsUpdated; i++)
    {
        delete m_objectsUpdatedDuringCurrentTransaction[i];
    }

    m_objectsUpdatedDuringCurrentTransaction.clear ();
    m_objectsDeletedIndividuallyDuringCurrentTransaction.clear ();

    for (i = 0; i < numberOfObjectsDeleted; i++)
    {
        delete m_objectsDeletedDuringCurrentTransaction[i];
    }

    m_objectsDeletedDuringCurrentTransaction.clear ();
    m_compositionEntriesDeletedDuringCurrentTransaction.clear ();
    m_associationEntriesDeletedDuringCurrentTrasnaction.clear ();
    m_relationshipEntriesDeletedDuringCurrentTransaction.clear ();

    if (NULL != m_pInputMessage)
    {
        transactionCounter = m_pInputMessage->m_transactionCounter;
        nestedSql = m_pInputMessage->m_nestedSql;
    }

    // We are appending the current sql with the already existing sql statement

    nestedSql += sql;
    numberOfConfigsAddedOrUpdated = m_configurationsCreatedDuringCurrentTransaction.size () + m_configurationsUpdatedDuringCurrentTransaction.size ();

    // Handle any commit transaction context flags.

    if (NULL != pWaveObjectManagerCommitTransactionContext)
    {
        isADelayedCommitTransaction = pWaveObjectManagerCommitTransactionContext->getIsADelayedCommitTransaction ();
    }

    if (transactionCounter > 1)
    {
        //tracePrintf (TRACE_LEVEL_DEBUG, false, false,  "WaveObjectManager::commitTransaction : This is a nested transaction and thus we should be copying the SQL statement in the input message context so that the sender plugin can do the composite SQL transaction ........ --- transactionCounter - %d .... NestedSql - %s", transactionCounter, nestedSql.c_str());

        if (true == isADelayedCommitTransaction)
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::commitTransaction : A delayed commit transaction should only be done at the start of a simple transaction or nested transaction, never in the middle of a nested transaction.");
            prismAssert (false, __FILE__, __LINE__);
        }

        status = FRAMEWORK_SUCCESS;
    }
    else
    {
        if (true == nestedSql.empty () && 0 == numberOfConfigsAddedOrUpdated)
        {
            m_isTransactionOn = false;

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (partitionManagedObjectsForGarbageCollection); // (CLEANUP1)

            trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::commitTransaction : The SQL is EMPTY");

            if (NULL != m_pInputMessage)
            {
                prismAssert (1 <= transactionCounter, __FILE__, __LINE__);
                transactionCounter--;
                m_pInputMessage->m_transactionCounter = transactionCounter;
                m_pInputMessage->m_nestedSql = nestedSql;
            }

            return (FRAMEWORK_SUCCESS);
        }

        if (true == isADelayedCommitTransaction)
        {
            // For delayed commit transactions, we will just queue up the sql transaction in Persistence OM.  Then the delayed commit transactions will be piggy-backed and committed along with any subsequently executed commit transaction.

            PersistenceObjectManagerAddDelayedTransactionMessage message (nestedSql);

            status = sendSynchronously (&message);

            nestedSql = "";

            if (status != WAVE_MESSAGE_SUCCESS)
            {
                trace (TRACE_LEVEL_ERROR, "WaveObjectManager::commitTransaction : Could not send message to Persistence Service.  Status : " + FrameworkToolKit::localize (status));
            }
            else
            {
                status = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "WaveObjectManager::commitTransaction : Failed to add the delayed transaction.  Completion Status : " + FrameworkToolKit::localize (status));
                }
                else
                {
                    status = FRAMEWORK_SUCCESS;
                }
            }
        }
        else
        {
            //If surrogate flag is set, reset the same inorder for the commitTransaction to happen
            //Will be reset once message is returned from persistence OM.

            bool isSurrogateFlag = false;

            if (m_pInputMessage)
            {
                isSurrogateFlag = m_pInputMessage->getIsMessageBeingSurrogatedFlag ();

                m_pInputMessage->setIsMessageBeingSurrogatedFlag (false);
            }

            vector<string>     configNamesForInsert;
            vector<string>     configValuesForInsert;
            vector<ResourceId> configTypesForInsert;
            WaveConfigurationAttributes::getConfigurationVectors (m_configurationsCreatedDuringCurrentTransaction, configNamesForInsert, configValuesForInsert, configTypesForInsert);

            vector<string>     configNamesForUpdate;
            vector<string>     configValuesForUpdate;
            vector<ResourceId> configTypesForUpdate;
            WaveConfigurationAttributes::getConfigurationVectors (m_configurationsUpdatedDuringCurrentTransaction, configNamesForUpdate, configValuesForUpdate, configTypesForUpdate);

            PersistenceObjectManagerExecuteTransactionMessage message (nestedSql);
            message.setConfigNamesForInsert  (configNamesForInsert);
            message.setConfigValuesForInsert (configValuesForInsert);
            message.setConfigTypesForInsert  (configTypesForInsert);
            message.setConfigNamesForUpdate  (configNamesForUpdate);
            message.setConfigValuesForUpdate (configValuesForUpdate);
            message.setConfigTypesForUpdate  (configTypesForUpdate);

            status = sendSynchronously (&message);

            if (m_pInputMessage)
            {
                m_pInputMessage->setIsMessageBeingSurrogatedFlag (isSurrogateFlag);
            }

            nestedSql = "";

            if (status != WAVE_MESSAGE_SUCCESS)
            {
                trace (TRACE_LEVEL_ERROR, "WaveObjectManager::commitTransaction : Could not send message to Persistence Service.  Status : " + FrameworkToolKit::localize (status));
            }
            else
            {
                status = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "WaveObjectManager::commitTransaction : Failed to execute the transaction.  Completion Status : " + FrameworkToolKit::localize (status));
                }
                else
                {
                    status = FRAMEWORK_SUCCESS;
                }
            }
        }
    }

    m_configurationsCreatedDuringCurrentTransaction.clear ();
    m_configurationsUpdatedDuringCurrentTransaction.clear ();

    m_isTransactionOn = false;

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (partitionManagedObjectsForGarbageCollection); // (CLEANUP1)

    if (NULL != m_pInputMessage)
    {
        prismAssert (1 <= transactionCounter, __FILE__, __LINE__);
        transactionCounter--;
        m_pInputMessage->m_transactionCounter = transactionCounter;
        m_pInputMessage->m_nestedSql = nestedSql;
    }

    return (status);
}

void WaveObjectManager::rollbackTransaction ()
{
    UI32   numberOfObjectsUpdated   = m_objectsUpdatedDuringCurrentTransaction.size ();
    UI32   numberOfObjectsDeleted   = m_objectsDeletedDuringCurrentTransaction.size ();
    UI32   i                        = 0;

    if (false == m_isTransactionOn)
    {
        trace (TRACE_LEVEL_FATAL , "WaveObjectManager::rollbackTransaction : There is no transaction in progress.");
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_managedObjectsDeletedAtTheBeginingOfTransaction.clear ();
    m_objectsCreatedDuringCurrentTransaction.clear ();
    m_compositionEntriesCreatedDuringCurrentTrasnaction.clear ();
    m_associationEntriesCreatedDuringCurrentTrasnaction.clear ();
    m_relationshipEntriesCreatedDuringCurrentTransaction.clear ();
    m_objectsUpdatedIndividuallyDuringCurrentTransaction.clear ();
    for (i = 0; i < numberOfObjectsUpdated; i++)
    {
        delete m_objectsUpdatedDuringCurrentTransaction[i];
    }
    m_objectsUpdatedDuringCurrentTransaction.clear ();
    m_objectsDeletedIndividuallyDuringCurrentTransaction.clear ();
    for (i = 0; i < numberOfObjectsDeleted; i++)
    {
        delete m_objectsDeletedDuringCurrentTransaction[i];
    }
    m_objectsDeletedDuringCurrentTransaction.clear ();
    m_compositionEntriesDeletedDuringCurrentTransaction.clear ();
    m_associationEntriesDeletedDuringCurrentTrasnaction.clear ();
    m_relationshipEntriesDeletedDuringCurrentTransaction.clear ();

    m_isTransactionOn = false;
}

void WaveObjectManager::addWaveConfigEntry (Attribute *attribute)
{
    if (NULL == attribute)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addWaveConfigEntry : attribute cannot be null");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == isALocalPrismService ())
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addWaveConfigEntry : Only Global OM can call this API to update global WaveConfigManagedObject");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == m_isTransactionOn)
    {
        string configValue;
        attribute->getPlainString (configValue);
        ResourceId configAttributeType = (attribute->getAttributeType ()).getAttributeTypeResourceId ();

        WaveConfigurationAttributes config (attribute->getAttributeName (), configValue, configAttributeType);
        m_configurationsCreatedDuringCurrentTransaction.push_back (config);
    }
}

void WaveObjectManager::updateWaveConfigEntry (Attribute *attribute)
{
    if (NULL == attribute)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateWaveConfigEntry : attribute cannot be null");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == isALocalPrismService ())
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateWaveConfigEntry : Only Global OM can call this API to update global WaveConfigManagedObject");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == m_isTransactionOn)
    {
        string configValue;
        attribute->getPlainString (configValue);
        ResourceId configAttributeType = (attribute->getAttributeType ()).getAttributeTypeResourceId ();

        WaveConfigurationAttributes config (attribute->getAttributeName (), configValue, configAttributeType);
        m_configurationsUpdatedDuringCurrentTransaction.push_back (config);
    }
}

void WaveObjectManager::getWaveConfigEntry (string &configName, Attribute *attribute, bool &foundConfig)
{
    foundConfig = false;

    if (NULL == attribute)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::getWaveConfigEntry : attribute cannot be null");
        prismAssert (false, __FILE__, __LINE__);
    }

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveConfigManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeString (configName, "configName"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (1 < numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::getWaveConfigEntry: Querying " + WaveConfigManagedObject::getClassName () + " resulted in more than one managed object.  This should never happen.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (1 == numberOfResults)
        {
            // Check if the Attribute type in DB and the attribut type expecting in the argument is same

            ResourceId configAttributeType = (attribute->getAttributeType ()).getAttributeTypeResourceId ();
            WaveConfigManagedObject *pWaveConfigManagedObject = dynamic_cast<WaveConfigManagedObject *> ((*pResults)[0]);
            ResourceId configAttributeTypeDB= pWaveConfigManagedObject->getConfigAttributeType ();

            if (configAttributeType != configAttributeTypeDB)
            {
                trace (TRACE_LEVEL_FATAL, "WaveObjectManager::getWaveConfigEntry : Mismatch o Config Attribute type in DB and the argument.");
                prismAssert (false, __FILE__, __LINE__);
            }

            string configValue = pWaveConfigManagedObject->getConfigValue ();
            attribute->fromString (configValue);
            foundConfig = true;
            delete (pWaveConfigManagedObject);
            delete pResults;
        }
        else
        {
            foundConfig = false;
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::getWaveConfigEntry: Null obtatined while Querying " + WaveConfigManagedObject::getClassName () + ".  This should never happen.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveObjectManager::updateWaveManagedObject (WaveManagedObject *pWaveManagedObject)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported ((pWaveManagedObject->getObjectClassName ()))))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateWaveManagedObject : This Object Manager " + m_name + " does not support Managed Object of type " + pWaveManagedObject->getObjectClassName ());
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (true == OrmRepository::isManagedClassAView ((pWaveManagedObject->getObjectClassName ())))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateWaveManagedObject : Managed View " + pWaveManagedObject->getObjectClassName () + "is not allowed in a transaction.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            pWaveManagedObject->setLastModifiedTimeStamp (DateTime ());
            m_objectsUpdatedIndividuallyDuringCurrentTransaction.push_back (pWaveManagedObject);
            m_objectsUpdatedDuringCurrentTransaction.push_back (new WaveManagedObjectUpdateSingle (pWaveManagedObject));
        }
    }
}

void WaveObjectManager::updateMultipleWaveManagedObjects (WaveManagedObjectSynchronousQueryContextForUpdate *pWaveManagedObjectSynchronousQueryContextForUpdate)
{
    if (NULL == pWaveManagedObjectSynchronousQueryContextForUpdate)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateMultipleWaveManagedObjects : The update context is NULL.");
        prismAssert (false, __FILE__, __LINE__);
    }

    WaveManagedObject* pWaveManagedObject = pWaveManagedObjectSynchronousQueryContextForUpdate->getWaveManagedObjectToAddUpdateAttributes();

    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported ((pWaveManagedObject->getObjectClassName ()))))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateMultipleWaveManagedObjects : This Object Manager " + m_name + " does not support Managed Object of type " + pWaveManagedObject->getObjectClassName ());
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (true == OrmRepository::isManagedClassAView ((pWaveManagedObject->getObjectClassName ())))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateMultipleWaveManagedObjects : Managed View " + pWaveManagedObject->getObjectClassName () + "is not allowed in a transaction.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            pWaveManagedObject->setLastModifiedTimeStamp (DateTime ());
            m_objectsUpdatedDuringCurrentTransaction.push_back (new WaveManagedObjectUpdateMultiple (pWaveManagedObjectSynchronousQueryContextForUpdate));
        }
    }
}

void WaveObjectManager::deleteWaveManagedObject (const ObjectId &objectId)
{
    //trace (TRACE_LEVEL_DEVEL, string ("WaveObjectManager::deleteWaveManagedObject : Entering with ObjectId : ") + objectId.toString () +  " ...");

    if (ObjectId::NullObjectId == objectId)
    {
        return;
    }

    if (true == m_isTransactionOn)
    {
        string managedObjectClassName = OrmRepository::getTableNameById (objectId.getClassId ());

        if (false == (isManagedClassSupported (managedObjectClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : This Object Manager " + m_name + " does not support Managed Object of type " + managedObjectClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (true == OrmRepository::isManagedClassAView (managedObjectClassName))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : Managed View " + managedObjectClassName + "is not allowed in a transaction.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            vector<string>  compositionFieldNames;
            UI32            numberOfCompositionFieldNames   = 0;

            // Get all composition field names for this class

            OrmTable *pTable = OrmRepository::getTableByName (managedObjectClassName);
            prismAssert (NULL != pTable, __FILE__, __LINE__);

            pTable->getCompositionFieldNamesInHierarchy (compositionFieldNames);

            numberOfCompositionFieldNames = compositionFieldNames.size ();

            if (0 < numberOfCompositionFieldNames)
            {
                // Query for the objectId but only load the composition field names

                WaveManagedObjectSynchronousQueryContext synchronousQueryContext (managedObjectClassName);

                synchronousQueryContext.addAndAttribute (new AttributeObjectId (objectId, "objectId"));
                synchronousQueryContext.addSelectFields (compositionFieldNames);

                vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

                if (NULL != pResults)
                {
                    UI32 numberOfResults = pResults->size ();

                    if (1 < numberOfResults)
                    {
                        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : Querying for an objectId of " + managedObjectClassName + " resulted in more than one managed object.  This should never happen.");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else if (1 == numberOfResults)
                    {
                        // We will mark this managed object for deletion which is partially loaded for all compositions in the inheritance hierarchy for this managed object.

                        delete ((*pResults)[0]);
                    }
                    else
                    {
                        // Nothing to delete
                    }

                    delete pResults;
                }
            }
            else
            {
                m_objectsDeletedIndividuallyDuringCurrentTransaction.push_back (objectId);
                m_objectsDeletedDuringCurrentTransaction.push_back (new WaveManagedObjectDeleteSingle (objectId));
            }
        }
    }
}

void WaveObjectManager::deleteWaveManagedObject (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion)
{
    //trace (TRACE_LEVEL_DEVEL, string ("WaveObjectManager::deleteWaveManagedObject : Entering ..."));

    if (NULL == pWaveManagedObjectSynchronousQueryContextForDeletion)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : The deletion context is NULL.");
        prismAssert (false, __FILE__, __LINE__);
    }

    string managedObjectClassName = pWaveManagedObjectSynchronousQueryContextForDeletion->getClassToQueryFor ();

    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (managedObjectClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : This Object Manager " + m_name + " does not support Managed Object of type " + managedObjectClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (true == OrmRepository::isManagedClassAView (managedObjectClassName))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : Managed View " + managedObjectClassName + "is not allowed in a transaction.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            vector<string>  compositionFieldNames;

            // Get all composition field names for this class

            OrmTable *pTable = OrmRepository::getTableByName (managedObjectClassName);
            prismAssert (NULL != pTable, __FILE__, __LINE__);

            pTable->getCompositionFieldNamesInHierarchy (compositionFieldNames);


            // Query for managed objects with given condition(s) plus load only the composition field names in the derivation hierarcy

            pWaveManagedObjectSynchronousQueryContextForDeletion->addSelectFields (compositionFieldNames);

            vector<WaveManagedObject *> *pResults = querySynchronously (pWaveManagedObjectSynchronousQueryContextForDeletion);

            if (NULL != pResults)
            {
                UI32 numberOfResults = pResults->size ();

                for (UI32 i = 0; i < numberOfResults; i++)
                {
                    delete (*pResults)[i];
                }

                delete pResults;
            }
        }
    }
}

/*
 * This API Functionality remains same as  deleteWaveManagedObject (WaveManagedObjectSynchronousQueryContextForDeletion *)
 * But, this API will delete all the objects pertaining to Query Context without loading those objects in Memory.
 *
 * NOTE: The pointer pWaveManagedObjectSynchronousQueryContextForDeletion should be pointing to heap memory and hence allocate the memory from heap using new operator.
 * This memory will be freed in either rollbackTransaction or commitTransaction.
 */
void WaveObjectManager::deleteMultipleWaveManagedObjects (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::deleteMultipleWaveManagedObjects: Entering ... ");

    if (NULL == pWaveManagedObjectSynchronousQueryContextForDeletion)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteMultipleWaveManagedObjects: The deletion context is NULL.");
        prismAssert (false, __FILE__, __LINE__);
    }

    string managedObjectClassName = pWaveManagedObjectSynchronousQueryContextForDeletion->getClassToQueryFor ();

    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (managedObjectClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : This Object Manager " + m_name + " does not support Managed Object of type " + managedObjectClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (true == OrmRepository::isManagedClassAView (managedObjectClassName))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteWaveManagedObject : Managed View " + managedObjectClassName + "is not allowed in a transaction.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            m_objectsDeletedDuringCurrentTransaction.push_back (new WaveManagedObjectDeleteMultiple (pWaveManagedObjectSynchronousQueryContextForDeletion));
        }
    }
}

void WaveObjectManager::deleteManagedObjectCompositions (
        WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion,
        const set<string> relationsSet)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::deleteManagedObjectCompositions: Entering ... ");

    if (NULL == pWaveManagedObjectSynchronousQueryContextForDeletion)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteManagedObjectCompositions: The deletion context is NULL.");
        prismAssert (false, __FILE__, __LINE__);
    }

    string managedObjectClassName = pWaveManagedObjectSynchronousQueryContextForDeletion->getClassToQueryFor ();

    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (managedObjectClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteManagedObjectCompositions : This Object Manager " + m_name + " does not support Managed Object of type " + managedObjectClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            WaveManagedObjectDeleteMultiple* waveMoDeleteMultipe = new WaveManagedObjectDeleteMultiple (pWaveManagedObjectSynchronousQueryContextForDeletion, true, relationsSet);
            m_objectsDeletedDuringCurrentTransaction.push_back (waveMoDeleteMultipe);
        }
    }
}

void WaveObjectManager::deleteManagedObjectCompositions (const ObjectId &objectId, const set<string> relationsSet)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::deleteManagedObjectCompositions (objectId, ...): Entering ... ");

    WaveManagedObjectSynchronousQueryContextForDeletion *queryContextForDeletion  = new WaveManagedObjectSynchronousQueryContextForDeletion(OrmRepository::getTableNameById (objectId.getClassId ()));

    queryContextForDeletion->addObjectId(objectId);

    deleteManagedObjectCompositions(queryContextForDeletion, relationsSet);
}

/*
 * This API should be used when you need to delete few entries from the vector composition and keep the parent MO as is.
 *
 * This will not delete the parent MO, but will delete only the composed child elements based on the query context given.
 * So Query context should be on the child MO.
 * Technical Details: The entries from the auxiliaxy table(Where vector composition details are stored) will also be removed.
 */
void WaveObjectManager::deletePartialManagedObjectCompositions (const string &parentClassName, const string &compositionName,
        WaveManagedObjectSynchronousQueryContextForDeletion *pQueryContextForDeletionOnChildMO)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::deletePartialManagedObjectCompositions: Entering ... ");

    if (NULL == pQueryContextForDeletionOnChildMO)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteManagedObjectCompositions: The deletion context is NULL.");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (parentClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteManagedObjectCompositions : This Object Manager " + m_name + " does not support Managed Object of type " + parentClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            bool partialDeleteFromVectorComposition = true;
            WaveManagedObjectDeleteMultiple* waveMoDeleteMultiple = new WaveManagedObjectDeleteMultiple (pQueryContextForDeletionOnChildMO, partialDeleteFromVectorComposition, parentClassName, compositionName);
            m_objectsDeletedDuringCurrentTransaction.push_back (waveMoDeleteMultiple);
        }
    }
}
void WaveObjectManager::addRelationship (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (parentClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addRelationship : This Object Manager " + m_name + " does not support Managed Object of type " + parentClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            RelationshipEntry relationshipEntry (parentClassName, childClassName, relationshipName, parentObjectId, childObjectId);

            m_relationshipEntriesCreatedDuringCurrentTransaction.push_back (relationshipEntry);
        }
    }
}

void WaveObjectManager::deleteRelationship (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (parentClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteRelationship : This Object Manager " + m_name + " does not support Managed Object of type " + parentClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            RelationshipEntry relationshipEntry (parentClassName, childClassName, relationshipName, parentObjectId, childObjectId);

            m_relationshipEntriesDeletedDuringCurrentTransaction.push_back (relationshipEntry);
        }
    }
}
void WaveObjectManager::addToComposition (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (OrmRepository::getTableNameById (parentObjectId.getClassId ()))))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addToComposition : This Object Manager " + m_name + " does not support Managed Object of type " + parentClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            CompositionEntry compositionEntry (parentClassName, childCalssName, compositionName, parentObjectId, childObjectId);

            m_compositionEntriesCreatedDuringCurrentTrasnaction.push_back (compositionEntry);
        }
    }
}

void WaveObjectManager::deleteFromComposition (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (OrmRepository::getTableNameById (parentObjectId.getClassId ()))))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteFromComposition : This Object Manager " + m_name + " does not support Managed Object of type " + parentClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            CompositionEntry compositionEntry (parentClassName, childCalssName, compositionName, parentObjectId, childObjectId);

            m_compositionEntriesDeletedDuringCurrentTransaction.push_back (compositionEntry);

            deleteWaveManagedObject (childObjectId);
        }
    }
}

void WaveObjectManager::addToAssociation (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (parentClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::addToAssociation : This Object Manager " + m_name + " does not support Managed Object of type " + parentClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            AssociationEntry associationEntry (parentClassName, childCalssName, associationName, parentObjectId, childObjectId);

            m_associationEntriesCreatedDuringCurrentTrasnaction.push_back (associationEntry);
        }
    }
}

void WaveObjectManager::deleteFromAssociation (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (parentClassName)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteFromAssociation : This Object Manager " + m_name + " does not support Managed Object of type " + parentClassName);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            AssociationEntry associationEntry (parentClassName, childCalssName, associationName, parentObjectId, childObjectId);

            m_associationEntriesDeletedDuringCurrentTrasnaction.push_back (associationEntry);
        }
    }
}

void WaveObjectManager::applyPartitionFiltersWithPropagatedPartitionContext (WaveManagedObjectQueryContextBase *pWaveManagedObjectQueryContextBase)
{
    if (NULL == m_pInputMessage)
    {
        return;
    }

    if (false == m_pInputMessage->getIsPartitionContextPropagated ())
    {
        return;
    }

    string      partitionName       = m_pInputMessage->getPartitionName ();
    LocationId  partitionLocationId = m_pInputMessage->getPartitionLocationIdForPropagation ();

    pWaveManagedObjectQueryContextBase->setPartitionFilter (partitionName, partitionLocationId);
}

void WaveObjectManager::query (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    applyPartitionFiltersWithPropagatedPartitionContext (pWaveManagedObjectQueryContext);

    string sqlString = pWaveManagedObjectQueryContext->getSqlWithPagingEnabled (this);

    DatabaseObjectManagerExecuteQueryMessage *pDatabaseObjectManagerExecuteQueryMessage = new DatabaseObjectManagerExecuteQueryMessage (pWaveManagedObjectQueryContext->getClassToQueryFor (), this, sqlString, pWaveManagedObjectQueryContext->getSchemaToQuery ());

    pDatabaseObjectManagerExecuteQueryMessage->setSelectFieldsInManagedObject (pWaveManagedObjectQueryContext->getSelectFieldsInManagedObject ());
    pDatabaseObjectManagerExecuteQueryMessage->setSelectFields                (pWaveManagedObjectQueryContext->getSelectFields                ());
    pDatabaseObjectManagerExecuteQueryMessage->setLoadOneToManyRelationships  (pWaveManagedObjectQueryContext->getLoadOneToManyRelationships  ());
    pDatabaseObjectManagerExecuteQueryMessage->setLoadCompositions            (pWaveManagedObjectQueryContext->getLoadCompositions            ());

    WaveMessageStatus status = send (pDatabaseObjectManagerExecuteQueryMessage, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::queryCallback), pWaveManagedObjectQueryContext);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::query : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

        pWaveManagedObjectQueryContext->setCompletionStatus (status);
        pWaveManagedObjectQueryContext->callback ();
        return;
    }
    else
    {
        // Wait for callback to arrive and then process.  Nothing to do here.

        return;
    }
}

void WaveObjectManager::queryCallback (FrameworkStatus frameworkStatus, DatabaseObjectManagerExecuteQueryMessage *pDatabaseObjectManagerExecuteQueryMessage, void *pContext)
{
    WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext = reinterpret_cast<WaveManagedObjectQueryContext *> (pContext);
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::queryCallback : Framework error : status : " + FrameworkToolKit::localize (frameworkStatus));
    }
    else
    {
        status = pDatabaseObjectManagerExecuteQueryMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::queryCallback : The message was erroed out : Completion Status : "+ FrameworkToolKit::localize (status));
        }
        else
        {
            pWaveManagedObjectQueryContext->moveOffsetOnSuccessfulQuery ();

            pWaveManagedObjectQueryContext->setPResults (pDatabaseObjectManagerExecuteQueryMessage->getPResults ());

            // These are not added to bt tracker in constructor of the objects, since they are created in DB OM thread. Adding them to btTracker now.
            ObjectTracker::addQueriedObjectsToBtTracker (pDatabaseObjectManagerExecuteQueryMessage->getPResults ());
        }

        delete pDatabaseObjectManagerExecuteQueryMessage;
    }

    pWaveManagedObjectQueryContext->setCompletionStatus (status);
    pWaveManagedObjectQueryContext->callback ();
    return;
}

vector<WaveManagedObject *> *WaveObjectManager::querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className)
{
    return (querySynchronouslyLocalManagedObjectsForLocationId (locationId, className, vector<string> ()));
}

vector<WaveManagedObject *> *WaveObjectManager::querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className, const vector<string> &selectFields)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContextForManagedObjects (className);

    synchronousQueryContextForManagedObjects.addSelectFields (selectFields);

    return (querySynchronouslyLocalManagedObjectsForLocationId (locationId, &synchronousQueryContextForManagedObjects));
}

vector<WaveManagedObject *> *WaveObjectManager::querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext)
{
    trace (TRACE_LEVEL_DEBUG, "WaveObjectManager:: querySynchronouslyiManagedObjectsForLocationId : Entering ...");
    vector<WaveManagedObject *> *pResultsManagedObjects = NULL;

    ObjectId objectId = FrameworkToolKit::getObjectIdForLocationId (locationId);

    if (ObjectId::NullObjectId == objectId)
    {
        trace (TRACE_LEVEL_DEBUG, "WaveObjectManager:: querySynchronouslyiManagedObjectsForLocationId no object Id found for this location id");
        pResultsManagedObjects = new vector<WaveManagedObject *>;
    }
    else
    {
        pWaveManagedObjectSynchronousQueryContext->addAndAttribute (new AttributeObjectId (objectId, "ownerWaveNodeObjectId"));
        pResultsManagedObjects = querySynchronously (pWaveManagedObjectSynchronousQueryContext);
    }

    return (pResultsManagedObjects);
}

ResourceId WaveObjectManager::querySynchronouslyForCount (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, UI32 &count)
{
    WaveManagedObjectSynchronousInnerQueryContext *pInnerQueryContext = dynamic_cast<WaveManagedObjectSynchronousInnerQueryContext *> (pWaveManagedObjectSynchronousQueryContext);

    if (NULL != pInnerQueryContext)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::querySynchronouslyForCount : WaveManagedObjectSynchronousInnerQueryContext can't be used for querySynchronouslyForCount");

        prismAssert (false, __FILE__, __LINE__);
    }

    WaveManagedObjectSynchronousQueryContextForSetOperation *pSetContext = dynamic_cast<WaveManagedObjectSynchronousQueryContextForSetOperation *> (pWaveManagedObjectSynchronousQueryContext);

    if (NULL != pSetContext)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::querySynchronouslyForCount : WaveManagedObjectSynchronousQueryContextForSetOperation can't be used for querySynchronouslyForCount");

        prismAssert (false, __FILE__, __LINE__);
    }

    string sqlStringForCount = pWaveManagedObjectSynchronousQueryContext->getSql (true);

    DatabaseObjectManagerExecuteQueryMessageForCount    databaseObjectManagerExecuteQueryMessageForCount (pWaveManagedObjectSynchronousQueryContext->getClassToQueryFor (), this, sqlStringForCount);
    ResourceId                                          completionStatus                                 = WAVE_MESSAGE_SUCCESS;

    WaveMessageStatus status = sendSynchronously (&databaseObjectManagerExecuteQueryMessageForCount);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronouslyForCount : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));
        return (status);
    }
    else
    {
        completionStatus = databaseObjectManagerExecuteQueryMessageForCount.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronouslyForCount : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));
            return (completionStatus);
        }
        else
        {
            count = databaseObjectManagerExecuteQueryMessageForCount.getCount ();
            return (status);
        }
    }
}

ResourceId WaveObjectManager::querySynchronouslyForCount (const string &managedClassName, UI32 &count, const string &schema)
{
    WaveManagedObjectSynchronousQueryContext prismManagedObjectSynchronousQueryContext (managedClassName);
    prismManagedObjectSynchronousQueryContext.setSchemaToQuery (schema);

    return (querySynchronouslyForCount (&prismManagedObjectSynchronousQueryContext, count));
}

ResourceId WaveObjectManager::querySynchronouslyForCount (const string &managedClassName, const string &fieldName, const string &range, UI32 &count, const string &schema)
{
    if (false == OrmRepository::isFieldOfIntegerType (managedClassName, fieldName))
    {
        return (WAVE_MESSAGE_ERROR_COUNT_QUERY_NOT_SUPPORTED_ON_NON_INTEGER_RANGE);
    }

    WaveManagedObjectSynchronousQueryContext prismManagedObjectSynchronousQueryContext (managedClassName);
    prismManagedObjectSynchronousQueryContext.setSchemaToQuery (schema);
    prismManagedObjectSynchronousQueryContext.setAttributeRangeForQuery (new AttributeUI32Range (range, fieldName));

    return (querySynchronouslyForCount (&prismManagedObjectSynchronousQueryContext, count));
}

vector<WaveManagedObject *> *WaveObjectManager::querySynchronously (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext)
{
    WaveManagedObjectSynchronousInnerQueryContext *pInnerQueryContext = dynamic_cast<WaveManagedObjectSynchronousInnerQueryContext *> (pWaveManagedObjectSynchronousQueryContext);

    if (NULL != pInnerQueryContext)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::querySynchronously : WaveManagedObjectSynchronousInnerQueryContext can't be used for querySynchronouslyForCount");

        prismAssert (false, __FILE__, __LINE__);
    }

    WaveManagedObjectSynchronousQueryContextForSetOperation *pSetContext = dynamic_cast<WaveManagedObjectSynchronousQueryContextForSetOperation *> (pWaveManagedObjectSynchronousQueryContext);

    if (NULL != pSetContext)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::querySynchronously : WaveManagedObjectSynchronousInnerQueryContext can't be used for querySynchronouslyForCount");

        prismAssert (false, __FILE__, __LINE__);
    }

    bool isManagedView = OrmRepository::isManagedClassAView (pWaveManagedObjectSynchronousQueryContext->getClassToQueryFor ());

    applyPartitionFiltersWithPropagatedPartitionContext (pWaveManagedObjectSynchronousQueryContext);
    if (isManagedView)
    {
        bool loadRelations = false;
        pWaveManagedObjectSynchronousQueryContext->setLoadCompositions (loadRelations);
        pWaveManagedObjectSynchronousQueryContext->setLoadOneToManyRelationships (loadRelations);
    }

    string sqlString = pWaveManagedObjectSynchronousQueryContext->getSqlWithPagingEnabled(this);

    //trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::querySynchronously sql = :" +  sqlString);

    DatabaseObjectManagerExecuteQueryMessage  databaseObjectManagerExecuteQueryMessage  (pWaveManagedObjectSynchronousQueryContext->getClassToQueryFor (), this, sqlString, pWaveManagedObjectSynchronousQueryContext->getSchemaToQuery ());
    ResourceId                                completionStatus                          = WAVE_MESSAGE_SUCCESS;

    databaseObjectManagerExecuteQueryMessage.setSelectFieldsInManagedObject (pWaveManagedObjectSynchronousQueryContext->getSelectFieldsInManagedObject ());
    databaseObjectManagerExecuteQueryMessage.setSelectFields                (pWaveManagedObjectSynchronousQueryContext->getSelectFields                ());
    databaseObjectManagerExecuteQueryMessage.setLoadOneToManyRelationships  (pWaveManagedObjectSynchronousQueryContext->getLoadOneToManyRelationships  ());
    databaseObjectManagerExecuteQueryMessage.setLoadCompositions            (pWaveManagedObjectSynchronousQueryContext->getLoadCompositions            ());

    WaveMessageStatus status = sendSynchronously (&databaseObjectManagerExecuteQueryMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronously : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

        return (NULL);
    }
    else
    {
        completionStatus = databaseObjectManagerExecuteQueryMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronously : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));

            return (NULL);
        }
        else
        {
            pWaveManagedObjectSynchronousQueryContext->moveOffsetOnSuccessfulQuery ();

            vector<WaveManagedObject *> *pResults = databaseObjectManagerExecuteQueryMessage.getPResults ();

            // These are not added to bt tracker in constructor of the objects, since they are created in DB OM thread. Adding them to btTracker now.
            ObjectTracker::addQueriedObjectsToBtTracker (pResults);

            // Allow NULL results to be passed to the application.

            return (pResults);
        }
    }
}

ResourceId WaveObjectManager::postbootValidate (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase)
{
    /*  Override this functin to add additional Postboot Validation
        return WAVE_MESSAGE_SUCCESS on success
        return WAVE_MESSAGE_SKIP_POSTBOOT on failure of validation but proceeding to next service
        return WAVE_MESSAGE_ERROR to suspend postboot */
    return (WAVE_MESSAGE_SUCCESS);
}

WaveManagedObject *WaveObjectManager::createInMemoryManagedObject (const  string &className)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    DatabaseObjectManagerCreateInMemoryManagedObjectMessage *pDatabaseObjectManagerCreateInMemoryManagedObjectMessage = new DatabaseObjectManagerCreateInMemoryManagedObjectMessage (className, this);

    WaveMessageStatus status = sendSynchronously (pDatabaseObjectManagerCreateInMemoryManagedObjectMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::createInMemoryManagedObject : Message not send to Database OM. Status " + FrameworkToolKit::localize (status));
    }
    else
    {
        trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::createInMemoryManagedObject : Mesage send successfully. Status " + FrameworkToolKit::localize (status));
        pWaveManagedObject = pDatabaseObjectManagerCreateInMemoryManagedObjectMessage->getInMemoryMO();
        pWaveManagedObject->prepareForSerialization ();
    }

    delete (pDatabaseObjectManagerCreateInMemoryManagedObjectMessage);

    return (pWaveManagedObject);
}


vector<WaveManagedObject *> *WaveObjectManager::querySynchronously (const string &managedClassName, const string &schema)
{
    bool isManagedView = OrmRepository::isManagedClassAView (managedClassName);

    WaveManagedObjectSynchronousQueryContext prismManagedObjectSynchronousQueryContext (managedClassName);

    prismManagedObjectSynchronousQueryContext.setSchemaToQuery (schema);
    if (isManagedView)
    {
        bool loadRelations = false;
        prismManagedObjectSynchronousQueryContext.setLoadCompositions (loadRelations);
        prismManagedObjectSynchronousQueryContext.setLoadOneToManyRelationships (loadRelations);
    }
    else
    {
        prismManagedObjectSynchronousQueryContext.addOrderField ("name");
    }

    applyPartitionFiltersWithPropagatedPartitionContext (&prismManagedObjectSynchronousQueryContext);

    DatabaseObjectManagerExecuteQueryMessage databaseObjectManagerExecuteQueryMessage  (managedClassName, this, prismManagedObjectSynchronousQueryContext.getSql (), schema);
    ResourceId                               completionStatus                          = WAVE_MESSAGE_SUCCESS;

    databaseObjectManagerExecuteQueryMessage.setSelectFieldsInManagedObject (prismManagedObjectSynchronousQueryContext.getSelectFieldsInManagedObject ());
    databaseObjectManagerExecuteQueryMessage.setSelectFields                (prismManagedObjectSynchronousQueryContext.getSelectFields                ());
    databaseObjectManagerExecuteQueryMessage.setLoadOneToManyRelationships  (prismManagedObjectSynchronousQueryContext.getLoadOneToManyRelationships  ());
    databaseObjectManagerExecuteQueryMessage.setLoadCompositions            (prismManagedObjectSynchronousQueryContext.getLoadCompositions            ());

    WaveMessageStatus status = sendSynchronously (&databaseObjectManagerExecuteQueryMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronously : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

        return (NULL);
    }
    else
    {
        completionStatus = databaseObjectManagerExecuteQueryMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronously : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));

            return (NULL);
        }
        else
        {
            vector<WaveManagedObject *> *pResults = databaseObjectManagerExecuteQueryMessage.getPResults ();

            // These are not added to bt tracker in constructor of the objects, since they are created in DB OM thread. Adding them to btTracker now.
            ObjectTracker::addQueriedObjectsToBtTracker (pResults);

            // Allow NULL results to be passed to the application.

            return (pResults);
        }
    }
}

vector<WaveManagedObject *> *WaveObjectManager::querySynchronously (const string &managedClassName, vector<ObjectId> &objectIds, const string &schema)
{
    WaveManagedObjectSynchronousQueryContext prismManagedObjectSynchronousQueryContext (managedClassName);

    prismManagedObjectSynchronousQueryContext.setSchemaToQuery (schema);
    prismManagedObjectSynchronousQueryContext.addObjectIds (objectIds);

    applyPartitionFiltersWithPropagatedPartitionContext (&prismManagedObjectSynchronousQueryContext);

    DatabaseObjectManagerExecuteQueryMessage  databaseObjectManagerExecuteQueryMessage (managedClassName, this, prismManagedObjectSynchronousQueryContext.getSql (), schema);
    ResourceId                                completionStatus                         = WAVE_MESSAGE_SUCCESS;

    databaseObjectManagerExecuteQueryMessage.setSelectFieldsInManagedObject (prismManagedObjectSynchronousQueryContext.getSelectFieldsInManagedObject ());
    databaseObjectManagerExecuteQueryMessage.setSelectFields                (prismManagedObjectSynchronousQueryContext.getSelectFields                ());
    databaseObjectManagerExecuteQueryMessage.setLoadOneToManyRelationships  (prismManagedObjectSynchronousQueryContext.getLoadOneToManyRelationships  ());
    databaseObjectManagerExecuteQueryMessage.setLoadCompositions            (prismManagedObjectSynchronousQueryContext.getLoadCompositions            ());

    WaveMessageStatus status = sendSynchronously (&databaseObjectManagerExecuteQueryMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronously : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

        return (NULL);
    }
    else
    {
        completionStatus = databaseObjectManagerExecuteQueryMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronously : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));

            return (NULL);
        }
        else
        {
            vector<WaveManagedObject *> *pResults = databaseObjectManagerExecuteQueryMessage.getPResults ();

            // These are not added to bt tracker in constructor of the objects, since they are created in DB OM thread. Adding them to btTracker now.
            ObjectTracker::addQueriedObjectsToBtTracker (pResults);

            // Allow NULL Results to be passed to application

            return (pResults);
        }
    }
}

vector<WaveManagedObject *> *WaveObjectManager::querySynchronouslyByName (const string &managedClassName, const string &managedObjectName, const string &schema)
{
    WaveManagedObjectSynchronousQueryContext prismManagedObjectSynchronousQueryContext (managedClassName);

    prismManagedObjectSynchronousQueryContext.setSchemaToQuery (schema);
    prismManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeString (managedObjectName, "name"));

    applyPartitionFiltersWithPropagatedPartitionContext (&prismManagedObjectSynchronousQueryContext);

    DatabaseObjectManagerExecuteQueryMessage  databaseObjectManagerExecuteQueryMessage (managedClassName, this, prismManagedObjectSynchronousQueryContext.getSql (), schema);
    ResourceId                                completionStatus                         = WAVE_MESSAGE_SUCCESS;

    databaseObjectManagerExecuteQueryMessage.setSelectFieldsInManagedObject (prismManagedObjectSynchronousQueryContext.getSelectFieldsInManagedObject ());
    databaseObjectManagerExecuteQueryMessage.setSelectFields                (prismManagedObjectSynchronousQueryContext.getSelectFields                ());
    databaseObjectManagerExecuteQueryMessage.setLoadOneToManyRelationships  (prismManagedObjectSynchronousQueryContext.getLoadOneToManyRelationships  ());
    databaseObjectManagerExecuteQueryMessage.setLoadCompositions            (prismManagedObjectSynchronousQueryContext.getLoadCompositions            ());

    WaveMessageStatus status = sendSynchronously (&databaseObjectManagerExecuteQueryMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronouslyByName : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

        return (NULL);
    }
    else
    {
        completionStatus = databaseObjectManagerExecuteQueryMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::querySynchronouslyByName : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));

            return (NULL);
        }
        else
        {
            vector<WaveManagedObject *> *pResults = databaseObjectManagerExecuteQueryMessage.getPResults ();

            // These are not added to bt tracker in constructor of the objects, since they are created in DB OM thread. Adding them to btTracker now.
            ObjectTracker::addQueriedObjectsToBtTracker (pResults);

            // Allow NULL Results to be passed to application

            return (pResults);
        }
    }
}

WaveManagedObject *WaveObjectManager::queryManagedObject (const ObjectId &managedObjectId, const string &schema)
{
    const string                  managedClassName     = OrmRepository::getTableNameById (managedObjectId.getClassId ());
    vector<ObjectId>              objectIdsToQueryFor;
    UI32                          numberOfResults      = 0;
    vector<WaveManagedObject *> *pResults             = NULL;
    WaveManagedObject           *pWaveManagedObject  = NULL;

    objectIdsToQueryFor.push_back (managedObjectId);
    pResults = querySynchronously (managedClassName, objectIdsToQueryFor, schema);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    if (NULL != pResults)
    {
        numberOfResults = pResults->size ();

        if (1 < numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::queryManagedObject : More than exepected (1) number of results : ") + numberOfResults);
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            if (0 == numberOfResults)
            {
                pWaveManagedObject = NULL;
            }
            else
            {
                pWaveManagedObject = (*pResults)[0];
            }
        }

        pResults->clear ();
        delete pResults;
    }

    return (pWaveManagedObject);
}

vector<WaveManagedObject *> *WaveObjectManager::queryManagedObjectAssociatedWithSlot (const string &managedClassName, UI32 slotNumber, LocationId locationId, const string &schema)
{
    trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::queryManagedObjectAssociatedWithSlot : Entering ...  ");

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (managedClassName);
    synchronousQueryContext.setSlotFilter (slotNumber);
    if (0 != locationId)
        synchronousQueryContext.setLocationIdFilter (locationId);

    return (querySynchronously (&synchronousQueryContext));
}

pthread_t WaveObjectManager::getPthreadId ()
{
    return (m_pAssociatedPrismThread->getId ());
}

void WaveObjectManager::consumeAllPendingMessages ()
{
    WaveThreadStatus status = m_pAssociatedPrismThread->consumePendingMessages ();

    if (WAVE_THREAD_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::consumeAllPendingMessages : Consuming All Pending Messages failed. status : " + FrameworkToolKit::localize (status));
    }
}

vector<WaveWorker *> WaveObjectManager::getWorkers(void)
{
    return(m_workers);
}

void WaveObjectManager::setTraceLevel (const TraceLevel &traceLevel)
{
    TraceObjectManager::setClientTraceLevelDirectly (getTraceClientId (), traceLevel);
}

void WaveObjectManager::setCpuAffinity (const vector<UI32> &cpuAffinityVector)
{
    m_pAssociatedPrismThread->setCpuAffinity (cpuAffinityVector);
}

void WaveObjectManager::setCpuAffinityHandler (PrismSetCpuAffinityObjectManagerMessage *pPrismSetCpuAffinityObjectManagerMessage)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::setCpuAffinityHandler : Entering ...");

    vector<UI32> cpuAffinityVector = pPrismSetCpuAffinityObjectManagerMessage->getCpuAffinityVector ();

    setCpuAffinity (cpuAffinityVector);

    pPrismSetCpuAffinityObjectManagerMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pPrismSetCpuAffinityObjectManagerMessage);
}

void WaveObjectManager::associateWithVirtualWaveObjectManager (WaveObjectManager *pAssociatedVirtualWaveObjectManager)
{
    if (NULL == pAssociatedVirtualWaveObjectManager)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::associateWithVirtualWaveObjectManager :  By default Associated Virtual Prism Object Manager is set to NULL.  If you are making this call, then you must supply a NON-NULL Prism Object Manager.");
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        // Ensure that we are a Local Prism Object Manager before applying the change - It is a runtime validation with dynamic_cast.

        WaveLocalObjectManager *pWaveLocalObjectManager = dynamic_cast<WaveLocalObjectManager *> (this);

        // Just use it in an expression so that the compiler does not complain about unsed variable.

        prismAssert (NULL != pWaveLocalObjectManager, __FILE__, __LINE__);

        // Then store it.

        m_pAssociatedVirtualWaveObjectManager = pAssociatedVirtualWaveObjectManager;
    }
}

ResourceId WaveObjectManager::sendSynchronouslyToWaveClient (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, const SI32 &Instance)
{
    if (NULL != m_pInputMessage && (m_pInputMessage->getIsMessageBeingSurrogatedFlag ()))
    {
        pManagementInterfaceMessage->updateForCompletionStatusDuringSurrogacy ();
        return WAVE_MESSAGE_SUCCESS;
    }

    pManagementInterfaceMessage->setClientName  (waveClientName+Instance);
    pManagementInterfaceMessage->setServiceCode (ManagementInterfaceObjectManager::getPrismServiceId ());

    return (sendSynchronously (pManagementInterfaceMessage));
}

WaveMessageStatus WaveObjectManager::sendToWaveServer (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds)
{
    pManagementInterfaceMessage->setServiceCode (WaveClientTransportObjectManager::getPrismServiceId ());
    pManagementInterfaceMessage->setServerId    (waveServerId);
    pManagementInterfaceMessage->setTtyName     (FrameworkToolKit::getCurrentTtyName ());

    return (send (pManagementInterfaceMessage, messageCallback, pInputContext, timeOutInMilliSeconds, 0, pPrismMessageSender));
}

ResourceId WaveObjectManager::sendOneWayToAllWaveClients(ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::sendOneWayToAllWaveClients : Entering");

    pManagementInterfaceMessage->setServiceCode (ManagementInterfaceObjectManager::getPrismServiceId ());
    pManagementInterfaceMessage->setAllWaveClients(true);

    return (sendOneWay(pManagementInterfaceMessage) );

}

ResourceId WaveObjectManager::sendToWaveClient (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext, UI32 timeOutInMilliSeconds, const SI32 &Instance)
{
    if (NULL != m_pInputMessage && (m_pInputMessage->getIsMessageBeingSurrogatedFlag ()))
    {
        trace (TRACE_LEVEL_DEBUG, string("WaveObjectManager::sendToWaveClient : getIsMessageBeingSurrogatedFlag returned : ") + m_pInputMessage->getIsMessageBeingSurrogatedFlag ());
        pManagementInterfaceMessage->updateForCompletionStatusDuringSurrogacy ();
        ResourceId status = executeMessageReplyDuringSurrogacy (pManagementInterfaceMessage, pPrismMessageCallback, pPrismMessageContext);
        return (status);
    }


    trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::sendToWaveClient : Name=" + waveClientName + " Instance=" +
       Instance);

    pManagementInterfaceMessage->setClientName  (waveClientName+Instance);
    pManagementInterfaceMessage->setServiceCode (ManagementInterfaceObjectManager::getPrismServiceId ());

    return (send (pManagementInterfaceMessage, pPrismMessageCallback, pPrismMessageContext, timeOutInMilliSeconds));
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name : executeMessageReplyDuringSurrogacy                                                                   //
// Purpose       : This function should be called in sendToWaveClient when a message is being surrogated by a local     //
//                 node for a disconnected node. In this scenario, the back-end daemon for the disconnected node will   //
//                 will not be available and hence the message should not be sent to back-end but should be returned    //
//                 with success. Along with this, the callback should be invoked for the same.                          //
//                 Steps involved : 1. set all the required fields  for the message as done in send                     //
//                 2. Add the callback/context for the message. 3. Add the message in the threads (OMs) replied messages//
//                 queue (submitT)      so that the callback will be called.                                            //
// Return value  : ResourceId (status)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ResourceId WaveObjectManager::executeMessageReplyDuringSurrogacy (ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pManagementInterfaceMessageContext)
{
    if (NULL == pManagementInterfaceMessage)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::executeMessageReplyDuringSurrogacy : Trying to send a message with NULL message.");
        return (WAVE_MESSAGE_ERROR_NULL_MESSAGE);
    }

    if (((PrismMessageResponseHandler) NULL) == pPrismMessageCallback)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::send : Trying to send a message with NULL callback.  If you do not want to register a callback send it as a one way message.");
        return (WAVE_MESSAGE_ERROR_NULL_CALLBACK);
    }

    // 1. Set all the required fields of the message

    // Set this so that the message can be returned after getting processed.
    pManagementInterfaceMessage->m_senderServiceCode = getServiceId ();

    // Store the receiver LocationId.
    pManagementInterfaceMessage->m_receiverLocationId = (FrameworkToolKit::getThisLocationId ());

    UI32 prismMessageId = pManagementInterfaceMessage->getMessageId ();
    if (NULL != (getResponseContext (prismMessageId)))
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::send : This message (with id : ") + prismMessageId + "was already registered.");
        return (WAVE_MESSAGE_ERROR_DUPLICATE_MESSAGE_SEND);
    }

    addMessageToMessageHistoryCalledFromSend (pManagementInterfaceMessage);


    if ((NULL != m_pInputMessage) && (m_pAssociatedPrismThread->getId () == pManagementInterfaceMessage->getPrismMessageCreatorThreadId ()))
    {
        pManagementInterfaceMessage->setIsConfigurationChange (m_pInputMessage->getIsConfigurationChange ());
        pManagementInterfaceMessage->setIsConfigurationTimeChange (m_pInputMessage->getIsConfigurationTimeChange ());
        pManagementInterfaceMessage->setTransactionCounter (m_pInputMessage->getTransactionCounter ());

        if (false == pManagementInterfaceMessage->getIsMessageBeingSurrogatedFlag ())
        {
            pManagementInterfaceMessage->setIsMessageBeingSurrogatedFlag  (m_pInputMessage->getIsMessageBeingSurrogatedFlag ());
            pManagementInterfaceMessage->setSurrogatingForLocationId      (m_pInputMessage->getSurrogatingForLocationId ());
        }
    }

    // 2. Add the callback for the message

    m_sendReplyMutexForResponseMap.lock ();
    // Now store the details related to callback so that we can call the appropriate callback when the reply to this message arrives.
    PrismMessageResponseContext *pManagementInterfaceMessageResponseContext = new PrismMessageResponseContext (pManagementInterfaceMessage, this, pPrismMessageCallback, pManagementInterfaceMessageContext);

    prismAssert (NULL != pManagementInterfaceMessageResponseContext, __FILE__, __LINE__);

    if (m_pAssociatedPrismThread->getId () == pManagementInterfaceMessage->getPrismMessageCreatorThreadId ())
    {
        pManagementInterfaceMessageResponseContext->setPInputMessageInResponseContext (m_pInputMessage);
    }
    addResponseMap (prismMessageId, pManagementInterfaceMessageResponseContext);

    m_sendReplyMutexForResponseMap.unlock ();

    // 3. Call the  to add this message in replied message queue

    PrismThread *pPrismThread = NULL;

    pPrismThread = PrismThread::getPrismThreadForServiceId (pManagementInterfaceMessage->getSenderServiceCode ());

    if (NULL == pPrismThread || (pPrismThread->getId () != PrismThread::getSelf ()))
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::executeMessageReplyDuringSurrogacy : PrismThread must be same.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pManagementInterfaceMessage->setType (WAVE_MESSAGE_TYPE_RESPONSE);

    addMessageToMessageHistoryCalledFromReply (pManagementInterfaceMessage);

    pPrismThread->submitReplyMessage (pManagementInterfaceMessage);

    return (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendToWaveClients (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    prismAssert (NULL != pWaveSendToClientsContext, __FILE__, __LINE__);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::sendPhase1MessageToAllInstancesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::computeFailedInstancesForPhase1Step),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::computeOverallInstancesStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveSendToClientsContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::sendPhase1MessageToAllInstancesStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::sendPhase1MessageToAllInstancesStep : Entering ...");

    WaveSendToClientsContext *pWaveSendToClientsContext = reinterpret_cast<WaveSendToClientsContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClientsContext, __FILE__, __LINE__);

    vector<SI32>              instancesToSendTo         = pWaveSendToClientsContext->getInstancesToSendToForPhase1 ();
    vector<SI32>              subInstancesToSendTo      = pWaveSendToClientsContext->getSubInstancesToSendToForPhase1 ();
    UI32                      numberOfInstancesToSendTo = instancesToSendTo.size ();
    UI32                      i                         = 0;
    ManagementInterfaceMessage             *pManagementInterfaceMessageForPhase1 = pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase1 ();

    prismAssert (NULL != pManagementInterfaceMessageForPhase1, __FILE__, __LINE__);
    UI32                      timeoutForPhase1          = pWaveSendToClientsContext->getTimeoutForPhase1 ();

    ++(*pPrismLinearSequencerContext);

    trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::sendPhase1MessageToAllInstancesStep : Name=") + pWaveSendToClientsContext->getClientName() + string(" Instances=") + numberOfInstancesToSendTo);

    // filter out none existing clients
    vector<SI32>              instancesForPhase1;
    vector<SI32>              subInstancesForPhase1;
    bool                      needSubInstance = pWaveSendToClientsContext->getIsSubInstanceToBeIncluded ();

    for (i = 0; i < numberOfInstancesToSendTo; i++)
    {
        // Skip MM (none LC) client
        if (instancesToSendTo[i] == 0)
        {
            continue;
        }

        string clientName;

        if (needSubInstance)
        {
            if (-1 == subInstancesToSendTo[i])
            {
                continue;
            }

            clientName = pWaveSendToClientsContext->getClientName () + string ("_p") + subInstancesToSendTo[i] + instancesToSendTo[i];
        }
        else
        {
            clientName = pWaveSendToClientsContext->getClientName () + instancesToSendTo[i];
        }

        if (false == (ManagementInterfaceObjectManager::getInstance ())->isAKnownClient (clientName))
        {
            trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::sendPhase1MessageToAllInstancesStep : Not found clientName=" + clientName);
        }
        else
        {
            instancesForPhase1.push_back (instancesToSendTo[i]);

            if (needSubInstance)
            {
                subInstancesForPhase1.push_back (subInstancesToSendTo[i]);
            }
        }
    }

    pWaveSendToClientsContext->setInstancesToSendToForPhase1 (instancesForPhase1);

    if (needSubInstance)
    {
        pWaveSendToClientsContext->setSubInstancesToSendToForPhase1 (subInstancesForPhase1);
    }

    numberOfInstancesToSendTo = instancesForPhase1.size ();
    pManagementInterfaceMessageForPhase1->setClientNameWithoutExtension (pWaveSendToClientsContext->getClientName ());
    pManagementInterfaceMessageForPhase1->setClientName (pWaveSendToClientsContext->getClientName ());
    pManagementInterfaceMessageForPhase1->setServiceCode (ManagementInterfaceObjectManager::getPrismServiceId ());

    for (i = 0; i < numberOfInstancesToSendTo; i++)
    {
        ManagementInterfaceMessage *pClonedManagementInterfaceMessageForPhase1 = reinterpret_cast<ManagementInterfaceMessage *> (pManagementInterfaceMessageForPhase1->clone ());
        prismAssert (NULL != pClonedManagementInterfaceMessageForPhase1, __FILE__, __LINE__);

        string clientName;

        if (needSubInstance)
        {
            clientName = pWaveSendToClientsContext->getClientName () + string ("_p") + subInstancesForPhase1[i] + instancesForPhase1[i];
        }
        else
        {
            clientName = pWaveSendToClientsContext->getClientName () + instancesForPhase1[i];
        }

        trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::sendPhase1MessageToAllInstancesStep : Name=" + clientName);

        pClonedManagementInterfaceMessageForPhase1->setClientNameWithoutExtension (clientName);
        pClonedManagementInterfaceMessageForPhase1->setClientName (clientName);
        pClonedManagementInterfaceMessageForPhase1->setSlotInstance (instancesForPhase1[i]);

        WaveMessageStatus status = send (pClonedManagementInterfaceMessageForPhase1, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::sendPhase1MessageToAllInstancesCallback), pPrismLinearSequencerContext, timeoutForPhase1);

        pWaveSendToClientsContext->setSendStatusForPhase1 (instancesForPhase1[i], status);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
            delete pClonedManagementInterfaceMessageForPhase1;
        }
        else
        {
            ++(*pPrismLinearSequencerContext);
        }
    }

    --(*pPrismLinearSequencerContext);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendPhase1MessageToAllInstancesCallback (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    --(*pPrismLinearSequencerContext);

    WaveSendToClientsContext *pWaveSendToClientsContext = reinterpret_cast<WaveSendToClientsContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClientsContext, __FILE__, __LINE__);
    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    SI32              instance = pManagementInterfaceMessage->getSlotInstance ();

    pWaveSendToClientsContext->setResultingMessageForPhase1 (instance, pManagementInterfaceMessage);

    pWaveSendToClientsContext->setFrameworkStatusForPhase1 (instance, frameworkStatus);

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllInstancesCallback : Instance %u : Framework Status : %s", instance, (FrameworkToolKit::localize (frameworkStatus)).c_str ());

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }
    else
    {
        ResourceId completionStatus = pManagementInterfaceMessage->getCompletionStatus ();

        pWaveSendToClientsContext->setCompletionStatusForPhase1 (instance, completionStatus);

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllInstancesCallback : Instance %u : Completion Status : %s", instance, (FrameworkToolKit::localize (completionStatus)).c_str ());

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::computeFailedInstancesForPhase1Step (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::computeFailedInstancesForPhase1Step : Entering ...");

    WaveSendToClientsContext *pWaveSendToClientsContext           = reinterpret_cast<WaveSendToClientsContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClientsContext, __FILE__, __LINE__);

    vector<SI32>        instancesToSendToForPhase1          = pWaveSendToClientsContext->getInstancesToSendToForPhase1 ();
    vector<SI32>        subInstancesToSendToForPhase1       = pWaveSendToClientsContext->getSubInstancesToSendToForPhase1 ();
    UI32                numberOfInstancesToSendToForPhase1  = instancesToSendToForPhase1.size ();
    UI32                i                                   = 0;
    vector<SI32>        failedInstancesForPhase1;
    UI32                numberOfFailedInstancesForPhase1    = 0;
    vector<SI32>        succeededInstancesForPhase1;
    vector<SI32>        succeededSubInstancesForPhase1;
    UI32                numberOfSucceededInstancesForPhase1 = 0;
    UI32                failureCountFromContext             = pPrismLinearSequencerContext->getNumberOfFailures ();
    bool                needSubInstance                     = pWaveSendToClientsContext->getIsSubInstanceToBeIncluded ();

    for (i = 0; i < numberOfInstancesToSendToForPhase1; i++)
    {
        WaveMessageStatus sendStatus = pWaveSendToClientsContext->getSendStatusForPhase1 (instancesToSendToForPhase1[i]);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            failedInstancesForPhase1.push_back (instancesToSendToForPhase1[i]);
        }
        else
        {
            FrameworkStatus frameworkStatus = pWaveSendToClientsContext->getFrameworkStatusForPhase1 (instancesToSendToForPhase1[i]);

            if (FRAMEWORK_SUCCESS != frameworkStatus)
            {
                failedInstancesForPhase1.push_back (instancesToSendToForPhase1[i]);
            }
            else
            {
                ResourceId completionStatus = pWaveSendToClientsContext->getCompletionStatusForPhase1 (instancesToSendToForPhase1[i]);

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    failedInstancesForPhase1.push_back (instancesToSendToForPhase1[i]);
                }
                else
                {
                    succeededInstancesForPhase1.push_back (instancesToSendToForPhase1[i]);

                    if (needSubInstance)
                    {
                        succeededSubInstancesForPhase1.push_back (subInstancesToSendToForPhase1[i]);
                    }
                }
            }
        }
    }

    numberOfFailedInstancesForPhase1    = failedInstancesForPhase1.size ();
    numberOfSucceededInstancesForPhase1 = succeededInstancesForPhase1.size ();

    prismAssert (failureCountFromContext == numberOfFailedInstancesForPhase1, __FILE__, __LINE__);
    prismAssert (numberOfInstancesToSendToForPhase1 == (numberOfFailedInstancesForPhase1 + numberOfSucceededInstancesForPhase1), __FILE__, __LINE__);

    pWaveSendToClientsContext->setNumberOfFailuresForPhase1 (numberOfFailedInstancesForPhase1);

    if ((0 < numberOfFailedInstancesForPhase1) && (0 < numberOfSucceededInstancesForPhase1))
    {
        pWaveSendToClientsContext->setInstancesToSendToForPhase2 (succeededInstancesForPhase1);

        if (needSubInstance)
        {
            pWaveSendToClientsContext->setSubInstancesToSendToForPhase2 (succeededSubInstancesForPhase1);
        }
    }

    trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::computeFailedInstancesForPhase1Step : Success=") + numberOfSucceededInstancesForPhase1 + string (" Fail=") + numberOfFailedInstancesForPhase1);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableStep : Entering ...");

    WaveSendToClientsContext *pWaveSendToClientsContext = reinterpret_cast<WaveSendToClientsContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClientsContext, __FILE__, __LINE__);
    vector<SI32>              instancesToSendToForPhase1          = pWaveSendToClientsContext->getInstancesToSendToForPhase1 ();
    UI32                      numberOfInstancesToSendToForPhase1  = instancesToSendToForPhase1.size ();
    UI32                      numberOfFailedInstancesForPhase1    = pWaveSendToClientsContext->getNumberOfFailuresForPhase1 ();
    vector<SI32>              instancesToSendToForPhase2          = pWaveSendToClientsContext->getInstancesToSendToForPhase2 ();
    vector<SI32>              subInstancesToSendToForPhase2       = pWaveSendToClientsContext->getSubInstancesToSendToForPhase2 ();
    UI32                      numberOfInstancesToSendToForPhase2  = instancesToSendToForPhase2.size ();
    UI32                      i                                   = 0;
    bool                      needSubInstance                     = pWaveSendToClientsContext->getIsSubInstanceToBeIncluded ();

    ManagementInterfaceMessage      *pManagementInterfaceMessageForPhase2  = pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase2 ();

    pPrismLinearSequencerContext->setNumberOfFailures (0);

    if (numberOfInstancesToSendToForPhase1 == numberOfFailedInstancesForPhase1)
    {
        trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableStep : not applicable 1");
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    if (NULL == pManagementInterfaceMessageForPhase2)
    {
        trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableStep : not applicable 2");
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    UI32                      timeoutForPhase2          = pWaveSendToClientsContext->getTimeoutForPhase2 ();

    ++(*pPrismLinearSequencerContext);

    pManagementInterfaceMessageForPhase2->setClientNameWithoutExtension (pWaveSendToClientsContext->getClientName ());
    pManagementInterfaceMessageForPhase2->setClientName (pWaveSendToClientsContext->getClientName ());

    for (i = 0; i < numberOfInstancesToSendToForPhase2; i++)
    {
        ManagementInterfaceMessage *pClonedManagementInterfaceMessageForPhase2 = reinterpret_cast<ManagementInterfaceMessage *> (pManagementInterfaceMessageForPhase2->clone ());

        prismAssert (NULL != pClonedManagementInterfaceMessageForPhase2, __FILE__, __LINE__);

        string clientName;

        if (needSubInstance)
        {
            clientName = pWaveSendToClientsContext->getClientName () + string ("_p") + subInstancesToSendToForPhase2[i] + instancesToSendToForPhase2[i];
        }
        else
        {
            clientName = pWaveSendToClientsContext->getClientName () + instancesToSendToForPhase2[i];
        }

        trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::sendPhase2MessageToAllInstancesStep : Name=" + clientName);

        pClonedManagementInterfaceMessageForPhase2->setClientNameWithoutExtension (clientName);
        pClonedManagementInterfaceMessageForPhase2->setClientName (clientName);

        pClonedManagementInterfaceMessageForPhase2->setServiceCode (ManagementInterfaceObjectManager::getPrismServiceId ());
        pClonedManagementInterfaceMessageForPhase2->setSlotInstance (instancesToSendToForPhase2[i]);

        WaveMessageStatus status = send (pClonedManagementInterfaceMessageForPhase2, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableCallback), pPrismLinearSequencerContext, timeoutForPhase2);

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableStep : Instance %u : Send Status : %s", instancesToSendToForPhase2[i], (FrameworkToolKit::localize (status)).c_str ());

        pWaveSendToClientsContext->setSendStatusForPhase2 (instancesToSendToForPhase2[i], status);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
            delete pClonedManagementInterfaceMessageForPhase2;
        }
        else
        {
            ++(*pPrismLinearSequencerContext);
        }
    }

    --(*pPrismLinearSequencerContext);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableCallback (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    --(*pPrismLinearSequencerContext);

    WaveSendToClientsContext *pWaveSendToClientsContext = reinterpret_cast<WaveSendToClientsContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClientsContext, __FILE__, __LINE__);

    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    SI32              instance = pManagementInterfaceMessage->getSlotInstance ();

    pWaveSendToClientsContext->setResultingMessageForPhase2 (instance, pManagementInterfaceMessage);

    pWaveSendToClientsContext->setFrameworkStatusForPhase2 (instance, frameworkStatus);

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase2MessageToAllInstancesIfApplicableCallback : Instance %u : Framework Status : %s", instance, (FrameworkToolKit::localize (frameworkStatus)).c_str ());

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }
    else
    {
        ResourceId completionStatus = pManagementInterfaceMessage->getCompletionStatus ();

        pWaveSendToClientsContext->setCompletionStatusForPhase2 (instance, completionStatus);

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableCallback : Instance %u : Send Status : %s", instance, (FrameworkToolKit::localize (completionStatus)).c_str ());

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
        }
        else
        {
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}



void WaveObjectManager::computeOverallInstancesStatusStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::computeOverallStatusStep : Entering ...");

    WaveSendToClientsContext *pWaveSendToClientsContext           = reinterpret_cast<WaveSendToClientsContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClientsContext, __FILE__, __LINE__);

    vector<SI32>        instancesToSendToForPhase1          = pWaveSendToClientsContext->getInstancesToSendToForPhase1 ();
    UI32                numberOfInstancesToSendToForPhase1  = instancesToSendToForPhase1.size ();
    vector<SI32>        instancesToSendToForPhase2          = pWaveSendToClientsContext->getInstancesToSendToForPhase2 ();
    UI32                numberOfInstancesToSendToForPhase2  = instancesToSendToForPhase2.size ();
    UI32                i                                   = 0;
    UI32                numberOfFailedInstancesForPhase1    = pWaveSendToClientsContext->getNumberOfFailuresForPhase1 ();
    vector<SI32>        failedInstancesForPhase2;
    UI32                numberOfFailedInstancesForPhase2    = 0;
    vector<SI32>        succeededInstancesForPhase2;
    UI32                numberOfSucceededInstancesForPhase2 = 0;
    UI32                failureCountFromContext             = pPrismLinearSequencerContext->getNumberOfFailures ();
    ResourceId          overAllCompletionStatus             = WAVE_MESSAGE_ERROR;

    ManagementInterfaceMessage             *pManagementInterfaceMessageForPhase2              = pWaveSendToClientsContext->getPManagementInterfaceMessageForPhase2 ();

    if (NULL != pManagementInterfaceMessageForPhase2)
    {
        for (i = 0; i < numberOfInstancesToSendToForPhase2; i++)
        {
            WaveMessageStatus sendStatus = pWaveSendToClientsContext->getSendStatusForPhase2 (instancesToSendToForPhase2[i]);

            if (WAVE_MESSAGE_SUCCESS != sendStatus)
            {
                failedInstancesForPhase2.push_back (instancesToSendToForPhase2[i]);
            }
            else
            {
                FrameworkStatus frameworkStatus = pWaveSendToClientsContext->getFrameworkStatusForPhase2 (instancesToSendToForPhase2[i]);

                if (FRAMEWORK_SUCCESS != frameworkStatus)
                {
                    failedInstancesForPhase2.push_back (instancesToSendToForPhase2[i]);
                }
                else
                {
                    ResourceId completionStatus = pWaveSendToClientsContext->getCompletionStatusForPhase2 (instancesToSendToForPhase2[i]);

                    if (WAVE_MESSAGE_SUCCESS != completionStatus)
                    {
                        failedInstancesForPhase2.push_back (instancesToSendToForPhase2[i]);
                    }
                    else
                    {
                        succeededInstancesForPhase2.push_back (instancesToSendToForPhase2[i]);
                    }
                }
            }
        }

        numberOfFailedInstancesForPhase2    = failedInstancesForPhase2.size ();
        numberOfSucceededInstancesForPhase2 = succeededInstancesForPhase2.size ();

        prismAssert (failureCountFromContext == numberOfFailedInstancesForPhase2, __FILE__, __LINE__);
        prismAssert (numberOfInstancesToSendToForPhase2 == (numberOfFailedInstancesForPhase2 + numberOfSucceededInstancesForPhase2), __FILE__, __LINE__);

        pWaveSendToClientsContext->setNumberOfFailuresForPhase2 (numberOfFailedInstancesForPhase2);
    }

    if (0 == numberOfFailedInstancesForPhase1)
    {
        overAllCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if (numberOfFailedInstancesForPhase1 == numberOfInstancesToSendToForPhase1)
        {
            overAllCompletionStatus = WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES;
        }
        else
        {
            if (NULL == pManagementInterfaceMessageForPhase2)
            {
                overAllCompletionStatus = WAVE_MESSAGE_ERROR_FAILED_ON_SOME_NODES;
            }
            else
            {
                if (0 == numberOfFailedInstancesForPhase2)
                {
                    overAllCompletionStatus = WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED;
                }
                else
                {
                    overAllCompletionStatus = WAVE_MESSAGE_ERROR_ROLLBACK_FAILED;
                }
            }
        }
    }
    trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::computeOverallStatusStep Phase1 : Fail=") + numberOfFailedInstancesForPhase1);

    trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::computeOverallStatusStep Phase2 : Fail=") + numberOfFailedInstancesForPhase2);

    trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::computeOverallStatusStep : ") + FrameworkToolKit::localize (overAllCompletionStatus));

    pPrismLinearSequencerContext->executeNextStep (overAllCompletionStatus);
}

bool WaveObjectManager::getAllowAutomaticallyUnlistenForEvents () const
{
    return (m_allowAutomaticallyUnlistenForEvents);
}

void WaveObjectManager::setAllowAutomaticallyUnlistenForEvents (const bool &allowAutomaticallyUnlistenForEvents)
{
    m_allowAutomaticallyUnlistenForEvents = allowAutomaticallyUnlistenForEvents;
}

void WaveObjectManager::printfToWaveClientSession (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, va_list &variableArguments)
{
    LocationId waveClientOriginatingLocationId = waveClientSessionContext.getWaveClientOriginatingLocationId ();
    UI32       waveNativeClientId              = waveClientSessionContext.getWaveNativeClientId              ();
    UI32       waveUserClientId                = waveClientSessionContext.getWaveUserClientId                ();

    if (0 == waveClientOriginatingLocationId)
    {
        return;
    }

    if ((0 != waveNativeClientId) || (0 != waveUserClientId))
    {
        char       messageBuffer[1024];
        SI32       numberOfCharacters   = 0;
        string     message;
        ResourceId status;

        numberOfCharacters = vsnprintf (messageBuffer, 1024, pFormat, variableArguments);

        if (0 < numberOfCharacters)
        {
			message = messageBuffer;

			// Send message to Wave Client Session on the appropriate location.

			TraceObjectManagerPrintToWaveClientSessionMessage *pTraceObjectManagerPrintToWaveClientSessionMessage = new TraceObjectManagerPrintToWaveClientSessionMessage (waveClientOriginatingLocationId, waveNativeClientId, waveUserClientId, message);

			status = sendOneWay (pTraceObjectManagerPrintToWaveClientSessionMessage, waveClientOriginatingLocationId);

			if (WAVE_MESSAGE_SUCCESS != status)
			{
				trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::printfToWaveClientSession : Could not send message.  Status : " + FrameworkToolKit::localize (status));
			}
        }
    }

    return;
}

void WaveObjectManager::printfToWaveClientSession (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    printfToWaveClientSession (waveClientSessionContext, pFormat, variableArguments);

    va_end (variableArguments);
}

void WaveObjectManager::printfToAllWaveClientSessions (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, va_list &variableArguments)
{
    LocationId waveClientOriginatingLocationId = waveClientSessionContext.getWaveClientOriginatingLocationId ();

    if (0 == waveClientOriginatingLocationId)
    {
        return;
    }

    char       messageBuffer[1024];
    SI32       numberOfCharacters   = 0;
    string     message;
    ResourceId status;

    numberOfCharacters = vsnprintf (messageBuffer, 1024, pFormat, variableArguments);

    if (0 < numberOfCharacters)
    {
		message = messageBuffer;

		// Send message to Wave Client Session on the appropriate location.

		TraceObjectManagerPrintToAllWaveClientSessionsMessage *pTraceObjectManagerPrintToAllWaveClientSessionsMessage = new TraceObjectManagerPrintToAllWaveClientSessionsMessage (waveClientOriginatingLocationId, message);

		status = sendOneWay (pTraceObjectManagerPrintToAllWaveClientSessionsMessage, waveClientOriginatingLocationId);

		if (WAVE_MESSAGE_SUCCESS != status)
		{
			trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::printfToAllWaveClientSessions : Could not send message.  Status : " + FrameworkToolKit::localize (status));
		}
    }

    return;
}

void WaveObjectManager::printfToAllWaveClientSessions (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    printfToAllWaveClientSessions (waveClientSessionContext, pFormat, variableArguments);

    va_end (variableArguments);
}


// When printfToWaveClientSession () calls are made inside a Message Handler, one-way messages are sent to TraceOM on client (cli session) location.
// It may happen that Message got replied from Message Handler (where printfToWaveClientSession calls are made),
// and actionpoint returned WYSEREEA_SUCCESS, prompt was displayed on cli session, and then traceOM printed text from one-way messages received.
// In this case prompt gets lost in output text.
//
// Following function sends a message with empty text string   synchronously to TraceOM on client location.
// Once printfToWaveClientSessionOver () returns, traceOM on client location has completed printing on cli for previously received one-way messages.
// This function should not be called by TraceOM.

void WaveObjectManager::printfToWaveClientSessionOver (const WaveClientSessionContext &waveClientSessionContext)
{
    LocationId waveClientOriginatingLocationId = waveClientSessionContext.getWaveClientOriginatingLocationId ();
    UI32       waveNativeClientId              = waveClientSessionContext.getWaveNativeClientId              ();
    UI32       waveUserClientId                = waveClientSessionContext.getWaveUserClientId                ();

    if (0 == waveClientOriginatingLocationId)
    {
        return;
    }

    if ((0 != waveNativeClientId) || (0 != waveUserClientId))
    {
        string     message  = "";
        ResourceId status   = WAVE_MESSAGE_ERROR;

        // Send message to Wave Client Session on the appropriate location.

        TraceObjectManagerPrintToWaveClientSessionMessage traceObjectManagerPrintToWaveClientSessionMessage (waveClientOriginatingLocationId, waveNativeClientId, waveUserClientId, message);

        status = sendSynchronously (&traceObjectManagerPrintToWaveClientSessionMessage, waveClientOriginatingLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::printfToWaveClientSessionOver : Could not send message.  Status : " + FrameworkToolKit::localize (status));
        }
        else
        {
            status = traceObjectManagerPrintToWaveClientSessionMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "WaveObjectManager::printfToWaveClientSessionOver : message processing failed. Status : " + FrameworkToolKit::localize (status));
            }
        }
    }

    return;
}

bool WaveObjectManager::isBeingSurrogated ()
{
    bool isbeingsurrogated = false;
    if (m_pInputMessage && (m_pInputMessage->getIsMessageBeingSurrogatedFlag ()))
    {
        isbeingsurrogated = true;
    }
    return isbeingsurrogated;
}

void WaveObjectManager::sendOneWayToWaveCluster (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    pWaveSendToClusterContext->setIsSendOneWayToWaveCluster (true);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        // These two sequencer steps are reused from sendToWaveCluster's linear sequencer.

        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::computeDisconnectedNodesIfSurrogateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::sendPhase1MessageToAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveSendToClusterContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::sendToWaveCluster (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    pWaveSendToClusterContext->setIsSendOneWayToWaveCluster (false);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::computeDisconnectedNodesIfSurrogateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::sendPhase1MessageToAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::computeFailedNodesForPhase1Step),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::computeOverallStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveSendToClusterContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::computeDisconnectedNodesIfSurrogateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : Entering ...");

    WaveSendToClusterContext *pWaveSendToClusterContext = reinterpret_cast<WaveSendToClusterContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());
    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    PrismMessage *pPrismMessageForPhase1 = pWaveSendToClusterContext->getPPrismMessageForPhase1 ();
    prismAssert (NULL != pPrismMessageForPhase1, __FILE__, __LINE__);

    if (false == pPrismMessageForPhase1->getNeedSurrogateSupportFlag ())
    {
        trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : Message doesn't need Surrogate support.");
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    // Check if the vector locationsToSendToForPhase1 is set by user. If yes, no need to get all disconnected locations.
    if (pWaveSendToClusterContext->getIsLocationsForPhase1SetByUserFlag ())
    {
        vector<LocationId> disconnectedLocations;
        vector<LocationId> locationsToSendToForPhase1           = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
        UI32               numberOfLocationsForPhase1           = locationsToSendToForPhase1.size ();

        // In this case, the vector numberOfLocationsForPhase1 will be kept as it is. Will update the disconnected locations from this vector
        // to disconnectedLocations.
        for (unsigned int i = 0; i < numberOfLocationsForPhase1; i++)
        {
            if (false == FrameworkToolKit::isAConnectedLocation (locationsToSendToForPhase1 [i]))
            {
                trace (TRACE_LEVEL_DEBUG, string("WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : Disconnected location : ") + locationsToSendToForPhase1 [i]);
                disconnectedLocations.push_back (locationsToSendToForPhase1 [i]);
            }
        }

        UI32 numberOfDisconnectedLocations = disconnectedLocations.size ();
        if (0 < numberOfDisconnectedLocations)
        {
            pWaveSendToClusterContext->setFailedOverLocations (disconnectedLocations);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, string("WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : No disconnected location in the list set by user."));
        }
    }
    else
    {
        vector<LocationId> knownLocations;
        vector<LocationId> disconnectedLocations;
        vector<LocationId> locationsToSendToForPhase1           = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
        UI32               numberOfInitialLocationsForPhase1    = locationsToSendToForPhase1.size ();

        FrameworkToolKit::getKnownLocations (knownLocations);

        for (unsigned int i = 0; i < knownLocations.size(); i++)
        {
            if (false == FrameworkToolKit::isAConnectedLocation (knownLocations [i]))
            {
                trace (TRACE_LEVEL_DEBUG, string("WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : Disconnected location : ") + knownLocations [i]);
                disconnectedLocations.push_back (knownLocations [i]);
            }
            else
            {
                trace (TRACE_LEVEL_DEBUG, string("WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : Connected Location : ") + knownLocations [i]);
            }
        }

        UI32 numberOfDisconnectedLocations = disconnectedLocations.size ();
        if (0 < numberOfDisconnectedLocations)
        {
            pWaveSendToClusterContext->setFailedOverLocations (disconnectedLocations);

            // Following logic is required to make sure no duplicate locations are set in the vector locationsToSendToForPhase1
            bool hash [100]   = {0};
            UI32 index;

            for (unsigned int i = 0 ; i < numberOfInitialLocationsForPhase1; i++)
            {
                index = locationsToSendToForPhase1 [i] / 256;
                hash [index] = 1;
            }

            for (unsigned int i = 0 ; i < numberOfDisconnectedLocations; i++)
            {
                index = disconnectedLocations [i] / 256;
                if (1 != hash [index])
                {
                    locationsToSendToForPhase1.push_back (disconnectedLocations [i]);
                    trace (TRACE_LEVEL_DEBUG, string("WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : Disconnected location added to phase1 location: ") + disconnectedLocations [i]);
                }
                else
                {
                    trace (TRACE_LEVEL_DEBUG, string("WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : location being repeated : ") + disconnectedLocations [i]);
                }
            }
            pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationsToSendToForPhase1);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : No disconnected location");
        }

        UI32 numberOfNewLocationsForPhase1 = (pWaveSendToClusterContext->getLocationsToSendToForPhase1 ()).size ();
        UI32 numberOfAllLocations          = knownLocations.size () + 1; // +1 is for local node count.

        if (numberOfAllLocations != numberOfNewLocationsForPhase1)
        {
            trace (TRACE_LEVEL_INFO, "WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : Failed to set the correct number of locations for phase 1 message send.");
            tracePrintf (TRACE_LEVEL_INFO, true, false, "WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : numberOfNewLocationsForPhase1 = %d, numberOfAllLocations = %d, numberOfDisconnectedLocations = %d, numberOfInitialLocationsForPhase1 = %d", numberOfNewLocationsForPhase1, numberOfAllLocations, numberOfDisconnectedLocations, numberOfInitialLocationsForPhase1);
            trace (TRACE_LEVEL_INFO, "WaveObjectManager::computeDisconnectedNodesIfSurrogateStep : sendToWaveCluster issued during Add node operation.");
            //Commented below to handle the case when sendToWaveCluster is issued during cluster operations(add node)

            //pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
            //return;
        }

    }
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendPhase1MessageToAllNodesStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::sendPhase1MessageToAllNodesStep : Entering ...");

    WaveSendToClusterContext *pWaveSendToClusterContext = reinterpret_cast<WaveSendToClusterContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    WaveMessageStatus         status                    = WAVE_MESSAGE_ERROR;
    vector<LocationId>        locationsToSendTo         = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                      numberOfLocationsToSendTo = locationsToSendTo.size ();
    UI32                      i                         = 0;
    PrismMessage             *pPrismMessageForPhase1    = pWaveSendToClusterContext->getPPrismMessageForPhase1 ();

    prismAssert (NULL != pPrismMessageForPhase1, __FILE__, __LINE__);

    UI32                      timeoutForPhase1          = pWaveSendToClusterContext->getTimeoutForPhase1 ();
    bool                      isSendOneWayToWaveCluster = pWaveSendToClusterContext->getIsSendOneWayToWaveCluster ();

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfLocationsToSendTo; i++)
    {
        PrismMessage *pClonedPrismMessageForPhase1 = pPrismMessageForPhase1->clone ();

        prismAssert (NULL != pClonedPrismMessageForPhase1, __FILE__, __LINE__);

        if (0 >= locationsToSendTo[i])
        {
            tracePrintf (TRACE_LEVEL_INFO, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesStep : incorrect location-id specified for phase1 message : %d", locationsToSendTo[i]);
        }
        else if (pPrismMessageForPhase1->getNeedSurrogateSupportFlag () && (locationsToSendTo[i] != FrameworkToolKit::getThisLocationId ()) && (false == FrameworkToolKit::isAConnectedLocation (locationsToSendTo[i])))
        {
            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesStep : getNeedSurrogateSupportFlag = %d", pClonedPrismMessageForPhase1->getNeedSurrogateSupportFlag ());
            LocationId locationToSurrogate = 0;
            LocationBase *pThisLocation = FrameworkToolKit::getPThisLocation ();
            if (NULL != pThisLocation)
            {
                locationToSurrogate = ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getLocationId ());
            }

            pClonedPrismMessageForPhase1->setSurrogatingForLocationId (locationsToSendTo[i]);
            pClonedPrismMessageForPhase1->setIsMessageBeingSurrogatedFlag (true);

            if (true == isSendOneWayToWaveCluster)
            {
                status = sendOneWay (pClonedPrismMessageForPhase1, locationToSurrogate);

                tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesStep : Location Id %u surrogating for %u : Send One Way Status : %s", locationToSurrogate, locationsToSendTo[i], (FrameworkToolKit::localize (status)).c_str ());
            }
            else
            {
                // Perform regular asynchronous send to wave cluster.

                status = send (pClonedPrismMessageForPhase1, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::sendPhase1MessageToAllNodesCallback), pPrismLinearSequencerContext, timeoutForPhase1, locationToSurrogate);

                tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesStep : Location Id %u surrogating for %u : Send Status : %s", locationToSurrogate, locationsToSendTo[i], (FrameworkToolKit::localize (status)).c_str ());
            }
       }
       else
       {
            if (true == isSendOneWayToWaveCluster)
            {
                status = sendOneWay (pClonedPrismMessageForPhase1, locationsToSendTo[i]);

                tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesStep : Location Id %u : Send One Way Status : %s", locationsToSendTo[i], (FrameworkToolKit::localize (status)).c_str ());
            }
            else
            {
                // Perform regular asynchronous send to wave cluster.

                status = send (pClonedPrismMessageForPhase1, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::sendPhase1MessageToAllNodesCallback), pPrismLinearSequencerContext, timeoutForPhase1, locationsToSendTo[i]);

                tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesStep : Location Id %u : Send Status : %s", locationsToSendTo[i], (FrameworkToolKit::localize (status)).c_str ());
            }
       }

        pWaveSendToClusterContext->setSendStatusForPhase1 (locationsToSendTo[i], status);


        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
            delete pClonedPrismMessageForPhase1;
        }
        else
        {
            if (false == isSendOneWayToWaveCluster)
            {
                ++(*pPrismLinearSequencerContext);
            }
        }
    }

    --(*pPrismLinearSequencerContext);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendPhase1MessageToAllNodesCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    --(*pPrismLinearSequencerContext);

    WaveSendToClusterContext *pWaveSendToClusterContext = reinterpret_cast<WaveSendToClusterContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    LocationId receiverLocationId = 0;
    if (true == pPrismMessage->getIsMessageBeingSurrogatedFlag ())
    {
        receiverLocationId = pPrismMessage->getSurrogatingForLocationId ();
    }
    else
    {
        receiverLocationId = pPrismMessage->getReceiverLocationId ();
    }


    pWaveSendToClusterContext->setResultingMessageForPhase1 (receiverLocationId, pPrismMessage);

    pWaveSendToClusterContext->setFrameworkStatusForPhase1 (receiverLocationId, frameworkStatus);

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesCallback : Location Id %u : Framework Status : %s", receiverLocationId, (FrameworkToolKit::localize (frameworkStatus)).c_str ());

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }
    else
    {
        ResourceId completionStatus = pPrismMessage->getCompletionStatus ();

        pWaveSendToClusterContext->setCompletionStatusForPhase1 (receiverLocationId, completionStatus);

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase1MessageToAllNodesCallback : Location Id %u : Completion Status : %s", receiverLocationId, (FrameworkToolKit::localize (completionStatus)).c_str ());

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
        }
        else
        {
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::computeFailedNodesForPhase1Step (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::computeFailedNodesForPhase1Step : Entering ...");

    WaveSendToClusterContext *pWaveSendToClusterContext           = reinterpret_cast<WaveSendToClusterContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    vector<LocationId>        locationsToSendToForPhase1          = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                      numberOfLocationsToSendToForPhase1  = locationsToSendToForPhase1.size ();
    UI32                      i                                   = 0;
    vector<LocationId>        failedLocationsForPhase1;
    UI32                      numberOfFailedLocationsForPhase1    = 0;
    vector<LocationId>        disconnectedLocationsForPhase1;
    UI32                      numberOfDisconnectedLocationsForPhase1    = 0;
    vector<LocationId>        succeededLocationsForPhase1;
    UI32                      numberOfSucceededLocationsForPhase1 = 0;
    UI32                      failureCountFromContext             = pPrismLinearSequencerContext->getNumberOfFailures ();

    for (i = 0; i < numberOfLocationsToSendToForPhase1; i++)
    {
        WaveMessageStatus sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (locationsToSendToForPhase1[i]);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            failedLocationsForPhase1.push_back (locationsToSendToForPhase1[i]);
        }
        else
        {
            FrameworkStatus frameworkStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase1 (locationsToSendToForPhase1[i]);

            if (FRAMEWORK_SUCCESS != frameworkStatus)
            {
                failedLocationsForPhase1.push_back (locationsToSendToForPhase1[i]);
            }
            else
            {
                ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1 (locationsToSendToForPhase1[i]);

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    if ((WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE == completionStatus) && pWaveSendToClusterContext->getTreatFailureOnFailingOverAsSuccessFlag())
                    {
                        disconnectedLocationsForPhase1.push_back (locationsToSendToForPhase1[i]);
                    }
                    else
                    {
                        failedLocationsForPhase1.push_back (locationsToSendToForPhase1[i]);
                    }
                }
                else
                {
                    succeededLocationsForPhase1.push_back (locationsToSendToForPhase1[i]);
                }
            }
        }
    }

    numberOfFailedLocationsForPhase1    = failedLocationsForPhase1.size ();
    numberOfDisconnectedLocationsForPhase1 = disconnectedLocationsForPhase1.size ();
    numberOfSucceededLocationsForPhase1 = succeededLocationsForPhase1.size ();

    prismAssert (failureCountFromContext == (numberOfFailedLocationsForPhase1 + numberOfDisconnectedLocationsForPhase1), __FILE__, __LINE__);
    prismAssert (numberOfLocationsToSendToForPhase1 == (numberOfFailedLocationsForPhase1 + numberOfDisconnectedLocationsForPhase1 + numberOfSucceededLocationsForPhase1), __FILE__, __LINE__);

    pWaveSendToClusterContext->setNumberOfFailuresForPhase1 (numberOfFailedLocationsForPhase1);
    pWaveSendToClusterContext->setNumberOfDisconnectedNodesForPhase1 (numberOfDisconnectedLocationsForPhase1);

    if ((0 < numberOfFailedLocationsForPhase1) && (0 < numberOfSucceededLocationsForPhase1))
    {
        if (true == pWaveSendToClusterContext->getPartialSuccessFlag ())
        {
            trace (TRACE_LEVEL_INFO, "WaveObjectManager::computeFailedNodesForPhase1Step : Partial Success case.");
            pWaveSendToClusterContext->setIsPartialSuccessCaseFlag (true);
        }
        else if (NULL != pWaveSendToClusterContext->getPPrismMessageForPhase2 ())
        {
            // Enforce that we do not update Phase2 locations when a Phase2 message is not set.

        pWaveSendToClusterContext->setLocationsToSendToForPhase2 (succeededLocationsForPhase1);
    }
        else
        {
            // Do nothing
        }
    }

    for (i = 0; i < numberOfFailedLocationsForPhase1; i++)
    {
        tracePrintf (TRACE_LEVEL_DEVEL, true, false, "WaveObjectManager::computeFailedNodesForPhase1Step : failed Location Id %u", failedLocationsForPhase1[i]);
    }
    for (i = 0; i < numberOfDisconnectedLocationsForPhase1; i++)
    {
        tracePrintf (TRACE_LEVEL_DEVEL, false, false, "WaveObjectManager::computeFailedNodesForPhase1Step : disconnected location Id %u", disconnectedLocationsForPhase1[i]);
    }
    for (i = 0; i < numberOfSucceededLocationsForPhase1; i++)
    {
        tracePrintf (TRACE_LEVEL_DEVEL, true, false, "WaveObjectManager::computeFailedNodesForPhase1Step : succeeded location Id %u", succeededLocationsForPhase1[i]);
    }
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableStep : Entering ...");

    WaveSendToClusterContext *pWaveSendToClusterContext = reinterpret_cast<WaveSendToClusterContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);
    vector<LocationId>        locationsToSendToForPhase1          = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                      numberOfLocationsToSendToForPhase1  = locationsToSendToForPhase1.size ();
    UI32                      numberOfFailedLocationsForPhase1    = pWaveSendToClusterContext->getNumberOfFailuresForPhase1 ();
    UI32                      numberOfDisconnectedLocationsForPhase1    = pWaveSendToClusterContext->getNumberOfDisconnectedNodesForPhase1 ();

    vector<LocationId>        locationsToSendTo         = pWaveSendToClusterContext->getLocationsToSendToForPhase2 ();
    UI32                      numberOfLocationsToSendTo = locationsToSendTo.size ();
    UI32                      i                         = 0;
    PrismMessage             *pPrismMessageForPhase2    = pWaveSendToClusterContext->getPPrismMessageForPhase2 ();

    pPrismLinearSequencerContext->setNumberOfFailures (0);

    if (numberOfLocationsToSendToForPhase1 == (numberOfFailedLocationsForPhase1 + numberOfDisconnectedLocationsForPhase1))
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    if (NULL == pPrismMessageForPhase2)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    UI32                      timeoutForPhase2          = pWaveSendToClusterContext->getTimeoutForPhase2 ();

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfLocationsToSendTo; i++)
    {
        if (0 >= locationsToSendTo[i])
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableStep : incorrect location-id specified. %d", locationsToSendTo[i]);

            pPrismLinearSequencerContext->incrementNumberOfFailures ();
            pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
            return;
        }

        PrismMessage *pClonedPrismMessageForPhase2 = pPrismMessageForPhase2->clone ();

        prismAssert (NULL != pClonedPrismMessageForPhase2, __FILE__, __LINE__);

        WaveMessageStatus status = send (pClonedPrismMessageForPhase2, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableCallback), pPrismLinearSequencerContext, timeoutForPhase2, locationsToSendTo[i]);

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableStep : Location Id %u : Send Status : %s", locationsToSendTo[i], (FrameworkToolKit::localize (status)).c_str ());

        pWaveSendToClusterContext->setSendStatusForPhase2 (locationsToSendTo[i], status);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
            delete pClonedPrismMessageForPhase2;
        }
        else
        {
            ++(*pPrismLinearSequencerContext);
        }
    }

    --(*pPrismLinearSequencerContext);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    --(*pPrismLinearSequencerContext);

    WaveSendToClusterContext *pWaveSendToClusterContext = reinterpret_cast<WaveSendToClusterContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    LocationId                receiverLocationId        = pPrismMessage->getReceiverLocationId ();

    pWaveSendToClusterContext->setResultingMessageForPhase2 (receiverLocationId, pPrismMessage);

    pWaveSendToClusterContext->setFrameworkStatusForPhase2 (receiverLocationId, frameworkStatus);

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableCallback : Location Id %u : Framework Status : %s", receiverLocationId, (FrameworkToolKit::localize (frameworkStatus)).c_str ());

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }
    else
    {
        ResourceId completionStatus = pPrismMessage->getCompletionStatus ();

        pWaveSendToClusterContext->setCompletionStatusForPhase2 (receiverLocationId, completionStatus);

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager::sendPhase2MessageToAllNodesIfApplicableCallback : Location Id %u : Send Status : %s", receiverLocationId, (FrameworkToolKit::localize (completionStatus)).c_str ());

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
        }
        else
        {
        }
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::computeOverallStatusStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::computeOverallStatusStep : Entering ...");

    WaveSendToClusterContext *pWaveSendToClusterContext           = reinterpret_cast<WaveSendToClusterContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    vector<LocationId>        locationsToSendToForPhase1          = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();
    UI32                      numberOfLocationsToSendToForPhase1  = locationsToSendToForPhase1.size ();
    vector<LocationId>        locationsToSendToForPhase2          = pWaveSendToClusterContext->getLocationsToSendToForPhase2 ();
    UI32                      numberOfLocationsToSendToForPhase2  = locationsToSendToForPhase2.size ();
    UI32                      i                                   = 0;
    UI32                      numberOfFailedLocationsForPhase1    = pWaveSendToClusterContext->getNumberOfFailuresForPhase1 ();
    UI32                      numberOfDisconnectedLocationsForPhase1    = pWaveSendToClusterContext->getNumberOfDisconnectedNodesForPhase1 ();
    vector<LocationId>        failedLocationsForPhase2;
    UI32                      numberOfFailedLocationsForPhase2    = 0;
    vector<LocationId>        succeededLocationsForPhase2;
    UI32                      numberOfSucceededLocationsForPhase2 = 0;
    UI32                      failureCountFromContext             = pPrismLinearSequencerContext->getNumberOfFailures ();
    ResourceId                overAllCompletionStatus             = WAVE_MESSAGE_ERROR;

    PrismMessage             *pPrismMessageForPhase2              = pWaveSendToClusterContext->getPPrismMessageForPhase2 ();
    bool                      noRollBackFlag                      = pWaveSendToClusterContext->getTreatFailureOnFailingOverAsSuccessFlag ();

    if (NULL != pPrismMessageForPhase2)
    {
        for (i = 0; i < numberOfLocationsToSendToForPhase2; i++)
        {
            WaveMessageStatus sendStatus = pWaveSendToClusterContext->getSendStatusForPhase2 (locationsToSendToForPhase2[i]);

            if (WAVE_MESSAGE_SUCCESS != sendStatus)
            {
                failedLocationsForPhase2.push_back (locationsToSendToForPhase2[i]);
            }
            else
            {
                FrameworkStatus frameworkStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase2 (locationsToSendToForPhase2[i]);

                if (FRAMEWORK_SUCCESS != frameworkStatus)
                {
                    failedLocationsForPhase2.push_back (locationsToSendToForPhase2[i]);
                }
                else
                {
                    ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase2 (locationsToSendToForPhase2[i]);

                    if (WAVE_MESSAGE_SUCCESS != completionStatus)
                    {
                        failedLocationsForPhase2.push_back (locationsToSendToForPhase2[i]);
                    }
                    else
                    {
                        succeededLocationsForPhase2.push_back (locationsToSendToForPhase2[i]);
                    }
                }
            }
        }

        numberOfFailedLocationsForPhase2    = failedLocationsForPhase2.size ();
        numberOfSucceededLocationsForPhase2 = succeededLocationsForPhase2.size ();

        prismAssert (failureCountFromContext == numberOfFailedLocationsForPhase2, __FILE__, __LINE__);
        prismAssert (numberOfLocationsToSendToForPhase2 == (numberOfFailedLocationsForPhase2 + numberOfSucceededLocationsForPhase2), __FILE__, __LINE__);

        pWaveSendToClusterContext->setNumberOfFailuresForPhase2 (numberOfFailedLocationsForPhase2);
    }

    // Overall Completion status cases with new flag 'm_treatFailureOnFailingOverAsSuccessFlag' added.
    // 1. For m_treatFailureOnFailingOverAsSuccessFlag = 0, there will be no list for 'Disconnected' Nodes (handled in step 2 of this sequencer). All failures
    // will add nodes in vector for 'Failed' Nodes. So, In below table, Failed for 'm_treatFailureOnFailingOverAsSuccessFlag = 0' is equal to Failed+Disconnected.
    // 2. For m_treatFailureOnFailingOverAsSuccessFlag = 1, seperate vectors for Disconnected & Failed Nodes will be formed.
    // 3. For m_treatFailureOnFailingOverAsSuccessFlag = 1, Disconnected needs to be considered as Success.
    // Considering 'x' node cluster and a, b are nonzero numbers and a+b < x
    // -----------------------------------------------------------------------------------------------------------------------------------------|
    // Case | #Success |#Failed |#Disconnected | m_treatFailureOnFailingOverAsSuccessFlag = 0    |   m_treatFailureOnFailingOverAsSuccessFlag = 1               |
    // -----|----------|--------|--------------|-----------------------------------------|------------------------------------------------------|
    //  1   | x        | 0      | 0            |  WAVE_MESSAGE_SUCCESS                   |    WAVE_MESSAGE_SUCCESS                              |
    // -----|----------|--------|--------------|-----------------------------------------|------------------------------------------------------|
    //  2   | 0        | x      | 0            |  WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES |    WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES            |
    // -----|----------|--------|--------------|-----------------------------------------|------------------------------------------------------|
    //  3   | 0        | 0      | x            |  WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES |    WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES            |
    // -----|----------|--------|--------------|-----------------------------------------|------------------------------------------------------|
    //  4   | 0        | a      | x-a          |  WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES |    WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES            |
    // -----|----------|--------|--------------|-----------------------------------------|------------------------------------------------------|
    //  5   | a        | b      | x-a-b > 0    |  WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED/ |    WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED/            |
    //      |          |        |              |  WAVE_MESSAGE_ERROR_ROLLBACK_FAILED     |      WAVE_MESSAGE_ERROR_ROLLBACK_FAILED              |
    // -----|----------|--------|--------------|-----------------------------------------|------------------------------------------------------|
    //  6   | a        | 0      | x-a          |  WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED/ |    WAVE_MESSAGE_SUCCESS                              |
    //      |          |        |              |  WAVE_MESSAGE_ERROR_ROLLBACK_FAILED     |                                                      |
    // -----|----------|--------|--------------|-----------------------------------------|------------------------------------------------------|

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager:::computeOverallStatusStep : numberOfFailedLocationsForPhase1=%d, numberOfLocationsToSendToForPhase1=%d, numberOfDisconnectedLocationsForPhase1=%d, numberOfFailedLocationsForPhase2=%d, noRollBackFlag=%d", numberOfFailedLocationsForPhase1, numberOfLocationsToSendToForPhase1, numberOfDisconnectedLocationsForPhase1, numberOfFailedLocationsForPhase2, noRollBackFlag);

    if (0 == numberOfFailedLocationsForPhase1)
    {
        overAllCompletionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if ((numberOfFailedLocationsForPhase1 + numberOfDisconnectedLocationsForPhase1) == numberOfLocationsToSendToForPhase1)
        {
            overAllCompletionStatus = WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES;
        }
        else
        {
            if (true == pWaveSendToClusterContext->getPartialSuccessFlag ())
            {
                overAllCompletionStatus = WAVE_MESSAGE_SUCCESS;
            }
            else if (NULL == pPrismMessageForPhase2)
            {
                overAllCompletionStatus = WAVE_MESSAGE_ERROR_FAILED_ON_SOME_NODES;
            }
            else
            {
                if (0 == numberOfFailedLocationsForPhase2)
                {
                    overAllCompletionStatus = WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED;
                }
                else
                {
                    overAllCompletionStatus = WAVE_MESSAGE_ERROR_ROLLBACK_FAILED;
                }
            }
        }
    }


    // transfer propagated status back to input message

    if (NULL != m_pInputMessage)
    {
        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager:::computeOverallStatusStep : Propagating status to pInputMessage : %p", m_pInputMessage);
        m_pInputMessage->addClusterStatusPropagation (pWaveSendToClusterContext, overAllCompletionStatus);
    }

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "WaveObjectManager:::computeOverallStatusStep : overAllCompletionStatus = %s", (FrameworkToolKit::localize (overAllCompletionStatus)).c_str ());
    pPrismLinearSequencerContext->executeNextStep (overAllCompletionStatus);
}

ResourceId WaveObjectManager::addLog (ResourceId logType, ResourceId logDescriptionType, const vector<Attribute *> &logDescriptionArguments)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::addLog : Entering ...");

    DistributedLogAddLogEntryMessage   *pDistributedLogAddLogEntryMessage   = NULL;
    string                              logDescription                      = "";
    LogStatus                           addLogStatus                        = DISTRIBUTED_LOG_ERROR;
    WaveMessageStatus                   status                              = WAVE_MESSAGE_ERROR;


    if (false == FrameworkToolKit::isAResourceId (logType))
    {
        addLogStatus = DISTRIBUTED_LOG_INVALID_LOG_TYPE_DEFINED;

        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

        return (addLogStatus);
    }

    if (false == FrameworkToolKit::isAResourceId (logDescriptionType))
    {
        addLogStatus = DISTRIBUTED_LOG_INVALID_LOG_DESCRIPTION_TYPE_DEFINED;

        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

        return (addLogStatus);
    }

    switch (logType)
    {
        case DISTRIBUTED_LOG_TYPE_GENERIC_LOG :
        {
            if (DISTRIBUTED_LOG_SUB_TYPE_GENERIC_ENTRY != logDescriptionType)
            {
                addLogStatus = DISTRIBUTED_LOG_INVALID_LOG_DESCRIPTION_TYPE_FOR_GIVEN_LOG_TYPE;

                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

                return (addLogStatus);
            }

            logDescription = FrameworkToolKit::localize (logDescriptionType, logDescriptionArguments);
            pDistributedLogAddLogEntryMessage = new DistributedLogAddLogEntryMessage (logType, logDescriptionType, logDescription, getServiceId (), FrameworkToolKit::getThisWaveNodeObjectId ());

            break;
        }
        case DISTRIBUTED_LOG_TYPE_SERVICE_LOG :
        {
            if (DISTRIBUTED_LOG_SUB_TYPE_SERVICE_ENTRY != logDescriptionType)
            {
                addLogStatus = DISTRIBUTED_LOG_INVALID_LOG_DESCRIPTION_TYPE_FOR_GIVEN_LOG_TYPE;

                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

                return (addLogStatus);
            }

            logDescription = FrameworkToolKit::localize (logDescriptionType, logDescriptionArguments);
            pDistributedLogAddLogEntryMessage = new DistributedLogAddLogEntryMessage (logType, logDescriptionType, logDescription, getServiceId (), FrameworkToolKit::getThisWaveNodeObjectId ());

            break;
        }
        case DISTRIBUTED_LOG_TYPE_MANAGED_OBJECT_LOG :
        {
            if (DISTRIBUTED_LOG_SUB_TYPE_MANAGED_OBJECT_ENTRY != logDescriptionType)
            {
                addLogStatus = DISTRIBUTED_LOG_INVALID_LOG_DESCRIPTION_TYPE_FOR_GIVEN_LOG_TYPE;

                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

                return (addLogStatus);
            }

            // The managed object Id is separately stored in it's own column for easier query purposes.
            if (AttributeType::AttributeTypeObjectId == logDescriptionArguments[0]->getAttributeType ())
            {
                vector<Attribute *> logDescriptionArgumentsCopy = logDescriptionArguments;

                AttributeObjectId *pAttributeObjectId = dynamic_cast<AttributeObjectId *> (logDescriptionArgumentsCopy[0]);
                prismAssert (NULL != pAttributeObjectId, __FILE__, __LINE__);

                ObjectId managedObjectId = pAttributeObjectId->getValue ();

                logDescriptionArgumentsCopy.erase (logDescriptionArgumentsCopy.begin ());

                logDescription = FrameworkToolKit::localize (logDescriptionType, logDescriptionArgumentsCopy);
                pDistributedLogAddLogEntryMessage = new DistributedLogAddLogEntryMessage (logType, logDescriptionType, logDescription, getServiceId (), FrameworkToolKit::getThisWaveNodeObjectId (), managedObjectId);
            }
            else
            {
                addLogStatus = DISTRIBUTED_LOG_INVALID_ATTRIBUTE_TYPE;

                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

                return (addLogStatus);
            }

            break;
        }
        case DISTRIBUTED_LOG_TYPE_AUDIT_LOG :
        {
            if ((DISTRIBUTED_LOG_SUB_TYPE_SESSION_LOGIN != logDescriptionType) && (DISTRIBUTED_LOG_SUB_TYPE_SESSION_LOGOUT != logDescriptionType) && (DISTRIBUTED_LOG_SUB_TYPE_COMMAND != logDescriptionType) && (DISTRIBUTED_LOG_SUB_TYPE_COMMAND_CONFIGURATION != logDescriptionType) && (DISTRIBUTED_LOG_SUB_TYPE_COMMAND_OPERATION != logDescriptionType))
            {
                addLogStatus = DISTRIBUTED_LOG_INVALID_LOG_DESCRIPTION_TYPE_FOR_GIVEN_LOG_TYPE;

                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

                return (addLogStatus);
            }

            // The username is separately stored in it's own column for easier query purposes and excluded from the log description column.
            if (AttributeType::AttributeTypeString == logDescriptionArguments[0]->getAttributeType ())
            {
                vector<Attribute *> logDescriptionArgumentsCopy = logDescriptionArguments;

                AttributeString *pAttributeString = dynamic_cast<AttributeString *> (logDescriptionArgumentsCopy[0]);
                prismAssert (NULL != pAttributeString, __FILE__, __LINE__);
                string user = pAttributeString->getValue ();

                logDescriptionArgumentsCopy.erase (logDescriptionArgumentsCopy.begin ());

                logDescription = FrameworkToolKit::localize (logDescriptionType, logDescriptionArgumentsCopy);
                pDistributedLogAddLogEntryMessage = new DistributedLogAddLogEntryMessage (logType, logDescriptionType, logDescription, user, getServiceId (), FrameworkToolKit::getThisWaveNodeObjectId ());
            }
            else
            {
                addLogStatus = DISTRIBUTED_LOG_INVALID_ATTRIBUTE_TYPE;

                trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::addLog : ") + FrameworkToolKit::localize (addLogStatus));

                return (addLogStatus);
            }

            break;
        }
        default :
        {
            // Allow for user defined log types.  A generic log entry type will be used.
            logDescription = FrameworkToolKit::localize (DISTRIBUTED_LOG_SUB_TYPE_GENERIC_ENTRY, logDescriptionArguments);
            pDistributedLogAddLogEntryMessage = new DistributedLogAddLogEntryMessage (logType, logDescriptionType, logDescription, getServiceId (), FrameworkToolKit::getThisWaveNodeObjectId ());
        }
    }

    //trace (TRACE_LEVEL_DEVEL, string ("WaveObjectManager::addLog : LogType : ") + FrameworkToolKit::localize (logType) + ", LogDescription : " + logDescription);

    status = WaveObjectManager::sendOneWay (pDistributedLogAddLogEntryMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        addLogStatus = DISTRIBUTED_LOG_ERROR;

        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::addLog : Could not send message.  Status : " + FrameworkToolKit::localize (status));

        delete pDistributedLogAddLogEntryMessage;
    }
    else
    {
        addLogStatus = DISTRIBUTED_LOG_SUCCESS;
    }

    return (addLogStatus);
}

void WaveObjectManager::backendSyncUpHandler (WaveObjectManagerBackendSyncUpMessage *pWaveObjectManagerBackendSyncUpMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::backendSyncUpWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::backendSyncUpSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerBackendSyncUpMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::backendSyncUpWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::backendSyncUpWorkersStep : Entering ...");

    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::backendSyncUpWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->backendSyncUp (pPrismAsynchronousContext);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::backendSyncUpWorkersStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::backendSyncUpWorkersStepCallback: Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                 status                     = pPrismAsynchronousContext->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pPrismAsynchronousContext;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::: Sync up with back end for one of the service failed.");
        //prismAssert (false, __FILE__, __LINE__);

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::backendSyncUpSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::backendSyncUpSelfStep : Entering ...");

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::backendSyncUpSelfStepCallback), pPrismLinearSequencerContext);

    backendSyncUp (pPrismAsynchronousContext);
}

void WaveObjectManager::backendSyncUpSelfStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::backendSyncUpSelfStepCallback: Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                 status                     = pPrismAsynchronousContext->getCompletionStatus ();

    delete pPrismAsynchronousContext;

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::backendSyncUp: Entering ...");

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();

}


// This function is called through wrapper functions. Whether history state is on or off is checked in wrapper functions.
void WaveObjectManager::addMessageToMessageHistory (PrismMessage *pPrismMessage, WaveMessageHistoryLogType messageHistoryLogType)
{
    // do not log message history related messages.
    if ((WAVE_OBJECT_MANAGER_MESSAGE_HISTORY_CONFIG == pPrismMessage->getOperationCode ()) || (WAVE_OBJECT_MANAGER_MESSAGE_HISTORY_DUMP == pPrismMessage->getOperationCode ()))
    {
        return;
    }

    WaveMessageSendType sendType = WAVE_MESSAGE_SEND_TYPE_ASYNCHRONOUS;

    if (true == pPrismMessage->getIsSynchronousMessage ())
    {
        sendType = WAVE_MESSAGE_SEND_TYPE_SYNCHRONOUS;
    }
    else if (true == pPrismMessage->getIsOneWayMessage ())
    {
        sendType = WAVE_MESSAGE_SEND_TYPE_ONEWAY;
    }

    MessageHistory *pMessageHistoryObject   = new MessageHistory (pPrismMessage, sendType, messageHistoryLogType);

    if (m_messageHistoryNextSlotInVector < m_messageHistoryVector.size ())
    {
        delete m_messageHistoryVector[m_messageHistoryNextSlotInVector];

        m_messageHistoryVector[m_messageHistoryNextSlotInVector] = pMessageHistoryObject;
    }
    else
    {
        m_messageHistoryVector.push_back (pMessageHistoryObject);
    }

    m_messageHistoryNextSlotInVector++;

    if (m_messageHistoryNextSlotInVector >= m_messageHistoryMaxSize)
    {
        m_messageHistoryNextSlotInVector = 0;
    }

    // // debug print string
    // string debugString = "";
    // pMessageHistoryObject->getMessageHistoryString (debugString);
    // trace (TRACE_LEVEL_INFO, getName () + string (" :  Logged a message : ") + debugString);
}

void WaveObjectManager::addMessageToMessageHistoryCalledFromSend (PrismMessage *pPrismMessage)
{
    if ((true == m_messageHistoryState) && (true == m_messageHistoryLogInsideSend))
    {
        addMessageToMessageHistory (pPrismMessage, WAVE_MESSAGE_HISTORY_LOG_AT_SEND);
    }
}

void WaveObjectManager::addMessageToMessageHistoryCalledFromReply (PrismMessage *pPrismMessage)
{
    if ((true == m_messageHistoryState) && (true == m_messageHistoryLogInsideReply))
    {
        addMessageToMessageHistory (pPrismMessage, WAVE_MESSAGE_HISTORY_LOG_AT_REPLY);
    }
}

void WaveObjectManager::addMessageToMessageHistoryCalledFromHandle (PrismMessage *pPrismMessage)
{
    if ((true == m_messageHistoryState) && (true == m_messageHistoryLogInsideHandleMessage))
    {
        addMessageToMessageHistory (pPrismMessage, WAVE_MESSAGE_HISTORY_LOG_AT_HANDLE);
    }
}

void WaveObjectManager::restrictMessageHistoryLogging (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage)
{
    m_messageHistoryLogInsideSend           = messageHistoryLogInsideSend;
    m_messageHistoryLogInsideReply          = messageHistoryLogInsideReply;
    m_messageHistoryLogInsideHandleMessage  = messageHistoryLogInsideHandleMessage;
}

// This function will be called only when history-config-change command is issued at wave (debug) shell.
void WaveObjectManager::updateMessageHistoryConfig (bool requestedMessageHistoryState, UI32 requestedMessageHistoryMaxSize)
{
    m_messageHistoryState = requestedMessageHistoryState;

    // We can get a request to only change state, and RequestedHistoryMaxSize can be equal to default 0
    if (0 < requestedMessageHistoryMaxSize)
    {
        // resize current history vector if necessary.

        // case 1 :  m_messageHistoryMaxSize = requestedMessageHistoryMaxSize
        // Nothing needs to be done.

        // case 2 :  m_messageHistoryMaxSize != requestedMessageHistoryMaxSize
        // shuffle elements of m_messageHistoryVector if  currentMessageHistoryVectorSize == m_messageHistoryMaxSize
        // i.e. change m_messageHistoryVector from |4|5|6|0|1|2|3| to - |0|1|2|3|4|5|6|.
        //                                                ^              ^
        // and set next slot number to zero (e.g. change from 3 to 0)

        UI32 currentMessageHistoryVectorSize = m_messageHistoryVector.size ();

        if (currentMessageHistoryVectorSize == m_messageHistoryMaxSize)
        {
            vector<MessageHistory *> tempMessageHistoryVector;
            tempMessageHistoryVector.assign (m_messageHistoryVector.begin (), m_messageHistoryVector.begin () + m_messageHistoryNextSlotInVector);
            m_messageHistoryVector.erase (m_messageHistoryVector.begin (), m_messageHistoryVector.begin () + m_messageHistoryNextSlotInVector);
            m_messageHistoryVector.insert (m_messageHistoryVector.end (), tempMessageHistoryVector.begin (), tempMessageHistoryVector.end ());

            // set next slot number to 0
            m_messageHistoryNextSlotInVector = 0;
        }

        if (currentMessageHistoryVectorSize < requestedMessageHistoryMaxSize)
        {
            // case 2a :  currentMessageHistoryVectorSize < requestedMessageHistoryMaxSize
            // i.e. MessageHistory vector is not filled upto requested max size.
            // set next slot number to currentMessageHistoryVectorSize

            m_messageHistoryNextSlotInVector = currentMessageHistoryVectorSize;
        }
        else if (currentMessageHistoryVectorSize > requestedMessageHistoryMaxSize)
        {
            // case 2b :  currentMessageHistoryVectorSize > requestedMessageHistoryMaxSize
            // erase old entries.

            m_messageHistoryVector.erase (m_messageHistoryVector.begin (), m_messageHistoryVector.begin () + (currentMessageHistoryVectorSize - requestedMessageHistoryMaxSize));
            m_messageHistoryNextSlotInVector = 0;
        }
        else if (currentMessageHistoryVectorSize == requestedMessageHistoryMaxSize)
        {
            // case 2c :  currentMessageHistoryVectorSize == requestedMessageHistoryMaxSize
            // Nothing needs to be done.
            m_messageHistoryNextSlotInVector = 0;
        }

        // update m_MessageHistoryMaxSize to RequestedMessageHistoryMaxSize.
        m_messageHistoryMaxSize = requestedMessageHistoryMaxSize;
    }
    else
    {
        // (0 == RequestedHistoryMaxSize) means this message was sent to change state only.
    }
}

void WaveObjectManager::getMessageHistoryDumpStringVector (vector<string> &messageHistoryDumpStringVector)
{
    UI32 messageHistoryVectorSize = m_messageHistoryVector.size ();

    string messageHistoryDumpString = "";

    // |4|5|6|0|1|2|3| => get |0|1|2|3|
    for (UI32 i = m_messageHistoryNextSlotInVector; i < messageHistoryVectorSize; i++)
    {
        messageHistoryDumpString.clear ();

        m_messageHistoryVector[i]->getMessageHistoryString (messageHistoryDumpString);

        messageHistoryDumpStringVector.push_back (getName () + string (" - ") + messageHistoryDumpString);
    }

    // |4|5|6|0|1|2|3| => append |4|5|6|
    for (UI32 i = 0; i < m_messageHistoryNextSlotInVector; i++)
    {
        messageHistoryDumpString.clear ();

        m_messageHistoryVector[i]->getMessageHistoryString (messageHistoryDumpString);

        messageHistoryDumpStringVector.push_back (getName () + string (" - ") + messageHistoryDumpString);
    }
}

void WaveObjectManager::waveObjectManagerMessageHistoryConfigMessageHandler (WaveObjectManagerMessageHistoryConfigMessage *pWaveObjectManagerMessageHistoryConfigMessage)
{
    bool requestedHistoryState   = pWaveObjectManagerMessageHistoryConfigMessage->getMessageHistoryState ();

    UI32 requestedHistoryMaxSize = pWaveObjectManagerMessageHistoryConfigMessage->getMessageHistoryMaxSize ();

    updateMessageHistoryConfig (requestedHistoryState, requestedHistoryMaxSize);

    pWaveObjectManagerMessageHistoryConfigMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pWaveObjectManagerMessageHistoryConfigMessage);
}

void WaveObjectManager::waveObjectManagerMessageHistoryDumpMessageHandler (WaveObjectManagerMessageHistoryDumpMessage *pWaveObjectManagerMessageHistoryDumpMessage)
{
    vector<string> messageHistoryDumpStringVector;

    getMessageHistoryDumpStringVector (messageHistoryDumpStringVector);

    pWaveObjectManagerMessageHistoryDumpMessage->setMessageHistoryDumpStringVector (messageHistoryDumpStringVector);

    pWaveObjectManagerMessageHistoryDumpMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pWaveObjectManagerMessageHistoryDumpMessage);
}

void WaveObjectManager::registerLock (const string &serviceString)
{
    PrismServiceId          prismServiceId = getServiceId ();

    //Populate the global map with serviceString and the prismServiceId

    m_serviceStringServiceIdMapMutex.lock ();

    if (m_serviceStringServiceIdMap.end () == WaveObjectManager::m_serviceStringServiceIdMap.find (serviceString))
    {
        m_serviceStringServiceIdMap[serviceString] = prismServiceId;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveObjectManager::registerLock: Lock already registered for serviceId =") + serviceString);
        prismAssert (false, __FILE__, __LINE__);
    }

    m_serviceStringServiceIdMapMutex.unlock ();

    //Populate the local set as well

    m_serviceStrings.insert (serviceString);
}

ResourceId WaveObjectManager::acquireLock (const string &serviceString)
{
    ResourceId status               = WAVE_MESSAGE_SUCCESS;
    ResourceId completionStatus     = WAVE_MESSAGE_ERROR;

    if (isServiceStringRegisteredWithService (serviceString))
    {
        LockManagementObjectManagerAcquireLockMessage *pMessage = new LockManagementObjectManagerAcquireLockMessage (serviceString);
        status = sendSynchronously (pMessage);
        completionStatus = pMessage->getCompletionStatus ();
        delete pMessage;

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::acquireLock : error sending message to LockManagementObjectManager");
        }
        else if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_SUCCESS, "WaveObjectManager::acquireLock : acquire lock message returned success");
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::acquireLock : acquire lock message returned error");
            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::acquireLock : Service has not registered the serviceString = ") + serviceString);
    }

    return (status);
}

ResourceId WaveObjectManager::releaseLock (const string &serviceString)
{
    ResourceId status               = WAVE_MESSAGE_SUCCESS;
    ResourceId completionStatus     = WAVE_MESSAGE_ERROR;

    if (isServiceStringRegisteredWithService (serviceString))
    {
        LockManagementObjectManagerReleaseLockMessage *pMessage = new LockManagementObjectManagerReleaseLockMessage (serviceString);
        status = sendSynchronously (pMessage);
        completionStatus = pMessage->getCompletionStatus ();
        delete pMessage;

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::releaseLock : error sending message to LockManagementObjectManager");
        }
        else if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_SUCCESS, "WaveObjectManager::releaseLock : release lock message returned success");
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::releaseLock : release lock message returned error");
            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::releaseLock : Service has not registered the serviceString = ") + serviceString);
    }

    return (status);
}

/**
 * Name:        updateHardwareSynchronizationState
 * Description: API to update the hardware synchronization state for one to many nodes.
 *              This attribute a part of WaveNode local MO and owned by CentralClusterConfigOM.
 *
 * @param1      hardwareSynchronizationState : This is a resourceId defined in WaveFrameworkCore.xml
 * @param2      locationIds                  : vector of location ids that will get updated the the above
 *                                             hardware sync state.
 *
 * @return      ResourceId : WAVE_MESSAGE_SUCCES if successful.
 */
ResourceId WaveObjectManager::updateHardwareSynchronizationState (ResourceId hardwareSynchronizationState, const vector<LocationId> &locationIds)
{
    UI32        numberOfLocations   = locationIds.size ();
    ResourceId  status              = WAVE_MESSAGE_ERROR;

    if (0 == numberOfLocations)
    {
        trace (TRACE_LEVEL_FATAL, "WaveObjectManager::updateHardwareSynchronizationState : No locations detected.  Please make sure to populate the locationIds vector with some locations before calling this API.");
        prismAssert (false, __FILE__, __LINE__);
    }


    CentralClusterConfigUpdateHardwareSynchronizationStateMessage *pCentralClusterConfigUpdateHardwareSynchronizationStateMessage = new CentralClusterConfigUpdateHardwareSynchronizationStateMessage (hardwareSynchronizationState, locationIds);

    status = sendOneWay (pCentralClusterConfigUpdateHardwareSynchronizationStateMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::updateHardwareSynchronizationState : Could not send message.  Status : " + FrameworkToolKit::localize (status));

        delete pCentralClusterConfigUpdateHardwareSynchronizationStateMessage;
    }

    return (status);
}

ResourceId WaveObjectManager::updateHardwareSynchronizationState (ResourceId hardwareSynchronizationState, LocationId locationId)
{
    vector<LocationId> locationIds;


    // Get this nodes location id if one has not been set

    if (0 == locationId)
    {
        locationId = FrameworkToolKit::getThisLocationId ();
    }

    locationIds.push_back (locationId);

    return (updateHardwareSynchronizationState (hardwareSynchronizationState, locationIds));
}

WaveObjectManagerStatisticsTracker *WaveObjectManager::getPWaveObjectManagerStatisticsTracker () const
{
    return (m_pWaveObjectManagerStatisticsTracker);
}

bool WaveObjectManager::isServiceStringRegisteredWithService (const string &serviceString)
{
    PrismServiceId          prismServiceId = getServiceId ();
    bool                    serviceRegistered = true;

    m_serviceStringServiceIdMapMutex.lock ();

    map <string, PrismServiceId>::iterator it = WaveObjectManager::m_serviceStringServiceIdMap.find (serviceString);

    if (m_serviceStringServiceIdMap.end () != it)
    {
        if (prismServiceId != it->second)
        {
            serviceRegistered = false;
        }
    }
    else
    {
        serviceRegistered = false;
    }

    m_serviceStringServiceIdMapMutex.unlock ();

    return (serviceRegistered);
}

void WaveObjectManager::databaseSanityCheckHandler (WaveObjectManagerDatabaseSanityCheckMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::checkBasicDatabaseSanityWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::checkBasicDatabaseSanitySelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::checkIncorrectEntriesWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::checkIncorrectEntriesSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::checkBasicDatabaseSanityWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkBasicDatabaseSanityWorkersStep: Entering ...");

    WaveObjectManagerDatabaseSanityCheckMessage *pWaveObjectManagerDatabaseSanityCheckMessage = reinterpret_cast<WaveObjectManagerDatabaseSanityCheckMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;
    ResourceId status    = WAVE_MESSAGE_SUCCESS;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::checkBasicDatabaseSanityWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForBootPhases->setBootReason (pWaveObjectManagerDatabaseSanityCheckMessage->getBootReason ());
        ++(*pPrismLinearSequencerContext);
        m_workers[i]->dbBasicSanityCheck (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::checkBasicDatabaseSanityWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkBasicDatabaseSanityWorkersStepCallback: Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::checkBasicDatabaseSanityWorkersStepCallback: DBSanityCheck one worker failed.");

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
        //prismAssert (false, __FILE__, __LINE__);
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::checkBasicDatabaseSanitySelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkBasicDatabaseSanitySelfStep : Entering ...");

    WaveObjectManagerDatabaseSanityCheckMessage *pWaveObjectManagerDatabaseSanityCheckMessage = reinterpret_cast<WaveObjectManagerDatabaseSanityCheckMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::checkBasicDatabaseSanitySelfStepCallback), pPrismLinearSequencerContext);
    pWaveAsynchronousContextForBootPhases->setBootReason (pWaveObjectManagerDatabaseSanityCheckMessage->getBootReason ());

    dbBasicSanityCheck (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::checkBasicDatabaseSanitySelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkBasicDatabaseSanitySelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::dbBasicSanityCheck (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::checkIncorrectEntriesWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkIncorrectEntriesWorkersStep : Entering ...");

    WaveObjectManagerDatabaseSanityCheckMessage *pWaveObjectManagerDatabaseSanityCheckMessage = reinterpret_cast<WaveObjectManagerDatabaseSanityCheckMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    UI32 numberOfWorkers = m_workers.size ();
    UI32 i               = 0;
    ResourceId status    = WAVE_MESSAGE_SUCCESS;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::checkIncorrectEntriesWorkersStepCallback), pPrismLinearSequencerContext);

        pWaveAsynchronousContextForBootPhases->setBootReason (pWaveObjectManagerDatabaseSanityCheckMessage->getBootReason ());
        ++(*pPrismLinearSequencerContext);
        m_workers[i]->dbInconsistencyCheck (pWaveAsynchronousContextForBootPhases);
    }

    --(*pPrismLinearSequencerContext);
    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::checkIncorrectEntriesWorkersStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkIncorrectEntriesWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForBootPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::checkIncorrectEntriesWorkersStepCallback : DBInconsistencyCheck one worker failed.");

        pPrismLinearSequencerContext->incrementNumberOfFailures ();
        //prismAssert (false, __FILE__, __LINE__);
    }

    if (0 != pPrismLinearSequencerContext->getNumberOfFailures ())
    {
        status = WAVE_MESSAGE_ERROR;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::checkIncorrectEntriesSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkIncorrectEntriesSelfStep : Entering ...");

    WaveObjectManagerDatabaseSanityCheckMessage *pWaveObjectManagerDatabaseSanityCheckMessage = reinterpret_cast<WaveObjectManagerDatabaseSanityCheckMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases = new WaveAsynchronousContextForBootPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::checkIncorrectEntriesSelfStepCallback), pPrismLinearSequencerContext);
    pWaveAsynchronousContextForBootPhases->setBootReason (pWaveObjectManagerDatabaseSanityCheckMessage->getBootReason ());

    dbInconsistencyCheck (pWaveAsynchronousContextForBootPhases);
}

void WaveObjectManager::checkIncorrectEntriesSelfStepCallback (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::checkIncorrectEntriesSelfStep : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForBootPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForBootPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForBootPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

/*
 * This method can be overridden to check if the MOs contents are actually proper after an abrupt reboot.(During Persistent boot mode)
 * It can check if the MO has zero entries but we expect it to be non-zero.
 * It can check if any duplicate entries exist due to DB corruption.
 * If bootReason is PERSISTEN_WITH_DEFAULT and emptyOnPersistentWithDefault, no need to check for 0 entries
 */
void WaveObjectManager::dbInconsistencyCheck (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveObjectManager::zeroizeHandler (PrismZeroizeObjectManagerMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::zeroizeWorkersStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::zeroizeSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::zeroizeWorkersStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::zeroizeWorkersStep : Entering ...");

    UI32 numberOfWorkers                            = m_workers.size ();
    UI32 i                                          = 0;

    ++(*pPrismLinearSequencerContext);

    for (i = 0; i < numberOfWorkers; i++)
    {
        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::zeroizeWorkersStepCallback), pPrismLinearSequencerContext);

        ++(*pPrismLinearSequencerContext);
        m_workers[i]->zeroize (pWaveAsynchronousContextForShutDownPhases);
    }

    --(*pPrismLinearSequencerContext);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveObjectManager::zeroizeWorkersStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::zeroizeWorkersStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                 status                     = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    --(*pPrismLinearSequencerContext);

    delete pWaveAsynchronousContextForShutDownPhases;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_DEBUG, "WaveObjectManager::zeroizeWorkersStepCallback : zeroizing a worker failed.");
        //FIXME: Not sure if we need to assert here.
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::zeroizeSelfStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::zeroizeSelfStep : Entering ...");

        WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases = new WaveAsynchronousContextForShutDownPhases (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveObjectManager::zeroizeSelfStepCallback), pPrismLinearSequencerContext);

    zeroize (pWaveAsynchronousContextForShutDownPhases);
}

void WaveObjectManager::zeroizeSelfStepCallback (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::zeroizeSelfStepCallback : Entering ...");

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveAsynchronousContextForShutDownPhases->getPCallerContext ());
    ResourceId                   status                       = pWaveAsynchronousContextForShutDownPhases->getCompletionStatus ();

    delete pWaveAsynchronousContextForShutDownPhases;
    pPrismLinearSequencerContext->executeNextStep (status);
}

void WaveObjectManager::zeroize (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveObjectManager::zeroize: Entering ...");

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void WaveObjectManager::deleteAllManagedObjectInstances (const string &className)
{
    if (true == m_isTransactionOn)
    {
        if (false == (isManagedClassSupported (className)))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteAllManagedObjectInstances : This Object Manager " + m_name + " does not support Managed Object of type " + className);
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (true == OrmRepository::isManagedClassAView (className))
        {
            trace (TRACE_LEVEL_FATAL, "WaveObjectManager::deleteAllManagedObjectInstances : Managed View " + className + "is not allowed in a transaction.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            m_managedObjectsDeletedAtTheBeginingOfTransaction.push_back (className);
        }
    }

}

void WaveObjectManager::updateTimeConsumedInThisThread (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds)
{
    m_secondsForMessageHandlerSequencerSteps[operationCode][sequencerStepIndex]     += numberOfSeconds;
    m_nanoSecondsForMessageHandlerSequencerSteps[operationCode][sequencerStepIndex] += numberOfNanoSeconds;
}

void WaveObjectManager::updateRealTimeConsumedInThisThread (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds)
{
    m_realSecondsForMessageHandlerSequencerSteps[operationCode][sequencerStepIndex]     += numberOfSeconds;
    m_realNanoSecondsForMessageHandlerSequencerSteps[operationCode][sequencerStepIndex] += numberOfNanoSeconds;
}

void WaveObjectManager::clearAllTimingsAccumulatedForSequencerSteps ()
{
    m_secondsForMessageHandlerSequencerSteps.clear         ();
    m_nanoSecondsForMessageHandlerSequencerSteps.clear     ();
    m_realSecondsForMessageHandlerSequencerSteps.clear     ();
    m_realNanoSecondsForMessageHandlerSequencerSteps.clear ();
}

void WaveObjectManager::getAllTimingsAccumulatedForSequencerSteps (string &allTimingsAccumulatedForSequencerSteps)
{
    static char *pBuffer                                         = new char[512];
           UI32  numberOfMessageHandlersForSecondsAndNanoSeconds = m_secondsForMessageHandlerSequencerSteps.size ();

    if (0 != numberOfMessageHandlersForSecondsAndNanoSeconds)
    {

        snprintf (pBuffer, 512, "%-10s : %-10s : %-20s : %-20s\r\n", "_________", "________", "____________________", "____________________");
        allTimingsAccumulatedForSequencerSteps += pBuffer;
        snprintf (pBuffer, 512, "%-10s : %-10s : %-20s : %-20s\r\n", "Operation", "Step", "Thread", "Real");
        allTimingsAccumulatedForSequencerSteps += pBuffer;
        snprintf (pBuffer, 512, "%-10s : %-10s : %-20s : %-20s\r\n", "_________", "________", "____________________", "____________________");

        allTimingsAccumulatedForSequencerSteps += pBuffer;

        //UI32 numberOfSequencerSteps = 0;

        map<UI32, map<UI32, UI64> >::const_iterator elementForOperationCodeForSeconds    = m_secondsForMessageHandlerSequencerSteps.begin ();
        map<UI32, map<UI32, UI64> >::const_iterator endElementForOperationCodeForSeconds = m_secondsForMessageHandlerSequencerSteps.end   ();

        while (endElementForOperationCodeForSeconds != elementForOperationCodeForSeconds)
        {
            UI32 operationCode = elementForOperationCodeForSeconds->first;

            map<UI32, UI64>::const_iterator elementForSequencerStepForSeconds    = (elementForOperationCodeForSeconds->second).begin ();
            map<UI32, UI64>::const_iterator endElementForSequencerStepForSeconds = (elementForOperationCodeForSeconds->second).end   ();

            while (endElementForSequencerStepForSeconds != elementForSequencerStepForSeconds)
            {
                UI32 sequencerStep = elementForSequencerStepForSeconds->first;

                SI64 totalNanoSeconds     = m_secondsForMessageHandlerSequencerSteps[operationCode][sequencerStep]     *  1000000000 + m_nanoSecondsForMessageHandlerSequencerSteps[operationCode][sequencerStep];
                SI64 totalRealNanoSeconds = m_realSecondsForMessageHandlerSequencerSteps[operationCode][sequencerStep] *  1000000000 + m_realNanoSecondsForMessageHandlerSequencerSteps[operationCode][sequencerStep];

                snprintf (pBuffer, 512, "%010u : %05u : %20.9f : %20.9f\r\n", operationCode, sequencerStep, ((double) totalNanoSeconds) / (1000000000.0F), ((double) totalRealNanoSeconds) / (1000000000.0F));

                allTimingsAccumulatedForSequencerSteps += pBuffer;

                elementForSequencerStepForSeconds++;
            }

            allTimingsAccumulatedForSequencerSteps += "\r\n";

            elementForOperationCodeForSeconds++;
        }
    }
}

bool WaveObjectManager::isManagedObjectBeingTrackedForCreations (const string &managedObject)
{
    m_clientsListeningMutex.lock ();

    bool isTracked = false;

    map<string, map<string, string> >::const_iterator element    = m_clientsListeningForCreateByManagedObject.find (managedObject);
    map<string, map<string, string> >::const_iterator endElement = m_clientsListeningForCreateByManagedObject.end  ();

    if (endElement != element)
    {
        isTracked = true;
    }

    m_clientsListeningMutex.unlock ();

    return (isTracked);
}

void WaveObjectManager::addClientListeningForManagedObjectCreate (const string &waveManagedObjectName, const string &waveClientName)
{
    m_clientsListeningMutex.lock ();

    // Add the entry to map by managed object name.

    m_clientsListeningForCreateByManagedObject[waveManagedObjectName][waveClientName] = waveClientName;

    // Add the entry to map by client name.

    m_managedObjectsForCreateByClient[waveClientName][waveManagedObjectName] = waveManagedObjectName;

    m_clientsListeningMutex.unlock ();
}

WaveWorker *WaveObjectManager::getPWaveManagedObjectCreateWorker ()
{
    return (m_pWaveManagedObjectCreateWorker);
}


void WaveObjectManager::addPartitionNameToSetOfPartitionNamesReferencedInCurrentTransaction(const string &partitionName)
{
    // prismAssert (false == partitionName.empty (), __FILE__, __LINE__);

    if (false == partitionName.empty ())
    {
        if (true == isTransactionInProgress())
        {
            m_setOfPartitionNamesReferencedInCurrentTransaction.insert(partitionName);
        }
    }
}

set<string>  &WaveObjectManager::getSetOfPartitionNamesReferencedInCurrentTransaction(void)
{
    return (m_setOfPartitionNamesReferencedInCurrentTransaction);
}

ResourceId WaveObjectManager::sendOneWayForStoringConfigurationIntent (PrismMessage *pPrismMessage)
{
    PrismThread    *pHaPeerTransportPrismThread                 = PrismThread::getPrismThreadForMessageHaPeerTransport ();
    UI32            prismMessageId                              = pPrismMessage->getMessageId ();
    string          serializedConfigurationIntentMessage        = "";

    if (NULL == pHaPeerTransportPrismThread)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendOneWayForStoringConfigurationIntent : Ha Peer Service is not registered to accept configuration intents."));
        return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE);
    }

    if (false == (pHaPeerTransportPrismThread->hasWaveObjectManagers ()))
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::sendOneWayForStoringConfigurationIntent : Service identified.  But Ha Peer Transport in not registered to process any kind of requests.");
        return (WAVE_MESSAGE_ERROR_NO_OMS_FOR_SERVICE);
    }

    FrameworkObjectManagerStoreConfigurationIntentMessage *pStoreConfigurationIntentMessage = new FrameworkObjectManagerStoreConfigurationIntentMessage ();
    prismAssert (NULL != pStoreConfigurationIntentMessage, __FILE__, __LINE__);

    pStoreConfigurationIntentMessage->setConfigurationIntentMessageId (prismMessageId);

    pPrismMessage->serialize2 (serializedConfigurationIntentMessage);

    pStoreConfigurationIntentMessage->addBuffer (SERIALIZED_CONFIGURATION_INTENT_BUFFER, serializedConfigurationIntentMessage.size (), (void *) (serializedConfigurationIntentMessage.c_str ()), false);

    //trace (TRACE_LEVEL_DEBUG, string ("WaveObjectManager::sendOneWayForStoringConfigurationIntent : Configuration intent message Id : ") + pStoreConfigurationIntentMessage->getConfigurationIntentMessageId () + ", Configuration intent serialized message : " + serializedConfigurationIntentMessage + ", serialized message size : " + serializedConfigurationIntentMessage.size ());

    pStoreConfigurationIntentMessage->setIsOneWayMessage    (true);
    pStoreConfigurationIntentMessage->m_senderServiceCode   = getServiceId ();
    pStoreConfigurationIntentMessage->m_receiverLocationId  = 1;

    addMessageToMessageHistoryCalledFromSend (pStoreConfigurationIntentMessage);

    WaveMessageStatus status = pHaPeerTransportPrismThread->submitMessage (pStoreConfigurationIntentMessage);

    return (status);
}

ResourceId WaveObjectManager::sendOneWayForRemovingConfigurationIntent (const UI32 &configurationIntentMessageId)
{
    PrismThread    *pHaPeerTransportPrismThread                 = PrismThread::getPrismThreadForMessageHaPeerTransport ();

    if (NULL == pHaPeerTransportPrismThread)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::sendOneWayForRemovingConfigurationIntent : Ha Peer Service is not registered to accept configuration intents."));
        return (WAVE_MESSAGE_ERROR_NO_SERVICE_TO_ACCEPT_MESSAGE);
    }

    if (false == (pHaPeerTransportPrismThread->hasWaveObjectManagers ()))
    {
        trace (TRACE_LEVEL_ERROR, "WaveObjectManager::sendOneWayForRemovingConfigurationIntent : Service identified.  But Ha Peer Transport in not registered to process any kind of requests.");
        return (WAVE_MESSAGE_ERROR_NO_OMS_FOR_SERVICE);
    }

    FrameworkObjectManagerRemoveConfigurationIntentMessage *pRemoveConfigurationIntentMessage = new FrameworkObjectManagerRemoveConfigurationIntentMessage ();
    prismAssert (NULL != pRemoveConfigurationIntentMessage, __FILE__, __LINE__);

    pRemoveConfigurationIntentMessage->setConfigurationIntentMessageId (configurationIntentMessageId);

    pRemoveConfigurationIntentMessage->setIsOneWayMessage    (true);
    pRemoveConfigurationIntentMessage->m_senderServiceCode   = getServiceId ();
    pRemoveConfigurationIntentMessage->m_receiverLocationId  = 1;

    addMessageToMessageHistoryCalledFromSend (pRemoveConfigurationIntentMessage);

    WaveMessageStatus status = pHaPeerTransportPrismThread->submitMessage (pRemoveConfigurationIntentMessage);

    return (status);
}

void WaveObjectManager::sendMulticast (WaveSendMulticastContext *pWaveSendMulticastContext)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::performSendMulticast),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveSendMulticastContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveObjectManager::performSendMulticast (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveSendMulticastContext       *pWaveSendMulticastContext = dynamic_cast<WaveSendMulticastContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());
    vector<LocationId>              locationsToSend           = pWaveSendMulticastContext->getAllLocationsToSent ();
    LocationId                      thisLocationId            = FrameworkToolKit::getThisLocationId ();
    UI32                            numberOfLocations         = locationsToSend.size ();
    bool                            isThisLocationInvolved    = false;
    ResourceId                      status                    = WAVE_MESSAGE_SUCCESS;
    PrismMessage                   *pPrismMessage             = pWaveSendMulticastContext->getPrismMessage ();
    set<LocationId>                 remoteLocations;

    for (UI32 i = 0; i < numberOfLocations; i++)
    {
        ResourceId  unknownStatus = WAVE_MESSAGE_SENT_STATUS_TO_LOCATION_IS_UNKONWN;

        pWaveSendMulticastContext->setStatusForALocation (locationsToSend [i], unknownStatus);

        if (thisLocationId == locationsToSend [i])
        {
            isThisLocationInvolved = true;
        }
        else
        {
            remoteLocations.insert (locationsToSend [i]);
        }
    }

    ++(*pPrismLinearSequencerContext);

    pPrismMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    if (0 != remoteLocations.size ())
    {
        // Set the multicast context before sending to remote locations
        InterLocationMulticastMessage *pInterLocationMulticastMessage = new InterLocationMulticastMessage ();

        pPrismMessage->serialize2 (pInterLocationMulticastMessage->getSerializedStringToSend ());

        pInterLocationMulticastMessage->setLocationsToSend (remoteLocations);
        pInterLocationMulticastMessage->setMessageIdForMessageToMulticast (pPrismMessage->getMessageId ());

        status = send (pInterLocationMulticastMessage, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::performSendMulticastRemoteCallback), pPrismLinearSequencerContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == status)
            {
                ResourceId locationStatus = WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION;

                set<LocationId>::iterator locationIdIter = remoteLocations.begin ();
                while (locationIdIter != remoteLocations.end ())
                {
                    LocationId locationId = *locationIdIter;
                    pWaveSendMulticastContext->setStatusForALocation (locationId, locationStatus);
                    locationIdIter++;
                }
            }
            else
            {
                prismAssert (false, __FILE__, __LINE__);
            }

            ResourceId overallStatus = WAVE_MESSAGE_ERROR_SEND_MULTICAST_FAILED;
            pWaveSendMulticastContext->setMulticastStatus (overallStatus);
        }
        else
        {
            ++(*pPrismLinearSequencerContext);
        }
    }

    if (true == isThisLocationInvolved)
    {
        status = send (pPrismMessage, reinterpret_cast<PrismMessageResponseHandler> (&WaveObjectManager::performSendMulticastLocalCallback), pPrismLinearSequencerContext);

        prismAssert (WAVE_MESSAGE_SUCCESS == status, __FILE__, __LINE__);

        ++(*pPrismLinearSequencerContext);
    }

    --(*pPrismLinearSequencerContext);

    pPrismLinearSequencerContext->executeNextStep (pWaveSendMulticastContext->getOverallMulticastStatus ());
}

void WaveObjectManager::performSendMulticastLocalCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pContext);
    WaveSendMulticastContext    *pWaveSendMulticastContext    = dynamic_cast<WaveSendMulticastContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());

    --(*pPrismLinearSequencerContext);

    prismAssert (FRAMEWORK_SUCCESS == frameworkStatus, __FILE__, __LINE__);

    ResourceId  completionstatus = pPrismMessage->getCompletionStatus ();
    LocationId  thisLocationId   = FrameworkToolKit::getThisLocationId ();

    pWaveSendMulticastContext->setStatusForALocation (thisLocationId, completionstatus);

    ResourceId completionStatus = pPrismMessage->getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != completionStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::performSendMulticastLocalCallback : Received Error ") + completionStatus + string (" for location id ") + thisLocationId);

        ResourceId overallStatus = WAVE_MESSAGE_ERROR_SEND_MULTICAST_FAILED;

        pWaveSendMulticastContext->setMulticastStatus (overallStatus);
    }

    pPrismLinearSequencerContext->executeNextStep (pWaveSendMulticastContext->getOverallMulticastStatus ());
}

void WaveObjectManager::performSendMulticastRemoteCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    PrismLinearSequencerContext    *pPrismLinearSequencerContext    = reinterpret_cast<PrismLinearSequencerContext *> (pContext);
    WaveSendMulticastContext       *pWaveSendMulticastContext       = dynamic_cast<WaveSendMulticastContext *> (pPrismLinearSequencerContext->getPPrismAsynchronousContext ());
    InterLocationMulticastMessage  *pInterLocationMulticastMessage  = dynamic_cast<InterLocationMulticastMessage *> (pPrismMessage);
    set<LocationId>::iterator       locationIterator;

    prismAssert (NULL != pInterLocationMulticastMessage, __FILE__, __LINE__);
    prismAssert (FRAMEWORK_SUCCESS == frameworkStatus, __FILE__, __LINE__);

    set<LocationId>                 remoteLocationsId;

    --(*pPrismLinearSequencerContext);

    pInterLocationMulticastMessage->getLocationsToSend (remoteLocationsId);

    for (locationIterator = remoteLocationsId.begin (); locationIterator != remoteLocationsId.end (); locationIterator++)
    {
        LocationId locationId   = *locationIterator;
        ResourceId status       = pInterLocationMulticastMessage->getStatusForALocation (locationId);

        pWaveSendMulticastContext->setStatusForALocation (locationId, status);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("WaveObjectManager::performSendMulticastRemoteCallback: Received Error ") + status + string (" for location id ") + locationId);

            ResourceId overallStatus = WAVE_MESSAGE_ERROR_SEND_MULTICAST_FAILED;

            pWaveSendMulticastContext->setMulticastStatus (overallStatus);
        }
    }

    delete pInterLocationMulticastMessage;
    pInterLocationMulticastMessage = NULL;

    pPrismLinearSequencerContext->executeNextStep (pWaveSendMulticastContext->getOverallMulticastStatus ());
}

void WaveObjectManager::getAllOwnedManagedObjectClassNames (vector<string> &ownedManagedObjectClassNames)  const
{
    map<string, string>::const_iterator itr = m_managedClasses.begin ();
    map<string, string>::const_iterator end = m_managedClasses.end ();

    for (; itr != end; itr++)
    {
        ownedManagedObjectClassNames.push_back (itr->first);
    }
}

void WaveObjectManager::postponeMessageHandling (PrismMessage *pPrismMessage)
{
    //Always insert at the front so that when we get from the front and populate into the prism queue we are in the correct order.
    m_postponedMessageQueue.insertAtTheFront (pPrismMessage);
}

void WaveObjectManager::resumeAllPostponedMessages ()
{
    if (false == (m_postponedMessageQueue.isEmpty ()))
    {
        if (WAVE_MUTEX_SUCCESS == (m_postponedMessageQueueMutex.tryLock ()))
        {
            PrismMessage *pPrismMessage = m_postponedMessageQueue.removeAndGetFromFront ();

            do
            {
                m_pAssociatedPrismThread->submitMessageAtFront (pPrismMessage);
                pPrismMessage = m_postponedMessageQueue.removeAndGetFromFront ();
            }
            while (pPrismMessage != NULL);

            m_postponedMessageQueueMutex.unlock ();
        }
    }
}

void WaveObjectManager::checkMessageAttributesInSerialization ()
{
    // 1. iterate through each of the operationCode.
    // 2. Create a Message instance for each of operationCode.
    // 3. call setupAttributesForSerialization for each of the message instance.

    UI32            operationCode   = 0;
    PrismMessage*   pPrismMessage   = NULL;

    m_createMessageInstanceWrapperMutex.lock ();

    map<UI32, PrismElement *>::iterator element    = m_ownersForCreatingMessageInstances.begin ();
    map<UI32, PrismElement *>::iterator endElement = m_ownersForCreatingMessageInstances.end ();

    while (element != endElement)
    {
        operationCode   = element->first;

        pPrismMessage = (m_ownersForCreatingMessageInstances[operationCode])->createMessageInstance (operationCode);


        if (NULL == pPrismMessage)
        {
            trace (TRACE_LEVEL_WARN, string ("WaveObjectManager::checkMessageAttributesInSerialization : Owner for \"") + operationCode + string ("\" has not implemented dynamically creating the instance of this Message Type.  Implement this functionality to proceed further."));
        }
        else
        {
            pPrismMessage->setupAttributesForSerialization ();

            trace (TRACE_LEVEL_INFO, string ("WaveObjectManager::checkMessageAttributesInSerialization : message for [") + operationCode + string ("] is created and setupAttributesForSerialization is successful for the same."));

            delete pPrismMessage;
            pPrismMessage = NULL;
        }

        element++;
    }

    m_createMessageInstanceWrapperMutex.unlock ();
}

ResourceId WaveObjectManager::blockCli (const CliBlockContext &cliBlockContext, const bool &clusterWide)
{

    if (cliBlockContext.getReason () == WAVE_FRAMEWORK_STATE_UNKNOWN_STATE)
    {
        return WAVE_MESSAGE_ERROR;
    }

    LocationId          originator          = FrameworkToolKit::getThisLocationId ();
    CliBlockDetail      detail              (m_serviceId, originator, cliBlockContext);

    // Block CLI on the local node
    if (false == clusterWide)
    {

        ResourceId status = CliBlockManagementToolKit::blockCli (detail);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::blockCli : Error in blockCli. ClusterWide=[false] ServiceName=[" + FrameworkToolKit::getServiceNameById (m_serviceId) + "] Completion Status=[" + FrameworkToolKit::localize (status) + "]");
        }

        return status;
    }

    // Block CLI on the whole cluster
    else
    {

        CliBlockMessage * pCliBlockMessage = new CliBlockMessage (true, detail);

        ResourceId messageStatus = sendSynchronously (pCliBlockMessage);

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::blockCli : Error in blockCli. ClusterWide=[true] ServiceName=[" + FrameworkToolKit::getServiceNameById (m_serviceId) + "] Message Status=[" + FrameworkToolKit::localize (messageStatus) + "]");
            delete pCliBlockMessage;
            return messageStatus;
        }
        else
        {

            ResourceId completionStatus = pCliBlockMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "WaveObjectManager::blockCli : Error in blockCli. ClusterWide=[true] ServiceName=[" + FrameworkToolKit::getServiceNameById (m_serviceId) + "] Completion Status=[" + FrameworkToolKit::localize (completionStatus) + "]");
            }

            delete pCliBlockMessage;
            return completionStatus;

        }
    }
}

ResourceId WaveObjectManager::unblockCli (const CliBlockContext &cliBlockContext, const bool &clusterWide)
{

    if (cliBlockContext.getReason () == WAVE_FRAMEWORK_STATE_UNKNOWN_STATE)
    {
        return WAVE_MESSAGE_ERROR;
    }

    LocationId          originator          = FrameworkToolKit::getThisLocationId ();
    CliBlockDetail      detail              (m_serviceId, originator, cliBlockContext);

    // Unblock CLI on the local node
    if (false == clusterWide)
    {

        ResourceId status = CliBlockManagementToolKit::unblockCli (detail);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::unblockCli : Error in unblockCli. ClusterWide=[false] ServiceName=[" + FrameworkToolKit::getServiceNameById (m_serviceId) + "] Completion Status=[" + FrameworkToolKit::localize (status) + "]");
        }

        return status;
    }

    // Unblock CLI on the whole cluster
    else
    {

        CliBlockMessage * pCliBlockMessage = new CliBlockMessage (false, detail);

        ResourceId messageStatus = sendSynchronously (pCliBlockMessage);

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveObjectManager::unblockCli : Error in unblockCli. ClusterWide=[true] ServiceName=[" + FrameworkToolKit::getServiceNameById (m_serviceId) + "] Message Status=[" + FrameworkToolKit::localize (messageStatus) + "]");
            delete pCliBlockMessage;
            return messageStatus;
        }
        else
        {

            ResourceId completionStatus = pCliBlockMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "WaveObjectManager::unblockCli : Error in unblockCli. ClusterWide=[true] ServiceName=[" + FrameworkToolKit::getServiceNameById (m_serviceId) + "] Completion Status=[" + FrameworkToolKit::localize (completionStatus) + "]");
            }

            delete pCliBlockMessage;
            return completionStatus;

        }
    }
}



void WaveObjectManager::bootStrapSelf ()
{
    bootStrapService (getServiceId ());
}

WaveMessageBrokerStatus WaveObjectManager::connectToMessageBroker (const string &brokerName, const string &brokerIpAddress, const SI32 &brokerPort)
{
    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository = WaveMessagingBrokerRepository::getInstance ();
    bool                           isNewBrokerAdded               = false;
    WaveMessagingBroker           *pWaveMessagingBroker           = NULL;
    WaveMessageBrokerStatus        status                         = WAVE_MESSAGE_BROKER_SUCCESS;

    WaveNs::prismAssert (NULL != pWaveMessagingBrokerRepository, __FILE__, __LINE__);

    isNewBrokerAdded = pWaveMessagingBrokerRepository->addBrokerIfNotAlreadyKnown (brokerName, brokerIpAddress, brokerPort);

    if (true == isNewBrokerAdded)
    {
        pWaveMessagingBroker = pWaveMessagingBrokerRepository->checkoutBroker (brokerName);

        WaveNs::prismAssert (NULL != pWaveMessagingBroker, __FILE__, __LINE__);

        ResourceId status1 = pWaveMessagingBroker->connect(1, 10);

        pWaveMessagingBrokerRepository->checkinBroker (pWaveMessagingBroker);

        if (FRAMEWORK_SUCCESS == status1)
        {
            trace (TRACE_LEVEL_SUCCESS, "WaveObjectManager::connectToMessageBroker : Successfully connected to the Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);

            status = WAVE_MESSAGE_BROKER_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "WaveObjectManager::connectToMessageBroker : Failed to connect to the Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);
            trace (TRACE_LEVEL_WARN, "WaveObjectManager::connectToMessageBroker :     Will continue to try connecting to the Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);

            status = WAVE_MESSAGE_BROKER_ERROR_RETRYING_CONNECTION_BACKGROUND;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Add the new Wave Messaging Broker : " + brokerName + " : " + brokerIpAddress + string (" : ") + brokerPort);

        status = WAVE_MESSAGE_BROKER_ERROR_COULD_NOT_ADD_DUPLICATE_PARAMETERS;
    }

    return (status);
}

WaveMessageBrokerStatus WaveObjectManager::subscribeToMessageBroker (const string &brokerName, const vector<string> &topicNames, const vector<WaveBrokerPublishMessageHandler> publishMessageHandlers, PrismElement *pSubscriber)
{
    UI32 numberOfTopicNames = topicNames.size ();
    UI32 i                  = 0;

    if (NULL == pSubscriber)
    {
        pSubscriber = this;
    }

    for (i = 0; i < numberOfTopicNames; i++)
    {
        string topicName            = topicNames[i];
        bool   isAKnownSubscription = isAKnownWaveBrokerBasedMessageSubscription (brokerName, topicName);

        prismAssert (false == isAKnownSubscription, __FILE__, __LINE__);

        m_waveBrokerBasedMessageSubscriberInformationMap[brokerName][topicName] = new WaveBrokerPublishMessageHandlerContext (publishMessageHandlers[i], pSubscriber);
    }

    return (WaveMessagingBrokerRepository::subscribeToMessageBroker (brokerName, topicNames));
}

WaveMessageBrokerStatus WaveObjectManager::subscribeToMessageBroker (const string &brokerName, const vector<string> &topicNames, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber)
{
    UI32 numberOfTopicNames = topicNames.size ();
    UI32 i                  = 0;

    if (NULL == pSubscriber)
    {
        pSubscriber = this;
    }

    for (i = 0; i < numberOfTopicNames; i++)
    {
        string topicName            = topicNames[i];
        bool   isAKnownSubscription = isAKnownWaveBrokerBasedMessageSubscription (brokerName, topicName);

        prismAssert (false == isAKnownSubscription, __FILE__, __LINE__);

        m_waveBrokerBasedMessageSubscriberInformationMap[brokerName][topicName] = new WaveBrokerPublishMessageHandlerContext (publishMessageHandler, pSubscriber);
    }

    return (WaveMessagingBrokerRepository::subscribeToMessageBroker (brokerName, topicNames));
}

WaveMessageBrokerStatus WaveObjectManager::subscribeToMessageBroker (const string &brokerName, const string &topicName, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber)
{
    if (NULL == pSubscriber)
    {
        pSubscriber = this;
    }

    m_waveBrokerBasedMessageSubscriberInformationMap[brokerName][topicName] = new WaveBrokerPublishMessageHandlerContext (publishMessageHandler, pSubscriber);

    return (WaveMessagingBrokerRepository::subscribeToMessageBroker (brokerName, topicName));
}

WaveMessageBrokerStatus WaveObjectManager::unsubscribeToMessageBroker (const string &brokerName, const vector<string> &topicNames, PrismElement *pSubscriber)
{
    return (WAVE_MESSAGE_BROKER_SUCCESS);
}

WaveMessageBrokerStatus WaveObjectManager::unsubscribeToMessageBroker (const string &brokerName, const string &topicName, PrismElement *pSubscriber)
{
    return (WAVE_MESSAGE_BROKER_SUCCESS);
}

WaveMessageBrokerStatus WaveObjectManager::publishToMessageBroker (const string &brokerName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage, PrismElement *pSubscriber)
{
    return (WaveMessagingBrokerRepository::publishToMessageBroker (brokerName, pWaveBrokerPublishMessage));
}

bool WaveObjectManager::isAKnownWaveBrokerBasedMessageSubscription (const string &brokerName, const string &topicName) const
{
    bool isAKnownSubscription = false;

    map<string, map<string, WaveBrokerPublishMessageHandlerContext *> >::const_iterator element    = m_waveBrokerBasedMessageSubscriberInformationMap.find (brokerName);
    map<string, map<string, WaveBrokerPublishMessageHandlerContext *> >::const_iterator endElement = m_waveBrokerBasedMessageSubscriberInformationMap.end  ();

    if (endElement != element)
    {
        map<string, WaveBrokerPublishMessageHandlerContext *>::const_iterator element1    = element->second.find (topicName);
        map<string, WaveBrokerPublishMessageHandlerContext *>::const_iterator endElement1 = element->second.end  ();

        if (endElement1 != element1)
        {
            isAKnownSubscription = true;
        }
        else
        {
            isAKnownSubscription = false;
        }
    }
    else
    {
        isAKnownSubscription = false;
    }

    return (isAKnownSubscription);
}

void WaveObjectManager::deliverWaveBrokerPublishedEvent (const string &brokerName, const string &topicName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage)
{
    map<string, map<string, WaveBrokerPublishMessageHandlerContext *> >::const_iterator element    = m_waveBrokerBasedMessageSubscriberInformationMap.find (brokerName);
    map<string, map<string, WaveBrokerPublishMessageHandlerContext *> >::const_iterator endElement = m_waveBrokerBasedMessageSubscriberInformationMap.end  ();

    if (endElement != element)
    {
        map<string, WaveBrokerPublishMessageHandlerContext *>::const_iterator element1    = element->second.find (topicName);
        map<string, WaveBrokerPublishMessageHandlerContext *>::const_iterator endElement1 = element->second.end  ();

        if (endElement1 != element1)
        {
            WaveBrokerPublishMessageHandlerContext *pWaveBrokerPublishMessageHandlerContext = element1->second;

            prismAssert (NULL != pWaveBrokerPublishMessageHandlerContext, __FILE__, __LINE__);

            WaveBrokerPublishMessageHandler  waveBrokerPublishMessageHandler = pWaveBrokerPublishMessageHandlerContext->getWaveBrokerPublishMessageHandler ();
            PrismElement                    *pSubscriber                     = pWaveBrokerPublishMessageHandlerContext->getPSubscriber                     ();

            prismAssert (NULL != pSubscriber, __FILE__, __LINE__);

            (pSubscriber->*waveBrokerPublishMessageHandler) (pWaveBrokerPublishMessage);
        }
    }
}

void WaveObjectManager::endOfLifeService (PrismServiceId prismServiceId)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    PrismShutdownObjectManagerMessage *pPrismShutdownObjectManagerMessage = new PrismShutdownObjectManagerMessage (prismServiceId);

    WaveNs::prismAssert (NULL != pPrismShutdownObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismShutdownObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismShutdownObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismShutdownObjectManagerMessage;

    PrismUninstallObjectManagerMessage *pPrismUninstallObjectManagerMessage = new PrismUninstallObjectManagerMessage (prismServiceId);

    WaveNs::prismAssert (NULL != pPrismUninstallObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismUninstallObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismUninstallObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismUninstallObjectManagerMessage;

    PrismDisableObjectManagerMessage *pPrismDisableObjectManagerMessage = new PrismDisableObjectManagerMessage (prismServiceId);

    WaveNs::prismAssert (NULL != pPrismDisableObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismDisableObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismDisableObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismDisableObjectManagerMessage;

    PrismUninitializeObjectManagerMessage *pPrismUninitializeObjectManagerMessage = new PrismUninitializeObjectManagerMessage (prismServiceId);

    WaveNs::prismAssert (NULL != pPrismUninitializeObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismUninitializeObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismUninitializeObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismUninitializeObjectManagerMessage;

    PrismDestructObjectManagerMessage *pPrismDestructObjectManagerMessage = new PrismDestructObjectManagerMessage (prismServiceId);

    WaveNs::prismAssert (NULL != pPrismDestructObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismDestructObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismDestructObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismDestructObjectManagerMessage;
}

void WaveObjectManager::bootStrapService (PrismServiceId prismServiceId)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    PrismInitializeObjectManagerMessage *pPrismInitializeObjectManagerMessage = new PrismInitializeObjectManagerMessage (prismServiceId, WAVE_BOOT_FIRST_TIME_BOOT);

    WaveNs::prismAssert (NULL != pPrismInitializeObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismInitializeObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismInitializeObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismInitializeObjectManagerMessage;

    PrismEnableObjectManagerMessage *pPrismEnableObjectManagerMessage = new PrismEnableObjectManagerMessage (prismServiceId, WAVE_BOOT_FIRST_TIME_BOOT);

    WaveNs::prismAssert (NULL != pPrismEnableObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismEnableObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismEnableObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismEnableObjectManagerMessage;

    PrismListenForEventsObjectManagerMessage *pPrismListenForEventsObjectManagerMessage = new PrismListenForEventsObjectManagerMessage (prismServiceId);

    WaveNs::prismAssert (NULL != pPrismListenForEventsObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismListenForEventsObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismListenForEventsObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismListenForEventsObjectManagerMessage;

    PrismInstallObjectManagerMessage *pPrismInstallObjectManagerMessage = new PrismInstallObjectManagerMessage (prismServiceId, WAVE_BOOT_FIRST_TIME_BOOT);

    WaveNs::prismAssert (NULL != pPrismInstallObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismInstallObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismInstallObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismInstallObjectManagerMessage;

    PrismBootObjectManagerMessage *pPrismBootObjectManagerMessage = new PrismBootObjectManagerMessage (prismServiceId, WAVE_BOOT_FIRST_TIME_BOOT);

    WaveNs::prismAssert (NULL != pPrismBootObjectManagerMessage, __FILE__, __LINE__);

    status = WaveObjectManagerToolKit::sendSynchronously (pPrismBootObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = pPrismBootObjectManagerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete pPrismBootObjectManagerMessage;
}

}
