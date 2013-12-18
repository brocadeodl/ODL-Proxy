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
#include "Ipadm/Local/IpadmIcmpv6RatelimitingConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIcmpv6RatelimitingConfigMessage::IpadmIcmpv6RatelimitingConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPV6RATELIMITINGCONFIG)
    {
    }

    IpadmIcmpv6RatelimitingConfigMessage::IpadmIcmpv6RatelimitingConfigMessage (const string &switchid,const unsigned int &ratelimiting)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPV6RATELIMITINGCONFIG),
        m_switchid    (switchid),
        m_ratelimiting   (ratelimiting)
    {
    }

    IpadmIcmpv6RatelimitingConfigMessage::~IpadmIcmpv6RatelimitingConfigMessage ()
    {
    }

    void  IpadmIcmpv6RatelimitingConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeUI32(&m_ratelimiting,"ratelimiting"));
    }

    void  IpadmIcmpv6RatelimitingConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIcmpv6RatelimitingConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIcmpv6RatelimitingConfigMessage::setRatelimiting(const unsigned int &ratelimiting)
    {
        m_ratelimiting = ratelimiting;
    }

	unsigned int IpadmIcmpv6RatelimitingConfigMessage::getRatelimiting() const
    {
        return (m_ratelimiting);
    }

}
