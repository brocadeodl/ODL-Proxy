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
 * StateEnum.h
 *
 *  Created on: May 24, 2012
 *      Author: rmadapur
 */

#ifndef STATEENUM_H_
#define STATEENUM_H_
#include <string>



using namespace std;
#define stringify( name ) # name

typedef enum DISCOVERY_TYPE_{
        VNM_NONE_DISCOVERY =0,
        VNM_FULL_DISCOVERY
   }DISCOVERY_TYPE ;

/**
 * Enumeration holding the various discoveryStates
 */
typedef enum {
    DISC_IDLE = 0, //vCenter configured but not activated
    DISC_IN_PRG, //vCenter Discovery scheduled or in progress
    DISC_ASSET_COMPLETED, // Discovery of Assets completed
    DISC_DIST_IN_PRG, //Distribution of discovered assets to all nodes(Local Plugin)
    DISC_DIST_COMPLETED
//Complete discovery completed.
} DISCOVERY_STATE;

typedef enum {
    ACTIVATE = 0, //Trigger on Activating vCenter configuration
    TIMER_EXPIRED, //vCenter Timer Expired (Trigger to schedule a new disocvery)
    ASSET_COLLECTION_COMPLETED, //Trigger when the Asset collection is completed
    DISTRIBUTE, //Trigger to distribute the Assets
    DISTRIBUTION_COMPLETED, //Distribution Completed
    DEACTIVATE, //Trigger to deactivate vCenter
    WARM_RESTART, ///Trigger after HA
    SHUTDOWN,
    TIMER_INTERVAL
//Trigger for Shuting down the Plugin

} DISCOVERY_STATE_TRIGGER;
//update DISCOVERY_STATE_TRIGGER_STRING if the enum is updated.


typedef struct vCenterState_ {
        string vcenterName;
        DISCOVERY_STATE discState;

} vCenterState_t;

#endif /* STATEENUM_H_ */
