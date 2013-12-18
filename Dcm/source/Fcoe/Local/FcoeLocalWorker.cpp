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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalWorker.h"
#include "Fcoe/Local/FcoeLocalFcoeIntfMessage.h"
#include "Fcoe/Local/FcoeLocalETIntfMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeFabricMapMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeMapMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeFipMessage.h"
#include "Fcoe/Local/FcoeIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeETIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeLocalFcoeShowMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"
#include "Fcoe/Local/FcoeLocalFcoeClearMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeInternalMessage.h"
#include "Fcoe/Local/FcoeLocalFcoeFsbMessage.h"

#include "Nsm/Local/FcoeWorker/FcoeLocalFcoeportAttrMessage.h"

#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForUpgradePhase.h"
#include "ClientInterface/Fcoe/FcoeClientFabricMapMessage.h"
#include "ClientInterface/Fcoe/FcoeClientFipMessage.h"
#include "ClientInterface/Fcoe/FcoeClientMapMessage.h"
#include "ClientInterface/Fcoe/FcoeClientShowMessage.h"
#include "ClientInterface/Fcoe/FcoeClientIntfMessage.h"
#include "ClientInterface/Fcoe/FcoeClientETIntfMessage.h"
#include "ClientInterface/Fcoe/FcoeClientClearMessage.h"
#include "ClientInterface/Fcoe/FcoeClientInternalMessage.h"
#include "ClientInterface/Fcoe/FcoeClientPluginControlMessage.h"
#include "ClientInterface/Fcoe/FcoeClientFsbMessage.h"

#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/LldpFcoePriorityConfMessage.h"
#include "ClientInterface/Lldp/LldpFcoeClientFsbMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Fcoe/Common/FcoePluginDebug.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Common/FcoePluginUtils.h"
#include "Fcoe/Common/FcoeCommonWorker.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Fcoe/Common/FcoePluginUtils.h"
#include "Fcoe/Local/FcoeLocalEnodeMessage.h"
#include "Fcoe/Local/FcoeEnodeLocalManagedObject.h"
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"


#include "sys/fabos/fabobj.h"
#include "hasm/hasm.h"

extern "C" {
#include <switch/port.h>
#include <fabric/getid.h>
#include <agd/public.h>
}

namespace DcmNs
{

    FcoeLocalWorker::FcoeLocalWorker ( FcoeLocalObjectManager *pFcoeLocalObjectManager)
        : FcoeCommonWorker  (pFcoeLocalObjectManager)
    {
        FcoeIntfLocalManagedObject    FcoeIntfLocalManagedObject    (pFcoeLocalObjectManager);
        FcoeIntfLocalManagedObject.setupOrm ();
        addManagedClass (FcoeIntfLocalManagedObject::getClassName (), this);

        FcoeETIntfLocalManagedObject    FcoeETIntfLocalManagedObject    (pFcoeLocalObjectManager);
        FcoeETIntfLocalManagedObject.setupOrm ();
        addManagedClass (FcoeETIntfLocalManagedObject::getClassName (), this);

        FcoeEnodeLocalManagedObject    FcoeEnodeLocalManagedObject    (pFcoeLocalObjectManager);
        FcoeEnodeLocalManagedObject.setupOrm ();
        addManagedClass (FcoeEnodeLocalManagedObject::getClassName (), this);

        addOperationMap (FCOE_LOCAL_UPD_FCOE_INTF, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeIntfMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_FABRIC_MAP, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeFabricMapMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_MAP, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeMapMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_FIP, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeFipMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_SHOW, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeShowMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_CLEAR, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeClearMessageHandler));
        addOperationMap (FCOE_LOCAL_ET_INTF, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalETIntfMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_INTERNAL, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeInternalMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_FSB, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalFcoeFsbMessageHandler));
        addOperationMap (FCOE_LOCAL_FCOE_ENODE, reinterpret_cast<PrismMessageHandler> (&FcoeLocalWorker::FcoeLocalEnodeMessageHandler));

    }

    FcoeLocalWorker::~FcoeLocalWorker ()
    {
    }

    PrismMessage  *FcoeLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FCOE_LOCAL_UPD_FCOE_INTF :
                pPrismMessage = new FcoeLocalFcoeIntfMessage ();
                break;
            case FCOE_LOCAL_FCOE_FABRIC_MAP :
                pPrismMessage = new FcoeLocalFcoeFabricMapMessage ();
                break;
            case FCOE_LOCAL_FCOE_MAP :
                pPrismMessage = new FcoeLocalFcoeMapMessage ();
                break;
            case FCOE_LOCAL_FCOE_FIP :
                pPrismMessage = new FcoeLocalFcoeFipMessage ();
                break;

            case FCOE_LOCAL_FCOE_SHOW:
                pPrismMessage = new FcoeLocalFcoeShowMessage ();
                break;

            case FCOE_LOCAL_FCOE_CLEAR:
                pPrismMessage = new FcoeLocalFcoeClearMessage();
                break;

            case FCOE_LOCAL_ET_INTF:
                pPrismMessage = new FcoeLocalETIntfMessage();
                break;

            case FCOE_LOCAL_FCOE_INTERNAL:
                pPrismMessage = new FcoeLocalFcoeInternalMessage();
                break;
            case FCOE_LOCAL_FCOE_ENODE :
                pPrismMessage = new FcoeLocalEnodeMessage ();
                break;

            case FCOE_LOCAL_FCOE_FSB :  
                pPrismMessage = new FcoeLocalFcoeFsbMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }


        return (pPrismMessage);
    }

    WaveManagedObject  *FcoeLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FcoeIntfLocalManagedObject::getClassName ()) == managedClassName) {
            pWaveManagedObject = new FcoeIntfLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()));
        } else if ((FcoeETIntfLocalManagedObject::getClassName ()) == managedClassName) {
            pWaveManagedObject = new FcoeETIntfLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()));
        } else if ((FcoeEnodeLocalManagedObject::getClassName ()) == managedClassName) {
            pWaveManagedObject = new FcoeEnodeLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()));

        } else {
            trace (TRACE_LEVEL_FATAL, "FcoeLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void 
    FcoeLocalWorker::FcoeLocalFcoeFabricMapSendToLldpDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeFabricMapMessage   *msg_p = dynamic_cast<FcoeLocalFcoeFabricMapMessage *> (ctx_p->getPPrismMessage ());
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        UI32                            priority_bitmap = 0;
        UI32                            new_priority = 0;
        LldpFcoePriorityConfMessage     *lldp_msg_p = NULL;
        UI32                            oper_bmp;

        oper_bmp = msg_p->getOper_bmp();
        
        if ((oper_bmp & FCOE_FAB_MAP_OPER_UPD_PRIORITY) == 0) {
            FCOE_PLUG_DBG_S(string("Not a priority command, returning"));
            ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }
    
        new_priority = msg_p->getPriority();
        priority_bitmap = 1 << new_priority;

        FCOE_PLUG_DBG_S(string("Sending Priority Bitmap to LLDP with new prority=") + msg_p->getPriority());

        lldp_msg_p = new LldpFcoePriorityConfMessage(0, priority_bitmap);
        FCOE_PLUG_ASSERT_PTR(lldp_msg_p);

        FCOE_SEND_SYNC_TO_DAEMON(LLDP_DCM_CLIENT_NAME, lldp_msg_p, status, FCOE_DEL_CLIENT_MSG);

        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR("Failed to update fcoe priority in LLDP daemon");
            status = FCOE_FAB_MAP_ERR_FAILED_TO_SET_PRIORITY_RETRY;
        }

        ctx_p->executeNextStep(status);
        return;
    }
    
    void
    FcoeLocalWorker::FcoeLocalFcoeFabricMapSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeFabricMapMessage   *msg_p = dynamic_cast<FcoeLocalFcoeFabricMapMessage *> (ctx_p->getPPrismMessage ());
        FcoeClientFabricMapMessage      *cl_msg_p = NULL;
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        void                            *data_p = NULL;
        UI32                            size = 0;
        UI32                            max_enodes_val = msg_p->getMaxEnodes();
        UI32                            oper_bmp = msg_p->getOper_bmp();

        FCOE_PLUG_DBG_S(string("RCVD Local FAB-MAP Msg: Oper=") + msg_p->getOper_bmp() +
                        string(" Fab-Map-name=") + msg_p->getFabric_map_name() +
                        string(" Vlan=") + msg_p->getVlan() +
                        string(" VFID=") + msg_p->getVfid() +
                        string(" Priority=") + msg_p->getPriority() +
                        string(" FCMAP=") + msg_p->getFcmap() +
                        string(" FKA=") + msg_p->getFka_intvl() +
                        string(" Timeout=") + msg_p->getTimeout() +
                        string(" MTU=") + msg_p->getMtu() +
                        string(" MAX-Enodes") + msg_p->getMaxEnodes() +
                        string(" Enodes Cfg Mode=") + msg_p->getEnodesCfgMode());

        if (oper_bmp & FCOE_FAB_MAP_OPER_UPD_ENODES_CFG_MODE) {
            FCOE_PLUG_DBG_S(string("Enode Cfg mode message received"));
            oper_bmp = FCOE_FAB_MAP_OPER_UPD_MAX_ENODES;

            if (msg_p->getEnodesCfgMode().compare("local") == 0) {
                // If Enodes config mode is local, calculate fcoe-enodes value
                // and send message to daemon

                max_enodes_val = FcoeLocalCalculateFcoeEnodes(ctx_p);

            } else if (msg_p->getEnodesCfgMode().compare("global") == 0) {
                // If Enodes config mode is global, send message to daemon to 
                // with global max-enodes value

                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFabricMapManagedObject::getClassName());
                vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

                UI32 numberOfResults = pResults->size ();
                if (numberOfResults == 0) {
                    FCOE_PLUG_DBG_ERR("Query returned 0 results, MO not found");
                    ctx_p->addMOVectorToReleaseVector(pResults);
                    ctx_p->executeNextStep(FCOE_ENODE_FABRIC_MAP_MO_NOT_FOUND);
                    return;
                }
                fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *> ((*pResults)[0]);
                max_enodes_val = fab_map_mo_p->getMaxEnodes();
            } 
        }

        cl_msg_p = new FcoeClientFabricMapMessage(oper_bmp,
                                               msg_p->getFabric_map_name(),
                                               msg_p->getVlan(),
                                               msg_p->getVfid(),
                                               msg_p->getPriority(),
                                               msg_p->getFcmap(),
                                               msg_p->getFka_intvl(),
                                               msg_p->getTimeout(),
                                               msg_p->getMtu(),
                                               max_enodes_val);

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DONT_DEL_CLIENT_MSG);
        
        
        if (status == WAVE_MESSAGE_SUCCESS) {
            data_p = cl_msg_p->findBuffer(msg_p->getOper_bmp(), size);
            if (data_p) {
                msg_p->addBuffer(DcmToolKit::getLocalMappedId(),
                                 size, (unsigned char *)data_p, false);
            }
        }

        delete cl_msg_p;
        delete fab_map_mo_p; 
        ctx_p->executeNextStep(status);

        return;
    }

    void
    FcoeLocalWorker::FcoeLocalFcoeFabricMapUpdateEnodes (FcoeLinearSequencerContext *ctx_p)
    {   
        FcoeLocalFcoeFabricMapMessage   *msg_p = dynamic_cast<FcoeLocalFcoeFabricMapMessage *> (ctx_p->getPPrismMessage ());
        FcoeFabricMapManagedObject      *fab_map_mo_p = NULL;
        FcoeEnodeLocalManagedObject     *enode_mo_p = NULL;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        UI32                            oper_bmp;

        oper_bmp = msg_p->getOper_bmp();

        if ((oper_bmp & FCOE_FAB_MAP_OPER_UPD_ENODES_CFG_MODE) == 0) {
            FCOE_PLUG_DBG_S(string("Not a enodes cfg message, returning"));
            ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

       // Get Fcoe enodes MO
        enode_mo_p = FcoeLocalEnodeGetMOByName(ctx_p);

        if (enode_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Fcoe Enodes MO not found");
            ctx_p->executeNextStep(FCOE_ENODE_MO_NOT_FOUND);
            return;
        }

        if (msg_p->getEnodesCfgMode().compare("local") == 0) {
           // Update fcoe enodes value
            enode_mo_p->setFcoeEnodes(FcoeLocalCalculateFcoeEnodes(ctx_p));
            updateWaveManagedObject(enode_mo_p);
            
        } else if (msg_p->getEnodesCfgMode().compare("global") == 0) {
           // Get Fabric map MO
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt_fab(FcoeFabricMapManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults_fab = querySynchronously (&syncQueryCtxt_fab);

            UI32 numberOfResults = pResults_fab->size ();
            if (numberOfResults == 0) {
                FCOE_PLUG_DBG_ERR("Query returned 0 results, MO not found");
                ctx_p->addMOVectorToReleaseVector(pResults_fab);
                ctx_p->executeNextStep(FCOE_ENODE_FABRIC_MAP_MO_NOT_FOUND);
                return;
            }
            fab_map_mo_p = dynamic_cast<FcoeFabricMapManagedObject *> ((*pResults_fab)[0]);

           // Update Fcoe enodes value with global max-enodes
            enode_mo_p->setFcoeEnodes(fab_map_mo_p->getMaxEnodes());
            updateWaveManagedObject(enode_mo_p);

           // Add this for deletion later.
           ctx_p->addMOPointerToReleaseVector(fab_map_mo_p);

        }
        ctx_p->addMOPointerToReleaseVector(enode_mo_p);
        ctx_p->executeNextStep(status);

        return;
    }

    UI32
    FcoeLocalWorker::FcoeLocalCalculateFcoeEnodes (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeClientIntfMessage           *msg_p = NULL;
        fcoe_intf_config_msg_t          *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        UI32                            size = 0;
        UI32                            fcoe_enodes_val = 0;

        // Check license
        if (licenseCheck(BASE_FCOE_LICENSE) == 0) {
           return FCOE_DEFAULT_FCOE_ENODES_WITH_OUT_LICENSE;
        }

        // Fetch number of bound interfaces from daemon
        msg_p = new FcoeClientIntfMessage();
        msg_p->setOper_bmp(FCOE_INTF_OPER_GET_IF_BIND_LIST);

        status = sendSynchronouslyToWaveClient("fcoed", msg_p);
        if (status == WAVE_MESSAGE_SUCCESS) {
            data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
            if (data_p != NULL) {
                trace(TRACE_LEVEL_INFO, string("Number of bound interfaces: ") + data_p->num_entries);
                if (data_p->num_entries <= FCOE_DEFAULT_FCOE_ENODES_WITH_LICENSE) {
                    fcoe_enodes_val = FCOE_DEFAULT_FCOE_ENODES_WITH_LICENSE;
                } else {
                    fcoe_enodes_val = data_p->num_entries;
                }
            }
        }
        delete msg_p;
        return fcoe_enodes_val;

    }


    void  FcoeLocalWorker::FcoeLocalFcoeFabricMapMessageHandler( FcoeLocalFcoeFabricMapMessage *pFcoeLocalFcoeFabricMapMessage)
    {

        FCOE_PLUG_DBG("Entering.....");
        // Validations and Queries should go here
        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalFcoeFabricMapSendToLldpDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalFcoeFabricMapSendToDaemon),

            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalFcoeFabricMapUpdateEnodes),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeFabricMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void 
    FcoeLocalWorker::FcoeLocalFcoeFipSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeFipMessage *msg_p = dynamic_cast<FcoeLocalFcoeFipMessage *> (ctx_p->getPPrismMessage ());
        FcoeClientFipMessage *cl_msg_p;
        ResourceId status;

        FCOE_PLUG_DBG_S(string("RCVD Local FAB-FIP Msg: Oper=") + msg_p->getOper_bmp() +
                        string(" FKA=") + msg_p->getFka_intvl() +
                        string(" Timeout=") + msg_p->getTimeout());

        cl_msg_p = new FcoeClientFipMessage(msg_p->getOper_bmp(),
                                            msg_p->getFka_intvl(),
                                            msg_p->getTimeout());

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

        ctx_p->executeNextStep(status);
        
        return;
    }

    void  FcoeLocalWorker::FcoeLocalFcoeFipMessageHandler( FcoeLocalFcoeFipMessage *pFcoeLocalFcoeFipMessage)
    {
        // Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalFcoeFipSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeFipMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeLocalWorker::FcoeLocalFcoeMapSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeMapMessage *msg_p = dynamic_cast<FcoeLocalFcoeMapMessage *> (ctx_p->getPPrismMessage ());
        FcoeClientMapMessage *cl_msg_p;
        ResourceId status;

        FCOE_PLUG_DBG_S(string("RCVD Local FCOE-MAP Msg: Oper=") + msg_p->getOper_bmp() +
                        string(" FabName=") + msg_p->getFabric_map_name() +
                        string(" CeeMapName=") + msg_p->getCee_map_name());

        cl_msg_p = new FcoeClientMapMessage(msg_p->getOper_bmp(),
                                            msg_p->getMap_name(),
                                            msg_p->getFabric_map_name(),
                                            msg_p->getCee_map_name());
        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);
        
        ctx_p->executeNextStep(status);
        return;
    }

    void  FcoeLocalWorker::FcoeLocalFcoeMapMessageHandler( FcoeLocalFcoeMapMessage *pFcoeLocalFcoeMapMessage)
    {
        // Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalFcoeMapSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    FcoeIntfLocalManagedObject *
    FcoeLocalWorker::FcoeLocalIntfGetMOByName(string &if_name,
                                              FcoeLinearSequencerContext *ctx_p)
    {
        FcoeIntfLocalManagedObject *if_mo_p = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&if_name, "fcoe_if_name"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_DEBUG_S(string("Query returned ") + numberOfResults + string(" results... returning"));
            if (ctx_p) {
                ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
	    }
       
        if_mo_p = dynamic_cast<FcoeIntfLocalManagedObject *> ((*pResults)[0]);

        pResults->clear();
        delete pResults;

        return if_mo_p;
    }

    FcoeETIntfLocalManagedObject *
    FcoeLocalWorker::FcoeLocalETIntfGetMOByName(string &if_name,
                                                FcoeLinearSequencerContext *ctx_p)
    {
        FcoeETIntfLocalManagedObject *if_mo_p = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeETIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&if_name, "et_if_name"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_DEBUG_S(string("Query returned ") + numberOfResults + string(" results... returning"));
            if (ctx_p) {
                ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }
       
        if_mo_p = dynamic_cast<FcoeETIntfLocalManagedObject *> ((*pResults)[0]);

        pResults->clear();
        delete pResults;

        return if_mo_p;
    }

    void
    FcoeLocalWorker::FcoeLocalETIntfSendToDaemon(FcoeLinearSequencerContext *ctx_p)
    {
        string                          if_name;
        ResourceId                      status;
        FcoeClientETIntfMessage         *cl_msg_p = NULL;
        FcoeETIntfLocalManagedObject    *if_mo_p = NULL;
        FcoeLocalETIntfMessage          *msg_p = NULL;
       
        FCOE_PLUG_DBG("Entering.....");
 
        msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());
        if_name = msg_p->getIfName();

        if ((msg_p->getOper_bmp() == FCOE_ET_INTF_OPER_CREATE) ||
            (msg_p->getOper_bmp() == FCOE_ET_INTF_OPER_CREATE_BULK) ||
            (msg_p->getOper_bmp() == FCOE_ET_INTF_OPER_DEL_UPORT_BULK) ||
            (msg_p->getOper_bmp() == FCOE_ET_INTF_OPER_REMOVE)) {
            FCOE_PLUG_DBG_DEBUG_S(string("Recvd a create/delete message for ET Name=") + if_name + 
                            string(".. Nothing to update in Daemon"));
            ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }
        
        if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(if_name, ctx_p);
        if (if_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Interface not found in the DB");
            ctx_p->executeNextStep(FCOE_IF_ERR_NOT_FOUND_IN_DB);
            return;
        }

        FCOE_PLUG_DBG_DEBUG_S(string("Found MO with Name=fcoe ") + if_mo_p->getName());
        cl_msg_p = new FcoeClientETIntfMessage();
        if (cl_msg_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to alloc ET Client message");
            ctx_p->executeNextStep(FCOE_IF_ET_ERR_ALLOC_CLIENT_MSG);
            return;
        }

        cl_msg_p->setOper_bmp(msg_p->getOper_bmp());
        cl_msg_p->setIfName(msg_p->getIfName());
        cl_msg_p->setFcPortNum(if_mo_p->getFcUportNum());
        
        switch (msg_p->getOper_bmp()) {
            case FCOE_ET_INTF_OPER_UPD_ISL_STATUS:
                cl_msg_p->setISLStatus(msg_p->getStatus());
                break;

            case FCOE_ET_INTF_OPER_UPD_TRUNK_STATUS:
                cl_msg_p->setTrunkStatus(msg_p->getTrunkStatus());
                break;

            default:
                FCOE_PLUG_DBG_ERR_S(string("Unknown oper code") +
                                    msg_p->getOper_bmp());
                break;
        }

        delete if_mo_p;

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status,
                                 FCOE_DEL_CLIENT_MSG);

        ctx_p->executeNextStep(status);

        return;
    }

    PhyIntfLocalManagedObject*
    FcoeLocalWorker::GetPhyIntfMoById (string &if_name,
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

    void
    FcoeLocalWorker::FcoeLocalIntfSendToDaemon(FcoeLinearSequencerContext *ctx_p)
    {
        string                      if_name;
        ResourceId                  status = WAVE_MESSAGE_SUCCESS;
        FcoeLocalFcoeIntfMessage    *msg_p = NULL;
        FcoeIntfLocalManagedObject  *if_mo_p = NULL;
        FcoeClientIntfMessage       *cl_msg_p = NULL;
        string                      bind_if_name;
        UI32                        bind_val_type = 0;
        PhyIntfLocalManagedObject   *phy_if_mo_p = NULL;
        bool                        send_to_daemon = true;
        FcoeETIntfLocalManagedObject  *et_if_mo_p = NULL;
        
        msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());
        if_name = msg_p->getIfName();

        if ((msg_p->getOper_bmp() == FCOE_INTF_OPER_CREATE) ||
            (msg_p->getOper_bmp() == FCOE_INTF_OPER_CREATE_BULK) ||
            (msg_p->getOper_bmp() == FCOE_INTF_OPER_DELETE_BULK)) {
            FCOE_PLUG_DBG_DEBUG_S(string("Recvd a create message for Name=") + if_name +
                                  string(".. Nothing to update in Daemon"));
            ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }
        
        // Check if the license is present.
        FCOE_PLUGIN_LICENSE_CHECK_FOR_SEQUENCER(ctx_p);
        
        if_mo_p = FcoeLocalWorker::FcoeLocalIntfGetMOByName(if_name, ctx_p);
        if (if_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Interface not found in the DB");
            ctx_p->executeNextStep(FCOE_IF_ERR_NOT_FOUND_IN_DB);
            return;
        }

        cl_msg_p = new FcoeClientIntfMessage();
        if (cl_msg_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to alloc Client message");
            ctx_p->executeNextStep(FCOE_IF_ERR_ALLOC_CLIENT_MSG);
        	FCOE_PLUGIN_DELETE(if_mo_p);
            return;
        }
       
        cl_msg_p->setConfigReplay(FrameworkToolKit::isFileReplayInProgress());
       
        FCOE_PLUG_DBG_DEBUG_S(string("Found MO with Name=fcoe ") + if_mo_p->getName());
        cl_msg_p->setOper_bmp(msg_p->getOper_bmp());
        cl_msg_p->setIfName(msg_p->getIfName());
        cl_msg_p->setFcPortNum(if_mo_p->getFcUportNum());
        cl_msg_p->setPortNum(if_mo_p->getPortNum());
        cl_msg_p->setVnNum(if_mo_p->getVnNum());
        
        switch (msg_p->getOper_bmp()) {
            case FCOE_INTF_OPER_UPD_STATUS:
                /* Note: the status in Daemon & Yang are reversed
                   Yang:
                        TRUE = Shutdown
                        FALSE = No Shutdown

                   Daemon
                        1 = Enabled
                        0 = Disabled
                   
                   This is to get the desired CLI look and feel.
                */
                cl_msg_p->setStatus(!msg_p->getStatus());
                break;

            case FCOE_INTF_OPER_UPD_BIND:
                bind_val_type = msg_p->getBindingType();
                if (bind_val_type) {
                    cl_msg_p->setBindingType(msg_p->getBindingType());
                } else {
                    bind_val_type = if_mo_p->getBindingType();
                    cl_msg_p->setBindingType(if_mo_p->getBindingType());
                }

                bind_if_name = msg_p->getBinding();
                if (bind_if_name.empty() == 0) {
                    cl_msg_p->setBinding(msg_p->getBinding());
                } else {
                    cl_msg_p->setBinding(if_mo_p->getBinding());
                    bind_if_name = if_mo_p->getBinding();
                }

                if ((bind_val_type == FCOE_INTERFACE_BIND_VAL_TYPE_TE) ||
                    (bind_val_type == FCOE_INTERFACE_BIND_VAL_TYPE_FO)) {
                    phy_if_mo_p = FcoeLocalWorker::GetPhyIntfMoById(bind_if_name);
                    if (phy_if_mo_p == NULL) {
                        FCOE_PLUG_DBG_ERR_S(string("PHY Interface=") + bind_if_name +
                                            string("not found in the DB"));
                        status = FCOE_IF_ERR_ETH_PORT_NOT_FOUND;
                    } else {
                        et_if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(bind_if_name);
                        if (et_if_mo_p == NULL) {
                           FCOE_PLUG_DBG_S(string("Interface not found ") + bind_if_name);
                            ctx_p->executeNextStep(FCOE_IF_ERR_ETH_PORT_NOT_FOUND);
        					FCOE_PLUGIN_DELETE(if_mo_p);
        					FCOE_PLUGIN_DELETE(phy_if_mo_p);
                            delete cl_msg_p;
                            return;
                        }

                        // TBDDCMOSS : ifindex_get_speed need to be platform specific
                        //
                        #if 0
                        if ((bind_val_type == FCOE_INTERFACE_BIND_VAL_TYPE_TE) &&
                            (ifindex_get_speed(et_if_mo_p->getIfindex()) != IF_SPEED_10G)) {
                            FCOE_PLUG_DBG_S(string("Error: link speed ") + portGetLinkSpeed(et_if_mo_p->getFcUportNum()));
                            ctx_p->executeNextStep(FCOE_IF_ERR_BIND_INTERFACE_SPEED_MISMATCH);
        					FCOE_PLUGIN_DELETE(if_mo_p);
        					FCOE_PLUGIN_DELETE(phy_if_mo_p);
        					FCOE_PLUGIN_DELETE(et_if_mo_p);
                            delete cl_msg_p;
                            return;
                        }

                        if ((bind_val_type == FCOE_INTERFACE_BIND_VAL_TYPE_FO) &&
                            (ifindex_get_speed(et_if_mo_p->getIfindex()) != IF_SPEED_40G)) {
                            FCOE_PLUG_DBG_S(string("Error: link speed ") + portGetLinkSpeed(et_if_mo_p->getFcUportNum()));
                            ctx_p->executeNextStep(FCOE_IF_ERR_BIND_INTERFACE_SPEED_MISMATCH);
        					FCOE_PLUGIN_DELETE(if_mo_p);
        					FCOE_PLUGIN_DELETE(phy_if_mo_p);
        					FCOE_PLUGIN_DELETE(et_if_mo_p);
                            delete cl_msg_p;
                            return;
                        }
						#endif

                        cl_msg_p->setIfIndex(phy_if_mo_p->getIfIndex());
                        FCOE_PLUG_DBG_S(string("PHY Interface Ifindex=") + phy_if_mo_p->getIfIndex());
                        delete et_if_mo_p;
                    }
                }
                break;
                
            case FCOE_INTF_OPER_REM_BIND:
                if (if_mo_p->getBindingType() != FCOE_INTERFACE_BIND_VAL_TYPE_NONE) {
                    cl_msg_p->setBinding(msg_p->getBinding());
                    cl_msg_p->setBindingType(msg_p->getBindingType());
                } else {
                    send_to_daemon = false;
                    delete cl_msg_p;
                }
                break;

            default:
                FCOE_PLUG_DBG_ERR_S(string("Unknown oper code") +
                                    msg_p->getOper_bmp());
                break;
        }

        FCOE_PLUGIN_DELETE(phy_if_mo_p);
        FCOE_PLUGIN_DELETE(if_mo_p);

        if (send_to_daemon) {
            FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status,
                                 FCOE_DEL_CLIENT_MSG);
        }

        ctx_p->executeNextStep(status);

        return;
    }

    int
    FcoeLocalWorker::FcoeLocalIntfSendToDaemonOneBinding(
	    string if_name, FcoeIntfLocalManagedObject *fcoe_if_p,
        UI32 binding_type, string binding, FcoeLinearSequencerContext *ctx_p)
    {
        ResourceId                  status = WAVE_MESSAGE_SUCCESS;
        FcoeIntfLocalManagedObject  *if_mo_p = NULL;
        FcoeClientIntfMessage       *cl_msg_p = NULL;
        PhyIntfLocalManagedObject   *phy_if_mo_p = NULL;
        bool                        send_to_daemon = true;
       
        if (fcoe_if_p) {
            // if valid pointer is passed in, no reason to search the db
            // using if_name
            FCOE_PLUG_DBG_DEBUG_S(string("Using FCOE IF with name=") + if_name);
            if_mo_p = fcoe_if_p;
        } else {
            FCOE_PLUG_DBG_DEBUG_S(string("Searching FCOE IF with name=") + if_name);
            if_mo_p = FcoeLocalWorker::FcoeLocalIntfGetMOByName(if_name, ctx_p);
            if (if_mo_p == NULL) {
                FCOE_PLUG_DBG_ERR("Interface not found in the DB");
                return FCOE_IF_ERR_NOT_FOUND_IN_DB;
            }
        }
 
        cl_msg_p = new FcoeClientIntfMessage();
        if (cl_msg_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to alloc Client message");
            return (-1);;
        }
        
        cl_msg_p->setConfigReplay(FrameworkToolKit::isFileReplayInProgress());
       
        FCOE_PLUG_DBG_DEBUG_S(string("Found MO with Name=fcoe ") + if_mo_p->getName());
        cl_msg_p->setOper_bmp(FCOE_INTF_OPER_UPD_BIND);
        cl_msg_p->setIfName(if_name);
        cl_msg_p->setFcPortNum(if_mo_p->getFcUportNum());
        cl_msg_p->setPortNum(if_mo_p->getPortNum());
        cl_msg_p->setVnNum(if_mo_p->getVnNum());
        
	    cl_msg_p->setBindingType(binding_type);

	    cl_msg_p->setBinding(binding);

	    phy_if_mo_p = FcoeLocalWorker::GetPhyIntfMoById(binding);
	    if (phy_if_mo_p == NULL) {
	        FCOE_PLUG_DBG_ERR_S(string("PHY Interface= ") + binding +
	            string(" not found in the DB"));
	    } else {
	        cl_msg_p->setIfIndex(phy_if_mo_p->getIfIndex());
	        FCOE_PLUG_DBG_S(string("PHY Interface Ifindex=") + phy_if_mo_p->getIfIndex());
	        if (ctx_p) {
	            ctx_p->addMOPointerToReleaseVector(phy_if_mo_p);
	        }
	    }	

        if (send_to_daemon) {
            FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status,
                                 FCOE_DEL_CLIENT_MSG);
        }

        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalDeleteETUportBulk (FcoeLinearSequencerContext *ctx_p)
    {
        uint32_t                    i = 0;
        string                      if_name;
        fcoe_intf_config_msg_t      *data_p = NULL;
        UI32                        size = 0;
        FcoeLocalETIntfMessage      *msg_p = NULL;
        //UI32                        mapped_id = 0;
        SI32                        uport = -1;

        msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());

        data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
        if (data_p == NULL) {
            FCOE_PLUG_DBG_ERR_S(string("Payload for CREATE_BULK message is Null."));
            return FCOE_IF_ERR_CREATE_BULK_NULL_MSG_RCVD;
        }
        FCOE_PLUG_ASSERT(size == data_p->size);
        
        //mapped_id = DcmToolKit::getLocalMappedId();
        
        for (i = 0; i < data_p->num_entries; i++) {
            if_name.assign((char *)data_p->list[i].if_name);
            
            FCOE_PLUG_DBG_DEBUG_S(string("Searching ET IF with name=") + if_name);
            FcoeETIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(if_name, ctx_p);

            if (if_mo_p != NULL) {
                FCOE_PLUG_DBG_DEBUG_S(string("Deleting UPORT info for ET-Port= ") + if_mo_p->getName());
                if_mo_p->setFcUportNum(uport);
                updateWaveManagedObject(if_mo_p);
            } else {
                FCOE_PLUG_DBG_ERR_S(string("ET IF not found for name=") + if_name);
            }
                
            // Add this for deletion later.
            ctx_p->addMOPointerToReleaseVector(if_mo_p);
        }

        return WAVE_MESSAGE_SUCCESS;
    }


    ResourceId
    FcoeLocalWorker::FcoeLocalCreateETInterfaceBulk (FcoeLinearSequencerContext *ctx_p,
                                                     fcoe_intf_config_msg_t *arg_data_p)
	{
		uint32_t                    i = 0;
		string                      if_name;
		int32_t                     slot_id = -1;
		fcoe_intf_config_msg_t      *data_p = NULL;
		UI32                        size = 0;
		FcoeLocalETIntfMessage    *msg_p = NULL;
		UI32                        mapped_id = 0;
		FcoeETIntfLocalManagedObject *if_mo_p = NULL;
		UI32    loc_id = 0;


		msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());

		if (arg_data_p == NULL) {
			data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
			if (data_p == NULL) {
				FCOE_PLUG_DBG_ERR_S(string("Payload for CREATE_BULK message is Null."));
				return FCOE_IF_ERR_CREATE_BULK_NULL_MSG_RCVD;
			}
			FCOE_PLUG_ASSERT(size == data_p->size);
		} else {
			data_p = arg_data_p;
			size = arg_data_p->size;
		}
		mapped_id = DcmToolKit::getLocalMappedId();

		if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
			FCOE_PLUG_DBG_S(string("VCS is disabled. Not creating ET interface(s)"));
			return WAVE_MESSAGE_SUCCESS;
		}


		// Get all the MOs from Data base
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeETIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectField("et_if_name");
		loc_id = FrameworkToolKit::getThisLocationId();
		if (loc_id != 0) {
			// Add location id filter to the context
			syncQueryCtxt.setLocationIdFilter(loc_id);
		}
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		

		map<string, FcoeETIntfLocalManagedObject *>etIntMap;
		if (pResults && (pResults->size() > 0))
		{
			trace(TRACE_LEVEL_INFO, string("Total no of entries retrieved from DB: ") + pResults->size()); 
			for (UI32 etIndex = 0; etIndex < pResults->size(); etIndex++) {
				if_mo_p =  dynamic_cast<FcoeETIntfLocalManagedObject*>((*pResults)[etIndex]);
				etIntMap[if_mo_p->getName()] = if_mo_p;

			}
		} else 	if (pResults && (pResults->size() == 0)) {
			if (ctx_p) {
				ctx_p->addMOVectorToReleaseVector(pResults);
			} else {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}


		}


		trace(TRACE_LEVEL_INFO, string("Total no of ET interfaces Received from backend: ") + data_p->num_entries); 

		for (i = 0; i < data_p->num_entries; i++) {
			if_name.assign((char *)data_p->list[i].if_name);

			if (if_name.empty()) {
				FCOE_PLUG_DBG("ET If name is received as NULL from dameon");
				FCOE_PLUG_ASSERT(if_name.empty());
			}

			if_mo_p = NULL;
			if_mo_p = etIntMap[if_name];
			if (if_mo_p != NULL) {
				vector<string> listOfAttributes;
				if_mo_p->setIfindex(data_p->list[i].if_index);
				if_mo_p->setFcUportNum(data_p->list[i].fc_port_num);
				if_mo_p->setStatus(data_p->list[i].status);
				if_mo_p->setTrunkStatus(data_p->list[i].trunk_status);
				if_mo_p->setBindType(data_p->list[i].bind_type);
				if_mo_p->setAsBreakoutPort(data_p->list[i].breakout);

				listOfAttributes.push_back("ifindex");
				listOfAttributes.push_back("fc_uport_num");
				listOfAttributes.push_back("status");
				listOfAttributes.push_back("trunk_status");
				listOfAttributes.push_back("bind_type");
				listOfAttributes.push_back("breakout"); 

				if_mo_p->setAttributesToBeUpdated(listOfAttributes); 	
				updateWaveManagedObject(if_mo_p);
				trace(TRACE_LEVEL_INFO, string("successfully updated the MO with Name : ") + if_mo_p->getName());
				
				listOfAttributes.clear();
			} else {
				FCOE_PLUG_DBG_S(string("Creating ET IF name=") + if_name +
						string(" If-index=") + data_p->list[i].if_index +
						string(" FcUportNum=") + data_p->list[i].fc_port_num);

				if_mo_p = new FcoeETIntfLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()));
				FCOE_PLUG_ASSERT_PTR(if_mo_p);

				if_mo_p->setName(if_name);
				if_mo_p->setIfindex(data_p->list[i].if_index);
				if_mo_p->setFcUportNum(data_p->list[i].fc_port_num);
				if_mo_p->setStatus(data_p->list[i].status);

				if_mo_p->setTrunkStatus(data_p->list[i].trunk_status);
				if_mo_p->setRbId(mapped_id);
				if_mo_p->setBindType(data_p->list[i].bind_type);
				if_mo_p->setAsBreakoutPort(data_p->list[i].breakout);

				slot_id = NsmUtils::getSlotId(if_name);
				if_mo_p->associateWithSlot(slot_id);
				trace(TRACE_LEVEL_INFO, string("successfully created the MO with Name : ") + if_name); 
				ctx_p->addMOPointerToReleaseVector(if_mo_p);
			}

		}
			// Add this for deletion later.
		if (pResults && (pResults->size() > 0))
		{
			for (UI32 etIndex = 0; etIndex < pResults->size(); etIndex++) {
				if_mo_p =  dynamic_cast<FcoeETIntfLocalManagedObject*>((*pResults)[etIndex]);
				if(ctx_p) {
					ctx_p->addMOPointerToReleaseVector(if_mo_p);
				}
			}
			pResults->clear();
			delete pResults;
		}

		return WAVE_MESSAGE_SUCCESS;
	}



    ResourceId
    FcoeLocalWorker::FcoeLocalCreateETInterface(FcoeLinearSequencerContext *ctx_p)
    {
        string                          if_name;
        int32_t                         slot_id = -1;

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
           FCOE_PLUG_DBG_S(string("VCS is disabled. Not creating ET interface(s)"));
           return WAVE_MESSAGE_SUCCESS;
        }

        FcoeLocalETIntfMessage *msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());

        if_name = msg_p->getIfName();

        FCOE_PLUG_DBG_DEBUG_S(string("Searching ET IF with name=") + if_name);
        FcoeETIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(if_name, ctx_p);
   
        if (if_mo_p != NULL) {
            FCOE_PLUG_DBG_DEBUG_S(string("Already Found MO with Name=fcoe ") + if_mo_p->getName() +
                                  string(" Updating the UportNum=") + msg_p->getFcUportNum());
            if_mo_p->setFcUportNum(msg_p->getFcUportNum());
            updateWaveManagedObject(if_mo_p);
        } else {
            FCOE_PLUG_DBG_S(string("Creating ET IF name=") + if_name +
                            string(" If-index=") + msg_p->getIfIndex() +
                            string(" FcUportNum=") + msg_p->getFcUportNum());

            if_mo_p = new FcoeETIntfLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()));
            FCOE_PLUG_ASSERT_PTR(if_mo_p);
                    if_mo_p->setName(if_name);
                    if_mo_p->setIfindex(msg_p->getIfIndex());
                    if_mo_p->setFcUportNum(msg_p->getFcUportNum());
                    if_mo_p->setStatus(FCOE_ET_INTF_STATUS_DEFAULT);
                    if_mo_p->setTrunkStatus(FCOE_ET_INTF_TRUNK_STATUS_DEFAULT);
                    if_mo_p->setAsBreakoutPort(false);

                    slot_id = NsmUtils::getSlotId(if_name);
                    if_mo_p->associateWithSlot(slot_id);
                }

        // Add this for deletion later.
        ctx_p->addMOPointerToReleaseVector(if_mo_p);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalDeleteETInterface(FcoeLinearSequencerContext *ctx_p)
    {   
        string if_name;

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
           FCOE_PLUG_DBG_S(string("VCS is disabled. Not deleting ET interface(s)"));
           return WAVE_MESSAGE_SUCCESS;
        }

        FcoeLocalETIntfMessage *msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());

        if_name = msg_p->getIfName();

        FCOE_PLUG_DBG_S(string("Searching ET IF with name=") + if_name);
        FcoeETIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(if_name, ctx_p);

        if (if_mo_p != NULL) {
            trace (TRACE_LEVEL_INFO, string("FcoeLocalWorker::FcoeLocalDeleteETInterface:- Deleting ET IF with name=") + if_mo_p->getName());
            delete if_mo_p;
        } else {
            FCOE_PLUG_DBG_ERR_S(string("ET IF MO not found with name ") + if_mo_p->getName());
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalHandleETInterfaceISLStatusUpdate (FcoeLinearSequencerContext *ctx_p)
    {
        string                          if_name;

        FcoeLocalETIntfMessage *msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());

        if_name = msg_p->getIfName();
                FCOE_PLUG_DBG_S(string("Updating ISL status for ET port=") + if_name +
                                string("ISL status=") + msg_p->getStatus());
                
        FCOE_PLUG_DBG_DEBUG_S(string("Searching ET IF with name=") + if_name);
        FcoeETIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(if_name, ctx_p);
        
        if (if_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to find ET MO");
            return (FCOE_IF_ET_ERR_NOT_FOUND_IN_DB);
        }

        FCOE_PLUG_DBG_DEBUG_S(string("Found MO with Name=ET ") + if_mo_p->getName());

	if_mo_p->setStatus(msg_p->getStatus()); 

        updateWaveManagedObject(if_mo_p);

        // Add this for deletion later.
        ctx_p->addMOPointerToReleaseVector(if_mo_p);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalHandleETInterfaceTrunkStatusUpdate (FcoeLinearSequencerContext *ctx_p)
    {
        string                          if_name;

        FcoeLocalETIntfMessage *msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());

        if_name = msg_p->getIfName();
        FCOE_PLUG_DBG_S(string("Updating Trunk status for ET port=") + if_name +
                        string("Trunk status=") + msg_p->getTrunkStatus());

        FCOE_PLUG_DBG_DEBUG_S(string("Searching ET IF with name=") + if_name);
        FcoeETIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(if_name, ctx_p);
        
        if (if_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to find ET MO");
            return (FCOE_IF_ET_ERR_NOT_FOUND_IN_DB);
                }

                FCOE_PLUG_DBG_DEBUG_S(string("Found MO with Name=ET ") + if_mo_p->getName());
                if_mo_p->setTrunkStatus(msg_p->getTrunkStatus());

        updateWaveManagedObject(if_mo_p);

        // Add this for deletion later.
        ctx_p->addMOPointerToReleaseVector(if_mo_p);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalHandleETInterfaceStatusUpdate (FcoeLinearSequencerContext *ctx_p)
    {   
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        // ISL status update
        status = FcoeLocalWorker::FcoeLocalHandleETInterfaceISLStatusUpdate(ctx_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            ctx_p->executeNextStep(status);
            return status;
        }

        // Trunk status update
        status = FcoeLocalWorker::FcoeLocalHandleETInterfaceTrunkStatusUpdate(ctx_p);

        ctx_p->executeNextStep(status);
        return status;
 
    }

    void
    FcoeLocalWorker::FcoeLocalETIntfHandleConfig(FcoeLinearSequencerContext *ctx_p)
    {
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        FcoeLocalETIntfMessage *msg_p = dynamic_cast<FcoeLocalETIntfMessage *> (ctx_p->getPPrismMessage ());

        /* Upon snapshot restore, all fcoe ET configs (isl and trunk status) are replayed as one message
         * to fcoe plugin. Added a single function to handle this case
         */
        if ((msg_p->getOper_bmp() & FCOE_ET_INTF_OPER_UPD_ISL_STATUS) &&
            (msg_p->getOper_bmp() & FCOE_ET_INTF_OPER_UPD_TRUNK_STATUS)) {
                status = FcoeLocalWorker::FcoeLocalHandleETInterfaceStatusUpdate(ctx_p);
                ctx_p->executeNextStep(status);
                return;
        }

        switch (msg_p->getOper_bmp()) {
            case FCOE_ET_INTF_OPER_CREATE:
                status = FcoeLocalWorker::FcoeLocalCreateETInterface(ctx_p);
                break;

            case FCOE_ET_INTF_OPER_REMOVE:
                status = FcoeLocalWorker::FcoeLocalDeleteETInterface(ctx_p);
                break;


            case FCOE_ET_INTF_OPER_UPD_ISL_STATUS:
                status = FcoeLocalWorker::FcoeLocalHandleETInterfaceISLStatusUpdate(ctx_p);
                break;

            case FCOE_ET_INTF_OPER_UPD_TRUNK_STATUS:
                status = FcoeLocalWorker::FcoeLocalHandleETInterfaceTrunkStatusUpdate(ctx_p);
                break;

            case FCOE_ET_INTF_OPER_CREATE_BULK:
                status = FcoeLocalWorker::FcoeLocalCreateETInterfaceBulk(ctx_p);
                break;

            case FCOE_ET_INTF_OPER_DEL_UPORT_BULK:
                status = FcoeLocalWorker::FcoeLocalDeleteETUportBulk(ctx_p);
                break;

            default:
                FCOE_PLUG_DBG_ERR_S(string("Unknown operation code=") + msg_p->getOper_bmp());
                FCOE_PLUG_ASSERT(0);
                break;
        }

        ctx_p->executeNextStep(status);

        return;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalDeleteInterfaceBulk (FcoeLinearSequencerContext *ctx_p,
                                                   fcoe_intf_config_msg_t *arg_data_p)
    {
        uint32_t                    i = 0;
        string                      if_name;
        string                      bind_if_name;
        fcoe_intf_config_msg_t      *data_p = NULL;
        UI32                        size = 0;
        //UI32                        mapped_id = 0;
        FcoeLocalFcoeIntfMessage    *msg_p = NULL;

        msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());

        if (arg_data_p == NULL) {
            data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
            if (data_p == NULL) {
                FCOE_PLUG_DBG_ERR_S(string("Payload for CREATE_BULK message is Null."));
                return FCOE_IF_ERR_CREATE_BULK_NULL_MSG_RCVD;
            }
            FCOE_PLUG_ASSERT(size == data_p->size);
        } else {
            data_p = arg_data_p;
            size = arg_data_p->size;
        }
        
        //mapped_id = DcmToolKit::getLocalMappedId();

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
            FCOE_PLUG_DBG_S(string("VCS is disabled. Not creating FCOE interface(s)"));
            return WAVE_MESSAGE_SUCCESS;
        }

        for (i = 0; i < data_p->num_entries; i++) {
            if_name.assign((char *)data_p->list[i].if_name);
            
            FCOE_PLUG_DBG_DEBUG_S(string("Searching FCOE IF with name=") + if_name);
            FcoeIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalIntfGetMOByName(if_name, ctx_p);
                
            if (if_mo_p != NULL) {
                FCOE_PLUG_DBG_S(string("Deleting FCOE interface Name=") + if_name);
                delete (if_mo_p);
            } else {
                FCOE_PLUG_DBG_S(string("Couldn't FIND FCOE interface Name=") + if_name);
            }
        }

        return WAVE_MESSAGE_SUCCESS;
    }


    ResourceId
    FcoeLocalWorker::FcoeLocalCreateInterfaceBulk (FcoeLinearSequencerContext *ctx_p,
                                                   fcoe_intf_config_msg_t *arg_data_p)
	{
		uint32_t                    i = 0;
		string                      if_name;
		string                      bind_if_name;
		int32_t                     slot_id = -1;
		fcoe_intf_config_msg_t      *data_p = NULL;
		UI32                        size = 0;
		UI32                        mapped_id = 0;
		FcoeLocalFcoeIntfMessage    *msg_p = NULL;
		UI32    loc_id = 0;
		FcoeIntfLocalManagedObject *if_mo_p = NULL;



		msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());
        FcoeLocalObjectManager     *pFcoeLocalObjectManager;

		FCOE_PLUG_DBG("Entering..");

		if (arg_data_p == NULL) {
			data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
			if (data_p == NULL) {
				FCOE_PLUG_DBG_ERR_S(string("Payload for CREATE_BULK message is Null."));
				return FCOE_IF_ERR_CREATE_BULK_NULL_MSG_RCVD;
			}
			FCOE_PLUG_ASSERT(size == data_p->size);
		} else {
			data_p = arg_data_p;
			size = arg_data_p->size;
		}

		mapped_id = DcmToolKit::getLocalMappedId();

		if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
			FCOE_PLUG_DBG_S(string("VCS is disabled. Not creating FCOE interface(s)"))
		} else {



		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeIntfLocalManagedObject::getClassName());

		syncQueryCtxt.addSelectField("fcoe_if_name");
		loc_id = FrameworkToolKit::getThisLocationId();
		if (loc_id != 0) {
			// Add location id filter to the context
			syncQueryCtxt.setLocationIdFilter(loc_id);
		}
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		
		map<string, FcoeIntfLocalManagedObject *>IntMap;
		if (pResults && (pResults->size() > 0))
		{
			trace(TRACE_LEVEL_INFO, string("Total no of FCoE  interfaces Retrieved from DB: ") +  pResults->size()); 
			for (UI32 Index = 0; Index < pResults->size(); Index++) {
				if_mo_p =  dynamic_cast<FcoeIntfLocalManagedObject*>((*pResults)[Index]);
				IntMap[if_mo_p->getName()] = if_mo_p;

			}

		} else if(pResults && (pResults->size() == 0)) {
			if (ctx_p) {
				ctx_p->addMOVectorToReleaseVector(pResults);
			} else {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}

		}
		trace(TRACE_LEVEL_INFO, string("Total no of FCoE  interfaces Received from backend: ") + data_p->num_entries); 

		for (i = 0; i < data_p->num_entries; i++) {
			if_name.assign((char *)data_p->list[i].if_name);

			if (if_name.empty()) {
				FCOE_PLUG_DBG("FCOE If name is received as NULL from daemon");
				FCOE_PLUG_ASSERT(if_name.empty());
			}

			if_mo_p = NULL;
			if_mo_p = IntMap[if_name];

			if (if_mo_p != NULL) {
				FCOE_PLUG_DBG_DEBUG_S(string("Already Found MO with Name=fcoe ") + if_mo_p->getName());
				if_mo_p->setIfindex(data_p->list[i].if_index);
				if_mo_p->setFcUportNum(data_p->list[i].fc_port_num);

                bind_if_name.assign((char *)data_p->list[i].bind_name);
                if_mo_p->setBinding(bind_if_name);
                if_mo_p->setBindingType(data_p->list[i].bind_type);
				vector<string> listOfAttributes;

				listOfAttributes.push_back("ifindex");
				listOfAttributes.push_back("fc_port_name");
				if_mo_p->setAttributesToBeUpdated(listOfAttributes);	
				updateWaveManagedObject(if_mo_p);
				trace(TRACE_LEVEL_INFO, string("Successfully updated MO with Name: ") + if_mo_p->getName()); 
				listOfAttributes.clear();

			} else {
				bind_if_name.assign((char *)data_p->list[i].bind_name);
				if (bind_if_name.empty() == false) {
					slot_id = NsmUtils::getSlotId(bind_if_name);
				}

				FCOE_PLUG_DBG_S(string("Creating FCOE IF name=") + if_name +
						string(" If-index=") + data_p->list[i].if_index +
						string(" FcUportNum=") + data_p->list[i].fc_port_num +
						string(" BindType=") + data_p->list[i].bind_type +
						string(" BindName=") + bind_if_name +
						string(" Slot=") + slot_id);

				if_mo_p = new FcoeIntfLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()),
						if_name,
						data_p->list[i].if_index, 
						data_p->list[i].fc_port_num,
						mapped_id,
						data_p->list[i].vn_num,
						data_p->list[i].port_num,
						FCOE_INTF_STATUS_DEFAULT,
						FCOE_INTF_DEFAULT_MODE,
						bind_if_name,
						data_p->list[i].bind_type);
				FCOE_PLUG_ASSERT_PTR(if_mo_p);

				if_mo_p->associateWithSlot(slot_id);

            // Add this for deletion later.
            ctx_p->addMOPointerToReleaseVector(if_mo_p);
        }
        }

			// Add this for deletion later.

		if (pResults && (pResults->size() > 0)) {
			for (UI32 Index = 0; Index < pResults->size(); Index++) {
				if_mo_p =  dynamic_cast<FcoeIntfLocalManagedObject*>((*pResults)[Index]);
				if (ctx_p) { 
					ctx_p->addMOPointerToReleaseVector(if_mo_p);
				}
			}
			pResults->clear();
			delete pResults;

		} 

        /* When AG mode is enabled, the AG plugin expects the Fcoe Plugin to complete INTF     *
         * MO creation process. The AG plugin needs INTF MOs to validate/translate the         *
         * interface id/switch ports. Thus, the following asynchronous postboot callback       *
         * ensures that the Fcoe plugin completes the INTF completion in the same postboot     *
         * phase, enabling AG plugin to process its configruation in the later postboot phases *
         */
        pFcoeLocalObjectManager = dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager());
        if ((pFcoeLocalObjectManager->postbootStartedInAgMode) && pFcoeLocalObjectManager->pPostBootPhaseCtxAgMode) {
            WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase;
            FCOE_PLUG_DBG("In AG mode and returning Postboot context");
            pWaveAsynchronousContextForPostbootPhase = pFcoeLocalObjectManager->pPostBootPhaseCtxAgMode;

            //Clearing the flags before we declare postboot end
            pFcoeLocalObjectManager->postbootStartedInAgMode = false; 
            pFcoeLocalObjectManager->pPostBootPhaseCtxAgMode = NULL; 

            pWaveAsynchronousContextForPostbootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pWaveAsynchronousContextForPostbootPhase->callback ();
		}

		}
		return WAVE_MESSAGE_SUCCESS;
	}
         

    ResourceId
    FcoeLocalWorker::FcoeLocalCreateInterface (FcoeLinearSequencerContext *ctx_p)
    {
        string                          if_name;
        UI32                            mapped_id = 0;
        int32_t                         slot_id = -1;
        string                          bind_if_name;

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
           FCOE_PLUG_DBG_S(string("VCS is disabled. Not creating FCOE interface(s)"));
           return WAVE_MESSAGE_SUCCESS;
        }

        FcoeLocalFcoeIntfMessage *msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());

        mapped_id = DcmToolKit::getLocalMappedId();

        if_name = msg_p->getIfName();

        FCOE_PLUG_DBG_DEBUG_S(string("Searching FCOE IF with name=") + if_name);
        FcoeIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalIntfGetMOByName(if_name, ctx_p);
   
        if (if_mo_p != NULL) {
            FCOE_PLUG_DBG_DEBUG_S(string("Already Found MO with Name=fcoe ") + if_mo_p->getName() +
                                  string(" Updating the UportNum=") + msg_p->getFcUportNum());
                    if_mo_p->setFcUportNum(msg_p->getFcUportNum());
                    updateWaveManagedObject(if_mo_p);
                } else {
                    bind_if_name = msg_p->getBinding();
                    if (bind_if_name.empty() == false) {
                        slot_id = NsmUtils::getSlotId(bind_if_name);
                    }
                    
                    FCOE_PLUG_DBG_S(string("Creating FCOE IF name=") + if_name +
                                    string(" If-index=") + msg_p->getIfIndex() +
                                    string(" FcUportNum=") + msg_p->getFcUportNum() +
                                    string(" BindType=") + msg_p->getBindingType() +
                                    string(" BindName=") + bind_if_name +
                                    string(" Slot=") + slot_id);

                    if_mo_p = new FcoeIntfLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()),
                                                             if_name,
                                                             msg_p->getIfIndex(), 
                                                             msg_p->getFcUportNum(),
                                                             mapped_id, msg_p->getVnNum(), msg_p->getPortNum(),
                                                             FCOE_INTF_STATUS_DEFAULT,
                                                             FCOE_INTF_DEFAULT_MODE,
                                                             msg_p->getBinding(),
                                                             msg_p->getBindingType());
                    FCOE_PLUG_ASSERT_PTR(if_mo_p);

                    if_mo_p->associateWithSlot(slot_id);
                }

        // Add this for deletion later.
        ctx_p->addMOPointerToReleaseVector(if_mo_p);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalHandleInterfaceStatusUpdate (FcoeLinearSequencerContext *ctx_p)
    {
        string                          if_name;

        FcoeLocalFcoeIntfMessage *msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());

        if_name = msg_p->getIfName();
        FCOE_PLUG_DBG_DEBUG_S(string("Searching FCOE IF with name=") + if_name);
        FcoeIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalIntfGetMOByName(if_name, ctx_p);
        FCOE_PLUG_DBG_S(string("Rcvd UPD status for FCoE-IF=") + if_name +
                                string(" Status =") + msg_p->getStatus());
        if (if_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Interface not found in the DB");
            return FCOE_IF_ERR_NOT_FOUND_IN_DB;
        }

        FCOE_PLUG_DBG_DEBUG_S(string("Found MO with Name=fcoe ") + if_mo_p->getName());
        FCOE_PLUG_DBG_DEBUG_S(string("Updating Status=")+msg_p->getStatus());
        if_mo_p->setStatus(msg_p->getStatus()); 

        updateWaveManagedObject(if_mo_p);

        // Add this for deletion later.
        ctx_p->addMOPointerToReleaseVector(if_mo_p);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalRemoveInterfaceBinding (FcoeLinearSequencerContext *ctx_p)
    {
        string      if_name;
        UI64        ifindex = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        FcoeLocalFcoeIntfMessage *msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());

        if_name = msg_p->getIfName();
        FCOE_PLUG_DBG_DEBUG_S(string("Searching FCOE IF with name=") + if_name);
        FcoeIntfLocalManagedObject *if_mo_p = FcoeLocalWorker::FcoeLocalIntfGetMOByName(if_name, ctx_p);
        if (if_mo_p == NULL) {
            FCOE_PLUG_DBG_ERR("Interface not found in the DB");
            return FCOE_IF_ERR_NOT_FOUND_IN_DB;
        }

        FCOE_PLUG_DBG_DEBUG_S(string("Removing Binding info for FCOE ") + if_mo_p->getName());
        if_mo_p->setBindingType(msg_p->getBindingType());
        if_mo_p->setBinding(msg_p->getBinding());
        if_mo_p->setIfindex(ifindex);

        updateWaveManagedObject(if_mo_p);

        // Add this for deletion later.
        ctx_p->addMOPointerToReleaseVector(if_mo_p);

        return status;
    }

    ResourceId
    FcoeLocalWorker::FcoeLocalUpdateInterfaceBinding (FcoeLinearSequencerContext *ctx_p)
    {
	    FcoeLocalFcoeIntfMessage *msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());

	    return FcoeLocalUpdateInterfaceBindingCommon(msg_p->getIfName(), NULL,
	        msg_p->getBindingType(), msg_p->getBinding(), ctx_p);
	}

    ResourceId
    FcoeLocalWorker::FcoeLocalUpdateInterfaceBindingCommon (
	    string if_name, FcoeIntfLocalManagedObject *fcoe_if_p,
	    UI32 binding_type, string binding, FcoeLinearSequencerContext *ctx_p)
    {
        UI64        ifindex;
        PhyIntfLocalManagedObject   *phy_if_mo_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 bind_type = 0;
        FcoeIntfLocalManagedObject *if_mo_p;

	    if (fcoe_if_p) {
	        // if valid pointer is passed in, no reason to search the db
	        // using if_name
            FCOE_PLUG_DBG_DEBUG_S(string("Using FCOE IF with name=") + if_name);
            if_mo_p = fcoe_if_p;
	    } else { 
            FCOE_PLUG_DBG_DEBUG_S(string("Searching FCOE IF with name=") + if_name);
            if_mo_p = FcoeLocalWorker::FcoeLocalIntfGetMOByName(if_name, ctx_p);
            if (if_mo_p == NULL) {
                FCOE_PLUG_DBG_ERR("Interface not found in the DB");
                return FCOE_IF_ERR_NOT_FOUND_IN_DB;
            }
	    }

        FCOE_PLUG_DBG_DEBUG_S(string("Found MO with Name=fcoe ") + if_mo_p->getName());
        FCOE_PLUG_DBG_DEBUG_S(string("Updating Binding Type=")+ binding_type +
                              string(" Value=") + binding);

        if (binding_type) {
            if_mo_p->setBindingType(binding_type);
            bind_type = binding_type;
        } else {
            bind_type = if_mo_p->getBindingType();
        }
        if_mo_p->setBinding(binding);

        if (bind_type == FCOE_INTERFACE_BIND_VAL_TYPE_TE ||
            bind_type == FCOE_INTERFACE_BIND_VAL_TYPE_FO) {
            string bind_if_name = binding;
            phy_if_mo_p = FcoeLocalWorker::GetPhyIntfMoById(bind_if_name);
            if (phy_if_mo_p == NULL) {
                FCOE_PLUG_DBG_ERR_S(string("PHY Interface=") + bind_if_name +
                                    string("not found in the DB"));
                status = FCOE_IF_ERR_ETH_PORT_NOT_FOUND;
            } else {
                ifindex = phy_if_mo_p->getIfIndex();
                FCOE_PLUG_DBG_S(string("PHY Interface Ifindex=") + phy_if_mo_p->getIfIndex());
                if_mo_p->setIfindex(ifindex);
            }

	        if (ctx_p) {
            ctx_p->addMOPointerToReleaseVector(phy_if_mo_p);
        }
        }

        if_mo_p->setIfindex(ifindex);

        updateWaveManagedObject(if_mo_p);
        //ctx_p->addMOPointerToReleaseVector(if_mo_p);

        return status;
    }


    void
    FcoeLocalWorker::FcoeLocalIntfHandleConfig(FcoeLinearSequencerContext *ctx_p)
    {
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        FcoeLocalFcoeIntfMessage *msg_p = dynamic_cast<FcoeLocalFcoeIntfMessage *> (ctx_p->getPPrismMessage ());

        FCOE_PLUG_DBG_DEBUG_S(string("Rcvd Mesg Oper=") + msg_p->getOper_bmp());

        switch (msg_p->getOper_bmp()) {
            case FCOE_INTF_OPER_CREATE:
                status = FcoeLocalWorker::FcoeLocalCreateInterface(ctx_p);
                break;

            case FCOE_INTF_OPER_REMOVE:
                break;

            case FCOE_INTF_OPER_UPD_STATUS:
                status = FcoeLocalWorker::FcoeLocalHandleInterfaceStatusUpdate(ctx_p);
                break;

            case FCOE_INTF_OPER_CREATE_BULK:
                status = FcoeLocalWorker::FcoeLocalCreateInterfaceBulk(ctx_p);
                break;
                
            case FCOE_INTF_OPER_DELETE_BULK:
                status = FcoeLocalWorker::FcoeLocalDeleteInterfaceBulk(ctx_p);
                break;

            case FCOE_INTF_OPER_UPD_BIND:
                status = FcoeLocalWorker::FcoeLocalUpdateInterfaceBinding(ctx_p);
                break;

            case FCOE_INTF_OPER_REM_BIND:
                status = FcoeLocalWorker::FcoeLocalRemoveInterfaceBinding(ctx_p);
                break;

            default:
                FCOE_PLUG_DBG_S(string("Unknown operation code=")+msg_p->getOper_bmp());
                FCOE_PLUG_ASSERT(0);
                break;
        }

        ctx_p->executeNextStep(status);
        return;
    }

    void  FcoeLocalWorker::FcoeLocalFcoeIntfMessageHandler( FcoeLocalFcoeIntfMessage *pFcoeLocalFcoeIntfMessage)
    {
        // Validations and Queries should go here

        FCOE_PLUG_DBG_DEBUG_S("Entered INTF Msg Handler");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalIntfSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalIntfHandleConfig),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeLocalWorker::FcoeLocalShowSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeShowMessage *msg_p = dynamic_cast<FcoeLocalFcoeShowMessage *> (ctx_p->getPPrismMessage ());
        void *data_p = NULL;
        UI32 size = 0;
        ResourceId status;

        FcoeClientShowMessage *cl_msg_p = new FcoeClientShowMessage(msg_p->getCmdCode(),
                                                                    msg_p->getVlan(),
                                                                    msg_p->getVfid(),
                                                                    msg_p->getFcUport(),
                                                                    msg_p->getFabMapName(),
                                                                    msg_p->getMapName(),
                                                                    msg_p->getPhyIfName(),
                                                                    msg_p->getStatsEnable());

        if (msg_p->getCmdCode() == FCOE_SHOW_CMD_CODE_ETHERNET_INTF) {
            FCOE_SEND_SYNC_TO_DAEMON(NSM_DCM_CLIENT_NAME, cl_msg_p, status,
                                     FCOE_DONT_DEL_CLIENT_MSG);
            FCOE_PLUG_DBG_S(string("Message sent to NSM Daemon for Command Code=") + 
                            msg_p->getCmdCode());

        } else {
            FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status,
                                     FCOE_DONT_DEL_CLIENT_MSG);
            FCOE_PLUG_DBG_S(string("Message sent to FCOE Daemon for Command Code=") + 
                            msg_p->getCmdCode());
        }

        if (status == WAVE_MESSAGE_SUCCESS) {
            msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
            cl_msg_p->getOutputBufferRef(msg_p->getCmdCode(), size, data_p);

            FCOE_PLUG_DBG_S(string("Rcvd Response Buffer =") + 
                            reinterpret_cast<UI64>(data_p) + 
                            string("size =") + size);

            if (data_p) {
                msg_p->addBuffer(DcmToolKit::getLocalMappedId(), 
                             size, (unsigned char *)data_p, false);
            }
        }

        delete cl_msg_p;

        ctx_p->executeNextStep(status);
        
        return;
    }

    void FcoeLocalWorker::FcoeLocalFcoeShowMessageHandler (FcoeLocalFcoeShowMessage *pFcoeLocalFcoeShowMessage)
    {
        FCOE_PLUG_DBG_S("Show MSG Handler");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalShowSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };
        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeLocalWorker::FcoeLocalInternalSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeInternalMessage *msg_p = dynamic_cast<FcoeLocalFcoeInternalMessage *> (ctx_p->getPPrismMessage ());
        void *data_p = NULL;
        UI32 size = 0;
        ResourceId status;

        FcoeClientInternalMessage *cl_msg_p = new FcoeClientInternalMessage(msg_p->getCmdCode(),
                                                                    msg_p->getVlan(),
                                                                    msg_p->getVfid(),
                                                                    msg_p->getFcUport(),
                                                                    msg_p->getIfName(),
                                                                    msg_p->getIfindex());

        
        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status,
                                 FCOE_DONT_DEL_CLIENT_MSG);
        FCOE_PLUG_DBG_S(string("Message sent to FCOE Daemon for Command Code=") +
                        msg_p->getCmdCode());

        if (status == WAVE_MESSAGE_SUCCESS) {
            msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
            cl_msg_p->getOutputBufferRef(msg_p->getCmdCode(), size, data_p);

            FCOE_PLUG_DBG_S(string("Rcvd Response Buffer =") +
                            reinterpret_cast<UI64>(data_p) +
                            string("size =") + size);

            if (data_p) {
                msg_p->addBuffer(FrameworkToolKit::getThisLocationId(),
                             size, (unsigned char *)data_p, false);
            }
        }

        delete cl_msg_p;

        ctx_p->executeNextStep(status);

        return;
    }

    void FcoeLocalWorker::FcoeLocalFcoeInternalMessageHandler (FcoeLocalFcoeInternalMessage *pFcoeLocalFcoeInternalMessage)
    {
        FCOE_PLUG_DBG_S("Internal MSG Handler");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalInternalSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };
        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeInternalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeLocalWorker::FcoeLocalClearSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeClearMessage   *msg_p = dynamic_cast<FcoeLocalFcoeClearMessage *> (ctx_p->getPPrismMessage ());
        ResourceId                  status;
        
        FcoeClientClearMessage *cl_msg_p = new FcoeClientClearMessage(msg_p->getCmdCode(),
                                                                     msg_p->getVlan(),
                                                                     msg_p->getVfid(),
                                                                     msg_p->getFcUport(),
                                                                     msg_p->getIfName(),
                                                                     msg_p->getWWN(),
                                                                     msg_p->getIfindex(),
                                                                     msg_p->getPo());

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

        ctx_p->executeNextStep(status);

        return;
    }

    void FcoeLocalWorker::FcoeLocalFcoeClearMessageHandler (FcoeLocalFcoeClearMessage *pFcoeLocalFcoeShowMessage)
    {
        FCOE_PLUG_DBG_S("Show MSG Handler");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalClearSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeShowMessage, this, sequencerSteps, 
                                                                                                  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    ResourceId FcoeLocalWorker::FcoeLocalFcoePluginStatusSendToDaemon (uint32_t plugin_status)
    {
        ResourceId status;
		
        FCOE_PLUG_DBG("Sending Fcoe Plugin status to daemon");

        FcoeClientPluginControlMessage *fcoe_plugin_ctrl_msg_p = new FcoeClientPluginControlMessage();
        if (fcoe_plugin_ctrl_msg_p) {

            fcoe_plugin_ctrl_msg_p->setOper_bmp(FCOE_PLUGIN_CMD_CODE_SHUT_NOTIFY);
            fcoe_plugin_ctrl_msg_p->setPlugin_status(plugin_status);
            status = sendSynchronouslyToWaveClient ("fcoed", fcoe_plugin_ctrl_msg_p);

            if ((status != WAVE_MESSAGE_SUCCESS) ||
                (fcoe_plugin_ctrl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)) {
                FCOE_PLUG_DBG("Error: Failed to send Fcoe Plugin status to Fcoe daemon");
            }

            delete fcoe_plugin_ctrl_msg_p;
            return status;
        }
        return WAVE_MESSAGE_ERROR;
    }

    void FcoeLocalWorker::NoShutdown (FcoeLinearSequencerContext *ctx_p)
    {
        FCOE_PLUG_DBG("Fcoe Plugin status is Up");
		ResourceId status;

    	status = FcoeLocalWorker::FcoeLocalFcoePluginStatusSendToDaemon(FCOE_PLUGIN_NO_SHUT);

		ctx_p->executeNextStep(status);

		return;
    }

    void FcoeLocalWorker::backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        FCOE_PLUG_DBG("Entering backendSyncUp phase for LOCAL FCOE Plug, syncing up MOs with fcoe backend");

        PrismLinearSequencerStep sequencerSteps[] =
        {   
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalBackendSyncUpETInterfaceStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalBackendSyncUpInterfaceStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pPrismAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
	}

    void FcoeLocalWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        FCOE_PLUG_DBG("Entering install phase for LOCAL FCOE Plug, creating default INTF MOs");
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        /*if (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT) {
            FCOE_PLUG_DBG("Not a FIRST_TIME_BOOT, Returning");
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }*/
            FcoeEnodeLocalManagedObject      *enode_mo_p = FcoeLocalEnodeGetMOByName(NULL);
            if (enode_mo_p == NULL) {
                FCOE_PLUG_DBG("FcoeEnodeLocalManagedObject MO is not present. Creating ...");
                startTransaction();
                enode_mo_p = new FcoeEnodeLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()), FCOE_MO_DEFAULT_NAME, FCOE_DEFAULT_FCOE_ENODES_WITH_LICENSE, false);
                assert(enode_mo_p);
                status = commitTransaction ();
                if (status == FRAMEWORK_SUCCESS) {
                    status = WAVE_MESSAGE_SUCCESS;
                } else {
                    FCOE_PLUG_ASSERT(0);
                }
             }

			delete enode_mo_p;

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalInstallInterfaceStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalInstallETInterfaceStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps,
                                                                                                  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void FcoeLocalWorker::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
    {   
        ResourceId status;

        FCOE_PLUG_DBG("Entering shutdown phase for LOCAL FCOE Plug");

        status = FcoeLocalWorker::FcoeLocalFcoePluginStatusSendToDaemon(FCOE_PLUGIN_SHUT);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // FIXME : Need to ahndle the erro case.  For now we are blindly propagating success.
        }

        pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForShutDownPhases->callback ();
    }

    void 
    FcoeLocalWorker::FcoeLocalInstallETInterfaceStep (FcoeLinearSequencerContext *ctx_p)
    {

        FcoeClientETIntfMessage           *msg_p = NULL;
        UI64                            i = 0;
        FcoeETIntfLocalManagedObject    *if_mo_p;
        string                          if_name;
        void                            *data_p;
        UI32                            size;
        fcoe_intf_config_msg_t         *if_list_p = NULL;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        int32_t                         slot_id = 0;

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
           FCOE_PLUG_DBG("VCS is Disabled");
           ctx_p->executeNextStep(status);
           return;
        }

        msg_p = new FcoeClientETIntfMessage();
        msg_p->setOper_bmp(FCOE_ET_INTF_OPER_GET_IF_LIST);

        status = sendSynchronouslyToWaveClient("fcoed", msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR("Failed to get ET interface list from FCOE Daemon");
            FCOE_PLUG_DBG_S(string("ERROR=") + status);
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        data_p = msg_p->findBuffer(msg_p->getOper_bmp(), size);
        if (data_p == NULL) {
            FCOE_PLUG_DBG_ERR("Response buffer is NULL");
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        if_list_p = reinterpret_cast<fcoe_intf_config_msg_t *> (data_p);

        FCOE_PLUG_DBG_S(string("Recieved IF LIST Num=") + if_list_p->num_entries)
        
        for (i = 0; i < if_list_p->num_entries; i++) {
            if_name.assign((char *)if_list_p->list[i].if_name);
            FCOE_PLUG_DBG_S(string("Creating ET interface ") + if_name);

            if_mo_p = FcoeLocalWorker::FcoeLocalETIntfGetMOByName(if_name);
            if (if_mo_p != NULL) {
                FCOE_PLUG_DBG_S(string("ET interface already present, not creating...."));

                delete if_mo_p;
                continue;
            }
         

            if_mo_p = new FcoeETIntfLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()));
            assert(if_mo_p);

            if_mo_p->setName(if_name);
            if_mo_p->setIfindex(if_list_p->list[i].if_index);
            if_mo_p->setFcUportNum(if_list_p->list[i].fc_port_num);
            if_mo_p->setStatus(if_list_p->list[i].status);
            if_mo_p->setTrunkStatus(if_list_p->list[i].trunk_status);
            if_mo_p->setRbId(DcmToolKit::getLocalMappedId());
            if_mo_p->setBindType(if_list_p->list[i].bind_type);
			if_mo_p->setAsBreakoutPort(if_list_p->list[i].breakout);

            slot_id = NsmUtils::getSlotId(if_name);
            if_mo_p->associateWithSlot(slot_id);

            ctx_p->addMOPointerToReleaseVector(if_mo_p);
        }

        delete msg_p;
        
        ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }

    void 
    FcoeLocalWorker::FcoeLocalInstallInterfaceStep (FcoeLinearSequencerContext *ctx_p)
    {

        FcoeClientIntfMessage           *msg_p = NULL;
        fcoe_intf_config_msg_t          *data_p;
        UI32                            size;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        FCOE_PLUG_DBG("Entering install phase for LOCAL FCOE Plug, creating default INTF MOs");

        /*if (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT) {
            FCOE_PLUG_DBG("Not a FIRST_TIME_BOOT, Returning");
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }*/

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
           FCOE_PLUG_DBG("VCS is Disabled");
           ctx_p->executeNextStep(status);
           return;
        }

        msg_p = new FcoeClientIntfMessage();
        msg_p->setOper_bmp(FCOE_INTF_OPER_GET_IF_LIST);

        status = sendSynchronouslyToWaveClient("fcoed", msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR("Failed to get interface list from FCOE Daemon");
            FCOE_PLUG_DBG_S(string("ERROR=") + status);
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
        if (data_p == NULL) {
            FCOE_PLUG_DBG_ERR("Response buffer is NULL");
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        FCOE_PLUG_ASSERT(size == data_p->size);
        status = FcoeLocalWorker::FcoeLocalCreateInterfaceBulk(ctx_p, data_p);

        delete msg_p;

        ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
  }

    void  FcoeLocalWorker::FcoeLocalETIntfMessageHandler(FcoeLocalETIntfMessage *pFcoeLocalETIntfMessage)
    {
        // Validations and Queries should go here

        FCOE_PLUG_DBG_DEBUG_S("Entered ET INTF Msg Handler");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalETIntfHandleConfig),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerCommitTransactionStep),

            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalETIntfSendToDaemon),

            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep),
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalETIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }

    void
    FcoeLocalWorker::FcoeLocalBackendSyncUpETInterfaceStep (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeClientETIntfMessage         *msg_p = NULL;
        fcoe_intf_config_msg_t          *data_p;
        UI32                            size;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
           FCOE_PLUG_DBG("VCS is Disabled");
           ctx_p->executeNextStep(status);
           return;
        }

        FCOE_PLUG_DBG("Entering ...");

        msg_p = new FcoeClientETIntfMessage();
        msg_p->setOper_bmp(FCOE_ET_INTF_OPER_GET_IF_LIST);

        status = sendSynchronouslyToWaveClient("fcoed", msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR("Failed to get ET interface list from FCOE Daemon");
            FCOE_PLUG_DBG_S(string("ERROR=") + status);
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
        if (data_p == NULL) {
            FCOE_PLUG_DBG_ERR("Response buffer is NULL");
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        FCOE_PLUG_ASSERT(size == data_p->size);
        status = FcoeLocalWorker::FcoeLocalCreateETInterfaceBulk(ctx_p, data_p);

        delete msg_p;
        ctx_p->executeNextStep(status);
        return;

    }

    void
    FcoeLocalWorker::FcoeLocalBackendSyncUpInterfaceStep (FcoeLinearSequencerContext *ctx_p)
    {

        FcoeClientIntfMessage           *msg_p = NULL;
        fcoe_intf_config_msg_t          *data_p;
        UI32                            size;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        if (!FCOE_PLUGIN_IS_VCS_ENABLED()) {
           FCOE_PLUG_DBG("VCS is Disabled");
           ctx_p->executeNextStep(status);
           return;
        }

        FCOE_PLUG_DBG("Entering ...");

        msg_p = new FcoeClientIntfMessage();
        msg_p->setOper_bmp(FCOE_INTF_OPER_GET_IF_LIST);

        status = sendSynchronouslyToWaveClient("fcoed", msg_p);
        if (status != WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG_ERR("Failed to get interface list from FCOE Daemon");
            FCOE_PLUG_DBG_S(string("ERROR=") + status);
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        data_p = (fcoe_intf_config_msg_t *)msg_p->findBuffer(msg_p->getOper_bmp(), size);
        if (data_p == NULL) {
            FCOE_PLUG_DBG_ERR("Response buffer is NULL");
            ctx_p->executeNextStep(status);
            delete msg_p;
            return;
        }

        FCOE_PLUG_ASSERT(size == data_p->size);
        status = FcoeLocalWorker::FcoeLocalCreateInterfaceBulk(ctx_p, data_p);

        delete msg_p;
        ctx_p->executeNextStep(status);
        return;
  }

    void
    FcoeLocalWorker::FcoeLocalFcoeFsbSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalFcoeFsbMessage *msg_p = dynamic_cast<FcoeLocalFcoeFsbMessage *> (ctx_p->getPPrismMessage ());
        FcoeClientFsbMessage *fcoe_cl_msg_p = NULL;
		LldpFcoeClientFsbMessage *lldp_cl_msg_p = NULL;

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        FCOE_PLUG_DBG_S(string("RCVD Local FSB Msg: enable=") + msg_p->getMode());

        fcoe_cl_msg_p = new FcoeClientFsbMessage(msg_p->getMode());
		lldp_cl_msg_p = new LldpFcoeClientFsbMessage(msg_p->getMode());

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, fcoe_cl_msg_p, status, FCOE_DEL_CLIENT_MSG);
		if(!FCOE_PLUGIN_IS_VCS_ENABLED())
		{
			FCOE_SEND_SYNC_TO_DAEMON(LLDP_DCM_CLIENT_NAME, lldp_cl_msg_p, status, FCOE_DEL_CLIENT_MSG);
		}
        
		ctx_p->executeNextStep(status);

        return;
    }

    void  FcoeLocalWorker::FcoeLocalFcoeFsbMessageHandler( FcoeLocalFcoeFsbMessage *pFcoeLocalFcoeFsbMessage)
    {   

        PrismLinearSequencerStep sequencerSteps[] =
        {  
            // Programming Protocol Daemons goes here 
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalFcoeFsbSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalFcoeFsbMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
    }


    void  
    FcoeLocalWorker::FcoeLocalEnodeMessageHandler( FcoeLocalEnodeMessage *pFcoeLocalEnodeMessage)
    {
        FCOE_PLUG_DBG("FcoeLocalEnodeMessageHandler start");

        PrismLinearSequencerStep sequencerSteps[] =
        {

            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalFcoeEnodeSendToDaemon),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLocalEnodesConfigUpdate),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FcoeLocalWorker::FcoeLinearSequencerFailedStep)
        };

        FcoeLinearSequencerContext *pFcoeLinearSequencerContext = new FcoeLinearSequencerContext (pFcoeLocalEnodeMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pFcoeLinearSequencerContext->start ();
   }


    void
    FcoeLocalWorker::FcoeLocalFcoeEnodeSendToDaemon (FcoeLinearSequencerContext *ctx_p)
    {
        FcoeLocalEnodeMessage *msg_p = dynamic_cast<FcoeLocalEnodeMessage *> (ctx_p->getPPrismMessage ());
        FcoeClientFabricMapMessage *cl_msg_p;
        UI32        vlan_id = 0;
        UI32        priority = 0;
        UI32        interval = 0;
        UI32        vfid = 0;
        bool        timeout = 0;
        string      fcmap_str("00:00:00");
        string      fabric_map_name("default");
        UI32        oper_bmp = 0;
        UI32        mtu = 0;
        ResourceId status;
        UI32 fcoe_enodes = 0;

        if (msg_p == NULL) {
           ctx_p->executeNextStep(FCOE_ENODE_MSG_FAILED);
           return;
        }

        fcoe_enodes =  msg_p->getFcoeEnodes();
        oper_bmp = msg_p->getFcoeEnodesOperBmp(); 
        trace(TRACE_LEVEL_INFO, string("FcoeLocalFcoeEnodeSendToDaemon fcoe_enodes: ") + fcoe_enodes);
        cl_msg_p = new FcoeClientFabricMapMessage(oper_bmp,
                                               fabric_map_name,
                                               vlan_id,
                                               vfid,
                                               priority,
                                               fcmap_str,
                                               interval,
                                               timeout,
                                               mtu,
                                              fcoe_enodes);

        FCOE_SEND_SYNC_TO_DAEMON(FCOE_DCM_CLIENT_NAME, cl_msg_p, status, FCOE_DEL_CLIENT_MSG);

        if (status == WAVE_MESSAGE_SUCCESS) {
            FCOE_PLUG_DBG("FCoE Enodes message send to daemon successfully");
        } else {
            FCOE_PLUG_DBG_ERR_S(string("FCoE Enode message send to daemon Failed") +
                 FrameworkToolKit::localize(status));
        }

        ctx_p->executeNextStep(status);

        return;
    }

    void
    FcoeLocalWorker::FcoeLocalEnodesConfigUpdate(FcoeLinearSequencerContext *ctx_p)
    {
        UI32 fcoe_enodes = 0;
        FcoeEnodeLocalManagedObject *enode_mo_p = NULL;
        FcoeLocalEnodeMessage *msg_p = dynamic_cast<FcoeLocalEnodeMessage *> (ctx_p->getPPrismMessage ());
        FCOE_PLUG_DBG("FcoeEnodeLocalManagedObject Mo update start... ");

        if (msg_p == NULL) {
           FCOE_PLUG_DBG_ERR_S(string("FCoE Enode message processing failed") +
                 FrameworkToolKit::localize(FCOE_ENODE_MSG_FAILED));
           ctx_p->executeNextStep(FCOE_ENODE_MSG_FAILED);
           return;
        }

        fcoe_enodes =  msg_p->getFcoeEnodes();
        enode_mo_p = FcoeLocalEnodeGetMOByName(ctx_p);

        if(enode_mo_p == NULL) {
           FCOE_PLUG_DBG_ERR_S(string("FCoE Enode Mo not found") +
                 FrameworkToolKit::localize(FCOE_ENODE_MO_NOT_FOUND));
            ctx_p->executeNextStep(FCOE_ENODE_MO_NOT_FOUND);
            return;
        }

        enode_mo_p->setFcoeEnodes(fcoe_enodes);

        updateWaveManagedObject(enode_mo_p);
        FCOE_PLUG_DBG("FcoeEnodeLocalManagedObject updated successfully... ");
        ctx_p->addMOPointerToReleaseVector(enode_mo_p);

        ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
       return;
    }

    FcoeEnodeLocalManagedObject *
    FcoeLocalWorker::FcoeLocalEnodeGetMOByName(FcoeLinearSequencerContext *ctx_p)
    {
        FcoeEnodeLocalManagedObject *enode_mo_p = NULL;
        UI32 loc_id  = FrameworkToolKit::getThisLocationId();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeEnodeLocalManagedObject::getClassName());
        
        vector<WaveManagedObject *> *pResults =querySynchronouslyLocalManagedObjectsForLocationId(loc_id, &syncQueryCtxt); 

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            FCOE_PLUG_DBG_DEBUG_S(string("FcoeLocalEnodeGetMOByName Query returned ") + numberOfResults + string(" results... returning"));
            if (ctx_p) {
                ctx_p->addMOVectorToReleaseVector(pResults);
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
            return NULL;
        }

        enode_mo_p = dynamic_cast<FcoeEnodeLocalManagedObject *> ((*pResults)[0]);

        pResults->clear();
        delete pResults;

        return enode_mo_p;
    }

    void 
    FcoeLocalWorker::upgrade (WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase) 
	{   
		ResourceId                      status = WAVE_MESSAGE_SUCCESS;
		UI32                            fcoe_enodes = 0;
		FcoeEnodeLocalManagedObject      *enode_mo_p = NULL;

		FCOE_PLUG_DBG("upgrade :: Entering FCoE upgrade  phase");
		enode_mo_p = FcoeLocalEnodeGetMOByName(NULL);
		if (enode_mo_p == NULL) {
			FCOE_PLUG_DBG("MO is not present. Creating the MO ....");
			startTransaction();
			enode_mo_p = new FcoeEnodeLocalManagedObject(dynamic_cast<FcoeLocalObjectManager *>(getPWaveObjectManager()), FCOE_MO_DEFAULT_NAME, fcoe_enodes, true);
			assert(enode_mo_p);
			status = commitTransaction ();
			if (status == FRAMEWORK_SUCCESS) {
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				FCOE_PLUG_ASSERT(0);
			}
			FCOE_PLUG_DBG("upgrade :: FcoeEnodeLocalManagedObject MO created successfully");

		}

		delete enode_mo_p;

		pWaveAsynchronousContextForUpgradePhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForUpgradePhase->callback ();
	} 

}
