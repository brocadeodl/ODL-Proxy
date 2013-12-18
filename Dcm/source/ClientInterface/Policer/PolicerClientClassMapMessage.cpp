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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "ClientInterface/Policer/PolicerClientClassMapMessage.h"
#include "qosd_fb_public.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
//#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs {

PolicerClientClassMapMessage::PolicerClientClassMapMessage() :
		DcmManagementInterfaceMessage(POLICERCLIENTCLASSMAP) {
}

PolicerClientClassMapMessage::PolicerClientClassMapMessage(
		const string &classMapName, const bool &any, const UI32 &opcode,
		const UI32 &filterCriteria, const string &accessListName) :
		DcmManagementInterfaceMessage(POLICERCLIENTCLASSMAP), m_classMapName(
				classMapName), m_any(any), m_opcode(opcode), m_filterCriteria(
				filterCriteria), m_accessListName(accessListName) {
}

PolicerClientClassMapMessage::~PolicerClientClassMapMessage() {
}

void PolicerClientClassMapMessage::setupAttributesForSerialization() {
	DcmManagementInterfaceMessage::setupAttributesForSerialization();
	addSerializableAttribute(
			new AttributeString(&m_classMapName, "ClassMapName"));
	addSerializableAttribute(new AttributeBool(&m_any, "any"));
	addSerializableAttribute(new AttributeUI32(&m_opcode, "opcode"));
	addSerializableAttribute(
			new AttributeUI32(&m_filterCriteria, "filterCriteria"));
	addSerializableAttribute(
			new AttributeString(&m_accessListName, "accessListName"));
}

void PolicerClientClassMapMessage::setClassMapName(const string &classMapName) {
	m_classMapName = classMapName;
}

string PolicerClientClassMapMessage::getClassMapName() const {
	return (m_classMapName);
}

void PolicerClientClassMapMessage::setAny(const bool &any) {
	m_any = any;
}

bool PolicerClientClassMapMessage::getAny() const {
	return (m_any);
}

void PolicerClientClassMapMessage::setOpcode(const UI32 &opcode) {
	m_opcode = opcode;
}

UI32 PolicerClientClassMapMessage::getOpcode() const {
	return (m_opcode);
}

string PolicerClientClassMapMessage::getAccessListName() const {
	return m_accessListName;
}

void PolicerClientClassMapMessage::setAccessListName(
		const string &accessListName) {
	m_accessListName = accessListName;
}

void PolicerClientClassMapMessage::setFilterCriteria(
		const UI32 &filterCriteria) {
	m_filterCriteria = filterCriteria;
}

UI32 PolicerClientClassMapMessage::getFilterCriteria() const {
	return m_filterCriteria;
}

const void *PolicerClientClassMapMessage::getCStructureForInputs() {

	/*trace(TRACE_LEVEL_INFO,
			"Entering PolicerClientClassMapMessage::getCStructureForInputs\n");*/

	fbDcmClassMapMessage_t *pInput = (fbDcmClassMapMessage_t *) malloc(
			sizeof(fbDcmClassMapMessage_t));

	pInput->opcode = (fbDcmCommandCode_t) m_opcode;
	strncpy(pInput->name, m_classMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
	pInput->name[FB_MAX_MAP_NAME_SIZE - 1] = '\0';
	//Populate the rule based on the classification criteria.
	pInput->rule = (fb_policy_match_criteria) m_filterCriteria;
	strncpy(pInput->accessListName, m_accessListName.c_str(),
			FB_MAX_ACL_NAME_SIZE-1);
	pInput->accessListName[FB_MAX_ACL_NAME_SIZE-1] = '\0';

	/*if(m_any == 1)
	 pInput->rule = SSM_POL_POLICY_RULE_MATCH_ANY_E;
	 */

	/*trace(
			TRACE_LEVEL_INFO,
			string(
					"PolicerClientClassMapMessage::getCStructureForInputs: Operation Code : ")
					+ pInput->opcode + "\n");
	trace(
			TRACE_LEVEL_INFO,
			string(
					"PolicerClientClassMapMessage::getCStructureForInputs: ClassMapName : ")
					+ pInput->name + "\n");
	trace(
			TRACE_LEVEL_INFO,
			string(
					"PolicerClientClassMapMessage::getCStructureForInputs: AccessListName : ")
					+ pInput->accessListName + "\n");
	trace(
			TRACE_LEVEL_INFO,
			string(
					"PolicerClientClassMapMessage::getCStructureForInputs: Classification Rule : ")
					+ pInput->rule + "\n");*/

	return (pInput);
}

void PolicerClientClassMapMessage::loadOutputsFromCStructure(
		const void *pOutputCStructure) {
}

}
