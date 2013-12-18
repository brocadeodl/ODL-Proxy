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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/PolicerGlobalWorker.h"
#include "Policer/Global/PolicerGlobalPriorityMapMessage.h"
#include "Policer/Global/PolicerGlobalClassMapMessage.h"
#include "Policer/Global/PolicerGlobalPolicyClassMapMessage.h"
#include "Policer/Global/PolicerPriorityMapManagedObject.h"
#include "Policer/Global/ClassMapManagedObject.h"
#include "Policer/Global/PolicyClassMapManagedObject.h"
#include "Policer/Global/PoClassMapManagedObject.h"
#include "Policer/Global/PolicerGlobalTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Policer/Local/PolicerPriorityMapMessage.h"
#include "Policer/Local/PolicerClassMapMessage.h"
#include "Policer/Local/PolicerPolicyClassMapMessage.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "ssm_policer_dcmd_public.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCStatus.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Common/QosUtils.h"
#include "Utils/DceClusterUtils.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Nsm/Global/NsmGlobalQosMapCleanUpMessage.h"




namespace DcmNs
{

    PolicerGlobalWorker::PolicerGlobalWorker ( PolicerGlobalObjectManager *pPolicerGlobalObjectManager)
        : WaveWorker  (pPolicerGlobalObjectManager)
    {
        PolicerPriorityMapManagedObject    PolicerPriorityMapManagedObject    (pPolicerGlobalObjectManager);
        ClassMapManagedObject    ClassMapManagedObject    (pPolicerGlobalObjectManager);
        PolicyClassMapManagedObject    PolicyClassMapManagedObject    (pPolicerGlobalObjectManager);
		PoClassMapManagedObject    PoClassMapManagedObject    (pPolicerGlobalObjectManager);
        PolicerPriorityMapManagedObject.setupOrm ();
        addManagedClass (PolicerPriorityMapManagedObject::getClassName (), this);
    	ClassMapManagedObject.setupOrm ();
        addManagedClass (ClassMapManagedObject::getClassName (), this);
        PolicyClassMapManagedObject.setupOrm ();
        addManagedClass (PolicyClassMapManagedObject::getClassName (), this);
        PoClassMapManagedObject.setupOrm ();
        addManagedClass (PoClassMapManagedObject::getClassName (), this);
		addOperationMap (POLICERGLOBALPRIORITYMAP, reinterpret_cast<PrismMessageHandler> (&PolicerGlobalWorker::PolicerGlobalPriorityMapMessageHandler));
        addOperationMap (POLICERGLOBALCLASSMAP, reinterpret_cast<PrismMessageHandler> (&PolicerGlobalWorker::PolicerGlobalClassMapMessageHandler));
        addOperationMap (POLICERGLOBALPOLICYCLASSMAP, reinterpret_cast<PrismMessageHandler> (&PolicerGlobalWorker::PolicerGlobalPolicyClassMapMessageHandler));
    }

    PolicerGlobalWorker::~PolicerGlobalWorker ()
    {
    }

    PrismMessage  *PolicerGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case POLICERGLOBALPRIORITYMAP :
                pPrismMessage = new PolicerGlobalPriorityMapMessage ();
                break;
            case POLICERGLOBALCLASSMAP :
                pPrismMessage = new PolicerGlobalClassMapMessage ();
                break;
            case POLICERGLOBALPOLICYCLASSMAP :
                pPrismMessage = new PolicerGlobalPolicyClassMapMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PolicerGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PolicerPriorityMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PolicerPriorityMapManagedObject(dynamic_cast<PolicerGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ClassMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((PoClassMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PolicyClassMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PolicyClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PolicerGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	void PolicerGlobalWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT)
                && (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }
        
		startTransaction();

		ClassMapManagedObject *ptr = new ClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()), "default", FB_POLICY_RULE_MATCH_INVALID,  "");
		ClassMapManagedObject *ptr2 = new ClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()), "cee", FB_POLICY_RULE_MATCH_INVALID, "");
		
		status = commitTransaction ();

        if (status == FRAMEWORK_SUCCESS) {
        	status = WAVE_MESSAGE_SUCCESS;
		}else{
			trace(TRACE_LEVEL_ERROR, "Failed to create default class map");
		}

        delete ptr;
        delete ptr2;

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
        return;

	}

	//Global Plugin code that handles the class map creation.

	void  PolicerGlobalWorker::PolicerGlobalClassMapMessageHandler( PolicerGlobalClassMapMessage *pPolicerGlobalClassMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::ClassMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerStartTransactionStep),
			
			reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::ClassMapMOStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPolicerGlobalClassMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }


	void PolicerGlobalWorker::ClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        PolicerGlobalClassMapMessage *pPolicerGlobalClassMapMessage = dynamic_cast<PolicerGlobalClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        WaveSendToClusterContext    *send_ctx_p;
        PolicerClassMapMessage *m = NULL;
        string acccessListName = pPolicerGlobalClassMapMessage->getAccessListName();
        UI32 macACLCount = 0;
        UI32 ipV4ACLCount = 0;
        UI32 ipV6ACLCount = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        //Validate the existence of the access list.

			if ("" != acccessListName) {

			//Verfiy whether a L2 ACL or L3 ACL exists with the referred acl name.

			WaveManagedObjectSynchronousQueryContext syncQueryCtxtMacACL(
					MacAclPolicyManagedObject::getClassName());
			syncQueryCtxtMacACL.addAndAttribute(
					new AttributeString(acccessListName, "policyName"));
			querySynchronouslyForCount(&syncQueryCtxtMacACL, macACLCount);

			WaveManagedObjectSynchronousQueryContext syncQueryCtxtIPACL(
					IpAclPolicyManagedObject::getClassName());
			syncQueryCtxtIPACL.addAndAttribute(
					new AttributeString(acccessListName, "policyName"));
			querySynchronouslyForCount(&syncQueryCtxtIPACL, ipV4ACLCount);

			WaveManagedObjectSynchronousQueryContext syncQueryCtxtIPv6ACL(
					Ip6AclPolicyManagedObject::getClassName());
			syncQueryCtxtIPv6ACL.addAndAttribute(
					new AttributeString(acccessListName, "policyName"));
			querySynchronouslyForCount(&syncQueryCtxtIPv6ACL, ipV6ACLCount);

			if ((0 == macACLCount) && (0 == ipV4ACLCount) && (0 == ipV6ACLCount)) {
				status = WRC_SSM_QOS_FLOWBASED_ACL_DOESNT_EXIST;
				pPrismLinearSequencerContext->executeNextStep(status);
				return;
			}
		    }

        m = new PolicerClassMapMessage();        
        m->setClassMapName(pPolicerGlobalClassMapMessage->getClassMapName());
        m->setAny(pPolicerGlobalClassMapMessage->getAny());
        m->setOpcode(pPolicerGlobalClassMapMessage->getOpcode());
        m->setFilterCriteria(pPolicerGlobalClassMapMessage->getFilterCriteria());
        m->setAccessListName(acccessListName);

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&PolicerGlobalWorker::PolicerGlobalSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
        trace(
			TRACE_LEVEL_DEBUG,
			string("Sending msg to Cluster : ClassMapName : <")
					+ m->getClassMapName() + "> AccessListName : <"
					+ m->getAccessListName() + "> Opcode : <"
					+ m->getOpcode() + ">");

        sendToWaveCluster(send_ctx_p);
        return ;
    }

    void PolicerGlobalWorker::ClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        int ExistMO=0;
        //int DeleteMap=0;
        PolicerGlobalClassMapMessage *pPolicerGlobalClassMapMessage = dynamic_cast<PolicerGlobalClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        ClassMapManagedObject *pClassMapManagedObject = NULL;
        string mapName = pPolicerGlobalClassMapMessage->getClassMapName();
        //if(pPolicerGlobalClassMapMessage->getOpcode()== SSM_POL_CLASSMAP_DELETE)
        //    DeleteMap=1;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ClassMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&mapName,"classMapName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
          UI32 numberOfResults = pResults->size ();
          if (0 == numberOfResults) {
            trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
            ExistMO = 0;
          } else if (1 == numberOfResults) {
            pClassMapManagedObject  = dynamic_cast<ClassMapManagedObject*>((*pResults)[0]);
            trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
            ExistMO = 1;
          }
        }

        if(pPolicerGlobalClassMapMessage->getOpcode()==FB_CLASSMAP_DELETE)
        {
            if(ExistMO==1)
			{
				//if code comes here means backend has deleted the class map and hence plugin should also remove all reference to this classmap and delete it.
				deleteReferenceToClassMap(pClassMapManagedObject->getObjectId(), pPrismLinearSequencerContext);
                delete pClassMapManagedObject;
			}
        }
        else
        {
          if(ExistMO==1){
            trace (TRACE_LEVEL_DEVEL, "MO already existing. Update MO");
            updateWaveManagedObject (pClassMapManagedObject);
            prismAssert (NULL != pClassMapManagedObject, __FILE__, __LINE__);
          }
          else {
            trace (TRACE_LEVEL_DEVEL, "MO not existing. Creating MO");
            pClassMapManagedObject = new ClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
            prismAssert (NULL != pClassMapManagedObject, __FILE__, __LINE__);
          }

        trace (TRACE_LEVEL_DEVEL, string("CLASS_CREATE") + pPolicerGlobalClassMapMessage->getAny());
        pClassMapManagedObject->setClassMapName(pPolicerGlobalClassMapMessage->getClassMapName());
        pClassMapManagedObject->setAccessListName(pPolicerGlobalClassMapMessage->getAccessListName());
        pClassMapManagedObject->setFilterCriteria(pPolicerGlobalClassMapMessage->getFilterCriteria());
        }

		if((pClassMapManagedObject)&&(pPolicerGlobalClassMapMessage->getOpcode()!= FB_CLASSMAP_DELETE))
		{
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pClassMapManagedObject);
		}

		if (pResults) {
        			delete pResults;
		}

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }

	void PolicerGlobalWorker::deleteReferenceToClassMap(ObjectId classMapObjId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PolicyClassMapManagedObject::getClassName());
        syncQueryCtxt.addSelectField("poManObj");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
          UI32 numberOfResults = pResults->size ();
          if (0 == numberOfResults) {
            trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
          } 
		  else{
			for(UI32 count=0; count < numberOfResults; count++)
			{
            	PolicyClassMapManagedObject *pPolicyClassMapManagedObject  = dynamic_cast<PolicyClassMapManagedObject*>((*pResults)[count]);
				if(pPolicyClassMapManagedObject != NULL)
				{
					 vector<WaveManagedObjectPointer<PoClassMapManagedObject> > vectPoClassMO;
					 vectPoClassMO = pPolicyClassMapManagedObject->getPoManObj();
					 for (UI32 i = 0; i < vectPoClassMO.size(); i++) {
            			if (vectPoClassMO[i]->getClassMapId() == classMapObjId){
							trace (TRACE_LEVEL_DEVEL, "deleting poclassmapMo");
						    prismAssert (NULL != pPolicyClassMapManagedObject, __FILE__, __LINE__);
							pPolicyClassMapManagedObject->deletePoManObj(classMapObjId);
            			}
        			 }

					 pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPolicyClassMapManagedObject);
				}
                pPolicyClassMapManagedObject->addAttributeToBeUpdated("poManObj");
                updateWaveManagedObject (pPolicyClassMapManagedObject);
			}
          }
		  delete pResults;
		}	
	}

	void PolicerGlobalWorker::deleteReferenceToPriorityMap(ObjectId prioMapObjId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PolicyClassMapManagedObject::getClassName());
        syncQueryCtxt.addSelectField("poManObj");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
          UI32 numberOfResults = pResults->size ();
          if (0 == numberOfResults) {
            trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
          }
          else{
            for(UI32 count=0; count < numberOfResults; count++)
            {
                PolicyClassMapManagedObject *pPolicyClassMapManagedObject  = dynamic_cast<PolicyClassMapManagedObject*>((*pResults)[count]);
                if(pPolicyClassMapManagedObject != NULL)
                {
                     vector<WaveManagedObjectPointer<PoClassMapManagedObject> > vectPoClassMO;
                     vectPoClassMO = pPolicyClassMapManagedObject->getPoManObj();
                     for (UI32 i = 0; i < vectPoClassMO.size(); i++) {
                        if (vectPoClassMO[i]->getPriorityMapId() == prioMapObjId){
                            trace (TRACE_LEVEL_DEVEL, "PriorityMapId set to NULL");
							vectPoClassMO[i]->setPriorityMapId(ObjectId::NullObjectId);
                            vectPoClassMO[i]->addAttributeToBeUpdated("priorityMapId");
                            updateWaveManagedObject (vectPoClassMO[i].operator->());
                            prismAssert (NULL != vectPoClassMO[i].operator->(), __FILE__, __LINE__);
                        }
                     }

                     pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPolicyClassMapManagedObject);
                }
            }
          }
          delete pResults;
        }
    }


    void PolicerGlobalWorker::PolicerGlobalPolicyClassMapMessageHandler(
		PolicerGlobalPolicyClassMapMessage *pPolicerGlobalPolicyClassMapMessage) {

	PrismLinearSequencerStep sequencerSteps[] =
			{
					//Policy map validations.
					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::PolicyMapValidationStep),

					//Back end daemon messaging.
					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::PolicyMapStep),
					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerStartTransactionStep),

					//DB Updation.
					//Need to clear the references of the policy-map from all the disconnected nodes.
					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::cleanupServicePolicyMapReferenceInFailedOverNodes),
					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::PolicyMapMOStep),
					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerCommitTransactionStep),

					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerSucceededStep),
					reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerFailedStep) };

	DceLinearSeqContext *pPrismLinearSequencerContext = new DceLinearSeqContext(
			pPolicerGlobalPolicyClassMapMessage, this, sequencerSteps,
			sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

	pPrismLinearSequencerContext->start();
}

    void PolicerGlobalWorker::PolicyMapValidationStep(
		DceLinearSeqContext *pPrismLinearSequencerContext) {

	PolicerGlobalPolicyClassMapMessage *pPolicerGlobalPolicyClassMapMessage =
			dynamic_cast<PolicerGlobalPolicyClassMapMessage*>(pPrismLinearSequencerContext->getPPrismMessage());

	ResourceId status = WAVE_MESSAGE_SUCCESS;

	//Fetch the policy map MO.
	string mapName =
			pPolicerGlobalPolicyClassMapMessage->getPolicyClassMapName();
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
			PolicyClassMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(
			new AttributeString(&mapName, "policyClassMapName"));
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	PolicyClassMapManagedObject *pPolicyClassMapManagedObject = NULL;
	ObjectId policyMapObjectId;

	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size();
		if (0 == numberOfResults) {
			trace(TRACE_LEVEL_DEVEL, "numberOfResults = 0");
			pPrismLinearSequencerContext->setPWaveManagedObject(NULL);
		} else if (1 == numberOfResults) {
			pPolicyClassMapManagedObject =
					dynamic_cast<PolicyClassMapManagedObject*>((*pResults)[0]);

			//Save the PolicyClassMapManagedObject, which can be used in further sequencer steps.
			pPrismLinearSequencerContext->setPWaveManagedObject(
					pPolicyClassMapManagedObject);
			trace(TRACE_LEVEL_DEVEL, "numberOfResults = 1");
		} else {
			status = WAVE_MESSAGE_ERROR;
			goto next;
		}
	}

	switch (pPolicerGlobalPolicyClassMapMessage->getOpcode()) {

	//For policy map delete operation, validate whether the policy map is applied on any of the interfaces [ isl or edge ].
	case FB_POLICYMAP_DELETE : {

		trace(TRACE_LEVEL_INFO, "Deletion of policy map is triggered.");
		policyMapObjectId = pPolicyClassMapManagedObject->getObjectId();
		UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext phyIntfQueryCtxt(
				PhyIntfLocalManagedObject::getClassName());
		phyIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"inputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		phyIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"outputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		phyIntfQueryCtxt.setFilterByConnectedNodes();
		status = querySynchronouslyForCount(&phyIntfQueryCtxt, count);

		trace(
				TRACE_LEVEL_INFO,
				string(
						"Nuumber of interfaces where the policy map is associated is  : ")
						+ count);

		if (WAVE_MESSAGE_SUCCESS == status) {
			if (count > 0) {
				status = WRC_SSM_POL_ERR_POLICY_MAP_IN_USE;
                //The PolicyClassMapManagedObject is already queried above.
                //We need to add the same for garbage collection in this error scenario.
                //Because, in this error scenario, the PolicyClassMapManagedObject doesn't get deleted 
                //since the subsequent sequencer steps are not executed for an error scenario.
                pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
						pPolicyClassMapManagedObject);
                goto next; 
			}
		}

		//Also verify whether the policy-map to be deleted is referred in a port-channel.
		count = 0;
		WaveManagedObjectSynchronousQueryContext poIntfQueryCtxt(
				PoIntfManagedObject::getClassName());
		poIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"inputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		poIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"outputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		status = querySynchronouslyForCount(&poIntfQueryCtxt, count);

		trace(
				TRACE_LEVEL_INFO,
				string(
						"Nuumber of port-channels where the policy map is associated is  : ")
						+ count);

		if (WAVE_MESSAGE_SUCCESS == status) {
			if (count > 0) {
				status = WRC_SSM_POL_ERR_POLICY_MAP_IN_USE;
                //The PolicyClassMapManagedObject is already queried above.
                //We need to add the same for garbage collection in this error scenario.
                //Because, in this error scenario, the PolicyClassMapManagedObject doesn't get deleted
                //since the subsequent sequencer steps are not executed for an error scenario.
                pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
						pPolicyClassMapManagedObject);
                goto next;
			}
		}


		break;
	}

	case FB_DEL_CLASS_MAP_TO_POLICY_MAP: {

		trace(TRACE_LEVEL_INFO,
				"Deletion of class map from policy map is triggered.");
		policyMapObjectId = pPolicyClassMapManagedObject->getObjectId();
		UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext phyIntfQueryCtxt(
				PhyIntfLocalManagedObject::getClassName());
		phyIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"inputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		phyIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"outputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		phyIntfQueryCtxt.setFilterByConnectedNodes();
		status = querySynchronouslyForCount(&phyIntfQueryCtxt, count);

		trace(TRACE_LEVEL_INFO,string(
						"Nuumber of interfaces where the policy map is associated is  : ") + count);

		if (WAVE_MESSAGE_SUCCESS == status) {
            if (count > 0) {
                if (pPolicyClassMapManagedObject->getPoManObj().size() <= 1) {
                    status = WRC_SSM_QOS_FLOWBASED_CANT_DEL_LAST_ACTIVE_CLASS;
                    goto next;
                }
            }
        }

		//Also verify whether the policy-map to be deleted is referred in a port-channel.
		count = 0;
		WaveManagedObjectSynchronousQueryContext poIntfQueryCtxt(
				PoIntfManagedObject::getClassName());
		poIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"inputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		poIntfQueryCtxt.addOrAttribute(
				new AttributeObjectIdAssociation(policyMapObjectId,
						"outputPolicyMapId",
						PolicyClassMapManagedObject::getClassName()));
		status = querySynchronouslyForCount(&poIntfQueryCtxt, count);

		trace(
				TRACE_LEVEL_INFO,
				string(
						"Nuumber of port-channels where the policy map is associated is  : ")
						+ count);

        if (WAVE_MESSAGE_SUCCESS == status) {
            if (count > 0) {
                if (pPolicyClassMapManagedObject->getPoManObj().size() <= 1) {
                    status = WRC_SSM_QOS_FLOWBASED_CANT_DEL_LAST_ACTIVE_CLASS;
                    goto next;
                }
            }
        }

		break;
	}
    
    //The below 2 defects contradict each other.
    //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
    //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.


    /*case FB_SET_COS_TC_ACTION_CREATE: {
        if((status = QosUtils::vcsQosSanity(QOS_INTERFACE_DEFAULT_COS, "", true, 0, 0, pPolicerGlobalPolicyClassMapMessage->getCoS()))
                != WAVE_MESSAGE_SUCCESS) {
            goto next;
        }
        if((status = QosUtils::vcsQosSanity(QOS_INTERFACE_DEFAULT_COS, "", true, 0, 0, pPolicerGlobalPolicyClassMapMessage->getTrafficClass()))
                != WAVE_MESSAGE_SUCCESS) {
            goto next;
        }
        break;
    }*/

	case FB_COS_MUTATION_MAP_ACTION_CREATE : {
		string cosMutationMapName = pPolicerGlobalPolicyClassMapMessage->getCoSMutationMapName();
		if(!QosUtils::doesMapExist(COS_MUTATION_MAP,cosMutationMapName,status)) {
			goto next;
        }
        if ((status = QosUtils::vcsQosSanity(QOS_INTERFACE_COS_MUTATION, cosMutationMapName,true,0,0,0)) 
                != WAVE_MESSAGE_SUCCESS) {
            goto next;
		}
		break;
	}

	case FB_COS_TRAFFIC_CLASS_MAP_ACTION_CREATE : {
		string cosTrafficClassMapName = pPolicerGlobalPolicyClassMapMessage->getCoSTrafficClassMapName();
		if(!QosUtils::doesMapExist(COS_TRAFFIC_CLASS_MAP,cosTrafficClassMapName,status)) {
			goto next;
		}
		break;
	}

	case FB_DSCP_MUTATION_MAP_ACTION_CREATE : {
		string dscpMutationMapName = pPolicerGlobalPolicyClassMapMessage->getDscpMutationMapName();
		if(!QosUtils::doesMapExist(DSCP_MUTATION_MAP,dscpMutationMapName,status)) {
			goto next;
		}
		break;
	}

	case FB_DSCP_COS_MAP_ACTION_CREATE : {
		string dscpCoSMapName = pPolicerGlobalPolicyClassMapMessage->getDscpCoSMapName();
        int reservePriorityBitmap = 0;
        if((status = QosUtils::getReservePriorityBitMapFromCeeMap(reservePriorityBitmap)) 
                != WAVE_MESSAGE_SUCCESS) {
            goto next;
        }

		if(!QosUtils::doesMapExist(DSCP_COS_MAP,dscpCoSMapName,status)) {
			goto next;
		}
        if ((status = QosUtils::vcsL3QosSanity(QOS_INTERFACE_DSCP_COS, dscpCoSMapName, true, reservePriorityBitmap, 0))
                != WAVE_MESSAGE_SUCCESS) {
            goto next;
        }
		break;
	}

	case FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_CREATE : {
		string dscpTrafficClassMapName = pPolicerGlobalPolicyClassMapMessage->getDscpTrafficClassMapName();
        int reservePriorityBitmap = 0;
        if((status = QosUtils::getReservePriorityBitMapFromCeeMap(reservePriorityBitmap))
                != WAVE_MESSAGE_SUCCESS) {
            goto next;
        }
        
		if(!QosUtils::doesMapExist(DSCP_TRAFFIC_CLASS_MAP,dscpTrafficClassMapName,status)) {
			goto next;
		}
        if ((status = QosUtils::vcsL3QosSanity(QOS_INTERFACE_DSCP_TRAFFIC_CLASS, dscpTrafficClassMapName, true, reservePriorityBitmap, 0))
                != WAVE_MESSAGE_SUCCESS) {
            goto next;
        }
		break;
	}

	 case FB_SFLOW_MAP_ACTION_CREATE : {
		string sflowMapName = pPolicerGlobalPolicyClassMapMessage->getSflowMapName();
		if(!QosUtils::doesMapExist(SFLOW_POLICY_MAP,sflowMapName,status)) {
			goto next;
		}
		break;
	}
	case FB_CEE_ACTION_CREATE : {
		string ceeMapName = pPolicerGlobalPolicyClassMapMessage->getCeeMapName();
		if(!QosUtils::doesMapExist(CEE_MAP,ceeMapName,status)) {
			goto next;
		}
		break;
	}

	case FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE: {

		UI32 sumOfDWRR =
				pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass0()
						+ pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass1()
						+ pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass2()
						+ pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass3()
						+ pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass4()
						+ pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass5()
						+ pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass6()
						+ pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass7();

		if (FB_DWRR_SUM != sumOfDWRR) {
			status = WRC_SSM_QOS_DWRR_WEIGHTS_SUM_NOT_100;
			goto next;
		}
		break;
	}
	}

	//Clean up of the created objects.
	next:

	if (pResults) {
        //Deletion of pPolicyClassMapManagedObject.
        //The below statement makes sure the object is deleted only in case of policer edit operation.
        //Handle the object deletion for create and delete scenarios in MO update step.
        if( (pPrismLinearSequencerContext) && (NULL!= pPolicyClassMapManagedObject)
        		&& (pPolicerGlobalPolicyClassMapMessage->getOpcode() != FB_POLICYMAP_DELETE)
                && (pPolicerGlobalPolicyClassMapMessage->getOpcode() != FB_POLICYMAP_CREATE)) {
            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPolicyClassMapManagedObject);
        }
		pResults->clear();
		delete pResults;
	}

	pPrismLinearSequencerContext->executeNextStep(status);

}

	
	void PolicerGlobalWorker::PolicyMapStep(DceLinearSeqContext *pPrismLinearSequencerContext)
    {
        PolicerGlobalPolicyClassMapMessage *pPolicerGlobalPolicyClassMapMessage = dynamic_cast<PolicerGlobalPolicyClassMapMessage*> 

(pPrismLinearSequencerContext->getPPrismMessage ());

        WaveSendToClusterContext    *send_ctx_p;

        trace(TRACE_LEVEL_DEBUG, string("input mapName ") + pPolicerGlobalPolicyClassMapMessage->getPolicyClassMapName());

		trace(TRACE_LEVEL_DEBUG, string("conformDscp ") + pPolicerGlobalPolicyClassMapMessage->getConformDscp());
		trace(TRACE_LEVEL_DEBUG, string("ConformPrecedence ") + pPolicerGlobalPolicyClassMapMessage->getConformPrecedence());
		trace(TRACE_LEVEL_DEBUG, string("ConformType ") + pPolicerGlobalPolicyClassMapMessage->getConformType());
		trace(TRACE_LEVEL_DEBUG, string("ExceedDscp ") + pPolicerGlobalPolicyClassMapMessage->getExceedDscp());
		trace(TRACE_LEVEL_DEBUG, string("ExceedPrecedence ") + pPolicerGlobalPolicyClassMapMessage->getExceedPrecedence());
		trace(TRACE_LEVEL_DEBUG, string("ExceedType ") + pPolicerGlobalPolicyClassMapMessage->getExceedType());
		trace(TRACE_LEVEL_DEBUG, string("ExceedTc ") + pPolicerGlobalPolicyClassMapMessage->getExceedTc());
		trace(TRACE_LEVEL_DEBUG, string("Opcode ") + pPolicerGlobalPolicyClassMapMessage->getOpcode());

		trace(TRACE_LEVEL_DEBUG, string("COS ") + pPolicerGlobalPolicyClassMapMessage->getCoS());
		trace(TRACE_LEVEL_DEBUG, string("TC ") + pPolicerGlobalPolicyClassMapMessage->getTrafficClass());
		trace(TRACE_LEVEL_DEBUG, string("DSCP ") + pPolicerGlobalPolicyClassMapMessage->getDscp());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE ") + pPolicerGlobalPolicyClassMapMessage->getShapingRate());
		trace(TRACE_LEVEL_DEBUG, string("COSMUTATION MAP NAME: ") + pPolicerGlobalPolicyClassMapMessage->getCoSMutationMapName());
		trace(TRACE_LEVEL_DEBUG, string("COS TRAFFIC CLASS MAP NAME: ") + pPolicerGlobalPolicyClassMapMessage->getCoSTrafficClassMapName());
		trace(TRACE_LEVEL_DEBUG, string("DSCP COS MAP NAME: ") + pPolicerGlobalPolicyClassMapMessage->getDscpCoSMapName());
		trace(TRACE_LEVEL_DEBUG, string("DSCP MUTATION MAP NAME: ") + pPolicerGlobalPolicyClassMapMessage->getDscpMutationMapName());
		trace(TRACE_LEVEL_DEBUG, string("DSCP TRAFFIC CLASS MAP NAME: ") + pPolicerGlobalPolicyClassMapMessage->getDscpTrafficClassMapName());
		trace(TRACE_LEVEL_DEBUG, string("CEE MAP NAME: ") + pPolicerGlobalPolicyClassMapMessage->getCeeMapName());
		trace(TRACE_LEVEL_DEBUG, string("STRICT PRIORITY : ") + pPolicerGlobalPolicyClassMapMessage->getStrictPriority());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC0: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass0());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC1: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass1());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC2: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass2());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC3: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass3());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC4: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass4());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC5: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass5());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC6: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass6());
		trace(TRACE_LEVEL_DEBUG, string("DWRR FOR TC7: ") + pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass7());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC0: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass0());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC1: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass1());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC2: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass2());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC3: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass3());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC4: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass4());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC5: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass5());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC6: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass6());
		trace(TRACE_LEVEL_DEBUG, string("SHAPING RATE  FOR TC7: ") + pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass7());



		if(pPolicerGlobalPolicyClassMapMessage->getOpcode() == FB_ADD_CLASS_MAP_TO_POLICY_MAP)
		{
			ObjectId classObjectId=getIdFromClassMapName(pPolicerGlobalPolicyClassMapMessage->getClassMapId(), pPrismLinearSequencerContext);
            trace(TRACE_LEVEL_DEBUG, string("Saving ClassMapId in DceCache as: ") + classObjectId.toString());
            pPrismLinearSequencerContext->getCache()->put("ClassMapId", classObjectId );
			if(classObjectId == ObjectId::NullObjectId)
			{
				pPrismLinearSequencerContext->executeNextStep(WRC_SSM_POL_ERR_CLASS_MAP_NOT_FOUND);
				return;
			}
		}
		else if((pPolicerGlobalPolicyClassMapMessage->getOpcode() == FB_SET_POLICER_ATTR)&&(!(pPolicerGlobalPolicyClassMapMessage->getPriorityMapId().empty())))
		{
			ObjectId priorityObjectId=getIdFromPriorityMapName(pPolicerGlobalPolicyClassMapMessage->getPriorityMapId(), pPrismLinearSequencerContext);
            trace(TRACE_LEVEL_DEBUG, string("Saving PriorityMapId in DceCache as: ") + priorityObjectId.toString());
            pPrismLinearSequencerContext->getCache()->put("PriorityMapId",priorityObjectId);

			if(priorityObjectId == ObjectId::NullObjectId)
			{
				pPrismLinearSequencerContext->executeNextStep(WRC_SSM_POL_ERR_PRIO_MAP_NOT_FOUND);
				return;
			}
		}

		PolicerPolicyClassMapMessage *m = new PolicerPolicyClassMapMessage();
				
			
        m->setPolicyClassMapName(pPolicerGlobalPolicyClassMapMessage->getPolicyClassMapName());
        m->setCir(pPolicerGlobalPolicyClassMapMessage->getCir());
		m->setCbs(pPolicerGlobalPolicyClassMapMessage->getCbs());
		m->setEir(pPolicerGlobalPolicyClassMapMessage->getEir());
		m->setEbs(pPolicerGlobalPolicyClassMapMessage->getEbs());
		m->setConformDscp(pPolicerGlobalPolicyClassMapMessage->getConformDscp());
		m->setConformPrecedence(pPolicerGlobalPolicyClassMapMessage->getConformPrecedence());
		m->setConformType(pPolicerGlobalPolicyClassMapMessage->getConformType());
		m->setConformTc(pPolicerGlobalPolicyClassMapMessage->getConformTc());
		m->setExceedDscp(pPolicerGlobalPolicyClassMapMessage->getExceedDscp());
		m->setExceedPrecedence(pPolicerGlobalPolicyClassMapMessage->getExceedPrecedence());
		m->setExceedType(pPolicerGlobalPolicyClassMapMessage->getExceedType());
		m->setExceedTc(pPolicerGlobalPolicyClassMapMessage->getExceedTc());
		m->setClassMapId(pPolicerGlobalPolicyClassMapMessage->getClassMapId());
		m->setPriorityMapId(pPolicerGlobalPolicyClassMapMessage->getPriorityMapId());
        m->setOpcode(pPolicerGlobalPolicyClassMapMessage->getOpcode());

        m->setCoS(pPolicerGlobalPolicyClassMapMessage->getCoS());
        m->setTrafficClass(pPolicerGlobalPolicyClassMapMessage->getTrafficClass());
        m->setDscp(pPolicerGlobalPolicyClassMapMessage->getDscp());

        m->setShapingRate(pPolicerGlobalPolicyClassMapMessage->getShapingRate());

        m->setCoSMutationMapName(pPolicerGlobalPolicyClassMapMessage->getCoSMutationMapName());
        m->setCoSTrafficClassMapName(pPolicerGlobalPolicyClassMapMessage->getCoSTrafficClassMapName());
        m->setDscpCoSMapName(pPolicerGlobalPolicyClassMapMessage->getDscpCoSMapName());
        m->setDscpMutationMapName(pPolicerGlobalPolicyClassMapMessage->getDscpMutationMapName());
        m->setDscpTrafficClassMapName(pPolicerGlobalPolicyClassMapMessage->getDscpTrafficClassMapName());
        m->setSflowMapName(pPolicerGlobalPolicyClassMapMessage->getSflowMapName());
        m->setCeeMapName(pPolicerGlobalPolicyClassMapMessage->getCeeMapName());


        m->setStrictPriority(pPolicerGlobalPolicyClassMapMessage->getStrictPriority());

        m->setDwrrTrafficClass0(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass0());
        m->setDwrrTrafficClass1(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass1());
        m->setDwrrTrafficClass2(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass2());
        m->setDwrrTrafficClass3(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass3());
        m->setDwrrTrafficClass4(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass4());
        m->setDwrrTrafficClass5(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass5());
        m->setDwrrTrafficClass6(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass6());
        m->setDwrrTrafficClass7(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass7());

        m->setShapingRateTrafficClass0(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass0());
        m->setShapingRateTrafficClass1(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass1());
        m->setShapingRateTrafficClass2(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass2());
        m->setShapingRateTrafficClass3(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass3());
        m->setShapingRateTrafficClass4(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass4());
        m->setShapingRateTrafficClass5(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass5());
        m->setShapingRateTrafficClass6(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass6());
        m->setShapingRateTrafficClass7(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass7());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&PolicerGlobalWorker::PolicerGlobalSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
        trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pPolicerGlobalPolicyClassMapMessage->getPolicyClassMapName());
        sendToWaveCluster(send_ctx_p);
        return ;
    }



	/*
	 * When a qos service-policy map is active on an interface and the node to which this interface belongs to is down,
	 * the deletion of this map should be allowed.
	 * And also stale entries from the database should be cleaned.
	 */
	void PolicerGlobalWorker::cleanupServicePolicyMapReferenceInFailedOverNodes(
			DceLinearSeqContext *pPrismLinearSequencerContext) {

		PolicerGlobalPolicyClassMapMessage *pPolicerGlobalPolicyClassMapMessage =
				dynamic_cast<PolicerGlobalPolicyClassMapMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
		ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;

		if (FB_POLICYMAP_DELETE
				== pPolicerGlobalPolicyClassMapMessage->getOpcode()) {

			trace(
					TRACE_LEVEL_DEBUG,
				"PolicerGlobalWorker::cleanupServicePolicyMapReferenceInFailedOverNodes");
		string qosServicePolicyMapName =
				pPolicerGlobalPolicyClassMapMessage->getPolicyClassMapName();
		NsmGlobalQosMapCleanUpMessage *pMsg = NULL;
		pMsg = new NsmGlobalQosMapCleanUpMessage(qosServicePolicyMapName,
				QOS_INTERFACE_SERVICE_POLICY);
		status = sendSynchronously(pMsg);
		trace(
				TRACE_LEVEL_DEBUG,
				"PolicerGlobalWorker::cleanupServicePolicyMapReferenceInFailedOverNodes status ="
						+ status);
		delete (pMsg);
		}
		pPrismLinearSequencerContext->executeNextStep(status);
		return;
	}


    void PolicerGlobalWorker::PolicyMapMOStep(DceLinearSeqContext *pPrismLinearSequencerContext)
    {
        int ExistMO=0;
        //int DeleteMap=0;
		ResourceId status = WAVE_MESSAGE_ERROR;
	    PolicerGlobalPolicyClassMapMessage *pPolicerGlobalPolicyClassMapMessage = dynamic_cast<PolicerGlobalPolicyClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        PolicyClassMapManagedObject *pPolicyClassMapManagedObject = NULL;
        PoClassMapManagedObject *pPoClassMapManagedObject = NULL;

        pPolicyClassMapManagedObject =
			dynamic_cast<PolicyClassMapManagedObject *>(pPrismLinearSequencerContext->getPWaveManagedObject());

		if (NULL == pPolicyClassMapManagedObject) {
			trace(TRACE_LEVEL_INFO, "PolicyClassMapManagedObject doesnt exist");
			ExistMO = 0;
		} else {
			trace(TRACE_LEVEL_INFO, "PolicyClassMapManagedObject exists");
			ExistMO = 1;
		}

        if(pPolicerGlobalPolicyClassMapMessage->getOpcode()==FB_POLICYMAP_DELETE)
        {
            if(ExistMO==1)
                delete pPolicyClassMapManagedObject;
			status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
          //update the parent managed object, only when a child is added or deleted
          if(ExistMO==1 && ((FB_ADD_CLASS_MAP_TO_POLICY_MAP
					== pPolicerGlobalPolicyClassMapMessage->getOpcode())
					|| ((FB_DEL_CLASS_MAP_TO_POLICY_MAP
							== pPolicerGlobalPolicyClassMapMessage->getOpcode())))){

            trace (TRACE_LEVEL_DEVEL, "MO already existing. Update MO");

                updateWaveManagedObject (pPolicyClassMapManagedObject);
                prismAssert (NULL != pPolicyClassMapManagedObject, __FILE__, __LINE__);
          }
          else if ( ExistMO == 0 ) {
            trace (TRACE_LEVEL_DEVEL, "MO not existing. Creating MO");
            pPolicyClassMapManagedObject = new PolicyClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
            prismAssert (NULL != pPolicyClassMapManagedObject, __FILE__, __LINE__);
          }
          else {
              trace(
					TRACE_LEVEL_DEVEL,
					"Mo exists and this is an edit operation on child MO, hence update only the child MO.");
          }

			trace (TRACE_LEVEL_DEVEL, "POLICY_CREATE");
			pPolicyClassMapManagedObject->setPolicyClassMapName(pPolicerGlobalPolicyClassMapMessage->getPolicyClassMapName());

			//check if the class map MO exists
//			ObjectId classObjectId=getIdFromClassMapName(pPolicerGlobalPolicyClassMapMessage->getClassMapId(), pPrismLinearSequencerContext);
            ObjectId classObjectId;

            if ( ObjectId::NullObjectId == pPrismLinearSequencerContext->getCache()->getObjectId("ClassMapId") ) {
                classObjectId = getIdFromClassMapName(pPolicerGlobalPolicyClassMapMessage->getClassMapId(), pPrismLinearSequencerContext);
            } else {
                classObjectId = pPrismLinearSequencerContext->getCache()->getObjectId("ClassMapId");
                trace(TRACE_LEVEL_DEBUG, string("Retrieving ClassMapId from DceCache as: ") + classObjectId.toString());
            }
			int ExistClassMO = 0;

			if(classObjectId != ObjectId::NullObjectId)
			{
				status = WAVE_MESSAGE_SUCCESS;
                pPoClassMapManagedObject = pPolicyClassMapManagedObject->getPoManObjByObjectId(classObjectId);
				ObjectId priorityObjectIdOld;
				if((pPoClassMapManagedObject)!=NULL)
				{
					trace (TRACE_LEVEL_DEVEL, "Class map MO exists");
					ExistClassMO = 1;
					priorityObjectIdOld = pPoClassMapManagedObject->getPriorityMapId();
					//pPolicyClassMapManagedObject->deletePoManObj(classObjectId);
				}
			
				switch ( pPolicerGlobalPolicyClassMapMessage->getOpcode() ) {
				case FB_ADD_CLASS_MAP_TO_POLICY_MAP :
				case FB_SET_POLICER_ATTR :
				case FB_CLEAR_POLICER_ATTR :
				{
					trace (TRACE_LEVEL_DEVEL, "CLASS_CREATE_OR_SET");

					//If the PolicyClassMapMO is already present in a PolicyMapMO, we just need to update the PolicyClassMapMO.
					//Else we need to create a PolicyClassMapMO and update the parent PolicyMapMO.
					if( !ExistClassMO ) {
					pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					} else {
						updateWaveManagedObject(pPoClassMapManagedObject);
					}

					if(!ExistClassMO)
					{
						trace (TRACE_LEVEL_DEVEL, "Class map MO does not exists");
					}
		
					pPoClassMapManagedObject->setCir(pPolicerGlobalPolicyClassMapMessage->getCir());
					pPoClassMapManagedObject->setCbs(pPolicerGlobalPolicyClassMapMessage->getCbs());
					pPoClassMapManagedObject->setEir(pPolicerGlobalPolicyClassMapMessage->getEir());
					pPoClassMapManagedObject->setEbs(pPolicerGlobalPolicyClassMapMessage->getEbs());
					
					pPoClassMapManagedObject->setConformDscp(pPolicerGlobalPolicyClassMapMessage->getConformDscp());
					pPoClassMapManagedObject->setConformPrecedence(pPolicerGlobalPolicyClassMapMessage->getConformPrecedence());
					pPoClassMapManagedObject->setConformType(pPolicerGlobalPolicyClassMapMessage->getConformType());
					pPoClassMapManagedObject->setConformTc(pPolicerGlobalPolicyClassMapMessage->getConformTc());
					pPoClassMapManagedObject->setExceedDscp(pPolicerGlobalPolicyClassMapMessage->getExceedDscp());
					pPoClassMapManagedObject->setExceedPrecedence(pPolicerGlobalPolicyClassMapMessage->getExceedPrecedence());
					pPoClassMapManagedObject->setExceedType(pPolicerGlobalPolicyClassMapMessage->getExceedType());
					pPoClassMapManagedObject->setExceedTc(pPolicerGlobalPolicyClassMapMessage->getExceedTc());
					
//					ObjectId priorityObjectId=getIdFromPriorityMapName(pPolicerGlobalPolicyClassMapMessage->getPriorityMapId(), pPrismLinearSequencerContext);
                    ObjectId priorityObjectId ;

                    if ( ObjectId::NullObjectId == pPrismLinearSequencerContext->getCache()->getObjectId("PriorityMapId") ) {
                        priorityObjectId = getIdFromPriorityMapName(pPolicerGlobalPolicyClassMapMessage->getPriorityMapId(), pPrismLinearSequencerContext);
                    } else {
                        priorityObjectId = pPrismLinearSequencerContext->getCache()->getObjectId("PriorityMapId");
                        trace(TRACE_LEVEL_DEBUG, string("Retrieving PriorityMapId from DceCache as: ") + priorityObjectId.toString());
                    }
					pPoClassMapManagedObject->setClassMapId(classObjectId);
					
					if((pPolicerGlobalPolicyClassMapMessage->getPriorityMapId()).empty())
					{
						trace (TRACE_LEVEL_DEVEL, "priority map null");
						pPoClassMapManagedObject->setPriorityMapId(ObjectId::NullObjectId);
					}
					else
					{
							pPoClassMapManagedObject->setPriorityMapId(priorityObjectId);
					}
					
					if( !ExistClassMO ) {
					pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
				}
					break;
				}
				case FB_DEL_CLASS_MAP_TO_POLICY_MAP :
				{
					if(ExistClassMO)
							pPolicyClassMapManagedObject->deletePoManObj(classObjectId);
					break;
				}

				case FB_SET_COS_TC_ACTION_CREATE: {
					trace(TRACE_LEVEL_DEVEL, "FB_SET_COS_TC_ACTION_CREATE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setCoS(
							pPolicerGlobalPolicyClassMapMessage->getCoS());
					pPoClassMapManagedObject->setTrafficClass(
							pPolicerGlobalPolicyClassMapMessage->getTrafficClass());
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}
				case FB_SET_COS_TC_ACTION_DELETE: {
					trace(TRACE_LEVEL_DEVEL, "FB_SET_COS_TC_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setCoS(
							pPolicerGlobalPolicyClassMapMessage->getCoS());
					pPoClassMapManagedObject->setTrafficClass(
							pPolicerGlobalPolicyClassMapMessage->getTrafficClass());
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}
				case FB_SET_DSCP_ACTION_CREATE: {
					trace(TRACE_LEVEL_DEVEL, "FB_SET_DSCP_ACTION_CREATE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setDscp(
							pPolicerGlobalPolicyClassMapMessage->getDscp());
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}
				case FB_SET_DSCP_ACTION_DELETE: {
					trace(TRACE_LEVEL_DEVEL, "FB_SET_DSCP_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setDscp(
							pPolicerGlobalPolicyClassMapMessage->getDscp());
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}
				case FB_COS_MUTATION_MAP_ACTION_CREATE: {
					trace(TRACE_LEVEL_DEVEL, "FB_COS_MUTATION_MAP_ACTION_CREATE");
					ObjectId cosMutationMapId = ObjectId::NullObjectId;
					status =
							QosUtils::getQosMapObjectIdByMapName(
									pPolicerGlobalPolicyClassMapMessage->getCoSMutationMapName(),
									cosMutationMapId, COS_MUTATION_MAP, pPrismLinearSequencerContext);

					if (status != WAVE_MESSAGE_SUCCESS){
					    trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName tcMap failed"));
					    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
					    return;
					}

					pPoClassMapManagedObject->setCoSMutationMap(cosMutationMapId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;
				}

				case FB_COS_MUTATION_MAP_ACTION_DELETE: {
					trace(TRACE_LEVEL_DEVEL, "FB_COS_MUTATION_MAP_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setCoSMutationMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}

				case FB_COS_TRAFFIC_CLASS_MAP_ACTION_CREATE : {
					trace(TRACE_LEVEL_DEVEL, "FB_COS_TRAFFIC_CLASS_MAP_ACTION_CREATE");
					ObjectId cosTrafficClassMapId = ObjectId::NullObjectId;
					status =
							QosUtils::getQosMapObjectIdByMapName(
									pPolicerGlobalPolicyClassMapMessage->getCoSTrafficClassMapName(),
									cosTrafficClassMapId, COS_TRAFFIC_CLASS_MAP, pPrismLinearSequencerContext);

					if (status != WAVE_MESSAGE_SUCCESS){
					    trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName tcMap failed"));
					    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
					    return;
					}

					pPoClassMapManagedObject->setCoSTrafficClassMap(cosTrafficClassMapId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;
				}
				case FB_COS_TRAFFIC_CLASS_MAP_ACTION_DELETE : {
					trace(TRACE_LEVEL_DEVEL, "FB_COS_TRAFFIC_CLASS_MAP_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setCoSTrafficClassMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}

				case FB_DSCP_MUTATION_MAP_ACTION_CREATE : {

					trace(TRACE_LEVEL_DEVEL, "FB_DSCP_MUTATION_MAP_ACTION_CREATE");
					ObjectId dscpMutationMapId = ObjectId::NullObjectId;
					status =
							QosUtils::getQosMapObjectIdByMapName(
									pPolicerGlobalPolicyClassMapMessage->getDscpMutationMapName(),
									dscpMutationMapId, DSCP_MUTATION_MAP, pPrismLinearSequencerContext);

					if (status != WAVE_MESSAGE_SUCCESS){
					    trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName tcMap failed"));
					    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
					    return;
					}

					pPoClassMapManagedObject->setDscpMutationMap(dscpMutationMapId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;

				}

				case FB_DSCP_MUTATION_MAP_ACTION_DELETE : {
					trace(TRACE_LEVEL_DEVEL, "FB_DSCP_MUTATION_MAP_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setDscpMutationMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}
				case FB_DSCP_COS_MAP_ACTION_CREATE : {

					trace(TRACE_LEVEL_DEVEL, "FB_DSCP_COS_MAP_ACTION_CREATE");
					ObjectId dscpCoSMapId = ObjectId::NullObjectId;
					status =
							QosUtils::getQosMapObjectIdByMapName(
									pPolicerGlobalPolicyClassMapMessage->getDscpCoSMapName(),
									dscpCoSMapId, DSCP_COS_MAP, pPrismLinearSequencerContext);

					if (status != WAVE_MESSAGE_SUCCESS){
					    trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName tcMap failed"));
					    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
					    return;
					}

					pPoClassMapManagedObject->setDscpCoSMap(dscpCoSMapId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;

				}
				case FB_DSCP_COS_MAP_ACTION_DELETE : {
					trace(TRACE_LEVEL_DEVEL, "FB_DSCP_COS_MAP_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setDscpCoSMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}

				case FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_CREATE : {

					trace(TRACE_LEVEL_DEVEL, "FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_CREATE");
					ObjectId dscpTrafficClassMapId = ObjectId::NullObjectId;
					status =
							QosUtils::getQosMapObjectIdByMapName(
									pPolicerGlobalPolicyClassMapMessage->getDscpTrafficClassMapName(),
									dscpTrafficClassMapId, DSCP_TRAFFIC_CLASS_MAP, pPrismLinearSequencerContext);

					if (status != WAVE_MESSAGE_SUCCESS){
					    trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName tcMap failed"));
					    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
					    return;
					}

					pPoClassMapManagedObject->setDscpTrafficClassMap(dscpTrafficClassMapId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;

				}

				case FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_DELETE : {
					trace(TRACE_LEVEL_DEVEL, "FB_DSCP_TRAFFIC_CLASS_MAP_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setDscpTrafficClassMap(
							ObjectId::NullObjectId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}

				case FB_CEE_ACTION_CREATE : {

					trace(TRACE_LEVEL_DEVEL, "FB_CEE_ACTION_CREATE");
					ObjectId ceeMapId = ObjectId::NullObjectId;
					status =
							QosUtils::getQosMapObjectIdByMapName(
									pPolicerGlobalPolicyClassMapMessage->getCeeMapName(),
									ceeMapId, CEE_MAP, pPrismLinearSequencerContext);

					if (status != WAVE_MESSAGE_SUCCESS){
						trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName ceeMapId failed"));
						pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
						return;
					}

					pPoClassMapManagedObject->setCeeMap(ceeMapId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;
				}

				case FB_CEE_ACTION_DELETE : {

					trace(TRACE_LEVEL_DEVEL, "FB_CEE_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setCeeMap(
					 ObjectId::NullObjectId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}

				case FB_SFLOW_MAP_ACTION_CREATE : {

					trace(TRACE_LEVEL_DEVEL, "FB_SFLOW_MAP_ACTION_CREATE");
					ObjectId sflowMapId = ObjectId::NullObjectId;
					status =
							QosUtils::getQosMapObjectIdByMapName(
									pPolicerGlobalPolicyClassMapMessage->getSflowMapName(),
									sflowMapId, SFLOW_POLICY_MAP, pPrismLinearSequencerContext);

					if (status != WAVE_MESSAGE_SUCCESS){
						trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName sflowMapId failed"));
						pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
						return;
					}
					pPoClassMapManagedObject->setSflowMap(sflowMapId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;
				}

				case FB_SFLOW_MAP_ACTION_DELETE : {
					trace(TRACE_LEVEL_DEVEL, "FB_SFLOW_MAP_ACTION_DELETE");
					pPoClassMapManagedObject->setSflowMap(
					 ObjectId::NullObjectId);
					updateWaveManagedObject(pPoClassMapManagedObject);
					break;
				}

				case FB_SHAPING_RATE_ACTION_CREATE :
				case FB_SHAPING_RATE_ACTION_DELETE : {
					trace(TRACE_LEVEL_DEVEL, "FB_SHAPING_RATE_ACTION_CREATE or FB_SHAPING_RATE_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);
					pPoClassMapManagedObject->setShapingRate(pPolicerGlobalPolicyClassMapMessage->getShapingRate());
					updateWaveManagedObject(pPoClassMapManagedObject);
					//pPolicyClassMapManagedObject->addPoManObj(pPoClassMapManagedObject);
					break;
				}

				case FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE :
				case FB_QOS_QUEUE_SCHEDULER_ACTION_DELETE : {
					trace(TRACE_LEVEL_DEVEL, "FB_QOS_QUEUE_SCHEDULER_ACTION_CREATE or FB_QOS_QUEUE_SCHEDULER_ACTION_DELETE");
					//pPoClassMapManagedObject =  new PoClassMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
					//pPoClassMapManagedObject->setClassMapId(classObjectId);

					pPoClassMapManagedObject->setStrictPriority(pPolicerGlobalPolicyClassMapMessage->getStrictPriority());

					pPoClassMapManagedObject->setDwrrTrafficClass0(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass0());
					pPoClassMapManagedObject->setDwrrTrafficClass1(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass1());
					pPoClassMapManagedObject->setDwrrTrafficClass2(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass2());
					pPoClassMapManagedObject->setDwrrTrafficClass3(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass3());
					pPoClassMapManagedObject->setDwrrTrafficClass4(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass4());
					pPoClassMapManagedObject->setDwrrTrafficClass5(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass5());
					pPoClassMapManagedObject->setDwrrTrafficClass6(pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass6());

					switch (pPolicerGlobalPolicyClassMapMessage->getStrictPriority()) {

					case 0:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass7());
						break;
					case 1:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass6());
						pPoClassMapManagedObject->setDwrrTrafficClass6(0);
						break;
					case 2:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass5());
						pPoClassMapManagedObject->setDwrrTrafficClass5(0);
						break;
					case 3:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass4());
						pPoClassMapManagedObject->setDwrrTrafficClass4(0);
						break;
					case 4:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass3());
						pPoClassMapManagedObject->setDwrrTrafficClass3(0);
						break;
					case 5:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass2());
						pPoClassMapManagedObject->setDwrrTrafficClass2(0);
						break;
					case 6:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass1());
						pPoClassMapManagedObject->setDwrrTrafficClass1(0);
						break;
					case 7:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								pPolicerGlobalPolicyClassMapMessage->getDwrrTrafficClass0());
						pPoClassMapManagedObject->setDwrrTrafficClass0(0);
						break;
					case FB_DEFAULT_STRICT_PRIORITY_VALUE:
						pPoClassMapManagedObject->setDwrrTrafficClassLast(
								FB_DEFAULT_QUEUE_DWRR_VALUE);
						break;
					}

					pPoClassMapManagedObject->setShapingRateTrafficClass0(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass0());
					pPoClassMapManagedObject->setShapingRateTrafficClass1(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass1());
					pPoClassMapManagedObject->setShapingRateTrafficClass2(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass2());
					pPoClassMapManagedObject->setShapingRateTrafficClass3(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass3());
					pPoClassMapManagedObject->setShapingRateTrafficClass4(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass4());
					pPoClassMapManagedObject->setShapingRateTrafficClass5(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass5());
					pPoClassMapManagedObject->setShapingRateTrafficClass6(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass6());
					pPoClassMapManagedObject->setShapingRateTrafficClass7(pPolicerGlobalPolicyClassMapMessage->getShapingRateTrafficClass7());

					updateWaveManagedObject(pPoClassMapManagedObject);
					break;
				}
			}
			}
			else
			{
				if(pPolicerGlobalPolicyClassMapMessage->getOpcode() != FB_POLICYMAP_CREATE)
				{
					//TODO put a proper error message
					status = WRC_SSM_POL_ERR_CLASS_MAP_NOT_FOUND;
				}
				else
					status = WAVE_MESSAGE_SUCCESS;
			}
			
		}

        if(pPrismLinearSequencerContext && 
            ( FB_POLICYMAP_CREATE == pPolicerGlobalPolicyClassMapMessage->getOpcode())) {
                pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPolicyClassMapManagedObject);
        }


			
        pPrismLinearSequencerContext->executeNextStep(status);
        return;
    }

	ObjectId PolicerGlobalWorker::getIdFromClassMapName(string Name, PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ClassMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&Name,"classMapName"));
        syncQueryCtxt.addSelectField("objectId");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
          UI32 numberOfResults = pResults->size ();
          if (0 == numberOfResults) {
          	return ObjectId::NullObjectId;
		  } else if (1 == numberOfResults) {
            ClassMapManagedObject *pClassMapManagedObject  = dynamic_cast<ClassMapManagedObject*>((*pResults)[0]);
			trace (TRACE_LEVEL_DEVEL, "class id exists");
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pClassMapManagedObject);
          	return pClassMapManagedObject->getObjectId();
		  }
        }

		trace (TRACE_LEVEL_DEVEL, "class id NOT exists");
		return ObjectId::NullObjectId;
	}

	ObjectId PolicerGlobalWorker::getIdFromPriorityMapName(string Name,PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PolicerPriorityMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&Name,"priorityMapName"));
        syncQueryCtxt.addSelectField("objectId");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
          UI32 numberOfResults = pResults->size ();
          if (0 == numberOfResults) {
            return ObjectId::NullObjectId;
          } else if (1 == numberOfResults) {
            PolicerPriorityMapManagedObject *pPriorityMapManagedObject  = dynamic_cast<PolicerPriorityMapManagedObject*>((*pResults)[0]);
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPriorityMapManagedObject);
            return pPriorityMapManagedObject->getObjectId();
          }
        }
		
        return ObjectId::NullObjectId;
    }





    void  PolicerGlobalWorker::PolicerGlobalPriorityMapMessageHandler( PolicerGlobalPriorityMapMessage *pPolicerGlobalPriorityMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::PriorityMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

			reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::PriorityMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&PolicerGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPolicerGlobalPriorityMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

	void PolicerGlobalWorker::PriorityMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		PolicerGlobalPriorityMapMessage *pPolicerGlobalPriorityMapMessage = dynamic_cast<PolicerGlobalPriorityMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

		WaveSendToClusterContext    *send_ctx_p;
		PolicerPriorityMapMessage *m = new PolicerPriorityMapMessage();

		
		trace(TRACE_LEVEL_DEBUG, string("input mapName ") + pPolicerGlobalPriorityMapMessage->getPriorityMapName());

		m->setPriorityMapName(pPolicerGlobalPriorityMapMessage->getPriorityMapName());
		m->setConformPriorityMap0(pPolicerGlobalPriorityMapMessage->getConformPriorityMap0());
		m->setConformPriorityMap1(pPolicerGlobalPriorityMapMessage->getConformPriorityMap1());
		m->setConformPriorityMap2(pPolicerGlobalPriorityMapMessage->getConformPriorityMap2());
		m->setConformPriorityMap3(pPolicerGlobalPriorityMapMessage->getConformPriorityMap3());
		m->setConformPriorityMap4(pPolicerGlobalPriorityMapMessage->getConformPriorityMap4());
		m->setConformPriorityMap5(pPolicerGlobalPriorityMapMessage->getConformPriorityMap5());
		m->setConformPriorityMap6(pPolicerGlobalPriorityMapMessage->getConformPriorityMap6());
		m->setConformPriorityMap7(pPolicerGlobalPriorityMapMessage->getConformPriorityMap7());
		m->setExceedPriorityMap0(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap0());
		m->setExceedPriorityMap1(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap1());
		m->setExceedPriorityMap2(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap2());
		m->setExceedPriorityMap3(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap3());
		m->setExceedPriorityMap4(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap4());
		m->setExceedPriorityMap5(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap5());
		m->setExceedPriorityMap6(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap6());
		m->setExceedPriorityMap7(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap7());
		m->setConformCos(pPolicerGlobalPriorityMapMessage->getConformCos());
		m->setExceedCos(pPolicerGlobalPriorityMapMessage->getExceedCos());
		m->setOpcode(pPolicerGlobalPriorityMapMessage->getOpcode());
	
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&PolicerGlobalWorker::PolicerGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pPolicerGlobalPriorityMapMessage->getPriorityMapName());
		send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
		sendToWaveCluster(send_ctx_p);
		return ;
	}

    void PolicerGlobalWorker::PriorityMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		int ExistMO=0;
		//int DeleteMap=0;
		PolicerGlobalPriorityMapMessage *pPolicerGlobalPriorityMapMessage = dynamic_cast<PolicerGlobalPriorityMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		PolicerPriorityMapManagedObject *pPolicerPriorityMapManagedObject = NULL;
		string mapName = pPolicerGlobalPriorityMapMessage->getPriorityMapName();
		//if(pPolicerGlobalPriorityMapMessage->getOpcode()== SSM_POL_PRIORITYMAP_DELETE)
		//	DeleteMap=1;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PolicerPriorityMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&mapName,"PriorityMapName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
		  UI32 numberOfResults = pResults->size ();
		  if (0 == numberOfResults) {
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
		  	ExistMO = 0;
		  } else if (1 == numberOfResults) {
			pPolicerPriorityMapManagedObject  = dynamic_cast<PolicerPriorityMapManagedObject*>((*pResults)[0]);
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
			ExistMO = 1;
		  }
		}

		if(pPolicerGlobalPriorityMapMessage->getOpcode()==SSM_POL_PRIORITYMAP_DELETE)
		{
			if(ExistMO==1)
			{
				deleteReferenceToPriorityMap(pPolicerPriorityMapManagedObject->getObjectId(),pPrismLinearSequencerContext);
				delete pPolicerPriorityMapManagedObject;
			}
		}
		else
		{
		  
          vector<string> listOfAttributes;
          
          if(ExistMO==1){
		  	trace (TRACE_LEVEL_DEVEL, "MO already existing. Update MO");
		  	updateWaveManagedObject (pPolicerPriorityMapManagedObject);
		  	prismAssert (NULL != pPolicerPriorityMapManagedObject, __FILE__, __LINE__);
		  } 
		  else {
		  	trace (TRACE_LEVEL_DEVEL, "MO not existing. Creating MO");
		  	pPolicerPriorityMapManagedObject = new PolicerPriorityMapManagedObject(dynamic_cast<PolicerGlobalObjectManager*>(getPWaveObjectManager()));
		    prismAssert (NULL != pPolicerPriorityMapManagedObject, __FILE__, __LINE__);
		  }
	
			trace (TRACE_LEVEL_DEVEL, string("opcode =")+pPolicerGlobalPriorityMapMessage->getOpcode());
			trace (TRACE_LEVEL_DEVEL, string("conform flag=")+pPolicerGlobalPriorityMapMessage->getConformCos());
			trace (TRACE_LEVEL_DEVEL, string("exceed flag=")+pPolicerGlobalPriorityMapMessage->getExceedCos());

			pPolicerPriorityMapManagedObject->setPriorityMapName(pPolicerGlobalPriorityMapMessage->getPriorityMapName());
            listOfAttributes.push_back("priorityMapName");

			if(pPolicerGlobalPriorityMapMessage->getConformCos()==1)
			{
				pPolicerPriorityMapManagedObject->setConformPriorityMap0(pPolicerGlobalPriorityMapMessage->getConformPriorityMap0());
				pPolicerPriorityMapManagedObject->setConformPriorityMap1(pPolicerGlobalPriorityMapMessage->getConformPriorityMap1());
				pPolicerPriorityMapManagedObject->setConformPriorityMap2(pPolicerGlobalPriorityMapMessage->getConformPriorityMap2());
				pPolicerPriorityMapManagedObject->setConformPriorityMap3(pPolicerGlobalPriorityMapMessage->getConformPriorityMap3());
				pPolicerPriorityMapManagedObject->setConformPriorityMap4(pPolicerGlobalPriorityMapMessage->getConformPriorityMap4());
				pPolicerPriorityMapManagedObject->setConformPriorityMap5(pPolicerGlobalPriorityMapMessage->getConformPriorityMap5());
				pPolicerPriorityMapManagedObject->setConformPriorityMap6(pPolicerGlobalPriorityMapMessage->getConformPriorityMap6());
				pPolicerPriorityMapManagedObject->setConformPriorityMap7(pPolicerGlobalPriorityMapMessage->getConformPriorityMap7());
                
                //Partial DB Update.
                listOfAttributes.push_back("conformPriorityMap0");
                listOfAttributes.push_back("conformPriorityMap1");
                listOfAttributes.push_back("conformPriorityMap2");
                listOfAttributes.push_back("conformPriorityMap3");
                listOfAttributes.push_back("conformPriorityMap4");
                listOfAttributes.push_back("conformPriorityMap5");
                listOfAttributes.push_back("conformPriorityMap6");
                listOfAttributes.push_back("conformPriorityMap7");

                pPolicerPriorityMapManagedObject->setAttributesToBeUpdated(listOfAttributes);


			}

			if(pPolicerGlobalPriorityMapMessage->getExceedCos() == 1)
			{
				pPolicerPriorityMapManagedObject->setExceedPriorityMap0(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap0());
				pPolicerPriorityMapManagedObject->setExceedPriorityMap1(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap1());
				pPolicerPriorityMapManagedObject->setExceedPriorityMap2(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap2());
				pPolicerPriorityMapManagedObject->setExceedPriorityMap3(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap3());
				pPolicerPriorityMapManagedObject->setExceedPriorityMap4(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap4());
				pPolicerPriorityMapManagedObject->setExceedPriorityMap5(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap5());
				pPolicerPriorityMapManagedObject->setExceedPriorityMap6(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap6());
				pPolicerPriorityMapManagedObject->setExceedPriorityMap7(pPolicerGlobalPriorityMapMessage->getExceedPriorityMap7());

                //Partial DB Update
                listOfAttributes.push_back("exceedPriorityMap0");
                listOfAttributes.push_back("exceedPriorityMap1");
                listOfAttributes.push_back("exceedPriorityMap2");
                listOfAttributes.push_back("exceedPriorityMap3");
                listOfAttributes.push_back("exceedPriorityMap4");
                listOfAttributes.push_back("exceedPriorityMap5");
                listOfAttributes.push_back("exceedPriorityMap6");
                listOfAttributes.push_back("exceedPriorityMap7");

                pPolicerPriorityMapManagedObject->setAttributesToBeUpdated(listOfAttributes);
			}
		}

		if((pPolicerPriorityMapManagedObject)&&(pPolicerGlobalPriorityMapMessage->getOpcode()!=SSM_POL_PRIORITYMAP_DELETE))
				pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPolicerPriorityMapManagedObject);
		
	    if (pResults) {
					delete pResults;
        }

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
  		return;
	}
    void PolicerGlobalWorker::PolicerGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p) {
    
        trace (TRACE_LEVEL_DEBUG, "Inside PolicerGlobalSendToClusterCallback");
        PrismLinearSequencerContext             *seq_ctx_p;
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        vector<LocationId>                      locationIdsInCluster;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        
        if ( WAVE_MESSAGE_SUCCESS == sendToClusterCompletionStatus ) {
            trace (TRACE_LEVEL_INFO, "MQC Global Command succeeded on all the nodes.");
        } else {
            trace (TRACE_LEVEL_INFO, "MQC Global Command failed on few or all the nodes.");
            locationIdsInCluster = ctx_p->getLocationsToSendToForPhase1();
            
            for (UI32 i = 0; i < locationIdsInCluster.size(); i++) {
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locationIdsInCluster[i]);
                
                if(WAVE_MESSAGE_SUCCESS != statusPerLocationId) {
                    trace(TRACE_LEVEL_ERROR, string("MQC config command failed on the node<")
                                    + locationIdsInCluster[i] + ">, with an error: " + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        }

        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        trace (TRACE_LEVEL_DEBUG, string("PolicerGlobalSendToClusterCallback:") + sendToClusterCompletionStatus);
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
    }	

}
