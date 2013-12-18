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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	*
 *   All rights reserved.                                               *
 *   Author : bpatel                                                    *
 **************************************************************************/

#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmLocalIpAclWorker.h"
#include "Ssm/Local/SsmLocalServiceAssocIpRuleToPolicyMessage.h"
#include "Ssm/Local/SsmMacIpAclLocalClearMessage.h"
#include "Ssm/Local/SsmLocalServiceAddIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceRemIpAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceResqIpAclPolicyMessage.h"
#include "Ssm/Local/SsmIpAclShowLocalSPMessage.h"
#include "Ssm/Local/SsmTypes.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include <sstream>
#include <algorithm>
#include "ClientInterface/Ssm/SsmIpAclShowMessage.h"
#include "ClientInterface/Ssm/SsmIpAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmIpAclRuleMessage.h"
#include "ClientInterface/Ssm/AclClearStatsMessage.h"
#include "Ssm/Global/SsmPlugUtils.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Ssm/SsmIpAclReseqMessage.h"
#include "ClientInterface/Ssm/SsmIpAclPolicyBulkMessage.h"

/* MO's For postboot*/
#include "Ssm/Global/StdIpAclPolicyManagedObject.h"
#include "Ssm/Global/ExtIpAclPolicyManagedObject.h"
#include "Ssm/Global/StdIpAclRuleManagedObject.h"
#include "Ssm/Global/ExtIpAclRuleManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
/* API's for postboot */
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Layer3/L3PostBootUtil.h"
#include "ClientInterface/Ssm/SsmIpAclIntfAssocBulkMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/Capabilities.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCStatus.h"

/* Show Cmds Related Include files */
#include "Ssm/Local/SsmMacIpAclLocalShowMessage.h"
#include "ClientInterface/Ssm/SsmMacIpAclShowMessage.h"
#include "ClientInterface/InterfaceCommon.h"

/* for AAA plugin invocation */
#include "AAA/Global/GlobalIpAclWorker.h"
#include "AAA/Global/AAASequencerContexts.h"
#include "AAA/Local/AAALocalIpAclMessages.h"
#include "AAA/Local/LocalIpAclWorker.h"

namespace DcmNs
{

    SsmLocalIpAclWorker::SsmLocalIpAclWorker (SsmLocalObjectManager *pSsmLocalObjectManager)
        : WaveWorker  (pSsmLocalObjectManager)
    {
        addOperationMap (SSMLOCALSERVICEASSOCIPRULETOPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalIpAclWorker::SsmLocalServiceAssocIpRuleToPolicyMessageHandler));
        addOperationMap (SSMLOCALSERVICEADDIPACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalIpAclWorker::SsmLocalServiceAddIpAclPolicyMessageHandler));
        addOperationMap (SSMLOCALSERVICEREMIPACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalIpAclWorker::SsmLocalServiceRemIpAclPolicyMessageHandler));
		addOperationMap (SSMLOCALSERVICERESEQIPACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalIpAclWorker::SsmLocalServiceResqIpAclPolicyMessageHandler));
		addOperationMap (SSMMACIPACLLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&SsmLocalIpAclWorker::SsmMacIpAclLocalShowMessageHandler));
		addOperationMap (SSMMACIPACLLOCALCLEAR, reinterpret_cast<PrismMessageHandler> (&SsmLocalIpAclWorker::SsmMacIpAclLocalClearMessageHandler));
    }

    SsmLocalIpAclWorker::~SsmLocalIpAclWorker ()
    {
    }

    PrismMessage  *SsmLocalIpAclWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SSMLOCALSERVICEASSOCIPRULETOPOLICY :
                pPrismMessage = new SsmLocalServiceAssocIpRuleToPolicyMessage ();
                break;
            case SSMLOCALSERVICEADDIPACLPOLICY :
                pPrismMessage = new SsmLocalServiceAddIpAclPolicyMessage ();
                break;
            case SSMLOCALSERVICEREMIPACLPOLICY :
                pPrismMessage = new SsmLocalServiceRemIpAclPolicyMessage ();
                break;
			case SSMLOCALSERVICERESEQIPACLPOLICY :
				 pPrismMessage = new SsmLocalServiceResqIpAclPolicyMessage ();
				 break;
			case SSMMACIPACLLOCALSHOW :
				pPrismMessage = new SsmMacIpAclLocalShowMessage ();
				break;
            case SSMMACIPACLLOCALCLEAR :
                pPrismMessage = new SsmMacIpAclLocalClearMessage();
                break;
			default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SsmLocalIpAclWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }


    void SsmLocalIpAclWorker::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string passName = pWaveAsynchronousContextForPostBootPhase->getPassName();

        SSM_PLUG_INFO_LVL_S(string("Ssm Postboot Entered with Ip passName: ") + passName);

        if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE1") == 0) {
			SSM_PLUG_INFO_LVL_S(string("SsmLocalIpAclWorker::postboot DCM_POSTBOOT_GLOBAL_STAGE1"));
            ConfigReplayIpAclPolicy();
        } else if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE5") == 0) {
            /* Vlan and Po */
			SSM_PLUG_INFO_LVL_S(string("SsmLocalIpAclWorker::postboot DCM_POSTBOOT_GLOBAL_STAGE5"));
            ConfigReplayIpAclGlobalIntf(IF_TYPE_VL);
            ConfigReplayIpAclGlobalIntf(IF_TYPE_PO);
        } else if (passName.compare("DCM_POSTBOOT_SLOT_STAGE2") == 0) {
			SSM_PLUG_INFO_LVL_S(string("SsmLocalIpAclWorker::postboot DCM_POSTBOOT_SLOT_STAGE2"));	
			SI32 slotNum = pWaveAsynchronousContextForPostBootPhase->getSlotNum();
			ConfigReplayIpAclIntf(slotNum);
		}

        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

    void SsmLocalIpAclWorker::ConfigReplayIpAclGlobalIntf(const UI32 ifType)
    {
        string className                                        = "";
        ResourceId status                                       = WAVE_MESSAGE_ERROR;
        vector<WaveManagedObject *> *pResults                   = NULL;

        SSM_PLUG_INFO_LVL_S(string ("ConfigReplay of IP ACL Global Interface Association: ...") + ifType);

        if (ifType == IF_TYPE_PO) {
            className = PoIntfManagedObject::getClassName();
        } else if (ifType == IF_TYPE_VL) {
            className = NsmIntfSviManagedObject::getClassName();
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);

        if (ifType == IF_TYPE_VL) {
        	syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        }

        pResults = querySynchronously(&syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults                                            = pResults->size ();

            SSM_PLUG_INFO_LVL_S(string("Number of Vlan/Po interfaces: ")+ numberOfResults);

            for (UI32 i = 0; i < numberOfResults; i++) {
                SsmIpAclIntfAssocBulkMessage *pSsmIpAclIntfAssocBulkMessage   = NULL;

                if(((*pResults)[i]) == NULL) {
                    SSM_PLUG_INFO_LVL_S("****VlanMOs/PortChannel MOs ith element is NULL in SsmLocalIpAclWorker***");
                    continue;
                }
                pSsmIpAclIntfAssocBulkMessage = SsmLocalIpAclWorker::ConfigReplayIpAclGlobalIntfAssoc(ifType, ((*pResults)[i]));

                if (pSsmIpAclIntfAssocBulkMessage) {
					status = sendSynchronouslyToWaveClient ("ssm",pSsmIpAclIntfAssocBulkMessage);
                    if (status == WAVE_MESSAGE_SUCCESS) {
                        SSM_PLUG_INFO_LVL_S("Vlan/Po Interface Ip Acl configuration successful ");
                    } else {
                        SSM_PLUG_INFO_LVL_S(string("Vlan/Po Inerface Ip Acl Interface configuration failed - ") + status);
                    }
                    delete pSsmIpAclIntfAssocBulkMessage;
                    pSsmIpAclIntfAssocBulkMessage = NULL;
                } else {
					SSM_PLUG_INFO_LVL_S("SsmIpAclIntfAssocBulkMessage is NULL ");
				}
            }

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        return;
    }

    SsmIpAclIntfAssocBulkMessage *SsmLocalIpAclWorker::ConfigReplayIpAclGlobalIntfAssoc(const UI32 ifType, WaveManagedObject *pWaveManagedObject)
    {

        PoIntfManagedObject *pPoIntfManagedObject       	= NULL;
        NsmIntfSviManagedObject *pNsmIntfSviManagedObject   = NULL;
        UI64 num                                        	= 0;
        char buffer[25];
		buffer[24] 	= '\0';
		UI32 bufferSize 									= sizeof (buffer);
        ObjectId IngresspolicyObjId                     	= ObjectId::NullObjectId;
        ObjectId EgresspolicyObjId                      	= ObjectId::NullObjectId;
        UI32 Ingressfound                               	= 0;
        UI32 Egressfound                                	= 0;
        SsmIpAclIntfAssocBulkMessage *intfBulkMsg      		= NULL;

        if (ifType == IF_TYPE_PO) {
            pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>(pWaveManagedObject);
            if (pPoIntfManagedObject) {
                if ((Ingressfound = pPoIntfManagedObject->getIngressEgressIpAclPolicyObjectId
                    (IngresspolicyObjId, SSMACL_BINDING_DIRECTION_IN_E)))
					num = num + 1;
                if ((Egressfound = pPoIntfManagedObject->getIngressEgressIpAclPolicyObjectId
                    (EgresspolicyObjId, SSMACL_BINDING_DIRECTION_OUT_E)))
					num = num + 1;

                snprintf(buffer, bufferSize, "po%d", pPoIntfManagedObject->getId());
            }
        } else if (ifType == IF_TYPE_VL) {
            pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>(pWaveManagedObject);
            if (pNsmIntfSviManagedObject) {
                if ((Ingressfound = pNsmIntfSviManagedObject->getIngressEgressIpAclPolicyObjectId
                    (IngresspolicyObjId, SSMACL_BINDING_DIRECTION_IN_E)))
					num = num + 1;
                if ((Egressfound = pNsmIntfSviManagedObject->getIngressEgressIpAclPolicyObjectId
                    (EgresspolicyObjId, SSMACL_BINDING_DIRECTION_OUT_E)))
					num = num + 1;

                snprintf(buffer, bufferSize, "vlan0.%d", pNsmIntfSviManagedObject->getIfId());
            }
        }

		if(Egressfound) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS)){
				WaveNs::trace(TRACE_LEVEL_INFO, "Applying ACL policy in egress direction not supported in this platform");
				Egressfound = 0;
				num = num-1;
			}
		}

        if ((Ingressfound) || (Egressfound)) {
            string IngresspolicyName    = "";
            string EgresspolicyName     = "";
            string ifname               = buffer;
            intfBulkMsg                 = new SsmIpAclIntfAssocBulkMessage();

            if ((Ingressfound) && (Egressfound)) {
                IngresspolicyName   = L3PostBootUtil::getPolicyNameByObjId(IngresspolicyObjId);
                EgresspolicyName    = L3PostBootUtil::getPolicyNameByObjId(EgresspolicyObjId);
                intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
                   SSMACL_BINDING_DIRECTION_IN_E);
                intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
                    SSMACL_BINDING_DIRECTION_OUT_E);
            } else if (Ingressfound) {
                IngresspolicyName   = L3PostBootUtil::getPolicyNameByObjId(IngresspolicyObjId);
                intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
                    SSMACL_BINDING_DIRECTION_IN_E);
            } else if (Egressfound) {
                EgresspolicyName    = L3PostBootUtil::getPolicyNameByObjId(EgresspolicyObjId);
                intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L3_IPV4_ACL,
                    SSMACL_BINDING_DIRECTION_OUT_E);
            }
            intfBulkMsg->setNumRecords(num);
        }
        return (intfBulkMsg);
    }

    void SsmLocalIpAclWorker::ConfigReplayIpAclIntf(SI32 slotNum)
    {

        SSM_PLUG_INFO_LVL("Replaying Ip Acl Interface Association:Binding....");

		if (slotNum >= 0) {
			vector<PhyIntfLocalManagedObject*> phyMOs;
			NsmUtils::SlotGetPhyIntfMOs(phyMOs, slotNum, FrameworkToolKit::getThisLocationId());
			UI32 size 	= phyMOs.size();
			UI32 i 		= 0;
			
			for (i = 0; i < size; i++) {
                
				SsmIpAclIntfAssocBulkMessage *pSsmIpAclIntfAssocBulkMessage = NULL; 
				
				if (phyMOs[i] == NULL) {
					SSM_PLUG_INFO_LVL_S("*** phyMOs ith element is NULL");
					continue;
				}

                if(phyMOs[i]->getPortrole() == false) {
					pSsmIpAclIntfAssocBulkMessage = L3PostBootUtil::ConfigReplayIpAclIntfAssoc(phyMOs[i]);
					if (pSsmIpAclIntfAssocBulkMessage) {
						ResourceId status = WAVE_MESSAGE_ERROR;
						status = sendSynchronouslyToWaveClient ("ssm", pSsmIpAclIntfAssocBulkMessage);
						if (status == WAVE_MESSAGE_SUCCESS) {
							SSM_PLUG_INFO_LVL_S("Phy Interface Ip Acl configuration successful ");
						} else {
							SSM_PLUG_INFO_LVL_S(string("Phy Inerface Ip Acl Interface configuration failed - ") + status);
						}
						
						delete pSsmIpAclIntfAssocBulkMessage;
						pSsmIpAclIntfAssocBulkMessage = NULL;
					} else {
						//SSM_PLUG_INFO_LVL_S("SsmLocalIpAclWorker::ConfigReplayIpAclIntf SsmIpAclIntfAssocBulkMessage is NULL ");
					}
				}
			}
            
			for (i = 0; i < size; i++) {
                //SSM_PLUG_INFO_LVL_S(string ("*** SsmLocalIpAclWorker:: Deleting phyMOs ") + i);
				if(phyMOs[i])
					delete phyMOs[i];
            }
		}
		return;
	}
	
    void SsmLocalIpAclWorker::ConfigReplayIpAclPolicy()
    {
        SSM_PLUG_INFO_LVL("Replaying Ip Acl Standard Access List....");
        ConfigReplayStdIpAclPolicy();
        SSM_PLUG_INFO_LVL("Replaying Ip Acl Extended Access List....");
        ConfigReplayExtIpAclPolicy();
    }

    void SsmLocalIpAclWorker::ConfigReplayStdIpAclPolicy()
    {
        StdIpAclPolicyManagedObject *stdPolicyMO   	= NULL;
        ResourceId status                           = WAVE_MESSAGE_ERROR;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(StdIpAclPolicyManagedObject::getClassName());
        vector<WaveManagedObject *> *pResultsPMO    = querySynchronously (&syncQueryCtxt);

        if (NULL != pResultsPMO)
        {
            UI32 numberOfResults = pResultsPMO->size ();
            SSM_PLUG_DBG_LVL_S(string("Number of Ip Acl Standard Policies: ") + numberOfResults);
            for (UI32 i = 0; i < numberOfResults; i++)
            {
                stdPolicyMO = dynamic_cast<StdIpAclPolicyManagedObject *>((*pResultsPMO)[i]);
                vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > stdRuleMoVector =  stdPolicyMO->getRules();
                UI64 numRules = stdRuleMoVector.size();
                SSM_PLUG_DBG_LVL_S(string("Number of Ip Acl Standard Rules: ") + numRules);
                SsmIpAclPolicyBulkMessage *aclPolicyBulkMsg = new SsmIpAclPolicyBulkMessage();
                if (aclPolicyBulkMsg)
                {
                    aclPolicyBulkMsg->setPolicyType(SSMACL_TYPE_L3_IPV4_STD_E);
                    aclPolicyBulkMsg->setOpCode(SSMACL_DCMD_CREATE_L3_IPV4_ACL);
                    aclPolicyBulkMsg->setPolicyName(stdPolicyMO->getPolicyName());
                    aclPolicyBulkMsg->setNumRules(numRules);

                    for (UI64 rindex = 0; rindex <  numRules; rindex++)
                    {
                        UI64 ruleid             		= stdRuleMoVector[rindex]->getRuleid();
                        UI32 action             		= stdRuleMoVector[rindex]->getAction();
                        bool count              		= stdRuleMoVector[rindex]->getIsCount();
                        bool log                		= stdRuleMoVector[rindex]->getIsLog();
                        UI32 srcType         			= stdRuleMoVector[rindex]->getSrcType();
						string srcUnion					= stdRuleMoVector[rindex]->getSrcUnion();
						IpV4Address srcIpAddress 		= stdRuleMoVector[rindex]->getSrcIpAddress();
						IpV4Address srcIpAddressMask 	= stdRuleMoVector[rindex]->getSrcIpAddressMask();
                        IpV4Address srcHostIpAddress 	= stdRuleMoVector[rindex]->getSrcHostIpAddress();
						UI32	 userEnteredSeqid 		= 1;
						
                        aclPolicyBulkMsg->setStdRuleParams(userEnteredSeqid, ruleid, action, srcUnion, 
							srcType, srcIpAddress, srcIpAddressMask, srcHostIpAddress, count, log);
						
                    }
                    status = sendSynchronouslyToWaveClient ("ssm",aclPolicyBulkMsg);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        SSM_PLUG_SUCCESS_LVL_S(string("Successfully configured stdIpPolicy: ") + stdPolicyMO->getPolicyName());
                    }
                    else
                    {
                        SSM_PLUG_ERR_LVL_S(string("Failed to configure stdIpPolicy: ") + stdPolicyMO->getPolicyName());
                    }
                    delete aclPolicyBulkMsg;
                }
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        }
    }

    void SsmLocalIpAclWorker::ConfigReplayExtIpAclPolicy()
    {
        ExtIpAclPolicyManagedObject *extPolicyMO = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;

        UI32 queryOffset = 0;
        UI32 queryLimit = 5;

        bool cntFlag = true;

        while ( cntFlag )
        {
            WaveManagedObjectSynchronousQueryContext extSyncQueryCtxt(ExtIpAclPolicyManagedObject::getClassName());
            extSyncQueryCtxt.addOrderField("policyName");

            extSyncQueryCtxt.addLimitForQueryResults (queryLimit, queryOffset);
            vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&extSyncQueryCtxt);

            if (NULL != pResultsPMO)
            {
                UI32 numberOfResults = pResultsPMO->size ();

                if( numberOfResults < 5 )
                {
                	cntFlag = false;
                }
                else
                {
                	queryOffset = queryOffset + queryLimit;
                }

                for (UI32 i = 0; i < numberOfResults; i++)
                {
                    extPolicyMO = dynamic_cast<ExtIpAclPolicyManagedObject *>((*pResultsPMO)[i]);
                    vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > extRuleMoVector =  extPolicyMO->getRules();
                    UI64 numRules = extRuleMoVector.size();
                    SSM_PLUG_DBG_LVL_S(string("Number of Ip Acl Extended Rules: ") + numRules);

    				SsmIpAclPolicyBulkMessage *aclPolicyBulkMsg = new SsmIpAclPolicyBulkMessage();

    				if (aclPolicyBulkMsg)
                    {
                        aclPolicyBulkMsg->setPolicyType(SSMACL_TYPE_L3_IPV4_EXT_E);
                        aclPolicyBulkMsg->setOpCode(SSMACL_DCMD_CREATE_L3_IPV4_ACL);
                        aclPolicyBulkMsg->setPolicyName(extPolicyMO->getPolicyName());
                        aclPolicyBulkMsg->setNumRules(numRules);

                        for (UI64 rindex = 0; rindex <  numRules; rindex++)
                        {
                            UI64 ruleid             			  = extRuleMoVector[rindex]->getRuleid();
                            UI32 action           		  		  = extRuleMoVector[rindex]->getAction();
                            bool count              			  = extRuleMoVector[rindex]->getIsCount();
                            bool log               		 		  = extRuleMoVector[rindex]->getIsLog();
                            UI32 srcType         			  	  = extRuleMoVector[rindex]->getSrcType();
    						string srcUnion						  = extRuleMoVector[rindex]->getSrcUnion();
    						IpV4Address srcIpAddress 			  = extRuleMoVector[rindex]->getSrcIpAddress();
    						IpV4Address srcIpAddressMask 		  = extRuleMoVector[rindex]->getSrcIpAddressMask();
                            IpV4Address srcHostIpAddress 		  = extRuleMoVector[rindex]->getSrcHostIpAddress();
                            UI32 dstType         			  	  = extRuleMoVector[rindex]->getDstType();
    						string dstUnion					      = extRuleMoVector[rindex]->getDstUnion();
    						IpV4Address dstIpAddress 			  = extRuleMoVector[rindex]->getDstIpAddress();
    						IpV4Address dstIpAddressMask 		  = extRuleMoVector[rindex]->getDstIpAddressMask();
                            IpV4Address dstHostIpAddress 		  = extRuleMoVector[rindex]->getDstHostIpAddress();
    						string ProtocolTypeUnion              = extRuleMoVector[rindex]->getProtocolTypeUnion();
    						UI32 l3l4CustomProtoType              = extRuleMoVector[rindex]->getL3l4CustomProtoType();
    						UI32 l3l4WellknownProtoType           = extRuleMoVector[rindex]->getL3l4WellknownProtoType();
    						UI32 srcPortOperator                  = extRuleMoVector[rindex]->getSrcPortOperator();
    						UI32 dstPortOperator                  = extRuleMoVector[rindex]->getDstPortOperator();
    						string srcPortNumberTcpUnion          = extRuleMoVector[rindex]->getSrcPortNumberTcpUnion();
    						string srcPortltNumberTcpUnion          = extRuleMoVector[rindex]->getSrcPortltNumberTcpUnion();
    						string srcPortgtNumberTcpUnion          = extRuleMoVector[rindex]->getSrcPortgtNumberTcpUnion();

    						string dstPortNumberTcpUnion          = extRuleMoVector[rindex]->getDstPortNumberTcpUnion();
    						string dstPortltNumberTcpUnion          = extRuleMoVector[rindex]->getDstPortltNumberTcpUnion();
    						string dstPortgtNumberTcpUnion          = extRuleMoVector[rindex]->getDstPortgtNumberTcpUnion();
    						string srcPortNumberUdpUnion          = extRuleMoVector[rindex]->getSrcPortNumberUdpUnion();
    						string srcPortltNumberUdpUnion          = extRuleMoVector[rindex]->getSrcPortltNumberUdpUnion();
    						string srcPortgtNumberUdpUnion          = extRuleMoVector[rindex]->getSrcPortgtNumberUdpUnion();
    						string dstPortNumberUdpUnion          = extRuleMoVector[rindex]->getDstPortNumberUdpUnion();
    						string dstPortltNumberUdpUnion          = extRuleMoVector[rindex]->getDstPortltNumberUdpUnion();
    						string dstPortgtNumberUdpUnion          = extRuleMoVector[rindex]->getDstPortgtNumberUdpUnion();
    						string srcPortrangeLowerTcpUnion      = extRuleMoVector[rindex]->getSrcPortrangeLowerTcpUnion();
    						string dstPortrangeLowerTcpUnion      = extRuleMoVector[rindex]->getDstPortrangeLowerTcpUnion();
    						string srcPortrangeHigherTcpUnion     = extRuleMoVector[rindex]->getSrcPortrangeHigherTcpUnion();
    						string dstPortrangeHigherTcpUnion     = extRuleMoVector[rindex]->getDstPortrangeHigherTcpUnion();
    						string srcPortrangeLowerUdpUnion      = extRuleMoVector[rindex]->getSrcPortrangeLowerUdpUnion();
    						string dstPortrangeLowerUdpUnion      = extRuleMoVector[rindex]->getDstPortrangeLowerUdpUnion();
    						string srcPortrangeHigherUdpUnion     = extRuleMoVector[rindex]->getSrcPortrangeHigherUdpUnion();
    						string dstPortrangeHigherUdpUnion     = extRuleMoVector[rindex]->getDstPortrangeHigherUdpUnion();
    						UI32 srcPortCustomNumberTcp           = extRuleMoVector[rindex]->getSrcPortCustomNumberTcp();
    						UI32 srcPortltCustomNumberTcp           = extRuleMoVector[rindex]->getSrcPortltCustomNumberTcp();
    						UI32 srcPortgtCustomNumberTcp           = extRuleMoVector[rindex]->getSrcPortgtCustomNumberTcp();
    						UI32 dstPortCustomNumberTcp           = extRuleMoVector[rindex]->getDstPortCustomNumberTcp();
    						UI32 dstPortltCustomNumberTcp           = extRuleMoVector[rindex]->getDstPortltCustomNumberTcp();
    						UI32 dstPortgtCustomNumberTcp           = extRuleMoVector[rindex]->getDstPortgtCustomNumberTcp();
    						UI32 srcPortCustomNumberUdp           = extRuleMoVector[rindex]->getSrcPortCustomNumberUdp();
    						UI32 srcPortltCustomNumberUdp           = extRuleMoVector[rindex]->getSrcPortgtCustomNumberUdp();
    						UI32 srcPortgtCustomNumberUdp           = extRuleMoVector[rindex]->getSrcPortltCustomNumberUdp();
    						UI32 dstPortCustomNumberUdp           = extRuleMoVector[rindex]->getDstPortCustomNumberUdp();
    						UI32 dstPortltCustomNumberUdp           = extRuleMoVector[rindex]->getDstPortltCustomNumberUdp();
    						UI32 dstPortgtCustomNumberUdp           = extRuleMoVector[rindex]->getDstPortgtCustomNumberUdp();
    						UI32 srcPortWellknownNumberTcp        = extRuleMoVector[rindex]->getSrcPortWellknownNumberTcp();
    						UI32 srcPortltWellknownNumberTcp        = extRuleMoVector[rindex]->getSrcPortltWellknownNumberTcp();
    						UI32 srcPortgtWellknownNumberTcp        = extRuleMoVector[rindex]->getSrcPortgtWellknownNumberTcp();
    						UI32 dstPortWellknownNumberTcp        = extRuleMoVector[rindex]->getDstPortWellknownNumberTcp();
    						UI32 dstPortltWellknownNumberTcp        = extRuleMoVector[rindex]->getDstPortltWellknownNumberTcp();
    						UI32 dstPortgtWellknownNumberTcp        = extRuleMoVector[rindex]->getDstPortgtWellknownNumberTcp();
    						UI32 srcPortWellknownNumberUdp        = extRuleMoVector[rindex]->getSrcPortWellknownNumberUdp();
    						UI32 srcPortltWellknownNumberUdp        = extRuleMoVector[rindex]->getSrcPortltWellknownNumberUdp();
    						UI32 srcPortgtWellknownNumberUdp        = extRuleMoVector[rindex]->getSrcPortgtWellknownNumberUdp();
    						UI32 dstPortWellknownNumberUdp        = extRuleMoVector[rindex]->getDstPortWellknownNumberUdp();
    						UI32 dstPortltWellknownNumberUdp        = extRuleMoVector[rindex]->getDstPortltWellknownNumberUdp();
    						UI32 dstPortgtWellknownNumberUdp        = extRuleMoVector[rindex]->getDstPortgtWellknownNumberUdp();
    						UI32 srcPortCustomrangeLowerTcp       = extRuleMoVector[rindex]->getSrcPortCustomrangeLowerTcp();
    						UI32 dstPortCustomrangeLowerTcp       = extRuleMoVector[rindex]->getDstPortCustomrangeLowerTcp();
    						UI32 srcPortCustomrangeLowerUdp       = extRuleMoVector[rindex]->getSrcPortCustomrangeLowerUdp();
    						UI32 dstPortCustomrangeLowerUdp       = extRuleMoVector[rindex]->getDstPortCustomrangeLowerUdp();
    						UI32 srcPortCustomrangeHigherTcp      = extRuleMoVector[rindex]->getSrcPortCustomrangeHigherTcp();
    						UI32 dstPortCustomrangeHigherTcp      = extRuleMoVector[rindex]->getDstPortCustomrangeHigherTcp();
    						UI32 srcPortCustomrangeHigherUdp      = extRuleMoVector[rindex]->getSrcPortCustomrangeHigherUdp();
    						UI32 dstPortCustomrangeHigherUdp      = extRuleMoVector[rindex]->getDstPortCustomrangeHigherUdp();
    						UI32 srcPortWellknownrangeLowerUdp    = extRuleMoVector[rindex]->getSrcPortWellknownrangeLowerUdp();
    						UI32 dstPortWellknownrangeLowerUdp    = extRuleMoVector[rindex]->getDstPortWellknownrangeLowerUdp();
    						UI32 srcPortWellknownrangeLowerTcp    = extRuleMoVector[rindex]->getSrcPortWellknownrangeLowerTcp();
    						UI32 dstPortWellknownrangeLowerTcp    = extRuleMoVector[rindex]->getDstPortWellknownrangeLowerTcp();
    						UI32 srcPortWellknownrangeHigherUdp   = extRuleMoVector[rindex]->getSrcPortWellknownrangeHigherUdp();
    						UI32 dstPortWellknownrangeHigherUdp   = extRuleMoVector[rindex]->getDstPortWellknownrangeHigherUdp();
    						UI32 srcPortWellknownrangeHigherTcp   = extRuleMoVector[rindex]->getSrcPortWellknownrangeHigherTcp();
    						UI32 dstPortWellknownrangeHigherTcp   = extRuleMoVector[rindex]->getDstPortWellknownrangeHigherTcp();
    						WellknownPrecedence wellknownPrecedence = extRuleMoVector[rindex]->getWellknownPrecedence();
    						WellknownTos wellknownTos               = extRuleMoVector[rindex]->getWellknownTos();
    						WellknownDscp wellknownDscp             = extRuleMoVector[rindex]->getWellknownDscp();
    						UI32  customPrecedence                  = extRuleMoVector[rindex]->getCustomPrecedence();
    						UI32  customTos                         = extRuleMoVector[rindex]->getCustomTos();
    						UI32  customDscp                        = extRuleMoVector[rindex]->getCustomDscp();
    						WaveUnion precedenceUnion               = extRuleMoVector[rindex]->getPrecedenceUnion();
    						WaveUnion dscpUnion                     = extRuleMoVector[rindex]->getDscpUnion();
    						WaveUnion tosUnion                      = extRuleMoVector[rindex]->getTosUnion();
    						bool urg                              	= extRuleMoVector[rindex]->getUrg();
    						bool ack                              	= extRuleMoVector[rindex]->getAck();
    						bool push                              	= extRuleMoVector[rindex]->getPush();
    						bool fin                              	= extRuleMoVector[rindex]->getFin();
    						bool rst                              	= extRuleMoVector[rindex]->getRst();
    						bool sync                             	= extRuleMoVector[rindex]->getSync();
    						UI32  userEnteredSeqid           	  	= 1;

                            aclPolicyBulkMsg->setExtRuleParams(userEnteredSeqid, ruleid, action, srcUnion,
    							srcType, srcIpAddress, srcIpAddressMask, srcHostIpAddress, count, log,
    							dstUnion, dstType, dstIpAddress, dstIpAddressMask, dstHostIpAddress,ProtocolTypeUnion,
    							l3l4CustomProtoType, l3l4WellknownProtoType, srcPortOperator, srcPortNumberTcpUnion,
    							srcPortltNumberTcpUnion, srcPortgtNumberTcpUnion,
    							srcPortNumberUdpUnion,
    							srcPortltNumberUdpUnion,
    							srcPortgtNumberUdpUnion,
    							srcPortrangeLowerTcpUnion, srcPortrangeLowerUdpUnion,
    							srcPortrangeHigherTcpUnion, srcPortrangeHigherUdpUnion, srcPortCustomNumberTcp,
    							srcPortltCustomNumberTcp, srcPortgtCustomNumberTcp,
    							srcPortWellknownNumberTcp, srcPortltWellknownNumberTcp, srcPortgtWellknownNumberTcp,
    							srcPortCustomNumberUdp, srcPortltCustomNumberUdp, srcPortgtCustomNumberUdp,
    							srcPortWellknownNumberUdp, srcPortltWellknownNumberUdp, srcPortgtWellknownNumberUdp,
    							srcPortCustomrangeLowerTcp, srcPortWellknownrangeLowerTcp, srcPortCustomrangeHigherTcp,
    							srcPortWellknownrangeHigherTcp, srcPortCustomrangeLowerUdp, srcPortWellknownrangeLowerUdp,
    							srcPortCustomrangeHigherUdp, srcPortWellknownrangeHigherUdp,
    							dstPortNumberTcpUnion, dstPortltNumberTcpUnion, dstPortgtNumberTcpUnion,
    							dstPortNumberUdpUnion, dstPortltNumberUdpUnion, dstPortgtNumberUdpUnion,
    							dstPortrangeLowerTcpUnion, dstPortrangeLowerUdpUnion,
    							dstPortrangeHigherTcpUnion, dstPortrangeHigherUdpUnion,	dstPortOperator,
    							dstPortCustomNumberTcp,
    							dstPortltCustomNumberTcp,
    							dstPortgtCustomNumberTcp,
    							dstPortWellknownNumberTcp, dstPortltWellknownNumberTcp, dstPortgtWellknownNumberTcp,
    							dstPortCustomNumberUdp, dstPortltCustomNumberUdp, dstPortgtCustomNumberUdp,
    							dstPortWellknownNumberUdp,
    							dstPortltWellknownNumberUdp,
    							dstPortgtWellknownNumberUdp,
    							dstPortCustomrangeLowerTcp, dstPortWellknownrangeLowerTcp,
    							dstPortCustomrangeHigherTcp, dstPortWellknownrangeHigherTcp, dstPortCustomrangeLowerUdp,
    							dstPortWellknownrangeLowerUdp, dstPortCustomrangeHigherUdp, dstPortWellknownrangeHigherUdp,
    							wellknownPrecedence, wellknownTos, wellknownDscp, customPrecedence, customTos,
    							customDscp, precedenceUnion, tosUnion, dscpUnion, urg, ack, push, fin, rst, sync);
                        }

                        status = sendSynchronouslyToWaveClient ("ssm",aclPolicyBulkMsg);
                        if (status == WAVE_MESSAGE_SUCCESS)
                        {
                            SSM_PLUG_SUCCESS_LVL_S(string("Successfully configured extIpPolicy: ")  + extPolicyMO->getPolicyName());
                        }
                        else
                        {
                            SSM_PLUG_ERR_LVL_S(string("Failed to configure extIpPolicy: ")  + extPolicyMO->getPolicyName());
                        }
                        delete aclPolicyBulkMsg;
                    }
                }
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
            }
            else
            {
            	break;
            }
        }
    }
	
    void SsmLocalIpAclWorker::SsmMacIpAclLocalShowMessageHandler (SsmMacIpAclLocalShowMessage *pSsmMacIpAclLocalShowMessage)
    {
        if (pSsmMacIpAclLocalShowMessage->getIsGwRequest())
        {
             PrismLinearSequencerStep sequencerSteps[] =
             {
                  reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalIpAclWorker::sendShowMacIpCommandToCluster),
                  reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalIpAclWorker::prismLinearSequencerSucceededStep),
                  reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalIpAclWorker::prismLinearSequencerFailedStep)
             };

             PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext
			 (pSsmMacIpAclLocalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
             pPrismLinearSequencerContext->start ();
             return;
        }
        else
        {
             PrismSynchronousLinearSequencerStep sequencerSteps[] =
             {
                 reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalIpAclWorker::sendShowMacIpToSsmDaemon),
                 reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalIpAclWorker::prismSynchronousLinearSequencerSucceededStep),
                 reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalIpAclWorker::prismSynchronousLinearSequencerFailedStep)
             };
             PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pSsmMacIpAclLocalShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
             pPrismSynchronousLinearSequencerContext->execute ();
        }
    }

    void SsmLocalIpAclWorker::setLocationIdsForActiveVlanId(const ObjectId vlanObjectId, UI32 vlanId, vector<LocationId>& locationIds)
    {
        set<LocationId> tmpIds;
        ResourceId status = WAVE_MESSAGE_ERROR;
        status = DcmToolKit::isVcsEnabled ();

        if (WAVE_PERSISTENCE_CLUSTER_ENABLED != status)
        {
            locationIds.push_back(FrameworkToolKit::getThisLocationId ());
        }
        else if(WAVE_PERSISTENCE_CLUSTER_ENABLED == status)
        {
        	SI32 mappedId = -1;
        	LocationId locationId = 0;

        	WaveManagedObjectSynchronousQueryContext queryCtx(PhyIntfLocalManagedObject::getClassName());

        	queryCtx.createConditionSet ("ACCESS_VLAN_CHECK");

        	queryCtx.addAttributeToConditionSet ("ACCESS_VLAN_CHECK", new AttributeObjectIdAssociation(vlanObjectId, "vlanObjectId",
        			VlanIntfManagedObject::getClassName()), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        	queryCtx.createConditionSet ("NO_VLAN_REMOVE_RANGE");

        	queryCtx.addAttributeToConditionSet("NO_VLAN_REMOVE_RANGE",new AttributeBool(true,"isAllowedVlanAll"),
        			WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        	UI32Range vlanRangeRemove;

        	queryCtx.addAttributeToConditionSet("NO_VLAN_REMOVE_RANGE",new AttributeUI32Range(&vlanRangeRemove, "vlanRangeRemove"),
        			WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        	queryCtx.combineConditionSets ("ACCESS_VLAN_CHECK+NO_VLAN_REMOVE_RANGE");

        	queryCtx.addSelectField("id");

        	vector<WaveManagedObject *> *pResults = querySynchronously(&queryCtx);

        	if (NULL != pResults)
        	{
        		UI32 numberOfResults = pResults->size ();

        		for (UI32 i = 0; i < numberOfResults; i++)
        		{
        			PhyIntfLocalManagedObject* phyIntfMO = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);

        			mappedId = NsmUtils::getMappedIdFromIfName(phyIntfMO->getId());
        			locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

        			if(tmpIds.insert(locationId).second) {
        				locationIds.push_back(locationId);
        			}
        		}

        		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        	}


        	// The command is "switchport trunk allowed vlan add 2-9"
        	// If the vlan exists in any vlanRangeAdd, then return error.

        	UI32Range nullRange;
        	PoPhyIntfManagedObject *pPoPhyIntfManagedObject = NULL;

        	WaveManagedObjectSynchronousQueryContext tmpCtxt(PoPhyIntfManagedObject::getClassName());
        	tmpCtxt.setLoadOneToManyRelationships(false);
        	tmpCtxt.setLoadCompositions(false);
        	tmpCtxt.addSelectField("ifName");
        	tmpCtxt.addSelectField("vlanRangeAdd");
        	tmpCtxt.addAndAttribute (new AttributeUI32Range(&nullRange, "vlanRangeAdd"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

        	vector<WaveManagedObject *> *pTmpResults = querySynchronously(&tmpCtxt);

        	if (NULL != pTmpResults)
        	{
        		UI32 tmpResultsSize = pTmpResults->size ();

        		for (UI32 idx = 0; idx < tmpResultsSize; idx++)
        		{
        			pPoPhyIntfManagedObject = dynamic_cast<PoPhyIntfManagedObject *>((*pTmpResults)[idx]);
        			vector <UI32> vlanRangeVector;
        			pPoPhyIntfManagedObject->getVlanRangeAdd().getUI32RangeVector(vlanRangeVector);
        			vector <UI32>::iterator it = find(vlanRangeVector.begin(), vlanRangeVector.end(), vlanId);
        			if(it != vlanRangeVector.end())
        			{
        				mappedId = NsmUtils::getMappedIdFromIfName(pPoPhyIntfManagedObject->getIfName());
        				locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

        				if(tmpIds.insert(locationId).second) {
        					locationIds.push_back(locationId);
        				}
        			}
        		}

        		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pTmpResults);
        	}

        	//The command is
        	//"switchport trunk allowed vlan all"
        	//"switchport trunk allowed vlan remove 3"
        	//If the vlan doesn't exist in vlanRangeRemove, then return error

        	WaveManagedObjectSynchronousQueryContext tmpCtxt2(PoPhyIntfManagedObject::getClassName());
        	tmpCtxt2.addSelectField("ifName");
        	tmpCtxt2.addSelectField("vlanRangeRemove");
        	tmpCtxt2.addAndAttribute (new AttributeBool(true, "isAllowedVlanAll"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        	tmpCtxt2.addAndAttribute (new AttributeUI32Range(&nullRange, "vlanRangeRemove"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

        	tmpCtxt2.setLoadOneToManyRelationships(false);
        	tmpCtxt2.setLoadCompositions(false);

        	vector<WaveManagedObject *> *pTmpResults2 = querySynchronously(&tmpCtxt2);

        	if (NULL != pTmpResults2)
        	{
        		UI32 tmpResultsSize2 = pTmpResults2->size ();

        		for (UI32 idx1 = 0; idx1 < tmpResultsSize2; idx1++)
        		{
        			pPoPhyIntfManagedObject = dynamic_cast<PoPhyIntfManagedObject *>((*pTmpResults2)[idx1]);

        			vector <UI32> vlanRangeVector;
        			pPoPhyIntfManagedObject->getVlanRangeRemove().getUI32RangeVector(vlanRangeVector);
        			vector <UI32>::iterator it = find(vlanRangeVector.begin(), vlanRangeVector.end(), vlanId);
        			if(it == vlanRangeVector.end())
        			{
        				mappedId = NsmUtils::getMappedIdFromIfName(pPoPhyIntfManagedObject->getIfName());
        				locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

        				if(tmpIds.insert(locationId).second) {
        					locationIds.push_back(locationId);
        				}
        			}
        		}

        		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pTmpResults2);
        	}
        }
    }

    void SsmLocalIpAclWorker::sendShowMacIpCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
        SsmMacIpAclLocalShowMessage        	*msg_p 		= NULL;
        SsmMacIpAclLocalShowMessage        	*loc_msg_p 	= NULL;
        WaveSendToClusterContext          	*send_ctx_p = NULL;
        UI32                              	locationId 	= 0;
        SI32                              	mappedId 	= 0;
        vector<UI32>                      	locations;
        bool                              	sendToLocal = false;

        msg_p = dynamic_cast<SsmMacIpAclLocalShowMessage *>(seq_ctx_p->getPPrismMessage ());
        
		SSM_PLUG_ASSERT_PTR(msg_p);

        loc_msg_p = new SsmMacIpAclLocalShowMessage();
        for (UI32 i = 0; i < locations.size(); i++) 
		{
            if (i ==0) {
		        SSM_PLUG_INFO_LVL_S(string("locations:") + locations[i]);
		    } else {
		        SSM_PLUG_INFO_LVL_S(string(":") + locations[i]);
		    }
		}
				
        loc_msg_p->setIfName(msg_p->getIfName());
        loc_msg_p->setPolicyName(msg_p->getPolicyName());
        loc_msg_p->setIfType(msg_p->getIfType());
        loc_msg_p->setOpCode(msg_p->getOpCode());
        loc_msg_p->setIsGwRequest(false);
        loc_msg_p->setAclType(msg_p->getAclType());
        loc_msg_p->setDirection(msg_p->getDirection());
        loc_msg_p->setBindOid(msg_p->getBindOid());
		
		SSM_PLUG_ASSERT_PTR(loc_msg_p);
        
        SSM_PLUG_INFO_LVL_S(string("Sending Show Access-list Command to Cluster"));
        SSM_PLUG_INFO_LVL_S(string("    OpCode =") + msg_p->getOpCode() +
                            string("    PolicyName =")  + msg_p->getPolicyName() +
                            string("    ifName =") + msg_p->getIfName() +
                            string("    ifType is=" ) + msg_p->getIfType() +
                            string("    Direction is =" ) + msg_p->getDirection() +
                            string("    BindOid is =" ) + msg_p->getBindOid() +
                            string("    AclType is =") + msg_p->getAclType()
                            );

        /* Check Validity of mapped id before sending any msg */
        if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI) 
			|| (msg_p->getIfType() == IF_TYPE_FO))
        {
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))
                sendToLocal = true;
            else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))
            {
                /* Send to the targetted LocationId pointed to by the mappedId. */
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
                if (!locationId)
                {
                    /* Fix Me : For Temporary purpose this Error will change it */
					msg_p->setCompletionStatus (SSM_ACL_CLEAR_STATS_RC_NO_IF);
                    seq_ctx_p->executeNextStep (SSM_ACL_CLEAR_STATS_RC_NO_IF);
                    delete loc_msg_p;
                    return;
                }
                else
                    locations.push_back(locationId);
            }
        }

        send_ctx_p = new WaveSendToClusterContext(this,reinterpret_cast
			<PrismAsynchronousCallback>(&SsmLocalIpAclWorker::
			SsmMacIpAclLocalShowSendToClusterCallback), seq_ctx_p);
        SSM_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p)
            return;

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        UI32 opCode = msg_p->getOpCode();

        if (0 != msg_p->getIfName().length())
        {
            if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI) 
				|| (msg_p->getIfType() == IF_TYPE_FO))
            {
                if (!sendToLocal)
                    send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
            else if (msg_p->getIfType () == IF_TYPE_PO)
            {
            	 // needs to find out the phy. interface which is a member of the po interface, and then send the request
                 // to the node of the phy. interface, to do the aggregation for the rule status of the po.

                 NsmUtils::setLocationIdsForPoSubModeCmds(msg_p->getIfName(),send_ctx_p);

                 if(send_ctx_p->getLocationsToSendToForPhase1().size() == 0) {
                	 vector<UI32> locationIds;
                	 locationIds.push_back(FrameworkToolKit::getThisLocationId ());
                	 send_ctx_p->setLocationsToSendToForPhase1(locationIds);
                 }
            }
            else if(msg_p->getIfType () == IF_TYPE_VL)
            {
                // needs to find out the phy. interface which is a member of the vlan, and then send the request
                // to the node of the phy. interface, to do the aggregation for the rule status of the vlan.
                if(opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE || opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ON_INTERFACE)
                {
                    vector<LocationId> locationIds;

                    if(msg_p->getAclType() == SSMACL_TYPE_L2_E)
                    {
                        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
                        UI32 vlanId = atol (msg_p->getIfName().c_str());
                        syncQueryCtxt.addAndAttribute(new AttributeUI32(vlanId, "id"));
                        syncQueryCtxt.setLoadOneToManyRelationships(false);
                        syncQueryCtxt.setLoadCompositions(false);
                        syncQueryCtxt.addSelectField("objectId");
                        vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);

                        if (NULL != pResults && pResults->size () > 0)
                        {
                            VlanIntfManagedObject* vlanIntfMo = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
                            setLocationIdsForActiveVlanId(vlanIntfMo->getObjectId(), vlanIntfMo->getId(), locationIds);
                        }
                    }

                    if( locationIds.size() == 0 ) {
                        locationIds.push_back(FrameworkToolKit::getThisLocationId ());
                    }

                    send_ctx_p->setLocationsToSendToForPhase1(locationIds);
                }
            } else if(msg_p->getIfType () == IF_TYPE_VE) {
            	if(opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ACL_ON_INTERFACE || opCode == SSMACL_DCMD_SHOW_ACCESSLIST_ON_INTERFACE
            			|| opCode == SSMACL_DCMD_SHOW_STATS_ON_INTERFACE || opCode == SSMACL_DCMD_SHOW_STATS_ACL_ON_INTERFACE)
            	{
					if((!strcmp(msg_p->getRbridgeId().c_str(),"all") == 0) && (!msg_p->getRbridgeId().empty()) )
					{
						UI32 rbridgeID = 0;
						stringstream tmpStream(msg_p->getRbridgeId());
						tmpStream >> rbridgeID;
						locationId = DcmToolKit::getLocationIdFromRbridgeId(rbridgeID);
		                if (!locationId)
		                {
							msg_p->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		                    seq_ctx_p->executeNextStep (WAVE_MESSAGE_SUCCESS);
		                    delete loc_msg_p;
		                    delete send_ctx_p;
		                    return;
		                }
		                locations.push_back(locationId);
					} else if(msg_p->getRbridgeId().empty()) {
						locations.push_back(FrameworkToolKit::getThisLocationId());
					}

					if(!locations.empty()) {
						send_ctx_p->setLocationsToSendToForPhase1(locations);
					}
            	}
            }
        }

        if (sendToLocal)
        {
            locationId = NsmUtils::getLocalLocationId();
            if (locationId)
               locations.push_back(locationId);
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }

        sendToWaveCluster(send_ctx_p);
        return;
    }

    void SsmLocalIpAclWorker::SsmMacIpAclLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p 	= NULL;
        SsmMacIpAclLocalShowMessage     *req_msg_p 	= NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p 	= NULL;

        seq_ctx_p 	= reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        req_msg_p 	= dynamic_cast<SsmMacIpAclLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p 	= dynamic_cast<SsmMacIpAclLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());

        prismAssert (req_msg_p != NULL, __FILE__, __LINE__);
        prismAssert (resp_msg_p != NULL, __FILE__, __LINE__);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        trace(TRACE_LEVEL_INFO, string("Response processing: Num resp:") + locations.size());
				
        for (i = 0; i < locations.size(); i++)
        {
        	trace(TRACE_LEVEL_INFO, string("locations: ") + locations[i]);
        }

        bool isAllFail = true;
        bool isRbridgeIdAll = false;

        if(strcmp(req_msg_p->getRbridgeId().c_str(),"all") == 0) {
        	isRbridgeIdAll = true;
        }

        ResourceId clusterCompletionStatus = WAVE_MESSAGE_SUCCESS;

        for (i = 0; i < locations.size(); i++)
        {
			WaveMessageStatus sendStatus = ctx_p->getSendStatusForPhase1 (locations[i]);
			if (WAVE_MESSAGE_SUCCESS == sendStatus) {
	            sw_resp_p = dynamic_cast<SsmMacIpAclLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
    	        prismAssert (sw_resp_p != NULL, __FILE__, __LINE__);
        	    if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
        	    	isAllFail = false;
            	    req_msg_p->copyAllBuffers(*sw_resp_p);
				} else {
					clusterCompletionStatus = sw_resp_p->getCompletionStatus();
					req_msg_p->setCompletionStatus(clusterCompletionStatus);
        	        trace(TRACE_LEVEL_INFO, string("Response processing: The location: ")
						+ locations[i] + string("returned error: ")
						+ FrameworkToolKit::localize(clusterCompletionStatus));
        	        if(!isRbridgeIdAll) {
        	        	break;
        	        }
				}
			}
        }

        if(isRbridgeIdAll && (!isAllFail)) {
        	req_msg_p->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            seq_ctx_p->executeNextStep(clusterCompletionStatus);
        }
        delete resp_msg_p;
        delete ctx_p;
        return;
    }
	
    ResourceId SsmLocalIpAclWorker::sendShowMacIpToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p)
    {
        SsmMacIpAclLocalShowMessage 	*msg_p 	= dynamic_cast<SsmMacIpAclLocalShowMessage *> (ctx_p->getPPrismMessage ());
        ResourceId      		status;
        SI32            		mappedId 		= 0;
        string          		ifName 			= "";

        SsmMacIpAclShowMessage *cl_msg_p 		= new SsmMacIpAclShowMessage();
        if (cl_msg_p != NULL)
        {
            char buffer[25];
			buffer[24] 		= '\0';
			UI32 bufferSize = sizeof (buffer);
            cl_msg_p->setPolicyName(msg_p->getPolicyName ());
            if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI)
				|| (msg_p->getIfType() == IF_TYPE_FO))
            {
                /* Three tuple to two tuple conversion */
                if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))
                    NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
				else 
					ifName = msg_p->getIfName();

				if (msg_p->getIfType() == IF_TYPE_TE)
                	snprintf(buffer, bufferSize, "te%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_GI)
					snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_FO)
					snprintf(buffer, bufferSize, "fo%s", ifName.c_str());
            }
            else if (msg_p->getIfType () == IF_TYPE_PO)
                snprintf(buffer, bufferSize, "po%s", msg_p->getIfName ().c_str());
            else if (msg_p->getIfType () == IF_TYPE_VL)
                snprintf(buffer, bufferSize, "vlan0.%s", msg_p->getIfName ().c_str());
            else if (msg_p->getIfType () == IF_TYPE_VE)
                snprintf(buffer, bufferSize, "ve0.%s", msg_p->getIfName ().c_str());
            else
                snprintf(buffer, bufferSize, "%s", msg_p->getIfName ().c_str());

            mappedId = DcmToolKit::getLocalMappedId();

            if (mappedId == (-1))
                mappedId = 0;

            cl_msg_p->setPolicyName(msg_p->getPolicyName ());
            cl_msg_p->setIfName(buffer);
            cl_msg_p->setOpCode(msg_p->getOpCode());
            cl_msg_p->setMappedId(mappedId);
            cl_msg_p->setAclType(msg_p->getAclType());
            cl_msg_p->setDirection(msg_p->getDirection());
            cl_msg_p->setBindOid(msg_p->getBindOid());

            trace (TRACE_LEVEL_INFO, "sendToLocalNode: Message being sent to Ssm");

            status = sendSynchronouslyToWaveClient ("ssm", cl_msg_p);

            if (status == WAVE_MESSAGE_SUCCESS)
            {
                if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                {
                    trace(TRACE_LEVEL_INFO, string("Resp: Client Completion Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                    msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }

                if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS)

                {
					trace(TRACE_LEVEL_INFO, string("cl_msg_p->getClientStatus() :") + cl_msg_p->getClientStatus());
					status = getResourceIdFromClientStatus(cl_msg_p->getClientStatus());

                    msg_p->setCompletionStatus(status);
                    trace(TRACE_LEVEL_INFO, string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(status));
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }
            }

            if (status != WAVE_MESSAGE_SUCCESS)
            {
                trace(TRACE_LEVEL_INFO, string("Client Message sending failed for MsgId: ") +  cl_msg_p->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
                if(cl_msg_p)
                    delete cl_msg_p;
                return status;
            }

            trace(TRACE_LEVEL_INFO, string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());
            msg_p->copyAllBuffers(*cl_msg_p);
            trace(TRACE_LEVEL_INFO, "sendToLocalNode: Successfully protocol daemon");

            status = cl_msg_p->getCompletionStatus();
            if(cl_msg_p)
                delete cl_msg_p;
            return status;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, "sendToLocalNode: Message creation failed");
        }
        if(cl_msg_p)
            delete cl_msg_p;
        return status;
    
	}	


    void SsmLocalIpAclWorker::SsmLocalServiceAddIpAclPolicyMessageHandler(SsmLocalServiceAddIpAclPolicyMessage *pMsg)
	{
		SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pMsg);
		const string policyName 	= pMsg->getPolicyName();
 		UI32 policyType 			= pMsg->getPolicyType();
 		ResourceId status 			= WAVE_MESSAGE_SUCCESS;

 		SsmIpAclPolicyMessage *msg = new SsmIpAclPolicyMessage(policyName, policyType, SSMACL_DCMD_CREATE_L3_IPV4_ACL);
 		if (msg != NULL) {
 			SSM_PLUG_DBG_LVL_S(string("SsmIpAclPolicyMessage message being sent to Ssm policy: ") + policyName + string ("policy type: ")
 							+ policyType);
 			status = sendSynchronouslyToWaveClient ("ssm", msg);
 			SSM_PLUG_INFO_LVL_S(string("Add ACL Policy: status - ") + status + string(",Completion status - ") + msg->getCompletionStatus ()
 								+ string(",Client status - ") + msg->getClientStatus());

 			if (status == WAVE_MESSAGE_SUCCESS)
 			{
 				if (msg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
 					status = msg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				} else if (msg->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
 					status = msg->getClientStatus();

 					if ( status == WRC_SSM_DCM_ERR_ACL_LIMIT )
 					{
 						status = getErrorStatusPerPlatform(status);
 					}

 					SSM_PLUG_ERR_LVL_S( string("Client Status:") + FrameworkToolKit::localize(status));

					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				}
			} else {
				SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
			}
 			delete msg;
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}
		pMsg->setCompletionStatus(status);	
		reply(pMsg);
	}
	
	
    void SsmLocalIpAclWorker::SsmLocalServiceRemIpAclPolicyMessageHandler(SsmLocalServiceRemIpAclPolicyMessage *pMsg)
	{
		SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pMsg);
		const string policyName 	= pMsg->getPolicyName();
 		UI32 policyType 			= pMsg->getPolicyType();
 		ResourceId status 			= WAVE_MESSAGE_SUCCESS;
		
 		SsmIpAclPolicyMessage *msg = new SsmIpAclPolicyMessage(policyName, policyType, SSMACL_DCMD_DELETE_L3_IPV4_ACL);
 		if (msg != NULL) {
 			SSM_PLUG_DBG_LVL("SsmIpAclPolicyMessage message being sent to Ssm");
 			status = sendSynchronouslyToWaveClient ("ssm", msg);
 			SSM_PLUG_INFO_LVL_S(string("Remove ACL Policy: status - ") + status + string(",Completion status - ") + msg->getCompletionStatus ()
 							+ string(",Client status - ") + msg->getClientStatus());
 			if (status == WAVE_MESSAGE_SUCCESS ) {
 				if (msg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
 					status = msg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to remove policy on the backend: ") + policyName);
				} else if (msg->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
 					status = msg->getClientStatus();
					SSM_PLUG_ERR_LVL_S(string("Failed to remove policy on the backend: ") + policyName);
				}
			} else {
				SSM_PLUG_ERR_LVL_S(string("Failed to remove policy on the backend: ") + policyName);
			}
 			delete msg;
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}
		pMsg->setCompletionStatus(status);	
		reply(pMsg);
	}

	void SsmLocalIpAclWorker::SsmLocalServiceAssocIpRuleToPolicyMessageHandler(SsmLocalServiceAssocIpRuleToPolicyMessage *pMsg)
	{

		SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pMsg);

        UI64 ruleid													= pMsg->getRuleid();
        UI32 action													= pMsg->getAction();
        UI32 policyType												= pMsg->getPolicyType();
        const string policyName										= pMsg->getPolicyName();
        IpAddrType  srcType											= pMsg->getSrcType();
        IpV4Address srcIpAddress									= pMsg->getSrcIpAddress();
        IpV4Address srcIpAddressMask								= pMsg->getSrcIpAddressMask();
        IpV4Address srcHostIpAddress								= pMsg->getSrcHostIpAddress();
        WaveUnion srcUnion = pMsg->getSrcUnion();
        bool isCount 												= pMsg->getIsCount();
        bool isLog 													= pMsg->getIsLog();
        UI32 opcode										 			= (UI32) pMsg->getOpcode();

		IpAddrType  dstType 										= pMsg->getDstType();
		IpV4Address dstIpAddress 									= pMsg->getDstIpAddress();
		IpV4Address dstIpAddressMask								= pMsg->getDstIpAddressMask();
		IpV4Address dstHostIpAddress								= pMsg->getDstHostIpAddress();
		WaveUnion dstUnion 											= pMsg->getDstUnion();
		WaveUnion ProtocolTypeUnion 								= pMsg->getProtocolTypeUnion();
		UI32 l3l4CustomProtoType 									= pMsg->getL3l4CustomProtoType();
		L3L4wellknownProtoType l3l4WellknownProtoType 				= pMsg->getL3l4WellknownProtoType();
		PortOperator srcPortOperator 								= pMsg->getSrcPortOperator();
		PortOperator dstPortOperator 								= pMsg->getDstPortOperator();
		WaveUnion srcPortNumberTcpUnion								= pMsg->getSrcPortNumberTcpUnion();
		WaveUnion srcPortltNumberTcpUnion							= pMsg->getSrcPortltNumberTcpUnion();
		WaveUnion srcPortgtNumberTcpUnion							= pMsg->getSrcPortgtNumberTcpUnion();
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
		PortWellknownNumberTcp srcPortWellknownNumberTcp 			= pMsg->getSrcPortWellknownNumberTcp();
		PortWellknownNumberTcp srcPortltWellknownNumberTcp 			= pMsg->getSrcPortltWellknownNumberTcp();
		PortWellknownNumberTcp srcPortgtWellknownNumberTcp 			= pMsg->getSrcPortgtWellknownNumberTcp();
		PortWellknownNumberTcp dstPortWellknownNumberTcp 			= pMsg->getDstPortWellknownNumberTcp();
		PortWellknownNumberTcp dstPortltWellknownNumberTcp 			= pMsg->getDstPortltWellknownNumberTcp();
		PortWellknownNumberTcp dstPortgtWellknownNumberTcp 			= pMsg->getDstPortgtWellknownNumberTcp();
		PortWellknownNumberUdp srcPortWellknownNumberUdp 			= pMsg->getSrcPortWellknownNumberUdp();
		PortWellknownNumberUdp srcPortltWellknownNumberUdp 			= pMsg->getSrcPortltWellknownNumberUdp();
		PortWellknownNumberUdp srcPortgtWellknownNumberUdp 			= pMsg->getSrcPortgtWellknownNumberUdp();
		PortWellknownNumberUdp dstPortWellknownNumberUdp 			= pMsg->getDstPortWellknownNumberUdp();
		PortWellknownNumberUdp dstPortltWellknownNumberUdp 			= pMsg->getDstPortltWellknownNumberUdp();
		PortWellknownNumberUdp dstPortgtWellknownNumberUdp 			= pMsg->getDstPortgtWellknownNumberUdp();
		UI32 srcPortCustomrangeLowerTcp								= pMsg->getSrcPortCustomrangeLowerTcp();
		UI32 dstPortCustomrangeLowerTcp								= pMsg->getDstPortCustomrangeLowerTcp();
		UI32 srcPortCustomrangeLowerUdp								= pMsg->getSrcPortCustomrangeLowerUdp();
		UI32 dstPortCustomrangeLowerUdp								= pMsg->getDstPortCustomrangeLowerUdp();
		UI32 srcPortCustomrangeHigherTcp						 	= pMsg->getSrcPortCustomrangeHigherTcp();
		UI32 dstPortCustomrangeHigherTcp						 	= pMsg->getDstPortCustomrangeHigherTcp();
		UI32 srcPortCustomrangeHigherUdp						 	= pMsg->getSrcPortCustomrangeHigherUdp();
		UI32 dstPortCustomrangeHigherUdp						 	= pMsg->getDstPortCustomrangeHigherUdp();
		PortWellknownNumberUdp srcPortWellknownrangeLowerUdp		= pMsg->getSrcPortWellknownrangeLowerUdp();
		PortWellknownNumberUdp dstPortWellknownrangeLowerUdp		= pMsg->getDstPortWellknownrangeLowerUdp();
		PortWellknownNumberTcp srcPortWellknownrangeLowerTcp		= pMsg->getSrcPortWellknownrangeLowerTcp();
		PortWellknownNumberTcp dstPortWellknownrangeLowerTcp		= pMsg->getDstPortWellknownrangeLowerTcp();
		PortWellknownNumberUdp srcPortWellknownrangeHigherUdp		= pMsg->getSrcPortWellknownrangeHigherUdp();
		PortWellknownNumberUdp dstPortWellknownrangeHigherUdp		= pMsg->getDstPortWellknownrangeHigherUdp();
		PortWellknownNumberTcp srcPortWellknownrangeHigherTcp		= pMsg->getSrcPortWellknownrangeHigherTcp();
		PortWellknownNumberTcp dstPortWellknownrangeHigherTcp		= pMsg->getDstPortWellknownrangeHigherTcp();
		WellknownPrecedence wellknownPrecedence       	            = pMsg->getWellknownPrecedence();
		WellknownTos wellknownTos                       	        = pMsg->getWellknownTos();
		WellknownDscp wellknownDscp                             	= pMsg->getWellknownDscp();
		UI32  customPrecedence                                  	= pMsg->getCustomPrecedence();
		UI32  customTos                                         	= pMsg->getCustomTos();
		UI32  customDscp                                        	= pMsg->getCustomDscp();
		WaveUnion precedenceUnion                               	= pMsg->getPrecedenceUnion();
		WaveUnion dscpUnion                                     	= pMsg->getDscpUnion();
		WaveUnion tosUnion                                      	= pMsg->getTosUnion();
		bool urg										 			= pMsg->getUrg();
		bool ack										 			= pMsg->getAck();
		bool push										 			= pMsg->getPush();
		bool fin										 			= pMsg->getFin();
		bool rst										 			= pMsg->getRst();
		bool sync										  			= pMsg->getSync();
		UI8	 userEnteredSeqid										= pMsg->getUserEnteredSeqid();
		ResourceId status 											= WAVE_MESSAGE_ERROR;
        
		SsmIpAclRuleMessage *newMsg = new SsmIpAclRuleMessage 
				(policyName, policyType, ruleid, action,
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
                dstPortCustomrangeHigherUdp, dstPortWellknownrangeHigherUdp, 
				wellknownPrecedence, wellknownTos, wellknownDscp, 
				customPrecedence, customTos, customDscp, precedenceUnion, tosUnion, dscpUnion, 
				urg, ack, push, fin, rst, sync, opcode, userEnteredSeqid);

        if (newMsg != NULL) {
 			SSM_PLUG_INFO_LVL_S(string("SsmIpAclRuleMessage message being sent to Ssm - Policyname: ") + policyName + string("policy Type: ") + policyType + string("Ruleid: ") + ruleid + string ("action: ") + action + srcPortltCustomNumberTcp + srcPortgtCustomNumberTcp + dstPortgtCustomNumberTcp + srcPortltCustomNumberTcp + action + string ("customTos: ")+ customTos + string ("wellknownTos: ") + wellknownTos
							+ string ("customDscp: ") + customDscp + string ("wellknownDscp: ") + wellknownDscp
							+ string ("customPrecedence :") + customPrecedence + string ("wellknownPrecedence :") + wellknownPrecedence);
        	status = sendSynchronouslyToWaveClient("ssm", newMsg);
 			SSM_PLUG_INFO_LVL_S(string("ACL Rule to Policy: status - ") + status + string(",Completion status - ") + newMsg->getCompletionStatus ()
 								+ string(",Client status - ") + newMsg->getClientStatus());

 			if (status == WAVE_MESSAGE_SUCCESS ) {
 				if (newMsg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
 					status = newMsg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				} else if (newMsg->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
 					status = newMsg->getClientStatus();
					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				} else {
					SSM_PLUG_INFO_LVL_S(string("New RuleId: ") + newMsg->getRuleid ());
					pMsg->setRuleid(newMsg->getRuleid ());
					if(opcode ==  SSMACL_DCMD_ADD_RULE) {
						ruleid = newMsg->getRuleid ();
					}
				}
			} else {
				SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
			}

        	delete newMsg;
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}

	/* the following code intimates the AAA plugin asynchronously */
		AAALocalAssocIpRuleToPolicyMessage *notifyManagementIfMsg = new AAALocalAssocIpRuleToPolicyMessage
				(policyName, policyType, ruleid, action,
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
                dstPortCustomrangeHigherUdp, dstPortWellknownrangeHigherUdp, 
				wellknownPrecedence, wellknownTos, wellknownDscp, 
				customPrecedence, customTos, customDscp, precedenceUnion, tosUnion, dscpUnion, 
				urg, ack, push, fin, rst, sync, opcode, userEnteredSeqid);

			LocationId locVector;
			locVector = NsmUtils::getLocalLocationId();

			trace (TRACE_LEVEL_INFO, string("SsmIpAclRuleMessage message being sent to AAA - Policyname : ") + policyName + string("policy Type: ") + policyType
					+ string("Ruleid: ") + ruleid + string ("action: ") + action);

			ResourceId aaastatus = sendOneWay(notifyManagementIfMsg, locVector);
			trace (TRACE_LEVEL_DEBUG, string("SSM notified AAA of rule change Oneway ") + aaastatus);
		pMsg->setCompletionStatus(status);
		reply(pMsg);
	}

	void SsmLocalIpAclWorker::SsmLocalServiceResqIpAclPolicyMessageHandler(SsmLocalServiceResqIpAclPolicyMessage *pMsg)
	{
		SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pMsg);
		UI64 seqId 				= pMsg->getSeqId();
		UI64 incSeqId 			= pMsg->getIncSeqId();
		const string policyName = pMsg->getPolicyName();
		ResourceId status 		= WAVE_MESSAGE_ERROR;

        SsmIpAclReseqMessage *newMsg =
        		new SsmIpAclReseqMessage(policyName, seqId,incSeqId);
        if (newMsg != NULL) {
 			SSM_PLUG_DBG_LVL_S(string("SsmIpAclReseqMessage message being sent to Ssm - Policyname: ") + policyName +
 							string("Seq Id: ") + seqId + string("Inc Seq Id: ") + incSeqId );
        	status = sendSynchronouslyToWaveClient("ssm", newMsg);
 			SSM_PLUG_INFO_LVL_S(string("ACL Resequence Rule : status - ") + status + string(",Completion status - ") + newMsg->getCompletionStatus ()
 								+ string(",Client status - ") + newMsg->getClientStatus());

 			if (status == WAVE_MESSAGE_SUCCESS ) {
 				if (newMsg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) {
 					status = newMsg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to resequence policy on the backend: ") + policyName);
				} else if (newMsg->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
 					status = newMsg->getClientStatus();
					SSM_PLUG_ERR_LVL_S(string("Failed to resequence  policy on the backend: ") + policyName);
				} else {
					SSM_PLUG_INFO_LVL_S(string("ACL Rule Sequencing is success: "));
				}
			} else {
				SSM_PLUG_ERR_LVL_S(string("Failed to resequence policy on the backend: ") + policyName);
			}

        	delete newMsg;
		} else {
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}

		/* asynchronously notify AAA local plugin of resequece operation */
		AAALocalIpAclReseqMessage *notifyManagementIfMsg =
					 new AAALocalIpAclReseqMessage(policyName, seqId,incSeqId);
		if (notifyManagementIfMsg) {
			LocationId locVector;
			locVector = NsmUtils::getLocalLocationId();
			ResourceId aaastatus = sendOneWay(notifyManagementIfMsg, locVector);
			trace (TRACE_LEVEL_INFO, string("SSM notified AAA of rule resequence change") + aaastatus);
		}

		pMsg->setCompletionStatus(status);	
		reply(pMsg);
	}


    void SsmLocalIpAclWorker::SsmMacIpAclLocalClearSendToClusterCallback ( WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p  = NULL;
        SsmMacIpAclLocalClearMessage            *req_msg_p  = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p  = NULL;

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<SsmMacIpAclLocalClearMessage *> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<SsmMacIpAclLocalClearMessage *> (ctx_p->getPPrismMessageForPhase1());

        SSM_PLUG_ASSERT_PTR(req_msg_p);
        SSM_PLUG_ASSERT_PTR(resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        bool isAllFail = true;
        bool isRbridgeIdAll = false;

        if(strcmp(req_msg_p->getRbridgeId().c_str(),"all") == 0) {
        	isRbridgeIdAll = true;
        }

        ResourceId clusterCompletionStatus = WAVE_MESSAGE_SUCCESS;

        for (i = 0; i < locations.size(); i++) 
		{
            WaveMessageStatus sendStatus = ctx_p->getSendStatusForPhase1 (locations[i]);
            if (WAVE_MESSAGE_SUCCESS == sendStatus) {
	            sw_resp_p = dynamic_cast<SsmMacIpAclLocalClearMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
    	        SSM_PLUG_ASSERT_PTR(sw_resp_p);
        	    if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()){
        	    	isAllFail = false;
            	    SSM_PLUG_INFO_LVL_S(string("WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus() "));
	            } else {
	            	 clusterCompletionStatus = sw_resp_p->getCompletionStatus();
        	         SSM_PLUG_ERR_LVL_S(string("Response processing: The location: ") + locations[i] + string("returned error: ") + FrameworkToolKit::localize(clusterCompletionStatus));
	                 req_msg_p->setCompletionStatus(clusterCompletionStatus);
					 if(!isRbridgeIdAll) {
						 break;
					 }
        	    }
			}
        }

        if(isRbridgeIdAll && (!isAllFail)) {
        	req_msg_p->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            seq_ctx_p->executeNextStep(clusterCompletionStatus);
        }

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void  SsmLocalIpAclWorker::sendMacIpAclClearCommandToCluster ( PrismLinearSequencerContext *seq_ctx_p)
    {
        SsmMacIpAclLocalClearMessage          *msg_p = NULL;
        SsmMacIpAclLocalClearMessage          *loc_msg_p = NULL;
        WaveSendToClusterContext              *send_ctx_p = NULL;
        bool                                  sendToLocal = false;
        UI32                                  locationId = 0;
        SI32                                  mappedId = 0;
        vector<UI32>                          locations;

        msg_p = dynamic_cast<SsmMacIpAclLocalClearMessage *> (seq_ctx_p->getPPrismMessage ());

        SSM_PLUG_ASSERT_PTR(msg_p);
        loc_msg_p = new SsmMacIpAclLocalClearMessage();
        loc_msg_p->setIfName(msg_p->getIfName());
        loc_msg_p->setPolicyName(msg_p->getPolicyName());
        loc_msg_p->setIfType(msg_p->getIfType());
        loc_msg_p->setOpCode(msg_p->getOpCode());
        loc_msg_p->setIsGwRequest(false);
		loc_msg_p->setAclType(msg_p->getAclType());
        loc_msg_p->setDirection(msg_p->getDirection());
														
        UI32 opCode = msg_p->getOpCode();

        SSM_PLUG_ASSERT_PTR(loc_msg_p);

        SSM_PLUG_INFO_LVL_S(string("Sending Clear Command to Cluster"));
        SSM_PLUG_INFO_LVL_S(string("    OpCode =") + msg_p->getOpCode() +
                        	string("    PolicyName =")  + msg_p->getPolicyName() +
                        	string("    ifName =") + msg_p->getIfName() +
                        	string("    ifType is=" ) + msg_p->getIfType() +
							string("    Direction is =" ) + msg_p->getDirection() +
							string("    AclType is =") + msg_p->getAclType()
							);

        /* Check Validity of mapped id before sending any msg */
        if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI) 
			|| (msg_p->getIfType() == IF_TYPE_FO))
		{
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  
                sendToLocal = true;
            else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  
			{
                /* Send to the targetted LocationId pointed to by the mappedId. */
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
                if (!locationId) 
				{
                    msg_p->setCompletionStatus (SSM_ACL_CLEAR_STATS_RC_NO_IF);
                    seq_ctx_p->executeNextStep (SSM_ACL_CLEAR_STATS_RC_NO_IF);
                    delete loc_msg_p;
                    return;
                } 
				else
                    locations.push_back(locationId);
            }
        }

        send_ctx_p = new WaveSendToClusterContext(this,reinterpret_cast<PrismAsynchronousCallback>(&SsmLocalIpAclWorker::SsmMacIpAclLocalClearSendToClusterCallback), seq_ctx_p);
        SSM_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p) 
            return;

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        /*if (0 == msg_p->getIfName().length()) 
		{
             * If the interface context is not there, send it to all nodes.
             * In case of adding a new cli which doesn't have interface
             * context make necessary changes in this code block
             *
        } */
		if (0 != msg_p->getIfName().length())
		{
            if ((msg_p->getIfType() == IF_TYPE_TE)  || (msg_p->getIfType() == IF_TYPE_GI) 
				|| (msg_p->getIfType() == IF_TYPE_FO))
			{
                if (!sendToLocal)
                    send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
            else if (msg_p->getIfType () == IF_TYPE_PO)
            {
            	 // needs to find out the phy. interface which is a member of the po interface, and then send the request
                 // to the node of the phy. interface, to do the aggregation for the rule status of the po.

                 NsmUtils::setLocationIdsForPoSubModeCmds(msg_p->getIfName(),send_ctx_p);

                 if(send_ctx_p->getLocationsToSendToForPhase1().size() == 0) {
                	 vector<UI32> locationIds;
                	 locationIds.push_back(FrameworkToolKit::getThisLocationId ());
                	 send_ctx_p->setLocationsToSendToForPhase1(locationIds);
                 }
            }
            else if(msg_p->getIfType () == IF_TYPE_VL)
            {
                // needs to find out the phy. interface which is a member of the vlan, and then send the request
                // to the node of the phy. interface, to do the aggregation for the rule status of the vlan.
                if(opCode == SSMACL_DCMD_CLEAR_STATS_ACL_ON_INTERFACE || opCode == SSMACL_DCMD_CLEAR_STATS_ON_INTERFACE)
                {
                    vector<LocationId> locationIds;

                    if(msg_p->getAclType() == SSMACL_TYPE_L2_E)
                    {
                        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
                        UI32 vlanId = atol (msg_p->getIfName().c_str());
                        syncQueryCtxt.addAndAttribute(new AttributeUI32(vlanId, "id"));
                        syncQueryCtxt.setLoadOneToManyRelationships(false);
                        syncQueryCtxt.setLoadCompositions(false);
                        syncQueryCtxt.addSelectField("objectId");
                        vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);

                        if (NULL != pResults && pResults->size () > 0)
                        {
                            VlanIntfManagedObject* vlanIntfMo = dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
                            setLocationIdsForActiveVlanId(vlanIntfMo->getObjectId(), vlanIntfMo->getId(), locationIds);
                        }
                    }

                    if( locationIds.size() == 0 ) {
                        locationIds.push_back(FrameworkToolKit::getThisLocationId ());
                    }

                    send_ctx_p->setLocationsToSendToForPhase1(locationIds);
                }
            }
            else if(msg_p->getIfType () == IF_TYPE_VE)
            {
            	if( opCode == SSMACL_DCMD_CLEAR_STATS_ACL_ON_INTERFACE || opCode == SSMACL_DCMD_CLEAR_STATS_ON_INTERFACE )
            	{
					if((!strcmp(msg_p->getRbridgeId().c_str(),"all") == 0) && (!msg_p->getRbridgeId().empty()) )
					{
						UI32 rbridgeID = 0;
						stringstream tmpStream(msg_p->getRbridgeId());
						tmpStream >> rbridgeID;
						locationId = DcmToolKit::getLocationIdFromRbridgeId(rbridgeID);
		                if (!locationId)
		                {
							msg_p->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		                    seq_ctx_p->executeNextStep (WAVE_MESSAGE_SUCCESS);
		                    delete loc_msg_p;
		                    delete send_ctx_p;
		                    return;
		                }
		                locations.push_back(locationId);
					} else if(msg_p->getRbridgeId().empty()) {
						locations.push_back(FrameworkToolKit::getThisLocationId());
					}

					if(!locations.empty()) {
						send_ctx_p->setLocationsToSendToForPhase1(locations);
					}
            	}
            }
        }

        if (sendToLocal) 
		{
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) 
               locations.push_back(locationId);
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
        sendToWaveCluster(send_ctx_p);
        return;
    }

    ResourceId SsmLocalIpAclWorker::sendClearToSsmDaemon ( PrismSynchronousLinearSequencerContext *ctx_p)
    {
        SsmMacIpAclLocalClearMessage *msg_p 	= dynamic_cast<SsmMacIpAclLocalClearMessage *> (ctx_p->getPPrismMessage());
        AclClearStatsMessage 		 *cl_msg_p  = NULL;
        ResourceId      			 status;
        SI32            			 mappedId   = 0;
        string          			 ifName     = "";
        ssmAcl_binding_direction direction = (ssmAcl_binding_direction) msg_p->getDirection();

        if( direction == SSMACL_BINDING_DIRECTION_OUT_E ) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS)){
				trace(TRACE_LEVEL_INFO,"Clear ACL in egress direction not supported in this platform");
				status = SSM_DCM_ERR_CLEAR_ACL_INTF_EGRESS;
				msg_p->setCompletionStatus(status);
				return status;
			}
        }

        cl_msg_p 								= new AclClearStatsMessage ();

        if (cl_msg_p != NULL) 
		{
            char buffer[25];
            buffer[24] = '\0';
            UI32 bufferSize = sizeof (buffer);
            if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI)
				|| (msg_p->getIfType() == IF_TYPE_FO))
			{
                /* Three tuple to two tuple conversion */
                if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) 
                    NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
                else 
                    ifName = msg_p->getIfName();
                
                if(msg_p->getIfType() == IF_TYPE_TE)
                    snprintf(buffer, bufferSize, "te%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_GI)
					snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_FO)
					snprintf(buffer, bufferSize, "fo%s", ifName.c_str());
            } 
			else if (msg_p->getIfType () == IF_TYPE_PO) 
                snprintf(buffer, bufferSize, "po%s", msg_p->getIfName ().c_str());
            else if (msg_p->getIfType () == IF_TYPE_VL) 
                snprintf(buffer, bufferSize, "vlan0.%s", msg_p->getIfName ().c_str());
            else if (msg_p->getIfType () == IF_TYPE_VE) 
                snprintf(buffer, bufferSize, "ve0.%s", msg_p->getIfName ().c_str());
            else 
                snprintf(buffer, bufferSize, "%s", msg_p->getIfName ().c_str());
            
            mappedId = DcmToolKit::getLocalMappedId();

            if (mappedId == (-1)) 
                mappedId = 0;

            cl_msg_p->setPolicyName(msg_p->getPolicyName ());
            cl_msg_p->setIfName(buffer);
            cl_msg_p->setOpCode(msg_p->getOpCode());
            cl_msg_p->setMappedId(mappedId);
			cl_msg_p->setAclType(msg_p->getAclType());
			cl_msg_p->setDirection(msg_p->getDirection());

            trace (TRACE_LEVEL_INFO, "sendToLocalNode: Message being sent to Ssm");

            status = sendSynchronouslyToWaveClient ("ssm", cl_msg_p);

            if (status == WAVE_MESSAGE_SUCCESS) 
			{
                if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) 
				{
                	status = getResourceIdFromClientStatus(cl_msg_p->getCompletionStatus());
                    trace(TRACE_LEVEL_INFO, string("Resp: Client Completion Error MsgId: ") +  cl_msg_p->getMessageId() + FrameworkToolKit::localize(status));

                    msg_p->setCompletionStatus(status);

                    if(cl_msg_p)
						delete cl_msg_p;
                    
					return status;
                }
				
				if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS)
				{
					trace(TRACE_LEVEL_INFO, string("Resp:  cl_msg_p->getClientStatus(): ") + cl_msg_p->getClientStatus());
                    status = getResourceIdFromClientStatus(cl_msg_p->getClientStatus());
                    trace(TRACE_LEVEL_INFO, string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(status));
					msg_p->setCompletionStatus(status);
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }
            }

            if (status != WAVE_MESSAGE_SUCCESS) 
			{
                trace(TRACE_LEVEL_INFO, string("Client Message sending failed for MsgId: ") + cl_msg_p->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
                if(cl_msg_p)
                    delete cl_msg_p;
                return status;
            }
            trace(TRACE_LEVEL_INFO, string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());

            trace(TRACE_LEVEL_INFO, "sendToLocalNode: Successfully protocol daemon");

            status = cl_msg_p->getCompletionStatus();
            if(cl_msg_p)
                delete cl_msg_p;
            return status;
        } 
		else 
		{
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, "sendToLocalNode: Message creation failed");
        }

        if(cl_msg_p)
            delete cl_msg_p;
        
        return status;
    }

    void SsmLocalIpAclWorker::SsmMacIpAclLocalClearMessageHandler ( SsmMacIpAclLocalClearMessage *pSsmMacIpAclLocalClearMessage)
    {
        if (pSsmMacIpAclLocalClearMessage->getIsGwRequest()) 
		{ 
			PrismLinearSequencerStep sequencerSteps[] =
        	{
            	reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalIpAclWorker::sendMacIpAclClearCommandToCluster),
	            reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalIpAclWorker::prismLinearSequencerSucceededStep),
    	        reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalIpAclWorker::prismLinearSequencerFailedStep)
        	};

	        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pSsmMacIpAclLocalClearMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
    	    pPrismLinearSequencerContext->start ();
        	return;
        } 
		else 
		{
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
	        {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalIpAclWorker::sendClearToSsmDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalIpAclWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalIpAclWorker::prismSynchronousLinearSequencerFailedStep)
            };

	        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pSsmMacIpAclLocalClearMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();
        }
	}

    ResourceId SsmLocalIpAclWorker::getResourceIdFromClientStatus(const SI32 &clientStatus)
    {
		ResourceId resourceId = WAVE_MESSAGE_SUCCESS;

		switch (clientStatus)
		{
			case SSM_DCM_ERR_ACL_CONFIG_CHANGE:
				resourceId = WRC_SSM_DCM_ERR_ACL_CONFIG_CHANGE_TMP;
				break;
			case SSM_DCM_ERR_MAC_ACL_EEXIST:
				resourceId = WRC_SSM_DCM_ERR_MAC_ACL_EXIST_TMP;
				break;
			case SSM_DCM_ERR_IP_ACL_EEXIST:
				resourceId = WRC_SSM_DCM_ERR_IP_ACL_EXIST_TMP;
				break;
			case SSM_ERR_ACL_NOT_APPLIED_TMP:
				resourceId = WRC_SSM_DCM_ERR_ACL_NOT_APPLIED_TMP;
				break;
			case SSM_ERR_ACL_CMD_FAIL_RETRY_TMP:
				resourceId = WRC_SSM_DCM_ERR_ACL_CMD_FAIL_RETRY_AGAIN_TMP;
				break;
			default:
				resourceId = clientStatus;
				break;
		}

		return resourceId;
   }

	ResourceId SsmLocalIpAclWorker::getErrorStatusPerPlatform (ResourceId status)
	{
		ResourceId retStatus = status;

    	switch (Capabilities::instance()->getSwModel())
    	{
			case SWBD1000:
			case SWBD1001:
			case SWBD1002:
			case SWBD1004:
			{
				if(status == WRC_SSM_DCM_ERR_ACL_LIMIT)
				{
					retStatus = WRC_SSM_DCM_ERR_ACL_LIMIT_WTH_LC;
				}
				else if(status == WRC_SSM_DCM_ERR_ACL_RULE_LIMIT)
				{
					retStatus = WRC_SSM_DCM_ERR_ACL_RULE_LIMIT_WITH_LC;
				}

				break;
			}
			case SWBD95:
			case SWBD96:
			case SWBD97:
			case SWBD107:
			case SWBD116:
			case SWBD131:
			case SWBD137:
			{
				if(status == WRC_SSM_DCM_ERR_ACL_LIMIT)
				{
					retStatus = WRC_SSM_DCM_ERR_ACL_LIMIT_NO_LC;
				}
				else if(status == WRC_SSM_DCM_ERR_ACL_RULE_LIMIT)
				{
					retStatus = WRC_SSM_DCM_ERR_ACL_RULE_LIMIT_NO_LC;
				}

				break;
			}
    	}

    	return retStatus;
	}

}



