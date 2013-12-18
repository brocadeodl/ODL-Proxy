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

#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmIpv6AutoconfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIpv6AutoconfigMessage::IpadmIpv6AutoconfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6AUTOCONFIG)
    {
    }

    IpadmIpv6AutoconfigMessage::IpadmIpv6AutoconfigMessage (const string &switchid,const bool &autoconfig)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6AUTOCONFIG),
        m_switchid    (switchid),
        m_autoconfig    (autoconfig)
    {
    }

    IpadmIpv6AutoconfigMessage::~IpadmIpv6AutoconfigMessage ()
    {
    }

    void  IpadmIpv6AutoconfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeBool(&m_autoconfig,"autoconfig"));
    }

    void  IpadmIpv6AutoconfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIpv6AutoconfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIpv6AutoconfigMessage::setAutoconfig(const bool &autoconfig)
    {
        m_autoconfig  =  autoconfig;
    }

    bool IpadmIpv6AutoconfigMessage::getAutoconfig() const
    {
        return (m_autoconfig);
    }

}
