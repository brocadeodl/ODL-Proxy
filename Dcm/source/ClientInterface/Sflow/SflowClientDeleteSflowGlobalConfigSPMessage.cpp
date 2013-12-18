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
#include "ClientInterface/Sflow/SflowClientDeleteSflowGlobalConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

	SflowClientDeleteSflowGlobalConfigSPMessage::SflowClientDeleteSflowGlobalConfigSPMessage ()
		: DcmManagementInterfaceMessage (SFLOWCLIENTDELETESFLOWGLOBALCONFIGSP),
		m_enable			(SFLOW_DEFAULT_ENABLE),
		m_collector			(SFLOW_DEFAULT_COLLECTOR),
		m_pollingInterval	(SFLOW_DEFAULT_POLLING_INTERVAL),
		m_sampleRate		(SFLOW_DEFAULT_SAMPLE_RATE),
		m_opCode			(SFLOW_OUT_OF_RANGE_VALUE)
    {
    }

	SflowClientDeleteSflowGlobalConfigSPMessage::SflowClientDeleteSflowGlobalConfigSPMessage (const bool &enable,const IpVxAddress &collector,const UI32 &pollingInterval,const UI32 &sampleRate,const UI32 &opCode)
	: DcmManagementInterfaceMessage (SFLOWCLIENTDELETESFLOWGLOBALCONFIGSP),
		m_enable			(enable),
		m_collector			(collector),
		m_pollingInterval	(pollingInterval),
		m_sampleRate		(sampleRate),
		m_opCode			(opCode)
    {
    }

    SflowClientDeleteSflowGlobalConfigSPMessage::~SflowClientDeleteSflowGlobalConfigSPMessage ()
    {
    }

    void  SflowClientDeleteSflowGlobalConfigSPMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeIpVxAddress(&m_collector,"collector"));
        addSerializableAttribute (new AttributeUI32(&m_pollingInterval,"pollingInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sampleRate,"sampleRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  SflowClientDeleteSflowGlobalConfigSPMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  SflowClientDeleteSflowGlobalConfigSPMessage::getEnable() const
    {
        return (m_enable);
    }

    void  SflowClientDeleteSflowGlobalConfigSPMessage::setCollector(const IpVxAddress &collector)
    {
        m_collector  =  collector;
    }

    IpVxAddress  SflowClientDeleteSflowGlobalConfigSPMessage::getCollector() const
    {
        return (m_collector);
    }

    void  SflowClientDeleteSflowGlobalConfigSPMessage::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval  =  pollingInterval;
    }

    UI32  SflowClientDeleteSflowGlobalConfigSPMessage::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  SflowClientDeleteSflowGlobalConfigSPMessage::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate  =  sampleRate;
    }

    UI32  SflowClientDeleteSflowGlobalConfigSPMessage::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  SflowClientDeleteSflowGlobalConfigSPMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowClientDeleteSflowGlobalConfigSPMessage::getOpCode() const
    {
        return (m_opCode);
    }

    const void *SflowClientDeleteSflowGlobalConfigSPMessage::getCStructureForInputs ()
    {
	string str_ipaddr;
        sflow_global_config_msg_t *pInput = (sflow_global_config_msg_ *)malloc (sizeof (struct sflow_global_config_msg_));

	str_ipaddr = m_collector.toString();
        strncpy (pInput->collector, str_ipaddr.c_str (), 64);
	pInput->collector[63]='\0';
        pInput->enable= m_enable;
        pInput->polling_interval= m_pollingInterval;
        pInput->sample_rate= m_sampleRate;
        pInput->opcode= m_opCode;

        return (pInput);
    }

    void SflowClientDeleteSflowGlobalConfigSPMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
