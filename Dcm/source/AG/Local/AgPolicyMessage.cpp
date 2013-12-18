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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Alphonsa Paul                                                *
 **************************************************************************/

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgPolicyMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgPolicyMessage::AgPolicyMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_POLICY_MESSAGE)
    {
    }

    AgPolicyMessage::AgPolicyMessage (const string &Policy, const bool &Enable)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_POLICY_MESSAGE),
        m_Policy(Policy),
        m_Enable(Enable)
    {
    }

    AgPolicyMessage::~AgPolicyMessage ()
    {
    }

    void  AgPolicyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_Policy, "policy"));
        addSerializableAttribute (new AttributeBool(&m_Enable, "enable"));
    }

    void  AgPolicyMessage::setPolicy(const string &Policy)
    {
        m_Policy =  Policy;
    }

    string  AgPolicyMessage::getPolicy() const
    {
        return (m_Policy);
    }

    void  AgPolicyMessage::setEnable(const bool &Enable)
    {
        m_Enable =  Enable;
    }

    bool AgPolicyMessage::getEnable() const
    {
        return (m_Enable);
    }

}
