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
 * HostVirtualSwitch.h
 *
 *  Created on: Apr 2, 2012
 *      Author: rmadapur
 */

#ifndef HOSTVIRTUALSWITCH_H_
#define HOSTVIRTUALSWITCH_H_
#include <vector>
#include <string>
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/ListUtil.h"
namespace DcmNs {

    class HostVirtualSwitch: public WSObject {

        public:
            string name;
            string key;
            vector<string*> *portgroup;
            vector<string*> *pnic;
            HostVirtualSwitch();
            virtual ~HostVirtualSwitch();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* HOSTVIRTUALSWITCH_H_ */
