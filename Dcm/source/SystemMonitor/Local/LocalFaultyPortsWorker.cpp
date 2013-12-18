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
#include "SystemMonitor/Local/LocalFaultyPortsWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeFaultyPortsThresholdMessage.h"
#include "SystemMonitor/Local/FaultyPortsThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
namespace DcmNs
{

    LocalFaultyPortsWorker::LocalFaultyPortsWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        FaultyPortsThresholdLocalManagedObject    FaultyPortsThresholdLocalManagedObject    (pSystemMonitorLocalObjectManager);
        FaultyPortsThresholdLocalManagedObject.setupOrm ();
        addManagedClass (FaultyPortsThresholdLocalManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORLOCALCHANGEFAULTYPORTSTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&LocalFaultyPortsWorker::SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler));
    }

    LocalFaultyPortsWorker::~LocalFaultyPortsWorker ()
    {
    }

    PrismMessage  *LocalFaultyPortsWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALCHANGEFAULTYPORTSTHRESHOLD :
                pPrismMessage = new SystemMonitorLocalChangeFaultyPortsThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalFaultyPortsWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FaultyPortsThresholdLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FaultyPortsThresholdLocalManagedObject(dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
       else
        {
            trace (TRACE_LEVEL_FATAL, "LocalFaultyPortsWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    
    int LocalFaultyPortsWorker::fwBackendOpFaultyPortsThreshold (swPolicyList_t *pList)
    {
        trace (TRACE_LEVEL_INFO, "LocalFaultyPortsWorker::fwBackendOpFaultyPortsThreshold - Entering..");
        int downTh = 25, marginalTh = 10;
        unsigned int i = 0;
        int ret = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        FaultyPortsThresholdLocalManagedObject *pFaultyPortsThreshMO = NULL;
        
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FaultyPortsThresholdLocalManagedObject::getClassName ());

        if (pResults->size() == 0) {
            trace (TRACE_LEVEL_ERROR, "LocalFaultyPortsWorker::fwBackendOpFaultyPortsThreshold No FaultyPortsThresholdMO");
            SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
            for (i = 0; i < lOM->m_defpList.num; i++) {
                if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_FAULTY_PORTS) {
                    downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
                    marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal; 
                    pList->statPolicy[i].down.down = downTh;
                    pList->statPolicy[i].marginal.marginal = marginalTh;
                    break;
                }
            }
            startTransaction ();
            pFaultyPortsThreshMO = new FaultyPortsThresholdLocalManagedObject
                (dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager()),
                 downTh, marginalTh);
            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS)
                trace (TRACE_LEVEL_INFO, "LocalFaultyPortsWorker::fwBackendOpFaultyPortsThreshold - Default commit success");
        } else {
            trace (TRACE_LEVEL_INFO, "LocalFaultyPortsWorker::fwBackendOpFaultyPorts - Threshold MO exists");
            pFaultyPortsThreshMO  = dynamic_cast <FaultyPortsThresholdLocalManagedObject *> ((*pResults)[0]);
            downTh        = pFaultyPortsThreshMO->getDownthreshold();
            marginalTh    = pFaultyPortsThreshMO->getMarginalthreshold();
            trace (TRACE_LEVEL_INFO, string ("LocalFaultyPortsWorker::fwBackendOpFaultyPortsThreshold - Down:") +downTh);
            trace (TRACE_LEVEL_INFO, string ("LocalFaultyPortsWorker::fwBackendOpFaultyPortsThreshold - Marginal:") +marginalTh);
        }
/*
        fw_fruObj_t fruCfg;
        memset (&fruCfg, 0, sizeof (fruCfg));
        trace (TRACE_LEVEL_INFO, "LocalFaultyPortsWorker::fwBackendOpFaultyPorts - Before FW get");
        ret = fw_fru_cfg_get_all (&fruCfg);
        trace (TRACE_LEVEL_INFO, string ("LocalFaultyPortsWorker::fwBackendOpFaultyPorts - After FW get") +fruCfg.fruNum);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string ("LocalFaultyPortsWorker::fwBackendOpFaultyPorts Get Failed") + ret);
            delete pFaultyPortsAlertMO;
            delete pResults;
            delete syncQueryFaultyPortsAlert;
            return (ret);
        }
  */ 
        for (i = 0; i < pList->num; i++) {
            if (pList->statPolicy[i].idx == SWSTATUS_FAULTY_PORTS) {
                pList->statPolicy[i].down.down = downTh;
                pList->statPolicy[i].marginal.marginal = marginalTh;
                break;
            }
        }
        /*
        trace (TRACE_LEVEL_INFO, "LocalFaultyPortsWorker::fwBackendOpFaultyPorts - Before FW set");
        ret = fw_fru_cfg_set_all (fruCfg);
        trace (TRACE_LEVEL_INFO, "LocalFaultyPortsWorker::fwBackendOpFaultyPorts - After FW set");
        if (ret == -1) {
            trace (TRACE_LEVEL_INFO, string("LocalFaultyPortsWorker::fwBackendOpFaultyPorts Set Failed") +ret);
            delete pFaultyPortsAlertMO;
            delete pResults;
            delete syncQueryFaultyPortsAlert;
            return (ret);
        }
        */
        delete pFaultyPortsThreshMO;
        delete pResults;
        return (ret);
    }   
    
    void  LocalFaultyPortsWorker::SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler( SystemMonitorLocalChangeFaultyPortsThresholdMessage *pSystemMonitorLocalChangeFaultyPortsThresholdMessage)
    {

		trace (TRACE_LEVEL_INFO, "Entering SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler..");
        if (pSystemMonitorLocalChangeFaultyPortsThresholdMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pSystemMonitorLocalChangeFaultyPortsThresholdMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pSystemMonitorLocalChangeFaultyPortsThresholdMessage);
            return;           
        }
        int ret = -1, isChanged = -1;
        unsigned int th = 0;
        int downThresh     = pSystemMonitorLocalChangeFaultyPortsThresholdMessage->getDownthreshold();
        int marginalThresh = pSystemMonitorLocalChangeFaultyPortsThresholdMessage->getMarginalthreshold();
        
        /* If threshold values are -1 then that means it needs to be defaulted */
        if (downThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler : Defaulting down..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_FAULTY_PORTS) {
                    downThresh     = lOM->m_defpList.defStatPolicy[th].down.down;
                    break;
                }
            }
        }
        
        if (marginalThresh == -1) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler : Defaulting marginal..");
            for (th = 0; th < lOM->m_defpList.num; th++) {
                if (lOM->m_defpList.defStatPolicy[th].idx == SWSTATUS_FAULTY_PORTS) {
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
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler : Get Failed") +ret);
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler : Get Failed") +isChanged);
            pSystemMonitorLocalChangeFaultyPortsThresholdMessage->setCompletionStatus
                (WRC_SM_FW_GET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeFaultyPortsThresholdMessage);
            return;
        }

        if (((UI32)downThresh > 1) || ((UI32)marginalThresh > 1) )
        {
            /* Threshold entered is greater than number of components in system */
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler : Invalid Thresh") +ret);
            pSystemMonitorLocalChangeFaultyPortsThresholdMessage->setCompletionStatus
                (WRC_SM_INVALID_THRESH_ERROR);
            reply (pSystemMonitorLocalChangeFaultyPortsThresholdMessage);
            return;
        }
       unsigned int i = 0;
        for (i = 0; i < pList.num; i++) {
            if (pList.statPolicy[i].idx == SWSTATUS_FAULTY_PORTS) {
                pList.statPolicy[i].down.down = downThresh;
                pList.statPolicy[i].marginal.marginal = marginalThresh;
                break;
            }
        }
        ret = switchStatusPolicySetData (&pList, &isChanged);
        if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler :Set Failed" )+ret);
            pSystemMonitorLocalChangeFaultyPortsThresholdMessage->setCompletionStatus
                (WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pSystemMonitorLocalChangeFaultyPortsThresholdMessage);
            return;
        }

        /* Update Local MO */
        FaultyPortsThresholdLocalManagedObject *pFaultyPortsThreshLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FaultyPortsThresholdLocalManagedObject::getClassName ());

		

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "No such FaultyPorts threshold managed object");
            pSystemMonitorLocalChangeFaultyPortsThresholdMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pSystemMonitorLocalChangeFaultyPortsThresholdMessage);
            return;
        }
        pFaultyPortsThreshLMO = dynamic_cast <FaultyPortsThresholdLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pFaultyPortsThreshLMO);
        pFaultyPortsThreshLMO->setDownthreshold(downThresh);
        pFaultyPortsThreshLMO->setMarginalthreshold(marginalThresh);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "SystemMonitorLocalChangeFaultyPortsThresholdMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pFaultyPortsThreshLMO;
        pSystemMonitorLocalChangeFaultyPortsThresholdMessage->setCompletionStatus (status);
        reply (pSystemMonitorLocalChangeFaultyPortsThresholdMessage);
    }

}
