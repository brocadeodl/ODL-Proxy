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
#include "Nsm/Local/NsmDeletePoToVlanIntfSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmDeletePoToVlanIntfSPMessage::NsmDeletePoToVlanIntfSPMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMDELETEPOTOVLANINTFSP)
    {
		m_poIfIndex = 0;
    }

    NsmDeletePoToVlanIntfSPMessage::NsmDeletePoToVlanIntfSPMessage (const UI64 &poIfIndex,const string &vlanName)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMDELETEPOTOVLANINTFSP),
        m_poIfIndex    (poIfIndex),
        m_vlanName    (vlanName)
    {
    }

    NsmDeletePoToVlanIntfSPMessage::~NsmDeletePoToVlanIntfSPMessage ()
    {
    }

    void  NsmDeletePoToVlanIntfSPMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI64(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
    }

    void  NsmDeletePoToVlanIntfSPMessage::setPoIfIndex(const UI64 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI64  NsmDeletePoToVlanIntfSPMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmDeletePoToVlanIntfSPMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmDeletePoToVlanIntfSPMessage::getVlanName() const
    {
        return ("m_vlanName");
    }

}
