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
#include "Ipadm/Local/IpadmMaskConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmMaskconfigMessage::IpadmMaskconfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMMASKCONFIG)
    {
    }

    IpadmMaskconfigMessage::IpadmMaskconfigMessage (const string &switchid,const IpV4AddressNetworkMask &mask)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMMASKCONFIG),
        m_switchid    (switchid),
        m_mask    (mask)
    {
    }

    IpadmMaskconfigMessage::~IpadmMaskconfigMessage ()
    {
    }

    void  IpadmMaskconfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_mask,"mask"));
    }

    void  IpadmMaskconfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmMaskconfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmMaskconfigMessage::setMask(const IpV4AddressNetworkMask &mask)
    {
        m_mask  =  mask;
    }

    IpV4AddressNetworkMask  IpadmMaskconfigMessage::getMask() const
    {
        return (m_mask);
    }

}
