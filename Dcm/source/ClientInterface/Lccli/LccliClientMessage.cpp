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
 *   Author : James Chen                                                   *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Lccli/LccliClientMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

namespace DcmNs
{


LccliMsg::LccliMsg ()
        : DcmManagementInterfaceMessage (LCCLI_MSG_CODE)
{
}

LccliMsg::LccliMsg (const string &Arg, const UI32 &CliCode)
        : DcmManagementInterfaceMessage (LCCLI_MSG_CODE),
        m_Arg    (Arg),
        m_CliCode   (CliCode)
{
}

LccliMsg::~LccliMsg ()
{
}

void  LccliMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32(&m_CliCode,"CliCode"));
    addSerializableAttribute (new AttributeString(&m_Arg,"Arg"));
}

void  LccliMsg::setArg(const string &Arg)
{
    m_Arg  =  Arg;
}

string  LccliMsg::getArg() const
{
        return (m_Arg);
}
    
void  LccliMsg::setCliCode(const UI32 &CliCode)
{
        m_CliCode  =  CliCode;
}

UI32  LccliMsg::getCliCode() const
{
        return (m_CliCode);
}

void LccliMsg::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
						  UI32 &size, void *&pBuffer)
{
        pBuffer   =   findBuffer(bufferNum, size);
}
    
const void *LccliMsg::getCStructureForInputs ()
{
        lccli_input_msg_t *pInput = new lccli_input_msg_t;

        strncpy (pInput->argv, m_Arg.c_str(), LCCLI_INPUT_LEN);
        pInput->cli_code    = m_CliCode;

        return (pInput);
}

void LccliMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
        lccli_output_msg_t *msg = (lccli_output_msg_t *)pOutputCStructure;

	tracePrintf(TRACE_LEVEL_INFO, "LCCLId Msg:data len=%d",  msg->out_buf_len);

	// duplicate data to buf for server to process
	// m->getDuplicateInterfaceOutputBuffer(LCCLI_MSG, size, data);
	// Call addBuffer(...., false) If the data is not included as part of lccli_out_msg_t
	// Expect pOutputCStructure should include lccli_out_msg_t and real data.
     addBuffer (LCCLI_MSG_CODE, sizeof(lccli_output_msg_t) + msg->out_buf_len,
		   pOutputCStructure, false);
}
}
