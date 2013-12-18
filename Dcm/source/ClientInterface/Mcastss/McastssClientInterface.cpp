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
 *   Author : Johnny Yang                                    *
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
#include "WyserEaGateway/GenericClientMessage.h"


using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *McastssClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
        case MCASTSS_SHOW:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW>();
			break;
        case MCASTSS_SHOW_INTERFACES:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_INTERFACES>();
			break;
        case MCASTSS_SHOW_VLANS:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_VLANS>();
			break;
        case MCASTSS_SHOW_MROUTER:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_MROUTER>();
			break;
        case MCASTSS_SHOW_VLANMAC:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_VLANMAC>();
			break;
        case MCASTSS_SHOW_DBGHA:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_DBGHA>();
			break;
        case MCASTSS_SHOW_TESTHA:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_TESTHA>();
			break;
        case MCASTSS_SHOW_VERBOSE:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_VERBOSE>();
			break;
        case MCASTSS_SHOW_GLOBALS:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_GLOBALS>();
			break;
        case MCASTSS_SHOW_PORTTREE:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_PORTTREE>();
			break;
        case MCASTSS_SHOW_VLANTREE:
        	pMsg = new GenericClientMessage<MCASTSS_SHOW_VLANTREE>();
			break;
        case MCASTSS_DEBUG:
        	pMsg = new GenericClientMessage<MCASTSS_DEBUG>();
			break;
        case MCASTSS_NODEBUG:
        	pMsg = new GenericClientMessage<MCASTSS_NODEBUG>();
			break;
		case MCASTSS_SHOW_VIFS:
			pMsg = new GenericClientMessage<MCASTSS_SHOW_VIFS>();
            break;
        case MCASTSS_SHOW_MRT:
            pMsg = new GenericClientMessage<MCASTSS_SHOW_MRT>();
            break;
        case MCASTSS_SHOW_RP:
            pMsg = new GenericClientMessage<MCASTSS_SHOW_RP>();
            break;
        case MCASTSS_SHOW_GRANGE:
            pMsg = new GenericClientMessage<MCASTSS_SHOW_GRANGE>();
            break;
        case MCASTSS_SHOW_RTE:
            pMsg = new GenericClientMessage<MCASTSS_SHOW_RTE>();
            break;
        case MCASTSS_SHOW_RTE_CLIENT:
            pMsg = new GenericClientMessage<MCASTSS_SHOW_RTE_CLIENT>();
            break;
        case MCASTSS_SHOW_RTE_TEST:
            pMsg = new GenericClientMessage<MCASTSS_SHOW_RTE_TEST>();
            break;
        case MCASTSS_SHOW_RP_TEST:
            pMsg = new GenericClientMessage<MCASTSS_SHOW_RP_TEST>();
            break;
		default:
			break;
	}

	return (pMsg);
}


int setMcastssMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (McastssClientMsgInstantiator);
     return status;
}


}
