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
#include "SystemMonitor/Local/LocalMMWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeMMThresholdMessage.h"
#include "SystemMonitor/Local/MMThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalMMWorker::LocalMMWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        MMThresholdLocalManagedObject    MMThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        MMThresholdLocalManagedObject.setupOrm ();
        addManagedClass (MMThresholdLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGEMMTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalMMWorker::SystemMonitorLocalChangeMMThresholdMessageHandler));
    }

    LocalMMWorker::~LocalMMWorker ()
    {
    }

    PrismMessage  *LocalMMWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGEMMTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeMMThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalMMWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((MMThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MMThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
       else
        {
            trace (TRACE_LEVEL_FATAL, "LocalMMWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    
    int LocalMMWorker::fwBackendOpMMThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalMMWorker::fwBackendOpMMThreshold - Entering..");
        int downTh = 0, marginalTh = 1;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        MMThresholdLocalManagedObject *pMMThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MMThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalMMWorker::fwBackendOpMMThreshold No MMThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_CP) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal; 
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pMMThreshMO = new MMThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalMMWorker::fwBackendOpMMThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalMMWorker::fwBackendOpMM - Threshold MO exists");
            pMMThreshMO  = dynamic_cast <MMThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pMMThreshMO->getDownthreshold();
            marginalTh    = pMMThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalMMWorker::fwBackendOpMMThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalMMWorker::fwBackendOpMMThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalMMWorker::fwBackendOpMM - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalMMWorker::fwBackendOpMM - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalMMWorker::fwBackendOpMM Get Failed") + ret);
            delete pMMAlertMO;
            delete pResults;
            delete syncQueryMMAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_CP) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalMMWorker::fwBackendOpMM - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalMMWorker::fwBackendOpMM - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalMMWorker::fwBackendOpMM Set Failed") +ret);
            delete pMMAlertMO;
            delete pResults;
            delete syncQueryMMAlert;
            return (ret);
        }
        */
        delete pMMThreshMO;
        delete pResults;
        return (ret);
    }   
    
    void  LocalMMWorker::SystemMonitorLocalChangeMMThresholdMessageHandler( SystemMonitorLocalChangeMMThresholdMessage *pSystemMonitorLocalChangeMMThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeMMThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeMMThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeMMThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeMMThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeMMThresholdMessage);
            return;           
        }
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        int downThresh     = pSystemMonitorLocalChangeMMThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeMMThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeMMThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_CP) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeMMThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_CP) {
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
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMMThresholdMessageHandler : Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMMThresholdMessageHandler : Get Failed") +isChanged);
            pSystemMonitorLocalChangeMMThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeMMThresholdMessage);
            return;
        }

        if (((UI32)downThresh > pList.cpNum) || ((UI32)marginalThresh > pList.cpNum) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMMThresholdMessageHandler : Invalid Thresh") +ret);
            pSystemMonitorLocalChangeMMThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeMMThresholdMessage);
            return;
        }
       unsigned int i = 0;
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_CP) {
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeMMThresholdMessageHandler :Set Failed" )+ret);
            pSystemMonitorLocalChangeMMThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeMMThresholdMessage);
            return;
        }

        /* Update Local MO */
        MMThresholdLocalManagedObject *pMMThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MMThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such MM threshold managed object");
            pSystemMonitorLocalChangeMMThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeMMThresholdMessage);
            return;
        }
        pMMThreshLMO = dynamic_cast <MMThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pMMThreshLMO);
        pMMThreshLMO->setDownthreshold(downThresh);
        pMMThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeMMThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pMMThreshLMO;
        pSystemMonitorLocalChangeMMThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeMMThresholdMessage);
    }

}
