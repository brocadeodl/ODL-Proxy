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
 * DataCenter.h
 *
 *  Created on: Apr 22, 2012
 *      Author: rmadapur
 */

#ifndef DATACENTER_H_
#define DATACENTER_H_
#include "discover/connector/Asset.h"

namespace DcmNs {

    class DataCenter :public Asset{
        public:
            string id;
            string name;
            DataCenter();
            virtual ~DataCenter();
    };

} /* namespace DcmNs */
#endif /* DATACENTER_H_ */
