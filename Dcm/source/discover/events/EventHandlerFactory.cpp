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



#include "EventHandlerFactory.h"
#include "EventHandler.h"
#include "DatacenterEventHandler.h"
#include "TaskEventHandler.h"
#include <string>
#include <iostream>

namespace DcmNs {

    EventHandlerFactory::EventHandlerFactory() {
        // TODO Auto-generated constructor stub

    }

    EventHandlerFactory::~EventHandlerFactory() {
        // TODO Auto-generated destructor stub
    }

    EventHandler* EventHandlerFactory::getHandler(string vcenter, Event* event) {
    	VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
    			"------get Handler ----------- ");

        if (event->type.compare("TaskEvent") == 0)
            return new TaskEventHandler(vcenter, event);
        else
            return new EventHandler(vcenter, event);

    	return NULL; // tmp
    }

} /* namespace DcmNs */
