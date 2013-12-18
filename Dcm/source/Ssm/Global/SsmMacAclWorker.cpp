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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmMacAclWorker.h"
#include "Ssm/Global/SsmGlobalAddMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalGetMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalRemMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocRuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalReseqMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocPolicyToInterfaceMessage.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Ssm/Global/StdMacAclPolicyManagedObject.h"
#include "Ssm/Global/ExtMacAclPolicyManagedObject.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "Ssm/Global/StdMacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Ssm/Local/SsmLocalServiceAddMacAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceRemMacAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceAssocRuleToPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceResqMacAclPolicyMessage.h"
#include "Nsm/Global/NsmGlobalReAssocMacAclPolicyMessage.h"
#include "APPM/Global/APPMGlobalAPPMSecurityMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "DcmCStatus.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressMacAclPolicyManagedObject.h"

#include "Nsm/Global/Ssm/SsmDeleteAssociationwithDisconnectedMessage.h"
#include <string>
#include <sstream>
#include "Ssm/Global/SsmPlugUtils.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Utils/DceClusterUtils.h"
#include "Qos/Common/QosUtils.h"

namespace DcmNs
{

    UI32  SsmMacAclWorker::ssmIsmacPolicyAttachInterface(const string& policy, DceLinearSeqContext *seqContext) {

        WaveManagedObjectSynchronousQueryContext
            aclSyncQueryCtxt(MacAclPolicyManagedObject::getClassName());

        aclSyncQueryCtxt.addAndAttribute (new AttributeString (policy, "policyName"));
		// adds the field in the query context to make use of the partial query
		aclSyncQueryCtxt.addSelectField ("objectId");
        vector<WaveManagedObject *> *pResultsP  = querySynchronously (&aclSyncQueryCtxt);
        if (pResultsP) {
            ObjectId policyId                   = ObjectId::NullObjectId;
            MacAclPolicyManagedObject* policyMO = NULL;
            if (pResultsP->size()) {
                policyMO = dynamic_cast<MacAclPolicyManagedObject *>((*pResultsP)[0]);
                if (policyMO) {
                    policyId                    = policyMO->getObjectId();
					// storing the policy if into the sequencer context to reuse it.
					seqContext->setWaveManagedObjectId(policyId);

					UI32 aclAssocCount = 0;
					WaveManagedObjectSynchronousQueryContext pSyncQueryCtx(NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName());
					pSyncQueryCtx.addAndAttribute(new AttributeObjectIdAssociation(policyId, "ingressEgressMacAcl",MacAclPolicyManagedObject::getClassName()));
					pSyncQueryCtx.setFilterByConnectedNodes(true);
					ResourceId status = querySynchronouslyForCount(&pSyncQueryCtx, aclAssocCount);

					if ( WAVE_MESSAGE_SUCCESS == status )
					{
						if ( aclAssocCount > 0 )
						{
							trace (TRACE_LEVEL_INFO, policy + string (" MAC ACL applied on the interface."));
							return (1);
						}
					}
					else
					{
						trace (TRACE_LEVEL_ERROR, "NsmLocalIngressEgressMacAclPolicyManagedObject database query failed");
					}

					/*
                    IntfManagedObject *intfMO   = NULL;
                    UI32 size                   = 0;
                    string intfManagedObjectClassName[3] = {
                    								PhyIntfLocalManagedObject::getClassName(),
                    								PoIntfManagedObject::getClassName(),
                    								VlanIntfManagedObject::getClassName()
                    							};
                    WaveManagedObjectSynchronousQueryContext *syncQueryCtxt = NULL;

                    for( size_t strIndex = 0; strIndex < 3; strIndex ++ ) {
                    	syncQueryCtxt = new WaveManagedObjectSynchronousQueryContext(intfManagedObjectClassName[strIndex]);
                    	if(strIndex == 0) {
                    		// For PhyIntfLocalManagedObject only
                    		syncQueryCtxt->setFilterByConnectedNodes(true);
                    	}
    					syncQueryCtxt->addSelectField ("ingressEgressMacAclPolicy");
                        vector<WaveManagedObject *> *pResultsMO = querySynchronously (syncQueryCtxt);
                         if (NULL != pResultsMO) {
                            UI32 numberOfResults = pResultsMO->size ();
                            for (UI32 i = 0; i < numberOfResults; i++) {
                                intfMO = dynamic_cast<IntfManagedObject *>((*pResultsMO)[i]);
                                if (intfMO != NULL) {
                                    vector<WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> >
                                        NsmIngressEgressMacAclPolicyMoVector = intfMO->getIngressEgressMacAclPolicy();
                                    size = NsmIngressEgressMacAclPolicyMoVector.size();
                                    for (UI32 j = 0; j < size; j++) {
                                        if (((NsmIngressEgressMacAclPolicyMoVector[j])->getIngressEgressMacAcl()) == policyId) {
                                            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsMO);
    										seqContext->addManagedObjectsForGarbageCollection(*pResultsP);
    										if (syncQueryCtxt) {
    											delete syncQueryCtxt;
    										}
                                            return (1);
    									}
    								}
    							}
    						}
    						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsMO);
    					}
 						if (syncQueryCtxt) {
 							delete syncQueryCtxt;
 						}
    				}
                    */
                }
			}
            seqContext->addManagedObjectsForGarbageCollection(*pResultsP);
            delete pResultsP;
		}		
		return 0;
	}
	SsmMacAclWorker::SsmMacAclWorker (SsmGlobalObjectManager *pSsmGlobalObjectManager )
     : WaveWorker  (pSsmGlobalObjectManager)
    {
        MacAclPolicyManagedObject    MacAclPolicyManagedObject    (pSsmGlobalObjectManager);
        StdMacAclPolicyManagedObject   StdMacAclPolicyManagedObject    (pSsmGlobalObjectManager);
        ExtMacAclPolicyManagedObject   ExtMacAclPolicyManagedObject    (pSsmGlobalObjectManager);
        MacAclRuleManagedObject    MacAclRuleManagedObject    (pSsmGlobalObjectManager);
        ExtMacAclRuleManagedObject    ExtMacAclRuleManagedObject    (pSsmGlobalObjectManager);
        StdMacAclRuleManagedObject    StdMacAclRuleManagedObject    (pSsmGlobalObjectManager);
        MacAclPolicyManagedObject.setupOrm ();
        addManagedClass (MacAclPolicyManagedObject::getClassName (), this);
        StdMacAclPolicyManagedObject.setupOrm ();
        addManagedClass (StdMacAclPolicyManagedObject::getClassName (), this);
        ExtMacAclPolicyManagedObject.setupOrm ();
        addManagedClass (ExtMacAclPolicyManagedObject::getClassName (), this);
        MacAclRuleManagedObject.setupOrm ();
        addManagedClass (MacAclRuleManagedObject::getClassName (), this);
        StdMacAclRuleManagedObject.setupOrm ();
        addManagedClass (StdMacAclRuleManagedObject::getClassName (), this);
        ExtMacAclRuleManagedObject.setupOrm ();
        addManagedClass (ExtMacAclRuleManagedObject::getClassName (), this);
        addOperationMap (SSMGLOBALADDMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmMacAclWorker::SsmGlobalAddMacAclPolicyMessageHandler));
        addOperationMap (SSMGLOBALREMMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmMacAclWorker::SsmGlobalRemMacAclPolicyMessageHandler));
        addOperationMap (SSMGLOBALASSOCRULETOPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmMacAclWorker::SsmGlobalAssocRuleToPolicyMessageHandler));
        addOperationMap (SSMGLOBALGETMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmMacAclWorker::SsmGlobalGetMacAclPolicyMessageHandler));
        addOperationMap (SSMGLOBALRESEQMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmMacAclWorker::SsmGlobalReseqMacAclPolicyMessageHandler));
	}

    SsmMacAclWorker::~SsmMacAclWorker ()
    {
    }

    PrismMessage  *SsmMacAclWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SSMGLOBALADDMACACLPOLICY :
                pPrismMessage = new SsmGlobalAddMacAclPolicyMessage ();
                break;
            case SSMGLOBALGETMACACLPOLICY :
                pPrismMessage = new SsmGlobalGetMacAclPolicyMessage ();
                break;
            case SSMGLOBALREMMACACLPOLICY :
                pPrismMessage = new SsmGlobalRemMacAclPolicyMessage ();
                break;
            case SSMGLOBALASSOCRULETOPOLICY :
                pPrismMessage = new SsmGlobalAssocRuleToPolicyMessage ();
                break;
            case SSMGLOBALRESEQMACACLPOLICY :
                pPrismMessage = new SsmGlobalReseqMacAclPolicyMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SsmMacAclWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((MacAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MacAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((StdMacAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new StdMacAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ExtMacAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ExtMacAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((MacAclRuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MacAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((StdMacAclRuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new StdMacAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
		}
        else if ((ExtMacAclRuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ExtMacAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "SsmMacAclWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }


    void SsmMacAclWorker::SsmGlobalGetMacAclPolicyMessageHandler(SsmGlobalGetMacAclPolicyMessage *pSsmGlobalGetMacAclPolicyMessage)
	{
		SSM_PLUG_DBG_TRACE();
		const string policyName = pSsmGlobalGetMacAclPolicyMessage->getPolicyName();
		MacAclPolicyManagedObject *policyMO = NULL;
		int status = WAVE_MESSAGE_ERROR;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(MacAclPolicyManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				startTransaction();
				policyMO = new MacAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
				if (policyMO) {
					policyMO->setPolicyName(policyName);
        			status = commitTransaction ();
        			if (status != FRAMEWORK_SUCCESS) {
        				SSM_PLUG_ERR_LVL("Policy MO commit failed");
       					status = WAVE_MESSAGE_ERROR;
        			} else {
        				SSM_PLUG_SUCCESS_LVL("Unknown Policy type Mo created");
						pSsmGlobalGetMacAclPolicyMessage->setPolicyObjId(policyMO->getObjectId ());
       					status = WAVE_MESSAGE_SUCCESS;
					}
					delete policyMO;
				} else {
					rollbackTransaction();
				}
			}else if (1 == numberOfResults) {
        		SSM_PLUG_INFO_LVL("Found Existing Policy");
				policyMO = dynamic_cast<MacAclPolicyManagedObject *>((*pResults)[0]);
				pSsmGlobalGetMacAclPolicyMessage->setPolicyObjId(policyMO->getObjectId ());
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				SSM_PLUG_ERR_LVL("Two or more object Exists");
				status = WAVE_MESSAGE_ERROR;
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		} else {
			status = WAVE_MESSAGE_ERROR;
        	SSM_PLUG_ERR_LVL("Query Policy Object Failed");
		}

		pSsmGlobalGetMacAclPolicyMessage->setCompletionStatus (status);
		reply(pSsmGlobalGetMacAclPolicyMessage);

	}

    void  SsmMacAclWorker::SsmGlobalAddMacAclPolicyMessageHandler( SsmGlobalAddMacAclPolicyMessage *pSsmGlobalAddMacAclPolicyMessage)
    {

    	SSM_PLUG_DBG_TRACE();

        PrismLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::validatePolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::createMacAclPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::createMacAclPolicyMO),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::deleteOldMacAclPolicyMO),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSsmGlobalAddMacAclPolicyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();

        pPrismLinearSequencerContext->start ();
    }


    void  SsmMacAclWorker::SsmGlobalAssocRuleToPolicyMessageHandler( SsmGlobalAssocRuleToPolicyMessage *pSsmGlobalAssocRuleToPolicyMessage)
    { 
    	SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pSsmGlobalAssocRuleToPolicyMessage);

    	UI32 opcode = (UI32)pSsmGlobalAssocRuleToPolicyMessage->getOpcode();
    	DceLinearSeqContext *pDCELinearSeqContext = NULL;

        PrismLinearSequencerStep AddsequencerSteps[] =
        {

        	reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::validateRule),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::udpateMacAclRuleToPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::assocMacAclRuleToPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerFailedStep)
        };
        
        PrismLinearSequencerStep RemsequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::validateRule),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::udpateMacAclRuleToPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::disAssocMacAclRuleToPolicy),

            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerFailedStep)
        };


        if (opcode == SSMACL_DCMD_ADD_RULE) {
        	pDCELinearSeqContext = new DceLinearSeqContext (pSsmGlobalAssocRuleToPolicyMessage, this, AddsequencerSteps, sizeof (AddsequencerSteps) /sizeof (AddsequencerSteps[0]));
		} else if (opcode == SSMACL_DCMD_DEL_RULE) {
			pDCELinearSeqContext = new DceLinearSeqContext (pSsmGlobalAssocRuleToPolicyMessage, this, RemsequencerSteps, sizeof (RemsequencerSteps) /sizeof (RemsequencerSteps[0]));
		}

		if (pDCELinearSeqContext) {
			pDCELinearSeqContext->holdAll ();
			pDCELinearSeqContext->start ();
		}
    }

    void  SsmMacAclWorker::SsmGlobalRemMacAclPolicyMessageHandler( SsmGlobalRemMacAclPolicyMessage *pSsmGlobalRemMacAclPolicyMessage)
    {
    	SSM_PLUG_DBG_TRACE();

        PrismLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::validateDeletePolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::deleteMacAclPolicy),
			reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::deleteAssociationwithDisconnectedPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::deleteMacAclPolicyMO),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::appmSecProfileUpdateStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pDCELinearSeqContext = new DceLinearSeqContext (pSsmGlobalRemMacAclPolicyMessage, this, 
			sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDCELinearSeqContext->holdAll ();

        pDCELinearSeqContext->start ();
    }

    string SsmMacAclWorker::convertToLower(string strToConvert)
	{
		for(unsigned int i=0;i<strToConvert.length();i++) {
			strToConvert[i] = tolower(strToConvert[i]);
		}

		return strToConvert;
	}
				

    void SsmMacAclWorker::validateRule(DceLinearSeqContext *seqContext) {
		SsmGlobalAssocRuleToPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalAssocRuleToPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);
		UI32 policyType								= pMsg->getPolicyType();
		const string policyName						= pMsg->getPolicyName();
		UI16 appTypeInt								= pMsg->getAppTypeInt();
		EtherType appTypeEnum						= pMsg->getAppTypeEnum();
		string srcMac								= "";
		string srcMacMask							= "";
		string dstMac								= "";
		string dstMacMask							= "";
		UI32 opcode                                 = (UI32) pMsg->getOpcode();
		UI64 seqid;
		string className;
		string childClassName;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		bool seqFound 								= false;
		bool ruleFound 								= false;
		UI32 UserEnteredSeqid 						= pMsg->getUserEnteredSeqid();
    	SSM_PLUG_DBG_TRACE();
		UI32 srcType 								= pMsg->getSrcType();	
		UI32 dstType 								= pMsg->getDstType();
        bool rulePropertiesExist                    = false;
		prismAssert(opcode == SSMACL_DCMD_ADD_RULE ||
				opcode == SSMACL_DCMD_DEL_RULE, __FILE__, __LINE__);

		
		if (srcType == ADDR_TYPE_ANY) {
			srcMac = "any";
		} else {
			srcMac 		= convertToLower(pMsg->getSrcMac());
			srcMacMask 	= convertToLower(pMsg->getSrcMacMask());
		}

		if (policyType == SSMACL_TYPE_L2_EXT_E) {
			if (dstType == ADDR_TYPE_ANY) {
			dstMac = "any";
		} else {
			dstMac 		= convertToLower(pMsg->getDstMac());
			dstMacMask  = convertToLower(pMsg->getDstMacMask());
		}
		}
		
		if (policyType == SSMACL_TYPE_L2_STD_E) {
			childClassName = StdMacAclRuleManagedObject::getClassName();
			className = StdMacAclPolicyManagedObject::getClassName();
		} else if (policyType == SSMACL_TYPE_L2_EXT_E) {
			childClassName = ExtMacAclRuleManagedObject::getClassName();
			className = ExtMacAclPolicyManagedObject::getClassName();
		} else {
			// This case should not appear and it is error case
			SSM_PLUG_INFO_LVL_S(string("MacAclPolicyManagedObject"));
			className = MacAclPolicyManagedObject::getClassName();
		}

        // Get the policy object id from message 
        ObjectId policyId = pMsg->getPolicyObjectId ();

        if (ObjectId::NullObjectId == policyId) 
        {
            // This means that the Policy ID isnt found in the incoming message, so query db for it

            /* Query the MO for a given policy*/
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
            // set the objectId as addSelectedField in the query
            syncQueryCtxt.addSelectFieldForObjectId ("objectId");
            syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
            syncQueryCtxt.setLoadOneToManyRelationships(false);
            vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

            if (NULL != pResultsPMO) {
                UI32 numberOfResults = pResultsPMO->size ();
                if (0 == numberOfResults) 
                {
                    SSM_PLUG_ERR_LVL("Can't find the policy Managed Object");
                    status = WRC_SSM_DCM_ERR_ACL_EEXIST;
                } 
                else if (1 == numberOfResults) 
                {
                    MacAclPolicyManagedObject *policyMO = dynamic_cast<MacAclPolicyManagedObject *>((*pResultsPMO)[0]);
                    policyId = policyMO->getObjectId();
                }
                else
                {
        	        SSM_PLUG_ERR_LVL("Two or more Objects exists");
                    status = WAVE_MESSAGE_ERROR;
        	    }
                
                seqContext->addManagedObjectsForGarbageCollection(*pResultsPMO);
                if (pResultsPMO)
                {
                    delete pResultsPMO;
                }
            }
        }

        UI64 seqIdTmp = pMsg->getRuleid();

        if (ObjectId::NullObjectId != policyId) 
        {
        	// set the policyId in the sequencer context
        	seqContext->setWaveManagedObjectId(policyId);
        	WaveManagedObjectSynchronousQueryContext queryCtxtForChild (childClassName);

        	queryCtxtForChild.createConditionSet ("RULEVALIDATE_POLICYID");
        	queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_POLICYID", new AttributeObjectId (policyId, "ownerManagedObjectId"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        	if (UserEnteredSeqid)
        	{
        		queryCtxtForChild.createConditionSet ("RULEVALIDATE_SEQID");
        		seqid = pMsg->getRuleid();
        		queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_SEQID", new AttributeUI64 (seqid, "ruleid"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        	}

        	if (((opcode == SSMACL_DCMD_DEL_RULE) && (!UserEnteredSeqid)) || ((opcode == SSMACL_DCMD_ADD_RULE)))
        	{
        		rulePropertiesExist = true;
        		queryCtxtForChild.createConditionSet ("RULEVALIDATE_PROPERTIES");
        		queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum(pMsg->getAction(), "action"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        		if (srcType == ADDR_TYPE_ANY)
        		{
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (srcType, "srcType"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        		}
        		else if (srcType == ADDR_TYPE_HOST)
        		{
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (srcType, "srcType"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeString(srcMac, "srcHost"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        		}
        		else if (srcType == ADDR_TYPE_NONE)
        		{
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeString(srcMac, "srcMac"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeString(srcMacMask, "srcMacMask"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        		}

        		if (policyType == SSMACL_TYPE_L2_EXT_E)
        		{
        			if (dstType == ADDR_TYPE_ANY)
        			{
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (dstType, "dstType"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}
        			else if (dstType == ADDR_TYPE_HOST)
        			{
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (dstType, "dstType"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeString(dstMac, "dstHost"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}
        			else if (dstType == ADDR_TYPE_NONE)
        			{
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeString(dstMac, "dstMac"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeString(dstMacMask, "dstMacMask"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (appTypeEnum, "appType"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (appTypeInt, "appTypeInt"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        		}
        	}

#if 0
        	UI64 tempSeqId = -1;

        	if(UserEnteredSeqid) {
        		tempSeqId = pMsg->getRuleid();
        	}
#endif

    		string rulePropertiesConditionStr = "";
    		string ruleSeqIdConditionStr = "";

        	if ( (UserEnteredSeqid || true == rulePropertiesExist) )
        	{
        		if(UserEnteredSeqid) {
        			ruleSeqIdConditionStr = "RULEVALIDATE_SEQID";
        		}

        		if(true == rulePropertiesExist ) {
        			if(UserEnteredSeqid) {
        				rulePropertiesConditionStr = "+";
        			}
        			rulePropertiesConditionStr = rulePropertiesConditionStr + "RULEVALIDATE_PROPERTIES";
        		}

        		queryCtxtForChild.combineConditionSets ("RULEVALIDATE_POLICYID*("+ruleSeqIdConditionStr+rulePropertiesConditionStr+")");
        		QuerychildValidation(queryCtxtForChild, status, ruleFound, seqFound, opcode, UserEnteredSeqid, seqid, policyType);
        	}

    		seqIdTmp = seqid;

        	if ( (status != WAVE_MESSAGE_SUCCESS) && (UserEnteredSeqid && true == rulePropertiesExist) )
        	{
        		seqid = pMsg->getRuleid();
        		seqFound = false;
        		ruleFound = false;

        		rulePropertiesConditionStr = "";
        		ruleSeqIdConditionStr = "";

        		if(UserEnteredSeqid) {
        			ruleSeqIdConditionStr = "RULEVALIDATE_SEQID";
        		}

        		if(true == rulePropertiesExist ) {
        			if(UserEnteredSeqid) {
        				rulePropertiesConditionStr = "*";
        			}
        			rulePropertiesConditionStr = rulePropertiesConditionStr + "RULEVALIDATE_PROPERTIES";
        		}

        		queryCtxtForChild.combineConditionSets ("RULEVALIDATE_POLICYID*("+ruleSeqIdConditionStr+rulePropertiesConditionStr+")");

        		QuerychildValidation(queryCtxtForChild, status, ruleFound, seqFound, opcode, UserEnteredSeqid, seqid, policyType);

        		if(seqFound)
            	{
            		status = WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE;
            	}
        	}

        	if ( (status != WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE) && UserEnteredSeqid )
            {
				if( (status != WAVE_MESSAGE_SUCCESS) && UserEnteredSeqid && (opcode == SSMACL_DCMD_DEL_RULE) && (!ruleFound))
				{
					status = WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE;
				}
            }
        }
		else 
        {
	        SSM_PLUG_ERR_LVL("MAC ACL poliy MO object does not exist");
            status = WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE;
	    }
		
        pMsg->setRuleid(seqIdTmp);
		/* Setting Seq id before moving to next step */
		if ((status != WAVE_MESSAGE_SUCCESS) && (seqFound || ruleFound) && (opcode == SSMACL_DCMD_ADD_RULE))
		{
			string localizedStatus = "";
			if(status == WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE)
			{
				localizedStatus = FrameworkToolKit::localize(status);
			}
			else
			{
				vector<Attribute *> errorArguments;
				errorArguments.push_back(new AttributeUI64(seqid, "seqid"));
				localizedStatus = FrameworkToolKit::localize (status, errorArguments);
				pMsg->addStatusPropagation (status, localizedStatus, FrameworkToolKit::getThisLocationId ());
				for (UI32 j = 0; j < static_cast<UI32> (errorArguments.size()); j++) {
					delete errorArguments[j];
				}
			}
			SSM_PLUG_INFO_LVL_S(string("Inside Status Propagation loop: "));
		}

		//SSM_PLUG_INFO_LVL_S(string("Return Status: ") + status + string("Seq id: ") + seqid);
		if (status != WAVE_MESSAGE_SUCCESS)
			pMsg->setCompletionStatus(status);

		seqContext->executeNextStep(status);
		return;
	}

	void SsmMacAclWorker::QuerychildValidation(WaveManagedObjectSynchronousQueryContext &queryCtxtForChild, ResourceId &status, bool &ruleFound, bool &seqFound, UI32 opcode, UI32 UserEnteredSeqid, UI64 &seqid, UI32 policyType) {

		vector<WaveManagedObject *> *pResultsChildMO = querySynchronously (&queryCtxtForChild);
		UI64 tempSeqid = seqid;
		UI32 numberOfResultsChild = pResultsChildMO->size ();
		
		if (NULL != pResultsChildMO) {
			if (0 == numberOfResultsChild) {
				//SSM_PLUG_ERR_LVL("Can't find the Rule Managed Object");

			} else if (1 == numberOfResultsChild) {
				trace(TRACE_LEVEL_INFO,"Rule already exists");
                
				if (policyType == SSMACL_TYPE_L2_EXT_E) {
					ExtMacAclRuleManagedObject *ExtruleMO = dynamic_cast<ExtMacAclRuleManagedObject *>((*pResultsChildMO)[0]);
					tempSeqid = ExtruleMO->getRuleid();
				} else if (policyType == SSMACL_TYPE_L2_STD_E) {
					StdMacAclRuleManagedObject *StdruleMO = dynamic_cast<StdMacAclRuleManagedObject *>((*pResultsChildMO)[0]);
					tempSeqid = StdruleMO->getRuleid();
				}
				
                if (UserEnteredSeqid && tempSeqid == seqid) {
					seqFound = true;
					trace(TRACE_LEVEL_INFO,"seqFound is true");
				} else {
					ruleFound = true;
					trace(TRACE_LEVEL_INFO,"ruleFound is true");
				}
			}
			else if (2 == numberOfResultsChild)
			{
				seqFound = true;
				trace(TRACE_LEVEL_INFO,"seqFound is true...");
			}
			else
			{
				SSM_PLUG_ERR_LVL("More Rules exists");
				status = WAVE_MESSAGE_ERROR;
			}
			
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsChildMO);
		}

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (opcode == SSMACL_DCMD_ADD_RULE) {
                /* Fix Me : Change Below Logic As NO_RULE_ID is Valid */
                if ((ruleFound) || (seqFound)) {
                    if (seqFound) {
                        status = WRC_SSM_DCM_ERR_ACL_RULE_SEQ_ALREADY_EXISTS;
					} else {
                        status = WRC_SSM_DCM_ERR_ACL_RULE_ALREADY_EXISTS;
					}
                    seqid = tempSeqid;
                }
            }
            else {// SSMACL_DCMD_DEL_RULE
                if ((ruleFound) || (seqFound))
                	seqid = tempSeqid;
                if (!((ruleFound) || (seqFound)))
                    status = WRC_SSM_DCM_ERR_ACL_RULE_EXISTS;
            }
        }
		return;
	}

	void SsmMacAclWorker::ssmMacAclRuleSendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
		PrismLinearSequencerContext *seqContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
		SsmGlobalAssocRuleToPolicyMessage *pMsg = NULL;

		prismAssert (NULL != seqContext, __FILE__, __LINE__);


		if (sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS) {
			pMsg = reinterpret_cast<SsmGlobalAssocRuleToPolicyMessage *>(seqContext->getPPrismMessage());
			if (pMsg->getOpcode() == SSMACL_DCMD_ADD_RULE) {
         	    WaveMessageStatus sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (FrameworkToolKit::getThisLocationId ());
            	if (WAVE_MESSAGE_SUCCESS == sendStatus) {
                	SsmLocalServiceAssocRuleToPolicyMessage *phase1Msg = reinterpret_cast<SsmLocalServiceAssocRuleToPolicyMessage *>(pWaveSendToClusterContext->getResultingMessageForPhase1(FrameworkToolKit::getThisLocationId ()));
	                prismAssert (NULL != phase1Msg, __FILE__, __LINE__);
    	            //SSM_PLUG_INFO_LVL_S(string("New Rule Id: ") + phase1Msg->getRuleid ());
        	        pMsg->setRuleid(phase1Msg->getRuleid ());
				}
			}	
		}
 
		sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
	    		
		//SSM_PLUG_INFO_LVL_S(string("status: ") + sendToClusterCompletionStatus);

		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
#if 0
		if (pWaveSendToClusterContext->getPPrismMessageForPhase2())
			delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
#endif
		delete pWaveSendToClusterContext;

		seqContext->executeNextStep(sendToClusterCompletionStatus);
        return;
	}



    void SsmMacAclWorker::udpateMacAclRuleToPolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalAssocRuleToPolicyMessage *pMsg				= reinterpret_cast<SsmGlobalAssocRuleToPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		UI64 ruleid											= pMsg->getRuleid();
		UI32 action											= pMsg->getAction();
		UI32 policyType										= pMsg->getPolicyType();
		const string policyName								= pMsg->getPolicyName();
		string srcMac;
		string srcMacMask									= "";
		AddrType srcType									= pMsg->getSrcType();
		string dstMac;
		string dstMacMask									= "";
		AddrType dstType									= pMsg->getDstType();
		bool isCount										= pMsg->getIsCount();
		bool isLog											= pMsg->getIsLog();
		string appType										= pMsg->getAppType(); 
		UI16 appTypeInt										= pMsg->getAppTypeInt();
		EtherType appTypeEnum								= pMsg->getAppTypeEnum();
		UI32 opcode											= (UI32) pMsg->getOpcode();
		ResourceId status									= WAVE_MESSAGE_ERROR;
		UI8	userEnteredSeqid								= pMsg->getUserEnteredSeqid();
		stringstream convert;
		WaveSendToClusterContext *pWaveSendToClusterContext = NULL;

		SSM_PLUG_INFO_LVL_S(string("Rule id: ") + ruleid + string ("Policy: ") + policyName + string ("policyType : ") + policyType);


		if (pMsg->getSrcType() == ADDR_TYPE_ANY) {
			srcMac 		= "any";
		} else {
			srcMac 		= pMsg->getSrcMac();
			srcMacMask 	= pMsg->getSrcMacMask ();
		}

		
		if (pMsg->getDstType() == ADDR_TYPE_ANY) {
			dstMac 		= "any";
		} else {
			dstMac 		= pMsg->getDstMac();
			dstMacMask 	= pMsg->getDstMacMask();
		}

        SsmLocalServiceAssocRuleToPolicyMessage *newMsg =
        	new SsmLocalServiceAssocRuleToPolicyMessage(policyName, policyType, ruleid, 
				action, srcMac, srcMacMask, srcType, dstMac, dstMacMask, dstType, appType, 
				appTypeEnum, appTypeInt, isCount, isLog, opcode, userEnteredSeqid);

		if (newMsg != NULL) {
        	SSM_PLUG_TRACE_SET_LVL(TRACE_LEVEL_DEVEL, "Message being sent to Local Service");
        	pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmMacAclWorker::ssmMacAclRuleSendToClusterCallback), seqContext);
        	pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
        	pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
        	pWaveSendToClusterContext->setPartialSuccessFlag(true);
        	sendToWaveCluster (pWaveSendToClusterContext);

		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message Creation Failed");
			seqContext->executeNextStep(status);
		}

	}

    void SsmMacAclWorker::assocMacAclRuleToPolicy(DceLinearSeqContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalAssocRuleToPolicyMessage *pMsg = reinterpret_cast<SsmGlobalAssocRuleToPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		UI64 ruleid 							= pMsg->getRuleid();
		UI32 action		 						= pMsg->getAction();;
		UI32 policyType 						= pMsg->getPolicyType();
		const string policyName 				= pMsg->getPolicyName();
		const string srcMac 					= pMsg->getSrcMac();
		const string srcMacMask 				= pMsg->getSrcMacMask();
		const AddrType srcType				   	= pMsg->getSrcType();
		const AddrType dstType				   	= pMsg->getDstType();
		const string dstMac 					= pMsg->getDstMac();
		const string dstMacMask 				= pMsg->getDstMacMask();
		bool isCount							= pMsg->getIsCount();
		bool isLog 								= pMsg->getIsLog();
		const UI16 appTypeInt					= pMsg->getAppTypeInt();
		EtherType appTypeEnum					= pMsg->getAppTypeEnum();
		ResourceId status 						= WAVE_MESSAGE_ERROR;
		MacAclRuleManagedObject *ruleMO 		= NULL;
		string className;
		string childClassName;	
		string compositionName;
		
		ObjectId policyObjId = seqContext->getWaveManagedObjectId();
	
		if (policyObjId == ObjectId::NullObjectId) {
			SSM_PLUG_DBG_LVL("Policy id is null");
			seqContext->executeNextStep(WAVE_MESSAGE_ERROR);
			return;
		}
		
		//SSM_PLUG_INFO_LVL_S(string("Rule id: ") + ruleid + string ("Policy: ") + policyName + string("policyType ") + policyType);
		
		startTransaction ();
		/* Update Policy MO with the corresponding Rule Composition  */
		if (policyType == SSMACL_TYPE_L2_STD_E) {
			className = StdMacAclPolicyManagedObject::getClassName();
			ruleMO = new StdMacAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			childClassName = StdMacAclRuleManagedObject::getClassName();
			compositionName = "rules";
		} else if (policyType == SSMACL_TYPE_L2_EXT_E) {
			className = ExtMacAclPolicyManagedObject::getClassName();
			ruleMO = new ExtMacAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			childClassName = ExtMacAclRuleManagedObject::getClassName();
			compositionName = "extRules";
		} else {
			className = MacAclPolicyManagedObject::getClassName();
			status = WAVE_MESSAGE_ERROR;
		}

		if (ruleMO == NULL) {
			status = WAVE_MESSAGE_ERROR;
		} else {
			ruleMO->setOwnerManagedObjectId(policyObjId);
			addToComposition(className, childClassName,	compositionName, policyObjId, ruleMO->getObjectId());
			//SSM_PLUG_SUCCESS_LVL("Successfully updated the Policy MO with the rule");
			status = WAVE_MESSAGE_SUCCESS;
		}

		if (status == WAVE_MESSAGE_SUCCESS) {
			ruleMO->setRuleid(ruleid);
			ruleMO->setAction(action);
			/* Bhavin */
			if (srcType == ADDR_TYPE_HOST) {
				ruleMO->setSrcHost(convertToLower(srcMac));	
				ruleMO->setSrcType(srcType);
			} else if (srcType == ADDR_TYPE_NONE) {
				ruleMO->setSrcMac(convertToLower(srcMac));
				ruleMO->setSrcMacMask(convertToLower(srcMacMask));
			} else {
				ruleMO->setSrcType(srcType);
			}

			ruleMO->setIsCount(isCount);
			ruleMO->setIsLog(isLog);
			
			if (policyType == SSMACL_TYPE_L2_EXT_E) {

				if (dstType == ADDR_TYPE_NONE) {
					dynamic_cast <ExtMacAclRuleManagedObject *>(ruleMO)->setDstMac(convertToLower(dstMac));
					dynamic_cast <ExtMacAclRuleManagedObject *>(ruleMO)->setDstMacMask(convertToLower(dstMacMask));
				} else if (dstType == ADDR_TYPE_HOST) {
					dynamic_cast <ExtMacAclRuleManagedObject *>(ruleMO)->setDstHost(convertToLower(dstMac));
					dynamic_cast <ExtMacAclRuleManagedObject *>(ruleMO)->setDstType(dstType);
				} else {
					dynamic_cast <ExtMacAclRuleManagedObject *>(ruleMO)->setDstType(dstType);
				}


				if (appTypeInt != 0) {
					dynamic_cast <ExtMacAclRuleManagedObject *>(ruleMO)->setAppTypeInt(appTypeInt);
				} else if (appTypeEnum == ETHER_TYPE_FCOE || appTypeEnum == ETHER_TYPE_ARP
						|| appTypeEnum == ETHER_TYPE_IPV4 || appTypeEnum == ETHER_TYPE_UNKNOWN) {
					dynamic_cast <ExtMacAclRuleManagedObject *>(ruleMO)->setAppType(appTypeEnum);
				}
			}
        	status = commitTransaction ();
        	if (status != FRAMEWORK_SUCCESS) {
        		SSM_PLUG_ERR_LVL_S(string("Commit transaction failed: ") + status);
       			status = WAVE_MESSAGE_ERROR;
        	} else {
        		//SSM_PLUG_SUCCESS_LVL("Commit Succeeded");
       			status = WAVE_MESSAGE_SUCCESS;
			}
		} else {
			SSM_PLUG_INFO_LVL("Rollback transaction");
			rollbackTransaction();
		}

		delete (ruleMO);

		seqContext->executeNextStep(status);

	}
	
    void SsmMacAclWorker::disAssocMacAclRuleToPolicy(DceLinearSeqContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalAssocRuleToPolicyMessage *pMsg = reinterpret_cast<SsmGlobalAssocRuleToPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);
		UI64 ruleid = pMsg->getRuleid();
		const string policyName = pMsg->getPolicyName();
        //MacAclPolicyManagedObject *policyMO = NULL; 
		string className;
		string childClassName;
		string compositionName;
		UI32 policyType = pMsg->getPolicyType();
		ResourceId status = WAVE_MESSAGE_ERROR;

		SSM_PLUG_INFO_LVL_S(string("Rule id: ") + ruleid + string ("Policy: ") + policyName + string ("policyType: ") +policyType);
        ObjectId policyObjId = seqContext->getWaveManagedObjectId();

        if (policyObjId == ObjectId::NullObjectId) {
            SSM_PLUG_DBG_LVL("Policy id is null");
            seqContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }       
		
		if (policyType == SSMACL_TYPE_L2_STD_E) {
            className = StdMacAclPolicyManagedObject::getClassName();
            childClassName = StdMacAclRuleManagedObject::getClassName();
            compositionName = "rules";
        } else if (policyType == SSMACL_TYPE_L2_EXT_E) {
            className = ExtMacAclPolicyManagedObject::getClassName();
            childClassName = ExtMacAclRuleManagedObject::getClassName();
            compositionName = "extRules";
        } else {
            // This case should not appear and it is error case
            className = MacAclPolicyManagedObject::getClassName();
            childClassName = MacAclRuleManagedObject::getClassName();
        }


        /* Query the MO for a given Rule*/

		WaveManagedObjectSynchronousQueryContext queryCtxtForChild(childClassName);
		queryCtxtForChild.addAndAttribute (new AttributeUI64 (ruleid, "ruleid"));
		queryCtxtForChild.setLoadOneToManyRelationships(false);
		queryCtxtForChild.addAndAttribute (new AttributeObjectId (policyObjId, "ownerManagedObjectId"));

		vector<WaveManagedObject *> *pResultsChildMO = querySynchronously (&queryCtxtForChild);
		if (NULL != pResultsChildMO) {
			UI32 numberOfResults = pResultsChildMO->size ();
			if (0 == numberOfResults) {
				SSM_PLUG_ERR_LVL("Can't find the Rule Managed Object");
				status = WAVE_MESSAGE_SUCCESS;
			} else if (1 == numberOfResults) {
				SSM_PLUG_ERR_LVL("New Deletion");
				MacAclRuleManagedObject *ruleMO = dynamic_cast<MacAclRuleManagedObject *>((*pResultsChildMO)[0]);
				deleteFromComposition(className, childClassName,
						compositionName, policyObjId, ruleMO->getObjectId());
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				SSM_PLUG_ERR_LVL("Two or more Rule objects exists");
				status = WAVE_MESSAGE_ERROR;
			}
			seqContext->addManagedObjectsForGarbageCollection(*pResultsChildMO);
			delete pResultsChildMO;
		}

		seqContext->executeNextStep(status);
    }


    void SsmMacAclWorker::deleteMacAclPolicy(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalRemMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalRemMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName = pMsg->getPolicyName();
		UI32 policyType = pMsg->getPolicyType();
		ResourceId  status   = WAVE_MESSAGE_ERROR;

        SsmLocalServiceRemMacAclPolicyMessage *newMsg = new SsmLocalServiceRemMacAclPolicyMessage (policyName, policyType);

        if (newMsg != NULL) {
        	SSM_PLUG_TRACE_SET_LVL(TRACE_LEVEL_DEVEL, "Message being sent to Local Service");
        	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmMacAclWorker::ssmMacAclPolicySendToClusterCallback), seqContext);
        	pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
        
            pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
			pWaveSendToClusterContext->setPartialSuccessFlag(true);
			sendToWaveCluster (pWaveSendToClusterContext);
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
			seqContext->executeNextStep(status);
		}

	}

	void SsmMacAclWorker::deleteAssociationwithDisconnectedPolicy (PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		trace (TRACE_LEVEL_DEBUG, "Inside deleteAssociationwithDisconnectedPolicy function");
		SsmGlobalRemMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalRemMacAclPolicyMessage *>(seqContext->getPPrismMessage());
	    SSM_PLUG_ASSERT_PTR(pMsg);
		ResourceId status = WAVE_MESSAGE_ERROR;
		SsmDeleteAssociationwithDisconnectedMessage *pSsmDeleteAssociationwithDisconnectedMessage = new SsmDeleteAssociationwithDisconnectedMessage ();
		if (pSsmDeleteAssociationwithDisconnectedMessage) {
			pSsmDeleteAssociationwithDisconnectedMessage->setPolicyId(pMsg->getPolicyId());
			pSsmDeleteAssociationwithDisconnectedMessage->setPolicyName(pMsg->getPolicyName());
			pSsmDeleteAssociationwithDisconnectedMessage->setAclType(pMsg->getPolicyType());
			status = sendSynchronously (pSsmDeleteAssociationwithDisconnectedMessage);
			if (WAVE_MESSAGE_SUCCESS == status) {
				status = pSsmDeleteAssociationwithDisconnectedMessage->getCompletionStatus();
				trace (TRACE_LEVEL_DEBUG, "Sent : Success.");
				trace(TRACE_LEVEL_DEBUG, string ("deleteAssociationwithDisconnectedPolicy: ") + FrameworkToolKit::localize (status));
			} else {
				trace (TRACE_LEVEL_DEBUG, "Failed to send.  Status : " + FrameworkToolKit::localize (status));
				status = WAVE_MESSAGE_ERROR;
			}
			delete pSsmDeleteAssociationwithDisconnectedMessage;
		}
        trace(TRACE_LEVEL_INFO, string ("deleteAssociationwithDisconnectedPolicy: ") + status);
		seqContext->executeNextStep(status);
	}

    void SsmMacAclWorker::deleteMacAclPolicyMO(DceLinearSeqContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		SsmGlobalRemMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalRemMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		trace(TRACE_LEVEL_DEBUG, "Entering SsmMacAclWorker::deleteMacAclPolicyMO step \n");
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName = pMsg->getPolicyName();
		ResourceId  status   = WAVE_MESSAGE_ERROR;

		// retrieving the policy id from the sequencer context
		ObjectId policyObjId = seqContext->getWaveManagedObjectId();
		if (policyObjId == ObjectId::NullObjectId) {
			SSM_PLUG_DBG_LVL("Policy id is null");
			seqContext->executeNextStep(status);
			return;
		} else {
			status = WAVE_MESSAGE_SUCCESS;
		}
	
		// using this API to make use of the optimized loading when deleting the MO
        deleteWaveManagedObject(policyObjId);

		SSM_PLUG_SUCCESS_LVL_S(string("Removed Policy: ") + policyName);

		seqContext->executeNextStep(status);
	}

    void SsmMacAclWorker::appmSecProfileUpdateStep(PrismLinearSequencerContext *seqContext)
    {

        SsmGlobalRemMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalRemMacAclPolicyMessage *>(seqContext->getPPrismMessage());
        SSM_PLUG_ASSERT_PTR(pMsg);

        string policyName = pMsg->getPolicyName();
        ResourceId  AppmStatus   = WAVE_MESSAGE_ERROR;
        APPMGlobalAPPMSecurityMessage *AppmSecProfileMsg = NULL;

        AppmSecProfileMsg = new APPMGlobalAPPMSecurityMessage();

        if (!AppmSecProfileMsg)
        {
            seqContext->executeNextStep(AppmStatus);
            return;
        }
        
        /*
         * Sending the message to APPM Plugin.
         */
        
        AppmSecProfileMsg->setOpCode(APPM_SSM_MSG);
        AppmSecProfileMsg->setCmdCode(MAC_ACCESS_GRP_DEL);
        AppmSecProfileMsg->setAccessGroup(policyName);
        AppmStatus = sendSynchronously(AppmSecProfileMsg);

        delete AppmSecProfileMsg;

        seqContext->executeNextStep(AppmStatus);
    }
	
	void SsmMacAclWorker::ssmMacAclPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
		PrismLinearSequencerContext *seqContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());

		prismAssert (NULL != seqContext, __FILE__, __LINE__);

		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
#if 0
		if (pWaveSendToClusterContext->getPPrismMessageForPhase2())
			delete (pWaveSendToClusterContext->getPPrismMessageForPhase2 ());
#endif
		delete pWaveSendToClusterContext;

		//SSM_PLUG_INFO_LVL_S(string("Completion Status: ") + sendToClusterCompletionStatus);

		
		seqContext->executeNextStep(sendToClusterCompletionStatus);
		
	}

	ResourceId SsmMacAclWorker::getMacAclPolicyType(const string &policyName, ObjectId &policyObjId, UI32 &policyType)
	{
		MacAclPolicyManagedObject *policyMO = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;

		SSM_PLUG_DBG_TRACE ();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(MacAclPolicyManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
		syncQueryCtxt.addSelectFieldForObjectId ("objectId");
		syncQueryCtxt.setLoadOneToManyRelationships(false);
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if ( 0 == numberOfResults) {
				//SSM_PLUG_INFO_LVL("Policy Object needs to be created");
				status = WAVE_MESSAGE_SUCCESS;
			}else if (1 == numberOfResults) {
				SSM_PLUG_INFO_LVL_S(string("Found policy with the name: ") + policyName);
				policyMO = dynamic_cast<MacAclPolicyManagedObject *>((*pResults)[0]);
				policyObjId = policyMO->getObjectId ();
				const string tableName = OrmRepository::getTableNameById (policyObjId.getClassId ());
				SSM_PLUG_INFO_LVL_S(string("Table Name: ") + tableName);
				if (tableName.compare("ExtMacAclPolicyManagedObject") == 0) {
					policyType = SSMACL_TYPE_L2_EXT_E;
				} else if (tableName.compare("StdMacAclPolicyManagedObject") == 0) {
					policyType = SSMACL_TYPE_L2_STD_E;
				} else {
					policyType = SSMACL_TYPE_UNKNOWN_E;
				}
				status = WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS;
			} else {
				SSM_PLUG_ERR_LVL("Two or more object Exists");
				status = WAVE_MESSAGE_ERROR;
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		} 

		return (status);
	}


	void SsmMacAclWorker::validatePolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalAddMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName 		= pMsg->getPolicyName();
		ResourceId  status  	= WAVE_MESSAGE_ERROR;
		UI32 oldPolicyType 		= SSMACL_TYPE_UNKNOWN_E; // Fix Me
		ObjectId policyObjId 	= ObjectId::NullObjectId; 
        UI32 count 				= 0;
		pMsg->setOldPolicyId(policyObjId);

        /* (a) First Check whether ACL Policy with same name exists for IP ACL
         * (b) Check whether ACL Policy with same name exists for IPv6 ACL
         * (c) Check for Mac Acl*/
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IpAclPolicyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        querySynchronouslyForCount(&syncQueryCtxt, count);

        if(count == 0) {
            WaveManagedObjectSynchronousQueryContext syncQueryCtxtIpv6(Ip6AclPolicyManagedObject::getClassName());
            syncQueryCtxtIpv6.addAndAttribute (new AttributeString (policyName, "policyName"));
            syncQueryCtxtIpv6.setLoadOneToManyRelationships(false);
            querySynchronouslyForCount(&syncQueryCtxtIpv6, count);

            if(count > 0) {
    			status = WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_IPV6;
    			seqContext->executeNextStep(status);
    			return;
            }
        } else {
            status = WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_IP;
            seqContext->executeNextStep(status);
            return;
        }

		status = getMacAclPolicyType(policyName, policyObjId, oldPolicyType);
		if (status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS) {
			if (oldPolicyType == SSMACL_TYPE_UNKNOWN_E && policyObjId != ObjectId::NullObjectId) {
					pMsg->setOldPolicyId(policyObjId);
					SSM_PLUG_INFO_LVL("Got valid objectId");
                status = WAVE_MESSAGE_SUCCESS;
            }
		}
	
		//SSM_PLUG_INFO_LVL_S(string ("status: ") + status);
		seqContext->executeNextStep(status);
		
	}

	void SsmMacAclWorker::validateDeletePolicy(DceLinearSeqContext *seqContext)
	{

        SsmGlobalRemMacAclPolicyMessage *pMsg    = reinterpret_cast<SsmGlobalRemMacAclPolicyMessage *>(seqContext->getPPrismMessage());
        UI32 IsmacPolicyAttachInterface          = 0;
		ResourceId status                        = WAVE_MESSAGE_SUCCESS;
	//ObjectId policyId                        = ObjectId::NullObjectId;

        IsmacPolicyAttachInterface 				 = ssmIsmacPolicyAttachInterface(pMsg->getPolicyName(), seqContext);
        pMsg->setPolicyId(seqContext->getWaveManagedObjectId());
        status 									 = QosUtils::isMacACLReferredByAnyClassMap(pMsg->getPolicyName());
        if (IsmacPolicyAttachInterface)
            status = WRC_SSM_DCM_ERR_MAC_ACL_APPLIED;

        trace(TRACE_LEVEL_INFO, string ("validateDeletePolicy: ") + status);
        seqContext->executeNextStep(status);
    }

    void SsmMacAclWorker::createMacAclPolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalAddMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName = pMsg->getPolicyName();
		UI32 policyType = pMsg->getPolicyType();
		ResourceId  status   = WAVE_MESSAGE_ERROR;
		
        SsmLocalServiceAddMacAclPolicyMessage *newMsg = new SsmLocalServiceAddMacAclPolicyMessage (policyName, policyType);
        if (newMsg != NULL) {
        	SSM_PLUG_DBG_LVL("Message being sent to Local service");
        	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmMacAclWorker::ssmMacAclPolicySendToClusterCallback), seqContext);
        	pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
			
			pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
            pWaveSendToClusterContext->setPartialSuccessFlag(true);
			sendToWaveCluster (pWaveSendToClusterContext);
        	trace (TRACE_LEVEL_DEVEL, "createMacAclPolicy: updated Local Service");
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
			seqContext->executeNextStep(status);
		}

	}

	void SsmMacAclWorker::deleteOldMacAclPolicyMO(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalAddMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		ResourceId  status = WAVE_MESSAGE_ERROR;
		WaveManagedObject   *pWaveManagedObject = NULL;


		if (pMsg->getOldPolicyId () == ObjectId::NullObjectId) {
			SSM_PLUG_DBG_LVL("Old policy id is null");
			seqContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return;
		}
		
		SSM_PLUG_DBG_LVL("Delete old policy");

		pWaveManagedObject = queryManagedObject (pMsg->getOldPolicyId ());
		if (NULL == pWaveManagedObject) {
			seqContext->executeNextStep(WAVE_MESSAGE_ERROR);
			return;
		}

		NsmGlobalReAssocMacAclPolicyMessage *nsmMsg = new NsmGlobalReAssocMacAclPolicyMessage ();

		if (nsmMsg) {
			nsmMsg->setOldPolicyId(pMsg->getOldPolicyId ());
			nsmMsg->setNewPolicyId(pMsg->getNewPolicyId ());
			status = sendSynchronously (nsmMsg);
			if (status == WAVE_MESSAGE_SUCCESS && nsmMsg->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}

			delete (nsmMsg);

			if (status == WAVE_MESSAGE_SUCCESS) {
				startTransaction ();
				delete(pWaveManagedObject);
				status = commitTransaction ();
        		if (status != FRAMEWORK_SUCCESS) {
        			SSM_PLUG_ERR_LVL("Deletion of old Policy MO failed");
       				status = WAVE_MESSAGE_ERROR;
        		} else {
        			SSM_PLUG_SUCCESS_LVL("Successfully deleted old policy");
       				status = WAVE_MESSAGE_SUCCESS;
				}
			}
		}
		SSM_PLUG_INFO_LVL_S(string("Status - ") + status);
       	seqContext->executeNextStep(status);
	}


	
    void SsmMacAclWorker::createMacAclPolicyMO(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddMacAclPolicyMessage *pMsg = reinterpret_cast<SsmGlobalAddMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName = pMsg->getPolicyName();
		UI32 policyType = pMsg->getPolicyType();
		ResourceId  status = WAVE_MESSAGE_ERROR;
		StdMacAclPolicyManagedObject *stdACLPolicyMO = NULL;
		ExtMacAclPolicyManagedObject *extACLPolicyMO = NULL;
		MacAclPolicyManagedObject *l2ACLPolicyMO = NULL; 
		pMsg->setNewPolicyId(ObjectId::NullObjectId);
		

		startTransaction ();

		if (policyType == SSMACL_TYPE_L2_STD_E)  {
			stdACLPolicyMO = new StdMacAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			l2ACLPolicyMO = dynamic_cast<MacAclPolicyManagedObject*>(stdACLPolicyMO);
		} else if (policyType == SSMACL_TYPE_L2_EXT_E){
			extACLPolicyMO = new ExtMacAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			l2ACLPolicyMO = dynamic_cast<MacAclPolicyManagedObject*>(extACLPolicyMO);
		} else {
       		rollbackTransaction();
       		seqContext->executeNextStep(status);
			return;
		}

        if (l2ACLPolicyMO == NULL) {
        	SSM_PLUG_ERR_LVL_S(string("Creating policy MO failed for policy: ") + policyName);
       		rollbackTransaction();
		} else {
        	l2ACLPolicyMO->setPolicyName(policyName);
        	status = commitTransaction ();
        	if (status != FRAMEWORK_SUCCESS) {
        		SSM_PLUG_ERR_LVL_S(string("commit transaction failed: ") + status);
       			status = WAVE_MESSAGE_ERROR;
        	} else {
        		//SSM_PLUG_SUCCESS_LVL("Successfully created policy");
				//trace(TRACE_LEVEL_SUCCESS, "CreatedteMacAclPolicyMO: Policy Mo created");
       			status = WAVE_MESSAGE_SUCCESS;
			}
			pMsg->setNewPolicyId(l2ACLPolicyMO->getObjectId());

			delete l2ACLPolicyMO;
		}

       	seqContext->executeNextStep(status);
		
	}
	
    void  SsmMacAclWorker::SsmGlobalReseqMacAclPolicyMessageHandler( SsmGlobalReseqMacAclPolicyMessage *pSsmGlobalReseqMacAclPolicyMessage)
    { 
    	SSM_PLUG_DBG_TRACE();

        PrismLinearSequencerStep sequencerSteps[] =
        {
        	//reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::validateResqPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::updateresequenceMacAclPolicy),
            
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::resequenceMacAclPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerCommitTransactionStep),
           
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmMacAclWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext ( pSsmGlobalReseqMacAclPolicyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismLinearSequencerContext->holdAll ();

        pPrismLinearSequencerContext->start ();
    }
	
	void SsmMacAclWorker::ssmMacAclReseqPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
		PrismLinearSequencerContext *seqContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
		prismAssert (NULL != seqContext, __FILE__, __LINE__);


		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
		delete pWaveSendToClusterContext;

		SSM_PLUG_INFO_LVL_S(string("Completion Status: ") + sendToClusterCompletionStatus);

		seqContext->executeNextStep(sendToClusterCompletionStatus);
		
   }



    void SsmMacAclWorker::updateresequenceMacAclPolicy(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalReseqMacAclPolicyMessage  *pMsg = reinterpret_cast<SsmGlobalReseqMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName = pMsg->getPolicyName();
		UI64 seqId = pMsg->getSeqId();
		UI64 incseqId = pMsg->getIncSeqId();
		ResourceId  status   = WAVE_MESSAGE_ERROR;

        SsmLocalServiceResqMacAclPolicyMessage *newMsg = new SsmLocalServiceResqMacAclPolicyMessage (policyName,seqId,incseqId);
        if (newMsg != NULL) {
        	SSM_PLUG_TRACE_SET_LVL(TRACE_LEVEL_DEVEL, "Message being sent to Local Service");
        	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmMacAclWorker::ssmMacAclReseqPolicySendToClusterCallback), seqContext);
        	pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
			
            pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
            pWaveSendToClusterContext->setPartialSuccessFlag(true);
			sendToWaveCluster (pWaveSendToClusterContext);
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
			seqContext->executeNextStep(status);
		}

	}
    
	void SsmMacAclWorker::resequenceMacAclPolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalReseqMacAclPolicyMessage  *pMsg = reinterpret_cast<SsmGlobalReseqMacAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName = pMsg->getPolicyName();
		UI64 seqId = pMsg->getSeqId();
		UI64 incseqId = pMsg->getIncSeqId();
	    UI32 numberOfResults,numberOfResults1;	
        ResourceId status = WAVE_MESSAGE_ERROR;
		string className;
        int i;

		vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > extRuleMoVector;
		vector<WaveManagedObjectPointer<StdMacAclRuleManagedObject> > stdRuleMoVector;

		ExtMacAclPolicyManagedObject *extPolicyMO = NULL;
		StdMacAclPolicyManagedObject *stdPolicyMO = NULL;
		
		SSM_PLUG_INFO_LVL_S(string("Rule id: ") + seqId + string ("Policy: ") + policyName);
        
       	/* Query the MO for a given policy*/

		className = StdMacAclPolicyManagedObject::getClassName();
		WaveManagedObjectSynchronousQueryContext stdsyncQueryCtxt(className);
	    stdsyncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&stdsyncQueryCtxt);

		if (NULL != pResults) {
		  //  startTransaction ();
			numberOfResults = pResults->size ();
			if (1 == numberOfResults) {
				/* Update Policy MO with the corresponding Rule Composition  */
				stdPolicyMO = dynamic_cast<StdMacAclPolicyManagedObject *>((*pResults)[0]);
				stdPolicyMO->sortRules();
				stdRuleMoVector = stdPolicyMO->getRules();
				SSM_PLUG_INFO_LVL_S(string("vectorsize")+stdRuleMoVector.size());
			    for (i = 0; i < (int) stdRuleMoVector.size(); i++) {
               	     stdRuleMoVector[i]->setRuleid(seqId +(incseqId * i)) ;
                     StdMacAclRuleManagedObject  *stdRuleMO = stdRuleMoVector[i].operator-> (); 
                     updateWaveManagedObject(stdRuleMO);
				SSM_PLUG_INFO_LVL_S(string("vectorsize ")+stdRuleMoVector.size()+string(" updated the rule"));
			    }
				updateWaveManagedObject(stdPolicyMO);
				SSM_PLUG_SUCCESS_LVL("Successfully updated the Policy MO with the Sequence Id");
				status = WAVE_MESSAGE_SUCCESS;
			} else if (0 == numberOfResults) {
				className = ExtMacAclPolicyManagedObject::getClassName();
				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
				syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
				vector<WaveManagedObject *> *pResults1  = querySynchronously (&syncQueryCtxt);
		 	    //startTransaction ();
				if (NULL != pResults1) {
                     numberOfResults1 = pResults1->size ();
					if (0 == numberOfResults1) {
						SSM_PLUG_ERR_LVL("Can't find the policy Managed Object");
						status = WAVE_MESSAGE_ERROR;
					}else if (1 == numberOfResults1) {
						extPolicyMO = dynamic_cast<ExtMacAclPolicyManagedObject *>((*pResults1)[0]);
						extPolicyMO->sortRules();
						extRuleMoVector = extPolicyMO->getRules();
			        	for (i = 0; i < (int) extRuleMoVector.size(); i++) {
                        	extRuleMoVector[i]->setRuleid(seqId +(incseqId * i)) ;
                     		ExtMacAclRuleManagedObject *extRuleMO =extRuleMoVector[i].operator-> ();
                            updateWaveManagedObject(extRuleMO);
     						SSM_PLUG_INFO_LVL_S(string("Rule Number: ")+i+string(" updated"));
			       		}
						updateWaveManagedObject(extPolicyMO);
						SSM_PLUG_SUCCESS_LVL("Successfully updated the Policy MO with the Sequence Id");
						status = WAVE_MESSAGE_SUCCESS;
        			} else {
						status =WAVE_MESSAGE_ERROR;
				    	SSM_PLUG_ERR_LVL("Two or more object exists");
					}

            		seqContext->addManagedObjectsForGarbageCollection(*pResults1);
            		delete pResults1; 
            	} else {
					status = WRC_SSM_DCM_ERR_ACL_EEXIST;
					SSM_PLUG_ERR_LVL("No Policy found resequence  failed");
            	}
        	}
            else {
				SSM_PLUG_ERR_LVL("Two or more object exists");
				status =WRC_SSM_DCM_ERR_ACL_EEXIST;
			}
            seqContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults; 
		} else {
					status = WAVE_MESSAGE_ERROR;
					SSM_PLUG_ERR_LVL("No Policy found resequence  failed");
        }

		seqContext->executeNextStep(status);

	}


}
