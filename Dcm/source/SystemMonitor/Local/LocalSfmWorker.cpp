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
#include "SystemMonitor/Local/LocalSfmWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeSfmThresholdMessage.h"
#include "SystemMonitor/Local/SfmThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalSfmWorker::LocalSfmWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        SfmThresholdLocalManagedObject    SfmThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        SfmThresholdLocalManagedObject.setupOrm ();
        addManagedClass (SfmThresholdLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGESFMTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalSfmWorker::SystemMonitorLocalChangeSfmThresholdMessageHandler));
    }

    LocalSfmWorker::~LocalSfmWorker ()
    {
    }

    PrismMessage  *LocalSfmWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGESFMTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeSfmThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalSfmWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((SfmThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SfmThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
       else
        {
            trace (TRACE_LEVEL_FATAL, "LocalSfmWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    
    int LocalSfmWorker::fwBackendOpSfmThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalSfmWorker::fwBackendOpSfmThreshold - Entering..");
        int downTh = 2, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        SfmThresholdLocalManagedObject *pSfmThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfmThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalSfmWorker::fwBackendOpSfmThreshold No SfmThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_CORE_BLADE) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal; 
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pSfmThreshMO = new SfmThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalSfmWorker::fwBackendOpSfmThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalSfmWorker::fwBackendOpSfm - Threshold MO exists");
            pSfmThreshMO  = dynamic_cast <SfmThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pSfmThreshMO->getDownthreshold();
            marginalTh    = pSfmThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalSfmWorker::fwBackendOpSfmThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalSfmWorker::fwBackendOpSfmThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalSfmWorker::fwBackendOpSfm - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalSfmWorker::fwBackendOpSfm - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalSfmWorker::fwBackendOpSfm Get Failed") + ret);
            delete pSfmAlertMO;
            delete pResults;
            delete syncQuerySfmAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_CORE_BLADE) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalSfmWorker::fwBackendOpSfm - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalSfmWorker::fwBackendOpSfm - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalSfmWorker::fwBackendOpSfm Set Failed") +ret);
            delete pSfmAlertMO;
            delete pResults;
            delete syncQuerySfmAlert;
            return (ret);
        }
        */
        delete pSfmThreshMO;
        delete pResults;
        return (ret);
    }   
    
    void  LocalSfmWorker::SystemMonitorLocalChangeSfmThresholdMessageHandler( SystemMonitorLocalChangeSfmThresholdMessage *pSystemMonitorLocalChangeSfmThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeSfmThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeSfmThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeSfmThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeSfmThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeSfmThresholdMessage);
            return;           
        }
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        int downThresh     = pSystemMonitorLocalChangeSfmThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeSfmThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeSfmThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_CORE_BLADE) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeSfmThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_CORE_BLADE) {
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
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeSfmThresholdMessageHandler : Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeSfmThresholdMessageHandler : Get Failed") +isChanged);
            pSystemMonitorLocalChangeSfmThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeSfmThresholdMessage);
            return;
        }

        if (((UI32)downThresh > pList.coreBladeNum) || ((UI32)marginalThresh > pList.coreBladeNum) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeSfmThresholdMessageHandler : Invalid Thresh") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeSfmThresholdMessageHandler : CoreBladeNum:") +pList.coreBladeNum);
            pSystemMonitorLocalChangeSfmThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeSfmThresholdMessage);
            return;
        }
       unsigned int i = 0;
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_CORE_BLADE) {
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeSfmThresholdMessageHandler :Set Failed" )+ret);
            pSystemMonitorLocalChangeSfmThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeSfmThresholdMessage);
            return;
        }

        /* Update Local MO */
        SfmThresholdLocalManagedObject *pSfmThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfmThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such Sfm threshold managed object");
            pSystemMonitorLocalChangeSfmThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeSfmThresholdMessage);
            return;
        }
        pSfmThreshLMO = dynamic_cast <SfmThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pSfmThreshLMO);
        pSfmThreshLMO->setDownthreshold(downThresh);
        pSfmThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeSfmThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pSfmThreshLMO;
        pSystemMonitorLocalChangeSfmThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeSfmThresholdMessage);
    }

}
