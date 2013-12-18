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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"

#include "DcmResourceIdEnums.h"

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/IntfConfGlobalWorker.h"
#include "Nsm/Global/Eld/EldGlobalWorker.h"
#include "Nsm/Global/VlanClassifierGlobalConfWorker.h"
#include "Nsm/Global/PVlanGlobalConfWorker.h"
#include "Nsm/Global/Igmps/IgmpsVlanGlobalConfWorker.h"
#include "Nsm/Global/OSPF/OSPFInterfaceGlobalConfWorker.h"
#include "Nsm/Global/VRRPWorker/VRRPGlobalConfWorker.h"
#include "Nsm/Global/Pim/PimIntfCfgGlobalWorker.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/IntfXstpGlobalConfWorker.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/Igmp/IgmpGlobalWorker.h"

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/Dot1xGlobalWorker.h"
#include "Nsm/Global/NsmGlobalTypes.h"

#include "Nsm/Local/Rmon/RmonHistoryBookKeepLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonStatisticsBookKeepLocalManagedObject.h"
#include "Nsm/Local/EcmpConfigManagedObject.h"

#include "Nsm/Global/NsmGlobalCreateVlanIntfMessage.h"
#include "Nsm/Global/XstpPortVlanConfigManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"

#include "Cluster/Local/WaveNode.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Global/Layer3/NsmGlobalL3IpWorker.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/ConnectorConfigLocalManagedObject.h"
//FCOE
#include "Nsm/Global/FcoeWorker/NsmFcoeGlobalWorker.h"

#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Ssm/SsmApp.h"

#include "Utils/DceClusterUtils.h"

#include <algorithm>
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "DcmCore/DcmToolKit.h"
#include "Nsm/Global/Eld/EldGlobalWorker.h"

using namespace std;

namespace DcmNs
{

    NsmGlobalObjectManager::NsmGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pEldGlobalWorker = new EldGlobalWorker (this);
        prismAssert (NULL != m_pEldGlobalWorker, __FILE__, __LINE__);
        m_pIntfConfGlobalWorker = new IntfConfGlobalWorker (this);
        prismAssert (NULL != m_pIntfConfGlobalWorker, __FILE__, __LINE__);
		m_pVlanClassifierGlobalConfWorker = new VlanClassifierGlobalConfWorker (this);
        prismAssert (NULL != m_pVlanClassifierGlobalConfWorker, __FILE__, __LINE__);
		m_pIntfXstpGlobalConfWorker = new IntfXstpGlobalConfWorker (this);
		prismAssert (NULL != m_pIntfXstpGlobalConfWorker, __FILE__, __LINE__);
        m_pDot1xGlobalWorker = new Dot1xGlobalWorker (this);
        prismAssert (NULL != m_pDot1xGlobalWorker, __FILE__, __LINE__);

        //PVLAN
        m_pPVlanGlobalConfWorker = new PVlanGlobalConfWorker (this);
        prismAssert (NULL != m_pPVlanGlobalConfWorker, __FILE__, __LINE__);

        m_pPortSecurityGlobalConfWorker = new PortSecurityGlobalConfWorker(this);
        prismAssert (NULL != m_pPortSecurityGlobalConfWorker, __FILE__, __LINE__);

        // FCOE Worker
        m_pNsmFcoeGlobalWorker = new NsmFcoeGlobalWorker(this);
        prismAssert (NULL != m_pNsmFcoeGlobalWorker, __FILE__, __LINE__);

        m_pIgmpsVlanGlobalConfWorker = new IgmpsVlanGlobalConfWorker (this);
        prismAssert (NULL != m_pIgmpsVlanGlobalConfWorker, __FILE__, __LINE__);

		//OSPF Global Worker
        m_pOSPFInterfaceGlobalConfWorker = new OSPFInterfaceGlobalConfWorker (this);
        prismAssert (NULL != m_pOSPFInterfaceGlobalConfWorker, __FILE__, __LINE__);

		//VRRP Global Worker
        m_pVRRPGlobalConfWorker = new VRRPGlobalConfWorker (this);
        prismAssert (NULL != m_pVRRPGlobalConfWorker, __FILE__, __LINE__);
        
        //PIM Global Worker
        m_pPimIntfCfgGlobalWorker = new PimIntfCfgGlobalWorker (this);
        prismAssert (NULL != m_pPimIntfCfgGlobalWorker, __FILE__, __LINE__);

		m_pNsmGlobalL3IpWorker = new NsmGlobalL3IpWorker(this);
		prismAssert (NULL != m_pNsmGlobalL3IpWorker, __FILE__, __LINE__);

        m_pIgmpGlobalWorker = new IgmpGlobalWorker (this);
        prismAssert (NULL != m_pIgmpGlobalWorker, __FILE__, __LINE__);
    }

    NsmGlobalObjectManager::~NsmGlobalObjectManager ()
    {
		delete m_pIntfXstpGlobalConfWorker;

        delete m_pNsmFcoeGlobalWorker;

        delete m_pNsmGlobalL3IpWorker;
        
        delete m_pPimIntfCfgGlobalWorker;

        delete m_pIgmpGlobalWorker;
    }

    NsmGlobalObjectManager  *NsmGlobalObjectManager::getInstance()
    {
        static NsmGlobalObjectManager *pNsmGlobalObjectManager = new NsmGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pNsmGlobalObjectManager, __FILE__, __LINE__);

        return (pNsmGlobalObjectManager);
    }

    PrismServiceId  NsmGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *NsmGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMGLOBALCREATEVLANINTF:
                trace (TRACE_LEVEL_ERROR, string ("NSMclientMangIntfMsgInstantiator : known Operation Code : ") + operationCode);
                pPrismMessage = new NsmGlobalCreateVlanIntfMessage ();
            break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
        return (pWaveManagedObject);
    }


    IntfXstpGlobalConfWorker* NsmGlobalObjectManager::getIntfXstpGlobalConfWorker() 
	{
		return m_pIntfXstpGlobalConfWorker;
	}


    void
    NsmGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage,
                                                                 PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        NSM_PLUG_DBG("Cluster Collect function called.");

        data_p = dce_cluster_validation_collect_init(NSM_PLUGIN_GLOBAL_VERSION);
        if (data_p == NULL) {
            NSM_PLUG_DBG_ERR("Failed to init ValidationData");
            pPrismAsynchronousContext->setCompletionStatus (DCE_CLUSTER_UTIL_ALLOC_FAILED);
            pPrismAsynchronousContext->callback ();
            return;
        }

        status = dce_cluster_validation_collect_done(pMessage, data_p);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            pPrismAsynchronousContext->setCompletionStatus (status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }   

    void
    NsmGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage,
                                                    PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;

        NSM_PLUG_DBG("Cluster Validate function called.");

        pMessage->getValidationDetails(payload_p, len);

        NSM_PLUG_DBG_S(string("Received Validation Data of LEN=") + len);

        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            NSM_PLUG_DBG_ERR_S(string("Failed to parse ValidationData Err=") +
                                FrameworkToolKit::localize(status));
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        NSM_PLUG_DBG_S(string("Recieved version = ") + version);

        if (version != NSM_PLUGIN_GLOBAL_VERSION) {
            NSM_PLUG_DBG_ERR_S(string("Version mismatch, My-Version=") + NSM_PLUGIN_GLOBAL_VERSION +
                                string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(NSM_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }
 
    void NsmGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
        vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_DEVEL, string ("NsmGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));

        if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
        {
            UI32 noOfFailedLocations = failedLocationIds.size ();
            IntfXstpFailoverContext xstpContext;

            if (0 < noOfFailedLocations) {
                // Query PhyIntfLocalMOs for freeing stp portId
                for (UI32 z = 0; z < noOfFailedLocations; z++) {
                    LocationId failedLocationId = failedLocationIds[z];
                    xstpContext.m_rbridges.insert(DcmToolKit::getMappedIdFromLocationId(failedLocationId));

                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName ());

                    syncQueryCtxt.addSelectField("poId");
                    syncQueryCtxt.addSelectField("id");

                    UI32 zero = 0;
                    syncQueryCtxt.addAndAttribute(new AttributeUI32(&zero, "poMode"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);
                    syncQueryCtxt.addAndAttribute(new AttributeUI32(&zero, "poType"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

                    syncQueryCtxt.setLocationIdFilter(failedLocationId);
                    //syncQueryCtxt.printSqlToDebugError();

                    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                    for (UI32 j = 0; j < pResults->size(); j++) {
                        PhyIntfLocalManagedObject *pIntfMo = dynamic_cast<PhyIntfLocalManagedObject*>(pResults->at(j));
                        ObjectId poOid = pIntfMo->getPoId();
                        if (poOid != ObjectId::NullObjectId) {
                            xstpContext.m_poObjectIds.insert(poOid.getInstanceId());
                        }
                    }
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                }

                vector<string> moClassNames;
                moClassNames.push_back(RmonHistoryBookKeepLocalManagedObject::getClassName ());
                moClassNames.push_back(RmonStatisticsBookKeepLocalManagedObject::getClassName ());
                moClassNames.push_back(NsmIntfSviManagedObject::getClassName ());
                moClassNames.push_back(NsmIntfLoopbackManagedObject::getClassName ());
                moClassNames.push_back(ConnectorConfigLocalManagedObject::getClassName ());
                moClassNames.push_back(EcmpConfigManagedObject::getClassName ());
                moClassNames.push_back(PhyIntfLocalManagedObject::getClassName ());

                startTransaction ();

                UI32 numberOfmoClassNames = moClassNames.size ();
                status = m_pIgmpsVlanGlobalConfWorker->deleteIgmpsTenGigIntfMOForLocation(failedLocationIds);

                            if (WAVE_MESSAGE_SUCCESS != status) {
                                trace (TRACE_LEVEL_FATAL, string("NsmGlobalObjectManager::failover:Error occurred in call to function deleteIgmpsTenGigIntfMOForLocation() : ") );
                            }
                        

                for (UI32 i = 0; i < numberOfmoClassNames; i++) {
                    string managedObjectClassName = moClassNames[i];
                    trace (TRACE_LEVEL_INFO, "NsmGlobalObjectManager::failover : [" + managedObjectClassName  + "]");
                    // Skip if not a Local Managed Object.
                    if (false == OrmRepository::isALocalManagedObject (managedObjectClassName)) {
                        trace (TRACE_LEVEL_INFO, "NsmGlobalObjectManager::failover : [" + managedObjectClassName  + "] is not a Local Managed Object. Skipped.");
                        continue;
                    }
                    WaveManagedObjectSynchronousQueryContextForDeletion *deleteCtxt = new WaveManagedObjectSynchronousQueryContextForDeletion (managedObjectClassName);
                    for (UI32 z = 0; z < noOfFailedLocations; z++) {
                        LocationId failedLocationId = failedLocationIds[z];
                        ObjectId locationIdObjectId = FrameworkToolKit::getObjectIdForLocationId (failedLocationId);
                        deleteCtxt->addOrAttribute (new AttributeObjectId (locationIdObjectId, "ownerWaveNodeObjectId"));

                    }
                    deleteMultipleWaveManagedObjects (deleteCtxt);
                }

                status = commitTransaction ();
               if (FRAMEWORK_SUCCESS != status) {
                    trace (TRACE_LEVEL_FATAL, "NsmGlobalObjectManager::failover can not commit local managed object delete to database ");
                }


                // Cleanup STP related information (port id)
                m_pIntfXstpGlobalConfWorker->failover(xstpContext);
            }
        }
        pFailoverAsynchronousContext->setCompletionStatus (status);
        pFailoverAsynchronousContext->callback ();
    }

/* ELD Global config replay */
#if 0
    UI32 setGlobalConfigForReplay (EldConfigManagedObject
                                        *pEldConfigMO, EldClientUpdateRxlimConfigMessage *m)
    {
        UI32 cmdcode = 0x00000000;

		m->setRxlim(pEldConfigMO->getRxlim());
		cmdcode |= ELD_GLOBAL_CONF_VCS_RX_LIMIT;	
		
		m->setHelloInt(pEldConfigMO->getHello());
		cmdcode |= ELD_GLOBAL_CONF_HELLO_TIME;	

		m->setShutdnTime(pEldConfigMO->getShutdown());
		cmdcode |= ELD_GLOBAL_CONF_SHUTDOWN_TIME;	

		if (cmdcode == 0) 
			return 0;

		m->setOpCode(cmdcode);
		return 1;
	}

    void NsmGlobalObjectManager::ConfigReplayEldGlobalScalars()
    {
        EldConfigManagedObject *pEldConfigMO = NULL;
        ResourceId status = WAVE_MESSAGE_ERROR;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(EldConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            trace(TRACE_LEVEL_DEVEL, string("EldConfigManagedObject: NumResults - ") + numberOfResults);

            if (numberOfResults == 1) {
                EldClientUpdateRxlimConfigMessage *m = new EldClientUpdateRxlimConfigMessage();
                pEldConfigMO = dynamic_cast<EldConfigManagedObject *>((*pResultsPMO)[0]);

                status = setGlobalConfigForReplay (pEldConfigMO, m);

                if (status != 0) {
                    status = sendSynchronouslyToWaveClient ("eld", m);
                    if (status == WAVE_MESSAGE_SUCCESS) {
                        trace(TRACE_LEVEL_SUCCESS, string("EldConfigManagedObject: configured "));
                    } else {
                        trace(TRACE_LEVEL_ERROR, string("EldConfigManagedObject: configured"));
                    }
                } else {
                    trace(TRACE_LEVEL_ERROR, string("EldConfigManagedObject: default config"));
                }
                delete m;
            } else {
                trace(TRACE_LEVEL_DEBUG, string("EldConfigManagedObject: no config present "));
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        }
	}

    void NsmGlobalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();
        
        trace (TRACE_LEVEL_INFO, string("LldpLocalObjectManager::postboot Entered with pass:") + pWaveAsynchronousContextForPostBootPhase->getPassNum());

        switch (PassNum)
        {
            case 0:
                trace (TRACE_LEVEL_DEVEL, "ConfigReplayEldGlobalScalars:: Replaying global config");
                ConfigReplayEldGlobalScalars();
                break;
            default:
                break;
        } 
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }
#endif
/* ELD Global config replay */

      void NsmGlobalObjectManager::printToNoscli(WaveClientSessionContext &waveClientSessionContext, const string &printstring) 
      {
        printfToWaveClientSession (waveClientSessionContext, "%s", printstring.c_str());
        printfToWaveClientSessionOver (waveClientSessionContext);
      }
}
