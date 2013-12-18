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

/****************************************************************************
 *  Copyright (C) 2011 Brocade Communications Systems, Inc.                 *
 *  All rights reserved.                                                    *
 *                                                                          *
 *  Author : Mayank Maheshwari                                              *
 *                                                                          *
 ****************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmLocalMacRebalanceMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{
    NsmLocalMacRebalanceMessage::NsmLocalMacRebalanceMessage()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), VLAGEXMREBALANCE) {
        m_poIfIndex = 0;
        m_locationId = 0;
    }

    NsmLocalMacRebalanceMessage::NsmLocalMacRebalanceMessage(const UI32 &poIfIndex, const UI32 &loc_id)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), VLAGEXMREBALANCE),
        m_poIfIndex(poIfIndex), m_locationId(loc_id) {
    }

    NsmLocalMacRebalanceMessage::~NsmLocalMacRebalanceMessage() {
    }

    void NsmLocalMacRebalanceMessage::setupAttributesForSerialization() {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
    }

    UI32 NsmLocalMacRebalanceMessage::getPoIfIndex() const {
        return (m_poIfIndex);
    }

    void NsmLocalMacRebalanceMessage::setPoIfIndex(const UI32 &poIfIndex) {
        m_poIfIndex = poIfIndex;
    }

    UI32 NsmLocalMacRebalanceMessage::getDestLocationId() const {
        return (m_locationId);
    }

    void NsmLocalMacRebalanceMessage::setDestLocationId(const UI32 &loc_id) {
        m_locationId = loc_id;
    }
}
