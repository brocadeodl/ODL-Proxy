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
 * Domain.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <string>
#include <vector>
#include "Framework/ObjectModel/SerializableObject.h"

using namespace WaveNs;

namespace DcmNs {
    class Host;
    class EventMessage;
    class Domain: public SerializableObject {
        protected:
            virtual void   setupAttributesForSerialization();
        public:
        	Domain();
            virtual ~Domain();
            virtual std::string getId() ;
            virtual void setId(std::string) ;
            virtual std::string getName();
            virtual void setName(std::string);
            virtual std::string getVlan() ;
            virtual void setVlan(std::string);
            virtual std::string getQos() ;
            virtual void setQos(std::string) ; 
            virtual std::string getHostId() ;
            virtual void setHostId(std::string) ;
            virtual std::string getDevice() ;
            virtual void setDevice(std::string);
            virtual std::string getVmId() ;
            virtual void setVmId(std::string) ;
            virtual std::string getDcId() ;
            virtual void setDcId(std::string) ;
            virtual std::string getMac() ;
            virtual void setMac(std::string);
            virtual std::string getPgNN();
            virtual void setPgNN(std::string);
            virtual std::string getDvpgNN();
            virtual void setDvpgNN(std::string);
            virtual std::string getDvpgId();
            virtual void setDvpgId(std::string);
            virtual std::string getPgId();
            virtual void setPgId(std::string);
            virtual std::string getDvs();
            virtual void setDvs(std::string);

            static bool compare(Domain* lhs, Domain* rhs);
            virtual bool fncompid(Domain* rhs) ;
            virtual void isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated);
            
            virtual void printDomain();

            static void printDomainVector(std::vector<Domain*> a);
            virtual void convertToEventMessage(EventMessage**);
            virtual void findAssetDependentUpdates(Host*, vector<Domain*>&);
            virtual void setDvsNN(std::string);
            virtual std::string getVs();
            virtual void setVs(std::string);
            virtual void setHost(std::string);
            virtual void setHostNN(std::string);
            virtual std::string getDVPGType();
            virtual void setDVPGType(std::string);
            virtual std::string getIface();
            virtual void setIface(std::string);
			virtual std::string getNetworkResourcePoolKey();
			virtual std::string getDvsId();
    };
}

#endif /* DOMAIN_H_ */
