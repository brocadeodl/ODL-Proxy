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
#include "Ipadm/Local/IpadmIcmpUnreachableConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIcmpUnreachableConfigMessage::IpadmIcmpUnreachableConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPUNREACHABLECONFIG)
    {
    }

    IpadmIcmpUnreachableConfigMessage::IpadmIcmpUnreachableConfigMessage (const string &switchid,const bool &unreachable)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMICMPUNREACHABLECONFIG),
        m_switchid    (switchid),
        m_unreachable (unreachable)
    {
    }

    IpadmIcmpUnreachableConfigMessage::~IpadmIcmpUnreachableConfigMessage ()
    {
    }

    void  IpadmIcmpUnreachableConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeBool(&m_unreachable,"unreachable"));
    }

    void  IpadmIcmpUnreachableConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIcmpUnreachableConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIcmpUnreachableConfigMessage::setUnreachable(const bool &unreachable)
    {
        m_unreachable = unreachable;
    }

    bool IpadmIcmpUnreachableConfigMessage::getUnreachable() const
    {
        return (m_unreachable);
    }

}
