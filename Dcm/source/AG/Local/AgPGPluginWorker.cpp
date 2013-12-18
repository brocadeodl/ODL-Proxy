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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ramachandran Anantharaman                                    *
 **************************************************************************/


#include <algorithm>
#include <vector>
#include <sstream>
#include <string>
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"



#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgPGPluginWorker.h"
#include "AG/Local/AgLocalConfigMessage.h"
#include "AG/Local/AgPGCreateMessage.h"
#include "AG/Local/AgPGModesMessage.h"
#include "AG/Local/AgPGAddMessage.h"
#include "AG/Local/AgPGDelMessage.h"
#include "AG/Local/AgPGRenameMessage.h"
#include "AG/Local/AgPGRemoveMessage.h"
#include "AG/Local/AgLocalManagedObject.h"
#include "AG/Local/AgNPortMapLocalManagedObject.h"
#include "AG/Local/AgPortGroupLocalManagedObject.h"
#include "AG/Local/AgLocalTypes.h"
#include "AG/Common/AgPluginTypes.h"
#include "AG/Common/AgUtils.h"
#include "AG/Common/AgUtilsLocal.h"

#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "ClientInterface/AG/AgClientMappingMessage.h"
#include "ClientInterface/AG/AgClientPGCreateMessage.h"
#include "ClientInterface/AG/AgClientPGAddMessage.h"
#include "ClientInterface/AG/AgClientPGDelMessage.h"
#include "ClientInterface/AG/AgClientPGModesMessage.h"
#include "ClientInterface/AG/AgClientPGRenameMessage.h"
#include "ClientInterface/AG/AgClientPGRemoveMessage.h"


#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"


#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"

#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "sys/fabos/fabobj.h"

using namespace WaveNs;

namespace DcmNs
{

    AgPGPluginWorker::AgPGPluginWorker ( AgLocalObjectManager *pAgLocalObjectManager)
          : WaveWorker  (pAgLocalObjectManager)
    {
        trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::AgPGPluginWorker Entering...");

        addOperationMap (AG_PGCREATE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgPGPluginWorker::AgPGCreateMessageHandler));
        addOperationMap (AG_PGADD_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgPGPluginWorker::AgPGAddMessageHandler));
        addOperationMap (AG_PGDEL_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgPGPluginWorker::AgPGDelMessageHandler));
        addOperationMap (AG_PGMODES_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgPGPluginWorker::AgPGModesMessageHandler));
        addOperationMap (AG_PGRENAME_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgPGPluginWorker::AgPGRenameMessageHandler));
        addOperationMap (AG_PGREMOVE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgPGPluginWorker::AgPGRemoveMessageHandler));

    }

    AgPGPluginWorker::~AgPGPluginWorker ()
    {
    }

    PrismMessage  *AgPGPluginWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case AG_PGCREATE_MESSAGE:
                pPrismMessage = new AgPGCreateMessage ();
                break;

            case AG_PGADD_MESSAGE:
                pPrismMessage = new AgPGAddMessage ();
                break;

            case AG_PGDEL_MESSAGE:
                pPrismMessage = new AgPGDelMessage ();
                break;

            case AG_PGMODES_MESSAGE:
                pPrismMessage = new AgPGModesMessage ();

            case AG_PGRENAME_MESSAGE:
                pPrismMessage = new AgPGRenameMessage ();
                break;

            case AG_PGREMOVE_MESSAGE:
                pPrismMessage = new AgPGRemoveMessage ();
                break;

            default :
                pPrismMessage = NULL;
          }

        return (pPrismMessage);
    }

    WaveManagedObject  *AgPGPluginWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
        return (pWaveManagedObject);
    }

    void  AgPGPluginWorker::AgPGModesMessageHandler( AgPGModesMessage *pAgPGModesMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::validatePGModesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updateBackendPGModesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updatePGModesMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPGModesMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

   }

  ResourceId AgPGPluginWorker::validatePGModesStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        trace(TRACE_LEVEL_INFO, string("Entering AgPGPluginWorker::validatePGModesStep!"));
        AgPGModesMessage *pAgPGModesMessage =
            dynamic_cast<AgPGModesMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        vector<string> pgModes = pAgPGModesMessage->getModes();
        UI32 pgid = pAgPGModesMessage->getpgId();
        ResourceId return_code = WAVE_MESSAGE_SUCCESS;

        if(pAgPGModesMessage->getAction() == AG_PGADDMODES) {
            return_code = AgUtilsLocal::validate_pgaddmodes(pgid, pgModes);
        } else if(pAgPGModesMessage->getAction() == AG_PGDELMODES) {
            return_code = AgUtilsLocal::validate_pgdelmodes(pgid, pgModes);
        }

        if(return_code != AG_VALIDATION_SUCCESS) {
            pAgPGModesMessage->setCompletionStatus(return_code);
            return return_code;
        }

        pAgPGModesMessage->setCompletionStatus(return_code);

        return WAVE_MESSAGE_SUCCESS;
  }

  ResourceId AgPGPluginWorker::updatePGModesMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        string mode;
        vector<string> existing_modes;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<string> modes;
        vector<string>::iterator iter, iter1;
        AgPGModesMessage *pAgPGModesMessage =
            dynamic_cast<AgPGModesMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        modes = pAgPGModesMessage->getModes();
        UI32 pgid = pAgPGModesMessage->getpgId();
        vector<string> temp_mode;
        LocationId loc_id;

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid, "agPgId"));
        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);
            existing_modes = pAgPortGroupLocalManagedObject->getPgFlags();
        }

        if(pAgPGModesMessage->getAction() == AG_PGADDMODES) {
            trace(TRACE_LEVEL_DEBUG, string("In AgPGPluginWorker::updatePGModesMOStep : In AG_PGADDMODES!\n"));

            for(iter = modes.begin(); iter != modes.end(); iter++) {
                 mode = *iter;
                 existing_modes.push_back(mode);
            }

        } else if(pAgPGModesMessage->getAction() == AG_PGDELMODES) {
            trace(TRACE_LEVEL_DEBUG, string("In AgPGPluginWorker::updatePGModesMOStep : In AG_PGDELMODES!\n"));

            for(iter = modes.begin(); iter != modes.end(); iter++) {
                iter1 = find(existing_modes.begin(), existing_modes.end(), *iter);
                if(iter1 != existing_modes.end()) {
                    existing_modes.erase(iter1);
                }
            }

        }

        updateWaveManagedObject(pAgPortGroupLocalManagedObject);
        pAgPortGroupLocalManagedObject->setPgFlags(existing_modes);

        if(pResults){
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;

        return status;
}

    ResourceId AgPGPluginWorker::updateBackendPGModesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        vector<string> modes;
        vector<string>::iterator iter;
        AgPGModesMessage *pAgPGModesMessage =
            dynamic_cast<AgPGModesMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        AgClientPGModesMessage *msg_p = new AgClientPGModesMessage();
        modes = pAgPGModesMessage->getModes();

        msg_p->setpgId(pAgPGModesMessage->getpgId());

        msg_p->setAction(pAgPGModesMessage->getAction());

        msg_p->setModes(pAgPGModesMessage->getModes());

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of PG modes is : ")+backend_ret);
            pAgPGModesMessage->setCompletionStatus (backend_ret);
            if(backend_ret == WAVE_MESSAGE_SUCCESS)
                ret =  WAVE_MESSAGE_SUCCESS;
        }  else {
            trace (TRACE_LEVEL_ERROR, string("Sendsync failed: ")+status);
        }


        if(msg_p)
            delete msg_p;


        return ret;

    }


    void  AgPGPluginWorker::AgPGRemoveMessageHandler( AgPGRemoveMessage *pAgPGRemoveMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::validatePGRemoveStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updateBackendPGRemoveStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updatePGMOForRemoveStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPGRemoveMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

   }

    ResourceId AgPGPluginWorker::validatePGRemoveStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        trace(TRACE_LEVEL_INFO, string("Entering AgPGPluginWorker::validatePGRemoveStep!"));
        ResourceId return_code = WAVE_MESSAGE_SUCCESS;
        AgPGRemoveMessage *pAgPGRemoveMessage =
            dynamic_cast<AgPGRemoveMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        int pgid = pAgPGRemoveMessage->getpgId();

        return_code = AgUtilsLocal::validate_pgremove(pgid);

        if(return_code != AG_VALIDATION_SUCCESS) {
            pAgPGRemoveMessage->setCompletionStatus(return_code);
            return return_code;
        }

        pAgPGRemoveMessage->setCompletionStatus(return_code);

        return WAVE_MESSAGE_SUCCESS;

    }
    
    ResourceId AgPGPluginWorker::updatePGMOForRemoveStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL,  *pAgPortGroupLocalManagedObject_other = NULL;
        UI32 pgid, pgid_new;     
        int mid =  AgUtils::getLocalMappedId(), portinteger, i;
        BitMap_t ports_bm = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        vector<string> nports_temp, ports;
        string interface_port;
        vector<string>::iterator iter;
        LocationId loc_id;

        trace (TRACE_LEVEL_DEBUG, "\n\nEntering AgPGPluginWorker::updatePGMOForRemoveStep!\n");

        AgPGRemoveMessage *pAgPGRemoveMessage =
            dynamic_cast<AgPGRemoveMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pgid = pAgPGRemoveMessage->getpgId();


        //update the n ports to pg0
        pgid_new = 0;
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid_new, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updateAgPGAddMOStep : Going to update the PG0 MO!\n");
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);

            BM_Clear(ports_bm, MAX_AG_PORT);

            // Mark bits for existing N ports
            nports_temp = pAgPortGroupLocalManagedObject->getPgNPortTopo();
            for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_SetBitN(portinteger, ports_bm);
                    } 
                } else {
                    if(ports_bm)
                        free(ports_bm);

                    if(pResults){
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                        delete pResults;
                    }

                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

            pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid, "agPgId"));

            loc_id = FrameworkToolKit::getThisLocationId();
            pSyncQueryCtx->setLocationIdFilter(loc_id);
            pResults = querySynchronously (pSyncQueryCtx);

            if (NULL != pResults && pResults->size()) {
                trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updateAgPGAddMOStep : Going to update the PG0 MO!\n");
                pAgPortGroupLocalManagedObject_other = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);

                // Mark bits for existing N ports
                nports_temp = pAgPortGroupLocalManagedObject_other->getPgNPortTopo();
                for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                    if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                        if(portinteger < MAX_AG_PORT) { 
                            BM_SetBitN(portinteger, ports_bm);
                        } 
                    } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                        return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                    }
                }

                //delete that row x of the MO
                delete pAgPortGroupLocalManagedObject_other;
            }
        }

        // Put the n ports in the MO in the interface format
        ports.clear();
        for(i = 0; i < MAX_AG_PORT; i++) {
            if ((BM_isBitNSet(i, ports_bm))) {
                if(AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, i, interface_port) != -1) {
                    ports.push_back(interface_port);
                } else {
                     if(ports_bm)
                         free(ports_bm);

                     if(pSyncQueryCtx)
                         delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }
        }

        //update pg 0 with the existing ports and the ports from the deleted PG 
        updateWaveManagedObject(pAgPortGroupLocalManagedObject);
        pAgPortGroupLocalManagedObject->setPgNPortTopo(ports);

        //cleanup
        if(ports_bm)
            free(ports_bm);

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;

        return status;
    }

    ResourceId AgPGPluginWorker::updateBackendPGRemoveStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        int backend_ret ;
        AgClientPGRemoveMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret =WAVE_MESSAGE_ERROR;
        AgPGRemoveMessage *pAgPGRemoveMessage =
            dynamic_cast<AgPGRemoveMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "\n\nEntering AgPGPluginWorker::updateBackendPGRemoveStep!\n");

        msg_p = new AgClientPGRemoveMessage();

        msg_p->setpgId(pAgPGRemoveMessage->getpgId());

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of PG remove is : ")+backend_ret);
            pAgPGRemoveMessage->setCompletionStatus (backend_ret);
            if(backend_ret == WAVE_MESSAGE_SUCCESS)
                ret =  WAVE_MESSAGE_SUCCESS;
        }  else {
            trace (TRACE_LEVEL_ERROR, string("Sendsync failed: ")+status);
        }



        if(msg_p) {
            delete msg_p;
        }


        return ret;

    }

    void  AgPGPluginWorker::AgPGRenameMessageHandler( AgPGRenameMessage *pAgPGRenameMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updateBackendPGRenameStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updatePGMOForRenameStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPGRenameMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

   }

    ResourceId AgPGPluginWorker::updatePGMOForRenameStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        string name;
        UI32 pgid;     
        LocationId loc_id;

        trace (TRACE_LEVEL_DEBUG, "\n\nEntering AgPGPluginWorker::updatePGMOForRenameStep!\n");

        AgPGRenameMessage *pAgPGRenameMessage =
            dynamic_cast<AgPGRenameMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pgid = pAgPGRenameMessage->getpgId();
        name = pAgPGRenameMessage->getName();

        // Retrieve row from MO
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updateAgPGRenameMOStep : Going to update the MO!\n");
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);

            updateWaveManagedObject(pAgPortGroupLocalManagedObject);
            pAgPortGroupLocalManagedObject->setPgName(name);
        }

        if(pResults){
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;

        return status;
    }

    ResourceId AgPGPluginWorker::updateBackendPGRenameStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        AgClientPGRenameMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret =WAVE_MESSAGE_ERROR;
        AgPGRenameMessage *pAgPGRenameMessage =
            dynamic_cast<AgPGRenameMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "\n\nEntering AgPGPluginWorker::updateBackendPGRenameStep!\n");

        msg_p = new AgClientPGRenameMessage();

        msg_p->setpgId(pAgPGRenameMessage->getpgId());
        msg_p->setName(pAgPGRenameMessage->getName());

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of PG rename is : ")+backend_ret);
            pAgPGRenameMessage->setCompletionStatus (backend_ret);
            if(backend_ret == WAVE_MESSAGE_SUCCESS)
                ret =  WAVE_MESSAGE_SUCCESS;
        }  else {
            trace (TRACE_LEVEL_ERROR, string("Sendsync failed: ")+status);
        }


        if(msg_p) {
            delete msg_p;
        }

        return ret;

    }



    void  AgPGPluginWorker::AgPGCreateMessageHandler( AgPGCreateMessage *pAgPGCreateMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::validatePGCreateStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updateBackendPGCreateStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updatePGMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPGCreateMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

   }

    ResourceId AgPGPluginWorker::validatePGCreateStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
   ResourceId return_code;

        UI32 pgid;

        trace (TRACE_LEVEL_DEBUG, "Entering AgPGPluginWorker::validatePGCreateStep!\n");

        AgPGCreateMessage *pAgPGCreateMessage =
            dynamic_cast<AgPGCreateMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pgid = pAgPGCreateMessage->getpgId();

        return_code = AgUtilsLocal::validate_pgcreate(pgid);

        if(return_code != AG_VALIDATION_SUCCESS) {
            pAgPGCreateMessage->setCompletionStatus(return_code);
            return return_code;
        }

        pAgPGCreateMessage->setCompletionStatus(return_code);

        return WAVE_MESSAGE_SUCCESS;
    }


    ResourceId AgPGPluginWorker::updatePGMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject_new = NULL;//, *pAgPortGroupLocalManagedObject = NULL;
        UI32 pgid_new;     
        string pgname;
        vector<string> pgmodes;

        trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updatePGMOStep AgPortGroupLocalManagedObject Step!\n");

        // Create new row in PG MO
        AgPGCreateMessage *pAgPGCreateMessage =
            dynamic_cast<AgPGCreateMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pgid_new = pAgPGCreateMessage->getpgId();
        pgname = string("pg")+pgid_new;
        pgmodes.clear();
        pgmodes.push_back("lb");

        pAgPortGroupLocalManagedObject_new = new AgPortGroupLocalManagedObject(dynamic_cast<AgLocalObjectManager*>(getPWaveObjectManager()), pgid_new, pgname, pgmodes);

        if (NULL == pAgPortGroupLocalManagedObject_new)
        {
            // FIXME : Sagar : Please handle this case.
        }

        //Removing deleting of MO since it crashes sometimes


        return status;

    }

    ResourceId AgPGPluginWorker::updateBackendPGCreateStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        AgClientPGCreateMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgPGCreateMessage *pAgPGCreateMessage =
            dynamic_cast<AgPGCreateMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        vector<string>::iterator iter, iter1;
        vector<string> test;        
        string port_str;

        msg_p = new AgClientPGCreateMessage();

        trace (TRACE_LEVEL_DEBUG, "\nEntering AgPGPluginWorker::updateBackendPGCreateStep!\n");

        msg_p->setpgId(pAgPGCreateMessage->getpgId());

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of PG create is : ")+backend_ret);
            pAgPGCreateMessage->setCompletionStatus (backend_ret);
            if(backend_ret == WAVE_MESSAGE_SUCCESS)
                ret =  WAVE_MESSAGE_SUCCESS;
        }  else {
            trace (TRACE_LEVEL_ERROR, string("Sendsync failed: ")+status);
        }

        return ret;
    }

    void  AgPGPluginWorker::AgPGDelMessageHandler( AgPGDelMessage *pAgPGDelMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::validatePGDelStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updateBackendPGDelStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updatePGMOForDelStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPGDelMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

   }

    ResourceId AgPGPluginWorker::validatePGDelStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        trace(TRACE_LEVEL_INFO, string("Entering AgPGPluginWorker::validatePGDelStep!"));
        ResourceId return_code = WAVE_MESSAGE_SUCCESS;
        AgPGDelMessage *pAgPGDelMessage =
            dynamic_cast<AgPGDelMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        int pgid = pAgPGDelMessage->getpgId(); 
        vector<string> nPorts = pAgPGDelMessage->getnPorts();

        return_code = AgUtilsLocal::validate_pgdel(pgid, nPorts);

        if(return_code != AG_VALIDATION_SUCCESS) {
            pAgPGDelMessage->setCompletionStatus(return_code);
            return return_code;
        }

        pAgPGDelMessage->setCompletionStatus(return_code);

        return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId AgPGPluginWorker::updateBackendPGDelStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        int backend_ret= WAVE_MESSAGE_ERROR;
        AgClientPGDelMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgPGDelMessage *pAgPGDelMessage =
            dynamic_cast<AgPGDelMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        vector<string>::iterator iter;
        vector<string> test;        
        int portinteger;
        string port_str;
        char buf[10];
        UI32 sz;
        ag_pgdel_msg_resp_t *response = NULL; 
        pgdel_nportmap_t *temp1;
        UI32 buf_num = AG_CONFIG_CMD_MAPPING;
        int i,j;
        vector<string> ports;
        int mid = DcmToolKit::getLocalMappedId();
        string interface_port;
        string nPort;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
        LocationId loc_id;

        msg_p = new AgClientPGDelMessage();

        trace (TRACE_LEVEL_INFO, string("\nEntering AgPGPluginWorker::updateBackendPGDelStep!\n"));

        trace (TRACE_LEVEL_DEBUG, string("\nAgPGPluginWorker::updateBackendPGDelStep Nports: "));

        msg_p->setpgId(pAgPGDelMessage->getpgId());
        msg_p->setpgId(pAgPGDelMessage->getpgId());

        for(iter = (pAgPGDelMessage->m_nPorts).begin(); iter != (pAgPGDelMessage->m_nPorts).end(); iter++) {
            if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                sprintf(buf,"%d",portinteger);
                port_str.assign(buf);
                (msg_p->m_nPorts).push_back(port_str);
            } else {
                 return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
            }
        }
        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of PG create is : ")+backend_ret);
            pAgPGDelMessage->setCompletionStatus (backend_ret);
            if(backend_ret == WAVE_MESSAGE_SUCCESS)
                ret =  WAVE_MESSAGE_SUCCESS;
        }  else {
            trace (TRACE_LEVEL_ERROR, string("Sendsync failed: ")+status);
        }


        if(backend_ret ==WAVE_MESSAGE_SUCCESS) {

		response = (ag_pgdel_msg_resp_t* )(msg_p->findBuffer(buf_num, sz));

		temp1 =(pgdel_nportmap_t *)((char*)response+sizeof(ag_pgdel_msg_resp_t));
		response->nport_maps = temp1;
		temp1->fports =(BitMap_t)((char*)temp1+sizeof(pgdel_nportmap_t));

		for( i = 0; i < (response->num_recs) ; i++) {

		    ports.clear();

		    if(AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, temp1->nport, nPort) != -1) { 
			trace (TRACE_LEVEL_INFO, string("nport : ")+nPort+string("\n"));
		    } else {
			trace (TRACE_LEVEL_INFO, string("trans failed!\n"));
		    }


		    pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());

		    pSyncQueryCtx->addAndAttribute (new AttributeString (nPort, "agNPortNb"));

		    loc_id = FrameworkToolKit::getThisLocationId();
		    pSyncQueryCtx->setLocationIdFilter(loc_id);
		    pResults = querySynchronously (pSyncQueryCtx);


		    if (NULL != pResults && pResults->size()) {
			startTransaction();
			trace (TRACE_LEVEL_INFO, string("\ngoing to update for nport : ")+nPort+string("\n"));
			pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
			updateWaveManagedObject(pAgNPortMapLocalManagedObject);
			(pAgNPortMapLocalManagedObject->getNFPortTopo()).clear();
			for(j=0;j<MAX_AG_PORT;j++) {
			    if(BM_isBitNSet(j, temp1->fports)){
				if(AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FCOE, j, interface_port) != -1) {
				    ports.push_back(interface_port);
				    trace (TRACE_LEVEL_INFO, string("\n____pushed back : ")+interface_port+string("\n"));
				} else {
				    trace (TRACE_LEVEL_INFO, string("\ndid not push back trans failed!\n"));
				}
			    }
			}
			pAgNPortMapLocalManagedObject->setNFPortTopo(ports);
			commitTransaction();
		    }


		    temp1=(pgdel_nportmap_t*)((char*)temp1+(sizeof(pgdel_nportmap_t))+SIZEOF_BM(MAX_AG_PORT));
		    temp1->fports =(BitMap_t)((char*)temp1+sizeof(pgdel_nportmap_t));
		}
        }
        pAgPGDelMessage->setCompletionStatus (backend_ret);
        /*
        Sometimes deleting causes crash, so commenting for now
        if(msg_p) {
            delete msg_p;
        }
        */



        return ret;

    }

    ResourceId AgPGPluginWorker::updatePGMOForDelStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        UI32 pgid_new;     
        vector<string> nports, fports, pgmodes, nports_temp, fports_temp, ports, temp;
        string pgname;
        vector<string>::iterator iter;
        BitMap_t ports_bm = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        int mid = AgUtils::getLocalMappedId(), i, portinteger;
        string interface_port;
        LocationId loc_id;

        trace (TRACE_LEVEL_DEBUG, "Entering AgPGPluginWorker::update AgPortGroupLocalManagedObject Step!\n");

        AgPGDelMessage *pAgPGDelMessage =
            dynamic_cast<AgPGDelMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pgid_new = pAgPGDelMessage->getpgId();
        nports = pAgPGDelMessage->getnPorts();

        // Retrieve row from MO
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid_new, "agPgId"));
        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updateAgPGAddMOStep : Going to update the MO!\n");
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);

            ports.clear();
            BM_Clear(ports_bm, MAX_AG_PORT);

            // Mark bits for existing N ports
            nports_temp = pAgPortGroupLocalManagedObject->getPgNPortTopo();
            for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_SetBitN(portinteger, ports_bm);
                    } 
                } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            //Clear the new bits that are added
            for(iter = nports.begin(); iter != nports.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_ClrBitN(portinteger, ports_bm);
                    } 
                } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            // Put the n ports in the MO in the interface format
            for(i = 0; i < MAX_AG_PORT; i++) {
                if ((BM_isBitNSet(i, ports_bm))) {
                    if(AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, i, interface_port) != -1) {
                        ports.push_back(interface_port);
                    } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                        return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                    }
                }
            }

            updateWaveManagedObject(pAgPortGroupLocalManagedObject);
            pAgPortGroupLocalManagedObject->setPgNPortTopo(ports);

	        //clean up data allocated.
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
	        pResults->clear();
            delete pResults;
        }
	    delete pSyncQueryCtx;

        // Clear the ports from PG0 MO
        pgid_new = 0;
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid_new, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updateAgPGAddMOStep : Going to update the PG0 MO!\n");
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);

            ports.clear();
            BM_Clear(ports_bm, MAX_AG_PORT);

            // Mark bits for existing N ports
            nports_temp = pAgPortGroupLocalManagedObject->getPgNPortTopo();
            for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_SetBitN(portinteger, ports_bm);
                    } 
                } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            // Set the bits that are added
            for(iter = nports.begin(); iter != nports.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_SetBitN(portinteger, ports_bm);
                    } 
                } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            // Put the n ports in the MO in the interface format
            for(i = 0; i < MAX_AG_PORT; i++) {
                if ((BM_isBitNSet(i, ports_bm))) {
                    if(AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, i, interface_port) != -1) {
                        ports.push_back(interface_port);
                    } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                        return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                    }
                }
            }

            updateWaveManagedObject(pAgPortGroupLocalManagedObject);
            pAgPortGroupLocalManagedObject->setPgNPortTopo(ports);

	        //clean up data allocated.
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
	        pResults->clear();
            delete pResults;
       }    

       delete pSyncQueryCtx;
       free(ports_bm);

       return status;
    }



    void  AgPGPluginWorker::AgPGAddMessageHandler( AgPGAddMessage *pAgPGAddMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::validatePGAddStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updateBackendPGAddStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::updatePGMOForAddStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgPGPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPGAddMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

   }

    ResourceId AgPGPluginWorker::validatePGAddStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        trace(TRACE_LEVEL_INFO, string("Entering AgPGPluginWorker::validatePGAddStep!"));
        ResourceId return_code = WAVE_MESSAGE_SUCCESS;
        AgPGAddMessage *pAgPGAddMessage =
            dynamic_cast<AgPGAddMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        int pgid = pAgPGAddMessage->getpgId(); 
        vector<string> nPorts = pAgPGAddMessage->getnPorts();

        return_code = AgUtilsLocal::validate_pgadd(pgid, nPorts);

        if(return_code != AG_VALIDATION_SUCCESS) {
            pAgPGAddMessage->setCompletionStatus(return_code);
            return return_code;
        }

        pAgPGAddMessage->setCompletionStatus(return_code);

        return WAVE_MESSAGE_SUCCESS;


    }

    ResourceId AgPGPluginWorker::updateBackendPGAddStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        AgClientPGAddMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgPGAddMessage *pAgPGAddMessage =
            dynamic_cast<AgPGAddMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        vector<string>::iterator iter;
        vector<string> test;        
        int portinteger;
        string port_str;
        char buf[10];
        int backend_ret = 0;

        msg_p = new AgClientPGAddMessage();

        trace (TRACE_LEVEL_DEBUG, "Entering AgPGPluginWorker::updateBackendPGAddStep!\n");

        trace (TRACE_LEVEL_DEBUG, string("AgPGPluginWorker::updateBackendPGAddStep Nports: "));

        msg_p->setpgId(pAgPGAddMessage->getpgId());

        for(iter = (pAgPGAddMessage->m_nPorts).begin(); iter != (pAgPGAddMessage->m_nPorts).end(); iter++) {
            if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                sprintf(buf,"%d",portinteger);
                port_str.assign(buf);
                (msg_p->m_nPorts).push_back(port_str);
                trace (TRACE_LEVEL_DEBUG, string("AgPGPluginWorker::updateBackendPGAddStep Nports: ")+port_str);
            } else {
                 return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
            }
        }

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend PG add is : ")+backend_ret);
            pAgPGAddMessage->setCompletionStatus (backend_ret);
            if(backend_ret == WAVE_MESSAGE_SUCCESS)
                ret =  WAVE_MESSAGE_SUCCESS;
        }  else {
            trace (TRACE_LEVEL_ERROR, string("Sendsync failed: ")+status);
        }

        return ret;
    }

    ResourceId AgPGPluginWorker::updatePGMOForAddStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        UI32 pgid_new;     
        vector<string> nports, fports, pgmodes, nports_temp, fports_temp, ports, temp;
        string pgname;
        vector<string>::iterator iter;
        BitMap_t ports_bm = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        int mid = DcmToolKit::getLocalMappedId(), i, portinteger;
        string interface_port;
        LocationId loc_id;

        trace (TRACE_LEVEL_DEBUG, "Entering AgPGPluginWorker::update AgPortGroupLocalManagedObject Step!\n");

        AgPGAddMessage *pAgPGAddMessage =
            dynamic_cast<AgPGAddMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pgid_new = pAgPGAddMessage->getpgId();
        nports = pAgPGAddMessage->getnPorts();

        // Retrieve row from MO
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid_new, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updateAgPGAddMOStep : Going to update the MO!\n");
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);

            ports.clear();
            BM_Clear(ports_bm, MAX_AG_PORT);

            // Mark bits for existing N ports
            nports_temp = pAgPortGroupLocalManagedObject->getPgNPortTopo();
            for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_SetBitN(portinteger, ports_bm);
                    } 
                } else {
                    if(ports_bm)
                        free(ports_bm);

                    if(pResults){
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                        delete pResults;
                    }

                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            //Mark the new bits that are added
            for(iter = nports.begin(); iter != nports.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_SetBitN(portinteger, ports_bm);
                    } 
                } else {
                    if(ports_bm)
                        free(ports_bm);

                    if(pResults){
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                        delete pResults;
                    }

                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            // Put the n ports in the MO in the interface format
            for(i = 0; i < MAX_AG_PORT; i++) {
                if ((BM_isBitNSet(i, ports_bm))) {
                    if(AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, i, interface_port) != -1) {
                        ports.push_back(interface_port);
                        trace (TRACE_LEVEL_DEBUG, string("AgPGPluginWorker::updateAgPGAddMOStep : PG : ")+pgid_new);
                        trace (TRACE_LEVEL_DEBUG, string("AgPGPluginWorker::updateAgPGAddMOStep : N port : ")+interface_port);
                    } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }
        
                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;


                        return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                    }
                }
            }

            updateWaveManagedObject(pAgPortGroupLocalManagedObject);
            pAgPortGroupLocalManagedObject->setPgNPortTopo(ports);
        }

        // Clear the ports from PG0 MO
        pgid_new = 0;
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid_new, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgPGPluginWorker::updateAgPGAddMOStep : Going to update the PG0 MO!\n");
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);

            ports.clear();
            BM_Clear(ports_bm, MAX_AG_PORT);

            // Mark bits for existing N ports
            nports_temp = pAgPortGroupLocalManagedObject->getPgNPortTopo();
            for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_SetBitN(portinteger, ports_bm);
                    } 
                } else {
                    if(ports_bm)
                        free(ports_bm);

                    if(pResults){
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                        delete pResults;
                    }

                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            // Clear the bits that are added
            for(iter = nports.begin(); iter != nports.end(); iter++) {
                if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
                    if(portinteger < MAX_AG_PORT) { 
                        BM_ClrBitN(portinteger, ports_bm);
                    } 
                } else {
                    if(ports_bm)
                        free(ports_bm);

                    if(pResults){
                        pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                        delete pResults;
                    }

                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                }
            }

            // Put the n ports in the MO in the interface format
            for(i = 0; i < MAX_AG_PORT; i++) {
                if ((BM_isBitNSet(i, ports_bm))) {
                    if(AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, i, interface_port) != -1) {
                        ports.push_back(interface_port);
                        trace (TRACE_LEVEL_DEBUG, string("AgPGPluginWorker::updateAgPGAddMOStep : Adding back to PG : ")+pgid_new);
                        trace (TRACE_LEVEL_DEBUG, string("AgPGPluginWorker::updateAgPGAddMOStep : N port : ")+interface_port);
                    } else {
                        if(ports_bm)
                            free(ports_bm);

                        if(pResults){
                            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                            delete pResults;
                        }

                        if(pSyncQueryCtx)
                            delete pSyncQueryCtx;

                        return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                    }
                }
            }

            updateWaveManagedObject(pAgPortGroupLocalManagedObject);
            pAgPortGroupLocalManagedObject->setPgNPortTopo(ports);
        }

        if(ports_bm)
            free(ports_bm);

        if(pResults){
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;

        return status;
    }

}
