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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFLocalConfWorker.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "OSPF/Local/OSPFConfigLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFRouterConfigMessage.h"
#include "ClientInterface/OSPF/OSPFInterfaceConfigMessage.h"
#include "ClientInterface/OSPF/OSPFPermitDenyRedistributeMessage.h"
#include "OSPF/Local/OSPFAreaRangeLocalManagedObject.h"
#include "OSPF/Local/OSPFAreaLocalManagedObject.h"
#include "OSPF/Local/OSPFPermitRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFDenyRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFVirtualLinkLocalManagedObject.h"
#include "Utils/Layer3LicenseUtil.h"
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIds.h"
#include "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    OSPFLocalObjectManager::OSPFLocalObjectManager ()
        : MultiPartitionCleanup  (getClassName (),getObjectToDelete())
    {
        associateWithVirtualWaveObjectManager (L3NodeSpecificGlobalObjectManager::getInstance());
        m_pOSPFLocalConfWorker = new OSPFLocalConfWorker (this);
        prismAssert (NULL != m_pOSPFLocalConfWorker, __FILE__, __LINE__);
    }

    OSPFLocalObjectManager::~OSPFLocalObjectManager ()
    {
    }

    OSPFLocalObjectManager  *OSPFLocalObjectManager::getInstance()
    {
        static OSPFLocalObjectManager *pOSPFLocalObjectManager = new OSPFLocalObjectManager ();

        WaveNs::prismAssert (NULL != pOSPFLocalObjectManager, __FILE__, __LINE__);

        return (pOSPFLocalObjectManager);
    }

    string  OSPFLocalObjectManager::getClassName()
    {
        return ("OSPFLocal");
    }

    PrismServiceId  OSPFLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *OSPFLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *OSPFLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	void OSPFLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
		string passName  = pWaveAsynchronousContextForPostBootPhase->getPassName();
		LocationId locId = FrameworkToolKit::getThisLocationId();

		trace (TRACE_LEVEL_INFO, string("OSPFLocalObjectManager:: OSPF postboot entered with pass:") + passName);

        // Check for Layer3 license agaiin before sending to backend
        if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
            trace (TRACE_LEVEL_ERROR, "OSPFLocalObjectManager::postboot. LAYER_3_LICENSE not present");
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }
		if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE5") == 0) {

			trace (TRACE_LEVEL_INFO, string("OSPFLocalObjectManager:: OSPF postboot global config"));
			configReplayGlobalConfig(locId);
			configReplayVlanIntfConfig(locId);
			configReplayLoopbackIntfConfig(locId);
			if (WAVE_PERSISTENCE_CLUSTER_ENABLED !=
				DcmToolKit::isVcsEnabled()) {
				configReplayPoIntfConfig(locId);
			}
		} else if (passName.compare("DCM_POSTBOOT_SLOT_STAGE3") == 0) {
			trace (TRACE_LEVEL_INFO, string("OSPFLocalObjectManager:: OSPF postboot interface config"));
			SI32 slotNum = pWaveAsynchronousContextForPostBootPhase->getSlotNum();
			configReplayPhyIntfConfig(slotNum, locId);
		}

		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback ();
		trace (TRACE_LEVEL_INFO, string("OSPFLocalObjectManager:: OSPF postboot complete"));
	}

	void OSPFLocalObjectManager::configReplayGlobalConfig(LocationId locId)
	{
		OSPFConfigLocalManagedObject *pOSPFConfigLocalMO = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFConfigLocalManagedObject::getClassName());
		vector<WaveManagedObject * >*            pResults = querySynchronouslyLocalManagedObjectsForLocationId(locId, &syncQueryCtxt);

		if ((NULL != pResults) && (pResults -> size() > 0)) {
			UI32 ospfInstanceCount = pResults->size();
			trace (TRACE_LEVEL_INFO, string("Number of OSPF Global Instances to replay:")+ospfInstanceCount);
			for(unsigned int i = 0; i < ospfInstanceCount; i++) {
				OSPFRouterConfigMessage *pOSPFRouterMsg = new OSPFRouterConfigMessage();
				pOSPFConfigLocalMO = dynamic_cast<OSPFConfigLocalManagedObject *>((*pResults)[i]);
				string vrfName = pOSPFConfigLocalMO->getVRF();
				trace (TRACE_LEVEL_INFO, string("Replaying OSPF Router Config for the VRF :")+vrfName);
				/*
				 * Fill up the message from MO
				 */
				pOSPFConfigLocalMO->updateClientMessageForReplay(pOSPFRouterMsg);
				sendSynchronouslyToWaveClient("ospf", pOSPFRouterMsg);
				delete pOSPFRouterMsg;

				/*
				 * Send Permit Messages
				 */
				vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> > permitList = pOSPFConfigLocalMO->getPermitRedistribute();
				for (UI32 idx =0; idx < permitList.size(); idx++) {
					OSPFPermitDenyRedistributeMessage *pPermitMsg = new OSPFPermitDenyRedistributeMessage();
					permitList[idx]->updateClientMessageForReplay(pPermitMsg);
					sendSynchronouslyToWaveClient("ospf", pPermitMsg);
					delete(pPermitMsg);
				}

				/*
				 * Send Deny Messages
				 */
				vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> > denyList = pOSPFConfigLocalMO->getDenyRedistribute();
				for (UI32 idx =0; idx < denyList.size(); idx++) {
					OSPFPermitDenyRedistributeMessage *pDenyMsg = new OSPFPermitDenyRedistributeMessage();
					denyList[idx]->updateClientMessageForReplay(pDenyMsg);
					sendSynchronouslyToWaveClient("ospf", pDenyMsg);
					delete(pDenyMsg);
				}

				/*
				 * Send AREA messages
				 */
				 vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> >  areaList = pOSPFConfigLocalMO->getArea();
				 for (UI32 idx = 0; idx < areaList.size(); idx ++) {
					string areaId = areaList[idx]->getAreaId();
					trace (TRACE_LEVEL_INFO, string("Replaying OSPF Area Config for the VRF :")+vrfName+" for the Area "+areaId);
					OSPFAreaMessage *pAreaMsg = new OSPFAreaMessage();
					pAreaMsg->setVRF(vrfName);
					areaList[idx]->updateClientMessageForReplay(pAreaMsg);
					trace (TRACE_LEVEL_INFO, string("VRF Name in  OSPFAreaMessage:")+pAreaMsg->getVRF());
					ResourceId status = sendSynchronouslyToWaveClient("ospf", pAreaMsg);

					if(status == WAVE_MESSAGE_SUCCESS)
					{
					    status = pAreaMsg->getClientStatus();
				        trace (TRACE_LEVEL_INFO, string("OSPFLocalObjectManager::configReplayGlobalConfig: ospf area config Local message sent to OSPF backend, client status is : ") + FrameworkToolKit::localize(status));
					}
					else {
				        trace (TRACE_LEVEL_ERROR, "OSPFLocalObjectManager::configReplayGlobalConfig: ospf area config Local message not sent to OSPF backend");
					}
					delete pAreaMsg;

					/*
					 * Scan through all virtual Link MO's
					 */
					vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> > virtualLinkList = areaList[idx]->getVirtualLink();
					for (UI32 ii = 0; ii < virtualLinkList.size(); ii ++) {
						OSPFAreaMessage *pVirtualLinkMsg = new OSPFAreaMessage();
						pVirtualLinkMsg->setAreaId(areaId);
						virtualLinkList[ii]->updateClientMessageForReplay(pVirtualLinkMsg);
						pVirtualLinkMsg->setVRF(vrfName);
						sendSynchronouslyToWaveClient("ospf", pVirtualLinkMsg);
						delete pVirtualLinkMsg;
					}

					vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> > areaRangeList = areaList[idx]->getRange();
					for (UI32 ii = 0; ii < areaRangeList.size(); ii ++) {
						OSPFAreaRangeMessage *pAreaRangeMsg = new OSPFAreaRangeMessage();
						pAreaRangeMsg->setAreaId(areaId);
						areaRangeList[ii]->updateClientMessageForReplay(pAreaRangeMsg);
						pAreaRangeMsg->setVRF(vrfName);
						sendSynchronouslyToWaveClient("ospf", pAreaRangeMsg);
						delete pAreaRangeMsg;
					}
				 }
			}
	    }

		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}

	void OSPFLocalObjectManager::configReplayVlanIntfConfig(LocationId locId)
	{
		NsmIntfSviManagedObject *pIntfSviManagedObject = NULL;
		vector<NsmIntfSviManagedObject *> results;
		const NsmL3ReadyManagedObject *pL3ReadyMO;

		NsmL3Util::NodeVlanIfGetMOs(results, locId);
		for(unsigned int i = 0; i < results.size(); i++) {
			pIntfSviManagedObject = results[i];
			pL3ReadyMO = pIntfSviManagedObject->getL3ReadyMO();

			if (pL3ReadyMO) {
				OSPFInterfaceLocalManagedObject *pOSPFIntfMO = (WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject>(pL3ReadyMO->getOSPFIntfConfig())).operator->();
				if (pOSPFIntfMO != NULL) {
					configReplayIntfConfig(pOSPFIntfMO);
				}
			}
			delete pIntfSviManagedObject;
		}
		return ;
	}

	void OSPFLocalObjectManager::configReplayLoopbackIntfConfig(LocationId locId)
	{
	NsmIntfLoopbackManagedObject	*pIntfLoopbackManagedObject = NULL;
		vector<NsmIntfLoopbackManagedObject *> results;
		const NsmL3ReadyManagedObject *pL3ReadyMO;

		NsmL3Util::GetIntfLoopbackMOs(results, locId);
		for(unsigned int i = 0; i < results.size(); i++) {

			pIntfLoopbackManagedObject = results[i];
			pL3ReadyMO = pIntfLoopbackManagedObject->getL3ReadyMO();

			if (pL3ReadyMO) {
				trace (TRACE_LEVEL_INFO, string("Replaying the OSPF config for the loopback interface ")+pIntfLoopbackManagedObject->getIfName());
				OSPFInterfaceLocalManagedObject *pOSPFIntfMO = (WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject>(pL3ReadyMO->getOSPFIntfConfig())).operator->();
				if (pOSPFIntfMO != NULL) {
					configReplayIntfConfig(pOSPFIntfMO);
				}
			}
			delete pIntfLoopbackManagedObject;
		}
		return ;
	}

	void OSPFLocalObjectManager::configReplayPoIntfConfig(LocationId locId)
	{
		SI32 mapId = 0;
		NsmNodePoManagedObject *pNodePoManagedObject = NULL;
		vector<NsmNodePoManagedObject *> results;
		const NsmL3ReadyManagedObject *pL3ReadyMO;

		NsmL3Util::NodePoIfGetMOs(results, mapId);
		for(unsigned int i = 0; i < results.size(); i++) {
			pNodePoManagedObject = results[i];
			pL3ReadyMO = pNodePoManagedObject->getL3ReadyMO();
			if (pL3ReadyMO) {
				OSPFInterfaceLocalManagedObject *pOSPFIntfMO = (WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject>(pL3ReadyMO->getOSPFIntfConfig())).operator->();
				if (pOSPFIntfMO != NULL) {
					configReplayIntfConfig(pOSPFIntfMO);
				}
			}
			delete pNodePoManagedObject;
		}
		return ;
	}

	void OSPFLocalObjectManager::configReplayPhyIntfConfig(SI32 slotId, LocationId locId)
	{
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		vector<PhyIntfLocalManagedObject *> results;
		const NsmL3ReadyManagedObject *pL3ReadyMO;

		NsmUtils::SlotGetPhyIntfMOs(results, slotId, locId);

		for (UI32 i = 0; i < results.size(); i++) {
			pPhyIntfLocalManagedObject = results[i];
			pL3ReadyMO = pPhyIntfLocalManagedObject->getL3ReadyMO();
			if (pL3ReadyMO) {
				OSPFInterfaceLocalManagedObject *pOSPFIntfMO = (WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject>(pL3ReadyMO->getOSPFIntfConfig())).operator->();
				if (pOSPFIntfMO != NULL) {
					configReplayIntfConfig(pOSPFIntfMO);
				}
			}
			delete pPhyIntfLocalManagedObject;
		}
		return ;
	}

	void OSPFLocalObjectManager::configReplayIntfConfig(OSPFInterfaceLocalManagedObject *pOSPFIntfMO)
	{
		trace (TRACE_LEVEL_INFO, "Entering OSPFLocalObjectManager::configReplayIntfConfig ..");

		OSPFInterfaceConfigMessage *msg = new OSPFInterfaceConfigMessage();
		/*
		 * Fill up the message
		 */
		pOSPFIntfMO->updateClientMessageForReplay(msg);
		trace (TRACE_LEVEL_DEVEL, "sending synchronously to ospf client");
		sendSynchronouslyToWaveClient("ospf", msg);
		delete msg;
	}
	void OSPFLocalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
	{
		dce_cluster_validation_data_t *data_p;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		trace(TRACE_LEVEL_DEBUG, "OPSFLocalObjectManager::clusterCreateCollectValidationData:Cluster Collect function called.");
		data_p = dce_cluster_validation_collect_init(OSPF_PLUGIN_LOCAL_VERSION);
		if (data_p == NULL)
		{
			trace(TRACE_LEVEL_ERROR, "OSPFLocalObjectManager::clusterCreateCollectValidationData:Failed to init ValidationData");
			pPrismAsynchronousContext->setCompletionStatus (DCE_CLUSTER_UTIL_ALLOC_FAILED);
			pPrismAsynchronousContext->callback ();
			return;
		}
		status = dce_cluster_validation_collect_done(pMessage, data_p);
		if (status != DCE_CLUSTER_UTIL_SUCCESS)
		{
			pPrismAsynchronousContext->setCompletionStatus (status);
			pPrismAsynchronousContext->callback ();
			return;
		}
		pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pPrismAsynchronousContext->callback ();
		return;
	}
	void OSPFLocalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
	{
		void                            *payload_p;
		dce_cluster_validation_data_t   *data_p;
		UI32                            len;
		ResourceId                      status = WAVE_MESSAGE_SUCCESS;
		uint32_t                        version = 0, num_records = 0;
		uint64_t                        cap_bits = 0;

		trace(TRACE_LEVEL_DEBUG, "OSPFLocalObjectManager::clusterCreateValidate:Cluster Validate function called.");
		pMessage->getValidationDetails(payload_p, len);
		trace(TRACE_LEVEL_DEBUG,string("OSPFLocalObjectManager::clusterCreateValidate:Received Validation Data of LEN=") + len);
		data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

		status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
		if (status != DCE_CLUSTER_UTIL_SUCCESS)
		{
			trace(TRACE_LEVEL_ERROR, string("OSPFLocalObjectManager::clusterCreateValidate:Failed to parse ValidationData Err=") + FrameworkToolKit::localize(status));
			pPrismAsynchronousContext->setCompletionStatus(status);
			pPrismAsynchronousContext->callback ();
			return;
		}
		trace(TRACE_LEVEL_DEBUG, string("OSPFLocalObjectManager::clusterCreateValidate:Recieved version = ") + version);
		if (version != OSPF_PLUGIN_LOCAL_VERSION)
		{
			trace(TRACE_LEVEL_ERROR, string("OSPFLocalObjectManager::clusterCreateValidate:Version mismatch, My-Version=") + OSPF_PLUGIN_LOCAL_VERSION + string(" Rcvd-Version=") + version);
			pPrismAsynchronousContext->setCompletionStatus(NSM_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
			pPrismAsynchronousContext->callback ();
			return;
		}

		pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pPrismAsynchronousContext->callback ();
		return;
	}

	vector<string> OSPFLocalObjectManager::getObjectToDelete()
	{
		vector<string> managedClass;
		managedClass.push_back(OSPFConfigLocalManagedObject::getClassName());
		return managedClass;
	}


}
