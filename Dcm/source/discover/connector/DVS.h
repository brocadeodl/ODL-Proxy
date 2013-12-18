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
 * DVS.h
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#ifndef DVS_H_
#define DVS_H_
#include "discover/connector/Asset.h"
namespace DcmNs {



    class DVS :public Asset{

        public:
            string id;
            string name;
            string prgroup_nn;
            string prgroup_id;
            vector<string> host_mors;

            multimap<string,string> host_pnicList;
            map<string,string> nrp_qosList;
            DVS();
            virtual ~DVS();
    };

} /* namespace DcmNs */
#endif /* DVS_H_ */
