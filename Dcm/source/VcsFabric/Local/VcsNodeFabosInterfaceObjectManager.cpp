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
 *   Copyright (C) 2010-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/


#include "VcsFabric/Local/EarlyConfigurationReplayWorker.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeDomainIdMessages.h"
#include "VcsFabric/Local/VcsNodeFabosSlotEvents.h"
#include "VcsFabric/Local/VcsNodeFabosSlotScnQueueObject.h"
#include "VcsFabric/Local/VcsNodeSlotMessages.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/OsLayer/PrismOsLayer.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManagerHaSyncWorker.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/Wave.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerEnableTransactionMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerLiveSyncEnableMessages.h"
#include "Framework/DistributedLog/DistributedLogUpdateLogIdMessage.h"
#include "Cluster/PrismCluster.h"
#include "Cluster/Local/WaveHaNodeTypes.h"
#include "Cluster/Local/ClusterLocalSetHaRoleMessage.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "DcmShell/DcmShell.h"
#include "Shell/ShellPrism.h"
#include "Shell/ShellDebug.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "Nsm/Local/ConnectorConfigLocalManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"
#include "utils/node_id.h"
#include "utils/slot.h"
#include "utils/mqueue.h"
#include <hasm/hasm.h>
#include "giot/giot.h"
#include "dcm/ipc.h"
#include "raslog/raslog_dcm.h"

namespace DcmNs
{

static  fssd_ops_t dcm_chassis_fssd_ops;
static  fssd_reg_t dcm_chassis_fss;

static  fssd_ops_t dcm_fcsw_fssd_ops;
static  fssd_reg_t dcm_fcsw_fss;

static  fssd_ops_t dcm_ethsw_fssd_ops;
static  fssd_reg_t dcm_ethsw_fss;

static  scid_t  dcm_cid;
static  smi_reg_t dcm_smi;
#if 0
static  said_t  dcm_said_fc;
static  said_t  dcm_said_eth;
#endif

static  isc_ops_t dcm_isc_ops;
static  isc_reg_t dcm_isc_reg;

static  map<int, string> dcmChassisFssStages;
static  map<int, string> dcmFcswFssStages;
static  map<int, string> dcmEthswFssStages;

static	dcmConfigFssSyncData_t	dcmConfigFssSyncData = {0};

mq_t            VcsNodeFabosInterfaceObjectManager::m_VcsMsgQ;
sysModHandle_t* VcsNodeFabosInterfaceObjectManager::m_sysmodh;
bool VcsNodeFabosInterfaceObjectManager::m_isDcmdReadyForHasmLaunch;
bool VcsNodeFabosInterfaceObjectManager::m_isHasmLaunched;
bool VcsNodeFabosInterfaceObjectManager::m_isConfigInitDone = false;
int VcsNodeFabosInterfaceObjectManager::m_fssHandles[] = {-1, -1, -1};
int VcsNodeFabosInterfaceObjectManager::m_iscHandle = -1;
bool VcsNodeFabosInterfaceObjectManager::m_syncEnabled = false;
bool VcsNodeFabosInterfaceObjectManager::m_isActiveNode = false;
bool VcsNodeFabosInterfaceObjectManager::m_issuInProgress = false;
UI32 VcsNodeFabosInterfaceObjectManager::m_linecardState[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int VcsNodeFabosInterfaceObjectManager::m_linecardActivePartitionId[] = {LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID,
                  LINECARD_PARTITION_INVALID, LINECARD_PARTITION_INVALID};
PrismMutex VcsNodeFabosInterfaceObjectManager::m_linecardActivePartitionIdMutex;
PrismMutex VcsNodeFabosInterfaceObjectManager::m_issuInProgressMutex;
PrismMutex VcsNodeFabosInterfaceObjectManager::m_syncEnabledMutex;

EarlyConfigurationReplayWorker *VcsNodeFabosInterfaceObjectManager::m_earlyConfigurationReplayWorker;

VcsNodeFabosInterfaceObjectManager::VcsNodeFabosInterfaceObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks (getClassName ())
{
    setAllowAutomaticallyUnlistenForEvents (false);

    addEventType    (VCS_NODE_FABOS_DOMAIN_VALID);
    addEventType    (VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE);
    addEventType    (VCS_NODE_FABOS_DOMAIN_INVALID);
    addEventType    (VCS_NODE_FABOS_DOMAIN_REACHABLE);
    addEventType    (VCS_NODE_FABOS_DOMAIN_UNREACHABLE);
    addEventType    (VCS_NODE_FABOS_SLOT_ONLINE);
    addEventType    (VCS_NODE_FABOS_SLOT_OFFLINE);
    addEventType    (VCS_NODE_FABOS_SWITCH_OFFLINE);
    addEventType    (VCS_NODE_FABOS_ZONE_CHANGED);
    addEventType    (VCS_NODE_FABOS_ZONE_ABORTED);
    addEventType    (VCS_NODE_FABOS_SWITCH_READY_TO_ACCEPT_COMMANDS);
    addEventType    (VCS_NODE_FABOS_SWITCH_ACTIVATION_COMPLETE);
    addDebugFunction ((ShellCmdFunction) (&VcsNodeFabosInterfaceObjectManager::fssStageDebugMessageHandler), "fssstagenumber");
    addEventType    (VCS_NODE_FABOS_PORT_DISABLE);
    addEventType    (VCS_NODE_FABOS_SYNC_START);
    addEventType    (VCS_NODE_FABOS_SERVICE_READY);
    addEventType    (VCS_NODE_FABOS_LINECARD_READY);
    addEventType    (VCS_NODE_FABOS_SYSTEM_READY);

	initializeSCNQueue ();
    m_isDcmdReadyForHasmLaunch = false;
    m_isHasmLaunched = false;
    m_fwVersion = "";
    m_swModel   = 0;
    m_earlyConfigurationReplayWorker = new EarlyConfigurationReplayWorker (this);
    m_isWarmRecoveryInvolvedInActivation = false;
}

VcsNodeFabosInterfaceObjectManager::~VcsNodeFabosInterfaceObjectManager ()
{
}

string VcsNodeFabosInterfaceObjectManager::getClassName ()
{
    return ("VCS Node Init Services");
}

VcsNodeFabosInterfaceObjectManager *VcsNodeFabosInterfaceObjectManager::getInstance ()
{
    static VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = new VcsNodeFabosInterfaceObjectManager ();

    WaveNs::prismAssert (NULL != pVcsNodeFabosInterfaceObjectManager, __FILE__, __LINE__);

    return (pVcsNodeFabosInterfaceObjectManager);
}

void VcsNodeFabosInterfaceObjectManager::initializeSCNQueue ()
{
	m_scnQueue.clear ();
	m_scnQueueMutex.unlock();
}

PrismServiceId VcsNodeFabosInterfaceObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}


PrismEvent *VcsNodeFabosInterfaceObjectManager::createEventInstance (const UI32 &eventOperationCode)
{
    PrismEvent *pPrismEvent = NULL;

    switch (eventOperationCode)
    {
        case VCS_NODE_FABOS_DOMAIN_VALID:
            pPrismEvent = new VcsNodeFabosDomainValidEvent ();
            break;

        case VCS_NODE_FABOS_DOMAIN_INVALID:
            pPrismEvent = new VcsNodeFabosDomainInvalidEvent ();
            break;

        case VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE:
            pPrismEvent = new VcsNodeFabosDomainCapabilitiesAvailableEvent ();
            break;

        case VCS_NODE_FABOS_DOMAIN_REACHABLE:
            pPrismEvent = new VcsNodeFabosDomainReachableEvent ();
            break;

        case VCS_NODE_FABOS_DOMAIN_UNREACHABLE:
            pPrismEvent = new VcsNodeFabosDomainUnreachableEvent ();
            break;

        case VCS_NODE_FABOS_ZONE_CHANGED:
            pPrismEvent = new VcsNodeFabosZoneChangedEvent ();
            break;

        case VCS_NODE_FABOS_ZONE_ABORTED:
            pPrismEvent = new VcsNodeFabosZoneAbortedEvent ();
            break;

        case VCS_NODE_FABOS_PORT_DISABLE:
            pPrismEvent = new VcsNodeFabosSwitchPortDisableEvent ();
            break;
        case VCS_NODE_FABOS_SLOT_ONLINE:
            pPrismEvent = new VcsNodeFabosSlotOnlineEvent ();
            break;

        case VCS_NODE_FABOS_SLOT_OFFLINE:
            pPrismEvent = new VcsNodeFabosSlotOfflineEvent ();
            break;

        case VCS_NODE_FABOS_SWITCH_OFFLINE:
            pPrismEvent = new VcsNodeFabosSwitchOfflineEvent ();
            break;

        case VCS_NODE_FABOS_SWITCH_READY_TO_ACCEPT_COMMANDS:
            pPrismEvent = new VcsNodeFabosSwitchReadyToAcceptCommandsEvent ();
            break;

        case VCS_NODE_FABOS_SWITCH_ACTIVATION_COMPLETE:
            pPrismEvent = new VcsNodeFabosSwitchActivationCompleteEvent ();
            break;

        case VCS_NODE_FABOS_SYNC_START:
            pPrismEvent = new VcsNodeFabosSyncStartEvent ();
            break;

		case VCS_NODE_FABOS_SERVICE_READY:
            pPrismEvent = new VcsNodeFabosServiceReadyEvent ();
            break;

  	    case VCS_NODE_FABOS_LINECARD_READY:
            pPrismEvent = new VcsNodeFabosLinecardReadyEvent ();
            break;

        case VCS_NODE_FABOS_SYSTEM_READY:
            pPrismEvent = new VcsNodeFabosSystemReadyEvent ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("VcsNodeFabosInterfaceObjectManager::createEventInstance : Unkown Event Operation Code : ") + eventOperationCode);
            prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismEvent);
}

void VcsNodeFabosInterfaceObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    // Since we told framework not to unlistenEvents, we must explicitly unlisten for events since we are going to go into an infinite while loop.

    unlistenEvents ();


    SI32                rc              = 0;
    UI8                 tmpMacAddr[6];
    fabSysParam_t       system_param;
    UI32                waitCount = 0;
    giot_t              *gio = NULL;
    ioss_t              *ipcioss = NULL;

    
    // If this Deamon is Ccmd, then unconfigure this node first before declaring boot complete
    if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        string ipAddress            = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
        SI32 port                   = FrameworkToolKit::getPortForLocationId (FrameworkToolKit::getThisLocationId ());
        ResourceId messageStatus    = WAVE_MESSAGE_ERROR;
        ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

        FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);

        messageStatus = sendSynchronously (pMessage);

        if (WAVE_MESSAGE_SUCCESS != messageStatus)
        {
            trace (TRACE_LEVEL_ERROR, "Unconfigure Self Node Fails");
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "Completion of Unconfigure Self Node Fails");
            }
        }

        delete pMessage;
    }
    
    m_isDcmdReadyForHasmLaunch = true; 

    trace (TRACE_LEVEL_INFO, "Dcmd Ready For Hasm Launch is true");

    trace (TRACE_LEVEL_INFO, "Ready to move forward since HASM has been launched");

    // Create a giot handle.
    rc = giot_create(&gio);
    if (rc < 0) 
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler giot init failed: " + rc);
        giot_free(&gio);
        ipcExit();
        trace (TRACE_LEVEL_INFO, string("Cannot initialized IPC. Giving up"));
        prismAssert(false, __FILE__, __LINE__);
        return;
    }

    rc = 1;
    waitCount = 0;
    while (0 != rc)
    {
        if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            rc = ipc_ioss_create (&ipcioss, DCMD_IPC_DEST_NAME, DCMD_IPC_DEST_INSTANCE, MAX_MSG_SIZE, 1);
        }
        else if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (CCM_NOOP == DcmToolKit::getDistributionMode ()))
        {
            rc = ipc_ioss_create (&ipcioss, CCMD_IPC_DEST_NAME, CCMD_IPC_DEST_INSTANCE, MAX_MSG_SIZE, 1);
        }
        else if (DCM_NSP == DcmToolKit::getDistributionMode ())
        {
            rc = ipc_ioss_create (&ipcioss, NSP_IPC_DEST_NAME, NSP_IPC_DEST_INSTANCE, MAX_MSG_SIZE, 1);
        }

        if (rc < 0) 
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler ipc_ioss_create failed: " + rc);
            if (ipcioss != NULL) 
            {
                ipc_ioss_free(&ipcioss);
                giot_free(&gio);
                ipcExit();
            }
            trace (TRACE_LEVEL_INFO, string("Cannot initialized IPC. Giving up"));
            prismAssert(false, __FILE__, __LINE__);
            return;
        }

        // Register IPC handles with giot handle, and spawn off giot in a separate worker thread
        rc = giot_register(gio, ipcioss);

        if (rc < 0) 
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler giot_register failed: " + rc);
            if (ipcioss != NULL) 
            {
                giot_unregister(gio, ipcioss);
                ipc_ioss_free(&ipcioss);
                giot_free(&gio);
                ipcExit();
            }
            trace (TRACE_LEVEL_INFO, string("Cannot initialized IPC. Giving up"));
            prismAssert(false, __FILE__, __LINE__);
            return;
        }

        if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            // Dcmd call back registration
            rc = ipcRegisterCBlw (IPC_QUERY_SLOT, dcmIpcCallback, NULL);
        }

        printf("****************return ipcRegisterCBlw %d\n", rc);
        if (0 == rc)
        {
            trace (TRACE_LEVEL_INFO, "\nInitialized IPC. Proceeding ...");
            break;
        }

        printf("initializing IPC \n");
        prismSleep(1);
        waitCount++;
        trace (TRACE_LEVEL_INFO, string ("Still initializing IPC. Elapsed seconds : ") + waitCount);

        if (30 <= waitCount)
        {
            trace (TRACE_LEVEL_INFO, string("Cannot initialized IPC. Giving up"));
            prismAssert(false, __FILE__, __LINE__);
            return;
        }
    }
//    if (raslogm_init()) {
//       tracePrintf(TRACE_LEVEL_INFO, (char *) "raslogm_init fail");
//    }

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        if (raslog_init((char *) CHASSIS_SVC_NAME, 0, (char *) "dcmd"))
        {
            tracePrintf(TRACE_LEVEL_INFO, (char *) "raslog_init fail");
        }

	if (pBootCompleteForThisLocationEvent->needLogAfterSchemaChange ())
	{
		// Raslog that schema has changed and config has reinitialized to default
		trace (TRACE_LEVEL_INFO, "Configuration change detected. RASLOG the event for manual replay of configuration.");
	    	//raslog_ext("VcsNodeFabosInterfaceOM", __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_1008);
	}
    }
    else if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (CCM_NOOP == DcmToolKit::getDistributionMode ()))
    {
        if (raslog_init((char *) CHASSIS_SVC_NAME, 0, (char *) "ccmd"))
        {
            tracePrintf(TRACE_LEVEL_INFO, (char *) "raslog_init fail");
        }
    }
    else if (DCM_NSP == DcmToolKit::getDistributionMode ())
    {
        if (raslog_init((char *) CHASSIS_SVC_NAME, 0, (char *) "nspd"))
        {
            tracePrintf(TRACE_LEVEL_INFO, (char *) "raslog_init fail");
        }
    }

    rc = sysModInit(&m_sysmodh);

    if ((0 != rc) || (NULL == m_sysmodh))
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    /****************************************************************************
     *  Get the switch platform number 
     ***************************************************************************/
    //  sysModGetSwModel(m_sysmodh, &m_swModel); Get SwBD from DcmToolKit
    m_swModel = DcmToolKit::getSwBDModel();

    trace(TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler :  SwitchBD = ") + m_swModel);
    
    VcsFabricLocalObjectManager::setThisVcsNodeSWBDModel (m_swModel);

    /****************************************************************************
     *  Get the switch mac
     ***************************************************************************/
    rc = sysCtrlGetSystemParameters(m_sysmodh, &system_param);

    WaveNs::prismAssert (0 == rc, __FILE__, __LINE__);

    memcpy(tmpMacAddr, &system_param.mac_pool.MACaddressMS, 6);

	m_switchMac = MacAddress (tmpMacAddr);

	trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler :  SwitchMac = " +
	m_switchMac.toString());


    /****************************************************************************
     *  Get the insistent domain id
     ***************************************************************************/
    if (false == getIsConfigInitDone ())
    {
        configInit ();
        m_isConfigInitDone = true;
    }

    m_domainIdMutex.lock ();
    if (configGet("fabric.domain", ResourceInt, &m_domainId) != 0)
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::Insistent domain Id not set");
       m_domainId = 1;
    }
    m_domainIdMutex.unlock ();

    trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler :  domainId = ") +
    m_domainId);

    /****************************************************************************
     *  Get Firmware Version
     ***************************************************************************/
    fw_version_info_t *pfwInfo = (fw_version_info_t *) calloc (1, sizeof (fw_version_info_t)); /// calloc

    if (NULL == pfwInfo)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler: calloc returned NULL pointer.");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    int fwVersionRet = get_fw_version_info (pfwInfo);

    if (0 == fwVersionRet)
    {
        m_fwVersion = string (pfwInfo->full_version);
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler: Firmware Version : ") + m_fwVersion);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::bootCompleteForThisLocationEventHandler: get_fw_version_info returned ") + fwVersionRet);
    }

    free (pfwInfo); /// free

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        if (0 < (rc = asp_init((char *) "dcm", ASP_FLG_SINGLE_THREAD | ASP_FLG_FCSWITCH | ASP_FLG_SYSMOD, vcsNodeAspInit, vcsNodeAspExit)))
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "----->VcsNodeFabosInterfaceObjectManager::install: Failed");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }
    else if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (CCM_NOOP == DcmToolKit::getDistributionMode ()))
    {
        if (0 < (rc = asp_init((char *) "ccm", ASP_FLG_SINGLE_THREAD | ASP_FLG_FCSWITCH | ASP_FLG_SYSMOD, vcsNodeAspInit, vcsNodeAspExit)))
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "----->VcsNodeFabosInterfaceObjectManager::install: Failed");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }
    else if (DCM_NSP == DcmToolKit::getDistributionMode ())
    {
        if (0 < (rc = asp_init((char *) "nsp", ASP_FLG_SINGLE_THREAD | ASP_FLG_FCSWITCH | ASP_FLG_SYSMOD, vcsNodeAspInit, vcsNodeAspExit)))
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "----->VcsNodeFabosInterfaceObjectManager::install: Failed");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));

    asp_main (vcsNodeServe);

    WaveNs::trace (TRACE_LEVEL_FATAL, "----->VcsNodeFabosInterfaceObjectManager: Exited..");
    WaveNs::prismAssert (false, __FILE__, __LINE__);

    asp_exit();

    // clean up ipc
    if (ipcioss != NULL) 
    {
        giot_unregister(gio, ipcioss);
        ipc_ioss_free(&ipcioss);
        giot_free(&gio);
        ipcExit();
    }
}

ResourceId VcsNodeFabosInterfaceObjectManager::fssStageDebugMessageHandler (UI32 argc, vector<string> argv)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::fssStageDebugMessageHandler : Entering ....");

    if (argc > 2)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::fssStageDebugMessageHandler : More arguments than expected ....");
        return (0);
    }

    int stage = atoi (argv[1].c_str());

    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

    FrameworkObjectManagerStartExternalStateSynchronizationMessage *pFrameworkObjectManagerStartExternalStateSynchronizationMessage = new FrameworkObjectManagerStartExternalStateSynchronizationMessage (stage, FCSW);

    WaveMessageStatus status = pVcsNodeFabosInterfaceObjectManager->sendOneWay (pFrameworkObjectManagerStartExternalStateSynchronizationMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmRecover: One way message not successful from DCM_FSS_STAGE1");
    }

    return (0);
}

void VcsNodeFabosInterfaceObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    /****************************************************************************
     *  Get the insistent domain id
     ***************************************************************************/
    configInit ();

    m_isConfigInitDone = true;

    m_domainIdMutex.lock ();
    if (configGet("fabric.domain", ResourceInt, &m_domainId) != 0)
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::Insistent domain Id not set");
       m_domainId = 1;
    }
    m_domainIdMutex.unlock ();

    UI32 domainId = (UI32) getDomainId ();
    
    VcsNodeSetDomainIdMessage *pVcsNodeSetDomainIdMessage = new VcsNodeSetDomainIdMessage (domainId);

    ResourceId status = sendSynchronously (pVcsNodeSetDomainIdMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::install: send to VcsFabricLocal service failed");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        ResourceId completionStatus = pVcsNodeSetDomainIdMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::install: Message completed with error");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete (pVcsNodeSetDomainIdMessage);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsNodeFabosInterfaceObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	SI32 rc = 0;
	WaveNs::tracePrintf(TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::initialize(%d): libInit Called;\n", __LINE__);
	libInit();
	rc = secPolicyInit();
	if (rc)
	{
		WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::secpolicy init failed\n");
	}

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        dcmChassisFssStages[DCM_FSS_STAGE1]               = string("DCM_FSS_STAGE1");
        dcmChassisFssStages[DCM_FSS_STAGE2]               = string("DCM_FSS_STAGE2");
        dcmChassisFssStages[DCM_FSS_STAGE_FCOE_PLUGIN]    = string("DCM_FSS_STAGE_FCOE_PLUGIN");
        dcmChassisFssStages[DCM_FSS_STAGE4_AAA_PLUGIN_CH] = string("DCM_FSS_STAGE4_AAA_PLUGIN");
        dcmChassisFssStages[DCM_WARM_PREPARE_PLUGINS]     = string("DCM_WARM_PREPARE_PLUGINS");

        dcmFcswFssStages[DCM_FSS_STAGE1]                  = string("DCM_FSS_STAGE1");
        dcmFcswFssStages[DCM_FSS_STAGE2]                  = string("DCM_FSS_STAGE2");
        dcmFcswFssStages[SNMPD_CONFIG_PUSH]               = string("SNMPD_CONFIG_PUSH");
        dcmFcswFssStages[TSD_FSS_STAGE4]                  = string("TSD_FSS_STAGE4");
        dcmFcswFssStages[DCM_FSS_STAGE_FCOE_PLUGIN_FC]    = string("DCM_FSS_STAGE_FCOE_PLUGIN");
        dcmFcswFssStages[AAA_CFG_STAGE_FC]                = string("AAA_CFG_STAGE");
        dcmFcswFssStages[FSPF_PLUGIN_FSS_STAGE]           = string("FSPF_PLUGIN_FSS_STAGE");
        dcmFcswFssStages[LICENSE_PLUGIN_FSS_STAGE]        = string("LICENSE_PLUGIN_FSS_STAGE");
        dcmFcswFssStages[FW_PLUGIN_FSS_STAGE]             = string("FW_PLUGIN_FSS_STAGE");
        dcmFcswFssStages[AUTH_PLUGIN_FSS_STAGE10]         = string("AUTH_PLUGIN_FSS_STAGE10");
		dcmFcswFssStages[DCM_FSPF_WARM_PUSH]              = string("DCM_FSPF_WARM_PUSH");
        dcmFcswFssStages[DCM_WARM_PREPARE_CLUSTER]        = string("DCM_WARM_PREPARE_CLUSTER");

        dcmEthswFssStages[DCM_FSS_STAGE1]                 = string("DCM_FSS_STAGE1");
        dcmEthswFssStages[DCM_FSS_STAGE2]                 = string("DCM_FSS_STAGE2");
        dcmEthswFssStages[DCM_FSS_STAGE_FCOE_PLUGIN_ETH]  = string("DCM_FSS_STAGE_FCOE_PLUGIN");
    }
    else if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (CCM_NOOP == DcmToolKit::getDistributionMode ()))
    {
        dcmChassisFssStages[CCM_FSS_STAGE1]            = string("CCM_FSS_STAGE1");
        dcmChassisFssStages[CCM_WARM_PREPARE_PLUGINS]  = string("CCM_WARM_PREPARE_PLUGINS");

        dcmFcswFssStages[CCM_FSS_STAGE1]               = string("CCM_FSS_STAGE1");
        dcmFcswFssStages[CCM_WARM_PREPARE_CLUSTER]     = string("CCM_WARM_PREPARE_CLUSTER");

        dcmEthswFssStages[CCM_FSS_STAGE1]              = string("CCM_FSS_STAGE1");
    }
    else if (DCM_NSP == DcmToolKit::getDistributionMode ())
    {
        dcmChassisFssStages[NSP_FSS_STAGE1]            = string("NSP_FSS_STAGE1");

        dcmFcswFssStages[NSP_FSS_STAGE1]               = string("NSP_FSS_STAGE1");

        dcmEthswFssStages[NSP_FSS_STAGE1]              = string("NSP_FSS_STAGE1");
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}


void VcsNodeFabosInterfaceObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

int VcsNodeFabosInterfaceObjectManager::vcsNodeAspInit ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::vcsNodeAspInit: Entering.. ");
    //SI32                rc              = 0;
    //int                 pid;

    string tempDaemon = DcmToolKit::getApplicationIdentifier ();

    /* Fill the cid and smi structures */
    strcpy (dcm_cid.ci_sname, "");
    strcpy (dcm_cid.ci_iname, "");
    strcpy (dcm_cid.ci_comp, tempDaemon.c_str ());

    dcm_smi.sr_flag  =  0;
    dcm_smi.sr_ctor  =  dcmCtor;
    dcm_smi.sr_dtor  =  dcmDtor;
    dcm_smi.sr_expd  =  NULL;
    dcm_smi.sr_dxpd  =  NULL;
    dcm_smi.sr_notify = dcmNotify;

    if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (CCM_NOOP == DcmToolKit::getDistributionMode ()))
    {
        UI32        sleepInSeconds  = 15;
        prismSleep (sleepInSeconds);   
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::vcsNodeAspInit: After sleeping 15 seconds.. ");
    }

    smi_register(&dcm_cid, &dcm_smi);
#if 0
    pid = getpid();

    strcpy(dcm_said_fc.ai_sname, SWITCH_SVC_NAME);
    strcpy(dcm_said_fc.ai_iname, "0");
    strcpy(dcm_said_fc.ai_comp, "dcm");
    dcm_said_fc.ai_sgno = 0;

    rc = hasm_setpid(&dcm_said_fc, pid);

    strcpy(dcm_said_eth.ai_sname, ESWITCH_SVC_NAME);
    strcpy(dcm_said_eth.ai_iname, "0");
    strcpy(dcm_said_eth.ai_comp, "dcm");
    dcm_said_eth.ai_sgno = 0;

    rc = hasm_setpid(&dcm_said_eth, pid);
#endif
    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmCtor (said_t *aid, mq_t mq, smi_cfg_t *config)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmCtor: Entering..");

    if (strcmp(aid->ai_sname, CHASSIS_SVC_NAME) == 0)
    {
        string localIp      = "";
        SI32   localPort    = 0;

        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmCtor: chassis instantiation");
    
        VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();
        pVcsNodeFabosInterfaceObjectManager->dcmInitHaIpAddress(localIp, localPort);

        m_VcsMsgQ = mq;
    
        if (m_fssHandles[0] != -1)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "dcmCtor: received duplicated chassis instantiation trigger");
            return(-1);
        }
		
        if (dcmCtorChassis(mq) < 0) {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmCtor: dcmCtorchassis instantiation failed ... ");
            return (-3);
        }
	
        if (0 == ham_is_active ())
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmCtor: Standby MM detected : localIp: ") + localIp + ", localPort: " + localPort);

            ResourceId status = waitForWyserEaStartPhasesCompletion ();

            if (WAIT_FOR_WYSEREA_PHASE2_TIMED_OUT != status)
            {
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmCtor : Standby MM Status: ") + FrameworkToolKit::localize (status));
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_WARN, string ("VcsNodeFabosInterfaceObjectManager::dcmCtor : Standby MM Status: ") + FrameworkToolKit::localize (status));
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmCtor: Active MM detected : localIp: ") + localIp + ", localPort: " + localPort);
        }

        dcm_chassis_fssd_ops.op_recover      =  dcmRecoverChassis;

        if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            dcm_chassis_fssd_ops.op_recv_update  =  NULL;
            dcm_chassis_fssd_ops.op_send_dump    =  dcmFSSSendDumpNull;
            dcm_chassis_fssd_ops.op_recv_dump    =  dcmFSSRecvDumpNull;
        }
        else
        {
            dcm_chassis_fssd_ops.op_recv_update  =  NULL;
            dcm_chassis_fssd_ops.op_send_dump    =  dcmFSSSendDumpNull;
            dcm_chassis_fssd_ops.op_recv_dump    =  dcmFSSRecvDumpNull;
        }

        fssd_stg_t *dcm_chassis_fssd_stgs = (fssd_stg_t *) calloc (dcmChassisFssStages.size(), sizeof (fssd_stg_t));
        
        if (NULL == dcm_chassis_fssd_stgs)
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::dcmCtor: unable to malloc for dcm_fcsw_chassis_stgs variable");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }

        map<int, string>::iterator element;
        int i = 0;

        for (element = dcmChassisFssStages.begin(); element != dcmChassisFssStages.end(); element++, i++)
        {
            dcm_chassis_fssd_stgs[i].fs_stagenum = element->first;
            strcpy (dcm_chassis_fssd_stgs[i].fs_stagename, element->second.c_str());
        }

        dcm_chassis_fss.fr_ops      =  &dcm_chassis_fssd_ops;
        dcm_chassis_fss.fr_nstages  =  dcmChassisFssStages.size();
        dcm_chassis_fss.fr_stages   =  dcm_chassis_fssd_stgs;
        dcm_chassis_fss.fr_cflags   =  0;
        dcm_chassis_fss.fr_said     =  *aid;
        dcm_chassis_fss.fr_ctxt     =  &m_fssHandles[0];

        m_fssHandles[0] = fssd_register(&dcm_chassis_fss, mq);
        if (m_fssHandles[0] == -1)
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "dcmCtor: failed chassis fssd_register");
            WaveNs::prismAssert(false, __FILE__, __LINE__);
        }
    }
    else if (strcmp(aid->ai_sname, SWITCH_SVC_NAME) == 0)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmCtor: fcsw instantiation");

        if (m_fssHandles[1] != -1)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "dcmCtor: received duplicated fcsw instantiation trigger");
            return(-1);
        }

        if (dcmCtorFcsw(mq) < 0) {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmCtor: dcmCtorfcsw instantiation failed ... ");
            return (-3);
        }

        dcm_fcsw_fssd_ops.op_recover      =  dcmRecoverFcsw;
        dcm_fcsw_fssd_ops.op_recv_update  =  NULL;
        dcm_fcsw_fssd_ops.op_send_dump    =  dcmFSSSendDumpNull;
        dcm_fcsw_fssd_ops.op_recv_dump    =  dcmFSSRecvDumpNull;

        fssd_stg_t *dcm_fcsw_fssd_stgs = (fssd_stg_t *) calloc (dcmFcswFssStages.size(), sizeof (fssd_stg_t));
        if (NULL == dcm_fcsw_fssd_stgs)
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::dcmCtor: unable to malloc for dcm_fcsw_fssd_stgs variable");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }

        map<int, string>::iterator element;
        int i = 0;

        for (element = dcmFcswFssStages.begin(); element != dcmFcswFssStages.end(); element++, i++)
        {
            dcm_fcsw_fssd_stgs[i].fs_stagenum = element->first;
            strcpy (dcm_fcsw_fssd_stgs[i].fs_stagename, element->second.c_str());
        }

        dcm_fcsw_fss.fr_ops      =  &dcm_fcsw_fssd_ops;
        dcm_fcsw_fss.fr_nstages  =  dcmFcswFssStages.size();
        dcm_fcsw_fss.fr_stages   =  dcm_fcsw_fssd_stgs;
        dcm_fcsw_fss.fr_cflags   =  0;
        dcm_fcsw_fss.fr_said     =  *aid;
        dcm_fcsw_fss.fr_ctxt     =  &m_fssHandles[1];

        m_fssHandles[1] = fssd_register(&dcm_fcsw_fss, mq);
        if (m_fssHandles[1] == -1)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "dcmCtor: failed fcsw fssd_register");
            WaveNs::prismAssert(false, __FILE__, __LINE__);
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmCtor: ethsw instantiation ");
        if (m_fssHandles[2] != -1)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "dcmCtor: received duplicated ethsw instantiation trigger");
            return(-1);
        }

        dcm_ethsw_fssd_ops.op_recover      =  dcmRecoverEthsw;
        if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()) || (CCM_ONLY == DcmToolKit::getDistributionMode ()))
        {
           	dcm_ethsw_fssd_ops.op_recv_update  =  dcmFSSRecvConfigUpdate;
            dcm_ethsw_fssd_ops.op_send_dump    =  dcmFSSSendDumpConfig;
            dcm_ethsw_fssd_ops.op_recv_dump    =  dcmFSSRecvDumpNull;
        }
        else
        {
            dcm_ethsw_fssd_ops.op_recv_update  =  NULL;
            dcm_ethsw_fssd_ops.op_send_dump    =  dcmFSSSendDumpNull;
            dcm_ethsw_fssd_ops.op_recv_dump    =  dcmFSSRecvDumpNull;
        }

        fssd_stg_t *dcm_ethsw_fssd_stgs = (fssd_stg_t *) calloc (dcmEthswFssStages.size(), sizeof (fssd_stg_t));

        if (NULL == dcm_ethsw_fssd_stgs)
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::dcmCtor: unable to malloc for dcm_ethsw_fssd_stgs variable");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }

        map<int, string>::iterator element;
        int i = 0;

        for (element = dcmEthswFssStages.begin(); element != dcmEthswFssStages.end(); element++, i++)
        {
            dcm_ethsw_fssd_stgs[i].fs_stagenum = element->first;
            strcpy (dcm_ethsw_fssd_stgs[i].fs_stagename, element->second.c_str());
        }

        dcm_ethsw_fss.fr_ops      =  &dcm_ethsw_fssd_ops;
        dcm_ethsw_fss.fr_nstages  =  dcmEthswFssStages.size();
        dcm_ethsw_fss.fr_stages   =  dcm_ethsw_fssd_stgs;
        dcm_ethsw_fss.fr_cflags   =  0;
        dcm_ethsw_fss.fr_said     =  *aid;
        dcm_ethsw_fss.fr_ctxt     =  &m_fssHandles[2];

        m_fssHandles[2] = fssd_register(&dcm_ethsw_fss, mq);
        if (m_fssHandles[2] == -1)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "dcmCtor: failed ethsw fssd_register");
            WaveNs::prismAssert(false, __FILE__, __LINE__);
        }
    }

    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmRecoverEthsw(void *context, int stage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverEthsw: Entering.....");

    WaveNs::trace (TRACE_LEVEL_DEVEL, string ("VcsNodeFabosInterfaceObjectManager::dcmRecoverEthsw: Got FSS Stage ") + stage);
    
    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

    FrameworkObjectManagerStartExternalStateSynchronizationMessage *pFrameworkObjectManagerStartExternalStateSynchronizationMessage = new FrameworkObjectManagerStartExternalStateSynchronizationMessage (stage, ETHSW);

    WaveMessageStatus status = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pFrameworkObjectManagerStartExternalStateSynchronizationMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmRecoverEthsw: Synchronous send not successful");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        ResourceId completionStatus = pFrameworkObjectManagerStartExternalStateSynchronizationMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmRecoverEthsw: The externalStateSync agent did not complete successfully");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverEthsw: The externalStateSync agent completed successfully");
        }
    }

    delete pFrameworkObjectManagerStartExternalStateSynchronizationMessage;

    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis(void *context, int stage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Got FSS Stage ") + stage);

    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

    m_isActiveNode = true;

    if (stage == FSS_RECOV_WARM)
    { 
        pVcsNodeFabosInterfaceObjectManager->setWarmRecoveryInvolvedInActivation (true);
    }

    // This is common code for cold recovery and warm recovery after DB sync dump started for MM failover case and common for both Dcmd and Ccmd
    else if (((stage == DCM_WARM_PREPARE_PLUGINS) && ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))) ||
             ((stage == CCM_WARM_PREPARE_PLUGINS) && (CCM_ONLY == DcmToolKit::getDistributionMode ())) ||
             ((stage == FSS_RECOV_COLD) && ((PrismFrameworkObjectManagerHaSyncWorker::getInstance ())-> getSyncServicedAsStandby () == true)))
    {
        // Enable local commit transactions, because it is the active MM now
        // Local commit transaction are blocked on the standby MM

        WaveMessageStatus status           = WAVE_MESSAGE_ERROR;

        //If this is MM failover after the initial sync dump ever started, Prepare the plugins for MM failover
        FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration *pMessage = new FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration ();

        pMessage->setWarmHaRecoveryPreparationInProgress (pVcsNodeFabosInterfaceObjectManager->isWarmRecoveryInvolvedInActivation ());

        status = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pMessage);

        WaveNs::prismAssert (status == WAVE_MESSAGE_SUCCESS, __FILE__, __LINE__);
        WaveNs::prismAssert (pMessage->getCompletionStatus () == WAVE_MESSAGE_SUCCESS, __FILE__, __LINE__);

        delete pMessage;


        // For MM failover, need to reset the domain bit map and enable slot SCN
        VcsNodeResetDomainBitMapMessage *pVcsNodeResetDomainBitMapMessage = new VcsNodeResetDomainBitMapMessage ();
        pVcsNodeResetDomainBitMapMessage->setIsWarmRecoveryInvolved (pVcsNodeFabosInterfaceObjectManager->isWarmRecoveryInvolvedInActivation ());

        status = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pVcsNodeResetDomainBitMapMessage);

        WaveNs::prismAssert (status == WAVE_MESSAGE_SUCCESS, __FILE__, __LINE__);
        WaveNs::prismAssert (pVcsNodeResetDomainBitMapMessage->getCompletionStatus () == WAVE_MESSAGE_SUCCESS, __FILE__, __LINE__);

        delete pVcsNodeResetDomainBitMapMessage;


        // send the message to set the slot SCN message
        VcsNodeEnableSlotScnHandlingMessage *pVcsNodeEnableSlotScnHandlingMessage = new VcsNodeEnableSlotScnHandlingMessage ();

        status = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pVcsNodeEnableSlotScnHandlingMessage);

        WaveNs::prismAssert (status == WAVE_MESSAGE_SUCCESS, __FILE__, __LINE__);
        WaveNs::prismAssert (pVcsNodeEnableSlotScnHandlingMessage->getCompletionStatus () == WAVE_MESSAGE_SUCCESS, __FILE__, __LINE__);

        delete pVcsNodeEnableSlotScnHandlingMessage;
    }

    // This is either cold boot or cold recovery with startup file case, trigger startup file early replay if DB invalid
    if ((stage == FSS_RECOV_COLD) && ((PrismFrameworkObjectManager::getInstance ())-> getStandbySyncState () == OUT_OF_SYNC))
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, " VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Got FSS Stage %d, FSS_RECOV_COLD", stage);

        if (!FrameworkToolKit::getIsStartupValid ())
        {
            if (FrameworkToolKit::getStartupFileType () == WAVE_PERSISTENCE_REGULAR_FILE)
            {
                // Parse the startup file for linecard configurations 
                // and write it to the database
                m_earlyConfigurationReplayWorker->invalidStartupDBHandler ();
            }
        }
    }
    else if (stage == FSS_RECOV_ACTIVE)
    {
        // First tear down stale HA transport with previous active MM if still up. This is to avoid plugin to get stuck trying to use it after failover.
        if (FrameworkToolKit::isConnectedToHaPeerLocation () == WAVE_MESSAGE_SUCCESS)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Tear down stale HA transport"));
            pVcsNodeFabosInterfaceObjectManager->disconnectFromHaPeer();
        }

        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Got FSS Stage %d, FSS_RECOV_ACTIVE", stage);
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Completed activation steps");
    }
    else if (stage == 1)
    {
        string tempDaemonName = DcmToolKit::getApplicationIdentifier ();

        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: hasm_setpid");

        int pid = getpid();
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: PID is ") + pid);

#ifdef VIRGO_MERGE
        strcpy(dcm_said_fc.ai_sname, SWITCH_SVC_NAME);
        strcpy(dcm_said_fc.ai_iname, "0");
        strcpy(dcm_said_fc.ai_comp, tempDaemonName.c_str ());
        dcm_said_fc.ai_sgno = 0;

        int rc = hasm_setpid(&dcm_said_fc, pid);
        if (rc < 0)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Return Value of hasm_setpid for FCSW context is ") + rc);
        }

        strcpy(dcm_said_eth.ai_sname, ESWITCH_SVC_NAME);
        strcpy(dcm_said_eth.ai_iname, "0");
        strcpy(dcm_said_eth.ai_comp, tempDaemonName.c_str ());
        dcm_said_eth.ai_sgno = 0;

        rc = hasm_setpid(&dcm_said_eth, pid);
        if (rc < 0)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Return Value of hasm_setpid for ETHSW context is ") + rc);
        }
#endif
    }

    // Do early Config Replay when snapshot restore is in progress
    if (stage == FSS_RECOV_COLD)
    {
        // If mode is MC and config is default
        if ((VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && (DCM_CCM == DcmToolKit::getDistributionMode ()) && (0 == DcmToolKit::getConfigurationTransactionId ()))
        {
            string configFilePath  = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
            string configFilename  = ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename();
 
            string filename = configFilePath + "/" + configFilename;
 
            //If local Config File Exist
            if (ConfigFileManagementToolKit::isFileExisting (filename))
            {
                WaveNs::trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Snapshot Filename")+filename);
                m_earlyConfigurationReplayWorker->configSnapshotEarlyConfigReplayHandler ();
            }
        }
    }


    FrameworkObjectManagerStartExternalStateSynchronizationMessage *pFrameworkObjectManagerStartExternalStateSynchronizationMessage = new FrameworkObjectManagerStartExternalStateSynchronizationMessage (stage, CHASSIS);

    WaveMessageStatus status = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pFrameworkObjectManagerStartExternalStateSynchronizationMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: Synchronous send not successful");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        ResourceId completionStatus = pFrameworkObjectManagerStartExternalStateSynchronizationMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: The externalStateSync agent did not complete successfully");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverChassis: The externalStateSync agent completed successfully");
        }
    }

    delete pFrameworkObjectManagerStartExternalStateSynchronizationMessage;

    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmNotify (said_t *aid, smi_evt_t *evt)
{
    UI32 eventType = 0;
    UI32 slotId = 0;
    int partitionId = 0;
    ResourceId recoveryType = WAVE_HA_COLD_RECOVERY;
    UI32 syncFlag;  
 
    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    { 
	    switch (evt->se_event)
        {
		    case SMI_EVT_SERVICE_READY:
            {
                eventType = VCS_NODE_FABOS_SERVICE_READY;
                recoveryType = (evt->se_arg)? WAVE_HA_WARM_RECOVERY : WAVE_HA_COLD_RECOVERY;
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmNotify: Recv SERVICE_READY"));
                VcsNodeFabosInterfaceObjectManager::getInstance ()->dcmNotifyPostBootEvents(eventType, slotId, recoveryType);
		        break;
            }
            case SMI_EVT_SLOT_READY:
            {
                eventType = VCS_NODE_FABOS_LINECARD_READY;
                recoveryType = (evt->se_arg)? WAVE_HA_WARM_RECOVERY : WAVE_HA_COLD_RECOVERY;
                slotId = evt->se_slot;
                partitionId = evt->se_part;
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmNotify: Recv SLOT_READY ") + slotId + string(" P") + partitionId);
                // This step is needed for warm recovery as SMI_EVT_SLOT_ACTIVE is not available 
                VcsNodeFabosInterfaceObjectManager::setActivePartitionIdBySlot(slotId, partitionId);
                VcsNodeFabosInterfaceObjectManager::getInstance ()->dcmNotifyPostBootEvents(eventType, slotId, recoveryType);
                break;
            }
            case SMI_EVT_SYSTEM_READY:
            {
                // this event will broadcast to all the service instances due
                // to the requirement from VRRP. skip this event in non-chassis
                // context.
                if (strcmp(aid->ai_sname, CHASSIS_SVC_NAME) == 0) {
                    eventType = VCS_NODE_FABOS_SYSTEM_READY;
                    recoveryType = (evt->se_arg)? WAVE_HA_WARM_RECOVERY : WAVE_HA_COLD_RECOVERY;
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmNotify: Recv SYSTEM_READY"));
                    VcsNodeFabosInterfaceObjectManager::getInstance ()->dcmNotifyPostBootEvents(eventType, slotId, recoveryType);
                }
                break;
            }
            case SMI_EVT_SLOT_ACTIVE:
            {
                slotId = evt->se_slot;
                partitionId = evt->se_part;
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmNotify: Recv SLOT_ACTIVE ") + slotId + string(" P") + partitionId);
                VcsNodeFabosInterfaceObjectManager::setActivePartitionIdBySlot(slotId, partitionId);
                break;
            }
            case SMI_EVT_SLOT_UP:
            {
                slotId = evt->se_slot;
                partitionId = evt->se_part;
                string ipaddrLC = VcsNodeFabosInterfaceObjectManager::getInstance ()->getIpAddressBySlotParitionId(slotId, partitionId);
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmNotify: Recv SLOT_UP ") + slotId + string(" P") + partitionId + string(" IP=") + ipaddrLC);
                FrameworkToolKit::disconnectFromAllInstanceClients (ipaddrLC);
                break;
            }
            case SMI_EVT_SLOT_NOT_READY:
            {
                slotId = evt->se_slot;
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmNotify: Recv SMI_EVT_SLOT_NOT_READY ") + slotId);
                VcsNodeFabosInterfaceObjectManager::setActivePartitionIdBySlot(slotId, LINECARD_PARTITION_INVALID);
                break;
            }
        }
    }
 
    if (evt->se_event == SMI_EVT_SYNC_DUMP)
    {
        syncFlag = evt->se_arg;
//      WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmNotify: Recv SYNC_DUMP flag : ") + syncFlag);

        if ((syncFlag == SMI_ARG_STOP) && (strcmp(aid->ai_sname, ESWITCH_SVC_NAME) == 0))
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager:: Recv Sync Stop evt."));
            VcsNodeFabosInterfaceObjectManager::getInstance ()->terminateDcmHaSync ();
        }
        else if ((syncFlag == SMI_ARG_START) && on_upgrade() && getIsActiveNode() && (strcmp(aid->ai_sname, CHASSIS_SVC_NAME) == 0))
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager:: Recv ISSU evt."));
			VcsNodeFabosInterfaceObjectManager::setIssuInProgress(true);
        }
    }

    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmNotifyPostBootEvents(UI32 &eventType, UI32 &slotId, ResourceId &recoveryType)
{
//    eventId = queueThisScn (eventType, 0, 0);

	switch (eventType)
    {
        case VCS_NODE_FABOS_SERVICE_READY: 
		{
			VcsNodeFabosServiceReadyEvent *pServiceReadyEvent = new VcsNodeFabosServiceReadyEvent ();
    		prismAssert (NULL != pServiceReadyEvent, __FILE__, __LINE__);
						
//    		pServiceReadyEvent->setEventId (eventId);
            pServiceReadyEvent->setRecoveryType (recoveryType);
    		broadcast (pServiceReadyEvent);
			break;
		}
        case VCS_NODE_FABOS_LINECARD_READY:
		{
            VcsNodeFabosLinecardReadyEvent *pLinecardReadyEvent = new VcsNodeFabosLinecardReadyEvent ();
            prismAssert (NULL != pLinecardReadyEvent, __FILE__, __LINE__);

            UI32 type = recoveryType;
            UI32 slotNumber = slotId;
            UI32 eventId = queueThisSlotScn(slotNumber, 0, 0, 0, SLOT_READY, type, 0);
            pLinecardReadyEvent->setEventId (eventId);
			pLinecardReadyEvent->setRecoveryType (recoveryType);
            pLinecardReadyEvent->setSlotId (slotId);
            broadcast (pLinecardReadyEvent);
            break;
		}
		case VCS_NODE_FABOS_SYSTEM_READY:
		{
            VcsNodeFabosSystemReadyEvent *pSystemReadyEvent = new VcsNodeFabosSystemReadyEvent ();
            prismAssert (NULL != pSystemReadyEvent, __FILE__, __LINE__);

//          pSystemReadyEvent->setEventId (eventId);
			pSystemReadyEvent->setRecoveryType (recoveryType);
            broadcast (pSystemReadyEvent);
            break;
		}
        default:
            trace(TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::dcmNotifyPostBootEvents: Unknown event."));
            break;
	}
    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmFSSSendDumpConfig(void *context, int vers, int tid)
{
    int fssh = *(int *)context;
    WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSSendDumpConfig: Entering.....");

    VcsNodeFabosInterfaceObjectManager::setIsSyncEnabled (true);

    FSSD_DUMP_BEGIN(fssh);

    VcsNodeFabosInterfaceObjectManager::getInstance ()->dcmFssNotifyStartSyncDump();

    return(0);
}

int VcsNodeFabosInterfaceObjectManager::dcmInitHaIpAddress(string &localIp, SI32 &localPort)
{
    struct in_addr local_ipa;

    local_ipa.s_addr = get_cp_ha_localip();
    localIp.assign(inet_ntoa(local_ipa));

    localPort = FrameworkToolKit::getHaInterfaceReceiverPort();

    WaveNs::trace (TRACE_LEVEL_INFO, string("dcmInitHaIpAddress: HA local ip:") + localIp + string(" port:") + localPort);

    FrameworkObjectManagerInitHaIpAddressMessage *pFrameworkObjectManagerInitHaIpAddressMessage =
        new FrameworkObjectManagerInitHaIpAddressMessage(localIp, localPort);

    WaveMessageStatus status = sendOneWay (pFrameworkObjectManagerInitHaIpAddressMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "dcmInitHaIpAddress: send InitHaIpAddressMessage failed\n");
    }

    return (status);
}

void VcsNodeFabosInterfaceObjectManager::dcmFssNotifyStartSyncDump()
{
    struct in_addr peer_ipa;
    string peerIp = "";
    SI32 peerPort = FrameworkToolKit::getHaInterfaceReceiverPort();;
    UI32 eventId = 0;

    peer_ipa.s_addr = get_cp_ha_peerip();
    peerIp.assign(inet_ntoa(peer_ipa));

    WaveNs::trace (TRACE_LEVEL_INFO, string("dcmFssNotifyStartSyncDump: HA peer ip:") + peerIp + string(" port:") + peerPort);

    VcsNodeFabosSyncStartEvent *pSyncStartEvent = new VcsNodeFabosSyncStartEvent (peerIp, peerPort);
    prismAssert (NULL != pSyncStartEvent, __FILE__, __LINE__);

	eventId = queueThisScn (VCS_NODE_FABOS_SYNC_START, 0, 0);
    pSyncStartEvent->setEventId (eventId);
    broadcast (pSyncStartEvent);
    
    return;
}

int VcsNodeFabosInterfaceObjectManager::dcmFSSSendConfigUpdate(int fssh, int vers, int tid)
{
	//struct  iovec iov = {0};
    //fssd_udb_t   fssu = {0};
    int pathsize = 0;   

	WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate: Entering...fssh:%d, tid:%x..", fssh, tid);
    VcsFabricLocalObjectManager::parseVcsConfFile (dcmConfigFssSyncData.m_vcsMode, dcmConfigFssSyncData.m_vcsId); 
	dcmConfigFssSyncData.m_clusterType = VcsFabricLocalObjectManager::getThisVcsNodeClusterType ();
    dcmConfigFssSyncData.m_clusterConf = VcsFabricLocalObjectManager::parseClusterConfFile ();
	dcmConfigFssSyncData.m_rbridgeId = VcsNodeFabosInterfaceObjectManager::getInstance()->getDomainId ();
	dcmConfigFssSyncData.m_startupFileType = FrameworkToolKit::getStartupFileType ();
    pathsize = readlink (FrameworkToolKit::getStartupFileName().c_str(), dcmConfigFssSyncData.m_startupFileName,
		sizeof(dcmConfigFssSyncData.m_startupFileName) - 1);
    WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate: startupFile %s %d", dcmConfigFssSyncData.m_startupFileName, dcmConfigFssSyncData.m_startupFileType);
    WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate: FrameworkToolKit startupFile %s %d",FrameworkToolKit::getStartupFileName().c_str(), FrameworkToolKit::getStartupFileType () );
    WaveNs::prismAssert (pathsize != -1 , __FILE__, __LINE__);  
    dcmConfigFssSyncData.m_startupFileName [pathsize] = '\0';

	if (dcmConfigFssSyncData.m_startupFileType == WAVE_PERSISTENCE_REGULAR_FILE) 
	{
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate: WAVE_PERSISTENCE_REGULAR_FILE ");
		string cmd = "", peerIp = "", fileName = "";
		struct in_addr ipa;
		ipa.s_addr = get_cp_ha_peerip();

		fileName.assign(dcmConfigFssSyncData.m_startupFileName);
		peerIp.assign(inet_ntoa(ipa));	// 

		cmd = "rcp " + fileName + " " + peerIp + ":" + fileName;
		WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate:" + cmd);
		(void)system(cmd.c_str());
	}
    else if (dcmConfigFssSyncData.m_startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate: startup WAVE_PERSISTENCE_DEFAULT_FILE"); 
    }
    else if (dcmConfigFssSyncData.m_startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate: startup WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE");
    }
	else
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFSSSendConfigUpdate: startupfile type unknown %d", dcmConfigFssSyncData.m_startupFileType);
    }
	
	//iov.iov_base = &dcmConfigFssSyncData;
    //iov.iov_len = sizeof (dcmConfigFssSyncData);
	//fssu.fu_iov = &iov;
    //fssu.fu_cnt = 1;
    //fssu.fu_tid = tid;

//	(void) fssd_send_update(fssh, &fssu);

	WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmFSSSendConfigUpdate: exit");
    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmFSSSendDumpNull(void *context, int vers, int tid)
{
	int fssh = *(int *)context;
	WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmFSSSendDumpNULL: Entering.....");

	FSSD_DUMP_BEGIN(fssh);
	// Nothing to do here...
	FSSD_DUMP_END(fssh, 0);

	return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmFSSRecvConfigUpdate(void *context, fssd_udb_t *pfssu)
{
	struct iovec *iov;
	dcmConfigFssSyncData_t  *dcmConfigData;
	
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmFSSRecvConfigUpdate.");
	
    iov = pfssu->fu_iov;
    dcmConfigData = (dcmConfigFssSyncData_t *)(iov->iov_base);

	VcsNodeFabosInterfaceObjectManager::getInstance ()->dcmFssHandleConfigUpdate(dcmConfigData);

	return(0);
}

int VcsNodeFabosInterfaceObjectManager::dcmFssHandleConfigUpdate(dcmConfigFssSyncData_t *dcmConfigData)
{
	WaveMessageStatus   status = WAVE_MESSAGE_ERROR;
    bool				idChanged = false;
    bool                clusterConfChanged = false;
	bool                localVcsMode;
    bool                isPrevStartupDbValid;
    UI32                localVcsId;
    UI32                localRbridgeId;
    SI32                localClusterConf;
    UI32                startupFileType;
    int                 domainId = 0;
    int                 ret = 0;

    isPrevStartupDbValid = FrameworkToolKit::getIsStartupValid ();

	WaveNs::trace (TRACE_LEVEL_INFO, string("dcmFSSHandleConfigUpdate: setup startup file to type:") + 
		dcmConfigData->m_startupFileType + " filename:" + string(dcmConfigData->m_startupFileName));

    startupFileType = dcmConfigData->m_startupFileType;
    if (dcmConfigData->m_startupFileType == WAVE_PERSISTENCE_REGULAR_FILE)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFssHandleConfigUpdate: startup WAVE_PERSISTENCE_REGULAR_FILE");
    }
    else if (dcmConfigData->m_startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFssHandleConfigUpdate: startup WAVE_PERSISTENCE_DEFAULT_FILE");
    }
    else if (dcmConfigData->m_startupFileType == WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFssHandleConfigUpdate: startup WAVE_PERSISTENCE_DEFAULT_FILE_AT_LOCAL_NODE");
        startupFileType = WAVE_PERSISTENCE_DEFAULT_FILE;	// For Persistence manager to be handled properly
    }
    else
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, "dcmFssHandleConfigUpdate: startupfile type unknown %d", dcmConfigFssSyncData.m_startupFileType);
    }

    PersistenceLocalObjectManagerSetStartupFileMessage *pPersistenceLocalObjectManagerSetStartupFileMessage =
		new PersistenceLocalObjectManagerSetStartupFileMessage(startupFileType, dcmConfigData->m_startupFileName);

    status = sendSynchronously (pPersistenceLocalObjectManagerSetStartupFileMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string("dcmFSSHandleConfigUpdate: send SetStartupFileMessage failed - status: ") + FrameworkToolKit::localize(status));
    }
    else
    {
        ResourceId completionStatus = pPersistenceLocalObjectManagerSetStartupFileMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "dcmFSSHandleConfigUpdate: SetStartupFileMessage complete with error\n");
        }
    }
    delete (pPersistenceLocalObjectManagerSetStartupFileMessage);
    FrameworkToolKit::savePrismConfiguration();

    VcsFabricLocalObjectManager::parseVcsConfFile (localVcsMode, localVcsId);
    localClusterConf = VcsFabricLocalObjectManager::parseClusterConfFile ();
    localRbridgeId = VcsNodeFabosInterfaceObjectManager::getInstance()->getDomainId ();

   
	if (dcmConfigData->m_vcsId != localVcsId)
	{
		WaveNs::trace (TRACE_LEVEL_INFO, string("dcmFSSHandleConfigUpdate:: need to set vcsId to ") + dcmConfigData->m_vcsId + string(" from ") + localVcsId);
		VcsNodeSetVcsIdMessage *pVcsNodeSetVcsIdMessage = new VcsNodeSetVcsIdMessage (dcmConfigData->m_vcsId);

        status = sendSynchronously (pVcsNodeSetVcsIdMessage);
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            ResourceId completionStatus = pVcsNodeSetVcsIdMessage->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: Failed to send VCS Id msg.\n");
            }
            else
            {
                idChanged = true;
            }
        }
        else
        {
			WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: VCS ID msg complete with error.\n");
        }
        delete (pVcsNodeSetVcsIdMessage);
	}

	if (dcmConfigData->m_rbridgeId != localRbridgeId)
	{
		WaveNs::trace (TRACE_LEVEL_INFO, string("dcmFSSHandleConfigUpdate:: need to set rbridgeId to ") + dcmConfigData->m_rbridgeId + string(" from ") + localRbridgeId);
    	VcsNodeSetRBridgeIdMessage *pVcsNodeSetRBridgeIdMessage = new VcsNodeSetRBridgeIdMessage(dcmConfigData->m_rbridgeId);

		status = sendSynchronously (pVcsNodeSetRBridgeIdMessage);
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            ResourceId completionStatus = pVcsNodeSetRBridgeIdMessage->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: Failed to send rbridgeId msg.\n");
            }
            else
            {
                idChanged = true;
                if (configGet("fabric.domain", ResourceInt, &domainId) != 0)
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: insistent domainId not set.\n");
                }
                else
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, string("dcmFSSHandleConfigUpdate: new domainId is ") + domainId);
                }
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: rbridgeId msg complete with error.\n");
        }
        delete (pVcsNodeSetRBridgeIdMessage);
	}

    if ((dcmConfigData->m_clusterConf != localClusterConf) && ((dcmConfigData->m_clusterConf == 1) || (dcmConfigData->m_clusterConf == 0))) 
	{
        WaveNs::trace (TRACE_LEVEL_INFO, string("dcmFSSHandleConfigUpdate:: need to set cluster conf to ") + dcmConfigData->m_clusterConf);

        ResourceId completionStatus = VcsFabricLocalObjectManager::setClusterConfFile(dcmConfigData->m_clusterConf);
        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: Failed to cluster conf.\n");
        }
        else
        {
            clusterConfChanged = true;
        }
    }

	if (dcmConfigData->m_vcsMode != localVcsMode)
	{
		WaveNs::trace (TRACE_LEVEL_INFO, string("dcmFSSHandleConfigUpdate: need to set vcsMode to ") + dcmConfigData->m_vcsMode);
    	VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage = new VcsNodeSetVcsModeMessage(dcmConfigData->m_vcsMode);

		status = sendSynchronously (pVcsNodeSetVcsModeMessage);
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            ResourceId completionStatus = pVcsNodeSetVcsModeMessage->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: Failed to send VCS mode msg.\n");
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: VCS mode msg complete with error.\n");
        }
        delete (pVcsNodeSetVcsModeMessage);
	}
    else if (idChanged)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: need to trigger reboot for id change.\n");

        if (0 == (UI32) (dcmConfigData->m_vcsMode))
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            VcsNodeCopyDefaultStartTriggerMessage *pVcsNodeCopyDefaultStartTriggerMessage = new VcsNodeCopyDefaultStartTriggerMessage ((UI32) (dcmConfigData->m_vcsMode));
            status = sendSynchronously (pVcsNodeCopyDefaultStartTriggerMessage);
            if (WAVE_MESSAGE_SUCCESS == status)
            {
                ResourceId completionStatus = pVcsNodeCopyDefaultStartTriggerMessage->getCompletionStatus ();
                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: Failed to send CopyDefaultStartTrigger msg.\n");
                }
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: CopyDefaultStartTrigger msg complete with error.\n");
            }
            delete (pVcsNodeCopyDefaultStartTriggerMessage);
        }
    }
    else if (clusterConfChanged)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: need to trigger reboot for cluster conf change.\n");
        (void) system ("rm -rf /etc/fabos/Ccmd/*.cfg");
        sync();
        (void) system ("/fabos/sbin/reboot -s");
    }
    else if (isPrevStartupDbValid == true)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "dcmFSSHandleConfigUpdate: need to trigger reboot since prev DB valid.\n");
        bool isValid = false;
        FrameworkToolKit::setIsStartupValid (isValid);
        FrameworkToolKit::savePrismConfiguration();
        sync();

        // Return reboot required status
        ret = HA_STANDBY_REBOOT_REQUIRED;
    }
    return (ret);
}

int	VcsNodeFabosInterfaceObjectManager::handleDcmFssConfigUpdate (void *dcmConfigData)
{
    return (VcsNodeFabosInterfaceObjectManager::getInstance ()->
            dcmFssHandleConfigUpdate((dcmConfigFssSyncData_t *)dcmConfigData));
}

int VcsNodeFabosInterfaceObjectManager::dcmFSSRecvDumpNull(void *context, int vers, int sgid)
{
	WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmFSSRecvDumpNULL: Nothing to do, sgid %x", sgid);
	return (0);
}

void VcsNodeFabosInterfaceObjectManager::dcmHaSyncDumpEnd (SI32 status)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmHASyncDumpEnd Entering.");

    if (VcsNodeFabosInterfaceObjectManager::getIsSyncEnabled())
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager: trigger FSSDumpEnd.");
        FSSD_DUMP_END(m_fssHandles[2], (int)status);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager exit. No sync dump on .");
    }
}

bool VcsNodeFabosInterfaceObjectManager::dcmHaSyncUpdateFailure (const ResourceId &reason)
{
    bool toNotifyFss = true;

    if (getIsSyncEnabled ())
    {
        if (getIssuInProgress () && ((reason == WAVE_MESSAGE_ERROR_CLUSTER_STATUS_HASYNC_NOT_SUPPORTED) ||
                                     (reason == WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_NOT_SUPPORTED)))
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager:dcmHaSyncUpdateFailure: cluster DB or status change not synced during ISSU due to unsupported. Skip reporting these cases.");
            toNotifyFss = false;
        }
        else if ( FrameworkToolKit::isConnectedToHaPeerLocation () != WAVE_MESSAGE_SUCCESS)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager:dcmHaSyncUpdateFailure: HA transport already disconnected,skip reporting.");
            toNotifyFss = false;
        } 
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager:dcmHaSyncUpdateFailure: trigger FSS_STATUS_SYNFAIL.");
            fssd_notify_status(m_fssHandles[2], FSS_STATUS_SYNFAIL, reason);
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmHaSyncUpdateFailure. HA sync disabled. Skip reporting");
        toNotifyFss = false;
    }
    return (toNotifyFss);
}


void VcsNodeFabosInterfaceObjectManager::disconnectFromHaPeer ()
{
    struct in_addr peer_ipa;
    string peerIp = "";
    SI32 peerPort = FrameworkToolKit::getHaInterfaceReceiverPort();;
    bool closepeersocket = true;

    peer_ipa.s_addr = get_cp_ha_peerip();
    peerIp.assign(inet_ntoa(peer_ipa));

    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager::disconnectFromHaPeer: Disconnect transport from HA peer ip:") + peerIp + string(" port:") + peerPort);

    (PrismFrameworkObjectManagerHaSyncWorker::getInstance ())->disconnectFromHaPeer (peerIp, peerPort, closepeersocket);
}

int VcsNodeFabosInterfaceObjectManager::terminateDcmHaSync ()
{
    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    // trigger this function only on active MM after sync is triggered. 
    if (getIsSyncEnabled ())
    {
        disconnectFromHaPeer();

        FrameworkObjectManagerEndHaSyncMessage *pFrameworkObjectManagerEndHaSyncMessage = new FrameworkObjectManagerEndHaSyncMessage ();
        prismAssert (pFrameworkObjectManagerEndHaSyncMessage != NULL, __FILE__, __LINE__);
        status = sendOneWay (pFrameworkObjectManagerEndHaSyncMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        { 
            trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::terminateDcmHaSync: Error sending endSync message.");
            prismAssert (false, __FILE__, __LINE__);
        }

        setIsSyncEnabled (false);
    }
    return (status);
}

void VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmConfigReplayDone ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmConfigReplayDone entering.");
    has_tell_status(-1, HAS_CONFIG_COMPLETE);
}

void VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmSlotPostbootDone (UI32 &slotId)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmPostbootDone entering.");
    has_tell_status(slotId, HAS_CONFIG_COMPLETE);
}

void VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmClusterReadyState (bool &readyState)
{
    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmClusterReadyState: ") + readyState);
    if (readyState == true)
    {
        has_tell_status(-1, HAS_CLUSTER_READY);
    }
    else
    {
        has_tell_status(-1, HAS_CLUSTER_UNREADY);
    }
}

ResourceId VcsNodeFabosInterfaceObjectManager::waitForWyserEaStartPhasesCompletion ()
{
    WaveNs::trace (TRACE_LEVEL_DEVEL, "VcsNodeFabosInterfaceObjectManager::waitForWyserEaStartPhasesCompletion : Entering...");

    UI32        count           = 0;
    UI32        lastCount       = 120;
    UI32        sleepInSeconds  = 1;
    ResourceId  status          = WAIT_FOR_WYSEREA_PHASE2_TIMED_OUT;

    while (count < lastCount)
    {
        if (true == VcsFabricLocalObjectManager::getIsWyserEaPhase2Complete ())
        {
            status = WAIT_FOR_WYSEREA_PHASE2_COMPLETED;
            break;
        }

        prismSleep (sleepInSeconds);
        count++;
    }

    return (status);
}

int VcsNodeFabosInterfaceObjectManager::dcmRecoverFcsw (void *context, int stage)
{
    WaveNs::tracePrintf (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverFcsw: Got FSS Stage %d", stage);

    ResourceId messageStatus = WAVE_MESSAGE_ERROR;
    ResourceId messageCompletionStatus = WAVE_MESSAGE_ERROR;

    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

    if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) && ((stage == CCM_WARM_PREPARE_CLUSTER) || (stage == FSS_RECOV_COLD)))
    {
            LocationRole currentLocationRole     = FrameworkToolKit::getThisLocationRole ();

            if (LOCATION_PRIMARY == currentLocationRole) 
            {
                FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage = new FrameworkObjectManagerDestroyClusterMessage ();
                pFrameworkObjectManagerDestroyClusterMessage->setOriginalRequester (VcsFabricLocalObjectManager::getPrismServiceId ()); 
                pFrameworkObjectManagerDestroyClusterMessage->setIsRebootRequired (false);

                messageStatus = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pFrameworkObjectManagerDestroyClusterMessage);

                if (WAVE_MESSAGE_SUCCESS != messageStatus)
                {
                    WaveNs::prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    messageCompletionStatus = pFrameworkObjectManagerDestroyClusterMessage->getCompletionStatus ();

                    if (WAVE_MESSAGE_SUCCESS != messageCompletionStatus)
                    {
                        WaveNs::trace (TRACE_LEVEL_ERROR, string ("Failed to destroy cluster") + FrameworkToolKit::localize (messageCompletionStatus));
                    }
                }

                delete pFrameworkObjectManagerDestroyClusterMessage;
            }
            else if (LOCATION_SECONDARY == currentLocationRole || LOCATION_PRIMARY_UNCONFIRMED == currentLocationRole || LOCATION_SECONDARY_UNCONFIRMED == currentLocationRole)
            {
                string ipAddress            = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
                SI32 port                   = FrameworkToolKit::getPortForLocationId (FrameworkToolKit::getThisLocationId ());

                FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);

                pMessage->setIsPerformWarmRecovery (true);

                messageStatus = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pMessage);

                if (WAVE_MESSAGE_SUCCESS != messageStatus)
                {
                    WaveNs::prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    messageCompletionStatus = pMessage->getCompletionStatus ();

                    if (WAVE_MESSAGE_SUCCESS != messageCompletionStatus)
                    {
                        WaveNs::trace (TRACE_LEVEL_ERROR, string ("Completion of Unconfigure Self Node Fails") + FrameworkToolKit::localize (messageCompletionStatus));
                    }
                }

                delete pMessage;
            }
    }

    FrameworkObjectManagerStartExternalStateSynchronizationMessage *pFrameworkObjectManagerStartExternalStateSynchronizationMessage = new FrameworkObjectManagerStartExternalStateSynchronizationMessage (stage, FCSW);

    WaveMessageStatus status = pVcsNodeFabosInterfaceObjectManager->sendSynchronously (pFrameworkObjectManagerStartExternalStateSynchronizationMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmRecoverFcsw: Synchronous send not successful");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        ResourceId completionStatus = pFrameworkObjectManagerStartExternalStateSynchronizationMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmRecoverFcsw: The externalStateSync agent did not complete successfully");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::dcmRecoverFcsw: The externalStateSync agent completed successfully");
        }
    }

    delete pFrameworkObjectManagerStartExternalStateSynchronizationMessage;

    if (stage == FSS_RECOV_ACTIVE)
    {
        //send Prepare Domain List Message To VcsFabricLocalObjectManager.

        VcsNodePrepareDomainListAfterHaRecoveryMessage vcsNodePrepareDomainListAfterHaRecoveryMessage;

        vcsNodePrepareDomainListAfterHaRecoveryMessage.setIsWarmRecoveryInvolved ((VcsNodeFabosInterfaceObjectManager::getInstance ())->isWarmRecoveryInvolvedInActivation ());

        messageStatus = (VcsNodeFabosInterfaceObjectManager::getInstance ())->sendSynchronously (&vcsNodePrepareDomainListAfterHaRecoveryMessage);

        WaveNs::prismAssert (WAVE_MESSAGE_SUCCESS == messageStatus, __FILE__, __LINE__);

        WaveNs::prismAssert (WAVE_MESSAGE_SUCCESS == vcsNodePrepareDomainListAfterHaRecoveryMessage.getCompletionStatus (), __FILE__, __LINE__);

        (VcsNodeFabosInterfaceObjectManager::getInstance ())->setWarmRecoveryInvolvedInActivation (false);
    }

    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmDtor (said_t *aid)
{
    return (0);
}

int VcsNodeFabosInterfaceObjectManager::dcmCtorChassis (mq_t mq)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::vcsNodeCtorChassis: Entering.. ");
    SI32                rc              = 0;

    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()))
    {
        u_int sys_scn_register_list [][2] = {
        {SLOT_SCN, SLOT_IN | SLOT_ON | SLOT_OUT | SLOT_FAULT},
        {SCN_END_MARKER, SCN_END_MARKER}
        };
        isc_tok_t tarr[1];

        rc = sysModScnRegisterWrapper (m_sysmodh, sys_scn_register_list, mq);
        if (0 != rc)
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "Cannot register for SLOT SCN.");
            WaveNs::prismAssert(false, __FILE__, __LINE__);
            return (1);
        }

        /* isc register for query slot api */
        dcm_isc_ops.op_recv_request = dcmIscReceive;
        dcm_isc_ops.op_recv_reply = NULL;
        dcm_isc_ops.op_recv_event = NULL;

        strcpy(dcm_isc_reg.ir_said.ai_sname, "chassis");
        strcpy(dcm_isc_reg.ir_said.ai_iname, "0");
        strcpy(dcm_isc_reg.ir_said.ai_comp, "dcm");

        dcm_isc_reg.ir_said.ai_sgno = 0;
        dcm_isc_reg.ir_ctxt = NULL;
        dcm_isc_reg.ir_ops = &dcm_isc_ops;
        dcm_isc_reg.ir_flag = ISC_CFLG_HA | ISC_CFLG_LOCAL;

        m_iscHandle = isc_register(ISC_DOM_CHASSIS, &dcm_isc_reg, mq);
        tarr[0].it_type = ISC_TYPE_QUERY_SLOT;
        tarr[0].it_flag = 0;
        isc_subscribe(m_iscHandle, 1, tarr);
    }

    return (0);
}


int VcsNodeFabosInterfaceObjectManager::dcmCtorFcsw (mq_t mq)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::vcsNodeCtorFcsw: Entering.. ");
    SI32                rc              = 1;

	/****************************************************************************
	 *  Get the switch serial number
	 ***************************************************************************/
	UI32                waitCount = 0;
    emChassisSNInfo_t   emSerialNumber;

    while (0 != rc)
    {
        rc = em_GetChassisSN(&emSerialNumber);

        if (0 == rc)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "\nConnected to emd. Proceeding ...");
            break;
        }

        prismSleep(1);
        waitCount++;
        WaveNs::trace (TRACE_LEVEL_INFO, string ("Still trying to connect to emd. Elapsed seconds : ") + waitCount);

        if (30 <= waitCount)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string("Cannot connect to emd. Giving up"));
            WaveNs::prismAssert(false, __FILE__, __LINE__);
            return 0;
        }
    }

    // Ugly workaround - could have done string chassisString (emSerialNumber.chassisSN) but it prepends a unterminated quote for some reason.
    char tmpString [EM_CHASSIS_SN_LEN+1];
    strncpy (tmpString, emSerialNumber.chassisSN, EM_CHASSIS_SN_LEN);
    tmpString [EM_CHASSIS_SN_LEN] = '\0';

    WaveNs::trace (TRACE_LEVEL_INFO, string ("Chassis Serial Number : ") + tmpString);

    /****************************************************************************
     * Get swbd model name e.g. "VDX6720-24"
     ***************************************************************************/

    string          swmodelName     = "";
    swmodel_nm_t   *pSwmodelName    = (swmodel_nm_t *) calloc (1, sizeof (swmodel_nm_t)); /// calloc

    if (NULL == pSwmodelName)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::dcmCtorFcsw: calloc returned NULL pointer.");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    int swmodelNameRet = em_get_swmodel_name (pSwmodelName);

    if (0 == swmodelNameRet)
    {
        char swmodelNameBuffer[SWMODEL_NAME_SIZE + 1];
        strncpy (swmodelNameBuffer, (char *) (*pSwmodelName), SWMODEL_NAME_SIZE);
        swmodelNameBuffer[SWMODEL_NAME_SIZE] = '\0';

        swmodelName = swmodelNameBuffer;

        WaveNs::trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeFabosInterfaceObjectManager::dcmCtorFcsw: Swmodel Name : [") + swmodelName + string ("]"));

        setSwmodelNameWrapper (swmodelName);
    }
    else
    {
        WaveNs::tracePrintf (TRACE_LEVEL_ERROR, "VcsNodeFabosInterfaceObjectManager::dcmCtorFcsw: em_get_swmodel_name returned [%d]", swmodelNameRet);
    }

    free (pSwmodelName); /// free

    setChassisStringWrapper (tmpString);

    /****************************************************************************
     *  Register to get the switch domain id
     ***************************************************************************/
    u_int scn_register_list [][2] = {
    /* New SCN registrations go above this line. */
    {SWITCH_SCN, DOMAIN_VALID | DOMAIN_INVALID | DOMAIN_CAP_AVAILABLE |
            DOMAIN_REACHABLE | DOMAIN_UNREACHABLE | SW_DISABLES_PORT | SW_OFFLINE |
            ZONE_CHANGED | ZONE_ABORTED |
            SWITCH_BOOTUP_STATE_UPDATE},
    {SCN_END_MARKER, SCN_END_MARKER}
    //{0xffffffff, 0xffffffff}
    };

    if (mq == NULL)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::dcmCtorFcsw: Could not create message queue");
        WaveNs::prismAssert(false, __FILE__, __LINE__);
        return (1);
    }

    rc = scnRegister(scn_register_list, NULL, mq, MYSWITCH);

    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmCtorFcsw: SCNRegister complete with status: ") + rc);
    if (0 != rc)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "Cannot register for FOS SCN.");
        WaveNs::prismAssert(false, __FILE__, __LINE__);
        return (1);
    }
    return (0);
}

int VcsNodeFabosInterfaceObjectManager::vcsNodeAspExit ()
{
    WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::vcsNodeAspExit.");
    WaveNs::prismAssert (false, __FILE__, __LINE__);
    return (0);
}

void VcsNodeFabosInterfaceObjectManager::vcsNodeServe (msg_t *msg)
{
        WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsNodeFabosInterfaceObjectManager::vcsNodeServe  IS CALLED ");

        if (CCM_NOOP != DcmToolKit::getDistributionMode ())
        {
            switch (msg->msg_type)
            {
                case MSG_SCN:
                {
                    WaveNs::trace(TRACE_LEVEL_DEBUG, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received message of type: ") + (msg->msg_type) + "msg scn type = "+msg->msg_scn.type + ", subtype: " + msg->msg_scn.subtype);
                    switch (msg->msg_scn.type)
                    {
                        case SLOT_SCN:
                        {
                            UI32 slotType, slotNumber;
                            UI32 slotState = msg->msg_scn.subtype;
                            UI32 slotPhyNumber = sysScnUnitNum(&(msg->msg_scn)); 
                            UI32 objectType = sysScnObject (&(msg->msg_scn));
                            UI32 reason = sysScnReason (&(msg->msg_scn));
                            UI32 hwType = sysScnHwType (&(msg->msg_scn));
                            UI32 hwId = sysScnHwId (&(msg->msg_scn));
                            UI32 version = sysScnVersion (&(msg->msg_scn));
                            slot_no2id((int)slotPhyNumber, (int *)(&slotType), (int *)(&slotNumber));

                            WaveNs::tracePrintf(TRACE_LEVEL_INFO,
                                "VcsNodeFabosInterfaceObjectManager:: Received slot scn:%d  slot%d slotpno%d state%d reason %d modid %d object%d type%d bladeId%d\n", 
                                msg->msg_scn.type, slotNumber, slotPhyNumber, slotState, SCN_GET_REASON(reason), SCN_GET_MODID(reason), objectType, hwType, hwId);

                            handleSlotEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version);
                            break;
                        }
                        case SWITCH_SCN:
                            switch (msg->msg_scn.subtype)
                            {
                                case DOMAIN_VALID:
                                    // MSG_SCN_SW_DOMAIN(msg) = domainId, MSG_SCN_SW_ARG1(msg) = fcPrincipal domainId 
                                    // these macros are defined in: ..fabos/src/mace/cnm/include/mqueue.h
                                    // fabos file where they get instantiated is: /vobs/projects/springboard/fabos/src/sys/dev/switch/switch_ioctl.c
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Domain Valid"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_VALID, MSG_SCN_SW_DOMAIN(msg), MSG_SCN_SW_ARG1(msg));  
                                    break;

                                case DOMAIN_INVALID:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Domain Invalid"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_INVALID, MSG_SCN_SW_DOMAIN(msg), 0);
                                    break;

                                case DOMAIN_CAP_AVAILABLE:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Domain Capabilities Available"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE, MSG_SCN_SW_DOMAIN(msg), 0);
                                    break;

                                case DOMAIN_REACHABLE:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Domain Reachable"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_REACHABLE, MSG_SCN_SW_DOMAIN(msg), 0);
                                    break;

                                case DOMAIN_UNREACHABLE:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Domain UnReachable"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_DOMAIN_UNREACHABLE, MSG_SCN_SW_DOMAIN(msg), 0);
                                    
                                    break;

                                case SW_DISABLES_PORT:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Disable Port"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handlePortMessage (VCS_NODE_FABOS_PORT_DISABLE, MSG_SCN_PORT_NO(msg), 0);
                                    break;

                                case SW_OFFLINE:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Switch Offline"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleDomainMessage (VCS_NODE_FABOS_SWITCH_OFFLINE,  MSG_SCN_SW_DOMAIN(msg), 0);
                                    break;

                                case ZONE_CHANGED:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received Zone changed"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleZoneChanged();
	                            break;
           
                                case ZONE_ABORTED:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received zone aborted"));
                                    
                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleZoneAborted();
	                            break;

                                case SWITCH_BOOTUP_STATE_UPDATE:
                                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Switch bootup state update"));

                                    (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleSwitchBootupStateUpdate(MSG_SCN_SW_ARG1(msg));
                                break;

                                default:
                                    WaveNs::trace(TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Unknown message received will not be handled message subtype = ")+msg->msg_scn.subtype);
                                	    
                                    break;
                            }
                            break;

                        case RSCN:
                            break;

                        default:
                            break;
                    }
                }
                break;
                case MSG_IPC:
                {
                    dcm_ipc_t *data = (dcm_ipc_t *) msg->msg_data;
                    WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received IPC message"));

                    switch (data->type)
                    {
                        default:
                           break;
                    }
                    free (data);
                }
                break;

            default:
                break;
         }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "CCM is no operataion mode so do nothing");
    }
}

void VcsNodeFabosInterfaceObjectManager::setChassisStringWrapper (string chassisString)
{
    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

    pVcsNodeFabosInterfaceObjectManager->setChassisString (chassisString);
}

void VcsNodeFabosInterfaceObjectManager::setSwmodelNameWrapper (const string &swmodelName)
{
    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();

    pVcsNodeFabosInterfaceObjectManager->setSwmodelName (swmodelName);
}

void VcsNodeFabosInterfaceObjectManager::setChassisString (string chassisString)
{
    m_chassisString = chassisString;

    /****************************************************************************
     *  Get the switch node wwn 
     ***************************************************************************/
    wwn_t wwn;
    char  tmpWWN[24];

    getNodeName (&wwn);
    wwnfmt_r (&wwn, tmpWWN, sizeof (tmpWWN));

    string wwnString (tmpWWN);
    m_switchWWN = WorldWideName (wwnString);

    WaveNs::trace (TRACE_LEVEL_INFO, string ("Switch Node WWN : ") + wwnString);

    SI32 domainId = getDomainId ();
    
    VcsNodeUpdateSwitchInfoMessage *pVcsNodeUpdateSwitchInfoMessage = new VcsNodeUpdateSwitchInfoMessage (m_chassisString, m_switchMac, domainId, m_switchWWN, m_swModel, m_fwVersion, getSwmodelName ());

    WaveMessageStatus status = sendOneWay (pVcsNodeUpdateSwitchInfoMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFabosInterfaceObjectManager::setChassisString: Failed to send one way message to VcsFabricLocalObjectManager");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
}

UI32 VcsNodeFabosInterfaceObjectManager::scnQueueSize ()
{
	m_scnQueueMutex.lock();
	UI32 queueSize = m_scnQueue.size ();
	m_scnQueueMutex.unlock();
	return queueSize;	
}

bool VcsNodeFabosInterfaceObjectManager::isScnEventInQueue (UI32 eventId)
{

    if (m_scnQueue.empty ())
    {
        return false;
    }

    UI32 queueObjectEventId = 0;
    
    for (std::deque<VcsNodeFabosScnQueueObject *>::iterator itr=m_scnQueue.begin(); itr!=m_scnQueue.end(); ++itr)
    {
        queueObjectEventId = (*itr)->getScnObjectId();
        if (queueObjectEventId == eventId)
        {
            return true;
        }
    }
    
    return false;
}

ResourceId VcsNodeFabosInterfaceObjectManager::popPScnQueueObject (UI32 eventId)
{
	m_scnQueueMutex.lock();
    ResourceId status = WAVE_MESSAGE_ERROR;
	if (0 != m_scnQueue.size ())
	{
		VcsNodeFabosScnQueueObject *pScnQueueObject = m_scnQueue.front ();
        if (pScnQueueObject->getScnObjectId () != eventId)
        {
            if (isScnEventInQueue (eventId))
            {
                // the pop event request is for an object not in front of the queue so it must have been poped earlier 
                trace (TRACE_LEVEL_FATAL, string ("VcsNodeFabosInterfaceObjectManager::popPScnQueueObject: popScnQueue failed, there may be an unhandled SCN event in front of queue: scnEventOid = ")+eventId+" front of queue scnObjectId = "+pScnQueueObject->getScnObjectId ());
                
                // asserting since this means a receiver is handling an event out of order or there's a unhandled event behavior is unpredictable
                prismAssert (false, __FILE__, __LINE__);  
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::popPScnQueueObject: popScnQueue failed, the scnEvent queue doesn't include this event, event has aleady been handled, eventId = ")+eventId);
            }
            
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::popPScnQueueObject: poped SCN from queue, type = ")+pScnQueueObject->getScnType()+ ", SCNObjectId = "+ pScnQueueObject->getScnObjectId()+ ", domainId = "+pScnQueueObject->getDomainId ());
            
    		delete pScnQueueObject;
    		m_scnQueue.pop_front ();
            status = WAVE_MESSAGE_SUCCESS;
        }
	}
	else
	{
		trace (TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::popPScnQueueObject: popScnQueue failed, the scnEvent queue is empty, event may already have been handled, eventId = ")+eventId);
	}
	m_scnQueueMutex.unlock();
    return (status);
}

void VcsNodeFabosInterfaceObjectManager::pushPScnQueueObject (VcsNodeFabosScnQueueObject* pScnQueueObject)
{
	static UI32 scnCount = 0;
	m_scnQueueMutex.lock();
	scnCount++;
	pScnQueueObject->setScnObjectId (scnCount);
	
	trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::pushPScnQueueObject: pushed SCN to queue, type = ")+pScnQueueObject->getScnType()+", SCNObjectId = "+ pScnQueueObject->getScnObjectId()+", domainId = "+pScnQueueObject->getDomainId ());
    
	m_scnQueue.push_back (pScnQueueObject);
	m_scnQueueMutex.unlock();
}

VcsNodeFabosScnQueueObject* VcsNodeFabosInterfaceObjectManager::getFrontScnQueueObject ()
{
	trace (TRACE_LEVEL_DEBUG, "VcsNodeFabosInterfaceObjectManager::getFrontScnQueueObject: Entering...");
	
	m_scnQueueMutex.lock();
	if (0 != m_scnQueue.size ())
	{
		VcsNodeFabosScnQueueObject *pScnQueueObject = m_scnQueue.front ();
        
		trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::getFrontScnQueueObject: front SCN from queue, type = ")+pScnQueueObject->getScnType()+", domainId = "+pScnQueueObject->getDomainId ());
        
		m_scnQueueMutex.unlock();
		return pScnQueueObject;
	}
	else
	{
		trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::getFrontScnQueueObject: the SCN queue is empty"));
        
		m_scnQueueMutex.unlock();
		return NULL;
	}

}

UI32 VcsNodeFabosInterfaceObjectManager::queueThisScn (UI32 eventType, UI32 domainId, UI32 fcPrincipalDomainId)
{
	trace (TRACE_LEVEL_DEBUG, "VcsNodeFabosInterfaceObjectManager::queueThisScn: Entering...");
    
	VcsNodeFabosScnQueueObject *pScnQueueObject = NULL;
	pScnQueueObject = new VcsNodeFabosScnQueueObject ();
	prismAssert (NULL != pScnQueueObject, __FILE__, __LINE__);
	
	pScnQueueObject->setScnType(eventType);
	pScnQueueObject->setDomainId(domainId);
	pScnQueueObject->setPrincipalDomainId (fcPrincipalDomainId);
	pushPScnQueueObject (pScnQueueObject);
    return pScnQueueObject->getScnObjectId ();
}


UI32 VcsNodeFabosInterfaceObjectManager::slotScnQueueSize ()
{
	m_slotScnQueueMutex.lock();
	UI32 queueSize = m_slotScnQueue.size ();
	m_slotScnQueueMutex.unlock();
	return queueSize;	
}

bool VcsNodeFabosInterfaceObjectManager::isSlotEventInQueue (UI32 eventId)
{

    if (m_slotScnQueue.empty ())
    {
        return false;
    }

    UI32 slotQueueObjectEventId = 0;
    
    for (std::deque<VcsNodeFabosSlotScnQueueObject *>::iterator itr=m_slotScnQueue.begin(); itr!=m_slotScnQueue.end(); ++itr)
    {
        slotQueueObjectEventId = (*itr)->getSlotScnObjectId();
        if (slotQueueObjectEventId == eventId)
        {
            return true;
        }
    }
    
    return false;
}

ResourceId VcsNodeFabosInterfaceObjectManager::popPSlotScnQueueObject (UI32 eventId)
{
	m_slotScnQueueMutex.lock();
    ResourceId status = WAVE_MESSAGE_ERROR;
	if (0 != m_slotScnQueue.size ())
	{
		VcsNodeFabosSlotScnQueueObject *pSlotScnQueueObject = m_slotScnQueue.front ();
        
	    if (eventId != pSlotScnQueueObject->getSlotScnObjectId())
        {
            if (isSlotEventInQueue (eventId))
            {
                // attempting to pop an event not in front of the queue may be attempt to handle event twice 
                trace (TRACE_LEVEL_FATAL, string ("VcsNodeFabosInterfaceObjectManager::popPSlotScnQueueObject: popSlotScnQueue failed, may be an unhandled slot event in front of queue, eventOid = ")+eventId + ", front of queue objectId = "+pSlotScnQueueObject->getSlotScnObjectId());
                
                // asserting since this means a receiver is handling an event out of order or there's a unhandled event, behavior is unpredictable
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::popPSlotScnQueueObject: popSlotScnQueue failed, slot SCN is not in queue, event may have already been handled, eventId = ")+eventId); 
            }
        }
        else
        {
    		trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::popPSlotScnQueueObject: poped SCN from queue, slotNumber = ")+pSlotScnQueueObject->getSlotNumber()+", hwType = "+ pSlotScnQueueObject->getHwType()+", slotState = "+pSlotScnQueueObject->getSlotState ()); 
            
    		delete pSlotScnQueueObject;		
    		m_slotScnQueue.pop_front ();
            status = WAVE_MESSAGE_SUCCESS;
        }
	}
	else
	{
		trace (TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::popPSlotScnQueueObject: popSlotScnQueue failed, the SCN queue is empty, event may have already been handled, eventId = ")+eventId);
	}
	m_slotScnQueueMutex.unlock();
    return (status);
}

void VcsNodeFabosInterfaceObjectManager::pushPSlotScnQueueObject (VcsNodeFabosSlotScnQueueObject* pSlotScnQueueObject)
{
	static UI32 slotScnCount = 0;
	m_slotScnQueueMutex.lock();
	slotScnCount++;
	pSlotScnQueueObject->setSlotScnObjectId (slotScnCount);
	
	trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::pushPSlotScnQueueObject: pushed slot SCN to queue, slotNumber = ")+pSlotScnQueueObject->getSlotNumber()+", hwType = "+ pSlotScnQueueObject->getHwType()+", slotState = "+pSlotScnQueueObject->getSlotState ()+" eventObjectId = "+pSlotScnQueueObject->getSlotScnObjectId ());
    
	m_slotScnQueue.push_back (pSlotScnQueueObject);
	m_slotScnQueueMutex.unlock();
}

VcsNodeFabosSlotScnQueueObject* VcsNodeFabosInterfaceObjectManager::getFrontSlotScnQueueObject ()
{
	trace (TRACE_LEVEL_DEBUG, "VcsNodeFabosInterfaceObjectManager::getFrontSlotScnQueueObject: Entering...");
	
	m_slotScnQueueMutex.lock();
	if (0 != m_slotScnQueue.size ())
	{
		VcsNodeFabosSlotScnQueueObject *pSlotScnQueueObject = m_slotScnQueue.front ();
        
		trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::getFrontSlotScnQueueObject: front SCN from queue, slotNumber = ")+pSlotScnQueueObject->getSlotNumber()+", hwType = "+ pSlotScnQueueObject->getHwType()+", slotState = "+pSlotScnQueueObject->getSlotState ()); 
        
		m_slotScnQueueMutex.unlock();
		return pSlotScnQueueObject;
	}
	else
	{
		trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::getFrontSlotScnQueueObject: the SCN queue is empty"));
        
		m_slotScnQueueMutex.unlock();
		return NULL;
	}

}

UI32 VcsNodeFabosInterfaceObjectManager::queueThisSlotScn (UI32 slotNumber, UI32 objectType, UI32 hwType, UI32 hwId, UI32 slotState, UI32 reason, UI32 version)
{
	trace (TRACE_LEVEL_DEBUG, "VcsNodeFabosInterfaceObjectManager::queueThisSlotScn: Entering...");
    
	VcsNodeFabosSlotScnQueueObject *pSlotScnQueueObject = new VcsNodeFabosSlotScnQueueObject (slotNumber, objectType, hwType, hwId, slotState, reason, version);
	prismAssert (NULL != pSlotScnQueueObject, __FILE__, __LINE__);
	pushPSlotScnQueueObject (pSlotScnQueueObject);
    return pSlotScnQueueObject->getSlotScnObjectId ();
}

void VcsNodeFabosInterfaceObjectManager::handleDomainMessage (VcsNodeDomainEvent event, UI32 domainId, UI32 fcPrincipalDomainId)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsNodeFabosInterfaceObjectManager::handleDomainMessage: Entering...");

    /****************************************************************************
     * Broadcast the domain valid event to the listeners
     ***************************************************************************/

    VcsNodeFabosDomainValidEvent                 *pDVEvent                          = NULL;
    VcsNodeFabosDomainCapabilitiesAvailableEvent *pDCAEvent                         = NULL;
    VcsNodeFabosDomainInvalidEvent               *pDIEvent                          = NULL;
    VcsNodeFabosDomainReachableEvent             *pDREvent                          = NULL;
    VcsNodeFabosDomainUnreachableEvent           *pDUEvent                          = NULL;
    VcsNodeFabosSwitchOfflineEvent               *pSwitchOfflineEvent               = NULL;
    string                                        ethernetInterface                 = "";
    string                                        ipAddress                         = "";
    UI32                                          maxNumberOfRetriesForIpAddress    = 60;
    UI32                                          numberOfRetriesForIpAddress       = 0;
    UI32                                          eventId                           = 0;
    string                                        interfaceState                    = "";

    trace (TRACE_LEVEL_DEBUG, string ("Handling Event for Domain : ") + domainId);

    switch (event)
    {
        case VCS_NODE_FABOS_DOMAIN_VALID:

            pDVEvent = new VcsNodeFabosDomainValidEvent ();
            pDVEvent->setDomainId (domainId);
            pDVEvent->setPrincipalDomainId (fcPrincipalDomainId);

            ethernetInterface = PrismFrameworkObjectManager::getEthernetInterfaceForThisLocation ();

            if ("" == ethernetInterface)
            {
                trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::handleDomainMessage : No specific Ethernet Interface Assigned");
            }
            else
            {
                ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);

                if (true == ((VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeLocalManagedObjectVcsMode()))
                {
                
                    while ("" == ipAddress)
                    {
                        trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::handleDomainMessage Retrying to fetch IP Address for Ethernet Interface \"" + ethernetInterface + "\"");
                        ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);
                        numberOfRetriesForIpAddress++;
                        if (numberOfRetriesForIpAddress > maxNumberOfRetriesForIpAddress)
                        {
                            trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::handleDomainMessage Cound not Fetch IpAddress:");
                            interfaceState = getStateForInterface(ethernetInterface);
                            trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager::handleDomainMessage :getStateForInterface ") + ethernetInterface + string(" : ") + interfaceState);

                            break;
                        }
                        else
                        {
                            prismSleep(1);
                        }
                    }
                    
                    trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::handleDomainMessage Able to Fetch IpAddress:\"" + ipAddress + "\" interface: \"" + ethernetInterface + "\"");
                    
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::handleDomainMessage Vcs Mode is Not Enabled ");
                }
            }
            
			eventId = queueThisScn (VCS_NODE_FABOS_DOMAIN_VALID, domainId, fcPrincipalDomainId);
            pDVEvent->setEventId(eventId);
            broadcast (pDVEvent);
            break;

        case VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE:

            pDCAEvent = new VcsNodeFabosDomainCapabilitiesAvailableEvent ();
            pDCAEvent->setDomainId (domainId);
			eventId = queueThisScn (VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE, domainId, fcPrincipalDomainId);
            pDCAEvent->setEventId(eventId);
            broadcast (pDCAEvent);
            break;

        case VCS_NODE_FABOS_DOMAIN_INVALID:

            pDIEvent = new VcsNodeFabosDomainInvalidEvent ();
            pDIEvent->setDomainId (domainId);
			eventId = queueThisScn (VCS_NODE_FABOS_DOMAIN_INVALID, domainId, fcPrincipalDomainId);
            pDIEvent->setEventId(eventId);
            broadcast (pDIEvent);
            break;

        case VCS_NODE_FABOS_DOMAIN_REACHABLE:

            pDREvent = new VcsNodeFabosDomainReachableEvent ();
            pDREvent->setDomainId (domainId);
			eventId = queueThisScn (VCS_NODE_FABOS_DOMAIN_REACHABLE, domainId, fcPrincipalDomainId);
            pDREvent->setEventId(eventId);
            broadcast (pDREvent);
            break;

        case VCS_NODE_FABOS_DOMAIN_UNREACHABLE:

            pDUEvent = new VcsNodeFabosDomainUnreachableEvent ();
            pDUEvent->setDomainId (domainId);
			eventId = queueThisScn (VCS_NODE_FABOS_DOMAIN_UNREACHABLE, domainId, fcPrincipalDomainId);
            pDUEvent->setEventId(eventId);
            broadcast (pDUEvent);
            break;

        case VCS_NODE_FABOS_SWITCH_OFFLINE:

            pSwitchOfflineEvent = new VcsNodeFabosSwitchOfflineEvent ();
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromAllKnownLocations ();
			eventId = queueThisScn (VCS_NODE_FABOS_SWITCH_OFFLINE, domainId, fcPrincipalDomainId);
            pSwitchOfflineEvent->setEventId(eventId);
            broadcast (pSwitchOfflineEvent);
            break;
            
        default:
            break;
            
    }
}

void VcsNodeFabosInterfaceObjectManager::handleSlotEvent (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::handleSlotEvent: slotState = ")+slotState+" slotNumber = "+slotNumber+" hwType = "+hwType+" objectType = "+objectType+" hwId = "+hwId+" reason = "+reason+" version = "+version);
	if ((hwType == SYS_BLADE_AP) || (hwType == SYS_BLADE_SW))
    {
        if ((SLOT_ON == slotState) && (SCN_GET_MODID(reason) == SCN_MOD_DIAG )
            && ((SCN_GET_REASON(reason) == SCN_NORMAL) ||
                (SCN_GET_REASON(reason) == SCN_SKIP_POST) ||
                (SCN_GET_REASON(reason) == SCN_SKIP_POST_AP) ||
                (SCN_GET_REASON(reason) == SCN_FAIL_OVER) ||
                (SCN_GET_REASON(reason) == SCN_FAIL_OVER_AP)))

        {
            m_linecardState[slotNumber] = slotState;
            (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleSlotOnlineEvent(slotNumber, objectType, hwType, hwId, slotState, reason, version);
        }
        else if (((slotState == SLOT_IN) || (slotState == SLOT_OUT) || (slotState == SLOT_FAULT)) &&
                 (SCN_GET_MODID(reason) == SCN_MOD_EM) &&
                 (slotState != m_linecardState[slotNumber])) 
                 
        {
            m_linecardState[slotNumber] = slotState;
            (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleSlotOfflineEvent(slotNumber, objectType, hwType, hwId, slotState, reason, version);
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::handleSlotEvent: unhandled slot event for hwType = ")+hwType+" SCN_GET_MODID(reason) = "+SCN_GET_MODID(reason)+" SCN_MOD_DIAG = "+SCN_MOD_DIAG+
                "(SCN_GET_REASON(reason) = "+SCN_GET_REASON(reason)+" SCN_NORMAL = "+SCN_NORMAL+" m_linecardState[slotNumber] = "+m_linecardState[slotNumber]); 
        }
    }
    else if ((hwType == SYS_BLADE_CPCTRL) && (hwId == MERCURY_MM_BLADE_ID))
    {
        if (SCN_GET_REASON(reason) == SCN_CP_FAULT)
        {
             VcsNodeFabosInterfaceObjectManager::getInstance ()->terminateDcmHaSync ();
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::handleSlotEvent: unhandled slot event for hwType = ")+hwType);
    }
}

void VcsNodeFabosInterfaceObjectManager::handleSlotOnlineEvent (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
{
    UI32 eventId = 0;
    if ((hwType == SYS_BLADE_AP) || (hwType == SYS_BLADE_SW))
    {
        hwType = SYS_SLOT_LC;
    }

    VcsNodeFabosSlotOnlineEvent *pOnlineEvent = new VcsNodeFabosSlotOnlineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);
    prismAssert (NULL != pOnlineEvent, __FILE__, __LINE__);
    eventId = queueThisSlotScn(slotNumber,objectType,hwType,hwId,slotState,reason,version);
    pOnlineEvent->setEventId (eventId);
    broadcast (pOnlineEvent);
    
    trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager:: SLOT_ONLINE event, slot: ") + slotNumber + string(" bladeId: ") + hwId);
}

string  VcsNodeFabosInterfaceObjectManager::getIpAddressBySlotParitionId(const UI32 &slotId, const int &partitionId)
{
    string ipaddrLC = "Invalid IP";

    if ((slotId < MAX_LINECARD_NUM) && ((partitionId == LINECARD_PARTITION_0) || (partitionId == LINECARD_PARTITION_1)))
    {
        int ipLC = get_bp_nodeip(get_bp_nodeid((int)slot_id2no(SYS_SLOT_LC,slotId), partitionId), 0);
        struct in_addr in;

        in.s_addr = ipLC;
        ipaddrLC = inet_ntoa(in);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("getIpAddressBySlotParitionId: Recv invalid slot/parition ID ") + slotId + string("P") + partitionId);
    }
    return (ipaddrLC);
}

void VcsNodeFabosInterfaceObjectManager::handleSlotOfflineEvent (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
{
    UI32 eventId = 0;
    int instance;

    if ((hwType == SYS_BLADE_AP) || (hwType == SYS_BLADE_SW))
    {
        hwType = SYS_SLOT_LC;
    }

    VcsNodeFabosSlotOfflineEvent *pOfflineEvent = new VcsNodeFabosSlotOfflineEvent (slotNumber, objectType, hwType, hwId, slotState, reason, version, eventId);
    prismAssert (NULL != pOfflineEvent, __FILE__, __LINE__);
    eventId = queueThisSlotScn(slotNumber,objectType,hwType,hwId,slotState,reason,version);
    pOfflineEvent->setEventId(eventId);
    broadcast (pOfflineEvent);
    
    trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager:: SLOT_OFFLINE event, slot: ") + slotNumber + " hwType: "+hwType+ " state:" + slotState);

    // Only trigger the client agent disconnect logic if the slot state 
    // is SLOT_IN or SLOT_OUT.  For the faulty slot state, we do not 
    // want to trigger the client agent disconnect logic
    if ((slotState == SLOT_IN) || (slotState == SLOT_OUT))
    {
        for (instance = 0; instance < 2; instance++) 
        {
            string ipaddrLC = getIpAddressBySlotParitionId(slotNumber, instance);
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::handleSlotOfflineEvent Slot=") + slotNumber + string(" IP=") + ipaddrLC);
            FrameworkToolKit::disconnectFromAllInstanceClients (ipaddrLC);
        }
        setActivePartitionIdBySlot(slotNumber, LINECARD_PARTITION_INVALID);
    }
    
}

void VcsNodeFabosInterfaceObjectManager::handleZoneChanged()
{
    VcsNodeFabosZoneChangedEvent *pEvent = new VcsNodeFabosZoneChangedEvent ();
    broadcast (pEvent);
}

void VcsNodeFabosInterfaceObjectManager::handleZoneAborted()
{
    VcsNodeFabosZoneAbortedEvent *pEvent = new VcsNodeFabosZoneAbortedEvent ();
    broadcast (pEvent);
}

void VcsNodeFabosInterfaceObjectManager::handleSwitchBootupStateUpdate (UI32 state)
{
    UI32 eventId = 0;

    if (CCM_ONLY == DcmToolKit::getDistributionMode ())
	{
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::handleSwitchBootupStateUpdate: CCM_ONLY mode");

        switch (state)
        {
        case SWITCH_STATE_READY_ACCEPT_COMMANDS:
            {
            VcsNodeFabosSwitchReadyToAcceptCommandsEvent *pReadyEvent = new VcsNodeFabosSwitchReadyToAcceptCommandsEvent ();
            eventId = queueThisScn (VCS_NODE_FABOS_SWITCH_READY_TO_ACCEPT_COMMANDS, 0, 0);
            pReadyEvent->setEventId(eventId);
            broadcast (pReadyEvent);
            break;
            }
			
        case SWITCH_STATE_ACTIVATION_COMPLETE:
            {
            VcsNodeFabosSwitchActivationCompleteEvent *pActivationEvent = new VcsNodeFabosSwitchActivationCompleteEvent ();
            eventId = queueThisScn (VCS_NODE_FABOS_SWITCH_ACTIVATION_COMPLETE, 0, 0);
            pActivationEvent->setEventId(eventId);
            broadcast (pActivationEvent);
            break;
            }
        }
    }
}

void VcsNodeFabosInterfaceObjectManager::handlePortMessage (VcsNodeDomainEvent event, int portNumber, int reasonCode)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsNodeFabosInterfaceObjectManager::handlePortMessage: Entering...");

    /****************************************************************************
     * Handle distribution of port-related state change events
     ***************************************************************************/

    VcsNodeFabosSwitchPortDisableEvent  *pSPDEvent = NULL;

    switch (event)
    {
        case VCS_NODE_FABOS_PORT_DISABLE:
            pSPDEvent = new VcsNodeFabosSwitchPortDisableEvent ();
            pSPDEvent->setPortNumber (portNumber);
            pSPDEvent->setDisableReasonCode (reasonCode);
            broadcast (pSPDEvent);
            break;

        default:
            break;
    }
}

void VcsNodeFabosInterfaceObjectManager::setIsDcmdReadyForHasmLaunch (bool isDcmdReadyForHasmLaunch)
{
    m_isDcmdReadyForHasmLaunch = isDcmdReadyForHasmLaunch;
}

bool VcsNodeFabosInterfaceObjectManager::getIsDcmdReadyForHasmLaunch ()
{
    return (m_isDcmdReadyForHasmLaunch); 
}

void VcsNodeFabosInterfaceObjectManager::setIsHasmLaunched (bool isHasmLaunched)
{
    m_isHasmLaunched = isHasmLaunched;
}

bool VcsNodeFabosInterfaceObjectManager::getIsHasmLaunched ()
{
    return (m_isHasmLaunched);
}    

bool VcsNodeFabosInterfaceObjectManager::getIsSyncEnabled()
{
    bool syncEnabled;

    m_syncEnabledMutex.lock ();

    syncEnabled = m_syncEnabled;

    m_syncEnabledMutex.unlock ();

    return (syncEnabled);
}    

void VcsNodeFabosInterfaceObjectManager::setIsSyncEnabled(bool syncEnabled)
{
    m_syncEnabledMutex.lock ();

    m_syncEnabled = syncEnabled;

    m_syncEnabledMutex.unlock ();
}

bool VcsNodeFabosInterfaceObjectManager::getIsConfigInitDone ()
{
    return (m_isConfigInitDone);
}

int VcsNodeFabosInterfaceObjectManager::getDomainId ()
{
    int domainId = 0;

    m_domainIdMutex.lock ();

    domainId = m_domainId;
    
    m_domainIdMutex.unlock ();
    
    return (domainId);
}

void VcsNodeFabosInterfaceObjectManager::setDomainId (int domainId)
{
    m_domainIdMutex.lock ();

    m_domainId = domainId;

    m_domainIdMutex.unlock ();
}

int VcsNodeFabosInterfaceObjectManager::getActivePartitionIdBySlot (const UI32 &slotId)
{
    int partitionId = LINECARD_PARTITION_INVALID;
    
    if (slotId < MAX_LINECARD_NUM)
    {
        m_linecardActivePartitionIdMutex.lock ();
        partitionId = m_linecardActivePartitionId[slotId];
        m_linecardActivePartitionIdMutex.unlock ();
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string("Recv invalid LC slot ID: slot ") + slotId);
    }

    return (partitionId);
}

void VcsNodeFabosInterfaceObjectManager::setActivePartitionIdBySlot (const UI32 &slotId, const int &partitionId)
{
    if (slotId < MAX_LINECARD_NUM)
    {
        int id = partitionId;
        if ((partitionId != LINECARD_PARTITION_0) && (partitionId != LINECARD_PARTITION_1) && (partitionId != LINECARD_PARTITION_INVALID))
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string("setActivePartitionIdBySlot: Recv invalid parition ID: slot ") + slotId + string(" P") + partitionId);
           id = LINECARD_PARTITION_INVALID;
        }

        m_linecardActivePartitionIdMutex.lock ();
        m_linecardActivePartitionId[slotId] = id;
        m_linecardActivePartitionIdMutex.unlock ();
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string("setActivePartitionIdBySlot: Recv invalid LC slot ID: slot ") + slotId);
    }
}

int VcsNodeFabosInterfaceObjectManager::dcmIpcCallback (IPC_ADDR *addr, MSG_TYPE type, char *buf, size_t size, void *stuff)
{
    msg_t msg;
    dcm_ipc_t *msgData;
    int ret = 0;
    int msgType = 0;

    // magic ipc offset to get real request data
    buf += sizeof (size_t);
    msg.msg_type = MSG_IPC;
    msgData = (dcm_ipc_t *) buf;
    msgType = msgData->type;

    switch (msgType)
    {
        default:
        {
            WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmIpcCallback: Unsupported message type: ") + msgType);
            return (ret);
            break;
        }
    }

    WaveNs::prismAssert (NULL != m_VcsMsgQ, __FILE__, __LINE__);

    ret = mqWrite(m_VcsMsgQ, &msg);
    if (ret != 0)
    {
        WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmIpcCallback: Error: mqWrite to m_VcsMsgQ was unsuccessful."));
    }
    return (ret);

}

void VcsNodeFabosInterfaceObjectManager::handleSlotTypeMessage (int reqh, int type, UI32 slotNumber, UI32 hwType)
{
    // The hwType for slot type query is always SYS_SLOT_LC
    hwType = SYS_SLOT_LC;
    trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::handleSlotTypeMessage slot: ")+slotNumber);

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFabosInterfaceObjectManager::getSlotObjects),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFabosInterfaceObjectManager::getSlotType),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFabosInterfaceObjectManager::replySlotQueryIscMessage),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFabosInterfaceObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFabosInterfaceObjectManager::prismLinearSequencerFailedStep)
    };

    VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext = new VcsNodeSlotMessagingContext (NULL, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    prismAssert (NULL != pVcsNodeSlotMessagingContext, __FILE__, __LINE__);

    pVcsNodeSlotMessagingContext->setSlotNumber (slotNumber);
    pVcsNodeSlotMessagingContext->setHwType(hwType);
    pVcsNodeSlotMessagingContext->setMessageType (type);
    pVcsNodeSlotMessagingContext->setIscDestination (reqh);
    pVcsNodeSlotMessagingContext->start ();
}

void VcsNodeFabosInterfaceObjectManager::getSlotObjects (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    
    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFabosInterfaceObjectManager::getSlotObjects: Entering... "));

    UI32       slotNumber       = pVcsNodeSlotMessagingContext->getSlotNumber ();
    UI32       hwType           = pVcsNodeSlotMessagingContext->getHwType ();
    LocationId locationId       = FrameworkToolKit::getMyLocationId (); 
    ObjectId   waveNodeObjectId = FrameworkToolKit::getThisWaveNodeObjectId();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::getSlotObjects: quering slot object for slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());

    // Retrieve the VcsSlotLocalManagedObject
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (VcsSlotLocalManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeUI32 (slotNumber, "slotNumber"));
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (hwType, "hwType"));
    synchronousQueryContext.addAndAttribute(new AttributeObjectId (waveNodeObjectId , "ownerWaveNodeObjectId"));
    
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);
    prismAssert (1 >= pWaveManagedObjects->size (), __FILE__, __LINE__);

    if (0 < pWaveManagedObjects->size ())
    {
        pVcsNodeSlotMessagingContext->setVcsSlotManagedObjectPointer(dynamic_cast<VcsSlotLocalManagedObject *> ((*pWaveManagedObjects)[0]));
    }
    else
    {
        pVcsNodeSlotMessagingContext->setVcsSlotManagedObjectPointer (NULL);

        trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::getSlotObjects: No vcs slot objects found for query slotNumber = ")+slotNumber+" hwType = "+hwType+" locationId = "+locationId+" waveNodeOID = "+waveNodeObjectId.toString  ());
    }

    pWaveManagedObjects->clear ();
    delete (pWaveManagedObjects);

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeFabosInterfaceObjectManager::getSlotType (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    UI32 slotNumber = pVcsNodeSlotMessagingContext->getSlotNumber();
    VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject = pVcsNodeSlotMessagingContext->getVcsSlotManagedObjectPointer ();

    if (NULL != pVcsSlotLocalManagedObject)
    {
        UI32 slotType     = pVcsSlotLocalManagedObject->getHwId();
        UI32 linecardMode = pVcsSlotLocalManagedObject->getLinecardMode();

        pVcsNodeSlotMessagingContext->setHwId(slotType);
        pVcsNodeSlotMessagingContext->setLinecardMode(linecardMode);

        delete (pVcsSlotLocalManagedObject);
        pVcsNodeSlotMessagingContext->setVcsSlotManagedObjectPointer (NULL);

        pVcsNodeSlotMessagingContext->setWaveSlotManagedObjectPointer (NULL);
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::getSlotType slot not found slotNumber = ")+slotNumber);

        pVcsNodeSlotMessagingContext->setHwId (VCS_NODE_SLOT_LINECARD_TYPE_UNKNOWN);
        pVcsNodeSlotMessagingContext->setLinecardMode(VCS_NODE_SLOT_LINECARD_MODE_DEFAULT);
        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void VcsNodeFabosInterfaceObjectManager::replySlotQueryIscMessage (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext)
{
    trace (TRACE_LEVEL_INFO, string("VcsNodeFabosInterfaceObjectManager::replySlotQueryIscMessage : Successfully retrieved the slot type: ") + pVcsNodeSlotMessagingContext->getHwId ());
    
    UI32 rc;
    sqry_t sqry;
    blcd_t *blcd = &sqry.sq_cdata;
    UI32 mode = VCS_NODE_SLOT_LINECARD_MODE_DEFAULT; 
    //UI32 type = 0;

    sqry.sq_slotno = pVcsNodeSlotMessagingContext->getSlotNumber ();
    sqry.sq_hwtype = pVcsNodeSlotMessagingContext->getHwType ();
    blcd->bc_id = pVcsNodeSlotMessagingContext->getHwId ();

    // Convert the VcsNodeSlotLinecardMode enum to the constants that fos is expecting
    mode = pVcsNodeSlotMessagingContext->getLinecardMode ();
    blcd->bc_mode = mode;
    if (mode == VCS_NODE_SLOT_LINECARD_MODE_10G)
    {
        blcd->bc_mode = TENGIGMODE;
    }

    // TODO: Need to fill with real values after it is available in the database
    blcd->bc_profile = 0;
    blcd->bc_brkout = getBreakoutConfig (pVcsNodeSlotMessagingContext);

    //type = pVcsNodeSlotMessagingContext->getMessageType ();

    int reqh = pVcsNodeSlotMessagingContext->getIscDestination ();
    if (reqh != -1)
    {
        isc_msg_t msg;
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::replySlotQueryIscMessage : Sending hwId = ") + blcd->bc_id + ", linecardMode = " + blcd->bc_mode);
        msg.im_msgp = &sqry;
        msg.im_size = sizeof (sqry_t);
        msg.im_cop = 0;
        rc = isc_send_reply (m_iscHandle, &msg, reqh);
        if (rc < 0)
        {
            WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::replySlotQueryIscMessage : Error: isc_send_reply was unsuccessful.")); 

            pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
        }
 	}
    else
    {
        WaveNs::trace(TRACE_LEVEL_ERROR, string ("VcsNodeFabosInterfaceObjectManager::replySlotQueryIscMessage : Error: req handle destination not found.")); 

        pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }

    pVcsNodeSlotMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

int VcsNodeFabosInterfaceObjectManager::dcmIscReceive(void *ctxt, isc_msg_t *msg, int reqh)
{
    sqry_t *sqry = (sqry_t *)msg->im_msgp;

    switch (msg->im_type) {
    case ISC_TYPE_QUERY_SLOT:
        WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::vcsNodeServe: Received slot-query message for slot number: ") + sqry->sq_slotno);
        (VcsNodeFabosInterfaceObjectManager::getInstance ())->handleSlotTypeMessage (reqh, ISC_TYPE_QUERY_SLOT, sqry->sq_slotno, sqry->sq_hwtype);
        break;

    default:
        WaveNs::trace(TRACE_LEVEL_INFO, string ("VcsNodeFabosInterfaceObjectManager::dcmIscReceive : type not supported."));
        break;
    }
    return (0);
}

void * VcsNodeFabosInterfaceObjectManager::getDcmConfigData (UI32 & dcmConfigDataSize)
{
    dcmConfigDataSize = sizeof (dcmConfigFssSyncData);
    return ((void * ) &dcmConfigFssSyncData);
}

bool VcsNodeFabosInterfaceObjectManager::getIsActiveNode ()
{
    return (m_isActiveNode);
}    

string VcsNodeFabosInterfaceObjectManager::getSwmodelName () const
{
    return m_swmodelName;
}

void VcsNodeFabosInterfaceObjectManager::setSwmodelName (const string &swmodelName)
{
    m_swmodelName = swmodelName;
}

bool VcsNodeFabosInterfaceObjectManager::isWarmRecoveryInvolvedInActivation ()
{
    return (m_isWarmRecoveryInvolvedInActivation);
}

void VcsNodeFabosInterfaceObjectManager::setWarmRecoveryInvolvedInActivation (const bool &warmRecovery)
{
    m_isWarmRecoveryInvolvedInActivation = warmRecovery;
}

bool VcsNodeFabosInterfaceObjectManager::getIssuInProgress ()
{
    bool issuInProgress = false;

    m_issuInProgressMutex.lock ();

    issuInProgress = m_issuInProgress;

    m_issuInProgressMutex.unlock ();

    return (issuInProgress);
}

void VcsNodeFabosInterfaceObjectManager::setIssuInProgress (const bool &issuInProgress)
{
    m_issuInProgressMutex.lock ();

    m_issuInProgress = issuInProgress;

    m_issuInProgressMutex.unlock ();
}

UI64 VcsNodeFabosInterfaceObjectManager::getBreakoutConfig (VcsNodeSlotMessagingContext *pContext)
{
    BitMap breakoutConfig(64);
    breakoutConfig.set(); // set all the bits, as HASM will breakout a port if the corresponding bit is 0
    UI32 swbd = DcmToolKit::getSwBDModel ();
    UI32 slotNumber = pContext->getSlotNumber (); 
    SI32 mappedId = getDomainId ();
    ResourceId isVcsEnabled = DcmToolKit::isVcsEnabled ();
    string regex;

    if(isVcsEnabled == WAVE_PERSISTENCE_CLUSTER_ENABLED)
    {
        // if the local mappedId is 2 and the slot id is 3, then we query for all the connector MOs with connectorName "2/3/xxx"
        regex = getString(mappedId) + string("/") + getString(slotNumber) + string("/%");
    }
    else
    {
        // if the slot id is 3, then we query for all the connector MOs with connectorName "3/xxx"
        regex = getString(slotNumber) + string("/%");
    }

    ConnectorConfigLocalManagedObject *connectorMO = NULL;
    WaveManagedObjectSynchronousQueryContext connectorSyncQueryCtxt (ConnectorConfigLocalManagedObject::getClassName());
    connectorSyncQueryCtxt.addAndAttribute(new AttributeString(regex, "connectorName"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
    connectorSyncQueryCtxt.addAndAttribute(new AttributeBool(true, "breakout"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&connectorSyncQueryCtxt);

    if (pResults == NULL || pResults->size() == 0)
    {
        return breakoutConfig.getElemValue();
    }

    pContext->addManagedObjectsForGarbageCollection(*pResults);

    for (UI32 i = 0; i < pResults->size(); i++)
    {
        connectorMO = dynamic_cast<ConnectorConfigLocalManagedObject*>((*pResults)[i]);
        //get port id
        UI32 port = NsmUtils::getPortId(connectorMO->getName());

        if(port < 1 || port > 64) // just a defensive check, to avoid any unnecessary operations.
            continue;

        //set corresponding bit in breakoutConfig(port id)
        //For Castor platform. 40G interface numbering starts from 49.
        if (swbd == SWBD_CASTOR || swbd == SWBD_CASTORT) {
            breakoutConfig.reset(port-49);
        } else {
            breakoutConfig.reset(port-1);
        }
    }

    pResults->clear();
    delete pResults;

    if (swbd == SWBD_CASTOR || swbd == SWBD_CASTORT) {
        // On Castor platform, there are four 40G interfaces. Hence it uses last 4 bits. Setting all higher bits to zero.
        UI64 brkoutVal = breakoutConfig.getElemValue();

        return (brkoutVal & 0x00F);
    } else {
        return breakoutConfig.getElemValue();
    }
}


}
