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

#include "SystemMonitor/Global/RelayIpManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "vcs.h"
namespace DcmNs
{

    RelayIpManagedObject::RelayIpManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (RelayIpManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager)
    {
    }

    RelayIpManagedObject::RelayIpManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const HostUC &hostip, const string &domainname
		)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (RelayIpManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager),
        m_hostip    (hostip),
        m_domainname    (domainname)
    {
    }

    RelayIpManagedObject::~RelayIpManagedObject ()
    {
    }

    string  RelayIpManagedObject::getClassName()
    {
        return ("RelayIpManagedObject");
    }

    void  RelayIpManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
		HostUC defaultHostUC("0.0.0.0");
        addPersistableAttribute (new AttributeHostUC(&m_hostip,true, true, defaultHostUC, "relayhostip", system_monitor_host_ip));
        addPersistableAttribute (new AttributeString(&m_domainname,"relaydomainname", system_monitor_domain_name));
    }

    void  RelayIpManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
		HostUC defaultHostUC("0.0.0.0");
        addPersistableAttributeForCreate  (new AttributeHostUC(&m_hostip,true, true, defaultHostUC, "relayhostip", system_monitor_host_ip));
        addPersistableAttributeForCreate  (new AttributeString(&m_domainname,"relaydomainname", system_monitor_domain_name));
		vector<string > keyList1;
        keyList1.push_back ("relayhostip");
        setUserDefinedKeyCombination (keyList1);
    }

    void  RelayIpManagedObject::setDomainname(const string &domainname)
    {
        m_domainname  =  domainname;
    }

    string  RelayIpManagedObject::getDomainname() const
    {
        return (m_domainname);
    }

    HostUC  RelayIpManagedObject::getHostip() const
    {
        return (m_hostip);
    }

    void  RelayIpManagedObject::setHostip(const HostUC &hostip)
    {
        m_hostip  =  hostip;
    }
}
