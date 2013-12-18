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
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/CeeGlobalWorker.h"
#include "Qos/Global/CeeGlobalCreateCeeRemapMessage.h"
#include "Qos/Global/CeeGlobalCreateCeeMapMessage.h"
#include "Qos/Global/CeeGlobalDeleteCeeMapMessage.h"
#include "Qos/Global/CeeGlobalCreatePriorityGroupMessage.h"
#include "Qos/Global/CeeGlobalDeletePriorityGroupMessage.h"
#include "Qos/Global/CeeGlobalCreatePriorityMapMessage.h"
#include "Qos/Global/CeeGlobalDeletePriorityMapMessage.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "Qos/Global/NasConfigManagedObject.h"
#include "Qos/Common/QosUtils.h"
#include "Qos/Local/QosCreateCeeRemapSPMessage.h"
#include "Qos/Local/QosCreateCeeMapSPMessage.h"
#include "Qos/Local/QosDeleteCeeMapSPMessage.h"
#include "Qos/Local/QosCreatePriorityGroupSPMessage.h"
#include "Qos/Local/QosDeletePriorityGroupSPMessage.h"
#include "Qos/Local/QosCreatePriorityMapSPMessage.h"
#include "Qos/Local/QosDeletePriorityMapSPMessage.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "DcmCStatus.h"
#include "DcmCore/DcmToolKit.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include <sstream>
#include "Utils/DceClusterUtils.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"


/* FcoeFabricMapManagedObject for remap validation */
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

    CeeGlobalWorker::CeeGlobalWorker ( QosGlobalObjectManager *pQosGlobalObjectManager)
        : WaveWorker  (pQosGlobalObjectManager)
    {
        PriorityGroupManagedObject    PriorityGroupManagedObject    (pQosGlobalObjectManager);
        CeeMapManagedObject    CeeMapManagedObject    (pQosGlobalObjectManager);
        PriorityMapManagedObject    PriorityMapManagedObject    (pQosGlobalObjectManager);
        PriorityGroupManagedObject.setupOrm ();
        addManagedClass (PriorityGroupManagedObject::getClassName (), this);
        PriorityMapManagedObject.setupOrm ();
        addManagedClass (PriorityMapManagedObject::getClassName (), this);
        CeeMapManagedObject.setupOrm ();
        addManagedClass (CeeMapManagedObject::getClassName (), this);
        addOperationMap (CEEGLOBALCREATECEEREMAP, reinterpret_cast<PrismMessageHandler> (&CeeGlobalWorker::CeeGlobalCreateCeeRemapMessageHandler));
        addOperationMap (CEEGLOBALCREATECEEMAP, reinterpret_cast<PrismMessageHandler> (&CeeGlobalWorker::CeeGlobalCreateCeeMapMessageHandler));
        addOperationMap (CEEGLOBALDELETECEEMAP, reinterpret_cast<PrismMessageHandler> (&CeeGlobalWorker::CeeGlobalDeleteCeeMapMessageHandler));
        addOperationMap (CEEGLOBALCREATEPRIORITYGROUP, reinterpret_cast<PrismMessageHandler> (&CeeGlobalWorker::CeeGlobalCreatePriorityGroupMessageHandler));
        addOperationMap (CEEGLOBALDELETEPRIORITYGROUP, reinterpret_cast<PrismMessageHandler> (&CeeGlobalWorker::CeeGlobalDeletePriorityGroupMessageHandler));
        addOperationMap (CEEGLOBALCREATEPRIORITYMAP, reinterpret_cast<PrismMessageHandler> (&CeeGlobalWorker::CeeGlobalCreatePriorityMapMessageHandler));
        addOperationMap (CEEGLOBALDELETEPRIORITYMAP, reinterpret_cast<PrismMessageHandler> (&CeeGlobalWorker::CeeGlobalDeletePriorityMapMessageHandler));
    }

    CeeGlobalWorker::~CeeGlobalWorker ()
    {
    }

    PrismMessage  *CeeGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case CEEGLOBALCREATECEEMAP :
                pPrismMessage = new CeeGlobalCreateCeeMapMessage ();
                break;
            case CEEGLOBALDELETECEEMAP :
                pPrismMessage = new CeeGlobalDeleteCeeMapMessage ();
                break;
            case CEEGLOBALCREATEPRIORITYGROUP :
                pPrismMessage = new CeeGlobalCreatePriorityGroupMessage ();
                break;
            case CEEGLOBALDELETEPRIORITYGROUP :
                pPrismMessage = new CeeGlobalDeletePriorityGroupMessage ();
                break;
            case CEEGLOBALCREATEPRIORITYMAP :
                pPrismMessage = new CeeGlobalCreatePriorityMapMessage ();
                break;
            case CEEGLOBALDELETEPRIORITYMAP :
                pPrismMessage = new CeeGlobalDeletePriorityMapMessage ();
                break;
			case CEEGLOBALCREATECEEREMAP:
				pPrismMessage = new CeeGlobalCreateCeeRemapMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *CeeGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PriorityGroupManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PriorityGroupManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((CeeMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CeeMapManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PriorityMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PriorityMapManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "CeeGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	ResourceId CeeGlobalWorker::validateRemapPriorityValue(SI32 remapPriority)
	{
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFabricMapManagedObject::getClassName());
        syncQueryCtxt.addSelectField("priority");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		FcoeFabricMapManagedObject *fcoeMapMO = NULL;
		SI32 fabric_priority;

        if (NULL == pResults) {
            trace(TRACE_LEVEL_INFO, "Query returned NULL results");
            return WAVE_MESSAGE_SUCCESS;
        }
		
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return WAVE_MESSAGE_SUCCESS;
        }

		fcoeMapMO = dynamic_cast<FcoeFabricMapManagedObject *> ((*pResults)[0]);
		if (fcoeMapMO == NULL) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			return WAVE_MESSAGE_SUCCESS;
		}

		fabric_priority = fcoeMapMO->getPriority();
		if (fabric_priority == remapPriority) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			return SSM_QOS_CEE_REMAP_PRIORITY_MATCHES_FABRIC_PRIORITY;
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (WAVE_MESSAGE_SUCCESS);
	}

  void CeeGlobalWorker::createCeeRemapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {    
	trace (TRACE_LEVEL_DEBUG, "enter createCeeRemapMOStep:");
	CeeGlobalCreateCeeRemapMessage *pCeeGlobalCreateCeeRemapMessage = dynamic_cast<CeeGlobalCreateCeeRemapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());    
    WaveSendToClusterContext    *send_ctx_p;
	ResourceId                      status = WAVE_MESSAGE_SUCCESS;

	/* TR 360126: Before sending a message to the backend, check whether the
	 * fabric priority and lossless priority conflict with fcoe fabric priority
	 */
	status = validateRemapPriorityValue(pCeeGlobalCreateCeeRemapMessage->getRemapPriority());

	if (status != WAVE_MESSAGE_SUCCESS) {
		pPrismLinearSequencerContext->executeNextStep(status);
		return;
	}
	
    QosCreateCeeRemapSPMessage *m = new QosCreateCeeRemapSPMessage();
    m->setCeeMapName(pCeeGlobalCreateCeeRemapMessage->getCeeMapName());
    m->setRemapPriority(pCeeGlobalCreateCeeRemapMessage->getRemapPriority());
	m->setRemapFabricP(pCeeGlobalCreateCeeRemapMessage->getRemapFabricP());
	trace(TRACE_LEVEL_DEBUG, string("createCeeRemapMOStep: ceeMapName ") + m->getCeeMapName());
    trace(TRACE_LEVEL_DEBUG, string("createCeeRemapMOStep: remapP ") + m->getRemapPriority());
    trace(TRACE_LEVEL_DEBUG, string("createCeeRemapMOStep: remapFabric ") + m->getRemapFabricP());

    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("CeeGlobalWorker::createCeeRemapStep: Sending msg to Cluster :"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return;
    }

    void CeeGlobalWorker::createCeeRemapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    //ResourceId status = WAVE_MESSAGE_SUCCESS;
    CeeGlobalCreateCeeRemapMessage *pCeeGlobalCreateCeeRemapMessage = dynamic_cast<CeeGlobalCreateCeeRemapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    /* search for Cee Map, if found, then update, otherwise create */
    string ceeMapName = pCeeGlobalCreateCeeRemapMessage->getCeeMapName();
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(ceeMapName,"ceeMapName"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    syncQueryCtxt.addSelectField("remapFabricPriority");
    syncQueryCtxt.addSelectField("remapLosslessPriority");
    
    CeeMapManagedObject *pCeeMapManagedObject = NULL;

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (1 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, "createCeeRemapMOStep: found CEE map, modify");
            pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
			if (pCeeGlobalCreateCeeRemapMessage->getRemapFabricP()) {
	            pCeeMapManagedObject->setRemapFabricPriority(pCeeGlobalCreateCeeRemapMessage->getRemapPriority());
                pCeeMapManagedObject->addAttributeToBeUpdated("remapFabricPriority");
			}
			else {
				pCeeMapManagedObject->setRemapLosslessPriority(pCeeGlobalCreateCeeRemapMessage->getRemapPriority());
                pCeeMapManagedObject->addAttributeToBeUpdated("remapLosslessPriority");
			}
			updateWaveManagedObject (pCeeMapManagedObject);
        }
        else {
            trace (TRACE_LEVEL_ERROR, "createCeeRemapMOStep: could not find valid CEE map");
            //status = WAVE_MESSAGE_ERROR;
        }
        // Mark the query results for GC
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        pResults->clear ();
        delete pResults;
    }
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }                                                                                             
		
    void  CeeGlobalWorker::CeeGlobalCreateCeeRemapMessageHandler( CeeGlobalCreateCeeRemapMessage *pCeeGlobalCreateCeeRemapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createCeeRemapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createCeeRemapMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCeeGlobalCreateCeeRemapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

  void CeeGlobalWorker::createCeeMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
	trace(TRACE_LEVEL_INFO, "enter CeeGlobalWorker::createCeeMapStep");

	CeeGlobalCreateCeeMapMessage *pCeeGlobalCreateCeeMapMessage = dynamic_cast<CeeGlobalCreateCeeMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

	WaveSendToClusterContext    *send_ctx_p;
	QosCreateCeeMapSPMessage *m = new QosCreateCeeMapSPMessage();

	m->setCeeMapName(pCeeGlobalCreateCeeMapMessage->getCeeMapName());
	m->setPrecedence(pCeeGlobalCreateCeeMapMessage->getPrecedence());
	m->setConfigReplay(pCeeGlobalCreateCeeMapMessage->getConfigReplay());

	trace(TRACE_LEVEL_INFO, string("CeeGlobalWorker:: configReplay = ") + pCeeGlobalCreateCeeMapMessage->getConfigReplay());

	send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
	prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
	send_ctx_p->setPPrismMessageForPhase1(m);
	trace(TRACE_LEVEL_INFO, string("CeeGlobalWorker::createCeeMapStep: Sending msg to Cluster :") + pCeeGlobalCreateCeeMapMessage->getCeeMapName());
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
	sendToWaveCluster(send_ctx_p);
	return ;
	}

	void CeeGlobalWorker::createCeeMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
	trace(TRACE_LEVEL_INFO, "enter CeeGlobalWorker::createCeeMapMOStep");

	CeeGlobalCreateCeeMapMessage *pCeeGlobalCreateCeeMapMessage = dynamic_cast<CeeGlobalCreateCeeMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

	/* search for Cee Map, if found, then update, otherwise create */
    string ceeMapName = pCeeGlobalCreateCeeMapMessage->getCeeMapName();
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(ceeMapName,"ceeMapName"));
    syncQueryCtxt.addSelectField("precedence");
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    CeeMapManagedObject *pCeeMapManagedObject = NULL;
	
    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, "createCeeMapMOStep: create CEE Map");
			trace (TRACE_LEVEL_DEBUG, "createCeeMapMOStep: create PGID 15.0-15.7");
			PriorityGroupManagedObject *pr_grp0_mo_p;
			pr_grp0_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.0", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp1_mo_p;
            pr_grp1_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.1", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp2_mo_p;
            pr_grp2_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.2", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp3_mo_p;
            pr_grp3_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.3", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp4_mo_p;
            pr_grp4_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.4", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp5_mo_p;
            pr_grp5_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.5", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp6_mo_p;
            pr_grp6_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.6", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp7_mo_p;
            pr_grp7_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.7", 0, false, ceeMapName);

		    vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pr_grp_vector;

    		WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp0_obj_ptr(pr_grp0_mo_p);
		    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp1_obj_ptr(pr_grp1_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp2_obj_ptr(pr_grp2_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp3_obj_ptr(pr_grp3_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp4_obj_ptr(pr_grp4_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp5_obj_ptr(pr_grp5_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp6_obj_ptr(pr_grp6_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp7_obj_ptr(pr_grp7_mo_p);

		    pr_grp_vector.push_back(pr_grp0_obj_ptr);
		    pr_grp_vector.push_back(pr_grp1_obj_ptr);
            pr_grp_vector.push_back(pr_grp2_obj_ptr);
            pr_grp_vector.push_back(pr_grp3_obj_ptr);
            pr_grp_vector.push_back(pr_grp4_obj_ptr);
            pr_grp_vector.push_back(pr_grp5_obj_ptr);
            pr_grp_vector.push_back(pr_grp6_obj_ptr);
            pr_grp_vector.push_back(pr_grp7_obj_ptr);

            trace (TRACE_LEVEL_DEBUG, "createCeeMapMOStep: create P-T table");
		    PriorityMapManagedObject *pr_map_mo_p;
		    pr_map_mo_p = new PriorityMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),                                         											     pr_grp6_mo_p->getObjectId(),
                                               pr_grp7_mo_p->getObjectId(),
                                               pr_grp5_mo_p->getObjectId(),
                                               pr_grp4_mo_p->getObjectId(),
                                               pr_grp3_mo_p->getObjectId(),
                                               pr_grp2_mo_p->getObjectId(),
                                               pr_grp1_mo_p->getObjectId(),
                                               pr_grp0_mo_p->getObjectId());
			pr_map_mo_p->setCeeMapName(ceeMapName);
		    WaveManagedObjectPointer<PriorityMapManagedObject> pr_map_obj_ptr(pr_map_mo_p);

			trace (TRACE_LEVEL_DEBUG, "createCeeMapMOStep: create CEE map");
		    pCeeMapManagedObject = new CeeMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), pCeeGlobalCreateCeeMapMessage->getCeeMapName(), pCeeGlobalCreateCeeMapMessage->getPrecedence(), pr_grp_vector, pr_map_obj_ptr);
			pCeeMapManagedObject->setPrecedence(pCeeGlobalCreateCeeMapMessage->getPrecedence());
			/* set up default remapped Priorities, need VCS judge here !!! */
			pCeeMapManagedObject->setRemapFabricPriority(0);
			pCeeMapManagedObject->setRemapLosslessPriority(0);

            // Mark the MO for GC
            if (pCeeMapManagedObject)
                pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pCeeMapManagedObject);
        } 
		else if (1 == numberOfResults) {
			pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
			pCeeMapManagedObject->setPrecedence(pCeeGlobalCreateCeeMapMessage->getPrecedence());
            pCeeMapManagedObject->addAttributeToBeUpdated("precedence");
            updateWaveManagedObject(pCeeMapManagedObject);
#if 0
			if (pCeeGlobalCreateCeeMapMessage->getConfigReplay())
			{
				/* if configreplay, clean the current default cee map config */
				trace (TRACE_LEVEL_INFO, "createCeeMapMOStep: clear default CEE config");
//				WaveManagedObjectPointer<PriorityMapManagedObject> pr_map_mo_p;
//				pr_map_mo_p = pCeeMapManagedObject->getPriorityMap();
			    // looking up for PGIDs refered in the cleared PrioirtyTable
			    WaveManagedObjectPointer<PriorityGroupManagedObject> *pPriorityGroupMO = NULL;
//			    vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pgid_vector = pCeeMapManagedObject->getPriorityGroup();
			    pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.6");
			    if (pPriorityGroupMO == NULL) {
			        trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.6"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			        return;
			    }
			    else {
					pCeeMapManagedObject->updatePriorityMap(0, (*pPriorityGroupMO)->getObjectId());
    			}
                pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.7");
                if (pPriorityGroupMO == NULL) {
                    trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.7"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
                    return;
                }
                else {
                    pCeeMapManagedObject->updatePriorityMap(1, (*pPriorityGroupMO)->getObjectId());
                }
                pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.5");
                if (pPriorityGroupMO == NULL) {
                    trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.5"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
                    return;
                }
                else {
                    pCeeMapManagedObject->updatePriorityMap(2, (*pPriorityGroupMO)->getObjectId());
                }
                pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.4");
                if (pPriorityGroupMO == NULL) {
                    trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.4"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
                    return;
                }
                else {
                    pCeeMapManagedObject->updatePriorityMap(3, (*pPriorityGroupMO)->getObjectId());
                }
                pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.3");
                if (pPriorityGroupMO == NULL) {
                    trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.3"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
                    return;
                }
                else {
                    pCeeMapManagedObject->updatePriorityMap(4, (*pPriorityGroupMO)->getObjectId());
                }
                pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.2");
                if (pPriorityGroupMO == NULL) {
                    trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.2"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
                    return;
                }
                else {
                    pCeeMapManagedObject->updatePriorityMap(5, (*pPriorityGroupMO)->getObjectId());
                }
                pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.1");
                if (pPriorityGroupMO == NULL) {
                    trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.1"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
                    return;
                }
                else {
                    pCeeMapManagedObject->updatePriorityMap(6, (*pPriorityGroupMO)->getObjectId());
                }
                pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName("15.0");
                if (pPriorityGroupMO == NULL) {
                    trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = 15.0"));
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
                    return;
                }
                else {
                    pCeeMapManagedObject->updatePriorityMap(7, (*pPriorityGroupMO)->getObjectId());
                }
				PriorityMapManagedObject *pr_map_mo_p = (pCeeMapManagedObject->getPriorityMap()).operator ->();
				updateWaveManagedObject(pr_map_mo_p);
					
				/* delete the non-strict priority MO */
				int i = 0;
				for (i = 0; i < 8; i++) {
					stringstream s;
					s << i;
					pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName(s.str());
					if (pPriorityGroupMO == NULL) {
						continue;
					}
					PriorityGroupManagedObject *pgidMo = NULL;
					pgidMo = (*pPriorityGroupMO).operator ->();
					if (pgidMo == NULL) {
						trace(TRACE_LEVEL_ERROR, string("createCeeMapMOStep failed: searching for PriorityGroup failed, PGID = ") + i);	
						continue;
					}
                    pCeeMapManagedObject->delPriorityGroup(s.str());
				}
			}
			else
#endif
	            trace (TRACE_LEVEL_INFO, "createCeeMapMOStep: modify");
        } 
        // Mark the query results for GC
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        pResults->clear ();
        delete pResults;
    }

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

    void  CeeGlobalWorker::CeeGlobalCreateCeeMapMessageHandler( CeeGlobalCreateCeeMapMessage *pCeeGlobalCreateCeeMapMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
	    	reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createCeeMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
	        reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createCeeMapMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCeeGlobalCreateCeeMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

  void CeeGlobalWorker::createPriorityGroupStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
	trace(TRACE_LEVEL_INFO, "enter CeeGlobalWorker::createPriorityGroupStep");

    CeeGlobalCreatePriorityGroupMessage *pCeeGlobalCreatePriorityGroupMessage = dynamic_cast<CeeGlobalCreatePriorityGroupMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

	/* VCS QoS Sanity check for PGID which enables PFC */
	if (pCeeGlobalCreatePriorityGroupMessage->getPfc()) {
		// search for the CEE map
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(pCeeGlobalCreatePriorityGroupMessage->getCeeMapName(), "ceeMapName"));
        syncQueryCtxt.addSelectField("priorityMap");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
              pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
            }
        else {
            trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for CEE map failed, CEE map: ") + pCeeGlobalCreatePriorityGroupMessage->getCeeMapName());
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        if (pResults) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        	pResults->clear();
            delete pResults;
        }

        int reservePriorityBitmap = 0;
		bool sanityNeed = false; // mark if the PGID is refered in the PriorityMap
        WaveManagedObjectPointer<PriorityMapManagedObject> p_pm;
        p_pm = pCeeMapManagedObject->getPriorityMap();
		/// if PriorityMap not established, skip check
		if (p_pm.operator ->() == NULL)
			goto next;
        PriorityGroupManagedObject *p_pg = NULL;
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos0ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mappriority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x1;	
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x1;
				sanityNeed = true;
			}
            delete p_pg;
        }
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos1ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x2;
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x2;
				sanityNeed = true;
			}
            delete p_pg;
        }
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos2ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x4;
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x4;
				sanityNeed = true;
			}
            delete p_pg;
        }
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos3ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x8;
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x8;
				sanityNeed = true;
			}
            delete p_pg;
        }
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos4ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x10;
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x10;
				sanityNeed = true;
			}
            delete p_pg;
        }
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos5ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x20;
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x20;
				sanityNeed = true;
			}
            delete p_pg;
        }
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos6ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x40;
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x40;
				sanityNeed = true;
			}
            delete p_pg;
        }
        p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos7ToPriorityGrouMap()));
        if (p_pg == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
			return;
        }
        else {
            if (p_pg->getPfc())
                reservePriorityBitmap |= 0x80;
            if (p_pg->getPgId() == pCeeGlobalCreatePriorityGroupMessage->getPgId()) {
                reservePriorityBitmap |= 0x80;
				sanityNeed = true;
			}
            delete p_pg;
        }

		if (sanityNeed) {

			ResourceId status = WAVE_MESSAGE_SUCCESS;
			//Validate whether the lossless cos conflicts with cos-mutated cos or default cos.
            status = QosUtils::vcsQosSanity(QOS_INTERFACE_CEE_MAP, pCeeGlobalCreatePriorityGroupMessage->getCeeMapName(), false, 0, reservePriorityBitmap,0);
			if (status != WAVE_MESSAGE_SUCCESS) {
				pPrismLinearSequencerContext->executeNextStep(status);
				return;
			}

            //Validate whether the lossless cos conflicts with dscp-mutated TC in dscp-tc map or dscp-mutated cos.
            status = QosUtils::vcsL3QosSanity(QOS_INTERFACE_CEE_MAP, pCeeGlobalCreatePriorityGroupMessage->getCeeMapName()
                        , false, reservePriorityBitmap, 0);

            if (status != WAVE_MESSAGE_SUCCESS) {
                pPrismLinearSequencerContext->executeNextStep(status);
                return;
            }

		
		}
	}

next:
	WaveSendToClusterContext    *send_ctx_p;
    QosCreatePriorityGroupSPMessage *m = new QosCreatePriorityGroupSPMessage();

	m->setCeeMapName(pCeeGlobalCreatePriorityGroupMessage->getCeeMapName());
    m->setPgId(pCeeGlobalCreatePriorityGroupMessage->getPgId());
    m->setWeight(pCeeGlobalCreatePriorityGroupMessage->getWeight());
	m->setPfc(pCeeGlobalCreatePriorityGroupMessage->getPfc());

	send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
	prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
	send_ctx_p->setPPrismMessageForPhase1(m);
	trace(TRACE_LEVEL_INFO, string("CeeGlobalWorker::createPriorityGroupStep: Sending msg to Cluster :"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
	sendToWaveCluster(send_ctx_p);
	return ;
    }

    void CeeGlobalWorker::createPriorityGroupMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
	trace(TRACE_LEVEL_INFO, "enter CeeGlobalWorker::createPriorityGroupMOStep");

    CeeGlobalCreatePriorityGroupMessage *pCeeGlobalCreatePriorityGroupMessage = dynamic_cast<CeeGlobalCreatePriorityGroupMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	/* search for the CEE map MO */
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(CeeMapManagedObject::getClassName());
	syncQueryCtxt1.addAndAttribute (new AttributeString(pCeeGlobalCreatePriorityGroupMessage->getCeeMapName(),"ceeMapName"));
    syncQueryCtxt1.addSelectField("priorityGroup");
	vector<WaveManagedObject *> *pResults1 = querySynchronously (&syncQueryCtxt1);
	CeeMapManagedObject *pCeeMapManagedObject = NULL;

	if (NULL != pResults1) {
		UI32 numberOfResults = pResults1->size ();
		if (0 == numberOfResults) {
			trace (TRACE_LEVEL_ERROR, "CreatePGIDMOStep: no CEE map found"); 
		}
		else if (1 == numberOfResults) {
			trace(TRACE_LEVEL_INFO, "CreatePGIDMOStep: find CEE map");
			pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults1)[0]);
		}
		else {
			trace (TRACE_LEVEL_ERROR, "CreatePGIDMOStep: more than one CEE map found");
		}
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults1);
        pResults1->clear ();
        delete pResults1;
	}
	else {
		trace (TRACE_LEVEL_ERROR, "CreatePGIDMOStep: no CEE map found");
	}

    if (pCeeMapManagedObject == NULL) {
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
        return;
    }
 
	/* search for the prioritygroup MO, if found, then update, otherwise create*/
    string pgId = pCeeGlobalCreatePriorityGroupMessage->getPgId();
	WaveManagedObjectPointer<PriorityGroupManagedObject> *pPriorityGroupMO = NULL;
	pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName(pgId);
	if (pPriorityGroupMO == NULL) {
		trace (TRACE_LEVEL_INFO, string("createPGIDMOStep: create pgid ") + pgId);
		PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;
        pPriorityGroupManagedObject = new PriorityGroupManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
        pPriorityGroupManagedObject->setPgId(pCeeGlobalCreatePriorityGroupMessage->getPgId());
        pPriorityGroupManagedObject->setWeight(pCeeGlobalCreatePriorityGroupMessage->getWeight());
        pPriorityGroupManagedObject->setPfc(pCeeGlobalCreatePriorityGroupMessage->getPfc());
        pPriorityGroupManagedObject->setCeeMapName(pCeeGlobalCreatePriorityGroupMessage->getCeeMapName());
		
		pCeeMapManagedObject->addPriorityGroup(pPriorityGroupManagedObject);
	}
	else {
		trace (TRACE_LEVEL_INFO, string("createPGIDMOStep: update pgid ") + pgId);
        trace (TRACE_LEVEL_INFO, string("createPGIDMOStep: update weight ") + pCeeGlobalCreatePriorityGroupMessage->getWeight());
		(*pPriorityGroupMO)->setWeight(pCeeGlobalCreatePriorityGroupMessage->getWeight());
		(*pPriorityGroupMO)->setPfc(pCeeGlobalCreatePriorityGroupMessage->getPfc());
		(*pPriorityGroupMO)->setCeeMapName(pCeeGlobalCreatePriorityGroupMessage->getCeeMapName());
		PriorityGroupManagedObject *pPriorityGroupManagedObject = (*pPriorityGroupMO).operator ->();
		updateWaveManagedObject(pPriorityGroupManagedObject);
	}
    pCeeMapManagedObject->addAttributeToBeUpdated("priorityGroup");
	updateWaveManagedObject(pCeeMapManagedObject);

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

#if 0
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PriorityGroupManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(pgId,"pgId"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, string("createPGIDMOStep: create pgid ") + pgId);
		    pPriorityGroupManagedObject = new PriorityGroupManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));    
			pPriorityGroupManagedObject->setPgId(pCeeGlobalCreatePriorityGroupMessage->getPgId());    
			pPriorityGroupManagedObject->setWeight(pCeeGlobalCreatePriorityGroupMessage->getWeight());    
			pPriorityGroupManagedObject->setPfc(pCeeGlobalCreatePriorityGroupMessage->getPfc());
            pPriorityGroupManagedObject->setCeeMapName(pCeeGlobalCreatePriorityGroupMessage->getCeeMapName());
			updateWaveManagedObject(pPriorityGroupManagedObject);
			// add this PGID into CEE map
			vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pr_grp_vector;
			pr_grp_vector = pCeeMapManagedObject->getPriorityGroup();
			WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp_obj_ptr(pPriorityGroupManagedObject);
			pr_grp_vector.push_back(pr_grp_obj_ptr);
			pCeeMapManagedObject->setPriorityGroup(pr_grp_vector);
			updateWaveManagedObject(pCeeMapManagedObject);

            // Mark the MO for GC
            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pCeeMapManagedObject);

            // Mark the query results for GC
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        } else if (1 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, string("createPGIDMOStep: modify pgid ") + pgId);
            pPriorityGroupManagedObject = dynamic_cast<PriorityGroupManagedObject*>((*pResults)[0]);
            pPriorityGroupManagedObject->setPgId(pCeeGlobalCreatePriorityGroupMessage->getPgId());
            pPriorityGroupManagedObject->setWeight(pCeeGlobalCreatePriorityGroupMessage->getWeight());
            pPriorityGroupManagedObject->setPfc(pCeeGlobalCreatePriorityGroupMessage->getPfc());
         pPriorityGroupManagedObject->setCeeMapName(pCeeGlobalCreatePriorityGroupMessage->getCeeMapName());
			updateWaveManagedObject(pPriorityGroupManagedObject);
        } else {
            trace (TRACE_LEVEL_ERROR, "deletePGIDMOStep: More than oneMO found");
        }
        pResults->clear ();
        delete pResults;
    }
	
	if (pPriorityGroupManagedObject != NULL)
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	else
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);

#endif
	return;
    }

	void CeeGlobalWorker::createPriorityGroupValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {    
	  	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	CeeGlobalCreatePriorityGroupMessage *pCeeGlobalCreatePriorityGroupMessage =
		  dynamic_cast<CeeGlobalCreatePriorityGroupMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

		if (WAVE_MESSAGE_SUCCESS == QosUtils::isAutoNasEnabled()) {
		  	/* if auto nas is enabled, PGID 3 cannot be modified. */
			if (pCeeGlobalCreatePriorityGroupMessage->getPgId() == AUTO_NAS_PGID_VALUE) {
				status = WRC_AUTO_NAS_PGID_3_CANNOT_BE_CHANGED;
			}
		}
        pPrismLinearSequencerContext->executeNextStep(status);
        return;
	}

    void  CeeGlobalWorker::CeeGlobalCreatePriorityGroupMessageHandler( CeeGlobalCreatePriorityGroupMessage *pCeeGlobalCreatePriorityGroupMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createPriorityGroupValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createPriorityGroupStep),

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createPriorityGroupMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCeeGlobalCreatePriorityGroupMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }
	void CeeGlobalWorker::createPriorityMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {    
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 pgid3Count = 0, i = 0;
		string cosValue[8];

		CeeGlobalCreatePriorityMapMessage *pCeeGlobalCreatePriorityMapMessage =
		  dynamic_cast<CeeGlobalCreatePriorityMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		cosValue[0] = pCeeGlobalCreatePriorityMapMessage->getCos0ToPriorityGrouMap();
		cosValue[1] = pCeeGlobalCreatePriorityMapMessage->getCos1ToPriorityGrouMap();
		cosValue[2] = pCeeGlobalCreatePriorityMapMessage->getCos2ToPriorityGrouMap();
		cosValue[3] = pCeeGlobalCreatePriorityMapMessage->getCos3ToPriorityGrouMap();
		cosValue[4] = pCeeGlobalCreatePriorityMapMessage->getCos4ToPriorityGrouMap();
		cosValue[5] = pCeeGlobalCreatePriorityMapMessage->getCos5ToPriorityGrouMap();
		cosValue[6] = pCeeGlobalCreatePriorityMapMessage->getCos6ToPriorityGrouMap();
		cosValue[7] = pCeeGlobalCreatePriorityMapMessage->getCos7ToPriorityGrouMap();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NasConfigManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		NasConfigManagedObject *pNasConfigManagedObject = NULL;

		if (pResults != NULL) {
			if (pResults->size() > 0) {
            	pNasConfigManagedObject = dynamic_cast<NasConfigManagedObject*>((*pResults)[0]);
			} else {
            	trace(TRACE_LEVEL_DEBUG, string("createPriorityMapValidationStep:size=0 returning success"));
				delete pResults;
        		pPrismLinearSequencerContext->executeNextStep(status);
        		return;
			}
        }
        else {
            trace(TRACE_LEVEL_DEBUG, string("createPriorityMapValidationStep:presults= NULL. returning success"));
        	pPrismLinearSequencerContext->executeNextStep(status);
        	return;
        }
		UI32 nasCosValue = pNasConfigManagedObject->getCos();
		if (nasCosValue == DEFAULT_NAS_COS) {
			nasCosValue = DEFAULT_AUTO_NAS_COS; /* If user has not explicitly set nas cos value, its cos 2 by default */
		}

		for (i = 0 ; i < 8; i++ ) {
			if (cosValue[i] == AUTO_NAS_PGID_VALUE) {
				pgid3Count++;
			}
			if (i != 7) {
				if (cosValue[i].find("15.") != string::npos) {
					/* Donot allow assignment of strict-priorities to cos values when auto-nas is enabled.
						Only exception is 15.0 assigned to cos 7 */
					status = WRC_AUTO_NAS_COS_STRICT_PRIORITY_NOT_ALLOWED;
				}
			}
			if (nasCosValue  == i) {
				if (cosValue[i] != AUTO_NAS_PGID_VALUE) {
					/* NAS cos value cannot be mapped to a PGID other than 3 */
					status = WRC_AUTO_NAS_COS_CANNOT_CHANGE_PGID;
				}
			}
		}
		if (pgid3Count > 1) {
		  	/* Donot allow more than one cos to be mapped to PGID 3 when auto-nas is enabled */
			status = WRC_AUTO_NAS_COS_PGID3_MAP_EXCEEDED;
		}
		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
        pPrismLinearSequencerContext->executeNextStep(status);
        return;
	}

  void CeeGlobalWorker::createPriorityMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {    
    ResourceId status = WAVE_MESSAGE_SUCCESS;
	CeeGlobalCreatePriorityMapMessage *pCeeGlobalCreatePriorityMapMessage = dynamic_cast<CeeGlobalCreatePriorityMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

	/* VCS QoS Sanity Check	*/
	// looking up for CEE map
    CeeMapManagedObject *pCeeMapManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(pCeeGlobalCreatePriorityMapMessage->getCeeMapName(), "ceeMapName"));
    syncQueryCtxt.addSelectField("priorityGroup");
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (pResults != NULL) {
        if (pResults->size() > 0) {
            pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
        }
        else {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mapfailed");
            status = WRC_SSM_DCM_QOS_VCS_ERROR;
        }
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        pResults->clear();
        delete pResults;
        if (status != WAVE_MESSAGE_SUCCESS) {
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
            return;
        }
    }
    else {
        trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mapfailed");
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
		return;
    }

	int reservePriorityBitmap = 0;
	bool sanityNeed = false;
	// looking up for PGIDs refered in the new PrioirtyTable
	WaveManagedObjectPointer<PriorityGroupManagedObject> *pPriorityGroupMO = NULL;
	vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pgid_vector = pCeeMapManagedObject->getPriorityGroup();	
	pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos0ToPriorityGrouMap());
	if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos0ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
	}
	else {
		if ((*pPriorityGroupMO)->getPfc()) {
			reservePriorityBitmap |= 0x1;
			sanityNeed = true;
		}
	}

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos1ToPriorityGrouMap());
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos1ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x2;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos2ToPriorityGrouMap());
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos2ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x4;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos3ToPriorityGrouMap());
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos3ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x8;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos4ToPriorityGrouMap());
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos4ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x10;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos5ToPriorityGrouMap());
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos5ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x20;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos6ToPriorityGrouMap());
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos6ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x40;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pCeeGlobalCreatePriorityMapMessage->getCos7ToPriorityGrouMap());
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = ") + pCeeGlobalCreatePriorityMapMessage->getCos7ToPriorityGrouMap());
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x80;
            sanityNeed = true;
        }
    }

	if (sanityNeed) {

        //Validate whether the lossless cos conflicts with cos-mutated cos or default cos.
		status = QosUtils::vcsQosSanity(QOS_INTERFACE_CEE_MAP, pCeeGlobalCreatePriorityMapMessage->getCeeMapName(), false, 0, reservePriorityBitmap,0);
		if (status != WAVE_MESSAGE_SUCCESS) {
			pPrismLinearSequencerContext->executeNextStep(status);
			return;
		}

        //Validate whether the lossless cos conflicts with dscp-mutated TC in dscp-tc map or dscp-mutated cos.
        status = QosUtils::vcsL3QosSanity(QOS_INTERFACE_CEE_MAP, pCeeGlobalCreatePriorityMapMessage->getCeeMapName()
                    , false, reservePriorityBitmap, 0);

        if (status != WAVE_MESSAGE_SUCCESS) {
            pPrismLinearSequencerContext->executeNextStep(status);
            return;
        }
	}

    WaveSendToClusterContext    *send_ctx_p;

	trace(TRACE_LEVEL_DEBUG, "enter CeeGlobalWorker::createPriorityMapStep");

    QosCreatePriorityMapSPMessage *m = new QosCreatePriorityMapSPMessage();
    m->setCeeMapName(pCeeGlobalCreatePriorityMapMessage->getCeeMapName());    
	m->setCos0ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos0ToPriorityGrouMap());
    m->setCos1ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos1ToPriorityGrouMap());
    m->setCos2ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos2ToPriorityGrouMap());
    m->setCos3ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos3ToPriorityGrouMap());
    m->setCos4ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos4ToPriorityGrouMap());
    m->setCos5ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos5ToPriorityGrouMap());
    m->setCos6ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos6ToPriorityGrouMap());
    m->setCos7ToPriorityGrouMap(pCeeGlobalCreatePriorityMapMessage->getCos7ToPriorityGrouMap());

	send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
	prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
	send_ctx_p->setPPrismMessageForPhase1(m);
	trace(TRACE_LEVEL_DEBUG, "CeeGlobalWorker::createPriorityMapStep: Sending msg to Cluster");
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
	sendToWaveCluster(send_ctx_p);
	return ;
    }

    void CeeGlobalWorker::createPriorityMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    CeeGlobalCreatePriorityMapMessage *pCeeGlobalCreatePriorityMapMessage = dynamic_cast<CeeGlobalCreatePriorityMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	/* search for CEE map*/
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxtCee(CeeMapManagedObject::getClassName());
        syncQueryCtxtCee.addAndAttribute (new AttributeString("default", "ceeMapName"));
        syncQueryCtxtCee.addSelectField("priorityMap");
        vector<WaveManagedObject *> *pResultsCee = querySynchronously (&syncQueryCtxtCee);

        if (pResultsCee && pResultsCee->size() > 0) {
              pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResultsCee)[0]);
            }
        else {
            trace(TRACE_LEVEL_ERROR, string("createPTMOStep failed: searching for default CEE map failed "));
            if (pResultsCee) {
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsCee);
                pResultsCee->clear();
                delete pResultsCee;
            }
            pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
            return;
        }
        if (pResultsCee) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsCee);
            pResultsCee->clear();
            delete pResultsCee;
        }
	

    /* search for the priorityMap MO, if found, then update, otherwise error except the ConfigReplay case */
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PriorityMapManagedObject::getClassName());
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    PriorityMapManagedObject *pPriorityMapManagedObject = NULL;

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
			if (FrameworkToolKit::isConfigReplayInProgress()) {
				trace(TRACE_LEVEL_INFO, "createPTMOStep: create PriorityMap MO in ConfigReplay");
				pPriorityMapManagedObject = new PriorityMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()));
				pPriorityMapManagedObject->setCeeMapName("default");
				WaveManagedObjectPointer<PriorityMapManagedObject> pr_map_obj_ptr(pPriorityMapManagedObject);
				pCeeMapManagedObject->setPriorityMap(pr_map_obj_ptr);
                pCeeMapManagedObject->addAttributeToBeUpdated("priorityMap");
                updateWaveManagedObject(pCeeMapManagedObject);
				goto next;
			}
			else {
            	trace (TRACE_LEVEL_ERROR, "createPTMOStep: error, not found PriorityMap MO");
				goto end;
			}
		}
		else if (numberOfResults == 1) {
            trace (TRACE_LEVEL_DEBUG, "CeeGlobalWorker::createPriorityMapMOStep: modify");
	        pPriorityMapManagedObject = dynamic_cast<PriorityMapManagedObject*>((*pResults)[0]);
next:			/* Looking for PriorityGroup refered in the P-T map */
			/* Update PGID0 */
			string pgId0 = pCeeGlobalCreatePriorityMapMessage->getCos0ToPriorityGrouMap();
		    WaveManagedObjectSynchronousQueryContext syncQueryCtxt0(PriorityGroupManagedObject::getClassName());
		    syncQueryCtxt0.addAndAttribute (new AttributeString(pgId0,"pgId"));
            syncQueryCtxt0.addSelectField("objectId");
		    vector<WaveManagedObject *> *pResults0 = querySynchronously (&syncQueryCtxt0);
		    PriorityGroupManagedObject *pPriorityGroupManagedObject0 = NULL;
			if (NULL != pResults0) {
				if (pResults0->size () == 0) {
					trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID0");
					pResults0->clear();
					delete pResults0;
					goto end;
				}
				else if (pResults0->size () == 1) {
					pPriorityGroupManagedObject0 = dynamic_cast<PriorityGroupManagedObject*>((*pResults0)[0]);
					pPriorityMapManagedObject->setCos0ToPriorityGrouMap(pPriorityGroupManagedObject0->getObjectId());
					trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS0 to new PGID");
				}
				else {
					trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGID MO found");
                    pResults0->clear();
                    delete pResults0;
                    goto end;				
				}
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults0);
                pResults0->clear();
                delete pResults0;
			}
			else {
				trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID0");
				goto end;
			}

            /* Update PGID1 */
            string pgId1 = pCeeGlobalCreatePriorityMapMessage->getCos1ToPriorityGrouMap();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt1.addAndAttribute (new AttributeString(pgId1,"pgId"));
            syncQueryCtxt1.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults1 = querySynchronously (&syncQueryCtxt1);
            PriorityGroupManagedObject *pPriorityGroupManagedObject1 = NULL;
            if (NULL != pResults1) {
                if (pResults1->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID1");
                    pResults1->clear();
                    delete pResults1;
                    goto end;
                }
                else if (pResults1->size () == 1) {
                    pPriorityGroupManagedObject1 = dynamic_cast<PriorityGroupManagedObject*>((*pResults1)[0]);
                    pPriorityMapManagedObject->setCos1ToPriorityGrouMap(pPriorityGroupManagedObject1->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS1 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGIDMO found");
                    pResults1->clear();
                    delete pResults1;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults1);
                pResults1->clear();
                delete pResults1;
            }
            else {
				trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID1");
                goto end;
            }   
			
            /* Update PGID2 */
            string pgId2 = pCeeGlobalCreatePriorityMapMessage->getCos2ToPriorityGrouMap();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt2.addAndAttribute (new AttributeString(pgId2,"pgId"));
            syncQueryCtxt2.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults2 = querySynchronously (&syncQueryCtxt2);
            PriorityGroupManagedObject *pPriorityGroupManagedObject2 = NULL;
            if (NULL != pResults2) {
                if (pResults2->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID2");
                    pResults2->clear();
                    delete pResults2;
                    goto end;
                }
                else if (pResults2->size () == 1) {
                    pPriorityGroupManagedObject2 = dynamic_cast<PriorityGroupManagedObject*>((*pResults2)[0]);
                    pPriorityMapManagedObject->setCos2ToPriorityGrouMap(pPriorityGroupManagedObject2->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS2 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGIDMO found");
                    pResults2->clear();
                    delete pResults2;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults2);
                pResults2->clear();
                delete pResults2;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID2");
                goto end;
            }

            /* Update PGID3 */
            string pgId3 = pCeeGlobalCreatePriorityMapMessage->getCos3ToPriorityGrouMap();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt3(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt3.addAndAttribute (new AttributeString(pgId3,"pgId"));
            syncQueryCtxt3.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults3 = querySynchronously (&syncQueryCtxt3);
            PriorityGroupManagedObject *pPriorityGroupManagedObject3 = NULL;
            if (NULL != pResults3) {
                if (pResults3->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID3");
                    pResults3->clear();
                    delete pResults3;
                    goto end;
                }
                else if (pResults3->size () == 1) {
                    pPriorityGroupManagedObject3 = dynamic_cast<PriorityGroupManagedObject*>((*pResults3)[0]);
                    pPriorityMapManagedObject->setCos3ToPriorityGrouMap(pPriorityGroupManagedObject3->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS3 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGIDMO found");
                    pResults3->clear();
                    delete pResults3;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults3);
                pResults3->clear();
                delete pResults3;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID3");
                goto end;
            }

            /* Update PGID4 */
            string pgId4 = pCeeGlobalCreatePriorityMapMessage->getCos4ToPriorityGrouMap();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt4(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt4.addAndAttribute (new AttributeString(pgId4,"pgId"));
            syncQueryCtxt4.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults4 = querySynchronously (&syncQueryCtxt4);
            PriorityGroupManagedObject *pPriorityGroupManagedObject4 = NULL;
            if (NULL != pResults4) {
                if (pResults4->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID4");
                    pResults4->clear();
                    delete pResults4;
                    goto end;
                }
                else if (pResults4->size () == 1) {
                    pPriorityGroupManagedObject4 = dynamic_cast<PriorityGroupManagedObject*>((*pResults4)[0]);
                    pPriorityMapManagedObject->setCos4ToPriorityGrouMap(pPriorityGroupManagedObject4->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS4 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGIDMO found");
                    pResults4->clear();
                    delete pResults4;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults4);
                pResults4->clear();
                delete pResults4;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID4");
                goto end;
            }

            /* Update PGID5 */
            string pgId5 = pCeeGlobalCreatePriorityMapMessage->getCos5ToPriorityGrouMap();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt5(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt5.addAndAttribute (new AttributeString(pgId5,"pgId"));
            syncQueryCtxt5.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults5 = querySynchronously (&syncQueryCtxt5);
            PriorityGroupManagedObject *pPriorityGroupManagedObject5 = NULL;
            if (NULL != pResults5) {
                if (pResults5->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID5");
                    pResults5->clear();
                    delete pResults5;
                    goto end;
                }
                else if (pResults5->size () == 1) {
                    pPriorityGroupManagedObject5 = dynamic_cast<PriorityGroupManagedObject*>((*pResults5)[0]);
                    pPriorityMapManagedObject->setCos5ToPriorityGrouMap(pPriorityGroupManagedObject5->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS5 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGIDMO found");
                    pResults5->clear();
                    delete pResults5;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults5);
                pResults5->clear();
                delete pResults5;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID5");
                goto end;
            }

            /* Update PGID6 */
            string pgId6 = pCeeGlobalCreatePriorityMapMessage->getCos6ToPriorityGrouMap();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt6(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt6.addAndAttribute (new AttributeString(pgId6,"pgId"));
            syncQueryCtxt6.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults6 = querySynchronously (&syncQueryCtxt6);
            PriorityGroupManagedObject *pPriorityGroupManagedObject6 = NULL;
            if (NULL != pResults6) {
                if (pResults6->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID6");
                    pResults6->clear();
                    delete pResults6;
                    goto end;
                }
                else if (pResults6->size () == 1) {
                    pPriorityGroupManagedObject6 = dynamic_cast<PriorityGroupManagedObject*>((*pResults6)[0]);
                    pPriorityMapManagedObject->setCos6ToPriorityGrouMap(pPriorityGroupManagedObject6->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS6 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGIDMO found");
                    pResults6->clear();
                    delete pResults6;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults6);
                pResults6->clear();
                delete pResults6;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID6");
                goto end;
            }

            /* Update PGID7 */
            string pgId7 = pCeeGlobalCreatePriorityMapMessage->getCos7ToPriorityGrouMap();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt7(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt7.addAndAttribute (new AttributeString(pgId7,"pgId"));
            syncQueryCtxt7.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults7 = querySynchronously (&syncQueryCtxt7);
            PriorityGroupManagedObject *pPriorityGroupManagedObject7 = NULL;
            if (NULL != pResults7) {
                if (pResults7->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID7");
                    pResults7->clear();
                    delete pResults7;
                    goto end;
                }
                else if (pResults7->size () == 1) {
                    pPriorityGroupManagedObject7 = dynamic_cast<PriorityGroupManagedObject*>((*pResults7)[0]);
                    pPriorityMapManagedObject->setCos7ToPriorityGrouMap(pPriorityGroupManagedObject7->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "createPTMOStep: refer CoS7 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one PGIDMO found");
                    pResults7->clear();
                    delete pResults7;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults7);
                pResults7->clear();
                delete pResults7;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "createPTMOStep: cannot find refered PGID7");
                goto end;
            }
			updateWaveManagedObject(pPriorityMapManagedObject);
		}
		else {
			trace(TRACE_LEVEL_ERROR, "createPTMOStep: More than one P-T MO found");
		}
    
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        pResults->clear ();
        delete pResults;
	}

    if (pPriorityMapManagedObject == NULL) {
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
		return;
	}
	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	return;

end:
    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
    pResults->clear ();
    delete pResults;
	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
    }

    void  CeeGlobalWorker::CeeGlobalCreatePriorityMapMessageHandler( CeeGlobalCreatePriorityMapMessage *pCeeGlobalCreatePriorityMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createPriorityMapValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createPriorityMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::createPriorityMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCeeGlobalCreatePriorityMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

  void CeeGlobalWorker::deleteCeeMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    CeeGlobalDeleteCeeMapMessage *pCeeGlobalDeleteCeeMapMessage = dynamic_cast<CeeGlobalDeleteCeeMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveSendToClusterContext    *send_ctx_p;

    QosDeleteCeeMapSPMessage *m = new QosDeleteCeeMapSPMessage();

    m->setCeeMapName(pCeeGlobalDeleteCeeMapMessage->getCeeMapName());

    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("CeeGlobalWorker::deleteCeeMapStep: Sending msg to Cluster :"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
    }

    void CeeGlobalWorker::deleteCeeMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
//    CeeGlobalDeleteCeeMapMessage *pCeeGlobalDeleteCeeMapMessage = dynamic_cast<CeeGlobalDeleteCeeMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

	/* no remove, instead, restore it; Will do remove with backend changed for eAnvil-B */
	

//    CeeMapManagedObject *pCeeMapManagedObject = new CeeMapManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));

//    pCeeMapManagedObject->setCeeMapName(pCeeGlobalCreateCeeMapMessage->getCeeMapName());

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }

    void  CeeGlobalWorker::CeeGlobalDeleteCeeMapMessageHandler( CeeGlobalDeleteCeeMapMessage *pCeeGlobalDeleteCeeMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deleteCeeMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deleteCeeMapMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCeeGlobalDeleteCeeMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

  void CeeGlobalWorker::deletePriorityGroupValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
	  	ResourceId status = WAVE_MESSAGE_SUCCESS;
		CeeGlobalDeletePriorityGroupMessage *pCeeGlobalDeletePriorityGroupMessage =
		  dynamic_cast<CeeGlobalDeletePriorityGroupMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		string pgidValue = pCeeGlobalDeletePriorityGroupMessage->getPgId();

		if (WAVE_MESSAGE_SUCCESS == QosUtils::isAutoNasEnabled()) {
			if (pgidValue == "2" || pgidValue == "3") {
				status = WRC_AUTO_NAS_PGID_DELETION_NOT_ALLOWED;
			}
		}
        pPrismLinearSequencerContext->executeNextStep(status);
        return;
	}

  void CeeGlobalWorker::deletePriorityGroupStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    CeeGlobalDeletePriorityGroupMessage *pCeeGlobalDeletePriorityGroupMessage = dynamic_cast<CeeGlobalDeletePriorityGroupMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    WaveSendToClusterContext    *send_ctx_p;

    QosDeletePriorityGroupSPMessage *m = new QosDeletePriorityGroupSPMessage();

    m->setCeeMapName(pCeeGlobalDeletePriorityGroupMessage->getCeeMapName());
    m->setPgId(pCeeGlobalDeletePriorityGroupMessage->getPgId());

    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("CeeGlobalWorker::deletePriorityGroupStep: Sending msg to Cluster :"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
    }

    void CeeGlobalWorker::deletePriorityGroupMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {

    CeeGlobalDeletePriorityGroupMessage *pCeeGlobalDeletePriorityGroupMessage = dynamic_cast<CeeGlobalDeletePriorityGroupMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    /* search for the CEE map MO */
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(CeeMapManagedObject::getClassName());
    syncQueryCtxt1.addAndAttribute (new AttributeString(pCeeGlobalDeletePriorityGroupMessage->getCeeMapName(),"ceeMapName"));
    syncQueryCtxt1.addSelectField("priorityGroup");
    vector<WaveManagedObject *> *pResults1 = querySynchronously (&syncQueryCtxt1);
    CeeMapManagedObject *pCeeMapManagedObject = NULL;

    if (NULL != pResults1) {
        UI32 numberOfResults = pResults1->size ();
        if (0 == numberOfResults) {
            trace (TRACE_LEVEL_ERROR, "deletePGIDMOStep: no CEE map found");
        }
        else if (1 == numberOfResults) {
            pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults1)[0]);
        }
        else {
            trace (TRACE_LEVEL_ERROR, "deletePGIDMOStep: more than one CEE map found");
        }
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults1);
        pResults1->clear ();
        delete pResults1;
    }
    else {
        trace (TRACE_LEVEL_ERROR, "deletePGIDMOStep: no CEE map found");
    }

    if (pCeeMapManagedObject == NULL) {
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
        return;
    }

	string pgId = pCeeGlobalDeletePriorityGroupMessage->getPgId();

	if(pgId.length() > 2) {
	    vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pgid_vector = pCeeMapManagedObject->getPriorityGroup();
	    WaveManagedObjectPointer<PriorityGroupManagedObject> *pPriorityGroupMO = NULL;
		/* for strict-priority PGID, "no command" equals to pfc off */	
		pPriorityGroupMO = findPriorityGroupByName(pgid_vector, pgId);
		if (pPriorityGroupMO == NULL) {
			trace (TRACE_LEVEL_ERROR, string("deletePGIDMOStep: failed searching for PGID") + pgId);
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
		}
		else {
			trace(TRACE_LEVEL_INFO, string("deletePGIDMOStep: disable pfc for PGID ") + pgId);
			(*pPriorityGroupMO)->setPfc(0);
			PriorityGroupManagedObject *pgMo = (*pPriorityGroupMO).operator ->();
			if (pgMo != NULL)
				updateWaveManagedObject(pgMo);
		}
	}
	else {
		/* for DWRR PGID, remove the PGID MO */
        pCeeMapManagedObject->delPriorityGroup(pgId);
        pCeeMapManagedObject->addAttributeToBeUpdated("priorityGroup");
        updateWaveManagedObject(pCeeMapManagedObject);
	}

#if 0
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PriorityGroupManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(pgId,"pgId"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;

    if (NULL != pResults) {
	    UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
        	trace (TRACE_LEVEL_ERROR, "deletePGIDMOStep: MO not found");
    	} else if (1 == numberOfResults) {
            pPriorityGroupManagedObject = dynamic_cast<PriorityGroupManagedObject*>((*pResults)[0]);
			/* for strict-priority PGID, "no command" equals to pfc off */
			if (pgId.length() > 2) {
				pPriorityGroupManagedObject->setPfc(0);
				updateWaveManagedObject(pPriorityGroupManagedObject);
                pResults->clear ();
                delete pResults;
				pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
				return;
			}
			else
			{
			/* look up the PriorityTable, check if the to-be-deleted PGID is refered */
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt0(PriorityMapManagedObject::getClassName());
			vector<WaveManagedObject *> *pResults0 = querySynchronously (&syncQueryCtxt0);
			PriorityMapManagedObject *m = NULL;
			if (NULL != pResults0) {
				if (0 == pResults0->size ()) {
					trace(TRACE_LEVEL_ERROR, "deletePGIDMOStep: P-T no found");
					pResults->clear ();
					delete pResults;
					pResults0->clear ();
					delete pResults0;
					pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
					return;
				}
				else if (1 == pResults0->size ()) {
					m = dynamic_cast<PriorityMapManagedObject*>((*pResults0)[0]);
					ObjectId PGID = pPriorityGroupManagedObject->getObjectId();
					if (m->getCos0ToPriorityGrouMap() == PGID || 
						m->getCos1ToPriorityGrouMap() == PGID ||
						m->getCos2ToPriorityGrouMap() == PGID ||
						m->getCos3ToPriorityGrouMap() == PGID ||
						m->getCos4ToPriorityGrouMap() == PGID ||
						m->getCos5ToPriorityGrouMap() == PGID ||
						m->getCos6ToPriorityGrouMap() == PGID ||
						m->getCos7ToPriorityGrouMap() == PGID) {
						trace(TRACE_LEVEL_ERROR, "deletePGIDMOStep: PGID is still activated");
	                    pResults->clear ();
	                    delete pResults;
    	                pResults0->clear ();
        	            delete pResults0;
	                    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            	        return;
					}
				}
				else {
                    trace(TRACE_LEVEL_ERROR, "deletePGIDMOStep: one more P-T found");
                    pResults->clear ();
                    delete pResults;
                    pResults0->clear ();
                    delete pResults0;
                    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
                    return;
				}
			}
			else {
                trace(TRACE_LEVEL_ERROR, "deletePGIDMOStep: P-T no found");
	            pResults->clear ();
                delete pResults;
                pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
                return;
			}
			}
    	} else {
	        trace (TRACE_LEVEL_ERROR, "deletePGIDMOStep: More than one PGID MO found");
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
    	}
	    pResults->clear ();
    	delete pResults;
  	}

	if (pPriorityGroupManagedObject == NULL) {
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
	    return;
  	} else {
    	prismAssert (NULL != pPriorityGroupManagedObject, __FILE__, __LINE__);
	    trace (TRACE_LEVEL_INFO, string ("Delete PGID:: ") + pPriorityGroupManagedObject->getPgId());
		// remove the PGID from CEE map prioity-group-table vector
		vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pr_grp_vector = pCeeMapManagedObject->getPriorityGroup();
		UI32 itr;
		for (itr = 0; itr < pr_grp_vector.size(); itr++) {
			WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp_obj_ptr = pr_grp_vector[itr];
			if (pr_grp_obj_ptr->getPgId() == pgId)
				break;
		}
		if (itr < pr_grp_vector.size()) {
			pr_grp_vector.erase(pr_grp_vector.begin() + itr);
			pCeeMapManagedObject->setPriorityGroup(pr_grp_vector);
			updateWaveManagedObject(pCeeMapManagedObject);			
		}
	    delete pPriorityGroupManagedObject;

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		return;
	}
#endif

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }

    void  CeeGlobalWorker::CeeGlobalDeletePriorityGroupMessageHandler( CeeGlobalDeletePriorityGroupMessage *pCeeGlobalDeletePriorityGroupMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deletePriorityGroupValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deletePriorityGroupStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deletePriorityGroupMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCeeGlobalDeletePriorityGroupMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

	void CeeGlobalWorker::deletePriorityMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (WAVE_MESSAGE_SUCCESS == QosUtils::isAutoNasEnabled()) {
			status = WRC_AUTO_NAS_PRIORITY_MAP_DELITION_NOT_ALLOWED;
		}
        pPrismLinearSequencerContext->executeNextStep(status);
        return;
	}
  void CeeGlobalWorker::deletePriorityMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    CeeGlobalDeletePriorityMapMessage *pCeeGlobalDeletePriorityMapMessage = dynamic_cast<CeeGlobalDeletePriorityMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

	/* VCS Qos Sanity check */
	// looking up for CEE map
    CeeMapManagedObject *pCeeMapManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString(pCeeGlobalDeletePriorityMapMessage->getCeeMapName(), "ceeMapName"));
    syncQueryCtxt.addSelectField("priorityGroup");
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (pResults && pResults->size() > 0) {
          pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
        }
    else {
        trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mapfailed");
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    if (pResults) {
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        pResults->clear();
        delete pResults;
    }

    int reservePriorityBitmap = 0;
    bool sanityNeed = false;
    // looking up for PGIDs refered in the new PrioirtyTable
    WaveManagedObjectPointer<PriorityGroupManagedObject> *pPriorityGroupMO = NULL;
    vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pgid_vector = pCeeMapManagedObject->getPriorityGroup();
    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.6");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.6"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x1;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.7");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.7"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x2;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.5");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.5"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x4;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.4");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.4"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x8;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.3");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.3"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x10;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.2");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.2"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x20;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.1");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.1"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x40;
            sanityNeed = true;
        }
    }

    pPriorityGroupMO = findPriorityGroupByName(pgid_vector, "15.0");
    if (pPriorityGroupMO == NULL) {
        trace(TRACE_LEVEL_ERROR, string("vcsQosSanity failed: searching for PriorityGroup mapfailed, PGID = 15.0"));
        pPrismLinearSequencerContext->executeNextStep(WRC_SSM_DCM_QOS_VCS_ERROR);
        return;
    }
    else {
        if ((*pPriorityGroupMO)->getPfc()) {
            reservePriorityBitmap |= 0x80;
            sanityNeed = true;
        }
    }

    if (sanityNeed) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        //Validate whether the lossless cos conflicts with cos-mutated cos or default cos.
        status = QosUtils::vcsQosSanity(QOS_INTERFACE_CEE_MAP, pCeeGlobalDeletePriorityMapMessage->getCeeMapName(), false, 0, reservePriorityBitmap, 0);
        if (status != WAVE_MESSAGE_SUCCESS) {
            pPrismLinearSequencerContext->executeNextStep(status);
            return;
        }

        //Validate whether the lossless cos conflicts with dscp-mutated TC in dscp-tc map or dscp-mutated cos.
        status = QosUtils::vcsL3QosSanity(QOS_INTERFACE_CEE_MAP, pCeeGlobalDeletePriorityMapMessage->getCeeMapName()
                        , false, reservePriorityBitmap, 0);
        if (status != WAVE_MESSAGE_SUCCESS) {
            pPrismLinearSequencerContext->executeNextStep(status);
            return;
        }
    }

    WaveSendToClusterContext    *send_ctx_p;

    QosDeletePriorityMapSPMessage *m = new QosDeletePriorityMapSPMessage();
    m->setCeeMapName(pCeeGlobalDeletePriorityMapMessage->getCeeMapName());

    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("CeeGlobalWorker::deletePriorityMapSte: Sending msg to Cluster :"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return;
    }

    void CeeGlobalWorker::deletePriorityMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    //CeeGlobalDeletePriorityMapMessage *pCeeGlobalDeletePriorityMapMessage = dynamic_cast<CeeGlobalDeletePriorityMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    /* search for the prioritygroup MO, then update it the default value */
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PriorityMapManagedObject::getClassName());
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    PriorityMapManagedObject *pPriorityMapManagedObject = NULL;

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {
            trace (TRACE_LEVEL_ERROR, "deletePTMOStep: error, not found P-T MO");
        } else if (1 == numberOfResults) {
            trace (TRACE_LEVEL_DEBUG, "deletePTMOStep: modify");
            pPriorityMapManagedObject = dynamic_cast<PriorityMapManagedObject*>((*pResults)[0]);
            /* Looking for PriorityGroup refered in the P-T map */
            /* Update PGID0 */
            string pgId0 = "15.6";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt0(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt0.addAndAttribute (new AttributeString(pgId0,"pgId"));
            syncQueryCtxt0.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults0 = querySynchronously (&syncQueryCtxt0);
            PriorityGroupManagedObject *pPriorityGroupManagedObject0 = NULL;
            if (NULL != pResults0) {
                if (pResults0->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.6");
                    pResults0->clear();
                    delete pResults0;
                    goto end;
                }
                else if (pResults0->size () == 1) {
                    pPriorityGroupManagedObject0 = dynamic_cast<PriorityGroupManagedObject*>((*pResults0)[0]);
                    pPriorityMapManagedObject->setCos0ToPriorityGrouMap(pPriorityGroupManagedObject0->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS0 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGID MO found");
                    pResults0->clear();
                    delete pResults0;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults0);
                pResults0->clear();
                delete pResults0;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.6");
                goto end;
            }

            /* Update PGID1 */
            string pgId1 = "15.7";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt1.addAndAttribute (new AttributeString(pgId1,"pgId"));
            syncQueryCtxt1.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults1 = querySynchronously (&syncQueryCtxt1);
            PriorityGroupManagedObject *pPriorityGroupManagedObject1 = NULL;
            if (NULL != pResults1) {
                if (pResults1->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.7");
                    pResults1->clear();
                    delete pResults1;
                    goto end;
                }
                else if (pResults1->size () == 1) {
                    pPriorityGroupManagedObject1 = dynamic_cast<PriorityGroupManagedObject*>((*pResults1)[0]);
                    pPriorityMapManagedObject->setCos1ToPriorityGrouMap(pPriorityGroupManagedObject1->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS1 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGIDMO found");
                    pResults1->clear();
                    delete pResults1;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults1);
                pResults1->clear();
                delete pResults1;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.7");
                goto end;
            }

            /* Update PGID2 */
            string pgId2 = "15.5";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt2.addAndAttribute (new AttributeString(pgId2,"pgId"));
            syncQueryCtxt2.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults2 = querySynchronously (&syncQueryCtxt2);
            PriorityGroupManagedObject *pPriorityGroupManagedObject2 = NULL;
            if (NULL != pResults2) {
                if (pResults2->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.5");
                    pResults2->clear();
                    delete pResults2;
                    goto end;
                }
                else if (pResults2->size () == 1) {
                    pPriorityGroupManagedObject2 = dynamic_cast<PriorityGroupManagedObject*>((*pResults2)[0]);
                    pPriorityMapManagedObject->setCos2ToPriorityGrouMap(pPriorityGroupManagedObject2->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS2 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGIDMO found");
                    pResults2->clear();
                    delete pResults2;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults2);
                pResults2->clear();
                delete pResults2;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.5");
                goto end;
            }

            /* Update PGID3 */
            string pgId3 = "15.4";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt3(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt3.addAndAttribute (new AttributeString(pgId3,"pgId"));
            syncQueryCtxt3.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults3 = querySynchronously (&syncQueryCtxt3);
            PriorityGroupManagedObject *pPriorityGroupManagedObject3 = NULL;
            if (NULL != pResults3) {
                if (pResults3->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.4");
                    pResults3->clear();
                    delete pResults3;
                    goto end;
                }
                else if (pResults3->size () == 1) {
                    pPriorityGroupManagedObject3 = dynamic_cast<PriorityGroupManagedObject*>((*pResults3)[0]);
                    pPriorityMapManagedObject->setCos3ToPriorityGrouMap(pPriorityGroupManagedObject3->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS3 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGIDMO found");
                    pResults3->clear();
                    delete pResults3;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults3);
                pResults3->clear();
                delete pResults3;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.4");
                goto end;
            }

            /* Update PGID4 */
            string pgId4 = "15.3";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt4(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt4.addAndAttribute (new AttributeString(pgId4,"pgId"));
            syncQueryCtxt4.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults4 = querySynchronously (&syncQueryCtxt4);
            PriorityGroupManagedObject *pPriorityGroupManagedObject4 = NULL;
            if (NULL != pResults4) {
                if (pResults4->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.3");
                    pResults4->clear();
                    delete pResults4;
                    goto end;
                }
                else if (pResults4->size () == 1) {
                    pPriorityGroupManagedObject4 = dynamic_cast<PriorityGroupManagedObject*>((*pResults4)[0]);
                    pPriorityMapManagedObject->setCos4ToPriorityGrouMap(pPriorityGroupManagedObject4->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS4 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGIDMO found");
                    pResults4->clear();
                    delete pResults4;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults4);
                pResults4->clear();
                delete pResults4;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.3");
                goto end;
            }

            /* Update PGID5 */
            string pgId5 = "15.2";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt5(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt5.addAndAttribute (new AttributeString(pgId5,"pgId"));
            syncQueryCtxt5.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults5 = querySynchronously (&syncQueryCtxt5);
            PriorityGroupManagedObject *pPriorityGroupManagedObject5 = NULL;
            if (NULL != pResults5) {
                if (pResults5->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.2");
                    pResults5->clear();
                    delete pResults5;
                    goto end;
                }
                else if (pResults5->size () == 1) {
                    pPriorityGroupManagedObject5 = dynamic_cast<PriorityGroupManagedObject*>((*pResults5)[0]);
                    pPriorityMapManagedObject->setCos5ToPriorityGrouMap(pPriorityGroupManagedObject5->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS5 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGIDMO found");
                    pResults5->clear();
                    delete pResults5;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults5);
                pResults5->clear();
                delete pResults5;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.2");
                goto end;
            }

            /* Update PGID6 */
            string pgId6 = "15.1";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt6(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt6.addAndAttribute (new AttributeString(pgId6,"pgId"));
            syncQueryCtxt6.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults6 = querySynchronously (&syncQueryCtxt6);
            PriorityGroupManagedObject *pPriorityGroupManagedObject6 = NULL;
            if (NULL != pResults6) {
                if (pResults6->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.1");
                    pResults6->clear();
                    delete pResults6;
                    goto end;
                }
                else if (pResults6->size () == 1) {
                    pPriorityGroupManagedObject6 = dynamic_cast<PriorityGroupManagedObject*>((*pResults6)[0]);
                    pPriorityMapManagedObject->setCos6ToPriorityGrouMap(pPriorityGroupManagedObject6->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS6 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGIDMO found");
                    pResults6->clear();
                    delete pResults6;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults6);
                pResults6->clear();
                delete pResults6;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.1");
                goto end;
            }

            /* Update PGID7 */
            string pgId7 = "15.0";
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt7(PriorityGroupManagedObject::getClassName());
            syncQueryCtxt7.addAndAttribute (new AttributeString(pgId7,"pgId"));
            syncQueryCtxt7.addSelectField("objectId");
            vector<WaveManagedObject *> *pResults7 = querySynchronously (&syncQueryCtxt7);
            PriorityGroupManagedObject *pPriorityGroupManagedObject7 = NULL;
            if (NULL != pResults7) {
                if (pResults7->size () == 0) {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.0");
                    pResults7->clear();
                    delete pResults7;
                    goto end;
                }
                else if (pResults7->size () == 1) {
                    pPriorityGroupManagedObject7 = dynamic_cast<PriorityGroupManagedObject*>((*pResults7)[0]);
                    pPriorityMapManagedObject->setCos7ToPriorityGrouMap(pPriorityGroupManagedObject7->getObjectId());
                    trace(TRACE_LEVEL_DEBUG, "deletePTMOStep: refer CoS7 to new PGID");
                }
                else {
                    trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one PGIDMO found");
                    pResults7->clear();
                    delete pResults7;
                    goto end;
                }
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults7);
                pResults7->clear();
                delete pResults7;
            }
            else {
                trace(TRACE_LEVEL_ERROR, "deletePTMOStep: cannot find refered PGID 15.0");
                goto end;
            }
			updateWaveManagedObject(pPriorityMapManagedObject);
        }
        else {
            trace(TRACE_LEVEL_ERROR, "deletePTMOStep: More than one P-T MO found");
        }

        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        pResults->clear ();
        delete pResults;
    }

    if (pPriorityMapManagedObject == NULL) {
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
        return;
    }
	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	return;
end:
    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
    pResults->clear ();
    delete pResults;
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
    }

    void  CeeGlobalWorker::CeeGlobalDeletePriorityMapMessageHandler( CeeGlobalDeletePriorityMapMessage *pCeeGlobalDeletePriorityMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deletePriorityMapValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deletePriorityMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::deletePriorityMapMOStep),

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCeeGlobalDeletePriorityMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

void CeeGlobalWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
	CeeMapManagedObject *cee_map_mo_p;

	startTransaction();

	trace(TRACE_LEVEL_INFO, "create empty CEE map");
	vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pr_grp_vector;
	WaveManagedObjectPointer<PriorityMapManagedObject> pr_map_obj_ptr;
    cee_map_mo_p = new CeeMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),
                                           "default", 1, pr_grp_vector, pr_map_obj_ptr);

    cee_map_mo_p->setRemapFabricPriority (0);
    cee_map_mo_p->setRemapLosslessPriority (0);
    status = commitTransaction ();
    if (status == FRAMEWORK_SUCCESS) {
        status = WAVE_MESSAGE_SUCCESS;
    } else {
        trace(TRACE_LEVEL_ERROR, "Failed to create empty CEE Map");
    }

    delete cee_map_mo_p;

#if 0
    CeeMapManagedObject *cee_map_mo_p;
    PriorityGroupManagedObject *pr_grp1_mo_p;
    PriorityGroupManagedObject *pr_grp2_mo_p;
    PriorityGroupManagedObject *pr_grp150_mo_p;
    PriorityGroupManagedObject *pr_grp151_mo_p;
    PriorityGroupManagedObject *pr_grp152_mo_p;
    PriorityGroupManagedObject *pr_grp153_mo_p;
    PriorityGroupManagedObject *pr_grp154_mo_p;
    PriorityGroupManagedObject *pr_grp155_mo_p;
    PriorityGroupManagedObject *pr_grp156_mo_p;
    PriorityGroupManagedObject *pr_grp157_mo_p;

    PriorityMapManagedObject *pr_map_mo_p;

/*
    if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT) && (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
        trace(TRACE_LEVEL_INFO, "Not a FIRST_TIME_BOOT, Returning");
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
        return;
    }
*/        

    startTransaction();

    trace(TRACE_LEVEL_INFO, "Creating 'default CEE Map");

    pr_grp1_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),
                                                      "1", 40, 1, "default");

    pr_grp2_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),
                                                  "2", 60, 0, "default");

	//when cee map created, PGID 15.0-15.7 created automatically
    pr_grp150_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.0", 0, 0, "default");
    pr_grp151_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager*> (getPWaveObjectManager()), "15.1", 0, 0, "default");
    pr_grp152_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager*> (getPWaveObjectManager()), "15.2", 0, 0, "default");
    pr_grp153_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager*> (getPWaveObjectManager()), "15.3", 0, 0, "default");
    pr_grp154_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager*> (getPWaveObjectManager()), "15.4", 0, 0, "default");
    pr_grp155_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager*> (getPWaveObjectManager()), "15.5", 0, 0, "default");
    pr_grp156_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager*> (getPWaveObjectManager()), "15.6", 0, 0, "default");
    pr_grp157_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager*> (getPWaveObjectManager()), "15.7", 0, 0, "default");


    vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pr_grp_vector;

    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp1_obj_ptr(pr_grp1_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp2_obj_ptr(pr_grp2_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp150_obj_ptr(pr_grp150_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp151_obj_ptr(pr_grp151_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp152_obj_ptr(pr_grp152_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp153_obj_ptr(pr_grp153_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp154_obj_ptr(pr_grp154_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp155_obj_ptr(pr_grp155_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp156_obj_ptr(pr_grp156_mo_p);
    WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp157_obj_ptr(pr_grp157_mo_p);

    pr_grp_vector.push_back(pr_grp1_obj_ptr);
    pr_grp_vector.push_back(pr_grp2_obj_ptr);
    pr_grp_vector.push_back(pr_grp150_obj_ptr);
    pr_grp_vector.push_back(pr_grp151_obj_ptr);
    pr_grp_vector.push_back(pr_grp152_obj_ptr);
    pr_grp_vector.push_back(pr_grp153_obj_ptr);
    pr_grp_vector.push_back(pr_grp154_obj_ptr);
    pr_grp_vector.push_back(pr_grp155_obj_ptr);
    pr_grp_vector.push_back(pr_grp156_obj_ptr);
    pr_grp_vector.push_back(pr_grp157_obj_ptr);

    pr_map_mo_p = new PriorityMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),
                                               pr_grp2_mo_p->getObjectId(),
                                               pr_grp2_mo_p->getObjectId(),
                                               pr_grp2_mo_p->getObjectId(),
                                               pr_grp1_mo_p->getObjectId(),
                                               pr_grp2_mo_p->getObjectId(),
                                               pr_grp2_mo_p->getObjectId(),
                                               pr_grp2_mo_p->getObjectId(),
                                               pr_grp2_mo_p->getObjectId());
	pr_map_mo_p->setCeeMapName("default");

    WaveManagedObjectPointer<PriorityMapManagedObject> pr_map_obj_ptr(pr_map_mo_p);

    cee_map_mo_p = new CeeMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),
                                           "default", 1, pr_grp_vector, pr_map_obj_ptr);

	cee_map_mo_p->setRemapFabricPriority (0);
	cee_map_mo_p->setRemapLosslessPriority (0);
	cee_map_mo_p->setPrecedence(1);
    status = commitTransaction ();
    if (status == FRAMEWORK_SUCCESS) {
        status = WAVE_MESSAGE_SUCCESS;
    } else {
        trace(TRACE_LEVEL_ERROR, "Failed to create default CEE Map");
    }
   
    delete cee_map_mo_p;
#endif
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
    return;
}

    void CeeGlobalWorker::QosGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p) {
        
        trace (TRACE_LEVEL_DEBUG, "Enter CeeGlobalWorker::QosGlobalSendToClusterCallback : ");
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        PrismLinearSequencerContext             *seq_ctx_p;
        vector<LocationId>                      locationIdsInCluster;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        
        if ( WAVE_MESSAGE_SUCCESS == sendToClusterCompletionStatus ) {
            trace (TRACE_LEVEL_INFO, "CEE Global Command succeeded on all the nodes.");
        } else {
            trace (TRACE_LEVEL_INFO, "CEE Global Command failed on few or all the nodes.");
            locationIdsInCluster = ctx_p->getLocationsToSendToForPhase1();
            
            for (UI32 i = 0; i < locationIdsInCluster.size(); i++) {
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locationIdsInCluster[i]);
                
                if(WAVE_MESSAGE_SUCCESS != statusPerLocationId) {
                    trace(TRACE_LEVEL_ERROR, string("CEE config command failed on the node<")
                        + locationIdsInCluster[i] + ">, with an error: " + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        }
        
        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
    }

    WaveManagedObjectPointer<PriorityGroupManagedObject>* CeeGlobalWorker::findPriorityGroupByName
                (vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > &pgid_vector, string pgIdName) {
		UI32 i;

		for (i = 0; i < pgid_vector.size(); i++) {
			if (pgid_vector[i]->getPgId() == pgIdName)
				return (&(pgid_vector[i]));
		}
		return (NULL);
	}

	void CeeGlobalWorker::configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
	{
       	trace (TRACE_LEVEL_INFO, "CeeGlobalWorker::configReplayEnd : entering... ");
    
        // looking up for the default CEE map
        string ceeMapName = "default";
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(ceeMapName,"ceeMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
			if ((0 == numberOfResults) || (numberOfResults > 1)) {
				trace (TRACE_LEVEL_ERROR, "PostConfigReplay: 0 or more than one default CEE map");
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                pWaveAsynchronousContextForConfigReplayEnd->callback ();
                return;
			}
			CeeMapManagedObject *pCeeMapManagedObject = NULL;
			pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
			if (pCeeMapManagedObject == NULL) {
				trace (TRACE_LEVEL_ERROR, "PostConfigReplay: error locating default CEE map");
				delete(pResults);
                pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                pWaveAsynchronousContextForConfigReplayEnd->callback ();
                return;
			}
			if ((pCeeMapManagedObject->getPriorityGroup()).size() != 0) {
				trace(TRACE_LEVEL_INFO, "PostConfigReplay: CEE map already replayed, check 15.0-15.7");

				startTransaction();
				// check if PGID 15.0-15.7 has been added
				WaveManagedObjectPointer<PriorityGroupManagedObject>* pg = NULL;
				PriorityGroupManagedObject *pr_grp0_mo_p =NULL;
                PriorityGroupManagedObject *pr_grp1_mo_p =NULL;
                PriorityGroupManagedObject *pr_grp2_mo_p =NULL;
                PriorityGroupManagedObject *pr_grp3_mo_p =NULL;
                PriorityGroupManagedObject *pr_grp4_mo_p =NULL;
                PriorityGroupManagedObject *pr_grp5_mo_p =NULL;
                PriorityGroupManagedObject *pr_grp6_mo_p =NULL;
                PriorityGroupManagedObject *pr_grp7_mo_p =NULL;
				PriorityMapManagedObject *pr_map_mo_p = NULL;
#if 0
				bool d0 = false;
                bool d1 = false;
                bool d2 = false;
                bool d3 = false;
                bool d4 = false;
                bool d5 = false;
                bool d6 = false;
                bool d7 = false;
#endif
				pg = pCeeMapManagedObject->findPriorityGroupByName("15.0");
				if (pg == NULL) {
        		    pr_grp0_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.0", 0, false, ceeMapName);
					pCeeMapManagedObject->addPriorityGroup(pr_grp0_mo_p);
					//d0 = true;
				}
				else 
					pr_grp0_mo_p = pg->operator ->();

                pg = pCeeMapManagedObject->findPriorityGroupByName("15.1");
                if (pg == NULL) {
                    pr_grp1_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.1", 0, false, ceeMapName);
                    pCeeMapManagedObject->addPriorityGroup(pr_grp1_mo_p);
                    //d1 = true;
                }
				else
					pr_grp1_mo_p = pg->operator ->();
                pg = pCeeMapManagedObject->findPriorityGroupByName("15.2");
                if (pg == NULL) {
                    pr_grp2_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.2", 0, false, ceeMapName);
                    pCeeMapManagedObject->addPriorityGroup(pr_grp2_mo_p);
                    //d2 = true;
                }
				else
					pr_grp2_mo_p = pg->operator ->();
                pg = pCeeMapManagedObject->findPriorityGroupByName("15.3");
                if (pg == NULL) {
                    pr_grp3_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.3", 0, false, ceeMapName);
                    pCeeMapManagedObject->addPriorityGroup(pr_grp3_mo_p);
                    //d3 = true;
                }
                else
                    pr_grp3_mo_p = pg->operator ->();
                pg = pCeeMapManagedObject->findPriorityGroupByName("15.4");
                if (pg == NULL) {
                    pr_grp4_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.4", 0, false, ceeMapName);
                    pCeeMapManagedObject->addPriorityGroup(pr_grp4_mo_p);
                    //d4 = true;
                }
                else
                    pr_grp4_mo_p = pg->operator ->();
                pg = pCeeMapManagedObject->findPriorityGroupByName("15.5");
                if (pg == NULL) {
                    pr_grp5_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.5", 0, false, ceeMapName);
                    pCeeMapManagedObject->addPriorityGroup(pr_grp5_mo_p);
                    //d5 = true;
                }
                else
                    pr_grp5_mo_p = pg->operator ->();
                pg = pCeeMapManagedObject->findPriorityGroupByName("15.6");
                if (pg == NULL) {
                    pr_grp6_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.6", 0, false, ceeMapName);
                    pCeeMapManagedObject->addPriorityGroup(pr_grp6_mo_p);
                    //d6 = true;
                }
                else
                    pr_grp6_mo_p = pg->operator ->();
                pg = pCeeMapManagedObject->findPriorityGroupByName("15.7");
                if (pg == NULL) {
                    pr_grp7_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.7", 0, false, ceeMapName);
                    pCeeMapManagedObject->addPriorityGroup(pr_grp7_mo_p);
                    //d7 = true;
                }
                else
                    pr_grp7_mo_p = pg->operator ->();

				// check if PriorityTable has been added
				WaveManagedObjectPointer<PriorityMapManagedObject> pt = pCeeMapManagedObject->getPriorityMap();
				if (pt.operator ->() == NULL)  {
        		    pr_map_mo_p = new PriorityMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),
                                           pr_grp6_mo_p->getObjectId(),
                                           pr_grp7_mo_p->getObjectId(),
                                           pr_grp5_mo_p->getObjectId(),
                                           pr_grp4_mo_p->getObjectId(),
                                           pr_grp3_mo_p->getObjectId(),
                                           pr_grp2_mo_p->getObjectId(),
                                           pr_grp1_mo_p->getObjectId(),
                                           pr_grp0_mo_p->getObjectId());
		            pr_map_mo_p->setCeeMapName(ceeMapName);
		            WaveManagedObjectPointer<PriorityMapManagedObject> pr_map_obj_ptr(pr_map_mo_p);
		            pCeeMapManagedObject->setPriorityMap(pr_map_obj_ptr);
				}
				updateWaveManagedObject(pCeeMapManagedObject);
	            ResourceId status = WAVE_MESSAGE_SUCCESS;
    	        status = commitTransaction ();
        	    if (status == FRAMEWORK_SUCCESS) {
            	    status = WAVE_MESSAGE_SUCCESS;
	            } else {
    	            trace(TRACE_LEVEL_ERROR, "PostConfigReplay: Failed to update PGID 15.0-15.7 in default CEE Map");
        	    }
			
#if 0
				if (d0)
					delete pr_grp0_mo_p;
                if (d1)
                    delete pr_grp1_mo_p;
                if (d2)
                    delete pr_grp2_mo_p;
                if (d3)
                    delete pr_grp3_mo_p;
                if (d4)
                    delete pr_grp4_mo_p;
                if (d5)
                    delete pr_grp5_mo_p;
                if (d6)
                    delete pr_grp6_mo_p;
                if (d7)
                    delete pr_grp7_mo_p;
				if (pr_map_mo_p != NULL)
					delete pr_map_mo_p;
				delete pResults;
#endif
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                pWaveAsynchronousContextForConfigReplayEnd->callback ();
                return;
			}

			//otherwise, create the default CEE map
            startTransaction();

            trace (TRACE_LEVEL_INFO, "PostConfigReplay: create PGID 15.0-15.7");
            PriorityGroupManagedObject *pr_grp0_mo_p;
            pr_grp0_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.0", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp1_mo_p;
            pr_grp1_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.1", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp2_mo_p;
            pr_grp2_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.2", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp3_mo_p;
            pr_grp3_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.3", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp4_mo_p;
            pr_grp4_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.4", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp5_mo_p;
            pr_grp5_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.5", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp6_mo_p;
            pr_grp6_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.6", 0, false, ceeMapName);
            PriorityGroupManagedObject *pr_grp7_mo_p;
            pr_grp7_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "15.7", 0, false, ceeMapName);
            trace (TRACE_LEVEL_INFO, "PostConfigReplay: create PGID 1 and 2");
            PriorityGroupManagedObject *pr_grp_dwrr1_mo_p;
            pr_grp_dwrr1_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "1", 40, true, ceeMapName);
            PriorityGroupManagedObject *pr_grp_dwrr2_mo_p;
            pr_grp_dwrr2_mo_p = new PriorityGroupManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()), "2", 60, false, ceeMapName);


            vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pr_grp_vector;

            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp0_obj_ptr(pr_grp0_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp1_obj_ptr(pr_grp1_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp2_obj_ptr(pr_grp2_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp3_obj_ptr(pr_grp3_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp4_obj_ptr(pr_grp4_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp5_obj_ptr(pr_grp5_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp6_obj_ptr(pr_grp6_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp7_obj_ptr(pr_grp7_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp_dwrr1_obj_ptr(pr_grp_dwrr1_mo_p);
            WaveManagedObjectPointer<PriorityGroupManagedObject> pr_grp_dwrr2_obj_ptr(pr_grp_dwrr2_mo_p);

            pr_grp_vector.push_back(pr_grp0_obj_ptr);
            pr_grp_vector.push_back(pr_grp1_obj_ptr);
            pr_grp_vector.push_back(pr_grp2_obj_ptr);
            pr_grp_vector.push_back(pr_grp3_obj_ptr);
            pr_grp_vector.push_back(pr_grp4_obj_ptr);
            pr_grp_vector.push_back(pr_grp5_obj_ptr);
            pr_grp_vector.push_back(pr_grp6_obj_ptr);
            pr_grp_vector.push_back(pr_grp7_obj_ptr);
            pr_grp_vector.push_back(pr_grp_dwrr1_obj_ptr);
            pr_grp_vector.push_back(pr_grp_dwrr2_obj_ptr);

            trace (TRACE_LEVEL_INFO, "PostConfigReplay: create P-T table");
            PriorityMapManagedObject *pr_map_mo_p;
            pr_map_mo_p = new PriorityMapManagedObject(dynamic_cast <QosGlobalObjectManager *> (getPWaveObjectManager()),
                                           pr_grp_dwrr2_mo_p->getObjectId(),
                                           pr_grp_dwrr2_mo_p->getObjectId(),
                                           pr_grp_dwrr2_mo_p->getObjectId(),
                                           pr_grp_dwrr1_mo_p->getObjectId(),
                                           pr_grp_dwrr2_mo_p->getObjectId(),
                                           pr_grp_dwrr2_mo_p->getObjectId(),
                                           pr_grp_dwrr2_mo_p->getObjectId(),
                                           pr_grp0_mo_p->getObjectId());
            pr_map_mo_p->setCeeMapName(ceeMapName);
            WaveManagedObjectPointer<PriorityMapManagedObject> pr_map_obj_ptr(pr_map_mo_p);
            pCeeMapManagedObject->setPriorityGroup(pr_grp_vector);
			pCeeMapManagedObject->setPriorityMap(pr_map_obj_ptr);
            pCeeMapManagedObject->setRemapFabricPriority(0);
            pCeeMapManagedObject->setRemapLosslessPriority(0);

			updateWaveManagedObject(pCeeMapManagedObject);

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            status = commitTransaction ();
            if (status == FRAMEWORK_SUCCESS) {
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                trace(TRACE_LEVEL_ERROR, "PostConfigReplay: Failed to create default CEE Map");
            }

#if 0
            delete pr_grp0_mo_p;
            delete pr_grp1_mo_p;
            delete pr_grp2_mo_p;
            delete pr_grp3_mo_p;
            delete pr_grp4_mo_p;
            delete pr_grp5_mo_p;
            delete pr_grp6_mo_p;
            delete pr_grp7_mo_p;
            delete pr_grp_dwrr1_mo_p;
            delete pr_grp_dwrr2_mo_p;
            delete pr_map_mo_p;
            //delete pCeeMapManagedObject;
			delete pResults;
#endif
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			// send PGID1 to backend
#if 0
		    WaveSendToClusterContext    *send_ctx_p;
		    QosCreatePriorityGroupSPMessage *m = new QosCreatePriorityGroupSPMessage();

		    m->setCeeMapName(ceeMapName);
		    m->setPgId("1");
		    m->setWeight(40);
		    m->setPfc(1);

		    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback1), NULL);
		    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		    send_ctx_p->setPPrismMessageForPhase1(m);
            trace(TRACE_LEVEL_INFO, string("PostConfigReplay: Sending PiroirtyGroup 1 to Cluster :"));
            send_ctx_p->setPartialSuccessFlag(true);
            send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		    sendToWaveCluster(send_ctx_p);

            // send PGID2 to backend
			QosCreatePriorityGroupSPMessage *m2 = new QosCreatePriorityGroupSPMessage();
			m2->setCeeMapName(ceeMapName);
            m2->setPgId("2");
            m2->setWeight(60);
            m2->setPfc(0);

            send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback1), NULL);
            prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
            send_ctx_p->setPPrismMessageForPhase1(m2);
            trace(TRACE_LEVEL_INFO, string("PostConfigReplay: Sending PiroirtyGroup 2 to Cluster :"));
            send_ctx_p->setPartialSuccessFlag(true);
            send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
            sendToWaveCluster(send_ctx_p);

			// send PriorityTable to backend
		    QosCreatePriorityMapSPMessage *m1 = new QosCreatePriorityMapSPMessage(ceeMapName, "2", "2", "2", "1", "2", "2", "2", "15.0");
		    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback1), NULL);
		    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		    send_ctx_p->setPPrismMessageForPhase1(m1);
		    trace(TRACE_LEVEL_INFO, "PostConfigReplay: Sending PriorityTable to Cluster");
            send_ctx_p->setPartialSuccessFlag(true);
            send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		    sendToWaveCluster(send_ctx_p);
   #endif
             configReplayPriorityMapMessageHandler(pWaveAsynchronousContextForConfigReplayEnd);
        }
        else {
            trace(TRACE_LEVEL_ERROR, "PostConfigReplay: Failed to search for default CEE Map");
        }

        return;
        
    }
    void CeeGlobalWorker::configReplaySendPrirityGroupMsg1(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		// send PGID1 to backend
        string ceeMapName = "default";
	    WaveSendToClusterContext    *send_ctx_p;
	    QosCreatePriorityGroupSPMessage *m = new QosCreatePriorityGroupSPMessage();

	    m->setCeeMapName(ceeMapName);
	    m->setPgId("1");
	    m->setWeight(40);
	    m->setPfc(1);

	    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback),pPrismLinearSequencerContext );
	    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
	    send_ctx_p->setPPrismMessageForPhase1(m);
        trace(TRACE_LEVEL_INFO, string("PostConfigReplay: Sending PiroirtyGroup 1 to Cluster :"));
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
	    sendToWaveCluster(send_ctx_p);
        return;

    }
    void CeeGlobalWorker::configReplaySendPrirityGroupMsg2(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        // send PGID2 to backend
        string ceeMapName = "default";
		WaveSendToClusterContext    *send_ctx_p;
		QosCreatePriorityGroupSPMessage *m2 = new QosCreatePriorityGroupSPMessage();
		m2->setCeeMapName(ceeMapName);
        m2->setPgId("2");
        m2->setWeight(60);
        m2->setPfc(0);

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback),pPrismLinearSequencerContext);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m2);
        trace(TRACE_LEVEL_INFO, string("PostConfigReplay: Sending PiroirtyGroup 2 to Cluster :"));
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);
        return;

    }
    void CeeGlobalWorker::configReplaySendPrirityMapMsg(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
			// send PriorityTable to backend
        string ceeMapName = "default";
		WaveSendToClusterContext    *send_ctx_p;
	    QosCreatePriorityMapSPMessage *m1 = new QosCreatePriorityMapSPMessage(ceeMapName, "2", "2", "2", "1", "2", "2", "2", "15.0");
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CeeGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m1);
		trace(TRACE_LEVEL_INFO, "PostConfigReplay: Sending PriorityTable to Cluster");
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
        return;

    }
    void  CeeGlobalWorker::configReplayPriorityMapMessageHandler( WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::configReplaySendPrirityGroupMsg1),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::configReplaySendPrirityGroupMsg2),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::configReplaySendPrirityMapMsg),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CeeGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveAsynchronousContextForConfigReplayEnd, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }
}
