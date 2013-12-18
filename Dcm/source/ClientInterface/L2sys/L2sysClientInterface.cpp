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
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "ClientInterface/L2sys/L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "ClientInterface/L2sys/L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "ClientInterface/L2sys/L2sysShowMessage.h"
#include "ClientInterface/L2sys/L2sysClearMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/DceDbg/DceDbgMsg.h"
#include "ClientInterface/Nsm/PortSecurityClientMessage.h"
#include "ClientInterface/L2sys/L2sysPortSecShowMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *L2sysClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	trace (TRACE_LEVEL_DEBUG, "Entering L2sysClientMsgInstantiator:: OPERATION:- " + operationCode);

	switch (operationCode) {
		case L2SYS_SHOW_MAC:
			pMsg = new L2sysShowMessage();
			break;
		case L2SYS_CLEAR_MAC:
			pMsg = new L2sysClearMessage();
			break;
		case L2SYSCLIENTGLOBALUPDATEL2SYSGLOBALSTATICMAC:
			pMsg = new L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage();
			break;
		case DCE_DBG_MSG:
			pMsg = new DceDbgMsg();
			break;
		case L2SYS_PORT_SECURITY_MSG:
			pMsg = new PortSecurityClientMessage();
			break;
		case L2SYS_PORT_SECURITY_SHOW_MSG:
			pMsg = new L2sysPortSecShowMessage();
			break;
		default:
			break;
	}

	return (pMsg);
}


int setL2sysMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (L2sysClientMsgInstantiator);
     return status;
}

}
