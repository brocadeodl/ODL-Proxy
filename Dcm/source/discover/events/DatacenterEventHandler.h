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


#ifndef DATACENTEREVENTHANDLER_H_
#define DATACENTEREVENTHANDLER_H_
#include "EventHandler.h"
#include <iostream>
#include "VirtualDb/DAO/DAOInclude.h"
#include "discover/vsphere/Debug.h"
#include "discover/utils/StopWatch.h"
#include "discover/utils/FileUtils.h"

using namespace std;
namespace DcmNs {

    class DatacenterEventHandler : public EventHandler {

        public:
            DatacenterEventHandler(string vcenter, Event* event);
            virtual ~DatacenterEventHandler();
            void handle(ServiceInstance *serviceInstance);
    };

} /* namespace DcmNs */
#endif /* DATACENTEREVENTHANDLER_H_ */
