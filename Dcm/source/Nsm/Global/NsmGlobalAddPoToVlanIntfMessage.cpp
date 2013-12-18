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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalAddPoToVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalAddPoToVlanIntfMessage::NsmGlobalAddPoToVlanIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALADDPOTOVLANINTF)
    {
		m_poIfIndex = 0;
		m_tagStatus = 0;
    }

    NsmGlobalAddPoToVlanIntfMessage::NsmGlobalAddPoToVlanIntfMessage (const UI64 &poIfIndex,const string &vlanName,const UI32 &tagStatus)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALADDPOTOVLANINTF),
        m_poIfIndex    (poIfIndex),
        m_vlanName    (vlanName),
        m_tagStatus    (tagStatus)
    {
    }

    NsmGlobalAddPoToVlanIntfMessage::~NsmGlobalAddPoToVlanIntfMessage ()
    {
    }

    void  NsmGlobalAddPoToVlanIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI64(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_tagStatus,"tagStatus"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
    }

    void  NsmGlobalAddPoToVlanIntfMessage::setPoIfIndex(const UI64 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI64  NsmGlobalAddPoToVlanIntfMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmGlobalAddPoToVlanIntfMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmGlobalAddPoToVlanIntfMessage::getVlanName() const
    {
        return ("m_vlanName");
    }

    void  NsmGlobalAddPoToVlanIntfMessage::setTagStatus(const UI32 &tagStatus)
    {
        m_tagStatus  =  tagStatus;
    }

    UI32  NsmGlobalAddPoToVlanIntfMessage::getTagStatus() const
    {
        return (m_tagStatus);
    }

}
