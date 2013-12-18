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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#include "ClientInterface/StormControl/StormControlClientShowMessage.h"
#include "ClientInterface/Qos/qos_bum_dcm_common.h"

namespace DcmNs
{
    StormControlClientShowMessage::StormControlClientShowMessage() : DcmManagementInterfaceMessage (STORMCONTROLCLIENTSHOW)
    {
    }

    StormControlClientShowMessage::StormControlClientShowMessage(const string &ifName, const UI32 &opCode,
		const UI32 &protocol , const UI32 &mappedId)
        : DcmManagementInterfaceMessage (STORMCONTROLCLIENTSHOW),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_protocol    (protocol),
		m_mappedId     (mappedId)
    {
    }

    StormControlClientShowMessage::~StormControlClientShowMessage ()
    {
    }

    void  StormControlClientShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_protocol,"protocol"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  StormControlClientShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  StormControlClientShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  StormControlClientShowMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  StormControlClientShowMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void StormControlClientShowMessage::setProtocol(const UI32 &protocol)
    {
        m_protocol = protocol;
    }

    UI32  StormControlClientShowMessage::getProtocol() const
    {
        return (m_protocol);
    }

    void  StormControlClientShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  StormControlClientShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    const void *StormControlClientShowMessage::getCStructureForInputs ()
    {
        qosBumDcmdShowAndClearMSg_t *pInput = (qosBumDcmdShowAndClearMSg_t *) calloc (1, sizeof(qosBumDcmdShowAndClearMSg_t));
        memset(pInput, 0, sizeof(qosBumDcmdShowAndClearMSg_t));

        strncpy (pInput->if_name, m_ifName.c_str(), 64);
        pInput->opcode     = (qosBumDcmdOpcode_t)m_opCode;

		pInput->protocol  = (qosBumProtocol_t)m_protocol;
        return (pInput);
    }

    void StormControlClientShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ssm_mgmt_buffer_num_t  buff_num;
        memset(&buff_num, 0, sizeof(buff_num));

        qosBumDcmdStatusResp_t  *resp_t = (qosBumDcmdStatusResp_t *)pOutputCStructure;

        if (!resp_t)
            return;

		size_t numOfRecords = ((resp_t->data).num_intf == 1) ? 1 : (resp_t->data).num_intf; // Number of interfaces

		buff_num.buff.mapped_id = this->getMappedId();

		addBuffer (buff_num.num, (sizeof(qosBumDcmdStatusResp_t)
			+ ((numOfRecords -1) *sizeof (qosBumIntfRecord_t))), (unsigned char *)pOutputCStructure, false);
	}
}
