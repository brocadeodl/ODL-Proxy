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
 * SourceCollector.h
 *
 *  Created on: Apr 24, 2012
 *      Author: rmadapur
 */

#ifndef SOURCECOLLECTOR_H_
#define SOURCECOLLECTOR_H_
#include "discover/WorkerPool/WorkerTask.h"

namespace DcmNs {

    class SourceCollector : public WorkerTask{

        public:
            SourceCollector();
            virtual ~SourceCollector();
            virtual void run();
            virtual void cleanup();
            volatile bool discoveryInProgress;
    };

} /* namespace DcmNs */
#endif /* SOURCECOLLECTOR_H_ */
