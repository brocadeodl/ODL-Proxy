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
#include "ThresholdMonitor/Local/LocalMemoryWorker.h"
#include "ThresholdMonitor/Local/ThresholdMonitorSetMemoryMessage.h"
#include "ThresholdMonitor/Local/MemoryLocalManagedObject.h"
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

    LocalMemoryWorker::LocalMemoryWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
        : WaveWorker  (pThresholdMonitorLocalObjectManager)
    {
        MemoryLocalManagedObject    MemoryLocalManagedObject    (pThresholdMonitorLocalObjectManager);
        MemoryLocalManagedObject.setupOrm ();
        addManagedClass (MemoryLocalManagedObject::getClassName (), this);
        addOperationMap (THRESHOLDMONITORSETMEMORY, reinterpret_cast<PrismMessageHandler> (&LocalMemoryWorker::ThresholdMonitorSetMemoryMessageHandler));
    }

    LocalMemoryWorker::~LocalMemoryWorker ()
    {
    }

    PrismMessage  *LocalMemoryWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case THRESHOLDMONITORSETMEMORY :
                pPrismMessage = new ThresholdMonitorSetMemoryMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalMemoryWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((MemoryLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MemoryLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalMemoryWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
    void LocalMemoryWorker::backendSync ()
	{

		trace (TRACE_LEVEL_INFO, "LocalMemoryWorker::backendSync - Entering..");
		int poll = 0, retry = 0, limit = 0, highLimit = 0, lowLimit = 0;
		int ret = -1;
		actionType actions = FW_NONE;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		struct pcbExtInterface cfg;

		MemoryLocalManagedObject *pMemoryMO = NULL;
		
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MemoryLocalManagedObject::getClassName ());
		if (pResults->size() == 0) {
			trace (TRACE_LEVEL_ERROR, "LocalMemoryWorker::backendSync No MemoryMO");
			poll 		= MEMORY_POLL_DEFAULT;
			retry 		= MEMORY_RETRY_DEFAULT;
			limit		= MEMORY_LIMIT_DEFAULT;
			lowLimit	= MEMORY_LOW_LIMIT_DEFAULT;
			highLimit	= MEMORY_HIGH_LIMIT_DEFAULT;
			actions		= FW_NONE;
			startTransaction ();
			pMemoryMO = new MemoryLocalManagedObject
				(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()),
				 poll, retry, limit, highLimit, lowLimit, actions);
			status = commitTransaction();
			if (status == FRAMEWORK_SUCCESS)
				trace (TRACE_LEVEL_INFO, "LocalMemoryWorker::backendSync - Default commit success");
		} else {
			trace (TRACE_LEVEL_INFO, "LocalMemoryWorker::backendSync - MO exists");
			pMemoryMO  	= dynamic_cast <MemoryLocalManagedObject *> ((*pResults)[0]);
			poll        = pMemoryMO->getPoll();
			retry	    = pMemoryMO->getRetry();
			limit	    = pMemoryMO->getLimit();
			lowLimit	= pMemoryMO->getLowlimit();
			highLimit	= pMemoryMO->getHighlimit();
			actions	    = pMemoryMO->getActions();
		}
		memset(&cfg, 0, sizeof (cfg));
		ret = fwCpuOrMemUsageHandler( &cfg, CLI_OP_SHOW, MEM_HANDLER);
		if (ret != 0)
			trace (TRACE_LEVEL_ERROR, string("LocalMemWorker::backendSync -fwCpuOrMemUsageHandler failed")+ret);
		cfg.memPollingInterval = poll;
		cfg.memNoOfRetries =  retry;
		cfg.memTh[MEM_ST_TH2] = limit;
		cfg.memTh[MEM_ST_TH1] = lowLimit;
		cfg.memTh[MEM_ST_TH3] = highLimit;
		cfg.memAction = actions - 1;
		ret = fwCpuOrMemUsageHandler(&cfg, CLI_OP_CONFIG, MEM_HANDLER);
		if (ret != 0)
			trace (TRACE_LEVEL_ERROR, string("LocalMemWorker::backendSync -fwCpuOrMemUsageHandler failed")+ret);

		delete pMemoryMO;
		delete pResults;
	}

    void  LocalMemoryWorker::ThresholdMonitorSetMemoryMessageHandler( ThresholdMonitorSetMemoryMessage *pThresholdMonitorSetMemoryMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering ThresholdMonitorSetMemoryMessageHandler..");
        if (pThresholdMonitorSetMemoryMessage == NULL)
        {
            trace (TRACE_LEVEL_INFO, "ThresholdMonitorSetMemoryMessageHandler : Empty Message");
            prismAssert (false, __FILE__, __LINE__);
        }
		int ret = -1; 
        int poll      		= pThresholdMonitorSetMemoryMessage->getPoll();
        int retry 	  		= pThresholdMonitorSetMemoryMessage->getRetry();
        int limit 	  		= pThresholdMonitorSetMemoryMessage->getLimit();
        int lowLimit  		= pThresholdMonitorSetMemoryMessage->getLowlimit();
        int highLimit 	  	= pThresholdMonitorSetMemoryMessage->getHighlimit();
        actionType action 	= pThresholdMonitorSetMemoryMessage->getActions();

		if (action == (FW_NONE + ALL)) {
	        pThresholdMonitorSetMemoryMessage->setCompletionStatus
    	            (WRC_SM_INVALID_ACTION_ERROR);
            reply (pThresholdMonitorSetMemoryMessage);
            return;
		}

		if ((lowLimit > highLimit) || (limit > highLimit)) {
			trace (TRACE_LEVEL_ERROR, "LocalMemWorker::ThresholdMonitorSetMemoryMessageHandler- Low greater than high");
	        pThresholdMonitorSetMemoryMessage->setCompletionStatus
    	            (WRC_SM_INVALID_LOWLIMIT_ERROR);
            reply (pThresholdMonitorSetMemoryMessage);
            return;
		}
		if (limit < lowLimit) {
			trace (TRACE_LEVEL_ERROR, "LocalMemWorker::ThresholdMonitorSetMemoryMessageHandler- Limit < lowlimit");
	        pThresholdMonitorSetMemoryMessage->setCompletionStatus
    	            (WRC_SM_INVALID_LIMIT_ERROR);
            reply (pThresholdMonitorSetMemoryMessage);
            return;
		}

		
//		trace (TRACE_LEVEL_ERROR, string("LocalMemWorker::ThresholdMonitorSetMemoryMessageHandler limit")+limit);
//		trace (TRACE_LEVEL_ERROR, string("LocalMemWorker::ThresholdMonitorSetMemoryMessageHandler highlimit")+highLimit);
//		trace (TRACE_LEVEL_ERROR, string("LocalMemWorker::ThresholdMonitorSetMemoryMessageHandler lowlimit")+lowLimit);
		struct pcbExtInterface cfg;
		memset(&cfg, 0, sizeof (cfg));
		ret = fwCpuOrMemUsageHandler( &cfg, CLI_OP_SHOW, MEM_HANDLER);
		if (ret != 0)
			trace (TRACE_LEVEL_ERROR, string("LocalMemWorker::ThresholdMonitorSetMemoryMessageHandler -fwCpuOrMemUsageHandler failed")+ret);
		cfg.memPollingInterval = poll;
	    cfg.memNoOfRetries =  retry;
	    cfg.memTh[MEM_ST_TH2] = limit;
	    cfg.memTh[MEM_ST_TH3] = highLimit;
	    cfg.memTh[MEM_ST_TH1] = lowLimit;
	    cfg.memAction = action - 1;
	    ret = fwCpuOrMemUsageHandler(&cfg, CLI_OP_CONFIG, MEM_HANDLER);
		if (ret != 0) {
			trace (TRACE_LEVEL_ERROR, string("LocalMemWorker::ThresholdMonitorSetMemoryMessageHandler -fwCpuOrMemUsageHandler failed")+ret);
			pThresholdMonitorSetMemoryMessage->setCompletionStatus 
				(WRC_SM_FW_SET_BACKEND_ERROR);
            reply (pThresholdMonitorSetMemoryMessage);
            return;
		}
        /* Update Local MO */
        MemoryLocalManagedObject *pMemoryLMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MemoryLocalManagedObject::getClassName ());

        if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "ThresholdMonitorSetMemoryMessageHandler : No such Memory threshold managed object");
            pThresholdMonitorSetMemoryMessage->setCompletionStatus
                (WAVE_MESSAGE_ERROR);
            reply (pThresholdMonitorSetMemoryMessage);
            return;
        }
        pMemoryLMO = dynamic_cast <MemoryLocalManagedObject *> ((*pResults)[0]);
        delete pResults;
        
        startTransaction ();
        updateWaveManagedObject (pMemoryLMO);
        pMemoryLMO->setPoll(poll);
        pMemoryLMO->setRetry(retry);
        pMemoryLMO->setLimit(limit);
        pMemoryLMO->setLowlimit(lowLimit);
        pMemoryLMO->setHighlimit(highLimit);
        pMemoryLMO->setActions(action);
        ResourceId status = commitTransaction ();
        if (status != FRAMEWORK_SUCCESS)
        {
            trace (TRACE_LEVEL_FATAL, "ThresholdMonitorSetMemoryMessageHandler : Commiting failed.Status:"
                    + FrameworkToolKit::localize (status));
        } else 
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
		delete pMemoryLMO;
        pThresholdMonitorSetMemoryMessage->setCompletionStatus (status);
        reply (pThresholdMonitorSetMemoryMessage);

    }

}
