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

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/Ssm/qoscommon.h"
#include "ClientInterface/Ssm/QosClientInterface.h"
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
#include "ClientInterface/Ssm/QosClientClearMessage.h"
#include "ClientInterface/Ssm/QosShowMessage.h"

#include "ClientInterface/APPM/APPMQosProfileMessage.h"
#include "ClientInterface/APPM/APPMSecProfileMessage.h"
#include "ClientInterface/APPM/APPMFcoeProfileMessage.h"
#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"

#include "ClientInterface/Policer/PolicerClientPriorityMapMessage.h"
#include "ClientInterface/Policer/PolicerClientClassMapMessage.h"
#include "ClientInterface/Policer/PolicerClientPolicyClassMapMessage.h"
#include "ClientInterface/Policer/PolicerClientTeMessage.h"
#include "ClientInterface/Policer/PolicerClientTeBulkMessage.h"
#include "ClientInterface/Policer/PolicerShowMessage.h"
#include "ClientInterface/Policer/PolicerClearMessage.h"
#include "ClientInterface/StormControl/NsmStormControlClientIntfConfSetMsg.h"
#include "ClientInterface/StormControl/StormControlClientShowMessage.h"
#include "ClientInterface/StormControl/StormControlClientClearMessage.h"

#include "WyserEaGateway/GenericClientMessage.h"
#include "ClientInterface/Qos/QosClientNasMessage.h"
#include "ClientInterface/Qos/QosClientNasShowMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *QosClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
		case QOSCLIENTCONFIGINTFNSMATTRIBUTES:
			pMsg = new QosClientConfigIntfNsmAttributesMessage();
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
		case QOSCLIENTCONFIGREPLAYDSCPCOSMAP:
			pMsg = new QosClientConfigReplayDscpCosMapMessage();
			break;
		case QOSCLIENTCONFIGREPLAYDSCPMUTATIONMAP:
			pMsg = new QosClientConfigReplayDscpMutationMapMessage();
			break;
		case QOSCLIENTCONFIGREPLAYDSCPTCMAP:
			pMsg = new QosClientConfigReplayDscpTcMapMessage();
			break;
		case QOSCLIENTCREATEQOSRCVQUEUEMULTICASTSETTING:
			pMsg = new QosClientCreateQosRcvQueueMulticastSettingMessage();
			break;
		case QOSCLIENTDELETEQOSRCVQUEUEMULTICASTSETTING:
			pMsg = new QosClientDeleteQosRcvQueueMulticastSettingMessage();
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
		case QOSCLIENTCREATECEEMAP:
			pMsg = new QosClientCreateCeeMapMessage();
			break;
		case QOSCLIENTDELETECEEMAP:
			pMsg = new QosClientDeleteCeeMapMessage();
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
		case QOSCLIENTCREATECEEREMAP:
			pMsg = new QosClientCreateCeeRemapMessage();
			break;
		case APPMQOSPROFILE:
			pMsg = new APPMQosProfileMessage();
			break;
        case APPMFCOEPROFILE:
            pMsg =  new APPMFcoeProfileMessage();
            break;
        case NSMPHYINTF:
            pMsg = new NsmPhyIntfMessage();
            break;
		case QOSCLIENTCLEAR:
			pMsg = new QosClientClearMessage();
			break;
		case POLICERCLIENTPRIORITYMAP:
		  pMsg = new PolicerClientPriorityMapMessage();
		  break;
		case POLICERCLIENTCLASSMAP:
		  pMsg = new PolicerClientClassMapMessage();
		  break;
		case POLICERCLIENTPOLICYCLASSMAP:
		  pMsg = new PolicerClientPolicyClassMapMessage();
		  break;
		case POLICERCLIENTTE:
		  pMsg = new PolicerClientTeMessage();
		  break;
		case POLICERCLIENTTEBULK:
		  pMsg = new PolicerClientTeBulkMessage();
		  break;
		case POLICER_SHOW_ALL:
		  pMsg = new PolicerShowMessage();
		  break;
		case POLICER_CLEAR_ALL:
		  pMsg = new PolicerClearMessage();
		  break;
		case POLICERDEBUG:
			pMsg = new GenericClientMessage<POLICERDEBUG>();
			break;
      	case MSG_STORMCONTROL_PHY_INTF_CONF:
      		pMsg =  new NsmStormControlClientIntfConfSetMsg();
         	break;
		case STORMCONTROLCLIENTCLEAR:
			pMsg =  new StormControlClientClearMessage();
			break;
		case STORMCONTROLCLIENTSHOW:
			pMsg =  new StormControlClientShowMessage();
			break;
		case QOSCLIENTNASCONFIG:
            pMsg =  new QosClientNasMessage();
            break;
		case QOSCLIENTNASSHOW:
            pMsg =  new QosClientNasShowMessage();
            break;
		default:
			break;
	}

	return (pMsg);
}

int setQOSMsgHandlerForClient()
{
	int status = 1;

	WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (QosClientMsgInstantiator);
	return status;
}

} /* extern "C" */
