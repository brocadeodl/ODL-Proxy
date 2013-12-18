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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi 											   *
 ***************************************************************************/

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/Dhcp/DhcpRelClientClearMessage.h"
#include "ClientInterface/Dhcp/DhcpRelClientShowMessage.h"
#include "ClientInterface/Dhcp/DhcpRelIntfClientConfigMessage.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"

#include "ClientInterface/DceWaveClientMessage.h"

#include "WyserEaGateway/GenericClientMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage *DhcpRelClientMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case IPHELPCONFIG:
            pDcmManagementInterfaceMessage =  new DhcpRelIntfClientConfigMessage();
            break;
    	case IPHELPSHOW:
	    	pDcmManagementInterfaceMessage = new DhcpRelClientShowMessage();
	    	break;
    	case IPHELPCLEAR:
	    	pDcmManagementInterfaceMessage = new DhcpRelClientClearMessage();
			break;
        default:
            trace (TRACE_LEVEL_INFO, string ("DhcpRelClientConfigMsgInstantiator : Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

int setIPHELPERMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (DhcpRelClientMsgInstantiator);
     return status;
}

}
