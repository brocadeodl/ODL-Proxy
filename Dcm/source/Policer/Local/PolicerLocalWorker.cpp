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
 *   Author : sbalodia, Shivanand Tatpatti 
 * 
 *   This Worker consists of message handlers for
         police-priority-map, class-map and policy-map CRUD operations.
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Policer/Local/PolicerLocalObjectManager.h"
#include "Policer/Local/PolicerLocalWorker.h"
#include "Policer/Local/PolicerPriorityMapMessage.h"
#include "Policer/Local/PolicerClassMapMessage.h"
#include "Policer/Local/PolicerPolicyClassMapMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Policer/Local/PolicerTypes.h"
#include "ClientInterface/Policer/PolicerClientPriorityMapMessage.h"
#include "ClientInterface/Policer/PolicerClientClassMapMessage.h"
#include "ClientInterface/Policer/PolicerClientPolicyClassMapMessage.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/Capabilities.h"
#include "DcmCStatus.h"

namespace DcmNs
{

    PolicerLocalWorker::PolicerLocalWorker ( PolicerLocalObjectManager *pPolicerLocalObjectManager)
        : WaveWorker  (pPolicerLocalObjectManager)
    {
        addOperationMap (POLICERPRIORITYMAP, reinterpret_cast<PrismMessageHandler> (&PolicerLocalWorker::PolicerPriorityMapMessageHandler));
        addOperationMap (POLICERCLASSMAP, reinterpret_cast<PrismMessageHandler> (&PolicerLocalWorker::PolicerClassMapMessageHandler));
        addOperationMap (POLICERPOLICYCLASSMAP, reinterpret_cast<PrismMessageHandler> (&PolicerLocalWorker::PolicerPolicyClassMapMessageHandler));
    }

    PolicerLocalWorker::~PolicerLocalWorker ()
    {
    }

    PrismMessage  *PolicerLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case POLICERPRIORITYMAP :
                pPrismMessage = new PolicerPriorityMapMessage ();
                break;
            case POLICERCLASSMAP :
                pPrismMessage = new PolicerClassMapMessage ();
                break;
            case POLICERPOLICYCLASSMAP :
                pPrismMessage = new PolicerPolicyClassMapMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PolicerLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

            trace (TRACE_LEVEL_FATAL, "PolicerLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    
    return (pWaveManagedObject);
    }

    void  PolicerLocalWorker::PolicerClassMapMessageHandler( PolicerClassMapMessage *pPolicerClassMapMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::ClassMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPolicerClassMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  PolicerLocalWorker::PolicerPolicyClassMapMessageHandler( PolicerPolicyClassMapMessage *pPolicerPolicyClassMapMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::PolicyMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPolicerPolicyClassMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId PolicerLocalWorker::ClassMapStep(
		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {

		ResourceId status;
		PolicerClassMapMessage *pPolicerClassMapMessage =
				dynamic_cast<PolicerClassMapMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage());
		
        PolicerClientClassMapMessage *m = new PolicerClientClassMapMessage();

        m->setClassMapName(pPolicerClassMapMessage->getClassMapName());
        m->setAny(pPolicerClassMapMessage->getAny());
        m->setOpcode(pPolicerClassMapMessage->getOpcode());
        m->setAccessListName(pPolicerClassMapMessage->getAccessListName());
        m->setFilterCriteria(pPolicerClassMapMessage->getFilterCriteria());

        trace(
			TRACE_LEVEL_DEBUG,
			string("Sending msg to back qos daemon : ClassMapName : <")
					+ m->getClassMapName() + "> AccessListName : <"
					+ m->getAccessListName() + "> Opcode : <"
					+ m->getOpcode() + ">");

        status = sendSynchronouslyToWaveClient ("qos",m);
        status = errorCode(pPolicerClassMapMessage, m, status);
        delete m;
        return (status);
    }

	ResourceId PolicerLocalWorker::PolicyMapStep(
		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {

		ResourceId status;
		PolicerPolicyClassMapMessage *pPolicerPolicyClassMapMessage =
				dynamic_cast<PolicerPolicyClassMapMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		PolicerClientPolicyClassMapMessage *m =
				new PolicerClientPolicyClassMapMessage();

		m->setPolicyClassMapName(pPolicerPolicyClassMapMessage->getPolicyClassMapName());
        m->setCir(pPolicerPolicyClassMapMessage->getCir());
		m->setCbs(pPolicerPolicyClassMapMessage->getCbs());
		m->setEir(pPolicerPolicyClassMapMessage->getEir());
		m->setEbs(pPolicerPolicyClassMapMessage->getEbs());
		m->setConformDscp(pPolicerPolicyClassMapMessage->getConformDscp());
		m->setConformPrecedence(pPolicerPolicyClassMapMessage->getConformPrecedence());
		m->setConformType(pPolicerPolicyClassMapMessage->getConformType());
		m->setConformTc(pPolicerPolicyClassMapMessage->getConformTc());
		m->setExceedDscp(pPolicerPolicyClassMapMessage->getExceedDscp());
		m->setExceedPrecedence(pPolicerPolicyClassMapMessage->getExceedPrecedence());
		m->setExceedType(pPolicerPolicyClassMapMessage->getExceedType());
		m->setExceedTc(pPolicerPolicyClassMapMessage->getExceedTc());
		m->setClassMapId(pPolicerPolicyClassMapMessage->getClassMapId());
		m->setPriorityMapId(pPolicerPolicyClassMapMessage->getPriorityMapId());
        m->setOpcode(pPolicerPolicyClassMapMessage->getOpcode());

        m->setCoS(pPolicerPolicyClassMapMessage->getCoS());
        m->setTrafficClass(pPolicerPolicyClassMapMessage->getTrafficClass());
        m->setDscp(pPolicerPolicyClassMapMessage->getDscp());
        m->setCoSMutationMapName(pPolicerPolicyClassMapMessage->getCoSMutationMapName());
        m->setCoSTrafficClassMapName(pPolicerPolicyClassMapMessage->getCoSTrafficClassMapName());
        m->setDscpCoSMapName(pPolicerPolicyClassMapMessage->getDscpCoSMapName());
        m->setDscpMutationMapName(pPolicerPolicyClassMapMessage->getDscpMutationMapName());
        m->setDscpTrafficClassMapName(pPolicerPolicyClassMapMessage->getDscpTrafficClassMapName());
        m->setSflowMapName(pPolicerPolicyClassMapMessage->getSflowMapName());
        m->setCeeMapName(pPolicerPolicyClassMapMessage->getCeeMapName());
        m->setShapingRate(pPolicerPolicyClassMapMessage->getShapingRate());
        m->setStrictPriority(pPolicerPolicyClassMapMessage->getStrictPriority());
        m->setDwrrTrafficClass0(pPolicerPolicyClassMapMessage->getDwrrTrafficClass0());
        m->setDwrrTrafficClass1(pPolicerPolicyClassMapMessage->getDwrrTrafficClass1());
        m->setDwrrTrafficClass2(pPolicerPolicyClassMapMessage->getDwrrTrafficClass2());
        m->setDwrrTrafficClass3(pPolicerPolicyClassMapMessage->getDwrrTrafficClass3());
        m->setDwrrTrafficClass4(pPolicerPolicyClassMapMessage->getDwrrTrafficClass4());
        m->setDwrrTrafficClass5(pPolicerPolicyClassMapMessage->getDwrrTrafficClass5());
        m->setDwrrTrafficClass6(pPolicerPolicyClassMapMessage->getDwrrTrafficClass6());
        m->setDwrrTrafficClass7(pPolicerPolicyClassMapMessage->getDwrrTrafficClass7());
        m->setShapingRateTrafficClass0(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass0());
        m->setShapingRateTrafficClass1(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass1());
        m->setShapingRateTrafficClass2(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass2());
        m->setShapingRateTrafficClass3(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass3());
        m->setShapingRateTrafficClass4(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass4());
        m->setShapingRateTrafficClass5(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass5());
        m->setShapingRateTrafficClass6(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass6());
        m->setShapingRateTrafficClass7(pPolicerPolicyClassMapMessage->getShapingRateTrafficClass7());

        trace (TRACE_LEVEL_DEBUG, "sending synchronously to policer client");
        status = sendSynchronouslyToWaveClient ("qos",m);
        status = errorCode(pPolicerPolicyClassMapMessage, m, status);
        delete m;
        return (status);
    }


    void  PolicerLocalWorker::PolicerPriorityMapMessageHandler( PolicerPriorityMapMessage *pPolicerPriorityMapMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::PriorityMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PolicerLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPolicerPriorityMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId PolicerLocalWorker::PriorityMapStep(
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {

		ResourceId status;
		PolicerPriorityMapMessage *pPolicerPriorityMapMessage = dynamic_cast<PolicerPriorityMapMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		PolicerClientPriorityMapMessage *m = new PolicerClientPriorityMapMessage();

		m->setPriorityMapName(pPolicerPriorityMapMessage->getPriorityMapName());
		m->setConformPriorityMap0(pPolicerPriorityMapMessage->getConformPriorityMap0());
		m->setConformPriorityMap1(pPolicerPriorityMapMessage->getConformPriorityMap1());
		m->setConformPriorityMap2(pPolicerPriorityMapMessage->getConformPriorityMap2());
		m->setConformPriorityMap3(pPolicerPriorityMapMessage->getConformPriorityMap3());
		m->setConformPriorityMap4(pPolicerPriorityMapMessage->getConformPriorityMap4());
		m->setConformPriorityMap5(pPolicerPriorityMapMessage->getConformPriorityMap5());
		m->setConformPriorityMap6(pPolicerPriorityMapMessage->getConformPriorityMap6());
		m->setConformPriorityMap7(pPolicerPriorityMapMessage->getConformPriorityMap7());
	
        m->setExceedPriorityMap0(pPolicerPriorityMapMessage->getExceedPriorityMap0());
        m->setExceedPriorityMap1(pPolicerPriorityMapMessage->getExceedPriorityMap1());
        m->setExceedPriorityMap2(pPolicerPriorityMapMessage->getExceedPriorityMap2());
        m->setExceedPriorityMap3(pPolicerPriorityMapMessage->getExceedPriorityMap3());
        m->setExceedPriorityMap4(pPolicerPriorityMapMessage->getExceedPriorityMap4());
        m->setExceedPriorityMap5(pPolicerPriorityMapMessage->getExceedPriorityMap5());
        m->setExceedPriorityMap6(pPolicerPriorityMapMessage->getExceedPriorityMap6());
        m->setExceedPriorityMap7(pPolicerPriorityMapMessage->getExceedPriorityMap7());
		m->setConformCos(pPolicerPriorityMapMessage->getConformCos());
		m->setExceedCos(pPolicerPriorityMapMessage->getExceedCos());
		m->setOpcode(pPolicerPriorityMapMessage->getOpcode());

		trace (TRACE_LEVEL_DEVEL, "sending synchronously to policer client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pPolicerPriorityMapMessage, m, status);
		delete m;
		return (status);
	}

	ResourceId PolicerLocalWorker::errorCode(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status)
    {
        trace (TRACE_LEVEL_DEBUG, string("PolicerLocalWorker::errorCode :- Entering  with status: ") + status);

        pMsg->setCompletionStatus(status);

        if (WAVE_MESSAGE_SUCCESS == status) {
            if (WAVE_MESSAGE_SUCCESS != pMsg->getCompletionStatus()) {
                status = pMsg->getCompletionStatus();
            } else {
                status = clientMsg->getClientStatus();
                trace (TRACE_LEVEL_INFO, string("PolicerLocalWorker::errorCode :- clientMsg->getClientStatus() returned: ") + status);
                pMsg->setCompletionStatus(status);
            }
        }
        return status;
   }

}
