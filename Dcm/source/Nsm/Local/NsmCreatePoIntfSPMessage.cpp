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
#include "Nsm/Local/NsmCreatePoIntfSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmCreatePoIntfSPMessage::NsmCreatePoIntfSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),NSMCREATEPOINTFSP)
    {
		m_poIfIndex = 0;
    }

    NsmCreatePoIntfSPMessage::NsmCreatePoIntfSPMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const string &desc)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),NSMCREATEPOINTFSP),
        m_poId    (poId),
        m_poName    (poName),
        m_poIfIndex    (poIfIndex),
        m_desc    (desc)
    {
    }

    NsmCreatePoIntfSPMessage::~NsmCreatePoIntfSPMessage ()
    {
    }

    void  NsmCreatePoIntfSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
    }

    void  NsmCreatePoIntfSPMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmCreatePoIntfSPMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmCreatePoIntfSPMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmCreatePoIntfSPMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmCreatePoIntfSPMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmCreatePoIntfSPMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmCreatePoIntfSPMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmCreatePoIntfSPMessage::getDesc() const
    {
        return (m_desc);
    }

}
