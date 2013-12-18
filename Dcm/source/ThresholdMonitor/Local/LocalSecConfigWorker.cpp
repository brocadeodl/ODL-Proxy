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
 *   Author : Ramya Rangarajan											   *
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
#include "ThresholdMonitor/Local/LocalSecConfigWorker.h"
#include "ThresholdMonitor/Local/SecLocalConfigMessage.h"
#include "ThresholdMonitor/Local/SecConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include <thresh/fwdPub.h>
#include <thresh/fwd.h>
#include <thresh/portCmd.h>
#define DEFAULT_BKEND_VAL 2
#define CUSTOM_BKEND_VAL 3
#define DEFAULT_LEVEL 1
#define CUSTOM_LEVEL 0
namespace DcmNs
{
	LocalSecConfigWorker::LocalSecConfigWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
		{

			SecConfigLocalManagedObject SecConfigLocalManagedObject	(pThresholdMonitorLocalObjectManager);
			SecConfigLocalManagedObject.setupOrm ();
			addManagedClass (SecConfigLocalManagedObject::getClassName (),
					this);
			addOperationMap (SECURITYLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalSecConfigWorker::SecLocalConfigMessageHandler));
		}
	LocalSecConfigWorker::~LocalSecConfigWorker ()
	{
	}
	
	PrismMessage  *LocalSecConfigWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case SECURITYLOCALCONFIG :
				pPrismMessage = new SecLocalConfigMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
			
	}
	
	WaveManagedObject  *LocalSecConfigWorker::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;
		if ((SecConfigLocalManagedObject::getClassName ()) == managedClassName)
		{
		pWaveManagedObject = new SecConfigLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		}
		else
		{
			trace (TRACE_LEVEL_FATAL, "LocalSecConfigWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
		}
		 return (pWaveManagedObject);
	}
	void LocalSecConfigWorker::SecLocalConfigMessageHandler(SecLocalConfigMessage *pSecLocalConfigMessage)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "Entering SecLocalConfigMessageHandler..");
		if (pSecLocalConfigMessage == NULL) {
			trace (TRACE_LEVEL_INFO, "SecLocalConfigMessageHandler : Empty Message");
			prismAssert (false, __FILE__, __LINE__);
		}
		string applyvalue;
		applyvalue.assign(pSecLocalConfigMessage->getApplyValue());
		bool pause = pSecLocalConfigMessage->getPause();
		trace (TRACE_LEVEL_INFO, "SecLocalConfigMessageHandler:Applyval" + applyvalue + ", Pause " + pause);
		SecConfigLocalManagedObject *pSecConfigLocalManagedObject = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SecConfigLocalManagedObject::getClassName ());
		 if (pResults != NULL) 
		 {
				 if (pResults->size() == 0)
				 {
					 trace (TRACE_LEVEL_ERROR, "SecLocalConfigMessageHandler: No such Managed Object");
			 		pSecLocalConfigMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
			 		reply (pSecLocalConfigMessage);
			 		return;
		 		}
		 }
		 pSecConfigLocalManagedObject = dynamic_cast <SecConfigLocalManagedObject *> ((*pResults)[0]);
        // Apply Custom or Default settings
         unsigned int area, ret = 0;
         unsigned int flag;
        /* Loop through all area types */
        	for (area = 0; area < MAX_SEC_AREA; area++) {
            	if (!(applyvalue.compare("default")))
				{     
					// Apply default policy
					flag = DEFAULT_BKEND_VAL;
					ret = fwApplyCustomOrDefaultConfigForagent(AGENT_SECURITY,secTypeVal[area].area, flag);
					if (ret != 0)
					{
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    			pSecLocalConfigMessage->setCompletionStatus(FW_APPLY_BACKEND_ERROR);
		    			reply (pSecLocalConfigMessage);
						trace(TRACE_LEVEL_ERROR,
								"SecLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig failed");
		    			return;
					}
            	}
            	if (!applyvalue.compare("custom"))
				{     // Apply custom policy
                	flag = CUSTOM_BKEND_VAL;
                	ret = fwApplyCustomOrDefaultConfigForagent(AGENT_SECURITY,
							secTypeVal[area].area, flag);
					if (ret != 0)
					{
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    			pSecLocalConfigMessage->setCompletionStatus(FW_APPLY_BACKEND_ERROR);
		    			reply (pSecLocalConfigMessage);
						trace(TRACE_LEVEL_ERROR,
								"SecLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig failed");
		    			return;
					}
            	}
         	 }

		//Pause or Continue security monitoring
			int port = THA_ALL_INDEX;
			ret = 0;
			if(pause ==  true) {
				trace(TRACE_LEVEL_INFO,"Entering SecLocalConfigMessageHandler:thaThresholdDisable");
				ret = thaThresholdDisable (AGENT_SECURITY,THA_ALL_AREAS,port +1) ;
				if (ret != 0) {
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
					pSecLocalConfigMessage->setCompletionStatus(FW_PAUSE_BACKEND_ERROR);
					reply (pSecLocalConfigMessage);
					trace(TRACE_LEVEL_ERROR,
							"SecLocalConfigMessageHandler:thaThresholdDisable failed");
					return;
				}
			}
			else {
				trace(TRACE_LEVEL_INFO,"Entering SecLocalConfigMessageHandler:thaThresholdEnable");
				ret = thaThresholdEnable (AGENT_SECURITY,THA_ALL_AREAS,port +1) ;
				if (ret != 0) {
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
					pSecLocalConfigMessage->setCompletionStatus(FW_CONTINUE_BACKEND_ERROR);
					reply (pSecLocalConfigMessage);
					trace(TRACE_LEVEL_ERROR,
							"SecLocalConfigMessageHandler:thaThresholdEnable failed");
					return;
				}
			}
		trace(TRACE_LEVEL_INFO, "Returned from updating backend, now going to update MO");
		 startTransaction ();
		 updateWaveManagedObject (pSecConfigLocalManagedObject);
		 if (applyvalue.compare("default") == 0)
		 	pSecConfigLocalManagedObject->setApplyValue(applyvalue, false);
		 else
			 pSecConfigLocalManagedObject->setApplyValue(applyvalue, true);
		pSecConfigLocalManagedObject->setPause(pause);
		 status = commitTransaction ();
		 if (status != FRAMEWORK_SUCCESS)
		 {
			 trace (TRACE_LEVEL_FATAL, "SecLocalConfigMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
		 } else {
			 status = WAVE_MESSAGE_SUCCESS;
		 }
		 WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
//		 delete pSecConfigLocalManagedObject;
		 pSecLocalConfigMessage->setCompletionStatus (status);
		 reply (pSecLocalConfigMessage);
	}
	int LocalSecConfigWorker::fwBackendApply()
	{
		trace (TRACE_LEVEL_INFO, "LocalSecConfigWorker::fwBackendApply Entering.");
		string applyvalue("default");
		bool pause = false;
		int ret = 0;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		SecConfigLocalManagedObject *pSecConfigLocalManagedObject = NULL;
		SecConfigLocalManagedObject *pSecConfigLocalManagedObjectCxt = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SecConfigLocalManagedObject::getClassName ());
		if (pResults != NULL) 
		{
			if (pResults->size() == 0) {
				trace (TRACE_LEVEL_ERROR, "LocalSecConfigWorker::fwBackendApply No SecConfigLocalManagedObject");
				startTransaction ();
				pSecConfigLocalManagedObject = new SecConfigLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
				pSecConfigLocalManagedObject->setApplyValue(applyvalue, false);
				pSecConfigLocalManagedObject->setPause(pause);
				status = commitTransaction();
				if (status == FRAMEWORK_SUCCESS)
				{
					trace (TRACE_LEVEL_INFO, "LocalSecConfigWorker::fwBackendApply - Default commit success");
				}
				delete pSecConfigLocalManagedObject;
			} else {
			/*WaveManagedObjectSynchronousQueryContext *syncQueryCxt = new WaveManagedObjectSynchronousQueryContext(SecConfigLocalManagedObject::getClassName());
	 		vector<WaveManagedObject *> *pResultsCxt = querySynchronously
				(syncQueryCxt); */
	 		pSecConfigLocalManagedObjectCxt = dynamic_cast <SecConfigLocalManagedObject *> ((*pResults)[0]);
        //	string applyvalue;
			applyvalue.assign(pSecConfigLocalManagedObjectCxt->getApplyValue());
			pause = pSecConfigLocalManagedObjectCxt->getPause();
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		//	delete pSecConfigLocalManagedObjectCxt;
			}
        // External State Synchronisation
        	int area = 0;
        	int flag;
				for (area = 0; area < MAX_AREA_VAL; area++){
					if (!applyvalue.compare("default")){     // Apply custom policy
						flag = DEFAULT_BKEND_VAL;
						ret = fwApplyCustomOrDefaultConfigForagent(AGENT_SECURITY,secTypeVal[area].area, flag);
						if (ret != 0) {
							return (FW_APPLY_BACKEND_ERROR);
						}
					}
					if (!applyvalue.compare("custom")){     // Apply default policy
						flag = CUSTOM_BKEND_VAL;
						ret = fwApplyCustomOrDefaultConfigForagent(AGENT_SECURITY,secTypeVal[area].area, flag);
						if (ret != 0) {
							return (FW_APPLY_BACKEND_ERROR);
						}
					}
				}
			//Pause security monitoring
			area = THA_ALL_AREAS;
			int port = THA_ALL_INDEX;
			if (pause ==  true) {
				ret = thaThresholdDisable (AGENT_SECURITY,area,port +1) ;
				trace (TRACE_LEVEL_INFO, string("Returned from backend after disabling monitoring with return val...") + ret);
				if (ret != 0) {
					return(FW_PAUSE_BACKEND_ERROR);
				}
			}
			else {
				ret = thaThresholdEnable (AGENT_SECURITY,area,port +1) ;
				trace (TRACE_LEVEL_INFO, string("Returned from backend after enabling monitoring with return val...") + ret);
				if (ret != 0) {
					return(FW_CONTINUE_BACKEND_ERROR);
				}
			}
		}
	return (ret);
	}
}
