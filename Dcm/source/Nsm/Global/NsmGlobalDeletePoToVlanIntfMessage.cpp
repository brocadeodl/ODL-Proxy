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

#include "Nsm/Global/NsmGlobalDeletePoToVlanIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalDeletePoToVlanIntfMessage::NsmGlobalDeletePoToVlanIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEPOTOVLANINTF)
    {
		m_poIfIndex = 0;
    }

    NsmGlobalDeletePoToVlanIntfMessage::NsmGlobalDeletePoToVlanIntfMessage (const UI64 &poIfIndex,const string &vlanName)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEPOTOVLANINTF),
        m_poIfIndex    (poIfIndex),
        m_vlanName    (vlanName)
    {
    }

    NsmGlobalDeletePoToVlanIntfMessage::~NsmGlobalDeletePoToVlanIntfMessage ()
    {
    }

    void  NsmGlobalDeletePoToVlanIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI64(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeString(&m_vlanName,"vlanName"));
    }

    void  NsmGlobalDeletePoToVlanIntfMessage::setPoIfIndex(const UI64 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI64  NsmGlobalDeletePoToVlanIntfMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmGlobalDeletePoToVlanIntfMessage::setVlanName(const string &vlanName)
    {
        m_vlanName  =  vlanName;
    }

    string  NsmGlobalDeletePoToVlanIntfMessage::getVlanName() const
    {
        return ("m_vlanName");
    }

}
