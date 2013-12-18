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
 * SflowProfilePrismMessage.cpp
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */

#include "Sflow/Local/SflowLocalObjectManager.h"
#include "Sflow/Local/SflowProfilePrismMessage.h"
#include "Sflow/Local/SflowTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"

namespace DcmNs
{

    SflowProfilePrismMessage::SflowProfilePrismMessage ()
        : PrismMessage (SflowLocalObjectManager::getPrismServiceId (),SFLOWPROFILECONFIGSP)
    {
    	m_name = "";
    	m_samplingRate = 0;
        m_opCode = SFLOW_OUT_OF_RANGE_VALUE;
    }

    SflowProfilePrismMessage::SflowProfilePrismMessage (const string &name,const UI32 &samplingRate,const UI32 &opCode)
        : PrismMessage (SflowLocalObjectManager::getPrismServiceId (),SFLOWPROFILECONFIGSP),
          m_name    (name),
          m_samplingRate (samplingRate),
          m_opCode    (opCode)
    {
    }

    SflowProfilePrismMessage::~SflowProfilePrismMessage ()
    {
    }

    void  SflowProfilePrismMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI32(&m_samplingRate, "samplingRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }


    void  SflowProfilePrismMessage::setProfileName(const string &name)
    {
        m_name  =  name;
    }

    string  SflowProfilePrismMessage::getProfileName() const
    {
        return (m_name);
    }

    void  SflowProfilePrismMessage::setSamplingRate(const UI32 &samplingRate)
    {
        m_samplingRate  =  samplingRate;
    }

    UI32  SflowProfilePrismMessage::getSamplingRate() const
    {
        return (m_samplingRate);
    }

    void  SflowProfilePrismMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  SflowProfilePrismMessage::getOpCode() const
    {
        return (m_opCode);
    }
}



