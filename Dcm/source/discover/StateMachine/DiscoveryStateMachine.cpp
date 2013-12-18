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

/*
 * StateMachine.cpp
 *
 *  Created on: May 23, 2012
 *      Author: rmadapur
 */

#include "discover/StateMachine/DiscoveryStateMachine.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "discover/vsphere/Debug.h"
#include "vCenter/User/vCenterToolkit.h"

#include <stdio.h>

using namespace std;

namespace DcmNs {
const char* DiscoveryStateMachine::DISCOVERY_STATE_TRIGGER_STRING[] = {
		stringify( ACTIVATE ), stringify( TIMER_EXPIRED ),
		stringify( ASSET_COLLECTION_COMPLETED ), stringify( DISTRIBUTE ),
		stringify( DISTRIBUTION_COMPLETED ), stringify( DEACTIVATE ),
		stringify( WARM_RESTART ), stringify( SHUTDOWN ) };
const char* DiscoveryStateMachine::DISCOVERY_STATE_STRING[] = {
		stringify( DISC_IDLE ), stringify( DISC_IN_PRG ),
		stringify( DISC_ASSET_COMPLETED ), stringify( DISC_DIST_IN_PRG ),
		stringify( DISC_DIST_COMPLETED )

};
const char* DiscoveryStateMachine::DISCOVERY_TYPE_STRING[] = { "None", "Basic",
		"Full" };

DiscoveryStateMachine::~DiscoveryStateMachine() {
	//De-allocate memory of stateArray

	VIC_PLUG_LOG(VIC_TRACE_INFO, "%s", "Discovery StateMachine Destructor");
	for (int i = 0; i <= DISC_DIST_COMPLETED; i++) {
		for (int j = 0; j <= TIMER_INTERVAL; j++) {
			delete stateArray[i][j];
			stateArray[i][j] = NULL;

		}
	}

	if (state != NULL) {
		delete (state);
		state = NULL;
	}

	if (vCenterdata != NULL) {
		delete (vCenterdata);
		vCenterdata = NULL;
	}

}
void DiscoveryStateMachine::trigger(DISCOVERY_STATE_TRIGGER _trigger,
		StateActivateMessage *data) {
	tracePrintf(
			TRACE_LEVEL_INFO,
			"DiscoveryStateMachine:Trigger Received =%s[%d],CurrentState = %s[%d]",
			DISCOVERY_STATE_TRIGGER_STRING[_trigger], _trigger,
			DISCOVERY_STATE_STRING[this->state->discState],
			this->state->discState);

	this->stateArray[this->state->discState][_trigger]->update(data, this);

}

void DiscoveryStateMachine::updateSuccess(DISCOVERY_STATE nextState) {
	this->state->discState = nextState;
	tracePrintf(TRACE_LEVEL_INFO, "DiscoveryStateMachine:Nextstate = %s[%d]",
			DISCOVERY_STATE_STRING[this->state->discState],
			this->state->discState);
}

void DiscoveryStateMachine::updateDiscoveryInterval() {

	VcenterManagedObject* pMO = vCenterToolkit::getVcenterConfigFromMo(
			state->vcenterName);
	if (pMO != NULL) {
		UI32 interval = VCENTER_MIN_TO_MILLISEC(pMO->getInterval());
		fullDiscoveryTimerInterval = interval;
		delete pMO;
	}
}

StateMethod::~StateMethod() {

}

void StateMethod::update(StateActivateMessage *data,
		DiscoveryStateMachine *sm) {
	//printf("StateMethod update\n");

}

}

/* namespace DcmNs */
