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
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/Sflow/SflowClientCreateSflowGlobalConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowClientDeleteSflowGlobalConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowShowMessage.h"
#include "ClientInterface/Sflow/SflowDebugMessage.h"
#include "ClientInterface/Sflow/SflowClientSflowProfileConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowProfileShowMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *SflowClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
		case SFLOWCLIENTCREATESFLOWGLOBALCONFIGSP:
			pMsg = new SflowClientCreateSflowGlobalConfigSPMessage();
			break;
		case SFLOWCLIENTDELETESFLOWGLOBALCONFIGSP:
			pMsg = new SflowClientDeleteSflowGlobalConfigSPMessage();
			break;
		case NSMPHYINTF:
			pMsg = new NsmPhyIntfMessage();
			break;
                case SFLOWSHOWMESSAGE:
			pMsg = new SflowShowMessage();
			break;
		case SFLOWDEBUGMESSAGE:
			pMsg = new SflowDebugMessage();
			break;
		case SFLOWCLIENTSFLOWPROFILECONFIGSP:
			pMsg = new SflowClientSflowProfileConfigSPMessage();
			break;
		case SFLOWPROFILESHOWMESSAGE:
			pMsg = new SflowProfileShowMessage();
			break;
		default:
			break;
	}

	return (pMsg);
}


int setSFLOWMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (SflowClientMsgInstantiator);
     return status;
}


}