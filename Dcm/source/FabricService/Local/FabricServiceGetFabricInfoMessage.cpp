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

#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV6Address.h"
#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "FabricService/Local/FabricServiceGetFabricInfoMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServiceGetFabricInfoMessage::FabricServiceGetFabricInfoMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETFABRICINFO)
    {
    }

    FabricServiceGetFabricInfoMessage::FabricServiceGetFabricInfoMessage (const UI32 &domain_count,const vector<UI32> &domain_id,const vector<WorldWideName> &domain_wwn,const vector<IpV4Address> &domain_eth_ip_addr,const vector<IpV4Address> &domain_fc_ip_addr,const vector<IpV6Address> &domain_ipv6_addr,const vector<string> &domain_name,const vector<UI32> &domain_hw_model,const vector<string> &domain_fw_version,const vector<bool> &principal_domain)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEGETFABRICINFO),
        m_domain_count    (domain_count),
        m_domain_id    (domain_id),
        m_domain_wwn    (domain_wwn),
        m_domain_eth_ip_addr    (domain_eth_ip_addr),
        m_domain_fc_ip_addr    (domain_fc_ip_addr),
        m_domain_ipv6_addr    (domain_ipv6_addr),
        m_domain_name    (domain_name),
        m_domain_hw_model    (domain_hw_model),
        m_domain_fw_version    (domain_fw_version),
        m_principal_domain    (principal_domain)
    {
    }

    FabricServiceGetFabricInfoMessage::~FabricServiceGetFabricInfoMessage ()
    {
    }

    void  FabricServiceGetFabricInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_domain_count,"domain_count"));
        addSerializableAttribute (new AttributeUI32Vector(&m_domain_id,"domain_id"));
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_domain_wwn,"domain_wwn"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_domain_eth_ip_addr,"domain_eth_ip_addr"));
        addSerializableAttribute (new AttributeIpV4AddressVector(&m_domain_fc_ip_addr,"domain_fc_ip_addr"));
        addSerializableAttribute (new AttributeIpV6AddressVector(&m_domain_ipv6_addr,"domain_ipv6_addr"));
        addSerializableAttribute (new AttributeStringVector(&m_domain_name,"domain_name"));
        addSerializableAttribute (new AttributeUI32Vector(&m_domain_hw_model,"domain_hw_model"));
        addSerializableAttribute (new AttributeStringVector(&m_domain_fw_version,"domain_fw_version"));
        addSerializableAttribute (new AttributeBoolVector(&m_principal_domain,"principal_domain"));
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_count(const UI32 &domain_count)
    {
        m_domain_count  =  domain_count;
    }

    UI32  FabricServiceGetFabricInfoMessage::getDomain_count() const
    {
        return (m_domain_count);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_id(const vector<UI32> &domain_id)
    {
        m_domain_id  =  domain_id;
    }

    vector<UI32>  FabricServiceGetFabricInfoMessage::getDomain_id() const
    {
        return (m_domain_id);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_wwn(const vector<WorldWideName> &domain_wwn)
    {
        m_domain_wwn  =  domain_wwn;
    }

    vector<WorldWideName>  FabricServiceGetFabricInfoMessage::getDomain_wwn() const
    {
        return (m_domain_wwn);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_eth_ip_addr(const vector<IpV4Address> &domain_eth_ip_addr)
    {
        m_domain_eth_ip_addr  =  domain_eth_ip_addr;
    }

    vector<IpV4Address>  FabricServiceGetFabricInfoMessage::getDomain_eth_ip_addr() const
    {
        return (m_domain_eth_ip_addr);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_fc_ip_addr(const vector<IpV4Address> &domain_fc_ip_addr)
    {
        m_domain_fc_ip_addr  =  domain_fc_ip_addr;
    }

    vector<IpV4Address>  FabricServiceGetFabricInfoMessage::getDomain_fc_ip_addr() const
    {
        return (m_domain_fc_ip_addr);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_ipv6_addr(const vector<IpV6Address> &domain_ipv6_addr)
    {
        m_domain_ipv6_addr  =  domain_ipv6_addr;
    }

    vector<IpV6Address>  FabricServiceGetFabricInfoMessage::getDomain_ipv6_addr() const
    {
        return (m_domain_ipv6_addr);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_name(const vector<string> &domain_name)
    {
        m_domain_name  =  domain_name;
    }

    vector<string>  FabricServiceGetFabricInfoMessage::getDomain_name() const
    {
        return (m_domain_name);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_hw_model(const vector<UI32> &domain_hw_model)
    {
        m_domain_hw_model  =  domain_hw_model;
    }

    vector<UI32>  FabricServiceGetFabricInfoMessage::getDomain_hw_model() const
    {
        return (m_domain_hw_model);
    }

    void  FabricServiceGetFabricInfoMessage::setDomain_fw_version(const vector<string> &domain_fw_version)
    {
        m_domain_fw_version  =  domain_fw_version;
    }

    vector<string>  FabricServiceGetFabricInfoMessage::getDomain_fw_version() const
    {
        return (m_domain_fw_version);
    }

    void  FabricServiceGetFabricInfoMessage::setPrincipal_domain(const vector<bool> &principal_domain)
    {
        m_principal_domain  =  principal_domain;
    }

    vector<bool>  FabricServiceGetFabricInfoMessage::getPrincipal_domain() const
    {
        return (m_principal_domain);
    }

}
