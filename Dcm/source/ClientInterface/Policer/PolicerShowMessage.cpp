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
 *   Author : Sudha, Shivanand Tatpatti
 **************************************************************************/

//#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Policer/PolicerShowMessage.h"
#include "ssm_policer_dcmd_public.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
//#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs {

PolicerShowMessage::PolicerShowMessage() :
		DcmManagementInterfaceMessage(POLICER_SHOW_ALL) {
}

PolicerShowMessage::PolicerShowMessage(const string &Interface,
		const string &ifName, const UI32 &direction, const UI32 &cmdCode,
		const UI32 &mappedId, const string &policyMapName) :
		DcmManagementInterfaceMessage(POLICER_SHOW_ALL), m_interface(Interface), m_ifName(
				ifName), m_direction(direction), m_cmdCode(cmdCode), m_mappedId(
				mappedId), m_policyMapName(policyMapName) {
}

PolicerShowMessage::~PolicerShowMessage() {
}

void PolicerShowMessage::setupAttributesForSerialization() {
	DcmManagementInterfaceMessage::setupAttributesForSerialization();
	addSerializableAttribute(new AttributeUI32(&m_cmdCode, "cmdCode"));
	addSerializableAttribute(new AttributeUI32(&m_direction, "direction"));
	addSerializableAttribute(new AttributeString(&m_ifName, "ifName"));
	addSerializableAttribute(new AttributeString(&m_interface, "Interface"));
	addSerializableAttribute(new AttributeUI32(&m_mappedId, "mappedId"));
	addSerializableAttribute(
			new AttributeString(&m_policyMapName, "policyMapName"));
}

void PolicerShowMessage::setInterface(const string &Interface) {
	m_interface = Interface;
}

string PolicerShowMessage::getInterface() const {
	return (m_interface);
}

void PolicerShowMessage::setIfName(const string &ifName) {
	m_ifName = ifName;
}

string PolicerShowMessage::getIfName() const {
	return (m_ifName);
}

void PolicerShowMessage::setDirection(const UI32 &direction) {
	m_direction = direction;
}

UI32 PolicerShowMessage::getDirection() const {
	return (m_direction);
}

void PolicerShowMessage::setCmdCode(const UI32 &cmdCode) {
	m_cmdCode = cmdCode;
}

UI32 PolicerShowMessage::getCmdCode() const {
	return (m_cmdCode);
}

void PolicerShowMessage::setMappedId(const UI32 &mappedId) {
	m_mappedId = mappedId;
}

UI32 PolicerShowMessage::getMappedId() const {
	return (m_mappedId);
}

void PolicerShowMessage::setPolicyMapName(const string &policyMapName) {
	m_policyMapName = policyMapName;
}

string PolicerShowMessage::getPolicyMapName() const {
	return m_policyMapName;
}

void PolicerShowMessage::getOutputBufferRef(UI32 bufferNum, UI32 &size,
		void *&pBuffer) {
	pBuffer = findBuffer(bufferNum, size);
}

void PolicerShowMessage::getDuplicateInterfaceOutputBuffer(UI32 bufferNum,
		UI32 &size, void *&pBuffer) {
	pBuffer = findBuffer(bufferNum, size);
}

const void *PolicerShowMessage::getCStructureForInputs() {

	/*trace(
			TRACE_LEVEL_INFO,
			"Entering PolicerShowMessage:: getCStructureForInputs:- "
					+ m_cmdCode);*/

	switch (m_cmdCode) {

	case FB_SHOW_POLICYMAP_ALL: {

		fbDcmShowPmapSummaryInputMessage_t *pInput =
				new fbDcmShowPmapSummaryInputMessage_t;
		pInput->opcode = (fbDcmCommandCode_t) m_cmdCode;
		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"Entering PolicerShowMessage:: getCStructureForInputs:- Opcode<")
						+ pInput->opcode + ">");*/

		return (pInput);

		break;
	}

	case FB_SHOW_POLICYMAP_INTERFACE: {
		fbDcmShowPmapIntInputMessage_t *pInput =
				new fbDcmShowPmapIntInputMessage_t;
		pInput->opcode = (fbDcmCommandCode_t) m_cmdCode;
		strncpy(pInput->if_name, m_ifName.c_str(), MAX_IF_NAME_LEN);
		pInput->if_name[MAX_IF_NAME_LEN] = '\0';
		pInput->direction = m_direction;
		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"Entering PolicerShowMessage:: getCStructureForInputs:- Opcode<")
						+ pInput->opcode + "> Interface Name<" + pInput->if_name
						+ "> Direction<" + pInput->direction + ">");*/

		return (pInput);

		break;
	}

	case FB_SHOW_POLICY_MAP_DETAIL: {
		fbDcmShowPmapDetailsInputMessage_t *pInput =
				new fbDcmShowPmapDetailsInputMessage_t;

		pInput->opcode = (fbDcmCommandCode_t) m_cmdCode;
		strncpy(pInput->policy_name, m_policyMapName.c_str(),
				FB_MAX_MAP_NAME_SIZE - 1);
		pInput->policy_name[FB_MAX_MAP_NAME_SIZE - 1] = '\0';
		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"Entering PolicerShowMessage:: getCStructureForInputs:- Opcode<")
						+ pInput->opcode + "> PolicyMapName<"
						+ pInput->policy_name + ">");*/

		return (pInput);

		break;
	}

	}

	return NULL;

}

void PolicerShowMessage::loadOutputsFromCStructure(
		const void *pOutputCStructure) {

	dot1x_mgmt_buffer_num_t buff_num;
	memset(&buff_num, 0, sizeof(buff_num));
	buff_num.buff.mapped_id = this->getMappedId();

	size_t data_size = 0;

	switch (m_cmdCode) {
	case FB_SHOW_POLICYMAP_ALL: {
		/*trace(
				TRACE_LEVEL_INFO,
				"Entering PolicerShowMessage:: loadOutputsFromCStructure:- FB_SHOW_POLICYMAP_ALL");*/
		fbDcmShowPmapOutputMessage_t *msg =
				((fbDcmShowPmapOutputMessage_t *) pOutputCStructure);
		buff_num.buff.tag = FB_SHOW_POLICYMAP_ALL;
		if (msg == 0) {
			/*trace(
					TRACE_LEVEL_INFO,
					"Entering PolicerShowMessage:: loadOutputsFromCStructure:- Message NULL");*/
			return;
		} else {
			data_size = msg->data_size;
			/*trace(
					TRACE_LEVEL_INFO,
					string(
							"Entering PolicerShowMessage:: loadOutputsFromCStructure:<")
							+ data_size + ">");*/
		}
		break;
	}
	case FB_SHOW_POLICYMAP_INTERFACE: {
		/*trace(
				TRACE_LEVEL_INFO,
				"Entering PolicerShowMessage:: loadOutputsFromCStructure:- FB_SHOW_POLICYMAP_INTERFACE");*/
		buff_num.buff.tag = FB_SHOW_POLICYMAP_INTERFACE;
		fbDcmShowPmapIntOutputMessage_t *msg =
				((fbDcmShowPmapIntOutputMessage_t *) pOutputCStructure);
		if (msg == 0) {
			/*trace(
					TRACE_LEVEL_INFO,
					"Entering PolicerShowMessage:: loadOutputsFromCStructure:- Message NULL");*/
			return;
		} else {
			data_size = msg->data_size;
			/*trace(
					TRACE_LEVEL_INFO,
					string(
							"Entering PolicerShowMessage:: loadOutputsFromCStructure:<")
							+ data_size + ">");*/
		}
		break;
	}

	case FB_SHOW_POLICY_MAP_DETAIL: {
		/*trace(
				TRACE_LEVEL_INFO,
				"Entering PolicerShowMessage:: loadOutputsFromCStructure:- FB_SHOW_POLICY_MAP_DETAIL");*/
		buff_num.buff.tag = FB_SHOW_POLICY_MAP_DETAIL;
		fbDcmShowPmapDetailOutputMessage_t *msg =
				((fbDcmShowPmapDetailOutputMessage_t *) pOutputCStructure);
		if (msg == 0) {
			/*trace(
					TRACE_LEVEL_INFO,
					"Entering PolicerShowMessage:: loadOutputsFromCStructure:- Message NULL");*/
			return;
		} else {
			data_size = msg->data_size;
			/*trace(
					TRACE_LEVEL_INFO,
					string(
							"Entering PolicerShowMessage:: loadOutputsFromCStructure:<")
							+ data_size + ">");*/
		}
		break;
	}
	}

	/*trace(
			TRACE_LEVEL_INFO,
			string("Entering PolicerShowMessage:: loadOutputsFromCStructure:- ")
					+ data_size);*/

	if (data_size != 0)
		addBuffer(buff_num.num, data_size, pOutputCStructure, false);
}
}
