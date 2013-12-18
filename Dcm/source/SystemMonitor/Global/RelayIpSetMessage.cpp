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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "SystemMonitor/Global/RelayIpSetMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    RelayIpSetMessage::RelayIpSetMessage ()
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), RELAYIPSET)
    {
    }

    RelayIpSetMessage::RelayIpSetMessage (const string &domainname, const HostUC &hostip)
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), RELAYIPSET),
        m_domainname    (domainname),
		m_hostip		(hostip)
    {
    }

    RelayIpSetMessage::~RelayIpSetMessage ()
    {
    }

    void  RelayIpSetMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_domainname,"relaydomain"));
		addSerializableAttribute (new AttributeHostUC(&m_hostip,false, "relayhostip"));
    }

    void  RelayIpSetMessage::setDomainname(const string &domainname)
    {
        m_domainname  =  domainname;
    }

    string  RelayIpSetMessage::getDomainname() const
    {
        return (m_domainname);
    }
    void  RelayIpSetMessage::setHostip(const HostUC &hostip)
    {
        m_hostip = hostip;
    }       

    HostUC  RelayIpSetMessage::getHostip () const
    {
        return ( m_hostip);
    }

}
