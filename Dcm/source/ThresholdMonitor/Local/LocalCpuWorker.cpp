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
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/LocalCpuWorker.h"
#include "ThresholdMonitor/Local/ThresholdMonitorSetCpuMessage.h"
#include "ThresholdMonitor/Local/CpuLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"


#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
#include <thresh/portCmd.h>

namespace DcmNs
{

    LocalCpuWorker::LocalCpuWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
        : WaveWorker  (pThresholdMonitorLocalObjectManager)
    {
        CpuLocalManagedObject    CpuLocalManagedObject    (pThresholdMonitorLocalObjectManager);
        CpuLocalManagedObject.setupOrm ();
        addManagedClass (CpuLocalManagedObject::getClassName (), this);
        addOperationMap (THRESHOLDMONITORSETCPU, reinterpret_cast<PrismMessageHandler> (&LocalCpuWorker::ThresholdMonitorSetCpuMessageHandler));
    }

    LocalCpuWorker::~LocalCpuWorker ()
    {
    }

    PrismMessage  *LocalCpuWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case THRESHOLDMONITORSETCPU :
                pPrismMessage = new ThresholdMonitorSetCpuMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalCpuWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((CpuLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CpuLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalCpuWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void LocalCpuWorker::backendSync ()
	{

		trace (TRACE_LEVEL_INFO, "LocalCpuWorker::backendSync - Entering..");
		int poll = 0, retry = 0, limit = 0;
		int ret  = -1;
		actionType actions = FW_NONE;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		CpuLocalManagedObject *pCpuMO = NULL;
		struct pcbExtInterface cfg;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, CpuLocalManagedObject::getClassName ());
/*
		WaveManagedObjectSynchronousQueryContext *syncQueryCpu = new
				WaveManagedObjectSynchronousQueryContext(CpuLocalManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryCpu);
*/
		if (pResults->size() == 0) {
			trace (TRACE_LEVEL_ERROR, "LocalCpuWorker::backendSync No CpuMO");
			poll 	= CPU_POLL_DEFAULT;
			retry 	= CPU_RETRY_DEFAULT;
			limit	= CPU_LIMIT_DEFAULT;
			actions	= FW_NONE;
			startTransaction ();
			pCpuMO = new CpuLocalManagedObject
				(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()),
				 poll, retry, limit, actions);
			status = commitTransaction();
			if (status == FRAMEWORK_SUCCESS)
				trace (TRACE_LEVEL_INFO, "LocalCpuWorker::backendSync - Default commit success");
		} else {
			trace (TRACE_LEVEL_INFO, "LocalCpuWorker::backendSync - MO exists");
			pCpuMO  	= dynamic_cast <CpuLocalManagedObject *> ((*pResults)[0]);
			poll        = pCpuMO->getPoll();
			retry	    = pCpuMO->getRetry();
			limit	    = pCpuMO->getLimit();
			actions	    = pCpuMO->getActions();
		}

		memset(&cfg, 0, sizeof (cfg));
		ret = fwCpuOrMemUsageHandler( &cfg, CLI_OP_SHOW, CPU_HANDLER);
		if (ret != 0)
			trace (TRACE_LEVEL_ERROR, string("LocalCpuWorker::backendSync -fwCpuOrMemUsageHandler Get failed")+ret);

		cfg.pollingInterval = poll;
		cfg.noOfRetries =  retry;
		cfg.cpuUsageLimit = limit;
		cfg.cpuAction = actions - 1;
		ret = fwCpuOrMemUsageHandler(&cfg, CLI_OP_CONFIG, CPU_HANDLER);
		if (ret != 0)
			trace (TRACE_LEVEL_ERROR, string("LocalCpuWorker::backendSync -fwCpuOrMemUsageHandler failed")+ret);

		delete pCpuMO;
		delete pResults;
		//delete syncQueryCpu;
	}

    void  LocalCpuWorker::ThresholdMonitorSetCpuMessageHandler( ThresholdMonitorSetCpuMessage *pThresholdMonitorSetCpuMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering ThresholdMonitorSetCpuMessageHandler..");
        if (pThresholdMonitorSetCpuMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "ThresholdMonitorSetCpuMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
		int ret = -1; 
        int poll     = pThresholdMonitorSetCpuMessage->getPoll();
        int retry = pThresholdMonitorSetCpuMessage->getRetry();
        int limit = pThresholdMonitorSetCpuMessage->getLimit();
        actionType action = pThresholdMonitorSetCpuMessage->getActions();

		if (action == (FW_NONE + ALL)) {
	        pThresholdMonitorSetCpuMessage->setCompletionStatus
    	            (WRC_SM_INVALID_ACTION_ERROR);
            reply (pThresholdMonitorSetCpuMessage);
            return;
		}

		struct pcbExtInterface cfg;
		memset(&cfg, 0, sizeof (cfg));
		ret = fwCpuOrMemUsageHandler( &cfg, CLI_OP_SHOW, CPU_HANDLER);
		if (ret != 0)
			trace (TRACE_LEVEL_ERROR, string("LocalCpuWorker::ThresholdMonitorSetCpuMessageHandler -fwCpuOrMemUsageHandler failed")+ret);
		cfg.pollingInterval = poll;
		cfg.noOfRetries =  retry;
		cfg.cpuUsageLimit = limit;
		cfg.cpuAction = action - 1;
	    ret = fwCpuOrMemUsageHandler(&cfg, CLI_OP_CONFIG, CPU_HANDLER);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("LocalCpuWorker::ThresholdMonitorSetCpuMessageHandler -fwCpuOrMemUsageHandler failed")+ret);
			pThresholdMonitorSetCpuMessage->setCompletionStatus 
				(WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pThresholdMonitorSetCpuMessage);
            return;

		}

        CpuLocalManagedObject *pCpuLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		trace (TRACE_LEVEL_ERROR, string("LocalCpuWorker::ThresholdMonitorSetCpuMessageHandler -LocationID is")+thisLocationId);
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, CpuLocalManagedObject::getClassName ());

		/*
        WaveManagedObjectSynchronousQueryContext *syncQueryCpu = new
                WaveManagedObjectSynchronousQueryContext(CpuLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryCpu);
*/
        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "ThresholdMonitorSetCpuMessageHandler : No such Cpu threshold managed object");
            pThresholdMonitorSetCpuMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pThresholdMonitorSetCpuMessage);
            //delete (syncQueryCpu);
            return;
        }
        pCpuLMO = dynamic_cast <CpuLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        //delete syncQueryCpu;
        
        startTransaction ();
        updateWaveManagedObject (pCpuLMO);
        pCpuLMO->setPoll(poll);
        pCpuLMO->setRetry(retry);
        pCpuLMO->setLimit(limit);
        pCpuLMO->setActions(action);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "ThresholdMonitorSetCpuMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pCpuLMO;
        pThresholdMonitorSetCpuMessage->setCompletionStatus (status);
        reply (pThresholdMonitorSetCpuMessage);
    }

}
