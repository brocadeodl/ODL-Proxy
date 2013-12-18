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
#include "Ipadm/Local/IpadmIcmpv6UnreachableConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIcmpv6UnreachableConfigMessage::IpadmIcmpv6UnreachableConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPV6UNREACHABLECONFIG)
    {
    }

    IpadmIcmpv6UnreachableConfigMessage::IpadmIcmpv6UnreachableConfigMessage (const string &switchid,const bool &unreachable)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPV6UNREACHABLECONFIG),
        m_switchid    (switchid),
        m_unreachable (unreachable)
    {
    }

    IpadmIcmpv6UnreachableConfigMessage::~IpadmIcmpv6UnreachableConfigMessage ()
    {
    }

    void  IpadmIcmpv6UnreachableConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeBool(&m_unreachable,"unreachable"));
    }

    void  IpadmIcmpv6UnreachableConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIcmpv6UnreachableConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIcmpv6UnreachableConfigMessage::setUnreachable(const bool &unreachable)
    {
        m_unreachable = unreachable;
    }

    bool IpadmIcmpv6UnreachableConfigMessage::getUnreachable() const
    {
        return (m_unreachable);
    }

}
