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


#ifndef EVENTHANDLERFACTORY_H_
#define EVENTHANDLERFACTORY_H_
#include <string>
#include "EventHandler.h"
#include "discover/vsphere/vim25/Event.h"
#include <iostream>

using namespace std;
namespace DcmNs {

    class EventHandlerFactory {

        public:
            EventHandlerFactory();
            virtual ~EventHandlerFactory();
            static EventHandler* getHandler(string vcenter, Event *event);
    };

} /* namespace DcmNs */
#endif /* EVENTHANDLERFACTORY_H_ */
