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
 **************************************************************************/
 
#include "VcsFabric/Local/VcsSlotLocalManagedObject.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h" 
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Ssm/Local/SsmBpratelimitLocalWorker.h"
#include "Ssm/Local/BpratelimitLocalConfigMessage.h"
#include "Ssm/Local/BpratelimitLocalManagedObject.h"
#include "Ssm/Local/SsmTypes.h"
#include "Ssm/Global/SsmPlugUtils.h"
#include "ClientInterface/Ssm/BpratelimitClientConfigMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

    SsmBpratelimitLocalWorker::SsmBpratelimitLocalWorker ( SsmLocalObjectManager *pSsmLocalObjectManager) : WaveWorker  (pSsmLocalObjectManager)
    {
        BpratelimitLocalManagedObject    BpratelimitLocalManagedObject(pSsmLocalObjectManager);
        BpratelimitLocalManagedObject.setupOrm ();
        addManagedClass (BpratelimitLocalManagedObject::getClassName (), this);
        addOperationMap (BPRATELIMITLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&SsmBpratelimitLocalWorker::BpratelimitLocalConfigMessageHandler));
    }

    SsmBpratelimitLocalWorker::~SsmBpratelimitLocalWorker ()
    {
    }

    PrismMessage  *SsmBpratelimitLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case BPRATELIMITLOCALCONFIG :
                pPrismMessage = new BpratelimitLocalConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SsmBpratelimitLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if (((BpratelimitLocalManagedObject::getClassName ()) == managedClassName) && NULL != getPWaveObjectManager())
        {
            pWaveManagedObject = new BpratelimitLocalManagedObject(dynamic_cast<SsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "SsmBpratelimitLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }




    ResourceId SsmBpratelimitLocalWorker::configBpratelimitLocalConfigStep 
                                                (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
 		ResourceId status = WAVE_MESSAGE_SUCCESS;
        BpratelimitLocalConfigMessage *pBpratelimitLocalConfigMessage = dynamic_cast<BpratelimitLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		SSM_PLUG_DBG_TRACE();
        trace (TRACE_LEVEL_DEBUG, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigStep: Entered....");
        if(NULL == pBpratelimitLocalConfigMessage)
        {
 			SSM_PLUG_DBG_LVL_S(string("pBpratelimitLocalConfigMessage is NULL:"));
            return WAVE_MESSAGE_ERROR ;
        }


       BpratelimitClientConfigMessage *m = new BpratelimitClientConfigMessage();
 		if (m != NULL) {
 			SSM_PLUG_DBG_LVL_S(string("configBpratelimitLocalConfig message being sent to Ssm : ") +  string ("slotId: ")
 							+ pBpratelimitLocalConfigMessage->getSlotId() );
             m->setSlotId(pBpratelimitLocalConfigMessage->getSlotId());
             m->setOpCode(pBpratelimitLocalConfigMessage->getOpCode());
 			status = sendSynchronouslyToWaveClient ("ssm", m);
 			SSM_PLUG_INFO_LVL_S(string("BpratelimitLocalConfigMessage : status - ") + status + string(",Completion status - ")
                                + m->getCompletionStatus ()
 								+ string(",Client status - ") + m->getClientStatus());

 			if (status == WAVE_MESSAGE_SUCCESS) {
 				if (m->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
 					status = m->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to Configure Bpratelimit  on the backend: "));
				} else if (m->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
 					status = m->getClientStatus();
					SSM_PLUG_ERR_LVL_S(string("Failed to  Configure Bpratelimit on the  backend: "));
				}
			} else {
					SSM_PLUG_ERR_LVL_S(string("Failed to  Configure Bpratelimit on the  backend: "));
			}
 			delete m;
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
        }

        return status;
    }

    ResourceId SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep(
		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	BpratelimitLocalConfigMessage *pBpratelimitLocalConfigMessage =
			dynamic_cast<BpratelimitLocalConfigMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage());
	trace(
			TRACE_LEVEL_DEBUG,
			"SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep: Entered....");

	if (pBpratelimitLocalConfigMessage->getSlotId() == SSM_BPRATELIMIT_ALL_SLOTS) {
		//For the command "bp-rate-limit heavy slot 0", need to apply bp rate limit for all the slots.
		//Query to get the list of slot numbers and apply the bp rate limit on each of the slots.
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
				VcsSlotLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectField("slotNumber");
		vector<WaveManagedObject *> *pResults = querySynchronously(
				&syncQueryCtxt);
		VcsSlotLocalManagedObject *vcsSlotManagedObject = NULL;

		for (unsigned int iterator = 0; iterator < pResults->size();
				iterator++) {
			vcsSlotManagedObject =
					dynamic_cast<VcsSlotLocalManagedObject *>((*pResults)[iterator]);
			pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(
					vcsSlotManagedObject);
			trace(
					TRACE_LEVEL_DEBUG,
					string("SsmBpratelimitLocalWorker:: SLOT NUMBER : ")
							+ vcsSlotManagedObject->getSlotNumber());
			status = configBpratelimitLocalConfigObjStep(
					pPrismSynchronousLinearSequencerContext,
					vcsSlotManagedObject->getSlotNumber());
		}
		if (pResults) {
			pResults->clear();
			delete pResults;
		}

	} else {
		status = configBpratelimitLocalConfigObjStep(
				pPrismSynchronousLinearSequencerContext,
				pBpratelimitLocalConfigMessage->getSlotId());
	}

	return status;
}

    ResourceId SsmBpratelimitLocalWorker::configBpratelimitLocalConfigObjStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext,UI32 slotId)
    {
        int createFlag = 1;
        int status = WAVE_MESSAGE_ERROR;

        BpratelimitLocalManagedObject *pBpratelimitLocalManagedObject = NULL;
        BpratelimitLocalConfigMessage *pBpratelimitLocalConfigMessage = dynamic_cast<BpratelimitLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        trace (TRACE_LEVEL_DEBUG, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep: Entered....");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(BpratelimitLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32 (&slotId, "slotId"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:numberOfResults = 0");
                createFlag = 1;
                status = WAVE_MESSAGE_SUCCESS;
            } else if (1 == numberOfResults) {
                if((NULL==(*pResults)[0])) {
                    status = WAVE_MESSAGE_ERROR;
                }
                else {
                    pBpratelimitLocalManagedObject  = dynamic_cast<BpratelimitLocalManagedObject*>((*pResults)[0]);
                    trace (TRACE_LEVEL_DEBUG, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:numberOfResults = 1");
                    createFlag = 0;
                    status = WAVE_MESSAGE_SUCCESS;
                }
            } else {
                    status = WAVE_MESSAGE_ERROR;
                }
            }
        if (createFlag == 0) {
            trace (TRACE_LEVEL_DEVEL, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:createFlag = 0");
            prismAssert (NULL != pBpratelimitLocalManagedObject, __FILE__, __LINE__);
            switch(pBpratelimitLocalConfigMessage->getOpCode())
            {
            case SSM_BPRATELIMIT_HEAVY:
                  trace (TRACE_LEVEL_DEVEL, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:SsmBpratelimitLocalWorkercase case SSM_BPRATELIMIT_HEAVY");
                   updateWaveManagedObject (pBpratelimitLocalManagedObject);
       
                  break;
            case SSM_BPRATELIMIT_NO_HEAVY:
                  delete pBpratelimitLocalManagedObject ;
                  delete pResults;
                  trace (TRACE_LEVEL_DEVEL, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:SsmBpratelimitLocalWorkercase case SSM_BPRATELIMIT_NO_HEAVY");
                  return WAVE_MESSAGE_SUCCESS;
                  break;
            default:
                   updateWaveManagedObject (pBpratelimitLocalManagedObject);
                break;
           }
        } else {
            trace (TRACE_LEVEL_DEVEL, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:createFlag = 1");

            switch(pBpratelimitLocalConfigMessage->getOpCode())
            {
                case SSM_BPRATELIMIT_HEAVY:
                    pBpratelimitLocalManagedObject = new BpratelimitLocalManagedObject(dynamic_cast<SsmLocalObjectManager*>(getPWaveObjectManager()));
                    prismAssert (NULL != pBpratelimitLocalManagedObject, __FILE__, __LINE__);
                    trace (TRACE_LEVEL_DEVEL, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:createFlag = 1 pBpratelimitLocalManagedObject created");
                    trace (TRACE_LEVEL_DEVEL, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:SsmBpratelimitLocalWorkercase case SSM_BPRATELIMIT_HEAVY");
                    pBpratelimitLocalManagedObject->setSlot(slotId);
       
                   break;
               case SSM_BPRATELIMIT_NO_HEAVY:
                  trace (TRACE_LEVEL_DEVEL, "SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep:SsmBpratelimitLocalWorkercase case SSM_BPRATELIMIT_NO_HEAVY");
                  if(pBpratelimitLocalManagedObject)
                      updateWaveManagedObject (pBpratelimitLocalManagedObject);
                  break;
               default:
                 break;
           }
        }

        if ( (createFlag) && (pBpratelimitLocalManagedObject)) {
            //mark the MO for GC
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(
                    pBpratelimitLocalManagedObject);
        }
		if (pResults) {
			pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(
				*pResults);
            delete pResults;
        }

        return status;
    }

  ResourceId SsmBpratelimitLocalWorker::validateBpratelimitLocalConfigStep(
		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	BpratelimitLocalConfigMessage *pBpratelimitLocalConfigMessage =
			dynamic_cast<BpratelimitLocalConfigMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage());
	SSM_PLUG_DBG_TRACE();
	SSM_PLUG_DBG_LVL("validateBpratelimitLocalConfigStep called");

	trace(TRACE_LEVEL_INFO, string("validate slot id : ") + status);
	if ((pBpratelimitLocalConfigMessage->getOpCode() == SSM_BPRATELIMIT_NO_HEAVY)
			|| (pBpratelimitLocalConfigMessage->getOpCode()
					== SSM_BPRATELIMIT_HEAVY)) {
		UI32 slotId;
		slotId = pBpratelimitLocalConfigMessage->getSlotId();

		//Validate whether the input slot exists in a given chassis.
		if (SSM_BPRATELIMIT_ALL_SLOTS
				!= pBpratelimitLocalConfigMessage->getSlotId()) {

			UI32 slotCount = 0;
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
					VcsSlotLocalManagedObject::getClassName());
			syncQueryCtxt.addAndAttribute(
					new AttributeUI32(&slotId, "slotNumber"));
			status = querySynchronouslyForCount(&syncQueryCtxt, slotCount);
			if (0 == slotCount) {
				status = SSM_QOS_INVALID_SLOT_NUMBER;
			}
		}
	}

	return status;
}
    void  SsmBpratelimitLocalWorker::BpratelimitLocalConfigMessageHandler( BpratelimitLocalConfigMessage *pBpratelimitLocalConfigMessage)
    {
        // Validations and Queries should go here
    	SSM_PLUG_DBG_TRACE();

		SSM_PLUG_DBG_LVL("SsmBpratelimitLocalWorker::BpratelimitLocalConfigMessageHandler called");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmBpratelimitLocalWorker::validateBpratelimitLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmBpratelimitLocalWorker::configBpratelimitLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmBpratelimitLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmBpratelimitLocalWorker::configBpratelimitLocalConfigMOStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmBpratelimitLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmBpratelimitLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmBpratelimitLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pBpratelimitLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
    void SsmBpratelimitLocalWorker::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string passName = pWaveAsynchronousContextForPostBootPhase->getPassName();

        SSM_PLUG_INFO_LVL_S(string("Ssm Postboot Entered with  passName: ") + passName);

        if (passName.compare("DCM_POSTBOOT_SLOT_STAGE2") == 0) {
			SSM_PLUG_INFO_LVL_S(string("SsmBpratelimitLocalWorker::postboot DCM_POSTBOOT_SLOT_STAGE2"));	
			UI32 slotNum = pWaveAsynchronousContextForPostBootPhase->getSlotNum();
			configReplayBpRateLimit(slotNum);
		}

        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

    void SsmBpratelimitLocalWorker::configReplayBpRateLimit(UI32 slotId)
    {
        int status = WAVE_MESSAGE_ERROR;

        trace (TRACE_LEVEL_DEBUG, "SsmBpratelimitLocalWorker::configReplayBpRateLimit: Entered....");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(BpratelimitLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32 (&slotId, "slotId"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "SsmBpratelimitLocalWorker::configReplayBpRateLimit:numberOfResults = 0");
            } else if (1 == numberOfResults) {
                if((NULL!=(*pResults)[0])) {
                    trace (TRACE_LEVEL_DEBUG, "SsmBpratelimitLocalWorker::configReplayBpRateLimit:numberOfResults = 1");
                     BpratelimitClientConfigMessage *m = new BpratelimitClientConfigMessage();
 		             if (m != NULL) {
 			             SSM_PLUG_DBG_LVL_S(string("configReplayBpRateLimit message being sent to Ssm : ") +  string ("slotId: ")
 						            	+ slotId);
                         m->setSlotId(slotId);
                         m->setOpCode(SSM_BPRATELIMIT_HEAVY);
 			             status = sendSynchronouslyToWaveClient ("ssm", m);
 			             SSM_PLUG_INFO_LVL_S(string("configReplayBpRateLimit BpratelimitLocalConfigMessage : status - ") + status + string(",Completion status - ")
                                                  + m->getCompletionStatus ()
 								              + string(",Client status - ") + m->getClientStatus());

 			             if (status == WAVE_MESSAGE_SUCCESS) {
 				             if (m->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
 					             status = m->getCompletionStatus ();
					             SSM_PLUG_ERR_LVL_S(string("configReplayBpRateLimit:Failed to Configure Bpratelimit  on the backend: "));
				             } else if (m->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
 					             status = m->getClientStatus();
					             SSM_PLUG_ERR_LVL_S(string("configReplayBpRateLimit:Failed to  Configure Bpratelimit on the  backend: "));
				            }
			             } else {
					         SSM_PLUG_ERR_LVL_S(string("configReplayBpRateLimit:Failed to  Configure Bpratelimit on the  backend: "));
			             }
 			             delete m;
		             }
                 }
             }
             WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
         }        
         return;
    }
}
