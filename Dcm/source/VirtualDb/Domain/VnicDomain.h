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
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef VNICDOMAIN_H_
#define VNICDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"
#include "discover/vsphere/Debug.h"

namespace DcmNs {

    class VnicDomain: public Domain {
		protected:
			virtual void   setupAttributesForSerialization();

        public:
            VnicDomain();
            VnicDomain(std::string, std::string, std::string, std::string, std::string, std::string, std::string,std::string);
            virtual ~VnicDomain();
            std::string getName();
            void setName(std::string);
            std::string getMac();
            void setMac(std::string);
            std::string getIp();
            void setIp(std::string);
            std::string getDvpgId();
            void setDvpgId(std::string);
            std::string getDvpgNN();
            void setDvpgNN(std::string);
            std::string getPgNN();
            void setPgNN(std::string);
            std::string getPortProfile();
            void setPortProfile(std::string);
            std::string getVmId();
            void setVmId(std::string);
            std::string getDvs();
            void setDvs(std::string);
            std::string getDcId();
            void setDcId(std::string);
            int getStatus();
            void setStatus(int);
            bool fncompid (Domain* rhs) {
VIC_PLUG_LOG(VIC_TRACE_DEBUG,
			"---this vmid(%s):mac(%s):dvpg(%s):name(%s) ", this->getVmId().c_str(),this->getMac().c_str(), this->getDvpgNN().c_str(), this->getName().c_str());                	
VIC_PLUG_LOG(VIC_TRACE_DEBUG,
			"---rhs vmid(%s):mac(%s):dvpg(%s):name(%s) ", rhs->getVmId().c_str(),rhs->getMac().c_str(), rhs->getDvpgNN().c_str(), rhs->getName().c_str());                	
            
                int ret = (this->getVmId()).compare(rhs->getVmId());
                if (ret < 0)
                    return true;

                if (ret == 0) {
                    ret = (this->getDvpgNN()).compare(rhs->getDvpgNN());
                    if (ret < 0)
                        return true;
                }
                return false;
            }
            void isConfigUpdate(Domain*, bool&, bool&, bool&);
            void printDomain();
			void convertToEventMessage(EventMessage**);
        public:
            std::string name;
            std::string mac;
            std::string ip;
            std::string dvpg_id;
            std::string dvpg_nn;
            std::string port_prof;
            std::string vm_id;
            std::string dvs;
            std::string dc_id;
            int status;
    };

} /* namespace DcmNs */
#endif /* VNICDOMAIN_H_ */
