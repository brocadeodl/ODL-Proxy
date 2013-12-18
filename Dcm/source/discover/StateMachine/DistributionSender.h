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
 * DistributionSender.h
 *
 *  Created on: Jun 6, 2012
 *      Author: rmadapur
 */

#ifndef DISTRIBUTIONSENDER_H_
#define DISTRIBUTIONSENDER_H_
#include "Framework/ObjectModel/WaveWorker.h"
#include "discover/StateMachine/StateEnum.h"
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/PrismMutex.h"
#include "discover/User/discoverDcmDef.h"
#include <time.h>
#include <string>
using namespace std;
using namespace WaveNs;
namespace DcmNs {




    /**
     * Distributes Discovered DB Details to Local Plugins.
     * Sends message to all Local Plugins.
     *
     * TODO - Ensure that only one worker thread is able to communicate with the Local Plugins
     */
    class DistributionSender {
        public:
            DistributionSender();
            PrismMutex *m_pSynchronizingMutex;
            PrismCondition *m_pSynchronizingCondition;

            virtual ~DistributionSender();
            static DistributionSender *instance;
            static DistributionSender *getInstance();
            WaveMessageStatus distribute(string vcenter,
                    DISCOVERY_TYPE _discType,discoverVcenterCmdCode_t correlationType);
            WaveMessageStatus nodeAddDistributionhandler(vector<string> vcenterList,
                                        vector<LocationId> locationsVector);
            ResourceId publishEvent(ManagementInterfaceMessage *pMessage);	  
        protected:
            /**
             * Distributed DB to Local Plugins on the specfied list of locations
             */
            WaveMessageStatus distributeDBtoLocalPlugins(string dbfile,vector<LocationId> connectedLocationsVector, string destdbfile="");
            /**
             * Distribute /tmp/vcenter.db.new to all local plugins in FC
             * Distribute /var/tmp/vcenter.db.new to all local plugin in MC mode;
             * In standalone returns success without distributing.
             */
            WaveMessageStatus distributeDiscoveredNewDBtoLocalPlugins(string vcenter,
                    DISCOVERY_TYPE _discType);
            WaveMessageStatus nodeAddDistribute(string vcenter,
                        vector<LocationId> locationsVector);

            WaveMessageStatus distributevCenterNames(vector<string> vcenterList,
                                vector<LocationId> locationsVector);

            /**
             * Send Distribute completed Message to Local Plugins.
             */
            void sendMessageToLocalPluginStep(string vcenter,
                    DISCOVERY_TYPE _discType,discoverVcenterCmdCode_t correlationType,vector<LocationId> locationsVector);

    };

} /* namespace DcmNs */
#endif /* DISTRIBUTIONSENDER_H_ */
