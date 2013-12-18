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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Framework/OsLayer/PrismOsLayer.h"
#include "Framework/Core/Wave.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"
#include "VcsFabric/Local/VcsNodePortEvents.h"
#include "VcsFabric/Local/VcsNodeFabosPortEvents.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerCopySchemaMessage.h"
#include "Framework/ObjectModel/WavePostBootEvent.h"
#include "Shell/ShellDebug.h"
#include "Shell/ShellPrism.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Cluster/PrismCluster.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Cluster/Local/ClusterLocalSetThisNodeIpAddressMessage.h"
#include "VcsFabric/Local/VcsNodePrincipalSelectionWorker.h"
#include "VcsFabric/Local/VcsNodeFCPrincipalTrackingWorker.h"
#include "Firmware/Global/FirmwareToolKit.h"
#include "Firmware/Global/NodeAutoUpgradeObjectManager.h"
#include "VcsFabric/Local/VcsNodeSlotManagementWorker.h"
#include "VcsFabric/Local/VcsCopyToStartupWorker.h"
#include "Framework/Postboot/PrismPostPersistentBootMessages.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSavePrismConfigurationMessage.h"
#include "VcsFabric/Global/VcsRemoveSwitchMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "DcmResourceIdEnums.h"
#include "WaveResourceIdEnums.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/VcsClusterConfiguration.h"
#include "Framework/Core/VersionInformation.h"
#include "VcsFabric/Global/VcsUpdatePrincipalMessage.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "VcsFabric/Local/VcsNodeInterDcmCcmConfigDistributeMessage.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "Fcoe/Global/FcoeGlobalFcoeFabricMapMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Firmware/Local/FirmwareOpActionMessage.h"
#include "Firmware/Global/NodeAutoUpgradeConfiguration.h"
#include "VcsFabric/Local/VcsFabricDebugClientMessage.h"
#include "VcsFabric/Local/VcsFabricSetTtyNameMessage.h"
#include "VcsFabric/Local/VcsNodeConfigDistributeContext.h"
#include "VcsFabric/Local/VcsNodeConfigSnapshotWorker.h"
#include "VcsFabric/Local/VcsNodeClusterInfoWorker.h"
#include "Framework/Types/BitMap.h"
//#include "WyserEaGateway/RunningConfigActionPoint/RunningConfigActionPoint.h"
#include <set>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <stdarg.h>
#include <dirent.h>
#include <switch/switch.h>

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

extern "C"
{
module_t MODNAME(DCM);
#define DCMLOG  &MODNAME(DCM)

}

using namespace WaveNs;

namespace DcmNs
{
string  myLocalSWVersion                                                                    = "1.0";
bool    VcsFabricLocalObjectManager::m_isInstantiated                                       = false;
static  ResourceId      m_thisVcsNodeClusterType                                            = VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE;
static  PrismMutex      m_thisVcsNodeClusterMutex;
static  PrismMutex      m_thisVcsNodeSWBDModelMutex;
static  int             m_thisVcsNodeSWBDModel;

MacAddress              VcsFabricLocalObjectManager::m_thisVcsNodeFcfMac                    ("00:00:00:00:00:00");
PrismMutex              VcsFabricLocalObjectManager::m_thisVcsNodeLocalManagedObjectMutex;
PrismMutex              VcsFabricLocalObjectManager::m_thisVcsNodeFcfMacMutex;
PrismMutex              VcsFabricLocalObjectManager::m_ClusterConfFileMutex;
bool                    VcsFabricLocalObjectManager::m_newShowVcs                           = true;

bool                    VcsFabricLocalObjectManager::m_isWyserEaPhase2Complete                = false;
PrismMutex              VcsFabricLocalObjectManager::m_isWyserEaPhase2CompleteMutex;


ResourceId              VcsFabricLocalObjectManager::m_vcsFabricLocalObjectManagerState;
PrismMutex              VcsFabricLocalObjectManager::m_vcsFabricLocalObjectManagerStateMutex;

UI64                    VcsFabricLocalObjectManager::m_vcsClusterGuid                       = 0;
PrismMutex              VcsFabricLocalObjectManager::m_vcsClusterGuidMutex;

map<string, UI32>       VcsFabricLocalObjectManager::m_WWNRbridgeIdMapCache;
PrismMutex              VcsFabricLocalObjectManager::m_WWNRbridgeIdMapCacheMutex;

 
VcsFabricLocalObjectManager::VcsFabricLocalObjectManager ()
    : WaveLocalObjectManager (getClassName ())
{
    PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());


    associateWithVirtualWaveObjectManager (VcsFabObjectManager::getInstance ());
    VcsNodeLocalManagedObject vcsNodeLocalManagedObject (this);
    vcsNodeLocalManagedObject.setupOrm ();

    m_pVcsNodeLocalManagedObject = NULL;
    addManagedClass (VcsNodeLocalManagedObject::getClassName ());

    VcsSlotLocalManagedObject vcsSlotLocalManagedObject (this);
    vcsSlotLocalManagedObject.setupOrm ();

    m_pVcsSlotLocalManagedObject = NULL;
    addManagedClass (VcsSlotLocalManagedObject::getClassName ());

    addDebugFunction ((ShellCmdFunction) (&VcsFabricLocalObjectManager::debugSetDomainId), "setDomainId");
    addDebugFunction ((ShellCmdFunction) (&VcsFabricLocalObjectManager::debugGenerateSCN), "generateSCN");
    addDebugFunction ((ShellCmdFunction) (&VcsFabricLocalObjectManager::addDomainsDebugMessageHandler), "adddomains");
    addDebugFunction ((ShellCmdFunction) (&VcsFabricLocalObjectManager::removeDomainsDebugMessageHandler), "removedomains");
    addDebugFunction ((ShellCmdFunction) (&VcsFabricLocalObjectManager::debugGenerateSlotScn), "generateslotscn");
    addDebugFunction ((ShellCmdFunction) (&VcsFabricLocalObjectManager::debugTriggerPostBootMessage), "triggerpostboot");
    addDebugFunction ((ShellCmdFunction) (&VcsFabricLocalObjectManager::debugTriggerHaMessage), "triggerha");

    addOperationMap (VCS_NODE_SET_VCSID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setVcsIdMessageHandler));
    addOperationMap (VCS_NODE_GET_VCSID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getVcsIdMessageHandler));
    addOperationMap (VCS_NODE_SET_ISPRINCIPAL, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setIsPrincipalMessageHandler));
    addOperationMap (VCS_NODE_GET_ISPRINCIPAL, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getIsPrincipalMessageHandler));
    addOperationMap (VCS_NODE_GET_SWITCHMAC, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getSwitchMacMessageHandler));
    addOperationMap (VCS_NODE_GET_LOCAL_MAPPEDID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getLocalMappedIdMessageHandler));
    addOperationMap (VCS_NODE_SET_FCFMAC, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setFcfMacMessageHandler));
    addOperationMap (VCS_NODE_SET_VCSMODE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setVcsModeMessageHandler));
    addOperationMap (VCS_NODE_GET_VCSMODE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getVcsModeMessageHandler));
    addOperationMap (VCS_NODE_ENQ_SWITCH, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::enqSwitchMessageHandler));
    addOperationMap (VCS_NODE_GENERATE_SCN, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::generateSCNMessageHandler));
    addOperationMap (VCS_NODE_SET_ISREADY, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setIsReadyMessageHandler));
    addOperationMap (VCS_NODE_GET_ISREADY, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getIsReadyMessageHandler));
    addOperationMap (VCS_NODE_GET_IS_DCMD_READY_FOR_HASM_LAUNCH, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getIsDcmdReadyForHasmLaunchMessageHandler));
    addOperationMap (VCS_NODE_SET_IS_HASM_LAUNCHED, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setIsHasmLaunchedMessageHandler));
    addOperationMap (VCS_NODE_TRIGGER_POSTBOOT, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::triggerPostBootMessageHandler));
    addOperationMap (VCS_NODE_SET_RBRIDGEID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setRBridgeIdMessageHandler));
    addOperationMap (VCS_NODE_GET_RBRIDGEID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getRBridgeIdMessageHandler));
    addOperationMap (VCS_NODE_COPY_DEFAULT_START_TRIGGER, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::copyDefaultStartTriggerMessageHandler));
    addOperationMap (VCS_NODE_SET_DOMAINID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setDomainIdMessageHandler));
    addOperationMap (VCS_NODE_ENABLE_VALIDATE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsNodeEnableValidationHandler));
    addOperationMap (VCS_NODE_CONFIG_DISTRIBUTE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::configDistributeMessageHandler));
    addOperationMap (VCS_NODE_SET_SWITCHWWN, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setSwitchWWNMessageHandler));
    addOperationMap (VCS_NODE_GET_SWITCHWWN, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getSwitchWWNMessageHandler));
    addOperationMap (VCS_NODE_GET_SWITCHWWN_FROM_RBRIDGEID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getSwitchWWNFromRBridgeIdMessageHandler));
    addOperationMap (VCS_NODE_GET_RBRIDGEID_FROM_SWITCHWWN, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getRBridgeIdFromSwitchWWNMessageHandler));
    addOperationMap (VCS_NODE_IS_NODE_IN_CLUSTER, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsIsNodeInClusterMessageHandler));
    addOperationMap (VCS_NODE_IS_LOCAL_NODE_IN_CLUSTER, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsIsLocalNodeInClusterMessageHandler));
    addOperationMap (VCS_NODE_GET_SWITCHIP_FROM_RBRIDGEID, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getSwitchIPFromRBridgeIdMessageHandler));
    addOperationMap (VCS_NODE_CLUSTER_MERGE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsClusterMergeMessageHandler));
    addOperationMap (VCS_NODE_GET_CONFIGURATION_NUMBER, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getConfigurationNumberMessageHandler));
    addOperationMap (VCS_NODE_GET_CONFIGURATION_TIME, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getConfigurationTimeMessageHandler));
    addOperationMap (VCS_NODE_GET_SWITCH_IDENTIFIER_INFO, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getSwitchIdentifierInfoMessageHandler));
    addOperationMap (VCS_NODE_SEGMENT_SWITCH, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsSegmentSwitchMessageHandler));
    addOperationMap (VCS_NODE_SET_NEWSHOWVCS, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setNewShowVcsFormatMessageHandler));
    addOperationMap (VCS_NODE_IS_IP_REACHABLE_FROM_LOCAL_NODE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsIsIpReachableFromNodeMessageHandler));
    addOperationMap (VCS_GET_CLUSTER_RBRIDGEID_FROM_WWN, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getClusterRbridgeIdFromWWNHandler));
    addOperationMap (VCS_NODE_IS_FABRIC_DIST_SERVICE_ENABLED, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsIsFabricDistributionEnabledMessageHandler) );
    addOperationMap (VCS_NODE_UPDATE_SWITCH_INFO, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::updateSwitchInfoMessageHandler));
    addOperationMap (VCS_NODE_GENERATE_SLOT_SCN, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::generateSlotScnMessageHandler));
    addOperationMap (VCS_NODE_TEST_CLIENT_FOR_DUAL_CONNECT, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::testClientForDualConnectMessageHandler));
    addOperationMap (VCS_NODE_GET_CURRENT_RBRIDGE_IDS_AND_LOCATION_IDS, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getCurrentRbridgeIdsAndLocationIdsMessageHandler));
    addOperationMap (VCS_SET_WYSEREA_RATE_LIMITING_DELAY, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsSetWyserSessionRateLimitingDelayMessageHandler));
    addOperationMap (VCS_GET_WYSEREA_RATE_LIMITING_DELAY, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsGetWyserEaRateLimitingDelayMessageHandler));
    addOperationMap (VCS_SET_FCF_MAC_ADDRESS_SERVICE_INDEPENDENT, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::setFcfMacAddressHandler));
    addOperationMap (VCS_NODE_INTER_DCM_CCM_CONFIG_DISTRIBUTE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsNodeInterDcmCcmConfigDistributeMessageHandler));
    addOperationMap (VCS_NODE_CLI_ALLOWED_FROM_THIS_LOCATION, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::cliAllowedFromThisLocationMessageHandler));
    addOperationMap (VCS_FABRIC_DEBUG_CLIENT_MSG, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsFabricDebugClientMessageHandler));
    addOperationMap (VCS_FABRIC_SET_TTYNAME, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsFabricSetTtyNameMessageHandler));
    addOperationMap (VCS_FABRIC_SET_MODE_CHANGE_PARAMETERS, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsFabricModeChangeParametersMessageHandler));
    addOperationMap (VCS_FABRIC_SET_VCS_CLUSTER_CONFIG, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationMessageHandler));
    addOperationMap (VCS_NODE_GET_PRINCIPAL_PRIORITY, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsNodeGetPrincipalPriorityHandler));
    addOperationMap (VCS_NODE_SET_PRINCIPAL_PRIORITY, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsNodeSetPrincipalPriorityHandler));
    addOperationMap (VCS_NODE_PRINCIPAL_TRIGGER_FAILOVER, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsNodePrincipalTriggerFailoverHandler));
    addOperationMap (VCS_RESET_DOMAIN_BITMAP_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::resetDomainBitmapMessageHandler));
    addOperationMap (VCS_PREPARE_DOMAIN_LIST_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::prepareDomainListAfterHaRecoveryMessageHandler));
    addOperationMap (VCS_ENABLE_SLOT_SCN_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::enableSlotScnHandlingMessageHandler));
    addOperationMap (VCS_NODE_IS_CLI_ALLOWED_FROM_THIS_LOCATION, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::isCliAllowedFromThisLocationMessageHandler));     
    addOperationMap (VCS_SET_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsSetWyserEaConfigFilterMessageHandler)); 
    addOperationMap (VCS_REMOVE_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsRemoveWyserEaConfigFilterMessageHandler)); 
    addOperationMap (VCS_REMOVE_CFG_FILE_SERVICE_INDEPENDENT, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler));
    addOperationMap (VCS_NODE_IS_GVLAN_CAPABLE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsIsGvlanCapableMessageHandler) );
    addOperationMap (VCS_NODE_IS_GVLAN_ENABLED, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::vcsIsGvlanEnabledMessageHandler) );
    addOperationMap (VCS_NODE_GET_CLUSTER_TYPE, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::getClusterTypeMessageHandler));
    addOperationMap (VCS_NODE_HA_SYNC_CONFIG_TO_KERNEL, reinterpret_cast<PrismMessageHandler> (&VcsFabricLocalObjectManager::syncConfigToKernelOnStandbyMessageHandler));

    addServiceIndependentOperationMap (VCS_NODE_IS_LOCAL_NODE_IN_CLUSTER, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::vcsIsLocalNodeInClusterStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_NODE_GET_LOCAL_MAPPEDID, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::getLocalMappedIdServiceIndependentStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_GET_CLUSTER_RBRIDGEID_FROM_WWN, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::getClusterRbridgeIdFromWWNStaticHandler));
    addServiceIndependentOperationMap (VCS_SET_FCF_MAC_ADDRESS_SERVICE_INDEPENDENT, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::setFcfMacAddressStaticHandler));
    addServiceIndependentOperationMap (VCS_NODE_CLI_ALLOWED_FROM_THIS_LOCATION, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::cliAllowedFromThisLocationServiceIndependentStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_NODE_GET_CONFIGURATION_NUMBER, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::getConfigurationNumberStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_NODE_IS_CLI_ALLOWED_FROM_THIS_LOCATION,reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::isCliAllowedFromThisLocationStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_SET_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::vcsSetWyserEaConfigFilterStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_REMOVE_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::vcsRemoveWyserEaConfigFilterStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_REMOVE_CFG_FILE_SERVICE_INDEPENDENT, reinterpret_cast<WaveServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageStaticHandler));
    addServiceIndependentOperationMap (VCS_NODE_GET_CLUSTER_TYPE, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::getClusterTypeStaticMessageHandler));
    addServiceIndependentOperationMap (VCS_NODE_IS_GVLAN_CAPABLE, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::vcsIsGvlanCapableStaticMessageHandler) );
    addServiceIndependentOperationMap (VCS_NODE_IS_GVLAN_ENABLED, reinterpret_cast<ManagementInterfaceServiceIndependentMessageHandler> (&VcsFabricLocalObjectManager::vcsIsGvlanEnabledStaticMessageHandler) );

    addEventType    (VCS_NODE_DOMAIN_VALID);
    addEventType    (VCS_NODE_DOMAIN_CAPABILITIES_AVAILABLE);
    addEventType    (VCS_NODE_DOMAIN_INVALID);
    addEventType    (VCS_NODE_DOMAIN_REACHABLE);
    addEventType    (VCS_NODE_DOMAIN_UNREACHABLE);
    addEventType    (VCS_NODE_PORT_DISABLE);
    addEventType    (VCS_NODE_SWITCH_OFFLINE);
    addEventType    (VCS_NODE_ZONE_CHANGED);
    addEventType    (VCS_NODE_ZONE_ABORTED);
    addEventType    (VCS_NODE_SWITCH_READY_TO_ACCEPT_COMMANDS);

    m_isInstantiated = true;
    m_noVcsEnableInProgressFlag = false;
    m_noVcsEnablePrincipalFlag = false;
    m_isReady = false;    
    m_noVcsEnablePrincipalControlFailoverFlag = false;
    m_serviceReadyEventReceived = false;
    m_systemReadyEventReceived = false;
    m_linecardReadyEventReceived = false;
    m_systemRecoveryType = WAVE_HA_COLD_RECOVERY;
    
    m_linecardReadySlotIdBitMap.reset ();
    m_linecardReadyPostBootCompleteBitMap.reset ();
    m_systemReadyPostbootProcessingComplete = false;
    m_isConfigReplayCompleted = false;
    m_thisVcsNodeFwVersion = "";
    m_isHaSyncInProgress = false;
    m_isNodeReadyForSlotScn = false;

    getPtrToPrincipalTrackingWorker ();
    m_vcsNodeSlotManagementWorker               = new VcsNodeSlotManagementWorker (this);
    m_pVcsCopyToStartupWorker                   = new VcsCopyToStartupWorker (this);
    m_pVcsNodeConfigSnapshotWorker              = new VcsNodeConfigSnapshotWorker (this);
    m_pVcsNodeClusterInfoWorker                 = new VcsNodeClusterInfoWorker (this);
    PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
}

void VcsFabricLocalObjectManager::getPtrToPrincipalTrackingWorker ()
{
    
    m_fcPrincipalTrackingWorker = NULL;
    m_VcsPrincipalSelectionWorker = NULL;
    if (DcmToolKit::fcPrincipalEnabled())
    {
        m_fcPrincipalTrackingWorker = new VcsNodeFCPrincipalTrackingWorker(this);
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getPtrToPrincipalTrackingWorker: FCPrincipal tracking enabled");
    }
    else
    {
        m_VcsPrincipalSelectionWorker = new VcsNodePrincipalSelectionWorker (this);
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getPtrToPrincipalTrackingWorker: Vcs Principal selection enabled");
    }
}

VcsFabricLocalObjectManager::~VcsFabricLocalObjectManager ()
{
    if (m_pVcsNodeLocalManagedObject)
    {
        delete (m_pVcsNodeLocalManagedObject);
        m_pVcsNodeLocalManagedObject = NULL;
    }

    if (m_pVcsSlotLocalManagedObject)
    {
        delete (m_pVcsSlotLocalManagedObject);
        m_pVcsSlotLocalManagedObject = NULL;
    }

    if (m_vcsNodeSlotManagementWorker)
    {
        delete (m_vcsNodeSlotManagementWorker);
    }

    if (m_pVcsCopyToStartupWorker)
    {
        delete (m_pVcsCopyToStartupWorker);
    }

    if (m_pVcsNodeClusterInfoWorker)
    {
        delete (m_pVcsNodeClusterInfoWorker);
    }
}

VcsFabricLocalObjectManager *VcsFabricLocalObjectManager::getInstance ()
{
    static VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = new VcsFabricLocalObjectManager ();

    WaveNs::prismAssert (NULL != pVcsFabricLocalObjectManager, __FILE__, __LINE__);

    return (pVcsFabricLocalObjectManager);
}

PrismServiceId VcsFabricLocalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

bool VcsFabricLocalObjectManager::getIsInstantiated ()
{
    return (m_isInstantiated);
}

WaveManagedObject *VcsFabricLocalObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((VcsNodeLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new VcsNodeLocalManagedObject(this);
    }
    else if ((VcsSlotLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new VcsSlotLocalManagedObject(this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

void VcsFabricLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_VALID, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::domainValidEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::domainCapabilitiesAvailableEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_INVALID, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::domainInvalidEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_REACHABLE, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::domainReachableEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_DOMAIN_UNREACHABLE, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::domainUnreachableEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SWITCH_OFFLINE, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::switchOfflineEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::bootCompleteEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), HA_BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::habootCompleteEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SLOT_ONLINE, reinterpret_cast<PrismEventHandler> (&VcsNodeSlotManagementWorker::slotOnlineEventHandler), m_vcsNodeSlotManagementWorker);
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SLOT_OFFLINE, reinterpret_cast<PrismEventHandler> (&VcsNodeSlotManagementWorker::slotOfflineEventHandler), m_vcsNodeSlotManagementWorker);
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_ZONE_CHANGED, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::zoneChangedEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_ZONE_ABORTED, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::zoneAbortedEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SWITCH_READY_TO_ACCEPT_COMMANDS, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::switchReadyToAcceptCommandsEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SWITCH_ACTIVATION_COMPLETE, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::switchActivationCompleteEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_LOCAL_NODE_DELETED_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::nodeDeletedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_BROADCAST_ADDITION_OF_NEW_NODES_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::nodeAddedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_DELETE_CLUSTER_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::clusterDeleteEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_BROADCAST_PHASE3_START_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::phase3StartEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_BROADCAST_PHASE3_COMPLETE_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::phase3CompleteEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SYNC_START, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::syncDumpStartEventHandler));

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        // handle port disable events from the kernel
        listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (),VCS_NODE_FABOS_PORT_DISABLE,reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::switchPortDisableEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SERVICE_READY, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::serviceReadyEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_LINECARD_READY, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::linecardReadyEventHandler));
    listenForEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_SYSTEM_READY, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::systemReadyEventHandler));

    listenForEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_SERVICE_READY, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::NsmServiceReadyEventHandler));
    listenForEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_CHASSIS_READY, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::NsmChassisReadyEventHandler));
    listenForEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_LINECARD_READY, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::NsmLinecardReadyEventHandler));
    listenForEvent (NsmLocalObjectManager::getPrismServiceId (), NSM_LINECARD_ONLINE, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::NsmLinecardOnlineEventHandler));

    // listen for postboot & config replay started & completed events
    // this allows addition of raslogs
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_POST_BOOT_STARTED_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::postbootStartedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_POST_BOOT_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::postbootCompletedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_STARTED_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::configReplayStartedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::configReplayCompletedEventHandler));
        listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_LAST_CONFIG_REPLAY_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&VcsFabricLocalObjectManager::lastConfigReplayCompletedEventHandler));
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsFabricLocalObjectManager::dbInconsistencyCheck (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::dbInconsistencyCheck : Entering ...");

    //ResourceId  status = WAVE_MESSAGE_SUCCESS;
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
    trace (TRACE_LEVEL_INFO, string ("BootReason : ") + FrameworkToolKit::localize (bootReason));

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeObjectId (FrameworkToolKit::getThisWaveNodeObjectId(), "ownerWaveNodeObjectId"));

/*
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);

    if (0 == pWaveManagedObjects->size ())
    {   
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::dbInconsistencyCheck : Zero entries found in VcsNodeLocalManagedObject for this location."));
        status = WAVE_MESSAGE_ERROR;
    }
    else if (1 < pWaveManagedObjects->size ())
    {   
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject1 = NULL;
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject2 = NULL;

        pVcsNodeLocalManagedObject1 = dynamic_cast<VcsNodeLocalManagedObject *> ((*pWaveManagedObjects)[0]);
        pVcsNodeLocalManagedObject2 = dynamic_cast<VcsNodeLocalManagedObject *> ((*pWaveManagedObjects)[1]);

        prismAssert (NULL != pVcsNodeLocalManagedObject1, __FILE__, __LINE__);
        prismAssert (NULL != pVcsNodeLocalManagedObject2, __FILE__, __LINE__);

        ObjectId object1 = pVcsNodeLocalManagedObject1->getObjectId ();
        ObjectId object2 = pVcsNodeLocalManagedObject2->getObjectId ();

        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::dbInconsistencyCheck : Duplicate entries found in VcsNodeLocalManagedObject for this location. The ObjectIds are ") + object1.toString () + " and " + object2.toString ());
        status = WAVE_MESSAGE_ERROR;

        delete pVcsNodeLocalManagedObject1;
        delete pVcsNodeLocalManagedObject2;
    }

    pWaveManagedObjects->clear ();
    delete pWaveManagedObjects;
*/

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (FrameworkToolKit::checkForExistenceOfOneMO (&synchronousQueryContext, VcsNodeLocalManagedObject::getClassName ()));
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsFabricLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId                  status          = WAVE_MESSAGE_SUCCESS;
    MacAddress                  fcfMac          = getFcfMacAddress(); 
    UI32                        domainId        = 0;
    UI32                        inVcs           = 0;
    bool                        vcsMode         = false;
    UI32                        vcsId           = 0;
    VcsNodeLocalManagedObject   *pVcsNodeLocalManagedObject = NULL;

    parseVcsConfFile (vcsMode, vcsId);

    /****************************************************************************
     * Create managed object or retrieve from database
     ***************************************************************************/

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason)
    {
        /* This is first boot of the box */

        // Use locationId as domain to ensure unique domainId specifically for x86 testing.
        // On actual switch, domain will be overwritten when DomainValid is received.
        domainId = FrameworkToolKit::getThisLocationId();

        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::install: First boot of local node") + domainId);
        
    }
    else if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason)
    {
        domainId = DcmToolKit::getThisVcsNodeDomainId ();
        inVcs    = 1;
        vcsMode  = true;
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::install: Wave boot secondary node configure boot") + domainId);
    }

    /****************************************************************************
     * Commit to database
     ***************************************************************************/

    if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) || (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason))
    {

        startTransaction();

        createThisVcsNodeLocalManagedObject();

        setThisVcsNodeLocalManagedObjectGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);

        if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason)
        {
            setThisVcsNodeLocalManagedObjectSpecificStatus (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_STAND_ALONE);

            if(CCM_ONLY == DcmToolKit::getDistributionMode ()) 
	        {
                trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::install: FDS state set successfully to false in VcsNodeLocalManagedObject");
	        }
        }
        else if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason)
        {
          ResourceId specificStatus = VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED;
          setThisVcsNodeLocalManagedObjectSpecificStatus (specificStatus);
          trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::install: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Add Phase2 Started ");   
		}

        setThisVcsNodeLocalManagedObjectDomainId    (domainId);
        setThisVcsNodeLocalManagedObjectVcsId       (vcsId);
        setThisVcsNodeLocalManagedObjectVcsMode     (vcsMode);
        setThisVcsNodeLocalManagedObjectInVcs       (inVcs);
        setThisVcsNodeLocalManagedObjectIsPrincipal (0);
        setThisVcsNodeLocalManagedObjectSerialNumber(getThisVcsNodeSerialNumber());
        setThisVcsNodeLocalManagedObjectfcfMac      (fcfMac);
        setThisVcsNodeLocalManagedObjectSwitchMac   (getThisVcsNodeSwitchMac());   
        setThisVcsNodeLocalManagedObjectSwitchWWN   (getThisVcsNodeSwitchWWN());

        vector<string> publicIpAddress;
        publicIpAddress.push_back (FrameworkToolKit::getIpAddressForInterface ("eth0"));
        publicIpAddress.push_back (FrameworkToolKit::getIpAddressForInterface ("eth1"));

        setThisVcsNodeLocalManagedObjectPublicIpAddress (publicIpAddress);
        setThisVcsNodeLocalManagedObjectLocationId      (FrameworkToolKit::getThisLocationId ());
        setThisVcsNodeLocalManagedObjectSwitchPort      (FrameworkToolKit::getThisLocationPort ());
        setThisVcsNodeLocalManagedObjectSWBDModel       (getThisVcsNodeSWBDModel ());
        setThisVcsNodeLocalManagedObjectSwitchFwVersion (getThisVcsNodeSwitchFwVersion ());

        string      switchname          = "";
        ResourceId  switchnameStatus    = getLocalNodeSwitchname (switchname);

        if (WAVE_MESSAGE_SUCCESS == switchnameStatus)
        {
            setThisVcsNodeLocalManagedObjectSwitchname (switchname);
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::install: publicIpAddress (eth0) = : ") + publicIpAddress[0] + " switch name = " + switchname);
            
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::install: Error while getting switchname : status : ") + FrameworkToolKit::localize (switchnameStatus));
        }

        // In case of WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT -
        //      cached value of swmodelName is available in m_swmodelName.
        // In case of WAVE_BOOT_FIRST_TIME_BOOT -
        //      m_swmodelName will be empty, but will get updated later by a message from VcsNodeFabosInterfaceObjectManager.
        setThisVcsNodeLocalManagedObjectSwmodelName     (getThisVcsNodeSwmodelName ());

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::install: completed");
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::install: Failed");
//            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }

        ObjectId objectId = getThisVcsNodeLocalManagedObjectObjectId ();
        setThisVcsNodeObjectId (objectId);

        m_vcsNodeSlotManagementWorker->installKnownSlotsWhileJoiningCluster (pWaveAsynchronousContextForBootPhases);

    }
    else if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)
    {

        vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName ());

        prismAssert (1 == (pResults->size ()), __FILE__, __LINE__);

        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);

        prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);

        bool isPrincipalNode = pVcsNodeLocalManagedObject->getIsPrincipal();

        tracePrintf (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::install:isPrincipalValue %u", isPrincipalNode);

        startTransaction ();

        setThisVcsNodeLocalManagedObject            (pVcsNodeLocalManagedObject);
        updateThisVcsNodeLocalManagedObject         ();
        setThisVcsNodeLocalManagedObjectIsPrincipal (false);
        setThisVcsNodeLocalManagedObjectPrincipalPriority (0);

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
        }

        //Change this set: Make the set thread safe
        ObjectId objectId = getThisVcsNodeLocalManagedObjectObjectId ();

        setThisVcsNodeObjectId (objectId);

        pResults->clear ();

        delete pResults;

        // For persistent boot with default, clear all the linecard configurations
        m_vcsNodeSlotManagementWorker->clearAllLinecardHandler();
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void  VcsFabricLocalObjectManager::externalStateSynchronization  (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
{

    tracePrintf(TRACE_LEVEL_INFO,false,false,"VcsFabricLocalObjectManager::externalStateSynchronization getFssStageNumber %d type %d, FCSW %d", pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() , pWaveAsynchronousContextForExternalStateSynchronization->getServiceType(), FCSW );

    if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber()) == 1001 && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW))
    {
        setThisNodePrincipalPriorityInFabric ();
    }

    pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForExternalStateSynchronization->callback();
 }

void VcsFabricLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::boot : Entering ...");

    VcsNodeLocalManagedObject  *pThisVcsNode        = NULL;
    //UI32                        domainId            = 0;
    LocationRole                thisLocationRole    = FrameworkToolKit::getThisLocationRole ();

    if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 != thisLocationRole) && (LOCATION_SECONDARY_CLUSTER_PHASE_1 != thisLocationRole) && (LOCATION_SECONDARY_REJOIN_PHASE_0 != thisLocationRole) && (LOCATION_SECONDARY_REJOIN_PHASE_1 != thisLocationRole))
    {
        // Resetting the cached vcs cluster guid.  This value is queried and cached during shutdown and used to service the configuration number when the local DB is empty during cluster operations add/rejoin in phase 0/1.

        VcsFabricLocalObjectManager::setCachedVcsClusterGuid (0);
    }

    if(NULL == m_pVcsNodeLocalManagedObject ) 
    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

        synchronousQueryContext.addAndAttribute (new AttributeObjectId (FrameworkToolKit::getThisWaveNodeObjectId(), "ownerWaveNodeObjectId"));

        vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
        prismAssert (1 == (pWaveManagedObjects->size ()), __FILE__, __LINE__);

        if (NULL == (*pWaveManagedObjects)[0])
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::boot: DB query returned one object but pointer to the object is NULL - this should'n happen so assert");
            prismAssert (false, __FILE__, __LINE__);
        }

        pThisVcsNode = dynamic_cast<VcsNodeLocalManagedObject *> ((*pWaveManagedObjects)[0]);

        //domainId = pThisVcsNode->getDomainId();

        setThisVcsNodeLocalManagedObject (pThisVcsNode);

        pWaveManagedObjects->clear ();

        delete pWaveManagedObjects;
    }
    
    ResourceId     status;
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    // cache the boot reason code

    setThisVcsNodeBootReason (bootReason);

    startTransaction();
    // update the location ID since it may have changed in this boot
    setThisVcsNodeLocalManagedObjectLocationId(FrameworkToolKit::getThisLocationId ());
    setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (false);                                                                                                                                                              
    if(LOCATION_SECONDARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole () || LOCATION_PRIMARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole ()) 
    {
        setThisVcsNodeLocalManagedObjectInVcs       (false);
        setThisVcsNodeLocalManagedObjectIsPrincipal (false);

        setThisVcsNodeLocalManagedObjectStatus      (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD,VCS_FABRIC_LOCAL_SPECIFIC_STATUS_WAITING_TO_REJOIN);
    }
    else if(WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT == bootReason)
    {
        setThisVcsNodeLocalManagedObjectInVcs  (true);
        setThisVcsNodeLocalManagedObjectStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD, VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_STARTED);

        fw_version_info_t *pfwInfo = (fw_version_info_t *) calloc (1, sizeof (fw_version_info_t));

        if (NULL == pfwInfo)
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::boot : calloc returned NULL.");
            prismAssert (false, __FILE__, __LINE__);
        }

        int fwVersionRet = get_fw_version_info (pfwInfo);

        if (0 == fwVersionRet)
        {
            string fwVersion = string (pfwInfo->full_version);
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::boot : Firmware Version : ") + fwVersion);
            setThisVcsNodeLocalManagedObjectSwitchFwVersion (fwVersion);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::boot : get_fw_version_info returned ") + fwVersionRet);
        }

        free (pfwInfo); /// free

        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::boot: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Rejoin Phase2 Started ");
        setThisVcsNodeLocalManagedObjectSwitchPort (FrameworkToolKit::getThisLocationPort());
    }
    else if(WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)
    {
        setThisVcsNodeLocalManagedObjectInVcs  (false);
        setThisVcsNodeLocalManagedObjectStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD, VCS_FABRIC_LOCAL_SPECIFIC_STATUS_STAND_ALONE);
    }

    updateThisVcsNodeLocalManagedObject ();

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
    }

    ObjectId objectId = getThisVcsNodeLocalManagedObjectObjectId ();

    setThisVcsNodeObjectId (objectId);

    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::boot :     Object ID   : ") + objectId.toString ());

    //Clear all the cached rbridgeIdMapCache, subsequent gets should always be from the database.
    VcsFabricLocalObjectManager::clearWWNRbridgeIdMapCache ();

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsFabricLocalObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    bool                        vcsMode         = false;
    bool                        clusterMode     = false;
    UI32                        vcsId           = 0;

    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::initialize : Entering ...");

    Wave::registerPersistencePostBootCheck (&DcmToolKit::isVcsEnabled);

    parseVcsConfFile (vcsMode, vcsId);

    clusterMode = parseClusterConfFile ();

    setThisVcsNodeClusterType (vcsMode, clusterMode);

    bool status = DcmToolKit::updateFirmwareVersions ();
    tracePrintf (TRACE_LEVEL_INFO, true, false, "VcsFabricLocalObjectManager::initialize : status = %d",status);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsFabricLocalObjectManager::disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    VcsNodeLocalManagedObject* pVcsNodeLocalManagedObject = getThisVcsNodeLocalManagedObject ();

    if( pVcsNodeLocalManagedObject) 
    {
        setThisVcsNodeLocalManagedObject (NULL);
        delete pVcsNodeLocalManagedObject;
    }

    m_isNodeReadyForSlotScn = false;

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void VcsFabricLocalObjectManager::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    vector<WaveManagedObject *>    *pResults    = NULL;

    // Query for the Global MO "VcsClusterManagedObject".  This Global MO will be cached.  The cached information will be used to still provide configuration information during add/rejoin phase 0/1 where local DB will be emptied.   

    pResults = WaveObjectManagerToolKit::querySynchronously (VcsClusterManagedObject::getClassName ());

    if (NULL != pResults)
    {
        prismAssert (0 == (pResults->size ()) || 1 == (pResults->size ()), __FILE__, __LINE__);

        if (1 == pResults->size ())
        {
            VcsClusterManagedObject *pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *> ((*pResults)[0]);
            prismAssert (NULL != pVcsClusterManagedObject, __FILE__, __LINE__);

            VcsFabricLocalObjectManager::setCachedVcsClusterGuid (pVcsClusterManagedObject->getClusterGUID ().getUuidHash ());
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }
 
    //Cache all the WWN and rbridgeIds. This is needed when fabric queries for the free rbridgeId corresponding to the given WWN.
    VcsFabricLocalObjectManager::updateWWNRbridgeIdMapCache ();
 
    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

PrismMessage *VcsFabricLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_NODE_SET_VCSID :
            pPrismMessage = new VcsNodeSetVcsIdMessage ();
            break;
        case VCS_NODE_GET_VCSID :
            pPrismMessage = new VcsNodeGetVcsIdMessage ();
            break;
        case VCS_NODE_SET_ISPRINCIPAL :
            pPrismMessage = new VcsNodeSetIsPrincipalMessage ();
            break;
        case VCS_NODE_GET_ISPRINCIPAL :
            pPrismMessage = new VcsNodeGetIsPrincipalMessage ();
            break;
        case VCS_NODE_GET_SWITCHMAC :
            pPrismMessage = new VcsNodeGetSwitchMacMessage ();
            break;
        case VCS_NODE_GET_LOCAL_MAPPEDID :
            pPrismMessage = new VcsNodeGetLocalMappedIdMessage ();
            break;
        case VCS_NODE_SET_FCFMAC :
            pPrismMessage = new VcsNodeSetFcfMacMessage ();
            break;
        case VCS_NODE_ENQ_SWITCH :
            pPrismMessage = new VcsEnqSwitchMessage     ();
            break;
        case VCS_NODE_GET_VCSMODE:
            pPrismMessage = new VcsNodeGetVcsModeMessage ();
            break;
        case VCS_NODE_SET_ISREADY :
            pPrismMessage = new VcsNodeSetIsReadyMessage ();
            break;
        case VCS_NODE_GET_ISREADY :
            pPrismMessage = new VcsNodeGetIsReadyMessage ();
            break;
        case VCS_NODE_TRIGGER_POSTBOOT :
            pPrismMessage = new VcsNodeTriggerPostBootMessage ();
            break;
        case VCS_NODE_GET_IS_DCMD_READY_FOR_HASM_LAUNCH:
            pPrismMessage = new VcsNodeGetIsDcmdReadyForHasmLaunchMessage ();
            break;
        case VCS_NODE_SET_IS_HASM_LAUNCHED:
            pPrismMessage = new VcsNodeSetIsHasmLaunchedMessage ();
            break;
        case VCS_NODE_GET_CLUSTER_TYPE:
            pPrismMessage = new VcsNodeGetClusterTypeMessage ();
            break;
        case VCS_NODE_SET_RBRIDGEID :
            pPrismMessage = new VcsNodeSetRBridgeIdMessage ();
            break;
        case VCS_NODE_GET_RBRIDGEID :
            pPrismMessage = new VcsNodeGetRBridgeIdMessage ();
            break;
        case VCS_NODE_COPY_DEFAULT_START_TRIGGER :
            pPrismMessage = new VcsNodeCopyDefaultStartTriggerMessage ();
            break;
        case VCS_NODE_SET_DOMAINID :
            pPrismMessage = new VcsNodeSetDomainIdMessage ();
            break;
        case VCS_NODE_GET_SWITCHWWN_FROM_RBRIDGEID :
            pPrismMessage = new VcsNodeGetSwitchWWNFromRBridgeIdMessage ();
            break;
        case VCS_NODE_GET_RBRIDGEID_FROM_SWITCHWWN :
            pPrismMessage = new VcsNodeGetRBridgeIdFromSwitchWWNMessage ();
            break;
        case VCS_NODE_IS_NODE_IN_CLUSTER :
            pPrismMessage = new VcsNodeIsNodeInClusterMessage ();
            break;
        case VCS_NODE_IS_LOCAL_NODE_IN_CLUSTER :
            pPrismMessage = new VcsNodeIsLocalNodeInClusterMessage ();
            break;            
        case VCS_NODE_GET_SWITCHIP_FROM_RBRIDGEID :
            pPrismMessage = new VcsNodeGetSwitchIPFromRBridgeIdMessage ();
            break;
        case VCS_NODE_CLUSTER_MERGE :
            pPrismMessage = new VcsClusterMergeMessage ();
            break;
        case VCS_NODE_GET_CONFIGURATION_NUMBER :
            pPrismMessage = new VcsNodeGetConfigurationNumberMessage ();
            break;
        case VCS_NODE_GET_CONFIGURATION_TIME :
            pPrismMessage = new VcsNodeGetConfigurationTimeMessage ();
            break;
        case VCS_NODE_GET_SWITCH_IDENTIFIER_INFO :
            pPrismMessage = new VcsFabricGetSwitchIdentifierInfoMessage ();
            break;
        case VCS_NODE_SEGMENT_SWITCH :
            pPrismMessage = new VcsNodeSegmentSwitchMessage ();
            break;
        case VCS_NODE_SET_NEWSHOWVCS :
            pPrismMessage = new VcsNodeSetNewShowVcsFormatMessage ();
            break;
        case VCS_NODE_IS_IP_REACHABLE_FROM_LOCAL_NODE:
            pPrismMessage = new VcsIsIpReachableFromNodeMessage ();
            break;
        case VCS_GET_CLUSTER_RBRIDGEID_FROM_WWN:
            pPrismMessage = new VcsGetClusterRbridgeIdFromWWNMessage ();
            break;
        case VCS_SET_FCF_MAC_ADDRESS_SERVICE_INDEPENDENT:
            pPrismMessage = new VcsNodeSetFcfMacMessageServiceIndependent();
            break;
        case VCS_NODE_IS_FABRIC_DIST_SERVICE_ENABLED:
            pPrismMessage = new VcsNodeIsFabricDistributionEnabledMessage ();
            break;
        case VCS_NODE_UPDATE_SWITCH_INFO:
            pPrismMessage = new VcsNodeUpdateSwitchInfoMessage ();
            break;
        case VCS_NODE_TEST_CLIENT_FOR_DUAL_CONNECT:
            pPrismMessage = new VcsNodeTestClientForDualConnectMessage ();
            break;
        case VCS_NODE_GET_CURRENT_RBRIDGE_IDS_AND_LOCATION_IDS:
            pPrismMessage = new VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage ();
            break;
        case VCS_SET_WYSEREA_RATE_LIMITING_DELAY:
            pPrismMessage = new VcsSetWyserEaRateLimitingDelayMessage ();
            break;
        case VCS_GET_WYSEREA_RATE_LIMITING_DELAY:
            pPrismMessage = new VcsGetWyserEaRateLimitingDelayMessage ();
            break;
        case VCS_NODE_CONFIG_DISTRIBUTE:
            pPrismMessage = new VcsNodeConfigDistributeMessage ();
            break;
        case VCS_NODE_INTER_DCM_CCM_CONFIG_DISTRIBUTE:
            pPrismMessage = new VcsNodeInterDcmCcmConfigDistributeMessage ();
            break;
        case VCS_NODE_CLI_ALLOWED_FROM_THIS_LOCATION:
            pPrismMessage = new VcsNodeCliAllowedFromThisLocationMessage ();
            break;
        case VCS_FABRIC_DEBUG_CLIENT_MSG:
            pPrismMessage = new VcsFabricDebugClientMessage ();
            break;
        case VCS_FABRIC_SET_TTYNAME:
            pPrismMessage = new VcsFabricSetTtyNameMessage ();
            break;
        case VCS_FABRIC_SET_MODE_CHANGE_PARAMETERS:
            pPrismMessage = new VcsNodeSetModeChangeParametersMessage ();
            break;
        case VCS_NODE_SET_PRINCIPAL_PRIORITY:
            pPrismMessage = new VcsNodeSetPrincipalPriorityMessage ();
            break;
        case VCS_NODE_GET_PRINCIPAL_PRIORITY:
            pPrismMessage = new VcsNodeGetPrincipalPriorityMessage ();
            break;
         case VCS_NODE_IS_CLI_ALLOWED_FROM_THIS_LOCATION:
            pPrismMessage = new VcsNodeIsCliAllowedFromThisLocationMessage ();
            break;
        case VCS_SET_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT:
            pPrismMessage = new VcsSetWyserEaConfigFilterServiceIndependentMessage ();
            break;
        case VCS_REMOVE_WYSEREA_CONFIG_FILTER_SERVICE_INDEPENDENT:
            pPrismMessage = new VcsRemoveWyserEaConfigFilterServiceIndependentMessage ();
            break;
        case VCS_FABRIC_SET_VCS_CLUSTER_CONFIG:
            pPrismMessage = new VcsNodeSetClusterConfigurationMessage ();
            break;
        case VCS_REMOVE_CFG_FILE_SERVICE_INDEPENDENT:
            pPrismMessage = new VcsRemoveStandbyCfgFileMessage ();
            break;
        case VCS_NODE_IS_GVLAN_CAPABLE:
            pPrismMessage = new VcsNodeIsGvlanCapableMessage ();
            break;
        case VCS_NODE_IS_GVLAN_ENABLED:
            pPrismMessage = new VcsNodeIsGvlanEnabledMessage ();
            break;
        case VCS_NODE_HA_SYNC_CONFIG_TO_KERNEL:
            pPrismMessage = new VcsNodeHaSyncConfigToKernelMessaage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

PrismEvent *VcsFabricLocalObjectManager::createEventInstance (const UI32 &eventOperationCode)
{
    PrismEvent *pPrismEvent = NULL;

    switch (eventOperationCode)
    {
        case VCS_NODE_DOMAIN_VALID:
            pPrismEvent = new VcsNodeDomainValidEvent ();
            break;

        case VCS_NODE_DOMAIN_INVALID:
            pPrismEvent = new VcsNodeDomainInvalidEvent ();
            break;

        case VCS_NODE_DOMAIN_CAPABILITIES_AVAILABLE:
            pPrismEvent = new VcsNodeDomainCapabilitiesAvailableEvent ();
            break;

        case VCS_NODE_DOMAIN_REACHABLE:
            pPrismEvent = new VcsNodeDomainReachableEvent ();
            break;

        case VCS_NODE_DOMAIN_UNREACHABLE:
            pPrismEvent = new VcsNodeDomainUnreachableEvent ();
            break;

        case VCS_NODE_PORT_DISABLE:
            pPrismEvent = new VcsNodeSwitchPortDisableEvent ();
            break;

        case VCS_NODE_SLOT_ONLINE:
            pPrismEvent = new VcsNodeSlotOnlineEvent ();
            break;
        case VCS_NODE_SLOT_OFFLINE:
            pPrismEvent = new VcsNodeSlotOfflineEvent ();
            break;

        case VCS_NODE_SWITCH_OFFLINE:
            pPrismEvent = new VcsNodeSwitchOfflineEvent ();
            break;

        case VCS_NODE_ZONE_CHANGED:
            pPrismEvent = new VcsNodeZoneChangedEvent ();
            break;

        case VCS_NODE_ZONE_ABORTED:
            pPrismEvent = new VcsNodeZoneAbortedEvent ();
            break;

        case VCS_NODE_SWITCH_READY_TO_ACCEPT_COMMANDS:
            pPrismEvent = new VcsNodeSwitchReadyToAcceptCommandsEvent ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::createEventInstance : Unkown Event Operation Code : ") + eventOperationCode);
            prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismEvent);
}

void VcsFabricLocalObjectManager::setVcsIdMessageHandler (VcsNodeSetVcsIdMessage *pVcsNodeSetVcsIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsIdVcsConfStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetVcsIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->setNewVcsId (pVcsNodeSetVcsIdMessage->getVcsId ());
    pVcsNodeLocalMessagingContext->setVcsId (DcmToolKit::getVcsId ());

    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    
    if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_ONLY == DcmToolKit::getDistributionMode ()))
    {
        // in FC mode, we skip this step
        return (WAVE_MESSAGE_SUCCESS);
    }
    
    LocationRole currentLocationRole = FrameworkToolKit::getThisLocationRole ();
    ResourceId messageStatus = WAVE_MESSAGE_ERROR;
    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

    if (LOCATION_STAND_ALONE == currentLocationRole)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep: standalone location role") );
        return (WAVE_MESSAGE_SUCCESS);
    }
    else if (LOCATION_PRIMARY == currentLocationRole)
    {
        
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep: primary location role") );
        
        // Disconnect first before destroy cluster so this node's membership is still in the cluster 

        ResourceId oldClusterType = pVcsNodeLocalMessagingContext->getClusterType ();
        ResourceId newClusterType = pVcsNodeLocalMessagingContext->getNewClusterType ();

        if (oldClusterType == newClusterType)
        {

            vector<LocationId> connectedLocationsVector;
            FrameworkToolKit::getConnectedLocations (connectedLocationsVector);
            
            if (connectedLocationsVector.size () == 0)
            {
                trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep: No connected locationIds.");
            }

            for (UI32 i = 0; i < connectedLocationsVector.size (); i++)
            {
                string     nodeIpAddress  = FrameworkToolKit::getIpAddressForLocationId (connectedLocationsVector[i]);
                UI32       nodePort       = FrameworkToolKit::getPortForLocationId      (connectedLocationsVector[i]);

                StopHeartBeatMessage *pStopHeartBeatMessage = new StopHeartBeatMessage ();
                pStopHeartBeatMessage->setDstIpAddress  (nodeIpAddress);
                pStopHeartBeatMessage->setDstPortNumber (nodePort);

                WaveMessageStatus status = sendSynchronously (pStopHeartBeatMessage);
                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep: Not able to send stop heartbeat message.");
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep: StopHeartBeatMessage: status: ") + FrameworkToolKit::localize (pStopHeartBeatMessage->getCompletionStatus ()));
                }

                delete pStopHeartBeatMessage;
            }
        }
        else
        {
            startTransaction ();

            setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (true);

            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep: Primary MO will be deleted after failover.");

            updateThisVcsNodeLocalManagedObject ();

            ResourceId status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep: Failed to Update isPrincipal on the node, locationId = ") + FrameworkToolKit::getThisLocationId ());
                prismAssert (false, __FILE__, __LINE__);
            }
        }

        PrismFrameworkObjectManager::disconnectFromAllKnownLocations ();

        FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage = new FrameworkObjectManagerDestroyClusterMessage ();
        pFrameworkObjectManagerDestroyClusterMessage->setOriginalRequester (VcsFabricLocalObjectManager::getPrismServiceId ());
        pFrameworkObjectManagerDestroyClusterMessage->setIsRebootRequired (false);

        messageStatus = sendSynchronously (pFrameworkObjectManagerDestroyClusterMessage);

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : Error in sending destoryCluster message : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
            return messageStatus;
        }
        else
        {
            completionStatus = pFrameworkObjectManagerDestroyClusterMessage->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : Error in destroyCluster message Completion Status : \"" + FrameworkToolKit::localize (completionStatus) + "\"");
                return completionStatus;
            }
        }

        delete pFrameworkObjectManagerDestroyClusterMessage;
        return (WAVE_MESSAGE_SUCCESS);

    }
    else if (LOCATION_SECONDARY == currentLocationRole || LOCATION_PRIMARY_UNCONFIRMED == currentLocationRole || LOCATION_SECONDARY_UNCONFIRMED == currentLocationRole)
    {

        if (LOCATION_SECONDARY == currentLocationRole)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : secondary location role"));
        }
        else if (LOCATION_PRIMARY_UNCONFIRMED == currentLocationRole)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : primarly unconfirmed location role"));
        }
        else if (LOCATION_SECONDARY_UNCONFIRMED == currentLocationRole)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : secondary unconfirmed location role"));
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : should not come to here"));
        }

        // If mode transition is involved, explicitly inform cluster to remove this node's membership
        // Otherwise, don't send remove switch message because we still want to keep this node's membership in the cluster
        
        ResourceId oldClusterType = pVcsNodeLocalMessagingContext->getClusterType ();
        ResourceId newClusterType = pVcsNodeLocalMessagingContext->getNewClusterType ();

        if (oldClusterType != newClusterType)
        {
            ResourceId status = sendRemoveSwitchMessage (pVcsNodeLocalMessagingContext);

            if ( WAVE_MESSAGE_ERROR == status )
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : Sending VcsRemoveSwitchMessage to  JMP failed. vcsid change cannot be processed");
                return WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER;
            }
        }
        
        string ipAddress = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
        SI32   port      = FrameworkToolKit::getPortForLocationId (FrameworkToolKit::getThisLocationId ());

        FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);
        messageStatus = sendSynchronously (pMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : Error in sending UnconfigureSecondary message : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
            return messageStatus;
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : UnconfigureSecondary message Completion Status : \"" + FrameworkToolKit::localize (completionStatus) + "\"");
                return completionStatus;
            }
        }

        delete pMessage;
        return (WAVE_MESSAGE_SUCCESS);

    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep : cluster is still forming ") );
        return (CLUSTER_FORMATION_IS_IN_PROGRESS);
    }
}


void VcsFabricLocalObjectManager::getVcsIdMessageHandler (VcsNodeGetVcsIdMessage *pVcsNodeGetVcsIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getVcsIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetVcsIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::setRBridgeIdMessageHandler (VcsNodeSetRBridgeIdMessage *pVcsNodeSetRBridgeIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setRBridgeIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setRBridgeIdBackendStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetRBridgeIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->setNewRbridgeId (pVcsNodeSetRBridgeIdMessage->getRBridgeId ());

    pVcsNodeLocalMessagingContext->execute ();
}
        
void VcsFabricLocalObjectManager::getRBridgeIdMessageHandler (VcsNodeGetRBridgeIdMessage *pVcsNodeGetRBridgeIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getRBridgeIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetRBridgeIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::getRBridgeIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetRBridgeIdMessage *pVcsNodeGetRBridgeIdMessage = reinterpret_cast<VcsNodeGetRBridgeIdMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    pVcsNodeGetRBridgeIdMessage->setRBridgeId (getThisVcsNodeLocalManagedObjectDomainId ());
    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::setDomainIdMessageHandler (VcsNodeSetDomainIdMessage *pVcsNodeSetDomainIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setDomainIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetDomainIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

bool VcsFabricLocalObjectManager::determineIsDefaultConfigDetected ()
{
    // The configuration transaction number can be from either DCM's DB or from CCM's DB depending on which daemon this step is executed from.

    UI64 configurationTransactionId = DcmToolKit::getConfigurationTransactionId ();

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::determineIsDefaultConfigDetected : \"") + DcmToolKit::getApplicationIdentifier ()  + "\" configuration transaction number : " + configurationTransactionId);

    if (DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        // If this is MC or SA mode, no configuration data is stored in CCM's DB so we only need to check DCM's DB for a configuration change.
        // The above configuration transaction number is from DCM's DB.

        if (0 == configurationTransactionId)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::determineIsDefaultConfigDetected : Detected that the current configuration is already the default-config.  Preserving the configuration for a default-config is not necessary and so the configuration preservation processing will be skipped."));
            return true;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::determineIsDefaultConfigDetected : Detected a non-default configuration.  Preserving the configuration will commence."));
        }
    }
    else if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_ONLY == DcmToolKit::getDistributionMode ()))
    {
        // If this is FC mode, then we need to check both DCM's DB and CCM's DB for a configuration change:
        // CCM_ONLY: The above configuration transaction number is from CCM's DB, so we will also need to get the configuration transaction number from DCM's DB below. 
        // DCM_ONLY: The above configuration transaction number is from DCM's DB, so we will also need to get the configuration transaction number from CCM's DB below. 

        UI64 thisDaemonsConfigurationTransactionId  = configurationTransactionId;
        UI64 otherDaemonsConfigurationTransactionId = 0;
        UI16 otherDaemonsConfigurationNumberArray[16];

        ValClientSynchronousConnection *pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient ();

        ResourceId status = pValClientSynchronousConnection->getConfigurationNumber (otherDaemonsConfigurationNumberArray);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            // Rebuild a proper configuration number:
            // The configuration number vector is currently stored as a vector of four UI64 elements but converted to a vector of sixteen UI16 elements.
            // The actual "configuration number" is the second UI64 element or fifth through eighth UI16 elements. 
            // configurationNumberVector[4] : Lowest 16-bit nibble of the configuration number
            // configurationNumberVector[5] : Low middle 16-bit nibble of the configuration number
            // configurationNumberVector[6] : High middle 16-bit nibble of the configuration number
            // configurationNumberVector[7] : Highest 16-bit nibble of the configuration number

            for (UI32 i = 7; i > 3; i--)
            {
                otherDaemonsConfigurationTransactionId = (otherDaemonsConfigurationTransactionId << 16) | otherDaemonsConfigurationNumberArray[i];
            }
        }
        else
        {
            otherDaemonsConfigurationTransactionId = 1;

            trace (TRACE_LEVEL_WARN, string ("VcsFabricLocalObjectManager::determineIsDefaultConfigDetected : Failed to retrieve the configuration from the other daemon, forcing the configuration to a non-default value."));    
        }

        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::determineIsDefaultConfigDetected : \"other daemon's\" configuration number : ") + otherDaemonsConfigurationTransactionId);

        if ((0 == thisDaemonsConfigurationTransactionId) && (0 == otherDaemonsConfigurationTransactionId))
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::determineIsDefaultConfigDetected : Detected that the current configuration is already the default-config.  Preserving the configuration for a default-config is not necessary and so the configuration preservation processing will be skipped."));
            return true;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::determineIsDefaultConfigDetected : Detected a non-default configuration.  Preserving the configuration will commence."));
        }
    }
    else
    {
        // Do not do anything for other distribution modes for now.
    }

    return false;
}


void VcsFabricLocalObjectManager::configDistributeMessageHandler (VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage)
{
    ResourceId                      configDistributeOperation       = pVcsNodeConfigDistributeMessage->getConfigDistributeOperation ();
    ResourceId                      vcsClusterType                  = pVcsNodeConfigDistributeMessage->getVcsClusterType ();
    VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext = NULL;

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == vcsClusterType)
    {
        if (VCS_CONFIG_DISTRIBUTE_OPERATION_PREPARE_CLUSTER_TRANSITION == configDistributeOperation)
        {
            PrismSynchronousLinearSequencerStep clusterTransitionSequencerSteps[] =
            {   
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setSwitchDeletePrimaryAfterControlledFailover),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::replyPrimaryConfigDistributeStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::sendUnconfigureClusterSecondaryStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::changeClusterTypeConfigDistributeStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setupConfigAndRebootSecondariesStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
            };

            pVcsNodeConfigDistributeContext = new VcsNodeConfigDistributeContext (reinterpret_cast<PrismMessage *> (pVcsNodeConfigDistributeMessage), this, clusterTransitionSequencerSteps, sizeof (clusterTransitionSequencerSteps) / sizeof (clusterTransitionSequencerSteps[0]));
        }
        else if (VCS_CONFIG_DISTRIBUTE_OPERATION_PRIMARY_REBOOT == configDistributeOperation)
        {
            PrismSynchronousLinearSequencerStep rebootPrimarySequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setupConfigAndRebootPrimaryStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
            };

            pVcsNodeConfigDistributeContext = new VcsNodeConfigDistributeContext (reinterpret_cast<PrismMessage *> (pVcsNodeConfigDistributeMessage), this, rebootPrimarySequencerSteps, sizeof (rebootPrimarySequencerSteps) / sizeof (rebootPrimarySequencerSteps[0]));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::configDistributeMessageHandler : Invalid config distribute operation specified. Please set a valid operation. Operation Code : " + FrameworkToolKit::localizeToSourceCodeEnum (configDistributeOperation));

            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == vcsClusterType)
    {
        if (VCS_CONFIG_DISTRIBUTE_OPERATION_RETRIEVE_GLOBAL_CONFIGS == configDistributeOperation)
        {

            PrismSynchronousLinearSequencerStep prepareClusterTransitionSequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setupForGlobalOnlyConfigStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::retrieveGlobalConfigAndSendReplyStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
            };

            pVcsNodeConfigDistributeContext = new VcsNodeConfigDistributeContext (reinterpret_cast<PrismMessage *> (pVcsNodeConfigDistributeMessage), this, prepareClusterTransitionSequencerSteps, sizeof (prepareClusterTransitionSequencerSteps) / sizeof (prepareClusterTransitionSequencerSteps[0]));
       }
        else if (VCS_CONFIG_DISTRIBUTE_OPERATION_DISTRIBUTE_GLOBAL_CONFIGS == configDistributeOperation)
        {
            PrismSynchronousLinearSequencerStep prepareClusterTransitionSequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setupForGlobalOnlyConfigStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::retrieveContextFilteredConfigStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
            };

            pVcsNodeConfigDistributeContext = new VcsNodeConfigDistributeContext (reinterpret_cast<PrismMessage *> (pVcsNodeConfigDistributeMessage), this, prepareClusterTransitionSequencerSteps, sizeof (prepareClusterTransitionSequencerSteps) / sizeof (prepareClusterTransitionSequencerSteps[0]));
        }
        else if (VCS_CONFIG_DISTRIBUTE_OPERATION_PREPARE_CLUSTER_TRANSITION == configDistributeOperation)
        {
            PrismSynchronousLinearSequencerStep prepareClusterTransitionSequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setupForLocalOnlyConfigStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::retrieveContextFilteredConfigStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::doInterDcmCcmConfigDistributeStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
            };

            pVcsNodeConfigDistributeContext = new VcsNodeConfigDistributeContext (reinterpret_cast<PrismMessage *> (pVcsNodeConfigDistributeMessage), this, prepareClusterTransitionSequencerSteps, sizeof (prepareClusterTransitionSequencerSteps) / sizeof (prepareClusterTransitionSequencerSteps[0]));
        }
        else if (VCS_CONFIG_DISTRIBUTE_OPERATION_REBOOT == configDistributeOperation)
        {
            PrismSynchronousLinearSequencerStep rebootLocalNodeSequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::rebootLocalNodeStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
            };

            pVcsNodeConfigDistributeContext = new VcsNodeConfigDistributeContext (reinterpret_cast<PrismMessage *> (pVcsNodeConfigDistributeMessage), this, rebootLocalNodeSequencerSteps, sizeof (rebootLocalNodeSequencerSteps) / sizeof (rebootLocalNodeSequencerSteps[0]));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::configDistributeMessageHandler : Invalid config distribute operation specified.  Please set a valid handled operation.  Operation Code : " + FrameworkToolKit::localizeToSourceCodeEnum (configDistributeOperation));

            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::configDistributeMessageHandler : Invalid cluster type specified : " + FrameworkToolKit::localizeToSourceCodeEnum (vcsClusterType));

        prismAssert (false, __FILE__, __LINE__);
    }
    
    prismAssert (NULL != pVcsNodeConfigDistributeContext, __FILE__, __LINE__);

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::configDistributeMessageHandler : Current cluster type : " + FrameworkToolKit::localizeToSourceCodeEnum (vcsClusterType));
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::configDistributeMessageHandler : Message operation code : " + FrameworkToolKit::localizeToSourceCodeEnum (configDistributeOperation));

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::configDistributeMessageHandler : LocationId : ") + FrameworkToolKit::getThisLocationId () + ", LocationRole : " + FrameworkToolKit::localizeToSourceCodeEnum (FrameworkToolKit::getThisLocationRole ()));

    // transfer message inputs to context

    pVcsNodeConfigDistributeContext->setVcsClusterType          (vcsClusterType);
    pVcsNodeConfigDistributeContext->setPreserveConfig          (pVcsNodeConfigDistributeMessage->getPreserveConfig ());
    pVcsNodeConfigDistributeContext->setIsDefaultConfigDetected (pVcsNodeConfigDistributeMessage->getIsDefaultConfigDetected ());
    pVcsNodeConfigDistributeContext->setForce                   (pVcsNodeConfigDistributeMessage->getForce ());
    pVcsNodeConfigDistributeContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::retrieveGlobalConfigAndSendReplyStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{

    VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = dynamic_cast<VcsNodeConfigDistributeMessage *> (pVcsNodeConfigDistributeContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);
    
    bool        force                       = pVcsNodeConfigDistributeContext->getForce ();
    bool        isDefaultConfigDetected     = determineIsDefaultConfigDetected ();
    string      globalOnlyConfigData        = "";
    ResourceId  status                      = WAVE_MESSAGE_ERROR;

    if (false == force || (true == force && false == isDefaultConfigDetected)) 
    {
       status = getFilteredConfigData (DCM_CONFIG_FILTER_CONTEXT_GLOBAL, globalOnlyConfigData);
    }

    pVcsNodeConfigDistributeMessage->setClusterGlobalOnlyConfigData (globalOnlyConfigData);
    pVcsNodeConfigDistributeMessage->setIsDefaultConfigDetected (isDefaultConfigDetected);

    return (status);
}

ResourceId VcsFabricLocalObjectManager::setSwitchDeletePrimaryAfterControlledFailover (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::setSwitchDeletePrimaryAfterControlledFailover: Entering.");
    prismAssert (NULL != pVcsNodeConfigDistributeContext, __FILE__, __LINE__);
 
    //UI32  vcsClusterId  = 0;

    // Set the location role to help decide whether to unconfigure the node and/or reboot later
    pVcsNodeConfigDistributeContext->setLocationRole (FrameworkToolKit::getThisLocationRole ());

    if (DcmToolKit::fcPrincipalEnabled ())
    {
        //vcsClusterId = m_fcPrincipalTrackingWorker->getVcsClusterId (clusterExists);
    }
    else
    {
        //vcsClusterId = m_VcsPrincipalSelectionWorker->getVcsClusterId (clusterExists);
    }

    if (LOCATION_PRIMARY == pVcsNodeConfigDistributeContext->getLocationRole ())
    {
        startTransaction ();

        setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (true);

        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setSwitchDeletePrimaryAfterControlledFailover: Primary MO will be deleted after failover.");

        updateThisVcsNodeLocalManagedObject ();
        
        ResourceId status = commitTransaction ();

        if (FRAMEWORK_SUCCESS != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::setSwitchDeletePrimaryAfterControlledFailover: Failed to Update isPrincipal on the node, locationId = ") + FrameworkToolKit::getThisLocationId ());
            prismAssert (false, __FILE__, __LINE__);
        }   
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::replyPrimaryConfigDistributeStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::replyPrimaryConfigDistributeStep: Entering.");
    prismAssert (NULL != pVcsNodeConfigDistributeContext, __FILE__, __LINE__);

    VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = dynamic_cast<VcsNodeConfigDistributeMessage *> (pVcsNodeConfigDistributeContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);
    
    pVcsNodeConfigDistributeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsNodeConfigDistributeMessage);

    pVcsNodeConfigDistributeContext->setPPrismMessage (NULL);

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::replyInterDcmCcmConfigDistributeStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::replyInterDcmCcmConfigDistributeStep : Entering...");
    prismAssert (NULL != pVcsNodeConfigDistributeContext, __FILE__, __LINE__);

    VcsNodeInterDcmCcmConfigDistributeMessage *pVcsNodeInterDcmCcmConfigDistributeMessage = dynamic_cast<VcsNodeInterDcmCcmConfigDistributeMessage *> (pVcsNodeConfigDistributeContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeInterDcmCcmConfigDistributeMessage, __FILE__, __LINE__);

    pVcsNodeInterDcmCcmConfigDistributeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsNodeInterDcmCcmConfigDistributeMessage);

    pVcsNodeConfigDistributeContext->setPPrismMessage (NULL);
    
    return WAVE_MESSAGE_SUCCESS;
}   

ResourceId VcsFabricLocalObjectManager::sendUnconfigureClusterSecondaryStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::sendUnconfigureClusterSecondaryStep: Entering.");
    prismAssert (NULL != pVcsNodeConfigDistributeContext, __FILE__, __LINE__);

    LocationRole currentLocationRole = pVcsNodeConfigDistributeContext->getLocationRole ();

    if (LOCATION_SECONDARY == currentLocationRole)
    {
        string ipAddress = "";
        SI32   port      = 0;

        ipAddress = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
        port      = FrameworkToolKit::getPortForLocationId      (FrameworkToolKit::getThisLocationId ());

        FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);
        prismAssert (pMessage != NULL, __FILE__, __LINE__);

        ResourceId status = sendSynchronously (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::sendUnconfigureClusterSecondaryStep: Failed to send FrameworkObjectManagerUnconfigureClusterSecondaryMessage " + FrameworkToolKit::localize (status));
            delete pMessage;
            return WAVE_MESSAGE_ERROR;
        }
        else
        {
            ResourceId completionStatus = pMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::sendUnconfigureClusterSecondaryStep: FrameworkObjectManagerUnconfigureClusterSecondaryMessage processing failed " + FrameworkToolKit::localize (completionStatus));
                delete pMessage;
                return WAVE_MESSAGE_ERROR;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::sendUnconfigureClusterSecondaryStep: FrameworkObjectManagerUnconfigureClusterSecondaryMessage sent successfully for location: ") + FrameworkToolKit::localize (currentLocationRole));
                delete pMessage;
                return WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::sendUnconfigureClusterSecondaryStep: FrameworkObjectManagerUnconfigureClusterSecondaryMessage not sent for ") + FrameworkToolKit::localize (currentLocationRole));
        return WAVE_MESSAGE_SUCCESS;
    }
}

ResourceId VcsFabricLocalObjectManager::setupForGlobalOnlyConfigStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    pVcsNodeConfigDistributeContext->setContextFilteredConfigType (DCM_CONFIG_FILTER_CONTEXT_GLOBAL); 

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setupForLocalOnlyConfigStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    bool        preserveConfig              = pVcsNodeConfigDistributeContext->getPreserveConfig ();
    bool        isDefaultConfigDetected     = pVcsNodeConfigDistributeContext->getIsDefaultConfigDetected ();
    string      globalOnlyConfigFilePath    = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory() + "/" + ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ();
    string      localOnlyConfigFilePath     = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory() + "/" + ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();
 
    if (false == preserveConfig) 
    {
        ConfigFileManagementToolKit::deleteConfigurationFile (globalOnlyConfigFilePath);
        ConfigFileManagementToolKit::deleteConfigurationFile (localOnlyConfigFilePath);
    }
    else 
    {
        if (true == isDefaultConfigDetected)
        {
            ConfigFileManagementToolKit::deleteConfigurationFile (globalOnlyConfigFilePath);
        }
    }
    
    pVcsNodeConfigDistributeContext->setContextFilteredConfigType (DCM_CONFIG_FILTER_CONTEXT_LOCAL); 
    return (WAVE_MESSAGE_SUCCESS);

}

ResourceId VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = dynamic_cast<VcsNodeConfigDistributeMessage *> (pVcsNodeConfigDistributeContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);

    bool                        preserveConfig                              = pVcsNodeConfigDistributeContext->getPreserveConfig ();
    bool                        force                                       = pVcsNodeConfigDistributeContext->getForce ();
    ResourceId  configFilterContextType     = pVcsNodeConfigDistributeContext->getContextFilteredConfigType ();

    if (false == preserveConfig)
    {
        // Validating the global-only config is only necessary if user wants to preserve the config during a cluster transition.

        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Preserving the existing configuration is not enabled.  Skipping this step."));

        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        if ((true == force) && (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType))
        {
            // If force option is provided, bypass global config validation

            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Force option is provided. Skipping validate step."));

            return (WAVE_MESSAGE_SUCCESS);
        }
    }

    string                      clusterPrimaryGlobalOnlyConfigData          = pVcsNodeConfigDistributeMessage->getClusterGlobalOnlyConfigData ();
    string                      localNodesGlobalOnlyConfigData              = pVcsNodeConfigDistributeContext->getContextFilteredConfigData ();
    vector<string>              clusterPrimaryGlobalOnlyConfigDataVector;
    vector<string>              localNodesGlobalOnlyConfigDataVector;
    vector<string>              differenceInLocalMinusPrimary;
    vector<string>              differenceInPrimaryMinusLocal;
    UI32                        numberOfDifferences                         = 0;
    bool                        differencesFound                            = false;
    ResourceId                  status                                      = WAVE_MESSAGE_SUCCESS;              

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Principal node's Global-only config :\n") + clusterPrimaryGlobalOnlyConfigData);
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Local node's Global-only config :\n") + localNodesGlobalOnlyConfigData);

    StringUtils::tokenize (clusterPrimaryGlobalOnlyConfigData, clusterPrimaryGlobalOnlyConfigDataVector, '\n');
    StringUtils::tokenize (localNodesGlobalOnlyConfigData, localNodesGlobalOnlyConfigDataVector, '\n');

    // Find differences between local minus primary

    ConfigFileManagementToolKit::getDifference (localNodesGlobalOnlyConfigDataVector, clusterPrimaryGlobalOnlyConfigDataVector, differenceInLocalMinusPrimary);

    numberOfDifferences = differenceInLocalMinusPrimary.size ();

    if (0 < numberOfDifferences)
    {
        differencesFound = true;

        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Extra configurations found on local node:"));

        for (UI32 i = 0; i < numberOfDifferences; i++)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Difference Found [") + i + "]: " + differenceInLocalMinusPrimary[i]);
        }
    }

    // Find differences between primary minus local

    ConfigFileManagementToolKit::getDifference (clusterPrimaryGlobalOnlyConfigDataVector, localNodesGlobalOnlyConfigDataVector, differenceInPrimaryMinusLocal);

    numberOfDifferences = differenceInPrimaryMinusLocal.size ();

    if (0 < numberOfDifferences)
    {
        differencesFound = true;

        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Missing configurations found on local node:"));

        for (UI32 i = 0; i < numberOfDifferences; i++)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Difference Found [") + i + "]: " + differenceInPrimaryMinusLocal[i]);
        }
    }

    if (true == differencesFound)
    {
        status = VCS_CONFIG_DISTRIBUTE_ERROR_GLOBAL_CONFIG_VALIDATION_MISMATCH;

        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::validateGlobalOnlyConfigStep : Validation Failed.  Status :") + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::getFilteredConfigData (ResourceId configFilterContextType, string& filteredData)
{

    string      configFilterContextCommand  = "";

    // determine appropriate config filter command

    if (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType)
    {
        configFilterContextCommand = "vcs config-filter context global-only";
    }
    else if (DCM_CONFIG_FILTER_CONTEXT_LOCAL == configFilterContextType)
    {
        configFilterContextCommand = "vcs config-filter context local-only";
    }
    else
    {
        // No config filter applied

        trace (TRACE_LEVEL_WARN, string ("VcsFabricLocalObjectManager::getFilteredConfigData : No config filter applied.  Status : ") + FrameworkToolKit::localize (configFilterContextType));

        return (WAVE_MESSAGE_SUCCESS);
    }

    string          command                     = configFilterContextCommand + "\nshow running-config";
    vector<string>  commandOutput;
    SI32            systemCommandReturnCode     = 0;
    string          contextFilteredConfigData   = "";

    // get this node's filtered config from noscli

    systemCommandReturnCode = ConfigFileManagementToolKit::configFileManagementClientCommandOutput (command, commandOutput);

    if (0 != systemCommandReturnCode)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;

        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::getFilteredConfigData : system command : \"") + command + "\" failed, return code : " + systemCommandReturnCode);

        if (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType)
        {
            status = VCS_CONFIG_DISTRIBUTE_ERROR_GLOBAL_CONFIG_FETCH_FAILED; 
        }
        else if (DCM_CONFIG_FILTER_CONTEXT_LOCAL == configFilterContextType)
        {
            status = VCS_CONFIG_DISTRIBUTE_ERROR_LOCAL_CONFIG_FETCH_FAILED;
        }

        return (status);
    }

    UI32 commandOutputSize  = commandOutput.size ();
    UI32 i                  = 0; 

    for (i = 0; i < commandOutputSize; i++)
    {
        contextFilteredConfigData += commandOutput[i];  
    }   

    if ("" == contextFilteredConfigData)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;

        if (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType)
        {
            status = VCS_CONFIG_DISTRIBUTE_ERROR_GLOBAL_CONFIG_EMPTY;
        }
        else if (DCM_CONFIG_FILTER_CONTEXT_LOCAL == configFilterContextType)
        {
            status = VCS_CONFIG_DISTRIBUTE_ERROR_LOCAL_CONFIG_EMPTY;
        }

        return (status);
    }

    filteredData = contextFilteredConfigData;
    return WAVE_MESSAGE_SUCCESS; 
}


ResourceId VcsFabricLocalObjectManager::retrieveContextFilteredConfigStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    bool        preserveConfig              = pVcsNodeConfigDistributeContext->getPreserveConfig ();
    ResourceId  configFilterContextType     = pVcsNodeConfigDistributeContext->getContextFilteredConfigType ();
    bool        force                       = pVcsNodeConfigDistributeContext->getForce ();

    if (false == preserveConfig)
    {

        // Retrieving the filtered config is only necessary if user wants to preserve the config during a cluster transition.
        // Still allow global config validations to be processed.
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::retrieveContextFilteredConfigStep : Preserving the existing configuration is not enabled.  Skipping this step."));

        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        if ((true == force) && (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType))
        {
            // If force option is provided, bypass global config validation

            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::retrieveContextFilteredConfigStep :  Force option is provided. Skipping retrieve global config step."));

            return (WAVE_MESSAGE_SUCCESS);
        }
    }

    string contextFilteredConfigData = "";
    ResourceId status = getFilteredConfigData (configFilterContextType, contextFilteredConfigData);
    pVcsNodeConfigDistributeContext->setContextFilteredConfigData (contextFilteredConfigData);

    return (status);
}

ResourceId VcsFabricLocalObjectManager::saveFilteredConfigData (ResourceId configFilterContextType, const string& contextFilteredConfigData)
{
   string      contextFilteredConfigFilename   = "";

    if (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType)
    {
        contextFilteredConfigFilename = ConfigFileManagementToolKit::getConfigFileManagementGlobalOnlyFilename ();
    }
    else if (DCM_CONFIG_FILTER_CONTEXT_LOCAL == configFilterContextType)
    {
        contextFilteredConfigFilename = ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename ();
    }
    else
    {
        // No config filter applied

        trace (TRACE_LEVEL_WARN, string ("VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep : Skipping this step.  Status : ") + FrameworkToolKit::localize (configFilterContextType));

        return (WAVE_MESSAGE_SUCCESS);
    }

    string      contextFilteredConfigFullPath   = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/" + contextFilteredConfigFilename;
    ofstream    contextFilteredConfigFile;
    ResourceId  status                          = WAVE_MESSAGE_SUCCESS;

    // save this node's filtered config to file.
    
    contextFilteredConfigFile.open (contextFilteredConfigFullPath.c_str (), ios::out);

    if (true == contextFilteredConfigFile.is_open ())
    {
        contextFilteredConfigFile << contextFilteredConfigData;
        contextFilteredConfigFile.close ();

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        if (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType)
        {
            status = VCS_CONFIG_DISTRIBUTE_ERROR_GLOBAL_CONFIG_SAVE_FAILED;    
        }
        else if (DCM_CONFIG_FILTER_CONTEXT_LOCAL == configFilterContextType)
        {
            status = VCS_CONFIG_DISTRIBUTE_ERROR_LOCAL_CONFIG_SAVE_FAILED;    
        }

        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep : Failed status : ") + FrameworkToolKit::localize (status));

    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{

    bool preserveConfig             = pVcsNodeConfigDistributeContext->getPreserveConfig ();
    bool isDefaultConfigDetected    = pVcsNodeConfigDistributeContext->getIsDefaultConfigDetected ();
    ResourceId  configFilterContextType     = pVcsNodeConfigDistributeContext->getContextFilteredConfigType ();

    VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = dynamic_cast<VcsNodeConfigDistributeMessage *> (pVcsNodeConfigDistributeContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);

    if (false == preserveConfig)
    {
        // saving the filtered config is only necessary if user wants to preserve the config during a cluster transition.

        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep : Preserving the existing configuration is not enabled.  Skipping this step."));
        
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        if (DCM_CONFIG_FILTER_CONTEXT_GLOBAL == configFilterContextType)
        {

            if (true == isDefaultConfigDetected) 
            {
                // we don't need to save any global-only config files when the current config is already default.

                trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep : Default config already exists.  Skipping this step."));
                return (WAVE_MESSAGE_SUCCESS);
            }

            string      globalOnlyConfigData = pVcsNodeConfigDistributeMessage->getClusterGlobalOnlyConfigData ();
            ResourceId  status = saveFilteredConfigData (configFilterContextType, globalOnlyConfigData);
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep :  Context Filtered Config Data : \n") + globalOnlyConfigData);

            return (status);
        }
        else if (DCM_CONFIG_FILTER_CONTEXT_LOCAL == configFilterContextType)
        {
        
            string      contextFilteredConfigData       = pVcsNodeConfigDistributeContext->getContextFilteredConfigData ();
            ResourceId  status = saveFilteredConfigData (configFilterContextType, contextFilteredConfigData); 
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep : Context Filtered Config Data : \n") + contextFilteredConfigData);

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_WARN, string ("VcsFabricLocalObjectManager::saveContextFilteredConfigToFileStep : Unknown config filter context type."));
            return (WAVE_MESSAGE_SUCCESS);
        }    
    }
}

// VcsFabricLocalObjectManager::doInterDcmCcmConfigDistributeStep
// This sequencer step will get the inter-deamon (dcm->ccm / ccm->dcm) client connection
// and send a management interface message to the other deamon.  In this case it's
// CCM to DCM.  
ResourceId VcsFabricLocalObjectManager::doInterDcmCcmConfigDistributeStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    ResourceId status   = WAVE_MESSAGE_ERROR;

    ValClientSynchronousConnection *pInterDcmCcmClient = DcmToolKit::getInterDcmCcmClient ();

    prismAssert (NULL != pInterDcmCcmClient, __FILE__, __LINE__);

    VcsNodeInterDcmCcmConfigDistributeMessage *pVcsNodeInterDcmCcmConfigDistributeMessage = new VcsNodeInterDcmCcmConfigDistributeMessage ();

    prismAssert (NULL != pVcsNodeInterDcmCcmConfigDistributeMessage, __FILE__, __LINE__);

    pVcsNodeInterDcmCcmConfigDistributeMessage->setVcsClusterType (pVcsNodeConfigDistributeContext->getVcsClusterType ());

    status = pInterDcmCcmClient->sendSynchronouslyToWaveServer (pVcsNodeInterDcmCcmConfigDistributeMessage);

    delete pVcsNodeInterDcmCcmConfigDistributeMessage;

    return (status);
}

ResourceId VcsFabricLocalObjectManager::prepareForDefaultConfigStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    FrameworkToolKit::setIsStartupValid (false);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::removeCcmdCfgOnStandbyStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{

    ResourceId status = removeCcmdCfgOnStandby ();

    return status;

}

void VcsFabricLocalObjectManager::vcsNodeInterDcmCcmConfigDistributeMessageHandler (VcsNodeInterDcmCcmConfigDistributeMessage *pVcsNodeInterDcmCcmConfigDistributeMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::changeClusterTypeConfigDistributeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prepareForDefaultConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::copyDefaultToStartInModeTransitionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::haSyncUpdateAtLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::removeCcmdCfgOnStandbyStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::replyInterDcmCcmConfigDistributeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::rebootLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext = new VcsNodeConfigDistributeContext (reinterpret_cast<PrismMessage *> (pVcsNodeInterDcmCcmConfigDistributeMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeConfigDistributeContext->execute ();
}

void VcsFabricLocalObjectManager::copyDefaultStartTriggerMessageHandler (VcsNodeCopyDefaultStartTriggerMessage *pVcsNodeCopyDefaultStartTriggerMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::copyDefaultToStartStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::rebootLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeCopyDefaultStartTriggerMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->setVcsMode (pVcsNodeCopyDefaultStartTriggerMessage->getVcsMode ()); 

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::setIsPrincipalMessageHandler (VcsNodeSetIsPrincipalMessage *pVcsNodeSetIsPrincipalMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setIsPrincipalStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetIsPrincipalMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}


void VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageStaticHandler (VcsRemoveStandbyCfgFileMessage *pVcsRemoveStandbyCfgFileMessage)
{

    string distributionMode = pVcsRemoveStandbyCfgFileMessage->getDistributionMode ();
    string changeCfgValidityCommand = "";

    ResourceId status = WAVE_MESSAGE_ERROR;
    bool doInterConnection = false;

    if (!distributionMode.compare ("DCMD"))
    {
        // we will remove dcmd cfg here

        if (DCM_CCM == DcmToolKit::getDistributionMode () || DCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            status = FrameworkToolKit::changePrismConfigurationValidity (false);        
        }
        else
        {
            changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 9110 debug setPrismConfigurationValid false ";
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : connect to dcmd to remove cfg file");
            doInterConnection = true;
        }   
    }
    else if (!distributionMode.compare ("CCMD"))
    {
        // we will remove ccmd cfg here

        if (CCM_ONLY == DcmToolKit::getDistributionMode () || CCM_NOOP == DcmToolKit::getDistributionMode ())
        {
            status = FrameworkToolKit::changePrismConfigurationValidity (false);
        }
        else if (DCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 7110 debug setPrismConfigurationValid false ";
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : connect to ccmd to remove cfg file");
            doInterConnection = true;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : do nothing");
            status = WAVE_MESSAGE_SUCCESS;
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : unknown cfg distribution mode");
        status = WAVE_MESSAGE_ERROR;
    }

    if (true == doInterConnection) 
    {
        vector<string>  output;
        SI32        returnStatus = FrameworkToolKit::systemCommandOutput(changeCfgValidityCommand.c_str (), output);
        string outputString = "";

        if (0 < output.size ())
        {
            outputString = output[0];
        }

        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : returned with message : %s, and  return status : %d", outputString.c_str(), returnStatus); 
    
        if (0 == returnStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
        }
    }

    pVcsRemoveStandbyCfgFileMessage->setCompletionStatus (status);
}


void VcsFabricLocalObjectManager::syncConfigToKernelOnStandbyMessageHandler (VcsNodeHaSyncConfigToKernelMessaage *pVcsNodeHaSyncConfigToKernelMessaage)
{

    ResourceId status = WAVE_MESSAGE_ERROR;

    string     globalConfigs = pVcsNodeHaSyncConfigToKernelMessaage->getGlobalConfigs ();
    status = DcmToolKit::pushConfigToKernelOnLocalMM (globalConfigs);

    pVcsNodeHaSyncConfigToKernelMessaage->setCompletionStatus (status);
    reply (pVcsNodeHaSyncConfigToKernelMessaage);
}

void VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler (VcsRemoveStandbyCfgFileMessage *pVcsRemoveStandbyCfgFileMessage)
{

    string distributionMode = pVcsRemoveStandbyCfgFileMessage->getDistributionMode ();
    string changeCfgValidityCommand = "";

    ResourceId status = WAVE_MESSAGE_ERROR;
    bool doInterConnection = false;

    if (!distributionMode.compare ("DCMD"))
    {
        // we will remove dcmd cfg here

        if (DCM_CCM == DcmToolKit::getDistributionMode () || DCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            status = FrameworkToolKit::changePrismConfigurationValidity (false);        
        }
        else
        {
            changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 9110 debug setPrismConfigurationValid false ";
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : connect to dcmd to remove cfg file");
            doInterConnection = true;
        }   
    }
    else if (!distributionMode.compare ("CCMD"))
    {
        // we will remove ccmd cfg here

        if (CCM_ONLY == DcmToolKit::getDistributionMode () || CCM_NOOP == DcmToolKit::getDistributionMode ())
        {
            status = FrameworkToolKit::changePrismConfigurationValidity (false);
        }
        else if (DCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 7110 debug setPrismConfigurationValid false ";
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : connect to ccmd to remove cfg file");
            doInterConnection = true;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : do nothing");
            status = WAVE_MESSAGE_SUCCESS;
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : unknown cfg distribution mode");
        status = WAVE_MESSAGE_ERROR;
    }

    if (true == doInterConnection) 
    {
        vector<string>  output;
        SI32        returnStatus = FrameworkToolKit::systemCommandOutput(changeCfgValidityCommand.c_str (), output);
        string outputString = "";

        if (0 < output.size ())
        {
            outputString = output[0];
        }

        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileOnStandbyMessageHandler : returned with message : %s, and  return status : %d", outputString.c_str(), returnStatus); 
    
        if (0 == returnStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
        }
    }

    pVcsRemoveStandbyCfgFileMessage->setCompletionStatus (status);
    reply (pVcsRemoveStandbyCfgFileMessage);
}



void VcsFabricLocalObjectManager::getIsPrincipalMessageHandler (VcsNodeGetIsPrincipalMessage *pVcsNodeGetIsPrincipalMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::queryThisLocalManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getIsPrincipalStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetIsPrincipalMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->setMappedId (pVcsNodeGetIsPrincipalMessage->getMappedId());
    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getSwitchMacMessageHandler (VcsNodeGetSwitchMacMessage *pVcsNodeGetSwitchMacMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::queryThisLocalManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getSwitchMacStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetSwitchMacMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->setMappedId (pVcsNodeGetSwitchMacMessage->getMappedId());
    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getLocalMappedIdMessageHandler (VcsNodeGetLocalMappedIdMessage *pVcsNodeGetLocalMappedIdMessage)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::getLocalMappedIdMessageHandler : Entering.");
    
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getLocalMappedIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetLocalMappedIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getLocalMappedIdServiceIndependentStaticMessageHandler (VcsNodeGetLocalMappedIdMessage *pVcsNodeGetLocalMappedIdMessage)
{
    WaveNs::trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::getLocalMappedIdServiceIndependentStaticMessageHandler : Entering.");
    
    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();
    
    int domainId = pVcsNodeFabosInterfaceObjectManager->getDomainId ();

    if (0 > domainId)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::getLocalMappedIdServiceIndependentStaticMessageHandler : domainId : %d", domainId);
        domainId = 0;
    }

    UI32 ui32DomainId = (UI32) domainId;
    
    pVcsNodeGetLocalMappedIdMessage->setMappedId (ui32DomainId);
    
    pVcsNodeGetLocalMappedIdMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    
    // Static.
    // reply (pVcsNodeGetLocalMappedIdMessage);
}

void VcsFabricLocalObjectManager::setFcfMacMessageHandler (VcsNodeSetFcfMacMessage *pVcsNodeSetFcfMacMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setFcfMacStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetFcfMacMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->setFcfMac (pVcsNodeSetFcfMacMessage->getFcfMac());
    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::setVcsModeMessageHandler (VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage)
{
    bool                        vcs_mode                   = pVcsNodeSetVcsModeMessage->getVcsMode ();

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsModeMessageHandler: Entering....");
    if (vcs_mode) {
        vcsModeEnableMessageHandler(pVcsNodeSetVcsModeMessage);
    } else {
        vcsModeDisableMessageHandler(pVcsNodeSetVcsModeMessage);
    }
}

void VcsFabricLocalObjectManager::vcsNodeEnableValidationHandler(VcsNodeEnableValidationMessage *pVcsNodeEnableValidationMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsNodeEnableValidationHandler: Entering....");
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsEnableValidationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeEnableValidationMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::vcsModeEnableRunScript (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
	return (runVcsModeEnableScript ());
}

ResourceId VcsFabricLocalObjectManager::runVcsModeEnableScript ()
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::runVcsModeEnableScript: Executing VCS Enable script");
	vector<string>  output;
    /* Update the Primary partition */
    SI32 status = FrameworkToolKit::systemCommandOutput("/etc/fabos/Dcmd/vcs_set_mode.sh enable", output);
    /* Update the Secondary partition */
    status = FrameworkToolKit::systemCommandOutput("/etc/fabos/Dcmd/vcs_set_mode.sh enable /mnt", output);

    if (0 != status)
    {
        // FIXME : Sagar : Please fix this case.
    }

    // Send RASLog for Vcs Enable
    int vcsId = getThisVcsNodeLocalManagedObjectVcsId ();    

    raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1011, vcsId);

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::vcsModeDisableRunScript (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
	return (runVcsModeDisableScript ());
}

ResourceId VcsFabricLocalObjectManager::runVcsModeDisableScript ()
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::runVcsModeDisableScript: Executing VCS Disable script");
	vector<string>  output;
    /* Update the Primary partition */
    SI32 status = FrameworkToolKit::systemCommandOutput("/etc/fabos/Dcmd/vcs_set_mode.sh disable", output);
    /* Update the Secondary partition */
    status = FrameworkToolKit::systemCommandOutput("/etc/fabos/Dcmd/vcs_set_mode.sh disable /mnt", output);

    if (0 != status)
    {
        // FIXME : Sagar : Please fix this case.
    }

    // Send RASLog for Vcs Disable
    int vcsId = getThisVcsNodeLocalManagedObjectVcsId ();    

    raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1012, vcsId);
    
    return WAVE_MESSAGE_SUCCESS;
}

void VcsFabricLocalObjectManager::vcsModeEnableMessageHandler(VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsModeEnableMessageHandler: Entering....");
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsModeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsConfStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsModeEnableRunScript),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::copyDefaultToStartInVcsModeEnableStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::haSyncUpdateAtLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::removeCcmdCfgOnStandbyStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::rebootLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetVcsModeMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::vcsModeDisableMessageHandler(VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsModeDisableMessageHandler: Entering....");
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsDisableValidationStep),        
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prepareForVcsModeDisableStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::unconfigureSecondaryStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsDisableStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::disableVcsModeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsDisableCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setRBridgeIdBackendStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setConfFilesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsModeDisableRunScript),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::copyDefaultToStartInVcsModeDisableStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::disconnectPrincipalFromAllLocationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::unconfigurePrimaryStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismLinearSequencerSucceededStepForNoVcsEnable),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForNoVcsEnable)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetVcsModeMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getVcsModeMessageHandler (VcsNodeGetVcsModeMessage *pVcsNodeGetVcsModeMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getVcsModeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetVcsModeMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForEnqSwitchMessage (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{

	ResourceId completionStatus = pPrismSynchronousLinearSequencerContext->getCompletionStatus ();
	
	VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep( pPrismSynchronousLinearSequencerContext );

	if (REPLACEMENT_SWITCH_MODEL_MISMATCH == completionStatus)
	{
	
	    trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForEnqSwitchMessage: ") + FrameworkToolKit::localize (completionStatus));
		sleep (1); // wait to ensure fabos doesn't disable the chassis before the reply message is sent
		int rc = chassisdisable();
		
		if (0 == rc)
		{

			 VcsFabricLocalObjectManager * pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance();
			 pVcsFabricLocalObjectManager->startTransaction ();
			 
			 pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();
			 pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectSpecificStatus (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_CHASSIS_DISABLED_REPLACEMENT_MODEL_MISMATCH);
			 ResourceId commitStatus = pVcsFabricLocalObjectManager->commitTransaction ();
			 
			 if (FRAMEWORK_SUCCESS != commitStatus)
			 {
				 WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForEnqSwitchMessage: update node specific status commit transaction failed:")+ FrameworkToolKit::localize (commitStatus));
			 }
			 
			 trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForEnqSwitchMessage: chassis disabled due to replace model mismatch"));
			 // now raslog this event
			 WorldWideName myWwn = DcmToolKit::getSwitchWwnFromDomainId(DcmToolKit::getThisVcsNodeDomainId());
			 raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1014, (myWwn.toString ()).c_str ());
		}
		else
		{
			 trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForEnqSwitchMessage: replace switch model mismatch but Chassis Disable failed so asserting: return code from chassisdisable ca	   ll = ")+rc);
			 prismAssert(false,__FILE__,__LINE__);
		}

	}
}

void VcsFabricLocalObjectManager::enqSwitchMessageHandler (VcsEnqSwitchMessage *pVcsEnqSwitchMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::VcsEnqSwitchMessageHandler : Entering ...");

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::validateClusterOperationInProgress),
		reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::validateSwitchReplacementCaseStep),
        //reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::queryThisLocalManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getSwitchParamsStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForEnqSwitchMessage)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsEnqSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pVcsNodeLocalMessagingContext->execute ();
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::VcsEnqSwitchMessageHandler : finished processing enq Switch message");
}

void  VcsFabricLocalObjectManager::vcsClusterMergeMessageHandler   (VcsClusterMergeMessage *pVcsClusterMergeMessage)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::vcsClusterMergeMessageHandler: Entering...");

    ResourceId currentVcsFabricLocalState = VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ();

    if ( (FrameworkToolKit::getSecondaryClusterFormationFlag ()) || (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == currentVcsFabricLocalState) || (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS == currentVcsFabricLocalState) || (VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == currentVcsFabricLocalState) || (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == currentVcsFabricLocalState) || (VCS_FABRIC_LOCAL_CLUSTER_SWITCH_OFFLINE_BUT_ADD_REJOIN_IN_PROGRESS == currentVcsFabricLocalState))
    {
        pVcsClusterMergeMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS);
        reply (pVcsClusterMergeMessage);

        return;
    }

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_CLUSTER_MERGE_REQUEST_IN_PROGRESS);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabricLocalObjectManager::sendClusterMergeMessageToFramework),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabricLocalObjectManager::resetThisNodeStateForClusterMerge),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabricLocalObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabricLocalObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (dynamic_cast<PrismMessage *> (pVcsClusterMergeMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();

    if (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {    
        // no need to do any thing. This is possible that we received reply after node is shutdown
        return;
    }    

    if (VCS_FABRIC_LOCAL_CLUSTER_MERGE_REQUEST_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
    }
}

void  VcsFabricLocalObjectManager::vcsFabricModeChangeParametersMessageHandler (VcsNodeSetModeChangeParametersMessage *pVcsNodeSetModeChangeParametersMessage)
{
    WaveNs::trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::vcsFabricModeChangeParametersMessageHandler: Entering.");
    VcsNodeLocalMessagingContext  *pVcsNodeLocalMessagingContext = NULL;
 
    if (pVcsNodeSetModeChangeParametersMessage->getRebootSw () == true)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsConfModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setRbridgeIdModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setClusterConfigModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setStartupConfigModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::deleteConfigFilesStep),
            //reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::haSyncUpdateAtLocalNodeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::rebootLocalNodeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetModeChangeParametersMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    }
    else
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsConfModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setRbridgeIdModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setClusterConfigModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setStartupConfigModeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::deleteConfigFilesStep),
            //reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::haSyncUpdateAtLocalNodeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetModeChangeParametersMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationMessageHandler (VcsNodeSetClusterConfigurationMessage *pVcsNodeSetClusterConfigurationMessage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationMessageHandler: Entering.");

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::validateSetClusterConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::savePreviousConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prepareForSetClusterConfigurationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsIdVcsConfStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setRBridgeIdBackendStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setVcsIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setRBridgeIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::changeClusterTypeAtLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::copyDefaultToStartStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::haSyncUpdateAtLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::removeCcmdCfgOnStandbyStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::rebootLocalNodeStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetClusterConfigurationMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->setNewRbridgeId      (pVcsNodeSetClusterConfigurationMessage->getRbridgeId ());
    pVcsNodeLocalMessagingContext->setNewVcsId          (pVcsNodeSetClusterConfigurationMessage->getVcsId ());
    pVcsNodeLocalMessagingContext->setVcsMode           (pVcsNodeSetClusterConfigurationMessage->getVcsMode ());
    pVcsNodeLocalMessagingContext->setNewClusterType    (pVcsNodeSetClusterConfigurationMessage->getClusterType ());

    pVcsNodeLocalMessagingContext->execute ();
}

bool VcsFabricLocalObjectManager::isClusterFormationInProgress ()
{

    bool inProgress = false; 

    m_vcsFabricLocalObjectManagerStateMutex.lock (); 

    if ( VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == m_vcsFabricLocalObjectManagerState || VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS == m_vcsFabricLocalObjectManagerState || VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == m_vcsFabricLocalObjectManagerState || true == FrameworkToolKit::getSecondaryClusterFormationFlag ())
    {
        inProgress = true;
    }
    
    m_vcsFabricLocalObjectManagerStateMutex.unlock ();

    return inProgress;
}

ResourceId VcsFabricLocalObjectManager::changeClusterTypeAtLocalNodeStep (VcsNodeLocalMessagingContext * pVcsNodeLocalMessagingContext)
{

    ResourceId oldClusterType = pVcsNodeLocalMessagingContext->getClusterType ();
    ResourceId newClusterType = pVcsNodeLocalMessagingContext->getNewClusterType ();

    if (oldClusterType == newClusterType)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::changeClusterTypeAtLocalNodeStep : new cluster type is same than old cluster type, skip this step");
        return WAVE_MESSAGE_SUCCESS;
    }

    // explicitly change to fabric cluster

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == newClusterType)
    {
        trace(TRACE_LEVEL_INFO,"VcsFabricLocalObjectManager::changeClusterTypeAtLocalNodeStep: Changing cluster type from Management to Fabric Cluster.");
        if (WAVE_MESSAGE_SUCCESS != setClusterConfFile (false))
        {
            return (WAVE_MESSAGE_ERROR);
        }

        setThisVcsNodeClusterType (true, false);
    }
  
    // explicitly change to management cluster

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == newClusterType)
    {
        trace(TRACE_LEVEL_INFO,"VcsFabricLocalObjectManager::changeClusterTypeAtLocalNodeStep: Changing cluster type from Fabric to Management Cluster.");
        if (WAVE_MESSAGE_SUCCESS != setClusterConfFile (true))
        {
            return (WAVE_MESSAGE_ERROR);
        }

        setThisVcsNodeClusterType (true, true);
    }
    
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationCommitTransactionStep (VcsNodeLocalMessagingContext * pVcsNodeLocalMessagingContext)
{

    VcsNodeSetClusterConfigurationMessage *pVcsNodeSetClusterConfigurationMessage = reinterpret_cast<VcsNodeSetClusterConfigurationMessage*> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    ResourceId status = prismSynchronousLinearSequencerCommitTransactionStep (pVcsNodeLocalMessagingContext);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        pVcsNodeSetClusterConfigurationMessage->setCommitTransactionCompleted (true);
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationCommitTransactionStep : Succeed");
    }
    
    return status; 
}

ResourceId VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep (VcsNodeLocalMessagingContext * pVcsNodeLocalMessagingContext)
{

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep : Entering......");

    ResourceId completionStatus = pVcsNodeLocalMessagingContext->getCompletionStatus ();

    if ((SWITCH_IN_FAULTY_STATE == completionStatus) || (CLUSTER_FORMATION_IS_IN_PROGRESS == completionStatus) || (WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER == completionStatus))
    {
        return prismSynchronousLinearSequencerFailedStep (pVcsNodeLocalMessagingContext);
    }

    VcsNodeSetClusterConfigurationMessage *pVcsNodeSetClusterConfigurationMessage = reinterpret_cast<VcsNodeSetClusterConfigurationMessage*> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
   
    bool        isCommitTransactionCompleted   = pVcsNodeSetClusterConfigurationMessage->getCommitTransactionCompleted (); 
    UI32        previous_rbridge_id            = pVcsNodeLocalMessagingContext->getDomainId ();
    UI32        previous_vcs_id                = pVcsNodeLocalMessagingContext->getVcsId ();
    ResourceId  previous_cluster_type          = pVcsNodeLocalMessagingContext->getClusterType ();  

    // restore vcs config file 
    setVcsConfFile (true, previous_vcs_id);

    // restore rbridgeid backend
    if ( previous_rbridge_id == 0 )
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep : Roll back rbridgeId cannot be 0. Hence returning without setting backend RbridgeId ");
    }
    setRBridgeIdBackend (previous_rbridge_id);

    // restore cluster type
    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == previous_cluster_type)
    {
        setClusterConfFile (false);
        setThisVcsNodeClusterType (true, false);
    }
    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == previous_cluster_type)
    {
        setClusterConfFile (true);
        setThisVcsNodeClusterType (true, true);
    }

    // restore DB and in-memory object
    if (false == isCommitTransactionCompleted)
    {
        if (getThisVcsNodeLocalManagedObjectDomainId() != previous_rbridge_id)
        {
            trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep : rbridgeId roll back to ") + previous_rbridge_id);
            VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();
            pVcsNodeFabosInterfaceObjectManager->setDomainId (previous_rbridge_id);

            setThisVcsNodeLocalManagedObjectDomainId (previous_rbridge_id);
        }

        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep : vcsId roll back to ") + previous_vcs_id); 
        setThisVcsNodeLocalManagedObjectVcsId (previous_vcs_id);
    }
    else
    {
        prismSynchronousLinearSequencerStartTransactionStep (pVcsNodeLocalMessagingContext);
        
        if (getThisVcsNodeLocalManagedObjectDomainId() != previous_rbridge_id)
        {
            trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep : rbridgeId roll back to ") + previous_rbridge_id);
            VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();
            pVcsNodeFabosInterfaceObjectManager->setDomainId (previous_rbridge_id);

            updateThisVcsNodeLocalManagedObject ();
            setThisVcsNodeLocalManagedObjectDomainId (previous_rbridge_id);
        } 
      
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep : vcsId roll back to ") + previous_vcs_id);
        updateThisVcsNodeLocalManagedObject ();
        setThisVcsNodeLocalManagedObjectVcsId (previous_vcs_id);

        prismSynchronousLinearSequencerCommitTransactionStep (pVcsNodeLocalMessagingContext);
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::vcsFabricSetClusterConfigurationFailedStep : DB roll back successfully "));
        
    } 

    ResourceId status = prismSynchronousLinearSequencerFailedStep (pVcsNodeLocalMessagingContext); 

    if (DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        VcsFabricLocalObjectManager::removeCcmdCfg();
        VcsFabricLocalObjectManager::removeDcmdCfg();
        VcsFabricLocalObjectManager::removeCcmdCfgOnStandby ();
        VcsFabricLocalObjectManager::removeDcmdCfgOnStandby ();
        trace (TRACE_LEVEL_FATAL, string ("set cluster configuration failed, cfg has been cleaned up to prevent any discrepancy."));
        prismAssert (false, __FILE__, __LINE__);
    }

    return status;
}

ResourceId VcsFabricLocalObjectManager::validateSetClusterConfigurationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext) 
{
    if (0 == pVcsNodeLocalMessagingContext->getVcsMode ())
    {
        // vcsid or rbridgeid change should only be allowed in FC or MC
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::validateSetClusterConfigurationStep : Since vcs mode is disabled, we cannot set vcsid or rbridgeid");
        return WAVE_MESSAGE_ERROR;
    }

    if (switchFaulty())
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::validateSetClusterConfigurationStep : Switch is in faulty state");
        return SWITCH_IN_FAULTY_STATE;
    }

    if (DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        if (true == VcsFabricLocalObjectManager::isClusterFormationInProgress ()) 
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::validateSetClusterConfigurationStep : cluster formation is in progress");
            return CLUSTER_FORMATION_IS_IN_PROGRESS;
        } 
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::savePreviousConfigurationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    //save old parameters here

    pVcsNodeLocalMessagingContext->setDomainId (DcmToolKit::getThisVcsNodeDomainId ());
    pVcsNodeLocalMessagingContext->setVcsId (DcmToolKit::getVcsId ());    
    
    if (DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        pVcsNodeLocalMessagingContext->setClusterType (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT);                
    }
    else if (DCM_ONLY == DcmToolKit::getDistributionMode () || CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        pVcsNodeLocalMessagingContext->setClusterType (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::savePreviousConfigurationStep : previous cluster type should not be standalone");
        return WAVE_MESSAGE_ERROR;
    }
    
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::savePreviousConfigurationStep : succeed");

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::copyDefaultToStartStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    bool        vcsMode                 = pVcsNodeLocalMessagingContext->getVcsMode ();
    ResourceId  status                  = WAVE_MESSAGE_SUCCESS;
    string      messageString           = "";

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::copyDefaultToStartStep : vcsMode is ") + vcsMode);

    status = copyDefaultToStartInModeChange (vcsMode, false, messageString);

    return status;
}

void VcsFabricLocalObjectManager::resetThisNodeStateForClusterMerge (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::resetThisNodeStateForClusterMerge: resetting this node state to VCS_FABRIC_LOCAL_SPECIFIC_STATUS_WAITING_TO_REJOIN");

    setThisVcsNodeLocalManagedObjectSpecificStatus (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_WAITING_TO_REJOIN);
    setThisVcsNodeLocalManagedObjectIsPrincipal    (false);

    startTransaction ();

    updateThisVcsNodeLocalManagedObject ();

    ResourceId status = commitTransaction ();

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::resetThisNodeStateForClusterMerge: commit transaction error resetting this node state to VCS_FABRIC_LOCAL_SPECIFIC_STATUS_WAITING_TO_REJOIN status = ")
            +FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS); 
}

void VcsFabricLocalObjectManager::sendClusterMergeMessageToFramework (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::sendClusterMergeMessageToFramework: Entering...");
    
    // sendResetNodeStateMessageToFramework
    FrameworkObjectManagerResetNodeToUnconfirmRole *pFrameworkObjectManagerResetNodeToUnconfirmRole = new FrameworkObjectManagerResetNodeToUnconfirmRole ();

    prismAssert (NULL != pFrameworkObjectManagerResetNodeToUnconfirmRole, __FILE__, __LINE__);
    
    ResourceId status = send (pFrameworkObjectManagerResetNodeToUnconfirmRole, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabricLocalObjectManager::sendClusterMergeMessageToFrameworkCallback), pPrismLinearSequencerContext);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::sendClusterMergeMessageToFramework: FrameworkObjectManagerResetNodeToUnconfirmRole send message failed"+FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
}

void VcsFabricLocalObjectManager::sendClusterMergeMessageToFrameworkCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    FrameworkObjectManagerResetNodeToUnconfirmRole *pMessage                      = dynamic_cast<FrameworkObjectManagerResetNodeToUnconfirmRole *> (pPrismMessage);
    PrismLinearSequencerContext                    *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pContext);

    prismAssert (NULL != pMessage, __FILE__, __LINE__);
    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
    prismAssert (FRAMEWORK_SUCCESS == frameworkStatus, __FILE__, __LINE__);

    if (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS == pMessage->getCompletionStatus () && VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {    
        // no need to do any thing. This is possible that we received reply after node is shutdown
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS);
        delete pMessage;

        return;
    }    

    prismAssert (WAVE_MESSAGE_SUCCESS == pMessage->getCompletionStatus (), __FILE__, __LINE__);

    delete pMessage;

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

/** 
 *Name:         isNodeReadyForSCNHandling
 *Description:  This function is used to determine if SCN handling 
 *              should be postponed.
 * 
 *@param1:      none
 *              
 *@return:      value of VcsNodeWorker's isNodeReadyForSCNHandling data member
 */
bool VcsFabricLocalObjectManager::isNodeReadyForSCNHandling () const
{
    bool readyToAcceptScn = true;

    if (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        readyToAcceptScn = false;
    }

    return (readyToAcceptScn);
}

bool VcsFabricLocalObjectManager::isNodeReadyForSlotSCNHandling () const
{
    return (m_isNodeReadyForSlotScn);
}

void VcsFabricLocalObjectManager::getIsDcmdReadyForHasmLaunchMessageHandler (VcsNodeGetIsDcmdReadyForHasmLaunchMessage *pVcsNodeGetIsDcmdReadyForHasmLaunchMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getIsDcmdReadyForHasmLaunchStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetIsDcmdReadyForHasmLaunchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::getIsDcmdReadyForHasmLaunchStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetIsDcmdReadyForHasmLaunchMessage *pVcsNodeGetIsDcmdReadyForHasmLaunchMessage = reinterpret_cast<VcsNodeGetIsDcmdReadyForHasmLaunchMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    pVcsNodeGetIsDcmdReadyForHasmLaunchMessage->setIsDcmdReadyForHasmLaunch (VcsNodeFabosInterfaceObjectManager::getIsDcmdReadyForHasmLaunch());

    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::setIsHasmLaunchedMessageHandler (VcsNodeSetIsHasmLaunchedMessage *pVcsNodeSetIsHasmLaunchedMessage) 
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setIsHasmLaunchedStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetIsHasmLaunchedMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::setIsHasmLaunchedStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext) 
{
    VcsNodeSetIsHasmLaunchedMessage *pVcsNodeSetIsHasmLaunchedMessage = reinterpret_cast<VcsNodeSetIsHasmLaunchedMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    
    VcsNodeFabosInterfaceObjectManager::setIsHasmLaunched (pVcsNodeSetIsHasmLaunchedMessage->getIsHasmLaunched ());

    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::getIsReadyMessageHandler (VcsNodeGetIsReadyMessage *pVcsNodeGetIsReadyMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getIsReadyStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetIsReadyMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::setIsReadyMessageHandler (VcsNodeSetIsReadyMessage *pVcsNodeSetIsReadyMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setIsReadyStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetIsReadyMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::generateSlotScnMessageHandler (VcsNodeGenerateSlotScnMessage *pVcsNodeGenerateSlotScnMessage)
{
    
    prismAssert (NULL != pVcsNodeGenerateSlotScnMessage, __FILE__, __LINE__);
    UI32 slotNumber = pVcsNodeGenerateSlotScnMessage->getSlotNumber (); 
    UI32 objectType = pVcsNodeGenerateSlotScnMessage->getObjectType ();   
    UI32 hwType = pVcsNodeGenerateSlotScnMessage->getHwType (); 
    UI32 hwId = pVcsNodeGenerateSlotScnMessage->getHwId ();  
    UI32 slotState = pVcsNodeGenerateSlotScnMessage->getSlotState ();  
    UI32 reason = pVcsNodeGenerateSlotScnMessage->getReason ();  
    UI32 version = pVcsNodeGenerateSlotScnMessage->getVersion (); 
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::generateSlotScnMessageHandler : Handling slot scn message slot state = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" reason = "+reason+" version = "+version);
    
    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleSlotEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version);
    pVcsNodeGenerateSlotScnMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeGenerateSlotScnMessage);
}

void VcsFabricLocalObjectManager::generateSCNMessageHandler (VcsNodeGenerateSCNMessage *pVcsNodeGenerateSCNMessage)
{

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::generateSCNMessageHandler : Handling for Domain ") + pVcsNodeGenerateSCNMessage->getDomainId ());

    switch (pVcsNodeGenerateSCNMessage->getSCNType ())
    {
        case 0:
            {
                (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_VALID, pVcsNodeGenerateSCNMessage->getDomainId (), pVcsNodeGenerateSCNMessage->getPDomainId());
                break;
            }

        case 1:
            {
                (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE, pVcsNodeGenerateSCNMessage->getDomainId (), 0);
                break;
            }

        case 2:
            {
                (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_INVALID, pVcsNodeGenerateSCNMessage->getDomainId (), 0);
                break;
            }

        case 3:
            {
                (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_REACHABLE, pVcsNodeGenerateSCNMessage->getDomainId (), 0);
                break;
            }

        case 4:
            {
                (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_UNREACHABLE, pVcsNodeGenerateSCNMessage->getDomainId (),0);
                break;
            }
        case 6:
	        {
	            (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleZoneChanged ();
	            break;
	        }
	        break;
        case 7:
	        {
	            (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleZoneAborted ();
	            break;
	        }
	        break;
        case 8:
            {
                (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleSwitchBootupStateUpdate (0);
                break;
            }
            break;
        case 9:
            {
                (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_SWITCH_OFFLINE, 0, 0);
                break;
            }
        default:
            break;
    }

    pVcsNodeGenerateSCNMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeGenerateSCNMessage);
}

void VcsFabricLocalObjectManager::syncDumpStartEventHandler (const VcsNodeFabosSyncStartEvent *&pVcsNodeFabosSyncStartEvent)
{
    WaveNs::trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::syncDumpStartEventHandler: entering...");

    if (false == isNodeReadyForSCNHandling ())
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::syncDumpStartEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pVcsNodeFabosSyncStartEvent));
        return;
    }

    prismAssert (NULL != pVcsNodeFabosSyncStartEvent, __FILE__, __LINE__);

    string peerHaIpAddress = pVcsNodeFabosSyncStartEvent->getPeerHaIpAddress ();
    SI32 peerHaIpPort      = pVcsNodeFabosSyncStartEvent->getPeerHaIpPort ();
    UI32 eventId           = pVcsNodeFabosSyncStartEvent->getEventId ();

    ResourceId status = notifyStartSyncDumpHandler (peerHaIpAddress, peerHaIpPort, eventId);
    
    if (WAVE_MESSAGE_SUCCESS == status)
    {
        WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::syncDumpStartEventHandler: syncDump is successful.");        
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_WARN, "VcsFabricLocalObjectManager::syncDumpStartEventHandler: sync dump start encountered a failure.");
    }

    reply (reinterpret_cast<const PrismEvent *&> (pVcsNodeFabosSyncStartEvent));
}

ResourceId VcsFabricLocalObjectManager::notifyStartSyncDumpHandler (const string& peerHaIpAddress, const SI32 peerHaIpPort, const UI32 eventId)
{
    ResourceId  status   = WAVE_MESSAGE_ERROR;

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(eventId);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_ERROR, true, false, "VcsFabricLocalObjectManager::notifyStartSyncDumpHandler: Event [%u] not in queue so skipping handling this event", eventId);

        return (status);
    }

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::notifyStartSyncDumpHandler : peerIp: ") + peerHaIpAddress + string("peerPort: ") + peerHaIpPort); 

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        // Set the HA sync in progress flag to block the handling of vcs mode changes
        m_isHaSyncInProgress = true;

        FrameworkObjectManagerStartHaSyncMessage *pFrameworkObjectManagerStartHaSyncMessage = new FrameworkObjectManagerStartHaSyncMessage (peerHaIpAddress, peerHaIpPort);

        prismAssert (pFrameworkObjectManagerStartHaSyncMessage != NULL, __FILE__, __LINE__);

        UI32 context = (DcmToolKit::isIssuInProgress () == true)? SYNC_DUMP_ISSU_CONTEXT : SYNC_DUMP_CONTEXT;

        pFrameworkObjectManagerStartHaSyncMessage->addBuffer (CONTEXT_INFO, sizeof (UI32), (void *) &context, false);

        status = send (pFrameworkObjectManagerStartHaSyncMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabricLocalObjectManager::notifyStartHaSyncMessageCallback), NULL);
    }
    else if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        FrameworkObjectManagerStartCcmdHaSyncMessage *pFrameworkObjectManagerStartCcmdHaSyncMessage = new FrameworkObjectManagerStartCcmdHaSyncMessage (peerHaIpAddress, peerHaIpPort);

        prismAssert (pFrameworkObjectManagerStartCcmdHaSyncMessage != NULL, __FILE__, __LINE__);

        UI32 context = (DcmToolKit::isIssuInProgress () == true)? CCMD_SYNC_DUMP_ISSU_CONTEXT : CCMD_SYNC_DUMP_CONTEXT;

        pFrameworkObjectManagerStartCcmdHaSyncMessage->addBuffer (CONTEXT_INFO, sizeof (UI32), (void *) &context, false);

        status = send (pFrameworkObjectManagerStartCcmdHaSyncMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabricLocalObjectManager::notifyStartCcmdHaSyncMessageCallback), NULL); 
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
         trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::notifyStartSyncDumpHandler: Error sending startSync message.");

         prismAssert (false, __FILE__, __LINE__);
    }

    return status;
}

void VcsFabricLocalObjectManager::notifyStartHaSyncMessageCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerStartHaSyncMessage *pFrameworkObjectManagerStartHaSyncMessage, void *pContext)
{
    SI32 ret = 0;

    // Unset the HA sync flag to allow vcs mode processing
    m_isHaSyncInProgress = false;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pFrameworkObjectManagerStartHaSyncMessage != NULL, __FILE__, __LINE__);
        ResourceId completionStatus = pFrameworkObjectManagerStartHaSyncMessage->getCompletionStatus ();
        delete pFrameworkObjectManagerStartHaSyncMessage;
        if (WAVE_MESSAGE_SUCCESS == completionStatus) 
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::StartHaSyncMessageCallback  sync succeeded"));
		}
        else
        {
            ret = -1;
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::StartHaSyncMessageCallback  sync failed: ")+FrameworkToolKit::localize (completionStatus));
        }
        VcsNodeFabosInterfaceObjectManager::dcmHaSyncDumpEnd(ret);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::StartHaSyncMessageCallback frameworkStatus error: ")+FrameworkToolKit::localize (frameworkStatus));
        prismAssert (false, __FILE__, __LINE__);
    }

}

void VcsFabricLocalObjectManager::notifyStartCcmdHaSyncMessageCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerStartCcmdHaSyncMessage *pFrameworkObjectManagerStartCcmdHaSyncMessage, void *pContext)
{
    SI32 ret = 0;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pFrameworkObjectManagerStartCcmdHaSyncMessage != NULL, __FILE__, __LINE__);

        ResourceId completionStatus = pFrameworkObjectManagerStartCcmdHaSyncMessage->getCompletionStatus ();

        delete pFrameworkObjectManagerStartCcmdHaSyncMessage;

        if (WAVE_MESSAGE_SUCCESS == completionStatus) 
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::notifyStartCcmdHaSyncMessageCallback  sync succeeded"));
		}
        else
        {
            ret = -1;

            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::notifyStartCcmdHaSyncMessageCallback  sync failed: ")+FrameworkToolKit::localize (completionStatus));
        }

        VcsNodeFabosInterfaceObjectManager::dcmHaSyncDumpEnd(ret);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::StartCcmdHaSyncMessageCallback frameworkStatus error: ")+FrameworkToolKit::localize (frameworkStatus));

        prismAssert (false, __FILE__, __LINE__);
    }

}

void VcsFabricLocalObjectManager::setSwitchWWNMessageHandler (VcsNodeSetSwitchWWNMessage *pVcsNodeSetSwitchWWNMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setErrorStringForIncorrectRbridgeIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::switchStatusValidationStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setSwitchWWNStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };
            
    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetSwitchWWNMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
   
    pVcsNodeLocalMessagingContext->setDomainId (pVcsNodeSetSwitchWWNMessage->getRBridgeId ());
    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getSwitchWWNMessageHandler (VcsNodeGetSwitchWWNMessage *pVcsNodeGetSwitchWWNMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getSwitchWWNStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetSwitchWWNMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getSwitchWWNFromRBridgeIdMessageHandler (VcsNodeGetSwitchWWNFromRBridgeIdMessage *pVcsNodeGetSwitchWWNFromRBridgeIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        // DomainId and RbridgeId are the same
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getSwitchWWNFromRBridgeIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetSwitchWWNFromRBridgeIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    // RBridge-Id and DomainId are the same
    UI32 rBridgeId = pVcsNodeGetSwitchWWNFromRBridgeIdMessage->getRBridgeId ();

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getSwitchWWNFromRBridgeIdMessageHandler : RBridge-Id : ") + rBridgeId);

    pVcsNodeLocalMessagingContext->setDomainId (pVcsNodeGetSwitchWWNFromRBridgeIdMessage->getRBridgeId ());
    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getRBridgeIdFromSwitchWWNMessageHandler (VcsNodeGetRBridgeIdFromSwitchWWNMessage *pVcsNodeGetRBridgeIdFromSwitchWWNMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::queryLocalManagedObjectFromWWN),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getRBridgeIdFromSwitchWWNStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetRBridgeIdFromSwitchWWNMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    WorldWideName switchWWN = pVcsNodeGetRBridgeIdFromSwitchWWNMessage->getSwitchWWN ();

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getRBridgeIdFromSwitchWWNMessageHandler : switchWWN : ") + switchWWN.toString());

    pVcsNodeLocalMessagingContext->setSwitchWWN (pVcsNodeGetRBridgeIdFromSwitchWWNMessage->getSwitchWWN ());
    pVcsNodeLocalMessagingContext->execute ();
}

void VcsFabricLocalObjectManager::getConfigurationTimeMessageHandler (VcsNodeGetConfigurationTimeMessage *pVcsNodeGetConfigurationTimeMessage)
{
    DateTime configurationTime = DcmToolKit::getLatestUpdateTime ();

    tracePrintf (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::getConfigurationTimeMessageHandler: timeStamp %s", (configurationTime.toString ()).c_str ());

    pVcsNodeGetConfigurationTimeMessage->setConfigurationTime (configurationTime);

    pVcsNodeGetConfigurationTimeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsNodeGetConfigurationTimeMessage);
}

bool VcsFabricLocalObjectManager::getNewShowVcs ( )
{
    return (m_newShowVcs);
}

void VcsFabricLocalObjectManager::setNewShowVcsFormatMessageHandler (VcsNodeSetNewShowVcsFormatMessage *pVcsNodeSetNewShowVcsFormatMessage)
{

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::setNewShowVcsFormat),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSetNewShowVcsFormatMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0])); 

    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::setNewShowVcsFormat (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetNewShowVcsFormatMessage *pVcsNodeSetNewShowVcsFormatMessage = reinterpret_cast<VcsNodeSetNewShowVcsFormatMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    
    m_newShowVcs = pVcsNodeSetNewShowVcsFormatMessage->getNewShowVcs(); 
    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::getConfigurationNumberMessageHandler (VcsNodeGetConfigurationNumberMessage *pVcsNodeGetConfigurationNumberMessage)
{
    vector<UI64>    configurationNumberVector;
    ResourceId      status                      = WAVE_MESSAGE_SUCCESS;

    status = DcmToolKit::getConfigurationNumber (configurationNumberVector);

    pVcsNodeGetConfigurationNumberMessage->setconfigurationNumber (configurationNumberVector);

    pVcsNodeGetConfigurationNumberMessage->setCompletionStatus (status);

    reply (pVcsNodeGetConfigurationNumberMessage);
}

void VcsFabricLocalObjectManager::getConfigurationNumberStaticMessageHandler (VcsNodeGetConfigurationNumberMessage *pVcsNodeGetConfigurationNumberMessage)
{
    vector<UI64>    configurationNumberVector;
    ResourceId      status                      = WAVE_MESSAGE_SUCCESS;

    status = DcmToolKit::getConfigurationNumber (configurationNumberVector);

    pVcsNodeGetConfigurationNumberMessage->setconfigurationNumber (configurationNumberVector);

    pVcsNodeGetConfigurationNumberMessage->setCompletionStatus (status);
}

void VcsFabricLocalObjectManager::getSwitchIPFromRBridgeIdMessageHandler (VcsNodeGetSwitchIPFromRBridgeIdMessage *pVcsNodeGetSwitchIPFromRBridgeIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        // DomainId and RbridgeId are the same
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::getSwitchIPFromRBridgeIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext = new VcsNodeLocalMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeGetSwitchIPFromRBridgeIdMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    // RBridge-Id and DomainId are the same
    UI32 rBridgeId = pVcsNodeGetSwitchIPFromRBridgeIdMessage->getRBridgeId ();

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getSwitchIPFromRBridgeIdMessageHandler : RBridge-Id : ") + rBridgeId);

    pVcsNodeLocalMessagingContext->setDomainId (pVcsNodeGetSwitchIPFromRBridgeIdMessage->getRBridgeId ());
    pVcsNodeLocalMessagingContext->execute ();
}

ResourceId VcsFabricLocalObjectManager::setErrorStringForIncorrectRbridgeIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetSwitchWWNMessage *pVcsNodeSetSwitchWWNMessage = reinterpret_cast<VcsNodeSetSwitchWWNMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    UI32                        domainId                    = pVcsNodeSetSwitchWWNMessage->getRBridgeId ();

    pVcsNodeSetSwitchWWNMessage->setMessageString (string ("Switch with rbridge-id ") + domainId + string(" is not part of this cluster."));
    return WAVE_MESSAGE_SUCCESS;
}


ResourceId VcsFabricLocalObjectManager::switchStatusValidationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetSwitchWWNMessage *pVcsNodeSetSwitchWWNMessage = reinterpret_cast<VcsNodeSetSwitchWWNMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    pVcsNodeSetSwitchWWNMessage->setMessageString (string(""));
    // Check if the new switch WWN is already part of the cluster. If so then we cannot use it for replace.
    WorldWideName switchWWN = pVcsNodeSetSwitchWWNMessage->getNewWWN ();
	
    if (NULL != getLocalManagedObjectFromWWN (switchWWN))
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::switchStatusValidationStep : switchWWN : ") + switchWWN.toString() + " is already part of the cluster");
        pVcsNodeSetSwitchWWNMessage->setMessageString (string("Switch WWN : ") + switchWWN.toString() + " is already part of the cluster");
        return WAVE_MESSAGE_ERROR;
    }

    if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER != ((pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject ())->getSpecificStatus()) &&
		 VCS_FABRIC_LOCAL_SPECIFIC_STATUS_NODE_REPLACEMENT_IN_PROGRESS != ((pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject ())->getSpecificStatus())) 
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::switchStatusValidationStep : switch is not disconnected from cluster"));
        pVcsNodeSetSwitchWWNMessage->setMessageString ("Replaced switch is not disconnected from cluster.");
        return WAVE_MESSAGE_ERROR;
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::setSwitchWWNStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetSwitchWWNMessage *pVcsNodeSetSwitchWWNMessage = reinterpret_cast<VcsNodeSetSwitchWWNMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    
    WorldWideName switchWWN = pVcsNodeSetSwitchWWNMessage->getNewWWN ();

    updateWaveManagedObject(pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject ());

    (pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject ())->setSwitchWWN (switchWWN);

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::setSwitchWWNStep: SwitchWWN is set to ") + switchWWN.toString ());

    // Set the node status to indicate that a replacement is in progress. This is the only situation in which the WWN needs to be changed.
    (pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject ())->setSpecificStatus (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_NODE_REPLACEMENT_IN_PROGRESS);

    return (WAVE_MESSAGE_SUCCESS);
}

/**************************************************************************************************
* Query interfaces
**************************************************************************************************/

ResourceId VcsFabricLocalObjectManager::queryLocalManagedObjectFromMappedId (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    ResourceId                                  status   = WAVE_MESSAGE_ERROR;
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;
    UI32                                        mapped_id   = pVcsNodeLocalMessagingContext->getMappedId ();
    WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

    pVcsNodeLocalManagedObject = getLocalManagedObjectFromMappedId (mapped_id);

    if (NULL != pVcsNodeLocalManagedObject)
    {
        pVcsNodeLocalMessagingContext->setPVcsNodeLocalManagedObject (pVcsNodeLocalManagedObject);
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    UI32                                    domain_id   = pVcsNodeLocalMessagingContext->getDomainId ();
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    ResourceId                              status   = WAVE_MESSAGE_ERROR;

    /* Get the object corresponding to this mapped ID */
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (domain_id, "domainId"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId : Cannot find Vcs Node");
            status = WAVE_MESSAGE_ERROR;
        }
        else if (1 == numberOfResults)
        {
            pVcsNodeLocalMessagingContext->setPVcsNodeLocalManagedObject (dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]));
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, " VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId: There can be only one local node.");
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId : System failure");
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::queryThisLocalManagedObject (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    ResourceId                              status   = WAVE_MESSAGE_ERROR;
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;

    /* Get the object corresponding to this mapped ID */
    synchronousQueryContext.addAndAttribute (new AttributeObjectId ((VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId (), "objectId"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::queryThisLocalManagedObject : Cannot find Vcs Node");
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        else if (1 == numberOfResults)
        {
            status = WAVE_MESSAGE_SUCCESS;
                        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
            /* TODO:put assert on null */
            /* Dlete the object on completion */
                        pVcsNodeLocalMessagingContext->setPVcsNodeLocalManagedObject (pVcsNodeLocalManagedObject);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, " VcsFabricLocalObjectManager::queryThisLocalManagedObject: There can be only one local node.");
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
        pResults->clear ();
        delete pResults;
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::queryLocalManagedObjectFromDomainId : System failure");
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::queryLocalManagedObjectFromWWN (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    ResourceId                                  status   = WAVE_MESSAGE_ERROR;
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;
    WorldWideName                               switchWWN   = pVcsNodeLocalMessagingContext->getSwitchWWN ();
    WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

    pVcsNodeLocalManagedObject = getLocalManagedObjectFromWWN (switchWWN);

    if (NULL != pVcsNodeLocalManagedObject)
    {
        pVcsNodeLocalMessagingContext->setPVcsNodeLocalManagedObject (pVcsNodeLocalManagedObject);
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::setVcsIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    
    UI32 vcs_id = pVcsNodeLocalMessagingContext->getNewVcsId ();

    updateThisVcsNodeLocalManagedObject ();
    setThisVcsNodeLocalManagedObjectVcsId (vcs_id);
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setVcsIdStep: VcsId is set to ") + vcs_id);
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setVcsIdVcsConfStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{

    UI32 newVcsId = pVcsNodeLocalMessagingContext->getNewVcsId ();
    UI32 oldVcsId = pVcsNodeLocalMessagingContext->getVcsId ();

    if (newVcsId != oldVcsId)
    {
        raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1001,  oldVcsId, newVcsId);
    }

    return (setVcsConfFile (true, newVcsId));

}

ResourceId VcsFabricLocalObjectManager::setVcsConfModeStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    // This provides a non-standard way of updating the vcs mode parameters
    VcsNodeSetModeChangeParametersMessage *pVcsNodeSetModeChangeParametersMessage = reinterpret_cast<VcsNodeSetModeChangeParametersMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    UI32                                   vcs_id                                 = pVcsNodeSetModeChangeParametersMessage->getVcsId ();
    string                                 vcs_mode                               = pVcsNodeSetModeChangeParametersMessage->getVcsMode ();
    ResourceId                             status                                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setVcsConfModeStep: Node will be transitioned to ") + vcs_mode + string (" with vcsid ") + vcs_id);

    if ( (vcs_mode == "FC") || (vcs_mode == "MC"))
    {
        status = setVcsConfFile (true, vcs_id);
        runVcsModeEnableScript ();
    }
    else if (vcs_mode == "SA")
    {
        UI32 swbd = DcmToolKit::getSwBDModel ();
        if ((swbd == 1000) || (swbd == 1001) || (swbd == 1002) || (swbd == 1003))
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setVcsConfModeStep: Vcs cannot be disabled on this platform.");
            return WAVE_MESSAGE_ERROR;
        }

        status = setVcsConfFile (false, vcs_id);
        runVcsModeDisableScript ();
    }

    return status;
}

ResourceId VcsFabricLocalObjectManager::setRBridgeIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    UI32 rbridge_id = pVcsNodeLocalMessagingContext->getNewRbridgeId (); 

    if (getThisVcsNodeLocalManagedObjectDomainId() != rbridge_id)
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::setRBridgeIdStep: rbridgeId changed to ") + rbridge_id); 
        VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();
        pVcsNodeFabosInterfaceObjectManager->setDomainId (rbridge_id);

        updateThisVcsNodeLocalManagedObject ();
        setThisVcsNodeLocalManagedObjectDomainId (rbridge_id);
    }
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setRbridgeIdModeStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetModeChangeParametersMessage *pVcsNodeSetModeChangeParametersMessage = reinterpret_cast<VcsNodeSetModeChangeParametersMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    UI32                                   rbridge_id                             = pVcsNodeSetModeChangeParametersMessage->getRbridgeId ();
    string                                 vcs_mode                               = pVcsNodeSetModeChangeParametersMessage->getVcsMode ();

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::setRBridgeIdModeStep: rbridgeId changed to ") + rbridge_id); 

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if ( 0 == rbridge_id)
    {
        // rbridge-id to be defaulted to 1 in SA, no rbridge-id given by user
        if ( "SA" == vcs_mode)
        {
            rbridge_id = 1;
            status = setRBridgeIdBackend (rbridge_id);
        }
    }
    else
    {
        // set rbridge-id to user provided value
        status = setRBridgeIdBackend (rbridge_id);
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::setRBridgeIdBackendStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    if (false == pVcsNodeLocalMessagingContext->getRbridgeIdResetFlag ())
    {
        //Skip this step for MGMT type
        trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::setRBridgeIdBackendStep: skipping reseting rbridge id in backend");
        return (WAVE_MESSAGE_SUCCESS);
    }

	UI32 rbridge_id = pVcsNodeLocalMessagingContext->getNewRbridgeId ();

    if ( rbridge_id == 0 )
    {
        trace (TRACE_LEVEL_ERROR,"VcsFabricLocalObjectManager::setRBridgeIdBackendStep: New RbridgeId cannot be 0. Hence returning without setting backend RbridgeId ");
        return (WAVE_MESSAGE_SUCCESS);
        
    }

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	status = setRBridgeIdBackend (rbridge_id);

	return (status);
}

ResourceId VcsFabricLocalObjectManager::setRBridgeIdBackend (const UI32 & rbridgeId)
{
    // First do a switchDisable
    switchDisableNoFicon();

    // Next set the DomainId 
    UI32 rc = fabSetLocalDomainId (rbridgeId);

    if (rc != rbridgeId)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::setRBridgeIdBackend: Error in setting domainId in the backend, return value of fabSetLocalDomainId ") + (int)rc + string(" != requested value of rbridge_id ") + (int)rbridgeId);

        raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1010, rbridgeId);

        return WAVE_MESSAGE_ERROR;
    }
    else
    {
        raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1009, rbridgeId);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setIsPrincipalStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetIsPrincipalMessage *pVcsNodeSetIsPrincipalMessage = reinterpret_cast<VcsNodeSetIsPrincipalMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    bool                                    isPrincipal      = pVcsNodeSetIsPrincipalMessage->getIsPrincipal ();
    updateThisVcsNodeLocalManagedObject ();
    setThisVcsNodeLocalManagedObjectIsPrincipal(isPrincipal);
        if (true == isPrincipal)
        {
        setThisVcsNodeLocalManagedObjectInVcs (true);
        setThisVcsNodeLocalManagedObjectStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD , VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY);
        }
        return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setDomainIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetDomainIdMessage *pVcsNodeSetDomainIdMessage = reinterpret_cast<VcsNodeSetDomainIdMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    updateThisVcsNodeLocalManagedObject ();
    UI32 domainId = pVcsNodeSetDomainIdMessage->getDomainId ();

    setThisVcsNodeLocalManagedObjectDomainId (domainId);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setIsReadyStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetIsReadyMessage *pVcsNodeSetIsReadyMessage = reinterpret_cast<VcsNodeSetIsReadyMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    m_isReady = pVcsNodeSetIsReadyMessage->getIsReady ();

        return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getVcsIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetVcsIdMessage *pVcsNodeGetVcsIdMessage = reinterpret_cast<VcsNodeGetVcsIdMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    UI32 vcsId = getThisVcsNodeLocalManagedObjectVcsId ();
    pVcsNodeGetVcsIdMessage->setVcsId (vcsId);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getIsPrincipalStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetIsPrincipalMessage *pVcsNodeGetIsPrincipalMessage = reinterpret_cast<VcsNodeGetIsPrincipalMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
       
    bool isPrincipal = getThisVcsNodeLocalManagedObjectIsPrincipal ();
    pVcsNodeGetIsPrincipalMessage->setIsPrincipal(isPrincipal);
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getIsReadyStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetIsReadyMessage *pVcsNodeGetIsReadyMessage = reinterpret_cast<VcsNodeGetIsReadyMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    pVcsNodeGetIsReadyMessage->setIsReady(m_isReady);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getSwitchMacStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetSwitchMacMessage *pVcsNodeGetSwitchMacMessage = reinterpret_cast<VcsNodeGetSwitchMacMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    VcsNodeLocalManagedObject               *pvcsNodeLocalManagedObject = pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject();

    pVcsNodeGetSwitchMacMessage->setSwitchMac(pvcsNodeLocalManagedObject->getSwitchMac());

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getLocalMappedIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetLocalMappedIdMessage *pVcsNodeGetLocalMappedIdMessage = reinterpret_cast<VcsNodeGetLocalMappedIdMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    UI32    mappedId = getThisVcsNodeLocalManagedObjectMappedId ();

    if ( 0 == mappedId )
    {
        return (WAVE_MESSAGE_ERROR);
    }

    pVcsNodeGetLocalMappedIdMessage->setMappedId(mappedId);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getSwitchWWNStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetSwitchWWNMessage *pVcsNodeGetSwitchWWNMessage = reinterpret_cast<VcsNodeGetSwitchWWNMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    VcsNodeLocalManagedObject               *pvcsNodeLocalManagedObject = pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject();

    pVcsNodeGetSwitchWWNMessage->setSwitchWWN(pvcsNodeLocalManagedObject->getSwitchWWN());

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getSwitchWWNFromRBridgeIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetSwitchWWNFromRBridgeIdMessage *pVcsNodeGetSwitchWWNFromRBridgeIdMessage   = dynamic_cast<VcsNodeGetSwitchWWNFromRBridgeIdMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    VcsNodeLocalManagedObject               *pVcsNodeLocalManagedObject                 = pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject();
    
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getSwitchWWNFromRBridgeIdStep : SwitchWWN from VcsNodeLocalMO : " + pVcsNodeLocalManagedObject->getSwitchWWN ().toString ());
    pVcsNodeGetSwitchWWNFromRBridgeIdMessage->setSwitchWWN (pVcsNodeLocalManagedObject->getSwitchWWN ());

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getRBridgeIdFromSwitchWWNStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetRBridgeIdFromSwitchWWNMessage *pVcsNodeGetRBridgeIdFromSwitchWWNMessage   = dynamic_cast<VcsNodeGetRBridgeIdFromSwitchWWNMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    VcsNodeLocalManagedObject               *pVcsNodeLocalManagedObject                 = pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject();

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getRBridgeIdFromSwitchWWNStep : RBridge-Id from VcsNodeLocalMO : ") + pVcsNodeLocalManagedObject->getDomainId ());
    pVcsNodeGetRBridgeIdFromSwitchWWNMessage->setRBridgeId (pVcsNodeLocalManagedObject->getDomainId ());

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getSwitchIPFromRBridgeIdStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetSwitchIPFromRBridgeIdMessage *pVcsNodeGetSwitchIPFromRBridgeIdMessage   = dynamic_cast<VcsNodeGetSwitchIPFromRBridgeIdMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    VcsNodeLocalManagedObject               *pVcsNodeLocalManagedObject                 = pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject();
    
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getSwitchIPFromRBridgeIdStep : DomainID : " + pVcsNodeLocalManagedObject->getDomainId ());
    pVcsNodeGetSwitchIPFromRBridgeIdMessage->setSwitchIP (DcmToolKit::getIpAddressFromDomainId (pVcsNodeLocalManagedObject->getDomainId ()).c_str ());

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setFcfMacStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetFcfMacMessage *pVcsNodeSetFcfMacMessage = reinterpret_cast<VcsNodeSetFcfMacMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    updateThisVcsNodeLocalManagedObject ();
    MacAddress fcfMac = pVcsNodeSetFcfMacMessage->getFcfMac();
    setThisVcsNodeLocalManagedObjectfcfMac(fcfMac);
    trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setFcfMacStep: fcf mac : " + fcfMac.toString());
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode (bool vcs_mode)
{
    ResourceId                                 status;
    string                                     defaultFile;

    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode: copying from default to start schema");

    if (true == vcs_mode)
    {
        defaultFile = "defaultconfig.vcs";
    }
    else
    {
        defaultFile = "defaultconfig.novcs";
    }

    // Copy default config to start config for the local node only

    PersistenceLocalObjectManagerSetStartupFileMessage *pPersistenceLocalObjectManagerSetStartupFileMessage = new PersistenceLocalObjectManagerSetStartupFileMessage (WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE, defaultFile);
    status = sendSynchronously (pPersistenceLocalObjectManagerSetStartupFileMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode: send setStartupFileMessage to Persistence service failed");
        return WAVE_MESSAGE_ERROR;
    }
    else
    {
        ResourceId completionStatus = pPersistenceLocalObjectManagerSetStartupFileMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode: setStartupFileMessage completed with error");
            return WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode: setStartupFileMessage succeeded");
            status = WAVE_MESSAGE_SUCCESS;
        }
    }

    // save prism configuration and set is startup valid to false
    PersistenceLocalObjectManagerSavePrismConfigurationMessage  *pSavePrismConfigurationMessage = new PersistenceLocalObjectManagerSavePrismConfigurationMessage (false);
    status = sendSynchronously (pSavePrismConfigurationMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode: send savePrismConfigurationMessage to Persistence service failed");
        return WAVE_MESSAGE_ERROR;
    }
    else
    {
        ResourceId completionStatus = pSavePrismConfigurationMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode: savePrismConfigurationMessage completed with error");
            return WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::copyDefaultToStartAtLocalNode: savePrismConfigurationMessage succeeded");
            status = WAVE_MESSAGE_SUCCESS;
        }
    }
 

    delete pPersistenceLocalObjectManagerSetStartupFileMessage;
    delete pSavePrismConfigurationMessage;
    return status;
}

ResourceId VcsFabricLocalObjectManager::haSyncUpdateAtLocalNodeStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::haSyncUpdateAtLocalNodeStep: sync update ha");

    ResourceId status = haSyncUpdateAtLocalNode ();

    return (status);
}

ResourceId VcsFabricLocalObjectManager::haSyncUpdateAtLocalNode ()
{
    //******** TODO: remove the following after HA sync moved to HA transport
    if (VcsNodeFabosInterfaceObjectManager::getIsSyncEnabled())
    {
        sendHaSyncUpdateMessage ();
    }
    sleep(3);

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::setVcsConfFile (bool vcs_mode, int vcs_id)
{

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setVcsConfFile [vcsid:") + vcs_id + string ("]: Entering ...... "));
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setVcsConfFile: VCS_MODE ")  + vcs_mode);

    FILE * pFile;
    string vcsConfPath = DcmToolKit::getVcsConfPath ();
    vcsConfPath.append ("/vcs.conf");

    pFile = fopen (vcsConfPath.c_str(),"w");
    if (pFile == NULL)
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setVcsConfFile: Cannot open the vcs.conf for writing");
        return (WAVE_MESSAGE_ERROR);
    }

    if (vcs_mode == true)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsConfFile: VCS MODE IS TRUE");
        fprintf (pFile, "%04d", vcs_id);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsConfFile: VCS MODE IS FALSE");
        fputs ("0000",pFile);
    }

    fclose (pFile);
    return (WAVE_MESSAGE_SUCCESS);

}

ResourceId VcsFabricLocalObjectManager::setVcsConfStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage = reinterpret_cast<VcsNodeSetVcsModeMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    bool                      vcs_mode                  = pVcsNodeSetVcsModeMessage->getVcsMode ();
    int                       vcs_id                    = 0;

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsConfStep: Entering ...... ");
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsConfStep: VCS_MODE " + vcs_mode);

    vcs_id = getThisVcsNodeLocalManagedObjectVcsId ();
    if (setVcsConfFile (vcs_mode, vcs_id) != WAVE_MESSAGE_SUCCESS)
    {
        /* 
         * return error if vcs_mode is set to true.
         * If setting vcs mode to false, the operation cannot fail and hence continue
         */ 
        if (vcs_mode)
        {
            reverseVcsMode (vcs_mode);
            pVcsNodeSetVcsModeMessage->setMessageString ("Error in accessing vcs configuration file\n");
            return (WAVE_MESSAGE_ERROR);
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::copyDefaultToStartInVcsModeEnableStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage = reinterpret_cast<VcsNodeSetVcsModeMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    bool vcs_mode = pVcsNodeSetVcsModeMessage->getVcsMode ();
    bool reverseVcsModeOnFailure = true;
    ResourceId status;
    string messageString = "";

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::copyDefaultToStartInVcsModeEnableStep");

    status = copyDefaultToStartInModeChange (vcs_mode, reverseVcsModeOnFailure, messageString);

    pVcsNodeSetVcsModeMessage->setMessageString (messageString);

    return (status);
}

ResourceId VcsFabricLocalObjectManager::copyDefaultToStartInModeTransitionStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    bool        vcsMode                 = true;
    bool        reverseVcsModeOnFailure = false;
    string      messageString           = "";
    ResourceId  status                  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::copyDefaultToStartInModeTransitionStep : Entering...");
    
    status = copyDefaultToStartInModeChange (vcsMode,reverseVcsModeOnFailure, messageString);

    return (status);
}

ResourceId VcsFabricLocalObjectManager::setupConfigAndRebootSecondariesStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setupConfigAndRebootSecondariesStep: Entering.");
    prismAssert (NULL != pVcsNodeConfigDistributeContext, __FILE__, __LINE__);

    LocationRole         locationRole           =  pVcsNodeConfigDistributeContext->getLocationRole ();
    bool                 preserveConfig         =  pVcsNodeConfigDistributeContext->getPreserveConfig ();
    bool                 isStandbyRebootOnly    =  false;
    ResourceId           status                 =  WAVE_MESSAGE_SUCCESS;

    // This check is to enforce the operation to be done only on non-primary locations
    // Operation on primary will be done as a last step from the global worker
    // we need to ensure the primary reboot happens after the controlled failover
    // is complete and cluster is destroyed. Although setupConfigAndRebootStep is
    // identical for both secondaries and primary, we want to postpone the reboot of primary
    // until it has completed all operations required for controlled failover.
    if ((LOCATION_SECONDARY == locationRole) || (LOCATION_STAND_ALONE == locationRole))
    {
        status = setStartupConfig (preserveConfig);

        if (WAVE_MESSAGE_SUCCESS == status) 
        {
            if (preserveConfig)
            {
                // If config preserve case, remove ccmd cfg file else the file is removed while doing copyDefaultToStart in previous step
                if (WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfg ())
                {
                    trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setupConfigAndRebootSecondariesStep: Failed to remove Ccmd cfg file");
                }
            }

            haSyncUpdateAtLocalNode ();

            if (WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfgOnStandby ())
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setupConfigAndRebootSecondariesStep: Failed to remove Ccmd cfg file on standby");
            }

            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setupConfigAndRebootSecondariesStep: Rebooting node with role:" + FrameworkToolKit::localize (locationRole)));
            status = rebootLocalNode (isStandbyRebootOnly);
        }
    }

    return status;
}

ResourceId VcsFabricLocalObjectManager::setupConfigAndRebootPrimaryStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setupConfigAndRebootPrimaryStep: Entering.");
    prismAssert (NULL != pVcsNodeConfigDistributeContext, __FILE__, __LINE__);

    VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage = reinterpret_cast<VcsNodeConfigDistributeMessage*> (pVcsNodeConfigDistributeContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeConfigDistributeMessage, __FILE__, __LINE__);

    bool                 isStandbyRebootOnly    =  false;
    bool                 preserveConfig         =  pVcsNodeConfigDistributeContext->getPreserveConfig ();
    LocationRole         locationRole           =  pVcsNodeConfigDistributeMessage->getLocationRole ();
    ResourceId           status                 =  WAVE_MESSAGE_SUCCESS;

    if (LOCATION_PRIMARY == locationRole)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setupConfigAndRebootPrimaryStep: Rebooting " + FrameworkToolKit::localize (locationRole)));
        status = setStartupConfig (preserveConfig);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            haSyncUpdateAtLocalNode ();
            
            if (false == preserveConfig)
            {
                removeCcmdCfgOnStandby ();
            }

            status = rebootLocalNode (isStandbyRebootOnly);
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setupConfigAndRebootPrimaryStep: " + FrameworkToolKit::localize (locationRole)));
    }

    return status;
}

ResourceId VcsFabricLocalObjectManager::setStartupConfig (bool preserveConfig)
{
    bool             vcsMode                         =  true;
    bool             reverseVcsModeOnFailure         =  false;
    string           messageString                   =  "";
    ResourceId       status                          =  WAVE_MESSAGE_SUCCESS;

    if (false == preserveConfig)
    {
        // If config preserve is not desired, copy the default to startup config.
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setStartupConfig: Performing copyDefaultToStart");
        status = copyDefaultToStartInModeChange (vcsMode, reverseVcsModeOnFailure, messageString);
    }
    else
    {
        // Copy the running-config to startup-config
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setStartupConfig: Performing copyRunningToStart");
        status = copyRunningToStart ();
    }
        
    return status;
}

ResourceId VcsFabricLocalObjectManager::setStartupConfigModeStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetModeChangeParametersMessage *pVcsNodeSetModeChangeParametersMessage = reinterpret_cast<VcsNodeSetModeChangeParametersMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    string                                 vcs_mode                               = pVcsNodeSetModeChangeParametersMessage->getVcsMode ();
    string                                 configFile                             = "";

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setStartupConfigModeStep: startup config file is set in ") + vcs_mode);

    if ( (vcs_mode == "FC") || (vcs_mode == "MC"))
    {
        configFile = "defaultconfig.vcs";
    }
    else if (vcs_mode == "SA")
    {
        configFile = "defaultconfig.novcs";
    }

    string   defaultConfigFile   = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + configFile;
    string   startupConfigFile   = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/startup-config";
    ifstream defaultCfgFileStream (defaultConfigFile.c_str());

    if ( true ==  defaultCfgFileStream.is_open() )
    {
        defaultCfgFileStream.close();
        vector<string>  output;

        string cmd = string("cp -f " + defaultConfigFile + " " + startupConfigFile);
        FrameworkToolKit::systemCommandOutput (cmd, output);
    }

    FrameworkToolKit::setStartupFileName (defaultConfigFile);
    FrameworkToolKit::setStartupFileType (WAVE_PERSISTENCE_REGULAR_FILE);
    FrameworkToolKit::setIsStartupValid  (true);

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::deleteConfigFilesStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::deleteConfigFilesStep: Cleanup Ccmd and Dcmd config files.");

    if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfg ())
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::deleteConfigFilesStep: Failed to remove Ccmd cfg.");
        return WAVE_MESSAGE_ERROR;
    }

    if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeDcmdCfg ())
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::deleteConfigFilesStep: Failed to remove Dcmd cfg.");
        return WAVE_MESSAGE_ERROR;
    }

    if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfgOnStandby ())
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::deleteConfigFilesStep: Failed to remove Ccmd cfg on standby.");
        return WAVE_MESSAGE_ERROR;
    }

    if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeDcmdCfgOnStandby ())
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::deleteConfigFilesStep: Failed to remove Dcmd cfg on standby.");
        return WAVE_MESSAGE_ERROR;
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::copyRunningToStart ()
{ 
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::copyRunningToStart: Entering.");

    string sourceSchema = OrmRepository::getWaveCurrentSchema ();
    string destSchema   = OrmRepository::getWaveStartSchema ();

    PersistenceLocalObjectManagerCopySchemaMessage *pPersistenceLocalObjectManagerCopySchemaMessage = new PersistenceLocalObjectManagerCopySchemaMessage (sourceSchema, destSchema);

    ResourceId status = sendSynchronously (pPersistenceLocalObjectManagerCopySchemaMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::copyRunningToStart: Send to Persistence service failed.");
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        ResourceId completionStatus = pPersistenceLocalObjectManagerCopySchemaMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::copyRunningToStart: Message to Persistence service completed with error");
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::copyRunningToStart: Copy running config to start operation succeeded");
            status = WAVE_MESSAGE_SUCCESS;
        }
    }

    delete pPersistenceLocalObjectManagerCopySchemaMessage;

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        string startupConfigFile  = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + "defaultconfig.vcs";

        FrameworkToolKit::setStartupFileName (startupConfigFile);
        FrameworkToolKit::setStartupFileType (WAVE_PERSISTENCE_REGULAR_FILE);
        FrameworkToolKit::setIsStartupValid  (true);

        status = FrameworkToolKit::savePrismConfiguration (false);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::copyRunningToStart: savePrismConfiguration status: ") + FrameworkToolKit::localize (status));
        }
    }

    return status;
}

ResourceId VcsFabricLocalObjectManager::rebootLocalNodeStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    bool                            isStandbyRebootOnly             = false;
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::rebootLocalNodeStep: Rebooting local node.");

    status = rebootLocalNode (isStandbyRebootOnly);

    return (status);
}

ResourceId VcsFabricLocalObjectManager::rebootLocalNode (const bool &isStandbyRebootOnly)
{
    vector<string>  output;
    SI32            rstatus = 0;
    ResourceId      status  = WAVE_MESSAGE_SUCCESS;

    if (VcsNodeFabosInterfaceObjectManager::getIsActiveNode())
    {
        rstatus = FrameworkToolKit::systemCommandOutput ("/fabos/sbin/chassisReboot", output);
    }
    else if (isStandbyRebootOnly)
    {
        rstatus = FrameworkToolKit::systemCommandOutput ("/fabos/sbin/reboot -s -r StandbySyncVcsMode", output);
    }

    trace (TRACE_LEVEL_DEVEL, string("VcsFabricLocalObjectManager::reboot : Reboot Status : ") + rstatus);

    if (0 != rstatus)
    {
        trace (TRACE_LEVEL_WARN, string("VcsFabricLocalObjectManager::reboot : Reboot Status : ") + rstatus);

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::copyDefaultToStartInModeChange (const bool & vcsMode, const bool & reverseVcsModeOnFailure, string & retMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::copyDefaultToStartInModeChange");

    FrameworkToolKit::savePrismConfiguration ();

    ResourceId status = copyDefaultToStartAtLocalNode (vcsMode);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        if (true == reverseVcsModeOnFailure)
        {
            if (vcsMode)
            {
                reverseVcsMode (vcsMode);
                retMessage = "Error in copying configuration from default to startup\n";

                /* Run the VCS Disable script as the Enable operation has failed. */
                runVcsModeDisableScript();
                return (status);
            }
            else
            {
                /* Run the VCS Enable script as the Disable operation has failed. */
                runVcsModeEnableScript();
            }
        }
        else
        {
            return (status);
        }
    }

    // add removing startup-config for VCS mode change
    string startupConfigFile = string (ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/startup-config");
    unlink (startupConfigFile.c_str());

    // Delete Cfg file in all mode changes. This change is done to avoid triggering DB conversion in case of SA->FC/FC->SA/FC->MC/MC->FC.
    if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfg())
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::copyDefaultToStartInModeChange: Failed to remove Ccmd cfg file");
        }

    return status;
}

ResourceId VcsFabricLocalObjectManager::copyDefaultToStartInVcsModeDisableStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    bool    clusterExists   = false;

    if (DcmToolKit::fcPrincipalEnabled ())
    {
        m_fcPrincipalTrackingWorker->getVcsClusterId (clusterExists);
    }

    /* When this step is executed even on secondary there wont be any cluster
         To proceed till last step we need to skip reboot*/

    if ( (false == clusterExists) && ( pVcsNodeLocalMessagingContext->getNoVcsEnableSecondaryFlag() == false ))
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::copyDefaultToStartInVcsModeDisableStep : No cluster exists.");
        VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage = reinterpret_cast<VcsNodeSetVcsModeMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
        bool vcsMode = pVcsNodeSetVcsModeMessage->getVcsMode ();
        string retMessage = "";

        ResourceId status = copyDefaultToStartInModeChange (vcsMode, true, retMessage);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pVcsNodeSetVcsModeMessage->setMessageString (retMessage);
            return (status);
        }

        status = haSyncUpdateAtLocalNode ();
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            retMessage += " HA sync failed ";
            pVcsNodeSetVcsModeMessage->setMessageString (retMessage);
            return (status);
        }

        status = removeCcmdCfgOnStandby ();
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            retMessage += " Remove ccmd cfg on standby failed ";
            pVcsNodeSetVcsModeMessage->setMessageString (retMessage);
            return (status);    
        }

        status = rebootLocalNode (false);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            retMessage += "reboot local node failed";
            pVcsNodeSetVcsModeMessage->setMessageString (retMessage);
            return (status);
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::sendRemoveSwitchMessage (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    ResourceId completionStatus = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::sendRemoveSwitchMessage: Entering...");
    vector<UI32> mappedIds;

    mappedIds.push_back (getThisVcsNodeLocalManagedObjectMappedId ());
    VcsRemoveSwitchMessage *pMessage = new VcsRemoveSwitchMessage (mappedIds);
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::sendRemoveSwitchMessage : Sending remove switch message to J/M Plugin");
    status = sendSynchronously (pMessage);

    if ( WAVE_MESSAGE_SUCCESS != status )
    {
        delete pMessage;
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::sendRemoveSwitchMessage : sendSynchronous failed ");
    }
    else
    {
        completionStatus = pMessage->getCompletionStatus ();
        delete pMessage;
        
        if ( WAVE_MESSAGE_SUCCESS == completionStatus )
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::sendRemoveSwitchMessage : remove/delete switch message returned with success");
            return (WAVE_MESSAGE_SUCCESS);
        }
        else if ( WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER == completionStatus )
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::sendRemoveSwitchMessage :  postToRemoteLocation for remove switch message failed");
        }
        else if ( WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE_DUE_TO_PRINCIPAL_FAILOVER == completionStatus  )
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::sendRemoveSwitchMessage : primary disconnected before replying to remove switch message. So treating it as success");
            return (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::sendRemoveSwitchMessage : remove/delete switch message returned with error "+FrameworkToolKit::localize (completionStatus));
        }
    }

    /* always return success we cant retract from this step */
    return (WAVE_MESSAGE_ERROR);
}

ResourceId VcsFabricLocalObjectManager::vcsDisableValidationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage = reinterpret_cast<VcsNodeSetVcsModeMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    if (m_isHaSyncInProgress)
    {
        pVcsNodeSetVcsModeMessage->setMessageString ("HA sync in progress.  Cannot perform the vcs mode change at this time.");

        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsDisableValidationStep: HA sync in progress.  Cannot perform the vcs mode change at this time.");

        return (WAVE_MESSAGE_ERROR);
    }

    if (!getThisVcsNodeLocalManagedObjectVcsMode())
    {
        // trying to set vcs mode to false when its already false
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false when it's already false");

        pVcsNodeSetVcsModeMessage->setMessageString ("VCS mode is already disabled.\n");

        return (WAVE_MESSAGE_ERROR);
    }
    
    ResourceId locationRole = FrameworkToolKit::getThisLocationRole ();

    if (LOCATION_SECONDARY_UNCONFIRMED == locationRole || LOCATION_PRIMARY_UNCONFIRMED == locationRole )
    {   
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsDisableValidationStep: Command not allowed when node is in role/state : " + FrameworkToolKit::localize (locationRole));

        pVcsNodeSetVcsModeMessage->setMessageString ("Command not allowed when node is in \"Secondary Waiting To Rejoin\" state - please try after sometime\n");

        return (WAVE_MESSAGE_ERROR);
    }

    ResourceId specificStatus =  getThisVcsNodeLocalManagedObjectSpecificStatus();
    m_noVcsEnablePrincipalFlag = false ;

    if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY == specificStatus )
    {
        m_noVcsEnablePrincipalFlag = true;

        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on primary");
    }
    else if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY == specificStatus )
    {
    	pVcsNodeLocalMessagingContext->setNoVcsEnableSecondaryFlag(true);

        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on secondary");
    }
    else if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_STAND_ALONE == specificStatus )
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on standalone");
    }
    else if (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_CHASSIS_DISABLED_REPLACEMENT_MODEL_MISMATCH == specificStatus)
	{
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on mismatched replacement node");
	}
	else if (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_NODE_REPLACEMENT_IN_PROGRESS == specificStatus)
	{
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on replacement node");
	}
    else 
    {
        tracePrintf (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on a node in : %s state is not allowed", FrameworkToolKit::localize(specificStatus).c_str());

        pVcsNodeSetVcsModeMessage->setMessageString ("Command not allowed when node is inconsistent - please try after sometime\n");

        return (WAVE_MESSAGE_ERROR);
    }


    if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY == specificStatus  )
    {

        VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance();

        prismAssert ((NULL != pVcsFabObjectManager), __FILE__, __LINE__);

        if ( pVcsFabObjectManager->getBecomePrincipalFlag () )
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on a secondary becoming primary is not allowed");

            pVcsNodeSetVcsModeMessage->setMessageString ("Principal failover in progress. Please try later\n");

            return (WAVE_MESSAGE_ERROR);
        }

        if ( ( !FrameworkToolKit::isAConnectedLocation(FrameworkToolKit::getClusterPrimaryLocationId() ) ))
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsDisableValidationStep: setting vcsMode to false on a secondary when primary is down, is not allowed");      

            pVcsNodeSetVcsModeMessage->setMessageString ("Principal failover in progress. Please try later\n");

            return (WAVE_MESSAGE_ERROR);
        }
    }

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == getThisVcsNodeClusterType ())
    {
        // rbridgeid Reset for MGMT
    	pVcsNodeLocalMessagingContext->setRbridgeIdResetFlag (true);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::unconfigureSecondaryStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{

    VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage = reinterpret_cast<VcsNodeSetVcsModeMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY !=  getThisVcsNodeLocalManagedObjectSpecificStatus() )
    {
        // If this node is not secondary then skip this step
        return (WAVE_MESSAGE_SUCCESS);
    }

    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::unconfigureSecondaryStep : Entering ...");

    ResourceId status = WAVE_MESSAGE_ERROR;

    status = sendRemoveSwitchMessage (pVcsNodeLocalMessagingContext);
    
    if ( WAVE_MESSAGE_ERROR == status )
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::unconfigureSecondaryStep : Sending VcsRemoveSwitchMessage to  JMP failed. No vcs enable cannot be processed");
        pVcsNodeSetVcsModeMessage->setMessageString ("Principal failover in progress. Please try later\n");
        return (WAVE_MESSAGE_ERROR);
    }

    string ipAddress = "";
    SI32 port = 0;    

    ipAddress = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
    port = FrameworkToolKit::getPortForLocationId (FrameworkToolKit::getThisLocationId ());
    FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);
    ResourceId messageStatus = sendSynchronously (pMessage, FrameworkToolKit::getThisLocationId ());

    if (WAVE_MESSAGE_SUCCESS != messageStatus)
    {   
        // Always return success
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::unconfigureSecondaryStep : Error in sending UnconfigureSecondary message : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
    }
    else
    {
        ResourceId completionStatus = pMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {   
            // Always return success
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::unconfigureSecondaryStep : UnconfigureSecondary message Completion Status : \"" + FrameworkToolKit::localize (completionStatus) + "\"");
        }
    }

    delete pMessage;
    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::prismLinearSequencerFailedStepForNoVcsEnable (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{

    VcsFabricLocalObjectManager::prismSynchronousLinearSequencerFailedStep( pPrismSynchronousLinearSequencerContext );

    m_noVcsEnablePrincipalFlag = false;
    m_noVcsEnableInProgressFlag = false;
    
}

ResourceId VcsFabricLocalObjectManager::unconfigurePrimaryStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    
    if (m_noVcsEnablePrincipalFlag == false)
    {
        //Execute this step on only primary
        return (WAVE_MESSAGE_SUCCESS);
    }
    
    ResourceId messageStatus = WAVE_MESSAGE_ERROR;
    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::unconfigurePrimaryStep : Entering unconfigurePrimaryStep...");

    VcsDeleteClusterMessage *pMessage = new VcsDeleteClusterMessage ();
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::unconfigurePrimaryStep : Sending delete cluster message to J/M Plugin");
    messageStatus = sendSynchronously (pMessage);

    if (WAVE_MESSAGE_SUCCESS != messageStatus)
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::unconfigurePrimaryStep : Error in sending DeleteCluster message : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
    }
    else
    {
        completionStatus = pMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::unconfigurePrimaryStep : Error in DeleteCluster message Completion Status : \"" + FrameworkToolKit::localize (completionStatus) + "\"");

        }
    }
    delete pMessage;
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::prismLinearSequencerSucceededStepForNoVcsEnable (PrismSynchronousLinearSequencerContext *pPrismLinearSequencerContext)
{
    PrismMessage *pPrismMessage = pPrismLinearSequencerContext->getPPrismMessage ();
    ResourceId completionStatus = pPrismLinearSequencerContext->getCompletionStatus ();
    bool x86 = false;

    
    if (DcmToolKit::fcPrincipalEnabled ())
    {
        x86 = m_fcPrincipalTrackingWorker->isEnvx86 ();
    }
    
    if (NULL != pPrismMessage)
    {
        if (x86)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::prismLinearSequencerSucceededStepForNoVcsEnable: returning...");
            pPrismMessage->setCompletionStatus (completionStatus);
            reply (pPrismMessage);
        }
    }

    delete pPrismLinearSequencerContext;
    return (completionStatus);
}

ResourceId VcsFabricLocalObjectManager::vcsEnableValidationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeEnableValidationMessage *pVcsNodeEnableValidationMessage = reinterpret_cast<VcsNodeEnableValidationMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsEnableValidationStep: Entering...");

    if (getThisVcsNodeLocalManagedObjectVcsMode())
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsEnableValidationStep: setting vcsMode to true when it's already true");
        pVcsNodeEnableValidationMessage->setMessageString ("VCS mode is already enabled.\n");
        return WAVE_MESSAGE_ERROR;
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::prepareForVcsModeDisableStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    m_noVcsEnableInProgressFlag = true;
    pVcsNodeLocalMessagingContext->setPrincipalControlFailoverFlag(false);
    pVcsNodeLocalMessagingContext->setNewRbridgeId (1);

    if (DcmToolKit::fcPrincipalEnabled ())
    {
        m_fcPrincipalTrackingWorker->disableTimer ();
    }

    if ( m_noVcsEnablePrincipalFlag )
    { 
        VcsClusterManagedObject *pVcsClusterManagedObject = VcsFabObjectManager::getInstance ()->getVcsClusterManagedObject ();

        /* TODO: verify if pVcsClusterManagedObject can ever be NULL in failure case */   
        prismAssert (NULL != pVcsClusterManagedObject, __FILE__, __LINE__);

        UI32 nodesInCluster = pVcsClusterManagedObject->getSwitchCount ();
        delete pVcsClusterManagedObject;

        if (1 < nodesInCluster)
        {   
            trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::prepareForVcsModeDisableStep: no vcs enable for Principal when cluster exists - Control Failover"));
            pVcsNodeLocalMessagingContext->setPrincipalControlFailoverFlag(true);
        }
    }
    else
    {   
        trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::prepareForVcsModeDisableStep: reset vcsMode request for secondary/standalone node");
    }

    return WAVE_MESSAGE_SUCCESS;
}

bool VcsFabricLocalObjectManager::getNoVcsEnableInProgressFlag ()
{
    return m_noVcsEnableInProgressFlag;
}

ResourceId VcsFabricLocalObjectManager::reverseVcsMode (const bool &vcsMode)
{
        startTransaction ();
        updateThisVcsNodeLocalManagedObject ();
        setThisVcsNodeLocalManagedObjectVcsMode(! vcsMode);
        ResourceId status = commitTransaction ();
         
        int vcsId = 0;
        if (true == vcsMode)
        {
        	vcsId = getThisVcsNodeLocalManagedObjectVcsId ();
        }
        setVcsConfFile(vcsMode, vcsId);

        string vcsModeString = vcsMode ? "false":"true"; // opposite of vcs_mode
        if (FRAMEWORK_SUCCESS == status)
        {
                trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::unDoVcsModeProcessing: VcsMode reset to "+vcsModeString);
        }
        else
        {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::unDoVcsModeProcessing: Unable to reset VcsMode back to "+vcsModeString);
        }
        return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::vcsDisableStartTransactionStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    startTransaction();
        return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::vcsDisableCommitTransactionStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    if (commitTransaction() != FRAMEWORK_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsDisableCommitTransactionStep: Commit transaction failed, But we dont rollback the cache since this can never occur.");
    }
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::setConfFilesStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    bool vcs_mode = false;
    int  vcs_id   = 0;

    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::setConfFileStep: Entering ...... ");

    if (setVcsConfFile (vcs_mode, vcs_id) != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setConfFileStep: setVcsConfFile returned error but still we proceed ...... ");
    }
    
    if ( setClusterConfFile( false) != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setConfFileStep: setclusterConfFile returned error but still we proceed ...... ");
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setClusterConfigModeStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetModeChangeParametersMessage *pVcsNodeSetModeChangeParametersMessage = reinterpret_cast<VcsNodeSetModeChangeParametersMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    string                                 vcs_mode                               = pVcsNodeSetModeChangeParametersMessage->getVcsMode ();
    ResourceId                             status                                 = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setClusterConfigModeStep: Cluster config file is set for ") + vcs_mode);

    if ( (vcs_mode == "SA") || (vcs_mode == "FC"))
    {
        status = setClusterConfFile (false);
    }
    else if (vcs_mode == "MC")
    {
        status = setClusterConfFile (true);
    }

    return (status);
}

ResourceId VcsFabricLocalObjectManager::disableVcsModeStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{       
        
    string vcsmode = "false";
    updateThisVcsNodeLocalManagedObject ();
    setThisVcsNodeLocalManagedObjectVcsMode (0);
    setThisVcsNodeLocalManagedObjectVcsId (0);

    if ( pVcsNodeLocalMessagingContext->getPrincipalControlFailoverFlag () )
    {
        setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (true);
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::disableVcsModeStep : switchDeleteAfterFailover flag set to true in preparation for control failover");
    }    

    trace (TRACE_LEVEL_INFO,"VcsFabricLocalObjectManager::disableVcsModeStep: vcs mode set to "+vcsmode);
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::setVcsModeStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage = reinterpret_cast<VcsNodeSetVcsModeMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    if (m_isHaSyncInProgress)
    {
        pVcsNodeSetVcsModeMessage->setMessageString ("HA sync in progress.  Cannot perform the vcs mode change at this time.");

        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsModeStep: HA sync in progress.  Cannot perform the vcs mode change at this time.");

        return (WAVE_MESSAGE_ERROR);
    }

    bool vcs_mode = pVcsNodeSetVcsModeMessage->getVcsMode ();

    updateThisVcsNodeLocalManagedObject ();
    setThisVcsNodeLocalManagedObjectVcsMode (vcs_mode);

    string vcsmode = vcs_mode ? "true":"false";
    trace (TRACE_LEVEL_INFO,"VcsFabricLocalObjectManager::setVcsModeStep: vcs mode set to "+vcsmode);
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::getVcsModeStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    VcsNodeGetVcsModeMessage *pVcsNodeGetVcsModeMessage = reinterpret_cast<VcsNodeGetVcsModeMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

    pVcsNodeGetVcsModeMessage->setVcsMode(getThisVcsNodeLocalManagedObjectVcsMode ());
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::validateClusterOperationInProgress (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    if ((VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()) || (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()) || (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()) || (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()) || (true == FrameworkToolKit::getSecondaryClusterFormationFlag ())) 
    {
        trace(TRACE_LEVEL_INFO,"VcsFabricLocalObjectManager::validateClusterOperationInProgress: Cluster operation in progress. Hence erroring enquiry");
        return (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::changeClusterTypeConfigDistributeStep (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext)
{
    trace(TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::changeClusterTypeConfigDistributeStep: Entering");

    ResourceId vcsNodeClusterType = pVcsNodeConfigDistributeContext->getVcsClusterType ();

    // If the local node is in fabric cluster mode then explicitly change to management cluster and start 

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == vcsNodeClusterType)
    {
        trace(TRACE_LEVEL_INFO,"VcsFabricLocalObjectManager::changeClusterTypeConfigDistributeStep: Changing cluster type from Fabric to Management Cluster.");
        if (WAVE_MESSAGE_SUCCESS != setClusterConfFile (true))
        {
            return (WAVE_MESSAGE_ERROR);
        }
        
        setThisVcsNodeClusterType (true, true);
    }

    // If the local node is in management cluster mode then explicitly change to fabric cluster

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == vcsNodeClusterType)
    {
        trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::changeClusterTypeConfigDistributeStep: Changing cluster type from Management to Fabric Cluster.");
        if (WAVE_MESSAGE_SUCCESS != setClusterConfFile (false))
        {
            return (WAVE_MESSAGE_ERROR);
        }

        setThisVcsNodeClusterType (true, false);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VcsFabricLocalObjectManager::validateSwitchReplacementCaseStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
	VcsEnqSwitchMessage *pEnqSwitchMessage = reinterpret_cast<VcsEnqSwitchMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());
    VcsFabricLocalSwitchInfoObject	localSwitchInfo = pEnqSwitchMessage->getRemoteVcsSwitchInfoObject ();
	vector<WorldWideName> wwnOfReplacementNodes = localSwitchInfo.getReplacementSwitchWWN ();
	vector<SI32> swbdOfReplacementNodes = localSwitchInfo.getReplacementSwitchSwbd ();

	UI32 myDomainId = DcmToolKit::getThisVcsNodeDomainId();
	WorldWideName myWwn = DcmToolKit::getSwitchWwnFromDomainId(myDomainId);
	SI32 mySwbd = DcmToolKit::getSwBDModelForDomainId( myDomainId );

	for (int i=0; i < (int) wwnOfReplacementNodes.size (); i++)
	{
		if (myWwn == wwnOfReplacementNodes[i])
		{
			if (mySwbd == swbdOfReplacementNodes[i])
			{
				return WAVE_MESSAGE_SUCCESS; // this is a replacement switch and the swbd model is same as switch to be replaced
			}
			else
			{
				trace(TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::validateSwitchReplacementCaseStep: Replacement switch model mismatch: ")+ "replacement switch swbd = " + swbdOfReplacementNodes[i]  + "my swbd = " + mySwbd + "This node will be segmented from fabric");
				return REPLACEMENT_SWITCH_MODEL_MISMATCH;
			}
		}
	}

	return WAVE_MESSAGE_SUCCESS; // no list of replacement switches in the enq message

}

ResourceId VcsFabricLocalObjectManager::getSwitchParamsStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
	ResourceId						status = WAVE_MESSAGE_SUCCESS;
    VcsFabricLocalSwitchInfoObject	localSwitchInfo;

    VcsEnqSwitchMessage *pEnqSwitchMessage = reinterpret_cast<VcsEnqSwitchMessage *> (pVcsNodeLocalMessagingContext->getPPrismMessage ());

		//NodeAutoUpgradeConfiguration AutoUpgradeConfigData = pEnqSwitchMessage->getNodeAutoUpgradeConfiguration();

	ResourceId currentDistributionMode = DcmToolKit::getDistributionMode();

	if (DCM_CCM == currentDistributionMode)
	{
		
		UI32							upgrade_status;

		if ((upgrade_status = FirmwareToolKit::getUpgradeStatus()) == 1)
		{
			pEnqSwitchMessage->setAutoUpgradeStatus(NA_STS_IN_PROGRESS);
			trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getSwitchParamsStep : AutoUpgrade in progress on this node.");

		}
		else
		{
			UI32 sizeOfData = 0;
			char * pAutoUpgradeData = reinterpret_cast<char *> (pEnqSwitchMessage->findBuffer (AUTO_UPGRADE_OBJECT, sizeOfData));

			if (pAutoUpgradeData != NULL)
			{
				string serializedData (pAutoUpgradeData);
				NodeAutoUpgradeConfiguration  AutoUpgradeConfigData;
				AutoUpgradeConfigData.loadFromSerializedData2 (serializedData);

				FirmwareOpAutoUpgradeMessage *pAutoUpgradeMessage = new FirmwareOpAutoUpgradeMessage(&AutoUpgradeConfigData);
			status = sendSynchronously(pAutoUpgradeMessage, FrameworkToolKit::getThisLocationId());

				trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getSwitchParamsStep : Sent AutoUpgrade Msg to Firmware Plug-in for this node : " + FrameworkToolKit::getThisLocationId());

				if (WAVE_MESSAGE_SUCCESS != status)
				{
					trace(TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::getSwitchParamsStep: Error in sending message to Firmware Plug-in: Status: \"" + FrameworkToolKit::localize(status) + "\"");

				}
				else
				{
					upgrade_status = pAutoUpgradeMessage->getStatus();
					pEnqSwitchMessage->setAutoUpgradeStatus(upgrade_status);

					if (upgrade_status == 0)
					{
						trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getSwitchParamsStep : AutoUpgrade in progress.");
					}
					else
					{
						trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getSwitchParamsStep : AutoUpgrade not done. Status : " + upgrade_status);
					}
				}

				if (WAVE_MESSAGE_SUCCESS != status)
				{
					trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::get"
						"SwitchParamsStep :Node not auto upgraded. Continue to"
						" join cluster");
				}

				delete pAutoUpgradeMessage;
			}
		}

	}

    m_fcPrincipalTrackingWorker->getLocalSwitchInfo (localSwitchInfo);

    m_fcPrincipalTrackingWorker->saveRemoteSwitchInfoFromEnqMessage (pEnqSwitchMessage);
    
    pEnqSwitchMessage->setRemoteVcsSwitchInfoObject (localSwitchInfo);

    return WAVE_MESSAGE_SUCCESS;
}

void VcsFabricLocalObjectManager::debugSetDomainId (UI32 argc, vector<string> argv)
{
    ResourceId          status          = FRAMEWORK_ERROR;
    UI32 domainId = 0;

    if (argc < 2)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::debugSetDomainId : Usage: setDomainId <domain_id>");
        return;
    }

    WaveNs::trace (TRACE_LEVEL_INFO, " VcsFabricLocalObjectManager::debugSetDomainId: running command");

    //VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = (VcsFabricLocalObjectManager::getInstance ())->getLocalManagedObject ();

    string serialNumber = FrameworkToolKit::getThisLocationIpAddress () + string (":") + FrameworkToolKit::getThisLocationPort ();

    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance(); 
    pVcsFabricLocalObjectManager->startTransaction();
    pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();
    domainId = atoi (argv[1].c_str ());
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectDomainId(domainId);
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectSerialNumber(serialNumber);
    pVcsFabricLocalObjectManager->setThisVcsNodeSerialNumber (serialNumber); // set the cached value of serialNumber so it gets copied into the vcsNodeLocalManagedObject on service reboot after cluster creation
    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();
    pVcsNodeFabosInterfaceObjectManager->setDomainId (domainId);
    status = pVcsFabricLocalObjectManager->commitTransaction ();

    if (FRAMEWORK_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::debugSetDomainId: Failed to set domain id.");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    return;
}

void VcsFabricLocalObjectManager::debugGenerateSCN (UI32 argc, vector<string> argv)
{
    ResourceId            status           = WAVE_MESSAGE_SUCCESS;
    ResourceId            completionStatus = WAVE_MESSAGE_SUCCESS;

    if (argc < 3 || (0 == atoi (argv[1].c_str ()) && argc < 4))
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::debugGenerateSCN : Usage: generateSCN <0|1|2> <domain_id> [principal_id]");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                0 - Domain Valid");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                1 - Domain Capabilities Available");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                2 - Domain Invalid");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                3 - Domain Reachable");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                4 - Domain Unreachable");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                6 - Zone Changed");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                7 - Zone Aborted");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                8 - Switch Ready to accept commands");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                9 - Switch Offline");

        return;
    }

    VcsNodeGenerateSCNMessage   *message = new VcsNodeGenerateSCNMessage ();
    message->setSCNType (atoi (argv[1].c_str ()));
    message->setDomainId (atoi (argv[2].c_str ()));

    // Set the principal only for Domain Valid
    if (0 == atoi (argv[1].c_str ()))
    {
        message->setPDomainId (atoi (argv[3].c_str ()));
    }

    WaveNs::trace (TRACE_LEVEL_INFO, " VcsFabricLocalObjectManager::debugGenerateSCN: running command");
    status = ShellPrism::shellSendSynchronously (message);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        completionStatus = message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::debugGenerateSCN : Success.");
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::debugGenerateSCN : Failed to generate SCN.  Status : " + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::debugGenerateSCN : Could not send message to Service.  Status : " + FrameworkToolKit::localize (status));
    }
}

void VcsFabricLocalObjectManager::debugTriggerPostBootMessage (UI32 argc, vector<string> argv)
{
    if (argc < 2)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::debugTriggerPostBootMessage : Usage: triggerpostboot <#linecards> ");
        return;    
    }
    UI32 numberOfSlots = atoi (argv[1].c_str ());
    if (numberOfSlots > 32)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::debugTriggerPostBootMessage: number of slots set = 32 which is the max supported");
        numberOfSlots = 32;
    }
    UI32 i = 0;

    //Acquire linecardReadySlotIdBitMapMutex
    VcsFabricLocalObjectManager * pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();
    pVcsFabricLocalObjectManager->m_linecardReadySlotIdBitMapMutex.lock();

    for (i=1; i < numberOfSlots; i++)
    {
        pVcsFabricLocalObjectManager->m_linecardReadySlotIdBitMap.set (i);
    }
    
    //Release linecardReadySlotIdBitMapMutex
    pVcsFabricLocalObjectManager->m_linecardReadySlotIdBitMapMutex.unlock();
    
    VcsNodeTriggerPostBootMessage *pVcsNodeTriggerPostBootMessage = new VcsNodeTriggerPostBootMessage ();
    WaveNs::prismAssert (NULL != pVcsNodeTriggerPostBootMessage, __FILE__, __LINE__);

    ResourceId status = ShellPrism::shellSendSynchronously (pVcsNodeTriggerPostBootMessage);
    if (WAVE_MESSAGE_SUCCESS == status)
    {
        ResourceId completionStatus = pVcsNodeTriggerPostBootMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::debugTriggerPostBootMessage: Success.");
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::debugTriggerPostBootMessage : Failed to trigger post boot.  Status : " + FrameworkToolKit::localize (completionStatus));
        }
    }
}


void VcsFabricLocalObjectManager::debugGenerateSlotScn (UI32 argc, vector<string> argv)
{
    ResourceId            status           = WAVE_MESSAGE_SUCCESS;
    ResourceId            completionStatus = WAVE_MESSAGE_SUCCESS;

    if (argc < 3)        
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::debugGenerateSlotScn : Usage: generateslotscn <0|1|2|3|4> <slotNumber> [objectType] [hwId] [reason] [version]");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                0 - slot on");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                1 - slot in");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                2 - slot out");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                3 - slot fault");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                4 - slot remove");

        return;
    }

    UI32 slotScnType = atoi (argv[1].c_str ());
    UI32 slotNumber = (atoi (argv[2].c_str ()));
    UI32 hwType = SYS_BLADE_AP;
    UI32 slotState;
   
    switch (slotScnType)
    {
        case 0:
            slotState = SLOT_ON;
            break;
        case 1:
            slotState = SLOT_IN;
            break;
        case 2:
            slotState = SLOT_OUT;
            break;
        case 3:
            slotState = SLOT_FAULT;
            break;
        case 4:
            slotState = SLOT_IN | SLOT_OUT; // not real just for this function for slot remove simulation
            break;
        default:
            
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::debugGenerateSlotScn : Usage: generateslotscn <0|1|2|3|4> <slotNumber> [objectType] [hwId] [reason] [version]");
            WaveNs::trace (TRACE_LEVEL_INFO, "                                                0 - slot on");
            WaveNs::trace (TRACE_LEVEL_INFO, "                                                1 - slot in");
            WaveNs::trace (TRACE_LEVEL_INFO, "                                                2 - slot out");
            WaveNs::trace (TRACE_LEVEL_INFO, "                                                3 - slot fault");
            WaveNs::trace (TRACE_LEVEL_INFO, "                                                4 - slot remove");
            
            return;
    }
    
    if (slotScnType != 4)
    {
        VcsNodeGenerateSlotScnMessage   *message = new VcsNodeGenerateSlotScnMessage ();
        WaveNs::prismAssert (NULL != message, __FILE__, __LINE__);
        message->setSlotState (slotState);
        message->setSlotNumber (slotNumber);
        message->setHwType (hwType);
        
        if (argc > 3)
        {
            message->setObjectType (atoi (argv[3].c_str ()));   //blade function
        }
        
        if (argc > 4)
        {
            message->setHwId (atoi (argv[4].c_str ()));
        }

        if (argc > 5)
        {
            message->setReason (atoi (argv[5].c_str ()));
        }
        
        if (argc > 6)
        {
            message->setVersion (atoi (argv[6].c_str ()));
        }
        WaveNs::trace (TRACE_LEVEL_INFO, " VcsFabricLocalObjectManager::debugGenerateSlotScn: running slotscn command");
        status = ShellPrism::shellSendSynchronously (message);
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = message->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::debugGenerateSlotScn : Success.");
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::debugGenerateSlotScn : Failed to generate slot SCN.  Status : " + FrameworkToolKit::localize (completionStatus));
            }
        }
        
    }
    else
    {
        UI32 rbridgeId = DcmToolKit::getThisVcsNodeDomainId ();
        if (argc > 3)
        {
            rbridgeId = (atoi (argv[3].c_str ()));    
        }

        if ((hwType == SYS_BLADE_AP) || (hwType == SYS_BLADE_SW))
        {
            hwType = SYS_SLOT_LC;
        }

        VcsNodeSlotRemoveMessage *message = new VcsNodeSlotRemoveMessage (slotNumber, rbridgeId, hwType);
        WaveNs::prismAssert (NULL != message, __FILE__, __LINE__);
        slotNumber = message->getSlotNumber ();
        hwType = message->getHwType ();
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::debugGenerateSlotScn: running remove slot command slotNumber = ")+slotNumber+" hwType = "+hwType+" rbridgeId = "+rbridgeId);
        status = ShellPrism::shellSendSynchronously (message);
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = message->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::debugGenerateSlotScn : Success.");
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::debugGenerateSlotScn : Failed to generate slot SCN.  Status : " + FrameworkToolKit::localize (completionStatus));
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::debugGenerateSlotScn : Could not send message to Service.  Status : " + FrameworkToolKit::localize (status));
        }
    }
    
}

void VcsFabricLocalObjectManager::debugTriggerHaMessage (UI32 argc, vector<string> argv)
{
    if (argc < 3)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::debugTriggerHaMessage : Usage: triggerha <1> <parameter>");
        WaveNs::trace (TRACE_LEVEL_INFO, "                                                1 - start sync dump (haport# as parameter");
        WaveNs::trace (TRACE_LEVEL_INFO, "triggerha eventType<1> <parameter>");

        return;
    }

    UI32 event = atoi (argv[1].c_str ());    
    UI32 paramter = atoi (argv[2].c_str ());

    switch (event)
    {
        case 1:
			SI32 haPort = SI32(paramter);
            VcsNodeNotifyStartSyncDumpMessage *pVcsNodeNotifyStartSyncDumpMessage = new VcsNodeNotifyStartSyncDumpMessage ("127.0.0.1", haPort);

            WaveNs::prismAssert (NULL != pVcsNodeNotifyStartSyncDumpMessage, __FILE__, __LINE__);

            ResourceId status = ShellPrism::shellSendSynchronously (pVcsNodeNotifyStartSyncDumpMessage);
            if (WAVE_MESSAGE_SUCCESS == status)
            {
                 ResourceId completionStatus = pVcsNodeNotifyStartSyncDumpMessage->getCompletionStatus ();
                 if (WAVE_MESSAGE_SUCCESS == completionStatus)
                 {
                      WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::debugiTrigerHaMessage: Success.");
                 }
                 else
                 {
                      WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::debugTriggerHaMessage : Failed to trigger Ha event.  Status : " + FrameworkToolKit::localize (completionStatus));
                 }
            }
    }
}

bool VcsFabricLocalObjectManager::getIsWyserEaPhase2Complete ()             
{
    bool isWyserEaPhase2Complete = false;

    m_isWyserEaPhase2CompleteMutex.lock ();
    isWyserEaPhase2Complete = m_isWyserEaPhase2Complete;
    m_isWyserEaPhase2CompleteMutex.unlock ();

    return (isWyserEaPhase2Complete);
}

void VcsFabricLocalObjectManager::setIsWyserEaPhase2Complete (bool isWyserEaPhase2Complete)
{
    m_isWyserEaPhase2CompleteMutex.lock ();
    m_isWyserEaPhase2Complete = isWyserEaPhase2Complete;
    m_isWyserEaPhase2CompleteMutex.unlock ();
}

void VcsFabricLocalObjectManager::setThisVcsNodeSerialNumber (const string &thisVcsNodeSerialNumber)
{
    m_thisVcsNodeSerialNumber = thisVcsNodeSerialNumber;
}

string VcsFabricLocalObjectManager::getThisVcsNodeSerialNumber ()
{
    return m_thisVcsNodeSerialNumber;
}

void VcsFabricLocalObjectManager::setThisVcsNodeSwitchMac (const MacAddress &thisVcsNodeSwitchMac)
{
    m_thisVcsNodeSwitchMac = thisVcsNodeSwitchMac;
}

MacAddress VcsFabricLocalObjectManager::getThisVcsNodeSwitchMac ()
{
    return m_thisVcsNodeSwitchMac;
}

void VcsFabricLocalObjectManager::setThisVcsNodeSwitchWWN (const WorldWideName &thisVcsNodeSwitchWWN)
{
    m_thisVcsNodeSwitchWWN = thisVcsNodeSwitchWWN;
}

WorldWideName VcsFabricLocalObjectManager::getThisVcsNodeSwitchWWN ()
{
    return m_thisVcsNodeSwitchWWN;
}

void VcsFabricLocalObjectManager::setThisVcsNodeObjectId (const ObjectId &thisVcsNodeObjectId)
{
    m_thisVcsNodeObjectIdMutex.lock ();

    m_thisVcsNodeObjectId = thisVcsNodeObjectId;
    trace (TRACE_LEVEL_INFO, string ("setting local object id to ") + m_thisVcsNodeObjectId.toString());

    m_thisVcsNodeObjectIdMutex.unlock ();
}

ObjectId VcsFabricLocalObjectManager::getThisVcsNodeObjectId ()
{
    ObjectId tempObjectId;

    m_thisVcsNodeObjectIdMutex.lock ();

    tempObjectId = m_thisVcsNodeObjectId;

    m_thisVcsNodeObjectIdMutex.unlock ();

    return (tempObjectId);
}

ObjectId VcsFabricLocalObjectManager::getVcsNodeObjectIdFromMappedId (const UI32 &mapped_id)
{
    VcsNodeLocalManagedObject               *pVcsNodeLocalManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

    /* Get the object corresponding to this mapped ID-which is replaced by domainId */
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (mapped_id, "domainId"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {

            trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::getVcsNodeObjectIdFromMappedId : Cannot find Vcs Node with domainId = ")+mapped_id);
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (1 == numberOfResults)
        {
            if (NULL != (*pResults)[0])
            {
                pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::getVcsNodeObjectIdFromMappedId : DB query returns one object but pointer to the object is NULL - should not happen so assert"));
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::getVcsNodeObjectIdFromMappedId: There can be only one local node #nodes found = ")+numberOfResults+ "for domainId = "+mapped_id);
            prismAssert (false, __FILE__, __LINE__);
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::getVcsNodeObjectIdFromMappedId : System failure pResults is NULL");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalManagedObject->getObjectId());
}

UI32 VcsFabricLocalObjectManager::updateLocalManagedObject (string chassisString, MacAddress &switchMac, int domainId, WorldWideName &switchWWN, int SWBDModel, const string &fwVersion, const string &swmodelName)
{
    ResourceId          status          = FRAMEWORK_ERROR;
    /****************************************************************************
     * Update managed object and commit to database
     ***************************************************************************/
    startTransaction();
    updateThisVcsNodeLocalManagedObject ();

    setThisVcsNodeLocalManagedObjectSerialNumber (chassisString);
    setThisVcsNodeLocalManagedObjectSwitchMac (switchMac);
    setThisVcsNodeLocalManagedObjectSwitchWWN (switchWWN);
    setThisVcsNodeLocalManagedObjectSWBDModel       (SWBDModel);
    setThisVcsNodeLocalManagedObjectSwitchFwVersion (fwVersion);
    setThisVcsNodeLocalManagedObjectSwmodelName     (swmodelName);

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == VcsFabricLocalObjectManager::getThisVcsNodeClusterType ())
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::updateLocalManagedObject: Fabric mode: setting mappedId same as domainId: ") + domainId);
        setThisVcsNodeLocalManagedObjectDomainId (domainId);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        setThisVcsNodeSerialNumber (chassisString);
        setThisVcsNodeSwitchWWN (switchWWN);
		setThisVcsNodeSwitchMac(switchMac);
        setThisVcsNodeSWBDModel         (SWBDModel);
        setThisVcsNodeSwitchFwVersion   (fwVersion);
        setThisVcsNodeSwmodelName       (swmodelName);

        return 0;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
        return 1;
    }
}

void VcsFabricLocalObjectManager::domainValidEventHandler (const VcsNodeFabosDomainValidEvent *&pEvent)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::domainValidEventHandler: Entering...");
    // Postponing SCN handling until boot is completed 

    if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainValidEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    ResourceId                      status          = WAVE_MESSAGE_SUCCESS;
    VcsNodeDomainValidEvent        *pDVEvent        = NULL;
    const VcsNodeDomainValidEvent  *pDVEventCopy    = NULL;

    string     ethernetInterface = PrismFrameworkObjectManager::getEthernetInterfaceForThisLocation ();

    // Reset the ip address since our ip address might have changed.

    // First Reset this node IP address if required.


    if ("" == ethernetInterface)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainValidEventHandler : No specific Ethernet Interface Assigned and hence no IP Address resetting is required.");
    }
    else
    {
        string ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);

        if ("" == ipAddress)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::domainValidEventHandler : No IP Address assigned to user assigned Ethernet Interface \"" + ethernetInterface + "\"");

            ////////////////////////////////////
            // Temporary fix - needs to be reenabled
            ////////////////////////////////////
            //prismAssert (false, __FILE__, __LINE__);
        }
        else if (FrameworkToolKit::getThisLocationIpAddress () == ipAddress)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainValidEventHandler : IP Address unchanged for Ethernet Interface \"" + ethernetInterface + "\"");
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "---VcsFabricLocalObjectManager::domainValidEventHandler : Resetting IP Address to :\"" + ipAddress + "\" for this node." + FrameworkToolKit::getThisLocationId ());

            ClusterLocalSetThisNodeIpAddressMessage message         (ipAddress);
            ResourceId                              messageStatus = WAVE_MESSAGE_ERROR;

            messageStatus = sendSynchronously (&message, FrameworkToolKit::getThisLocationId ());
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainValidEventHandler : After Sending Resetting IP Address to :\"" + ipAddress + "\" for this node." + FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != messageStatus)
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::domainValidEventHandler : Error in sending message to set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");

                status = messageStatus;
            }
            else
            {
                messageStatus = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != messageStatus)
                {
                    trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::domainValidEventHandler : Failed to set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");

                    status = messageStatus;
                }
                else
                {
                    trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::domainValidEventHandler : successfully set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    UI32 domainId = pEvent->getDomainId ();

    // if support for dynamic RBridge Id allocation is enabled then only this could should be triggered.
    if (getThisVcsNodeLocalManagedObjectDomainId () != domainId) 
    {
        startTransaction();
    
        updateThisVcsNodeLocalManagedObject ();
        setThisVcsNodeLocalManagedObjectDomainId (domainId);

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::domainValidEventHandler: Failed to set domain id.");
            prismAssert (false, __FILE__, __LINE__);
            return;
        }
    }
    
    pDVEvent = new VcsNodeDomainValidEvent ();

    pDVEvent->setDomainId           (domainId);
    pDVEvent->setPrincipalDomainId  (pEvent->getPrincipalDomainId());
    pDVEvent->setEventId            (pEvent->getEventId());

    pDVEventCopy = pDVEvent;
    
    status = m_fcPrincipalTrackingWorker->DomainValidEventHandler(pDVEventCopy);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pDVEvent);
    }
    else
    {
        delete (pDVEvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::domainCapabilitiesAvailableEventHandler (const VcsNodeFabosDomainCapabilitiesAvailableEvent *&pEvent)
{
    // Postponing SCN handling until boot is completed 

    if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainCapabilitiesAvailableEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    ResourceId status = WAVE_MESSAGE_ERROR;
    VcsNodeDomainCapabilitiesAvailableEvent *pDCAEvent                  = NULL;
    const VcsNodeDomainCapabilitiesAvailableEvent *pDCAEventCopy              = NULL;

    pDCAEvent = new VcsNodeDomainCapabilitiesAvailableEvent ();
    pDCAEvent->setDomainId (pEvent->getDomainId ());
    pDCAEvent->setEventId(pEvent->getEventId());
    pDCAEventCopy = pDCAEvent;
    
    status = m_fcPrincipalTrackingWorker->DomainCapabilitiesAvailableEventHandler (pDCAEventCopy);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pDCAEvent);
    }
    else
    {
        delete (pDCAEvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::domainInvalidEventHandler (const VcsNodeFabosDomainInvalidEvent *&pEvent)
{
    // Postponing SCN handling until boot is completed 

    ResourceId status = WAVE_MESSAGE_ERROR;

    if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainInvalidEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    VcsNodeDomainInvalidEvent               *pDIEvent                   = NULL;
    const VcsNodeDomainInvalidEvent         *pDIEventCopy               = NULL;

    pDIEvent = new VcsNodeDomainInvalidEvent ();

    pDIEvent->setDomainId (pEvent->getDomainId ());
    pDIEvent->setEventId(pEvent->getEventId());

    pDIEventCopy = pDIEvent;
    
    status = m_fcPrincipalTrackingWorker->DomainInvalidEventHandler (pDIEventCopy);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pDIEvent);
    }
    else
    {
        delete (pDIEvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::domainReachableEventHandler (const VcsNodeFabosDomainReachableEvent *&pEvent)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::domainReachableEventHandler: Entering...");
    // Postponing SCN handling until boot is completed 
    ResourceId status = WAVE_MESSAGE_ERROR;

    if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainReachableEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    VcsNodeDomainReachableEvent             *pDREvent                   = NULL;
    const VcsNodeDomainReachableEvent       *pDREventCopy               = NULL;

    pDREvent = new VcsNodeDomainReachableEvent ();

    pDREvent->setDomainId (pEvent->getDomainId ());
    pDREvent->setEventId(pEvent->getEventId());

    pDREventCopy = pDREvent;
    
    status = m_fcPrincipalTrackingWorker->DomainReachableEventHandler (pDREventCopy);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pDREvent);
    }
    else
    {
        delete (pDREvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::switchOfflineEventHandler (const VcsNodeFabosSwitchOfflineEvent *&pEvent)
{
    // Postponing SCN handling
    ResourceId status = WAVE_MESSAGE_ERROR;

    if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::switchOfflineEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    VcsNodeSwitchOfflineEvent         *pSwitchOfflineEvent                   = NULL;
    const VcsNodeSwitchOfflineEvent   *pSwitchOfflineEventCopy               = NULL;

    pSwitchOfflineEvent = new VcsNodeSwitchOfflineEvent ();
    pSwitchOfflineEvent->setEventId(pEvent->getEventId());

    pSwitchOfflineEventCopy = pSwitchOfflineEvent;

    status = m_fcPrincipalTrackingWorker->SwitchOfflineEventHandler(pSwitchOfflineEventCopy);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pSwitchOfflineEvent);
    }
    else
    {
        delete (pSwitchOfflineEvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::domainUnreachableEventHandler (const VcsNodeFabosDomainUnreachableEvent *&pEvent)
{
    // Postponing SCN handling until boot is completed

    ResourceId status = WAVE_MESSAGE_ERROR;

    if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainUnreachableEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    VcsNodeDomainUnreachableEvent           *pDUEvent                   = NULL;
    const VcsNodeDomainUnreachableEvent     *pDUEventCopy               = NULL;

    pDUEvent = new VcsNodeDomainUnreachableEvent ();

    pDUEvent->setDomainId (pEvent->getDomainId ());
    pDUEvent->setEventId(pEvent->getEventId());

    pDUEventCopy = pDUEvent;
    
    status = m_fcPrincipalTrackingWorker->DomainUnReachableEventHandler (pDUEventCopy);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pDUEvent);
    }
    else
    {
        delete (pDUEvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::nodeDeletedEventHandler (const WaveNodeLocalNodeDeletedEvent* & pEvent)
{
    if ( true == m_noVcsEnableInProgressFlag || true == m_pVcsNodeConfigSnapshotWorker->getConfigSnapshotRestoreFlag() )
    {
        if ( true == m_pVcsNodeConfigSnapshotWorker->getConfigSnapshotRestoreFlag())
        {   
            string retMessage("");
            ResourceId status = copyDefaultToStartInModeChange (true, false, retMessage );
            if ( WAVE_MESSAGE_SUCCESS != status )
            {   
                trace (TRACE_LEVEL_ERROR, string("VcsFabricLocalObjectManager::nodeDeletedEventHandler: CopydefaultToStart failed for snapshot restore with status-") + FrameworkToolKit::localizeToSourceCodeEnum (status));
            }
        }
        else //if call is for the configsnapshot, dont delete the Ccmd file here as it is already delete in the above step. 
        {
            if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfg())
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::nodeDeletedEventHandler: Failed to remove Ccmd cfg file");
        }

            if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfgOnStandby ())
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::nodeDeletedEventHandler: Failed to remove Ccmd cfg file on standby");
            }
        }
        
        vector<string> output;
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::nodeDeletedEventHandler: attempting to reboot");
        FrameworkToolKit::systemCommandOutput ("/fabos/sbin/chassisReboot", output);
    }
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::clusterDeleteEventHandler (const WaveNodeDeleteClusterEvent* &pEvent)
{
    if (true == pEvent->getIsRebootRequired ())
    {
        if ( true == m_pVcsNodeConfigSnapshotWorker->getConfigSnapshotRestoreFlag())
        {   
            string retMessage("");
            ResourceId status = copyDefaultToStartInModeChange (true, false, retMessage );
            if ( WAVE_MESSAGE_SUCCESS != status )
            {   
                trace (TRACE_LEVEL_ERROR, string("VcsFabricLocalObjectManager::clusterDeleteEventHandler: CopydefaultToStart failed for snapshot restore with status-") + FrameworkToolKit::localizeToSourceCodeEnum (status));
            }
        }
        else //if call is for the configsnapshot, dont delete the Ccmd file here as it is already delete in the above step.
        {
            if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfg())
        {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::clusterDeleteEventHandler: Failed to remove Ccmd cfg file");
        }
        
            if ( WAVE_MESSAGE_SUCCESS != VcsFabricLocalObjectManager::removeCcmdCfgOnStandby ())
            { 
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::clusterDeleteEventHandler: Failed to remove Ccmd cfg file on standby");
            }
        }

        vector<string> output;
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::clusterDeleteEventHandler: attempting to reboot on Primary");
        FrameworkToolKit::systemCommandOutput ("/fabos/sbin/chassisReboot", output);
    }
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

ResourceId VcsFabricLocalObjectManager::removeCcmdCfgOnStandby ()
{

    if (FrameworkToolKit::getIsSyncDumpCompleted () && FrameworkToolKit::getIsLiveSyncEnabled ())
    {           
    
        VcsRemoveStandbyCfgFileMessage * pVcsRemoveStandbyCfgFileMessage = new VcsRemoveStandbyCfgFileMessage ("CCMD"); 

        ResourceId messageStatus = VcsFabricLocalObjectManager::getInstance ()->sendSynchronously (pVcsRemoveStandbyCfgFileMessage, 1);

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::removeCcmdCfgOnStandby : Error in sending VcsRemoveStandbyCfgFileMessage : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
            delete pVcsRemoveStandbyCfgFileMessage;
            return messageStatus;
        }
        else
        {
            ResourceId completionStatus = pVcsRemoveStandbyCfgFileMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::removeCcmdCfgOnStandby : Error in VcsRemoveStandbyCfgFileMessage Completion Status : \"" + FrameworkToolKit::localize (completionStatus) + "\"");
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::removeCcmdCfgOnStandby : successfully remove ccmd cfg on standby");
            }
            delete pVcsRemoveStandbyCfgFileMessage;
            return completionStatus;

        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::removeCcmdCfgOnStandby : sync dump not completed or live sync not enabled so don't send VcsRemoveStandbyCfgFileMessage to standby")); 
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::removeDcmdCfgOnStandby ()
{

    if (FrameworkToolKit::getIsSyncDumpCompleted () && FrameworkToolKit::getIsLiveSyncEnabled ())
    {

        VcsRemoveStandbyCfgFileMessage * pVcsRemoveStandbyCfgFileMessage = new VcsRemoveStandbyCfgFileMessage ("DCMD");
        ResourceId messageStatus = VcsFabricLocalObjectManager::getInstance ()->sendSynchronously (pVcsRemoveStandbyCfgFileMessage, 1);

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::removeDcmdCfgOnStandby : Error in sending VcsRemoveStandbyCfgFileMessage : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
            delete pVcsRemoveStandbyCfgFileMessage;
            return messageStatus;
        }
        else
        {
            ResourceId completionStatus = pVcsRemoveStandbyCfgFileMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::removeDcmdCfgOnStandby : Error in VcsRemoveStandbyCfgFileMessage Completion Status : \"" + FrameworkToolKit::localize (completionStatus) + "\"");
            }

            delete pVcsRemoveStandbyCfgFileMessage;
            return completionStatus;
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::removeDcmdCfgOnStandby : isSyncDumpCompleted is false so don't send VcsRemoveStandbyCfgFileMessage to standby"));  
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::removeCcmdCfg ()
{

    string changeCfgValidityCommand = "";
    ResourceId status = WAVE_MESSAGE_ERROR;

    if (CCM_ONLY == DcmToolKit::getDistributionMode () || CCM_NOOP == DcmToolKit::getDistributionMode ())
    {
        status = FrameworkToolKit::changePrismConfigurationValidity (false);
    }
    else if (DCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 7110 debug setPrismConfigurationValid false ";
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileMessageHandler: connect to ccmd to remove cfg file");
        vector<string>  output;
        SI32        returnStatus = FrameworkToolKit::systemCommandOutput(changeCfgValidityCommand.c_str (), output);
        string outputString = "";

        if (0 < output.size ())
        {
            outputString = output[0];
        }

        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::removeCfgFileMessageHandlerStep: returned with message : %s, and  return status : %d", outputString.c_str(), returnStatus); 
    
        if (0 == returnStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileMessageHandler: do nothing");
        status = WAVE_MESSAGE_SUCCESS;
    }

    return status;
}

ResourceId VcsFabricLocalObjectManager::removeDcmdCfg ()
{

    string changeCfgValidityCommand = "";
    ResourceId status = WAVE_MESSAGE_ERROR;

    if (DCM_ONLY == DcmToolKit::getDistributionMode () || DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        status = FrameworkToolKit::changePrismConfigurationValidity (false);
    }
    else
    {
        changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 9110 debug setPrismConfigurationValid false ";
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsRemoveCfgFileMessageHandler: connect to dcmd to remove cfg file");
        vector<string>  output;
        SI32        returnStatus = FrameworkToolKit::systemCommandOutput(changeCfgValidityCommand.c_str (), output);
        string outputString = "";

        if (0 < output.size ())
        {
            outputString = output[0];
        }

        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::removeCfgFileMessageHandlerStep: returned with message : %s, and  return status : %d", outputString.c_str(), returnStatus); 
    
        if (0 == returnStatus)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
        }
    }

    return status;
}

void VcsFabricLocalObjectManager::postbootStartedEventHandler (const PostBootStartedEvent* &pEvent)
{
    string configName;
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::postbootStartedEventHandler: Entered...");

    switch (pEvent->getPostBootStartedEventId())
	{
        case POSTBOOT_GLOBAL_CONFIG:
        configName = string("global config");
        break;

        case POSTBOOT_LINECARD_CONFIG:
        configName = string("L") + pEvent->getPostBootStartedEventParameter() + string(" config");
        break;
    }
	raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1002, configName.c_str());
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::postbootCompletedEventHandler (const PostBootCompletedEvent* &pEvent)
{
    string configName;
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::postbootCompletedEventHandler: Entered...");

    switch (pEvent->getPostBootCompletedEventId())
    {
        case POSTBOOT_GLOBAL_CONFIG:
        configName = string("global config");
        break;

        case POSTBOOT_LINECARD_CONFIG:
        UI32 slotId = pEvent->getPostBootCompletedEventParameter();
        configName = string("L") + slotId + string(" config");
        VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmSlotPostbootDone(slotId);
        break;
    }
    raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1003, configName.c_str());
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::configReplayStartedEventHandler (const ConfigReplayStartedEvent* &pEvent)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::configReplayStartedEventHandler: Entered...");

    if (pEvent->getConfigFileReplayFlag() == true)
    {
        raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1004);
    }
    else
    {
        raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1007);
    }
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::configReplayCompletedEventHandler (const ConfigReplayCompletedEvent* &pEvent)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::configReplayCompletedEventHandler: Entered...");
    raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1005);

    m_isConfigReplayCompleted = true;

    //Now replay the SCNs only for DCMD, since configreplay is complete after reboot of the node -> defect 367357
    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ())) 
    {
        UI32 numberOfScnQueueEventsReplayed = 0;

        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::configReplayCompletedEventHandler:FC Principal enabled, replaySCN"));

        numberOfScnQueueEventsReplayed = m_fcPrincipalTrackingWorker->replaySCNQueueEvents ();

        if (0 == numberOfScnQueueEventsReplayed)
        {
            u_short fabDomainList[257];
            size_t  recv_size = (sizeof (u_short)) * 257;

            SI32 returnCode = DcmToolKit::getDomainListInFabric (fabDomainList, recv_size);

            if (0 == returnCode)
            {
                UI32        numberOfNodesInFabric   = fabDomainList[0];
                ResourceId  recoveryType            = pEvent->getRecoveryType ();

                if ((WAVE_HA_WARM_RECOVERY == recoveryType) && (1 == numberOfNodesInFabric))
                {
                    // Start the principal tracking timer in the warm recovery case where we have a single node in the fabric.
                    // Since Domain SCNs (Domain Valid, and Domain Capabilites Available) are not received in single node warm
                    // recovery case, meaning there is no regular SCN trigger to start VCS cluster principle role change from
                    // unconfirmed to confirmed principal, we need to trigger VCS cluster principle role change for the single
                    // node here.

                    m_fcPrincipalTrackingWorker->startPrincipalTrackingTimer (1);
                }
            }
            else
            {
                trace (TRACE_LEVEL_WARN, string ("VcsFabricLocalObjectManager::configReplayCompletedEventHandler : DcmToolKit::getDomainListInFabric () returned : ") + returnCode + ".  Cannot determine current number of nodes in the fabric.");
            }
        }

        getFcfMacFromFcoeBackendAndUpdateMO ();
    }

//    m_vcsNodeSlotManagementWorker->replaySlotScnQueueEvents();

    // Send switch ready to accept Commands SCN only in case of fabric cluster mode
    if (DCM_ONLY == DcmToolKit::getDistributionMode ())
    { 
        // get the fcf MAC addressfrom fcoe backen and passit on to Ccmd as Ccmd is not connected to fcoe client directly 
        getFcfMacFromFcoeBackendAndNotifyCcmd ();    

        //Config replay complete in dcmd indicates that the switch is ready to accept all the command Send the SCN for CCMd to act on.
        switchReadyToAcceptCommands();
    }
 
    VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmConfigReplayDone();

    // Temporary place hold for sending clusterReady event at boot up before final optimization done
    bool readyState = true;
    VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmClusterReadyState(readyState);
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::lastConfigReplayCompletedEventHandler (const LastConfigReplayCompletedEvent* &pEvent)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::lastConfigReplayCompletedEventHandler: Entered...");

    ResourceId lastConfigReplayCompletionStatus = pEvent->getLastConfigReplayCompletedEventStatus ();

    if (WAVE_MESSAGE_SUCCESS == lastConfigReplayCompletionStatus)
    {
        raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1110);
    }
    else
    {
        raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1111, FrameworkToolKit::localize (lastConfigReplayCompletionStatus).c_str ());
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndNotifyCcmd ()
{
    FcoeGlobalFcoeFabricMapMessage  *fcoeFabricMsg = NULL;
    ResourceId                      status = WAVE_MESSAGE_SUCCESS;
    string                          errString;
    UI32                            locationId, mappedId;

    fcoeFabricMsg = new FcoeGlobalFcoeFabricMapMessage();
    if (fcoeFabricMsg == NULL) {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndNotifyCcmd: fcoeFabricMsg msg create failed"));
        return ;
    }

    fcoeFabricMsg->setOperBmp(FCOE_FAB_MAP_OPER_GET_FCF_MAC_ADDR);
    fcoeFabricMsg->setVfid(FCOE_FAB_MAP_DEFAULT_VFID);
	fcoeFabricMsg->setName(FCOE_MO_DEFAULT_NAME);

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndNotifyCcmd: Getting mac address from Fcoe Backend"));

    status = sendSynchronously(fcoeFabricMsg);

    locationId = FrameworkToolKit::getThisLocationId();
    mappedId = DcmToolKit::getMappedIdFromLocationId(locationId);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndNotifyCcmd: Failed to send, Status:") + FrameworkToolKit::localize(status));
    } 
    else 
    {   
        ResourceId completionStatus = fcoeFabricMsg->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus) {
            trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndNotifyCcmd: Failure to process, Status:") + FrameworkToolKit::localize(completionStatus));
        } 
        else 
        {
            char *FcfMacAddrStr  = new char[50];
            sprintf(FcfMacAddrStr,"%s",fcoeFabricMsg->getFCFMacForLocation(mappedId));

            string newmac(FcfMacAddrStr);

            if (true == MacAddress::isValidMacAddressString (newmac))
            {
            MacAddress fcfMac( newmac);

            m_thisVcsNodeFcfMacMutex.lock ();
            m_thisVcsNodeFcfMac = fcfMac;
            m_thisVcsNodeFcfMacMutex.unlock ();

            setFcfMacAddress ();

            // Pass this FCF MAC to CCMD as well 
            vector<string>  output;
            string      setFcfMacAddressInCcmdCommand;
            setFcfMacAddressInCcmdCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverport 7110 dcmdebug setFcfMacAddress " + fcfMac.toString () ;
            
            status = FrameworkToolKit::systemCommandOutput (setFcfMacAddressInCcmdCommand.c_str (), output);
            string result = output[0];

            if (FRAMEWORK_SUCCESS != status)
            {   
                trace (TRACE_LEVEL_ERROR, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndNotifyCcmd:  Failed to set FCF MAC Address returned with message :") + result);
            }
        }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndNotifyCcmd:  Invalid FCF MAC Address returned with message : ") + newmac);
            }
        }
    }

    /* Free allocated memory before returning */
    if (fcoeFabricMsg != NULL) {
        delete fcoeFabricMsg;
    }
}

void VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndUpdateMO ()
{   
    FcoeGlobalFcoeFabricMapMessage  *fcoeFabricMsg = NULL;
    ResourceId                      status = WAVE_MESSAGE_SUCCESS;
    string                          errString;
    UI32                            locationId, mappedId;

    fcoeFabricMsg = new FcoeGlobalFcoeFabricMapMessage ();
    if (fcoeFabricMsg == NULL) {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndUpdateMO: fcoeFabricMsg msg create failed"));
        return;
    }
    
    fcoeFabricMsg->setOperBmp(FCOE_FAB_MAP_OPER_GET_FCF_MAC_ADDR);
    fcoeFabricMsg->setVfid(FCOE_FAB_MAP_DEFAULT_VFID);
    fcoeFabricMsg->setName(FCOE_MO_DEFAULT_NAME);

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndUpdateMO: Getting mac address from Fcoe Backend"));

    status = sendSynchronously(fcoeFabricMsg);

    locationId = FrameworkToolKit::getThisLocationId ();
    mappedId = DcmToolKit::getMappedIdFromLocationId (locationId);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndUpdateMO: Failed to send, Status:") + FrameworkToolKit::localize(status));
    }
    else
    {
        ResourceId completionStatus = fcoeFabricMsg->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus) {
            trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndUpdateMO: Failure to process, Status:") + FrameworkToolKit::localize(completionStatus));
        }
        else
        {
            char *FcfMacAddrStr  = new char[50];
            UI8  *fcfMacStr = fcoeFabricMsg->getFCFMacForLocation(mappedId);

            if ( NULL != fcfMacStr )
            {
                sprintf(FcfMacAddrStr,"%s",fcfMacStr);
            }
            else
            {
                FcfMacAddrStr[0] = '\0';
            }

            string newmac(FcfMacAddrStr);

            if (true == MacAddress::isValidMacAddressString (newmac))
            {
                MacAddress fcfMac( newmac);

                m_thisVcsNodeFcfMacMutex.lock ();
                m_thisVcsNodeFcfMac = fcfMac;
                m_thisVcsNodeFcfMacMutex.unlock ();

                setFcfMacAddress ();
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("VcsFabricLocalObjectManager::getFcfMacFromFcoeBackendAndUpdateMO:  Invalid FCF MAC Address returned with message : ") + newmac);
            }
        }
    }

    /* Free allocated memory before returning */
    if (fcoeFabricMsg != NULL) {
        delete fcoeFabricMsg;
    }
}

void VcsFabricLocalObjectManager::setFcfMacAddress ()
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    m_thisVcsNodeFcfMacMutex.lock ();
    
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager:: setFcfMacAddress: setting FCF MAC Address to : ") + m_thisVcsNodeFcfMac.toString());
    startTransaction ();

    updateThisVcsNodeLocalManagedObject ();
    
    setThisVcsNodeLocalManagedObjectfcfMac (m_thisVcsNodeFcfMac);

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS != status)
    {   
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager:: setFcfMacAddress:  Failed to set FCF MAC Address to : ") + m_thisVcsNodeFcfMac.toString());
    }
  
    m_thisVcsNodeFcfMacMutex.unlock ();
}

MacAddress VcsFabricLocalObjectManager::getFcfMacAddress()
{
    m_thisVcsNodeFcfMacMutex.lock ();
    MacAddress fcfMac( m_thisVcsNodeFcfMac );
    m_thisVcsNodeFcfMacMutex.unlock ();
    return fcfMac;
}

void VcsFabricLocalObjectManager::nodeAddedEventHandler (const WaveNewNodesAddedEvent* &pEvent)
{
    UI32        i = 0;
    LocationId  locationId;
    ResourceId specificStatus =  getThisVcsNodeLocalManagedObjectSpecificStatus();

    if( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED == specificStatus || VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_STARTED == specificStatus)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        startTransaction();

        updateThisVcsNodeLocalManagedObject ();

        if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED == specificStatus )
        {   
            //This node is being added to the cluster and phase2 is complete
            setThisVcsNodeLocalManagedObjectSpecificStatus( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_COMPLETED );
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::nodeAddedEventHandler: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Add Phase2 Completed ");
        }
        else if (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_STARTED == specificStatus)
        {   
            //This node is being rejoined to the cluster and phase2 is complete
            setThisVcsNodeLocalManagedObjectSpecificStatus( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_COMPLETED );
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::nodeAddedEventHandler: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Rejoin Phase2 Completed ");
        }

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::nodeAddedEventHandler: VcsNodeLocalManagedObject specific status has been committed to DB");
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::nodeAddedEventHandler: VcsNodeLocalManagedObject specific status commit to DB Failed");
            reply (reinterpret_cast<const PrismEvent *&> (pEvent));
            return;
//            prismAssert (false, __FILE__, __LINE__);
        }
    }    
    
    for(i = 0; i < pEvent->getNumberOfNewLocations(); i++)
    {
        locationId = pEvent->getLocationIdAtIndex(i);
        VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject   = NULL;
        WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

        /* Get the object corresponding to the locationId */
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size ();

            if (0 == numberOfResults)
            {
                trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::nodeAddedEventHandler: Cannot find Vcs Node");
                //prismAssert (false, __FILE__, __LINE__);
            }
            else if (1 == numberOfResults)
            {
                if (NULL != (*pResults)[0])
                {
                    pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
                    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::nodeAddedEventHandler: Domain id is ") + pVcsNodeLocalManagedObject->getDomainId ());
                }
                else
                {
                    trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::nodeAddedEventHandler: DB query returned one object but pointer to the object is NULL - this should'n happen so assert");
                    prismAssert (false, __FILE__, __LINE__);
                }
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::nodeAddedEventHandler: There can be only one local node.");
                prismAssert (false, __FILE__, __LINE__);
            }
            pResults->clear ();
            delete pResults;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::nodeAddedEventHandler: System failure");
            prismAssert (false, __FILE__, __LINE__);
        }

        if (NULL != pVcsNodeLocalManagedObject)
        {
            switchDomainJoinComplete(pVcsNodeLocalManagedObject->getDomainId ());

            delete (pVcsNodeLocalManagedObject);
        }
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));

}

void VcsFabricLocalObjectManager::phase3StartEventHandler( const ClusterPhase3StartEvent* &pEvent )
{
    ResourceId specificStatus =  getThisVcsNodeLocalManagedObjectSpecificStatus();

    if((VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_COMPLETED == specificStatus) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_COMPLETED == specificStatus) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED == specificStatus) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_STARTED == specificStatus))
    {

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        startTransaction();
        updateThisVcsNodeLocalManagedObject ();
        if ((VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_COMPLETED == specificStatus) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED == specificStatus))
        {   
            //This node is being added to the cluster and phase3 has started
            setThisVcsNodeLocalManagedObjectSpecificStatus( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE3_STARTED );
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::phase3StartEventHandler: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Add Phase3 Started ");
        }
        else //  VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_COMPLETED == specificStatus
        {
            //This node is being rejoined to the cluster and phase3 has started
            setThisVcsNodeLocalManagedObjectSpecificStatus( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE3_STARTED );
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::phase3StartEventHandler: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Rejoin Phase3 Started ");
        }
        status = commitTransaction ();
        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::phase3StartEventHandler: VcsNodeLocalManagedObject specific status has been committed to DB");
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::phase3StartEventHandler: VcsNodeLocalManagedObject specific status commit to DB Failed");
//            prismAssert (false, __FILE__, __LINE__);
        }
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));

}

void VcsFabricLocalObjectManager::phase3CompleteEventHandler( const ClusterPhase3CompleteEvent* &pEvent )
{   
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));

    ResourceId specificStatus =  getThisVcsNodeLocalManagedObjectSpecificStatus();

    if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE3_STARTED == specificStatus || VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE3_STARTED == specificStatus  )
    {   
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        startTransaction();
        updateThisVcsNodeLocalManagedObject ();
        if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE3_STARTED == specificStatus )
        {
            //This node got added to the cluster and phase3 has completed
            setThisVcsNodeLocalManagedObjectSpecificStatus( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY );

	    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::phase3CompleteEventHandler: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Connected To Cluster");
        }
        else  //  VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE3_STARTED == specificStatus
        {
            //This node rejoined to the cluster and phase3 has completed
            setThisVcsNodeLocalManagedObjectSpecificStatus( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY );
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::phase3CompleteEventHandler: VcsNodeLocalManagedObject specific status of this node has been set to --> Secondary Connected To Cluster");
        }
        status = commitTransaction ();
        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::phase3CompleteEventHandler: VcsNodeLocalManagedObject specific status has been committed to DB");
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {

		/* defect: 347440
    	 * No Assert required, secondaryFailover will complete the transaction
    	 */
            tracePrintf (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::phase3CompleteEventHandler: VcsNodeLocalManagedObject specific status commit to DB Failed, return status %s",FrameworkToolKit::localize(status).c_str());
        }
    }

    //Set the current principal priority in fabric
    setThisNodePrincipalPriorityInFabric ();

    if (LOCATION_SECONDARY == FrameworkToolKit::getThisLocationRole ())
    {
        m_fcPrincipalTrackingWorker->replaySCNQueueEvents();

        m_isNodeReadyForSlotScn = true;

        m_vcsNodeSlotManagementWorker->replaySlotScnQueueEvents();
    }

    // Fabric reformation should be done after addmessagecallbackreply. Need to handle add and rejoin together case.
/*    else if (LOCATION_PRIMARY == FrameworkToolKit::getThisLocationRole ())
    {
        triggerPrincipalSwitchoverBasedOnPrincipalPriority ();
    }
 */   
    // Check if there is any pending request (wating for all plugin services to come up) to segment switches.
/*    vector<VcsFabMessagingContext *> segMessagingContext = getSegmentMessagingContext ();
    UI32 numPendingSegments = segMessagingContext.size ();

    if (numPendingSegments > 0)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::phase3CompleteEventHandler: Phase3 complete, service the pending segment requests");
        for (UI32 i = 0; i < numPendingSegments; i++)    
        {
            sendSegmentSwitchMessageToFabric(segMessagingContext[i]);
        }
        clearSegmentMessagingContext ();
//            prismAssert (false, __FILE__, __LINE__);
    }*/
}

void VcsFabricLocalObjectManager::zoneChangedEventHandler (const VcsNodeFabosZoneChangedEvent *&pEvent)
{
	trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::zoneChangedEventHandler: Entering...");

	// Broadcast VCS_NODE_ZONE_CHANGED event
	VcsNodeZoneChangedEvent *pVcsNodeZoneChangedEvent = new VcsNodeZoneChangedEvent ();
    pVcsNodeZoneChangedEvent->setEventId(pEvent->getEventId());
	broadcast (pVcsNodeZoneChangedEvent);

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::zoneAbortedEventHandler (const VcsNodeFabosZoneAbortedEvent *&pEvent)
{
	trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::zoneAbortedEventHandler: Entering...");

	// Broadcast VCS_NODE_ZONE_ABORTED event
	VcsNodeZoneAbortedEvent *pVcsNodeZoneAbortedEvent = new VcsNodeZoneAbortedEvent ();
    pVcsNodeZoneAbortedEvent->setEventId(pEvent->getEventId());
	broadcast (pVcsNodeZoneAbortedEvent);

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}
void VcsFabricLocalObjectManager::switchPortDisableEventHandler (const VcsNodeFabosSwitchPortDisableEvent *&pEvent)
{
    VcsNodeSwitchPortDisableEvent  *pDUEvent = NULL;

        // SEG: temporarily change the logging level for this message so it will be dumped
    // trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::switchPortDisableEventHandler: Entering...");
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::switchPortDisableEventHandler: Entering...");

    pDUEvent = new VcsNodeSwitchPortDisableEvent ();
    pDUEvent->setPortNumber (pEvent->getPortNumber ());
    pDUEvent->setDisableReasonCode (pEvent->getDisableReasonCode ());
    broadcast (pDUEvent);

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::switchReadyToAcceptCommandsEventHandler (const VcsNodeFabosSwitchReadyToAcceptCommandsEvent *&pEvent)
{

    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::switchReadyToAcceptCommandsEventHandler: entering ....");
    if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::switchReadyToAcceptCommandsEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    VcsNodeSwitchReadyToAcceptCommandsEvent *pVcsNodeSwitchReadyToAcceptCommandsEvent = NULL;
    const VcsNodeSwitchReadyToAcceptCommandsEvent   *pVcsNodeSwitchReadyToAcceptCommandsEventCopy = NULL;
    ResourceId status = WAVE_MESSAGE_ERROR;
    
    pVcsNodeSwitchReadyToAcceptCommandsEvent = new VcsNodeSwitchReadyToAcceptCommandsEvent ();
    pVcsNodeSwitchReadyToAcceptCommandsEvent->setEventId(pEvent->getEventId());
    pVcsNodeSwitchReadyToAcceptCommandsEventCopy = pVcsNodeSwitchReadyToAcceptCommandsEvent;
    if (DcmToolKit::fcPrincipalEnabled ())
    {
        status = m_fcPrincipalTrackingWorker->switchReadyToAcceptCommandsEventHandler (pVcsNodeSwitchReadyToAcceptCommandsEventCopy);
    }
    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pVcsNodeSwitchReadyToAcceptCommandsEvent);
    }
    else
    {
        delete (pVcsNodeSwitchReadyToAcceptCommandsEvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::switchActivationCompleteEventHandler (const VcsNodeFabosSwitchActivationCompleteEvent *&pEvent)
{
	if (false == isNodeReadyForSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::switchActivationCompleteEventHandler: Postponing SCN Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    VcsNodeSwitchActivationCompleteEvent *pVcsNodeSwitchActivationCompleteEvent = NULL;
    const VcsNodeSwitchActivationCompleteEvent   *pVcsNodeSwitchActivationCompleteEventCopy = NULL;
    ResourceId status = WAVE_MESSAGE_ERROR;

    pVcsNodeSwitchActivationCompleteEvent = new VcsNodeSwitchActivationCompleteEvent ();
    pVcsNodeSwitchActivationCompleteEvent->setEventId(pEvent->getEventId());
    pVcsNodeSwitchActivationCompleteEventCopy = pVcsNodeSwitchActivationCompleteEvent;
    if (DcmToolKit::fcPrincipalEnabled ())
    {
        status = m_fcPrincipalTrackingWorker->switchActivationCompleteEventHandler (pVcsNodeSwitchActivationCompleteEventCopy);
    }
    if (WAVE_MESSAGE_SUCCESS == status)
    {
        broadcast (pVcsNodeSwitchActivationCompleteEvent);
    }
    else
    {
        delete (pVcsNodeSwitchActivationCompleteEvent);
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

WaveMessageStatus VcsFabricLocalObjectManager::synchronousMessageSend (ManagementInterfaceMessage *message)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    status = sendSynchronously (message, FrameworkToolKit::getThisLocationId ());

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (WAVE_MESSAGE_ERROR);
    }
    else
    {
        ResourceId completionStatus = message->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            return (WAVE_MESSAGE_ERROR);
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

VcsNodeLocalManagedObject* VcsFabricLocalObjectManager::getLocalManagedObjectFromMappedId (const UI32 domainId)
{
    WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;

    /* Get the object corresponding to this mapped ID which is now domainId*/
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (domainId, "domainId")); // mappedId has been removed from VcsNodeLocalManagedObject its now = domainId

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
           trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::getLocalManagedObjectFromMappedId : Cannot find Vcs Node for domainId =")+domainId);


        }
        else if (1 == numberOfResults)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
        }
        else
        {
           

            trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::getLocalManagedObjectFromMappedId: There can be only one local node for domainId =")+ domainId);
            prismAssert (false, __FILE__, __LINE__);
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::getLocalManagedObjectFromMappedId : System failure ");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalManagedObject);
}

VcsNodeLocalManagedObject* VcsFabricLocalObjectManager::getLocalManagedObjectFromLocationId (const LocationId locationId)
{
    WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getLocalManagedObjectFromLocationId: LocationId ") + locationId);

    /* Get the object corresponding to this location ID */
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::getLocalManagedObjectFromLocationId : Cannot find Vcs Node");
        }
        else if (1 == numberOfResults)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, " VcsFabricLocalObjectManager::getLocalManagedObjectFromLocationId: There can be only one local node.");
            prismAssert (false, __FILE__, __LINE__);
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::getLocalManagedObjectFromLocationId : System failure ");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalManagedObject);
}

VcsNodeLocalManagedObject* VcsFabricLocalObjectManager::getLocalManagedObjectFromWWN (const WorldWideName &wwn)
{
    WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::getLocalManagedObjectFromWWN: WorldWideName ") + wwn.toString ());

    /* Get the object corresponding to this location ID */
    synchronousQueryContext.addAndAttribute (new AttributeWorldWideName (wwn, "switchWWN"));

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::getLocalManagedObjectFromWWN : Cannot find Vcs Node");
        }
        else if (1 == numberOfResults)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, " VcsFabricLocalObjectManager::getLocalManagedObjectFromWWN: There can be only one local node.");
            prismAssert (false, __FILE__, __LINE__);
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::getLocalManagedObjectFromWWN : System failure ");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalManagedObject);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Principal Selection Section ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void VcsFabricLocalObjectManager::bootCompleteEventHandler (const WaveNs::BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    if ((LOCATION_SECONDARY_CLUSTER_PHASE_3 != FrameworkToolKit::getThisLocationRole ()) && (LOCATION_SECONDARY_REJOIN_PHASE_3 != FrameworkToolKit::getThisLocationRole ())) 
    {
    // If either config replay is complete [softreboot] or the daemon is CCMD,  then replay SCN here -> defect 367357   
        if ((m_isConfigReplayCompleted == true) || (CCM_ONLY == DcmToolKit::getDistributionMode ()))
        {
            m_fcPrincipalTrackingWorker->replaySCNQueueEvents();
        }

        m_isNodeReadyForSlotScn = true;

        m_vcsNodeSlotManagementWorker->replaySlotScnQueueEvents();
    }

    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));
}

/*
void VcsFabricLocalObjectManager::establishPrincipalTimerHandler (TimerHandle timerHandle, void * pContext)
{
        static UI32 numberOfTimeOuts = 0;
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::establishPrincipalTimerHandler : Timeout # ") + numberOfTimeOuts);
        numberOfTimeOuts++;
        ResourceId status = FRAMEWORK_ERROR;

    
    if (! DcmToolKit::fcPrincipalEnabled ())
    {
        if (! isNodeReadyForSCNHandling ())
        {
                trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::establishPrincipalTimerHandler: NodeReadyForSCNHandling not received yet so timer will be set");
            status = startTimer (m_VcsPrincipalSelectionWorker->m_timerHandle, wait_domain_lists_sync, reinterpret_cast<PrismTimerExpirationHandler> (&VcsFabricLocalObjectManager::establishPrincipalTimerHandler), NULL);
                return;
        }
        else
        {
            m_VcsPrincipalSelectionWorker->establishPrincipalTimer ();
        }
    }

}
*/

ResourceId VcsFabricLocalObjectManager::addDomainsDebugMessageHandler (UI32 argc, vector<string> argv)
{
    UI32 numberOfArguments = argv.size ();
        UI32 i;
        UI32 domainListSize = 0;
        UI32 x86domainId;
        string x86ipAddress;
        UI32 x86port;
    if (DcmToolKit::fcPrincipalEnabled ())
    {
        
        domainListSize = (getInstance ()->m_fcPrincipalTrackingWorker)->m_x86domainList.size ();
    }

    switch (numberOfArguments)
        {
                case 1:
                {
                        if (0 < domainListSize)
                        {
                                cout.width (9); cout << left << "DomainId";
                                cout.width (20); cout   << left << "ipAdderss";
                                cout.width (5); cout << left << "Port" << endl;
                        }
                        else
                        {
                                break;
                        }
                        for (i = 0; i < domainListSize; i++)
                        {
                            cout.width (9); cout << left << (getInstance ()->m_fcPrincipalTrackingWorker)->m_x86domainList[i];
                            cout.width (20); cout << left << (getInstance ()->m_fcPrincipalTrackingWorker)->m_x86ipAddresses[i];
                            cout.width (5); cout << left << (getInstance ()->m_fcPrincipalTrackingWorker)->m_x86ports[i] << endl;
                        }
                        break;
                }
                case 4:
                {
                    x86domainId = atoi (argv[1].c_str ());
                    x86ipAddress = argv[2].c_str ();
                    x86port = atoi (argv[3].c_str ());

                    if (DcmToolKit::fcPrincipalEnabled ())
                    {
                        (getInstance ()->m_fcPrincipalTrackingWorker)->addTox86DomainList (x86domainId);
                        (getInstance ()->m_fcPrincipalTrackingWorker)->addTox86IpAddressList (x86ipAddress);
                        (getInstance ()->m_fcPrincipalTrackingWorker)->addTox86PortList (x86port);
                        domainListSize = (getInstance ()->m_fcPrincipalTrackingWorker)->m_x86domainList.size ();
                    }

                    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::addDomainsDebugMessageHandler : Added ")+ " domainId = " + x86domainId + " ipAddress = " + x86ipAddress + " port = " +x86port + " size = "+ domainListSize);
                    break;
                }
                default:
                {
                        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setupDomainsDebugMessageHandler : USAGE :");
                        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setupDomainsDebugMessageHandler : adddomain <domain> <ipAddress> <portId>");
                        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setupDomainsDebugMessageHandler : adddomain");
                }
        }
    return (0);
}

void VcsFabricLocalObjectManager::parseVcsConfFile (bool &vcsMode, UI32 &vcsId)
{
    // WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::parseVcsConfFile: Entering ...... ");
    
    FILE * pFile;
    string vcsConfPath = DcmToolKit::getVcsConfPath ();
    vcsConfPath.append ("/vcs.conf");
    
    pFile = fopen (vcsConfPath.c_str(),"r"); 
    if (pFile == NULL)
    {
        // WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::parseVcsConfFile: Cannot open the vcs.conf for reading");        
        return;
    }

    char vcsModeFromFile [5];

    fgets (vcsModeFromFile, sizeof (vcsModeFromFile),  pFile);

    fclose (pFile);

    if (0 == strcmp("0000", vcsModeFromFile))
    {
        vcsMode = false;
        vcsId = 0;
    }
    else
    {
        vcsMode = true;
        vcsId = atoi (vcsModeFromFile);
    }
}

bool VcsFabricLocalObjectManager::parseClusterConfFile ()
{
    // WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::parseClusterConfFile: Entering ...... ");

    int clusterMode = 0;

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))    
    {
        FILE * pFile;
        string clusterConfPath = DcmToolKit::getVcsConfPath ();
        clusterConfPath.append ("/cluster.conf");
    
        m_ClusterConfFileMutex.lock ();
        pFile = fopen (clusterConfPath.c_str(),"r"); 
        if (pFile == NULL)
        {
            m_ClusterConfFileMutex.unlock ();
            // WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::parseClusterConfFile: Cannot open the cluster.conf for reading");        
            return (false);
        }

        clusterMode = fgetc (pFile);

        // WaveNs::tracePrintf (TRACE_LEVEL_INFO, false, false, "VcsFabricLocalObjectManager::parseClusterConfFile: MODE %c \n", clusterMode);

        fclose (pFile);
        m_ClusterConfFileMutex.unlock ();
    }
    else
    {
        clusterMode = 1;
    }

    if ('0' == clusterMode)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

ResourceId VcsFabricLocalObjectManager::setClusterConfFile (bool clusterMode)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setClusterConfFile: Entering ...... ");

    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::setClusterConfFile: cluster mode ") + clusterMode);

    FILE * pFile;
    string clusterConfPath = DcmToolKit::getVcsConfPath ();
    clusterConfPath.append ("/cluster.conf");

    m_ClusterConfFileMutex.lock ();
    pFile = fopen (clusterConfPath.c_str(),"w");
    if (pFile == NULL)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::setClusterConfFile: Cannot open the vcs.conf for writing");
        m_ClusterConfFileMutex.unlock ();
        return (WAVE_MESSAGE_ERROR);
    }

    if (clusterMode == true)
    {
        fputs ("1",pFile);
    }
    else
    {
        fputs ("0",pFile);
    }

    fclose (pFile);
    m_ClusterConfFileMutex.unlock ();

    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::setThisVcsNodeClusterType (const bool &vcsMode, const bool &clusterMode)
{
    m_thisVcsNodeClusterMutex.lock ();

    if (false == vcsMode)
    {
        m_thisVcsNodeClusterType = VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE; 
    }
    else
    {
        if (false == clusterMode)
        {
            m_thisVcsNodeClusterType = VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC;
        }
        else
        {
            m_thisVcsNodeClusterType = VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT;
        }
    }

    m_thisVcsNodeClusterMutex.unlock ();
}

ResourceId VcsFabricLocalObjectManager::getThisVcsNodeClusterType ()
{
    ResourceId thisVcsNodeClusterType;

    m_thisVcsNodeClusterMutex.lock ();
    thisVcsNodeClusterType = m_thisVcsNodeClusterType;
    m_thisVcsNodeClusterMutex.unlock ();

    return (thisVcsNodeClusterType);
}

void VcsFabricLocalObjectManager::setThisVcsNodeSWBDModel (const int &swbdModel)
{
    m_thisVcsNodeSWBDModelMutex.lock ();
    m_thisVcsNodeSWBDModel = swbdModel;
    m_thisVcsNodeSWBDModelMutex.unlock ();
}

int VcsFabricLocalObjectManager::getThisVcsNodeSWBDModel ()
{
    int thisVcsNodeSWBDModel;

    m_thisVcsNodeSWBDModelMutex.lock ();
    thisVcsNodeSWBDModel = m_thisVcsNodeSWBDModel;
    m_thisVcsNodeSWBDModelMutex.unlock ();

    return (thisVcsNodeSWBDModel);
}

void VcsFabricLocalObjectManager::setThisVcsNodeSwitchFwVersion (const string &fwVersion)
{
    m_thisVcsNodeFwVersionMutex.lock ();
    m_thisVcsNodeFwVersion = fwVersion;
    m_thisVcsNodeFwVersionMutex.unlock ();
}

string VcsFabricLocalObjectManager::getThisVcsNodeSwitchFwVersion ()
{
    string fwVersion = "";

    m_thisVcsNodeFwVersionMutex.lock ();
    fwVersion = m_thisVcsNodeFwVersion;
    m_thisVcsNodeFwVersionMutex.unlock ();

    return (fwVersion);
}

void VcsFabricLocalObjectManager::setThisVcsNodeSwmodelName (const string &thisVcsNodeSwmodelName)
{
    m_thisVcsNodeSwmodelNameMutex.lock ();
    m_thisVcsNodeSwmodelName = thisVcsNodeSwmodelName;
    m_thisVcsNodeSwmodelNameMutex.unlock ();
}

string VcsFabricLocalObjectManager::getThisVcsNodeSwmodelName ()
{
    string thisVcsNodeSwmodelName;

    m_thisVcsNodeSwmodelNameMutex.lock ();
    thisVcsNodeSwmodelName = m_thisVcsNodeSwmodelName;
    m_thisVcsNodeSwmodelNameMutex.unlock ();

    return (thisVcsNodeSwmodelName);
}

ResourceId VcsFabricLocalObjectManager::getLocalNodeSwitchname (string &switchname)
{
    // get switchname using gethostname () call.
    // int gethostname(char *name, size_t namelen);
    //  if namelen is an insufficient length to hold the host name,
    //  then the returned name is truncated
    //  and it is unspecified whether the returned name is null-terminated.

    ResourceId  returnStatus            = WAVE_MESSAGE_ERROR;
    char        switchNameBuffer[256]   = {0};
    int         returnValue             = gethostname (switchNameBuffer, 255);

    // initialize switchname to empty string.
    switchname              = "";

    if (0 == returnValue)
    {
        switchNameBuffer[255] = '\0';
        
        // switchname is empty string till now.
        switchname.append (switchNameBuffer);

        returnStatus = WAVE_MESSAGE_SUCCESS;
    }

    return returnStatus;
}

void VcsFabricLocalObjectManager::setThisVcsNodeBootReason (const WaveBootReason &thisVcsNodeBootReason)
{
    m_thisVcsNodeBootReason = thisVcsNodeBootReason;
}

WaveBootReason VcsFabricLocalObjectManager::getThisVcsNodeBootReason ()
{
    return (m_thisVcsNodeBootReason);
}

void VcsFabricLocalObjectManager::vcsIsLocalNodeInClusterMessageHandler (VcsNodeIsLocalNodeInClusterMessage  *pVcsNodeIsLocalNodeInClusterMessage)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::vcsIsLocalNodeInClusterMessageHandler : entering..."));

    if ((LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ()) && (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS != VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()))
    {
        pVcsNodeIsLocalNodeInClusterMessage->setIsLocalNodeInCluster (false);
    }
    else
    {
        pVcsNodeIsLocalNodeInClusterMessage->setIsLocalNodeInCluster (true);
    }

    pVcsNodeIsLocalNodeInClusterMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeIsLocalNodeInClusterMessage);
}

void VcsFabricLocalObjectManager::vcsIsLocalNodeInClusterStaticMessageHandler (VcsNodeIsLocalNodeInClusterMessage  *pVcsNodeIsLocalNodeInClusterMessage)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::vcsIsLocalNodeInClusterStaticMessageHandler : entering..."));

    if ((LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ()) && (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS != VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()))
    {
        UI64 configurationTransactionNumber = DcmToolKit::getConfigurationTransactionId ();

        if (0 != configurationTransactionNumber)
        {
            // Special Case Handling:  Single-node FC to MC transition with configuration preservation
            // For this case, we want to report to fabricd that this single transitioned node is in
            // a cluster so that it will remain the FC principal when other nodes join to create the 
            // cluster.  We want to retain this single transitioned node as FC principal because we
            // have replayed the preserved configuration on this single node and want this preserved 
            // configuration to be pushed to all joining members when the cluster is created for the
            // first time.

            pVcsNodeIsLocalNodeInClusterMessage->setIsLocalNodeInCluster (true);
        }
        else
        {
            pVcsNodeIsLocalNodeInClusterMessage->setIsLocalNodeInCluster (false);
        }
    }
    else
    {
        pVcsNodeIsLocalNodeInClusterMessage->setIsLocalNodeInCluster (true);
    }

    pVcsNodeIsLocalNodeInClusterMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::vcsIsNodeInClusterMessageHandler (VcsNodeIsNodeInClusterMessage  *pVcsNodeIsNodeInClusterMessage)
{
    trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::vcsIsNodeInClusterMessageHandler : entering..."));

    bool isLocalNode        = false;
    UI32 domainId           = pVcsNodeIsNodeInClusterMessage->getRBridgeId();

    if (0 == domainId || domainId == getThisVcsNodeLocalManagedObjectDomainId ())
    {
        isLocalNode = true;
        trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::vcsIsNodeInClusterMessageHandler : This is a check for local node. rbridge-id:") + domainId);
    }

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT != getThisVcsNodeClusterType ())
    {
        pVcsNodeIsNodeInClusterMessage->setIsNodeInCluster (false);                  
        if (!isLocalNode)
        {
            tracePrintf (TRACE_LEVEL_INFO, true, false, "VcsFabricLocalObjectManager::vcsIsNodeInClusterMessageHandler : isNodeInCluster for rbridge-id=%d is set to false as this node is not in cluster.", domainId);
        }
    }
    else
    {
        bool incluster = false;
        
        if (isLocalNode)
        {
            VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject();
            if (NULL == pVcsClusterManagedObject)
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsIsThisNodeInClusterMessageHandler : No Cluster MO created in database.");
            }
            else
            {
                incluster = true;
                delete pVcsClusterManagedObject;
            }
        }
        else
        {
            VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getLocalManagedObjectFromMappedId (domainId);
            if (NULL == pVcsNodeLocalManagedObject)    
            {
                trace (TRACE_LEVEL_WARN, string ("VcsFabricLocalObjectManager::vcsIsNodeInClusterMessageHandler : switch not in cluster domainId = ")+ domainId);
            }       
            else
            {
                incluster = true;
                trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::vcsIsNodeInClusterMessageHandler : switch is in the cluster domainId = ")+ domainId);
                delete pVcsNodeLocalManagedObject;
            }

        }
        pVcsNodeIsNodeInClusterMessage->setIsNodeInCluster (incluster);

    }
    
    pVcsNodeIsNodeInClusterMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    reply(pVcsNodeIsNodeInClusterMessage);
}

VcsClusterManagedObject *VcsFabricLocalObjectManager::getVcsClusterManagedObject ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::getVcsClusterManagedObject : entering...");
    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;

    vector<WaveManagedObject *> *pResults = querySynchronously (VcsClusterManagedObject::getClassName());
    prismAssert (1 >= pResults->size (), __FILE__, __LINE__);

    if (1 == pResults->size ())
    {
        pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *> ((*pResults)[0]);
        delete pResults;
        return pVcsClusterManagedObject;
    }
    // pResults->size () must be 0
    delete pResults;
    return NULL;
}

ResourceId VcsFabricLocalObjectManager::removeDomainsDebugMessageHandler (UI32 argc, vector<string> argv)
{
    UI32 numberOfArguments = argv.size ();
    UI32 domainListSize = 0;
    UI32 x86domainId;


    if (DcmToolKit::fcPrincipalEnabled())
    {
        if (numberOfArguments == 2) 
        {
            x86domainId = atoi (argv[1].c_str ());
            (getInstance ()->m_fcPrincipalTrackingWorker)->removeFromx86DomainList (x86domainId);
            domainListSize = (getInstance ()->m_fcPrincipalTrackingWorker)->m_x86domainList.size ();
            WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::removeDomainsDebugMessageHandler : Remove ")+ " domainId = " + x86domainId + " size = "+ domainListSize);
        } 
        else 
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::removeDomainsDebugMessageHandler : USAGE :");
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::removeDomainsDebugMessageHandler : removedomains <domain>");
        }
    }

    return (0);
}

void VcsFabricLocalObjectManager::serviceReadyEventHandler (const VcsNodeFabosServiceReadyEvent *&pEvent)
{
    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::serviceReadyEventHandler: Entering... event received status:") + m_serviceReadyEventReceived);

//	Need to add popping out of Q and Q event replay 
    
    ResourceId recoveryType = pEvent->getRecoveryType();
    handleServiceReadyEvent (recoveryType);
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::NsmServiceReadyEventHandler (const NsmServiceReadyEvent *&pEvent)
{
    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::NsmServiceReadyEventHandler: Entering... event received status:") + m_serviceReadyEventReceived);
    handleServiceReadyEvent (WAVE_HA_COLD_RECOVERY);
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::handleServiceReadyEvent (const ResourceId &recoveryType)
{
    m_serviceReadyEventReceived = true;
    m_systemRecoveryType = recoveryType;

    if (DCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        //Config replay complete in dcmd indicates that the switch is ready to accept all the command Send the SCN for CCMd to act on.
        switchActivationComplete();
    }

    if (true == getIsWyserEaPhase2Complete ())
    {
        // Trigger Postboot 
        triggerPostBoot (POSTBOOT_EVENT_SERVICE_READY, 0, recoveryType); 
    }

    if ((true == getIsWyserEaPhase2Complete ()) && (true == m_linecardReadyEventReceived))
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager: service ready event arrived ");
        // linecard_ready arrived before service_ready
        UI32 i;

        m_linecardReadySlotIdBitMapMutex.lock();
        for (i = 0; i < MAX_LINECARD_NUM; i++)
        {
            if (m_linecardReadySlotIdBitMap[i])
            {
                 triggerPostBoot (POSTBOOT_EVENT_LINECARD_READY, i, m_linecardRecoveryType[i]);
            }
        }
        m_linecardReadySlotIdBitMapMutex.unlock();
    }
}

void VcsFabricLocalObjectManager::systemReadyEventHandler (const VcsNodeFabosSystemReadyEvent *&pEvent)
{
    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::systemReadyEventHandler: Entering... event received status:") + m_systemReadyEventReceived);

//  Need to add popping out of Q and Q event replay
    ResourceId recoveryType = pEvent->getRecoveryType();
    handleSystemReadyEvent (recoveryType);
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::NsmChassisReadyEventHandler (const NsmChassisReadyEvent *&pEvent)
{
    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::NsmChassisReadyEventHandler: Entering... event received status:") + m_systemReadyEventReceived);
    handleSystemReadyEvent (WAVE_HA_COLD_RECOVERY);
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::handleSystemReadyEvent (const ResourceId &recoveryType)
{
    m_systemReadyEventReceived = true;

    m_linecardReadySlotIdBitMapMutex.lock();
    if ((true == getIsWyserEaPhase2Complete ()) && (isLinecardPostBootComplete ()))
    {
        // Trigger Config Replay
        triggerPostBoot (POSTBOOT_EVENT_SYSTEM_READY, 0, recoveryType);
    }
    m_linecardReadySlotIdBitMapMutex.unlock();
}

void VcsFabricLocalObjectManager::linecardReadyEventHandler (const VcsNodeFabosLinecardReadyEvent *&pEvent)
{
    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::linecardReadyEventHandler: Entering... event received status:") + m_linecardReadyEventReceived);

    if (false == VcsFabricLocalObjectManager::getInstance ()->isNodeReadyForSlotSCNHandling ())
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::slotOnlineEventHandler: Postponing slot_ready Handling...");
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    ResourceId status = WAVE_MESSAGE_ERROR;
    UI32 eventId = pEvent->getEventId();

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPSlotScnQueueObject (eventId);
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_SUCCESS, string ("VcsFabricLocalObjectManager::linecardReadyEventHandler: slot ready event not in queue, may have been handled already eventId = ")+eventId);

        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        return;
    }

    ResourceId recoveryType = pEvent->getRecoveryType();
    UI32 slotId = pEvent->getSlotId();
    handleLinecardReadyEvent (slotId, recoveryType);
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::NsmLinecardReadyEventHandler (const NsmLinecardReadyEvent *&pEvent)
{
    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::NsmLinecardReadyEventHandler: Entering... event received status:") + m_linecardReadyEventReceived);
    UI32 slotId = pEvent->getSlotId();
    handleLinecardReadyEvent (slotId, WAVE_HA_COLD_RECOVERY);
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsFabricLocalObjectManager::handleLinecardReadyEvent (const UI32 &slotId, const ResourceId &recoveryType)
{
    m_linecardReadyEventReceived = true;
    m_linecardReadySlotIdBitMapMutex.lock();
    m_linecardReadySlotIdBitMap.set (slotId);
    m_linecardReadySlotIdBitMapMutex.unlock();
	m_linecardRecoveryType[slotId] = recoveryType;

    if ((true == getIsWyserEaPhase2Complete ()) && 
        (m_serviceReadyEventReceived == true))
    {
        // Trigger Postboot and Config Replay
        triggerPostBoot (POSTBOOT_EVENT_LINECARD_READY, slotId, recoveryType);
    }
}

void VcsFabricLocalObjectManager::NsmLinecardOnlineEventHandler (const NsmLinecardOnlineEvent *&pEvent)
{
    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::NsmLinecardOnlineEventHandler: Entering, slotId :") + pEvent->getSlotId());

    UI32 slotNumber = pEvent->getSlotId ();
    UI32 objectType = 0;
    UI32 hwType = SYS_BLADE_AP;
    UI32 hwId = 0;
    UI32 slotState = SLOT_ON;
    UI32 reason = 0;
    UI32 version = 0;

    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleSlotEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version);

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

bool VcsFabricLocalObjectManager::isLinecardPostBootComplete ()
{
	int nBits = (int) m_linecardReadySlotIdBitMap.getNumberOfBits ();
	for (int i = 0; i < nBits; i++)
	{
	// check if linecardReadyPostBootCompletBitMap bit for slot i is set if and only if the bit is set in linecardReadySlotIdBitMap. We specifically don't check
	// for the equality of the two since a linecard could go offline after postboot for the line card has been started. In this case its possible for the postBootComplete
	// bit to be set when the linecardReadySlotId bit is clear due to the slot offline event.
		if (m_linecardReadySlotIdBitMap [i])
		{
		   if (! m_linecardReadyPostBootCompleteBitMap [i])
	   	   {
	   		  return false;
	   	   }
		}
	}

	return true;
}

void VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback (FrameworkStatus frameworkStatus, PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pPrismPostPersistenceBootMessage != NULL, __FILE__, __LINE__);
        UI32 slotId = pPrismPostPersistenceBootMessage->getParameter ();
        UI32 eventId = pPrismPostPersistenceBootMessage->getEventId ();
        ResourceId completionStatus = pPrismPostPersistenceBootMessage->getCompletionStatus ();
        delete pPrismPostPersistenceBootMessage;
        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            switch (eventId)
            {
                case POSTBOOT_EVENT_SERVICE_READY:
                    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback service ready postboot processing completed "));
                    return;
                case POSTBOOT_EVENT_SYSTEM_READY:
                    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback chassis ready postboot processing completed "));
                    m_systemReadyPostbootProcessingComplete = true;
                    return;
                case POSTBOOT_EVENT_LINECARD_READY:
                    m_linecardReadyPostBootCompleteBitMap.set (slotId);
                    // if chassis ready event has been received and postboot processing for all the linecards is complete then trigger post boot for chassis ready
                    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback linecard ready postboot processing completed for slot ")+slotId);
                    m_linecardReadySlotIdBitMapMutex.lock();
                    if ((m_systemReadyEventReceived) && (!m_systemReadyPostbootProcessingComplete) &&
                        (isLinecardPostBootComplete ()))
                    {
                        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback: triggering chassis ready post boot "));
                        triggerPostBoot (POSTBOOT_EVENT_SYSTEM_READY, 0, m_systemRecoveryType);
                    }
                    m_linecardReadySlotIdBitMapMutex.unlock();

                    break;
                default:
                    trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback callback received for unknown event: ")+eventId);
                    prismAssert (false, __FILE__, __LINE__);
            }
        
        }
        else 
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback linecard postboot processing failed for slot ")+slotId);
        }
        
    } 
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback frameworkStatus error: ")+FrameworkToolKit::localize (frameworkStatus));
        prismAssert (false, __FILE__, __LINE__);
    }
}

void VcsFabricLocalObjectManager::triggerPostBoot (UI32 eventId, UI32 parameter, ResourceId recoveryType)
{
    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        WaveMessageStatus   status      = WAVE_MESSAGE_SUCCESS;

        ///////////////////////////////////////////////////////////////////////
        // Trigger PostBoot processing and config replay  
        ///////////////////////////////////////////////////////////////////////
        PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage = new PrismPostPersistenceBootMessage (eventId, parameter, recoveryType);

        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::triggerPostBootExecution : Triggering POSTBOOT processing"));
        if (! (DCM_CCM == DcmToolKit::getDistributionMode ()  && POSTBOOT_EVENT_SYSTEM_READY == eventId) )
        {
            pPrismPostPersistenceBootMessage->setIsBannerOutputToClientsRequired (false);
        }

        status = send (pPrismPostPersistenceBootMessage,
            reinterpret_cast<PrismMessageResponseHandler> (&VcsFabricLocalObjectManager::prismPostPersistenceBootMessageCallback),
            NULL);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::triggerPostBootExecution: Error sending postboot message.");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }

    }
    else if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        //m_slotReadyEventReceived = true;
        m_isConfigReplayCompleted = true;

        // Note : in CCM_ONLY mode, postboot-configReplay is not done.
        // So, following message is sent to set db schema valid.

        bool   isStartupValid = true;

        PersistenceLocalObjectManagerSavePrismConfigurationMessage *pPersistenceLocalObjectManagerSavePrismConfigurationMessage = new PersistenceLocalObjectManagerSavePrismConfigurationMessage (isStartupValid);

        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::triggerPostBoot : sending message to set database schema valid."));

        WaveMessageStatus status = sendOneWay (pPersistenceLocalObjectManagerSavePrismConfigurationMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::triggerPostBoot: Error sending message to set database schema valid.");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void VcsFabricLocalObjectManager::triggerPostBootMessageHandler (VcsNodeTriggerPostBootMessage *pVcsNodeTriggerPostBootMessage)
{
	UI32 i = 0;

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::triggerPostBootMessageHandler : Entering... ");
    pVcsNodeTriggerPostBootMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeTriggerPostBootMessage);

    /* Check if WyserEaPhase2Complete is set to TRUE. We will perform this
     * check only when checkNotifyPhase2CompletedBool is TRUE, to make sure, 
     * we are handling the restart of wyserea. During restart, there is no
     * need to perform PostBoot operation. PostBoot operation needs to be
     * done only when wyserea process gets started for the first time.
     */
    if ((true == pVcsNodeTriggerPostBootMessage->getCheckNotifyPhase2CompletedBool()) && (true == getIsWyserEaPhase2Complete())) {
       trace (TRACE_LEVEL_INFO, "WyserEaPhase2Complete is already set to TRUE, Handling wyserea restart");
       // Do nothing, simply return
       return;
    }

    /* Handling the wyserea process start for the first time. Setting the
     * WyserEaPhase2Complete to true and triggering PostBoot activity
     */
    setIsWyserEaPhase2Complete (true);

    // Check if DB Conversion was successful or not and generate raslog 
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::triggerPostBootMessageHandler : DB CONVERSION Status : " + FrameworkToolKit::localize (dbConversionStatus));

    if (DATABASE_SCHEMA_CONVERSION_SUCCEEDED == dbConversionStatus)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::triggerPostBootMessageHandler1 : DB CONVERSION Status is SUCCESS");
	    raslog_ext("VcsFabLocalMO",__FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_4001);
        if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            FrameworkToolKit::consolePrint ("DCM : Database schema conversion successful.");
        }
        else if (CCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            FrameworkToolKit::consolePrint ("CCM : Database schema conversion successful.");
        }
    }
    else if (DATABASE_SCHEMA_CONVERSION_FAILED == dbConversionStatus)
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::triggerPostBootMessageHandler1 : DB CONVERSION Status is FAILURE");
	    raslog_ext("VcsFabLocalMO",__FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_4002);
        if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            FrameworkToolKit::consolePrint ("DCM : Database schema conversion failed.");
        }
        else if (CCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            FrameworkToolKit::consolePrint ("CCM : Database schema conversion failed.");
        }
    }
    
    // Check if last reboot was because of DB corruption auto-recovery and generate raslog for same.
    const string dbCorruptIdentifierFile = DatabaseObjectManager::getDBCorruptIdentifierFileName ();

    ifstream dbHadCorrupted(dbCorruptIdentifierFile.c_str());
    if(dbHadCorrupted.is_open())
    {
        dbHadCorrupted.close();
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::triggerPostBootMessageHandler : Auto Recovey Complete for DB Corruption.");
        raslog_ext("VcsFabLocalMO",__FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_3052);

        if (unlink(dbCorruptIdentifierFile.c_str()) < 0)
        {
            trace (TRACE_LEVEL_WARN, "VcsFabricLocalObjectManager::triggerPostBootMessageHandler : failed to delete " + dbCorruptIdentifierFile);
        }
    }
    
    // Do Postboot Trigger

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        if (true == m_serviceReadyEventReceived)
        {
            triggerPostBoot (POSTBOOT_EVENT_SERVICE_READY, 0, m_systemRecoveryType);
        }

        if (true == m_linecardReadyEventReceived)
        {
            m_linecardReadySlotIdBitMapMutex.lock();
            for (i = 0; i < MAX_LINECARD_NUM; i++)
            {
                if (m_linecardReadySlotIdBitMap[i])
                {
                     triggerPostBoot (POSTBOOT_EVENT_LINECARD_READY, i, m_linecardRecoveryType[i]);
                }
            }
            m_linecardReadySlotIdBitMapMutex.unlock();
        }

        m_linecardReadySlotIdBitMapMutex.lock();
    	if (true == m_systemReadyEventReceived &&
            (isLinecardPostBootComplete ()))
            // chassis ready event received and postboot processing not done, and linecard postboot processing completed for all line cards 
        {
            triggerPostBoot (POSTBOOT_EVENT_SYSTEM_READY, 0, m_systemRecoveryType);
        }
        m_linecardReadySlotIdBitMapMutex.unlock();
    }
    else if ( CCM_ONLY == DcmToolKit::getDistributionMode () )
    {
        // in case of CCM_ONLY mode, call triggerPostBoot function. triggerPostBoot () will just make startup-schema valid.

        triggerPostBoot (POSTBOOT_EVENT_SERVICE_READY, 0, m_systemRecoveryType);
    }
}


ResourceId VcsFabricLocalObjectManager::setSwitchDeleteAfterFailover (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setSwitchDeleteAfterFailover: Entering...");
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setSwitchDeleteAfterFailover: Entering...");

    bool clusterExists = false;
    if (DcmToolKit::fcPrincipalEnabled())
    {
        clusterExists = m_fcPrincipalTrackingWorker->getVcsClusterId (clusterExists);
    }

    if (clusterExists && getThisVcsNodeLocalManagedObjectIsPrincipal () && m_noVcsEnablePrincipalControlFailoverFlag)
    {
        setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (true);
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setSwitchDeleteAfterFailover : switchDeleteAfterFailover flag set to true in preparation for control failover");
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setSwitchDeleteAfterFailover : switchDeleteAfterFailover flag not changed");
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsFabricLocalObjectManager::disconnectPrincipalFromAllLocationStep (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::disconnectPrincipalFromAllLocationStep: Entering...");

    if ( false ==  pVcsNodeLocalMessagingContext->getPrincipalControlFailoverFlag () )
    {
        trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::disconnectPrincipalFromAllLocationStep : No disconnect needed on a secondary");
        return WAVE_MESSAGE_SUCCESS;
    }

    PrismFrameworkObjectManager::disconnectFromAllKnownLocations ();
    
    return WAVE_MESSAGE_SUCCESS;
}

void VcsFabricLocalObjectManager::getClusterTypeMessageHandler (VcsNodeGetClusterTypeMessage *pVcsNodeGetClusterTypeMessage)
{
    pVcsNodeGetClusterTypeMessage->setClusterType (VcsFabricLocalObjectManager::getThisVcsNodeClusterType ());

    pVcsNodeGetClusterTypeMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    reply(pVcsNodeGetClusterTypeMessage);
}

void VcsFabricLocalObjectManager::getClusterTypeStaticMessageHandler (VcsNodeGetClusterTypeMessage *pVcsNodeGetClusterTypeMessage)
{
    pVcsNodeGetClusterTypeMessage->setClusterType (DcmToolKit::getThisVcsNodeClusterType ());
 
    pVcsNodeGetClusterTypeMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::vcsSegmentSwitchMessageHandler (VcsNodeSegmentSwitchMessage *pVcsNodeSegmentSwitchMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabricLocalObjectManager::sendSegmentSwitchMessageToFabric),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabricLocalObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabricLocalObjectManager::prismLinearSequencerFailedStep)
    };
    
    VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeSegmentSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pVcsFabMessagingContext->start ();
}

void VcsFabricLocalObjectManager::sendSegmentSwitchMessageToFabric (VcsFabMessagingContext *pVcsFabMessagingContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::sendSegmentSwitchMessageToFabric: Entering...");

    // To send segment message to the fabric plugin, service should be enabled.
    // Plugins come up during phase3 so this message should be sent only
    // after phase3 completes, node status will indicate the completion of phase3.
    // Check is mainly done for the secondary node. For primary, message send
    // should be allowed.
    ResourceId specificStatus =  getThisVcsNodeLocalManagedObjectSpecificStatus();
    if ((VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY != specificStatus) &&
        (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY != specificStatus))
    {
        //Store the context and perform operation once phase3 completes.
        addToSegmentMessagingContext (pVcsFabMessagingContext);
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::sendSegmentSwitchMessageToFabric : Phase3 is not complete yet, storing the context."));
        return;
    }

    //ResourceId status;
    VcsNodeSegmentSwitchMessage *pVcsNodeSegmentSwitchMessage = reinterpret_cast<VcsNodeSegmentSwitchMessage *> (pVcsFabMessagingContext->getPPrismMessage ());
    vector<WorldWideName> domainWwn = pVcsNodeSegmentSwitchMessage->getDomainWwn ();
    vector<UI32> segmentReason = pVcsNodeSegmentSwitchMessage->getSegmentReason ();
    UI32 numDomainWwns = domainWwn.size ();
    trace (TRACE_LEVEL_DEVEL, string("VcsFabricLocalObjectManager::sendSegmentSwitchMessageToFabric : numDomainWwns = ") + numDomainWwns);

    ++(*pVcsFabMessagingContext);

#if 0
    for(UI32 i = 0; i < numDomainWwns; ++i)
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::sendSegmentSwitchMessageToFabric : Sending message to fabric to segment switch with WWN = ") + domainWwn[i].toString() + " and reason = " + segmentReason[i]);
        FabricServiceDisableIslsToNeighborMessage *pFabricServiceDisableIslsToNeighborMessage = new FabricServiceDisableIslsToNeighborMessage (domainWwn[i], segmentReason[i]);

        status = send (pFabricServiceDisableIslsToNeighborMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabricLocalObjectManager::segmentSwitchMessageToFabricCallback), pVcsFabMessagingContext);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::sendSegmentSwitchMessageToFabric : Failed to send FabricServiceDisableIslsToNeighborMessage : Status")  + FrameworkToolKit::localize (status));
            pVcsFabMessagingContext->incrementNumberOfFailures ();
            delete pFabricServiceDisableIslsToNeighborMessage;
        }
        else
        {
             ++(*pVcsFabMessagingContext);
        }

    }
#endif
    --(*pVcsFabMessagingContext);
    
    pVcsFabMessagingContext->executeNextStep (((pVcsFabMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}
    
   
#if 0
void VcsFabricLocalObjectManager::segmentSwitchMessageToFabricCallback (FrameworkStatus frameworkStatus, FabricServiceDisableIslsToNeighborMessage *pFabricServiceDisableIslsToNeighborMessage, VcsFabMessagingContext *pVcsFabMessagingContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::segmentSwitchMessageToFabricCallback : Starting ...");

    --(*pVcsFabMessagingContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert ((NULL != pFabricServiceDisableIslsToNeighborMessage), __FILE__, __LINE__);
        ResourceId completionStatus = pFabricServiceDisableIslsToNeighborMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::segmentSwitchMessageToFabricCallback : Failed to segment switch with wwn : ") + (pFabricServiceDisableIslsToNeighborMessage->getSwitch_wwn().toString()).c_str() + string (" Completion status : ") + FrameworkToolKit::localize (completionStatus));
            pVcsFabMessagingContext->incrementNumberOfFailures ();
        }
        else 
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::segmentSwitchMessageToFabricCallback : Successfully segmented switch with wwn : ") + (pFabricServiceDisableIslsToNeighborMessage->getSwitch_wwn().toString()).c_str());
        }
    }
    else
    {
        if (NULL != pFabricServiceDisableIslsToNeighborMessage)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::segmentSwitchMessageToFabricCallback : Failed to send message to fabric to segment switch with wwn : ") + (pFabricServiceDisableIslsToNeighborMessage->getSwitch_wwn().toString()).c_str() + string (" Framework status : ") + FrameworkToolKit::localize (frameworkStatus));
            pVcsFabMessagingContext->incrementNumberOfFailures ();
        }
    }

    if (NULL != pFabricServiceDisableIslsToNeighborMessage)
    {
        delete pFabricServiceDisableIslsToNeighborMessage;
    }

    pVcsFabMessagingContext->executeNextStep (((pVcsFabMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}
#endif

void VcsFabricLocalObjectManager::addToSegmentMessagingContext (VcsFabMessagingContext *pVcsFabMessagingContext)
{
    m_segmentMessagingContext.push_back (pVcsFabMessagingContext);
}

vector<VcsFabMessagingContext *> VcsFabricLocalObjectManager::getSegmentMessagingContext () const
{
    return (m_segmentMessagingContext);
}

void VcsFabricLocalObjectManager::clearSegmentMessagingContext ()
{
    m_segmentMessagingContext.clear ();
}

void  VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObject(VcsNodeLocalManagedObject*  pVcsNodeLocalManagedObject)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();

   m_pVcsNodeLocalManagedObject = pVcsNodeLocalManagedObject;

   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

void  VcsFabricLocalObjectManager:: setThisVcsNodeLocalManagedObjectVcsMode (UI32 vcsMode)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();

   m_pVcsNodeLocalManagedObject->setVcsMode(vcsMode);

   m_thisVcsNodeLocalManagedObjectMutex.unlock();
  
}

void VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (bool switchDeleteAfterFailoverFlag)
{
    m_thisVcsNodeLocalManagedObjectMutex.lock ();

    m_pVcsNodeLocalManagedObject->setSwitchDeleteAfterFailoverFlag(switchDeleteAfterFailoverFlag);
    
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
}

void  VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectVcsId (UI32 vcsId)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();

   m_pVcsNodeLocalManagedObject->setVcsId (vcsId);

   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}
void  VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectStatus       (ResourceId genericStatus, ResourceId specificStatus)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();

   m_pVcsNodeLocalManagedObject->setGenericStatus(genericStatus);
   m_pVcsNodeLocalManagedObject->setSpecificStatus(specificStatus);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}
void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectDomainId     (UI32 domainId)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setDomainId(domainId);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}
void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectLocationId   (LocationId locationId)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setLocationId (locationId);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}


void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSerialNumber (const string & serialNumber)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setSerialNumber (serialNumber);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSwitchMac    (MacAddress switchMac)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setSwitchMac(switchMac);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}
void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectfcfMac       (MacAddress  fcfMac)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setFcfMac(fcfMac);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}
void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectInVcs        (bool inVcs)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setInVcs(inVcs);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}
void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectIsPrincipal  (bool isPrincipal)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setIsPrincipal(isPrincipal);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectPublicIpAddress    (const vector<string> & ipAddress)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setPublicIpAddress(ipAddress);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}
void  VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectGenericStatus (ResourceId genericStatus)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setGenericStatus(genericStatus);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();

}

void  VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSpecificStatus (ResourceId specificStatus)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setSpecificStatus(specificStatus);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
 
}

void VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSwitchPort (SI32 switchPort)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setSwitchPort (switchPort);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

void   VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSwitchWWN    (WorldWideName switchWWN)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setSwitchWWN(switchWWN);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

//Get Functions


bool  VcsFabricLocalObjectManager:: getThisVcsNodeLocalManagedObjectVcsMode ()
{
   bool vcsMode = false;

   if(m_pVcsNodeLocalManagedObject) 
   {
       m_thisVcsNodeLocalManagedObjectMutex.lock();
       vcsMode = m_pVcsNodeLocalManagedObject->getVcsMode (); 
       m_thisVcsNodeLocalManagedObjectMutex.unlock();
   }
   else 
   {
     trace(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager:: getThisVcsNodeLocalManagedObjectVcsMode: VcsNodeLocalManagedObject not found, getting the mode from Vcs Conf file");
     UI32 vcsId = 0;
     parseVcsConfFile(vcsMode,vcsId);
   }

   return vcsMode;  
}
UI32  VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectVcsId () const
{
    UI32 vcsId   = 0;
    bool vcsMode = false;

    if(NULL != m_pVcsNodeLocalManagedObject)
    {
        m_thisVcsNodeLocalManagedObjectMutex.lock();

        vcsId = m_pVcsNodeLocalManagedObject->getVcsId ();

        m_thisVcsNodeLocalManagedObjectMutex.unlock();
    }
    else
    {
        parseVcsConfFile(vcsMode,vcsId);
    }

    return vcsId;
}

ResourceId  VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectGenericStatus () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   ResourceId genericStatus = m_pVcsNodeLocalManagedObject->getGenericStatus();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();

   return genericStatus;
}

ResourceId  VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSpecificStatus () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   ResourceId specificStatus = m_pVcsNodeLocalManagedObject->getSpecificStatus();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return specificStatus;
}

UI32   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectMappedId     () const
{
    UI32 mappedId = 0; // invalid value

    m_thisVcsNodeLocalManagedObjectMutex.lock();
    if ( NULL != m_pVcsNodeLocalManagedObject )
    {
        mappedId = m_pVcsNodeLocalManagedObject->getMappedId ();
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectMappedId: m_pVcsNodeLocalManagedObject is NULL. Return 0");
    }
    m_thisVcsNodeLocalManagedObjectMutex.unlock();
    return mappedId;
}

UI32   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectDomainId     () const
{
   VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

   UI32 domainId = pVcsNodeFabosInterfaceObjectManager->getDomainId ();
   return domainId;
}
LocationId   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectLocationId   () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   LocationId locationId = m_pVcsNodeLocalManagedObject->getLocationId ();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return locationId;
}
ObjectId   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectObjectId     () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   ObjectId objectId = m_pVcsNodeLocalManagedObject->getObjectId();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return objectId;
}
string   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSerialNumber () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   string serialNumber = m_pVcsNodeLocalManagedObject->getSerialNumber ();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return serialNumber;
}

MacAddress VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSwitchMac    () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   MacAddress switchMac = m_pVcsNodeLocalManagedObject->getSwitchMac();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return switchMac;
}
MacAddress   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectfcfMac       () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   MacAddress fcfMac = m_pVcsNodeLocalManagedObject->getFcfMac();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return fcfMac;
}
bool    VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectInVcs        () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   bool inVcs = m_pVcsNodeLocalManagedObject->getInVcs();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return inVcs;
}

bool   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectIsPrincipal  () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   bool isPrincipal = m_pVcsNodeLocalManagedObject->getIsPrincipal();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return isPrincipal;
}
bool  VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectWillingMode () const
{
  m_thisVcsNodeLocalManagedObjectMutex.lock ();
  bool willingMode = m_pVcsNodeLocalManagedObject->getWillingMode ();
  m_thisVcsNodeLocalManagedObjectMutex.unlock ();
  return willingMode;
}

vector<string>    VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectPublicIpAddress () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   vector<string>  ipAddress = m_pVcsNodeLocalManagedObject->getPublicIpAddress();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return ipAddress;
}

SI32   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSwitchPort () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   SI32 switchPort = m_pVcsNodeLocalManagedObject->getSwitchPort ();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return switchPort;

}
WorldWideName   VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSwitchWWN () const
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   WorldWideName switchSwitchWWN = m_pVcsNodeLocalManagedObject->getSwitchWWN ();
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
   return switchSwitchWWN;

}
void VcsFabricLocalObjectManager::updateThisVcsNodeLocalManagedObject()
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   updateWaveManagedObject(m_pVcsNodeLocalManagedObject);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

void VcsFabricLocalObjectManager::createThisVcsNodeLocalManagedObject()
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject = new VcsNodeLocalManagedObject(this);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

VcsNodeLocalManagedObject* VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObject() const
{
    m_thisVcsNodeLocalManagedObjectMutex.lock();   
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject =  m_pVcsNodeLocalManagedObject;
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
    return pVcsNodeLocalManagedObject;

}

void VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSWBDModel (int SWBDModel)
{
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    m_pVcsNodeLocalManagedObject->setSWBDModel (SWBDModel);
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
}

int VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSWBDModel () const
{
    int SWBDModel = -1;

    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    SWBDModel = m_pVcsNodeLocalManagedObject->getSWBDModel ();
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();

    return SWBDModel;
}

void VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSwitchFwVersion (const string &fwVersion)
{
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    m_pVcsNodeLocalManagedObject->setFwVersion (fwVersion);
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
}

string VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSwitchFwVersion () const
{
    string fwVersion = "";
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    fwVersion = m_pVcsNodeLocalManagedObject->getFwVersion ();
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
    return fwVersion;
}

void VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSwitchname (const string &switchname)
{
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    m_pVcsNodeLocalManagedObject->setSwitchname (switchname);
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
}

string VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSwitchname () const
{
    string switchname = "";
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    switchname = m_pVcsNodeLocalManagedObject->getSwitchname ();
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
    return switchname;
}

void VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectSwmodelName (const string &swmodelName)
{
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    m_pVcsNodeLocalManagedObject->setSwmodelName (swmodelName);
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
}

string VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectSwmodelName () const
{
    string swmodelName = "";
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    swmodelName = m_pVcsNodeLocalManagedObject->getSwmodelName ();
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
    return swmodelName;
}

void VcsFabricLocalObjectManager::setThisVcsNodeLocalManagedObjectPrincipalPriority (UI32 principalPriority)
{
   m_thisVcsNodeLocalManagedObjectMutex.lock();
   m_pVcsNodeLocalManagedObject->setPrincipalPriority (principalPriority);
   m_thisVcsNodeLocalManagedObjectMutex.unlock();
}

UI32 VcsFabricLocalObjectManager::getThisVcsNodeLocalManagedObjectPrincipalPriority () const
{
    UI32 principalPriority = 0;
    m_thisVcsNodeLocalManagedObjectMutex.lock ();
    principalPriority = m_pVcsNodeLocalManagedObject->getPrincipalPriority ();
    m_thisVcsNodeLocalManagedObjectMutex.unlock ();
    return principalPriority;
}

bool VcsFabricLocalObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
{
    switch (operationCode)
    {
        case VCS_NODE_IS_LOCAL_NODE_IN_CLUSTER :
        case VCS_NODE_GET_VCSMODE :
        case VCS_NODE_GET_VCSID :
        case WAVE_OBJECT_MANAGER_FIPS_ZEROIZE:
            return (true);
            break;
        default :
            return (WaveObjectManager::isOperationAllowedBeforeEnabling (operationCode));
            break;
    }
}

void VcsFabricLocalObjectManager::getSwitchIdentifierInfoMessageHandler (VcsFabricGetSwitchIdentifierInfoMessage *pVcsFabricGetSwitchIdentifierInfoMessage)
{
    // get chassis ip address using DcmToolKit function.
    string chassisIpAddr      = "";
    DcmToolKit::getChassisIpAddress (chassisIpAddr);    

    pVcsFabricGetSwitchIdentifierInfoMessage->setEth0IpAddress (chassisIpAddr);

    // get IPv6 address.

    string interfaceName    = "eth0";
    string eth0IPv6Address  = WaveNs::getIPv6AddressForInterface (interfaceName);

    pVcsFabricGetSwitchIdentifierInfoMessage->setEth0IPv6Address (eth0IPv6Address);

    // get local node switchname

    string      switchname  = "";
    ResourceId  status      = getLocalNodeSwitchname (switchname);

    pVcsFabricGetSwitchIdentifierInfoMessage->setSwitchname (switchname);

    pVcsFabricGetSwitchIdentifierInfoMessage->setCompletionStatus (status);

    reply (pVcsFabricGetSwitchIdentifierInfoMessage);    
}


void VcsFabricLocalObjectManager::vcsIsIpReachableFromNodeMessageHandler (VcsIsIpReachableFromNodeMessage *pVcsIsIpReachableFromNodeMessage)
{

    string ipToPing = pVcsIsIpReachableFromNodeMessage->getIpAddress();
    string pingCmdString = string("/bin/ping -q  -c 3 -w 20 ") + ipToPing;
    const UI32  lineLength = 256;
    char pingOutputLog [lineLength] = {'\0'};
    bool notFound = true;
    bool noPingLogs = true;

    FILE *fin = popen(pingCmdString.c_str(), "r");
    if (fin == NULL)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::vcsIsIpReachableFromNodeMessageHandler : popen failed to open"));
        pVcsIsIpReachableFromNodeMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pVcsIsIpReachableFromNodeMessage);
        return;
    }



    while (((fgets(pingOutputLog, lineLength, fin) != NULL)) & (notFound))
    {

          noPingLogs = false;

	   char * pch = strtok (pingOutputLog, ",");
		 while (pch != NULL)
		 {
			if (NULL != strstr(pch, "100% packet loss"))
			{
				notFound = false;
				break;
			}
			pch = strtok (NULL, ",");
		 }
    }
    pclose(fin);

    // In some cases, code flow will not enter above while loop (fgets returns NULL).
    if(true == noPingLogs)
    {
        notFound = false;
    }

    

    if (false == notFound)
    {
        pVcsIsIpReachableFromNodeMessage->setIsReachableFlag(false);
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::vcsIsIpReachableFromNodeMessageHandler  Ping =Fail"));
    }
    else
    {
        pVcsIsIpReachableFromNodeMessage->setIsReachableFlag(true);
        trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::vcsIsIpReachableFromNodeMessageHandler  Ping =Pass"));
    }

    pVcsIsIpReachableFromNodeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pVcsIsIpReachableFromNodeMessage); 

}

void VcsFabricLocalObjectManager::getClusterRbridgeIdFromWWNStaticHandler (VcsGetClusterRbridgeIdFromWWNMessage *pVcsGetClusterRbridgeIdFromWWNMessage)
{
    (VcsFabricLocalObjectManager::getInstance ())->getClusterRbridgeIdFromWWN (pVcsGetClusterRbridgeIdFromWWNMessage);
}

void VcsFabricLocalObjectManager::cliAllowedFromThisLocationMessageHandler (VcsNodeCliAllowedFromThisLocationMessage *pVcsNodeCliAllowedFromThisLocationMessage)
{
    WaveNs::prismAssert (NULL != pVcsNodeCliAllowedFromThisLocationMessage, __FILE__, __LINE__);
    
    DcmToolKit::setIsCliAllowedFromThisLocation (pVcsNodeCliAllowedFromThisLocationMessage->getIsCliAllowedFromThisLocation ());
    
    trace (TRACE_LEVEL_INFO,string ("VcsFabricLocalObjectManager::cliAllowedFromThisLocationMessageHandler cliAllowedFromThisLocation set = ")+pVcsNodeCliAllowedFromThisLocationMessage->getIsCliAllowedFromThisLocation ());
    
    pVcsNodeCliAllowedFromThisLocationMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeCliAllowedFromThisLocationMessage);
}

void VcsFabricLocalObjectManager::cliAllowedFromThisLocationServiceIndependentStaticMessageHandler (VcsNodeCliAllowedFromThisLocationMessage *pVcsNodeCliAllowedFromThisLocationMessage)
{
    WaveNs::prismAssert (NULL != pVcsNodeCliAllowedFromThisLocationMessage, __FILE__, __LINE__);
    
    DcmToolKit::setIsCliAllowedFromThisLocation (pVcsNodeCliAllowedFromThisLocationMessage->getIsCliAllowedFromThisLocation ());
    
    WaveNs::trace (TRACE_LEVEL_INFO,string ("VcsFabricLocalObjectManager::cliAllowedFromThisLocationServiceIndependentStaticMessageHandler cliAllowedFromThisLocation set = ")+pVcsNodeCliAllowedFromThisLocationMessage->getIsCliAllowedFromThisLocation ());
    
    pVcsNodeCliAllowedFromThisLocationMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::getClusterRbridgeIdFromWWNHandler (VcsGetClusterRbridgeIdFromWWNMessage *pVcsGetClusterRbridgeIdFromWWNMessage)
{
    getClusterRbridgeIdFromWWN (pVcsGetClusterRbridgeIdFromWWNMessage);
    reply (pVcsGetClusterRbridgeIdFromWWNMessage);
}

void VcsFabricLocalObjectManager::getClusterRbridgeIdFromWWN (VcsGetClusterRbridgeIdFromWWNMessage *pVcsGetClusterRbridgeIdFromWWNMessage)
{
    WorldWideName                   inputWWN                    = pVcsGetClusterRbridgeIdFromWWNMessage->getInputWWN ();
    ResourceId                      status                      = WAVE_MESSAGE_SUCCESS;
    const UI32                      maxRbridgeId                = 239;
    BitMap                          availableRbridgeIdBitMap (maxRbridgeId);
    map<string, UI32>               wwnRbridgeIdMap;

    availableRbridgeIdBitMap.reset ();
    wwnRbridgeIdMap.clear ();

    //get the WWN RbridgeId map from DB if nothing is present get the same from the cache. If both are NULL assert. 

    VcsFabricLocalObjectManager::getWWNRbridgeIdMapFromDB (wwnRbridgeIdMap);

    if (wwnRbridgeIdMap.size () == 0)
    {
        VcsFabricLocalObjectManager::getWWNRbridgeIdMapCache (wwnRbridgeIdMap);

        if (wwnRbridgeIdMap.size () == 0)
        {
            trace (TRACE_LEVEL_FATAL, string ("VcsFabricLocalObjectManager::getClusterRbridgeIdFromWWN: Cache also cannot be empty. It should atleast have entry for the current node"));
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR;
        }
    }

    //Check if the inputWWN is present
    if (wwnRbridgeIdMap.end () != wwnRbridgeIdMap.find (inputWWN.toString ()))
    {
        UI32 rbridgeId = wwnRbridgeIdMap[inputWWN.toString ()];
        availableRbridgeIdBitMap.set (rbridgeId-1);
    }
    else
    {
        //WWN is not found, fill all the free slots in the bit map

        //Set all the values of the BitMap and reset only the ones which are currently being used.
        availableRbridgeIdBitMap.set ();

        for (map <string,UI32>::iterator it = wwnRbridgeIdMap.begin (); it != wwnRbridgeIdMap.end (); ++it)
        {
            availableRbridgeIdBitMap.reset ((it->second)-1);
        }

        //if the size of the map is same as maxRbridgeId then all the rbridgeIds are in use
        if (maxRbridgeId == wwnRbridgeIdMap.size ())
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::getClusterRbridgeIdFromWWN: All Rbridge-id slots are in use, no free slots found"));
                //No available slot
                status = WAVE_MESSAGE_ERROR;
            }
    }

    pVcsGetClusterRbridgeIdFromWWNMessage->setRbridgeIdBitMap (availableRbridgeIdBitMap);

    pVcsGetClusterRbridgeIdFromWWNMessage->setCompletionStatus (status);
}

void VcsFabricLocalObjectManager::setFcfMacAddressHandler(VcsNodeSetFcfMacMessageServiceIndependent *pVcsNodeSetFcfMacMessage)
{

    VcsFabricLocalObjectManager::setFcfMacAddressStaticHandler (pVcsNodeSetFcfMacMessage);
    reply(pVcsNodeSetFcfMacMessage);
}

void VcsFabricLocalObjectManager::setFcfMacAddressStaticHandler (VcsNodeSetFcfMacMessageServiceIndependent *pVcsNodeSetFcfMacMessage)
{
    m_thisVcsNodeFcfMacMutex.lock ();

    m_thisVcsNodeFcfMac = pVcsNodeSetFcfMacMessage->getFcfMac ();   

    WaveNs::tracePrintf ( TRACE_LEVEL_INFO ," VcsFabricLocalObjectManager::setFcfMacAddressStaticHandler Mac address :%s ",( m_thisVcsNodeFcfMac.toString ()).c_str());

    m_thisVcsNodeFcfMacMutex.unlock ();
}

void VcsFabricLocalObjectManager::registerClusterEnabledCheckFunctionWithPersistence ()
{
    // A function will be registered with persistence OM which will tell if MC is enabled.

    // If the node was part of multi node cluster, Its location role will not be LOCATION_STAND_ALONE,
    // and, in this case copy startup-schema to running-schema is not done in PersistenceObjectManager::boot ()

    // If location role is LOCATION_STAND_ALONE, then, PersistenceObjectManager will call the function registered below,
    // If the function returns ClusterEnabled = true,
    // Persistence OM will skip the operation : copy startup-schema to running-schema.
    // So, running schema will be used as is. and configuration will not be lost in case of reboot of Single Node MC enabled node.

    // set value of m_thisVcsNodeClusterType by parsing cluster.conf and vcs.conf

    bool   vcsMode         = false;
    bool   clusterMode     = false;
    UI32   vcsId           = 0;

    parseVcsConfFile (vcsMode, vcsId);

    clusterMode = parseClusterConfFile ();

    setThisVcsNodeClusterType (vcsMode, clusterMode);

    Wave::registerClusterEnabledCheck (&DcmToolKit::isVcsClusterEnabled);
}


void VcsFabricLocalObjectManager::registerAddRASLogFunctionWithPersistence ()
{
    Wave::registerLogOperationStatusFunction (&VcsFabricLocalObjectManager::addRASLogForFramework);
}

void VcsFabricLocalObjectManager::addRASLogForFramework (ResourceId resourceIdFromFramework)
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::addRASLogForCopyCommand : resourceIdFromFramework = ") + FrameworkToolKit::localize (resourceIdFromFramework));

    int raslog_message_id = VcsFabricLocalObjectManager::getRASLogIdFromFrameworkResourceId (resourceIdFromFramework); 
    if (-1 != raslog_message_id)
    {
        raslog_ext("VcsFabLocalMO", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, raslog_message_id);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_WARN, string ("Unsupported resourceId for RAS Log : ") + FrameworkToolKit::localize (resourceIdFromFramework));
    }
}

void VcsFabricLocalObjectManager::registerVCSEnableCheckFunctionWithWave ()
{
    Wave::registerIsWaveClusteringEnabledFunction (&VcsFabricLocalObjectManager::isVcsEnabled);
}

bool VcsFabricLocalObjectManager::isVcsEnabled ()
{
   
    bool isVcsEnabledFlag = false;
    VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance();

    isVcsEnabledFlag = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsMode();

    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::isVcsEnabled : isVcsEnabledFlag = ") + isVcsEnabledFlag); 
    return isVcsEnabledFlag;
}

void VcsFabricLocalObjectManager::registerLinecardReadySlotIdBitMapFunction()
{   
    Wave::registerGetLinecardReadySlotIdBitMapFunction ( &VcsFabricLocalObjectManager::getLinecardReadySlotIdBitMap );
}

void VcsFabricLocalObjectManager::getLinecardReadySlotIdBitMap ( BitMap &linecardReadySlotIdBitMap, UI32 &maxLinecards )
{   
    linecardReadySlotIdBitMap.reset();

    VcsFabricLocalObjectManager * pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

    pVcsFabricLocalObjectManager->m_linecardReadySlotIdBitMapMutex.lock();

    linecardReadySlotIdBitMap = pVcsFabricLocalObjectManager->m_linecardReadySlotIdBitMap ;
    maxLinecards = MAX_LINECARD_NUM ;

    pVcsFabricLocalObjectManager->m_linecardReadySlotIdBitMapMutex.unlock();

}

UI32 VcsFabricLocalObjectManager::getVcsID ()
{   

    WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsFabricLocalObjectManager::getVcsID" );

    UI32 vcsId = 0;
    VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance();

    vcsId = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsId ();
   
    return vcsId;
}

int VcsFabricLocalObjectManager::getRASLogIdFromFrameworkResourceId (ResourceId resourceIdFromFramework)
{
    int raslog_message_id = -1;

    switch (resourceIdFromFramework)
    {
        case COPY_DEFAULT_TO_STARTUP_SUCCESS :
            //raslog_message_id = DCM_1103;
            break;            

        case COPY_DEFAULT_TO_STARTUP_FAILURE :
            //raslog_message_id = DCM_1104;
            break;

        case FIPS_OPERATION_ZEROIZE_REQUEST_RECEIVED :
            raslog_message_id = DCM_1201;
            break;

        case FIPS_OPERATION_FAILED_FOR_NODE_NOT_STANDALONE:
            raslog_message_id = DCM_1202;
            break;

        case FIPS_OPERATION_CONFIRMED_NODE_AS_STANDALONE :
            raslog_message_id = DCM_1203;
            break;

        case FIPS_OPERATION_NOTIFY_SESSIONS_ABT_ZEROIZE :
            raslog_message_id = DCM_1204;
            break;

        case FIPS_OPERATION_TRIGGER_CLEANUP_FOR_ZEROIZE:
            raslog_message_id = DCM_1205;
            break;

        case FIPS_OPERATION_SHUT_DOWN_SERVICES_FOR_ZEROIZE :
            raslog_message_id = DCM_1206;
            break;

        case FIPS_OPERATION_FAILED_FOR_SERVICES_NOT_SHUT_DOWN:
            raslog_message_id = DCM_1207;
            break;

        case FIPS_OPERATION_RUN_ZEROIZE_FOR_SUPPORTED_SERVICES:
            raslog_message_id = DCM_1208;
            break;

        case FIPS_OPERATION_FAILED_FOR_RUNNING_ZEROIZE_FOR_SUPPORTED_SERVICES :
            raslog_message_id = DCM_1209;
            break;

        case FIPS_OPERATION_ZEROIZE_FAILED :
            raslog_message_id = DCM_1210;
            break;

        case FIPS_OPERATION_ZEROIZE_SUCCESS :
            raslog_message_id = DCM_1211;
            break;
        case FIPS_OPERATION_ZEROIZE_FAILED_ALREADY_ZEROIZED:
            raslog_message_id = DCM_1212;
            break;
       case DATABASE_SCHEMA_CONVERSION_SUCCEEDED:
	        raslog_message_id = DCM_4001;
	        break;
       case DATABASE_SCHEMA_CONVERSION_FAILED:
	        raslog_message_id = DCM_4002;
	        break;
       case REBOOT_FOR_DB_CORRUPTION_AUTO_RECOVERY:
	        raslog_message_id = DCM_3051;
	        break;

        default:
            raslog_message_id = -1;
    }
    return (raslog_message_id);
}

void  VcsFabricLocalObjectManager::vcsIsFabricDistributionEnabledMessageHandler(VcsNodeIsFabricDistributionEnabledMessage *pVcsNodeIsFabricDistributionEnabledMessage)
{

       ResourceId   status = WAVE_MESSAGE_ERROR;

       if(LOCATION_STAND_ALONE!=FrameworkToolKit::getThisLocationRole())
       {
          pVcsNodeIsFabricDistributionEnabledMessage->setFabricDistributionEnabled(true);
       }

       else 
       {
         pVcsNodeIsFabricDistributionEnabledMessage->setFabricDistributionEnabled(false);
       }
       status = WAVE_MESSAGE_SUCCESS;
    

       pVcsNodeIsFabricDistributionEnabledMessage->setCompletionStatus(status);
       reply (pVcsNodeIsFabricDistributionEnabledMessage);
}

void VcsFabricLocalObjectManager::updateSwitchInfoMessageHandler (VcsNodeUpdateSwitchInfoMessage *pVcsNodeUpdateSwitchInfoMessage)
{
    string          chassisString   = pVcsNodeUpdateSwitchInfoMessage->getChassisString ();
    MacAddress      switchMac       = pVcsNodeUpdateSwitchInfoMessage->getSwitchMac ();
    int             domainId        = pVcsNodeUpdateSwitchInfoMessage->getDomainId ();
    WorldWideName   switchWWN       = pVcsNodeUpdateSwitchInfoMessage->getSwitchWWN ();
    int             SWBDModel       = pVcsNodeUpdateSwitchInfoMessage->getSWBDModel ();
    string          fwVersion       = pVcsNodeUpdateSwitchInfoMessage->getFwVersion ();
    string          swmodelName     = pVcsNodeUpdateSwitchInfoMessage->getSwmodelName ();
    
    updateLocalManagedObject (chassisString, switchMac, domainId, switchWWN, SWBDModel, fwVersion, swmodelName);
    
    pVcsNodeUpdateSwitchInfoMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeUpdateSwitchInfoMessage);
}

void VcsFabricLocalObjectManager::testClientForDualConnectMessageHandler (VcsNodeTestClientForDualConnectMessage *pMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::testClientForDualConnectMessageHandler : entering...");
    pMessage->setString (FrameworkToolKit::getProcessInitialWorkingDirectory ());

    trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::testClientForDualConnectMessageHandler : directory : ") + FrameworkToolKit::getProcessInitialWorkingDirectory ());
    pMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void VcsFabricLocalObjectManager::getCurrentRbridgeIdsAndLocationIdsMessageHandler (VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage *pVcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::getCurrentRbridgeIdsAndLocationIdsMessageHandler : Entering...");
    
    vector<UI32>        rbridgeIds;
    vector<LocationId>  locationIds;
    ResourceId          status      = WAVE_MESSAGE_ERROR;

    // Query for all VcsNodeLocalMO's and build a local mapping of rbridgeIds to locationIds

    WaveManagedObjectSynchronousQueryContext  synchronousQueryContext (VcsNodeLocalManagedObject::getClassName ());
    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::getCurrentRbridgeIdsAndLocationIdsMessageHandler : Query of VcsNodeLocalMO's returned 0 results.");
        }
        else
        {
            for (UI32 i = 0; i < numberOfResults; i++)
            {
                VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[i]);

                if (NULL == pVcsNodeLocalManagedObject)
                {
                    trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::getCurrentRbridgeIdsAndLocationIdsMessageHandler : Managed object results failed to be dynamic casted to a VcsNodeLocalManagedObject.");

                    prismAssert (false, __FILE__, __LINE__);
                }

                UI32       rbridgeId = pVcsNodeLocalManagedObject->getDomainId ();
                LocationId locationId = pVcsNodeLocalManagedObject->getLocationId ();

                trace (TRACE_LEVEL_DEBUG, string ("VcsFabricLocalObjectManager::getCurrentRbridgeIdsAndLocationIdsMessageHandler : Found Rbridge-id / Location id : ") + rbridgeId + " / " + locationId);

                rbridgeIds.push_back (rbridgeId);
                locationIds.push_back (locationId);
            }

            status = WAVE_MESSAGE_SUCCESS;
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::getCurrentRbridgeIdsAndLocationIdsMessageHandler : Query of VcsNodeLocalMO's is NULL.");
    }
    
    // Save Rbridge Id / Location Id queried results

    pVcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage->setRbridgeIds (rbridgeIds);
    pVcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage->setLocationIds (locationIds);

    pVcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage->setCompletionStatus (status);
    reply (pVcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage); 
}

void VcsFabricLocalObjectManager::vcsSetWyserSessionRateLimitingDelayMessageHandler (VcsSetWyserEaRateLimitingDelayMessage *pVcsSetWyserEaRateLimitingDelayMessage)
{
    string      wyserUserSessionContext        = pVcsSetWyserEaRateLimitingDelayMessage->getWyserUserInfoContext ();
    //UI32        delayInMicroseconds = pVcsSetWyserEaRateLimitingDelayMessage->getDelayInMicroseconds ();
    ResourceId  status              = WAVE_MESSAGE_ERROR;

    pVcsSetWyserEaRateLimitingDelayMessage->setCompletionStatus (status);
    reply (pVcsSetWyserEaRateLimitingDelayMessage);
}

void VcsFabricLocalObjectManager::vcsGetWyserEaRateLimitingDelayMessageHandler (VcsGetWyserEaRateLimitingDelayMessage *pVcsGetWyserEaRateLimitingDelayMessage)
{
    string      wyserUserSessionContext        = pVcsGetWyserEaRateLimitingDelayMessage->getWyserUserInfoContext ();
    ResourceId  status              = WAVE_MESSAGE_ERROR;

    pVcsGetWyserEaRateLimitingDelayMessage->setCompletionStatus (status);
    reply (pVcsGetWyserEaRateLimitingDelayMessage);
}

void VcsFabricLocalObjectManager::vcsSetWyserEaConfigFilterStaticMessageHandler (VcsSetWyserEaConfigFilterServiceIndependentMessage *pVcsSetWyserEaConfigFilterServiceIndependentMessage)
{
    //SI32        userSessionId                   = pVcsSetWyserEaConfigFilterServiceIndependentMessage->getUserSessionId ();
    //ResourceId  sessionConfigFilterContextType  = pVcsSetWyserEaConfigFilterServiceIndependentMessage->getSessionConfigFilterContextType ();
    ResourceId  status                          = WAVE_MESSAGE_SUCCESS;
    
    // TBDDCMOSS : Link with Wyser ::addConfigFilterContextForSession (sessionConfigFilterContextType, userSessionId);

    pVcsSetWyserEaConfigFilterServiceIndependentMessage->setCompletionStatus (status);
}

void VcsFabricLocalObjectManager::vcsSetWyserEaConfigFilterMessageHandler (VcsSetWyserEaConfigFilterServiceIndependentMessage *pVcsSetWyserEaConfigFilterServiceIndependentMessage)
{
    //SI32        userSessionId                   = pVcsSetWyserEaConfigFilterServiceIndependentMessage->getUserSessionId ();
    //ResourceId  sessionConfigFilterContextType  = pVcsSetWyserEaConfigFilterServiceIndependentMessage->getSessionConfigFilterContextType ();
    ResourceId  status                          = WAVE_MESSAGE_SUCCESS;

    // TBDDCMOSS : Link with Wyser ::addConfigFilterContextForSession (sessionConfigFilterContextType, userSessionId);

    pVcsSetWyserEaConfigFilterServiceIndependentMessage->setCompletionStatus (status);
    reply (pVcsSetWyserEaConfigFilterServiceIndependentMessage);
}

void VcsFabricLocalObjectManager::vcsRemoveWyserEaConfigFilterStaticMessageHandler (VcsRemoveWyserEaConfigFilterServiceIndependentMessage *pVcsRemoveWyserEaConfigFilterServiceIndependentMessage)
{   
    //SI32        userSessionId                   = pVcsRemoveWyserEaConfigFilterServiceIndependentMessage->getUserSessionId ();
    ResourceId  status                          = WAVE_MESSAGE_SUCCESS;

    // TBDDCMOSS : Link with Wyser ::removeConfigFilterContextForSession (userSessionId);
    
    pVcsRemoveWyserEaConfigFilterServiceIndependentMessage->setCompletionStatus (status);
}
    
void VcsFabricLocalObjectManager::vcsRemoveWyserEaConfigFilterMessageHandler (VcsRemoveWyserEaConfigFilterServiceIndependentMessage *pVcsRemoveWyserEaConfigFilterServiceIndependentMessage)
{
    //SI32        userSessionId                   = pVcsRemoveWyserEaConfigFilterServiceIndependentMessage->getUserSessionId ();
    ResourceId  status                          = WAVE_MESSAGE_SUCCESS;
    
    // TBDDCMOSS : Link with Wyser ::removeConfigFilterContextForSession (userSessionId);

    pVcsRemoveWyserEaConfigFilterServiceIndependentMessage->setCompletionStatus (status);
    reply (pVcsRemoveWyserEaConfigFilterServiceIndependentMessage);
}

void VcsFabricLocalObjectManager::zeroize (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{   
    // STEP 1. reset rbridge-id if in Dcmd, and in Vcs Enabled mode. (This will internally do chassis-disable ?)

    ResourceId  currentDistributionMode = DcmToolKit::getDistributionMode ();
    bool        isVcsEnabledFlag        = false;

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : START : currentDistributionMode [") + FrameworkToolKit::localizeToSourceCodeEnum (currentDistributionMode) + string ("]"));

    if ((DCM_ONLY == currentDistributionMode) || (DCM_CCM == currentDistributionMode))
    {
        if (WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ()) // Check for NOOP mode when in MC when supported.
        {
            isVcsEnabledFlag = true;
        }

        if (true == isVcsEnabledFlag)
        {
            UI32        defaultRbridgeId    = 1;
            ResourceId  setRbridgeIdStatus  = setRBridgeIdBackend (defaultRbridgeId);

            if (WAVE_MESSAGE_SUCCESS == setRbridgeIdStatus)
            {
                trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::zeroize : STEP 1 : successfully reset rbridge-id to 1");
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::zeroize : STEP 1 : setRBridgeIdBackend failed. status [" + FrameworkToolKit::localizeToSourceCodeEnum (setRbridgeIdStatus) + string ("], value [") + setRbridgeIdStatus + string ("]"));
            }
        }
    }


    // STEP 2. Delete .cfg files.
    //      (Should this be done by an ObjectManager in Wave library itself ?)

    //   2.a Acquire file lock.

    bool    cfgFileLocked   = false;
    string  lockFilename    = PrismFrameworkObjectManager::getLockFileForConfigurationFile ();
    FILE   *pCfgLockFile    = fopen (lockFilename.c_str(), "w");

    if (NULL != pCfgLockFile)
    {
        int lockStatus = flock (fileno (pCfgLockFile), LOCK_EX);

        if (0 == lockStatus)
        {
            cfgFileLocked = true;
        }
        else
        {
            fclose (pCfgLockFile);
        }
    }

    //   2.b Delete/clear .cfg files.

    bool    deleteOnClean = true;

    string configFilePath = PrismFrameworkObjectManager::getConfigurationFileName() ;
    FrameworkToolKit::secureClearFile(configFilePath,deleteOnClean);
    FrameworkToolKit::secureClearFile("/mnt/"+configFilePath,deleteOnClean);
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 2 : cleared and deleted .cfg files. [") + configFilePath + string ("]"));

    //   2.c Delete/clear .cfg backup (.cfg.bk) files.

    string configurationBackupFilePath = PrismFrameworkObjectManager::getConfigurationBackupFileName ();
    FrameworkToolKit::secureClearFile (configurationBackupFilePath, deleteOnClean);
    FrameworkToolKit::secureClearFile ("/mnt/" + configurationBackupFilePath, deleteOnClean);
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 2 : cleared and deleted .cfg.bk files. [") + configurationBackupFilePath + string ("]"));

    //   2.d Release file lock.

    if (true == cfgFileLocked)
    {
        flock (fileno (pCfgLockFile), LOCK_UN);
        fclose (pCfgLockFile);
    }

    // Following steps are executed only in Dcmd.

    if ((DCM_ONLY == currentDistributionMode) || (DCM_CCM == currentDistributionMode))
    {
        // STEP 3. Reset cluster.conf and vcs.conf files

        // Get default values for vcs.conf and cluster.conf.
        bool vcsEnabledFlag = false;
        SI32 vcsIdValue     = 0;

        if (true == isVcsEnabledFlag)
        {
            vcsEnabledFlag  = true;
            vcsIdValue      = 1;
        }

    // cleanup vcs.conf and cluster.conf files but dont delete them.

        //     3.a : vcs.conf

    deleteOnClean = false;

    string vcsConfPath = DcmToolKit::getVcsConfPath ();
    vcsConfPath.append ("/vcs.conf");
        FrameworkToolKit::secureClearFile(vcsConfPath,deleteOnClean);
        // Need to write "0000" / proper vcs id, to make sure node is coming up with vcs disabled.
        setVcsConfFile (vcsEnabledFlag, vcsIdValue);
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 3 : set vcsid [") + vcsIdValue + string ("] in vcs.conf"));

        FrameworkToolKit::secureClearFile("/mnt/"+vcsConfPath,deleteOnClean);
        string          cmdCopyFilesToMnt    = "/bin/cp -f " + vcsConfPath + " " + "/mnt/"+vcsConfPath;
        vector<string>  output;
        int systemRet = FrameworkToolKit::systemCommandOutput (cmdCopyFilesToMnt, output);
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 3 : copied vcs.conf to mnt partition, status [") + systemRet + string ("]")); 
        output.clear ();

        //     3.a : cluster.conf
   
        string clusterConfPath = DcmToolKit::getVcsConfPath () ;
        clusterConfPath.append ("/cluster.conf");
        FrameworkToolKit::secureClearFile(clusterConfPath,deleteOnClean);
        setClusterConfFile (false);
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 3 : set cluster.conf to 0"));

        FrameworkToolKit::secureClearFile("/mnt/"+clusterConfPath,deleteOnClean);
        cmdCopyFilesToMnt.clear ();
        cmdCopyFilesToMnt   = "/bin/cp -f " + clusterConfPath + " " + "/mnt/"+clusterConfPath;
        systemRet = FrameworkToolKit::systemCommandOutput (cmdCopyFilesToMnt, output);
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 3 : copied cluster.conf to mnt partition, status [") + systemRet + string ("]"));
        output.clear ();


        // STEP 4. Delete/clear config files, and reset startup file to default config depending on default vcs mode.

        //      (Should this be done by Persistence*ObjectManager ?)

        /* Also clear startup_config file if created. */

        removeUserCreatedFiles (string (ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/").c_str ());
        removeUserCreatedFiles (string ("/mnt" + ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/").c_str ());
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 4 : Removed UserCreatedFiles in /var/config/vcs/scripts/, and /mnt/var/config/vcs/scripts/"));

        string          startupFileName     = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory () + "/startup-config";

        string defaultConfigFile    = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/";
        
        if (true == vcsEnabledFlag)
        {
            defaultConfigFile = defaultConfigFile + "defaultconfig.vcs";
        }
        else
        {
            defaultConfigFile = defaultConfigFile + "defaultconfig.novcs";
        }

        string cmd = string("cp -f " + defaultConfigFile + " " + startupFileName);
        systemRet = FrameworkToolKit::systemCommandOutput (cmd, output);
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 4 : copied [") + defaultConfigFile + string ("] to [") + startupFileName + string ("], status [") + systemRet + string ("]"));
        output.clear ();


        // STEP 5. Delete startup config backup file.

        deleteOnClean       = true;

        string startupConfigBackupFileName = FrameworkToolKit::getStartupBackupFileName ();
        FrameworkToolKit::secureClearFile (startupConfigBackupFileName.c_str (), deleteOnClean);
        FrameworkToolKit::secureClearFile ((string ("/mnt") + startupConfigBackupFileName).c_str (), deleteOnClean);
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : STEP 5 : cleared and deleted StartupBackupFile s, [") + startupConfigBackupFileName + string ("]"));

        // STEP 6. Delete wyserea audit Log files
    }

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::zeroize : COMPLETE"));

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback();
}

void VcsFabricLocalObjectManager::removeUserCreatedFiles(const char* flashPath)
{
    /* Also clear startup_config file if created. */

    set<string> filesToBeSkipped;
    filesToBeSkipped.insert (".");
    filesToBeSkipped.insert ("..");
    filesToBeSkipped.insert ("defaultconfig.novcs");
    filesToBeSkipped.insert ("defaultconfig.vcs");

    bool    deleteOnClean   = true;
    DIR    *pDIR            = opendir(flashPath);

    if (NULL != pDIR)
    {
    struct  dirent *entry       = NULL;

        while((entry = readdir(pDIR)) != NULL)
        {
            string fileName = entry->d_name;

            set<string>::iterator itr = filesToBeSkipped.find (fileName);
            set<string>::iterator end = filesToBeSkipped.end ();

            if (end == itr)
            {
                FrameworkToolKit::secureClearFile (string (flashPath) + fileName, deleteOnClean);

                trace (TRACE_LEVEL_DEVEL, string ("VcsFabricLocalObjectManager::removeUserCreatedFiles : cleared and deleted [") + fileName + string ("]"));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::removeUserCreatedFiles : skipped [") + fileName + string ("]"));
            }

        }

        closedir(pDIR);
    }
    else
    {
        tracePrintf (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::removeUserCreatedFiles : could not open the directory [%s].", flashPath);
    }
}

string VcsFabricLocalObjectManager::getFwVersion ()
{
    return (VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeSwitchFwVersion ());
}

void VcsFabricLocalObjectManager::sendHaSyncUpdateMessageToCcmd (const UI32 &context)
{
    ResourceId                     status;
    ResourceId                     connectionStatus;
    struct in_addr                 localIpa;
    string                         localIp = "";
    SI32                           ccmdPort = DcmToolKit::getManagementInterfaceReceiverPortForCcmd ();
    struct in_addr                 peerIpa;
    string                         peerIp = "";
    SI32                           peerPort = DcmToolKit::getManagementInterfaceReceiverPortForCcmd ();;
    string                         myName = "";

    localIpa.s_addr = get_cp_ha_localip ();
    localIp.assign (inet_ntoa (localIpa));

    myName = string ("VcsFabricLocalObjectManager:") + localIp + string (":") + ccmdPort;

    peerIpa.s_addr = get_cp_ha_peerip ();
    peerIp.assign (inet_ntoa (peerIpa));

    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::sendHaSyncUpdateMessageToCcmd : HA peer ip:") + peerIp + string(" port:") + peerPort);

    ValClientSynchronousConnection valClientSynchronousConnection (myName, localIp, ccmdPort);

    connectionStatus = valClientSynchronousConnection.connect ();

    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER != connectionStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::sendHaSyncUpdateMessageToCcmd : ValClientSynchronousConnection failed.  Error: ") + FrameworkToolKit::localize (connectionStatus));

        return;
    }

    status = valClientSynchronousConnection.sendSyncUpdateToCcmd (peerIp, peerPort, context);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "VcsFabricLocalObjectManager::sendHaSyncUpdateMessageToCcmd : sync update message to Ccmd was successful.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::sendHaSyncUpdateMessageToCcmd : Send sync update message to Ccmd failed : " + FrameworkToolKit::localize (status));
    }

    valClientSynchronousConnection.close ();

    if (valClientSynchronousConnection.isCurrentlyConnected ())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::sendHaSyncUpdateMessageToCcmd : Connection to server was not properly closed ipAddress = " ) + localIp);
    }

    return;
}

ResourceId VcsFabricLocalObjectManager::sendCcmdSyncUpdateMessage (const UI32 &context)
{
    
    ResourceId distributionMode = DcmToolKit::getDistributionMode ();

    if (FrameworkToolKit::getIsSyncDumpCompleted ())
    {
        if (COPY_RUNNING_TO_STARTUP_CONTEXT == context)
        {

            // Send Sync update to Ccmd only in FC Mode
            // If live sync is enabled, we don't need to sync ccmd because running config is already synced

            if (DCM_ONLY == distributionMode && (FrameworkToolKit::getIsLiveSyncEnabled () == false))
            {
                trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::sendCcmdSyncUpdateMessage: trigger Ccmd HA sync update in copy running to start context");
                sendHaSyncUpdateMessageToCcmd (CCMD_COPY_RUNNING_TO_STARTUP_CONTEXT);
            }

        }
        else if (COPY_RUNNING_TO_STARTUP_OPTIMIZATION_CONTEXT == context)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::sendCcmdSyncUpdateMessage: Ccmd running db already in sync, skip this step.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::sendCcmdSyncUpdateMessage: sync dump is not completed, can't send Ccmd HA sync update.");
    }

    return WAVE_MESSAGE_SUCCESS;
}


void VcsFabricLocalObjectManager::sendHaSyncUpdateMessage ()
{
    FrameworkObjectManagerHaSyncUpdateMessage *pFrameworkObjectManagerHaSyncUpdateMessage = new FrameworkObjectManagerHaSyncUpdateMessage ();
    prismAssert (NULL != pFrameworkObjectManagerHaSyncUpdateMessage, __FILE__, __LINE__);

    pFrameworkObjectManagerHaSyncUpdateMessage->setIsDbSyncRequired (false);

    SyncContextTypes context = VCS_CLUSTER_CONF_CHANGE_CONTEXT;
    pFrameworkObjectManagerHaSyncUpdateMessage->addBuffer (CONTEXT_INFO, sizeof (UI32), (void *) &context, false);

    ResourceId status;

    status = sendSynchronously (pFrameworkObjectManagerHaSyncUpdateMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::sendHaSyncUpdateMessage: Failed send to FrameworkObjectManagerHaSyncUpdateMessage.  Status: ") + FrameworkToolKit::localize (status));
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::sendHaSyncUpdateMessage: FrameworkObjectManagerHaSyncUpdateMessage sent.  "));
    }

    if (pFrameworkObjectManagerHaSyncUpdateMessage != NULL)
    {
        delete pFrameworkObjectManagerHaSyncUpdateMessage;
    }
}

ResourceId VcsFabricLocalObjectManager::setVcsClusterConfigHandler (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context)
{
    bool                localVcsMode;
    UI32                localVcsId;
    //UI32                localRbridgeId;
    SI32                localClusterConf;
    bool				vcsIdChanged = false;
    bool				rbridgeIdChanged = false;
    bool                clusterConfChanged = false;
    bool                vcsModeChanged = false;
    UI32                rbridgeId = 0;

    VcsFabricLocalObjectManager * pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance();
    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

    pVcsFabricLocalObjectManager->parseVcsConfFile (localVcsMode, localVcsId);
    localClusterConf = pVcsFabricLocalObjectManager->parseClusterConfFile ();
    //localRbridgeId = pVcsNodeFabosInterfaceObjectManager->getDomainId ();

    pVcsFabricLocalObjectManager->startTransaction ();

    if (pDcmVcsClusterConfigData->getVcsId () != localVcsId)
    {
    	WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::setVcsClusterConfigHandler: need to set vcsId to ") + pDcmVcsClusterConfigData->getVcsId () + string(" from ") + localVcsId);

    	pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();
    	pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectVcsId (pDcmVcsClusterConfigData->getVcsId ());

        vcsIdChanged = true;
    }

    if (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectDomainId() != pDcmVcsClusterConfigData->getRbridgeId ())
    {
        rbridgeId = pDcmVcsClusterConfigData->getRbridgeId ();

        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::setVcsClusterConfigHandler: rbridgeId changed to ") + rbridgeId);

        // Set the rbridge id
        pVcsNodeFabosInterfaceObjectManager->setDomainId (rbridgeId);

        pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();
        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectDomainId (rbridgeId);

        rbridgeIdChanged = true;
	}

	if (pDcmVcsClusterConfigData->getVcsMode () != localVcsMode)
	{
		bool vcsMode = pDcmVcsClusterConfigData->getVcsMode ();

		WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::setVcsClusterConfigHandler: need to set vcsId to ") + vcsMode + string(" from ") + localVcsMode);

		pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();
		pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectVcsMode (vcsMode);

		string vcsModeString = vcsMode ? "true":"false";

		WaveNs::trace (TRACE_LEVEL_INFO,"VcsFabricLocalObjectManager::setVcsClusterConfigHandler: vcs mode set to " + vcsModeString);

		if (vcsMode)
		{
			pVcsFabricLocalObjectManager->runVcsModeEnableScript ();
		}
		else
		{
            pVcsFabricLocalObjectManager->runVcsModeDisableScript ();
		}

        vcsModeChanged = true;
    }

    ResourceId commitStatus = pVcsFabricLocalObjectManager->commitTransaction ();

    if (FRAMEWORK_SUCCESS != commitStatus)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsClusterConfigHandler: Failed to commit transaction.");

        return (WAVE_MESSAGE_ERROR);
    }

    if ((pDcmVcsClusterConfigData->getClusterConf () != localClusterConf) && ((pDcmVcsClusterConfigData->getClusterConf () == 1) || (pDcmVcsClusterConfigData->getClusterConf () == 0)))
	{
        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabricLocalObjectManager::setVcsClusterConfigHandler: need to set cluster conf to ") + pDcmVcsClusterConfigData->getClusterConf ());

        ResourceId completionStatus = pVcsFabricLocalObjectManager->setClusterConfFile(pDcmVcsClusterConfigData->getClusterConf ());

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsClusterConfigHandler: Failed to cluster conf.");
        }
        else
        {
            clusterConfChanged = true;
        }
    }

    if (rbridgeIdChanged)
    {
        // Set rbridge id backend
        ResourceId status = pVcsFabricLocalObjectManager->setRBridgeIdBackend (pDcmVcsClusterConfigData->getRbridgeId ());

		if ( status == WAVE_MESSAGE_ERROR )
		{
			WaveNs::trace (TRACE_LEVEL_ERROR,"VcsFabricLocalObjectManager::setVcsClusterConfigHandler: Failed to set backend RbridgeId ");

			return (status);
		}
    }

    if (vcsIdChanged)
    {
        
        if (pVcsFabricLocalObjectManager->setVcsConfFile (pDcmVcsClusterConfigData->getVcsMode (), pDcmVcsClusterConfigData->getVcsId ()) != WAVE_MESSAGE_SUCCESS)
        {
            // If the operation fails, then return error status and the entire HA sync fails 
            WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsFabricLocalObjectManager::setVcsClusterConfigHandler: Error in accessing vcs configuration file"));

            return (WAVE_MESSAGE_ERROR);
        }

         raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1001,  localVcsId, pDcmVcsClusterConfigData->getVcsId ());
    }

    // Cases that require a reboot/restart
    if (vcsModeChanged)
    {
		string retMessage = "";
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		ResourceId rebootLocalNodeStatus = WAVE_MESSAGE_SUCCESS;
        bool vcsMode = pDcmVcsClusterConfigData->getVcsMode ();

        // Trigger standby only reboot if the context is sync dump
        if ((context == SYNC_DUMP_CONTEXT) || (context == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT))
        {
		    status = pVcsFabricLocalObjectManager->copyDefaultToStartInModeChange (vcsMode, true, retMessage);
            rebootLocalNodeStatus = pVcsFabricLocalObjectManager->rebootLocalNode (true);
        }
        else
        {
		    status = pVcsFabricLocalObjectManager->copyDefaultToStartInModeChange (vcsMode, true, retMessage);
        }

		if (WAVE_MESSAGE_SUCCESS != status || WAVE_MESSAGE_SUCCESS != rebootLocalNodeStatus)
		{
			WaveNs::trace (TRACE_LEVEL_ERROR,"VcsFabricLocalObjectManager::setVcsClusterConfigHandler:" + retMessage);
		}
	}
    // Included clusterConfChanged to be consistent with vcsId/rbridgeId change
    // The issue is observed when cluster mode transition(MC or FC) is done on a dual MM chassis.
	else if (vcsIdChanged || rbridgeIdChanged || clusterConfChanged)
	{
        bool vcsMode = pDcmVcsClusterConfigData->getVcsMode ();
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        ResourceId rebootLocalNodeStatus = WAVE_MESSAGE_SUCCESS;
        string retMessage = "";

        // Trigger standby only reboot if the context is sync dump
        if ((context == SYNC_DUMP_CONTEXT) || (context == SYNC_DUMP_WITH_PAUSE_DB_CONTEXT))
        {
            if (vcsMode)
            {
                status = pVcsFabricLocalObjectManager->copyDefaultToStartInModeChange (vcsMode, false, retMessage);
                rebootLocalNodeStatus = pVcsFabricLocalObjectManager->rebootLocalNode (true);
            }
            else 
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsClusterConfigHandler : Since vcs mode is disabled, we cannot trigger copydefault and reboot");
            }
        }
        else
        {
            if (vcsMode)
            {
                status = pVcsFabricLocalObjectManager->copyDefaultToStartInModeChange (vcsMode, false, retMessage);
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsClusterConfigHandler : Since vcs mode is disabled, we cannot trigger copydefault and reboot");
            }
        }

        if (WAVE_MESSAGE_SUCCESS != status || WAVE_MESSAGE_SUCCESS != rebootLocalNodeStatus)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setVcsClusterConfigHandler: CopyDefaultStartTrigger completed with error : " + retMessage);
        }
	}

    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::getVcsClusterConfigHandler (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context)
{
    VcsFabricLocalObjectManager * pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance();

	pDcmVcsClusterConfigData->setVcsId (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsId ());
	pDcmVcsClusterConfigData->setVcsMode (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsMode ());
    pDcmVcsClusterConfigData->setRbridgeId (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectDomainId ());
    pDcmVcsClusterConfigData->setClusterConf (pVcsFabricLocalObjectManager->parseClusterConfFile ());
}

void VcsFabricLocalObjectManager::haSyncCollectValidationData (WaveObjectManagerHaSyncCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::haSyncCollectValidationData : Entering ...");

    /*
    VersionInformation versionInfo;

    versionInfo.setFirmwareVersion (VcsFabricLocalObjectManager::getInstance ()->getFwVersion ());
    versionInfo.setHaVersion (0);

    pMessage->setValidationDetails ((void *) (&versionInfo), sizeof (versionInfo), false);
    */

    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void VcsFabricLocalObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::haboot : Entering ...");

    VcsNodeLocalManagedObject *pThisVcsNode = NULL;

    if (NULL != m_pVcsNodeLocalManagedObject ) 
    {
       trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::haboot: m_pVcsNodeLocalManagedObject is not NULL - this should not happen");

       prismAssert (false, __FILE__, __LINE__);
    }

    if (NULL == m_pVcsNodeLocalManagedObject ) 
    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

        synchronousQueryContext.addAndAttribute (new AttributeObjectId (FrameworkToolKit::getThisWaveNodeObjectId(), "ownerWaveNodeObjectId"));

        vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

        prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
        prismAssert (1 == (pWaveManagedObjects->size ()), __FILE__, __LINE__);

        if (NULL == (*pWaveManagedObjects)[0])
        {
            trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::haboot: DB query returned one object but pointer to the object is NULL - this should'n happen so assert");

            prismAssert (false, __FILE__, __LINE__);
        }

        pThisVcsNode = dynamic_cast<VcsNodeLocalManagedObject *> ((*pWaveManagedObjects)[0]);

        setThisVcsNodeLocalManagedObject(pThisVcsNode);

        pWaveManagedObjects->clear ();
        delete pWaveManagedObjects;
    }
    
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    // cache the boot reason code

    setThisVcsNodeBootReason (bootReason);

    ObjectId objectId = getThisVcsNodeLocalManagedObjectObjectId ();
    setThisVcsNodeObjectId (objectId);

    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::haboot : Object ID   : ") + objectId.toString ());

    WaveNs::trace (TRACE_LEVEL_SUCCESS, string ("VcsFabricLocalObjectManager::haboot : serial# = ") + getThisVcsNodeLocalManagedObjectSerialNumber ());

    startTransaction();
    setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (false); 

    if(LOCATION_SECONDARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole () || LOCATION_PRIMARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole ()) 
    {
        setThisVcsNodeLocalManagedObjectInVcs(false);
        setThisVcsNodeLocalManagedObjectIsPrincipal (false);
        setThisVcsNodeLocalManagedObjectStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD,VCS_FABRIC_LOCAL_SPECIFIC_STATUS_WAITING_TO_REJOIN);
    }

    updateThisVcsNodeLocalManagedObject ();

    ResourceId status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        WaveNs::trace (TRACE_LEVEL_SUCCESS, string ("VcsFabricLocalObjectManager::haboot : updated isPrincipal on this node, locationId = ") + FrameworkToolKit::getThisLocationId ());
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::haboot : Failed to update isPrincipal on this node, locationId = ") + FrameworkToolKit::getThisLocationId ());
    }

    bool isPrincipal = getThisVcsNodeLocalManagedObjectIsPrincipal ();

    tracePrintf(TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::haboot :isPrincipal: %u",isPrincipal);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

// This is only executed on the standby MM for CCMD.  Similar code for DCMD is executed after MM failover.  
void VcsFabricLocalObjectManager::habootCompleteEventHandler (const WaveNs::HaBootCompleteForThisLocationEvent *&pHaBootCompleteForThisLocationEvent)
{
    UI32 myDomainId = DcmToolKit::getThisVcsNodeDomainId ();

    if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        if (0 != VcsNodeFabosInterfaceObjectManager::getInstance ()->scnQueueSize ())
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::habootCompleteEventHandler: SCN queue is not empty."));

            prismAssert (false, __FILE__, __LINE__);
        }

        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::habootCompleteEventHandler: haboot complete event received my domainId = ") + myDomainId);
 
        m_fcPrincipalTrackingWorker->replaySCNQueueEvents ();
    }

    m_isNodeReadyForSlotScn = true;

    m_vcsNodeSlotManagementWorker->replaySlotScnQueueEvents ();

    reply (reinterpret_cast<const PrismEvent *&> (pHaBootCompleteForThisLocationEvent));
}

void VcsFabricLocalObjectManager::hainstall (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabricLocalObjectManager::hainstall : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)
    {
        m_vcsNodeSlotManagementWorker->clearAllLinecardHandler ();
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsFabricLocalObjectManager::vcsFabricDebugClientMessageHandler (VcsFabricDebugClientMessage *pVcsFabricDebugClientMessage)
{
    vector<UI32>                 domains;
    vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName ());

    prismAssert(NULL != pResults, __FILE__, __LINE__);

    UI32 numberOfNodes = pResults->size ();

    for (UI32 i = 0; i < numberOfNodes; i++)
    {
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject =  dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[i]);

        prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);

        domains.push_back (pVcsNodeLocalManagedObject->getDomainId ());
    }
    
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    pVcsFabricDebugClientMessage->setListOfDomainInCluster (domains);

    pVcsFabricDebugClientMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsFabricDebugClientMessage);
}

void VcsFabricLocalObjectManager::vcsFabricSetTtyNameMessageHandler (VcsFabricSetTtyNameMessage *pVcsFabricSetTtyNameMessage)
{
#if 0
    RunningConfigActionpointWorker::setTtyNameToPrintOutput (pVcsFabricSetTtyNameMessage->getNewTtyName ());
#endif

    pVcsFabricSetTtyNameMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsFabricSetTtyNameMessage);
}

void VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (const ResourceId &state)
{
    m_vcsFabricLocalObjectManagerStateMutex.lock ();

    m_vcsFabricLocalObjectManagerState = state;    

    m_vcsFabricLocalObjectManagerStateMutex.unlock ();
}

ResourceId VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()
{
    m_vcsFabricLocalObjectManagerStateMutex.lock ();

    ResourceId state = m_vcsFabricLocalObjectManagerState;

    m_vcsFabricLocalObjectManagerStateMutex.unlock ();

    return state;
}

void VcsFabricLocalObjectManager::setCachedVcsClusterGuid (const UI64 &vcsClusterGuid)
{
    m_vcsClusterGuidMutex.lock ();

    m_vcsClusterGuid = vcsClusterGuid;

    m_vcsClusterGuidMutex.unlock ();
}

ResourceId VcsFabricLocalObjectManager::getCachedVcsClusterGuid (UI64 &vcsClusterGuid)
{
    m_vcsClusterGuidMutex.lock ();

    vcsClusterGuid = m_vcsClusterGuid;

    m_vcsClusterGuidMutex.unlock ();

    return (WAVE_MESSAGE_SUCCESS);
}

void VcsFabricLocalObjectManager::vcsNodeGetPrincipalPriorityHandler (VcsNodeGetPrincipalPriorityMessage *pVcsNodeGetPrincipalPriorityMessage)
{
    pVcsNodeGetPrincipalPriorityMessage->setPrincipalPriority (getThisVcsNodeLocalManagedObjectPrincipalPriority ());
    pVcsNodeGetPrincipalPriorityMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsNodeGetPrincipalPriorityMessage);
}

void VcsFabricLocalObjectManager::vcsNodeSetPrincipalPriorityHandler (VcsNodeSetPrincipalPriorityMessage *pVcsNodeSetPrincipalPriorityMessage)
{

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    UI32 principalPriority = pVcsNodeSetPrincipalPriorityMessage->getPrincipalPriority ();
    UI32 mode              = 1; //Mode 1 is to set the principal priority and mode 2 is to trigger fabric reformation without changing the principalPriority.
    UI32 returnValue       = 0;

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::vcsNodeSetPrincipalPriorityMessage: Setting the principal priority value as ") + principalPriority);

    if (principalPriority)
    {
        //Invoke fabric API to set the principal priority.

        returnValue = fabPrinSelModeCmd (mode, principalPriority);
    }
    else
    {
        mode = 0; //To disable principal selection mode
        returnValue = fabPrinSelModeCmd (mode, principalPriority);
    }

    if (returnValue >= 0)
    {
        startTransaction();

        setThisVcsNodeLocalManagedObjectPrincipalPriority (principalPriority);

        updateThisVcsNodeLocalManagedObject ();

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status)
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::vcsNodeSetPrincipalPriorityMessage: Setting principal priority in the database failed");
            status = WAVE_MESSAGE_ERROR;
        }    
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::vcsNodeSetPrincipalPriorityMessage: Setting principal priority in the fabric failed  with return value = ") + returnValue + string (" mode = ") + mode + string (" priority = ") + principalPriority);
        //Setting principal priority failed in fabric. Return appropriate error.
        status = WAVE_MESSAGE_ERROR;
    }

    pVcsNodeSetPrincipalPriorityMessage->setCompletionStatus (status);

    reply (pVcsNodeSetPrincipalPriorityMessage);
}

void VcsFabricLocalObjectManager::vcsNodePrincipalTriggerFailoverHandler (VcsNodePrincipalTriggerFailover *pVcsNodePrincipalTriggerFailover)
{
    //Trigger principal failover with fabricd

    ResourceId status      = WAVE_MESSAGE_SUCCESS;
    UI32       lowestPriorityValue     = 0; 

    //Following will get the lowest principal priority value
    status = getPrincipalPriority (0, lowestPriorityValue);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        UI32 priorityOfCurrentPrincipal = 0;

        getPrincipalPriority (FrameworkToolKit::getClusterPrimaryLocationId (), priorityOfCurrentPrincipal);

        if (lowestPriorityValue != priorityOfCurrentPrincipal)
        {
            
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsNodePrincipalTriggerFailoverHandler: Triggering principal switchover");

            UI32 principalPriority = 0; //Dummy value, do not send anyother value here. During fabric reformation case, principalPriority should not be changed.
            UI32 mode              = 4; //Mode 4 is to trigger fabric reformation.
            UI32 returnValue       = 0;

            //Use fabric API to trigger fabric reformation.

            returnValue = fabPrinSelModeCmd (mode, principalPriority);

            if (returnValue < 0)
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::vcsNodePrincipalTriggerFailoverHandler: Could not trigger fabric reformation. fabric API returned error no ") + returnValue);
                //fabric reformation failed in fabric. Return appropriate error.
                status = WAVE_MESSAGE_ERROR;
            }

        }
        else
        {
            //Logical chassis principal same as the node with the lowest principal priority. Hence not triggering principal failover.
            status = VCS_PRINCIPAL_AND_PRIORITY_PRINCIPAL_MATCH;
        }

    }

    pVcsNodePrincipalTriggerFailover->setCompletionStatus (status);

    reply (pVcsNodePrincipalTriggerFailover);
}

void VcsFabricLocalObjectManager::resetDomainBitmapMessageHandler (VcsNodeResetDomainBitMapMessage *pVcsNodeResetDomainBitMapMessage)
{
    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::resetDomainBitmapMessageHandler "));

    m_isNodeReadyForSlotScn = false;

    m_fcPrincipalTrackingWorker->resetDomainBitmap ();

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {   
            // for both cold and warm recovery case reset config replay flag, as config replay complete event compelete will be triggered after cold and warm recovery. This message will come only when cold or warm recovery is happening after ha failover.

        m_isConfigReplayCompleted = false;
    }

    pVcsNodeResetDomainBitMapMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeResetDomainBitMapMessage);
}

void VcsFabricLocalObjectManager::enableSlotScnHandlingMessageHandler (VcsNodeEnableSlotScnHandlingMessage *pVcsNodeEnableSlotScnHandlingMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::enableSlotScnHandlingMessageHandler message handler");

    m_isNodeReadyForSlotScn = true;

    m_vcsNodeSlotManagementWorker->replaySlotScnQueueEvents ();

    pVcsNodeEnableSlotScnHandlingMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsNodeEnableSlotScnHandlingMessage);
}

void VcsFabricLocalObjectManager::prepareDomainListAfterHaRecoveryMessageHandler (VcsNodePrepareDomainListAfterHaRecoveryMessage *pVcsNodePrepareDomainListAfterHaRecoveryMessage)
{
    bool isWarmRecovery = pVcsNodePrepareDomainListAfterHaRecoveryMessage->getIsWarmRecoveryInvolved ();

    trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::prepareDomainListAfterHaRecoveryMessageHandler with warm recovery as ") + isWarmRecovery);

    m_fcPrincipalTrackingWorker->prepareDomainListAfterHaRecovery (isWarmRecovery);

    pVcsNodePrepareDomainListAfterHaRecoveryMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsNodePrepareDomainListAfterHaRecoveryMessage);
}

void VcsFabricLocalObjectManager::isCliAllowedFromThisLocationMessageHandler (VcsNodeIsCliAllowedFromThisLocationMessage *pVcsNodeIsCliAllowedFromThisLocationMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::isCliAllowedFromThisLocationMessageHandler: inside");

    pVcsNodeIsCliAllowedFromThisLocationMessage->setIsCliAllowedFromThisLocation(DcmToolKit::isCliAllowedAtThisLocation ());
  
    pVcsNodeIsCliAllowedFromThisLocationMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pVcsNodeIsCliAllowedFromThisLocationMessage);
}

void VcsFabricLocalObjectManager::isCliAllowedFromThisLocationStaticMessageHandler (VcsNodeIsCliAllowedFromThisLocationMessage *pVcsNodeIsCliAllowedFromThisLocationMessage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::isCliAllowedFromThisLocationStaticMessageHandler: inside");

    pVcsNodeIsCliAllowedFromThisLocationMessage->setIsCliAllowedFromThisLocation(DcmToolKit::isCliAllowedAtThisLocation ());

    pVcsNodeIsCliAllowedFromThisLocationMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

}
bool VcsFabricLocalObjectManager::triggerPrincipalSwitchoverBasedOnPrincipalPriority ()
{
    UI32       lowestPriorityValue           = 0; 
    bool       isPrincipalSwitchOverTriggred = false;

    //if we are not in MC mode dont perform anything.
    if (!((DCM_CCM == DcmToolKit::getDistributionMode ()) && (DcmToolKit::isVcsClusterEnabled ())))
    {
        return isPrincipalSwitchOverTriggred;
    }

    //Following will get the lowest principal priority value
    ResourceId status = getPrincipalPriority (0, lowestPriorityValue); 

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        //Case of no principalPriority set for any of the nodes in the cluster.
        return isPrincipalSwitchOverTriggred;
    }

    UI32 priorityOfCurrentPrincipal = 0;

    getPrincipalPriority (FrameworkToolKit::getThisLocationId (), priorityOfCurrentPrincipal);

    //Check if the lowest priority locationId is not same as principal locationId trigger principal switchover
    if (lowestPriorityValue != priorityOfCurrentPrincipal)
    {
        UI32 principalPriority = 0; //Dummy value, do not send anyother value here. During fabric reformation case, principalPriority should not be changed.
        UI32 mode              = 4; //Mode 4 is to trigger fabric reformation.
        UI32 returnValue       = 0;

        //Use fabric API to trigger fabric reformation.

        returnValue = fabPrinSelModeCmd (mode, principalPriority);

        if (returnValue < 0)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::triggerPrincipalSwitchoverBasedOnPrincipalPriority: Could not trigger fabric reformation. fabric API returned error no ") + returnValue);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabricLocalObjectManager::triggerPrincipalSwitchoverBasedOnPrincipalPriority: triggered fabric reformation "));
            isPrincipalSwitchOverTriggred = true;
        }
    }

    return (isPrincipalSwitchOverTriggred);
}

UI32 VcsFabricLocalObjectManager::setThisNodePrincipalPriorityInFabric ()
{
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::setThisNodePrincipalPriorityInFabric: Inside");

    UI32 returnValue       = 0;
    UI32 principalPriority = 0;
    UI32 mode              = 0; //Mode 0 is to disable principal selection 

    //If we are in MC mode and if principal priority is non zero value, enable principal selection and set the principal priority in the fabric
    if ((DCM_CCM == DcmToolKit::getDistributionMode ()) && (DcmToolKit::isVcsClusterEnabled ()))
    {
        getPrincipalPriority (FrameworkToolKit::getThisLocationId (), principalPriority);

        if (principalPriority)
        {
            mode = 1; //Mode 1 is to set the principal priority and mode 2 is to trigger fabric reformation without changing the principalPriority.

            //Invoke fabric API to set the principal priority.

            returnValue = fabPrinSelModeCmd (mode, principalPriority);
        }
        else
        {
            //Since principal priority is zero, disable principal selection
            returnValue = fabPrinSelModeCmd (mode, principalPriority);
        }

    }
    else
    {
        //If we are not in MC mode disable principal selection in the fabric
        returnValue = fabPrinSelModeCmd (mode, principalPriority);
    }


    if (returnValue < 0)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabricLocalObjectManager::setThisNodePrincipalPriorityInFabric: Setting principal priority in the fabric failed with return value = ") + returnValue + string (" mode = ") + mode + string (" priority = ") + principalPriority);
    }

    return (returnValue);
}

/*
 * Gets the principal priority of a given locationId. If the locationId is zero it gets the lowest principal priority
 *
 */
ResourceId VcsFabricLocalObjectManager::getPrincipalPriority (UI32 locationId, UI32 &principalPriority)
{

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsNodeLocalManagedObject::getClassName ());
    synchronousQueryContext.addAndAttribute  (new AttributeUI32 ((UI32)0, "principalPriority"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);
    synchronousQueryContext.addSelectField ("principalPriority");

    if (!locationId)
    {
        //If location id is not set get the lowest principal priority value
        synchronousQueryContext.addOrderField ("principalPriority"); //ascending order
        synchronousQueryContext.addLimitForQueryResults (1);
    }
    else
    {
        //Get the principal priority value for the given locationId
        synchronousQueryContext.addAndAttribute  (new AttributeUI32 (locationId, "locationId"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL); 
    }

    vector<WaveManagedObject *>   *pResults = querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32 count = pResults->size ();

    if (0 == count)
    {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        //Principal priority not set for any of the node and hence return
        return VCS_PRINCIPAL_PRIORITY_NOT_SET_FOR_SWITCHOVER;
    }

    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject*> ((*pResults)[0]);
    prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);
    principalPriority = pVcsNodeLocalManagedObject->getPrincipalPriority ();

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);

    return WAVE_MESSAGE_SUCCESS;

}

void VcsFabricLocalObjectManager::addToWWNRbridgeIdMapCache (const WorldWideName &wwn, const UI32 rbridgeId)
{
    m_WWNRbridgeIdMapCacheMutex.lock ();

    m_WWNRbridgeIdMapCache.insert (pair<string, UI32> (wwn.toString (), rbridgeId));

    m_WWNRbridgeIdMapCacheMutex.unlock ();    
}

void VcsFabricLocalObjectManager::deleteFromWWNRbridgeIdMapCache (const WorldWideName &wwn)
{
    m_WWNRbridgeIdMapCacheMutex.lock ();

    m_WWNRbridgeIdMapCache.erase (wwn.toString ());

    m_WWNRbridgeIdMapCacheMutex.unlock ();
}

void VcsFabricLocalObjectManager::setWWNRbridgeIdMapCache (const map <string, UI32> &wwnRbridgeIdMap)
{
    m_WWNRbridgeIdMapCacheMutex.lock ();

    m_WWNRbridgeIdMapCache = wwnRbridgeIdMap;

    m_WWNRbridgeIdMapCacheMutex.unlock ();
}

void VcsFabricLocalObjectManager::getWWNRbridgeIdMapCache (map <string, UI32> &wwnRbridgeIdMap)
{
    m_WWNRbridgeIdMapCacheMutex.lock ();

    wwnRbridgeIdMap = m_WWNRbridgeIdMapCache;

    m_WWNRbridgeIdMapCacheMutex.unlock ();
}

void VcsFabricLocalObjectManager::clearWWNRbridgeIdMapCache ()
{
    m_WWNRbridgeIdMapCacheMutex.lock ();

    m_WWNRbridgeIdMapCache.clear ();

    m_WWNRbridgeIdMapCacheMutex.unlock ();
}

void VcsFabricLocalObjectManager::updateWWNRbridgeIdMapCache ()
{
    map <string, UI32> wwnRbridgeIdMap;

    VcsFabricLocalObjectManager::getWWNRbridgeIdMapFromDB (wwnRbridgeIdMap);    

    if (0 != wwnRbridgeIdMap.size ())
    {
        setWWNRbridgeIdMapCache (wwnRbridgeIdMap);
    }
}

void VcsFabricLocalObjectManager::getWWNRbridgeIdMapFromDB (map <string, UI32> &wwnRbridgeIdMap)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    synchronousQueryContext.addSelectField ("switchWWN");
    synchronousQueryContext.addSelectField ("domainId");
    vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously (&synchronousQueryContext);

    wwnRbridgeIdMap.clear ();

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::updateWWNRbridgeIdMapCache: Query of VcsNodeLocalMO's returned 0 results.");
        }
        else
        {
            for (UI32 i = 0; i < numberOfResults; i++)
            {
                VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[i]);

                if (NULL == pVcsNodeLocalManagedObject)
                {
                    WaveNs::trace (TRACE_LEVEL_FATAL, "VcsFabricLocalObjectManager::updateWWNRbridgeIdMapCache: Managed object results failed to be dynamic casted to a VcsNodeLocalManagedObject.");

                    WaveNs::prismAssert (false, __FILE__, __LINE__);
                }
                
                wwnRbridgeIdMap [(pVcsNodeLocalManagedObject->getSwitchWWN ()).toString ()] = pVcsNodeLocalManagedObject->getDomainId ();
            }

        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }

}

void  VcsFabricLocalObjectManager::vcsIsGvlanCapableMessageHandler(VcsNodeIsGvlanCapableMessage *pVcsNodeIsGvlanCapableMessage)
{
    ResourceId   status = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsIsGvlanCapableMessageHandler: Inside");

    /* Compute if GVLAN is capable and set. */
    bool gvlanCapable = DcmToolKit::isSwitchGvlanCapable ();
    pVcsNodeIsGvlanCapableMessage->setGvlanCapable (gvlanCapable);

    status = WAVE_MESSAGE_SUCCESS;

    pVcsNodeIsGvlanCapableMessage->setCompletionStatus (status);
    reply (pVcsNodeIsGvlanCapableMessage);
}

void  VcsFabricLocalObjectManager::vcsIsGvlanCapableStaticMessageHandler(VcsNodeIsGvlanCapableMessage *pVcsNodeIsGvlanCapableMessage)
{
    ResourceId   status = WAVE_MESSAGE_ERROR;
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsIsGvlanCapableStaticMessageHandler: Inside");

    /* Compute if GVLAN is capable and set. */
    bool gvlanCapable = DcmToolKit::isSwitchGvlanCapable ();
    pVcsNodeIsGvlanCapableMessage->setGvlanCapable (gvlanCapable);

    status = WAVE_MESSAGE_SUCCESS;

    pVcsNodeIsGvlanCapableMessage->setCompletionStatus (status);
}

void  VcsFabricLocalObjectManager::vcsIsGvlanEnabledMessageHandler(VcsNodeIsGvlanEnabledMessage *pVcsNodeIsGvlanEnabledMessage)
{
    ResourceId   status = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsIsGvlanEnabledMessageHandler: Inside");

    /* Compute if GVLAN is enabled and set. The dummy and default implementation setting it to false always */
    pVcsNodeIsGvlanEnabledMessage->setGvlanEnabled (false);

    status = WAVE_MESSAGE_SUCCESS;

    pVcsNodeIsGvlanEnabledMessage->setCompletionStatus (status);
    reply (pVcsNodeIsGvlanEnabledMessage);
}

void  VcsFabricLocalObjectManager::vcsIsGvlanEnabledStaticMessageHandler(VcsNodeIsGvlanEnabledMessage *pVcsNodeIsGvlanEnabledMessage)
{
    ResourceId   status = WAVE_MESSAGE_ERROR;
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::vcsIsGvlanEnabledStaticMessageHandler: Inside");

    /* Compute if GVLAN is enabled and set. The dummy and default implementation setting it to false always */
    pVcsNodeIsGvlanEnabledMessage->setGvlanEnabled (false);

    status = WAVE_MESSAGE_SUCCESS;

    pVcsNodeIsGvlanEnabledMessage->setCompletionStatus (status);
}
}
