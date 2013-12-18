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

#include "Utils/Capabilities.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/QosLocalInterfaceWorker.h"
#include "Nsm/Local/NsmLocalQosPfcConfigMessage.h"
#include "Nsm/Local/QosCosProfileAndThresholdConfigMessage.h"
#include "Nsm/Local/NsmQosPfcConfigSPMessage.h"
#include "Nsm/Local/QosPoProfileAndThresholdSPMessage.h"
#include "Nsm/Local/PfcFlowControlLocalManagedObject.h"
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Nsm/Local/NsmQosCosTailDropConfigMessage.h"
#include "Nsm/Local/QosCosToProfileIdManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Ssm/QosClientConfigIntfNsmAttributesMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include <sstream>
namespace DcmNs
{

    QosLocalInterfaceWorker::QosLocalInterfaceWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        PfcFlowControlLocalManagedObject    PfcFlowControlLocalManagedObject    (pNsmLocalObjectManager);
        PfcFlowControlLocalManagedObject.setupOrm ();
        addManagedClass (PfcFlowControlLocalManagedObject::getClassName (), this);

		QosCosToTailDropManagedObject QosCosToTailDropManagedObject (pNsmLocalObjectManager);
		QosCosToTailDropManagedObject.setupOrm ();
		addManagedClass (QosCosToTailDropManagedObject::getClassName (), this);

		QosCosToProfileIdManagedObject  QosCosToProfileIdManagedObject (pNsmLocalObjectManager);
        QosCosToProfileIdManagedObject.setupOrm ();
        addManagedClass (QosCosToProfileIdManagedObject::getClassName (), this);

        addOperationMap (NSMQOSPFCCONFIG, reinterpret_cast<PrismMessageHandler> (&QosLocalInterfaceWorker::NsmLocalQosPfcConfigMessageHandler));
        addOperationMap (NSMQOSPFCCONFIGSP, reinterpret_cast<PrismMessageHandler> (&QosLocalInterfaceWorker::NsmQosPfcConfigSPMessageHandler));
		addOperationMap (NSMQOSCOSTHRESHOLDCONFIG, reinterpret_cast<PrismMessageHandler> (&QosLocalInterfaceWorker::NsmQosCosTailDropConfigMessageHandler));
		addOperationMap (QOSCOSPROFILEANDTHRESHOLDCONFIG, reinterpret_cast<PrismMessageHandler> (&QosLocalInterfaceWorker::QosCosProfileAndThresholdConfigMessageHandler));
    	addOperationMap (QOSPOPROFILEANDTHRESHOLDSP, reinterpret_cast<PrismMessageHandler> (&QosLocalInterfaceWorker::QosPoProfileAndThresholdSPMessageHandler));
    }

    QosLocalInterfaceWorker::~QosLocalInterfaceWorker ()
    {
    }

    PrismMessage  *QosLocalInterfaceWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMQOSPFCCONFIG :
                pPrismMessage = new NsmLocalQosPfcConfigMessage ();
                break;
            case NSMQOSPFCCONFIGSP:
                pPrismMessage = new NsmQosPfcConfigSPMessage();
                break;
			case NSMQOSCOSTHRESHOLDCONFIG:
				pPrismMessage = new NsmQosCosTailDropConfigMessage();
				break;
			case QOSCOSPROFILEANDTHRESHOLDCONFIG:
				pPrismMessage = new QosCosProfileAndThresholdConfigMessage();
				break;
            case QOSPOPROFILEANDTHRESHOLDSP:
				 pPrismMessage = new QosPoProfileAndThresholdSPMessage();
				break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *QosLocalInterfaceWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PfcFlowControlLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PfcFlowControlLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        } else if ((QosCosToTailDropManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new QosCosToTailDropManagedObject(
					dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((QosCosToProfileIdManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new QosCosToProfileIdManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((QosCosToTailDropManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new QosCosToTailDropManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "QosLocalInterfaceWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	ResourceId QosLocalInterfaceWorker::updateQosPfcIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    NsmLocalQosPfcConfigMessage *pNsmLocalQosPfcConfigMessage = dynamic_cast<NsmLocalQosPfcConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("Inside updateQosPfcIntfStep -------"));
    ResourceId status = WAVE_MESSAGE_SUCCESS;


    //Validation
	switch (pNsmLocalQosPfcConfigMessage->getOpCode()) {
	case CONFIGURE_INTERFACE_QOS: {
		switch (pNsmLocalQosPfcConfigMessage->getCmdCode()) {
		case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH: {
			status = WAVE_MESSAGE_SUCCESS;
			if ((DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED)) {
				status = WRC_SSM_INTF_QOS_FLOW_CONTROL_BLOCKED_IN_VCS_MODE;
				return status;
			}
		}
		}
	}
	}

	char buffer[25];
	buffer[24] = '\0';
	UI32 bufferSize = sizeof (buffer);
    QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
    trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getIfName()=") + pNsmLocalQosPfcConfigMessage->getIfName());
    string ifName = pNsmLocalQosPfcConfigMessage->getIfName();
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
	
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeString (&ifName, "Id"));
	vector<WaveManagedObject *> *phyIntPResults = querySynchronously (&syncQueryCtxt);

	if (phyIntPResults && phyIntPResults->size() == 1) {
		pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*phyIntPResults)[0]);
		/*Check ISL Port or Not */
		if (pPhyIntfLocalManagedObject)
		{
			if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole()) )
			{
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyIntPResults);
				if (m)
					delete m;
				return (WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR);
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyIntPResults);
		}
	}

    if(true  == NsmUtils::isValidThreeTuple(ifName)) 
	{
       	string twoTuppleIfName;
		NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
		ifName = twoTuppleIfName;
	}
	GENERATE_SHORT_IF_NAME_FOR_BACK_END(pNsmLocalQosPfcConfigMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
	ifName = buffer;
	
	//ifName.insert(0, "te");
    trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage-ifName()=") + ifName);

    m->setIfName(ifName);
    m->setOpCode(pNsmLocalQosPfcConfigMessage->getOpCode());
    m->setCmdCode(pNsmLocalQosPfcConfigMessage->getCmdCode());

    trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getOpCode()=") + pNsmLocalQosPfcConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getCmdCode()=") + pNsmLocalQosPfcConfigMessage->getCmdCode());
    switch (pNsmLocalQosPfcConfigMessage->getOpCode())
    {
		case CONFIGURE_INTERFACE_QOS:
        {
          switch (pNsmLocalQosPfcConfigMessage->getCmdCode())
          {
            case QOS_INTERFACE_PFC_FLOW_CONTROL_TX:
                m->setPfcFlowControlCos(pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
				m->setPfcFlowControlTx(pNsmLocalQosPfcConfigMessage->getPfcFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos()=") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getPfcFlowControlTx()=") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlTx());
                break;
            case QOS_INTERFACE_PFC_FLOW_CONTROL_RX:
                m->setPfcFlowControlCos(pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
				m->setPfcFlowControlRx(pNsmLocalQosPfcConfigMessage->getPfcFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos()=") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getPfcFlowControlRx()=") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlRx());
                break;

            case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
            {
                m->setPfcFlowControlCos(pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
				m->setPfcFlowControlTx(pNsmLocalQosPfcConfigMessage->getPfcFlowControlTx());
				m->setPfcFlowControlRx(pNsmLocalQosPfcConfigMessage->getPfcFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos()=") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getPfcFlowControlTx()=") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmLocalQosPfcConfigMessage->getPfcFlowControlRx()=") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlRx());
                break;
            }

        }
            break;
        }
		case DELETE_INTERFACE_QOS:
        {
            trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
			switch(pNsmLocalQosPfcConfigMessage->getCmdCode()) {
                case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
                	m->setPfcFlowControlCos(pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
                break;
            }
       break;
       }

   }

    m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    status = sendSynchronouslyToWaveClient ("qos",m);

    trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyToWaveClient  ***** status= ") + status);

    pNsmLocalQosPfcConfigMessage->setCompletionStatus(status);

	if(status == WAVE_MESSAGE_SUCCESS)
   	{
		if(m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
		{
			status = m->getCompletionStatus();
            trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
            pNsmLocalQosPfcConfigMessage->setCompletionStatus(status);

		}
		else
		{
			status = m->getClientStatus();
            trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
            pNsmLocalQosPfcConfigMessage->setCompletionStatus(status);
		}
	}
	//bpatel: To avoid memory leak
	if(m){
		delete m;
	}
    return status;
}

	ResourceId QosLocalInterfaceWorker::updateQosCosThresholdIntfStep (PrismSynchronousLinearSequencerContext
			*pPrismSynchronousLinearSequencerContext)
	{

		// qos rcv-queue cos-threshold command is not supported on wolverine based switches.

	if (!Capabilities::instance()->isCapabilityPresent(
			QOS_RCV_QUEUE_PER_INTERFACE_CAPABILITY)) {

		return (WRC_SSM_INTF_QOS_RCV_QUEUE_NOT_SUPPORTED_ERROR);
	}

		NsmQosCosTailDropConfigMessage *pNsmQosCosTailDropConfigMessage = dynamic_cast<NsmQosCosTailDropConfigMessage*>
			(pPrismSynchronousLinearSequencerContext->getPPrismMessage());
		trace (TRACE_LEVEL_DEBUG, string("Inside updateQosCosThresholdIntfStep ---"));
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		char buffer[25];
		buffer[24] = '\0';
		UI32 bufferSize = sizeof (buffer);
		QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
		trace (TRACE_LEVEL_DEBUG, string("pNsmQosCosTailDropConfigMessage->getIfName()=") +
				pNsmQosCosTailDropConfigMessage->getIfName());
		string ifName = pNsmQosCosTailDropConfigMessage->getIfName();

		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (&ifName, "Id"));
		syncQueryCtxt.addSelectField("Portrole");
		vector<WaveManagedObject *> *phyIntPResults = querySynchronously (&syncQueryCtxt);

		if (phyIntPResults && phyIntPResults->size() == 1) {
			pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*phyIntPResults)[0]);
			/*Check ISL Port or Not */
			if (pPhyIntfLocalManagedObject)
			{
                if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole()) ) {

                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyIntPResults);
                    if(NULL != m) {
                        delete m;
                    }
                    if(CONFIGURE_INTERFACE_QOS == pNsmQosCosTailDropConfigMessage->getOpCode()) {
                        status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                    }
                    pNsmQosCosTailDropConfigMessage->setCompletionStatus(status);
                    return status;
                } else {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyIntPResults);
                }
			}
		}

		if(true  == NsmUtils::isValidThreeTuple(ifName)) 
		{
			string twoTuppleIfName;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}
		GENERATE_SHORT_IF_NAME_FOR_BACK_END(pNsmQosCosTailDropConfigMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
		ifName = buffer;

		trace (TRACE_LEVEL_DEBUG, string("getIfType=") + pNsmQosCosTailDropConfigMessage->getIfType());
		trace (TRACE_LEVEL_DEBUG, string("new ifName = ") + ifName);
		
		m->setIfName(ifName);
		m->setOpCode(pNsmQosCosTailDropConfigMessage->getOpCode());
		m->setCmdCode(pNsmQosCosTailDropConfigMessage->getCmdCode());

		trace (TRACE_LEVEL_DEBUG, string("pNsmQosCosTailDropConfigMessage->getOpCode()=") +
				pNsmQosCosTailDropConfigMessage->getOpCode());
		trace (TRACE_LEVEL_DEBUG, string("pNsmQosCosTailDropConfigMessage->getCmdCode()=") +
				pNsmQosCosTailDropConfigMessage->getCmdCode());

		if (pNsmQosCosTailDropConfigMessage->getOpCode() == CONFIGURE_INTERFACE_QOS) {
			m->setCos0Threshold(pNsmQosCosTailDropConfigMessage->getCos0Threshold());
			m->setCos1Threshold(pNsmQosCosTailDropConfigMessage->getCos1Threshold());
			m->setCos2Threshold(pNsmQosCosTailDropConfigMessage->getCos2Threshold());
			m->setCos3Threshold(pNsmQosCosTailDropConfigMessage->getCos3Threshold());
			m->setCos4Threshold(pNsmQosCosTailDropConfigMessage->getCos4Threshold());
			m->setCos5Threshold(pNsmQosCosTailDropConfigMessage->getCos5Threshold());
			m->setCos6Threshold(pNsmQosCosTailDropConfigMessage->getCos6Threshold());
			m->setCos7Threshold(pNsmQosCosTailDropConfigMessage->getCos7Threshold());
		}
		
		m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		status = sendSynchronouslyToWaveClient ("qos",m);

		trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyToWaveClient  ***** status= ") + status);
		pNsmQosCosTailDropConfigMessage->setCompletionStatus(status);

		if (status == WAVE_MESSAGE_SUCCESS) {
			if(m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
			{
				status = m->getCompletionStatus();
				trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
				pNsmQosCosTailDropConfigMessage->setCompletionStatus(status);

			}
			else
			{
				status = m->getClientStatus();
				trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
				pNsmQosCosTailDropConfigMessage->setCompletionStatus(status);
			}
		}
		if (m) {
			delete m;
		}
		return (status);
	}


	ResourceId QosLocalInterfaceWorker::updateQosPfcIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	trace (TRACE_LEVEL_DEBUG, string("Inside updateQosPfcIntfMOStep-------"));
    NsmLocalQosPfcConfigMessage *pNsmLocalQosPfcConfigMessage = dynamic_cast<NsmLocalQosPfcConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    if(pNsmLocalQosPfcConfigMessage->getOpCode())
	{
		trace (TRACE_LEVEL_DEBUG, string("DB pNsmLocalQosPfcConfigMessage->getOpCode()=") + pNsmLocalQosPfcConfigMessage->getOpCode());
	}
	else
	  trace (TRACE_LEVEL_DEBUG, string("DB	pNsmLocalQosPfcConfigMessage->getOpCode()=") );
	if(pNsmLocalQosPfcConfigMessage->getCmdCode())
	{
		trace (TRACE_LEVEL_DEBUG, string("DB pNsmLocalQosPfcConfigMessage->getCmdCode()=") + pNsmLocalQosPfcConfigMessage->getCmdCode());
    }
	else
	  trace (TRACE_LEVEL_DEBUG, string("DB pNsmLocalQosPfcConfigMessage->getCmdCode()="));
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
    string ifName = pNsmLocalQosPfcConfigMessage->getIfName();
    //ResourceId  status   = WAVE_MESSAGE_ERROR;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
    	UI32 numberOfResults = pResults->size ();
       	if (0 == numberOfResults) {
       		trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
           	//status = WAVE_MESSAGE_ERROR;
		 } else if (1 == numberOfResults) {
          	trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
           	pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
           	//status = WAVE_MESSAGE_SUCCESS;
          } else {
                //status = WAVE_MESSAGE_ERROR;
           }
      }

        if (pPhyIntfLocalManagedObject == NULL) {
            return (WAVE_MESSAGE_ERROR);
        }

        updateWaveManagedObject (pPhyIntfLocalManagedObject);
        prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);
		switch(pNsmLocalQosPfcConfigMessage->getOpCode())
        {
            case CONFIGURE_INTERFACE_QOS:
				{
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
                	switch (pNsmLocalQosPfcConfigMessage->getCmdCode()) {
						case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
						{
                    	trace (TRACE_LEVEL_DEBUG, string("DB QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH pfc =") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
						UI32 tx = 0, rx = 0;
						SI32 ret = pPhyIntfLocalManagedObject->getPfcFlowControlRow(pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos(), tx, rx);
						if (!ret) {
                    		trace (TRACE_LEVEL_DEBUG, string("DB got the pfc row"));
						  	pPhyIntfLocalManagedObject->deletePfc(pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
						  
						} 
                    	trace (TRACE_LEVEL_DEBUG, string("DB new pfc row"));
							PfcFlowControlLocalManagedObject *pfcMO = 
							new PfcFlowControlLocalManagedObject( dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()),
										pNsmLocalQosPfcConfigMessage->getPfcFlowControlTx(), pNsmLocalQosPfcConfigMessage->getPfcFlowControlRx(),
										pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
							pPhyIntfLocalManagedObject->addPfc(pfcMO);
							break;
						}
					}
					break;
				}
			case DELETE_INTERFACE_QOS:
				{
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE DELETE_INTERFACE_QOS-------"));
                	switch (pNsmLocalQosPfcConfigMessage->getCmdCode()) {
						case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
						{
                    	trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH pfc =") + pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
						pPhyIntfLocalManagedObject->deletePfc(pNsmLocalQosPfcConfigMessage->getPfcFlowControlCos());
						break;
						}
					}
				break;
				}
		}
		trace (TRACE_LEVEL_DEBUG, string("DB ***** completionstatus = ") + pNsmLocalQosPfcConfigMessage->getCompletionStatus());
        if (NULL != pResults) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        return pNsmLocalQosPfcConfigMessage->getCompletionStatus ();
  }

	ResourceId QosLocalInterfaceWorker::updateQosCosThresholdIntfMOStep (PrismSynchronousLinearSequencerContext
			*pPrismSynchronousLinearSequencerContext)
	{
		trace (TRACE_LEVEL_DEBUG, string("Inside updateQosCosThresholdIntfMOStep---"));
		NsmQosCosTailDropConfigMessage *pNsmQosCosTailDropConfigMessage = dynamic_cast<NsmQosCosTailDropConfigMessage *>
			(pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		trace (TRACE_LEVEL_DEBUG, string("DB pNsmQosCosTailDropConfigMessage->getOpCode()=") +
				pNsmQosCosTailDropConfigMessage->getOpCode());
		trace (TRACE_LEVEL_DEBUG, string("DB pNsmQosCosTailDropConfigMessage->getCmdCode()=") +
				pNsmQosCosTailDropConfigMessage->getCmdCode());

		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		QosCosToTailDropManagedObject *pQosCosToTailDropManagedObject = NULL;
		WaveManagedObjectPointer<QosCosToTailDropManagedObject> pQosCosToTailDropMO;
		string ifName = pNsmQosCosTailDropConfigMessage->getIfName();
		//ResourceId  status   = WAVE_MESSAGE_ERROR;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
				//status = WAVE_MESSAGE_ERROR;
			 } else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
				pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
				//status = WAVE_MESSAGE_SUCCESS;
			  } else {
					//status = WAVE_MESSAGE_ERROR;
			   }
		}

        prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);
		pQosCosToTailDropMO = WaveManagedObjectPointer<QosCosToTailDropManagedObject>
			(pPhyIntfLocalManagedObject->getCosTailDropConfig());
		pQosCosToTailDropManagedObject = pQosCosToTailDropMO.operator-> ();
		
		if (NULL != pResults) {
			pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			pResults->clear();
			delete pResults;
		}
		
		switch(pNsmQosCosTailDropConfigMessage->getOpCode())
		{
			case CONFIGURE_INTERFACE_QOS:
				{
					if (pQosCosToTailDropManagedObject) {
						trace(TRACE_LEVEL_DEBUG, string("DATABASE UPDATE CONFIGURE_INTERFACE_QOS---"));
						pPhyIntfLocalManagedObject->setCosTailDropConfig(WaveManagedObjectPointer<QosCosToTailDropManagedObject>
								(NULL));
					} else {
						trace(TRACE_LEVEL_DEBUG, string("DATABASE CREATE CONFIGURE_INTERFACE_QOS---"));
					}

					pQosCosToTailDropManagedObject = new QosCosToTailDropManagedObject(dynamic_cast<NsmLocalObjectManager *>
							(getPWaveObjectManager()));

					pQosCosToTailDropManagedObject->setCos0Threshold(pNsmQosCosTailDropConfigMessage->getCos0Threshold());
					pQosCosToTailDropManagedObject->setCos1Threshold(pNsmQosCosTailDropConfigMessage->getCos1Threshold());
					pQosCosToTailDropManagedObject->setCos2Threshold(pNsmQosCosTailDropConfigMessage->getCos2Threshold());
					pQosCosToTailDropManagedObject->setCos3Threshold(pNsmQosCosTailDropConfigMessage->getCos3Threshold());
					pQosCosToTailDropManagedObject->setCos4Threshold(pNsmQosCosTailDropConfigMessage->getCos4Threshold());
					pQosCosToTailDropManagedObject->setCos5Threshold(pNsmQosCosTailDropConfigMessage->getCos5Threshold());
					pQosCosToTailDropManagedObject->setCos6Threshold(pNsmQosCosTailDropConfigMessage->getCos6Threshold());
					pQosCosToTailDropManagedObject->setCos7Threshold(pNsmQosCosTailDropConfigMessage->getCos7Threshold());
					pPhyIntfLocalManagedObject->setCosTailDropConfig(WaveManagedObjectPointer<QosCosToTailDropManagedObject>
							(pQosCosToTailDropManagedObject));
					updateWaveManagedObject (pPhyIntfLocalManagedObject);
					break;
				}
			case DELETE_INTERFACE_QOS:
				{
					if (pQosCosToTailDropManagedObject) {
						trace(TRACE_LEVEL_DEBUG, string("DB DELETE CONFIGURE_INTERFACE_QOS---"));
						pPhyIntfLocalManagedObject->setCosTailDropConfig(WaveManagedObjectPointer<QosCosToTailDropManagedObject>
								(NULL));
						updateWaveManagedObject (pPhyIntfLocalManagedObject);
					} else {
						trace(TRACE_LEVEL_DEBUG, string("DB NOP DELETE CONFIGURE_INTERFACE_QOS---"));
					}
					break;
				}
		}

        /*if (NULL != pResults) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }*/
        return pNsmQosCosTailDropConfigMessage->getCompletionStatus ();
	}

    void  QosLocalInterfaceWorker::NsmLocalQosPfcConfigMessageHandler( NsmLocalQosPfcConfigMessage *pNsmLocalQosPfcConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updateQosPfcIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updateQosPfcIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmLocalQosPfcConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	
	void QosLocalInterfaceWorker::NsmQosCosTailDropConfigMessageHandler (NsmQosCosTailDropConfigMessage
			*pNsmQosCosTailDropConfigMessage)
	{
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updateQosCosThresholdIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updateQosCosThresholdIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerFailedStep)
		};

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new
			PrismSynchronousLinearSequencerContext (pNsmQosCosTailDropConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
	}

    ResourceId QosLocalInterfaceWorker::updatePOQosProfAndThresIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    QosPoProfileAndThresholdSPMessage *pQosPoProfileAndThresholdSPMessage = dynamic_cast<QosPoProfileAndThresholdSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("Inside updatePOQosProfnThresIntfStep-------"));
    
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    //Validate whether the platform supports red profile config.
    if (!Capabilities::instance()->isCapabilityPresent(
            QOS_RED_PROFILE_CAPABILITY)) {
        status = WRC_SSM_QOS_EXTENDED_QOS_STATS_NOT_SUPPORTED_ON_THIS_PLATORM;
        pQosPoProfileAndThresholdSPMessage->setCompletionStatus(status);
        return (status);
    }

    QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
    trace (TRACE_LEVEL_DEBUG, string("pQosPoProfileAndThresholdSPMessage->getPoId()=") + pQosPoProfileAndThresholdSPMessage->getPoId());

    stringstream poName;

    poName << pQosPoProfileAndThresholdSPMessage->getPoId();
    string ifName = poName.str();
    ifName.insert(0, "po");
    trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage-ifName()=") + ifName);

    m->setIfName(ifName);
    m->setOpCode(pQosPoProfileAndThresholdSPMessage->getOpCode());
    m->setCmdCode(pQosPoProfileAndThresholdSPMessage->getCmdCode());

    trace (TRACE_LEVEL_DEBUG, string("pQosPoProfileAndThresholdSPMessage->getOpCode()=") + pQosPoProfileAndThresholdSPMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("pQosPoProfileAndThresholdSPMessage->getCmdCode()=") + pQosPoProfileAndThresholdSPMessage->getCmdCode());
    switch (pQosPoProfileAndThresholdSPMessage->getOpCode())
    {
        case CONFIGURE_INTERFACE_QOS:
        {
          switch (pQosPoProfileAndThresholdSPMessage->getCmdCode())
          {
            case QOS_INTERFACE_COS_PROFILE:
                m->setCos(pQosPoProfileAndThresholdSPMessage->getCos());
                m->setRedProfile(pQosPoProfileAndThresholdSPMessage->getRedProfile());
                trace (TRACE_LEVEL_DEBUG, string("pQosPoProfileAndThresholdSPMessage->getCos()=") + pQosPoProfileAndThresholdSPMessage->getCos());
                trace (TRACE_LEVEL_DEBUG, string("pQosPoProfileAndThresholdSPMessage->getRedProfile()=") + pQosPoProfileAndThresholdSPMessage->getRedProfile());
                break;
            case QOS_INTERFACE_TAIL_DROP:
                m->setCos(pQosPoProfileAndThresholdSPMessage->getCos());
                m->setTailDrop(pQosPoProfileAndThresholdSPMessage->getTailDrop());
                trace (TRACE_LEVEL_DEBUG, string("pQosPoProfileAndThresholdSPMessage->getCos()=") + pQosPoProfileAndThresholdSPMessage->getCos());
                trace (TRACE_LEVEL_DEBUG, string("pQosPoProfileAndThresholdSPMessage->getTailDrop()=") + pQosPoProfileAndThresholdSPMessage->getTailDrop());
                break;

          }
          break;
        }
        case DELETE_INTERFACE_QOS:
        {
            trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
            m->setCos(pQosPoProfileAndThresholdSPMessage->getCos());   
       	    break;
        }
   }

    m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    status = sendSynchronouslyToWaveClient ("qos",m);

    trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyToWaveClient  ***** status= ") + status);

    pQosPoProfileAndThresholdSPMessage->setCompletionStatus(status);

    if( status == WAVE_MESSAGE_SUCCESS)
    {
        if( m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
        {
            status = m->getCompletionStatus();
			trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
            pQosPoProfileAndThresholdSPMessage->setCompletionStatus(status);

        }
        else
        {
            status = m->getClientStatus();
            trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
            pQosPoProfileAndThresholdSPMessage->setCompletionStatus(status);

        }

    }
    //bpatel : to avoid memory leak
    if(m){
        delete m;
    }
    return status;
}

	ResourceId QosLocalInterfaceWorker::updatePOQosPfcIntfStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    NsmQosPfcConfigSPMessage *pNsmQosPfcConfigSPMessage = dynamic_cast<NsmQosPfcConfigSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("Inside updatePOQosPfcIntfStep-------"));
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
    trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPoId()=") + pNsmQosPfcConfigSPMessage->getPoId());

	stringstream poName;
	
	poName << pNsmQosPfcConfigSPMessage->getPoId();
    string ifName = poName.str();
    ifName.insert(0, "po");
    trace (TRACE_LEVEL_DEBUG, string("pNsmQosPOConfigSPMessage-ifName()=") + ifName);

    m->setIfName(ifName);
    m->setOpCode(pNsmQosPfcConfigSPMessage->getOpCode());
    m->setCmdCode(pNsmQosPfcConfigSPMessage->getCmdCode());

    trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getOpCode()=") + pNsmQosPfcConfigSPMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getCmdCode()=") + pNsmQosPfcConfigSPMessage->getCmdCode());
    switch (pNsmQosPfcConfigSPMessage->getOpCode())
    {
		case CONFIGURE_INTERFACE_QOS:
        {
          switch (pNsmQosPfcConfigSPMessage->getCmdCode())
          {
            case QOS_INTERFACE_PFC_FLOW_CONTROL_TX:
                m->setPfcFlowControlCos(pNsmQosPfcConfigSPMessage->getPfcFlowControlCos());
				m->setPfcFlowControlTx(pNsmQosPfcConfigSPMessage->getPfcFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPfcFlowControlCos()=") + pNsmQosPfcConfigSPMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPfcFlowControlTx()=") + pNsmQosPfcConfigSPMessage->getPfcFlowControlTx());
                break;
            case QOS_INTERFACE_PFC_FLOW_CONTROL_RX:
                m->setPfcFlowControlCos(pNsmQosPfcConfigSPMessage->getPfcFlowControlCos());
				m->setPfcFlowControlRx(pNsmQosPfcConfigSPMessage->getPfcFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPfcFlowControlCos()=") + pNsmQosPfcConfigSPMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPfcFlowControlRx()=") + pNsmQosPfcConfigSPMessage->getPfcFlowControlRx());
                break;

            case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
            {
                m->setPfcFlowControlCos(pNsmQosPfcConfigSPMessage->getPfcFlowControlCos());
				m->setPfcFlowControlTx(pNsmQosPfcConfigSPMessage->getPfcFlowControlTx());
				m->setPfcFlowControlRx(pNsmQosPfcConfigSPMessage->getPfcFlowControlRx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPfcFlowControlCos()=") + pNsmQosPfcConfigSPMessage->getPfcFlowControlCos());
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPfcFlowControlTx()=") + pNsmQosPfcConfigSPMessage->getPfcFlowControlTx());
                trace (TRACE_LEVEL_DEBUG, string("pNsmQosPfcConfigSPMessage->getPfcFlowControlRx()=") + pNsmQosPfcConfigSPMessage->getPfcFlowControlRx());
                break;
            }

        }
            break;
        }
		case DELETE_INTERFACE_QOS:
        {
            trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
			switch(pNsmQosPfcConfigSPMessage->getCmdCode()) {
                case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
                	m->setPfcFlowControlCos(pNsmQosPfcConfigSPMessage->getPfcFlowControlCos());
                break;
            }
       break;
       }

   }

    m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    status = sendSynchronouslyToWaveClient ("qos",m);

    trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyToWaveClient  ***** status= ") + status);

    pNsmQosPfcConfigSPMessage->setCompletionStatus(status);

	if( status == WAVE_MESSAGE_SUCCESS)
	{
		if( m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
		{
			status = m->getCompletionStatus();
            trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
            pNsmQosPfcConfigSPMessage->setCompletionStatus(status);

		}
		else
		{
			status = m->getClientStatus();
            trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
            pNsmQosPfcConfigSPMessage->setCompletionStatus(status);

		}

	}
	//bpatel : to avoid memory leak
	if(m){
		delete m;
	}
    return status;
}

    void  QosLocalInterfaceWorker::NsmQosPfcConfigSPMessageHandler( NsmQosPfcConfigSPMessage *pNsmQosPfcConfigSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updatePOQosPfcIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmQosPfcConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


	 void  QosLocalInterfaceWorker::QosPoProfileAndThresholdSPMessageHandler(QosPoProfileAndThresholdSPMessage *pQosConfigSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updatePOQosProfAndThresIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
//QOS Profile and Threshold update

	void  QosLocalInterfaceWorker::QosCosProfileAndThresholdConfigMessageHandler( QosCosProfileAndThresholdConfigMessage *pQosConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updateQosCosProfileIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::updateQosCosProfileIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalInterfaceWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId QosLocalInterfaceWorker::updateQosCosProfileIntfStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	    if (!Capabilities::instance()->isCapabilityPresent(
				QOS_RED_PROFILE_CAPABILITY)) {
			return (WRC_SSM_QOS_EXTENDED_QOS_STATS_NOT_SUPPORTED_ON_THIS_PLATORM);
		}

	QosCosProfileAndThresholdConfigMessage *pQosConfigMessage = dynamic_cast<QosCosProfileAndThresholdConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("Inside updateQosCosProfileIntfStep -------"));
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    QosClientConfigIntfNsmAttributesMessage *m = new QosClientConfigIntfNsmAttributesMessage();
	char buffer[IFNAME_MAX_STR];
	buffer[IFNAME_MAX_STR - 1] = '\0';
	UI32 bufferSize = sizeof (buffer);
    trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getIfName()=") + pQosConfigMessage->getIfName());
    trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getIfType()=") + pQosConfigMessage->getIfType());
    string ifName = pQosConfigMessage->getIfName();

    bool portrole = false;
    PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext phyLocSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    phyLocSyncQueryCtxt.addAndAttribute (new AttributeString (&ifName,"Id"));
    phyLocSyncQueryCtxt.addSelectField("Portrole");
    vector<WaveManagedObject *> *phyIntPResults = querySynchronously (&phyLocSyncQueryCtxt);

    if (phyIntPResults && phyIntPResults->size() == 1) {
        pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*phyIntPResults)[0]);
        if (NULL != pPhyIntfLocalManagedObject) {
            portrole = pPhyIntfLocalManagedObject->getPortrole();
            if ( pPrismSynchronousLinearSequencerContext && NULL != phyIntPResults ) {
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*phyIntPResults);
                phyIntPResults->clear();
                delete phyIntPResults;
            }
            if ( (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (portrole)) {
                if(CONFIGURE_INTERFACE_QOS == pQosConfigMessage->getOpCode()) {
                    status = WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                }
                pQosConfigMessage->setCompletionStatus(status);
                return status;
            }
        }
    } else {
        return WAVE_MESSAGE_ERROR;
    }

    if(true  == NsmUtils::isValidThreeTuple(ifName))
    {
        string twoTuppleIfName;
        NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
        ifName = twoTuppleIfName;
    }

	GENERATE_SHORT_IF_NAME_FOR_BACK_END(pQosConfigMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
	trace(TRACE_LEVEL_DEBUG, string("buffer is: ") + buffer);
	ifName = buffer;
	trace(TRACE_LEVEL_DEBUG, string("ifName is: ") + ifName);
    trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage-ifName()=") + ifName);

    m->setIfName(ifName);
    m->setOpCode(pQosConfigMessage->getOpCode());
    m->setCmdCode(pQosConfigMessage->getCmdCode());

    trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getOpCode()=") + pQosConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getCmdCode()=") + pQosConfigMessage->getCmdCode());
    switch (pQosConfigMessage->getOpCode())
    {
        case CONFIGURE_INTERFACE_QOS:
        {
          switch (pQosConfigMessage->getCmdCode())
          {
            case QOS_INTERFACE_COS_PROFILE:
                m->setCos(pQosConfigMessage->getCos());
                m->setRedProfile(pQosConfigMessage->getRedProfile());
				trace (TRACE_LEVEL_DEBUG, string("m->getCos()=") + m->getCos());
				trace (TRACE_LEVEL_DEBUG, string("m->getRedProfile()=") + m->getRedProfile());
                trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getCos()=") + pQosConfigMessage->getCos());
                trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getRedProfile()=") + pQosConfigMessage->getRedProfile());
                break;
            case QOS_INTERFACE_TAIL_DROP:
                m->setCos(pQosConfigMessage->getCos());
                m->setTailDrop(pQosConfigMessage->getTailDrop());
                trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getCos()=") + pQosConfigMessage->getCos());
                trace (TRACE_LEVEL_DEBUG, string("pQosConfigMessage->getTailDrop()=") + pQosConfigMessage->getTailDrop());
                break;
          }
        break;
        }
        case DELETE_INTERFACE_QOS:
        {
            trace (TRACE_LEVEL_DEBUG, string("DELETE_INTERFACE_QOS"));
            m->setCos(pQosConfigMessage->getCos());
       		break;
       	}
    }

    m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    status = sendSynchronouslyToWaveClient ("qos",m);

    trace (TRACE_LEVEL_DEBUG, string("sendSynchronouslyToWaveClient  ***** status= ") + status);

    pQosConfigMessage->setCompletionStatus(status);

    if(status == WAVE_MESSAGE_SUCCESS)
    {
        if(m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
        {
            status = m->getCompletionStatus();
            trace(TRACE_LEVEL_DEBUG, string("Completion status=") + status);
            pQosConfigMessage->setCompletionStatus(status);

        }
        else
        {
            status = m->getClientStatus();
			trace(TRACE_LEVEL_DEBUG, string("Client status=") + status);
            pQosConfigMessage->setCompletionStatus(status);
        }
    }
    //bpatel: To avoid memory leak
    if(m){
        delete m;
    }
    return status;
}



ResourceId QosLocalInterfaceWorker::updateQosCosProfileIntfMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    trace (TRACE_LEVEL_DEBUG, string("Inside updateQosCosProfileIntfMOStep-------"));
    QosCosProfileAndThresholdConfigMessage *pQosConfigMessage = dynamic_cast<QosCosProfileAndThresholdConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    trace (TRACE_LEVEL_DEBUG, string("DB pQosConfigMessage->getOpCode()=") + pQosConfigMessage->getOpCode());
    trace (TRACE_LEVEL_DEBUG, string("DB pQosConfigMessage->getCmdCode()=") + pQosConfigMessage->getCmdCode());
    PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
    string ifName = pQosConfigMessage->getIfName();
    //ResourceId  status   = WAVE_MESSAGE_ERROR;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));

    //Partial DB Query
    syncQueryCtxt.addSelectField("cosProfile");
    syncQueryCtxt.addSelectField("cosTailDrop");
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
            //status = WAVE_MESSAGE_ERROR;
         } else if (1 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
            pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
            //status = WAVE_MESSAGE_SUCCESS;
          } else {
                //status = WAVE_MESSAGE_ERROR;
           }
      }

        if (pPhyIntfLocalManagedObject == NULL) {
            return (WAVE_MESSAGE_ERROR);
        }

        updateWaveManagedObject (pPhyIntfLocalManagedObject);
        prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);
		UI32 red_profile;
        switch(pQosConfigMessage->getOpCode())
        {
            case CONFIGURE_INTERFACE_QOS:
                {
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE CONFIGURE_INTERFACE_QOS-------"));
                    switch (pQosConfigMessage->getCmdCode()) {
                        case QOS_INTERFACE_COS_PROFILE:
                        {
							trace (TRACE_LEVEL_DEBUG, string("DB QOS_INTERFACE_COS_PROFILE =") + pQosConfigMessage->getCos());
							SI32 ret = pPhyIntfLocalManagedObject->getCosProfileRow(pQosConfigMessage->getCos(), red_profile);
							if (!ret) {
								trace (TRACE_LEVEL_DEBUG, string("DB got the profile row"));
								pPhyIntfLocalManagedObject->deleteCosProfile(pQosConfigMessage->getCos());

							}
							red_profile=0;
							trace (TRACE_LEVEL_DEBUG, string("DB new cos profile row= ")+pQosConfigMessage->getRedProfile());
							QosCosToProfileIdManagedObject *profileMO =
								new QosCosToProfileIdManagedObject( dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()),
											pQosConfigMessage->getCos(), pQosConfigMessage->getRedProfile());
							pPhyIntfLocalManagedObject->addCosProfile(profileMO);
                           
                            // Partial DB Update
                            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("cosProfile");
							break;
                        }
						case QOS_INTERFACE_TAIL_DROP:
                        {
							trace (TRACE_LEVEL_DEBUG, string("DB QOS_INTERFACE_TAIL_DROP =") + pQosConfigMessage->getCos());
							UI32 tail_drop;
							SI32 ret = pPhyIntfLocalManagedObject->getCosTailDropRow(pQosConfigMessage->getCos(), tail_drop);
							if (!ret) {
								trace (TRACE_LEVEL_DEBUG, string("DB got the tail drop row"));
								pPhyIntfLocalManagedObject->deleteCosTailDrop(pQosConfigMessage->getCos());

							}
							trace (TRACE_LEVEL_DEBUG, string("DB new tail drop row"));
                            QosCosToTailDropManagedObject *tailDropMO =
								new QosCosToTailDropManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
                            pPhyIntfLocalManagedObject->addCosTailDrop(tailDropMO);

                            // Partial DB Update
                            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("cosTailDrop");
                            break;
                        }

                    }
                    break;
                }
            case DELETE_INTERFACE_QOS:
                {
                    trace (TRACE_LEVEL_DEBUG, string("DB UPDATE DELETE_INTERFACE_QOS-------"));
                    switch (pQosConfigMessage->getCmdCode()) {
                        case QOS_INTERFACE_COS_PROFILE:
                        {
							trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_COS_PROFILE cos =") + pQosConfigMessage->getCos());
							pPhyIntfLocalManagedObject->deleteCosProfile(pQosConfigMessage->getCos());

                            // Partial DB Update
                            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("cosProfile");
							break;
                        }
						case QOS_INTERFACE_TAIL_DROP:
                        {
							trace (TRACE_LEVEL_DEBUG, string("DB DELETE QOS_INTERFACE_TAIL_DROP cos =") + pQosConfigMessage->getCos());
							pPhyIntfLocalManagedObject->deleteCosTailDrop(pQosConfigMessage->getCos());
                            
                            // Partial DB Update
                            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("cosTailDrop");
							break;
                        }
                    }
                break;
                }
        }
        trace (TRACE_LEVEL_DEBUG, string("DB ***** completionstatus = ") + pQosConfigMessage->getCompletionStatus());
        if (NULL != pResults) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        return pQosConfigMessage->getCompletionStatus ();
    }

}
