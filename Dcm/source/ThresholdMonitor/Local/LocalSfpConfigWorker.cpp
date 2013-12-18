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
#include "ThresholdMonitor/Local/LocalSfpConfigWorker.h"
#include "ThresholdMonitor/Local/SfpLocalConfigMessage.h"
#include "ThresholdMonitor/Local/SfpConfigLocalManagedObject.h"
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
	LocalSfpConfigWorker::LocalSfpConfigWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
		{

			SfpConfigLocalManagedObject SfpConfigLocalManagedObject	(pThresholdMonitorLocalObjectManager);
			SfpConfigLocalManagedObject.setupOrm ();
			addManagedClass (SfpConfigLocalManagedObject::getClassName (),
					this);
			addOperationMap (SFPLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalSfpConfigWorker::SfpLocalConfigMessageHandler));
		}
	LocalSfpConfigWorker::~LocalSfpConfigWorker ()
	{
	}
	
	PrismMessage  *LocalSfpConfigWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case SFPLOCALCONFIG :
				pPrismMessage = new SfpLocalConfigMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
			
	}
	
	WaveManagedObject  *LocalSfpConfigWorker::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;
		if ((SfpConfigLocalManagedObject::getClassName ()) == managedClassName)
		{
		pWaveManagedObject = new SfpConfigLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		}
		else
		{
			trace (TRACE_LEVEL_FATAL, "LocalSfpConfigWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
		}
		 return (pWaveManagedObject);
	}
	void LocalSfpConfigWorker::SfpLocalConfigMessageHandler(SfpLocalConfigMessage *pSfpLocalConfigMessage)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "Entering SfpLocalConfigMessageHandler..");
		if (pSfpLocalConfigMessage == NULL) {
			trace (TRACE_LEVEL_INFO, "SfpLocalConfigMessageHandler : Empty Message");
			prismAssert (false, __FILE__, __LINE__);
		}
		string applyvalue;
		applyvalue.assign(pSfpLocalConfigMessage->getApplyValue());
		trace (TRACE_LEVEL_INFO, "SfpLocalConfigMessageHandler:Applyval" + applyvalue );
		bool paus = pSfpLocalConfigMessage->getPause();
		SfpConfigLocalManagedObject *pSfpConfigLocalManagedObject = NULL;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpConfigLocalManagedObject::getClassName ());
		 if (pResults != NULL) 
		 {
				 if (pResults->size() == 0)
				 {
			 trace (TRACE_LEVEL_ERROR, "SfpLocalConfigMessageHandler: No such Managed Object");
			 pSfpLocalConfigMessage->setCompletionStatus(WAVE_MESSAGE_ERROR);
			  reply (pSfpLocalConfigMessage);
			  return;
		 }
		 }
		 pSfpConfigLocalManagedObject = dynamic_cast <SfpConfigLocalManagedObject *> ((*pResults)[0]);
        // Apply Custom or Default settings
        	int area_iter, ret = 0;
        	int flag;
			unsigned int oui_iter = 0;
			vector<string> oui;
			oui.push_back("1GSR");
			oui.push_back("1GLR");
			oui.push_back("10GSR");
			oui.push_back("10GLR");
			oui.push_back("10GUSR");
			oui.push_back("QSFP");
		
        	/* Loop through all the OUI and area types */
        	for (oui_iter = 0; oui_iter < oui.size(); oui_iter++)
			{      
        		for (area_iter = 0; area_iter < MAX_AREA_VAL; area_iter++)
				{
            		if (!(applyvalue.compare("default")))
					{     // Apply default policy
						flag = DEFAULT_BKEND_VAL;
						trace(TRACE_LEVEL_INFO,
								"SfpLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig Default case");
						ret = fwApplyCustomOrDefaultConfig(oui[oui_iter].c_str(), area_iter, flag);
						if (ret != 0)
						{
							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    				pSfpLocalConfigMessage->setCompletionStatus(FW_APPLY_BACKEND_ERROR);
		    				reply (pSfpLocalConfigMessage);
							trace(TRACE_LEVEL_ERROR,
								"SfpLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig failed");
		    				return;
						}
            		}
            		if (!applyvalue.compare("custom"))
					{     // Apply custom policy
						flag = CUSTOM_BKEND_VAL;
						trace(TRACE_LEVEL_INFO,
								"SfpLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig Custom case");
						ret = fwApplyCustomOrDefaultConfig(oui[oui_iter].c_str(), area_iter, flag);
						if (ret != 0)
						{
							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
							pSfpLocalConfigMessage->setCompletionStatus(FW_APPLY_BACKEND_ERROR);
							reply (pSfpLocalConfigMessage);
							trace(TRACE_LEVEL_ERROR,
								"SfpLocalConfigMessageHandler:fwApplyCustomOrDefaultConfig failed");
							return;
						}
            		}
         	 	}
        	}
	     // Pause or Continue SFP Monitoring
	      	int agentClass = AGENT_GBIC;
		 	int area = THA_ALL_AREAS;
		 	int port = THA_ALL_INDEX;
		 	ret = 0;
		 	if (paus ==  true) {
				trace(TRACE_LEVEL_INFO,"Entering SfpLocalConfigMessageHandler:thaThresholdDisable");
		 		ret = thaThresholdDisable (agentClass,area,port +1) ;
					if (ret != 0)
					{
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    			pSfpLocalConfigMessage->setCompletionStatus(FW_PAUSE_BACKEND_ERROR);
		    			reply (pSfpLocalConfigMessage);
						trace(TRACE_LEVEL_ERROR,
								"SfpLocalConfigMessageHandler:thaThresholdDisable failed");
		    			return;
					}
		 	} 
		 	else {
		 		ret = thaThresholdEnable (agentClass,area,port +1) ;
					if (ret != 0)
					{
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    			pSfpLocalConfigMessage->setCompletionStatus(FW_CONTINUE_BACKEND_ERROR);
		    			reply (pSfpLocalConfigMessage);
						trace(TRACE_LEVEL_ERROR,
								"SfpLocalConfigMessageHandler:thaThresholdEnable failed");
		    			return;
					}
			}		 	
		 trace(TRACE_LEVEL_INFO, "Returned from updating backend, now going to update MO");
		 startTransaction ();
		 updateWaveManagedObject (pSfpConfigLocalManagedObject);
		 if (applyvalue.compare("default") == 0)
		 	pSfpConfigLocalManagedObject->setApplyValue(applyvalue, false);
		 else
		 	pSfpConfigLocalManagedObject->setApplyValue(applyvalue, true);
		 pSfpConfigLocalManagedObject->setPause(paus);
		 status = commitTransaction ();
		 if (status != FRAMEWORK_SUCCESS)
		 {
			 trace (TRACE_LEVEL_FATAL, "SfpLocalConfigMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
		 } else {
			 status = WAVE_MESSAGE_SUCCESS;
		 }
		 WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		 //delete pSfpConfigLocalManagedObject;
		 pSfpLocalConfigMessage->setCompletionStatus (status);
		 reply (pSfpLocalConfigMessage);
	}
	int LocalSfpConfigWorker::fwBackendApplyPause()
	{
		trace (TRACE_LEVEL_INFO, "LocalSfpConfigWorker::fwBackendApplyPause Entering.");
	//	int applyVal  = DEFAULT_LEVEL;
		string applyVal("default");
		int ret = 0;
		bool pause = false;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		SfpConfigLocalManagedObject *pSfpConfigLocalManagedObject = NULL;
		SfpConfigLocalManagedObject *pSfpConfigLocalManagedObjectCxt = NULL;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpConfigLocalManagedObject::getClassName ());
		if (pResults != NULL) 
		{
			if (pResults->size() == 0) {
				trace (TRACE_LEVEL_ERROR, "LocalSfpConfigWorker::fwBackendApplyPause No SfpConfigLocalManagedObject");
				startTransaction ();
				pSfpConfigLocalManagedObject = new SfpConfigLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
				pSfpConfigLocalManagedObject->setApplyValue(applyVal, false);
				pSfpConfigLocalManagedObject->setPause(pause);
				status = commitTransaction();
				if (status == FRAMEWORK_SUCCESS)
				{
					trace (TRACE_LEVEL_INFO, "LocalSfpConfigWorker::fwBackendApplyPause - Default commit success");
				}
				delete pSfpConfigLocalManagedObject;
			} else {
				 trace (TRACE_LEVEL_INFO,
						 "LocalSfpConfigWorker::fwBackendApplyPause Entering ESS");
				pSfpConfigLocalManagedObjectCxt = dynamic_cast <SfpConfigLocalManagedObject *> ((*pResults)[0]);
				applyVal.assign(pSfpConfigLocalManagedObjectCxt->getApplyValue());  
				pause = pSfpConfigLocalManagedObjectCxt->getPause();
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			//	delete pSfpConfigLocalManagedObjectCxt;
				trace (TRACE_LEVEL_INFO, "Leaving ESS");	
			}
			trace (TRACE_LEVEL_INFO, "SfpLocalConfigMessageHandler:Applyval" + applyVal );
			// Apply custom or default config
			int area_iter = 0;
			int flag;
			unsigned int oui_iter = 0;

			vector<string> oui;
			oui.push_back("1GSR");
			oui.push_back("1GLR");
			oui.push_back("10GSR");
			oui.push_back("10GLR");
			oui.push_back("10GUSR");
			oui.push_back("QSFP");
			/* Loop through all the OUI and area types */
			for (oui_iter = 0; oui_iter < oui.size(); oui_iter++){
				for (area_iter = 0; area_iter < MAX_AREA_VAL; area_iter++){
					if (!applyVal.compare("default")){     // Apply custom policy
						flag = DEFAULT_BKEND_VAL;
						ret = fwApplyCustomOrDefaultConfig(oui[oui_iter].c_str(), area_iter, flag);
						trace (TRACE_LEVEL_INFO, string("Returned from backend after applying default config with return val...") + oui[oui_iter].c_str() + "\t" + ret);
						if (ret != 0) {
							return (ret);
						}
					}
					if (!applyVal.compare("custom")) {     // Apply default policy
						flag = CUSTOM_BKEND_VAL;
						ret =  fwApplyCustomOrDefaultConfig(oui[oui_iter].c_str(), area_iter, flag);
						trace (TRACE_LEVEL_INFO, string("Returned from backend after applying custom config with return val...") + ret);
						if (ret != 0) {
							return (ret);
						}
					}
				}
			}
			
			// Pause SFP Monitoring
			int agentClass = AGENT_GBIC;
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
		trace (TRACE_LEVEL_INFO, "Returning from LocalSfpConfigWorker::fwBackendApplyPause...");
	return (ret);
	}
}
