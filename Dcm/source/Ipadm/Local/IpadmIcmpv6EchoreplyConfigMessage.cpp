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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Min Zhang
 **************************************************************************/

#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmIcmpv6EchoreplyConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIcmpv6EchoreplyConfigMessage::IpadmIcmpv6EchoreplyConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPV6ECHOREPLYCONFIG)
    {
    }

    IpadmIcmpv6EchoreplyConfigMessage::IpadmIcmpv6EchoreplyConfigMessage (const string &switchid,const bool &echoreply)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPV6ECHOREPLYCONFIG),
        m_switchid    (switchid),
        m_echoreply   (echoreply)
    {
    }

    IpadmIcmpv6EchoreplyConfigMessage::~IpadmIcmpv6EchoreplyConfigMessage ()
    {
    }

    void  IpadmIcmpv6EchoreplyConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeBool(&m_echoreply,"echoreply"));
    }

    void  IpadmIcmpv6EchoreplyConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIcmpv6EchoreplyConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIcmpv6EchoreplyConfigMessage::setEchoreply(const bool &echoreply)
    {
        m_echoreply = echoreply;
    }

    bool IpadmIcmpv6EchoreplyConfigMessage::getEchoreply() const
    {
        return (m_echoreply);
    }

}
