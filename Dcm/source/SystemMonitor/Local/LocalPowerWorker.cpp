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
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/LocalPowerWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangePowerThresholdMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangePowerAlertMessage.h"
#include "SystemMonitor/Local/PowerThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/PowerAlertLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>

/* brocade-system-monitor.yang supported-state bit none (5) */
#define SM_FRU_POWER_NONE 32
/* include/thresh/fwdPub.h supported FRU state no bits set */
#define FW_FRU_POWER_NONE 0
/* brocade-system-monitor.yang supported-state bit all (6) */
#define SM_FRU_POWER_ALL 64
/* include/thresh/fwdPub.h supported FRU state bit defs */
#define FW_FRU_POWER_ALL (FW_FRU_ABSENT|FW_FRU_INSERTED|FW_FRU_ON|FW_FRU_FAULTY)

namespace DcmNs
{

    LocalPowerWorker::LocalPowerWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        PowerThresholdLocalManagedObject    PowerThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        PowerAlertLocalManagedObject    PowerAlertLocalManagedObject    (pSystemMonitorLocalObjectManager);
        PowerThresholdLocalManagedObject.setupOrm ();
        addManagedClass (PowerThresholdLocalManagedObject::getClassName (), this);
        PowerAlertLocalManagedObject.setupOrm ();
        addManagedClass (PowerAlertLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGEPOWERTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalPowerWorker::SystemMonitorLocalChangePowerThresholdMessageHandler));
        addOperationMap (SYSTEMMONITORLOCALCHANGEPOWERALERT, reinterpret_cast<PrismMessageHandler> (&LocalPowerWorker::SystemMonitorLocalChangePowerAlertMessageHandler));
    }

    LocalPowerWorker::~LocalPowerWorker ()
    {
    }

    PrismMessage  *LocalPowerWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGEPOWERTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangePowerThresholdMessage ();
                break;
            case SYSTEMMONITORLOCALCHANGEPOWERALERT :
                pPrismMessage = new SystemMonitorLocalChangePowerAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalPowerWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PowerThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PowerThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PowerAlertLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PowerAlertLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalPowerWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
    UI32 LocalPowerWorker::fwtoDcmState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case FW_FRU_POWER_NONE:
                {
                    retState = SM_FRU_POWER_NONE;
                    break;
                }
            case FW_FRU_POWER_ALL:
                {
                    retState = SM_FRU_POWER_ALL;
                    break;
                }
        }
        return (retState);
    }
    
    UI32 LocalPowerWorker::fwtoDcmAction (UI32 actionValue)
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
   
    UI32 LocalPowerWorker::DcmtoFwState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case 0:
                retState = 1;
                break;
            case SM_FRU_POWER_NONE:
                retState = FW_FRU_POWER_NONE;
                break;
            case SM_FRU_POWER_ALL:
                retState = FW_FRU_POWER_ALL;
                break;
        }

        return (retState); 
    }
    
    UI32 LocalPowerWorker::DcmtoFwAction (UI32 actionValue)
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
    
    int LocalPowerWorker::fwBackendOpPowerThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPowerThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        PowerThresholdLocalManagedObject *pPowerThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, PowerThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalPowerWorker::fwBackendOpPowerThreshold No PowerThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_POWER_SUPPLIES) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal; 
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pPowerThreshMO = new PowerThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPowerThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Threshold MO exists");
            pPowerThreshMO  = dynamic_cast <PowerThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pPowerThreshMO->getDownthreshold();
            marginalTh    = pPowerThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPowerThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPowerThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower Get Failed") + ret);
            delete pPowerAlertMO;
            delete pResults;
            delete syncQueryPowerAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_POWER_SUPPLIES) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalPowerWorker::fwBackendOpPower Set Failed") +ret);
            delete pPowerAlertMO;
            delete pResults;
            delete syncQueryPowerAlert;
            return (ret);
        }
        */
        delete pPowerThreshMO;
        delete pResults;
        return (ret);
    }   
    int LocalPowerWorker::fwBackendOpPowerAlert (fw_fruObj_t *fruCfg)
    {
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Entering..");
        BitMap actionbitMap, statebitMap;
        int i = 0, ret = 0;
        UI32   actionVal = 1, stateVal = 1;
        UI32   dcmactionVal = 1, dcmstateVal = 1;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        PowerAlertLocalManagedObject *pPowerAlertMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, PowerAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalPowerWorker::fwBackendOpPower No PowerAlertMO");
            SystemMonitorLocalObjectManager *LOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < LOM->m_defFruCfg.fruNum; i++) {
                if (LOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_PS) {
                    stateVal = LOM->m_defFruCfg.defFruObj[i].alarm_state;
                    actionVal = LOM->m_defFruCfg.defFruObj[i].alarm_action;
                    dcmstateVal = fwtoDcmState (stateVal);
                    dcmactionVal = fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
                    break;
                }
            }
            startTransaction ();
            pPowerAlertMO = new PowerAlertLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 statebitMap, actionbitMap);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Alert MO exists");
            pPowerAlertMO  = dynamic_cast <PowerAlertLocalManagedObject *> ((*pResults)[0]);
            statebitMap  = pPowerAlertMO->getState();
            actionbitMap = pPowerAlertMO->getAction();
            stateVal     = DcmtoFwState (statebitMap.getElemValue ());
            actionVal    = DcmtoFwAction (actionbitMap.getElemValue ());
            trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower - StateVal") +stateVal);
            trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower - ActionVal") +actionVal);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower Get Failed") + ret);
            delete pPowerAlertMO;
            delete pResults;
            delete syncQueryPowerAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < fruCfg->fruNum; i++) {
            if (fruCfg->fruObj[i].fru_type == FW_FRU_PS) {
                trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower - StateVal") +stateVal);
                trace (TRACE_LEVEL_INFO, string ("LocalPowerWorker::fwBackendOpPower - ActionVal") +actionVal);
                fruCfg->fruObj[i].alarm_state = stateVal;
                fruCfg->fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalPowerWorker::fwBackendOpPower - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalPowerWorker::fwBackendOpPower Set Failed") +ret);
            delete pPowerAlertMO;
            delete pResults;
            delete syncQueryPowerAlert;
            return (ret);
        }
        */
        delete pPowerAlertMO;
        delete pResults;
        return (ret);
    }
    
    void  LocalPowerWorker::SystemMonitorLocalChangePowerAlertMessageHandler( SystemMonitorLocalChangePowerAlertMessage *pSystemMonitorLocalChangePowerAlertMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangePowerAlertMessageHandler..");
        if (pSystemMonitorLocalChangePowerAlertMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangePowerAlertMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }

        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangePowerAlertMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangePowerAlertMessage);
            return;           
        }
 
        int ret = -1;
        int i = 0;
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        BitMap actionBM = pSystemMonitorLocalChangePowerAlertMessage->getAction();
        BitMap stateBM = pSystemMonitorLocalChangePowerAlertMessage->getState();

        UI32    actionVal = actionBM.getElemValue();
        UI32    stateVal  = stateBM.getElemValue();
        UI32    dcmstateVal = 1, dcmactionVal = 1;

        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangePowerAlertMessageHandler:actionVal:") +actionVal);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangePowerAlertMessageHandler:stateVal:") +stateVal);
        
        /* Do All and None combination validation here */
        switch (actionVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_PS) {
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
                    trace (TRACE_LEVEL_ERROR, "SystemMonitorLocalChangePowerAlertMessageHandler: Not a supported action");
                    pSystemMonitorLocalChangePowerAlertMessage->setCompletionStatus
                        (WRC_SM_INVALID_ACTION_ERROR);
                    reply (pSystemMonitorLocalChangePowerAlertMessage);
                    return;
                }
        }
        

        if ( ((stateVal > SM_FRU_POWER_NONE) &&
              (stateVal != SM_FRU_POWER_ALL)) ||
             (stateVal > SM_FRU_POWER_ALL))
        {
            trace (TRACE_LEVEL_ERROR, "SystemMonitorLocalChangePowerAlertMessageHandler: Not a supported state");
            pSystemMonitorLocalChangePowerAlertMessage->setCompletionStatus
                (WRC_SM_INVALID_STATE_ERROR);
            reply (pSystemMonitorLocalChangePowerAlertMessage);
            return;
            
        }

        switch (stateVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_PS) {
                            stateVal = lOM->m_defFruCfg.defFruObj[i].alarm_state;
                            dcmstateVal = fwtoDcmState (stateVal);
                            stateBM.setAllValues (dcmstateVal);
                            break;
                        }
                    }
                }
                break;
            case SM_FRU_POWER_NONE:
                stateVal = FW_FRU_POWER_NONE;
                break;
            case SM_FRU_POWER_ALL:
                stateVal = FW_FRU_POWER_ALL;
                break;
        }
       /* Do FW Get and Set here */
        ret = fw_fru_cfg_get_all (&fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangePowerAlertMessageHandler: Get Failed") +ret);
            pSystemMonitorLocalChangePowerAlertMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangePowerAlertMessage);
            return;
        }
        for (i = 0; i < fruCfg.fruNum; i++) {
            if (fruCfg.fruObj[i].fru_type == FW_FRU_PS) {
                fruCfg.fruObj[i].alarm_state = stateVal;
                fruCfg.fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        ret = fw_fru_cfg_set_all (fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("SystemMonitorLocalChangePowerAlertMessageHandler: Get Failed") +ret);
            pSystemMonitorLocalChangePowerAlertMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangePowerAlertMessage);
            return;
        }
        
        /* Update Local MO */
        PowerAlertLocalManagedObject *pPowerAlertLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, PowerAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such Power Alert managed object");
            pSystemMonitorLocalChangePowerAlertMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangePowerAlertMessage);
            return;
        }
        pPowerAlertLMO = dynamic_cast <PowerAlertLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        pPowerAlertLMO->setState(stateBM);
        pPowerAlertLMO->setAction(actionBM);
        updateWaveManagedObject (pPowerAlertLMO);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangePowerAlertMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }

		delete (pPowerAlertLMO);
        pSystemMonitorLocalChangePowerAlertMessage->setCompletionStatus
            (status);
        reply (pSystemMonitorLocalChangePowerAlertMessage);
    }

    void  LocalPowerWorker::SystemMonitorLocalChangePowerThresholdMessageHandler( SystemMonitorLocalChangePowerThresholdMessage *pSystemMonitorLocalChangePowerThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangePowerThresholdMessageHandler..");
        if (pSystemMonitorLocalChangePowerThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangePowerThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangePowerThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangePowerThresholdMessage);
            return;           
        }
 
        int ret = -1, isChanged = -1;
        unsigned int th    = 0;
        int downThresh     = pSystemMonitorLocalChangePowerThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangePowerThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangePowerThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_POWER_SUPPLIES) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangePowerThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_POWER_SUPPLIES) {
                    marginalThresh = lOM->m_defpList.defStatPolicy[th].marginal.marginal; 
                    break;
                }
            }
        }
        
         /* Do Fabric Watch validation here */
        swPolicyList_t pList;
        memset (&pList, 0, sizeof(pList));
        ret = switchStatusPolicyGetData(&pList);
        if (ret == -1) {
            tracePrintf(TRACE_LEVEL_INFO, "SystemMonitorLocalChangePowerThresholdMessageHandler Get Failed %x",ret);
            pSystemMonitorLocalChangePowerThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangePowerThresholdMessage);
            return;
        }
        
        if (((UI32)downThresh > pList.ps) || ((UI32)marginalThresh > pList.ps) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangePowerThresholdMessageHandler:Thresh Validation Failed") +pList.ps);
            pSystemMonitorLocalChangePowerThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangePowerThresholdMessage);
            return;
        }

        unsigned int i = 0;
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_POWER_SUPPLIES) {
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangePowerThresholdMessageHandler :Set Failed" +isChanged);
            pSystemMonitorLocalChangePowerThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangePowerThresholdMessage);
            return;
        }

        /* Update Local MO */
        PowerThresholdLocalManagedObject *pPowerThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, PowerThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such Power threshold managed object");
            pSystemMonitorLocalChangePowerThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangePowerThresholdMessage);
            return;
        }
        pPowerThreshLMO = dynamic_cast <PowerThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pPowerThreshLMO);
        pPowerThreshLMO->setDownthreshold(downThresh);
        pPowerThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangePowerThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete (pPowerThreshLMO);
        pSystemMonitorLocalChangePowerThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangePowerThresholdMessage);
    }

}
