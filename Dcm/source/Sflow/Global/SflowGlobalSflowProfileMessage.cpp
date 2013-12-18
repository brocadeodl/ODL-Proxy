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
 * SflowGlobalSflowProfileMessage.cpp
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved
 *
 *  Created on: Sep 10, 2012
 *      Author: sunniram
 */

#include "Sflow/Global/SflowGlobalSflowProfileMessage.h"
#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "Sflow/Global/SflowGlobalTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"


namespace DcmNs
{

    SflowGlobalSflowProfileMessage::SflowGlobalSflowProfileMessage ()
        : ManagementInterfaceMessage (SflowGlobalObjectManager::getClassName (),SFLOWGLOBALSFLOWPROFILE)
    {
    	m_name = "";
    	m_samplingRate = 0;
        m_opCode = SFLOW_OUT_OF_RANGE_VALUE;
    }

    SflowGlobalSflowProfileMessage::SflowGlobalSflowProfileMessage (const string &name,const UI32 &samplingRate,const UI32 &opCode)
        : ManagementInterfaceMessage (SflowGlobalObjectManager::getClassName (),SFLOWGLOBALSFLOWPROFILE),
          m_name    (name),
          m_samplingRate (samplingRate),
          m_opCode    (opCode)
    {
    }

    SflowGlobalSflowProfileMessage::~SflowGlobalSflowProfileMessage ()
    {
    }

    void  SflowGlobalSflowProfileMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI32(&m_samplingRate, "samplingRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  SflowGlobalSflowProfileMessage::setProfileName(const string &name)
    {
        m_name  =  name;
    }

    string  SflowGlobalSflowProfileMessage::getProfileName() const
    {
        return (m_name);
    }

    void  SflowGlobalSflowProfileMessage::setSamplingRate(const UI32 &samplingRate)
    {
        m_samplingRate  =  samplingRate;
    }

    UI32  SflowGlobalSflowProfileMessage::getSamplingRate() const
    {
        return (m_samplingRate);
    }

    void  SflowGlobalSflowProfileMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowGlobalSflowProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

}



