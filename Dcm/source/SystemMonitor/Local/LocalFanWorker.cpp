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
#include "SystemMonitor/Local/LocalFanWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeFanThresholdMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeFanAlertMessage.h"
#include "SystemMonitor/Local/FanThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/FanAlertLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>

/* brocade-system-monitor.yang supported-state bit none (5) */
#define SM_FRU_FAN_NONE 32
/* include/thresh/fwdPub.h supported FRU state no bits set */
#define FW_FRU_FAN_NONE 0
/* brocade-system-monitor.yang supported-state bit all (6) */
#define SM_FRU_FAN_ALL 64
/* include/thresh/fwdPub.h supported FRU state bit defs */
#define FW_FRU_FAN_ALL (FW_FRU_ABSENT|FW_FRU_INSERTED|FW_FRU_ON|FW_FRU_FAULTY)

namespace DcmNs
{

    LocalFanWorker::LocalFanWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        FanThresholdLocalManagedObject    FanThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        FanAlertLocalManagedObject    FanAlertLocalManagedObject    (pSystemMonitorLocalObjectManager);
        FanThresholdLocalManagedObject.setupOrm ();
        addManagedClass (FanThresholdLocalManagedObject::getClassName (), this);
        FanAlertLocalManagedObject.setupOrm ();
        addManagedClass (FanAlertLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGEFANTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalFanWorker::SystemMonitorLocalChangeFanThresholdMessageHandler));
        addOperationMap (SYSTEMMONITORLOCALCHANGEFANALERT, reinterpret_cast<PrismMessageHandler> (&LocalFanWorker::SystemMonitorLocalChangeFanAlertMessageHandler));
    }

    LocalFanWorker::~LocalFanWorker ()
    {
    }

    PrismMessage  *LocalFanWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGEFANTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeFanThresholdMessage ();
                break;
            case SYSTEMMONITORLOCALCHANGEFANALERT :
                pPrismMessage = new SystemMonitorLocalChangeFanAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalFanWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FanThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FanThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((FanAlertLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FanAlertLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalFanWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
    
    UI32 LocalFanWorker::fwtoDcmState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case FW_FRU_FAN_NONE:
                {
                    retState = SM_FRU_FAN_NONE;
                    break;
                }
            case FW_FRU_FAN_ALL:
                {
                    retState = SM_FRU_FAN_ALL;
                    break;
                }
        }
        return (retState);
    }
    
    UI32 LocalFanWorker::fwtoDcmAction (UI32 actionValue)
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
   
    UI32 LocalFanWorker::DcmtoFwState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case 0:
                retState = FW_FRU_ABSENT;
                break;
            case SM_FRU_FAN_NONE:
                retState = FW_FRU_FAN_NONE;
                break;
            case SM_FRU_FAN_ALL:
                retState = FW_FRU_FAN_ALL;
                break;
        }

        return (retState); 
    }
    
    UI32 LocalFanWorker::DcmtoFwAction (UI32 actionValue)
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
    
    int LocalFanWorker::fwBackendOpFanThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFanThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        FanThresholdLocalManagedObject *pFanThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FanThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalFanWorker::fwBackendOpFanThreshold No FanThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_FANS) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pFanThreshMO = new FanThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFanThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Threshold MO exists");
            pFanThreshMO  = dynamic_cast <FanThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pFanThreshMO->getDownthreshold();
            marginalTh    = pFanThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFanThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFanThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan Get Failed") + ret);
            delete pFanAlertMO;
            delete pResults;
            delete syncQueryFanAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_FANS) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalFanWorker::fwBackendOpFan Set Failed") +ret);
            delete pFanAlertMO;
            delete pResults;
            delete syncQueryFanAlert;
            return (ret);
        }
        */
        delete pFanThreshMO;
        delete pResults;
        return (ret);
    }   
    int LocalFanWorker::fwBackendOpFanAlert (fw_fruObj_t *fruCfg)
    {
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Entering..");
        BitMap actionbitMap, statebitMap;
        int i = 0, ret = 0;
        UI32   actionVal = 1, stateVal = 1;
        UI32   dcmactionVal = 1, dcmstateVal = 1;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        FanAlertLocalManagedObject *pFanAlertMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FanAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalFanWorker::fwBackendOpFan No FanAlertMO");
            SystemMonitorLocalObjectManager *LOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < LOM->m_defFruCfg.fruNum; i++) {
                if (LOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_FAN) {
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
            pFanAlertMO = new FanAlertLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 statebitMap, actionbitMap);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Alert MO exists");
            pFanAlertMO  = dynamic_cast <FanAlertLocalManagedObject *> ((*pResults)[0]);
            statebitMap  = pFanAlertMO->getState();
            actionbitMap = pFanAlertMO->getAction();
            stateVal     = DcmtoFwState (statebitMap.getElemValue ());
            actionVal    = DcmtoFwAction (actionbitMap.getElemValue ());
            trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan - StateVal") +stateVal);
            trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan - ActionVal") +actionVal);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan Get Failed") + ret);
            delete pFanAlertMO;
            delete pResults;
            delete syncQueryFanAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < fruCfg->fruNum; i++) {
            if (fruCfg->fruObj[i].fru_type == FW_FRU_FAN) {
                trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan - StateVal") +stateVal);
                trace (TRACE_LEVEL_INFO, string ("LocalFanWorker::fwBackendOpFan - ActionVal") +actionVal);
                fruCfg->fruObj[i].alarm_state = stateVal;
                fruCfg->fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalFanWorker::fwBackendOpFan - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalFanWorker::fwBackendOpFan Set Failed") +ret);
            delete pFanAlertMO;
            delete pResults;
            delete syncQueryFanAlert;
            return (ret);
        }
        */
        delete pFanAlertMO;
        delete pResults;
        return (ret);
    }
    void LocalFanWorker::externalStateSynchronization (
            WaveAsynchronousContextForExternalStateSynchronization
            *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        /*
        int ret = 0;
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 10)
                && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {
            trace (TRACE_LEVEL_INFO, "LocalFanWorker::externalStateSynchronization Entering... ");
            sleep (10);
            //ret = fwBackendOpFan ();
            if (ret != 0)
            {
                trace (TRACE_LEVEL_ERROR, "LocalFanWorker::ESS - fwBackendOpFan Failed");
            }
        }
        */
        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();

    }
    void LocalFanWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        /*
       trace (TRACE_LEVEL_INFO, "Entering LocalFanWorker::install");
		ResourceId                      status      = WAVE_MESSAGE_SUCCESS;
        BitMap                          actionbitMap, statebitMap;
        FanThresholdLocalManagedObject  *pFanThreshMO = NULL;
        FanAlertLocalManagedObject      *pFanAlertMO = NULL;
        actionbitMap.setAllValues(1);
        statebitMap.setAllValues(1);
        startTransaction ();
        pFanThreshMO = new FanThresholdLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             3, 3);
        pFanAlertMO = new FanAlertLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             statebitMap, actionbitMap);
 
        status = commitTransaction();
        if (status == FRAMEWORK_SUCCESS)
            trace (TRACE_LEVEL_INFO, "LocalFanWorker::install - Commit Success");
        delete pFanThreshMO;
        delete pFanAlertMO;
        */
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }
    
    void  LocalFanWorker::SystemMonitorLocalChangeFanAlertMessageHandler( SystemMonitorLocalChangeFanAlertMessage *pSystemMonitorLocalChangeFanAlertMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeFanAlertMessageHandler..");
        if (pSystemMonitorLocalChangeFanAlertMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeFanAlertMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeFanAlertMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeFanAlertMessage);
            return;           
        }
 
        int ret = -1;
        int i = 0;
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        BitMap actionBM = pSystemMonitorLocalChangeFanAlertMessage->getAction();
        BitMap stateBM = pSystemMonitorLocalChangeFanAlertMessage->getState();

        UI32    actionVal = actionBM.getElemValue();
        UI32    stateVal = stateBM.getElemValue();
        UI32    dcmactionVal = 1, dcmstateVal = 1;

        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanAlertMessageHandler:actionVal:") +actionVal);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanAlertMessageHandler:stateVal:") +stateVal);
        
        /* Do All and None combination validation here */
        switch (actionVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_FAN) {
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
                    trace (TRACE_LEVEL_ERROR, "LocalFanWorker::SystemMonitorLocalChangeFanAlertMessageHandler Not a supported action");
                    pSystemMonitorLocalChangeFanAlertMessage->setCompletionStatus
                        (WRC_SM_INVALID_ACTION_ERROR);
                    reply (pSystemMonitorLocalChangeFanAlertMessage);
                    return;
                }
        }
        

        if ( ((stateVal > SM_FRU_FAN_NONE) && (stateVal != SM_FRU_FAN_ALL)) || (stateVal > SM_FRU_FAN_ALL))
        {
            trace (TRACE_LEVEL_ERROR, "LocalFanWorker::SystemMonitorLocalChangeFanAlertMessageHandler Not a supported state");
            pSystemMonitorLocalChangeFanAlertMessage->setCompletionStatus
                (WRC_SM_INVALID_STATE_ERROR);
            reply (pSystemMonitorLocalChangeFanAlertMessage);
            return;
            
        }

        switch (stateVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_FAN) {
                            stateVal = lOM->m_defFruCfg.defFruObj[i].alarm_state;
                            dcmstateVal = fwtoDcmState (stateVal);
                            stateBM.setAllValues (dcmstateVal);
                            break;
                        }
                    }
                }
                break;
            case SM_FRU_FAN_NONE:
                stateVal = FW_FRU_FAN_NONE;
                break;
            case SM_FRU_FAN_ALL:
                stateVal = FW_FRU_FAN_ALL;
                break;
        }
       /* Do FW Get and Set here */
        ret = fw_fru_cfg_get_all (&fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, "LocalFanWorker::SystemMonitorLocalChangeFanAlertMessageHandler Get Failed" +ret);
            pSystemMonitorLocalChangeFanAlertMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeFanAlertMessage);
            return;
        }
        
        for (i = 0; i < fruCfg.fruNum; i++) {
            if (fruCfg.fruObj[i].fru_type == FW_FRU_FAN) {
                fruCfg.fruObj[i].alarm_state = stateVal;
                fruCfg.fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        
        ret = fw_fru_cfg_set_all (fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("LocalFanWorker::SystemMonitorLocalChangeFanAlertMessageHandler Set Failed") +ret);
            pSystemMonitorLocalChangeFanAlertMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeFanAlertMessage);
            return;
        }
        /* Update Local MO */
        FanAlertLocalManagedObject *pFanAlertLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FanAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "LocalFanWorker::SystemMonitorLocalChangeFanAlertMessageHandler No such Fan Alert managed object");
            pSystemMonitorLocalChangeFanAlertMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeFanAlertMessage);
            return;
        }
        pFanAlertLMO = dynamic_cast <FanAlertLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        pFanAlertLMO->setState(stateBM);
        pFanAlertLMO->setAction(actionBM);
        updateWaveManagedObject (pFanAlertLMO);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeFanAlertMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

		delete pFanAlertLMO;
        pSystemMonitorLocalChangeFanAlertMessage->setCompletionStatus
            (status);
        reply (pSystemMonitorLocalChangeFanAlertMessage);
    }

    void  LocalFanWorker::SystemMonitorLocalChangeFanThresholdMessageHandler( SystemMonitorLocalChangeFanThresholdMessage *pSystemMonitorLocalChangeFanThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeFanThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeFanThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeFanThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeFanThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeFanThresholdMessage);
            return;           
        }
 
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        unsigned int i = 0;
        int downThresh     = pSystemMonitorLocalChangeFanThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeFanThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeFanThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_FANS) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeFanThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_FANS) {
                    marginalThresh = lOM->m_defpList.defStatPolicy[th].marginal.marginal; 
                    break;
                }
            }
        } 
         /* Do Fabric Watch validation here */
        swPolicyList_t pList;
        memset (&pList, 0, sizeof(pList));
        ret = switchStatusPolicyGetData(&pList);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler : Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler : Get Failed") +isChanged);
            pSystemMonitorLocalChangeFanThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeFanThresholdMessage);
            return;
        }

        if (((UI32)downThresh > pList.fan) || ((UI32)marginalThresh > pList.fan) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler : Invalid Thresh"));
            pSystemMonitorLocalChangeFanThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeFanThresholdMessage);
            return;
        }

	    trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler : Down threshold") +downThresh);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler : Marginal thresho") +marginalThresh);
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_FANS) {
	            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler : Inside Down threshold") +downThresh);
            	trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler : Inside Marginal thresho") +marginalThresh);
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFanThresholdMessageHandler :Set Failed" )+ret);
            pSystemMonitorLocalChangeFanThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeFanThresholdMessage);
            return;
        }

        /* Update Local MO */
        FanThresholdLocalManagedObject *pFanThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FanThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "SystemMonitorLocalChangeFanThresholdMessageHandler : No such Fan threshold managed object");
            pSystemMonitorLocalChangeFanThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeFanThresholdMessage);
            return;
        }
        pFanThreshLMO = dynamic_cast <FanThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pFanThreshLMO);
        pFanThreshLMO->setDownthreshold(downThresh);
        pFanThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeFanThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pFanThreshLMO;
        pSystemMonitorLocalChangeFanThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeFanThresholdMessage);
    }

}
