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

#include "ClientInterface/Policer/PolicerClientPolicyClassMapMessage.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "ssm_policer_dcmd_public.h"
#include "Policer/Global/PolicerGlobalTypes.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/DceWaveClientMessage.h"
//#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"

namespace DcmNs {

PolicerClientPolicyClassMapMessage::PolicerClientPolicyClassMapMessage() :
		DcmManagementInterfaceMessage(POLICERCLIENTPOLICYCLASSMAP) {
}

PolicerClientPolicyClassMapMessage::PolicerClientPolicyClassMapMessage(
		const string &policyClassMapName, const UI64 &cir, const UI64 &cbs,
		const UI64 &eir, const UI64 &ebs, const UI32 &conformDscp,
		const UI32 &conformPrecedence, const UI32 &conformType,
		const UI32 &conformTc, const UI32 &exceedDscp,
		const UI32 &exceedPrecedence, const UI32 &exceedType,
		const UI32 &exceedTc, const string &classMapId,
		const string &priorityClassMapId, const UI32 &opcode, const UI8 &cos,
		const UI8 &trafficClass, const UI16 &dscp,
		const string &CoSMutationMapName, const string &CoSTrafficClassMapName,
		const string &dscpCoSMapName, const string &dscpMutationMapName,
		const string &dscpTrafficClassMapName, const string &sflowMapName,
		const string &ceeMapName, const UI64 &shapingRate,

		const UI8 &strictPriority,

		const UI32 &dwrrTrafficClass0, const UI32 &dwrrTrafficClass1,
		const UI32 &dwrrTrafficClass2, const UI32 &dwrrTrafficClass3,
		const UI32 &dwrrTrafficClass4, const UI32 &dwrrTrafficClass5,
		const UI32 &dwrrTrafficClass6, const UI32 &dwrrTrafficClass7,

		const UI64 &shapingRateTrafficClass0,
		const UI64 &shapingRateTrafficClass1,
		const UI64 &shapingRateTrafficClass2,
		const UI64 &shapingRateTrafficClass3,
		const UI64 &shapingRateTrafficClass4,
		const UI64 &shapingRateTrafficClass5,
		const UI64 &shapingRateTrafficClass6,
		const UI64 &shapingRateTrafficClass7

		) :
		DcmManagementInterfaceMessage(POLICERCLIENTPOLICYCLASSMAP), m_policyClassMapName(
				policyClassMapName), m_cir(cir), m_cbs(cbs), m_eir(eir), m_ebs(
				ebs), m_conformDscp(conformDscp), m_conformPrecedence(
				conformPrecedence), m_conformType(conformType), m_conformTc(
				conformTc), m_exceedDscp(exceedDscp), m_exceedPrecedence(
				exceedPrecedence), m_exceedType(exceedType), m_exceedTc(
				exceedTc), m_classMapId(classMapId), m_priorityMapId(
				priorityClassMapId), m_opcode(opcode), m_cos(cos), m_trafficClass(
				trafficClass), m_dscp(dscp),

		m_CoSMutationMapName(CoSMutationMapName), m_CoSTrafficClassMapName(
				CoSTrafficClassMapName), m_dscpCoSMapName(dscpCoSMapName), m_dscpMutationMapName(
				dscpMutationMapName), m_dscpTrafficClassMapName(
				dscpTrafficClassMapName), m_sflowMapName(sflowMapName), m_ceeMapName(
				ceeMapName),

		m_shapingRate(shapingRate),

		m_strictPriority(strictPriority),

		m_dwrrTrafficClass0(dwrrTrafficClass0), m_dwrrTrafficClass1(
				dwrrTrafficClass1), m_dwrrTrafficClass2(dwrrTrafficClass2), m_dwrrTrafficClass3(
				dwrrTrafficClass3), m_dwrrTrafficClass4(dwrrTrafficClass4), m_dwrrTrafficClass5(
				dwrrTrafficClass5), m_dwrrTrafficClass6(dwrrTrafficClass6), m_dwrrTrafficClass7(
				dwrrTrafficClass7),

		m_shapingRateTrafficClass0(shapingRateTrafficClass0), m_shapingRateTrafficClass1(
				shapingRateTrafficClass1), m_shapingRateTrafficClass2(
				shapingRateTrafficClass2), m_shapingRateTrafficClass3(
				shapingRateTrafficClass3), m_shapingRateTrafficClass4(
				shapingRateTrafficClass4), m_shapingRateTrafficClass5(
				shapingRateTrafficClass5), m_shapingRateTrafficClass6(
				shapingRateTrafficClass6), m_shapingRateTrafficClass7(
				shapingRateTrafficClass7) {
}

PolicerClientPolicyClassMapMessage::~PolicerClientPolicyClassMapMessage() {
}

void PolicerClientPolicyClassMapMessage::setupAttributesForSerialization() {
	DcmManagementInterfaceMessage::setupAttributesForSerialization();
	addSerializableAttribute(
			new AttributeString(&m_policyClassMapName, "policyClassMapName"));
	addSerializableAttribute(new AttributeUI64(&m_cir, "cir"));
	addSerializableAttribute(new AttributeUI64(&m_cbs, "cbs"));
	addSerializableAttribute(new AttributeUI64(&m_eir, "eir"));
	addSerializableAttribute(new AttributeUI64(&m_ebs, "ebs"));
	addSerializableAttribute(new AttributeUI32(&m_conformDscp, "conformDscp"));
	addSerializableAttribute(
			new AttributeUI32(&m_conformPrecedence, "conformPrecedence"));
	addSerializableAttribute(new AttributeUI32(&m_conformType, "conformType"));
	addSerializableAttribute(new AttributeUI32(&m_conformTc, "conformTc"));
	addSerializableAttribute(new AttributeUI32(&m_exceedDscp, "exceedDscp"));
	addSerializableAttribute(
			new AttributeUI32(&m_exceedPrecedence, "exceedPrecedence"));
	addSerializableAttribute(new AttributeUI32(&m_exceedType, "exceedType"));
	addSerializableAttribute(new AttributeUI32(&m_exceedTc, "exceedTc"));
	addSerializableAttribute(new AttributeString(&m_classMapId, "classMapId"));
	addSerializableAttribute(
			new AttributeString(&m_priorityMapId, "priorityMapId"));
	addSerializableAttribute(new AttributeUI32(&m_opcode, "opcode"));

	addSerializableAttribute(new AttributeUI8(&m_cos, "cos"));
	addSerializableAttribute(new AttributeUI8(&m_trafficClass, "trafficClass"));
	addSerializableAttribute(new AttributeUI16(&m_dscp, "dscp"));

	addSerializableAttribute(
			new AttributeString(&m_CoSMutationMapName, "CoSMutationMapName"));
	addSerializableAttribute(
			new AttributeString(&m_CoSTrafficClassMapName,
					"CoSTrafficClassMapName"));
	addSerializableAttribute(
			new AttributeString(&m_dscpCoSMapName, "dscpCoSMapName"));
	addSerializableAttribute(
			new AttributeString(&m_dscpMutationMapName, "dscpMutationMapName"));
	addSerializableAttribute(
			new AttributeString(&m_dscpTrafficClassMapName,
					"dscpTrafficClassMapName"));
	addSerializableAttribute(
			new AttributeString(&m_sflowMapName, "ssflowMapName"));
	addSerializableAttribute(new AttributeString(&m_ceeMapName, "ceeMapName"));

	addSerializableAttribute(new AttributeUI64(&m_shapingRate, "shapingRate"));
	addSerializableAttribute(
			new AttributeUI8(&m_strictPriority, "strictPriority"));

	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass0, "dwrrTrafficClass0"));
	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass1, "dwrrTrafficClass1"));
	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass2, "dwrrTrafficClass2"));
	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass3, "dwrrTrafficClass3"));
	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass4, "dwrrTrafficClass4"));
	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass5, "dwrrTrafficClass5"));
	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass6, "dwrrTrafficClass6"));
	addSerializableAttribute(
			new AttributeUI32(&m_dwrrTrafficClass7, "dwrrTrafficClass7"));

	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass0,
					"shapingRateTrafficClass0"));
	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass1,
					"shapingRateTrafficClass1"));
	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass2,
					"shapingRateTrafficClass2"));
	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass3,
					"shapingRateTrafficClass3"));
	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass4,
					"shapingRateTrafficClass4"));
	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass5,
					"shapingRateTrafficClass5"));
	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass6,
					"shapingRateTrafficClass6"));
	addSerializableAttribute(
			new AttributeUI64(&m_shapingRateTrafficClass7,
					"shapingRateTrafficClass7"));

}

void PolicerClientPolicyClassMapMessage::setPolicyClassMapName(
		const string &policyClassMapName) {
	m_policyClassMapName = policyClassMapName;
}

string PolicerClientPolicyClassMapMessage::getPolicyClassMapName() const {
	return (m_policyClassMapName);
}

void PolicerClientPolicyClassMapMessage::setCir(const UI64 &cir) {
	m_cir = cir;
}

UI64 PolicerClientPolicyClassMapMessage::getCir() const {
	return (m_cir);
}

void PolicerClientPolicyClassMapMessage::setCbs(const UI64 &cbs) {
	m_cbs = cbs;
}

UI64 PolicerClientPolicyClassMapMessage::getCbs() const {
	return (m_cbs);
}

void PolicerClientPolicyClassMapMessage::setEir(const UI64 &eir) {
	m_eir = eir;
}

UI64 PolicerClientPolicyClassMapMessage::getEir() const {
	return (m_eir);
}

void PolicerClientPolicyClassMapMessage::setEbs(const UI64 &ebs) {
	m_ebs = ebs;
}

UI64 PolicerClientPolicyClassMapMessage::getEbs() const {
	return (m_ebs);
}

void PolicerClientPolicyClassMapMessage::setConformDscp(
		const UI32 &conformDscp) {
	m_conformDscp = conformDscp;
}

UI32 PolicerClientPolicyClassMapMessage::getConformDscp() const {
	return (m_conformDscp);
}

void PolicerClientPolicyClassMapMessage::setConformPrecedence(
		const UI32 &conformPrecedence) {
	m_conformPrecedence = conformPrecedence;
}

UI32 PolicerClientPolicyClassMapMessage::getConformPrecedence() const {
	return (m_conformPrecedence);
}

void PolicerClientPolicyClassMapMessage::setConformType(
		const UI32 &conformType) {
	m_conformType = conformType;
}

UI32 PolicerClientPolicyClassMapMessage::getConformType() const {
	return (m_conformType);
}

void PolicerClientPolicyClassMapMessage::setConformTc(const UI32 &conformTc) {
	m_conformTc = conformTc;
}

UI32 PolicerClientPolicyClassMapMessage::getConformTc() const {
	return (m_conformTc);
}

void PolicerClientPolicyClassMapMessage::setExceedDscp(const UI32 &exceedDscp) {
	m_exceedDscp = exceedDscp;
}

UI32 PolicerClientPolicyClassMapMessage::getExceedDscp() const {
	return (m_exceedDscp);
}

void PolicerClientPolicyClassMapMessage::setExceedPrecedence(
		const UI32 &exceedPrecedence) {
	m_exceedPrecedence = exceedPrecedence;
}

UI32 PolicerClientPolicyClassMapMessage::getExceedPrecedence() const {
	return (m_exceedPrecedence);
}

void PolicerClientPolicyClassMapMessage::setExceedType(const UI32 &exceedType) {
	m_exceedType = exceedType;
}

UI32 PolicerClientPolicyClassMapMessage::getExceedType() const {
	return (m_exceedType);
}

void PolicerClientPolicyClassMapMessage::setExceedTc(const UI32 &exceedTc) {
	m_exceedTc = exceedTc;
}

UI32 PolicerClientPolicyClassMapMessage::getExceedTc() const {
	return (m_exceedTc);
}
void PolicerClientPolicyClassMapMessage::setClassMapId(
		const string &classMapId) {
	m_classMapId = classMapId;
}

string PolicerClientPolicyClassMapMessage::getClassMapId() const {
	return (m_classMapId);
}

void PolicerClientPolicyClassMapMessage::setPriorityMapId(
		const string &priorityMapId) {
	m_priorityMapId = priorityMapId;
}

string PolicerClientPolicyClassMapMessage::getPriorityMapId() const {
	return (m_priorityMapId);
}

void PolicerClientPolicyClassMapMessage::setOpcode(const UI32 &opcode) {
	m_opcode = opcode;
}

UI32 PolicerClientPolicyClassMapMessage::getOpcode() const {
	return (m_opcode);
}

void PolicerClientPolicyClassMapMessage::setCoS(const UI8 &cos) {
	m_cos = cos;
}

UI8 PolicerClientPolicyClassMapMessage::getCoS() const {
	return m_cos;
}

void PolicerClientPolicyClassMapMessage::setTrafficClass(
		const UI8 &trafficClass) {
	m_trafficClass = trafficClass;
}
UI8 PolicerClientPolicyClassMapMessage::getTrafficClass() const {
	return m_trafficClass;
}

void PolicerClientPolicyClassMapMessage::setDscp(const UI16 &dscp) {
	m_dscp = dscp;
}
UI16 PolicerClientPolicyClassMapMessage::getDscp() const {
	return m_dscp;
}

void PolicerClientPolicyClassMapMessage::setCoSMutationMapName(
		const string &CoSMutationMapName) {
	m_CoSMutationMapName = CoSMutationMapName;
}

string PolicerClientPolicyClassMapMessage::getCoSMutationMapName() const {
	return m_CoSMutationMapName;
}

void PolicerClientPolicyClassMapMessage::setCoSTrafficClassMapName(
		const string &cosTrafficClassMapName) {
	m_CoSTrafficClassMapName = cosTrafficClassMapName;
}
string PolicerClientPolicyClassMapMessage::getCoSTrafficClassMapName() const {
	return m_CoSTrafficClassMapName;
}
void PolicerClientPolicyClassMapMessage::setDscpCoSMapName(
		const string &dscpCoSMapName) {
	m_dscpCoSMapName = dscpCoSMapName;
}
string PolicerClientPolicyClassMapMessage::getDscpCoSMapName() const {
	return m_dscpCoSMapName;;
}
void PolicerClientPolicyClassMapMessage::setDscpMutationMapName(
		const string &dscpMutationMapName) {
	m_dscpMutationMapName = dscpMutationMapName;
}
string PolicerClientPolicyClassMapMessage::getDscpMutationMapName() const {
	return m_dscpMutationMapName;
}
void PolicerClientPolicyClassMapMessage::setDscpTrafficClassMapName(
		const string &dscpTrafficClassMapName) {
	m_dscpTrafficClassMapName = dscpTrafficClassMapName;
}
string PolicerClientPolicyClassMapMessage::getDscpTrafficClassMapName() const {
	return m_dscpTrafficClassMapName;
}
void PolicerClientPolicyClassMapMessage::setSflowMapName(
		const string &sflowMapName) {
	m_sflowMapName = sflowMapName;
}
string PolicerClientPolicyClassMapMessage::getSflowMapName() const {
	return m_sflowMapName;
}
void PolicerClientPolicyClassMapMessage::setCeeMapName(
		const string &ceeMapName) {
	m_ceeMapName = ceeMapName;
}
string PolicerClientPolicyClassMapMessage::getCeeMapName() const {
	return m_ceeMapName;
}

void PolicerClientPolicyClassMapMessage::setShapingRate(
		const UI64 &shapingRate) {
	m_shapingRate = shapingRate;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRate() const {
	return m_shapingRate;
}

void PolicerClientPolicyClassMapMessage::setStrictPriority(
		const UI8 &strictPriority) {
	m_strictPriority = strictPriority;
}
UI8 PolicerClientPolicyClassMapMessage::getStrictPriority() const {
	return m_strictPriority;
}

void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass0(
		const UI32 &dwrrTrafficClass0) {
	m_dwrrTrafficClass0 = dwrrTrafficClass0;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass0() const {
	return m_dwrrTrafficClass0;
}
void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass1(
		const UI32 &dwrrTrafficClass1) {
	m_dwrrTrafficClass1 = dwrrTrafficClass1;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass1() const {
	return m_dwrrTrafficClass1;
}
void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass2(
		const UI32 &dwrrTrafficClass2) {
	m_dwrrTrafficClass2 = dwrrTrafficClass2;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass2() const {
	return m_dwrrTrafficClass2;
}
void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass3(
		const UI32 &dwrrTrafficClass3) {
	m_dwrrTrafficClass3 = dwrrTrafficClass3;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass3() const {
	return m_dwrrTrafficClass3;
}
void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass4(
		const UI32 &dwrrTrafficClass4) {
	m_dwrrTrafficClass4 = dwrrTrafficClass4;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass4() const {
	return m_dwrrTrafficClass4;
}
void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass5(
		const UI32 &dwrrTrafficClass5) {
	m_dwrrTrafficClass5 = dwrrTrafficClass5;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass5() const {
	return m_dwrrTrafficClass5;
}
void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass6(
		const UI32 &dwrrTrafficClass6) {
	m_dwrrTrafficClass6 = dwrrTrafficClass6;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass6() const {
	return m_dwrrTrafficClass6;
}
void PolicerClientPolicyClassMapMessage::setDwrrTrafficClass7(
		const UI32 &dwrrTrafficClass7) {
	m_dwrrTrafficClass7 = dwrrTrafficClass7;
}
UI32 PolicerClientPolicyClassMapMessage::getDwrrTrafficClass7() const {
	return m_dwrrTrafficClass7;
}

void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass0(
		const UI64 &shapingRateTrafficClass0) {
	m_shapingRateTrafficClass0 = shapingRateTrafficClass0;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass0() const {
	return m_shapingRateTrafficClass0;
}
void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass1(
		const UI64 &shapingRateTrafficClass1) {
	m_shapingRateTrafficClass1 = shapingRateTrafficClass1;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass1() const {
	return m_shapingRateTrafficClass1;
}
void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass2(
		const UI64 &shapingRateTrafficClass2) {
	m_shapingRateTrafficClass2 = shapingRateTrafficClass2;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass2() const {
	return m_shapingRateTrafficClass2;
}
void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass3(
		const UI64 &shapingRateTrafficClass3) {
	m_shapingRateTrafficClass3 = shapingRateTrafficClass3;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass3() const {
	return m_shapingRateTrafficClass3;
}
void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass4(
		const UI64 &shapingRateTrafficClass4) {
	m_shapingRateTrafficClass4 = shapingRateTrafficClass4;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass4() const {
	return m_shapingRateTrafficClass4;
}
void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass5(
		const UI64 &shapingRateTrafficClass5) {
	m_shapingRateTrafficClass5 = shapingRateTrafficClass5;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass5() const {
	return m_shapingRateTrafficClass5;
}
void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass6(
		const UI64 &shapingRateTrafficClass6) {
	m_shapingRateTrafficClass6 = shapingRateTrafficClass6;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass6() const {
	return m_shapingRateTrafficClass6;
}
void PolicerClientPolicyClassMapMessage::setShapingRateTrafficClass7(
		const UI64 &shapingRateTrafficClass7) {
	m_shapingRateTrafficClass7 = shapingRateTrafficClass7;
}
UI64 PolicerClientPolicyClassMapMessage::getShapingRateTrafficClass7() const {
	return m_shapingRateTrafficClass7;
}

const void *PolicerClientPolicyClassMapMessage::getCStructureForInputs() {

	/*trace(
			TRACE_LEVEL_INFO,
			string(
					"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ")
					+ "\n");

	trace(
			TRACE_LEVEL_INFO,
			string(
					"PolicerClientPolicyClassMapMessage::getCStructureForInputs: cmdcode<")
					+ m_opcode + ">\n");*/
	//Use switch statement, based on which decide what structure to be populated.

	switch (m_opcode) {

	case FB_POLICYMAP_CREATE:
	case FB_POLICYMAP_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *pInput =
				(fbDcmPolicyClassMapActionMessage_t *) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));
		pInput->opcode = (fbDcmCommandCode_t) m_opcode;
		strncpy((pInput->policy_name), m_policyClassMapName.c_str(),
				FB_MAX_MAP_NAME_SIZE - 1);
		pInput->policy_name[FB_MAX_MAP_NAME_SIZE - 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_POLICYMAP_CREATE or FB_POLICYMAP_DELETE : ")
						+ pInput->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ (pInput->policy_name) + "\n");*/

		return (pInput);
		break;
	}

	case FB_ADD_CLASS_MAP_TO_POLICY_MAP:
	case FB_DEL_CLASS_MAP_TO_POLICY_MAP: {

		fbDcmPolicyClassMapActionMessage_t *pInput =
				(fbDcmPolicyClassMapActionMessage_t *) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));
		pInput->opcode = (fbDcmCommandCode_t) m_opcode;
		strncpy((pInput->policy_name), m_policyClassMapName.c_str(),
				FB_MAX_MAP_NAME_SIZE - 1);
		pInput->policy_name[FB_MAX_MAP_NAME_SIZE - 1] = '\0';
		strncpy((pInput->class_name).name, m_classMapId.c_str(),
				FB_MAX_MAP_NAME_SIZE - 1);
		(pInput->class_name).name[FB_MAX_MAP_NAME_SIZE - 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_ADD_CLASS_MAP_TO_POLICY_MAP or FB_DEL_CLASS_MAP_TO_POLICY_MAP: ")
						+ pInput->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ (pInput->policy_name) + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ (pInput->class_name).name + "\n");*/

		return (pInput);
		break;
	}

	case FB_SET_POLICER_ATTR:
	case FB_CLEAR_POLICER_ATTR: {

		fbDcmPolicyClassMapActionMessage_t *pInput =
				(fbDcmPolicyClassMapActionMessage_t *) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		pInput->opcode = (fbDcmCommandCode_t) m_opcode;
		strncpy((pInput->policy_name), m_policyClassMapName.c_str(),
				FB_MAX_MAP_NAME_SIZE - 1);
		pInput->policy_name[FB_MAX_MAP_NAME_SIZE - 1] = '\0';
		strncpy((pInput->class_name).name, m_classMapId.c_str(),
				FB_MAX_MAP_NAME_SIZE - 1);
		(pInput->class_name).name[FB_MAX_MAP_NAME_SIZE - 1] = '\0';
		strncpy(pInput->action.police_attr.prio_map_name,
				m_priorityMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		pInput->action.police_attr.prio_map_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		pInput->action.police_attr.cir = m_cir;
		pInput->action.police_attr.cbs = m_cbs;
		pInput->action.police_attr.eir = m_eir;
		pInput->action.police_attr.ebs = m_ebs;
		pInput->action.police_attr.conform_traffic_class = m_conformTc;
		pInput->action.police_attr.exceed_traffic_class = m_exceedTc;
		pInput->action.police_attr.set_conform_tc = 0;
		pInput->action.police_attr.set_exceed_tc = 0;
		//trace(TRACE_LEVEL_DEBUG, string("m_confrom_type")+ m_conformType);

		pInput->action.police_attr.conform_ip_tos = POLICER_NOT_USR_CFGED_VAL;
		pInput->action.police_attr.set_conform_dscp = 0;
		pInput->action.police_attr.set_exceed_ip_prec = 0;
		pInput->action.police_attr.set_exceed_dscp = 0;
		pInput->action.police_attr.set_conform_ip_prec = 0;
		pInput->action.police_attr.exceed_ip_tos = POLICER_NOT_USR_CFGED_VAL;

		if (m_conformTc != POLICER_NOT_USR_CFGED_VAL)
			pInput->action.police_attr.set_conform_tc = 1;

		if (m_exceedTc != POLICER_NOT_USR_CFGED_VAL)
			pInput->action.police_attr.set_exceed_tc = 1;

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_SET_POLICER_ATTR or FB_CLEAR_POLICER_ATTR : ")
						+ pInput->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ (pInput->policy_name) + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ (pInput->class_name).name + "\n");*/

		if (m_conformType == CONFORM_PREC) {
			pInput->action.police_attr.conform_ip_tos = m_conformPrecedence;
			pInput->action.police_attr.set_conform_ip_prec = 1;
			pInput->action.police_attr.set_conform_dscp = 0;
		} else if (m_conformType == CONFORM_DSCP) {
			pInput->action.police_attr.set_conform_ip_prec = 0;
			pInput->action.police_attr.set_conform_dscp = 1;
			pInput->action.police_attr.conform_ip_tos = m_conformDscp;
		}

		if (m_exceedType == EXCEED_PREC) {
			pInput->action.police_attr.exceed_ip_tos = m_exceedPrecedence;
			pInput->action.police_attr.set_exceed_ip_prec = 1;
			pInput->action.police_attr.set_exceed_dscp = 0;
		} else if (m_exceedType == EXCEED_DSCP) {
			pInput->action.police_attr.exceed_ip_tos = m_exceedDscp;
			pInput->action.police_attr.set_exceed_dscp = 1;
			pInput->action.police_attr.set_exceed_ip_prec = 0;
		}
		return (pInput);
		break;
	}

	case FB_SET_COS_TC_ACTION_CREATE:
	case FB_SET_COS_TC_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		policyClassMapActionMessage->action.cos_attr.cos = m_cos;
		policyClassMapActionMessage->action.cos_attr.tc = m_trafficClass;

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_SET_COS_TC_ACTION_CREATE or FB_SET_COS_TC_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: CoS Value : ")
						+ policyClassMapActionMessage->action.cos_attr.cos
						+ "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: TC Value : ")
						+ policyClassMapActionMessage->action.cos_attr.tc
						+ "\n");*/

		return (policyClassMapActionMessage);
		break;
	}

	case FB_SET_DSCP_ACTION_CREATE:
	case FB_SET_DSCP_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		policyClassMapActionMessage->action.dscp_attr.dscp = m_dscp;

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_SET_DSCP_ACTION_CREATE or FB_SET_DSCP_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: DSCP Value : ")
						+ policyClassMapActionMessage->action.dscp_attr.dscp
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_COS_MUTATION_MAP_ACTION_CREATE:
	case FB_COS_MUTATION_MAP_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->action.cosmutation_attr.map_name,
				m_CoSMutationMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->action.cosmutation_attr.map_name[FB_MAX_MAP_NAME_SIZE
				- 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_COS_MUTATION_MAP_ACTION_CREATE or FB_COS_MUTATION_MAP_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: CoSMutationMapName : ")
						+ policyClassMapActionMessage->action.cosmutation_attr.map_name
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_COS_TRAFFIC_CLASS_MAP_ACTION_CREATE:
	case FB_COS_TRAFFIC_CLASS_MAP_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(
				policyClassMapActionMessage->action.costrafficclass_attr.map_name,
				m_CoSTrafficClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->action.costrafficclass_attr.map_name[FB_MAX_MAP_NAME_SIZE
				- 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_COS_TRAFFIC_CLASS_MAP_ACTION_CREATE or FB_COS_TRAFFIC_CLASS_MAP_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: CoSTrafficClassMapName : ")
						+ policyClassMapActionMessage->action.costrafficclass_attr.map_name
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_DSCP_COS_MAP_ACTION_CREATE:
	case FB_DSCP_COS_MAP_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->action.dscpcos_attr.map_name,
				m_dscpCoSMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->action.dscpcos_attr.map_name[FB_MAX_MAP_NAME_SIZE
				- 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_DSCP_COS_MAP_ACTION_CREATE or FB_DSCP_COS_MAP_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: dscpCoSMapName : ")
						+ policyClassMapActionMessage->action.dscpcos_attr.map_name
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_DSCP_MUTATION_MAP_ACTION_CREATE:
	case FB_DSCP_MUTATION_MAP_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->action.dscpmutation_attr.map_name,
				m_dscpMutationMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->action.dscpmutation_attr.map_name[FB_MAX_MAP_NAME_SIZE
				- 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_DSCP_MUTATION_MAP_ACTION_CREATE or FB_DSCP_MUTATION_MAP_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: dscpMutationMapName : ")
						+ policyClassMapActionMessage->action.dscpmutation_attr.map_name
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_CREATE:
	case FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(
				policyClassMapActionMessage->action.dscptrafficclass_attr.map_name,
				m_dscpTrafficClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->action.dscptrafficclass_attr.map_name[FB_MAX_MAP_NAME_SIZE
				- 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_CREATE or FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: dscpTrafficClassMapName : ")
						+ policyClassMapActionMessage->action.dscptrafficclass_attr.map_name
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_SFLOW_MAP_ACTION_CREATE:
	case FB_SFLOW_MAP_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->action.sflow_attr.map_name,
				m_sflowMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->action.sflow_attr.map_name[FB_MAX_MAP_NAME_SIZE
				- 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_SFLOW_MAP_ACTION_CREATE or  FB_SFLOW_MAP_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: sflowMapName : ")
						+ policyClassMapActionMessage->action.sflow_attr.map_name
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_CEE_ACTION_CREATE:
	case FB_CEE_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->action.cee_attr.map_name,
				m_ceeMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->action.cee_attr.map_name[FB_MAX_MAP_NAME_SIZE
				- 1] = '\0';

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_CEE_ACTION_CREATE or FB_CEE_ACTION_DELETE: ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ceeMapName : ")
						+ policyClassMapActionMessage->action.cee_attr.map_name
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_SHAPING_RATE_ACTION_CREATE:
	case FB_SHAPING_RATE_ACTION_DELETE: {
		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';
		policyClassMapActionMessage->action.portshape_attr.shape_rate =
				m_shapingRate;

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_SHAPING_RATE_ACTION_CREATE or FB_SHAPING_RATE_ACTION_DELETE : ")
						+ policyClassMapActionMessage->opcode + "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ShapingRate : ")
						+ policyClassMapActionMessage->action.portshape_attr.shape_rate
						+ "\n");*/
		return (policyClassMapActionMessage);
		break;
	}

	case FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE:
	case FB_QOS_QUEUE_SCHEDULER_ACTION_DELETE: {

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: obtained  : FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE or FB_QOS_QUEUE_SCHEDULER_ACTION_DELETE"));*/

		fbDcmPolicyClassMapActionMessage_t *policyClassMapActionMessage =
				(fbDcmPolicyClassMapActionMessage_t*) malloc(
						sizeof(fbDcmPolicyClassMapActionMessage_t));

		policyClassMapActionMessage->opcode = (fbDcmCommandCode_t) m_opcode;

		strncpy(policyClassMapActionMessage->policy_name,
				m_policyClassMapName.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->policy_name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';

		strncpy(policyClassMapActionMessage->class_name.name,
				m_classMapId.c_str(), FB_MAX_MAP_NAME_SIZE - 1);
		policyClassMapActionMessage->class_name.name[FB_MAX_MAP_NAME_SIZE - 1] =
				'\0';

		policyClassMapActionMessage->action.schd_attr.sp_count =
				m_strictPriority;

		policyClassMapActionMessage->action.schd_attr.dwrr_wt[0] =
				m_dwrrTrafficClass0;
		policyClassMapActionMessage->action.schd_attr.dwrr_wt[1] =
				m_dwrrTrafficClass1;
		policyClassMapActionMessage->action.schd_attr.dwrr_wt[2] =
				m_dwrrTrafficClass2;
		policyClassMapActionMessage->action.schd_attr.dwrr_wt[3] =
				m_dwrrTrafficClass3;
		policyClassMapActionMessage->action.schd_attr.dwrr_wt[4] =
				m_dwrrTrafficClass4;
		policyClassMapActionMessage->action.schd_attr.dwrr_wt[5] =
				m_dwrrTrafficClass5;
		policyClassMapActionMessage->action.schd_attr.dwrr_wt[6] =
				m_dwrrTrafficClass6;
		policyClassMapActionMessage->action.schd_attr.dwrr_wt[7] =
				m_dwrrTrafficClass7;

		policyClassMapActionMessage->action.schd_attr.shape_bw[0] =
				m_shapingRateTrafficClass0;
		policyClassMapActionMessage->action.schd_attr.shape_bw[1] =
				m_shapingRateTrafficClass1;
		policyClassMapActionMessage->action.schd_attr.shape_bw[2] =
				m_shapingRateTrafficClass2;
		policyClassMapActionMessage->action.schd_attr.shape_bw[3] =
				m_shapingRateTrafficClass3;
		policyClassMapActionMessage->action.schd_attr.shape_bw[4] =
				m_shapingRateTrafficClass4;
		policyClassMapActionMessage->action.schd_attr.shape_bw[5] =
				m_shapingRateTrafficClass5;
		policyClassMapActionMessage->action.schd_attr.shape_bw[6] =
				m_shapingRateTrafficClass6;
		policyClassMapActionMessage->action.schd_attr.shape_bw[7] =
				m_shapingRateTrafficClass7;

		/*trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: OPCODE FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE or FB_QOS_QUEUE_SCHEDULER_ACTION_DELETE : ")
						+ policyClassMapActionMessage->opcode + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: PolicyMapName : ")
						+ policyClassMapActionMessage->policy_name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ClassMapName : ")
						+ policyClassMapActionMessage->class_name.name + "\n");
		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: SP COUNT : ")
						+ policyClassMapActionMessage->action.schd_attr.sp_count
						+ "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: DWRR : ")
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[0]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[1]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[2]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[3]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[4]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[5]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[6]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.dwrr_wt[7]
						+ "\n");

		trace(
				TRACE_LEVEL_INFO,
				string(
						"PolicerClientPolicyClassMapMessage::getCStructureForInputs: ShapingRates : ")
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[0]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[1]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[2]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[3]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[4]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[5]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[6]
						+ "<"
						+ policyClassMapActionMessage->action.schd_attr.shape_bw[7]
						+ "\n");*/

		return (policyClassMapActionMessage);
		break;
	}

	}
	return (NULL);
}

void PolicerClientPolicyClassMapMessage::loadOutputsFromCStructure(
		const void *pOutputCStructure) {
}
}
