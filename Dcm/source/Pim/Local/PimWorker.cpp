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
 *   Author : rjain                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h" 
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"           
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h" 
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Postboot/PrismPostbootMessages.h"
#include "DcmCStatus.h"  
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimWorker.h"
#include "Pim/Local/PimBsrCandidateCfgMessage.h"
#include "Pim/Local/PimRpCandidateCfgMessage.h"
#include "Pim/Local/PimRpCandidatePrefixCfgMessage.h"
#include "Pim/Local/PimStaticRpCfgMessage.h"
#include "Pim/Local/PimRtrInstMessage.h"
#include "Pim/Local/PimMultRtrCfgMessage.h"
#include "Pim/Local/PimTypes.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimIpMultCfgClientMsg.h"
#include "ClientInterface/Pim/PimBsrCandClientMsg.h"
#include "ClientInterface/Pim/PimStaticRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimCandRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimCandRpPrefixCfgClientMsg.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/Pim/PimIntfCfgMessage.h"
#include "Utils/Layer3LicenseUtil.h"
#include "ClientInterface/Pim/PimUtils.h"
#include "DcmResourceIds.h"

namespace DcmNs
{

    PimWorker::PimWorker ( PimLocalObjectManager *pPimLocalObjectManager)
        : WaveWorker  (pPimLocalObjectManager)
    {

	 /* Removing support for BSR, RP-CAND AND PREFIX */


		//BsrCandidateLocalManagedObject    BsrCandidateLocalManagedObject
	    //(pPimLocalObjectManager);		
        //RpCandidateLocalManagedObject    RpCandidateLocalManagedObject
	    //(pPimLocalObjectManager);
		//RpCandidatePrefixLocalManagedObject 
		//RpCandidatePrefixLocalManagedObject	    (pPimLocalObjectManager);
        StaticRpLocalManagedObject    StaticRpLocalManagedObject
	    (pPimLocalObjectManager);
        MultRtrCfgLocalManagedObject    MultRtrCfgLocalManagedObject
	    (pPimLocalObjectManager);
        PimRtrCfgLocalManagedObject    PimRtrCfgLocalManagedObject
	    (pPimLocalObjectManager);

        //BsrCandidateLocalManagedObject.setupOrm ();
        //addManagedClass (BsrCandidateLocalManagedObject::getClassName (), this);
        PimRtrCfgLocalManagedObject.setupOrm ();
        addManagedClass (PimRtrCfgLocalManagedObject::getClassName (), this);
        //RpCandidateLocalManagedObject.setupOrm ();
        //addManagedClass (RpCandidateLocalManagedObject::getClassName (), this);
		//RpCandidatePrefixLocalManagedObject.setupOrm ();
        //addManagedClass (RpCandidatePrefixLocalManagedObject::getClassName (), this);
        StaticRpLocalManagedObject.setupOrm ();
        addManagedClass (StaticRpLocalManagedObject::getClassName (), this);
        MultRtrCfgLocalManagedObject.setupOrm ();
        addManagedClass (MultRtrCfgLocalManagedObject::getClassName (), this);

        //addOperationMap (PIMBSRCANDIDATECFG,
        //    reinterpret_cast<PrismMessageHandler> (&PimWorker::PimBsrCandidateCfgMessageHandler));
        //addOperationMap (PIMRPCANDIDATECFG,
        //    reinterpret_cast<PrismMessageHandler> (&PimWorker::PimRpCandidateCfgMessageHandler));
		//addOperationMap (PIMRPCANDIDATEPREFIXCFG,
		//	reinterpret_cast<PrismMessageHandler> (&PimWorker::PimRpCandidatePrefixCfgMessageHandler));
        addOperationMap (PIMSTATICRPCFG,
            reinterpret_cast<PrismMessageHandler> (&PimWorker::PimStaticRpCfgMessageHandler));
        addOperationMap (PIMRTRINST,
            reinterpret_cast<PrismMessageHandler> (&PimWorker::PimRtrInstMessageHandler));
        addOperationMap (PIMMULTRTRCFG,
            reinterpret_cast<PrismMessageHandler> (&PimWorker::PimMultRtrCfgMessageHandler));
    }

    PimWorker::~PimWorker ()
    {
    }

    PrismMessage *
    PimWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case PIMBSRCANDIDATECFG :
                pPrismMessage = new PimBsrCandidateCfgMessage ();
                break;
            case PIMRPCANDIDATECFG :
                pPrismMessage = new PimRpCandidateCfgMessage ();
                break;
			case PIMRPCANDIDATEPREFIXCFG:
				pPrismMessage = new PimRpCandidatePrefixCfgMessage ();
				break;
            case PIMSTATICRPCFG :
                pPrismMessage = new PimStaticRpCfgMessage ();
                break;
            case PIMRTRINST :
                pPrismMessage = new PimRtrInstMessage ();
                break;
            case PIMMULTRTRCFG :
                pPrismMessage = new PimMultRtrCfgMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject *PimWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if((BsrCandidateLocalManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new BsrCandidateLocalManagedObject(
                dynamic_cast<PimLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if((RpCandidateLocalManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new RpCandidateLocalManagedObject(
                dynamic_cast<PimLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if((RpCandidatePrefixLocalManagedObject::getClassName()) == managedClassName)
		{
		    pWaveManagedObject = new RpCandidatePrefixLocalManagedObject(
		        dynamic_cast<PimLocalObjectManager *>(getPWaveObjectManager()));
		}
        else if((StaticRpLocalManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new StaticRpLocalManagedObject(
                dynamic_cast<PimLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if((PimRtrCfgLocalManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new PimRtrCfgLocalManagedObject(
                dynamic_cast<PimLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if((MultRtrCfgLocalManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new MultRtrCfgLocalManagedObject(
                dynamic_cast<PimLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace(TRACE_LEVEL_FATAL, string("PimWorker::createManagedObjectInstance")
                + string(": Unknown Managed Class Name : ")
                + managedClassName);
        }
        return (pWaveManagedObject);
    
}

    /* Message handler/sequencer initialization for BSR candidate msg */
    void  
    PimWorker::PimBsrCandidateCfgMessageHandler(PimBsrCandidateCfgMessage *pPimBsrCandidateCfgMessage)
    {
// Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			/* Send msg to backend daemon */
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				 &PimWorker::sendPimBsrCandidateStep),
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				 &PimWorker::prismSynchronousLinearSequencerStartTransactionStep),

			 /* Update DCM DB */
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				 &PimWorker::updatePimBsrCandidateDbStep),

			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				  &PimWorker::prismSynchronousLinearSequencerCommitTransactionStep),

			  // Programming Protocol Daemons goes here

			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				  &PimWorker::prismSynchronousLinearSequencerSucceededStep),
			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				  &PimWorker::prismSynchronousLinearSequencerFailedStep)
		};

        PrismSynchronousLinearSequencerContext *pSeqCtx = 
            new PrismSynchronousLinearSequencerContext(pPimBsrCandidateCfgMessage, 
                this,
                sequencerSteps, 
                sizeof(sequencerSteps) /sizeof (sequencerSteps[0]));

       pSeqCtx->execute ();
    }


    /* Message handler/sequencer initialization for Router Config msg */
    void  
    PimWorker::PimRtrInstMessageHandler(PimRtrInstMessage *pPimRtrInstMessage)
    {
// Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		  	/* Validation */
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				&PimWorker::validationStep),
			/* Send msg to backend daemon */
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				 &PimWorker::sendPimRtrCfgStep),
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				 &PimWorker::prismSynchronousLinearSequencerStartTransactionStep),
			 /* Update DCM DB */
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				 &PimWorker::updPimRtrCfgDbStep),

			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				  &PimWorker::prismSynchronousLinearSequencerCommitTransactionStep),

			  // Programming Protocol Daemons goes here

			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				  &PimWorker::prismSynchronousLinearSequencerSucceededStep),
			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(
				  &PimWorker::prismSynchronousLinearSequencerFailedStep)
		};

        PrismSynchronousLinearSequencerContext *pSeqCtx = 
            new PrismSynchronousLinearSequencerContext(pPimRtrInstMessage, this,
                sequencerSteps, 
                sizeof(sequencerSteps) /sizeof (sequencerSteps[0]));

        pSeqCtx->execute ();
    }
	
    /* Message handler/sequencer initialization for multicast routing msg */
    void
    PimWorker::PimMultRtrCfgMessageHandler(
                   PimMultRtrCfgMessage *pPimMultRtrCfgMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Send msg to backend daemon */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::sendPimIpMultCfgStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerStartTransactionStep),
            /* Update DCM DB */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::updPimIpMultCfgDbStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pSeqCtx =
            new PrismSynchronousLinearSequencerContext (pPimMultRtrCfgMessage,
                this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pSeqCtx->execute ();
    }

    /* Message handler/sequencer initialization for RP candidate msg */
    void
    PimWorker::PimRpCandidateCfgMessageHandler(
                   PimRpCandidateCfgMessage *pPimRpCandidateCfgMessage)
    {
        // Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Send msg to backend daemon */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::sendPimCandRpCfgStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerStartTransactionStep),
            /* Update DCM DB */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::updPimCandRpCfgDbStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pSeqCtx =
            new PrismSynchronousLinearSequencerContext (pPimRpCandidateCfgMessage,
                this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pSeqCtx->execute ();
    }

    /* Message handler/sequencer initialization for RP candidate prefix msg */
    void
    PimWorker::PimRpCandidatePrefixCfgMessageHandler(
        PimRpCandidatePrefixCfgMessage *pPimRpCandidatePrefixCfgMessage)
    {
        // Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			/* Validate MO for msgType DEL */
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(
			&PimWorker::validatePimCandRpPrefixCfgDbStep),
            /* Send msg to backend daemon */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
            &PimWorker::sendPimCandRpPrefixCfgStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
			&PimWorker::prismSynchronousLinearSequencerStartTransactionStep),
            /* Update DCM DB */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
			&PimWorker::updPimCandRpPrefixCfgDbStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(
            &PimWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
            &PimWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
	        &PimWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pSeqCtx =
            new PrismSynchronousLinearSequencerContext (
				pPimRpCandidatePrefixCfgMessage,
                this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pSeqCtx->execute ();
    }


    /* Message handler/sequencer initialization for static RP msg */
    void
    PimWorker::PimStaticRpCfgMessageHandler(
                   PimStaticRpCfgMessage *pPimStaticRpCfgMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Send msg to backend daemon */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::sendPimStaticRpCfgStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerStartTransactionStep),
            /* Update DCM DB */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::updPimStaticRpCfgDbStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pSeqCtx =
            new PrismSynchronousLinearSequencerContext (pPimStaticRpCfgMessage,
                this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pSeqCtx->execute ();
    }
	
    /******************************************************/
    /*                 Utility functions                  */
    /******************************************************/
    PimRtrCfgLocalManagedObject* PimWorker::getPimRtrCfgMo(
        PrismSynchronousLinearSequencerContext  *pContext, string MOAttr)
    {
        PimRtrCfgLocalManagedObject *pMo = NULL;

        /* Query the database and get the MO if it exists */
        vector<string> selectFields;
		LocationId locId = FrameworkToolKit::getThisLocationId();
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(PimRtrCfgLocalManagedObject::getClassName());
        selectFields.push_back("objectId");
        selectFields.push_back("ownerManagedObjectId");
        selectFields.push_back(MOAttr);
        syncQueryCtxt.addSelectFields(selectFields);
        vector<WaveManagedObject *> *pResults = 
					querySynchronouslyLocalManagedObjectsForLocationId(locId, &syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0 || numberOfResults > 1) {
                pContext->addManagedObjectsForGarbageCollection(*pResults);
                delete pResults;
                return NULL;
            }
            pMo = dynamic_cast<PimRtrCfgLocalManagedObject*>((*pResults)[0]);
        }

        // user shoudld add it to GC
        // pContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;

        return pMo;
    }

    int32_t 
    PimWorker::pimExtractSlot(string ifName, UI32 ifType)
    {
        int32_t slotId = 0;
        
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimExtractSlot") +
            string(": Entered...."));
            
        switch(ifType) {
            case INTERFACE_TENGIG:
            case INTERFACE_ONEGIG:
            case INTERFACE_FORTYGIG:
                slotId = NsmUtils::getSlotId(ifName);
                break;
            case INTERFACE_VLAN:
            case INTERFACE_PORT_CHANNEL:
                slotId = WAVE_INVALID_SLOT;
                break;
            default:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimExtractSlot") +
                    string(": Unrecognized ifType ") + ifType);
        }
        
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimExtractSlot") +
            string(": Extracted slot = ") + slotId);
            
        return(slotId);
    }
   
    void
    PimWorker::copyPimDfltVals(PimRtrInstMessage *pMsg, 
        PimRtrCfgClientMsg *pClientMsg)
    {
        pClientMsg->setMaxMcache(pMsg->getMaxMcache());
        pClientMsg->setHelloTimer(pMsg->getHelloTimer());
        pClientMsg->setNbrTo(pMsg->getNbrTimeout());
        pClientMsg->setInactTimer(pMsg->getInactivityTimer());
        pClientMsg->setMsgIntv(pMsg->getMsgInterval());
        pClientMsg->setSptThold(pMsg->getSptThreshold());

        return;
    } 

    /******************************************************/
    /*        Daemon call and update DB functions         */
    /******************************************************/

    /* PIM BSR candidate configuration daemon call */
    ResourceId
    PimWorker::sendPimBsrCandidateStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimBsrCandidateStep") +
            string(": Entered...."));
		bool is_pim_supported_platform;
        int status = WAVE_MESSAGE_SUCCESS;
        // Check for Layer3 License
        LAYER_3_LICENSE_CHECK();

        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform) {
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }

        PimBsrCandidateCfgMessage *pMsg = dynamic_cast<PimBsrCandidateCfgMessage *>
                                      (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimWorker:sendPimBsrCandidateStep:") +
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }

        /* Create the client message and populate its fields */
        PimBsrCandClientMsg *pClientMsg = new PimBsrCandClientMsg();
        pClientMsg->setIntfType(pMsg->getIntfType());
        pClientMsg->setIntfName(pMsg->getIntfName());
        pClientMsg->setHashMask(pMsg->getMask());
        pClientMsg->setPriority(pMsg->getPriority());
        pClientMsg->setMsgType(pMsg->getMsgType());

        /* Send the message to the backend daemon */
        status = sendSynchronouslyToWaveClient("pim", pClientMsg);
        if(status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimBsrCandidateStep") +
                string(": Message sent successfully to backend daemon"));
            
            /* Get the status returned by the backend daemon code */
            status = pClientMsg->getClientStatus();
            pMsg->setCompletionStatus(status);

            /* Update the MO even if the interface is not PIM enabled. A 
             * warning message will be displayed to the user */
            if(status == WRC_PIM_CLI_WARN_CAND_NOTACTV) {
                startTransaction();
                updatePimBsrCandidateDbStep(pSeqCtx);

                if(commitTransaction() == WAVE_MESSAGE_SUCCESS)
                    trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimBsrCandidateStep") +
                        string(": BSR candidate cfg commited to DB"));
                else
                    trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimBsrCandidateStep") +
                        string(": BSR candidate cfg commit failed with status = ")
                        + status);
            }
        }
        else
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimBsrCandidateStep") +
                string(": Error while sending message to backend daemon"));

        delete pClientMsg;
        return(status);
    }
    
    /* Function to update the BSR candidate MO fields */
    void
    PimWorker::pimBsrCandMOAdd(BsrCandidateLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, PimBsrCandidateCfgMessage *pMsg, 
        int addInVector)
    {
        bool usr_cfg = PIM_FALSE;
        int32_t slotId = 0;
        
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimBsrCandMOAdd") +
            string(": Entered...."));

        /* Set the fields of the MO */
        pMo->setIntfType(pMo->mapIntfTypeToEnumVal(pMsg->getIntfType()));
        pMo->setIntfName(pMsg->getIntfName());

        if(pMsg->getMask() != 0)
            pMo->setMask(pMsg->getMask());
        pMsg->setMask(pMo->getMask());

        if(pMsg->getPriority() != 0)
            usr_cfg = PIM_TRUE;
        pMo->setPriority(pMsg->getPriority(), usr_cfg);
        
        /* Associate the MO with the slot ID */
        slotId = pimExtractSlot(pMsg->getIntfName(), pMsg->getIntfType());
        pMo->associateWithSlot(slotId);
        
		/* Add the BSR MO to the PIM router MO if it is add*/
		if(addInVector) {
	        pMo->setOwnerManagedObjectId(pPimRtrMo->getObjectId());
    	    addToComposition(PimRtrCfgLocalManagedObject::getClassName(), 
				BsrCandidateLocalManagedObject::getClassName(), "BsrCandidateCfg", 
				pPimRtrMo->getObjectId(), pMo->getObjectId());
		} else { /* For update operation just update the child MO */
		  	updateWaveManagedObject (pMo);
		}
        
        return;
    }

    /* Function to delete the BSR candidate MO */
    void
    PimWorker::pimBsrCandMODel(BsrCandidateLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, PimBsrCandidateCfgMessage *pMsg)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimBsrCandMODel") +
            string(": Entered...."));

        
        /* Mark the parent MO for updation */
        deleteFromComposition(PimRtrCfgLocalManagedObject::getClassName(), 
			BsrCandidateLocalManagedObject::getClassName(), "BsrCandidateCfg", 
			pMo->getOwnerManagedObjectId(), pMo->getObjectId());
        
        return;
    }
    
    /* PIM BSR candidate configuration update DB */
    ResourceId
    PimWorker::updatePimBsrCandidateDbStep(PrismSynchronousLinearSequencerContext *pSeqCtx) 
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updatePimBsrCandidateDbStep") +
            string(": Entered...."));

        PimRtrCfgLocalManagedObject *pPimRtrMo = NULL;
        BsrCandidateLocalManagedObject *pMo = NULL;
        int status = WAVE_MESSAGE_SUCCESS;
		string MOAttr = "BsrCandidateCfg";
        UI8 intfType;
        string intfName;
        PimBsrCandidateCfgMessage *pMsg = dynamic_cast<PimBsrCandidateCfgMessage*>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);
        
        /* Get a pointer to the PIM router MO */
        pPimRtrMo = getPimRtrCfgMo(pSeqCtx, MOAttr);
        PIM_PLUG_ASSERT_PTR (pPimRtrMo);
        
        intfType = pMsg->getIntfType();
        intfName = pMsg->getIntfName();
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updatePimBsrCandidateDbStep: ") +
            string("Intf Type = ") + pimGetIntfTypeStr(intfType) + 
            string(" Intf Name = ") + intfName);
        
        /* Get a pointer to the BSR candidate MO if it exists */
        pMo = pPimRtrMo->getBsrCandMo(intfType, intfName);
        
        if(pMo == NULL) {
            trace (TRACE_LEVEL_DEBUG, string("PimWorker::updatePimBsrCandidateDbStep: ") +
                string("BSR Candidate MO not found"));

            /* Create a new MO since we didnt find one */
            pMo = new BsrCandidateLocalManagedObject(
                      dynamic_cast<PimLocalObjectManager*>(getPWaveObjectManager()));
            PIM_PLUG_ASSERT_PTR (pMo);
        }
        else {
            trace (TRACE_LEVEL_DEBUG,
                string("PimWorker::updatePimBsrCandidateDbStep: ") +
                string("BSR Candidate MO exists"));
        }
        
        trace (TRACE_LEVEL_DEBUG, string("PimWorker::updatePimBsrCandidateDbStep: ")
            + string("Updating PIM BSR candidate cfg MO "));
        switch(pMsg->getMsgType()) {
            case PIM_MSG_ADD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updatePimBsrCandidateDbStep:")
                    + string(" Recvd add message") );
                /* Call the Add function to update the fields of the MO */
                pimBsrCandMOAdd(pMo, pPimRtrMo, pMsg, PIM_TRUE);
                break;
            case PIM_MSG_UPD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updatePimBsrCandidateDbStep:")
                    + string(" Recvd update message") );
                /* Call the update function to update the fields of the MO */
                pimBsrCandMOAdd(pMo, pPimRtrMo, pMsg, PIM_FALSE);
                break;
            case PIM_MSG_DEL:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updatePimBsrCandidateDbStep:")
                    + string(" Recvd delete message") );
                pimBsrCandMODel(pMo, pPimRtrMo, pMsg);
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimWorker::updatePimBsrCandidateDbStep: ") +
                    string("Unrecognized MsgType: ") +
                    pMsg->getMsgType());
        }
                
        if(pPimRtrMo)
            pSeqCtx->addManagedObjectForGarbageCollection(pPimRtrMo);

        if(pMo && (pMsg->getMsgType() == PIM_MSG_ADD)) {
            pSeqCtx->addManagedObjectForGarbageCollection(pMo);
        }

        return(status);
    }

    ResourceId
    PimWorker::validationStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        bool is_pim_supported_platform;
        PimRtrCfgLocalManagedObject *pMo = NULL;
		LocationId locId = FrameworkToolKit::getThisLocationId();
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::validationStep") +
            string(": Entered...."));

        // Check for Layer3 license agaiin before sending to backend
        LAYER_3_LICENSE_CHECK();

        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform) {
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }

        PimRtrInstMessage *pMsg = dynamic_cast<PimRtrInstMessage *>
                                      (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimWorker::validationStep") +
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }

        int status = WAVE_MESSAGE_SUCCESS;
		SI16 helloTimer = 0;

        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(PimRtrCfgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = 
					querySynchronouslyLocalManagedObjectsForLocationId(locId, &syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size();
            if ((0 == numberOfResults) && (pMsg->getCmdCode() !=
				  PIM_CFG_RTR_PIM_CMD)) {
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::validationStep:") +
                    string("Query resulted in no results"));

                status = WRC_PIM_CLI_ERR_GLBL_PIM_NOT_EN;
		    } else if (1 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::validationStep:") +
                    string("Query result success"));
                pMo  = dynamic_cast<PimRtrCfgLocalManagedObject*>((*pResults)[0]);
                PIM_PLUG_ASSERT_PTR (pMo);
				helloTimer = pMo->getHelloTimer();
	    	}
			pSeqCtx->addManagedObjectsForGarbageCollection(*pResults);
			delete pResults;
			

    	    switch(pMsg->getCmdCode()) {
        	    case PIM_CFG_NBR_TO_CMD:
            	    trace(TRACE_LEVEL_DEBUG, string("PimWorker::validationStep:")
                	    + string("PIM_CFG_NBR_TO_CMD, val = ")
                    	+ pMsg->getNbrTimeout());
					SI16 msgNbrTimeout = pMsg->getNbrTimeout();
					if(msgNbrTimeout < (double)(3.5 * helloTimer))
			    		return WRC_PIM_CLI_ERR_NBR_TIMEOUT_LESS;
            	    break;
        	}
		}
		return status;
    }


    /* PIM router configuration daemon call */
    ResourceId
    PimWorker::sendPimRtrCfgStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
		bool is_pim_supported_platform;
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep") +
            string(": Entered...."));

        // Check for Layer3 license agaiin before sending to backend
        LAYER_3_LICENSE_CHECK();

        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform) {
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }

        PimRtrInstMessage *pMsg = dynamic_cast<PimRtrInstMessage *>
                                      (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimWorker::sendPimRtrCfgStep") +
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }

        int status = WAVE_MESSAGE_SUCCESS;
        /* Create the client message and populate its fields */
        PimRtrCfgClientMsg *pClientMsg = new PimRtrCfgClientMsg();
        switch(pMsg->getCmdCode()) {
            case PIM_CFG_RTR_PIM_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_RTR_PIM_CMD, val = ")
                    + pMsg->getVrfId());
                pClientMsg->setVrfId(pMsg->getVrfId());
  				/* Copy the default values of the router pim fields to be 
                 * sent to the backend */
                copyPimDfltVals(pMsg, pClientMsg);
                break;
            case PIM_CFG_MAX_MCACHE_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_MAX_MCACHE_CMD, val = ")
                    + pMsg->getMaxMcache());
                pClientMsg->setMaxMcache(pMsg->getMaxMcache());
                break;
            case PIM_CFG_HELLO_TIMER_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_HELLO_TIMER_CMD, val = ")
                    + pMsg->getHelloTimer());
                pClientMsg->setHelloTimer(pMsg->getHelloTimer());
                break;
            case PIM_CFG_NBR_TO_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_NBR_TO_CMD, val = ")
                    + pMsg->getNbrTimeout());
                pClientMsg->setNbrTo(pMsg->getNbrTimeout());
                break;
            case PIM_CFG_INACT_TIMER_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_INACT_TIMER_CMD, val = ")
                    + pMsg->getInactivityTimer());
                pClientMsg->setInactTimer(pMsg->getInactivityTimer());
                break;
            case PIM_CFG_MSG_INTV_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_MSG_INTV_CMD, val = ")
                    + pMsg->getMsgInterval());
                pClientMsg->setMsgIntv(pMsg->getMsgInterval());
                break;
            case PIM_CFG_SPT_THOLD_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_SPT_THOLD_CMD, val = ")
                    + pMsg->getSptThreshold());
                pClientMsg->setSptThold(pMsg->getSptThreshold());
                break;
            default:
                trace(TRACE_LEVEL_ERROR, string("PimWorker::sendPimRtrCfgStep:")
                    + string("Unrecognized cmdCode: ") + pMsg->getCmdCode());
        }

        /* Set the command code to be passed to the backend daemon */
        pClientMsg->setCmdCode(pMsg->getCmdCode());
        
        /* Set the message type to be passed to the backend daemon */
        pClientMsg->setMsgType(pMsg->getMsgType());

        /* Send the message to the backend daemon */
        status = sendSynchronouslyToWaveClient("pim", pClientMsg);
        if(status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep") +
                string(": Message sent successfully to backend daemon"));
            
            /* Get the status returned by the backend daemon code */
            status = pClientMsg->getClientStatus();
            pMsg->setCompletionStatus(status);
        }
        else
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep") +
                string(": Error while sending message to backend daemon"));

        delete pClientMsg;
        return(status);
    }

    /* Function to update the router PIM cfg MO fields */
    ResourceId
    PimWorker::pimRtrCfgFillMO(PimRtrCfgLocalManagedObject *pMo,
        PimRtrInstMessage *pMsg, bool enable_val)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO") +
            string(": Entered...."));
            
        switch(pMsg->getCmdCode()) {
            case PIM_CFG_RTR_PIM_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO:")
                    + string("PIM_CFG_RTR_PIM_CMD, val = ")
                    + pMsg->getVrfId());
                pMo->setPresenceFlag(enable_val);
                pMo->setVrfId(pMsg->getVrfId());
                /* Set the user configurable fields explicitly. This is required
                 * since the code checks for the user configured field to return
                 * the value stored in the MO */
                pMo->setMsgInterval(pMsg->getMsgInterval(), PIM_TRUE);
                pMo->setMaxMcache(pMsg->getMaxMcache(), PIM_TRUE);

                /* Set the default value for SPT when the MO is created for
                 * the first time */
                pMo->setSptThresholdInt(pMsg->getSptThreshold());
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO:")
                    + string("PIM_CFG_RTR_PIM_CMD, spt = ")
                    + pMsg->getSptThreshold());
                break;
            case PIM_CFG_MAX_MCACHE_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO:")
                    + string("PIM_CFG_MAX_MCACHE_CMD, val = ")
                    + pMsg->getMaxMcache());
				/* We always set user_config to true as even for a delete message
                 * the MO will be updated with the default value */
                pMo->setMaxMcache(pMsg->getMaxMcache(), PIM_TRUE);
                break;
            case PIM_CFG_HELLO_TIMER_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO:")
                    + string("PIM_CFG_HELLO_TIMER_CMD, val = ")
                    + pMsg->getHelloTimer());
                pMo->setHelloTimer(pMsg->getHelloTimer());
                break;
            case PIM_CFG_NBR_TO_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO:")
                    + string("PIM_CFG_NBR_TO_CMD, val = ")
                    + pMsg->getNbrTimeout());
                pMo->setNbrTimeout(pMsg->getNbrTimeout());
                break;
            case PIM_CFG_INACT_TIMER_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO:")
                    + string("PIM_CFG_INACT_TIMER_CMD, val = ")
                    + pMsg->getInactivityTimer());
                pMo->setInactivityTimer(pMsg->getInactivityTimer());
                break;
            case PIM_CFG_MSG_INTV_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgFillMO:")
                    + string("PIM_CFG_MSG_INTV_CMD, val = ")
                    + pMsg->getMsgInterval());
				/* We always set user_config to true as even for a delete message
                 * the MO will be updated with the default value */
                pMo->setMsgInterval(pMsg->getMsgInterval(), PIM_TRUE);
                break;
            case PIM_CFG_SPT_THOLD_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimRtrCfgStep:")
                    + string("PIM_CFG_SPT_THOLD_CMD, val = ")
                    + pMsg->getSptThreshold());
                /* If user executed ``no'' cmd, set the spt value to default, 
                 * else set it based on its type */
                if(pMsg->getMsgType() == PIM_MSG_DEL)
                    pMo->setSptThresholdInt(pMsg->getSptThreshold());
                else if(pMsg->getSptThreshold() == PIM_SPT_VAL_INFINITY)
                    pMo->setSptThresholdInf(pMo->mapSptIntToEnumVal(
                        pMsg->getSptThreshold()));
                else
                    pMo->setSptThresholdInt(pMsg->getSptThreshold());
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimWorker::pimRtrCfgFillMO:") +
                    string("Unrecognized cmdCode: ") +
                    pMsg->getCmdCode());
                status = WAVE_MESSAGE_ERROR;
        }
        
        /* Now mark the MO to be updated in the DB */
        if(status == WAVE_MESSAGE_SUCCESS)
            status = pimRtrCfgMarkMoForUpd(pMo, pMsg);
            
        return(status);
    }
    
    /* This function will mark the MO for updation if required */
    ResourceId
    PimWorker::pimRtrCfgMarkMoForUpd(PimRtrCfgLocalManagedObject *pMo,
        PimRtrInstMessage *pMsg)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        /* If we recvd ``no router pim'' cmd, then delete the MO and dont 
         * mark it for updation. For all other no cmds and config cmds, mark 
         * the MO for updation */

		trace(TRACE_LEVEL_DEBUG,
			string("command code in pim local worker") + 
			pMsg->getCmdCode() + PIM_CFG_RTR_PIM_CMD + 
			pMsg->getMsgType() + PIM_MSG_DEL);

        if(pMsg->getCmdCode() == PIM_CFG_RTR_PIM_CMD) {
            if(pMsg->getMsgType() == PIM_MSG_DEL) {
                delete pMo;
				PimIntfCfgMessage *pPimIntfCfgMessage =  new PimIntfCfgMessage();
				pPimIntfCfgMessage->setCmdCode(pMsg->getCmdCode());
				pPimIntfCfgMessage->setMsgType(pMsg->getMsgType());
				
				trace(TRACE_LEVEL_DEBUG, string("sending the message to nsm"));
				if((status = sendSynchronously(pPimIntfCfgMessage))== WAVE_MESSAGE_SUCCESS)	{
					trace (TRACE_LEVEL_DEBUG, string("PimWorker::pimRtrCfgMarkMoForUpd : No router pim message is sent to Nsm from Pim Local successfully:"));
				}
				delete pPimIntfCfgMessage;
				if (WAVE_MESSAGE_SUCCESS == status)
               		 status = PIM_CLEAR_PRESULTS;
            }
            else
                updateWaveManagedObject(pMo);
        }
        else
            updateWaveManagedObject(pMo);
            
        return(status);
    }
    
    /* PIM router configuration update DB */
    ResourceId
    PimWorker::updPimRtrCfgDbStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        int status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimRtrCfgDbStep") +
            string(": Entered...."));

        PimRtrCfgLocalManagedObject *pMo = NULL;
		LocationId locId = FrameworkToolKit::getThisLocationId();
        PimRtrInstMessage *pMsg = dynamic_cast<PimRtrInstMessage *>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);

        /* Query the database and get the MO if it exists */
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(PimRtrCfgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = 
					querySynchronouslyLocalManagedObjectsForLocationId(locId, &syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimRtrCfgDbStep:") +
                    string("Query resulted in no results"));

                /* Create a new MO since we didnt find one */
                pMo = new PimRtrCfgLocalManagedObject(
                          dynamic_cast<PimLocalObjectManager*>(getPWaveObjectManager()));
                PIM_PLUG_ASSERT_PTR (pMo)
            } else if (1 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimRtrCfgDbStep:") +
                    string("Query result success"));
                pMo  = dynamic_cast<PimRtrCfgLocalManagedObject*>((*pResults)[0]);
                PIM_PLUG_ASSERT_PTR (pMo);
            } else {
                status = WAVE_MESSAGE_ERROR;
            }

            /* Update the fields of the MO if we were able to find a MO */
              if(status == WAVE_MESSAGE_SUCCESS) {
//              trace (TRACE_LEVEL_DEVEL, "PimWorker::updPimRtrCfgDbStep:" +
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimRtrCfgDbStep:")
                    + string("Updating PIM router cfg MO "));
                switch(pMsg->getMsgType()) {
                    case PIM_MSG_ADD:
                        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimRtrCfgDbStep:")
                            + string(" Recvd add message") );
                        /* Call the function to update the fields of the MO */
                        pimRtrCfgFillMO(pMo, pMsg, PIM_TRUE);
                        break;
                    case PIM_MSG_UPD:
                        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimRtrCfgDbStep:")
                            + string(" Recvd update message") );
                        break;
                    case PIM_MSG_DEL:
                        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimRtrCfgDbStep:")
                            + string(" Recvd delete message") );
                        /* Call the function to update the fields of the MO */
                        if(pimRtrCfgFillMO(pMo, pMsg, PIM_FALSE) == 
                            PIM_CLEAR_PRESULTS) {
                                pResults->clear();
                        }
                        break;
                    default:
                        trace(TRACE_LEVEL_ERROR,
                            string("PimWorker::updPimRtrCfgDbStep: ") +
                            string("Unrecognized MsgType: ") +
                            pMsg->getMsgType());
                }
            }

            pSeqCtx->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;

            if(0 == numberOfResults)
                pSeqCtx->addManagedObjectForGarbageCollection(pMo);
        }

        return(status);
    }

    /* Function to update the ip multicast routing MO fields */
    void
    PimWorker::pimIpMultMOAdd(MultRtrCfgLocalManagedObject *pMo, PimMultRtrCfgMessage *pMsg)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimIpMultMOAdd") +
            string(": Entered...."));

        switch(pMsg->getCmdCode()) {
            case PIM_CFG_IP_MULT_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimIpMultMOAdd: ")
                    + string("PIM_CFG_IP_MULT_CMD, val = ")
                    + pMsg->getEnable());
                pMo->setPresenceFlag(PIM_TRUE);
                pMo->setEnable(pMsg->getEnable());
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimWorker::pimIpMultMOAdd: ") +
                    string("Unrecognized cmdCode: ") +
                    pMsg->getCmdCode());
        }

        return;
    }

    /* Function to update the ip multicast routing MO fields */
    ResourceId
    PimWorker::pimIpMultMODel(MultRtrCfgLocalManagedObject *pMo, PimMultRtrCfgMessage *pMsg)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimIpMultMODel") +
            string(": Entered...."));

        int status = WAVE_MESSAGE_SUCCESS;

        switch(pMsg->getCmdCode()) {
            case PIM_CFG_IP_MULT_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimIpMultMODel: ")
                    + string("PIM_CFG_IP_MULT_CMD, val = ")
                    + pMsg->getEnable());
                pMo->setPresenceFlag(PIM_FALSE);
                pMo->setEnable(pMsg->getEnable());
                delete pMo;
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimWorker::pimIpMultMODel: ") +
                    string("Unrecognized cmdCode: ") +
                    pMsg->getCmdCode());
                status = WAVE_MESSAGE_ERROR;
        }

        return(status);
    }

    /* PIM ip multicast-routing configuration daemon call */
    ResourceId
    PimWorker::sendPimIpMultCfgStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
		bool is_pim_supported_platform;
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimIpMultCfgStep") +
            string(": Entered...."));

        // Check for Layer3 license agaiin before sending to backend
        LAYER_3_LICENSE_CHECK();

        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform){
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }

        PimMultRtrCfgMessage *pMsg = dynamic_cast<PimMultRtrCfgMessage *>
                                      (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimWorker:sendPimIpMultCfgStep:") +
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }

        int status = WAVE_MESSAGE_SUCCESS;
        /* Create the client message and populate its fields */
        PimIpMultCfgClientMsg *pClientMsg = new PimIpMultCfgClientMsg();
        switch(pMsg->getCmdCode()) {
            case PIM_CFG_IP_MULT_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimIpMultCfgStep:")
                    + string("PIM_CFG_IP_MULT_CMD: val = ")
                    + pMsg->getEnable());
                pClientMsg->setEnable(pMsg->getEnable());
                break;
            default:
                trace(TRACE_LEVEL_ERROR, string("PimWorker::sendPimIpMultCfgStep:")
                    + string("Unrecognized cmdCode: ") + pMsg->getCmdCode());
        }

        /* Send the message to the backend daemon */
        status = sendSynchronouslyToWaveClient("pim", pClientMsg);
        if(status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimIpMultCfgStep") +
                string(": Message sent successfully to backend daemon"));
            /* Get the status returned by the backend daemon code */
            status = pClientMsg->getClientStatus();
            pMsg->setCompletionStatus(status);
         } else
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimIpMultCfgStep") +
                string(": Error while sending message to backend daemon"));

        delete pClientMsg;
        return(status);
    }

    /* PIM ip multicast-routing configuration update DB */
    ResourceId
    PimWorker::updPimIpMultCfgDbStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        int status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimIpMultCfgDbStep") +
            string(": Entered...."));

        MultRtrCfgLocalManagedObject *pMo = NULL;
		LocationId locId = FrameworkToolKit::getThisLocationId();
        PimMultRtrCfgMessage *pMsg = dynamic_cast<PimMultRtrCfgMessage*>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);

        /* Query the database and get the MO if it exists */
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(MultRtrCfgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = 
					querySynchronouslyLocalManagedObjectsForLocationId(locId, &syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimIpMultCfgDbStep: ") +
                    string("Query resulted in no results"));

                /* Create a new MO since we didnt find one */
                pMo = new MultRtrCfgLocalManagedObject(
                          dynamic_cast<PimLocalObjectManager*>(getPWaveObjectManager()));
                PIM_PLUG_ASSERT_PTR (pMo);
            } else if (1 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimIpMultCfgDbStep: ") +
                    string("Query result success"));
                pMo  = dynamic_cast<MultRtrCfgLocalManagedObject*>((*pResults)[0]);
                PIM_PLUG_ASSERT_PTR (pMo);
            } else {
                status = WAVE_MESSAGE_ERROR;
            }

            /* Update the fields of the MO if we were able to find a MO */
            if(status == WAVE_MESSAGE_SUCCESS) {
                trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimIpMultCfgDbStep: ")
                    + string("Updating PIM ip multicast router cfg MO "));
                switch(pMsg->getMsgType()) {
                    case PIM_MSG_ADD:
                        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimIpMultCfgDbStep:")
                            + string(" Recvd add message") );
                        /* Call the Add function to update the fields of the MO */
                        pimIpMultMOAdd(pMo, pMsg);

                        /* Now mark the MO to be updated in the DB */
                        updateWaveManagedObject (pMo);
                        break;
                    case PIM_MSG_UPD:
                        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimIpMultCfgDbStep:")
                            + string(" Recvd update message") );
                        break;
                    case PIM_MSG_DEL:
                        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimIpMultCfgDbStep:")
                            + string(" Recvd delete message") );
                        if(pimIpMultMODel(pMo, pMsg) == WAVE_MESSAGE_SUCCESS) {
                            pResults->clear();
                        }
                        break;
                    default:
                        trace(TRACE_LEVEL_ERROR,
                            string("PimWorker::updPimIpMultCfgDbStep: ") +
                            string("Unrecognized MsgType: ") +
                            pMsg->getMsgType());
                }
            }

            pSeqCtx->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;

            if(0 == numberOfResults)
                pSeqCtx->addManagedObjectForGarbageCollection(pMo);
        }

        return(status);
    }

    /* PIM static RP configuration daemon call */
    ResourceId
    PimWorker::sendPimStaticRpCfgStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
		bool is_pim_supported_platform;
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimStaticRpCfgStep") +
            string(": Entered...."));

        // Check for Layer3 license agaiin before sending to backend
        LAYER_3_LICENSE_CHECK();

        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform){
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }

        int status = WAVE_MESSAGE_SUCCESS;
        PimStaticRpCfgMessage *pMsg = dynamic_cast<PimStaticRpCfgMessage *>
                                      (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimWorker:sendPimStaticRpCfgStep:") +
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }

        /* Create the client message and populate its fields */
        PimStaticRpCfgClientMsg *pClientMsg = new PimStaticRpCfgClientMsg();
        pClientMsg->setRpAddr(pMsg->getRpAddr());
        pClientMsg->setPrefixList(pMsg->getPrefixList());
        pClientMsg->setMsgType(pMsg->getMsgType());

        /* Send the message to the backend daemon */
        status = sendSynchronouslyToWaveClient("pim", pClientMsg);
        if(status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimStaticRpCfgStep") +
                string(": Message sent successfully to backend daemon"));
            
            /* Get the status returned by the backend daemon code */
            status = pClientMsg->getClientStatus();
            pMsg->setCompletionStatus(status);
        }
        else
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimStaticRpCfgStep") +
                string(": Error while sending message to backend daemon"));

        delete pClientMsg;
        return(status);
    }
    
    /* Function to update the static RP MO fields */
    void
    PimWorker::pimStaticRpMOAdd(StaticRpLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, PimStaticRpCfgMessage *pMsg, 
        int addInVector)
    {
        int str_len = 0;
        bool usr_cfg = PIM_FALSE;
        
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimStaticRpMOAdd") +
            string(": Entered...."));

        /* Set the fields of the MO */
        pMo->setRpAddr(pMsg->getRpAddr());
        str_len = strlen((pMsg->getPrefixList()).c_str());
        if(str_len != 0)
            usr_cfg = PIM_TRUE;
        pMo->setPrefixList(pMsg->getPrefixList(), usr_cfg);
        
		/* Add the static RP MO to the PIM router MO for add operation*/
		if(addInVector) {
	        pMo->setOwnerManagedObjectId(pPimRtrMo->getObjectId());
    	    addToComposition(PimRtrCfgLocalManagedObject::getClassName(), 
				StaticRpLocalManagedObject::getClassName(), "StaticRpCfg", 
				pPimRtrMo->getObjectId(), pMo->getObjectId());
		} else { /* For update only update the child MO */
		  	updateWaveManagedObject (pMo);
		}
        
        return;
    }

    /* Function to delete the static RP MO */
    void
    PimWorker::pimStaticRpMODel(StaticRpLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, PimStaticRpCfgMessage *pMsg)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimStaticRpMODel") +
            string(": Entered...."));

        
        /* Mark the parent MO for updation */
        deleteFromComposition(PimRtrCfgLocalManagedObject::getClassName(), 
			StaticRpLocalManagedObject::getClassName(), "StaticRpCfg", 
			pMo->getOwnerManagedObjectId(), pMo->getObjectId());
        
        return;
    }
    
    /* PIM static RP configuration update DB */
    ResourceId
    PimWorker::updPimStaticRpCfgDbStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimStaticRpCfgDbStep") +
            string(": Entered...."));

        PimRtrCfgLocalManagedObject *pPimRtrMo = NULL;
        StaticRpLocalManagedObject *pMo = NULL;
		string MOAttr = "StaticRpCfg";
        int status = WAVE_MESSAGE_SUCCESS;
        IpV4Address rpAddr;
        PimStaticRpCfgMessage *pMsg = dynamic_cast<PimStaticRpCfgMessage*>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);
        
        /* Get a pointer to the PIM router MO */
        pPimRtrMo = getPimRtrCfgMo(pSeqCtx, MOAttr);
        PIM_PLUG_ASSERT_PTR (pPimRtrMo);
        
        rpAddr = pMsg->getRpAddr();
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimStaticRpCfgDbStep: ") +
            string("RP Addr = ") + rpAddr.toString());
        
        /* Get a pointer to the Static RP MO if it exists */
        pMo = pPimRtrMo->getStaticRpMo(rpAddr);
        
        if(pMo == NULL) {
            trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimStaticRpCfgDbStep: ") +
                string("Static RP MO not found"));

            /* Create a new MO since we didnt find one */
            pMo = new StaticRpLocalManagedObject(
                      dynamic_cast<PimLocalObjectManager*>(getPWaveObjectManager()));
            PIM_PLUG_ASSERT_PTR (pMo);
        }
        else {
            trace (TRACE_LEVEL_DEBUG,
                string("PimWorker::updPimStaticRpCfgDbStep: ") +
                string("Static RP MO exists"));
        }
        
        trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimStaticRpCfgDbStep: ")
            + string("Updating PIM Static RP cfg MO "));
        switch(pMsg->getMsgType()) {
            case PIM_MSG_ADD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimStaticRpCfgDbStep:")
                    + string(" Recvd add message") );
                /* Call the Add function to update the fields of the MO */
                pimStaticRpMOAdd(pMo, pPimRtrMo, pMsg, PIM_TRUE);
                break;
            case PIM_MSG_UPD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimStaticRpCfgDbStep:")
                    + string(" Recvd update message") );
                /* Call the update function to update the fields of the MO */
                pimStaticRpMOAdd(pMo, pPimRtrMo, pMsg, PIM_FALSE);
                break;
            case PIM_MSG_DEL:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimStaticRpCfgDbStep:")
                    + string(" Recvd delete message") );
                pimStaticRpMODel(pMo, pPimRtrMo, pMsg);
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimWorker::updPimStaticRpCfgDbStep: ") +
                    string("Unrecognized MsgType: ") +
                    pMsg->getMsgType());
        }
                
        if(pPimRtrMo) {
            pSeqCtx->addManagedObjectForGarbageCollection(pPimRtrMo);
		}
		if(pMo && (pMsg->getMsgType() == PIM_MSG_ADD)) {
			pSeqCtx->addManagedObjectForGarbageCollection(pMo);
		}
						

        return(status);
    }
    
    /* PIM RP Candidate Prefix configuration daemon call */
    ResourceId
    PimWorker::sendPimCandRpPrefixCfgStep
				(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimCandRpPrefixCfgStep")
			 + string(": Entered...."));

        int status = WAVE_MESSAGE_SUCCESS;
        PimRpCandidatePrefixCfgMessage *pMsg = 
			dynamic_cast<PimRpCandidatePrefixCfgMessage *>
            (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimWorker:sendPimCandRpPrefixCfgStep:") +
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }

        /* Create the client message and populate its fields */
        PimCandRpPrefixCfgClientMsg *pClientMsg = 
					new PimCandRpPrefixCfgClientMsg();
        pClientMsg->setPrefixIP(pMsg->getPrefixIP());
        pClientMsg->setPrefixLen(pMsg->getPrefixLen());
        pClientMsg->setMsgType(pMsg->getMsgType());
		pClientMsg->setAction(pMsg->getAction());

		// Check for Layer3 license agaiin before sending to backend
        LAYER_3_LICENSE_CHECK();

        /* Send the message to the backend daemon */
        status = sendSynchronouslyToWaveClient("pim", pClientMsg);
        if(status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, 
				string("PimWorker::sendPimCandRpPrefixCfgStep") +
                string(": Message sent successfully to backend daemon"));
            
            /* Get the status returned by the backend daemon code */
            status = pClientMsg->getClientStatus();
            pMsg->setCompletionStatus(status);
        }
        else
            trace(TRACE_LEVEL_DEBUG, 
				string("PimWorker::sendPimCandRpPrefixCfgStep") +
                string(": Error while sending message to backend daemon"));

        delete pClientMsg;
        return(status);
    }
    
    /* Function to update the RP Candidate Prefix MO fields */
    void
    PimWorker::pimCandRpPrefixMOAdd(RpCandidatePrefixLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, PimRpCandidatePrefixCfgMessage
		 *pMsg, int addInVector)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimCandRpPrefixMOAdd") +
            string(": Entered...."));

        /* Set the fields of the MO */
		pMo->setAction(pMo->mapPrefixActionToEnumVal(pMsg->getAction()));
        pMo->setPrefixIP(pMsg->getPrefixIP());
        pMo->setPrefixLen(pMsg->getPrefixLen());
        
		/* Add the static RP MO to the PIM router MO for add operation*/
		if(addInVector) {
	        pMo->setOwnerManagedObjectId(pPimRtrMo->getObjectId());
    	    addToComposition(PimRtrCfgLocalManagedObject::getClassName(), 
				RpCandidatePrefixLocalManagedObject::getClassName(), 
				"RpCandidatePrefixCfg", pPimRtrMo->getObjectId(), pMo->getObjectId());
		} else { /* For update only update the child MO */
		  	updateWaveManagedObject (pMo);
		}
        
        return;
    }

    /* Function to delete the RP Candidate Prefix MO */
    void
    PimWorker::pimCandRpPrefixMODel(RpCandidatePrefixLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, 
		PimRpCandidatePrefixCfgMessage *pMsg)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimCandRpPrefixMODel") +
            string(": Entered...."));
        
        /* Mark the parent MO for updation */
        deleteFromComposition(PimRtrCfgLocalManagedObject::getClassName(), 
			RpCandidatePrefixLocalManagedObject::getClassName(), "RpCandidatePrefixCfg",
			pPimRtrMo->getOwnerManagedObjectId(), pMo->getObjectId());
        
        return;
    }
	
	/* PIM RP Candidate Prefix MO validation */
	ResourceId
    PimWorker::validatePimCandRpPrefixCfgDbStep
			(PrismSynchronousLinearSequencerContext *pSeqCtx)
	{
	  	bool is_pim_supported_platform;
		trace(TRACE_LEVEL_DEBUG, 
			string("PimWorker::validatePimCandRpPrefixCfgDbStep") +
            string(": Entered...."));

        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform){
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }

        PimRtrCfgLocalManagedObject *pPimRtrMo = NULL;
        RpCandidatePrefixLocalManagedObject *pMo = NULL;
		string MOAttr = "RpCandidatePrefixCfg";
        int status = WAVE_MESSAGE_SUCCESS;
        IpV4Address prefixIP;
        //UI8 prefixAction;
        UI16 prefixLen;
        PimRpCandidatePrefixCfgMessage *pMsg = 
			dynamic_cast<PimRpCandidatePrefixCfgMessage*>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);

		if(pMsg->getMsgType() == PIM_MSG_DEL){
        
			/* Get a pointer to the PIM router MO */
			pPimRtrMo = getPimRtrCfgMo(pSeqCtx, MOAttr);
			PIM_PLUG_ASSERT_PTR (pPimRtrMo);

	        //prefixAction = pMsg->getAction();
		    prefixIP = pMsg->getPrefixIP();
	        prefixLen = pMsg->getPrefixLen();
			trace(TRACE_LEVEL_DEBUG, 
				string("PimWorker::validatePimCandRpPrefixCfgDbStep: ") +
				string("Prefix IP = ") + prefixIP.toString());
	        trace(TRACE_LEVEL_DEBUG, 
				string("PimWorker::validatePimCandRpPrefixCfgDbStep: ") +
		        string("Prefix Length = ") + prefixLen);

			/* Get a pointer to the RP candidate Prefix MO if it exists */
			pMo = pPimRtrMo->getCandRpPrefixMo(prefixIP, prefixLen);
			if(pMo == NULL) {
				trace (TRACE_LEVEL_DEBUG, 
					string("PimWorker::validatePimCandRpPrefixCfgDbStep: ") +
					string("RP Candidate Prefix MO not found for delete"));
				status = WRC_PIM_CLI_ERR_RP_CAND_PREFIX_NOTFOUND;
		    }
		}

        /*
		if(pPimRtrMo)
            pSeqCtx->addManagedObjectForGarbageCollection(pPimRtrMo);
		if(pMo)
			pSeqCtx->addManagedObjectForGarbageCollection(pMo);
		*/
        return(status);

	}
    
    /* PIM RP Candidate Prefix configuration update DB */
    ResourceId
    PimWorker::updPimCandRpPrefixCfgDbStep
		(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        trace(TRACE_LEVEL_DEBUG, 
			string("PimWorker::updPimCandRpPrefixCfgDbStep") +
            string(": Entered...."));

        PimRtrCfgLocalManagedObject *pPimRtrMo = NULL;
        RpCandidatePrefixLocalManagedObject *pMo = NULL;
		string MOAttr = "RpCandidatePrefixCfg";
        int status = WAVE_MESSAGE_SUCCESS;
        IpV4Address prefixIP;
		UI8 prefixAction;
		UI16 prefixLen;
        PimRpCandidatePrefixCfgMessage *pMsg = 
			dynamic_cast<PimRpCandidatePrefixCfgMessage*>
                     (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);
        
        /* Get a pointer to the PIM router MO */
        pPimRtrMo = getPimRtrCfgMo(pSeqCtx, MOAttr);
        PIM_PLUG_ASSERT_PTR (pPimRtrMo);
        
		prefixAction = pMsg->getAction();
        prefixIP = pMsg->getPrefixIP();
		prefixLen = pMsg->getPrefixLen();
		trace(TRACE_LEVEL_DEBUG, 
			string("PimWorker::updPimCandRpPrefixCfgDbStep: ") +
            string("Prefix action = ") + prefixAction);
        trace(TRACE_LEVEL_DEBUG, 
			string("PimWorker::updPimCandRpPrefixCfgDbStep: ") +
            string("Prefix IP = ") + prefixIP.toString());
		trace(TRACE_LEVEL_DEBUG, 
			string("PimWorker::updPimCandRpPrefixCfgDbStep: ") +
            string("Prefix Length = ") + prefixLen);
        
        /* Get a pointer to the RP candidate Prefix MO if it exists */
        pMo = pPimRtrMo->getCandRpPrefixMo(prefixIP, prefixLen);
        
        if(pMo == NULL) {
            trace (TRACE_LEVEL_DEBUG, 
				string("PimWorker::updPimCandRpPrefixCfgDbStep: ") +
                string("RP Candidate Prefix MO not found"));

            /* Create a new MO since we didnt find one */
            pMo = new RpCandidatePrefixLocalManagedObject(
                      dynamic_cast<PimLocalObjectManager*>
						(getPWaveObjectManager()));
            PIM_PLUG_ASSERT_PTR (pMo);
        }
        else {
            trace (TRACE_LEVEL_DEBUG,
                string("PimWorker::updPimCandRpPrefixCfgDbStep: ") +
                string("RP Candidate Prefix MO exists"));
        }
        
        trace (TRACE_LEVEL_DEBUG, 
			string("PimWorker::updPimCandRpPrefixCfgDbStep: ")
            + string("Updating PIM RP Candidate Prefix cfg MO "));
        switch(pMsg->getMsgType()) {
            case PIM_MSG_ADD:
                trace(TRACE_LEVEL_DEBUG, 
					string("PimWorker::updPimCandRpPrefixCfgDbStep:")
                    + string(" Recvd add message") );
                /* Call the Add function to update the fields of the MO */
                pimCandRpPrefixMOAdd(pMo, pPimRtrMo, pMsg, PIM_TRUE);
                break;
            case PIM_MSG_UPD:
                trace(TRACE_LEVEL_DEBUG, 
					string("PimWorker::updPimCandRpPrefixCfgDbStep:")
                    + string(" Recvd update message") );
                /* Call the update function to update the fields of the MO */
                pimCandRpPrefixMOAdd(pMo, pPimRtrMo, pMsg, PIM_FALSE);
                break;
            case PIM_MSG_DEL:
                trace(TRACE_LEVEL_DEBUG, 
					string("PimWorker::updPimCandRpPrefixCfgDbStep:")
                    + string(" Recvd delete message") );
                pimCandRpPrefixMODel(pMo, pPimRtrMo, pMsg);
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimWorker::updPimCandRpPrefixCfgDbStep: ") +
                    string("Unrecognized MsgType: ") +
                    pMsg->getMsgType());
        }
                
        if(pPimRtrMo)
            pSeqCtx->addManagedObjectForGarbageCollection(pPimRtrMo);

        if(pMo && (pMsg->getMsgType() == PIM_MSG_ADD)) {
            pSeqCtx->addManagedObjectForGarbageCollection(pMo);
        }

        return(status);
    }
 

  /* PIM candidate RP configuration daemon call */
    ResourceId
    PimWorker::sendPimCandRpCfgStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
		bool is_pim_supported_platform;
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimCandRpCfgStep") +
            string(": Entered...."));

        // Check for Layer3 license agaiin before sending to backend
        LAYER_3_LICENSE_CHECK();

        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform){
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }

        int status = WAVE_MESSAGE_SUCCESS;
        PimRpCandidateCfgMessage *pMsg = dynamic_cast<PimRpCandidateCfgMessage *>
                                      (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimWorker:sendPimCandRpCfgStep:") +
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }

        /* Create the client message and populate its fields */
        PimCandRpCfgClientMsg *pClientMsg = new PimCandRpCfgClientMsg();
        pClientMsg->setIntfType(pMsg->getIntfType());
        pClientMsg->setIntfName(pMsg->getIntfName());
        pClientMsg->setPrefixList(pMsg->getPrefixList());
        pClientMsg->setMsgType(pMsg->getMsgType());

        /* Send the message to the backend daemon */
        status = sendSynchronouslyToWaveClient("pim", pClientMsg);
        if(status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimCandRpCfgStep") +
                string(": Message sent successfully to backend daemon"));
            
            /* Get the status returned by the backend daemon code */
            status = pClientMsg->getClientStatus();
            pMsg->setCompletionStatus(status);

            /* Update the MO even if the interface is not PIM enabled. A 
             * warning message will be displayed to the user */
            if(status == WRC_PIM_CLI_WARN_CAND_NOTACTV) {
                startTransaction();
                updPimCandRpCfgDbStep(pSeqCtx);

                if(commitTransaction() == WAVE_MESSAGE_SUCCESS)
                    trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimCandRpCfgStep") +
                        string(": RP candidate cfg commited to DB"));
                else
                    trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimCandRpCfgStep") +
                        string(": RP candidate cfg commit failed with status = ")
                        + status);
            }
        }
        else
            trace(TRACE_LEVEL_DEBUG, string("PimWorker::sendPimCandRpCfgStep") +
                string(": Error while sending message to backend daemon"));

        delete pClientMsg;
        return(status);
    }

    /* Function to update the candidate RP MO fields */
    void
    PimWorker::pimCandRpMOAdd(RpCandidateLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, PimRpCandidateCfgMessage *pMsg, 
        int addInVector)
    {
        int str_len = 0;
        bool usr_cfg = PIM_FALSE;
        int32_t slotId = 0;
        
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimCandRpMOAdd") +
            string(": Entered...."));

        /* Set the fields of the MO */
        pMo->setIntfType(pMo->mapIntfTypeToEnumVal(pMsg->getIntfType()));
        pMo->setIntfName(pMsg->getIntfName());
        
        str_len = strlen((pMsg->getPrefixList()).c_str());
        if(str_len != 0)
            usr_cfg = PIM_TRUE;
        pMo->setPrefixList(pMsg->getPrefixList(), usr_cfg);
        
        /* Associate the MO with the slot ID */
        slotId = pimExtractSlot(pMsg->getIntfName(), pMsg->getIntfType());
        pMo->associateWithSlot(slotId);
        
		/* Add the RP candidate MO to the PIM router MO for add operation*/
		if(addInVector) {		  
	        pMo->setOwnerManagedObjectId(pPimRtrMo->getObjectId());
    	    addToComposition(PimRtrCfgLocalManagedObject::getClassName(), 
				RpCandidateLocalManagedObject::getClassName(), "RpCandidateCfg", 
				pPimRtrMo->getObjectId(), pMo->getObjectId());
		} else { /* For update operation just update the child MO */
		  	updateWaveManagedObject (pMo);
		}
        
        return;
    }

    /* Function to delete the candidate RP MO */
    void
    PimWorker::pimCandRpMODel(RpCandidateLocalManagedObject *pMo, 
        PimRtrCfgLocalManagedObject *pPimRtrMo, PimRpCandidateCfgMessage *pMsg)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::pimCandRpMODel") +
            string(": Entered...."));

        
        /* Mark the parent MO for updation */
        deleteFromComposition(PimRtrCfgLocalManagedObject::getClassName(), 
			RpCandidateLocalManagedObject::getClassName(), "RpCandidateCfg", 
			pMo->getOwnerManagedObjectId(), pMo->getObjectId());
        
        return;
    }
    
    /* PIM candidate RP configuration update DB */
    ResourceId
    PimWorker::updPimCandRpCfgDbStep(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimCandRpCfgDbStep") +
            string(": Entered...."));

        PimRtrCfgLocalManagedObject *pPimRtrMo = NULL;
        RpCandidateLocalManagedObject *pMo = NULL;
		string MOAttr = "RpCandidateCfg";
        int status = WAVE_MESSAGE_SUCCESS;
        UI8 intfType;
        string intfName;
        PimRpCandidateCfgMessage *pMsg = dynamic_cast<PimRpCandidateCfgMessage*>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);
        
        /* Get a pointer to the PIM router MO */
        pPimRtrMo = getPimRtrCfgMo(pSeqCtx, MOAttr);
        PIM_PLUG_ASSERT_PTR (pPimRtrMo);
        
        intfType = pMsg->getIntfType();
        intfName = pMsg->getIntfName();
        trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimCandRpCfgDbStep: ") +
            string("Intf Type = ") + pimGetIntfTypeStr(intfType) + 
            string(" Intf Name = ") + intfName);
        
        /* Get a pointer to the RP candidate MO if it exists */
        pMo = pPimRtrMo->getCandRpMo(intfType, intfName);
        
        if(pMo == NULL) {
            trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimCandRpCfgDbStep: ") +
                string("Candidate RP MO not found"));

            /* Create a new MO since we didnt find one */
            pMo = new RpCandidateLocalManagedObject(
                      dynamic_cast<PimLocalObjectManager*>(getPWaveObjectManager()));
            PIM_PLUG_ASSERT_PTR (pMo);
        }
        else {
            trace (TRACE_LEVEL_DEBUG,
                string("PimWorker::updPimCandRpCfgDbStep: ") +
                string("Candidate RP MO exists"));
        }
        
        trace (TRACE_LEVEL_DEBUG, string("PimWorker::updPimCandRpCfgDbStep: ")
            + string("Updating PIM candidate RP cfg MO "));
        switch(pMsg->getMsgType()) {
            case PIM_MSG_ADD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimCandRpCfgDbStep:")
                    + string(" Recvd add message") );
                /* Call the Add function to update the fields of the MO */
                pimCandRpMOAdd(pMo, pPimRtrMo, pMsg, PIM_TRUE);
                break;
            case PIM_MSG_UPD:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimCandRpCfgDbStep:")
                    + string(" Recvd update message") );
                /* Call the update function to update the fields of the MO */
                pimCandRpMOAdd(pMo, pPimRtrMo, pMsg, PIM_FALSE);
                break;
            case PIM_MSG_DEL:
                trace(TRACE_LEVEL_DEBUG, string("PimWorker::updPimCandRpCfgDbStep:")
                    + string(" Recvd delete message") );
                pimCandRpMODel(pMo, pPimRtrMo, pMsg);
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimWorker::updPimCandRpCfgDbStep: ") +
                    string("Unrecognized MsgType: ") +
                    pMsg->getMsgType());
        }
    
        if(pPimRtrMo)
            pSeqCtx->addManagedObjectForGarbageCollection(pPimRtrMo);

        if(pMo && (pMsg->getMsgType() == PIM_MSG_ADD)) {
            pSeqCtx->addManagedObjectForGarbageCollection(pMo);
        }

        return(status);
    }
}
