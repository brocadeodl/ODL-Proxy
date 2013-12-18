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

#include "Sflow/Global/SflowGlobalDeleteSflowGlobalConfigMessage.h"
#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "Sflow/Global/SflowGlobalTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"

namespace DcmNs
{

    SflowGlobalDeleteSflowGlobalConfigMessage::SflowGlobalDeleteSflowGlobalConfigMessage ()
        : ManagementInterfaceMessage (SflowGlobalObjectManager::getClassName (),SFLOWGLOBALDELETESFLOWGLOBALCONFIG)
    {
        m_enable = SFLOW_DEFAULT_ENABLE;
        m_pollingInterval = SFLOW_DEFAULT_POLLING_INTERVAL;
        m_sampleRate = SFLOW_DEFAULT_SAMPLE_RATE;
        m_opCode = SFLOW_OUT_OF_RANGE_VALUE;
    }

    SflowGlobalDeleteSflowGlobalConfigMessage::SflowGlobalDeleteSflowGlobalConfigMessage (const bool &enable,const UI32 &pollingInterval,const UI32 &sampleRate,const UI32 &opCode)
        : ManagementInterfaceMessage (SflowGlobalObjectManager::getClassName (),SFLOWGLOBALDELETESFLOWGLOBALCONFIG),
        m_enable    (enable),
        m_pollingInterval    (pollingInterval),
        m_sampleRate    (sampleRate),
        m_opCode    (opCode)
    {
    }

    SflowGlobalDeleteSflowGlobalConfigMessage::~SflowGlobalDeleteSflowGlobalConfigMessage ()
    {
    }

    void  SflowGlobalDeleteSflowGlobalConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeUI32(&m_pollingInterval, SFLOW_DEFAULT_POLLING_INTERVAL, "pollingInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sampleRate, SFLOW_DEFAULT_SAMPLE_RATE, "sampleRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  SflowGlobalDeleteSflowGlobalConfigMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  SflowGlobalDeleteSflowGlobalConfigMessage::getEnable() const
    {
        return (m_enable);
    }

    void  SflowGlobalDeleteSflowGlobalConfigMessage::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval  =  pollingInterval;
    }

    UI32  SflowGlobalDeleteSflowGlobalConfigMessage::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  SflowGlobalDeleteSflowGlobalConfigMessage::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate  =  sampleRate;
    }

    UI32  SflowGlobalDeleteSflowGlobalConfigMessage::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  SflowGlobalDeleteSflowGlobalConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowGlobalDeleteSflowGlobalConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
