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
 * DistributedVirtualSwitchPortConnection.h
 *
 *  Created on: Mar 26, 2012
 *      Author: rmadapur
 */

#ifndef DISTRIBUTEDVIRTUALSWITCHPORTCONNECTION_H_
#define DISTRIBUTEDVIRTUALSWITCHPORTCONNECTION_H_
#include <string>
#include "discover/vsphere/vim25/WSObject.h"
using namespace std;
namespace DcmNs {

    /**
     * Represents a connection, or a association between DistributedVirtualPort and vnic/pnic.
     */
    class DistributedVirtualSwitchPortConnection: public WSObject {
        public:
            string switchUuid;
            string portgroupKey;
            string portKey;
            string connectionCookie;
            DistributedVirtualSwitchPortConnection();
            virtual ~DistributedVirtualSwitchPortConnection();
            void deSerialize(xmlNodePtr node);

    };

} /* namespace DcmNs */
#endif /* DISTRIBUTEDVIRTUALSWITCHPORTCONNECTION_H_ */
