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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "Igmps/Local/IgmpsLocalConfigWorker.h"

#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Igmps/Global/IgmpsGlobalConfigManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsMrtrVlanConfigManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsSgVlanConfigManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateIgmpsConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateMrtrVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateSgVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    IgmpsLocalObjectManager::IgmpsLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pIgmpsLocalConfigWorker = new IgmpsLocalConfigWorker (this);
        prismAssert (NULL != m_pIgmpsLocalConfigWorker, __FILE__, __LINE__);
    }

    IgmpsLocalObjectManager::~IgmpsLocalObjectManager ()
    {
    }

    IgmpsLocalObjectManager  *IgmpsLocalObjectManager::getInstance()
    {
        static IgmpsLocalObjectManager *pIgmpsLocalObjectManager = new IgmpsLocalObjectManager ();

        WaveNs::prismAssert (NULL != pIgmpsLocalObjectManager, __FILE__, __LINE__);

        return (pIgmpsLocalObjectManager);
    }

    string  IgmpsLocalObjectManager::getClassName()
    {
        return ("Igmps");
    }

    PrismServiceId  IgmpsLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *IgmpsLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IgmpsLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    IgmpsVlanIntfConfManagedObject*
    IgmpsLocalObjectManager::IgmpsVlanGetChildMoByParentMo(VlanIntfManagedObject *pVlanIntfManagedObject)
    {
        trace (TRACE_LEVEL_DEBUG, "IgmpsLocalObjectManager::IgmpsVlanGetChildMoByParentMo: Entered....");

        IgmpsVlanIntfConfManagedObject *pIgmpsVlanIntfConfManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsVlanIntfConfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (pVlanIntfManagedObject->getObjectId(), "ownerManagedObjectId"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0) {
                trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
                pIgmpsVlanIntfConfManagedObject = NULL;
            } else if (numberOfResults > 1) {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
                pIgmpsVlanIntfConfManagedObject = NULL;
            } else {
                pIgmpsVlanIntfConfManagedObject = dynamic_cast<IgmpsVlanIntfConfManagedObject*>((*pResults)[0]);
            }
            delete pResults;
        }

        return pIgmpsVlanIntfConfManagedObject;
    }

	void IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig()
	{
		IgmpsGlobalConfigManagedObject *pIgmpsGlobalConfigManagedObject = NULL;

		trace (TRACE_LEVEL_INFO, "IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig: Entered....");

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsGlobalConfigManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (1 == numberOfResults) {
				pIgmpsGlobalConfigManagedObject = dynamic_cast<IgmpsGlobalConfigManagedObject*>((*pResults)[0]);
				trace (TRACE_LEVEL_DEBUG, "IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig:numberOfResults = 1");
			} else {
                trace (TRACE_LEVEL_ERROR, "IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig:Unexpected numberOfResults");
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                return;
			}
		}

        if (! pIgmpsGlobalConfigManagedObject)
            return;

        IgmpsClientUpdateIgmpsConfigMessage *m = new IgmpsClientUpdateIgmpsConfigMessage ();
        m->setOpCode(IGMPS_GLOBAL_CONFIG_REPLAY);
        m->setEnable(pIgmpsGlobalConfigManagedObject->getEnable());
        m->setRestrictUnknownMcast(pIgmpsGlobalConfigManagedObject->getRestrictUnknownMcast());
        trace (TRACE_LEVEL_DEBUG, string("IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig:Opcode")+m->getOpCode());
        trace (TRACE_LEVEL_DEBUG, string("IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig:enable")+m->getEnable());
        trace (TRACE_LEVEL_DEBUG, string("IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig:restrict")+m->getRestrictUnknownMcast());
		ResourceId status = sendSynchronouslyToWaveClient ("IGMP", m);
		if (status == WAVE_MESSAGE_SUCCESS) {
			trace(TRACE_LEVEL_SUCCESS, string("IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig: sendToClient success"));
		} else {
			trace(TRACE_LEVEL_ERROR, string("IgmpsLocalObjectManager::ConfigReplayIgmpsGlobalConfig: sendToClient failed"));
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        delete m;
        return;
    }

/*	void IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig(vector<WaveManagedObject *>* &pResultsPMO)
	{
		UI32 i;
		ResourceId status = WAVE_MESSAGE_ERROR;

		trace (TRACE_LEVEL_INFO, "IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");

		WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> pIgmpsVlanConfigManagedObject = NULL;

		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

//		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
//		vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);
		trace (TRACE_LEVEL_INFO, "IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");
		if (NULL == pResultsPMO) {
			return;
		}

		UI32 numberOfResults = pResultsPMO->size ();
		if (0 == numberOfResults) {
			return;
		}
		trace(TRACE_LEVEL_DEVEL, string("IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: NumResults - ")+numberOfResults);
		trace (TRACE_LEVEL_INFO, "IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");
		for (i = 0; i < numberOfResults; i++) {
			pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsPMO)[i]);
            if (! pVlanIntfManagedObject)
                continue;

            pIgmpsVlanConfigManagedObject = pVlanIntfManagedObject->getIgmpsVlanIntfConfig();
            if(pIgmpsVlanConfigManagedObject.operator->() == NULL)
				continue;

			IgmpsClientUpdateVlanConfigMessage *m = new IgmpsClientUpdateVlanConfigMessage ();

			m->setLmqi(pIgmpsVlanConfigManagedObject->getLmqi());
			m->setQmrt(pIgmpsVlanConfigManagedObject->getQmrt());
			m->setEnable(pIgmpsVlanConfigManagedObject->getEnable());
			m->setFastLeave(pIgmpsVlanConfigManagedObject->getFastLeave());
			m->setQueryInterval(pIgmpsVlanConfigManagedObject->getQueryInterval());
			m->setQuerierEnable(pIgmpsVlanConfigManagedObject->getQuerierEnable());
			m->setMrouterTimeout(pIgmpsVlanConfigManagedObject->getMrouterTimeout());
			m->setVlanId(pVlanIntfManagedObject->getId());

			UI32 command = IGMPS_VLAN_LMQI | IGMPS_VLAN_QMRT | IGMPS_VLAN_QUERY_INTERVAL | 
				IGMPS_VLAN_MROUTER_TIMEOUT | IGMPS_VLAN_SNOOP_ENABLE | IGMPS_VLAN_FAST_LEAVE | IGMPS_VLAN_QUERIER_ENABLE;
			m->setOpCode(command);

            if(! ( (pIgmpsVlanConfigManagedObject->getLmqi() == 1000) &&
                (pIgmpsVlanConfigManagedObject->getQmrt() == 10 ) &&
                (!pIgmpsVlanConfigManagedObject->getEnable() ) &&
                (!pIgmpsVlanConfigManagedObject->getFastLeave() ) &&
                (pIgmpsVlanConfigManagedObject->getQueryInterval() ==125) &&
                (!pIgmpsVlanConfigManagedObject->getQuerierEnable() ) &&
                (pIgmpsVlanConfigManagedObject->getMrouterTimeout() == 300) ))
            {
			status = sendSynchronouslyToWaveClient ("IGMP", m);
			if (status == WAVE_MESSAGE_SUCCESS) {
				trace(TRACE_LEVEL_SUCCESS, string("IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: sendToClient success"));
			} else {
				trace(TRACE_LEVEL_ERROR, string("IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: sendToClient failed"));
			}
            }
            //delete pIgmpsVlanConfigManagedObject;
			delete m;
		}
		trace (TRACE_LEVEL_INFO, "IgmpsLocalObjectManager::ConfigReplayIgmpsVlanIntfConfig: Entered....");

    //    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		return;
	} */

/*	void IgmpsLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig(vector<WaveManagedObject *>* &pResultsPMO)
	{
		ResourceId status = WAVE_MESSAGE_ERROR;

		trace (TRACE_LEVEL_INFO, "IgmpsLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: Entered....");

		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

//		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
//		vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);
		if (NULL == pResultsPMO) {
			return;
		}

		UI32 numberOfResults = pResultsPMO->size ();
		if (0 == numberOfResults) {
			return;
		}
		trace(TRACE_LEVEL_DEVEL, string("IgmpsLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: NumResults - ")+numberOfResults);
		for (UI32 j = 0; j < numberOfResults; j++) {
			pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsPMO)[j]);
            if (! pVlanIntfManagedObject)
                continue;

            vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > igmpsMrtrVlanConfigMOVector =
                pVlanIntfManagedObject->getIgmpsMrtrVlanConfig();
            
            for (UI32 i = 0; i < igmpsMrtrVlanConfigMOVector.size(); i++) {
				if((IGMPS_IF_TYPE_FORTYGIGABIT == igmpsMrtrVlanConfigMOVector[i]->getIfType()) ||
                   (IGMPS_IF_TYPE_TENGIGABIT == igmpsMrtrVlanConfigMOVector[i]->getIfType()) || 
                   (IGMPS_IF_TYPE_GIGABIT == igmpsMrtrVlanConfigMOVector[i]->getIfType()))
				{
					// Do not do postboot for Fortygigabit/Tengigabit/Onegigabit ethernet. This will be done in NSM module.
					continue;
				}
                IgmpsClientUpdateMrtrVlanConfigMessage *m = new IgmpsClientUpdateMrtrVlanConfigMessage ();

                m->setVlanId(pVlanIntfManagedObject->getId());
                m->setIfType(igmpsMrtrVlanConfigMOVector[i]->getIfType());
                m->setIfName(igmpsMrtrVlanConfigMOVector[i]->getIfName());
                m->setNegation(false);

                if(true == NsmUtils::isValidThreeTuple(m->getIfName())) {
                    LocationId locationId = NsmUtils::getLocationId(m->getIfName());
                    if (locationId == NsmUtils::getLocalLocationId()) {
                        string twoTuppleIfName;
                        NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
                        m->setIfName(twoTuppleIfName);
                    } else {
                        // If this command does not belong to this node, discard.
                         // It will be replayed by corresponding node.
                         //
                        delete m;
                        continue;
                    }
                }

				trace (TRACE_LEVEL_INFO, string("IgmpsLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: Replay Mr ")+ m->getIfType()+ m->getIfName());
                status = sendSynchronouslyToWaveClient ("IGMP", m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_SUCCESS, string("IgmpsLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: sendToClient success"));
                } else {
                    trace(TRACE_LEVEL_ERROR, string("IgmpsLocalObjectManager::ConfigReplayIgmpsMrtrVlanConfig: sendToClient failed"));
                }
                delete m;
            }
        }

//        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		return;
    } */

	/*
	void IgmpsLocalObjectManager::ConfigReplayIgmpsSgVlanConfig(vector<WaveManagedObject *>* &pResultsPMO)
	{
		ResourceId status = WAVE_MESSAGE_ERROR;

		trace (TRACE_LEVEL_INFO, "IgmpsLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: Entered....");

		VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

//		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
//		vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);
		if (NULL == pResultsPMO) {
			return;
		}

		UI32 numberOfResults = pResultsPMO->size ();
		if (0 == numberOfResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
			return;
		}
		trace(TRACE_LEVEL_DEVEL, string("IgmpsLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: NumResults - ")+numberOfResults);
		for (UI32 j = 0; j < numberOfResults; j++) {
			pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResultsPMO)[j]);
            if (! pVlanIntfManagedObject)
                continue;

            vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > igmpsSgVlanConfigMOVector =
                pVlanIntfManagedObject->getIgmpsSgVlanConfig();
            
            for (UI32 i = 0; i < igmpsSgVlanConfigMOVector.size(); i++) {
				if((IGMPS_IF_TYPE_FORTYGIGABIT == igmpsSgVlanConfigMOVector[i]->getIfType()) || 
                   (IGMPS_IF_TYPE_TENGIGABIT == igmpsSgVlanConfigMOVector[i]->getIfType()) || 
                   (IGMPS_IF_TYPE_GIGABIT == igmpsSgVlanConfigMOVector[i]->getIfType()))
				{
					// Do not do postboot for Fortygigabit/Tengigabit/Onegigabit ethernet. This will be done in NSM module.
					continue;
				}

                IgmpsClientUpdateSgVlanConfigMessage *m = new IgmpsClientUpdateSgVlanConfigMessage ();

                m->setVlanId(pVlanIntfManagedObject->getId());
                m->setGroupAddress(igmpsSgVlanConfigMOVector[i]->getGroupAddress());
                m->setIfType(igmpsSgVlanConfigMOVector[i]->getIfType());
                m->setIfName(igmpsSgVlanConfigMOVector[i]->getIfName());
                m->setNegation(false);

                if(true == NsmUtils::isValidThreeTuple(m->getIfName())) {
                    LocationId locationId = NsmUtils::getLocationId(m->getIfName());
                    if (locationId == NsmUtils::getLocalLocationId()) {
                        string twoTuppleIfName;
                        NsmUtils::getTwoTupleIfName(m->getIfName(), twoTuppleIfName);
                        m->setIfName(twoTuppleIfName);
                    } else {
                        // If this command does not belong to this node, discard.
                         // It will be replayed by corresponding node.
                         //
                        delete m;
                        continue;
                    }
                }

				trace (TRACE_LEVEL_INFO, string("IgmpsLocalObjectManager::ConfigReplayIgmpsSgVlanConfig Replay Sg")+m->getIfType()   + m->getIfName());
                status = sendSynchronouslyToWaveClient ("IGMP", m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_SUCCESS, string("IgmpsLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: sendToClient success"));
                } else {
                    trace(TRACE_LEVEL_ERROR, string("IgmpsLocalObjectManager::ConfigReplayIgmpsSgVlanConfig: sendToClient failed"));
                }
                delete m;
            }
        }

//        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		return;
    }
*/
	void IgmpsLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
        string name = pWaveAsynchronousContextForPostBootPhase->getPassName();

		trace (TRACE_LEVEL_INFO, string("IgmpsLocalObjectManager::postboot Entered with pass:") + pWaveAsynchronousContextForPostBootPhase->getPassNum());

		if (name == "DCM_POSTBOOT_GLOBAL_STAGE1") {
            ConfigReplayIgmpsGlobalConfig();
		}
		else if (name == "DCM_POSTBOOT_GLOBAL_STAGE2") {
/*        
                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
                vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);
                ConfigReplayIgmpsVlanIntfConfig(pResultsPMO);
                ConfigReplayIgmpsMrtrVlanConfig(pResultsPMO);
                ConfigReplayIgmpsSgVlanConfig(pResultsPMO);
                if (pResultsPMO && pResultsPMO->size() > 0)
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
                
*/
        }
        
		trace (TRACE_LEVEL_INFO, string("IgmpsLocalObjectManager::postboot Completed"));
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
	}
}
