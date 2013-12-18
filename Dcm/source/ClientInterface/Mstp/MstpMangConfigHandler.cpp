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
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "WyserEaGateway/GenericClientMessage.h"


#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpShowMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Mstp/MstpMangConfigHandler.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpPortVlanConfigMessage.h"
#include "ClientInterface/Mstp/MstpVlanBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpInstanceConfigMessage.h"
#include "ClientInterface/Mstp/MstpClearMessage.h"
#include "ClientInterface/Mstp/MstpVlanConfigMessage.h"
#include "ClientInterface/Mstp/MstpPortIdConfigMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

ManagementInterfaceMessage *MstpClientMangMsgIntantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode)
    {
        case MSTPBRIDGECONFIG:
            pDcmManagementInterfaceMessage =  new MstpBridgeConfigMessage ();
            break;
        case MSTPVLANBRIDGECONFIG:
            pDcmManagementInterfaceMessage =  new MstpVlanBridgeConfigMessage ();
            break;
        case MSTPPORTCONFIG:
            pDcmManagementInterfaceMessage =  new MstpPortConfigMessage ();
            break;
        case MSTPPORTINSTCONFIG:
            pDcmManagementInterfaceMessage =  new MstpPortConfigMessage ();
            break;
        case MSTPPORTVLANCONFIG:
            pDcmManagementInterfaceMessage =  new MstpPortVlanConfigMessage ();
            break;
        case MSTPVLANCONFIG:
            pDcmManagementInterfaceMessage =  new MstpVlanConfigMessage ();
            break;
        case MSTPBRIDGEINSTCONFIG:
            pDcmManagementInterfaceMessage =  new MstpInstanceConfigMessage ();
            break;
        case MSTP_SHOW:
            pDcmManagementInterfaceMessage = new MstpShowMessage();
            break;
        case MSTPCLEAR:
            pDcmManagementInterfaceMessage = new MstpClearMessage();
            break;
        case MSTPSHOWGENERIC:
            pDcmManagementInterfaceMessage = new GenericClientMessage<MSTPSHOWGENERIC>();
            break;
        case MSTP_PORT_ID_CONFIG:
            pDcmManagementInterfaceMessage = new MstpPortIdConfigMessage();
            break;
        default:
            trace (TRACE_LEVEL_INFO, string ("MstpClientMangMsgIntantiator : Unknown Operation Code : ") + operationCode);
            pDcmManagementInterfaceMessage = NULL;
            break;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

int setMstpMangConfigHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (MstpClientMangMsgIntantiator);
     return status;
}

}
