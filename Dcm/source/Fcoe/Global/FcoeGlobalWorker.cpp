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

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForUpgradePhase.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalWorker.h"

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
// FCoE enodes MO
#include "Fcoe/Local/FcoeEnodeLocalManagedObject.h"

#include "Fcoe/Global/FcoeGlobalFcoeShowMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeShowMessage.h"

#include "Fcoe/Global/FcoeGlobalFcoeInternalMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeInternalMessage.h"

#include "Fcoe/Global/FcoeGlobalTypes.h"

#include "Fcoe/Common/FcoePluginDebug.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Fcoe/Common/FcoeCommonWorker.h"
#include "Fcoe/Common/FcoePluginUtils.h"

#include "Qos/Global/CeeMapManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/FcoeWorker/FcoeportAttrLocalManagedObject.h"

#include "Nsm/Global/NsmGlobalFcoeVlanMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/DceClusterUtils.h"

#include "ClientInterface/Nsm/NsmUtils.h"
#include "hasm/hasm.h"

namespace DcmNs
{

    FcoeGlobalWorker::FcoeGlobalWorker ( FcoeGlobalObjectManager *pFcoeGlobalObjectManager)
        : FcoeCommonWorker  (pFcoeGlobalObjectManager)
    {
        FcoeFabricMapManagedObject    FcoeFabricMapManagedObject    (pFcoeGlobalObjectManager);
        FcoeMapManagedObject    FcoeMapManagedObject    (pFcoeGlobalObjectManager);
        FcoeFipManagedObject    FcoeFipManagedObject    (pFcoeGlobalObjectManager);
		FcoeGlobalFsbManagedObject      FcoeGlobalFsbManagedObject      (pFcoeGlobalObjectManager);
    
        FcoeFabricMapManagedObject.setupOrm ();
        addManagedClass (FcoeFabricMapManagedObject::getClassName (), this);
        
        FcoeMapManagedObject.setupOrm ();
        addManagedClass (FcoeMapManagedObject::getClassName (), this);
        
        FcoeFipManagedObject.setupOrm ();
        addManagedClass (FcoeFipManagedObject::getClassName (), this);

		FcoeGlobalFsbManagedObject.setupOrm ();
        addManagedClass (FcoeGlobalFsbManagedObject::getClassName (), this);
        
        // Fabric-Map OP-Maps
        addOperationMap (FCOE_GLOBAL_FCOE_FABRIC_MAP, reinterpret_cast<PrismMessageHandler> (&FcoeGlobalWorker::FcoeGlobalFcoeFabricMapMessageHandler));
       
        // FCOE Map Op-Maps
        addOperationMap (FCOE_GLOBAL_FCOE_MAP, reinterpret_cast<PrismMessageHandler> (&FcoeGlobalWorker::FcoeGlobalFcoeMapMessageHandler));

        // FIP Op-Maps
        addOperationMap (FCOE_GLOBAL_FCOE_FIP, reinterpret_cast<PrismMessageHandler> (&FcoeGlobalWorker::FcoeGlobalFipMessageHandler));

        //Clear Message
        addOperationMap (FCOE_GLOBAL_FCOE_CLEAR, reinterpret_cast<PrismMessageHandler> (&FcoeGlobalWorker::FcoeGlobalFcoeClearMessageHandler));

        // Show Message
        addOperationMap (FCOE_GLOBAL_FCOE_SHOW, reinterpret_cast<PrismMessageHandler> (&FcoeGlobalWorker::FcoeGlobalFcoeShowMessageHandler));

        // Internal Message
        addOperationMap (FCOE_GLOBAL_FCOE_INTERNAL, reinterpret_cast<PrismMessageHandler> (&FcoeGlobalWorker::FcoeGlobalFcoeInternalMessageHandler));

		// FSB
        addOperationMap (FCOE_GLOBAL_FCOE_FSB, reinterpret_cast<PrismMessageHandler> (&FcoeGlobalWorker::FcoeGlobalFcoeFsbMessageHandler));

    }

    FcoeGlobalWorker::~FcoeGlobalWorker ()
    {
    }

    PrismMessage  *FcoeGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FCOE_GLOBAL_FCOE_FABRIC_MAP:
                pPrismMessage = new FcoeGlobalFcoeFabricMapMessage ();
                break;
            case FCOE_GLOBAL_FCOE_MAP :
                pPrismMessage = new FcoeGlobalFcoeMapMessage ();
                break;
            case FCOE_GLOBAL_FCOE_FIP:
                pPrismMessage = new FcoeGlobalFipMessage ();
                break;

            case FCOE_GLOBAL_FCOE_SHOW:
                pPrismMessage = new FcoeGlobalFcoeShowMessage();
                break;

            case FCOE_GLOBAL_FCOE_CLEAR:
                pPrismMessage = new FcoeGlobalFcoeClearMessage();
                break;

            case FCOE_GLOBAL_FCOE_INTERNAL:
                pPrismMessage = new FcoeGlobalFcoeInternalMessage();
                break;

			case FCOE_GLOBAL_FCOE_FSB:
                pPrismMessage = new FcoeLocalFcoeFsbMessage();
                break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FcoeGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FcoeFabricMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FcoeFabricMapManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((FcoeMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FcoeMapManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((FcoeFipManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FcoeFipManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((FcoeGlobalFsbManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FcoeGlobalFsbManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "FcoeGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);

        }
        return (pWaveManagedObject);
    }

    ObjectId
    FcoeGlobalWorker::CeeMapGetObjectIdByName (string &cee_map_name,
                                               FcoeLinearSequencerContext *seq_ctx_p)
    {
        CeeMapManagedObject *cee_map_mo_p;
        ObjectId            obj_id;
        
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&cee_map_name, "ceeMapName"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_ERR("Query returned 0 or >1 results, MO not found");
            obj_id = ObjectId::NullObjectId;
        } else {
            cee_map_mo_p = dynamic_cast<CeeMapManagedObject *> ((*pResults)[0]);
            FCOE_PLUG_ASSERT_PTR(cee_map_mo_p);
            obj_id = cee_map_mo_p->getObjectId();
        }

        if (seq_ctx_p) {
            seq_ctx_p->addMOVectorToReleaseVector(pResults);
        } else {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        return obj_id;
    }

    FcoeMapManagedObject *
    FcoeGlobalWorker::FcoeMapGetMOByName (string &map_name,
                                          FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeMapManagedObject *map_mo_p;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&map_name, "map_name"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_ERR("Query returned 0 or >1 results, MO not found");
            if (seq_ctx_p) {
                seq_ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }

        map_mo_p = dynamic_cast<FcoeMapManagedObject *> ((*pResults)[0]);
        FCOE_PLUG_ASSERT_PTR(map_mo_p);

        pResults->clear ();
        delete pResults;
        
        return map_mo_p;
    }

    ObjectId
    FcoeGlobalWorker::FcoeMapGetObjectIdByName (string &map_name,
                                                FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeMapManagedObject *map_mo_p;
        ObjectId             obj_id;

        map_mo_p = FcoeGlobalWorker::FcoeMapGetMOByName(map_name, seq_ctx_p);

        if (map_mo_p == NULL) {
            return ObjectId::NullObjectId;
        }

        obj_id = map_mo_p->getObjectId();
        if (seq_ctx_p) {
            seq_ctx_p->addMOPointerToReleaseVector(map_mo_p);
        } else {
            delete map_mo_p;
        }

        return obj_id;
    }

    FcoeFabricMapManagedObject *
    FcoeGlobalWorker::FcoeFabricMapGetMOByName (string &fab_map_name,
                                                FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeFabricMapManagedObject *fab_map_mo_p;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFabricMapManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&fab_map_name, "fabric_map_name"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
       
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_ERR("Query returned 0 or >1 results, MO not found");
            if (seq_ctx_p) {
                seq_ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            
            return NULL;
        }

        fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *> ((*pResults)[0]);
        
        pResults->clear ();
        delete pResults;
        
        return fab_map_mo_p;
    }

    ObjectId 
    FcoeGlobalWorker::FcoeFabricMapGetObjectIdByName (string &fab_map_name,
                                                      FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeFabricMapManagedObject  *fab_map_mo_p;
        ObjectId                    obj_id;

        fab_map_mo_p = FcoeGlobalWorker::FcoeFabricMapGetMOByName(fab_map_name, seq_ctx_p);

        if (fab_map_mo_p == NULL) {
            return ObjectId::NullObjectId;
        }

        obj_id = fab_map_mo_p->getObjectId();
        if (seq_ctx_p) {
            seq_ctx_p->addMOPointerToReleaseVector(fab_map_mo_p);
        } else {
            delete fab_map_mo_p;
        }

        return obj_id;
    }

    int FcoeGlobalWorker::isAgModeEnabled(void)
    {
        char    agFname[PATH_MAX];
        int     agMode = 0;

        snprintf(agFname, PATH_MAX-1, "/etc/fabos/ag_platform.0");

        if (access(agFname, F_OK) == 0) {
                agMode = 1;
        }

        return (agMode);
    }


    void
    FcoeGlobalWorker::FcoeGlobalFcoeMapHandleConfig (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FCOE_PLUG_DBG("Entering ... ");


        FcoeGlobalFcoeMapMessage    *msg_p = dynamic_cast<FcoeGlobalFcoeMapMessage *>(seq_ctx_p->getPPrismMessage ());
        FcoeMapManagedObject        *map_mo_p = NULL;
        ObjectId                    fab_map_object_id;
        string                      fab_map_name;
        string                      cee_map_name;
        ObjectId                    object_id;
        string                      map_name;

        map_name = msg_p->getName();
      
        FCOE_PLUG_ASSERT_PTR(msg_p);
        
        switch (msg_p->getOperBmp()) {
            case FCOE_MAP_OPER_ADD_FAB_MAP:
            case FCOE_MAP_OPER_REM_FAB_MAP:
                map_mo_p = FcoeGlobalWorker::FcoeMapGetMOByName(map_name);
                if (map_mo_p == NULL) {
                    seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_NOT_FOUND_IN_DB);
                    return;
                }

                fab_map_name = msg_p->getFabricMapName();
                if (fab_map_name.empty() == true) {
                    seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_NAME_EMPTY);
                    return;
                }
                
                object_id = FcoeGlobalWorker::FcoeFabricMapGetObjectIdByName(fab_map_name, seq_ctx_p);
            
                if (msg_p->getOperBmp() == FCOE_MAP_OPER_ADD_FAB_MAP) {
                    map_mo_p->addFabricMap(object_id);
                } else {
                    map_mo_p->remFabricMap(object_id);
                }
                break;

            case FCOE_MAP_OPER_ADD_CEE_MAP:
            case FCOE_MAP_OPER_REM_CEE_MAP:
                map_mo_p = FcoeGlobalWorker::FcoeMapGetMOByName(map_name);
                if (map_mo_p == NULL) {
                    seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_NOT_FOUND_IN_DB);
                    return;
                }
                
                cee_map_name = msg_p->getCeeMapName();
                if (cee_map_name.empty() == true) {
                    seq_ctx_p->executeNextStep(FCOE_MAP_ERR_CEE_MAP_NAME_EMPTY);
                    return;
                }
                
                object_id = FcoeGlobalWorker::CeeMapGetObjectIdByName(cee_map_name, seq_ctx_p);
                if (msg_p->getOperBmp() == FCOE_MAP_OPER_ADD_CEE_MAP) {
                    map_mo_p->setCee_map(object_id);
                } else {
                    map_mo_p->clearCeeMap();
                }
                break;

            case FCOE_MAP_OPER_CREATE:
                map_mo_p = new FcoeMapManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()));
                if (map_mo_p == NULL) {
                    seq_ctx_p->executeNextStep(FCOE_MAP_ERR_FAILED_TO_CREATE_OBJ);
                    return;
                }
                map_mo_p->setMap_name(msg_p->getName());
                FCOE_PLUG_DBG_S(string("Creating FCOE Map with NullObjectId=") +
                                ObjectId::NullObjectId.toString());
                break;

            default:
                FCOE_PLUG_ASSERT(0);
                break;
        }

        if (msg_p->getOperBmp() != FCOE_MAP_OPER_CREATE) {
            updateWaveManagedObject(map_mo_p);
        }

        seq_ctx_p->addMOPointerToReleaseVector(map_mo_p);

        //create_msg_p->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
       

        return;
    }

    void
    FcoeGlobalWorker::FcoeGlobalTransFailRollbackCompleteCallback (PrismAsynchronousContext *ctx_p)
    {
        FcoeLinearSequencerContext             *seq_ctx_p = NULL;

        seq_ctx_p = reinterpret_cast<FcoeLinearSequencerContext *>(ctx_p->getPCallerContext());
        FCOE_PLUG_ASSERT(seq_ctx_p);

        FcoeGlobalWorker::prismLinearSequencerFailedStep(seq_ctx_p);
    }

    void
    FcoeGlobalWorker::FcoeGlobalSendToClusterCallback (PrismAsynchronousContext *ctx_p)
    {
        FcoeLinearSequencerContext             *seq_ctx_p = NULL;
        WaveSendToClusterContext                *send_ctx_p = NULL;
        PrismMessage                            *gl_msg_p = NULL;
        PrismMessage                            *loc_msg_p = NULL;
        vector<LocationId>                      loc_vector;
        ResourceId                              status = WAVE_MESSAGE_SUCCESS;
        ResourceId                              final_status = WAVE_MESSAGE_SUCCESS;
        UI32                                    i = 0;
        
        seq_ctx_p = reinterpret_cast<FcoeLinearSequencerContext *>(ctx_p->getPCallerContext());
        gl_msg_p = seq_ctx_p->getPPrismMessage ();
        FCOE_PLUG_ASSERT(gl_msg_p);

        send_ctx_p = dynamic_cast<WaveSendToClusterContext *> (ctx_p);
        
        loc_vector = send_ctx_p->getLocationsToSendToForPhase1();
        for (i = 0; i < loc_vector.size(); i++) {
            
            status = ClusterGetPhase1StatusFromContextForLocation(send_ctx_p, loc_vector[i]);
            
            FCOE_PLUG_DBG_S(string("Apply Status of Location[") + loc_vector[i] +
                            string("] = ") + FrameworkToolKit::localize(status));

            loc_msg_p = send_ctx_p->getResultingMessageForPhase1(loc_vector[i]);
            FCOE_PLUG_ASSERT(loc_msg_p);
        
            loc_msg_p->transferAllBuffers(gl_msg_p);
        }
        
        /* Dump the Rollback Status */
        loc_vector = send_ctx_p->getLocationsToSendToForPhase2();
        for (i = 0; i < loc_vector.size(); i++) {
            status = ClusterGetPhase2StatusFromContextForLocation(send_ctx_p, loc_vector[i]);
            FCOE_PLUG_DBG_S(string("Rollback Status of Location[") + loc_vector[i] +
                            string("] = ") + FrameworkToolKit::localize(status));
        }

        final_status = ClusterGetStatusFromContext(send_ctx_p);

        FCOE_PLUG_DBG_S(string("Final status of Cluster Operation=") + 
                        FrameworkToolKit::localize(final_status));

        gl_msg_p->setCompletionStatus(final_status);
        
        seq_ctx_p->executeNextStep(final_status);
            
        // Delete the APPLY message
        loc_msg_p = send_ctx_p->getPPrismMessageForPhase1();
        if (loc_msg_p) {
            delete loc_msg_p;
        }

        // Delete the ROLLBACK message
        loc_msg_p = send_ctx_p->getPPrismMessageForPhase2();
        if (loc_msg_p) {
            delete loc_msg_p;
        }

        delete send_ctx_p;
        
        return;
    }

    FcoeLocalFcoeMapMessage *
    FcoeGlobalWorker::FcoeGlobalFcoeMapGetRollbackMessage (PrismMessage *prism_msg_p)
    {
        UI32                        oper_bmp;
        string                      fabric_map_name("");
        string                      cee_map_name("");
        string                      map_name("");
        FcoeGlobalFcoeMapMessage    *msg_p = NULL;

        FcoeLocalFcoeMapMessage *rbk_msg_p = NULL;

        msg_p = dynamic_cast <FcoeGlobalFcoeMapMessage *>(prism_msg_p);

        map_name = msg_p->getName();
        switch (msg_p->getOperBmp()) {
            case FCOE_MAP_OPER_ADD_FAB_MAP:
                oper_bmp = FCOE_MAP_OPER_REM_FAB_MAP;
                fabric_map_name = msg_p->getFabricMapName(); 
                break;

            case FCOE_MAP_OPER_REM_FAB_MAP:
                oper_bmp = FCOE_MAP_OPER_ADD_FAB_MAP;
                fabric_map_name = msg_p->getFabricMapName();
                break;

            case FCOE_MAP_OPER_ADD_CEE_MAP:
                oper_bmp = FCOE_MAP_OPER_REM_CEE_MAP;
                cee_map_name = msg_p->getCeeMapName();
                break;

            case FCOE_MAP_OPER_REM_CEE_MAP:
                oper_bmp = FCOE_MAP_OPER_ADD_CEE_MAP;
                cee_map_name = msg_p->getCeeMapName();
                break;

            case FCOE_MAP_OPER_CREATE:
                oper_bmp = FCOE_MAP_OPER_REMOVE;
                break;

            case FCOE_MAP_OPER_REMOVE:
                oper_bmp = FCOE_MAP_OPER_CREATE;
                break;
        }

        FCOE_PLUG_DBG_S(string("Creating FCOE_MAP Rollback message Oper=") + oper_bmp +
                        string(" Map-Name=") + map_name +
                        string(" Fabric-Map-Name=") + fabric_map_name +
                        string(" Cee-Map-Name=") + cee_map_name);

        rbk_msg_p = new FcoeLocalFcoeMapMessage(oper_bmp,
                                                map_name,
                                                fabric_map_name,
                                                cee_map_name);

        return rbk_msg_p;
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeMapSendToLocal(FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalFcoeMapMessage    *msg_p = dynamic_cast<FcoeGlobalFcoeMapMessage *>(seq_ctx_p->getPPrismMessage ());
        FcoeLocalFcoeMapMessage     *loc_msg_p;
        WaveSendToClusterContext    *send_ctx_p;
        //FcoeLocalFcoeMapMessage     *rbk_msg_p;

        FCOE_PLUG_ASSERT_PTR(msg_p);

        FCOE_PLUG_DBG("Entering ... ");
        
        loc_msg_p = new FcoeLocalFcoeMapMessage(msg_p->getOperBmp(),
                                                msg_p->getName(),
                                                msg_p->getFabricMapName(),
                                                msg_p->getCeeMapName());
        if (loc_msg_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_LOCAL_MESSAGE);
            return;
        }
        

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalSendToClusterCallback),
                                                  seq_ctx_p);
        FCOE_PLUG_ASSERT_PTR(send_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
 
        /*
        rbk_msg_p =  FcoeGlobalWorker::FcoeGlobalFcoeMapGetRollbackMessage(msg_p);
        if (rbk_msg_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_ROLLBACK_MESSAGE);
            return;
        }
        send_ctx_p->setPPrismMessageForPhase2(rbk_msg_p);
        */

        send_ctx_p->setPartialSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);

        return;
    }


    void  FcoeGlobalWorker::FcoeGlobalFcoeMapMessageHandler( FcoeGlobalFcoeMapMessage *pFcoeGlobalFcoeMapMessage)
    {
        // Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {

            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeMapSendToLocal),
           
            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeMapHandleConfig),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeGlobalFcoeMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    ResourceId
    FcoeGlobalWorker::FcoeFabricMapVlanUpdateSanityCheck()
    {
        ResourceId  status = WAVE_MESSAGE_SUCCESS;
        UI32        loc_id = 0;
		vector<UI32> mappedIds;
		ResourceId  locationStatus = WAVE_MESSAGE_SUCCESS;

		DcmToolKit::getDomainIdsForFullyConnectedLocations (mappedIds, true);

		for (UI32 i = 0; i < mappedIds.size(); i++) {
			loc_id = DcmToolKit::getLocationIdFromMappedId(mappedIds[i]);

        	if (NsmUtils::isFcoeConfiguredForEthPort(loc_id)) {
            	status = FCOE_FAB_MAP_ERR_FCOE_VLAN_IN_USE;
        	}

			locationStatus = DcmToolKit::getSpecificStatusFromMappedId (mappedIds[i]);

			// Ignoring fcoe provisioned port if it belongs to disconnected node
			if (status == FCOE_FAB_MAP_ERR_FCOE_VLAN_IN_USE &&
				locationStatus == VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER) {
				status = WAVE_MESSAGE_SUCCESS;
			}
        }
        return status;
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeFabricMapSendToNsmGlobal (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalFcoeFabricMapMessage  *msg_p = NULL;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        NsmGlobalFcoeVlanMessage        *nsm_msg_p = NULL;
        string                          fab_map_name;
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        UI32                            cmd_code = 0;
        
        FCOE_PLUG_DBG("Entering ... Sending FCOE VLAN request to NSM Daemon ");
        msg_p = dynamic_cast<FcoeGlobalFcoeFabricMapMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);

        if (msg_p->getOperBmp() != FCOE_FAB_MAP_OPER_UPD_VLAN) {
            FCOE_PLUG_DBG("Not a FCOE VLAN request, return SUCCESS");
            seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        status = FcoeGlobalWorker::FcoeFabricMapVlanUpdateSanityCheck();
        if (status != WAVE_MESSAGE_SUCCESS) {
            seq_ctx_p->executeNextStep(status);
            return;
        }
        
        fab_map_name = msg_p->getName();
        if (fab_map_name.empty() == true) {
            seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_NAME_EMPTY);
            return;
        }

        fab_map_mo_p = FcoeGlobalWorker::FcoeFabricMapGetMOByName(fab_map_name);
        if (fab_map_mo_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_NOT_FOUND_IN_DB);
            return;
        }
        
        
        FCOE_PLUG_DBG_S(string("FOUND FAB-MAP with Name=")+fab_map_mo_p->getName());
        FCOE_PLUG_DBG_S(string("Upd Vlan Old-Vlan=") + fab_map_mo_p->getVlan() +
                        string(" New-Vlan=") + msg_p->getVlan());
       if(NsmUtils::getVlanClassificationMOCount(0, msg_p->getVlan())) {
           
            seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_VLAN_CTAG_FOUND);
            delete fab_map_mo_p;
            return;
          
       } 
        switch(msg_p->getOperBmp()) {
            case FCOE_FAB_MAP_OPER_UPD_VLAN:
                cmd_code = NSM_FCOE_UPDATE_VLAN;
                break;

            default:
                FCOE_PLUG_ASSERT(0);
                break;
        }

        nsm_msg_p = new NsmGlobalFcoeVlanMessage(cmd_code,
                                           fab_map_mo_p->getVlan(),
                                           msg_p->getVlan(),
                                           "FCOE VLAN",
                                           0,
                                           "FCOE VLAN");
        
        status = sendSynchronously(nsm_msg_p);

        msg_p->setCompletionStatus(nsm_msg_p->getCompletionStatus());
        if (status == WAVE_MESSAGE_SUCCESS) {
            status = msg_p->getCompletionStatus();
            if (status != WAVE_MESSAGE_SUCCESS) {
			    if (status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
				    status = FCOE_PORT_ERR_FCOE_PROV_ON_PORT_VLAN_CHANGE; // Changing the genric error to proper error.
				}
			}
            FCOE_PLUG_DBG("Succeeded in sending message to NSM");
        } else {
            FCOE_PLUG_DBG_ERR("Failed to send message to NSM");
        }

        delete nsm_msg_p;
        delete fab_map_mo_p;
        
        seq_ctx_p->executeNextStep(status);

        return;
    }

    FcoeLocalFcoeFabricMapMessage *
    FcoeGlobalWorker::FcoeGlobalFcoeFabricMapGetRollbackMessage (PrismMessage *prism_msg_p)
    {
        string                          fab_map_name("");
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        UI32                            oper_bmp = 0;
        FcoeLocalFcoeFabricMapMessage   *rbk_msg_p = NULL;
        FcoeGlobalFcoeFabricMapMessage  *msg_p = NULL;

        msg_p = dynamic_cast <FcoeGlobalFcoeFabricMapMessage *> (prism_msg_p);

        fab_map_name = msg_p->getName();
        fab_map_mo_p = FcoeFabricMapGetMOByName(fab_map_name);
        if (fab_map_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to find FAB-MAP MO");
            return NULL;
        }

        switch (msg_p->getOperBmp()) {
            case FCOE_FAB_MAP_OPER_UPD_FCMAP:
            case FCOE_FAB_MAP_OPER_UPD_VLAN:
            case FCOE_FAB_MAP_OPER_UPD_PRIORITY:
            case FCOE_FAB_MAP_OPER_UPD_VFID:
            case FCOE_FAB_MAP_OPER_UPD_TIMEOUT:
            case FCOE_FAB_MAP_OPER_UPD_FKA_INTVL:
            case FCOE_FAB_MAP_OPER_UPD_MTU:
            case FCOE_FAB_MAP_OPER_UPD_MAX_ENODES:
                oper_bmp = msg_p->getOperBmp();
                break;

            case FCOE_FAB_MAP_OPER_CREATE:
                oper_bmp = FCOE_FAB_MAP_OPER_REMOVE;
                break;

            case FCOE_FAB_MAP_OPER_REMOVE:
                oper_bmp = FCOE_FAB_MAP_OPER_CREATE;
                break;

            default:
                break;
        }

        if (oper_bmp == 0) {
            delete fab_map_mo_p;
            return NULL;
        }
        
        rbk_msg_p = new FcoeLocalFcoeFabricMapMessage(oper_bmp,
                                                      fab_map_mo_p->getName(),
                                                      fab_map_mo_p->getVlan(),
                                                      fab_map_mo_p->getVfid(),
                                                      fab_map_mo_p->getPriority(),
                                                      fab_map_mo_p->getFcmap(),
                                                      fab_map_mo_p->getFka_intvl(),
                                                      fab_map_mo_p->getTimeout(),
                                                      fab_map_mo_p->getMtu(),
                                                      fab_map_mo_p->getMaxEnodes(),
                                                      fab_map_mo_p->getEnodesCfgMode());
        if (rbk_msg_p == NULL) {
            delete fab_map_mo_p;
            return NULL;
        }
        
        FCOE_PLUG_DBG_S(string("NEW Rollback FAB-MAP Msg: Oper=") + rbk_msg_p->getOper_bmp() +
                        string(" Fab-Map-name=") + rbk_msg_p->getFabric_map_name() +
                        string(" Vlan=") + rbk_msg_p->getVlan() +
                        string(" VFID=") + rbk_msg_p->getVfid() +
                        string(" Priority=") + rbk_msg_p->getPriority() +
                        string(" FCMAP=") + rbk_msg_p->getFcmap() +
                        string(" FKA=") + rbk_msg_p->getFka_intvl() +
                        string(" Timeout=") + rbk_msg_p->getTimeout() +
                        string(" MTU=") + rbk_msg_p->getMtu() +
                        string(" MAX-Endes=") + rbk_msg_p->getMaxEnodes());

        delete fab_map_mo_p;
       
        return rbk_msg_p;
    }

    bool
    FcoeGlobalWorker::FcoeFabricMapNeedToCreateRollbackMessage(const UI32 &oper)
    {
        if (oper == FCOE_FAB_MAP_OPER_GET_FCF_MAC_ADDR) {
            return false;
        }

        return true;
    }

    ResourceId
    FcoeGlobalWorker::FcoeGlobalFabricMapValidatePriorityChange(const uint32_t priority,
                                                                FcoeLinearSequencerContext *seq_ctx_p)
    {
        SI32    remap_priority;
        SI32    lossless_priority;
        
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
        
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL == pResults) {
            FCOE_PLUG_DBG_ERR("Query returned NULL results");
            return WAVE_MESSAGE_SUCCESS;
        }

        CeeMapManagedObject *cee_map_mo_p = NULL;

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_ERR("Query returned 0 or >1 results, MO not found");
            if (seq_ctx_p) {
                seq_ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return WAVE_MESSAGE_SUCCESS;
        }

        cee_map_mo_p = dynamic_cast<CeeMapManagedObject *> ((*pResults)[0]);
        if (cee_map_mo_p == NULL) {
            return WAVE_MESSAGE_SUCCESS;
        }

        pResults->clear ();
        delete pResults;

        remap_priority = cee_map_mo_p->getRemapFabricPriority();
        lossless_priority = cee_map_mo_p->getRemapLosslessPriority();

        delete cee_map_mo_p;
        
        if (remap_priority == (SI32)priority) {
            return FCOE_FAB_MAP_ERR_PRIORTY_MATCHES_REMAP_PRIORITY;
        }
            
        if (lossless_priority == (SI32)priority) {
            return FCOE_FAB_MAP_ERR_PRIORTY_MATCHES_REMAP_LOSSLESS_PRIORITY;
        }
            
        return WAVE_MESSAGE_SUCCESS;
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeFabricMapSendToLocal (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalFcoeFabricMapMessage  *msg_p = NULL;
        FcoeLocalFcoeFabricMapMessage   *loc_msg_p = NULL;
        WaveSendToClusterContext        *send_ctx_p;
        //FcoeLocalFcoeFabricMapMessage   *rbk_msg_p = NULL;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        
        FCOE_PLUG_DBG("Entering ... ");
        msg_p = dynamic_cast<FcoeGlobalFcoeFabricMapMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);

        // if the massage is received from the FcoeEnodeWyserEaConfigurationWorker just return here.
        if(msg_p->getOperBmp() == FCOE_FAB_MAP_OPER_UPD_FCOE_ENODES) {
            seq_ctx_p->executeNextStep(status);
            return;
        }

       if(msg_p->getOperBmp() == FCOE_FAB_MAP_OPER_UPD_MAX_ENODES) {
            seq_ctx_p->executeNextStep(FCOE_ENODE_FABRIC_MAP_MAX_ENODE_NOT_ALLOWED);
            return;
        }
        
        if (msg_p->getOperBmp() & FCOE_FAB_MAP_OPER_UPD_PRIORITY) {
            status = FcoeGlobalFabricMapValidatePriorityChange(msg_p->getPriority(), NULL);
            if (status != WAVE_MESSAGE_SUCCESS) {
                seq_ctx_p->executeNextStep(status);
                return;
            }
        }

        loc_msg_p = new FcoeLocalFcoeFabricMapMessage(msg_p->getOperBmp(),
                                                      msg_p->getName(),
                                                      msg_p->getVlan(),
                                                      msg_p->getVfid(),
                                                      msg_p->getPriority(),
                                                      msg_p->getFcmap(),
                                                      msg_p->getAdvIntvl(),
                                                      msg_p->getTimeout(),
                                                      msg_p->getMtu(),
                                                      msg_p->getMaxEnodes(),
                                                      msg_p->getEnodesCfgMode());
        if (loc_msg_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_LOCAL_MESSAGE);
            return;
        }

        
        FCOE_PLUG_DBG_S(string("NEW Local FAB-MAP Msg: Oper=") + loc_msg_p->getOper_bmp() +
                        string(" Fab-Map-name=") + loc_msg_p->getFabric_map_name() +
                        string(" Vlan=") + loc_msg_p->getVlan() +
                        string(" VFID=") + loc_msg_p->getVfid() +
                        string(" Priority=") + loc_msg_p->getPriority() +
                        string(" FCMAP=") + loc_msg_p->getFcmap() +
                        string(" FKA=") + loc_msg_p->getFka_intvl() +
                        string(" Timeout=") + loc_msg_p->getTimeout() +
                        string(" MTU=") + loc_msg_p->getMtu() +
                        string(" MAX-Enodes=") + loc_msg_p->getMaxEnodes() +
                        string(" Enodes-cfg-mode=") + loc_msg_p->getEnodesCfgMode());
        
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalSendToClusterCallback),
                                                  seq_ctx_p);
        FCOE_PLUG_ASSERT_PTR(send_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        /*
        if (FcoeGlobalWorker::FcoeFabricMapNeedToCreateRollbackMessage(msg_p->getOperBmp())) {
            rbk_msg_p = FcoeGlobalWorker::FcoeGlobalFcoeFabricMapGetRollbackMessage(msg_p);
            if (rbk_msg_p == NULL) {
                seq_ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_ROLLBACK_MESSAGE);
                delete loc_msg_p;
                delete send_ctx_p;
                return;
            }
            send_ctx_p->setPPrismMessageForPhase2(rbk_msg_p);
        }
        */

        send_ctx_p->setPartialSuccessFlag(true);
        sendToWaveCluster(send_ctx_p);

        return;
    }



    void
    FcoeGlobalWorker::FcoeGlobalFcoeFabricMapHandleConfig(FcoeLinearSequencerContext *seq_ctx_p)
    {
        string fab_map_name;
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        FcoeGlobalFcoeFabricMapMessage  *msg_p = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFabricMapManagedObject::getClassName());
        UI32 max_enodes, fcoe_enodes; 
        FCOE_PLUG_DBG("Entering ... ");

        msg_p = dynamic_cast<FcoeGlobalFcoeFabricMapMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);

        // This is a GET request, no DB update is required for this operation.
        if (msg_p->getOperBmp() == FCOE_FAB_MAP_OPER_GET_FCF_MAC_ADDR) {
            seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        if (msg_p->getOperBmp() == FCOE_FAB_MAP_OPER_UPD_MAX_ENODES) {
            seq_ctx_p->executeNextStep(FCOE_ENODE_FABRIC_MAP_MAX_ENODE_NOT_ALLOWED);
            return;
        }

        fab_map_name = msg_p->getName();
        if (fab_map_name.empty() == true) {
            seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_NAME_EMPTY);
            return;
        }
      
        // If it is a create REQ, Query the MO instance
        if (msg_p->getOperBmp() != FCOE_FAB_MAP_OPER_CREATE) {
            fab_map_mo_p = FcoeGlobalWorker::FcoeFabricMapGetMOByName(fab_map_name, seq_ctx_p);
            if (fab_map_mo_p == NULL) {
                seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_NOT_FOUND_IN_DB);
                return;
            }
            FCOE_PLUG_DBG_S(string("FOUND FAB-MAP with Name=")+fab_map_mo_p->getName());
        }

        switch (msg_p->getOperBmp()) {
            case FCOE_FAB_MAP_OPER_CREATE:
                fab_map_mo_p = new FcoeFabricMapManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()));
                if (fab_map_mo_p == NULL) {
                    seq_ctx_p->executeNextStep(FCOE_FAB_MAP_ERR_FAILED_TO_CREATE_OBJ);
                    return;
                }


                fab_map_mo_p->setName(msg_p->getName());
                fab_map_mo_p->setVlan(0);
                fab_map_mo_p->setPriority(FCOE_FAB_MAP_DEFAULT_PRIORITY);
                fab_map_mo_p->setFcmap(string("00:00:00"));
                fab_map_mo_p->setVfid(0);
                fab_map_mo_p->setFka_intvl(FCOE_FIP_DEFAULT_FKA_INTVL);
                fab_map_mo_p->setTimeout(FCOE_FIP_DEFAULT_TIMEOUT);
                fab_map_mo_p->setMtu(FCOE_FAB_MAP_DEFAULT_MTU);
				fab_map_mo_p->setMaxEnodes(FCOE_FAB_MAP_DEFAULT_MAX_ENODES);
                fab_map_mo_p->setEnodesCfgMode(FCOE_FAB_MAP_DEFAULT_ENODES_CFG_MODE);
                break;
               
            case FCOE_FAB_MAP_OPER_UPD_FCMAP:
                FCOE_PLUG_DBG_S(string("Rcvd Update FCMAP= ")+msg_p->getFcmap());
                fab_map_mo_p->setFcmap(msg_p->getFcmap());
                break;

            case FCOE_FAB_MAP_OPER_UPD_VLAN:
                FCOE_PLUG_DBG_S(string("Rcvd Update VLAN = ")+msg_p->getVlan());
                fab_map_mo_p->setVlan(msg_p->getVlan());
                break;

            case FCOE_FAB_MAP_OPER_UPD_PRIORITY:
                FCOE_PLUG_DBG_S(string("Rcvd Update Priority=")+msg_p->getPriority());
                fab_map_mo_p->setPriority(msg_p->getPriority());
                break;

            case FCOE_FAB_MAP_OPER_UPD_VFID:
                FCOE_PLUG_DBG_S(string("Rcvd Update VFID=")+msg_p->getVfid());
                fab_map_mo_p->setVfid(msg_p->getVfid());
                break;

            case FCOE_FAB_MAP_OPER_UPD_TIMEOUT:
                FCOE_PLUG_DBG_S(string("Rcvd Update Timeout=")+msg_p->getTimeout());
                fab_map_mo_p->setTimeout(msg_p->getTimeout());
                break;

            case FCOE_FAB_MAP_OPER_UPD_FKA_INTVL:
                FCOE_PLUG_DBG_S(string("Rcvd Update FKA_INTVL=")+msg_p->getAdvIntvl());
                fab_map_mo_p->setFka_intvl(msg_p->getAdvIntvl());
                break;

            case FCOE_FAB_MAP_OPER_UPD_MTU:
                FCOE_PLUG_DBG_S(string("Rcvd Update MTU=")+msg_p->getMtu());
                fab_map_mo_p->setMtu(msg_p->getMtu());
                break;
                
            case FCOE_FAB_MAP_OPER_UPD_MAX_ENODES:
                FCOE_PLUG_DBG_S(string("Rcvd Update MAX_ENODES=")+msg_p->getMaxEnodes());
                fab_map_mo_p->setMaxEnodes(msg_p->getMaxEnodes());
                break;

            case FCOE_FAB_MAP_OPER_UPD_ENODES_CFG_MODE:
                max_enodes = FCOE_FAB_MAP_DEFAULT_MAX_ENODES;
                FCOE_PLUG_DBG_S(string("Rcvd Update Enodes CFG Mode= ")+msg_p->getEnodesCfgMode());
                fab_map_mo_p->setEnodesCfgMode(msg_p->getEnodesCfgMode());
                if (msg_p->getEnodesCfgMode().compare("local") == 0) {
                    max_enodes = FcoeGlobalWorker::recalculateMaxEnodes(seq_ctx_p);
                    FCOE_PLUG_DBG_S(string("Max enodes set to = ")+max_enodes);
                    fab_map_mo_p->setMaxEnodes(max_enodes);
                }
                break;

            case FCOE_FAB_MAP_OPER_UPD_FCOE_ENODES: 
                 fcoe_enodes = msg_p->getMaxEnodes();
                 FCOE_PLUG_DBG_S(string("Rcvd Update from enode call point worker for MAX_ENODES=")+msg_p->getMaxEnodes());
      
                if(fcoe_enodes < FCOE_FAB_MAP_DEFAULT_MAX_ENODES) {
                    fab_map_mo_p->setMaxEnodes(FCOE_FAB_MAP_DEFAULT_MAX_ENODES);
                } else {
                    fab_map_mo_p->setMaxEnodes(fcoe_enodes);
                }
                break;

        }

        if (msg_p->getOperBmp() != FCOE_FAB_MAP_OPER_CREATE) {
            updateWaveManagedObject(fab_map_mo_p);
        }

        // Add this for deletion after the transaction.
        seq_ctx_p->addMOPointerToReleaseVector(fab_map_mo_p);

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);


        return;
    }


    void  FcoeGlobalWorker::FcoeGlobalFcoeFabricMapMessageHandler(FcoeGlobalFcoeFabricMapMessage *pFcoeGlobalFcoeFabricMapMessage)
    {
        // Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeFabricMapSendToNsmGlobal),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeFabricMapSendToLocal),
            
            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeFabricMapHandleConfig),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeGlobalFcoeFabricMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    FcoeLocalFcoeFipMessage *
    FcoeGlobalWorker::FcoeGlobalFcoeFipGetRollbackMessage (PrismMessage *prism_msg_p)
    {
        FcoeLocalFcoeFipMessage     *rbk_msg_p = NULL;
        FcoeFipManagedObject        *fip_mo_p = NULL;
        FcoeGlobalFipMessage        *msg_p = NULL;

        msg_p = dynamic_cast <FcoeGlobalFipMessage *> (prism_msg_p);
        
        fip_mo_p = FcoeGlobalWorker::FcoeFipGetMO();
        if (fip_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to find FIP MO");
            return NULL;
        }
        
        rbk_msg_p = new FcoeLocalFcoeFipMessage(msg_p->getOperBmp(),
                                                fip_mo_p->getFka_intvl(),
                                                fip_mo_p->getTimeout());
        
        if (rbk_msg_p == NULL) {
            delete fip_mo_p;
            return NULL;
        }

        FCOE_PLUG_DBG_S(string("Created FIP Rollback Message. Oper=") + msg_p->getOperBmp() +
                        string(" Intvl=") + rbk_msg_p->getFka_intvl() +
                        string(" Timeout=") + rbk_msg_p->getTimeout());

        delete fip_mo_p;

        return rbk_msg_p;
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeFipSendToLocal (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalFipMessage        *msg_p = NULL;
        FcoeLocalFcoeFipMessage     *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
        //FcoeLocalFcoeFipMessage     *rbk_msg_p = NULL;
        
        FCOE_PLUG_DBG("Entering ... ");
        msg_p = dynamic_cast<FcoeGlobalFipMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new FcoeLocalFcoeFipMessage(msg_p->getOperBmp(),
                                                msg_p->getFka_intvl(),
                                                msg_p->getTimeout());
        if (loc_msg_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_LOCAL_MESSAGE);
            return;
        }

        FCOE_PLUG_DBG_S(string("Created FIP Rollback Message. Oper=") + msg_p->getOperBmp() +
                        string(" Intvl=") + loc_msg_p->getFka_intvl() +
                        string(" Timeout=") + loc_msg_p->getTimeout());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalSendToClusterCallback),
                                                  seq_ctx_p);
        FCOE_PLUG_ASSERT_PTR(send_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        /*
        rbk_msg_p = FcoeGlobalWorker::FcoeGlobalFcoeFipGetRollbackMessage(msg_p);
        if (rbk_msg_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_ROLLBACK_MESSAGE);
            return;
        }
        send_ctx_p->setPPrismMessageForPhase2(rbk_msg_p);
        */
        send_ctx_p->setPartialSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);
        
        return;
    }

    FcoeFipManagedObject *
    FcoeGlobalWorker::FcoeFipGetMO (FcoeLinearSequencerContext *seq_ctx_p)
    {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFipManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL == pResults) {
            FCOE_PLUG_DBG_ERR("Query returned NULL results");
            return NULL;
        }

        FcoeFipManagedObject *fip_mo_p = NULL;

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_ERR("Query returned 0 or >1 results, MO not found");
            if (seq_ctx_p) {
                seq_ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }

        fip_mo_p = dynamic_cast<FcoeFipManagedObject *> ((*pResults)[0]);
        if (fip_mo_p == NULL) {
            return NULL;
        }

        pResults->clear ();
        delete pResults;

        return fip_mo_p;
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeFipHandleConfig (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FCOE_PLUG_DBG("Entering...");
        
        FcoeGlobalFipMessage *msg_p = dynamic_cast<FcoeGlobalFipMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);

        FcoeFipManagedObject *fip_mo_p = FcoeGlobalWorker::FcoeFipGetMO(seq_ctx_p);
        if (fip_mo_p == NULL) {
            seq_ctx_p->executeNextStep(FCOE_FIP_ERR_NOT_FOUND_IN_DB);
            return;
        }

        switch (msg_p->getOperBmp()) {
            case FCOE_FIP_OPER_UPD_FKA_INTVL:
                fip_mo_p->setFka_intvl(msg_p->getFka_intvl());
                break;

            case FCOE_FIP_OPER_UPD_TIMEOUT:
                fip_mo_p->setTimeout(msg_p->getTimeout());
                break;

            default:
                FCOE_PLUG_ASSERT(0);
                break;
        }

        updateWaveManagedObject(fip_mo_p);

        // Add it to the Release vector for deletion later
        seq_ctx_p->addMOPointerToReleaseVector(fip_mo_p);

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        return;
    }

    void  FcoeGlobalWorker::FcoeGlobalFipMessageHandler( FcoeGlobalFipMessage *pFcoeGlobalFipMessage)
    {
        // Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeFipSendToLocal),
            
            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeFipHandleConfig),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeGlobalFipMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeClearSendToLocal (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalFcoeClearMessage          *msg_p = NULL;
        FcoeLocalFcoeClearMessage       *loc_msg_p = NULL;
        WaveSendToClusterContext        *send_ctx_p;
        
        FCOE_PLUG_DBG("Entering ... ");
        msg_p = dynamic_cast<FcoeGlobalFcoeClearMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new FcoeLocalFcoeClearMessage(msg_p->getCmdCode());
        FCOE_PLUG_ASSERT_PTR(loc_msg_p);

        loc_msg_p->setVlan(msg_p->getVlan());
        loc_msg_p->setVfid(msg_p->getVfid());
        loc_msg_p->setFcUport(msg_p->getFcUport());
        loc_msg_p->setIfName(msg_p->getIfName());
        loc_msg_p->setWWN(msg_p->getWWN());
        loc_msg_p->setPo(msg_p->getPo());
        
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalSendToClusterCallback),
                                                  seq_ctx_p);
        FCOE_PLUG_ASSERT_PTR(send_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        send_ctx_p->setPartialSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);
        
        return;
    }

    void  FcoeGlobalWorker::FcoeGlobalFcoeClearMessageHandler (FcoeGlobalFcoeClearMessage *pClearMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeClearSendToLocal),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pClearMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeShowSendToLocal (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalFcoeShowMessage          *msg_p = NULL;
        FcoeLocalFcoeShowMessage       *loc_msg_p = NULL;
        WaveSendToClusterContext        *send_ctx_p;
        
        FCOE_PLUG_DBG("Entering ... ");
        msg_p = dynamic_cast<FcoeGlobalFcoeShowMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new FcoeLocalFcoeShowMessage(msg_p->getCmdCode());
        FCOE_PLUG_ASSERT_PTR(loc_msg_p);

        loc_msg_p->setVlan(msg_p->getVlan());
        loc_msg_p->setVfid(msg_p->getVfid());
        loc_msg_p->setFcUport(msg_p->getFcUport());
        loc_msg_p->setPhyIfName(msg_p->getPhyIfName());
        loc_msg_p->setFabMapName(msg_p->getFabMapName());
        loc_msg_p->setMapName(msg_p->getMapName());
        loc_msg_p->setStatsEnable(msg_p->getStatsEnable());
        
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalSendToClusterCallback),
                                                  seq_ctx_p);
        FCOE_PLUG_ASSERT_PTR(send_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        send_ctx_p->setPartialSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);
        
        return;
    }


    void  FcoeGlobalWorker::FcoeGlobalFcoeShowMessageHandler (FcoeGlobalFcoeShowMessage *pShowMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeShowSendToLocal),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeGlobalWorker::FcoeGlobalFcoeInternalSendToLocal (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalFcoeInternalMessage          *msg_p = NULL;
        FcoeLocalFcoeInternalMessage       *loc_msg_p = NULL;
        WaveSendToClusterContext        *send_ctx_p;

        FCOE_PLUG_DBG("Entering ... ");
        msg_p = dynamic_cast<FcoeGlobalFcoeInternalMessage *>(seq_ctx_p->getPPrismMessage ());
        FCOE_PLUG_ASSERT_PTR(msg_p);


        loc_msg_p = new FcoeLocalFcoeInternalMessage(msg_p->getCmdCode());
        FCOE_PLUG_ASSERT_PTR(loc_msg_p);

        loc_msg_p->setVlan(msg_p->getVlan());
        loc_msg_p->setVfid(msg_p->getVfid());
        loc_msg_p->setIfName(msg_p->getIfName());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalSendToClusterCallback),
                                                  seq_ctx_p);
        FCOE_PLUG_ASSERT_PTR(send_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        sendToWaveCluster(send_ctx_p);

        return;
    }

    void  FcoeGlobalWorker::FcoeGlobalFcoeInternalMessageHandler (FcoeGlobalFcoeInternalMessage *pInternalMessage)
    {   
        PrismLinearSequencerStep sequencerSteps[] =
        {   
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeInternalSendToLocal),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pInternalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

/*
    void
    FcoeGlobalWorker::FcoeLinearSequencerStartTransactionStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        seq_ctx_p->setTransactionStartedStatus(true);
        FcoeGlobalWorker::prismLinearSequencerStartTransactionStep(seq_ctx_p);
    }

    void
    FcoeGlobalWorker::FcoeLinearSequencerCommitTransactionStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalWorker::prismLinearSequencerCommitTransactionStep(seq_ctx_p);
    }

    void
    FcoeGlobalWorker::FcoeLinearSequencerSucceededStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeGlobalWorker::prismLinearSequencerSucceededStep(seq_ctx_p);
    }
*/        

    void
    FcoeGlobalWorker::FcoeLinearSequencerFailedStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        PrismMessage                    *msg_p = seq_ctx_p->getPPrismMessage ();
        PrismMessage                    *rbk_msg_p = NULL;
        WaveSendToClusterContext        *send_ctx_p = NULL;

        FCOE_PLUG_DBG_S(string("Entered FAILED step. TRANS=") + seq_ctx_p->getTransactionStartedStatus());

        if (seq_ctx_p->getTransactionStartedStatus() == false) {
            FcoeGlobalWorker::prismLinearSequencerFailedStep(seq_ctx_p);
            return;
        }

        // Disable the Rollback code.
        FcoeGlobalWorker::prismLinearSequencerFailedStep(seq_ctx_p);
        return;
        
        switch (msg_p->getOperationCode()) {
            case FCOE_GLOBAL_FCOE_FABRIC_MAP:
                rbk_msg_p = FcoeGlobalFcoeFabricMapGetRollbackMessage(msg_p);
                break;

            case FCOE_GLOBAL_FCOE_MAP:
                rbk_msg_p = FcoeGlobalFcoeMapGetRollbackMessage(msg_p);
                break;

            case FCOE_GLOBAL_FCOE_FIP:
                rbk_msg_p = FcoeGlobalFcoeFipGetRollbackMessage(msg_p);
                break;

            default:
                break;
        }

        if (rbk_msg_p) {
            send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalTransFailRollbackCompleteCallback),
                                                      seq_ctx_p);
            FCOE_PLUG_ASSERT_PTR(send_ctx_p);
            send_ctx_p->setPPrismMessageForPhase1(rbk_msg_p);

            sendToWaveCluster(send_ctx_p);
        } else {
            FcoeGlobalWorker::prismLinearSequencerFailedStep(seq_ctx_p);
        }
        
        return;
    }


    void FcoeGlobalWorker::upgrade (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase)
    {   
        FCOE_PLUG_DBG("Entering upgrade phase for FCOE Global plugin");

        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        FcoeMapManagedObject            *fcoe_map_mo_p = NULL;
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        ObjectId                        fab_map_object_id;
        vector<ObjectId>                fab_map_vector;
        string                          default_map_name(FCOE_MO_DEFAULT_NAME);
        string                          default_enodes_config(FCOE_FAB_MAP_DEFAULT_ENODES_CFG_MODE);

        fab_map_mo_p = FcoeGlobalWorker::FcoeFabricMapGetMOByName(default_map_name);

        // If fabric-map is not present, create new fabric-map with default values
        // IF fabric-map is present, update enode-config mode to default in fabric-map
        if (fab_map_mo_p == NULL) {
            FCOE_PLUG_DBG("Fcoe Fabric map MO not present. Creating MO with default values");

            startTransaction();

       	    fab_map_mo_p = new FcoeFabricMapManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()),
                                          FCOE_MO_DEFAULT_NAME,
                                          FCOE_FAB_MAP_DEFAULT_VLAN,
                                          FCOE_FAB_MAP_DEFAULT_PRIORITY,
                                          FCOE_FAB_MAP_DEFAULT_FCMAP,
                                          FCOE_FAB_MAP_DEFAULT_VFID,
                                          FCOE_FAB_MAP_DEFAULT_FKA_INTVL,
                                          FCOE_FAB_MAP_DEFAULT_TIMEOUT,
                                          FCOE_FAB_MAP_DEFAULT_MTU,
                                          FCOE_FAB_MAP_DEFAULT_MAX_ENODES,
                                          FCOE_FAB_MAP_DEFAULT_ENODES_CFG_MODE);

            fab_map_object_id = fab_map_mo_p->getObjectId();
            fab_map_vector.push_back(fab_map_object_id);

            fcoe_map_mo_p = FcoeGlobalWorker::FcoeMapGetMOByName(default_map_name);
            if (fcoe_map_mo_p == NULL) {
               FCOE_PLUG_DBG_ERR("Fcoe map MO is NULL");
               pWaveAsynchronousContextForUpgradePhase->setCompletionStatus (FCOE_MAP_ERR_FCOE_MAP_NOT_FOUND_IN_DB);
               pWaveAsynchronousContextForUpgradePhase->callback ();
               return; 
            }

            fcoe_map_mo_p->setFabric_map_list(fab_map_vector);
            updateWaveManagedObject (fcoe_map_mo_p);
            status = commitTransaction ();
            if (status == FRAMEWORK_SUCCESS) {
        	    status = WAVE_MESSAGE_SUCCESS;
            } else {
        	    FCOE_PLUG_ASSERT(0);
            }
        	delete fcoe_map_mo_p;

        } else {
            FCOE_PLUG_DBG("Fcoe Fabric map MO already present. Updating enodes-config mode to default");

            startTransaction();
            fab_map_mo_p->setEnodesCfgMode(default_enodes_config);
            updateWaveManagedObject (fab_map_mo_p);
            status = commitTransaction ();
            if (status == FRAMEWORK_SUCCESS) {
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                FCOE_PLUG_ASSERT(0);
            }

        }

        delete fab_map_mo_p;

        pWaveAsynchronousContextForUpgradePhase->setCompletionStatus (status);
        pWaveAsynchronousContextForUpgradePhase->callback ();
        return;

    }

    void FcoeGlobalWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        FCOE_PLUG_DBG("Entering install phase for FCOE Plug, creating default MOs");

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT)
                && (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
            FCOE_PLUG_DBG("Not a FIRST_TIME_BOOT or a PERSISTENT_WITH_DEFAULT_BOOT, Returning");
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }
       
#if 0 
        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
            FCOE_PLUG_DBG("VCS is Disabled");
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }
#endif

		FcoeMapManagedObject            *fcoe_map_mo_p;
		FcoeFabricMapManagedObject      *fab_map_mo_p;
		FcoeFipManagedObject            *fip_mo_p;
		FcoeGlobalFsbManagedObject      *fsb_mo_p;
		ObjectId                        cee_map_object_id;
		ObjectId                        fab_map_object_id;
		vector<ObjectId>                fab_map_vector;
		string                          cee_map_name(FCOE_MO_DEFAULT_NAME);
		string                          fab_map_name(FCOE_MO_DEFAULT_NAME);

		// Get the CEE MAP OBJ ID outside the TRANS Scope.
		cee_map_object_id = FcoeGlobalWorker::CeeMapGetObjectIdByName(cee_map_name);

		startTransaction();

		fab_map_mo_p = new FcoeFabricMapManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()),
										  FCOE_MO_DEFAULT_NAME, 
										  FCOE_FAB_MAP_DEFAULT_VLAN,
										  FCOE_FAB_MAP_DEFAULT_PRIORITY,
										  FCOE_FAB_MAP_DEFAULT_FCMAP,
										  FCOE_FAB_MAP_DEFAULT_VFID,
										  FCOE_FAB_MAP_DEFAULT_FKA_INTVL,
										  FCOE_FAB_MAP_DEFAULT_TIMEOUT,
										  FCOE_FAB_MAP_DEFAULT_MTU,
										  FCOE_FAB_MAP_DEFAULT_MAX_ENODES,
                                          FCOE_FAB_MAP_DEFAULT_ENODES_CFG_MODE);

        fip_mo_p = new FcoeFipManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()),
										FCOE_FAB_MAP_DEFAULT_FKA_INTVL,
										FCOE_FAB_MAP_DEFAULT_TIMEOUT);


		fab_map_object_id = fab_map_mo_p->getObjectId();
		fab_map_vector.push_back(fab_map_object_id);
		fcoe_map_mo_p = new FcoeMapManagedObject(dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()),
											 FCOE_MO_DEFAULT_NAME,
											 cee_map_object_id,
											 fab_map_vector);

		fsb_mo_p = new FcoeGlobalFsbManagedObject (dynamic_cast<FcoeGlobalObjectManager *>(getPWaveObjectManager()), FALSE);
		
		status = commitTransaction ();
		if (status == FRAMEWORK_SUCCESS) {
		status = WAVE_MESSAGE_SUCCESS;
		} else {
		FCOE_PLUG_ASSERT(0);
		}

		//delete fcoe_map_mo_p;
		delete fab_map_mo_p;
		delete fip_mo_p;
		delete fcoe_map_mo_p;
		delete fsb_mo_p;

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
		pWaveAsynchronousContextForBootPhases->callback ();
		return;

    }

FcoeGlobalFsbManagedObject *
    FcoeGlobalWorker::FcoeFsbGetMO (FcoeLinearSequencerContext *seq_ctx_p)
    {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeGlobalFsbManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL == pResults) {
            FCOE_PLUG_DBG_ERR("Query returned NULL results");
            return NULL;
        }

        FcoeGlobalFsbManagedObject *fsb_mo_p = NULL;

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_ERR("Query returned 0 or >1 results, MO not found");
            if (seq_ctx_p) {
                seq_ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }

        fsb_mo_p = dynamic_cast<FcoeGlobalFsbManagedObject *> ((*pResults)[0]);
        if (fsb_mo_p == NULL) {
            return NULL;
        }

        pResults->clear ();
        delete pResults;

        return fsb_mo_p;
    }

void
  FcoeGlobalWorker::FcoeGlobalFcoeFsbSendToLocal(FcoeLinearSequencerContext *ctx_p)
  {
    FcoeGlobalFcoeFsbMessage    *msg_p = NULL;
    FcoeLocalFcoeFsbMessage *loc_msg_p = NULL;
    WaveSendToClusterContext        *send_ctx_p = NULL;
	UI32    loc_id = 0;

    FCOE_PLUG_DBG("Entering ... ");

	loc_id = FrameworkToolKit::getThisLocationId();

	if (NsmUtils::isFcoeConfiguredForEthPort(loc_id)) {
        ctx_p->executeNextStep(FCOE_FSB_ERR_FCOE_PORT_CONFIG_PRESENT);
		return;
	}	

    if (NsmUtils::isFcoeConfiguredOnAnyProfiledPort()) {
        ctx_p->executeNextStep(FCOE_FSB_ERR_FCOE_PORT_PROFILE_CONFIG_PRESENT);
        return;
    }

    msg_p = dynamic_cast<FcoeGlobalFcoeFsbMessage *> (ctx_p->getPPrismMessage ());
    FCOE_PLUG_ASSERT_PTR(msg_p);

    loc_msg_p = new FcoeLocalFcoeFsbMessage(msg_p->getMode());
    if (loc_msg_p == NULL) {
        ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_LOCAL_MESSAGE);
        return;
    }

    FCOE_PLUG_DBG_S(string("NEW Local FSB Msg: mode=") + loc_msg_p->getMode());

    send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&FcoeGlobalWorker::FcoeGlobalSendToClusterCallback),
                                                  ctx_p);
    FCOE_PLUG_ASSERT_PTR(send_ctx_p);
    send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

    send_ctx_p->setPartialSuccessFlag(true);
    sendToWaveCluster(send_ctx_p);

    FCOE_PLUG_DBG("FcoeGlobalFcoeFsbSendToLocal Success");
    return;
  }

 void
    FcoeGlobalWorker::FcoeGlobalFcoeFsbHandleConfig(FcoeLinearSequencerContext *ctx_p)
    {
        FCOE_PLUG_DBG("FcoeGlobalFcoeFsbHandleConfig Entering...");

        FcoeGlobalFcoeFsbMessage *msg_p = dynamic_cast<FcoeGlobalFcoeFsbMessage *> (ctx_p->getPPrismMessage ());

        FcoeGlobalFsbManagedObject *fsb_mo_p = FcoeGlobalWorker::FcoeFsbGetMO(ctx_p);

        if (fsb_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("FcoeGlobalFcoeFsbHandleConfig FSB MO is NULL");
            ctx_p->executeNextStep(FCOE_FSB_ERR_NOT_FOUND_IN_DB);
            return;
        }

        fsb_mo_p->setMode(msg_p->getMode());

        updateWaveManagedObject(fsb_mo_p);

        FCOE_PLUG_DBG("FcoeGlobalFcoeFsbHandleConfig after update");
        // Add this for deletion later.
        ctx_p->addMOPointerToReleaseVector(fsb_mo_p);

        ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        FCOE_PLUG_DBG("FcoeGlobalFcoeFsbHandleConfig Success");

        return;
    }

void  FcoeGlobalWorker::FcoeGlobalFcoeFsbMessageHandler(FcoeGlobalFcoeFsbMessage *pFcoeGlobalFcoeFsbMessage)
    {
        // Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeFsbSendToLocal),

            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeGlobalFcoeFsbHandleConfig),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeGlobalFcoeFsbMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }
 

    UI32 FcoeGlobalWorker::recalculateMaxEnodes(FcoeLinearSequencerContext *seq_ctx_p)
    {  
        UI32 max_enodes = FCOE_FAB_MAP_DEFAULT_MAX_ENODES;
 
        FcoeEnodeLocalManagedObject *enode_mo_p = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeEnodeLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0) {
            FCOE_PLUG_DBG("No fcoe enode MOs found");
            if (seq_ctx_p) {
                seq_ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            } 
            return FCOE_FAB_MAP_DEFAULT_MAX_ENODES;
        }

        for (UI32 i = 0; i < numberOfResults; i++) {
            enode_mo_p = dynamic_cast<FcoeEnodeLocalManagedObject *> ((*pResults)[i]);
            if (enode_mo_p->getFcoeEnodes() > max_enodes) {
                max_enodes = enode_mo_p->getFcoeEnodes();
            }
            if (seq_ctx_p) {
                seq_ctx_p->addMOPointerToReleaseVector(enode_mo_p);
            } else {
                delete enode_mo_p;
            }
        }

        pResults->clear();
        delete pResults;

        return max_enodes;
    }
}

