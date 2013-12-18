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
 *   Author : rprashar                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/PVlanLocalConfWorker.h"
#include "Nsm/Local/NsmPVlanTrunkAssocPhyMessage.h"
#include "Nsm/Local/NsmPVlanMappingPhyMessage.h"
#include "Nsm/Local/NsmPVlanTrunkAssocPoSPMessage.h"
#include "Nsm/Local/NsmPVlanMappingPoSPMessage.h"

#include "Nsm/Local/PVlanTrunkAssocManagedObject.h"
#include "Nsm/Local/PVlanMappingManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/NsmVlanProvisionLocalMessage.h"
#include "Nsm/Local/VLAN/GvlanConfigUtils.h"
#include "Nsm/Local/VLAN/VlanForPoPhyIntfView.h"
#include "Nsm/Local/VLAN/VlanIdForPoPhyIntfView.h"
#include "Nsm/Local/VLAN/VlanRangeForPoPhyIntfView.h"

#include "Nsm/Common/PVlanUtils.h"

#include "ClientInterface/Nsm/NsmClientPVlanTrunkAssocMessage.h"
#include "ClientInterface/Nsm/NsmClientPVlanMappingMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/PVlanCommon.h"

#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "DcmCStatus.h"
#include <iostream>
#include <string>
#include "Nsm/Local/CtagBasedVlanClassificationMO.h"


namespace DcmNs
{
    PVlanLocalConfWorker::PVlanLocalConfWorker(NsmLocalObjectManager * pNsmLocalObjectManager):
        WaveWorker(pNsmLocalObjectManager)
    {
        PVlanTrunkAssocManagedObject PVlanTrunkAssocManagedObject(pNsmLocalObjectManager);
        PVlanTrunkAssocManagedObject.setupOrm();
        addManagedClass(PVlanTrunkAssocManagedObject::getClassName(), this);

        PVlanMappingManagedObject  PVlanMappingManagedObject(pNsmLocalObjectManager);
        PVlanMappingManagedObject .setupOrm();
        addManagedClass(PVlanMappingManagedObject ::getClassName(), this);

        VlanIdForPoPhyIntfView vlanIdForPhyIntfView(pNsmLocalObjectManager);
        vlanIdForPhyIntfView.setupOrm();
        addManagedClass(VlanIdForPoPhyIntfView::getClassName(), this);

        VlanRangeForPoPhyIntfView vlanRangeForPoPhyIntfView(pNsmLocalObjectManager);
        vlanRangeForPoPhyIntfView.setupOrm();
        addManagedClass(VlanRangeForPoPhyIntfView::getClassName(), this);

        VlanForPoPhyIntfView vlanForPhyIntfView(pNsmLocalObjectManager);
        vlanForPhyIntfView.setupOrm();
        addManagedClass(VlanForPoPhyIntfView::getClassName(), this);

        addOperationMap(NSMPVLANTRUNKASSOCTOPHY, reinterpret_cast<PrismMessageHandler>(&PVlanLocalConfWorker::NsmPVlanTrunkAssocPhyMessageHandler));
        addOperationMap(NSMPVLANTRUNKASSOCTOPOSP, reinterpret_cast<PrismMessageHandler>(&PVlanLocalConfWorker::NsmPVlanTrunkAssocPoSPMessageHandler));


        addOperationMap(NSMPVLANMAPPINGTOPHY, reinterpret_cast<PrismMessageHandler>(&PVlanLocalConfWorker::NsmPVlanMappingPhyMessageHandler));
        addOperationMap(NSMPVLANMAPPINGTOPOSP, reinterpret_cast<PrismMessageHandler>(&PVlanLocalConfWorker::NsmPVlanMappingPoSPMessageHandler));

        addOperationMap(NSM_LOCAL_VLAN_PROVISION_MSG, reinterpret_cast<PrismMessageHandler>(&PVlanLocalConfWorker::GvlanProvisionMessageHandler));
    }
    PVlanLocalConfWorker::~PVlanLocalConfWorker()
    {
    }
    PrismMessage * PVlanLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage* pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMPVLANTRUNKASSOCTOPHY:
                pPrismMessage = new NsmPVlanTrunkAssocPhyMessage();
                break;

            case NSMPVLANTRUNKASSOCTOPOSP:
                pPrismMessage = new NsmPVlanTrunkAssocPoSPMessage();
                break;

            case NSMPVLANMAPPINGTOPHY:
                pPrismMessage = new NsmPVlanMappingPhyMessage();
                break;

            case NSMPVLANMAPPINGTOPOSP:
                pPrismMessage = new NsmPVlanMappingPoSPMessage();
                break;

            case NSM_LOCAL_VLAN_PROVISION_MSG:
                pPrismMessage = new NsmVlanProvisionLocalMessage();
                break;

            default:
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
    WaveManagedObject * PVlanLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject* pWaveManagedObject = NULL;

        if ((PVlanTrunkAssocManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new PVlanTrunkAssocManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        }
        else if ((PVlanMappingManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new PVlanMappingManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        }
        else if ((VlanIdForPoPhyIntfView::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new VlanIdForPoPhyIntfView(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        }
        else if ((VlanRangeForPoPhyIntfView::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new VlanRangeForPoPhyIntfView(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        }
        else if ((VlanForPoPhyIntfView::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new VlanForPoPhyIntfView(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        }
        else
        {
            trace(TRACE_LEVEL_INFO, "PVlanLocalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }

        return (pWaveManagedObject);
    }


    ResourceId PVlanLocalConfWorker::validateTrunkAssocOnPhyStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext) {

    	trace(TRACE_LEVEL_DEVEL,"Entering PVlanLocalConfWorker :: validateTrunkAssocOnPhyStep ... ");

    	NsmPVlanTrunkAssocPhyMessage *pNsmPVlanTrunkAssocPhyMessage  =
    			dynamic_cast<NsmPVlanTrunkAssocPhyMessage *>(pDceSynchronousLinearSeqContext ->getPPrismMessage());
    	ResourceId  status   = WAVE_MESSAGE_SUCCESS;

    	if(pNsmPVlanTrunkAssocPhyMessage->getOpcode() == PVLAN_TRUNK_ASSOC_PHY){

    	status   = WAVE_MESSAGE_ERROR;
    	string ifName = pNsmPVlanTrunkAssocPhyMessage->getIfName();
    	vector<string> selectFields;
        selectFields.push_back ("swModeFlag");
        selectFields.push_back ("active");
        selectFields.push_back ("pVlanModeTrunkPromis");
        selectFields.push_back ("pVlanModeTrunkBas");

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    	syncQueryCtxt.addSelectFields(selectFields);
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                 status = WAVE_MESSAGE_ERROR;
             }else if (1 == numberOfResults) {
             pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                 status = WAVE_MESSAGE_SUCCESS;
             } else {
                 status = WAVE_MESSAGE_ERROR;
            }
			pResults->clear();
			delete pResults;
         }

    	trace(TRACE_LEVEL_INFO,string("PVlanLocalConfWorker validateTrunkAssocOnPhyStep : ")+ pNsmPVlanTrunkAssocPhyMessage ->getOpcode());

    	if (pPhyIntfLocalManagedObject == NULL || status != WAVE_MESSAGE_SUCCESS) {
    		trace(TRACE_LEVEL_INFO,string("pPhyIntfLocalManagedObject is NULL: ")+ pNsmPVlanTrunkAssocPhyMessage ->getOpcode());
    		return WAVE_MESSAGE_ERROR;
    	} else {

            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);
            NSM_PLUG_INFO_S( string ("UP_PHY_L Validating validateTrunkAssocOnPhyStep : ") + ifName + string("OpCode :") + pNsmPVlanTrunkAssocPhyMessage->getOpcode());
			if(false == pPhyIntfLocalManagedObject->getActive())
			{
				return WRC_NSM_INTERFACE_NOT_IN_SERVICE;
			}

			if(true == NsmUtils::isPhyPortInISLMode(ifName, true, pDceSynchronousLinearSeqContext) )
			{
				NSM_PLUG_DBG_S(string("port in ISL Mode Nothing to be Done") + ifName);
				return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
			}

    		trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations prim"));
    		UI32 primVId = pNsmPVlanTrunkAssocPhyMessage ->getPrimaryVlanId();
			UI32 secVId = pNsmPVlanTrunkAssocPhyMessage->getSecondaryVlanId();
    		NSM_PLUG_DBG_S("\t IN VALidations prim" + primVId);

    		//sw enabled
    		if (!pPhyIntfLocalManagedObject->getSwModeFlag()) {
    			trace(TRACE_LEVEL_ERROR,string("\t Interface not configured for switching "));
    			status = WRC_SWITCHING_NOT_ENABLED;
    		}

    		//sw mode trunk set
    		else if (!(pPhyIntfLocalManagedObject->getPvlanModeTrunkBas())){
    			trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations sw mode trunk set"));
    			status = WRC_PVLAN_MODE_NOT_TRUNK;
    		}

    		//is Primary vlan
    		else if (!PVlanUtils::isPrimaryVlan(primVId)) {
    			trace(TRACE_LEVEL_DEBUG, string("\t IN isPrimaryVlan "));
    			status = WRC_NSM_PVLAN_NOT_PRIM_VLAN;
    		}

            //Is secondary Vlan
            else if (!PVlanUtils::isSecondaryVlan(secVId))
            {
				status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
				trace(TRACE_LEVEL_INFO,string("validateTrunkAssocOnPhyStep isSecondaryVlan"));
            }

    		//doesAssociation exists
            else if (!PVlanUtils::doesAssociationExists(primVId, secVId, pDceSynchronousLinearSeqContext)){
    			trace(TRACE_LEVEL_DEBUG, string("\t IN doesAssociationExists"));
    			status = WRC_PVLAN_ASSOC_NOT_PRESENT;
    		}

			//When
			//1. The port is in private-vlan-trunk mode,
			//2. Either primary/secondary vlan >= 4096,
			//3. If the primary/secondary vlan has no ctag associated with it, return ERROR.
			else if((primVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagPVlanClassificationMO::getClassName(), primVId, "", 0, 0, pPhyIntfLocalManagedObject->getObjectId()) == 0))
			{
				trace(TRACE_LEVEL_ERROR, string("PVlanLocalConfWorker::validateTrunkAssocOnPhyStep:Primary Vlan is not associated with a ctag."));
				status = WRC_NSM_GVLAN_PRIMARY_HAVE_NO_CTAG;
			}
			else if((secVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagPVlanClassificationMO::getClassName(), secVId, "", 0, 0, pPhyIntfLocalManagedObject->getObjectId()) == 0))
			{
				trace(TRACE_LEVEL_ERROR, string("PVlanLocalConfWorker::validateTrunkAssocOnPhyStep:Secondary Vlan is not associated with a ctag."));
				status = WRC_NSM_GVLAN_SECONDARY_HAVE_NO_CTAG;
			}

    		trace(TRACE_LEVEL_INFO, string("validateTrunkAssocOnPhyStep ")+ primVId);
    	}
    	}
    	pNsmPVlanTrunkAssocPhyMessage ->setCompletionStatus(status);
     	return status;
 	}

    ResourceId PVlanLocalConfWorker::PVlanTrunkAssocOnPhyIntfStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG( "Entering ...");

        ResourceId status = 0;
		NsmPVlanTrunkAssocPhyMessage* pNsmPVlanTrunkAssocPhyMessage =
            dynamic_cast<NsmPVlanTrunkAssocPhyMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());

		NSM_PLUG_ASSERT(pNsmPVlanTrunkAssocPhyMessage);

		const UI32 primaryVlanId = pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId();
		const UI32 secondaryVlanId = pNsmPVlanTrunkAssocPhyMessage -> getSecondaryVlanId();
		PROVISION_VLAN(pDceSynchronousLinearSeqContext, primaryVlanId);

		NsmClientPVlanTrunkAssocMessage* m      = new NsmClientPVlanTrunkAssocMessage();
		NSM_PLUG_ASSERT(m);

        string                                   ifName = pNsmPVlanTrunkAssocPhyMessage -> getIfName();
		if(true  == NsmUtils::isValidThreeTuple(ifName) )
		{
			string twoTuppleIfName ;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}

        m -> setIfName(ifName);
        m -> setIfType(pNsmPVlanTrunkAssocPhyMessage->getIfType());
        m ->setPrimaryVlanId(primaryVlanId);
        m ->setSecondaryVlanId(secondaryVlanId);
        m -> setOpcode(pNsmPVlanTrunkAssocPhyMessage -> getOpcode());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        trace(TRACE_LEVEL_INFO, " PVLANLocal worker" + ifName);
        status = sendSynchronouslyToWaveClient("nsm", m);
        trace(TRACE_LEVEL_INFO, " PVLANLocal worker after send " + ifName);

        return errorCode(pNsmPVlanTrunkAssocPhyMessage, m, status);
    }

    ResourceId PVlanLocalConfWorker::PVlanTrunkAssocOnPhyIntfMOStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;

        NsmPVlanTrunkAssocPhyMessage* pNsmPVlanTrunkAssocPhyMessage =
            dynamic_cast<NsmPVlanTrunkAssocPhyMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmPVlanTrunkAssocPhyMessage);

		NsmUtils::getPhyMoByPhyName(pNsmPVlanTrunkAssocPhyMessage -> getIfName(), pPhyIntfLocalManagedObject);
		NSM_PLUG_ASSERT(pPhyIntfLocalManagedObject);


		switch(pNsmPVlanTrunkAssocPhyMessage->getOpcode())
		{

        case PVLAN_TRUNK_ASSOC_PHY :
        {
            PVlanTrunkAssocManagedObject* m = new PVlanTrunkAssocManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
    		NSM_PLUG_ASSERT(m);
    		m -> setPrimaryVlanId(pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId());
    		m -> setSecondaryVlanId(pNsmPVlanTrunkAssocPhyMessage -> getSecondaryVlanId());
    		trace (TRACE_LEVEL_DEBUG, string("PVLAN_TRUNK_ASSOC_PHY : primVid =") + pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId());
    		pPhyIntfLocalManagedObject -> addPVlanTrunkAssoc(m);
    		break;
        }

        case PVLAN_TRUNK_ASSOC_PHY_DEL :
        {
			trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_TRUNK_ASSOC_PHY_DEL prim secIds =") + pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId() + pNsmPVlanTrunkAssocPhyMessage -> getSecondaryVlanId());
			pPhyIntfLocalManagedObject->deletePVlanTrunkAssocPrimSec(pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId(),pNsmPVlanTrunkAssocPhyMessage -> getSecondaryVlanId());

            // Partial DB Update
            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
			break;
        }

        case PVLAN_TRUNK_ASSOC_PHY_DEL_PRIM :
        {
        	NSM_PLUG_DBG("PVLAN_TRUNK_ASSOC_PHY_DEL_PRIM " + pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId());
        	trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_TRUNK_ASSOC_PHY_DEL_PRIM prim Id =") + pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId());
			pPhyIntfLocalManagedObject->deletePVlanTrunkAssocPrim(pNsmPVlanTrunkAssocPhyMessage -> getPrimaryVlanId());

            // Partial DB Update
            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
			break;
        }

        case PVLAN_TRUNK_ASSOC_PHY_DEL_ALL :
        {
			trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_TRUNK_ASSOC_PHY_DEL_ALL") );
			pPhyIntfLocalManagedObject->deletePVlanTrunkAssocAll();

            // Partial DB Update
            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
			break;
        }
        default :
        {
        	NSM_PLUG_DBG("RCHD in defaultttttt ");
        	trace (TRACE_LEVEL_DEBUG, string("RCHD in default") );
        	break;
        }
		}

		// Partial DB Update not sufficient for create **
       // pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");

        updateWaveManagedObject(pPhyIntfLocalManagedObject);

        pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
        return WAVE_MESSAGE_SUCCESS;
    }
    void PVlanLocalConfWorker::NsmPVlanTrunkAssocPhyMessageHandler(NsmPVlanTrunkAssocPhyMessage * pNsmPVlanTrunkAssocPhyMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::validateTrunkAssocOnPhyStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::PVlanTrunkAssocOnPhyIntfStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::PVlanTrunkAssocOnPhyIntfMOStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
		};

		DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmPVlanTrunkAssocPhyMessage, this,
                																sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		pDceSynchronousLinearSeqContext->execute();
    }


    ResourceId PVlanLocalConfWorker::PVlanTrunkAssocOnPoSPIntfStep(DceSynchronousLinearSeqContext *pContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId status = 0;
        NsmPVlanTrunkAssocPoSPMessage* pNsmPVlanTrunkAssocPoSPMessage = dynamic_cast<NsmPVlanTrunkAssocPoSPMessage*>(pContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmPVlanTrunkAssocPoSPMessage);


        const UI32 primaryVlanId = pNsmPVlanTrunkAssocPoSPMessage -> getPrimaryVlanId();
        const UI32 secondaryVlanId = pNsmPVlanTrunkAssocPoSPMessage -> getSecondaryVlanId();
        PROVISION_VLAN(pContext, primaryVlanId)

		NsmClientPVlanTrunkAssocMessage* m      = new NsmClientPVlanTrunkAssocMessage();
		NSM_PLUG_ASSERT(m);

        m -> setIfType(IF_TYPE_PO);
        m -> setIfName(pNsmPVlanTrunkAssocPoSPMessage -> getIfName());
        m ->setPrimaryVlanId(primaryVlanId);
        m ->setSecondaryVlanId(secondaryVlanId);
        m -> setOpcode(pNsmPVlanTrunkAssocPoSPMessage -> getOpcode());

        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);

        trace(TRACE_LEVEL_INFO, " PVLANLocal worker after send  PVlanTrunkAssocOnPoSPIntfStep" + m->getIfName());

		return errorCode(pNsmPVlanTrunkAssocPoSPMessage, m, status);

    }

    void PVlanLocalConfWorker::NsmPVlanTrunkAssocPoSPMessageHandler(NsmPVlanTrunkAssocPoSPMessage * pNsmPVlanTrunkAssocPoSPMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::PVlanTrunkAssocOnPoSPIntfStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        DceSynchronousLinearSeqContext *pContext =
                new DceSynchronousLinearSeqContext(
                        pNsmPVlanTrunkAssocPoSPMessage, this, sequencerSteps,
                        sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pContext -> execute();

    }


    ResourceId PVlanLocalConfWorker::validateMappingOnPhyStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext) {

    	trace(TRACE_LEVEL_DEVEL,"Entering PVlanLocalConfWorker :: validateMappingOnPhyStep ... ");

    	NsmPVlanMappingPhyMessage *pNsmPVlanMappingPhyMessage  =
    			dynamic_cast<NsmPVlanMappingPhyMessage *>(pDceSynchronousLinearSeqContext ->getPPrismMessage());
    	ResourceId  status   = WAVE_MESSAGE_SUCCESS;

    	if(pNsmPVlanMappingPhyMessage->getOpcode() == PVLAN_MAPPING_PHY){

    	string ifName = pNsmPVlanMappingPhyMessage->getIfName();
    	status   = WAVE_MESSAGE_ERROR;

    	vector<string> selectFields;
        selectFields.push_back ("swModeFlag");
        selectFields.push_back ("active");
        selectFields.push_back ("pVlanModeTrunkPromis");
        selectFields.push_back ("pVlanModePromiscuous");
        selectFields.push_back ("pVlanMapping");

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    	syncQueryCtxt.addSelectFields(selectFields);
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                 status = WAVE_MESSAGE_ERROR;
             }else if (1 == numberOfResults) {
             pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                 status = WAVE_MESSAGE_SUCCESS;
             } else {
                 status = WAVE_MESSAGE_ERROR;
            }
			pResults->clear();
			delete pResults;
         }

    	trace(TRACE_LEVEL_INFO,string("PVlanLocalConfWorker validateMappingOnPhyStep : ")+ pNsmPVlanMappingPhyMessage ->getOpcode());

    	if (pPhyIntfLocalManagedObject == NULL || status != WAVE_MESSAGE_SUCCESS) {
    		trace(TRACE_LEVEL_INFO,string("pPhyIntfLocalManagedObject is NULL: ")+ pNsmPVlanMappingPhyMessage ->getOpcode());
    		return WAVE_MESSAGE_ERROR;
    	} else {
            prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);
            NSM_PLUG_INFO_S( string ("UP_PHY_L Validating Physical Interface MO : ") + ifName + string("Opcode :") + pNsmPVlanMappingPhyMessage->getOpcode());
			if(false == pPhyIntfLocalManagedObject->getActive())
			{

				return WRC_NSM_INTERFACE_NOT_IN_SERVICE;
			}

			if(true == NsmUtils::isPhyPortInISLMode(ifName, true, pDceSynchronousLinearSeqContext) )
			{
				NSM_PLUG_DBG_S(string("port in ISL Mode Nothing to be Done") + ifName);
				return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
			}

    		trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations prim"));
    		UI32 primVId = pNsmPVlanMappingPhyMessage ->getPrimaryVlanId();
    		NSM_PLUG_DBG_S("\t IN VALidations prim" + primVId);


    		//sw enabled
    		if (!pPhyIntfLocalManagedObject->getSwModeFlag()) {
    			trace(TRACE_LEVEL_ERROR,string("\t Interface not configured for switching "));
    			status = WRC_SWITCHING_NOT_ENABLED;
    		}

    		//is Vlaid range
    		if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmPVlanMappingPhyMessage ->getSecVlanRange())== WAVE_MESSAGE_ERROR) {
    			status = WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;
    		}

    		//sw mode trunk set
    		else if (!(pPhyIntfLocalManagedObject->getPvlanModePromiscuous() == true ||(pPhyIntfLocalManagedObject->getPvlanModeTrunkPromis() == true))) {
    			trace(TRACE_LEVEL_DEBUG, string("\t PVlanLocalConfWorker mode trunk"));
    			status = WRC_PVLAN_MODE_NOT_PROMIS;
    		}

    		//is Primary vlan
    		else if (!PVlanUtils::isPrimaryVlan(primVId)) {
    			trace(TRACE_LEVEL_DEBUG, string("\t PVlanLocalConfWorker isPrimaryVlan"));
    			status = WRC_NSM_PVLAN_NOT_PRIM_VLAN;
    		}

    		//Are all secondary Vlans
    		else if (!PVlanUtils::isSecondaryVlanRange(pNsmPVlanMappingPhyMessage ->getSecVlanRange())) {
    			status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
    			trace(TRACE_LEVEL_INFO,string("PVlanLocalConfWorker validateMappingOnPOStep 56"));
    		}

    		//doesAssociation exists

    		else if (!PVlanUtils::doesAssociationExists(primVId, pNsmPVlanMappingPhyMessage ->getSecVlanRange(), pDceSynchronousLinearSeqContext)){
    			trace(TRACE_LEVEL_DEBUG, string("\t PVlanLocalConfWorker doesAssociationExists "));
    			status = WRC_PVLAN_ASSOC_NOT_PRESENT;
    		}

    		else if(pNsmPVlanMappingPhyMessage->getoper()== PVLAN_MAPPING_ADD)
			{
    			//domain max on promiscuous
				vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > pVlanMapping = pPhyIntfLocalManagedObject->getPVlanMapping();
				if((pVlanMapping.size() == MAX_PVLAN_DOMAIN_ON_PROMISCUOS) && (!PVlanUtils::isDomainPresentOnPromiscuous(pVlanMapping, primVId)))
    			{
    				trace(TRACE_LEVEL_DEBUG, string("\t PVlanLocalConfWorker domain max on promiscuous"));
    				status = WRC_NSM_PVLAN_MAX_DOMAIN_ON_PROMISCUOS;
    			}

				//When
				//1. The port is in private-vlan-trunk-promiscuous mode,
				//2. The primary Vlan Id >= 4096,
				//3. The primary vlan should have atleast one ctag associated it, already.
				//Else, return ERROR.
				else if(pPhyIntfLocalManagedObject->getPvlanModeTrunkPromis() && (primVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagVlanClassificationMO::getClassName(), primVId, "", 0, 0, pPhyIntfLocalManagedObject->getObjectId()) == 0))
				{
					trace(TRACE_LEVEL_ERROR, string("PVlanLocalConfWorker::validateMappingOnPhyStep:Primary Vlan is not associated with a ctag."));
					status = WRC_NSM_GVLAN_PRIMARY_HAVE_NO_CTAG;
				}
			}

    		trace(TRACE_LEVEL_INFO, string("validateMappingOnPhyStep ")+ primVId);
    	}
    	}

    	pNsmPVlanMappingPhyMessage ->setCompletionStatus(status);
     	return status;
 	}

    ResourceId PVlanLocalConfWorker::PVlanMappingOnPhyIntfStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG( "Entering ...");

        ResourceId status = 0;
		NsmPVlanMappingPhyMessage* pNsmPVlanMappingPhyMessage =
            dynamic_cast<NsmPVlanMappingPhyMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());

		NSM_PLUG_ASSERT(pNsmPVlanMappingPhyMessage);

		if(true == NsmUtils::isPhyPortInISLMode(pNsmPVlanMappingPhyMessage -> getIfName(), true, pDceSynchronousLinearSeqContext) )
		{
			NSM_PLUG_DBG_S(string("port in ISL Mode Nothing to be Done") + pNsmPVlanMappingPhyMessage -> getIfName() );
			return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
		}

		if (PVLAN_MAPPING_ADD == pNsmPVlanMappingPhyMessage->getoper()) {
		    PROVISION_VLAN(pDceSynchronousLinearSeqContext,
		            pNsmPVlanMappingPhyMessage -> getPrimaryVlanId());
		}

		NsmClientPVlanMappingMessage* m      = new NsmClientPVlanMappingMessage();
		NSM_PLUG_ASSERT(m);

        string                                   ifName = pNsmPVlanMappingPhyMessage -> getIfName();
		if(true  == NsmUtils::isValidThreeTuple(ifName) )
		{
			string twoTuppleIfName ;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}

		if(pNsmPVlanMappingPhyMessage->getoper() == PVLAN_MAPPING_DEL && pNsmPVlanMappingPhyMessage-> getSecVlanRange().isEmpty()){
			UI32Range dbVlanRange = PVlanUtils::getSecPVlanRangeFromDb(pNsmPVlanMappingPhyMessage -> getPrimaryVlanId(), pDceSynchronousLinearSeqContext);
			m -> setSecVlanRange(dbVlanRange);
		} else{
			m -> setSecVlanRange(pNsmPVlanMappingPhyMessage-> getSecVlanRange());
		}

        m -> setIfName(ifName);
        m -> setIfType(pNsmPVlanMappingPhyMessage->getIfType());
        m -> setPrimaryVlanId(pNsmPVlanMappingPhyMessage -> getPrimaryVlanId());
   		m -> setoper(pNsmPVlanMappingPhyMessage->getoper());
        m -> setOpcode(pNsmPVlanMappingPhyMessage -> getOpcode());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        trace(TRACE_LEVEL_INFO, "in local worker" + ifName);

        status = sendSynchronouslyToWaveClient("nsm", m);
        trace(TRACE_LEVEL_INFO, " PVLANLocal worker after send  PVlanMappingOnPhyIntfStep " + m->getIfName());

        return errorCode(pNsmPVlanMappingPhyMessage, m, status);
    }

    ResourceId PVlanLocalConfWorker::PVlanMappingOnPhyIntfMOStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;

        NsmPVlanMappingPhyMessage* pNsmPVlanMappingPhyMessage =
            dynamic_cast<NsmPVlanMappingPhyMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmPVlanMappingPhyMessage);

		NsmUtils::getPhyMoByPhyName(pNsmPVlanMappingPhyMessage -> getIfName(), pPhyIntfLocalManagedObject);
		NSM_PLUG_ASSERT(pPhyIntfLocalManagedObject);


		switch(pNsmPVlanMappingPhyMessage->getOpcode()){

		case PVLAN_MAPPING_PHY:
		{

		PVlanMappingManagedObject* pvlanMappingMO = NULL;

		vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > pvlanMapMoVector = pPhyIntfLocalManagedObject->getPVlanMapping();

		bool isPresent = false;
		for (unsigned int j = 0; j < pvlanMapMoVector.size(); j++)
		{
			if(pvlanMapMoVector[j]-> getPrimaryVlanId() == pNsmPVlanMappingPhyMessage -> getPrimaryVlanId()){
				pvlanMappingMO = pvlanMapMoVector[j].operator-> ();
				isPresent = true;
				break;
			}
		}

		if (isPresent && pNsmPVlanMappingPhyMessage->getoper() == PVLAN_MAPPING_DEL && pNsmPVlanMappingPhyMessage-> getSecVlanRange().isEmpty()){

			trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_MAPPING_PHY_DEL_PRIM") );
			pPhyIntfLocalManagedObject->deletePVlanMappingPrim(pNsmPVlanMappingPhyMessage -> getPrimaryVlanId());
			pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanMapping");
		} else if(isPresent){

			trace (TRACE_LEVEL_DEBUG, string("OPER %d==") +  pNsmPVlanMappingPhyMessage->getoper());
			NSM_PLUG_DBG("Entry exists --  PVlanMappingOnPhyIntfMOStep ...");
			UI32Range updatedVlanRange = PVlanUtils :: getSecPVlanRange(pNsmPVlanMappingPhyMessage-> getSecVlanRange(), pvlanMappingMO->getSecPvlanRange(),(PVlanMappingOper) pNsmPVlanMappingPhyMessage->getoper());
			NSM_PLUG_DBG("Got updated range --  PVlanMappingOnPhyIntfMOStep ...");
			pvlanMappingMO -> setSecPvlanRange(updatedVlanRange);
			pvlanMappingMO -> setoper(PVLAN_MAPPING_ADD);
			updateWaveManagedObject(pvlanMappingMO);
		}else if(pNsmPVlanMappingPhyMessage->getoper() != PVLAN_MAPPING_DEL){

			pvlanMappingMO = new PVlanMappingManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
			NSM_PLUG_ASSERT(pvlanMappingMO);
			NSM_PLUG_DBG("New entry -- PVlanMappingOnPhyIntfMOStep ...");
			pvlanMappingMO -> setPrimaryVlanId(pNsmPVlanMappingPhyMessage -> getPrimaryVlanId());
			pvlanMappingMO -> setSecPvlanRange(pNsmPVlanMappingPhyMessage-> getSecVlanRange());
			pvlanMappingMO -> setoper(PVLAN_MAPPING_ADD);
			pPhyIntfLocalManagedObject -> addPVlanMapping(pvlanMappingMO);
		}

		NSM_PLUG_DBG("AFTER PVlanMappingOnPhyIntfMOStep ...");
		break;
		}

        case PVLAN_MAPPING_PHY_DEL_PRIM:
        {
        	trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_MAPPING_PHY_DEL_PRIM prim Id =") +  pNsmPVlanMappingPhyMessage -> getPrimaryVlanId());
			pPhyIntfLocalManagedObject->deletePVlanMappingPrim( pNsmPVlanMappingPhyMessage -> getPrimaryVlanId());

            // Partial DB Update
            pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanMapping");
			break;
        }

        case PVLAN_MAPPING_PHY_DEL_ALL:
        {
			trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_MAPPING_PHY_DEL_ALL") );
			pPhyIntfLocalManagedObject->deletePVlanMappingAll();
            // Partial DB Update
			pPhyIntfLocalManagedObject->addAttributeToBeUpdated("pVlanMapping");
			break;
        }

        default :
        {
        	trace (TRACE_LEVEL_DEBUG, string("RCHD in default") );
        	break;
        }
		}

		updateWaveManagedObject(pPhyIntfLocalManagedObject);
		pDceSynchronousLinearSeqContext -> addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
        return WAVE_MESSAGE_SUCCESS;
    }


    void PVlanLocalConfWorker::NsmPVlanMappingPhyMessageHandler(NsmPVlanMappingPhyMessage * pNsmPVlanMappingPhyMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::validateMappingOnPhyStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::PVlanMappingOnPhyIntfStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::PVlanMappingOnPhyIntfMOStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
		};

		DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmPVlanMappingPhyMessage, this,
                																sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		pDceSynchronousLinearSeqContext->execute();
    }


    ResourceId PVlanLocalConfWorker::PVlanMappingOnPoSPIntfStep(DceSynchronousLinearSeqContext * pDceSynchronousLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId status = 0;
        NsmPVlanMappingPoSPMessage* pNsmPVlanMappingPoSPMessage =
            dynamic_cast<NsmPVlanMappingPoSPMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmPVlanMappingPoSPMessage);

		PROVISION_VLAN(pDceSynchronousLinearSeqContext,
		        pNsmPVlanMappingPoSPMessage -> getPrimaryVlanId());

		NsmClientPVlanMappingMessage* m      = new NsmClientPVlanMappingMessage();
		NSM_PLUG_ASSERT(m);

		m -> setIfType(IF_TYPE_PO);
        m -> setIfName(pNsmPVlanMappingPoSPMessage -> getIfName());
        m ->setPrimaryVlanId(pNsmPVlanMappingPoSPMessage -> getPrimaryVlanId());
        m ->setoper(pNsmPVlanMappingPoSPMessage ->getoper());
        m -> setSecVlanRange(pNsmPVlanMappingPoSPMessage -> getSecVlanRange());
        m -> setOpcode(pNsmPVlanMappingPoSPMessage -> getOpcode());

        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);
        trace(TRACE_LEVEL_INFO, " PVLANLocal worker after send PVlanMappingOnPoSPIntfStep " + m -> getIfName());
		return errorCode(pNsmPVlanMappingPoSPMessage, m, status);

    }

    void PVlanLocalConfWorker::NsmPVlanMappingPoSPMessageHandler(NsmPVlanMappingPoSPMessage * pNsmPVlanMappingPoSPMessage)
    {
//      Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::PVlanMappingOnPoSPIntfStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PVlanLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pNsmPVlanMappingPoSPMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext -> execute();
    }


	ResourceId PVlanLocalConfWorker::errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status)
	{
		NSM_PLUG_DBG_TRACE();
		loc_msg_p -> setCompletionStatus(status);
		if(WAVE_MESSAGE_SUCCESS == status)
		{
			if(WAVE_MESSAGE_SUCCESS != loc_msg_p -> getCompletionStatus())
			{
				status = loc_msg_p -> getCompletionStatus();
			}
			else if(WAVE_MESSAGE_SUCCESS != NsmUtils::mapBackEndErrorsToDcmdErrors(c_msg_p -> getClientStatus()))
			{
				status =  NsmUtils::mapBackEndErrorsToDcmdErrors(c_msg_p -> getClientStatus());
				loc_msg_p -> setCompletionStatus(status);
				NSM_PLUG_DBG_S(string("Error From Nsm Client") + status);
			}
		}
		delete c_msg_p;
		return status;
		//PVLAN todo hk
		//return WAVE_MESSAGE_SUCCESS;
	}


	void PVlanLocalConfWorker::GvlanProvisionMessageHandler (NsmVlanProvisionLocalMessage *pMessage)
	{
	    GvlanConfigUtils gvlanUtils ( NULL );
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    if (pMessage->isTriggerVlanUnprovision()) {
	        status = gvlanUtils.triggerUnprovisionEligibleVlans();
	    }

	    if (WAVE_MESSAGE_SUCCESS == status &&
	            pMessage->hasUnprovisionVlans()) {
	        // hopefully it works :)
	        UI32Range gvids = pMessage->getUnprovisionVlans();
	        gvids -= pMessage->getProvisionVlans();

	        if (!gvids.isEmpty()) {
	            status = gvlanUtils.provision(gvids, UNPROVISION_SPCIFIED);
	        }
	    }

	    if (status == WAVE_MESSAGE_SUCCESS && pMessage->hasProvisionVlans()) {
	        status = gvlanUtils.provision(
	                pMessage->getProvisionVlans(), PROVISION_SPECIFIED,
	                pMessage->getNeedsExplicitUnprovision());
	    }

	    pMessage->setCompletionStatus(status);
	    reply(pMessage);
	}

}

