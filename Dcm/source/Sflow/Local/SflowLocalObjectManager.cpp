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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vsatyana                                                     *
 **************************************************************************/

#include "Sflow/Local/SflowLocalObjectManager.h"
#include "Sflow/Local/SflowLocalConfigWorker.h"
#include "Sflow/Global/SflowGlobalConfigManagedObject.h"
#include "Sflow/Global/SflowProfileManagedObject.h"
#include "ClientInterface/Sflow/SflowClientCreateSflowGlobalConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowClientSflowProfileConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    SflowLocalObjectManager::SflowLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pSflowLocalConfigWorker = new SflowLocalConfigWorker (this);
        prismAssert (NULL != m_pSflowLocalConfigWorker, __FILE__, __LINE__);
    }

    SflowLocalObjectManager::~SflowLocalObjectManager ()
    {
    }

    SflowLocalObjectManager  *SflowLocalObjectManager::getInstance()
    {
        static SflowLocalObjectManager *pSflowLocalObjectManager = new SflowLocalObjectManager ();

        WaveNs::prismAssert (NULL != pSflowLocalObjectManager, __FILE__, __LINE__);

        return (pSflowLocalObjectManager);
    }

    string  SflowLocalObjectManager::getClassName()
    {
        return ("Sflow");
    }

    PrismServiceId  SflowLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SflowLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SflowLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void SflowLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string  name = pWaveAsynchronousContextForPostBootPhase->getPassName();

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE1") {
                configReplaySflowGlobalConfig();
                configReplaySflowProfileConfig();
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

    void SflowLocalObjectManager::configReplaySflowProfileConfig()
    {
    	trace(TRACE_LEVEL_INFO, string("SflowLocalObjectManager:configReplaySflowProfileConfig: Entering "));
        SflowProfileManagedObject *pSflowProfileMO = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SflowProfileManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();
            trace(TRACE_LEVEL_INFO, string("SflowLocalObjectManager:configReplaySflowProfileConfig: Number of Profiles in DB ")+numberOfResults);
			for (UI32 i=0; i< numberOfResults; i++)
			{
				pSflowProfileMO = dynamic_cast<SflowProfileManagedObject *>((*pResults)[i]);
				if (pSflowProfileMO) {
					SflowClientSflowProfileConfigSPMessage *m = new SflowClientSflowProfileConfigSPMessage();
					m->setProfileName(pSflowProfileMO->getProfileName());
					m->setSamplingRate(pSflowProfileMO->getSamplingRate());
					m->setOpCode(SFLOW_MSG_PROFILE_CREATE);

					status = sendSynchronouslyToWaveClient("Sflow", m);
					if (status == WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_SUCCESS,string("configReplaySflowProfileConfig: configured "));
					} else {
						trace(TRACE_LEVEL_ERROR,string("configReplaySflowProfileConfig: Error"));
					}
					delete m;
					m = NULL;
				}
			}
	    	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

    void SflowLocalObjectManager::configReplaySflowGlobalConfig()
    {
    	trace(TRACE_LEVEL_INFO, string("SflowLocalObjectManager:configReplaySflowGlobalConfig: Entering "));
    	SflowGlobalConfigManagedObject *pSflowGlobalConfigManagedObject = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SflowGlobalConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size();
            trace(TRACE_LEVEL_INFO, string("SflowLocalObjectManager:configReplaySflowGlobalConfig: Number of SflowGlobalConfigManagedObjects to replay=")+numberOfResults);

	    for (UI32 i=0; i< numberOfResults; i++)
            {
                pSflowGlobalConfigManagedObject = dynamic_cast<SflowGlobalConfigManagedObject *>((*pResults)[i]);
                if (pSflowGlobalConfigManagedObject)
                {
                    SflowClientCreateSflowGlobalConfigSPMessage *m = new SflowClientCreateSflowGlobalConfigSPMessage();;
                    m->setEnable(pSflowGlobalConfigManagedObject->getEnable());
                    trace(TRACE_LEVEL_INFO, string("SflowLocalObjectManager: configReplaySflowGlobalConfig:sflow enabled=")+pSflowGlobalConfigManagedObject->getEnable());
                    m->setPollingInterval(pSflowGlobalConfigManagedObject->getPollingInterval());
                    m->setSampleRate(pSflowGlobalConfigManagedObject->getSampleRate());
		            m->setOpCode(SFLOW_MSG_SET_ALL);

                    status = sendSynchronouslyToWaveClient ("Sflow", m);
                    if (status == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_SUCCESS, string("ConfigReplaySflowGlobalConfig: configured "));
                    }
                    else
                    {
                        trace (TRACE_LEVEL_ERROR, string("ConfigReplaySflowGlobalConfig: Error"));
                    }
					
					delete m;
					m = NULL;
                }
            }
	    	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

	    trace(TRACE_LEVEL_INFO, string("Config Replay : Sflow Collector "));
        SflowCollectorManagedObject *pSflowCollectorMO = NULL;
        status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext collectorsyncQueryCtxt(SflowCollectorManagedObject::getClassName());
        vector<WaveManagedObject *> *pCollectorResults = querySynchronously(&collectorsyncQueryCtxt);
        if (NULL != pCollectorResults)
        {
            UI32 numberOfResults = pCollectorResults->size();
            trace(TRACE_LEVEL_INFO, string("Number of configured Sflow Collectors:")+numberOfResults);
			for (UI32 i=0; i< numberOfResults; i++)
			{
				pSflowCollectorMO = dynamic_cast<SflowCollectorManagedObject *>((*pCollectorResults)[i]);
				if (pSflowCollectorMO)
				{
					string collectorType = "";
					WaveNs::IpVxAddress ipAddr = pSflowCollectorMO->getCollectorIP();
					UI32 portNumber = pSflowCollectorMO->getCollectorPort();
					SflowClientCreateSflowGlobalConfigSPMessage *m = new SflowClientCreateSflowGlobalConfigSPMessage();
					m->setCollector(ipAddr);
					m->setPortNumber(portNumber);
					if(ipAddr.getAddressFamily() == AF_INET) {
						collectorType = "IPv4";
						m->setCollectorAddressType(SFLOW_COLLECTOR_ADDRESSTYPE_IPV4);
					}
					else if(ipAddr.getAddressFamily() == AF_INET6){
						collectorType = "IPv6";
						m->setCollectorAddressType(SFLOW_COLLECTOR_ADDRESSTYPE_IPV6);
					}
					else {
						NSM_PLUG_DBG_ERR_S(string("Error: Invalid collector address type!"));
					}
					m->setOpCode(SFLOW_MSG_COLLECTOR);
					status = sendSynchronouslyToWaveClient ("Sflow", m);
					if (status == WAVE_MESSAGE_SUCCESS)
					{
						trace(TRACE_LEVEL_SUCCESS, string("Replayed the collector:IP Address=")+ipAddr.toString()+":Port Number="+portNumber+":Address Type="+collectorType);
					}
					else
					{
						trace (TRACE_LEVEL_ERROR, string("ERROR: Sflow Collector Config Replay failed "));
					}
					delete m;
					m = NULL;
				}
			}
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCollectorResults);
        }
    }

}
