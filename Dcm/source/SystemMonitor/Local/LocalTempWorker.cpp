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
#include "SystemMonitor/Local/LocalTempWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeTempThresholdMessage.h"
#include "SystemMonitor/Local/TempThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalTempWorker::LocalTempWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        TempThresholdLocalManagedObject    TempThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        TempThresholdLocalManagedObject.setupOrm ();
        addManagedClass (TempThresholdLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGETEMPTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalTempWorker::SystemMonitorLocalChangeTempThresholdMessageHandler));
    }

    LocalTempWorker::~LocalTempWorker ()
    {
    }

    PrismMessage  *LocalTempWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGETEMPTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeTempThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalTempWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((TempThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new TempThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
       else
        {
            trace (TRACE_LEVEL_FATAL, "LocalTempWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
    
    int LocalTempWorker::fwBackendOpTempThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalTempWorker::fwBackendOpTempThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        TempThresholdLocalManagedObject *pTempThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, TempThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalTempWorker::fwBackendOpTempThreshold No TempThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_TEMPERATURES) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal; 
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pTempThreshMO = new TempThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalTempWorker::fwBackendOpTempThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalTempWorker::fwBackendOpTemp - Threshold MO exists");
            pTempThreshMO  = dynamic_cast <TempThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pTempThreshMO->getDownthreshold();
            marginalTh    = pTempThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalTempWorker::fwBackendOpTempThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalTempWorker::fwBackendOpTempThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalTempWorker::fwBackendOpTemp - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalTempWorker::fwBackendOpTemp - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalTempWorker::fwBackendOpTemp Get Failed") + ret);
            delete pTempAlertMO;
            delete pResults;
            delete syncQueryTempAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_TEMPERATURES) {
            	trace (TRACE_LEVEL_INFO, string ("LocalTempWorker::fwBackendOpTempThreshold - Inside Down:") +downTh);
            	trace (TRACE_LEVEL_INFO, string ("LocalTempWorker::fwBackendOpTempThreshold - Inside Marginal:") +marginalTh);
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalTempWorker::fwBackendOpTemp - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalTempWorker::fwBackendOpTemp - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalTempWorker::fwBackendOpTemp Set Failed") +ret);
            delete pTempAlertMO;
            delete pResults;
            delete syncQueryTempAlert;
            return (ret);
        }
        */
        delete pTempThreshMO;
        delete pResults;
        return (ret);
    }  

    
    void  LocalTempWorker::SystemMonitorLocalChangeTempThresholdMessageHandler( SystemMonitorLocalChangeTempThresholdMessage *pSystemMonitorLocalChangeTempThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeTempThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeTempThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeTempThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeTempThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeTempThresholdMessage);
            return;           
        }
 
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        int downThresh     = pSystemMonitorLocalChangeTempThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeTempThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeTempThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_TEMPERATURES) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeTempThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_TEMPERATURES) {
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
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeTempThresholdMessageHandler Get Failed" +ret);
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeTempThresholdMessageHandler Get Failed" +isChanged);
            pSystemMonitorLocalChangeTempThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeTempThresholdMessage);
            return;
        }

        if (((UI32)downThresh > pList.temp) || ((UI32)marginalThresh > pList.temp) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeTempThresholdMessageHandler : Invalid thresh"));
            pSystemMonitorLocalChangeTempThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeTempThresholdMessage);
            return;
        }
        unsigned int i = 0;
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_TEMPERATURES) {
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeTempThresholdMessageHandler Set Failed" +ret);
            pSystemMonitorLocalChangeTempThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeTempThresholdMessage);
            return;
        }

        /* Update Local MO */
        TempThresholdLocalManagedObject *pTempThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, TempThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such Temp threshold managed object");
            pSystemMonitorLocalChangeTempThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeTempThresholdMessage);
            return;
        }
        pTempThreshLMO = dynamic_cast <TempThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pTempThreshLMO);
        pTempThreshLMO->setDownthreshold(downThresh);
        pTempThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeTempThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pTempThreshLMO;
        pSystemMonitorLocalChangeTempThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeTempThresholdMessage);
    }

}
