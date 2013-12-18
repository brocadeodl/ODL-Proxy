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

/***************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : mgarg                                                     *
 **************************************************************************/

#ifndef FABRICSERVICEGETFABRICINFOMESSAGE_H
#define FABRICSERVICEGETFABRICINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FabricServiceGetFabricInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FabricServiceGetFabricInfoMessage ();
            FabricServiceGetFabricInfoMessage (const UI32 &domain_count,const vector<UI32> &domain_id,const vector<WorldWideName> &domain_wwn,const vector<IpV4Address> &domain_eth_ip_addr,const vector<IpV4Address> &domain_fc_ip_addr,const vector<IpV6Address> &domain_ipv6_addr,const vector<string> &domain_name,const vector<UI32> &domain_hw_model,const vector<string> &domain_fw_version,const vector<bool> &principal_domain);
            virtual    ~FabricServiceGetFabricInfoMessage ();
            void setDomain_count(const UI32 &domain_count);
            UI32 getDomain_count()  const;
            void setDomain_id(const vector<UI32> &domain_id);
            vector<UI32> getDomain_id()  const;
            void setDomain_wwn(const vector<WorldWideName> &domain_wwn);
            vector<WorldWideName> getDomain_wwn()  const;
            void setDomain_eth_ip_addr(const vector<IpV4Address> &domain_eth_ip_addr);
            vector<IpV4Address> getDomain_eth_ip_addr()  const;
            void setDomain_fc_ip_addr(const vector<IpV4Address> &domain_fc_ip_addr);
            vector<IpV4Address> getDomain_fc_ip_addr()  const;
            void setDomain_ipv6_addr(const vector<IpV6Address> &domain_ipv6_addr);
            vector<IpV6Address> getDomain_ipv6_addr()  const;
            void setDomain_name(const vector<string> &domain_name);
            vector<string> getDomain_name()  const;
            void setDomain_hw_model(const vector<UI32> &domain_hw_model);
            vector<UI32> getDomain_hw_model()  const;
            void setDomain_fw_version(const vector<string> &domain_fw_version);
            vector<string> getDomain_fw_version()  const;
            void setPrincipal_domain(const vector<bool> &principal_domain);
            vector<bool> getPrincipal_domain()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_domain_count;
            vector<UI32>  m_domain_id;
            vector<WorldWideName>  m_domain_wwn;
            vector<IpV4Address>  m_domain_eth_ip_addr;
            vector<IpV4Address>  m_domain_fc_ip_addr;
            vector<IpV6Address>  m_domain_ipv6_addr;
            vector<string>  m_domain_name;
            vector<UI32>  m_domain_hw_model;
            vector<string>  m_domain_fw_version;
            vector<bool>  m_principal_domain;
    };
}
#endif                                            //FABRICSERVICEGETFABRICINFOMESSAGE_H
