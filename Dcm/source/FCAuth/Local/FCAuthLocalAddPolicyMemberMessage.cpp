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
#include "FCAuth/Local/FCAuthLocalAddPolicyMemberMessage.h"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    FCAuthLocalAddPolicyMemberMessage::FCAuthLocalAddPolicyMemberMessage ()
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALADDPOLICYMEMBER)
    {
    }

    FCAuthLocalAddPolicyMemberMessage::FCAuthLocalAddPolicyMemberMessage (const string &PolicyName,const string &PolicyEntryList,const bool &DefinedFlag,const bool &CfgReplayFlag)
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALADDPOLICYMEMBER),
        m_PolicyName    (PolicyName),
        m_PolicyEntryList    (PolicyEntryList),
        m_DefinedFlag    (DefinedFlag),
        m_CfgReplayFlag    (CfgReplayFlag)
    {
    }

    FCAuthLocalAddPolicyMemberMessage::~FCAuthLocalAddPolicyMemberMessage ()
    {
    }

    void  FCAuthLocalAddPolicyMemberMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_PolicyName,"PolicyName"));
        addSerializableAttribute (new AttributeString(&m_PolicyEntryList,"PolicyEntryList"));
        addSerializableAttribute (new AttributeBool(&m_DefinedFlag,"DefinedFlag"));
        addSerializableAttribute (new AttributeBool(&m_CfgReplayFlag,"CfgReplayFlag"));
    }

    void  FCAuthLocalAddPolicyMemberMessage::setPolicyName(const string &PolicyName)
    {
        m_PolicyName  =  PolicyName;
    }

    string  FCAuthLocalAddPolicyMemberMessage::getPolicyName() const
    {
        return (m_PolicyName);
    }

    void  FCAuthLocalAddPolicyMemberMessage::setPolicyEntryList(const string &PolicyEntryList)
    {
        m_PolicyEntryList  =  PolicyEntryList;
    }

    string  FCAuthLocalAddPolicyMemberMessage::getPolicyEntryList() const
    {
        return (m_PolicyEntryList);
    }

    void  FCAuthLocalAddPolicyMemberMessage::setDefinedFlag(const bool &DefinedFlag)
    {   
        m_DefinedFlag  =  DefinedFlag;
    }

    bool  FCAuthLocalAddPolicyMemberMessage::getDefinedFlag() const
    {
        return (m_DefinedFlag);
    }

    void  FCAuthLocalAddPolicyMemberMessage::setCfgReplayFlag(const bool &CfgReplayFlag)
    {
        m_CfgReplayFlag  =  CfgReplayFlag;
    }

    bool  FCAuthLocalAddPolicyMemberMessage::getCfgReplayFlag() const
    {
        return (m_CfgReplayFlag);
    }
}
