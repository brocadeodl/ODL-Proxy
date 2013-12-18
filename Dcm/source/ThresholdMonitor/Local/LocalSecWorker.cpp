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

/************************************************************************
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          	*
 *   All rights reserved.                                               *
 *   Author : Sachin Mishra					   	*
 ************************************************************************/


#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/LocalSecWorker.h"
#include "ThresholdMonitor/Local/SecLocalMessage.h"
#include "ThresholdMonitor/Local/SecMonitorPolicyLocalManagedObject.h"
#include "ThresholdMonitor/Local/SecLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
#include <thresh/portCmd.h>
#define FW_BKEND_ERROR  do { \
	tempAreaThreshAlertMapVector.clear(); \
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults); \
		} while (0)
#define FW_MSG_HNDLR_ERROR do { \
				trace (TRACE_LEVEL_INFO, string("LocalSfpMonitorWorker: Alert update Failed at backend") + "  " + ret); \
				pSecLocalMessage->setCompletionStatus(FW_SET_ALERT_BACKEND_ERROR); \
				reply (pSecLocalMessage);	\
				return; \
			} while(0)

namespace DcmNs
{
	LocalSecWorker::LocalSecWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
	{
		SecLocalManagedObject secLocalManagedObject(pThresholdMonitorLocalObjectManager);
		SecMonitorPolicyLocalManagedObject SecMonitorPolicyLocalManagedObject(pThresholdMonitorLocalObjectManager);
		secLocalManagedObject.setupOrm ();
		addManagedClass (SecLocalManagedObject::getClassName (), this);
		SecMonitorPolicyLocalManagedObject.setupOrm ();
		addManagedClass (SecMonitorPolicyLocalManagedObject::getClassName (),
				this);
		addOperationMap (SECLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalSecWorker::SecLocalMessageHandler));
	}

	LocalSecWorker::~LocalSecWorker ()
	{
	}

	PrismMessage  *LocalSecWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case SECLOCALCONFIG :
				pPrismMessage = new SecLocalMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);

	}

	WaveManagedObject  *LocalSecWorker::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;
		if ((SecMonitorPolicyLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new
				SecMonitorPolicyLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager
						*>(getPWaveObjectManager()));
		}
		else if ((SecLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new SecLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		}
		else
		{
			trace (TRACE_LEVEL_FATAL, "LocalSecWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
		}
		return (pWaveManagedObject);
	}

	int LocalSecWorker::fwSwitchCaseAlert(UI64 optArg, char *ptr)
	{
		int ret_success = 0, ret_failed = -1;	
		switch (optArg) {
			case NONEACT:
				strncpy(ptr, "none", 4); 
				return(ret_success);	
				break;
			case RASLOGEMAIL:	
			case ALL:
				strncpy(ptr, "raslog,email", 12); 
				return(ret_success);
				break;
			case EMAIL:
				strncpy(ptr, "email", 5);
				return(ret_success);
				break;
			case RASLOG:
				strncpy(ptr, "raslog", 6);
				return(ret_success);
				break;
			default:
				trace (TRACE_LEVEL_ERROR, "Not a supported action");
				return(ret_failed);	
				break;
		}
	}

	int LocalSecWorker::backendSync()
	{
		trace (TRACE_LEVEL_INFO, " LocalSecWorker::backendSync - Entering");
		BitMap abovehigh[MAX_SEC_AREA];
		BitMap abovelow[MAX_SEC_AREA];
		BitMap belowhigh[MAX_SEC_AREA];
		BitMap belowlow[MAX_SEC_AREA];
		struct threshold thObj[MAX_SEC_AREA] = {{'\0'}};
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		unsigned int i = 0;
		int count = 0;
		int ret = 0;
		bool usr_cfg = false;
		string policyName("custom");
		SecMonitorPolicyLocalManagedObject *pSecPolicyMo;
		SecLocalManagedObject *pSecMo[MAX_SEC_AREA];

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SecMonitorPolicyLocalManagedObject::getClassName ());
		vector<WaveManagedObjectPointer<SecLocalManagedObject> > tempAreaThreshAlertMapVector;
		if (pResults != NULL)  {
			if (pResults->size() == 0) {
				trace (TRACE_LEVEL_INFO, "LocalSecWorker :: backendSync MO not present");
				// Custom updation
				startTransaction ();
				pSecPolicyMo = new SecMonitorPolicyLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
				prismAssert (NULL != pSecPolicyMo, __FILE__, __LINE__);
				trace (TRACE_LEVEL_INFO, "LocalSecWorker::backendSync - Type MOUpdate Entering");
				pSecPolicyMo->setpolicyName (policyName, usr_cfg);
				for (i = 0; i < MAX_SEC_AREA; i++) {
					pSecMo[count] = new SecLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
					prismAssert (NULL != pSecMo[count], __FILE__, __LINE__);
					abovehigh[i].setAllValues(secTypeVal[i].sfpAlertVal.aboveHigh);
					abovelow[i].setAllValues(secTypeVal[i].sfpAlertVal.aboveLow);
					belowhigh[i].setAllValues(secTypeVal[i].sfpAlertVal.belowHigh);
					belowlow[i].setAllValues(secTypeVal[i].sfpAlertVal.belowLow);
					thObj[i].area = secTypeVal[i].area;
					thObj[i].timebase = secTypeVal[i].timebase;

					thObj[i].highTh.val = secTypeVal[i].threshVal.highthresh;
					thObj[i].buffer.val = secTypeVal[i].threshVal.buffer;
					thObj[i].lowTh.val = secTypeVal[i].threshVal.lowthresh;
					pSecMo[count]->setAreaName(secTypeVal[i].area, usr_cfg);
					pSecMo[count]->setTimebase(thObj[i].timebase);
					pSecMo[count]->setHighThresh(thObj[i].highTh.val);
					pSecMo[count]->setLowThresh(thObj[i].lowTh.val);
					pSecMo[count]->setBuffer(thObj[i].buffer.val);
					pSecMo[count]->setAboveHighThreshAction(abovehigh[i]);
					pSecMo[count]->setAboveLowThreshAction(abovelow[i]);
					pSecMo[count]->setBelowHighThreshAction(belowhigh[i]);
					pSecMo[count]->setBelowLowThreshAction(belowlow[i]);
					pSecPolicyMo->addAreaThreshAlertMap(pSecMo[count]);
					count ++;
				}
				status = commitTransaction();
				if (pSecPolicyMo != NULL)
					delete pSecPolicyMo;
				if (status != FRAMEWORK_SUCCESS) {
					trace (TRACE_LEVEL_INFO, string("LocalSecWorker :: Commit transaction falied") + status);
					return (status);
				}	
			} else {
					trace (TRACE_LEVEL_INFO, "LocalSecWorker :: MO is already present");
					pSecPolicyMo = dynamic_cast<SecMonitorPolicyLocalManagedObject *>((*pResults)[0]);
					if ((pSecPolicyMo->getpolicyName()).compare(policyName) == 0)
						tempAreaThreshAlertMapVector =	pSecPolicyMo->getAreaThresholdAlertMap();
					unsigned int iter ;	
					//	areaName = i;
					for (iter = 0;  iter < tempAreaThreshAlertMapVector.size(); iter ++)
					{
						thObj[iter].area =
							(tempAreaThreshAlertMapVector[iter]->m_areaname.getUI32Value());
						thObj[iter].timebase =	tempAreaThreshAlertMapVector[iter]->m_timebase;
						thObj[iter].highTh.val = tempAreaThreshAlertMapVector[iter]->m_highthresh;
						thObj[iter].lowTh.val =  tempAreaThreshAlertMapVector[iter]->m_lowthresh;
						thObj[iter].buffer.val = tempAreaThreshAlertMapVector[iter]->m_buffer;
						abovehigh[iter] = tempAreaThreshAlertMapVector[iter]->m_abovehighthreshaction;
						abovelow[iter] =  tempAreaThreshAlertMapVector[iter]->m_abovelowthreshaction;
						belowhigh[iter] = tempAreaThreshAlertMapVector[iter]->m_belowhighthreshaction;
						belowlow[iter] =  tempAreaThreshAlertMapVector[iter]->m_belowlowthreshaction;
					}
			}
			int ret_fail = -1;
			for (i = 0; i < MAX_SEC_AREA; i++) {
				thObj[i].highTh.valFound = 1;
				thObj[i].lowTh.valFound = 1;
				thObj[i].buffer.valFound = 1;
				thObj[i].saveFound = 1;
				thObj[i].save = SAVE_ONLY;
				thObj[i].ouiFound = 0;
				thObj[i].areaFound = 1;
				thObj[i].timeBaseFound = 1;
				ret = fwdSetCmdConfig(AGENT_SECURITY, thObj[i].area, &thObj[i], OP_THRESHOLD);
				if (ret != 0) {
					trace (TRACE_LEVEL_INFO,string("LocalSecWorker backendSync Threshold failed with ret ..") + ret);
					FW_BKEND_ERROR;
					return (ret);
				}

				thObj[i].highTh.actionFound = 1;
				thObj[i].highTh.boundry = thAbove;
				thObj[i].highTh.boundryFound = 1;
				if (fwSwitchCaseAlert(abovehigh[i].getElemValue(), thObj[i].highTh.actions) != 0)
				{
					trace (TRACE_LEVEL_INFO,"LocalSecWorker backendSync updation failed in switch case");
					FW_BKEND_ERROR;
					return(ret_fail);
				}
				ret = fwdSetCmdConfig(AGENT_SECURITY, thObj[i].area, &thObj[i], OP_ALERT);
				if (ret != 0) {
					trace (TRACE_LEVEL_INFO,string("LocalSecWorker backendSync Alert above high failed with ret ..") + ret);
					FW_BKEND_ERROR;
					return (ret);
				}
				thObj[i].highTh.boundry = thBelow;
				if (fwSwitchCaseAlert(belowhigh[i].getElemValue(), thObj[i].highTh.actions) != 0)
				{
					trace (TRACE_LEVEL_INFO,"LocalSecWorker backendSync updation failed in switch case");
					FW_BKEND_ERROR;
					return(ret_fail);
				}
				ret = fwdSetCmdConfig(AGENT_SECURITY, thObj[i].area, &thObj[i], OP_ALERT);
				if (ret != 0) {
					trace (TRACE_LEVEL_INFO,string("LocalSecWorker backendSync Alert below high failed with ret ..") + ret);
					FW_BKEND_ERROR;
					return (ret);
				}

				thObj[i].lowTh.actionFound = 1;
				thObj[i].lowTh.boundry = thBelow;
				thObj[i].lowTh.boundryFound = 1;
				if (fwSwitchCaseAlert(belowlow[i].getElemValue(), thObj[i].lowTh.actions) != 0)
				{
					trace (TRACE_LEVEL_INFO,"LocalSecWorker backendSync updation failed in switch case");
					FW_BKEND_ERROR;
					return(ret_fail);
				}
				ret = fwdSetCmdConfig(AGENT_SECURITY, thObj[i].area, &thObj[i], OP_ALERT);
				if (ret != 0) {
					trace (TRACE_LEVEL_INFO,string("LocalSecWorker backendSync Alert below low  failed with ret ..") + ret);
					FW_BKEND_ERROR;
					return (ret);
				}
			}
			FW_BKEND_ERROR;
		}
		return (ret);
	}

	void LocalSecWorker::SecLocalMessageHandler( SecLocalMessage *pSecLocalMessage)
	{
		trace (TRACE_LEVEL_INFO, "Entering SecLocalMessageHandler..");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (pSecLocalMessage == NULL)
		{
			trace (TRACE_LEVEL_INFO, "SecLocalMessageHandler : Empty Meassage");
			prismAssert (false, __FILE__, __LINE__);
		}
		string policyName ;
		policyName.assign(pSecLocalMessage->getPolicyName());
		unsigned int area = pSecLocalMessage->getAreaName();
		unsigned int timebase = pSecLocalMessage->getTimebase();
		int highthresh = pSecLocalMessage->getHighThresh();
		int lowthresh = pSecLocalMessage->getLowThresh();
		int bufferthresh = pSecLocalMessage->getBuffer();
                BitMap abovehighthreshactionBM = pSecLocalMessage->getAboveHighThreshAction();
                BitMap abovelowthreshactionBM = pSecLocalMessage->getAboveLowThreshAction();
                BitMap belowhighthreshactionBM = pSecLocalMessage->getBelowHighThreshAction();
                BitMap belowlowthreshactionBM = pSecLocalMessage->getBelowLowThreshAction();
                trace (TRACE_LEVEL_INFO, string("Value for alert at plugin ") + abovehighthreshactionBM.getElemValue());
		trace (TRACE_LEVEL_INFO, string("Value for high, low, buffer ") + highthresh + "\n" + lowthresh + "\n" + bufferthresh);
		if (lowthresh > highthresh) {
			status = INVALID_THRESH_VALUE;
			pSecLocalMessage->setCompletionStatus(status);
			reply (pSecLocalMessage);
			return;
		}
		if ((bufferthresh < 0) || 
				(bufferthresh > (highthresh - lowthresh)/2)) {
			status = INVALID_BUFFER_VAL;
			pSecLocalMessage->setCompletionStatus(status);
			reply (pSecLocalMessage);
			return;
		}
		int ret = 0;
		struct threshold thObj;
		memset(&thObj, 0, sizeof (thObj));
		thObj.area = area;
		thObj.areaFound = 1;
		thObj.timebase = timebase;
		thObj.timeBaseFound = 1; 
		thObj.highTh.val = highthresh;
		thObj.highTh.valFound = 1;
		thObj.lowTh.val = lowthresh;
		thObj.lowTh.valFound = 1;
		thObj.buffer.val = bufferthresh;
		thObj.buffer.valFound = 1;
		thObj.saveFound = 1;
		thObj.save = SAVE_ONLY;
		thObj.ouiFound = 0;
		trace (TRACE_LEVEL_INFO, "Set threshold values");
		ret = fwdSetCmdConfig(AGENT_SECURITY, area, &thObj, OP_THRESHOLD);
		if (ret != 0) {
            trace (TRACE_LEVEL_INFO, string("LocalSfpMonitorWorker: Threshold update Failed at backend  ") + ret);
            if (thInvalidBS == ret) {
                pSecLocalMessage->setCompletionStatus(FW_SET_BUFFER_BACKEND_ERROR);
            } else if ((thInvalidHigh == ret)||(thInvalidLow == ret)) {
                pSecLocalMessage->setCompletionStatus(FW_INVALID_THRESH_VALUE);
            } else {
                pSecLocalMessage->setCompletionStatus(FW_SET_THRESHOLD_BACKEND_ERROR);
            }
            reply (pSecLocalMessage);
            return;
		}
		// Revert timebaseFound to 0 as this is already handled.
		thObj.timeBaseFound = 0; 
		if (abovehighthreshactionBM.getElemValue()) {
			thObj.highTh.actionFound = 1;
			thObj.highTh.boundry = thAbove;
			thObj.highTh.boundryFound = 1;
			trace (TRACE_LEVEL_INFO, "Set abovehighthreshaction values");
			if(fwSwitchCaseAlert(abovehighthreshactionBM.getElemValue (), thObj.highTh.actions) != 0) {
				FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_SECURITY, area, &thObj, OP_ALERT);
			if (ret != 0) {
				FW_MSG_HNDLR_ERROR;
			} 
		}
		if (belowhighthreshactionBM.getElemValue()) {
			thObj.highTh.actionFound = 1;
			thObj.highTh.boundry = thBelow;
			thObj.highTh.boundryFound = 1;
			trace (TRACE_LEVEL_INFO, "Set belowhighthreshaction values");
			if(fwSwitchCaseAlert(belowhighthreshactionBM.getElemValue (), thObj.highTh.actions) != 0) {	
				FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_SECURITY, area, &thObj, OP_ALERT);
			if (ret != 0) {
				FW_MSG_HNDLR_ERROR;
			} 
		}
		if (belowlowthreshactionBM.getElemValue()) {
			thObj.lowTh.actionFound = 1;
			thObj.lowTh.boundry = thBelow;
			thObj.lowTh.boundryFound = 1;
			trace (TRACE_LEVEL_INFO, "Set belowlowthreshaction values");
			if(fwSwitchCaseAlert(belowlowthreshactionBM.getElemValue (), thObj.lowTh.actions) != 0) {
				FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_SECURITY, area, &thObj, OP_ALERT);
			if (ret != 0) {
				FW_MSG_HNDLR_ERROR;
			} 
		}
		trace(TRACE_LEVEL_INFO, "LocalSecWorker: Back from backend");
		SecMonitorPolicyLocalManagedObject *pSecPolicyMo = NULL;

		SecLocalManagedObject *pSecMo = NULL;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SecMonitorPolicyLocalManagedObject::getClassName ());
		if (pResults->size() == 0)
		{
			trace (TRACE_LEVEL_ERROR, "SecLocalMessageHandler : No such Sfp type Managed Object");
			pSecLocalMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
			reply (pSecLocalMessage);
			return;
		}

		pSecPolicyMo = dynamic_cast <SecMonitorPolicyLocalManagedObject *> ((*pResults)[0]);
		unsigned int iter;
		vector<WaveManagedObjectPointer<SecLocalManagedObject> > tempAreaThreshAlertMapVector;
		//   Currently custom policy alone is supported. If more policies are
		//   supported, iterate through pResults
		if ((pSecPolicyMo->getpolicyName()).compare(policyName) == 0)
		{
			pSecPolicyMo->m_policyName.setIsUserConfigured(true);
			startTransaction ();
			updateWaveManagedObject (pSecPolicyMo);
			status = commitTransaction ();
			if (status != FRAMEWORK_SUCCESS)
			{
				trace (TRACE_LEVEL_FATAL, "SecLocalMessageHandler:Commiting	failed.Status:" + FrameworkToolKit::localize (status));
			//	delete pSecPolicyMo;
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				pSecLocalMessage->setCompletionStatus(status);
				reply (pSecLocalMessage);
				return;
			}
		}
		tempAreaThreshAlertMapVector =
			pSecPolicyMo->getAreaThresholdAlertMap();
		bool usr_cfg = true;
		for (iter = 0;  iter < tempAreaThreshAlertMapVector.size(); iter ++)
		{
			if ((tempAreaThreshAlertMapVector[iter]->m_areaname.getUI32Value()) == area)
				break;
		}
		pSecMo = tempAreaThreshAlertMapVector[iter].operator->();
		pSecMo->setAreaName(area, usr_cfg);
		pSecMo->setTimebase(timebase);
		pSecMo->setHighThresh(highthresh);
		pSecMo->setLowThresh(lowthresh);
		pSecMo->setBuffer(bufferthresh); 
		if (abovehighthreshactionBM.getElemValue() )
			pSecMo->setAboveHighThreshAction(abovehighthreshactionBM);
		if (abovelowthreshactionBM.getElemValue() )
			pSecMo->setAboveLowThreshAction(abovelowthreshactionBM);
		if (belowhighthreshactionBM.getElemValue() )
			pSecMo->setBelowHighThreshAction(belowhighthreshactionBM);
		if (belowlowthreshactionBM.getElemValue() )
			pSecMo->setBelowLowThreshAction(belowlowthreshactionBM);

		startTransaction (); 
		updateWaveManagedObject (pSecMo);
		status = commitTransaction();
		if (status != FRAMEWORK_SUCCESS)
		{
			trace (TRACE_LEVEL_FATAL, "SecLocalMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
		} else 
		{
			status = WAVE_MESSAGE_SUCCESS;
		}
//		delete pSecPolicyMo;
		tempAreaThreshAlertMapVector.clear();
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		pSecLocalMessage->setCompletionStatus(status);
		reply (pSecLocalMessage);
	}
}
