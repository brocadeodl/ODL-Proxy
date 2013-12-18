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
 *   Author : kverma, asharma                                              *
 **************************************************************************/

#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "APPM/Global/PfcRxTxMapManagedObject.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/APPM/APPMQosProfileMessage.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "APPM/Global/PfcRxTxMapManagedObject.h"
#include "DcmCStatus.h"
#include "DcmCore/DcmToolKit.h"
#include <raslog/raslog.h>
#include <raslog/raslog_nsm.h>

namespace DcmNs
{

    QosProfileCfgManagedObject::QosProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (QosProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager)
    {
    }

    QosProfileCfgManagedObject::QosProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const ObjectId &ceeMapId,const SI32 &defaultCosValue,const bool &qosTrust,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> > &flowControlPfcRxTxMap,const ObjectId &cosTrafficClass,const ObjectId &cosMutation)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (QosProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager),
        m_ceeMapId    (ceeMapId),
        m_defaultCosValue    (defaultCosValue),
        m_qosTrust    (qosTrust),
        m_flowControlTxEnable    (flowControlTxEnable),
        m_flowControlRxEnable    (flowControlRxEnable),
        m_flowControlPfcRxTxMap    (flowControlPfcRxTxMap),
        m_cosTrafficClass    (cosTrafficClass),
        m_cosMutation    (cosMutation)
    {
    }

    QosProfileCfgManagedObject::~QosProfileCfgManagedObject ()
    {
    }

    string  QosProfileCfgManagedObject::getClassName()
    {
        return ("QosProfileCfgManagedObject");
    }

    void  QosProfileCfgManagedObject::setupAttributesForPersistence()
    {
        SubProfileManagedObject::setupAttributesForPersistence ();

        addPersistableAttribute (new AttributeObjectIdAssociation(&m_ceeMapId,"ceeMapId", CeeMapManagedObject::getClassName (), true, appm_cee));
        addPersistableAttribute (new AttributeSI32(&m_defaultCosValue,"defaultCosValue", appm_cos));
        addPersistableAttribute (new AttributeBool(&m_qosTrust,"qosTrust", appm_trust_cos));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_flowControlTxEnable, APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL, "flowControlTxEnable", appm_tx));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_flowControlRxEnable, APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL, "flowControlRxEnable", appm_rx));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<PfcRxTxMapManagedObject>(&m_flowControlPfcRxTxMap,"flowControlPfcRxTxMap", getClassName (), getObjectId (), PfcRxTxMapManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cosTrafficClass,"cosTrafficClass", CosToTrafficClassMapManagedObject::getClassName (), true, appm_cos_traffic_class));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cosMutation,"cosMutation", CosToCosMutationMapManagedObject::getClassName (), true, appm_cos_mutation));
    }

    void  QosProfileCfgManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_ceeMapId,"ceeMapId", CeeMapManagedObject::getClassName (), true, appm_cee));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_defaultCosValue,"defaultCosValue",  appm_cos));
        addPersistableAttributeForCreate  (new AttributeBool(&m_qosTrust,"qosTrust",  appm_trust_cos));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_flowControlTxEnable,"flowControlTxEnable", appm_tx));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_flowControlRxEnable,"flowControlRxEnable", appm_rx));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<PfcRxTxMapManagedObject>(&m_flowControlPfcRxTxMap,"flowControlPfcRxTxMap", getClassName (), getObjectId (), PfcRxTxMapManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cosTrafficClass,"cosTrafficClass", CosToTrafficClassMapManagedObject::getClassName (), true, appm_cos_traffic_class));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cosMutation,"cosMutation", CosToCosMutationMapManagedObject::getClassName (), true, appm_cos_mutation));

        vector<string > keyName;
        keyName.push_back ("description");
        setUserDefinedKeyCombination (keyName);

    }

    void  QosProfileCfgManagedObject::setCeeMapId(const ObjectId &ceeMapId)
    {
        m_ceeMapId  =  ceeMapId;
    }

    ObjectId  QosProfileCfgManagedObject::getCeeMapId() const
    {
        return (m_ceeMapId);
    }

    void  QosProfileCfgManagedObject::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  QosProfileCfgManagedObject::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  QosProfileCfgManagedObject::setQosTrust(const bool &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    bool  QosProfileCfgManagedObject::getQosTrust() const
    {
        return (m_qosTrust);
    }

    void  QosProfileCfgManagedObject::setFlowControlTxEnable(const SI32 &flowControlTxEnable)
    {
        m_flowControlTxEnable  =  flowControlTxEnable;
    }

    SI32  QosProfileCfgManagedObject::getFlowControlTxEnable() const
    {
        return (m_flowControlTxEnable);
    }

    void  QosProfileCfgManagedObject::setFlowControlRxEnable(const SI32 &flowControlRxEnable)
    {
        m_flowControlRxEnable  =  flowControlRxEnable;
    }

    SI32  QosProfileCfgManagedObject::getFlowControlRxEnable() const
    {
        return (m_flowControlRxEnable);
    }

    void  QosProfileCfgManagedObject::setFlowControlPfcRxTxMap(const vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> > &flowControlPfcRxTxMap)
    {
        m_flowControlPfcRxTxMap  =  flowControlPfcRxTxMap;
    }

    vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  QosProfileCfgManagedObject::getFlowControlPfcRxTxMap() const
    {
        return (m_flowControlPfcRxTxMap);
    }

    void  QosProfileCfgManagedObject::setCosTrafficClass(const ObjectId &cosTrafficClass)
    {
        m_cosTrafficClass  =  cosTrafficClass;
    }

    ObjectId  QosProfileCfgManagedObject::getCosTrafficClass() const
    {
        return (m_cosTrafficClass);
    }

    void  QosProfileCfgManagedObject::setCosMutation(const ObjectId &cosMutation)
    {
        m_cosMutation  =  cosMutation;
    }

    ObjectId  QosProfileCfgManagedObject::getCosMutation() const
    {
        return (m_cosMutation);
    }

    void QosProfileCfgManagedObject::addPfcTxRxToVector(PfcRxTxMapManagedObject *PfcTxRxMap)
    {
        WaveManagedObjectPointer<PfcRxTxMapManagedObject> PfcTxRx (PfcTxRxMap);
        m_flowControlPfcRxTxMap.push_back(PfcTxRx);
    }

    void QosProfileCfgManagedObject::deletePfcTxRxToVector(int index)
    {
        m_flowControlPfcRxTxMap.erase(m_flowControlPfcRxTxMap.begin() + index );
    }

    void getFlowcontrolPfcTxRxValues(SI32 &pfc_cos, SI32 &flowControlTxEnable, SI32 &flowControlRxEnable)
    {
    }

    void getFlowcontrolTxRxValues(SI32 &flowControlTxEnable, SI32 &flowControlRxEnable)
    {
    }

    Attribute *QosProfileCfgManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
        Attribute *pAttribute = NULL;
        pAttribute = new AttributeBool (true, "appm");
        return (pAttribute);
    }

    void QosProfileCfgManagedObject::setDefaultFlowControlVals()
    {
        m_flowControlTxEnable = APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL;
        m_flowControlRxEnable = APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL;
    }

    bool QosProfileCfgManagedObject::verifyCeeMap(const string mapName)
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);

        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(CeeMapManagedObject::getClassName());
        bool isExist = false;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0 ;

        syncQueryCtxt.addAndAttribute (new AttributeString (mapName, "ceeMapName"));
        status = querySynchronouslyForCount ( &syncQueryCtxt, count );

        if ( ( WAVE_MESSAGE_SUCCESS == status ) && ( count >= 1 ) ) {
            {
                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("MATCH_FOUND"));
                isExist = true;
            }
        }

		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_INFO);
        return isExist;
    }

    bool QosProfileCfgManagedObject::verifyCosMutMap(const string mapName)
    {
        WaveManagedObjectSynchronousQueryContext
            SyncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
        bool isExist = false;
        UI32 count = 0 ;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        SyncQueryCtxt.addAndAttribute (new AttributeString (mapName, "cosToCosMutationMapName"));
        status = querySynchronouslyForCount (&SyncQueryCtxt, count);

        if ( (  WAVE_MESSAGE_SUCCESS == status ) && ( count >= 1 )) {
                isExist = true;
        }

        return isExist;
    }

    bool QosProfileCfgManagedObject::verifyCosTcMap(const string mapName)
    {
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
        bool isExist = false;
        UI32 count = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        syncQueryCtxt.addAndAttribute (new AttributeString (mapName, "cosToTrafficClassMapName"));
        
        status = querySynchronouslyForCount ( &syncQueryCtxt, count);

        if ( ( WAVE_MESSAGE_SUCCESS == status ) && ( count >= 1 )) {
                isExist = true;
        }

        return isExist;
    }

    void QosProfileCfgManagedObject::searchResetCosMutMap(const ObjectId cosMutation)
    {
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(QosProfileCfgManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults;

        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;

        syncQueryCtxt.addAndAttribute
            (new AttributeUI32(cosMutation.getClassId(), "objectidclassid"));
        syncQueryCtxt.addAndAttribute
            (new AttributeUI64(cosMutation.getInstanceId(), "objectidinstanceid"));

        pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            unsigned int i =0;

            for ( i = 0; i < pResults->size(); i++)
            {
                pQosProfileCfgManagedObject =
                    dynamic_cast<QosProfileCfgManagedObject *>((*pResults)[i]);
                pQosProfileCfgManagedObject->setCosMutation(ObjectId::NullObjectId);
            }

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

    void QosProfileCfgManagedObject::searchResetCosTcMap(const ObjectId cosTc)
    {
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(QosProfileCfgManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults;

        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;

        syncQueryCtxt.addAndAttribute
            (new AttributeUI32(cosTc.getClassId(), "objectidclassid"));
        syncQueryCtxt.addAndAttribute
            (new AttributeUI64(cosTc.getInstanceId(), "objectidinstanceid"));

        pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            unsigned int i =0;

            for ( i = 0; i < pResults->size(); i++)
            {
                pQosProfileCfgManagedObject =
                    dynamic_cast<QosProfileCfgManagedObject *>((*pResults)[i]);
                pQosProfileCfgManagedObject->setCosTrafficClass(ObjectId::NullObjectId);
            }

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

    void QosProfileCfgManagedObject::searchResetCeeMap(const ObjectId ceeMap)
    {

        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(QosProfileCfgManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults;

        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;

        syncQueryCtxt.addAndAttribute
            (new AttributeUI32(ceeMap.getClassId(), "objectidclassid"));
        syncQueryCtxt.addAndAttribute
            (new AttributeUI64(ceeMap.getInstanceId(), "objectidinstanceid"));

        pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            unsigned int i =0;

            for ( i = 0; i < pResults->size(); i++)
            {
                pQosProfileCfgManagedObject =
                    dynamic_cast<QosProfileCfgManagedObject *>((*pResults)[i]);
                pQosProfileCfgManagedObject->setCeeMapId(ObjectId::NullObjectId);
            }

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

    void QosProfileCfgManagedObject::applyCeeMap()
    {

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        unsigned int iter;
        vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  tempFlowControlPfcRxTxMapVector;

        /* Resetting trust. */
        this->setQosTrust(APPM_QOS_WYSEREA_DEFAULT_TRUST);

        /* Resetting Cos-Mutation Map. */
        this->setCosMutation(ObjectId::NullObjectId);

        /* Resetting Cos-Traffic Class Map. */
        this->setCosTrafficClass(ObjectId::NullObjectId);

        /* Resetting Flowcontrol Values. */
        this->setDefaultFlowControlVals();
		
		/* Resetting Qos Cos Value. */
		this->setDefaultCosValue(APPM_QOS_WYSEREA_DEFAULT_COS);

        /* Resetting Pfc Values. */
        tempFlowControlPfcRxTxMapVector = this->getFlowControlPfcRxTxMap();
        for (iter=0 ; iter < tempFlowControlPfcRxTxMapVector.size(); iter++ )
        {
            this->deletePfcTxRxToVector(iter);
        }
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_INFO);

    }

    UI32 QosProfileCfgManagedObject::activationValidateQosFcoeConflict()
    {
        vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  tempFlowControlPfcRxTxMapVector;
        vector<WaveManagedObject *> *pResults = NULL;
        FcoeProfileCfgManagedObject *pFcoeProfileCfgManagedObject = NULL;
        bool isFcoePort = false;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeProfileCfgManagedObject::getClassName());
        string profile_name = "default";
        UI32 status = WAVE_MESSAGE_SUCCESS;

        syncQueryCtxt.addAndAttribute (new AttributeString (profile_name, "description"));
        pResults = querySynchronously (&syncQueryCtxt);

        if (!pResults) {
            return status;
        }

        if (!(pResults->size()))
        {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return status;
        }

        pFcoeProfileCfgManagedObject = dynamic_cast<FcoeProfileCfgManagedObject *>((*pResults)[0]);
        prismAssert (NULL != pFcoeProfileCfgManagedObject, __FILE__, __LINE__);

        isFcoePort = (pFcoeProfileCfgManagedObject->getFcoePort());

        /*
         * Freeing up the query result for fcoeprofile.
         */
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        if (isFcoePort)
        {
            /*
             * If FcoePort is enabled then verify whether qos profile
             * conflicts with fcoe config.
             */


            /*
             * Check for Cee Map.
             */
            if (this->getCeeMapId() != (ObjectId::NullObjectId))
            {
                return WRC_APPM_ERR_PROFILE_ACTIVATION_QOS;
            }

            /*
             * Check for Cos-Mutation Map.
             */
            if (this->getCosMutation() != (ObjectId::NullObjectId))
            {
                return WRC_APPM_ERR_PROFILE_ACTIVATION_QOS;
            }

            /*
             * Check for Cos Value.
             */

            if (this->getDefaultCosValue() != APPM_QOS_WYSEREA_DEFAULT_COS)
            {
                return WRC_APPM_ERR_PROFILE_ACTIVATION_QOS;
            }

            /*
             * Check for Trust.
             */
            if (this->getQosTrust() != APPM_QOS_WYSEREA_DEFAULT_TRUST)
            {
                return WRC_APPM_ERR_PROFILE_ACTIVATION_QOS;
            }

            /*
             * Check for Cos-TrafficClass Map.
             */
            if (this->getCosTrafficClass() != (ObjectId::NullObjectId))
            {
                return WRC_APPM_ERR_PROFILE_ACTIVATION_QOS;
            }


            /*
             * Check for FlowControl values.
             */
            if ((this->getFlowControlTxEnable() != APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL)
                || (this->getFlowControlRxEnable() != APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL))
            {
                return WRC_APPM_ERR_PROFILE_ACTIVATION_QOS;
            }

            tempFlowControlPfcRxTxMapVector = this->getFlowControlPfcRxTxMap();

            if (tempFlowControlPfcRxTxMapVector.size())
            {
                return WRC_APPM_ERR_PROFILE_ACTIVATION_QOS;
            }

        }

        return status;
    }

    UI32 QosProfileCfgManagedObject::activationValidate()
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        UI32 status = WAVE_MESSAGE_SUCCESS;
        string profileName = getDescription();

        /*
         * If VCS enabled then check whether qos profile conflicts with
         * FCOE profile config.
         */

        if (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            //status = activationValidateQosFcoeConflict();
        }

        /*
         * Printing Raslog for failure. For success, raslog will be printed
         * by backend.
         */
        
        if (status != WAVE_MESSAGE_SUCCESS)
            raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, NSM_2001, profileName.c_str(), "Qos Profile Conflict");


        return status;
    }

    bool QosProfileCfgManagedObject::isDefaultConfig()
    {
        trace (TRACE_LEVEL_INFO, "QosProfile :: isDefaultConfig ENTER");
        vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  tempFlowControlPfcRxTxMapVector;
        bool isdefaultConfig = true;

        /*
         * Check for Cee Map.
         */
        if (this->getCeeMapId() != (ObjectId::NullObjectId))
        {
            isdefaultConfig = false;
            return  isdefaultConfig;
        }

        /*
         * Check for Cos-Mutation Map.
         */
        if (this->getCosMutation() != (ObjectId::NullObjectId))
        {
            isdefaultConfig = false;
            return  isdefaultConfig;
        }

        /*
         * Check for Cos Value.
         */

        if (this->getDefaultCosValue() != APPM_QOS_WYSEREA_DEFAULT_COS)
        {
            isdefaultConfig = false;
            return  isdefaultConfig;
        }

        /*
         * Check for Trust.
         */
        if (this->getQosTrust() != APPM_QOS_WYSEREA_DEFAULT_TRUST)
        {
            isdefaultConfig = false;
            return  isdefaultConfig;
        }

        /*
         * Check for Cos-TrafficClass Map.
         */
        if (this->getCosTrafficClass() != (ObjectId::NullObjectId))
        {
            isdefaultConfig = false;
            return  isdefaultConfig;
        }


        /*
         * Check for FlowControl values.
         */
        if ((this->getFlowControlTxEnable() != APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL)
            || (this->getFlowControlRxEnable() != APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL))
        {
            isdefaultConfig = false;
            return  isdefaultConfig;
        }

        tempFlowControlPfcRxTxMapVector = this->getFlowControlPfcRxTxMap();

        if (tempFlowControlPfcRxTxMapVector.size())
        {
            isdefaultConfig = false;
            return  isdefaultConfig;
        }

        trace (TRACE_LEVEL_INFO, "QosProfile :: isDefaultConfig TRUE EXIT");
        return isdefaultConfig;
    }

    UI8 QosProfileCfgManagedObject::sendBulkRequestToClient(const string profile_name, const UI32 profile_id)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        int default_cos = 0;
        int tx_enable = 0;
        int rx_enable = 0;
        unsigned int iter = 0;
        APPMQosProfileMessage *pMsg = NULL;
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;
        CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;
        WaveManagedObject   *pWaveManagedObject = NULL;
        vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  tempFlowControlPfcRxTxMapVector;

        /*
         * TODO :: send the bulked message later, not
         * implemented right now because of time crunch.
         */

        /* check for cos */
        if (m_defaultCosValue != APPM_QOS_WYSEREA_DEFAULT_COS)
        {
            pMsg = new APPMQosProfileMessage;
            prismAssert(NULL != pMsg, __FILE__, __LINE__);
            pMsg->setOpCode(APPM_SET_ELEM);
            pMsg->setCmdCode(QOS_DEFAULT_COS);
            pMsg->setQosProfileName(profile_name);
            pMsg->setQosCos(m_defaultCosValue);
            pMsg->setProfileId(profile_id);

            status = sendSynchronouslyToWaveClient("qos",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                delete pMsg;
                return 0;
            }

            delete pMsg;
        }

        /* Check for CEE map & no need to send any other data, if it exists. */
        if ( m_ceeMapId != ObjectId::NullObjectId)
        {
            pMsg = new APPMQosProfileMessage;
            prismAssert(NULL != pMsg, __FILE__, __LINE__);
            pMsg->setOpCode(APPM_SET_ELEM);
            pMsg->setCmdCode(QOS_PROFILE_CEE_MAP);
            pMsg->setQosProfileName(profile_name);
            pMsg->setProfileId(profile_id);
            pWaveManagedObject = queryManagedObject (m_ceeMapId);
            pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject *>(pWaveManagedObject);

            if (!pCeeMapManagedObject){
                delete pMsg;
                return 0;
            }
            pMsg->setQosMapName(pCeeMapManagedObject->getCeeMapName());

            delete pCeeMapManagedObject;

            status = sendSynchronouslyToWaveClient("qos",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                delete pMsg;
                return 0;
            }

            delete pMsg;
            return 1;
        }

        /* Check for trust. */
        if (m_qosTrust)
        {
            pMsg = new APPMQosProfileMessage;
            prismAssert(NULL != pMsg, __FILE__, __LINE__);
            pMsg->setOpCode(APPM_SET_ELEM);
            pMsg->setCmdCode(QOS_TRUST);
            pMsg->setQosProfileName(profile_name);
            pMsg->setProfileId(profile_id);

            status = sendSynchronouslyToWaveClient("qos",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                delete pMsg;
                return 0;
            }

            delete pMsg;
        }

        /* Check for cos-mutation map. */
        if (m_cosMutation != ObjectId::NullObjectId)
        {
            pMsg = new APPMQosProfileMessage;
            prismAssert(NULL != pMsg, __FILE__, __LINE__);
            pMsg->setOpCode(APPM_SET_ELEM);
            pMsg->setCmdCode(QOS_COS_TO_COS_MUTATION);
            pMsg->setQosProfileName(profile_name);
            pMsg->setProfileId(profile_id);
            pWaveManagedObject = queryManagedObject (m_cosMutation);
            pCosToCosMutationMapManagedObject = dynamic_cast<CosToCosMutationMapManagedObject *>(pWaveManagedObject);

            if (!pCosToCosMutationMapManagedObject){
                delete pMsg;
                return 0;
            }
            pMsg->setQosMapName(pCosToCosMutationMapManagedObject->getCosToCosMutationMapName());

            delete pCosToCosMutationMapManagedObject;

            status = sendSynchronouslyToWaveClient("qos",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                delete pMsg;
                return 0;
            }

            delete pMsg;
        }

        /*  Check for cos-traffic-class map. */
        if (m_cosTrafficClass != ObjectId::NullObjectId)
        {
            pMsg = new APPMQosProfileMessage;
            prismAssert(NULL != pMsg, __FILE__, __LINE__);
            pMsg->setOpCode(APPM_SET_ELEM);
            pMsg->setCmdCode(QOS_COS_TO_TRAFFIC_MAP);
            pMsg->setQosProfileName(profile_name);
            pMsg->setProfileId(profile_id);
            pWaveManagedObject = queryManagedObject (m_cosTrafficClass);
            pCosToTrafficClassMapManagedObject = dynamic_cast<CosToTrafficClassMapManagedObject *>(pWaveManagedObject);

            if (!pCosToTrafficClassMapManagedObject){
                delete pMsg;
                return 0;
            }
            pMsg->setQosMapName(pCosToTrafficClassMapManagedObject->getCosToTrafficClassMapName());

            delete pCosToTrafficClassMapManagedObject;

            status = sendSynchronouslyToWaveClient("qos",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                delete pMsg;
                return 0;
            }

            delete pMsg;
        }

         /* Flowcontrol All config. */
         //Send message to back end only for non default config.
         if ( (APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL != m_flowControlTxEnable ||
                APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL != m_flowControlRxEnable) ){

             pMsg = new APPMQosProfileMessage;
             prismAssert(NULL != pMsg, __FILE__, __LINE__);
             pMsg->setOpCode(APPM_SET_ELEM);
             pMsg->setCmdCode(QOS_FLOW_CTRL_ALL);
             pMsg->setQosProfileName(profile_name);
             pMsg->setProfileId(profile_id);
             pMsg->setQosTx(m_flowControlTxEnable);
             pMsg->setQosRx(m_flowControlRxEnable);
             status = sendSynchronouslyToWaveClient("qos",pMsg);

             if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                 delete pMsg;
                 return 0;
             }

             delete pMsg;
         }

         /* Pfc Flowcontrol.  */
         for (iter=0 ; iter < m_flowControlPfcRxTxMap.size(); iter++ )
         {
             tx_enable = (m_flowControlPfcRxTxMap[iter])->getTxValue();
             rx_enable = (m_flowControlPfcRxTxMap[iter])->getRxValue();
             default_cos = (m_flowControlPfcRxTxMap[iter])->getPfcValue();

             if (tx_enable ||  rx_enable)
             {
                 pMsg = new APPMQosProfileMessage;
                 prismAssert(NULL != pMsg, __FILE__, __LINE__);
                 pMsg->setOpCode(APPM_SET_ELEM);
                 pMsg->setCmdCode(QOS_FLOW_CTRL_PFC);
                 pMsg->setQosProfileName(profile_name);
                 pMsg->setProfileId(profile_id);
                 pMsg->setQosCos(default_cos);
                 pMsg->setQosTx(tx_enable);
                 pMsg->setQosRx(rx_enable);

                 status = sendSynchronouslyToWaveClient("qos",pMsg);
                 delete pMsg;
             }

         }


        return 1;
    }

}

