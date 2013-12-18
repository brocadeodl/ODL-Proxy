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
 *   Author : rhkumar/ranantha                                             *
 **************************************************************************/

#include <algorithm>
#include <vector>
#include <sstream>

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalConfigMessage.h"
#include "AG/Local/AgLocalShowMessage.h"
#include "AG/Local/AgLocalManagedObject.h"
#include "AG/Local/AgPGFnmtovMessage.h"
#include "AG/Local/AgRelCounterMessage.h"
#include "AG/Local/AgNPortMapLocalManagedObject.h"
#include "AG/Local/AgPortGroupLocalManagedObject.h"
#include "AG/Local/AgLocalTypes.h"
#include "AG/Common/AgPluginTypes.h"
#include "AG/Common/AgUtils.h"
#include "AG/Common/AgUtilsLocal.h"

#include "PortCfg/Local/PortCfgUpdateFCInfoMessage.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"

#include "ClientInterface/Nsm/NsmUtils.h"

#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "ClientInterface/AG/AgClientShowMessage.h"
#include "ClientInterface/AG/AgClientFnmtovMessage.h"
#include "ClientInterface/AG/AgClientRelCounterMessage.h"
#include "ClientInterface/AG/AgClientPolicyMessage.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"


#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"

#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "sys/fabos/fabobj.h"
#include "AG/Local/AgLocalWorker.h"
#include "VcsFabric/Local/VcsNodeCopyDefaultStartMessages.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "fabos/license.h"



namespace DcmNs
{

    AgLocalWorker::AgLocalWorker ( AgLocalObjectManager *pAgLocalObjectManager)
          : WaveWorker  (pAgLocalObjectManager)
    {
        trace (TRACE_LEVEL_DEBUG, "AgLocalWorker::AgLocalWorker Entering...");

        AgLocalManagedObject    AgLocalManagedObject    (pAgLocalObjectManager);
        AgLocalManagedObject.setupOrm ();
        addManagedClass (AgLocalManagedObject::getClassName (), this);

        AgNPortMapLocalManagedObject    AgNPortMapLocalManagedObject    (pAgLocalObjectManager);
        AgNPortMapLocalManagedObject.setupOrm ();
        addManagedClass (AgNPortMapLocalManagedObject::getClassName (), this);

        AgPortGroupLocalManagedObject    AgPortGroupLocalManagedObject    (pAgLocalObjectManager);
        AgPortGroupLocalManagedObject.setupOrm ();
        addManagedClass (AgPortGroupLocalManagedObject::getClassName (), this);

        addOperationMap (AG_LOCAL_CONFIG_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgLocalWorker::AgLocalConfigMessageHandler));
        addOperationMap (AG_LOCAL_SHOW_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgLocalWorker::AgLocalShowMessageHandler));
        addOperationMap (AG_POLICY_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgLocalWorker::AgPolicyMessageHandler));
        addOperationMap (AG_FNMTOV_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgLocalWorker::AgPGFnmtovMessageHandler));
        addOperationMap (AG_RELCOUNTER_MESSAGE, reinterpret_cast<PrismMessageHandler> (&AgLocalWorker::AgRelCounterMessageHandler));
    }

    AgLocalWorker::~AgLocalWorker ()
    {
    }

    PrismMessage  *AgLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case AG_LOCAL_CONFIG_MESSAGE:
                pPrismMessage = new AgLocalConfigMessage ();
                break;

            case AG_LOCAL_SHOW_MESSAGE:
                pPrismMessage = new AgLocalShowMessage ();
                break;

            case AG_POLICY_MESSAGE:
                pPrismMessage = new AgPolicyMessage ();
                break;

            case AG_FNMTOV_MESSAGE:
                pPrismMessage = new AgPGFnmtovMessage();
                break;

            case AG_RELCOUNTER_MESSAGE:
                pPrismMessage = new AgRelCounterMessage();
                break;

            default :
                pPrismMessage = NULL;
          }

        return (pPrismMessage);
    }

    WaveManagedObject  *AgLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        trace (TRACE_LEVEL_DEBUG, "AgLocalWorker::createManagedObjectInstance Entering...");

        if ((AgLocalManagedObject::getClassName ()) == managedClassName)
        {
            trace (TRACE_LEVEL_DEBUG, "AgLocalWorker::createManagedObjectInstance creating MO...");

            pWaveManagedObject = new AgLocalManagedObject(dynamic_cast<AgLocalObjectManager *>(getPWaveObjectManager()));
        } 
        else if ((AgPortGroupLocalManagedObject::getClassName ()) == managedClassName)
        {
            trace (TRACE_LEVEL_DEBUG, "AgLocalObjectManager::createManagedObjectInstance creating AgPortGroupLocalManagedObject...");
            pWaveManagedObject = new AgPortGroupLocalManagedObject(dynamic_cast<AgLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((AgNPortMapLocalManagedObject::getClassName ()) == managedClassName)
        {
            trace (TRACE_LEVEL_DEBUG, "AgLocalObjectManager::createManagedObjectInstance creating AgNPortMapLocalManagedObject...");
            pWaveManagedObject = new AgNPortMapLocalManagedObject(dynamic_cast<AgLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "AgLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }

        return (pWaveManagedObject);
    }

    void AgLocalWorker::AgRelCounterMessageHandler( AgRelCounterMessage *pRelCounterMessage)
   {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::updateBackendRelCounterStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::updateRelCounterMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pRelCounterMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
   }

    ResourceId AgLocalWorker::updateRelCounterMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) 
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        AgRelCounterMessage *pAgRelCounterMessage =
            dynamic_cast<AgRelCounterMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        trace (TRACE_LEVEL_DEBUG, string("\nIn updateRelcounterMOStep!"));
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgLocalManagedObject *pAgLocalManagedObject = NULL;

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgLocalManagedObject::getClassName());

        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgLocalWorker::updateRelCounterMOStep : Going to update the MO!\n");
            pAgLocalManagedObject = dynamic_cast<AgLocalManagedObject *>((*pResults)[0]);
            updateWaveManagedObject(pAgLocalManagedObject);
            pAgLocalManagedObject->setRelCounterValue(pAgRelCounterMessage->m_RelCounterValue);
        }

        if(pResults){
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;

        return status;
    }

    ResourceId AgLocalWorker::updateBackendRelCounterStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgRelCounterMessage *pAgRelCounterMessage =
            dynamic_cast<AgRelCounterMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        AgClientRelCounterMessage *msg_p = new AgClientRelCounterMessage();
        UI32 value;


        value = pAgRelCounterMessage->getRelCounterValue();

        msg_p->setRelCounterValue(value);

        msg_p->setCmdCode(pAgRelCounterMessage->getCmdCode());

        trace (TRACE_LEVEL_DEBUG, string("\nIn updateiBackendRelCounterStep with value = ") + value);

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of rel counter set is : ")+backend_ret);
            pAgRelCounterMessage->setCompletionStatus (backend_ret);
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

    void AgLocalWorker::AgPGFnmtovMessageHandler( AgPGFnmtovMessage *pAgPGFnmtovMessage)
   {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::updateBackendFnmtovStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::updateFnmtovMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPGFnmtovMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
   }

    ResourceId AgLocalWorker::updateFnmtovMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) 
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        AgPGFnmtovMessage *pAgPGFnmtovMessage =
            dynamic_cast<AgPGFnmtovMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        trace (TRACE_LEVEL_DEBUG, string("\nIn updateFnmtovMOStep!"));
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgLocalManagedObject *pAgLocalManagedObject = NULL;

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgLocalManagedObject::getClassName());

        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, "AgLocalWorker::updateFnmtovMOStep : Going to update the MO!\n");
            pAgLocalManagedObject = dynamic_cast<AgLocalManagedObject *>((*pResults)[0]);
            updateWaveManagedObject(pAgLocalManagedObject);
            pAgLocalManagedObject->setFnmtov(pAgPGFnmtovMessage->m_Fnmtovalue);

        }

        if(pResults){
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;

        return status;
    }

    ResourceId AgLocalWorker::updateBackendFnmtovStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgPGFnmtovMessage *pAgPGFnmtovMessage =
            dynamic_cast<AgPGFnmtovMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        AgClientFnmtovMessage * msg_p = new AgClientFnmtovMessage();

        UI32 value = pAgPGFnmtovMessage->getFnmtoValue();

        msg_p->setFnmtoValue(value);

        trace (TRACE_LEVEL_DEBUG, string("\nIn updateiBackendFnmtovStep with value = ") + value);

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of fnmtov set is : ")+backend_ret);
            pAgPGFnmtovMessage->setCompletionStatus (backend_ret);
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

    void  AgLocalWorker::AgLocalConfigMessageHandler( AgLocalConfigMessage *pAgLocalConfigMessage) 
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::validationForEnableStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::updateBackendEnableStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::rebootLocalNodeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
        
    }

    ResourceId AgLocalWorker::validationForEnableStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {

        int swModel = get_pod_SWBD();

        trace (TRACE_LEVEL_DEBUG, "\nIn  validationForEnableStep!!");

        /* Platform Check */
        if(swModel != SWBD_ELARA2F &&
           swModel != SWBD_CALLISTOF) {
            trace (TRACE_LEVEL_DEBUG, "\nPlatform not supported!!");
            return AG_UNSUPPORTED_PLATFORM;
        }

        /* License check */
        AgLocalConfigMessage *pAgLocalConfigMessage =
            dynamic_cast<AgLocalConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        if (pAgLocalConfigMessage->m_agEnable == true) {
            if (licenseCheck(BASE_FCOE_LICENSE) == 0) {
                trace (TRACE_LEVEL_DEBUG, "\n BASE_FCOE_LICENSE not present");
                return AG_NO_FCOE_LICENSE; 
            }
        }

        return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId AgLocalWorker::updateBackendEnableStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        AgClientEnableMessage *msg_p = NULL;
	    ResourceId status = WAVE_MESSAGE_SUCCESS, error = WAVE_MESSAGE_SUCCESS;
	    //SI32 vcs_mode = 0;
	    UI32 sz;
	    UI32 buf_num = AG_CONFIG_CMD_ENABLE;
	    ag_config_msg_resp_t *response;

        AgLocalConfigMessage *pAgLocalConfigMessage =
            dynamic_cast<AgLocalConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "\nIn updateBackendEnableStep with enable = " + pAgLocalConfigMessage->m_agEnable);

		 
        if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            //vcs_mode = 0;
        } else {
            //vcs_mode = 1;
        }


        msg_p = new AgClientEnableMessage();
        msg_p->setEnable(pAgLocalConfigMessage->m_agEnable);
        msg_p->setCmdCode(AG_CONFIG_CMD_ENABLE);

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if (WAVE_MESSAGE_SUCCESS == status) {
            status = msg_p->getClientStatus();
            if (WRC_WAVE_MESSAGE_SUCCESS == status) {
            } else {
                trace (TRACE_LEVEL_DEBUG, "\nagenable - agd error\n");
                response = (ag_config_msg_resp_t* )(msg_p->findBuffer(buf_num, sz));
                error =  AgLocalWorker::convert_agerror_to_dcmderror(response->status);
                trace (TRACE_LEVEL_INFO, string("\nagenable - agd response->status: ")+response->status+string("\n"));
                trace (TRACE_LEVEL_INFO, string("\nagenable - agd msg_p->getClientStatus(): ")+status+string("\n"));
                delete msg_p;
                return error;
            }
        } else {
            trace (TRACE_LEVEL_DEBUG, "\nFailed to send agenable to agd");
        }
        delete msg_p;

        pAgLocalConfigMessage->setCompletionStatus (status);

        return status;
    }

    ResourceId AgLocalWorker::rebootLocalNodeStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
    bool                            isStandbyRebootOnly             = false;
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;

	AgLocalConfigMessage *pAgLocalConfigMessage =
	    dynamic_cast<AgLocalConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

    trace (TRACE_LEVEL_INFO, string("AgLocalWorker::rebootLocalNodeStep: Rebooting local node. reboot request is ") + pAgLocalConfigMessage->getCopyDefToStartUpFlag());

	if (pAgLocalConfigMessage->getCopyDefToStartUpFlag() == true) {
        status = rebootLocalNode (isStandbyRebootOnly);
	}

    return (status);
    }

    ResourceId AgLocalWorker::rebootLocalNode (const bool &isStandbyRebootOnly)
    {
    vector<string>  output;
    SI32            rstatus = 0;
    ResourceId      status  = WAVE_MESSAGE_SUCCESS;

    if (VcsNodeFabosInterfaceObjectManager::getIsActiveNode())
    {
        rstatus = FrameworkToolKit::systemCommandOutput ("/fabos/sbin/chassisReboot", output);
    }
    else if (isStandbyRebootOnly)
    {
        rstatus = FrameworkToolKit::systemCommandOutput ("/fabos/sbin/reboot -s -r StandbySyncVcsMode", output);
    }

    trace (TRACE_LEVEL_DEVEL, string("VcsFabricLocalObjectManager::reboot : Reboot Status : ") + rstatus);

    if (0 != rstatus)
    {
        trace (TRACE_LEVEL_WARN, string("VcsFabricLocalObjectManager::reboot : Reboot Status : ") + rstatus);

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    return (status);
    }

    ResourceId AgLocalWorker::convert_agerror_to_dcmderror(int agerror) {
        switch(agerror) {
            case AGMODE_ESWONLINE:
                return AG_AGMODE_ESWONLINE;

            case AGMODE_ENOCHG:
                return AG_AGMODE_ENOCHG;

            case AGMODE_EFAILED:
                return AG_AGMODE_EFAILED;

            case AGMODE_ESECMODE:
                return AG_AGMODE_ESECMODE;

            case AGMODE_ENOSYS:
                return AG_AGMODE_ENOSYS;

            case AGMODE_EMSPLDB:
                return AG_AGMODE_EMSPLDB;

            default:
                return AG_GENERIC_ERROR;

        }
    }

    ResourceId AgLocalWorker::sendShowCommandToAgDaemon(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        AgClientShowMessage  *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 cmdCode;

        trace(TRACE_LEVEL_DEBUG, "AgLocalWorker::sendShowCommandToAgDaemon Entering...");
        AgLocalShowMessage *pAgLocalShowMessage =
                dynamic_cast<AgLocalShowMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        msg_p = new AgClientShowMessage();
        cmdCode = pAgLocalShowMessage->getCmdCode();
        msg_p->setCmdCode(cmdCode);
        msg_p->setElements(pAgLocalShowMessage->getElements());

        status = sendSynchronouslyToWaveClient("agd", msg_p);
        if(WAVE_MESSAGE_SUCCESS != status)
        {
            trace(TRACE_LEVEL_ERROR, "AgLocalWorker::sendShowCommandToAgDaemon Send failed..." + status);
            delete msg_p;
            return status;
        }

        if(!(status = msg_p->getClientStatus()))
            status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, "AgLocalWorker::sendShowCommandToAgDaemon Final status..." + status);
        pAgLocalShowMessage->setCompletionStatus(status);
        pAgLocalShowMessage->copyAllBuffers(*msg_p);

        if(msg_p){
            delete msg_p;
        }

        return status;
    }

    void AgLocalWorker::AgLocalShowMessageHandler(AgLocalShowMessage *pAgLocalShowMessage)
    {
        //seq steps
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::sendShowCommandToAgDaemon),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = 
                new PrismSynchronousLinearSequencerContext (pAgLocalShowMessage, this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
    void AgLocalWorker::AgPolicyMessageHandler(AgPolicyMessage *pAgPolicyMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::validatePolicyStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::updateBackendPolicyStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::updatePolicyMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&AgLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAgPolicyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId AgLocalWorker::validatePolicyStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
       trace (TRACE_LEVEL_INFO, string("Entering AgLocalWorker::validatePolicyStep!\n"));

        ResourceId return_code = WAVE_MESSAGE_SUCCESS;
        AgPolicyMessage *pAgPolicyMessage =
            dynamic_cast<AgPolicyMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        string policy = pAgPolicyMessage->m_Policy;
        vector<string> policies;

        policies.clear();
        policies.push_back(policy);
        return_code = AgUtilsLocal::validate_policy(policies, pAgPolicyMessage->getEnable());

        if(return_code != AG_VALIDATION_SUCCESS) {
            pAgPolicyMessage->setCompletionStatus(return_code);
            return return_code;
        }

        pAgPolicyMessage->setCompletionStatus(return_code);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId AgLocalWorker::updateBackendPolicyStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int backend_ret;
        AgClientPolicyMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS, ret = WAVE_MESSAGE_ERROR;
        AgPolicyMessage *pAgPolicyMessage =
            dynamic_cast<AgPolicyMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        //char buf[10];
        string policy;
        vector<string>::iterator iter;

        trace (TRACE_LEVEL_DEBUG, "Entering AgLocalWorker::updateBackendPolicyStep!");

        msg_p = new AgClientPolicyMessage();

        msg_p->m_Policy.assign(pAgPolicyMessage->m_Policy);
        trace (TRACE_LEVEL_DEBUG, string("AgLocalWorker::updateBackendPolicyStep Enable : ")+msg_p->m_Policy);
        msg_p->setEnable(pAgPolicyMessage->m_Enable);
        trace (TRACE_LEVEL_DEBUG, string("AgLocalWorker::updateBackendPolicyStep Enable : ")+msg_p->m_Enable);

        status = sendSynchronouslyToWaveClient("agd", msg_p);

        if(status == WAVE_MESSAGE_SUCCESS) {
            backend_ret = msg_p->getClientStatus();
            trace (TRACE_LEVEL_DEBUG, string("The return status of backend of policyset is : ")+backend_ret);
            pAgPolicyMessage->setCompletionStatus (backend_ret);
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

   ResourceId AgLocalWorker::updatePolicyMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgLocalManagedObject *pAgLocalManagedObject = NULL;
        vector<string> curr_policy;
        vector<string>::iterator iter;

        trace (TRACE_LEVEL_DEBUG, "Entering AgLocalWorker::updatePolicyMOStep!\n");

        AgPolicyMessage *pAgPolicyMessage =
            dynamic_cast<AgPolicyMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgLocalManagedObject::getClassName());
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && (pResults->size())) {
            trace (TRACE_LEVEL_DEBUG, "AgLocalWorker::updatePolicyMOStep : Going to update the MO!\n");
            pAgLocalManagedObject = dynamic_cast<AgLocalManagedObject *>((*pResults)[0]);

            curr_policy = pAgLocalManagedObject->getPolicy();

            if(pAgPolicyMessage->m_Enable) {
                curr_policy.push_back(pAgPolicyMessage->m_Policy);
                trace (TRACE_LEVEL_DEBUG, string("\nAgLocalWorker::updatePolicyMOStep policy added = ") + string(curr_policy.back()));
            } else {
               iter = find(curr_policy.begin(), curr_policy.end(), pAgPolicyMessage->m_Policy);
               trace (TRACE_LEVEL_DEBUG, string("\nAgLocalWorker::updatePolicyMOStep policy deleted = ") + (*iter));
               curr_policy.erase(iter);
            }

            updateWaveManagedObject(pAgLocalManagedObject);
            pAgLocalManagedObject->setPolicy(curr_policy);
        } else {
            trace (TRACE_LEVEL_ERROR, "\nAgLocalWorker::updatePolicyMOStep : No rows exist!!\n");
            status = WAVE_MESSAGE_ERROR;
        }

        if(pSyncQueryCtx) {
            delete pSyncQueryCtx;
        }

        return status;

    }

}
