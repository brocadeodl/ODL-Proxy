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
#include "Nsm/Local/NsmDeleteVlanIntfSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmDeleteVlanIntfSPMessage::NsmDeleteVlanIntfSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMDELETEVLANINTFSP)
    {
		m_vlanIfIndex = 0;
        isDBUpdateReq = false;
    }

    NsmDeleteVlanIntfSPMessage::NsmDeleteVlanIntfSPMessage (const UI32 &vlanId,const UI32 &vlanIfIndex,const ObjectId &vlanObjectId)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMDELETEVLANINTFSP),
        m_vlanId    (vlanId),
        m_vlanIfIndex    (vlanIfIndex),
        m_vlanObjectId   (vlanObjectId)
    {
        isDBUpdateReq = false;
    }

    NsmDeleteVlanIntfSPMessage::~NsmDeleteVlanIntfSPMessage ()
    {
    }

    void  NsmDeleteVlanIntfSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeObjectId(&m_vlanObjectId,"vlanObjectId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeBool(&isDBUpdateReq, "isDBUpdateReq"));
    }

    void  NsmDeleteVlanIntfSPMessage::setVlanObjectID(const ObjectId &vlanObjectId)
    {
        m_vlanObjectId  =  vlanObjectId;
    }

    ObjectId  NsmDeleteVlanIntfSPMessage::getVlanObjectID() const
    {
        return (m_vlanObjectId);
    }

    void  NsmDeleteVlanIntfSPMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmDeleteVlanIntfSPMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmDeleteVlanIntfSPMessage::setVlanIfIndex(const UI32 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI32  NsmDeleteVlanIntfSPMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    bool NsmDeleteVlanIntfSPMessage::isDBUpdateRequest() const
    {
        return isDBUpdateReq;
    }

    void NsmDeleteVlanIntfSPMessage::setDBUpdateRequest(const bool &updateDB)
    {
        isDBUpdateReq = updateDB;
    }
}
