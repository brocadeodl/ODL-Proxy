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



#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sstream>

//FrameWork
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/StringUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCStatus.h"

//Global
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/PVlanGlobalConfWorker.h"
#include "Nsm/Global/NsmGlobalPVlanTrunkAssocPoMessage.h"
#include "Nsm/Global/NsmGlobalPVlanMappingPoMessage.h"

#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"

//Local
#include "Nsm/Local/NsmPVlanTrunkAssocPoSPMessage.h"
#include "Nsm/Local/NsmPVlanMappingPoSPMessage.h"
#include "Nsm/Local/PVlanTrunkAssocManagedObject.h"
#include "Nsm/Local/PVlanMappingManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"

#include "Nsm/Common/PVlanUtils.h"
#include "Utils/DceLinearSeqContext.h"

//ClientInterface
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/PVlanCommon.h"
#include "Utils/DceClusterUtils.h"
#include "Nsm/Local/CtagBasedVlanClassificationMO.h"

namespace DcmNs
{
    PVlanGlobalConfWorker::PVlanGlobalConfWorker(NsmGlobalObjectManager * pNsmGlobalObjectManager):
        WaveWorker(pNsmGlobalObjectManager)
    {
        addOperationMap(NSMGLOBALPVLANTRUNKASSOCTOPO, reinterpret_cast<PrismMessageHandler>(&PVlanGlobalConfWorker::NsmGlobalPVlanTrunkAssocPoMessageHandler));
        addOperationMap(NSMGLOBALPVLANMAPPINGTOPO, reinterpret_cast<PrismMessageHandler>(&PVlanGlobalConfWorker::NsmGlobalPVlanMappingPoMessageHandler));

    }
    PVlanGlobalConfWorker::~PVlanGlobalConfWorker()
    {
    }
    PrismMessage * PVlanGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage* pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMGLOBALPVLANTRUNKASSOCTOPO:
                pPrismMessage = new NsmGlobalPVlanTrunkAssocPoMessage();

                break;
            case NSMGLOBALPVLANMAPPINGTOPO:
                pPrismMessage = new NsmGlobalPVlanMappingPoMessage();

                break;

            default:
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject * PVlanGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject* pWaveManagedObject = NULL;
                if ((PVlanTrunkAssocManagedObject::getClassName()) == managedClassName)
                {
                    pWaveManagedObject = new PVlanTrunkAssocManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
                }
                else if ((PVlanMappingManagedObject::getClassName()) == managedClassName)
                {
                    pWaveManagedObject = new PVlanMappingManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
                }
                else
                {
                    trace(TRACE_LEVEL_FATAL, "PVlanGlobalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
                }

        return (pWaveManagedObject);
    }


    void PVlanGlobalConfWorker::validateTrunkAssocOnPOStep(	DceLinearSeqContext *pDceLinearSeqContext) {


    	trace(TRACE_LEVEL_DEVEL,"Entering IntfConfGlobalWorker::validatePoIntfStep ... ");

    	NsmGlobalPVlanTrunkAssocPoMessage *pNsmGlobalPVlanTrunkAssocPoMessage  =
    			dynamic_cast<NsmGlobalPVlanTrunkAssocPoMessage *>(pDceLinearSeqContext ->getPPrismMessage());
    	ResourceId status = WAVE_MESSAGE_SUCCESS;

    	if(pNsmGlobalPVlanTrunkAssocPoMessage ->getOpcode() == PVLAN_TRUNK_ASSOC_PO){

    	UI32 poId = pNsmGlobalPVlanTrunkAssocPoMessage ->getId();
    	PoIntfManagedObject *pPoIntfManagedObject = NULL;
    	status = NsmUtils::getPoMoByPoId(poId, pPoIntfManagedObject);
    	pDceLinearSeqContext->addManagedObjectForGarbageCollection(pPoIntfManagedObject);
    	trace(TRACE_LEVEL_INFO,string("PVlanGlobalConfWorker validateTrunkAssocOnPOStep : ")+ pNsmGlobalPVlanTrunkAssocPoMessage ->getOpcode());

    	if (pPoIntfManagedObject == NULL || status != WAVE_MESSAGE_SUCCESS) {
    		pDceLinearSeqContext ->executeNextStep(WAVE_MESSAGE_ERROR);
    		trace(TRACE_LEVEL_INFO,string("pPoIntfManagedObject is NULL: ")+ pNsmGlobalPVlanTrunkAssocPoMessage ->getOpcode());
    		return;
    	} else {

    		trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations prim"));
    		UI32 primVId = pNsmGlobalPVlanTrunkAssocPoMessage ->getPrimaryVlanId();
    		UI32 secVId = pNsmGlobalPVlanTrunkAssocPoMessage ->getSecondaryVlanId();
    		NSM_PLUG_DBG_S("\t IN VALidations prim" + primVId);

    		//sw enabled
    		if (!pPoIntfManagedObject->getSwModeFlag()) {
    			trace(TRACE_LEVEL_ERROR,string("\t Interface not configured for switching "));
    			status = WRC_SWITCHING_NOT_ENABLED;
    		}

    		//sw mode trunk set
    		else if (!(pPoIntfManagedObject->getPvlanModeTrunkBas())){
    			trace(TRACE_LEVEL_DEBUG, string("\t PVlanGlobalConfWorker mode trunk set"));
    			status = WRC_PVLAN_MODE_NOT_TRUNK;
    		}

    		//is Primary vlan
    		else if (!PVlanUtils::isPrimaryVlan(primVId)) {
    			trace(TRACE_LEVEL_DEBUG, string("\t IN PVlanGlobalConfWorker isPrimaryVlan"));
    			status = WRC_NSM_PVLAN_NOT_PRIM_VLAN;
    		}

            //Is secondary Vlan
            else if (!PVlanUtils::isSecondaryVlan(secVId))
            {
				status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
				trace(TRACE_LEVEL_INFO,string("PVlanGlobalConfWorker validateTrunkAssocOnPOStep"));
            }

    		//doesAssociation exists
            else if (!PVlanUtils::doesAssociationExists(primVId, secVId, pDceLinearSeqContext)){
    			trace(TRACE_LEVEL_DEBUG, string("\t PVlanGlobalConfWorker doesAssociationExists"));
    			status = WRC_PVLAN_ASSOC_NOT_PRESENT;
    		}

			//When
			//1. The port is in private-vlan-trunk mode,
			//2. Either primary/secondary vlan >= 4096,
			//3. If the primary/secondary vlan has no ctag associated with it, return ERROR.
			else if((primVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagPVlanClassificationMO::getClassName(), primVId, "", 0, 0, pPoIntfManagedObject->getObjectId()) == 0))
			{
				trace(TRACE_LEVEL_ERROR, string("PVlanGlobalConfWorker::validateTrunkAssocOnPoStep:Primary Vlan is not associated with a ctag."));
				status = WRC_NSM_GVLAN_PRIMARY_HAVE_NO_CTAG;
			}
			else if((secVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagPVlanClassificationMO::getClassName(), secVId, "", 0, 0, pPoIntfManagedObject->getObjectId()) == 0))
			{
				trace(TRACE_LEVEL_ERROR, string("PVlanGlobalConfWorker::validateTrunkAssocOnPoStep:Secondary Vlan is not associated with a ctag."));
				status = WRC_NSM_GVLAN_SECONDARY_HAVE_NO_CTAG;
			}

    		trace(TRACE_LEVEL_INFO, string("validateTrunkAssocOnPOStep ")+ primVId);
    	}
    	}

     	pDceLinearSeqContext->executeNextStep(status);
     	return;

 	}


    void PVlanGlobalConfWorker::PVlanTrunkAssocOnPoIntfStep(DceLinearSeqContext * pDceLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
		WaveSendToClusterContext    *send_ctx_p;

        NsmGlobalPVlanTrunkAssocPoMessage* pNsmGlobalPVlanTrunkAssocPoMessage =
            dynamic_cast<NsmGlobalPVlanTrunkAssocPoMessage*>(pDceLinearSeqContext -> getPPrismMessage());
		string PoName = pNsmGlobalPVlanTrunkAssocPoMessage -> getIfName();
        NsmPVlanTrunkAssocPoSPMessage* m = new NsmPVlanTrunkAssocPoSPMessage();

        m -> setIfName(pNsmGlobalPVlanTrunkAssocPoMessage -> getIfName());
		m -> setPrimaryVlanId(pNsmGlobalPVlanTrunkAssocPoMessage -> getPrimaryVlanId());
		m -> setSecondaryVlanId(pNsmGlobalPVlanTrunkAssocPoMessage -> getSecondaryVlanId());
        m -> setOpcode(pNsmGlobalPVlanTrunkAssocPoMessage -> getOpcode());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&PVlanGlobalConfWorker::
				                                  PVlanGlobalSendToClusterCallback), pDceLinearSeqContext);
		NSM_PLUG_ASSERT(send_ctx_p);
		if(PORT_NOT_PART_OF_ANY_INTERFACE == NsmUtils::setLocationIdsForPoSubModeCmds(PoName, send_ctx_p))
		{
			NSM_PLUG_DBG("Port not part of any interface");
			pDceLinearSeqContext -> executeNextStep(WAVE_MESSAGE_SUCCESS);
			delete m;
			delete send_ctx_p;
			return;
		}
		send_ctx_p->setPPrismMessageForPhase1(m);
        setClusterContextFlags(send_ctx_p);
		sendToWaveCluster(send_ctx_p);
        return ;
    }


    void PVlanGlobalConfWorker::PVlanTrunkAssocOnPoIntfMOStep(DceLinearSeqContext * pDceLinearSeqContext) {

    	NSM_PLUG_DBG("Entering PVlanTrunkAssocOnPoIntfMOStep ...");
    	ObjectId PoOid;
    	ObjectId VlanOid;
    	PoIntfManagedObject *pPoIntfManagedObject = NULL;

        NsmGlobalPVlanTrunkAssocPoMessage* pNsmGlobalPVlanTrunkAssocPoMessage = dynamic_cast<NsmGlobalPVlanTrunkAssocPoMessage*>(pDceLinearSeqContext -> getPPrismMessage());

		//NsmUtils::getPoIntfObjectIdByPoIfName(pNsmGlobalPVlanTrunkAssocPoMessage -> getIfName(), PoOid, true, pDceLinearSeqContext);
		//m -> setIfName(pNsmGlobalPVlanTrunkAssocPoMessage -> getIfName());
		//m -> setOwnerManagedObjectId(PoOid);

        UI32 poId = pNsmGlobalPVlanTrunkAssocPoMessage ->getId();
        trace (TRACE_LEVEL_INFO, string("PVlanTrunkAssocOnPoIntfMOStep poID is") +getString(poId));
	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	    syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
	    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	    pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);

		if (NULL != pResults) {
	    	UI32 numberOfResults = pResults->size ();
	       	if (0 == numberOfResults) {
	       		trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
			 } else if (1 == numberOfResults) {
	          	trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
	           	pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
	          }
			pResults->clear();
			delete pResults;
	      }

	    prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);
	    trace (TRACE_LEVEL_INFO, string("PVlanTrunkAssocOnPoIntfMOStep after assert"));

	    switch(pNsmGlobalPVlanTrunkAssocPoMessage ->getOpcode())
		{

        case PVLAN_TRUNK_ASSOC_PO :
        {
        	PVlanTrunkAssocManagedObject* m = new PVlanTrunkAssocManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
    		NSM_PLUG_ASSERT(m);
    		m -> setPrimaryVlanId(pNsmGlobalPVlanTrunkAssocPoMessage  -> getPrimaryVlanId());
    		m -> setSecondaryVlanId(pNsmGlobalPVlanTrunkAssocPoMessage  -> getSecondaryVlanId());
    		trace (TRACE_LEVEL_INFO, string("PVlanTrunkAssocOnPoIntfMOStep PVLAN_TRUNK_ASSOC_PO  "));
    		pPoIntfManagedObject-> addPVlanTrunkAssoc(m);
    		break;
        }

        case PVLAN_TRUNK_ASSOC_PO_DEL :
        {
			trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_TRUNK_ASSOC_PHY_DEL prim secIds =") + pNsmGlobalPVlanTrunkAssocPoMessage  -> getPrimaryVlanId() + pNsmGlobalPVlanTrunkAssocPoMessage  -> getSecondaryVlanId());
			pPoIntfManagedObject->deletePVlanTrunkAssocPrimSec(pNsmGlobalPVlanTrunkAssocPoMessage  -> getPrimaryVlanId(),pNsmGlobalPVlanTrunkAssocPoMessage  -> getSecondaryVlanId());

            // Partial DB Update
			pPoIntfManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
			break;
        }

        case PVLAN_TRUNK_ASSOC_PO_DEL_PRIM :
        {
        	NSM_PLUG_DBG("PVLAN_TRUNK_ASSOC_PHY_DEL_PRIM " + pNsmGlobalPVlanTrunkAssocPoMessage  -> getPrimaryVlanId());
			trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_TRUNK_ASSOC_PHY_DEL_PRIM prim Id =") + pNsmGlobalPVlanTrunkAssocPoMessage  -> getPrimaryVlanId());
			pPoIntfManagedObject->deletePVlanTrunkAssocPrim(pNsmGlobalPVlanTrunkAssocPoMessage  -> getPrimaryVlanId());

            // Partial DB Update
			pPoIntfManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
			break;
        }

        case PVLAN_TRUNK_ASSOC_PO_DEL_ALL :
        {
			trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_TRUNK_ASSOC_PO_DEL_ALL") );
			pPoIntfManagedObject->deletePVlanTrunkAssocAll();

            // Partial DB Update
			pPoIntfManagedObject->addAttributeToBeUpdated("pVlanTrunkAssoc");
			break;
        }
        default :
        {
        	trace (TRACE_LEVEL_DEBUG, string("RCHD in default") );
        	break;
        }
		}

	    updateWaveManagedObject (pPoIntfManagedObject);
	    trace (TRACE_LEVEL_INFO, string("PVlanTrunkAssocOnPoIntfMOStep UPDATED  "));
		pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

        return;
    }

    void PVlanGlobalConfWorker::NsmGlobalPVlanTrunkAssocPoMessageHandler(NsmGlobalPVlanTrunkAssocPoMessage * pNsmGlobalPVlanTrunkAssocPoMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::validateTrunkAssocOnPOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::PVlanTrunkAssocOnPoIntfStep),
     	    reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerStartTransactionStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::PVlanTrunkAssocOnPoIntfMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerSucceededStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pNsmGlobalPVlanTrunkAssocPoMessage, this,
                																sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
        pDceLinearSeqContext->holdAll ();
        pDceLinearSeqContext->start();
    }


    void PVlanGlobalConfWorker::validateMappingOnPOStep (DceLinearSeqContext *pDceLinearSeqContext)
 	{
    	trace(TRACE_LEVEL_DEVEL,"Entering PVlanGlobalConfWorker::validateMappingOnPOStep  ... ");

    	NsmGlobalPVlanMappingPoMessage  *pNsmGlobalPVlanMappingPoMessage   =
    			dynamic_cast<NsmGlobalPVlanMappingPoMessage  *>(pDceLinearSeqContext ->getPPrismMessage());
    	ResourceId status = WAVE_MESSAGE_SUCCESS;

    	if(pNsmGlobalPVlanMappingPoMessage ->getOpcode() == PVLAN_MAPPING_PO){

    	UI32 poId = pNsmGlobalPVlanMappingPoMessage  ->getId();
    	PoIntfManagedObject *pPoIntfManagedObject = NULL;
    	status = NsmUtils::getPoMoByPoId(poId, pPoIntfManagedObject);
    	pDceLinearSeqContext->addManagedObjectForGarbageCollection(pPoIntfManagedObject);
    	trace(TRACE_LEVEL_INFO,string("PVlanGlobalConfWorker validateMappingOnPOStep : ")+ pNsmGlobalPVlanMappingPoMessage  ->getOpcode());

    	if (pPoIntfManagedObject == NULL || status != WAVE_MESSAGE_SUCCESS) {
    		pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_ERROR);
    		trace(TRACE_LEVEL_INFO,string("pPoIntfManagedObject is NULL: ")+ pNsmGlobalPVlanMappingPoMessage ->getOpcode());
    		return;
    	} else {

    		trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations prim"));
    		UI32 primVId = pNsmGlobalPVlanMappingPoMessage  ->getPrimaryVlanId();
    		NSM_PLUG_DBG_S("\t IN VALidations prim" + primVId);

    		//sw enabled
    		if (!pPoIntfManagedObject->getSwModeFlag()) {
    			trace(TRACE_LEVEL_ERROR,string("\t Interface not configured for switching "));
    			status = WRC_SWITCHING_NOT_ENABLED;
    		}

    		//is Vlaid range
    		if (NsmUtils::isAllUI32RangeVlanConfigured(pNsmGlobalPVlanMappingPoMessage->getSecVlanRange())== WAVE_MESSAGE_ERROR) {
    			status = WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;
    		}

    		//sw mode promiscuous set
    		else if (!(pPoIntfManagedObject->getPvlanModePromiscuous() == true || pPoIntfManagedObject->getPvlanModeTrunkPromis() == true)) {
    			trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations 2"));
    			status = WRC_PVLAN_MODE_NOT_PROMIS;
    		}

    		//is Primary vlan
    		else if (!PVlanUtils::isPrimaryVlan(primVId)) {
    			trace(TRACE_LEVEL_DEBUG, string("\t IN VALidations 23"));
    			status = WRC_NSM_PVLAN_NOT_PRIM_VLAN;
    		}

    		//Are all secondary Vlans
    		else if (!PVlanUtils::isSecondaryVlanRange(pNsmGlobalPVlanMappingPoMessage->getSecVlanRange())) {
    			status = WRC_NSM_PVLAN_NOT_SEC_VLAN;
    			trace(TRACE_LEVEL_INFO,string("PVlanGlobalConfWorker validateMappingOnPOStep "));
    		}

    		//doesAssociation exists
    		 else if (!PVlanUtils::doesAssociationExists(primVId, pNsmGlobalPVlanMappingPoMessage ->getSecVlanRange(), pDceLinearSeqContext)){
    			trace(TRACE_LEVEL_DEBUG, string("\t IN doesAssociationExists "));
    			status = WRC_PVLAN_ASSOC_NOT_PRESENT;
    		}

    		//domain max on promiscuous
    		else if(pNsmGlobalPVlanMappingPoMessage->getoper()== PVLAN_MAPPING_ADD)
    		{
				vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > pVlanMapping = pPoIntfManagedObject->getPVlanMapping();
    			if((pVlanMapping.size() == MAX_PVLAN_DOMAIN_ON_PROMISCUOS) && (!PVlanUtils::isDomainPresentOnPromiscuous(pVlanMapping, primVId)))
    			{
    				trace(TRACE_LEVEL_DEBUG, string("\t PVlanGlobalConfWorker domain max on promiscuous"));
    				status = WRC_NSM_PVLAN_MAX_DOMAIN_ON_PROMISCUOS;
    			}

				//When
				//1. The port is in private-vlan-trunk-promiscuous mode,
				//2. The primary Vlan Id >= 4096,
				//3. The primary vlan should have atleast one ctag associated it, already.
				//Else, return ERROR.
				else if(pPoIntfManagedObject->getPvlanModeTrunkPromis() && (primVId >= MAX_DOT1Q_VLAN_ID) && (NsmUtils::getVlanClassificationMOCount(CtagVlanClassificationMO::getClassName(), primVId, "", 0, 0, pPoIntfManagedObject->getObjectId()) == 0))
				{
					trace(TRACE_LEVEL_ERROR, string("PVlanGlobalConfWorker::validateMappingOnPoStep:Primary Vlan is not associated with a ctag."));
					status = WRC_NSM_GVLAN_PRIMARY_HAVE_NO_CTAG;
				}
    		}

    		trace(TRACE_LEVEL_INFO, string("validateMappingOnPOStep")+ primVId);
    	}
    	}
     	pDceLinearSeqContext->executeNextStep(status);
     	return;
 	}

    void PVlanGlobalConfWorker::PVlanMappingOnPoIntfStep(DceLinearSeqContext *pDceLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
		WaveSendToClusterContext    *send_ctx_p;

        NsmGlobalPVlanMappingPoMessage* pNsmGlobalPVlanMappingPoMessage =
            dynamic_cast<NsmGlobalPVlanMappingPoMessage*>(pDceLinearSeqContext -> getPPrismMessage());
		string PoName = pNsmGlobalPVlanMappingPoMessage -> getIfName();

        NsmPVlanMappingPoSPMessage* m = new NsmPVlanMappingPoSPMessage();

		if(pNsmGlobalPVlanMappingPoMessage->getoper() == PVLAN_MAPPING_DEL && pNsmGlobalPVlanMappingPoMessage-> getSecVlanRange().isEmpty()){
			UI32Range dbVlanRange = PVlanUtils::getSecPVlanRangeFromDb(pNsmGlobalPVlanMappingPoMessage -> getPrimaryVlanId(), pDceLinearSeqContext);
			m -> setSecVlanRange(dbVlanRange);
		} else{
			m -> setSecVlanRange(pNsmGlobalPVlanMappingPoMessage->getSecVlanRange());
		}

        m -> setIfName(pNsmGlobalPVlanMappingPoMessage -> getIfName());
		m -> setPrimaryVlanId(pNsmGlobalPVlanMappingPoMessage -> getPrimaryVlanId());
		m -> setoper(pNsmGlobalPVlanMappingPoMessage->getoper());
        m -> setOpcode(pNsmGlobalPVlanMappingPoMessage -> getOpcode());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&PVlanGlobalConfWorker::
				                                  PVlanGlobalSendToClusterCallback), pDceLinearSeqContext);
		NSM_PLUG_ASSERT(send_ctx_p);
		if(PORT_NOT_PART_OF_ANY_INTERFACE == NsmUtils::setLocationIdsForPoSubModeCmds(PoName, send_ctx_p))
		{
			NSM_PLUG_DBG("Port not part of any interface");
			pDceLinearSeqContext -> executeNextStep(WAVE_MESSAGE_SUCCESS);
			delete m;
			delete send_ctx_p;
			return;
		}
		send_ctx_p->setPPrismMessageForPhase1(m);
        setClusterContextFlags(send_ctx_p);
		sendToWaveCluster(send_ctx_p);
        return ;
    }


    void PVlanGlobalConfWorker::PVlanMappingOnPoIntfMOStep(DceLinearSeqContext *pDceLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ObjectId             PoOid;
        ObjectId             VlanOid;

        NsmGlobalPVlanMappingPoMessage* pNsmGlobalPVlanMappingPoMessage =
            dynamic_cast<NsmGlobalPVlanMappingPoMessage*>(pDceLinearSeqContext -> getPPrismMessage());

		trace(TRACE_LEVEL_INFO,string("PVlanGlobalConfWorker PVlanMappingOnPoIntfMOStep"));

        UI32 poId = pNsmGlobalPVlanMappingPoMessage ->getId();
        trace (TRACE_LEVEL_INFO, string("PVlanMappingOnPoIntfMOStep poID is  -------") +getString(poId));

	    PoIntfManagedObject *pPoIntfManagedObject= NULL;
	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	    syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId,"id"));
	    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	    pDceLinearSeqContext->addManagedObjectsForGarbageCollection(*pResults);

		if (NULL != pResults) {
	    	UI32 numberOfResults = pResults->size ();
	       	if (0 == numberOfResults) {
	       		trace (TRACE_LEVEL_DEBUG, string("PResults is ZERO **** -------"));
			 } else if (1 == numberOfResults) {
	          	trace (TRACE_LEVEL_DEBUG, string("PResults is ONE**** -------"));
	           	pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
	          }
			pResults->clear();
			delete pResults;
	      }

	        prismAssert (NULL != pPoIntfManagedObject, __FILE__, __LINE__);

	        switch(pNsmGlobalPVlanMappingPoMessage->getOpcode()){

	        case PVLAN_MAPPING_PO :
	        {

	        PVlanMappingManagedObject* pvlanMappingMO = NULL;
			vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > pvlanMapMoVector = pPoIntfManagedObject->getPVlanMapping();

			bool isPresent = false;
			for (unsigned int j = 0; j < pvlanMapMoVector.size(); j++)
			{
				if(pvlanMapMoVector[j]-> getPrimaryVlanId() == pNsmGlobalPVlanMappingPoMessage -> getPrimaryVlanId()){
					pvlanMappingMO = pvlanMapMoVector[j].operator-> ();
					isPresent = true;
					break;
				}
			}


			if (isPresent && (pNsmGlobalPVlanMappingPoMessage->getoper() == PVLAN_MAPPING_DEL && pNsmGlobalPVlanMappingPoMessage-> getSecVlanRange().isEmpty())){

				trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_MAPPING_PO_DEL_PRIM") );
				pPoIntfManagedObject ->deletePVlanMappingPrim(pNsmGlobalPVlanMappingPoMessage -> getPrimaryVlanId());
				pPoIntfManagedObject ->addAttributeToBeUpdated("pVlanMapping");
			} else if(isPresent){

				NSM_PLUG_DBG("Entry exists --  PVlanMappingOnPoIntfMOStep ...");
				UI32Range updatedVlanRange = PVlanUtils :: getSecPVlanRange(pNsmGlobalPVlanMappingPoMessage-> getSecVlanRange(), pvlanMappingMO->getSecPvlanRange(),(PVlanMappingOper) pNsmGlobalPVlanMappingPoMessage->getoper());
				NSM_PLUG_DBG("Got updated range --  PVlanMappingOnPoIntfMOStep ...");
				pvlanMappingMO -> setSecPvlanRange(updatedVlanRange);
				pvlanMappingMO -> setoper(PVLAN_MAPPING_ADD);
				updateWaveManagedObject(pvlanMappingMO);
			}else if(pNsmGlobalPVlanMappingPoMessage->getoper() != PVLAN_MAPPING_DEL){
				pvlanMappingMO = new PVlanMappingManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				NSM_PLUG_ASSERT(pvlanMappingMO);
				NSM_PLUG_DBG("New entry -- PVlanMappingOnPoIntfMOStep ...");
				pvlanMappingMO -> setPrimaryVlanId(pNsmGlobalPVlanMappingPoMessage -> getPrimaryVlanId());
				pvlanMappingMO -> setSecPvlanRange(pNsmGlobalPVlanMappingPoMessage-> getSecVlanRange());
				pvlanMappingMO -> setoper(PVLAN_MAPPING_ADD);
				pPoIntfManagedObject -> addPVlanMapping(pvlanMappingMO);
			}
				break;
	        }


	        case PVLAN_MAPPING_PO_DEL_PRIM:
	        {
	        	trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_MAPPING_PO_DEL_PRIM  prim Id =") +  pNsmGlobalPVlanMappingPoMessage -> getPrimaryVlanId());
				pPoIntfManagedObject->deletePVlanMappingPrim( pNsmGlobalPVlanMappingPoMessage -> getPrimaryVlanId());

	            // Partial DB Update
	            pPoIntfManagedObject->addAttributeToBeUpdated("pVlanMapping");
				break;
	        }

	        case PVLAN_MAPPING_PO_DEL_ALL:
	        {
				trace (TRACE_LEVEL_DEBUG, string("DB DELETE PVLAN_MAPPING_PO_DEL_ALL") );
				pPoIntfManagedObject->deletePVlanMappingAll();

	            // Partial DB Update
				pPoIntfManagedObject->addAttributeToBeUpdated("pVlanMapping");
				break;
	        }

	        default :
	        {
	        	trace (TRACE_LEVEL_DEBUG, string("RCHD in default") );
	        	break;
	        }
	        }

			NSM_PLUG_DBG("AFTER PVlanMappingOnPoIntfMOStep ...");

			updateWaveManagedObject(pPoIntfManagedObject);

			pDceLinearSeqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

        return;
    }
    void PVlanGlobalConfWorker::NsmGlobalPVlanMappingPoMessageHandler(NsmGlobalPVlanMappingPoMessage * pNsmGlobalPVlanMappingPoMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::validateMappingOnPOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::PVlanMappingOnPoIntfStep),
     	    reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerStartTransactionStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::PVlanMappingOnPoIntfMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerSucceededStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&PVlanGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        DceLinearSeqContext *pDceLinearSeqContext = new DceLinearSeqContext (pNsmGlobalPVlanMappingPoMessage, this,
                															sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
        pDceLinearSeqContext->holdAll ();
        pDceLinearSeqContext->start();
    }

	void PVlanGlobalConfWorker::PVlanGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p)
	{
		NSM_PLUG_DBG_TRACE();
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
		NSM_PLUG_DBG_S(string("sendToClusterCompletionStatus = ") + sendToClusterCompletionStatus);
		PrismLinearSequencerContext             *seq_ctx_p;
		seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
		delete ctx_p  -> getPPrismMessageForPhase1();
		delete ctx_p  -> getPPrismMessageForPhase2();
		delete ctx_p;
		seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
		return;
	}

}
