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
 * SflowProfileShowMessage.cpp
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */

#include "ClientInterface/Sflow/SflowProfileShowMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"

namespace DcmNs
{

	SflowProfileShowMessage::SflowProfileShowMessage()
		: DcmManagementInterfaceMessage (SFLOWPROFILESHOWMESSAGE),
		  m_profileName	(""),
		  m_samplingRate	(0),
		m_cmdCode	(SFLOW_OUT_OF_RANGE_VALUE)
    {
    }

	SflowProfileShowMessage::SflowProfileShowMessage(const string &profileName, const UI32 &samplingRate, const UI32 &cmdCode)
		: DcmManagementInterfaceMessage (SFLOWPROFILESHOWMESSAGE),
		  m_profileName	(profileName),
		  m_samplingRate	(samplingRate),
		m_cmdCode	(cmdCode)
    {
    }

    SflowProfileShowMessage::~SflowProfileShowMessage ()
    {
    }

    void  SflowProfileShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
		addSerializableAttribute (new AttributeUI32(&m_samplingRate,"samplingRate"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
    }

    void  SflowProfileShowMessage::setProfileName(const string &name)
    {
        m_profileName  =  name;
    }

    string  SflowProfileShowMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  SflowProfileShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  SflowProfileShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void  SflowProfileShowMessage::setSamplingRate(const UI32 &samplingRate)
	{
		m_samplingRate  =  samplingRate;
	}

	UI32  SflowProfileShowMessage::getSamplingRate() const
	{
		return (m_samplingRate);
	}


	void SflowProfileShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}
    const void *SflowProfileShowMessage::getCStructureForInputs ()
    {
    	sflow_profile_show_input_msg_ *pInput = new sflow_profile_show_input_msg_;

        strncpy (pInput->profile_name, m_profileName.c_str (), 64);
        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

    void SflowProfileShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
		SflowBufferNum_t  buffNum;
		memset(&buffNum, 0, sizeof(buffNum));
		sflow_profile_show_msg_t *msg = ((sflow_profile_show_msg_t *)pOutputCStructure);

        if (!msg) return;

		int num_records = msg->num_records;
		switch (msg->record_type)
		{
			case SFLOW_SHOW_SFLOWPROFILE:
				buffNum.buff.tag = SFLOW_SHOW_SFLOWPROFILE;
				addBuffer (buffNum.num, (sizeof(sflow_profile_show_msg_t)+ ((num_records - 1) * sizeof (sflow_show_profile_msg_t))), pOutputCStructure, false);
				break;
			case SFLOW_SHOW_SFLOWPROFILE_ALL:
				buffNum.buff.tag = SFLOW_SHOW_SFLOWPROFILE_ALL;
				addBuffer (buffNum.num, (sizeof(sflow_profile_show_msg_t)+ ((num_records - 1) * sizeof (sflow_show_profile_msg_t))), pOutputCStructure, false);
				break;
			default:
				break;
		}

    }
}



