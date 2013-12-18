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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : Venkat														*
 ***************************************************************************/
#include "ClientInterface/Nsm/ConnectorShowMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    ConnectorShowMessage::ConnectorShowMessage ()
		: DcmManagementInterfaceMessage (SHOW_SFP_MESSAGE)
    {
		m_cmdCode = SHOW_SFP;
		m_linecard= 0;
		m_port = 0;
    }

    ConnectorShowMessage::ConnectorShowMessage (const UI32 &cmdCode, const UI32 &linecard, const UI32 &port)
		: DcmManagementInterfaceMessage (SHOW_SFP_MESSAGE),
        m_cmdCode(cmdCode),
        m_linecard(linecard),
		m_port(port)
    {
    }

    ConnectorShowMessage::~ConnectorShowMessage ()
    {
    }

    void  ConnectorShowMessage::setupAttributesForSerialization()
    {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_linecard, "linecard"));
        addSerializableAttribute (new AttributeUI32(&m_port, "port"));
    }

    void  ConnectorShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode = cmdCode;
    }

    UI32 ConnectorShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  ConnectorShowMessage::setLinecard(const UI32 &linecard)
    {
        m_linecard = linecard;
    }

    UI32  ConnectorShowMessage::getLinecard() const
    {
        return (m_linecard);
    }

    void  ConnectorShowMessage::setPort(const UI32 &port)
    {
        m_port = port;
    }

    UI32  ConnectorShowMessage::getPort() const
    {
        return (m_port);
    }

    const void *ConnectorShowMessage::getCStructureForInputs ()
    {
        SfpShowInputMsg_t *pInput = new SfpShowInputMsg_t;
        pInput->cmdCode = m_cmdCode;
        pInput->linecard = m_linecard;
        pInput->port = m_port;

        return (pInput);
    }

	void ConnectorShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		SfpShowOutputMsg_t *msg = ((SfpShowOutputMsg_t *)pOutputCStructure);

		if (!msg)
		{
			return;
		}

		int num_records = msg->num_records;
		addBuffer (SHOW_SFP, (sizeof(SfpShowOutputMsg_t)+ ((num_records - 1) * sizeof (SfpShowOutputRecord_t))), pOutputCStructure, false);
	}

	void ConnectorShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

}

