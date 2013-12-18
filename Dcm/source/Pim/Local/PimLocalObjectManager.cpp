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
 *   Author : rjain                                                     *
 **************************************************************************/

#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimWorker.h"
#include "Pim/Local/PimTypes.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Pim/PimLocalIntfWorker.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/Layer3LicenseUtil.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"

namespace DcmNs
{

    PimLocalObjectManager::PimLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
		associateWithVirtualWaveObjectManager (L3NodeSpecificGlobalObjectManager::getInstance());
		trace (TRACE_LEVEL_DEBUG, "Inside PimLocalObjectManager constructor:");
        m_pPimWorker = new PimWorker (this);
        prismAssert (NULL != m_pPimWorker, __FILE__, __LINE__);
    }

    PimLocalObjectManager::~PimLocalObjectManager ()
    {
		trace (TRACE_LEVEL_DEBUG, "Inside PimLocalObjectManager destructor:");
    }

    PimLocalObjectManager  *PimLocalObjectManager::getInstance()
    {
        static PimLocalObjectManager *pPimLocalObjectManager = new PimLocalObjectManager ();

        WaveNs::prismAssert (NULL != pPimLocalObjectManager, __FILE__, __LINE__);

        return (pPimLocalObjectManager);
    }

    string  PimLocalObjectManager::getClassName()
    {
        return ("Pim");
    }

    PrismServiceId  PimLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *PimLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
		trace (TRACE_LEVEL_DEBUG, string("Inside PimLocalObjectManager createMessageInstance with operation code:") + operationCode);
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PimLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
		trace (TRACE_LEVEL_DEBUG, string("Inside PimLocalObjectManager createManagedObjectInstance with managed class name:") + managedClassName);
        return NULL;
    }
    
    /* Function to convert the interface type to PIM interface type */
    UI8
    PimLocalObjectManager::xltToPimIntfType(UI8 intfType)
    {
        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::xltToPimIntfType:") +
            string("Entered..."));
            
        switch(intfType) {
            case IF_TYPE_GI:
                return(INTERFACE_ONEGIG);
            case IF_TYPE_TE:
                return(INTERFACE_TENGIG);
            case IF_TYPE_FO:
                return(INTERFACE_FORTYGIG);
            default:
                return(INVALID_INTERFACE);
        }
    }
   

void
PimLocalObjectManager::setPimRtrCfgClientMsgFieldsForReplay(PimRtrCfgClientMsg *msg, const PimRtrCfgLocalManagedObject *pMo)
{
    WaveUnion sptVal;
    
    trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgClientMsg::setPimRtrCfgFieldsForReplay:"
        " Entered...."));
    
    /* Set the message type and command code for postboot replay */
    msg->m_cmdCode = PIM_GLBL_CFG_REPLAY_CMD;
    msg->m_msgType = PIM_MSG_ADD;
    
    /* Populate the fields of the message */
    //m_vrfId = pMo->getVrfId();
    if(pMo->getPresenceFlag() == PIM_TRUE)
        msg->m_enable = PIM_TRUE;
    else
        msg->m_enable = PIM_FALSE;
    msg->m_maxMcache = pMo->getMaxMcache();
    msg->m_helloTimer = pMo->getHelloTimer();
    msg->m_nbrTo = pMo->getNbrTimeout();
    msg->m_inactTimer = pMo->getInactivityTimer();
    msg->m_msgIntv = pMo->getMsgInterval();
    sptVal = pMo->getSptThresholdUnion();
    msg->m_sptThold = pMo->getSptThreshold(sptVal);
    
    trace(TRACE_LEVEL_DEBUG, string("msgType: ") + toString(msg->m_msgType));
    trace(TRACE_LEVEL_DEBUG, string("cmdCode: ") + toString(msg->m_cmdCode));
    trace(TRACE_LEVEL_DEBUG, string("mcache: ") + toString(msg->m_maxMcache));
    trace(TRACE_LEVEL_DEBUG, string("hello_timer: ") + toString(msg->m_helloTimer));
    trace(TRACE_LEVEL_DEBUG, string("nbr_to: ") + toString(msg->m_nbrTo));
    trace(TRACE_LEVEL_DEBUG, string("inact: ") + toString(msg->m_inactTimer));
    trace(TRACE_LEVEL_DEBUG, string("msg_intv: ") + toString(msg->m_msgIntv));
    trace(TRACE_LEVEL_DEBUG, string("spt_thold: ") + toString(msg->m_sptThold));
    
    return;
}

 
    /* Function to restore the global router pim configs except RP candidate
     * and BSR candidate */
    void 
    PimLocalObjectManager::restorePimRtrCfg(void)
    {
        PimRtrCfgClientMsg *msg = new PimRtrCfgClientMsg(PIM_CFG_GLBL_REPLAY);
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		LocationId locId = FrameworkToolKit::getThisLocationId();

        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::restorePimRtrCfg:") +
            string("Entered..."));

        /* Query for the PIM router config MO and populate the message fields */
        PimRtrCfgLocalManagedObject *pMo = NULL;
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtx(PimRtrCfgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = 
					querySynchronouslyLocalManagedObjectsForLocationId(locId, &syncQueryCtx);

        if (pResults && pResults->size () == 1) {
            pMo = dynamic_cast<PimRtrCfgLocalManagedObject*> ((*pResults)[0]);
            
            /* Populate the fields of the message */
            setPimRtrCfgClientMsgFieldsForReplay (msg, pMo);
            
            /* Send message to the daemon */
            status = sendSynchronouslyToWaveClient("pim", msg);
            if (status == WAVE_MESSAGE_SUCCESS)
                status = msg->getCompletionStatus();
        //if (status == WAVE_MESSAGE_SUCCESS)
        //    status = ::mapRtmBackEndErrorsToDcmdErrors(msg.getClientStatus());
            if (status != WAVE_MESSAGE_SUCCESS)
                trace(TRACE_LEVEL_ERROR, 
                    string("PimLocalObjectManager::restorePimRtrCfg:") +
                    string("Replay of PIM router global config failed"));
            else
                trace(TRACE_LEVEL_INFO, 
                    string("PimLocalObjectManager::restorePimRtrCfg:") +
                    string("Replay of PIM router global config succeeded"));
        }
        else {
            trace(TRACE_LEVEL_DEBUG, 
                string("PimLocalObjectManager::restorePimRtrCfg:") +
                string(" PIM config does not exists. Do nothing"));
        }

        /* Free memory */
        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        delete msg;

        return;
    }
   
void 
PimLocalObjectManager::setPimStaticRpCfgClientMsgFieldsForReplay(PimStaticRpCfgClientMsg *msg, const 
    StaticRpLocalManagedObject *pMo)
{
    trace(TRACE_LEVEL_DEBUG, 
        string("PimStaticRpCfgClientMsg::setPimStaticRpFieldsForReplay: ") + 
        string("Entered...."));
    
    /* Set the message type to add since we are doing post boot replay */
    msg->m_msgType = PIM_MSG_ADD;
    
    /* Copy the fields from the MO */
    msg->m_rpAddr = pMo->getRpAddr();
    msg->m_prefixList = pMo->getPrefixList();
    
    trace(TRACE_LEVEL_DEBUG, string("Static RP addr: ") + msg->m_rpAddr.toString());
    trace(TRACE_LEVEL_DEBUG, string("Prefix list: ") + toString(msg->m_prefixList));
    
    return;
}

 
    /* Function to restore the static RP configs */
    void 
    PimLocalObjectManager::restoreStaticRpCfg(void)
    {
        PimStaticRpCfgClientMsg *msg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::restoreStaticRpCfg:") +
            string("Entered..."));

        /* Query for the PIM router config MO and populate the message fields */
        StaticRpLocalManagedObject *pMo = NULL;
		LocationId locId = FrameworkToolKit::getThisLocationId();
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtx(StaticRpLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = 
						querySynchronouslyLocalManagedObjectsForLocationId(locId, &syncQueryCtx);

        if(!pResults) {
            trace(TRACE_LEVEL_DEBUG, 
                string("PimLocalObjectManager::restoreStaticRpCfg:") +
                string(" PIM static RP config does not exists. Do nothing"));
			return;
		}
                
        for(UI32 i = 0; i < pResults->size(); i++) {
            pMo = dynamic_cast<StaticRpLocalManagedObject*> ((*pResults)[i]);
			msg = new PimStaticRpCfgClientMsg();
            
            /* Populate the fields of the message */
            setPimStaticRpCfgClientMsgFieldsForReplay(msg, pMo);
            
            /* Send message to the daemon */
            status = sendSynchronouslyToWaveClient("pim", msg);
            if (status == WAVE_MESSAGE_SUCCESS)
                status = msg->getCompletionStatus();
        //if (status == WAVE_MESSAGE_SUCCESS)
        //    status = ::mapRtmBackEndErrorsToDcmdErrors(msg.getClientStatus());
            if (status != WAVE_MESSAGE_SUCCESS)
                trace(TRACE_LEVEL_ERROR, 
                    string("PimLocalObjectManager::restoreStaticRpCfg:") +
                    string("Replay of PIM static RP config failed"));
            else
                trace(TRACE_LEVEL_INFO, 
                    string("PimLocalObjectManager::restoreStaticRpCfg:") +
                    string("Replay of PIM static RP config succeeded"));
			delete msg;
        }

        /* Free memory */
        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return;
    }


void 
PimLocalObjectManager::setPimCandRpPrefixCfgClientMsgFieldsForReplay(PimCandRpPrefixCfgClientMsg *msg, const 
    RpCandidatePrefixLocalManagedObject *pMo)
{
    trace(TRACE_LEVEL_DEBUG, string(
		"PimCandRpPrefixCfgClientMsg::setPimCandRpPrefixFieldsForReplay: ") + 
        string("Entered...."));
    
    /* Set the message type to add since we are doing post boot replay */
    msg->m_msgType = PIM_MSG_ADD;
    
    /* Copy the fields from the MO */
	msg->m_prefixAction = pMo->getAction();
    msg->m_prefixIP = pMo->getPrefixIP();
    msg->m_prefixLen = pMo->getPrefixLen();
    trace(TRACE_LEVEL_DEBUG, string("Prefix Action: ") + 
		toString(msg->m_prefixAction));
    trace(TRACE_LEVEL_DEBUG, string("Prefix Ipaddr: ") + 
		msg->m_prefixIP.toString());
    trace(TRACE_LEVEL_DEBUG, string("Prefix Len: ") + 
		toString(msg->m_prefixLen));
    
    return;
}


    /* Function to restore the RP Candidate Prefix configs */
    void 
    PimLocalObjectManager::restoreRpCandPrefixCfg(void)
    {
        PimCandRpPrefixCfgClientMsg msg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::restoreRpCandPrefixCfg:") +
            string("Entered..."));

        /* Query for the PIM router config MO and populate the message fields */
        RpCandidatePrefixLocalManagedObject *pMo = NULL;
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtx(RpCandidatePrefixLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (
			&syncQueryCtx);

        if(!pResults)
            trace(TRACE_LEVEL_DEBUG, 
                string("PimLocalObjectManager::restoreRpCandPrefixCfg:") +
                string(
				" PIM RP Candidate prefix config does not exists. Do nothing")
				);
                
        for(UI32 i = 0; i < pResults->size(); i++) {
            pMo = dynamic_cast<RpCandidatePrefixLocalManagedObject*> 
				((*pResults)[i]);
            
            /* Populate the fields of the message */
            setPimCandRpPrefixCfgClientMsgFieldsForReplay(&msg, pMo);
            
            /* Send message to the daemon */
            status = sendSynchronouslyToWaveClient("pim", &msg);
            if (status == WAVE_MESSAGE_SUCCESS)
                status = msg.getCompletionStatus();
            if (status != WAVE_MESSAGE_SUCCESS)
                trace(TRACE_LEVEL_ERROR, 
                    string("PimLocalObjectManager::restoreRpCandPrefixCfg:") +
                    string("Replay of PIM RP Candidate Prefix config failed"));
            else
                trace(TRACE_LEVEL_INFO, 
                    string("PimLocalObjectManager::restoreRpCandPrefixCfg:") +
                    string("Replay of PIM RP Candidate Prefix config succeeded"));
        }

        /* Free memory */
        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return;
    }
 
    
    /* Function to restore PIM configs on physical interfaces. This function
     * will only extract the relevant PIM Intf MO under L3 Ready MO */
    void
    PimLocalObjectManager::restorePhyIntfPimCfg(SI32 slotNum)
    {
        PhyIntfLocalManagedObject *pPhyMo = NULL;
        vector<PhyIntfLocalManagedObject *> pResults;
        const NsmL3ReadyManagedObject *pL3RdyMo;
        UI8 intfType;

        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::restorePhyIntfPimCfg:") +
            string("Entered...slotNum = %d") + slotNum);
            
        /* Given the slot_id extract the L3 ready MOs */
        NsmUtils::SlotGetPhyIntfMOs(pResults, slotNum);

        for (UI32 i = 0; i < pResults.size(); i++) {
            pPhyMo = pResults[i];
            intfType = xltToPimIntfType(pPhyMo->getPhyType());
            if(intfType == INVALID_INTERFACE) {
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimLocalObjectManager::restorePhyIntfPimCfg:") + 
                    string("Skipping config replay for invalid interface type ") +
                    intfType);
                /* Free memory */
                delete pPhyMo;
                continue;
            }
                
            pL3RdyMo = pPhyMo->getL3ReadyMO();
            if (pL3RdyMo) {
                PimIntfCfgLocalManagedObject *pMo = 
                    PimLocalIntfWorker::getPimIntfCfgLclMo(pL3RdyMo);
                if (pMo != NULL) {
                    replayPimIntfCfg(pMo, intfType, pPhyMo->getId());
                }
                else {
                    trace(TRACE_LEVEL_DEBUG, 
                        string("PimLocalObjectManager::restorePhyIntfPimCfg:") +
                        string("PIM config does not exists for ") + pPhyMo->getId());
                }
            }

            /* Free memory */
            delete pPhyMo;
        }        
            
        return;
    }
   
void
PimLocalObjectManager::setPimIntfCfgClientMsgFieldsForReplay(PimIntfCfgClientMsg *msg, const PimIntfCfgLocalManagedObject *pMo, const UI8 intfType, const string intfName)
{
    trace(TRACE_LEVEL_DEBUG, 
        string("PimIntfCfgClientMsg::setPimIntfCfgFieldsForReplay:") +
        string(" Entered...."));
    
    /* Set the message type for interface post boot replay */
    msg->m_cmdCode = PIM_LOCAL_CFG_REPLAY_CMD;
    
    /* Populate the fields of the message */
    msg->m_intfType = intfType;
    msg->m_intfName = intfName;
    msg->m_enable = pMo->getEnabled();
    msg->m_drPriority = pMo->getDrPriority();
    msg->m_pimBorder = pMo->getPimBorder();
    msg->m_ttlThold = pMo->getTtlThold();
    msg->m_multBoundaryAcl = pMo->getMultBoundaryAcl();
    
    trace(TRACE_LEVEL_DEBUG, string("cmdCode: ") + toString(msg->m_cmdCode));
    trace(TRACE_LEVEL_DEBUG, string("enable: ") + toString(msg->m_enable));
    trace(TRACE_LEVEL_DEBUG, string("dr_prio: ") + toString(msg->m_drPriority));
    trace(TRACE_LEVEL_DEBUG, string("brdr_rtr: ") + toString(msg->m_pimBorder));
    trace(TRACE_LEVEL_DEBUG, string("ttl_thold: ") + toString(msg->m_ttlThold));
    trace(TRACE_LEVEL_DEBUG, string("intf_type: ") + toString(msg->m_intfType));
    trace(TRACE_LEVEL_DEBUG, string("intf_name: ") + toString(msg->m_intfName));
    
    return;
}


    /* Function to replay the PIM interface configs to the backend daemon. All
     * interfaces are taken care for i.e. physical, vlan and lag. The input
     * parameteres intfName and intfType are used to identify the interface */
    void
    PimLocalObjectManager::replayPimIntfCfg(PimIntfCfgLocalManagedObject *pMo,
        UI8 intfType, string intfName)
    {
        PimIntfCfgClientMsg msg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string ifname;
        
        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::replayPimIntfCfg:") +
            string("Entered..."));
       
        /* Pass 2 tuple name to backend if we are in VCS mode */
        if(NsmUtils::isValidThreeTuple(intfName)) {
            NsmUtils::getTwoTupleIfName(intfName, ifname);
        }
        else {
            ifname = intfName;
        }
 
        /* Populate the fields of the message */
	setPimIntfCfgClientMsgFieldsForReplay(&msg, pMo, intfType, ifname);
            
        /* Send message to the daemon */
        status = sendSynchronouslyToWaveClient("pim", &msg);
        if (status == WAVE_MESSAGE_SUCCESS)
            status = msg.getCompletionStatus();
        //if (status == WAVE_MESSAGE_SUCCESS)
        //    status = ::mapRtmBackEndErrorsToDcmdErrors(msg.getClientStatus());
        if (status != WAVE_MESSAGE_SUCCESS)
            trace(TRACE_LEVEL_ERROR, 
                string("PimLocalObjectManager::replayPimIntfCfg:") +
                string("Replay of PIM physical interface config failed"));
        else
            trace(TRACE_LEVEL_INFO, 
                string("PimLocalObjectManager::replayPimIntfCfg:") +
                string("Replay of PIM physical interface config succeeded"));
            
        return;         
    }
   
    void PimLocalObjectManager::configReplayPimPhyIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)   {
        const NsmL3ReadyManagedObject *pL3ReadyMO;
        UI8 intfType;

        intfType = xltToPimIntfType(pPhyIntfLocalManagedObject ->getPhyType());
        if(intfType == INVALID_INTERFACE) {
                trace(TRACE_LEVEL_DEBUG,
                    string("PimLocalObjectManager::restorePhyIntfPimCfg:") +
                    string("Skipping config replay for invalid interface type ") +
                    intfType);
                return;
        }

        pL3ReadyMO = pPhyIntfLocalManagedObject->getL3ReadyMO();
        if (pL3ReadyMO) {
            PimIntfCfgLocalManagedObject *pMo =
            PimLocalIntfWorker::getPimIntfCfgLclMo(pL3ReadyMO);
            if (pMo != NULL) {
                replayPimIntfCfg(pMo, intfType, pPhyIntfLocalManagedObject->getId());
            }
            else {
                trace(TRACE_LEVEL_DEBUG,
                    string("PimLocalObjectManager::restorePhyIntfPimCfg:") +
                    string("PIM config does not exists for ") + pPhyIntfLocalManagedObject->getId());
            }
        }
    }

    /* Function to restore PIM configs on VLAN interfaces. This function
     * will only extract the relevant PIM Intf MO under L3 Ready MO */
    void
    PimLocalObjectManager::restoreVlanIntfPimCfg()
    {
		NsmIntfSviManagedObject *pVlanMo = NULL;
		vector<NsmIntfSviManagedObject *> pResults;
        const NsmL3ReadyManagedObject *pL3RdyMo;

        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::restoreVlanIntfPimCfg:") +
            string("Entered..."));
            
        /* Given the map_id extract the L3 ready MOs */
		NsmL3Util::NodeVlanIfGetMOs(pResults, FrameworkToolKit::getThisLocationId());

        for (UI32 i = 0; i < pResults.size(); i++) {
            pVlanMo = pResults[i];
            pL3RdyMo = pVlanMo->getL3ReadyMO();
            if (pL3RdyMo) {
                PimIntfCfgLocalManagedObject *pMo = 
                    PimLocalIntfWorker::getPimIntfCfgLclMo(pL3RdyMo);
                if (pMo != NULL) {
                    replayPimIntfCfg(pMo, INTERFACE_VLAN, 
                        toString(pVlanMo->getIfId()));
                }
                else {
                    trace(TRACE_LEVEL_DEBUG, 
                        string("PimLocalObjectManager::restoreVlanIntfPimCfg:") +
                        string("PIM config does not exists for VLAN ") + 
                        pVlanMo->getIfId());
                }
            }
            /* Free memory */
            delete pVlanMo;
        }        
            
        return;
    }
   
    /* Function to restore PIM configs on port-channel interfaces. This 
     * function will only extract the relevant PIM Intf MO under L3 Ready MO */
    void
    PimLocalObjectManager::restorePoIntfPimCfg()
    {
		NsmNodePoManagedObject *pPoMo = NULL;
		vector<NsmNodePoManagedObject *> pResults;
        const NsmL3ReadyManagedObject *pL3RdyMo;
        SI32 mapId = 0;

        trace(TRACE_LEVEL_DEBUG, 
            string("PimLocalObjectManager::restorePoIntfPimCfg:") +
            string("Entered..."));
            
        /* Given the map_id extract the L3 ready MOs */
		NsmL3Util::NodePoIfGetMOs(pResults, mapId);

        for (UI32 i = 0; i < pResults.size(); i++) {
            pPoMo = pResults[i];
            pL3RdyMo = pPoMo->getL3ReadyMO();
            if (pL3RdyMo) {
                PimIntfCfgLocalManagedObject *pMo = 
                    PimLocalIntfWorker::getPimIntfCfgLclMo(pL3RdyMo);
                if (pMo != NULL) {
                    replayPimIntfCfg(pMo, INTERFACE_PORT_CHANNEL, 
                        toString(pPoMo->getIfId()));
                }
                else {
                    trace(TRACE_LEVEL_DEBUG, 
                        string("PimLocalObjectManager::restorePoIntfPimCfg:") +
                        string("PIM config does not exists for port-channel ") + 
                        pPoMo->getIfId());
                }
            }
            /* Free memory */
            delete pPoMo;
        }        
            
        return;
    }
   

void
PimLocalObjectManager::setPimCandRpCfgClientMsgFieldsForReplay(PimCandRpCfgClientMsg *msg, const RpCandidateLocalManagedObject *pMo)
{
    trace(TRACE_LEVEL_DEBUG, string("PimCandRpCfgClientMsg::setPimRpCandFieldsForReplay:"
        " Entered...."));

    /* Set the message type to add since we are doing postboot replay */
    msg->m_msgType = PIM_MSG_ADD;
    
    /* Now copy the data members */
    msg->m_intfType = pMo->getIntfType();
    msg->m_intfName = pMo->getIntfName();
    msg->m_prefixList = pMo->getPrefixList();
    
    trace(TRACE_LEVEL_DEBUG, string("msg_type: ") + toString(msg->m_msgType));
    trace(TRACE_LEVEL_DEBUG, string("intf_type: ") + toString(msg->m_intfType));
    trace(TRACE_LEVEL_DEBUG, string("intf_name: ") + toString(msg->m_intfName));
    trace(TRACE_LEVEL_DEBUG, string("prefix_list: ") + msg->m_prefixList);
    
    return;
}


 
    /* Function to restore the RP candidate configs. Will be called for both
     * the global and local stages of postboot. 
     * During global stage, RP candidates referring to VLAN and LAG will be 
     * restored. In this case slotNum will be -1.
     * During the local/slot stage, RP candidates referring to physical 
     * interfaces will be restored. In this case slotNum will be the slot 
     * associated with the interface */
    void
    PimLocalObjectManager::restoreRpCandCfg(SI32 slotNum)
    {
        RpCandidateLocalManagedObject *pMo = NULL;
        PimCandRpCfgClientMsg msg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG,
            string("PimLocalObjectManager::restoreRpCandCfg:") +
            string("Entered...slot = ") + slotNum);
            
        /* Query for all RP candidate MO's that are associated with this 
         * slot */
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtx(RpCandidateLocalManagedObject::getClassName());
        syncQueryCtx.setSlotFilter(slotNum);
        vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtx);
        
        if (pResults && pResults->size () == 0) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimLocalObjectManager::restoreRpCandCfg:") +
                string("PIM config does not exists"));
            /* Free memory */
            if(pResults != NULL)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return;
        }
        
        for(UI32 i = 0; i < pResults->size(); i++) {
            pMo = dynamic_cast<RpCandidateLocalManagedObject *>((*pResults)[i]);
            
            /* Populate the fields of the message */
            setPimCandRpCfgClientMsgFieldsForReplay(&msg, pMo);

            /* Send message to the daemon */
            status = sendSynchronouslyToWaveClient("pim", &msg);
            if (status == WAVE_MESSAGE_SUCCESS)
                status = msg.getCompletionStatus();
                //if (status == WAVE_MESSAGE_SUCCESS)
                //    status = ::mapRtmBackEndErrorsToDcmdErrors(msg.getClientStatus());
            /* Check daemon status code */
            if (status != WAVE_MESSAGE_SUCCESS)
                trace(TRACE_LEVEL_ERROR,
                    string("PimLocalObjectManager::replayRpCandCfg:") +
                    string("Replay of RP candidate config failed"));
            else
                trace(TRACE_LEVEL_INFO,
                    string("PimLocalObjectManager::replayRpCandCfg:") +
                    string("Replay of RP candidate config succeeded"));
        }

        /* Free memory */
        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return;
    }
   
void
PimLocalObjectManager::setPimBsrCandClientMsgFieldsForReplay(PimBsrCandClientMsg *msg, const BsrCandidateLocalManagedObject *pMo)
{
    trace(TRACE_LEVEL_DEBUG, string("PimBsrCandClientMsg::getCStructureForInputs:"
        " Entered...."));

    /* Now copy the data members into the structure variables */
    msg->m_msgType = PIM_MSG_ADD;
    msg->m_intfType = pMo->getIntfType();
    msg->m_intfName = pMo->getIntfName();
	msg->m_hashMask = pMo->getMask();
	msg->m_priority = pMo->getPriority();
   
    trace(TRACE_LEVEL_DEBUG, string("msg_type: ") + toString(msg->m_msgType));
    trace(TRACE_LEVEL_DEBUG, string("intf_type: ") + toString(msg->m_intfType));
    trace(TRACE_LEVEL_DEBUG, string("intf_name: ") + toString(msg->m_intfName));
    trace(TRACE_LEVEL_DEBUG, string("hash_mask: ") + toString(msg->m_hashMask));
    trace(TRACE_LEVEL_DEBUG, string("priority: ") + toString(msg->m_priority));
    
    return;
}



 
    /* Function to restore the BSR candidate configs. Will be called for both
     * the global and local stages of postboot. 
     * During global stage, BSR candidates referring to VLAN and LAG will be 
     * restored. In this case slotNum will be -1.
     * During the local/slot stage, BSR candidates referring to physical 
     * interfaces will be restored. In this case slotNum will be the slot 
     * associated with the interface */
    void
    PimLocalObjectManager::restoreBsrCandCfg(SI32 slotNum)
    {
        BsrCandidateLocalManagedObject *pMo = NULL;
        PimBsrCandClientMsg msg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG,
            string("PimLocalObjectManager::restoreBsrCandCfg:") +
            string("Entered..."));

        /* Query for all RP candidate MO's that are associated with this 
         * slot */
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtx(BsrCandidateLocalManagedObject::getClassName());
        syncQueryCtx.setSlotFilter(slotNum);
        vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtx);

        if (pResults && pResults->size () == 0) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimLocalObjectManager::restoreBsrCandCfg:") +
                string("PIM config does not exists"));
            /* Free memory */
            if(pResults != NULL)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return;
        }

        for(UI32 i = 0; i < pResults->size(); i++) {
            pMo = dynamic_cast<BsrCandidateLocalManagedObject *>((*pResults)[i]);
            
            /* Populate the fields of the message */
            setPimBsrCandClientMsgFieldsForReplay(&msg, pMo);

            /* Send message to the daemon */
            status = sendSynchronouslyToWaveClient("pim", &msg);
            if (status == WAVE_MESSAGE_SUCCESS)
                status = msg.getCompletionStatus();
                //if (status == WAVE_MESSAGE_SUCCESS)
                //    status = ::mapRtmBackEndErrorsToDcmdErrors(msg.getClientStatus());
            /* Check daemon status code */
            if (status != WAVE_MESSAGE_SUCCESS)
                trace(TRACE_LEVEL_ERROR,
                    string("PimLocalObjectManager::replayRpCandCfg:") +
                    string("Replay of BSR candidate config failed"));
            else
                trace(TRACE_LEVEL_INFO,
                    string("PimLocalObjectManager::replayRpCandCfg:") +
                    string("Replay of BSR candidate config succeeded"));
        }

        /* Free memory */
        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return;
    }

    /* PIM implementation of the virtual function postboot which will be 
     * called to replay the config */
    void
    PimLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *phase)
    {
        string passName  = phase->getPassName();

        trace(TRACE_LEVEL_INFO, 
            string("PimLocalObjectManager::postboot Entered with pass:") +
            passName);

        // Check for Layer3 license before sending to backend
        if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
            trace(TRACE_LEVEL_DEBUG, "PimLocalObjectManager::postboot. LAYER_3_LICENSE not present");
            phase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            phase->callback();
            return;
        }

        if(passName.compare("DCM_POSTBOOT_GLOBAL_STAGE5") == 0) {
            trace (TRACE_LEVEL_INFO, 
                string("PimLocalObjectManager::postboot started:") + passName);
            
            /* First restore the variables under router pim cmd */
            restorePimRtrCfg();
            
            /* Restore any static RP config */
            restoreStaticRpCfg();
			
            /* Restore any RP candidate configs present on VLAN and LAG 
             * interfaces */

			/* Supressing the restore since RpCandidateLocalManagedObject is not supported for NOS4.0.0 */
            //restoreRpCandCfg(WAVE_INVALID_SLOT);
           
			/* Restore any RP Candidate Prefix config */

			/* Supressing the restore since RpCandidatePrefixLocalManagedObject is not supported for NOS4.0.0 */
            //restoreRpCandPrefixCfg();
 
            /* Restore any BSR candidate configs present on VLAN and LAG 
             * interfaces */

			/* Supressing the restore since BsrCandidateLocalManagedObject is not supported for NOS4.0.0 */
            //restoreBsrCandCfg(WAVE_INVALID_SLOT);
            
            /* Restore config under VLAN interface */
            restoreVlanIntfPimCfg();
           
            /* Restore config under port-channel interface */
            restorePoIntfPimCfg();
            
            trace (TRACE_LEVEL_INFO, 
                string("PimLocalObjectManager::postboot done:") + passName);
        }

        phase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        phase->callback();

        return;
    }
    
    /* PIM implementation of the virtual function slotFailover which will be 
     * called when clear linecard cmd is issued */
    void 
    PimLocalObjectManager::slotFailover(
        WaveAsynchronousContextForSlotFailover *pCtxt)
    {
        int32_t slotId = pCtxt->getSlotNumber();
        LocationId locId = FrameworkToolKit::getThisLocationId();
        
        trace(TRACE_LEVEL_INFO, string("PimLocalObjectManager::slotFailover: ")
            + string("Start... slot = ") + slotId 
            + string(" LocationId = ") + locId);
            
        /* Since multicast is not supported under VCS mode, do not do any 
         * slotFailover processing if we are called under VCS mode */
        if(DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            pCtxt->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            pCtxt->callback();
         
            trace(TRACE_LEVEL_INFO, string("PimLocalObjectManager::slotFailover: ")
                + string("Done... slot = ") + slotId 
                + string(" LocationId = ") + locId);

            return;
        }

        startTransaction();

        /* Delete RP candidate MO's referencing any physical interfaces on this
         * slot */
        delRpCandOnSlotFailover(slotId);

        /* Delete BSR candidate MO's referencing any physical interfaces on this
         * slot */
        delBsrCandOnSlotFailover(slotId);

        commitTransaction();
        
        pCtxt->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pCtxt->callback();
        
        trace(TRACE_LEVEL_INFO, string("PimLocalObjectManager::slotFailover: ")
            + string("Done... slot = ") + slotId 
            + string(" LocationId = ") + locId);
            
        return;
    }

    /* This function removes all RP candidate MO that reference to physical 
     * interfaces which belong to the slot being powered off */
    void 
    PimLocalObjectManager::delRpCandOnSlotFailover(SI32 slotNum)
    {
        PimRtrCfgLocalManagedObject *pRtrMo = NULL;
        RpCandidateLocalManagedObject *pMo = NULL;

        trace(TRACE_LEVEL_DEBUG,
            string("PimLocalObjectManager::delRpCandOnSlotFailover:") +
            string("Entered..."));

        /* First query for the PIM router MO */
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtx(PimRtrCfgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtx);

        if (pResults && pResults->size () == 0) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimLocalObjectManager::delRpCandOnSlotFailover:") +
                string("PIM config does not exists"));
            /* Free memory */
            if(pResults != NULL)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return;
        }
        else {
            pRtrMo = dynamic_cast<PimRtrCfgLocalManagedObject *>((*pResults)[0]);
        }

        /* Now query for the RP candidate MO's associated with this slot */
        WaveManagedObjectSynchronousQueryContext
            queryCtx(RpCandidateLocalManagedObject::getClassName());
        queryCtx.setSlotFilter(slotNum);
        pResults = querySynchronously(&queryCtx);
        
        if (pResults && pResults->size () == 0) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimLocalObjectManager::delRpCandOnSlotFailover:") +
                string("PIM RP candidate config does not exists"));
            /* Free memory */
            if(pResults != NULL)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return;
        }

        /* Iterate through all RP Candidates and remove them from the 
         * PIM router MO */
        for(UI32 i = 0; i < pResults->size(); i++) {
            pMo = dynamic_cast<RpCandidateLocalManagedObject *>((*pResults)[i]);

            pRtrMo->delCandRp(pMo->getIntfType(), pMo->getIntfName()); 
        }

        /* Mark the parent MO for updation */
        updateWaveManagedObject(pRtrMo);

        /* Free memory */
        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return;
    } 

    /* This function removes all BSR candidate MO that reference to physical 
     * interfaces which belong to the slot being powered off */
    void 
    PimLocalObjectManager::delBsrCandOnSlotFailover(SI32 slotNum)
    {
        PimRtrCfgLocalManagedObject *pRtrMo = NULL;
        BsrCandidateLocalManagedObject *pMo = NULL;

        trace(TRACE_LEVEL_DEBUG,
            string("PimLocalObjectManager::delBsrCandOnSlotFailover:") +
            string("Entered..."));

        /* First query for the PIM router MO */
        WaveManagedObjectSynchronousQueryContext
            syncQueryCtx(PimRtrCfgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtx);

        if (pResults && pResults->size () == 0) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimLocalObjectManager::delBsrCandOnSlotFailover:") +
                string("PIM config does not exists"));
            /* Free memory */
            if(pResults != NULL)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return;
        }
        else {
            pRtrMo = dynamic_cast<PimRtrCfgLocalManagedObject *>((*pResults)[0]);
        }

        /* Now query for the BSR candidate MO's associated with this slot */
        WaveManagedObjectSynchronousQueryContext
            queryCtx(BsrCandidateLocalManagedObject::getClassName());
        queryCtx.setSlotFilter(slotNum);
        pResults = querySynchronously(&queryCtx);
        
        if (pResults && pResults->size () == 0) {
            trace(TRACE_LEVEL_DEBUG,
                string("PimLocalObjectManager::delBsrCandOnSlotFailover:") +
                string("PIM BSR candidate config does not exists"));
            /* Free memory */
            if(pResults != NULL)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return;
        }

        /* Iterate through all BSR Candidates and remove them from the 
         * PIM router MO */
        for(UI32 i = 0; i < pResults->size(); i++) {
            pMo = dynamic_cast<BsrCandidateLocalManagedObject *>((*pResults)[i]);

            pRtrMo->delBsrCand(pMo->getIntfType(), pMo->getIntfName()); 
        }

        /* Mark the parent MO for updation */
        updateWaveManagedObject(pRtrMo);

        /* Free memory */
        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        return;
    } 
}
