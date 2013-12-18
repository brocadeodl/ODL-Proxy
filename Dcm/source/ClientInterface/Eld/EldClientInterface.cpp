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
 *   Author : Amitayu Das                                                  *
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

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/DceDbg/DceDbgMsg.h"
#include "ClientInterface/Eld/EldClientUpdateRxlimConfigMessage.h"
#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "ClientInterface/Eld/EldShowMessage.h"
#include "ClientInterface/Eld/EldClearMessage.h"
#include "ClientInterface/Eld/EldDebugMessage.h"
#include "ClientInterface/Eld/EldPoIntfMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmPoIntfMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *EldClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	trace (TRACE_LEVEL_DEBUG, "Entering EldClientMsgInstantiator:: OPERATION:- " + operationCode);

	switch (operationCode) {
		case MSG_ELD_SHOW_INTERFACE_VLAN:
			pMsg = new EldShowMessage();
			break;
		case MSG_ELD_INTF_CONF:
			pMsg = new EldClientUpdatePhyIntfMessage();
			break;
		case MSG_ELD_CLEAR:
			pMsg = new EldClearMessage();
			break;
		case MSG_ELD_DEBUG:
			pMsg = new EldDebugMessage();
			break;
		case MSG_ELD_SCALARS_GLOBAL_CONF:
			pMsg = new EldClientUpdateRxlimConfigMessage();
			break;
        case MSG_ELD_INTF_PO_CONF:
            pMsg =  new EldPoIntfMessage();
            break;

		default:
			break;
	}

	return (pMsg);
}


int setEldMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (EldClientMsgInstantiator); 
     return status;
}

}
