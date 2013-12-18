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
 * MultiDiscoveryRoutine.h
 *
 *  Created on: Jun 5, 2012
 *      Author: rmadapur
 */

#ifndef MULTIDISCOVERYROUTINE_H_
#define MULTIDISCOVERYROUTINE_H_

#include "discover/StateMachine/StateEnum.h"
#include "discover/collector/vCenterSourceCollector.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/User/DiscoverMultiWorker.h"
#include "discover/vsphere/Debug.h"

namespace DcmNs {
    class DiscoveryStateMachine;
    class StateActivateMessage;
    class MultiDiscoveryRoutine {
        private:
            void scheduleCollector(void *data,
                        DiscoveryStateMachine *sm,StateActivateMessage *stateMsg, DISCOVERY_STATE nextState);
        public:
            MultiDiscoveryRoutine();
            virtual ~MultiDiscoveryRoutine();
            void scheduleDiscovery(void *data,
                        DiscoveryStateMachine *sm, DISCOVERY_STATE nextState);

            void stopDiscoveryCollector(void *data,
                        DiscoveryStateMachine *sm);
            void startFullDiscoveryTimer(void *data,
                        DiscoveryStateMachine *sm);

            void stopFullDiscoveryTimer(void *data,
                           DiscoveryStateMachine *sm);


            void executePostDiscoveryComplete(void *data,
                           DiscoveryStateMachine *sm);



            void scheduleFullDiscoveryMethod(void *data,
                                        DiscoveryStateMachine *sm);
            void processFullDiscoverTimerExpiry (TimerHandle timerHandle, void * pContext);



            void distributeToLocalPlugins(void *data,DiscoveryStateMachine *sm);
            void deleteVCenter(void *data,DiscoveryStateMachine *sm);

            void shutdownVcenter(void *data,DiscoveryStateMachine *sm);

            void no_op_log(void *data);
            void updatevCenterData(string vcenterName,DiscoveryStateMachine *sm);
            void updateTimerInterval(void *data,
                        DiscoveryStateMachine *sm,bool restartTimer);
    };

} /* namespace DcmNs */
#endif /* MULTIDISCOVERYROUTINE_H_ */
