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

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosLocalWorker.h"
#include "Qos/Local/QosTypes.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpMarkListManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Qos/Global/RedProfileManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Global/QosQueueMulticastSchedulerManagedObject.h"
#include "Qos/Global/QosQueueSchedulerManagedObject.h"
#include "Qos/Global/QosQueueSchedulerManagedObject.h"
#include "Qos/Global/QosRcvQueueMulticastSettingManagedObject.h"
#include "Qos/Local/QosPhyIntfConfigLocalManagedObject.h"
#include "Qos/Global/NasServerIpManagedObject.h"
#include "Qos/Global/NasConfigManagedObject.h"
#include "Qos/Common/QosPluginDebug.h"
#include "ClientInterface/Ssm/QosClientCreateCosToCosMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCosToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosQueueSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosQueueMulticastSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosRcvQueueMulticastSettingMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCeeMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCeeRemapMessage.h"
#include "ClientInterface/Ssm/QosClientCreatePriorityMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreatePriorityGroupMessage.h"
#include "ClientInterface/Ssm/QosClientConfigReplayDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientConfigReplayDscpTcMapMessage.h"
#include "ClientInterface/Ssm/QosClientConfigReplayDscpCosMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateRedProfileMessage.h"
#include "ClientInterface/Qos/QosClientNasMessage.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/PfcFlowControlLocalManagedObject.h"
#include "Nsm/Local/QosCosToProfileIdManagedObject.h"
#include "ClientInterface/Ssm/QosClientConfigIntfNsmAttributesMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

namespace DcmNs
{

    QosLocalObjectManager::QosLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pQosLocalWorker = new QosLocalWorker (this);
        prismAssert (NULL != m_pQosLocalWorker, __FILE__, __LINE__);
    }

    QosLocalObjectManager::~QosLocalObjectManager ()
    {
    }

    QosLocalObjectManager  *QosLocalObjectManager::getInstance()
    {
        static QosLocalObjectManager *pQosLocalObjectManager = new QosLocalObjectManager ();

        WaveNs::prismAssert (NULL != pQosLocalObjectManager, __FILE__, __LINE__);

        return (pQosLocalObjectManager);
    }

    string  QosLocalObjectManager::getClassName()
    {
        return ("Qos");
    }

    PrismServiceId  QosLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *QosLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *QosLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void QosLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string name  = pWaveAsynchronousContextForPostBootPhase->getPassName();
        trace (TRACE_LEVEL_INFO, string("QosLocalObjectManager::postboot Entered with pass:") + name);

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE1") {
            ConfigReplayCosToCosMutationMap();
            ConfigReplayCosToTrafficClassMap();
            ConfigReplayQosQueueScheduler();
            ConfigReplayQosQueueMulticastScheduler();
            ConfigReplayQosRcvQueueMulticastSetting();
            QOS_PLUG_DBG("Started with configReplayCeeMap\n");
            ConfigReplayCeeMap();
            QOS_PLUG_DBG("Started with configReplayPriorityGroupMap\n");
            ConfigReplayPriorityGroup();
            QOS_PLUG_DBG("Started with configReplayPriorityMap\n");
            ConfigReplayPriorityMap();
            QOS_PLUG_DBG("Started with ConfigReplayCeeReemap\n");
            ConfigReplayCeeReemap();
            QOS_PLUG_DBG("Done with ConfigReplayCeeReemap\n");
            QOS_PLUG_DBG("Started with configReplayNasAutoQos\n");
            ConfigReplayAutoNasConfiguration();
            QOS_PLUG_DBG("Done with configReplayNasAutoQos\n");
			QOS_PLUG_DBG("Started with DscpMutationMap\n");
			ConfigReplayDscpMutationMap();
			QOS_PLUG_DBG("Done with DscpMutationMap\n");
			QOS_PLUG_DBG("Started with DscpTcMap\n");
			ConfigReplayDscpTcMap();
			QOS_PLUG_DBG("Done with DscpTcMap\n");
			QOS_PLUG_DBG("Started with DscpCosMap\n");
			ConfigReplayDscpCosMap();
			QOS_PLUG_DBG("Done with DscpCosMap\n");
			QOS_PLUG_DBG("Started with RedProfiles\n");
			ConfigReplayRedProfiles();
			QOS_PLUG_DBG("Done with RedProfiles\n");
        }
        else if ( name == "DCM_POSTBOOT_GLOBAL_STAGE3") {
            // QOS_PLUG_DBG("Started with configReplayPhyQosMaps\n");
            // configReplayPhyQosMaps();
            QOS_PLUG_DBG("Started with configReplayPoQosMaps\n");
            configReplayPoQosMaps();
            // QOS_PLUG_DBG("Started with configReplayPhyQosPfc\n");
            // configReplayPhyQosPfc();
            QOS_PLUG_DBG("Started with configReplayPoQosPfc\n");
            configReplayPoQosPfc();
            // QOS_PLUG_DBG("Started with configReplayPhyCee\n");
            // configReplayPhyCee();
            QOS_PLUG_DBG("Started with configReplayPoCee\n");
            configReplayPoCee();
            QOS_PLUG_DBG("Done with configReplayPoCee\n");
			QOS_PLUG_DBG("Started with configReplayPoQosCosProfile\n");
			configReplayPoQosCosProfile();
			QOS_PLUG_DBG("Done with configReplayPoQosCosProfile\n");
                configReplayPoPriorityTag();
                QOS_PLUG_DBG("Done with configReplayPoPriorityTag\n");
		}

        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

    void QosLocalObjectManager::ConfigReplayPriorityGroup()
    {
        QOS_PLUG_DBG("Entering::ConfigReplayPriorityGroup");
		PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PriorityGroupManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);
		if (NULL != pResults)
        {
			UI32 numberOfResults = pResults->size();

			for (UI32 i=0; i< numberOfResults; i++)
            {
				pPriorityGroupManagedObject = dynamic_cast<PriorityGroupManagedObject*>((*pResults)[i]);
				if (pPriorityGroupManagedObject)
                {
                    QosClientCreatePriorityGroupMessage *m = new QosClientCreatePriorityGroupMessage();

                    m->setCeeMapName(pPriorityGroupManagedObject->getCeeMapName());
                    m->setPgId(pPriorityGroupManagedObject->getPgId());
                    m->setWeight(pPriorityGroupManagedObject->getWeight());
                    m->setPfc(pPriorityGroupManagedObject->getPfc());
                    //TR342020, using this flag to ignor PFC sanity check during normal reboot
                    m->setConfigReplay(1);

                    status = sendSynchronouslyToWaveClient ("qos", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayPriorityGroup: configured cee map =") +
							pPriorityGroupManagedObject->getCeeMapName() + string ("PG id=") + pPriorityGroupManagedObject->getPgId());
                    }
                    else
                    {
                        trace(TRACE_LEVEL_ERROR, string("ConfigReplayPriorityGroup: cee map =") +
							pPriorityGroupManagedObject->getCeeMapName() + string ("PG id=") + pPriorityGroupManagedObject->getPgId());
                    }
                    delete m;
                }
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}

	PriorityGroupManagedObject* QosLocalObjectManager:: getPgMoByPgObjectId (const ObjectId &objectId)
    {
        PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PriorityGroupManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(objectId.getClassId(), "objectidclassid") );
        syncQueryCtxt.addAndAttribute(new AttributeUI64(objectId.getInstanceId(), "objectidinstanceid") );
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (pResults && pResults->size()  == 1) {
            pPriorityGroupManagedObject =  dynamic_cast<PriorityGroupManagedObject*>((*pResults)[0]);
        }
        if (pResults) {
                pResults->clear();
                delete pResults;
        }
        return pPriorityGroupManagedObject;
    }


    void QosLocalObjectManager::ConfigReplayPriorityMap()
    {
        QOS_PLUG_DBG("Entering::ConfigReplayPriorityMap");
		PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;
		PriorityMapManagedObject *pPriorityMapManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PriorityMapManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

		if (NULL != pResults)
        {
			UI32 numberOfResults = pResults->size();

			for (UI32 i=0; i< numberOfResults; i++)
            {
				pPriorityMapManagedObject = dynamic_cast<PriorityMapManagedObject*>((*pResults)[i]);
				if (pPriorityMapManagedObject)
                {
                    QosClientCreatePriorityMapMessage *m = new QosClientCreatePriorityMapMessage();
                    //TR342020, using this flag to ignor PFC sanity check during normal reboot
                    m->setConfigReplay(1);

                    m->setCeeMapName(pPriorityMapManagedObject->getCeeMapName());
					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos0ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos0ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}
					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos1ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos1ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}

					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos2ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos2ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}

					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos3ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos3ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}

					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos4ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos4ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}

					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos5ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos5ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}

					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos6ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos6ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}

					pPriorityGroupManagedObject = getPgMoByPgObjectId(pPriorityMapManagedObject->getCos7ToPriorityGrouMap());
					if (pPriorityGroupManagedObject)
					{
                    	m->setCos7ToPriorityGrouMap(pPriorityGroupManagedObject->getPgId());
                        delete pPriorityGroupManagedObject;
					}

                    status = sendSynchronouslyToWaveClient ("qos", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayPriorityGroup: configured priority map for cee map =") +
							pPriorityMapManagedObject->getCeeMapName());
                    }
                    else
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayPriorityGroup: failed priority map for cee map =") +
							pPriorityMapManagedObject->getCeeMapName());
                    }
                    delete m;
                }
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}


    void QosLocalObjectManager::configReplayPhyQosMaps()
    {
        QOS_PLUG_DBG("Entering::configReplayPhyQosMaps");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
		if(pResults != NULL)
		{
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
            	{
	                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);
					if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
					{
						continue;
					}
    	            string ifName;
	
					if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        	        {
            	        NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
                	}
					else
					{
						ifName = pPhyIntfLocalManagedObject->getIfName();
					}
	                ifName.insert(0, "te");
        	        QOS_PLUG_DBG(string("Configuring the interface") + ifName);
	
					if(pPhyIntfLocalManagedObject->getQosCosMap() != ObjectId::NullObjectId)
					{   
						ObjectId cosMapObjectId;
						cosMapObjectId = pPhyIntfLocalManagedObject->getQosCosMap();		
						CosToCosMutationMapManagedObject *pCosObject;
						pCosObject = getCosToCosMutationMapMOByObjectId(cosMapObjectId);	
						if(pCosObject != NULL)
						{
            	    		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
		        	        m->setIfName(ifName);
        		    	    m->setOpCode(CONFIGURE_INTERFACE_QOS);
							m->setCosToCosMutationMapName(pCosObject->getCosToCosMutationMapName());
	                   		m->setCmdCode(QOS_INTERFACE_COS_MUTATION);
							QOS_PLUG_DBG("Sending CosToCosMutationMap");
        	            	sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pCosObject;
						}
					}
					if(pPhyIntfLocalManagedObject->getQosTrafficClassMap() != ObjectId::NullObjectId)
					{
		 				ObjectId cosMapObjectId;
						cosMapObjectId = pPhyIntfLocalManagedObject->getQosTrafficClassMap();
						CosToTrafficClassMapManagedObject *pCosObject;
						pCosObject = getCosToTrafficClassMapMOByObjectId(cosMapObjectId);
					    if(pCosObject != NULL)
						{	
            	    		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
							m->setIfName(ifName);
		            	    m->setOpCode(CONFIGURE_INTERFACE_QOS);
                    		m->setCosToTrafficClassMapName(pCosObject->getCosToTrafficClassMapName());
	                    	m->setCmdCode(QOS_INTERFACE_COS_TRAFFIC_CLASS);
							QOS_PLUG_DBG("Sending CosToCosTrafficClassMap");
        	            	sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pCosObject;
						}
					}
					if(pPhyIntfLocalManagedObject->getQosTrust())
					{
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	        	        m->setIfName(ifName);
    	        	    m->setOpCode(CONFIGURE_INTERFACE_QOS);
        	        	m->setCmdCode(QOS_INTERFACE_TRUST_COS);
	            	    m->setQosTrust(pPhyIntfLocalManagedObject->getQosTrust());
						QOS_PLUG_DBG("Sending TrustCos");
	    	            sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
                    //"default cos" always need to be configured.
					{
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	    	            m->setIfName(ifName);
    	    	        m->setOpCode(CONFIGURE_INTERFACE_QOS);
        	    	    m->setCmdCode(QOS_INTERFACE_DEFAULT_COS);
            	    	m->setDefaultCosValue(pPhyIntfLocalManagedObject->getDefaultCos());
						QOS_PLUG_DBG("Sending DefaultCos");
		                sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
					if((pPhyIntfLocalManagedObject->getFlowControlTx() == 1) || (pPhyIntfLocalManagedObject->getFlowControlRx() == 1))
					{
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
		                m->setIfName(ifName);
    		            m->setOpCode(CONFIGURE_INTERFACE_QOS);
        		        m->setCmdCode(QOS_INTERFACE_FLOW_CONTROL_BOTH);
            		   	m->setFlowControlTx(pPhyIntfLocalManagedObject->getFlowControlTx());
               			m->setFlowControlRx(pPhyIntfLocalManagedObject->getFlowControlRx());
						QOS_PLUG_DBG("Sending FlowControl");
	    	           	sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
				}
	            if(pResults)
    	        {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	            }
    	    }
			else
			{
				QOS_PLUG_DBG("pResults size is zero");
			}
		}
		else
		{
			QOS_PLUG_DBG("pResults is NULL");	
		}
        return ;
    }

	void QosLocalObjectManager::configReplayPoQosCosProfile()
    {
        QOS_PLUG_DBG("Entering::configReplayPoQosCosProfile");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PoIntfManagedObject* pPoIntfManagedObject = NULL;
		if (pResults != NULL)
		{
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
	            {
    	            pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[i]);
					stringstream poName;

					poName << pPoIntfManagedObject->getId();
					string ifName = poName.str();
					ifName.insert(0, "po");
					QOS_PLUG_DBG(string("Configuring the Portchannel") + ifName);

					vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> > cosProfileMOVector
								= pPoIntfManagedObject->getCosProfile();
					unsigned int size = cosProfileMOVector.size();
					unsigned int j;
					for (j = 0; j < size; j++)
					{
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
						m->setIfName(ifName);
						m->setOpCode(CONFIGURE_INTERFACE_QOS);
						m->setCmdCode(QOS_INTERFACE_COS_PROFILE);
						m->setCos(cosProfileMOVector[j]->getCos());
						m->setRedProfile(cosProfileMOVector[j]->getRedProfile());
						//m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
						QOS_PLUG_DBG("Sending cos profile");
						sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
				}
				if(pResults)
				{
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				}
			}
		}
		return ;
    }

    void QosLocalObjectManager::configReplayPoQosMaps()
    {
        QOS_PLUG_DBG("Entering configReplayPoQosMaps");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PoIntfManagedObject* pPoIntfManagedObject = NULL;
		if (pResults != NULL)
		{
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
	            {
    	            pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[i]);
					stringstream poName;
	
					poName << pPoIntfManagedObject->getId();
					string ifName = poName.str();
            	    ifName.insert(0, "po");
        	        QOS_PLUG_DBG(string("Configuring the Portchannel") + ifName);

					if(pPoIntfManagedObject->getQosCosMap() != ObjectId::NullObjectId)
					{               
						ObjectId cosMapObjectId;
						cosMapObjectId = pPoIntfManagedObject->getQosCosMap();		
						CosToCosMutationMapManagedObject *pCosObject;
						pCosObject = getCosToCosMutationMapMOByObjectId(cosMapObjectId);	
						if(pCosObject != NULL)
						{	
        	        		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	    		            m->setIfName(ifName);
    	    		        m->setOpCode(CONFIGURE_INTERFACE_QOS);
							m->setCosToCosMutationMapName(pCosObject->getCosToCosMutationMapName());
                   			m->setCmdCode(QOS_INTERFACE_COS_MUTATION);
							QOS_PLUG_DBG("Sending CosToCosMutationMap");
    	                	sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pCosObject;
						}
					}
					if(pPoIntfManagedObject->getQosTrafficClassMap() != ObjectId::NullObjectId)
					{               
			 			ObjectId cosMapObjectId;
						cosMapObjectId = pPoIntfManagedObject->getQosTrafficClassMap();
						CosToTrafficClassMapManagedObject *pCosObject;
						pCosObject = getCosToTrafficClassMapMOByObjectId(cosMapObjectId);
					    if(pCosObject != NULL)
						{	
        	        		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	    		            m->setIfName(ifName);
    	    		        m->setOpCode(CONFIGURE_INTERFACE_QOS);
                    		m->setCosToTrafficClassMapName(pCosObject->getCosToTrafficClassMapName());
                    		m->setCmdCode(QOS_INTERFACE_COS_TRAFFIC_CLASS);
							QOS_PLUG_DBG("Sending CosToCosTrafficClassMap");
    	                	sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pCosObject;
						}
					}
					if(pPoIntfManagedObject->getQosDscpMap() != ObjectId::NullObjectId)
					{
						ObjectId dscpMapObjectId;
						dscpMapObjectId = pPoIntfManagedObject->getQosDscpMap();
						DscpToDscpMutationMapManagedObject *pDscpObject;
						pDscpObject = getDscpToDscpMutationMapMOByObjectId(dscpMapObjectId);
						if(pDscpObject != NULL)
						{
							QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
							m->setIfName(ifName);
							m->setOpCode(CONFIGURE_INTERFACE_QOS);
							m->setDscpToDscpMutationMapName(pDscpObject->getDscpToDscpMutationMapName());
							m->setCmdCode(QOS_INTERFACE_DSCP_MUTATION);
							QOS_PLUG_DBG("Sending DscpToDscpMutationMap");
							sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pDscpObject;
						}
					}
					if(pPoIntfManagedObject->getQosDscpTrafficClassMap() != ObjectId::NullObjectId)
					{
						ObjectId dscpMapObjectId;
						dscpMapObjectId = pPoIntfManagedObject->getQosDscpTrafficClassMap();
						DscpToTrafficClassMapManagedObject *pDscpObject;
						pDscpObject = getDscpToTrafficClassMapMOByObjectId(dscpMapObjectId);
						if(pDscpObject != NULL)
						{
							QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
							m->setIfName(ifName);
							m->setOpCode(CONFIGURE_INTERFACE_QOS);
							m->setDscpToTrafficClassMapName(pDscpObject->getDscpToTrafficClassMapName());
							m->setCmdCode(QOS_INTERFACE_DSCP_TRAFFIC_CLASS);
							QOS_PLUG_DBG("Sending DscpToTrafficClassMap");
							sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pDscpObject;
						}
					}
					if(pPoIntfManagedObject->getQosDscpCosMap() != ObjectId::NullObjectId)
					{
						ObjectId cosMapObjectId;
						cosMapObjectId = pPoIntfManagedObject->getQosDscpCosMap();
						DscpToCosMapManagedObject *pDscpCosObject;
						pDscpCosObject = getDscpToCosMapMOByObjectId(cosMapObjectId);
						if(pDscpCosObject != NULL)
						{
							QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
							m->setIfName(ifName);
							m->setOpCode(CONFIGURE_INTERFACE_QOS);
							m->setDscpToCosMapName(pDscpCosObject->getDscpToCosMapName());
							m->setCmdCode(QOS_INTERFACE_DSCP_COS);
							QOS_PLUG_DBG("Sending DscpToCosTrafficClassMap");
							sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pDscpCosObject;
						}
					}
					if(pPoIntfManagedObject->getQosDscpTrust())
					{
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
						m->setIfName(ifName);
						m->setOpCode(CONFIGURE_INTERFACE_QOS);
						m->setCmdCode(QOS_INTERFACE_TRUST_DSCP);
						m->setQosDscpTrust(pPoIntfManagedObject->getQosDscpTrust());
						QOS_PLUG_DBG("Sending TrustDscp");
						sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
					if(pPoIntfManagedObject->getQosTrust())
					{	
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
    		            m->setIfName(ifName);
   	    		        m->setOpCode(CONFIGURE_INTERFACE_QOS);
	            	    m->setCmdCode(QOS_INTERFACE_TRUST_COS);
    	            	m->setQosTrust(pPoIntfManagedObject->getQosTrust());
						QOS_PLUG_DBG("Sending TrustCos");
    	        	    sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
					{
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	    	            m->setIfName(ifName);
   		    	        m->setOpCode(CONFIGURE_INTERFACE_QOS);
	    	            m->setCmdCode(QOS_INTERFACE_DEFAULT_COS);
    	    	        m->setDefaultCosValue(pPoIntfManagedObject->getDefaultCos());
						QOS_PLUG_DBG("Sending DefaultCos");
            	    	sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
					if((pPoIntfManagedObject->getFlowControlTx()  == 1) || (pPoIntfManagedObject->getFlowControlRx()  == 1))
					{
						QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
    	        	    m->setIfName(ifName);
   	    	        	m->setOpCode(CONFIGURE_INTERFACE_QOS);
		                m->setCmdCode(QOS_INTERFACE_FLOW_CONTROL_BOTH);
    		           	m->setFlowControlTx(pPoIntfManagedObject->getFlowControlTx());
        		       	m->setFlowControlRx(pPoIntfManagedObject->getFlowControlRx());
						QOS_PLUG_DBG("Sending FlowControl");
               			sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
        	    }
                if(pResults)
                {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                }
            }
			else
			{
				QOS_PLUG_DBG("pResults size is zero");
			}
		}
		else
		{
			QOS_PLUG_DBG("pResults is NULL");	
		}
        return ;
    }

    void QosLocalObjectManager::configReplayPhyQosPfc()
    {
        QOS_PLUG_DBG("Entering::configReplayPhyQosPfc");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
		if(pResults != NULL)
		{
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
            	{
	                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);
					if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
					{
						continue;
					}
    	            string ifName;
	
					if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        	        {
            	        NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
                	}
					else
					{
						ifName = pPhyIntfLocalManagedObject->getIfName();
					}
	                ifName.insert(0, "te");
        	        QOS_PLUG_DBG(string("Configuring the interface") + ifName);
	
					vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> > pfcMOVector =  pPhyIntfLocalManagedObject->getPfcFlowControl();
					unsigned int size = pfcMOVector.size();
					unsigned int j; 
					for (j = 0; j < size; j++)
					{
		                QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	    	            m->setIfName(ifName);
    	    	        m->setOpCode(CONFIGURE_INTERFACE_QOS);
        	    	    m->setCmdCode(QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH);
						m->setPfcFlowControlCos(pfcMOVector[j]->getCos());
	               		m->setPfcFlowControlTx(pfcMOVector[j]->getTx());
    	           		m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
						QOS_PLUG_DBG("Sending PfcFlowControl");
            	    	sendSynchronouslyToWaveClient ("qos",m);
						delete m;
					}
        	    }
            	if(pResults)
	            {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            	}
	        }
			else
			{
				QOS_PLUG_DBG("pResults size is zero");
			}
		}
		else
		{
			QOS_PLUG_DBG("pResults is NULL");	
		}
        return ;
    }

    void QosLocalObjectManager::configReplayPoQosPfc()
    {
        QOS_PLUG_DBG("Entering::configReplayPoQosPfc");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PoIntfManagedObject* pPoIntfManagedObject = NULL;
		if(pResults != NULL)
		{
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
            	{
	                pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[i]);
					stringstream poName;
	
					poName << pPoIntfManagedObject->getId();
					string ifName = poName.str();
            	    ifName.insert(0, "po");
        	        QOS_PLUG_DBG(string("Configuring the Portchannel") + ifName);

					vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> > pfcMOVector =  pPoIntfManagedObject->getPfcFlowControl();
					unsigned int size = pfcMOVector.size();
					unsigned int j; 
					for (j = 0; j < size; j++)
					{
	                	QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	    	            m->setIfName(ifName);
    	    	        m->setOpCode(CONFIGURE_INTERFACE_QOS);
        	    	    m->setCmdCode(QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH);
						m->setPfcFlowControlCos(pfcMOVector[j]->getCos());
              			m->setPfcFlowControlTx(pfcMOVector[j]->getTx());
               			m->setPfcFlowControlRx(pfcMOVector[j]->getRx());
						QOS_PLUG_DBG("Sending FlowControl");
   		            	sendSynchronouslyToWaveClient ("qos",m);
   		            	if(NULL != m) {
   		            		delete m;
   		            	}
					}
            	}
	            if(pResults)
    	        {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	            }
			}
			else
			{
				QOS_PLUG_DBG("pResults size is zero");
			}
		}
		else
		{
			QOS_PLUG_DBG("pResults is NULL");	
		}
        return ;
    }

    void QosLocalObjectManager::configReplayPhyCee()
    {
        QOS_PLUG_DBG("Entering::configReplayPhyCee");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
		if(pResults != NULL)
		{
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
            	{
	                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);
					if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
					{
						continue;
					}
    	            string ifName;
	
					if (NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getIfName()))
        	        {
            	        NsmUtils::getTwoTupleIfName(pPhyIntfLocalManagedObject->getIfName(), ifName);
                	}
					else
					{
						ifName = pPhyIntfLocalManagedObject->getIfName();
					}
	                ifName.insert(0, "te");
        	        QOS_PLUG_DBG(string("Configuring the interface") + ifName);
					
					if(pPhyIntfLocalManagedObject->getCeeMap() != ObjectId::NullObjectId)
					{   
						ObjectId ceeMapObjectId;
						ceeMapObjectId = pPhyIntfLocalManagedObject->getCeeMap();		
						CeeMapManagedObject *pCeeObject;
						pCeeObject = getCeeMapMOByObjectId(ceeMapObjectId);	
						if(pCeeObject != NULL)
						{
            	    		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
		        	        m->setIfName(ifName);
        		    	    m->setOpCode(CONFIGURE_INTERFACE_QOS);
							m->setCeeMapName(pCeeObject->getCeeMapName());
	                   		m->setCmdCode(QOS_INTERFACE_CEE_MAP);
							QOS_PLUG_DBG("Sending CeeMap");
        	            	sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pCeeObject;
						}
					}
                }
                if(pResults)
                {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                }
	        }
			else
			{
				QOS_PLUG_DBG("pResults size is zero");
			}
		}
		else
		{
			QOS_PLUG_DBG("pResults is NULL");	
		}
        return ;
    }

    void QosLocalObjectManager::configReplayPoCee()
    {
        QOS_PLUG_DBG("Entering::configReplayPoCee");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PoIntfManagedObject* pPoIntfManagedObject = NULL;
		if(pResults != NULL)
		{
	        if (pResults->size() > 0)
    	    {
        	    for(unsigned int i = 0;i <  pResults->size(); i++)
            	{
	                pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[i]);
					stringstream poName;
	
					poName << pPoIntfManagedObject->getId();
					string ifName = poName.str();
            	    ifName.insert(0, "po");
        	        QOS_PLUG_DBG(string("Configuring the Portchannel") + ifName);

					if(pPoIntfManagedObject->getCeeMap() != ObjectId::NullObjectId)
					{   
						ObjectId ceeMapObjectId;
						ceeMapObjectId = pPoIntfManagedObject->getCeeMap();		
						CeeMapManagedObject *pCeeObject;
						pCeeObject = getCeeMapMOByObjectId(ceeMapObjectId);	
						if(pCeeObject != NULL)
						{
            	    		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
		        	        m->setIfName(ifName);
        		    	    m->setOpCode(CONFIGURE_INTERFACE_QOS);
							m->setCeeMapName(pCeeObject->getCeeMapName());
	                   		m->setCmdCode(QOS_INTERFACE_CEE_MAP);
							QOS_PLUG_DBG("Sending CeeMap");
        	            	sendSynchronouslyToWaveClient ("qos",m);
							delete m;
                            delete pCeeObject;
							/*WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCeeObject);
							 * Mem cleanup to be done*/
						}
					}
        	    }
	        }
			else
			{
				QOS_PLUG_DBG("pResults size is zero");
			}
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		else
		{
			QOS_PLUG_DBG("pResults is NULL");	
		}
        return ;
    }

    void QosLocalObjectManager::configReplayPoPriorityTag()
    {
        QOS_PLUG_DBG("Entering::configReplayPoPriorityTag");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject * > *pResults = querySynchronously(&syncQueryCtxt);

        PoIntfManagedObject* pPoIntfManagedObject = NULL;
        if(pResults != NULL)
        {
            if (pResults->size() > 0)
            {
                for(unsigned int i = 0;i <  pResults->size(); i++)
                {
                    pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[i]);
                    stringstream poName;

                    poName << pPoIntfManagedObject->getId();
                    string ifName = poName.str();
                    ifName.insert(0, "po");
                    QOS_PLUG_DBG(string("Configuring the Portchannel") + ifName);

                    if(pPoIntfManagedObject->getPriorityTag() != false)
                    {
                        QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
                        m->setIfName(ifName);
                        m->setOpCode(CONFIGURE_INTERFACE_QOS);
                        m->setCmdCode(QOS_INTERFACE_PRIORITY_TAG);
                        QOS_PLUG_DBG("Sending Priority Tag");
                        sendSynchronouslyToWaveClient ("qos",m);
                        delete m;
                    }
                }
            }
            else
            {
                QOS_PLUG_DBG("pResults size is zero");
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        else
        {
            QOS_PLUG_DBG("pResults is NULL");
        }
        return ;        
    }
 
    void QosLocalObjectManager::ConfigReplayCosToCosMutationMap()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering::ConfigReplayCosToCosMutationMap");
        CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();

	    for (UI32 i=0; i< numberOfResults; i++)
            {
                pCosToCosMutationMapManagedObject = dynamic_cast<CosToCosMutationMapManagedObject *>((*pResults)[i]);
                if (pCosToCosMutationMapManagedObject)
                {
                    QosClientCreateCosToCosMutationMapMessage *m = new QosClientCreateCosToCosMutationMapMessage();

                    m->setCosToCosMutationMapName(pCosToCosMutationMapManagedObject->getCosToCosMutationMapName());
                    m->setCos0MutatedCosValue(pCosToCosMutationMapManagedObject->getCos0MutatedCosValue());
                    m->setCos1MutatedCosValue(pCosToCosMutationMapManagedObject->getCos1MutatedCosValue());
                    m->setCos2MutatedCosValue(pCosToCosMutationMapManagedObject->getCos2MutatedCosValue());
                    m->setCos3MutatedCosValue(pCosToCosMutationMapManagedObject->getCos3MutatedCosValue());
                    m->setCos4MutatedCosValue(pCosToCosMutationMapManagedObject->getCos4MutatedCosValue());
                    m->setCos5MutatedCosValue(pCosToCosMutationMapManagedObject->getCos5MutatedCosValue());
                    m->setCos6MutatedCosValue(pCosToCosMutationMapManagedObject->getCos6MutatedCosValue());
                    m->setCos7MutatedCosValue(pCosToCosMutationMapManagedObject->getCos7MutatedCosValue());

                    status = sendSynchronouslyToWaveClient ("qos", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayCosToCosMutationMap: configured ") + pCosToCosMutationMapManagedObject->getCosToCosMutationMapName());
                    }
                    else
                    {
                        trace(TRACE_LEVEL_ERROR, string("ConfigReplayCosToCosMutationMap: Error") + pCosToCosMutationMapManagedObject->getCosToCosMutationMapName());
                    }
                    delete m;
                }
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

	void QosLocalObjectManager::ConfigReplayDscpMutationMap()
	{
        QOS_PLUG_DBG("Entering::ConfigReplayDscpMutationMap");
		DscpToDscpMutationMapManagedObject *pDscpToDscpMutationMapManagedObject = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToDscpMutationMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

		if (NULL != pResults) {
			QOS_PLUG_DBG("Results found for DSCP Mutation Map");
			UI32 numberOfResults = pResults->size();
			for (UI32 i=0; i<numberOfResults; i++) {
				pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject *>((*pResults)[i]);
				if (pDscpToDscpMutationMapManagedObject) {
					uint32_t j;
					vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > markListVector;
					
					QOS_PLUG_DBG("Creating Client message");
					QosClientConfigReplayDscpMutationMapMessage *m = new QosClientConfigReplayDscpMutationMapMessage();
					m->initialize();
					QOS_PLUG_DBG("Created and initialized Client message");
					m->setDscpMutationMapName(pDscpToDscpMutationMapManagedObject->getDscpToDscpMutationMapName());
					QOS_PLUG_DBG("Set map name..");
					markListVector = pDscpToDscpMutationMapManagedObject->getDscpMarkList();
					for (j=0; j<markListVector.size(); j++) {
						m->setDscpMarkListValues(markListVector[j]->getMarkList(), markListVector[j]->getToValue());
					}
					QOS_PLUG_DBG("Set mark lists...");

					status = sendSynchronouslyToWaveClient("qos", m);
					if (status == WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_DEBUG, string("ConfigReplayDscpMutationMap: configured ") + 
								pDscpToDscpMutationMapManagedObject->getDscpToDscpMutationMapName());
					} else {
						trace(TRACE_LEVEL_ERROR, string("ConfigReplayDscpMutationMap: Error configuring ") + 
								pDscpToDscpMutationMapManagedObject->getDscpToDscpMutationMapName());
					}
					delete m;
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	
	void QosLocalObjectManager::ConfigReplayDscpTcMap()
	{
        trace (TRACE_LEVEL_DEBUG, "Entering::ConfigReplayDscpTcMap");
		DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToTrafficClassMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size();
			for (UI32 i=0; i<numberOfResults; i++) {
				pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[i]);
				if (pDscpToTrafficClassMapManagedObject) {
					uint32_t j;
					vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > markListVector;
					
					QosClientConfigReplayDscpTcMapMessage *m = new QosClientConfigReplayDscpTcMapMessage();
					m->initialize();
					m->setDscpTcMapName(pDscpToTrafficClassMapManagedObject->getDscpToTrafficClassMapName());
					markListVector = pDscpToTrafficClassMapManagedObject->getDscpMarkList();
					for (j=0; j<markListVector.size(); j++) {
						m->setDscpMarkListValues(markListVector[j]->getMarkList(), markListVector[j]->getToValue());
					}

					status = sendSynchronouslyToWaveClient("qos", m);
					if (status == WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_DEBUG, string("ConfigReplayDscpTcMap: configured ") + 
								pDscpToTrafficClassMapManagedObject->getDscpToTrafficClassMapName());
					} else {
						trace(TRACE_LEVEL_ERROR, string("ConfigReplayDscpTcMap: Error configuring ") + 
								pDscpToTrafficClassMapManagedObject->getDscpToTrafficClassMapName());
					}
					delete m;
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	
	void QosLocalObjectManager::ConfigReplayDscpCosMap()
	{
        trace (TRACE_LEVEL_DEBUG, "Entering::ConfigReplayDscpCosMap");
		DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToCosMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size();
			for (UI32 i=0; i<numberOfResults; i++) {
				pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject *>((*pResults)[i]);
				if (pDscpToCosMapManagedObject) {
					uint32_t j;
					vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > markListVector;
					
					QosClientConfigReplayDscpCosMapMessage *m = new QosClientConfigReplayDscpCosMapMessage();
					m->initialize();
					m->setDscpCosMapName(pDscpToCosMapManagedObject->getDscpToCosMapName());
					markListVector = pDscpToCosMapManagedObject->getDscpMarkList();
					for (j=0; j<markListVector.size(); j++) {
						m->setDscpMarkListValues(markListVector[j]->getMarkList(), markListVector[j]->getToValue());
					}

					status = sendSynchronouslyToWaveClient("qos", m);
					if (status == WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_DEBUG, string("ConfigReplayDscpCosMap: configured ") + 
								pDscpToCosMapManagedObject->getDscpToCosMapName());
					} else {
						trace(TRACE_LEVEL_ERROR, string("ConfigReplayDscpCosMap: Error configuring ") + 
								pDscpToCosMapManagedObject->getDscpToCosMapName());
					}
					delete m;
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}

	void QosLocalObjectManager::ConfigReplayRedProfiles()
	{
        trace (TRACE_LEVEL_DEBUG, "Entering::ConfigReplayDscpCosMap");
		RedProfileManagedObject *pRedProfileManagedObject = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RedProfileManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size();
			for (UI32 i=0; i<numberOfResults; i++) {
				pRedProfileManagedObject = dynamic_cast<RedProfileManagedObject *>((*pResults)[i]);
				if (pRedProfileManagedObject) {
					QosClientCreateRedProfileMessage *m = new QosClientCreateRedProfileMessage();
					m->setProfileId(pRedProfileManagedObject->getProfileId());
					m->setMinThreshold(pRedProfileManagedObject->getMinThreshold());
					m->setMaxThreshold(pRedProfileManagedObject->getMaxThreshold());
					m->setDropProbability(pRedProfileManagedObject->getDropProbability());

					status = sendSynchronouslyToWaveClient("qos", m);
					if (status == WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_DEBUG, string("ConfigReplayRedProfiles: configured profile: ") + 
								pRedProfileManagedObject->getProfileId());
					} else {
						trace(TRACE_LEVEL_ERROR, string("ConfigReplayRedProfiles: Error configuring profile: ") + 
								pRedProfileManagedObject->getProfileId());
					}
					delete m;
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	
    void QosLocalObjectManager::ConfigReplayCosToTrafficClassMap()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering::ConfigReplayCosToTrafficClassMap");
        CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();

	    for (UI32 i = 0; i < numberOfResults; i++)
            {
                pCosToTrafficClassMapManagedObject = dynamic_cast < CosToTrafficClassMapManagedObject *>((*pResults)[i]);
                if (pCosToTrafficClassMapManagedObject)
                {
                    QosClientCreateCosToTrafficClassMapMessage *m = new QosClientCreateCosToTrafficClassMapMessage();

                    m->setCosToTrafficClassMapName(pCosToTrafficClassMapManagedObject->getCosToTrafficClassMapName());
                    m->setCos0MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos0MappedTrafficClassValue());
                    m->setCos1MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos1MappedTrafficClassValue());
                    m->setCos2MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos2MappedTrafficClassValue());
                    m->setCos3MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos3MappedTrafficClassValue());
                    m->setCos4MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos4MappedTrafficClassValue());
                    m->setCos5MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos5MappedTrafficClassValue());
                    m->setCos6MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos6MappedTrafficClassValue());
                    m->setCos7MappedTrafficClassValue(pCosToTrafficClassMapManagedObject->getCos7MappedTrafficClassValue());

                    status = sendSynchronouslyToWaveClient ("qos", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayCosToCosMutationMap: configured ") + pCosToTrafficClassMapManagedObject->getCosToTrafficClassMapName());
                    }
                    else
                    {
                        trace(TRACE_LEVEL_ERROR, string("ConfigReplayCosToCosMutationMap: Error") + pCosToTrafficClassMapManagedObject->getCosToTrafficClassMapName());
                    }
                    delete m;
                }
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

	void QosLocalObjectManager::ConfigReplayCeeMap()
	{
        QOS_PLUG_DBG("Entering::configReplayCeeMap");
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();

		    for (UI32 i=0; i< numberOfResults; i++)
            {
                pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject *>((*pResults)[i]);
                if (pCeeMapManagedObject)
                {
                    QosClientCreateCeeMapMessage *m = new QosClientCreateCeeMapMessage();
					m->setCeeMapName(pCeeMapManagedObject->getCeeMapName());
					m->setPrecedence(pCeeMapManagedObject->getPrecedence());
					//TR342020, using this flag to ignor PFC sanity check during normal reboot
					m->setConfigReplay(1);
					
					QOS_PLUG_DBG("sending CeeMap");
                    status = sendSynchronouslyToWaveClient ("qos", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
						QOS_PLUG_DBG("sending CeeMap-Success");
                    }
                    else
                    {
						QOS_PLUG_DBG("sending CeeMap-Failed");
                    }
                    delete m;
                }
            }
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

	}

	void QosLocalObjectManager::ConfigReplayCeeReemap()
	{
        QOS_PLUG_DBG("Entering::configReplayCeeReeMap");
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();

		    for (UI32 i=0; i< numberOfResults; i++)
            {
                pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject *>((*pResults)[i]);
                if (pCeeMapManagedObject)
                {
					{
	                    QosClientCreateCeeRemapMessage *m = new QosClientCreateCeeRemapMessage();

						m->setCeeMapName(pCeeMapManagedObject->getCeeMapName());
						m->setRemapPriority(pCeeMapManagedObject->getRemapFabricPriority());
						m->setRemapFabricP(1);
    	                //TR342020, using this flag to ignor PFC sanity check during normal reboot
        	            m->setConfigReplay(1);

						QOS_PLUG_DBG("sending CeeRemap-FabricPriority");
                	    status = sendSynchronouslyToWaveClient ("qos", m);
                    	if (status == WAVE_MESSAGE_SUCCESS)
	                    {
							QOS_PLUG_DBG("sending CeeRemap-FabricPriority-Success");
        	            }
            	        else
                	    {
							QOS_PLUG_DBG("sending CeeRemap-FabricPriority-Failed");
	                    }
    	                delete m;
					}
					{
        	            QosClientCreateCeeRemapMessage *m = new QosClientCreateCeeRemapMessage();
						m->setCeeMapName(pCeeMapManagedObject->getCeeMapName());
						m->setRemapPriority(pCeeMapManagedObject->getRemapLosslessPriority());
						m->setRemapFabricP(0);
	                    //TR342020, using this flag to ignor PFC sanity check during normal reboot
    	                m->setConfigReplay(1);
					
						QOS_PLUG_DBG("sending CeeRemap-LosslessPriority");
	                    status = sendSynchronouslyToWaveClient ("qos", m);
    	                if (status == WAVE_MESSAGE_SUCCESS)
        	            {
							QOS_PLUG_DBG("sending CeeRemap-LosslessPriority-Success");
                	    }
                    	else
	                    {
							QOS_PLUG_DBG("sending CeeRemap-LosslessPriority-Failed");
        	            }
            	        delete m;
					}
                }
            }
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
	}

    void QosLocalObjectManager::ConfigReplayQosQueueScheduler()
    {
        QosQueueSchedulerManagedObject *pQosQueueSchedulerManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosQueueSchedulerManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		SI32 strictPriority = 0;

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();

	    for (UI32 i=0; i< numberOfResults; i++)
            {
                pQosQueueSchedulerManagedObject = dynamic_cast<QosQueueSchedulerManagedObject *>((*pResults)[i]);
                if (pQosQueueSchedulerManagedObject)
                {
                    QosClientCreateQosQueueSchedulerMessage *m = new QosClientCreateQosQueueSchedulerMessage();

					strictPriority = pQosQueueSchedulerManagedObject->getStrictPriority();

                    m->setStrictPriority(strictPriority);
                    m->setDwrrTrafficClass0(pQosQueueSchedulerManagedObject->getDwrrTrafficClass0());
                    m->setDwrrTrafficClass1(pQosQueueSchedulerManagedObject->getDwrrTrafficClass1());
                    m->setDwrrTrafficClass2(pQosQueueSchedulerManagedObject->getDwrrTrafficClass2());
                    m->setDwrrTrafficClass3(pQosQueueSchedulerManagedObject->getDwrrTrafficClass3());
                    m->setDwrrTrafficClass4(pQosQueueSchedulerManagedObject->getDwrrTrafficClass4());
                    m->setDwrrTrafficClass5(pQosQueueSchedulerManagedObject->getDwrrTrafficClass5());
                    m->setDwrrTrafficClass6(pQosQueueSchedulerManagedObject->getDwrrTrafficClass6());
                    m->setDwrrTrafficClass7(0);

					/*
					 * getDwrrTrafficClassLast() will always be the last dwrr
					 * value configured. So, based on strict-priority assign
					 * getDwrrTrafficClassLast() to appropriate traffic class
					 */

					switch (strictPriority)
					{
						case 7:
						{
							m->setDwrrTrafficClass0(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}
						case 6:
						{
							m->setDwrrTrafficClass1(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}
						case 5:
						{
							m->setDwrrTrafficClass2(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}
						case 4:
						{
							m->setDwrrTrafficClass3(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}
						case 3:
						{
							m->setDwrrTrafficClass4(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}
						case 2:
						{
							m->setDwrrTrafficClass5(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}
						case 1:
						{
							m->setDwrrTrafficClass6(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}
						case 0:
						{
							m->setDwrrTrafficClass7(pQosQueueSchedulerManagedObject->getDwrrTrafficClassLast());
							break;
						}

					}

                    status = sendSynchronouslyToWaveClient ("qos", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayCosToCosMutationMap: configured ") + pQosQueueSchedulerManagedObject->getStrictPriority());
                    }
                    else
                    {
                        trace(TRACE_LEVEL_ERROR, string("ConfigReplayCosToCosMutationMap: Error") + pQosQueueSchedulerManagedObject->getStrictPriority());
                    }
                    delete m;
                }
            }
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

    void QosLocalObjectManager::ConfigReplayQosQueueMulticastScheduler()
    {
        QosQueueMulticastSchedulerManagedObject *pQosQueueMulticastSchedulerManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosQueueMulticastSchedulerManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();

	    for (UI32 i=0; i< numberOfResults; i++)
            {
                pQosQueueMulticastSchedulerManagedObject = dynamic_cast<QosQueueMulticastSchedulerManagedObject *>((*pResults)[i]);
                if (pQosQueueMulticastSchedulerManagedObject)
                {
                    QosClientCreateQosQueueMulticastSchedulerMessage *m = new QosClientCreateQosQueueMulticastSchedulerMessage();

                    m->setDwrrTrafficClass0(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass0());
                    m->setDwrrTrafficClass1(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass1());
                    m->setDwrrTrafficClass2(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass2());
                    m->setDwrrTrafficClass3(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass3());
                    m->setDwrrTrafficClass4(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass4());
                    m->setDwrrTrafficClass5(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass5());
                    m->setDwrrTrafficClass6(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass6());
                    m->setDwrrTrafficClass7(pQosQueueMulticastSchedulerManagedObject->getDwrrTrafficClass7());

                    status = sendSynchronouslyToWaveClient ("qos", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayCosToCosMutationMap: configured "));
                    }
                    else
                    {
                        trace(TRACE_LEVEL_ERROR, string("ConfigReplayCosToCosMutationMap: Error"));
                    }
                    delete m;
                }
            }
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

    void QosLocalObjectManager::ConfigReplayQosRcvQueueMulticastSetting()
    {
        QosRcvQueueMulticastSettingManagedObject *pQosRcvQueueMulticastSettingManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosRcvQueueMulticastSettingManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();

	    for (UI32 i=0; i< numberOfResults; i++)
            {
                pQosRcvQueueMulticastSettingManagedObject = dynamic_cast<QosRcvQueueMulticastSettingManagedObject *>((*pResults)[i]);
                if (pQosRcvQueueMulticastSettingManagedObject)
                {
					{
						if ((pQosRcvQueueMulticastSettingManagedObject->getRateLimit() != DEFAULT_RATE_LIMIT) || (pQosRcvQueueMulticastSettingManagedObject->getBurst() != DEFAULT_BURST))
						{
							QosClientCreateQosRcvQueueMulticastSettingMessage *m = new QosClientCreateQosRcvQueueMulticastSettingMessage();

							m->setRateLimit(pQosRcvQueueMulticastSettingManagedObject->getRateLimit());
							m->setBurst(pQosRcvQueueMulticastSettingManagedObject->getBurst());
							m->setOpCode(MCAST_RATE_BURST);
							status = sendSynchronouslyToWaveClient ("qos", m);
							if (status == WAVE_MESSAGE_SUCCESS)
							{
								trace(TRACE_LEVEL_SUCCESS, string("BURST: configured "));
							}
							else
							{
								trace(TRACE_LEVEL_ERROR, string("BURST: Error"));
							}
							delete m;
						}
					}
					{
						QosClientCreateQosRcvQueueMulticastSettingMessage *m = new QosClientCreateQosRcvQueueMulticastSettingMessage();

						m->setThresholdTrafficClass0(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass0());
						m->setThresholdTrafficClass1(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass1());
						m->setThresholdTrafficClass2(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass2());
						m->setThresholdTrafficClass3(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass3());
						m->setThresholdTrafficClass4(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass4());
						m->setThresholdTrafficClass5(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass5());
						m->setThresholdTrafficClass6(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass6());
						m->setThresholdTrafficClass7(pQosRcvQueueMulticastSettingManagedObject->getThresholdTrafficClass7());
						m->setOpCode(MCAST_THRESHOLD);

						status = sendSynchronouslyToWaveClient ("qos", m);
						if (status == WAVE_MESSAGE_SUCCESS)
						{
							trace(TRACE_LEVEL_SUCCESS, string("THRESHOLD: configured "));
						}
						else
						{
							trace(TRACE_LEVEL_ERROR, string("THRESHOLD: Error"));
						}
						delete m;
					}
				}
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

CeeMapManagedObject * QosLocalObjectManager::getCeeMapMOByObjectId(const ObjectId &oId)
{
	QOS_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
	CeeMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
		}
		else
		{
			QOS_PLUG_DBG("could not find MO");
		}
		free (pResults);
	}
	return mapMO;
}

DscpToDscpMutationMapManagedObject * QosLocalObjectManager::getDscpToDscpMutationMapMOByObjectId(const ObjectId &oId)
{
	QOS_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToDscpMutationMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
	DscpToDscpMutationMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<DscpToDscpMutationMapManagedObject *>((*pResults)[0]);
		}
		else
		{
			QOS_PLUG_DBG("could not find MO");
		}
		free (pResults);
	}
	return mapMO;
}

DscpToTrafficClassMapManagedObject * QosLocalObjectManager::getDscpToTrafficClassMapMOByObjectId(const ObjectId &oId)
{
	QOS_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToTrafficClassMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
	DscpToTrafficClassMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<DscpToTrafficClassMapManagedObject *>((*pResults)[0]);
		}
		else
		{
			QOS_PLUG_DBG("could not find MO");
		}
		free (pResults);
	}
	return mapMO;
}

DscpToCosMapManagedObject * QosLocalObjectManager::getDscpToCosMapMOByObjectId(const ObjectId &oId)
{
	QOS_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToCosMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
	DscpToCosMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<DscpToCosMapManagedObject *>((*pResults)[0]);
		}
		else
		{
			QOS_PLUG_DBG("could not find MO");
		}
		free (pResults);
	}
	return mapMO;
}

CosToCosMutationMapManagedObject * QosLocalObjectManager::getCosToCosMutationMapMOByObjectId(const ObjectId &oId)
{
	QOS_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *>* pResults = querySynchronously(&syncQueryCtxt);
	CosToCosMutationMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<CosToCosMutationMapManagedObject*>((*pResults)[0]);
		}
		else
		{
			QOS_PLUG_DBG("could not find MO");
		}
        pResults->clear();
        delete pResults;
	}
	return mapMO;
}

CosToTrafficClassMapManagedObject * QosLocalObjectManager::getCosToTrafficClassMapMOByObjectId(const ObjectId &oId)
{
	QOS_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(oId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(oId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	CosToTrafficClassMapManagedObject *mapMO = NULL;
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			mapMO =  dynamic_cast<CosToTrafficClassMapManagedObject *>((*pResults)[0]);
		}
		else
		{
			QOS_PLUG_DBG("could not find MO");
		}
        pResults->clear();
        delete pResults;
	}
	return mapMO;
}

void QosLocalObjectManager::ConfigReplayAutoNasConfiguration()
{

	trace (TRACE_LEVEL_DEBUG, "Entering::ConfigReplayAutoNasConfiguration NasConfigManagedObject");
	NasConfigManagedObject *pNasConfigManagedObject = NULL;
	ResourceId status = WAVE_MESSAGE_ERROR;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NasConfigManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

	if (NULL != pResults && pResults->size() != 0)
	{
		pNasConfigManagedObject = dynamic_cast<NasConfigManagedObject*>((*pResults)[0]);
		if (pNasConfigManagedObject) {
			QosClientNasMessage *pQosClientNasMessage = new QosClientNasMessage();
			pQosClientNasMessage->setOpCode(QOS_NAS_DCMD_CONFIG);
			pQosClientNasMessage->setCmdCode(QOS_NAS_DCMD_AUTO);
			pQosClientNasMessage->setCos(pNasConfigManagedObject->getCos());
			pQosClientNasMessage->setDscp(pNasConfigManagedObject->getDscp());

			status = sendSynchronouslyToWaveClient ("qos", pQosClientNasMessage);
			if (status != WAVE_MESSAGE_SUCCESS) {
			} else {
			  	status = pQosClientNasMessage->getClientStatus();
			  	if (status != WAVE_MESSAGE_SUCCESS) {
				}
			}
			if (status != WAVE_MESSAGE_SUCCESS) {
				trace(TRACE_LEVEL_ERROR, string("ConfigReplayAutoNasConfiguration: NasConfigManagedObject: Error=") + status);
			} else {
				status = pQosClientNasMessage->getCompletionStatus();
				if (status != WAVE_MESSAGE_SUCCESS) {
					trace(TRACE_LEVEL_ERROR, string("ConfigReplayAutoNasConfiguration: NasConfigManagedObject: Completion Error=") + status);
				} else {
					status = pQosClientNasMessage->getClientStatus();
					if (status != WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_ERROR, string("ConfigReplayAutoNasConfiguration: NasConfigManagedObject client Error=") + status);
					}
				}
			}
			delete pQosClientNasMessage;
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
	trace(TRACE_LEVEL_INFO, string("Done with postboot of ConfigReplayAutoNasConfiguration: NasConfigManagedObject"));

	trace (TRACE_LEVEL_DEBUG, "Entering::ConfigReplayAutoNasConfiguration NasServerIpManagedObject");
	NasServerIpManagedObject *pNasServerIpManagedObject = NULL;
	status = WAVE_MESSAGE_ERROR;
	IpV4AddressNetworkMask serverAddress;
	QosClientNasMessage *pQosClientNasMessage = NULL;
	vector<WaveManagedObjectPointer<NasServerIpVrfManagedObject> > vrfVector;
	vector<WaveManagedObjectPointer<NasServerIpVlanManagedObject> > vlanVector;
	bool isVrf = false;
	UI32 count = 0;
	WaveManagedObjectSynchronousQueryContext serverQueryCtxt(NasServerIpManagedObject::getClassName());
	vector<WaveManagedObject *> *pServerResults = querySynchronously(&serverQueryCtxt);

	if (NULL != pServerResults) {
		UI32 numberOfResults = pServerResults->size();

        for (UI32 i=0; i< numberOfResults; i++) {
			pNasServerIpManagedObject = dynamic_cast<NasServerIpManagedObject*>((*pServerResults)[i]);

			if (pNasServerIpManagedObject) {
		  		count = 0;
				isVrf = false;
				vrfVector.clear();
				vlanVector.clear();

				if (pNasServerIpManagedObject->getVrfs().size() != 0) {
					count = pNasServerIpManagedObject->getVrfs().size();
					isVrf = true;
					vrfVector = pNasServerIpManagedObject->getVrfs();
				} else if (pNasServerIpManagedObject->getVlans().size() != 0) {
					count = pNasServerIpManagedObject->getVlans().size();
					vlanVector = pNasServerIpManagedObject->getVlans();
				}

				for (UI32 j = 0 ; j < count ; j++) {
					pQosClientNasMessage = new QosClientNasMessage();
					pQosClientNasMessage->setOpCode(QOS_NAS_DCMD_CONFIG);
					pQosClientNasMessage->setCmdCode(QOS_NAS_DCMD_SERVER_IP);
					serverAddress = pNasServerIpManagedObject->getServerIpAddressPrefix();
					pQosClientNasMessage->setServerIpAddressPrefix(serverAddress);
					if (isVrf) {
						pQosClientNasMessage->setVrfName(vrfVector[j]->getVrfName());
					} else {
						pQosClientNasMessage->setVlanNumber(vlanVector[j]->getVlanNumber());
					}

					status = sendSynchronouslyToWaveClient ("qos", pQosClientNasMessage);
					if (status != WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_ERROR, string("NasServerIpManagedObject postboot failed for server ") +
						  serverAddress.toString() + string(" Error=") + status);
					} else {
					  	status = pQosClientNasMessage->getCompletionStatus();
						if (status != WAVE_MESSAGE_SUCCESS) {
							trace(TRACE_LEVEL_ERROR, string("NasServerIpManagedObject postboot failed for server ") +
								serverAddress.toString() + string(" completion status Error=") + status);
						} else {
			  				status = pQosClientNasMessage->getClientStatus();
			  				if (status != WAVE_MESSAGE_SUCCESS) {
								trace(TRACE_LEVEL_ERROR, string("NasServerIpManagedObject postboot failed for server ") +
						  			serverAddress.toString() + string(" Client Error=") + status);
							}
						}
					}
					delete pQosClientNasMessage;
				}
			}
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pServerResults);
	}
	trace(TRACE_LEVEL_INFO, string("Done with postboot of ConfigReplayAutoNasConfiguration: NasConfigManagedObject"));
}
}
