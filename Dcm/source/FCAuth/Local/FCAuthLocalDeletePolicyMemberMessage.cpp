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
 *   Author : noku                                                     *
 **************************************************************************/

#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/FCAuthLocalDeletePolicyMemberMessage.h"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    FCAuthLocalDeletePolicyMemberMessage::FCAuthLocalDeletePolicyMemberMessage ()
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALDELETEPOLICYMEMBER)
    {
    }

    FCAuthLocalDeletePolicyMemberMessage::FCAuthLocalDeletePolicyMemberMessage (const string &PolicyName,const string &PolicyEntryList)
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALDELETEPOLICYMEMBER),
        m_PolicyName    (PolicyName),
        m_PolicyEntryList    (PolicyEntryList)
    {
    }

    FCAuthLocalDeletePolicyMemberMessage::~FCAuthLocalDeletePolicyMemberMessage ()
    {
    }

    void  FCAuthLocalDeletePolicyMemberMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_PolicyName,"PolicyName"));
        addSerializableAttribute (new AttributeString(&m_PolicyEntryList,"PolicyEntryList"));
    }

    void  FCAuthLocalDeletePolicyMemberMessage::setPolicyName(const string &PolicyName)
    {
        m_PolicyName  =  PolicyName;
    }

    string  FCAuthLocalDeletePolicyMemberMessage::getPolicyName() const
    {
        return (m_PolicyName);
    }

    void  FCAuthLocalDeletePolicyMemberMessage::setPolicyEntryList(const string &PolicyEntryList)
    {
        m_PolicyEntryList  =  PolicyEntryList;
    }

    string  FCAuthLocalDeletePolicyMemberMessage::getPolicyEntryList() const
    {
        return (m_PolicyEntryList);
    }

}
