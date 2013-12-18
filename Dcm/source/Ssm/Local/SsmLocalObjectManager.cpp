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

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmLocalServiceAddMacAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceRemMacAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceAssocRuleToPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceResqMacAclPolicyMessage.h"
#include "Ssm/Local/SsmLocalServiceAssocPolicyToInterfaceMessage.h"
#include "Ssm/Local/SsmTypes.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Ssm/Global/StdMacAclPolicyManagedObject.h"
#include "Ssm/Global/ExtMacAclPolicyManagedObject.h"
#include "Ssm/Global/StdMacAclRuleManagedObject.h"
#include "Ssm/Local/SsmMacAclShowLocalSPMessage.h"
#include "ClientInterface/Ssm/SsmMacAclPolicyBulkMessage.h"
#include "ClientInterface/Ssm/SsmMacAclIntfAssocBulkMessage.h"
#include "ClientInterface/Ssm/SsmMacAclPolicyMessage.h"
#include "ClientInterface/Ssm/SsmMacAclRuleMessage.h"
#include "ClientInterface/Ssm/SsmMacAclReseqMessage.h"
#include "ClientInterface/Ssm/SsmMacAclShowMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include <sstream>
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/Capabilities.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Ssm/Global/SsmPlugUtils.h"
#include "Ssm/Local/SsmLocalIpAclWorker.h"
#include "Ssm/Local/SsmBpratelimitLocalWorker.h"
#include "Ssm/Local/StormControlLocalWorker.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressMacAclPolicyManagedObject.h"
#include "ClientInterface/StormControl/NsmStormControlClientIntfConfSetMsg.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "DcmCStatus.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Qos/qos_bum_dcm_common.h"

namespace DcmNs
{
    SsmLocalObjectManager::SsmLocalObjectManager ()  : WaveLocalObjectManager  (getClassName ())
    {
        m_pSsmLocalIpAclWorker = new SsmLocalIpAclWorker (this);
        prismAssert (NULL != m_pSsmLocalIpAclWorker, __FILE__, __LINE__);
        m_pSsmBpratelimitLocalWorker = new SsmBpratelimitLocalWorker (this);
        prismAssert (NULL != m_pSsmBpratelimitLocalWorker, __FILE__, __LINE__);
        m_pStormControlLocalWorker = new StormControlLocalWorker (this);
        prismAssert (NULL != m_pStormControlLocalWorker, __FILE__, __LINE__);

		addOperationMap (SSMLOCALSERVICEADDMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalObjectManager::SsmLocalServiceAddMacAclPolicyMessageHandler));
        addOperationMap (SSMLOCALSERVICEREMMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalObjectManager::SsmLocalServiceRemMacAclPolicyMessageHandler));
        addOperationMap (SSMLOCALSERVICEASSOCRULETOPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalObjectManager::SsmLocalServiceAssocRuleToPolicyMessageHandler));
        addOperationMap (SSMMACACLSHOWLOCALSP, reinterpret_cast<PrismMessageHandler> (&SsmLocalObjectManager::SsmMacAclShowLocalSPMessageHandler));
        addOperationMap (SSMLOCALSERVICERESEQMACACLPOLICY, reinterpret_cast<PrismMessageHandler> (&SsmLocalObjectManager::SsmLocalServiceResqMacAclPolicyMessageHandler));
		//addOperationMap (SSMMACACLLOCALCLEAR, reinterpret_cast<PrismMessageHandler> (&SsmLocalObjectManager::SsmMacAclLocalClearMessageHandler));
    }

    SsmLocalObjectManager::~SsmLocalObjectManager ()
    {
    }

    SsmLocalObjectManager  *SsmLocalObjectManager::getInstance()
    {
        static SsmLocalObjectManager *pSsmLocalObjectManager = new SsmLocalObjectManager ();
        WaveNs::prismAssert (NULL != pSsmLocalObjectManager, __FILE__, __LINE__);
        return (pSsmLocalObjectManager);
    }

    string  SsmLocalObjectManager::getClassName()
    {
        return ("Ssm");
    }

    PrismServiceId  SsmLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SsmLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case SSMLOCALSERVICEADDMACACLPOLICY :
                pPrismMessage = new SsmLocalServiceAddMacAclPolicyMessage ();
                break;
            case SSMLOCALSERVICEREMMACACLPOLICY :
                pPrismMessage = new SsmLocalServiceRemMacAclPolicyMessage ();
                break;
            case SSMLOCALSERVICEASSOCRULETOPOLICY :
                pPrismMessage = new SsmLocalServiceAssocRuleToPolicyMessage ();
                break;
            case SSMLOCALSERVICEASSOCPOLICYTOINTERFACE :
                pPrismMessage = new SsmLocalServiceAssocPolicyToInterfaceMessage ();
                break;
            case SSMMACACLSHOWLOCALSP :
            	pPrismMessage = new SsmMacAclShowLocalSPMessage ();
            	break;
            case SSMLOCALSERVICERESEQMACACLPOLICY :
            	pPrismMessage = new SsmLocalServiceResqMacAclPolicyMessage ();
            	break;
#if 0
			case SSMMACACLLOCALCLEAR :
				pPrismMessage = new SsmMacAclLocalClearMessage();
				break;
#endif
			default :
                pPrismMessage = NULL;
        }
        return (pPrismMessage);
    }

    WaveManagedObject  *SsmLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
        return (pWaveManagedObject);
    }

    void SsmLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
		string passName = pWaveAsynchronousContextForPostBootPhase->getPassName();
		
		SSM_PLUG_INFO_LVL_S(string("Ssm Postboot Entered with passName: ") + passName);
		
		if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE1") == 0) {
			ConfigReplayMacAclPolicy();
        } else if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE5") == 0) {
			/* Vlan and Po */
			ConfigReplayMacAclGlobalIntf(VLAN_INTF_TYPE);
			ConfigReplayMacAclGlobalIntf(PO_INTF_TYPE);
            SSM_PLUG_INFO_LVL_S("SsmLocalObjectManager::postboot passName DCM_POSTBOOT_GLOBAL_STAGE5");
		} else if (passName.compare("DCM_POSTBOOT_SLOT_STAGE2") == 0) {
            SSM_PLUG_INFO_LVL_S("SsmLocalObjectManager::postboot passName DCM_POSTBOOT_SLOT_STAGE2");
			SI32 slotNum = pWaveAsynchronousContextForPostBootPhase->getSlotNum();
            ConfigReplayMacAclIntf(slotNum);
        }	
		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback ();
	}

	void SsmLocalObjectManager::ConfigReplayMacAclGlobalIntf(const UI32 ifType)
    {
        string className										= "";
        vector<WaveManagedObject *> *pPoVlanResults             = NULL;

        SSM_PLUG_INFO_LVL_S(string ("ConfigReplay of MAC ACL Global Interface Association: ...") + ifType);

        if (ifType == PO_INTF_TYPE) {
            className = PoIntfManagedObject::getClassName();
        } else if (ifType == VLAN_INTF_TYPE) {
            className = VlanIntfManagedObject::getClassName();
        }


        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        syncQueryCtxt.setLoadCompositions(false);
        syncQueryCtxt.addSelectField("objectId");
        syncQueryCtxt.addSelectField("id");
		pPoVlanResults = querySynchronously (&syncQueryCtxt);
		configReplayMacAclGlobalIntf(ifType, pPoVlanResults);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPoVlanResults);

    }

	void SsmLocalObjectManager::configReplayMacAclGlobalIntf(const UI32 ifType,
	        vector<WaveManagedObject *> *&pPoVlanResults) {

	    vector<Attribute*> poVlanObjectIds;
	    map<ObjectId, UI32> poVlanIdObjectIdMap;
	    ObjectId poVlanObjectId                                 = ObjectId::NullObjectId;
	    ResourceId status                                       = WAVE_MESSAGE_ERROR;

		if (NULL != pPoVlanResults)
		{
            UI32 numberOfPoVlanResults = pPoVlanResults->size ();
            WaveNs::trace(TRACE_LEVEL_INFO, string("Number of Vlan/Po interfaces: ")+ numberOfPoVlanResults);
            UI32 poVlanId = 0;
            for (UI32 i = 0; i < numberOfPoVlanResults; i++) {
                if (ifType == PO_INTF_TYPE) {
                	PoIntfManagedObject *pPoIntfManagedObject  = dynamic_cast<PoIntfManagedObject *>((*pPoVlanResults)[i]);
                	poVlanObjectId = pPoIntfManagedObject->getObjectId();
                	poVlanId = pPoIntfManagedObject->getId();
                } else if (ifType == VLAN_INTF_TYPE) {
                	VlanIntfManagedObject *pVlanntfManagedObject  = dynamic_cast<VlanIntfManagedObject *>((*pPoVlanResults)[i]);
                	poVlanObjectId = pVlanntfManagedObject->getObjectId();
                	poVlanId = pVlanntfManagedObject->getId();
                }
            	if (ObjectId::NullObjectId != poVlanObjectId) {
            		poVlanObjectIds.push_back(new AttributeUI64(poVlanObjectId.getInstanceId(), "ownermanagedobjectidinstanceid"));
                	if (poVlanId) {
                		poVlanIdObjectIdMap[poVlanObjectId] = poVlanId;
                	}
            	}
            }
		}

		vector<WaveManagedObject *> *pIngressEgressResults      = NULL;
		if( poVlanObjectIds.size() > 0 ) {
	        WaveManagedObjectSynchronousQueryContext syncQueryCtxt2("NsmLocalIngressEgressMacAclPolicyManagedObject");
	        syncQueryCtxt2.setLoadOneToManyRelationships(false);
	        syncQueryCtxt2.setLoadCompositions(false);
	        syncQueryCtxt2.addAndAttribute (new AttributeSqlIn(poVlanObjectIds), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
            pIngressEgressResults = WaveObjectManagerToolKit::querySynchronously (&syncQueryCtxt2);
		}

		if (NULL != pIngressEgressResults) {
            UI32 numberOfResults = pIngressEgressResults->size ();
            WaveNs::trace(TRACE_LEVEL_INFO, string("Number of MAC ACL applied on Ingress/Egress direction: ")+ numberOfResults);

            for (UI32 i = 0; i < numberOfResults; i++) {
				SsmMacAclIntfAssocBulkMessage *pSsmMacAclIntfAssocBulkMessage   = NULL;

				if(((*pIngressEgressResults)[i]) == NULL) {
				    WaveNs::trace(TRACE_LEVEL_INFO,
				            "****VlanMOs/PortChannel MOs ith element is NULL in SsmLocalObjectManager***");
					continue;	
				}

				NsmLocalIngressEgressMacAclPolicyManagedObject *nsmIngressEgressMacAclManagedObject  =
						dynamic_cast<NsmLocalIngressEgressMacAclPolicyManagedObject *>((*pIngressEgressResults)[i]);

				UI32 vlanPoId = poVlanIdObjectIdMap[nsmIngressEgressMacAclManagedObject->getOwnerManagedObjectId()];

				if(vlanPoId) {
					pSsmMacAclIntfAssocBulkMessage = SsmLocalObjectManager::ConfigReplayMacAclGlobalIntfAssoc(ifType, nsmIngressEgressMacAclManagedObject,vlanPoId);
				} else {
					continue;
				}
				
				if (pSsmMacAclIntfAssocBulkMessage) {
					status = WaveObjectManagerToolKit::sendSynchronouslyToWaveClient ("ssm",pSsmMacAclIntfAssocBulkMessage);
					if (status == WAVE_MESSAGE_SUCCESS) {
					    WaveNs::trace(TRACE_LEVEL_INFO, "Vlan/Po Interface Mac Acl configuration successful ");
					} else {
					    WaveNs::trace(TRACE_LEVEL_INFO, string("Vlan/Po Inerface Mac Acl Interface configuration failed - ") + status);
					}
					delete pSsmMacAclIntfAssocBulkMessage;
					pSsmMacAclIntfAssocBulkMessage = NULL;
				} else {
				    WaveNs::trace(TRACE_LEVEL_INFO, "SsmLocalObjectManager::ConfigReplayMacAclGlobalIntf SsmMacAclIntfAssocBulkMessage is NULL");
				}
			}
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pIngressEgressResults);
		}

		return;
	}

	SsmMacAclIntfAssocBulkMessage *SsmLocalObjectManager::ConfigReplayMacAclGlobalIntfAssoc(const UI32 ifType,
			NsmLocalIngressEgressMacAclPolicyManagedObject* nsmIngressEgressMacAclManagedObject, UI32 poVlanId )
	{
		UI32 num 										= 0;
		char buffer[25];
		ObjectId IngresspolicyObjId						= ObjectId::NullObjectId;
		ObjectId EgresspolicyObjId						= ObjectId::NullObjectId;
		UI32 Ingressfound								= 0;
		UI32 Egressfound								= 0;
		SsmMacAclIntfAssocBulkMessage *intfBulkMsg		= NULL;

		if (ifType == PO_INTF_TYPE) {
			sprintf(buffer, "po%d", poVlanId);
		} else if (ifType == VLAN_INTF_TYPE) {
			sprintf(buffer, "vlan0.%d", poVlanId);
		}

		if (nsmIngressEgressMacAclManagedObject) {
			if ( nsmIngressEgressMacAclManagedObject->getDirection() == SSMACL_BINDING_DIRECTION_IN_E ) {
				IngresspolicyObjId = nsmIngressEgressMacAclManagedObject->getIngressEgressMacAcl();
				Ingressfound = 1;
				num = num + 1;
			}
			if ( nsmIngressEgressMacAclManagedObject->getDirection() == SSMACL_BINDING_DIRECTION_OUT_E ) {
				EgresspolicyObjId = nsmIngressEgressMacAclManagedObject->getIngressEgressMacAcl();
				Egressfound = 1;
				num = num + 1;
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
			string IngresspolicyName   	= "";
			string EgresspolicyName     = "";
			string ifname               = buffer;
			intfBulkMsg                 = new SsmMacAclIntfAssocBulkMessage();

			if ((Ingressfound) && (Egressfound)) {
				IngresspolicyName   = SsmLocalObjectManager::getPolicyNameByObjId(IngresspolicyObjId);
				EgresspolicyName    = SsmLocalObjectManager::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_IN_E);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			} else if (Ingressfound) {
				IngresspolicyName   = SsmLocalObjectManager::getPolicyNameByObjId(IngresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_IN_E);
			} else if (Egressfound) {
				EgresspolicyName    = SsmLocalObjectManager::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			}
			intfBulkMsg->setNumRecords(num);
		}
		return (intfBulkMsg);
	}

	SsmMacAclIntfAssocBulkMessage *SsmLocalObjectManager::ConfigReplayMacAclGlobalIntfAssoc(const UI32 ifType, WaveManagedObject 		* pWaveManagedObject) 
	{	
		
		PoIntfManagedObject *pPoIntfManagedObject 		= NULL;
		VlanIntfManagedObject *pVlanIntfManagedObject 	= NULL;
		UI32 num 										= 0;
		char buffer[25];				
		ObjectId IngresspolicyObjId						= ObjectId::NullObjectId;
		ObjectId EgresspolicyObjId						= ObjectId::NullObjectId;
		UI32 Ingressfound								= 0;
		UI32 Egressfound								= 0;
		SsmMacAclIntfAssocBulkMessage *intfBulkMsg		= NULL;
		
		if (ifType == PO_INTF_TYPE) {
			pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>(pWaveManagedObject);
			if (pPoIntfManagedObject) {
				if ((Ingressfound = pPoIntfManagedObject->getIngressEgressMacAclPolicyObjectId
					(IngresspolicyObjId, SSMACL_BINDING_DIRECTION_IN_E)))
					num = num + 1;
				if ((Egressfound = pPoIntfManagedObject->getIngressEgressMacAclPolicyObjectId
					(EgresspolicyObjId, SSMACL_BINDING_DIRECTION_OUT_E)))
					num = num + 1;
				
				sprintf(buffer, "po%d", pPoIntfManagedObject->getId());
			}
		} else if (ifType == VLAN_INTF_TYPE) {
			pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>(pWaveManagedObject);
			if (pVlanIntfManagedObject) {
				if ((Ingressfound = pVlanIntfManagedObject->getIngressEgressMacAclPolicyObjectId
					(IngresspolicyObjId, SSMACL_BINDING_DIRECTION_IN_E)))
					num = num + 1;
				if ((Egressfound = pVlanIntfManagedObject->getIngressEgressMacAclPolicyObjectId
					(EgresspolicyObjId, SSMACL_BINDING_DIRECTION_OUT_E)))
					num = num + 1;

				sprintf(buffer, "vlan0.%d", pVlanIntfManagedObject->getId());
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
			string IngresspolicyName   	= "";
			string EgresspolicyName     = "";
			string ifname               = buffer;
			intfBulkMsg                 = new SsmMacAclIntfAssocBulkMessage();
			
			if ((Ingressfound) && (Egressfound)) {
				IngresspolicyName   = SsmLocalObjectManager::getPolicyNameByObjId(IngresspolicyObjId);
				EgresspolicyName    = SsmLocalObjectManager::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_IN_E);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			} else if (Ingressfound) {
				IngresspolicyName   = SsmLocalObjectManager::getPolicyNameByObjId(IngresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_IN_E);
			} else if (Egressfound) {
				EgresspolicyName    = SsmLocalObjectManager::getPolicyNameByObjId(EgresspolicyObjId);
				intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
					SSMACL_BINDING_DIRECTION_OUT_E);
			}
			intfBulkMsg->setNumRecords(num);
		}
		return (intfBulkMsg);
	}
	
	void SsmLocalObjectManager::ConfigReplayMacAclPolicy()
	{
		SSM_PLUG_INFO_LVL("Replaying Standard Access List....");
		ConfigReplayStdMacAclPolicy();
		SSM_PLUG_INFO_LVL("Replaying Extended Access List....");
		ConfigReplayExtMacAclPolicy();
	}

    void SsmLocalObjectManager::ConfigReplayMacAclIntf(SI32 slotNum)
    {
        SSM_PLUG_INFO_LVL("Replaying Mac Acl Interface Association:Binding....");
        if (slotNum >= 0) {
			/* Phy Mo's */
			vector<PhyIntfLocalManagedObject *> phyMOs;
			NsmUtils::SlotGetPhyIntfMOs(phyMOs, slotNum, FrameworkToolKit::getThisLocationId());
            UI32 size = phyMOs.size();
			UI32 i = 0;
			for (i = 0; i < size; i++) {
				DcmManagementInterfaceMessage *pSsmMacAclIntfAssocBulkMessage = NULL;
					
				if (phyMOs[i] == NULL) {
					SSM_PLUG_INFO_LVL_S("****phyMOs ith element is NULL in SsmLocalObjectManager***");
					continue;
				}

				if(phyMOs[i]->getPortrole() == false) {
					//SSM_PLUG_INFO_LVL_S( string("Reply configuration Edge Port :") + phyMOs[i]->getIfName());	
					
					pSsmMacAclIntfAssocBulkMessage = SsmLocalObjectManager::ConfigReplayMacAclIntfAssoc(phyMOs[i]);
					
					if (pSsmMacAclIntfAssocBulkMessage) {
						ResourceId status = WAVE_MESSAGE_ERROR;
						status = sendSynchronouslyToWaveClient ("ssm",pSsmMacAclIntfAssocBulkMessage);
						if (status == WAVE_MESSAGE_SUCCESS) {
							SSM_PLUG_INFO_LVL_S("Phy Interface Mac Acl configuration successful ");
						} else {
							SSM_PLUG_ERR_LVL_S(string("Phy Inerface Mac Acl Interface configuration failed - ") + status);
						}
						delete pSsmMacAclIntfAssocBulkMessage;
						pSsmMacAclIntfAssocBulkMessage = NULL;
					} else {
						//SSM_PLUG_INFO_LVL_S(" SsmLocalObjectManager::ConfigReplayMacAclIntf SsmMacAclIntfAssocBulkMessage is NULL ");
					}
				}
			}
				
			for (i = 0; i < size; i++) {
				if (phyMOs[i])
					delete(phyMOs[i]);
			}
		}
        return;
    }

	void SsmLocalObjectManager::ConfigReplayStormControlIntf(SI32 slotNum)
	{
		SSM_PLUG_INFO_LVL("Inside SsmLocalObjectManager:: Replaying StormControlIntf Configuration");
		
		if (slotNum >= 0) 
		{
			vector<PhyIntfLocalManagedObject *> phyMOs;
			NsmUtils::SlotGetPhyIntfMOs(phyMOs, slotNum);

			//Replay the strom control config per interface.
			for (size_t i = 0; i < phyMOs.size(); i++) {
				PhyIntfLocalManagedObject *phyIntfLocalManagedObject = phyMOs[i];
				SsmLocalObjectManager::ConfigReplayStormControlPerInterface(phyIntfLocalManagedObject);
			}

			for (size_t i = 0; i < phyMOs.size(); i++)
				delete (phyMOs[i]);
		}
		return;
	}





	void SsmLocalObjectManager::ConfigReplayStormControlPerInterface(
		PhyIntfLocalManagedObject *phyIntfLocalManagedObject) {

	WaveNs::trace(TRACE_LEVEL_DEBUG,
			"Entering ConfigReplayStormControlPerInterface");
	if (phyIntfLocalManagedObject == NULL) {
		WaveNs::trace(
				TRACE_LEVEL_DEBUG,
				"Returing from ConfigReplayStormControlPerInterface as the phyIntfLocalManagedObject is NULL");
		return;
	}

	WaveNs::trace(
			TRACE_LEVEL_DEBUG,
			"Inside ConfigReplayStormControlPerInterface as the phyIntfLocalManagedObject port role is edge");

	vector<WaveManagedObjectPointer<NsmStormControlIntfConfLocalManagedObject> > bumConfigMO =
			phyIntfLocalManagedObject->getBumConfig();

	string ifName = "";
	UI8 Iftype = phyIntfLocalManagedObject->getPhyType();
	char buffer[25];
	buffer[24] = '\0';
	UI32 bufferSize = sizeof(buffer);

	if (true
			== NsmUtils::isValidThreeTuple(
					phyIntfLocalManagedObject->getIfName()))
		NsmUtils::getTwoTupleIfName(phyIntfLocalManagedObject->getIfName(),
				ifName);
	else
		ifName = phyIntfLocalManagedObject->getIfName();

	if (Iftype == IF_TYPE_TE)
		snprintf(buffer, bufferSize, "te%s", ifName.c_str());
	else if (Iftype == IF_TYPE_GI)
		snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
	else if (Iftype == IF_TYPE_FO)
		snprintf(buffer, bufferSize, "fo%s", ifName.c_str());

	for (size_t j = 0; j < bumConfigMO.size(); j++) {
		NsmStormControlClientIntfConfSetMsg *pNsmStormControlClientIntfConfSetMsg =
				new NsmStormControlClientIntfConfSetMsg;

		ResourceId status = WAVE_MESSAGE_ERROR;

		/* Setting fields in the Message to be sent to backend */
		pNsmStormControlClientIntfConfSetMsg->setIfName(buffer);
		pNsmStormControlClientIntfConfSetMsg->setOpCode(QOS_BUM_DCMD_ENABLE);
		pNsmStormControlClientIntfConfSetMsg->setProtocolType(
				bumConfigMO[j]->getProtocolType());
		pNsmStormControlClientIntfConfSetMsg->setRateFormat(
				bumConfigMO[j]->getRateFormat());
		pNsmStormControlClientIntfConfSetMsg->setAction(
				bumConfigMO[j]->getAction());
		if (bumConfigMO[j]->getRateFormat() == QOS_BUM_RATE_FORMAT_BPS_E)
			pNsmStormControlClientIntfConfSetMsg->setBps(
					bumConfigMO[j]->getBps());
		else if (bumConfigMO[j]->getRateFormat()
				== QOS_BUM_RATE_FORMAT_PRECENTAGE_E)
			pNsmStormControlClientIntfConfSetMsg->setPercentage(
					bumConfigMO[j]->getPercentage());

		if (pNsmStormControlClientIntfConfSetMsg) {
			status = WaveObjectManagerToolKit::sendSynchronouslyToWaveClient(
					"qos", pNsmStormControlClientIntfConfSetMsg);

			if (status == WAVE_MESSAGE_SUCCESS) {
				WaveNs::trace(TRACE_LEVEL_INFO,
						"Phy Interface StormControl configuration successful ");
			} else {
				WaveNs::trace(
						TRACE_LEVEL_INFO,
						string(
								"Phy Interface StormControl configuration not successful")
								+ status);
			}
		}
		delete pNsmStormControlClientIntfConfSetMsg;
	}
}






    const string SsmLocalObjectManager::getPolicyNameByObjId(const ObjectId objId)
    {
        WaveManagedObject   *pWaveManagedObject = NULL;
        MacAclPolicyManagedObject *policyMO = NULL;
        string policyName;

        pWaveManagedObject = WaveObjectManagerToolKit::queryManagedObject (objId);
        if (pWaveManagedObject != NULL) {
            policyMO = dynamic_cast<MacAclPolicyManagedObject *>(pWaveManagedObject);
            policyName = policyMO->getPolicyName();
            delete pWaveManagedObject;
        }

        return (policyName);
    }

    DcmManagementInterfaceMessage *SsmLocalObjectManager::ConfigReplayMacAclIntfAssoc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        ObjectId IngresspolicyObjId                 = ObjectId::NullObjectId;
        ObjectId EgresspolicyObjId                  = ObjectId::NullObjectId;
        UI32 Ingressfound                           = 0;
        UI32 Egressfound                            = 0;
        UI32 NumofRecords                           = 0;
        char buffer[MAX_STR];
		UI32 bufferSize 							= sizeof (buffer);
		SsmMacAclIntfAssocBulkMessage *intfBulkMsg = NULL; 
		
		if (pPhyIntfLocalManagedObject == NULL)
            return (0);

		UI8 Iftype = pPhyIntfLocalManagedObject->getPhyType();
		
        if ((Ingressfound = pPhyIntfLocalManagedObject->getIngressEgressMacAclPolicyObjectId
            (IngresspolicyObjId, SSMACL_BINDING_DIRECTION_IN_E)))
            NumofRecords = NumofRecords + 1 ;
        if ((Egressfound = pPhyIntfLocalManagedObject->getIngressEgressMacAclPolicyObjectId
            (EgresspolicyObjId, SSMACL_BINDING_DIRECTION_OUT_E)))
            NumofRecords = NumofRecords + 1 ;

        if (true  == NsmUtils::isValidThreeTuple(pPhyIntfLocalManagedObject->getId())) {
            string twoTuppleIfName;
            string threeTupple = pPhyIntfLocalManagedObject->getId();
            NsmUtils::getTwoTupleIfName(threeTupple, twoTuppleIfName);
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, twoTuppleIfName.c_str(), buffer, bufferSize);
        } else {
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(Iftype, pPhyIntfLocalManagedObject->getId().c_str(), buffer, bufferSize);
        }

        if(Egressfound) {
			if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS)){
				WaveNs::trace(TRACE_LEVEL_INFO, "Applying ACL policy in egress direction not supported in this platform");
				Egressfound = 0;
				NumofRecords = NumofRecords-1;
			}
		}

        if ((Ingressfound) || (Egressfound)) {
            string IngresspolicyName    = "";
            string EgresspolicyName     = "";
            string ifname               = buffer;
			intfBulkMsg 				= new SsmMacAclIntfAssocBulkMessage();
			
	
            if ((Ingressfound) && (Egressfound)) {
                IngresspolicyName   = SsmLocalObjectManager::getPolicyNameByObjId(IngresspolicyObjId);
                EgresspolicyName    = SsmLocalObjectManager::getPolicyNameByObjId(EgresspolicyObjId);
                intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
                    SSMACL_BINDING_DIRECTION_IN_E);
                intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
                    SSMACL_BINDING_DIRECTION_OUT_E);
            } else if (Ingressfound) {
                IngresspolicyName   = SsmLocalObjectManager::getPolicyNameByObjId(IngresspolicyObjId);
                intfBulkMsg->setIntfDetails(ifname, IngresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
                    SSMACL_BINDING_DIRECTION_IN_E);
            } else if (Egressfound) {
                EgresspolicyName    = SsmLocalObjectManager::getPolicyNameByObjId(EgresspolicyObjId);
                intfBulkMsg->setIntfDetails(ifname, EgresspolicyName, SSMACL_DCMD_BIND_L2_ACL,
                    SSMACL_BINDING_DIRECTION_OUT_E);
            }
            intfBulkMsg->setNumRecords(NumofRecords);
        }
    
		return (intfBulkMsg);
    }

	void SsmLocalObjectManager::ConfigReplayStdMacAclPolicy()
	{
		StdMacAclPolicyManagedObject *stdPolicyMO 	= NULL;
		ResourceId status 							= WAVE_MESSAGE_ERROR;

		SSM_PLUG_DBG_TRACE();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(StdMacAclPolicyManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsPMO 	= querySynchronously (&syncQueryCtxt);

		if (NULL != pResultsPMO) 
		{
			UI32 numberOfResults = pResultsPMO->size ();
			SSM_PLUG_DBG_LVL_S(string("Number of Policies: ") + numberOfResults);
			for (UI32 i = 0; i < numberOfResults; i++) 
			{
				stdPolicyMO = dynamic_cast<StdMacAclPolicyManagedObject *>((*pResultsPMO)[i]);
				vector<WaveManagedObjectPointer<StdMacAclRuleManagedObject> > stdRuleMoVector =  stdPolicyMO->getRules();
				UI64 numRules = stdRuleMoVector.size();
				SSM_PLUG_DBG_LVL_S(string("Number of Rules: ") + numRules);
				SsmMacAclPolicyBulkMessage *aclPolicyBulkMsg = new SsmMacAclPolicyBulkMessage();
				if (aclPolicyBulkMsg) 
				{
					aclPolicyBulkMsg->setPolicyType(SSMACL_TYPE_L2_STD_E);
					aclPolicyBulkMsg->setOpCode(SSMACL_DCMD_CREATE_L2_ACL);
					aclPolicyBulkMsg->setPolicyName(stdPolicyMO->getPolicyName());
					aclPolicyBulkMsg->setNumRules(numRules);

					for (UI64 rindex = 0; rindex <  numRules; rindex++) 
					{
						string stdSrcMac		= "";
						string stdSrcMacMask 	= "";
						UI64 ruleid 			= stdRuleMoVector[rindex]->getRuleid();
						UI32 action				= stdRuleMoVector[rindex]->getAction();
						bool count				= stdRuleMoVector[rindex]->getIsCount();
						bool log				= stdRuleMoVector[rindex]->getIsLog();
						UI32 stdSrcType			= stdRuleMoVector[rindex]->getSrcType();
						
						if (stdSrcType == ADDR_TYPE_ANY) 
						{
							stdSrcMac = "any";
						}
					  	else if (stdSrcType == ADDR_TYPE_HOST)
					  	{
						 	stdSrcMac 		= stdRuleMoVector[rindex]->getSrcHost();
					  	}
						else 
						{
							stdSrcMac 		= stdRuleMoVector[rindex]->getSrcMac();
							stdSrcMacMask 	= stdRuleMoVector[rindex]->getSrcMacMask();
						}
						aclPolicyBulkMsg->setStdRuleParams(ruleid, action, stdSrcMac, stdSrcMacMask, count, log, stdSrcType);
					}
					status = sendSynchronouslyToWaveClient ("ssm",aclPolicyBulkMsg);
					if (status == WAVE_MESSAGE_SUCCESS) 
					{
						SSM_PLUG_SUCCESS_LVL_S(string("Successfully configured: ") + stdPolicyMO->getPolicyName());
					} 
					else 
					{
						SSM_PLUG_ERR_LVL_S(string("Failed to  configure: ") + stdPolicyMO->getPolicyName());
					}
					delete aclPolicyBulkMsg;
				}
			}
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		}
	}

	void SsmLocalObjectManager::ConfigReplayExtMacAclPolicy()
	{
		ExtMacAclPolicyManagedObject *extPolicyMO = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;

		SSM_PLUG_DBG_TRACE();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ExtMacAclPolicyManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

		if (NULL != pResultsPMO) 
		{
			UI32 numberOfResults = pResultsPMO->size ();
			SSM_PLUG_DBG_LVL_S(string("Number of Policies: ") + numberOfResults);
			for (UI32 i = 0; i < numberOfResults; i++) 
			{
				extPolicyMO = dynamic_cast<ExtMacAclPolicyManagedObject *>((*pResultsPMO)[i]);
				vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > extRuleMoVector =  extPolicyMO->getRules();
				UI64 numRules = extRuleMoVector.size();
				SSM_PLUG_DBG_LVL_S(string("Number of Rules: ") + numRules);
				SsmMacAclPolicyBulkMessage *aclPolicyBulkMsg = new SsmMacAclPolicyBulkMessage();
				if (aclPolicyBulkMsg) 
				{
					aclPolicyBulkMsg->setPolicyType(SSMACL_TYPE_L2_EXT_E);
					aclPolicyBulkMsg->setOpCode(SSMACL_DCMD_CREATE_L2_ACL);
					aclPolicyBulkMsg->setPolicyName(extPolicyMO->getPolicyName());
					aclPolicyBulkMsg->setNumRules(numRules);

					for (UI64 rindex = 0; rindex <  numRules; rindex++) 
					{
						string extSrcMac			= "";
						string extSrcMacMask		= "";
						string extDstMac			= "";
						string extDstMacMask		= "";
						string extAppType			= "";
						UI16 extAppTypeInt			= 0;
						EtherType extAppTypeEnum	= ETHER_TYPE_UNKNOWN;
						WaveUnion extAppUnion		= extRuleMoVector[rindex]->getAppTypeUnion();
						UI64 ruleid					= extRuleMoVector[rindex]->getRuleid();
						UI32 action					= extRuleMoVector[rindex]->getAction();
						bool count					= extRuleMoVector[rindex]->getIsCount();
						bool log					= extRuleMoVector[rindex]->getIsLog();
						UI32 extSrcType				= extRuleMoVector[rindex]->getSrcType();
						UI32 extDstType				= extRuleMoVector[rindex]->getDstType();
						
						if (extSrcType == ADDR_TYPE_ANY) 
						{
							extSrcMac = "any";
						} 
						else if (extSrcType == ADDR_TYPE_HOST) 
						{
							extSrcMac = extRuleMoVector[rindex]->getSrcHost();
						} 
						else 
						{
							extSrcMac 		= extRuleMoVector[rindex]->getSrcMac();
							extSrcMacMask 	= extRuleMoVector[rindex]->getSrcMacMask();
						}

						if (extDstType == ADDR_TYPE_ANY) 
						{
							extDstMac = "any";
						} 
						else if (extDstType == ADDR_TYPE_HOST) 
						{
							extDstMac = extRuleMoVector[rindex]->getDstHost();
						} 
						else 
						{
							extDstMac 		= extRuleMoVector[rindex]->getDstMac();
							extDstMacMask 	= extRuleMoVector[rindex]->getDstMacMask();
						}
						
						
						if (extAppUnion.compare("appTypeInt") == 0) 
						{
							if (extRuleMoVector[rindex]->getAppTypeInt()) {
							extAppTypeInt = extRuleMoVector[rindex]->getAppTypeInt();
							extAppType	  = "appTypeInt";
						} 
						} 
						else if (extAppUnion.compare("appType") == 0) 
						{
							if (extRuleMoVector[rindex]->getAppType()) {
							extAppTypeEnum = extRuleMoVector[rindex]->getAppType();
							extAppType    = "appTypeEnum";
						}
						}
						
						aclPolicyBulkMsg->setExtRuleParams(ruleid, action, extSrcMac, extSrcMacMask, 
							extDstMac, extDstMacMask, extAppType, count, log, extSrcType, extDstType, 
							extAppTypeInt, extAppTypeEnum);
					}
					status = sendSynchronouslyToWaveClient ("ssm",aclPolicyBulkMsg);
					if (status == WAVE_MESSAGE_SUCCESS) 
					{
						SSM_PLUG_SUCCESS_LVL_S(string("Successfully configured: ")  + extPolicyMO->getPolicyName());
					} 
					else 
					{
						SSM_PLUG_ERR_LVL_S(string("Failed to configure: ")  + extPolicyMO->getPolicyName());
					}
					delete aclPolicyBulkMsg;
				}
			}
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		}
	}

    void SsmLocalObjectManager::SsmLocalServiceRemMacAclPolicyMessageHandler(SsmLocalServiceRemMacAclPolicyMessage *pMsg)
	{
		SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pMsg);
		const string policyName = pMsg->getPolicyName();
 		UI32 policyType = pMsg->getPolicyType();
 		ResourceId status;

 		SsmMacAclPolicyMessage *msg = new SsmMacAclPolicyMessage(policyName, policyType, DELETE_MAC_ACL_POLICY);
 		if (msg != NULL) 
		{
 			SSM_PLUG_DBG_LVL("SsmMacAclPolicyMessage message being sent to Ssm");
 			status = sendSynchronouslyToWaveClient ("ssm", msg);
 			SSM_PLUG_INFO_LVL_S(string("Remove ACL Policy: status - ") + status + string(",Completion status - ") + msg->getCompletionStatus ()	+ string(",Client status - ") + msg->getClientStatus());
 			if (status == WAVE_MESSAGE_SUCCESS ) 
			{
 				if (msg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) 
				{
 					status = msg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to remove policy on the backend: ") + policyName);
				} 
				else if (msg->getClientStatus() != WAVE_MESSAGE_SUCCESS) 
				{
 					status = msg->getClientStatus();
					SSM_PLUG_ERR_LVL_S(string("Failed to remove policy on the backend: ") + policyName);
				}
			} 
			else 
			{
				SSM_PLUG_ERR_LVL_S(string("Failed to remove policy on the backend: ") + policyName);
			}
 			delete msg;
		} 
		else 
		{
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}
		pMsg->setCompletionStatus(status);	
		reply(pMsg);
	}

    void SsmLocalObjectManager::SsmLocalServiceAddMacAclPolicyMessageHandler(SsmLocalServiceAddMacAclPolicyMessage *pMsg)
	{
		SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pMsg);
		const string policyName = pMsg->getPolicyName();
 		UI32 policyType = pMsg->getPolicyType();
 		ResourceId status;

 		SsmMacAclPolicyMessage *msg = new SsmMacAclPolicyMessage(policyName, policyType, CREATE_MAC_ACL_POLICY);
 		if (msg != NULL) 
		{
 			SSM_PLUG_INFO_LVL_S(string("SsmMacAclPolicyMessage message being sent to Ssm policy: ") + policyName + string ("policy type: ") + policyType);
 			status = sendSynchronouslyToWaveClient ("ssm", msg);
 			//SSM_PLUG_INFO_LVL_S(string("Add ACL Policy: status - ") + status + string(",Completion status - ") + msg->getCompletionStatus ()+ string(",Client status - ") + msg->getClientStatus());

 			if (status == WAVE_MESSAGE_SUCCESS) 
			{
 				if (msg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) 
				{
 					status = msg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				} 
				else if (msg->getClientStatus() != WAVE_MESSAGE_SUCCESS) 
				{
 					status = msg->getClientStatus();

 					if ( status == WRC_SSM_DCM_ERR_ACL_LIMIT )
 					{
 						status = getErrorStatusPerPlatform(status);
 					}
 					SSM_PLUG_ERR_LVL_S(string("Client Status:") + FrameworkToolKit::localize(status));

					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				}
			} 
			else 
			{
				SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
			}
 			delete msg;
		} 
		else 
		{
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}
		
		pMsg->setCompletionStatus(status);	
		reply(pMsg);
	}

    
	void SsmLocalObjectManager::SsmLocalServiceAssocRuleToPolicyMessageHandler(SsmLocalServiceAssocRuleToPolicyMessage *pMsg)
    {
    	DceLinearSeqContext *pDCELinearSeqContext = NULL;

        PrismLinearSequencerStep AddsequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&SsmLocalObjectManager::ssmLocalAssocMacAclRuleToPolicy),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmLocalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SsmLocalObjectManager::prismLinearSequencerFailedStep)
        };
        
        pDCELinearSeqContext = new DceLinearSeqContext (pMsg, this, AddsequencerSteps, sizeof (AddsequencerSteps) /sizeof (AddsequencerSteps[0]));

		if (pDCELinearSeqContext) {
			pDCELinearSeqContext->holdAll ();
			pDCELinearSeqContext->start ();
		}
    }


	void SsmLocalObjectManager::ssmLocalAssocMacAclRuleToPolicy (DceLinearSeqContext *seqContext)
	{
		SSM_PLUG_DBG_TRACE();
        SsmLocalServiceAssocRuleToPolicyMessage *pMsg = reinterpret_cast<SsmLocalServiceAssocRuleToPolicyMessage *>(seqContext->getPPrismMessage());
    	SSM_PLUG_ASSERT_PTR(pMsg);

		UI64 ruleid					= pMsg->getRuleid();
		UI32 action					= pMsg->getAction();
		UI32 policyType				= pMsg->getPolicyType();
		const string policyName		= pMsg->getPolicyName();
		const string srcMac			= pMsg->getSrcMac();
		const string srcMacMask		= pMsg->getSrcMacMask();
		const AddrType srcType		= pMsg->getSrcType();
		const string dstMac			= pMsg->getDstMac();
		const string dstMacMask		= pMsg->getDstMacMask();
		const AddrType dstType		= pMsg->getDstType();
		bool isCount				= pMsg->getIsCount();
		bool isLog					= pMsg->getIsLog();
		const string appType		= pMsg->getAppType();
		const EtherType appTypeEnum = pMsg->getAppTypeEnum();
		const UI16 appTypeInt		= pMsg->getAppTypeInt();
		UI32 opcode					= pMsg->getOpcode();
		UI8	userEnteredSeqid		= pMsg->getUserEnteredSeqid();
		ResourceId status			= WAVE_MESSAGE_ERROR;

        SsmMacAclRuleMessage *newMsg =	new SsmMacAclRuleMessage(policyName, policyType, 
			ruleid, action, srcMac, srcMacMask, srcType, dstMac, dstMacMask, dstType, 
			appType, appTypeEnum, appTypeInt, isCount, isLog, opcode, userEnteredSeqid);
		
        if (newMsg != NULL) 
		{
 			//SSM_PLUG_DBG_LVL_S(string("SsmMacAclRuleMessage message being sent to Ssm - Policyname: ") + policyName + string("policy Type: ") + policyType + string("Ruleid: ") + ruleid + string ("action: ") + action + string ("appType: ")
                //+ appType + string("appTypeEnum: ") + appTypeEnum + string("appTypeInt: ") + appTypeInt);
	
        	status = sendSynchronouslyToWaveClient("ssm", newMsg);
			//if (isLog)
				//SSM_PLUG_INFO_LVL_S("Value of isCount is 1");
			//SSM_PLUG_INFO_LVL_S(string("ACL Rule to Policy: status - ") + status + string(",Completion status - ") + newMsg->getCompletionStatus ()	+ string(",Client status - ") + newMsg->getClientStatus());

 			if (status == WAVE_MESSAGE_SUCCESS ) 
			{
 				if (newMsg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) 
				{
 					status = newMsg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				} 
				else if (newMsg->getClientStatus() != WAVE_MESSAGE_SUCCESS) 
				{
 					status = newMsg->getClientStatus();

 					SSM_PLUG_ERR_LVL_S( string("Client Status:") + FrameworkToolKit::localize(status));

					SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
				} 
				else 
				{
					//SSM_PLUG_INFO_LVL_S(string("New RuleId: ") + newMsg->getRuleid ());
					pMsg->setRuleid(newMsg->getRuleid ());
				}
			} 
			else 
			{
				SSM_PLUG_ERR_LVL_S(string("Failed to add policy on the backend: ") + policyName);
			}
        	delete newMsg;
		} 
		else 
		{
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}
		pMsg->setCompletionStatus(status);	
		
        seqContext->executeNextStep(status);
		return;
	}

    void SsmLocalObjectManager::SsmLocalServiceAssocPolicyToInterfaceMessageHandler(SsmLocalServiceAssocPolicyToInterfaceMessage *pMsg)
	{
		const string policyName = pMsg->getPolicyName();
 		const string ifName = pMsg->getIfName();
 		int status;

 		trace(TRACE_LEVEL_INFO, "SsmLocalServiceAssocPolicyInterfaceMessageHandler: updating protocol daemon");

 		SsmMacAclPolicyMessage *msg = new SsmMacAclPolicyMessage(policyName, ifName, SSMACL_DCMD_BIND_L2_ACL);
 		if (msg != NULL) 
		{
 			trace (TRACE_LEVEL_INFO, "SsmLocalServiceAssocPolicyInterfaceMessageHandler: Message being sent to Ssm");
 			status = sendSynchronouslyToWaveClient ("ssm", msg);
 			if (status == WAVE_MESSAGE_SUCCESS && msg->getClientStatus() == 0) 
			{
 				status = WAVE_MESSAGE_SUCCESS;
			} 
			else 
			{
				status = WAVE_MESSAGE_ERROR;
			}
 			delete msg;
 			trace(TRACE_LEVEL_INFO, "SsmLocalServiceAssocPolicyInterfaceMessageHandler: Successfully protocol daemon");
		} 
		else 
		{
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "SsmLocalServiceAssocPolicyInterfaceMessageHandler: Message creation failed");
		}
		pMsg->setCompletionStatus(status);	
		reply(pMsg);
	}

#if 0	
	void
    SsmLocalObjectManager::SsmMacAclLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        SsmMacAclLocalClearMessage              *req_msg_p = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p = NULL;
		ResourceId sendToClusterCompletionStatus = WAVE_MESSAGE_SUCCESS;

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<SsmMacAclLocalClearMessage *>
                    (seq_ctx_p->getPPrismMessage ());
        
        resp_msg_p =  dynamic_cast<SsmMacAclLocalClearMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        SSM_PLUG_ASSERT_PTR(req_msg_p);
        SSM_PLUG_ASSERT_PTR(resp_msg_p);
       
        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;
		
        for (i = 0; i < locations.size(); i++) {
            if (i ==0) { 
                SSM_PLUG_INFO_LVL_S(string("locations:") + locations[i]);
            } else { 
                SSM_PLUG_INFO_LVL_S(string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++) {
            
            if (WAVE_MESSAGE_SUCCESS == ctx_p->getSendStatusForPhase1(locations[i])) {
                SSM_PLUG_INFO_LVL_S
					(string("WAVE_MESSAGE_SUCCESS == ctx_p->getSendStatusForPhase1(") +
					 locations[i] + string(")"));
            } else {
                SSM_PLUG_ERR_LVL_S(string("Response processing: The location: ")
                    + locations[i] + string("returned error: ")  +
                FrameworkToolKit::localize(ctx_p->getSendStatusForPhase1(locations[i])));
                sendToClusterCompletionStatus = ctx_p->getSendStatusForPhase1(locations[i]);
                req_msg_p->setCompletionStatus(ctx_p->getSendStatusForPhase1(locations[i]));
				continue;
            }

            if (FRAMEWORK_SUCCESS == ctx_p->getFrameworkStatusForPhase1(locations[i])) {
                SSM_PLUG_INFO_LVL_S
					(string("FRAMEWORK_SUCCESS == ctx_p->getFrameworkStatusForPhase1(") + 
					 locations[i] + string(")"));
            } else {
                SSM_PLUG_ERR_LVL_S(string("Response processing: The location: ")
                    + locations[i] + string("returned error: ")  +
                FrameworkToolKit::localize(ctx_p->getFrameworkStatusForPhase1(locations[i])));
                sendToClusterCompletionStatus = ctx_p->getFrameworkStatusForPhase1(locations[i]);
                req_msg_p->setCompletionStatus(ctx_p->getFrameworkStatusForPhase1(locations[i]));
				continue;
            }

	
            sw_resp_p = dynamic_cast<SsmMacAclLocalClearMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            SSM_PLUG_ASSERT_PTR(sw_resp_p);
            
			
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                SSM_PLUG_INFO_LVL_S(string("WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus() "));
            } else {
				
                 SSM_PLUG_ERR_LVL_S(string("Response processing: The location: ") 
                            + locations[i] + string("returned error: ")  + 
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus())); 
				 sendToClusterCompletionStatus = sw_resp_p->getCompletionStatus();
				 req_msg_p->setCompletionStatus(sw_resp_p->getCompletionStatus());
            }
        }

        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }
    

    void
    SsmLocalObjectManager::sendMacAclClearCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        SsmMacAclLocalClearMessage        	*msg_p = NULL;
        SsmMacAclLocalClearMessage        	*loc_msg_p = NULL;
        WaveSendToClusterContext    	  	*send_ctx_p = NULL;
        bool                        		sendToLocal = false;
        UI32                        		locationId = 0;
        SI32                        		mappedId = 0;
        vector<UI32>                		locations;

  
		msg_p = dynamic_cast<SsmMacAclLocalClearMessage *>
            (seq_ctx_p->getPPrismMessage ());

        SSM_PLUG_ASSERT_PTR(msg_p);
		loc_msg_p = new SsmMacAclLocalClearMessage();
        loc_msg_p->setIfName(msg_p->getIfName());
        loc_msg_p->setL2aclPolicyName(msg_p->getL2aclPolicyName());
        loc_msg_p->setIfType(msg_p->getIfType());
        loc_msg_p->setCmdCode(msg_p->getCmdCode());
        loc_msg_p->setIsGwRequest(false);

        
		SSM_PLUG_ASSERT_PTR(loc_msg_p);

        SSM_PLUG_INFO_LVL_S(string("Sending Clear Command to Cluster"));
        SSM_PLUG_INFO_LVL_S(string("	CmdCode =") + msg_p->getCmdCode() + 
                        string ("	L2aclPolicyName =")  + msg_p->getL2aclPolicyName() + 
						string ("	ifName =") + msg_p->getIfName() +
						string ("   ifType is=" ) + msg_p->getIfType());

       	/* Check Validity of mapped id before sending any msg */
		if ((msg_p->getIfType() == PHY_INTF_TE_TYPE) || (msg_p->getIfType() == PHY_INTF_GI_TYPE)) {
			
			if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  {
				sendToLocal = true;
			} else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  {
				/* Send to the targetted LocationId pointed to by the
				 * mappedId.
				 */
				sendToLocal = false;
				mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
				locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
				if (!locationId) { 
					msg_p->setCompletionStatus (ACL_CLEAR_STATS_RC_NO_IF);
					seq_ctx_p->executeNextStep (ACL_CLEAR_STATS_RC_NO_IF);
					delete loc_msg_p;
					return;
				} else {
					locations.push_back(locationId);
				}
			}
		}
		send_ctx_p = new WaveSendToClusterContext(this, 
			reinterpret_cast<PrismAsynchronousCallback>
			(&SsmLocalObjectManager::SsmMacAclLocalClearSendToClusterCallback),
			seq_ctx_p);
		SSM_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p) { 
            return;
        }
        
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
		
        if (0 == msg_p->getIfName().length()) {
            /* If the interface context is not there, send it to all nodes.
             * In case of adding a new cli which doesn't have interface
             * context make necessary changes in this code block
             */
        } else {
            if ((msg_p->getIfType() == PHY_INTF_TE_TYPE) || (msg_p->getIfType() == PHY_INTF_GI_TYPE)) {
				if (!sendToLocal) 
					send_ctx_p->setLocationsToSendToForPhase1(locations);
            } else if (msg_p->getIfType () == PO_INTF_TYPE) {
                 NsmUtils::setLocationIdsForPoSubModeCmds(msg_p->getIfName(),send_ctx_p);
            } else if (msg_p->getIfType () == VLAN_INTF_TYPE) {
            } else {
                /* This code block doesn't hit ideally */
            }
        }
        if (sendToLocal) {
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) {
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
        
		sendToWaveCluster(send_ctx_p);

        return;
    }


    ResourceId 
    SsmLocalObjectManager::sendClearToSsmDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        SsmMacAclLocalClearMessage *msg_p = dynamic_cast<SsmMacAclLocalClearMessage *>
                                      (ctx_p->getPPrismMessage());
        AclClearStatsMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = ""; 
		cl_msg_p = new AclClearStatsMessage ();
		
		if (cl_msg_p != NULL) {
            char buffer[25];
            buffer[24] = '\0';
            UI32 bufferSize = sizeof (buffer);
            if ((msg_p->getIfType() == PHY_INTF_TE_TYPE) || (msg_p->getIfType() == PHY_INTF_GI_TYPE)) {
				/* Three tuple to two tuple conversion */
                trace (TRACE_LEVEL_INFO,"Inside (msg_p->getIfType() == PHY_INTF_TE_TYPE) || (msg_p->getIfType() == PHY_INTF_GI_TYPE");
				if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) {
                    NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
                } else {
                    ifName = msg_p->getIfName();
                }
                if(msg_p->getIfType() == PHY_INTF_TE_TYPE)
                    snprintf(buffer, bufferSize, "te%s", ifName.c_str());
                else if(msg_p->getIfType() == PHY_INTF_GI_TYPE)
                    snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
            } else if (msg_p->getIfType () == PO_INTF_TYPE) {
                snprintf(buffer, bufferSize, "po%s", msg_p->getIfName ().c_str());
            } else if (msg_p->getIfType () == VLAN_INTF_TYPE) {
                snprintf(buffer, bufferSize, "vlan0.%s", msg_p->getIfName ().c_str());
            } else {
                snprintf(buffer, bufferSize, "%s", msg_p->getIfName ().c_str());
            }
            mappedId = DcmToolKit::getLocalMappedId();

            if (mappedId == (-1)) {
                mappedId = 0;
            }

			cl_msg_p->setGrpName(msg_p->getL2aclPolicyName ());
			cl_msg_p->setIfName(buffer);
            cl_msg_p->setCmdCode(msg_p->getCmdCode());
            cl_msg_p->setMappedId(mappedId);

            trace (TRACE_LEVEL_INFO, "sendToLocalNode: Message being sent to Ssm");

            status = sendSynchronouslyToWaveClient ("ssm", cl_msg_p);

            if (status == WAVE_MESSAGE_SUCCESS) {
                if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_INFO,
                        string("Resp: Client Completion Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                        FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                    
					msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p){
                        delete cl_msg_p;
                    }
                    return status;
				}

                if (cl_msg_p->getClientStatus() != 0) {
                    trace(TRACE_LEVEL_INFO,
                        string("Resp:  Client Error MsgId: ") +
                        cl_msg_p->getMessageId() +
                        FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                    msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p){
                        delete cl_msg_p;
                    }
                    return status;
                }
            }

            if (status != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_INFO,
                    string("Client Message sending failed for MsgId: ") +
                    cl_msg_p->getMessageId() +
                    string("reason:") +  FrameworkToolKit::localize(status));
                if(cl_msg_p){
                    delete cl_msg_p;
                    }
                return status;
            }
            trace(TRACE_LEVEL_INFO,
                string("Client Message processing succesful for MsgId: ") +
                cl_msg_p->getMessageId());

            trace(TRACE_LEVEL_INFO, "sendToLocalNode: Successfully protocol daemon");

            status = cl_msg_p->getCompletionStatus();
            if(cl_msg_p){
                delete cl_msg_p;
            }
            return status;
        } else {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, "sendToLocalNode: Message creation failed");
        }
        
		if(cl_msg_p){
            delete cl_msg_p;
        }
        return status;
    }

    void  
    SsmLocalObjectManager::SsmMacAclLocalClearMessageHandler (
            SsmMacAclLocalClearMessage *pSsmMacAclLocalClearMessage)
    {

        if (pSsmMacAclLocalClearMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&SsmLocalObjectManager::sendMacAclClearCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&SsmLocalObjectManager::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&SsmLocalObjectManager::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = 
                new PrismLinearSequencerContext(pSsmMacAclLocalClearMessage, 
                    this, sequencerSteps, 
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else { 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&SsmLocalObjectManager::sendClearToSsmDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           				(&SsmLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           				(&SsmLocalObjectManager::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                        pSsmMacAclLocalClearMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }
#endif 	
	void SsmLocalObjectManager::SsmLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        SsmMacAclShowLocalSPMessage             *req_msg_p = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p = NULL;

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        req_msg_p = dynamic_cast<SsmMacAclShowLocalSPMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p =  dynamic_cast<SsmMacAclShowLocalSPMessage *> (ctx_p->getPPrismMessageForPhase1());
		prismAssert (req_msg_p != NULL, __FILE__, __LINE__);
		prismAssert (resp_msg_p != NULL, __FILE__, __LINE__);
       
        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        trace(TRACE_LEVEL_INFO,	string("Response processing: Num resp:") + locations.size());

        for (i = 0; i < locations.size(); i++) 
		{
            if (i ==0) 
			{ 
                trace(TRACE_LEVEL_INFO, string("locations:") + locations[i]);
            } 
			else 
			{ 
                trace(TRACE_LEVEL_INFO, string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++) 
		{
            sw_resp_p = dynamic_cast<SsmMacAclShowLocalSPMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));

            if (WAVE_MESSAGE_SUCCESS == ctx_p->getSendStatusForPhase1(locations[i])) {
                SSM_PLUG_INFO_LVL_S
					(string("WAVE_MESSAGE_SUCCESS == ctx_p->getSendStatusForPhase1(") + 
					 locations[i] + string(")"));
            } else {
                SSM_PLUG_ERR_LVL_S(string("Response processing: The location: ")
                    + locations[i] + string("returned error: ")  +
                FrameworkToolKit::localize(ctx_p->getSendStatusForPhase1(locations[i])));
                //sendToClusterCompletionStatus = ctx_p->getSendStatusForPhase1(locations[i]);
                //req_msg_p->setCompletionStatus(ctx_p->getSendStatusForPhase1(locations[i]));
                continue;
            }

            if (FRAMEWORK_SUCCESS == ctx_p->getFrameworkStatusForPhase1(locations[i])) {
                SSM_PLUG_INFO_LVL_S
					(string("FRAMEWORK_SUCCESS == ctx_p->getFrameworkStatusForPhase1(") +
					 locations[i] + string(")"));
            } else {
                SSM_PLUG_ERR_LVL_S(string("Response processing: The location: ")
                    + locations[i] + string("returned error: ")  +
                FrameworkToolKit::localize(ctx_p->getFrameworkStatusForPhase1(locations[i])));
               	//sendToClusterCompletionStatus = ctx_p->getFrameworkStatusForPhase1(locations[i]);
                //req_msg_p->setCompletionStatus(ctx_p->getFrameworkStatusForPhase1(locations[i]));
                continue;
            }
			
			prismAssert (sw_resp_p != NULL, __FILE__, __LINE__);
            
	
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) 
			{
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } 
			else 
			{
				trace(TRACE_LEVEL_INFO, string("Response processing: The location: ") + locations[i] + string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus())); 
            }
        }
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        delete resp_msg_p;
        delete ctx_p;
        return;
    }
    
    void  SsmLocalObjectManager::sendShowCommandToCluster ( PrismLinearSequencerContext *seq_ctx_p)
    {
        SsmMacAclShowLocalSPMessage        *msg_p = NULL;
        SsmMacAclShowLocalSPMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext    	   *send_ctx_p = NULL;
        UI32                        	   locationId = 0;
        SI32                        	   mappedId = 0;
        vector<UI32>               		   locations;
        bool                   		       sendToLocal = false;

        msg_p = dynamic_cast<SsmMacAclShowLocalSPMessage *> (seq_ctx_p->getPPrismMessage ());
		prismAssert (msg_p != NULL, __FILE__, __LINE__);
		
        loc_msg_p = new SsmMacAclShowLocalSPMessage();
		loc_msg_p->setIfName(msg_p->getIfName());
		loc_msg_p->setBinding(msg_p->getBinding());
		loc_msg_p->setDetail(msg_p->getDetail());
		loc_msg_p->setPolicyName(msg_p->getPolicyName());
		loc_msg_p->setPolicyType(msg_p->getPolicyType());
		loc_msg_p->setIfType(msg_p->getIfType());
		loc_msg_p->setOpcode(msg_p->getOpCode());
		loc_msg_p->setIsGwRequest(false);
		
		prismAssert (loc_msg_p != NULL, __FILE__, __LINE__);

        trace (TRACE_LEVEL_INFO, string("Sending Show Command to Cluster") + string(" Opcode =") + msg_p->getOpCode() + string ("ifName =")  + msg_p->getIfName()  + string ("binding =") + msg_p->getBinding() + string("detail =") + msg_p->getDetail() + string("PolicyName =") + msg_p->getPolicyName());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&SsmLocalObjectManager::SsmLocalShowSendToClusterCallback), seq_ctx_p);

		prismAssert (send_ctx_p != NULL, __FILE__, __LINE__);

        if (!send_ctx_p) 
		{ 
            return;
        }
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        
		if (0 == msg_p->getIfName().length()) 
		{ 
            /* If the interface context is not there, send it to all nodes.
			 * In case of adding a new cli which doesn't have interface
			 * context make necessary changes in this code block
			 */
		} 
		else 
		{
			if (msg_p->getIfType() == PHY_INTF_TYPE) 
			{
				if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  
				{
					sendToLocal = true;
				} 
				else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))  
				{
					/* Send to the targetted LocationId pointed to by the 
					 * mappedId. 
					 */
					sendToLocal = false;
					mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
					locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
					locations.push_back(locationId);
					send_ctx_p->setLocationsToSendToForPhase1(locations);
				}
			} 
			else if (msg_p->getIfType () == PO_INTF_TYPE) 
			{
			} 
			else if (msg_p->getIfType () == VLAN_INTF_TYPE) 
			{
			} 
			else 
			{
				/* This code block doesn't hit ideally */
			}
		}

        if (sendToLocal) 
		{
            locationId = NsmUtils::getLocalLocationId();
            if (locationId) 
			{ 
               locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
        sendToWaveCluster(send_ctx_p);
        return;
    }

    ResourceId SsmLocalObjectManager::sendShowToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p)
    {
		SsmMacAclShowLocalSPMessage *msg_p = dynamic_cast<SsmMacAclShowLocalSPMessage *> (ctx_p->getPPrismMessage ());
					
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = ""; 

		SsmMacAclShowMessage *cl_msg_p = new SsmMacAclShowMessage();
 		if (cl_msg_p != NULL) 
		{
 			char buffer[25];
			buffer[24] = '\0';
 		    UI32 bufferSize = sizeof (buffer);	
			cl_msg_p->setPolicyName(msg_p->getPolicyName ());
 			if ((msg_p->getIfType() == PHY_INTF_TE_TYPE) 
				|| (msg_p->getIfType() == PHY_INTF_GI_TYPE)) {
				/* Three tuple to two tuple conversion */
				if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) 
				{
					NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
				} 
				else 
				{
					ifName = msg_p->getIfName();
				}
			if(msg_p->getIfType() == PHY_INTF_TE_TYPE)
				snprintf(buffer, bufferSize, "te%s", ifName.c_str());
			else if(msg_p->getIfType() == PHY_INTF_GI_TYPE)
				snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
			msg_p->setIfType(PHY_INTF_TYPE);
			} else if (msg_p->getIfType () == PO_INTF_TYPE) {
				snprintf(buffer, bufferSize, "po%s", msg_p->getIfName ().c_str());
			} else if (msg_p->getIfType () == VLAN_INTF_TYPE) {
				snprintf(buffer, bufferSize, "vlan0.%s", msg_p->getIfName ().c_str());
			} else {
				snprintf(buffer, bufferSize, "%s", msg_p->getIfName ().c_str());
			} 
			mappedId = DcmToolKit::getLocalMappedId();
			
			if (mappedId == (-1)) 
			{ 
				mappedId = 0;
			}
			cl_msg_p->setIfType(msg_p->getIfType ());
			cl_msg_p->setIfName(buffer);
			cl_msg_p->setOpcode(msg_p->getOpCode ());
			cl_msg_p->setBinding(msg_p->getBinding());
			cl_msg_p->setDetail(msg_p->getDetail());
			cl_msg_p->setMappedId(mappedId);

			trace (TRACE_LEVEL_INFO, "sendToLocalNode: Message being sent to Ssm");

			status = sendSynchronouslyToWaveClient ("ssm", cl_msg_p);
			
			if (status == WAVE_MESSAGE_SUCCESS) 
			{
				if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) 
				{
					trace(TRACE_LEVEL_INFO, string("Resp: Client Completion Error MsgId: ") + cl_msg_p->getMessageId() +  FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
					msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
					//bpatel: To avoid memory leak
					status = msg_p->getCompletionStatus();
					if(cl_msg_p)
					{
						delete cl_msg_p;
					}
					return status;
				}
				
				if (cl_msg_p->getClientStatus() != 0) 
				{ 
					trace(TRACE_LEVEL_INFO,	string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
					msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
					//bpatel: To avoid memory leak
					status = msg_p->getCompletionStatus();
					if(cl_msg_p)
					{
						delete cl_msg_p;
					}
					return status;
				}
			}
			
			if (status != WAVE_MESSAGE_SUCCESS) 
			{
				trace(TRACE_LEVEL_INFO, string("Client Message sending failed for MsgId: ") + cl_msg_p->getMessageId() +  string("reason:") +  FrameworkToolKit::localize(status));
				//bpatel: To avoid memory leak
				if(cl_msg_p)
				{
					delete cl_msg_p;
				}
				return status;
			}
			
			trace(TRACE_LEVEL_INFO, string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());
			
			msg_p->copyAllBuffers(*cl_msg_p);
			trace(TRACE_LEVEL_INFO, "sendToLocalNode: Successfully protocol daemon");
			
			//bpatel: To avoid memory leak
			status = cl_msg_p->getCompletionStatus();
			if(cl_msg_p)
			{
				delete cl_msg_p;
			}
			return status;
		} 
		else 
		{
			status = WAVE_MESSAGE_ERROR;
			trace (TRACE_LEVEL_ERROR, "sendToLocalNode: Message creation failed");
		}
		//bpatel: to avoid memory leak
		if(cl_msg_p)
		{
			delete cl_msg_p;
		}
		return status;
	}

    void SsmLocalObjectManager::SsmMacAclShowLocalSPMessageHandler (SsmMacAclShowLocalSPMessage *pSsmMacAclShowLocalSPMessage)
    {
        if (pSsmMacAclShowLocalSPMessage->getIsGwRequest()) 
		{
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalObjectManager::sendShowCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalObjectManager::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&SsmLocalObjectManager::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pSsmMacAclShowLocalSPMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->holdAll();
            pPrismLinearSequencerContext->start ();
            return;
        } 
		else
		{ 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalObjectManager::sendShowToSsmDaemon),
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SsmLocalObjectManager::prismSynchronousLinearSequencerFailedStep) 
            };
            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(pSsmMacAclShowLocalSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();
        }
    }
	
	void SsmLocalObjectManager::SsmLocalServiceResqMacAclPolicyMessageHandler(SsmLocalServiceResqMacAclPolicyMessage *pMsg)
	{
		SSM_PLUG_DBG_TRACE();
    	SSM_PLUG_ASSERT_PTR(pMsg);
		UI64 seqId = pMsg->getSeqId();
		UI64 incSeqId = pMsg->getIncSeqId();
		const string policyName = pMsg->getPolicyName();
		ResourceId status = WAVE_MESSAGE_ERROR;

        SsmMacAclReseqMessage *newMsg =	new SsmMacAclReseqMessage(policyName, seqId,incSeqId);
        if (newMsg != NULL) 
		{
 			SSM_PLUG_DBG_LVL_S(string("SsmMacAclReseqMessage message being sent to Ssm - Policyname: ") + policyName + string("Seq Id: ") + seqId + string("Inc Seq Id: ") + incSeqId );
        	status = sendSynchronouslyToWaveClient("ssm", newMsg);
 			SSM_PLUG_INFO_LVL_S(string("ACL Resequence Rule : status - ") + status + string(",Completion status - ") + newMsg->getCompletionStatus ()+ string(",Client status - ") + newMsg->getClientStatus());

 			if (status == WAVE_MESSAGE_SUCCESS ) 
			{
 				if (newMsg->getCompletionStatus () != WAVE_MESSAGE_SUCCESS) 
				{
 					status = newMsg->getCompletionStatus ();
					SSM_PLUG_ERR_LVL_S(string("Failed to resequence policy on the backend: ") + policyName);
				} 
				else if (newMsg->getClientStatus() != WAVE_MESSAGE_SUCCESS) 
				{
 					status = newMsg->getClientStatus();
					SSM_PLUG_ERR_LVL_S(string("Failed to resequence  policy on the backend: ") + policyName);
				} 
				else 
				{
					SSM_PLUG_INFO_LVL_S(string("ACL Rule Sequencing is success: "));
				}
			} 
			else 
			{
				SSM_PLUG_ERR_LVL_S(string("Failed to resequence policy on the backend: ") + policyName);
			}
        	delete newMsg;
		} 
		else 
		{
			status = WAVE_MESSAGE_ERROR;
			SSM_PLUG_ERR_LVL("Message creation failed");
		}
		
		pMsg->setCompletionStatus(status);	
		reply(pMsg);
	}

	ResourceId SsmLocalObjectManager::getErrorStatusPerPlatform (ResourceId status)
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
