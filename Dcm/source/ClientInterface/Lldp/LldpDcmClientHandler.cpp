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
 *   Author : Sanjeev Joshi                                                *
 ***************************************************************************/

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Lldp/LldpClientDebugMessage.h"
#include "ClientInterface/Lldp/LldpShowMessage.h"
#include "ClientInterface/Lldp/LldpClearMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/LldpClientScalarsConfSetMessage.h"
#include "ClientInterface/Lldp/LldpClientProfileConfSetMessage.h"
#include "ClientInterface/Lldp/NsmLldpClientIntfConfSetMessage.h"
#include "ClientInterface/Lldp/LldpFcoePriorityConfMessage.h"
#include "ClientInterface/Lldp/CdpClientConfSetMessage.h"
#include "ClientInterface/Lldp/LldpFcoeClientFsbMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage*
LldpDcmClientMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case MSG_LLDP_SHOW:
            pDcmManagementInterfaceMessage =  new LldpShowMessage();
            break;
        
		case MSG_LLDP_SCALARS_GLOBAL_CONF:
            pDcmManagementInterfaceMessage =  new LldpClientScalarsConfSetMessage();
            break;
		
		case MSG_LLDP_PROFILE_GLOBAL_CONF:
            pDcmManagementInterfaceMessage =  new LldpClientProfileConfSetMessage();
            break;
		
		case MSG_LLDP_PHY_INTF_CONF:
            pDcmManagementInterfaceMessage =  new NsmLldpClientIntfConfSetMessage();
            break;

        case MSG_LLDP_FCOE_PRIORITY_CONF:
            pDcmManagementInterfaceMessage = new LldpFcoePriorityConfMessage();
            break;

        case MSG_LLDP_CLEAR:
            pDcmManagementInterfaceMessage =  new LldpClearMessage();
            break;
        
        case MSG_LLDP_DEBUG:
            pDcmManagementInterfaceMessage =  new LldpClientDebugMessage();
            break;

        case MSG_CDP_GLOBAL_CONF:
            pDcmManagementInterfaceMessage = new CdpClientConfSetMessage();
            break;

		case MSG_LLDP_FSB_MODE:
			pDcmManagementInterfaceMessage =  new LldpFcoeClientFsbMessage();
			break;

		default:
            trace (TRACE_LEVEL_INFO, string 
                    ("LldpDcmClientMsgInstantiator : "
                     "Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

int 
setLldpDcmClientMsgHandler ()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient(LldpDcmClientMsgInstantiator);
     return status;
}

}
