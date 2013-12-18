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
#include "Sflow/Local/SflowDeleteSflowGlobalConfigSPMessage.h"
#include "Sflow/Local/SflowTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

    SflowDeleteSflowGlobalConfigSPMessage::SflowDeleteSflowGlobalConfigSPMessage ()
        : PrismMessage (SflowLocalObjectManager::getPrismServiceId (),SFLOWDELETESFLOWGLOBALCONFIGSP)
    {
        m_collector.fromString(SFLOW_DEFAULT_COLLECTOR);
        m_enable = SFLOW_DEFAULT_ENABLE;
        m_pollingInterval = SFLOW_DEFAULT_POLLING_INTERVAL;
        m_sampleRate = SFLOW_DEFAULT_SAMPLE_RATE;
        m_opCode = SFLOW_OUT_OF_RANGE_VALUE;
    }

    SflowDeleteSflowGlobalConfigSPMessage::SflowDeleteSflowGlobalConfigSPMessage (const bool &enable,const IpVxAddress &collector,const UI32 &pollingInterval,const UI32 &sampleRate,const UI32 &opCode)
        : PrismMessage (SflowLocalObjectManager::getPrismServiceId (),SFLOWDELETESFLOWGLOBALCONFIGSP),
        m_enable    (enable),
        m_collector    (collector),
        m_pollingInterval    (pollingInterval),
        m_sampleRate    (sampleRate),
        m_opCode    (opCode)
    {
    }

    SflowDeleteSflowGlobalConfigSPMessage::~SflowDeleteSflowGlobalConfigSPMessage ()
    {
    }

    void  SflowDeleteSflowGlobalConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeIpVxAddress(&m_collector,"collector"));
        addSerializableAttribute (new AttributeUI32(&m_pollingInterval,"pollingInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sampleRate,"sampleRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  SflowDeleteSflowGlobalConfigSPMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  SflowDeleteSflowGlobalConfigSPMessage::getEnable() const
    {
        return (m_enable);
    }

    void  SflowDeleteSflowGlobalConfigSPMessage::setCollector(const IpVxAddress &collector)
    {
        m_collector  =  collector;
    }

    IpVxAddress  SflowDeleteSflowGlobalConfigSPMessage::getCollector() const
    {
        return (m_collector);
    }

    void  SflowDeleteSflowGlobalConfigSPMessage::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval  =  pollingInterval;
    }

    UI32  SflowDeleteSflowGlobalConfigSPMessage::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  SflowDeleteSflowGlobalConfigSPMessage::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate  =  sampleRate;
    }

    UI32  SflowDeleteSflowGlobalConfigSPMessage::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  SflowDeleteSflowGlobalConfigSPMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowDeleteSflowGlobalConfigSPMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
