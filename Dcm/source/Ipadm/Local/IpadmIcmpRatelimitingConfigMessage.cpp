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
#include "Ipadm/Local/IpadmIcmpRatelimitingConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIcmpRatelimitingConfigMessage::IpadmIcmpRatelimitingConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPRATELIMITINGCONFIG)
    {
    }

    IpadmIcmpRatelimitingConfigMessage::IpadmIcmpRatelimitingConfigMessage (const string &switchid,const unsigned int &ratelimiting)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPRATELIMITINGCONFIG),
        m_switchid    (switchid),
        m_ratelimiting   (ratelimiting)
    {
    }

    IpadmIcmpRatelimitingConfigMessage::~IpadmIcmpRatelimitingConfigMessage ()
    {
    }

    void  IpadmIcmpRatelimitingConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeUI32(&m_ratelimiting,"ratelimiting"));
    }

    void  IpadmIcmpRatelimitingConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIcmpRatelimitingConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIcmpRatelimitingConfigMessage::setRatelimiting(const unsigned int &ratelimiting)
    {
        m_ratelimiting = ratelimiting;
    }

	unsigned int IpadmIcmpRatelimitingConfigMessage::getRatelimiting() const
    {
        return (m_ratelimiting);
    }

}
