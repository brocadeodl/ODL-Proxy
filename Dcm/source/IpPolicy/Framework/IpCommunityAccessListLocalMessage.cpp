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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpCommunityAccessListLocalMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    IpCommunityAccessListLocalMessage::IpCommunityAccessListLocalMessage (const UI32 &prismOpcode)
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), prismOpcode)
    {
        m_name_flag = false;
        m_instance_flag = false;
        m_action_flag = false;
        m_ip_community_acl_type_flag = false;
        m_communityExpr = "";
		m_regExpr = "";
		m_userEnteredSeqid = true;
    }

    IpCommunityAccessListLocalMessage::IpCommunityAccessListLocalMessage (const string &name,
        const UI16 &instance,
        const PolicyAction &action,
        const IpCommunityAclType &ip_community_acl_type,
        const string &communityExpr,
        const string &regExpr,
        const bool &userEnteredSeqid,
        const UI32 &prismOpcode)
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), prismOpcode),
        m_name    (name),
        m_instance    (instance),
        m_action    (action),
        m_ip_community_acl_type (ip_community_acl_type),
        m_communityExpr  (communityExpr),
    	m_regExpr(regExpr),
    	m_userEnteredSeqid(userEnteredSeqid)
    {
        m_name_flag = true;
        m_instance_flag = true;
    }

    IpCommunityAccessListLocalMessage::~IpCommunityAccessListLocalMessage ()
    {
    }

    void  IpCommunityAccessListLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)&m_op_code,"op_code"));
        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI16(&m_instance,"instance"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_ip_community_acl_type,"ip_community_acl_type"));
        addSerializableAttribute (new AttributeString(&m_communityExpr,"communityExpr"));
        addSerializableAttribute (new AttributeString(&m_regExpr,"regExpr"));
        addSerializableAttribute (new AttributeBool(&m_userEnteredSeqid,"userEnteredSeqid"));
        addSerializableAttribute (new AttributeString(&m_backend_err,"backend_err"));
       	addSerializableAttribute (new AttributeBool(&m_name_flag,"name_flag"));
       	addSerializableAttribute (new AttributeBool(&m_instance_flag,"instance_flag"));
       	addSerializableAttribute (new AttributeBool(&m_action_flag,"action_flag"));
       	addSerializableAttribute (new AttributeBool(&m_ip_community_acl_type_flag,"ip_community_acl_type_flag"));
    }

    void  IpCommunityAccessListLocalMessage::setName(const string &name)
    {
        m_name = name;
        m_name_flag = true;
    }

    string  IpCommunityAccessListLocalMessage::getName() const
    {
        return (m_name);
    }

    bool  IpCommunityAccessListLocalMessage::getNameFlag() const
    {
        return (m_name_flag);
    }

    void  IpCommunityAccessListLocalMessage::setInstance(const UI16 &instance)
    {
        m_instance = instance;
        m_instance_flag = true;
    }

    UI16  IpCommunityAccessListLocalMessage::getInstance() const
    {
        return (m_instance);
    }

    bool  IpCommunityAccessListLocalMessage::getInstanceFlag() const
    {
        return (m_instance_flag);
    }

    void  IpCommunityAccessListLocalMessage::setAction(const PolicyAction &action)
    {
        m_action = action;
        m_action_flag = true;
    }

    PolicyAction  IpCommunityAccessListLocalMessage::getAction() const
    {
        return (m_action);
    }

    bool  IpCommunityAccessListLocalMessage::getActionFlag() const
    {
        return (m_action_flag);
    }

	void IpCommunityAccessListLocalMessage::setIp_community_acl_type(const IpCommunityAclType &ip_community_acl_type) {
		m_ip_community_acl_type = ip_community_acl_type;
        m_ip_community_acl_type_flag = true;
	}

	IpCommunityAclType IpCommunityAccessListLocalMessage::getIp_community_acl_type()  const {
		return (m_ip_community_acl_type);
	}

	bool IpCommunityAccessListLocalMessage::getIp_community_acl_typeFlag()  const {
		return (m_ip_community_acl_type_flag);
	}

	void IpCommunityAccessListLocalMessage::setCommunityExpr(const string &communityExpr) {
		m_communityExpr = communityExpr;
	}

	string IpCommunityAccessListLocalMessage::getCommunityExpr() const {
		return m_communityExpr;
	}

	string IpCommunityAccessListLocalMessage::getCommunityRegExpr()  const {
		return m_regExpr;
	}

	void IpCommunityAccessListLocalMessage::setCommunityRegExpr(const string &regExpr) {
		m_regExpr = regExpr;
	}

    bool IpCommunityAccessListLocalMessage::getUserEnteredSeqid() const {
    	return m_userEnteredSeqid;
    }

    void IpCommunityAccessListLocalMessage::setUserEnteredSeqid(const bool &userEnteredSeqid) {
    	m_userEnteredSeqid = userEnteredSeqid;
    }

}
