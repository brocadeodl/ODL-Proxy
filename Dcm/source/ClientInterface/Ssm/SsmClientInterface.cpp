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

#include "ClientInterface/Ssm/qoscommon.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Ssm/SsmMacAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmMacAclRuleMessage.h"
#include "ClientInterface/Ssm/SsmMacAclPolicyBulkMessage.h"
#include "ClientInterface/Ssm/SsmMacAclIntfAssocBulkMessage.h"
#include "ClientInterface/Ssm/SsmMacAclReseqMessage.h"
#include "ClientInterface/Ssm/SsmMacAclShowMessage.h"
/* IP Acl */
#include "ClientInterface/Ssm/SsmIpAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmIpAclRuleMessage.h"
#include "ClientInterface/Ssm/SsmIpAclReseqMessage.h"
#include "ClientInterface/Ssm/SsmMacIpAclShowMessage.h"
#include "ClientInterface/Ssm/SsmIpAclPolicyBulkMessage.h"
#include "ClientInterface/Ssm/SsmIpAclShowMessage.h"
#include "ClientInterface/Ssm/SsmIpAclIntfAssocBulkMessage.h"

// IP Policy - PBR
#include "ClientInterface/Ssm/SsmPbrShowClientMessage.h"

#include "ClientInterface/Ssm/QosClientCreateCosToCosMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteCosToCosMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCosToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteCosToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosRcvQueueMulticastSettingMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteQosRcvQueueMulticastSettingMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosQueueMulticastSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteQosQueueMulticastSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosQueueSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteQosQueueSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCeeMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteCeeMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCeeRemapMessage.h"
#include "ClientInterface/Ssm/QosClientCreatePriorityGroupMessage.h"
#include "ClientInterface/Ssm/QosClientDeletePriorityGroupMessage.h"
#include "ClientInterface/Ssm/QosClientCreatePriorityMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeletePriorityMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpMarkListMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpToCosMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpToDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpMarkListMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpToCosMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpToDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateRedProfileMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteRedProfileMessage.h"
#include "ClientInterface/Ssm/QosClientConfigReplayDscpTcMapMessage.h"
#include "ClientInterface/Ssm/QosClientConfigReplayDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientConfigReplayDscpCosMapMessage.h"
#include "ClientInterface/Ssm/QosClientConfigIntfNsmAttributesMessage.h"
#include "ClientInterface/Ssm/QosShowMessage.h"
/* Manually Restoring Mstp related stuff */
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpShowMessage.h"

#include "ClientInterface/Ssm/AclClearStatsMessage.h"
#include "ClientInterface/APPM/APPMQosProfileMessage.h"
#include "ClientInterface/APPM/APPMSecProfileMessage.h"
#include "ClientInterface/APPM/APPMFcoeProfileMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "WyserEaGateway/GenericClientMessage.h"
#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"
#include "ClientInterface/Ssm/QosClientClearMessage.h"

#include "ClientInterface/Ssm/BpratelimitClientConfigMessage.h"

/*APPM related header files*/
#include "ClientInterface/APPM/APPMProfileMessage.h"


using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *SsmClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
		case SSMMACACLPOLICY:
			pMsg = new SsmMacAclPolicyMessage();
			break;
		case SSMMACACLRULE:
			pMsg = new SsmMacAclRuleMessage();
			break;
		case SSMMACACLBULKPOLICY:
			pMsg = new SsmMacAclPolicyBulkMessage();
			break;
		case SSMMACACLSHOW:
			pMsg = new SsmMacAclShowMessage ();
			break;
		case SSMMACIPACLSHOW:
			pMsg = new SsmMacIpAclShowMessage ();
			break;
		case SSMIPACLSHOW:
			pMsg = new SsmIpAclShowMessage ();
			break;
		case SSMMACACLINTFASSOCBULK: 
			pMsg = new SsmMacAclIntfAssocBulkMessage();
			break;
		case SSMMACACLRESEQUENCE:
			pMsg = new SsmMacAclReseqMessage();
			break;
		/* Ip Acl */
		case SSMIPACLPOLICY:
			pMsg = new SsmIpAclPolicyMessage();
			break;
        case SSMIPACLRULE:
            pMsg = new SsmIpAclRuleMessage();
            break;
		case SSMIPACLRESEQUENCE:
			pMsg = new SsmIpAclReseqMessage();
			break;
		case SSMIPACLBULKPOLICY:
			pMsg = new SsmIpAclPolicyBulkMessage();
			break;
		case SSMIPACLINTFASSOCBULK: 
			pMsg = new SsmIpAclIntfAssocBulkMessage();
			break;
		// IP Policy - PBR
		case SSM_CLIENT_PBR_SHOW:
			pMsg = new SsmPbrShowClientMessage ();
			break;
		case QOSCLIENTCREATECOSTOCOSMUTATIONMAP:
			pMsg = new QosClientCreateCosToCosMutationMapMessage();
			break;
		case QOSCLIENTDELETECOSTOCOSMUTATIONMAP:
			pMsg = new QosClientDeleteCosToCosMutationMapMessage();
			break;
		case QOSCLIENTCREATECOSTOTRAFFICCLASSMAP:
			pMsg = new QosClientCreateCosToTrafficClassMapMessage();
			break;
		case QOSCLIENTDELETECOSTOTRAFFICCLASSMAP:
			pMsg = new QosClientDeleteCosToTrafficClassMapMessage();
			break;
		case QOSCLIENTCREATEQOSRCVQUEUEMULTICASTSETTING:
			pMsg = new QosClientCreateQosRcvQueueMulticastSettingMessage();
			break;
		case QOSCLIENTDELETEQOSRCVQUEUEMULTICASTSETTING:
			pMsg = new QosClientDeleteQosRcvQueueMulticastSettingMessage();
			break;
		case QOSCLIENTCREATEQOSQUEUEMULTICASTSCHEDULER:
			pMsg = new QosClientCreateQosQueueMulticastSchedulerMessage();
			break;
		case QOSCLIENTDELETEQOSQUEUEMULTICASTSCHEDULER:
			pMsg = new QosClientDeleteQosQueueMulticastSchedulerMessage();
			break;
		case QOSCLIENTCREATEQOSQUEUESCHEDULER:
			pMsg = new QosClientCreateQosQueueSchedulerMessage();
			break;
		case QOSCLIENTDELETEQOSQUEUESCHEDULER:
			pMsg = new QosClientDeleteQosQueueSchedulerMessage();
			break;
		case QOSCLIENTCREATECEEMAP:
			pMsg = new QosClientCreateCeeMapMessage();
			break;
		case QOSCLIENTDELETECEEMAP:
			pMsg = new QosClientDeleteCeeMapMessage();
            break;
        case QOSCLIENTCREATECEEREMAP:
            pMsg = new QosClientCreateCeeRemapMessage();
            break;
		case QOSCLIENTCREATEPRIORITYGROUP:
			pMsg = new QosClientCreatePriorityGroupMessage();
			break;
		case QOSCLIENTDELETEPRIORITYGROUP:
            pMsg = new QosClientDeletePriorityGroupMessage();
            break;
		case QOSCLIENTCREATEPRIORITYMAP:
			pMsg = new QosClientCreatePriorityMapMessage();
			break;
		case QOSCLIENTDELETEPRIORITYMAP:
            pMsg = new QosClientDeletePriorityMapMessage();
            break;
		case QOSCLIENTCREATEDSCPTODSCPMUTATIONMAP:
			pMsg = new QosClientCreateDscpToDscpMutationMapMessage();
			break;
		case QOSCLIENTDELETEDSCPTODSCPMUTATIONMAP:
			pMsg = new QosClientDeleteDscpToDscpMutationMapMessage();
			break;
		case QOSCLIENTCREATEDSCPTOTRAFFICCLASSMAP:
			pMsg = new QosClientCreateDscpToTrafficClassMapMessage();
			break;
		case QOSCLIENTDELETEDSCPTOTRAFFICCLASSMAP:
			pMsg = new QosClientDeleteDscpToTrafficClassMapMessage();
			break;
		case QOSCLIENTCREATEDSCPTOCOSMAP:
			pMsg = new QosClientCreateDscpToCosMapMessage();
			break;
		case QOSCLIENTDELETEDSCPTOCOSMAP:
			pMsg = new QosClientDeleteDscpToCosMapMessage();
			break;
		case QOSCLIENTCREATEDSCPMARKLIST:
			pMsg = new QosClientCreateDscpMarkListMessage();
			break;
		case QOSCLIENTDELETEDSCPMARKLIST:
			pMsg = new QosClientDeleteDscpMarkListMessage();
			break;
		case QOSCLIENTCREATEREDPROFILE:
			pMsg = new QosClientCreateRedProfileMessage();
			break;
		case QOSCLIENTDELETEREDPROFILE:
			pMsg = new QosClientDeleteRedProfileMessage();
			break;
		case QOSSHOWMESSAGE:
			pMsg = new QosShowMessage();
			break;
		case QOSCLIENTCLEAR:
			pMsg = new QosClientClearMessage();
			break;
		case QOSCLIENTCONFIGREPLAYDSCPCOSMAP:
			pMsg = new QosClientConfigReplayDscpCosMapMessage();
			break;
		case QOSCLIENTCONFIGREPLAYDSCPMUTATIONMAP:
			pMsg = new QosClientConfigReplayDscpMutationMapMessage();
			break;
		case QOSCLIENTCONFIGREPLAYDSCPTCMAP:
			pMsg = new QosClientConfigReplayDscpTcMapMessage();
			break;
		/* APPM */
		case APPMPROFILE:
			pMsg = new APPMProfileMessage();
			break;
		case APPMSECPROFILE:
			pMsg = new APPMSecProfileMessage();
			break;
		case APPMQOSPROFILE:
			pMsg = new APPMQosProfileMessage();
			break;

        case APPMFCOEPROFILE:
            pMsg =  new APPMFcoeProfileMessage();
            break;
			
        case SSMMACIPACLCLEAR:
            pMsg = new AclClearStatsMessage();
            break;
		case BPRATELIMITCLIENTCONFIG:
			pMsg = new BpratelimitClientConfigMessage();
			break;

        case SSMDEBUG:
        	pMsg = new GenericClientMessage<SSMDEBUG>();
        	break;

        case SSMNODEBUG:
        	pMsg = new GenericClientMessage<SSMNODEBUG>();
        	break;

        case SSMSHOWDEBUG:
        	pMsg = new GenericClientMessage<SSMSHOWDEBUG>();
			break;

        case SSMSHOWL2ACL:
        	pMsg = new GenericClientMessage<SSMSHOWL2ACL>();
			break;

        case SSMSHOWINTERFACE:
        	pMsg = new GenericClientMessage<SSMSHOWINTERFACE>();
			break;

        case SSMSHOWPORT:
        	pMsg = new GenericClientMessage<SSMSHOWPORT>();
			break;

        case SSMSHOWAPPTYPE:
        	pMsg = new GenericClientMessage<SSMSHOWAPPTYPE>();
			break;

        case SSMSHOWCTLPKT:
        	pMsg = new GenericClientMessage<SSMSHOWCTLPKT>();
			break;
        case SSMSHOWDNLD:
        	pMsg = new GenericClientMessage<SSMSHOWDNLD>();
			break;

        case SSMSHOWMSGQ:
        	pMsg = new GenericClientMessage<SSMSHOWMSGQ>();
			break;

        case SSMSHOWL2SYSTCAM:
        	pMsg = new GenericClientMessage<SSMSHOWL2SYSTCAM>();
			break;

		case SSMSHOWL2CTRL:
        	pMsg = new GenericClientMessage<SSMSHOWL2CTRL>();
			break;

		case SSMSHOWL3CTRL:
        	pMsg = new GenericClientMessage<SSMSHOWL3CTRL>();
			break;

        case NSMPHYINTF:
            pMsg = new NsmPhyIntfMessage();
            break;

		case MSTPPORTCONFIG:
			pMsg = new MstpPortConfigMessage();
			break;
			
		case MSTP_SHOW:
		    pMsg = new MstpShowMessage();
		    break;
		

		  // IP policy
		case SSM_CLIENT_ADD_ROUTEMAP:
		case SSM_CLIENT_DEL_ROUTEMAP:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_NH:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_ADDR:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_METRIC:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TYPE:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TAG:
		case SSM_CLIENT_SET_ROUTEMAP_SET_DISTANCE:
		case SSM_CLIENT_SET_ROUTEMAP_SET_METRIC:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TYPE:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TAG:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH_PEER_ADDR:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_VRF_NH:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_GLOBAL_NH:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_WEIGHT:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_RS_PREFIX_LIST:
		case SSM_CLIENT_ADD_IPPREFIX:
		case SSM_CLIENT_DEL_IPPREFIX:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_ASPATH:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ASPATH_PREPEND:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ASPATH_TAG:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_AUTOMATIC_TAG:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_COMMUNITY_ACCESS_LIST:
		case SSM_CLIENT_ADD_ASPATHACESSLIST:
		case SSM_CLIENT_DEL_ASPATHACESSLIST:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_LOCAL_PREFERENCE:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ORIGIN:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_BGP_PROTOCOL_ROUTE_TYPE:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_COMMUNITY_LIST:
		case SSM_CLIENT_ADD_IPCOMMUNITYSTDACESSLIST:
		case SSM_CLIENT_DEL_IPCOMMUNITYSTDACESSLIST:
		case SSM_CLIENT_ADD_IPCOMMUNITYEXTACESSLIST:
		case SSM_CLIENT_DEL_IPCOMMUNITYEXTACESSLIST:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_DAMPENING:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE:
		case SSM_CLIENT_SET_ROUTEMAP_CONTINUE:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NEXT_HOP_ADDRESS:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_NEXT_HOP_FILTER:
		case SSM_CLIENT_ADD_FILTERCHANGEUPDATEDELAY:
		case SSM_CLIENT_DEL_FILTERCHANGEUPDATEDELAY:
		case SSM_CLIENT_CLEAR_FILTERCHANGEUPDATEDELAY:
		case SSM_CLIENT_SET_ROUTEMAP_MATCH_IPACL:
		case SSM_CLIENT_SET_ROUTEMAP_SET_INTERFACE:
		case SSM_CLIENT_SET_ROUTEMAP_SET_DSCP:	
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_COMMUNITY_EXPR:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_EXTCOMMUNITY_EXPR:
		case SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_EXTCOMMUNITY_SOO_EXPR:
		case SSM_CLIENT_ADD_IPCOMMUNITYSTDACESSLIST_WITHOUT_SEQ:
		case SSM_CLIENT_ADD_ASPATHACESSLIST_WITHOUT_SEQ:
		case SSM_CLIENT_SET_ROUTEMAP_REMOVE_INTERFACE:
		case SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE:

		  pMsg = new GenericClientMessage_(operationCode);
		  break;
		
		default:
			break;
	}

	return (pMsg);
}


int setSSMMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (SsmClientMsgInstantiator);
     return status;
}


}
