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
 *   Author : Ramachandran Anantharaman                                    *
 **************************************************************************/

#include <algorithm>
#include <vector>

#include <sstream>
#include <string>
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgMappingsPluginWorker.h"
#include "AG/Local/AgLocalConfigMessage.h"
#include "AG/Local/AgStaticMappingsMessage.h"
#include "AG/Local/AgMappingsMessage.h"
#include "AG/Local/AgFailoverMessage.h"
#include "AG/Local/AgFailbackMessage.h"
#include "AG/Local/AgLocalManagedObject.h"
#include "AG/Local/AgNPortMapLocalManagedObject.h"
#include "AG/Local/AgVFPortMessage.h"

#include "AG/Local/AgLocalTypes.h"

#include "AG/Common/AgPluginTypes.h"
#include "AG/Common/AgUtils.h"
#include "AG/Common/AgUtilsLocal.h"

#include "Fcoe/Common/FcoePluginTypes.h"

#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "ClientInterface/AG/AgClientStaticMappingMessage.h"
#include "ClientInterface/AG/AgClientMappingMessage.h"
#include "ClientInterface/AG/AgClientFailoverMessage.h"
#include "ClientInterface/AG/AgClientFailbackMessage.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"


#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"

#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "sys/fabos/fabobj.h"

namespace DcmNs
{

    AgMappingsPluginWorker::AgMappingsPluginWorker ( AgLocalObjectManager *pAgLocalObjectManager)
          : WaveWorker  (pAgLocalObjectManager)
    {
        trace (TRACE_LEVEL_DEBUG, "AgMappingsPluginWorker::AgMappingsPluginWorker Entering...");

        addOperationMap (AG_MAPPINGS_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgMappingsPluginWorker::AgMappingsMessageHandler));

        addOperationMap (AG_STATIC_MAPPINGS_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgMappingsPluginWorker::AgStaticMappingsMessageHandler));


        addOperationMap (AG_FAILOVER_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgMappingsPluginWorker::AgFailoverMessageHandler));

        addOperationMap (AG_FAILBACK_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgMappingsPluginWorker::AgFailbackMessageHandler));

	    addOperationMap (AG_VFPORT_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgMappingsPluginWorker::AgVFPortMessageHandler));

    }

    AgMappingsPluginWorker::~AgMappingsPluginWorker ()
    {
    }

    PrismMessage  *AgMappingsPluginWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case AG_MAPPINGS_MESSAGE:
                pPrismMessage = new AgMappingsMessage ();
                break;

            case AG_STATIC_MAPPINGS_MESSAGE:
                pPrismMessage = new AgStaticMappingsMessage ();
                break;

            case AG_FAILOVER_MESSAGE:
                pPrismMessage = new AgFailoverMessage ();
                break;

            case AG_FAILBACK_MESSAGE:
                pPrismMessage = new AgFailbackMessage ();
                break;

            case AG_VFPORT_MESSAGE:
                pPrismMessage = new AgVFPortMessage();
                break;

            default :
                pPrismMessage = NULL;
          }

        return (pPrismMessage);
    }

    WaveManagedObject  *AgMappingsPluginWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
        return (pWaveManagedObject);
    }



    void  AgMappingsPluginWorker::AgFailbackMessageHandler( AgFailbackMessage *pAgFailbackMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::validateFailbackStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateBackendFailbackStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateFailbackMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgFailbackMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
   }

    ResourceId AgMappingsPluginWorker::validateFailbackStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
       trace (TRACE_LEVEL_INFO, string("Entering AgMappingsPluginWorker::validateFailbackStep!\n"));

       AgFailbackMessage *pAgFailbackMessage =
            dynamic_cast<AgFailbackMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

       string nPort(pAgFailbackMessage->m_Port);

       if(!(AgUtils::isInterfaceNportLocked(nPort))) {
             trace (TRACE_LEVEL_ERROR, "Validation error : Check N port value !!\n");
             pAgFailbackMessage->setCompletionStatus(AG_INVALID_NPORT);
             return AG_INVALID_NPORT;
       }

       pAgFailbackMessage->setCompletionStatus(AG_VALIDATION_SUCCESS);

       trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::validateFailbackStep : Validation success!\n"));
       return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId AgMappingsPluginWorker::updateFailbackMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
    
        trace (TRACE_LEVEL_DEBUG, "Entering AgMappingsPluginWorker::updateFailbackMOStep!\n");

        AgFailbackMessage *pAgFailbackMessage =
            dynamic_cast<AgFailbackMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        string nPort(pAgFailbackMessage->m_Port);

        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateFailbackMOStep NPort = ") + nPort);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());

        pSyncQueryCtx->addAndAttribute (new AttributeString (nPort, "agNPortNb"));
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgMappingsPluginWorker::updateFailbackMOStep : Going to update the MO!\n");
            pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
            updateWaveManagedObject(pAgNPortMapLocalManagedObject);
            pAgNPortMapLocalManagedObject->setFailback(pAgFailbackMessage->m_Enable);
        } else {
            trace (TRACE_LEVEL_ERROR, "\nAgMappingsPluginWorker::updateFailbackMOStep : No rows exist!!\n");
            status = WAVE_MESSAGE_ERROR;
        }

        if(pSyncQueryCtx) {
            delete pSyncQueryCtx;
        }

        return status;

    }

    ResourceId AgMappingsPluginWorker::updateBackendFailbackStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        AgClientFailbackMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        int portinteger;
        char buf[10];
        AgFailbackMessage *pAgFailbackMessage =
            dynamic_cast<AgFailbackMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "\nEntering AgMappingsPluginWorker::updateBackendFailbackStep!\n");

        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateBackendFailbackStep Nport: ")+pAgFailbackMessage->m_Port);
        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateBackendFailbackStep Enable : ")+pAgFailbackMessage->m_Enable);

        msg_p = new AgClientFailbackMessage();

        if((AgUtils::getSwitchPortFromInterfaceId(pAgFailbackMessage->m_Port, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
            sprintf(buf,"%d",portinteger);
            (msg_p->m_Port).assign(buf);
        } else {
            return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
        }

        msg_p->setEnable(pAgFailbackMessage->m_Enable);

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of failback is : ")+backend_ret);
            pAgFailbackMessage->setCompletionStatus (backend_ret);
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


    void  AgMappingsPluginWorker::AgFailoverMessageHandler( AgFailoverMessage *pAgFailoverMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::validateFailoverStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateBackendFailoverStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateFailoverMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgFailoverMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
   }
   
   ResourceId AgMappingsPluginWorker::validateFailoverStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        trace (TRACE_LEVEL_INFO, string("Entering AgMappingsPluginWorker::validateFailoverStep!\n"));

       AgFailoverMessage *pAgFailoverMessage =
            dynamic_cast<AgFailoverMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

       string nPort(pAgFailoverMessage->m_Port);

       if(!(AgUtils::isInterfaceNportLocked(nPort))) {
             trace (TRACE_LEVEL_ERROR, "Validation error : Check N port value !!\n");
             pAgFailoverMessage->setCompletionStatus(AG_INVALID_NPORT);
             return AG_INVALID_NPORT;
       }

       pAgFailoverMessage->setCompletionStatus(AG_VALIDATION_SUCCESS);

       trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::validateFailoverStep : Validation Success!\n"));
       return WAVE_MESSAGE_SUCCESS;
   }

    ResourceId AgMappingsPluginWorker::updateFailoverMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
    
        trace (TRACE_LEVEL_DEBUG, "Entering AgMappingsPluginWorker::updateFailoverMOStep!\n");

        AgFailoverMessage *pAgFailoverMessage =
            dynamic_cast<AgFailoverMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        string nPort(pAgFailoverMessage->m_Port);

        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateFailoverMOStep NPort = ") + nPort);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());

        pSyncQueryCtx->addAndAttribute (new AttributeString (nPort, "agNPortNb"));
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgMappingsPluginWorker::updateFailoverMOStep : Going to update the MO!\n");
            pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
            updateWaveManagedObject(pAgNPortMapLocalManagedObject);
            pAgNPortMapLocalManagedObject->setFailover(pAgFailoverMessage->m_Enable);
        } else {
            trace (TRACE_LEVEL_ERROR, "\nAgMappingsPluginWorker::updateFailoverMOStep : No rows exist!!\n");
            status = WAVE_MESSAGE_ERROR;
        }

        if(pSyncQueryCtx) {
            delete pSyncQueryCtx;
        }

        return status;

    }

    ResourceId AgMappingsPluginWorker::updateBackendFailoverStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        AgClientFailoverMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        int portinteger;
        char buf[10];
        AgFailoverMessage *pAgFailoverMessage =
            dynamic_cast<AgFailoverMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        
        trace (TRACE_LEVEL_DEBUG, "\nEntering AgMappingsPluginWorker::updateBackendFailoverStep!\n");

        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateBackendFailoverStep Nport: ")+pAgFailoverMessage->m_Port);
        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateBackendFailoverStep Enable : ")+pAgFailoverMessage->m_Enable);

        msg_p = new AgClientFailoverMessage();

        if((AgUtils::getSwitchPortFromInterfaceId(pAgFailoverMessage->m_Port, AG_INTERFACE_TYPE_FI, portinteger)) !=-1) {
            sprintf(buf,"%d",portinteger);
            (msg_p->m_Port).assign(buf);
        } else {
            return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
        }

        msg_p->setEnable(pAgFailoverMessage->m_Enable);

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of failover is : ")+backend_ret);
            pAgFailoverMessage->setCompletionStatus (backend_ret);
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


    void  AgMappingsPluginWorker::AgStaticMappingsMessageHandler( AgStaticMappingsMessage *pAgStaticMappingsMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::validateStaticMappingsStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateBackendStaticMappingsStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateStaticMappingsMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgStaticMappingsMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
   }

    ResourceId AgMappingsPluginWorker::validateStaticMappingsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        trace (TRACE_LEVEL_INFO, string("Entering AgMappingsPluginWorker::validateStaticMappingsStep!\n"));
        ResourceId validation = AG_VALIDATION_SUCCESS;

        AgStaticMappingsMessage *pAgStaticMappingsMessage =
            dynamic_cast<AgStaticMappingsMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        string nPort = pAgStaticMappingsMessage->m_nPort;

        vector<string> fPorts = pAgStaticMappingsMessage->getStaticFPorts();

        UI32 action = pAgStaticMappingsMessage->getAction();

        if(action == AG_STATIC_MAPADD) {
            validation = AgUtilsLocal::validate_staticmapadd(nPort, fPorts);
        } else if (action == AG_STATIC_MAPDEL) {
            validation = AgUtilsLocal::validate_staticmapdel(nPort, fPorts);
        }

        if(validation != AG_VALIDATION_SUCCESS) {
            pAgStaticMappingsMessage->setCompletionStatus(validation);
            return validation;
        }

        pAgStaticMappingsMessage->setCompletionStatus(validation);

        trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::validateStaticMappingsStep : Validation Success!\n"));
        return WAVE_MESSAGE_SUCCESS;


    }

    ResourceId AgMappingsPluginWorker::updateStaticMappingsMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL, *pResults_new = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL, *pSyncQueryCtx_new = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL, *pAgNPortMapLocalManagedObject_deleteFport = NULL;
        vector<string> toDeleteFports, map_fports, ports, toAddStaticFports, existingStaticFports, toRemoveStaticFports, map_st_fports;
        AgStaticMappingsMessage *pAgStaticMappingsMessage =
            dynamic_cast<AgStaticMappingsMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        string nPort(pAgStaticMappingsMessage->m_nPort), interface_port, nport_interface_converted;
        int portint, mid = AgUtils::getLocalMappedId(), nport_cf;
        vector<string>::iterator iter, iter_delete_ports, iter_exis_ports;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        BitMap_t inverted_bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        string delete_port;

        BM_Clear(bitmap, MAX_AG_PORT);
        BM_Clear(inverted_bitmap, MAX_AG_PORT);

        trace (TRACE_LEVEL_DEBUG, "Entering AgMappingsPluginWorker::updateMappingsMOStep!\n");

        if(pAgStaticMappingsMessage->getAction() == AG_STATIC_MAPADD) {
            //add the static fports to the existing list
            pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());

            pSyncQueryCtx->addAndAttribute (new AttributeString (nPort, "agNPortNb"));
            pResults = querySynchronously (pSyncQueryCtx);

            if (NULL != pResults && pResults->size()) {
                pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
                existingStaticFports = pAgNPortMapLocalManagedObject->getStaticNFPortTopo();

                AgUtils::string_to_bitmap(existingStaticFports, bitmap, AG_INTERFACE_TYPE_FCOE);

                toAddStaticFports = pAgStaticMappingsMessage->getStaticFPorts();

                AgUtils::string_to_bitmap(toAddStaticFports, bitmap, AG_INTERFACE_TYPE_FCOE);

                updateWaveManagedObject(pAgNPortMapLocalManagedObject);

                (pAgNPortMapLocalManagedObject->m_agStaticNFPortTopo).clear();

                ports.clear();

                ports = AgUtils::bitmap_to_vectorstring(bitmap, AG_INTERFACE_TYPE_FCOE);

                pAgNPortMapLocalManagedObject->setStaticNFPortTopo(ports);
            } else {
                trace (TRACE_LEVEL_ERROR, "\nAgStaticMappingsPluginWorker::updateMappingsMOStep : No rows exist!!\n");
                status = WAVE_MESSAGE_ERROR;
            }

            commitTransaction();
            startTransaction();

            //create inverted bitmap of the F ports being added in staticmapadd to remove from mapset
            BM_Clear(bitmap, MAX_AG_PORT);

            toDeleteFports = pAgStaticMappingsMessage->getStaticFPorts();

            AgUtils::string_to_bitmap(toDeleteFports, bitmap, AG_INTERFACE_TYPE_FCOE);

            BM_Invert(inverted_bitmap, bitmap, MAX_AG_PORT);
            //creating inverted bitmap end

            for(nport_cf = 0;nport_cf < MAX_AG_PORT; nport_cf ++) {
                trace (TRACE_LEVEL_DEBUG, string("AgMappingsPluginWorker::updateStaticMappingsMOStep : Retrieving Fports for N port value = ")+nport_cf);

                AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, nport_cf, nport_interface_converted);
    
                trace (TRACE_LEVEL_DEBUG, string("AgMappingsPluginWorker::updateStaticMappingsMOStep : Retrieving Fports for N port value = ")+nport_interface_converted); 

                if(AgUtils::isInterfaceNportLocked(nport_interface_converted)) {
                    pSyncQueryCtx_new = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());
                    pSyncQueryCtx_new->addAndAttribute (new AttributeString (nport_interface_converted, "agNPortNb"));
                    pResults_new = querySynchronously (pSyncQueryCtx_new);

                    if (NULL != pResults_new && pResults_new->size()) {
                        BM_Clear(bitmap, MAX_AG_PORT);

                        pAgNPortMapLocalManagedObject_deleteFport = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults_new)[0]);
                        map_fports = pAgNPortMapLocalManagedObject_deleteFport->getNFPortTopo();
                        map_st_fports = pAgNPortMapLocalManagedObject_deleteFport->getNFPortTopo();

                        for(iter_delete_ports = toDeleteFports.begin();iter_delete_ports != toDeleteFports.end(); iter_delete_ports++) {
                            delete_port = *iter_delete_ports;
                            if(find(map_fports.begin(), map_fports.end(), delete_port) != map_fports.end()){

                                AgUtils::string_to_bitmap(map_fports, bitmap, AG_INTERFACE_TYPE_FCOE);

                                BM_And(bitmap, inverted_bitmap, MAX_AG_PORT);
    
                                ports.clear();

                                ports = AgUtils::bitmap_to_vectorstring(bitmap, AG_INTERFACE_TYPE_FCOE);

                                updateWaveManagedObject(pAgNPortMapLocalManagedObject_deleteFport);

                                pAgNPortMapLocalManagedObject_deleteFport->setNFPortTopo(ports);
                            }
                        }
                    }
                }
            }
        } else if(pAgStaticMappingsMessage->getAction() == AG_STATIC_MAPDEL){
            //remove the static fports from the n port
            pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());

            pSyncQueryCtx->addAndAttribute (new AttributeString (nPort, "agNPortNb"));
            pResults = querySynchronously (pSyncQueryCtx);

            if (NULL != pResults && pResults->size()) {
                pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
                existingStaticFports = pAgNPortMapLocalManagedObject->getStaticNFPortTopo();

                AgUtils::string_to_bitmap(existingStaticFports, bitmap, AG_INTERFACE_TYPE_FCOE);

                toRemoveStaticFports = pAgStaticMappingsMessage->getStaticFPorts();

                for(iter = toRemoveStaticFports.begin(); iter != toRemoveStaticFports.end(); iter++) {
                    AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, portint);
                    if(portint <= MAX_AG_PORT) {
                        BM_ClrBitN(portint, bitmap);
                    } else {
                        trace (TRACE_LEVEL_ERROR, string("AgMappingsPluginWorker::updateStaticMappingsMOStep : Port integer above MAX_AG_PORT\n")+portint);
                    }
                } 

                updateWaveManagedObject(pAgNPortMapLocalManagedObject);

                (pAgNPortMapLocalManagedObject->m_agStaticNFPortTopo).clear();

                ports.clear();

                ports = AgUtils::bitmap_to_vectorstring(bitmap, AG_INTERFACE_TYPE_FCOE);

                pAgNPortMapLocalManagedObject->setStaticNFPortTopo(ports);
            } else {
                trace (TRACE_LEVEL_ERROR, "\nAgStaticMappingsPluginWorker::updateMappingsMOStep : No rows exist!!\n");
                status = WAVE_MESSAGE_ERROR;
            }


        }

        return status;

    }

    ResourceId AgMappingsPluginWorker::updateBackendStaticMappingsStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        AgClientStaticMappingMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgStaticMappingsMessage *pAgStaticMappingsMessage =
            dynamic_cast<AgStaticMappingsMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        vector<string> test_fports = pAgStaticMappingsMessage->m_staticFPorts;
        vector<string>::iterator iter;
        int fportinteger, nportinteger;
        string fport_str;
        char buf[10];
        
        trace (TRACE_LEVEL_DEBUG, "\nEntering AgMappingsPluginWorker::updateStaticBackendMappingsStep!\n");

        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateStaticBackendMappingsStep Nport: ")+pAgStaticMappingsMessage->m_nPort);
        trace (TRACE_LEVEL_DEBUG, string("AgMappingsPluginWorker::updateStaticBackendMappingsStep F-Ports :\n"));
        for(iter = test_fports.begin();iter != test_fports.end(); iter++) {
            trace (TRACE_LEVEL_DEBUG, string("AgMappingsPluginWorker::updateStaticBackendMappingsStep Port :")+ *iter);
        }

        msg_p = new AgClientStaticMappingMessage();

        if((AgUtils::getSwitchPortFromInterfaceId(pAgStaticMappingsMessage->m_nPort, AG_INTERFACE_TYPE_FI, nportinteger)) !=-1) {
                sprintf(buf,"%d",nportinteger);
                (msg_p->m_nPort).assign(buf);
        } else {
            return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;

        }

        for(iter = (pAgStaticMappingsMessage->m_staticFPorts).begin(); iter != (pAgStaticMappingsMessage->m_staticFPorts).end(); iter++) {
            if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, fportinteger)) !=-1) {
                sprintf(buf,"%d",fportinteger);
                fport_str.assign(buf);
                (msg_p->m_staticFPorts).push_back(fport_str);
            } else {
                return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
            }
        }

        msg_p->m_mode = pAgStaticMappingsMessage->m_action;

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of staticmapping is : ")+backend_ret);
            pAgStaticMappingsMessage->setCompletionStatus (backend_ret);
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

    void  AgMappingsPluginWorker::AgMappingsMessageHandler( AgMappingsMessage *pAgMappingsMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::validateMappingsStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateBackendMappingsStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateMappingsMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgMappingsMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
   }

    ResourceId AgMappingsPluginWorker::validateMappingsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        trace (TRACE_LEVEL_INFO, string("Entering AgMappingsPluginWorker::validateMappingsStep!\n"));
        AgMappingsMessage *pAgMappingsMessage =
            dynamic_cast<AgMappingsMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        string nPort(pAgMappingsMessage->m_nPort);
        vector<string> fPorts = pAgMappingsMessage->m_fPorts;

        ResourceId validation;

        validation = AgUtilsLocal::validate_mapset(nPort, fPorts);

        if(validation != AG_VALIDATION_SUCCESS) {
            pAgMappingsMessage->setCompletionStatus(validation);
            return validation;
        }

        pAgMappingsMessage->setCompletionStatus(validation);

        trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::validateMappingsStep : Validation Success!\n"));
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId AgMappingsPluginWorker::updateMappingsMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
    
        trace (TRACE_LEVEL_DEBUG, "Entering AgMappingsPluginWorker::updateMappingsMOStep!\n");

        AgMappingsMessage *pAgMappingsMessage =
            dynamic_cast<AgMappingsMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        string nPort(pAgMappingsMessage->m_nPort);

        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateMappingsMOStep NPort = ") + nPort);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());

        pSyncQueryCtx->addAndAttribute (new AttributeString (nPort, "agNPortNb"));
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgMappingsPluginWorker::updateMappingsMOStep : Going to update the MO!\n");
            pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
            updateWaveManagedObject(pAgNPortMapLocalManagedObject);
            pAgNPortMapLocalManagedObject->setNFPortTopo(pAgMappingsMessage->m_fPorts);
        } else {
            trace (TRACE_LEVEL_ERROR, "\nAgMappingsPluginWorker::updateMappingsMOStep : No rows exist!!\n");
            status = WAVE_MESSAGE_ERROR;
        }

        if(pSyncQueryCtx) {
            delete pSyncQueryCtx;
        }

        return status;

    }

    ResourceId AgMappingsPluginWorker::updateBackendMappingsStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        AgClientMappingMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgMappingsMessage *pAgMappingsMessage =
            dynamic_cast<AgMappingsMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        vector<string> test_fports = pAgMappingsMessage->m_fPorts;
        vector<string>::iterator iter;
        int fportinteger, nportinteger;
        string fport_str;
        char buf[10];
        
        trace (TRACE_LEVEL_DEBUG, "\nEntering AgMappingsPluginWorker::updateBackendMappingsStep!\n");

        trace (TRACE_LEVEL_DEBUG, string("\nAgMappingsPluginWorker::updateBackendMappingsStep Nport: ")+pAgMappingsMessage->m_nPort);
        trace (TRACE_LEVEL_DEBUG, string("AgMappingsPluginWorker::updateBackendMappingsStep F-Ports :\n"));
        for(iter = test_fports.begin();iter != test_fports.end(); iter++) {
            trace (TRACE_LEVEL_DEBUG, string("AgMappingsPluginWorker::updateBackendMappingsStep Port :")+ *iter);
        }

        msg_p = new AgClientMappingMessage();

        if((AgUtils::getSwitchPortFromInterfaceId(pAgMappingsMessage->m_nPort, AG_INTERFACE_TYPE_FI, nportinteger)) !=-1) {
                sprintf(buf,"%d",nportinteger);
                (msg_p->m_nPort).assign(buf);
        } else {
            return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;

        }

        for(iter = (pAgMappingsMessage->m_fPorts).begin(); iter != (pAgMappingsMessage->m_fPorts).end(); iter++) {
            if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, fportinteger)) !=-1) {
                sprintf(buf,"%d",fportinteger);
                fport_str.assign(buf);
                (msg_p->m_fPorts).push_back(fport_str);
            } else {
                return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
            }
        }

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of mapping is : ")+backend_ret);
            pAgMappingsMessage->setCompletionStatus (backend_ret);
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

    void  AgMappingsPluginWorker::AgVFPortMessageHandler( AgVFPortMessage *pAgVFPortMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateBackendStepByVFEvent),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::updateMOStepByVFEvent),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgMappingsPluginWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgVFPortMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
   }

/*
	ResourceId AgMappingsPluginWorker::postbootDefaultMapSetup()
	{
        ResourceId status = WAVE_MESSAGE_SUCCESS;
	    vector<UI32> list_by_port;
	    AgVFPortMessage *msg_p = new AgVFPortMessage(
	        VFPORT_MESSAGE_CREATE);
        int i;

        // pull out the list of logical port numbers
        for (i = 0; i < 1100; i++) {
            list_by_port.push_back(i);
        }
	    msg_p->setVFPortsByPort(list_by_port);

	    if ((status = processVFEvent(msg_p, AG_UPDATE_BACKEND)) !=
	        WAVE_MESSAGE_SUCCESS) {
	        return (status);
	    }

	    startTransaction();
	    status = processVFEvent(msg_p, AG_UPDATE_DCMD);
	    commitTransaction();

	    return status;
	}
*/

    ResourceId AgMappingsPluginWorker::sendUpdateBackendMappings(AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject)
    {
        int backend_ret;
        AgClientMappingMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        int fportinteger, nportinteger;
        string fport_str;
	    string nport_ifname;
        char buf[10];
        
        trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::sendUpdateBackendMappings for n_port: ") + pAgNPortMapLocalManagedObject->getNPortNb());

        msg_p = new AgClientMappingMessage();

	    nport_ifname = pAgNPortMapLocalManagedObject->getNPortNb();
        if((AgUtils::getSwitchPortFromInterfaceId(nport_ifname, AG_INTERFACE_TYPE_FI, nportinteger)) !=-1) {
	        sprintf(buf,"%d",nportinteger);
	        (msg_p->m_nPort).assign(buf);
        } else {
            return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
        }

	    for (unsigned int i = 0; i < pAgNPortMapLocalManagedObject->getNFPortTopo().size(); i++) {
            if((AgUtils::getSwitchPortFromInterfaceId(pAgNPortMapLocalManagedObject->getNFPortTopo()[i], AG_INTERFACE_TYPE_FCOE, fportinteger)) !=-1) {
                sprintf(buf,"%d",fportinteger);
                fport_str.assign(buf);
                (msg_p->m_fPorts).push_back(fport_str);
            } else {
                return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
            }
        }

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of send update backend mappings is : ")+backend_ret);
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

    ResourceId AgMappingsPluginWorker::updateBackendStepByVFEvent(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        AgVFPortMessage *pAgVFPortMessage =
            dynamic_cast<AgVFPortMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::updateBackendStepByVFEvent got: ") + pAgVFPortMessage->getMessageType());

		// we need to see if the newly expanded/deexpand message
	   	return (processVFEvent(pAgVFPortMessage, AG_UPDATE_BACKEND));
	}

    ResourceId AgMappingsPluginWorker::updateMOStepByVFEvent(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        AgVFPortMessage *pAgVFPortMessage =
            dynamic_cast<AgVFPortMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::updateMOStepByVFEvent got: ") + pAgVFPortMessage->getMessageType());

		// we need to see if the newly expanded/deexpand message
	   	return (processVFEvent(pAgVFPortMessage, AG_UPDATE_DCMD));
	}

    ResourceId AgMappingsPluginWorker::processVFEvent(AgVFPortMessage *pAgVFPortMessage, AgActionType action) {
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<AgNPortMapLocalManagedObject *> agNPortMapLocalManagedObjectList;
	    unsigned int i, j, k, l;
	    int mapped_id;
	    int configured_n_ports = 0;
	    vector<int> updated_index;
    
	    mapped_id = DcmToolKit::getLocalMappedId();

	    // is already a part
	    // of existing N_Port to VF_Ports mapping. since it is costl to
	    // find the mapping for a given vf_port, let's find them and
	    // create a vector to just walk through.

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());

        pResults = querySynchronously (pSyncQueryCtx);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0) {
            trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::found no nports "));
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return status;
        }
      
	    // looks like we got more than 0 entries  
	    for (i = 0; i < numberOfResults; i++) {
            pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[i]);
            trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::searchNPortMapByNPortTuple found nport ") + pAgNPortMapLocalManagedObject->getNPortNb());
	        agNPortMapLocalManagedObjectList.push_back(pAgNPortMapLocalManagedObject);
	    }

        pResults->clear();
        delete pResults;

	    configured_n_ports = numberOfResults;

	    for (i = 0; i < (unsigned int)configured_n_ports; i++) {
	        updated_index.push_back(0);
	    }

	    if (pAgVFPortMessage->getMessageType() == VFPORT_MESSAGE_CREATE) {
	        // now that I have a list of all the nport mappings. let's go
	        // through newly created vf_port and see if that shows up anywhere.
	        vector<UI32> portList = pAgVFPortMessage->getVFPortsByPort();
	        for (j = 0; j < portList.size(); j++) {
	            int portNum = portList[j];
	            int found = 0;
	            string vfPort;
	            if(AgUtils::getInterfaceIdFromSwitchPort(mapped_id,
	                AG_INTERFACE_TYPE_FCOE, portNum, vfPort) != -1) {
                    trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::processing add vfport ") + vfPort + string(" logical port ") + portNum);
	            } else {
                    trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::did not find vfport for logical port to add ") + portNum);
	                continue;
	            }

	            for (k = 0;
	                k < agNPortMapLocalManagedObjectList.size(); k++) {
	                pAgNPortMapLocalManagedObject =
	                    agNPortMapLocalManagedObjectList[k];

	                vector<string> map_fports;
	                map_fports = pAgNPortMapLocalManagedObject->getNFPortTopo();
	                for (l = 0; l < map_fports.size(); l++) {
	                    if (map_fports[l].compare(vfPort) == 0) {
	                        found++;
	                        break;
	                    }
	                }

	                if (found) {
                        trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::found vfport ") + vfPort + string(" with ") + pAgNPortMapLocalManagedObject->getNPortNb() + string(" skip adding"));
	                    break;
	                }

	                vector<string> staticmap_fports;
	                staticmap_fports = pAgNPortMapLocalManagedObject->getStaticNFPortTopo();
	                for (l = 0; l < staticmap_fports.size(); l++) {
	                    if (staticmap_fports[l].compare(vfPort) == 0) {
	                        found++;
	                        break;
	                    }
	                }

	                if (found) {
                        trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::found vfport ") + vfPort + string(" statically mapped with ") + pAgNPortMapLocalManagedObject->getNPortNb() + string(" skip adding"));
	                    break;
	                }
    	        }

			    if (!found) {
	                // if vf_port is not found, we need to add to its default
	                // n_port
	                int nPortIndex = portNum % configured_n_ports;

	                pAgNPortMapLocalManagedObject =
	                    agNPortMapLocalManagedObjectList[nPortIndex];

	                trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker:: ") + vfPort + string(" need to be added to ") + pAgNPortMapLocalManagedObject->getNPortNb() + string(" located at index of ") + nPortIndex + string(" out of ") + configured_n_ports);

	                vector<string> vfports;
	                vfports.clear();

	                for (l = 0; l < pAgNPortMapLocalManagedObject->getNFPortTopo().size(); l++) {
	                    vfports.push_back(pAgNPortMapLocalManagedObject->getNFPortTopo()[l]);
	                }
	                vfports.push_back(vfPort);

	                if (action == AG_UPDATE_DCMD) {
                        updateWaveManagedObject(pAgNPortMapLocalManagedObject);
	                } else {
	                    updated_index[nPortIndex]++;
	                }
	                pAgNPortMapLocalManagedObject->getNFPortTopo().clear();
	                pAgNPortMapLocalManagedObject->setNFPortTopo(vfports);
	            }
	 	    }
	    } else {
	        vector<string> portList = pAgVFPortMessage->getVFPortsByIfName();
	        // looks like delete case
	        for (j = 0; j < portList.size(); j++) {
	            int found = 0;
	            string vfPort = portList[j];
                trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker::processing delete vfport ") + vfPort);

	            for (k = 0;
	                k < agNPortMapLocalManagedObjectList.size(); k++) {
	                pAgNPortMapLocalManagedObject =
	                    agNPortMapLocalManagedObjectList[k];

	                vector<string> vfports;
	                vfports.clear();
	                for (l = 0; l < pAgNPortMapLocalManagedObject->getNFPortTopo().size(); l++) {
	                    if (pAgNPortMapLocalManagedObject->getNFPortTopo()[l].compare(vfPort) == 0) {
	                        found++;
	                    } else {
	                        vfports.push_back(pAgNPortMapLocalManagedObject->getNFPortTopo()[l]);
	                    }
	                }
	                
	                if (found) {            
	                    trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker:: ") + vfPort + string(" deleted from ") + pAgNPortMapLocalManagedObject->getNPortNb());
	                    if (action == AG_UPDATE_DCMD) {
                            updateWaveManagedObject(pAgNPortMapLocalManagedObject);
	                    } else {
	                        updated_index[k]++;
	                    }
	                    pAgNPortMapLocalManagedObject->getNFPortTopo().clear();
	                    pAgNPortMapLocalManagedObject->setNFPortTopo(vfports);
	                    break;
	                }

	                vfports.clear();
	                for (l = 0; l < pAgNPortMapLocalManagedObject->getStaticNFPortTopo().size(); l++) {
	                    if (pAgNPortMapLocalManagedObject->getStaticNFPortTopo()[l].compare(vfPort) == 0) {
	                        found++;
	                    } else {
	                        vfports.push_back(pAgNPortMapLocalManagedObject->getStaticNFPortTopo()[l]);
	                    }
	                }
	                
	                if (found) {            
	                    trace (TRACE_LEVEL_INFO, string("AgMappingsPluginWorker:: ") + vfPort + string(" deleted from static map of ") + pAgNPortMapLocalManagedObject->getNPortNb());
	                    if (action == AG_UPDATE_DCMD) {
                            updateWaveManagedObject(pAgNPortMapLocalManagedObject);
	                    } else {
	                        updated_index[k]++;
	                    }
	                    pAgNPortMapLocalManagedObject->getStaticNFPortTopo().clear();
	                    pAgNPortMapLocalManagedObject->setStaticNFPortTopo(vfports);
	                    break;
	                }
    	        }
	 	    }
	    }

/*
	    for  (i = 0; i < agNPortMapLocalManagedObjectList.size(); i++) {
	        pAgNPortMapLocalManagedObject =
	            agNPortMapLocalManagedObjectList[i];
	        trace (TRACE_LEVEL_INFO, string("nport: ") + pAgNPortMapLocalManagedObject->getNPortNb());

	        for (l = 0;
	            l < pAgNPortMapLocalManagedObject->getNFPortTopo().size();
	            l++) {
	            trace (TRACE_LEVEL_INFO, string("associated vfport:: ") + pAgNPortMapLocalManagedObject->getNFPortTopo()[l]);
	        }
	    }
*/

	    if (action == AG_UPDATE_BACKEND) {
	        for (i = 0; i < updated_index.size(); i++) {
	            if (updated_index[i]) {
	                status = sendUpdateBackendMappings(agNPortMapLocalManagedObjectList[i]);
	            }
	        }
	    }
        return status;
    }

}
