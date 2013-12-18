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
#include "ThresholdMonitor/Local/LocalSfpMonitorWorker.h"
#include "ThresholdMonitor/Local/SfpMonitorLocalConfigMessage.h"
#include "ThresholdMonitor/Local/SfpMonitorTypeLocalManagedObject.h"
#include "ThresholdMonitor/Local/SfpMonitorAreaLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include <thresh/fwd.h>
#include <thresh/fwdPub.h>
#include <thresh/portCmd.h>

#define FW_BKEND_ERROR  do { \
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsType); \
				tempAreaThreshAlertMapVector.clear(); \
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults); \
			} while(0)
#define FW_MSG_HNDLR_ERROR do { \
				trace (TRACE_LEVEL_INFO, "LocalSfpMonitorWorker: Alert update Failed at backend"); \
				pSfpMonitorLocalConfigMessage->setCompletionStatus(FW_SET_ALERT_BACKEND_ERROR); \
				reply (pSfpMonitorLocalConfigMessage);	\
				return; \
			} while(0)

namespace DcmNs
{
	LocalSfpMonitorWorker::LocalSfpMonitorWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
		{

			SfpMonitorAreaLocalManagedObject SfpMonitorAreaLocalManagedObject
				(pThresholdMonitorLocalObjectManager);

			SfpMonitorTypeLocalManagedObject SfpMonitorTypeLocalManagedObject	(pThresholdMonitorLocalObjectManager);
			SfpMonitorAreaLocalManagedObject.setupOrm ();
			addManagedClass (SfpMonitorAreaLocalManagedObject::getClassName (),
					this);
			SfpMonitorTypeLocalManagedObject.setupOrm ();
			addManagedClass (SfpMonitorTypeLocalManagedObject::getClassName (),
					this);
			addOperationMap (SFPMONITORLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalSfpMonitorWorker::SfpMonitorLocalConfigMessageHandler));
		}
	LocalSfpMonitorWorker::~LocalSfpMonitorWorker ()
	{
	}
	PrismMessage  *LocalSfpMonitorWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case SFPMONITORLOCALCONFIG :
				pPrismMessage = new SfpMonitorLocalConfigMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
			
	}

	WaveManagedObject  *LocalSfpMonitorWorker::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;
		if ((SfpMonitorTypeLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new SfpMonitorTypeLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		}
		 else if ((SfpMonitorAreaLocalManagedObject::getClassName ()) == managedClassName)
		 {
			pWaveManagedObject = new SfpMonitorAreaLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		 }
		else
		{
			trace (TRACE_LEVEL_FATAL, "LocalSfpMonitorWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
		}
		 return (pWaveManagedObject);
	}
	int LocalSfpMonitorWorker::fwSwitchCaseAlert(UI64 optArg, char *ptr)
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
	int LocalSfpMonitorWorker::fwBackendOpSfpmonitorThreshold()
	{
		trace (TRACE_LEVEL_INFO, " LocalSfpMonitorWorker::fwBackendOpSfpmonitorThreshold - Entering");
		int j = 0, i = 0;
		int count = 0;
		int ret = 0, ret_fail = -1;;
		bool usr_cfg = false;
		//unsigned int policyName = 0;
		string policyName("custom");
		vector<WaveManagedObjectPointer<SfpMonitorAreaLocalManagedObject> > tempAreaThreshAlertMapVector;
		vector<WaveManagedObject *> *pResultsType = NULL;	
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		SfpMonitorTypeLocalManagedObject *pSfpmonitorTypeMo;
		SfpMonitorAreaLocalManagedObject *pSfpmonitorAreaMo[MAX_OUI_VAL * MAX_AREA_VAL];
		WaveManagedObjectPointer<SfpMonitorAreaLocalManagedObject> areaThreshAlertMap;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpMonitorTypeLocalManagedObject::getClassName ());
		if (pResults != NULL)  {
			/* Type MO is not present. Create it now */
			if (pResults->size() == 0) {
				BitMap aboveHigh;
				BitMap aboveLow;
				BitMap belowHigh;
				BitMap belowLow;
				trace (TRACE_LEVEL_ERROR, "LocalSfpMonitorWorker::fwBackendOpSfpmonitorThreshold No Type MO");
					// Custom updation
				startTransaction ();
				pSfpmonitorTypeMo = new SfpMonitorTypeLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
				prismAssert (NULL != pSfpmonitorTypeMo,__FILE__, __LINE__);
				trace (TRACE_LEVEL_INFO, "LocalSfpMonitorWorker::fwBackendOpSfpmonitorThreshold - Type MO Update Entering");
				pSfpmonitorTypeMo->setpolicyName (policyName, usr_cfg);

				for (i = 0 ; i < MAX_OUI_VAL; i++) {
					aboveHigh.setAllValues(sfptypeval[i].sfpAlertVal.aboveHigh);
					aboveLow.setAllValues(sfptypeval[i].sfpAlertVal.aboveLow);
					belowHigh.setAllValues(sfptypeval[i].sfpAlertVal.belowHigh);
					belowLow.setAllValues(sfptypeval[i].sfpAlertVal.belowLow);
					// For a given SFP type, create all the area MO
					for (j = 0; j <  MAX_AREA_VAL; j++) {
						pSfpmonitorAreaMo[count] = new SfpMonitorAreaLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));  
						prismAssert (NULL != pSfpmonitorAreaMo[count],__FILE__, __LINE__);	
						pSfpmonitorAreaMo[count]->setSfptypeName(i, usr_cfg);
						pSfpmonitorAreaMo[count]->setAreaName(j, usr_cfg);
						pSfpmonitorAreaMo[count]->setHighThresh(sfptypeval[i].sfpareaval[j].highthresh);
						pSfpmonitorAreaMo[count]->setLowThresh(sfptypeval[i].sfpareaval[j].lowthresh);
						pSfpmonitorAreaMo[count]->setBuffer(sfptypeval[i].sfpareaval[j].buffer);
						pSfpmonitorAreaMo[count]->setAboveHighThreshAction(aboveHigh);
						pSfpmonitorAreaMo[count]->setAboveLowThreshAction(aboveLow);
						pSfpmonitorAreaMo[count]->setBelowHighThreshAction(belowHigh);
						pSfpmonitorAreaMo[count]->setBelowLowThreshAction(belowLow);
						pSfpmonitorTypeMo->addAreaThreshAlertMap(pSfpmonitorAreaMo[count]);
						count++;
					}
				}
				status = commitTransaction();
				if (status == FRAMEWORK_SUCCESS) {
					trace (TRACE_LEVEL_INFO,
							"LocalSfpMonitorWorker::fwBackendOpSfpmonitorThreshold -Default commit success");
				}
				else {
					trace (TRACE_LEVEL_ERROR,string("LocalSfpMonitorWorker::fwBackendOpSfpmonitorThreshold -Default commit failed")+ FrameworkToolKit::localize(status));
					delete pSfpmonitorTypeMo;
					return (ret_fail);
				}
				if (pSfpmonitorTypeMo != NULL)
					delete pSfpmonitorTypeMo;
			} 
			trace (TRACE_LEVEL_INFO, "LocalSfpMonitorWorker::fwBackendOpSfpmonitorThreshold MO exists");
		//	policyName = CUSTOM_POLICY;
	//		string policyname("custom");
			unsigned int areaName  = 0, sfptypeName = 0;
			i = 0;
			char sfptype[MAX_OUI_LEN] = {0};
			struct threshold thObj;
			unsigned int iter;
			pResultsType = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpMonitorTypeLocalManagedObject::getClassName ());

			if((pResultsType != NULL)&&(pResultsType->size() > 0)) {
				pSfpmonitorTypeMo = dynamic_cast <SfpMonitorTypeLocalManagedObject *> ((*pResultsType)[0]);
				if ((pSfpmonitorTypeMo->getpolicyName()).compare(policyName) == 0)
					tempAreaThreshAlertMapVector = pSfpmonitorTypeMo->getAreaThresholdAlertMap();

				for ( i = 0 ; i < MAX_OUI_VAL; i++) {
					sfptypeName = i;
					for (j  = 0; j < MAX_AREA_VAL; j++) {
						areaName = j;
						for (iter = 0;  iter < tempAreaThreshAlertMapVector.size(); iter ++)
						{
							if
								(((tempAreaThreshAlertMapVector[iter]->m_areaname.getUI32Value()) ==
										areaName) &&
									((tempAreaThreshAlertMapVector[iter]->m_sfptypename.getUI32Value())
									 == sfptypeName)) 
								break;
						}

						memset(&thObj, 0, sizeof (thObj));
						thObj.area = areaName;
						thObj.areaFound = SET;
						thObj.timeBaseFound = UNSET; 
						thObj.timebase = SECONDS;
						thObj.highTh.val = tempAreaThreshAlertMapVector[iter]->m_highthresh;
						thObj.highTh.valFound = SET;
						thObj.lowTh.val = tempAreaThreshAlertMapVector[iter]->m_lowthresh;
						thObj.lowTh.valFound = SET;
						thObj.buffer.val = tempAreaThreshAlertMapVector[iter]->m_buffer;
						thObj.buffer.valFound = SET;
						thObj.saveFound = SET;
						thObj.save = SAVE_ONLY;
						thObj.ouiFound = SET;
						switch (i) {
							case 0:
								strncpy(sfptype, "1GSR", 4);
								break;
							case 1:
								strncpy(sfptype, "1GLR", 4);
								break;
							case 2:
								strncpy(sfptype, "10GSR", 5);
								break;
							case 3:
								strncpy(sfptype, "10GLR", 5);
								break;
							case 4:
								strncpy(sfptype, "10GUSR", 6);
								break;
							case 5:
								strncpy(sfptype, "QSFP", 4);
								break;
							default:
								break;
						}
						strncpy(thObj.oui, sfptype, strlen(sfptype));
						ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_THRESHOLD);
						if (ret != 0) {
							trace (TRACE_LEVEL_INFO,string("fwBackendOpSfpmonitorThreshold Backend updation failed with ret..") + ret);	
							FW_BKEND_ERROR;
							return (ret);
						}
						thObj.highTh.actionFound = SET;
						thObj.highTh.boundry = thAbove;
						thObj.highTh.boundryFound = SET;
						if(fwSwitchCaseAlert((tempAreaThreshAlertMapVector[iter]->m_abovehighthreshaction).getElemValue(), thObj.highTh.actions) != 0) {
							trace (TRACE_LEVEL_INFO,"fwBackendOpSfpmonitorThreshold Backend updation failed in switch case");
							FW_BKEND_ERROR;
							return(ret_fail);
						} 
						ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_ALERT);
						if (ret != 0) {
							 trace
								 (TRACE_LEVEL_INFO,string("fwBackendOpSfpmonitorThreshold Alert Abovehigh Backend updation failed with ret..") + ret);
							FW_BKEND_ERROR;
							return (ret);
						}
						thObj.highTh.actionFound = SET;
						thObj.highTh.boundry = thBelow;
						thObj.highTh.boundryFound = SET;
						if (fwSwitchCaseAlert((tempAreaThreshAlertMapVector[iter]->m_belowhighthreshaction).getElemValue(), thObj.highTh.actions) != 0) {
							trace (TRACE_LEVEL_INFO,"fwBackendOpSfpmonitorThreshold Backend updation failed in switch case");
							FW_BKEND_ERROR;
							return(ret_fail);
						} 
						ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_ALERT);
						if (ret != 0) {
							trace(TRACE_LEVEL_INFO,string("fwBackendOpSfpmonitorThreshold Alert belowhigh Backend updation failed with ret..") + ret);
							FW_BKEND_ERROR;
							return (ret);
						}
						thObj.lowTh.actionFound = SET;
						thObj.lowTh.boundry = thBelow;
						thObj.lowTh.boundryFound = SET;
						if (fwSwitchCaseAlert((tempAreaThreshAlertMapVector[iter]->m_belowlowthreshaction).getElemValue(), thObj.lowTh.actions) != 0) {
							trace (TRACE_LEVEL_INFO,"fwBackendOpSfpmonitorThreshold Backend updation failed in switch case");
							FW_BKEND_ERROR;
							return(ret_fail);
						} 
						ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_ALERT);
						if (ret != 0) {
							trace(TRACE_LEVEL_INFO,string("fwBackendOpSfpmonitorThreshold Alert belowlow  Backend updation failed with ret..") + ret);
							FW_BKEND_ERROR;
							return (ret);
						}
					}
				}
				FW_BKEND_ERROR;
			}
		}
		return (ret);
	}
	void LocalSfpMonitorWorker::SfpMonitorLocalConfigMessageHandler( SfpMonitorLocalConfigMessage *pSfpMonitorLocalConfigMessage)
	{
		trace (TRACE_LEVEL_INFO, "Entering SfpMonitorLocalConfigMessageHandler..");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (pSfpMonitorLocalConfigMessage == NULL)
		{
			trace (TRACE_LEVEL_INFO, "SfpMonitorLocalConfigMessageHandler : Empty Message");
			prismAssert (false, __FILE__, __LINE__);
		}
		string policyName ;
		policyName.assign(pSfpMonitorLocalConfigMessage->getPolicyName());
		unsigned int sfptypename = pSfpMonitorLocalConfigMessage->getSfpTypeName();
		unsigned int areaName = pSfpMonitorLocalConfigMessage->getAreaName();
		int highthresh = pSfpMonitorLocalConfigMessage->getHighThresh();
		int lowthresh = pSfpMonitorLocalConfigMessage->getLowThresh();
		int bufferthresh = pSfpMonitorLocalConfigMessage->getBufferThresh(); 
		BitMap abovehighthreshactionBM = pSfpMonitorLocalConfigMessage->getAboveHighThreshAction();
		BitMap abovelowthreshactionBM = pSfpMonitorLocalConfigMessage->getAboveLowThreshAction();
		BitMap belowhighthreshactionBM = pSfpMonitorLocalConfigMessage->getBelowHighThreshAction();
		BitMap belowlowthreshactionBM = pSfpMonitorLocalConfigMessage->getBelowLowThreshAction();
		trace (TRACE_LEVEL_INFO, string("Value for alert at plugin ") + abovehighthreshactionBM.getElemValue());
								
		if (lowthresh > highthresh) {
			status = INVALID_THRESH_VALUE;
			pSfpMonitorLocalConfigMessage->setCompletionStatus(status);
			reply (pSfpMonitorLocalConfigMessage);
			return;
		}
		if ((bufferthresh < 0) || 
				(bufferthresh > (highthresh - lowthresh)/2)) {
			status = INVALID_BUFFER_VAL;
			pSfpMonitorLocalConfigMessage->setCompletionStatus(status);
			reply (pSfpMonitorLocalConfigMessage);
			return;
		}
		char sfptype[MAX_OUI_LEN] = {0};
		int ret = 0;
		struct threshold thObj;
		memset(&thObj, 0, sizeof (thObj));
		thObj.area = areaName;
		thObj.areaFound = SET;
		thObj.timeBaseFound = UNSET; 
		thObj.timebase = SECONDS;
		thObj.highTh.val = highthresh;
		thObj.highTh.valFound = SET;
		thObj.lowTh.val = lowthresh;
		thObj.lowTh.valFound = SET;
		thObj.buffer.val = bufferthresh;
		thObj.buffer.valFound = SET;
		thObj.saveFound = SET;
		thObj.save = SAVE_ONLY;
		thObj.ouiFound = SET;

		switch (sfptypename) {
			case 0:
				strncpy(sfptype, "1GSR", 4);
				break;
			case 1:
				strncpy(sfptype, "1GLR", 4);
				break;
			case 2:
				strncpy(sfptype, "10GSR", 5);
				break;
			case 3:
				strncpy(sfptype, "10GLR", 5);
				break;
			case 4:
				strncpy(sfptype, "10GUSR", 6);
				break;
			case 5:
				strncpy(sfptype, "QSFP", 4);
				break;
			default:
				break;
		}
		strncpy(thObj.oui, sfptype, strlen(sfptype));

		if (abovehighthreshactionBM.getElemValue()) {
			thObj.highTh.actionFound = SET;
			thObj.highTh.boundry = thAbove;
			thObj.highTh.boundryFound = SET;
			if(fwSwitchCaseAlert(abovehighthreshactionBM.getElemValue (), thObj.highTh.actions) != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_ALERT);
			if (ret != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
		}
		if (belowhighthreshactionBM.getElemValue()) {
			thObj.highTh.actionFound = SET;
			thObj.highTh.boundry = thBelow;
			thObj.highTh.boundryFound = SET;
			if(fwSwitchCaseAlert(belowhighthreshactionBM.getElemValue (), thObj.highTh.actions) != 0) {	
			 FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_ALERT);
			if (ret != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
		}
		if (belowlowthreshactionBM.getElemValue()) {
			thObj.lowTh.actionFound = SET;
			thObj.lowTh.boundry = thBelow;
			thObj.lowTh.boundryFound = SET;
			if(fwSwitchCaseAlert(belowlowthreshactionBM.getElemValue (), thObj.lowTh.actions) != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_ALERT);
			if (ret != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
		}
		ret = fwdSetCmdConfig(AGENT_GBIC, areaName, &thObj, OP_THRESHOLD);
		trace(TRACE_LEVEL_INFO, string("Return value of SfpMonitorLocalConfigMessageHandler: fwdSetCmdConfig is ") + ret);
		if (ret != 0) {
			trace (TRACE_LEVEL_INFO, "LocalSfpMonitorWorker: Threshold config update Failed at backend");
			pSfpMonitorLocalConfigMessage->setCompletionStatus(FW_SET_THRESHOLD_BACKEND_ERROR);
			reply (pSfpMonitorLocalConfigMessage);
			return;
		} 	
		SfpMonitorTypeLocalManagedObject *psfpmonitortypeMO = NULL;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpMonitorTypeLocalManagedObject::getClassName ());
		if (pResults->size() == 0)
		{
			trace (TRACE_LEVEL_ERROR, "SfpMonitorLocalConfigMessageHandler : No such Sfp type Managed Object");
			pSfpMonitorLocalConfigMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
			reply (pSfpMonitorLocalConfigMessage);
			return;
		}
			
		psfpmonitortypeMO = dynamic_cast <SfpMonitorTypeLocalManagedObject *> ((*pResults)[0]);
		unsigned int iter;
		vector<WaveManagedObjectPointer<SfpMonitorAreaLocalManagedObject> > tempAreaThreshAlertMapVector;
		//   Currently custom policy alone is supported. If more policies are
		//   supported, iterate through pResults
		if ((psfpmonitortypeMO->getpolicyName()).compare(policyName) == 0)
		{
			psfpmonitortypeMO->m_policyName.setIsUserConfigured(true);
			startTransaction ();
			updateWaveManagedObject (psfpmonitortypeMO);
			status = commitTransaction ();
			if (status != FRAMEWORK_SUCCESS)
			{	
				trace (TRACE_LEVEL_FATAL, "SfpMonitorLocalConfigMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
			//	delete psfpmonitortypeMO;
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				pSfpMonitorLocalConfigMessage->setCompletionStatus(status);
				reply (pSfpMonitorLocalConfigMessage);
				return;
			}
		}
		tempAreaThreshAlertMapVector = psfpmonitortypeMO->getAreaThresholdAlertMap();
		bool usr_cfg = true;
		for (iter = 0;  iter < tempAreaThreshAlertMapVector.size(); iter ++)
		{
			if
				(((tempAreaThreshAlertMapVector[iter]->m_areaname.getUI32Value()) == areaName) &&
					((tempAreaThreshAlertMapVector[iter]->m_sfptypename.getUI32Value()) == 
					 sfptypename))
				break;
		}

		SfpMonitorAreaLocalManagedObject *pSfpMonitorAreaMO = tempAreaThreshAlertMapVector[iter].operator->();
		pSfpMonitorAreaMO->setSfptypeName(sfptypename, usr_cfg);
		pSfpMonitorAreaMO->setAreaName(areaName, usr_cfg);
	 	pSfpMonitorAreaMO->setHighThresh(highthresh);
	 	pSfpMonitorAreaMO->setLowThresh(lowthresh);
	 	pSfpMonitorAreaMO->setBuffer(bufferthresh);
		if (abovehighthreshactionBM.getElemValue() )
			pSfpMonitorAreaMO->setAboveHighThreshAction(abovehighthreshactionBM);
		if (abovelowthreshactionBM.getElemValue() )
			pSfpMonitorAreaMO->setAboveLowThreshAction(abovelowthreshactionBM);
		if (belowhighthreshactionBM.getElemValue() )
			pSfpMonitorAreaMO->setBelowHighThreshAction(belowhighthreshactionBM);
		if (belowlowthreshactionBM.getElemValue() )
			pSfpMonitorAreaMO->setBelowLowThreshAction(belowlowthreshactionBM);

		startTransaction (); 
		updateWaveManagedObject (pSfpMonitorAreaMO);
		status = commitTransaction();
		if (status != FRAMEWORK_SUCCESS)
		{
			trace (TRACE_LEVEL_FATAL, "SfpMonitorLocalConfigMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
		} else 
		{
			status = WAVE_MESSAGE_SUCCESS;
		}
		//delete psfpmonitortypeMO;
		tempAreaThreshAlertMapVector.clear();
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		pSfpMonitorLocalConfigMessage->setCompletionStatus(status);
		reply (pSfpMonitorLocalConfigMessage);
	}
}
