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
 * SflowClientSflowProfileConfigSPMessage.cpp
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */

#include "ClientInterface/Sflow/SflowClientSflowProfileConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

	SflowClientSflowProfileConfigSPMessage::SflowClientSflowProfileConfigSPMessage ()
		: DcmManagementInterfaceMessage (SFLOWCLIENTSFLOWPROFILECONFIGSP)
    {
        	m_name = "";
        	m_samplingRate = 0;
            m_opCode = SFLOW_OUT_OF_RANGE_VALUE;
    }

	SflowClientSflowProfileConfigSPMessage::SflowClientSflowProfileConfigSPMessage (const string &profileName,const UI32 &sampleRate,const UI32 &opCode)
		: DcmManagementInterfaceMessage (SFLOWCLIENTSFLOWPROFILECONFIGSP),
		m_name			(profileName),
		m_samplingRate		(sampleRate),
		m_opCode			(opCode)
    {
    }

    SflowClientSflowProfileConfigSPMessage::~SflowClientSflowProfileConfigSPMessage ()
    {
    }

    void  SflowClientSflowProfileConfigSPMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI32(&m_samplingRate, "samplingRate"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));

    }

    void  SflowClientSflowProfileConfigSPMessage::setProfileName(const string &name)
     {
         m_name  =  name;
     }

     string  SflowClientSflowProfileConfigSPMessage::getProfileName() const
     {
         return (m_name);
     }

     void  SflowClientSflowProfileConfigSPMessage::setSamplingRate(const UI32 &samplingRate)
     {
         m_samplingRate  =  samplingRate;
     }

     UI32  SflowClientSflowProfileConfigSPMessage::getSamplingRate() const
     {
         return (m_samplingRate);
     }

     void  SflowClientSflowProfileConfigSPMessage::setOpCode(const UI32 &opCode)
     {
         m_opCode  =  opCode;
     }

     UI32  SflowClientSflowProfileConfigSPMessage::getOpCode() const
     {
         return (m_opCode);
     }

    const void *SflowClientSflowProfileConfigSPMessage::getCStructureForInputs ()
    {
        sflow_profile_config_msg_ *pInput = (sflow_profile_config_msg_ *)malloc (sizeof (struct sflow_profile_config_msg_));
        dumpMessageToBackend();
        strncpy (pInput->profile_name, m_name.c_str (), 64);
        pInput->profile_name[64]='\0';
        pInput->sample_rate= m_samplingRate;
        pInput->opcode= m_opCode;
        return (pInput);
    }

    void SflowClientSflowProfileConfigSPMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

    void SflowClientSflowProfileConfigSPMessage::dumpMessageToBackend () {
    	NSM_PLUG_DBG_S(string("Sflow Profile Config Message to Backend"));
    	NSM_PLUG_DBG_S(string("Sflow Profile Name :")+m_name);
    	NSM_PLUG_DBG_S(string("Sflow Profile Sampling Rate :")+m_samplingRate);
    	NSM_PLUG_DBG_S(string("Sflow Profile Config OpCode :")+m_opCode);
    	NSM_PLUG_DBG_S(string("End of Sflow Profile Config Message to Backend"));
    }
}


