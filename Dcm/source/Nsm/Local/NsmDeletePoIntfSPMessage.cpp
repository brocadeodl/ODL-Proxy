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
#include "Nsm/Local/NsmDeletePoIntfSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmDeletePoIntfSPMessage::NsmDeletePoIntfSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMDELETEPOINTFSP)
    {
		m_poIfIndex = 0;
        isDBUpdateReq = false;
    }

    NsmDeletePoIntfSPMessage::NsmDeletePoIntfSPMessage (const string &poId,const UI32 &poIfIndex)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMDELETEPOINTFSP),
        m_poId    (poId),
        m_poIfIndex    (poIfIndex)
    {
        isDBUpdateReq = false;
    }

    NsmDeletePoIntfSPMessage::~NsmDeletePoIntfSPMessage ()
    {
    }

    void  NsmDeletePoIntfSPMessage::setupAttributesForSerialization()
    {
        PrismMessage ::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeBool(&isDBUpdateReq, "isDBUpdateReq"));
    }

    void  NsmDeletePoIntfSPMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmDeletePoIntfSPMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmDeletePoIntfSPMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmDeletePoIntfSPMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    bool NsmDeletePoIntfSPMessage::isDBUpdateRequest() const
    {
        return isDBUpdateReq;
    }

    void NsmDeletePoIntfSPMessage::setDBUpdateRequest(const bool &updateDB)
    {
        isDBUpdateReq = updateDB;
    }

}
