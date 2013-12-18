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
#include "Nsm/Global/VlanClassifierGlobalConfWorker.h"
#include "Nsm/Global/NsmGlobalCreateVlanClassifierRuleMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanClassifierRuleMessage.h"
#include "Nsm/Global/NsmGlobalUpdateVlanClassifierGroupMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanClassifierGroupMessage.h"
#include "Nsm/Global/NsmGlobalAddVlanClassifierToPoMessage.h"
#include "Nsm/Global/NsmGlobalDeleteVlanClassifierToPoMessage.h"
#include "Nsm/Global/VlanClassifierRulesManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanClassifierGroupAssocRulesManagedObject.h"
#include "Nsm/Global/VlanClassifierActivateGlobalManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"

//Local
#include "Nsm/Local/NsmCreateVlanClassifierRuleSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierRuleSPMessage.h"
#include "Nsm/Local/NsmUpdateVlanClassifierGroupSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierGroupSPMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierToPoSPMessage.h"
#include "Nsm/Local/NsmAddVlanClassifierToPoSPMessage.h"
#include "Nsm/Local/VlanClassifierActivateLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmAddVlanClassifierToPhyMessage.h"
#include "Nsm/Local/NsmDeleteVlanClassifierToPoMessage.h"

//ClientInterface
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Utils/DceClusterUtils.h"

namespace DcmNs
{
    VlanClassifierGlobalConfWorker::VlanClassifierGlobalConfWorker(NsmGlobalObjectManager * pNsmGlobalObjectManager):
        WaveWorker(pNsmGlobalObjectManager)
    {
        VlanClassifierRulesManagedObject           VlanClassifierRulesManagedObject(pNsmGlobalObjectManager);
        VlanClassifierGroupAssocRulesManagedObject VlanClassifierGroupAssocRulesManagedObject(pNsmGlobalObjectManager);

        VlanClassifierRulesManagedObject.setupOrm();
        addManagedClass(VlanClassifierRulesManagedObject::getClassName(), this);
        VlanClassifierGroupAssocRulesManagedObject.setupOrm();
        addManagedClass(VlanClassifierGroupAssocRulesManagedObject::getClassName(), this);

        VlanClassifierActivateGlobalManagedObject VlanClassifierActivateGlobalManagedObject(pNsmGlobalObjectManager);
        VlanClassifierActivateGlobalManagedObject.setupOrm();
		
        addManagedClass(VlanClassifierActivateGlobalManagedObject::getClassName(), this);
        addOperationMap(NSMGLOBALCREATEVLANCLASSIFIERRULE, reinterpret_cast<PrismMessageHandler>(&VlanClassifierGlobalConfWorker::NsmGlobalCreateVlanClassifierRuleMessageHandler));
        addOperationMap(NSMGLOBALDELETEVLANCLASSIFIERRULE, reinterpret_cast<PrismMessageHandler>(&VlanClassifierGlobalConfWorker::NsmGlobalDeleteVlanClassifierRuleMessageHandler));
        addOperationMap(NSMGLOBALUPDATEVLANCLASSIFIERGROUP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierGlobalConfWorker::NsmGlobalUpdateVlanClassifierGroupMessageHandler));
        addOperationMap(NSMGLOBALDELETEVLANCLASSIFIERGROUP, reinterpret_cast<PrismMessageHandler>(&VlanClassifierGlobalConfWorker::NsmGlobalDeleteVlanClassifierGroupMessageHandler));
        addOperationMap(NSMGLOBALADDVLANCLASSIFIERTOPO, reinterpret_cast<PrismMessageHandler>(&VlanClassifierGlobalConfWorker::NsmGlobalAddVlanClassifierToPoMessageHandler));
        addOperationMap(NSMGLOBALDELETEVLANCLASSIFIERTOPO, reinterpret_cast<PrismMessageHandler>(&VlanClassifierGlobalConfWorker::NsmGlobalDeleteVlanClassifierToPoMessageHandler));
    }
    VlanClassifierGlobalConfWorker::~VlanClassifierGlobalConfWorker()
    {
    }
    PrismMessage * VlanClassifierGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage* pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMGLOBALCREATEVLANCLASSIFIERRULE:
                pPrismMessage = new NsmGlobalCreateVlanClassifierRuleMessage();

                break;

            case NSMGLOBALDELETEVLANCLASSIFIERRULE:
                pPrismMessage = new NsmGlobalDeleteVlanClassifierRuleMessage();

                break;

            case NSMGLOBALUPDATEVLANCLASSIFIERGROUP:
                pPrismMessage = new NsmGlobalUpdateVlanClassifierGroupMessage();

                break;

            case NSMGLOBALDELETEVLANCLASSIFIERGROUP:
                pPrismMessage = new NsmGlobalDeleteVlanClassifierGroupMessage();

                break;

            case NSMGLOBALADDVLANCLASSIFIERTOPO:
                pPrismMessage = new NsmGlobalAddVlanClassifierToPoMessage();

                break;

            case NSMGLOBALDELETEVLANCLASSIFIERTOPO:
                pPrismMessage = new NsmGlobalDeleteVlanClassifierToPoMessage();

                break;

            default:
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
    WaveManagedObject * VlanClassifierGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject* pWaveManagedObject = NULL;

        if ((VlanClassifierRulesManagedObject::getClassName()) == managedClassName)
        {
            pWaveManagedObject = new VlanClassifierRulesManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
        }
        else
        {
            if ((VlanClassifierGroupAssocRulesManagedObject::getClassName()) == managedClassName)
            {
                pWaveManagedObject = new VlanClassifierGroupAssocRulesManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
            }
            else
            {
                if ((VlanClassifierActivateGlobalManagedObject::getClassName()) == managedClassName)
                {
                    pWaveManagedObject = new VlanClassifierActivateGlobalManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
                }
                else
                {
                    trace(TRACE_LEVEL_FATAL, "VlanClassifierGlobalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
                }
            }
        }

        return (pWaveManagedObject);
    }
    void VlanClassifierGlobalConfWorker::ActivateVlanClassfierOnPoInterfaceStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
		WaveSendToClusterContext    *send_ctx_p;

        NsmGlobalAddVlanClassifierToPoMessage* pNsmGlobalAddVlanClassifierToPoMessage =
            dynamic_cast<NsmGlobalAddVlanClassifierToPoMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
		string PoName = pNsmGlobalAddVlanClassifierToPoMessage -> getIfName();
        NsmAddVlanClassifierToPoSPMessage* m = new NsmAddVlanClassifierToPoSPMessage();

        m -> setId(pNsmGlobalAddVlanClassifierToPoMessage -> getId());
        m -> setIfName(pNsmGlobalAddVlanClassifierToPoMessage -> getIfName());
        m -> setIfIndex(pNsmGlobalAddVlanClassifierToPoMessage -> getIfIndex());
        m -> setGroupId(pNsmGlobalAddVlanClassifierToPoMessage -> getGroupId());
        m -> setVlanId(pNsmGlobalAddVlanClassifierToPoMessage -> getVlanId());
        m -> setOpcode(pNsmGlobalAddVlanClassifierToPoMessage -> getOpcode());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&VlanClassifierGlobalConfWorker::
				                                  VlanClassifierGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		NSM_PLUG_ASSERT(send_ctx_p);
		if(PORT_NOT_PART_OF_ANY_INTERFACE == NsmUtils::setLocationIdsForPoSubModeCmds(PoName, send_ctx_p))
		{
			NSM_PLUG_DBG("Port not part of any interface");
			pPrismLinearSequencerContext -> executeNextStep(WAVE_MESSAGE_SUCCESS);
			delete m;
			delete send_ctx_p;
			return;
		}	
		send_ctx_p->setPPrismMessageForPhase1(m);
        setClusterContextFlags(send_ctx_p); 
		sendToWaveCluster(send_ctx_p);
        return ;
    }

	
    void VlanClassifierGlobalConfWorker::ActivateVlanClassfierMOStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ObjectId             PoOid;
        ObjectId             VlanOid;

        NsmGlobalAddVlanClassifierToPoMessage* pNsmGlobalAddVlanClassifierToPoMessage =
            dynamic_cast<NsmGlobalAddVlanClassifierToPoMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());

        VlanClassifierActivateGlobalManagedObject* m = new VlanClassifierActivateGlobalManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
		NSM_PLUG_ASSERT(m);
		
        m -> setGroupId(pNsmGlobalAddVlanClassifierToPoMessage -> getGroupId());

        UI32 VlanName= pNsmGlobalAddVlanClassifierToPoMessage -> getVlanId();
		
		NsmUtils::getPoIntfObjectIdByPoIfName(pNsmGlobalAddVlanClassifierToPoMessage -> getIfName(), PoOid, true, pPrismLinearSequencerContext);
		NsmUtils::getVlanIntfObjectIdByVlanIfName(VlanName, VlanOid, true, pPrismLinearSequencerContext);
		
        m -> setVlanId(VlanOid);
        m -> setdummy(0);
		m -> setIfName(pNsmGlobalAddVlanClassifierToPoMessage -> getIfName());
		m -> setOwnerManagedObjectId(PoOid);

		addToComposition(PoIntfManagedObject::getClassName(), VlanClassifierActivateGlobalManagedObject::getClassName(),
												                     "vlanClassifierActivate", PoOid, m -> getObjectId());

		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(m);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

        return; 
    }
    void VlanClassifierGlobalConfWorker::NsmGlobalAddVlanClassifierToPoMessageHandler(NsmGlobalAddVlanClassifierToPoMessage * pNsmGlobalAddVlanClassifierToPoMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::ActivateVlanClassfierOnPoInterfaceStep),
     	    reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerStartTransactionStep), 
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::ActivateVlanClassfierMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerSucceededStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext* pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmGlobalAddVlanClassifierToPoMessage, this,
                                                                                              sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		
        pPrismLinearSequencerContext->holdAll();  
        pPrismLinearSequencerContext->start ();
    }
    void VlanClassifierGlobalConfWorker::VlanClassifierRuleCreationStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        WaveSendToClusterContext    *send_ctx_p;

        NsmGlobalCreateVlanClassifierRuleMessage* pNsmGlobalCreateVlanClassifierRuleMessage =
            dynamic_cast<NsmGlobalCreateVlanClassifierRuleMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
        NsmCreateVlanClassifierRuleSPMessage* m      = new NsmCreateVlanClassifierRuleSPMessage();
		NSM_PLUG_ASSERT(m);

        m -> setOpcode(pNsmGlobalCreateVlanClassifierRuleMessage -> getOpcode());
        m -> setOpcode(pNsmGlobalCreateVlanClassifierRuleMessage -> getOpcode());
        m -> setRuleId(pNsmGlobalCreateVlanClassifierRuleMessage -> getRuleId());
        m -> setMac(pNsmGlobalCreateVlanClassifierRuleMessage -> getMac());

        if (pNsmGlobalCreateVlanClassifierRuleMessage -> getProtoValEnum() != PROTO_TYPE_UNKNOWN)
        {
            m -> setProtoVal(pNsmGlobalCreateVlanClassifierRuleMessage -> getProtoValEnum());
        }
        else
        {
            string protoValStr;

            protoValStr = pNsmGlobalCreateVlanClassifierRuleMessage -> getProtoValStr();
			NSM_PLUG_DBG_S(string("Vlan Classifier Protocol Type = ") + protoValStr );

            if (protoValStr.length() != 0)
            {
                if (!protoValStr.compare("arp"))
                {
					NSM_PLUG_DBG("Protocol Type ARP");
                    m -> setProtoVal(PROTO_TYPE_ARP);
                }
                else
                {
                    if (!protoValStr.compare("ip"))
                    {
						NSM_PLUG_DBG("Protocol Type ARP");
						m -> setProtoVal(PROTO_TYPE_IPV4);
					}
					else
					{
						if (!protoValStr.compare("ipv6"))
						{
							NSM_PLUG_DBG("Protocol Type ARP");
                            m -> setProtoVal(PROTO_TYPE_IPV6);
                        }
						else
						{
							NSM_PLUG_DBG_S(string("Protocol Type ") + protoValStr);
							m -> setProtoVal(strtoul(protoValStr.c_str(),NULL,16));
						}	
                    }
                }
            }
            else
            {
				NSM_PLUG_DBG_S(string("To Local Plugin Protocol Type arp(2054) ip(2048) ipv6(34525) ") + protoValStr);
                m -> setProtoVal(pNsmGlobalCreateVlanClassifierRuleMessage -> getProtoValEnum());
            }
        }
		NSM_PLUG_DBG_S(string("To Local Plugin Encap type Ethv2(1) snapllc(2) nosnapllc(3)") + pNsmGlobalCreateVlanClassifierRuleMessage -> getEncap());
        m -> setEncap(pNsmGlobalCreateVlanClassifierRuleMessage -> getEncap());
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&VlanClassifierGlobalConfWorker::VlanClassifierGlobalSendToClusterCallback),
			                                                    pPrismLinearSequencerContext); 
		NSM_PLUG_ASSERT(send_ctx_p);
		send_ctx_p->setPPrismMessageForPhase1(m);
        setClusterContextFlags(send_ctx_p); 
		sendToWaveCluster(send_ctx_p);
        return ;
    }
    void VlanClassifierGlobalConfWorker::VlanClassifierRuleCreationMapMOStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");

        NsmGlobalCreateVlanClassifierRuleMessage* pNsmGlobalCreateVlanClassifierRuleMessage =
            dynamic_cast<NsmGlobalCreateVlanClassifierRuleMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
        VlanClassifierRulesManagedObject* pVlanClassifierRulesManagedObject = new VlanClassifierRulesManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
		NSM_PLUG_ASSERT(pVlanClassifierRulesManagedObject);

        pVlanClassifierRulesManagedObject -> setRuleId(pNsmGlobalCreateVlanClassifierRuleMessage -> getRuleId());
        pVlanClassifierRulesManagedObject -> setMac(pNsmGlobalCreateVlanClassifierRuleMessage -> getMac());
        pVlanClassifierRulesManagedObject -> setAppType(pNsmGlobalCreateVlanClassifierRuleMessage -> getAppType());
        pVlanClassifierRulesManagedObject -> setProtoValStr(pNsmGlobalCreateVlanClassifierRuleMessage -> getProtoValStr());
        pVlanClassifierRulesManagedObject -> setProtoValEnum(pNsmGlobalCreateVlanClassifierRuleMessage -> getProtoValEnum());
        pVlanClassifierRulesManagedObject -> setEncap(pNsmGlobalCreateVlanClassifierRuleMessage -> getEncap());
        pVlanClassifierRulesManagedObject -> setConfigTypeChoice(pNsmGlobalCreateVlanClassifierRuleMessage -> getConfigTypeChoice());
		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pVlanClassifierRulesManagedObject);

	    pPrismLinearSequencerContext -> executeNextStep(WAVE_MESSAGE_SUCCESS);
        return; 
    }
    void VlanClassifierGlobalConfWorker::NsmGlobalCreateVlanClassifierRuleMessageHandler(NsmGlobalCreateVlanClassifierRuleMessage * pNsmGlobalCreateVlanClassifierRuleMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierRuleCreationStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerStartTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierRuleCreationMapMOStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerSucceededStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext* pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmGlobalCreateVlanClassifierRuleMessage,
                                                                                              this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();

    }
    void VlanClassifierGlobalConfWorker::VlanClassifierRuleDeleteStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        WaveSendToClusterContext    *send_ctx_p;

        NsmGlobalDeleteVlanClassifierRuleMessage* pNsmGlobalDeleteVlanClassifierRuleMessage =
            dynamic_cast<NsmGlobalDeleteVlanClassifierRuleMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
        NsmDeleteVlanClassifierRuleSPMessage* m      = new NsmDeleteVlanClassifierRuleSPMessage();

        m -> setRuleId(pNsmGlobalDeleteVlanClassifierRuleMessage -> getRuleId());
	    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&VlanClassifierGlobalConfWorker::VlanClassifierGlobalSendToClusterCallback),
			                                                                 pPrismLinearSequencerContext);
		NSM_PLUG_ASSERT(send_ctx_p);
		send_ctx_p->setPPrismMessageForPhase1(m);
		setClusterContextFlags(send_ctx_p); 
		sendToWaveCluster(send_ctx_p);
        return ;
    }
    void VlanClassifierGlobalConfWorker::VlanClassifierRuleDeleteMapMOStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ResourceId                               status = WAVE_MESSAGE_ERROR;

        NsmGlobalDeleteVlanClassifierRuleMessage* pNsmGlobalDeleteVlanClassifierRuleMessage =
            dynamic_cast<NsmGlobalDeleteVlanClassifierRuleMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
		
        SI32                                     ruleId = pNsmGlobalDeleteVlanClassifierRuleMessage -> getRuleId();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierRulesManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(ruleId, "ruleId"));

        vector<WaveManagedObject * >*     pResults                          = querySynchronously(&syncQueryCtxt);
        VlanClassifierRulesManagedObject* pVlanClassifierRulesManagedObject = NULL;

		if (pResults && pResults -> size() == 1)
		{
			NSM_PLUG_DBG_S(string("ruleId =") + ruleId + string(" deleted")); 
			pVlanClassifierRulesManagedObject = dynamic_cast<VlanClassifierRulesManagedObject*>((*pResults)[0]);
			delete pVlanClassifierRulesManagedObject;
			pResults -> clear();
			delete pResults;
			pResults = NULL;
		    status                            = WAVE_MESSAGE_SUCCESS;
		}
		else
		{
			NSM_PLUG_DBG_S(string("Error in Deleting Vlan Classifier ruleId = ") + ruleId);
			if(pResults)
			{
				pResults -> clear();
				delete pResults;
				pResults = NULL;
			}	
			status = WAVE_MESSAGE_ERROR;
		}
		pPrismLinearSequencerContext->executeNextStep(status);
		return; 
    }
    void VlanClassifierGlobalConfWorker::DeleteVlanClassifierGroupStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        WaveSendToClusterContext    *send_ctx_p;

        NsmGlobalDeleteVlanClassifierGroupMessage* pNsmGlobalDeleteVlanClassifierGroupMessage =
            dynamic_cast<NsmGlobalDeleteVlanClassifierGroupMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
        
        NsmDeleteVlanClassifierGroupSPMessage* m      = new NsmDeleteVlanClassifierGroupSPMessage();
		NSM_PLUG_ASSERT(m);
		
        m -> setGroupId(pNsmGlobalDeleteVlanClassifierGroupMessage -> getGroupId());
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&VlanClassifierGlobalConfWorker::VlanClassifierGlobalSendToClusterCallback),
			                                                               pPrismLinearSequencerContext);
		NSM_PLUG_ASSERT(send_ctx_p);
		send_ctx_p->setPPrismMessageForPhase1(m);
        setClusterContextFlags(send_ctx_p); 
		sendToWaveCluster(send_ctx_p);
        return ;
    }
    void VlanClassifierGlobalConfWorker::DeleteVlanClassiferGroupMOStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        NsmGlobalDeleteVlanClassifierGroupMessage* pNsmGlobalDeleteVlanClassifierGroupMessage =
            dynamic_cast<NsmGlobalDeleteVlanClassifierGroupMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
        UI32 groupId = pNsmGlobalDeleteVlanClassifierGroupMessage -> getGroupId();
        deleteVlanClassifierGroup(groupId);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return ;
    }
    
    ResourceId VlanClassifierGlobalConfWorker::deleteVlanClassifierGroupsAssociatedWithRules(UI32 &ruleId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        unsigned int                                i;
        int                                         status = WAVE_MESSAGE_SUCCESS;
		
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
        ObjectId  ruleOid ;
		NsmUtils::getObjectIdByVlanClassifierRuleId(ruleId, ruleOid, true, pPrismLinearSequencerContext);
        syncQueryCtxt.addAndAttribute(new AttributeObjectId(ruleOid, "ruleId"));
        vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
        VlanClassifierGroupAssocRulesManagedObject* pVlanClassifierGroupAssocRulesManagedObject = NULL;

		if(pResults)
		{	
			for (i = 0; i < pResults -> size(); i++)
			{
				pVlanClassifierGroupAssocRulesManagedObject = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*>((*pResults)[i]);
				delete pVlanClassifierGroupAssocRulesManagedObject;
				status = WAVE_MESSAGE_SUCCESS;
			}
			if(i == 0)
				 status = WAVE_MESSAGE_ERROR;
		}
        if (pResults)
        {
            pResults -> clear();
            delete pResults;
        }

        return status;
    }

    ResourceId VlanClassifierGlobalConfWorker::deleteVlanClassifierGroup(UI32 &groupId)
    {
		NSM_PLUG_DBG("Entering ...");
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));

        vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
        VlanClassifierGroupAssocRulesManagedObject* pVlanClassifierGroupAssocRulesManagedObject = NULL;
        unsigned int                                i;
        int                                         status = WAVE_MESSAGE_SUCCESS;


		if(pResults)
		{
		NSM_PLUG_DBG_S( string("number of instances of group ") + groupId + string("is = ") +  pResults -> size()); 

			for (i = 0; i < pResults -> size(); i++)
			{
				pVlanClassifierGroupAssocRulesManagedObject = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*>((*pResults)[i]);
				delete pVlanClassifierGroupAssocRulesManagedObject;
				status = WAVE_MESSAGE_SUCCESS;
			}
			if(i == 0)
				status = WAVE_MESSAGE_ERROR;
		}
        if (pResults)
        {
            pResults -> clear();
            delete pResults;
        }
        return status;
    }
	void VlanClassifierGlobalConfWorker::deactivateVlanClassifierOnPoInterface(PrismLinearSequencerContext *pPrismLinearSequencerContext )
	{
		NSM_PLUG_DBG("Entering ...");
		NsmGlobalDeleteVlanClassifierGroupMessage* pNsmGlobalDeleteVlanClassifierGroupMessage =
			dynamic_cast<NsmGlobalDeleteVlanClassifierGroupMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
		UI32 groupId = pNsmGlobalDeleteVlanClassifierGroupMessage->getGroupId();
		WaveManagedObjectSynchronousQueryContext syncDeActivateQueryCtxt(VlanClassifierActivateGlobalManagedObject::getClassName());
		syncDeActivateQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));
		syncDeActivateQueryCtxt.addSelectField("ifName");
		syncDeActivateQueryCtxt.addSelectField("objectId");
		syncDeActivateQueryCtxt.addSelectField("ownerManagedObjectId");

		vector<WaveManagedObject * >*               pDeActivateResults                   =  querySynchronously(&syncDeActivateQueryCtxt);
		if(pDeActivateResults)
		{
			startTransaction();
			for(unsigned int j = 0;j < pDeActivateResults -> size() ;j++)
			{
				VlanClassifierActivateGlobalManagedObject *pVlanClassifierActivateGlobalManagedObject = dynamic_cast<VlanClassifierActivateGlobalManagedObject*>((*pDeActivateResults)[j]);

				ObjectId poObjId;
				//NsmUtils::getPoIntfObjectIdByPoIfName(pVlanClassifierActivateGlobalManagedObject -> getIfName(), poObjId, true , pPrismLinearSequencerContext);
				deleteFromComposition(PoIntfManagedObject::getClassName(), VlanClassifierActivateGlobalManagedObject::getClassName(),
										                     "vlanClassifierActivate", pVlanClassifierActivateGlobalManagedObject -> getOwnerManagedObjectId(), pVlanClassifierActivateGlobalManagedObject-> getObjectId());
			}
			commitTransaction();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDeActivateResults);
		}
		pPrismLinearSequencerContext -> executeNextStep(WAVE_MESSAGE_SUCCESS);
	}	

    void VlanClassifierGlobalConfWorker::NsmGlobalDeleteVlanClassifierGroupMessageHandler(NsmGlobalDeleteVlanClassifierGroupMessage * pNsmGlobalDeleteVlanClassifierGroupMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::deactivateVlanClassifierOnPoInterface),
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::DeleteVlanClassifierGroupStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerStartTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::DeleteVlanClassiferGroupMOStep),
     	    reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerSucceededStep),
     	    reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext* pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmGlobalDeleteVlanClassifierGroupMessage,
                                                                                              this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }
    void VlanClassifierGlobalConfWorker::VlanClassifierDeleteGroupAssoWithRulesMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext )
	{
		NSM_PLUG_DBG("VlanClassifierRuleDeleteMapMOStep");
		NsmGlobalDeleteVlanClassifierRuleMessage *pNsmGlobalDeleteVlanClassifierRuleMessage = dynamic_cast<NsmGlobalDeleteVlanClassifierRuleMessage*>
			(pPrismLinearSequencerContext->getPPrismMessage ());
		UI32 ruleId = pNsmGlobalDeleteVlanClassifierRuleMessage->getRuleId();
		deleteVlanClassifierGroupsAssociatedWithRules(ruleId, pPrismLinearSequencerContext);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		return ;

    }
    void VlanClassifierGlobalConfWorker::VlanClassifierDeActivateGroupAssoWithRulesMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext )
     {
	    
	        NSM_PLUG_DBG("Entering...");
	        ObjectId ruleObjectId;
            NsmGlobalDeleteVlanClassifierRuleMessage *pNsmGlobalDeleteVlanClassifierRuleMessage = dynamic_cast<NsmGlobalDeleteVlanClassifierRuleMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
            NsmUtils::getObjectIdByVlanClassifierRuleId(pNsmGlobalDeleteVlanClassifierRuleMessage -> getRuleId(), ruleObjectId, true, pPrismLinearSequencerContext);
			deActivateVlanClassifierOnPo(ruleObjectId, pPrismLinearSequencerContext);
			NSM_PLUG_DBG("Completed Deaitvating VlanClasifier on PO");
			deActivateVlanClassifierOnPhy(ruleObjectId, pPrismLinearSequencerContext);
			NSM_PLUG_DBG("Completed Deaitvating VlanClasifier on PHY");
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return ;

	 }		
	 void VlanClassifierGlobalConfWorker::deActivateVlanClassifierOnPo(const ObjectId &ruleObjectId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
	 {
		 NSM_PLUG_DBG("Entering...");
		 WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
		 syncQueryCtxt.addAndAttribute(new AttributeObjectId(ruleObjectId, "ruleId"));

		 vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
		 VlanClassifierGroupAssocRulesManagedObject* pVlanClassifierGroupAssocRulesManagedObject = NULL;
		 unsigned int                                i;
		 if(pResults)
		 {	 
			 for (i = 0; i < pResults -> size(); i++)
			 {
				 pVlanClassifierGroupAssocRulesManagedObject = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*>((*pResults)[i]);
				 UI32 groupId = pVlanClassifierGroupAssocRulesManagedObject -> getGroupId();
				 WaveManagedObjectSynchronousQueryContext syncGroupQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
				 syncGroupQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));

				 UI32 count = 0;
				 querySynchronouslyForCount (&syncGroupQueryCtxt, count);
				 if(count == 1)
				 {
					 WaveManagedObjectSynchronousQueryContext syncDeActivateQueryCtxt(VlanClassifierActivateGlobalManagedObject::getClassName());
					 syncDeActivateQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));
					 syncDeActivateQueryCtxt.addSelectField("ifName");
					 syncDeActivateQueryCtxt.addSelectField("objectId");
					 syncDeActivateQueryCtxt.addSelectField("ownerManagedObjectId");

					 vector<WaveManagedObject * >*               pDeActivateResults                   =  querySynchronously(&syncDeActivateQueryCtxt);

					 startTransaction();
					 for(unsigned int j = 0;j < pDeActivateResults -> size() ;j++)
					 {
						 VlanClassifierActivateGlobalManagedObject *pVlanClassifierActivateGlobalManagedObject = dynamic_cast<VlanClassifierActivateGlobalManagedObject*>((*pDeActivateResults)[j]);

						 ObjectId poObjId;
						 //NsmUtils::getPoIntfObjectIdByPoIfName(pVlanClassifierActivateGlobalManagedObject -> getIfName(), poObjId, true , pPrismLinearSequencerContext);
						 deleteFromComposition(PoIntfManagedObject::getClassName(), VlanClassifierActivateGlobalManagedObject::getClassName(),
						                     "vlanClassifierActivate", pVlanClassifierActivateGlobalManagedObject -> getOwnerManagedObjectId(), pVlanClassifierActivateGlobalManagedObject-> getObjectId());

					 }
					 commitTransaction();
					 if(pDeActivateResults)
					 {
						 pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pDeActivateResults);
						 pDeActivateResults->clear();
						 delete pDeActivateResults;
					 } 	 
				 }
			 }
		 }
		 if (pResults)
		 {
	         pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			 pResults -> clear();
			 delete pResults;
		 }

	 }	 

	 void VlanClassifierGlobalConfWorker::deActivateVlanClassifierOnPhy(const ObjectId &ruleObjectId, PrismLinearSequencerContext *pPrismLinearSequencerContext)
	 {
		 NSM_PLUG_DBG("Entering...");
		 WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
		 syncQueryCtxt.addAndAttribute(new AttributeObjectId(ruleObjectId, "ruleId"));

		 vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
		 VlanClassifierGroupAssocRulesManagedObject* pVlanClassifierGroupAssocRulesManagedObject = NULL;
		 unsigned int                                i;
		 int                                         status = WAVE_MESSAGE_SUCCESS;
		 if(pResults)
		 {	 
			 for (i = 0; i < pResults -> size(); i++)
			 {
				 pVlanClassifierGroupAssocRulesManagedObject = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*>((*pResults)[i]);
				 UI32 groupId = pVlanClassifierGroupAssocRulesManagedObject -> getGroupId();
				 WaveManagedObjectSynchronousQueryContext syncGroupQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
				 syncGroupQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));

				 UI32 count = 0;
				 querySynchronouslyForCount (&syncGroupQueryCtxt, count);
				 if(count == 1)
				 {
					 WaveManagedObjectSynchronousQueryContext syncDeActivateQueryCtxt(VlanClassifierActivateLocalManagedObject::getClassName());
					 syncDeActivateQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));
					 syncDeActivateQueryCtxt.addSelectField("ifName");
					 vector<WaveManagedObject * >*               pDeActivateResults                   =  querySynchronously(&syncDeActivateQueryCtxt);

					 if(pDeActivateResults)
					 {	 
						 for(unsigned int j = 0;j < pDeActivateResults -> size() ;j++)
						 {
							 VlanClassifierActivateLocalManagedObject *pVlanClassifierActivateLocalManagedObject = dynamic_cast<VlanClassifierActivateLocalManagedObject*>((*pDeActivateResults)[j]);
							 string ifName = pVlanClassifierActivateLocalManagedObject -> getIfName();
							 NsmDeleteVlanClassifierToPhyMessage* m       = new NsmDeleteVlanClassifierToPhyMessage(ifName, ifName, 0, groupId, VLAN_CLASSIFIER_TE_REMOVE_MO);
							 LocationId locationId = 0;

							 if(true  == NsmUtils::isValidThreeTuple(ifName) )
							 {
								 SI32 MappedId = NsmUtils::getMappedIdFromIfName(ifName);
								 locationId = DcmToolKit::getLocationIdFromMappedId(MappedId);
							 }	 
							 else
							 {
								 locationId = FrameworkToolKit::getThisLocationId ();
							 }
							 status = sendSynchronously(m, locationId);
							 if(WAVE_MESSAGE_SUCCESS == status )
							 {
								 NSM_PLUG_DBG_S(string("sendSynchronously Success") + status);
								 if(WAVE_MESSAGE_SUCCESS !=  m -> getCompletionStatus() )
								 {
									 NSM_PLUG_DBG_S(string("Error from Local Plugin") +  m -> getCompletionStatus() );
								 }
							 }
							 else
							 {
								 NSM_PLUG_DBG_S(string("sendSynchronously Failed") + status);
							 }
							 delete m;
						 }	 
                     }
					 if(pDeActivateResults)
					 {
	                     pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pDeActivateResults);
						 pDeActivateResults->clear();
						 delete pDeActivateResults;
						 pDeActivateResults = NULL;
					 }	 

				 }	 

				 status = WAVE_MESSAGE_SUCCESS;
			 }
		 }

		 if (pResults)
		 {
	         pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			 pResults -> clear();
			 delete pResults;
			 pResults = NULL;
		 }

	 }	 
	 
    void VlanClassifierGlobalConfWorker::NsmGlobalDeleteVlanClassifierRuleMessageHandler(NsmGlobalDeleteVlanClassifierRuleMessage * pNsmGlobalDeleteVlanClassifierRuleMessage)
    {
//      Validations and Queries should go here
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierRuleDeleteStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierDeActivateGroupAssoWithRulesMapMOStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerStartTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierDeleteGroupAssoWithRulesMapMOStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerStartTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierRuleDeleteMapMOStep),
     	    reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerSucceededStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext* pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmGlobalDeleteVlanClassifierRuleMessage,
                                                                                              this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }
    void VlanClassifierGlobalConfWorker::DeActivateVlanClassifierGroupFromPo(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        WaveSendToClusterContext    *send_ctx_p;
        NsmGlobalDeleteVlanClassifierToPoMessage* pNsmGlobalDeleteVlanClassifierToPoMessage =
            dynamic_cast<NsmGlobalDeleteVlanClassifierToPoMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());

		NSM_PLUG_ASSERT(pNsmGlobalDeleteVlanClassifierToPoMessage);
		
        NsmDeleteVlanClassifierToPoSPMessage* m = new NsmDeleteVlanClassifierToPoSPMessage();
		NSM_PLUG_ASSERT(m);
		
        m -> setId(pNsmGlobalDeleteVlanClassifierToPoMessage -> getId());
        m -> setIfName(pNsmGlobalDeleteVlanClassifierToPoMessage -> getIfName());
        m -> setIfIndex(pNsmGlobalDeleteVlanClassifierToPoMessage -> getIfIndex());
        m -> setGroupId(pNsmGlobalDeleteVlanClassifierToPoMessage -> getGroupId());
        m -> setOpcode(pNsmGlobalDeleteVlanClassifierToPoMessage -> getOpcode());
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&VlanClassifierGlobalConfWorker::
				                                  VlanClassifierGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		NSM_PLUG_ASSERT(send_ctx_p);
		
        if(PORT_NOT_PART_OF_ANY_INTERFACE == NsmUtils::setLocationIdsForPoSubModeCmds(pNsmGlobalDeleteVlanClassifierToPoMessage -> getIfName(), send_ctx_p))
        {
            NSM_PLUG_DBG("Port not part of any interface");
            pPrismLinearSequencerContext -> executeNextStep(WAVE_MESSAGE_SUCCESS);
			delete m;
			delete send_ctx_p;
            return;
        }
		send_ctx_p->setPPrismMessageForPhase1(m);
        setClusterContextFlags(send_ctx_p); 
		sendToWaveCluster(send_ctx_p);
        return ;
    }
    void VlanClassifierGlobalConfWorker::DeActivateVlanClassifierGroupFromPoMoStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        PoIntfManagedObject* pPoIntfManagedObject = NULL;
        NsmGlobalDeleteVlanClassifierToPoMessage* pNsmGlobalDeleteVlanClassifierToPoMessage =
            dynamic_cast<NsmGlobalDeleteVlanClassifierToPoMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());

		NsmUtils::getPoMoByPoId(pNsmGlobalDeleteVlanClassifierToPoMessage -> getId(), pPoIntfManagedObject);
		NSM_PLUG_ASSERT(pPoIntfManagedObject);
		
        pPoIntfManagedObject -> deleteVlanClassifierActivatedGroup(pNsmGlobalDeleteVlanClassifierToPoMessage -> getGroupId());
        updateWaveManagedObject(pPoIntfManagedObject);
		pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pPoIntfManagedObject);
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

        return; 
    }
    void VlanClassifierGlobalConfWorker::NsmGlobalDeleteVlanClassifierToPoMessageHandler(NsmGlobalDeleteVlanClassifierToPoMessage * pNsmGlobalDeleteVlanClassifierToPoMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::DeActivateVlanClassifierGroupFromPo),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::DeActivateVlanClassifierGroupFromPoMoStep), 
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerSucceededStep), 
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext* pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmGlobalDeleteVlanClassifierToPoMessage,
                                                                        this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }
    void VlanClassifierGlobalConfWorker::VlanClassifierGroupCreationStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        WaveSendToClusterContext    *send_ctx_p;

        NsmGlobalUpdateVlanClassifierGroupMessage* pNsmGlobalUpdateVlanClassifierGroupMessage =
            dynamic_cast<NsmGlobalUpdateVlanClassifierGroupMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());

		NSM_PLUG_ASSERT(pNsmGlobalUpdateVlanClassifierGroupMessage);

        NsmUpdateVlanClassifierGroupSPMessage* m      = new NsmUpdateVlanClassifierGroupSPMessage();
		NSM_PLUG_ASSERT(m);
		
        m -> setGroupId(pNsmGlobalUpdateVlanClassifierGroupMessage -> getGroupId());
        m -> setOperation(pNsmGlobalUpdateVlanClassifierGroupMessage -> getOperation());
        m -> setRuleId(pNsmGlobalUpdateVlanClassifierGroupMessage -> getRuleId());
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&VlanClassifierGlobalConfWorker::
				                                  VlanClassifierGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		NSM_PLUG_ASSERT(send_ctx_p);
		send_ctx_p->setPPrismMessageForPhase1(m);
        setClusterContextFlags(send_ctx_p); 
		sendToWaveCluster(send_ctx_p);
        return ;
    }

	
    void VlanClassifierGlobalConfWorker::VlanClassifierGroupAssociationMapMOStep(PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
		ObjectId ruleOid;

        NsmGlobalUpdateVlanClassifierGroupMessage* pNsmGlobalUpdateVlanClassifierGroupMessage =
            dynamic_cast<NsmGlobalUpdateVlanClassifierGroupMessage*>(pPrismLinearSequencerContext -> getPPrismMessage());
        UI32                    groupId = pNsmGlobalUpdateVlanClassifierGroupMessage -> getGroupId();
        UI32                    ruleId  = pNsmGlobalUpdateVlanClassifierGroupMessage -> getRuleId();
        vlanClassifierOperation oper    = (vlanClassifierOperation) pNsmGlobalUpdateVlanClassifierGroupMessage -> getOperation();

		if (oper == VLAN_CLASSIFIER_ADD_GRP)
		{
			VlanClassifierGroupAssocRulesManagedObject* pVlanClassifierGroupAssocRulesManagedObject =
				new VlanClassifierGroupAssocRulesManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));

			NsmUtils::getObjectIdByVlanClassifierRuleId(ruleId, ruleOid, true, pPrismLinearSequencerContext);
			pVlanClassifierGroupAssocRulesManagedObject -> setGroupId(groupId);
			pVlanClassifierGroupAssocRulesManagedObject -> setRuleId(ruleOid);
			pVlanClassifierGroupAssocRulesManagedObject -> setoper(oper);
			pVlanClassifierGroupAssocRulesManagedObject -> setdummy(0);
		    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pVlanClassifierGroupAssocRulesManagedObject);
		}
		else
		{
			VlanClassifierDeleteRuleForGroup(groupId, ruleId, pPrismLinearSequencerContext);
		}
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return ;
    }
    int VlanClassifierGlobalConfWorker::VlanClassifierDeleteRuleForGroup(UI32 &groupId, UI32 &ruleId, PrismLinearSequencerContext * pPrismLinearSequencerContext)
    {
		NSM_PLUG_DBG("Entering ...");
        ObjectId ruleOid;
        int status = WAVE_MESSAGE_SUCCESS;
		
		NsmUtils::getObjectIdByVlanClassifierRuleId(ruleId, ruleOid, true, pPrismLinearSequencerContext);
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeObjectId(ruleOid, "ruleId"));
		syncQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));

		vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
		VlanClassifierGroupAssocRulesManagedObject* pVlanClassifierGroupAssocRulesManagedObject = NULL;

		if(pResults && pResults->size() == 1)
		{	
			pVlanClassifierGroupAssocRulesManagedObject = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*>((*pResults)[0]);
			delete pVlanClassifierGroupAssocRulesManagedObject;
		}
		return status;
    }

    void VlanClassifierGlobalConfWorker::NsmGlobalUpdateVlanClassifierGroupMessageHandler(NsmGlobalUpdateVlanClassifierGroupMessage * pNsmGlobalUpdateVlanClassifierGroupMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierGroupCreationStep),
	        reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::VlanClassifierGroupAssociationMapMOStep), 
			reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerSucceededStep), 
			reinterpret_cast<PrismLinearSequencerStep>(&VlanClassifierGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        PrismLinearSequencerContext* pPrismLinearSequencerContext = new PrismLinearSequencerContext(pNsmGlobalUpdateVlanClassifierGroupMessage,
                                                                                              this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

	void VlanClassifierGlobalConfWorker::VlanClassifierGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p)	
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
	


	bool vlanClassifierRuleMoCompare(WaveManagedObject* p1, WaveManagedObject* p2)
	{
		//WaveNs::trace(TRACE_LEVEL_DEBUG, "Entering ...");
		VlanClassifierRulesManagedObject *rp1 = dynamic_cast<VlanClassifierRulesManagedObject*> (p1);
		VlanClassifierRulesManagedObject *rp2 = dynamic_cast<VlanClassifierRulesManagedObject*> (p2);
		//WaveNs::trace(TRACE_LEVEL_DEBUG, string("rule1 = ") + rp1->m_ruleId + string("rule2 = ") + rp2->m_ruleId);
		return (rp1->m_ruleId < rp2->m_ruleId);
	}	


	bool vlanClassifierGroupMoCompare(WaveManagedObject* p1, WaveManagedObject* p2)
	{
		VlanClassifierGroupAssocRulesManagedObject *rp1 = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*> (p1);
		VlanClassifierGroupAssocRulesManagedObject *rp2 = dynamic_cast<VlanClassifierGroupAssocRulesManagedObject*> (p2);
		ObjectId ruleOid1 = rp1->m_ruleId; 
		ObjectId ruleOid2 = rp2->m_ruleId; 
		UI32 ruleId1 = NsmUtils::getVlanClassifierRuleByObjectId(ruleOid1, false);
		UI32 ruleId2 = NsmUtils::getVlanClassifierRuleByObjectId(ruleOid2, false);
		if(rp1->m_groupId < rp2->m_groupId)
			return true;
		if(rp1->m_groupId == rp2->m_groupId)
		{	
			if (ruleId1 < ruleId2) 
				return true;
		}	
		return false;
	}	

	bool vlanClassifierPoActivateMoCompare(WaveManagedObject* p1, WaveManagedObject* p2)
	{
		VlanClassifierActivateGlobalManagedObject *rp1 = dynamic_cast<VlanClassifierActivateGlobalManagedObject*> (p1);
		VlanClassifierActivateGlobalManagedObject *rp2 = dynamic_cast<VlanClassifierActivateGlobalManagedObject*> (p2);
		ObjectId vlanOid1 = rp1->m_vlanId; 
		ObjectId vlanOid2 = rp2->m_vlanId; 
		UI32 vlanId1 = NsmUtils::getVlanIdByObjectId(vlanOid1, false);
		UI32 vlanId2 = NsmUtils::getVlanIdByObjectId(vlanOid2, false);
			
		if(rp1->m_groupId < rp2->m_groupId)
			return true;
		if(rp1->m_groupId == rp2->m_groupId)
		{	
			if (vlanId1 < vlanId2) 
				return true;
		}	
		return false;
		
	}	

	bool vlanClassifierPhyActivateMoCompare(WaveManagedObject* p1, WaveManagedObject* p2)
	{
		VlanClassifierActivateLocalManagedObject *rp1 = dynamic_cast<VlanClassifierActivateLocalManagedObject*> (p1);
		VlanClassifierActivateLocalManagedObject *rp2 = dynamic_cast<VlanClassifierActivateLocalManagedObject*> (p2);
		ObjectId vlanOid1 = rp1->m_vlanId; 
		ObjectId vlanOid2 = rp2->m_vlanId; 
		UI32 vlanId1 = NsmUtils::getVlanIdByObjectId(vlanOid1, false);
		UI32 vlanId2 = NsmUtils::getVlanIdByObjectId(vlanOid2, false);
		
		if(rp1->m_groupId < rp2->m_groupId)
			return true;
		if(rp1->m_groupId == rp2->m_groupId)
		{	
			if (vlanId1 < vlanId2) 
				return true;
		}	
		return false;
	}	
}
