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

#include "SystemMonitor/Local/RelayIpLocalConfigMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

namespace DcmNs
{

    RelayIpLocalConfigMessage::RelayIpLocalConfigMessage ()
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), RELAYIPLOCALCONFIG)
    {
    }

    RelayIpLocalConfigMessage::RelayIpLocalConfigMessage (const string &domainname, const HostUC &hostip)
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), RELAYIPLOCALCONFIG),
        m_domainname    (domainname),
		m_hostip		(hostip)
    {
    }

    RelayIpLocalConfigMessage::~RelayIpLocalConfigMessage ()
    {
    }

    void  RelayIpLocalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_domainname,"relaydomain"));
		addSerializableAttribute (new AttributeHostUC(&m_hostip,false,"relayhostip"));
    }

    void  RelayIpLocalConfigMessage::setDomainname(const string &domainname)
    {
        m_domainname  =  domainname;
    }

    string  RelayIpLocalConfigMessage::getDomainname() const
    {
        return (m_domainname);
    }
    void  RelayIpLocalConfigMessage::setHostip(const HostUC &hostip)
    {
        m_hostip = hostip;
    }       

    HostUC  RelayIpLocalConfigMessage::getHostip () const
    {
        return ( m_hostip);
    }

}
