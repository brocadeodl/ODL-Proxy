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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmIpAclWorker.h"
#include "Ssm/Global/SsmGlobalAddIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalRemIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocIpRuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalReseqIpAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalGetIpAclPolicyMessage.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Ssm/Global/StdIpAclPolicyManagedObject.h"
#include "Ssm/Global/ExtIpAclPolicyManagedObject.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "Ssm/Global/StdIpAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Ssm/Local/SsmTypes.h"
#include "Ssm/Local/SsmLocalServiceAddIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceRemIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceAssocIpRuleToPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceResqIpAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "DcmCStatus.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Global/NsmGlobalReAssocIpAclPolicyMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"
#include "Nsm/Global/Ssm/SsmDeleteAssociationwithDisconnectedMessage.h"
#include <string>
#include <sstream>
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Ssm/Global/SsmPlugUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"

/* for AAA plugin invocation */
#include "AAA/Global/GlobalIpAclWorker.h"
#include "AAA/Global/AAASequencerContexts.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "AAA/Local/LocalIpAclWorker.h"
#include "AAA/Local/IpAclLocalManagedObjects.h"
#include "Qos/Common/QosUtils.h"

namespace DcmNs
{

    SsmIpAclWorker::SsmIpAclWorker ( SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : WaveWorker  (pSsmGlobalObjectManager)
    {
        IpAclPolicyManagedObject    IpAclPolicyManagedObject    (pSsmGlobalObjectManager);
        StdIpAclPolicyManagedObject   StdIpAclPolicyManagedObject    (pSsmGlobalObjectManager);
        ExtIpAclPolicyManagedObject   ExtIpAclPolicyManagedObject    (pSsmGlobalObjectManager);
        IpAclRuleManagedObject    IpAclRuleManagedObject    (pSsmGlobalObjectManager);
        ExtIpAclRuleManagedObject    ExtIpAclRuleManagedObject    (pSsmGlobalObjectManager);
        StdIpAclRuleManagedObject    StdIpAclRuleManagedObject    (pSsmGlobalObjectManager);
        IpAclPolicyManagedObject.setupOrm ();
        addManagedClass (IpAclPolicyManagedObject::getClassName (), this);
        StdIpAclPolicyManagedObject.setupOrm ();
        addManagedClass (StdIpAclPolicyManagedObject::getClassName (), this);
        ExtIpAclPolicyManagedObject.setupOrm ();
        addManagedClass (ExtIpAclPolicyManagedObject::getClassName (), this);
        IpAclRuleManagedObject.setupOrm ();
        addManagedClass (IpAclRuleManagedObject::getClassName (), this);
        StdIpAclRuleManagedObject.setupOrm ();
        addManagedClass (StdIpAclRuleManagedObject::getClassName (), this);
       	ExtIpAclRuleManagedObject.setupOrm ();
        addManagedClass (ExtIpAclRuleManagedObject::getClassName (), this);
        addOperationMap (SSMGLOBALADDIPACLPOLICY, 
			reinterpret_cast<PrismMessageHandler> (&SsmIpAclWorker::SsmGlobalAddIpAclPolicyMessageHandler));
        addOperationMap (SSMGLOBALREMIPACLPOLICY, 
			reinterpret_cast<PrismMessageHandler> (&SsmIpAclWorker::SsmGlobalRemIpAclPolicyMessageHandler));
        addOperationMap (SSMGLOBALASSOCIPRULETOPOLICY, 
			reinterpret_cast<PrismMessageHandler> (&SsmIpAclWorker::SsmGlobalAssocIpRuleToPolicyMessageHandler));
    	addOperationMap (SSMGLOBALRESEQIPACLPOLICY, 
			reinterpret_cast<PrismMessageHandler> (&SsmIpAclWorker::SsmGlobalReseqIpAclPolicyMessageHandler));
		addOperationMap (SSMGLOBALGETIPACLPOLICY, 
			reinterpret_cast<PrismMessageHandler> (&SsmIpAclWorker::SsmGlobalGetIpAclPolicyMessageHandler));
		
	}

    SsmIpAclWorker::~SsmIpAclWorker ()
    {
    }

    PrismMessage  *SsmIpAclWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SSMGLOBALADDIPACLPOLICY :
                pPrismMessage = new SsmGlobalAddIpAclPolicyMessage ();
                break;
			case SSMGLOBALREMIPACLPOLICY :
                pPrismMessage = new SsmGlobalRemIpAclPolicyMessage ();
                break;
            case SSMGLOBALASSOCIPRULETOPOLICY :
                pPrismMessage = new SsmGlobalAssocIpRuleToPolicyMessage ();
                break;
			case SSMGLOBALRESEQIPACLPOLICY :	
				pPrismMessage = new SsmGlobalReseqIpAclPolicyMessage ();
				break;
			case SSMGLOBALGETIPACLPOLICY :
				pPrismMessage = new SsmGlobalGetIpAclPolicyMessage ();
				break;
			default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SsmIpAclWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((IpAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((StdIpAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new StdIpAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
		
        else if ((ExtIpAclPolicyManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ExtIpAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((IpAclRuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((StdIpAclRuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new StdIpAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
		}
		
        else if ((ExtIpAclRuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ExtIpAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
		
        else
        {
            trace (TRACE_LEVEL_FATAL, "SsmIpAclWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  SsmIpAclWorker::SsmGlobalAddIpAclPolicyMessageHandler( SsmGlobalAddIpAclPolicyMessage *pSsmGlobalAddIpAclPolicyMessage)
    {

    	SSM_PLUG_DBG_TRACE();

        PrismLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::validatePolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::createIpAclPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::createIpAclPolicyMO),
            /* Fix Me : Below Step Not Needed as of now will be needed for unkonwn policytype */
			reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::deleteOldIpAclPolicyMO),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = 
			new PrismLinearSequencerContext (pSsmGlobalAddIpAclPolicyMessage, this, 
				sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }


    void  SsmIpAclWorker::SsmGlobalAssocIpRuleToPolicyMessageHandler( SsmGlobalAssocIpRuleToPolicyMessage *pSsmGlobalAssocIpRuleToPolicyMessage)
    { 
    	SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pSsmGlobalAssocIpRuleToPolicyMessage);

    	UI32 opcode = (UI32)pSsmGlobalAssocIpRuleToPolicyMessage->getOpcode();
    	DceLinearSeqContext *pDceLinearSeqContext = NULL;

        PrismLinearSequencerStep AddsequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::validateRule),
           	reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::udpateIpAclRuleToPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::assocIpAclRuleToPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerFailedStep)
        };
        
        PrismLinearSequencerStep RemsequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::validateRule),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::udpateIpAclRuleToPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::disAssocIpAclRuleToPolicy),

            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerFailedStep)
        };

        if (opcode == SSMACL_DCMD_ADD_RULE) {
        	pDceLinearSeqContext = new DceLinearSeqContext (pSsmGlobalAssocIpRuleToPolicyMessage, this, 
				AddsequencerSteps, sizeof (AddsequencerSteps) /sizeof (AddsequencerSteps[0]));
		} else if (opcode == SSMACL_DCMD_DEL_RULE) {
        	pDceLinearSeqContext = new DceLinearSeqContext (pSsmGlobalAssocIpRuleToPolicyMessage, this, 
				RemsequencerSteps, sizeof (RemsequencerSteps) /sizeof (RemsequencerSteps[0]));
		}

		if (pDceLinearSeqContext) {
			pDceLinearSeqContext->holdAll();
        	pDceLinearSeqContext->start ();
    }
    }

    void  SsmIpAclWorker::SsmGlobalRemIpAclPolicyMessageHandler( SsmGlobalRemIpAclPolicyMessage *pSsmGlobalRemIpAclPolicyMessage)
    {
    	SSM_PLUG_DBG_TRACE();

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::validateDeletePolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::deleteIpAclPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::deleteAssociationwithDisconnectedPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::deleteIpAclPolicyMO),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pDCELinearSeqContext = new
        		DceLinearSeqContext(pSsmGlobalRemIpAclPolicyMessage, this,
				sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDCELinearSeqContext->holdAll();
        pDCELinearSeqContext->start ();
    }

    void SsmIpAclWorker::validateRule(DceLinearSeqContext *seqContext)
	{
		SsmGlobalAssocIpRuleToPolicyMessage *pMsg = reinterpret_cast<SsmGlobalAssocIpRuleToPolicyMessage *>(seqContext->getPPrismMessage());

		SSM_PLUG_ASSERT_PTR(pMsg);
		UI32 policyType								= pMsg->getPolicyType();
		const string policyName						= pMsg->getPolicyName();
		IpV4Address srcHostIpAddress				= pMsg->getSrcHostIpAddress();
		IpV4Address srcIpAddress					= pMsg->getSrcIpAddress();
		IpV4Address srcIpAddressMask				= pMsg->getSrcIpAddressMask();
		IpAddrType srcType 							= pMsg->getSrcType();
		IpAclPolicyManagedObject *policyMO         	= NULL;
		UI64 seqid;
		string className							= "";
		string childClassName						= "";
		ResourceId status 							= WAVE_MESSAGE_SUCCESS;
		bool seqFound 								= false;
		bool ruleFound 								= false;
        UI32 opcode                                 = (UI32) pMsg->getOpcode();
		UI32 UserEnteredSeqid                       = pMsg->getUserEnteredSeqid();
		bool rulePropertiesExist                    = false;
	
    	SSM_PLUG_DBG_TRACE();
		
		prismAssert(opcode == SSMACL_DCMD_ADD_RULE ||
				opcode == SSMACL_DCMD_DEL_RULE, __FILE__, __LINE__);
		if (policyType == SSMACL_TYPE_L3_IPV4_STD_E) {
			className = StdIpAclPolicyManagedObject::getClassName();
			childClassName = StdIpAclRuleManagedObject::getClassName();
		} else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E) {
			className = ExtIpAclPolicyManagedObject::getClassName();
			childClassName = ExtIpAclRuleManagedObject::getClassName();
		} else {
			// This case should not appear and it is error case
			className = IpAclPolicyManagedObject::getClassName();
		}

        // Get the policy object id from message
        ObjectId policyId = pMsg->getPolicyObjectId ();


        if (ObjectId::NullObjectId == policyId)
        {
    		/* Query the MO for a given policy*/
    		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
    		// set the objectId as addSelectedField in the query
    		syncQueryCtxt.addSelectFieldForObjectId ("objectId");
    		syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
    		syncQueryCtxt.setLoadOneToManyRelationships(false);
    		vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

    		if (NULL != pResultsPMO)
    		{
    			UI32 numberOfResults = pResultsPMO->size ();
    			if (0 == numberOfResults)
    			{
    				SSM_PLUG_ERR_LVL("Can't find the policy Managed Object");
    				status = WRC_SSM_DCM_ERR_ACL_EEXIST; //Acl does not exist
    			}
    			else if (1 == numberOfResults)
    			{
    				SSM_PLUG_DBG_LVL("Found Policy Managed Object");
    				policyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResultsPMO)[0]);
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

        	WaveManagedObjectSynchronousQueryContext queryCtxtForChild(childClassName);

        	queryCtxtForChild.createConditionSet ("RULEVALIDATE_POLICYID");
        	queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_POLICYID", new AttributeObjectId (policyId, "ownerManagedObjectId"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        	if (UserEnteredSeqid)
        	{
        		queryCtxtForChild.createConditionSet ("RULEVALIDATE_SEQID");
        		seqid = pMsg->getRuleid();
        		queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_SEQID", new AttributeUI64 (seqid, "ruleid"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        	}

        	if (((opcode == SSMACL_DCMD_DEL_RULE) && (!UserEnteredSeqid)) || (opcode == SSMACL_DCMD_ADD_RULE))
        	{
        		rulePropertiesExist = true;
        		queryCtxtForChild.createConditionSet ("RULEVALIDATE_PROPERTIES");

        		queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum(pMsg->getAction(), "action"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        		if (srcType == IP_ADDR_TYPE_ANY) {
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (srcType, "srcType"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        		} else if (srcType == IP_ADDR_TYPE_HOST) {
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (srcType, "srcType"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeIpV4Address(srcHostIpAddress, "srcHostIpAddress"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        		} else if (srcType == IP_ADDR_TYPE_NONE) {
        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum(srcType, "srcType"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeIpV4Address(srcIpAddress, "srcIpAddress"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeIpV4Address(srcIpAddressMask, "srcIpAddressMask"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        		}

        		if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E)
        		{
        			IpAddrType  dstType                                     = pMsg->getDstType();
        			IpV4Address dstIpAddress                                = pMsg->getDstIpAddress();
        			IpV4Address dstIpAddressMask                            = pMsg->getDstIpAddressMask();
        			IpV4Address dstHostIpAddress                            = pMsg->getDstHostIpAddress();
        			WaveUnion dstUnion                                      = pMsg->getDstUnion();
        			WaveUnion ProtocolTypeUnion                             = pMsg->getProtocolTypeUnion();
        			UI32 l3l4CustomProtoType                                = pMsg->getL3l4CustomProtoType();
        			L3L4wellknownProtoType l3l4WellknownProtoType           = pMsg->getL3l4WellknownProtoType();
        			PortOperator srcPortOperator                            = pMsg->getSrcPortOperator();
        			PortOperator dstPortOperator                            = pMsg->getDstPortOperator();
        			WaveUnion srcPortNumberTcpUnion                         = pMsg->getSrcPortNumberTcpUnion();
        			WaveUnion srcPortltNumberTcpUnion                       = pMsg->getSrcPortltNumberTcpUnion();
        			WaveUnion srcPortgtNumberTcpUnion                       = pMsg->getSrcPortgtNumberTcpUnion();
        			WaveUnion dstPortNumberTcpUnion                         = pMsg->getDstPortNumberTcpUnion();
        			WaveUnion dstPortltNumberTcpUnion                       = pMsg->getDstPortltNumberTcpUnion();
        			WaveUnion dstPortgtNumberTcpUnion                       = pMsg->getDstPortgtNumberTcpUnion();
        			WaveUnion srcPortNumberUdpUnion                         = pMsg->getSrcPortNumberUdpUnion();
        			WaveUnion srcPortltNumberUdpUnion                       = pMsg->getSrcPortltNumberUdpUnion();
        			WaveUnion srcPortgtNumberUdpUnion                       = pMsg->getSrcPortgtNumberUdpUnion();
        			WaveUnion dstPortNumberUdpUnion                         = pMsg->getDstPortNumberUdpUnion();
        			WaveUnion dstPortltNumberUdpUnion                       = pMsg->getDstPortltNumberUdpUnion();
        			WaveUnion dstPortgtNumberUdpUnion                       = pMsg->getDstPortgtNumberUdpUnion();
        			WaveUnion srcPortrangeLowerTcpUnion                     = pMsg->getSrcPortrangeLowerTcpUnion();
        			WaveUnion dstPortrangeLowerTcpUnion                     = pMsg->getDstPortrangeLowerTcpUnion();
        			WaveUnion srcPortrangeHigherTcpUnion                    = pMsg->getSrcPortrangeHigherTcpUnion();
        			WaveUnion dstPortrangeHigherTcpUnion                    = pMsg->getDstPortrangeHigherTcpUnion();
        			WaveUnion srcPortrangeLowerUdpUnion                     = pMsg->getSrcPortrangeLowerUdpUnion();
        			WaveUnion dstPortrangeLowerUdpUnion                     = pMsg->getDstPortrangeLowerUdpUnion();
        			WaveUnion srcPortrangeHigherUdpUnion                    = pMsg->getSrcPortrangeHigherUdpUnion();
        			WaveUnion dstPortrangeHigherUdpUnion                    = pMsg->getDstPortrangeHigherUdpUnion();
        			UI32UC srcPortCustomNumberTcp                           = pMsg->getSrcPortCustomNumberTcp();
        			UI32UC srcPortltCustomNumberTcp                         = pMsg->getSrcPortltCustomNumberTcp();
        			UI32UC srcPortgtCustomNumberTcp                         = pMsg->getSrcPortgtCustomNumberTcp();
        			UI32UC dstPortCustomNumberTcp                           = pMsg->getDstPortCustomNumberTcp();
        			UI32UC dstPortltCustomNumberTcp                         = pMsg->getDstPortltCustomNumberTcp();
        			UI32UC dstPortgtCustomNumberTcp                         = pMsg->getDstPortgtCustomNumberTcp();
        			UI32UC srcPortCustomNumberUdp                           = pMsg->getSrcPortCustomNumberUdp();
        			UI32UC srcPortltCustomNumberUdp                         = pMsg->getSrcPortltCustomNumberUdp();
        			UI32UC srcPortgtCustomNumberUdp                         = pMsg->getSrcPortgtCustomNumberUdp();
        			UI32UC dstPortCustomNumberUdp                           = pMsg->getDstPortCustomNumberUdp();
        			UI32UC dstPortltCustomNumberUdp                         = pMsg->getDstPortltCustomNumberUdp();
        			UI32UC dstPortgtCustomNumberUdp                         = pMsg->getDstPortgtCustomNumberUdp();
        			EnumUC srcPortWellknownNumberTcp						= pMsg->getSrcPortWellknownNumberTcp();
        			EnumUC srcPortltWellknownNumberTcp						= pMsg->getSrcPortltWellknownNumberTcp();
        			EnumUC srcPortgtWellknownNumberTcp						= pMsg->getSrcPortgtWellknownNumberTcp();
        			EnumUC dstPortWellknownNumberTcp						= pMsg->getDstPortWellknownNumberTcp();
        			EnumUC dstPortltWellknownNumberTcp						= pMsg->getDstPortltWellknownNumberTcp();
        			EnumUC dstPortgtWellknownNumberTcp						= pMsg->getDstPortgtWellknownNumberTcp();
        			EnumUC srcPortWellknownNumberUdp						= pMsg->getSrcPortWellknownNumberUdp();
        			EnumUC srcPortltWellknownNumberUdp						= pMsg->getSrcPortltWellknownNumberUdp();
        			EnumUC srcPortgtWellknownNumberUdp						= pMsg->getSrcPortgtWellknownNumberUdp();
        			EnumUC dstPortWellknownNumberUdp						= pMsg->getDstPortWellknownNumberUdp();
        			EnumUC dstPortltWellknownNumberUdp						= pMsg->getDstPortltWellknownNumberUdp();
        			EnumUC dstPortgtWellknownNumberUdp						= pMsg->getDstPortgtWellknownNumberUdp();
        			UI32UC srcPortCustomrangeLowerTcp                       = pMsg->getSrcPortCustomrangeLowerTcp();
        			UI32UC dstPortCustomrangeLowerTcp                       = pMsg->getDstPortCustomrangeLowerTcp();
        			UI32UC srcPortCustomrangeLowerUdp                       = pMsg->getSrcPortCustomrangeLowerUdp();
        			UI32UC dstPortCustomrangeLowerUdp                       = pMsg->getDstPortCustomrangeLowerUdp();
        			UI32UC srcPortCustomrangeHigherTcp                      = pMsg->getSrcPortCustomrangeHigherTcp();
        			UI32UC dstPortCustomrangeHigherTcp                      = pMsg->getDstPortCustomrangeHigherTcp();
        			UI32UC srcPortCustomrangeHigherUdp                      = pMsg->getSrcPortCustomrangeHigherUdp();
        			UI32UC dstPortCustomrangeHigherUdp                      = pMsg->getDstPortCustomrangeHigherUdp();
        			EnumUC srcPortWellknownrangeLowerUdp					= pMsg->getSrcPortWellknownrangeLowerUdp();
        			EnumUC dstPortWellknownrangeLowerUdp					= pMsg->getDstPortWellknownrangeLowerUdp();
        			EnumUC srcPortWellknownrangeLowerTcp					= pMsg->getSrcPortWellknownrangeLowerTcp();
        			EnumUC dstPortWellknownrangeLowerTcp					= pMsg->getDstPortWellknownrangeLowerTcp();
        			EnumUC srcPortWellknownrangeHigherUdp					= pMsg->getSrcPortWellknownrangeHigherUdp();
        			EnumUC dstPortWellknownrangeHigherUdp					= pMsg->getDstPortWellknownrangeHigherUdp();
        			EnumUC srcPortWellknownrangeHigherTcp					= pMsg->getSrcPortWellknownrangeHigherTcp();
        			EnumUC dstPortWellknownrangeHigherTcp					= pMsg->getDstPortWellknownrangeHigherTcp();
        			EnumUC wellknownPrecedence								= pMsg->getWellknownPrecedence();
        			EnumUC wellknownTos                               		= pMsg->getWellknownTos();
        			EnumUC wellknownDscp                             		= pMsg->getWellknownDscp();
        			UI32UC customPrecedence									= pMsg->getCustomPrecedence();
        			UI32UC customTos                                       	= pMsg->getCustomTos();
        			UI32UC customDscp                                      	= pMsg->getCustomDscp();
        			WaveUnion precedenceUnion                               = pMsg->getPrecedenceUnion();
        			WaveUnion dscpUnion                                     = pMsg->getDscpUnion();
        			WaveUnion tosUnion                                      = pMsg->getTosUnion();
        			bool urg                                                = pMsg->getUrg();
        			bool ack                                                = pMsg->getAck();
        			bool push                                               = pMsg->getPush();
        			bool fin                                                = pMsg->getFin();
        			bool sync                                               = pMsg->getSync();
        			bool rst                                                = pMsg->getRst();

        			if (dstType == IP_ADDR_TYPE_ANY)
        			{
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (dstType, "dstType"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}
        			else if (dstType == IP_ADDR_TYPE_HOST)
        			{
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (dstType, "dstType"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeIpV4Address(dstHostIpAddress, "dstHostIpAddress"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}
        			else if (dstType == IP_ADDR_TYPE_NONE)
        			{
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum (dstType, "dstType"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeIpV4Address(dstIpAddress, "dstIpAddress"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeIpV4Address(dstIpAddressMask, "dstIpAddressMask"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUnion(ProtocolTypeUnion, "ProtocolTypeUnion"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			if (ProtocolTypeUnion.compare("l3l4WellknownProtoType") == 0) {
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum(l3l4WellknownProtoType, "l3l4WellknownProtoType"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			} else if (ProtocolTypeUnion.compare("l3l4CustomProtoType") == 0) {
        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32(l3l4CustomProtoType, "l3l4CustomProtoType"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum(srcPortOperator, "srcPortOperator"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			if (srcPortOperator) {
        				/* TCP */
        				if (l3l4WellknownProtoType == L3L4_PROTOTYPE_TCP) {
        					if (srcPortOperator == PORTOPERATOR_LT) {
        						if ((srcPortltNumberTcpUnion.compare ("srcPortltCustomNumberTcp") == 0) && (srcPortltCustomNumberTcp != DEFAULT_PORTNUM)) {
        							srcPortltCustomNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortltCustomNumberTcp, true, "srcPortltCustomNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortltNumberTcpUnion.compare ("srcPortltWellknownNumberTcp") == 0)&& (srcPortltWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							srcPortltWellknownNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortltWellknownNumberTcp, true, "srcPortltWellknownNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (srcPortOperator == PORTOPERATOR_GT) {
        						if ((srcPortgtNumberTcpUnion.compare ("srcPortgtCustomNumberTcp") == 0) && (srcPortgtCustomNumberTcp != DEFAULT_PORTNUM)) {
        							srcPortgtCustomNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortgtCustomNumberTcp, true, "srcPortgtCustomNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortgtNumberTcpUnion.compare ("srcPortgtWellknownNumberTcp") == 0) && (srcPortgtWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							srcPortgtWellknownNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortgtWellknownNumberTcp, true, "srcPortgtWellknownNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}  else if ((srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ)) {
        						if ((srcPortNumberTcpUnion.compare ("srcPortWellknownNumberTcp") == 0) && (srcPortWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							srcPortWellknownNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortWellknownNumberTcp, true, "srcPortWellknownNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortNumberTcpUnion.compare ("srcPortCustomNumberTcp") == 0) && (srcPortCustomNumberTcp != DEFAULT_PORTNUM)) {
        							srcPortCustomNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortCustomNumberTcp, true, "srcPortCustomNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (srcPortOperator == PORTOPERATOR_RANGE) {
        						if ((srcPortrangeLowerTcpUnion.compare("srcPortWellknownrangeLowerTcp") == 0) && (srcPortWellknownrangeLowerTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							srcPortWellknownrangeLowerTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortWellknownrangeLowerTcp, true, "srcPortWellknownrangeLowerTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortrangeLowerTcpUnion.compare("srcPortCustomrangeLowerTcp") == 0) && (srcPortCustomrangeLowerTcp != DEFAULT_PORTNUM)){
        							srcPortCustomrangeLowerTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortCustomrangeLowerTcp, true, "srcPortCustomrangeLowerTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        						if ((srcPortrangeHigherTcpUnion.compare("srcPortWellknownrangeHigherTcp") == 0) && (srcPortWellknownrangeHigherTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							srcPortWellknownrangeHigherTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortWellknownrangeHigherTcp, true, "srcPortWellknownrangeHigherTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortrangeHigherTcpUnion.compare("srcPortCustomrangeHigherTcp") == 0) && (srcPortCustomrangeHigherTcp != DEFAULT_PORTNUM)){
        							srcPortCustomrangeHigherTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortCustomrangeHigherTcp, true, "srcPortCustomrangeHigherTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}
        				} else if (l3l4WellknownProtoType == L3L4_PROTOTYPE_UDP) {
        					if (srcPortOperator == PORTOPERATOR_LT) {
        						if ((srcPortltNumberUdpUnion.compare ("srcPortltCustomNumberUdp") == 0) && (srcPortltCustomNumberUdp != DEFAULT_PORTNUM)) {
        							srcPortltCustomNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortltCustomNumberUdp, true, "srcPortltCustomNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortltNumberUdpUnion.compare ("srcPortltWellknownNumberUdp") == 0)&& (srcPortltWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							srcPortltWellknownNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortltWellknownNumberUdp, true, "srcPortltWellknownNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (srcPortOperator == PORTOPERATOR_GT) {
        						if ((srcPortgtNumberUdpUnion.compare ("srcPortgtCustomNumberUdp") == 0) && (srcPortgtCustomNumberUdp != DEFAULT_PORTNUM)) {
        							srcPortgtCustomNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortgtCustomNumberUdp, true, "srcPortgtCustomNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortgtNumberUdpUnion.compare ("srcPortgtWellknownNumberUdp") == 0) && (srcPortgtWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							srcPortgtWellknownNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortgtWellknownNumberUdp, true, "srcPortgtWellknownNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}  else if ((srcPortOperator == PORTOPERATOR_EQ) || (srcPortOperator == PORTOPERATOR_NEQ)) {
        						if ((srcPortNumberUdpUnion.compare ("srcPortWellknownNumberUdp") == 0) && (srcPortWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							srcPortWellknownNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortWellknownNumberUdp, true, "srcPortWellknownNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortNumberUdpUnion.compare ("srcPortCustomNumberUdp") == 0) && (srcPortCustomNumberUdp != DEFAULT_PORTNUM)) {
        							srcPortCustomNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortCustomNumberUdp, true ,"srcPortCustomNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (srcPortOperator == PORTOPERATOR_RANGE) {
        						if ((srcPortrangeLowerUdpUnion.compare("srcPortWellknownrangeLowerUdp") == 0) && (srcPortWellknownrangeLowerUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							srcPortWellknownrangeLowerUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortWellknownrangeLowerUdp, true , "srcPortWellknownrangeLowerUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortrangeLowerUdpUnion.compare("srcPortCustomrangeLowerUdp") == 0) && (srcPortCustomrangeLowerUdp != DEFAULT_PORTNUM)){
        							srcPortCustomrangeLowerUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortCustomrangeLowerUdp, true, "srcPortCustomrangeLowerUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        						if ((srcPortrangeHigherUdpUnion.compare("srcPortWellknownrangeHigherUdp") == 0) && (srcPortWellknownrangeHigherUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							srcPortWellknownrangeHigherUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(srcPortWellknownrangeHigherUdp, true, "srcPortWellknownrangeHigherUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((srcPortrangeHigherUdpUnion.compare("srcPortCustomrangeHigherUdp") == 0) && (srcPortCustomrangeHigherUdp != DEFAULT_PORTNUM)){
        							srcPortCustomrangeHigherUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(srcPortCustomrangeHigherUdp, true, "srcPortCustomrangeHigherUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}
        				}
        			}

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnum(dstPortOperator, "dstPortOperator"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			if (dstPortOperator) {
        				/* TCP */
        				if (l3l4WellknownProtoType == L3L4_PROTOTYPE_TCP) {
        					if (dstPortOperator == PORTOPERATOR_LT) {
        						if ((dstPortltNumberTcpUnion.compare ("dstPortltCustomNumberTcp") == 0) && (dstPortltCustomNumberTcp != DEFAULT_PORTNUM)) {
        							dstPortltCustomNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortltCustomNumberTcp, true, "dstPortltCustomNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortltNumberTcpUnion.compare ("dstPortltWellknownNumberTcp") == 0)&& (dstPortltWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							dstPortltWellknownNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortltWellknownNumberTcp, true, "dstPortltWellknownNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (dstPortOperator == PORTOPERATOR_GT) {
        						if ((dstPortgtNumberTcpUnion.compare ("dstPortgtCustomNumberTcp") == 0) && (dstPortgtCustomNumberTcp != DEFAULT_PORTNUM)) {
        							dstPortgtCustomNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortgtCustomNumberTcp, true, "dstPortgtCustomNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortgtNumberTcpUnion.compare ("dstPortgtWellknownNumberTcp") == 0) && (dstPortgtWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							dstPortgtWellknownNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortgtWellknownNumberTcp, true, "dstPortgtWellknownNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}  else if ((dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ)) {
        						if ((dstPortNumberTcpUnion.compare ("dstPortWellknownNumberTcp") == 0) && (dstPortWellknownNumberTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							dstPortWellknownNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortWellknownNumberTcp, true, "dstPortWellknownNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortNumberTcpUnion.compare ("dstPortCustomNumberTcp") == 0) && (dstPortCustomNumberTcp != DEFAULT_PORTNUM)) {
        							dstPortCustomNumberTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortCustomNumberTcp, true, "dstPortCustomNumberTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (dstPortOperator == PORTOPERATOR_RANGE) {
        						if ((dstPortrangeLowerTcpUnion.compare("dstPortWellknownrangeLowerTcp") == 0) && (dstPortWellknownrangeLowerTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							dstPortWellknownrangeLowerTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortWellknownrangeLowerTcp, true, "dstPortWellknownrangeLowerTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortrangeLowerTcpUnion.compare("dstPortCustomrangeLowerTcp") == 0) && (dstPortCustomrangeLowerTcp != DEFAULT_PORTNUM)){
        							dstPortCustomrangeLowerTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortCustomrangeLowerTcp, true, "dstPortCustomrangeLowerTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        						if ((dstPortrangeHigherTcpUnion.compare("dstPortWellknownrangeHigherTcp") == 0) && (dstPortWellknownrangeHigherTcp != WELLKNOWNPORT_TCP_UNKNOWN)) {
        							dstPortWellknownrangeHigherTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortWellknownrangeHigherTcp, true, "dstPortWellknownrangeHigherTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortrangeHigherTcpUnion.compare("dstPortCustomrangeHigherTcp") == 0) && (dstPortCustomrangeHigherTcp != DEFAULT_PORTNUM)){
        							dstPortCustomrangeHigherTcp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortCustomrangeHigherTcp, true, "dstPortCustomrangeHigherTcp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}
        				} else if (l3l4WellknownProtoType == L3L4_PROTOTYPE_UDP) {
        					if (dstPortOperator == PORTOPERATOR_LT) {
        						if ((dstPortltNumberUdpUnion.compare ("dstPortltCustomNumberUdp") == 0) && (dstPortltCustomNumberUdp != DEFAULT_PORTNUM)) {
        							dstPortltCustomNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortltCustomNumberUdp, true, "dstPortltCustomNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortltNumberUdpUnion.compare ("dstPortltWellknownNumberUdp") == 0)&& (dstPortltWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							dstPortltWellknownNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortltWellknownNumberUdp, true, "dstPortltWellknownNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (dstPortOperator == PORTOPERATOR_GT) {
        						if ((dstPortgtNumberUdpUnion.compare ("dstPortgtCustomNumberUdp") == 0) && (dstPortgtCustomNumberUdp != DEFAULT_PORTNUM)) {
        							dstPortgtCustomNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortgtCustomNumberUdp, true, "dstPortgtCustomNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortgtNumberUdpUnion.compare ("dstPortgtWellknownNumberUdp") == 0) && (dstPortgtWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							dstPortgtWellknownNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortgtWellknownNumberUdp, true, "dstPortgtWellknownNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}  else if ((dstPortOperator == PORTOPERATOR_EQ) || (dstPortOperator == PORTOPERATOR_NEQ)) {
        						if ((dstPortNumberUdpUnion.compare ("dstPortWellknownNumberUdp") == 0) && (dstPortWellknownNumberUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							dstPortWellknownNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortWellknownNumberUdp, true, "dstPortWellknownNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortNumberUdpUnion.compare ("dstPortCustomNumberUdp") == 0) && (dstPortCustomNumberUdp != DEFAULT_PORTNUM)) {
        							dstPortCustomNumberUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortCustomNumberUdp, true ,"dstPortCustomNumberUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					} else if (dstPortOperator == PORTOPERATOR_RANGE) {
        						if ((dstPortrangeLowerUdpUnion.compare("dstPortWellknownrangeLowerUdp") == 0) && (dstPortWellknownrangeLowerUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							dstPortWellknownrangeLowerUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortWellknownrangeLowerUdp, true , "dstPortWellknownrangeLowerUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortrangeLowerUdpUnion.compare("dstPortCustomrangeLowerUdp") == 0) && (dstPortCustomrangeLowerUdp != DEFAULT_PORTNUM)){
        							dstPortCustomrangeLowerUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortCustomrangeLowerUdp, true, "dstPortCustomrangeLowerUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        						if ((dstPortrangeHigherUdpUnion.compare("dstPortWellknownrangeHigherUdp") == 0) && (dstPortWellknownrangeHigherUdp != WELLKNOWNPORT_UDP_UNKNOWN)) {
        							dstPortWellknownrangeHigherUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(dstPortWellknownrangeHigherUdp, true, "dstPortWellknownrangeHigherUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						} else if ((dstPortrangeHigherUdpUnion.compare("dstPortCustomrangeHigherUdp") == 0) && (dstPortCustomrangeHigherUdp != DEFAULT_PORTNUM)){
        							dstPortCustomrangeHigherUdp.setIsUserConfigured(true);
        							queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(dstPortCustomrangeHigherUdp, true, "dstPortCustomrangeHigherUdp"),
        									WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        						}
        					}

        				}
        			}

        			if (customPrecedence != DEFAULT_PREC_DSCP_TOS_VAL)
        				customPrecedence.setIsUserConfigured(true);
        			if (wellknownPrecedence != WELLKNOWN_PRECEDENCE_UNKNOWN)
        				wellknownPrecedence.setIsUserConfigured(true);
        			if (customDscp != DEFAULT_PREC_DSCP_TOS_VAL)
        				customDscp.setIsUserConfigured(true);
        			if (wellknownDscp != WELLKNOWN_DSCP_UNKNOWN)
        				wellknownDscp.setIsUserConfigured(true);
        			if (customTos != DEFAULT_PREC_DSCP_TOS_VAL)
        				customTos.setIsUserConfigured(true);
        			if (wellknownTos != WELLKNOWN_TOS_UNKNOWN)
        				wellknownTos.setIsUserConfigured(true);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(customPrecedence, true, "customPrecedence"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(wellknownPrecedence, true, "wellknownPrecedence"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeUI32UC(customDscp, true, "customDscp"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(wellknownDscp, true, "wellknownDscp"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(wellknownDscp, true, "wellknownDscp"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeEnumUC(wellknownTos, true, "wellknownTos"),
        					WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        			if (l3l4WellknownProtoType == L3L4_PROTOTYPE_TCP) {
        				/* TCP Flags */

        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeBool(urg, "urg"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeBool(ack, "ack"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeBool(push, "push"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeBool(fin, "fin"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeBool(rst, "rst"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        				queryCtxtForChild.addAttributeToConditionSet ("RULEVALIDATE_PROPERTIES", new AttributeBool(sync, "sync"),
        						WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        			}
        		}

        	}

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
        		if( (status != WAVE_MESSAGE_SUCCESS) && UserEnteredSeqid && (opcode == SSMACL_DCMD_DEL_RULE) && (!ruleFound)) {
        			status = WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE;
        		}
        	}
        }
        else
        {
	        SSM_PLUG_ERR_LVL("IP ACL poliy MO object does not exist");
            status = WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE;
	    }

        pMsg->setRuleid(seqIdTmp);
        /* Setting Seq id before moving to next step */
        if ((status != WAVE_MESSAGE_SUCCESS) && (seqFound || ruleFound) && (opcode == SSMACL_DCMD_ADD_RULE)) {
        	string localizedStatus = "";
        	if(status == WRC_SSM_DCM_ERR_SPL_NO_ERR_MESSAGE) {
        		localizedStatus = FrameworkToolKit::localize(status);
        	} else {
    			vector<Attribute *> errorArguments;
    			errorArguments.push_back(new AttributeUI64(seqid, "seqid"));
    			localizedStatus = FrameworkToolKit::localize (status, errorArguments);
    			for (UI32 j = 0; j < static_cast<UI32> (errorArguments.size()); j++) {
    				delete errorArguments[j];
                }
        	}
			pMsg->addStatusPropagation (status, localizedStatus, FrameworkToolKit::getThisLocationId ());
        }

        if (status != WAVE_MESSAGE_SUCCESS)
            pMsg->setCompletionStatus(status);

        seqContext->executeNextStep(status);
        return;
    }


    void SsmIpAclWorker::QuerychildValidation(WaveManagedObjectSynchronousQueryContext &queryCtxtForChild, ResourceId &status, bool &ruleFound, bool &seqFound, UI32 opcode, UI32 UserEnteredSeqid, UI64 &seqid, UI32 policyType) {

		/**
		* added the selectField value as ruleid, and set the
		* LoadOneToManyRelationships to false to improve the
		* select query performance.
		*/
    	queryCtxtForChild.addSelectField("ruleid");
    	queryCtxtForChild.setLoadOneToManyRelationships(false);

        vector<WaveManagedObject *> *pResultsChildMO = querySynchronously (&queryCtxtForChild);
        UI64 tempSeqid = seqid;
        UI32 numberOfResultsChild = pResultsChildMO->size ();

        if (NULL != pResultsChildMO) {
            if (0 == numberOfResultsChild) {
                SSM_PLUG_ERR_LVL("Can't find the Rule Managed Object");

            } else if (1 == numberOfResultsChild)
            {
                trace(TRACE_LEVEL_INFO,"Rule already exists");

                if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E) {
                    ExtIpAclRuleManagedObject *ExtruleMO = dynamic_cast<ExtIpAclRuleManagedObject *>((*pResultsChildMO)[0]);
                    tempSeqid = ExtruleMO->getRuleid();
                } else if (policyType == SSMACL_TYPE_L3_IPV4_STD_E) {
                    StdIpAclRuleManagedObject *StdruleMO = dynamic_cast<StdIpAclRuleManagedObject *>((*pResultsChildMO)[0]);
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
			else {
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
	
	void SsmIpAclWorker::ssmIpAclRuleSendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		
		ResourceId sendToClusterCompletionStatus 	= pWaveSendToClusterContext->getCompletionStatus ();
		PrismLinearSequencerContext *seqContext 	= reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
		
		SsmGlobalAssocIpRuleToPolicyMessage *pMsg = NULL;

		prismAssert (NULL != seqContext, __FILE__, __LINE__);

		if (sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS) {
			pMsg = reinterpret_cast<SsmGlobalAssocIpRuleToPolicyMessage *>(seqContext->getPPrismMessage());
			if (pMsg->getOpcode() == SSMACL_DCMD_ADD_RULE) {
                WaveMessageStatus sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (FrameworkToolKit::getThisLocationId ());
                if (WAVE_MESSAGE_SUCCESS == sendStatus) {
	                SsmLocalServiceAssocIpRuleToPolicyMessage *phase1Msg = reinterpret_cast<SsmLocalServiceAssocIpRuleToPolicyMessage *>(pWaveSendToClusterContext->getResultingMessageForPhase1(FrameworkToolKit::getThisLocationId ()));
    	            prismAssert (NULL != phase1Msg, __FILE__, __LINE__);
        	        SSM_PLUG_INFO_LVL_S(string("New Rule Id: ") + phase1Msg->getRuleid ());
            	    pMsg->setRuleid(phase1Msg->getRuleid ());
				}
			}
		} else  {
			/* Fix Me : Future Release: For Hydra No problem as no MC */
			SSM_PLUG_INFO_LVL_S(string("Entering else sendToClusterCompletionStatus not succes case in SsmIpAclWorker::ssmIpAclRuleSendToClusterCallback"));
			LocationId locationid;
			NsmUtils::ClusterGetFirstFailureLocation(pWaveSendToClusterContext, locationid);
			sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationid);
		} 
		
		SSM_PLUG_INFO_LVL_S(string("status: ") + sendToClusterCompletionStatus);

		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());

		delete pWaveSendToClusterContext;
	
		seqContext->executeNextStep(sendToClusterCompletionStatus);
        return;
	}


    void SsmIpAclWorker::udpateIpAclRuleToPolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalAssocIpRuleToPolicyMessage *pMsg 				= reinterpret_cast<SsmGlobalAssocIpRuleToPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);
		
		/* Getting Data From Message to Send to Local Plugin */
		UI64 ruleid												= pMsg->getRuleid();
		UI32 action												= pMsg->getAction();
		UI32 policyType											= pMsg->getPolicyType();
		const string policyName									= pMsg->getPolicyName();
		IpAddrType  srcType										= pMsg->getSrcType();
		IpV4Address srcIpAddress								= pMsg->getSrcIpAddress();
		IpV4Address srcIpAddressMask							= pMsg->getSrcIpAddressMask();
		IpV4Address srcHostIpAddress							= pMsg->getSrcHostIpAddress();
		WaveUnion srcUnion										= pMsg->getSrcUnion();
		bool isCount											= pMsg->getIsCount();
		bool isLog												= pMsg->getIsLog();
		UI32 opcode												= (UI32) pMsg->getOpcode();
		ResourceId status										= WAVE_MESSAGE_ERROR;
		WaveSendToClusterContext *pWaveSendToClusterContext 	= NULL;

		SSM_PLUG_INFO_LVL_S(string("Rule id: ") + ruleid + string ("Policy: ") + policyName);
	
		IpAddrType  dstType 									= pMsg->getDstType();	
		IpV4Address	dstIpAddress 								= pMsg->getDstIpAddress();
		IpV4Address dstIpAddressMask						 	= pMsg->getDstIpAddressMask();
		IpV4Address dstHostIpAddress						 	= pMsg->getDstHostIpAddress();
		WaveUnion dstUnion 										= pMsg->getDstUnion();
		WaveUnion ProtocolTypeUnion 							= pMsg->getProtocolTypeUnion();
		UI32 l3l4CustomProtoType 								= pMsg->getL3l4CustomProtoType();
		L3L4wellknownProtoType l3l4WellknownProtoType 			= pMsg->getL3l4WellknownProtoType();
		PortOperator srcPortOperator						 	= pMsg->getSrcPortOperator();
		PortOperator dstPortOperator						 	= pMsg->getDstPortOperator();
		WaveUnion srcPortNumberTcpUnion							= pMsg->getSrcPortNumberTcpUnion();
		WaveUnion srcPortltNumberTcpUnion						= pMsg->getSrcPortltNumberTcpUnion();
		WaveUnion srcPortgtNumberTcpUnion						= pMsg->getSrcPortgtNumberTcpUnion();
		WaveUnion dstPortNumberTcpUnion							= pMsg->getDstPortNumberTcpUnion();
		WaveUnion dstPortltNumberTcpUnion						= pMsg->getDstPortltNumberTcpUnion();
		WaveUnion dstPortgtNumberTcpUnion						= pMsg->getDstPortgtNumberTcpUnion();
		WaveUnion srcPortNumberUdpUnion							= pMsg->getSrcPortNumberUdpUnion();
		WaveUnion srcPortltNumberUdpUnion						= pMsg->getSrcPortltNumberUdpUnion();
		WaveUnion srcPortgtNumberUdpUnion						= pMsg->getSrcPortgtNumberUdpUnion();
		WaveUnion dstPortNumberUdpUnion							= pMsg->getDstPortNumberUdpUnion();
		WaveUnion dstPortltNumberUdpUnion						= pMsg->getDstPortltNumberUdpUnion();
		WaveUnion dstPortgtNumberUdpUnion						= pMsg->getDstPortgtNumberUdpUnion();
		WaveUnion srcPortrangeLowerTcpUnion						= pMsg->getSrcPortrangeLowerTcpUnion();
		WaveUnion dstPortrangeLowerTcpUnion						= pMsg->getDstPortrangeLowerTcpUnion();
		WaveUnion srcPortrangeHigherTcpUnion				 	= pMsg->getSrcPortrangeHigherTcpUnion();
		WaveUnion dstPortrangeHigherTcpUnion				 	= pMsg->getDstPortrangeHigherTcpUnion();
		WaveUnion srcPortrangeLowerUdpUnion						= pMsg->getSrcPortrangeLowerUdpUnion();
		WaveUnion dstPortrangeLowerUdpUnion						= pMsg->getDstPortrangeLowerUdpUnion();
		WaveUnion srcPortrangeHigherUdpUnion					= pMsg->getSrcPortrangeHigherUdpUnion();
		WaveUnion dstPortrangeHigherUdpUnion					= pMsg->getDstPortrangeHigherUdpUnion();
		UI32 srcPortCustomNumberTcp								= pMsg->getSrcPortCustomNumberTcp();
		UI32 srcPortltCustomNumberTcp							= pMsg->getSrcPortltCustomNumberTcp();
		UI32 srcPortgtCustomNumberTcp							= pMsg->getSrcPortgtCustomNumberTcp();
		UI32 dstPortCustomNumberTcp								= pMsg->getDstPortCustomNumberTcp();
		UI32 dstPortltCustomNumberTcp							= pMsg->getDstPortltCustomNumberTcp();
		UI32 dstPortgtCustomNumberTcp							= pMsg->getDstPortgtCustomNumberTcp();
		UI32 srcPortCustomNumberUdp								= pMsg->getSrcPortCustomNumberUdp();
		UI32 srcPortltCustomNumberUdp							= pMsg->getSrcPortltCustomNumberUdp();
		UI32 srcPortgtCustomNumberUdp							= pMsg->getSrcPortgtCustomNumberUdp();
		UI32 dstPortCustomNumberUdp								= pMsg->getDstPortCustomNumberUdp();
		UI32 dstPortltCustomNumberUdp							= pMsg->getDstPortltCustomNumberUdp();
		UI32 dstPortgtCustomNumberUdp							= pMsg->getDstPortgtCustomNumberUdp();
		PortWellknownNumberTcp srcPortWellknownNumberTcp		= pMsg->getSrcPortWellknownNumberTcp();
		PortWellknownNumberTcp srcPortltWellknownNumberTcp		= pMsg->getSrcPortltWellknownNumberTcp();
		PortWellknownNumberTcp srcPortgtWellknownNumberTcp		= pMsg->getSrcPortgtWellknownNumberTcp();
		PortWellknownNumberTcp dstPortWellknownNumberTcp		= pMsg->getDstPortWellknownNumberTcp();
		PortWellknownNumberTcp dstPortltWellknownNumberTcp		= pMsg->getDstPortltWellknownNumberTcp();
		PortWellknownNumberTcp dstPortgtWellknownNumberTcp		= pMsg->getDstPortgtWellknownNumberTcp();
		PortWellknownNumberUdp srcPortWellknownNumberUdp		= pMsg->getSrcPortWellknownNumberUdp();
		PortWellknownNumberUdp srcPortltWellknownNumberUdp		= pMsg->getSrcPortltWellknownNumberUdp();
		PortWellknownNumberUdp srcPortgtWellknownNumberUdp		= pMsg->getSrcPortgtWellknownNumberUdp();
		PortWellknownNumberUdp dstPortWellknownNumberUdp		= pMsg->getDstPortWellknownNumberUdp();
		PortWellknownNumberUdp dstPortltWellknownNumberUdp		= pMsg->getDstPortltWellknownNumberUdp();
		PortWellknownNumberUdp dstPortgtWellknownNumberUdp		= pMsg->getDstPortgtWellknownNumberUdp();
		UI32 srcPortCustomrangeLowerTcp							= pMsg->getSrcPortCustomrangeLowerTcp();
		UI32 dstPortCustomrangeLowerTcp							= pMsg->getDstPortCustomrangeLowerTcp();
		UI32 srcPortCustomrangeLowerUdp							= pMsg->getSrcPortCustomrangeLowerUdp();
		UI32 dstPortCustomrangeLowerUdp							= pMsg->getDstPortCustomrangeLowerUdp();
		UI32 srcPortCustomrangeHigherTcp 						= pMsg->getSrcPortCustomrangeHigherTcp();
		UI32 dstPortCustomrangeHigherTcp 						= pMsg->getDstPortCustomrangeHigherTcp();
		UI32 srcPortCustomrangeHigherUdp 						= pMsg->getSrcPortCustomrangeHigherUdp();
		UI32 dstPortCustomrangeHigherUdp 						= pMsg->getDstPortCustomrangeHigherUdp();
		PortWellknownNumberUdp srcPortWellknownrangeLowerUdp 	= pMsg->getSrcPortWellknownrangeLowerUdp();
		PortWellknownNumberUdp dstPortWellknownrangeLowerUdp 	= pMsg->getDstPortWellknownrangeLowerUdp();
		PortWellknownNumberTcp srcPortWellknownrangeLowerTcp 	= pMsg->getSrcPortWellknownrangeLowerTcp();
		PortWellknownNumberTcp dstPortWellknownrangeLowerTcp 	= pMsg->getDstPortWellknownrangeLowerTcp();
		PortWellknownNumberUdp srcPortWellknownrangeHigherUdp	= pMsg->getSrcPortWellknownrangeHigherUdp();
		PortWellknownNumberUdp dstPortWellknownrangeHigherUdp	= pMsg->getDstPortWellknownrangeHigherUdp();
		PortWellknownNumberTcp srcPortWellknownrangeHigherTcp	= pMsg->getSrcPortWellknownrangeHigherTcp();
		PortWellknownNumberTcp dstPortWellknownrangeHigherTcp	= pMsg->getDstPortWellknownrangeHigherTcp();
		WellknownPrecedence wellknownPrecedence                 = pMsg->getWellknownPrecedence();
		WellknownTos wellknownTos                               = pMsg->getWellknownTos();
		WellknownDscp wellknownDscp                             = pMsg->getWellknownDscp();
		UI32  customPrecedence                                  = pMsg->getCustomPrecedence();
		UI32  customTos                                         = pMsg->getCustomTos();
		UI32  customDscp                                        = pMsg->getCustomDscp();
		WaveUnion precedenceUnion								= pMsg->getPrecedenceUnion();
		WaveUnion tosUnion										= pMsg->getTosUnion();
		WaveUnion dscpUnion										= pMsg->getDscpUnion();	
		bool urg											 	= pMsg->getUrg();
		bool ack											 	= pMsg->getAck();
		bool push											 	= pMsg->getPush();
		bool fin											 	= pMsg->getFin();
		bool rst											 	= pMsg->getRst();
		bool sync								  				= pMsg->getSync();
		UI8	 userEnteredSeqid									= pMsg->getUserEnteredSeqid();
			
		/* Fix Me : leaving it to last hop do decide what need to be filled in msg sent to backend*/
        SsmLocalServiceAssocIpRuleToPolicyMessage *newMsg =
			new SsmLocalServiceAssocIpRuleToPolicyMessage(policyName, policyType, ruleid, action, 
				srcUnion, srcType, srcHostIpAddress, srcIpAddress, srcIpAddressMask, isCount, 
				isLog, dstHostIpAddress, dstIpAddress, dstIpAddressMask, dstUnion, dstType, 
				ProtocolTypeUnion, l3l4CustomProtoType, l3l4WellknownProtoType, srcPortOperator, 
				srcPortNumberTcpUnion, srcPortltNumberTcpUnion, srcPortgtNumberTcpUnion, 
				srcPortNumberUdpUnion, srcPortltNumberUdpUnion, srcPortgtNumberUdpUnion, 
				srcPortrangeLowerTcpUnion, srcPortrangeLowerUdpUnion, srcPortrangeHigherTcpUnion, 
				srcPortrangeHigherUdpUnion, srcPortCustomNumberTcp, srcPortltCustomNumberTcp, 
				srcPortgtCustomNumberTcp, srcPortWellknownNumberTcp, srcPortltWellknownNumberTcp, 
				srcPortgtWellknownNumberTcp, srcPortCustomNumberUdp, srcPortltCustomNumberUdp,
				srcPortgtCustomNumberUdp, srcPortWellknownNumberUdp, 
				srcPortltWellknownNumberUdp, srcPortgtWellknownNumberUdp,
				srcPortCustomrangeLowerTcp, srcPortWellknownrangeLowerTcp, 
				srcPortCustomrangeHigherTcp, srcPortWellknownrangeHigherTcp, srcPortCustomrangeLowerUdp, 
				srcPortWellknownrangeLowerUdp, srcPortCustomrangeHigherUdp, srcPortWellknownrangeHigherUdp, 
				dstPortNumberTcpUnion, dstPortltNumberTcpUnion, dstPortgtNumberTcpUnion,
				dstPortNumberUdpUnion, dstPortltNumberUdpUnion, dstPortgtNumberUdpUnion,
				dstPortrangeLowerTcpUnion, dstPortrangeLowerUdpUnion, 
				dstPortrangeHigherTcpUnion, dstPortrangeHigherUdpUnion, dstPortOperator, 
				dstPortCustomNumberTcp, dstPortltCustomNumberTcp, dstPortgtCustomNumberTcp,
				dstPortWellknownNumberTcp, dstPortltWellknownNumberTcp, dstPortgtWellknownNumberTcp,
				dstPortCustomNumberUdp, dstPortltCustomNumberUdp, dstPortgtCustomNumberUdp,
				dstPortWellknownNumberUdp, dstPortltWellknownNumberUdp, dstPortgtWellknownNumberUdp, 
				dstPortCustomrangeLowerTcp, dstPortWellknownrangeLowerTcp, dstPortCustomrangeHigherTcp, 
				dstPortWellknownrangeHigherTcp, dstPortCustomrangeLowerUdp, dstPortWellknownrangeLowerUdp, 
				dstPortCustomrangeHigherUdp, dstPortWellknownrangeHigherUdp, wellknownPrecedence, wellknownTos, 
				wellknownDscp, customPrecedence, customTos, customDscp, precedenceUnion, tosUnion, dscpUnion,
				urg, ack, push, fin, rst, sync, opcode, userEnteredSeqid);
        
		if (newMsg != NULL) {
        	SSM_PLUG_TRACE_SET_LVL(TRACE_LEVEL_DEVEL, "Message being sent to Local Service");
        	pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmIpAclWorker::ssmIpAclRuleSendToClusterCallback), seqContext);
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
    
	void SsmIpAclWorker::assocIpAclRuleToPolicy(DceLinearSeqContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalAssocIpRuleToPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalAssocIpRuleToPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		UI64 ruleid 								= pMsg->getRuleid();
		UI32 action 								= pMsg->getAction();
		UI32 policyType 							= pMsg->getPolicyType();
		const string policyName 					= pMsg->getPolicyName();
		const IpV4Address HostIpAddress 			= pMsg->getSrcHostIpAddress();
		const IpV4Address srcIpAddress 				= pMsg->getSrcIpAddress();
		const IpV4Address srcHostIpAddress			= pMsg->getSrcHostIpAddress();
		const IpV4Address srcIpAddressMask			= pMsg->getSrcIpAddressMask();
		IpAddrType srcType						   	= pMsg->getSrcType();
		WaveUnion srcUnion						   	= pMsg->getSrcUnion();
		bool isCount 								= pMsg->getIsCount();
		bool isLog 									= pMsg->getIsLog();
		ResourceId status 							= WAVE_MESSAGE_ERROR;
       	//IpAclPolicyManagedObject *policyMO 			= NULL; 
		IpAclRuleManagedObject *ruleMO 				= NULL;
		string className;
		string childClassName;
		string compositionName;
		ObjectId policyObjId = seqContext->getWaveManagedObjectId();
		
		if (policyObjId == ObjectId::NullObjectId) {
			SSM_PLUG_DBG_LVL("Policy id is null");
			seqContext->executeNextStep(WAVE_MESSAGE_ERROR);
			return;
		}

		SSM_PLUG_INFO_LVL_S(string("Rule id: ") + ruleid + string ("Policy: ") + policyName + + policyType);;
		
		startTransaction ();

		/* Update Policy MO with the respective Std/Ext Rule Composition  */
		if (policyType == SSMACL_TYPE_L3_IPV4_STD_E) {
			className = StdIpAclPolicyManagedObject::getClassName();
			ruleMO 	= new StdIpAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			childClassName 	= StdIpAclRuleManagedObject::getClassName();
			compositionName = "rules";
		} else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E) {
			className = ExtIpAclPolicyManagedObject::getClassName();
			ruleMO = new ExtIpAclRuleManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			childClassName = ExtIpAclRuleManagedObject::getClassName();
			compositionName = "extRules";
		} else {
			className = IpAclPolicyManagedObject::getClassName();
			status = WAVE_MESSAGE_ERROR;
		}

		if (ruleMO == NULL) {
			status = WAVE_MESSAGE_ERROR;
		} else {
			ruleMO->setOwnerManagedObjectId(policyObjId);
			addToComposition(className, childClassName,
						compositionName, policyObjId, ruleMO->getObjectId());
			SSM_PLUG_SUCCESS_LVL("Successfully updated the IPv4Policy MO with the rule");
			status = WAVE_MESSAGE_SUCCESS;
		}

		if (status == WAVE_MESSAGE_SUCCESS) {
			ruleMO->setRuleid(ruleid);
			ruleMO->setAction(action);
			ruleMO->setSrcType(srcType);
			ruleMO->setSrcHostIpAddress(srcHostIpAddress);
			ruleMO->setSrcIpAddress(srcIpAddress);
			ruleMO->setSrcIpAddressMask(srcIpAddressMask);
			ruleMO->setSrcUnion(srcUnion); 
			ruleMO->setIsCount(isCount);
			ruleMO->setIsLog(isLog);
			
			if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E) {
				IpAddrType  dstType 										= pMsg->getDstType();	
				IpV4Address	dstIpAddress 									= pMsg->getDstIpAddress();
				IpV4Address dstIpAddressMask 								= pMsg->getDstIpAddressMask();
				IpV4Address dstHostIpAddress 								= pMsg->getDstHostIpAddress();
				WaveUnion dstUnion 											= pMsg->getDstUnion();
				WaveUnion ProtocolTypeUnion 								= pMsg->getProtocolTypeUnion();
				UI32 l3l4CustomProtoType 									= pMsg->getL3l4CustomProtoType();
				L3L4wellknownProtoType l3l4WellknownProtoType 				= pMsg->getL3l4WellknownProtoType();
				PortOperator srcPortOperator 								= pMsg->getSrcPortOperator();
				PortOperator dstPortOperator 								= pMsg->getDstPortOperator();
				WaveUnion srcPortNumberTcpUnion								= pMsg->getSrcPortNumberTcpUnion();
				WaveUnion srcPortltNumberTcpUnion                      		= pMsg->getSrcPortltNumberTcpUnion();
				WaveUnion srcPortgtNumberTcpUnion                       	= pMsg->getSrcPortgtNumberTcpUnion();

	
				WaveUnion dstPortNumberTcpUnion								= pMsg->getDstPortNumberTcpUnion();
				WaveUnion dstPortltNumberTcpUnion							= pMsg->getDstPortltNumberTcpUnion();
				WaveUnion dstPortgtNumberTcpUnion							= pMsg->getDstPortgtNumberTcpUnion();
				WaveUnion srcPortNumberUdpUnion								= pMsg->getSrcPortNumberUdpUnion();

				WaveUnion srcPortltNumberUdpUnion							= pMsg->getSrcPortltNumberUdpUnion();
				WaveUnion srcPortgtNumberUdpUnion							= pMsg->getSrcPortgtNumberUdpUnion();
				

				WaveUnion dstPortNumberUdpUnion								= pMsg->getDstPortNumberUdpUnion();

        		WaveUnion dstPortltNumberUdpUnion							= pMsg->getDstPortltNumberUdpUnion();
        		WaveUnion dstPortgtNumberUdpUnion							= pMsg->getDstPortgtNumberUdpUnion();

				WaveUnion srcPortrangeLowerTcpUnion							= pMsg->getSrcPortrangeLowerTcpUnion();
				WaveUnion dstPortrangeLowerTcpUnion							= pMsg->getDstPortrangeLowerTcpUnion();
				WaveUnion srcPortrangeHigherTcpUnion					 	= pMsg->getSrcPortrangeHigherTcpUnion();
				WaveUnion dstPortrangeHigherTcpUnion					 	= pMsg->getDstPortrangeHigherTcpUnion();
				WaveUnion srcPortrangeLowerUdpUnion							= pMsg->getSrcPortrangeLowerUdpUnion();
				WaveUnion dstPortrangeLowerUdpUnion							= pMsg->getDstPortrangeLowerUdpUnion();
				WaveUnion srcPortrangeHigherUdpUnion					 	= pMsg->getSrcPortrangeHigherUdpUnion();
				WaveUnion dstPortrangeHigherUdpUnion					 	= pMsg->getDstPortrangeHigherUdpUnion();


				UI32 srcPortCustomNumberTcp									= pMsg->getSrcPortCustomNumberTcp();
				UI32 srcPortltCustomNumberTcp								= pMsg->getSrcPortltCustomNumberTcp();
				UI32 srcPortgtCustomNumberTcp								= pMsg->getSrcPortgtCustomNumberTcp();
				UI32 dstPortCustomNumberTcp									= pMsg->getDstPortCustomNumberTcp();
				UI32 dstPortltCustomNumberTcp								= pMsg->getDstPortltCustomNumberTcp();
				UI32 dstPortgtCustomNumberTcp								= pMsg->getDstPortgtCustomNumberTcp();
				UI32 srcPortCustomNumberUdp									= pMsg->getSrcPortCustomNumberUdp();
				UI32 srcPortltCustomNumberUdp								= pMsg->getSrcPortltCustomNumberUdp();
				UI32 srcPortgtCustomNumberUdp								= pMsg->getSrcPortgtCustomNumberUdp();
				UI32 dstPortCustomNumberUdp									= pMsg->getDstPortCustomNumberUdp();
				UI32 dstPortltCustomNumberUdp								= pMsg->getDstPortltCustomNumberUdp();
				UI32 dstPortgtCustomNumberUdp								= pMsg->getDstPortgtCustomNumberUdp();
				PortWellknownNumberTcp srcPortWellknownNumberTcp			= pMsg->getSrcPortWellknownNumberTcp();
				PortWellknownNumberTcp srcPortltWellknownNumberTcp			= pMsg->getSrcPortltWellknownNumberTcp();
				PortWellknownNumberTcp srcPortgtWellknownNumberTcp			= pMsg->getSrcPortgtWellknownNumberTcp();
				PortWellknownNumberTcp dstPortWellknownNumberTcp			= pMsg->getDstPortWellknownNumberTcp();
				PortWellknownNumberTcp dstPortltWellknownNumberTcp			= pMsg->getDstPortltWellknownNumberTcp();
				PortWellknownNumberTcp dstPortgtWellknownNumberTcp			= pMsg->getDstPortgtWellknownNumberTcp();
				PortWellknownNumberUdp srcPortWellknownNumberUdp			= pMsg->getSrcPortWellknownNumberUdp();
				PortWellknownNumberUdp srcPortltWellknownNumberUdp			= pMsg->getSrcPortltWellknownNumberUdp();
				PortWellknownNumberUdp srcPortgtWellknownNumberUdp			= pMsg->getSrcPortgtWellknownNumberUdp();
				PortWellknownNumberUdp dstPortWellknownNumberUdp			= pMsg->getDstPortWellknownNumberUdp();
				PortWellknownNumberUdp dstPortltWellknownNumberUdp			= pMsg->getDstPortltWellknownNumberUdp();
				PortWellknownNumberUdp dstPortgtWellknownNumberUdp			= pMsg->getDstPortgtWellknownNumberUdp();

				UI32 srcPortCustomrangeLowerTcp 							= pMsg->getSrcPortCustomrangeLowerTcp();
				UI32 dstPortCustomrangeLowerTcp								= pMsg->getDstPortCustomrangeLowerTcp();
				UI32 srcPortCustomrangeLowerUdp								= pMsg->getSrcPortCustomrangeLowerUdp();
				UI32 dstPortCustomrangeLowerUdp								= pMsg->getDstPortCustomrangeLowerUdp();
				UI32 srcPortCustomrangeHigherTcp							= pMsg->getSrcPortCustomrangeHigherTcp();
				UI32 dstPortCustomrangeHigherTcp							= pMsg->getDstPortCustomrangeHigherTcp();
				UI32 srcPortCustomrangeHigherUdp							= pMsg->getSrcPortCustomrangeHigherUdp();
				UI32 dstPortCustomrangeHigherUdp							= pMsg->getDstPortCustomrangeHigherUdp();
				PortWellknownNumberUdp srcPortWellknownrangeLowerUdp	 	= pMsg->getSrcPortWellknownrangeLowerUdp();
				PortWellknownNumberUdp dstPortWellknownrangeLowerUdp	 	= pMsg->getDstPortWellknownrangeLowerUdp();
				PortWellknownNumberTcp srcPortWellknownrangeLowerTcp	 	= pMsg->getSrcPortWellknownrangeLowerTcp();
				PortWellknownNumberTcp dstPortWellknownrangeLowerTcp	 	= pMsg->getDstPortWellknownrangeLowerTcp();
				PortWellknownNumberUdp srcPortWellknownrangeHigherUdp	 	= pMsg->getSrcPortWellknownrangeHigherUdp();
				PortWellknownNumberUdp dstPortWellknownrangeHigherUdp	 	= pMsg->getDstPortWellknownrangeHigherUdp();
				PortWellknownNumberTcp srcPortWellknownrangeHigherTcp	 	= pMsg->getSrcPortWellknownrangeHigherTcp();
				PortWellknownNumberTcp dstPortWellknownrangeHigherTcp	 	= pMsg->getDstPortWellknownrangeHigherTcp();
                WellknownPrecedence wellknownPrecedence                 	= pMsg->getWellknownPrecedence();
                WellknownTos wellknownTos                               	= pMsg->getWellknownTos();
                WellknownDscp wellknownDscp                             	= pMsg->getWellknownDscp();
                UI32  customPrecedence                                  	= pMsg->getCustomPrecedence();
                UI32  customTos                                         	= pMsg->getCustomTos();
                UI32  customDscp                                        	= pMsg->getCustomDscp();
				WaveUnion precedenceUnion									= pMsg->getPrecedenceUnion();
				WaveUnion dscpUnion											= pMsg->getDscpUnion();
				WaveUnion tosUnion											= pMsg->getTosUnion();
				bool urg												 	= pMsg->getUrg();
				bool ack												 	= pMsg->getAck();
				bool push												 	= pMsg->getPush();
				bool fin												 	= pMsg->getFin();
				bool rst												 	= pMsg->getRst();
				bool sync												 	= pMsg->getSync();
				
				/* Assigning  Values to MO fields */	
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstType(dstType);
				if (dstType == IP_ADDR_TYPE_HOST) {
					dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstHostIpAddress(dstHostIpAddress);
				} else if (dstType == IP_ADDR_TYPE_NONE) {
					dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstIpAddress(dstIpAddress);
					dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstIpAddressMask(dstIpAddressMask);
				}	
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstUnion(dstUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setProtocolTypeUnion(ProtocolTypeUnion);
				if (ProtocolTypeUnion.compare("l3l4CustomProtoType") == 0) {
					dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setL3l4CustomProtoType(l3l4CustomProtoType);
				} else if (ProtocolTypeUnion.compare("l3l4WellknownProtoType") == 0){
					dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setL3l4WellknownProtoType(l3l4WellknownProtoType);
				}
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortOperator(srcPortOperator);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortOperator(dstPortOperator);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortNumberTcpUnion(srcPortNumberTcpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortltNumberTcpUnion(srcPortltNumberTcpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortgtNumberTcpUnion(srcPortgtNumberTcpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortNumberTcpUnion(dstPortNumberTcpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortltNumberTcpUnion(dstPortltNumberTcpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortgtNumberTcpUnion(dstPortgtNumberTcpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortNumberUdpUnion(srcPortNumberUdpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortltNumberUdpUnion(srcPortltNumberUdpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortgtNumberUdpUnion(srcPortgtNumberUdpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortNumberUdpUnion(dstPortNumberUdpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortltNumberUdpUnion(dstPortltNumberUdpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortgtNumberUdpUnion(dstPortgtNumberUdpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortrangeLowerTcpUnion(srcPortrangeLowerTcpUnion);	 
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortrangeLowerTcpUnion(dstPortrangeLowerTcpUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortrangeHigherTcpUnion(srcPortrangeHigherTcpUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortrangeHigherTcpUnion(dstPortrangeHigherTcpUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortrangeLowerUdpUnion(srcPortrangeLowerUdpUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortrangeLowerUdpUnion(dstPortrangeLowerUdpUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortrangeHigherUdpUnion(srcPortrangeHigherUdpUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortrangeHigherUdpUnion(dstPortrangeHigherUdpUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortCustomNumberTcp(srcPortCustomNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortltCustomNumberTcp(srcPortltCustomNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortgtCustomNumberTcp(srcPortgtCustomNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortCustomNumberTcp(dstPortCustomNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortltCustomNumberTcp(dstPortltCustomNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortgtCustomNumberTcp(dstPortgtCustomNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortCustomNumberUdp(srcPortCustomNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortltCustomNumberUdp(srcPortltCustomNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortgtCustomNumberUdp(srcPortgtCustomNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortCustomNumberUdp(dstPortCustomNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortltCustomNumberUdp(dstPortltCustomNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortgtCustomNumberUdp(dstPortgtCustomNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortWellknownNumberTcp(srcPortWellknownNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortltWellknownNumberTcp(srcPortltWellknownNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortgtWellknownNumberTcp(srcPortgtWellknownNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortWellknownNumberTcp(dstPortWellknownNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortltWellknownNumberTcp(dstPortltWellknownNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortgtWellknownNumberTcp(dstPortgtWellknownNumberTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortWellknownNumberUdp(srcPortWellknownNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortltWellknownNumberUdp(srcPortltWellknownNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortgtWellknownNumberUdp(srcPortgtWellknownNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortWellknownNumberUdp(dstPortWellknownNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortltWellknownNumberUdp(dstPortltWellknownNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortgtWellknownNumberUdp(dstPortgtWellknownNumberUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeLowerTcp(srcPortCustomrangeLowerTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeLowerTcp(dstPortCustomrangeLowerTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeLowerUdp(srcPortCustomrangeLowerUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeLowerUdp(dstPortCustomrangeLowerUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeHigherTcp(srcPortCustomrangeHigherTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeHigherTcp(dstPortCustomrangeHigherTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortCustomrangeHigherUdp(srcPortCustomrangeHigherUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortCustomrangeHigherUdp(dstPortCustomrangeHigherUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeLowerUdp(srcPortWellknownrangeLowerUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeLowerUdp(dstPortWellknownrangeLowerUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeLowerTcp(srcPortWellknownrangeLowerTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeLowerTcp(dstPortWellknownrangeLowerTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeHigherUdp(srcPortWellknownrangeHigherUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeHigherUdp(dstPortWellknownrangeHigherUdp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSrcPortWellknownrangeHigherTcp(srcPortWellknownrangeHigherTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDstPortWellknownrangeHigherTcp(dstPortWellknownrangeHigherTcp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setCustomTos(customTos);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setCustomPrecedence(customPrecedence);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setCustomDscp(customDscp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setWellknownPrecedence(wellknownPrecedence);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setWellknownDscp(wellknownDscp);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setWellknownTos(wellknownTos);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setPrecedenceUnion(precedenceUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setDscpUnion(dscpUnion);
				dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setTosUnion(tosUnion);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setUrg(urg);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setAck(ack);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setPush(push);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setFin(fin);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setRst(rst);
                dynamic_cast <ExtIpAclRuleManagedObject *>(ruleMO)->setSync(sync);
			}
			
        	status = commitTransaction ();
        	if (status != FRAMEWORK_SUCCESS) {
        		SSM_PLUG_ERR_LVL_S(string("Commit transaction failed: ") + status);
       			status = WAVE_MESSAGE_ERROR;
        	} else {
        		SSM_PLUG_SUCCESS_LVL("Commit Succeeded");
       			status = WAVE_MESSAGE_SUCCESS;
			}
		} else {
			SSM_PLUG_INFO_LVL("Rollback transaction");
			rollbackTransaction();
		}

		if(ruleMO) {
			delete (ruleMO);
		}
		seqContext->executeNextStep(status);
	}
	
    void SsmIpAclWorker::disAssocIpAclRuleToPolicy(DceLinearSeqContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalAssocIpRuleToPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalAssocIpRuleToPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);
		UI64 ruleid 								= pMsg->getRuleid();
		const string policyName 					= pMsg->getPolicyName();
		UI32 policyType 							= pMsg->getPolicyType();
		ResourceId status 							= WAVE_MESSAGE_ERROR;
		string className;
        string childClassName;
        string compositionName;

		SSM_PLUG_INFO_LVL_S(string("Rule id: ") + ruleid + string ("Policy: ") + policyName + policyType);
        ObjectId policyObjId = seqContext->getWaveManagedObjectId();

        if (policyObjId == ObjectId::NullObjectId) {
            SSM_PLUG_DBG_LVL("Policy id is null");
            seqContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }

		if (policyType == SSMACL_TYPE_L3_IPV4_STD_E) {
			className 		= StdIpAclPolicyManagedObject::getClassName();
            childClassName 	= StdIpAclRuleManagedObject::getClassName();
            compositionName = "rules";
		} else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E) {
			className 		= ExtIpAclPolicyManagedObject::getClassName();
            childClassName 	= ExtIpAclRuleManagedObject::getClassName();
            compositionName = "extRules";
		}
		else {
			// This case should not appear and it is error case
			className = IpAclPolicyManagedObject::getClassName();
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
				IpAclRuleManagedObject *ruleMO = dynamic_cast<IpAclRuleManagedObject *>((*pResultsChildMO)[0]);
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

    UI32  SsmIpAclWorker::ssmIsipPolicyAttachInterface(const string& policy, DceLinearSeqContext *seqContext)
    {
        WaveManagedObjectSynchronousQueryContext
            aclSyncQueryCtxt(IpAclPolicyManagedObject::getClassName());

        aclSyncQueryCtxt.addAndAttribute (new AttributeString (policy, "policyName"));
		aclSyncQueryCtxt.addSelectFieldForObjectId ("objectId");
		aclSyncQueryCtxt.setLoadCompositions(false);
		aclSyncQueryCtxt.setLoadOneToManyRelationships(false);
        vector<WaveManagedObject *> *pResultsP = querySynchronously (&aclSyncQueryCtxt);
        if (pResultsP) {
            ObjectId policyId                   = ObjectId::NullObjectId;
            IpAclPolicyManagedObject* policyMO  = NULL;
            if (pResultsP->size()) {
                policyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResultsP)[0]);
                if (policyMO) {
                    policyId                    = policyMO->getObjectId();
					// storing the policy if into the sequencer context to reuse it.
					seqContext->setWaveManagedObjectId(policyId);

					UI32 aclAssocCount = 0;
					WaveManagedObjectSynchronousQueryContext pSyncQueryCtx(NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName());
					pSyncQueryCtx.addAndAttribute(new AttributeObjectIdAssociation(policyId, "ingressEgressIpAcl",IpAclPolicyManagedObject::getClassName()));
					pSyncQueryCtx.setFilterByConnectedNodes(true);
					ResourceId status = querySynchronouslyForCount(&pSyncQueryCtx, aclAssocCount);

					if ( WAVE_MESSAGE_SUCCESS == status )
					{
						if ( aclAssocCount > 0 )
						{
							trace (TRACE_LEVEL_INFO, policy + string (" IP ACL applied on the interface."));
							return (1);
						}
					}
					else
					{
						trace (TRACE_LEVEL_ERROR, "NsmLocalIngressEgressIpAclPolicyManagedObject database query failed");
					}


/* optimized the query above, and commented out the below code
                    IntfManagedObject *intfMO   = NULL;
                    UI32 size                   = 0;

                    string intfManagedObjectClassName[3] = {
                    								PhyIntfLocalManagedObject::getClassName(),
                    								PoIntfManagedObject::getClassName(),
                    								NsmIntfSviManagedObject::getClassName()
                    							};
                    WaveManagedObjectSynchronousQueryContext *syncQueryCtxt = NULL;

                    for( size_t strIndex = 0; strIndex < 2; strIndex ++ ) {
                    	syncQueryCtxt = new WaveManagedObjectSynchronousQueryContext(intfManagedObjectClassName[strIndex]);
                    	if(strIndex == 0 || strIndex == 2) {
                    		// For PhyIntfLocalManagedObject and NsmIntfSviManagedObject only
                    		syncQueryCtxt->setFilterByConnectedNodes(true);
                    	}
                        vector<WaveManagedObject *> *pResultsMO = querySynchronously (syncQueryCtxt);
                     if (NULL != pResultsMO) {
                        UI32 numberOfResults = pResultsMO->size ();
                        for (UI32 i = 0; i < numberOfResults; i++) {
                            intfMO = dynamic_cast<IntfManagedObject *>((*pResultsMO)[i]);
                            if (intfMO != NULL) {
                                vector<WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> >
                                    NsmIngressEgressIpAclPolicyMoVector = intfMO->getIngressEgressIpAclPolicy();
                                size = NsmIngressEgressIpAclPolicyMoVector.size();
                                for (UI32 j = 0; j < size; j++) {
                                    if (((NsmIngressEgressIpAclPolicyMoVector[j])->getIngressEgressIpAcl()) == policyId) {        
                                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsMO);
                                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsP);
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
comment ends */
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsP);
		}		
		/* check for the ACL applied on any mgmt interface, this API returns 1 if ACL applied else 0 */
		return (isIpPolicyAttachToMgmtInterface(policy));
	}
	
    void SsmIpAclWorker::validateDeletePolicy(DceLinearSeqContext *seqContext)
    {
        SsmGlobalRemIpAclPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalRemIpAclPolicyMessage *>(seqContext->getPPrismMessage());
        UI32 IsipPolicyAttachInterface			= 0;	
        ResourceId status 			     		= WAVE_MESSAGE_SUCCESS;

        IsipPolicyAttachInterface = ssmIsipPolicyAttachInterface(pMsg->getPolicyName(), seqContext);
        pMsg->setPolicyId(seqContext->getWaveManagedObjectId());
        status 									 = QosUtils::isMacACLReferredByAnyClassMap(pMsg->getPolicyName());
        if (IsipPolicyAttachInterface)
            status = WRC_SSM_DCM_ERR_IP_ACL_APPLIED;
        trace(TRACE_LEVEL_INFO, string ("validateDeletePolicy: ") + status);
        seqContext->executeNextStep(status);
    }

    void SsmIpAclWorker::deleteIpAclPolicy(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalRemIpAclPolicyMessage *pMsg			= reinterpret_cast<SsmGlobalRemIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName								= pMsg->getPolicyName();
		UI32 policyType									= pMsg->getPolicyType();
		ResourceId  status								= WAVE_MESSAGE_ERROR;

        SsmLocalServiceRemIpAclPolicyMessage *newMsg 	= new SsmLocalServiceRemIpAclPolicyMessage (policyName, policyType);

        if (newMsg != NULL) {
        	SSM_PLUG_TRACE_SET_LVL(TRACE_LEVEL_DEVEL, "Message being sent to Local Service");
        	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmIpAclWorker::ssmIpAclPolicySendToClusterCallback), seqContext);
        	pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
        	pWaveSendToClusterContext->setPartialSuccessFlag(true);
        	pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
        	sendToWaveCluster (pWaveSendToClusterContext);
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
			seqContext->executeNextStep(status);
		}

	}

	void SsmIpAclWorker::deleteAssociationwithDisconnectedPolicy (PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		trace (TRACE_LEVEL_DEBUG, "Inside deleteAssociationwithDisconnectedPolicy function");
		SsmGlobalRemIpAclPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalRemIpAclPolicyMessage *>(seqContext->getPPrismMessage());
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

    void SsmIpAclWorker::deleteIpAclPolicyMO(DceLinearSeqContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		SsmGlobalRemIpAclPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalRemIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName 						= pMsg->getPolicyName();
		ResourceId  status   					= WAVE_MESSAGE_ERROR;
		
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
        // deleteWaveManagedObject(policyObjId);
		string moClassName = "";

		if (pMsg->getPolicyType() == SSMACL_TYPE_L3_IPV4_STD_E)
		{
			moClassName = StdIpAclPolicyManagedObject::getClassName();
		}
		else if (pMsg->getPolicyType() == SSMACL_TYPE_L3_IPV4_EXT_E)
		{
			moClassName = ExtIpAclPolicyManagedObject::getClassName();
		}

        WaveManagedObjectSynchronousQueryContextForDeletion *syncQueryCtxtForMultipleDeletion = new WaveManagedObjectSynchronousQueryContextForDeletion(moClassName);
        syncQueryCtxtForMultipleDeletion->addAndAttribute (new AttributeObjectId (policyObjId, "objectId"));

        deleteMultipleWaveManagedObjects(syncQueryCtxtForMultipleDeletion);

		SSM_PLUG_SUCCESS_LVL_S(string("Removed Policy: ") + policyName);

		seqContext->executeNextStep(status);
	}
	
	void SsmIpAclWorker::ssmIpAclPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		
		ResourceId sendToClusterCompletionStatus	= pWaveSendToClusterContext->getCompletionStatus ();
		PrismLinearSequencerContext *seqContext 	= reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());

		prismAssert (NULL != seqContext, __FILE__, __LINE__);

//		if (sendToClusterCompletionStatus != WAVE_MESSAGE_SUCCESS) {
//			/* Fix Me : In Hydra Release No problem as no MC */
//			SSM_PLUG_INFO_LVL_S(string("Entering sendToClusterCompletionStatus != WAVE_MESSAGE_SUCCESS of SsmIpAclWorker::ssmIpAclPolicySendToClusterCallback"));
//			LocationId locationid;
//			NsmUtils::ClusterGetFirstFailureLocation(pWaveSendToClusterContext, locationid);
//			sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationid);
//		}
		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());

		delete pWaveSendToClusterContext;

		SSM_PLUG_INFO_LVL_S(string("Completion Status: ") + sendToClusterCompletionStatus)

		seqContext->executeNextStep(sendToClusterCompletionStatus);
		
	}

	ResourceId SsmIpAclWorker::getIpAclPolicyType(const string &policyName, ObjectId &policyObjId, UI32 &policyType)
	{
		IpAclPolicyManagedObject *policyMO 	= NULL;
		ResourceId status 					= WAVE_MESSAGE_ERROR;

		SSM_PLUG_DBG_TRACE ();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IpAclPolicyManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
		syncQueryCtxt.addSelectFieldForObjectId ("objectId");
		syncQueryCtxt.setLoadOneToManyRelationships(false);
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if ( 0 == numberOfResults) {
				SSM_PLUG_INFO_LVL("IPAcl Policy Object needs to be created");
				status = WAVE_MESSAGE_SUCCESS;
			}else if (1 == numberOfResults) {
				SSM_PLUG_INFO_LVL_S(string("Found IPAcl policy with the name: ") + policyName);
				policyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResults)[0]);
				policyObjId = policyMO->getObjectId ();
				const string tableName = OrmRepository::getTableNameById (policyObjId.getClassId ());
				SSM_PLUG_INFO_LVL_S(string("Table Name: ") + tableName);
				
				if (tableName.compare("ExtIpAclPolicyManagedObject") == 0) {
					policyType = SSMACL_TYPE_L3_IPV4_EXT_E;
				} else if (tableName.compare("StdIpAclPolicyManagedObject") == 0) {
					policyType = SSMACL_TYPE_L3_IPV4_STD_E;
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

	void SsmIpAclWorker::validatePolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddIpAclPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalAddIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName 						= pMsg->getPolicyName();
		UI32 policyType 						= pMsg->getPolicyType();
		ResourceId  status   					= WAVE_MESSAGE_ERROR;
		UI32 oldPolicyType 						= SSMACL_TYPE_UNKNOWN_E;
		ObjectId policyObjId 					= ObjectId::NullObjectId; 
        UI32 count 								= 0;

		pMsg->setOldPolicyId(policyObjId);
		/* (a) First Check whether ACL Policy with same name exists for MAC ACL
		 * (b) Check whether ACL Policy with same name exists for IPv6 ACL
		 * (c) Check for IP Acl*/
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(MacAclPolicyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        querySynchronouslyForCount(&syncQueryCtxt, count);

        if (count == 0) {
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
			status = WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS_MAC;
			seqContext->executeNextStep(status);
			return;
		}

		status = getIpAclPolicyType(policyName, policyObjId, oldPolicyType);
		if (status == WRC_SSM_DCM_ERR_ACL_ALREADY_EXISTS) {
			if (policyType == oldPolicyType || oldPolicyType == SSMACL_TYPE_UNKNOWN_E) {
				if (oldPolicyType == SSMACL_TYPE_UNKNOWN_E && policyObjId != ObjectId::NullObjectId) {
					pMsg->setOldPolicyId(policyObjId);
					SSM_PLUG_INFO_LVL("Got valid objectId");
					status = WAVE_MESSAGE_SUCCESS;
				}
			}

		} 
		
		SSM_PLUG_INFO_LVL_S(string ("status: ") + status);
		seqContext->executeNextStep(status);
		
	}
    
	void SsmIpAclWorker::createIpAclPolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddIpAclPolicyMessage *pMsg			= reinterpret_cast<SsmGlobalAddIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName								= pMsg->getPolicyName();
		UI32 policyType									= pMsg->getPolicyType();
		ResourceId  status								= WAVE_MESSAGE_ERROR;
		
        SsmLocalServiceAddIpAclPolicyMessage *newMsg 	= new SsmLocalServiceAddIpAclPolicyMessage (policyName, policyType);
        if (newMsg != NULL) {
        	SSM_PLUG_DBG_LVL("Message being sent to Local service");
        	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmIpAclWorker::ssmIpAclPolicySendToClusterCallback), seqContext);
        	pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
        	pWaveSendToClusterContext->setPartialSuccessFlag(true);
        	pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
        	sendToWaveCluster (pWaveSendToClusterContext);
        	trace (TRACE_LEVEL_DEVEL, "createIpAclPolicy: updated Local Service");
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
			seqContext->executeNextStep(status);
		}

	}
	
    void SsmIpAclWorker::createIpAclPolicyMO(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddIpAclPolicyMessage *pMsg		= reinterpret_cast<SsmGlobalAddIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName							= pMsg->getPolicyName();
		UI32 policyType								= pMsg->getPolicyType();
		ResourceId  status							= WAVE_MESSAGE_ERROR;
		StdIpAclPolicyManagedObject *stdACLPolicyMO = NULL;
		ExtIpAclPolicyManagedObject *extACLPolicyMO = NULL;
		IpAclPolicyManagedObject *l3ACLPolicyMO 	= NULL; 
		pMsg->setNewPolicyId(ObjectId::NullObjectId);
		

		startTransaction ();

		if (policyType == SSMACL_TYPE_L3_IPV4_STD_E)  {
			stdACLPolicyMO 	= new StdIpAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			l3ACLPolicyMO 	= dynamic_cast<IpAclPolicyManagedObject*>(stdACLPolicyMO);
		}
		else if (policyType == SSMACL_TYPE_L3_IPV4_EXT_E){
			extACLPolicyMO 	= new ExtIpAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
			l3ACLPolicyMO 	= dynamic_cast<IpAclPolicyManagedObject*>(extACLPolicyMO);
		} 
		else {
       		rollbackTransaction();
       		seqContext->executeNextStep(status);
			return;
		}

        if (l3ACLPolicyMO == NULL) {
        	SSM_PLUG_ERR_LVL_S(string("Creating policy MO failed for policy: ") + policyName);
       		rollbackTransaction();
		} else {
        	l3ACLPolicyMO->setPolicyName(policyName);
        	status = commitTransaction ();
        	if (status != FRAMEWORK_SUCCESS) {
        		SSM_PLUG_ERR_LVL_S(string("commit transaction failed: ") + status);
       			status = WAVE_MESSAGE_ERROR;
        	} else {
        		SSM_PLUG_SUCCESS_LVL("Successfully created policy");
				trace(TRACE_LEVEL_SUCCESS, "CreatedteIpAclPolicyMO: Policy Mo created");
       			status = WAVE_MESSAGE_SUCCESS;
			}
			pMsg->setNewPolicyId(l3ACLPolicyMO->getObjectId());

			delete l3ACLPolicyMO;
		}

       	seqContext->executeNextStep(status);
		
	}

    
	void SsmIpAclWorker::deleteOldIpAclPolicyMO(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalAddIpAclPolicyMessage *pMsg 	= reinterpret_cast<SsmGlobalAddIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		ResourceId  status 						= WAVE_MESSAGE_ERROR;
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

		NsmGlobalReAssocIpAclPolicyMessage *nsmMsg = new NsmGlobalReAssocIpAclPolicyMessage ();

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

	void  SsmIpAclWorker::SsmGlobalReseqIpAclPolicyMessageHandler( SsmGlobalReseqIpAclPolicyMessage *pSsmGlobalReseqIpAclPolicyMessage)
    { 
    	SSM_PLUG_DBG_TRACE();

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::updateresequenceIpAclPolicy),
            
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::resequenceIpAclPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerCommitTransactionStep),
           
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmIpAclWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext ( pSsmGlobalReseqIpAclPolicyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

	
	void SsmIpAclWorker::ssmIpAclReseqPolicySendToClusterCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
		PrismLinearSequencerContext *seqContext = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());

		prismAssert (NULL != seqContext, __FILE__, __LINE__);

		if (sendToClusterCompletionStatus != WAVE_MESSAGE_SUCCESS) {
			// Fix it properly to return all failures
			LocationId locationid;
			NsmUtils::ClusterGetFirstFailureLocation(pWaveSendToClusterContext, locationid);
			sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationid);
		} 

		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
		delete pWaveSendToClusterContext;

		SSM_PLUG_INFO_LVL_S(string("Completion Status: ") + sendToClusterCompletionStatus);

		seqContext->executeNextStep(sendToClusterCompletionStatus);
		
   }



    void SsmIpAclWorker::updateresequenceIpAclPolicy(PrismLinearSequencerContext *seqContext)
	{

		SSM_PLUG_DBG_TRACE();
		SsmGlobalReseqIpAclPolicyMessage  *pMsg			= reinterpret_cast<SsmGlobalReseqIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName								= pMsg->getPolicyName();
		UI64 seqId										= pMsg->getSeqId();
		UI64 incseqId									= pMsg->getIncSeqId();
		ResourceId  status								= WAVE_MESSAGE_ERROR;

        SsmLocalServiceResqIpAclPolicyMessage *newMsg 	= new SsmLocalServiceResqIpAclPolicyMessage (policyName,seqId,incseqId);
        
		if (newMsg != NULL) {
        	SSM_PLUG_TRACE_SET_LVL(TRACE_LEVEL_DEVEL, "Message being sent to Local Service");
        	WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&SsmIpAclWorker::ssmIpAclReseqPolicySendToClusterCallback), seqContext);
        	pWaveSendToClusterContext->setPPrismMessageForPhase1(newMsg);
        	pWaveSendToClusterContext->setPartialSuccessFlag(true);
        	pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
        	sendToWaveCluster (pWaveSendToClusterContext);
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
			seqContext->executeNextStep(status);
		}

	}
    void SsmIpAclWorker::resequenceIpAclPolicy(PrismLinearSequencerContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE ();
		SsmGlobalReseqIpAclPolicyMessage  *pMsg = reinterpret_cast<SsmGlobalReseqIpAclPolicyMessage *>(seqContext->getPPrismMessage());
		SSM_PLUG_ASSERT_PTR(pMsg);

		string policyName 						= pMsg->getPolicyName();
		UI64 seqId 								= pMsg->getSeqId();
		UI64 incseqId 							= pMsg->getIncSeqId();
	    UI32 numberOfResults,numberOfResults1;	
        ResourceId status 						= WAVE_MESSAGE_ERROR;
		string className;
        int i;

		vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > extRuleMoVector;
		vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > stdRuleMoVector;

		ExtIpAclPolicyManagedObject *extPolicyMO = NULL;
		StdIpAclPolicyManagedObject *stdPolicyMO = NULL;
		
		SSM_PLUG_INFO_LVL_S(string("Rule id: ") + seqId + string ("Policy: ") + policyName);
        
       	/* Query the MO for a given policy*/

		className 								= StdIpAclPolicyManagedObject::getClassName();
		WaveManagedObjectSynchronousQueryContext stdsyncQueryCtxt(className);
	    stdsyncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
		vector<WaveManagedObject *> *pResults 	= querySynchronously (&stdsyncQueryCtxt);

		if (NULL != pResults) {
		  //  startTransaction ();
			numberOfResults = pResults->size ();
			if (1 == numberOfResults) {
				/* Update Policy MO with the corresponding Rule Composition  */
				stdPolicyMO = dynamic_cast<StdIpAclPolicyManagedObject *>((*pResults)[0]);
				stdPolicyMO->sortRules();
				stdRuleMoVector = stdPolicyMO->getRules();
				SSM_PLUG_INFO_LVL_S(string("vectorsize")+stdRuleMoVector.size());
			    for (i = 0; i < (int) stdRuleMoVector.size(); i++) {
               	     stdRuleMoVector[i]->setRuleid(seqId +(incseqId * i)) ;
                     StdIpAclRuleManagedObject  *stdRuleMO = stdRuleMoVector[i].operator-> (); 
                     updateWaveManagedObject(stdRuleMO);
				SSM_PLUG_INFO_LVL_S(string("vectorsize ")+stdRuleMoVector.size()+string(" updated the rule"));
			    }
				updateWaveManagedObject(stdPolicyMO);
				SSM_PLUG_SUCCESS_LVL("Successfully updated the Policy MO with the Sequence Id");
				status = WAVE_MESSAGE_SUCCESS;
			} else if (0 == numberOfResults) {
				className = ExtIpAclPolicyManagedObject::getClassName();
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
						extPolicyMO = dynamic_cast<ExtIpAclPolicyManagedObject *>((*pResults1)[0]);
						extPolicyMO->sortRules();
						extRuleMoVector = extPolicyMO->getRules();
			        	for (i = 0; i < (int) extRuleMoVector.size(); i++) {
                        	extRuleMoVector[i]->setRuleid(seqId +(incseqId * i)) ;
                     		ExtIpAclRuleManagedObject *extRuleMO = extRuleMoVector[i].operator-> ();
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

    void SsmIpAclWorker::SsmGlobalGetIpAclPolicyMessageHandler(SsmGlobalGetIpAclPolicyMessage *pSsmGlobalGetIpAclPolicyMessage)
	{
		SSM_PLUG_DBG_TRACE();
		const string policyName 			= pSsmGlobalGetIpAclPolicyMessage->getPolicyName();
		IpAclPolicyManagedObject *policyMO 	= NULL;
		int status 							= WAVE_MESSAGE_ERROR;
		
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IpAclPolicyManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (policyName, "policyName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				startTransaction();
				policyMO = new IpAclPolicyManagedObject(dynamic_cast<SsmGlobalObjectManager*>(getPWaveObjectManager()));
				if (policyMO) {
					policyMO->setPolicyName(policyName);
        			status = commitTransaction ();
        			if (status != FRAMEWORK_SUCCESS) {
        				SSM_PLUG_ERR_LVL("IP ACL Policy MO commit failed");
       					status = WAVE_MESSAGE_ERROR;
        			} else {
						SSM_PLUG_SUCCESS_LVL("Unknown Policy type IP ACL Mo created");
						pSsmGlobalGetIpAclPolicyMessage->setPolicyObjId(policyMO->getObjectId ());
       					status = WAVE_MESSAGE_SUCCESS;
					}
					delete policyMO;
				} else {
					rollbackTransaction();
				}
			}else if (1 == numberOfResults) {
        		SSM_PLUG_INFO_LVL("Found Existing Policy");
				policyMO = dynamic_cast<IpAclPolicyManagedObject *>((*pResults)[0]);
				pSsmGlobalGetIpAclPolicyMessage->setPolicyObjId(policyMO->getObjectId ());
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

		pSsmGlobalGetIpAclPolicyMessage->setCompletionStatus (status);
		reply(pSsmGlobalGetIpAclPolicyMessage);

	}

/* this routine queries the AAA IP ACL MO to check if the ACL is applied on mgmt interface */
int SsmIpAclWorker::isIpPolicyAttachToMgmtInterface(const string& policyName)
{
	UI32 count = 0;

	// Get all the switches which enforced this policy on mgmt interface.
	WaveManagedObjectSynchronousQueryContext pSyncQueryCtx(IpAclPolicyLocalManagedObject::getClassName());
	pSyncQueryCtx.addAndAttribute (new AttributeString(policyName, "ipaclpolicy"));

	ResourceId status = querySynchronouslyForCount(&pSyncQueryCtx, count);

	if ( WAVE_MESSAGE_SUCCESS == status )
	{
		if ( 0 != count )
		{
			trace (TRACE_LEVEL_INFO, policyName + string ("Is applied on mgmt interface"));
		}
		else
		{
			trace (TRACE_LEVEL_INFO, string ("None of the switches are enforced with this policy") + policyName);
		}
	}
	else
	{
		trace (TRACE_LEVEL_INFO, "Ip/Ip6AclPolicyLocalManagedObject database failed");
	}

	return (count);
}
}

