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
 *   Author : Umakiran G                                    *
 ***************************************************************************/

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/Span/SpanShowMessage.h"
#include "ClientInterface/Span/SpanClientLocalSpanLocalConfigMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

	ManagementInterfaceMessage *SpanClientMsgInstantiator(const UI32 &operationCode)
	{
		DcmManagementInterfaceMessage *pMsg = NULL;

		switch (operationCode) 
		{
			case NSMCLIENTCREATESPANLOCALCONFIG:
				pMsg = new SpanClientLocalSpanLocalConfigMessage();
				break;
			case SPANSHOWMESSAGE:
				pMsg = new SpanShowMessage();
				break;
			default:
				break;
		}

		return (pMsg);
	}

	int setSPANMsgHandlerForClient()
	{
    	int status = 1 ;

     	WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (SpanClientMsgInstantiator);
     	return status;
	}
}
