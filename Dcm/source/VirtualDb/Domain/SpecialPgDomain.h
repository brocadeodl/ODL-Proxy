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
 * SpecialPgDomain.h
 *
 *  Created on: Sep 1, 2012
 *      Author: nfarhi
 */

#ifndef SPECIALPGDOMAIN_H_
#define SPECIALPGDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"

namespace DcmNs {

    class SpecialPgDomain: public Domain{
    protected:
        virtual void   setupAttributesForSerialization();

        public:
            SpecialPgDomain();
            SpecialPgDomain(std::string, std::string, std::string, std::string, std::string, std::string);
            virtual ~SpecialPgDomain();
            std::string getPgNN();
            void setPgNN(std::string);
            std::string getPortProfile();
            void setPortProfile(std::string);
            int getStatus();
            void setStatus(int);
            bool fncompid (Domain* rhs);
            void isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated);
            std::string getDcId();
            void setDcId(std::string);

            std::string getHostNN();
            void setHostNN(std::string);
            std::string getVs();
            void setVs(std::string);
            std::string getVlan();
            void setVlan(std::string);
            std::string getHostId();
            void setHostId(std::string);
            void convertToEventMessage(EventMessage**);


            void printDomain();

        public:
            std::string pg_nn;
            std::string port_prof;
            int status;
            std::string dc_id;
            std::string host_id;
            std::string host_nn;
            std::string vs;
            std::string vlan;
    };

} /* namespace DcmNs */
#endif /* SPECIALPGDOMAIN_H_ */
