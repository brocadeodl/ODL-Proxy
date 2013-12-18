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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <time.h>
#include <signal.h>
#include "Framework/OsLayer/PrismOsLayer.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Core/SecondaryNodeClusterContext.h"
#include "Framework/Core/SecondaryNodeClusterPhase2Context.h"
#include "Framework/Core/SecondaryNodeClusterPhase3Context.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "FrameworkObjectManagerMessages.h"
#include "Framework/Trace/TraceMessages.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/LocationManagement/Location.h"
#include "Framework/LocationManagement/SubLocation.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/Test/ClusterTestObjectManager.h"
#include "Framework/Core/CreateClusterWithNodesContext.h"
#include "Framework/Core/AddNodesToClusterContext.h"
#include "Framework/Core/DeleteNodesFromClusterContext.h"
#include "Framework/Core/DeleteNodeOnSecondaryContext.h"
#include "Framework/Messaging/Remote/InterLocationMessageTransportObjectManager.h"
#include "Framework/Core/RejoinNodesToClusterContext.h"
#include "Framework/Core/DestroyClusterContext.h"
#include "Framework/Messaging/Remote/InterLocationMessageReceiverObjectManager.h"
#include "Framework/Core/PrismFrameworkConfigurationWorker.h"
#include "Framework/Shutdown/PrismFinalizeWorker.h"
#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Shell/ShellObjectManager.h"
#include "Regression/RegressionTestObjectManager.h"
#include "App/AppObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Version/PrismVersion.h"
#include "Framework/Core/HeartBeatLostContext.h"
#include "Framework/Core/DestroyClusterAsynchronousContext.h"
#include "Framework/Core/DetachFromClusterAsynchronousContext.h"
#include "Framework/Core/PrismFrameworkConfigurationWorker.h"
#include "Framework/Core/PrismFrameworkFailoverWorker.h"
#include "Framework/Core/PrismFrameworkFailoverWorkerContext.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Core/DetachFromClusterContext.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Database/DatabaseObjectManagerBackupMessage.h"
#include "Framework/Database/DatabaseObjectManagerCleanPreparedTransactionsMessage.h"
#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "Framework/Database/DatabaseObjectManagerRestoreMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Core/UpdateListOfSecondariesContext.h"
#include "Framework/Core/PrismFrameworkObjectManagerServiceControlWorker.h"
#include "Framework/Core/PrismFrameworkObjectManagerPostbootWorker.h"
#include "Framework/Core/PrismFrameworkObjectManagerPostPersistentBootWorker.h"
#include "Framework/Core/PrismFrameworkObjectManagerHaSyncWorker.h"
#include "Cluster/Local/WaveNode.h"
#include "Cluster/PrismCluster.h"
#include <sys/types.h>
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Cluster/Local/ClusterLocalReportPrimaryNodeChangedMessage.h"
#include "Cluster/Local/ClusterLocalReportRemovedNodeFromClusterMessage.h"
#include "Cluster/Local/ClusterLocalReportReplaceMessage.h"

#include "Framework/ExternalStateSynchronization/PrismExternalStateSynchronizationAgent.h"
#include "Framework/SlotFailover/SlotFailoverAgent.h"
#include "Framework/Core/WaveNewNodesAddedEvent.h"
#include "Framework/Core/WaveNodesAdditionToClusterCompletedEvent.h"
#include "Framework/Core/WaveNodeDeleteClusterEvent.h"
#include "Framework/Core/WaveNodeLocalNodeDeletedEvent.h"
#include "Framework/Core/ClusterPhase3StartEvent.h"
#include "Framework/Core/ClusterPhase3CompleteEvent.h"
#include "Framework/Core/PrimaryChangedEvent.h"
#include "Framework/Postboot/PrismPostbootMessages.h"
 #include "Framework/Postboot/PrismPostPersistentBootWorkerClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Persistence/PersistenceObjectManagerExecuteTransactionMessage.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include <set>
#include "Framework/Zeroize/ZeroizeForFIPSLinearSequencerContext.h"
#include "Framework/Zeroize/ZeroizeForFIPSMessage.h"
#include "Framework/Zeroize/PrismZeroizeAgent.h"
#include "Framework/Core/Wave.h"
#include "Framework/Core/VcsClusterConfiguration.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "Framework/ConfigReplayEnd/FileReplayEndAgent.h"
#include "Framework/Core/PrismFrameworkServiceIndependentMessages.h"
#include "Framework/Core/WaveConfigurationIntentRepository.h"
#include "Framework/Persistence/PersistenceToolKit.h"
#include "Framework/Core/PrismFrameworkReplayConfigMessage.h"
#include "Framework/CliBlockManagement/CliBlockManagementToolKit.h"

namespace WaveNs
{

static bool                                 s_isFrameworkReadyToBoot                                         = false;
static PrismMutex            *              s_pFrameworkReadinessMutex                                       = NULL;
static PrismMutex                           s_disconnectMutex;
static PrismMutex                           s_instantiationMutex;
static PrismMutex                           s_startupValidMutex;
static PrismMutex                           s_startupFileMutex;
static PrismMutex                           s_startupFileTypeMutex;
static string                               s_startupFileName                                                = "startupFile";
static bool                                 s_isFrameworkInstantiated                                        = false;

static FrameworkSequenceType                s_frameworkSequenceType                                          = FRAMEWORK_SEQUENCE_TYPE0;

static string                               s_frameworkConfigurationFile                                     = "";
static string                               s_frameworkConfigurationBackupFile                               = "";

static string                               s_frameworkGlobalConfigurationFile                               = "";
static string                               s_frameworkGlobalConfigurationTempFile                           = "";

static string                               s_lockFileForConfigurationFile                                   = "";     

static UI32                                 s_offSetForValidationResults                                     = 0x0000FFFF;

static LocationRole                         s_locationRole                                                   = LOCATION_STAND_ALONE;
static LocationId                           s_locationParentLocationId                                       = 0;
static string                               s_locationParentIpAddress                                        = "";
static SI32                                 s_locationParentPort                                             = 3016;
static LocationId                           s_locationLocationId                                             = 0;
static string                               s_locationIpAddress                                              = "";
static SI32                                 s_locationPort                                                   = 3016;

static PrismMutex                           s_bootSynchronizationMutex;
static PrismMutex                           s_dbRestoreMutex;
static PrismMutex                           s_dbConversionStatusMutex;

static map<PrismThreadId, PrismThreadId>    s_prismThreadIdRepository;
static PrismMutex                           s_prismThredIdRepositoryMutex;

static const UI32                           s_clusterPhase0TimeoutInMilliseconds                             = 900000; // 15 * 60 * 1000 
static const UI32                           s_clusterPhase1TimeoutInMilliseconds                             = 900000; // 15 * 60 * 1000 
static const UI32                           s_clusterPhase2TimeoutInMilliseconds                             = 900000; // 15 * 60 * 1000
static const UI32                           s_clusterPhase3TimeoutInMilliseconds                             = 900000; // 15 * 60 * 1000

PrismMutex                                  PrismFrameworkObjectManager::m_externalStateSynchronizationRequiredListLock;
vector<PrismServiceId>                      PrismFrameworkObjectManager::m_externalStateSynchronizationRequiredList;

string                                      PrismFrameworkObjectManager::m_ipAddressForThisLocation;
PrismMutex                                  PrismFrameworkObjectManager::m_ipAddressForThisLocationMutex;
string                                      PrismFrameworkObjectManager::m_ethernetInterfaceForThisLocation;

GetInstancesFunction                        PrismFrameworkObjectManager::m_getInstancesFunction                     = NULL;
GetInstancesAndSubInstancesFunction         PrismFrameworkObjectManager::m_getInstancesAndSubInstancesFunction      = NULL;
GetThisSlotInstanceFunction                 PrismFrameworkObjectManager::m_getThisSlotInstanceFunction              = NULL;
CreateDummySlotFunction                     PrismFrameworkObjectManager::m_createDummySlotFunction                  = NULL;
RaslogAssertFunction                        PrismFrameworkObjectManager::m_raslogAssertFunction                     = NULL;

GetFirmwareVersionFunction                  PrismFrameworkObjectManager::m_getFirmwareVersionFunction          = NULL;
HandleDcmFssConfigUpdateFunction            PrismFrameworkObjectManager::m_handleDcmFssConfigUpdateFunction = NULL;
SetVcsClusterConfigHandlerFunction          PrismFrameworkObjectManager::m_setVcsClusterConfigHandlerFunction          = NULL;
GetVcsClusterConfigHandlerFunction          PrismFrameworkObjectManager::m_getVcsClusterConfigHandlerFunction          = NULL;
NotifySyncUpdateFailureFunction             PrismFrameworkObjectManager::m_notifySyncUpdateFailureFunction  = NULL;
NotifyClusterReadyStateFunction             PrismFrameworkObjectManager::m_notifyClusterReadyStateFunction  = NULL;

void PrismFrameworkObjectManager::setIsInstantiated (const bool &isInstantiated)
{
    s_instantiationMutex.lock ();
    s_isFrameworkInstantiated = isInstantiated;
    s_instantiationMutex.unlock ();
}

bool PrismFrameworkObjectManager::getIsInstantiated ()
{
    bool isInstantiated;

    s_instantiationMutex.lock ();
    isInstantiated = s_isFrameworkInstantiated;
    s_instantiationMutex.unlock ();

    return (isInstantiated);
}

void PrismFrameworkObjectManager::setConfigurationFile (const string &configurationFile)
{
    s_frameworkConfigurationFile        = configurationFile;
    s_frameworkConfigurationBackupFile  = s_frameworkConfigurationFile + ".bk";
}

void PrismFrameworkObjectManager::setGlobalConfigurationFile (const string &globalConfigurationFile)
{
    s_frameworkGlobalConfigurationFile        = globalConfigurationFile;
    s_frameworkGlobalConfigurationTempFile    = s_frameworkGlobalConfigurationFile + ".tmp";
}

void PrismFrameworkObjectManager::setLockFileForConfigurationFile (const string &lockFile)
{
    s_lockFileForConfigurationFile = lockFile;
}

LocationId PrismFrameworkObjectManager::getLastUsedLocationIdInPrismFrameworkObjectManager () const
{
    return (m_lastUsedLocationId);
}

void PrismFrameworkObjectManager::setLastUsedLocationIdInPrismFrameworkObjectManager (const LocationId &locationId)
{
    m_lastUsedLocationId = locationId;
}

bool PrismFrameworkObjectManager::getPrismConfigurationValidity ()
{
    return (m_pConfigurationWorker)->getPrismConfigurationValidity ();
}

PrismFrameworkObjectManager::PrismFrameworkObjectManager ()
    : WaveLocalObjectManager (getServiceName ()),
      m_pThisLocation (NULL),
      m_isStartupValid (false),
      m_secondaryNodeClusterCreationFlag (false),
      m_primaryNodeClusterOperationFlag (false),
      m_activationCompleted (false), 
      m_postBootStarted (false),
      m_postBootCompleted (false),
      m_configReplayStarted (false),      
      m_configReplayCompleted (false),
      m_nodeReadyForAllCommands (false),
      m_clusterPhaseTimerHandler (0),
      m_isPostBootNeededDuringRejoin (false),
      m_needNotifyClusterReadyState (false),
      m_nodeZeroized(false),
      m_isDBRestoreIncomplete (false),
      m_dbConversionStatus (DATABASE_SCHEMA_CONVERSION_NOT_ATTEMPTED),
      m_tableIdsChangedForSchemaCompatibility (false),
      m_isWarmHaRecoveryPreparationInProgress (false)
{

    acquireBootSynchronizationMutex ();
    m_externalStateSynchronizationRequiredList.clear ();
    // First configure this location

    if (LOCATION_STAND_ALONE == s_locationRole)
    {
        configureThisLocationAsLocation (s_locationPort);
    }
    else if (LOCATION_SUB_LOCATION == s_locationRole)
    {
        configureThisLocationAsSubLocation (s_locationParentLocationId, s_locationParentIpAddress, s_locationParentPort, s_locationLocationId, s_locationIpAddress, s_locationPort);
    }
    else
    {
        // Cannot use tracing at this point in time.
        cerr << "PrismFrameworkObjectManager::PrismFrameworkObjectManager : Invalid type of Location Configuration.  Cannot Continue" << endl;
        exit (-1);
    }

    prismAssert (NULL != s_pFrameworkReadinessMutex, __FILE__, __LINE__);

    s_isFrameworkReadyToBoot = false;

    m_pInitializeWorker = new PrismFrameworkObjectManagerInitializeWorker (this);

    prismAssert (NULL != m_pInitializeWorker, __FILE__, __LINE__);

    m_pPrismFinalizeWorker = new PrismFinalizeWorker (this);

    prismAssert (NULL != m_pPrismFinalizeWorker, __FILE__, __LINE__);

    m_pConfigurationWorker = new PrismFrameworkConfigurationWorker (this);

    prismAssert (NULL != m_pConfigurationWorker, __FILE__, __LINE__);

    m_pPrismFrameworkFailoverWorker = new PrismFrameworkFailoverWorker (this);

    prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

    m_pPrismFrameworkObjectManagerServiceControlWorker = new PrismFrameworkObjectManagerServiceControlWorker (this);

    prismAssert (NULL != m_pPrismFrameworkObjectManagerServiceControlWorker, __FILE__, __LINE__);

    m_pPrismFrameworkObjectManagerPostbootWorker = new PrismFrameworkObjectManagerPostbootWorker (this);

    prismAssert (NULL != m_pPrismFrameworkObjectManagerPostbootWorker, __FILE__, __LINE__);

    m_pPrismFrameworkObjectManagerPostPersistentBootWorker = new PrismFrameworkObjectManagerPostPersistentBootWorker (this);

    prismAssert (NULL != m_pPrismFrameworkObjectManagerPostPersistentBootWorker, __FILE__, __LINE__);

    m_pPrismFrameworkObjectManagerHaSyncWorker = new PrismFrameworkObjectManagerHaSyncWorker (this);

    prismAssert (NULL != m_pPrismFrameworkObjectManagerHaSyncWorker, __FILE__, __LINE__);

    //prismAssert (NULL != m_dbStandaloneTransaction, __FILE__, __LINE__);*/
//Temporarily disabled all xerces related code.
#if 0
    try
    {
        XMLPlatformUtils::Initialize ();
    }
    catch (const XMLException &exception)
    {
        cout << " E R R O R in Initializing the XMLPlatformUtils." << endl;
        prismAssert (0, __FILE__, __LINE__);
        exit (-1);
    }
#endif
    // Indicate that the Instatiation of WaveObjectManager is complete.

    setIsInstantiated (true);
    setIsEnabled (true);

    // Now send a oneway initialize message to self.

    // We know the service id for the Prism framework object manager must be 1.  And it must not be anything else.  So we use the value 1 in the next statement.

    PrismInitializeObjectManagerMessage *pPrismInitializeObjectManagerMessage = new PrismInitializeObjectManagerMessage (1, WAVE_BOOT_FIRST_TIME_BOOT); // We cannot use PrismFrameworkObjectManager::getPrismServiceId () - it iwill invariably fail since we are inside the getInstance method
                                                                                                                                                        // We always say it is the first time boot, but, this reason is not to be looked into in the initialize for this OM.

    WaveMessageStatus status = sendOneWay (pPrismInitializeObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        cerr << "Cannot Initialize the Framework.  Cannot continue.  Exiting ..." << endl;
        exit (-2);
    }

    PrismEnableObjectManagerMessage *pPrismEnableObjectManagerMessage = new PrismEnableObjectManagerMessage (1, WAVE_BOOT_FIRST_TIME_BOOT); // We cannot use PrismFrameworkObjectManager::getPrismServiceId () - it iwill invariably fail since we are inside the getInstance method
                                                                                                                                            // We always say it is the first time boot, but, this reason is not to be looked into in the enable for this OM.

    status = sendOneWay (pPrismEnableObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        cerr << "Cannot Enable the Framework.  Cannot continue.  Exiting ..." << endl;
        exit (-3);
    }

    PrismInstallObjectManagerMessage *pPrismInstallObjectManagerMessage = new PrismInstallObjectManagerMessage (1, WAVE_BOOT_FIRST_TIME_BOOT); // We cannot use PrismFrameworkObjectManager::getPrismServiceId () - it iwill invariably fail since we are inside the getInstance method
                                                                                                                                               // We always say it is the first time boot, but, this reason is not to be looked into in the install for this OM.

    status = sendOneWay (pPrismInstallObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        cerr << "Cannot Install the Framework.  Cannot continue.  Exiting ..." << endl;
        exit (-4);
    }

    PrismBootObjectManagerMessage *pPrismBootObjectManagerMessage = new PrismBootObjectManagerMessage (1, WAVE_BOOT_FIRST_TIME_BOOT); // We cannot use PrismFrameworkObjectManager::getPrismServiceId () - it iwill invariably fail since we are inside the getInstance method
                                                                                                                                      // We always say it is the first time boot, but, this reason is not to be looked into in the boot for this OM.

    status = sendOneWay (pPrismBootObjectManagerMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        cerr << "Cannot Boot the Framework.  Cannot continue.  Exiting ..." << endl;
        exit (-5);
    }

    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CREATE_CLUSTER_WITH_NODES,           reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::createClusterWithNodesMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY,         reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::configureSecondaryNodeMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_1, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::configureSecondaryNodePhase1MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_2, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::configureSecondaryNodePhase2MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_3, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_UPDATE_LIST_OF_CLUSTER_SECONDARIES,  reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::updateListOfSecondariesMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_ADD_NODES_TO_CLUSTER,                reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::addNodesToClusterMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_DELETE_NODES_FROM_CLUSTER,           reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::deleteNodesFromClusterMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_UNCONFIGURE_CLUSTER_SECONDARY,       reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_DESTORY_CLUSTER,                     reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::destroyClusterMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_NODES_TO_CLUSTER,             reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinNodesToClusterMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY,            reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_1,    reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryPhase1MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_2,    reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryPhase2MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_3,    reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryPhase3MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_DETACH_FROM_CLUSTER,                 reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::detachFromClusterMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_LOST_HEART_BEAT_MESSAGE,             reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::lostHeartBeatMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_MESSAGE,             reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::primaryChangedMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_1_MESSAGE,     reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::primaryChangedPhase1MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_2_MESSAGE,     reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::primaryChangedPhase2MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_3_MESSAGE,     reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::primaryChangedPhase3MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_START_EXTERNAL_STATE_SYNCHRONIZATION,reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::startExternalStateSynchronizationHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_START_SLOT_FAILOVER,                 reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::startSlotFailoverHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_NEW_PRINCIPAL_ESTABLISHED_AFTER_FAILOVER, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::newPrincipalSelectedAfterFaioverMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_SECONDARY_NODE_FAILURE_NOTIFICATION, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::secondaryNodeFailureNotificationMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_DISCONNECT_FROM_ALL_NODES,           reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::disconnectFromAllNodesHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_ESTABLISH_PRINCIPAL_AFTER_CLUSTER_REBOOT, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::establishPrincipalAfterClusterRebootHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_RESET_NODE_FOR_CLUSTER_MERGE,reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::resetNodeForClusterMergeHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REMOVE_KNOWN_LOCATIONS_MESSAGE,reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::removeFailedLocationsFromKnownLocationsHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_FIPS_ZEROIZE,                      reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::zeroizeForFIPSMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_DISCONNECT_FROM_ALL_INSTANCE_CLIENTS, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::disconnectFromAllInstanceClientsHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PREPARE_FOR_WARM_RECOV_WITH_DEFAULT_CONFIG_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::prepareNodeForWarmRecoveryWithDefaultConfiguration));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_STORE_CONFIGURATION_INTENT, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::storeConfigurationIntentMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REMOVE_CONFIGURATION_INTENT, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::removeConfigurationIntentMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PREPARE_FOR_HA_RECOV_WITH_PERSISTENT_CONFIG_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::prepareFrameworkForHaRecoveryMessageHandler));

    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER,         reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::configureSecondaryNodeHaPeerMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER_PHASE_1, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::configureSecondaryNodeHaPeerPhase1MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER_PHASE_3, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::configureSecondaryNodeHaPeerPhase3MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER,            reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER_PHASE_1,    reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerPhase1MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER_PHASE_3,    reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerPhase3MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_MESSAGE_HA_PEER,             reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::primaryChangedHaPeerMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_1_HA_PEER_MESSAGE,     reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::primaryChangedHaPeerPhase1MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_3_HA_PEER_MESSAGE,     reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::primaryChangedHaPeerPhase3MessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_ROLLBACK_STANDBY_ON_ACTIVE_ROLLBACK,         reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_REPLAY_CONFIG_FILE,                          reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManager::replayConfigFileMessageHandler));

    addServiceIndependentOperationMap (FRAMEWORK_OBJECT_MANAGER_STORE_CONFIGURATION_INTENT, reinterpret_cast<WaveServiceIndependentMessageHandler> (&PrismFrameworkObjectManager::storeConfigurationIntentStaticMessageHandler));
    addServiceIndependentOperationMap (FRAMEWORK_OBJECT_MANAGER_REMOVE_CONFIGURATION_INTENT, reinterpret_cast<WaveServiceIndependentMessageHandler> (&PrismFrameworkObjectManager::removeConfigurationIntentStaticMessageHandler));
}

PrismFrameworkObjectManager *PrismFrameworkObjectManager::getInstance ()
{
    // First create the framework readiness mutex.  By the time we enter the constructor for PrismFrameworkObjectManager
    // it might be too late to create the mutex.  Depending on the pthread implementation, a thread might have already started
    // which needs to access the mutex.

    if (NULL == s_pFrameworkReadinessMutex)
    {
        s_pFrameworkReadinessMutex = new PrismMutex ();
        WaveNs::prismAssert (NULL != s_pFrameworkReadinessMutex, __FILE__, __LINE__);
    }

    static PrismFrameworkObjectManager *pPrismFrameworkObjectManager = NULL;

    if (NULL == pPrismFrameworkObjectManager)
    {
        pPrismFrameworkObjectManager = new PrismFrameworkObjectManager ();
        WaveNs::prismAssert (NULL != pPrismFrameworkObjectManager, __FILE__, __LINE__);
    }

    return (pPrismFrameworkObjectManager);
}

string PrismFrameworkObjectManager::getServiceName ()
{
    return ("Prism Framework");
}

PrismFrameworkObjectManager::~PrismFrameworkObjectManager ()
{
    if (NULL != m_pInitializeWorker)
    {
        delete m_pInitializeWorker;
    }

    if (NULL != m_pPrismFinalizeWorker)
    {
        delete m_pPrismFinalizeWorker;
    }

    if (NULL != m_pConfigurationWorker)
    {
        delete m_pConfigurationWorker;
    }

    if (NULL != m_pPrismFrameworkFailoverWorker)
    {
        delete m_pPrismFrameworkFailoverWorker;
    }

    if (NULL != m_pThisLocation)
    {
        delete m_pThisLocation;
    }

    if (NULL != m_pPrismFrameworkObjectManagerServiceControlWorker)
    {
        delete m_pPrismFrameworkObjectManagerServiceControlWorker;
    }

    if (NULL != m_pPrismFrameworkObjectManagerPostbootWorker)
    {
        delete m_pPrismFrameworkObjectManagerPostbootWorker;
    }

    if (NULL != m_pPrismFrameworkObjectManagerPostPersistentBootWorker)
    {
        delete m_pPrismFrameworkObjectManagerPostPersistentBootWorker;
    }

    if (NULL != m_pPrismFrameworkObjectManagerHaSyncWorker)
    {
        delete m_pPrismFrameworkObjectManagerHaSyncWorker;
    }

//Temporarily disabled all xerces related code.
#if 0
    // Terminating the XMLPlatformUtils must be the last one.

    XMLPlatformUtils::Terminate ();
#endif
}

PrismMessage *PrismFrameworkObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case FRAMEWORK_OBJECT_MANAGER_UPDATE_LIST_OF_CLUSTER_SECONDARIES :
            pPrismMessage = new FrameworkObjectManagerUpdateListOfSecondariesMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY :
            pPrismMessage = new PrismConfigureClusterSecondaryMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_1 :
            pPrismMessage = new PrismConfigureClusterSecondaryPhase1Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_2 :
            pPrismMessage = new PrismConfigureClusterSecondaryPhase2Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_3 :
            pPrismMessage = new PrismConfigureClusterSecondaryPhase3Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_UNCONFIGURE_CLUSTER_SECONDARY :
            pPrismMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY :
            pPrismMessage = new FrameworkObjectManagerRejoinClusterSecondaryMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_1 :
            pPrismMessage = new PrismRejoinClusterSecondaryPhase1Message();
            break;

        case FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_2 :
            pPrismMessage = new PrismRejoinClusterSecondaryPhase2Message();
            break;

        case FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_3 :
            pPrismMessage = new PrismRejoinClusterSecondaryPhase3Message();
            break;

        case FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_MESSAGE :
            pPrismMessage = new FrameworkObjectManagerPrimaryChangedMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_1_MESSAGE:
            pPrismMessage = new FrameworkObjectManagerPrimaryChangedPhase1Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_2_MESSAGE:
            pPrismMessage = new FrameworkObjectManagerPrimaryChangedPhase2Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_3_MESSAGE:
            pPrismMessage = new FrameworkObjectManagerPrimaryChangedPhase3Message;
            break;
        case FRAMEWORK_OBJECT_MANAGER_FIPS_ZEROIZE:
            pPrismMessage = new ZeroizeForFIPSMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REMOVE_KNOWN_LOCATIONS_MESSAGE:
            pPrismMessage = new FrameworkObjectManagerRemoveKnownLocationsMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_DISCONNECT_FROM_ALL_INSTANCE_CLIENTS :
            pPrismMessage = new FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_STORE_CONFIGURATION_INTENT :
            pPrismMessage = new FrameworkObjectManagerStoreConfigurationIntentMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REMOVE_CONFIGURATION_INTENT :
            pPrismMessage = new FrameworkObjectManagerRemoveConfigurationIntentMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER:
            pPrismMessage = new PrismConfigureClusterSecondaryHaPeerMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER_PHASE_1:
            pPrismMessage = new PrismConfigureClusterSecondaryHaPeerPhase1Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER_PHASE_3:
            pPrismMessage = new PrismConfigureClusterSecondaryHaPeerPhase3Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER:
            pPrismMessage = new FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER_PHASE_1:
            pPrismMessage = new PrismRejoinClusterSecondaryHaPeerPhase1Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER_PHASE_3:
            pPrismMessage = new PrismRejoinClusterSecondaryHaPeerPhase3Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_MESSAGE_HA_PEER:
            pPrismMessage = new FrameworkObjectManagerPrimaryChangedHaPeerMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_1_HA_PEER_MESSAGE:
            pPrismMessage = new FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_3_HA_PEER_MESSAGE:
            pPrismMessage = new FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message;
            break;

        case FRAMEWORK_OBJECT_MANAGER_ROLLBACK_STANDBY_ON_ACTIVE_ROLLBACK:
            pPrismMessage = new FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_REPLAY_CONFIG_FILE:
            pPrismMessage = new PrismFrameworkReplayConfigMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

PrismServiceId PrismFrameworkObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void PrismFrameworkObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    // First reply to it.  Initializemessage is sent to us by ourselves.
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();

    // Initialize the last used location Id.

    initializeLastUsedLocationId ();

    OrmRepository* pOrmRepository = OrmRepository::getInstance();

    pOrmRepository->computeTableSpace();

    /* zeroize if previous zeroize was incomplete */
    validateAndZeroizeAtBoot();

    // Start the Initialization sequnce.

    ResourceId status = m_pInitializeWorker->startPrismServices ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::boot Initial Boot of Services should Never Fail");
        prismAssert (false, __FILE__, __LINE__);
    }

}

void  PrismFrameworkObjectManager::validateAndZeroizeAtBoot()
{
    ifstream zeroizeStream(getFIPSZeroizeFile().c_str());
    bool noShutdownAtZeroize = true;
    if(zeroizeStream.is_open())
    {
        /*File exists, zeroize was not complete. So complete now.
         */
        zeroizeStream.close();
        trace(TRACE_LEVEL_INFO,"PrismFrameworkObjectManager::validateAndZeroizeAtBoot: Previous zeroize was incomplete. Clearing now..\n");
        Wave::logOperationStatus (FIPS_OPERATION_TRIGGER_CLEANUP_FOR_ZEROIZE);
    
        FrameworkSequenceGenerator &frameworkSequenceGenerator = getCurrentFrameworkSequenceGenerator ();
        PrismZeroizeAgent *pZAgent = new PrismZeroizeAgent(this, frameworkSequenceGenerator,noShutdownAtZeroize);

        prismAssert(NULL != pZAgent, __FILE__,__LINE__);
    
        ResourceId status = pZAgent->execute();
        if(status != WAVE_MESSAGE_SUCCESS)
        {
            trace(TRACE_LEVEL_FATAL,"PrismFrameworkObjectManager::validateAndZeroizeAtBoot: one or more services failed to zeroize\n");
            prismAssert(false,__FILE__,__LINE__);
        }
    }
    else
    {
        trace(TRACE_LEVEL_INFO,"PrismFrameworkObjectManager::validateAndZeroizeAtBoot: state is clean.\n");
    }
    

}

void PrismFrameworkObjectManager::bootPrism ()
{
    setIsFrameworkReadyToBoot (true);
}

void PrismFrameworkObjectManager::waitForPrismServicesToFinish ()
{
    PrismMutex mutexForAllPrismServices;

	// double lock to ensure this function doesn't complete
    mutexForAllPrismServices.lock ();
    mutexForAllPrismServices.lock ();
}

void PrismFrameworkObjectManager::configureThisLocationAsLocation (SI32 port)
{
    if (NULL != m_pThisLocation)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureThisLocationAsLocation : This location is already configured");
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    string ipAddress = getIpAddressForThisLocation ();

    if ("" == ipAddress)
    {
        ipAddress = FrameworkToolKit::getLoopBackIpAddressForThisMachine ();
    }

    LocationId locationId = (UI32) (1 << 8);

    m_pThisLocation = new Location (locationId, ipAddress, port);

    if (NULL == m_pThisLocation)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureThisLocationAsLocation : Could not allocate a Location.");
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_pThisLocation->setClusterPrimaryLocationId (locationId);
    m_pThisLocation->setClusterPrimaryPort       (port);
}

void PrismFrameworkObjectManager::configureThisLocationAsSubLocation (LocationId parentLocationId, string &parentIpAddress, SI32 parentPort, LocationId locationId, string &ipAddress, SI32 port)
{
    if (NULL != m_pThisLocation)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureThisLocationAsSubLocation : This location is already configured");
        prismAssert (false, __FILE__, __LINE__);
    }

    SubLocation *pSubLocation = new SubLocation (parentLocationId, parentIpAddress, parentPort, locationId, ipAddress, port);

    if (NULL == pSubLocation)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureThisLocationAsSubLocation : Could not allocate a Sub Location.");
        prismAssert (false, __FILE__, __LINE__);
    }

    Location *pLocation = new Location (parentLocationId, parentIpAddress, parentPort);

    if (NULL == pLocation)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureThisLocationAsSubLocation : Could not allocate a Location.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pSubLocation->setPParentLocation (pLocation);
    pSubLocation->setClusterPrimaryLocationId (parentLocationId);
    pSubLocation->setClusterPrimaryPort       (parentPort);

    m_pThisLocation = pSubLocation;
}

bool PrismFrameworkObjectManager::addSubLocation (LocationId locationId, string &ipAddress, SI32 port)
{
    if (NULL == m_pThisLocation)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::addSubLocation : This location is not configured yet.");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (true == (m_pThisLocation->supportsSubLocations ()))
    {
        m_pThisLocation->addSubLocation (locationId, ipAddress, port);
        return (true);
    }
    else
    {
        return (false);
    }
}

void PrismFrameworkObjectManager::setFrameworkSequenceType (FrameworkSequenceType frameworkSequenceType)
{
    s_frameworkSequenceType = frameworkSequenceType;
}

void PrismFrameworkObjectManager::initializeServerCommunications ()
{
    static const UI32 retryCount = 120;
                 UI32 i          = 0;

    if (NULL != m_pThisLocation)
    {
        ServerStreamingSocketStatus status = SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_BIND;

        while (SERVER_STREAMING_SOCKET_SUCCESS != status)
        {
            status = m_pThisLocation->initializeServerCommunications ();

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
                        trace (TRACE_LEVEL_ERROR, "Could not listen on a Local TCP/IP Socket.  Please make sure that there is no other instance of this program is currently active.");
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
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::initializeServerCommunications : This Locations is not configured yet.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void PrismFrameworkObjectManager::initializeClientCommunications ()
{
    if (NULL != m_pThisLocation)
    {
        m_pThisLocation->initializeClientCommunications ();
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::initializeClientCommunications : This Locations is not configured yet.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool PrismFrameworkObjectManager::acceptNewConnection (ServerStreamingSocket &newSocket)
{
    bool successfullyAcceptedNewConnection = false;

    if (NULL != m_pThisLocation)
    {
        successfullyAcceptedNewConnection = m_pThisLocation->acceptNewConnection (newSocket);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::acceptNewConnection : This Locations is not configured yet.");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (successfullyAcceptedNewConnection);
}

void PrismFrameworkObjectManager::disconnectFromLocation (LocationId locationId, const bool &closePeerServerSocket)
{
    s_disconnectMutex.lock ();

    if (0 != locationId)
    {
        string ipAddress = FrameworkToolKit::getIpAddressForLocationId (locationId);
        SI32   port      = FrameworkToolKit::getPortForLocationId      (locationId);

        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::disconnectFromLocation : Disconnecting from Location : " + ipAddress + string (", Port : ") + port);

        if (true == closePeerServerSocket)
        {
            InterLocationMessageReceiverObjectManager::closePeerServerStreamingSocketAndRemoveInterLocationMessageReceiverThreadFromCache (ipAddress, port);
        }

        m_pThisLocation->invalidateClientStreamingSocketForRemoteLocation (locationId);
        (InterLocationMessageTransportObjectManager::getInstance ())->replyToRemoteMessagesPendingOnLocation (locationId, WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
    }

    s_disconnectMutex.unlock ();
}

void PrismFrameworkObjectManager::disconnectFromLocation (const string &ipAddress, const SI32 &port, const bool &closePeerServerSocket)
{
    if (0 == (FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress, port)))
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::disconnectFromLocation : We have already disconnected from Location : " + ipAddress);
    }
    else
    {
        disconnectFromLocation (m_pThisLocation->getLocationIdForIpAddressAndPort (ipAddress, port), closePeerServerSocket);
    }
}

void PrismFrameworkObjectManager::removeKnownLocation (const LocationId &locationId)
{
    s_disconnectMutex.lock ();

    if (0 != locationId)
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::removeKnownLocation : Disconnecting from Location : " + FrameworkToolKit::getIpAddressForLocationId (locationId));
        m_pThisLocation->invalidateClientStreamingSocketForRemoteLocation (locationId);
        (InterLocationMessageTransportObjectManager::getInstance ())->replyToRemoteMessagesPendingOnLocation (locationId, WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
        m_pThisLocation->removeKnownLocation (locationId);
    }

    s_disconnectMutex.unlock ();
}

ResourceId PrismFrameworkObjectManager::connectToLocation (const string &ipAddress, const SI32 &port)
{
    static const UI32 maximumNumberOfRetriesToConnectToARemoteLocation  = 10;
    static const UI32 maximumNumberOfSecondsToWaitToConnect             = 10;

    return (m_pThisLocation->connectToRemoteLocation (m_pThisLocation->getLocationIdForIpAddressAndPort (ipAddress, port), maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect));
}

LocationBase *PrismFrameworkObjectManager::getThisLocation () const
{
    return (m_pThisLocation);
}

const UI32 PrismFrameworkObjectManager::getThisLocationId () const
{
    return ((getThisLocation ())->getLocationId ());
}

void PrismFrameworkObjectManager::setIsFrameworkReadyToBoot (bool isFrameworkReadyToBoot)
{
    s_pFrameworkReadinessMutex->lock ();
    s_isFrameworkReadyToBoot = isFrameworkReadyToBoot;
    s_pFrameworkReadinessMutex->unlock ();
}

bool PrismFrameworkObjectManager::getIsFrameworkReadyToBoot ()
{
    bool isFrameworkReadyToBoot = false;

    s_pFrameworkReadinessMutex->lock ();
    isFrameworkReadyToBoot = s_isFrameworkReadyToBoot;
    s_pFrameworkReadinessMutex->unlock ();

    return (isFrameworkReadyToBoot);
}

void PrismFrameworkObjectManager::addPrismThreadId (const PrismThreadId &prismThreadId)
{
    s_prismThredIdRepositoryMutex.lock ();

    s_prismThreadIdRepository [prismThreadId] = prismThreadId;

    s_prismThredIdRepositoryMutex.unlock ();
}

void PrismFrameworkObjectManager::removePrismThreadId (const PrismThreadId &prismThreadId)
{
    s_prismThredIdRepositoryMutex.lock ();

    map<PrismThreadId, PrismThreadId>::iterator  element = s_prismThreadIdRepository.find (prismThreadId);
    map<PrismThreadId, PrismThreadId>::iterator  end     = s_prismThreadIdRepository.end (); 

    if (end != element)
    {   
        s_prismThreadIdRepository.erase (element);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_WARN, string ("PrismFrameworkObjectManager::removePrismThreadId This thread Id does not exist"));
    } 

    s_prismThredIdRepositoryMutex.unlock ();    
}

void PrismFrameworkObjectManager::getAllPrismThreads (map<PrismThreadId, PrismThreadId> &prismThreadsMap)
{
    s_prismThredIdRepositoryMutex.lock ();

    prismThreadsMap = s_prismThreadIdRepository;

    s_prismThredIdRepositoryMutex.unlock ();
}
        
void PrismFrameworkObjectManager::deleteAllPrismThreads ()
{
    map<PrismThreadId, PrismThreadId>::iterator  threadIterator;
    UI32 retVal = 0;
    s_prismThredIdRepositoryMutex.lock ();

    // This code has been added so that we test the existence of the thread id that is pthread_cancel'ed before proceeding to kill other pthreads
    // Also, at the end of the function we do not unlock the mutex so that no other deleteAllPrismThreads call is made to kill the same set of threads again

    for (threadIterator = s_prismThreadIdRepository.begin (); threadIterator != s_prismThreadIdRepository.end (); threadIterator++)
    {   
        retVal = pthread_cancel (threadIterator->first);
        
        while (1)
        {
            retVal = pthread_kill (threadIterator->first, 0);
            if (!retVal)
            {
                WaveNs::tracePrintf (TRACE_LEVEL_DEBUG, false, false, "PrismFrameworkObjectManager::deleteAllPrismThreads : Thread successfully canceled \n");
                break;
            }
            else
            {
                prismUSleep (100000);
            }
        }
    }   

    //s_prismThredIdRepositoryMutex.unlock ();
}

ResourceId PrismFrameworkObjectManager::createBufferForFileToSync ( const string &filenameToSync, char* &pfileBuffer, UI32 &sizeOfFileBuffer )
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string("PrismFrameworkObjectManager::createBufferForFileToSync : filename - ") + filenameToSync );
    ifstream file;
    file.open ( (filenameToSync).c_str() );

    if ( true == file.is_open() )
    {
        file.seekg (0, ios::end);
        UI32 fileSize = file.tellg ();
        char * pBuffer = NULL;
        if ( 0 < fileSize )
        {
            pBuffer = new char [fileSize];
            prismAssert ( NULL != pBuffer, __FILE__, __LINE__ );         
            file.seekg (0, ios::beg);
            file.read ( pBuffer, fileSize );
            pfileBuffer = pBuffer;
            sizeOfFileBuffer= fileSize;   
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::createBufferForFileToSync : Empty file - \"") + filenameToSync + ("\"") );
        }
    } 
    else
    {
        status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::createBufferForFileToSync : Unable to open file - \"") + filenameToSync + ("\"") );    
    }
    file.close();

    return (status);
} 

ResourceId PrismFrameworkObjectManager::createFileForSyncBuffer ( const string &filenameToSync, char* &pfileBuffer, UI32 &sizeOfFileBuffer )
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    prismAssert ( NULL != pfileBuffer, __FILE__, __LINE__ );

    trace (TRACE_LEVEL_DEVEL, string("PrismFrameworkObjectManager::createFileForSyncBuffer : filename - ") + filenameToSync );
    ofstream file;
    file.open ( (filenameToSync).c_str() );

    if ( true == file.is_open() )
    {   
        if ( 0 < sizeOfFileBuffer )
        {  
            file.write ( pfileBuffer, sizeOfFileBuffer );
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::createFileForSyncBuffer : Empty file - \"") + filenameToSync + ("\"") );
        }
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;   
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::createFileForSyncBuffer : Unable to open file - \"") + filenameToSync + ("\"") );
    }
    file.close();

    return (status);
}

void PrismFrameworkObjectManager::createDatabaseBackupBuffer(char * &pDatabaseBackupBuffer, SI32 &sizeOfBackupFile)
{
    ifstream  databaseBackupFile;


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

void PrismFrameworkObjectManager::createClusterWithNodesMessageHandler (PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rollbackNodeIfRequiredStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesValidateLocationRoleStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesReplayGlobalOnlyConfigIfRequiredStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesPausePersistenceStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesAddKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConnectToNewKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesResumeDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastNewNodesAddedEventOnPrimaryStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::sendReplyBackToClusterGlobalService),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::failoverforNodesFailedInPhase2),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startHeartBeatToSecondaryNodes),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastNodesAdditionToClusterCompletedEvent),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterSuccessStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterFailureStep),
    };

    CreateClusterWithNodesContext *pCreateClusterWithNodesContext = new CreateClusterWithNodesContext (pPrismCreateClusterWithNodesMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pCreateClusterWithNodesContext->setIsCreateClusterContext (true);

    // Set an indication that cluster operation is in progress on primary node
    setPrimaryNodeClusterOperationFlag (true);
    
    // Setting the flag here instead of setting in the context constructor to help in the following case:
    // When a primary node is becoming secondary(flag is set TRUE), primary controlled failover is done
    // followed by destroy cluster which resets the flag to FALSE in base class context (CreateClusterWithNodesContext)
    // The flag should always be TRUE on a secondary node joining the cluster and should not be reset upon failover.
    // This causes issue when operations are allowed based on the status of the flag on a secondary node
    setSecondaryNodeClusterCreationFlag (false);

    pCreateClusterWithNodesContext->holdAll ();
    pCreateClusterWithNodesContext->start ();
}

void PrismFrameworkObjectManager::createClusterWithNodesValidateLocationRoleStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesValidateLocationRoleStep : Starting ...");

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    LocationRole                        currentRole                         = m_pThisLocation->getLocationRole ();
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;

    // FIXME : Sagar : Once we implement shutting down location services prior to configuring a location as part of a cluster
    //                 will make things easier.  Until then we must have following statement to adjust the sender locationid
    //                 in the cluster creation message.  Since the message has been generated, we might have changed our locationId.
    //                 Also we know that this request can only come from the CEntral cluster that is local to us.

    pPrismCreateClusterWithNodesMessage->setSenderLocationId (FrameworkToolKit::getThisLocationId ());

    if (LOCATION_PRIMARY == currentRole)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesValidateLocationRoleStep : We are already Primary of a cluster.  Will not honor the request to create a cluster.  Will not reply.");
        status = FRAMEWORK_ERROR_LOCATION_ALREADY_CONFIGURED;
    }
    if (LOCATION_SECONDARY == currentRole)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesValidateLocationRoleStep : We are already Secondary of a cluster.  Will not honor the request to create a cluster.  Will not reply.");
        status = FRAMEWORK_ERROR_LOCATION_ALREADY_CONFIGURED;
    }
    else if (currentRole == LOCATION_SUB_LOCATION)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesValidateLocationRoleStep : SubLocations cannot create clusters.  Will not reply.");
        status = FRAMEWORK_ERROR_SUB_LOCATION_CANNOT_BECOME_MEMBER_OF_A_CLUSTER;
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // In this case we do not want the reply to be sent over to the remote location that requested.
        // If we respond it may cause the response to be delivered to a wrong location as the requesting
        // location is considered to be an imposter.

        pPrismCreateClusterWithNodesMessage->setDropReplyAcrossLocations (true);
    }

    pCreateClusterWithNodesContext->executeNextStep (status);
    return;
}

void PrismFrameworkObjectManager::createClusterWithNodesValidateStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesValidateStep : Starting ...");

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    UI32                                numberOfNewNodes                    = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
    UI32                                i                                   = 0;
    string                              thisLocationIpAddress               = FrameworkToolKit::getThisLocationIpAddress ();
    SI32                                thisLocationPort                    = FrameworkToolKit::getThisLocationPort ();

    if (NULL == m_pThisLocation)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::createClusterWithNodesValidateStep : Trying to create a cluster without configuring this location first.");
        prismAssert (false, __FILE__, __LINE__);
        pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_LOCATION_NOT_CONFIGURED);
        return;
    }
    else
    {
        if (true != (FrameworkToolKit::isThisALocation ()))
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::createClusterWithNodesValidateStep : Trying to create a cluster using a sub location");
            prismAssert (false, __FILE__, __LINE__);
            pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_SUB_LOCATION_CANNOT_CREATE_CLUSTER);
            return;
        }

        // FIXME : sagar : validate if this is a Stand alone location.
    }

    for (i = 0; i < numberOfNewNodes; i++)
    {
        string ipAddress = pPrismCreateClusterWithNodesMessage->getNodeAt (i);
        SI32   port             = pPrismCreateClusterWithNodesMessage->getNodePortAt (i);

        if ("" == ipAddress)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::createClusterWithNodesValidateStep : Trying to create a cluster using NULL ipAddress (" + ipAddress + ")");
            pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_NULL_IPADDRESS);
            return;
        }

        // Check if the ipaddress represents a valid ipaddress in number and dots format.

        sockaddr_in socketAddress;

        SI32 status = inet_pton (AF_INET, ipAddress.c_str (), &socketAddress.sin_addr);

        if (0 >= status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesValidateStep : Trying to create a cluster using an invalid ipAddress (" + ipAddress + ")");
            pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_INVALID_IPADDRESS);
            return;
        }

        if (true == (FrameworkToolKit::isAnIpAddressForThisMachine (ipAddress)))
        {
            if (thisLocationPort == port)
            {
                trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::createClusterWithNodesValidateStep : Trying to create a cluster using self as partner (" + ipAddress + ")");
                pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_CREATE_HA_WITH_SELF);
                return;
            }
        }
    }

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep : Starting ...");

    vector<PrismServiceId>  prismServiceIds;
    UI32                    numberOfPrismServiceIds;
    UI32                    i;
    LocationId              thisLocationId               = FrameworkToolKit::getThisLocationId ();
    ResourceId              status                       = WAVE_MESSAGE_SUCCESS;
    vector<PrismServiceId> &prismServiceIdsToCommunicate = pCreateClusterWithNodesContext->getPrismServiceIdsToCommunicate ();

    PrismThread::getListOfServiceIds (prismServiceIds);
    numberOfPrismServiceIds = prismServiceIds.size ();

    for (i = 0; i < numberOfPrismServiceIds; i++)
    {
        trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep : Collecting Validation Information from Service : ") + prismServiceIds[i]);

        if (true == (isServiceToBeExcludedInClusterCommunications (prismServiceIds[i])))
        {
            continue;
        }

        // Collect the list of services so that we use the list later.

        prismServiceIdsToCommunicate.push_back (prismServiceIds[i]);

        WaveObjectManagerCollectValidationDataMessage message (prismServiceIds[i]);

        status  = sendSynchronously (&message, thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_COULD_NOT_COLLECT_VALIDATION_DATA;
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep : Failed to obtain Validation Details for Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Status : " + FrameworkToolKit::localize (status));
            break;
        }

        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_COULD_NOT_COLLECT_VALIDATION_DATA;
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep : Failed to obtain Validation Details for Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Completion Status : " + FrameworkToolKit::localize (status));
            break;
        }

        void *pData = NULL;
        UI32  size  = 0;

        message.getValidationDetails (pData, size);

        if ((0 != size) && (NULL != pData))
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep : Obtained Validation Data for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            pCreateClusterWithNodesContext->addValidationDetailsForService (prismServiceIds[i], pData, size);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep : No Validation details are obtained for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));
        }
    }

    pCreateClusterWithNodesContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::createClusterWithNodesAddKnownLocationsStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesAddKnownLocationsStep : Starting ...");

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    UI32                                numberOfNewNodes                    = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
    UI32                                i                                   = 0;
    UI32                                numberOfFailures                    = 0;
    string                              ipAddress                           = "";
    SI32                                port                                = 0;
    LocationId                          newLocationId                       = 0;

    for (i = 0; i < numberOfNewNodes; i++)
    {
        ipAddress = pPrismCreateClusterWithNodesMessage->getNodeAt (i);
        port      = pPrismCreateClusterWithNodesMessage->getNodePortAt (i);

        if (false == (FrameworkToolKit::isAKnownLocation (ipAddress, port)))
        {
            newLocationId = getNextLocationId ();

            if (0 != newLocationId)
            {
                m_pThisLocation->addKnownLocation (newLocationId, ipAddress, port);
                pCreateClusterWithNodesContext->addNewLocationId (newLocationId);
            }
            else
            {
                numberOfFailures++;
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesAddKnownLocationsStep Location Ids are not Available");
                pPrismCreateClusterWithNodesMessage->setNewNodeStatus (ipAddress, port, FRAMEWORK_ERROR_NO_UNUSED_LOCATION_IDS_AVAILABLE);
            }
        }
        else
        {
            numberOfFailures++;
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesAddKnownLocationsStep Already a member of cluster");
            pPrismCreateClusterWithNodesMessage->setNewNodeStatus (ipAddress, port, FRAMEWORK_ERROR_IPADDRESS_ALREADY_MEMBER_OF_CLUSTER);
        }
    }

    if (numberOfFailures == numberOfNewNodes)
    {
        trace (TRACE_LEVEL_INFO, string ("Number of failures = ") + numberOfFailures);

        resumePersistence ();
        pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_CREATE_CLUSTER_NO_VALID_RESOURCES);
    }
    else
    {
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::createClusterWithNodesConnectToNewKnownLocationsStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesConnectToNewKnownLocationsStep : Starting ...");

    static const UI32                                maximumNumberOfRetriesToConnectToARemoteLocation  = 30;
    static const UI32                                maximumNumberOfSecondsToWaitToConnect             = 30;
                 PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage               = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
                 UI32                                numberOfNewLocationIds                            = pCreateClusterWithNodesContext->getNumberOfNewLocationIds ();
                 LocationId                          newLocationId                                     = 0;
                 SI32                                i                                                 = 0;
                 string                              ipAddress                                         = "";
                 SI32                                port                                              = 0;
                 ResourceId                          status                                            = FRAMEWORK_SUCCESS;
                 UI32                                numberOfFailures                                  = 0;

    for (i = numberOfNewLocationIds - 1; i >= 0; i--)
    {
        newLocationId = pCreateClusterWithNodesContext->getNewLocationIdAt ((UI32) i);
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        if (0 != newLocationId)
        {
            status = m_pThisLocation->connectToRemoteLocation (newLocationId, maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect);

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::createClusterWithNodesConnectToNewKnownLocationsStep : failed to connect IP = ") + ipAddress + 
                    " Port = " + port + " newLocation = " + newLocationId);
                // Indicate that the status saying that we could not connect to the location.
                // For this location we will not send a message to configure itself as a secondary.

                pPrismCreateClusterWithNodesMessage->setNewNodeStatus (ipAddress, port, status);

                // Since we are not able to connect to the location remove it from the known locations.

                removeKnownLocation (newLocationId);
                pCreateClusterWithNodesContext->removeNewLocationId (newLocationId);
                numberOfFailures++;
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    if (numberOfFailures == numberOfNewLocationIds)
    {
        resumePersistence ();
        pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONECT_TO_ALL_NEW_LOCATIONS);
    }
    else
    {
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::createClusterWithNodesReplayGlobalOnlyConfigIfRequiredStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesReplayGlobalOnlyConfigIfRequiredStep : Starting ...");

    string configFilePath           = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
    string configFilename           = ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ();
    string fullConfigFilename       = configFilePath + "/" + configFilename;
    string preConfigReplayCommands  = "bypassCliValidations cluster-formation";

    if (true == ConfigFileManagementToolKit::isFileExisting (fullConfigFilename))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesReplayGlobalOnlyConfigIfRequiredStep : Replay global-only config file on principal node...");

        status = ConfigFileManagementToolKit::replayConfigurationFile (configFilePath, configFilename, preConfigReplayCommands);

        
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesReplayGlobalOnlyConfigIfRequiredStep : Replay of global-only config file on principal node completed with status: " + FrameworkToolKit::localize (status));

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            string newConfigFilename = fullConfigFilename + "." + FrameworkToolKit::getThisLocationIpAddress () + ".backup";

            ConfigFileManagementToolKit::renameConfigurationFile (fullConfigFilename, newConfigFilename);
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesReplayGlobalOnlyConfigIfRequiredStep : Skipping this step. " + fullConfigFilename + " does not exist.");
    }

    pCreateClusterWithNodesContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::backUpDatabaseAfterSendingClusterPhase0Message ()
{
    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName ());

    DatabaseObjectManagerBackupMessage message (backupFileName);
    message.setDataOnlyBackup (true);

    ResourceId status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::backUpDatabaseAfterSendingClusterPhase0Message : Could not send message to backup database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::backUpDatabaseAfterSendingClusterPhase0Message : Message to backup database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }     
        else  
        {     
             trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::backUpDatabaseAfterSendingClusterPhase0Message : Successfully backed up the database.");
        }     
    }
}

void PrismFrameworkObjectManager::createClusterWithNodesPausePersistenceStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    // Pause the persistence object manager before taking a backup of the DB.

    pausePersistence ();

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsStep : Starting ...");

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    UI32                                numberOfNewLocationIds              = pCreateClusterWithNodesContext->getNumberOfNewLocationIds ();
    UI32                                i                                   = 0;
    LocationId                          thisLocationId                      = FrameworkToolKit::getThisLocationId ();
    SI32                                thisLocationPort                    = FrameworkToolKit::getThisLocationPort ();
    LocationId                          newLocationId                       = 0;
    string                              thisLocationIpAddress               = FrameworkToolKit::getThisLocationIpAddress ();
    string                              ipAddress                           = "";
    SI32                                port                                = 0;
    UI32                                numberOfFailures                    = 0;
    string                              prismVersionString                  = PrismVersion::getVersionString ();



    prismAssert (NULL != pPrismCreateClusterWithNodesMessage, __FILE__, __LINE__);

    ++(*pCreateClusterWithNodesContext);

    for (i = 0; i < numberOfNewLocationIds; i++)
    {
        newLocationId = pCreateClusterWithNodesContext->getNewLocationIdAt ((UI32) i);
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        // We will send configure message only to the locations that are known to us at this point.  If user supplied
        // a bogus ipaddress, we would have timed out trying to add that location in the previous step
        // and hence that location will not be known to us at this time.

        if (true == (FrameworkToolKit::isAKnownLocation (ipAddress, port)))
        {
            PrismConfigureClusterSecondaryMessage *pMessage = new PrismConfigureClusterSecondaryMessage (prismVersionString, thisLocationIpAddress, thisLocationPort, thisLocationId, ipAddress, port, newLocationId);

            // Add all the validation buffers to the message before sending out

            vector<PrismServiceId> &prismServiceIds                = pCreateClusterWithNodesContext->getPrismServiceIdsVector ();
            vector<void *>         &validationDetailsVector        = pCreateClusterWithNodesContext->getValidationDetailsVector ();
            vector<UI32>           &validationDetailsSizesVector   = pCreateClusterWithNodesContext->getValidationDetailsSizesVector ();

            UI32                    numberOfPrismServiceIds        = prismServiceIds.size ();
            UI32                    numberOfValidationDetails      = validationDetailsVector.size ();
            UI32                    numberOfValidationDetailsSizes = validationDetailsSizesVector.size ();
            UI32                    j                              = 0;

            prismAssert (numberOfPrismServiceIds == numberOfValidationDetails, __FILE__, __LINE__);
            prismAssert (numberOfPrismServiceIds == numberOfValidationDetailsSizes, __FILE__, __LINE__);

            for (j = 0; j < numberOfPrismServiceIds; j++)
            {
                if ((0 != validationDetailsSizesVector[j]) && (NULL != validationDetailsVector[j]))
                {
                    pMessage->addBuffer (prismServiceIds[j], validationDetailsSizesVector[j], validationDetailsVector[j]);
                }
                else
                {
                    trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsStep : We could not have added an entry with 0 size and/or NULL validation buffer.");
                    prismAssert (false, __FILE__, __LINE__);
                }
            }

            pMessage->setSchemaChangeVectors ();

            pMessage->setIsAddNodeForSpecialCaseFlag (pPrismCreateClusterWithNodesMessage->getIsAddNodeForSpecialCaseFlag ());

            pMessage->setToAllowSendForOneWayCommunication (true);

            WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsCallback), pCreateClusterWithNodesContext, s_clusterPhase0TimeoutInMilliseconds, newLocationId);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                removeKnownLocation (newLocationId);
                pPrismCreateClusterWithNodesMessage->setNewNodeStatus (ipAddress, port, status);
                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsStep : Not able to configure one secondary location : ") + newLocationId + string (" Error Code : ") + status);
                numberOfFailures++;

                delete pMessage;
            }
            else
            {
                ++(*pCreateClusterWithNodesContext);
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    backUpDatabaseAfterSendingClusterPhase0Message ();

    --(*pCreateClusterWithNodesContext);

    if (numberOfFailures == numberOfNewLocationIds)
    {
        resumeDatabaseOperation ();
        
        pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsCallback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsCallback : Starting ...");

    CreateClusterWithNodesContext      *pCreateClusterWithNodesContext      = reinterpret_cast<CreateClusterWithNodesContext *> (pContext);
    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    LocationId                          newLocationId;
    string                              ipAddress;
    SI32                                port;

    --(*pCreateClusterWithNodesContext);

    if (NULL != pPrismConfigureClusterSecondaryMessage)
    {
        newLocationId = pPrismConfigureClusterSecondaryMessage->getLocationId ();
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        pCreateClusterWithNodesContext->addValidaionResultsLocation (ipAddress, port);

        vector<PrismServiceId> &prismServiceIdsToCommunicate = pCreateClusterWithNodesContext->getPrismServiceIdsToCommunicate ();
        UI32                    numberOfPrismServices        = prismServiceIdsToCommunicate.size ();
        UI32                    i                            = 0;

        for (i = 0; i < numberOfPrismServices; i++)
        {
            void *pValidationResults    = NULL;
            UI32  validationResultsSize = 0;

            pValidationResults = pPrismConfigureClusterSecondaryMessage->transferBufferToUser (s_offSetForValidationResults + prismServiceIdsToCommunicate[i], validationResultsSize);

            pCreateClusterWithNodesContext->addValidationResultsForService (ipAddress, port, prismServiceIdsToCommunicate[i], pValidationResults, validationResultsSize);
        }

        if (FRAMEWORK_SUCCESS == frameworkStatus)
        {
            prismAssert (NULL != pPrismConfigureClusterSecondaryMessage, __FILE__, __LINE__);

            ResourceId completionStatus = pPrismConfigureClusterSecondaryMessage->getCompletionStatus ();

            pPrismCreateClusterWithNodesMessage->setNewNodeStatus (ipAddress, port, completionStatus);

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsCallback : Configuring ") + ipAddress + " failed : " + completionStatus);
                removeKnownLocation (newLocationId);
            }
        }
        else
        {
            if (NULL != pPrismConfigureClusterSecondaryMessage)
            {
                LocationId newLocationId1 = pPrismConfigureClusterSecondaryMessage->getLocationId ();
                string     ipAddress1     = FrameworkToolKit::getIpAddressForLocationId (newLocationId1);
                SI32       port1          = FrameworkToolKit::getPortForLocationId (newLocationId1);

                pPrismCreateClusterWithNodesMessage->setNewNodeStatus (ipAddress1, port1, frameworkStatus);
    
                removeKnownLocation (newLocationId1);
            }
        }

    }

    if (NULL != pPrismConfigureClusterSecondaryMessage)
    {
        delete pPrismConfigureClusterSecondaryMessage;
    }

    if (0 == (pCreateClusterWithNodesContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        UI32       numberOfNewIpAddresses  = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
        UI32       i                       = 0;
        LocationId newLocationId1          = 0;
        string     ipAddress1              = "";
        SI32       port1                   = 0;
        UI32       numberOfFailures        = 0;

        for (i = 0; i < numberOfNewIpAddresses; i++)
        {
            ipAddress1     = pPrismCreateClusterWithNodesMessage->getNodeAt ((UI32) i);
            port1          = pPrismCreateClusterWithNodesMessage->getNodePortAt ((UI32) i);
            newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

            if (true != (pPrismCreateClusterWithNodesMessage->isNewNodeStatusSet (ipAddress1, port1)))
            {
                pPrismCreateClusterWithNodesMessage->setNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_TIMED_OUT);

                if (0 != newLocationId1)
                {
                    removeKnownLocation (newLocationId1);
                }

                numberOfFailures++;
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != (pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1)))
                {
                    numberOfFailures++;
                }
            }
        }

        if (numberOfFailures == numberOfNewIpAddresses)
        {
            resumeDatabaseOperation ();
            
            pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
        }
        else
        {
            m_pThisLocation->setLocationRole (LOCATION_PRIMARY);
            pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step : Starting ...");

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());

    prismAssert (NULL != pPrismCreateClusterWithNodesMessage, __FILE__, __LINE__);

    UI32                                i                                   = 0;
    UI32                                numberOfNewIpAddresses              = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
    LocationId                          newLocationId1                      = 0;
    string                              ipAddress1                          = "";     SI32                                port1                               = 0;
    UI32                                numberOfFailures                    = 0;
    SI32                                sizeOfBackupFile                    = 0;
    char                               *pDatabaseBackupBuffer               = NULL;
    vector<string>                      filenamesToSync;
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;
    bool                                fileSyncRequired                    = false;
    
    ++(*pCreateClusterWithNodesContext);


    pPrismCreateClusterWithNodesMessage->getFilenamesToSync ( filenamesToSync );    
    
    if ( filenamesToSync.size() == numberOfNewIpAddresses )
    {
        fileSyncRequired = true ;
    }     
        
    createDatabaseBackupBuffer(pDatabaseBackupBuffer, sizeOfBackupFile);
    
    for (i = 0; i < numberOfNewIpAddresses; i++)
    {
        ipAddress1     = pPrismCreateClusterWithNodesMessage->getNodeAt ((UI32) i);
        port1          = pPrismCreateClusterWithNodesMessage->getNodePortAt ((UI32) i);
        newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

        // We will send configure phase1 message only to the locations that are known to us at this point.
        // Only to those locations that are succeeded as part of phase 0.
        // If user supplied a bogus ipaddress, we would have timed out trying to add that location in the prior steps
        // and hence that location will not be known to us at this time.

        if (WAVE_MESSAGE_SUCCESS == (pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1)))
        {
            PrismConfigureClusterSecondaryPhase1Message *pMessage = new PrismConfigureClusterSecondaryPhase1Message ();

            if (NULL != pDatabaseBackupBuffer)
            {
                pMessage->addBuffer (pMessage->getDatabaseBackupBufferTag (), sizeOfBackupFile, pDatabaseBackupBuffer, false);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step : No Database Backup is being sent to the lcoation.");
            }
   
            if ( (true == fileSyncRequired) && (false == filenamesToSync[i].empty()) )
            {    
                char *pfileBuffer = NULL ;
                UI32  sizeOfFileBuffer;

                trace (TRACE_LEVEL_DEVEL, string("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step : Adding file buffers. FileNum - ") + i );
                //using tag starting from 11 for now.        
                status = createBufferForFileToSync ( filenamesToSync[i], pfileBuffer, sizeOfFileBuffer );
                if ( WAVE_MESSAGE_SUCCESS != status )
                {
                    trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step : Unable to create buffer for file = \"") + filenamesToSync[i] + ("\"") );
                    prismAssert ( false, __FILE__, __LINE__ );
                }                        

                pMessage->addBuffer ( 11, sizeOfFileBuffer, pfileBuffer, false );
                pMessage->setFilenameToSync ( filenamesToSync[i] );
                delete[] (pfileBuffer);
                pfileBuffer = NULL;
            }

            status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Callback), pCreateClusterWithNodesContext, s_clusterPhase1TimeoutInMilliseconds, newLocationId1);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                removeKnownLocation (newLocationId1);
                pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, status);
                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step : Not able to configure one secondary location During Phase 2: ") + newLocationId1 + string (" Status : ") + FrameworkToolKit::localize (status));
                numberOfFailures++;

                delete pMessage;
            }
            else
            {
                pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE1_UNKNOWN);
                ++(*pCreateClusterWithNodesContext);
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    --(*pCreateClusterWithNodesContext);

    if (NULL != pDatabaseBackupBuffer)
    {
        delete[] pDatabaseBackupBuffer;
    }

    if ( numberOfFailures == numberOfNewIpAddresses )
    {
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step : , numberOfFailures - ") + numberOfFailures );
        resumeDatabaseOperation ();
        
        if (true == pCreateClusterWithNodesContext->getIsCreateClusterContext ())
        {
           m_pThisLocation->setLocationRole (LOCATION_STAND_ALONE);        
        }

        pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}


void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Callback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryPhase1Message *pPrismConfigureClusterSecondaryPhase1Message, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Callback : Starting ...");

    CreateClusterWithNodesContext      *pCreateClusterWithNodesContext      = reinterpret_cast<CreateClusterWithNodesContext *> (pContext);
    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    LocationId                          newLocationId;
    string                              ipAddress;
    SI32                                port;

    --(*pCreateClusterWithNodesContext); 

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismConfigureClusterSecondaryPhase1Message, __FILE__, __LINE__);

        newLocationId = pPrismConfigureClusterSecondaryPhase1Message->getReceiverLocationId ();
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        ResourceId completionStatus = pPrismConfigureClusterSecondaryPhase1Message->getCompletionStatus ();

        pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Callback : Case 1 - Configuring ") + ipAddress + " failed : " + completionStatus);
            removeKnownLocation (newLocationId);
        }
    }
    else
    {
        if (NULL != pPrismConfigureClusterSecondaryPhase1Message)
        {
            LocationId newLocationId1 = pPrismConfigureClusterSecondaryPhase1Message->getReceiverLocationId ();
            string     ipAddress1     = FrameworkToolKit::getIpAddressForLocationId (newLocationId1);
            SI32       port1          = FrameworkToolKit::getPortForLocationId (newLocationId1);

            pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, frameworkStatus);

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Callback : Case 2 - Configuring ") + ipAddress + " failed : " + frameworkStatus);
            removeKnownLocation (newLocationId1);
        }
    }

    if (NULL != pPrismConfigureClusterSecondaryPhase1Message)
    {
        delete pPrismConfigureClusterSecondaryPhase1Message;
    }

    if (0 == (pCreateClusterWithNodesContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        UI32       numberOfNewIpAddresses  = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
        UI32       i                       = 0;
        LocationId newLocationId1          = 0;
        string     ipAddress1              = "";
        SI32       port1                   = 0;
        UI32       numberOfFailures        = 0;

        for (i = 0; i < numberOfNewIpAddresses; i++)
        {
            ipAddress1     = pPrismCreateClusterWithNodesMessage->getNodeAt ((UI32) i);
            port1          = pPrismCreateClusterWithNodesMessage->getNodePortAt ((UI32) i);
            newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

            if (FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE1_UNKNOWN == (pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1)))
            {
                pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE1_TIMED_OUT);

                if (0 != newLocationId1)
                {
                    trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Callback : Case 3 - Configuring ") + ipAddress + " Timeout ");
                    removeKnownLocation (newLocationId1);
                }

                numberOfFailures++;
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != (pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1)))
                {
                    numberOfFailures++;
                }
            }
        }

        if (numberOfFailures == numberOfNewIpAddresses)
        {
            resumeDatabaseOperation ();
            
            if (true == pCreateClusterWithNodesContext->getIsCreateClusterContext ())
            {
               m_pThisLocation->setLocationRole (LOCATION_STAND_ALONE);
            }

            pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
        }
        else
        {
            pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }       
}

void PrismFrameworkObjectManager::resumeDatabase ()
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::resumeDatabase : Starting ...");

    PrismResumeObjectManagerMessage message (DatabaseObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::resumeDatabase : Could not resume Database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::resumeDatabase : Could not resume Database.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::resumeDatabase : Successfully resumed Database.");
        }
    }
}

void PrismFrameworkObjectManager::pausePersistence ()
{

    CliBlockManagementToolKit::lock ();

    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::pausePersistence : Starting ...");

    PrismPauseObjectManagerMessage message (PersistenceObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::pausePersistence : Could not pause Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::pausePersistence : Could not pause Persistence.  Completion Status : " + FrameworkToolKit::localize (status));            
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::pausePersistence : Successfully paused Persistence.");
        }
    }
}

void PrismFrameworkObjectManager::resumePersistence ()
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::resumePersistence : Starting ...");

    PrismResumeObjectManagerMessage message (PersistenceObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::resumePersistence : Could not resume Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::resumePersistence : Could not resume Persistence.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::resumePersistence : Successfully resumed Persistence.");
        }
    }

    CliBlockManagementToolKit::unlock ();
}

void PrismFrameworkObjectManager::resumeDatabaseOperation ()
{
    resumeDatabase ();
    resumePersistence ();
}

void PrismFrameworkObjectManager::createClusterWithNodesResumeDatabaseStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesResumeDatabaseStep : Starting ...");

    resumeDatabaseOperation ();
    
    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Step (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Step : Starting ...");

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());

    prismAssert (NULL != pPrismCreateClusterWithNodesMessage, __FILE__, __LINE__);

    UI32                                i                                   = 0;
    UI32                                numberOfNewIpAddresses              = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
    LocationId                          newLocationId1                      = 0;
    string                              ipAddress1                          = "";
    SI32                                port1                               = 0;
    UI32                                numberOfFailures                    = 0;
    UI32                                numberOfNewLocationIds              = pCreateClusterWithNodesContext->getNumberOfNewLocationIds ();

    ++(*pCreateClusterWithNodesContext);

    for (i = 0; i < numberOfNewIpAddresses; i++)
    {
        ipAddress1     = pPrismCreateClusterWithNodesMessage->getNodeAt ((UI32) i);
        port1          = pPrismCreateClusterWithNodesMessage->getNodePortAt ((UI32) i);
        newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

        // We will send configure phase2 message only to the locations that are known to us at this point.
        // Only to those locations that are succeeded as part of phase 1.
        // If user supplied a bogus ipaddress, we would have timed out trying to add that location in the prior steps
        // and hence that location will not be known to us at this time.

        if (WAVE_MESSAGE_SUCCESS == (pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1)))
        {
            PrismConfigureClusterSecondaryPhase2Message *pMessage = new PrismConfigureClusterSecondaryPhase2Message ();

            WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Callback), pCreateClusterWithNodesContext, 120000, newLocationId1);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                removeKnownLocation (newLocationId1);
                pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, status);
                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Step : Not able to configure one secondary location During Phase 2: ") + newLocationId1 + string (" Status : ") + FrameworkToolKit::localize (status));
                numberOfFailures++;

                delete pMessage;
            }
            else
            {
                pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE2_UNKNOWN);

                ++(*pCreateClusterWithNodesContext);
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    --(*pCreateClusterWithNodesContext);

    if (numberOfFailures == numberOfNewLocationIds)
    {
//        resumeDatabaseOperation ();               already done successfully as last step.

        if (true == pCreateClusterWithNodesContext->getIsCreateClusterContext ())
        {
           m_pThisLocation->setLocationRole (LOCATION_STAND_ALONE);
        }


        pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Callback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryPhase2Message *pPrismConfigureClusterSecondaryPhase2Message, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Callback : Starting ...");

    CreateClusterWithNodesContext      *pCreateClusterWithNodesContext      = reinterpret_cast<CreateClusterWithNodesContext *> (pContext);
    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    LocationId                          newLocationId;
    string                              ipAddress;
    SI32                                port;

    --(*pCreateClusterWithNodesContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismConfigureClusterSecondaryPhase2Message, __FILE__, __LINE__);

        newLocationId = pPrismConfigureClusterSecondaryPhase2Message->getReceiverLocationId ();
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        ResourceId completionStatus = pPrismConfigureClusterSecondaryPhase2Message->getCompletionStatus ();

        pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Callback : Case 1 - Configuring ") + ipAddress + " failed : " + completionStatus);

            disconnectFromLocation (newLocationId);
            removeKnownLocation (newLocationId);

            pCreateClusterWithNodesContext->addToFailedLocationIdVector (newLocationId);
        }
    }
    else
    {
        if (NULL != pPrismConfigureClusterSecondaryPhase2Message)
        {
            LocationId newLocationId1 = pPrismConfigureClusterSecondaryPhase2Message->getReceiverLocationId ();
            string     ipAddress1     = FrameworkToolKit::getIpAddressForLocationId (newLocationId1);
            SI32       port1          = FrameworkToolKit::getPortForLocationId (newLocationId1);

            pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, frameworkStatus);

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Callback : Case 2 - Configuring ") + ipAddress + " failed : " + frameworkStatus);

            disconnectFromLocation (newLocationId1);
            removeKnownLocation (newLocationId1);

            pCreateClusterWithNodesContext->addToFailedLocationIdVector (newLocationId1);
        }
    }

    if (NULL != pPrismConfigureClusterSecondaryPhase2Message)
    {
        delete pPrismConfigureClusterSecondaryPhase2Message;
    }

    if (0 == (pCreateClusterWithNodesContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        UI32       numberOfNewIpAddresses  = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
        UI32       i                       = 0;
        LocationId newLocationId1          = 0;
        string     ipAddress1              = "";
        SI32       port1                   = 0;
        UI32       numberOfFailures        = 0;

        for (i = 0; i < numberOfNewIpAddresses; i++)
        {
            ipAddress1     = pPrismCreateClusterWithNodesMessage->getNodeAt ((UI32) i);
            port1          = pPrismCreateClusterWithNodesMessage->getNodePortAt ((UI32) i);
            newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

            if (FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE2_UNKNOWN == (pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1)))
            {
                pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE2_TIMED_OUT);

                if (0 != newLocationId1)
                {
                    trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Callback : Case 3 - Configuring ") + ipAddress + " Timeout ");

                    disconnectFromLocation (newLocationId1);
                    removeKnownLocation (newLocationId1);

                    pCreateClusterWithNodesContext->addToFailedLocationIdVector (newLocationId1);
                }

                numberOfFailures++;
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != (pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1)))
                {
                    numberOfFailures++;
                }
            }

            if (WAVE_MESSAGE_SUCCESS == pPrismCreateClusterWithNodesMessage->getNewNodeStatus (ipAddress1, port1))
            {
                pCreateClusterWithNodesContext->addToSuccessfullyAddedLocationIdVector (newLocationId1);
            }
        }

        if (numberOfFailures == numberOfNewIpAddresses)
        {
            if (true == pCreateClusterWithNodesContext->getIsCreateClusterContext ())
            {
               m_pThisLocation->setLocationRole (LOCATION_STAND_ALONE);
            }

            vector<LocationId> failedLocationIdsVector            = pCreateClusterWithNodesContext->getFailedLocationIdVector ();
            UI32               numberOffailedLocationIdsVector    = failedLocationIdsVector.size ();

            if (0 < numberOffailedLocationIdsVector)
            {

                PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInPhase2Callback), pCreateClusterWithNodesContext);

                for (UI32 k = 0; k < numberOffailedLocationIdsVector; k++)
                {
                    pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVector[k]);
                }

                pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
                pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);

                tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::Phase 2 Call Back Invoking failover for %u nodes", numberOffailedLocationIdsVector);

                m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
            }
        }
        else
        {
            m_pThisLocation->setLocationRole (LOCATION_PRIMARY);

            pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::failoverforNodesFailedInPhase2 (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    vector<LocationId> failedLocationIdsVector            = pCreateClusterWithNodesContext->getFailedLocationIdVector ();
    UI32               numberOffailedLocationIdsVector    = failedLocationIdsVector.size ();

    if (0 < numberOffailedLocationIdsVector)
    {
        PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInPhase2Callback), pCreateClusterWithNodesContext);

        for (UI32 k = 0; k < numberOffailedLocationIdsVector; k++)
        {
            pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVector[k]);
        }

        pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
        pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);

        tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::failoverforNodesFailedInPhase2 Invoking failover for %u nodes", numberOffailedLocationIdsVector);

        m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
    }
    else
    {
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}

void PrismFrameworkObjectManager::failoverforNodesFailedInPhase2Callback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::failoverforNodesFailedInPhase2Callback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    CreateClusterWithNodesContext  *pCreateClusterWithNodesContext = reinterpret_cast<CreateClusterWithNodesContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pCreateClusterWithNodesContext, __FILE__, __LINE__);

    //clear the failed location vector setup in previous phases as failover is triggered for those locations.
    pCreateClusterWithNodesContext->clearFailedLocationIdVector ();

    vector<LocationId>      successfullyAddedLocationIdVector = pCreateClusterWithNodesContext->getSuccessfullyAddedLocationIdVector ();
                 UI32       numberOfSuccessfullyAddedNode     = successfullyAddedLocationIdVector.size ();

    if (0 == numberOfSuccessfullyAddedNode)
    {
        if (true == pCreateClusterWithNodesContext->getIsCreateClusterContext ())
        {
            m_pThisLocation->setLocationRole (LOCATION_STAND_ALONE);

            saveConfiguration (true);
        }

        pCreateClusterWithNodesContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else 
    { 
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }


}

/*
Name: startHeartBeatToSecondaryNodes
Description: Start HeartBeat from Primary to Secondary Node After Phase 2 completes
@param1 CreateClusterWithNodesContext: This is the base class context. This function is called from Create Cluster, Add Node, Rejoin with respective Context.

*/
void PrismFrameworkObjectManager::startHeartBeatToSecondaryNodes (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::startHeartBeatToSecondaryNodes Entring");

    vector<LocationId>  successfullyAddedLocationIdVector = pCreateClusterWithNodesContext->getSuccessfullyAddedLocationIdVector ();
    UI32                numberOfSuccessfullyAddedNode     = successfullyAddedLocationIdVector.size ();
    LocationId          locationId;
    string              ipAddress;
    SI32                port;
    ResourceId          status                            = WAVE_MESSAGE_SUCCESS;

    for (UI32 i = 0; i < numberOfSuccessfullyAddedNode; i++)
    {
        locationId = successfullyAddedLocationIdVector[i];

        if (true == pCreateClusterWithNodesContext->isAFailedLocation (locationId))
        {
            continue;
        }

        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId (locationId);

        StartHeartBeatMessage *pMessage = new StartHeartBeatMessage ();

        pMessage->setDstIpAddress               (ipAddress);
        pMessage->setDstPortNumber              (port);
        pMessage->setHeartBeatInterval          (DEFAULT_HEART_BEAT_PERIOD);
        pMessage->setHeartBeatFailureThreshold  (DEFAULT_MAX_LOST_HEARTBEATS);

        status = sendSynchronously (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismFrameworkObjectManager::startHeartBeatToSecondaryNodes: Could not start heart beating with Node : ") + ipAddress + string (", Port : ") + port + string (", Status = ") + FrameworkToolKit::localize (status));

            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            status = pMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFrameworkObjectManager::startHeartBeatToSecondaryNodes : Could not start heart beating with Node : ") + ipAddress + string (", Port : ") + port + string (", Completion Status = ") + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }

            trace (TRACE_LEVEL_INFO, string ("Started HeartBeat with node ") + ipAddress + string (":") + port);
        }

        delete pMessage;
    }

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step : Starting ...");

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    vector<LocationId>                  successfullyAddedLocationIdVector   = pCreateClusterWithNodesContext->getSuccessfullyAddedLocationIdVector ();
                 UI32                   numberOfSuccessfullyAddedNode       = successfullyAddedLocationIdVector.size ();
                 UI32                   numberOfFailures                    = 0;
           LocationId                   locationId;    

    ++(*pCreateClusterWithNodesContext);

    for (UI32 i = 0; i < numberOfSuccessfullyAddedNode; i++)
    {

        locationId = successfullyAddedLocationIdVector[i];

        PrismConfigureClusterSecondaryPhase3Message *pPrismConfigureClusterSecondaryPhase3Message = new PrismConfigureClusterSecondaryPhase3Message ();

        WaveMessageStatus status = send (pPrismConfigureClusterSecondaryPhase3Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Callback), pCreateClusterWithNodesContext, 0, locationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            if (NULL != pPrismCreateClusterWithNodesMessage)
            {
                pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (FrameworkToolKit::getIpAddressForLocationId (locationId), FrameworkToolKit::getPortForLocationId (locationId), status);
            }

            disconnectFromLocation (locationId);
            removeKnownLocation (locationId);

            pCreateClusterWithNodesContext->addToFailedLocationIdVector (locationId);

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step : Not able to configure one secondary location During Phase 3: ") + locationId + string (" Status : ") + FrameworkToolKit::localize (status));

            numberOfFailures++;

            delete (pPrismConfigureClusterSecondaryPhase3Message);
        }
        else
        {
            ++(*pCreateClusterWithNodesContext);
        }
    }

    --(*pCreateClusterWithNodesContext);

    // Replay the local-only configuration file on principal node only in the case of a cluster transition.

    string configFilePath           = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
    string configFilename           = ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();
    string fullConfigFilename       = configFilePath + "/" + configFilename;
    string preConfigReplayCommands  = "bypassCliValidations cluster-formation";

    if (true == ConfigFileManagementToolKit::isFileExisting (fullConfigFilename))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step : Replay local-only config file on principal node...");

        ResourceId status = ConfigFileManagementToolKit::replayConfigurationFile (configFilePath, configFilename, preConfigReplayCommands);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step : Replay of local-only config file on principal node completed with status: " + FrameworkToolKit::localize (status));

        if (WAVE_MESSAGE_SUCCESS ==  status)
        {
            string newConfigFilename = fullConfigFilename + "." + FrameworkToolKit::getThisLocationIpAddress () + ".backup";

            ConfigFileManagementToolKit::renameConfigurationFile (fullConfigFilename, newConfigFilename);
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step : Skipping this step. " + fullConfigFilename + " does not exist.");
    }

    // Do failover if all nodes failed to be added in phase3.

    if (numberOfFailures == numberOfSuccessfullyAddedNode)
    {
        vector<LocationId> failedLocationIdsVectorInPhase3                   = pCreateClusterWithNodesContext->getFailedLocationIdVector ();
                      UI32 numberOffailedLocationIdsVectorInPhase3           = failedLocationIdsVectorInPhase3.size ();

            PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInPhase3Callback), pCreateClusterWithNodesContext);

            for (UI32 j = 0; j < numberOffailedLocationIdsVectorInPhase3; j++)
            {
                pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVectorInPhase3[j]);
            }

            pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
            pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);

            tracePrintf (TRACE_LEVEL_INFO, "Invoking failover for %u nodes", numberOffailedLocationIdsVectorInPhase3);

            m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
    }
    else
    {
        pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Callback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryPhase3Message *pPrismConfigureClusterSecondaryPhase3Message, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Callback : Starting ...");

    CreateClusterWithNodesContext      *pCreateClusterWithNodesContext      = reinterpret_cast<CreateClusterWithNodesContext *> (pContext);
    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = reinterpret_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());

    prismAssert (NULL != pPrismConfigureClusterSecondaryPhase3Message, __FILE__, __LINE__);
 
    LocationId locationId = pPrismConfigureClusterSecondaryPhase3Message->getReceiverLocationId ();
    
    --(*pCreateClusterWithNodesContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        ResourceId completionStatus = pPrismConfigureClusterSecondaryPhase3Message->getCompletionStatus ();

        if (NULL != pPrismCreateClusterWithNodesMessage)
        {
            pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (FrameworkToolKit::getIpAddressForLocationId (locationId), FrameworkToolKit::getPortForLocationId (locationId), completionStatus);
        }

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Callback : Failed to configure Location Id ") + locationId + " status : " + completionStatus);
            disconnectFromLocation (locationId);
            removeKnownLocation (locationId);

            pCreateClusterWithNodesContext->addToFailedLocationIdVector (locationId);
        }
    }
    else
    {
         trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Callback : Failed to configure Location Id ") + locationId + " status : " + frameworkStatus);

        if (NULL != pPrismCreateClusterWithNodesMessage)
        {
            pPrismCreateClusterWithNodesMessage->updateNewNodeStatus (FrameworkToolKit::getIpAddressForLocationId (locationId), FrameworkToolKit::getPortForLocationId (locationId), frameworkStatus);
        }

        disconnectFromLocation (locationId);
        removeKnownLocation (locationId);

        pCreateClusterWithNodesContext->addToFailedLocationIdVector (locationId);
    }

    if (0 == (pCreateClusterWithNodesContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        vector<LocationId> failedLocationIdsVectorInPhase3                   = pCreateClusterWithNodesContext->getFailedLocationIdVector ();
                      UI32 numberOffailedLocationIdsVectorInPhase3           = failedLocationIdsVectorInPhase3.size ();
        
        if (false == pCreateClusterWithNodesContext->getIsCreateClusterContext ())
        {
            vector<WaveManagedObject *> *pResults = querySynchronously (PrismCluster::getClassName ());

            prismAssert (NULL != pResults, __FILE__, __LINE__);

            if (0 == pResults->size ())
            {
                if (NULL != pPrismConfigureClusterSecondaryPhase3Message)
                {
                    delete pPrismConfigureClusterSecondaryPhase3Message;
                }

                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

                pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_ERROR);
                return;
            }
            else
            {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }

        // check if any node is failed, then trigger failover for those nodes.
        if (0 < numberOffailedLocationIdsVectorInPhase3)
        {
            
            PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInPhase3Callback), pCreateClusterWithNodesContext);

            for (UI32 j = 0; j < numberOffailedLocationIdsVectorInPhase3; j++)
            {
                pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVectorInPhase3[j]);
            }

            pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
            pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);

            tracePrintf (TRACE_LEVEL_INFO, "Invoking failover for %u nodes", numberOffailedLocationIdsVectorInPhase3);

            m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
        }
        else
        {
            pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }

    delete (pPrismConfigureClusterSecondaryPhase3Message);
}

void PrismFrameworkObjectManager::failoverforNodesFailedInPhase3Callback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::failoverforNodesFailedInPhase3Callback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    CreateClusterWithNodesContext  *pCreateClusterWithNodesContext = reinterpret_cast<CreateClusterWithNodesContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    vector<LocationId> failedLocationIds;
    vector<LocationId> connectedLocationsVector;

    FrameworkToolKit::getConnectedLocations (connectedLocationsVector);
    UI32 totalNodes = connectedLocationsVector.size ();

    pPrismFrameworkFailoverWorkerContext->getFailedLocationIds (failedLocationIds);

    for (UI32 i = 0; i < totalNodes; i++)
    {
        FrameworkObjectManagerRemoveKnownLocationsMessage *pFrameworkObjectManagerRemoveKnownLocationsMessage = new FrameworkObjectManagerRemoveKnownLocationsMessage ();

        
        pFrameworkObjectManagerRemoveKnownLocationsMessage->setFailedLocations (failedLocationIds);

        ResourceId status = sendOneWay (pFrameworkObjectManagerRemoveKnownLocationsMessage, connectedLocationsVector[i]);

        prismAssert (WAVE_MESSAGE_SUCCESS == status, __FILE__, __LINE__);
    }

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pCreateClusterWithNodesContext, __FILE__, __LINE__);

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

    
}

void PrismFrameworkObjectManager::setWarmHaRecoveryPreparationInProgress (const bool &isWarmRecovery)
{
    m_warmHaRecoveryPreparationInProgressMutex.lock ();
    m_isWarmHaRecoveryPreparationInProgress = isWarmRecovery;
    m_warmHaRecoveryPreparationInProgressMutex.unlock ();
}

bool PrismFrameworkObjectManager::isWarmHaRecoveryPreparationInProgress ()
{
    bool isWarmRecovery = false;

    m_warmHaRecoveryPreparationInProgressMutex.lock ();
    isWarmRecovery = m_isWarmHaRecoveryPreparationInProgress;
    m_warmHaRecoveryPreparationInProgressMutex.unlock ();

    return (isWarmRecovery);
}

void PrismFrameworkObjectManager::setActivationCompleted (bool isActivationCompleted)
{
    m_activationCompletedMutex.lock();
    m_activationCompleted = isActivationCompleted;
    generateTimeStamp (m_activationCompletedTimeStamp);
    m_activationCompletedMutex.unlock();
}

bool  PrismFrameworkObjectManager::getActivationCompleted ()
{
    m_activationCompletedMutex.lock();
    bool isActivationCompleted =  m_activationCompleted;
    m_activationCompletedMutex.unlock();
    return isActivationCompleted;
}

string PrismFrameworkObjectManager::getActivationCompletedTimeStamp()
{
    m_activationCompletedMutex.lock ();
    string activationCompletedTimeStamp = m_activationCompletedTimeStamp;
    m_activationCompletedMutex.unlock ();
    return activationCompletedTimeStamp;
}

void PrismFrameworkObjectManager::incrementLineCardPostBootCount ()
{
    m_lineCardPostBootMutex.lock ();

    m_numberOfLineCardPostBootInProgress++;

    m_lineCardPostBootMutex.unlock ();
}

UI32 PrismFrameworkObjectManager::getNumberOfLineCardPostBootCurrentlyRunning ()
{
    UI32 numberOfLineCardPostbootInProgress = 0;

    m_lineCardPostBootMutex.lock ();

    numberOfLineCardPostbootInProgress = m_numberOfLineCardPostBootInProgress;

    m_lineCardPostBootMutex.unlock ();

    return (numberOfLineCardPostbootInProgress);
}

void PrismFrameworkObjectManager::decrementLineCardPostBootCount ()
{
    m_lineCardPostBootMutex.lock ();

    if (m_numberOfLineCardPostBootInProgress)
    {
        m_numberOfLineCardPostBootInProgress--;
    }
    else
    {
        //Decrementing when there are no LC postboot in progress hence assert
        prismAssert (false, __FILE__, __LINE__);
    }

     m_lineCardPostBootMutex.unlock ();
}

void PrismFrameworkObjectManager::setPostBootStarted (bool isPostBootStarted)
{
    m_postBootProgressMutex.lock();
    m_postBootStarted = isPostBootStarted;
    generateTimeStamp(m_postBootStartedTimeStamp);
    m_postBootProgressMutex.unlock();
}

bool PrismFrameworkObjectManager::getPostBootStarted ()
{
    m_postBootProgressMutex.lock();
    bool isPostBootStarted = m_postBootStarted;
    m_postBootProgressMutex.unlock();
    return isPostBootStarted;
}

string PrismFrameworkObjectManager::getPostBootStartedTimeStamp()
{  
    m_postBootProgressMutex.lock ();
    string postBootStartedTimeStamp = m_postBootStartedTimeStamp;
    m_postBootProgressMutex.unlock ();
    return postBootStartedTimeStamp;
}

void PrismFrameworkObjectManager::setPostBootCompleted (bool isPostBootCompleted)
{
    m_postBootProgressMutex.lock();
    m_postBootCompleted = isPostBootCompleted;
    generateTimeStamp(m_postBootCompletedTimeStamp);
    m_postBootProgressMutex.unlock();
}

bool PrismFrameworkObjectManager::getPostBootCompleted() 
{
    m_postBootProgressMutex.lock();
    bool isPostBootCompleted = m_postBootCompleted;
    m_postBootProgressMutex.unlock();
    return isPostBootCompleted;
}

string PrismFrameworkObjectManager::getPostBootCompletedTimeStamp()
{  
    m_postBootProgressMutex.lock ();
    string postBootCompletedTimeStamp = m_postBootCompletedTimeStamp;
    m_postBootProgressMutex.unlock ();

    return postBootCompletedTimeStamp;
}

void  PrismFrameworkObjectManager::setFileReplayStarted (bool isFileReplayStarted)
{
    m_fileReplayInProgressMutex.lock ();
    
    m_fileReplayStarted = isFileReplayStarted;
    if (true == m_fileReplayStarted)
    {
        generateTimeStamp(m_fileReplayStartedTimeStamp);
    }
    else
    {
        m_fileReplayStartedTimeStamp = "";
    }

    m_fileReplayInProgressMutex.unlock ();
}

bool  PrismFrameworkObjectManager::getFileReplayStarted ()
{
 m_fileReplayInProgressMutex.lock ();
 bool isFileReplayStarted = m_fileReplayStarted;
 m_fileReplayInProgressMutex.unlock ();

 return isFileReplayStarted;
}
string PrismFrameworkObjectManager::getFileReplayStartedTimeStamp()
{  
    m_fileReplayInProgressMutex.lock ();
    string fileReplayStartedTimeStamp = m_fileReplayStartedTimeStamp;
    m_fileReplayInProgressMutex.unlock ();
    return fileReplayStartedTimeStamp;
}


void  PrismFrameworkObjectManager::setFileReplayCompleted (bool isFileReplayCompleted)
{
   m_fileReplayInProgressMutex.lock ();
   m_fileReplayCompleted = isFileReplayCompleted;
   if (true == m_fileReplayCompleted)
   {
       generateTimeStamp(m_fileReplayCompletedTimeStamp);
   }
   else
   {
       m_fileReplayCompletedTimeStamp = "";
   }

   m_fileReplayInProgressMutex.unlock ();
}

bool  PrismFrameworkObjectManager::getFileReplayCompleted ()
{
   m_fileReplayInProgressMutex.lock ();
   bool isFileReplayCompleted = m_fileReplayCompleted;
   m_fileReplayInProgressMutex.unlock ();
   return isFileReplayCompleted;
}

string PrismFrameworkObjectManager::getFileReplayCompletedTimeStamp()
{  
    m_fileReplayInProgressMutex.lock ();
    string fileReplayCompletedTimeStamp = m_fileReplayCompletedTimeStamp;
    m_fileReplayInProgressMutex.unlock ();
    return fileReplayCompletedTimeStamp;
}

bool PrismFrameworkObjectManager::getFileReplayInProgress()
{

  m_fileReplayInProgressMutex.lock();
  bool isFileReplayInProgress =  false;

  if(false == m_fileReplayCompleted &&  true == m_fileReplayStarted) 
  {
      isFileReplayInProgress = true;
  }

  m_fileReplayInProgressMutex.unlock();

  return isFileReplayInProgress;
}

void PrismFrameworkObjectManager::startFileReplayEndAgent ()
{
   // Call the File Replay End Agent. This is a synchronous call. Should this call be made from within the mutex lock ? Its not necessary since all the "copy file to running" commands are serialized from the top level. 

   /* Call the virtual function FileReplayEnd */
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::setFileReplayCompleted : Start the File Replay End Agent.... ");

    FileReplayEndAgent *pPrismFileReplayEnd = new FileReplayEndAgent (m_pWaveObjectManager);

    pPrismFileReplayEnd->execute ();

    delete pPrismFileReplayEnd;

    // Print the FileReplay Started and Ended timestamp 

    tracePrintf (TRACE_LEVEL_INFO, false, false, "PrismFrameworkObjectManager::setFileReplayCompleted : File Replay Started Timestamp : %s \n\n", getFileReplayStartedTimeStamp().c_str ());
    tracePrintf (TRACE_LEVEL_INFO, false, false, "PrismFrameworkObjectManager::setFileReplayCompleted : File Replay Completed Timestamp : %s \n\n", (getFileReplayCompletedTimeStamp ()).c_str ());

    // After calling the Agent, reset the values of fileReplayStarted and fileReplayCompleted

    setFileReplayStarted (false);
    setFileReplayCompleted (false);
}

void PrismFrameworkObjectManager::fileReplayCompletedProcessing (bool fileReplayCompleted)
{
    if (true == fileReplayCompleted)
    {
        setFileReplayCompleted (fileReplayCompleted);
        startFileReplayEndAgent ();
    }
    else
    {
        setFileReplayCompleted (fileReplayCompleted);
    }
}


void  PrismFrameworkObjectManager::setConfigReplayStarted (bool isConfigreplayStarted)
{
  m_configReplayInProgressMutex.lock ();
  m_configReplayStarted = isConfigreplayStarted;
  generateTimeStamp(m_configReplayStartedTimeStamp);
  m_configReplayInProgressMutex.unlock ();

}

bool  PrismFrameworkObjectManager::getConfigReplayStarted ()
{
 m_configReplayInProgressMutex.lock ();
 bool isConfigreplayStarted = m_configReplayStarted;
 m_configReplayInProgressMutex.unlock ();

 return isConfigreplayStarted;
}
string PrismFrameworkObjectManager::getConfigReplayStartedTimeStamp()
{  
    m_configReplayInProgressMutex.lock ();
    string configReplayStartedTimeStamp = m_configReplayStartedTimeStamp;
    m_configReplayInProgressMutex.unlock ();
    return configReplayStartedTimeStamp;
}


void  PrismFrameworkObjectManager::setConfigReplayCompleted (bool isConfigReplayCompleted)
{
   m_configReplayInProgressMutex.lock ();
   m_configReplayCompleted = isConfigReplayCompleted;
   generateTimeStamp(m_configReplayCompletedTimeStamp);
   m_configReplayInProgressMutex.unlock ();
}
bool  PrismFrameworkObjectManager::getConfigReplayCompleted ()
{
   m_configReplayInProgressMutex.lock ();
   bool isConfigReplayCompleted = m_configReplayCompleted;
   m_configReplayInProgressMutex.unlock ();
   return isConfigReplayCompleted;
}

string PrismFrameworkObjectManager::getConfigReplayCompletedTimeStamp()
{  
    m_configReplayInProgressMutex.lock ();
    string configReplayCompletedTimeStamp = m_configReplayCompletedTimeStamp;
    m_configReplayInProgressMutex.unlock ();
    return configReplayCompletedTimeStamp;
}

bool PrismFrameworkObjectManager::getConfigReplayInProgress()
{
    m_configReplayInProgressMutex.lock();
    bool isConfigReplayInProgress =  false;

    if (false == m_configReplayCompleted &&  true == m_configReplayStarted) 
    {
        isConfigReplayInProgress = true;
    }

    m_configReplayInProgressMutex.unlock();

    return isConfigReplayInProgress;
}


void  PrismFrameworkObjectManager::setNodeReadyForAllCommands (bool isNodeReadyForAllCommands)
{
    m_nodeReadyForAllCommandsMutex.lock();
    m_nodeReadyForAllCommands = isNodeReadyForAllCommands;
    generateTimeStamp (m_nodeReadyForAllCommandsTimeStamp);

    m_nodeReadyForAllCommandsMutex.unlock();

}

bool  PrismFrameworkObjectManager::getNodeReadyForAllCommands ()
{
  m_nodeReadyForAllCommandsMutex.lock();
  bool isNodeReadyForAllCommands =  m_nodeReadyForAllCommands;
  m_nodeReadyForAllCommandsMutex.unlock();

  return isNodeReadyForAllCommands;

}

void PrismFrameworkObjectManager::setNodeZeroized (bool nodeZeroized)
{
    m_nodeZeroizedMutex.lock();
    m_nodeZeroized = nodeZeroized;
    m_nodeZeroizedMutex.unlock();

}

bool  PrismFrameworkObjectManager::isNodeZeroized ()
{
    m_nodeZeroizedMutex.lock();
    bool nodeZeroized=  m_nodeZeroized;
    m_nodeZeroizedMutex.unlock();
    return nodeZeroized;

}

string PrismFrameworkObjectManager::getNodeReadyForAllCommandsTimeStamp()
{

    m_nodeReadyForAllCommandsMutex.lock();
    string nodeReadyForAllCommandsTimeStamps = m_nodeReadyForAllCommandsTimeStamp;
    m_nodeReadyForAllCommandsMutex.unlock();

    return nodeReadyForAllCommandsTimeStamps;
}

void   PrismFrameworkObjectManager::generateTimeStamp (string & timeString)
{
  time_t local_time = time(NULL);
  timeString = (ctime(&local_time));
}

void PrismFrameworkObjectManager::deleteObjects (vector<WaveManagedObject *> *pResults)
{
    int i;
    if (NULL == pResults) return;
    int count = pResults->size ();

    for (i = 0; i < count; i++)
    {
        if (NULL != ((*pResults)[i])) delete ((*pResults)[i]);
    }
    delete pResults;
}
void PrismFrameworkObjectManager::cleanupManageObject (LocationId locationId)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext(WaveNode::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeLocationId (locationId, "locationId"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResults, __FILE__, __LINE__);
    vector<ObjectId> deleteObjectIds;
    vector<string> tableNames;
    ResourceId status = FRAMEWORK_SUCCESS;
    UI32 count = pResults->size ();

    deleteObjectIds.clear();
    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::cleanupManageObject : cleanup MO associated with LocationId = ") + locationId);
    prismAssert (1 >= count,__FILE__, __LINE__);

    if (count == 1) {
        string sql;
        WaveManagedObject *pWaveManagedObject = dynamic_cast<WaveManagedObject *> ((*pResults)[0]);
        ObjectId waveNodeObjectId = pWaveManagedObject->getObjectId();
        trace(TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::cleanupManageObject: Wave Node OID = ") + waveNodeObjectId.toString());

        /* Get all table in database */
        tableNames = (OrmRepository::getInstance ())->getTableNames(false);
        UI32 tableSizes = tableNames.size();
        UI32 i = 0;
        /* Go through all tables to look for Object ID that has ownerwavenodeobjectidclassid and ownerwavenodeobjectidinstanceid matched 
                * with the WaveNode Object Id */
        trace(TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::cleanupManageObject: Table size = ") + tableSizes);
        for (i = 0; i < tableSizes; i++) {
            WaveManagedObjectSynchronousQueryContext synchronousQueryContext1(tableNames[i]);
            synchronousQueryContext1.addAndAttribute (new AttributeUI32(waveNodeObjectId.getClassId(), "ownerWaveNodeObjectIdClassId"));
            synchronousQueryContext1.addAndAttribute (new AttributeUI64 (waveNodeObjectId.getInstanceId(), "ownerWaveNodeObjectIdInstanceId"));
            vector<WaveManagedObject *> *pMOResults = querySynchronously (&synchronousQueryContext1);
            prismAssert (NULL != pMOResults, __FILE__, __LINE__);
            UI32 j = 0;
            UI32 countMOResults = pMOResults->size();
            trace(TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::cleanupManageObject: Table = ") + tableNames[i] + " Number = " + countMOResults);
            for (j = 0; j < countMOResults; j ++) {
                WaveManagedObject *pManagedObject = dynamic_cast<WaveManagedObject *> ((*pMOResults)[j]);
                trace(TRACE_LEVEL_DEVEL, string("PrismFrameworkObjectManager::cleanupManageObject: OID = ") + (pManagedObject->getObjectId()).toString());
                deleteObjectIds.push_back(pManagedObject->getObjectId());
            }
            deleteObjects(pMOResults);
        }
        
        UI32 numberOfObjectsDeleted = deleteObjectIds.size();
        trace(TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::cleanupManageObject: Number of Delete Objects = ") + numberOfObjectsDeleted);

        for (i = 0; i < numberOfObjectsDeleted; i++)
        {
            (OrmRepository::getInstance ())->getSqlForDelete (deleteObjectIds[i], sql, OrmRepository::getWaveCurrentSchema ());
        }

        if (true == sql.empty ())
        {
            trace(TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::cleanupManageObject: No thing to delete "));
            deleteObjects(pResults);
            return;
        }

        PersistenceObjectManagerExecuteTransactionMessage message (sql);

        status = sendSynchronously (&message);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::cleanupManageObject : Could not send message to Persistence Service.  Status : " + FrameworkToolKit::localize (status));
        }
        else
        {
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::cleanupManageObject : Failed to execute the transaction.  Completion Status : " + FrameworkToolKit::localize (status));
            }
            else
            {
                trace(TRACE_LEVEL_SUCCESS, string("PrismFrameworkObjectManager::cleanupManageObject: Successfully cleanup ") + numberOfObjectsDeleted + string(" Management Objects "));
                status = FRAMEWORK_SUCCESS;
            }
        }
    }
    deleteObjects(pResults);
}

void PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep : Starting ...");

    vector<PrismServiceId> &prismServiceIdsToCommunicate = pCreateClusterWithNodesContext->getPrismServiceIdsToCommunicate ();
    UI32                    numberOfPrismServices        = prismServiceIdsToCommunicate.size ();
    UI32                    i                            = 0;

    UI32                    numberOfResultsLocations     = pCreateClusterWithNodesContext->getNumberOfResultsLocations ();
    UI32                    j                            = 0;
    string                  resultLocationIpAddress      = "";
    SI32                    resultLocationPort           = 0;

    void                   *pValidationResults           = NULL;
    UI32                    validationResultsSize        = 0;

    for (i = 0; i < numberOfPrismServices; i++)
    {
        WaveObjectManagerSendValidationResultsMessage *pMessage = new WaveObjectManagerSendValidationResultsMessage (prismServiceIdsToCommunicate[i]);

        for (j = 0; j < numberOfResultsLocations; j++)
        {
            pCreateClusterWithNodesContext->getResultsLocationAt (j, resultLocationIpAddress, resultLocationPort);
            pCreateClusterWithNodesContext->getValidationResultsForService (resultLocationIpAddress, resultLocationPort, prismServiceIdsToCommunicate[i], pValidationResults, validationResultsSize);

            if ((NULL != pValidationResults) && (0 != validationResultsSize))
            {
                trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep : Sending Validation Results to Servcie (") + FrameworkToolKit::getServiceNameById (prismServiceIdsToCommunicate[i]) + ") For One Location.");

                pMessage->addValidationResults (resultLocationIpAddress, resultLocationPort, pValidationResults, validationResultsSize);
            }
        }

        trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep : Sending Validation Results to Servcie (") + FrameworkToolKit::getServiceNameById (prismServiceIdsToCommunicate[i]) + ").");

        ResourceId status = sendOneWay (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep : Sending Validation Results to Servcie (") + FrameworkToolKit::getServiceNameById (prismServiceIdsToCommunicate[i]) + ") Failed. Status = " + FrameworkToolKit::localize (status));
        }
    }

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesStep (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesStep : Entering ...");

    vector<LocationId> currentlyKnownLocations;
    vector<LocationId> currentlyConnectedLocations;
    UI32               numberOfCurrentlyKnownLocations = 0;
    UI32               i                               = 0;
    UI32               j                               = 0;
    LocationId         locationId;
    string             ipAddress;
    UI32               port;
    ResourceId         status;
    UI32               numberOfFailures                = 0;

    FrameworkToolKit::getKnownLocations (currentlyKnownLocations);
    numberOfCurrentlyKnownLocations = currentlyKnownLocations.size ();
 
    FrameworkToolKit::getFullyConnectedLocations (currentlyConnectedLocations);

//    ++(*pCreateClusterWithNodesContext);

    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = 0;
    if(pCreateClusterWithNodesContext->getPPrismMessage()) 
    {
     pPrismCreateClusterWithNodesMessage =  dynamic_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    prismAssert(NULL != pPrismCreateClusterWithNodesMessage, __FILE__, __LINE__);
    }


    for (i = 0; i < numberOfCurrentlyKnownLocations; i++)
    {
        FrameworkObjectManagerUpdateListOfSecondariesMessage *pFrameworkObjectManagerUpdateListOfSecondariesMessage = new FrameworkObjectManagerUpdateListOfSecondariesMessage ();

        pFrameworkObjectManagerUpdateListOfSecondariesMessage->setConnectedLocationIds (currentlyConnectedLocations);

        for (j = 0; j < numberOfCurrentlyKnownLocations; j++)
        {
            locationId = currentlyKnownLocations[j];
            ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
            port       = FrameworkToolKit::getPortForLocationId (locationId);

            //determine whether info for a node  being sent in the message is 
            // for a newlyadded node
            bool isNewlyAddedNode = false; 
            if(pPrismCreateClusterWithNodesMessage) 
            {
                 UI32 numberOfNewIpAddresses = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();
            //Loop through each node in the message
                for(UI32 k = 0; k <numberOfNewIpAddresses; ++k) 
                {
                    string      tempIpAddress     = pPrismCreateClusterWithNodesMessage->getNodeAt ((UI32) k);
                    UI32        tempPort          = pPrismCreateClusterWithNodesMessage->getNodePortAt ((UI32) k);
                    LocationId  tempLocationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (tempIpAddress, tempPort);

                    if((locationId == tempLocationId)  && (WAVE_MESSAGE_SUCCESS == pPrismCreateClusterWithNodesMessage->getNewNodeStatus(tempIpAddress,tempPort))) 
                    {
                        isNewlyAddedNode = true;
                        break;
                    }
                }
            }

            pFrameworkObjectManagerUpdateListOfSecondariesMessage->addSecondaryNodeDetails (locationId, ipAddress, port,isNewlyAddedNode);
       }

       //For now we will suppress broadcasts in case of deletes. Set the field here so that the secondary can determine
       // whether to suppress the broadcasts
       pFrameworkObjectManagerUpdateListOfSecondariesMessage->setIsDeletion(pCreateClusterWithNodesContext->getIsDeletion());

       pFrameworkObjectManagerUpdateListOfSecondariesMessage->setLastUsedLocationId (FrameworkToolKit::getLastUsedLocationId ());
    
        status = sendOneWay (pFrameworkObjectManagerUpdateListOfSecondariesMessage, currentlyKnownLocations[i]);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            numberOfFailures++;

            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesStep : Error in informing " + ipAddress + string (":") + port);
        }
        else
        {
//            ++(*pCreateClusterWithNodesContext);
        }
    }

//    --(*pCreateClusterWithNodesContext);

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerUpdateListOfSecondariesMessage *pFrameworkObjectManagerUpdateListOfSecondariesMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesCallback : Entering ...");

    CreateClusterWithNodesContext *pCreateClusterWithNodesContext = reinterpret_cast<CreateClusterWithNodesContext *> (pContext);
    ResourceId                     status;

    --(*pCreateClusterWithNodesContext);

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesCallback : Error in informing a location. Framework Status : " + FrameworkToolKit::localize (frameworkStatus));
    }
    else
    {
        status = pFrameworkObjectManagerUpdateListOfSecondariesMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesCallback : Error in informing a location. Completion Status : " + FrameworkToolKit::localize (status));
        }
    }

    if (NULL != pFrameworkObjectManagerUpdateListOfSecondariesMessage)
    {
        delete pFrameworkObjectManagerUpdateListOfSecondariesMessage;
    }

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}
/// Name
/// broadcastNewNodesAddedEventOnPrimaryStep
/// Description
/// Broadcasts lists of all new nodes added as part of addnode 
/// or create cluster
/// Input
/// CreateClusterWithNodesContext *: Used by the f/w
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::broadcastNewNodesAddedEventOnPrimaryStep(CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{

    trace(TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::broadcastNewNodesAddedEventOnPrimary: Entering..");

    //Extract the message
    PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage = dynamic_cast<PrismCreateClusterWithNodesMessage *> (pCreateClusterWithNodesContext->getPPrismMessage ());
    prismAssert(NULL != pPrismCreateClusterWithNodesMessage,__FILE__, __LINE__);

    //Number of nodes in the message received from the cluster service
    UI32 numberOfNewIpAddresses = pPrismCreateClusterWithNodesMessage->getNumberOfNewNodes ();

    WaveNewNodesAddedEvent* pWaveNewNodesAddedEvent = new WaveNewNodesAddedEvent();
    for(UI32 i = 0; i<numberOfNewIpAddresses; ++i)
    {
       string ipAddress     = pPrismCreateClusterWithNodesMessage->getNodeAt ((UI32) i);
       UI32    port         = pPrismCreateClusterWithNodesMessage->getNodePortAt ((UI32) i);

       //Only nodes that were successfully added
       if(WAVE_MESSAGE_SUCCESS == pPrismCreateClusterWithNodesMessage->getNewNodeStatus(ipAddress, port))
        {
           LocationId locationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress, port);
           pWaveNewNodesAddedEvent->addNewNodeDetails(locationId,ipAddress,port);
        } else {
            // For the node that is added failed, we should remove MO

        }

    }

    //Broadcast the event
    ResourceId status = broadcast(pWaveNewNodesAddedEvent);

    if(status != WAVE_MESSAGE_SUCCESS)
    {
       trace(TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::broadcastNewNodesAddedEventOnPrimary: Failure during broadcast..");
       pCreateClusterWithNodesContext->executeNextStep (status);
       return;
    }

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    trace(TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::broadcastNewNodesAddedEventOnPrimary: Exiting..");
    return;

}


void PrismFrameworkObjectManager::configureSecondaryNodeMessageHandler (PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;
    
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeMessageHandler: (ACTIVE)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeValidateDefaultConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rollbackNodeIfRequiredStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::removePreviousDatabaseBackupFile),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prepareNodeForAddNodeIfRequired),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeValidateVersionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeShutdownServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeAddNewKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeConnectToNewKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeConfigureMyLocationIdStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pPrismConfigureClusterSecondaryMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->setClusterPrimaryIpAddress  (pPrismConfigureClusterSecondaryMessage->getClusterPrimaryIpAddress ());
    pSecondaryNodeClusterContext->setClusterPrimaryPort       (pPrismConfigureClusterSecondaryMessage->getClusterPrimaryPort ());
    pSecondaryNodeClusterContext->setClusterPrimaryLocationId (pPrismConfigureClusterSecondaryMessage->getClusterPrimaryLocationId ());

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::configureSecondaryNodeHaPeerMessageHandler (PrismConfigureClusterSecondaryHaPeerMessage *pPrismConfigureClusterSecondaryHaPeerMessage)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeHaPeerMessageHandler: (STANDBY)");

    // Timers are not required on standby since active triggers rollback on standby
    PrismLinearSequencerStep sequencerSteps[] =
    {
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::removePreviousDatabaseBackupFile),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeShutdownServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseForStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeSetLocationRoleOnStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pPrismConfigureClusterSecondaryHaPeerMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::configureSecondaryNodePhase1MessageHandler (PrismConfigureClusterSecondaryPhase1Message *pPrismConfigureClusterSecondaryPhase1Message)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodePhase1MessageHandler: (ACTIVE)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInPhase1),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeUpdateInstanceIdsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pPrismConfigureClusterSecondaryPhase1Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::configureSecondaryNodeHaPeerPhase1MessageHandler (PrismConfigureClusterSecondaryHaPeerPhase1Message *pPrismConfigureClusterSecondaryHaPeerPhase1Message)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeHaPeerPhase1MessageHandler: (STANDBY)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pPrismConfigureClusterSecondaryHaPeerPhase1Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();

}

void PrismFrameworkObjectManager::updateLocationRoleStepInPhase1 (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if (LOCATION_SECONDARY_CLUSTER_PHASE_0 == locationRole)
    {
    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_CLUSTER_PHASE_1);
    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::updateLocationRoleStepInPhase1 received Message in incorrect location role, return with error");
        pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void PrismFrameworkObjectManager::rollbackNodeIfRequiredStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    LocationRole                           thisLocationRole                       = m_pThisLocation->getLocationRole ();
    ResourceId                             status                                 = WAVE_MESSAGE_SUCCESS;
        
    if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 == thisLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_1 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_1 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_0 == thisLocationRole))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rollbackNodeIfRequiredStep : Rolling back as the node is in phase 0/1");

        status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackNodeIfRequiredStep : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
        
    }
    else if ((LOCATION_SECONDARY_CLUSTER_PHASE_2 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_2 == thisLocationRole))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rollbackNodeIfRequiredStep : Rolling back as the node is in phase 2");

        status = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_ROLLBACK);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackNodeIfRequiredStep : shutdown should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }

        status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackNodeIfRequiredStep : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
        
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::prepareNodeForAddNodeIfRequired (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    LocationRole                           currentRole                            = m_pThisLocation->getLocationRole ();
    ResourceId                             status                                 = WAVE_MESSAGE_SUCCESS;

    // this function should be called when this node is in Primary or Secondary Role and If some other node is inviting this node for cluster formation/Add Node

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::prepareNodeForAddNodeIfRequired Location Role" + FrameworkToolKit::localize (currentRole));

    if (LOCATION_STAND_ALONE == currentRole)
    {
        pSecondaryNodeClusterContext->executeNextStep (status);
    }
    
    if ((LOCATION_SECONDARY == currentRole) || (LOCATION_PRIMARY_UNCONFIRMED == currentRole) || (LOCATION_SECONDARY_UNCONFIRMED == currentRole))
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondarySendStopHeartBeat),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryDisconnectFromConnectedLocationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryUpdateThisLocationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryNodeBootServicesStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryUnconfigureThisLocationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
        };

        PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::prepareSecondaryNodeForAddNodeIfRequiredCallback), pSecondaryNodeClusterContext);

        DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext = new DeleteNodeOnSecondaryContext (pPrismAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        pDeleteNodeOnSecondaryContext->holdAll ();
        pDeleteNodeOnSecondaryContext->start ();
    }    

    if (LOCATION_PRIMARY == currentRole)
    {
        //Disconnect from all the nodes, before sending destroy cluster as in this case we do not want to unconfigure all the attached secondaries
        disconnectFromAllConnectedNodes ();

        //Destroy cluster 
        //Note: This will not send unconfigure to all secondary nodes
        DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext = new DestroyClusterAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::preparePrimaryNodeForAddNodeIfRequiredCallBack), pSecondaryNodeClusterContext);

        pDestroyClusterAsynchronousContext->setPreparingForAddNode        (true);
        pDestroyClusterAsynchronousContext->setReasonForDestroyingCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);
        pDestroyClusterAsynchronousContext->setOriginalRequester          (PrismFrameworkObjectManager::getPrismServiceId ());
        pDestroyClusterAsynchronousContext->setIsRebootRequired           (false);

        destroyClusterAsynchronousHandler (pDestroyClusterAsynchronousContext);
    }
}

void PrismFrameworkObjectManager::prepareSecondaryNodeForAddNodeIfRequiredCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    prismAssert (NULL != pPrismAsynchronousContext, __FILE__, __LINE__);

    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                   status                       = pPrismAsynchronousContext->getCompletionStatus ();

    prismAssert (NULL != pSecondaryNodeClusterContext, __FILE__, __LINE__);

    delete pPrismAsynchronousContext;

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::preparePrimaryNodeForAddNodeIfRequiredCallBack (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::prepareNodeForAddNodeIfRequiredCallBack : Entering ...");

    prismAssert (NULL !=  pDestroyClusterAsynchronousContext, __FILE__, __LINE__);

    ResourceId                   status                       = pDestroyClusterAsynchronousContext->getCompletionStatus ();
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pDestroyClusterAsynchronousContext->getPCallerContext ());

    delete pDestroyClusterAsynchronousContext;

    if (FrameworkToolKit::getSecondaryClusterFormationFlag ())
    {
        trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::prepareNodeForAddNodeIfRequiredCallback: Flag is TRUE.");
    }
    else
    {
        trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::prepareNodeForAddNodeIfRequiredCallback: Flag is FALSE.");
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeValidateDefaultConfigurationStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (true == FrameworkToolKit::isConfigurationCompatibilityCheckRequired ())
    {
        UI64      configurationTransactionId  = 0;  
        DateTime  configurationTime;
        DateTime  latestUpdateTime;
      
        PersistenceToolKit::getConfigurationManagedObjectInformation (configurationTransactionId, configurationTime, latestUpdateTime); 

        if (0 != configurationTransactionId)
        {
            // Add node request is received by a node which is not in default configuration, hence erroring out the request
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateDefaultConfigurationStep check failed");

            status = WAVE_MESSAGE_ERROR_CONFIGURATION_MISMATCH;
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeValidateStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    LocationRole                           currentRole                            = m_pThisLocation->getLocationRole ();
    ResourceId                             status                                 = WAVE_MESSAGE_SUCCESS;

    if (LOCATION_PRIMARY == currentRole)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Some one else is trying to hijack our Location Id.  Will not reply to this message.");
        status = FRAMEWORK_ERROR_LOCATION_ALREADY_CONFIGURED;
    }
    else if (LOCATION_SECONDARY == currentRole)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Some one else is trying to hijack the Primary Location Id we already know.  Will not reply to this message.");
        status = FRAMEWORK_ERROR_LOCATION_ALREADY_CONFIGURED;
    }
    else if (currentRole == LOCATION_SUB_LOCATION)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Sub Locations cannot play Primary / Secondary Role.  Will not reply");
        status = FRAMEWORK_ERROR_SUB_LOCATION_CANNOT_BECOME_MEMBER_OF_A_CLUSTER;
    }

    // Check if the requesting location is still connected to us.  Otherwise simply drop the request request.
    //  This situation can arise if a requesting location dies or aborts the request before we get a chance to
    // process the message.

    if (false == (InterLocationMessageReceiverObjectManager::isACurrentlyConnectedLocation (pSecondaryNodeClusterContext->getClusterPrimaryIpAddress (), pSecondaryNodeClusterContext->getClusterPrimaryPort ())))
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Requesting location is not interested us any more.  Dropping the request.");
        status = FRAMEWORK_ERROR_CANNOT_BECOME_SECONDARY_REQUESTING_LOCATION_UNAVAILABLE;
    }

    string ipAddressForThisLocation = pPrismConfigureClusterSecondaryMessage->getIpAddress ();

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Currently assumed IP Address for this location : " + m_pThisLocation->getIpAddress ());
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Requested         IP Address for this Location : " + ipAddressForThisLocation);

    if (false == (FrameworkToolKit::isAnIpAddressForThisMachine (ipAddressForThisLocation)))
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Invalid IP Address is specified for this location : " + ipAddressForThisLocation);

        string ethernetInterface = getEthernetInterfaceForThisLocation ();
        string interfaceState = getStateForInterface(ethernetInterface);
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : StateForInterface " + ethernetInterface + " : " + interfaceState);
        status = FRAMEWORK_ERROR_CANNOT_BECOME_SECONDARY_NOT_A_VALID_IPADDRESS_FOR_THIS_LOCATION;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeValidateStep : Resetting         IP Address for this Location");

        // Right away set our new ipaddress to the Location.
        m_pThisLocation->resetIpAddress (ipAddressForThisLocation);
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

// revertTableIdsIfRequired () : This function helps in reverting back the table-ids for the tables in case changed.
// Currently, not being used any place. Will be called in case required.

#if 0
void PrismFrameworkObjectManager::revertTableIdsIfRequired ()
{
    if (false == getTableIdsChangedForSchemaCompatibilityFlag ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::revertTableIdsIfRequired: No changes in tableIds for Schema compatibility.");
        return;
    }
    
    // 1. restore tableIds.
    OrmRepository * pOrmRepository = OrmRepository::getInstance();
    pOrmRepository->restoreOldTableIds ();
    
    pOrmRepository->buildMoSchemaRepository ();

    // 2. Handle AuxilliaryTables and their constraints.
    vector<string> oldAuxilliaryTableNames, newAuxilliaryTableNames, parentTableNames, relatedToTableNames;

    getAuxilliaryTableDetailsForSchemaConversion (oldAuxilliaryTableNames, newAuxilliaryTableNames, parentTableNames, relatedToTableNames);

    string sqlToAlterTables = generateSqlToAlterAuxilliaryTableNames (newAuxilliaryTableNames, oldAuxilliaryTableNames, parentTableNames, relatedToTableNames);

    DatabaseStandaloneTransaction dbStandaloneTransaction;
    if (!dbStandaloneTransaction.execute (sqlToAlterTables))
    {   
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager : Error in applying SQL to alter auxillary table names");
        pOrmRepository->restoreOldTableIds (); 
    }
    else
    {
        setTableIdsChangedForSchemaCompatibilityFlag (false);
    }

    return;
}
#endif
/*
string PrismFrameworkObjectManager::handleConstraintsForAuxilliaryTable (string waveSchema, string& oldAuxilliaryTableName, string& newAuxilliaryTableName, string& parentTableName, string &relatedToTableName)
{
    string sqlPrefix        = "ALTER TABLE ONLY " + waveSchema +"."+newAuxilliaryTableName;
    // 1. get sql to drop all existing constraints
    string sqlForConstraint = sqlPrefix + " DROP CONSTRAINT " + oldAuxilliaryTableName + "_ownerclassid_fkey; \n"
                            + sqlPrefix + " DROP CONSTRAINT " + oldAuxilliaryTableName + "_ownerinstanceid_fkey;\n"
                            + sqlPrefix + " DROP CONSTRAINT " + oldAuxilliaryTableName + "_relatedtoclassid_fkey;\n" 
                            + sqlPrefix + " DROP CONSTRAINT " + oldAuxilliaryTableName + "_relatedtoinstanceid_fkey;\n"; 
    
    // Add sql to add constraints.
           sqlForConstraint += sqlPrefix +" ADD CONSTRAINT "+newAuxilliaryTableName + "_ownerclassid_fkey FOREIGN KEY (ownerclassid) REFERENCES " 
                            + waveSchema + "." + parentTableName + "derivations(objectidclassid) DEFERRABLE INITIALLY DEFERRED;\n"
                            + sqlPrefix +" ADD CONSTRAINT "+newAuxilliaryTableName +"_ownerinstanceid_fkey FOREIGN KEY (ownerinstanceid) REFERENCES " 
                            + waveSchema + "." + parentTableName + "derivationsinstances(objectidinstanceid) DEFERRABLE INITIALLY DEFERRED;\n"
                            + sqlPrefix +" ADD CONSTRAINT "+newAuxilliaryTableName +"_relatedtoclassid_fkey FOREIGN KEY (relatedtoclassid) REFERENCES " 
                            + waveSchema + "." + relatedToTableName + "derivations(objectidclassid) DEFERRABLE INITIALLY DEFERRED;\n"
                            + sqlPrefix +" ADD CONSTRAINT "+newAuxilliaryTableName +"_relatedtoinstanceid_fkey FOREIGN KEY (relatedtoinstanceid) REFERENCES " 
                            + waveSchema + "." + relatedToTableName + "derivationsinstances(objectidinstanceid) DEFERRABLE INITIALLY DEFERRED;\n";
    
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::handleConstraintsForAuxilliaryTable: sql for constraints = " + sqlForConstraint);
    return (sqlForConstraint);
}

string PrismFrameworkObjectManager::generateSqlToAlterAuxilliaryTableNames (vector<string>& oldAuxilliaryTableNames,vector<string>& newAuxilliaryTableNames, vector<string>& parentTableNames, vector<string>& relatedToTableNames)
{
    string sqlToChangeAuxilliaryTableNames;
    UI32   numberOfAuxilliaryTables         = oldAuxilliaryTableNames.size ();

    for(UI32 i = 0; i<numberOfAuxilliaryTables; ++i) 
    {
        if (0 != oldAuxilliaryTableNames[i].compare (newAuxilliaryTableNames[i]))
        {
            tracePrintf (TRACE_LEVEL_INFO, true, false, "PrismFrameworkObjectManager::generateSqlToAlterAuxilliaryTableNames : Updating sql to alter name of table [%s] to [%s]", oldAuxilliaryTableNames[i].c_str(), newAuxilliaryTableNames[i].c_str());
            sqlToChangeAuxilliaryTableNames+= "ALTER TABLE " +OrmRepository::getWaveCurrentSchema() + "."+ oldAuxilliaryTableNames[i] +" RENAME TO " + newAuxilliaryTableNames[i] +";";
            sqlToChangeAuxilliaryTableNames+= "ALTER TABLE " +OrmRepository::getWaveStartSchema () + "."+ oldAuxilliaryTableNames[i] +" RENAME TO " + newAuxilliaryTableNames[i] +";";
            sqlToChangeAuxilliaryTableNames+= handleConstraintsForAuxilliaryTable (OrmRepository::getWaveCurrentSchema(), oldAuxilliaryTableNames[i], newAuxilliaryTableNames[i], parentTableNames[i], relatedToTableNames[i]);
            sqlToChangeAuxilliaryTableNames+= handleConstraintsForAuxilliaryTable (OrmRepository::getWaveStartSchema(), oldAuxilliaryTableNames[i], newAuxilliaryTableNames[i], parentTableNames[i], relatedToTableNames[i]);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::generateSqlToAlterAuxilliaryTableNames : no change for table " + oldAuxilliaryTableNames[i]);
        }
    }

    return sqlToChangeAuxilliaryTableNames;
}


ResourceId PrismFrameworkObjectManager::updateTableIdsInOrmWithSchemaInfoReceivedFromPrimary (const vector<string>& managedObjectNamesForSchemaChange, const vector<UI32>& classIds, const vector<UI32>& parentTableIds)
{
    ResourceId      status          = FRAMEWORK_SUCCESS;
    OrmRepository*  pOrmRepository  = OrmRepository::getInstance ();

    vector<string>  oldAuxilliaryTableNames, newAuxilliaryTableNames, parentTablesNames, relatedToTableNames;

    pOrmRepository->updateTableIdsWithTableIdsReceivedFromPrimary (managedObjectNamesForSchemaChange, classIds, parentTableIds,oldAuxilliaryTableNames,newAuxilliaryTableNames, parentTablesNames, relatedToTableNames);
    string sqlToAlterTables = generateSqlToAlterAuxilliaryTableNames (oldAuxilliaryTableNames,newAuxilliaryTableNames, parentTablesNames, relatedToTableNames);

    DatabaseStandaloneTransaction dbStandaloneTransaction;
    if (!dbStandaloneTransaction.execute (sqlToAlterTables))
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager : Error in applying SQL to alter auxillary table names");
        //pOrmRepository->restoreOldTableIds ();
        status = FRAMEWORK_ERROR;
    }
    else
    {
        setTableIdsChangedForSchemaCompatibilityFlag (true);
        setAuxilliaryTableDetailsForSchemaConversion (oldAuxilliaryTableNames, newAuxilliaryTableNames, parentTablesNames, relatedToTableNames);
    }

    return status;
}
*/
bool PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible(const vector<string> & managedObjectNamesForSchemaChange,const vector<string> & fieldNamesStringsForSchemaChange,const vector<string> & fieldNamesTypesForSchemaChange,const vector<UI32> & classIds,const vector<UI32>  & parentTableIds)
{
    //setTableIdsChangedForSchemaCompatibilityFlag (false);    

	bool                                isCompatible            = true;
    bool                                hasTableIdsChanged      = false;
	OrmRepository*                      pOrmRepository          = OrmRepository::getInstance();
	ManagedObjectSchemaInfoRepository&  moSchemaInfoRepository  = pOrmRepository->getMOSchemaInfoRepository();

    UI32 numberOfManagedObjectsInSchema  = moSchemaInfoRepository.getNumberOfInfoObjectsInRepository ();
	UI32 numberOfElementsInPrimarySchema = managedObjectNamesForSchemaChange.size ();
	

        tracePrintf (TRACE_LEVEL_INFO,true, false, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: number of MO in Primary = %u and this node numberOfManagedObjectsInSchema = %u.", numberOfElementsInPrimarySchema, numberOfManagedObjectsInSchema);

    if (numberOfManagedObjectsInSchema != numberOfElementsInPrimarySchema)
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: number of MO in Primary and this node differs.");
        return (false);
    }

	for(UI32 i = 0; i<numberOfElementsInPrimarySchema; ++i) 
	{
		string                   managedObjectName        = managedObjectNamesForSchemaChange[i];
		ManagedObjectSchemaInfo* pManagedObjectSchemaInfo = moSchemaInfoRepository.findSchemaInfoObject(managedObjectName);

        trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: " + managedObjectName);

		if(!pManagedObjectSchemaInfo) 
		{		 
			tracePrintf (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: [%s] doesn't exist in schema.", managedObjectName.c_str()); 
	        isCompatible = false;
			break;
		}

        // check if tableId has changed.
        if (!hasTableIdsChanged && (classIds [i] != pManagedObjectSchemaInfo->getTableId ()))
        {
            hasTableIdsChanged = true;
            tracePrintf (TRACE_LEVEL_FATAL, true, false, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: ClassId mismatch found. classId on local node = %u, table-id on Primary (pManagedObjectSchemaInfo->getTableId) = %u ", classIds [i], pManagedObjectSchemaInfo->getTableId ());
            prismAssert (false, __FILE__, __LINE__);
        }

        // check if the fields in MO matches.
		string fieldNames = fieldNamesStringsForSchemaChange[i];
		string fieldTypes = fieldNamesTypesForSchemaChange [i];

        trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: fieldTypes" + fieldTypes);
		vector<string> fieldNamesInPrimarySchema;
		vector<string> fieldTypesInPrimarySchema;

		tokenize(fieldNamesStringsForSchemaChange[i],fieldNamesInPrimarySchema,':');
		tokenize(fieldNamesTypesForSchemaChange [i],fieldTypesInPrimarySchema, ':');

		map<string,string>	simpleFields	= pManagedObjectSchemaInfo->getSimpleFields();
		map<string, string> relationFields  = pManagedObjectSchemaInfo->getRelationFields();

        if ((fieldNamesInPrimarySchema.size ()) != (simpleFields.size () + relationFields.size ()))
        {
            tracePrintf (TRACE_LEVEL_WARN, true, false, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: no. of fields in [%s] doesn't match with Primary's object.", managedObjectName.c_str());
	        isCompatible = false;
            break;
        }

		for (UI32 j = 0; j<fieldNamesInPrimarySchema.size(); ++j) 
		{
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: fieldTypesInPrimarySchema : " + fieldTypesInPrimarySchema [j]);
			 map<string,string>::iterator simpleFieldsIterator = simpleFields.find (fieldNamesInPrimarySchema[j]);

			 if(simpleFieldsIterator != simpleFields.end()) 
			 {
                 trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: simpleField :" + simpleFieldsIterator->second); 
        		 if(simpleFieldsIterator->second != fieldTypesInPrimarySchema [j])
	        	 {
		            isCompatible = false;
				    break;
				 }
			 }
			 else
			 {
				 map<string,string>::iterator relationFieldsIterator = relationFields.find (fieldNamesInPrimarySchema[j]);

				 if(relationFieldsIterator != relationFields.end()) 
				 {
                 trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: relationField :" + relationFieldsIterator->second); 
        			 if(relationFieldsIterator->second != fieldTypesInPrimarySchema[j])
	        		 {
		    			 isCompatible = false;
		    			 break;
		  			 }
				 }
				 else 
				 {
		        	 isCompatible = false;
			         break;
				 }
			 }
		}

		if(!isCompatible) 
		{
            tracePrintf (TRACE_LEVEL_WARN, true, false, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: atleast a field doesn't found in [%s]", managedObjectName.c_str());
			break;
		}
	}

/*
    // update tableIds if required.
    if (isCompatible && hasTableIdsChanged)
    {   
        if (FRAMEWORK_SUCCESS != updateTableIdsInOrmWithSchemaInfoReceivedFromPrimary (managedObjectNamesForSchemaChange, classIds, parentTableIds))
        {   
            trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: failed to update tableIds received from Primary.");
            isCompatible = false;
        }
        else
        {
            pOrmRepository->buildMoSchemaRepository ();

            //save UpgradeMO in the database as Orm has changed.
            m_pInitializeWorker->populateSchemaUpgradeTableInDb ();
        }
    }
*/
    tracePrintf (TRACE_LEVEL_INFO, true, false, "PrismFrameworkObjectManager::isSchemaReceivedFromPrimaryCompatible: returning %u", isCompatible); 
    return (isCompatible);
}

void PrismFrameworkObjectManager::configureSecondaryNodeValidateVersionStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    string                                 primaryPrismVersionString              = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryPrismVersion ();
    string                                 thisLocationPrismPrimaryVersionString  = PrismVersion::getVersionString ();
    ResourceId                             status                                 = WAVE_MESSAGE_SUCCESS;

    if (getNumberOfLineCardPostBootCurrentlyRunning ())
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateVersionStep :LC Postboot in progress. Returning error.");
        pSecondaryNodeClusterContext->executeNextStep (FRAMEWORK_LINECARD_POSTBOOT_IN_PROGRESS);
        return;
    }

    trace (TRACE_LEVEL_INFO, "This       Location Prism Version : " + thisLocationPrismPrimaryVersionString);
    trace (TRACE_LEVEL_INFO, "Requesting Location Prism Version : " + primaryPrismVersionString);

    if (0 == thisLocationPrismPrimaryVersionString.compare (primaryPrismVersionString))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeValidateVersionStep : Version Check succeeded.\n");

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateVersionStep : Version Check failed.\n");

        status = FRAMEWORK_ERROR_CANNOT_BECOME_SECONDARY_WAVE_VERSION_MISMATCH;
    }


    vector<string>  managedObjectNamesForSchemaChange;
    vector<string>  fieldNamesStringsForSchemaChange;
    vector<string>  fieldNamesTypesForSchemaChange;
    vector<UI32>    classIds;
    vector<UI32>    parentTableIds;

    pPrismConfigureClusterSecondaryMessage->getSchemaChangeVectors (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange,classIds,parentTableIds); 

    if (!isSchemaReceivedFromPrimaryCompatible (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange,classIds,parentTableIds))
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeValidateVersionStep : Detected schema incompatible between the Primary and this node. These nods must be running the different firmwares.");
        status = WAVE_MESSAGE_ERROR_SCHEMA_MISMATCH;
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    vector<PrismServiceId> prismServiceIds;
    UI32                   numberOfPrismServiceIds;
    UI32                   i;
    LocationId             thisLocationId           = FrameworkToolKit::getThisLocationId ();
    ResourceId             status                   = WAVE_MESSAGE_SUCCESS;

    PrismThread::getListOfServiceIds (prismServiceIds);
    numberOfPrismServiceIds = prismServiceIds.size ();

    PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    prismAssert (NULL != pPrismConfigureClusterSecondaryMessage, __FILE__, __LINE__);

    for (i = 0; i < numberOfPrismServiceIds; i++)
    {
        trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep : Validating Service : ") + prismServiceIds[i]);

        if (true == (isServiceToBeExcludedInClusterCommunications (prismServiceIds[i])))
        {
            continue;
        }

        //PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
        void                                  *pValidationData                        = NULL;
        UI32                                   size                                   = 0;

        pValidationData = pPrismConfigureClusterSecondaryMessage->transferBufferToUser (prismServiceIds[i], size);

        WaveObjectManagerValidateClusterCreationMessage message (prismServiceIds[i]);

        // If we have got some validation data then attach it and send it to the service.

        if ((NULL != pValidationData) && (0 != size))
        {
            trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep : Sending Validation details to Service : ") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            message.setValidationDetails (pValidationData, size, true);
        }

        status = sendSynchronously (&message, thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_SERVICE_VALIDATION_FAILED;
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep : Failed to Validate Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Status : " + FrameworkToolKit::localize (status));
            break;
        }

        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_SERVICE_VALIDATION_FAILED;
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep : Failed to Validate Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Completion Status : " + FrameworkToolKit::localize (status));
            break;
        }

        void *pValidationResults = NULL;

        message.getValidationResults (pValidationResults, size);

        if ((0 != size) && (NULL != pValidationResults))
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep : Obtained Validation Results for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            pPrismConfigureClusterSecondaryMessage->addBuffer (prismServiceIds[i] + s_offSetForValidationResults, size, pValidationResults, true);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeValidateServicesStep : No Validation Results are obtained for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep(SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep Entering"); 

    if (true == FrameworkToolKit::isConfigurationCompatibilityCheckRequired ())
    {
        // For LC mode Notify HA that secondary joining cluster to start, so HA sync to be stopped.
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep: Notify on ClusterUnReady event for secondary firsttime join event ");

        FrameworkToolKit::disableLiveSync ();
        setSyncState (OUT_OF_SYNC);
        setNeedNotifyClusterReadyState (true);
        bool readyState = false;
        notifyClusterReadyState (readyState);
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);
    }
    else if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep: Ha Peer is connected and Live Sync enabled.");

        PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
        prismAssert (NULL != pPrismConfigureClusterSecondaryMessage, __FILE__, __LINE__);

        PrismConfigureClusterSecondaryHaPeerMessage *pPrismConfigureClusterSecondaryHaPeerMessage = new PrismConfigureClusterSecondaryHaPeerMessage ();
        prismAssert (NULL != pPrismConfigureClusterSecondaryHaPeerMessage, __FILE__, __LINE__);

        pPrismConfigureClusterSecondaryHaPeerMessage->copyBuffersFrom (*pPrismConfigureClusterSecondaryMessage);

        ResourceId haPeerSendStatus = send (pPrismConfigureClusterSecondaryHaPeerMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::configureClusterSecondaryHaPeerMessageCallback), pSecondaryNodeClusterContext, 200000, FrameworkToolKit::getHaPeerLocationId ());
            
        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep: Send to Ha Peer failed.");
            delete pPrismConfigureClusterSecondaryHaPeerMessage;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep: Cluster Phase 0 to Ha Peer start.");
                pSecondaryNodeClusterContext->setClusterHaSyncInProgress (true);
            }
        }
    }
    else if ((FrameworkToolKit::getSyncState () == IN_SYNC) && (false == FrameworkToolKit::getIsLiveSyncEnabled ()))
    {
        // DB previously in sync from sync dump. but live transaction sync not supported. So set out of sync and notify on sync update fail
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep: Live sync disabled. Notify sync update failure on first update after sync dump."); 
        FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_NOT_SUPPORTED);
    }
    else
    {         
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeNotifyHaPeerStep: Ha Peer is unavailable OR live sync disabled.");
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureClusterSecondaryHaPeerMessageCallback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryHaPeerMessage *pPrismConfigureClusterSecondaryHaPeerMessage, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureClusterSecondaryHaPeerMessageCallback: Begin.");

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureClusterSecondaryHaPeerMessageCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pPrismConfigureClusterSecondaryHaPeerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureClusterSecondaryHaPeerMessageCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pPrismConfigureClusterSecondaryHaPeerMessage)
    {
        delete pPrismConfigureClusterSecondaryHaPeerMessage;
    }

    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pContext);

    if (true == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureClusterSecondaryHaPeerMessageCallback: Cluster Phase 0 to Ha Peer end.");

        if ( (pSecondaryNodeClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterContext);
            pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::checkHaPeerSyncStatusStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::checkHaPeerSyncStatusStep");

    if (true == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::checkHaPeerSyncStatusStep: Cluster sync to HaPeer in-progress");
        ++(*pSecondaryNodeClusterContext);
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::checkHaPeerSyncPostPhaseStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::checkHaPeerSyncPostPhaseStep");

    if (true == pSecondaryNodeClusterPhase3Context->getClusterHaSyncInProgress ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::checkHaPeerSyncPostPhaseStep: Cluster sync to HaPeer in-progress");
        ++(*pSecondaryNodeClusterPhase3Context);
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    PrismConfigureClusterSecondaryPhase1Message *pPrismConfigureClusterSecondaryPhase1Message = reinterpret_cast<PrismConfigureClusterSecondaryPhase1Message *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    prismAssert (NULL != pPrismConfigureClusterSecondaryPhase1Message, __FILE__, __LINE__);

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        PrismConfigureClusterSecondaryHaPeerPhase1Message *pPrismConfigureClusterSecondaryHaPeerPhase1Message = new PrismConfigureClusterSecondaryHaPeerPhase1Message;
        prismAssert (NULL != pPrismConfigureClusterSecondaryHaPeerPhase1Message, __FILE__, __LINE__);

        pPrismConfigureClusterSecondaryHaPeerPhase1Message->copyBuffersFrom (*pPrismConfigureClusterSecondaryPhase1Message);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseStep: Ha Peer is connected and Live Sync enabled.");

        ResourceId haPeerSendStatus = send (pPrismConfigureClusterSecondaryHaPeerPhase1Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseCallback), pSecondaryNodeClusterContext, 200000, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseStep: Send to Ha Peer failed.");
            delete pPrismConfigureClusterSecondaryHaPeerPhase1Message;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseStep: Cluster Pre-phase to Ha Peer start.");
                pSecondaryNodeClusterContext->setClusterHaSyncInProgress (true);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseStep: Ha Peer is unavailable OR Live sync is disabled.");
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseCallback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryHaPeerPhase1Message *pPrismConfigureClusterSecondaryHaPeerPhase1Message, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseCallback: Begin.");

    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pPrismConfigureClusterSecondaryHaPeerPhase1Message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pPrismConfigureClusterSecondaryHaPeerPhase1Message)
    {
        delete pPrismConfigureClusterSecondaryHaPeerPhase1Message;
    }

    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pContext);

    if (true == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPrePhaseCallback: Cluster Pre-phase to Ha Peer end.");

        if ( (pSecondaryNodeClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterContext);
            pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    PrismConfigureClusterSecondaryPhase3Message *pPrismConfigureClusterSecondaryPhase3Message = reinterpret_cast<PrismConfigureClusterSecondaryPhase3Message *> (pSecondaryNodeClusterPhase3Context->getPPrismMessage ());
    prismAssert (NULL != pPrismConfigureClusterSecondaryPhase3Message, __FILE__, __LINE__);

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        PrismConfigureClusterSecondaryHaPeerPhase3Message *pPrismConfigureClusterSecondaryHaPeerPhase3Message = new PrismConfigureClusterSecondaryHaPeerPhase3Message;
        prismAssert (NULL != pPrismConfigureClusterSecondaryHaPeerPhase3Message, __FILE__, __LINE__);

        pPrismConfigureClusterSecondaryHaPeerPhase3Message->copyBuffersFrom (*pPrismConfigureClusterSecondaryPhase3Message);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseStep: Ha Peer is connected and Live Sync enabled.");

        ResourceId haPeerSendStatus = send (pPrismConfigureClusterSecondaryHaPeerPhase3Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseCallback), pSecondaryNodeClusterPhase3Context, s_clusterPhase3TimeoutInMilliseconds, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseStep: Send to Ha Peer failed.");
            delete pPrismConfigureClusterSecondaryHaPeerPhase3Message;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterPhase3Context->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseStep: Cluster Post-phase to Ha Peer start.");
                pSecondaryNodeClusterPhase3Context->setClusterHaSyncInProgress (true);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseStep: Ha Peer is unavailable OR Live sync is disabled.");
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseCallback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryHaPeerPhase3Message *pPrismConfigureClusterSecondaryHaPeerPhase3Message, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseCallback: Begin.");
 
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pPrismConfigureClusterSecondaryHaPeerPhase3Message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pPrismConfigureClusterSecondaryHaPeerPhase3Message)
    {
        delete pPrismConfigureClusterSecondaryHaPeerPhase3Message;
    }

    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = reinterpret_cast<SecondaryNodeClusterPhase3Context *> (pContext); 

    if (true == pSecondaryNodeClusterPhase3Context->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterPhase3Context->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseCallback: Cluster Post-phase to Ha Peer end.");

        if ( (pSecondaryNodeClusterPhase3Context->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterPhase3Context);
            pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::configureSecondaryNodeAddNewKnownLocationsStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    string     clusterPrimaryIpAddress  = pSecondaryNodeClusterContext->getClusterPrimaryIpAddress ();
    SI32       clusterPrimaryPort       = pSecondaryNodeClusterContext->getClusterPrimaryPort ();
    LocationId clusterPrimaryLocationId = pSecondaryNodeClusterContext->getClusterPrimaryLocationId ();

    m_pThisLocation->addKnownLocation (clusterPrimaryLocationId, clusterPrimaryIpAddress, clusterPrimaryPort);

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::configureSecondaryNodeAddNewKnownLocationsStep : Added Primary Location to known locations : ") + clusterPrimaryLocationId);
    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::configureSecondaryNodeAddNewKnownLocationsStep : Primary IP Address : ") + clusterPrimaryIpAddress);

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureSecondaryNodeConnectToNewKnownLocationsStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
                 PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage           = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
                 LocationId                             clusterPrimaryLocationId                         = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryLocationId ();
    static const UI32                                   maximumNumberOfRetriesToConnectToARemoteLocation = 30;
    static const UI32                                   maximumNumberOfSecondsToWaitToConnect            = 30;


    m_pThisLocation->connectToRemoteLocation (clusterPrimaryLocationId, maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect);

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeConnectToNewKnownLocationsStep : Now connected to Primary Location.");

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep : Entering ...");

    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ());

    DatabaseObjectManagerBackupMessage message (backupFileName, false); // Request not to pause the service.  By default it pauses the service.
    message.setDataOnlyBackup (true);

    ResourceId status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep : Could not send message to backup database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep : Message to backup database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep: Entering ...");

    DatabaseObjectManagerCleanPreparedTransactionsMessage databaseObjectManagerCleanPreparedTransactionsMessage;
    ResourceId                                            cleanPreparedTransactionsStatus                        = WAVE_MESSAGE_SUCCESS;

    cleanPreparedTransactionsStatus = sendSynchronously (&databaseObjectManagerCleanPreparedTransactionsMessage);

    if (WAVE_MESSAGE_SUCCESS != cleanPreparedTransactionsStatus)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep: Could not send message to Clean the Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        cleanPreparedTransactionsStatus = databaseObjectManagerCleanPreparedTransactionsMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == cleanPreparedTransactionsStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep: Succeeded in cleaning the previously Prepared Transactions.");
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep: Failed to Clean the previously Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (cleanPreparedTransactionsStatus);
}

void PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep : Entering ...");

    DatabaseObjectManagerEmptyMessage message;
    ResourceId                        status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep : Could not send message to empty database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep : Message to empty database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep : Entering ...");
    PrismConfigureClusterSecondaryPhase1Message *pPrismConfigureClusterSecondaryPhase1Message = reinterpret_cast<PrismConfigureClusterSecondaryPhase1Message *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    UI32                                   sizeOfTheDatabaseBackupFromPrimary     = 0;
    char                                  *pBuffer                                = reinterpret_cast<char *> (pPrismConfigureClusterSecondaryPhase1Message->findBuffer (pPrismConfigureClusterSecondaryPhase1Message->getDatabaseBackupBufferTag (), sizeOfTheDatabaseBackupFromPrimary));

    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName3 ());

    string filenameToSync;

    pPrismConfigureClusterSecondaryPhase1Message->getFilenameToSync (filenameToSync);

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if ( false == filenameToSync.empty())
    {
        UI32   sizeOfFileBuffer;
        char *pFileBuffer = (reinterpret_cast<char *> (pPrismConfigureClusterSecondaryPhase1Message->findBuffer ( 11, sizeOfFileBuffer) ));
        prismAssert ( NULL != pFileBuffer, __FILE__, __LINE__);

        string configMgmtScriptsPath = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
        string configMgmtFileName = configMgmtScriptsPath + "/" + ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();

        status = createFileForSyncBuffer ( configMgmtFileName, pFileBuffer, sizeOfFileBuffer);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep : Could not create file sent from primary.  Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    if (NULL != pBuffer)
    {
        ofstream                               databaseBackupFile;

        databaseBackupFile.open (backupFileName.c_str (), ios::binary);
        databaseBackupFile.write (pBuffer, sizeOfTheDatabaseBackupFromPrimary);
        databaseBackupFile.close ();
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep : No Database backup has been received ???");
    }

    // Request the Database service to restore itself from the backup file that we created based on the buffer we obtained from the primary.

    DatabaseObjectManagerRestoreMessage message (backupFileName);
    message.setDataOnlyRestore (true);

    status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep : Could not send message to restore database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep : Message to restore database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep : SUCCESSFULLY Synced up local database with that of Primary.");
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeUpdateInstanceIdsStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    PersistenceLocalObjectManagerUpdateInstanceIdMesssage message;

    ResourceId status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeUpdateInstanceIdsStep: Could not send message to update Instance Ids.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeUpdateInstanceIdsStep: Message to update instance Ids failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeShutdownServicesStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeShutdownServicesStep : Entering ...");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    status = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_CONFIGURE);

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeConfigureMyLocationIdStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeConfigureMyLocationIdStep : Entering ...");

    PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    LocationId                             myNewLocationId                        = pPrismConfigureClusterSecondaryMessage->getLocationId ();
    SI32                                   clusterPrimaryPort                     = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryPort ();
    LocationId                             clusterPrimaryLocationId               = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryLocationId ();

    // First set my new locationId.  Because, when we inform the localclusterservice it might try to use remote communication
    // for heartbeat purposes.
    // Also All event registration related entities must use the correct location id.

    // Set my new location id

    m_pThisLocation->setLocationId (myNewLocationId);

    // Set the cluster primary locationId

    m_pThisLocation->setClusterPrimaryLocationId (clusterPrimaryLocationId);
    m_pThisLocation->setClusterPrimaryPort       (clusterPrimaryPort);

    // Configure the location Role.

    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_CLUSTER_PHASE_0);

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseStep : Entering ...");

    ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_SECONDARY_CONFIGURE, WAVE_BOOT_PHASE_PRE_PHASE);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseStep : Booting Secondary immediately after configuring must NEVER fail.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseForStandbyStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseForStandbyStep : Entering ...");

    ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_HASTANDBY, WAVE_BOOT_PHASE_PRE_PHASE);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseForStandbyStep : Booting Secondary immediately after configuring must NEVER fail.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodeSetLocationRoleOnStandbyStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodeSetLocationRoleOnStandbyStep: Entering ...");

    // Setting the location role is done here to handle the scenario when active goes away before syncing the location role
    // to standby(via saveConfigurationStep), the location role on standby should be correct inorder to perform necessary
    // recovery actions upon active failure, this is observed during slotpoweron/off on a chassis in the middle of add node

    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_CLUSTER_PHASE_0);

    // Save the location role in configuration file on standby to ensure it comes in appropriate role even
    // if something unexpected happens on standby
    FrameworkToolKit::savePrismConfiguration (false);
    
    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::rejoinSecondaryNodeSetLocationRoleOnStandbyStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::rejoinSecondaryNodeSetLocationRoleOnStandbyStep: Entering ...");

    // Setting the location role is done here to handle the scenario when active goes away before syncing the location role
    // to standby(via saveConfigurationStep), the location role on standby should be correct inorder to perform necessary
    // recovery actions upon active failure, this is observed during slotpoweron/off on a chassis with rejoin/primary change in progress

    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_REJOIN_PHASE_0);

    // Save the location role in configuration file on standby to ensure it comes in appropriate role even
    // if something unexpected happens on standby
    FrameworkToolKit::savePrismConfiguration (false);

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::primaryNodeClusterSuccessStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    PrismFrameworkObjectManager::prismLinearSequencerSucceededStep (pPrismLinearSequencerContext);

    setPrimaryNodeClusterOperationFlag (false);
    resumePostponedMessages ();
}

void PrismFrameworkObjectManager::primaryNodeClusterFailureStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    PrismFrameworkObjectManager::prismLinearSequencerFailedStep (pPrismLinearSequencerContext);

    setPrimaryNodeClusterOperationFlag (false);
    resumePostponedMessages ();
}

void PrismFrameworkObjectManager::secondaryNodeClusterFailureStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::secondaryNodeClusterFailureStep connecting back to primary to return error back to primary"));

    PrismMessage *pMessage = reinterpret_cast<PrismMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());

    SI32       currentPrimaryPort       = pSecondaryNodeClusterContext->getClusterPrimaryPort       ();
    LocationId currentPrimaryLocationId = pSecondaryNodeClusterContext->getClusterPrimaryLocationId ();
    string     currentPrimaryIpAddress  = pSecondaryNodeClusterContext->getClusterPrimaryIpAddress  ();


    if (0 != currentPrimaryLocationId)
    {
        if (false == isAKnownLocation (currentPrimaryLocationId))
        {
            m_pThisLocation->addKnownLocation (currentPrimaryLocationId, currentPrimaryIpAddress, currentPrimaryPort);

            pMessage->setRemoveNodeFromKnownLocationAfterReply (true);
        }
    }
    else
    {
        currentPrimaryLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
        currentPrimaryIpAddress  = FrameworkToolKit::getClusterPrimaryIpAddress  ();
        currentPrimaryPort       = FrameworkToolKit::getClusterPrimaryPort ();
    }

    if (false == FrameworkToolKit::isAConnectedLocation (currentPrimaryLocationId))
    {
        ResourceId connectionStatus = m_pThisLocation->connectToRemoteLocation (currentPrimaryLocationId, 30, 30);
    
        if (FRAMEWORK_SUCCESS != connectionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::secondaryNodeClusterFailureStep : Could not connect to Current Primary : ") + currentPrimaryIpAddress + string (", Status = ") + FrameworkToolKit::localize (connectionStatus));
        }
    }

    pMessage->setDisconnectFromLocationAfterReply (true);

    PrismFrameworkObjectManager::prismLinearSequencerFailedStep (pSecondaryNodeClusterContext);

    setSecondaryNodeClusterCreationFlag (false);
    if (true == getNeedNotifyClusterReadyState())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::secondaryNodeClusterFailureStep: Notify on ClusterReady event for secondary join falure event");
        bool readyState = true; 
        notifyClusterReadyState (readyState);
        setNeedNotifyClusterReadyState (false);
    }
    resumePostponedMessages ();
}

void PrismFrameworkObjectManager::startClusterPhaseTimer (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, " PrismFrameworkObjectManager::startClusterPhaseTimer ");

    if (0 != m_clusterPhaseTimerHandler)
    {
        trace (TRACE_LEVEL_INFO, "Previous Timer is still not expired, so cancelling previous Timer before Starting Timer Again");

        deleteTimer (m_clusterPhaseTimerHandler);
        m_clusterPhaseTimerHandler = 0;
    }

    startTimer (m_clusterPhaseTimerHandler, 300000, reinterpret_cast<PrismTimerExpirationHandler> (&PrismFrameworkObjectManager::clusterPhaseTimerCallback));

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::stopClusterPhaseTimer(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Stopping Cluster Phase Timer as expected phase is received");

    if (0 != m_clusterPhaseTimerHandler)
    {
        deleteTimer (m_clusterPhaseTimerHandler);

        m_clusterPhaseTimerHandler = 0;
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::clusterPhaseTimerCallback (TimerHandle timerHandle, void * pContext)
{
    LocationRole thisLocationRole = m_pThisLocation->getLocationRole ();

    m_clusterPhaseTimerHandler = 0;

    disconnectFromAllConnectedNodes ();

    if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 == thisLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_1 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_1 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_0 == thisLocationRole))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::PrismFrameworkObjectManager::clusterPhaseTimerCallback : Timer expired as Expected Phase 2 is not Received");

        ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::clusterPhaseTimerCallback : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::clusterPhaseTimerCallback : Roll Back complete hence resetting cluster creation flag in framework");    
        PrismFrameworkObjectManager::setSecondaryNodeClusterCreationFlag (false);
        if (true == getNeedNotifyClusterReadyState())
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::clusterPhaseTimerCallback: Notify on ClusterReady event for clusterPhaseTimeout event");
            bool readyState = true;
            notifyClusterReadyState (readyState);
            setNeedNotifyClusterReadyState (false);
        }
        resumePostponedMessages ();
    }
    else if ((LOCATION_SECONDARY_CLUSTER_PHASE_2 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_2 == thisLocationRole))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::PrismFrameworkObjectManager::clusterPhaseTimerCallback : Timer expired as expected phase 3 is not received");

        ResourceId status = WAVE_MESSAGE_SUCCESS;
    
        status = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_ROLLBACK);
        
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::clusterPhaseTimerCallback : shutdown should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
        
        status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::clusterPhaseTimerCallback : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::clusterPhaseTimerCallback : Roll Back from phase2 complete hence resetting cluster creation flag in framework");
        PrismFrameworkObjectManager::setSecondaryNodeClusterCreationFlag (false);
        if (true == getNeedNotifyClusterReadyState())
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::clusterPhaseTimerCallback: Notify on ClusterReady event for clusterPhaseTimeout event");
            bool readyState = true;
            notifyClusterReadyState (readyState);
            setNeedNotifyClusterReadyState (false);
        }
        resumePostponedMessages ();
    }

}

void PrismFrameworkObjectManager::configureSecondaryNodePhase2MessageHandler (PrismConfigureClusterSecondaryPhase2Message *pPrismConfigureClusterSecondaryPhase2Message)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInPhase2),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),  
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase2BootServicesPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase2ConfigureThisLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
//        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3ExecutePostBootStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context = new SecondaryNodeClusterPhase2Context (pPrismConfigureClusterSecondaryPhase2Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), false);

    pSecondaryNodeClusterPhase2Context->holdAll ();
    pSecondaryNodeClusterPhase2Context->start ();
}

void PrismFrameworkObjectManager::updateLocationRoleStepInPhase2 (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context)
{
    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if (LOCATION_SECONDARY_CLUSTER_PHASE_1 == locationRole)
    {
        m_pThisLocation->setLocationRole (LOCATION_SECONDARY_CLUSTER_PHASE_2);
        pSecondaryNodeClusterPhase2Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::updateLocationRoleStepInPhase2 received Message in incorrect location role, return with error");
        pSecondaryNodeClusterPhase2Context->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void PrismFrameworkObjectManager::configureSecondaryNodePhase2BootServicesPostPhaseStep (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodePhase2BootServicesPostPhaseStep : Entering ...");

    ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_SECONDARY_CONFIGURE, WAVE_BOOT_PHASE_POST_PHASE);

    // Instead of asserting, rollback to last known good state.

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "configureSecondaryNodePhase2BootServicesPostPhaseStep: Rollback to last known state");

        disconnectFromAllConnectedNodes ();

        ResourceId rollbackStatus = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_ROLLBACK);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodePhase2BootServicesPostPhaseStep : shutdown should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }

        rollbackStatus = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodePhase2BootServicesPostPhaseStep : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    
#if 0
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodePhase2BootServicesPostPhaseStep : Booting Secondary immediately after configuring must NEVER fail.");
        prismAssert (false, __FILE__, __LINE__);
    }
#endif
    pSecondaryNodeClusterPhase2Context->executeNextStep (status);
}


/// Name
/// configureSecondaryNodePhase2ReplaceNodeIfNeededStep
/// Description
/// This function sends a message to the local cluster service
/// to inform of a new primary node and the local cluster service
/// to inform of replaced node 
///  Input
///  SecondaryNodeClusterContext*:Pointer to phas2context 
/// Output
/// none
/// Return
/// none
void PrismFrameworkObjectManager::configureSecondaryNodePhase2ReplaceNodeIfNeededStep (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodePhase2ReplaceNodeIfNeededStep : Entering ...");

    PrismRejoinClusterSecondaryPhase2Message *pPrismRejoinClusterSecondaryPhase2Message = reinterpret_cast<PrismRejoinClusterSecondaryPhase2Message *> (pSecondaryNodeClusterPhase2Context->getPPrismMessage ());
    ResourceId         processingStatus = WAVE_MESSAGE_SUCCESS;   

    if (true == pPrismRejoinClusterSecondaryPhase2Message->getIsReplaceRejoin ())
    {
        // Inform the local cluster configuration service that the node was replaced so that it can configure itself
        ClusterLocalReportReplaceMessage message;

        LocationId         thisLocationId       = m_pThisLocation->getLocationId();

        message.setThisNodeIpAddress    (m_pThisLocation->getIpAddress ());
        message.setThisNodePort         (m_pThisLocation->getPort());
        message.setThisNodeLocationId   (thisLocationId);

        WaveMessageStatus  status = sendSynchronously (&message, thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            processingStatus = status;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                processingStatus = completionStatus;
            }
        }

        if (WAVE_MESSAGE_SUCCESS != processingStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::configureSecondaryNodePhase2ReplaceNodeIfNeededStep : Configuring this location failed : ") + FrameworkToolKit::localize(processingStatus));
        }
    }

    pSecondaryNodeClusterPhase2Context->executeNextStep(processingStatus);
   
}

/// Name
/// configureSecondaryNodePhase2ConfigureThisLocationStep
/// Description
/// This function sends a message to the local cluster service
/// should then update its node status
///  Input
///  SecondaryNodeClusterContext*:Pointer to phas2context 
/// Output
/// none
/// Return
/// none
void PrismFrameworkObjectManager::configureSecondaryNodePhase2ConfigureThisLocationStep (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::configureSecondaryNodePhase2ConfigureThisLocationStep : Entering ...");

    // Now inform the local cluster configuration service that the primary changed so that it can configure itself
    ClusterLocalReportPrimaryNodeChangedMessage message;

    //In phase1 of secondary node confguration m_pThisLocation has already been populated
    //Here we will just copy fields from m_pThisLocation to populate the message being sent
    // to the local cluster service

    message.setThisNodeIpAddress    (m_pThisLocation->getIpAddress ());
    message.setThisNodePort         (m_pThisLocation->getPort());

    LocationId thisLocationId = m_pThisLocation->getLocationId();
    message.setThisNodeLocationId   (thisLocationId);

    WaveMessageStatus  status = sendSynchronously (&message, thisLocationId);
    ResourceId         processingStatus = WAVE_MESSAGE_SUCCESS;   

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        processingStatus = status;
    }
    else
    {
        ResourceId completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            processingStatus = completionStatus;
        }
    }

    if (WAVE_MESSAGE_SUCCESS != processingStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::configureSecondaryNodePhase2ConfigureThisLocationStep : Configuring this location failed : ") + FrameworkToolKit::localize(processingStatus));
    }

    pSecondaryNodeClusterPhase2Context->executeNextStep(processingStatus);
   
}

void PrismFrameworkObjectManager::configureSecondaryNodePhase3SendStartHearBeat (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    string primaryNodeIpAddress = FrameworkToolKit::getClusterPrimaryIpAddress();
    UI32   primaryNodePort      = FrameworkToolKit::getClusterPrimaryPort();

    StartHeartBeatMessage  *pMessage = new StartHeartBeatMessage ();
    pMessage->setDstIpAddress (primaryNodeIpAddress);
    pMessage->setDstPortNumber (primaryNodePort);
    pMessage->setHeartBeatInterval (DEFAULT_HEART_BEAT_PERIOD);
    pMessage->setHeartBeatFailureThreshold (DEFAULT_MAX_LOST_HEARTBEATS);

    trace (TRACE_LEVEL_DEBUG, string("configureSecondaryNodePhase3SendStartHearBeat: ") + primaryNodeIpAddress + string(" Port :") +  primaryNodePort);

    WaveMessageStatus status =  sendSynchronously (pMessage);
    ResourceId       processingStatus = WAVE_MESSAGE_SUCCESS;

    if(WAVE_MESSAGE_SUCCESS != status) 
    {
    processingStatus = status;
    }
    else 
    {
        ResourceId completionStatus = pMessage->getCompletionStatus();
        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            processingStatus = completionStatus;
        }
    }

    delete (pMessage);

    if (WAVE_MESSAGE_SUCCESS != processingStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::configureSecondaryNodePhase3SendStartHearBeat : Send StartHeartBeatMessage Failed: ") + FrameworkToolKit::localize(processingStatus));
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep(processingStatus);
}

void PrismFrameworkObjectManager::configureSecondaryNodePhase3ExecutePostBootStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS ;

    if ( NULL == m_pPrismFrameworkObjectManagerPostPersistentBootWorker )
    {   
        trace (TRACE_LEVEL_FATAL, string("PrismFrameworkObjectManager::configureSecondaryNodePhase3ExecutePostBootStep : postboot worker pointer is NULL") );
        pSecondaryNodeClusterPhase3Context->executeNextStep( WAVE_MESSAGE_ERROR );
    }

    // During cluster reformation since DB is replaced we trigger COLD recovery
    ResourceId recoveryType = WAVE_HA_COLD_RECOVERY;

    PrismPostPersistentBootWorkerClusterContext *pPostPersistentBootWorkerClusterContext = new PrismPostPersistentBootWorkerClusterContext();
    pPostPersistentBootWorkerClusterContext->setRecoveryType( recoveryType );

    status = m_pPrismFrameworkObjectManagerPostPersistentBootWorker->triggerPostBootPassTableForAll ( pPostPersistentBootWorkerClusterContext );

    if ( WAVE_MESSAGE_SUCCESS != status )
    {   
        trace (TRACE_LEVEL_FATAL, string("PrismFrameworkObjectManager::configureSecondaryNodePhase3ExecutePostBootStep : postboot failed with status -") + FrameworkToolKit::localize ( status ) );
    }

    delete ( pPostPersistentBootWorkerClusterContext );
    pSecondaryNodeClusterPhase3Context->executeNextStep(status);
}

void PrismFrameworkObjectManager::configureSecondaryNodePhase3MessageHandler (PrismConfigureClusterSecondaryPhase3Message *pPrismConfigureClusterSecondaryPhase3Message)
{
    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodePhase3MessageHandler: (ACTIVE)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3StartEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInPhase3),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryHaPeerPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3BootServicesAfterPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3ExecutePostBootStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3ReplayLocalOnlyConfigIfRequiredStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepPhase3Complete),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3SendStartHearBeat),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterPhase3Context = new SecondaryNodeClusterPhase3Context (pPrismConfigureClusterSecondaryPhase3Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterPhase3Context->holdAll ();
    pSecondaryNodeClusterPhase3Context->start ();
}

void PrismFrameworkObjectManager::configureSecondaryNodeHaPeerPhase3MessageHandler (PrismConfigureClusterSecondaryHaPeerPhase3Message *pPrismConfigureClusterSecondaryHaPeerPhase3Message)
{
    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodePhase3MessageHandler: (STANDBY)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prepareStandbyAfterClusterPhaseSync),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterPhase3Context = new SecondaryNodeClusterPhase3Context (pPrismConfigureClusterSecondaryHaPeerPhase3Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterPhase3Context->holdAll ();
    pSecondaryNodeClusterPhase3Context->start ();

}

void PrismFrameworkObjectManager::updateLocationRoleStepInPhase3 (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{

    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if (LOCATION_SECONDARY_CLUSTER_PHASE_2 == locationRole)
    {
    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_CLUSTER_PHASE_3);
    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::updateLocationRoleStepInPhase3 received Message in incorrect location role, return with error");
        pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void PrismFrameworkObjectManager::updateLocationRoleStepPhase3Complete (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    m_pThisLocation->setLocationRole (LOCATION_SECONDARY);
    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::configureSecondaryNodePhase3BootServicesAfterPostPhaseStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodePhase3BootServicesAfterPostPhaseStep : Entering ...");

    ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_SECONDARY_CONFIGURE, WAVE_BOOT_PHASE_AFTER_POST_PHASE);

    // Instead of asserting, rollback to last known good state.

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "configureSecondaryNodePhase3BootServicesAfterPostPhaseStep: Rollback to last known state");

        disconnectFromAllConnectedNodes ();

        ResourceId rollbackStatus = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_ROLLBACK);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodePhase3BootServicesAfterPostPhaseStep : shutdown should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }

        rollbackStatus = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodePhase3BootServicesAfterPostPhaseStep : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

#if 0
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureSecondaryNodePhase3BootServicesAfterPostPhaseStep : Configuring Boot Phases for Phase 3 Fails");
        prismAssert (false, __FILE__, __LINE__);
    }
#endif
    pSecondaryNodeClusterPhase3Context->executeNextStep (status);
}

void PrismFrameworkObjectManager::configureSecondaryNodePhase3ReplayLocalOnlyConfigIfRequiredStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::configureSecondaryNodePhase3ReplayLocalOnlyConfigIfRequiredStep : Starting ...");

    string configFileForReplay (ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/" + ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ());

    string configFilePath           = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
    string localOnlyFilename        = ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();
    string fullLocalOnlyFilename    = configFilePath + "/" + localOnlyFilename;
    string preConfigReplayCommands  = "bypassCliValidations cluster-formation\nbypassCliValidations principal-only";
    string globalOnlyFilename       = ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ();
    string fullGlobalOnlyFilename   = configFilePath + "/" + globalOnlyFilename;


    if (true == ConfigFileManagementToolKit::isFileExisting (fullLocalOnlyFilename))
    {
        // replay the local only configuration file

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodePhase3ReplayLocalOnlyConfigIfRequiredStep : Replay local-only config file on secondary node... ");

        status = ConfigFileManagementToolKit::replayConfigurationFile (configFilePath, localOnlyFilename, preConfigReplayCommands);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::configureSecondaryNodePhase3ReplayLocalOnlyConfigIfRequiredStep : Replay of local-only config file on secondary node completed with status: " + FrameworkToolKit::localize (status));

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            // rename the local only configuration file

            string newLocalOnlyFilename = fullLocalOnlyFilename + "." + FrameworkToolKit::getThisLocationIpAddress () + ".backup";
        
            ConfigFileManagementToolKit::renameConfigurationFile (fullLocalOnlyFilename, newLocalOnlyFilename);
        }
    }

    if (true == ConfigFileManagementToolKit::isFileExisting (fullGlobalOnlyFilename))
    {
        // rename the global only configuration file

        string newGlobalOnlyFilename = fullGlobalOnlyFilename + "." + FrameworkToolKit::getThisLocationIpAddress () + ".backup";

        ConfigFileManagementToolKit::renameConfigurationFile (fullGlobalOnlyFilename, newGlobalOnlyFilename);
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (status);
}

void PrismFrameworkObjectManager::updateListOfSecondariesMessageHandler (FrameworkObjectManagerUpdateListOfSecondariesMessage *pFrameworkObjectManagerUpdateListOfSecondariesMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateListOfSecondariesSetupContextStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateListOfSecondariesAddKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateListOfSecondariesConnectToNewKnownLocationsStep),        
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),  
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastListOfNewlyAddedNodesStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext = new UpdateListOfSecondariesContext (pFrameworkObjectManagerUpdateListOfSecondariesMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pUpdateListOfSecondariesContext->holdAll ();
    pUpdateListOfSecondariesContext->start ();
}

void PrismFrameworkObjectManager::updateListOfSecondariesSetupContextStep (UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext)
{

    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if (LOCATION_SECONDARY_CLUSTER_PHASE_2 != locationRole && LOCATION_SECONDARY_REJOIN_PHASE_2 != locationRole && LOCATION_SECONDARY != locationRole)
    {    
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::updateListOfSecondariesSetupContextStep received Message in incorrect location role, return with error");
        pUpdateListOfSecondariesContext->executeNextStep (WAVE_MESSAGE_ERROR);

        return;
    }    

    FrameworkObjectManagerUpdateListOfSecondariesMessage *pFrameworkObjectManagerUpdateListOfSecondariesMessage = reinterpret_cast<FrameworkObjectManagerUpdateListOfSecondariesMessage *> (pUpdateListOfSecondariesContext->getPPrismMessage ());
    UI32                                                  numberOfNewSecondaryLocations                         = pFrameworkObjectManagerUpdateListOfSecondariesMessage->getNumberOfSecondaryLocations ();
    UI32                                                  i                                                     = 0;
    LocationId                                            locationId;
    string                                                ipAddress;
    UI32                                                  port;
    LocationId                                            thisLocationId                                        = FrameworkToolKit::getThisLocationId ();
    bool                                                  isNodeNew                                             = false;
    vector<LocationId>                                    connectedLocationIds;

    for (i = 0; i < numberOfNewSecondaryLocations; i++)
    {
        locationId = pFrameworkObjectManagerUpdateListOfSecondariesMessage->getLocationIdAtIndex (i);
        ipAddress  = pFrameworkObjectManagerUpdateListOfSecondariesMessage->getIpAddressAtIndex (i);
        port       = pFrameworkObjectManagerUpdateListOfSecondariesMessage->getPortAtIndex (i);
        isNodeNew  = pFrameworkObjectManagerUpdateListOfSecondariesMessage->isNodeNewAtIndex (i);

        if (thisLocationId != locationId)
        {
            pUpdateListOfSecondariesContext->addSecondaryNodeDetails (locationId, ipAddress, port, isNodeNew);
        }
        //If location id  of this location matches some location id received in the message and the node is newly added
        //then update this information in the context. Later on this information is used to populate the lists 
        // in the broadcast event
        else if(isNodeNew) 
        {
            pUpdateListOfSecondariesContext->setIsThisNodeNew(true); 
        }
    }

    pFrameworkObjectManagerUpdateListOfSecondariesMessage->getConnectedLocationIds (connectedLocationIds);

    pUpdateListOfSecondariesContext->setConnectedLocationsAtPrimary (connectedLocationIds);

    FrameworkToolKit::setLastUsedLocationId (pFrameworkObjectManagerUpdateListOfSecondariesMessage->getLastUsedLocationId ());

    pUpdateListOfSecondariesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::updateListOfSecondariesAddKnownLocationsStep (UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext)
{
    UI32       numberOfNewSecondaryLocations = pUpdateListOfSecondariesContext->getNumberOfSecondaryLocations ();
    UI32       i                             = 0;
    UI32       j                             = 0;
    LocationId locationId;
    string     ipAddress;
    UI32       port;
    UI32       numberOfKnownLocations    = 0;
    vector<LocationId> knownLocationIdsVector;            
    bool       removeFlag                    = true;
    LocationId currentPrimaryLocationId      = FrameworkToolKit::getClusterPrimaryLocationId ();

    FrameworkToolKit::getKnownLocations (knownLocationIdsVector);
    numberOfKnownLocations = knownLocationIdsVector.size ();

    for (i = 0; i < numberOfNewSecondaryLocations; i++)
    {
        locationId = pUpdateListOfSecondariesContext->getLocationIdAtIndex (i);
        ipAddress  = pUpdateListOfSecondariesContext->getIpAddressAtIndex (i);
        port       = pUpdateListOfSecondariesContext->getPortAtIndex (i);

        if (false == (FrameworkToolKit::isAKnownLocation (ipAddress, port)))
        {
            m_pThisLocation->addKnownLocation (locationId, ipAddress, port);

            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::updateListOfSecondariesAddKnownLocationsStep : Added New Known Location : LocationId : ") + locationId + string (", ") + ipAddress + string (":") + port);
        }

    }

    for (j = 0; j < numberOfKnownLocations; j++)
    {
        for (i = 0; i < numberOfNewSecondaryLocations; i++)
        {

            if (pUpdateListOfSecondariesContext->getLocationIdAtIndex (i) == knownLocationIdsVector [j])
            {
                removeFlag = false;
                break;
            }
        }

        if (true == removeFlag && (currentPrimaryLocationId != knownLocationIdsVector [j]))
        {
           disconnectFromLocation (knownLocationIdsVector[j]);
           removeKnownLocation (knownLocationIdsVector[j]);
        }
        else
        {
            removeFlag = true;
        }

    }

    pUpdateListOfSecondariesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::updateListOfSecondariesConnectToNewKnownLocationsStep (UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext)
{
                 vector<LocationId> primaryConnectedLocations;        
                 UI32               i                                                = 0;
                 LocationId         locationId;
    static const UI32               maximumNumberOfRetriesToConnectToARemoteLocation = 30;
    static const UI32               maximumNumberOfSecondsToWaitToConnect            = 30;
         vector<LocationId>         remoteLocationIdsVector;
                 //UI32               numberOfRemoteLocationIds;
                 UI32               numberOfLocationConnectedToPrimary;
    
    FrameworkToolKit::getConnectedLocations (remoteLocationIdsVector);
    //numberOfRemoteLocationIds = remoteLocationIdsVector.size ();

    pUpdateListOfSecondariesContext->getConnectedLocationsAtPrimary (primaryConnectedLocations);
    numberOfLocationConnectedToPrimary = primaryConnectedLocations.size ();

    for (i = 0; i < numberOfLocationConnectedToPrimary; i++)
    {
        locationId = primaryConnectedLocations [i];

        if (false == FrameworkToolKit::isAConnectedLocation (locationId))
        {
            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::updateListOfSecondariesAddKnownLocationsStep : Connecting to New Known Location : LocationId : ") + locationId);
            m_pThisLocation->connectToRemoteLocation (locationId, maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect);
        }
    }

    pUpdateListOfSecondariesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
/// Name
/// broadcastListOfNewlyAddedNodesStep
/// Description
/// Broadcasts (on the secondary) list of all new nodes 
/// added as part of addnode or create cluster 
/// Input
/// pdateListOfSecondariesContext *: Used by the f/w
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::broadcastListOfNewlyAddedNodesStep(UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext)
{
  
   trace(TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::broadcastListOfNewlyAddedNodesStep: Entering.."); 

   FrameworkObjectManagerUpdateListOfSecondariesMessage *pFrameworkObjectManagerUpdateListOfSecondariesMessage = dynamic_cast<FrameworkObjectManagerUpdateListOfSecondariesMessage *> (pUpdateListOfSecondariesContext->getPPrismMessage ());

   prismAssert( NULL != pFrameworkObjectManagerUpdateListOfSecondariesMessage, __FILE__, __LINE__);

   //For now broadcast is suppressed for deletion
   if(pFrameworkObjectManagerUpdateListOfSecondariesMessage->isDeletion()) 
   {
     trace(TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::broadcastListOfNewlyAddedNodesStep:Suppressing broadcast for delete");
     pUpdateListOfSecondariesContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
     return;
   }
   UI32 numberOfNewSecondaryLocations = pUpdateListOfSecondariesContext->getNumberOfSecondaryLocations ();
   WaveNewNodesAddedEvent* pWaveNewNodesAddedEvent = new WaveNewNodesAddedEvent();

   //if it is a new node broadcast should contain all nodes received in the message
   // as well as the primary node
   if(pUpdateListOfSecondariesContext->isThisNodeNew())
   {
     //this means that this node is also new and we need to add 
     //the primary to the broadcast event's list.
     string     primaryIpAddress  = FrameworkToolKit::getClusterPrimaryIpAddress();
     UI32       primaryPort       = FrameworkToolKit::getClusterPrimaryPort();
     LocationId primaryLocationId = FrameworkToolKit::getClusterPrimaryLocationId();

     pWaveNewNodesAddedEvent->addNewNodeDetails(primaryLocationId, primaryIpAddress, primaryPort);

     //We need to add all nodes
     for(UI32 i = 0; i < numberOfNewSecondaryLocations; ++i) 
      {
         LocationId locationId = pUpdateListOfSecondariesContext->getLocationIdAtIndex(i);
         string ipAddress = pUpdateListOfSecondariesContext->getIpAddressAtIndex(i);
         UI32   port      = pUpdateListOfSecondariesContext->getPortAtIndex(i);
         pWaveNewNodesAddedEvent->addNewNodeDetails(locationId, ipAddress, port);
      }


   }

   //If this node is not new, then add only the nodes that have been newly added 
   else {
       for(UI32 i = 0; i < numberOfNewSecondaryLocations; ++i) 
       {
       if(pUpdateListOfSecondariesContext->isNodeNewAtIndex(i)) 
       {
          LocationId locationId = pUpdateListOfSecondariesContext->getLocationIdAtIndex(i);
          string ipAddress = pUpdateListOfSecondariesContext->getIpAddressAtIndex(i);
          UI32   port      = pUpdateListOfSecondariesContext->getPortAtIndex(i);
          pWaveNewNodesAddedEvent->addNewNodeDetails(locationId, ipAddress, port);
           
       }
       }
   }


    ResourceId status = broadcast(pWaveNewNodesAddedEvent);

    if(status != WAVE_MESSAGE_SUCCESS) 
    {
       trace(TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::broadcastListOfNewlyAddedNodesStep: Failure during broadcast..");
       pUpdateListOfSecondariesContext->executeNextStep (status);
       return;
    }

    trace(TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::broadcastListOfNewlyAddedNodesStep: Exiting..");
    pUpdateListOfSecondariesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void PrismFrameworkObjectManager::getKnownRemoteLocations (vector<LocationId> &remoteLocationsVector)
{
    if (NULL != m_pThisLocation)
    {
        m_pThisLocation->getKnownRemoteLocations (remoteLocationsVector);
    }
}

void PrismFrameworkObjectManager::getKnownLocations (vector<LocationId> &knownLocationsVector)
{
    if (NULL != m_pThisLocation)
    {
        m_pThisLocation->getKnownLocations (knownLocationsVector);
    }
}

void PrismFrameworkObjectManager::getConnectedLocations (vector<LocationId> &connectedLocationsVector)
{
    if (NULL != m_pThisLocation)
    {
        m_pThisLocation->getConnectedLocations (connectedLocationsVector);
    }
}

void PrismFrameworkObjectManager::getKnownSubLocations (vector<LocationId> &knownSubLocationsVector)
{
    if (NULL != m_pThisLocation)
    {
        m_pThisLocation->getKnownSubLocations (knownSubLocationsVector);
    }
}

bool PrismFrameworkObjectManager::isAKnownLocation (LocationId locationId)
{
    if (NULL != m_pThisLocation)
    {
        return (m_pThisLocation->isAKnownLocation (locationId));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "Trying to check if a location is known before this location is configured.");
        prismAssert (false, __FILE__, __LINE__);
        return (false);
    }
}

void PrismFrameworkObjectManager::initializeLastUsedLocationId ()
{
    m_lastUsedLocationId = 256;
}

LocationId PrismFrameworkObjectManager::getNextLocationId ()
{
    UI32 numberOfTries = 0;

    while (true)
    {
        trace (TRACE_LEVEL_INFO, string ("last known locationId: ") + m_lastUsedLocationId);

        m_lastUsedLocationId += 0x00000100;

        if (0 != m_lastUsedLocationId)
        {
            if (false == (isAKnownLocation (m_lastUsedLocationId)))
            {
                trace (TRACE_LEVEL_INFO, string ("return last known locationId: ") + m_lastUsedLocationId);
                return (m_lastUsedLocationId);
            }
        }

        numberOfTries += 0x100;

        if (UINT_MAX == numberOfTries)
        {
            return (0);
        }
    }

    return (0);
}

void PrismFrameworkObjectManager::addNodesToClusterMessageHandler (FrameworkObjectManagerAddNodesToClusterMessage *pFrameworkObjectManagerAddNodesToClusterMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rollbackNodeIfRequiredStep),        
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::addNodesToClusterValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep), // Reuse Sequencer Step
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesPausePersistenceStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesAddKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConnectToNewKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase1Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesResumeDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase2Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep), // Reuse Sequencer Step
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastNewNodesAddedEventOnPrimaryStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::sendReplyBackToClusterGlobalService),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::failoverforNodesFailedInPhase2),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesConfigureNewKnownLocationsPhase3Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startHeartBeatToSecondaryNodes),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastNodesAdditionToClusterCompletedEvent),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterSuccessStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterFailureStep),
    };

    AddNodesToClusterContext *pAddNodesToClusterContext = new AddNodesToClusterContext (pFrameworkObjectManagerAddNodesToClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    // Set an indication that cluster operation is in progress on primary node
    setPrimaryNodeClusterOperationFlag (true);

    // The flag is set here to avoid the default setting in base cluster context creation
    // which causes the previous state of the flag to be lost upon controlled failover.
    // Please refer the class hierarchy for various contexts used in cluster operations
    setSecondaryNodeClusterCreationFlag (false);

    pAddNodesToClusterContext->holdAll ();
    pAddNodesToClusterContext->start ();
}

void PrismFrameworkObjectManager::addNodesToClusterValidateStep (AddNodesToClusterContext *pAddNodesToClusterContext)
{
    FrameworkObjectManagerAddNodesToClusterMessage *pFrameworkObjectManagerAddNodesToClusterMessage = reinterpret_cast<FrameworkObjectManagerAddNodesToClusterMessage *> (pAddNodesToClusterContext->getPPrismMessage ());
    UI32                                            numberOfNewNodes                                = pFrameworkObjectManagerAddNodesToClusterMessage->getNumberOfNewNodes ();
    UI32                                            i                                               = 0;
    string                                          thisLocationIpAddress                           = FrameworkToolKit::getThisLocationIpAddress ();
    SI32                                            thisLocationPort                                = FrameworkToolKit::getThisLocationPort ();

    if (NULL == m_pThisLocation)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::addNodesToClusterValidateStep : Configure this location first.");
        prismAssert (false, __FILE__, __LINE__);
        pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_LOCATION_NOT_CONFIGURED);
        return;
    }
    else
    {
        if (true != (FrameworkToolKit::isThisALocation ()))
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::addNodesToClusterValidateStep : Trying to add a node to cluster using a sub location");
            prismAssert (false, __FILE__, __LINE__);
            pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_SUB_LOCATION_CANNOT_ADD_NODES_TO_CLUSTER);
            return;
        }

        if (LOCATION_PRIMARY != (m_pThisLocation->getRole ()))
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::addNodesToClusterValidateStep : Trying to add a node to cluster using a secondary location.");
            prismAssert (false, __FILE__, __LINE__);
            pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_SECONDARY_LOCATION_CANNOT_ADD_NODES_TO_CLUSTER);
            return;
        }
    }

    for (i = 0; i < numberOfNewNodes; i++)
    {
        string ipAddress = pFrameworkObjectManagerAddNodesToClusterMessage->getNodeAt (i);
        SI32   port      = pFrameworkObjectManagerAddNodesToClusterMessage->getNodePortAt (i);

        if ("" == ipAddress)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::addNodesToClusterValidateStep : Trying to add a node to cluster using a NULL ipAddress (" + ipAddress + ")");
            prismAssert (false, __FILE__, __LINE__);
            pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_NULL_IPADDRESS);
            return;
        }

        sockaddr_in socketAddress;

        SI32 status = inet_pton (AF_INET, ipAddress.c_str (), &socketAddress.sin_addr);

        if (0 >= status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::addNodesToClusterValidateStep : Trying to create a cluster using an invalid ipAddress (" + ipAddress + ")");
            pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_INVALID_IPADDRESS);
            return;
        }


        if (true == (FrameworkToolKit::isAnIpAddressForThisMachine (ipAddress)))
        {
            if (thisLocationPort == port)
            {
                trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::addNodesToClusterValidateStep : Trying to add self as a node to cluster (" + ipAddress + ").  We are already the primary of teh cluster.");
                prismAssert (false, __FILE__, __LINE__);
                pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_ADD_SELF_TO_CLUSTER);
                return;
            }
        }
    }

    pAddNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void PrismFrameworkObjectManager::addNodesToClusterAddKnownLocationsStep (AddNodesToClusterContext *pAddNodesToClusterContext)
{
    FrameworkObjectManagerAddNodesToClusterMessage *pFrameworkObjectManagerAddNodesToClusterMessage = reinterpret_cast<FrameworkObjectManagerAddNodesToClusterMessage *> (pAddNodesToClusterContext->getPPrismMessage ());
    UI32                                            numberOfNewNodes                                = pFrameworkObjectManagerAddNodesToClusterMessage->getNumberOfNewNodes ();
    UI32                                            i                                               = 0;
    UI32                                            numberOfFailures                                = 0;
    string                                          ipAddress                                       = "";
    SI32                                            port                                            = 0;
    LocationId                                      newLocationId                                   = 0;

    for (i = 0; i < numberOfNewNodes; i++)
    {
        ipAddress = pFrameworkObjectManagerAddNodesToClusterMessage->getNodeAt (i);
        port      = pFrameworkObjectManagerAddNodesToClusterMessage->getNodePortAt (i);

        if (false == (FrameworkToolKit::isAKnownLocation (ipAddress, port)))
        {
            newLocationId = getNextLocationId ();

            if (0 != newLocationId)
            {
                m_pThisLocation->addKnownLocation (newLocationId, ipAddress, port);
                pAddNodesToClusterContext->addNewLocationId (newLocationId);
            }
            else
            {
                numberOfFailures++;
                pFrameworkObjectManagerAddNodesToClusterMessage->setNewNodeStatus (ipAddress, port, FRAMEWORK_ERROR_NO_UNUSED_LOCATION_IDS_AVAILABLE);
            }
        }
        else
        {
            numberOfFailures++;
            pFrameworkObjectManagerAddNodesToClusterMessage->setNewNodeStatus (ipAddress, port, FRAMEWORK_ERROR_IPADDRESS_ALREADY_MEMBER_OF_CLUSTER);
        }
    }

    if (numberOfFailures == numberOfNewNodes)
    {
        pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_ADD_NODES_TO_CLUSTER_NO_VALID_RESOURCES);
    }
    else
    {
        pAddNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::addNodesToClusterConnectToNewKnownLocationsStep (AddNodesToClusterContext *pAddNodesToClusterContext)
{
    static const UI32                                            maximumNumberOfRetriesToConnectToARemoteLocation  = 30;
    static const UI32                                            maximumNumberOfSecondsToWaitToConnect             = 30;
                 FrameworkObjectManagerAddNodesToClusterMessage *pFrameworkObjectManagerAddNodesToClusterMessage   = reinterpret_cast<FrameworkObjectManagerAddNodesToClusterMessage *> (pAddNodesToClusterContext->getPPrismMessage ());
                 UI32                                            numberOfNewLocationIds                            = pAddNodesToClusterContext->getNumberOfNewLocationIds ();
                 LocationId                                      newLocationId                                     = 0;
                 SI32                                            i                                                 = 0;
                 string                                          ipAddress                                         = "";
                 SI32                                            port                                              = 0;
                 ResourceId                                      status                                            = FRAMEWORK_SUCCESS;
                 UI32                                            numberOfFailures                                  = 0;

    for (i = numberOfNewLocationIds - 1; i >= 0; i--)
    {
        newLocationId = pAddNodesToClusterContext->getNewLocationIdAt ((UI32) i);
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        if (0 != newLocationId)
        {
            status = m_pThisLocation->connectToRemoteLocation (newLocationId, maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect);

            if (FRAMEWORK_SUCCESS != status)
            {
                // Indicate that the status saying that we could not connect to the location.
                // For this location we will not send a message to configure itself as a secondary.

                pFrameworkObjectManagerAddNodesToClusterMessage->setNewNodeStatus (ipAddress, port, status);

                // Since we are not able to connect to the location remove it from the known locations.

                removeKnownLocation (newLocationId);
                pAddNodesToClusterContext->removeNewLocationId (newLocationId);
                numberOfFailures++;
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    if (numberOfFailures == numberOfNewLocationIds)
    {
        pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONECT_TO_ALL_NEW_LOCATIONS);
    }
    else
    {
        pAddNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::addNodesToClusterConfigureNewKnownLocationsStep (AddNodesToClusterContext *pAddNodesToClusterContext)
{
    FrameworkObjectManagerAddNodesToClusterMessage *pFrameworkObjectManagerAddNodesToClusterMessage = reinterpret_cast<FrameworkObjectManagerAddNodesToClusterMessage *> (pAddNodesToClusterContext->getPPrismMessage ());
    UI32                              numberOfNewLocationIds            = pAddNodesToClusterContext->getNumberOfNewLocationIds ();
    UI32                              i                                 = 0;
    LocationId                        thisLocationId                    = FrameworkToolKit::getThisLocationId ();
    SI32                              thisLocationPort                  = FrameworkToolKit::getThisLocationPort ();
    LocationId                        newLocationId                     = 0;
    string                            thisLocationIpAddress             = FrameworkToolKit::getThisLocationIpAddress ();
    string                            ipAddress                         = "";
    SI32                              port                              = 0;
    UI32                              numberOfFailures                  = 0;
    string                            prismVersionString                = PrismVersion::getVersionString ();

    prismAssert (NULL != pFrameworkObjectManagerAddNodesToClusterMessage, __FILE__, __LINE__);

    ++(*pAddNodesToClusterContext);

    for (i = 0; i < numberOfNewLocationIds; i++)
    {
        newLocationId = pAddNodesToClusterContext->getNewLocationIdAt ((UI32) i);
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        // We will send configure message only to the locations that are known to us at this point.  If user supplied
        // a bogus ipaddress, we would have timed out trying to add that location in the previous step
        // and hence that location will not be known to us at this time.

        if (true == (FrameworkToolKit::isAKnownLocation (ipAddress, port)))
        {
            PrismConfigureClusterSecondaryMessage *pMessage = new PrismConfigureClusterSecondaryMessage (prismVersionString, thisLocationIpAddress, thisLocationPort, thisLocationId, ipAddress, port, newLocationId);

            // Add all the validation buffers to the message before sending out

            vector<PrismServiceId> &prismServiceIds                = pAddNodesToClusterContext->getPrismServiceIdsVector ();
            vector<void *>         &validationDetailsVector        = pAddNodesToClusterContext->getValidationDetailsVector ();
            vector<UI32>           &validationDetailsSizesVector   = pAddNodesToClusterContext->getValidationDetailsSizesVector ();

            UI32                    numberOfPrismServiceIds        = prismServiceIds.size ();
            UI32                    numberOfValidationDetails      = validationDetailsVector.size ();
            UI32                    numberOfValidationDetailsSizes = validationDetailsSizesVector.size ();
            UI32                    j                              = 0;

            prismAssert (numberOfPrismServiceIds == numberOfValidationDetails, __FILE__, __LINE__);
            prismAssert (numberOfPrismServiceIds == numberOfValidationDetailsSizes, __FILE__, __LINE__);

            for (j = 0; j < numberOfPrismServiceIds; j++)
            {
                if ((0 != validationDetailsSizesVector[j]) && (NULL != validationDetailsVector[j]))
                {
                    pMessage->addBuffer (prismServiceIds[j], validationDetailsSizesVector[j], validationDetailsVector[j]);
                }
                else
                {
                    trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::addNodesToClusterConfigureNewKnownLocationsStep : We could not have added an entry with 0 size and/or NULL validation buffer.");
                    prismAssert (false, __FILE__, __LINE__);
                }
            }

            WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::addNodesToClusterConfigureNewKnownLocationsCallback), pAddNodesToClusterContext, s_clusterPhase0TimeoutInMilliseconds, newLocationId);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                removeKnownLocation (newLocationId);
                pFrameworkObjectManagerAddNodesToClusterMessage->setNewNodeStatus (ipAddress, port, status);
                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::addNodesToClusterConfigureNewKnownLocationsStep : Not able to configure one secondary location : ") + newLocationId + string (" Error Code : ") + status);
                numberOfFailures++;

                delete pMessage;
            }
            else
            {
                ++(*pAddNodesToClusterContext);
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    --(*pAddNodesToClusterContext);

    if (numberOfFailures == numberOfNewLocationIds)
    {
        pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pAddNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::addNodesToClusterConfigureNewKnownLocationsCallback (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage, void *pContext)
{
    AddNodesToClusterContext                       *pAddNodesToClusterContext                       = reinterpret_cast<AddNodesToClusterContext *> (pContext);
    FrameworkObjectManagerAddNodesToClusterMessage *pFrameworkObjectManagerAddNodesToClusterMessage = reinterpret_cast<FrameworkObjectManagerAddNodesToClusterMessage *> (pAddNodesToClusterContext->getPPrismMessage ());
    LocationId                                      newLocationId;
    string                                          ipAddress;
    SI32                                            port;

    --(*pAddNodesToClusterContext);

    if (NULL != pPrismConfigureClusterSecondaryMessage)
    {
        newLocationId = pPrismConfigureClusterSecondaryMessage->getLocationId ();
        ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        pAddNodesToClusterContext->addValidaionResultsLocation (ipAddress, port);

        vector<PrismServiceId> &prismServiceIdsToCommunicate = pAddNodesToClusterContext->getPrismServiceIdsToCommunicate ();
        UI32                    numberOfPrismServices        = prismServiceIdsToCommunicate.size ();
        UI32                    i                            = 0;

        for (i = 0; i < numberOfPrismServices; i++)
        {
            void *pValidationResults    = NULL;
            UI32  validationResultsSize = 0;

            pValidationResults = pPrismConfigureClusterSecondaryMessage->transferBufferToUser (s_offSetForValidationResults + prismServiceIdsToCommunicate[i], validationResultsSize);

            pAddNodesToClusterContext->addValidationResultsForService (ipAddress, port, prismServiceIdsToCommunicate[i], pValidationResults, validationResultsSize);
        }

        if (FRAMEWORK_SUCCESS == frameworkStatus)
        {
            prismAssert (NULL != pPrismConfigureClusterSecondaryMessage, __FILE__, __LINE__);

            ResourceId completionStatus = pPrismConfigureClusterSecondaryMessage->getCompletionStatus ();

            pFrameworkObjectManagerAddNodesToClusterMessage->setNewNodeStatus (ipAddress, port, completionStatus);

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                removeKnownLocation (newLocationId);
            }
        }
        else
        {
            if (NULL != pPrismConfigureClusterSecondaryMessage)
            {
                LocationId newLocationId1 = pPrismConfigureClusterSecondaryMessage->getLocationId ();
                string     ipAddress1     = FrameworkToolKit::getIpAddressForLocationId (newLocationId1);
                SI32       port1          = FrameworkToolKit::getPortForLocationId (newLocationId1);

                pFrameworkObjectManagerAddNodesToClusterMessage->setNewNodeStatus (ipAddress1, port1, frameworkStatus);

                removeKnownLocation (newLocationId1);
            }
        }

    }

    if (NULL != pPrismConfigureClusterSecondaryMessage)
    {
        delete pPrismConfigureClusterSecondaryMessage;
    }

    if (0 == (pAddNodesToClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        UI32       numberOfNewIpAddresses  = pFrameworkObjectManagerAddNodesToClusterMessage->getNumberOfNewNodes ();
        UI32       i                       = 0;
        LocationId newLocationId1          = 0;
        string     ipAddress1              = "";
        SI32       port1                   = 0;
        UI32       numberOfFailures        = 0;

        for (i = 0; i < numberOfNewIpAddresses; i++)
        {
            ipAddress1     = pFrameworkObjectManagerAddNodesToClusterMessage->getNodeAt ((UI32) i);
            port1          = pFrameworkObjectManagerAddNodesToClusterMessage->getNodePortAt ((UI32) i);
            newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

            if (true != (pFrameworkObjectManagerAddNodesToClusterMessage->isNewNodeStatusSet (ipAddress1, port1)))
            {
                pFrameworkObjectManagerAddNodesToClusterMessage->setNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_TIMED_OUT);

                if (0 != newLocationId1)
                {
                    removeKnownLocation (newLocationId1);
                }

                numberOfFailures++;
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != (pFrameworkObjectManagerAddNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1)))
                {
                    numberOfFailures++;
                }
            }
        }

        if (numberOfFailures == numberOfNewIpAddresses)
        {
            pAddNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
        }
        else
        {
            pAddNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::deleteNodesFromClusterMessageHandler (FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage)
{
 
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::deleteNodesFromClusterValidateStep),
// reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::deleteNodesFromClusterRemoveKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::deleteNodesFromClusterRunFailoverStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesStep),        
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext = new DeleteNodesFromClusterContext (pFrameworkObjectManagerDeleteNodesFromClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDeleteNodesFromClusterContext->setIsDeletion(true);

    pDeleteNodesFromClusterContext->setIsConfigurationChange (pFrameworkObjectManagerDeleteNodesFromClusterMessage->getIsConfigurationChange ());

    pDeleteNodesFromClusterContext->holdAll ();
    pDeleteNodesFromClusterContext->start ();
}

void PrismFrameworkObjectManager::deleteNodesFromClusterValidateStep (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::deleteNodesFromClusterValidateStep : Entering ...");

    FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage = reinterpret_cast<FrameworkObjectManagerDeleteNodesFromClusterMessage *> (pDeleteNodesFromClusterContext->getPPrismMessage ());
    UI32                                                 numberOfNodes                                        = pFrameworkObjectManagerDeleteNodesFromClusterMessage->getNumberOfNodes ();
    UI32                                                 i                                                    = 0;
    string                                               ipAddress                                            = "";
    SI32                                                 port                                                 = 0;
    UI32                                                 numberOfFailures                                     = 0;
    LocationRole                                         thisLocationRole                                     = m_pThisLocation->getLocationRole ();

    if (LOCATION_PRIMARY != thisLocationRole)
    {
        for (i = 0; i < numberOfNodes; i++)
        {
            ipAddress = pFrameworkObjectManagerDeleteNodesFromClusterMessage->getNodeAt (i);
            port      = pFrameworkObjectManagerDeleteNodesFromClusterMessage->getNodePortAt (i);

            pFrameworkObjectManagerDeleteNodesFromClusterMessage->setNodeStatus (ipAddress, port, FRAMEWORK_ERROR_CANNOT_DELETE_NODES_FROM_CLUSTER_ON_SECONDARY);
        }

        pDeleteNodesFromClusterContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_DELETE_NODES_FROM_CLUSTER_ON_SECONDARY);
        return;
    }

    for (i = 0; i < numberOfNodes; i++)
    {
        ipAddress = pFrameworkObjectManagerDeleteNodesFromClusterMessage->getNodeAt (i);
        port      = pFrameworkObjectManagerDeleteNodesFromClusterMessage->getNodePortAt (i);

        if (true == (FrameworkToolKit::isAKnownLocation (ipAddress, port)))
        {
            pDeleteNodesFromClusterContext->addLocationId (FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress, port));
        }
        else
        {
            pFrameworkObjectManagerDeleteNodesFromClusterMessage->setNodeStatus (ipAddress, port, FRAMEWORK_ERROR_CANNOT_DELETE_NODES_FROM_CLUSTER_NOT_A_KNOWN_LOCATION);
            numberOfFailures++;
        }
    }

    if (numberOfFailures == numberOfNodes)
    {
        pDeleteNodesFromClusterContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_DELETE_NODES_FROM_CLUSTER_NO_VALID_RESOURCES);
    }
    else
    {
        pDeleteNodesFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsStep (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsStep : Entering ...");

    FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage = reinterpret_cast<FrameworkObjectManagerDeleteNodesFromClusterMessage *> (pDeleteNodesFromClusterContext->getPPrismMessage ());
    UI32                                                 numberOfLocationIds                                  = pDeleteNodesFromClusterContext->getNumberOfLocationIds ();
    LocationId                                           locationId                                           = 0;
    UI32                                                 i                                                    = 0;
    WaveMessageStatus                                   messageStatus                                        = WAVE_MESSAGE_SUCCESS;
    string                                               ipAddress                                            = "";
    SI32                                                 port                                                 = 0;

    ++(*pDeleteNodesFromClusterContext);

    for (i = 0; i < numberOfLocationIds; i++)
    {
        locationId = pDeleteNodesFromClusterContext->getLocationIdAt (i);
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId (locationId);

        FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);

        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        messageStatus = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsCallback), pDeleteNodesFromClusterContext, 10000, locationId);

        if (WAVE_MESSAGE_SUCCESS == messageStatus)
        {
            ++(*pDeleteNodesFromClusterContext);
        }
        else
        {
            pFrameworkObjectManagerDeleteNodesFromClusterMessage->setNodeStatus (ipAddress, port, messageStatus);
            delete pMessage;
        }
    }

    --(*pDeleteNodesFromClusterContext);
    pDeleteNodesFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pFrameworkObjectManagerUnconfigureClusterSecondaryMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsCallback : Entering ...");

    DeleteNodesFromClusterContext                       *pDeleteNodesFromClusterContext                       = reinterpret_cast<DeleteNodesFromClusterContext *> (pContext);
    FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage = reinterpret_cast<FrameworkObjectManagerDeleteNodesFromClusterMessage *> (pDeleteNodesFromClusterContext->getPPrismMessage ());
    string                                               ipAddress                                            = "";
    SI32                                                 port                                                 = 0;
    ResourceId                                           completionStatus                                     = WAVE_MESSAGE_SUCCESS;

    --(*pDeleteNodesFromClusterContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pFrameworkObjectManagerUnconfigureClusterSecondaryMessage, __FILE__, __LINE__);

        ipAddress        = pFrameworkObjectManagerUnconfigureClusterSecondaryMessage->getClusterSecondaryIpAddress ();
        port             = pFrameworkObjectManagerUnconfigureClusterSecondaryMessage->getClusterSecondaryPort ();
        completionStatus = pFrameworkObjectManagerUnconfigureClusterSecondaryMessage->getCompletionStatus ();

        if ((WAVE_MESSAGE_SUCCESS == completionStatus) || (WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE == completionStatus))
        {
            trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsCallback : Succeeded to unconfigure a location (" + ipAddress + ").");
            pFrameworkObjectManagerDeleteNodesFromClusterMessage->setNodeStatus (ipAddress, port, WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsCallback : Failed to unconfigure a location (" + ipAddress + "). Status = " + completionStatus);
            pFrameworkObjectManagerDeleteNodesFromClusterMessage->setNodeStatus (ipAddress, port, completionStatus);
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::deleteNodesFromClusterUnconfigureKnownLocationsCallback : Failed to unconfigure a location.");
    }

    if (NULL != pFrameworkObjectManagerUnconfigureClusterSecondaryMessage)
    {
        delete pFrameworkObjectManagerUnconfigureClusterSecondaryMessage;
    }

    pDeleteNodesFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::deleteNodesFromClusterRemoveKnownLocationsStep (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::deleteNodesFromClusterRemoveKnownLocationsStep : Entering ...");

    FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage = reinterpret_cast<FrameworkObjectManagerDeleteNodesFromClusterMessage *> (pDeleteNodesFromClusterContext->getPPrismMessage ());
    UI32                                                 numberOfLocationIds                                  = pDeleteNodesFromClusterContext->getNumberOfLocationIds ();
    LocationId                                           locationId                                           = 0;
    UI32                                                 i                                                    = 0;
    string                                               ipAddress                                            = "";
    SI32                                                 port                                                 = 0;

    for (i = 0; i < numberOfLocationIds; i++)
    {
        locationId = pDeleteNodesFromClusterContext->getLocationIdAt (i);
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId (locationId);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::deleteNodesFromClusterRemoveKnownLocationsStep : 111 ...");

        // In case of deletion of a disconnected node, we need not do disconnectFromLocation again as it would have been
        // done during HB loss itself. 
        if (false == pFrameworkObjectManagerDeleteNodesFromClusterMessage->getIsDisconnected())
        {
            disconnectFromLocation (locationId);

            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::deleteNodesFromClusterRemoveKnownLocationsStep : 222 ...");

        }

        removeKnownLocation (locationId);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::deleteNodesFromClusterRemoveKnownLocationsStep : 333 ...");

        if (false == (pFrameworkObjectManagerDeleteNodesFromClusterMessage->isNodeStatusSet (ipAddress, port)))
        {
            pFrameworkObjectManagerDeleteNodesFromClusterMessage->setNodeStatus (ipAddress, port, FRAMEWORK_ERROR_DELETE_NODES_FROM_CLUSTER_UNCONFIGURE_SECONDARY_TIMED_OUT);
        }
    }

    pDeleteNodesFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::deleteNodesFromClusterRunFailoverStep (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::deleteNodesFromClusterRunFailoverStep : Entering ...");

    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::deleteNodesFromClusterRunFailoverCallback), pDeleteNodesFromClusterContext);
    UI32                                 numberOfLocationIds                  = pDeleteNodesFromClusterContext->getNumberOfLocationIds ();
    UI32                                 i                                    = 0;
    LocationId                           locationId                           = 0;

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
    prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

    for (i = 0;i < numberOfLocationIds; i++)
    {
        locationId = pDeleteNodesFromClusterContext->getLocationIdAt (i);
        pPrismFrameworkFailoverWorkerContext->addFailedLocationId (locationId);
    }

    pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
    pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);
    pPrismFrameworkFailoverWorkerContext->setIsConfigurationChange (pDeleteNodesFromClusterContext->getIsConfigurationChange ());

    // FIXME : sagar : Appropriately set the nodes that are removed so that failover can notify the list of failed nodes to the services.

    m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
}

void PrismFrameworkObjectManager::deleteNodesFromClusterRunFailoverCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::deleteNodesFromClusterRunFailoverCallback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext = reinterpret_cast<DeleteNodesFromClusterContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId                     status                         = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pDeleteNodesFromClusterContext, __FILE__, __LINE__);


    pDeleteNodesFromClusterContext->executeNextStep (status);
}


void PrismFrameworkObjectManager::unconfigureClusterSecondaryMessageHandler (FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pFrameworkObjectManagerUnconfigureClusterSecondaryMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep),      
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondarySendStopHeartBeat),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryDisconnectFromKnownLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryUpdateThisLocationStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryNodeBootServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryUnconfigureThisLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterBroadcastLocalNodeDeletedEventStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext = new DeleteNodeOnSecondaryContext (pFrameworkObjectManagerUnconfigureClusterSecondaryMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    if (true == pFrameworkObjectManagerUnconfigureClusterSecondaryMessage->getIsPerformWarmRecovery ())
    {
        m_pThisLocation->setLocationRole (LOCATION_SECONDARY_UNCONFIRMED);

        m_pThisLocation->setClusterPrimaryLocationId (FrameworkToolKit::getThisLocationId ());
        m_pThisLocation->setClusterPrimaryPort       (FrameworkToolKit::getThisLocationPort());
    }

    pDeleteNodeOnSecondaryContext->holdAll ();
    pDeleteNodeOnSecondaryContext->start ();

}

void PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    LocationRole currentLocationRole = m_pThisLocation->getLocationRole ();
    ResourceId   status              = WAVE_MESSAGE_SUCCESS;

    if ((LOCATION_SECONDARY == currentLocationRole) || (LOCATION_PRIMARY_UNCONFIRMED == currentLocationRole) || (LOCATION_SECONDARY_UNCONFIRMED == currentLocationRole))
    {
        trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep : Proceeding with unconfiguring the cluster secondary.");

        if (LOCATION_SECONDARY != currentLocationRole)
        {
            pDeleteNodeOnSecondaryContext->setIsStartServiceIsRequired (false);
        }
    }
    else
    {
        if (LOCATION_SUB_LOCATION == currentLocationRole)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep : Sub Location coannot be unconfigured from a cluster.");
            status = FRAMEWORK_ERROR_CANNOT_UNCONFIGURE_SUB_LOCATION;
        }
        else if (LOCATION_STAND_ALONE == currentLocationRole)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep : Stand Alone node cannot be unconfigured from Cluster.");
            status = FRAMEWORK_ERROR_CANNOT_UNCONFIGURE_STAND_ALONE_NODE;
        }
        else if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 == currentLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_1 == currentLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_2 == currentLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_3 == currentLocationRole) || 
                (LOCATION_SECONDARY_REJOIN_PHASE_0 == currentLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_1 == currentLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_2 == currentLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_3 == currentLocationRole))
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep : Node is in the middle of cluster operation and cannot be unconfigured.");
            status = FRAMEWORK_ERROR_CANNOT_UNCONFIGURE_LOCATION_WITH_IN_CLUSTER_PHASES_ROLE;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::unconfigureClusterSecondaryValidateStep : Location with unknown role cannot be unconfigured from cluster.");
            status = FRAMEWORK_ERROR_CANNOT_UNCONFIGURE_LOCATION_WITH_UNKNOWN_ROLE;
        }
    }

    pDeleteNodeOnSecondaryContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::unconfigureClusterSecondarySendStopHeartBeat (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    LocationId primaryNodeLocationId = m_pThisLocation->getClusterPrimaryLocationId();
    string     primaryNodeIpAddress  = m_pThisLocation->getIpAddressForLocationId(primaryNodeLocationId);
    UI32       primaryNodePort       = m_pThisLocation->getClusterPrimaryPort();

    StopHeartBeatMessage *pStopHeartBeatMessage = new StopHeartBeatMessage();
    pStopHeartBeatMessage->setDstIpAddress(primaryNodeIpAddress);
    pStopHeartBeatMessage->setDstPortNumber(primaryNodePort);
   
    WaveMessageStatus status = sendSynchronously(pStopHeartBeatMessage);
    ResourceId        processingStatus = WAVE_MESSAGE_SUCCESS;

    if(WAVE_MESSAGE_SUCCESS != status) 
    {
       processingStatus = status;
    }
    else 
    {
       ResourceId    completionStatus = pStopHeartBeatMessage->getCompletionStatus();
       if(completionStatus != WAVE_MESSAGE_SUCCESS) 
       {
         processingStatus = completionStatus;
       }
    }

    if(WAVE_MESSAGE_SUCCESS !=  processingStatus) 
    {
        /* log error and continue */
        trace(TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::unconfigureClusterSecondarySendStopHeartBeat:Failure sending StopHeartBeatMessage: status: " + FrameworkToolKit::localize(processingStatus));
        processingStatus = WAVE_MESSAGE_SUCCESS;
    }

    delete pStopHeartBeatMessage;
    pDeleteNodeOnSecondaryContext->executeNextStep(processingStatus);
}

void PrismFrameworkObjectManager::unconfigureClusterSecondaryDisconnectFromKnownLocationStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    vector<LocationId> knownLocationsVector;
    LocationId         nodeLocationId;
    UI32               nodeCount;
    UI32               totalNodes               = 0;

    FrameworkToolKit::getKnownLocations (knownLocationsVector);
    totalNodes = knownLocationsVector.size ();

    for (nodeCount = 0; nodeCount < totalNodes; nodeCount++)
    {
        nodeLocationId = knownLocationsVector[nodeCount];
        disconnectFromLocation (nodeLocationId);
        removeKnownLocation    (nodeLocationId);

        pDeleteNodeOnSecondaryContext->addKnownLocationId (nodeLocationId);

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::unconfigureClusterSecondaryDisconnectFromKnownLocation ") + nodeLocationId);
    }


    pDeleteNodeOnSecondaryContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::unconfigureClusterSecondaryDisconnectFromConnectedLocationStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    vector<LocationId> connectedLocationsVector;
    LocationId         nodeLocationId;
    UI32               nodeCount;
    UI32               totalNodes               = 0;

    FrameworkToolKit::getConnectedLocations (connectedLocationsVector);
    totalNodes = connectedLocationsVector.size ();

    for (nodeCount = 0; nodeCount < totalNodes; nodeCount++)
    {
        nodeLocationId = connectedLocationsVector[nodeCount];
        disconnectFromLocation (nodeLocationId);
        removeKnownLocation    (nodeLocationId);

        pDeleteNodeOnSecondaryContext->addKnownLocationId (nodeLocationId);

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::unconfigureClusterSecondaryDisconnectFromConnectedLocation ")+nodeLocationId);
    }

    vector<LocationId> knownLocationsVector;

    FrameworkToolKit::getKnownLocations (knownLocationsVector);
    totalNodes = knownLocationsVector.size ();
    
    for (nodeCount = 0; nodeCount < totalNodes; nodeCount++)
    {
        nodeLocationId = knownLocationsVector[nodeCount];
        removeKnownLocation    (nodeLocationId);
        pDeleteNodeOnSecondaryContext->addKnownLocationId (nodeLocationId);

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::unconfigureClusterSecondaryRemovedFromKnownLocation ")+nodeLocationId);

    }

    pDeleteNodeOnSecondaryContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::unconfigureClusterSecondaryUpdateThisLocationStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    LocationId         thisLocationId           = FrameworkToolKit::getThisLocationId ();

    m_pThisLocation->resetLocationToStandAlone (thisLocationId);

    pDeleteNodeOnSecondaryContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

}
void PrismFrameworkObjectManager::unconfigureClusterSecondaryNodeBootServicesStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (true == pDeleteNodeOnSecondaryContext->getIsStartServiceIsRequired ())
    {
        status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_SECONDARY_UNCONFIGURE);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::unconfigureClusterSecondaryNodeBootServicesStep starting of service should never fail");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    pDeleteNodeOnSecondaryContext->executeNextStep (status);
}
void PrismFrameworkObjectManager::unconfigureClusterBroadcastLocalNodeDeletedEventStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    trace(TRACE_LEVEL_DEVEL, "Entering rismFrameworkObjectManager::unconfigureClusterBroadcastLocalNodeDeletedEventStep");
    WaveNodeLocalNodeDeletedEvent* pWaveNodeLocalNodeDeletedEvent = new WaveNodeLocalNodeDeletedEvent();

    ResourceId status =  broadcast(pWaveNodeLocalNodeDeletedEvent);

    if (WAVE_MESSAGE_SUCCESS != status) 
    {
       trace(TRACE_LEVEL_ERROR, "Entering rismFrameworkObjectManager::unconfigureClusterBroadcastLocalNodeDeletedEventStep:Broadcast Failed");
    }

    pDeleteNodeOnSecondaryContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::unconfigureClusterSecondaryUnconfigureThisLocationStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    LocationId         thisNodeLocationId           = FrameworkToolKit::getThisLocationId ();
    string             thisNodeIpAddress            = FrameworkToolKit::getThisLocationIpAddress();
    UI32               thisNodePort                 = FrameworkToolKit::getThisLocationPort();

    ClusterLocalReportRemovedNodeFromClusterMessage message(thisNodeIpAddress, thisNodePort, thisNodeLocationId);

    //Sending the message to Local Cluster Service
    WaveMessageStatus status = sendSynchronously (&message, thisNodeLocationId);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::unconfigureClusterSecondaryUnconfigureThisLocationStep : Local Cluster failed to process report indicting that the node is removed from the cluster.  Proceeding anyway. Status = ") + status);
    }
    else
    {
        ResourceId completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::unconfigureClusterSecondaryUnconfigureThisLocationStep : Local Cluster succeeded to process report indicting that the node is removed from the cluster.");
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::unconfigureClusterSecondaryUnconfigureThisLocationStep : Local Cluster failed to process report indicting that the node is removed from the cluster.  Proceeding anyway. Completion Status = ") + completionStatus);
        }
    }

    pDeleteNodeOnSecondaryContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverStep (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverStep : Entering ...");
    
    WaveNode                        *pNode = NULL;
    set<LocationId>                 locationIdSet;
    set<LocationId>::iterator       it;
    UI32                            i = 0; 
    
    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverCallback), pDeleteNodeOnSecondaryContext);

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
    prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

    vector<WaveManagedObject *> *pResults = querySynchronously (WaveNode::getClassName ());
    prismAssert (NULL != pResults, __FILE__, __LINE__);    

    UI32 totalWaveNodes = pResults->size ();
    vector<LocationId> locationIdVector    = pDeleteNodeOnSecondaryContext->getKnownLocationId();
    UI32               noOfFailedLocations = locationIdVector.size ();

    if (totalWaveNodes != noOfFailedLocations)
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverStep: Total wave nodes does not match with the known locations");
    }

    if (1 > totalWaveNodes)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverStep: Wave node does not have itself in the database");
        prismAssert (false, __FILE__, __LINE__);
    }

    //Perform a union of locationId from known location and wave node
    //set will not allow duplicate entries, so it is ok to insert the same locationId twice
    for (i = 0; i < noOfFailedLocations; i++)
    {
        locationIdSet.insert (locationIdVector[i]);
    }

    for (i = 0; i < totalWaveNodes; i++)
    {
        pNode       = dynamic_cast<WaveNode *> ((*pResults)[i]);

        locationIdSet.insert (pNode->getLocationId ());
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);

    LocationId locationId = 0;
    for (it = locationIdSet.begin (); it != locationIdSet.end (); it++)
    {
        locationId = *it;

        if (locationId != FrameworkToolKit::getThisLocationId ())
        {
            pPrismFrameworkFailoverWorkerContext->addFailedLocationId (locationId);
        }
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);

    pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_SECONDARY);
    pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);

    m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);

}

void PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::unconfigureClusterSecondaryRunFailoverCallback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext = reinterpret_cast<DeleteNodeOnSecondaryContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId                   status                       = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pDeleteNodeOnSecondaryContext, __FILE__, __LINE__);

    pDeleteNodeOnSecondaryContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::destroyClusterMessageHandler (FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage)
{
    DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext = new DestroyClusterAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::destroyClusterMessageHandlerCallback), pFrameworkObjectManagerDestroyClusterMessage);

    pDestroyClusterAsynchronousContext->setReasonForDestroyingCluster (pFrameworkObjectManagerDestroyClusterMessage->getReasonForDestroyingCluster ());
    pDestroyClusterAsynchronousContext->setOriginalRequester          (pFrameworkObjectManagerDestroyClusterMessage->getOriginalRequester ());
    pDestroyClusterAsynchronousContext->setIsRebootRequired           (pFrameworkObjectManagerDestroyClusterMessage->getIsRebootRequired ());

    destroyClusterAsynchronousHandler (pDestroyClusterAsynchronousContext);
}

void PrismFrameworkObjectManager::destroyClusterMessageHandlerCallback (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext)
{
    prismAssert (NULL !=  pDestroyClusterAsynchronousContext, __FILE__, __LINE__);

    FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage = reinterpret_cast<FrameworkObjectManagerDestroyClusterMessage *> (pDestroyClusterAsynchronousContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkObjectManagerDestroyClusterMessage, __FILE__, __LINE__);

    pFrameworkObjectManagerDestroyClusterMessage->setCompletionStatus (pDestroyClusterAsynchronousContext->getCompletionStatus ());

    delete pDestroyClusterAsynchronousContext;

    reply (pFrameworkObjectManagerDestroyClusterMessage);
}

void PrismFrameworkObjectManager::destroyClusterAsynchronousHandler (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterGetKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterUnconfigureKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterDeleteHeartBeatToKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterTerminateClientConnectionsForKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterRemoveKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterUnconfigureSelfStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterRunFailoverStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::destroyClusterBroadcastDeleteClusterEventStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    DestroyClusterContext *pDestroyClusterContext = new DestroyClusterContext (pDestroyClusterAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDestroyClusterContext->setReasonForDestroyingCluster       (pDestroyClusterAsynchronousContext->getReasonForDestroyingCluster ());
    pDestroyClusterContext->setIsRebootRequired                 (pDestroyClusterAsynchronousContext->getIsRebootRequired ());

    pDestroyClusterContext->holdAll ();
    pDestroyClusterContext->start ();
}

void PrismFrameworkObjectManager::destroyClusterValidateStep (DestroyClusterContext *pDestroyClusterContext)
{
    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    pDestroyClusterContext->setThisLocationRoleBeforeDestroy (locationRole);

    if (LOCATION_PRIMARY != locationRole)
    {
        if (LOCATION_STAND_ALONE == locationRole)
        {
            pDestroyClusterContext->executeNextStep (WAVE_CLUSTER_DELETE_STATUS_CLUSTER_NOT_FOUND);
        }
        else
        {
            pDestroyClusterContext->executeNextStep (FRAMEWORK_ERROR_ONLY_PRIMARY_LOCATION_CAN_DESTROY_CLUSTER);
        }
    }
    else
    {
        pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::destroyClusterGetKnownLocationsStep (DestroyClusterContext *pDestroyClusterContext)
{
    vector<LocationId> remoteLocationIdsVector;
    UI32               numberOfRemoteLocationIds;
    UI32               i;

    FrameworkToolKit::getKnownRemoteLocations (remoteLocationIdsVector);
    numberOfRemoteLocationIds = remoteLocationIdsVector.size ();

    for (i = 0; i < numberOfRemoteLocationIds; i++)
    {
        pDestroyClusterContext->addLocationId (remoteLocationIdsVector[i]);
    }

    pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::destroyClusterUnconfigureKnownLocationsStep (DestroyClusterContext *pDestroyClusterContext)
{
    UI32             numberOfLocationIds = pDestroyClusterContext->getNumberOfLocationIds ();
    LocationId       locationId          = 0;
    UI32             i                   = 0;
    WaveMessageStatus messageStatus       = WAVE_MESSAGE_SUCCESS;
    string           ipAddress           = "";
    SI32             port                = 0 ;

    ++(*pDestroyClusterContext);

    for (i = 0; i < numberOfLocationIds; i++)
    {
        locationId = pDestroyClusterContext->getLocationIdAt (i);
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId (locationId);

        FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);

        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        messageStatus = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::destroyClusterUnconfigureKnownLocationsCallback), pDestroyClusterContext, 10000, locationId);

        if (WAVE_MESSAGE_SUCCESS == messageStatus)
        {
            ++(*pDestroyClusterContext);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::destroyClusterUnconfigureKnownLocationsStep : Could not send a message to unconfigure a known location : ") + locationId + string (" : Status : ") + messageStatus);
            delete pMessage;
        }
    }

    --(*pDestroyClusterContext);
    pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

}

void PrismFrameworkObjectManager::destroyClusterUnconfigureKnownLocationsCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pFrameworkObjectManagerUnconfigureClusterSecondaryMessage, void *pContext)
{
    DestroyClusterContext *pDestroyClusterContext = reinterpret_cast<DestroyClusterContext *> (pContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        ResourceId completionStatus = pFrameworkObjectManagerUnconfigureClusterSecondaryMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_WARN, string ("PrismFrameworkObjectManager::destroyClusterUnconfigureKnownLocationsCallback : Error in unconfiguring a location. Completion Status : ") + completionStatus);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::destroyClusterUnconfigureKnownLocationsCallback : Error in unconfiguring a location. Framework Status : ") + frameworkStatus);
    }

    if (NULL != pFrameworkObjectManagerUnconfigureClusterSecondaryMessage)
    {
        delete pFrameworkObjectManagerUnconfigureClusterSecondaryMessage;
    }

    --(*pDestroyClusterContext);
    pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::destroyClusterDeleteHeartBeatToKnownLocationsStep (DestroyClusterContext *pDestroyClusterContext)
{
    UI32        numberOfLocationIds = pDestroyClusterContext->getNumberOfLocationIds ();
    LocationId  locationId          = 0;
    UI32        i                   = 0;
    string      ipAddress           = "";
    SI32        port                = 0;
    ResourceId  status              = WAVE_MESSAGE_SUCCESS;
    LocationId  thisLocationId      = FrameworkToolKit::getThisLocationId ();

    for (i = 0; i < numberOfLocationIds; i++)
    {
        locationId = pDestroyClusterContext->getLocationIdAt     (i);
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId      (locationId);

        StopHeartBeatMessage stopHeartBeatMessage;
        IpV4Address          ip (ipAddress);

        stopHeartBeatMessage.setDstIpAddress       (ip);
        stopHeartBeatMessage.setDstPortNumber      (port);

        status = sendSynchronously (&stopHeartBeatMessage, thisLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::destroyClusterDeleteHeartBeatToKnownLocationsStep : Could not send a message to stop heart beat message for " + ipAddress + ":" + port + ", Error = " + FrameworkToolKit::localize (status));
        }

        status = stopHeartBeatMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::destroyClusterDeleteHeartBeatToKnownLocationsStep : Could not stop Heart Beat to " + ipAddress + ":" + port + ", Error = " + FrameworkToolKit::localize (status));
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::destroyClusterDeleteHeartBeatToKnownLocationsStep :  stopped Heart Beat to " + ipAddress + ":" + port);
        }
    }

    pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::destroyClusterTerminateClientConnectionsForKnownLocationsStep (DestroyClusterContext *pDestroyClusterContext)
{
    UI32       numberOfLocationIds      = pDestroyClusterContext->getNumberOfLocationIds ();
    LocationId locationId               = 0;
    UI32       i                        = 0;
    string     ipAddress                = "";
    SI32       port                     = 0;
    string     clusterPrimaryIpAddress  = "";
    SI32       clusterPrimaryPort       = -1;
    bool       isPreparingForAddNode    = false;

    DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext = reinterpret_cast<DestroyClusterAsynchronousContext *> (pDestroyClusterContext->getPPrismAsynchronousContext ());

    isPreparingForAddNode = pDestroyClusterAsynchronousContext->getIsPreparingForAddNode ();

    if (true == isPreparingForAddNode)
    {
        SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pDestroyClusterAsynchronousContext->getPCallerContext ()); 

        PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());

        clusterPrimaryIpAddress = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryIpAddress ();
        clusterPrimaryPort      = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryPort ();
    }

    for (i = 0; i < numberOfLocationIds; i++)
    {
        locationId = pDestroyClusterContext->getLocationIdAt (i);
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId (locationId);

        if (false == isPreparingForAddNode || (clusterPrimaryIpAddress != ipAddress && clusterPrimaryPort != port))
        {
            InterLocationMessageReceiverObjectManager::terminateAndRemoveInterLocationMessageReceiverThreadFromCache (ipAddress, port);
        }
    }

    pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::destroyClusterRemoveKnownLocationsStep (DestroyClusterContext *pDestroyClusterContext)
{
    UI32       numberOfLocationIds      = pDestroyClusterContext->getNumberOfLocationIds ();
    LocationId locationId               = 0;
    UI32       i                        = 0;
    string     ipAddress                = "";
    string     clusterPrimaryIpAddress  = "";
    SI32       clusterPrimaryPort       = -1;
    bool       isPreparingForAddNode    = false;

    DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext = reinterpret_cast<DestroyClusterAsynchronousContext *> (pDestroyClusterContext->getPPrismAsynchronousContext ()); 

    isPreparingForAddNode = pDestroyClusterAsynchronousContext->getIsPreparingForAddNode ();

    if (true == isPreparingForAddNode)
    {    
        SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pDestroyClusterAsynchronousContext->getPCallerContext ()); 

        PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage = reinterpret_cast<PrismConfigureClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ()); 

        clusterPrimaryIpAddress = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryIpAddress ();
        clusterPrimaryPort      = pPrismConfigureClusterSecondaryMessage->getClusterPrimaryPort ();
    }    

    for (i = 0; i < numberOfLocationIds; i++)
    {
        locationId = pDestroyClusterContext->getLocationIdAt (i);
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        SI32 port  = FrameworkToolKit::getPortForLocationId (locationId);

        if (false == isPreparingForAddNode || (clusterPrimaryIpAddress != ipAddress && clusterPrimaryPort != port))
        {
           disconnectFromLocation (locationId);
        }

        removeKnownLocation (locationId);
    }

    pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::destroyClusterUnconfigureSelfStep (DestroyClusterContext *pDestroyClusterContext)
{
    LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();

    m_pThisLocation->resetLocationToStandAlone (thisLocationId);

    pDestroyClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::destroyClusterRunFailoverStep (DestroyClusterContext *pDestroyClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::destroyClusterRunFailoverStep : Entering ...");

    DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext = reinterpret_cast<DestroyClusterAsynchronousContext *> (pDestroyClusterContext->getPPrismAsynchronousContext ());

    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::destroyClusterRunFailoverCallback), pDestroyClusterContext);

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
    prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

    UI32 numberOfLocationIds = pDestroyClusterContext->getNumberOfLocationIds ();
    UI32 i                   = 0;
    LocationId locationId;


    for (i = 0; i < numberOfLocationIds; i++)
    {
        locationId = pDestroyClusterContext->getLocationIdAt (i);
        pPrismFrameworkFailoverWorkerContext->addFailedLocationId (locationId);
    }

    pPrismFrameworkFailoverWorkerContext->setThisLocationRole (pDestroyClusterContext->getThisLocationRoleBeforeDestroy ());
    pPrismFrameworkFailoverWorkerContext->setFailoverReason (pDestroyClusterContext->getReasonForDestroyingCluster ());
    pPrismFrameworkFailoverWorkerContext->setServiceToBeIgnored (pDestroyClusterAsynchronousContext->getOriginalRequester ());

    m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
}

void PrismFrameworkObjectManager::destroyClusterRunFailoverCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::destroyClusterRunFailoverCallback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    DestroyClusterContext *pDestroyClusterContext = reinterpret_cast<DestroyClusterContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId             status                 = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pDestroyClusterContext, __FILE__, __LINE__);

    pDestroyClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::destroyClusterBroadcastDeleteClusterEventStep (DestroyClusterContext *pDestroyClusterContext)
{
    trace(TRACE_LEVEL_INFO, "Entering PrismFrameworkObjectManager::destroyClusterBroadcastDeleteClusterEventStep");

    WaveNodeDeleteClusterEvent* pWaveNodeDeleteClusterEvent = new WaveNodeDeleteClusterEvent ();

    pWaveNodeDeleteClusterEvent->setIsRebootRequired (pDestroyClusterContext->getIsRebootRequired ());

    ResourceId status =  broadcast(pWaveNodeDeleteClusterEvent);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
       trace(TRACE_LEVEL_ERROR, "Entering PrismFrameworkObjectManager::destroyClusterBroadcastDeleteClusterEventStep Broadcast Failed");
    }

    pDestroyClusterContext->executeNextStep (status);

}

/// Name
/// rejoinNodesToClusterMessageHandler
/// Description
/// This function lays down the steps for procssing (within the f/w) the Rejoin 
/// request received from the Cluster service on the Primary. It also creates the context 
/// and starts the processing.
///  Input
/// FrameworkObjectManagerRejoinNodesToClusterMessage: Pointer to message
/// received from the Global Cluster service 
/// Output
/// none
/// Return
/// none
void PrismFrameworkObjectManager::rejoinNodesToClusterMessageHandler (FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinNodesToClusterValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesCollectValidationDataStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesPausePersistenceStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinNodesToClusterConnectToNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1Step),


        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesResumeDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase2Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesSendValidationResultsStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::createClusterWithNodesSendListOfSecondariesToAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastNewNodesAddedEventOnPrimaryStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::sendReplyBackToClusterGlobalService),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase2),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startHeartBeatToSecondaryNodes),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastNodesAdditionToClusterCompletedEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterSuccessStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterFailureStep),
    };

    //This memory is freed within the Framework as part of prsimLinearSequencerSucceeded or
    //prismLinearSequencerFailedStep
    RejoinNodesToClusterContext *pRejoinNodesToClusterContext = new RejoinNodesToClusterContext (pFrameworkObjectManagerRejoinNodesToClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    // Set an indication that cluster operation is in progress on primary node
    setPrimaryNodeClusterOperationFlag (true);

    // The flag is set here to avoid the default setting in base cluster context creation
    // which causes the previous state of the flag to be lost upon controlled failover.
    setSecondaryNodeClusterCreationFlag (false);
    
    pRejoinNodesToClusterContext->holdAll ();
    pRejoinNodesToClusterContext->start ();
}

/// Name
/// rejoinNodesToClusterValidateStep
/// Description
/// This function validates the rejoin Node request received from the cluster.
/// It essentially looks at every node in the request and adds it to the context 
/// only if it is a know location
///  Input
/// RejoinNodesToClusterContext:pointer for  context for rejoin created in
/// in the handler
/// Output
/// none
/// Return
/// none
/// 
void PrismFrameworkObjectManager::rejoinNodesToClusterValidateStep (RejoinNodesToClusterContext *pRejoinNodesToClusterContext)
{
    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

    prismAssert(NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage, __FILE__, __LINE__);

    UI32                                               numberOfNodes                                      = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNumberOfNodes (); 
    UI32                                               numberOfFailures                                   = 0;

    //The location of all nodes in the message should already be known to the primary
    // as the REjoining node was part of the cluster earlier
    for (UI32 i = 0; i < numberOfNodes; i++)
    {
        string ipAddress = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeAt (i);
        UI32 port = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodePortAt (i);

        if (true == (FrameworkToolKit::isAKnownLocation (ipAddress, port)))
        {
            pRejoinNodesToClusterContext->addLocationId (FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress, port));
        }
        else
        {
            if (true == pFrameworkObjectManagerRejoinNodesToClusterMessage->getIsReplaceRejoin ())
            {
                LocationId replacedLocationId = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeLocationId (i);

                // Remove the old binding of location to IP address
                removeKnownLocation (replacedLocationId);

                // Recreate binding of same location to IP address
                m_pThisLocation->addKnownLocation (replacedLocationId, ipAddress, port);
                pRejoinNodesToClusterContext->addLocationId (replacedLocationId);

                #if 0
                // Create a new location with new IP and port
                LocationId newLocationId = getNextLocationId ();

                if (0 != newLocationId)
                {
                    m_pThisLocation->addKnownLocation (newLocationId, ipAddress, port);
                    pRejoinNodesToClusterContext->addLocationId (newLocationId);
                }
                else
                {
                    pFrameworkObjectManagerRejoinNodesToClusterMessage->setNodeStatus (ipAddress, port, FRAMEWORK_ERROR_NO_UNUSED_LOCATION_IDS_AVAILABLE);
                    numberOfFailures++;
                }
                #endif
            }
            else
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->setNodeStatus (ipAddress, port, FRAMEWORK_ERROR_CANNOT_REJOIN_NODE_TO_CLUSTER_NOT_A_KNOWN_LOCATION);
                numberOfFailures++;
            }
        }
    }

    //If there is no known  node in the request signal a failure. Even if one node is valid we will
    //process the request
    if (numberOfFailures == numberOfNodes)
    {
        pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_REJOIN_NODES_TO_CLUSTER_NO_VALID_RESOURCES);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

    return;
}


/// Name
/// rejoinNodesToClusterConnectToNodesStep
/// Description
/// This function establishes a connection with every known location in the
/// RejoinContext
///  Input
/// RejoinNodesToClusterContext:pointer for  context for rejoin created in
/// in the handler
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinNodesToClusterConnectToNodesStep (RejoinNodesToClusterContext *pRejoinNodesToClusterContext)
{
    static const UI32  maximumNumberOfRetriesToConnectToARemoteLocation   = 30;
    static const UI32  maximumNumberOfSecondsToWaitToConnect              = 30;

    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

    prismAssert(NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage, __FILE__, __LINE__);

    UI32  numberOfLocationIds = pRejoinNodesToClusterContext->getNumberOfLocationIds ();    
    UI32  numberOfFailures    = 0;

    for (UI32 i = 0; i< numberOfLocationIds; ++i)
    {
        LocationId locationId = pRejoinNodesToClusterContext->getLocationIdAt (i);

        prismAssert (0 != locationId, __FILE__, __LINE__);

        string  ipAddress   = FrameworkToolKit::getIpAddressForLocationId (locationId);
        UI32    port        = FrameworkToolKit::getPortForLocationId (locationId);

        if (false == pFrameworkObjectManagerRejoinNodesToClusterMessage->isNewNodeStatusSet (ipAddress, port))
        {
            ResourceId status = m_pThisLocation->connectToRemoteLocation (locationId, maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect);

            if (FRAMEWORK_SUCCESS != status)
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->setNodeStatus (ipAddress, port, status);
                numberOfFailures++;
            }
        }
        else
        {
            string traceString = string ("PrismFrameworkObjectManager::rejoinNodesToClusterConnectToNodesStep : locationId : ") + locationId + string ("already set with error status :") + FrameworkToolKit::localize (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress, port));
            trace (TRACE_LEVEL_INFO, traceString);
            numberOfFailures++;
        }
    }

    if (numberOfFailures == numberOfLocationIds)
    {
        resumePersistence ();

        pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONECT_TO_ALL_NEW_LOCATIONS);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}


/// Name
/// rejoinNodesToClusterRejoinClusterSecondariesStep
/// Description
/// This function creates a rejoin message and sends it to the wave framework on 
/// every secondary node in the rejoin message. Along with the rejon message
/// it also sends the validation data for each service and the primary Database.
///  Input
/// RejoinNodesToClusterContext:pointer for  context for rejoin created in
/// in the handler
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesStep (RejoinNodesToClusterContext *pRejoinNodesToClusterContext)
{
    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());
 
    prismAssert (NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage, __FILE__, __LINE__);

    WaveMessageStatus status  = WAVE_MESSAGE_ERROR;  
    string prismVersionString = PrismVersion::getVersionString ();

    ++(*pRejoinNodesToClusterContext);

    UI32        numberOfLocationIds     = pRejoinNodesToClusterContext->getNumberOfLocationIds ();
    string      thisLocationIpAddress   = FrameworkToolKit::getThisLocationIpAddress ();
    SI32        thisLocationPort        = FrameworkToolKit::getThisLocationPort ();
    LocationId  thisLocationId          = FrameworkToolKit::getThisLocationId ();
    UI32        numberOfFailures        = 0;

    // Add all the validation buffers to the message before sending out
    vector<PrismServiceId> &prismServiceIds                = pRejoinNodesToClusterContext->getPrismServiceIdsVector ();
    vector<void *>         &validationDetailsVector        = pRejoinNodesToClusterContext->getValidationDetailsVector ();
    vector<UI32>           &validationDetailsSizesVector   = pRejoinNodesToClusterContext->getValidationDetailsSizesVector ();
    UI32                    numberOfPrismServiceIds        = prismServiceIds.size ();
    UI32                    numberOfValidationDetails      = validationDetailsVector.size ();
    UI32                    numberOfValidationDetailsSizes = validationDetailsSizesVector.size ();

    prismAssert (numberOfPrismServiceIds == numberOfValidationDetails, __FILE__, __LINE__);
    prismAssert (numberOfPrismServiceIds == numberOfValidationDetailsSizes, __FILE__, __LINE__);

    //Sending the request to each secondary location
    for (UI32 i = 0; i < numberOfLocationIds; i++)
    {
        LocationId locationId = pRejoinNodesToClusterContext->getNewLocationIdAt ((UI32) i);
        string ipAddress     = FrameworkToolKit::getIpAddressForLocationId (locationId);
        SI32 port          = FrameworkToolKit::getPortForLocationId (locationId);

        // Rejoin messages sent only locations known to the primary. 

        if (true == (FrameworkToolKit::isAKnownLocation (ipAddress, port)) && (false == pFrameworkObjectManagerRejoinNodesToClusterMessage->isNewNodeStatusSet (ipAddress, port)))
        {
            FrameworkObjectManagerRejoinClusterSecondaryMessage *pMessage = new FrameworkObjectManagerRejoinClusterSecondaryMessage (prismVersionString, thisLocationIpAddress, thisLocationPort, thisLocationId, ipAddress, port, locationId);

            pMessage->setSchemaChangeVectors ();

            pMessage->setIsReplaceRejoin (pFrameworkObjectManagerRejoinNodesToClusterMessage->getIsReplaceRejoin ());

            if (true == FrameworkToolKit::isConfigurationCompatibilityCheckRequired ())
            {
                UI64      configurationTransactionId  = 0;
                DateTime  configurationTime;
                DateTime  latestUpdateTime;
                
                PersistenceToolKit::getConfigurationManagedObjectInformation (configurationTransactionId, configurationTime, latestUpdateTime); 

                vector<UI64> configurationNumber;

                configurationNumber.push_back (configurationTransactionId);

                pMessage->setConfigurationNumber (configurationNumber);
            }

            //For each location go through each serviceId and attach the validation data for each service 
            //to the message
            for (UI32 j = 0; j < numberOfPrismServiceIds; j++)
            {
                if ((0 != validationDetailsSizesVector[j]) && (NULL != validationDetailsVector[j]))
                {
                    pMessage->addBuffer (prismServiceIds[j], validationDetailsSizesVector[j], validationDetailsVector[j]);
                }
                else
                {
                    trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesStep : We could not have added an entry with 0 size and/or NULL validation buffer.");
                    prismAssert (false, __FILE__, __LINE__);
                }
            }

            pMessage->setToAllowSendForOneWayCommunication (true);

            //send the message and lodge a callback. Note that the processing is Asynchronous here
            status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondaryCallback), pRejoinNodesToClusterContext, s_clusterPhase0TimeoutInMilliseconds, locationId);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                //QUESTION: ASK SAGAR:No need to remove location
                pFrameworkObjectManagerRejoinNodesToClusterMessage->setNewNodeStatus (ipAddress, port, status);
                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesStep: Not able to configure one secondary location : ") + locationId + string (" Error Code : ") + status);
                numberOfFailures++;
                delete pMessage;
            }
            else
            {
                 ++(*pRejoinNodesToClusterContext);
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    backUpDatabaseAfterSendingClusterPhase0Message ();

    --(*pRejoinNodesToClusterContext);

    //If there is a failure in sending a message to all locations
    //then mark a failure.
    if (numberOfFailures == numberOfLocationIds)
    {
        resumeDatabaseOperation ();
        
        pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}
/// Name
/// rejoinNodesToClusterRejoinClusterSecondaryCallback
/// Description
/// When the framework receives a reply from the secondary
/// it calls this callback.
///  Input
/// FrameworkStatus: success/failure in framework
/// FrameworkObjectManagerRejoinClusterSecondaryMessage: pointer to the message received back from the 
/// secondary in reply
/// RejoinNodesToClusterContext:pointer for  context for rejoin created in
/// the handler
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondaryCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage, void *pContext)
{
    RejoinNodesToClusterContext  *pRejoinNodesToClusterContext = reinterpret_cast<RejoinNodesToClusterContext *> (pContext);

    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

    prismAssert(NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage,__FILE__,__LINE__);

    tracePrintf(TRACE_LEVEL_DEVEL,"PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondaryCallback:FrameworkStatus received:0x%x",frameworkStatus);
  
    --(*pRejoinNodesToClusterContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage, __FILE__, __LINE__);

        LocationId newLocationId    = pFrameworkObjectManagerRejoinClusterSecondaryMessage->getLocationId ();
        string     ipAddress        = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        SI32       port             = FrameworkToolKit::getPortForLocationId (newLocationId);
        ResourceId completionStatus = pFrameworkObjectManagerRejoinClusterSecondaryMessage->getCompletionStatus ();

        pFrameworkObjectManagerRejoinNodesToClusterMessage->setNodeStatus (ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS == completionStatus) 
        {
            tracePrintf(TRACE_LEVEL_DEVEL,"PrismFrameworkObjectManager:rejoinNodesToClusterRejoinClusterSecondaryCallback:Wave Success received for Location Id %d",newLocationId);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager:rejoinNodesToClusterRejoinClusterSecondaryCallback, not able rejoin node ") + newLocationId + string (" failure reason is ") + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {   
       trace(TRACE_LEVEL_DEVEL,"PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondaryCallback:Framework Failure received");

        //Note that a NULL message pointer can be received in case of a timeout. 
        if (NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage)
        {
           LocationId newLocationId    = pFrameworkObjectManagerRejoinClusterSecondaryMessage->getLocationId ();
           string     ipAddress        = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
           SI32       port             = FrameworkToolKit::getPortForLocationId (newLocationId);

           pFrameworkObjectManagerRejoinNodesToClusterMessage->setNodeStatus (ipAddress, port, frameworkStatus);

           tracePrintf(TRACE_LEVEL_DEVEL,"PrismFrameworkObjectManager:rejoinNodesToClusterRejoinClusterSecondaryCallback:Framework Failure received for Location Id %d",newLocationId);
        }
    }

    //Release memory of the secondary message
    if (NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage)
    {
        delete pFrameworkObjectManagerRejoinClusterSecondaryMessage;
    }

    //If responses have been received from each secondary  then we are ready to evaluate 
    //whether the operation succeeded or not
    if (0 == (pRejoinNodesToClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        UI32 numberOfIpAddresses = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNumberOfNodes (); 
        UI32 numberOfFailures = 0;

        //Loop thru all the nodes  and count the number of failures
        for (UI32 i = 0; i < numberOfIpAddresses; i++)
        {
            string ipAddress = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeAt     ((UI32) i);
            SI32   port      = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodePortAt ((UI32) i);

            if (true != (pFrameworkObjectManagerRejoinNodesToClusterMessage->isNodeStatusSet (ipAddress, port)))
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->setNodeStatus (ipAddress, port, FRAMEWORK_ERROR_REJOIN_NODES_TO_CLUSTER_REJOIN_SECONDARY_TIMED_OUT);
                disconnectFromLocation(ipAddress, port, true);

                numberOfFailures++;
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeStatus (ipAddress, port)))
                {
                    disconnectFromLocation(ipAddress, port, true);
                    numberOfFailures++;
                }
            }
        }

        if (numberOfFailures == numberOfIpAddresses)
        {
            resumeDatabaseOperation ();
            
            pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_REJOIN_ALL_LOCATIONS);
        }
        else
        {
            pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }

}

void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1Step (RejoinNodesToClusterContext *pRejoinNodesToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterecondariesPhase1Step : Entering ...");

    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

    prismAssert (NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage, __FILE__, __LINE__);

    SI32   sizeOfBackupFile                    = 0;
    char  *pDatabaseBackupBuffer               = NULL;
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;
    UI32                                numberOfFailures                    = 0;
    UI32                                numberOfNewIpAddresses              = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNumberOfNodes ();

    ++(*pRejoinNodesToClusterContext);

    createDatabaseBackupBuffer(pDatabaseBackupBuffer, sizeOfBackupFile);

    for (UI32 i = 0; i < numberOfNewIpAddresses; i++)
    {
        string ipAddress1         = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeAt ((UI32) i);
        SI32 port1                = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodePortAt ((UI32) i);
        LocationId newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

        // We will send  phase1 message only to the locations that are known to us at this point.
        // Only to those locations that are succeeded as part of phase 0.

        if (WAVE_MESSAGE_SUCCESS == (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1)))
        {
            PrismRejoinClusterSecondaryPhase1Message *pMessage = new PrismRejoinClusterSecondaryPhase1Message ();

            if (NULL != pDatabaseBackupBuffer)
            {
                pMessage->addBuffer (pMessage->getDatabaseBackupBufferTag (), sizeOfBackupFile, pDatabaseBackupBuffer, false);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1Step : No Database Backup is being sent to the lcoation.");
            }
   
            status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1StepCallback), pRejoinNodesToClusterContext, s_clusterPhase1TimeoutInMilliseconds, newLocationId1);
    
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, status);

                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1Step : Not able to configure one secondary location During Phase 1: ") + newLocationId1 + string (" Status : ") + FrameworkToolKit::localize (status));

                numberOfFailures++;
                delete pMessage;
            }
            else
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE1_UNKNOWN);

                ++(*pRejoinNodesToClusterContext);
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    --(*pRejoinNodesToClusterContext);

    if (NULL != pDatabaseBackupBuffer)
    {
        delete[] pDatabaseBackupBuffer;
    }

    if ( numberOfFailures == numberOfNewIpAddresses)
    {
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1Step : numberOfFailures - ") + numberOfFailures );
        resumeDatabaseOperation ();
        
        pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}


void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1StepCallback(FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryPhase1Message *pPrismRejoinClusterSecondaryPhase1Message, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager:: rejoinNodesToClusterRejoinClusterSecondariesPhase1StepCallback: Starting ...");

    RejoinNodesToClusterContext *pRejoinNodesToClusterContext = reinterpret_cast<RejoinNodesToClusterContext *> (pContext);

    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

    prismAssert(NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage,__FILE__,__LINE__);

    --(*pRejoinNodesToClusterContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismRejoinClusterSecondaryPhase1Message, __FILE__, __LINE__);

        LocationId  newLocationId = pPrismRejoinClusterSecondaryPhase1Message->getReceiverLocationId ();
        string      ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        SI32        port          = FrameworkToolKit::getPortForLocationId (newLocationId);

        ResourceId completionStatus = pPrismRejoinClusterSecondaryPhase1Message->getCompletionStatus ();

        pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus(ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1StepCallback : Case 1 Configuring ") + ipAddress + " failed : " + completionStatus);
        }
    }
    else
    {
        if (NULL != pPrismRejoinClusterSecondaryPhase1Message)
        {
            LocationId newLocationId1 = pPrismRejoinClusterSecondaryPhase1Message->getReceiverLocationId();
            string     ipAddress1     = FrameworkToolKit::getIpAddressForLocationId (newLocationId1);
            SI32       port1          = FrameworkToolKit::getPortForLocationId (newLocationId1);

            pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, frameworkStatus);

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase1StepCallback : Case 2 Configuring ") + ipAddress1 + " failed : " + frameworkStatus);
        }
    }

    if (NULL != pPrismRejoinClusterSecondaryPhase1Message)
    {
        delete pPrismRejoinClusterSecondaryPhase1Message;
    }

    if (0 == (pRejoinNodesToClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        UI32       numberOfNewIpAddresses  = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNumberOfNewNodes ();
        UI32       i                       = 0;
        LocationId newLocationId1          = 0;
        string     ipAddress1              = "";
        SI32       port1                   = 0;
        UI32       numberOfFailures        = 0;

        for (i = 0; i < numberOfNewIpAddresses; i++)
        {
            ipAddress1     = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeAt      ((UI32) i);
            port1          = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodePortAt  ((UI32) i);
            newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

            if (FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE1_UNKNOWN == (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1)))
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE1_TIMED_OUT);

                if (0 != newLocationId1)
                {
                    trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager:: rejoinNodesToClusterRejoinClusterSecondariesPhase1StepCallback : Case 3 - Configuring ") + ipAddress1 + " Timeout ");
                }

                numberOfFailures++;
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1)))
                {
                    numberOfFailures++;
                }
            }
        }
            
        if (numberOfFailures == numberOfNewIpAddresses)
        {
            resumeDatabaseOperation ();
            
            pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
        }
        else
        {
            pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    } 
}

/// Name
/// rejoinNodesToClusterRejoinClusterSecondariesPhase2Step
/// Description
/// When the framework receives a reply from each secondary
/// the it calls this method. This method sends a message to
/// each secondary to start the phase2 which involves restarting
/// all the services on the secondary.
///  Input
/// RejoinNodesToClusterContext:pointer for  context for rejoin created in
/// in the handler
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase2Step(RejoinNodesToClusterContext *pRejoinNodesToClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterecondariesPhase2Step : Entering ...");

    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

  
    prismAssert (NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage, __FILE__, __LINE__);
    UI32                                numberOfFailures                    = 0;
    UI32                                numberOfNewLocationIds              = pRejoinNodesToClusterContext->getNumberOfNewLocationIds ();

    ++(*pRejoinNodesToClusterContext);


    UI32 numberOfNewIpAddresses = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNumberOfNodes ();
    for (UI32 i = 0; i < numberOfNewIpAddresses; i++)
    {
        string ipAddress1         = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeAt ((UI32) i);
        SI32 port1                = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodePortAt ((UI32) i);
        LocationId newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

        // We will send  phase2 message only to the locations that are known to us at this point.
        // Only to those locations that are succeeded as part of phase 1.


        if (WAVE_MESSAGE_SUCCESS == (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1)))
        {
            PrismRejoinClusterSecondaryPhase2Message *pMessage = new PrismRejoinClusterSecondaryPhase2Message ();

            pMessage->setIsReplaceRejoin (pFrameworkObjectManagerRejoinNodesToClusterMessage->getIsReplaceRejoin ());

        //Note that a callback is being passed since the processing is Asynchronous
            WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase2StepCallback), pRejoinNodesToClusterContext, 120000, newLocationId1);

            if (WAVE_MESSAGE_SUCCESS != status)
            {

         #if 0
         //QUESTION: CHECK WITH SAGAR:In case of rejoin we should not remove this location
                removeKnownLocation (newLocationId1);
         #endif
                pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, status);
         
                trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase2Step : Not able to configure one secondary location During Phase 2: ") + newLocationId1 + string (" Status : ") + FrameworkToolKit::localize (status));
                numberOfFailures++;
                delete pMessage;
            }
            else
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE2_UNKNOWN);

                ++(*pRejoinNodesToClusterContext);
            }
        }
        else
        {
            numberOfFailures++;
        }
    }

    --(*pRejoinNodesToClusterContext);

    //Send for every secondary location failed
    if (numberOfFailures == numberOfNewLocationIds)
    {
        pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }


}


/// Name
/// rejoinNodesToClusterRejoinClusterSecondariesPhase2StepCallback
/// Description
/// When the framework receives a reply for the phase 2 rejoin message from
/// a secondary then it calls this callback.This method does the bookkeeping
/// of all responses. Once it has received a response from each node it decides
/// whether the entire processing was scuccessful or not.
///  Input
/// FrameworkStatus: success/failure in framework
/// PrismRejoinClusterSecondaryPhase2Message: pointer to the phase 2 message 
/// received back from the secondary in reply
/// RejoinNodesToClusterContext:pointer for  context for rejoin created in
/// the handler
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase2StepCallback(FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryPhase2Message *pPrismRejoinClusterSecondaryPhase2Message, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager:: rejoinNodesToClusterRejoinClusterSecondariesPhase2StepCallback: Starting ...");

    RejoinNodesToClusterContext                       *pRejoinNodesToClusterContext                       = reinterpret_cast<RejoinNodesToClusterContext *> (pContext);
    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

    prismAssert(NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage,__FILE__,__LINE__);

    UI32 numberOfFailures = 0;

    --(*pRejoinNodesToClusterContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismRejoinClusterSecondaryPhase2Message, __FILE__, __LINE__);

        LocationId  newLocationId = pPrismRejoinClusterSecondaryPhase2Message->getReceiverLocationId ();
        string      ipAddress     = FrameworkToolKit::getIpAddressForLocationId (newLocationId);
        SI32        port          = FrameworkToolKit::getPortForLocationId (newLocationId);       
        ResourceId completionStatus = pPrismRejoinClusterSecondaryPhase2Message->getCompletionStatus ();

        pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus(ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase2StepCallback : Configuring ") + ipAddress + " failed : " + completionStatus);     

            disconnectFromLocation (newLocationId);
            pRejoinNodesToClusterContext->addToFailedLocationIdVector (newLocationId);
        }
    }
    else
    {
        if (NULL != pPrismRejoinClusterSecondaryPhase2Message)
        {
            LocationId newLocationId1 = pPrismRejoinClusterSecondaryPhase2Message->getReceiverLocationId();
            string     ipAddress1     = FrameworkToolKit::getIpAddressForLocationId (newLocationId1);
            SI32       port1          = FrameworkToolKit::getPortForLocationId (newLocationId1);

            pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, frameworkStatus);

            disconnectFromLocation (newLocationId1);
            pRejoinNodesToClusterContext->addToFailedLocationIdVector (newLocationId1);
        }
    }

    if (NULL != pPrismRejoinClusterSecondaryPhase2Message)
    {
        delete pPrismRejoinClusterSecondaryPhase2Message;
    }

    //All responses have been received
    if (0 == (pRejoinNodesToClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        UI32       numberOfNewIpAddresses  = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNumberOfNewNodes ();
      
        for (UI32 i = 0; i < numberOfNewIpAddresses; i++)
        {
            string     ipAddress1     = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodeAt ((UI32) i);
            SI32       port1          = pFrameworkObjectManagerRejoinNodesToClusterMessage->getNodePortAt ((UI32) i);
            LocationId newLocationId1 = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress1, port1);

            if (FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE2_UNKNOWN == (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1)))
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_CREATE_CLUSTER_CONFIGURE_SECONDARY_PHASE2_TIMED_OUT);

                disconnectFromLocation (newLocationId1);
                pRejoinNodesToClusterContext->addToFailedLocationIdVector (newLocationId1);
                numberOfFailures++;
            }
            else
            {
                if (WAVE_MESSAGE_SUCCESS != (pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1)))
                {
                    numberOfFailures++;
                }
            }

            // Added the successfully added location in a vector. This is going to be used in Phase 3

            if (WAVE_MESSAGE_SUCCESS == pFrameworkObjectManagerRejoinNodesToClusterMessage->getNewNodeStatus (ipAddress1, port1))
            {
                pRejoinNodesToClusterContext->addToSuccessfullyAddedLocationIdVector (newLocationId1);
            }
        }

        if (numberOfFailures == numberOfNewIpAddresses)
        {
            vector<LocationId> failedLocationIdsVectorInPhase2           = pRejoinNodesToClusterContext->getFailedLocationIdVector ();
            UI32               numberOffailedLocationIdsVectorInPhase2   = failedLocationIdsVectorInPhase2.size ();

            if (0 < numberOffailedLocationIdsVectorInPhase2)
            {
                PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase2Callback), pRejoinNodesToClusterContext);

                for (UI32 j = 0; j < numberOffailedLocationIdsVectorInPhase2; j++)
                {
                    pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVectorInPhase2[j]);
                }
      
                pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
                pPrismFrameworkFailoverWorkerContext->setFailoverReason  (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

                tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase2StepCallback Invoking Uncontrolled failover for %u nodes", numberOffailedLocationIdsVectorInPhase2);

                m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
            }
        }   
        else
        {
            pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }

}

void PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase2 (RejoinNodesToClusterContext *pRejoinNodesToClusterContext)
{
    vector<LocationId> failedLocationIdsVector            = pRejoinNodesToClusterContext->getFailedLocationIdVector ();
    UI32               numberOffailedLocationIdsVector    = failedLocationIdsVector.size ();

    if (0 < numberOffailedLocationIdsVector)
    {
        PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase2Callback), pRejoinNodesToClusterContext);

        for (UI32 k = 0; k < numberOffailedLocationIdsVector; k++)
        {
            pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVector[k]);
        }

        pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
        pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

        tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase2 Invoking failover for %u nodes", numberOffailedLocationIdsVector);

        m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase2Callback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase2Callback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    RejoinNodesToClusterContext  *pRejoinNodesToClusterContext = reinterpret_cast<RejoinNodesToClusterContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pRejoinNodesToClusterContext, __FILE__, __LINE__);

    //clear the failed location vector setup in previous phases as failover is triggered for those locations.
    pRejoinNodesToClusterContext->clearFailedLocationIdVector ();

    vector<LocationId>      successfullyAddedLocationIdVector = pRejoinNodesToClusterContext->getSuccessfullyAddedLocationIdVector ();
                 UI32       numberOfSuccessfullyAddedNode     = successfullyAddedLocationIdVector.size ();

    if (0 == numberOfSuccessfullyAddedNode)
    {
        pRejoinNodesToClusterContext->executeNextStep (FRAMEWORK_ERROR_FAILED_TO_CONFIGURE_ALL_NEW_LOCATIONS);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3Step (RejoinNodesToClusterContext *pRejoinNodesToClusterContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3Step : Starting ...");

    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());
    vector<LocationId>                                 successfullyAddedLocationIdVector                  = pRejoinNodesToClusterContext->getSuccessfullyAddedLocationIdVector ();
    UI32                                               numberOfSuccessfullyAddedNode                      = successfullyAddedLocationIdVector.size ();
    UI32                                               numberOfFailures                                   = 0;
    LocationId                                         locationId;

    ++(*pRejoinNodesToClusterContext);

    for (UI32 i = 0; i < numberOfSuccessfullyAddedNode; i++)
    {

        locationId = successfullyAddedLocationIdVector[i];

        PrismRejoinClusterSecondaryPhase3Message *pPrismRejoinClusterSecondaryPhase3Message = new PrismRejoinClusterSecondaryPhase3Message ();

        WaveMessageStatus status = send (pPrismRejoinClusterSecondaryPhase3Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3StepCallback), pRejoinNodesToClusterContext, 0, locationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            disconnectFromLocation (locationId);
            pRejoinNodesToClusterContext->addToFailedLocationIdVector (locationId);

            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3Step : Not able to Rejoin secondary location During Phase 3: ") + locationId + string (" Status : ") + FrameworkToolKit::localize (status));

            numberOfFailures++;

            if (NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage)
            {
                pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (FrameworkToolKit::getIpAddressForLocationId (locationId), FrameworkToolKit::getPortForLocationId (locationId), status);
            }

            delete pPrismRejoinClusterSecondaryPhase3Message;            
        }
        else
        {
            ++(*pRejoinNodesToClusterContext);
        }
    }
    
    --(*pRejoinNodesToClusterContext);

    if (numberOfFailures == numberOfSuccessfullyAddedNode)
    {
        // Trigger Uncontrolled failover
        vector<LocationId> failedLocationIdsVectorInPhase3           = pRejoinNodesToClusterContext->getFailedLocationIdVector ();
        UI32               numberOffailedLocationIdsVectorInPhase3   = failedLocationIdsVectorInPhase3.size ();

        PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase3Callback), pRejoinNodesToClusterContext); 

        for (UI32 j = 0; j < numberOffailedLocationIdsVectorInPhase3; j++)
        {
            pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVectorInPhase3[j]);
        }

        pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
        pPrismFrameworkFailoverWorkerContext->setFailoverReason  (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

        tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3Step Invoking Uncontrolled failover for %u nodes", numberOffailedLocationIdsVectorInPhase3);

        m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
    }
    else
    {
        pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3StepCallback (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryPhase3Message *pPrismRejoinClusterSecondaryPhase3Message, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3StepCallback: Starting ....");

    RejoinNodesToClusterContext                       *pRejoinNodesToClusterContext                       = reinterpret_cast<RejoinNodesToClusterContext *> (pContext);
    FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage = dynamic_cast<FrameworkObjectManagerRejoinNodesToClusterMessage *> (pRejoinNodesToClusterContext->getPPrismMessage ());

    prismAssert (NULL != pPrismRejoinClusterSecondaryPhase3Message, __FILE__, __LINE__);

    LocationId                   locationId                   = pPrismRejoinClusterSecondaryPhase3Message->getReceiverLocationId ();

    --(*pRejoinNodesToClusterContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        ResourceId completionStatus = pPrismRejoinClusterSecondaryPhase3Message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3StepCallback:Failed to Rejoin Location Id ") + locationId + " status : " + completionStatus);
            disconnectFromLocation (locationId);
            pRejoinNodesToClusterContext->addToFailedLocationIdVector (locationId);            
        }

        if (NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage)
        {
            pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (FrameworkToolKit::getIpAddressForLocationId (locationId), FrameworkToolKit::getPortForLocationId (locationId), completionStatus);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3StepCallback: Failed to Rejoin Location Id ") + locationId + " status : " + frameworkStatus);

        disconnectFromLocation (locationId);
        pRejoinNodesToClusterContext->addToFailedLocationIdVector (locationId);

        if (NULL != pFrameworkObjectManagerRejoinNodesToClusterMessage)
        {
            pFrameworkObjectManagerRejoinNodesToClusterMessage->updateNewNodeStatus (FrameworkToolKit::getIpAddressForLocationId (locationId), FrameworkToolKit::getPortForLocationId (locationId), frameworkStatus);
        }
    }
    

    if (0 == (pRejoinNodesToClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        vector<LocationId> failedLocationIdsVectorInPhase3           = pRejoinNodesToClusterContext->getFailedLocationIdVector ();
        UI32               numberOffailedLocationIdsVectorInPhase3   = failedLocationIdsVectorInPhase3.size ();

        if (0 < numberOffailedLocationIdsVectorInPhase3)
        {
            PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase3Callback), pRejoinNodesToClusterContext);

            for (UI32 j = 0; j < numberOffailedLocationIdsVectorInPhase3; j++)
            {
                pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIdsVectorInPhase3[j]);
            }
            
            pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
            pPrismFrameworkFailoverWorkerContext->setFailoverReason  (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

            tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinNodesToClusterRejoinClusterSecondariesPhase3StepCallback Invoking Uncontrolled failover for %u nodes", numberOffailedLocationIdsVectorInPhase3);

            m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
        }
        else
        {
            pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    } 

    delete pPrismRejoinClusterSecondaryPhase3Message;
}

void PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase3Callback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::failoverforNodesFailedInRejoinPhase3Callback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    RejoinNodesToClusterContext  *pRejoinNodesToClusterContext = reinterpret_cast<RejoinNodesToClusterContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pRejoinNodesToClusterContext, __FILE__, __LINE__);

    pRejoinNodesToClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

/// Name
/// rejoinClusterSecondaryMessageHandler
/// Description
/// This handler processes the rejoin request received on the secondary node frameowrk
/// from the primary node framework.It establishes the steps required to process the message
/// and lodges them in the context
///  Input
/// FrameworkObjectManagerRejoinClusterSecondaryMessage:pointer to message
/// received from the primary
/// Output
/// none
/// Return
/// none
void PrismFrameworkObjectManager::rejoinClusterSecondaryMessageHandler (FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;
    
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryMessageHandler: (ACTIVE)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::removePreviousDatabaseBackupFile),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopHeartBeatToNode),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeValidateVersionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeShutdownServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeConfigureThisLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeBootServicesPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    //Note that memory is freed in the framework in the success or the failure step
    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pFrameworkObjectManagerRejoinClusterSecondaryMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->setClusterPrimaryIpAddress  (pFrameworkObjectManagerRejoinClusterSecondaryMessage->getClusterPrimaryIpAddress ());
    pSecondaryNodeClusterContext->setClusterPrimaryPort       (pFrameworkObjectManagerRejoinClusterSecondaryMessage->getClusterPrimaryPort ());
    pSecondaryNodeClusterContext->setClusterPrimaryLocationId (pFrameworkObjectManagerRejoinClusterSecondaryMessage->getClusterPrimaryLocationId ());

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageHandler (FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage *pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageHandler: (STANDBY)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::removePreviousDatabaseBackupFile),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeShutdownServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseForStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeSetLocationRoleOnStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeEmptyDatabaseStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();

}

void PrismFrameworkObjectManager::rejoinClusterSecondaryPhase1MessageHandler (PrismRejoinClusterSecondaryPhase1Message *pPrismRejoinClusterSecondaryPhase1Message)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryPhase1MessageHandler: (ACTIVE)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase1),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeUpdateInstanceIdsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pPrismRejoinClusterSecondaryPhase1Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerPhase1MessageHandler (PrismRejoinClusterSecondaryHaPeerPhase1Message *pPrismRejoinClusterSecondaryHaPeerPhase1Message)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryPhase1MessageHandler: (STANDBY)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pPrismRejoinClusterSecondaryHaPeerPhase1Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();

}

void PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase1 (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if (LOCATION_SECONDARY_REJOIN_PHASE_0 == locationRole)
    {
    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_REJOIN_PHASE_1);
    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase1 received Message in incorrect location role, return with error");
        pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

/// Name
/// rejoinSecondaryNodeValidateStep
/// Description
/// This function validates the rejoin request received on the secondary node frameowrk
/// from the primary node framework.It validates
/// 1. Location Role
/// 2. Loaction Id of the Node
/// 3. Location Id of the primary
/// 4. IP Address
///  Input
/// SecondaryNodeClusterContext:pointer to the context
/// Output
/// none
/// Return
/// none
void PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : Entering ...");

    FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage = reinterpret_cast<FrameworkObjectManagerRejoinClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());

    prismAssert(NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage,__FILE__,__LINE__);

    ResourceId    status               = WAVE_MESSAGE_SUCCESS;

    //Postboot needed to false and this will be reset only in case of replace rejoin or default configuration rejoin.
    setIsPostBootNeededDuringRejoin (false);

    LocationRole  currentLocationRole             = m_pThisLocation->getLocationRole ();

    if (true == FrameworkToolKit::isConfigurationCompatibilityCheckRequired ())
    {
        UI64      configurationTransactionId  = 0;  
        DateTime  configurationTime;
        DateTime  latestUpdateTime;
                      
        PersistenceToolKit::getConfigurationManagedObjectInformation (configurationTransactionId, configurationTime, latestUpdateTime); 

        vector<UI64> configurationNumber = pFrameworkObjectManagerRejoinClusterSecondaryMessage->getConfigurationNumber ();

        trace (TRACE_LEVEL_INFO, string (" size of configuration number from primary is ") + configurationNumber.size ());

        if ((configurationTransactionId != configurationNumber [0]) && (0 != configurationTransactionId))
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep configuration mismatch primary node transaction id is ") + configurationNumber [0] + string (" This node transaction id is ") + configurationTransactionId);

            status  = WAVE_MESSAGE_ERROR_CONFIGURATION_MISMATCH; 
        }
    }

    // Ensure node is in standalone role if rejoin is happening due to switch replacement.
    if (true == pFrameworkObjectManagerRejoinClusterSecondaryMessage->getIsReplaceRejoin ())
    {
        if (LOCATION_STAND_ALONE != currentLocationRole)
        {
            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : This location does not have standalone role. Cannot replace rejoin.");

                status = FRAMEWORK_ERROR_CANNOT_JOIN_CLUSTER_LOCATION_UNKNOWN_ROLE;
            }
        }

        //Postboot needed during replace rejoin
        setIsPostBootNeededDuringRejoin (true);

        if (WAVE_MESSAGE_SUCCESS == status)
        {        
            pSecondaryNodeClusterContext->executeNextStep (status);
            return;
        }
    }

        /* We need to support rejoin node after failover from the cluster */
    if (LOCATION_SECONDARY_UNCONFIRMED != currentLocationRole && LOCATION_PRIMARY_UNCONFIRMED != currentLocationRole)
    {
        if (LOCATION_PRIMARY == currentLocationRole)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : This location is a primary location.");
        }
        else if (LOCATION_SECONDARY == currentLocationRole)
        {
            // This coindition can happen when link toggling cause HB failure but no FC principal change. In this case on HB failure Primary will trigger uncontrolled failover and sent rejoin               to this node. This node didn't get a chance to become principal as on link toggling this node become fc principal for very short duration. By the time heartbeat fails this node               was not fc principal any more.

            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : This location is already a confirmed secondary location, Proceeding For Rejoin ");
        }
        else if (LOCATION_SUB_LOCATION == currentLocationRole)
        {
            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : This location is a sub location.");

                status = FRAMEWORK_ERROR_CANNOT_JOIN_CLUSTER_LOCATION_SUB_LOCATION;
            }
        }
        else if (LOCATION_STAND_ALONE == currentLocationRole)
        {
            //Postboot needed during default configuration rejoin.
            setIsPostBootNeededDuringRejoin (true);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : Stand Alone, getting rejoin");

                pSecondaryNodeClusterContext->executeNextStep (status);
                return;
            }
        }
        else
        {
            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : This location has unknown role.");

                status = FRAMEWORK_ERROR_CANNOT_JOIN_CLUSTER_LOCATION_UNKNOWN_ROLE;
            }
        }
    }

    //LocationId validation
    LocationId   currentLocationId = FrameworkToolKit::getThisLocationId ();
    LocationId   suggestedLocationId = pFrameworkObjectManagerRejoinClusterSecondaryMessage->getLocationId ();

    if (currentLocationId != suggestedLocationId)
    {
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinSecondaryNodeValidateStep : CurrentLocationId (") + currentLocationId + ") does not match with Suggected LocationId (" + suggestedLocationId + ").");

            status = FRAMEWORK_ERROR_CANNOT_JOIN_CLUSTER_LOCATIONID_MISMATCH;
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
    return;
}

/// Name
/// rejoinSecondaryNodeValidateVersionStep
/// Description
/// This function compares the version received on the primary 
/// with the version received on the secondary
/// Input
/// SecondaryNodeClusterContext:pointer to the context
/// Output
/// none
/// Return
/// none
void PrismFrameworkObjectManager::rejoinSecondaryNodeValidateVersionStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
   FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage = dynamic_cast<FrameworkObjectManagerRejoinClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());

   prismAssert(NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage,__FILE__, __LINE__);
 
    string                                 primaryPrismVersionString              = pFrameworkObjectManagerRejoinClusterSecondaryMessage->getClusterPrimaryPrismVersion ();
    string                                 thisLocationPrismPrimaryVersionString  = PrismVersion::getVersionString ();
    ResourceId                             status                                 = WAVE_MESSAGE_SUCCESS;

    if (getNumberOfLineCardPostBootCurrentlyRunning ())
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateVersionStep :LC Postboot in progress. Returning error.");
        pSecondaryNodeClusterContext->executeNextStep (FRAMEWORK_LINECARD_POSTBOOT_IN_PROGRESS);
        return;
    }

    trace (TRACE_LEVEL_INFO, "This       Location Prism Version : " + thisLocationPrismPrimaryVersionString);
    trace (TRACE_LEVEL_INFO, "Requesting Location Prism Version : " + primaryPrismVersionString);

    if (0 == thisLocationPrismPrimaryVersionString.compare (primaryPrismVersionString))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateVersionStep : Version Check succeeded.\n");

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateVersionStep : Version Check failed.\n");

        status = FRAMEWORK_ERROR_CANNOT_BECOME_SECONDARY_WAVE_VERSION_MISMATCH;
    }

    vector<string>  managedObjectNamesForSchemaChange;
    vector<string>  fieldNamesStringsForSchemaChange;
    vector<string>  fieldNamesTypesForSchemaChange;
    vector<UI32>    classIds;
    vector<UI32>    parentTableIds;

    pFrameworkObjectManagerRejoinClusterSecondaryMessage->getSchemaChangeVectors (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange, classIds, parentTableIds);

    if (!isSchemaReceivedFromPrimaryCompatible (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange, classIds, parentTableIds))
    {
        status = WAVE_MESSAGE_ERROR_SCHEMA_MISMATCH;
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}


/// Name
/// rejoinSecondaryNodeValidateServicesStep
/// Description
/// This function loops through each of the services and
/// if the service is enabled it sends the validation data
/// to the service.The response received is appended to
/// the message to be sent back to the primary.
/// Input
/// SecondaryNodeClusterContext:pointer to the context
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep : Entering\n");
    vector<PrismServiceId> prismServiceIds;
    PrismThread::getListOfServiceIds (prismServiceIds);

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    UI32  numberOfPrismServiceIds = prismServiceIds.size ();

    FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage = dynamic_cast<FrameworkObjectManagerRejoinClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    prismAssert(NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage, __FILE__ , __LINE__);

    for (UI32 i = 0; i < numberOfPrismServiceIds; i++)
    {
        trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep : Validating Service : ") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));
 
        if (true == (isServiceToBeExcludedInClusterCommunications (prismServiceIds[i]))  || false == isServiceEnabled(prismServiceIds[i]))
        {
            continue;
        }

        UI32   size  = 0;
        void* pValidationData = pFrameworkObjectManagerRejoinClusterSecondaryMessage->transferBufferToUser (prismServiceIds[i], size);

        WaveObjectManagerValidateClusterCreationMessage message (prismServiceIds[i]);

        // If we have got some validation data then attach it and send it to the service.

        if ((NULL != pValidationData) && (0 != size))
        {
            trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep : Sending Validation details to Service : ") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            message.setValidationDetails (pValidationData, size, true);
        }

    LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();

    status = sendSynchronously (&message, thisLocationId);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_SERVICE_VALIDATION_FAILED;
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep : Failed to Validate Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Status : " + FrameworkToolKit::localize (status));
            break;
        }

        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            status = FRAMEWORK_ERROR_SERVICE_VALIDATION_FAILED;
            trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep : Failed to Validate Service (") + FrameworkToolKit::getServiceNameById (prismServiceIds[i]) + "), Completion Status : " + FrameworkToolKit::localize (status));
            break;
        }

        void *pValidationResults = NULL;

        message.getValidationResults (pValidationResults, size);

    //attach validation results to the message to be sent back to the principal
        if ((0 != size) && (NULL != pValidationResults))
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep : Obtained Validation Results for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));

            pFrameworkObjectManagerRejoinClusterSecondaryMessage->addBuffer (prismServiceIds[i] + s_offSetForValidationResults, size, pValidationResults, true);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::rejoinSecondaryNodeValidateServicesStep : No Validation Results are obtained for Service : " + FrameworkToolKit::getServiceNameById (prismServiceIds[i]));
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep Entering");

    if ((true == FrameworkToolKit::isConfigurationCompatibilityCheckRequired ()) && (true == getIsPostBootNeededDuringRejoin ()))
    {
        // For LC mode Notify HA that secondary rejoining cluster with default config to start, so HA sync to be stopped.
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep: Notify on ClusterUnReady event for secondary rejoin with default config");

        FrameworkToolKit::disableLiveSync ();
        setSyncState (OUT_OF_SYNC);
        setNeedNotifyClusterReadyState (true);
        bool readyState = false;
        notifyClusterReadyState (readyState);
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);
    }
    else if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
         trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep: Ha Peer is connected and Live Sync enabled.");

         FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage = dynamic_cast<FrameworkObjectManagerRejoinClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
         prismAssert(NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage, __FILE__ , __LINE__);

         FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage *pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage = new FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage;

         prismAssert (NULL != pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage, __FILE__, __LINE__);

         pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage->copyBuffersFrom (*pFrameworkObjectManagerRejoinClusterSecondaryMessage);  

         ResourceId haPeerSendStatus = send (pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageCallback), pSecondaryNodeClusterContext, 200000, FrameworkToolKit::getHaPeerLocationId ());
 
        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep: Send to Ha Peer failed.");
            delete pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep: Cluster Phase 0 to Ha Peer start.");
                pSecondaryNodeClusterContext->setClusterHaSyncInProgress (true);
            }
        }
    }
    else if ((FrameworkToolKit::getSyncState () == IN_SYNC) && (false == FrameworkToolKit::getIsLiveSyncEnabled ()))
    { 
        // DB previously in sync from sync dump. but live transaction sync not supported. So set out of sync and notify on sync update fail
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep: Live sync disabled. Notify sync update failure on first update after sync dump.");
        FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_NOT_SUPPORTED);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeNotifyHaPeerStep: Ha Peer is unavailable OR Live sync is disabled.");
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage *pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageCallback: Begin.");
 
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage)
    {
        delete pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage;
    }

    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pContext); 

    if (true == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerMessageCallback: Cluster Phase 0 to Ha Peer end.");

        if ( (pSecondaryNodeClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterContext);
            pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

/// Name
/// rejoinSecondaryNodeShutdownServicesStep
/// Description
/// This will trigger the shutdown of services
/// (shutdown, disable, uninitialize).
/// Input
/// SecondaryNodeClusterContext:pointer to the context
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinSecondaryNodeShutdownServicesStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::rejoinSecondaryNodeShutdownServicesStep : Entering ...");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    status = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_REJOIN);

    pSecondaryNodeClusterContext->executeNextStep (status);
}

/// Name
/// rejoinSecondaryNodeConnectToPrimaryStep
/// Description
/// This function connects back to the primary. Typically,
/// by this time the connection is already set up to the primary.
/// The connection to the primary is established when the first
/// message has been received from the Primary.
/// Input
/// SecondaryNodeClusterContext:pointer to the context
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep : Entering ...");

    static const UI32                                    maximumNumberOfRetriesToConnectToARemoteLocation     = 30;
    static const UI32                                    maximumNumberOfSecondsToWaitToConnect                = 30;
    FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage = dynamic_cast<FrameworkObjectManagerRejoinClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());

    prismAssert (NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage,__FILE__, __LINE__);
         
    SI32       currentPrimaryPort       = pSecondaryNodeClusterContext->getClusterPrimaryPort();
    LocationId currentPrimaryLocationId = pSecondaryNodeClusterContext->getClusterPrimaryLocationId();
    string     primaryIpAddress         = pSecondaryNodeClusterContext->getClusterPrimaryIpAddress ();

    m_pThisLocation->setClusterPrimaryLocationId (currentPrimaryLocationId);
    m_pThisLocation->setClusterPrimaryPort       (currentPrimaryPort);

    //The rejoining  node should have the primary node as one of its known locations
    //If the node has rebooted and is then rejoining, it should get the list of 
    // all known locations from the persistent config.
    if (true != (isAKnownLocation (currentPrimaryLocationId)))
    {
        if (true == pFrameworkObjectManagerRejoinClusterSecondaryMessage->getIsReplaceRejoin ())
        { 
            // If this is a replace rejoin then add the cluster primary as a known location.

            m_pThisLocation->addKnownLocation (currentPrimaryLocationId, primaryIpAddress, currentPrimaryPort);
        }
        else if ((LOCATION_STAND_ALONE == m_pThisLocation->getLocationRole ()) || (LOCATION_PRIMARY_UNCONFIRMED == m_pThisLocation->getLocationRole ()) || (LOCATION_SECONDARY_UNCONFIRMED == m_pThisLocation->getLocationRole ()))
        {
            // If a primary/secondary node in cluster is rebooted and before it comes up(can be held at boot,failed to boot etc.,)
            // a new node has joined the cluster and has become new primary(the current primary is rebooted after add node).
            // The new node will send an invite to the node which went for a reboot and is UP now.
            // Since the other node is in primary/secondary unconfirmed role after it is up from a reboot 
            // and is not aware of the new primary yet it should consider adding the primary to its known locations.

            m_pThisLocation->addKnownLocation (currentPrimaryLocationId, primaryIpAddress, currentPrimaryPort);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep: Primary location is unknown. Our location " + FrameworkToolKit::localize (m_pThisLocation->getLocationRole ()));

            prismAssert (false, __FILE__, __LINE__);

            pSecondaryNodeClusterContext->executeNextStep (FRAMEWORK_ERROR_CANNOT_JOIN_CLUSTER_LOCATION_CONFIGURATION_INVALID);
            return;
        }
    }

    string       currentPrimaryIpAddress  = FrameworkToolKit::getIpAddressForLocationId (currentPrimaryLocationId);
    ResourceId   status                   = m_pThisLocation->connectToRemoteLocation (currentPrimaryLocationId, maximumNumberOfRetriesToConnectToARemoteLocation, maximumNumberOfSecondsToWaitToConnect);

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep : Could not connect to Current Primary : " + currentPrimaryIpAddress + ", Status = " + status);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep : Now Connected to Current Primary : " + currentPrimaryIpAddress + ".");
        status = WAVE_MESSAGE_SUCCESS;
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep : Rolling back as the node is in phase 0/1");

        ResourceId rollbackStatus = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodeConnectToPrimaryStep : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        m_pThisLocation->setLocationRole(LOCATION_SECONDARY_REJOIN_PHASE_0);
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

/// Name
/// rejoinSecondaryNodeConnectToPrimaryStep
/// Description
///TODO: AASHISH: This should setup fields in the 
/// Local Cluster Object Manager
/// Input
/// SecondaryNodeClusterContext:pointer to the context
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinSecondaryNodeConfigureThisLocationStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::rejoinSecondaryNodeConfigureThisLocationStep : Entering ...");
    FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage = dynamic_cast<FrameworkObjectManagerRejoinClusterSecondaryMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());

    prismAssert(NULL != pFrameworkObjectManagerRejoinClusterSecondaryMessage, __FILE__, __LINE__);

#if 0
    LocationId                                           currentPrimaryLocationId                             = m_pThisLocation->getClusterPrimaryLocationId ();
    SI32                                                 clusterPrimaryPort                                   = FrameworkToolKit::getPortForLocationId (currentPrimaryLocationId);
    string                                               currentPrimaryIpAddress                              = FrameworkToolKit::getIpAddressForLocationId (currentPrimaryLocationId);
#endif

    ResourceId                                           processingStatus                                     = WAVE_MESSAGE_SUCCESS;
    UI32                                                 sizeOfValidationDetails                              = 0;
    void                                                *pValidationDetailsBuffer                             = NULL;

    // If replace rejoin scenario then reset the current locationId and ipAddress
    if ((true == pFrameworkObjectManagerRejoinClusterSecondaryMessage->getIsReplaceRejoin ()) || (getThisLocationId () != pFrameworkObjectManagerRejoinClusterSecondaryMessage->getLocationId ()))
    {
        LocationId myNewLocationId          = pFrameworkObjectManagerRejoinClusterSecondaryMessage->getLocationId ();

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::rejoinSecondaryNodeConfigureThisLocationStep : replaceRejoin: reconfiguring this location with locationId ") + myNewLocationId);

        m_pThisLocation->setLocationId (myNewLocationId);
    }

    // Get the validation details, transfer those to the user from the message.
    // We will resuse those to send it to Local Cluster Conficuration service.

    pFrameworkObjectManagerRejoinClusterSecondaryMessage->getValidationDetails (sizeOfValidationDetails, pValidationDetailsBuffer, true);

    pSecondaryNodeClusterContext->executeNextStep (processingStatus);
}

/// Name
/// rejoinSecondaryNodeBootServicesPrePhaseStep
/// Description
/// This will trigger the bbot of some services 
/// in the first phase of rejoin processing.
/// Input
/// SecondaryNodeClusterContext:pointer to the context
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinSecondaryNodeBootServicesPrePhaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::rejoinSecondaryNodeBootServicesPrePhaseStep : Entering ...");

    //The first parameter will result in the worker choosing a specific boot agent written especially for rejoin.
    //It will trigger the pre phase
    ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_SECONDARY_REJOIN, WAVE_BOOT_PHASE_PRE_PHASE);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodeBootServicesPrePhaseStep : Booting Secondary during rejoin must NEVER fail.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

/// Name
/// rejoinClusterSecondaryPhase2MessageHandler
/// Description
/// This handler establishes the steps to process te phase 2
/// rejoin message from the secondary and kick starts its processing.
/// Input
/// PrismRejoinClusterSecondaryPhase2Message: Pointer to phase 2 message
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinClusterSecondaryPhase2MessageHandler (PrismRejoinClusterSecondaryPhase2Message *pPrismRejoinClusterSecondaryPhase2Message)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase2),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodePhase2BootServicesPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase2ReplaceNodeIfNeededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase2ConfigureThisLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context = new SecondaryNodeClusterPhase2Context (pPrismRejoinClusterSecondaryPhase2Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]), false);

    pSecondaryNodeClusterPhase2Context->holdAll ();
    pSecondaryNodeClusterPhase2Context->start ();
}

void PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase2 (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context)
{
    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if (LOCATION_SECONDARY_REJOIN_PHASE_1 == locationRole)
    {
    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_REJOIN_PHASE_2);
    pSecondaryNodeClusterPhase2Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase2 received Message in incorrect location role, return with error");
        pSecondaryNodeClusterPhase2Context->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

/// Name
/// rejoinClusterSecondaryPhase2MessageHandler
/// Description
/// This function restarts all the services on the 
/// Input
/// SecondaryNodeClusterContext: pointer to ctxt 
/// Output
/// none
/// Return
/// none

void PrismFrameworkObjectManager::rejoinSecondaryNodePhase2BootServicesPostPhaseStep (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase2BootServicesPostPhaseStep : Entering ...");

    //The first argument results in selection of specific boot agent for rejoin node
    ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_SECONDARY_REJOIN, WAVE_BOOT_PHASE_POST_PHASE);

    // Instead of asserting, rollback to last known good state.

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "rejoinSecondaryNodePhase2BootServicesPostPhaseStep: Rollback to last known state");

        disconnectFromAllConnectedNodes ();

        ResourceId rollbackStatus = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_ROLLBACK);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase2BootServicesPostPhaseStep : shutdown should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }

        rollbackStatus = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase2BootServicesPostPhaseStep : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

#if 0
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase2BootServicesPostPhaseStep : Booting Secondary immediately after configuring must NEVER fail.");
        prismAssert (false, __FILE__, __LINE__);
    }
#endif
    pSecondaryNodeClusterPhase2Context->executeNextStep (status);
}

void PrismFrameworkObjectManager::rejoinClusterSecondaryPhase3MessageHandler (PrismRejoinClusterSecondaryPhase3Message *pPrismRejoinClusterSecondaryPhase3Message)
{
    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryPhase3MessageHandler: (ACTIVE)");

    if (getIsPostBootNeededDuringRejoin ())
    {
        // Perform post boot in this case after backend sync up.
        // Backend sync up is done as part of rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3StartEvent),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3ExecutePostBootStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3Complete),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3SendStartHearBeat),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
        };

        pSecondaryNodeClusterPhase3Context = new SecondaryNodeClusterPhase3Context (pPrismRejoinClusterSecondaryPhase3Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        pSecondaryNodeClusterPhase3Context->holdAll ();
        pSecondaryNodeClusterPhase3Context->start ();

    }
    else
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3StartEvent),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3Complete),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3SendStartHearBeat),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncPostPhaseStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
        };

        pSecondaryNodeClusterPhase3Context = new SecondaryNodeClusterPhase3Context (pPrismRejoinClusterSecondaryPhase3Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        pSecondaryNodeClusterPhase3Context->holdAll ();
        pSecondaryNodeClusterPhase3Context->start ();
    }
}

void PrismFrameworkObjectManager::rejoinClusterSecondaryHaPeerPhase3MessageHandler (PrismRejoinClusterSecondaryHaPeerPhase3Message *pPrismRejoinClusterSecondaryHaPeerPhase3Message)
{
    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinClusterSecondaryPhase3MessageHandler: (STANDBY)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prepareStandbyAfterClusterPhaseSync),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterPhase3Context = new SecondaryNodeClusterPhase3Context (pPrismRejoinClusterSecondaryHaPeerPhase3Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterPhase3Context->holdAll ();
    pSecondaryNodeClusterPhase3Context->start ();
}

void PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    PrismRejoinClusterSecondaryPhase1Message *pPrismRejoinClusterSecondaryPhase1Message = reinterpret_cast<PrismRejoinClusterSecondaryPhase1Message *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    prismAssert (NULL != pPrismRejoinClusterSecondaryPhase1Message, __FILE__, __LINE__);

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        PrismRejoinClusterSecondaryHaPeerPhase1Message *pPrismRejoinClusterSecondaryHaPeerPhase1Message = new PrismRejoinClusterSecondaryHaPeerPhase1Message;
        prismAssert (NULL != pPrismRejoinClusterSecondaryHaPeerPhase1Message, __FILE__, __LINE__);

        pPrismRejoinClusterSecondaryHaPeerPhase1Message->copyBuffersFrom (*pPrismRejoinClusterSecondaryPhase1Message);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseStep: Ha Peer is connected and Live Sync enabled.");

        ResourceId haPeerSendStatus = send (pPrismRejoinClusterSecondaryHaPeerPhase1Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseCallback), pSecondaryNodeClusterContext, 200000, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseStep: Send to Ha Peer failed.");
            delete pPrismRejoinClusterSecondaryHaPeerPhase1Message;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseStep: Cluster Pre-Phase to Ha Peer start.");
                pSecondaryNodeClusterContext->setClusterHaSyncInProgress (true);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseStep: Ha Peer is unavailable OR Live sync is disabled.");
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseCallback (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryHaPeerPhase1Message *pPrismRejoinClusterSecondaryHaPeerPhase1Message, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseCallback: Begin.");
    
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pPrismRejoinClusterSecondaryHaPeerPhase1Message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pPrismRejoinClusterSecondaryHaPeerPhase1Message)
    {
        delete pPrismRejoinClusterSecondaryHaPeerPhase1Message;
    }

    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pContext); 

    if (true == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPrePhaseCallback: Cluster Pre-Phase to Ha Peer end.");

        if ( (pSecondaryNodeClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterContext);
            pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    PrismRejoinClusterSecondaryPhase3Message *pPrismRejoinClusterSecondaryPhase3Message = reinterpret_cast<PrismRejoinClusterSecondaryPhase3Message *> (pSecondaryNodeClusterPhase3Context->getPPrismMessage ());
    prismAssert (NULL != pPrismRejoinClusterSecondaryPhase3Message, __FILE__, __LINE__);

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        PrismRejoinClusterSecondaryHaPeerPhase3Message *pPrismRejoinClusterSecondaryHaPeerPhase3Message = new PrismRejoinClusterSecondaryHaPeerPhase3Message;
        prismAssert (NULL != pPrismRejoinClusterSecondaryHaPeerPhase3Message, __FILE__, __LINE__);

        pPrismRejoinClusterSecondaryHaPeerPhase3Message->copyBuffersFrom (*pPrismRejoinClusterSecondaryPhase3Message);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseStep: Ha Peer is connected and Live Sync enabled.");

        ResourceId haPeerSendStatus = send (pPrismRejoinClusterSecondaryHaPeerPhase3Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseCallback), pSecondaryNodeClusterPhase3Context, s_clusterPhase3TimeoutInMilliseconds, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseStep: Send to Ha Peer failed.");
            delete pPrismRejoinClusterSecondaryHaPeerPhase3Message;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterPhase3Context->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseStep: Cluster Post-Phase to Ha Peer start.");
                pSecondaryNodeClusterPhase3Context->setClusterHaSyncInProgress (true);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseStep: Ha Peer is unavailable OR Live sync is disabled.");
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseCallback (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryHaPeerPhase3Message *pPrismRejoinClusterSecondaryHaPeerPhase3Message, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseCallback: Begin.");
 
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pPrismRejoinClusterSecondaryHaPeerPhase3Message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pPrismRejoinClusterSecondaryHaPeerPhase3Message)
    {
        delete pPrismRejoinClusterSecondaryHaPeerPhase3Message;
    }

    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = reinterpret_cast<SecondaryNodeClusterPhase3Context *> (pContext); 

    if (true == pSecondaryNodeClusterPhase3Context->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterPhase3Context->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryHaPeerPostPhaseCallback: Cluster Post-phase to Ha Peer end.");

        if ( (pSecondaryNodeClusterPhase3Context->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterPhase3Context);
            pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }

}

void PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3 (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if (LOCATION_SECONDARY_REJOIN_PHASE_2 == locationRole)
    {
    m_pThisLocation->setLocationRole (LOCATION_SECONDARY_REJOIN_PHASE_3);
    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3 received Message in incorrect location role, return with error");
        pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3Complete (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    m_pThisLocation->setLocationRole (LOCATION_SECONDARY);
    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
void PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep : Entering ...");

    //The first argument results in selection of specific boot agent for rejoin node
    ResourceId status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_SECONDARY_REJOIN, WAVE_BOOT_PHASE_AFTER_POST_PHASE);

    // Instead of asserting, rollback to last known good state.

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep: Rollback to last known state");

        disconnectFromAllConnectedNodes ();

        ResourceId rollbackStatus = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_ROLLBACK);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep : shutdown should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }

        rollbackStatus = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2);

        if (WAVE_MESSAGE_SUCCESS != rollbackStatus)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep : Roll Back should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

#if 0
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep : Booting Secondary immediately after configuring must NEVER fail.");
        prismAssert (false, __FILE__, __LINE__);
    }
#endif
    pSecondaryNodeClusterPhase3Context->executeNextStep (status);

}



void PrismFrameworkObjectManager::detachFromClusterMessageHandler (FrameworkObjectManagerDetachFromClusterMessage *pFrameworkObjectManagerDetachFromClusterMessage)
{
    DetachFromClusterAsynchronousContext *pDetachFromClusterAsynchronousContext = new DetachFromClusterAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::detachFromClusterMessageHandlerCallback), pFrameworkObjectManagerDetachFromClusterMessage);

    pDetachFromClusterAsynchronousContext->setReasonForDetachingFromCluster (pFrameworkObjectManagerDetachFromClusterMessage->getReasonForDetachingFromCluster ());

    detachFromClusterAsynchronousHandler (pDetachFromClusterAsynchronousContext);
}

void PrismFrameworkObjectManager::detachFromClusterMessageHandlerCallback (DetachFromClusterAsynchronousContext *pDetachFromClusterAsynchronousContext)
{
    prismAssert (NULL != pDetachFromClusterAsynchronousContext, __FILE__, __LINE__);

    FrameworkObjectManagerDetachFromClusterMessage *pFrameworkObjectManagerDetachFromClusterMessage = reinterpret_cast<FrameworkObjectManagerDetachFromClusterMessage *> (pDetachFromClusterAsynchronousContext->getPCallerContext ());

    prismAssert (NULL != pFrameworkObjectManagerDetachFromClusterMessage, __FILE__, __LINE__);

    pFrameworkObjectManagerDetachFromClusterMessage->setCompletionStatus (pDetachFromClusterAsynchronousContext->getCompletionStatus ());

    delete pDetachFromClusterAsynchronousContext;

    reply (pFrameworkObjectManagerDetachFromClusterMessage);
}

void PrismFrameworkObjectManager::detachFromClusterAsynchronousHandler (DetachFromClusterAsynchronousContext *pDetachFromClusterAsynchronousContext)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::detachFromClusterValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::unconfigureClusterSecondaryNodeBootServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::detachFromClusterTerminateClientConnectionsForKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::detachFromClusterDisconnectFromKnownLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::detachFromClusterUnconfigureThisLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::detachFromClusterRunFailoverStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    DetachFromClusterContext *pDetachFromClusterContext = new DetachFromClusterContext (pDetachFromClusterAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDetachFromClusterContext->setReasonForDetachingFromCluster (pDetachFromClusterAsynchronousContext->getReasonForDetachingFromCluster ());

    pDetachFromClusterContext->holdAll ();
    pDetachFromClusterContext->start ();
}

void PrismFrameworkObjectManager::detachFromClusterValidateStep (DetachFromClusterContext *pDetachFromClusterContext)
{
    LocationRole thisLocationRole = m_pThisLocation->getLocationRole ();

    pDetachFromClusterContext->setThisLocationRoleBeforeDetach (thisLocationRole);

    if (LOCATION_SECONDARY == thisLocationRole)
    {
        pDetachFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }
    else
    {
        pDetachFromClusterContext->executeNextStep (FRAMEWORK_ERROR_ONLY_SECONDARY_CAN_DETACH_ITSELF_FROM_CLUSTER);
        return;
    }
}

void PrismFrameworkObjectManager::detachFromClusterTerminateClientConnectionsForKnownLocationsStep (DetachFromClusterContext *pDetachFromClusterContext)
{
    LocationId clusterPrimaryLocationId = m_pThisLocation->getClusterPrimaryLocationId ();
    string     clusterPrimaryIpAddress  = FrameworkToolKit::getIpAddressForLocationId (clusterPrimaryLocationId);
    SI32       clusterPrimaryPort       = FrameworkToolKit::getPortForLocationId (clusterPrimaryLocationId);

    InterLocationMessageReceiverObjectManager::terminateAndRemoveInterLocationMessageReceiverThreadFromCache (clusterPrimaryIpAddress, clusterPrimaryPort);

    pDetachFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::detachFromClusterDisconnectFromKnownLocationsStep (DetachFromClusterContext *pDetachFromClusterContext)
{
    LocationId clusterPrimaryLocationId = m_pThisLocation->getClusterPrimaryLocationId ();

    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::detachFromClusterDisconnectFromKnownLocationsStep: Disconnect from location");
    disconnectFromLocation (clusterPrimaryLocationId);
    removeKnownLocation (clusterPrimaryLocationId);

    pDetachFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::detachFromClusterUnconfigureThisLocationStep (DetachFromClusterContext *pDetachFromClusterContext)
{
    LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();

    m_pThisLocation->resetLocationToStandAlone (thisLocationId);
    m_lastUsedLocationId = thisLocationId;

    pDetachFromClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::detachFromClusterRunFailoverStep (DetachFromClusterContext *pDetachFromClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::detachFromClusterRunFailoverStep : Entering ...");

    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::detachFromClusterRunFailoverCallback), pDetachFromClusterContext);

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
    prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

    pPrismFrameworkFailoverWorkerContext->setThisLocationRole (pDetachFromClusterContext->getThisLocationRoleBeforeDetach ());
    pPrismFrameworkFailoverWorkerContext->setFailoverReason (pDetachFromClusterContext->getReasonForDetachingFromCluster ());

    m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
}

void PrismFrameworkObjectManager::detachFromClusterRunFailoverCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::detachFromClusterRunFailoverCallback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    DetachFromClusterContext *pDetachFromClusterContext = reinterpret_cast<DetachFromClusterContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId                status                    = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pDetachFromClusterContext, __FILE__, __LINE__);

    pDetachFromClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::lostHeartBeatMessageHandler (FrameworkObjectManagerLostHeartBeatMessage *pFrameworkObjectManagerLostHeartBeatMessage)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::lostHeartBeatMessageHandler : Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::lostHeartBeatStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    HeartBeatLostContext *pHeartBeatLostContext      = new HeartBeatLostContext (pFrameworkObjectManagerLostHeartBeatMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    UI32                  numberOfHeartBeatLostNodes = pFrameworkObjectManagerLostHeartBeatMessage->getNumberOfNodes ();
    UI32                  i                          = 0;

    // Transfer the details about failed nodes into the context.

    for (i = 0; i < numberOfHeartBeatLostNodes; i++)
    {
        pHeartBeatLostContext->addNodeIpAddressAndPort (pFrameworkObjectManagerLostHeartBeatMessage->getNodeAt (i), pFrameworkObjectManagerLostHeartBeatMessage->getNodePortAt(i));
    }

    pHeartBeatLostContext->holdAll ();
    pHeartBeatLostContext->start ();
}

void PrismFrameworkObjectManager::lostHeartBeatStep (HeartBeatLostContext *pHeartBeatLostContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::lostHeartBeatCleanupClusterStep : Entering ...");

    UI32       numberOfFailedLocations = pHeartBeatLostContext->getNumberOfNodes ();
    UI32       i                       = 0;
    LocationId failedLocationId;
    string     failedIpAddress;
    UI32       failedPort;

    if (LOCATION_PRIMARY == (m_pThisLocation->getLocationRole ()))
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::lostHeartBeatCleanupClusterStep : Initiating Primary Un-Controlled Failover.");

        PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::lostHeartBeatPrimaryUncontrolledFailoverCallback), pHeartBeatLostContext);

        prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
        prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

        pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);
        pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

        for (i = 0; i < numberOfFailedLocations; i++)
        {
            failedIpAddress = pHeartBeatLostContext->getNodeIpAddressAtIndex (i);
            failedPort      = pHeartBeatLostContext->getNodePortAtIndex (i);

            failedLocationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (failedIpAddress, failedPort);

            pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationId);
        }

        m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
    }
    else if (LOCATION_SECONDARY == (m_pThisLocation->getLocationRole ()))
    {
        // On a secondary location there can be only one failed heart beat.  That heart beat will belong to the current primary node.  It is so because the secondaries heart beat with primary only.

        prismAssert (1 == numberOfFailedLocations, __FILE__, __LINE__);

        failedIpAddress = pHeartBeatLostContext->getNodeIpAddressAtIndex (i);
        failedPort      = pHeartBeatLostContext->getNodePortAtIndex (i);

        failedLocationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (failedIpAddress, failedPort);

        prismAssert (failedLocationId == (m_pThisLocation->getClusterPrimaryLocationId ()), __FILE__, __LINE__);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::lostHeartBeatCleanupClusterStep : Initiating Role Arbitration Sequence.");

        vector<LocationId> connectedLocationsVector;
        UI32               numberOfConnectedLocations;
        LocationId         nextPrimaryCandidateLocationId = 0;
        string             nextPrimaryCandidateIpAddress;
        UI32               nextPrimaryCandidatePort;
        LocationId         thisLocationId                 = FrameworkToolKit::getThisLocationId ();

        // Add this location also into the list since it also can be a possible candidate.

        connectedLocationsVector.push_back (thisLocationId);
        FrameworkToolKit::getConnectedLocations (connectedLocationsVector);
        numberOfConnectedLocations = connectedLocationsVector.size ();

        nextPrimaryCandidateLocationId = connectedLocationsVector[0];

        for (i = 1; i < numberOfConnectedLocations; i++)
        {
            if (nextPrimaryCandidateLocationId > connectedLocationsVector[i])
            {
                nextPrimaryCandidateLocationId = connectedLocationsVector[i];
            }
        }

        nextPrimaryCandidateIpAddress = FrameworkToolKit::getIpAddressForLocationId (nextPrimaryCandidateLocationId);
        nextPrimaryCandidatePort      = FrameworkToolKit::getPortForLocationId      (nextPrimaryCandidateLocationId);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::lostHeartBeatStep : The Next Primary will be the following location : " + nextPrimaryCandidateIpAddress + string (":") + nextPrimaryCandidatePort + string (", ") + nextPrimaryCandidateLocationId);

        if (nextPrimaryCandidateLocationId == thisLocationId)
        {
            // Activate the Secondary Uncontrolled Failover Agent.

            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::lostHeartBeatCleanupClusterStep : Initiating Secondary Un-Controlled Failover.");

            PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::lostHeartBeatSecondaryUncontrolledFailoverCallback), pHeartBeatLostContext);

            prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
            prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

            pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_SECONDARY);
            pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

            for (i = 0; i < numberOfFailedLocations; i++)
            {
                failedIpAddress = pHeartBeatLostContext->getNodeIpAddressAtIndex (i);
                failedPort      = pHeartBeatLostContext->getNodePortAtIndex (i);

                failedLocationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (failedIpAddress, failedPort);

                pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationId);
            }

            m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
        }
        else
        {
            // Otherwise simply proceed and finish.  The next Primary candidate will take over.
            // FIXME : sagar : however, we need to arm a timer here and check if some one has really taken over or not.
            //                 There can be a case where the next primary candidate can fail in the middle of taking over.

            pHeartBeatLostContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::lostHeartBeatCleanupClusterStep : We got a Heartbeat Lost Message when there is no cluster created.  Simply ignoring the message.");
        pHeartBeatLostContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFrameworkObjectManager::lostHeartBeatPrimaryUncontrolledFailoverCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::lostHeartBeatPrimaryUncontrolledFailoverCallback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    HeartBeatLostContext *pHeartBeatLostContext = reinterpret_cast<HeartBeatLostContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId            status                = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pHeartBeatLostContext, __FILE__, __LINE__);

    pHeartBeatLostContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::lostHeartBeatSecondaryUncontrolledFailoverCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::lostHeartBeatSecondaryUncontrolledFailoverCallback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    HeartBeatLostContext    *pHeartBeatLostContext = reinterpret_cast<HeartBeatLostContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId               status                = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pHeartBeatLostContext, __FILE__, __LINE__);

    pHeartBeatLostContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::lostHeartBeatSecondaryControlledFailoverCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::lostHeartBeatSecondaryControlledFailoverCallback : Entering ...");

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__); 
    HeartBeatLostContext *pHeartBeatLostContext = reinterpret_cast<HeartBeatLostContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId            status                = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    vector<LocationId> failedLocationIds;
    UI32               numberOfFailedLocations;

    pPrismFrameworkFailoverWorkerContext->getFailedLocationIds (failedLocationIds); 
    numberOfFailedLocations = failedLocationIds.size ();

    for (UI32 i = 0; i < numberOfFailedLocations; i++)
    {
        removeKnownLocation (failedLocationIds [i]);
    }
    
    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pHeartBeatLostContext, __FILE__, __LINE__);

    pHeartBeatLostContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::lostHeartBeatCleanupClusterDestroyClusterCallback (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::lostHeartBeatCleanupClusterDestroyClusterCallback : Entering ...");

    prismAssert (NULL != pDestroyClusterAsynchronousContext, __FILE__, __LINE__);

    HeartBeatLostContext *pHeartBeatLostContext = reinterpret_cast<HeartBeatLostContext *> (pDestroyClusterAsynchronousContext->getPCallerContext ());

    prismAssert (NULL != pHeartBeatLostContext, __FILE__, __LINE__);

    ResourceId status = pDestroyClusterAsynchronousContext->getCompletionStatus ();

    delete pDestroyClusterAsynchronousContext;

    pHeartBeatLostContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::lostHeartBeatCleanupClusterDetachFromClusterCallback (DetachFromClusterAsynchronousContext *pDetachFromClusterAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::lostHeartBeatCleanupClusterDetachFromClusterCallback : Entering ...");

    prismAssert (NULL != pDetachFromClusterAsynchronousContext, __FILE__, __LINE__);

    HeartBeatLostContext *pHeartBeatLostContext = reinterpret_cast<HeartBeatLostContext *> (pDetachFromClusterAsynchronousContext->getPCallerContext ());

    prismAssert (NULL != pHeartBeatLostContext, __FILE__, __LINE__);

    ResourceId status = pDetachFromClusterAsynchronousContext->getCompletionStatus ();

    delete pDetachFromClusterAsynchronousContext;

    pHeartBeatLostContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::startExternalStateSynchronizationHandler(FrameworkObjectManagerStartExternalStateSynchronizationMessage *pFrameworkObjectManagerStartExternalStateSynchronizationMessage)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::startExternalStateSynchronizationHandler : Start the External State Synchronization .... ");

    int stageNumber = pFrameworkObjectManagerStartExternalStateSynchronizationMessage->getFssStageNumber();
    
    ResourceId serviceType = pFrameworkObjectManagerStartExternalStateSynchronizationMessage->getServiceType();

    string perfTraceStr = string("ESS Bootup; stage: ") + stageNumber + string("; service-type: ") + FrameworkToolKit::localize(serviceType);
    trace (TRACE_LEVEL_PERF_START, perfTraceStr);

    PrismExternalStateSynchronizationAgent *pPrismExternalStateSynchronization = new PrismExternalStateSynchronizationAgent (m_pWaveObjectManager, stageNumber, serviceType);

    pPrismExternalStateSynchronization->execute ();

    delete pPrismExternalStateSynchronization;

    pFrameworkObjectManagerStartExternalStateSynchronizationMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pFrameworkObjectManagerStartExternalStateSynchronizationMessage);

    trace (TRACE_LEVEL_PERF_END, perfTraceStr);
}

void PrismFrameworkObjectManager::startSlotFailoverHandler (FrameworkObjectManagerStartSlotFailoverMessage *pFrameworkObjectManagerStartSlotFailoverMessage)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::startSlotFailoverHandler : Start the SlotFailover Agent.... ");

    int slotNumber = pFrameworkObjectManagerStartSlotFailoverMessage->getSlotNumber ();
    
    SlotFailoverAgent *pSlotFailoverAgent = new SlotFailoverAgent (m_pWaveObjectManager, slotNumber);

    pSlotFailoverAgent->execute ();

    delete pSlotFailoverAgent;
    
    pFrameworkObjectManagerStartSlotFailoverMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pFrameworkObjectManagerStartSlotFailoverMessage);

}

void PrismFrameworkObjectManager::primaryChangedMessageHandler (FrameworkObjectManagerPrimaryChangedMessage *pFrameworkObjectManagerPrimaryChangedMessage)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;
    
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedMessageHandler: Received Primary changed notification(ACTIVE).");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::removePreviousDatabaseBackupFile),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedStopHeartBeatToOldPrimayStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeShutdownServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedUpdatePrimaryDetailsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeBootServicesPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedEmptyDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncStatusStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pFrameworkObjectManagerPrimaryChangedMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->setClusterPrimaryIpAddress  (pFrameworkObjectManagerPrimaryChangedMessage->getNewPrimaryIpAddress ());
    pSecondaryNodeClusterContext->setClusterPrimaryPort       (pFrameworkObjectManagerPrimaryChangedMessage->getNewPrimaryPort ());
    pSecondaryNodeClusterContext->setClusterPrimaryLocationId (pFrameworkObjectManagerPrimaryChangedMessage->getNewPrimaryLocationId ());

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::primaryChangedHaPeerMessageHandler (FrameworkObjectManagerPrimaryChangedHaPeerMessage *pFrameworkObjectManagerPrimaryChangedHaPeerMessage)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerMessageHandler: (STANDBY)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::removePreviousDatabaseBackupFile),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeShutdownServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBootServicesPrePhaseForStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeBackupCurrentDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodeSetLocationRoleOnStandbyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeCleanPreparedTransactionsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedEmptyDatabaseStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pFrameworkObjectManagerPrimaryChangedHaPeerMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->setDBEmptyRequired (pFrameworkObjectManagerPrimaryChangedHaPeerMessage->getIsDBEmptyRequired ());

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::primaryChangedPhase1MessageHandler (FrameworkObjectManagerPrimaryChangedPhase1Message *pFrameworkObjectManagerPrimaryChangedPhase1Message)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedPhase1MessageHandler: Received Primary changed notification: (ACTIVE)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase1),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedLoadDatabaseFromPrimaryDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodeUpdateInstanceIdsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncStatusStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pFrameworkObjectManagerPrimaryChangedPhase1Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    UI64 trackingNumber                 = PersistenceLocalObjectManager::getAnyConfigurationChangeTrackingNumber ();
    UI64 primaryPrincipalTrackingNumber = pFrameworkObjectManagerPrimaryChangedPhase1Message->getAnyConfigurationChangeTrackingNumber ();

    if (trackingNumber == primaryPrincipalTrackingNumber)
    {
        pSecondaryNodeClusterContext->setDBEmptyRequired (false);
    }

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();
}

void PrismFrameworkObjectManager::primaryChangedHaPeerPhase1MessageHandler (FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message)
{
    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedPhase1MessageHandler: Received Primary changed notification: (STANDBY)");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedLoadDatabaseFromPrimaryDatabaseStep),
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterContext = new SecondaryNodeClusterContext (pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterContext->setDBEmptyRequired (pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message->getIsDBRestoreRequired ());

    pSecondaryNodeClusterContext->holdAll ();
    pSecondaryNodeClusterContext->start ();

}

void PrismFrameworkObjectManager::primaryChangedValidateStep ( PrismLinearSequencerContext *pPrismLinearSequencerContext )
{

    if (getNumberOfLineCardPostBootCurrentlyRunning ())
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedValidateStep :LC Postboot in progress. Returning error.");
        pPrismLinearSequencerContext->executeNextStep (FRAMEWORK_LINECARD_POSTBOOT_IN_PROGRESS);
        return;
    }    

    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    if ( LOCATION_SECONDARY == locationRole)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_WARN, string("PrismFrameworkObjectManager::primaryChangedValidateStep : received Message in incorrect location role : ") + FrameworkToolKit::localize(locationRole) + (", return with error"));
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep ( SecondaryNodeClusterContext *pSecondaryNodeClusterContext )
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep: Entering \n");

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep: Ha Peer is connected and Live Sync enabled.");

        FrameworkObjectManagerPrimaryChangedMessage *pFrameworkObjectManagerPrimaryChangedMessage = dynamic_cast<FrameworkObjectManagerPrimaryChangedMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());
        prismAssert(NULL != pFrameworkObjectManagerPrimaryChangedMessage, __FILE__ , __LINE__);

        FrameworkObjectManagerPrimaryChangedHaPeerMessage *pFrameworkObjectManagerPrimaryChangedHaPeerMessage = new FrameworkObjectManagerPrimaryChangedHaPeerMessage;

        pFrameworkObjectManagerPrimaryChangedHaPeerMessage->setDBEmptyRequired (pSecondaryNodeClusterContext->getIsDBEmptyRequired ());

        prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedHaPeerMessage, __FILE__, __LINE__);

        pFrameworkObjectManagerPrimaryChangedHaPeerMessage->copyBuffersFrom (*pFrameworkObjectManagerPrimaryChangedMessage);

        ResourceId haPeerSendStatus = send (pFrameworkObjectManagerPrimaryChangedHaPeerMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::primaryChangedNotifyHaPeerMessageCallback), pSecondaryNodeClusterContext, 200000, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep: Send to Ha Peer failed.");
            delete pFrameworkObjectManagerPrimaryChangedHaPeerMessage;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep: Cluster Phase 0 to Ha Peer start.");
                pSecondaryNodeClusterContext->setClusterHaSyncInProgress (true);
            }
        }
    }
    else if ((FrameworkToolKit::getSyncState () == IN_SYNC) && (false == FrameworkToolKit::getIsLiveSyncEnabled ()))
    {
        // DB previously in sync from sync dump. but live transaction sync not supported. So set out of sync and notify on sync update fail
        trace (TRACE_LEVEL_WARN, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep: Live sync disabled. Notify sync update failure on first update after sync dump.");
        FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_NOT_SUPPORTED);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerStep: Ha Peer is unavailable OR live sync disabled.");
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::primaryChangedNotifyHaPeerMessageCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerPrimaryChangedHaPeerMessage *pFrameworkObjectManagerPrimaryChangedHaPeerMessage, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerMessageCallback: Begin.");
 
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerMessageCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pFrameworkObjectManagerPrimaryChangedHaPeerMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerMessageCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pFrameworkObjectManagerPrimaryChangedHaPeerMessage)
    {
        delete pFrameworkObjectManagerPrimaryChangedHaPeerMessage;
    }

    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pContext); 

    if (true == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedNotifyHaPeerMessageCallback: Cluster Phase 0 to Ha Peer end.");

        if ( (pSecondaryNodeClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterContext);
            pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    FrameworkObjectManagerPrimaryChangedPhase1Message *pFrameworkObjectManagerPrimaryChangedPhase1Message = reinterpret_cast<FrameworkObjectManagerPrimaryChangedPhase1Message *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedPhase1Message, __FILE__, __LINE__);

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message = new FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message;
        prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message, __FILE__, __LINE__);

        pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message->setDBRestoreRequired (pSecondaryNodeClusterContext->getIsDBEmptyRequired ());
        pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message->copyBuffersFrom (*pFrameworkObjectManagerPrimaryChangedPhase1Message);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseStep: Ha Peer is connected and Live Sync enabled.");

        ResourceId haPeerSendStatus = send (pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseCallback), pSecondaryNodeClusterContext, 200000, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseStep: Send to Ha Peer failed.");
            delete pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseStep: Cluster Pre-phase to Ha Peer start.");
                pSecondaryNodeClusterContext->setClusterHaSyncInProgress (true);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseStep: Ha Peer is unavailable OR Live sync is disabled.");
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseCallback: Starting.");
    
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message)
    {
        delete pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message;
    }

    SecondaryNodeClusterContext *pSecondaryNodeClusterContext = reinterpret_cast<SecondaryNodeClusterContext *> (pContext); 

    if (true == pSecondaryNodeClusterContext->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterContext->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPrePhaseCallback: Cluster Pre-Phase to Ha Peer end.");

        if ( (pSecondaryNodeClusterContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterContext);
            pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseStep (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    FrameworkObjectManagerPrimaryChangedPhase3Message *pFrameworkObjectManagerPrimaryChangedPhase3Message = reinterpret_cast<FrameworkObjectManagerPrimaryChangedPhase3Message *> (pSecondaryNodeClusterPhase3Context->getPPrismMessage ());
    prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedPhase3Message, __FILE__, __LINE__);

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message = new FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message;
        prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message, __FILE__, __LINE__);

        pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message->copyBuffersFrom (*pFrameworkObjectManagerPrimaryChangedPhase3Message);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseStep: Ha Peer is connected and Live Sync enabled.");

        ResourceId haPeerSendStatus = send (pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message, reinterpret_cast<PrismMessageResponseHandler> (&PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseCallback), pSecondaryNodeClusterPhase3Context, s_clusterPhase3TimeoutInMilliseconds, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != haPeerSendStatus)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseStep: Send to Ha Peer failed.");
            delete pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message;
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            if (false == pSecondaryNodeClusterPhase3Context->getClusterHaSyncInProgress ())
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseStep: Cluster Post-phase to Ha Peer start.");
                pSecondaryNodeClusterPhase3Context->setClusterHaSyncInProgress (true);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseStep: Ha Peer is unavailable OR Live sync is disabled.");
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message, void *pContext)
{
    //trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseCallback: Starting.");
    
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseCallback: Failed to configure Peer, Framework Status: " + FrameworkToolKit::localize (frameworkStatus));
        FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
    }
    else
    {
        ResourceId status = pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseCallback: Failed to configure Peer, Completion Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
    }

    if (NULL != pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message)
    {
        delete pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message;
    }

    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = reinterpret_cast<SecondaryNodeClusterPhase3Context *> (pContext);

    if (true == pSecondaryNodeClusterPhase3Context->getClusterHaSyncInProgress ())
    {
        pSecondaryNodeClusterPhase3Context->setClusterHaSyncInProgress (false);

        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseCallback: Cluster Post-phase to Ha Peer end.");

        if ( (pSecondaryNodeClusterPhase3Context->getNumberOfCallbacksBeforeAdvancingToNextStep ()) > 0)
        {
            --(*pSecondaryNodeClusterPhase3Context);
            pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
    }
}

void PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler (FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage *pFrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    // Shutdown All Services First

    status = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_CONFIGURE);

    // Bringup PrePhase service
    status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_HASTANDBY, WAVE_BOOT_PHASE_PRE_PHASE);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler : Booting Secondary immediately after configuring must NEVER fail.");
        prismAssert (false, __FILE__, __LINE__);
    }

    DatabaseObjectManagerCleanPreparedTransactionsMessage cleanPreparedTransactionsMessage;

    status = sendSynchronously (&cleanPreparedTransactionsMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Unable to clean prepared transactions. Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = cleanPreparedTransactionsMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Succeeded in cleaning the previously prepared transactions.");
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Failed to clean previously prepared transactions. Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    const string    prismConfigurationfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName (); 
    vector<string>  output;
    SI32            cmdStatus                    = 0;

    trace (TRACE_LEVEL_WARN, string ("PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: deleting file ") + prismConfigurationfileName);

    cmdStatus = FrameworkToolKit::systemCommandOutput ((string ("/bin/rm -rf ") + prismConfigurationfileName).c_str(), output);

    if ( cmdStatus != 0 ) 
    {            
        trace (TRACE_LEVEL_ERROR, string("PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Cmd to delete file ")+ prismConfigurationfileName + string(" failed with error message : ") + output[0]);
    }

    DatabaseObjectManagerEmptyMessage databaseEmptyMessage;
    status  = sendSynchronously (&databaseEmptyMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {         
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Could not send message to empty database. Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = databaseEmptyMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Message to empty database failed. Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ());

    DatabaseObjectManagerRestoreMessage databaseRestoreMessage (backupFileName);
    databaseRestoreMessage.setDataOnlyRestore (true);
    databaseRestoreMessage.setSaveConfiguration (false);

    status  = sendSynchronously (&databaseRestoreMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Could not send message to restore database. Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = databaseRestoreMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_ERROR_DB_RESTORE_FAILED == status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Not Able to Restore DB from Backed Up File. Completion Status : " + FrameworkToolKit::localize (status));
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Going For Extreme Recovery By going to default/startup configuration");

            status = FrameworkToolKit::changePrismConfigurationValidity ( false );
            if ( status != WAVE_MESSAGE_SUCCESS  )
            {
                trace (TRACE_LEVEL_FATAL, ("PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: Removing CFG file failed in extreme Recoverycase "));
            }
            system ("/sbin/reboot -f");

            sleep (300);
    
            trace (TRACE_LEVEL_FATAL, ("System is not rebooting, Manual recovery is required"));
        
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::rollbackStandbyOnActiveRollbackHandler: SUCCESSFULLY rollback to the last known valid previous DB configuration.");
        }
    }

    // restore CFG file too

    rollbackCfgFile ();

    // Save Configuration File After DB restore.

    FrameworkToolKit::savePrismConfiguration ();

    // Bring up postphase service
    status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_HASTANDBY, WAVE_BOOT_PHASE_POST_PHASE);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::prepareStandbyAfterClusterPhaseSync: Post phase failed.");
        prismAssert (false, __FILE__, __LINE__);
    }

    PrismFrameworkObjectManager::setSecondaryNodeClusterCreationFlag (false);

    pFrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage->setCompletionStatus (status);
    reply (pFrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage);
}

void PrismFrameworkObjectManager::primaryChangedStopHeartBeatToOldPrimayStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::primaryChangedStopHeartBeatToOldPrimayStep : Starting ...");

    FrameworkObjectManagerPrimaryChangedMessage *pFrameworkObjectManagerPrimaryChangedMessage = reinterpret_cast<FrameworkObjectManagerPrimaryChangedMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedMessage, __FILE__, __LINE__);
    prismAssert (NULL != m_pThisLocation, __FILE__, __LINE__);

    vector<string>  managedObjectNamesForSchemaChange;
    vector<string>  fieldNamesStringsForSchemaChange;
    vector<string>  fieldNamesTypesForSchemaChange;
    vector<UI32>    classIds;
    vector<UI32>    parentTableIds;

    pFrameworkObjectManagerPrimaryChangedMessage->getSchemaChangeVectors (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange,classIds,parentTableIds); 

    if (!isSchemaReceivedFromPrimaryCompatible (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange,classIds,parentTableIds))
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR_SCHEMA_MISMATCH);
        return;
    }

    LocationId oldPrimaryLocationId     = m_pThisLocation->getClusterPrimaryLocationId ();
    string     oldPrimaryNodeIpAddress  = m_pThisLocation->getIpAddressForLocationId(oldPrimaryLocationId);
    UI32       oldPrimaryNodePort       = m_pThisLocation->getClusterPrimaryPort ();


    StopHeartBeatMessage *pStopHeartBeatMessage = new StopHeartBeatMessage();
    pStopHeartBeatMessage->setDstIpAddress(oldPrimaryNodeIpAddress);
    pStopHeartBeatMessage->setDstPortNumber(oldPrimaryNodePort);
   
    WaveMessageStatus status = sendSynchronously(pStopHeartBeatMessage);
    ResourceId        processingStatus = WAVE_MESSAGE_SUCCESS;

    if(WAVE_MESSAGE_SUCCESS != status) 
    {
        processingStatus = status;
    }
    else 
    {
        ResourceId    completionStatus = pStopHeartBeatMessage->getCompletionStatus();
        if(completionStatus != WAVE_MESSAGE_SUCCESS) 
        {
            processingStatus = completionStatus;
        }
    }

    if(WAVE_MESSAGE_SUCCESS !=  processingStatus) 
    {
        trace(TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedStopHeartBeatToOldPrimayStep:Failure sending StopHeartBeatMessage: status: " + FrameworkToolKit::localize(processingStatus));
    }
    delete pStopHeartBeatMessage;

    trace (TRACE_LEVEL_INFO, string("primaryChangedStopHeartBeatToOldPrimayStep : Loc: ") + oldPrimaryLocationId + string(" Port:") + oldPrimaryNodePort);

    //Also Disconnect From Old Primary
    disconnectFromLocation (oldPrimaryLocationId);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

}

void PrismFrameworkObjectManager::primaryChangedRemoveOldPrimaryLocationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager:: primaryChangedRemoveOldPrimaryLocationStep : Starting ...");


    FrameworkObjectManagerPrimaryChangedMessage *pFrameworkObjectManagerPrimaryChangedMessage = reinterpret_cast<FrameworkObjectManagerPrimaryChangedMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedMessage, __FILE__, __LINE__);
    prismAssert (NULL != m_pThisLocation, __FILE__, __LINE__);

    if(pFrameworkObjectManagerPrimaryChangedMessage->isPrimaryChangeDueToControlledFailover()) 
    {
        LocationId oldPrimaryLocationId     = m_pThisLocation->getClusterPrimaryLocationId ();    
        removeKnownLocation(oldPrimaryLocationId);
        tracePrintf (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager:: primaryChangedRemoveOldPrimaryLocationStep :Removing the old primary location id %u due to controlled failover", oldPrimaryLocationId);
    }
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::primaryChangedUpdatePrimaryDetailsStep (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::primaryChangedUpdatePrimaryDetailsStep : Starting ...");

    FrameworkObjectManagerPrimaryChangedMessage *pFrameworkObjectManagerPrimaryChangedMessage = reinterpret_cast<FrameworkObjectManagerPrimaryChangedMessage *> (pSecondaryNodeClusterContext->getPPrismMessage ());

    prismAssert (NULL != pFrameworkObjectManagerPrimaryChangedMessage, __FILE__, __LINE__);
    prismAssert (NULL != m_pThisLocation, __FILE__, __LINE__);

    LocationId newPrimaryLocationId = pSecondaryNodeClusterContext->getClusterPrimaryLocationId ();
    UI32       newPrimaryPort       = pSecondaryNodeClusterContext->getClusterPrimaryPort ();

    m_pThisLocation->setClusterPrimaryLocationId (newPrimaryLocationId);
    m_pThisLocation->setClusterPrimaryPort       (newPrimaryPort);
    m_pThisLocation->setLocationRole             (LOCATION_SECONDARY_REJOIN_PHASE_0);

    trace (TRACE_LEVEL_INFO, string("primaryChangedUpdatePrimaryDetailsStep : Loc: ") + newPrimaryLocationId + string(" Port:") + newPrimaryPort);

    UI64 trackingNumber                 = PersistenceLocalObjectManager::getAnyConfigurationChangeTrackingNumber ();
    UI64 primaryPrincipalTrackingNumber = pFrameworkObjectManagerPrimaryChangedMessage->getAnyConfigurationChangeTrackingNumber ();

    if (trackingNumber == primaryPrincipalTrackingNumber)
    {
        pSecondaryNodeClusterContext->setDBEmptyRequired (false);
    }

    pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::primaryChangedEmptyDatabaseStep  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    trace (TRACE_LEVEL_DEBUG, "PrismFrameworkObjectManager::primaryChangedEmptyDatabaseStep : Entering ...");

    if (false == pSecondaryNodeClusterContext->getIsDBEmptyRequired ())
    {
        pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    DatabaseObjectManagerEmptyMessage message;
    ResourceId                        status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::primaryChangedEmptyDatabaseStep : Could not send message to empty database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::primaryChangedEmptyDatabaseStep : Message to empty database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::primaryChangedLoadDatabaseFromPrimaryDatabaseStep ( SecondaryNodeClusterContext *pSecondaryNodeClusterContext)
{
    if (false  == pSecondaryNodeClusterContext->getIsDBEmptyRequired ())
    {
        pSecondaryNodeClusterContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    FrameworkObjectManagerPrimaryChangedPhase1Message *pFrameworkObjectManagerPrimaryChangedPhase1Message = reinterpret_cast<FrameworkObjectManagerPrimaryChangedPhase1Message *> (pSecondaryNodeClusterContext->getPPrismMessage ());
    UI32                                              sizeOfTheDatabaseBackupFromPrimary                  = 0;
    char                                             *pBuffer                                             = reinterpret_cast<char *> (pFrameworkObjectManagerPrimaryChangedPhase1Message->findBuffer (pFrameworkObjectManagerPrimaryChangedPhase1Message->getDatabaseBackupBufferTag (), sizeOfTheDatabaseBackupFromPrimary));

    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName3 ());

    if (NULL != pBuffer)
    {
        ofstream                               databaseBackupFile;

        databaseBackupFile.open (backupFileName.c_str (), ios::binary);
        databaseBackupFile.write (pBuffer, sizeOfTheDatabaseBackupFromPrimary);
        databaseBackupFile.close ();
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::primaryChangedLoadDatabaseFromPrimaryDatabaseStep : No Database backup has been received ???");
    }

    // Request the Database service to restore itself from the backup file that we created based on the buffer we obtained from the primary.

    DatabaseObjectManagerRestoreMessage message (backupFileName);
    message.setDataOnlyRestore (true);

    ResourceId status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::primaryChangedLoadDatabaseFromPrimaryDatabaseStep : Could not send message to restore database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::primaryChangedLoadDatabaseFromPrimaryDatabaseStep : Message to restore database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedLoadDatabaseFromPrimaryDatabaseStep : SUCCESSFULLY Synced up local database with that of Primary.");
        }
    }

    pSecondaryNodeClusterContext->executeNextStep (status);
}

void PrismFrameworkObjectManager::primaryChangedPhase2MessageHandler (FrameworkObjectManagerPrimaryChangedPhase2Message *pFrameworkObjectManagerPrimaryChangedPhase2Message)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::primaryChangedPhase2MessageHandler : Received Primary changed notification ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase2),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodePhase2BootServicesPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase2ConfigureThisLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::secondaryNodeClusterFailureStep),
    };

    SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context = new SecondaryNodeClusterPhase2Context (pFrameworkObjectManagerPrimaryChangedPhase2Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]),false);

    pSecondaryNodeClusterPhase2Context->holdAll ();
    pSecondaryNodeClusterPhase2Context->start ();
}

void PrismFrameworkObjectManager::primaryChangedPhase3MessageHandler (FrameworkObjectManagerPrimaryChangedPhase3Message *pFrameworkObjectManagerPrimaryChangedPhase3Message)
{
    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedPhase3MessageHandler: Received Primary changed notification(ACTIVE) ");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3StartEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryChangedHaPeerPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateLocationRoleStepInRejoinPhase3Complete),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::broadcastPrimaryChangedEventForPlugins),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureSecondaryNodePhase3SendStartHearBeat),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::savePrismConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::checkHaPeerSyncPostPhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterPhase3Context = new SecondaryNodeClusterPhase3Context (pFrameworkObjectManagerPrimaryChangedPhase3Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterPhase3Context->holdAll ();
    pSecondaryNodeClusterPhase3Context->start ();
}

void PrismFrameworkObjectManager::primaryChangedHaPeerPhase3MessageHandler (FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message)
{
    SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context = NULL;

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::primaryChangedPhase3MessageHandler: Received Primary changed notification(STANDBY) ");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        //reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopClusterPhaseTimer),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prepareStandbyAfterClusterPhaseSync),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    pSecondaryNodeClusterPhase3Context = new SecondaryNodeClusterPhase3Context (pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pSecondaryNodeClusterPhase3Context->holdAll ();
    pSecondaryNodeClusterPhase3Context->start ();
}

void PrismFrameworkObjectManager::broadcastClusterPhase3StartEvent (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::broadcastClusterPhase3StartEvent: Entering....");

    ClusterPhase3StartEvent *pClusterPhase3StartEvent = new ClusterPhase3StartEvent ();

    ResourceId status = broadcast (pClusterPhase3StartEvent);

    if(status != WAVE_MESSAGE_SUCCESS)
    {  
       trace(TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::broadcastClusterPhase3StartEvent: Failure during broadcast..");
       pSecondaryNodeClusterPhase3Context->executeNextStep (status);
       return;
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent Entering....");

    //Reset the secondary cluster formation flag so that the SCN handling is not postponed in case of secondary node.
    PrismFrameworkObjectManager::setSecondaryNodeClusterCreationFlag (false);

/*
    if (true == getTableIdsChangedForSchemaCompatibilityFlag ())
    {
        setTableIdsChangedForSchemaCompatibilityFlag (false);
    }
*/

    ClusterPhase3CompleteEvent *pClusterPhase3CompleteEvent = new ClusterPhase3CompleteEvent ();

    ResourceId status = broadcast (pClusterPhase3CompleteEvent);

    if(status != WAVE_MESSAGE_SUCCESS)
    {
       trace(TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::broadcastClusterPhase3CompleteEvent: Failure during broadcast..");
       pSecondaryNodeClusterPhase3Context->executeNextStep (status);
       return;
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void PrismFrameworkObjectManager::broadcastNodesAdditionToClusterCompletedEvent (CreateClusterWithNodesContext *pCreateClusterWithNodesContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::broadcastNodesAdditionToClusterCompletedEvent Entering....");

    vector<LocationId>      successfullyAddedLocationIdVector = pCreateClusterWithNodesContext->getSuccessfullyAddedLocationIdVector ();

    WaveNodesAdditionToClusterCompletedEvent* pWaveNodesAdditionToClusterCompletedEvent = new WaveNodesAdditionToClusterCompletedEvent();
    for(UI32 i = 0; i<successfullyAddedLocationIdVector.size(); ++i)
    {
       string ipAddress     = FrameworkToolKit::getIpAddressForLocationId(successfullyAddedLocationIdVector[i]);
       UI32    port         = FrameworkToolKit::getPortForLocationId(successfullyAddedLocationIdVector[i]);
       pWaveNodesAdditionToClusterCompletedEvent->addNewNodeDetails(successfullyAddedLocationIdVector[i], ipAddress, port);
    }

    ResourceId status = broadcast(pWaveNodesAdditionToClusterCompletedEvent);
    if(status != WAVE_MESSAGE_SUCCESS)
    {
       trace(TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::broadcastNodesAdditionToClusterCompletedEvent: Failure during broadcast..");
       pCreateClusterWithNodesContext->executeNextStep (status);
       return;
    }

    pCreateClusterWithNodesContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    trace(TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::broadcastNodesAdditionToClusterCompletedEvent: Exiting..");
    return;

}

void PrismFrameworkObjectManager::broadcastPrimaryChangedEventForPlugins (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::broadcastPrimaryChangedEventForPlugins Entering....");

    PrimaryChangedEvent *pPrimaryChangedEvent = new PrimaryChangedEvent ();
    LocationId           newPrimaryLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();

    pPrimaryChangedEvent->setNewPrimaryLocationId (newPrimaryLocationId);

    broadcast (pPrimaryChangedEvent);

    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::broadcastPrimaryChangedEventForPlugins : Broadcasted an event PrimaryChangedEvent with New Primary LocationId is ") + newPrimaryLocationId);
    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void  PrismFrameworkObjectManager::savePrismConfigurationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace(TRACE_LEVEL_DEVEL,"PrismFrameworkObjectManager::savePrismConfigurationStep: Entering");
    FrameworkToolKit::savePrismConfiguration(true);
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

}

void PrismFrameworkObjectManager::sendReplyBackToClusterGlobalService  (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    //PrismMessage *pMessage = pPrismLinearSequencerContext->getPPrismMessage ();

//    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

//    reply (pMessage);

//    pPrismLinearSequencerContext->setPPrismMessage (NULL);

    pPrismLinearSequencerContext->unholdAll ();

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

FrameworkSequenceGenerator &PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ()
{
    FrameworkSequenceGenerator *pFrameworkSequenceGenerator = FrameworkSequenceGeneratorFactory::obtainFrameworkSequenceGenerator (s_frameworkSequenceType);

    return (*pFrameworkSequenceGenerator);
}

ResourceId PrismFrameworkObjectManager::saveConfiguration (const bool &syncToStandby)
{
    return (m_pConfigurationWorker->savePrismConfiguration (s_frameworkConfigurationFile, syncToStandby));
}

ResourceId PrismFrameworkObjectManager::changePrismConfigurationValidity( const bool &validity)
{
    return (m_pConfigurationWorker->changePrismConfigurationValidity( validity ));
}

string PrismFrameworkObjectManager::getConfigurationFileName ()
{
    return (s_frameworkConfigurationFile);
}

string PrismFrameworkObjectManager::getConfigurationBackupFileName ()
{
    return (s_frameworkConfigurationBackupFile);
}

string PrismFrameworkObjectManager::getLockFileForConfigurationFile ()
{
    return (s_lockFileForConfigurationFile);
}

SI32 PrismFrameworkObjectManager::getLocationPort ()
{
    return (s_locationPort);
}

PrismFrameworkConfigurationWorker *PrismFrameworkObjectManager::getPConfigurationWorker ()
{
    return (m_pConfigurationWorker);
}

PrismFinalizeWorker *PrismFrameworkObjectManager::getPFinalizeWorker ()
{
    return (m_pPrismFinalizeWorker);
}

PrismFrameworkObjectManagerInitializeWorker *PrismFrameworkObjectManager::getPInitializeWorker ()
{
    return (m_pInitializeWorker);
}

bool PrismFrameworkObjectManager::isServiceToBeExcludedInClusterCommunications (const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId               ()) == prismServiceId)           ||
        ((CentralClusterConfigObjectManager::getPrismServiceId         ()) == prismServiceId)           ||
        ((ClusterTestObjectManager::getPrismServiceId                  ()) == prismServiceId)           ||
        ((RegressionTestObjectManager::getPrismServiceId               ()) == prismServiceId)           ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))) ||
        (true == (isServiceDynamicallyExcludedFromClusterCommunications (prismServiceId))))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void PrismFrameworkObjectManager::configureAsLocation (SI32 port)
{
    s_locationRole = LOCATION_STAND_ALONE;
    s_locationPort = port;
}

void PrismFrameworkObjectManager::configureAsSubLocation (LocationId parentLocationId, string &parentIpAddress, SI32 parentPort, LocationId locationId, string &ipAddress, SI32 port)
{
    s_locationRole             = LOCATION_SUB_LOCATION;
    s_locationParentLocationId = parentLocationId;
    s_locationParentIpAddress  = parentIpAddress;
    s_locationParentPort       = parentPort;
    s_locationLocationId       = locationId;
    s_locationIpAddress        = ipAddress;
    s_locationPort             = port;
}

void PrismFrameworkObjectManager::resetLocationToPrimary ()
{
    prismAssert (NULL != m_pThisLocation, __FILE__, __LINE__);

    m_pThisLocation->resetLocationToPrimary ();
}

void PrismFrameworkObjectManager::acquireBootSynchronizationMutex ()
{
    s_bootSynchronizationMutex.lock ();
}

void PrismFrameworkObjectManager::releaseBootSynchronizationMutex ()
{
    s_bootSynchronizationMutex.unlock ();
}

void PrismFrameworkObjectManager::dynamicallyExcludeServiceFromClusterCommunications (const PrismServiceId &prismServiceId)
{
    m_servicesToBeExcludedForClusterCommunicationsLock.lock ();

    map<PrismServiceId, PrismServiceId>::iterator element    = m_servicesToBeExcludedForClusterCommunications.find (prismServiceId);
    map<PrismServiceId, PrismServiceId>::iterator endElement = m_servicesToBeExcludedForClusterCommunications.end  ();

    if (endElement == element)
    {
        m_servicesToBeExcludedForClusterCommunications[prismServiceId] = prismServiceId;
    }

    m_servicesToBeExcludedForClusterCommunicationsLock.unlock ();
}

bool PrismFrameworkObjectManager::isServiceDynamicallyExcludedFromClusterCommunications (const PrismServiceId &prismServiceId)
{
    bool isFound = false;

    m_servicesToBeExcludedForClusterCommunicationsLock.lock ();

    map<PrismServiceId, PrismServiceId>::iterator element    = m_servicesToBeExcludedForClusterCommunications.find (prismServiceId);
    map<PrismServiceId, PrismServiceId>::iterator endElement = m_servicesToBeExcludedForClusterCommunications.end  ();

    if (endElement != element)
    {
        isFound = true;
    }

    m_servicesToBeExcludedForClusterCommunicationsLock.unlock ();

    return (isFound);
}

void PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (const PrismServiceId &prismServiceId)
{
    (PrismFrameworkObjectManager::getInstance ())->dynamicallyExcludeServiceFromClusterCommunications (prismServiceId);
}

void PrismFrameworkObjectManager::setIpAddressForThisLocation (const string &ipAddressForThisLocation)
{
    m_ipAddressForThisLocationMutex.lock ();

    m_ipAddressForThisLocation = ipAddressForThisLocation;

    m_ipAddressForThisLocationMutex.unlock ();
}

string PrismFrameworkObjectManager::getIpAddressForThisLocation ()
{
    string ipAddressForThisLocation;

    m_ipAddressForThisLocationMutex.lock ();

    ipAddressForThisLocation = m_ipAddressForThisLocation;

    m_ipAddressForThisLocationMutex.unlock ();

    return (ipAddressForThisLocation);
}

void PrismFrameworkObjectManager::setEthernetInterfaceForThisLocation (const string &ethernetInterfaceForThisLocation)
{
    m_ethernetInterfaceForThisLocation = ethernetInterfaceForThisLocation;
}

string PrismFrameworkObjectManager::getEthernetInterfaceForThisLocation ()
{
    return (m_ethernetInterfaceForThisLocation);
}

void PrismFrameworkObjectManager::updateIpAddressForThisLocation (const string &ipAddressForThisLocation)
{
    setIpAddressForThisLocation (ipAddressForThisLocation);

    (PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->resetIpAddress (ipAddressForThisLocation);
}

void PrismFrameworkObjectManager::setIsDBRestoreIncomplete (bool dbRestoreIncomplete)
{
    s_dbRestoreMutex.lock ();
    m_isDBRestoreIncomplete = dbRestoreIncomplete;
    s_dbRestoreMutex.unlock ();
}

bool PrismFrameworkObjectManager::getIsDBRestoreIncomplete ()
{
    bool isRestoreIncomplete;

    s_dbRestoreMutex.lock ();
    isRestoreIncomplete = m_isDBRestoreIncomplete;
    s_dbRestoreMutex.unlock ();

    return (isRestoreIncomplete);
}

void PrismFrameworkObjectManager::setDbConversionStatus (ResourceId dbConversionStatus)
{
    s_dbConversionStatusMutex.lock ();
    m_dbConversionStatus = dbConversionStatus;
    s_dbConversionStatusMutex.unlock ();
}

ResourceId PrismFrameworkObjectManager::getDbConversionStatus () const
{
    ResourceId dbConversionStatus;

    s_dbConversionStatusMutex.lock ();
    dbConversionStatus = m_dbConversionStatus;
    s_dbConversionStatusMutex.unlock ();

    return (dbConversionStatus);
}

/*
void PrismFrameworkObjectManager::setTableIdsChangedForSchemaCompatibilityFlag (bool tableIdsChanged)
{
    m_tableIdsChangedForSchemaCompatibility = tableIdsChanged; 
    tracePrintf (TRACE_LEVEL_INFO, true, false, "PrismFrameworkObjectManager::setTableIdsChangedForSchemaCompatibilityFlag: flag set to %u", tableIdsChanged);
}

bool PrismFrameworkObjectManager::getTableIdsChangedForSchemaCompatibilityFlag () 
{
    return (m_tableIdsChangedForSchemaCompatibility);
}
*/
void PrismFrameworkObjectManager::setAuxilliaryTableDetailsForSchemaConversion (vector<string> oldAuxilliaryTables, vector<string> newAuxilliaryTables, vector<string> parentTables, vector<string> relatedToTables)
{
    m_oldAuxilliaryTableNamesBeforeConversion = oldAuxilliaryTables;
    m_newAuxilliaryTableNamesAfterConversion  = newAuxilliaryTables;
    m_parentTableNamesForAuxilliaryTables     = parentTables;
    m_relatedToTableNamesForAuxilliaryTables  = relatedToTables;   
}

void PrismFrameworkObjectManager::getAuxilliaryTableDetailsForSchemaConversion (vector<string>& oldAuxilliaryTables, vector<string>& newAuxilliaryTables, vector<string>& parentTables, vector<string>& relatedToTables)
{
    oldAuxilliaryTables = m_oldAuxilliaryTableNamesBeforeConversion;
    newAuxilliaryTables = m_newAuxilliaryTableNamesAfterConversion;
    parentTables        = m_parentTableNamesForAuxilliaryTables;
    relatedToTables     = m_relatedToTableNamesForAuxilliaryTables;
}

void PrismFrameworkObjectManager::setIsStartupValid (const bool &isValid)
{
    s_startupValidMutex.lock ();
    m_isStartupValid = isValid;
    s_startupValidMutex.unlock ();
}

bool PrismFrameworkObjectManager::getIsStartupValid ()
{
    bool isStartupValid;

    s_startupValidMutex.lock ();
    isStartupValid = m_isStartupValid;
    s_startupValidMutex.unlock ();

    return (isStartupValid);
}

string PrismFrameworkObjectManager::getStartupFileName () const
{
    string startupFileName;

    s_startupFileMutex.lock ();
    startupFileName = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + s_startupFileName;
    s_startupFileMutex.unlock ();

    return (startupFileName);
}

void PrismFrameworkObjectManager::setStartupFileName (const string &fileName)
{
    if (getStartupFileName () != fileName)
    {
        string createFileLink = string("ln -sf " + fileName + " " + FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + s_startupFileName);
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager:: setStartupFileName: setting startupFile to " + fileName);
        s_startupFileMutex.lock ();
        system (createFileLink.c_str ());
        s_startupFileMutex.unlock ();
    }
}

ResourceId PrismFrameworkObjectManager::getStartupFileType () const
{
    ResourceId startupFileType;

    s_startupFileTypeMutex.lock ();
    startupFileType = m_startupFileType;
    s_startupFileTypeMutex.unlock ();

    return (startupFileType);
}

void PrismFrameworkObjectManager::setStartupFileType (const ResourceId &fileType)
{
    s_startupFileTypeMutex.lock ();
    m_startupFileType = fileType;
    s_startupFileTypeMutex.unlock ();
}

bool PrismFrameworkObjectManager::getSecondaryNodeClusterCreationFlag ()
{
    bool secondaryNodeClusterCreationFlag;

    m_secondaryNodeClusterCreationFlagMutex.lock ();
    secondaryNodeClusterCreationFlag = m_secondaryNodeClusterCreationFlag;
    m_secondaryNodeClusterCreationFlagMutex.unlock ();

    return (secondaryNodeClusterCreationFlag);
}

void PrismFrameworkObjectManager::setSecondaryNodeClusterCreationFlag (bool secondaryNodeClusterCreationFlag)
{
    m_secondaryNodeClusterCreationFlagMutex.lock ();
    m_secondaryNodeClusterCreationFlag = secondaryNodeClusterCreationFlag;
    m_secondaryNodeClusterCreationFlagMutex.unlock ();
}

bool PrismFrameworkObjectManager::getPrimaryNodeClusterOperationFlag ()
{
    bool primaryNodeClusterOperationFlag;

    m_primaryNodeClusterOperationFlagMutex.lock ();
    primaryNodeClusterOperationFlag = m_primaryNodeClusterOperationFlag;
    m_primaryNodeClusterOperationFlagMutex.unlock ();

    return (primaryNodeClusterOperationFlag);
}

void PrismFrameworkObjectManager::setPrimaryNodeClusterOperationFlag (bool primaryNodeClusterOperationFlag)
{
    m_primaryNodeClusterOperationFlagMutex.lock ();
    m_primaryNodeClusterOperationFlag = primaryNodeClusterOperationFlag;
    m_primaryNodeClusterOperationFlagMutex.unlock ();
}

// Processing Secondary Node failure.
void PrismFrameworkObjectManager::secondaryNodeFailureNotificationMessageHandler(FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager:: secondaryNodeFailureNotificationMessageHandler: Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::processSecondeyNodeFailureMessage),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
    
}
void PrismFrameworkObjectManager::processSecondeyNodeFailureMessage(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage = dynamic_cast<FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *>(pPrismLinearSequencerContext->getPPrismMessage());

    vector<string>     failedIpAddresses  = pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getFailedIpAddresses();
    vector<LocationId> failedLocationIds  = pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getFailedLocationIds ();

    LocationId  thisLocationId = FrameworkToolKit::getThisLocationId ();


    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = NULL;

   // Activate the Primary Uncontrolled Failover Agent.

    if (true == pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getIsControlledFailoverInSpecialCase ())
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::secondaryNodeFailureNotificationMessageHandler::Initiating Primary Controlled Failover.");

        pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::lostHeartBeatSecondaryControlledFailoverCallback), pPrismLinearSequencerContext);

        prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
        prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

        pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);

        pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::secondaryNodeFailureNotificationMessageHandler::Initiating Primary Un-Controlled Failover.");

        pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::lostHeartBeatSecondaryUncontrolledFailoverCallback), pPrismLinearSequencerContext);

        prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
        prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

        pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);

        pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);
    }

    UI32 numFailedLocationIds = failedLocationIds.size ();

    for (UI32 i = 0; i < numFailedLocationIds; ++i) 
    {
        pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIds[i]);
        disconnectFromLocation (failedLocationIds[i]);
        prismAssert (failedLocationIds[i] != thisLocationId, __FILE__, __LINE__);
    }
    
    m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);

}


//New Principal Selection handling

void PrismFrameworkObjectManager::newPrincipalSelectedAfterFaioverMessageHandler(FrameworkObjectManagerNewPrincipalEstablishedMessage *pFrameworkObjectManagerNewPrincipalEstablishedMessage)
{
    //Steps
    //1. validate
    //2. reboot global services
    //3. Send messages to all secondaries informing them that this node 
    //   is the new principal
    //4. Update local WaveNode object to inform it of the new primary Selection
    //5. Update the WaveNode of the old Principal to mark it as secondary_disconnected
    // 

    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager:: newPrincipalSelectedAfterFaioverMessageHandler: Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
    reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::validateNewPrincipalSelection),
    reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::processNewPrincipalEstablishedMessage),
#if 0
    reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::updateWaveNodeManagedObjctsStep),
    reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::startHeartBeatToAllSecondariesStep),
#endif
    reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterSuccessStep),
    reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::primaryNodeClusterFailureStep),
    };
    
    setPrimaryNodeClusterOperationFlag (true);

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFrameworkObjectManagerNewPrincipalEstablishedMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
    
}

void PrismFrameworkObjectManager::validateNewPrincipalSelection (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  trace(TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::validateNewPrincipalSelection..Entering");

  LocationRole locationRole = m_pThisLocation->getLocationRole ();
 
  // When a node goes to UNCONFIRMED role following a rollback from rejoin cluster phases 0/1 and
  // If there is a message pending in the queue to have this node become principal before 
  // the services were shutdown we should return an error upon picking the message since it is UNCONFIRMED role.
  // Framework should act upon new principal message only if the node's current role is SECONDARY

  if ((true == PrismFrameworkObjectManager::getSecondaryNodeClusterCreationFlag ()) || (LOCATION_PRIMARY_UNCONFIRMED == locationRole) || (LOCATION_SECONDARY_UNCONFIRMED == locationRole))
  {
      pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS);
      return;
  }

  prismAssert(m_pThisLocation->getLocationRole () == LOCATION_SECONDARY, __FILE__, __LINE__);

  FrameworkObjectManagerNewPrincipalEstablishedMessage *pFrameworkObjectManagerNewPrincipalEstablishedMessage = dynamic_cast<FrameworkObjectManagerNewPrincipalEstablishedMessage *>(pPrismLinearSequencerContext->getPPrismMessage());

  prismAssert(NULL != pFrameworkObjectManagerNewPrincipalEstablishedMessage, __FILE__, __LINE__);

  pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);


}

void PrismFrameworkObjectManager::processNewPrincipalEstablishedMessage (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

   FrameworkObjectManagerNewPrincipalEstablishedMessage *pFrameworkObjectManagerNewPrincipalEstablishedMessage = dynamic_cast<FrameworkObjectManagerNewPrincipalEstablishedMessage *>(pPrismLinearSequencerContext->getPPrismMessage());

   prismAssert (NULL != pFrameworkObjectManagerNewPrincipalEstablishedMessage, __FILE__, __LINE__);

   string     failedIpAddress  = pFrameworkObjectManagerNewPrincipalEstablishedMessage->getFailedPrincipalIpAddress();
   //UI32       failedPort       = pFrameworkObjectManagerNewPrincipalEstablishedMessage->getFailedPrincipalPort();
   LocationId failedLocationId = pFrameworkObjectManagerNewPrincipalEstablishedMessage->getFailedPrincipalLocationId ();

   LocationId  thisLocationId = FrameworkToolKit::getThisLocationId ();

   prismAssert (failedLocationId != thisLocationId, __FILE__, __LINE__);

   // Activate the Secondary Uncontrolled Failover Agent.
  

    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::lostHeartBeatSecondaryUncontrolledFailoverCallback), pPrismLinearSequencerContext);

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
    prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

    pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_SECONDARY);
    if(pFrameworkObjectManagerNewPrincipalEstablishedMessage->getTriggerControlledFailover()) 
    {
      trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::processNewPrincipalEstablishedMessage::Initiating Secondary Controlled Failover.");
      pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);
      pPrismFrameworkFailoverWorkerContext->setSecondaryControlledFailoverDueToPrimaryRemoval();

    }
    else 
    {
      trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::processNewPrincipalEstablishedMessage::Initiating Secondary Un-Controlled Failover.");
      pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);
    }
    pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationId);
    m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
 
}

void PrismFrameworkObjectManager::disconnectFromAllNodesHandler (FrameworkObjectManagerDisconnectFromAllNodesMessage *pFrameworkObjectManagerDisconnectFromAllNodesMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::disconnectAllKnownLocationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFrameworkObjectManagerDisconnectFromAllNodesMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void PrismFrameworkObjectManager::establishPrincipalAfterClusterRebootHandler (FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage* pFrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage)
{
    PrismLinearSequencerStep sequencerSteps [] = 
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::establishPrincipalAfterClusterRebootStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep)
    };
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();


}

void PrismFrameworkObjectManager::establishPrincipalAfterClusterRebootStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager::establishPrincipalAfterClusterRebootStep");

    resetLocationToPrimary ();
    
    FrameworkToolKit::savePrismConfiguration (true);

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::disconnectAllKnownLocationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    vector<LocationId> connectedLocationsVector;
    LocationId         nodeLocationId;
    UI32               nodeCount;
    UI32               totalNodes               = 0;

    FrameworkToolKit::getKnownLocations (connectedLocationsVector);
    totalNodes = connectedLocationsVector.size ();

    for (nodeCount = 0; nodeCount < totalNodes; nodeCount++)
    {
        nodeLocationId = connectedLocationsVector[nodeCount];
        disconnectFromLocation (nodeLocationId);
        //removeKnownLocation    (nodeLocationId);

        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::disconnectAllKnownLocationStep ")+nodeLocationId);
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::invalidateClientStreamingSocketForRemoteLocation (LocationId locationId)
{
    m_pThisLocation->invalidateClientStreamingSocketForRemoteLocation(locationId);

}
void PrismFrameworkObjectManager::resetNodeForClusterMergeHandler (FrameworkObjectManagerResetNodeToUnconfirmRole *pFrameworkObjectManagerResetNodeToUnconfirmRole)
{
    if (LOCATION_STAND_ALONE == m_pThisLocation->getLocationRole ())
    {
        pFrameworkObjectManagerResetNodeToUnconfirmRole->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pFrameworkObjectManagerResetNodeToUnconfirmRole);

        return;
    }

    if (true == PrismFrameworkObjectManager::getSecondaryNodeClusterCreationFlag ())
    {
        pFrameworkObjectManagerResetNodeToUnconfirmRole->setCompletionStatus (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS);
        reply (pFrameworkObjectManagerResetNodeToUnconfirmRole);
        
        return;
    }    

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::stopHeartBeatToNode),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::disconnectFromAllNodes),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::configureNodeForResetAndStartServices),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::triggerUncontrolledFailoverForRemainingNodes),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFrameworkObjectManagerResetNodeToUnconfirmRole, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();

}

void PrismFrameworkObjectManager::triggerUncontrolledFailoverForRemainingNodes (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    vector<LocationId> failedLocationIds;
    LocationId         thisLocationId = FrameworkToolKit::getThisLocationId ();

    FrameworkToolKit::getKnownLocations (failedLocationIds);

    PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext = NULL;


    pPrismFrameworkFailoverWorkerContext = new PrismFrameworkFailoverWorkerContext (this, reinterpret_cast<PrismAsynchronousCallback> (&PrismFrameworkObjectManager::triggerUncontrolledFailoverForRemainingNodesCallback), pPrismLinearSequencerContext);

    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);
    prismAssert (NULL != m_pPrismFrameworkFailoverWorker, __FILE__, __LINE__);

    pPrismFrameworkFailoverWorkerContext->setThisLocationRole (LOCATION_PRIMARY);

    pPrismFrameworkFailoverWorkerContext->setFailoverReason (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

    UI32 numFailedLocationIds = failedLocationIds.size ();

    for (UI32 i = 0; i < numFailedLocationIds; ++i) 
    {    
      pPrismFrameworkFailoverWorkerContext->addFailedLocationId (failedLocationIds[i]);
      prismAssert (failedLocationIds[i] != thisLocationId, __FILE__, __LINE__);
    }    
    
    m_pPrismFrameworkFailoverWorker->executeFailover (pPrismFrameworkFailoverWorkerContext);
}

void PrismFrameworkObjectManager::triggerUncontrolledFailoverForRemainingNodesCallback (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext)
{
    prismAssert (NULL != pPrismFrameworkFailoverWorkerContext, __FILE__, __LINE__);

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismFrameworkFailoverWorkerContext->getPCallerContext ());
    ResourceId                   status                       = pPrismFrameworkFailoverWorkerContext->getCompletionStatus ();

    delete pPrismFrameworkFailoverWorkerContext;

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    pPrismLinearSequencerContext->executeNextStep (status);
}


void PrismFrameworkObjectManager::configureNodeForResetAndStartServices (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::configureNodeForResetAndStartServices Location Role is ") + FrameworkToolKit::localize (m_pThisLocation->getLocationRole ()));

    if (LOCATION_PRIMARY == m_pThisLocation->getLocationRole ())
    {
        m_pThisLocation->setLocationRole (LOCATION_PRIMARY_UNCONFIRMED);
    }
    else if (LOCATION_SECONDARY == m_pThisLocation->getLocationRole ())
    {
        status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_RESET_NODE_TO_UNCONFIRM_ROLE);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::configureNodeForResetAndStartServices : Start of Service should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else if (LOCATION_STAND_ALONE == m_pThisLocation->getLocationRole ())
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::disconnectFromAllNodes (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::disconnectFromAllNodes Entering....");

    disconnectFromAllConnectedNodes ();

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::disconnectFromAllConnectedNodes ()
{
    vector<LocationId> connectedLocationsVector;

    FrameworkToolKit::getConnectedLocations (connectedLocationsVector);

    UI32         numberOfConnectedLocations  = connectedLocationsVector.size ();
    LocationId   locationId                  = 0;

    for (UI32 i = 0; i < numberOfConnectedLocations; i++)
    {
        locationId = connectedLocationsVector [i];
        disconnectFromLocation (locationId);
    }
}

void PrismFrameworkObjectManager::stopHeartBeatToNode (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    LocationRole locationRole = m_pThisLocation->getLocationRole ();
    LocationId   locationId   = 0;
    string       ipAddress    = "";
    SI32         port         = 0;
    ResourceId  status        = WAVE_MESSAGE_SUCCESS;
    vector<LocationId> currentlyKnownLocations;
    UI32               numberOfCurrentlyKnownLocations = 0;

    if (LOCATION_PRIMARY == locationRole)
    {
        // stop heartbeat to all the connected nodes
        vector<LocationId> connectedLocationsVector;
        FrameworkToolKit::getKnownLocations (currentlyKnownLocations);
        numberOfCurrentlyKnownLocations = currentlyKnownLocations.size ();

        for (UI32 i = 0; i < numberOfCurrentlyKnownLocations; i++)
        {
            locationId = currentlyKnownLocations [i];
            ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
            port       = FrameworkToolKit::getPortForLocationId      (locationId);

            StopHeartBeatMessage stopHeartBeatMessage;
            IpV4Address          ip (ipAddress);

            stopHeartBeatMessage.setDstIpAddress       (ip);
            stopHeartBeatMessage.setDstPortNumber      (port);

            status = sendSynchronously (&stopHeartBeatMessage);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::stopHeartBeatToNode : " + ipAddress + ":" + port + ", Error = " + FrameworkToolKit::localize (status));
            }

            status = stopHeartBeatMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::stopHeartBeatToNode : Could not stop Heart Beat to " + ipAddress + ":" + port + ", Error = " + FrameworkToolKit::localize (status));         
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::stopHeartBeatToNode :  For Node Reset : stopped Heart Beat to " + ipAddress + ":" + port);
            }
        }
    }
    else if (LOCATION_SECONDARY == locationRole)
    {
        // stop hearbeat to primary
        locationId = m_pThisLocation->getClusterPrimaryLocationId ();
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId      (locationId);

        StopHeartBeatMessage stopHeartBeatMessage;
        IpV4Address          ip (ipAddress);

        stopHeartBeatMessage.setDstIpAddress       (ip);
        stopHeartBeatMessage.setDstPortNumber      (port);

        status = sendSynchronously (&stopHeartBeatMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManager::stopHeartBeatToNode : " + ipAddress + ":" + port + ", Error = " + FrameworkToolKit::localize (status));
        }

        status = stopHeartBeatMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::stopHeartBeatToNode : Could not stop Heart Beat to Primary " + ipAddress + ":" + port + ", Error = " + FrameworkToolKit::localize (status));
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::stopHeartBeatToNode :  For Node Reset : stopped Heart Beat to Primary " + ipAddress + ":" + port);
        }
    }

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

bool PrismFrameworkObjectManager::getIsPostBootNeededDuringRejoin ()
{
    return (m_isPostBootNeededDuringRejoin);
}

void PrismFrameworkObjectManager::setIsPostBootNeededDuringRejoin (bool isPostBootNeededDuringRejoin)
{
    m_isPostBootNeededDuringRejoin = isPostBootNeededDuringRejoin;
}

bool PrismFrameworkObjectManager::getNeedNotifyClusterReadyState ()
{
    return (m_needNotifyClusterReadyState);
}

void PrismFrameworkObjectManager::setNeedNotifyClusterReadyState (bool needNotifyClusterReadyState)
{
    m_needNotifyClusterReadyState = needNotifyClusterReadyState;
}

void PrismFrameworkObjectManager::removeFailedLocationsFromKnownLocationsHandler (FrameworkObjectManagerRemoveKnownLocationsMessage *pFrameworkObjectManagerRemoveKnownLocationsMessage)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::removeFailedLocationsFromKnownLocationsHandler : entering");

    vector<LocationId> failedLocations      = pFrameworkObjectManagerRemoveKnownLocationsMessage->getFailedLocationIds ();
    UI32               numOfFailedLocations = failedLocations.size ();

    prismAssert (0 != numOfFailedLocations, __FILE__, __LINE__);

    trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::removeFailedLocationsFromKnownLocationsHandler : numOfFailedLocations = ") + numOfFailedLocations);

    for (UI32 i = 0; i < numOfFailedLocations; i++)
    {
        LocationId failedLocation = failedLocations[i];
        trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::removeFailedLocationsFromKnownLocationsHandler : failedLocation = ") + failedLocation);
        removeKnownLocation (failedLocation);
    } 

    pFrameworkObjectManagerRemoveKnownLocationsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    FrameworkToolKit::savePrismConfiguration(true);

    reply (pFrameworkObjectManagerRemoveKnownLocationsMessage);
}

void PrismFrameworkObjectManager::removePreviousDatabaseBackupFile (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    string commandToRemoveDatabaseBackupFile = string ("rm -rf " + FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ());

    system (commandToRemoveDatabaseBackupFile.c_str ());

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::setGetInstancesFunction (GetInstancesFunction getInstancesFunction)
{
    m_getInstancesFunction  = getInstancesFunction;
}


void PrismFrameworkObjectManager::getInstances (vector<SI32> &connectedInstanceVector)
{
    trace (TRACE_LEVEL_DEBUG, ("PrismFrameworkObjectManager:: getInstances"));
    
    prismAssert (NULL != m_getInstancesFunction, __FILE__, __LINE__);

    if (NULL != m_getInstancesFunction)
    {
         (*m_getInstancesFunction) (connectedInstanceVector);
    }
}

void PrismFrameworkObjectManager::setGetInstancesAndSubInstancesFunction (GetInstancesAndSubInstancesFunction getInstancesAndSubInstancesFunction)
{
    m_getInstancesAndSubInstancesFunction = getInstancesAndSubInstancesFunction;
}

void PrismFrameworkObjectManager::getInstancesAndSubInstances (vector<SI32> &connectedInstanceVector, vector<SI32> &connectedSubInstanceVector)
{
    trace (TRACE_LEVEL_DEBUG, ("PrismFrameworkObjectManager:: getInstancesAndSubInstances"));

    prismAssert (NULL != m_getInstancesAndSubInstancesFunction, __FILE__, __LINE__);

    if (NULL != m_getInstancesAndSubInstancesFunction)
    {
         (*m_getInstancesAndSubInstancesFunction) (connectedInstanceVector, connectedSubInstanceVector);
    }
}

void PrismFrameworkObjectManager::setGetThisSlotInstanceFunction (GetThisSlotInstanceFunction getThisSlotInstanceFunction)
{
    m_getThisSlotInstanceFunction  = getThisSlotInstanceFunction;
}

SI32 PrismFrameworkObjectManager::getThisSlotInstance ()
{
    SI32 instnace = 0;
    trace (TRACE_LEVEL_INFO, ("PrismFrameworkObjectManager:: getSlotInstance"));
    
    prismAssert (NULL != m_getThisSlotInstanceFunction, __FILE__, __LINE__);

    if (NULL != m_getThisSlotInstanceFunction)
    {
         instnace = (*m_getThisSlotInstanceFunction) ();
    }
    return (instnace);
}

void PrismFrameworkObjectManager::setCreateDummySlotFunction (CreateDummySlotFunction createDummySlotFunction)
{
    m_createDummySlotFunction = createDummySlotFunction;
}

void PrismFrameworkObjectManager::createDummySlot (const UI32 slotNumber)
{
    trace (TRACE_LEVEL_INFO, ("PrismFrameworkObjectManager:: createDummySlot"));

    prismAssert (NULL != m_createDummySlotFunction, __FILE__, __LINE__);

    //Create a dummy slot
    (*m_createDummySlotFunction) (slotNumber);
}

void PrismFrameworkObjectManager::disconnectFromAllInstanceClientsHandler (FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage *pFrameworkObjectManagerDisconnectFromAllInstanceClientsMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::disconnectAllInstanceClientsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFrameworkObjectManagerDisconnectFromAllInstanceClientsMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void PrismFrameworkObjectManager::disconnectAllInstanceClientsStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
   FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage *pFrameworkObjectManagerDisconnectFromAllInstanceClientsMessage = dynamic_cast<FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage *>(pPrismLinearSequencerContext->getPPrismMessage());

    string     clientIpAddress = pFrameworkObjectManagerDisconnectFromAllInstanceClientsMessage->getClientIpAddress();

    FrameworkToolKit::disconnectFromAllInstanceClients (clientIpAddress);

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::disconnectAllInstanceClientsStep: ")+clientIpAddress);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

// ZEROIZE_FOR_FIPS
void PrismFrameworkObjectManager::zeroizeForFIPSMessageHandler(ZeroizeForFIPSMessage *pMessage)
{

    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::zeroizeForFIPSMessageHandler: processing FIPSMEssage");
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::validateStandaloneStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::notifyAllClientSessionsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::triggerFIPSZeroizeStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::zeroizeSuccessStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFrameworkObjectManager::prismLinearSequencerFailedStep),
    };
    
    ZeroizeForFIPSLinearSequencerContext *pContext = new ZeroizeForFIPSLinearSequencerContext (pMessage, (PrismElement*)this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    
    pContext->holdAll ();
    pContext->start (); 
    return; 
}


void PrismFrameworkObjectManager::validateStandaloneStep(ZeroizeForFIPSLinearSequencerContext *pContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    LocationRole  thisLocationRole = FrameworkToolKit::getThisLocationRole ();

    if ((LOCATION_STAND_ALONE != thisLocationRole) &&
        (LOCATION_PRIMARY != thisLocationRole) &&
        (LOCATION_SECONDARY != thisLocationRole) &&
        (LOCATION_PRIMARY_UNCONFIRMED != thisLocationRole) &&
        (LOCATION_SECONDARY_UNCONFIRMED != thisLocationRole))
    {
        status = FIPS_OPERATION_ZEROIZE_FAILED_LOCATION_NOT_READY;

        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::validateStandaloneStep : thisLocationRole is [") + FrameworkToolKit::localizeToSourceCodeEnum (thisLocationRole) + string ("]. So, failing Zeroize operation. Returning [") + FrameworkToolKit::localizeToSourceCodeEnum (status) + string ("]."));
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::validateStandaloneStep : thisLocationRole is [") + FrameworkToolKit::localizeToSourceCodeEnum (thisLocationRole) + string ("]"));
    }

    if(true == isNodeZeroized() )
    {
        status = FIPS_OPERATION_ZEROIZE_FAILED_ALREADY_ZEROIZED;
        Wave::logOperationStatus (FIPS_OPERATION_ZEROIZE_FAILED_ALREADY_ZEROIZED);

        trace (TRACE_LEVEL_ERROR, string ("PrismFrameworkObjectManager::validateStandaloneStep : FIPS_OPERATION_ZEROIZE_FAILED_ALREADY_ZEROIZED"));
    }
    else
    {
        setNodeZeroized(true);

        // Zeroize is to be supported in FC mode.
        // i.e. It is to be allowed when cluster exists / does not exist.
        // So, commenting follwong "Wave::isWaveClusteringEnabled ()" check.
        // May need to update this in case of logical chassis.
        // Should a RASLOG be added in place of "FIPS_OPERATION_CONFIRMED_NODE_AS_STANDALONE" ?

#if 0
        if (Wave::isWaveClusteringEnabled())
        {
            status = FIPS_OPERATION_FAILED_FOR_NODE_NOT_STANDALONE;
            Wave::logOperationStatus (FIPS_OPERATION_FAILED_FOR_NODE_NOT_STANDALONE);
            setNodeZeroized(false);
        }
        else
        {
            Wave::logOperationStatus (FIPS_OPERATION_CONFIRMED_NODE_AS_STANDALONE);
        }
#endif
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::validateStandaloneStep : validation successful.");
    }
    pContext->executeNextStep(status);
}

void PrismFrameworkObjectManager::notifyAllClientSessionsStep (ZeroizeForFIPSLinearSequencerContext *pContext)
{
    
    vector<string> commandOutput;
    string          cmd = "/bin/touch " + PrismFrameworkObjectManager::getFIPSZeroizeFile();
    FrameworkToolKit::systemCommandOutput (cmd, commandOutput);
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::notifyAllClientSessionsStep : Beginning System Call SYNC");
    sync ();
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::notifyAllClientSessionsStep : Ending System Call SYNC");

    /* Rotate trc file so that only zeroize logs are present.
     */
    TraceObjectManager::rotateTraceFile (true);
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::notifyAllClientSessionsStep : rotateTraceFile");
    
    /* Prompt all user session about zeroization */

    bool clusterEnabled = Wave::clusterEnabledCheck ();

    if (false == clusterEnabled)
    {
        // In FC mode, print warning only from Dcmd.

        WaveClientSessionContext tempWaveClientSessionContext;
        tempWaveClientSessionContext.setWaveClientOriginatingLocationId(FrameworkToolKit::getThisLocationId());
        printfToAllWaveClientSessions(tempWaveClientSessionContext,"%s%s%s%s",
        "\n",
        "**********************************************************************\n",
        "System is Zeroizing.. Logout immediately.\n",
        "**********************************************************************\n");

        Wave::logOperationStatus (FIPS_OPERATION_NOTIFY_SESSIONS_ABT_ZEROIZE);
    }

    pContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::triggerFIPSZeroizeStep (ZeroizeForFIPSLinearSequencerContext *pContext)
{

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    trace(TRACE_LEVEL_INFO,"PrismFrameworkObjectManager::triggerFIPSZeroizeStep: Entered\n");
    Wave::logOperationStatus (FIPS_OPERATION_TRIGGER_CLEANUP_FOR_ZEROIZE);

    FrameworkSequenceGenerator &frameworkSequenceGenerator = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();
    PrismZeroizeAgent *pZAgent = new PrismZeroizeAgent(this, frameworkSequenceGenerator);

    status = pZAgent->execute();
    if(status != WAVE_MESSAGE_SUCCESS)
    {
        trace(TRACE_LEVEL_FATAL,"PrismFrameworkObjectManager::triggerFIPSZeroizeStep: one or more services failed to zeroize\n");
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::triggerFIPSZeroizeStep : successfully executed PrismZeroizeAgent.");

        Wave::logOperationStatus (FIPS_OPERATION_ZEROIZE_SUCCESS);
    }
    pContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::zeroizeSuccessStep (ZeroizeForFIPSLinearSequencerContext *pZeroizeForFIPSLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::zeroizeSuccessStep.");

    PrismFrameworkObjectManager::prismLinearSequencerSucceededStep (pZeroizeForFIPSLinearSequencerContext);

    UI32 numberOfIterations = 0;

    while (1)
    {
        if (0 == numberOfIterations % 10)
        {
            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::zeroizeSuccessStep : waiting for the switch to reboot. [") + numberOfIterations + string ("]"));
        }

        if (numberOfIterations < 900)
        {
            ++numberOfIterations;
            prismSleep (1);
        }
        else
        {
            // In Dcmd, need to wait until Ccmd zeroize is over.
            // Is this assert required ? Or, should the system be kept running ?
            WaveNs::trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::zeroizeSuccessStep : after zeroize was successful, 15 minutes are over. System has not rebooted. So Dcmd/Ccmd is forcefully asserting thus causing the system to reboot");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }
}

string PrismFrameworkObjectManager::getFIPSZeroizeFile()
{
    return (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + ".FIPSZeroizing");
}

void PrismFrameworkObjectManager::disconnectFromAllKnownLocations ()
{

    (PrismFrameworkObjectManager::getInstance())->disconnectFromAllConnectedNodes ();
    
    return;
}

void PrismFrameworkObjectManager::getFullyConnectedLocations (vector<LocationId> &connectedLocationsVector)
{
    if (NULL != m_pThisLocation)
    {
        m_pThisLocation->getFullyConnectedLocations (connectedLocationsVector);
    }
}

void PrismFrameworkObjectManager::rollbackCfgFile ()
{
    LocationBase *pThisLocation         = (PrismFrameworkObjectManager::getInstance ())->getThisLocation ();

    string        thisLocationIpAddress = FrameworkToolKit::getThisLocationIpAddress ();
    UI32          thisNodePort          = (UI32) FrameworkToolKit::getThisLocationPort ();

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext(WaveNode::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (thisLocationIpAddress, "ipAddress"));
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (thisNodePort, "port"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

//    prismAssert (1 == pResults->size (), __FILE__, __LINE__);

    if (1 == pResults->size ())
    {
        WaveNode *pThisNode = dynamic_cast<WaveNode *> ((*pResults)[0]);

        prismAssert (NULL != pThisNode, __FILE__, __LINE__);

        LocationId locationId = pThisNode->getLocationId ();

        pThisLocation->setLocationId   (locationId);
        pThisLocation->setClusterPrimaryLocationId (locationId);
        pThisLocation->setClusterPrimaryPort (thisNodePort);
        FrameworkToolKit::setLastUsedLocationId (locationId);

        delete pThisNode;

        pResults->clear ();

        delete pResults;

        pResults = NULL;

        pResults = querySynchronously (PrismCluster::getClassName ());

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        if (0 == pResults->size ())
        {
            pThisLocation->setLocationRole (LOCATION_STAND_ALONE);
        }
    }
    else if (0 == pResults->size ())
    {
        trace (TRACE_LEVEL_WARN, "Database is in inconsistent state, Query For WaveNode returns 0 WaveNode");
        trace (TRACE_LEVEL_WARN, "Going For Extreme Recovery");

        ResourceId status = FrameworkToolKit::changePrismConfigurationValidity ( false );
        if ( status != WAVE_MESSAGE_SUCCESS  )
        {
            trace (TRACE_LEVEL_FATAL, ("PrismFrameworkObjectManager::rollbackCfgFile : Removing CFG file failed in extreme Recoverycase "));        
        }

        system ("/sbin/reboot -f");

        sleep (300);

        trace (TRACE_LEVEL_FATAL, ("System is not rebooting, Manual recovery is required"));

        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "Very Inconsistent State, Asserting");
        prismAssert (false, __FILE__, __LINE__);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    if (LOCATION_STAND_ALONE == (pThisLocation->getLocationRole ()))
    {
        // clear known location if any
        vector<LocationId>  knownRemoteLocations;
        UI32                numberOfKnownRemoteLocations = 0;

        pThisLocation->getKnownRemoteLocations (knownRemoteLocations);

        numberOfKnownRemoteLocations = knownRemoteLocations.size ();

        for (UI32 i = 0; i < numberOfKnownRemoteLocations; i++)
        {
            pThisLocation->removeKnownLocation (knownRemoteLocations[i]);
        }
    }
}

void PrismFrameworkObjectManager::setRaslogAssertFunction (RaslogAssertFunction raslogAssertFunction)
{
    m_raslogAssertFunction  = raslogAssertFunction;
}

void PrismFrameworkObjectManager::raslogAssert (const string &file, const string &function, const UI32 line, const string &assertString)
{
    if (NULL != m_raslogAssertFunction)
        (*m_raslogAssertFunction) (file, function, line, assertString);
}

void PrismFrameworkObjectManager::prepareNodeForHASupportHandler ()
{
    ResourceId status = m_pPrismFinalizeWorker->shutdownPrismServices ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::prepareNodeForHASupportHandler : shutdown should Never Fail");
        prismAssert (false, __FILE__, __LINE__);
    }

    status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PREPARE_FOR_HA_BOOT);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::prepareNodeForHASupportHandler Boot of Services should Never Fail In this case");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void PrismFrameworkObjectManager::setGetFirmwareVersionFunction (GetFirmwareVersionFunction getFirmwareVersionFunction)
{
    m_getFirmwareVersionFunction = getFirmwareVersionFunction;
}

string PrismFrameworkObjectManager::getFirmwareVersion ()
{
    if (NULL != m_getFirmwareVersionFunction)
    {
        return ((*m_getFirmwareVersionFunction) ());
    }
    return ("");
}

void PrismFrameworkObjectManager::setHandleDcmFssConfigUpdateFunction (HandleDcmFssConfigUpdateFunction handleDcmFssConfigUpdateFunction)
{
    m_handleDcmFssConfigUpdateFunction = handleDcmFssConfigUpdateFunction;
}

int PrismFrameworkObjectManager::handleDcmFssConfigUpdate (void *dcmConfigData)
{
    if (NULL != m_handleDcmFssConfigUpdateFunction)
    {
        return ((*m_handleDcmFssConfigUpdateFunction) (dcmConfigData));
    }
    return (0);
}

void PrismFrameworkObjectManager::setSetVcsClusterConfigHandlerFunction (SetVcsClusterConfigHandlerFunction setVcsClusterConfigHandlerFunction)
{
    m_setVcsClusterConfigHandlerFunction = setVcsClusterConfigHandlerFunction;
}

ResourceId PrismFrameworkObjectManager::setVcsClusterConfigData (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context)
{
    if (NULL != m_setVcsClusterConfigHandlerFunction)
    {
        return ((*m_setVcsClusterConfigHandlerFunction) (pDcmVcsClusterConfigData, context));
    }
    return (WAVE_MESSAGE_ERROR);
}

void PrismFrameworkObjectManager::setGetVcsClusterConfigHandlerFunction (GetVcsClusterConfigHandlerFunction getVcsClusterConfigHandlerFunction)
{
    m_getVcsClusterConfigHandlerFunction = getVcsClusterConfigHandlerFunction;
}

void PrismFrameworkObjectManager::getVcsClusterConfigData (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context)
{
    if (NULL != m_getVcsClusterConfigHandlerFunction)
    {
        return ((*m_getVcsClusterConfigHandlerFunction) (pDcmVcsClusterConfigData, context));
    }
}

void PrismFrameworkObjectManager::setNotifySyncUpdateFailureFunction (NotifySyncUpdateFailureFunction notifySyncUpdateFailureFunction)
{
    m_notifySyncUpdateFailureFunction = notifySyncUpdateFailureFunction;
}

bool PrismFrameworkObjectManager::notifySyncUpdateFailure (const ResourceId &reason)
{
    if (NULL != m_notifySyncUpdateFailureFunction)
    {
        return ((*m_notifySyncUpdateFailureFunction) (reason));
    }
    return (false);
}

void PrismFrameworkObjectManager::setNotifyClusterReadyStateFunction (NotifyClusterReadyStateFunction notifyClusterReadyStateFunction)
{
    m_notifyClusterReadyStateFunction = notifyClusterReadyStateFunction;
}

void PrismFrameworkObjectManager::notifyClusterReadyState (bool &readyState)
{
    if (NULL != m_notifyClusterReadyStateFunction)
    {
        return ((*m_notifyClusterReadyStateFunction) (readyState));
    }
    return ;
}

bool PrismFrameworkObjectManager::isExternalStateSynchronizationRequired (PrismServiceId prismServiceId)
{
    m_externalStateSynchronizationRequiredListLock.lock();

    for (UI32 i = 0; i < m_externalStateSynchronizationRequiredList.size(); ++i)
    {
        if (prismServiceId == m_externalStateSynchronizationRequiredList[i])
        {
            m_externalStateSynchronizationRequiredListLock.unlock ();
            return true;
        }
    }
    
    m_externalStateSynchronizationRequiredListLock.unlock ();
    return false;
}

void  PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (PrismServiceId prismServiceId)
{
    if (isExternalStateSynchronizationRequired (prismServiceId))
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string("PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList prismServiceId ")+prismServiceId+ "already in state synchronization required list, request ignored");
    }
    else
    {
        m_externalStateSynchronizationRequiredListLock.lock();
        m_externalStateSynchronizationRequiredList.push_back (prismServiceId);
        m_externalStateSynchronizationRequiredListLock.unlock ();
        WaveNs::trace (TRACE_LEVEL_DEVEL, string ("PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList:  ")+ FrameworkToolKit::getServiceNameById (prismServiceId)+ " SID: "+ prismServiceId+" added to external synchronization required list");
    }
}


bool PrismFrameworkObjectManager::getIsSyncDumpCompleted ( )
{
    return (m_pPrismFrameworkObjectManagerHaSyncWorker->getSyncDumpComplete ());
}

UI32 PrismFrameworkObjectManager::getSyncState ()
{
    return (m_pPrismFrameworkObjectManagerHaSyncWorker->getSyncState ());
}

void PrismFrameworkObjectManager::setSyncState (const UI32 &syncState)
{
    return (m_pPrismFrameworkObjectManagerHaSyncWorker->setSyncState (syncState));
}

UI32 PrismFrameworkObjectManager::getStandbySyncState ()
{
    return (m_pPrismFrameworkObjectManagerHaSyncWorker->getStandbySyncState ());
}

ResourceId PrismFrameworkObjectManager::upgradeDatabase ()
{
    PrismFrameworkConfiguration prismFrameworkConfiguration;
    bool ignoreConfigFile = true;
    ResourceId status = getPInitializeWorker ()->performSchemaConversion(prismFrameworkConfiguration, ignoreConfigFile);;
    if (FRAMEWORK_SUCCESS == status)
    {
        getPInitializeWorker ()->populateSchemaUpgradeTableInDb ();
    }
    return status;
}

void PrismFrameworkObjectManager::prepareNodeForWarmRecoveryWithDefaultConfiguration (FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration *pFrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration)
{
    prepareNodeForHASupportHandler ();

    pFrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pFrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration);
}

void PrismFrameworkObjectManager::storeConfigurationIntentMessageHandler (FrameworkObjectManagerStoreConfigurationIntentMessage *pFrameworkObjectManagerStoreConfigurationIntentMessage)
{
    //trace (TRACE_LEVEL_DEVEL, string ("PrismFrameworkObjectManager::storeConfigurationIntentMessageHandler : Entering ..."));

    UI32    configurationIntentMessageId            = pFrameworkObjectManagerStoreConfigurationIntentMessage->getConfigurationIntentMessageId ();
    UI32    configurationIntentBufferSize           = 0;
    char   *configurationIntentBuffer               = reinterpret_cast<char *> (pFrameworkObjectManagerStoreConfigurationIntentMessage->findBuffer (SERIALIZED_CONFIGURATION_INTENT_BUFFER, configurationIntentBufferSize));
    string  configurationIntentSerializedMessage    (configurationIntentBuffer, configurationIntentBufferSize);

    //trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::storeConfigurationIntentMessageHandler : Configuration intent message id : ") + configurationIntentMessageId + string (", Configuration intent serialized message : ") + configurationIntentSerializedMessage + string (", serialized buffer size : ") + configurationIntentBufferSize + ", serialized message size : " + configurationIntentSerializedMessage.size ());

    WaveConfigurationIntentRepository::addConfigurationIntent (configurationIntentMessageId, configurationIntentSerializedMessage);

    pFrameworkObjectManagerStoreConfigurationIntentMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pFrameworkObjectManagerStoreConfigurationIntentMessage);
}

void PrismFrameworkObjectManager::removeConfigurationIntentMessageHandler (FrameworkObjectManagerRemoveConfigurationIntentMessage *pFrameworkObjectManagerRemoveConfigurationIntentMessage)
{
    //trace (TRACE_LEVEL_DEVEL, string ("PrismFrameworkObjectManager::removeConfigurationIntentMessageHandler : Entering ..."));

    UI32    configurationIntentMessageId    = pFrameworkObjectManagerRemoveConfigurationIntentMessage->getConfigurationIntentMessageId ();

    //trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::removeConfigurationIntentMessageHandler : Configuration intent message id : ") + configurationIntentMessageId);

    WaveConfigurationIntentRepository::removeConfigurationIntent (configurationIntentMessageId);

    pFrameworkObjectManagerRemoveConfigurationIntentMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pFrameworkObjectManagerRemoveConfigurationIntentMessage);
}

void PrismFrameworkObjectManager::storeConfigurationIntentStaticMessageHandler (FrameworkObjectManagerStoreConfigurationIntentMessage *pFrameworkObjectManagerStoreConfigurationIntentMessage)
{
    //WaveNs::trace (TRACE_LEVEL_DEVEL, string ("PrismFrameworkObjectManager::storeConfigurationIntentStaticMessageHandler : Entering ..."));

    UI32    configurationIntentMessageId            = pFrameworkObjectManagerStoreConfigurationIntentMessage->getConfigurationIntentMessageId ();
    UI32    configurationIntentBufferSize           = 0;
    char   *configurationIntentBuffer               = reinterpret_cast<char *> (pFrameworkObjectManagerStoreConfigurationIntentMessage->findBuffer (SERIALIZED_CONFIGURATION_INTENT_BUFFER, configurationIntentBufferSize));
    string  configurationIntentSerializedMessage    (configurationIntentBuffer, configurationIntentBufferSize);

    //WaveNs::trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::storeConfigurationIntentStaticMessageHandler : Configuration intent message id : ") + configurationIntentMessageId + string (", Configuration intent serialized message : ") + configurationIntentSerializedMessage + string (", serialized buffer size : ") + configurationIntentBufferSize + ", serialized message size : " + configurationIntentSerializedMessage.size ());

    WaveConfigurationIntentRepository::addConfigurationIntent (configurationIntentMessageId, configurationIntentSerializedMessage);   

    pFrameworkObjectManagerStoreConfigurationIntentMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::removeConfigurationIntentStaticMessageHandler (FrameworkObjectManagerRemoveConfigurationIntentMessage *pFrameworkObjectManagerRemoveConfigurationIntentMessage)
{
    //WaveNs::trace (TRACE_LEVEL_DEVEL, string ("PrismFrameworkObjectManager::removeConfigurationIntentStaticMessageHandler : Entering ..."));

    //UI32    configurationIntentMessageId    = pFrameworkObjectManagerRemoveConfigurationIntentMessage->getConfigurationIntentMessageId ();

    //WaveNs::trace (TRACE_LEVEL_DEBUG, string ("PrismFrameworkObjectManager::removeConfigurationIntentStaticMessageHandler : Configuration intent message id : ") + configurationIntentMessageId);

    //WaveConfigurationIntentRepository::removeConfigurationIntent (configurationIntentMessageId);

    // For now we will always clear the entire intent repository since we currently only allowing a single intent to be replayed (the last stored intent).  Change this when we support batched configurations.

    WaveConfigurationIntentRepository::clearConfigurationIntentRepository ();

    pFrameworkObjectManagerRemoveConfigurationIntentMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::prepareStandbyAfterClusterPhaseSync (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context)
{
    trace (TRACE_LEVEL_INFO, "PrismFrameworkObjectManager::prepareStandbyAfterClusterPhaseSync (STANDBY)");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_HASTANDBY, WAVE_BOOT_PHASE_POST_PHASE);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::prepareStandbyAfterClusterPhaseSync: Post phase failed.");
        prismAssert (false, __FILE__, __LINE__);
    }

    pSecondaryNodeClusterPhase3Context->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFrameworkObjectManager::prepareFrameworkForHaRecoveryMessageHandler (FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration *pFrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration)
{
    FrameworkToolKit::setSyncFailureNotified (false);

    LocationRole locationRole = m_pThisLocation->getLocationRole ();

    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::prepareFrameworkForHaRecoveryMessageHandler with location role") + FrameworkToolKit::localize (locationRole));

    // If standby MM has the DB synced from active MM, always use running DB for failover.
    // In case DB is not in sync with active MM, HASM/FSS should trigger chassis reboot instead of failover
    // This covers the warm recovery and cold recovery with running config synced from active MM.
    // If DB was nevered synced to standby MM, and in case of active MM removal, standby MM reboot with
    // startup config file synced from active MM in case FC mode, and default config in MC mode with cold recovery

    if (getStandbySyncState () == IN_SYNC)
    {
        bool isWarmRecoveryInvolved = pFrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration->isWarmHaRecoveryPreparationInProgress ();

        setWarmHaRecoveryPreparationInProgress (isWarmRecoveryInvolved);

        ResourceId status = m_pPrismFinalizeWorker->shutdownPrismServices (WAVE_SHUTDOWN_SECONDARY_REJOIN);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::prepareNodeForHASupportHandler : shutdown should Never Fail");
            prismAssert (false, __FILE__, __LINE__);
        }

        status = m_pInitializeWorker->startPrismServices (WAVE_BOOT_PERSISTENT, WAVE_BOOT_PREPARE_FOR_HA_FAILOVER_PHASE);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFrameworkObjectManager::prepareNodeForHASupportHandler Boot of Services should Never Fail In this case");
            prismAssert (false, __FILE__, __LINE__);
        } 

        setWarmHaRecoveryPreparationInProgress (false);
    }
    else
    {
        prepareNodeForHASupportHandler ();
    }

    PrismFrameworkObjectManager::setSecondaryNodeClusterCreationFlag (false);
    pFrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pFrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration);
}

void PrismFrameworkObjectManager::resumePostponedMessages ()
{
    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::resumePostponedMessages: Resuming all the pending messages"));
    resumeAllPostponedMessages ();
}

ClientStreamingSocket* PrismFrameworkObjectManager::getClientStreamingSocketForRemoteLocation (LocationId locationId)
{
    if (NULL != m_pThisLocation)
    {
        return (m_pThisLocation->getClientStreamingSocketForRemoteLocation (locationId));
    }

    return (NULL);
}

void PrismFrameworkObjectManager::replayConfigFileMessageHandler (PrismFrameworkReplayConfigMessage *pPrismFrameworkReplayConfigMessage)
{
    string      configFilePath          = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
    string      globalConfigFilename    = configFilePath + "/" + ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ();
    string      localConfigFilename     = configFilePath + "/" + ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();
    string      preConfigReplayCommands = "";
    ResourceId  status                  = WAVE_MESSAGE_SUCCESS;

    //replay global config file
    if (true == ConfigFileManagementToolKit::isFileExisting (globalConfigFilename))
    {
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::replayConfigFileIfRequired : Replay \"") + globalConfigFilename+ "\" config file on principal node...");
 
        status = ConfigFileManagementToolKit::replayConfigurationFile (configFilePath, ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename (), preConfigReplayCommands);
 
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::replayConfigFileIfRequired : Replay of \"") + globalConfigFilename + "\" config file on principal node completed with status: " + FrameworkToolKit::localize (status));
 
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            string newConfigFilename = globalConfigFilename + "." + FrameworkToolKit::getThisLocationIpAddress () + ".backup";
 
            ConfigFileManagementToolKit::renameConfigurationFile (globalConfigFilename, newConfigFilename);
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::replayConfigFileIfRequired : Skipping this step. ") + globalConfigFilename + " does not exist.");
    }

    //replay local Config file
    if (true == ConfigFileManagementToolKit::isFileExisting (localConfigFilename))
    {
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::replayConfigFileIfRequired : Replay \"") + localConfigFilename + "\" config file on principal node...");
 
        status = ConfigFileManagementToolKit::replayConfigurationFile (configFilePath, ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename (), preConfigReplayCommands);
            trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::replayConfigFileIfRequired : Replay of \"") + localConfigFilename + "\" config file on principal node completed with status: " + FrameworkToolKit::localize (status));    
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            string newConfigFilename = localConfigFilename + "." + FrameworkToolKit::getThisLocationIpAddress () + ".backup";
            
            ConfigFileManagementToolKit::renameConfigurationFile (localConfigFilename, newConfigFilename);
        }   
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::replayConfigFileIfRequired : Skipping this step. ") + localConfigFilename + " does not exist.");
    }


    pPrismFrameworkReplayConfigMessage->setCompletionStatus (status);
    reply (pPrismFrameworkReplayConfigMessage);
}

void PrismFrameworkObjectManager::resetFrameworkConfigurationToDefault ()
{
    if (NULL == m_pThisLocation)
    {   
        trace (TRACE_LEVEL_ERROR, "RecoverPrismBootAgent::updateFrameworkConfigurationToDefaultStep: Location config cannot be null");
        prismAssert (false, __FILE__, __LINE__);
    }

    LocationId locationId = (UI32) (1 << 8);

    m_pThisLocation->resetLocationToStandAlone   (locationId);
    m_pThisLocation->removeAllKnownLocations     ();
    m_pThisLocation->removeAllSubLocations       ();
    initializeLastUsedLocationId                 ();// resets last used location Id to 256
    setIsStartupValid                            (false);// This will be used to replay the startup file

}

string PrismFrameworkObjectManager::getGlobalConfigurationFileName ()
{
  return (s_frameworkGlobalConfigurationFile) ;
}

string PrismFrameworkObjectManager::getGlobalConfigurationTempFileName ()
{
  return (s_frameworkGlobalConfigurationTempFile);
}

}

