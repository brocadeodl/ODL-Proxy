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
 * HostDomain.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef HOSTDOMAIN_H_
#define HOSTDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"
#include "discover/vsphere/Debug.h"
namespace DcmNs {

    class HostDomain : public Domain{
    protected:
        virtual void   setupAttributesForSerialization();
    public:
			HostDomain();
            HostDomain(std::string, std::string, std::string);
            virtual ~HostDomain();
            std::string getId();
            void setId(std::string);
            std::string getName();
            void setName(std::string);
            std::string getDcId();
            void setDcId(std::string);
            bool fncompid (Domain* rhs) {
                int ret = (this->getId()).compare(rhs->getId());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"lhs %s rhs %s ",getId().c_str(),rhs->getId().c_str());
                if (ret < 0)
                    return true;

                return false;
            }

            void convertToEventMessage(EventMessage**);
            void printDomain();

    public:
            std::string id;
            std::string name;
            std::string dc_id;
    };

} /* namespace DcmNs */
#endif /* HOSTDOMAIN_H_ */
