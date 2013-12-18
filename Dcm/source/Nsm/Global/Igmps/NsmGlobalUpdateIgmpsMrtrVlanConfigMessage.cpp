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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Nsm/Global/Igmps/NsmGlobalUpdateIgmpsMrtrVlanConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::NsmGlobalUpdateIgmpsMrtrVlanConfigMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEIGMPSMRTRVLANCONFIG)
    {
		m_ifType = 0;
		m_negation = false;
		m_vlanId = 1;
		m_vlanIfIndex = 0;
    }

    NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::NsmGlobalUpdateIgmpsMrtrVlanConfigMessage (const UI32 &ifType,const string &ifName,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const bool &negation)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEIGMPSMRTRVLANCONFIG),
        m_ifType    (ifType),
        m_ifName    (ifName),
        m_vlanId    (vlanId),
        m_vlanIfName    (vlanIfName),
        m_vlanIfIndex    (vlanIfIndex),
        m_negation    (negation)
    {
    }

    NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::~NsmGlobalUpdateIgmpsMrtrVlanConfigMessage ()
    {
    }

    void  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeString(&m_vlanIfName,"vlanIfName"));
        addSerializableAttribute (new AttributeUI64(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
    }

    void  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::setVlanIfName(const string &vlanIfName)
    {
        m_vlanIfName  =  vlanIfName;
    }

    string  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::getVlanIfName() const
    {
        return (m_vlanIfName);
    }

    void  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::setVlanIfIndex(const UI64 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI64  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::setNegation(const bool &negation)
    {
        m_negation  =  negation;
    }

    bool  NsmGlobalUpdateIgmpsMrtrVlanConfigMessage::getNegation() const
    {
        return (m_negation);
    }

}
