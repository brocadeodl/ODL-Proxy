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
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/FcoeWorker/NsmFcoeLocalWorker.h"
#include "Nsm/Local/FcoeWorker/FcoeLocalFcoeportAttrMessage.h"
#include "Nsm/Local/FcoeWorker/FcoeportAttrLocalManagedObject.h"
#include "Nsm/Local/FcoeWorker/NsmFcoeWorkerTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForFileReplayEnd.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"

#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/QosGlobalTypes.h"

#include "ClientInterface/Nsm/NsmFcoeportAttrMessage.h"
#include "ClientInterface/Fcoe/FcoeClientEthConfigMessage.h"

#include "Fcoe/Common/FcoePluginDebug.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Common/FcoePluginUtils.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Fcoe/Common/FcoeCommonWorker.h"

#include "Fcoe/Local/FcoeLocalWorker.h"
#include "Fcoe/Global/FcoeGlobalFsbManagedObject.h"

#include "Framework/Utils/FrameworkToolKit.h"

#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"


#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    NsmFcoeLocalWorker::NsmFcoeLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : FcoeCommonWorker  (pNsmLocalObjectManager)
    {
        FcoeportAttrLocalManagedObject    FcoeportAttrLocalManagedObject    (pNsmLocalObjectManager);
        FcoeportAttrLocalManagedObject.setupOrm ();
        addManagedClass (FcoeportAttrLocalManagedObject::getClassName (), this);
        addOperationMap (NSM_FCOE_WORKER_FCOEPORT_ATTR_LOCAL, reinterpret_cast<PrismMessageHandler> (&NsmFcoeLocalWorker::FcoeLocalFcoeportAttrMessageHandler));
    }

    NsmFcoeLocalWorker::~NsmFcoeLocalWorker ()
    {
    }

    PrismMessage  *NsmFcoeLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSM_FCOE_WORKER_FCOEPORT_ATTR_LOCAL:
                pPrismMessage = new FcoeLocalFcoeportAttrMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmFcoeLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FcoeportAttrLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FcoeportAttrLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "NsmFcoeLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void
    NsmFcoeLocalWorker::FcoeLocalFcoeportProcessBackend (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeportAttrMessage    *msg_p = dynamic_cast<FcoeLocalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        

        FCOE_PLUG_DBG_S(string("RCVD FCOEPORT Msg: Oper=") + msg_p->getOper_bmp() +
                        string(" IF=") + msg_p->getIfId() +
                        string(" Map-name=") + msg_p->getMap_name());

        if (msg_p->getOper_bmp() == 0) {
            FCOE_PLUG_DBG_ERR_S("Oper_BMP is Zero !!!!");
            ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        if (FrameworkToolKit::isFileReplayInProgress()) {
            if (msg_p->getIsALastConfigReplay() == false) {
                FCOE_PLUG_DBG_S(string("CONFIG REPLAY is in progress... buffering config."));
                ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
                return;
            } else {
                FCOE_PLUG_DBG_S(string("Config is part of Intent replay"));
            }
        }

        switch (msg_p->getOper_bmp()) {
            case FCOEPORT_ATTR_CREATE:
            case FCOEPORT_ATTR_REMOVE:
                status = NsmFcoeLocalWorker::FcoeLocalFcoeportAttrPhyPort(ctx_p);
                break;

            case FCOEPORT_ATTR_LAG_CREATE:
            case FCOEPORT_ATTR_LAG_REMOVE:
                status = NsmFcoeLocalWorker::FcoeLocalFcoeportAttrLagPort(ctx_p);
                break;
        }

        ctx_p->executeNextStep(status);
        return;
    }

    ResourceId
    NsmFcoeLocalWorker::FcoeLocalFcoeportAttrLagPort (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeportAttrMessage    *msg_p = dynamic_cast<FcoeLocalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());
        string                          if_id;
        string                          map_name;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        FcoeMapManagedObject            *map_mo_p = NULL;
        FcoeportAttrGlobalManagedObject  *fcoeport_mo_p = NULL;
        PoIntfManagedObject             *if_mo_p;
        UI64 ifindex = 0;

        if_id = msg_p->getIfId();
        
        if_mo_p = NsmFcoeLocalWorker::GetPoIntfMoById(if_id);
		if(if_mo_p != NULL) {
        	ifindex = if_mo_p->getIfIndex();
		}
        if(msg_p->get_prov_context() == FCOE_PROVISIONING_VIA_PORT_PROFILE_PORT) {
            map_name = msg_p->getMap_name();
            status = NsmFcoeLocalWorker::FcoeLocalFcoeportAttrSendToDaemon(ctx_p, map_name, ifindex);
            return status;

        }
        if (if_mo_p == NULL) {
            return (FCOE_PORT_ERR_INTF_MO_NOT_FOUND);
        }

        switch (msg_p->getOper_bmp()) {
            case FCOEPORT_ATTR_LAG_CREATE:
                map_name = msg_p->getMap_name();
                break;

            case FCOEPORT_ATTR_LAG_REMOVE:
                fcoeport_mo_p = NsmFcoeLocalWorker::FcoeportGetChildMoByParentLagMo(if_mo_p);
                if (fcoeport_mo_p == NULL) {
                    FCOE_PLUGIN_DELETE(if_mo_p);
                    return (FCOE_PORT_ERR_FCOEPORT_MO_NOT_FOUND);
                }
                
                map_mo_p = dynamic_cast<FcoeMapManagedObject *>(queryManagedObject(fcoeport_mo_p->getMap()));
                if (map_mo_p == NULL) {
                    FCOE_PLUGIN_DELETE(if_mo_p);
                    return (FCOE_PORT_ERR_MAP_NOT_FOUND);
                }

                map_name = map_mo_p->getMap_name();
                
                break;
        }

        status = NsmFcoeLocalWorker::FcoeLocalFcoeportAttrSendToDaemon(ctx_p, map_name, ifindex);

        FCOE_PLUGIN_DELETE(if_mo_p);
        FCOE_PLUGIN_DELETE(map_mo_p);

        return status;
    }

    ResourceId
    NsmFcoeLocalWorker::FcoeLocalFcoeportAttrPhyPort (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeportAttrMessage    *msg_p = dynamic_cast<FcoeLocalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());
        string                          if_id;
        string                          map_name;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        FcoeMapManagedObject            *map_mo_p = NULL;
        FcoeportAttrLocalManagedObject  *fcoeport_mo_p = NULL;
        PhyIntfLocalManagedObject       *if_mo_p;
        UI64 ifindex;

        if_id = msg_p->getIfId();
        
        if_mo_p = NsmFcoeLocalWorker::GetPhyIntfMoById(if_id);
        if (if_mo_p == NULL) {
            return (FCOE_PORT_ERR_INTF_MO_NOT_FOUND);
        }


        ifindex = if_mo_p->getIfIndex();
		/*
		 * Handel the port-profile-port case 
		 *
		 */
		
		if(msg_p->get_prov_context() == FCOE_PROVISIONING_VIA_PORT_PROFILE_PORT) {
			map_name = msg_p->getMap_name();
        	status = NsmFcoeLocalWorker::FcoeLocalFcoeportAttrSendToDaemon(ctx_p, map_name, ifindex);
        	return status;
			
		}
        switch (msg_p->getOper_bmp()) {
            case FCOEPORT_ATTR_CREATE:
                map_name = msg_p->getMap_name();
                if (if_mo_p->getPortrole()) {
                    return (NSM_API_ISL_INTERFACE_COMMAND_ERROR);
                }
                break;

            case FCOEPORT_ATTR_REMOVE:
                fcoeport_mo_p = NsmFcoeLocalWorker::FcoeportGetChildMoByParentMo(if_mo_p);
                if (fcoeport_mo_p == NULL) {
                   	FCOE_PLUGIN_DELETE(if_mo_p);
                   	return (FCOE_PORT_ERR_FCOEPORT_MO_NOT_FOUND);
                }
                
                map_mo_p = dynamic_cast<FcoeMapManagedObject *>(queryManagedObject(fcoeport_mo_p->getMap()));
                if (map_mo_p == NULL) {
                    FCOE_PLUGIN_DELETE(if_mo_p);
                    return (FCOE_PORT_ERR_MAP_NOT_FOUND);
                }

                map_name = map_mo_p->getMap_name();
                
                break;
        }

        status = NsmFcoeLocalWorker::FcoeLocalFcoeportAttrSendToDaemon(ctx_p, map_name, ifindex);

        FCOE_PLUGIN_DELETE(if_mo_p);
        FCOE_PLUGIN_DELETE(map_mo_p);

        return status;
    }

    FcoeClientEthConfigMessage*
    NsmFcoeLocalWorker::FcoeLocalCreateEthConfigMessage(UI32 oper, bool rollback)
    {
        UI32 cmd = 0;
        bool create_message = true;
        FcoeClientEthConfigMessage *dmn_msg_p = NULL;

        switch (oper) {
            case FCOEPORT_ATTR_CREATE:
                cmd = rollback ? FCOE_ETH_INTF_OPER_FCOEPORT_DEL
                               : FCOE_ETH_INTF_OPER_FCOEPORT_ADD;
                break;

            case FCOEPORT_ATTR_REMOVE:
                if (rollback == false) {
                    cmd = FCOE_ETH_INTF_OPER_FCOEPORT_DEL;
                } else {
                    FCOE_PLUG_DBG_S(string("Not creating RBK message for PHY_REMOVE"));
                    create_message = false;
                }
                break;

            case FCOEPORT_ATTR_LAG_CREATE:
                cmd = rollback ? FCOE_ETH_INTF_OPER_FCOEPORT_LAG_DEL
                               : FCOE_ETH_INTF_OPER_FCOEPORT_LAG_ADD;
                break;

            case FCOEPORT_ATTR_LAG_REMOVE:
                if (rollback == false) {
                    cmd = FCOE_ETH_INTF_OPER_FCOEPORT_LAG_DEL;
                } else {
                    FCOE_PLUG_DBG_S(string("Not creating RBK message for LAG_REMOVE"));
                    create_message = false;
                }
                break;

            default:
                FCOE_PLUG_DBG_S(string("Not creating message for UNK Oper=") + oper);    
                return NULL;
        }

        if (create_message) {
            dmn_msg_p = new FcoeClientEthConfigMessage();
            dmn_msg_p->setOper_bmp(cmd);
        }
        
        return dmn_msg_p;
    }

    ResourceId
    NsmFcoeLocalWorker::FcoeLocalSendEthConfigMessageToDaemon (NsmFcoeportAttrMessage *msg_p,
                                                               bool rollback)
    {
        FcoeClientEthConfigMessage  *dmn_msg_p = NULL;
        vector<UI32> vlan_vec;
        vector<UI64> ifindex_vec;
        UI32 oper = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        oper = msg_p->getOper_bmp();
        dmn_msg_p = FcoeLocalCreateEthConfigMessage(oper, rollback);
        if (dmn_msg_p == NULL) {
            return WAVE_MESSAGE_SUCCESS;
        }
        
        dmn_msg_p->setMap_name(msg_p->getMap_name());
        dmn_msg_p->setCeeMapName(msg_p->getCeeMapName());
        dmn_msg_p->setIfindex(msg_p->getIfindex());
        dmn_msg_p->setIfId(msg_p->getIfId());
        dmn_msg_p->setFcoeVlanVec(msg_p->getFcoeVlanVec());
        
        if (oper == FCOEPORT_ATTR_LAG_CREATE ||
            oper == FCOEPORT_ATTR_LAG_REMOVE) {
            UI32 i = 0;
			/*
			 * filter the local node interfaces to send to local daemon.
			 */
            getPoMemberIfindexListForLocalNode(msg_p->getIfId(), ifindex_vec);
            FCOE_PLUG_DBG_S(string("Retrieved Ifindex List of Size=") +
                            ifindex_vec.size());
            for (i = 0; i < ifindex_vec.size(); i++) {
                FCOE_PLUG_DBG_S(string("Retrieved Ifindex=") + ifindex_vec.at(i));
            }
            dmn_msg_p->setFcoeMbrIfindexVec(ifindex_vec);
        } 
        status = sendSynchronouslyToWaveClient (FCOE_DCM_CLIENT_NAME, dmn_msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Message Status = ") + status);
        }

        if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Client Status = ") + dmn_msg_p->getClientStatus());
            status = dmn_msg_p->getClientStatus();
        }

        if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("COmpletion Status = ") + dmn_msg_p->getCompletionStatus());
            status = dmn_msg_p->getCompletionStatus();
        }

        FCOE_PLUG_DBG_S(string("Final Status") + status);
        FCOE_PLUGIN_DELETE(dmn_msg_p);

        return (status);
    }

    ResourceId
    NsmFcoeLocalWorker::FcoeLocalFcoeportAttrSendToDaemon (FcoeLinearSequencerContext *ctx_p,
                                                           string &map_name, UI64 &ifindex)
    {
        FcoeLocalFcoeportAttrMessage    *msg_p = dynamic_cast<FcoeLocalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());
        NsmFcoeportAttrMessage          *dmn_msg_p = NULL;
        FcoeMapManagedObject            *map_mo_p = NULL;
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        CeeMapManagedObject             *cee_map_mo_p = NULL;
        vector<ObjectId>                fab_map_vec;
        ObjectId                        fab_map_object_id;
        UI32                            vlan_id = 0;
        UI32                            i = 0;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        string                          if_id;
        vector<UI64>                    ifindex_vec;
        bool                            send_to_nsm = true;
        PhyIntfLocalManagedObject       *if_mo_p = NULL;


        map_mo_p = NsmFcoeLocalWorker::FcoeMapGetMoByName(map_name);
        if (map_mo_p == NULL) {
            return (FCOE_PORT_ERR_MAP_NOT_FOUND);
        }
        
        dmn_msg_p = new NsmFcoeportAttrMessage();
        if (dmn_msg_p == NULL) {
            FCOE_PLUGIN_DELETE(map_mo_p);
            return (FCOE_PORT_ERR_FAILED_TO_ALLOC_CLIENT_MESSAGE);
        }
            
        // Set the Oper_bmp in the Daemon Message
        dmn_msg_p->setOper_bmp(msg_p->getOper_bmp());
        
		if_id = msg_p->getIfId();

        switch (msg_p->getOper_bmp()){
        case FCOEPORT_ATTR_LAG_CREATE:
        case FCOEPORT_ATTR_LAG_REMOVE:
			dmn_msg_p->setPhyType(IF_TYPE_PO);
			break;
		case FCOEPORT_ATTR_CREATE:
		case FCOEPORT_ATTR_REMOVE:
        	if_mo_p = NsmFcoeLocalWorker::GetPhyIntfMoById(if_id);
        	if (if_mo_p == NULL) {
        		FCOE_PLUG_DBG_S(string("PHY Update ERROR"));
            	return (FCOE_PORT_ERR_INTF_MO_NOT_FOUND);
        	}
			dmn_msg_p->setPhyType(if_mo_p->getPhyType());
			break;
		default:
			break;
		}
		 
        FCOE_PLUG_DBG_S(string("FOUND FCOE-Map MO for Name=") + map_mo_p->getMap_name());
        // Add FCOE-Map name to the message
        dmn_msg_p->setMap_name(map_mo_p->getMap_name());
        
        cee_map_mo_p = dynamic_cast<CeeMapManagedObject *> (queryManagedObject(map_mo_p->getCee_map()));
        if (cee_map_mo_p == NULL) {
            FCOE_PLUGIN_DELETE(map_mo_p);
            FCOE_PLUGIN_DELETE(dmn_msg_p);
            FCOE_PLUGIN_DELETE(if_mo_p);
            return (FCOE_PORT_ERR_CEE_MAP_NOT_FOUND);
        }
           
        FCOE_PLUG_DBG_S(string("FOUND CEE-Map MO for Name=") + cee_map_mo_p->getCeeMapName());
        // Add CEE-Map name to the message
        dmn_msg_p->setCeeMapName(cee_map_mo_p->getCeeMapName());

        //Set the IF ID
        FCOE_PLUG_DBG_S(string("Setting IF-ID=")+msg_p->getIfId());
        dmn_msg_p->setIfId(msg_p->getIfId());
        FCOE_PLUG_DBG_S(string("Setting IF-Index=")+ifindex);
        dmn_msg_p->setIfindex(ifindex);

        fab_map_vec = map_mo_p->getFabric_map_list();
        FCOE_PLUG_DBG_S(string("FOUND FAB-MAP Vector of Size=") + fab_map_vec.size());
        for (i = 0; i < fab_map_vec.size(); i++) {
            fab_map_object_id = fab_map_vec.at(i);

            fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *>(queryManagedObject(fab_map_object_id));
            if (fab_map_mo_p == NULL) {
                FCOE_PLUGIN_DELETE(map_mo_p);
                FCOE_PLUGIN_DELETE(dmn_msg_p);
                FCOE_PLUGIN_DELETE(cee_map_mo_p);
            	FCOE_PLUGIN_DELETE(if_mo_p);
                return (FCOE_PORT_ERR_FAB_MAP_NOT_FOUND);
            }
               
            FCOE_PLUG_DBG_S(string("FOUND FCOE-Fab Map for VLAN=") + fab_map_mo_p->getVlan());
            vlan_id = fab_map_mo_p->getVlan();
            
            //Add FCOE Vlans to the message
            dmn_msg_p->addFcoeVlan(vlan_id);

            delete fab_map_mo_p;
        }

        // Send to FCOE Daemon when
        // 1. fsb mode is disabled
        // 2. fsb mode is enabled and interface is not po 

        if (NsmUtils::isFsbEnabled() == TRUE) {
			PoIntfManagedObject *po_if_mo_p = NULL;
			if_id = msg_p->getIfId();
			po_if_mo_p = NsmFcoeLocalWorker::GetPoIntfMoById(if_id);
			if (po_if_mo_p == NULL) {
            	status = FcoeLocalSendEthConfigMessageToDaemon(dmn_msg_p);
			}
			delete po_if_mo_p;
        } else {
            status = FcoeLocalSendEthConfigMessageToDaemon(dmn_msg_p);
        }
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Failed to process EthConfigMessage in FCOE"));
            FCOE_PLUGIN_DELETE(map_mo_p);
            FCOE_PLUGIN_DELETE(dmn_msg_p);
            FCOE_PLUGIN_DELETE(cee_map_mo_p);
            FCOE_PLUGIN_DELETE(if_mo_p);

            return (status);
        }
        
        // For LAG configuration, we will send it to NSM, only if it has
        // members. Otherwise, the config will pushed after the first
        // member gets added. This is done to keep it consistent with
        // other protocol configs over LAG.
        if ((dmn_msg_p->getOper_bmp() == FCOEPORT_ATTR_LAG_CREATE) ||
            (dmn_msg_p->getOper_bmp() == FCOEPORT_ATTR_LAG_REMOVE)) {
            ObjectId PoOid;
            UI32 loc_id;
            NsmUtils::getPoIntfObjectIdByPoIfName(dmn_msg_p->getIfId(), PoOid, false);
            loc_id = FrameworkToolKit::getThisLocationId ();
            if ((NsmUtils::isPoIntfHasMemberByLocation(PoOid, loc_id) != WAVE_MESSAGE_SUCCESS)) {
                send_to_nsm = false;
            }
        }
        
        if (send_to_nsm) {
            // Send to NSM Daemon
            status = sendSynchronouslyToWaveClient ("nsm",dmn_msg_p);
            if (status != WAVE_MESSAGE_SUCCESS) {
                FCOE_PLUG_DBG_ERR_S(string("Message Status = ") + status);
            }

            if (dmn_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS) {
                FCOE_PLUG_DBG_ERR_S(string("Client Status = ") + dmn_msg_p->getClientStatus());
                status = dmn_msg_p->getClientStatus();
            }

            if (dmn_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                FCOE_PLUG_DBG_ERR_S(string("Completion Status = ") + dmn_msg_p->getCompletionStatus());
                status = dmn_msg_p->getCompletionStatus();
            }

            if (status != WAVE_MESSAGE_SUCCESS) {
                // Send a Rollback message to FCOE Daemon
                ResourceId rbkStatus = FcoeLocalSendEthConfigMessageToDaemon(dmn_msg_p, true);
                if (rbkStatus != WAVE_MESSAGE_SUCCESS) {
                    FCOE_PLUG_DBG_ERR_S(string("Failed to process EthConfigMessage in FCOE"));
                }
            }
        }
       
        FCOE_PLUG_DBG_S(string("Final Status") + status);
        FCOE_PLUGIN_DELETE(map_mo_p);
        FCOE_PLUGIN_DELETE(dmn_msg_p);
        FCOE_PLUGIN_DELETE(cee_map_mo_p);
        FCOE_PLUGIN_DELETE(if_mo_p);

        return (status);
    }

    FcoeMapManagedObject*
    NsmFcoeLocalWorker::FcoeMapGetMoByName (string &map_name,
                                            FcoeLinearSequencerContext *ctx_p)
    {
        FcoeMapManagedObject *map_mo_p;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&map_name, "map_name"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
            if (ctx_p) {
                ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }

        map_mo_p = dynamic_cast<FcoeMapManagedObject *> ((*pResults)[0]);

        pResults->clear ();
        delete pResults;

        return map_mo_p;
    }

    ObjectId
    NsmFcoeLocalWorker::FcoeMapGetObjectIdByName (string &map_name,
                                                  FcoeLinearSequencerContext *ctx_p)
    {
        FcoeMapManagedObject    *map_mo_p = NsmFcoeLocalWorker::FcoeMapGetMoByName(map_name);
        ObjectId                obj_id;

        if (map_mo_p == NULL) {
            return ObjectId::NullObjectId;
        } else {
            obj_id = map_mo_p->getObjectId();
        }
        if (ctx_p) {
            ctx_p->addMOPointerToReleaseVector(map_mo_p);
        } else {
            FCOE_PLUGIN_DELETE(map_mo_p);
        }

        return obj_id;
    }

    FcoeportAttrGlobalManagedObject*
    NsmFcoeLocalWorker::FcoeportGetChildMoByParentLagMo(PoIntfManagedObject *if_mo_p,
                                                        FcoeLinearSequencerContext *ctx_p)
    {
        WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> child;

        child = if_mo_p->getFcoeport();

        return child.operator->();
    }

    FcoeportAttrLocalManagedObject*
    NsmFcoeLocalWorker::FcoeportGetChildMoByParentMo(PhyIntfLocalManagedObject *if_mo_p,
                                                     FcoeLinearSequencerContext *ctx_p)
    {
        WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> child;

        child = if_mo_p->getFcoeport();

        return child.operator->();
    }

    PhyIntfLocalManagedObject*
    NsmFcoeLocalWorker::GetPhyIntfMoById (string &if_name,
                                          FcoeLinearSequencerContext *ctx_p)
    {
        PhyIntfLocalManagedObject *if_mo_p;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&if_name,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
            if (ctx_p) {
                ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }

        if_mo_p = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);

        pResults->clear ();
        delete pResults;
        
        return if_mo_p;
    }

    PoIntfManagedObject*
    NsmFcoeLocalWorker::GetPoIntfMoById (string &if_name,
                                         FcoeLinearSequencerContext *ctx_p)
    {
        PoIntfManagedObject *if_mo_p;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&if_name,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
            if (ctx_p) {
                ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }

        if_mo_p = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);

        pResults->clear ();
        delete pResults;
        
        return if_mo_p;
    }

    void
    NsmFcoeLocalWorker::FcoeLocalFcoeportAttrValidate (FcoeLinearSequencerContext *ctx_p)
	{
		FcoeLocalFcoeportAttrMessage *msg_p = dynamic_cast<FcoeLocalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());
		string if_id;
		string fcoe_map_name;
		WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> child;
		ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
		UI32    oper;
		// Check for the FCOE license for the local switch.
		FCOE_PLUGIN_LICENSE_CHECK_FOR_SEQUENCER(ctx_p);
		trace (TRACE_LEVEL_INFO, "NsmFcoeLocalWorker::FcoeLocalFcoeportAttrValidate validation start ");

		oper = msg_p->getOper_bmp();
		if (oper == FCOEPORT_ATTR_CREATE ||
				oper == FCOEPORT_ATTR_REMOVE) {
			PhyIntfLocalManagedObject *if_mo_p = NULL;
			if_id = msg_p->getIfId();
			if_mo_p = NsmFcoeLocalWorker::GetPhyIntfMoById(if_id);

			if (if_mo_p == NULL) {
				ctx_p->executeNextStep(FCOE_PORT_ERR_INTF_MO_NOT_FOUND);
				return;
			}

			if(false == if_mo_p->getActive())
			{
				ctx_p->executeNextStep(WRC_NSM_INTERFACE_NOT_IN_SERVICE);
				FCOE_PLUGIN_DELETE(if_mo_p);
				return;
			}

			if (!NsmUtils::checkPhyPortFcoeQosCompatibility(if_mo_p, status)) {
				ctx_p->executeNextStep(status);
				FCOE_PLUGIN_DELETE(if_mo_p);
				return;
			}
			// Not allow if port-profile-port configured on interface
			if ((msg_p->get_prov_context() == FCOE_PROVISIONING_VIA_FCOEPORT_DEFAULT)) {
				if (NsmUtils::isProfileSpeficFcoeApplied(if_mo_p))  {
					ctx_p->executeNextStep(FCOE_PORT_ERR_PORT_PROFILE_PORT);
				}
				else {
					ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
				}
			} else {
				ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
			}
			FCOE_PLUGIN_DELETE(if_mo_p);
			return;
		}

		if (oper == FCOEPORT_ATTR_LAG_CREATE) {
			PoIntfManagedObject *if_mo_p = NULL;
			if_id = msg_p->getIfId();
			if_mo_p = NsmFcoeLocalWorker::GetPoIntfMoById(if_id);

			if (if_mo_p == NULL) {
				ctx_p->executeNextStep(FCOE_PORT_ERR_INTF_MO_NOT_FOUND);
				return;
			}
			// Not allow FCoE configuration if port-profile-port configured on LAG
			if ((msg_p->get_prov_context() == FCOE_PROVISIONING_VIA_FCOEPORT_DEFAULT)) {
				if (NsmUtils::isIntfPortProfiled(IF_TYPE_PO, msg_p->getIfId())) {
					ctx_p->executeNextStep(FCOE_PORT_ERR_LAG_PROFILE_PORT);
					FCOE_PLUGIN_DELETE(if_mo_p);
					return;
				}
			}
			if (!NsmUtils::checkLagFcoeQosCompatibility(if_mo_p, status)) {
				ctx_p->executeNextStep(status);
			} else {
				ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
			}
			FCOE_PLUGIN_DELETE(if_mo_p);
			return;

		}
		ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
		trace (TRACE_LEVEL_INFO, "NsmFcoeLocalWorker::FcoeLocalFcoeportAttrValidate validation End ");
		return;
	}


    void
    NsmFcoeLocalWorker::FcoeLocalFcoeportAttrHandleMoUpdate (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeportAttrMessage *msg_p = dynamic_cast<FcoeLocalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());
        PhyIntfLocalManagedObject *if_mo_p = NULL;
        NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        string if_id;
        FcoeportAttrLocalManagedObject *fcoeport_mo_p = NULL;
        ObjectId fcoe_map_object_id;
        string fcoe_map_name;
        WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> child;

		if(msg_p->get_prov_context() == FCOE_PROVISIONING_VIA_PORT_PROFILE_PORT) {
        	ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        	return;
		}

        if_id = msg_p->getIfId();

        switch (msg_p->getOper_bmp()) {
            case FCOEPORT_ATTR_CREATE:
                if_mo_p = NsmFcoeLocalWorker::GetPhyIntfMoById(if_id);

                if (if_mo_p->getPortrole()) {
                    break;
                }
               
                FCOE_PLUG_DBG_S(string("Recvd CREATE request for FCOE Port for Interface=")+msg_p->getIfId());
                fcoe_map_name = msg_p->getMap_name();

                fcoe_map_object_id = NsmFcoeLocalWorker::FcoeMapGetObjectIdByName(fcoe_map_name, ctx_p); 
                
                fcoeport_mo_p = new FcoeportAttrLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()),
                                                                   fcoe_map_object_id);
                if_mo_p->setFcoeport(WaveManagedObjectPointer<FcoeportAttrLocalManagedObject>(fcoeport_mo_p));
                // delete L3 configuration before apply fcoeport defect #TR000459293 
				pL3ReadyMo = NsmL3Util::PhyIfNameGetL3ReadyMO(if_id);

                if (pL3ReadyMo) {
                    if (!(pL3ReadyMo->getCanDeleteL3ReadyMO())) {
                        ctx_p->executeNextStep(WRC_NSM_ERR_REMOVE_L3_CONFIG);
                        return;
                    }
			        if_mo_p->setL3Ready(NULL);
            	    trace (TRACE_LEVEL_INFO, "NsmFcoeLocalWorker::FcoeLocalFcoeportAttrHandleMoUpdate : Now L3 is disabled for interface :" + if_id);
                    ctx_p->addManagedObjectForGarbageCollection(pL3ReadyMo);
                }
                           
                updateWaveManagedObject(if_mo_p);
                break;

            case FCOEPORT_ATTR_REMOVE:
                if_mo_p = NsmFcoeLocalWorker::GetPhyIntfMoById(if_id);

                FCOE_PLUG_DBG_S(string("Recvd REMOVE request for FCOE Port for Interface=")+msg_p->getIfId());
                FCOE_PLUG_DBG_S(string("Found TE MO Name=")+if_mo_p->getId());
                
                fcoeport_mo_p = NsmFcoeLocalWorker::FcoeportGetChildMoByParentMo(if_mo_p, ctx_p);
                FCOE_PLUG_ASSERT_PTR(fcoeport_mo_p);
                
                if_mo_p->setFcoeport(WaveManagedObjectPointer<FcoeportAttrLocalManagedObject>(NULL));
                updateWaveManagedObject(if_mo_p);

                break;
        }
        
        if (if_mo_p) {
            ctx_p->addMOPointerToReleaseVector(if_mo_p);
        }

        ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }
                

    void  NsmFcoeLocalWorker::FcoeLocalFcoeportAttrMessageHandler(FcoeLocalFcoeportAttrMessage *pFcoeLocalFcoeportAttrMessage)
    {
        // Validations and Queries should go here
        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeLocalWorker::FcoeLocalFcoeportAttrValidate),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeLocalWorker::FcoeLocalFcoeportProcessBackend),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeLocalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeLocalWorker::FcoeLocalFcoeportAttrHandleMoUpdate),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeLocalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeportAttrMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    ResourceId
    NsmFcoeLocalWorker::pushFcoeProvisioningConfig ()
    {
        NSM_PLUG_INFO_S(string("Entered..."));

        // Check for the FCOE license first
        FCOE_PLUGIN_LICENSE_CHECK();

		UI32    loc_id = 0;
		loc_id = FrameworkToolKit::getThisLocationId();
        
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());

		if (loc_id != 0) {
			// Add location id filter to the context 
			syncQueryCtxt.setLocationIdFilter(loc_id);
		}

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        
        for (UI32 i = 0; i < pResults->size(); i++) {
            pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);
            NSM_PLUG_INFO_S(string("Replaying FCOE provisioning for Phy IF=") +
                            pPhyIntfLocalManagedObject->getId());

            (NsmLocalObjectManager::getInstance())->postBootFcoeProvisioningConfigForInterface(pPhyIntfLocalManagedObject);
        }
        
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        PoIntfManagedObject *pPoIntfManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxtLag(PoIntfManagedObject::getClassName());
        vector<WaveManagedObject *> *pResultsLag = querySynchronously (&syncQueryCtxtLag);
        
        for (UI32 i = 0; i < pResultsLag->size(); i++) {
            pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResultsLag)[i]);
            UI32 po_id = strtoul(pPoIntfManagedObject->getIfName().c_str(), NULL, 0);

            if (NsmUtils::isFcoeConfiguredForLagPort(po_id) == false) {
                NSM_PLUG_INFO_S(string("Fcoe is not configured for Po ") +
                                       pPoIntfManagedObject->getIfName());
                continue;
            }

            NSM_PLUG_INFO_S(string("Replaying FCOE provisioning for PO=") +
                            pPoIntfManagedObject->getIfName());
                                                                
            (NsmLocalObjectManager::getInstance())->postBootFcoeProvisioningConfigForPoInterface(pPoIntfManagedObject);
        }
        
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsLag);

        return WAVE_MESSAGE_SUCCESS;
    }

    void NsmFcoeLocalWorker::configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
    {
        
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        status = NsmFcoeLocalWorker::pushFcoeProvisioningConfig();
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Failed to push FcoeProvisioningConfig Error=") + status);
        }
        
        pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForConfigReplayEnd->callback ();
        return;
    }

    void NsmFcoeLocalWorker::fileReplayEnd (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd) 
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        status = NsmFcoeLocalWorker::pushFcoeProvisioningConfig();
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Failed to push FcoeProvisioningConfig Error=") + status);
        }

        pWaveAsynchronousContextForFileReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForFileReplayEnd->callback ();
        return;
    }
	void NsmFcoeLocalWorker::getPoMemberIfindexListForLocalNode(const string  &PoName, vector<UI64> &ifx_list)
	{

    	ObjectId PoOid;
		SI32 MappedId = -1;
		LocationId locationId = 0;
       	UI64 ifindex = 0; 
    	vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
    	NsmUtils::getPoIntfObjectIdByPoIfName(PoName, PoOid, false);
    	NsmUtils::GetPhyMoByPoOid(PoOid,  vpPhyIntfLocalManagedObject);
    	for(unsigned int i = 0;i< vpPhyIntfLocalManagedObject.size(); i++)
    	{   
        	ifindex = vpPhyIntfLocalManagedObject[i]->getIfIndex();
			MappedId = NsmUtils::getMappedIdFromIfName(vpPhyIntfLocalManagedObject[i]->getIfName());
			locationId = DcmToolKit::getLocationIdFromMappedId(MappedId);
			if(locationId == FrameworkToolKit::getThisLocationId ()) {
        		ifx_list.push_back(ifindex);
				
        		NSM_PLUG_INFO_S(string("Retrieved Ifindex for Mbr=") + vpPhyIntfLocalManagedObject[i]->getIfName() +
                        string(" Ifindex=") + ifindex);
			}
        	delete vpPhyIntfLocalManagedObject[i];
    	}
	}
}
