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
 *   .Author : abhaskar                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsMrtrVlanLocalConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmUpdateIgmpsMrtrVlanLocalConfigMessage::NsmUpdateIgmpsMrtrVlanLocalConfigMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEIGMPSMRTRVLANLOCALCONFIG)
    {
		m_ifType = 0;
		m_negation = false;
		m_vlanId = 1;
		m_vlanIfIndex = 0;
    }

    NsmUpdateIgmpsMrtrVlanLocalConfigMessage::NsmUpdateIgmpsMrtrVlanLocalConfigMessage (const UI32 &ifType,const string &ifName,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const bool &negation)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEIGMPSMRTRVLANLOCALCONFIG),
        m_ifType    (ifType),
        m_ifName    (ifName),
        m_vlanId    (vlanId),
        m_vlanIfName    (vlanIfName),
        m_vlanIfIndex    (vlanIfIndex),
        m_negation    (negation)
    {
    }

    NsmUpdateIgmpsMrtrVlanLocalConfigMessage::~NsmUpdateIgmpsMrtrVlanLocalConfigMessage ()
    {
    }

    void  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeString(&m_vlanIfName,"vlanIfName"));
        addSerializableAttribute (new AttributeUI64(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
    }

    void  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::setVlanIfName(const string &vlanIfName)
    {
        m_vlanIfName  =  vlanIfName;
    }

    string  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::getVlanIfName() const
    {
        return (m_vlanIfName);
    }

    void  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::setVlanIfIndex(const UI64 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI64  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::setNegation(const bool &negation)
    {
        m_negation  =  negation;
    }

    bool  NsmUpdateIgmpsMrtrVlanLocalConfigMessage::getNegation() const
    {
        return (m_negation);
    }

}
