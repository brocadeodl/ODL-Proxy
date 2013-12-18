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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#include "Sflow/Local/SflowLocalObjectManager.h"
#include "Sflow/Local/SflowCreateSflowGlobalConfigSPMessage.h"
#include "Sflow/Local/SflowTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

    SflowCreateSflowGlobalConfigSPMessage::SflowCreateSflowGlobalConfigSPMessage ()
        : PrismMessage (SflowLocalObjectManager::getPrismServiceId (),SFLOWCREATESFLOWGLOBALCONFIGSP)
    {
        m_collector.fromString(SFLOW_DEFAULT_COLLECTOR);
        m_enable = SFLOW_DEFAULT_ENABLE;
        m_portNumber = SFLOW_DEFAULT_COLLECTOR_PORT;
        m_pollingInterval = SFLOW_DEFAULT_POLLING_INTERVAL;
        m_sampleRate = SFLOW_DEFAULT_SAMPLE_RATE;
        m_opCode = SFLOW_OUT_OF_RANGE_VALUE;
        m_collectorAddressType = SFLOW_COLLECTOR_ADDRESSTYPE_IPV4;
    }

    SflowCreateSflowGlobalConfigSPMessage::SflowCreateSflowGlobalConfigSPMessage (const bool &enable,const IpVxAddress &collector,const UI32 &portNumber,const UI32 &pollingInterval,const UI32 &sampleRate,const UI32 &opCode)
        : PrismMessage (SflowLocalObjectManager::getPrismServiceId (),SFLOWCREATESFLOWGLOBALCONFIGSP),
        m_enable    (enable),
        m_collector    (collector),
        m_portNumber    (portNumber),
        m_pollingInterval    (pollingInterval),
        m_sampleRate    (sampleRate),
        m_opCode    (opCode)
    {
    }

    SflowCreateSflowGlobalConfigSPMessage::~SflowCreateSflowGlobalConfigSPMessage ()
    {
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeIpVxAddress(&m_collector,"collector"));
        addSerializableAttribute (new AttributeUI32(&m_portNumber,"portNumber"));
        addSerializableAttribute (new AttributeUI32(&m_collectorAddressType,"collectorAddressType"));
        addSerializableAttribute (new AttributeUI32(&m_pollingInterval,"pollingInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sampleRate,"sampleRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  SflowCreateSflowGlobalConfigSPMessage::getEnable() const
    {
        return (m_enable);
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setCollector(const IpVxAddress &collector)
    {
        m_collector  =  collector;
    }

    IpVxAddress  SflowCreateSflowGlobalConfigSPMessage::getCollector() const
    {
        return (m_collector);
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setPortNumber(const UI32 &portNumber)
    {
        m_portNumber  =  portNumber;
    }

    UI32  SflowCreateSflowGlobalConfigSPMessage::getPortNumber() const
    {
        return (m_portNumber);
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setCollectorAddressType(const UI32 &type)
    {
    	m_collectorAddressType  =  type;
    }

    UI32  SflowCreateSflowGlobalConfigSPMessage::getCollectorAddressType() const
    {
        return (m_collectorAddressType);
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval  =  pollingInterval;
    }

    UI32  SflowCreateSflowGlobalConfigSPMessage::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate  =  sampleRate;
    }

    UI32  SflowCreateSflowGlobalConfigSPMessage::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  SflowCreateSflowGlobalConfigSPMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowCreateSflowGlobalConfigSPMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
