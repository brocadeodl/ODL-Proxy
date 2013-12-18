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
 *   Author : Kunjan Kumar Verma                                           *
 ***************************************************************************/

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/OSPF/OSPFRouterConfigMessage.h"
#include "ClientInterface/OSPF/OSPFPermitDenyRedistributeMessage.h"
#include "ClientInterface/OSPF/OSPFInterfaceConfigMessage.h"
#include "ClientInterface/OSPF/OSPFShowMessage.h"
#include "ClientInterface/OSPF/OSPFClearMessage.h"
#include "ClientInterface/OSPF/OSPFDebugMessage.h"
#include "ClientInterface/OSPF/OSPFAreaMessage.h"
#include "ClientInterface/OSPF/OSPFAreaRangeMessage.h"

#include "ClientInterface/DceDbg/DceDbgMsg.h"

#include "ClientInterface/DceWaveClientMessage.h"

#include "WyserEaGateway/GenericClientMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage *OSPFClientRouterConfigMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case OSPFROUTERCONFIG:
            pDcmManagementInterfaceMessage =  new OSPFRouterConfigMessage();
            break;
	    case OSPFSHOW:
			pDcmManagementInterfaceMessage = new OSPFShowMessage();
			break;
	    case OSPFAREA:
			pDcmManagementInterfaceMessage = new OSPFAreaMessage();
			break;
	    case OSPFAREARANGE:
			pDcmManagementInterfaceMessage = new OSPFAreaRangeMessage();
			break;
        case OSPFCLEAR:
            pDcmManagementInterfaceMessage = new OSPFClearMessage();
            break;
        case OSPFDEBUG:
            pDcmManagementInterfaceMessage = new OSPFDebugMessage();
            break;
        case OSPFINTERFACECONFIG:
            pDcmManagementInterfaceMessage =  new OSPFInterfaceConfigMessage();
			break;
		case OSPFPERMITDENYCONFIG:
			pDcmManagementInterfaceMessage = new OSPFPermitDenyRedistributeMessage();
			break;
        case OSPFDEBUGDEV:    
            pDcmManagementInterfaceMessage = new GenericClientMessage<OSPFDEBUGDEV>();
            break;
        default:
            trace (TRACE_LEVEL_INFO, string ("OSPFClientRouterConfigMsgInstantiator : Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

int setOSPFRouterConfigMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (OSPFClientRouterConfigMsgInstantiator);
     return status;
}

}
