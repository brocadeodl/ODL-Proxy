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
 * PhysicalNic.h
 *
 *  Created on: Apr 1, 2012
 *      Author: rmadapur
 */

#ifndef PHYSICALNIC_H_
#define PHYSICALNIC_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
using namespace std;

namespace DcmNs {
    /**
     * This data object type describes the physical network adapter
     *  as seen by the primary operating system.
     */
    class PhysicalNic: public WSObject {
        public:
            /**
             * The linkable identifier.
             */
            string key;
            /**
             * The device name of the physical network adapter.
             */
            string device;
            /*
             * The media access control (MAC) address of the physical network adapter.
             */
            string mac;

            PhysicalNic();
            virtual ~PhysicalNic();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* PHYSICALNIC_H_ */
