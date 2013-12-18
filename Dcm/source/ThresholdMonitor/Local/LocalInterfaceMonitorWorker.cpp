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
#include "ThresholdMonitor/Local/LocalInterfaceMonitorWorker.h"
#include "ThresholdMonitor/Local/InterfaceMonitorLocalConfigMessage.h"
#include "ThresholdMonitor/Local/InterfaceMonitorTypeLocalManagedObject.h"
#include "ThresholdMonitor/Local/InterfaceMonitorAreaLocalManagedObject.h"
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
				trace (TRACE_LEVEL_INFO, "LocalInterfaceMonitorWorker: Alert update Failed at backend"); \
				pInterfaceMonitorLocalConfigMessage->setCompletionStatus(FW_SET_ALERT_BACKEND_ERROR); \
				reply (pInterfaceMonitorLocalConfigMessage);	\
				return; \
			} while(0)

namespace DcmNs
{
	LocalInterfaceMonitorWorker::LocalInterfaceMonitorWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
		{

			InterfaceMonitorAreaLocalManagedObject InterfaceMonitorAreaLocalManagedObject
				(pThresholdMonitorLocalObjectManager);

			InterfaceMonitorTypeLocalManagedObject InterfaceMonitorTypeLocalManagedObject	(pThresholdMonitorLocalObjectManager);
			InterfaceMonitorAreaLocalManagedObject.setupOrm ();
			addManagedClass (InterfaceMonitorAreaLocalManagedObject::getClassName (),
					this);
			InterfaceMonitorTypeLocalManagedObject.setupOrm ();
			addManagedClass (InterfaceMonitorTypeLocalManagedObject::getClassName (),
					this);
			addOperationMap (INTERFACEMONITORLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalInterfaceMonitorWorker::InterfaceMonitorLocalConfigMessageHandler));
		}
	LocalInterfaceMonitorWorker::~LocalInterfaceMonitorWorker ()
	{
	}
	PrismMessage  *LocalInterfaceMonitorWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case INTERFACEMONITORLOCALCONFIG :
				pPrismMessage = new InterfaceMonitorLocalConfigMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
			
	}

	WaveManagedObject  *LocalInterfaceMonitorWorker::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;
		if ((InterfaceMonitorTypeLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new InterfaceMonitorTypeLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		}
		 else if ((InterfaceMonitorAreaLocalManagedObject::getClassName ()) == managedClassName)
		 {
			pWaveManagedObject = new InterfaceMonitorAreaLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
		 }
		else
		{
			trace (TRACE_LEVEL_FATAL, "LocalInterfaceMonitorWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
		}
		 return (pWaveManagedObject);
	}
	int LocalInterfaceMonitorWorker::fwSwitchCaseAlert(UI64 optArg, char *ptr)
	{
		int ret_success = 0, ret_failed = -1, ret_pf = 1;	
		switch (optArg) {
			case NONEACT:
				strncpy(ptr, "none", 4);
				return(ret_success);	
				break;
			case ALL:
			case (RASLOG+EMAIL+PORTFENCE):
				strncpy(ptr, "raslog,email", 12);
				return(ret_pf);
				break;
			case (RASLOG+PORTFENCE):
				strncpy(ptr, "raslog", 6);
				return(ret_pf);
				break;
			case EMAIL:
				strncpy(ptr, "email", 5);
				return(ret_success);
				break;
			case RASLOG:
				strncpy(ptr, "raslog", 6);
				return(ret_success);
				break;
			case PORTFENCE:
				return(ret_pf);
				break;
			case (RASLOG+EMAIL):
				strncpy(ptr, "raslog,email", 12); 
				return(ret_success);
				break;
			case (EMAIL+PORTFENCE):
				strncpy(ptr, "email", 5); 
				return(ret_pf);
				break;

			default:
				trace (TRACE_LEVEL_ERROR, "Not a supported action");
				return(ret_failed);	
				break;
		}
	}
	int LocalInterfaceMonitorWorker::fwBackendOpInterfacemonitorThreshold()
	{
		trace (TRACE_LEVEL_INFO, " LocalInterfaceMonitorWorker::fwBackendOpInterfacemonitorThreshold - Entering");
		int j = 0, i = 0;
		thClassAreaEntry aaEntry;
		int selection = 0;
		int ret = 0, ret_case = 0;
		int ret_fail = -1;;
		bool usr_cfg = false;
		int count = 0;
		string policyName("custom");
		//unsigned int policyName = 0;
		vector<WaveManagedObject *> *pResultsType = NULL;	
		vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > tempAreaThreshAlertMapVector;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		InterfaceMonitorTypeLocalManagedObject *pInterfacemonitorTypeMo;
		InterfaceMonitorAreaLocalManagedObject *pInterfacemonitorAreaMo[MAX_INT_VAL * MAX_AREA_VAL];
		WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> areaThreshAlertMap;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceMonitorTypeLocalManagedObject::getClassName ());
		if (pResults != NULL)  {
			/* Type MO is not present. Create it now */
			if (pResults->size() == 0) {
				BitMap aboveHigh;
				BitMap aboveLow;
				BitMap belowHigh;
				BitMap belowLow;
				trace (TRACE_LEVEL_ERROR, "LocalInterfaceMonitorWorker::fwBackendOpInterfacemonitorThreshold No Type MO");
				startTransaction ();
				pInterfacemonitorTypeMo = new InterfaceMonitorTypeLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));
				prismAssert (NULL != pInterfacemonitorTypeMo,__FILE__,__LINE__);
				trace (TRACE_LEVEL_INFO, "LocalInterfaceMonitorWorker::fwBackendOpInterfacemonitorThreshold - Type MO Update Entering");
				pInterfacemonitorTypeMo->setpolicyName (policyName, usr_cfg);
				for (i = 0 ; i < MAX_INT_VAL; i++) {
					// For a given INTERFACE type, create all the area MO
					for (j = 0; j < MAX_AREA_VAL; j++) {
						aboveHigh.setAllValues(interfacetypeval[j].interfaceAlertVal.aboveHigh);
						aboveLow.setAllValues(interfacetypeval[j].interfaceAlertVal.aboveLow);
						belowHigh.setAllValues(interfacetypeval[j].interfaceAlertVal.belowHigh);
						belowLow.setAllValues(interfacetypeval[j].interfaceAlertVal.belowLow);
						pInterfacemonitorAreaMo[count] = new InterfaceMonitorAreaLocalManagedObject(dynamic_cast<ThresholdMonitorLocalObjectManager *>(getPWaveObjectManager()));  
						prismAssert (NULL != pInterfacemonitorAreaMo[count],__FILE__, __LINE__);	
						pInterfacemonitorAreaMo[count]->setInterfacetypeName(i, usr_cfg);
						pInterfacemonitorAreaMo[count]->setAreaName(j, usr_cfg);
						pInterfacemonitorAreaMo[count]->setHighThresh(interfacetypeval[j].interfaceareaval.highthresh);
						pInterfacemonitorAreaMo[count]->setLowThresh(interfacetypeval[j].interfaceareaval.lowthresh);
						pInterfacemonitorAreaMo[count]->setBuffer(interfacetypeval[j].interfaceareaval.buffer);
						pInterfacemonitorAreaMo[count]->setAboveHighThreshAction(aboveHigh);
						pInterfacemonitorAreaMo[count]->setAboveLowThreshAction(aboveLow);
						pInterfacemonitorAreaMo[count]->setBelowHighThreshAction(belowHigh);
						pInterfacemonitorAreaMo[count]->setBelowLowThreshAction(belowLow);
						pInterfacemonitorAreaMo[count]->setTimeBase(TH_TMBASE_MIN);
						pInterfacemonitorTypeMo->addAreaThreshAlertMap(pInterfacemonitorAreaMo[count]);
						count ++;
					}
				}
				status = commitTransaction();
				if (status == FRAMEWORK_SUCCESS) {
					trace (TRACE_LEVEL_INFO, "LocalInterfaceMonitorWorker::fwBackendOpInterfacemonitorThreshold  - Default commit success");
				} else {
					trace (TRACE_LEVEL_ERROR,string("LocalInterfaceMonitorWorker::fwBackendOpInterfacemonitorThreshold  - Default commit failed")+ FrameworkToolKit::localize(status));
					delete pInterfacemonitorTypeMo;
					return (ret_fail);
				}
				if (pInterfacemonitorTypeMo != NULL)
					delete pInterfacemonitorTypeMo;
				
			} 
			trace (TRACE_LEVEL_INFO, "LocalInterfaceMonitorWorker::fwBackendOpInterfacemonitorThreshold MO exists");
			//policyName = CUSTOM_POLICY;
			unsigned int areaName  = 0, interfacetypename = 0;
			i = 0;
			struct threshold thObj;
			unsigned int iter;

			pResultsType = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceMonitorTypeLocalManagedObject::getClassName ());

			if((pResultsType != NULL)&&(pResultsType->size() > 0)) {
				pInterfacemonitorTypeMo = dynamic_cast <InterfaceMonitorTypeLocalManagedObject *> ((*pResultsType)[0]);
				if ((pInterfacemonitorTypeMo->getpolicyName()).compare(policyName)
						== 0)
					tempAreaThreshAlertMapVector = pInterfacemonitorTypeMo->getAreaThresholdAlertMap();
				for ( i = 0 ; i < MAX_INT_VAL; i++) {
					interfacetypename = i;
					for (j  = 0; j < MAX_AREA_VAL; j++) {
						areaName = j;
						for (iter = 0;  iter < tempAreaThreshAlertMapVector.size(); iter ++)
						{
							if
								(((tempAreaThreshAlertMapVector[iter]->m_areaname.getUI32Value())
								 == areaName) &&
									((tempAreaThreshAlertMapVector[iter]->m_interfacetypename.getUI32Value())
									 == interfacetypename))
								break;
						}

						memset(&thObj, 0, sizeof (thObj));
						thObj.area = areaName;
						thObj.areaFound = SET;
						thObj.timeBaseFound = SET; 
						thObj.timebase = TH_TMBASE_MIN;
						thObj.highTh.val = tempAreaThreshAlertMapVector[iter]->m_highthresh;
						thObj.highTh.valFound = SET;
						thObj.lowTh.val = tempAreaThreshAlertMapVector[iter]->m_lowthresh;
						thObj.lowTh.valFound = SET;
						thObj.buffer.val = tempAreaThreshAlertMapVector[iter]->m_buffer;
						thObj.buffer.valFound = SET;
						thObj.saveFound = SET;
						thObj.save = SAVE_ONLY;

						ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_THRESHOLD);
						if (ret != 0) {
							trace (TRACE_LEVEL_INFO,string("fwBackendOpInterfacemonitorThreshold Backend updation failed with ret..") + ret);	
							FW_BKEND_ERROR;
							return (ret);
						}
						thObj.highTh.actionFound = SET;
						thObj.highTh.boundry = thAbove;
						thObj.highTh.boundryFound = SET;
						ret_case = fwSwitchCaseAlert((tempAreaThreshAlertMapVector[iter]->m_abovehighthreshaction).getElemValue (),thObj.highTh.actions);
						if (ret_case == -1) {
							FW_BKEND_ERROR;
							return (ret_case);
						}
						
						ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
						if (ret != 0) {
							FW_BKEND_ERROR;
							return (ret);
						}
						if ((ret_case == 1) && (areaName == GEPORT_IFG)) {
							bzero((char *)&aaEntry, sizeof (thClassAreaEntry));
							(void) thaClassAreaStructGet(AGENT_GEPORT, areaName, &aaEntry);
							selection = aaEntry.thCustAboveActs | ACT_PORTBLOCK;
							ret = thaActFieldSet(AGENT_GEPORT, areaName, thAbove, selection);
							if (ret != 0) {
								FW_BKEND_ERROR;
								return (ret);
							}
						}

						thObj.highTh.actionFound = SET;
						thObj.highTh.boundry = thBelow;
						thObj.highTh.boundryFound = SET;
						if (fwSwitchCaseAlert((tempAreaThreshAlertMapVector[iter]->m_belowhighthreshaction).getElemValue(), thObj.highTh.actions) == -1) {
							trace (TRACE_LEVEL_INFO,"fwBackendOpInterfacemonitorThreshold Backend updation failed in switch case");
							FW_BKEND_ERROR;
							return(ret_fail);
						} 
						ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
						if (ret != 0) {
							FW_BKEND_ERROR;
							return (ret);
						}
						thObj.lowTh.actionFound = SET;
						thObj.lowTh.boundry = thBelow;
						thObj.lowTh.boundryFound = SET;
						if (fwSwitchCaseAlert((tempAreaThreshAlertMapVector[iter]->m_belowlowthreshaction).getElemValue(), thObj.lowTh.actions) == -1) {
							trace (TRACE_LEVEL_INFO,"fwBackendOpInterfacemonitorThreshold Backend updation failed in switch case");
							FW_BKEND_ERROR;
							return(ret_fail);
						} 
						ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
						if (ret != 0) {
							FW_BKEND_ERROR;
							return (ret);
						}
						thObj.lowTh.actionFound = SET;
						thObj.lowTh.boundry = thAbove;
						thObj.lowTh.boundryFound = SET;
						if (fwSwitchCaseAlert((tempAreaThreshAlertMapVector[iter]->m_abovelowthreshaction).getElemValue(), thObj.lowTh.actions) == -1) {
							trace (TRACE_LEVEL_INFO,"fwBackendOpInterfacemonitorThreshold Backend updation failed in switch case");
							FW_BKEND_ERROR;
							return(ret_fail);
						} 
						ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
						if (ret != 0) {
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
	void LocalInterfaceMonitorWorker::InterfaceMonitorLocalConfigMessageHandler( InterfaceMonitorLocalConfigMessage *pInterfaceMonitorLocalConfigMessage)
	{
		trace (TRACE_LEVEL_INFO, "Entering InterfaceMonitorLocalConfigMessageHandler..");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (pInterfaceMonitorLocalConfigMessage == NULL)
		{
			trace (TRACE_LEVEL_INFO, "InterfaceMonitorLocalConfigMessageHandler : Empty Message");
			prismAssert (false, __FILE__, __LINE__);
		}
		string policyName;
		policyName.assign(pInterfaceMonitorLocalConfigMessage->getPolicyName());
		unsigned int interfacetypename = pInterfaceMonitorLocalConfigMessage->getInterfaceTypeName();
		unsigned int areaName = pInterfaceMonitorLocalConfigMessage->getAreaName();
		unsigned int timeBase = pInterfaceMonitorLocalConfigMessage->getTimeBase();
		int highthresh = pInterfaceMonitorLocalConfigMessage->getHighThresh();
		int lowthresh = pInterfaceMonitorLocalConfigMessage->getLowThresh();
		int bufferthresh = pInterfaceMonitorLocalConfigMessage->getBufferThresh(); 
		BitMap abovehighthreshactionBM = pInterfaceMonitorLocalConfigMessage->getAboveHighThreshAction();
		BitMap abovelowthreshactionBM = pInterfaceMonitorLocalConfigMessage->getAboveLowThreshAction();
		BitMap belowhighthreshactionBM = pInterfaceMonitorLocalConfigMessage->getBelowHighThreshAction();
		BitMap belowlowthreshactionBM = pInterfaceMonitorLocalConfigMessage->getBelowLowThreshAction();
								
		if (lowthresh > highthresh) {
			status = INVALID_THRESH_VALUE;
			pInterfaceMonitorLocalConfigMessage->setCompletionStatus(status);
			reply (pInterfaceMonitorLocalConfigMessage);
			return;
		}
		if ((bufferthresh < 0) || 
				(bufferthresh > (highthresh - lowthresh)/2)) {
			status = INVALID_BUFFER_VAL;
			pInterfaceMonitorLocalConfigMessage->setCompletionStatus(status);
			reply (pInterfaceMonitorLocalConfigMessage);
			return;
		}
		if ((abovehighthreshactionBM.getElemValue() == PORTFENCE) && (areaName != GEPORT_IFG)) {
			status = INVALID_FENCE_VALUE;
			pInterfaceMonitorLocalConfigMessage->setCompletionStatus(status);
			reply (pInterfaceMonitorLocalConfigMessage);
			return;
		}
		thClassAreaEntry aaEntry;
		int selection = 0;

		int ret = 0, ret_case = 0;
		struct threshold thObj;
		memset(&thObj, 0, sizeof (thObj));
		thObj.area = areaName;
		thObj.areaFound = SET;
		thObj.timeBaseFound = SET; 
		thObj.timebase = timeBase;
		thObj.highTh.val = highthresh;
		thObj.highTh.valFound = SET;
		thObj.lowTh.val = lowthresh;
		thObj.lowTh.valFound = SET;
		thObj.buffer.val = bufferthresh;
		thObj.buffer.valFound = SET;
		thObj.saveFound = SET;
		thObj.save = SAVE_ONLY;

		if (abovehighthreshactionBM.getElemValue()) {
			thObj.highTh.actionFound = SET;
			thObj.highTh.boundry = thAbove;
			thObj.highTh.boundryFound = SET;
			ret_case = fwSwitchCaseAlert(abovehighthreshactionBM.getElemValue (), thObj.highTh.actions);
			if (ret_case == -1) {
			 FW_MSG_HNDLR_ERROR;
			}
			
			ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
			if (ret != 0) {
				FW_MSG_HNDLR_ERROR;
			}
			if ((ret_case == 1) && (areaName == GEPORT_IFG)) {
				bzero((char *)&aaEntry, sizeof (thClassAreaEntry));
				(void) thaClassAreaStructGet(AGENT_GEPORT, areaName, &aaEntry);
				selection = aaEntry.thCustAboveActs | ACT_PORTBLOCK;
				ret = thaActFieldSet(AGENT_GEPORT, areaName, thAbove, selection);
				if (ret != 0) {
					FW_MSG_HNDLR_ERROR;
				}
			}
		}
		if (belowhighthreshactionBM.getElemValue()) {
			thObj.highTh.actionFound = SET;
			thObj.highTh.boundry = thBelow;
			thObj.highTh.boundryFound = SET;
			if(fwSwitchCaseAlert(belowhighthreshactionBM.getElemValue (), thObj.highTh.actions) == -1) {	
			 FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
			if (ret != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
		}
		if (belowlowthreshactionBM.getElemValue()) {
			thObj.lowTh.actionFound = SET;
			thObj.lowTh.boundry = thBelow;
			thObj.lowTh.boundryFound = SET;
			if(fwSwitchCaseAlert(belowlowthreshactionBM.getElemValue (), thObj.lowTh.actions) == -1) {
			 FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
			if (ret != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
		}
		if (abovelowthreshactionBM.getElemValue()) {
			thObj.lowTh.actionFound = SET;
			thObj.lowTh.boundry = thAbove;
			thObj.lowTh.boundryFound = SET;
			if(fwSwitchCaseAlert(abovelowthreshactionBM.getElemValue (), thObj.lowTh.actions) == -1) {
			 FW_MSG_HNDLR_ERROR;
			} 
			ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_ALERT);
			if (ret != 0) {
			 FW_MSG_HNDLR_ERROR;
			} 
		}
		ret = fwdSetCmdConfig(AGENT_GEPORT, areaName, &thObj, OP_THRESHOLD);
		if (ret != 0) {
			trace (TRACE_LEVEL_INFO, string("LocalInterfaceMonitorWorker: Threshold config update Failed at backend ")+ret);
			if (thInvalidBS == ret) {
				pInterfaceMonitorLocalConfigMessage->setCompletionStatus(FW_SET_BUFFER_BACKEND_ERROR);
			} else if ((thInvalidHigh == ret)||(thInvalidLow == ret)) {
				pInterfaceMonitorLocalConfigMessage->setCompletionStatus(FW_INVALID_THRESH_VALUE);
			} else {
				pInterfaceMonitorLocalConfigMessage->setCompletionStatus(FW_SET_THRESHOLD_BACKEND_ERROR);
			}
			reply (pInterfaceMonitorLocalConfigMessage);
			return;
		} 	
		InterfaceMonitorTypeLocalManagedObject *pinterfacemonitortypeMO = NULL;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceMonitorTypeLocalManagedObject::getClassName ());
		if (pResults->size() == 0)
		{
			trace (TRACE_LEVEL_ERROR, "InterfaceMonitorLocalConfigMessageHandler : No such Interface type Managed Object");
			pInterfaceMonitorLocalConfigMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
			reply (pInterfaceMonitorLocalConfigMessage);
			return;
		}
			
		pinterfacemonitortypeMO = dynamic_cast <InterfaceMonitorTypeLocalManagedObject *> ((*pResults)[0]);
		unsigned int iter;
		vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > tempAreaThreshAlertMapVector;
		if ((pinterfacemonitortypeMO->getpolicyName()).compare(policyName) == 0)
		{
			pinterfacemonitortypeMO->m_policyName.setIsUserConfigured(true);
			startTransaction ();
			updateWaveManagedObject (pinterfacemonitortypeMO);
			status = commitTransaction ();
			if (status != FRAMEWORK_SUCCESS)
			{
				trace (TRACE_LEVEL_FATAL,
						"InterfaceMonitorLocalConfigMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
			//	delete pinterfacemonitortypeMO;
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				pInterfaceMonitorLocalConfigMessage->setCompletionStatus(FW_SET_THRESHOLD_BACKEND_ERROR);
				reply (pInterfaceMonitorLocalConfigMessage);
				return;
			}
		}

		tempAreaThreshAlertMapVector = pinterfacemonitortypeMO->getAreaThresholdAlertMap();
		bool usr_cfg = true;
		for (iter = 0;  iter < tempAreaThreshAlertMapVector.size(); iter ++)
		{
			if
				(((tempAreaThreshAlertMapVector[iter]->m_areaname.getUI32Value())
				  == areaName) &&
				 ((tempAreaThreshAlertMapVector[iter]->m_interfacetypename.getUI32Value())
				  == interfacetypename))
				break;
		}

		InterfaceMonitorAreaLocalManagedObject *pInterfaceMonitorAreaMO = tempAreaThreshAlertMapVector[iter].operator->();
		pInterfaceMonitorAreaMO->setInterfacetypeName(interfacetypename, usr_cfg);
		pInterfaceMonitorAreaMO->setAreaName(areaName, usr_cfg);
	 	pInterfaceMonitorAreaMO->setHighThresh(highthresh);
	 	pInterfaceMonitorAreaMO->setLowThresh(lowthresh);
	 	pInterfaceMonitorAreaMO->setBuffer(bufferthresh);
		if (abovehighthreshactionBM.getElemValue() )
			pInterfaceMonitorAreaMO->setAboveHighThreshAction(abovehighthreshactionBM);
		if (abovelowthreshactionBM.getElemValue() )
			pInterfaceMonitorAreaMO->setAboveLowThreshAction(abovelowthreshactionBM);
		if (belowhighthreshactionBM.getElemValue() )
			pInterfaceMonitorAreaMO->setBelowHighThreshAction(belowhighthreshactionBM);
		if (belowlowthreshactionBM.getElemValue() )
			pInterfaceMonitorAreaMO->setBelowLowThreshAction(belowlowthreshactionBM);
		pInterfaceMonitorAreaMO->setTimeBase(timeBase);

		startTransaction (); 
		updateWaveManagedObject (pInterfaceMonitorAreaMO);
		status = commitTransaction();
		if (status != FRAMEWORK_SUCCESS)
		{
			trace (TRACE_LEVEL_FATAL, "InterfaceMonitorLocalConfigMessageHandler : Commiting failed.Status:" + FrameworkToolKit::localize (status));
		} else 
		{
			status = WAVE_MESSAGE_SUCCESS;
		}
		//delete pinterfacemonitortypeMO;
	 	//delete pInterfaceMonitorAreaMO;
		tempAreaThreshAlertMapVector.clear();
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		pInterfaceMonitorLocalConfigMessage->setCompletionStatus(status);
		reply (pInterfaceMonitorLocalConfigMessage);
	}
}
