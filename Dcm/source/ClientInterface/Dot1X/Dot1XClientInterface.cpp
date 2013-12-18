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

#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "ClientInterface/Dot1X/NsmClientDot1xUpdatePortConfigMessage.h"
#include "ClientInterface/Nsm/NsmClientUpdateLacpConfigMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Dot1X/Dot1XShowMessage.h"
#include "ClientInterface/Dot1X/Dot1XClearMessage.h"
#include "ClientInterface/Dot1X/Dot1XDebugCommand.h"
#include "ClientInterface/Dot1X/Dot1XReauthenticateMessage.h"
#include "ClientInterface/Dot1X/Dot1XTestMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Dot1X/Dot1XRadiusMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *Dot1XClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	trace (TRACE_LEVEL_DEBUG, "Entering Dot1XClientMsgInstantiator:: OPERATION:- " + operationCode);

	switch (operationCode) {
		case DOT1X_SHOW_ALL:
			pMsg = new Dot1XShowMessage();
			break;
        case DOT1X_REAUTHENTICATE_ALL:
            pMsg = new Dot1XReauthenticateMessage();
            break;
        case DOT1X_TEST_ALL:
            pMsg = new Dot1XTestMessage();
            break;
        case DOT1X_RADIUS_MSG:
            pMsg = new Dot1XRadiusMessage();
            break;
		case DOT1X_CLEAR_ALL:
			pMsg = new Dot1XClearMessage();
			break;
		case DOT1X_DEBUG_ALL:
			pMsg = new Dot1XDebugMessage();
			break;
		case DOT1X_CLIENT_CONFIG:
			pMsg = new NsmClientDot1xUpdatePortConfigMessage();
			break;
		case NSMGLOBALLACPCONFIG:
			pMsg = new NsmClientUpdateLacpConfigMessage();
			break;
		default:
			break;
	}

	return (pMsg);
}


int setDot1XMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (Dot1XClientMsgInstantiator);
     return status;
}

}
