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
#include "WyserEaGateway/GenericClientMessage.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "ClientInterface/Arp/ArpClearMessage.h"
#include "ClientInterface/Arp/ArpShowMessage.h"
#include "ClientInterface/Arp/ArpDebugClientMessage.h"
#include "ClientInterface/Arp/ArpConfigClientMessage.h"
#include "ClientInterface/Arp/ArpStaticConfigClientMessage.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"


using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *ArpClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
		case MSG_IP_INT:
			pMsg = new NsmClientCreateIntIpConfigSPMessage();
			break;
		case ARPCLEAR:
			pMsg = new ArpClearMessage();
			break;
		case ARPDEBUGCLIENT:
			pMsg = new ArpDebugClientMessage();
			break;
		case ARPSHOW:
			pMsg = new ArpShowMessage();
            break;
		case ARPCONFIGCLIENT:
			pMsg = new ArpConfigClientMessage();
			break;
		case ARPSTATICCONFIGCLIENT:
			pMsg = new ArpStaticConfigClientMessage();
			break;
		case ARPSHOWIFLIST:
			pMsg = new GenericClientMessage<ARPSHOWIFLIST>();
			break;
		default:
			break;
	}

	return (pMsg);
}


int setARPMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (ArpClientMsgInstantiator);
     return status;
}


}
