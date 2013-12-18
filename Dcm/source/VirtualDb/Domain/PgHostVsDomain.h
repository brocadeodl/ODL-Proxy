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
 * PgHostVsDomain.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef PGHOSTVSDOMAIN_H_
#define PGHOSTVSDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"
#include "discover/vsphere/Debug.h"

namespace DcmNs {

    class EventMessage;
    class PgHostVsDomain : public Domain {
        protected:
            virtual void   setupAttributesForSerialization();
        public:
            PgHostVsDomain();
            PgHostVsDomain(std::string, std::string, std::string, std::string,
                    std::string,std::string);
            virtual ~PgHostVsDomain();
            std::string getPgId();
            void setPgId(std::string);
            std::string getHostNN();
            void setHostNN(std::string);
            std::string getVs();
            void setVs(std::string);
            std::string getVlan();
            void setVlan(std::string);
            std::string getHostId();
            void setHostId(std::string);
            std::string getDcId();
            void setDcId(std::string);
            std::string getPgNN();
            void setPgNN (std::string);
            void isConfigUpdate(Domain*, bool&, bool&, bool&);
            bool fncompid (Domain* rhs) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"pg id  lhs %s rhs %s ",getPgId().c_str(),rhs->getPgId().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"host id lhs %s rhs %s ",getHostId().c_str(),rhs->getHostId().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"dvs lhs %s rhs %s ",getVs().c_str(),rhs->getVs().c_str());

                int ret = (this->getPgId()).compare(rhs->getPgId());
                if (ret < 0)
                    return true;
                if (ret == 0) {
                    ret = (this->getHostId()).compare(rhs->getHostId());
                    if (ret < 0)
                        return true;
                    if (ret == 0) {
                        ret = (this->getVs()).compare(rhs->getVs());
                        if (ret < 0)
                            return true;
}
                }
                return false;
            }
            void convertToEventMessage(EventMessage**);
            void printDomain();

        public:
            std::string pg_id;
            std::string host_id;
            std::string host_nn;
            std::string vs;
            std::string vlan;
            std::string dc_id;
            std::string pg_nn;
    };

} /* namespace DcmNs */
#endif /* PGHOSTVSDOMAIN_H_ */
