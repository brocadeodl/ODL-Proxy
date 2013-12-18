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
#include "Ipadm/Local/IpadmIpaddressConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIpaddressconfigMessage::IpadmIpaddressconfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPADDRESSCONFIG)
    {
    }

    IpadmIpaddressconfigMessage::IpadmIpaddressconfigMessage (const string &switchid,const string &ip_address)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPADDRESSCONFIG),
        m_switchid    (switchid),
        m_ip_address    (ip_address)
    {
    }

    IpadmIpaddressconfigMessage::~IpadmIpaddressconfigMessage ()
    {
    }

    void  IpadmIpaddressconfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeString(&m_ip_address,"ip_address"));
    }

    void  IpadmIpaddressconfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIpaddressconfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIpaddressconfigMessage::setIp_address(const string &ip_address)
    {
        m_ip_address  =  ip_address;
    }

    string  IpadmIpaddressconfigMessage::getIp_address() const
    {
        return (m_ip_address);
    }

}
