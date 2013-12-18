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
 *   Author : rhkumar/ranantha                                             *
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

#include "ClientInterface/DceWaveClientMessage.h"

#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "ClientInterface/AG/AgClientConfigAllMessage.h"
#include "ClientInterface/AG/AgClientMappingMessage.h"
#include "ClientInterface/AG/AgClientFailoverMessage.h"
#include "ClientInterface/AG/AgClientFailbackMessage.h"
#include "ClientInterface/AG/AgClientShowMessage.h"
#include "ClientInterface/AG/AgClientFnmtovMessage.h"
#include "ClientInterface/AG/AgClientRelCounterMessage.h"
#include "ClientInterface/AG/AgClientPGCreateMessage.h"
#include "ClientInterface/AG/AgClientPGAddMessage.h"
#include "ClientInterface/AG/AgClientPGDelMessage.h"
#include "ClientInterface/AG/AgClientPGModesMessage.h"
#include "ClientInterface/AG/AgClientPolicyMessage.h"
#include "ClientInterface/AG/AgClientPGRenameMessage.h"
#include "ClientInterface/AG/AgClientPGRemoveMessage.h"
#include "ClientInterface/AG/AgClientStaticMappingMessage.h"




using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage *AgClientMsgIntantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *msg_p = NULL;

    trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator Entering.... ");
    switch (operationCode)
    {
        case AG_CLIENTMSG_MODE_ENABLE:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_MODE_ENABLE:.... ");
            msg_p = new AgClientEnableMessage();
            break;

        case AG_CLIENTMSG_CONFIG_ALL:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_CONFIG_ALL:.... ");
            msg_p = new AgClientConfigAllMessage();
            break;

        case AG_CLIENTMSG_MAPPING:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_MAPPING:.... ");
            msg_p = new AgClientMappingMessage();
            break;

       case AG_CLIENTMSG_FAILOVER:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_FAILOVER:.... ");
            msg_p= new AgClientFailoverMessage();
            break;

       case AG_CLIENTMSG_FAILBACK:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_FAILBACK:.... ");
            msg_p= new AgClientFailbackMessage();
            break;

       case AG_CLIENTMSG_FNMTOV:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_FNMTOV:.... ");
            msg_p= new AgClientFnmtovMessage();
            break;

       case AG_CLIENTMSG_RELCOUNTER:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_RELCOUNTER:.... ");
            msg_p= new AgClientRelCounterMessage();
            break;

        case AG_CLIENTMSG_SHOW_AG:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_SHOW_AG:.... ");
            msg_p = new AgClientShowMessage();
            break;

        case AG_CLIENTMSG_PGCREATE:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_PGCREATE:.... ");
            msg_p = new AgClientPGCreateMessage();
            break;

        case AG_CLIENTMSG_PGADD:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_PGADD:.... ");
            msg_p = new AgClientPGAddMessage();
            break;

        case AG_CLIENTMSG_PGDEL:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_PGDEL:.... ");
            msg_p = new AgClientPGDelMessage();
            break;

        case AG_CLIENTMSG_PGMODES:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_PGMODES:.... ");
            msg_p = new AgClientPGModesMessage();
            break;

        case AG_CLIENTMSG_POLICY:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_POLICY:.... ");
            msg_p = new AgClientPolicyMessage();
            break;

        case AG_CLIENTMSG_PGRENAME:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_PGRENAME:.... ");
            msg_p = new AgClientPGRenameMessage();
            break;

        case AG_CLIENTMSG_PGREMOVE:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_PGREMOVE:.... ");
            msg_p = new AgClientPGRemoveMessage();
            break;

        case AG_CLIENTMSG_STATIC_MAPPING:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgIntantiator AG_CLIENTMSG_PGREMOVE:.... ");
            msg_p = new AgClientStaticMappingMessage();
            break;

        default:
            trace(TRACE_LEVEL_DEBUG, "AgClientMsgInstantiator Invalid operation code");
            msg_p = NULL;
    }                                                                                                                
    return (msg_p);
}

int setAgConfigHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (AgClientMsgIntantiator);
     return status;
}

}
