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
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/FcoeWorker/FcoeGlobalFcoeportAttrMessage.h"

#include "Nsm/Global/NsmGlobalTypes.h"
#include "Fcoe/Common/FcoePluginDebug.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Common/FcoePluginUtils.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

#include "Nsm/Global/FcoeWorker/NsmFcoeGlobalWorker.h"

#include "DcmCore/DcmToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/FcoeWorker/NsmFcoeWorkerTypes.h"
namespace DcmNs
{

    NsmFcoeGlobalWorker::NsmFcoeGlobalWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : FcoeCommonWorker (pNsmGlobalObjectManager)
    {
        FcoeportAttrGlobalManagedObject    FcoeportAttrGlobalManagedObject    (pNsmGlobalObjectManager);
        FcoeportAttrGlobalManagedObject.setupOrm ();
        addManagedClass (FcoeportAttrGlobalManagedObject::getClassName (), this);
        addOperationMap (NSM_FCOE_WORKER_FCOEPORT_ATTR, reinterpret_cast<PrismMessageHandler> (&NsmFcoeGlobalWorker::FcoeGlobalFcoeportAttrMessageHandler));
    }

    NsmFcoeGlobalWorker::~NsmFcoeGlobalWorker ()
    {
    }

    PrismMessage  *NsmFcoeGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSM_FCOE_WORKER_FCOEPORT_ATTR:
                pPrismMessage = new FcoeGlobalFcoeportAttrMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmFcoeGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FcoeportAttrGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FcoeportAttrGlobalManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "NsmFcoeGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void NsmFcoeGlobalWorker::NsmGlobalFcoeportAttrSendToClusterCallback (PrismAsynchronousContext *ctx_p)
    {
        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        WaveSendToClusterContext                *send_ctx_p = NULL;
        PrismMessage                            *gl_msg_p = NULL;
        PrismMessage                            *loc_msg_p = NULL;
        vector<LocationId>                      loc_vector;
        ResourceId                              status = WAVE_MESSAGE_SUCCESS;
        ResourceId                              final_status = WAVE_MESSAGE_SUCCESS;
        UI32                                    i = 0;
        bool                                    local_plugin_error = false;
        ResourceId                              local_plugin_error_status = WAVE_MESSAGE_SUCCESS;

        seq_ctx_p = reinterpret_cast<FcoeLinearSequencerContext *>(ctx_p->getPCallerContext());
        gl_msg_p = seq_ctx_p->getPPrismMessage ();
        FCOE_PLUG_ASSERT(gl_msg_p);

        send_ctx_p = dynamic_cast<WaveSendToClusterContext *> (ctx_p);

        loc_vector = send_ctx_p->getLocationsToSendToForPhase1();
        for (i = 0; i < loc_vector.size(); i++) {
            status = send_ctx_p->getCompletionStatusForPhase1(loc_vector[i]);
            FCOE_PLUG_DBG_S(string("Apply Status of Location[") + loc_vector[i] +
            string("] = ") + FrameworkToolKit::localize(status));

            if (status != WAVE_MESSAGE_SUCCESS) {
                local_plugin_error = true;
                local_plugin_error_status = status;
            }

            loc_msg_p = send_ctx_p->getResultingMessageForPhase1(loc_vector[i]);
            FCOE_PLUG_ASSERT(loc_msg_p);

            loc_msg_p->transferAllBuffers(gl_msg_p);
        }

        final_status = ClusterGetStatusFromContext(send_ctx_p);

        /* If any local plugin returns error, use that error code instead of generic cluster error.
         * This fix has a limitation that if there are different errors returned by different local
         * plugins, only the last error will be returned. But since we can return and print only
         * one error, this is still better than printing the generic error code.
         */

        if (local_plugin_error) {
            final_status = local_plugin_error_status;
        }

        FCOE_PLUG_DBG_S(string("Final status of Cluster Operation=") +
                        FrameworkToolKit::localize(final_status));

        gl_msg_p->setCompletionStatus(final_status);

        seq_ctx_p->executeNextStep(final_status);

        // Delete the APPLY message
        loc_msg_p = send_ctx_p->getPPrismMessageForPhase1();
        if (loc_msg_p) {
            delete loc_msg_p;
        }
        delete send_ctx_p;
        return;
    }

    void NsmFcoeGlobalWorker::FcoeGlobalFcoeportAttrSendToLocal (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeGlobalFcoeportAttrMessage *msg_p = dynamic_cast<FcoeGlobalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());

        FcoeLocalFcoeportAttrMessage  *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;

        FCOE_PLUG_DBG_S(string("RCVD FCOEPORT Msg: Oper=") + msg_p->getOper_bmp() +
                        string(" Po =") + msg_p->getIfId() +
                        string(" Map-name=") + msg_p->getMap_name());


		if (NsmUtils::isAnyPoMemberFcoeConfigured(msg_p->getIfId())) {
			FCOE_PLUG_DBG_ERR_S(string("Member has fcoe config."));
			ctx_p->executeNextStep(FCOE_PORT_ERR_LAG_MBR_FCOE_ENABLED);
			return;
		}
	    
			
        loc_msg_p = new FcoeLocalFcoeportAttrMessage(msg_p->getOper_bmp(),
                                                     msg_p->getIfId(),
                                                     msg_p->getMap_name(), msg_p->get_prov_context());
        if (loc_msg_p == NULL) {
            ctx_p->executeNextStep(FCOE_PORT_ERR_FAILED_TO_ALLOC_LOCAL_MESSAGE);
            return;
        }

        send_ctx_p = new WaveSendToClusterContext(this,
                                                  reinterpret_cast<PrismAsynchronousCallback>(&NsmFcoeGlobalWorker::NsmGlobalFcoeportAttrSendToClusterCallback),
                                                  ctx_p);

        if (send_ctx_p == NULL) {
            ctx_p->executeNextStep(FCOE_COMMON_ERR_FAILED_TO_ALLOC_CLUSTER_CONTEXT);
            delete loc_msg_p;
            return;
        }

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        sendToWaveCluster(send_ctx_p);

        return;
    }

    FcoeMapManagedObject*
    NsmFcoeGlobalWorker::FcoeMapGetMoByName (string &map_name,
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
    NsmFcoeGlobalWorker::FcoeMapGetObjectIdByName (string &map_name,
                                                   FcoeLinearSequencerContext *ctx_p)
    {
        FcoeMapManagedObject    *map_mo_p = NsmFcoeGlobalWorker::FcoeMapGetMoByName(map_name);
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

    PoIntfManagedObject*
    NsmFcoeGlobalWorker::GetPoIntfMoById (string &if_name,
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

    FcoeportAttrGlobalManagedObject*
    NsmFcoeGlobalWorker::FcoeportGetChildMoByParentMo(PoIntfManagedObject *if_mo_p,
                                                     FcoeLinearSequencerContext *ctx_p)
    {
        
        WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> child;

        child = if_mo_p->getFcoeport();

        return child.operator->();
    }
        
    void NsmFcoeGlobalWorker::FcoeGlobalFcoeportAttrUpdateConfig (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeGlobalFcoeportAttrMessage   *msg_p = dynamic_cast<FcoeGlobalFcoeportAttrMessage *> (ctx_p->getPPrismMessage ());
        FcoeportAttrGlobalManagedObject *fcoeport_mo_p = NULL;
        PoIntfManagedObject             *if_mo_p = NULL;
        ObjectId                        fcoe_map_object_id;
        string                          fcoe_map_name;
        string                          if_id;

        WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> child;

        if_id = msg_p->getIfId();
		if(msg_p->get_prov_context() == FCOE_PROVISIONING_VIA_PORT_PROFILE_PORT) {
            ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }
        if_mo_p = NsmFcoeGlobalWorker::GetPoIntfMoById(if_id);
        

        FCOE_PLUG_DBG_S(string("RCVD FCOEPORT Msg: Oper=") + msg_p->getOper_bmp() +
                        string(" Po =") + msg_p->getIfId() +
                        string(" Map-name=") + msg_p->getMap_name());

        switch (msg_p->getOper_bmp()) {
            case FCOEPORT_ATTR_LAG_CREATE:
                FCOE_PLUG_DBG_S(string("Recvd CREATE request for FCOE Port for Po=")+msg_p->getIfId());
                fcoe_map_name = msg_p->getMap_name();

                fcoe_map_object_id = NsmFcoeGlobalWorker::FcoeMapGetObjectIdByName(fcoe_map_name, ctx_p); 
                
                fcoeport_mo_p = new FcoeportAttrGlobalManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()),
                                                                    fcoe_map_object_id);
                if_mo_p->setFcoeport(WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject>(fcoeport_mo_p));
                updateWaveManagedObject(if_mo_p);
                break;

            case FCOEPORT_ATTR_LAG_REMOVE:
                FCOE_PLUG_DBG_S(string("Recvd REMOVE request for FCOE Port for Interface=")+msg_p->getIfId());
                FCOE_PLUG_DBG_S(string("Found TE MO Name=")+if_mo_p->getId());
                
                fcoeport_mo_p = NsmFcoeGlobalWorker::FcoeportGetChildMoByParentMo(if_mo_p, ctx_p);
                FCOE_PLUG_ASSERT_PTR(fcoeport_mo_p);
                
                //FCOE_PLUG_DBG_S(string("Fcoeport CHILD MO for Map=") + fcoeport_mo_p->getMap());

                if_mo_p->setFcoeport(WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject>(NULL));
                updateWaveManagedObject(if_mo_p);

                break;
        }

        ctx_p->addMOPointerToReleaseVector(if_mo_p);
        
        ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }


    void  NsmFcoeGlobalWorker::FcoeGlobalFcoeportAttrMessageHandler(FcoeGlobalFcoeportAttrMessage *pFcoeGlobalFcoeportAttrMessage)
    {

        FCOE_PLUG_DBG("Entering.....");
        // Validations and Queries should go here
        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeGlobalWorker::FcoeGlobalFcoeportAttrSendToLocal),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeGlobalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeGlobalWorker::FcoeGlobalFcoeportAttrUpdateConfig),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeGlobalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeGlobalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&NsmFcoeGlobalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeGlobalFcoeportAttrMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start();
    }
}
