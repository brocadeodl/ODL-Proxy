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

#include "ClientInterface/Sflow/SflowClientCreateSflowGlobalConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

	SflowClientCreateSflowGlobalConfigSPMessage::SflowClientCreateSflowGlobalConfigSPMessage ()
		: DcmManagementInterfaceMessage (SFLOWCLIENTCREATESFLOWGLOBALCONFIGSP),
		m_enable			(SFLOW_DEFAULT_ENABLE),
		m_collector			(SFLOW_DEFAULT_COLLECTOR),
		m_portNumber        (SFLOW_DEFAULT_COLLECTOR_PORT),
		m_pollingInterval	(SFLOW_DEFAULT_POLLING_INTERVAL),
		m_sampleRate		(SFLOW_DEFAULT_SAMPLE_RATE),
		m_opCode			(SFLOW_OUT_OF_RANGE_VALUE)
    {
    }

	SflowClientCreateSflowGlobalConfigSPMessage::SflowClientCreateSflowGlobalConfigSPMessage (const bool &enable,const IpVxAddress &collector,const UI32 &portNumber,const UI32 &pollingInterval,const UI32 &sampleRate,const UI32 &opCode)
		: DcmManagementInterfaceMessage (SFLOWCLIENTCREATESFLOWGLOBALCONFIGSP),
		m_enable			(enable),
		m_collector			(collector),
		m_portNumber        (portNumber),
		m_pollingInterval	(pollingInterval),
		m_sampleRate		(sampleRate),
		m_opCode			(opCode)
    {
    }

    SflowClientCreateSflowGlobalConfigSPMessage::~SflowClientCreateSflowGlobalConfigSPMessage ()
    {
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeIpVxAddress(&m_collector,"collector"));
        addSerializableAttribute (new AttributeUI32(&m_portNumber,"portNumber"));
        addSerializableAttribute (new AttributeUI32(&m_collectorAddressType,"collectorAddressType"));
        addSerializableAttribute (new AttributeUI32(&m_pollingInterval,"pollingInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sampleRate,"sampleRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  SflowClientCreateSflowGlobalConfigSPMessage::getEnable() const
    {
        return (m_enable);
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setCollector(const IpVxAddress &collector)
    {
        m_collector  =  collector;
    }

    IpVxAddress  SflowClientCreateSflowGlobalConfigSPMessage::getCollector() const
    {
        return (m_collector);
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setPortNumber(const UI32 &portNumber)
    {
        m_portNumber  =  portNumber;
    }

    UI32  SflowClientCreateSflowGlobalConfigSPMessage::getPortNumber() const
    {
        return (m_portNumber);
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setCollectorAddressType(const UI32 &type)
    {
    	m_collectorAddressType  =  type;
    }

    UI32  SflowClientCreateSflowGlobalConfigSPMessage::getCollectorAddressType() const
    {
        return (m_collectorAddressType);
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval  =  pollingInterval;
    }

    UI32  SflowClientCreateSflowGlobalConfigSPMessage::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate  =  sampleRate;
    }

    UI32  SflowClientCreateSflowGlobalConfigSPMessage::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  SflowClientCreateSflowGlobalConfigSPMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowClientCreateSflowGlobalConfigSPMessage::getOpCode() const
    {
        return (m_opCode);
    }

    const void *SflowClientCreateSflowGlobalConfigSPMessage::getCStructureForInputs ()
    {
        string str_ipaddr;
	sflow_global_config_msg_t *pInput = (sflow_global_config_msg_ *)malloc (sizeof (struct sflow_global_config_msg_));

	str_ipaddr = m_collector.toString();

        strncpy (pInput->collector, str_ipaddr.c_str (), 64);
        pInput->collector[63]='\0';
        pInput->collector_port=m_portNumber;
        pInput->collector_type=m_collectorAddressType;
        pInput->enable= m_enable;
        pInput->polling_interval= m_pollingInterval;
        pInput->sample_rate= m_sampleRate;
        pInput->opcode= m_opCode;

        return (pInput);
    }

    void SflowClientCreateSflowGlobalConfigSPMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
