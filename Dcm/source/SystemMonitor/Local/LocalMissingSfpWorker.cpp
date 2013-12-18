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
#include "SystemMonitor/Local/LocalMissingSfpWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeMissingSfpThresholdMessage.h"
#include "SystemMonitor/Local/MissingSfpThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalMissingSfpWorker::LocalMissingSfpWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        MissingSfpThresholdLocalManagedObject    MissingSfpThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        MissingSfpThresholdLocalManagedObject.setupOrm ();
        addManagedClass (MissingSfpThresholdLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGEMISSINGSFPTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalMissingSfpWorker::SystemMonitorLocalChangeMissingSfpThresholdMessageHandler));
    }

    LocalMissingSfpWorker::~LocalMissingSfpWorker ()
    {
    }

    PrismMessage  *LocalMissingSfpWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGEMISSINGSFPTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeMissingSfpThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalMissingSfpWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((MissingSfpThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MissingSfpThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalMissingSfpWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
    
    UI32 LocalMissingSfpWorker::fwtoDcmState (UI32 stateValue)
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
    
    UI32 LocalMissingSfpWorker::fwtoDcmAction (UI32 actionValue)
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
   
    UI32 LocalMissingSfpWorker::DcmtoFwState (UI32 stateValue)
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
    
    UI32 LocalMissingSfpWorker::DcmtoFwAction (UI32 actionValue)
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
    
    int LocalMissingSfpWorker::fwBackendOpMissingSfpThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::fwBackendOpMissingSfpThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        MissingSfpThresholdLocalManagedObject *pMissingSfpThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MissingSfpThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalMissingSfpWorker::fwBackendOpMissingSfpThreshold No MissingSfpThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_MISSING_GBICS) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pMissingSfpThreshMO = new MissingSfpThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::fwBackendOpMissingSfpThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::fwBackendOpMissingSfp - Threshold MO exists");
            pMissingSfpThreshMO  = dynamic_cast <MissingSfpThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pMissingSfpThreshMO->getDownthreshold();
            marginalTh    = pMissingSfpThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalMissingSfpWorker::fwBackendOpMissingSfpThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalMissingSfpWorker::fwBackendOpMissingSfpThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::fwBackendOpFan - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalMissingSfpWorker::fwBackendOpFan - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalMissingSfpWorker::fwBackendOpFan Get Failed") + ret);
            delete pMissingSfpAlertMO;
            delete pResults;
            delete syncQueryMissingSfpAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_MISSING_GBICS) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::fwBackendOpFan - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::fwBackendOpFan - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalMissingSfpWorker::fwBackendOpFan Set Failed") +ret);
            delete pMissingSfpAlertMO;
            delete pResults;
            delete syncQueryMissingSfpAlert;
            return (ret);
        }
        */
        delete pMissingSfpThreshMO;
        delete pResults;
        return (ret);
    }   

    void LocalMissingSfpWorker::externalStateSynchronization (
            WaveAsynchronousContextForExternalStateSynchronization
            *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        /*
        int ret = 0;
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 10)
                && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {
            trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::externalStateSynchronization Entering... ");
            sleep (10);
            //ret = fwBackendOpMissingSfp ();
            if (ret != 0)
            {
                trace (TRACE_LEVEL_ERROR, "LocalMissingSfpWorker::ESS - fwBackendOpFan Failed");
            }
        }
        */
        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();

    }
    void LocalMissingSfpWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        /*
       trace (TRACE_LEVEL_INFO, "Entering LocalMissingSfpWorker::install");
		ResourceId                      status      = WAVE_MESSAGE_SUCCESS;
        BitMap                          actionbitMap, statebitMap;
        MissingSfpThresholdLocalManagedObject  *pFanThreshMO = NULL;
        MissingSfpAlertLocalManagedObject      *pFanAlertMO = NULL;
        actionbitMap.setAllValues(1);
        statebitMap.setAllValues(1);
        startTransaction ();
        pMissingSfpThreshMO = new FanThresholdLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             3, 3);
        pMissingSfpAlertMO = new FanAlertLocalManagedObject
            (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
             statebitMap, actionbitMap);
 
        status = commitTransaction();
        if (status == FRAMEWORK_SUCCESS)
            trace (TRACE_LEVEL_INFO, "LocalMissingSfpWorker::install - Commit Success");
        delete pMissingSfpThreshMO;
        delete pMissingSfpAlertMO;
        */
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }
    

    void  LocalMissingSfpWorker::SystemMonitorLocalChangeMissingSfpThresholdMessageHandler( SystemMonitorLocalChangeMissingSfpThresholdMessage *pSystemMonitorLocalChangeMissingSfpThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeMissingSfpThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeMissingSfpThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeMissingSfpThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeMissingSfpThresholdMessage);
            return;           
        }
 
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        unsigned int i = 0;
        int downThresh     = pSystemMonitorLocalChangeMissingSfpThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeMissingSfpThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_MISSING_GBICS) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_MISSING_GBICS) {
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
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Get Failed") +isChanged);
            pSystemMonitorLocalChangeMissingSfpThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeMissingSfpThresholdMessage);
            return;
        }

        if (((UI32)downThresh > pList.picNum) || ((UI32)marginalThresh > pList.picNum) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Invalid Thresh"));
            pSystemMonitorLocalChangeMissingSfpThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeMissingSfpThresholdMessage);
            return;
        }

	    trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Down threshold") +downThresh);
        trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Marginal thresho") +marginalThresh);
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_MISSING_GBICS) {
	            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Inside Down threshold") +downThresh);
            	trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Inside Marginal thresho") +marginalThresh);
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMissingSfpThresholdMessageHandler :Set Failed" )+ret);
            pSystemMonitorLocalChangeMissingSfpThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeMissingSfpThresholdMessage);
            return;
        }

        /* Update Local MO */
        MissingSfpThresholdLocalManagedObject *pMissingSfpThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MissingSfpThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : No such MissingSfp threshold managed object");
            pSystemMonitorLocalChangeMissingSfpThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeMissingSfpThresholdMessage);
            return;
        }
        pMissingSfpThreshLMO = dynamic_cast <MissingSfpThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pMissingSfpThreshLMO);
        pMissingSfpThreshLMO->setDownthreshold(downThresh);
        pMissingSfpThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeMissingSfpThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
        pSystemMonitorLocalChangeMissingSfpThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeMissingSfpThresholdMessage);
    }

}
