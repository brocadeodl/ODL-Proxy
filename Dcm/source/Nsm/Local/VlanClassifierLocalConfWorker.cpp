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
 *   Author : rmuthigi                                                     *
 **************************************************************************/



#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/VlanClassifierLocalConfWorker.h"
#include "Nsm/Local/NsmAddVlanClassifierToPhyMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierToPoMessage.h"
#include "Nsm/Local/NsmCreateVlanClassifierRuleSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierRuleSPMessage.h"
#include "Nsm/Local/NsmUpdateVlanClassifierGroupSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierGroupSPMessage.h"
#include "Nsm/Local/NsmAddVlanClassifierToPoSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierToPoSPMessage.h"
#include "Nsm/Local/VlanClassifierActivateLocalManagedObject.h"
#include "Nsm/Local/NsmDeleteVlanClassifierToPoMessage.h"
#include "Nsm/Local/NsmLocalShowVlanClassifierMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/VLAN/GvlanConfigUtils.h"

#include "ClientInterface/Nsm/NsmClientCreateVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierRuleSPMessage.h"
#include "ClientInterface/Nsm/NsmClientUpdateVlanClassifierGroupSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierGroupSPMessage.h"
#include "ClientInterface/Nsm/NsmClientAddVlanClassifierToPoSPMessage.h"
#include "ClientInterface/Nsm/NsmClientDeleteVlanClassifierToPoSPMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/NsmShowVlanClassifierMessage.h"

#include "Nsm/Global/VlanClassifierRulesManagedObject.h"
#include "Nsm/Global/VlanClassifierGroupAssocRulesManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"

#include "DcmCStatus.h"
#include <iostream>
#include <string>

namespace DcmNs
{
    VlanClassifierLocalConfWorker::VlanClassifierLocalConfWorker(NsmLocalObjectManager * pNsmLocalObjectManager):
        WaveWorker(pNsmLocalObjectManager)
    {
        VlanClassifierActivateLocalManagedObject VlanClassifierActivateLocalManagedObject(pNsmLocalObjectManager);

        VlanClassifierActivateLocalManagedObject.setupOrm();
        addManagedClass(VlanClassifierActivateLocalManagedObject::getClassName(), this);
        addOperationMap(NSMADDVLANCLASSIFIERTOPHY, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmAddVlanClassifierToPhyMessageHandler));
        addOperationMap(NSMDELETEVLANCLASSIFIERTOPO, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierToPhyMessageHandler));
        addOperationMap(NSMCREATEVLANCLASSIFIERRULESP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmCreateVlanClassifierRuleSPMessageHandler));
        addOperationMap(NSMDELETEVLANCLASSIFIERRULESP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierRuleSPMessageHandler));
        addOperationMap(NSMUPDATEVLANCLASSIFIERGROUPSP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmUpdateVlanClassifierGroupSPMessageHandler));
        addOperationMap(NSMDELETEVLANCLASSIFIERGROUPSP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierGroupSPMessageHandler));
        addOperationMap(NSMADDVLANCLASSIFIERTOPOSP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmAddVlanClassifierToPoSPMessageHandler));
        addOperationMap(NSMDELETEVLANCLASSIFIERTOPOSP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierToPoSPMessageHandler));
        addOperationMap(NSMLOCALSHOWVLANCLASSIFIER, reinterpret_cast<PrismMessageHandler>(&VlanClassifierLocalConfWorker::NsmLocalShowVlanClassifierMessageHandler));
    }
    VlanClassifierLocalConfWorker::~VlanClassifierLocalConfWorker()
    {
    }
    PrismMessage * VlanClassifierLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage* pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMADDVLANCLASSIFIERTOPHY:
                pPrismMessage = new NsmAddVlanClassifierToPhyMessage();

                break;

            case NSMDELETEVLANCLASSIFIERTOPO:
                pPrismMessage = new NsmDeleteVlanClassifierToPhyMessage();

                break;

            case NSMCREATEVLANCLASSIFIERRULESP:
                pPrismMessage = new NsmCreateVlanClassifierRuleSPMessage();

                break;

            case NSMDELETEVLANCLASSIFIERRULESP:
                pPrismMessage = new NsmDeleteVlanClassifierRuleSPMessage();

                break;

            case NSMUPDATEVLANCLASSIFIERGROUPSP:
                pPrismMessage = new NsmUpdateVlanClassifierGroupSPMessage();

                break;

            case NSMDELETEVLANCLASSIFIERGROUPSP:
                pPrismMessage = new NsmDeleteVlanClassifierGroupSPMessage();

                break;

            case NSMADDVLANCLASSIFIERTOPOSP:
                pPrismMessage = new NsmAddVlanClassifierToPoSPMessage();

                break;

            case NSMDELETEVLANCLASSIFIERTOPOSP:
                pPrismMessage = new NsmDeleteVlanClassifierToPoSPMessage();

                break;
            case NSMLOCALSHOWVLANCLASSIFIER:
                pPrismMessage = new NsmLocalShowVlanClassifierMessage();

                break;

            default:
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
    WaveManagedObject * VlanClassifierLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject* pWaveManagedObject = NULL;

        if ((VlanClassifierActivateLocalManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new VlanClassifierActivateLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        }
        else
        {
            trace(TRACE_LEVEL_FATAL, "IntfConfLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }

        return (pWaveManagedObject);
    }

	void VlanClassifierLocalConfWorker::NsmLocalShowVlanClassifierMessageHandler(NsmLocalShowVlanClassifierMessage* pNsmLocalShowVlanClassifierMessage)
	{     	

		if (pNsmLocalShowVlanClassifierMessage->getIsGwRequest()) 
		{
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep> (&VlanClassifierLocalConfWorker::sendVlanClassifierCommandToCluster),
				reinterpret_cast<PrismLinearSequencerStep> (&VlanClassifierLocalConfWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep> (&VlanClassifierLocalConfWorker::prismLinearSequencerFailedStep)
			};

			PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmLocalShowVlanClassifierMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
			pPrismLinearSequencerContext->start ();
			return;
		} else {
			PrismSynchronousLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VlanClassifierLocalConfWorker::sendVlanClassifierCommandToNsmDaemon),
				reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
				reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
			};

			PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext( pNsmLocalShowVlanClassifierMessage, this, sequencerSteps,
				sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
			pPrismSynchronousLinearSequencerContext->execute ();

		}
	}

	void VlanClassifierLocalConfWorker::sendVlanClassifierCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
	{
		NSM_PLUG_DBG("Entering ...");

		NsmLocalShowVlanClassifierMessage *msg_p = dynamic_cast<NsmLocalShowVlanClassifierMessage*> (seq_ctx_p->getPPrismMessage ()); 
		NSM_PLUG_ASSERT_PTR(msg_p);

		NsmLocalShowVlanClassifierMessage *loc_msg_p = new NsmLocalShowVlanClassifierMessage(msg_p -> getBackEndData(), msg_p -> getCmdCode(), false);
		NSM_PLUG_ASSERT_PTR(loc_msg_p);
		loc_msg_p->setLastIndex(msg_p->getLastIndex());

		WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&VlanClassifierLocalConfWorker::vlanClassifierShowSendToClusterCallback), seq_ctx_p);
		NSM_PLUG_ASSERT_PTR(send_ctx_p);

		send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        switch( msg_p -> getCmdCode())
        {
			case VLANCLASSIFIER_SHOW_RULE:
		    case VLANCLASSIFIER_SHOW_RULE_ALL:
			case VLANCLASSIFIER_SHOW_GROUP:
			case VLANCLASSIFIER_SHOW_GROUP_ALL:
				NsmUtils::setLocalLocationIdForPhase1Message(send_ctx_p);
				break;
			case VLANCLASSIFIER_SHOW_INTF_TE:
				{
					vector<LocationId> locationId;
					if(-1 != msg_p -> getLocationId())
					{	
					    locationId.push_back(msg_p -> getLocationId());
					    send_ctx_p->setLocationsToSendToForPhase1(locationId);
					}	
					else
					{	
						 NsmUtils::setLocalLocationIdForPhase1Message(send_ctx_p);
					}	 
					break; 
				}
			case VLANCLASSIFIER_SHOW_INTF_PO:
				{
					ResourceId status = DcmToolKit::isVcsEnabled ();
					if(WAVE_PERSISTENCE_CLUSTER_ENABLED == status)
					{	
						vector<LocationId> temp;
						vector<LocationId> locationId;
						NSM_PLUG_DBG("Inside po cmd Code");
						string poName = msg_p -> getBackEndData();
						string ifName = poName.substr(2);
						NsmUtils::getPoMemberLocationIds(ifName, temp);
						locationId.push_back(temp[0]);
						send_ctx_p->setLocationsToSendToForPhase1(locationId);
					}
					else
					{
						//standalone
						NsmUtils::setLocalLocationIdForPhase1Message(send_ctx_p);
					}	
					break;
				}
			default:
				break;
        }  
		sendToWaveCluster(send_ctx_p);
		return;
	}

	ResourceId VlanClassifierLocalConfWorker::sendVlanClassifierCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p)
	{
		NSM_PLUG_DBG("Entering ...");
		SI32 mappedId = DcmToolKit::getLocalMappedId();
		if(mappedId == -1)
			mappedId = 0;

		NsmLocalShowVlanClassifierMessage *msg_p = dynamic_cast<NsmLocalShowVlanClassifierMessage*> (seq_ctx_p->getPPrismMessage ());    
		NsmShowVlanClassifierMessage *m = new NsmShowVlanClassifierMessage(msg_p -> getBackEndData(), msg_p -> getCmdCode(),mappedId);
		m->setLastIndex(msg_p->getLastIndex());
		ResourceId status = sendSynchronouslyToWaveClient ("nsm",m);

		if(WAVE_MESSAGE_SUCCESS != status)
		{
			NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
			delete m;
			return status;
		}
		msg_p->copyAllBuffers(*m);
		//bpatel: To avoid memory leak
		if(m){
			delete m;
		}
		return status;
	}

    void VlanClassifierLocalConfWorker::vlanClassifierShowSendToClusterCallback(WaveSendToClusterContext *ctx_p)
	{
		NSM_PLUG_DBG("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        NsmLocalShowVlanClassifierMessage *req_msg_p = NULL;
        NsmLocalShowVlanClassifierMessage *resp_msg_p = NULL;
        NsmLocalShowVlanClassifierMessage *sw_resp_p = NULL;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        NSM_PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<NsmLocalShowVlanClassifierMessage*> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<NsmLocalShowVlanClassifierMessage*> (ctx_p->getPPrismMessageForPhase1());

        NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        NSM_PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());

        for (i = 0; i < locations.size(); i++) 
        {
            if (i == 0) 
			{	
                NSM_PLUG_DBG_S(string("locations:") + locations[i]);
			}	
            else 
			{	
                NSM_PLUG_DBG_S(string(":") + locations[i]);
			}	
            
        }

        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<NsmLocalShowVlanClassifierMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            NSM_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } 
            else 
            {
                NSM_PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
				NSM_PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
		}
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;

	}

    ResourceId VlanClassifierLocalConfWorker::ActivateVlanClassiferOnPhyIntfStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        NSM_PLUG_DBG( "Entering ...");

        ResourceId status = 0;
		char buffer[25];
		buffer[24] = '\0';
		UI32 bufferSize = sizeof (buffer);
        NsmAddVlanClassifierToPhyMessage* pNsmAddVlanClassifierToPhySPMessage =
            dynamic_cast<NsmAddVlanClassifierToPhyMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());

		NSM_PLUG_ASSERT(pNsmAddVlanClassifierToPhySPMessage);


		if(pNsmAddVlanClassifierToPhySPMessage -> getOpcode() == VLAN_CLASSIFIER_TE_REMOVE_MO)
		{
			/*This Case will be hit during "no vlan classifier rule */
			pNsmAddVlanClassifierToPhySPMessage -> setOpcode(VLAN_CLASSIFIER_TE_DEACTIVATE);
			return WAVE_MESSAGE_SUCCESS;
		}	
		if(true == NsmUtils::isPhyPortInISLMode(pNsmAddVlanClassifierToPhySPMessage -> getIfName(), true, pDceSynchronousLinearSeqContext) )
		{
			NSM_PLUG_DBG_S(string("port in ISL Mode Nothing to be Done") + pNsmAddVlanClassifierToPhySPMessage -> getIfName() );
			return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
		}

		PROVISION_VLAN(pDceSynchronousLinearSeqContext, pNsmAddVlanClassifierToPhySPMessage -> getVlanId());

        NsmClientAddVlanClassifierToPoSPMessage* m      = new NsmClientAddVlanClassifierToPoSPMessage();
		NSM_PLUG_ASSERT(m);
		
        string                                   ifName = pNsmAddVlanClassifierToPhySPMessage -> getIfName();
		if(true  == NsmUtils::isValidThreeTuple(ifName) )
		{
			string twoTuppleIfName ;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}	

		GENERATE_SHORT_IF_NAME_FOR_BACK_END(pNsmAddVlanClassifierToPhySPMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
		ifName = buffer;
        //ifName.insert(0, "te");
        m -> setIfName(ifName);
        m -> setIfIndex(pNsmAddVlanClassifierToPhySPMessage -> getIfIndex());
        m -> setGroupId(pNsmAddVlanClassifierToPhySPMessage -> getGroupId());
        m -> setVlanId(pNsmAddVlanClassifierToPhySPMessage -> getVlanId());
        m -> setOpcode(pNsmAddVlanClassifierToPhySPMessage -> getOpcode());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);

        return errorCode(pNsmAddVlanClassifierToPhySPMessage, m, status);
    }

	ResourceId VlanClassifierLocalConfWorker::deleteVlanClassifierGroupFromPhyStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		NSM_PLUG_DBG("Entering ...");
        ResourceId status = 0;
		char buffer[25];
		buffer[24] = '\0';
		UI32 bufferSize = sizeof (buffer);
        NsmDeleteVlanClassifierToPhyMessage* pNsmDeleteVlanClassifierToPhySPMessage =
            dynamic_cast<NsmDeleteVlanClassifierToPhyMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());

		NSM_PLUG_ASSERT(pNsmDeleteVlanClassifierToPhySPMessage);

		if(pNsmDeleteVlanClassifierToPhySPMessage -> getOpcode() == VLAN_CLASSIFIER_TE_REMOVE_MO)
		{
			/*This Case will be hit during "no vlan classifier rule */
			return WAVE_MESSAGE_SUCCESS;
		}	
		if(true == NsmUtils::isPhyPortInISLMode(pNsmDeleteVlanClassifierToPhySPMessage -> getIfName(), true, pPrismSynchronousLinearSequencerContext) )
		{
			NSM_PLUG_DBG_S(string("port in ISL Mode Nothing to be Done") + pNsmDeleteVlanClassifierToPhySPMessage-> getIfName() );
			return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
		}
        auto_ptr<NsmClientAddVlanClassifierToPoSPMessage> m (new NsmClientAddVlanClassifierToPoSPMessage());
		NSM_PLUG_ASSERT(m.get());
		
        string                                   ifName = pNsmDeleteVlanClassifierToPhySPMessage -> getIfName();
		if(true  == NsmUtils::isValidThreeTuple(ifName) )
		{
			string twoTuppleIfName ;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}	
		GENERATE_SHORT_IF_NAME_FOR_BACK_END(pNsmDeleteVlanClassifierToPhySPMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
		ifName = buffer;
        //ifName.insert(0, "te");
        m -> setIfName(ifName);
        m -> setIfIndex(pNsmDeleteVlanClassifierToPhySPMessage -> getIfIndex());
        m -> setGroupId(pNsmDeleteVlanClassifierToPhySPMessage -> getGroupId());
        m -> setVlanId(0);
        m -> setOpcode(pNsmDeleteVlanClassifierToPhySPMessage -> getOpcode());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m.get());
        status = inspectErrors(pNsmDeleteVlanClassifierToPhySPMessage, m.get(), status);
        if (WAVE_MESSAGE_SUCCESS == status) {
            HANDLE_PROVISION_RESPONSE(m.get());
        }
        return status;

	}	


	ResourceId VlanClassifierLocalConfWorker::deleteVlanClassifierGroupFromPhyMoStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		NSM_PLUG_DBG("Entering ...");
		PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
        NsmDeleteVlanClassifierToPhyMessage* pNsmDeleteVlanClassifierToPhyMessage =
            dynamic_cast<NsmDeleteVlanClassifierToPhyMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());

		NSM_PLUG_ASSERT(pNsmDeleteVlanClassifierToPhyMessage);
		NsmUtils::getPhyMoByPhyName(pNsmDeleteVlanClassifierToPhyMessage -> getIfName(), pPhyIntfLocalManagedObject);
		NSM_PLUG_ASSERT(pPhyIntfLocalManagedObject);
		pPhyIntfLocalManagedObject -> deleteVlanClassifierActivatedGroup(pNsmDeleteVlanClassifierToPhyMessage -> getGroupId());
		updateWaveManagedObject(pPhyIntfLocalManagedObject);
		pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
        return WAVE_MESSAGE_SUCCESS;
	}	

    ResourceId VlanClassifierLocalConfWorker::ActivateVlanClassiferOnPhyIntfMOStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ObjectId                   VlanOid;
        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;

        NsmAddVlanClassifierToPhyMessage* pNsmAddVlanClassifierToPhyMessage =
            dynamic_cast<NsmAddVlanClassifierToPhyMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmAddVlanClassifierToPhyMessage);

        VlanClassifierActivateLocalManagedObject* m = new VlanClassifierActivateLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
		NSM_PLUG_ASSERT(m);

		NsmUtils::getPhyMoByPhyName(pNsmAddVlanClassifierToPhyMessage -> getIfName(), pPhyIntfLocalManagedObject);
		NSM_PLUG_ASSERT(pPhyIntfLocalManagedObject);
		m -> setGroupId(pNsmAddVlanClassifierToPhyMessage -> getGroupId());
		UI32 VlanName = pNsmAddVlanClassifierToPhyMessage -> getVlanId();
		NsmUtils::getVlanIntfObjectIdByVlanIfName(VlanName, VlanOid, true, pDceSynchronousLinearSeqContext);
		m -> setVlanId(VlanOid);
		m -> setdummy(0);
		m -> setIfName(pNsmAddVlanClassifierToPhyMessage -> getIfName());
		pPhyIntfLocalManagedObject -> addVlanClassifierActivate(m);
		updateWaveManagedObject(pPhyIntfLocalManagedObject);
//		pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(m);
		pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
        return WAVE_MESSAGE_SUCCESS;
    }
    void VlanClassifierLocalConfWorker::NsmAddVlanClassifierToPhyMessageHandler(NsmAddVlanClassifierToPhyMessage * pNsmAddVlanClassifierToPhyMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::ActivateVlanClassiferOnPhyIntfStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::ActivateVlanClassiferOnPhyIntfMOStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
		};
        DceSynchronousLinearSeqContext* pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext(pNsmAddVlanClassifierToPhyMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pDceSynchronousLinearSeqContext -> execute();
    }
    ResourceId VlanClassifierLocalConfWorker::VlanClassifierActivateGroupStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId status = 0;
        NsmAddVlanClassifierToPoSPMessage* pNsmAddVlanClassifierToPoSPMessage =
            dynamic_cast<NsmAddVlanClassifierToPoSPMessage*>(pDceSynchronousLinearSeqContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmAddVlanClassifierToPoSPMessage);

        PROVISION_VLAN(pDceSynchronousLinearSeqContext, pNsmAddVlanClassifierToPoSPMessage-> getVlanId());

        NsmClientAddVlanClassifierToPoSPMessage* m      = new NsmClientAddVlanClassifierToPoSPMessage();
		NSM_PLUG_ASSERT(m);
		
        string                                   ifName = pNsmAddVlanClassifierToPoSPMessage -> getIfName();
        ifName.insert(0, "po");
        m -> setId(pNsmAddVlanClassifierToPoSPMessage -> getId());
        m -> setIfName(ifName);
        m -> setIfIndex(pNsmAddVlanClassifierToPoSPMessage -> getIfIndex());
        m -> setGroupId(pNsmAddVlanClassifierToPoSPMessage -> getGroupId());
        m -> setVlanId(pNsmAddVlanClassifierToPoSPMessage -> getVlanId());
        m -> setOpcode(pNsmAddVlanClassifierToPoSPMessage -> getOpcode());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);

		return errorCode(pNsmAddVlanClassifierToPoSPMessage, m, status);

    }
    void VlanClassifierLocalConfWorker::NsmAddVlanClassifierToPoSPMessageHandler(NsmAddVlanClassifierToPoSPMessage * pNsmAddVlanClassifierToPoSPMessage)
    {
//      Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::VlanClassifierActivateGroupStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext(pNsmAddVlanClassifierToPoSPMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pDceSynchronousLinearSeqContext -> execute();
    }
    ResourceId VlanClassifierLocalConfWorker::createVlanClassfierRuleStep(PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId status = 0;
        NsmCreateVlanClassifierRuleSPMessage* pNsmCreateVlanClassifierRuleSPMessage =
            dynamic_cast<NsmCreateVlanClassifierRuleSPMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmCreateVlanClassifierRuleSPMessage);
		
        NsmClientCreateVlanClassifierRuleSPMessage* m = new NsmClientCreateVlanClassifierRuleSPMessage();
		NSM_PLUG_ASSERT(m);

        m -> setRuleId(pNsmCreateVlanClassifierRuleSPMessage -> getRuleId());
        m -> setMac(pNsmCreateVlanClassifierRuleSPMessage -> getMac());
        m -> setProtoVal(pNsmCreateVlanClassifierRuleSPMessage -> getProtoVal());
        m -> setEncap(pNsmCreateVlanClassifierRuleSPMessage -> getEncap());
        m -> setOpcode(pNsmCreateVlanClassifierRuleSPMessage -> getOpcode());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);

		return errorCode(pNsmCreateVlanClassifierRuleSPMessage, m, status);
    }
    void VlanClassifierLocalConfWorker::NsmCreateVlanClassifierRuleSPMessageHandler(NsmCreateVlanClassifierRuleSPMessage * pNsmCreateVlanClassifierRuleSPMessage)
    {
//      Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::createVlanClassfierRuleStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pNsmCreateVlanClassifierRuleSPMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext -> execute();
    }
    ResourceId VlanClassifierLocalConfWorker::deleteVlanClassfierRuleStep(PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId status = 0;
        NsmDeleteVlanClassifierRuleSPMessage* pNsmDeleteVlanClassifierRuleSPMessage =
            dynamic_cast<NsmDeleteVlanClassifierRuleSPMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmDeleteVlanClassifierRuleSPMessage);
		
        NsmClientDeleteVlanClassifierRuleSPMessage* m = new NsmClientDeleteVlanClassifierRuleSPMessage();
		NSM_PLUG_ASSERT(m);
		
        m -> setRuleId(pNsmDeleteVlanClassifierRuleSPMessage -> getRuleId());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);

		return errorCode(pNsmDeleteVlanClassifierRuleSPMessage, m, status);

    }
    ResourceId VlanClassifierLocalConfWorker::DeleteVlanClassifierGroupStep(PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId                                    status                                 = 0;
        NsmDeleteVlanClassifierGroupSPMessage* pNsmDeleteVlanClassifierGroupSPMessage =
            dynamic_cast<NsmDeleteVlanClassifierGroupSPMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmDeleteVlanClassifierGroupSPMessage);
		
        NsmClientDeleteVlanClassifierGroupSPMessage* m = new NsmClientDeleteVlanClassifierGroupSPMessage();
		NSM_PLUG_ASSERT(m);

        m -> setGroupId(pNsmDeleteVlanClassifierGroupSPMessage -> getGroupId());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);
		return errorCode(pNsmDeleteVlanClassifierGroupSPMessage, m, status);

    }

	ResourceId VlanClassifierLocalConfWorker::deactivateVlanClassifierOnInterface(PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext)
	{
		NSM_PLUG_DBG("Entering ...");
		NsmDeleteVlanClassifierGroupSPMessage* pNsmDeleteVlanClassifierGroupSPMessage =
			dynamic_cast<NsmDeleteVlanClassifierGroupSPMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmDeleteVlanClassifierGroupSPMessage);

		UI32 groupId =  pNsmDeleteVlanClassifierGroupSPMessage->getGroupId();
		deactivateVlanClassifierOnPhy(groupId);
		return WAVE_MESSAGE_SUCCESS;    
	}

	void VlanClassifierLocalConfWorker::deactivateVlanClassifierOnPhy(UI32 groupId)
	{
		NSM_PLUG_DBG("Entering ...");
		WaveManagedObjectSynchronousQueryContext syncDeActivateQueryCtxt(VlanClassifierActivateLocalManagedObject::getClassName());
		syncDeActivateQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));
        syncDeActivateQueryCtxt.addSelectField("objectId");
        syncDeActivateQueryCtxt.addSelectField("ownerManagedObjectId");
		vector<WaveManagedObject * >*               pDeActivateResults                   =  querySynchronously(&syncDeActivateQueryCtxt);
		if(pDeActivateResults)
		{
			for(unsigned int j = 0;j < pDeActivateResults -> size() ;j++)
			{
				VlanClassifierActivateLocalManagedObject *pVlanClassifierActivateLocalManagedObject = dynamic_cast<VlanClassifierActivateLocalManagedObject*>
					((*pDeActivateResults)[j]);

                deleteFromComposition(PhyIntfLocalManagedObject::getClassName(), VlanClassifierActivateLocalManagedObject::getClassName(),
                                                             "vlanClassifierActivate", pVlanClassifierActivateLocalManagedObject -> getOwnerManagedObjectId(), pVlanClassifierActivateLocalManagedObject-> getObjectId());
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDeActivateResults);
		}	
	}	
	
    void VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierGroupSPMessageHandler(NsmDeleteVlanClassifierGroupSPMessage * pNsmDeleteVlanClassifierGroupSPMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::DeleteVlanClassifierGroupStep), 
	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::deactivateVlanClassifierOnInterface),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep), 
	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pNsmDeleteVlanClassifierGroupSPMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext -> execute();
    }
    void VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierRuleSPMessageHandler(NsmDeleteVlanClassifierRuleSPMessage * pNsmDeleteVlanClassifierRuleSPMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::deleteVlanClassfierRuleStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pNsmDeleteVlanClassifierRuleSPMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext -> execute();
    }
    void VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierToPhyMessageHandler(NsmDeleteVlanClassifierToPhyMessage * pNsmDeleteVlanClassifierToPhyMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::deleteVlanClassifierGroupFromPhyStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::deleteVlanClassifierGroupFromPhyMoStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep), 
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pNsmDeleteVlanClassifierToPhyMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext -> execute();
    }
    ResourceId VlanClassifierLocalConfWorker::DeactivateVlanClassfierGroupFromPo(PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");

        ResourceId                                   status                                = 0;
        NsmDeleteVlanClassifierToPoSPMessage* pNsmDeleteVlanClassifierToPoSPMessage =
            dynamic_cast<NsmDeleteVlanClassifierToPoSPMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmDeleteVlanClassifierToPoSPMessage);
		
        string ifName = pNsmDeleteVlanClassifierToPoSPMessage -> getIfName();
        ifName.insert(0, "po");
        NsmClientDeleteVlanClassifierToPoSPMessage* m = new NsmClientDeleteVlanClassifierToPoSPMessage();
		NSM_PLUG_ASSERT(m);

        m -> setId(pNsmDeleteVlanClassifierToPoSPMessage -> getId());
        m -> setIfName(ifName);
        m -> setIfIndex(pNsmDeleteVlanClassifierToPoSPMessage -> getIfIndex());
        m -> setGroupId(pNsmDeleteVlanClassifierToPoSPMessage -> getGroupId());
        m -> setOpcode(pNsmDeleteVlanClassifierToPoSPMessage -> getOpcode());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);
		return errorCode(pNsmDeleteVlanClassifierToPoSPMessage, m, status);

    }
    void VlanClassifierLocalConfWorker::NsmDeleteVlanClassifierToPoSPMessageHandler(NsmDeleteVlanClassifierToPoSPMessage * pNsmDeleteVlanClassifierToPoSPMessage)
    {
//      Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::DeactivateVlanClassfierGroupFromPo), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext = new  PrismSynchronousLinearSequencerContext(pNsmDeleteVlanClassifierToPoSPMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext -> execute();
    }
    ResourceId VlanClassifierLocalConfWorker::createVlanClassfierGroupStep(PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId                             status                                 = 0;
        NsmUpdateVlanClassifierGroupSPMessage* pNsmUpdateVlanClassifierGroupSPMessage =
            dynamic_cast<NsmUpdateVlanClassifierGroupSPMessage*>(pPrismSynchronousLinearSequencerContext -> getPPrismMessage());
		NSM_PLUG_ASSERT(pNsmUpdateVlanClassifierGroupSPMessage);
		
        NsmClientUpdateVlanClassifierGroupSPMessage* m = new NsmClientUpdateVlanClassifierGroupSPMessage();
		NSM_PLUG_ASSERT(m);

        m -> setGroupId(pNsmUpdateVlanClassifierGroupSPMessage -> getGroupId());
        m -> setOperation(pNsmUpdateVlanClassifierGroupSPMessage -> getOperation());
        m -> setRuleId(pNsmUpdateVlanClassifierGroupSPMessage -> getRuleId());
        NSM_PLUG_DBG("sending synchronously to nsm client");

        status = sendSynchronouslyToWaveClient("nsm", m);
		return errorCode(pNsmUpdateVlanClassifierGroupSPMessage, m, status);

    }
    void VlanClassifierLocalConfWorker::NsmUpdateVlanClassifierGroupSPMessageHandler(NsmUpdateVlanClassifierGroupSPMessage * pNsmUpdateVlanClassifierGroupSPMessage)
    {
//      Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::createVlanClassfierGroupStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerSucceededStep), 
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanClassifierLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext* pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pNsmUpdateVlanClassifierGroupSPMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext -> execute();
    }
	ResourceId VlanClassifierLocalConfWorker::errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status)
	{
        status = inspectErrors(loc_msg_p, c_msg_p, status);
		delete c_msg_p;
		return status;
	}	

	ResourceId VlanClassifierLocalConfWorker::inspectErrors(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status)
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
		return status;
	}	


}	
