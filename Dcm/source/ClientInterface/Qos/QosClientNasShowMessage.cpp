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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "ClientInterface/Qos/QosClientNasShowMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include <netinet/in.h>


namespace DcmNs
{

    QosClientNasShowMessage::QosClientNasShowMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTNASSHOW),
		QosNasBaseMessage(),
		m_mappedId(0)
    {
    }

	QosClientNasShowMessage::QosClientNasShowMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber)
        : DcmManagementInterfaceMessage(QOSCLIENTNASSHOW),
        QosNasBaseMessage(opCode, cmdCode, cos, dscp, serverIpAddressPrefix, vrfName, vlanNumber)
    {
    }

    QosClientNasShowMessage::~QosClientNasShowMessage ()
    {
    }

    void  QosClientNasShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  QosClientNasShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void  QosClientNasShowMessage::setupAttributesForSerialization()
    {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_cos,"cos"));
        addSerializableAttribute (new AttributeUI32(&m_dscp,"dscp"));
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_serverIpAddressPrefix,"serverIpAddressPrefix"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanNumber,"vlanNumber"));
        addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));

    }

	const void *QosClientNasShowMessage::getCStructureForInputs ()
    {
        qosNasDcmdConfigMsg_t *pInput = (qosNasDcmdConfigMsg_s*)calloc(1, sizeof (struct qosNasDcmdConfigMsg_s));
		pInput->opcode = (qosNasDcmdOpcode_t) m_opCode;
		pInput->cmdcode = (qosNasDcmdCmdcode_t) m_cmdCode;
		pInput->cos = m_cos;
		pInput->dscp = m_dscp;
		pInput->vlan_number = m_vlanNumber;
		strncpy(pInput->vrf_name, m_vrfName.c_str(), DEFAULT_NAS_VRF_LEN);
		pInput->vrf_name[DEFAULT_NAS_VRF_LEN]='\0';
		string ipaddress = m_serverIpAddressPrefix.getIpAddress();
		pInput->ip_addr = inet_addr(ipaddress.c_str());
		pInput->mask = m_serverIpAddressPrefix.getLength();
        return (pInput);
    }

    void QosClientNasShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        QosNasBufferNum_t  buffNum;
        memset(&buffNum, 0, sizeof(buffNum));

		if (!pOutputCStructure) {
			return;
		}

        qosNasDcmdStatusRespMsg_t *msg = ((qosNasDcmdStatusRespMsg_t *)pOutputCStructure);
        unsigned int num_records = msg->num_records;

        /* Set QosMessageBufferId */
        switch (msg->cmdcode)
       	{
            case QOS_NAS_DCMD_SHOW_STATS:
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOS_NAS_DCMD_SHOW_STATS;
                addBuffer (buffNum.num, (sizeof(qosNasDcmdStatusRespMsg_t)+ ((num_records - 1) * sizeof (qosNasDcmdStatusRespData_t))), pOutputCStructure, false);
                break;
            }
            case QOS_NAS_DCMD_CLEAR_STATS:
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOS_NAS_DCMD_CLEAR_STATS;
                addBuffer (buffNum.num, (sizeof(qosNasDcmdStatusRespMsg_t)+ ((num_records - 1) * sizeof (qosNasDcmdStatusRespData_t))), pOutputCStructure, false);
                break;
            }
            case QOS_NAS_DCMD_DEBUG:
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOS_NAS_DCMD_DEBUG;
                addBuffer (buffNum.num, (sizeof(qosNasDcmdStatusRespMsg_t)+ ((num_records - 1) * sizeof (qosNasDcmdStatusRespData_t))), pOutputCStructure, false);
                break;
            }
			default:
			{
				break;
			}
		}
    }

}
