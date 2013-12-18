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
 *   Author : Priya Ahuja												   *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/LocalInterfaceConfigWorker.h"
#include "ThresholdMonitor/Local/InterfaceLocalConfigMessage.h"
#include "ThresholdMonitor/Local/InterfaceConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include <thresh/fwdPub.h>
#include <thresh/fwd.h>
#include <thresh/portCmd.h>
#define DEFAULT_BKEND_VAL 2
#define CUSTOM_BKEND_VAL 3
#define DEFAULT_LEVEL 1
#define CUSTOM_LEVEL 0
#define SET 1
namespace DcmNs
{
	LocalInterfaceConfigWorker::LocalInterfaceConfigWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
		{

			InterfaceConfigLocalManagedObject InterfaceConfigLocalManagedObject	(pThresholdMonitorLocalObjectManager);
			InterfaceConfigLocalManagedObject.setupOrm ();
			addManagedClass (InterfaceConfigLocalManagedObject::getClassName (),
					this);
			addOperationMap (INTERFACELOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalInterfaceConfigWorker::InterfaceLocalConfigMessageHandler));
		}
	LocalInterfaceConfigWorker::~LocalInterfaceConfigWorker ()
	{
	}
	
	PrismMessage  *LocalInterfaceConfigWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case INTERFACELOCALCONFIG :
				pPrismMessage = new InterfaceLocalConfigMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
			
	}
	
	WaveManagedObject  *LocalInterfaceConfigWorker::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;
		if ((InterfaceConfigLocalManagedObject::getClassName ()) == managedClassName)
		{
		pWaveManagedObject = new InterfaceConfigLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		}
		else
		{
			trace (TRACE_LEVEL_FATAL, "LocalInterfaceConfigWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
		}
		 return (pWaveManagedObject);
	}
	void LocalInterfaceConfigWorker::InterfaceLocalConfigMessageHandler(InterfaceLocalConfigMessage *pInterfaceLocalConfigMessage)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "Entering InterfaceLocalConfigMessageHandler..");
		if (pInterfaceLocalConfigMessage == NULL) {
			trace (TRACE_LEVEL_INFO, "InterfaceLocalConfigMessageHandler : Empty Message");
			prismAssert (false, __FILE__, __LINE__);
		}
		string applyvalue;
		applyvalue.assign(pInterfaceLocalConfigMessage->getApplyValue());
		bool paus = pInterfaceLocalConfigMessage->getPause();
		InterfaceConfigLocalManagedObject *pInterfaceConfigLocalManagedObject = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceConfigLocalManagedObject::getClassName ());
		 if (pResults != NULL) 
		 {
				 if (pResults->size() == 0)
				 {
			 trace (TRACE_LEVEL_ERROR, "InterfaceLocalConfigMessageHandler: No such Managed Object");
			 pInterfaceLocalConfigMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
			  reply (pInterfaceLocalConfigMessage);
			  return;
		 }
		 }
		 pInterfaceConfigLocalManagedObject = dynamic_cast <InterfaceConfigLocalManagedObject *> ((*pResults)[0]);

        // Apply Custom or Default settings
		int flag, area_iter, ret = 0, retT = 0, retA = 0; 
	
		for (area_iter = 0; area_iter < MAX_AREA_VAL; area_iter++)
		{
			if (!(applyvalue.compare("default")))
			{     // Apply default policy
				flag = DEFAULT_BKEND_VAL;
				retT = thaThLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
				retA = thaActLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
				if ((retT != 0) || (retA != 0))
				{
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
					pInterfaceLocalConfigMessage->setCompletionStatus(FW_APPLY_BACKEND_ERROR);
					reply (pInterfaceLocalConfigMessage);
					trace(TRACE_LEVEL_ERROR,
						"InterfaceLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig failed");
					return;
				}
			}
			if (!applyvalue.compare("custom"))
			{     // Apply custom policy
				flag = CUSTOM_BKEND_VAL;
				retT = thaThLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
				retA = thaActLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
				if ((retT != 0) || (retA != 0))
				{
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
					pInterfaceLocalConfigMessage->setCompletionStatus(FW_APPLY_BACKEND_ERROR);
					reply (pInterfaceLocalConfigMessage);
					trace(TRACE_LEVEL_ERROR,
						"InterfaceLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig failed");
					return;
				}
			}
		}
	     // Pause or Continue INTERFACE Monitoring
	      	int agentClass = AGENT_GEPORT;
		 	int area = THA_ALL_AREAS;
		 	int port = THA_ALL_INDEX;
		 	ret = 0;
		 	if (paus ==  true) {
				trace(TRACE_LEVEL_INFO,"Entering InterfaceLocalConfigMessageHandler:thaThresholdDisable");
		 		ret = thaThresholdDisable (agentClass,area,port +1) ;
					if (ret != 0)
					{
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    			pInterfaceLocalConfigMessage->setCompletionStatus(FW_PAUSE_BACKEND_ERROR);
		    			reply (pInterfaceLocalConfigMessage);
						trace(TRACE_LEVEL_ERROR,
								"InterfaceLocalConfigMessageHandler:thaThresholdDisable failed");
		    			return;
					}
		 	} 
		 	else {
		 		ret = thaThresholdEnable (agentClass,area,port +1) ;
					if (ret != 0)
					{
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    			pInterfaceLocalConfigMessage->setCompletionStatus(FW_CONTINUE_BACKEND_ERROR);
		    			reply (pInterfaceLocalConfigMessage);
						trace(TRACE_LEVEL_ERROR,
								"InterfaceLocalConfigMessageHandler:thaThresholdEnable failed");
		    			return;
					}
			}		 	
		 trace(TRACE_LEVEL_INFO, "Returned from updating backend, now going to update MO");
		 startTransaction ();
		 updateWaveManagedObject (pInterfaceConfigLocalManagedObject);
		 if (applyvalue.compare("default") == 0)
			 pInterfaceConfigLocalManagedObject->setApplyValue(applyvalue, false);
		 else
			 pInterfaceConfigLocalManagedObject->setApplyValue(applyvalue, true);
		 pInterfaceConfigLocalManagedObject->setPause(paus);
		 status = commitTransaction ();
		 if (status != FRAMEWORK_SUCCESS)
		 {
			 trace (TRACE_LEVEL_FATAL, "InterfaceLocalConfigMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
		 } else {
			 status = WAVE_MESSAGE_SUCCESS;
		 }
		 WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		 //delete pInterfaceConfigLocalManagedObject;
		 pInterfaceLocalConfigMessage->setCompletionStatus (status);
		 reply (pInterfaceLocalConfigMessage);
	}
	int LocalInterfaceConfigWorker::fwBackendApplyPause()
	{
		trace (TRACE_LEVEL_INFO, "LocalInterfaceConfigWorker::fwBackendApplyPause Entering.");
		string applyVal("default");
		int ret = 0, retT = 0, retA= 0;
		bool pause = false;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		InterfaceConfigLocalManagedObject *pInterfaceConfigLocalManagedObject = NULL;
		InterfaceConfigLocalManagedObject *pInterfaceConfigLocalManagedObjectCxt = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceConfigLocalManagedObject::getClassName ());
		if (pResults != NULL) 
		{
			if (pResults->size() == 0) {
				trace (TRACE_LEVEL_ERROR, "LocalInterfaceConfigWorker::fwBackendApplyPause No InterfaceConfigLocalManagedObject");
				startTransaction ();
				pInterfaceConfigLocalManagedObject = new InterfaceConfigLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
				pInterfaceConfigLocalManagedObject->setApplyValue(applyVal,	false);
				pInterfaceConfigLocalManagedObject->setPause(pause);
				status = commitTransaction();
				if (status == FRAMEWORK_SUCCESS)
				{
					trace (TRACE_LEVEL_INFO, "LocalInterfaceConfigWorker::fwBackendApplyPause - Default commit success");
				}
				delete pInterfaceConfigLocalManagedObject;
			} else {
				 trace (TRACE_LEVEL_INFO, "Entering ESS");
				pInterfaceConfigLocalManagedObjectCxt = dynamic_cast <InterfaceConfigLocalManagedObject *> ((*pResults)[0]);
				applyVal.assign(pInterfaceConfigLocalManagedObjectCxt->getApplyValue());  
				pause = pInterfaceConfigLocalManagedObjectCxt->getPause();
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				//delete pInterfaceConfigLocalManagedObjectCxt;
				trace (TRACE_LEVEL_INFO, "Leaving ESS");	
			}
			// Apply custom or default config
			int flag, area_iter = 0; 
				for (area_iter = 0; area_iter < MAX_AREA_VAL; area_iter++){
					if (!applyVal.compare("default")){     // Apply custom policy
						flag = DEFAULT_BKEND_VAL;
						retT = thaThLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
						trace (TRACE_LEVEL_DEBUG, string("Returned from backend after applying default config with return val...") + ret);
						if (retT != 0) {
							return (retT);
						}
						retA = thaActLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
						if (retA != 0) {
							return (retA);
						}
					}
					if (!applyVal.compare("custom")){     // Apply default policy
						flag = CUSTOM_BKEND_VAL;
						retT = thaThLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
						trace (TRACE_LEVEL_DEBUG, string("Returned from backend after applying custom config with return val...") + ret);
						if (retT != 0) {
							return (retT);
						}
						retA = thaActLevelSet(AGENT_GEPORT, area_iter, flag, TRUE);
						trace (TRACE_LEVEL_DEBUG, string("Returned from backend after applying custom config with return val...") + ret);
						if (retA != 0) {
							return (retA);
						}
					}
				}
			
			// Pause INTERFACE Monitoring
			int agentClass = AGENT_GEPORT;
			int area = THA_ALL_AREAS;
			int port = THA_ALL_INDEX;
			if (pause ==  true) {
				ret = thaThresholdDisable (agentClass,area,port +1) ;
				trace (TRACE_LEVEL_INFO, string("Returned from backend after disabling monitoring with return val...") + ret);
				if (ret != 0) {
					return (ret);
				}
			} 
			else {
				ret = thaThresholdEnable (agentClass,area,port +1) ;
				trace (TRACE_LEVEL_INFO, string("Returned from backend after enabling monitoring with return val...") + ret);
				if (ret != 0) {
					return (ret);
				}
			}
		}
		//delete pResults;
		trace (TRACE_LEVEL_INFO, "Returning from LocalInterfaceConfigWorker::fwBackendApplyPause...");
		return (ret);
	}
}
