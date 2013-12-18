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

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/Nsm/NsmVlanIntfMessage.h"
#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"
#include "ClientInterface/Nsm/NsmPoIntfMessage.h"
#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"
#include "ClientInterface/Nsm/NsmShowIntfMessage.h"
#include "ClientInterface/Nsm/NsmPortMediaShowMessage.h"
#include "ClientInterface/Nsm/NsmDebugMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmFcoeVlanMessage.h"
#include "ClientInterface/Nsm/NSMClientReservedVlanMessage.h"
#include "ClientInterface/Nsm/NsmFcoeportAttrMessage.h"
#include "ClientInterface/Nsm/NsmClearMessage.h"
#include "ClientInterface/Nsm/NsmDefaultConfigurationCmdsMessage.h"
#include "ClientInterface/Nsm/NsmClientUpdateGlobalConfigMessage.h"
#include "ClientInterface/Nsm/ConfigMgmtShowMessage.h"
#include "ClientInterface/Nsm/NsmEcmpConfigMessage.h"
#include "ClientInterface/Nsm/NsmShowEcmpMessage.h"
#include "ClientInterface/Nsm/NsmBeaconMessage.h"

#include "ClientInterface/Ssm/QosClientConfigIntfNsmAttributesMessage.h"

//VlanClassifier Header Files

#include "ClientInterface/Nsm/NsmClientCreateVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/NsmClientUpdateVlanClassifierGroupSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierGroupSPMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/Span/SpanClientLocalSpanLocalConfigMessage.h"
#include "ClientInterface/Span/SpanShowMessage.h"

#include "Nsm/Global/NsmGlobalAddVlanClassifierToPoMessage.h"
#include "ClientInterface/Nsm/NsmShowVlanClassifierMessage.h"
#include "VcsFabric/Local/VcsNodeHaMessages.h"

//Vlan Classifier Header files end heres

/*  Lacp Config Header files */

#include "ClientInterface/Nsm/NsmClientUpdateLacpConfigMessage.h"
#include "ClientInterface/Nsm/NsmClientAddVlanClassifierToPoSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierToPoSPMessage.h"
//PVLAN
#include "ClientInterface/Nsm/NsmClientPVlanTrunkAssocMessage.h"
#include "ClientInterface/Nsm/NsmClientPVlanMappingMessage.h"

#include "ClientInterface/Nsm/LacpShowMessage.h"
#include "ClientInterface/Nsm/LacpDebugMessage.h"
#include "ClientInterface/Nsm/LacpClearMessage.h"
#include "ClientInterface/Nsm/NsmVlagConfigMessage.h"
#include "ClientInterface/Nsm/NsmShowVlagMessage.h"
#include "ClientInterface/DceDbg/DceDbgMsg.h"

#include "ClientInterface/Nsm/MacRebalanceMessage.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"

#include "ClientInterface/DceWaveClientMessage.h"
/*APPM related header files*/
#include "ClientInterface/APPM/APPMProfileMessage.h"
#include "ClientInterface/APPM/APPMPortProfileDomainConfigMessage.h"
#include "ClientInterface/APPM/APPMProfileBulkMessage.h"
#include "ClientInterface/APPM/APPMVlanProfileMessage.h"
#include "ClientInterface/APPM/APPMFcoeProfileMessage.h"
#include "ClientInterface/APPM/PortProfileIfAssociationMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

 /* APPM Functionality header files*/
#include "ClientInterface/APPM/APPMShowMessage.h"

 /* VRRP Functionality header files*/
#include "ClientInterface/VRRP/VRRPShowMessage.h"

#include "ClientInterface/Fcoe/FcoeClientShowMessage.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "Nsm/Local/NsmTypes.h"
#include "WyserEaGateway/GenericClientMessage.h"
#include "ClientInterface/Nsm/NsmClientConfigVRFMessage.h"
#include "ClientInterface/Nsm/ConnectorShowMessage.h"
#include "ClientInterface/Nsm/MacGroupConfigMessage.h"
#include "ClientInterface/Nsm/SwitchportVlanClassificationMessage.h"
#include "ClientInterface/Nsm/GVlanEnableMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage *NSMclientMangIntfMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case NSM_CLEAR_MSG:
            pDcmManagementInterfaceMessage =  new NsmClearMessage();
            break;
        case NSMVLANINTF:
            pDcmManagementInterfaceMessage =  new NsmVlanIntfMessage();
            break;
        case NSMPOINTF:
            pDcmManagementInterfaceMessage =  new NsmPoIntfMessage();
            break;
        case NSMPHYINTF:
            pDcmManagementInterfaceMessage =  new NsmPhyIntfMessage();
            break;
        case NSM_SHOW_INTERFACE:
            pDcmManagementInterfaceMessage =  new NsmShowIntfMessage();
            break;
        case NSM_SHOW_VLAN_CLASSIFIER:
            pDcmManagementInterfaceMessage =  new NsmShowVlanClassifierMessage();
            break;
        case NSM_PORTMEDIA_SHOW_INTERFACE:
            pDcmManagementInterfaceMessage =  new NsmPortMediaShowIntfMessage();
            break;
        case NSM_DEBUG_HANDLER:
            pDcmManagementInterfaceMessage =  new NsmDebugMessage();
            break;
        case NSMCLIENTCREATEVLANCLASSIFIERRULESP:
            pDcmManagementInterfaceMessage =  new NsmClientCreateVlanClassifierRuleSPMessage(); 	
            break;
        case NSMCLIENTDELETEVLANCLASSIFIERRULESP:
            pDcmManagementInterfaceMessage =  new NsmClientDeleteVlanClassifierRuleSPMessage();
            break;
        case NSMCLIENTDELETEVLANCLASSIFIERGROUPSP:
            pDcmManagementInterfaceMessage =  new NsmClientDeleteVlanClassifierGroupSPMessage();
            break;

        case NSMCLIENTUPDATEVLANCLASSIFIERGROUPSP:
            pDcmManagementInterfaceMessage =  new NsmClientUpdateVlanClassifierGroupSPMessage();
            break;
	case NSMCLIENTADDVLANCLASSIFIERTOPOSP:
            pDcmManagementInterfaceMessage =  new NsmClientAddVlanClassifierToPoSPMessage();
            break;
	case NSMCLIENTDELETEVLANCLASSIFIERTOPOSP:
            pDcmManagementInterfaceMessage =  new NsmClientDeleteVlanClassifierToPoSPMessage();
            break;
        case NSMGLOBALLACPCONFIG:
            pDcmManagementInterfaceMessage =  new NsmClientUpdateLacpConfigMessage();
            break;
        
		case NSM_CLIENT_PVLAN_TRUNK_ASSOC_MSG:
            pDcmManagementInterfaceMessage =  new NsmClientPVlanTrunkAssocMessage();
            break;

		case NSM_CLIENT_PVLAN_MAPPING_MSG:
            pDcmManagementInterfaceMessage =  new NsmClientPVlanMappingMessage();
            break;

		case NSM_GLOBAL_CONFIG_MSG:
            pDcmManagementInterfaceMessage =  new NsmClientUpdateGlobalConfigMessage();
            break;

			/*ECMP*/
		case NSMECMPCONFIG:
			pDcmManagementInterfaceMessage = new NsmEcmpConfigMessage();
			break;
		case NSM_SHOW_ECMP:
			pDcmManagementInterfaceMessage =  new NsmShowEcmpMessage();
			break;

			/* APPM */
		case APPMPROFILE:
			pDcmManagementInterfaceMessage =  new APPMProfileMessage();
			break;

		case APPMVLANPROFILE:
			pDcmManagementInterfaceMessage =  new APPMVlanProfileMessage();
			break;

		case APPMFCOEPROFILE:
			pDcmManagementInterfaceMessage =  new APPMFcoeProfileMessage();
			break;

		case APPMSHOWMESSAGE:
			pDcmManagementInterfaceMessage =  new APPMShowMessage();
			break;

		case APPMPROFILEBULKMACASSOC:
			pDcmManagementInterfaceMessage =  new APPMProfileBulkMessage();
			break;

        case APPMPORTPROFILEDOMAIN:
            pDcmManagementInterfaceMessage = new APPMPortProfileDomainConfigMessage();
            break;

        case PORT_PROFILE_INTERFACE_ASSOCIATION: 
            pDcmManagementInterfaceMessage = new PortProfileInterfaceAssociationMessage();
            break;

			/* VRRP */
		case VRRP_SHOW_MSG:
			pDcmManagementInterfaceMessage =  new VRRPShowMessage();
			break;

			
        case NSMFCOEVLAN:
            pDcmManagementInterfaceMessage = new NsmFcoeVlanMessage();
            break;
      
        case NSMCLIENTRESERVEDVLAN:
            pDcmManagementInterfaceMessage = new NsmClientReservedVlanMessage();
            break;


        case NSMFCOEPORT:
            pDcmManagementInterfaceMessage = new NsmFcoeportAttrMessage();
            break;
        case NSM_DEFAULT_CONFIG_CMD:
            pDcmManagementInterfaceMessage = new NsmDefaultConfigurationCmdsMessage();
            break;
	case NSMCLIENTCREATESPANLOCALCONFIG:
            pDcmManagementInterfaceMessage = new SpanClientLocalSpanLocalConfigMessage();
            break;
		case SPANSHOWMESSAGE:
			pDcmManagementInterfaceMessage = new SpanShowMessage();
			break;
       case MSG_IP_INT:
              pDcmManagementInterfaceMessage  = new NsmClientCreateIntIpConfigSPMessage();
				break;
            
	   case MSG_CONFIGMGMT_SHOW:
              pDcmManagementInterfaceMessage  = new ConfigMgmtShowMessage();
				break;

        case DCE_DBG_MSG:
            pDcmManagementInterfaceMessage  = new DceDbgMsg();
            break;
        
        case FCOE_CLIENT_SHOW:
            pDcmManagementInterfaceMessage = new FcoeClientShowMessage();
            break;

			/*
			 * NSM debug commands.
			 */
			
		case NSMVLAGCONFIG:
			pDcmManagementInterfaceMessage = new NsmVlagConfigMessage();
			break;

		case NSM_SHOW_VLAG:
			pDcmManagementInterfaceMessage = new NsmShowVlagMessage();
			break;
			
		case NSM_INTERNAL_SHOW_PORT_PROFILES:
			pDcmManagementInterfaceMessage = new GenericClientMessage
											<NSM_INTERNAL_SHOW_PORT_PROFILES>();
			
        	break;

		case NSM_INTERNAL_SHOW_PROFILED_PORT:
			pDcmManagementInterfaceMessage = new GenericClientMessage
											 <NSM_INTERNAL_SHOW_PROFILED_PORT>();
			break;
		
		case NSM_INTERNAL_SHOW_GVLAN:
			pDcmManagementInterfaceMessage = new GenericClientMessage <NSM_INTERNAL_SHOW_GVLAN>();
        	break;

		case NSM_INTERNAL_SHOW_IVID:
			pDcmManagementInterfaceMessage = new GenericClientMessage <NSM_INTERNAL_SHOW_IVID>();
        	break;

		case NSMBEACONMSG: 
			pDcmManagementInterfaceMessage = new NsmBeaconMessage();
			break;

// Cp state change message for x86            
        case NSM_CP_STATE_CHANGE_MSG:
        {
            pDcmManagementInterfaceMessage = new VcsNodeCpStateChangeClientMessage (NSM_CP_STATE_CHANGE_MSG);
            break;
        }
			
        case VLAGEXMREBALANCE:
            pDcmManagementInterfaceMessage = new MacRebalanceMessage();
            break;

        case QOSCLIENTCONFIGINTFNSMATTRIBUTES:
            pDcmManagementInterfaceMessage = new QosClientConfigIntfNsmAttributesMessage();
            break;

        case NSM_VCS_ISL_DISABLE:
            pDcmManagementInterfaceMessage = new GenericClientMessage<NSM_VCS_ISL_DISABLE>();
            break;

        case MSG_VRF_INT:
            pDcmManagementInterfaceMessage  = new NsmClientConfigVRFMessage();
			break;

        case SHOW_SFP_MESSAGE:
            pDcmManagementInterfaceMessage  = new ConnectorShowMessage();
			break;

        case NSM_VLAN_PROVISION:
            pDcmManagementInterfaceMessage = new NsmVlanProvisionMessage();
            break;
        case SW_VLAN_CLASSIFICATION_MESSAGE:
            pDcmManagementInterfaceMessage = new SwitchportVlanClassificationMessage();
            break;
        case MAC_GROUP_CONFIG_MESSAGE:
            pDcmManagementInterfaceMessage = new MacGroupConfigMessage();
            break;
        case GVLAN_ENABLE_MESSAGE:
            pDcmManagementInterfaceMessage = new GVlanEnableMessage();
            break;
        default:
            trace (TRACE_LEVEL_INFO, string ("NSMclientMangIntfMsgInstantiator : Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }
    return (pDcmManagementInterfaceMessage);
}

ManagementInterfaceMessage *LACPclientMangIntfMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case NSMGLOBALLACPCONFIG:
            pDcmManagementInterfaceMessage =  new NsmClientUpdateLacpConfigMessage();
            break;
        case NSMPHYINTF:
			pDcmManagementInterfaceMessage =  new NsmPhyIntfMessage();
            break;
        case LACP_SHOW:
			pDcmManagementInterfaceMessage =  new LacpShowMessage();
            break;
        case LACP_DEBUG_DCMD:
			pDcmManagementInterfaceMessage =  new LacpDebugMessage();
            break;
		case LACP_CLEARCOUNTER:
			pDcmManagementInterfaceMessage = new LacpClearMessage();
			break;
        default:
            trace (TRACE_LEVEL_INFO, string ("LACPclientMangIntfMsgInstantiator : Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

ManagementInterfaceMessage *PCAPclientMangIntfMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case PCAPCLIENTARPDEBUG:
            pDcmManagementInterfaceMessage =  new GenericClientMessage<PCAPCLIENTARPDEBUG>();
            break;

			case PCAPCLIENTACLDEBUG:
              pDcmManagementInterfaceMessage =  new GenericClientMessage<PCAPCLIENTACLDEBUG>();
              break;
			
			case PCAPCLIENTPKTCAPTURE:
				pDcmManagementInterfaceMessage = new GenericClientMessage<PCAPCLIENTPKTCAPTURE>();
				break;

        default:
            trace (TRACE_LEVEL_INFO, string ("PCAPclientMangIntfMsgInstantiator : Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }
    return (pDcmManagementInterfaceMessage);
}

int setNSMMangIntfMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (NSMclientMangIntfMsgInstantiator);
     return status;
}

int setLACPMangIntfMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (LACPclientMangIntfMsgInstantiator);
     return status;
}

int setPCAPMangIntfMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (PCAPclientMangIntfMsgInstantiator);
     return status;
}

}
