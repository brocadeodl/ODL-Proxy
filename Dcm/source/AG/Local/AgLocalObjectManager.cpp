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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Rohit Kumar/Ramachandran Anantharaman                        *
 **************************************************************************/


#include <algorithm>
#include <vector>
#include <sstream>

#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/TraceUtils.h"

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalWorker.h"
#include "AG/Local/AgMappingsPluginWorker.h"
#include "AG/Local/AgLocalTypes.h"

#include "AG/Local/AgLocalManagedObject.h"
#include "AG/Local/AgLocalWorker.h"
#include "AG/Local/AgMappingsMessage.h"
#include "AG/Local/AgPGPluginWorker.h"

#include "AG/Local/AgPortGroupLocalManagedObject.h"
#include "AG/Local/AgNPortMapLocalManagedObject.h"
#include "AG/Local/AgLocalConfigMessage.h"


#include "AG/Common/AgUtils.h"
#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "PortCfg/Local/PortCfgUpdateFCInfoMessage.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"

#include "agd/public.h"


#include "AG/Common/AgPluginTypes.h"
#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "ClientInterface/AG/AgClientConfigAllMessage.h"
#include "ClientInterface/AG/AgClientStaticMappingMessage.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{
    AgLocalObjectManager::AgLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pAgLocalWorker = new AgLocalWorker (this);
        prismAssert (NULL != m_pAgLocalWorker, __FILE__, __LINE__);

        m_pAgMappingsPluginWorker = new AgMappingsPluginWorker (this);
        prismAssert (NULL != m_pAgMappingsPluginWorker, __FILE__, __LINE__);

        m_pAgPGPluginWorker = new AgPGPluginWorker (this);
        prismAssert (NULL != m_pAgPGPluginWorker, __FILE__, __LINE__);


        associateWithVirtualWaveObjectManager (L3NodeSpecificGlobalObjectManager::getInstance());
    }
    
    AgLocalObjectManager::~AgLocalObjectManager ()
    {
        delete m_pAgLocalWorker;
        delete m_pAgMappingsPluginWorker;
        delete m_pAgPGPluginWorker;
    }

    AgLocalObjectManager  *AgLocalObjectManager::getInstance()
    {
        static AgLocalObjectManager *pAgLocalObjectManager = new AgLocalObjectManager ();

        WaveNs::prismAssert (NULL != pAgLocalObjectManager, __FILE__, __LINE__);

        return (pAgLocalObjectManager);
    }

    PrismServiceId  AgLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *AgLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *AgLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }


    int AgLocalObjectManager::isFactoryConfigRequired(LocationId locId)
    {
        int factoryRequired = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::isFactoryConfigRequired.. AG mode : ") + AgUtils::isAgModeEnabled());
        if (pResults)
        {
            WaveNs::trace(TRACE_LEVEL_DEBUG,  string("AgLocalObjectManager::isFactoryConfigRequired.. pResults is not NULL, size :") + pResults->size());
            if (pResults->size() == 0)
            {
                //AG MO is absent, factory config required
                factoryRequired = 1;
            } 
            else if (pResults->size() == 1)
            {
                //AG MO is present, factory config not required
                WaveNs::trace(TRACE_LEVEL_DEBUG,  string("AgLocalObjectManager::isFactoryConfigRequired.. AG MO Present !, size :") + pResults->size());
            }
            else
            {
                //Error case: AG MO cannot have more than a row
                WaveNs::trace(TRACE_LEVEL_ERROR,  string("AgLocalObjectManager::isFactoryConfigRequired.. Invalid MO Config, size :") + pResults->size());
            }
            //Cleanup the query results.
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        else
          WaveNs::trace(TRACE_LEVEL_DEBUG,  string("AgLocalObjectManager::isFactoryConfigRequired.. pResults is NULL"));

        return (factoryRequired);
    }

    void AgLocalObjectManager::printAgLocalMO(void)
    {
        AgLocalManagedObject *pAgLocalManagedObject = NULL;
        vector<string> policy;
        vector<string>::iterator iter;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::printAgLocalMO.. AG mode : ") + AgUtils::isAgModeEnabled());
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults)
        {
            WaveNs::trace(TRACE_LEVEL_DEBUG,  string("AgLocalObjectManager::printAgLocalMO.. pResults is not NULL") + pResults->size());
            if (pResults->size() == 1)
            {
                pAgLocalManagedObject =  dynamic_cast<AgLocalManagedObject*>((*pResults)[0]);
                policy = pAgLocalManagedObject->getPolicy();
                for(iter = policy.begin(); iter < policy.end(); iter++) {
                    WaveNs::trace(TRACE_LEVEL_DEBUG,  string("AgLocalObjectManager::printAgLocalMO.. policy :") + *iter);
                }
                WaveNs::trace(TRACE_LEVEL_DEBUG,  string("AgLocalObjectManager::printAgLocalMO.. fportTrunkEnable :") + pAgLocalManagedObject->m_fportTrunkEnable);
            }
            //Cleanup the query results.
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        else
            WaveNs::trace(TRACE_LEVEL_DEBUG,  string("AgLocalObjectManager::printAgLocalMO.. pResults is NULL"));
    }

    int AgLocalObjectManager::updateAgMO(vector<string >policy, bool fportTrunkEnabled, LocationId locId)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 default_fnmtov=DEFAULT_FNMTO_VALUE;
        UI32 default_relcounter = DEFAULT_REL_COUNTER_VALUE;
		
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgLocalManagedObject *pAgLocalManagedObject = NULL;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::updateAgMO.. "));

        // AG Local MO can have only one row
        if(pResults->size() == 0) {
            WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::updateAgMO.. New MO"));
            startTransaction();
            pAgLocalManagedObject = new AgLocalManagedObject(dynamic_cast<AgLocalObjectManager*>(getPWaveObjectManager()), policy, fportTrunkEnabled);
            pAgLocalManagedObject->setFnmtov(default_fnmtov);
            pAgLocalManagedObject->setRelCounterValue(default_relcounter);
            commitTransaction();
        } else {
            WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::updateAgMO.. Existing MO"));
            //startTransaction();
            //pAgLocalManagedObject = dynamic_cast<AgLocalManagedObject*>((*pResults)[0]);
            //updateWaveManagedObject(pAgLocalManagedObject);
            //pAgLocalManagedObject->setEnable(pAgLocalConfigMessage->m_agEnable);
            //commitTransaction();
        }

        // Clean up
        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        if (pAgLocalManagedObject) {
            delete(pAgLocalManagedObject);
        }
        return status;
    }

    int AgLocalObjectManager::updateNPortMapMO(string nport, vector<string> fportList, bool agfailover, bool failback, LocationId locId)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgNPortMapLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::updateNPortMapMO.. "));

        startTransaction();
        pAgNPortMapLocalManagedObject = new AgNPortMapLocalManagedObject(dynamic_cast<AgLocalObjectManager*>(getPWaveObjectManager()),
                                                nport, fportList, agfailover, failback);
        commitTransaction();

        // Clean up
        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        if (pAgNPortMapLocalManagedObject) {
            delete(pAgNPortMapLocalManagedObject);
        }
        return status;
    }

    int AgLocalObjectManager::updatePortGroupMO(int pgId, bool pgEnabled,  vector<string> pgFlags, string pgName, vector<string> nportList, vector<string> fportList, LocationId locId)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgPortGroupLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::updatePortGroupMO.. "));

        startTransaction();
        pAgPortGroupLocalManagedObject = new AgPortGroupLocalManagedObject(dynamic_cast<AgLocalObjectManager*>(getPWaveObjectManager()),
                                                pgId, pgEnabled, pgFlags, pgName, nportList, fportList);
        commitTransaction();

        // Clean up
        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        if (pAgPortGroupLocalManagedObject) {
            delete(pAgPortGroupLocalManagedObject);
        }
        return status;
    }

    void AgLocalObjectManager::addPortToSortedList(string port, vector<string> &portList) 
    {

        portList.push_back(port);

#if 0
        // The following logic works for integers, need to do it now for strings
        if (portList.size() == 0 || atoi(portList.front()) > port)
        {
            portList.insert(portList.begin(), port);
        }
        else if (atoi(portList.back()) < port)
        {
            //portList.insert(portList.end(), port);
            portList.push_back(port);
        }
        else
        {
            vector<string>::iterator iter;

            iter = portList.begin();
            while((port < *iter) && (iter < portList.end()))
                ++iter;

            if (port == *(iter - 1))
                return;
            else if (iter < portList.end())
                 portList.insert((iter - 1), port);
            else
                // We should not be hitting this case, as the first "else if" takes care
                // Keeping it just for completeness 
                portList.push_back(port);
        }
#endif
    }

    int AgLocalObjectManager::configNportsDefault(bool nportFlag)
    {
        PortCfgUpdateFCInfoMessage *pMessage = NULL;
        int localMappedId = AgUtils::getLocalMappedId();
        int status, locationId, port, completionStatus, mappedId;
        string intf;

        trace (TRACE_LEVEL_DEBUG, string ("AgLocalObjectManager::configNportsDefault - entry : "));
        pMessage = new PortCfgUpdateFCInfoMessage();

        if (nportFlag)
            pMessage->setLock_port(FC_CFG_MODE_N_PORT);
        else
            pMessage->setLock_port(FC_CFG_MODE_AUTO);

        pMessage->setDbEntryType(PortCfgLocalObjectManager::fc_db_type_modelock);

        trace (TRACE_LEVEL_INFO, string ("AgLocalObjectManager::configNportsDefault - MAX_PHY_PORT: ") + MAX_PHY_PORT
                              + string (" MAX_FI_PORT: ") + MAX_FI_PORT + string (" MAX_AG_PORT: ") + MAX_AG_PORT);

        for (port = 0; port < MAX_PHY_PORT; port++) {
            if(AgUtils::getInterfaceIdFromSwitchPort(localMappedId, AG_INTERFACE_TYPE_FI, port, intf) != -1) {

                pMessage->setId(intf);

                mappedId = NsmUtils::getMappedIdFromIfName(intf);
                if ((locationId = DcmToolKit::getLocationIdFromMappedId (mappedId)) == -1) {
                    trace(TRACE_LEVEL_ERROR, string("AgLocalObjectManager::configNportsDefault locationId is -1 for mappedId: ") + mappedId
                       + string (" intf: ") + intf);
                    delete pMessage;
                    return (-1);
                }

                trace (TRACE_LEVEL_INFO, string ("AgLocalObjectManager::configNportsDefault - Intf: ") + intf 
                      + string (" locationId: ") + locationId);

                status = sendSynchronously (pMessage, locationId);

                if (WAVE_MESSAGE_SUCCESS == status) {
                    completionStatus = pMessage->getCompletionStatus();
                    if (completionStatus == WAVE_MESSAGE_SUCCESS) {
                        trace (TRACE_LEVEL_INFO, "AgLocalObjectManager::configNportsDefault - completionStatus: Success.");
                    } else {
                        trace (TRACE_LEVEL_ERROR, "AgLocalObjectManager::configNportsDefault - completionStatus: Failure.");
                    }
                } else {
                    trace (TRACE_LEVEL_ERROR, "AgLocalObjectManager::configNportsDefault - Send Failed. " +
                            FrameworkToolKit::localize (status));
                    delete pMessage;
                    return(-1);
                }
            }
        }

        if(pMessage)
            delete pMessage;

         return (0);
    }

    int AgLocalObjectManager::downloadAgFactoryConfig(LocationId locId)
    {
	    int numberOfNPorts = 0;
        bool fportTrunkEnable = false, agfailover, failback;
        string nport, fport; 
        vector<string> fportList, pgNportList, pgFportList, pgModes, policy;
        string fportInterfaceConverted, nportInterfaceConverted;
        //int fportInteger, localMappedId, nportInteger;
	    unsigned int i, j;

	    // Update AG MO
        policy.push_back("pg");
	    updateAgMO(policy, fportTrunkEnable, locId);

	    //localMappedId = AgUtils::getLocalMappedId();

        WaveManagedObjectSynchronousQueryContext syncNPortQueryCtxt(PortFCCfgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pNPortResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncNPortQueryCtxt);
	    PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject;

        if (NULL != pNPortResults && pNPortResults->size()) {
	        for (i = 0; i < pNPortResults->size(); i++) {
            pPortFCCfgLocalManagedObject = dynamic_cast<PortFCCfgLocalManagedObject *>((*pNPortResults)[i]);
	        numberOfNPorts++;
            trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::downloadAgFactoryConfig... found ") + pPortFCCfgLocalManagedObject->getId() + string(" port # ") + pPortFCCfgLocalManagedObject->getSwitchPort());
	        }
        } else {
            trace (TRACE_LEVEL_INFO, string("AgLocalObjectManager::downloadAgFactoryConfig... none found "));
	        return (WAVE_MESSAGE_SUCCESS);
	    }

        trace (TRACE_LEVEL_INFO, string("AgLocalObjectManager::downloadAgFactoryConfig... found ") + numberOfNPorts + string(" nports"));

        WaveManagedObjectSynchronousQueryContext syncVFPortQueryCtxt(FcoeIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pVFPortResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncVFPortQueryCtxt);
        FcoeIntfLocalManagedObject *pFcoeIntfLocalManagedObject;

	    for (i = 0; i < pNPortResults->size(); i++) {
	        pPortFCCfgLocalManagedObject = dynamic_cast<PortFCCfgLocalManagedObject *>((*pNPortResults)[i]);
            //nportInteger = pPortFCCfgLocalManagedObject->getSwitchPort();
	        nportInterfaceConverted =
	            pPortFCCfgLocalManagedObject->getId();

		    nport.clear();
		    agfailover = true;
		    failback = true;
		    fportList.clear();

            pgNportList.push_back(nportInterfaceConverted);

	        if (NULL != pVFPortResults && pVFPortResults->size()) {
	            for (j = 0; j < pVFPortResults->size(); j++) {
	                pFcoeIntfLocalManagedObject = dynamic_cast<FcoeIntfLocalManagedObject *>((*pVFPortResults)[j]);

	                if ((pFcoeIntfLocalManagedObject->getFcUportNum() % numberOfNPorts) != pPortFCCfgLocalManagedObject->getSwitchPort()) {
	                    continue;
	                }

                    trace (TRACE_LEVEL_INFO, string("AgLocalObjectManager::downloadAgFactoryConfig... found vfport ") + pFcoeIntfLocalManagedObject->getName() + string(" port # ") + pFcoeIntfLocalManagedObject->getFcUportNum() + string(" for nport ") + pPortFCCfgLocalManagedObject->getSwitchPort());

                    //fportInteger = pFcoeIntfLocalManagedObject->getFcUportNum();
	                fportInterfaceConverted =
	                    pFcoeIntfLocalManagedObject->getName();

                    fportList.push_back(fportInterfaceConverted);
                    pgFportList.push_back(fportInterfaceConverted);
                }
	        }

	        std::sort(fportList.begin(), fportList.end(), compareByPort);

            // Update NPortMap MO
            updateNPortMapMO(nportInterfaceConverted, fportList,
	            agfailover, failback, locId);
        }

        // Update PortGroup MO
        pgModes.push_back("lb");
        updatePortGroupMO(0, true, pgModes, "pg0",
	        pgNportList, pgFportList, locId);

        return 0;
    }

    bool AgLocalObjectManager::compareByPort(string a, string b)
    {
	    return (NsmUtils::getPortId(a) < NsmUtils::getPortId(b));
    }

    int AgLocalObjectManager::configAgDefaults(LocationId locId)
    {
        //By default configure all FC ports as Nports
        if(configNportsDefault(true) != 0)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string("AgLocalObjectManager::configAgDefaults... configNportDefault failed"));
        }

        //Download AG factory configuration
        int rc = downloadAgFactoryConfig(locId);

        return (rc);
    }

    void AgLocalObjectManager::clearAgPortGroupLocalMO(LocationId locId)
    {
        UI32 row;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgPortGroupLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::clearAgPortGroupLocalMO.."));

        if(pResults && (pResults->size() != 0)) {
            startTransaction();
            for (row = 0; row < pResults->size(); ++row)
            {
                pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject*>((*pResults)[row]);
                delete pAgPortGroupLocalManagedObject;
            }
            commitTransaction();
        }
    }

    void AgLocalObjectManager::clearAgNPortMapLocalMO(LocationId locId)
    {
        UI32 row;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgNPortMapLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::clearAgNPortMapLocalMO.."));

        if(pResults && (pResults->size() != 0)) {
            startTransaction();
            for (row = 0; row < pResults->size(); ++row)
            {
                pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject*>((*pResults)[row]);
                delete pAgNPortMapLocalManagedObject;
            }
            commitTransaction();
        }
    }

    void AgLocalObjectManager::clearAGLocalMO(LocationId locId, bool &agDisableFirstRebootFlag)
    {
        UI32 row;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgLocalManagedObject *pAgLocalManagedObject = NULL;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::clearAGLocalMO..."));

        agDisableFirstRebootFlag = false;
        if(pResults && (pResults->size() != 0)) {
            agDisableFirstRebootFlag = true;
            startTransaction();
            for (row = 0; row < pResults->size(); ++row)
            {
                pAgLocalManagedObject = dynamic_cast<AgLocalManagedObject*>((*pResults)[row]);
                delete pAgLocalManagedObject;
            }
            commitTransaction();
        }
    }

    void AgLocalObjectManager::removeAgConfigFromRunningConfig(LocationId locId)
    {
        bool agDisableFirstRebootFlag;
        clearAGLocalMO(locId, agDisableFirstRebootFlag);
        clearAgNPortMapLocalMO(locId);
        clearAgPortGroupLocalMO(locId);

        //If its first reboot after AG disable, configure FC ports back to Gports
        if (agDisableFirstRebootFlag)
            configNportsDefault(false);
    }

    void AgLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

        if ((WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) || (WAVE_BOOT_SECONDARY_NODE_REJOIN_BOOT == bootReason))         {

	    WaveNs::trace(TRACE_LEVEL_INFO,  string("AgLocalObjectManager::boot entering..."));
    	    bool factoryConfigDwlStatus = true;
    	    LocationId locId = FrameworkToolKit::getThisLocationId();

	    /* Check if AG is enabled:
	     * => if enabled, check if the AG MO is present. If not present, create all the MOs
	     * based on AG factory file. and push it to AG daemon
	     * => if disabled, cleanup the existing config, if any
	     */
	    if (AgUtils::isAgModeEnabled())
	    {
		if(isFactoryConfigRequired(locId))
		{
			WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::boot.. Calling download factory config !!!"));
			if (configAgDefaults(locId) != 0)
			{
				factoryConfigDwlStatus = false;
				WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::boot.. Downloading factory config failed !!!"));
			}
			else
			{
				WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::boot.. Downloading factory config success !!!"));
			}
		}

		if (!factoryConfigDwlStatus || (pushConfigToAgDaemon(locId) != 0))
		{
			WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::boot.. Push AG Config to Daemon failed!!!"));
		}
		else
		{
			WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::boot.. Push AG Config to Daemon success!!!"));
		}
	    }
	    else
	    {
	    	removeAgConfigFromRunningConfig(locId);
	    }

        }

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    int AgLocalObjectManager::updatePGToConfigAllClientMsg(AgClientConfigAllMessage *msg_p, LocationId locId)
    {
        UI32 nbPg, index;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgPortGroupLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgPortGroupLocalManagedObject *pAgPGMO = NULL;
        UI32 mode_temp = 0;
        vector<string>::iterator iter;
        string mode;
        vector<string> switchFPort_str, switchNPort_str;
        string fportString, nportString;
        char buf[10];
        int switchFPort, switchNPort;



        if((nbPg = pResults->size()) != 0)
        {
            for (index = 0; index < nbPg; ++index)
            {
                pAgPGMO = dynamic_cast<AgPortGroupLocalManagedObject*>((*pResults)[index]);

                for(iter = (pAgPGMO->m_agPgFlags).begin();iter!=(pAgPGMO->m_agPgFlags).end();iter++) {
                    mode = *iter;
                    if (strcmp(mode.c_str(), AG_PGFLG_LB_NAME) == 0) {
                        mode_temp |= AG_PGFLG_LB;
                    } else if (strcmp(mode.c_str(), AG_PGFLG_MFNM_NAME) == 0) {
                        mode_temp |= AG_PGFLG_MFNM;
                    } else if (strcmp(mode.c_str(), AG_PGFLG_NPORT_UP_NO_REBAL_NAME) == 0) {
                        mode_temp |= AG_PGFLG_NPORT_UP_NO_REBAL;
                    }
                }

                // convert 3 tuple to switch port just before sending to backend

                switchNPort_str.clear();
                for(iter =  (pAgPGMO->m_agPgNPortTopo).begin(); iter != (pAgPGMO->m_agPgNPortTopo).end();iter++) {
                    if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, switchNPort)) != -1) {
                        sprintf(buf, "%d", switchNPort);
                        nportString.assign(buf);
                        switchNPort_str.push_back(nportString);
                    } else {
                        WaveNs::trace (TRACE_LEVEL_ERROR, string("iAgLocalObjectManager::updatePGToConfigAllClientMsg : Error in conversion to switch port of N port "));

                    }
                }

                switchFPort_str.clear();
                for(iter =  (pAgPGMO->m_agPgFPortTopo).begin(); iter != (pAgPGMO->m_agPgFPortTopo).end();iter++) {
                    if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, switchFPort)) != -1) {
                        sprintf(buf,"%d",switchFPort);
                        fportString.assign(buf);
                        switchFPort_str.push_back(fportString);
                    } else {
                        WaveNs::trace (TRACE_LEVEL_ERROR, string("AgLocalObjectManager::updatePGToConfigAllClientMsg : Error in conversion to switch port of F port "));

                    }
                }

                msg_p->addPortGroup(pAgPGMO->m_agPgId, pAgPGMO->m_agPgEnabled, mode_temp,
                        pAgPGMO->m_agPgName, switchNPort_str, switchFPort_str);
            }
        }

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        return (0);
    }

    int AgLocalObjectManager::updateNPortMapToConfigAllClientMsg(AgClientConfigAllMessage *msg_p, LocationId locId)
    {
        UI32 nbPorts, index;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgNPortMapLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        AgNPortMapLocalManagedObject *pAgNPortMO = NULL;
        vector<string>::iterator iter;
        int switchFPort, switchNPort;
        vector<string> switchFPort_str;
        string fportString, nportString;
        char buf[10];


        if((nbPorts = pResults->size()) != 0)
        {
            for (index = 0; index < nbPorts; ++index)
            {
                pAgNPortMO = dynamic_cast<AgNPortMapLocalManagedObject*>((*pResults)[index]);

                // convert 3 tuple to switch port just before sending to backend

                if((AgUtils::getSwitchPortFromInterfaceId(pAgNPortMO->m_agNPortNb, AG_INTERFACE_TYPE_FI, switchNPort)) != -1) { 
                    sprintf(buf,"%d",switchNPort);
                    nportString.assign(buf);

                } else {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string("AgLocalObjectManager::updateNPortMapToConfigAllClientMsg : Error in conversion to switch port of N port!"));
                }

                switchFPort_str.clear();
                for(iter =  (pAgNPortMO->m_agNFPortTopo).begin(); iter != (pAgNPortMO->m_agNFPortTopo).end();iter++) {
                    if((AgUtils::getSwitchPortFromInterfaceId(*iter,AG_INTERFACE_TYPE_FCOE, switchFPort)) != -1) {
                        sprintf(buf,"%d",switchFPort);
                        fportString.assign(buf);
                        switchFPort_str.push_back(fportString);
                    } else {
                        WaveNs::trace (TRACE_LEVEL_ERROR, string("AgLocalObjectManager::updateNPortMapToConfigAllClientMsg : Error in conversion to switch port of F port "));

                    }
                }

                msg_p->addNportMap(nportString, switchFPort_str, pAgNPortMO->m_agFailover, pAgNPortMO->m_agFailback);

            }
        }

        return (0);
    }

    int AgLocalObjectManager::pushStaticMappingConfigToAgDaemon(LocationId locId)
    {
        int nbNPorts, index, ret = 0;
        AgNPortMapLocalManagedObject *pAgNportMO;
        WaveManagedObjectSynchronousQueryContext syncQueryCtx(AgNPortMapLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtx);
        AgClientStaticMappingMessage *msg_p = NULL;
        ResourceId status;
        char buf[10];
        string fport_str;
        vector<string>::iterator iter;
        int fportinteger, nportinteger;


        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::pushStaticMappingConfigToAgDaemon.. "));
        if(pResults && ((nbNPorts = pResults->size()) != 0)) {

            for (index = 0; index < nbNPorts; ++index) {

                pAgNportMO = dynamic_cast<AgNPortMapLocalManagedObject*>((*pResults)[index]);

                WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::pushStaticMappingConfigToAgDaemon Nport:") + pAgNportMO->m_agNPortNb);

                if((pAgNportMO->m_agStaticNFPortTopo).size()) {
                    msg_p = new AgClientStaticMappingMessage();

                    if((AgUtils::getSwitchPortFromInterfaceId(pAgNportMO->m_agNPortNb, AG_INTERFACE_TYPE_FI, nportinteger)) !=-1) {
                        sprintf(buf,"%d",nportinteger);
                        (msg_p->m_nPort).assign(buf);
                    } else {
                       return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                    }

                    for(iter = (pAgNportMO->m_agStaticNFPortTopo).begin(); iter != (pAgNportMO->m_agStaticNFPortTopo).end(); iter++) {
                        if((AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, fportinteger)) !=-1) {
                            sprintf(buf,"%d",fportinteger);
                            fport_str.assign(buf);
                            (msg_p->m_staticFPorts).push_back(fport_str);
                        } else {
                            return AG_TRANSLATION_FROM_INTER_TO_SWPORT_FAILED;
                        }
                    }
    
                    msg_p->m_mode = AG_STATIC_MAPADD;

                    status = sendSynchronouslyToWaveClient("agd", msg_p);

                    if (status != WAVE_MESSAGE_SUCCESS) {
                        WaveNs::trace (TRACE_LEVEL_ERROR, string("AgLocalObjectManager::pushStaticMappingConfigToAgDaemon..faailed nport intf:") + pAgNportMO->m_agNPortNb + string("status ") + status);
    
                        ret = -1;
                        if (msg_p) {
                            delete msg_p;
                        }
                        break;
                    }

                    if (msg_p) {
                        delete msg_p;
                        msg_p = NULL;
                    }
                }
            }
        }

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        return (ret);
    }


    int AgLocalObjectManager::pushConfigToAgDaemon(LocationId locId)
    {
        int ret = 0;
        AgClientConfigAllMessage *msg_p = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        //Just opening and closing these two log files to make sure
        //we have new info at every reboot
        //And we cannot open as "w" in the client message since we open and
        //close these files for every PG/mapping.

        FILE *mapping = fopen("/var/log/client_message_mapping", "w");
        fclose(mapping);

        FILE *pgs = fopen("/var/log/client_message_pgs", "w");
        fclose(pgs);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AgLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
        
        AgLocalManagedObject *pAgLocalMO = NULL;

        WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::pushConfigToAgDaemon.. "));

        if(pResults->size() != 0) {
            WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgLocalObjectManager::pushConfigToAgDaemon.. Existing MO"));
            pAgLocalMO = dynamic_cast<AgLocalManagedObject*>((*pResults)[0]);
            msg_p = new AgClientConfigAllMessage(pAgLocalMO->m_policy, pAgLocalMO->m_fportTrunkEnable, pAgLocalMO->m_fnmtovalue, pAgLocalMO->m_relcountervalue);

            updateNPortMapToConfigAllClientMsg(msg_p, locId);
            updatePGToConfigAllClientMsg(msg_p, locId);

            // Clean up
            if (pResults) {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }

            status = sendSynchronouslyToWaveClient("agd", msg_p);
            if (status == WAVE_MESSAGE_SUCCESS)
                ret = 0;
            else
                ret = -1;

            delete msg_p;


            // Push Static Mapping configuration
            if (ret == 0) {
                ret = pushStaticMappingConfigToAgDaemon(locId);
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string("AgLocalObjectManager::pushConfigToAgDaemon.. Failed: AG MO Not found!!"));
        }

        return ret;
    }

    void AgLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase)
    {
        WaveNs::trace(TRACE_LEVEL_INFO,  string("AgLocalObjectManager::postboot entering..."));
        bool factoryConfigDwlStatus = true;
		LocationId locId = FrameworkToolKit::getThisLocationId();

        /* Check if AG is enabled:
         * => if enabled, check if the AG MO is present. If not present, create all the MOs
         * based on AG factory file. and push it to AG daemon
         * => if disabled, cleanup the existing config, if any
         */
        if (AgUtils::isAgModeEnabled())
        {
            if(isFactoryConfigRequired(locId))
            {
                WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::postboot.. Calling download factory config !!!"));
                if (configAgDefaults(locId) != 0)
                {
                    factoryConfigDwlStatus = false;
                    WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::postboot.. Downloading factory config failed !!!"));
                }
                else
                {
                    WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::postboot.. Downloading factory config success !!!"));
                }
            }

            WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::postboot.. Calling code to push AG Config to Daemon!!"));
            if (!factoryConfigDwlStatus || (pushConfigToAgDaemon(locId) != 0))
            {
                WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::postboot.. Push AG Config to Daemon failed!!!"));
            }
            else
            {
                WaveNs::trace(TRACE_LEVEL_INFO, string("AgLocalObjectManager::postboot.. Push AG Config to Daemon success!!!"));
            }
        }
        else
        {
            removeAgConfigFromRunningConfig(locId);
        }

        pWaveAsynchronousContextForPostbootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostbootPhase->callback ();
    }
}
