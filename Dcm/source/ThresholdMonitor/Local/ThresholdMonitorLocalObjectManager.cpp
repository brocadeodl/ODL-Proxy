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

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/LocalCpuWorker.h"
#include "ThresholdMonitor/Local/LocalMemoryWorker.h"
#include "ThresholdMonitor/Local/LocalSfpMonitorWorker.h"
#include "ThresholdMonitor/Local/LocalSfpConfigWorker.h"
#include "ThresholdMonitor/Local/LocalSecWorker.h"
#include "ThresholdMonitor/Local/LocalSecConfigWorker.h"
#include "ThresholdMonitor/Local/LocalInterfaceMonitorWorker.h"
#include "ThresholdMonitor/Local/LocalInterfaceConfigWorker.h"
#include "ThresholdMonitor/Local/LocalInterfaceShowWorker.h"
#include "ThresholdMonitor/Local/LocalSfpShowWorker.h"
#include "ThresholdMonitor/Local/LocalSecShowWorker.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"
#include "DcmShell/DcmShell.h"
#include <thresh/portCmd.h>

#include "ThresholdMonitor/Local/CpuLocalManagedObject.h"
#include "ThresholdMonitor/Local/MemoryLocalManagedObject.h"
#include "ThresholdMonitor/Local/SfpMonitorTypeLocalManagedObject.h"
#include "ThresholdMonitor/Local/SfpConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/SecMonitorPolicyLocalManagedObject.h"
#include "ThresholdMonitor/Local/SecConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/InterfaceMonitorTypeLocalManagedObject.h"
#include "ThresholdMonitor/Local/InterfaceConfigLocalManagedObject.h"

namespace DcmNs
{

	ThresholdMonitorLocalObjectManager::ThresholdMonitorLocalObjectManager ()
		: WaveLocalObjectManager  (getClassName ())
	{
	    associateWithVirtualWaveObjectManager   (ThresholdMonitorGlobalObjectManager::getInstance ());
		m_pLocalCpuWorker = new LocalCpuWorker (this);
		prismAssert (NULL != m_pLocalCpuWorker, __FILE__, __LINE__);
		m_pLocalMemoryWorker = new LocalMemoryWorker (this);
		prismAssert (NULL != m_pLocalMemoryWorker, __FILE__, __LINE__);
		m_pLocalSfpMonitorWorker = new LocalSfpMonitorWorker (this);
		prismAssert (NULL != m_pLocalSfpMonitorWorker, __FILE__, __LINE__);
		m_pLocalSfpConfigWorker = new LocalSfpConfigWorker (this);
		prismAssert (NULL != m_pLocalSfpConfigWorker, __FILE__, __LINE__);
		m_pLocalSecWorker = new LocalSecWorker (this);
		prismAssert (NULL != m_pLocalSecWorker, __FILE__, __LINE__);
		m_pLocalSecConfigWorker = new LocalSecConfigWorker (this);
		prismAssert (NULL != m_pLocalSecConfigWorker, __FILE__, __LINE__);
		m_pLocalInterfaceMonitorWorker = new LocalInterfaceMonitorWorker (this);
		prismAssert (NULL != m_pLocalInterfaceMonitorWorker, __FILE__, __LINE__);
		m_pLocalInterfaceConfigWorker = new LocalInterfaceConfigWorker (this);
		prismAssert (NULL != m_pLocalInterfaceConfigWorker, __FILE__, __LINE__);
		m_pLocalInterfaceShowWorker = new LocalInterfaceShowWorker (this);
		prismAssert (NULL != m_pLocalInterfaceShowWorker, __FILE__, __LINE__);
		m_pLocalSfpShowWorker = new LocalSfpShowWorker (this);
		prismAssert (NULL != m_pLocalSfpShowWorker, __FILE__, __LINE__);
		m_pLocalSecShowWorker = new LocalSecShowWorker (this);
		prismAssert (NULL != m_pLocalSecShowWorker, __FILE__, __LINE__);
        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
	}

	ThresholdMonitorLocalObjectManager::~ThresholdMonitorLocalObjectManager ()
	{
		delete m_pLocalCpuWorker;
		delete m_pLocalMemoryWorker;
		delete m_pLocalSfpMonitorWorker;
		delete m_pLocalSfpConfigWorker;
		delete m_pLocalSecWorker;
		delete m_pLocalSecConfigWorker;
		delete m_pLocalInterfaceMonitorWorker;
		delete m_pLocalInterfaceConfigWorker;
		delete m_pLocalInterfaceShowWorker;
		delete m_pLocalSfpShowWorker;
		delete m_pLocalSecShowWorker;

	}

	ThresholdMonitorLocalObjectManager  *ThresholdMonitorLocalObjectManager::getInstance()
	{
		static ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager = new ThresholdMonitorLocalObjectManager ();

		WaveNs::prismAssert (NULL != pThresholdMonitorLocalObjectManager, __FILE__, __LINE__);

		return (pThresholdMonitorLocalObjectManager);
	}

	string  ThresholdMonitorLocalObjectManager::getClassName()
	{
		return ("ThresholdMonitor");
	}

	PrismServiceId  ThresholdMonitorLocalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *ThresholdMonitorLocalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

		switch (operationCode)
		{

			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	WaveManagedObject  *ThresholdMonitorLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
		return NULL;
	}

	void ThresholdMonitorLocalObjectManager::externalStateSynchronization (
			WaveAsynchronousContextForExternalStateSynchronization
			*pWaveAsynchronousContextForExternalStateSynchronization) {

		if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 9)
				&& (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {
			trace (TRACE_LEVEL_INFO, "ThresholdMonitorLocalObjectManager::externalStateSynchronization entering..");
			m_pLocalCpuWorker->backendSync ();
			m_pLocalMemoryWorker->backendSync ();
			m_pLocalSfpMonitorWorker->fwBackendOpSfpmonitorThreshold();
			m_pLocalSfpConfigWorker->fwBackendApplyPause();
			m_pLocalSecWorker->backendSync();
			m_pLocalSecConfigWorker->fwBackendApply();
			m_pLocalInterfaceMonitorWorker->fwBackendOpInterfacemonitorThreshold();
			m_pLocalInterfaceConfigWorker->fwBackendApplyPause();

		}
		pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForExternalStateSynchronization->callback ();
	}

	void ThresholdMonitorLocalObjectManager::configReplayEnd (
			WaveAsynchronousContextForConfigReplayEnd
			*pWaveAsynchronousContextForConfigReplayEnd)
	{
		trace (TRACE_LEVEL_INFO, "ThresholdMonitorLocalObjectManager::Configreplayend entering..");
		m_pLocalCpuWorker->backendSync ();
		m_pLocalMemoryWorker->backendSync ();
		m_pLocalSfpMonitorWorker->fwBackendOpSfpmonitorThreshold();
		m_pLocalSfpConfigWorker->fwBackendApplyPause();
		m_pLocalSecWorker->backendSync();
		m_pLocalSecConfigWorker->fwBackendApply();
		m_pLocalInterfaceMonitorWorker->fwBackendOpInterfacemonitorThreshold();
		m_pLocalInterfaceConfigWorker->fwBackendApplyPause();


		pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForConfigReplayEnd->callback ();
	}
	void ThresholdMonitorLocalObjectManager::boot (
			WaveAsynchronousContextForBootPhases 
			*pWaveAsynchronousContextForBootPhases)
	{
		trace (TRACE_LEVEL_INFO, "ThresholdMonitorLocalObjectManager::boot entering..");
		WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
        if ((WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) ||
			(WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT == bootReason)) {
			trace (TRACE_LEVEL_INFO, "ThresholdMonitorLocalObjectManager::boot entering secondary node config..");

			initManagedObjects();

			m_pLocalCpuWorker->backendSync ();
			m_pLocalMemoryWorker->backendSync ();
			m_pLocalSfpMonitorWorker->fwBackendOpSfpmonitorThreshold();
			m_pLocalSfpConfigWorker->fwBackendApplyPause();
			m_pLocalSecWorker->backendSync();
			m_pLocalSecConfigWorker->fwBackendApply();
			m_pLocalInterfaceMonitorWorker->fwBackendOpInterfacemonitorThreshold();
			m_pLocalInterfaceConfigWorker->fwBackendApplyPause();
		}

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

	void ThresholdMonitorLocalObjectManager::initManagedObjects()
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pCpuMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, CpuLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pMemoryMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MemoryLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pSfpmonitorTypeMo = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpMonitorTypeLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pSfpConfigLocalManagedObject = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpConfigLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pSecPolicyMo = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SecMonitorPolicyLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pSecConfigLocalManagedObject = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SecConfigLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pInterfacemonitorTypeMo = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceMonitorTypeLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pInterfaceConfigLocalManagedObject =  querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceConfigLocalManagedObject::getClassName ());

		startTransaction();

		//CpuLocalManagedObject
		CpuLocalManagedObject *pCpuMO_new = NULL;
		if((pCpuMO != NULL) && (pCpuMO->size() == 0))
		{
            int CPU_poll    = CPU_POLL_DEFAULT;
            int CPU_retry   = CPU_RETRY_DEFAULT;
            int CPU_limit   = CPU_LIMIT_DEFAULT;
            actionType CPU_actions = FW_NONE;
			pCpuMO_new = new CpuLocalManagedObject(this,CPU_poll, CPU_retry, CPU_limit, CPU_actions);

            prismAssert (NULL != pCpuMO_new, __FILE__, __LINE__);
		}

		//MemoryLocalManagedObject
		MemoryLocalManagedObject *pMemoryMO_new = NULL;
		if((pMemoryMO != NULL) && (pMemoryMO->size() == 0))
		{
			int poll        = MEMORY_POLL_DEFAULT;
            int retry       = MEMORY_RETRY_DEFAULT;
            int limit       = MEMORY_LIMIT_DEFAULT;
            int lowLimit    = MEMORY_LOW_LIMIT_DEFAULT;
            int highLimit   = MEMORY_HIGH_LIMIT_DEFAULT;
            actionType actions     = FW_NONE;
			pMemoryMO_new = new MemoryLocalManagedObject(this,poll, retry, limit, highLimit, lowLimit, actions);

            prismAssert (NULL != pMemoryMO_new, __FILE__, __LINE__);
		}

		//SfpMonitorTypeLocalManagedObject
		SfpMonitorTypeLocalManagedObject *pSfpmonitorTypeMo_new = NULL;
		if((pSfpmonitorTypeMo != NULL) && (pSfpmonitorTypeMo->size() == 0))
		{
                BitMap aboveHigh;
                BitMap aboveLow;
                BitMap belowHigh;
                BitMap belowLow;
				pSfpmonitorTypeMo_new = new SfpMonitorTypeLocalManagedObject(this);
				prismAssert (NULL != pSfpmonitorTypeMo_new,__FILE__, __LINE__);
				bool usr_cfg = false;
				string policyName("custom");
				pSfpmonitorTypeMo_new->setpolicyName (policyName, usr_cfg);
				SfpMonitorAreaLocalManagedObject *pSfpmonitorAreaMo[MAX_OUI_VAL * MAX_AREA_VAL];
				int count = 0;
				for (int i = 0 ; i < MAX_OUI_VAL; i++) {
					aboveHigh.setAllValues(sfptypeval[i].sfpAlertVal.aboveHigh);
					aboveLow.setAllValues(sfptypeval[i].sfpAlertVal.aboveLow);
					belowHigh.setAllValues(sfptypeval[i].sfpAlertVal.belowHigh);
					belowLow.setAllValues(sfptypeval[i].sfpAlertVal.belowLow);
					for (int j = 0; j <  MAX_AREA_VAL; j++) {
						pSfpmonitorAreaMo[count] = new SfpMonitorAreaLocalManagedObject(this);
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
						pSfpmonitorTypeMo_new->addAreaThreshAlertMap(pSfpmonitorAreaMo[count]);
						count++;
					}
				}
		}

		//SfpConfigLocalManagedObject
		SfpConfigLocalManagedObject *pSfpConfigLocalManagedObject_new = NULL;
		if((pSfpConfigLocalManagedObject != NULL) && (pSfpConfigLocalManagedObject->size() == 0))
		{
			string applyVal("default");
			bool pause = false;
			pSfpConfigLocalManagedObject_new = new SfpConfigLocalManagedObject(this);
			pSfpConfigLocalManagedObject_new->setApplyValue(applyVal, false);
			pSfpConfigLocalManagedObject_new->setPause(pause);
		}

		//SecMonitorPolicyLocalManagedObject
		SecMonitorPolicyLocalManagedObject *pSecPolicyMo_new = NULL;
		if((pSecPolicyMo != NULL) && (pSecPolicyMo->size() == 0))
		{
	        BitMap abovehigh[MAX_SEC_AREA];
    	    BitMap abovelow[MAX_SEC_AREA];
        	BitMap belowhigh[MAX_SEC_AREA];
	        BitMap belowlow[MAX_SEC_AREA];
			struct threshold thObj[MAX_SEC_AREA] = {{'\0'}};
			string policyName("custom");
			bool usr_cfg = false;
			pSecPolicyMo_new = new SecMonitorPolicyLocalManagedObject(this);
			prismAssert (NULL != pSecPolicyMo_new, __FILE__, __LINE__);
			pSecPolicyMo_new->setpolicyName (policyName, usr_cfg);
			SecLocalManagedObject *pSecMo[MAX_SEC_AREA];
			int count = 0;
			for (unsigned int i = 0; i < MAX_SEC_AREA; i++) {
				pSecMo[count] = new SecLocalManagedObject(this);
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
				pSecPolicyMo_new->addAreaThreshAlertMap(pSecMo[count]);
				count ++;
			}
		}

		//SecConfigLocalManagedObject
		SecConfigLocalManagedObject *pSecConfigLocalManagedObject_new = NULL;
		if((pSecConfigLocalManagedObject != NULL) && (pSecConfigLocalManagedObject->size() == 0))
		{
			string applyvalue("default");
			bool pause = false;
			pSecConfigLocalManagedObject_new = new SecConfigLocalManagedObject(this);
			pSecConfigLocalManagedObject_new->setApplyValue(applyvalue, false);
			pSecConfigLocalManagedObject_new->setPause(pause);
		}

		//InterfaceMonitorTypeLocalManagedObject
		InterfaceMonitorTypeLocalManagedObject *pInterfacemonitorTypeMo_new = NULL;
		if((pInterfacemonitorTypeMo != NULL) && (pInterfacemonitorTypeMo->size() == 0))
		{
			BitMap aboveHigh;
            BitMap aboveLow;
            BitMap belowHigh;
            BitMap belowLow;
			string policyName("custom");
			bool usr_cfg = false;
			pInterfacemonitorTypeMo_new = new InterfaceMonitorTypeLocalManagedObject(this);
			prismAssert (NULL != pInterfacemonitorTypeMo_new,__FILE__,__LINE__);
			pInterfacemonitorTypeMo_new->setpolicyName (policyName, usr_cfg);
			InterfaceMonitorAreaLocalManagedObject *pInterfacemonitorAreaMo[MAX_INT_VAL * MAX_AREA_VAL];
			int count = 0;
			for (int i = 0 ; i < MAX_INT_VAL; i++) {
				for (int j = 0; j < MAX_AREA_VAL; j++) {
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
                    pInterfacemonitorTypeMo_new->addAreaThreshAlertMap(pInterfacemonitorAreaMo[count]);
                    count ++;
				}
			}
		}

		//InterfaceConfigLocalManagedObject
		InterfaceConfigLocalManagedObject *pInterfaceConfigLocalManagedObject_new = NULL;
		if((pInterfaceConfigLocalManagedObject != NULL) && (pInterfaceConfigLocalManagedObject->size() == 0))
		{
			string applyVal("default");
			bool pause = false;
			pInterfaceConfigLocalManagedObject_new = new InterfaceConfigLocalManagedObject(this);
			pInterfaceConfigLocalManagedObject_new->setApplyValue(applyVal, false);
			pInterfaceConfigLocalManagedObject_new->setPause(pause);
		}

		status = commitTransaction();
		if (status == FRAMEWORK_SUCCESS)
			trace (TRACE_LEVEL_INFO, "ThresholdMonitorLocalObjectManager::initManagedObjects - Default commit success");
		else
			trace (TRACE_LEVEL_INFO, "ThresholdMonitorLocalObjectManager::initManagedObjects - Default commit failed");


		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCpuMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMemoryMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSfpmonitorTypeMo);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSfpConfigLocalManagedObject);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSecPolicyMo);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSecConfigLocalManagedObject);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pInterfacemonitorTypeMo);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pInterfaceConfigLocalManagedObject);

        delete pCpuMO_new;
        delete pMemoryMO_new;
        delete pSfpmonitorTypeMo_new;
        delete pSfpConfigLocalManagedObject_new;
        delete pSecPolicyMo_new;
        delete pSecConfigLocalManagedObject_new;
        delete pInterfacemonitorTypeMo_new;
        delete pInterfaceConfigLocalManagedObject_new;

		return;
	}


}
