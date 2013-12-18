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
 *   Author : vsatyana														*
 ****************************************************************************/

#include "ClientInterface/Nsm/SwitchportVlanClassificationMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

	SwitchportVlanClassificationMessage::SwitchportVlanClassificationMessage ()
	: DcmManagementInterfaceMessage (SW_VLAN_CLASSIFICATION_MESSAGE)
	{
	}

	SwitchportVlanClassificationMessage::~SwitchportVlanClassificationMessage ()
	{
	}

	void SwitchportVlanClassificationMessage::setupAttributesForSerialization()
	{
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32Vector(&m_cmdCode, "cmdCode"));
		addSerializableAttribute (new AttributeUI32Vector(&m_vlanId, "vlanId"));
		addSerializableAttribute (new AttributeUI32Vector(&m_ifType, "ifType"));
		addSerializableAttribute (new AttributeStringVector(&m_ifName, "ifName"));
		addSerializableAttribute (new AttributeStringVector(&m_macAddress, "macAddress"));
		addSerializableAttribute (new AttributeUI32Vector(&m_macGroupId, "macGroupId"));
		addSerializableAttribute (new AttributeUI32Vector(&m_ctagId, "ctagId"));
	}

	UI32 SwitchportVlanClassificationMessage::getNumRec() const
	{
		return m_cmdCode.size();
	}

	void SwitchportVlanClassificationMessage::setMacAddressRec(const UI32 &cmdCode, const UI32 &ifType, const string &ifName, const UI32 &vlanId, const string &macAddress)
	{
		m_cmdCode.push_back(cmdCode);
		m_ifType.push_back(ifType);
		m_ifName.push_back(ifName);
		m_vlanId.push_back(vlanId);
		m_macAddress.push_back(macAddress);
		m_macGroupId.push_back(0);
		m_ctagId.push_back(0);
	}

	void SwitchportVlanClassificationMessage::setMacGroupIdRec(const UI32 &cmdCode, const UI32 &ifType, const string &ifName, const UI32 &vlanId, const UI32 &macGroupId)
	{
		m_cmdCode.push_back(cmdCode);
		m_ifType.push_back(ifType);
		m_ifName.push_back(ifName);
		m_vlanId.push_back(vlanId);
		m_macGroupId.push_back(macGroupId);
		m_macAddress.push_back("");
		m_ctagId.push_back(0);
	}

	void SwitchportVlanClassificationMessage::setCtagIdRec(const UI32 &cmdCode, const UI32 &ifType, const string &ifName, const UI32 &vlanId, const UI32 &ctagId)
	{
		m_cmdCode.push_back(cmdCode);
		m_ifType.push_back(ifType);
		m_ifName.push_back(ifName);
		m_vlanId.push_back(vlanId);
		m_ctagId.push_back(ctagId);
		m_macAddress.push_back("");
		m_macGroupId.push_back(0);
	}


	const void *SwitchportVlanClassificationMessage::getCStructureForInputs ()
	{
		UI32 numRecords = m_cmdCode.size();
	  	SwitchportVlanClassificationMsg_t *pInput = NULL;

		if (numRecords < 1)
		{
			return pInput;
		}

		pInput = (SwitchportVlanClassificationMsg_t *)calloc(1, (sizeof(SwitchportVlanClassificationMsg_t) + (numRecords - 1) * sizeof(SwitchportVlanClassificationRec_t)));
		pInput->numRecords = numRecords;

		for(UI32 recIdx = 0; recIdx < numRecords; recIdx++)
		{
			SwitchportVlanClassificationRec_t *vlanClassificationRec = &(pInput->rec[recIdx]);

			vlanClassificationRec->cmdCode = m_cmdCode[recIdx];
			vlanClassificationRec->ifType = m_ifType[recIdx];
			strncpy(vlanClassificationRec->ifName, (char *)m_ifName[recIdx].c_str(), IF_LONG_NAMESIZE);
			vlanClassificationRec->ifName[IF_LONG_NAMESIZE - 1] = '\0';

			switch(m_cmdCode[recIdx])
			{
				case SW_VLAN_CLASSIFICATION_SET_MAC_ADDR:
				case SW_VLAN_CLASSIFICATION_DEL_MAC_ADDR:
				case SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_ADDR:
				case SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_ADDR:
				{
					vlanClassificationRec->payload.macAddressRec.vlanId = m_vlanId[recIdx];
					sscanf(m_macAddress[recIdx].c_str(), "%4hx.%4hx.%4hx", (unsigned short *)&(vlanClassificationRec->payload.macAddressRec.mac[0]), (unsigned short *)&(vlanClassificationRec->payload.macAddressRec.mac[2]), (unsigned short *)&(vlanClassificationRec->payload.macAddressRec.mac[4]));
				}
				break;

				case SW_VLAN_CLASSIFICATION_SET_MAC_GROUP:
				case SW_VLAN_CLASSIFICATION_DEL_MAC_GROUP:
				case SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_GROUP:
				case SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_GROUP:
				{
					vlanClassificationRec->payload.macGroupRec.vlanId = m_vlanId[recIdx];
					vlanClassificationRec->payload.macGroupRec.macGroupId = m_macGroupId[recIdx];
				}
				break;

				case SW_VLAN_CLASSIFICATION_SET_CTAG:
				case SW_VLAN_CLASSIFICATION_DEL_CTAG:
				case SW_VLAN_CLASSIFICATION_SET_RSPAN_CTAG:
				case SW_VLAN_CLASSIFICATION_DEL_RSPAN_CTAG:
				case SW_VLAN_CLASSIFICATION_SET_PVLAN_CTAG:
				case SW_VLAN_CLASSIFICATION_DEL_PVLAN_CTAG:
				{
					vlanClassificationRec->payload.ctagRec.vlanId = m_vlanId[recIdx];
					vlanClassificationRec->payload.ctagRec.ctagId = m_ctagId[recIdx];
				}
				break;

				default:
					break;
			}
		}

		return pInput;
	}

	void SwitchportVlanClassificationMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}

}

