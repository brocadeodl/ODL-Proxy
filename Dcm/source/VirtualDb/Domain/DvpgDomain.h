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

#ifndef DVPGDOMAIN_H_
#define DVPGDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"

namespace DcmNs {

    class DvpgDomain: public Domain {
		private:
		protected:
			virtual void   setupAttributesForSerialization();

        public:
            DvpgDomain();
            DvpgDomain(std::string, std::string, std::string, std::string,
                    std::string, std::string, std::string,std::string,std::string);
            virtual ~DvpgDomain();
            std::string getId();
            void setId(std::string);
            std::string getName();
            void setName(std::string);
            std::string getVlan();
            void setVlan(std::string);
            std::string getQos();
            void setQos(std::string);
            std::string getPortProfile();
            void setPortProfile(std::string);
            std::string getDvsId();
            void setDvsId(std::string);
            std::string getDvsNN();
            void setDvsNN(std::string);
            std::string getDcId();
            void setDcId(std::string);
            std::string getNetworkResourcePoolKey();
            void setNetworkResourcePoolKey(std::string);
            int getStatus();
            void setStatus(int);
            void isConfigUpdate(Domain*, bool&, bool&, bool&);
            std::string getPgNN();
            void printDomain();
            std::string getDVPGType();
            void setDVPGType(std::string);
    		void convertToEventMessage(EventMessage** msg);

        public:
            std::string id;
            std::string name;
            std::string vlan;
            std::string qos;
            std::string port_prof;
            std::string dvs_id;
            std::string dvs_nn;
            std::string dc_id;
            std::string nrpk;
            std::string dvpgtype;
            int status;
    };

} /* namespace DcmNs */
#endif /* DVPGDOMAIN_H_ */
