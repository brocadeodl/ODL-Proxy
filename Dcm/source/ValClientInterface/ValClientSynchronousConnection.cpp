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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Interface/ValInterfaceLocalObjectManagerGetInterfaceMessage.h"
#include "PhysicalInterface/Local/PhysicalInterfaceMessages.h"
#include "Ssm/Global/SsmGlobalAssocPolicyToInterfaceMessage.h"
#include "Ssm/Global/SsmGlobalAssocRuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalRemMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAddMacAclPolicyMessage.h"
#include "Nsm/Global/NsmGlobalCreateVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanIntfMessage.h"

#include "Nsm/Global/NsmGlobalCreatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalDeletePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"

#include "Nsm/Local/NsmCreatePhyIntfMessage.h"
#include "Nsm/Local/NsmDeletePhyIntfMessage.h"
#include "Nsm/Local/NsmUpdatePhyIntfMessage.h"
#include "Nsm/Local/NsmUpdateBulkPhyIntfMessage.h"

#include "Nsm/Local/NsmEventMessage.h"
#include "hasm/hasm.h"

#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/NsmTypes.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeFcfMacMessages.h"
#include "VcsFabric/Local/VcsNodeMappedIdMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsEnqSwitchMessage.h"
#include "VcsFabric/Local/VcsClusterMergeMessage.h"
#include "VcsFabric/Local/VcsNodeSlotManagementInterfaceMessages.h"
#include "VcsFabric/Local/VcsNodeSetModeChangeParametersMessage.h"

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "Nsm/Local/NsmTypes.h"

#include "Fcoe/Local/FcoeLocalETIntfMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeIntfMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"

#include "ClientInterface/RAS/RASClient.h"
#include "ClientInterface/RAS/RASClientMessage.h"

#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalUpdateVlanBridgeConfigMessage.h"

#include "Nsm/Local/NsmUpdatePoPrimaryLinkMessage.h"

#include "PortCfg/Local/PortCfgPortCfgCreateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgPortStateUpdateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgTrunkUpdateMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "VcsFabric/Local/VcsClientLocalParams.h"
#include "VcsFabric/Local/VcsNodePostBootMessages.h"
#include "Interface/InterfaceInstallDemoObjectMessage.h"
#include "Framework/Core/Test/FrameworkTestabilityManagementInterfaceMessages.h"
#include "Framework/Core/Test/FrameworkTestabilityTypes.h"

#include "LicenseService/Local/LicenseServiceUpdateLicenseFabsysInfoMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include <APPM/Global/APPMGlobalAPPMProfileMessage.h>
#include <APPM/Global/APPMGlobalAPPMProfileBulkMessage.h>
#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Local/APPMVnmLocalCdpNotifyMessage.h"
#include "APPM/Global/APPMGlobalAPPMQosMessage.h"
#include "APPM/Local/APPMLocalAPPMVlanMessage.h"
#include "APPM/Local/APPMLocalAPPMProfileBulkMessage.h"
#include "APPM/Local/APPMLocalAPPMQosMessage.h"
#include "APPM/Global/APPMGlobalProfileDomainMessage.h"

#include "VcsFabric/Local/VcsNodeRBridgeIdMessages.h"
#include "VcsFabric/Local/VcsNodeWWNMessages.h"
#include "VcsFabric/Global/VcsIsIpReachableFromClusterMessage.h"
#include "VcsFabric/Local/VcsNodeIsFabricDistributionEnabledMessage.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "VcsFabric/Local/VcsNodeGvlanMessages.h"

#include "Framework/DistributedDebugInfrastructure/DistributedDebugParameters.h"

#include "Framework/Database/DatabaseObjectManagerShutdownMessage.h"

#include "VcsFabric/Local/VcsFabricDebugClientMessage.h"
#include "Framework/Utils/Integer.h"
#include "VcsFabric/Local/VcsFabricSetTtyNameMessage.h"
#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "discover/Framework/EventEngineManagementInterfaceMessages.h"
#include "discover/User/discoverDcmDef.h"
#include "Lldp/Global/CdpGlobalConfSetMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "APPM/Global/VcenterGetAllPortProfileListMessage.h"
#include "WyserEaGateway/NosSmdReadyForConnectionMessage.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Zone/Global/ZoneBulkConfigurationMessage.h"
#include "SystemManagement/Configuration/CliConfigurationFile.h"

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddFlowMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage.h"

using OpenFlowNs::OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage;
using OpenFlowNs::OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage;
using OpenFlowNs::OpenFlowSwitchObjectManagerAddFlowMessage;
using OpenFlowNs::OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage;
using OpenFlowNs::OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage;

extern "C" {
#include <switch/switch.h>
}

#define NOT_SPECIFIED -2
#define MAC_BULK_REQUEST_SIZE 200

namespace DcmNs
{

ValClientSynchronousConnection *ValClientSynchronousConnection::m_pValClientSynchronousConnectionForCApi = NULL;
ValClientSynchronousConnection *ValClientSynchronousConnection::m_pValClientSynchronousConnectionForCApiWithCcm = NULL;

ValClientSynchronousConnection::ValClientSynchronousConnection (const string &valClientName, const string &waveServerIpAddress, const UI32 &waveServerPort)
    : WaveClientSynchronousConnection (valClientName, waveServerIpAddress, waveServerPort)
{
}

ValClientSynchronousConnection::ValClientSynchronousConnection ()
    : WaveClientSynchronousConnection ()
{
}

ValClientSynchronousConnection::~ValClientSynchronousConnection ()
{
}

void ValClientSynchronousConnection::initializeCApi (ValClientSynchronousConnection *pValClientSynchronousConnection)
{
    prismAssert (NULL != pValClientSynchronousConnection, __FILE__, __LINE__);

    m_pValClientSynchronousConnectionForCApi = pValClientSynchronousConnection;
}

void ValClientSynchronousConnection::unInitializeCApi ()
{
    m_pValClientSynchronousConnectionForCApi = NULL;
}

bool ValClientSynchronousConnection::isCApiInitialized ()
{
    if (NULL == m_pValClientSynchronousConnectionForCApi)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

void ValClientSynchronousConnection::initializeSynchronousConnectionForCcm (ValClientSynchronousConnection *pValClientSynchronousConnection)
{
    prismAssert (NULL != pValClientSynchronousConnection, __FILE__, __LINE__);

    m_pValClientSynchronousConnectionForCApiWithCcm = pValClientSynchronousConnection;
}

void ValClientSynchronousConnection::unInitializeSynchronousConnectionForCcm ()
{
    m_pValClientSynchronousConnectionForCApiWithCcm = NULL;
}

bool ValClientSynchronousConnection::isSynchronousConnectionForCcmInitialized ()
{
    return ((NULL == m_pValClientSynchronousConnectionForCApiWithCcm) ? false : true);
}

ValClientSynchronousConnection *ValClientSynchronousConnection::getValClientSynchronousConnectionForDualDaemons ()
{
    return ((NULL != m_pValClientSynchronousConnectionForCApiWithCcm) ? (m_pValClientSynchronousConnectionForCApiWithCcm) : (m_pValClientSynchronousConnectionForCApi));
}

ValClientSynchronousConnection *ValClientSynchronousConnection::getValClientSynchronousConnectionForCApi ()
{
    return (m_pValClientSynchronousConnectionForCApi);
}
ResourceId ValClientSynchronousConnection::test_api_for_dual_connection ()
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::test_api_for_dual_connection : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::test_api_for_dual_connection : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                trace (TRACE_LEVEL_INFO, string("ValClientSynchronousConnection::test_api_for_dual_connection : processing directory : ") + message.getString ());
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::valQueryForInterface (const string &interfaceName, ValClientPhysicalInterface &valClientPhysicalInterface)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::valQueryForInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::valQueryForInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::dcmNsmEventNotify (const UI32 &eventid, const UI32 &parameter)
{

	ResourceId             status;
	NsmEventMessage	*pMessage = new NsmEventMessage (eventid, parameter);
	if (true == (isCurrentlyConnected ()))
	{
		status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pMessage, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::dcmNsmEventNotify : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = getConnectionStatus ();
		trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::dcmNsmEventNotify : nsm client not connected: " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId ValClientSynchronousConnection::vnmAppmCdpNotify (const UI32 cmdCode, const string ifName, const string hostMor, const string portId)
{
	ResourceId status;
    APPMVnmLocalCdpNotifyMessage *pMessage = new APPMVnmLocalCdpNotifyMessage (cmdCode, ifName, hostMor, portId);

    if (true == (isCurrentlyConnected ())) 
    {
        status = sendOneWayToWaveServer (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status) 
        {
            trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::vnmAppmCdpNotify: Sending message failed : " + FrameworkToolKit::localize (status));
        } 
        else 
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::vnmAppmCdpNotify: Send message successfully ");
        }
    } 
    else 
    {
        status = getConnectionStatus ();

        trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::vnmAppmCdpNotify: connection to Wave Server failed: " + FrameworkToolKit::localize (status));
    }

    return (status);
}


ResourceId ValClientSynchronousConnection::createPhysicalInterface (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createPhysicalInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createPhysicalInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::vcsNodeLocalSetFcfMac (const char pfcfMac[6])
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
            trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::vcsNodeLocalSetFcfMac : Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::vcsNodeLocalSetFcfMac : Message Processing failed : " +
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

ResourceId ValClientSynchronousConnection::vcsNodeLocalSetFcfMacServiceIndependent(const char * pfcfMac )
{
    WaveMessageStatus               status;
    ResourceId                      clientStatus     = WAVE_MESSAGE_ERROR;

    
    if (true == (isCurrentlyConnected ()))
    {
        string macString(pfcfMac);
        MacAddress fcfMacAddress(macString);

        VcsNodeSetFcfMacMessageServiceIndependent  message (fcfMacAddress);

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::vcsNodeLocalSetFcfMac : Sending message failed : " +
                    FrameworkToolKit::localize (status));

            clientStatus = status;
        }   
        else
        {
            trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::vcsNodeLocalSetFcfMac : Sending message passed : " );
            clientStatus = WAVE_MESSAGE_SUCCESS;
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::vcsGetThisNodeClusterType (ResourceId &vcsClusterType)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetThisNodeClusterType: Sending message failed : " +
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

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetClusterMode: Message Processing failed : " +
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

ResourceId ValClientSynchronousConnection::vcsGetVcsMode (VcsClientVcsModeParams &vcsModeParams)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetVcsId: Sending message failed : " +
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

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetVcsId: Message Processing failed : " +
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

ResourceId ValClientSynchronousConnection::vcsGetRBridgeId (UI32 &rbridgeId)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetRBridgeId: Sending message failed : " +
                        FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();


            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetRBridgeId: Message Processing failed : " +
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


ResourceId ValClientSynchronousConnection::isConfigFileAllowed (SI32 &isAllowed)
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
            printf ("ValClientSynchronousConnection::isConfigFileAllowed : Sending message failed : %x\n", status);

            clientStatus = status;
            isAllowed = 0;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("ValClientSynchronousConnection::isConfigFileAllowed : Message Processing failed : %x\n", completionStatus);
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

ResourceId ValClientSynchronousConnection::vcsGetVcsId (VcsClientVcsIdParams &vcsIdParams)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetVcsId: Sending message failed : " +
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

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsGetVcsId: Message Processing failed : " +
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

ResourceId ValClientSynchronousConnection::vcsNodeLocalGetMappedId (VcsClientLocalParams &vLocalParams)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsNodeLocalGetMappedId : Sending message failed : " +
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

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsNodeLocalGetMappedId : Message Processing failed : " +
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
ResourceId ValClientSynchronousConnection::macAclPolicy(const string &policyName, const UI32 &policyType, const UI32 &opcode)
{
	//SsmAssocPolicyToPhyInterfaceMessage *message = new SsmAssocPolicyToPhyInterfaceMessage(ifindex, name, opcode);
	SsmGlobalAddMacAclPolicyMessage message (policyName, policyType);
	SsmGlobalRemMacAclPolicyMessage delMessage (policyName, policyType);
	WaveMessageStatus              status = WAVE_MESSAGE_ERROR;
	ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
	ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

	if (opcode == 1) {
		trace (TRACE_LEVEL_INFO, "Poldfjdljfdk " + message.getPolicyName());
	} else if (opcode == 2) {
		trace (TRACE_LEVEL_INFO, "DelPolicy " + delMessage.getPolicyName());
	}

	if (true == (isCurrentlyConnected ())) {
		if (opcode == 1) {
			status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);
		} else if (opcode == 2) {
			status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &delMessage);
		}


        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToLogInterface: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            if (opcode == 1) {
            		completionStatus = message.getCompletionStatus ();
			} else {
				completionStatus = delMessage.getCompletionStatus ();
			}

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToLogInterface: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::AssocRuleToPolicy(const string &policyName,const UI32 &policyType,const UI32 &ruleid,const UI32 &action,const string &srcMac, const AddrType &srcType, const bool &srcMask,const string &dstMac, const AddrType &dstType, const bool &dstMask,const string &appType, const bool &isCount,const UI32 &opcode)
{
	SsmGlobalAssocRuleToPolicyMessage message (policyName, policyType, ruleid, action, srcMac, srcMask, dstMac, dstMask, appType,isCount, opcode);
	WaveMessageStatus              status;
	ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
	ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

	message.setSrcType(srcType);
	message.setDstType(dstType);

		//trace (TRACE_LEVEL_INFO, "Poldfjdljfdk " + message.getPolicyName());

	if (true == (isCurrentlyConnected ())) {
		status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToLogInterface: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToLogInterface: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::AssocPolicyToLogInterface(const string &ifName, const string &name, const UI32 &opcode)
{
	//SsmAssocPolicyToPhyInterfaceMessage *message = new SsmAssocPolicyToPhyInterfaceMessage(ifindex, name, opcode);
	SsmGlobalAssocPolicyToInterfaceMessage message (ifName, name, opcode);
	WaveMessageStatus              status;
	ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
	ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

	trace (TRACE_LEVEL_INFO, "Poldfjdljfdk " + message.getPolicyName());

	if (true == (isCurrentlyConnected ())) {
		status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToLogInterface: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToLogInterface: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::AssocPolicyToPhyInterface(const string &ifname, const string &name, const UI32 &opcode)
{
	//SsmAssocPolicyToPhyInterfaceMessage *message = new SsmAssocPolicyToPhyInterfaceMessage(ifindex, name, opcode);
	SsmGlobalAssocPolicyToInterfaceMessage message (ifname, name, opcode);
	WaveMessageStatus              status;
	ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
	ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

	trace (TRACE_LEVEL_INFO, "Poldfjdljfdk " + message.getPolicyName());

	if (true == (isCurrentlyConnected ())) {
		status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToPhyInterface: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::AssocPolicyToPhyInterface: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::deletePoInterface (const string &name)
{
    NsmGlobalDeletePoIntfMessage   message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);


    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::deletePoInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::deletePoInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::createPoInterface (const string &name)
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
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPoInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPoInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::delPoProtocalStatus (const string &name, const bool &flag)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setFlag(flag);
    message.setCmdCode(NSMUPDATE_PO_DELSTATUS);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::updatePoInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::updatePoInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::delVlanProtocalStatus (const string &name, const bool &flag)
{
    NsmGlobalUpdateVlanIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	UI32 vlanId = atoi(name.c_str());

    message.setVlanId(vlanId);
    message.setFlag(flag);
    message.setCmdCode(NSMUPDATE_VLAN_DELSTATUS);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO,"ValClientSynchronousConnection::delVlanProtocalStatus: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection:delVlanProtocalStatus: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setPoProtocalStatus (const string &name, const bool &flag)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setFlag(flag);
    message.setCmdCode(NSMUPDATE_PO_STATUS);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoProtocalStatus : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoProtocalStatus : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::setVlanProtocalStatus (const string &name, const bool &flag)
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

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setVlanProtocalStatus: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setVlanProtocalStatus: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::delPoDesc (const string &name, const string &description)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setDesc(description);
    message.setCmdCode(NSMUPDATE_PO_DELDESC);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::delPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::delPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::delVlanDesc (const string &name, const string &description)
{
    NsmGlobalUpdateVlanIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	UI32 vlanId = atoi(name.c_str());

    message.setVlanId(vlanId);
    message.setDesc(description);
    message.setCmdCode(NSMUPDATE_VLAN_DELDESC);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::delVlanDesc: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::delVlanDesc: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::delPvlanType (const string &name)
{
    NsmGlobalUpdateVlanIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	UI32 vlanId = atoi(name.c_str());

    message.setVlanId(vlanId);
//    message.setDesc(description);
    message.setCmdCode(NSMUPDATE_VLAN_DELPVLAN_TYPE);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::delPvlanType : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::delPvlanType : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::setPoSwitchPort (const string &name)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setSwModeFlag(true);
    message.setCmdCode(NSMUPDATE_PO_SW);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::setPoMode (const string &name, const int &mode)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setSwMode((swModeEnum)mode);
    message.setCmdCode(NSMUPDATE_PO_MODE);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setPoPrimaryLinkDomainId (const string &name, const unsigned int &poPrimaryLinkDomainId,const unsigned int &poPartnerSystemPriority,
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

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoPrimaryLinkDomainId : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoPrimaryLinkDomainId : completion status: " + FrameworkToolKit::localize (completionStatus));

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                if (WRC_VLAG_REQUEST_FAILED == completionStatus)
                {
                    clientStatus = WAVE_MESSAGE_SUCCESS;
                    replyCode = message.getReplyCode();
                } else {
                    clientStatus = completionStatus;
                    trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoPrimaryLinkDomainId : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::delPoSwitchPort (const string &name)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setSwModeFlag(false);
    message.setCmdCode(NSMUPDATE_PO_DELSW);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::delPoMode (const string &name)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setCmdCode(NSMUPDATE_PO_DELMODE);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::setPoDesc (const string &name, const string &description)
{
    NsmGlobalUpdatePoIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setPoId(name);
    message.setDesc(description);
    message.setCmdCode(NSMUPDATE_VLAN_DESC);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setVlanDesc (const string &name, const string &description)
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

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setVlanDesc: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setVlanDesc: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::deleteVlanInterface (const string &name)
{

    NsmGlobalDeleteVlanIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	UI32 vlanId = atoi(name.c_str());

    message.setVlanId(vlanId);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::deleteVlanInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::deleteVlanInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ValClientSynchronousConnection::createFcoeETInterface (const string &name, const UI64 &ifIndex, const UI32 &fcUportNum)
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
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createFcoeETInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
ValClientSynchronousConnection::createFcoeInterface (const string &name, const UI64 &ifIndex,
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
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createFcoeInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
ValClientSynchronousConnection::createFcoeInterfaceBulk (const void *data_p,
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
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createFcoeInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
ValClientSynchronousConnection::deleteFcoeInterfaceBulk (const void *data_p,
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
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createFcoeInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
ValClientSynchronousConnection::createFcoeETInterfaceBulk (const void *data_p,
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
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createFcoeETInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
ValClientSynchronousConnection::updateFcoeETInterfaceBulk (const void *data_p,
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
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::updateFcoeETInterfaceBulk : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    } else {
        status = getConnectionStatus ();
    }
    return (status);
}

ResourceId
ValClientSynchronousConnection::createPortCfgInterface (const string &name, const UI32 &fc_uport_num)
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
    //tracePrintf (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPortCfgInterface: Create PortCfg Interface ID=%s SwPort=%d",
    //             name.c_str(), fc_uport_num);

    status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &portCfgCreateMsg);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createPhysicalInterface : portCfgCreateMsg failed : " + FrameworkToolKit::localize (status));

        clientStatus = status;
    }
    else
    {
        completionStatus = portCfgCreateMsg.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            clientStatus = completionStatus;

            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createPhysicalInterface : portCfgCreateMsg failed : " + FrameworkToolKit::localize (completionStatus));
        }
        else
        {
            clientStatus = WAVE_MESSAGE_SUCCESS;
        }
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::createPhyInterface (const string &name, const UI64 &ifIndex, const UI32 &mtu, const string &description, const UI32 isl_capable)
{
    NsmCreatePhyIntfMessage*       pMessage=new NsmCreatePhyIntfMessage(name,ifIndex,mtu,description);
    ResourceId                     status = WAVE_MESSAGE_SUCCESS;

	pMessage->setIslCapability(isl_capable);
	if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pMessage, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Sending message failed : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = getConnectionStatus ();
        trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : nsm client not connected: " + FrameworkToolKit::localize (status));
    }

    return (status);

}


ResourceId ValClientSynchronousConnection::deletePhyInterface (const string &name)
{

    NsmDeletePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::deletePhyInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::deletePhyInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::delPhyProtocalStatus (const string &name, const bool &flag)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setFlag(flag);
    message.setCmdCode(NSMUPDATE_PHY_DELSTATUS);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setPhySwitchPort (const string &name)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setSwModeFlag(true);
    message.setCmdCode(NSMUPDATE_PHY_SW);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::setPhyMode (const string &name, const int &mode)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setSwMode((swModeEnum) mode);
    message.setCmdCode(NSMUPDATE_PHY_MODE);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::delPhySwitchPort (const string &name)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setSwModeFlag(false);
    message.setCmdCode(NSMUPDATE_PHY_DELSW);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::delPhyMode (const string &name)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setCmdCode(NSMUPDATE_PHY_DELMODE);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setPoDesc : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::dcmNsmNotifyPortroleConfig (const string &name, const unsigned int portrole, unsigned int exception, unsigned int msg_id)
{
    NsmUpdatePhyIntfMessage *pPhyMsg = new NsmUpdatePhyIntfMessage();
    ResourceId             status;

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
            trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::dcmNsmNotifyPortroleConfig: Sending message failed : " + FrameworkToolKit::localize (status));
    }
    else
        status = getConnectionStatus ();

    return (status);
}

ResourceId ValClientSynchronousConnection::dcmNsmModifyInterfaceConfig (void *intfConfig,unsigned int msg_id)
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
                trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::dcmNsmModifyInterfaceConfig: Sending message failed : " + FrameworkToolKit::localize (status));
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

ResourceId ValClientSynchronousConnection::dcmNsmNotifyPhyStatusConfig (const string &name, const unsigned int flag, unsigned int msg_id)
{
    NsmUpdatePhyIntfMessage *pPhyMsg = new NsmUpdatePhyIntfMessage();
    ResourceId             status = WAVE_MESSAGE_SUCCESS;

    pPhyMsg->setIfName(name);
    pPhyMsg->setFlag(flag);
    pPhyMsg->setMsgId(msg_id);
    pPhyMsg->setCmdCode(NSMUPDATE_PHY_EXCEPTION_STATUS);

    if (true == (isCurrentlyConnected ())) {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pPhyMsg, 0);

        if (WAVE_MESSAGE_SUCCESS != status)
            trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::dcmNsmNotifyPhyStatusConfig: Sending message failed : " + FrameworkToolKit::localize (status));
    }
    else
        status = getConnectionStatus ();

    return (status);
}

ResourceId ValClientSynchronousConnection::setPhyProtocalStatus (const string &name, const bool &flag)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setFlag(flag);
    message.setCmdCode(NSMUPDATE_PHY_STATUS);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::delPhyDesc (const string &name, const string &description)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setDesc(description);
    message.setCmdCode(NSMUPDATE_PHY_DELDESC);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setPhyDesc (const string &name, const string &description)
{
    NsmUpdatePhyIntfMessage message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    message.setIfName(name);
    message.setDesc(description);
    message.setCmdCode(NSMUPDATE_PHY_DESC);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createPhyInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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



ResourceId ValClientSynchronousConnection::createVlanInterface (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description)
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

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createVlanInterface : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::createVlanInterface : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::vcsRASLogging (vcslog_t *vcslog, const string &vcslogString)
{
    RASClientMessage   *pMessage = new RASClientMessage();
    WaveMessageStatus  status = WAVE_MESSAGE_SUCCESS;
    VcsClientLocalParams            vLocalParams;

    pMessage->getCStructure (vcslog);
    pMessage->m_vcslogString = vcslogString;
    // pMessage->m_data = vcslogString;
    trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsRASLogging : Sending message=" + pMessage->m_swname);


    if (true == (isCurrentlyConnected ()))
    {
	if (WAVE_MESSAGE_SUCCESS == vcsNodeLocalGetMappedId (vLocalParams)) {
            pMessage->setMappedId(vLocalParams.getMappedId());

	} else {
	    trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::vcsRASLogging : getMappedId failed : "
	       + FrameworkToolKit::localize (status));
	    return (status);
	}

	status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pMessage);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::vcsRASLogging : Sending message failed : " + FrameworkToolKit::localize (status));

        }
    }
    return (status);
}


ResourceId ValClientSynchronousConnection::setBridgeXstp (const vector<string> & vals)
{

    MstpGlobalUpdateBridgeConfigMessage  message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	int i = 0;
	bool isNo = false;

	static int mode = MSTP_DCM_BRIDGE_NONE;


	trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setBridgeXstp : Enter");
	string cmd = vals[i];
	if (cmd.find("no") != string::npos) {
		isNo = true;
		cmd = vals[++i];
	}
	++i;
	if (cmd.find("pri") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NO_PRIORITY);
		} else  {
			message.setPriority(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_PRIORITY);
		}

	} else if (cmd.find("pro") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NONE);
		} else {
			if (vals[i].find("rstp") != string::npos) {
				mode = MSTP_DCM_BRIDGE_RSTP;
			} else if (vals[i].find("mstp") != string::npos) {
				mode = MSTP_DCM_BRIDGE_MSTP;
			} else if (vals[i].find("pvst") != string::npos) {
				mode = MSTP_DCM_BRIDGE_PVSTP;
			} else if (vals[i].find("rpvst") != string::npos) {
				mode = MSTP_DCM_BRIDGE_RPVSTP;
			} else if (vals[i].find("stp") != string::npos) {
				mode = MSTP_DCM_BRIDGE_STP;
			} else {
				status = WAVE_MESSAGE_ERROR;
				trace (TRACE_LEVEL_INFO, "error mode "+vals[i]);
				return status;
			}
			message.setCmdCode(MSTP_MSG_BR_CREATE);
		}

	} else if (cmd.find("des") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NO_DESCRITION);
		} else {
			message.setDesc(vals[i]);
			message.setCmdCode(MSTP_MSG_BR_DESCRITION);
		}

	} else if (cmd.find("for") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NO_FWD_DELAY);
		} else {
			message.setForwardDelay(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_FWD_DELAY);
		}

	} else if (cmd.find("hel") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NO_HELLO_TIME);
		} else {
			message.setHelloTime(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_HELLO_TIME);
		}
	} else if (cmd.find("max") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NO_MAX_AGE);
		} else {
			message.setMaxAge(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_MAX_AGE);
		}

	} else if (cmd.find("err") != string::npos) {
		if (vals[i].find("en") != string::npos) {
			if (isNo) {
				message.setCmdCode(MSTP_MSG_BR_NO_ERR_TIMEOUT);
			} else {
				message.setCmdCode(MSTP_MSG_BR_ERR_TIMEOUT);
			}
		} else {
			if (isNo) {
				message.setCmdCode(MSTP_MSG_BR_NO_ERR_INTERVAL);
			} else {
				message.setErrDisableInterval(atoi(vals[i].data()));
				message.setCmdCode(MSTP_MSG_BR_ERR_INTERVAL);
			}
		}

	} else if (cmd.find("port") != string::npos) {
		if (vals[i].find("std") != string::npos) {
			if (isNo) {
				message.setCmdCode(MSTP_MSG_BR_NO_PATHCOST_STDANDARD);
			} else {
				message.setCmdCode(MSTP_MSG_BR_PATHCOST_STDANDARD);
			}
		} else {
			if (isNo) {
				message.setCmdCode(MSTP_MSG_BR_PATHCOST_STDANDARD);
			} else {
				message.setCmdCode(MSTP_MSG_BR_NO_PATHCOST_STDANDARD);
			}
		}

	} else if (cmd.find("bpdu") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NO_BPDU_MAC);
		} else {
			message.setCmdCode(MSTP_MSG_BR_BPDU_MAC);
		}

	} else if (cmd.find("tran") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_NO_TRANS_HOLDCOUNT);
		} else {
			message.setTxHoldCount(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_TRANS_HOLDCOUNT);
		}

	} else if (cmd.find("shut") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_PROTO_ENABLE);
		} else {
			message.setCmdCode(MSTP_MSG_BR_NO_PROTO_ENABLE);
		}
	} else {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setBridgeXstp : Error command");
			return (WAVE_MESSAGE_ERROR);
	}
	message.setMode(mode);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setBridgeXstp : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setBridgeXstp : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setVlanBridgeXstp (const vector<string> & vals)
{

    MstpGlobalUpdateVlanBridgeConfigMessage  message;
    WaveMessageStatus              status;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;
	int i = 0;
	bool isNo = false;
	int mode;


	trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setBridgeXstp : Enter");
	string cmd = vals[i];
	if (cmd.find("pv") != string::npos) {
		mode = MSTP_DCM_BRIDGE_PVSTP;
	} else if (cmd.find("pv") != string::npos) {
		mode = MSTP_DCM_BRIDGE_RPVSTP;
	}  else {
		return WAVE_MESSAGE_ERROR;
	}

	cmd = vals[++i];
	if (cmd.find("no") != string::npos) {
		isNo = true;
		cmd = vals[++i];
	}
	++i;
	if (cmd.find("pri") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_VLAN_NO_PRIORITY);
		} else  {
			message.setPriority(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_VLAN_PRIORITY);
		}
	} else if (cmd.find("for") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_VLAN_NO_FWD_DELAY);
		} else {
			message.setForwardDelay(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_VLAN_FWD_DELAY);
		}

	} else if (cmd.find("hel") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_VLAN_NO_HELLO_TIME);
		} else {
			message.setHelloTime(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_VLAN_HELLO_TIME);
		}
	} else if (cmd.find("max") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_VLAN_NO_MAX_AGE);
		} else {
			message.setMaxAge(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_VLAN_MAX_AGE);
		}
	} else if (cmd.find("agi") != string::npos) {
		if (isNo) {
			message.setCmdCode(MSTP_MSG_BR_VLAN_NO_AGING_TIME);
		} else {
			message.setAgingTime(atoi(vals[i].data()));
			message.setCmdCode(MSTP_MSG_BR_VLAN_AGING_TIME);
		}
	}
	message.setMode(mode);

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message, 0);
        if (WAVE_MESSAGE_SUCCESS != status)
        {

            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setVlanBridgeXstp : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO,"ValClientSynchronousConnection::setVlanBridgeXstp : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::copyToStartupConfig (const string &path)
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
            printf ("ValClientSynchronousConnection::copyToStartupConfig : Sending message failed : %x\n", status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("ValClientSynchronousConnection::copyToStartupConfig : Message Processing failed : %x\n", completionStatus);
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

ResourceId ValClientSynchronousConnection::copyRunningToStartup(void)
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
            printf ("ValClientSynchronousConnection::copyRunningToStartup : Sending message failed : %x\n", status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("ValClientSynchronousConnection::copyRunningToStartup : Message Processing failed : %x\n", completionStatus);
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

ResourceId ValClientSynchronousConnection::copyDefaultToStartup(void)
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
            printf ("ValClientSynchronousConnection::copyDefaultToStartup : Sending message failed : %x\n", status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                printf ("ValClientSynchronousConnection::copyDefaultToStartup : Message Processing failed : %x\n", completionStatus);
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


ResourceId ValClientSynchronousConnection::fosCliPortPersistent (const UI32 slot, const UI32 port, const UI32 persiststate )
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
			trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::fosCliPortPersistent : fosCliPortPersistent failed : " + FrameworkToolKit::localize (status));
			clientStatus = status;
		}
		else
		{
			completionStatus = portCfgStateUpdateMsg.getCompletionStatus ();

			if (WAVE_MESSAGE_SUCCESS != completionStatus)
			{
				clientStatus = completionStatus;

				trace (TRACE_LEVEL_INFO, string ("Command did not succeed. Make sure to execute only from VCS mode enabled"));
				trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::fosCliPortPersistent : portCfgStateUpdateMsg failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::fosCliPortTrunk (const UI32 slot, const UI32 port, const UI32 trunkcapable)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::fosCliPortTrunk : fosCliPortTrunk failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = portCfgTrunkUpdateMsg.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, string ("Command did not succeed. Make sure to execute only from VCS mode enabled"));
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::fosCliPortTrunk : portCfgTrunkUpdateMsg failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::isDcmdReadyForConnection (bool &isReady)
{
    VcsNodeGetIsReadyMessage                            message;
    WaveMessageStatus                                   status;
    ResourceId                                          completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                          clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::isDcmdReadyForConnection : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::isDcmdReadyForConnection : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                isReady = message.getIsReady ();
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::notifyWyserEaPhase2Complete ()
{
    VcsNodeTriggerPostBootMessage                       message;
    WaveMessageStatus                                   status;
    ResourceId                                          completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                          clientStatus     = WAVE_MESSAGE_ERROR;
    bool                                                checkP2Completed = true;

    if (true == (isCurrentlyConnected ()))
    {
        /*
         * WyserEa process can restart . Hence we need to diffentiate
         * between initial start and restart cases to avoid post-boot
         * process from being executed on restart scenarion.
         */
        message.setCheckNotifyPhase2CompletedBool(checkP2Completed);

        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::notifyWyserEaPhase2Complete : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::notifyWyserEaPhase2Complete : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::isDcmdReadyForHasmLaunch (bool &isDcmdReadyForHasmLaunchBool)
{
    VcsNodeGetIsDcmdReadyForHasmLaunchMessage           message;
    WaveMessageStatus                                   status;
    ResourceId                                          completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                          clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::isDcmdReadyForConnection : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::isDcmdReadyForConnection : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                isDcmdReadyForHasmLaunchBool = message.getIsDcmdReadyForHasmLaunch ();
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::setHasmLaunched (bool &hasmLaunchState)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::isDcmdReadyForConnection : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::isDcmdReadyForConnection : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

  ResourceId ValClientSynchronousConnection::HasmSetMvCFeature (bool hasmMvCState)
  {

      char feature[] = "vcenter";
      WaveNs::trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::HasmSetMvCFeature enable/disable");
      if(hasmMvCState)
          has_set_feature(feature, 1);
      else
          has_set_feature(feature, 0);
      return (WAVE_MESSAGE_SUCCESS);

  }

ResourceId ValClientSynchronousConnection::queryLinecardType (const UI32 &slotNumber, UI32 &slotType)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::verifyLinecardType : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::verifyLinecardType : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
}

ResourceId ValClientSynchronousConnection::notifySystemShutdown (const string &databaseDirectory)
{
    DatabaseObjectManagerShutdownMessage       *pMessage = new DatabaseObjectManagerShutdownMessage (databaseDirectory);

    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        UI32 numberOfIterations = 0;

        do {
            status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), pMessage);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_WARN, "ValClientSynchronousConnection::notifySystemShutdown : Message Sending failed : " + FrameworkToolKit::localize (status));
                if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == status)
                {
                    ++numberOfIterations;
                    trace (TRACE_LEVEL_WARN, string ("ValClientSynchronousConnection::notifySystemShutdown : Service NOT enabled, Retry count : ") + numberOfIterations);
                    prismSleep (1);
                }
                clientStatus = status;
            }
            else
            {
                completionStatus = pMessage->getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    trace (TRACE_LEVEL_WARN, "ValClientSynchronousConnection::notifySystemShutdown : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));

                    if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == completionStatus)
                    {
                        ++numberOfIterations;
                        trace (TRACE_LEVEL_WARN, string ("ValClientSynchronousConnection::notifySystemShutdown : Service NOT enabled, Retry count : ") + numberOfIterations);
                        prismSleep (1);
                    }
                    clientStatus = completionStatus;
                }
                else
                {
                    clientStatus = WAVE_MESSAGE_SUCCESS;
                }
            }
        } while ((WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == clientStatus) && (20 > numberOfIterations));
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    delete pMessage;
    return (clientStatus);
}



ResourceId ValClientSynchronousConnection::installDemoObject ()
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::Install Demo Object : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::Install Demo Object : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
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

ResourceId ValClientSynchronousConnection::dumpObjectLeaks (vector<string> arguments)
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
                trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::dumpObjectLeaks : Sending message failed : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                completionStatus = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    clientStatus = completionStatus;

                    trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::dumpObjectLeaks : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
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

}

ResourceId ValClientSynchronousConnection::messageLeaksStats (vector<string> arguments)
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
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::Memory Leaks Stats : Sending message failed : " + FrameworkToolKit::localize (status));
                clientStatus = status;
                return (clientStatus);
            }
            else
            {
                completionStatus = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    clientStatus = completionStatus;
                    trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::Memory Leak Stats : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
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
}

ResourceId ValClientSynchronousConnection::podUpdateLicenseData (const char *setName, const int len, const char *configString)
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
	trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::podUpdateLicenseData  : invoked");

	if (true == (isCurrentlyConnected ()))
	{
		// notification of being connected
		trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::podUpdateLicenseData : connected");

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
				trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection:: podUpdateLicenseData : podUpdateLicenseData   failed : " + FrameworkToolKit::localize (status));
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
					trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::podUpdateLicenseData  : licenseUpdateMsg failed : " + FrameworkToolKit::localize (completionStatus));
					delete licenseUpdateMsg;
					return (completionStatus);
				}
				else
				{
					trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::podUpdateLicenseData - Msg Update Successful");
					clientStatus = WAVE_MESSAGE_SUCCESS;
				}
			}
			// free up the license update message to clean up the context
			delete licenseUpdateMsg;
		}
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::podUpdateLicenseData - Connection failure");
		clientStatus = getConnectionStatus ();
	}
	return (clientStatus);
}


// this method is a brute force parser for the config string passed in when link-up events occur.
// it accepts a pointer to a string containing a comma-delimited list of both individual ports
// as well as port ranges.  The individual ports and all of the ports within a given range are
// converted to integers and returned in the portList array parameter
// the method returns the total number of entries extracted
int ValClientSynchronousConnection::extractPortNumbers (const char *configString, int portList[])
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

ResourceId  ValClientSynchronousConnection::configureFileService(const string &sArgument)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::configureFileService : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = fsmessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::configureFileService : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
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

ResourceId ValClientSynchronousConnection::changeBtTrackerState (vector<string> arguments)
{
    ResourceId  status           = WAVE_MESSAGE_ERROR;
    ResourceId  completionStatus = WAVE_MESSAGE_ERROR;

    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::changeBtTrackerState : <cmdname> <object/message> <on/offAndClear>");
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
        trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::changeBtTrackerState : incorrect leak type specified");
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
        trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::changeBtTrackerState : incorrect bt tracker state type specified");
        return status;
    }

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), pMessage);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::changeBtTrackerState : Sending message failed : " + FrameworkToolKit::localize (status));
            return status;
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::changeBtTrackerState : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::changeBtTrackerState : operation successful");
            }
            return completionStatus;
        }
    }
    else
    {
        status = getConnectionStatus ();
        trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::changeBtTrackerState : Connection error : "+ FrameworkToolKit::localize (status));
        return status;
    }


}

ResourceId ValClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId (WorldWideName &worldWideName, const UI32 rBridgeId)
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
                    trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Switch WWN (" + switchWWN.toString () + ") validation check passed for given RBridge-Id : " + rBridgeId);
                    worldWideName = switchWWN;

                    clientStatus = WAVE_MESSAGE_SUCCESS;
                }
                else
                {
                    trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Switch WWN validation check failed for given RBridge-Id : ") + rBridgeId);

                    clientStatus = WAVE_MESSAGE_ERROR;
                }
            }
            else
            {
                // Assuming failure of message completion due to switch WWN not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_SWITCH_WWN_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("ValClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsNodeGetWWNFromRBridgeId : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN (UI32 &rBridgeId, const WorldWideName &worldWideName)
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

                trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : RBridge-Id (") + rBridgeId + ") found for given switch WWN : " + worldWideName.toString ());

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // Assuming failure of message completion due to RBridge-Id not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_RBRIDGE_ID_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("ValClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsNodeGetRBridgeIdFromWWN : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId  ValClientSynchronousConnection::isNodeInCluster (const unsigned int rbrigdeId, bool &inCluster)
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
            trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isNodeInCluster : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isNodeInCluster : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
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
        trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isNodeInCluster : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    inCluster = message.getIsNodeInCluster();
    return (clientStatus);
}

ResourceId  ValClientSynchronousConnection::isLocalNodeInCluster (bool &isInCluster)
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
            trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isLocalNodeInCluster : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isLocalNodeInCluster : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
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
        trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isNodeInCluster : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    isInCluster = message.getIsLocalNodeInCluster ();
    return (clientStatus);
}

ResourceId  ValClientSynchronousConnection::getConfigurationNumber (UI16 configurationNumber[16])
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
                trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::getConfigurationNumber : Sending message failed : " + FrameworkToolKit::localize (status));
            }

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::getConfigurationNumber : completionStatus : " + FrameworkToolKit::localize (completionStatus));

                if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == completionStatus)
                {
                    for (UI32 i = 0; i < 16; i++)
                    {
                        configurationNumber [i] = 0;
                    }

                    completionStatus = WAVE_MESSAGE_SUCCESS;
                }

                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::getConfigurationNumber : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
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

ResourceId  ValClientSynchronousConnection::setNewShowVcs (bool newShowVcs)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setNewShowVcs : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setNewShowVcs : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
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

ResourceId  ValClientSynchronousConnection::getConfigurationTime (char *p_configurationTime, const int configurationTimeStringSize)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::getConfigurationTime : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::getConfigurationTime : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                configurationTime = message.getConfigurationTime ();
                if ((UI32) configurationTimeStringSize < configurationTime.size ())
                {
                    clientStatus = WAVE_MESSAGE_ERROR;
                    trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::getConfigurationTime : passed string length is not enough to store DateTime");
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


ResourceId ValClientSynchronousConnection::vcsNodeGetIPFromRBridgeId (const unsigned int rBridgeId, string &ipAddress)
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

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Switch IP (" + switchIpAddr + ") validation check passed for given RBridge-Id : " + rBridgeId);
                ipAddress = switchIpAddr;

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // Assuming failure of message completion due to switch IP not found in VcsNodeLocalMO's
                completionStatus = VAL_CLIENT_SWITCH_IP_NOT_FOUND;

                trace (TRACE_LEVEL_WARN, string("ValClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));

                clientStatus = completionStatus;
            }

        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Message processing failed : ") + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::vcsNodeGetIPFromRBridgeId : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));

        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::isClusterConnectedtoIpAddress (const string &ipAddress, vector<IpAddressPingResponseParameters*>& connectedNodesList)
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

                    /*
                    *  Set the ping response in out param.
                     * IpAddressPingResponseParameters is used to add other ping attributes later without interface changes.
                    */
                    temp->rbridgeId = rbridgeIdVec[i];
                    connectedNodesList.push_back(temp);
                }
            }
            else
            {
                // Assuming failure of message completion due to switch IP not found in VcsNodeLocalMO's
                trace (TRACE_LEVEL_WARN, string("ValClientSynchronousConnection::isClusterConnectedtoIpAddress : Message completion failed : ") + FrameworkToolKit::localize (completionStatus));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::isClusterConnectedtoIpAddress : Message processing failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
        trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::isClusterConnectedtoIpAddress : Client connection failed : ") + FrameworkToolKit::localize (clientStatus));
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::vcsGetClusterRbridgeIdFromWWN (const WorldWideName &wwn, BitMap &rbridgeIdBitMap)
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

ResourceId  ValClientSynchronousConnection::isGvlanCapable (bool &gvlanCapable)
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

ResourceId  ValClientSynchronousConnection::isGvlanEnabled (bool &gvlanEnabled)
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

ResourceId  ValClientSynchronousConnection::isFabricDistributionEnabled (bool &isDistributionEnabled)
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
            trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isFabricDistributionEnabled : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isFabricDistributionEnabled : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
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
        trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::isFabricDistributionEnabled : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    isDistributionEnabled = message.isFabricDistributionEnabled ();
    return (clientStatus);
}

ResourceId  ValClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds (vector<UI32> &rbridgeIds, vector<LocationId> &locationIds)
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
            trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : Sending message failed : ") + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : Message Processing Failed: ") + FrameworkToolKit::localize (clientStatus));
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
        trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : ClientConnection Status = ") + FrameworkToolKit::localize (clientStatus));
    }

    rbridgeIds = message.getRbridgeIds ();
    locationIds = message.getLocationIds ();

    trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::getCurrentRbridgeIdsAndLocationIds : Size of rbridge id vector: ") + rbridgeIds.size () + ", Size of location id vector: " + locationIds.size ());

    return (clientStatus);
}

/**
 * Name: runDebugScript
 * Description: We will convert the input rbridgeIdRange into location ids here in the Dcm Client layer.
 *              The reason is because the Wave Framework has it's own independent way to distinguish nodes
 *              by location id. In this method we will do the rbridge id to location id conversion and then
 *              delegate the work to the Wave Client's parent method.
 *
 * @param fullyQualifiedScriptFileName: input script file
 * @param nosOrbash:  is the script nos or shell script
 * @param fullyQualifiedOutputFileName : output file
 * @param rbridgeIdRange : range of rbridge ids
 *
 * @return ResourceId: success or failure
 */
ResourceId ValClientSynchronousConnection::runDebugScript (const string & fullyQualifiedScriptFileName, const string & nosOrbash, const string & fullyQualifiedOutputFileName, const string & rbridgeIdRange)
{
    trace (TRACE_LEVEL_DEVEL, string("ValClientSynchronousConnection::runDebugScript : Entering..."));

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
        trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::runDebugScript : getCurrentRbridgeIdsAndLocationIds Status : ") + FrameworkToolKit::localize (clientStatus));

        return (clientStatus);
    }


    // Build a map and reverse map of Rbridge Ids and Location Ids
    // The Location Ids to Rbridge Ids map will be used later to convert
    // the location Id tagged response output back to rbridge Ids.

    numberOfRbridges = rbridgeIds.size ();
    numberOfLocations = locationIds.size ();

    if (numberOfRbridges != numberOfLocations)
    {
        trace (TRACE_LEVEL_FATAL, string("ValClientSynchronousConnection::runDebugScript : Mismatch detected between number of Rbridge Ids / Location Ids : ") + numberOfRbridges + " / " + numberOfLocations);
        prismAssert (false, __FILE__, __LINE__);
    }

    for (UI32 i = 0; i < numberOfRbridges; ++i)
    {
        trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::runDebugScript : Current Rbridge Id / Location Id : ") + rbridgeIds[i] + " / " + locationIds[i]);

        mapOfRbridgeIdsToLocationIds[rbridgeIds[i]] = locationIds[i];
        mapOfLocationIdsToRbridgeIds[locationIds[i]] = rbridgeIds[i];
    }


    // Case 1: rbridgeIdRange = ""          => Run command/script on this node's only
    //      For Case 1, we will simply get this node's rbridge id and fall into Case 3.
    // Case 2: rbridgeIdRange = "all"       => Run command/script on all nodes in cluster
    // Case 3: rbridgeIdRange = "1,4-6,10"  => Run command/script on specified nodes

    trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::runDebugScript : Rbridge Id Range : \"") + tempRbridgeIdRange + "\"");

    if ("" == tempRbridgeIdRange)
    {
        // Case 1: Get this node's rbridge-id

        UI32 thisNodeRbridgeId = 0;

        clientStatus = vcsGetRBridgeId (thisNodeRbridgeId);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::runDebugScript : vcsGetRbridgeId Status : ") + FrameworkToolKit::localize (clientStatus));

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

                trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::runDebugScript : Found location id : ") + mapOfRbridgeIdsToLocationIds[tempRbridgeIdVector[i]] + " for Rbridge-id : " + tempRbridgeIdVector[i]);
            }
            else
            {
                trace (TRACE_LEVEL_WARN, string("ValClientSynchronousConnection::runDebugScript : Could not find a corresponding location id for Rbridge-id : ") + tempRbridgeIdVector[i]);
            }
        }

        sort (tempLocationIdVector.begin (), tempLocationIdVector.end ());

        commaDelimitedLocationIds = UI32Range::getUI32RangeStringFromVector (tempLocationIdVector);
    }

    trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::runDebugScript : Location Range : \"") + commaDelimitedLocationIds + "\"");


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

        trace (TRACE_LEVEL_DEBUG, string("ValClientSynchronousConnection::runDebugScript : locations vector size : ") + locationIds.size () + ", output string vector size : " + outputStringVector.size ());


        // Combine output from all locations

        for (UI32 i = 0; i < static_cast<UI32> (newLocationIds.size ()); ++i)
        {
            UI32 convertedRbridgeId (mapOfLocationIdsToRbridgeIds[newLocationIds[i]]);

            char rbridgeIdInfoString[256];
            sprintf (rbridgeIdInfoString, "\nOutput for Rbridge-id %d:\n", convertedRbridgeId);

            string tempString = rbridgeIdInfoString;
            tempString += outputStringVector[i];

            trace (TRACE_LEVEL_DEBUG, string ("ValClientSynchronousConnection::runDebugScript : Location Id: ") + newLocationIds[i] + tempString);

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
                trace (TRACE_LEVEL_FATAL, "ValClientSynchronousConnection::runDebugScript : Could not open output file" );
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
        trace (TRACE_LEVEL_ERROR, string ("ValClientSynchronousConnection::runDebugScript : Call to parent ValClientSynchronousConnection::runDebugScript returned: ") + FrameworkToolKit::localize (clientStatus));
    }

    return (clientStatus);
}

ResourceId  ValClientSynchronousConnection::disconnectInstanceClients (vector<string> arguments)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;
    FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage message;
    ResourceId       completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId       clientStatus     = WAVE_MESSAGE_ERROR;

    if (arguments.size () == 0) {
	    WaveNs:: trace (TRACE_LEVEL_ERROR, string("ValClientSynchronousConnection::disconnectInstnaceClients: ipaddress missing"));
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
            WaveNs:: trace (TRACE_LEVEL_ERROR, string ("ValClientSynchronousConnection::disconnectInstnaceClients : Sending message failed :") + status);

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                 WaveNs:: trace (TRACE_LEVEL_ERROR, string ("ValClientSynchronousConnection::disconnectInstnaceClients : Message Processing failed :") + completionStatus);
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

ResourceId ValClientSynchronousConnection:: createPortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::deletePortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deletePortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deletePortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::createVlanProfileForPortProfile(const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setSwitchPortForVlanProfileForPortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setSwitchPortModeAccessForVlanProfileForPortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortModeAccessForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortModeAccessForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setSwitchPortAccessVlanForVlanProfileForPortProfile (const string &portProfileName, const UI32 &vlanId)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortAccessVlanForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortAccessVlanForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setSwitchPortAllowedVlansForVlanProfileForPortProfile (const string &portProfileName, const UI32Range &allowedVlans)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setSwitchPortModeTrunkForVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::activatePortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::activatePortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::activatePortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection:: deactivatePortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deactivatePortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deactivatePortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::associateMacToPortProfile (const string &portProfileName, const string &macAddress)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacToPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacToPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId  ValClientSynchronousConnection::configCdpProtocol (bool &cenable)
{
	CdpGlobalConfSetMessage message;

    trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::configCdpProtocol : " + cenable );
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    message.setProtoEnable(cenable);
    message.setCmdcode(CDP_GLOBAL_CONF_PROTO_STATUS);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::configCdpProtocol : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::configCdpProtocol : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


/* vCenter APPM APIs */ 

ResourceId  ValClientSynchronousConnection::getProfileInfoFromDcmdDB ( PortProfileObject &pPortProfileObject)

{

    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&pPortProfileObject);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::getProfileInfoFromDcmdDB: Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = pPortProfileObject.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::getProfileInfoFromDcmdDB: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::getAllPortProfilesFromDcmd (string pattern, vector<string> &portprofileList)
{

    VcenterGetAllPortProfileListMessage message;
    WaveMessageStatus            status;
    ResourceId                   completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {

       message.setPattern(pattern);
        status = sendSynchronouslyToWaveServer (&message);
       if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::VcenterGetAllPortProfileListMessage: Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            portprofileList = message.getPortProfiles();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::VcenterGetAllPortProfileListMessage: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

return clientStatus;
}

ResourceId ValClientSynchronousConnection::setDefaultDomainForPortProfile (const string &portProfileName)
{
    APPMGlobalProfileDomainMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;
    message.setPortProfileName (portProfileName);
    message.setPortProfileDomainName (DEFAULT_PORT_PROFILE_DOMAIN_NAME);
    message.setCmdCode(PORT_PROFILE_ADD_TO_PORT_PROFILE_DOMAIN);

    if (true == (isCurrentlyConnected ()))
    {
      trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection:: Sending sendSynchronouslyToWaveServer....." );
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setDefaultDomainForPortProfile: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setDefaultDomainForPortProfile: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::removePortProfileFromDefaultDomain (const string &portProfileName)
{
    APPMGlobalProfileDomainMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;
    message.setPortProfileName (portProfileName);
    message.setPortProfileDomainName (DEFAULT_PORT_PROFILE_DOMAIN_NAME);
    message.setCmdCode(PORT_PROFILE_DELETE_FROM_PORT_PROFILE_DOMAIN);

    if (true == (isCurrentlyConnected ()))
    {
      trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection:: Sending sendSynchronouslyToWaveServer....." );
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::removePortProfileFromDefaultDomain: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::removePortProfileFromDefaultDomain: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::createAccessVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32 &vlanId)
{
    APPMGlobalAPPMVlanMessage message;
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;
    trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::  createAccessVlanProfileWithVlanForPortProfile" );
    message.setProfileName (portProfileName);
    message.setOpCode      (APPM_VCENTER);
    message.setCmdCode     (VCENTER_ACCESS_VLAN_PROFILE_CREATE);
    message.setSwModeFlag  (true);
    message.setSwMode      (VLAN_SW_MODE_ACCESS);
    message.setAccessVlan  (vlanId);

    if (true == (isCurrentlyConnected ()))
    {
      trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection:: Sending sendSynchronouslyToWaveServer....." );
        status = sendSynchronouslyToWaveServer (&message);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::createSwitchportTrunkNativeVlanProfile (const string &portProfileName, const UI32 &vlanId)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::updateAccessVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32 &vlanId)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createVlanProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::createBulkVlanInterface (const UI32Range &vlanRange)
{
    ResourceId    clientStatus = WAVE_MESSAGE_SUCCESS;
    vector <UI32> vlanCreateVector;
    vlanRange.getUI32RangeVector(vlanCreateVector);

    vector<UI32>::iterator vlaniter;

    for (vlaniter=vlanCreateVector.begin(); vlaniter != vlanCreateVector.end(); vlaniter++) {

            string vlanStr = (Integer (*vlaniter)).toString();
            if(vlanStr =="0" || vlanStr =="4091" || vlanStr =="4092" || vlanStr =="4093" || vlanStr =="4094" || vlanStr =="4095") {
                trace (TRACE_LEVEL_INFO, "skipping vlan: " + vlanStr);
                continue;
            }
            trace (TRACE_LEVEL_INFO, "creating vlan: " + vlanStr);
            createVlanInterface (vlanStr,0,0,"");
        }
    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::createTrunkVlanAllProfilePortProfile(const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createTrunkVlanAllProfilePortProfile: Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createTrunkVlanAllProfilePortProfile: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::createTrunkVlanNoneProfilePortProfile(const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createTrunkVlanNoneProfilePortProfile: Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createTrunkVlanNoneProfilePortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::createTrunkVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32Range &allowedVlans)
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
    message.setVlanRange   (allowedVlans);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);
		if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::: SendingteTrunkVlanProfileWithVlanForPortProfile message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createTrunkVlanProfileWithVlanForPortProfile: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::updateTrunkVlanProfileWithVlanForPortProfile (const string &portProfileName, const UI32Range &allowedVlans)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::updateTrunkVlanProfileWithVlanForPortProfile: Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
				trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::updateTrunkVlanProfileWithVlanForPortProfile: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::deleteVlanProfileForPortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deleteVlanProfileForPortProfile: Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deleteVlanProfileForPortProfile: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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


ResourceId ValClientSynchronousConnection::associateMacToVcenterPortProfile(const string &portProfileName, const string &macAddress)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacToVcenterPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacToVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::associateMacsToPortProfile (const string &portProfileName, vector <string> &macAddressList)
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
		 trace (TRACE_LEVEL_INFO,string("ValClientSynchronousConnection::associateMacsToPortProfile:: mac in vector= ") + macVector.size());

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
			trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacsToPortProfile::Sending message" );
        	status = sendSynchronouslyToWaveServer (&message);
	        if (WAVE_MESSAGE_SUCCESS != status)
    	    {
        	    trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacToVcenterPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            	clientStatus = status;
	        }
    	    else
        	{
            	completionStatus = message.getCompletionStatus ();
	            if (WAVE_MESSAGE_SUCCESS != completionStatus)
    	        {
        	        clientStatus = completionStatus;
            	    trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacToVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
}

ResourceId ValClientSynchronousConnection::disassociateMacFromVcenterPortProfile (const string &portProfileName, const string &macAddress)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::disassociateMacFromVcenterPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::disassociateMacFromVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
/*
ResourceId ValClientSynchronousConnection::disassociateMacsFromPortProfile (const string &portProfileName, vector <string> &macAddressList)
{
    APPMGlobalAPPMProfileMessage message;
    ResourceId                   clientStatus     = WAVE_MESSAGE_ERROR;
    string macString;
    vector<string>::iterator maciter;
    for (maciter = macAddressList.begin(); maciter != macAddressList.end(); maciter++) {
            macString += (*maciter);
            clientStatus = disassociateMacFromVcenterPortProfile(portProfileName, macString);
        }
    return (clientStatus);
}
*/

ResourceId ValClientSynchronousConnection::deAssociateMacsFromPortProfile (const string &portProfileName, vector <string> &macAddressList)
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
         trace (TRACE_LEVEL_INFO,string("ValClientSynchronousConnection::deAssociateMacsFromPortProfile:: mac in vector= ") + macVector.size());

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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deAssociateMacsFromPortProfile::Sending message" );
            status = sendSynchronouslyToWaveServer (&message);
            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deAssociateMacsFromPortProfile: Sending message failed : " + FrameworkToolKit::localize (status));
                clientStatus = status;
            }
            else
            {
                completionStatus = message.getCompletionStatus ();
                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    clientStatus = completionStatus;
                    trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::associateMacToVcenterPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
}

ResourceId ValClientSynchronousConnection::createQoSProfileWithDefaultCOSForPortProfile (const string &portProfileName, const UI32 &cosValue)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createQoSProfileWithDefaultCOSForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createQoSProfileWithDefaultCOSForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId ValClientSynchronousConnection::updateQoSProfileWithDefaultCOSForPortProfile (const string &portProfileName, const UI32 &cosValue)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::updateQoSProfileWithDefaultCOSForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::updateQoSProfileWithDefaultCOSForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::deleteQoSProfileForPortProfile (const string &portProfileName)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deleteQoSProfileForPortProfile : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::deleteQoSProfileForPortProfile : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

/* vCenter APPM APIs END  */

ResourceId ValClientSynchronousConnection::sendSyncUpdateToCcmd (const string &peerHaIpAddress, const SI32 &peerHaIpPort, const UI32 &context)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::sendSyncUpdateToCcmd : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::sendSyncUpdateToCcmd : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
void ValClientSynchronousConnection::retrievePowerStatus(
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
                        "ValClientSynchronousConnection::Vcenter : VM Power Status : Sending message failed : "
                                + FrameworkToolKit::localize(status));

                clientStatus = status;
            }
            else {
                completionStatus = message.getCompletionStatus();

                if (WAVE_MESSAGE_SUCCESS != completionStatus) {
                    clientStatus = completionStatus;

                    trace(
                            TRACE_LEVEL_DEBUG,
                            "ValClientSynchronousConnection::Vcenter : VM Power Status : Message Processing failed : "
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

    ResourceId ValClientSynchronousConnection::dcmvCenterEventNotify(
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
                        "ValClientSynchronousConnection::dcmEventEngineNotify : Sending message failed : "
                                + FrameworkToolKit::localize(status));
            }
        }
        else {
            status = getConnectionStatus();
            trace(
                    TRACE_LEVEL_INFO,
                    "ValClientSynchronousConnection::dcmEventEngineNotify : EventEngine client not connected: "
                            + FrameworkToolKit::localize(status));
        }

        return (status);
}

ResourceId ValClientSynchronousConnection::setIsCliAllowedFromThisLocation (bool isCliAllowedFromThisLocation)
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setIsCliAllowedFromThisLocation : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setIsCliAllowedFromThisLocation : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId ValClientSynchronousConnection::setModeChangeParameters (const UI32 &vcsId, const string &vcsMode, const UI32 &rbridgeId, const bool &rebootSw)
{
    VcsNodeSetModeChangeParametersMessage           message;
    WaveMessageStatus                               status;
    ResourceId                                      completionStatus    = WAVE_MESSAGE_SUCCESS;
    ResourceId                                      clientStatus        = WAVE_MESSAGE_ERROR;

    message.setVcsId      (vcsId);
    message.setVcsMode    (vcsMode);
    message.setRbridgeId  (rbridgeId);
    message.setRebootSw   (rebootSw);

    status = sendSynchronouslyToWaveServer (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setModeChangeParameters : Sending message failed : " + FrameworkToolKit::localize (status));
        clientStatus = status;
    }
    else
    {
        completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            clientStatus = completionStatus;

            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setIsCliAllowedFromThisLocation : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
        }
        else
        {
            clientStatus = WAVE_MESSAGE_SUCCESS;
        }
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::getClusterStatistics (WaveClientMessageCallbackHandler handler)
{
    VcsFabricDebugClientMessage *pVcsFabricDebugClientMessage = new VcsFabricDebugClientMessage ();
    WaveMessageStatus            status;
    ResourceId                   clientStatus                 = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ())) 
    {
        status = sendToWaveServer (pVcsFabricDebugClientMessage, handler);

        if (WAVE_MESSAGE_SUCCESS != status)
        {    
            trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::getClusterStatistics: Sending message failed : " + FrameworkToolKit::localize (status));

        }    

        clientStatus = status;
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::setTtyName (const string &ttyName)
{
    if (true == ttyName.empty ())
    {
        trace (TRACE_LEVEL_ERROR, "ValClientSynchronousConnection::setTtyName : ttyName is an empty string .. Please try again with a proper argument");
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
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setTtyName : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = pVcsFabricSetTtyNameMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::setTtyName : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
}

ResourceId ValClientSynchronousConnection::retrievevCenterMachineStatus( vector<vector<string> > &respList) {
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
                        "ValClientSynchronousConnection::Vcenter : retrievevCenterMachineStatus : Sending message failed : "
                                + FrameworkToolKit::localize(status));

                clientStatus = status;
            }
            else {
                completionStatus = message.getCompletionStatus();

                if (WAVE_MESSAGE_SUCCESS != completionStatus) {
                    clientStatus = completionStatus;

                    trace(
                            TRACE_LEVEL_DEBUG,
                            "ValClientSynchronousConnection::Vcenter : retrievevCenterMachineStatus : Message Processing failed : "
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
}

ResourceId ValClientSynchronousConnection::isNosSmdReadyForConnection (bool &isReady)
{
    NosSmdReadyForConnectionMessage                     message;
    WaveMessageStatus                                   status;
    ResourceId                                          completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                          clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::isNosSmdReadyForConnection : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "ValClientSynchronousConnection::isNosSmdReadyForConnection : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                isReady = message.getIsReady ();
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::setHasmFeature(char *feature, int state)
{
	tracePrintf(TRACE_LEVEL_INFO, "ValClientSynchronousConnection::setHasmFeature for feature:%s, state:%d.", feature, state);

	has_set_feature(feature, state);

	return WAVE_MESSAGE_SUCCESS;
}
ResourceId ValClientSynchronousConnection::loadZoneBulkConfiguration (const string &zoneConfigurationFilePath)
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

ResourceId ValClientSynchronousConnection::createOpenFlowSwitch (const string &openFlowSwitchName, const Uri &openFlowSwitchUri)
{
    OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage message;
    ResourceId                                             clientStatus = WAVE_MESSAGE_ERROR;

    message.setOpenFlowSwitchName (openFlowSwitchName);
    message.setOpenFlowSwitchUri  (openFlowSwitchUri);

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createOpenFlowSwitch : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createOpenFlowSwitch : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::createOpenFlowLogicalSwitch (const string &openFlowLogicalSwitchName, const Uri &openFlowLogicalSwitchUri)
{
    OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage message;
    ResourceId                                                    clientStatus = WAVE_MESSAGE_ERROR;

    message.setOpenFlowLogicalSwitchName (openFlowLogicalSwitchName);
    message.setOpenFlowLogicalSwitchUri  (openFlowLogicalSwitchUri);

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createOpenFlowLogicalSwitch : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::createOpenFlowLogicalSwitch : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::addFlow (const UI32 &ingressIfIndex, const UI32 &egressIfIndex, const UI32 &srcIpV4Address, const UI32 &srcIpV4AddressMask,const UI32 &dstIpV4Address, const UI32 &dstIpV4AddressMask, const string &appType, const UI16 &srcPort, const UI16 &dstPort)
{
    OpenFlowSwitchObjectManagerAddFlowMessage    message;
    ResourceId                                   clientStatus = WAVE_MESSAGE_ERROR;

    message.setOpenFlowIngressIfIndex (ingressIfIndex);
    message.setOpenFlowEgressIfIndex (egressIfIndex);
    message.setOpenFlowSrcIpV4Address (srcIpV4Address);
    message.setOpenFlowSrcIpV4AddressMask (srcIpV4AddressMask);
    message.setOpenFlowDstIpV4Address (dstIpV4Address);
    message.setOpenFlowDstIpV4AddressMask (dstIpV4AddressMask);
    message.setOpenFlowAppType (appType);
    message.setOpenFlowSrcPort (srcPort);
    message.setOpenFlowDstPort (dstPort);

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::testFtm : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::testFtm : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::addOpenFlowControllerToOpenFlowLogicalSwitch (const string &openFlowLogicalSwitchName, const Uri &openFlowLogicalSwitchUri, const IpV4AddressNetworkMask ipPrefix, UI32 port)
{
    OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage message;
    ResourceId                                                                     clientStatus = WAVE_MESSAGE_ERROR;

    message.setOpenFlowLogicalSwitchName   (openFlowLogicalSwitchName);
    message.setOpenFlowLogicalSwitchUri    (openFlowLogicalSwitchUri);
    message.setOpenFlowControllerIpAddress (ipPrefix.toString ());
    message.setOpenFlowControllerPort      (port);

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::addOpenFlowControllerToOpenFlowLogicalSwitch : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::addOpenFlowControllerToOpenFlowLogicalSwitch : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId ValClientSynchronousConnection::connectOpenFlowLogicalSwitchToOpenFlowController (const string &openFlowLogicalSwitchName, const Uri &openFlowLogicalSwitchUri, const IpV4AddressNetworkMask ipPrefix, UI32 port)
{
    OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage message;
    ResourceId                                                                         clientStatus = WAVE_MESSAGE_ERROR;

    message.setOpenFlowLogicalSwitchName   (openFlowLogicalSwitchName);
    message.setOpenFlowLogicalSwitchUri    (openFlowLogicalSwitchUri);
    message.setOpenFlowControllerIpAddress (ipPrefix.toString ());
    message.setOpenFlowControllerPort      (port);

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::connectOpenFlowLogicalSwitchToOpenFlowController : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "ValClientSynchronousConnection::connectOpenFlowLogicalSwitchToOpenFlowController : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}


}
