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
 * StateMachine.h
 *
 *  Created on: May 23, 2012
 *      Author: rmadapur
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_
#include <malloc.h>
#include <string>
#include "discover/StateMachine/StateEnum.h"
#include "discover/StateMachine/MultiDiscoveryRoutine.h"
#include "discover/collector/vCenterEventCollector.h"
#include "Framework/Types/Uuid.h"
#define VCENTER_MIN_TO_MILLISEC(x)          ((x)*60*1000)

using namespace std;
namespace DcmNs {

    class DiscoveryStateMachine;

    class StateMethod: public MultiDiscoveryRoutine {
        public:
            StateMethod(DISCOVERY_STATE _nextState) :
                    nextState(_nextState) {

            }
            virtual ~StateMethod();
            /**
             * State Method getting called on entering the state
             */
            virtual void update(StateActivateMessage *data, DiscoveryStateMachine *sm);
            /**
             * Next state after executing the StateMethod.
             */
            DISCOVERY_STATE nextState;
    };
//Macro for adding a associating stateMethods to stateArray
//STATE - Current State
//TRIGGER - Trigger on the Current State
#define STATE_METHOD(STATE,TRIGGER,NEXT_STATE) stateArray[STATE][TRIGGER] =   (StateMethod *) new STATE##_##TRIGGER(NEXT_STATE);

//StateMethod Definition
#define STATE_METHOD_DEF(STATE,TRIGGER) class  STATE##_##TRIGGER :public StateMethod{ \
    public : \
    STATE##_##TRIGGER(DISCOVERY_STATE _nextState) :\
                StateMethod(_nextState) { \
    }   \
    void update(StateActivateMessage *data,DiscoveryStateMachine *sm); \
    virtual ~STATE##_##TRIGGER() {\
    }\
    };


    STATE_METHOD_DEF(DISC_IDLE, ACTIVATE)
    STATE_METHOD_DEF(DISC_IDLE, TIMER_EXPIRED)
    STATE_METHOD_DEF(DISC_IDLE, ASSET_COLLECTION_COMPLETED)
    STATE_METHOD_DEF(DISC_IDLE, DISTRIBUTE)
    STATE_METHOD_DEF(DISC_IDLE, DISTRIBUTION_COMPLETED)
    STATE_METHOD_DEF(DISC_IDLE, DEACTIVATE)
    STATE_METHOD_DEF(DISC_IDLE, WARM_RESTART)
    STATE_METHOD_DEF(DISC_IDLE, SHUTDOWN)
    STATE_METHOD_DEF(DISC_IDLE, TIMER_INTERVAL)

    STATE_METHOD_DEF(DISC_IN_PRG, ACTIVATE)
    STATE_METHOD_DEF(DISC_IN_PRG, TIMER_EXPIRED)
    STATE_METHOD_DEF(DISC_IN_PRG, ASSET_COLLECTION_COMPLETED)
    STATE_METHOD_DEF(DISC_IN_PRG, DISTRIBUTE)
    STATE_METHOD_DEF(DISC_IN_PRG, DISTRIBUTION_COMPLETED)
    STATE_METHOD_DEF(DISC_IN_PRG, DEACTIVATE)
    STATE_METHOD_DEF(DISC_IN_PRG, WARM_RESTART)
    STATE_METHOD_DEF(DISC_IN_PRG, SHUTDOWN)
    STATE_METHOD_DEF(DISC_IN_PRG, TIMER_INTERVAL)

    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, ACTIVATE)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, TIMER_EXPIRED)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, ASSET_COLLECTION_COMPLETED)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, DISTRIBUTE)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, DISTRIBUTION_COMPLETED)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, DEACTIVATE)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, WARM_RESTART)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, SHUTDOWN)
    STATE_METHOD_DEF(DISC_ASSET_COMPLETED, TIMER_INTERVAL)

    STATE_METHOD_DEF(DISC_DIST_IN_PRG, ACTIVATE)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, TIMER_EXPIRED)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, ASSET_COLLECTION_COMPLETED)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, DISTRIBUTE)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, DISTRIBUTION_COMPLETED)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, DEACTIVATE)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, WARM_RESTART)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, SHUTDOWN)
    STATE_METHOD_DEF(DISC_DIST_IN_PRG, TIMER_INTERVAL)

    STATE_METHOD_DEF(DISC_DIST_COMPLETED, ACTIVATE)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, TIMER_EXPIRED)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, ASSET_COLLECTION_COMPLETED)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, DISTRIBUTE)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, DISTRIBUTION_COMPLETED)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, DEACTIVATE)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, WARM_RESTART)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, SHUTDOWN)
    STATE_METHOD_DEF(DISC_DIST_COMPLETED, TIMER_INTERVAL)




    class DiscoveryStateMachine {

            //Holds the various states of the statemachine
            StateMethod *stateArray[DISC_DIST_COMPLETED + 1][TIMER_INTERVAL + 1];

        public:
            vCenterState_t *state;
            //Holds the vCenterDetails
            StateActivateMessage *vCenterdata;
            //hanlde for the timer
            TimerHandle fullDiscoveryTimerHandle;


            volatile bool stopRequest;
            volatile bool markedForDeletion;
            volatile bool isVcenterActivated;
            string uuid;

            DISCOVERY_TYPE currentDiscoveryType;
            volatile bool scheduleFullDiscovery;
            volatile bool cleanStateMachineOnly;
            volatile bool eventProcessingInProgress;


            //Helper for obtaining the string representation of the Enum(StateEnum.h)
            static const char* DISCOVERY_STATE_TRIGGER_STRING[];
            static const char* DISCOVERY_STATE_STRING[];
            static const char* DISCOVERY_TYPE_STRING[];
            vCenterSourceCollector *collector;
            vCenterEventCollector *ecol;

            UI32 fullDiscoveryTimerInterval;
            DiscoveryStateMachine(vCenterState_t *_state) :
                    state(_state),vCenterdata(NULL),fullDiscoveryTimerHandle(0),stopRequest(false),markedForDeletion(false),isVcenterActivated(false),uuid(""),
                    currentDiscoveryType(VNM_FULL_DISCOVERY),scheduleFullDiscovery(false),cleanStateMachineOnly(false),eventProcessingInProgress(false),collector(NULL),ecol(NULL),fullDiscoveryTimerInterval(0){
                Uuid temp;
                uuid = temp.toString();
                updateDiscoveryInterval();
                STATE_METHOD(DISC_IDLE, ACTIVATE, DISC_IN_PRG);
                STATE_METHOD(DISC_IDLE, TIMER_EXPIRED, DISC_IDLE);
                STATE_METHOD(DISC_IDLE, ASSET_COLLECTION_COMPLETED, DISC_IDLE);
                STATE_METHOD(DISC_IDLE, DISTRIBUTE, DISC_IDLE);
                STATE_METHOD(DISC_IDLE, DISTRIBUTION_COMPLETED, DISC_IDLE);
                STATE_METHOD(DISC_IDLE, DEACTIVATE, DISC_IDLE);
                STATE_METHOD(DISC_IDLE, WARM_RESTART, DISC_IDLE);
                STATE_METHOD(DISC_IDLE, SHUTDOWN, DISC_IDLE);
                STATE_METHOD(DISC_IDLE, TIMER_INTERVAL, DISC_IDLE);

                STATE_METHOD(DISC_IN_PRG, ACTIVATE, DISC_IN_PRG);
                STATE_METHOD(DISC_IN_PRG, TIMER_EXPIRED, DISC_IN_PRG);
                STATE_METHOD(DISC_IN_PRG, ASSET_COLLECTION_COMPLETED,
                        DISC_ASSET_COMPLETED);
                STATE_METHOD(DISC_IN_PRG, DISTRIBUTE, DISC_IN_PRG);
                STATE_METHOD(DISC_IN_PRG, DISTRIBUTION_COMPLETED, DISC_IN_PRG);
                STATE_METHOD(DISC_IN_PRG, DEACTIVATE, DISC_IDLE);
                STATE_METHOD(DISC_IN_PRG, WARM_RESTART, DISC_IN_PRG);
                STATE_METHOD(DISC_IN_PRG, SHUTDOWN, DISC_IDLE);
                STATE_METHOD(DISC_IN_PRG, TIMER_INTERVAL, DISC_IN_PRG);

                STATE_METHOD(DISC_ASSET_COMPLETED, ACTIVATE,
                        DISC_ASSET_COMPLETED);
                STATE_METHOD(DISC_ASSET_COMPLETED, TIMER_EXPIRED,
                        DISC_ASSET_COMPLETED);
                STATE_METHOD(DISC_ASSET_COMPLETED, ASSET_COLLECTION_COMPLETED,
                        DISC_ASSET_COMPLETED);
                STATE_METHOD(DISC_ASSET_COMPLETED, DISTRIBUTE,
                        DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_ASSET_COMPLETED, DISTRIBUTION_COMPLETED,
                        DISC_ASSET_COMPLETED);
                STATE_METHOD(DISC_ASSET_COMPLETED, DEACTIVATE, DISC_IDLE);
                STATE_METHOD(DISC_ASSET_COMPLETED, WARM_RESTART,
                        DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_ASSET_COMPLETED, SHUTDOWN, DISC_IDLE);
                STATE_METHOD(DISC_ASSET_COMPLETED, TIMER_INTERVAL, DISC_ASSET_COMPLETED);

                STATE_METHOD(DISC_DIST_IN_PRG, ACTIVATE, DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_DIST_IN_PRG, TIMER_EXPIRED, DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_DIST_IN_PRG, ASSET_COLLECTION_COMPLETED,
                        DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_DIST_IN_PRG, DISTRIBUTE, DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_DIST_IN_PRG, DISTRIBUTION_COMPLETED,
                        DISC_DIST_COMPLETED);
                STATE_METHOD(DISC_DIST_IN_PRG, DEACTIVATE, DISC_IDLE);
                STATE_METHOD(DISC_DIST_IN_PRG, WARM_RESTART, DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_DIST_IN_PRG, SHUTDOWN, DISC_IDLE);
                STATE_METHOD(DISC_DIST_IN_PRG, TIMER_INTERVAL, DISC_DIST_IN_PRG);


                STATE_METHOD(DISC_DIST_COMPLETED, ACTIVATE,
                        DISC_DIST_IN_PRG);
                STATE_METHOD(DISC_DIST_COMPLETED, TIMER_EXPIRED, DISC_IN_PRG);
                STATE_METHOD(DISC_DIST_COMPLETED, ASSET_COLLECTION_COMPLETED,
                        DISC_DIST_COMPLETED);
                STATE_METHOD(DISC_DIST_COMPLETED, DISTRIBUTE,
                        DISC_DIST_COMPLETED);
                STATE_METHOD(DISC_DIST_COMPLETED, DISTRIBUTION_COMPLETED,
                        DISC_DIST_COMPLETED);
                STATE_METHOD(DISC_DIST_COMPLETED, DEACTIVATE, DISC_IDLE);
                STATE_METHOD(DISC_DIST_COMPLETED, WARM_RESTART,
                        DISC_DIST_COMPLETED);
                STATE_METHOD(DISC_DIST_COMPLETED, SHUTDOWN, DISC_IDLE);
                STATE_METHOD(DISC_DIST_COMPLETED, TIMER_INTERVAL, DISC_DIST_COMPLETED);




            }

            virtual ~DiscoveryStateMachine();
            void trigger(DISCOVERY_STATE_TRIGGER trigger, StateActivateMessage *data);
            void updateSuccess(DISCOVERY_STATE nextStateAfterUpdate);
            void updateDiscoveryInterval() ;
    };

} /* namespace DcmNs */
#endif /* STATEMACHINE_H_ */
