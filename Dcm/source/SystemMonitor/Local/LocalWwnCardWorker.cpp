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
#include "SystemMonitor/Local/LocalWwnCardWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeWwnCardThresholdMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeWwnCardAlertMessage.h"
#include "SystemMonitor/Local/WwnCardThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/WwnCardAlertLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalWwnCardWorker::LocalWwnCardWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        WwnCardThresholdLocalManagedObject    WwnCardThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        WwnCardAlertLocalManagedObject    WwnCardAlertLocalManagedObject    (pSystemMonitorLocalObjectManager);
        WwnCardThresholdLocalManagedObject.setupOrm ();
        addManagedClass (WwnCardThresholdLocalManagedObject::getClassName (), this);
        WwnCardAlertLocalManagedObject.setupOrm ();
        addManagedClass (WwnCardAlertLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGEWWNCARDTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardThresholdMessageHandler));
        addOperationMap (SYSTEMMONITORLOCALCHANGEWWNCARDALERT, reinterpret_cast<PrismMessageHandler> (&LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardAlertMessageHandler));
    }

    LocalWwnCardWorker::~LocalWwnCardWorker ()
    {
    }

    PrismMessage  *LocalWwnCardWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGEWWNCARDTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeWwnCardThresholdMessage ();
                break;
            case SYSTEMMONITORLOCALCHANGEWWNCARDALERT :
                pPrismMessage = new SystemMonitorLocalChangeWwnCardAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalWwnCardWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((WwnCardThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new WwnCardThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((WwnCardAlertLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new WwnCardAlertLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalWwnCardWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
    UI32 LocalWwnCardWorker::fwtoDcmState (UI32 stateValue)
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
    
    UI32 LocalWwnCardWorker::fwtoDcmAction (UI32 actionValue)
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
   
    UI32 LocalWwnCardWorker::DcmtoFwState (UI32 stateValue)
    {
        UI32 retState = stateValue;
        
        switch (stateValue) {
            case 0:
                retState = 3;
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
    
    UI32 LocalWwnCardWorker::DcmtoFwAction (UI32 actionValue)
    {
        UI32 retAction = actionValue;
        
        switch (actionValue) {
            case 0:
                {
                    retAction = 3;
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
    
    int LocalWwnCardWorker::fwBackendOpWwnCardThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCardThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        WwnCardThresholdLocalManagedObject *pWwnCardThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, WwnCardThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalWwnCardWorker::fwBackendOpWwnCardThreshold No WwnCardThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_WWN) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal; 
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pWwnCardThreshMO = new WwnCardThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCardThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Threshold MO exists");
            pWwnCardThreshMO  = dynamic_cast <WwnCardThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pWwnCardThreshMO->getDownthreshold();
            marginalTh    = pWwnCardThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCardThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCardThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard Get Failed") + ret);
            delete pWwnCardAlertMO;
            delete pResults;
            delete syncQueryWwnCardAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_WWN) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalWwnCardWorker::fwBackendOpWwnCard Set Failed") +ret);
            delete pWwnCardAlertMO;
            delete pResults;
            delete syncQueryWwnCardAlert;
            return (ret);
        }
        */
        delete pWwnCardThreshMO;
        delete pResults;
        return (ret);
    }   
    int LocalWwnCardWorker::fwBackendOpWwnCardAlert (fw_fruObj_t *fruCfg)
    {
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Entering..");
        BitMap actionbitMap, statebitMap;
        int i = 0, ret = 0;
        UI32   actionVal = 1, stateVal = 1;
        UI32   dcmactionVal = 1, dcmstateVal = 1;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        WwnCardAlertLocalManagedObject *pWwnCardAlertMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, WwnCardAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalWwnCardWorker::fwBackendOpWwnCard No WwnCardAlertMO");
            SystemMonitorLocalObjectManager *LOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < LOM->m_defFruCfg.fruNum; i++) {
                if (LOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_WWN) {
                    stateVal = LOM->m_defFruCfg.defFruObj[i].alarm_state;
                    actionVal = LOM->m_defFruCfg.defFruObj[i].alarm_action;
                    dcmstateVal = fwtoDcmState (stateVal);
                    dcmactionVal = fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
                    break;
                }
            }

			/* 
			 * Setting default state and action values as none
			 * when the fru is not supported on the platform.
			 */
			if (LOM->m_defFruCfg.defFruObj[FW_FRU_WWN].fru_type != FW_FRU_WWN) {
				statebitMap.setAllValues (32);
				actionbitMap.setAllValues (1);
			}
			
            startTransaction ();
            pWwnCardAlertMO = new WwnCardAlertLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 statebitMap, actionbitMap);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Alert MO exists");
            pWwnCardAlertMO  = dynamic_cast <WwnCardAlertLocalManagedObject *> ((*pResults)[0]);
            statebitMap  = pWwnCardAlertMO->getState();
            actionbitMap = pWwnCardAlertMO->getAction();
            stateVal     = DcmtoFwState (statebitMap.getElemValue ());
            actionVal    = DcmtoFwAction (actionbitMap.getElemValue ());
            trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard - StateVal") +stateVal);
            trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard - ActionVal") +actionVal);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard Get Failed") + ret);
            delete pWwnCardAlertMO;
            delete pResults;
            delete syncQueryWwnCardAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < fruCfg->fruNum; i++) {
            if (fruCfg->fruObj[i].fru_type == FW_FRU_WWN) {
                trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard - StateVal") +stateVal);
                trace (TRACE_LEVEL_INFO, string ("LocalWwnCardWorker::fwBackendOpWwnCard - ActionVal") +actionVal);
                fruCfg->fruObj[i].alarm_state = stateVal;
                fruCfg->fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalWwnCardWorker::fwBackendOpWwnCard - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalWwnCardWorker::fwBackendOpWwnCard Set Failed") +ret);
            delete pWwnCardAlertMO;
            delete pResults;
            delete syncQueryWwnCardAlert;
            return (ret);
        }
        */
        delete pWwnCardAlertMO;
        delete pResults;
        return (ret);
    }
    void  LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardAlertMessageHandler( SystemMonitorLocalChangeWwnCardAlertMessage *pSystemMonitorLocalChangeWwnCardAlertMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeWwnCardAlertMessageHandler..");
        if (pSystemMonitorLocalChangeWwnCardAlertMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeWwnCardAlertMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeWwnCardAlertMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardAlertMessage);
            return;           
        }
 
        int ret = -1;
        int i = 0;
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        BitMap actionBM = pSystemMonitorLocalChangeWwnCardAlertMessage->getAction();
        BitMap stateBM = pSystemMonitorLocalChangeWwnCardAlertMessage->getState();

        UI32    actionVal = actionBM.getElemValue();
        UI32    stateVal = stateBM.getElemValue();
        UI32    dcmactionVal = 1, dcmstateVal = 1;

        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeWwnCardAlertMessageHandler:actionVal:") +actionVal);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeWwnCardAlertMessageHandler:stateVal:") +stateVal);
        trace (TRACE_LEVEL_DEBUG, string("SystemMonitorLocalChangeWwnCardAlertMessageHandler:i:") +i);
        
        /* Do All and None combination validation here */
        switch (actionVal) {
            case 0:
                {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_WWN) {
                            actionVal = lOM->m_defFruCfg.defFruObj[i].alarm_action;
                            dcmactionVal = fwtoDcmAction (actionVal);
                            actionBM.setAllValues (dcmactionVal);
                            break;
                        }
                    }

					if (lOM->m_defFruCfg.defFruObj[FW_FRU_WWN].fru_type != FW_FRU_WWN) {
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
                    trace (TRACE_LEVEL_ERROR, "LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardAlertMessageHandler Not a supported action");
                    pSystemMonitorLocalChangeWwnCardAlertMessage->setCompletionStatus
                        (WRC_SM_INVALID_ACTION_ERROR);
                    reply (pSystemMonitorLocalChangeWwnCardAlertMessage);
                    return;
                }
        }
        

        if ( ((stateVal > 32) && (stateVal != 64)) || (stateVal > 64))
        {
            trace (TRACE_LEVEL_ERROR, "LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardAlertMessageHandler Not a supported state");
            pSystemMonitorLocalChangeWwnCardAlertMessage->setCompletionStatus
                (WRC_SM_INVALID_STATE_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardAlertMessage);
            return;
            
        }

        switch (stateVal) {
            case 0:
                 {
                    for (i = 0; i < lOM->m_defFruCfg.fruNum; i++) {
                        if (lOM->m_defFruCfg.defFruObj[i].fru_type == FW_FRU_WWN) {
                            stateVal = lOM->m_defFruCfg.defFruObj[i].alarm_state;
                            dcmstateVal = fwtoDcmState (stateVal);
                            stateBM.setAllValues (dcmstateVal);
                            break;
                        }
                    }

					if (lOM->m_defFruCfg.defFruObj[FW_FRU_WWN].fru_type != FW_FRU_WWN) {
						stateBM.setAllValues(32);
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
            trace (TRACE_LEVEL_INFO, string("LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardAlertMessageHandler Get Failed") +ret);
            pSystemMonitorLocalChangeWwnCardAlertMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardAlertMessage);
            return;
        }
        
        for (i = 0; i < fruCfg.fruNum; i++) {
            if (fruCfg.fruObj[i].fru_type == FW_FRU_WWN) {
                fruCfg.fruObj[i].alarm_state = stateVal;
                fruCfg.fruObj[i].alarm_action = actionVal;
                break;
            }
        }
        
        ret = fw_fru_cfg_set_all (fruCfg);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardAlertMessageHandler Get Failed") +ret);
            pSystemMonitorLocalChangeWwnCardAlertMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardAlertMessage);
            return;
        }
        
        /* Update Local MO */
        WwnCardAlertLocalManagedObject *pWwnCardAlertLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, WwnCardAlertLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such WwnCard Alert managed object");
            pSystemMonitorLocalChangeWwnCardAlertMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardAlertMessage);
            return;
        }
        pWwnCardAlertLMO = dynamic_cast <WwnCardAlertLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        pWwnCardAlertLMO->setState(stateBM);
        pWwnCardAlertLMO->setAction(actionBM);
        updateWaveManagedObject (pWwnCardAlertLMO);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeWwnCardAlertMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pWwnCardAlertLMO;
        pSystemMonitorLocalChangeWwnCardAlertMessage->setCompletionStatus
            (status);
        reply (pSystemMonitorLocalChangeWwnCardAlertMessage);
    }

    void  LocalWwnCardWorker::SystemMonitorLocalChangeWwnCardThresholdMessageHandler( SystemMonitorLocalChangeWwnCardThresholdMessage *pSystemMonitorLocalChangeWwnCardThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeWwnCardThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeWwnCardThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeWwnCardThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeWwnCardThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardThresholdMessage);
            return;           
        }
 
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        int downThresh     = pSystemMonitorLocalChangeWwnCardThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeWwnCardThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeWwnCardThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_WWN) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeWwnCardThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_WWN) {
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
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeWwnCardThresholdMessageHandler Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeWwnCardThresholdMessageHandler Get Failed") +isChanged);
            pSystemMonitorLocalChangeWwnCardThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardThresholdMessage);
            return;
        }
        if (((UI32)downThresh > pList.wwnNum) || ((UI32)marginalThresh > pList.wwnNum) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeWwnCardThresholdMessageHandler : Get Failed") +ret);
            pSystemMonitorLocalChangeWwnCardThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardThresholdMessage);
            return;
        }


        unsigned int i = 0;
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_WWN) {
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeWwnCardThresholdMessageHandler Set Failed") +ret);
            pSystemMonitorLocalChangeWwnCardThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardThresholdMessage);
            return;
        }

        /* Update Local MO */
        WwnCardThresholdLocalManagedObject *pWwnCardThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, WwnCardThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such WwnCard threshold managed object");
            pSystemMonitorLocalChangeWwnCardThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeWwnCardThresholdMessage);
            return;
        }
        pWwnCardThreshLMO = dynamic_cast <WwnCardThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pWwnCardThreshLMO);
        pWwnCardThreshLMO->setDownthreshold(downThresh);
        pWwnCardThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeWwnCardThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pWwnCardThreshLMO;
        pSystemMonitorLocalChangeWwnCardThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeWwnCardThresholdMessage);
    }

}
