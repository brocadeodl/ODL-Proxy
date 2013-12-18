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
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/L3sys/L3ssMessageDef.h"
#include "ClientInterface/L3sys/L3ssShowMessage.h"
#include "WyserEaGateway/GenericClientMessage.h"
#include "ClientInterface/L3sys/L3ssClearMessage.h"
#include "ClientInterface/L3sys/L3ssArpShowMessage.h"
#include "ClientInterface/L3sys/L3ssDebugLpmMessage.h"



using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *L3ssClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
		case L3SSSHOW:
                        pMsg = new L3ssShowMessage();
			break;
		case L3SS_DEBUG_LPM:
			pMsg = new L3ssDebugLpmMessage();
			break;
		case L3SS_DEBUG_NEXT_HOP:
			pMsg = new GenericClientMessage<L3SS_DEBUG_NEXT_HOP>();
			break;
		case L3SSCLEAR:
			pMsg = new L3ssClearMessage();
			break;
		case L3SSARPSHOW:
			pMsg = new L3ssArpShowMessage();
			break;

		default:
			break;
	}

	return (pMsg);
}


int setL3SSMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (L3ssClientMsgInstantiator);
     return status;
}


}
