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

#include "Nsm/Global/NsmGlobalDeletePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalDeletePoIntfMessage::NsmGlobalDeletePoIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEPOINTF)
    {
		m_poIfIndex = 0;
    }

    NsmGlobalDeletePoIntfMessage::NsmGlobalDeletePoIntfMessage (const string &poId,const UI32 &poIfIndex)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALDELETEPOINTF),
        m_poId    (poId),
        m_poIfIndex    (poIfIndex)
    {
    }

    NsmGlobalDeletePoIntfMessage::~NsmGlobalDeletePoIntfMessage ()
    {
    }

    void  NsmGlobalDeletePoIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
    }

    void  NsmGlobalDeletePoIntfMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmGlobalDeletePoIntfMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmGlobalDeletePoIntfMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmGlobalDeletePoIntfMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

}
