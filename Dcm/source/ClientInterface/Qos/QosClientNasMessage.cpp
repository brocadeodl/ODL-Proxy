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

#include "ClientInterface/Qos/QosClientNasMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include <netinet/in.h>


namespace DcmNs
{

    QosClientNasMessage::QosClientNasMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTNASCONFIG)
    {
    }

	QosClientNasMessage::QosClientNasMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber)
        : DcmManagementInterfaceMessage(QOSCLIENTNASCONFIG),
        QosNasBaseMessage(opCode, cmdCode, cos, dscp, serverIpAddressPrefix, vrfName, vlanNumber)
    {
    }

    QosClientNasMessage::~QosClientNasMessage ()
    {
    }

    void  QosClientNasMessage::setupAttributesForSerialization()
    {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_cos,"cos"));
        addSerializableAttribute (new AttributeUI32(&m_dscp,"dscp"));
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_serverIpAddressPrefix,"serverIpAddressPrefix"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanNumber,"vlanNumber"));

    }

	const void *QosClientNasMessage::getCStructureForInputs ()
    {
        qosNasDcmdConfigMsg_t *pInput = (qosNasDcmdConfigMsg_s*)calloc(1, sizeof (struct qosNasDcmdConfigMsg_s));
		pInput->opcode = (qosNasDcmdOpcode_t) m_opCode;
		pInput->cmdcode = (qosNasDcmdCmdcode_t) m_cmdCode;
		pInput->cos = m_cos;
		pInput->dscp = m_dscp;
		pInput->vlan_number = m_vlanNumber;
		strncpy(pInput->vrf_name, m_vrfName.c_str(), DEFAULT_NAS_VRF_LEN);
		pInput->vrf_name[DEFAULT_NAS_VRF_LEN]='\0';
		if (m_cmdCode == QOS_NAS_DCMD_SERVER_IP)
		{
			string ipaddress = m_serverIpAddressPrefix.getIpAddress();
			cout << string("ipaddress=") + ipaddress;
			pInput->ip_addr = inet_addr(ipaddress.c_str());
			cout << string("ipaddr=") + pInput->ip_addr;
			pInput->mask = m_serverIpAddressPrefix.getLength();
			cout << string("ipaddr mask =") + pInput->mask;
		} else {
			pInput->ip_addr = 0;
			pInput->mask = 0;
		}
        return (pInput);
    }

    void QosClientNasMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
