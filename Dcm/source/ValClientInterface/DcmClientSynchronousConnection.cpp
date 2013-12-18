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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka, Kuai Yu                           *
 ***************************************************************************/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "ValClientInterface/DcmClientSynchronousConnection.h" 
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugParameters.h"

#include "Interface/ValInterfaceLocalObjectManagerGetInterfaceMessage.h"
#include "Interface/InterfaceInstallDemoObjectMessage.h"

#include "ClientInterface/RAS/RASClient.h"
#include "ClientInterface/RAS/RASClientMessage.h"
#include "LicenseService/Local/LicenseServiceUpdateLicenseFabsysInfoMessage.h"
#include "PhysicalInterface/Local/PhysicalInterfaceMessages.h"

#include "discover/Framework/EventEngineManagementInterfaceMessages.h"
#include "discover/Framework/EventEngineMessages.h"
#include "discover/Framework/MigrationEventsMessage.h"
#include "discover/Framework/discoverMessages.h"
#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalAPPMProfileBulkMessage.h"
#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalAPPMProfileBulkMessage.h"
#include "APPM/Global/APPMGlobalAPPMQosMessage.h"
#include "APPM/Local/APPMVnmLocalCdpNotifyMessage.h"
#include "Ssm/Global/SsmGlobalAddMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocRuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocPolicyToInterfaceMessage.h"
#include "Nsm/Global/NsmGlobalDeletePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalCreatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalCreateVlanIntfMessage.h"
#include "Nsm/Local/NsmUpdatePoPrimaryLinkMessage.h"
#include "Nsm/Local/NsmCreatePhyIntfMessage.h"
#include "Nsm/Local/NsmDeletePhyIntfMessage.h"
#include "Nsm/Local/NsmUpdatePhyIntfMessage.h"
#include "Nsm/Local/NsmUpdateBulkPhyIntfMessage.h"
#include "Nsm/Local/NsmUpdatePhyIntfMessage.h"
#include "Nsm/Local/NsmEventMessage.h"
#include "Nsm/Local/NsmEventPortSecLocalMessage.h"
#include "Nsm/Global/NsmEventPortSecGlobalMessage.h"
#include "Nsm/Local/InterfaceStateReplayServiceIndependentMessage.h"
#include "Nsm/Local/NsmLocalSlotOfflineMessage.h"
#include "Nsm/Local/NsmVlanProvisionLocalMessage.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalUpdateVlanBridgeConfigMessage.h"
#include "VcsFabric/Local/VcsNodeTestClientForDualConnectMessage.h"
#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "VcsFabric/Local/VcsNodeIsReadyMessages.h"
#include "VcsFabric/Local/VcsNodePostBootMessages.h"
#include "VcsFabric/Local/VcsNodeIsDcmdReadyForHasmLaunchMessages.h"
#include "VcsFabric/Local/VcsNodeIsHasmLaunchedMessages.h"
#include "VcsFabric/Local/VcsNodeSlotMessages.h"
#include "VcsFabric/Local/VcsNodeIsNodeInClusterMessage.h"
#include "VcsFabric/Local/VcsNodeIsLocalNodeInClusterMessage.h"
#include "VcsFabric/Local/VcsNodeGetConfigurationNumberMessage.h"
#include "VcsFabric/Local/VcsNodeVcsModeMessages.h"
#include "VcsFabric/Local/VcsNodeGetConfigurationTimeMessage.h"
#include "VcsFabric/Local/VcsNodeIsFabricDistributionEnabledMessage.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "VcsFabric/Local/VcsFabricLocalManagementInterfaceMessages.h"
#include "VcsFabric/Local/VcsNodeCliAllowedFromThisLocationMessage.h"
#include "VcsFabric/Local/VcsFabricDebugClientMessage.h"
#include "VcsFabric/Local/VcsFabricSetTtyNameMessage.h"
#include "VcsFabric/Local/VcsNodeFcfMacMessages.h"
#include "VcsFabric/Local/VcsNodeVcsModeMessages.h"
#include "VcsFabric/Local/VcsNodeRBridgeIdMessages.h"
#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "VcsFabric/Local/VcsNodeVcsIdMessages.h"
#include "VcsFabric/Local/VcsNodeMappedIdMessages.h"
#include "VcsFabric/Local/VcsNodeWWNMessages.h"
#include "VcsFabric/Local/VcsNodeIpMessages.h"
#include "VcsFabric/Global/VcsSnapshotFromFtpMessage.h"
#include "VcsFabric/Local/VcsNodeIsCliAllowedFromThisLocationMessage.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "PortCfg/Local/PortCfgPortCfgCreateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgPortStateUpdateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgTrunkUpdateMessage.h"
#include "Fcoe/Local/FcoeLocalETIntfMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeIntfMessage.h"
#include "AG/Local/AgLocalTypes.h"
#include "AG/Local/AgVFPortMessage.h"
#include "VcsFabric/Local/VcsNodeGvlanMessages.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/InterfaceCommon.h"

#include "Fcoe/Common/FcoePluginTypes.h"

#include "Zone/Global/ZoneBulkConfigurationMessage.h"
#include "SystemManagement/Configuration/CliConfigurationFile.h"

extern "C" {
#include <switch/switch.h>
}

#define NOT_SPECIFIED -2
#define MAC_BULK_REQUEST_SIZE 500

namespace DcmNs
{

DcmClientSynchronousConnection *DcmClientSynchronousConnection::m_pDcmClientSynchronousConnectionForCApi = NULL;
DcmClientSynchronousConnection *DcmClientSynchronousConnection::m_pDcmClientSynchronousConnectionForCApiWithCcm = NULL;

DcmClientSynchronousConnection::DcmClientSynchronousConnection (const string &valClientName, const string &waveServerIpAddress, const UI32 &waveServerPort)
    : WaveClientSynchronousConnection (valClientName, waveServerIpAddress, waveServerPort)
{
}

DcmClientSynchronousConnection::DcmClientSynchronousConnection ()
    : WaveClientSynchronousConnection ()
{
}

DcmClientSynchronousConnection::~DcmClientSynchronousConnection ()
{
}

DcmClientSynchronousConnection *DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForDualDaemons ()
{
    return ((NULL != m_pDcmClientSynchronousConnectionForCApiWithCcm) ? (m_pDcmClientSynchronousConnectionForCApiWithCcm) : (m_pDcmClientSynchronousConnectionForCApi));
}

DcmClientSynchronousConnection *DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ()
{
    return (m_pDcmClientSynchronousConnectionForCApi);
}

void DcmClientSynchronousConnection::initializeCApi (DcmClientSynchronousConnection *pDcmClientSynchronousConnection)
{
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    m_pDcmClientSynchronousConnectionForCApi = pDcmClientSynchronousConnection;
}

void DcmClientSynchronousConnection::unInitializeCApi ()
{
    m_pDcmClientSynchronousConnectionForCApi = NULL;
}

bool DcmClientSynchronousConnection::isCApiInitialized ()
{
    if (NULL == m_pDcmClientSynchronousConnectionForCApi)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

void DcmClientSynchronousConnection::initializeSynchronousConnectionForCcm (DcmClientSynchronousConnection *pDcmClientSynchronousConnection)
{
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    m_pDcmClientSynchronousConnectionForCApiWithCcm = pDcmClientSynchronousConnection;
}

void DcmClientSynchronousConnection::unInitializeSynchronousConnectionForCcm ()
{
    m_pDcmClientSynchronousConnectionForCApiWithCcm = NULL;
}

bool DcmClientSynchronousConnection::isSynchronousConnectionForCcmInitialized ()
{
    return ((NULL == m_pDcmClientSynchronousConnectionForCApiWithCcm) ? false : true);
}

ResourceId DcmClientSynchronousConnection::test_api_for_dual_connection ()
{
    VcsNodeTestClientForDualConnectMessage      message;
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::test_api_for_dual_connection : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::test_api_for_dual_connection : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                trace (TRACE_LEVEL_INFO, string("DcmClientSynchronousConnection::test_api_for_dual_connection : processing directory : ") + message.getString ());
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::valQueryForInterface (const string &interfaceName, ValClientPhysicalInterface &valClientPhysicalInterface)
{
    ValInterfaceLocalObjectManagerGetInterfaceMessage message (interfaceName);
    WaveMessageStatus                                 status;
    ResourceId                                        completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                        clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::valQueryForInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::valQueryForInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                valClientPhysicalInterface.setName      (message.getName      ());
                valClientPhysicalInterface.setIpAddress ((message.getIpAddress ()).toString());
                valClientPhysicalInterface.setMtu       (message.getMtu32u    ());
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::dcmNsmEventNotify (const UI32 &eventid, const UI32 &parameter)
{

	ResourceId             status;
    if (INTERFACE_STATE_REPLAY == eventid) {
        InterfaceStateReplayServiceIndependentMessage msg(eventid, parameter);
        if (true == (isCurrentlyConnected ())) {
            status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &msg, 0);
            if (WAVE_MESSAGE_SUCCESS == status) {
                bool isPluginEnabled = msg.isPluginEnabled();

                if (! isPluginEnabled) {
                    tracePrintf(TRACE_LEVEL_INFO, "**** Nsm Plugin service is NOT Enabled. Event INTERFACE_STATE_REPLAY for slot: %d, can't be processed at this moment\n", parameter);
                    return WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED;
                }
            } else {
                tracePrintf (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::dcmNsmEventNotify : Sending message failed : %s", (FrameworkToolKit::localize (status)).c_str());
                return status;
            }
        } else {
            status = getConnectionStatus ();
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::dcmNsmEventNotify : nsm client not connected: " + FrameworkToolKit::localize (status));
            return status;
        }
    }

	NsmEventMessage	*pMessage = new NsmEventMessage (eventid, parameter);
	if (true == (isCurrentlyConnected ()))
	{
		status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pMessage, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::dcmNsmEventNotify : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = getConnectionStatus ();
		trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::dcmNsmEventNotify : nsm client not connected: " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmClientSynchronousConnection::vnmVcCdpNotify (const UI32 cmdCode, const string ifName, const string hostMor, const string portId, const string vc)
{

	  	VNMDPMessage  *message = new VNMDPMessage();
        WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
        ResourceId clientStatus = WAVE_MESSAGE_ERROR;

		trace (TRACE_LEVEL_DEBUG, string("vnmVcCdpNotify: PUBLISHING VNMDP MESSAGE "));
        if (true == (isCurrentlyConnected())) {

            message->m_cmdCode = cmdCode;
            message->m_ifName = ifName;
            message->m_hostMor = hostMor;
            message->m_portId = portId;
            message->m_vcenter = vc;

			status = (WaveUserInterfaceObjectManager::getInstance())->sendOneWayToWaveServer (getWaveServerId (), message, 0);

            if (WAVE_MESSAGE_SUCCESS != status) {
                trace(
                        TRACE_LEVEL_ERROR,
                        "DcmClientSynchronousConnection::Vcenter : VNMDP Status : Sending message failed : "
                                + FrameworkToolKit::localize(status));
                clientStatus = status;
            }
            else {
                    clientStatus = WAVE_MESSAGE_SUCCESS;
						trace (TRACE_LEVEL_DEBUG, string("Completed!! ::vnmVcCdpNotify: PUBLISHING VNMDP MESSAGE "));
            }
		}

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vnmAppmCdpNotify (const UI32 cmdCode, const string ifName, const string hostMor, const string portId, const string vc)
{
	ResourceId status;
    APPMVnmLocalCdpNotifyMessage *pMessage = new APPMVnmLocalCdpNotifyMessage (cmdCode, ifName, hostMor, portId);
    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pMessage, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vnmAppmCdpNotify: Sending message failed : " + FrameworkToolKit::localize (status));
        } else {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vnmAppmCdpNotify: Send message successfully ");
        }

    } else {
        status = getConnectionStatus ();
        trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::vnmAppmCdpNotify: connection to Wave Server failed: " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmClientSynchronousConnection::createPhysicalInterface (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description)
{
    PhysicalInterfaceCreateMessage message          (name, ifIndex, mtu, description);
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPhysicalInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPhysicalInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsGetThisNodeClusterType (ResourceId &vcsClusterType)
{
    WaveMessageStatus              status           = WAVE_MESSAGE_ERROR;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        VcsNodeGetClusterTypeMessage message;
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetThisNodeClusterType: Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            vcsClusterType = message.getClusterType();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetClusterMode: Message Processing failed : " +
                        FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}

ResourceId DcmClientSynchronousConnection::vcsGetVcsMode (VcsClientVcsModeParams &vcsModeParams)
{
    WaveMessageStatus              status           = WAVE_MESSAGE_ERROR;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        VcsNodeGetVcsModeMessage message;
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetVcsId: Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            vcsModeParams.setVcsMode (message.getVcsMode());

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetVcsId: Message Processing failed : " +
                        FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}

ResourceId DcmClientSynchronousConnection::vcsGetRBridgeId (UI32 &rbridgeId)
{
    WaveMessageStatus              status           = WAVE_MESSAGE_ERROR;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    rbridgeId = 0;

    if (true == (isCurrentlyConnected ()))
    {
        VcsNodeGetRBridgeIdMessage message;
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetRBridgeId: Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();


            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetRBridgeId: Message Processing failed : " +
                        FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                rbridgeId = message.getRBridgeId ();
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}

ResourceId DcmClientSynchronousConnection::isConfigFileAllowed (SI32 &isAllowed)
{
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        VcsCopyToStartupAllowedMessage message;

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            printf ("DcmClientSynchronousConnection::isConfigFileAllowed : Sending message failed : %x\n", status);

            clientStatus = status;
            isAllowed = 0;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("DcmClientSynchronousConnection::isConfigFileAllowed : Message Processing failed : %x\n", completionStatus);
            }
            else
            {
                if(false == message.getIsAllowed ())
                {
                    isAllowed = 0;
                }
                else
                {
                    isAllowed = 1;
                }
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        isAllowed = 0;
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::isCliAllowedAtThisLocation (SI32 &isCliAllowed)
{
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        VcsNodeIsCliAllowedFromThisLocationMessage message;

        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            printf ("DcmClientSynchronousConnection::isCliAllowedAtThisLocation : Sending message failed : %x\n", status);
            clientStatus = status;
            isCliAllowed = 0;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("DcmClientSynchronousConnection::isCliAllowedAtThisLocation : Message Processing failed : %x\n", completionStatus);
            }
            else
            {
                if(false == message.getIsCliAllowedFromThisLocation ())
                {
                    isCliAllowed = 0;
                }
                else
                {
                    isCliAllowed = 1;
                }
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        isCliAllowed = 0;
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsGetVcsId (VcsClientVcsIdParams &vcsIdParams)
{
    WaveMessageStatus              status           = WAVE_MESSAGE_ERROR;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        VcsNodeGetVcsIdMessage message;
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetVcsId: Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            vcsIdParams.setVcsId (message.getVcsId());

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsGetVcsId: Message Processing failed : " +
                        FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}


ResourceId DcmClientSynchronousConnection::createPoInterface (const string &name)
{
    NsmGlobalCreatePoIntfMessage   message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setPoName(name);
    message.setDesc(name);
    message.setPoMode(1);
    message.setPoType(1);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createPoInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createPoInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}


ResourceId DcmClientSynchronousConnection::setVlanProtocalStatus (const string &name, const bool &flag)
{
    NsmGlobalUpdateVlanIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	UI32 vlanId = atoi(name.c_str());

    message.setVlanId(vlanId);
    message.setFlag(flag);
    message.setCmdCode(NSMUPDATE_VLAN_STATUS);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createVlanInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createVlanInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}






ResourceId DcmClientSynchronousConnection::setPoPrimaryLinkDomainId (const string &name, const unsigned int &poPrimaryLinkDomainId,const unsigned int &poPartnerSystemPriority,
                                                         const unsigned int &poPartnerKey, const string &poPartnerSystem, const unsigned int &commandCode,
                                                         unsigned int &replyCode)
{
    NsmUpdatePoPrimaryLinkMessage       message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setPoName(name);
    message.setPoPrimaryLinkDomainId(poPrimaryLinkDomainId);
    message.setPoPartnerSystemPriority(poPartnerSystemPriority);
    message.setPoPartnerKey(poPartnerKey);
    message.setPoPartnerSystem(poPartnerSystem);
    message.setCmdCode(commandCode);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::setPoPrimaryLinkDomainId : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::setPoPrimaryLinkDomainId : completion status: " + FrameworkToolKit::localize (completionStatus));

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                if (WRC_VLAG_REQUEST_FAILED == completionStatus)
                {
                    clientStatus = WAVE_MESSAGE_SUCCESS;
                    replyCode = message.getReplyCode();
                } else {
                    clientStatus = completionStatus;
                    trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::setPoPrimaryLinkDomainId : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
                }
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                replyCode = message.getReplyCode();
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}





ResourceId DcmClientSynchronousConnection::setVlanDesc (const string &name, const string &description)
{
    NsmGlobalUpdateVlanIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	UI32 vlanId = atoi(name.c_str());

    message.setVlanId(vlanId);
    message.setDesc(description);
    message.setCmdCode(NSMUPDATE_VLAN_DESC);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createVlanInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createVlanInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}


ResourceId
DcmClientSynchronousConnection::createFcoeETInterface (const string &name, const UI64 &ifIndex, const UI32 &fcUportNum)
{
    UI32                           oper = FCOE_ET_INTF_OPER_CREATE;
    FcoeLocalETIntfMessage         *msg_p = new FcoeLocalETIntfMessage();
    ResourceId             status = WAVE_MESSAGE_SUCCESS;

    if (true == (isCurrentlyConnected ())) {
        msg_p->setOper_bmp(oper);
        msg_p->setIfName(name);
        msg_p->setFcUportNum(fcUportNum);
        msg_p->setIfIndex(ifIndex);

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createFcoeETInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
DcmClientSynchronousConnection::createFcoeInterface (const string &name, const UI64 &ifIndex,
                                                     const string &bind_name, const UI32 &bind_type,
                                                     const UI32 &fcUportNum, const UI32 &vn_num,
                                                     const UI32 &port_num,
                                                     const string &description)
{

    UI32                           oper = FCOE_INTF_OPER_CREATE;
    FcoeLocalFcoeIntfMessage       *msg_p = new FcoeLocalFcoeIntfMessage(oper, name, FCOE_INTF_STATUS_DEFAULT,
                                                                         ifIndex, fcUportNum, vn_num, port_num,
                                                                         description, FCOE_INTF_DEFAULT_MODE,
                                                                         bind_name, bind_type);
    ResourceId                     status = WAVE_MESSAGE_SUCCESS;

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createFcoeInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
DcmClientSynchronousConnection::createFcoeInterfaceBulk (const void *data_p,
                                                         const unsigned int size)
{

    FcoeLocalFcoeIntfMessage       *msg_p = new FcoeLocalFcoeIntfMessage();
    ResourceId              status = WAVE_MESSAGE_SUCCESS;

    msg_p->setOper_bmp(FCOE_INTF_OPER_CREATE_BULK);
    msg_p->addBuffer(FCOE_INTF_OPER_CREATE_BULK, size,
                      (unsigned char *)data_p, false);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createFcoeInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
DcmClientSynchronousConnection::deleteFcoeInterfaceBulk (const void *data_p,
                                                         const unsigned int size)
{

    FcoeLocalFcoeIntfMessage       *msg_p = new FcoeLocalFcoeIntfMessage();
    ResourceId              status = WAVE_MESSAGE_SUCCESS;

    msg_p->setOper_bmp(FCOE_INTF_OPER_DELETE_BULK);
    msg_p->addBuffer(FCOE_INTF_OPER_DELETE_BULK, size,
                      (unsigned char *)data_p, false);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createFcoeInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
DcmClientSynchronousConnection::createFcoeETInterfaceBulk (const void *data_p,
                                                           const unsigned int size)
{

    FcoeLocalETIntfMessage  *msg_p = new FcoeLocalETIntfMessage();
    ResourceId       status = WAVE_MESSAGE_SUCCESS;

    msg_p->setOper_bmp(FCOE_ET_INTF_OPER_CREATE_BULK);
    msg_p->addBuffer(FCOE_ET_INTF_OPER_CREATE_BULK, size,
                     (unsigned char *)data_p, false);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createFcoeETInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
DcmClientSynchronousConnection::updateFcoeETInterfaceBulk (const void *data_p,
                                                           const unsigned int size,
                                                           const UI32 oper)
{

    FcoeLocalETIntfMessage  *msg_p = new FcoeLocalETIntfMessage();
    ResourceId       status = WAVE_MESSAGE_SUCCESS;

    msg_p->setOper_bmp(oper);
    msg_p->addBuffer(oper, size, (unsigned char *)data_p, false);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::updateFcoeETInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
DcmClientSynchronousConnection::createStickyMacs (const void *data, const unsigned int size)
{
    ResourceId status;
    NsmEventPortSecLocalMessage *pPhyMsg = NULL;
	NsmEventPortSecGlobalMessage *pPoMsg = NULL;
	StickyMacRecordsType type = ((PortSecurityStickyOutputMsg_t *)data)->records_type;

	trace(TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createStickyMacs : Sending MAC records to Nsm plugin...");

	if (type == PO_INTERFACE_TYPE || type == PO_PHY_INTERFACE_TYPE) {
		pPoMsg = new NsmEventPortSecGlobalMessage();

		pPoMsg->setOpCode(STICKY_MAC_CREATE_BULK);
		pPoMsg->addBuffer(STICKY_MAC_CREATE_BULK, size, (unsigned char *)data, false);

		if (true == (isCurrentlyConnected ())) {
            status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pPoMsg, 0);
            if (WAVE_MESSAGE_SUCCESS != status) {
                trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createStickyMacs: Sending message failed : " + FrameworkToolKit::localize (status));
            }
        } else {
            status = getConnectionStatus ();
        }
	}

	if (type == PHY_INTERFACE_TYPE || type == PO_PHY_INTERFACE_TYPE) {
		pPhyMsg = new NsmEventPortSecLocalMessage();

    	pPhyMsg->setOpCode(STICKY_MAC_CREATE_BULK);
    	pPhyMsg->addBuffer(STICKY_MAC_CREATE_BULK, size, (unsigned char *)data, false);

    	if (true == (isCurrentlyConnected ())) {
        	status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pPhyMsg, 0);
        	if (WAVE_MESSAGE_SUCCESS != status) {
            	trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createStickyMacs: Sending message failed : " + FrameworkToolKit::localize (status));
        	}
    	} else {
        	status = getConnectionStatus ();
    	}
	}

    return status;
}

ResourceId
DcmClientSynchronousConnection::createPortCfgInterface (const string &name, const UI32 &fc_uport_num)
{
	PortCfgPortCfgCreateMessage    portCfgCreateMsg;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true != (isCurrentlyConnected ())) {
        return getConnectionStatus ();
    }

    string te_type("TE");
    portCfgCreateMsg.setId(name);
    portCfgCreateMsg.setIsIdCompete(false);
    portCfgCreateMsg.setSwitchPort(fc_uport_num);
    portCfgCreateMsg.setPortType(te_type);
    portCfgCreateMsg.setSwitchPortType(te_type);
    portCfgCreateMsg.setDispId(name);
    //tracePrintf (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createPortCfgInterface: Create PortCfg Interface ID=%s SwPort=%d",
    //             name.c_str(), fc_uport_num);

    status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &portCfgCreateMsg);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPhysicalInterface : portCfgCreateMsg failed : " + FrameworkToolKit::localize (status));

        clientStatus = status;
    }
    else
    {
        completionStatus = portCfgCreateMsg.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            clientStatus = completionStatus;

            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPhysicalInterface : portCfgCreateMsg failed : " + FrameworkToolKit::localize (completionStatus));
        }
        else
        {
            clientStatus = WAVE_MESSAGE_SUCCESS;
        }
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::createPhyInterface (const string &name, const UI64 &ifIndex, const UI32 &mtu, const string &description, const UI32 isl_capable)
{
    NsmCreatePhyIntfMessage*       pMessage=new NsmCreatePhyIntfMessage(name,ifIndex,mtu,description);
    ResourceId                     status = WAVE_MESSAGE_SUCCESS;

    pMessage->setIslCapability(isl_capable);
	string configXpath;
	string strtype = "";
    char  stype[3];
 
    strtype = name.substr(0, 2);
    strncpy(stype, strtype.c_str(), 2);
    stype[2] = '\0';
	if (strncasecmp(stype, "gi", 2) == 0) { 
		configXpath = "/interface/gigabitethernet";	
		pMessage->addXPathStringForTimestampUpdate(configXpath);
	}
	else if (strncasecmp(stype, "te", 2) == 0) {
		configXpath = "/interface/tengigabitethernet";	
		pMessage->addXPathStringForTimestampUpdate(configXpath);
	}
	else if (strncasecmp(stype, "fo", 2) == 0) {
		configXpath = "/interface/fortygigabitethernet";	
		pMessage->addXPathStringForTimestampUpdate(configXpath);
	}
	
    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pMessage, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createPhyInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = getConnectionStatus ();
        trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createPhyInterface : nsm client not connected: " + FrameworkToolKit::localize (status));
    }

    return (status);

}




ResourceId DcmClientSynchronousConnection::dcmNsmNotifyPortroleConfig (const string &name, const unsigned int portrole, unsigned int exception, unsigned int msg_id)
{
    NsmUpdatePhyIntfMessage *pPhyMsg = new NsmUpdatePhyIntfMessage();
    ResourceId             status = WAVE_MESSAGE_SUCCESS;

    pPhyMsg->setIfName(name);
    pPhyMsg->setException(exception);
    pPhyMsg->setMsgId(msg_id);

    if (portrole & HAL_IF_CAP_EDGE)
        pPhyMsg->setCmdCode(NSM_PLUGIN_ISL_TO_EDGE);
    else if(portrole & HAL_IF_CAP_ISL)
        pPhyMsg->setCmdCode(NSM_PLUGIN_EDGE_TO_ISL);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pPhyMsg, 0);

        if (WAVE_MESSAGE_SUCCESS != status)
            trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::dcmNsmNotifyPortroleConfig: Sending message failed : " + FrameworkToolKit::localize (status));
    }
    else
        status = getConnectionStatus ();

    return (status);
}

ResourceId DcmClientSynchronousConnection::dcmNsmModifyInterfaceConfig (void *intfConfig,unsigned int msg_id)
{
    NsmUpdateBulkPhyIntfMessage *pPhyMsg = new NsmUpdateBulkPhyIntfMessage();
    ResourceId             status = WAVE_MESSAGE_SUCCESS;

    nsm_intf_all_msg_t *msg = ((nsm_intf_all_msg_t *)intfConfig);
    if( (msg != NULL )  && (msg->num_records > 0 ) )
    {
        pPhyMsg->setMsgId(msg_id);
        pPhyMsg->setCmdCode(msg->record_type);
        pPhyMsg->loadOutputsFromCStructure((sizeof(nsm_intf_all_msg_t)+ ((msg->num_records - 1) * sizeof (nsm_intf_msg_t))), intfConfig);

        if (true == (isCurrentlyConnected ())) {
            status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pPhyMsg, 0);

            if (WAVE_MESSAGE_SUCCESS != status)
                trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::dcmNsmModifyInterfaceConfig: Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
		{
            status = getConnectionStatus ();
		}
    }
	//This is message should not be freed here as it is freed by sendOneWayToWaveServer
    //delete pPhyMsg;
    return (status);
}

ResourceId DcmClientSynchronousConnection::dcmNsmNotifySlotOffline (const unsigned int cmdcode, const unsigned int slotid)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    NsmLocalSlotOfflineMessage *msg = new NsmLocalSlotOfflineMessage ();
    msg->setCmdCode (cmdcode);
    msg->setSlotId (slotid);

	if (true == (isCurrentlyConnected ()))
	{
		status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::dcmNsmNotifySlotOffline : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = getConnectionStatus ();
		trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::dcmNsmNotifySlotOffline : nsm client not connected: " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmClientSynchronousConnection::dcmNsmNotifyPhyStatusConfig (const string &name, const unsigned int flag, unsigned int msg_id)
{
    NsmUpdatePhyIntfMessage *pPhyMsg = new NsmUpdatePhyIntfMessage();
    ResourceId             status;

    pPhyMsg->setIfName(name);
    pPhyMsg->setFlag(flag);
    pPhyMsg->setMsgId(msg_id);
    pPhyMsg->setCmdCode(NSMUPDATE_PHY_EXCEPTION_STATUS);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pPhyMsg, 0);

        if (WAVE_MESSAGE_SUCCESS != status)
            trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::dcmNsmNotifyPhyStatusConfig: Sending message failed : " + FrameworkToolKit::localize (status));
    }
    else
        status = getConnectionStatus ();

    return (status);
}

ResourceId DcmClientSynchronousConnection::dcmNsmNotifyPoStatusConfig (const string &poName, const unsigned int flag)
{
    NsmGlobalUpdatePoIntfMessage poMsg;
    ResourceId             status           = WAVE_MESSAGE_SUCCESS;
    ResourceId             clientStatus     = WAVE_MESSAGE_ERROR;

    poMsg.setPoId(poName);
    poMsg.setPoName(poName);
    poMsg.setFlag(flag);
    poMsg.setCmdCode(NSMUPDATE_PO_EXCEPTION_STATUS);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &poMsg, 0);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::dcmNsmNotifyPoStatusConfig: Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        } else {
            ResourceId completionStatus = poMsg.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus) {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::dcmNsmNotifyPoStatusConfig: Processing message failed : " + FrameworkToolKit::localize (clientStatus));
            } else {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    } else {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}


ResourceId DcmClientSynchronousConnection::createVlanInterface (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description)
{
	UI32 vlanId = atoi(name.c_str());
    NsmGlobalCreateVlanIntfMessage  message          (vlanId,name,mtu,description);
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createVlanInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createVlanInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }


    return (clientStatus);
}


ResourceId DcmClientSynchronousConnection::copyToStartupConfig (const string &path)
{
    VcsCopyToStartupMessage                     message (path);
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            printf ("DcmClientSynchronousConnection::copyToStartupConfig : Sending message failed : %x\n", status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("DcmClientSynchronousConnection::copyToStartupConfig : Message Processing failed : %x\n", completionStatus);
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::copyRunningToStartup(void)
{
    VcsCopyRunningToStartupMessage              message;
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            printf ("DcmClientSynchronousConnection::copyRunningToStartup : Sending message failed : %x\n", status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("DcmClientSynchronousConnection::copyRunningToStartup : Message Processing failed : %x\n", completionStatus);
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::copyDefaultToStartup(void)
{
    VcsCopyDefaultToStartupMessage              message;
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            printf ("DcmClientSynchronousConnection::copyDefaultToStartup : Sending message failed : %x\n", status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("DcmClientSynchronousConnection::copyDefaultToStartup : Message Processing failed : %x\n", completionStatus);
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::copySnapshotFromFtp ( const string &snapshotId, const UI32 &rbridgeId, const string &localFilename, const string &globalFilename )
{
    VcsCopySnapshotFromFtpMessage               message (snapshotId, rbridgeId, localFilename,globalFilename);
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {   
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            printf ("DcmClientSynchronousConnection::copySnapshotFromFtp : Sending message failed : %x\n", status);

            clientStatus = status;
        }
        else
        {   
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {  
                if (VCS_CONFIG_SNAPSHOT_ID_PRESENT == completionStatus)
                {
                    completionStatus = VCS_CONFIG_SNAPSHOT_ID_PRESENT_DURING_DOWNLOAD;
                }

                clientStatus = completionStatus;

                printf ("%s\n", (FrameworkToolKit::localize (completionStatus)).c_str ());
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::fosCliPortPersistent (const UI32 slot, const UI32 port, const UI32 persiststate )
{
    PortCfgPortCfgPortStateUpdateMessage    portCfgStateUpdateMsg;
    char                           ifName[20];
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;


    if (true == (isCurrentlyConnected ()))
    {
        // construct the interface descriptor string from the slot and port passed in

        sprintf(ifName, "%d/%d", slot, port);
        string passid(ifName);

        portCfgStateUpdateMsg.setId(passid);
        if (persiststate == 1)
        portCfgStateUpdateMsg.setPortStateEnable(true);
        else
        portCfgStateUpdateMsg.setPortStateEnable(false);
        portCfgStateUpdateMsg.setIsIdComplete(false);

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &portCfgStateUpdateMsg);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::fosCliPortPersistent : fosCliPortPersistent failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = portCfgStateUpdateMsg.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, string ("Command did not succeed. Make sure to execute only from VCS mode enabled"));
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::fosCliPortPersistent : portCfgStateUpdateMsg failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::fosCliPortTrunk (const UI32 slot, const UI32 port, const UI32 trunkcapable)
{
    PortCfgPortCfgTrunkUpdateMessage    portCfgTrunkUpdateMsg;
    char                           ifName[20];
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;


    if (true == (isCurrentlyConnected ()))
    {
        // construct the interface descriptor string from the slot and port
        // passed in

        sprintf(ifName, "%d/%d", slot, port);
        string passid(ifName);

        portCfgTrunkUpdateMsg.setId(passid);
        if (trunkcapable == 1)
        portCfgTrunkUpdateMsg.setTrunkEnable(true);
        else
        portCfgTrunkUpdateMsg.setTrunkEnable(false);
        portCfgTrunkUpdateMsg.setIsIdComplete(false);

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &portCfgTrunkUpdateMsg);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::fosCliPortTrunk : fosCliPortTrunk failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = portCfgTrunkUpdateMsg.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, string ("Command did not succeed. Make sure to execute only from VCS mode enabled"));
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::fosCliPortTrunk : portCfgTrunkUpdateMsg failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsRASLogging (vcslog_t *vcslog, const string &vcslogString)
{
    RASClientMessage   *pMessage = new RASClientMessage();
    WaveMessageStatus  status = WAVE_MESSAGE_SUCCESS;
    VcsClientLocalParams            vLocalParams;

    pMessage->getCStructure (vcslog);
    pMessage->m_vcslogString = vcslogString;
    // pMessage->m_data = vcslogString;
    trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsRASLogging : Sending message=" + pMessage->m_swname);


    if (true == (isCurrentlyConnected ()))
    {
    if (WAVE_MESSAGE_SUCCESS == vcsNodeLocalGetMappedId (vLocalParams)) {
            pMessage->setMappedId(vLocalParams.getMappedId());

    } else {
        trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::vcsRASLogging : getMappedId failed : "
           + FrameworkToolKit::localize (status));
        return (status);
    }

    status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pMessage);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::vcsRASLogging : Sending message failed : " + FrameworkToolKit::localize (status));

        }
    }
    return (status);
}



/*ResourceId DcmClientSynchronousConnection::setHasmLaunched (bool &hasmLaunchState)
{
    VcsNodeSetIsHasmLaunchedMessage                     message;
    WaveMessageStatus                                   status;
    ResourceId                                          completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                          clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        message.setIsHasmLaunched (hasmLaunchState);

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::isDcmdReadyForConnection : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::isDcmdReadyForConnection : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::queryLinecardType (const UI32 &slotNumber, UI32 &slotType)
{
    VcsNodeSlotTypeQueryMessage  *pMessage = new VcsNodeSlotTypeQueryMessage(slotNumber);
    WaveMessageStatus                          status;
    ResourceId                                 completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                 clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::verifyLinecardType : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::verifyLinecardType : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                slotType = pMessage->getSlotType();
                trace (TRACE_LEVEL_INFO, string("Connection::queryLinecardType slot:") + slotNumber + string(" type:") + slotType);
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    delete pMessage;
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::notifySystemShutdown (const string &databaseDirectory)
{
    DatabaseObjectManagerShutdownMessage       *pMessage = new DatabaseObjectManagerShutdownMessage (databaseDirectory);

    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::notifySystemShutdown : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::notifySystemShutdown : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    delete pMessage;
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::installDemoObject ()
{
    InterfaceInstallDemoObjectMessage message;

    WaveMessageStatus  status;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::Install Demo Object : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::Install Demo Object : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::dumpObjectLeaks (vector<string> arguments)
{
    WaveMessageStatus               status;
    ResourceId                      completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                      clientStatus     = WAVE_MESSAGE_ERROR;
    PrismServiceId                  prismServiceId;
    vector<PrismServiceId>          serviceIds;
    UI32                            numberofServiceIds;


    if ("all" == arguments[0])
    {
        vector<bool>                    enabledStates;
        vector<bool>                    localServiceStates;
        vector<string>                  cpuAffinities;
        vector<string>                  serviceNames;

        ResourceId serviceIdStatus = getServicesInformation (serviceIds, serviceNames, enabledStates, localServiceStates, cpuAffinities);

        if (WAVE_MESSAGE_SUCCESS != serviceIdStatus)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "Failed to Get Service Id information Error Code %u", serviceIdStatus);
            return (serviceIdStatus);
        }
    }
    else
    {
        prismServiceId   = atoi ((arguments[0]).c_str ());
        serviceIds.push_back (prismServiceId);
    }

    numberofServiceIds = serviceIds.size ();

    tracePrintf (TRACE_LEVEL_INFO, "Total Number of Service are %u", numberofServiceIds);

    for (UI32 j = 0; j < numberofServiceIds; j++)
    {
        if (true == (isCurrentlyConnected ()))
        {
            FrameworkTestabilityManagementInterfaceObjectLeakMessage message;

            message.setPrismServiceId (serviceIds[j]);

            status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::dumpObjectLeaks : Sending message failed : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                completionStatus = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    clientStatus = completionStatus;

                    trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::dumpObjectLeaks : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
                    return (clientStatus);
                }
                else
                {
                    clientStatus = WAVE_MESSAGE_SUCCESS;

                    vector<string>  managedObjectClassNames;
                    vector<string>  managedObjectNames;
                    vector<bool>    queryResults;
                    vector<string>  btStrings;

                    message.getMessageArguments (managedObjectClassNames, managedObjectNames, queryResults, btStrings);

                    UI32 numberOfLeakedObjects = managedObjectClassNames.size ();

                    trace (TRACE_LEVEL_INFO, "Leaked Object Summary For Service : \'" + getServiceNameById (serviceIds[j]) + "\'");

                    for (UI32 i = 0; i < numberOfLeakedObjects; i++)
                    {
                        tracePrintf (TRACE_LEVEL_ERROR, true, false, "%07u %-50s %-50s %-5s", i + 1, managedObjectClassNames[i].c_str (),managedObjectNames[i].c_str (), queryResults[i] ? "TRUE" : "FALSE");

                        if (false == btStrings[i].empty ())
                        {
                            trace (TRACE_LEVEL_INFO, btStrings[i]);
                        }
                    }

                    if (0 < numberOfLeakedObjects)
                    {
                        tracePrintf (TRACE_LEVEL_ERROR, true, false, "Total Number Of Object Leaks found : %u", numberOfLeakedObjects);
                    }
                    else
                    {
                        tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "NO Object Leaks Found.");
                    }
                }
            }
        }
        else
        {
            clientStatus = getConnectionStatus ();
        }
    }

    return (clientStatus);

}*/

/*ResourceId DcmClientSynchronousConnection::messageLeaksStats (vector<string> arguments)
{
    WaveMessageStatus           status;
    ResourceId                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                  clientStatus     = WAVE_MESSAGE_ERROR;
    vector<PrismServiceId>      prismServiceId;

    vector<PrismServiceId>      messageServiceIds;
    vector<UI32>                messageOperationCodes;
    vector<WaveMessageType>     messageTypes;
    vector<string>              btStrings;
    UI32                        numberOfLeakedMessages = 0;
    UI32                        i                      = 0;


    if ("all" == arguments[0])
    {
        vector<bool>                    enabledStates;
        vector<bool>                    localServiceStates;
        vector<string>                  cpuAffinities;
        vector<string>                  serviceNames;

        ResourceId serviceIdStatus = getServicesInformation (prismServiceId, serviceNames, enabledStates, localServiceStates, cpuAffinities);

        if (WAVE_MESSAGE_SUCCESS != serviceIdStatus)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "Failed to Get Service Id information Error Code %u", serviceIdStatus);
            return (serviceIdStatus);
        }
    }
    else
    {
        prismServiceId.push_back (atoi ((arguments[0]).c_str ()));
    }

    UI32 numberOfServiceId = prismServiceId.size ();

    tracePrintf (TRACE_LEVEL_INFO, "Total Number of Service are %u", numberOfServiceId);

    for (UI32 j = 0; j < numberOfServiceId; j++)
    {
        if (true == (isCurrentlyConnected ()))
        {
            FrameworkTestabilityManagementInterfaceMessageLeakMessage message;

            message.setPrismServiceId (prismServiceId [j]);

            status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::Memory Leaks Stats : Sending message failed : " + FrameworkToolKit::localize (status));
                clientStatus = status;
                return (clientStatus);
            }
            else
            {
                completionStatus = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    clientStatus = completionStatus;
                    trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::Memory Leak Stats : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
                }
                else
                {
                    clientStatus = WAVE_MESSAGE_SUCCESS;

                    message.getMessageArguments (messageServiceIds, messageOperationCodes, messageTypes, btStrings);

                    numberOfLeakedMessages = messageServiceIds.size ();

                    trace (TRACE_LEVEL_INFO, "Leaked Message Summary For Service : \'" + getServiceNameById (prismServiceId [j]) + "\'");

                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "_______", "______", "__________________________________________________", "________________", "________________");
                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "   #   ", " S ID ", "                   Service Name                   ", "     OpCode     ", "      Type      ");
                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "_______", "______", "__________________________________________________", "________________", "________________");

                    for (i = 0; i < numberOfLeakedMessages; i++)
                    {
                        tracePrintf (TRACE_LEVEL_ERROR, true, false, "%07u %06u %50s %-16u %-16s", i, messageServiceIds[i], (getServiceNameById (messageServiceIds[i])).c_str (), messageOperationCodes[i], (FrameworkToolKit::localize (messageTypes[i])).c_str ());

                        if (false == btStrings[i].empty ())
                        {
                            trace (TRACE_LEVEL_INFO, btStrings[i]);
                        }
                    }

                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "_______", "______", "__________________________________________________", "________________", "________________");

                    if (0 < numberOfLeakedMessages)
                    {
                        tracePrintf (TRACE_LEVEL_ERROR, true, false, "Total Number Of Message / Event Leaks found : %u", numberOfLeakedMessages);
                    }
                    else
                    {
                        tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "NO Message / Event Leaks Found.");
                    }

                }
            }
        }
        else
        {
            clientStatus = getConnectionStatus ();
        }
    }
    return (clientStatus);
}*/

ResourceId DcmClientSynchronousConnection::podUpdateLicenseData (const char *setName, const int len, const char *configString)
{
	// declare the plugin message object
	LicenseServiceUpdateLicenseFabsysInfoMessage    *licenseUpdateMsg;
	char                           ifName[20];
	WaveMessageStatus              status;
	int							   userPortNo;
	ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
	ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	int							   portList[255];
	int							   portCount = 0;
	char						   pod_assigntype[30];
	int							   portIndex;
	string						   podSet;

	// SEG: notification of successful entry into podUpdateLicenseData()
	trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::podUpdateLicenseData  : invoked");

	if (true == (isCurrentlyConnected ()))
	{
		// notification of being connected
		trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::podUpdateLicenseData : connected");

		// extract the port numbers from the configString
		portCount = extractPortNumbers(configString, portList);

		// for every port in the list, update the associated set name type (single/double/etc.)
		for (portIndex = 0; portIndex < portCount; portIndex++)
		{
			licenseUpdateMsg = new LicenseServiceUpdateLicenseFabsysInfoMessage;
			if (licenseUpdateMsg == NULL)
				break;   // unable to allocate a new message...

			// construct the interface descriptor string from the port passed in
			// note that this feature only runs on pizzaboxes, so slot number will always be zero
			userPortNo = portList[portIndex];

			sprintf(ifName, "%d/%d", 0, userPortNo);  // construct the interface identifier
			string passid(ifName);

			snprintf(pod_assigntype, 30, "%s", (char *) setName);
			if (strcasecmp (pod_assigntype, "POD.reservations.base")  == 0) {
				podSet = "Base";
			} else if (strcasecmp (pod_assigntype, "POD.reservations.single") == 0) {
				podSet = "Single";
			} else if (strcasecmp (pod_assigntype, "POD.reservations.double") == 0) {
				podSet = "Double";
			} else {
				podSet = "None";
			}

			// populate the individual fields of the update license message using
			licenseUpdateMsg->setPortId(passid);
			licenseUpdateMsg->setAssignType(podSet);

			// send the message to the license plugin
			status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), licenseUpdateMsg);

			// check the status of the command execution from the license plugin
			if (WAVE_MESSAGE_SUCCESS != status)
			{
				trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection:: podUpdateLicenseData : podUpdateLicenseData   failed : " + FrameworkToolKit::localize (status));
				delete licenseUpdateMsg;
				return (status);
			}
			else
			{
				// so the message was delivered successfully.  Now see if the data was updated successfully
				completionStatus = licenseUpdateMsg->getCompletionStatus ();

				if (WAVE_MESSAGE_SUCCESS != completionStatus)
				{
					trace (TRACE_LEVEL_INFO, string ("Command did not succeed. Make sure to execute only from VCS mode"));
					trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::podUpdateLicenseData  : licenseUpdateMsg failed : " + FrameworkToolKit::localize (completionStatus));
					delete licenseUpdateMsg;
					return (completionStatus);
				}
				else
				{
					trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::podUpdateLicenseData - Msg Update Successful");
					clientStatus = WAVE_MESSAGE_SUCCESS;
				}
			}
			// free up the license update message to clean up the context
			delete licenseUpdateMsg;
		}
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::podUpdateLicenseData - Connection failure");
		clientStatus = getConnectionStatus ();
	}
	return (clientStatus);
}

// this method is a brute force parser for the config string passed in when link-up events occur.
// it accepts a pointer to a string containing a comma-delimited list of both individual ports
// as well as port ranges.  The individual ports and all of the ports within a given range are
// converted to integers and returned in the portList array parameter
// the method returns the total number of entries extracted
int DcmClientSynchronousConnection::extractPortNumbers (const char *configString, int portList[])
{
	string rawString(configString);
	unsigned int startPos = 0;
	int dashPos = 0;
	int commaPos = 0;
	int portCount = 0;

	while (startPos < rawString.size()) {
		dashPos = rawString.find_first_of("-", startPos);
		commaPos = rawString.find_first_of(",", startPos);
		if ((commaPos == -1) && (dashPos == -1)) {
			// this is the last number in the string
			string finalNum(rawString, startPos, rawString.size()-startPos);
			portList[portCount++] = atoi(finalNum.c_str());
		} else if (((commaPos < dashPos) && (commaPos != -1)) || (dashPos == -1)) {
			// single value only
			std::string singleNum(rawString, startPos, commaPos-startPos);
			portList[portCount++] = atoi(singleNum.c_str());
		} else {
			// a range of values separated by a dash -- get the first and last numbers
			// and add those as well as the numbers in-between to the port list
			int startNum, endNum;
			string firstNum(rawString, startPos, dashPos-startPos);
			commaPos = rawString.find_first_of(",", dashPos);
			string lastNum(rawString, dashPos+1, commaPos-(dashPos+1));
			startNum = atoi(firstNum.c_str());
			endNum = atoi(lastNum.c_str());
			if (endNum >= startNum) {
				// don't process if end number is less than start number
				for (int i=startNum; i<=endNum; i++)
					portList[portCount++] = i;
			}
		}

		// check for end of string condition
		if (commaPos == -1)
			break;

		// update the start position for the search functions
		startPos = commaPos + 1;
	}

	return (portCount);
}

/*ResourceId  DcmClientSynchronousConnection::configureFileService(const string &sArgument)
{

    WaveMessageStatus  status = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;
    bool               bFileServiceEnable = true;

    if (sArgument.compare("disable") == 0)
    {
        bFileServiceEnable = false;
    }

    FrameworkTestabilityManagementInterfaceFileServiceUsageMessage fsmessage;
    fsmessage.setFileServiceUseforFileTransferFlag(bFileServiceEnable);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &fsmessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::configureFileService : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = fsmessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::configureFileService : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}*/

/*ResourceId DcmClientSynchronousConnection::changeBtTrackerState (vector<string> arguments)
{
    ResourceId  status           = WAVE_MESSAGE_ERROR;
    ResourceId  completionStatus = WAVE_MESSAGE_ERROR;

    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::changeBtTrackerState : <cmdname> <object/message> <on/offAndClear>");
        return status;
    }

    FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage *pMessage = new FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage;

    if ("object" == arguments[0])
    {
        pMessage->setLeakType (OBJECT_LEAK);
    }
    else if ("message" == arguments[0])
    {
        pMessage->setLeakType (MESSAGE_LEAK);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::changeBtTrackerState : incorrect leak type specified");
        return status;
    }

    if ("on" == arguments[1])
    {
        pMessage->setBtTrackerState (true);
    }
    else if ("offAndClear" == arguments[1])
    {
        pMessage->setBtTrackerState (false);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::changeBtTrackerState : incorrect bt tracker state type specified");
        return status;
    }

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), pMessage);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::changeBtTrackerState : Sending message failed : " + FrameworkToolKit::localize (status));
            return status;
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::changeBtTrackerState : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::changeBtTrackerState : operation successful");
            }
            return completionStatus;
        }
    }
    else
    {
        status = getConnectionStatus ();
        trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::changeBtTrackerState : Connection error : "+ FrameworkToolKit::localize (status));
        return status;
    }


}*/

/*ResourceId DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId (WorldWideName &worldWideName, const UI32 rBridgeId)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsNodeGetSwitchWWNFromRBridgeIdMessage getSwitchWWNMessage;
    getSwitchWWNMessage.setRBridgeId(rBridgeId);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getSwitchWWNMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getSwitchWWNMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                WorldWideName switchWWN = getSwitchWWNMessage.getSwitchWWN ();

                if (WorldWideName::isValidWorldWideNameString (switchWWN.toString ()))
                {
                    trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Switch WWN (" + switchWWN.toString () + ") validation check passed for given RBridge-Id : " + rBridgeId);
                    worldWideName = switchWWN;

                    clientStatus = WAVE_MESSAGE_SUCCESS;
                }
                else
                {
                    trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Switch WWN validation check failed for given RBridge-Id : ") + rBridgeId);

                    clientStatus = WAVE_MESSAGE_ERROR;
                }
            }
            else
            {
                // Assuming failure of message completion due to switch WWN not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_SWITCH_WWN_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN (UI32 &rBridgeId, const WorldWideName &worldWideName)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsNodeGetRBridgeIdFromSwitchWWNMessage getRBridgeIdMessage;
    getRBridgeIdMessage.setSwitchWWN (worldWideName);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getRBridgeIdMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getRBridgeIdMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                rBridgeId = getRBridgeIdMessage.getRBridgeId ();

                trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : RBridge-Id (") + rBridgeId + ") found for given switch WWN : " + worldWideName.toString ());

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // Assuming failure of message completion due to RBridge-Id not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_RBRIDGE_ID_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

ResourceId  DcmClientSynchronousConnection::isNodeInCluster (const unsigned int rbrigdeId, bool &inCluster)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsNodeIsNodeInClusterMessage message(rbrigdeId);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isNodeInCluster : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isNodeInCluster : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isNodeInCluster : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    inCluster = message.getIsNodeInCluster();
    return (clientStatus);
}

ResourceId  DcmClientSynchronousConnection::isLocalNodeInCluster (bool &isInCluster)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsNodeIsLocalNodeInClusterMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isLocalNodeInCluster : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isLocalNodeInCluster : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isNodeInCluster : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    isInCluster = message.getIsLocalNodeInCluster ();
    return (clientStatus);
}

ResourceId  DcmClientSynchronousConnection::getConfigurationNumber (UI16 configurationNumber[16])
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsNodeGetConfigurationNumberMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == status)
            {
                for (UI32 i = 0; i < 16; i++)
                {
                    configurationNumber [i] = 0;
                }

                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::getConfigurationNumber : Sending message failed : " + FrameworkToolKit::localize (status));
            }

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::getConfigurationNumber : completionStatus : " + FrameworkToolKit::localize (completionStatus));

                if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == completionStatus)
                {
                    for (UI32 i = 0; i < 16; i++)
                    {
                        configurationNumber [i] = 0;
                    }

                    completionStatus = WAVE_MESSAGE_SUCCESS;
                }

                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::getConfigurationNumber : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                message.getConfigurationNumber (configurationNumber);
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

/*ResourceId  DcmClientSynchronousConnection::setNewShowVcs (bool newShowVcs)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsNodeSetNewShowVcsFormatMessage message ( newShowVcs );

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setNewShowVcs : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setNewShowVcs : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

ResourceId  DcmClientSynchronousConnection::getConfigurationTime (char *p_configurationTime, const int configurationTimeStringSize)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;
    string             configurationTime;

    VcsNodeGetConfigurationTimeMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::getConfigurationTime : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::getConfigurationTime : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                configurationTime = message.getConfigurationTime ();
                if ((UI32) configurationTimeStringSize < configurationTime.size ())
                {
                    clientStatus = WAVE_MESSAGE_ERROR;
                    trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::getConfigurationTime : passed string length is not enough to store DateTime");
                }
                else
                {
                    strcpy (p_configurationTime, configurationTime.c_str ());
                }
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

/*ResourceId DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId (const unsigned int rBridgeId, string &ipAddress)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsNodeGetSwitchIPFromRBridgeIdMessage getSwitchIPMessage;
    getSwitchIPMessage.setRBridgeId(rBridgeId);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getSwitchIPMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getSwitchIPMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                string switchIpAddr = getSwitchIPMessage.getSwitchIP ();

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Switch IP (" + switchIpAddr + ") validation check passed for given RBridge-Id : " + rBridgeId);
                ipAddress = switchIpAddr;

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // Assuming failure of message completion due to switch IP not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_SWITCH_IP_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::isClusterConnectedtoIpAddress (const string &ipAddress, vector<IpAddressPingResponseParameters*>& connectedNodesList)
{

    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsIsIpReachableFromClusterMessage isIpReachableFromClusterMessage(ipAddress);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &isIpReachableFromClusterMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = isIpReachableFromClusterMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                vector<UI32>rbridgeIdVec  = isIpReachableFromClusterMessage.getPingResponseForNode();
                for(UI32 i=0;i<rbridgeIdVec.size();i++)
                {
                    IpAddressPingResponseParameters* temp = new IpAddressPingResponseParameters;

                    
                    //  Set the ping response in out param.
                    // IpAddressPingResponseParameters is used to add other ping attributes later without interface changes.
                    
                    temp->rbridgeId = rbridgeIdVec[i];
                    connectedNodesList.push_back(temp);
                }
            }
            else
            {
                // Assuming failure of message completion due to switch IP not found in VcsNodeLocalMO's
                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::isClusterConnectedtoIpAddress : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::isClusterConnectedtoIpAddress : Message processing failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::isClusterConnectedtoIpAddress : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN (UI32 &rbridgeId, const WorldWideName &wwn)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsGetClusterRbridgeIdFromWWNMessage getClusterRbridgeIdFromWWNMessage (rbridgeId, wwn);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getClusterRbridgeIdFromWWNMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getClusterRbridgeIdFromWWNMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                rbridgeId = getClusterRbridgeIdFromWWNMessage.getRbridgeId ();

                trace (TRACE_LEVEL_DEBUG, string ("DcmClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: rbridge-Id (") + rbridgeId + string (") allocated for the WWN (") + wwn.toString () + string (")"));

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                completionStatus = VAL_ALL_RBRIDGE_ID_IN_USE;

                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}*/

ResourceId  DcmClientSynchronousConnection::isGvlanCapable (bool &gvlanCapable)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsNodeIsGvlanCapableMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::isGvlanCapable : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::isGvlanCapable : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_INFO, string("ValClientSynchronousConnection::isGvlanCapable : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    gvlanCapable = message.isGvlanCapable ();
    return (clientStatus);
}

ResourceId  DcmClientSynchronousConnection::isGvlanEnabled (bool &gvlanEnabled)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsNodeIsGvlanEnabledMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::isGvlanEnabled : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::isGvlanEnabled : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_INFO, string("ValClientSynchronousConnection::isGvlanEnabled : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    gvlanEnabled = message.isGvlanEnabled ();
    return (clientStatus);
}

ResourceId  DcmClientSynchronousConnection::isFabricDistributionEnabled (bool &isDistributionEnabled)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsNodeIsFabricDistributionEnabledMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isFabricDistributionEnabled : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isFabricDistributionEnabled : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::isFabricDistributionEnabled : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    isDistributionEnabled = message.isFabricDistributionEnabled ();
    return (clientStatus);
}

ResourceId  DcmClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds (vector<UI32> &rbridgeIds, vector<LocationId> &locationIds)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    rbridgeIds = message.getRbridgeIds ();
    locationIds = message.getLocationIds ();

    trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : Size of rbridge id vector: ") + rbridgeIds.size () + ", Size of location id vector: " + locationIds.size ());

    return (clientStatus);
}

//
// Name: runDebugScript
// Description: We will convert the input rbridgeIdRange into location ids here in the Dcm Client layer.
//              The reason is because the Wave Framework has it's own independent way to distinguish nodes
//              by location id. In this method we will do the rbridge id to location id conversion and then
//              delegate the work to the Wave Client's parent method.
//
// @param fullyQualifiedScriptFileName: input script file
// @param nosOrbash:  is the script nos or shell script
// @param fullyQualifiedOutputFileName : output file
// @param rbridgeIdRange : range of rbridge ids
//
// @return ResourceId: success or failure
//
ResourceId DcmClientSynchronousConnection::runDebugScript (const string & fullyQualifiedScriptFileName, const string & nosOrbash, const string & fullyQualifiedOutputFileName, const string & rbridgeIdRange)
{
    trace (TRACE_LEVEL_DEVEL, string("DcmClientSynchronousConnection::runDebugScript : Entering..."));

    ResourceId              clientStatus     = WAVE_MESSAGE_ERROR;

    vector<UI32>            rbridgeIds;
    vector<LocationId>      locationIds;

    UI32                    numberOfRbridges = 0;
    UI32                    numberOfLocations = 0;

    map<UI32, LocationId>   mapOfRbridgeIdsToLocationIds;
    map<LocationId, UI32>   mapOfLocationIdsToRbridgeIds;

    string                  tempRbridgeIdRange = rbridgeIdRange;
    string                  commaDelimitedLocationIds = "";

    // First we need to retrive all Rbridge Ids and their corresponding
    // Location Ids so that we can easily convert between two.

    clientStatus = getCurrentRbridgeIdsAndLocationIds (rbridgeIds, locationIds);

    if (WAVE_MESSAGE_SUCCESS != clientStatus)
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::runDebugScript : getCurrentRbridgeIdsAndLocationIds Status : ") + FrameworkToolKit::localize (clientStatus));

        return (clientStatus);
    }


    // Build a map and reverse map of Rbridge Ids and Location Ids
    // The Location Ids to Rbridge Ids map will be used later to convert
    // the location Id tagged response output back to rbridge Ids.

    numberOfRbridges = rbridgeIds.size ();
    numberOfLocations = locationIds.size ();

    if (numberOfRbridges != numberOfLocations)
    {
        trace (TRACE_LEVEL_FATAL, string("DcmClientSynchronousConnection::runDebugScript : Mismatch detected between number of Rbridge Ids / Location Ids : ") + numberOfRbridges + " / " + numberOfLocations);
        prismAssert (false, __FILE__, __LINE__);
    }

    for (UI32 i = 0; i < numberOfRbridges; ++i)
    {
        trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::runDebugScript : Current Rbridge Id / Location Id : ") + rbridgeIds[i] + " / " + locationIds[i]);

        mapOfRbridgeIdsToLocationIds[rbridgeIds[i]] = locationIds[i];
        mapOfLocationIdsToRbridgeIds[locationIds[i]] = rbridgeIds[i];
    }


    // Case 1: rbridgeIdRange = ""          => Run command/script on this node's only
    //      For Case 1, we will simply get this node's rbridge id and fall into Case 3.
    // Case 2: rbridgeIdRange = "all"       => Run command/script on all nodes in cluster
    // Case 3: rbridgeIdRange = "1,4-6,10"  => Run command/script on specified nodes

    trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::runDebugScript : Rbridge Id Range : \"") + tempRbridgeIdRange + "\"");

    if ("" == tempRbridgeIdRange)
    {
        // Case 1: Get this node's rbridge-id

        UI32 thisNodeRbridgeId = 0;

        clientStatus = vcsGetRBridgeId (thisNodeRbridgeId);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::runDebugScript : vcsGetRbridgeId Status : ") + FrameworkToolKit::localize (clientStatus));

            return (clientStatus);
        }

        UI32UC tempUI32 (thisNodeRbridgeId);

        tempRbridgeIdRange = tempUI32.toString ();

        // Case 1 will now fall into Case 3.
    }


    if ("all" == tempRbridgeIdRange)
    {
        // Case 2: Use ALL the found Location Ids for runnning the debug script

        sort (locationIds.begin (), locationIds.end ());

        commaDelimitedLocationIds = UI32Range::getUI32RangeStringFromVector (locationIds);
    }
    else
    {
        // Case 3: Parse the Rbridge-Id range and convert them to Location Ids for a filtered debug script run.

        // FIXME: Brian - Here is a potential assert in UI32Range if the string range is NOT in increasing order.
        UI32Range range (tempRbridgeIdRange);

        vector<UI32> tempRbridgeIdVector;
        range.getUI32RangeVector (tempRbridgeIdVector);

        vector<LocationId>              tempLocationIdVector;

        for (UI32 i = 0; i < static_cast<UI32> (tempRbridgeIdVector.size ()); ++i)
        {
            map<UI32, LocationId>::iterator element = mapOfRbridgeIdsToLocationIds.find (tempRbridgeIdVector[i]);
            map<UI32, LocationId>::iterator end     = mapOfRbridgeIdsToLocationIds.end ();

            if (element != end)
            {
                tempLocationIdVector.push_back (mapOfRbridgeIdsToLocationIds[tempRbridgeIdVector[i]]);

                trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::runDebugScript : Found location id : ") + mapOfRbridgeIdsToLocationIds[tempRbridgeIdVector[i]] + " for Rbridge-id : " + tempRbridgeIdVector[i]);
            }
            else
            {
                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::runDebugScript : Could not find a corresponding location id for Rbridge-id : ") + tempRbridgeIdVector[i]);
            }
        }

        sort (tempLocationIdVector.begin (), tempLocationIdVector.end ());

        commaDelimitedLocationIds = UI32Range::getUI32RangeStringFromVector (tempLocationIdVector);
    }

    trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::runDebugScript : Location Range : \"") + commaDelimitedLocationIds + "\"");


    // Create input script file

    string  commandLineSeparator    = "";

    if ("noscli" == nosOrbash)
    {
        commandLineSeparator = " ; ";
    }
    else
    {
        commandLineSeparator = "";
    }

    ifstream  scriptFile( fullyQualifiedScriptFileName.c_str ());

    string inputStringFromScript;
    string line;

    while (getline(scriptFile,line))
    {
        inputStringFromScript = inputStringFromScript + line + commandLineSeparator;
    }


    // Now invoke parent's runDebugScript since we have converted rbridge-ids to locations ids.

    DistributedDebugParameters distributedDebugParameters (inputStringFromScript, nosOrbash, commaDelimitedLocationIds);

    clientStatus = WaveClientSynchronousConnection::runDebugScript (distributedDebugParameters);

    if (WAVE_MESSAGE_SUCCESS == clientStatus)
    {
        vector<LocationId>  newLocationIds      = distributedDebugParameters.getLocationsToRunDebugScript ();
        vector<string>      outputStringVector  = distributedDebugParameters.getOutputStringVector ();
        string              outputString        = "";

        trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::runDebugScript : locations vector size : ") + locationIds.size () + ", output string vector size : " + outputStringVector.size ());


        // Combine output from all locations

        for (UI32 i = 0; i < static_cast<UI32> (newLocationIds.size ()); ++i)
        {
            UI32 convertedRbridgeId (mapOfLocationIdsToRbridgeIds[newLocationIds[i]]);

            char rbridgeIdInfoString[256];
            sprintf (rbridgeIdInfoString, "\nOutput for Rbridge-id %d:\n", convertedRbridgeId);

            string tempString = rbridgeIdInfoString;
            tempString += outputStringVector[i];

            trace (TRACE_LEVEL_DEBUG, string ("DcmClientSynchronousConnection::runDebugScript : Location Id: ") + newLocationIds[i] + tempString);

            outputString += tempString;
        }

        if ("" != fullyQualifiedOutputFileName)
        {
            // An output filename was provided so we will write the output string to file

            ofstream outputFile;
            outputFile.open (fullyQualifiedOutputFileName.c_str (), ios::out);

            if(outputFile)
            {
                outputFile << outputString;
                outputFile.close ();
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "DcmClientSynchronousConnection::runDebugScript : Could not open output file" );
                clientStatus = WAVE_MESSAGE_ERROR;
            }
        }
        else
        {
            // No output file name provided so we will write the output string to stdout

            cout << outputString;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DcmClientSynchronousConnection::runDebugScript : Call to parent WaveClientSynchronousConnection::runDebugScript returned: ") + FrameworkToolKit::localize (clientStatus));
    }

    return (clientStatus);
}

/*ResourceId  DcmClientSynchronousConnection::disconnectInstanceClients (vector<string> arguments)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;
    FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage message;
    ResourceId       completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId       clientStatus     = WAVE_MESSAGE_ERROR;

    if (arguments.size () == 0) {
	    WaveNs:: trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::disconnectInstnaceClients: ipaddress missing"));
	    return (clientStatus);
    }

    WaveNs::trace (TRACE_LEVEL_INFO, string ("ManagementInterfaceObjectManager::removeInstanceClient: ") +
	   string("clientIpAddress:") + arguments[0]);

   message.setClientIpAddress (arguments[0]);


    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs:: trace (TRACE_LEVEL_ERROR, string ("DcmClientSynchronousConnection::disconnectInstnaceClients : Sending message failed :") + status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                 WaveNs:: trace (TRACE_LEVEL_ERROR, string ("DcmClientSynchronousConnection::disconnectInstnaceClients : Message Processing failed :") + completionStatus);
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::createPortProfile (const string &portProfileName)
{
    APPMGlobalAPPMProfileMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_CREATE);
    message.setCmdCode     (PORT_PROFILE_CREATE);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::deletePortProfile (const string &portProfileName)
{
    APPMGlobalAPPMProfileMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_REMOVE);
    message.setCmdCode     (PORT_PROFILE_DELETE);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::deletePortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::deletePortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::createVlanProfileForPortProfile (const string &portProfileName)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_CREATE);
    message.setCmdCode     (VLAN_PROFILE_CREATE);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::setSwitchPortForVlanProfileForPortProfile (const string &portProfileName)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (VLAN_SW_MODE_FLAG);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_ACCESS);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::setSwitchPortModeAccessForVlanProfileForPortProfile (const string &portProfileName)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (VLAN_SW_MODE);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_ACCESS);


    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortModeAccessForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortModeAccessForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::setSwitchPortAccessVlanForVlanProfileForPortProfile (const string &portProfileName, const UI32 &vlanId)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (VLAN_ACCESS_VLAN);
    message.setAccessVlan  (vlanId);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortAccessVlanForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortAccessVlanForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile (const string &portProfileName)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (VLAN_SW_MODE);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_TRUNK);


    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::setSwitchPortAllowedVlansForVlanProfileForPortProfile (const string &portProfileName, const UI32Range &allowedVlans)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (VLAN_IS_ALLOWED_VLAN_ADD);
    message.setSwModeFlag  (true);
    message.setVlanRange   (allowedVlans);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::activatePortProfile (const string &portProfileName)
{
    APPMGlobalAPPMProfileMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setIsActivated (true);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (PORT_PROFILE_ACTIVATE);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::activatePortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::activatePortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::deactivatePortProfile (const string &portProfileName)
{
    APPMGlobalAPPMProfileMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setIsActivated (false);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (PORT_PROFILE_DEACTIVATE);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::deactivatePortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::deactivatePortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::associateMacToPortProfile (const string &portProfileName, const string &macAddress)
{
    APPMGlobalAPPMProfileMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setProfileMac  (macAddress);
    message.setOpCode      (APPM_SET_ELEM);
    message.setCmdCode     (PORT_PROFILE_ASSOCIATE);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::createAccessVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32 &vlanId)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::  createAccessVlanProfileWithVlanForPortProfile" );
    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_ACCESS_VLAN_PROFILE_CREATE);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_ACCESS);
    message.setAccessVlan  (vlanId);

    if (true == (isCurrentlyConnected ()))
    {
      trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection:: Sending sendSynchronouslyToWaveServer....." );
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::createSwitchportTrunkNativeVlanProfile (const string &portProfileName, const UI32 &vlanId)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN);
	message.setNativeVlanId(vlanId);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);

}*/

/*ResourceId DcmClientSynchronousConnection::updateAccessVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32 &vlanId)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_ACCESS_VLAN_PROFILE_UPDATE);
	message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_ACCESS);
    message.setAccessVlan  (vlanId);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::createBulkVlanInterface (const UI32Range &vlanRange)
{
    ResourceId    clientStatus = WAVE_MESSAGE_SUCCESS;
    vector <UI32> vlanCreateVector;
    vlanRange.getUI32RangeVector(vlanCreateVector);

    vector<UI32>::iterator vlaniter;

    for (vlaniter=vlanCreateVector.begin(); vlaniter != vlanCreateVector.end(); vlaniter++) {
            string vlanStr = (Integer (*vlaniter)).toString();
			trace (TRACE_LEVEL_INFO, "creating vlan: " + vlanStr);
            createVlanInterface (vlanStr,0,0,"");
        }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::createTrunkVlanAllProfilePortProfile(const string &portProfileName)
{

    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;
    message.setProfileName (portProfileName);
	message.setOpCode      (APPM_VCENTER);
	message.setCmdCode     (VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL);
	message.setSwModeFlag  (true);
	message.setSwMode      (VLAN_SW_MODE_TRUNK);
	message.setIsAllowedVlanAll (true);

	if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);

}*/

/*ResourceId DcmClientSynchronousConnection::createTrunkVlanNoneProfilePortProfile(const string &portProfileName)
{

    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;
    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_TRUNK);
    message.setIsAllowedVlanNone (true);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);

}*/

/*ResourceId DcmClientSynchronousConnection::createTrunkVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32Range &allowedVlans)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_TRUNK_VLAN_PROFILE_CREATE);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_TRUNK);
    //message.setCmdCode     (VLAN_IS_ALLOWED_VLAN_ADD);
    message.setVlanRange   (allowedVlans);

    //createBulkVlanInterface (allowedVlans);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
		if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::updateTrunkVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32Range &allowedVlans)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_TRUNK_VLAN_PROFILE_UPDATE);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_TRUNK);
    //message.setCmdCode     (VLAN_IS_ALLOWED_VLAN_ADD);
    message.setVlanRange   (allowedVlans);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
				trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::deleteVlanProfileForPortProfile (const string &portProfileName)
{

    APPMGlobalAPPMVlanMessage    message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_VLAN_PROFILE_DELETE);
    message.setProfileName (portProfileName);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::associateMacToVcenterPortProfile(const string &portProfileName, const string &macAddress)
{
    APPMGlobalAPPMProfileMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setProfileMac  (macAddress);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_ASSOC_MAC);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToVcenterPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::associateMacsToPortProfile (const string &portProfileName, vector <string> &macAddressList)
{
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;
	vector <string> macVector;

	int macSize = macAddressList.size();
	int numberOfMessages = (macSize/MAC_BULK_REQUEST_SIZE) + 1;
	// breaking macAddressList into requests of MAC_BULK_REQUEST_SIZE each and sending the messages in loop
	for(int j = 0; j<numberOfMessages; j++)
	{
		 for(int k =j*MAC_BULK_REQUEST_SIZE;( k < (j*MAC_BULK_REQUEST_SIZE)+MAC_BULK_REQUEST_SIZE) && (k < macSize); k++)
		   {
		     macVector.push_back(macAddressList[k]);
		   }
		 trace (TRACE_LEVEL_INFO,string("DcmClientSynchronousConnection::associateMacsToPortProfile:: mac in vector= ") + macVector.size());

	if(macVector.size() > 0)
	{
		APPMGlobalAPPMProfileBulkMessage message;
		message.setProfileName (portProfileName);
	  	message.setOpCode      (APPM_VCENTER);
		message.setCmdCode     (VCENTER_ASSOC_MAC);	
		message.setMacCount    (macVector.size());
		message.setProfileMacs (macVector);

	    if (true == (isCurrentlyConnected ()))
    	{
			trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacsToPortProfile::Sending message" );
        	status = sendSynchronouslyToWaveServer (&message);
	        if (WAVE_MESSAGE_SUCCESS != status)
    	    {
        	    trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToVcenterPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            	clientStatus = status;
	        }
    	    else
        	{
            	completionStatus = message.getCompletionStatus ();
	            if (WAVE_MESSAGE_SUCCESS != completionStatus)
    	        {
        	        clientStatus = completionStatus;
            	    trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
	            }
    	        else
        	    {
            	    clientStatus = WAVE_MESSAGE_SUCCESS;
	            }
    	    }
	    }
    	else
	    {
    	    clientStatus = getConnectionStatus ();
	    }
	}
	macVector.clear();
}
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::disassociateMacFromVcenterPortProfile (const string &portProfileName, const string &macAddress)
{
    APPMGlobalAPPMProfileMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setProfileMac  (macAddress);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_DEASSOC_MAC);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::disassociateMacFromVcenterPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::disassociateMacFromVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);

}*/

/*ResourceId DcmClientSynchronousConnection::deAssociateMacsFromPortProfile (const string &portProfileName, vector <string> &macAddressList)
{
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;
    vector <string> macVector;

    int macSize = macAddressList.size();
    int numberOfMessages = (macSize/MAC_BULK_REQUEST_SIZE) + 1;
    // breaking macAddressList into requests of MAC_BULK_REQUEST_SIZE each and sending the messages in loop
    for(int j = 0; j<numberOfMessages; j++)
    {
         for(int k =j*MAC_BULK_REQUEST_SIZE;( k < (j*MAC_BULK_REQUEST_SIZE)+MAC_BULK_REQUEST_SIZE) && (k < macSize); k++)
           {
             macVector.push_back(macAddressList[k]);
           }
         trace (TRACE_LEVEL_INFO,string("DcmClientSynchronousConnection::associateMacsToPortProfile:: mac in vector= ") + macVector.size());

    if(macVector.size() > 0)
    {
        APPMGlobalAPPMProfileBulkMessage message;
        message.setProfileName (portProfileName);
        message.setOpCode      (APPM_VCENTER);
        message.setCmdCode     (VCENTER_DEASSOC_MAC);
        message.setMacCount    (macVector.size());
        message.setProfileMacs (macVector);

        if (true == (isCurrentlyConnected ()))
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacsToPortProfile::Sending message" );
            status = sendSynchronouslyToWaveServer (&message);
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToVcenterPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
                clientStatus = status;
            }
            else
            {
                completionStatus = message.getCompletionStatus ();
                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    clientStatus = completionStatus;
                    trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::associateMacToVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
                }
                else
                {
                    clientStatus = WAVE_MESSAGE_SUCCESS;
                }
            }
        }
        else
        {
            clientStatus = getConnectionStatus ();
        }
    }
    macVector.clear();
	}
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::createQoSProfileWithDefaultCOSForPortProfile (const string &portProfileName, const UI32 &cosValue)
{
    APPMGlobalAPPMQosMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_CREATE);
    message.setCmdCode     (QOS_PROFILE_CREATE);
    message.setQosTrust    (false);
    message.setDefaultCosValue(cosValue);
    message.setFlowControlTxEnable(0);
    message.setFlowControlRxEnable(0);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createQoSProfileWithDefaultCOSForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createQoSProfileWithDefaultCOSForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
			{
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::updateQoSProfileWithDefaultCOSForPortProfile (const string &portProfileName, const UI32 &cosValue)
{
    APPMGlobalAPPMQosMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_CREATE);
    message.setCmdCode     (QOS_DEFAULT_COS);
    message.setQosTrust    (false);
    message.setDefaultCosValue(cosValue);
    message.setFlowControlTxEnable(0);
    message.setFlowControlRxEnable(0);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::updateQoSProfileWithDefaultCOSForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::updateQoSProfileWithDefaultCOSForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::deleteQoSProfileForPortProfile (const string &portProfileName)
{
    APPMGlobalAPPMQosMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_REMOVE);
    message.setCmdCode     (QOS_PROFILE_DEL);

	if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::deleteQoSProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::deleteQoSProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }
    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::sendSyncUpdateToCcmd (const string &peerHaIpAddress, const SI32 &peerHaIpPort, const UI32 &context)
{
    FrameworkObjectManagerCcmdHaSyncUpdateMessage   message;
    WaveMessageStatus                               status;
    ResourceId                                      completionStatus    = WAVE_MESSAGE_SUCCESS;
    ResourceId                                      clientStatus        = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        message.addBuffer (CONTEXT_INFO, sizeof (UI32), (void *) &context, false);

        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::sendSyncUpdateToCcmd : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::sendSyncUpdateToCcmd : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

void DcmClientSynchronousConnection::retrievePowerStatus(
            vnm_req_t *req, UI32 &message_id, vnm_resp_t **resp) {
        EventEngineMessage message;
        WaveMessageStatus status;
        ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;
        ResourceId clientStatus = WAVE_MESSAGE_ERROR;


        if (true == (isCurrentlyConnected())) {
            //TODO
            //Send the correct Cmd Code
            message.setCmdcode(3);

            //Request send as buffer

            message.addBuffer(POWER_PLD_REQ_TAG,sizeof(vnm_req_t),req,false);
            printf("Cokkie %d",req->payload.cookie[0].mig_task_id);
            status = sendSynchronouslyToWaveServer(&message);

            message_id = message.getMessageId();


            if (WAVE_MESSAGE_SUCCESS != status) {
                trace(
                        TRACE_LEVEL_DEBUG,
                        "DcmClientSynchronousConnection::Vcenter : VM Power Status : Sending message failed : "
                                + FrameworkToolKit::localize(status));

                clientStatus = status;
            }
            else {
                completionStatus = message.getCompletionStatus();

                if (WAVE_MESSAGE_SUCCESS != completionStatus) {
                    clientStatus = completionStatus;

                    trace(
                            TRACE_LEVEL_DEBUG,
                            "DcmClientSynchronousConnection::Vcenter : VM Power Status : Message Processing failed : "
                                    + FrameworkToolKit::localize(
                                            completionStatus));
                }
                else { //Completed Successfully

                    clientStatus = WAVE_MESSAGE_SUCCESS;
                    UI32 size;
                    vnm_resp_t *temp_resp = (vnm_resp_t *) message.findBuffer(POWER_PLD_RESP_TAG, size);
                    //Allocate memory to hold the response

                    *resp = (vnm_resp_t *) malloc(size);

                    memcpy(*resp, temp_resp, size);

                }
            }
        }
        else {
            clientStatus = getConnectionStatus();
        }

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            // FIXME : Sagar : We need to handle this case.  Otherwise remove the clientStatus variable.
        }
    }

/*ResourceId DcmClientSynchronousConnection::dcmvCenterEventNotify(
            const vector<string> &arguments) {


        ResourceId status = WAVE_MESSAGE_SUCCESS;
        //Since the Message is sent using sendOneWay.framework does the deallocation.
        MigrationEventsMessage *pMessage = new MigrationEventsMessage();

        pMessage->eventID = atoi(arguments[0].c_str());
        pMessage->eventChainID = atoi(arguments[1].c_str());
        pMessage->eventType = atoi(arguments[2].c_str());
        pMessage->vCenterIP = arguments[3];

        pMessage->hostMOR = arguments[4];
        pMessage->destHostMOR = arguments[5];
        pMessage->vmMOR = arguments[6];

        pMessage->srcConsoleIP = arguments[7];
        pMessage->destConsoleIP = arguments[8];

        int num_of_src_kernel_ip = atoi(arguments[9].c_str());
        int num_of_dest_kernel_ip = atoi(arguments[10].c_str());
        //int num_of_vm_ip = atoi(arguments[11].c_str());
        int num_of_vm_mac = atoi(arguments[11].c_str());

        int i = 0;
        int arg = MIN_VCENTER_EVENT_MESSAGE_FIELD_CNT;


        for (i = 0; i < num_of_src_kernel_ip; i++) {
            vector<string> kernel_ip;
            kernel_ip.push_back(arguments[arg++]); //ip
            kernel_ip.push_back(arguments[arg++]); //nictype

            pMessage->srckernelIPList.push_back(kernel_ip);
        }

        for (i = 0; i < num_of_dest_kernel_ip; i++) {
            vector<string> kernel_ip;
            kernel_ip.push_back(arguments[arg++]); //ip
            kernel_ip.push_back(arguments[arg++]); //nictype
            pMessage->destKernelIPList.push_back(kernel_ip);
        }

        for (i = 0; i < num_of_vm_mac; i++) {
            vector<string> vm_mac_List;
            vm_mac_List.push_back(arguments[arg++]);
            //printf("Exiting here destKernelIPList Fields %s\n",arguments[arg++].c_str());
            int nu_of_ips = atoi(arguments[arg++].c_str());

            int j =0;
            for(j=0;j<nu_of_ips;j++){
                vm_mac_List.push_back(arguments[arg++]);
            }
            pMessage->vmmacaddrList.push_back(vm_mac_List);
        }


        if (true == (isCurrentlyConnected())) {
            status =
                    (WaveUserInterfaceObjectManager::getInstance())->sendOneWayToWaveServer(
                            getWaveServerId(), pMessage);
            if (WAVE_MESSAGE_SUCCESS != status) {
                trace(
                        TRACE_LEVEL_INFO,
                        "DcmClientSynchronousConnection::dcmEventEngineNotify : Sending message failed : "
                                + FrameworkToolKit::localize(status));
            }
        }
        else {
            status = getConnectionStatus();
            trace(
                    TRACE_LEVEL_INFO,
                    "DcmClientSynchronousConnection::dcmEventEngineNotify : EventEngine client not connected: "
                            + FrameworkToolKit::localize(status));
        }

        return (status);
}*/

/*ResourceId DcmClientSynchronousConnection::setIsCliAllowedFromThisLocation (bool isCliAllowedFromThisLocation)
{
    VcsNodeCliAllowedFromThisLocationMessage        message;
    WaveMessageStatus                               status;
    ResourceId                                      completionStatus    = WAVE_MESSAGE_SUCCESS;
    ResourceId                                      clientStatus        = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        message.setIsCliAllowedFromThisLocation (isCliAllowedFromThisLocation);

        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setIsCliAllowedFromThisLocation : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setIsCliAllowedFromThisLocation : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::getClusterStatistics (WaveClientMessageCallbackHandler handler)
{
    VcsFabricDebugClientMessage *pVcsFabricDebugClientMessage = new VcsFabricDebugClientMessage ();
    WaveMessageStatus            status;
    ResourceId                   clientStatus                 = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ())) 
    {
        status = sendToWaveServer (pVcsFabricDebugClientMessage, handler);

        if (WAVE_MESSAGE_SUCCESS != status)
        {    
            trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::getClusterStatistics: Sending message failed : " + FrameworkToolKit::localize (status));

        }    

        clientStatus = status;
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::setTtyName (const string &ttyName)
{
    if (true == ttyName.empty ())
    {
        trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::setTtyName : ttyName is an empty string .. Please try again with a proper argument");
        return (WAVE_MESSAGE_ERROR);
    }
    
    VcsFabricSetTtyNameMessage *pVcsFabricSetTtyNameMessage = new VcsFabricSetTtyNameMessage ();

    pVcsFabricSetTtyNameMessage->setNewTtyName (ttyName);

    WaveMessageStatus            status;
    ResourceId                   clientStatus                 = WAVE_MESSAGE_ERROR;
    ResourceId                   completionStatus             = WAVE_MESSAGE_ERROR;
    
    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (pVcsFabricSetTtyNameMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setTtyName : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = pVcsFabricSetTtyNameMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::setTtyName : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    delete (pVcsFabricSetTtyNameMessage);

    return (clientStatus);
}*/

/*ResourceId DcmClientSynchronousConnection::retrievevCenterMachineStatus( vector<vector<string> > &respList) {
    DiscoverInternalMessage message;
        WaveMessageStatus status;
        ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;
        ResourceId clientStatus = WAVE_MESSAGE_ERROR;


        if (true == (isCurrentlyConnected())) {
            //TODO
            //Send the correct Cmd Code
            message.setCmdcode(DISC_INT_TYPE_GET_STATE_MACHINES);

            //Request send as buffer


            status = sendSynchronouslyToWaveServer(&message);

            if (WAVE_MESSAGE_SUCCESS != status) {
                trace(
                        TRACE_LEVEL_DEBUG,
                        "DcmClientSynchronousConnection::Vcenter : retrievevCenterMachineStatus : Sending message failed : "
                                + FrameworkToolKit::localize(status));

                clientStatus = status;
            }
            else {
                completionStatus = message.getCompletionStatus();

                if (WAVE_MESSAGE_SUCCESS != completionStatus) {
                    clientStatus = completionStatus;

                    trace(
                            TRACE_LEVEL_DEBUG,
                            "DcmClientSynchronousConnection::Vcenter : retrievevCenterMachineStatus : Message Processing failed : "
                                    + FrameworkToolKit::localize(
                                            completionStatus));
                }
                else { //Completed Successfully
                        respList = message.respList;
                    clientStatus = WAVE_MESSAGE_SUCCESS;


                }
            }
        }
        else {
            clientStatus = getConnectionStatus();
        }


    return (clientStatus);
}*/

ResourceId DcmClientSynchronousConnection::vcsNodeLocalSetFcfMac (const char pfcfMac[6])
{
    WaveMessageStatus               status;
    ResourceId                      completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                      clientStatus     = WAVE_MESSAGE_ERROR;
    UI8                             mac[6];
    UI8                             i;

    for (i=0; i<6; i++)
    {
        mac[i] = 0;
        mac[i] |= pfcfMac[i];
    }

    if (true == (isCurrentlyConnected ()))
    {
        MacAddress                     fcfMacAddress(mac);

        VcsNodeSetFcfMacMessage        message(fcfMacAddress);

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::vcsNodeLocalSetFcfMac : Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_ERROR, "DcmClientSynchronousConnection::vcsNodeLocalSetFcfMac : Message Processing failed : " +
                        FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {   
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsNodeLocalGetMappedId (VcsClientLocalParams &vLocalParams)
{
    WaveMessageStatus              status           = WAVE_MESSAGE_ERROR;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        VcsNodeGetLocalMappedIdMessage        message;
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsNodeLocalGetMappedId : Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            vLocalParams.setMappedId(message.getMappedId());

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsNodeLocalGetMappedId : Message Processing failed : " +
                        FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN (UI32 &rBridgeId, const WorldWideName &worldWideName)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsNodeGetRBridgeIdFromSwitchWWNMessage getRBridgeIdMessage;
    getRBridgeIdMessage.setSwitchWWN (worldWideName);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getRBridgeIdMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getRBridgeIdMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                rBridgeId = getRBridgeIdMessage.getRBridgeId ();

                trace (TRACE_LEVEL_DEBUG, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : RBridge-Id (") + rBridgeId + ") found for given switch WWN : " + worldWideName.toString ());

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // Assuming failure of message completion due to RBridge-Id not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_RBRIDGE_ID_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId (WorldWideName &worldWideName, const UI32 rBridgeId)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsNodeGetSwitchWWNFromRBridgeIdMessage getSwitchWWNMessage;
    getSwitchWWNMessage.setRBridgeId(rBridgeId);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getSwitchWWNMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getSwitchWWNMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                WorldWideName switchWWN = getSwitchWWNMessage.getSwitchWWN ();

                if (WorldWideName::isValidWorldWideNameString (switchWWN.toString ()))
                {
                    trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Switch WWN (" + switchWWN.toString () + ") validation check passed for given RBridge-Id : " + rBridgeId);
                    worldWideName = switchWWN;

                    clientStatus = WAVE_MESSAGE_SUCCESS;
                }
                else
                {
                    trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Switch WWN validation check failed for given RBridge-Id : ") + rBridgeId);

                    clientStatus = WAVE_MESSAGE_ERROR;
                }
            }
            else
            {
                // Assuming failure of message completion due to switch WWN not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_SWITCH_WWN_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId (const unsigned int rBridgeId, string &ipAddress)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsNodeGetSwitchIPFromRBridgeIdMessage getSwitchIPMessage;
    getSwitchIPMessage.setRBridgeId(rBridgeId);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getSwitchIPMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getSwitchIPMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                string switchIpAddr = getSwitchIPMessage.getSwitchIP ();

                trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Switch IP (" + switchIpAddr + ") validation check passed for given RBridge-Id : " + rBridgeId);
                ipAddress = switchIpAddr;

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // Assuming failure of message completion due to switch IP not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_SWITCH_IP_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DcmClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId DcmClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN (const WorldWideName &wwn, BitMap &rbridgeIdBitMap)
{
    WaveMessageStatus   status              = WAVE_MESSAGE_ERROR;
    ResourceId          completionStatus    = WAVE_MESSAGE_ERROR;
    ResourceId          clientStatus        = WAVE_MESSAGE_ERROR;

    VcsGetClusterRbridgeIdFromWWNMessage getClusterRbridgeIdFromWWNMessage (wwn);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &getClusterRbridgeIdFromWWNMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            completionStatus = getClusterRbridgeIdFromWWNMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == completionStatus)
            {
                rbridgeIdBitMap = getClusterRbridgeIdFromWWNMessage.getRbridgeIdBitMap ();
                trace (TRACE_LEVEL_DEBUG, string ("ValClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: rbridge-Ids allocated for the WWN (") + wwn.toString () + string (")"));

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                completionStatus = VAL_ALL_RBRIDGE_ID_IN_USE;

                trace (TRACE_LEVEL_WARN, string("ValClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN: Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}
ResourceId DcmClientSynchronousConnection::dcmNsmTriggerVlanUnprovision() {

    const char *func = __FUNCTION__;
    tracePrintf(TRACE_LEVEL_INFO, "%s: triggering vlan unprovision handling",
            func);
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    NsmVlanProvisionLocalMessage *vlanUnprov = new NsmVlanProvisionLocalMessage();
    vlanUnprov->setTriggerVlanUnprovision(true);

    if (true == (isCurrentlyConnected())) {
        status =
                (WaveUserInterfaceObjectManager::getInstance())->sendOneWayToWaveServer(
                        getWaveServerId(), vlanUnprov, 0);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace(TRACE_LEVEL_ERROR,
                    "DcmClientSynchronousConnection::dcmNsmTriggerVlanUnprovision: Sending message failed : "
                    + FrameworkToolKit::localize(status));
        }
    } else
        status = getConnectionStatus();

    return status;

}

ResourceId
DcmClientSynchronousConnection::createAGFcoeInterfaceBulk (
	void *void_msg_p)
{
    AgVFPortMessage       *msg_p = new AgVFPortMessage(
	    VFPORT_MESSAGE_CREATE);
    ResourceId              status = WAVE_MESSAGE_SUCCESS;
	vector<UI32> list_by_port;
	int i;
	fcoe_intf_config_msg_t *msg_fcoe_p = (fcoe_intf_config_msg_t *)void_msg_p;

	if (msg_fcoe_p) {
	    // pull out the list of logical port numbers
	    for (i = 0; i < msg_fcoe_p->num_entries; i++) {
	        list_by_port.push_back(msg_fcoe_p->list[i].fc_port_num);
	    }
	}
	msg_p->setVFPortsByPort(list_by_port);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_DEBUG, "DcmClientSynchronousConnection::createFcoeInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
DcmClientSynchronousConnection::deleteAGFcoeInterfaceBulk (
	void *void_msg_p)
{

    AgVFPortMessage       *msg_p = new AgVFPortMessage(
	    VFPORT_MESSAGE_DELETE);
    ResourceId              status = WAVE_MESSAGE_SUCCESS;
	vector<string> list_by_ifname;
	int i;
	fcoe_intf_config_msg_t *msg_fcoe_p = (fcoe_intf_config_msg_t *)void_msg_p;

	if (msg_fcoe_p) {
	    // pull out the list of logical port numbers
	    for (i = 0; i < msg_fcoe_p->num_entries; i++) {
	        list_by_ifname.push_back(string((char *)msg_fcoe_p->list[i].if_name));
	    }
	}
	msg_p->setVFPortsByIfName(list_by_ifname);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), msg_p, 0);
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "DcmClientSynchronousConnection::createFcoeInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId DcmClientSynchronousConnection::loadZoneBulkConfiguration (const string &zoneConfigurationFilePath)
{
    ZoneBulkConfigurationMessage zoneBulkConfigurationMessage;

    CliConfigurationFile cliFile (zoneConfigurationFilePath);

    vector<string> cfgs;
    vector<string> cfgMembers;
    vector<string> zones;
    vector<string> zoneMembers;
    UI32           i                   = 0;
    UI32           j                   = 0;
    string         cfgCli              = "zoning defined-configuration cfg";
    UI32           numberOfCfgs        = 0;
    string         cfgMemberCli;
    UI32           numberOfCfgMembers  = 0;
    UI32           numberOfZones       = 0;
    UI32           numberOfZoneMembers = 0;
    string         zoneCli             = "zoning defined-configuration zone";
    string         zoneMemberCli;

    cliFile.getChildrenCliNodeNamesForCommandLine (cfgCli, cfgs);

    numberOfCfgs = cfgs.size ();

    for (i = 0; i < numberOfCfgs; i++)
    {
        cfgMemberCli = cfgCli + " " + cfgs[i] + " member-zone";

        cfgMembers.clear ();

        cliFile.getChildrenCliNodeNamesForCommandLine (cfgMemberCli, cfgMembers);

        numberOfCfgMembers = cfgMembers.size ();

        //WaveNs::trace (TRACE_LEVEL_INFO, cfgs[i]);

        for (j = 0; j < numberOfCfgMembers; j++)
        {
            //WaveNs::trace (TRACE_LEVEL_INFO, "    " + cfgMembers[j]);
        }

        zoneBulkConfigurationMessage.addCfg (cfgs[i], cfgMembers);
    }

    cliFile.getChildrenCliNodeNamesForCommandLine (zoneCli, zones);

    numberOfZones = zones.size ();

    for (i = 0; i < numberOfZones; i++)
    {
        zoneMemberCli = zoneCli + " " + zones[i] + " member-entry";

        zoneMembers.clear ();

        cliFile.getChildrenCliNodeNamesForCommandLine (zoneMemberCli, zoneMembers);

        numberOfZoneMembers = zoneMembers.size ();

        //WaveNs::trace (TRACE_LEVEL_INFO, zones[i]);

        for (j = 0; j < numberOfZoneMembers; j++)
        {
            //WaveNs::trace (TRACE_LEVEL_INFO, "    " + zoneMembers[j]);
        }

        zoneBulkConfigurationMessage.addZone (zones[i], zoneMembers);
    }

    ResourceId status = sendSynchronouslyToWaveServer (&zoneBulkConfigurationMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ZoneObjectManager::loadZoningBulkConfiguration : Failed with Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = zoneBulkConfigurationMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "ZoneObjectManager::loadZoningBulkConfiguration : Failed with Status : " + FrameworkToolKit::localize (status));
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "ZoneObjectManager::loadZoningBulkConfiguration : Succeeded to load zoning bulk configuration from " + zoneConfigurationFilePath);
        }
    }

    return (status);

}

}






