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
 *   Author : Manju Mahajan													*
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


#include "TrillOam/Common/TrillOamPluginTypes.h"
#include "ClientInterface/TrillOam/TrillOamMangConfigHandler.h"
#include "ClientInterface/TrillOam/TrillOamPingMessage.h"
#include "ClientInterface/TrillOam/TrillOamTraceRouteMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage *TrillOamClientMangMsgIntantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case TRILLOAMPING:
            pDcmManagementInterfaceMessage =  new TrillOamPingMessage ();
            break;
        case TRILLOAMTRACEROUTE:
            pDcmManagementInterfaceMessage =  new TrillOamTraceRouteMessage ();
            break;
        default:
            trace (TRACE_LEVEL_INFO, string ("TrillOamClientMangMsgIntantiator : Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

int setTrillOamMangConfigHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (TrillOamClientMangMsgIntantiator);
     return status;
}

}
