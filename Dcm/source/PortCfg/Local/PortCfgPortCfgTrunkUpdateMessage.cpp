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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgPortCfgTrunkUpdateMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgPortCfgTrunkUpdateMessage::PortCfgPortCfgTrunkUpdateMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGTRUNKUPDATE)
    {
    }

    PortCfgPortCfgTrunkUpdateMessage::PortCfgPortCfgTrunkUpdateMessage (const string &id,const bool &trunkEnable,const bool &isIdComplete)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGTRUNKUPDATE),
        m_id    (id),
        m_trunkEnable    (trunkEnable),
        m_isIdComplete    (isIdComplete)
    {
    }

    PortCfgPortCfgTrunkUpdateMessage::~PortCfgPortCfgTrunkUpdateMessage ()
    {
    }

    void  PortCfgPortCfgTrunkUpdateMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeBool(&m_trunkEnable,"trunkEnable"));
        addSerializableAttribute (new AttributeBool(&m_isIdComplete,"isIdComplete"));
    }

    void  PortCfgPortCfgTrunkUpdateMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PortCfgPortCfgTrunkUpdateMessage::getId() const
    {
        return (m_id);
    }

    void  PortCfgPortCfgTrunkUpdateMessage::setTrunkEnable(const bool &trunkEnable)
    {
        m_trunkEnable  =  trunkEnable;
    }

    bool  PortCfgPortCfgTrunkUpdateMessage::getTrunkEnable() const
    {
        return (m_trunkEnable);
    }

    void  PortCfgPortCfgTrunkUpdateMessage::setIsIdComplete(const bool &isIdComplete)
    {
        m_isIdComplete  =  isIdComplete;
    }

    bool  PortCfgPortCfgTrunkUpdateMessage::getIsIdComplete() const
    {
        return (m_isIdComplete);
    }

}
