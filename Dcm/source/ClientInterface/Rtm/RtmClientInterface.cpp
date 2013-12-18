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
 *   Author :                                                              *
 ***************************************************************************/

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/DceWaveClientMessage.h"
#include "WyserEaGateway/GenericClientMessage.h"

#include "Rtm/Local/RtmTypes.h"
#include "RtmMessageDef.h"
#include "RtmClientConfigMessage.h"
#include "RtmClientStaticRouteMessage.h"
#include "RtmShowMessage.h"

#include "DcmCStatus.h"
#include "WaveCStatus.h"
#include "DcmResourceIdEnums.h"
#include "rtm_util.h"
#include "ClientInterface/Nsm/NsmClientConfigVRFMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *RtmClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
	case RTMDEBUG:
     	pMsg = new GenericClientMessage<RTMDEBUG>();
       	break;

	case RTMNODEBUG:
      	pMsg = new GenericClientMessage<RTMNODEBUG>();
       	break;

    case RTMSHOW:
       	pMsg = new RtmShowMessage();
		break;

    case RTMCLEAR:
       	pMsg = new GenericClientMessage<RTMCLEAR>();
		break;

	case RTMRTMCONFIG:
		pMsg = new RtmClientConfigMessage();
		break;

	case RTMSTATICROUTE:
		pMsg = new RtmClientStaticRouteMessage();
        break;

    case MSG_VRF_INT:
        pMsg = new NsmClientConfigVRFMessage();
        break;
	default:
		break;
	}

	return (pMsg);
}


int setRtmMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (RtmClientMsgInstantiator);
     return status;
}

ResourceId mapRtmBackEndErrorsToDcmdErrors(int err)
{

	switch (err) {
	case (CLI_RTM_SUCCESS):
		return WAVE_MESSAGE_SUCCESS;

	case (CLI_STATIC_NON_CONT_SUBNETMASK):
		return WRC_RTM_ERR_STATIC_NON_CONT_SUBNETMASK;

	case (CLI_STATIC_INCONSISTENT_SUBNETMASK):
		return WRC_RTM_ERR_STATIC_INCONSISTENT_SUBNETMASK;

	case (CLI_STATIC_MULTICAST_ADDRESS):
		return WRC_RTM_ERR_STATIC_MULTICAST_ADDRESS;

	case (CLI_STATIC_INVLD_PORT):
		return WRC_RTM_ERR_STATIC_INVLD_PORT;

	case (CLI_STATIC_OIF_NOT_EXIST):
		return WRC_RTM_ERR_STATIC_OIF_NOT_EXIST;

	case (CLI_STATIC_DUPLICATE_ROUTE):
		return WRC_RTM_ERR_STATIC_DUPLICATE_ROUTE;

	case (CLI_STATIC_INVLD_ROUTE):
		return WRC_RTM_ERR_STATIC_INVLD_ROUTE;

	case (CLI_STATIC_ONE_PATH_ALLOWED):
		return WRC_RTM_ERR_STATIC_ONE_PATH_ALLOWED;

	case (CLI_STATIC_INVLD_NHOP):
		return WRC_RTM_ERR_STATIC_INVLD_NHOP;

	case (CLI_STATIC_ZERO_NHOP):
		return WRC_RTM_ERR_STATIC_ZERO_NHOP;

	case (CLI_ROUTE_NOT_FOUND):
		return WRC_RTM_ERR_ROUTE_NOT_FOUND;

	case (CLI_STATIC_INT_DEF_NOT_ALLOWED):
		return WRC_RTM_ERR_STATIC_INT_DEF_NOT_ALLOWED;

	case (CLI_STATIC_BAD_PREFIX):
		return WRC_RTM_ERR_STATIC_BAD_PREFIX;

	case (CLI_STATIC_BAD_NHOP_IP):
		return WRC_RTM_ERR_STATIC_BAD_NHOP_IP;

	case (CLI_STATIC_MAX_ROUTES_REACHED):
		return WRC_RTM_ERR_STATIC_MAX_ROUTES_REACHED;

	case (CLI_RTM_INVALID_ROUTER_ID):
		return WRC_RTM_ERR_INVALID_ROUTER_ID;

	case (CLI_RTM_VRF_RD_AF_NOTSET):
		return WRC_RTM_ERR_VRF_RD_AF_NOTSET;

	case (CLI_RTM_VRF_INVLD_RD):
		return WRC_RTM_ERR_VRF_INVLD_RD;

	case (CLI_STATIC_INVLD_VRF):
	default:
		return WRC_RTM_ERR_UNKNOWN;
	}

    return WAVE_MESSAGE_ERROR;
}
}
