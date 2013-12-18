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
 *   Author : Karthik A                                                    *
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
#include "ClientInterface/DceWaveClientMessage.h"

#include "ClientInterface/VRRP/VRRPClientIntfConfigSPMessage.h"
#include "ClientInterface/VRRP/VRRPGlobalConfigSPMessage.h"
#include "ClientInterface/VRRP/VRRPClearStatsMessage.h"
#include "ClientInterface/VRRP/VRRPDebugMessage.h"
#include "ClientInterface/VRRP/VRRPShowMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *VRRPClientMsgInstantiator(const UI32 &operationCode)
{
  DcmManagementInterfaceMessage *pMsg = NULL;

  switch (operationCode) {
    case VRRPGLOBALUPDATEVRRPGLOBALCONFIG:
        pMsg = new VRRPGlobalConfigSPMessage();
        break;

    case VRRP_INTF_CONFIG_CLIENT_MSG:
      pMsg = new VRRPClientIntfConfigSPMessage();
      break;

    case VRRP_CLEAR_STATS_MSG:
      pMsg = new VRRPClearStatsMessage();
      break;

    case VRRP_DEBUG_MSG:
      pMsg = new VRRPDebugMessage();
      break;

    case VRRP_SHOW_MSG:
      pMsg = new VRRPShowMessage();
      break;

    default:
      break;
  }

  return (pMsg);
}

int setVRRPMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (VRRPClientMsgInstantiator);
     return status;
}

}
