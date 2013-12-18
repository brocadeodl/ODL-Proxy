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
 *   Author : pbalacha													   *
 ***************************************************************************/

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/Rmon/rmon_common.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateRmonEventMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteRmonEventMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateRmonAlarmMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteRmonAlarmMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateCollectionStatisticsMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteCollectionStatisticsMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateCollectionHistoryMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteCollectionHistoryMessage.h"
#include "ClientInterface/Rmon/RmonClientClearMessage.h"
#include "ClientInterface/Rmon/RmonClientInterface.h"
#include "ClientInterface/Rmon/RmonShowMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ManagementInterfaceMessage *RmonClientMsgInstantiator(const UI32 &operationCode)
{
	DcmManagementInterfaceMessage *pMsg = NULL;

	switch (operationCode) {
		case RMONCLIENTCREATERMONEVENT:
			pMsg = new RmonClientCreateRmonEventMessage();
			break;
		case RMONCLIENTDELETERMONEVENT:
			pMsg = new RmonClientDeleteRmonEventMessage();
			break;
		case RMONCLIENTCREATERMONALARM:
			pMsg = new RmonClientCreateRmonAlarmMessage();
			break;
		case RMONCLIENTDELETERMONALARM:
			pMsg = new RmonClientDeleteRmonAlarmMessage();
			break;
		case RMONCLIENTCREATECOLLECTIONSTATISTICS:
			pMsg = new RmonClientCreateCollectionStatisticsMessage();
			break;
		case RMONCLIENTDELETECOLLECTIONSTATISTICS:
			pMsg = new RmonClientDeleteCollectionStatisticsMessage();
			break;
		case RMONCLIENTCREATECOLLECTIONHISTORY:
			pMsg = new RmonClientCreateCollectionHistoryMessage();
			break;
		case RMONCLIENTDELETECOLLECTIONHISTORY:
			pMsg = new RmonClientDeleteCollectionHistoryMessage();
			break;
		case RMONSHOW:
            pMsg = new RmonShowMessage();
            break;
		case RMONCLIENTCLEAR:
			pMsg = new RmonClientClearMessage();
			break;

		default:
			break;
	}

	return (pMsg);
}


int setRmonMsgHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (RmonClientMsgInstantiator);
     return status;
}


}
