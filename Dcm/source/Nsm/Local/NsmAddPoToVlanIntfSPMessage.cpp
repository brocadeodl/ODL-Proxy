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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmAddPoToVlanIntfSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmAddPoToVlanIntfSPMessage::NsmAddPoToVlanIntfSPMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMADDPOTOVLANINTFSP)
    {
		m_poIfIndex = 0;
		m_tagStatus = 0;
    }

    NsmAddPoToVlanIntfSPMessage::NsmAddPoToVlanIntfSPMessage (const UI64 &poIfIndex,const string &vlanName,const UI32 &tagStatus)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMADDPOTOVLANINTFSP),
        m_poIfIndex    (poIfIndex),
        m_vlanName    (vlanName),
        m_tagStatus    (tagStatus)
    {
    }

    NsmAddPoToVlanIntfSPMessage::~NsmAddPoToVlanIntfSPMessage ()
    {
    }

    void  NsmAddPoToVlanIntfSPMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI64(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_tagStatus,"tagStatus"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
    }

    void  NsmAddPoToVlanIntfSPMessage::setPoIfIndex(const UI64 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI64  NsmAddPoToVlanIntfSPMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmAddPoToVlanIntfSPMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmAddPoToVlanIntfSPMessage::getVlanName() const
    {
        return ("m_vlanName");
    }

    void  NsmAddPoToVlanIntfSPMessage::setTagStatus(const UI32 &tagStatus)
    {
        m_tagStatus  =  tagStatus;
    }

    UI32  NsmAddPoToVlanIntfSPMessage::getTagStatus() const
    {
        return (m_tagStatus);
    }

}
