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
 * VmnicDomain.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef VMNICDOMAIN_H_
#define VMNICDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"
#include "discover/vsphere/Debug.h"

namespace DcmNs {

    class VmnicDomain: public Domain {
        protected:
            virtual void   setupAttributesForSerialization();
        public:
            VmnicDomain();
            VmnicDomain(std::string, std::string, std::string, std::string, std::string,std::string);
            virtual ~VmnicDomain();
            std::string getName();
            void setName(std::string);
            std::string getMac();
            void setMac(std::string);
            std::string getHostId();
            void setHostId(std::string);
            std::string getIface();
            void setIface(std::string);
            std::string getDvs();
            void setDvs(std::string);
            std::string getDvsNN();
            void setDvsNN(std::string);
            std::string getDcId();
            void setDcId(std::string);
            bool fncompid (Domain* rhs) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"name lhs %s rhs %s ",getName().c_str(),rhs->getName().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"host id lhs %s rhs %s ",getHostId().c_str(),rhs->getHostId().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"dvs lhs %s rhs %s ",getDvs().c_str(),rhs->getDvs().c_str());
                int ret = (this->getName()).compare(rhs->getName());

                if (ret < 0)
                    return true;
                if (ret == 0) {
                    ret = (this->getHostId()).compare(rhs->getHostId());
                    if (ret < 0)
                        return true;
                    if (ret == 0) {
                        ret = (this->getDvs()).compare(rhs->getDvs());
                        if (ret < 0)
                            return true;
                    }
                }

                return false;
            }
            void convertToEventMessage(EventMessage**);
            void printDomain();

        public:
            std::string name;
            std::string mac;
            std::string host_id;
            std::string iface;
            std::string dvs;
            std::string dvs_nn;
            std::string dc_id;
    };

} /* namespace DcmNs */
#endif /* VMNICDOMAIN_H_ */
