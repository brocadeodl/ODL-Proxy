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
 *   Author : cshah                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmLinkSpeedConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmLinkSpeedConfigMessage::IpadmLinkSpeedConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMLINKSPEEDCONFIG)
    {
    }

    IpadmLinkSpeedConfigMessage::IpadmLinkSpeedConfigMessage (const string &switchid, const LinkSpeed &speed)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMLINKSPEEDCONFIG),
        m_switchid    (switchid),
        m_link_speed  (speed)
    {
    }

    IpadmLinkSpeedConfigMessage::~IpadmLinkSpeedConfigMessage ()
    {
    }

    void  IpadmLinkSpeedConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_link_speed),"link_speed"));
    }

    void  IpadmLinkSpeedConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmLinkSpeedConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmLinkSpeedConfigMessage::setLinkSpeed(const LinkSpeed &speed)
    {
        m_link_speed  =  speed;
    }

    LinkSpeed IpadmLinkSpeedConfigMessage::getLinkSpeed() const
    {
        return (m_link_speed);
    }

}
