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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/LocalSfpWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeSfpAlertMessage.h"
#include "SystemMonitor/Local/SfpAlertLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalSfpWorker::LocalSfpWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        SfpAlertLocalManagedObject    SfpAlertLocalManagedObject    (pSystemMonitorLocalObjectManager);
        SfpAlertLocalManagedObject.setupOrm ();
        addManagedClass (SfpAlertLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGESFPALERT, reinterpret_cast<PrismMessageHandler> (&LocalSfpWorker::SystemMonitorLocalChangeSfpAlertMessageHandler));
    }

    LocalSfpWorker::~LocalSfpWorker ()
    {
    }

    PrismMessage  *LocalSfpWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGESFPALERT :
                pPrismMessage = new SystemMonitorLocalChangeSfpAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalSfpWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

		if ((SfpAlertLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SfpAlertLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalSfpWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    UI32 LocalSfpWorker::fwtoDcmState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case 0:
                {
                    retState = 32;
                    break;
                }
            case 19:
                {
                    retState = 64;
                    break;
                }
        }
        return (retState);
    }  
    UI32 LocalSfpWorker::fwtoDcmAction (UI32 actionValue)
    {
        UI32 retAction = actionValue;

        switch (actionValue) {
            case 0:
                {
                    retAction = 1;
                    break;
                }
            case 1:
                {
                    retAction = 8;
                    break;
                }
            case 16:
                {
                    retAction = 4;
                    break;
                }
            case 17:
                {
                    retAction = 2;
                    break;
                }
            default:
                {
                    break;
                }
        }
        return (retAction);
    }

    UI32 LocalSfpWorker::DcmtoFwState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case 0:
                retState = 1;
                break;
            case 32:
                retState = 0;
                break;
            case 64:
                retState = 19;
                break;
        }

        return (retState); 
    }
    
    UI32 LocalSfpWorker::DcmtoFwAction (UI32 actionValue)
    {
        UI32 retAction = actionValue;
        
        switch (actionValue) {
            case 0:
                {
                    retAction = 1;
                    break;
                }
            case 1:
                {
                    retAction = 0;
                    break;
                }
            case 2:
                {
                    retAction = 17;
                    break;
                }
            case 4:
                {
                    retAction = 16;
                    break;
                }
            case 8:
                {
                    retAction = 1;
                    break;
                }
            case 12:
                {
                    retAction = 17;
                    break;
                }
            default:
                {
                    trace (TRACE_LEVEL_ERROR, "Not a supported action");
                    break;
                }
        }
        return (retAction);
    }
    
    int LocalSfpWorker::fwBackendOpSfpAlert (fw_fruObj_t *fruCfg)
    {
        trace (TRACE_LEVEL_INFO, "LocalSfpWorker::fwBackendOpSfp - Entering..");
        BitMap actionbitMap, statebitMap;
        int i = 0, ret = 0;
        UI32   actionVal = 1, stateVal = 1;
        UI32   dcmactionVal = 1, dcmstateVal = 1;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        SfpAlertLocalManagedObject *pSfpAlertMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalSfpWorker::fwBackendOpSfp No SfpAlertMO");
            SystemMonitorLocalObjectManager *LOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < LOM->m_defFruCfg.fruNum; i++) {
                if (LOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_SFP) {
                    actionVal = LOM->m_defFruCfg.defFruObj[i].alarm_action;
                    stateVal  = LOM->m_defFruCfg.defFruObj[i].alarm_state;
                    dcmstateVal = fwtoDcmState (stateVal);
                    dcmactionVal = fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
                    break;
                }
            }
            startTransaction ();
            pSfpAlertMO = new SfpAlertLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 statebitMap, actionbitMap);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalSfpWorker::fwBackendOpSfp - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalSfpWorker::fwBackendOpSfp - Alert MO exists");
            pSfpAlertMO  = dynamic_cast <SfpAlertLocalManagedObject *> ((*pResults)[0]);
            statebitMap  = pSfpAlertMO->getState();
            actionbitMap = pSfpAlertMO->getAction();
            stateVal     = DcmtoFwState (statebitMap.getElemValue ());
            actionVal    = DcmtoFwAction (actionbitMap.getElemValue ());
            trace (TRACE_LEVEL_INFO, string ("LocalSfpWorker::fwBackendOpSfp - StateVal") +stateVal);
            trace (TRACE_LEVEL_INFO, string ("LocalSfpWorker::fwBackendOpSfp - ActionVal") +actionVal);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalSfpWorker::fwBackendOpSfp - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalSfpWorker::fwBackendOpSfp - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalSfpWorker::fwBackendOpSfp Get Failed") + ret);
            delete pSfpAlertMO;
            delete pResults;
            delete syncQuerySfpAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < fruCfg->fruNum; i++) {
            if (fruCfg->fruObj[i].fru_type == FW_FRU_SFP) {
                trace (TRACE_LEVEL_INFO, string ("LocalSfpWorker::fwBackendOpSfp - StateVal") +stateVal);
                trace (TRACE_LEVEL_INFO, string ("LocalSfpWorker::fwBackendOpSfp - ActionVal") +actionVal);
                fruCfg->fruObj[i].alarm_state = stateVal;
                fruCfg->fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalSfpWorker::fwBackendOpSfp - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalSfpWorker::fwBackendOpSfp - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalSfpWorker::fwBackendOpSfp Set Failed") +ret);
            delete pSfpAlertMO;
            delete pResults;
            delete syncQuerySfpAlert;
            return (ret);
        }
        */
        delete pSfpAlertMO;
        delete pResults;
        return (ret);
    }
    void LocalSfpWorker::externalStateSynchronization (
            WaveAsynchronousContextForExternalStateSynchronization
            *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        /*
        int ret = 0;
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 10)
                && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {
            trace (TRACE_LEVEL_INFO, "LocalSfpWorker::externalStateSynchronization Entering... ");
            sleep (10);
            //ret = fwBackendOpSfp ();
            if (ret != 0)
            {
                trace (TRACE_LEVEL_ERROR, "LocalSfpWorker::ESS - fwBackendOpSfp Failed");
            }
        }
        */
        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();

    }
    void LocalSfpWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        /*
       trace (TRACE_LEVEL_INFO, "Entering LocalSfpWorker::install");
		ResourceId                      status      = WAVE_MESSAGE_SUCCESS;
        BitMap                          actionbitMap, statebitMap;
        SfpThresholdLocalManagedObject  *pSfpThreshMO = NULL;
        SfpAlertLocalManagedObject      *pSfpAlertMO = NULL;
        actionbitMap.setAllValues(1);
        statebitMap.setAllValues(1);
        startTransaction ();
        pSfpThreshMO = new SfpThresholdLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             3, 3);
        pSfpAlertMO = new SfpAlertLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             statebitMap, actionbitMap);
 
        status = commitTransaction();
        if (status == FRAMEWORK_SUCCESS)
            trace (TRACE_LEVEL_INFO, "LocalSfpWorker::install - Commit Success");
        delete pSfpThreshMO;
        delete pSfpAlertMO;
        */
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }
    
    void  LocalSfpWorker::SystemMonitorLocalChangeSfpAlertMessageHandler( SystemMonitorLocalChangeSfpAlertMessage *pSystemMonitorLocalChangeSfpAlertMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeSfpAlertMessageHandler..");
        if (pSystemMonitorLocalChangeSfpAlertMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeSfpAlertMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeSfpAlertMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeSfpAlertMessage);
            return;           
        }
 
        int ret = -1;
        int i = 0;
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        BitMap actionBM = pSystemMonitorLocalChangeSfpAlertMessage->getAction();
        BitMap stateBM = pSystemMonitorLocalChangeSfpAlertMessage->getState();

        UI32    actionVal = actionBM.getElemValue();
        UI32    stateVal = stateBM.getElemValue();
        UI32    dcmactionVal = 1, dcmstateVal = 1;

        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeSfpAlertMessageHandler:actionVal:") +actionVal);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeSfpAlertMessageHandler:stateVal:") +stateVal);
        
        /* Do All and None combination validation here */
        switch (actionVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_SFP) {
                            actionVal = lOM->m_defFruCfg.defFruObj[i].alarm_action;
                            dcmactionVal = fwtoDcmAction (actionVal);
                            actionBM.setAllValues (dcmactionVal);
                            break;
                        }
                    }
                    break;
                }
            case 1:
                {
                    actionVal = 0;
                    break;
                }
            case 2:
                {
                    actionVal = 17;
                    break;
                }
            case 4:
                {
                    actionVal = 16;
                    break;
                }
            case 8:
                {
                    actionVal = 1;
                    break;
                }
            case 12:
                {
                    actionVal = 17;
                    break;
                }
            default:
                {
                    trace (TRACE_LEVEL_ERROR, "LocalSfpWorker::SystemMonitorLocalChangeSfpAlertMessageHandler Not a supported action");
                    pSystemMonitorLocalChangeSfpAlertMessage->setCompletionStatus
                        (WRC_SM_INVALID_ACTION_ERROR);
                    reply (pSystemMonitorLocalChangeSfpAlertMessage);
                    return;
                }
        }
        

        if ( ((stateVal > 32) && (stateVal != 64)) || (stateVal > 64))
        {
            trace (TRACE_LEVEL_ERROR, "LocalSfpWorker::SystemMonitorLocalChangeSfpAlertMessageHandler Not a supported state");
            pSystemMonitorLocalChangeSfpAlertMessage->setCompletionStatus
                (WRC_SM_INVALID_STATE_ERROR);
            reply (pSystemMonitorLocalChangeSfpAlertMessage);
            return;
            
        }

        switch (stateVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_SFP) {
                            stateVal = lOM->m_defFruCfg.defFruObj[i].alarm_state;
                            dcmstateVal = fwtoDcmState (stateVal);
                            stateBM.setAllValues (dcmstateVal);
                            break;
                        }
                    }
                }
                break;
            case 32:
                stateVal = 0;
                break;
            case 64:
                stateVal = 19;
                break;
        }
       /* Do FW Get and Set here */
        ret = fw_fru_cfg_get_all (&fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, "LocalSfpWorker::SystemMonitorLocalChangeSfpAlertMessageHandler Get Failed" +ret);
            pSystemMonitorLocalChangeSfpAlertMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeSfpAlertMessage);
            return;
        }
        
        for (i = 0; i < fruCfg.fruNum; i++) {
            if (fruCfg.fruObj[i].fru_type == FW_FRU_SFP) {
                fruCfg.fruObj[i].alarm_state = stateVal;
                fruCfg.fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        
        ret = fw_fru_cfg_set_all (fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("LocalSfpWorker::SystemMonitorLocalChangeSfpAlertMessageHandler Set Failed") +ret);
            pSystemMonitorLocalChangeSfpAlertMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeSfpAlertMessage);
            return;
        }
        /* Update Local MO */
        SfpAlertLocalManagedObject *pSfpAlertLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "LocalSfpWorker::SystemMonitorLocalChangeSfpAlertMessageHandler No such Sfp Alert managed object");
            pSystemMonitorLocalChangeSfpAlertMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeSfpAlertMessage);
            return;
        }
        pSfpAlertLMO = dynamic_cast <SfpAlertLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        pSfpAlertLMO->setState(stateBM);
        pSfpAlertLMO->setAction(actionBM);
        updateWaveManagedObject (pSfpAlertLMO);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeSfpAlertMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pSfpAlertLMO;
        pSystemMonitorLocalChangeSfpAlertMessage->setCompletionStatus
            (status);
        reply (pSystemMonitorLocalChangeSfpAlertMessage);
    }

}
