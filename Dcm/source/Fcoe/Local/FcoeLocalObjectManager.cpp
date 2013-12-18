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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalWorker.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

// FCOE Fabric Map MO
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "Fcoe/Global/FcoeGlobalFcoeFabricMapMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeFabricMapMessage.h"

// FCOE MAP MO
#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Fcoe/Global/FcoeGlobalFcoeMapMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeMapMessage.h"

// FCOE FIP MO
#include "Fcoe/Global/FcoeFipManagedObject.h"
#include "Fcoe/Global/FcoeGlobalFipMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeFipMessage.h"

// FCOE FSB MO
#include "Fcoe/Global/FcoeGlobalFsbManagedObject.h"
#include "Fcoe/Global/FcoeGlobalFcoeFsbMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeFsbMessage.h"

#include "Fcoe/Global/FcoeGlobalFcoeClearMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeClearMessage.h"

#include "Fcoe/Global/FcoeGlobalFcoeShowMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeShowMessage.h"

#include "Fcoe/Global/FcoeGlobalTypes.h"

#include "Fcoe/Common/FcoePluginDebug.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Common/FcoePluginUtils.h"
#include "Qos/Global/CeeMapManagedObject.h"

#include "Nsm/Global/NsmGlobalFcoeVlanMessage.h"
#include "Nsm/Local/NsmLocalFcoeVlanMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"

#include "ClientInterface/Fcoe/FcoeClientFabricMapMessage.h"
#include "ClientInterface/Fcoe/FcoeClientFipMessage.h"
#include "ClientInterface/Fcoe/FcoeClientMapMessage.h"
#include "ClientInterface/Fcoe/FcoeClientShowMessage.h"
#include "ClientInterface/Fcoe/FcoeClientIntfMessage.h"
#include "ClientInterface/Fcoe/FcoeClientClearMessage.h"
#include "ClientInterface/Fcoe/FcoeClientETIntfMessage.h"
#include "ClientInterface/Fcoe/FcoeClientFsbMessage.h"

#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/LldpFcoePriorityConfMessage.h"
#include "ClientInterface/Lldp/LldpFcoeClientFsbMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "Fcoe/Common/FcoePluginUtils.h"
#include "AG/Common/AgUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Fcoe/Local/FcoeEnodeLocalManagedObject.h"
namespace DcmNs
{
    FcoeLocalObjectManager::FcoeLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        associateWithVirtualWaveObjectManager (FcoeGlobalObjectManager::getInstance());

        postbootStartedInAgMode = false;
        pPostBootPhaseCtxAgMode = NULL;

        m_pFcoeLocalWorker = new FcoeLocalWorker (this);
        prismAssert (NULL != m_pFcoeLocalWorker, __FILE__, __LINE__);
        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
    }

    FcoeLocalObjectManager::~FcoeLocalObjectManager ()
    {
    }

    FcoeLocalObjectManager  *FcoeLocalObjectManager::getInstance()
    {
        static FcoeLocalObjectManager *pFcoeLocalObjectManager = new FcoeLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFcoeLocalObjectManager, __FILE__, __LINE__);

        return (pFcoeLocalObjectManager);
    }

    PrismServiceId  FcoeLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FcoeLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FcoeLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId
    FcoeLocalObjectManager::FcoeLocalPostBootFcoeVlans()
    {
        FcoeFabricMapManagedObject  *fab_map_mo_p;
        NsmLocalFcoeVlanMessage     *nsm_msg_p = NULL;
        ResourceId                  status = WAVE_MESSAGE_SUCCESS;
        
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFabricMapManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        UI32 i;
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0) {
            FCOE_PLUG_DBG_ERR("Query returned 0 results, MO not found");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return status;
        }

        for (i = 0; i < numberOfResults; i++) {
            fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *> ((*pResults)[i]);
            FCOE_PLUG_ASSERT_PTR(fab_map_mo_p);

            nsm_msg_p = new NsmLocalFcoeVlanMessage(NSM_FCOE_UPDATE_VLAN,
                                                    FCOE_FAB_MAP_DEFAULT_VLAN,
                                                    fab_map_mo_p->getVlan(),
                                                    "FCOE VLAN", 0 , "FCOE VLAN");

            status = sendSynchronously(nsm_msg_p);
            if (status != WAVE_MESSAGE_SUCCESS ||
                nsm_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                FCOE_PLUG_DBG_S(string("Failed to send message to NSM Global") + status +
                                string(" Client-Status = ") +
                                FrameworkToolKit::localize(nsm_msg_p->getCompletionStatus()));
            }
            delete nsm_msg_p;

        }
 
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
           
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalObjectManager::FcoeLocalPostBootFabricMapConfig()
    {
        FcoeFabricMapManagedObject  *fab_map_mo_p;
        FcoeClientFabricMapMessage  *cl_msg_p = NULL;
        UI32                        oper_bmp = 0;
        ResourceId                  status = WAVE_MESSAGE_SUCCESS;
        LldpFcoePriorityConfMessage *lldp_msg_p = NULL;
        UI32                        priority = 0;
        UI32                        priority_bitmap = 0;
        UI32                        fcoe_enodes = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFabricMapManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        UI32 i;
        string fcoe_enodes_config_mode("global");
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0) {
            FCOE_PLUG_DBG_ERR("Query returned 0 results, MO not found");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return status;
        }

        for (i = 0; i < numberOfResults; i++) {
            fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *> ((*pResults)[i]);
            FCOE_PLUG_ASSERT_PTR(fab_map_mo_p);

            cl_msg_p = new FcoeClientFabricMapMessage();
            FCOE_PLUG_ASSERT_PTR(cl_msg_p);

            cl_msg_p->setFabric_map_name(fab_map_mo_p->getName());
            cl_msg_p->setConfigReplay();

            oper_bmp |= FCOE_FAB_MAP_OPER_UPD_VLAN;
            cl_msg_p->setVlan(fab_map_mo_p->getVlan());

			oper_bmp |= FCOE_FAB_MAP_OPER_UPD_VFID;
			cl_msg_p->setVfid(fab_map_mo_p->getVfid());
            
            oper_bmp |= FCOE_FAB_MAP_OPER_UPD_PRIORITY;
            cl_msg_p->setPriority(fab_map_mo_p->getPriority());

            if (fab_map_mo_p->getPriority() != FCOE_FAB_MAP_DEFAULT_PRIORITY) {
            
                priority = fab_map_mo_p->getPriority();
                priority_bitmap = 1 << priority;
                lldp_msg_p = new LldpFcoePriorityConfMessage(0, priority_bitmap);
                FCOE_PLUG_ASSERT_PTR(lldp_msg_p);

                status = sendSynchronouslyToWaveClient(LLDP_DCM_CLIENT_NAME,
                                                       lldp_msg_p);
                if (status != WAVE_MESSAGE_SUCCESS ||
                    lldp_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                    FCOE_PLUG_DBG_S(string("Failed to send message to LLDP Status=") + status +
                                    string(" Completion-Status = ") +
                                    lldp_msg_p->getCompletionStatus());
                }

                delete lldp_msg_p;
            }
            
            string fcmap = fab_map_mo_p->getFcmap();
            oper_bmp |= FCOE_FAB_MAP_OPER_UPD_FCMAP;
            cl_msg_p->setFcmap(fcmap);

            oper_bmp |= FCOE_FAB_MAP_OPER_UPD_FKA_INTVL;
            cl_msg_p->setFka_intvl(fab_map_mo_p->getFka_intvl());

            oper_bmp |= FCOE_FAB_MAP_OPER_UPD_TIMEOUT;
            cl_msg_p->setTimeout(fab_map_mo_p->getTimeout());

            oper_bmp |= FCOE_FAB_MAP_OPER_UPD_MTU;
            cl_msg_p->setMtu(fab_map_mo_p->getMtu());

            oper_bmp |= FCOE_FAB_MAP_OPER_UPD_MAX_ENODES;
           
            if(!(fcoe_enodes_config_mode.compare (fab_map_mo_p->getEnodesCfgMode()))) { // max-enodes
                cl_msg_p->setMaxEnodes(fab_map_mo_p->getMaxEnodes());
            } else { // fcoe-enodes 
                // FcoeLocalObjectManager::getFcoeEnodes(fcoe_enodes, status, upgradeFlag);
                FcoeLocalObjectManager::fcoeEnodeSanityCheckForSystemUpgrade(fcoe_enodes, status);
			    if(status == WAVE_MESSAGE_SUCCESS) { 
					cl_msg_p->setMaxEnodes(fcoe_enodes);

			     } else {
                     FCOE_PLUG_DBG_ERR("FCoE Sanity check failed");
                     return status;
                 }
            }

            if (oper_bmp) {
                cl_msg_p->setOper_bmp(oper_bmp);
                FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);
            }
        }
 
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
           
        return status;
    }

    ResourceId
    FcoeLocalObjectManager::FcoeLocalPostBootMapConfig()
    {
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalObjectManager::FcoeLocalPostBootFipConfig()
    {
        FcoeFipManagedObject        *fip_mo_p;
        FcoeClientFipMessage        *cl_msg_p = NULL;
        UI32                        oper_bmp = 0;
        ResourceId                  status = WAVE_MESSAGE_SUCCESS;
        
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFipManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0) {
            FCOE_PLUG_DBG_ERR("Query returned 0 results, MO not found");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return status;
        }

        fip_mo_p = dynamic_cast<FcoeFipManagedObject *> ((*pResults)[0]);
        FCOE_PLUG_ASSERT(fip_mo_p);

        cl_msg_p = new FcoeClientFipMessage();
        FCOE_PLUG_ASSERT(cl_msg_p);

        if (fip_mo_p->getFka_intvl() != FCOE_DEFAULT_FKA_INTVL) {
            oper_bmp |= FCOE_FIP_OPER_UPD_TIMEOUT;
            cl_msg_p->setFka_intvl(fip_mo_p->getFka_intvl());
        }

        if (fip_mo_p->getTimeout() != FCOE_FIP_DEFAULT_TIMEOUT) {
            oper_bmp |= FCOE_FIP_OPER_UPD_FKA_INTVL;
            cl_msg_p->setTimeout(fip_mo_p->getTimeout());
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        if (oper_bmp == 0) {
            delete cl_msg_p;
            return WAVE_MESSAGE_SUCCESS;
        }

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

        return status;
    }
    
    ResourceId
    FcoeLocalObjectManager::FcoeLocalPostBootFsbConfig()
    {
        FcoeGlobalFsbManagedObject        *fsb_mo_p;
        FcoeClientFsbMessage      *cl_msg_p = NULL;
        ResourceId                  status = WAVE_MESSAGE_SUCCESS;
        
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeGlobalFsbManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0) {
            FCOE_PLUG_DBG_ERR("Query returned 0 results, MO not found");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return status;
        }

        fsb_mo_p = dynamic_cast<FcoeGlobalFsbManagedObject *> ((*pResults)[0]);
        FCOE_PLUG_ASSERT(fsb_mo_p);

        cl_msg_p = new FcoeClientFsbMessage(fsb_mo_p->getMode());
        FCOE_PLUG_ASSERT(cl_msg_p);

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	    FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

         if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
            LldpFcoeClientFsbMessage *lldp_cl_msg_p = NULL;
            lldp_cl_msg_p = new LldpFcoeClientFsbMessage(fsb_mo_p->getMode());
            FCOE_SEND_SYNC_TO_DAEMON(LLDP_DCM_CLIENT_NAME, lldp_cl_msg_p, status, FCOE_DEL_CLIENT_MSG);
        }

        return status;
    }

    ResourceId
    FcoeLocalObjectManager::FcoeLocalDownloadETInterfaceBinding()
    {
        FcoeETIntfLocalManagedObject *if_mo_p = NULL;
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId (), FcoeETIntfLocalManagedObject::getClassName());
        FcoeClientETIntfMessage   *cl_msg_p;
        ResourceId status;
        string if_name;

        UI32 numberOfResults = pResults->size ();

        FCOE_PLUG_DBG_S(string("ET_CONFIG_REPLAY: Found ") + numberOfResults + string(" ET Interfaces"));

        UI32 size = sizeof (fcoe_et_intf_config_msg_t);
        if (numberOfResults) {
            size += (numberOfResults - 1) * sizeof(fcoe_et_intf_confg_if_list_entry_t);
        }

        fcoe_et_intf_config_msg_t *msg_p = (fcoe_et_intf_config_msg_t *)malloc(size);
        if (msg_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to ALLOC IF_BIND_DOWNLOAD Message");
            return FCOE_IF_ERR_FAILED_ALLOC_MESSAGE_BUFFER;
        }

        cl_msg_p = new FcoeClientETIntfMessage();
        if (cl_msg_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to ALLOC IF_BIND_DOWNLOAD Message");
            free(msg_p);
            return FCOE_IF_ERR_ALLOC_CLIENT_MSG;
        }
        
        cl_msg_p->setOper_bmp(FCOE_ET_INTF_OPER_CONFIG_REPLAY);
        
        memset(msg_p, 0, size);

        msg_p->cmd_code = FCOE_ET_INTF_OPER_CONFIG_REPLAY;
        msg_p->num_entries = numberOfResults;
        msg_p->size = size;
      
        for (UI32 i = 0; i < numberOfResults; i++) {
            if_mo_p = dynamic_cast<FcoeETIntfLocalManagedObject *>((*pResults)[i]);
            
            if_name = if_mo_p->getName();
            strncpy((char*)msg_p->list[i].if_name, if_name.c_str(), FCOE_PHY_INTF_NAME_LEN_MAX); 
            msg_p->list[i].if_index = if_mo_p->getIfindex();
            msg_p->list[i].fc_port_num = if_mo_p->getFcUportNum();
            msg_p->list[i].status = if_mo_p->getStatus();
            msg_p->list[i].trunk_status = if_mo_p->getTrunkStatus();
        }

        cl_msg_p->addBuffer(msg_p->cmd_code, size, msg_p, true);

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

        return  WAVE_MESSAGE_SUCCESS;
    }


    ResourceId
    FcoeLocalObjectManager::FcoeLocalDownloadInterfaceBinding()
    {
        FcoeIntfLocalManagedObject *if_mo_p = NULL;
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId (), FcoeIntfLocalManagedObject::getClassName());
        FcoeClientIntfMessage   *cl_msg_p;
        ResourceId status;
        string bind_if_name;
        
        UI32 numberOfResults = pResults->size ();
        UI32 size = sizeof (fcoe_intf_config_msg_t);
        if (numberOfResults) {
            size += (numberOfResults - 1) * sizeof(fcoe_intf_confg_if_list_entry_t);
        }

        fcoe_intf_config_msg_t *msg_p = (fcoe_intf_config_msg_t *)malloc(size);
        if (msg_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to ALLOC IF_BIND_DOWNLOAD Message");
            return FCOE_IF_ERR_FAILED_ALLOC_MESSAGE_BUFFER;
        }

        cl_msg_p = new FcoeClientIntfMessage();
        if (cl_msg_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to ALLOC IF_BIND_DOWNLOAD Message");
            free(msg_p);
            return FCOE_IF_ERR_ALLOC_CLIENT_MSG;
        }
        
        cl_msg_p->setOper_bmp(FCOE_INTF_OPER_CONFIG_REPLAY);

        FCOE_PLUG_DBG_S(string("VX_CONFIG_REPLAY: Found ") + numberOfResults + string(" VX Interfaces"));
        
        memset(msg_p, 0, size);

        msg_p->cmd_code = FCOE_INTF_OPER_CONFIG_REPLAY;
        msg_p->num_entries = numberOfResults;
      
        for (UI32 i = 0; i < numberOfResults; i++) {
            if_mo_p = dynamic_cast<FcoeIntfLocalManagedObject *>((*pResults)[i]);
           
            msg_p->list[i].if_index = if_mo_p->getIfindex();
            msg_p->list[i].port_num = if_mo_p->getPortNum();
            msg_p->list[i].vn_num = if_mo_p->getVnNum();
            msg_p->list[i].fc_port_num = if_mo_p->getFcUportNum();
            msg_p->list[i].status = !(if_mo_p->getStatus());
            msg_p->list[i].bind_type = if_mo_p->getBindingType();
            bind_if_name = if_mo_p->getBinding();
            memcpy(msg_p->list[i].bind_name, bind_if_name.c_str(),
                   FCOE_PHY_INTF_NAME_LEN_MAX);
        }

        cl_msg_p->addBuffer(msg_p->cmd_code, size, msg_p, true);

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

        return  WAVE_MESSAGE_SUCCESS;
    }
                                                        
    void
    FcoeLocalObjectManager::externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        /*
        FCOE_PLUG_DBG_S(string("Recvd FSS Stage SYNC for Stage = ") + 
                        pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() +
                        string(" SRVC =") + 
                        pWaveAsynchronousContextForExternalStateSynchronization->getServiceType());

        if (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == CHASSIS) {
            FCOE_PLUG_DBG_S(string("Rcvd FSS STAGE for CHASSIS"));
        }
        */

        if ((pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == ETHSW) &&
            (pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 3)) {
            FCOE_PLUG_DBG_S(string("Rcvd FSS STAGE for ETHSW for Stage 3"));
            status = FcoeLocalObjectManager::FcoeLocalDownloadInterfaceBinding();
            if (status != WAVE_MESSAGE_SUCCESS) {
                FCOE_PLUG_DBG_ERR_S(string("Failed in VX intf CFG Replay. Status=") + status);
            }

            status = FcoeLocalObjectManager::FcoeLocalDownloadETInterfaceBinding();
            if (status != WAVE_MESSAGE_SUCCESS) {
                FCOE_PLUG_DBG_ERR_S(string("Failed in ET intf CFG Replay Status=") + status);
            }

            FCOE_PLUG_DBG_S(string("Completed FCOE_IF_CONFIG_REPLAY."));
        }

        /*
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) &&
            (pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 5)) {
            FCOE_PLUG_DBG_S(string("Rcvd FSS STAGE for FCSW for Stage 5"));
        }
        */

        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();

        return;

    }

    void
    FcoeLocalObjectManager::slotFailover (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
    {
        UI32    slot_id = 0;
        UI32    loc_id = 0;
        vector<WaveManagedObject *> *pResultsET = NULL;
        vector<WaveManagedObject *> *pResultsVF = NULL;
        WaveManagedObjectSynchronousQueryContext    etSyncQueryCtxt(FcoeETIntfLocalManagedObject::getClassName());
        WaveManagedObjectSynchronousQueryContext    vfSyncQueryCtxt(FcoeIntfLocalManagedObject::getClassName());
        FcoeETIntfLocalManagedObject    *pETIntfMo;
        FcoeIntfLocalManagedObject      *pVFIntfMo;

        FcoeClientETIntfMessage         *cl_msg_p;
        fcoe_intf_config_msg_t          *msg_p = NULL;
        UI32                            size = 0;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        slot_id = pWaveAsynchronousContextForSlotFailover->getSlotNumber();
        loc_id = FrameworkToolKit::getThisLocationId();

        if (loc_id != 0) {
            etSyncQueryCtxt.setLocationIdFilter(loc_id);
            vfSyncQueryCtxt.setLocationIdFilter(loc_id);
        }

        if (slot_id != 0) {
            etSyncQueryCtxt.setSlotFilter(slot_id);
            vfSyncQueryCtxt.setSlotFilter(slot_id);
        }

        startTransaction();
        
        pResultsET = querySynchronously(&etSyncQueryCtxt);
        if (pResultsET) {
            UI32 numberOfResults = pResultsET->size ();
            size = sizeof (fcoe_intf_config_msg_t);
            if (numberOfResults) {
                size += (numberOfResults - 1) * sizeof(fcoe_intf_confg_if_list_entry_t);
            }

            msg_p = (fcoe_intf_config_msg_t *)malloc(size);
            if (msg_p == NULL) {
                FCOE_PLUG_DBG_ERR("Failed to ALLOC SLOT_FAILOVER Message");
                pWaveAsynchronousContextForSlotFailover->setCompletionStatus(FCOE_IF_ERR_FAILED_SLOT_FAILOVER_MESSAGE);
                pWaveAsynchronousContextForSlotFailover->callback();
                return;
            }

            msg_p->cmd_code = FCOE_ET_INTF_OPER_SLOT_FAILOVER;
            msg_p->num_entries = numberOfResults;

            cl_msg_p = new FcoeClientETIntfMessage();
            if (cl_msg_p == NULL) {
                FCOE_PLUG_DBG_ERR("Failed to ALLOC IF_BIND_DOWNLOAD Message");
                free(msg_p);
                pWaveAsynchronousContextForSlotFailover->setCompletionStatus(FCOE_IF_ERR_ALLOC_CLIENT_MSG);
                pWaveAsynchronousContextForSlotFailover->callback();
                return;
            }

            cl_msg_p->setOper_bmp(FCOE_ET_INTF_OPER_SLOT_FAILOVER);

                
            for (UI32 i = 0; i < numberOfResults; ++i) {
                pETIntfMo = dynamic_cast<FcoeETIntfLocalManagedObject *> ((*pResultsET)[i]);
                FCOE_PLUG_DBG_S(string("Deleting Config for ET IF ") +
                                pETIntfMo->getName());

                msg_p->list[i].if_index = pETIntfMo->getIfindex();

                delete pETIntfMo;
            }

            cl_msg_p->addBuffer(msg_p->cmd_code, size, msg_p, true);

            FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

            delete pResultsET;
        }
        
        pResultsVF = querySynchronously(&vfSyncQueryCtxt);
        if (pResultsVF) {
            UI32 numberOfResults = pResultsVF->size ();
            for (UI32 i = 0; i < numberOfResults; ++i) {
                pVFIntfMo = dynamic_cast<FcoeIntfLocalManagedObject *> ((*pResultsVF)[i]);
                FCOE_PLUG_DBG_S(string("Deleting Config for VF IF ") +
                                pVFIntfMo->getName());

                delete pVFIntfMo;
            }

            delete pResultsVF;
        }

        commitTransaction();

        FCOE_PLUG_DBG_S(string("Slot Failover for Slot=") + slot_id +
                        string(" Loc=") + loc_id +
                        string(" Completed successfully."));

        pWaveAsynchronousContextForSlotFailover->setCompletionStatus(status);
        pWaveAsynchronousContextForSlotFailover->callback();
        
    }

    bool
    FcoeLocalObjectManager::intfCountMaxEnodeChange(void)
    {
        // Get the max enode config
        FcoeEnodeLocalManagedObject *enode_mo_p = NULL;
        UI32 loc_id = FrameworkToolKit::getThisLocationId();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeEnodeLocalManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(loc_id, &syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_ERR("Query returned 0 or >1 results, MO not found");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return false;
        }

        enode_mo_p = dynamic_cast<FcoeEnodeLocalManagedObject *> ((*pResults)[0]);
        FCOE_PLUG_ASSERT_PTR(enode_mo_p);

        UI32 newIntfCount = enode_mo_p->getFcoeEnodes();

        // Get the current interface count in the DB
        vector<WaveManagedObject *> *pIntfResults = 
            querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId (),
                FcoeIntfLocalManagedObject::getClassName());

        UI32 currIntfCount = pIntfResults->size ();

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pIntfResults);

        FCOE_PLUG_DBG_S(string("newIntfCount = ") + newIntfCount + string(" currIntfCount = ") + currIntfCount);

        if (newIntfCount != currIntfCount)
            return (true); // Max enode config is different from the current interface count
        else
            return (false);
    }

    void
    FcoeLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string         name  = pWaveAsynchronousContextForPostBootPhase->getPassName();
        ResourceId  status = WAVE_MESSAGE_SUCCESS;

        FCOE_PLUG_DBG_S(string("Postboot called for Pass=") + name);
        if (name != "DCM_POSTBOOT_GLOBAL_STAGE1" &&
                name != "DCM_POSTBOOT_GLOBAL_STAGE2") {
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE1") {
            // In the FIRST Pass, we need to only create FCOE VLANs
            status = FcoeLocalObjectManager::FcoeLocalPostBootFcoeVlans();
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

        status = FcoeLocalPostBootFabricMapConfig();
        if (status != WAVE_MESSAGE_SUCCESS) {
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

        status = FcoeLocalPostBootMapConfig();
        if (status != WAVE_MESSAGE_SUCCESS) {
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }
       
        status = FcoeLocalPostBootFipConfig();
        if (status != WAVE_MESSAGE_SUCCESS) {
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }
        
        status = FcoeLocalPostBootFsbConfig();
        if (status != WAVE_MESSAGE_SUCCESS) {
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

        // In AG mode, the postboot phase is returned asynchronously after the INTF MOs
        // are created from FcoeLocalWorker as part of a FCOE_INTF_OPER_CREATE_BULK handler.
        // However, in certain scenarios where the interfaces are already created in the DB
        // and the max_enode value is equal to the number of current interfaces,
        // the FCOE_INTF_OPER_CREATE_BULK handler is not triggered by the Fcoe Daemon => this
        // will hang the Fcoe Plugin and the entire DCMd in turn. Hence postpone the postboot
        // callback only if the interface counts are different.
        if (AgUtils::isAgModeEnabled() && intfCountMaxEnodeChange()) {
            FCOE_PLUG_DBG("AG mode is enabled, saving postboot ctx");
            postbootStartedInAgMode = true;
            pPostBootPhaseCtxAgMode = pWaveAsynchronousContextForPostBootPhase;
        } else {
            FCOE_PLUG_DBG("AG mode is disabled or no change in intf count, returning postboot callback");
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (status);
            pWaveAsynchronousContextForPostBootPhase->callback ();
        }

        return;
    }

 // This function checks the upgrade or normal reboot . If the upgrade flag is set it will send the request to daemon 
 // to get the bound ports defails. 
 void    
    FcoeLocalObjectManager::fcoeEnodeSanityCheckForSystemUpgrade(UI32 &fcoe_enodes, ResourceId  &status)
	{
		FcoeEnodeLocalManagedObject *enode_mo_p = NULL;
		UI32 loc_id = FrameworkToolKit::getThisLocationId();
        bool upgradeFlag = false;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeEnodeLocalManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(loc_id, &syncQueryCtxt);

		UI32 numberOfResults = pResults->size ();
		if (numberOfResults == 0 || numberOfResults > 1) {
			FCOE_PLUG_DBG("getFcoeEnodes :: Query returned");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			status = FCOE_ENODE_MO_NOT_FOUND;
			return ;
		}

		enode_mo_p = dynamic_cast<FcoeEnodeLocalManagedObject *> ((*pResults)[0]);
		upgradeFlag = enode_mo_p->getFcoeEnodesUpgradeFlag();
		FCOE_PLUG_DBG("fcoeEnodeSanityCheckForSystemUpgrade:: Checking the Upgrade Flag");

        // Here need to add new API to check the version. TBD
		if (upgradeFlag) {
			FcoeClientIntfMessage           *msg_p = NULL;
			fcoe_intf_config_msg_t          *data_p;
			UI32                             size;
			msg_p = new FcoeClientIntfMessage();
			msg_p->setOper_bmp(FCOE_INTF_OPER_GET_IF_BIND_LIST);

			status = sendSynchronouslyToWaveClient("fcoed", msg_p);
			if (status == WAVE_MESSAGE_SUCCESS) {
				FCOE_PLUG_DBG("Message successful");

				data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
				if (data_p != NULL) {
					trace(TRACE_LEVEL_INFO, string("Got the fcoe bound interfaces from backend: ") + data_p->num_entries);
					if(data_p->num_entries <= FCOE_DEFAULT_FCOE_ENODES_WITH_LICENSE) {
						// here check the license
						fcoe_enodes = FCOE_PLUGIN_GET_FCOE_ENODES();

					} else {
						fcoe_enodes = data_p->num_entries;
					}
					startTransaction();
					enode_mo_p->setFcoeEnodes(fcoe_enodes);
					enode_mo_p->setFcoeEnodesUpgradeFlag(false);
					updateWaveManagedObject(enode_mo_p);
					status = commitTransaction ();
					if (status == FRAMEWORK_SUCCESS) {
						status = WAVE_MESSAGE_SUCCESS;
					} else {
						FCOE_PLUG_ASSERT(0);
					}

				} else {
					FCOE_PLUG_DBG("Message successful, data is NULL");
					status = FCOE_ENODE_UPGRADE_SANITY_CHECK_FAIL; 
                    return; 
				}
			} else {
				FCOE_PLUG_DBG("Posting Message to backend Failed ");
				status = FCOE_ENODE_UPGRADE_SANITY_CHECK_FAIL;  
                return; 
			}

		} else {
            // Not an upgrade fetch the values from the MO to send to daemon.
            if(licenseCheck(BASE_FCOE_LICENSE) != 0) {
		  	     fcoe_enodes = enode_mo_p->getFcoeEnodes();
					trace(TRACE_LEVEL_INFO, string("fcoe_enodes : ") + fcoe_enodes);
            } else {
				startTransaction();
			    fcoe_enodes = FCOE_PLUGIN_GET_FCOE_ENODES();
			 	enode_mo_p->setFcoeEnodes(fcoe_enodes);
				enode_mo_p->setFcoeEnodesUpgradeFlag(false);
				updateWaveManagedObject(enode_mo_p);
				status = commitTransaction ();
				if (status == FRAMEWORK_SUCCESS) {
					status = WAVE_MESSAGE_SUCCESS;
				} else {
					FCOE_PLUG_ASSERT(0);
				}
            }   

		}

		status = WAVE_MESSAGE_SUCCESS;
		pResults->clear();
		delete pResults;
		delete enode_mo_p;
		return;

	}
}
