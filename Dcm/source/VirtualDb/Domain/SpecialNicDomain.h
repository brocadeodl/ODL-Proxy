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
 * SpecialNicDomain.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef SPECIALNICDOMAIN_H_
#define SPECIALNICDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"

namespace DcmNs {

    class EventMessage;
    class SpecialNicDomain: public Domain{
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SpecialNicDomain();
            SpecialNicDomain(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string);
            virtual ~SpecialNicDomain();
            std::string getDevice();
            void setDevice(std::string);
            std::string getMac();
            void setMac(std::string);
            std::string getIp();
            void setIp(std::string);
            std::string getPgNN();
            void setPgNN(std::string);
            std::string getPortProfile();
            void setPortProfile(std::string);
            std::string getHostId();
            void setHostId(std::string);
            std::string getVs();
            void setVs(std::string);
            std::string getVnicType();
            void setVnicType(std::string);
            int getStatus();
            void setStatus(int);
            bool fncompid (Domain* rhs) {
                int ret = (this->getHostId()).compare(rhs->getHostId());
                if (ret < 0)
                    return true;

                if (ret == 0) {
                    ret = (this->getDevice()).compare(rhs->getDevice());
                    if (ret < 0)
                        return true;
                }

                return false;
            }
            std::string getDcId();
            std::string getDvpgId();
            void setDvpgId(std::string);
            void setDcId(std::string);
            void isConfigUpdate(Domain*, bool&, bool&, bool&);
            void convertToEventMessage(EventMessage**);
            void printDomain();

        public:
            std::string device;
            std::string mac;
            std::string ip;
            std::string pg_nn;
            std::string dvpg_id; // incase of spnics
            std::string port_prof;
            std::string host_id;
            std::string vs;
            std::string vnic_type;
            int status;
            std::string dc_id;
    };

} /* namespace DcmNs */
#endif /* SPECIALNICDOMAIN_H_ */
