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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Rahul Jain                                                   *
 ***************************************************************************/

#include <iostream>
#include <strings.h>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/WaveCliShell.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Pim/PimRtrCfgClientMsg.h"
#include "ClientInterface/Pim/PimIpMultCfgClientMsg.h"
#include "ClientInterface/Pim/PimBsrCandClientMsg.h"
#include "ClientInterface/Pim/PimIntfCfgClientMsg.h"
#include "ClientInterface/Pim/PimShowNeighborMessage.h"
#include "ClientInterface/Pim/PimShowSparseMessage.h"
#include "ClientInterface/Pim/PimStaticRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimCandRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimCandRpPrefixCfgClientMsg.h"
#include "ClientInterface/Pim/PimShowMcacheMsg.h"
#include "WyserEaGateway/GenericClientMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *PimClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
        /* Add one case statement for each show command ID. For example
         *     case MCASTSS_SHOW:
         *         pMsg = new GenericClientMessage<MCASTSS_SHOW>();
         */ 
        /* Case statements for show, debug and clear commands */
        case PIM_SHOW_VRF_CMDS:
             pMsg = new GenericClientMessage<PIM_SHOW_VRF_CMDS>();
             break;
        case PIM_CLEAR_VRF_CMDS:
             pMsg = new GenericClientMessage<PIM_CLEAR_VRF_CMDS>();
             break;
        case PIM_SHOW_RP_CAND:
             pMsg = new GenericClientMessage<PIM_SHOW_RP_CAND>();
             break;
        case PIM_SHOW_RP_MAP:
             pMsg = new GenericClientMessage<PIM_SHOW_RP_MAP>();
             break;
        case PIM_SHOW_RP_HASH:
             pMsg = new GenericClientMessage<PIM_SHOW_RP_HASH>();
             break;
        case PIM_SHOW_RP_SET:
             pMsg = new GenericClientMessage<PIM_SHOW_RP_SET>();
             break;
        case PIM_SHOW_RPF:
             pMsg = new GenericClientMessage<PIM_SHOW_RPF>();
             break;
        case PIM_SHOW_PORT_ALL:
             pMsg = new GenericClientMessage<PIM_SHOW_PORT_ALL>();
             break;
        case PIM_CLEAR_RP_MAP:
             pMsg = new GenericClientMessage<PIM_CLEAR_RP_MAP>();
             break;
        case PIM_SHOW_NEIGHBOR:
             pMsg = new PimShowNeighborMessage();
             break;        
        case PIM_SHOW_SPARSE:
             pMsg = new PimShowSparseMessage();
             break;        
		case PIM_SHOW_GROUP:
		     pMsg = new GenericClientMessage<PIM_SHOW_GROUP>();
		     break;			 			 
        case PIM_SHOW_BSR:
		     pMsg = new GenericClientMessage<PIM_SHOW_BSR>();
		     break;			 
        case PIM_SHOW_MCACHE:
             pMsg = new PimShowMcacheMsg();
		     break;			 
        case PIM_SHOW_TRAFFIC:
		     pMsg = new GenericClientMessage<PIM_SHOW_TRAFFIC>();
		     break;			 
        case PIM_SHOW_VLAN_ALL:
		     pMsg = new GenericClientMessage<PIM_SHOW_VLAN_ALL>();
		     break;			 
        case PIM_CLEAR_MCACHE:
		     pMsg = new GenericClientMessage<PIM_CLEAR_MCACHE>();
             break;			 
        case PIM_CLEAR_TRAFFIC:
		     pMsg = new GenericClientMessage<PIM_CLEAR_TRAFFIC>();
		     break;
        case PIM_DEBUG_CMD:
		     pMsg = new GenericClientMessage<PIM_DEBUG_CMD>();
		     break;
        case PIM_NO_DEBUG_CMD:
		     pMsg = new GenericClientMessage<PIM_NO_DEBUG_CMD>();
		     break;
		case PIM_SHOW_DEBUG:
             pMsg = new GenericClientMessage<PIM_SHOW_DEBUG>();
             break;

        /* Case statements for config commands */
        case PIM_CFG_IP_MULT:
            pMsg = new PimIpMultCfgClientMsg();
            break; 
        case PIM_CFG_RTR_PIM:
            pMsg = new PimRtrCfgClientMsg();
            break; 
        case PIM_BSR_CANDIDATE:
            pMsg = new PimBsrCandClientMsg();
            break; 
        case PIM_CFG_STATIC_RP:
            pMsg = new PimStaticRpCfgClientMsg();
            break;
        case PIM_CFG_CAND_RP:
            pMsg = new PimCandRpCfgClientMsg();
            break;
		case PIM_CFG_CAND_RP_PREFIX:
			pMsg = new PimCandRpPrefixCfgClientMsg();
			break;
        case PIM_CFG_MAX_MCACHE:
            break; 
        case PIM_CFG_HELLO_TIMER:
            break; 
        case PIM_CFG_NBR_TO:
            break; 
        case PIM_CFG_INACT_TIMER:
            break; 
        case PIM_CFG_MSG_INTV:
            break; 
        case PIM_CFG_SPT_THOLD:
            break;
        case PIM_INTF_CFG_PIM_SP:
            pMsg = new PimIntfCfgClientMsg();
            break;
        case PIM_CFG_GLBL_REPLAY:
            pMsg = new PimRtrCfgClientMsg(PIM_CFG_GLBL_REPLAY);
            break; 
		default:
			break;
	}

	return (pMsg);
}


int setPimMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (PimClientMsgInstantiator);
     return status;
}


}
