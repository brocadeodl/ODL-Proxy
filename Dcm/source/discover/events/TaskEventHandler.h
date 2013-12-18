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


#ifndef TASKEVENTHANDLER_H_
#define TASKEVENTHANDLER_H_
#include "EventHandler.h"
#include <iostream>
#include "VirtualDb/Domain/DomainInclude.h"
#include "VirtualDb/DAO/DAOInclude.h"
#include "discover/vsphere/Debug.h"
#include "discover/utils/StopWatch.h"
#include "discover/utils/FileUtils.h"

using namespace std;
namespace DcmNs {

    class TaskEventHandler : public EventHandler {

        public:
            TaskEventHandler(string vcenter, Event* event);
            virtual ~TaskEventHandler();
            void handle(ServiceInstance *serviceInstance);
    		void removeDatacenter(ServiceInstance *serviceInstance,ManagedObjectReference mor, string dcId); 
    		void renameHost(ServiceInstance *serviceInstance,ManagedObjectReference mor, string dc_id); 
    		void renamePortgroup(ServiceInstance *serviceInstance);
    		void registerVM(ServiceInstance *serviceInstance);
    		void registerHost(ServiceInstance *serviceInstance);
    		string getHostName(string id);
    		int  waitTillTaskDone(ServiceInstance *serviceInstance,ManagedObjectReference mor); 
    		void qosUpdate(ServiceInstance *serviceInstance,string  dvs_mor, string dc_id);

    };

} /* namespace DcmNs */
#endif /* TASKEVENTHANDLER_H_ */
