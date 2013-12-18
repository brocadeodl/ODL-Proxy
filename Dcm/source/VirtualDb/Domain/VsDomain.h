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
 * VsDomain.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef VSDOMAIN_H_
#define VSDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"

namespace DcmNs {

    class EventMessage;
    class Host;
    class VsDomain : public Domain {
        protected:
            virtual void   setupAttributesForSerialization();
        public:
            VsDomain();
            VsDomain(std::string, std::string, std::string,std::string);
            virtual ~VsDomain();
            std::string getName();
            void setName(std::string);
            std::string getHostId();
            void setHostId(std::string);
            std::string getHost();
            void setHost(std::string);
            std::string getDcId();
            void setDcId(std::string);
            void printDomain();
            void convertToEventMessage(EventMessage**);
            void findAssetDependentUpdates(Host* host, vector<Domain*>& vmnics);
            bool fncompid (Domain* rhs) {
                int ret = (this->getHostId()).compare(rhs->getHostId());                if (ret < 0)
                    return true;

                if (ret == 0) {
                    ret = (this->getName()).compare(rhs->getName());                    if (ret < 0)
                        return true;
                }

                return false;
            }


        public:
            std::string name;
            std::string host_id;
            std::string host;
            std::string dc_id;
    };

} /* namespace DcmNs */
#endif /* VSDOMAIN_H_ */
