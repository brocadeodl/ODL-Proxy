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

#include "SystemMonitor/Global/RelayIpDelMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    RelayIpDelMessage::RelayIpDelMessage ()
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), RELAYIPDEL)
    {
    }

    RelayIpDelMessage::RelayIpDelMessage (const string &domainname, const HostUC &hostip)
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), RELAYIPDEL),
        m_domainname    (domainname),
		m_hostip		(hostip)
    {
    }

    RelayIpDelMessage::~RelayIpDelMessage ()
    {
    }

    void  RelayIpDelMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_domainname,"relaydomain"));
		addSerializableAttribute (new AttributeHostUC(&m_hostip,false, "relayhostip"));
    }

    void  RelayIpDelMessage::setDomainname(const string &domainname)
    {
        m_domainname  =  domainname;
    }

    string  RelayIpDelMessage::getDomainname() const
    {
        return (m_domainname);
    }
    void  RelayIpDelMessage::setHostip(const HostUC &hostip)
    {
        m_hostip = hostip;
    }       

    HostUC  RelayIpDelMessage::getHostip () const
    {
        return ( m_hostip);
    }

}
