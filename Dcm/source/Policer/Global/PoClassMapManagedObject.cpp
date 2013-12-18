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

#include "Policer/Global/PoClassMapManagedObject.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/ClassMapManagedObject.h"
#include "Policer/Global/PolicerPriorityMapManagedObject.h"
#include "Policer/Global/PolicerGlobalTypes.h"
#include  "ssm_policer_public.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "vcs.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Sflow/Global/SflowProfileManagedObject.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Qos/Global/CeeMapManagedObject.h"


namespace DcmNs
{

    PoClassMapManagedObject::PoClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (PoClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager)
    {
        //Populate all the default values of attributes in constructor.
    	m_cos = FB_DEFAULT_COS_VALUE;
        m_traffic_class = FB_DEFAULT_TRAFFIC_CLASS_VALUE;
        m_dscp = FB_DEFAULT_DSCP_VALUE;

        m_sflowMapId = ObjectId::NullObjectId;
        m_cosMutationMapId = ObjectId::NullObjectId;
        m_cosTrafficClassMapId = ObjectId::NullObjectId;
        m_dscpMutationMapId = ObjectId::NullObjectId;
        m_dscpCosMapId = ObjectId::NullObjectId;
        m_dscpTrafficClassMapId = ObjectId::NullObjectId;
        m_ceeMapId = ObjectId::NullObjectId;

        m_shapingRate = FB_DEFAULT_SHAPING_RATE_VALUE;

        m_strictPriority = FB_DEFAULT_STRICT_PRIORITY_VALUE;

        m_dwrrTrafficClass0 = FB_DEFAULT_QUEUE_DWRR_VALUE;
        m_dwrrTrafficClass1 = FB_DEFAULT_QUEUE_DWRR_VALUE;
        m_dwrrTrafficClass2 = FB_DEFAULT_QUEUE_DWRR_VALUE;
        m_dwrrTrafficClass3 = FB_DEFAULT_QUEUE_DWRR_VALUE;
        m_dwrrTrafficClass4 = FB_DEFAULT_QUEUE_DWRR_VALUE;
        m_dwrrTrafficClass5 = FB_DEFAULT_QUEUE_DWRR_VALUE;
        m_dwrrTrafficClass6 = FB_DEFAULT_QUEUE_DWRR_VALUE;
        m_dwrrTrafficClassLast = FB_DEFAULT_QUEUE_DWRR_VALUE;

        m_shapingRateTrafficClass0 = FB_DEFAULT_SHAPING_RATE_VALUE;
        m_shapingRateTrafficClass1 = FB_DEFAULT_SHAPING_RATE_VALUE;
        m_shapingRateTrafficClass2 = FB_DEFAULT_SHAPING_RATE_VALUE;
        m_shapingRateTrafficClass3 = FB_DEFAULT_SHAPING_RATE_VALUE;
        m_shapingRateTrafficClass4 = FB_DEFAULT_SHAPING_RATE_VALUE;
        m_shapingRateTrafficClass5 = FB_DEFAULT_SHAPING_RATE_VALUE;
        m_shapingRateTrafficClass6 = FB_DEFAULT_SHAPING_RATE_VALUE;
        m_shapingRateTrafficClass7 = FB_DEFAULT_SHAPING_RATE_VALUE;

        m_scheduler_type = FB_DEFAULT_QUEUE_SCHEDULER_TYPE;
    }

    PoClassMapManagedObject::PoClassMapManagedObject(
		PolicerGlobalObjectManager *pPolicerGlobalObjectManager,
		const UI64UC &cir, const UI64UC &cbs, const UI64UC &eir,
		const UI64UC &ebs, const UI32UC &conformDscp,
		const UI32UC &conformPrecedence, const UI32 &conformType,
		const UI32UC &conformTc, const UI32UC &exceedDscp,
		const UI32UC &exceedPrecedence, const UI32 &exceedType,
		const UI32UC &exceedTc, const ObjectId &classMapId,
		const ObjectId &priorityMapId, const UI8 &cos,
		const UI8 &traffic_class, const UI16 &dscp,
		const ObjectId &sflowMapId, const ObjectId &cosMutationMapId,
		const ObjectId &cosTrafficClassMapId, const ObjectId &dscpMutationMapId,
		const ObjectId &dscpCosMapId, const ObjectId &dscpTrafficClassMapId,
		const ObjectId &ceeMapId,

		const UI64 &shapingRate,

		const UI8 &strictPriority, const UI32 &dwrrTrafficClass0,
		const UI32 &dwrrTrafficClass1, const UI32 &dwrrTrafficClass2,
		const UI32 &dwrrTrafficClass3, const UI32 &dwrrTrafficClass4,
		const UI32 &dwrrTrafficClass5, const UI32 &dwrrTrafficClass6,
		const UI32 &dwrrTrafficClassLast,

		const UI64 &shapingRateTrafficClass0,
		const UI64 &shapingRateTrafficClass1,
		const UI64 &shapingRateTrafficClass2,
		const UI64 &shapingRateTrafficClass3,
		const UI64 &shapingRateTrafficClass4,
		const UI64 &shapingRateTrafficClass5,
		const UI64 &shapingRateTrafficClass6,
		const UI64 &shapingRateTrafficClass7)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (PoClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager),
        m_cir    (cir),
        m_cbs    (cbs),
        m_eir    (eir),
        m_ebs    (ebs),
        m_conformDscp    (conformDscp),
        m_conformPrecedence    (conformPrecedence),
		m_conformType (conformType),
        m_conformTc    (conformTc),
        m_exceedDscp    (exceedDscp),
        m_exceedPrecedence    (exceedPrecedence),
        m_exceedType   (exceedType),
		m_exceedTc    (exceedTc),
        m_classMapId    (classMapId),
        m_priorityMapId    (priorityMapId),

    	m_cos	(cos),
    	m_traffic_class	(traffic_class),
    	m_dscp (dscp),
    	m_sflowMapId (sflowMapId),
    	m_cosMutationMapId (cosMutationMapId),
    	m_cosTrafficClassMapId (cosTrafficClassMapId),
    	m_dscpMutationMapId (dscpMutationMapId),
    	m_dscpCosMapId (dscpCosMapId),
    	m_dscpTrafficClassMapId (dscpTrafficClassMapId),
    	m_ceeMapId (ceeMapId),
    	m_shapingRate (shapingRate),
    	m_strictPriority (strictPriority),
    	m_dwrrTrafficClass0 (dwrrTrafficClass0),
    	m_dwrrTrafficClass1 (dwrrTrafficClass1),
    	m_dwrrTrafficClass2 (dwrrTrafficClass2),
    	m_dwrrTrafficClass3 (dwrrTrafficClass3),
    	m_dwrrTrafficClass4 (dwrrTrafficClass4),
    	m_dwrrTrafficClass5 (dwrrTrafficClass5),
    	m_dwrrTrafficClass6 (dwrrTrafficClass6),
    	m_dwrrTrafficClassLast (dwrrTrafficClassLast),
    	m_shapingRateTrafficClass0 (shapingRateTrafficClass0),
    	m_shapingRateTrafficClass1 (shapingRateTrafficClass1),
    	m_shapingRateTrafficClass2 (shapingRateTrafficClass2),
    	m_shapingRateTrafficClass3 (shapingRateTrafficClass3),
    	m_shapingRateTrafficClass4 (shapingRateTrafficClass4),
    	m_shapingRateTrafficClass5 (shapingRateTrafficClass5),
    	m_shapingRateTrafficClass6 (shapingRateTrafficClass6),
    	m_shapingRateTrafficClass7 (shapingRateTrafficClass7)


    {
    }

    PoClassMapManagedObject::~PoClassMapManagedObject ()
    {
    }

    string  PoClassMapManagedObject::getClassName()
    {
        return ("PoClassMapManagedObject");
    }

    void  PoClassMapManagedObject::setupAttributesForPersistence()
    {
		//UI32 defaultDscp = 0;
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI64UC(&m_cir, true, "cir"));
        addPersistableAttribute (new AttributeUI64UC(&m_cbs, true, "cbs"));
        addPersistableAttribute (new AttributeUI64UC(&m_eir, true, "eir"));
        addPersistableAttribute (new AttributeUI64UC(&m_ebs, true, "ebs"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformDscp,true, "conformDscp"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPrecedence, true, "conformPrecedence"));
        addPersistableAttribute (new AttributeUI32(&m_conformType,"conformType"));
		addPersistableAttribute (new AttributeUI32UC(&m_conformTc, true, "conformTc"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedDscp, true, "exceedDscp"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPrecedence, true, "exceedPrecedence"));
        addPersistableAttribute (new AttributeUI32(&m_exceedType,"exceedType"));
		addPersistableAttribute (new AttributeUI32UC(&m_exceedTc, true, "exceedTc"));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_classMapId,"classMapId", ClassMapManagedObject::getClassName (), true, policer_cl_name));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_priorityMapId,"priorityMapId", PolicerPriorityMapManagedObject::getClassName (), true, policer_set_priority));
		setUserTagForAttribute("cir",policer_cir);
		setUserTagForAttribute("cbs", policer_cbs);
		setUserTagForAttribute("eir",policer_eir);
		setUserTagForAttribute("ebs", policer_ebs);
		setUserTagForAttribute("conformDscp", policer_conform_set_dscp);
		setUserTagForAttribute("exceedDscp", policer_exceed_set_dscp);
		setUserTagForAttribute("conformPrecedence", policer_conform_set_prec);
		setUserTagForAttribute("conformTc", policer_conform_set_tc);
		setUserTagForAttribute("exceedPrecedence",policer_exceed_set_prec);
		setUserTagForAttribute("exceedTc", policer_exceed_set_tc);

		addPersistableAttribute  (new AttributeUI8(&m_cos,FB_DEFAULT_COS_VALUE, "cos",policer_cos));
		addPersistableAttribute  (new AttributeUI8(&m_traffic_class, FB_DEFAULT_TRAFFIC_CLASS_VALUE, "trafficClass", policer_traffic_class));
		addPersistableAttribute  (new AttributeUI16(&m_dscp,FB_DEFAULT_DSCP_VALUE, "dscp", policer_dscp));
		addPersistableAttribute  (new AttributeUI64(&m_shapingRate, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRate", policer_shaping_rate));

		addPersistableAttribute  (new AttributeObjectIdAssociation(&m_sflowMapId,"sflowMapId", SflowProfileManagedObject::getClassName (), true, policer_sflow));
		addPersistableAttribute  (new AttributeObjectIdAssociation(&m_cosMutationMapId,"cosMutationMapId", CosToCosMutationMapManagedObject::getClassName (), true, policer_cos_mutation));
		addPersistableAttribute  (new AttributeObjectIdAssociation(&m_cosTrafficClassMapId,"cosTrafficClassMapId", CosToTrafficClassMapManagedObject::getClassName (), true, policer_cos_traffic_class));
		addPersistableAttribute  (new AttributeObjectIdAssociation(&m_dscpMutationMapId,"dscpMutationMapId", DscpToDscpMutationMapManagedObject::getClassName (), true, policer_dscp_mutation));
		addPersistableAttribute  (new AttributeObjectIdAssociation(&m_dscpCosMapId,"dscpCosMapId", DscpToCosMapManagedObject::getClassName (), true, policer_dscp_cos));
		addPersistableAttribute  (new AttributeObjectIdAssociation(&m_dscpTrafficClassMapId,"dscpTrafficClassMapId", DscpToTrafficClassMapManagedObject::getClassName (), true, policer_dscp_traffic_class));
		addPersistableAttribute  (new AttributeObjectIdAssociation(&m_ceeMapId,"ceeMapId", CeeMapManagedObject::getClassName (), true, policer_cee));


        addPersistableAttribute (new AttributeUI8(&m_strictPriority, FB_DEFAULT_STRICT_PRIORITY_VALUE,  "strictPriority", policer_priority_number));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClass0, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass0", policer_dwrr_traffic_class0));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClass1, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass1", policer_dwrr_traffic_class1));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClass2, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass2", policer_dwrr_traffic_class2));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClass3, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass3", policer_dwrr_traffic_class3));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClass4, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass4", policer_dwrr_traffic_class4));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClass5, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass5", policer_dwrr_traffic_class5));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClass6, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass6", policer_dwrr_traffic_class6));
        addPersistableAttribute (new AttributeUI32(&m_dwrrTrafficClassLast, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClassLast",
			  policer_dwrr_traffic_class_last));

        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass0, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass0"));
        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass1, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass1", policer_TC1));
        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass2, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass2", policer_TC2));
        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass3, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass3", policer_TC3));
        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass4, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass4", policer_TC4));
        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass5, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass5", policer_TC5));
        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass6, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass6", policer_TC6));
        addPersistableAttribute (new AttributeUI64(&m_shapingRateTrafficClass7, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass7", policer_TC7));


        addPersistableAttribute (new AttributeEnum(&m_scheduler_type, FB_DEFAULT_QUEUE_SCHEDULER_TYPE, "schedulertype", policer_scheduler_type));


    }

    void  PoClassMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI64UC(&m_cir, true, "cir"));
        addPersistableAttributeForCreate  (new AttributeUI64UC(&m_cbs, true, "cbs"));
        addPersistableAttributeForCreate  (new AttributeUI64UC(&m_eir, true, "eir"));
        addPersistableAttributeForCreate  (new AttributeUI64UC(&m_ebs, true, "ebs"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformDscp,true, "conformDscp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPrecedence, true,"conformPrecedence"));
		addPersistableAttributeForCreate  (new AttributeUI32(&m_conformType,"conformType"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformTc,true,"conformTc"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedDscp,true, "exceedDscp"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPrecedence,true,"exceedPrecedence"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_exceedType,"exceedType"));
		addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedTc,true,"exceedTc"));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_classMapId,"classMapId", ClassMapManagedObject::getClassName (), true, policer_cl_name));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_priorityMapId,"priorityMapId", PolicerPriorityMapManagedObject::getClassName (), true, policer_set_priority));

        addPersistableAttributeForCreate  (new AttributeUI8(&m_cos, FB_DEFAULT_COS_VALUE, "cos", policer_cos));
        addPersistableAttributeForCreate  (new AttributeUI8(&m_traffic_class, FB_DEFAULT_TRAFFIC_CLASS_VALUE, "trafficClass", policer_traffic_class));
        addPersistableAttributeForCreate  (new AttributeUI16(&m_dscp, FB_DEFAULT_DSCP_VALUE, "dscp", policer_dscp));
        addPersistableAttributeForCreate  (new AttributeUI64(&m_shapingRate, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRate", policer_shaping_rate));

        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_sflowMapId,"sflowMapId", SflowProfileManagedObject::getClassName (), true, policer_sflow));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cosMutationMapId,"cosMutationMapId", CosToCosMutationMapManagedObject::getClassName (), true, policer_cos_mutation));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cosTrafficClassMapId,"cosTrafficClassMapId", CosToTrafficClassMapManagedObject::getClassName (), true, policer_cos_traffic_class));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_dscpMutationMapId,"dscpMutationMapId", DscpToDscpMutationMapManagedObject::getClassName (), true, policer_dscp_mutation));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_dscpCosMapId,"dscpCosMapId", DscpToCosMapManagedObject::getClassName (), true, policer_dscp_cos));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_dscpTrafficClassMapId,"dscpTrafficClassMapId", DscpToTrafficClassMapManagedObject::getClassName (), true, policer_dscp_traffic_class));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_ceeMapId,"ceeMapId", CeeMapManagedObject::getClassName (), true, policer_cee));

        addPersistableAttributeForCreate (new AttributeUI8(&m_strictPriority, FB_DEFAULT_STRICT_PRIORITY_VALUE, "strictPriority", policer_priority_number));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClass0, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass0", policer_dwrr_traffic_class0));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClass1, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass1", policer_dwrr_traffic_class1));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClass2, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass2", policer_dwrr_traffic_class2));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClass3, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass3", policer_dwrr_traffic_class3));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClass4, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass4", policer_dwrr_traffic_class4));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClass5, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass5", policer_dwrr_traffic_class5));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClass6, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClass6", policer_dwrr_traffic_class6));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dwrrTrafficClassLast, FB_DEFAULT_QUEUE_DWRR_VALUE, "dwrrTrafficClassLast",
			  policer_dwrr_traffic_class_last));

        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass0, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass0"));
        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass1, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass1", policer_TC1));
        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass2, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass2", policer_TC2));
        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass3, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass3", policer_TC3));
        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass4, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass4", policer_TC4));
        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass5, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass5", policer_TC5));
        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass6, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass6", policer_TC6));
        addPersistableAttributeForCreate (new AttributeUI64(&m_shapingRateTrafficClass7, FB_DEFAULT_SHAPING_RATE_VALUE, "shapingRateTrafficClass7", policer_TC7));


        addPersistableAttributeForCreate  (new AttributeEnum(&m_scheduler_type, FB_DEFAULT_QUEUE_SCHEDULER_TYPE, "schedulertype",
			  policer_scheduler_type));


	   vector<string > keyName;
        keyName.push_back ("classMapId");
		setUserDefinedKeyCombination (keyName);
    }

    void  PoClassMapManagedObject::setCir(const UI64 &cir)
    {
		if(cir != POLICER_NOT_USR_CFGED_CIR_CBS)
            m_cir.setIsUserConfigured(true);
        else
            m_cir.setIsUserConfigured(false);

        m_cir.setUI64Value(cir);
    }

    UI64  PoClassMapManagedObject::getCir() const
    {
		if(m_cir.getIsUserConfigured())
        	return (m_cir.getUI64Value());
		else
			return POLICER_NOT_USR_CFGED_CIR_CBS;
    }

    void  PoClassMapManagedObject::setCbs(const UI64 &cbs)
    {
		if(cbs != POLICER_NOT_USR_CFGED_CIR_CBS)
            m_cbs.setIsUserConfigured(true);
        else
            m_cbs.setIsUserConfigured(false);

        m_cbs.setUI64Value(cbs);
    }

    UI64  PoClassMapManagedObject::getCbs() const
    {
		if(m_cbs.getIsUserConfigured())
        	return (m_cbs.getUI64Value());
		else
			return POLICER_NOT_USR_CFGED_CIR_CBS;
    }

    void  PoClassMapManagedObject::setEir(const UI64 &eir)
    {
		if(eir != POLICER_NOT_USR_CFGED_CIR_CBS)
            m_eir.setIsUserConfigured(true);
        else
            m_eir.setIsUserConfigured(false);

        m_eir.setUI64Value(eir);
    }

    UI64  PoClassMapManagedObject::getEir() const
    {
		if(m_eir.getIsUserConfigured())
        	return (m_eir.getUI64Value());
		else
			return POLICER_NOT_USR_CFGED_CIR_CBS;
    }

    void  PoClassMapManagedObject::setEbs(const UI64 &ebs)
    {
		if(ebs != POLICER_NOT_USR_CFGED_CIR_CBS)
			m_ebs.setIsUserConfigured(true);
		else
			m_ebs.setIsUserConfigured(false);
		
        m_ebs.setUI64Value(ebs);
    }

    UI64  PoClassMapManagedObject::getEbs() const
    {
		if(m_ebs.getIsUserConfigured())
        	return (m_ebs.getUI64Value());
		else
			return POLICER_NOT_USR_CFGED_CIR_CBS;
    }

    void  PoClassMapManagedObject::setConformDscp(const UI32 &conformDscp)
    {
		if(conformDscp != POLICER_NOT_USR_CFGED_VAL)
			m_conformDscp.setIsUserConfigured(true);
		else
			m_conformDscp.setIsUserConfigured(false);
		
		m_conformDscp.setUI32Value(conformDscp);
    }

    UI32  PoClassMapManagedObject::getConformDscp() const
    {
		if(m_conformDscp.getIsUserConfigured())
            return(m_conformDscp.getUI32Value());
        else
            return(POLICER_NOT_USR_CFGED_VAL);
    }

    void  PoClassMapManagedObject::setConformPrecedence(const UI32 &conformPrecedence)
    {
		if(conformPrecedence != POLICER_NOT_USR_CFGED_VAL)
            m_conformPrecedence.setIsUserConfigured(true);
        else
            m_conformPrecedence.setIsUserConfigured(false);
        
		m_conformPrecedence.setUI32Value(conformPrecedence);
    }

    UI32  PoClassMapManagedObject::getConformPrecedence() const
    {
		if(m_conformPrecedence.getIsUserConfigured())
			return(m_conformPrecedence.getUI32Value());
		else
        	return (POLICER_NOT_USR_CFGED_VAL);
    }

	void  PoClassMapManagedObject::setConformType(const UI32 &conformType)
    {
        m_conformType  =  conformType;
    }

    UI32  PoClassMapManagedObject::getConformType() const
    {
        return (m_conformType);
    }

    void  PoClassMapManagedObject::setConformTc(const UI32 &conformTc)
    {
		if(conformTc != POLICER_NOT_USR_CFGED_VAL)
            m_conformTc.setIsUserConfigured(true);
        else
            m_conformTc.setIsUserConfigured(false);
        
		m_conformTc.setUI32Value(conformTc);
    }

    UI32  PoClassMapManagedObject::getConformTc() const
    {
		if(m_conformTc.getIsUserConfigured())
			return (m_conformTc.getUI32Value());
		else
        	return (POLICER_NOT_USR_CFGED_VAL);
    }

    void  PoClassMapManagedObject::setExceedDscp(const UI32 &exceedDscp)
    {
		if(exceedDscp != POLICER_NOT_USR_CFGED_VAL)
            m_exceedDscp.setIsUserConfigured(true);
        else
            m_exceedDscp.setIsUserConfigured(false);
		
		m_exceedDscp.setUI32Value(exceedDscp);
    }

    UI32  PoClassMapManagedObject::getExceedDscp() const
    {
		if(m_exceedDscp.getIsUserConfigured())
            return(m_exceedDscp.getUI32Value());
        else
            return(POLICER_NOT_USR_CFGED_VAL);
    }

    void  PoClassMapManagedObject::setExceedPrecedence(const UI32 &exceedPrecedence)
    {
		if(exceedPrecedence != POLICER_NOT_USR_CFGED_VAL)
            m_exceedPrecedence.setIsUserConfigured(true);
        else
            m_exceedPrecedence.setIsUserConfigured(false);
        
		m_exceedPrecedence.setUI32Value(exceedPrecedence);
    }

    UI32  PoClassMapManagedObject::getExceedPrecedence() const
    {
		if(m_exceedPrecedence.getIsUserConfigured())
        	return (m_exceedPrecedence.getUI32Value());
		else
			return(POLICER_NOT_USR_CFGED_VAL);
    }

	void  PoClassMapManagedObject::setExceedType(const UI32 &exceedType)
    {
        m_exceedType  =  exceedType;
    }

    UI32  PoClassMapManagedObject::getExceedType() const
    {
        return (m_exceedType);
    }

    void  PoClassMapManagedObject::setExceedTc(const UI32 &exceedTc)
    {
		if(exceedTc != POLICER_NOT_USR_CFGED_VAL)
            m_exceedTc.setIsUserConfigured(true);
        else
            m_exceedTc.setIsUserConfigured(false);
        
		m_exceedTc.setUI32Value(exceedTc);
    }

    UI32  PoClassMapManagedObject::getExceedTc() const
    {
		if(m_exceedTc.getIsUserConfigured())
        	return (m_exceedTc.getUI32Value());
		else
			return(POLICER_NOT_USR_CFGED_VAL);
    }

    void  PoClassMapManagedObject::setClassMapId(const ObjectId &classMapId)
    {
        m_classMapId  =  classMapId;
    }

    ObjectId  PoClassMapManagedObject::getClassMapId() const
    {
        return (m_classMapId);
    }

    void  PoClassMapManagedObject::setPriorityMapId(const ObjectId &priorityMapId)
    {
        m_priorityMapId  =  priorityMapId;
    }

    ObjectId  PoClassMapManagedObject::getPriorityMapId() const
    {
        return (m_priorityMapId);
    }

	UI32 PoClassMapManagedObject::getCase (const UI32 &userChoiceWyserTag)
    {
        trace (TRACE_LEVEL_DEBUG, "PoClassMapManagedObject::getcase: Entering ...");

        if ( CONFORM_DSCP == m_conformType)
        {
                return policer_conform_set_dscp;
        }
        else if ( CONFORM_PREC == m_conformType)
        {
                return policer_conform_set_prec;
        }

        return 0;
    }


	void PoClassMapManagedObject::setCoS(const UI8 &cos) {
		m_cos = cos;
	}

	UI8 PoClassMapManagedObject::getCoS() const {
		return m_cos;
	}

	void PoClassMapManagedObject::setTrafficClass(const UI8 &trafficClass) {
		m_traffic_class = trafficClass;
	}

	UI8 PoClassMapManagedObject::getTrafficClass() const {
		return m_traffic_class;
	}

	void PoClassMapManagedObject::setDscp(const UI16 &dscp) {
		m_dscp = dscp;
	}

	UI16 PoClassMapManagedObject::getDscp() const {
		return m_dscp;
	}

	void PoClassMapManagedObject::setShapingRate(const UI64 &shapingRate) {
		m_shapingRate = shapingRate;
	}

	UI64 PoClassMapManagedObject::getShapingRate() const {
		return m_shapingRate;
	}


	ObjectId PoClassMapManagedObject::getSflowMap() const {
		return m_sflowMapId;
	}

	void PoClassMapManagedObject::setSflowMap(const ObjectId &sflowMapId) {
		m_sflowMapId = sflowMapId;
	}

	ObjectId PoClassMapManagedObject::getCoSMutationMap() const {
		return m_cosMutationMapId;
	}

	void PoClassMapManagedObject::setCoSMutationMap(
			const ObjectId &cosMutationMapId) {
		m_cosMutationMapId = cosMutationMapId;
	}

	ObjectId PoClassMapManagedObject::getCoSTrafficClassMap() const {
		return m_cosTrafficClassMapId;
	}
	void PoClassMapManagedObject::setCoSTrafficClassMap(
			const ObjectId &cosToTrafficClassMapId) {
		m_cosTrafficClassMapId = cosToTrafficClassMapId;
	}

	ObjectId PoClassMapManagedObject::getDscpCoSMap() const {
		return m_dscpCosMapId;
	}
	void PoClassMapManagedObject::setDscpCoSMap(const ObjectId &dscpCosMapId) {
		m_dscpCosMapId = dscpCosMapId;
	}

	ObjectId PoClassMapManagedObject::getDscpMutationMap() const {
		return m_dscpMutationMapId;
	}
	void PoClassMapManagedObject::setDscpMutationMap(
			const ObjectId &dscpMutationMapId) {
		m_dscpMutationMapId = dscpMutationMapId;
	}

	ObjectId PoClassMapManagedObject::getDscpTrafficClassMap() const {
		return m_dscpTrafficClassMapId;
	}
	void PoClassMapManagedObject::setDscpTrafficClassMap(
			const ObjectId &dscpTrafficClassMapId) {
		m_dscpTrafficClassMapId = dscpTrafficClassMapId;
	}

	ObjectId PoClassMapManagedObject::getCeeMap() const {
		return m_ceeMapId;
	}
	void PoClassMapManagedObject::setCeeMap(
			const ObjectId &ceeMapId) {
		m_ceeMapId = ceeMapId;
	}

	UI8 PoClassMapManagedObject::getStrictPriority() const {
		return m_strictPriority;
	}
	void PoClassMapManagedObject::setStrictPriority(const UI8 &strictPriority) {
		m_strictPriority = strictPriority;
		if ( FB_DEFAULT_STRICT_PRIORITY_VALUE == strictPriority ) {
			m_scheduler_type = FB_DEFAULT_QUEUE_SCHEDULER_TYPE;
		} else {
			m_scheduler_type = 1;
		}
	}

	void PoClassMapManagedObject::setDwrrTrafficClass0(
			const UI32 &dwrrTrafficClass0) {
		m_dwrrTrafficClass0 = dwrrTrafficClass0;
	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClass0() const {
		return m_dwrrTrafficClass0;
	}
	void PoClassMapManagedObject::setDwrrTrafficClass1(
			const UI32 &dwrrTrafficClass1) {
		m_dwrrTrafficClass1 = dwrrTrafficClass1;
	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClass1() const {
		return m_dwrrTrafficClass1;
	}
	void PoClassMapManagedObject::setDwrrTrafficClass2(
			const UI32 &dwrrTrafficClass2) {
		m_dwrrTrafficClass2 = dwrrTrafficClass2;

	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClass2() const {
		return m_dwrrTrafficClass2;
	}
	void PoClassMapManagedObject::setDwrrTrafficClass3(
			const UI32 &dwrrTrafficClass3) {
		m_dwrrTrafficClass3 = dwrrTrafficClass3;

	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClass3() const {
		return m_dwrrTrafficClass3;
	}
	void PoClassMapManagedObject::setDwrrTrafficClass4(
			const UI32 &dwrrTrafficClass4) {
		m_dwrrTrafficClass4 = dwrrTrafficClass4;

	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClass4() const {
		return m_dwrrTrafficClass4;
	}
	void PoClassMapManagedObject::setDwrrTrafficClass5(
			const UI32 &dwrrTrafficClass5) {
		m_dwrrTrafficClass5 = dwrrTrafficClass5;

	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClass5() const {
		return m_dwrrTrafficClass5;
	}
	void PoClassMapManagedObject::setDwrrTrafficClass6(
			const UI32 &dwrrTrafficClass6) {
		m_dwrrTrafficClass6 = dwrrTrafficClass6;

	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClass6() const {
		return m_dwrrTrafficClass6;
	}
	void PoClassMapManagedObject::setDwrrTrafficClassLast(
			const UI32 &dwrrTrafficClassLast) {
		m_dwrrTrafficClassLast = dwrrTrafficClassLast;

	}
	UI32 PoClassMapManagedObject::getDwrrTrafficClassLast() const {
		return m_dwrrTrafficClassLast;
	}

	void PoClassMapManagedObject::setShapingRateTrafficClass0(
			const UI64 &shapingRateTrafficClass0) {
		m_shapingRateTrafficClass0 = shapingRateTrafficClass0;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass0() const {
		return m_shapingRateTrafficClass0;
	}
	void PoClassMapManagedObject::setShapingRateTrafficClass1(
			const UI64 &shapingRateTrafficClass1) {
		m_shapingRateTrafficClass1 = shapingRateTrafficClass1;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass1() const {
		return m_shapingRateTrafficClass1;

	}
	void PoClassMapManagedObject::setShapingRateTrafficClass2(
			const UI64 &shapingRateTrafficClass2) {
		m_shapingRateTrafficClass2 = shapingRateTrafficClass2;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass2() const {
		return m_shapingRateTrafficClass2;

	}
	void PoClassMapManagedObject::setShapingRateTrafficClass3(
			const UI64 &shapingRateTrafficClass3) {
		m_shapingRateTrafficClass3 = shapingRateTrafficClass3;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass3() const {
		return m_shapingRateTrafficClass3;

	}
	void PoClassMapManagedObject::setShapingRateTrafficClass4(
			const UI64 &shapingRateTrafficClass4) {
		m_shapingRateTrafficClass4 = shapingRateTrafficClass4;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass4() const {
		return m_shapingRateTrafficClass4;

	}
	void PoClassMapManagedObject::setShapingRateTrafficClass5(
			const UI64 &shapingRateTrafficClass5) {
		m_shapingRateTrafficClass5 = shapingRateTrafficClass5;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass5() const {
		return m_shapingRateTrafficClass5;

	}
	void PoClassMapManagedObject::setShapingRateTrafficClass6(
			const UI64 &shapingRateTrafficClass6) {
		m_shapingRateTrafficClass6 = shapingRateTrafficClass6;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass6() const {
		return m_shapingRateTrafficClass6;

	}
	void PoClassMapManagedObject::setShapingRateTrafficClass7(
			const UI64 &shapingRateTrafficClassLast) {
		m_shapingRateTrafficClass7 = shapingRateTrafficClassLast;
	}
	UI64 PoClassMapManagedObject::getShapingRateTrafficClass7() const {
		return m_shapingRateTrafficClass7;
	}


}
