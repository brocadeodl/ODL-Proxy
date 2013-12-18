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
 * DVPG.h
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#ifndef DVPG_H_
#define DVPG_H_
#include "discover/connector/Asset.h"
namespace DcmNs {

    class DVPG :public Asset{
        public:
            string id;
            string name;
            string dvs_nn;
            string vlan;
            string qos;
            string dvs_id;
            string dvs_name;
            string uplinktype;
            string networkResourcePoolKey;
            DVPG();
            virtual ~DVPG();
    };

} /* namespace DcmNs */
#endif /* DVPG_H_ */
