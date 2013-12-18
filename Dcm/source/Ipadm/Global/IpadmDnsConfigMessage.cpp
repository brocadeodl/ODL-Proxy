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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : aviswana                                                     *
 **************************************************************************/

#include "Ipadm/Global/IpadmGlobalObjectManager.h"
#include "Ipadm/Global/IpadmDnsConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

    IpadmDnsConfigMessage::IpadmDnsConfigMessage ()
        : ManagementInterfaceMessage (IpadmGlobalObjectManager::getClassName (), IPADMDNSCONFIG)
    {
    }

    IpadmDnsConfigMessage::IpadmDnsConfigMessage (const string &opr, const UI32 &userWyserTag)
        : ManagementInterfaceMessage (IpadmGlobalObjectManager::getClassName (), IPADMDNSCONFIG),
        m_opr    (opr),
	m_userWyserTag	 (userWyserTag)
    {
    }

    IpadmDnsConfigMessage::~IpadmDnsConfigMessage ()
    {
    }

    void  IpadmDnsConfigMessage::setupAttributesForSerialization()
    {
	ManagementInterfaceMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&m_domain_name,    "domain_name"));
        addSerializableAttribute (new AttributeIpVxAddress  (&m_nameSrvIp,      "nameSrvIp"));
 	    addSerializableAttribute (new AttributeUI32         (&m_userWyserTag,    "userWyserTag"));
 	    addSerializableAttribute (new AttributeString       (&m_data,           "data"));
 	    addSerializableAttribute (new AttributeString       (&m_opr,            "opr"));
 	    addSerializableAttribute (new AttributeString       (&m_support,        "support"));
	    addSerializableAttribute (new AttributeUI32         (&m_index,          "index"));
    }

    void  IpadmDnsConfigMessage::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  IpadmDnsConfigMessage::getIndex() const
    {
        return (m_index);
    }

    void  IpadmDnsConfigMessage::setDomainName(const string &domain_name)
    {
        m_domain_name  =  domain_name;
    }

    string  IpadmDnsConfigMessage::getDomainName() const
    {
        return (m_domain_name);
    }

    void  IpadmDnsConfigMessage::setNameSrvIp(IpVxAddress &nameSrvIp)
    {
        m_nameSrvIp = nameSrvIp;
    }

    IpVxAddress  IpadmDnsConfigMessage::getNameSrvIp () const
    {
        return ( m_nameSrvIp);
    }

    void  IpadmDnsConfigMessage::setOpr(const string &opr)
    {
        m_opr  =  opr;
    }

    string  IpadmDnsConfigMessage::getOpr() const
    {
        return (m_opr);
    }

    UI32  IpadmDnsConfigMessage::getWyserUserTag() const
    {
        return (m_userWyserTag);
    }
    void   IpadmDnsConfigMessage::setWyserUserTag(const  UI32 &userWyserTag)
    {
        m_userWyserTag = userWyserTag;
    }
    string  IpadmDnsConfigMessage::getData() const
    {
        return (m_data);
    }
    void  IpadmDnsConfigMessage::setData(const string &data)
    {
        m_data += data;
    }
}
