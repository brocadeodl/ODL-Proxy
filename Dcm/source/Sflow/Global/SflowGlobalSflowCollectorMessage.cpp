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
 *  SflowGlobalSflowCollectorMessage.cpp

 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved. 

 *  Created on: Aug 14, 2012
 *  Author: sunniram
 */

#include "Sflow/Global/SflowGlobalSflowCollectorMessage.h"
#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "Sflow/Global/SflowGlobalTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

    SflowGlobalSflowCollectorMessage::SflowGlobalSflowCollectorMessage ()
        : ManagementInterfaceMessage (SflowGlobalObjectManager::getClassName (),SFLOWGLOBALSFLOWCOLLECTOR)
    {
        m_collector.fromString(SFLOW_DEFAULT_COLLECTOR);
        m_portNumber = SFLOW_DEFAULT_COLLECTOR_PORT;
        m_opCode = SFLOW_OUT_OF_RANGE_VALUE;
    }

    SflowGlobalSflowCollectorMessage::SflowGlobalSflowCollectorMessage (const IpVxAddress &collector,const UI32 &portNumber,const UI32 &opCode)
        : ManagementInterfaceMessage (SflowGlobalObjectManager::getClassName (),SFLOWGLOBALSFLOWCOLLECTOR),
        m_collector    (collector),
        m_portNumber (portNumber),
        m_opCode    (opCode)
    {
    }

    SflowGlobalSflowCollectorMessage::~SflowGlobalSflowCollectorMessage ()
    {
    }

    void  SflowGlobalSflowCollectorMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeIpVxAddress(&m_collector,"collector"));
        addSerializableAttribute (new AttributeUI32(&m_portNumber, SFLOW_DEFAULT_COLLECTOR_PORT, "portNumber"));
        addSerializableAttribute (new AttributeUI32(&m_collectorAddressType,"collectorAddressType"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }


    void  SflowGlobalSflowCollectorMessage::setCollectorIP(const IpVxAddress &collector)
    {
        m_collector  =  collector;
    }

    IpVxAddress  SflowGlobalSflowCollectorMessage::getCollectorIP() const
    {
        return (m_collector);
    }

    void  SflowGlobalSflowCollectorMessage::setCollectorPort(const UI32 &portNumber)
    {
        m_portNumber  =  portNumber;
    }

    UI32  SflowGlobalSflowCollectorMessage::getCollectorPort() const
    {
        return (m_portNumber);
    }

    void  SflowGlobalSflowCollectorMessage::setCollectorAddressType(const UI32 &type)
    {
    	m_collectorAddressType  =  type;
    }

    UI32  SflowGlobalSflowCollectorMessage::getCollectorAddressType() const
    {
        return (m_collectorAddressType);
    }

    void  SflowGlobalSflowCollectorMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowGlobalSflowCollectorMessage::getOpCode() const
    {
        return (m_opCode);
    }

}



