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

#include "Policer/Global/PolicerGlobalPolicyClassMapMessage.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/PolicerGlobalTypes.h"

namespace DcmNs
{

    PolicerGlobalPolicyClassMapMessage::PolicerGlobalPolicyClassMapMessage ()
        : ManagementInterfaceMessage (PolicerGlobalObjectManager::getClassName (),POLICERGLOBALPOLICYCLASSMAP)
    {
    }

    PolicerGlobalPolicyClassMapMessage::PolicerGlobalPolicyClassMapMessage(
		const string &policyClassMapName, const UI64 &cir, const UI64 &cbs,
		const UI64 &eir, const UI64 &ebs, const UI32 &conformDscp,
		const UI32 &conformPrecedence, const UI32 &conformType,
		const UI32 &conformTc, const UI32 &exceedDscp,
		const UI32 &exceedPrecedence, const UI32 &exceedType,
		const UI32 &exceedTc, const string &classMapId,
		const string &priorityMapId, const UI32 &opcode, const UI8 &cos,
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
		const UI64 &shapingRateTrafficClass7)
        : ManagementInterfaceMessage (PolicerGlobalObjectManager::getClassName (),POLICERGLOBALPOLICYCLASSMAP),
        m_policyClassMapName    (policyClassMapName),
        m_cir    (cir),
        m_cbs    (cbs),
        m_eir    (eir),
        m_ebs    (ebs),
        m_conformDscp    (conformDscp),
        m_conformPrecedence    (conformPrecedence),
		m_conformType	(conformType),
        m_conformTc    (conformTc),
        m_exceedDscp    (exceedDscp),
        m_exceedPrecedence    (exceedPrecedence),
		m_exceedType 	(exceedType),
        m_exceedTc    (exceedTc),
        m_classMapId    (classMapId),
        m_priorityMapId    (priorityMapId),
        m_opcode    (opcode), m_cos(cos), m_trafficClass(
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
				shapingRateTrafficClass7)

    {
    }

    PolicerGlobalPolicyClassMapMessage::~PolicerGlobalPolicyClassMapMessage ()
    {
    }

    void  PolicerGlobalPolicyClassMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_policyClassMapName,"policyClassMapName"));
        addSerializableAttribute (new AttributeUI64(&m_cir,"cir"));
        addSerializableAttribute (new AttributeUI64(&m_cbs,"cbs"));
        addSerializableAttribute (new AttributeUI64(&m_eir,"eir"));
        addSerializableAttribute (new AttributeUI64(&m_ebs,"ebs"));
        addSerializableAttribute (new AttributeUI32(&m_conformDscp,"conformDscp"));
        addSerializableAttribute (new AttributeUI32(&m_conformPrecedence,"conformPrecedence"));
        addSerializableAttribute (new AttributeUI32(&m_conformType,"conformType"));
		addSerializableAttribute (new AttributeUI32(&m_conformTc,"conformTc"));
        addSerializableAttribute (new AttributeUI32(&m_exceedDscp,"exceedDscp"));
        addSerializableAttribute (new AttributeUI32(&m_exceedPrecedence,"exceedPrecedence"));
		addSerializableAttribute (new AttributeUI32(&m_exceedType,"exceedType"));
        addSerializableAttribute (new AttributeUI32(&m_exceedTc,"exceedTc"));
        addSerializableAttribute (new AttributeString(&m_classMapId,"classMapId"));
        addSerializableAttribute (new AttributeString(&m_priorityMapId,"priorityMapId"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));

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
    			new AttributeString(&m_sflowMapName, "sflowMapName"));
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

    void  PolicerGlobalPolicyClassMapMessage::setPolicyClassMapName(const string &policyClassMapName)
    {
        m_policyClassMapName  =  policyClassMapName;
    }

    string  PolicerGlobalPolicyClassMapMessage::getPolicyClassMapName() const
    {
        return (m_policyClassMapName);
    }

    void  PolicerGlobalPolicyClassMapMessage::setCir(const UI64 &cir)
    {
        m_cir  =  cir;
    }

    UI64  PolicerGlobalPolicyClassMapMessage::getCir() const
    {
        return (m_cir);
    }

    void  PolicerGlobalPolicyClassMapMessage::setCbs(const UI64 &cbs)
    {
        m_cbs  =  cbs;
    }

    UI64  PolicerGlobalPolicyClassMapMessage::getCbs() const
    {
        return (m_cbs);
    }

    void  PolicerGlobalPolicyClassMapMessage::setEir(const UI64 &eir)
    {
        m_eir  =  eir;
    }

    UI64  PolicerGlobalPolicyClassMapMessage::getEir() const
    {
        return (m_eir);
    }

    void  PolicerGlobalPolicyClassMapMessage::setEbs(const UI64 &ebs)
    {
        m_ebs  =  ebs;
    }

    UI64  PolicerGlobalPolicyClassMapMessage::getEbs() const
    {
        return (m_ebs);
    }

    void  PolicerGlobalPolicyClassMapMessage::setConformDscp(const UI32 &conformDscp)
    {
        m_conformDscp  =  conformDscp;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getConformDscp() const
    {
        return (m_conformDscp);
    }

    void  PolicerGlobalPolicyClassMapMessage::setConformPrecedence(const UI32 &conformPrecedence)
    {
        m_conformPrecedence  =  conformPrecedence;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getConformPrecedence() const
    {
        return (m_conformPrecedence);
    }

	void  PolicerGlobalPolicyClassMapMessage::setConformType(const UI32 &conformType)
    {
        m_conformType  =  conformType;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getConformType() const
    {
        return (m_conformType);
    }

    void  PolicerGlobalPolicyClassMapMessage::setConformTc(const UI32 &conformTc)
    {
        m_conformTc  =  conformTc;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getConformTc() const
    {
        return (m_conformTc);
    }

    void  PolicerGlobalPolicyClassMapMessage::setExceedDscp(const UI32 &exceedDscp)
    {
        m_exceedDscp  =  exceedDscp;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getExceedDscp() const
    {
        return (m_exceedDscp);
    }

    void  PolicerGlobalPolicyClassMapMessage::setExceedPrecedence(const UI32 &exceedPrecedence)
    {
        m_exceedPrecedence  =  exceedPrecedence;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getExceedPrecedence() const
    {
        return (m_exceedPrecedence);
    }

	void  PolicerGlobalPolicyClassMapMessage::setExceedType(const UI32 &exceedType)
    {
        m_exceedType  =  exceedType;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getExceedType() const
    {
        return (m_exceedType);
    }

    void  PolicerGlobalPolicyClassMapMessage::setExceedTc(const UI32 &exceedTc)
    {
        m_exceedTc  =  exceedTc;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getExceedTc() const
    {
        return (m_exceedTc);
    }

    void  PolicerGlobalPolicyClassMapMessage::setClassMapId(const string &classMapId)
    {
        m_classMapId  =  classMapId;
    }

    string  PolicerGlobalPolicyClassMapMessage::getClassMapId() const
    {
        return (m_classMapId);
    }

    void  PolicerGlobalPolicyClassMapMessage::setPriorityMapId(const string &priorityMapId)
    {
        m_priorityMapId  =  priorityMapId;
    }

    string  PolicerGlobalPolicyClassMapMessage::getPriorityMapId() const
    {
        return (m_priorityMapId);
    }

    void  PolicerGlobalPolicyClassMapMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  PolicerGlobalPolicyClassMapMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void PolicerGlobalPolicyClassMapMessage::setCoS(const UI8 &cos) {
    	m_cos = cos;
    }

    UI8 PolicerGlobalPolicyClassMapMessage::getCoS() const {
    	return m_cos;
    }

    void PolicerGlobalPolicyClassMapMessage::setTrafficClass(
    		const UI8 &trafficClass) {
    	m_trafficClass = trafficClass;
    }
    UI8 PolicerGlobalPolicyClassMapMessage::getTrafficClass() const {
    	return m_trafficClass;
    }

    void PolicerGlobalPolicyClassMapMessage::setDscp(const UI16 &dscp) {
    	m_dscp = dscp;
    }
    UI16 PolicerGlobalPolicyClassMapMessage::getDscp() const {
    	return m_dscp;
    }

    void PolicerGlobalPolicyClassMapMessage::setCoSMutationMapName(
    		const string &CoSMutationMapName) {
    	m_CoSMutationMapName = CoSMutationMapName;
    }

    string PolicerGlobalPolicyClassMapMessage::getCoSMutationMapName() const {
    	return m_CoSMutationMapName;
    }

    void PolicerGlobalPolicyClassMapMessage::setCoSTrafficClassMapName(
    		const string &cosTrafficClassMapName) {
    	m_CoSTrafficClassMapName = cosTrafficClassMapName;
    }
    string PolicerGlobalPolicyClassMapMessage::getCoSTrafficClassMapName() const {
    	return m_CoSTrafficClassMapName;
    }
    void PolicerGlobalPolicyClassMapMessage::setDscpCoSMapName(
    		const string &dscpCoSMapName) {
    	m_dscpCoSMapName = dscpCoSMapName;
    }
    string PolicerGlobalPolicyClassMapMessage::getDscpCoSMapName() const {
    	return m_dscpCoSMapName;;
    }
    void PolicerGlobalPolicyClassMapMessage::setDscpMutationMapName(
    		const string &dscpMutationMapName) {
    	m_dscpMutationMapName = dscpMutationMapName;
    }
    string PolicerGlobalPolicyClassMapMessage::getDscpMutationMapName() const {
    	return m_dscpMutationMapName;
    }
    void PolicerGlobalPolicyClassMapMessage::setDscpTrafficClassMapName(
    		const string &dscpTrafficClassMapName) {
    	m_dscpTrafficClassMapName = dscpTrafficClassMapName;
    }
    string PolicerGlobalPolicyClassMapMessage::getDscpTrafficClassMapName() const {
    	return m_dscpTrafficClassMapName;
    }
    void PolicerGlobalPolicyClassMapMessage::setsSlowMapName(
    		const string &sflowMapName) {
    	m_sflowMapName = sflowMapName;
    }
    string PolicerGlobalPolicyClassMapMessage::getSflowMapName() const {
    	return m_sflowMapName;
    }
    void PolicerGlobalPolicyClassMapMessage::setCeeMapName(
    		const string &ceeMapName) {
    	m_ceeMapName = ceeMapName;
    }
    string PolicerGlobalPolicyClassMapMessage::getCeeMapName() const {
    	return m_ceeMapName;
    }

    void PolicerGlobalPolicyClassMapMessage::setShapingRate(
    		const UI64 &shapingRate) {
    	m_shapingRate = shapingRate;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRate() const {
    	return m_shapingRate;
    }

    void PolicerGlobalPolicyClassMapMessage::setStrictPriority(
    		const UI8 &strictPriority) {
    	m_strictPriority = strictPriority;
    }
    UI8 PolicerGlobalPolicyClassMapMessage::getStrictPriority() const {
    	return m_strictPriority;
    }

    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass0(
    		const UI32 &dwrrTrafficClass0) {
    	m_dwrrTrafficClass0 = dwrrTrafficClass0;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass0() const {
    	return m_dwrrTrafficClass0;
    }
    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass1(
    		const UI32 &dwrrTrafficClass1) {
    	m_dwrrTrafficClass1 = dwrrTrafficClass1;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass1() const {
    	return m_dwrrTrafficClass1;
    }
    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass2(
    		const UI32 &dwrrTrafficClass2) {
    	m_dwrrTrafficClass2 = dwrrTrafficClass2;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass2() const {
    	return m_dwrrTrafficClass2;
    }
    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass3(
    		const UI32 &dwrrTrafficClass3) {
    	m_dwrrTrafficClass3 = dwrrTrafficClass3;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass3() const {
    	return m_dwrrTrafficClass3;
    }
    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass4(
    		const UI32 &dwrrTrafficClass4) {
    	m_dwrrTrafficClass4 = dwrrTrafficClass4;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass4() const {
    	return m_dwrrTrafficClass4;
    }
    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass5(
    		const UI32 &dwrrTrafficClass5) {
    	m_dwrrTrafficClass5 = dwrrTrafficClass5;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass5() const {
    	return m_dwrrTrafficClass5;
    }
    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass6(
    		const UI32 &dwrrTrafficClass6) {
    	m_dwrrTrafficClass6 = dwrrTrafficClass6;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass6() const {
    	return m_dwrrTrafficClass6;
    }
    void PolicerGlobalPolicyClassMapMessage::setDwrrTrafficClass7(
    		const UI32 &dwrrTrafficClass7) {
    	m_dwrrTrafficClass7 = dwrrTrafficClass7;
    }
    UI32 PolicerGlobalPolicyClassMapMessage::getDwrrTrafficClass7() const {
    	return m_dwrrTrafficClass7;
    }

    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass0(
    		const UI64 &shapingRateTrafficClass0) {
    	m_shapingRateTrafficClass0 = shapingRateTrafficClass0;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass0() const {
    	return m_shapingRateTrafficClass0;
    }
    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass1(
    		const UI64 &shapingRateTrafficClass1) {
    	m_shapingRateTrafficClass1 = shapingRateTrafficClass1;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass1() const {
    	return m_shapingRateTrafficClass1;
    }
    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass2(
    		const UI64 &shapingRateTrafficClass2) {
    	m_shapingRateTrafficClass2 = shapingRateTrafficClass2;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass2() const {
    	return m_shapingRateTrafficClass2;
    }
    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass3(
    		const UI64 &shapingRateTrafficClass3) {
    	m_shapingRateTrafficClass3 = shapingRateTrafficClass3;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass3() const {
    	return m_shapingRateTrafficClass3;
    }
    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass4(
    		const UI64 &shapingRateTrafficClass4) {
    	m_shapingRateTrafficClass4 = shapingRateTrafficClass4;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass4() const {
    	return m_shapingRateTrafficClass4;
    }
    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass5(
    		const UI64 &shapingRateTrafficClass5) {
    	m_shapingRateTrafficClass5 = shapingRateTrafficClass5;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass5() const {
    	return m_shapingRateTrafficClass5;
    }
    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass6(
    		const UI64 &shapingRateTrafficClass6) {
    	m_shapingRateTrafficClass6 = shapingRateTrafficClass6;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass6() const {
    	return m_shapingRateTrafficClass6;
    }
    void PolicerGlobalPolicyClassMapMessage::setShapingRateTrafficClass7(
    		const UI64 &shapingRateTrafficClass7) {
    	m_shapingRateTrafficClass7 = shapingRateTrafficClass7;
    }
    UI64 PolicerGlobalPolicyClassMapMessage::getShapingRateTrafficClass7() const {
    	return m_shapingRateTrafficClass7;
    }

}
