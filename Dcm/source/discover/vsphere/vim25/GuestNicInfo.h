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
 * GuestNicInfo.h
 *
 *  Created on: Jan 17, 2013
 *      Author: rmadapur
 */

#ifndef GUESTNICINFO_H_
#define GUESTNICINFO_H_
#include <string>
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/ListUtil.h"
using namespace std;

namespace DcmNs {

    class NetIpConfigInfoIpAddress: public WSObject {
        public:
            string ipAddress;
            NetIpConfigInfoIpAddress(){}
            virtual ~NetIpConfigInfoIpAddress(){}
            void deSerialize(xmlNodePtr dyPropNode);
    };

    class NetIpConfigInfo: public WSObject {
        public:


            vector<NetIpConfigInfoIpAddress *> *ipAddress;
            NetIpConfigInfo();
            virtual ~NetIpConfigInfo();
            void deSerialize(xmlNodePtr dyPropNode);
    };

    class GuestNicInfo: public WSObject {
        public:
            string macAddress;
            NetIpConfigInfo ipConfig;
            GuestNicInfo(){}
            virtual ~GuestNicInfo();
            void deSerialize(xmlNodePtr dyPropNode);
    };

} /* namespace DcmNs */
#endif /* GUESTNICINFO_H_ */
