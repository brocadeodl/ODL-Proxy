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
#include "Ipadm/Local/IpadmGatewayConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmGatewayconfigMessage::IpadmGatewayconfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMGATEWAYCONFIG)
    {
    }

    IpadmGatewayconfigMessage::IpadmGatewayconfigMessage (const string &switchid,const IpV4Address &gateway)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMGATEWAYCONFIG),
        m_switchid    (switchid),
        m_gateway    (gateway)
    {
    }

    IpadmGatewayconfigMessage::~IpadmGatewayconfigMessage ()
    {
    }

    void  IpadmGatewayconfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeIpV4Address(&m_gateway,"gateway"));
    }

    void  IpadmGatewayconfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmGatewayconfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmGatewayconfigMessage::setGateway(const IpV4Address &gateway)
    {
        m_gateway  =  gateway;
    }

    IpV4Address  IpadmGatewayconfigMessage::getGateway() const
    {
        return (m_gateway);
    }

}
