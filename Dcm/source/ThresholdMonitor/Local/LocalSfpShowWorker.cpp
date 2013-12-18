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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Yugant Chandrakar                                            *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/LocalSfpShowWorker.h"
#include "ThresholdMonitor/Local/SfpLocalShowMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include <thresh/portCmd.h>

namespace DcmNs
{
	LocalSfpShowWorker::LocalSfpShowWorker (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
	{
		addOperationMap (SFPLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&LocalSfpShowWorker::SfpLocalShowMessageHandler));
	}

	LocalSfpShowWorker::~LocalSfpShowWorker ()
	{
	}

	PrismMessage	*LocalSfpShowWorker::createMessageInstance (const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case SFPLOCALSHOW :
				pPrismMessage = new SfpLocalShowMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
	}

	string generateIfName (UI32 ifType, string ifName)
	{
		string retString;
		switch(ifType) {
			case IF_TYPE_GI:
				retString = string("GigabitEthernet ") + ifName;
				break;
			case IF_TYPE_TE:
				retString = string("TenGigabitEthernet ") + ifName;
				break;
			case IF_TYPE_FO:
				retString = string("FortyGigabitEthernet ") + ifName;
				break;
			default:
				retString = string("Invalid Type ") + ifName;
		}
		return retString;
	}

	void LocalSfpShowWorker::SfpLocalShowMessageHandler (SfpLocalShowMessage *pSfpLocalShowMessage)
	{
		trace (TRACE_LEVEL_INFO, "Entering SfpLocalShowMessageHandler..");
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		UI32 areaName = pSfpLocalShowMessage->getAreaName();
		string ifName = pSfpLocalShowMessage->getIfName();
		UI32 ifType = IF_TYPE_INVALID;

		const char *ifName_char;
		UI32 i;
		int slot = 0, port = 0, subport = 0, rbridgeId = 0;
		UI32 location_id = FrameworkToolKit::getThisLocationId();
		struct sfp_data {
			thThresholdEntry thEntry;
			char ifName[64];
		};
		sfp_data *data;
//		vector <thThresholdEntry> sfp_data ;

		if (ifName != "all") {
			trace(TRACE_LEVEL_INFO, "Entered interface part");
			if (NsmUtils::isValidThreeTuple(ifName)) {
				ifName_char = ifName.c_str();
				sscanf (ifName_char, "%d/%d/%d", &rbridgeId, &slot, &port);
			} else if (NsmUtils::isValidTwoTuple(ifName)) {
				ifName_char = ifName.c_str();
				sscanf (ifName_char, "%d/%d", &slot, &port);
			} else {
				trace (TRACE_LEVEL_ERROR, string("SfpLocalShowMessageHandler: Invalid interface given - ") + ifName);
				status = WAVE_MESSAGE_ERROR;
				pSfpLocalShowMessage->setCompletionStatus(status);
				reply(pSfpLocalShowMessage);
				return;
			}
			trace (TRACE_LEVEL_INFO, string("SfpLocalShowMessageHandler areaName")+areaName);
			//thaThresholdStructGet(AGENT_GBIC, areaName, portId, &thEntry);
			//thaClassAreaStructGet(AGENT_GBIC, area, &aaEntry);

		} else {
			trace(TRACE_LEVEL_INFO, "Entered interface all part");
			PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
			WaveManagedObjectSynchronousQueryContext *syncQueryInterface = new WaveManagedObjectSynchronousQueryContext(
				PhyIntfLocalManagedObject::getClassName());
			vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryInterface);
			if (pResults == NULL) {
				trace (TRACE_LEVEL_ERROR, string("SfpLocalShowMessageHandler: querySynchronously failed for PhyIntfLocalManagedObject "));
				status = FRAMEWORK_ERROR;
				pSfpLocalShowMessage->setCompletionStatus(status);
				reply(pSfpLocalShowMessage);
				return;
			}
			if (pResults->size() > 0) {
				data = (sfp_data *)calloc(pResults->size(),sizeof(sfp_data));
				UI32 port_count=0;
				for (i = 0; i < pResults->size(); i++) {
					pPhyIntfLocalManagedObject = dynamic_cast <PhyIntfLocalManagedObject *> ((*pResults)[i]);
					ifName = pPhyIntfLocalManagedObject->getIfName();
					ifType = pPhyIntfLocalManagedObject->getPhyType();
					subport = 0;
					if (NsmUtils::isValidThreeTuple(ifName)) {
						ifName_char = ifName.c_str();
						sscanf (ifName_char, "%d/%d/%d", &rbridgeId, &slot, &port);
						int position = ifName.find(':');
						if(position != -1) {
							subport = atoi(ifName.substr(position+1).c_str());
						}
						if (DcmToolKit::getLocationIdFromRbridgeId(rbridgeId) != DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId())) {
							continue;
						}
					} else if (NsmUtils::isValidTwoTuple(ifName)) {
						ifName_char = ifName.c_str();
						sscanf (ifName_char, "%d/%d", &slot, &port);
						int position = ifName.find(':');
						if(position != -1) {
							subport = atoi(ifName.substr(position+1).c_str());
						}
					} else {
						trace (TRACE_LEVEL_ERROR, string("SfpLocalShowMessageHandler: Invalid interface given - ") + ifName);
						continue;
					}
					trace (TRACE_LEVEL_INFO, string("SfpLocalShowMessageHandler areaName-")+areaName+" "+slot+"/"+port+":"+subport);
					int ret = thaInterfaceThresholdStructGet(AGENT_GBIC, areaName, slot, port, subport, &(data[port_count].thEntry));
					if (ret == 0) {
						string interface_name = generateIfName(ifType,ifName);
						strcpy(data[port_count].ifName,interface_name.c_str());
						port_count++;
						trace (TRACE_LEVEL_INFO, string("SfpLocalShowMessageHandler::thaInterfaceThresholdStructGet Passed for areaName=")+areaName+" ifName="+interface_name);
					} else {
						trace (TRACE_LEVEL_INFO, string("SfpLocalShowMessageHandler::thaInterfaceThresholdStructGet Failed for areaName=")+areaName+" ifName="+ifName+" retVal="+ret);
					}
				}
				if (port_count != 0)
					pSfpLocalShowMessage->addBuffer(location_id,sizeof(sfp_data)*port_count,data,false);

			free(data);
			}

			delete pResults;
			delete syncQueryInterface;
		}
		pSfpLocalShowMessage->setCompletionStatus(status);
		reply(pSfpLocalShowMessage);
		return;
	}

}
