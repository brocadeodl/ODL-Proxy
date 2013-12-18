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
 *   Author : Rahul Jain                                                   *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Pim/PimIntfCfgLocalManagedObject.h"
#include "Nsm/Local/Pim/PimIntfCfgMessage.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Pim/PimLocalIntfWorker.h"
#include "ClientInterface/Pim/PimIntfCfgClientMsg.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "Nsm/Local/NsmTypes.h"
#include "Utils/Layer3LicenseUtil.h"
#include "ClientInterface/Pim/PimUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"

namespace DcmNs
{

    PimLocalIntfWorker::PimLocalIntfWorker (NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
	    PimIntfCfgLocalManagedObject PimIntfCfgLocalManagedObject(pNsmLocalObjectManager);
        PimIntfCfgLocalManagedObject.setupOrm ();
        addManagedClass (PimIntfCfgLocalManagedObject::getClassName (), this);

        addOperationMap (NSMLOCALPIMINTFMSG, 
            reinterpret_cast<PrismMessageHandler>
            (&PimLocalIntfWorker::NsmLocalPimIntfCfgMessageHandler));
    }

    PimLocalIntfWorker::~PimLocalIntfWorker ()
    {
    }

    PrismMessage *PimLocalIntfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMLOCALPIMINTFMSG :
                pPrismMessage = new PimIntfCfgMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject *PimLocalIntfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PimIntfCfgLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PimIntfCfgLocalManagedObject(
                dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace(TRACE_LEVEL_FATAL, 
                string("PimLocalIntfWorker::createManagedObjectInstance :") +
                string(" Unknown Managed Class Name : ") + managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId PimLocalIntfWorker::updatePIMNoRtrPimConfigSessionMO(PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        UI32 MOCount = 0;
        vector<string> selectFields;
		LocationId locId = FrameworkToolKit::getThisLocationId();
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        WaveManagedObjectPointer<PimIntfCfgLocalManagedObject> pimMO;
        vector<WaveManagedObject* >* pL3MOResults;
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        selectFields.push_back("pimsmCfg");
        WaveManagedObjectSynchronousQueryContext SyncQueryCtxt
          (NsmL3ReadyManagedObject::getClassName());
        SyncQueryCtxt.addSelectFields(selectFields);
        pL3MOResults = querySynchronouslyLocalManagedObjectsForLocationId(locId, &SyncQueryCtxt);
        MOCount = pL3MOResults->size ();

        for (UI32 i = 0; i < MOCount; i++) {
            pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pL3MOResults)[i]);
            pimMO = pL3ReadyMo->getPimSmCfg();
            if(pimMO.operator->() == NULL)
                continue;
            pL3ReadyMo->addAttributeToBeUpdated("pimsmCfg");
            pL3ReadyMo->setPimSmCfg(NULL);
            updateWaveManagedObject(pL3ReadyMo);

        }

        if(pSeqCtx)
            pSeqCtx->addManagedObjectsForGarbageCollection(*pL3MOResults);
        if (pL3MOResults)
            delete pL3MOResults;
        return status;
    }


	void  PimLocalIntfWorker::NsmLocalPimIntfCfgMessageHandler(PimIntfCfgMessage *pPimIntfLocalMsg)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            /* Validate operation vs. DB */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimLocalIntfWorker::validatePimIntfCfgStep),
            /* Send message to backend daemon */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimLocalIntfWorker::sendPimPhyIntfCfgStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimLocalIntfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            /* Update the DB with MO information */
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimLocalIntfWorker::updPimPhyIntfCfgDBStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimLocalIntfWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimLocalIntfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimLocalIntfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = 
            new DceSynchronousLinearSeqContext(pPimIntfLocalMsg, this,
                sequencerSteps, 
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
    
    /******************************************************/
    /*                 Utility functions                  */
    /******************************************************/
    
    PimIntfCfgLocalManagedObject* 
    PimLocalIntfWorker::getPimIntfCfgLclMo(const NsmL3ReadyManagedObject *pL3RdyMo)
    {
        //PIM_PLUG_ASSERT_PTR(pL3RdyMo);
        //WaveWorker::prismAssert((pL3RdyMo) != NULL, __FILE__, __LINE__);

        return((WaveManagedObjectPointer<PimIntfCfgLocalManagedObject>
            (pL3RdyMo->getPimSmCfg())).operator->());
    }
    
    NsmL3ReadyManagedObject *
    PimLocalIntfWorker::pimGetL3RdyMo(PrismSynchronousLinearSequencerContext *pSeqCtx,
        PimIntfCfgMessage *pMsg)
    {
        string ifName;
        UI8 ifType;
        NsmL3ReadyManagedObject *pL3RdyMo = NULL;
        SI32 mapId = DcmToolKit::getLocalMappedId();
        
        PIM_PLUG_ASSERT_PTR(pMsg);
        
        ifName = pMsg->getIntfName();
        ifType = pMsg->getIntfType();
        
        trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::pimGetL3RdyMo:") + 
            string("Searching for interface ") + pimGetIntfTypeStr(ifType) +
            string(" ") + ifName);
        switch(ifType) {
            case INTERFACE_TENGIG:
            case INTERFACE_ONEGIG:
            case INTERFACE_FORTYGIG:
                pL3RdyMo = NsmL3Util::PhyIfNameGetL3ReadyMO(ifName, pSeqCtx, 
                               PIM_TRUE);
                break;
            case INTERFACE_VLAN:
                pL3RdyMo = NsmL3Util::VlanIdGetL3ReadyMO(toUI32(ifName),
                               mapId, pSeqCtx, PIM_FALSE);
                break;
            case INTERFACE_PORT_CHANNEL:
                pL3RdyMo = NsmL3Util::PoIdGetL3ReadyMO(toUI32(ifName),
                               mapId, pSeqCtx, PIM_FALSE);
                break;
            default:
                trace(TRACE_LEVEL_ERROR, 
                    string("PimLocalIntfWorker::pimGetL3RdyMo:") +
                    string("Unrecognized interface type (") + ifType + 
                    string(")"));
                break;
        }
        
        return(pL3RdyMo);
    }
    
    /******************************************************/
    /*        Daemon call and update DB functions         */
    /******************************************************/
    
    /* Function to validate operation versus DB (needed for WyserEa read optimizations) */
    ResourceId 
    PimLocalIntfWorker::validatePimIntfCfgStep(DceSynchronousLinearSeqContext *pSeqCtx)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        NsmL3ReadyManagedObject *pL3RdyMo = NULL;
        PimIntfCfgLocalManagedObject *pMo = NULL;
        bool is_pim_supported_platform;
        UI8 ifType;
        bool phyIntf=false, veIntf=false, poIntf=false;
        vector<string> selectField;
       
        /* feature support check depending on  platform */
        is_pim_supported_platform = Pim_Utils::PimVerifyPlatformSupport();
        if(!is_pim_supported_platform) {
            return WRC_PIM_CLI_ERR_NOT_PIM_SUPPORTED_PLATFORM;
        }
 
        trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::validatePimIntfCfgStep") +
            string(": Entered....\n"));

        PimIntfCfgMessage *pMsg = dynamic_cast<PimIntfCfgMessage *>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);

        // VRF check
        ifType = pMsg->getIntfType();
        switch(ifType) {
            case INTERFACE_TENGIG:
            case INTERFACE_ONEGIG:
            case INTERFACE_FORTYGIG:
                phyIntf = true;
                break;
            case INTERFACE_VLAN:
                veIntf = true;
                break;
            case INTERFACE_PORT_CHANNEL:
                poIntf = true;
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimLocalIntfWorker::validationStep:") +
                    string("Unrecognized interface"));
                break;
        }

        if(phyIntf) {

            selectField.push_back("vrfid");
            selectField.push_back("phyType");
            selectField.push_back("layer3Ready");
            selectField.push_back("swModeFlag");

            PhyIntfLocalManagedObject *phyIntfMO =
                NsmL3Util::PhyIfNameGetPhyIfLocalMO(pMsg->getIntfName(), pSeqCtx, true, &selectField);

            if(phyIntfMO != NULL) {
			    if(phyIntfMO->getVrfObjectId() != ObjectId::NullObjectId) {				  
                	trace (TRACE_LEVEL_ERROR, "validationStep:VRF is configured on this interface");
                	status = WRC_PIM_CLI_VRF_CONFIGURED_INTERFACE_ERROR;
                	pMsg->setCompletionStatus(status);
					pSeqCtx->addManagedObjectForGarbageCollection(phyIntfMO);
					return (status);
				}
				pSeqCtx->addManagedObjectForGarbageCollection(phyIntfMO);
            }

        }

        if(veIntf) {

            selectField.push_back("vrfid");
            selectField.push_back("layer3Ready");
            LocationId locationId = FrameworkToolKit::getThisLocationId ();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfSviManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute(new AttributeUI32(toUI32(pMsg->getIntfName()), "vlanId"));
            vector<WaveManagedObject *> *pResults =
                                            querySynchronouslyLocalManagedObjectsForLocationId (locationId, &syncQueryCtxt);
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0 || numberOfResults > 1) {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                status =  WRC_NSM_INTERFACE_DOES_NOT_EXIST;
                pMsg->setCompletionStatus(status);
                return (status);
            }

            NsmIntfSviManagedObject *sviMO = dynamic_cast<NsmIntfSviManagedObject*>((*pResults)[0]);
            if (sviMO != NULL) {
                //pSeqCtx->getCache()->put(NsmIntfSviManagedObject::getClassName(), sviMO, true);
                if(sviMO->getVrfObjectId() != ObjectId::NullObjectId) {
                    trace (TRACE_LEVEL_ERROR, "validationStep:VRF is configured on this interface");
                    status = WRC_PIM_CLI_VRF_CONFIGURED_INTERFACE_ERROR;
                    pMsg->setCompletionStatus(status);
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
					return (status);
                }
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }

        }

        if(poIntf) {
            // TODO ...
        }

        /* If "no router pim" is configured do the following */ 
        if((PIM_CFG_RTR_PIM_CMD == pMsg->getCmdCode()) && (PIM_MSG_DEL == pMsg->getMsgType()))
        {
            return(WAVE_MESSAGE_SUCCESS);
        }

        pL3RdyMo = pimGetL3RdyMo(pSeqCtx, pMsg);
        if (!pL3RdyMo) {
            return WRC_PIM_CLI_ERR_NO_IF;
        }
                
        pSeqCtx->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pL3RdyMo);
        //Ensure that this cache object is garbage collected
        pSeqCtx->addManagedObjectForGarbageCollection(pL3RdyMo);

        /* Get a pointer to the PIM interface MO if it exists */
        pMo = getPimIntfCfgLclMo(pL3RdyMo);
        if (NULL == pMo) {
            trace (TRACE_LEVEL_DEBUG, 
                string("PimLocalIntfWorker::validatePimIntfCfgStep:") +
                string(" PIM intf MO does not exist"));

            if(pMsg->getMsgType() == PIM_MSG_DEL) {
                trace(TRACE_LEVEL_INFO, "PimLocalIntfWorker::validatePimIntfCfgStep: Delete for nonexistent object");

                // To prevent a request to delete a nonexistent config from causing any issues (which is needed for
                // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
            }

        } else {
            trace (TRACE_LEVEL_DEBUG, 
                string("PimLocalIntfWorker::validatePimIntfCfgStep:") +
                string(" PIM intf MO exists"));

            if(pMsg->getMsgType() == PIM_MSG_ADD) {

                if(pMsg->getCmdCode() == PIM_INTF_CFG_PIM_SP_CMD) {
                    trace(TRACE_LEVEL_INFO, "PimLocalIntfWorker::validatePimIntfCfgStep: Create for existing object");

                    // To prevent a request to create an already-existing config from causing any issues (which is needed for
                    // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                    status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                }
                else if((pMsg->getCmdCode() == PIM_INTF_CFG_MULT_ACL_CMD) && pMo->getMultBoundaryAcl()) {
                    trace(TRACE_LEVEL_INFO, "PimLocalIntfWorker::validatePimIntfCfgStep: multicast boundary already enabled");
                    // To suppress a backend warning when enabling multicast-boundary when it's already enabled (which results
                    // from Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                    status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                }
            }
        }
        
        return(status);
    }

    /* Function to send message to PIM daemon */
    ResourceId 
    PimLocalIntfWorker::sendPimPhyIntfCfgStep(DceSynchronousLinearSeqContext *pSeqCtx)
	{
        trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::sendPimPhyIntfCfgStep") + 
            string(": Entered...."));
	
        string ifname;	
        PimIntfCfgMessage *pMsg = dynamic_cast<PimIntfCfgMessage *>
                                      (pSeqCtx->getPPrismMessage());
        if(!pMsg) {
            trace(TRACE_LEVEL_DEBUG, 
                string("PimLocalIntfWorker::sendPimPhyIntfCfgStep") + 
                string("Unable to get Prism Context from input param"));
            return WAVE_MESSAGE_ERROR;
        }
        
        int status = WAVE_MESSAGE_SUCCESS;
		/* Just Return success if "no router pim" is configured */
		
		trace(TRACE_LEVEL_DEBUG, string("\nPimLocalIntfWorker::sendPimPhyIntfCfgStep Entered..\n") +
								string("PIM_CFG_RTR_PIM_CMD:") + PIM_CFG_RTR_PIM_CMD + 
								string("pMsg:") + pMsg->getCmdCode() + string("\nPIM_MSG_DEL :") + 
								PIM_MSG_DEL + string("pMsg :") + pMsg->getMsgType());
		if((PIM_CFG_RTR_PIM_CMD == pMsg->getCmdCode()) && (PIM_MSG_DEL == pMsg->getMsgType()))	{
				return(status);
		}
 
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();

        /* Create the client message and populate its fields */
        PimIntfCfgClientMsg *pClientMsg = new PimIntfCfgClientMsg();

        /* Pass 2 tuple name to backend if we are in VCS mode */
        if(NsmUtils::isValidThreeTuple(pMsg->getIntfName())) {
            NsmUtils::getTwoTupleIfName(pMsg->getIntfName(), ifname);
        }
        else {
            ifname = pMsg->getIntfName();
        }

        pClientMsg->setIntfName(ifname);
        pClientMsg->setIntfType(pMsg->getIntfType());
        switch(pMsg->getCmdCode()) {
            case PIM_INTF_CFG_PIM_SP_CMD:
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalIntfWorker::sendPimPhyIntfCfgStep:") +
                    string("PIM_INTF_CFG_PIM_SP_CMD, val = ") +
                    pMsg->getEnable());
                pClientMsg->setEnable(pMsg->getEnable());
                break;
            case PIM_INTF_CFG_DR_PRIO_CMD:
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalIntfWorker::sendPimPhyIntfCfgStep:") +
                    string("PIM_INTF_CFG_DR_PRIO_CMD, val = ") +
                    pMsg->getDrPriority());
                pClientMsg->setDrPriority(pMsg->getDrPriority());
                break;
            case PIM_INTF_CFG_PIM_BRDR_CMD:
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalIntfWorker::sendPimPhyIntfCfgStep:") +
                    string("PIM_INTF_CFG_PIM_BRDR_CMD, val = ") +
                    pMsg->getPimBorder());
                pClientMsg->setPimBorder(pMsg->getPimBorder());
                break;
            case PIM_INTF_CFG_MULT_ACL_CMD:
				{
                	trace(TRACE_LEVEL_DEBUG, 
                    	string("PimLocalIntfWorker::sendPimPhyIntfCfgStep:") +
	                    string("PIM_INTF_CFG_MULT_ACL_CMD, val = ") +
    	                pMsg->getMultBoundaryAcl());
					if(PIM_MSG_DEL == pMsg->getMsgType())
						pClientMsg->setMultBoundaryAcl(false);
					else
        	        	pClientMsg->setMultBoundaryAcl(pMsg->getMultBoundaryAcl());
            	    break;
				}
            case PIM_INTF_CFG_TTL_THOLD_CMD:
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalIntfWorker::sendPimPhyIntfCfgStep:") +
                    string("PIM_INTF_CFG_TTL_THOLD_CMD, val = ") +
                    pMsg->getTtlThold());
                pClientMsg->setTtlThold(pMsg->getTtlThold());
                break;
            default:
                trace(TRACE_LEVEL_ERROR, 
                    string("PimLocalIntfWorker::sendPimPhyIntfCfgStep:") +
                    string("Unrecognized cmdCode: ") + pMsg->getCmdCode());
        }
        
        /* Set the command code to be passed to the backend daemon */
        pClientMsg->setCmdCode(pMsg->getCmdCode());

        /* Send the message to the backend daemon */
        status = sendSynchronouslyToWaveClient("pim", pClientMsg);
        if(status == WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, 
                string("PimLocalIntfWorker::sendPimPhyIntfCfgStep") +
                string(": Message sent successfully to backend daemon"));

            /* Get the status returned by the backend daemon code */
            status = pClientMsg->getClientStatus();
            pMsg->setCompletionStatus(status);
        }
        else
            trace(TRACE_LEVEL_DEBUG, 
                string("PimLocalIntfWorker::sendPimPhyIntfCfgStep") +
                string(": Error while sending message to backend daemon"));

        delete pClientMsg;
        
        return(status);
	}

    /* Function to update the ip multicast routing MO fields */
    void
    PimLocalIntfWorker::pimIntfFillMO(PimIntfCfgLocalManagedObject *pMo, 
        NsmL3ReadyManagedObject *pL3RdyMo, PimIntfCfgMessage *pMsg, 
        bool enable_val)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::pimIntfFillMO") +
            string(": Entered...."));

        switch(pMsg->getCmdCode()) {
            case PIM_INTF_CFG_PIM_SP_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::pimIntfFillMO: ")
                    + string("PIM_INTF_CFG_PIM_SP_CMD, val = ")
                    + pMsg->getEnable());
                pMo->setEnabled(pMsg->getEnable());
                break;
            case PIM_INTF_CFG_DR_PRIO_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::pimIntfFillMO: ")
                    + string("PIM_INTF_CFG_DR_PRIO_CMD, val = ")
                    + pMsg->getDrPriority());
                pMo->setDrPriority(pMsg->getDrPriority(), enable_val);
                break;
            case PIM_INTF_CFG_PIM_BRDR_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::pimIntfFillMO: ")
                    + string("PIM_INTF_CFG_PIM_BRDR_CMD, val = ")
                    + pMsg->getPimBorder());
                pMo->setPimBorder(pMsg->getPimBorder(), enable_val);
                break;
            case PIM_INTF_CFG_MULT_ACL_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::pimIntfFillMO: ")
                    + string("PIM_INTF_CFG_MULT_ACL_CMD, val = ")
                    + pMsg->getMultBoundaryAcl());
                pMo->setMultBoundaryAcl(pMsg->getMultBoundaryAcl(), enable_val);
                break;
            case PIM_INTF_CFG_TTL_THOLD_CMD:
                trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::pimIntfFillMO: ")
                    + string("PIM_INTF_CFG_TTL_THOLD_CMD, val = ")
                    + pMsg->getTtlThold());
                pMo->setTtlThold(pMsg->getTtlThold(), enable_val);
                break;
            default:
                trace(TRACE_LEVEL_ERROR,
                    string("PimLocalIntfWorker::pimIntfFillMO: ") +
                    string("Unrecognized cmdCode: ") +
                    pMsg->getCmdCode());
                status = WAVE_MESSAGE_ERROR;
        }

        /* Mark the MO for updation */
        if(status == WAVE_MESSAGE_SUCCESS)
            pimIntfCfgMarkMoForUpd(pMo, pL3RdyMo, pMsg);
        return;
    }    
    
    /* This function will mark the MO for updation if required */
    void
    PimLocalIntfWorker::pimIntfCfgMarkMoForUpd(PimIntfCfgLocalManagedObject *pMo, 
        NsmL3ReadyManagedObject *pL3RdyMo, PimIntfCfgMessage *pMsg)
    {
        /* If we recvd ``no ip pim-sparse'' cmd, then delete the MO and dont 
         * mark it for updation. For all other no cmds and config cmds, mark 
         * the MO for updation */
        if(pMsg->getCmdCode() == PIM_INTF_CFG_PIM_SP_CMD) {
            if(pMsg->getMsgType() == PIM_MSG_DEL) {
                /* Clear the link between PIM intf MO and L3 ready MO */
                pL3RdyMo->setPimSmCfg(NULL);
                /* Now mark the L3 ready MO for updation */ 
                updateWaveManagedObject (pL3RdyMo);
            }
            else
                updateWaveManagedObject(pMo);
        }
        else
            updateWaveManagedObject(pMo);
            
        return;
    }
    
    /* Function to update DB with MO information */
    ResourceId 
    PimLocalIntfWorker::updPimPhyIntfCfgDBStep(DceSynchronousLinearSeqContext *pSeqCtx)
	{
        int status = WAVE_MESSAGE_SUCCESS;
        string ifName;
        NsmL3ReadyManagedObject *pL3RdyMo = NULL;
        PimIntfCfgLocalManagedObject *pMo = NULL;
        
        trace(TRACE_LEVEL_DEBUG, string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep") +
            string(": Entered....\n"));

        PimIntfCfgMessage *pMsg = dynamic_cast<PimIntfCfgMessage *>
                                      (pSeqCtx->getPPrismMessage());
        PIM_PLUG_ASSERT_PTR (pMsg);
		
		/* If "no router pim" is configured do the following */ 

		trace(TRACE_LEVEL_DEBUG, string("PIM_CFG_RTR_PIM_CMD :") + PIM_CFG_RTR_PIM_CMD + 
								string("pMsg:") + pMsg->getCmdCode() +
								string("PIM_MSG_DEL=") +	PIM_MSG_DEL + 
								string("pMsg:") + pMsg->getMsgType() + string("\n"));       
		if((PIM_CFG_RTR_PIM_CMD == pMsg->getCmdCode()) && (PIM_MSG_DEL == pMsg->getMsgType()))
			{
				status = updatePIMNoRtrPimConfigSessionMO(pSeqCtx);	
				return(status);
			}

        ifName = pMsg->getIntfName();
        pL3RdyMo = dynamic_cast<NsmL3ReadyManagedObject *>
            (pSeqCtx->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
        PIM_PLUG_ASSERT_PTR(pL3RdyMo);
                
        /* Get a pointer to the PIM interface MO if it exists */
        pMo = getPimIntfCfgLclMo(pL3RdyMo);
        if (NULL == pMo) {
            trace (TRACE_LEVEL_DEBUG, 
                string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep:") +
                string(" PIM intf MO does not exist"));

            /* Create a new MO since we didnt find one */
            pMo = new PimIntfCfgLocalManagedObject(
                      dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
            PIM_PLUG_ASSERT_PTR (pMo);
                
            /* Create the link between PIM intf MO and L3 ready MO */
            pL3RdyMo->setPimSmCfg(pMo);
            /* Now mark the L3 ready MO for updation */ 
            updateWaveManagedObject (pL3RdyMo);
        } else {
            trace (TRACE_LEVEL_DEBUG, 
                string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep:") +
                string(" PIM intf MO exists"));
        }
        
        /* Update the fields of the MO if we were able to find a MO */
        trace (TRACE_LEVEL_DEBUG, 
            string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep:") +
            string(" Updating PIM interface cfg MO "));
        switch(pMsg->getMsgType()) {
            case PIM_MSG_ADD:
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep:")
                    + string(" Recvd add message"));
                pimIntfFillMO(pMo, pL3RdyMo, pMsg, PIM_TRUE);
                break;
            case PIM_MSG_UPD:
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep:")
                    + string(" Recvd update message"));
                break;
            case PIM_MSG_DEL:
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep:")
                    + string(" Recvd delete message"));
                pimIntfFillMO(pMo, pL3RdyMo, pMsg, PIM_FALSE);
                break;
            default:
                trace (TRACE_LEVEL_ERROR, 
                    string("PimLocalIntfWorker::updPimPhyIntfCfgDBStep:") 
                    + string(" Unrecognized MsgType: ")
                    + pMsg->getMsgType());
                break;
        }

        return(status);
	}
}
