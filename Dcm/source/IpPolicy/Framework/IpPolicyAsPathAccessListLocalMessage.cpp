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
#include "IpPolicy/Framework/IpPolicyAsPathAccessListLocalMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    IpPolicyAsPathAccessListLocalMessage::IpPolicyAsPathAccessListLocalMessage ()
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYIPASPATHACCESSLIST)
    {
        m_name_flag = false;
        m_instance_flag = false;
        m_action_flag = false;
        m_RegExprFlag = false;
    }

    IpPolicyAsPathAccessListLocalMessage::IpPolicyAsPathAccessListLocalMessage (const string &name,
        const UI16 &instance,
        const PolicyAction &action,
        const string &RegExpr)
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYIPASPATHACCESSLIST),
        m_name    (name),
        m_instance    (instance),
        m_action    (action),
        m_RegExpr    (RegExpr)
    {
        m_name_flag = true;
        m_instance_flag = true;
        m_action_flag = true;
        m_RegExprFlag = true;
    }

    IpPolicyAsPathAccessListLocalMessage::~IpPolicyAsPathAccessListLocalMessage ()
    {
    }

    void  IpPolicyAsPathAccessListLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)&m_op_code,"op_code"));
        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI16(&m_instance,"instance"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
        addSerializableAttribute (new AttributeString(&m_RegExpr,"RegExpr"));
        addSerializableAttribute (new AttributeString(&m_backend_err,"backend_err"));
       	addSerializableAttribute (new AttributeBool(&m_name_flag,"name_flag"));
       	addSerializableAttribute (new AttributeBool(&m_instance_flag,"instance_flag"));
       	addSerializableAttribute (new AttributeBool(&m_action_flag,"action_flag"));
       	addSerializableAttribute (new AttributeBool(&m_RegExprFlag,"RegExprFlag"));
    }

    void  IpPolicyAsPathAccessListLocalMessage::setName(const string &name)
    {
        m_name = name;
        m_name_flag = true;
    }

    string  IpPolicyAsPathAccessListLocalMessage::getName() const
    {
        return (m_name);
    }

    bool  IpPolicyAsPathAccessListLocalMessage::getNameFlag() const
    {
        return (m_name_flag);
    }

    void  IpPolicyAsPathAccessListLocalMessage::setInstance(const UI16 &instance)
    {
        m_instance = instance;
        m_instance_flag = true;
    }

    UI16  IpPolicyAsPathAccessListLocalMessage::getInstance() const
    {
        return (m_instance);
    }

    bool  IpPolicyAsPathAccessListLocalMessage::getInstanceFlag() const
    {
        return (m_instance_flag);
    }

    void  IpPolicyAsPathAccessListLocalMessage::setAction(const PolicyAction &action)
    {
        m_action = action;
        m_action_flag = true;
    }

    PolicyAction  IpPolicyAsPathAccessListLocalMessage::getAction() const
    {
        return (m_action);
    }

    bool  IpPolicyAsPathAccessListLocalMessage::getActionFlag() const
    {
        return (m_action_flag);
    }

    void  IpPolicyAsPathAccessListLocalMessage::setRegExpr(const string &RegExpr)
    {
    	m_RegExpr = RegExpr;
        m_RegExprFlag = true;
    }

    string  IpPolicyAsPathAccessListLocalMessage::getRegExpr() const
    {
        return (m_RegExpr);
    }

    bool  IpPolicyAsPathAccessListLocalMessage::getRegExprFlag() const
    {
        return (m_RegExprFlag);
    }


}
