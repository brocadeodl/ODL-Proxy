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

#include "Fcoe/Common/FcoePluginDebug.h"
#include "ClientInterface/DceWaveClientMessage.h"

#include "ClientInterface/Fcoe/FcoeClientFabricMapMessage.h"
#include "ClientInterface/Fcoe/FcoeClientMapMessage.h"
#include "ClientInterface/Fcoe/FcoeClientFipMessage.h"
#include "ClientInterface/Fcoe/FcoeClientFsbMessage.h"
#include "ClientInterface/Fcoe/FcoeClientIntfMessage.h"
#include "ClientInterface/Fcoe/FcoeClientShowMessage.h"
#include "ClientInterface/Fcoe/FcoeClientClearMessage.h"
#include "ClientInterface/Fcoe/FcoeClientDebugMessage.h"
#include "ClientInterface/Fcoe/FcoeClientPluginControlMessage.h"
#include "ClientInterface/Fcoe/FcoeClientETIntfMessage.h"
#include "ClientInterface/Fcoe/FcoeClientInternalMessage.h"
#include "ClientInterface/Fcoe/FcoeClientEthConfigMessage.h"
#include "VcsFabric/Local/VcsNodeHaMessages.h"
#include "Fcoe/Common/FcoePluginTypes.h"


using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage *FcoeClientMsgIntantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *msg_p = NULL;

    switch (operationCode)
    {
        case FCOE_CLIENT_FABRIC_MAP_CONFIG:
            msg_p = new FcoeClientFabricMapMessage();
            break;

        case FCOE_CLIENT_MAP_CONFIG:
            msg_p = new FcoeClientMapMessage();
            break;

        case FCOE_CLIENT_FIP_CONFIG:
            msg_p = new FcoeClientFipMessage();
            break;

        case FCOE_CLIENT_INTF_CONFIG:
            msg_p = new FcoeClientIntfMessage();
            break;

        case FCOE_CLIENT_SHOW:
            msg_p = new FcoeClientShowMessage();
            break;

        case FCOE_CLIENT_CLEAR:
            msg_p = new FcoeClientClearMessage();
            break;

        case FCOE_CLIENT_DEBUG:
            msg_p = new FcoeClientDebugMessage();
            break;
            
        case FCOE_CLIENT_PLUGIN_CONTROL:
            msg_p = new FcoeClientPluginControlMessage();
            break;

        case FCOE_CLIENT_ET_INTF_CONFIG:
            msg_p = new FcoeClientETIntfMessage();
            break;

        case FCOE_CLIENT_INTERNAL:
            msg_p = new FcoeClientInternalMessage();
            break;

        case FCOE_CLIENT_ETH_INTF_CONFIG:
            msg_p = new FcoeClientEthConfigMessage();
            break;
        
        case FCOE_CP_STATE_CHANGE_MSG:
            msg_p = new VcsNodeCpStateChangeClientMessage (FCOE_CP_STATE_CHANGE_MSG);
            break;
       
		case FCOE_CLIENT_FSB_CONFIG:
			msg_p = new FcoeClientFsbMessage();
			break;
 
        default:
            FCOE_PLUG_DBG_S(string ("Unknown Operation Code : ") + operationCode);
            msg_p = NULL;
            break;
    }                                                                                                                
    return (msg_p);
}

int setFcoeConfigHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (FcoeClientMsgIntantiator);
     return status;
}

}
