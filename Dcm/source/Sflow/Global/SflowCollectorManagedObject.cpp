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

/*
 *  SflowCollectorManagedObject.cpp

 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved. 

 *  Created on: Aug 14, 2012
 *  Author: sunniram
 */

#include "Sflow/Global/SflowCollectorManagedObject.h"
#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

    SflowCollectorManagedObject::SflowCollectorManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager)
        : PrismElement  (pSflowGlobalObjectManager),
        PrismPersistableObject (SflowCollectorManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSflowGlobalObjectManager),
        DcmManagedObject (pSflowGlobalObjectManager),
		m_collector			(SFLOW_DEFAULT_COLLECTOR),
		m_portNumber         (SFLOW_DEFAULT_COLLECTOR_PORT)
    {
    }

    SflowCollectorManagedObject::SflowCollectorManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager,const IpVxAddress &collector,const UI32 &portNumber)
        : PrismElement  (pSflowGlobalObjectManager),
        PrismPersistableObject (SflowCollectorManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSflowGlobalObjectManager),
        DcmManagedObject (pSflowGlobalObjectManager),
        m_collector    (collector),
        m_portNumber    (portNumber)
    {
    }

    SflowCollectorManagedObject::~SflowCollectorManagedObject ()
    {
    }

    string  SflowCollectorManagedObject::getClassName()
    {
        return ("SflowCollectorManagedObject");
    }


    void SflowCollectorManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("ipAddress");
        keyName.push_back("portNumber");
        setUserDefinedKeyCombination (keyName);
    }

    void  SflowCollectorManagedObject::setupAttributesForPersistence()
    {
        IpVxAddress defaultIp("");
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpVxAddress(&m_collector,defaultIp,"ipAddress", sflow_collector_ip_address));
        addPersistableAttribute (new AttributeUI32(&m_portNumber, SFLOW_DEFAULT_COLLECTOR_PORT, "portNumber", sflow_collector_port_number));

    }

    void  SflowCollectorManagedObject::setupAttributesForCreate()
    {
        IpVxAddress defaultIp("");
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpVxAddress(&m_collector,defaultIp,"ipAddress", sflow_collector_ip_address));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_portNumber, SFLOW_DEFAULT_COLLECTOR_PORT, "portNumber", sflow_collector_port_number));
    }

    void  SflowCollectorManagedObject::setCollectorIP(const IpVxAddress &collector)
    {
        m_collector  =  collector;
    }

    IpVxAddress  SflowCollectorManagedObject::getCollectorIP() const
    {
        return (m_collector);
    }

    void  SflowCollectorManagedObject::setCollectorPort(const UI32 &portNumber)
    {
        m_portNumber  =  portNumber;
    }

    UI32  SflowCollectorManagedObject::getCollectorPort() const
    {
        return (m_portNumber);
    }



}




