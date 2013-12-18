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
#include "SystemMonitor/Local/LocalCompactFlashWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeCompactFlashThresholdMessage.h"
#include "SystemMonitor/Local/CompactFlashThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalCompactFlashWorker::LocalCompactFlashWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        CompactFlashThresholdLocalManagedObject    CompactFlashThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        CompactFlashThresholdLocalManagedObject.setupOrm ();
        addManagedClass (CompactFlashThresholdLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGECOMPACTFLASHTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalCompactFlashWorker::SystemMonitorLocalChangeCompactFlashThresholdMessageHandler));
    }

    LocalCompactFlashWorker::~LocalCompactFlashWorker ()
    {
    }

    PrismMessage  *LocalCompactFlashWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGECOMPACTFLASHTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeCompactFlashThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalCompactFlashWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((CompactFlashThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CompactFlashThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
       else
        {
            trace (TRACE_LEVEL_FATAL, "LocalCompactFlashWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    
    int LocalCompactFlashWorker::fwBackendOpCompactFlashThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalCompactFlashWorker::fwBackendOpCompactFlashThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
        CompactFlashThresholdLocalManagedObject *pCompactFlashThreshMO = NULL;
        
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, CompactFlashThresholdLocalManagedObject::getClassName());
        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalCompactFlashWorker::fwBackendOpCompactFlashThreshold No CompactFlashThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_FLASH) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal; 
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pCompactFlashThreshMO = new CompactFlashThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalCompactFlashWorker::fwBackendOpCompactFlashThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalCompactFlashWorker::fwBackendOpCompactFlash - Threshold MO exists");
            pCompactFlashThreshMO  = dynamic_cast <CompactFlashThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pCompactFlashThreshMO->getDownthreshold();
            marginalTh    = pCompactFlashThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalCompactFlashWorker::fwBackendOpCompactFlashThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalCompactFlashWorker::fwBackendOpCompactFlashThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalCompactFlashWorker::fwBackendOpCompactFlash - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalCompactFlashWorker::fwBackendOpCompactFlash - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalCompactFlashWorker::fwBackendOpCompactFlash Get Failed") + ret);
            delete pCompactFlashAlertMO;
            delete pResults;
            delete syncQueryCompactFlashAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_FLASH) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalCompactFlashWorker::fwBackendOpCompactFlash - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalCompactFlashWorker::fwBackendOpCompactFlash - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalCompactFlashWorker::fwBackendOpCompactFlash Set Failed") +ret);
            delete pCompactFlashAlertMO;
            delete pResults;
            delete syncQueryCompactFlashAlert;
            return (ret);
        }
        */
        delete pCompactFlashThreshMO;
        delete pResults;
        return (ret);
    }   
    
    void  LocalCompactFlashWorker::SystemMonitorLocalChangeCompactFlashThresholdMessageHandler( SystemMonitorLocalChangeCompactFlashThresholdMessage *pSystemMonitorLocalChangeCompactFlashThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeCompactFlashThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeCompactFlashThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeCompactFlashThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeCompactFlashThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeCompactFlashThresholdMessage);
            return;           
        }
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        int downThresh     = pSystemMonitorLocalChangeCompactFlashThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeCompactFlashThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeCompactFlashThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_FLASH) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeCompactFlashThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_FLASH) {
                    marginalThresh = lOM->m_defpList.defStatPolicy[th].marginal.marginal; 
                    break;
                }
            }
        } 

         /* Do Fabric Watch validation here */
        swPolicyList_t pList;
        memset (&pList, 0, sizeof (pList));
        ret = switchStatusPolicyGetData(&pList);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeCompactFlashThresholdMessageHandler : Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeCompactFlashThresholdMessageHandler : Get Failed") +isChanged);
            pSystemMonitorLocalChangeCompactFlashThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeCompactFlashThresholdMessage);
            return;
        }

        if (((UI32)downThresh > 1) || ((UI32)marginalThresh > 1) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeCompactFlashThresholdMessageHandler : Invalid Thresh") +ret);
            pSystemMonitorLocalChangeCompactFlashThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeCompactFlashThresholdMessage);
            return;
        }
       unsigned int i = 0;
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_FLASH) {
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeCompactFlashThresholdMessageHandler :Set Failed" )+ret);
            pSystemMonitorLocalChangeCompactFlashThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeCompactFlashThresholdMessage);
            return;
        }

        /* Update Local MO */
        CompactFlashThresholdLocalManagedObject *pCompactFlashThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, CompactFlashThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such CompactFlash threshold managed object");
            pSystemMonitorLocalChangeCompactFlashThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeCompactFlashThresholdMessage);
            return;
        }
        pCompactFlashThreshLMO = dynamic_cast <CompactFlashThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pCompactFlashThreshLMO);
        pCompactFlashThreshLMO->setDownthreshold(downThresh);
        pCompactFlashThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeCompactFlashThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pCompactFlashThreshLMO;
        pSystemMonitorLocalChangeCompactFlashThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeCompactFlashThresholdMessage);
    }

}
