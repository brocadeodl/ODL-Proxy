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
#include "SystemMonitor/Local/LocalLineCardWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeLineCardThresholdMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeLineCardAlertMessage.h"
#include "SystemMonitor/Local/LineCardThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/LineCardAlertLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalLineCardWorker::LocalLineCardWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        LineCardThresholdLocalManagedObject    LineCardThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        LineCardAlertLocalManagedObject    LineCardAlertLocalManagedObject    (pSystemMonitorLocalObjectManager);
        LineCardThresholdLocalManagedObject.setupOrm ();
        addManagedClass (LineCardThresholdLocalManagedObject::getClassName (), this);
        LineCardAlertLocalManagedObject.setupOrm ();
        addManagedClass (LineCardAlertLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGELINECARDTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalLineCardWorker::SystemMonitorLocalChangeLineCardThresholdMessageHandler));
        addOperationMap (SYSTEMMONITORLOCALCHANGELINECARDALERT, reinterpret_cast<PrismMessageHandler> (&LocalLineCardWorker::SystemMonitorLocalChangeLineCardAlertMessageHandler));
    }

    LocalLineCardWorker::~LocalLineCardWorker ()
    {
    }

    PrismMessage  *LocalLineCardWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGELINECARDTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeLineCardThresholdMessage ();
                break;
            case SYSTEMMONITORLOCALCHANGELINECARDALERT :
                pPrismMessage = new SystemMonitorLocalChangeLineCardAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalLineCardWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((LineCardThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new LineCardThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((LineCardAlertLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new LineCardAlertLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalLineCardWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
    
    UI32 LocalLineCardWorker::fwtoDcmState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case 0:
                {
                    retState = 32;
                    break;
                }
            case 31:
                {
                    retState = 64;
                    break;
                }
        }
        return (retState);
    }
    
    UI32 LocalLineCardWorker::fwtoDcmAction (UI32 actionValue)
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
   
    UI32 LocalLineCardWorker::DcmtoFwState (UI32 stateValue)
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
                retState = 31;
                break;
        }

        return (retState); 
    }
    
    UI32 LocalLineCardWorker::DcmtoFwAction (UI32 actionValue)
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
    
    int LocalLineCardWorker::fwBackendOpLineCardThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCardThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        LineCardThresholdLocalManagedObject *pLineCardThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, LineCardThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalLineCardWorker::fwBackendOpLineCardThreshold No LineCardThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_BLADE) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pLineCardThreshMO = new LineCardThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCardThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Threshold MO exists");
            pLineCardThreshMO  = dynamic_cast <LineCardThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pLineCardThreshMO->getDownthreshold();
            marginalTh    = pLineCardThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCardThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCardThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard Get Failed") + ret);
            delete pLineCardAlertMO;
            delete pResults;
            delete syncQueryLineCardAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_BLADE) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalLineCardWorker::fwBackendOpLineCard Set Failed") +ret);
            delete pLineCardAlertMO;
            delete pResults;
            delete syncQueryLineCardAlert;
            return (ret);
        }
        */
        delete pLineCardThreshMO;
        delete pResults;
        return (ret);
    }   
    int LocalLineCardWorker::fwBackendOpLineCardAlert (fw_fruObj_t *fruCfg)
    {
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Entering..");
        BitMap actionbitMap, statebitMap;
        int i = 0, ret = 0;
        UI32   actionVal = 1, stateVal = 1;
        UI32   dcmactionVal = 1, dcmstateVal = 1;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        LineCardAlertLocalManagedObject *pLineCardAlertMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, LineCardAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalLineCardWorker::fwBackendOpLineCard No LineCardAlertMO");
            SystemMonitorLocalObjectManager *LOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < LOM->m_defFruCfg.fruNum; i++) {
                if (LOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_SLOT) {
                    actionVal = LOM->m_defFruCfg.defFruObj[i].alarm_action;
                    stateVal  = LOM->m_defFruCfg.defFruObj[i].alarm_state;
                    dcmstateVal = fwtoDcmState (stateVal);
                    dcmactionVal = fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
                    break;
                }
            }

			/* 
			 * Setting default state and action values as none when the fru is
			 * not supported on the platform.
			 */
			if (LOM->m_defFruCfg.defFruObj[FW_FRU_SLOT].fru_type != FW_FRU_SLOT) {
				statebitMap.setAllValues (32);
				actionbitMap.setAllValues (1);
			}
            startTransaction ();
            pLineCardAlertMO = new LineCardAlertLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 statebitMap, actionbitMap);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Alert MO exists");
            pLineCardAlertMO  = dynamic_cast <LineCardAlertLocalManagedObject *> ((*pResults)[0]);
            statebitMap  = pLineCardAlertMO->getState();
            actionbitMap = pLineCardAlertMO->getAction();
            stateVal     = DcmtoFwState (statebitMap.getElemValue ());
            actionVal    = DcmtoFwAction (actionbitMap.getElemValue ());
            trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard - StateVal") +stateVal);
            trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard - ActionVal") +actionVal);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard Get Failed") + ret);
            delete pLineCardAlertMO;
            delete pResults;
            delete syncQueryLineCardAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < fruCfg->fruNum; i++) {
            if (fruCfg->fruObj[i].fru_type == FW_FRU_SLOT) {
                trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard - StateVal") +stateVal);
                trace (TRACE_LEVEL_INFO, string ("LocalLineCardWorker::fwBackendOpLineCard - ActionVal") +actionVal);
                fruCfg->fruObj[i].alarm_state = stateVal;
                fruCfg->fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::fwBackendOpLineCard - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalLineCardWorker::fwBackendOpLineCard Set Failed") +ret);
            delete pLineCardAlertMO;
            delete pResults;
            delete syncQueryLineCardAlert;
            return (ret);
        }
        */
        delete pLineCardAlertMO;
        delete pResults;
        return (ret);
    }
    void LocalLineCardWorker::externalStateSynchronization (
            WaveAsynchronousContextForExternalStateSynchronization
            *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        /*
        int ret = 0;
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 10)
                && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {
            trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::externalStateSynchronization Entering... ");
            sleep (10);
            //ret = fwBackendOpLineCard ();
            if (ret != 0)
            {
                trace (TRACE_LEVEL_ERROR, "LocalLineCardWorker::ESS - fwBackendOpLineCard Failed");
            }
        }
        */
        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();

    }
    void LocalLineCardWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        /*
       trace (TRACE_LEVEL_INFO, "Entering LocalLineCardWorker::install");
		ResourceId                      status      = WAVE_MESSAGE_SUCCESS;
        BitMap                          actionbitMap, statebitMap;
        LineCardThresholdLocalManagedObject  *pLineCardThreshMO = NULL;
        LineCardAlertLocalManagedObject      *pLineCardAlertMO = NULL;
        actionbitMap.setAllValues(1);
        statebitMap.setAllValues(1);
        startTransaction ();
        pLineCardThreshMO = new LineCardThresholdLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             3, 3);
        pLineCardAlertMO = new LineCardAlertLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             statebitMap, actionbitMap);
 
        status = commitTransaction();
        if (status == FRAMEWORK_SUCCESS)
            trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::install - Commit Success");
        delete pLineCardThreshMO;
        delete pLineCardAlertMO;
        */
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }
    
    void  LocalLineCardWorker::SystemMonitorLocalChangeLineCardAlertMessageHandler( SystemMonitorLocalChangeLineCardAlertMessage *pSystemMonitorLocalChangeLineCardAlertMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeLineCardAlertMessageHandler..");
        if (pSystemMonitorLocalChangeLineCardAlertMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeLineCardAlertMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeLineCardAlertMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeLineCardAlertMessage);
            return;           
        }
 
        int ret = -1;
        int i = 0;
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        BitMap actionBM = pSystemMonitorLocalChangeLineCardAlertMessage->getAction();
        BitMap stateBM = pSystemMonitorLocalChangeLineCardAlertMessage->getState();

        UI32    actionVal = actionBM.getElemValue();
        UI32    stateVal = stateBM.getElemValue();
        UI32    dcmactionVal = 1, dcmstateVal = 1;

        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardAlertMessageHandler:actionVal:") +actionVal);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardAlertMessageHandler:stateVal:") +stateVal);
        
        /* Do All and None combination validation here */
        switch (actionVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_SLOT) {
                            actionVal = lOM->m_defFruCfg.defFruObj[i].alarm_action;
                            dcmactionVal = fwtoDcmAction (actionVal);
                            actionBM.setAllValues (dcmactionVal);
                            break;
                        }
                    }
					
					if (lOM->m_defFruCfg.defFruObj[FW_FRU_SLOT].fru_type != FW_FRU_SLOT) {
						actionBM.setAllValues (1);
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
                    trace (TRACE_LEVEL_ERROR, "LocalLineCardWorker::SystemMonitorLocalChangeLineCardAlertMessageHandler Not a supported action");
                    pSystemMonitorLocalChangeLineCardAlertMessage->setCompletionStatus
                        (WRC_SM_INVALID_ACTION_ERROR);
                    reply (pSystemMonitorLocalChangeLineCardAlertMessage);
                    return;
                }
        }
        

        if ( ((stateVal > 32) && (stateVal != 64)) || (stateVal > 64))
        {
            trace (TRACE_LEVEL_ERROR, "LocalLineCardWorker::SystemMonitorLocalChangeLineCardAlertMessageHandler Not a supported state");
            pSystemMonitorLocalChangeLineCardAlertMessage->setCompletionStatus
                (WRC_SM_INVALID_STATE_ERROR);
            reply (pSystemMonitorLocalChangeLineCardAlertMessage);
            return;
            
        }

        switch (stateVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_SLOT) {
                            stateVal = lOM->m_defFruCfg.defFruObj[i].alarm_state;
                            dcmstateVal = fwtoDcmState (stateVal);
                            stateBM.setAllValues (dcmstateVal);
                            break;
                        }
                    }

					if (lOM->m_defFruCfg.defFruObj[FW_FRU_SLOT].fru_type !=	FW_FRU_SLOT) {
						stateBM.setAllValues (32);
					}
					break;
				}
            case 32:
                stateVal = 0;
                break;
            case 64:
                stateVal = 31;
                break;
        }
       /* Do FW Get and Set here */
        ret = fw_fru_cfg_get_all (&fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, "LocalLineCardWorker::SystemMonitorLocalChangeLineCardAlertMessageHandler Get Failed" +ret);
            pSystemMonitorLocalChangeLineCardAlertMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeLineCardAlertMessage);
            return;
        }
        
        for (i = 0; i < fruCfg.fruNum; i++) {
            if (fruCfg.fruObj[i].fru_type == FW_FRU_SLOT) {
                fruCfg.fruObj[i].alarm_state = stateVal;
                fruCfg.fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        
        ret = fw_fru_cfg_set_all (fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("LocalLineCardWorker::SystemMonitorLocalChangeLineCardAlertMessageHandler Set Failed") +ret);
            pSystemMonitorLocalChangeLineCardAlertMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeLineCardAlertMessage);
            return;
        }
        /* Update Local MO */
        LineCardAlertLocalManagedObject *pLineCardAlertLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, LineCardAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "LocalLineCardWorker::SystemMonitorLocalChangeLineCardAlertMessageHandler No such LineCard Alert managed object");
            pSystemMonitorLocalChangeLineCardAlertMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeLineCardAlertMessage);
            return;
        }
        pLineCardAlertLMO = dynamic_cast <LineCardAlertLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        pLineCardAlertLMO->setState(stateBM);
        pLineCardAlertLMO->setAction(actionBM);
        updateWaveManagedObject (pLineCardAlertLMO);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeLineCardAlertMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pLineCardAlertLMO;

        pSystemMonitorLocalChangeLineCardAlertMessage->setCompletionStatus
            (status);
        reply (pSystemMonitorLocalChangeLineCardAlertMessage);
    }

    void  LocalLineCardWorker::SystemMonitorLocalChangeLineCardThresholdMessageHandler( SystemMonitorLocalChangeLineCardThresholdMessage *pSystemMonitorLocalChangeLineCardThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeLineCardThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeLineCardThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeLineCardThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeLineCardThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeLineCardThresholdMessage);
            return;           
        }
 
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        unsigned int i = 0;
        int downThresh     = pSystemMonitorLocalChangeLineCardThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeLineCardThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeLineCardThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_BLADE) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeLineCardThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_BLADE) {
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
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler : Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler : Get Failed") +isChanged);
            pSystemMonitorLocalChangeLineCardThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeLineCardThresholdMessage);
            return;
        }

        if (((UI32)downThresh > pList.bladeNum) || ((UI32)marginalThresh > pList.bladeNum) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler : Invalid Thresh"));
            pSystemMonitorLocalChangeLineCardThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeLineCardThresholdMessage);
            return;
        }

	    trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler : Down threshold") +downThresh);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler : Marginal thresho") +marginalThresh);
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_BLADE) {
	            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler : Inside Down threshold") +downThresh);
            	trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler : Inside Marginal thresho") +marginalThresh);
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeLineCardThresholdMessageHandler :Set Failed" )+ret);
            pSystemMonitorLocalChangeLineCardThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeLineCardThresholdMessage);
            return;
        }

        /* Update Local MO */
        LineCardThresholdLocalManagedObject *pLineCardThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, LineCardThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "SystemMonitorLocalChangeLineCardThresholdMessageHandler : No such LineCard threshold managed object");
            pSystemMonitorLocalChangeLineCardThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeLineCardThresholdMessage);
            return;
        }
        pLineCardThreshLMO = dynamic_cast <LineCardThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pLineCardThreshLMO);
        pLineCardThreshLMO->setDownthreshold(downThresh);
        pLineCardThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeLineCardThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pLineCardThreshLMO;
        pSystemMonitorLocalChangeLineCardThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeLineCardThresholdMessage);
    }

}
