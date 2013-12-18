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

#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"

#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/LocalFanWorker.h"
#include "SystemMonitor/Local/LocalPowerWorker.h"
#include "SystemMonitor/Local/LocalTempWorker.h"
#include "SystemMonitor/Local/LocalWwnCardWorker.h"
#include "SystemMonitor/Local/LocalSfpWorker.h"
#include "SystemMonitor/Local/LocalCompactFlashWorker.h"
#include "SystemMonitor/Local/LocalFaultyPortsWorker.h"
#include "SystemMonitor/Local/LocalMissingSfpWorker.h"
#include "SystemMonitor/Local/LocalMMWorker.h"
#include "SystemMonitor/Local/LocalLineCardWorker.h"
#include "SystemMonitor/Local/LocalSfmWorker.h"
#include "SystemMonitor/Local/LocalFruMailWorker.h"
#include "SystemMonitor/Local/LocalSecMailWorker.h"
#include "SystemMonitor/Local/LocalSfpMailWorker.h"
#include "SystemMonitor/Local/LocalIntMailWorker.h"
#include "SystemMonitor/Local/LocalRelayIpWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"

#include "SystemMonitor/Local/FanThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/PowerThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/TempThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/WwnCardThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/CompactFlashThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/MissingSfpThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/FaultyPortsThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/MMThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/LineCardThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SfmThresholdLocalManagedObject.h"

#include "SystemMonitor/Local/FanAlertLocalManagedObject.h"
#include "SystemMonitor/Local/PowerAlertLocalManagedObject.h"
#include "SystemMonitor/Local/WwnCardAlertLocalManagedObject.h"
#include "SystemMonitor/Local/SfpAlertLocalManagedObject.h"
#include "SystemMonitor/Local/LineCardAlertLocalManagedObject.h"

#include "SystemMonitor/Local/ShowMonitorWorker.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

    SystemMonitorLocalObjectManager::SystemMonitorLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
	    associateWithVirtualWaveObjectManager   (SystemMonitorGlobalObjectManager::getInstance ());
        m_pShowMonitorWorker = new ShowMonitorWorker (this);
        prismAssert (NULL != m_pShowMonitorWorker, __FILE__, __LINE__);
        m_pLocalFanWorker = new LocalFanWorker (this);
        prismAssert (NULL != m_pLocalFanWorker, __FILE__, __LINE__);
        m_pLocalPowerWorker = new LocalPowerWorker (this);
        prismAssert (NULL != m_pLocalPowerWorker, __FILE__, __LINE__);
        m_pLocalTempWorker = new LocalTempWorker (this);
        prismAssert (NULL != m_pLocalTempWorker, __FILE__, __LINE__);
        m_pLocalWwnCardWorker = new LocalWwnCardWorker (this);
        prismAssert (NULL != m_pLocalWwnCardWorker, __FILE__, __LINE__);
        m_pLocalSfpWorker = new LocalSfpWorker (this);
        prismAssert (NULL != m_pLocalSfpWorker, __FILE__, __LINE__);
        m_pLocalCompactFlashWorker = new LocalCompactFlashWorker (this);
        prismAssert (NULL != m_pLocalCompactFlashWorker, __FILE__, __LINE__);
        m_pLocalFaultyPortsWorker = new LocalFaultyPortsWorker (this);
        prismAssert (NULL != m_pLocalFaultyPortsWorker, __FILE__, __LINE__);
        m_pLocalMissingSfpWorker = new LocalMissingSfpWorker (this);
        prismAssert (NULL != m_pLocalMissingSfpWorker, __FILE__, __LINE__);
        m_pLocalMMWorker = new LocalMMWorker (this);
        prismAssert (NULL != m_pLocalMMWorker, __FILE__, __LINE__);
        m_pLocalLineCardWorker = new LocalLineCardWorker (this);
        prismAssert (NULL != m_pLocalLineCardWorker, __FILE__, __LINE__);     
        m_pLocalSfmWorker = new LocalSfmWorker (this);
        prismAssert (NULL != m_pLocalSfmWorker, __FILE__, __LINE__);     
		m_pLocalFruMailWorker = new LocalFruMailWorker (this);
        prismAssert (NULL != m_pLocalFruMailWorker, __FILE__, __LINE__);
		m_pLocalSecMailWorker = new LocalSecMailWorker (this);
        prismAssert (NULL != m_pLocalSecMailWorker, __FILE__, __LINE__);
		m_pLocalSfpMailWorker = new LocalSfpMailWorker (this);
        prismAssert (NULL != m_pLocalSfpMailWorker, __FILE__, __LINE__);
		m_pLocalIntMailWorker = new LocalIntMailWorker (this);
        prismAssert (NULL != m_pLocalIntMailWorker, __FILE__, __LINE__);
		m_pLocalRelayIpWorker = new LocalRelayIpWorker (this);
        prismAssert (NULL != m_pLocalRelayIpWorker, __FILE__, __LINE__);

        m_flag = 0;
        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
    }

    SystemMonitorLocalObjectManager::~SystemMonitorLocalObjectManager ()
    {
        delete m_pShowMonitorWorker;
        delete m_pLocalFanWorker;
        delete m_pLocalPowerWorker;
        delete m_pLocalTempWorker;
        delete m_pLocalWwnCardWorker;
        delete m_pLocalSfpWorker;
        delete m_pLocalCompactFlashWorker;
        delete m_pLocalFaultyPortsWorker;
        delete m_pLocalMissingSfpWorker;
        delete m_pLocalMMWorker;
        delete m_pLocalLineCardWorker;
        delete m_pLocalSfmWorker;
        delete m_pLocalFruMailWorker;
		delete m_pLocalSecMailWorker;
        delete m_pLocalSfpMailWorker;
        delete m_pLocalIntMailWorker;
        delete m_pLocalRelayIpWorker;
    }

    SystemMonitorLocalObjectManager  *SystemMonitorLocalObjectManager::getInstance()
    {
        static SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager = new SystemMonitorLocalObjectManager ();

        WaveNs::prismAssert (NULL != pSystemMonitorLocalObjectManager, __FILE__, __LINE__);

        return (pSystemMonitorLocalObjectManager);
    }
    void SystemMonitorLocalObjectManager::setFlag (bool value)
    {
        m_flag = value;
    }
    
    bool SystemMonitorLocalObjectManager::getFlag ()
    {
        return (m_flag);
    }


	int SystemMonitorLocalObjectManager::backendRelaySync () {
		m_pLocalRelayIpWorker->fwBackendRelay();
		return (0);	
	}
	
	int SystemMonitorLocalObjectManager::backendSync () {
		int ret = fw_fru_cfg_get_all (&m_defFruCfg);
		int isChanged = 1;
		
		fw_fruObj_t fruCfg;
		memset (&fruCfg, 0, sizeof (fruCfg));
		int ret1 = fw_fru_cfg_get_all (&fruCfg);
		if ((ret != 0) || (ret1 != 0)) {
			sleep (1);
			ret = fw_fru_cfg_get_all (&m_defFruCfg);
			ret1 = fw_fru_cfg_get_all (&fruCfg);
			if ((ret != 0) || (ret1 != 0)) {
				trace (TRACE_LEVEL_INFO, string ("SystemMonitorLocalObjectManager::backendSync fw_fru_cfg_get_all Failed") +ret1);
				trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalObjectManager::backendSync fw_fru_cfg_get_all Failed")+isChanged);
				return (ret);
			}
		}
		
		int frv = m_pLocalFanWorker->fwBackendOpFanAlert (&fruCfg);
		int prv = m_pLocalPowerWorker->fwBackendOpPowerAlert (&fruCfg);
		int wrv = m_pLocalWwnCardWorker->fwBackendOpWwnCardAlert (&fruCfg);
		int srv = m_pLocalSfpWorker->fwBackendOpSfpAlert (&fruCfg);
		int lrv = m_pLocalLineCardWorker->fwBackendOpLineCardAlert (&fruCfg);
		
		if ((frv == 0) && (prv == 0) && (wrv == 0) && (srv == 0) && (lrv == 0)) {
			int setrv = fw_fru_cfg_set_all (fruCfg);
			if (setrv != 0)
				trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalObjectManager::backendSync Set failed")+setrv);
		}
		else { 
			trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalObjectManager::backendSync BackendOp Get failed"));
			return (-1);
		}

		int dRet = switchStatusPolicyGetData(&m_defpList);
		if (dRet != 0) {
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::backendSync switchStatusPolicyGetData Failed");
			return (dRet);
		}
		swPolicyList_t pList;
		memset (&pList, 0, sizeof (pList));
		int gRet  = switchStatusPolicyGetData(&pList);
		if (gRet != 0) {
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::backendSync switchStatusPolicyGetData Failed");
			return (gRet);
		}
		int ftrv = m_pLocalFanWorker->fwBackendOpFanThreshold (&pList);
		int ptrv = m_pLocalPowerWorker->fwBackendOpPowerThreshold (&pList);
		int ttrv = m_pLocalTempWorker->fwBackendOpTempThreshold (&pList);
		int wtrv = m_pLocalWwnCardWorker->fwBackendOpWwnCardThreshold (&pList);
		int ctrv = m_pLocalCompactFlashWorker->fwBackendOpCompactFlashThreshold (&pList);
		int mstrv = m_pLocalMissingSfpWorker->fwBackendOpMissingSfpThreshold (&pList);
		int fpstrv = m_pLocalFaultyPortsWorker->fwBackendOpFaultyPortsThreshold (&pList);
		int mmstrv = m_pLocalMMWorker->fwBackendOpMMThreshold (&pList);
		int lcstrv = m_pLocalLineCardWorker->fwBackendOpLineCardThreshold (&pList);
		int cbstrv = m_pLocalSfmWorker->fwBackendOpSfmThreshold (&pList);
		if ((ftrv == 0) && (ptrv == 0) && (ttrv == 0) && (wtrv == 0) && (ctrv == 0) 
				&& (mstrv == 0) && (fpstrv == 0) && (mmstrv == 0) && (lcstrv == 0) && (cbstrv == 0)) {
			int setthrv = switchStatusPolicySetData(&pList, &isChanged);
			if (setthrv != 0) {
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::backendSync switchStatusPolicyGetData Failed");
				return (setthrv);
			}
		} else {
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::backendSync switchStatusPolicyGetData Failed");
		}

		int frumail = m_pLocalFruMailWorker->fwBackendOpFruMail ();
		if (0 != frumail) { 
			trace (TRACE_LEVEL_INFO, string("SystemMonitorLocalObjectManager::backendSync BackendOp FruMail failed"));
			return (frumail);
		}

		setFlag (true);
		return (0);
	}
	void SystemMonitorLocalObjectManager::listenForEvents (
			WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_STARTED_EVENT, 
				reinterpret_cast<PrismEventHandler> (&SystemMonitorLocalObjectManager::configReplayEndEventHandler));
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();
	}
	
 	void SystemMonitorLocalObjectManager::configReplayEndEventHandler (
			const ConfigReplayStartedEvent* &pEvent)
	{
		int fwdUp = getFlag();
		int ret = -1;
		if (fwdUp == 0) {
			ret = backendSync ();
			if (ret != 0)
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::configReplayEndEventHandler: backendSync Failed");
		}
		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

	void SystemMonitorLocalObjectManager::externalStateSynchronization (
            WaveAsynchronousContextForExternalStateSynchronization
            *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 9)
                && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::externalStateSynchronization Entering..");
			int ret = backendSync ();
			if (ret != 0)
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::ESS backendSync failed");
			ret = backendRelaySync ();
			if (ret != 0)
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::ESS backendReplaySync failed");

    	}
		pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	    pWaveAsynchronousContextForExternalStateSynchronization->callback ();
	}
	void SystemMonitorLocalObjectManager::boot (
			WaveAsynchronousContextForBootPhases 
			*pWaveAsynchronousContextForBootPhases)
	{
		trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::boot entering..");
		WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
        if ((WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) ||
			(WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT == bootReason)) {
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::boot entering secondary node config..");

			initManagedObjects ();

			int ret = backendSync ();
			if (ret != 0)
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::boot backendSync failed");
			ret = backendRelaySync();
			if (ret != 0) {
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::boot backendRelaySync failed");
			}
		}
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();

	}

	void SystemMonitorLocalObjectManager::initManagedObjects ()
	{
		trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects entering..");

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		int downTh = 2, marginalTh = 1;
		UI32   actionVal = 1, stateVal = 1;
		UI32   dcmactionVal = 1, dcmstateVal = 1;
		BitMap actionbitMap, statebitMap;
		unsigned int i = 0;
		int j = 0;
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pFanThreshMO   = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FanThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pPowerThreshMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, PowerThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pTempThreshMO  = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, TempThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pWwnCardThreshMO  = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, WwnCardThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pCompactFlashThreshMO  = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, CompactFlashThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pMissingSfpThreshMO  = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MissingSfpThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pFaultyPortsThreshMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FaultyPortsThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pMMThreshMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, MMThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pLineCardThreshMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, LineCardThresholdLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pSfmThreshMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfmThresholdLocalManagedObject::getClassName ());

		vector<WaveManagedObject *> *pFanAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, FanAlertLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pPowerAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, PowerAlertLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pWwnCardAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, WwnCardAlertLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pSfpAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, SfpAlertLocalManagedObject::getClassName ());
		vector<WaveManagedObject *> *pLineCardAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, LineCardAlertLocalManagedObject::getClassName ());

		startTransaction ();

		FanThresholdLocalManagedObject *pFanThreshNewMO = NULL;
		PowerThresholdLocalManagedObject *pPowerThreshNewMO = NULL;
		TempThresholdLocalManagedObject *pTempThreshNewMO = NULL;
		WwnCardThresholdLocalManagedObject *pWwnCardThreshNewMO = NULL;
		CompactFlashThresholdLocalManagedObject *pCompactFlashThreshNewMO = NULL;
		MissingSfpThresholdLocalManagedObject *pMissingSfpThreshNewMO = NULL;
		FaultyPortsThresholdLocalManagedObject *pFaultyPortsThreshNewMO = NULL;
		MMThresholdLocalManagedObject  *pMMThreshNewMO = NULL;
		LineCardThresholdLocalManagedObject *pLineCardThreshNewMO = NULL;
		SfmThresholdLocalManagedObject *pSfmThreshNewMO = NULL;

		FanAlertLocalManagedObject *pFanAlertNewMO  = NULL;
		PowerAlertLocalManagedObject *pPowerAlertNewMO = NULL;
		WwnCardAlertLocalManagedObject *pWwnCardAlertNewMO = NULL;
		SfpAlertLocalManagedObject *pSfpAlertNewMO = NULL;
		LineCardAlertLocalManagedObject *pLineCardAlertNewMO = NULL;


		SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
		for (i = 0; i < lOM->m_defpList.num; i++) {
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_FANS) {
				if ((pFanThreshMO != NULL)&&(pFanThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pFanThreshNewMO = new FanThresholdLocalManagedObject (this, downTh, marginalTh);
                    prismAssert (NULL != pFanThreshNewMO, __FILE__, __LINE__);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding FanThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_POWER_SUPPLIES) {
				if ((pPowerThreshMO != NULL)&&(pPowerThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pPowerThreshNewMO = new PowerThresholdLocalManagedObject (this, downTh, marginalTh);
                    prismAssert (NULL != pPowerThreshNewMO, __FILE__, __LINE__);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding PowerThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_TEMPERATURES) {
				if ((pTempThreshMO != NULL)&&(pTempThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pTempThreshNewMO = new TempThresholdLocalManagedObject (this, downTh, marginalTh);
                    prismAssert (NULL != pTempThreshNewMO, __FILE__, __LINE__);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding TempThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_WWN) {
				if ((pWwnCardThreshMO != NULL)&&(pWwnCardThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pWwnCardThreshNewMO = new WwnCardThresholdLocalManagedObject (this, downTh, marginalTh);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding WwnCardThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_FLASH) {
				if ((pCompactFlashThreshMO != NULL)&&(pCompactFlashThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pCompactFlashThreshNewMO = new CompactFlashThresholdLocalManagedObject (this, downTh, marginalTh);
                    prismAssert (NULL != pCompactFlashThreshNewMO, __FILE__, __LINE__);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding CompactFlashThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_MISSING_GBICS) {
				if ((pMissingSfpThreshMO != NULL)&&(pMissingSfpThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pMissingSfpThreshNewMO = new MissingSfpThresholdLocalManagedObject (this, downTh, marginalTh);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding MissingSfpThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_FAULTY_PORTS) {
				if ((pFaultyPortsThreshMO != NULL)&&(pFaultyPortsThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pFaultyPortsThreshNewMO = new FaultyPortsThresholdLocalManagedObject (this, downTh, marginalTh);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding FaultyPortsThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_CP) {
				if ((pMMThreshMO != NULL)&&(pMMThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pMMThreshNewMO = new MMThresholdLocalManagedObject (this, downTh, marginalTh);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding MMThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_BLADE) {
				if ((pLineCardThreshMO != NULL)&&(pLineCardThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pLineCardThreshNewMO = new LineCardThresholdLocalManagedObject (this, downTh, marginalTh);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding LineCardThresholdLocalManagedObject");
				}
			}
			if (lOM->m_defpList.defStatPolicy[i].idx == SWSTATUS_CORE_BLADE) {
				if ((pSfmThreshMO != NULL)&&(pSfmThreshMO->size() == 0)) {
					downTh     = lOM->m_defpList.defStatPolicy[i].down.down;
					marginalTh = lOM->m_defpList.defStatPolicy[i].marginal.marginal;
					pSfmThreshNewMO = new SfmThresholdLocalManagedObject (this, downTh, marginalTh);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding SfmThresholdLocalManagedObject");
				}
			}
		}

		for (j = 0; j < lOM->m_defFruCfg.fruNum; j++) {
			if (lOM->m_defFruCfg.defFruObj[j].fru_type == FW_FRU_FAN) {
				if ((pFanAlertMO != NULL)&&(pFanAlertMO->size() == 0)) {
					actionVal = lOM->m_defFruCfg.defFruObj[j].alarm_action;
					stateVal  = lOM->m_defFruCfg.defFruObj[j].alarm_state;
					dcmstateVal = m_pLocalFanWorker->fwtoDcmState (stateVal);
					dcmactionVal = m_pLocalFanWorker->fwtoDcmAction (actionVal);
					statebitMap.setAllValues (dcmstateVal);
					actionbitMap.setAllValues (dcmactionVal);
					pFanAlertNewMO = new FanAlertLocalManagedObject (this, statebitMap, actionbitMap);
                    prismAssert (NULL != pFanAlertNewMO, __FILE__, __LINE__);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding FanAlertLocalManagedObject");
				}
			}
			if (lOM->m_defFruCfg.defFruObj[j].fru_type == FW_FRU_PS) {
				if ((pPowerAlertMO != NULL)&&(pPowerAlertMO->size() == 0)) {
					actionVal = lOM->m_defFruCfg.defFruObj[j].alarm_action;
                    stateVal  = lOM->m_defFruCfg.defFruObj[j].alarm_state;
                    dcmstateVal = m_pLocalPowerWorker->fwtoDcmState (stateVal);
                    dcmactionVal = m_pLocalPowerWorker->fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
					pPowerAlertNewMO = new PowerAlertLocalManagedObject (this, statebitMap, actionbitMap);
                    prismAssert (NULL != pPowerAlertNewMO, __FILE__, __LINE__);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding PowerAlertLocalManagedObject");
				}
			}
			if (lOM->m_defFruCfg.defFruObj[j].fru_type == FW_FRU_WWN) {
				if ((pWwnCardAlertMO != NULL)&&(pWwnCardAlertMO->size() == 0)) {
					actionVal = lOM->m_defFruCfg.defFruObj[j].alarm_action;
                    stateVal  = lOM->m_defFruCfg.defFruObj[j].alarm_state;
                    dcmstateVal = m_pLocalWwnCardWorker->fwtoDcmState (stateVal);
                    dcmactionVal = m_pLocalWwnCardWorker->fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
					pWwnCardAlertNewMO = new WwnCardAlertLocalManagedObject (this, statebitMap, actionbitMap);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding WwnCardAlertLocalManagedObject");
				}
			}
			if (lOM->m_defFruCfg.defFruObj[j].fru_type == FW_FRU_SFP) {
				if ((pSfpAlertMO != NULL)&&(pSfpAlertMO->size() == 0)) {
					actionVal = lOM->m_defFruCfg.defFruObj[j].alarm_action;
                    stateVal  = lOM->m_defFruCfg.defFruObj[j].alarm_state;
                    dcmstateVal = m_pLocalSfpWorker->fwtoDcmState (stateVal);
                    dcmactionVal = m_pLocalSfpWorker->fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
					pSfpAlertNewMO = new SfpAlertLocalManagedObject (this, statebitMap, actionbitMap);
                    prismAssert (NULL != pSfpAlertNewMO, __FILE__, __LINE__);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding SfpAlertLocalManagedObject");
				}
			}
			if (lOM->m_defFruCfg.defFruObj[j].fru_type == FW_FRU_SLOT) {
				if ((pLineCardAlertMO != NULL)&&(pLineCardAlertMO->size() == 0)) {
					actionVal = lOM->m_defFruCfg.defFruObj[j].alarm_action;
                    stateVal  = lOM->m_defFruCfg.defFruObj[j].alarm_state;
                    dcmstateVal = m_pLocalLineCardWorker->fwtoDcmState (stateVal);
                    dcmactionVal = m_pLocalLineCardWorker->fwtoDcmAction (actionVal);
                    statebitMap.setAllValues (dcmstateVal);
                    actionbitMap.setAllValues (dcmactionVal);
					pLineCardAlertNewMO = new LineCardAlertLocalManagedObject (this, statebitMap, actionbitMap);
					trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding LineCardAlertLocalManagedObject");
				}
			}
		}

		if (lOM->m_defFruCfg.defFruObj[FW_FRU_WWN].fru_type != FW_FRU_WWN) {
			statebitMap.setAllValues (32);
			actionbitMap.setAllValues (1);
			if ((pWwnCardAlertNewMO == NULL)&&(pWwnCardAlertMO != NULL)&&(pWwnCardAlertMO->size() == 0)){
				pWwnCardAlertNewMO = new WwnCardAlertLocalManagedObject (this, statebitMap, actionbitMap);
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding WwnCardAlertLocalManagedObject");
			}
		}
		if (lOM->m_defFruCfg.defFruObj[FW_FRU_SLOT].fru_type != FW_FRU_SLOT) {
			statebitMap.setAllValues (32);
			actionbitMap.setAllValues (1);
			if ((pLineCardAlertNewMO == NULL)&&(pLineCardAlertMO != NULL)&&(pLineCardAlertMO->size() == 0)) {
				pLineCardAlertNewMO = new LineCardAlertLocalManagedObject (this, statebitMap, actionbitMap);
				trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding LineCardAlertLocalManagedObject");
			}
		}

		if ((pWwnCardThreshNewMO == NULL)&&(pWwnCardThreshMO != NULL)&&(pWwnCardThreshMO->size() == 0)) {
			downTh = 2; marginalTh = 1;
			pWwnCardThreshNewMO = new WwnCardThresholdLocalManagedObject (this, downTh, marginalTh);
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding WwnCardThresholdLocalManagedObject");
		}
		if ((pMissingSfpThreshNewMO == NULL)&&(pMissingSfpThreshMO != NULL)&&(pMissingSfpThreshMO->size() == 0)) {
			downTh = 2; marginalTh = 1;
			pMissingSfpThreshNewMO = new MissingSfpThresholdLocalManagedObject (this, downTh, marginalTh);
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding MissingSfpThresholdLocalManagedObject");
		}
		if ((pFaultyPortsThreshNewMO == NULL)&&(pFaultyPortsThreshMO != NULL)&&(pFaultyPortsThreshMO->size() == 0)) {
			downTh = 2; marginalTh = 1;
			pFaultyPortsThreshNewMO = new FaultyPortsThresholdLocalManagedObject (this, downTh, marginalTh);
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding FaultyPortsThresholdLocalManagedObject");
		}
		if ((pMMThreshNewMO == NULL)&&(pMMThreshMO != NULL)&&(pMMThreshMO->size() == 0)) {
			downTh = 2; marginalTh = 1;
			pMMThreshNewMO = new MMThresholdLocalManagedObject (this, downTh, marginalTh);
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding MMThresholdLocalManagedObject");
		}
		if ((pLineCardThreshNewMO == NULL)&&(pLineCardThreshMO != NULL)&&(pLineCardThreshMO->size() == 0)) {
			downTh = 2; marginalTh = 1;
			pLineCardThreshNewMO = new LineCardThresholdLocalManagedObject (this, downTh, marginalTh);
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding LineCardThresholdLocalManagedObject");
		}
		if ((pSfmThreshNewMO == NULL)&&(pSfmThreshMO != NULL)&&(pSfmThreshMO->size() == 0)) {
			downTh = 2; marginalTh = 1;
			pSfmThreshNewMO = new SfmThresholdLocalManagedObject (this, downTh, marginalTh);
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects adding SfmThresholdLocalManagedObject");
		}

		status = commitTransaction();
		if (status == FRAMEWORK_SUCCESS)
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects - Default commit success");
		else
			trace (TRACE_LEVEL_INFO, "SystemMonitorLocalObjectManager::initManagedObjects - Default commit failed");


		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pFanThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPowerThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pTempThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pWwnCardThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCompactFlashThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMissingSfpThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pFaultyPortsThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMMThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pLineCardThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSfmThreshMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pFanAlertMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPowerAlertMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pWwnCardAlertMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pSfpAlertMO);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pLineCardAlertMO);

		delete pFanThreshNewMO;
		delete pPowerThreshNewMO;
		delete pTempThreshNewMO;
		delete pWwnCardThreshNewMO;
		delete pCompactFlashThreshNewMO;
		delete pMissingSfpThreshNewMO;
		delete pFaultyPortsThreshNewMO;
		delete pMMThreshNewMO;
		delete pLineCardThreshNewMO;
		delete pSfmThreshNewMO;
		delete pFanAlertNewMO;
		delete pPowerAlertNewMO;
		delete pWwnCardAlertNewMO;
		delete pSfpAlertNewMO;
		delete pLineCardAlertNewMO;

		return;
	}

    string  SystemMonitorLocalObjectManager::getClassName()
    {
        return ("SystemMonitor");
    }

    PrismServiceId  SystemMonitorLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SystemMonitorLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SystemMonitorLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
