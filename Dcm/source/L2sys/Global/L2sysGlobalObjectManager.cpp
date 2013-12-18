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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "DcmResourceIdEnums.h"
#include "Utils/DceClusterUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "L2sys/Global/L2sysGlobalObjectManager.h"
#include "L2sys/Global/L2sysAgingTimeoutManagedObject.h"
#include "L2sys/Global/L2sysGlobalStaticMacManagedObject.h"
#include "L2sys/Global/L2sysGlobalUpdateL2sysAgingTimeoutMessage.h"
#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalClearMessage.h"
#include "L2sys/Global/L2sysGlobalTypes.h"
#include "L2sys/Global/MacAddressTableAgingTimeConfigHandler.h"
#include "L2sys/Global/MacAddressTableStaticConfigHandler.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalStaticMacSPMessage.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalClearSPMessage.h"
#include "L2sys/Local/L2sysUpdateL2sysAgingTimeoutSPMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/FortyGigLocalManagedObject.h"
#include "Nsm/Local/OneGigLocalManagedObject.h"
#include "Nsm/Local/TenGigLocalManagedObject.h"
#include "DcmCore/DcmToolKit.h"

//PVLAN
#include "Nsm/Common/PVlanUtils.h"

//#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "ClientInterface/Nsm/NsmUtils.h"


#define INTERFACE_TYPE_GIGAETHERNET 1
#define INTERFACE_TYPE_TENGIGAETHERNET 2
#define INTERFACE_TYPE_FORTYGIGAETHERNET 3
#define INTERFACE_TYPE_PORTCHANNEL 4

namespace DcmNs
{

    L2sysGlobalObjectManager::L2sysGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        L2sysAgingTimeoutManagedObject    L2sysAgingTimeoutManagedObject    (this);
        L2sysGlobalStaticMacManagedObject    L2sysGlobalStaticMacManagedObject    (this);

        m_pMacAddressTableStaticConfigHandler = new MacAddressTableStaticConfigHandler (this);
        prismAssert (NULL != m_pMacAddressTableStaticConfigHandler, __FILE__, __LINE__);

        m_pMacAddressTableAgingTimeConfigHandler = new MacAddressTableAgingTimeConfigHandler (this);
        prismAssert (NULL != m_pMacAddressTableAgingTimeConfigHandler, __FILE__, __LINE__);

        L2sysAgingTimeoutManagedObject.setupOrm ();
        addManagedClass (L2sysAgingTimeoutManagedObject::getClassName ());
        L2sysGlobalStaticMacManagedObject.setupOrm ();
        addManagedClass (L2sysGlobalStaticMacManagedObject::getClassName ());
        addOperationMap (L2SYSGLOBALUPDATEL2SYSAGINGTIMEOUT, reinterpret_cast<PrismMessageHandler> (&L2sysGlobalObjectManager::L2sysGlobalUpdateL2sysAgingTimeoutMessageHandler));
        addOperationMap (L2SYSGLOBALUPDATEL2SYSGLOBALSTATICMAC, reinterpret_cast<PrismMessageHandler> (&L2sysGlobalObjectManager::L2sysGlobalUpdateL2sysGlobalStaticMacMessageHandler));
        addOperationMap (L2SYSGLOBALUPDATEL2SYSGLOBALCLEAR, reinterpret_cast<PrismMessageHandler> (&L2sysGlobalObjectManager::L2sysGlobalUpdateL2sysGlobalClearMessageHandler));
    }

    L2sysGlobalObjectManager::~L2sysGlobalObjectManager ()
    {
    }

    L2sysGlobalObjectManager  *L2sysGlobalObjectManager::getInstance()
    {
        static L2sysGlobalObjectManager *pL2sysGlobalObjectManager = new L2sysGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pL2sysGlobalObjectManager, __FILE__, __LINE__);

        return (pL2sysGlobalObjectManager);
    }

    string  L2sysGlobalObjectManager::getClassName()
    {
        return ("L2sysGlobal");
    }

    PrismServiceId  L2sysGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *L2sysGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case L2SYSGLOBALUPDATEL2SYSAGINGTIMEOUT :
                pPrismMessage = new L2sysGlobalUpdateL2sysAgingTimeoutMessage ();
                break;
            case L2SYSGLOBALUPDATEL2SYSGLOBALSTATICMAC :
                pPrismMessage = new L2sysGlobalUpdateL2sysGlobalStaticMacMessage ();
                break;
            case L2SYSGLOBALUPDATEL2SYSGLOBALCLEAR :
                pPrismMessage = new L2sysGlobalUpdateL2sysGlobalClearMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *L2sysGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((L2sysAgingTimeoutManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new L2sysAgingTimeoutManagedObject(this);
        }
        else if ((L2sysGlobalStaticMacManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new L2sysGlobalStaticMacManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "L2sysGlobalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void L2sysGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, string("Cluster Collect function called"));

        data_p = dce_cluster_validation_collect_init(L2SYS_PLUGIN_GLOBAL_VERSION);

        if (data_p == NULL) {
            trace(TRACE_LEVEL_ERROR, string("Failed to init ValidationData"));
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

	void L2sysGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;

        trace(TRACE_LEVEL_DEBUG, string("Cluster Validate function called"));

        pMessage->getValidationDetails(payload_p, len);

        trace(TRACE_LEVEL_DEBUG,string("Received Validation Data of LEN=") + len);

        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            trace(TRACE_LEVEL_ERROR, string("Failed to parse ValidationData Err=") + FrameworkToolKit::localize(status));
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        trace(TRACE_LEVEL_DEBUG, string("Recieved version = ") + version);

        if (version != L2SYS_PLUGIN_GLOBAL_VERSION) {
            trace(TRACE_LEVEL_ERROR, string("Version mismatch, My-Version=") + L2SYS_PLUGIN_GLOBAL_VERSION + string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(WRC_L2SYS_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }

    void L2sysGlobalObjectManager::updateL2sysAging (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (pPrismLinearSequencerContext->getPPrismMessage () == NULL) {
			trace (TRACE_LEVEL_DEBUG, "getPPrismMessage () is returning NULL; returning...\n");
			return;
		}

        L2sysGlobalUpdateL2sysAgingTimeoutMessage *pGlobalMessage = dynamic_cast<L2sysGlobalUpdateL2sysAgingTimeoutMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *pWaveSendToClusterContext;
		if (pGlobalMessage == NULL){
			trace (TRACE_LEVEL_INFO, "pGlobalMessage is NULL; returning.....");
			return;
		}

        /* create clustercontext and initialize with callback func */
        pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&L2sysGlobalObjectManager::L2sysGlobalAgingConfigSendToClusterCallback), pPrismLinearSequencerContext);

        trace (TRACE_LEVEL_DEBUG, "L2sysGlobalObjectManager::updateL2sysAging: Enter \n");

        L2sysUpdateL2sysAgingTimeoutSPMessage  localMessage;
		localMessage.setAgingTimeout(pGlobalMessage->getAgingTimeout());

        /* Set Phase 1 message */
        pWaveSendToClusterContext->setPPrismMessageForPhase1(&localMessage);
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);
        /* Send the message to all nodes in cluster */
        sendToWaveCluster (pWaveSendToClusterContext);
        trace (TRACE_LEVEL_DEBUG, "L2sysGlobalObjectManager::updateL2sysAging: status "+FrameworkToolKit::localize(status));
        return;
    }

    void L2sysGlobalObjectManager::updateL2sysAgingMO (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		L2sysAgingTimeoutManagedObject *pMo = NULL;
		int markToDelete = 0;
		if((pPrismLinearSequencerContext->getPPrismMessage ()) == NULL){
			trace (TRACE_LEVEL_DEBUG, "getPPrismMessage () is returning NULL; returning...\n");
			return;
		}
        L2sysGlobalUpdateL2sysAgingTimeoutMessage *pGlobalMessage = dynamic_cast<L2sysGlobalUpdateL2sysAgingTimeoutMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		if(pGlobalMessage == NULL) { 
			trace (TRACE_LEVEL_DEBUG, "pGlobalMessage is NULL; returning...\n");
			return;
		}

        trace (TRACE_LEVEL_DEBUG, "L2sysGlobalObjectManager::updateL2sysAgingMO: Enter \n");

		UI32 agingTime = pGlobalMessage->getAgingTimeout();

        vector<WaveManagedObject *> *pResults = querySynchronously(L2sysAgingTimeoutManagedObject::getClassName());
        if (pResults && pResults->size() > 0) {
            pMo = dynamic_cast<L2sysAgingTimeoutManagedObject*>((*pResults)[0]);
        }

		if (!pMo) {
			if (getPWaveObjectManager() == NULL) {
				  trace (TRACE_LEVEL_INFO, "getPWaveObjectManager() is returning NULL; returning.....");
				if (NULL != pResults) {
					delete pResults;
				}
				return;
			}
			pMo = new L2sysAgingTimeoutManagedObject(dynamic_cast<L2sysGlobalObjectManager *>(getPWaveObjectManager()));
			markToDelete = 1;
			if (pMo == NULL) {
				  trace (TRACE_LEVEL_INFO, "pMo is NULL; returning.....");
				if (NULL != pResults) {
					delete pResults;
				}
				return;
			}
		} else {
			updateWaveManagedObject(pMo);
		}
		pMo->setAgingTimeout(agingTime);

        if (NULL != pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        if (markToDelete) { 
          pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
        }

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS); 
		return;
	}

	void
    L2sysGlobalObjectManager::L2sysGlobalAgingConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace (TRACE_LEVEL_DEVEL, "L2sysGlobalAgingConfigSendToClusterCallback::Entering");
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
        PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
		
        if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message succeeded on all locations");
//      else if(pPrismLinearSequencerContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
//          trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on all locations");
        else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes");
        else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes");
        else
            trace (TRACE_LEVEL_DEVEL, string("getCompletionStatus:: Unknown Error : ") + sendToClusterCompletionStatus);

		LocationId locationid;
		NsmUtils::ClusterGetFirstFailureLocation(pWaveSendToClusterContext, locationid);
		sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationid);
		trace (TRACE_LEVEL_DEVEL, string("getCompletionStatus: ") + sendToClusterCompletionStatus);

		pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
        trace (TRACE_LEVEL_DEVEL, "L2sysGlobalAgingConfigSendToClusterCallback::Done");
        delete pWaveSendToClusterContext;
        return;
     }

    LocationId
    L2sysGlobalObjectManager::L2sysGlobalGetDestLocationId(PrismLinearSequencerContext *pPrismLinearSequencerContext, L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage)
    {
/*      Function to determine the location id to send the Message Object to

        Set location id to local node.
        If MAC address is assigned to port-channel,
            Query database to determine if port-channel exists.
            If port-channel exists, 
                Query database to determine if physical ports assigned to it.
                If at least one port is assigned to port-channel,
                    Set location id to node of first port.
                Else
                    Set location id to node which owns the physical port.
        Return location id.
*/
        LocationId locationId;
	     UI32 mappedId;

        trace (TRACE_LEVEL_DEVEL, "L2sysGlobalGetDestLocationId::Entering");
        mappedId = DcmToolKit::getLocalMappedId();
        locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);

				if (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getInterface_type() == INTERFACE_TYPE_PORTCHANNEL) {
					PoIntfManagedObject *pPoIntfManagedObject = NULL;
					UI32 po_id;
					ObjectId poObjectId;
					PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

					WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
					po_id = strtoul(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname().c_str(), NULL, 0);
					poSyncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
					vector<WaveManagedObject *> *poResults = querySynchronously(&poSyncQueryCtxt);

					if (poResults != NULL && poResults->size() > 0) {
							pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*poResults);
							pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*poResults)[0]);

							if (pPoIntfManagedObject != NULL) {	/* Port channel MO exists */
								poObjectId =  pPoIntfManagedObject->getObjectId();
								WaveManagedObjectSynchronousQueryContext phySyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
								phySyncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
								vector<WaveManagedObject *> *pResults = querySynchronously(&phySyncQueryCtxt);

								if (pResults != NULL && pResults->size() > 0) {
									pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
									pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);

                                	if (pPhyIntfLocalManagedObject != NULL) {
										mappedId = (UI32) NsmUtils::getMappedIdFromIfName(pPhyIntfLocalManagedObject->getIfName());
										locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
									}

									pResults->clear();
									delete pResults;
								}
							}

						poResults->clear();
						delete poResults;
					}
				} else {
					mappedId = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getRbridgeid();
					locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
				}

        return (locationId);
    }

    void  L2sysGlobalObjectManager::L2sysGlobalUpdateL2sysAgingTimeoutMessageHandler( L2sysGlobalUpdateL2sysAgingTimeoutMessage *pL2sysGlobalUpdateL2sysAgingTimeoutMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::updateL2sysAging),
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::updateL2sysAgingMO),

            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pL2sysGlobalUpdateL2sysAgingTimeoutMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start();
    }

	void L2sysGlobalObjectManager::L2sysUpdateMacMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
	         trace (TRACE_LEVEL_INFO, "Entering L2sysUpdateMacMOStep........");

			  L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage = dynamic_cast<L2sysGlobalUpdateL2sysGlobalStaticMacMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

		if(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage == NULL) {
			trace (TRACE_LEVEL_INFO, "pL2sysGlobalUpdateL2sysGlobalStaticMacMessage is NULL; returning.....");
			return;
		}

        string Mac = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getMac();
        unsigned int forward = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getForward();
        unsigned int type = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getInterface_type();
        string ifname = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname();
        unsigned int vlan = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlan();
        unsigned int vlan_id = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid();

        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(L2sysGlobalStaticMacManagedObject::getClassName());


		if(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode() == L2SYS_NO_LINECARD){
			L2sysGlobalStaticMacManagedObject *pL2sysGlobalStaticMacManagedObject = NULL;
			vector<L2sysGlobalStaticMacManagedObject *> results;
			vector<WaveManagedObject *> *pMacResults;


			unsigned int slotId = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getSlotid();
			trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::L2sysUpdateMacMOStep:Message from Local ObjectManager for deletion of Global MO with slotid " + slotId);
			
			syncQueryCtxt.addAndAttribute (new AttributeUI32(&slotId,"slotid"));
			pMacResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

			if (NULL != pMacResults) {
				UI32 numberOfResults = pMacResults->size ();
				for (UI32 i = 0; i < numberOfResults; ++i) {

					pL2sysGlobalStaticMacManagedObject = dynamic_cast<L2sysGlobalStaticMacManagedObject *> ((*pMacResults)[i]);
					trace (TRACE_LEVEL_INFO,  " Removing Mac: " +
						pL2sysGlobalStaticMacManagedObject->getMac());

					delete pL2sysGlobalStaticMacManagedObject;

				}
				delete pMacResults;
			}
			else
				trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::L2sysUpdateMacMOStep:pMacResults is NULL ");

			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return;
		}


        if (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode() == L2SYS_DELETE_OP) {
            trace(TRACE_LEVEL_DEBUG, "L2sysUpdateMacMOStep DELETE Message........");

            syncQueryCtxt.addAndAttribute(new AttributeString(&Mac, "Mac"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE);
            syncQueryCtxt.addAndAttribute(new AttributeUI32(&forward, "forward"));
            syncQueryCtxt.addAndAttribute(new AttributeUI32(&type, "interface_type"));
            syncQueryCtxt.addAndAttribute(new AttributeString(&ifname, "ifname"));
            syncQueryCtxt.addAndAttribute(new AttributeUI32(&vlan, "vlan"));
            syncQueryCtxt.addAndAttribute(new AttributeUI32(&vlan_id, "vlanid"));
            vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
            L2sysGlobalStaticMacManagedObject *pL2sysGlobalStaticMacManagedObject = NULL;

            if (NULL != pResults) {
                UI32 numberOfResults = pResults->size();

                if (1 == numberOfResults) {
                    pL2sysGlobalStaticMacManagedObject = dynamic_cast<L2sysGlobalStaticMacManagedObject *>((*pResults)[0]);
                } else if (numberOfResults > 1) {
					/*
					 * This is an invalid case... but...
					 * We may reach here because till nos4.0.0 it is allowed to enter MAC address with lower case letters
					 * and same MAC address with upper case letters. For example strings "AAAA.BBBB.CCCC" and "aaaa.bbbb.cccc"
					 * are treated as 2 different MAC address and have 2 different enteries in db. This is wrong and fixed now
					 * from nos4.1.0 onwards and above 2 strings are treated as same MAC address and stored in db in lower
					 * case letters. But if someone has nos4.0.0 installed earlier and having these kind of entries present
					 * in db then this is the only way to delete them in nos4.1.0 (after firmware download) as it converts all
					 * the upper case letters to lower case letters before reaching here.
					 */
                    trace(TRACE_LEVEL_WARN, string("L2sysGlobalObjectManager::L2sysUpdateMacMOStep: Invalid MO count = ")
                                    + numberOfResults);

					for (UI32 i = 0; i < numberOfResults; i++) {
						delete ((*pResults)[i]);
					}
                    delete pResults;

                    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                    return;
                }

                delete pResults;
            }

            if (pL2sysGlobalStaticMacManagedObject == NULL) {
                trace(TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::L2sysUpdateMacMOStep: DELETE Message. There are no Objects.");
            } else {
                delete pL2sysGlobalStaticMacManagedObject;
                trace(TRACE_LEVEL_DEBUG, "L2sysGlobalObjectManager::L2sysUpdateMacMOStep: DELETE Message. MO deleted.");
            }

            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        if (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode() == L2SYS_SET_OP) {
            // We should do a pre-existence check here, if there is a possibility for getting a create
            // message to this point without having done a pre-existence before.
            // But currently, pre-existence check is not required, as reach to this step only after validate step
            // has passed, and validate steps already does a pre-existence check.

            trace(TRACE_LEVEL_DEBUG, "L2sysGlobalObjectManager::L2sysUpdateMacMOStep: CREATE Message.");

            L2sysGlobalStaticMacManagedObject *pL2sysGlobalStaticMacManagedObject =
                    new L2sysGlobalStaticMacManagedObject( dynamic_cast<L2sysGlobalObjectManager *>(getPWaveObjectManager()));

            prismAssert(NULL != pL2sysGlobalStaticMacManagedObject, __FILE__, __LINE__);

            pL2sysGlobalStaticMacManagedObject->setMac(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getMac());
            pL2sysGlobalStaticMacManagedObject->setForward(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getForward());
            pL2sysGlobalStaticMacManagedObject->setInterface_type(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getInterface_type());
            pL2sysGlobalStaticMacManagedObject->setIfname(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname());
            pL2sysGlobalStaticMacManagedObject->setVlan(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlan());
            pL2sysGlobalStaticMacManagedObject->setVlanid(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid());

            int32_t slotId = NsmUtils::getSlotId(ifname);
            pL2sysGlobalStaticMacManagedObject->setSlotid(slotId);

            trace(TRACE_LEVEL_DEBUG, "L2sysGlobalObjectManager::L2sysUpdateMacMOStep: Leaving L2sysUpdateMacMOStep.");

            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pL2sysGlobalStaticMacManagedObject);

            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
	}

	void L2sysGlobalObjectManager::L2sysUpdateMac(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
	     LocationId locationId = 0;
	     trace (TRACE_LEVEL_INFO, "Entering L2sysUpdateMac........");

	  	 L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage = dynamic_cast<L2sysGlobalUpdateL2sysGlobalStaticMacMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
//		int status = 0;
		if (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage == NULL) {
			trace (TRACE_LEVEL_INFO, "pL2sysGlobalUpdateL2sysGlobalStaticMacMessage is NULL; returning.....");
			return;
		}


		if (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode() == L2SYS_DELETE_OP) {

		  trace (TRACE_LEVEL_INFO, "Entering L2sysUpdateMac DELETE Message........");
		} else {

		trace (TRACE_LEVEL_INFO, "Entering L2sysUpdateMac CREATE Message........");
		}

		if(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode() == L2SYS_NO_LINECARD){
			trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::L2sysUpdateMac: Message from Local ObjectManager for slotFailover().So returning ");

			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return;
		}

		trace (TRACE_LEVEL_INFO, string(" L2sys -global object mgr -  port channel id  ") + pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getPortChannelId());
		
        L2sysUpdateL2sysGlobalStaticMacSPMessage *m = new L2sysUpdateL2sysGlobalStaticMacSPMessage(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getRbridgeid(),pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getMac(),pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getForward(),pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getInterface_type(),pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname(),0, pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlan(), pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid(), pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode(), pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getPortChannelId());

		/*	If VCS is enabled,
				If creating MAC address,
					If unicast MAC address,
						Query database to determine if MAC address already exists for the vlan.
						If MAC address already exists for same vlan,
							If same interface name,
								Set status to L2SYS_SUCCESS.
							Else,
								Set status to L2SYS_DUPLICATE_MAC error.
					Call L2sysGlobalGetDestLocationId to get destination node for message.
				Else if deleting MAC address,
					Call L2sysGlobalGetDestLocationId to get destination node for message.
				Set up location id by calling send_ctx_p->setLocationsToSendToForPhase1. */

        ResourceId status = WAVE_MESSAGE_SUCCESS;

		if(PVlanUtils::isSecondaryVlan(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid())){
			status = WRC_NSM_PVLAN_SEC_VLAN;
			trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager PVlanUtils::isSecondaryVlan");
			pPrismLinearSequencerContext->executeNextStep(status);
			return;
		}
		if(NsmUtils::isRemoteSpanVlan(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid())) {
			status = WRC_SPAN_CONFIG_NOT_ALLOWED_ON_RSPAN_VLAN;
			NSM_PLUG_DBG_ERR_S(string("Configuration not allowed becuase the vlan ")+pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid()+" is a remote-span vlan");
			pPrismLinearSequencerContext->executeNextStep(status);
			return;
		}

		string Mac = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getMac();
		char mac_string[17];
		unsigned int mac_value;
		int i, j, multicast_flag;

		/* Determine if multicast MAC address */
		strncpy(mac_string, Mac.c_str(),16);

		for (i = 0; i < 5; i++) {
			if (mac_string[i] == '.') {
				break;
			}
		}

		mac_value = 0;

		for (j = 0; j < i; j++) {
			if (isxdigit(mac_string[j])  == 0) {	/* Not hex character.  This should never happen */
				trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::L2sysUpdateMac: not hex char in MAC address; returning.....");
				status = WRC_L2SYS_INVALID_ADDRESS;
				pPrismLinearSequencerContext->executeNextStep(status);
				return;
			}

			mac_value <<= 4;

			if (mac_string[j] <= '9') {
				mac_value += (mac_string[j] - '0');
			} else if (mac_string[j] >= 'a') {
				mac_value += (mac_string[j] - 'a' + 10);
			} else {
				mac_value += (mac_string[j] - 'A' + 10);
			}
		}

		multicast_flag = mac_value & 0x100;

		if (multicast_flag) { 
			trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::L2sysUpdateMac: Multicast MAC address is not allowed in static MAC CLI; returning....."); 
			status = WRC_L2SYS_MCAST_MAC_ADDRESS_NOT_ALLOWED; 
			pPrismLinearSequencerContext->executeNextStep(status); 
			return;
		} 

        /* create clustercontext and initialize with callback func */
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this,
			reinterpret_cast<PrismAsynchronousCallback>(&L2sysGlobalObjectManager::L2sysGlobalProfileConfigSendToClusterCallback), pPrismLinearSequencerContext);

		if(WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled ()) {
			if (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode() == L2SYS_SET_OP) {
				unsigned int vlan_id = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid();
				unsigned int forward = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getForward();
				unsigned int iftype = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getInterface_type();
				string ifName = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname();

				if (multicast_flag == 0) {	/* Unicast address */
					WaveManagedObjectSynchronousQueryContext syncQueryCtxt(L2sysGlobalStaticMacManagedObject::getClassName());
					syncQueryCtxt.addAndAttribute (new AttributeString(&Mac,"Mac"));
					syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlan_id,"vlanid"));
					syncQueryCtxt.addAndAttribute (new AttributeUI32(&forward,"forward"));
					syncQueryCtxt.addAndAttribute (new AttributeUI32(&iftype,"interface_type"));
					syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));
					vector<WaveManagedObject *> *pMacResults = querySynchronously(&syncQueryCtxt);
					L2sysGlobalStaticMacManagedObject *pL2sysGlobalStaticMacManagedObject = NULL;

					if (pMacResults != NULL) {
						if (pMacResults->size() > 0) {
							pL2sysGlobalStaticMacManagedObject = dynamic_cast<L2sysGlobalStaticMacManagedObject *>((*pMacResults)[0]);

							if (pL2sysGlobalStaticMacManagedObject != NULL) {	/* Static MAC address MO exists */
								if (pL2sysGlobalStaticMacManagedObject->getIfname().compare(pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname()) == 0) {
									trace (TRACE_LEVEL_INFO, "L2sysUpdateMac: CREATE MAC interface already configured for this MAC address");
									status = WAVE_MESSAGE_SUCCESS;
								} else {
									trace (TRACE_LEVEL_INFO, "L2sysUpdateMac: CREATE MAC MAC address already configured for different interface");
									status = WRC_L2SYS_DUPLICATE_MAC_ADDRESS;
								}

								pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pL2sysGlobalStaticMacManagedObject);
								delete pMacResults;
								delete pWaveSendToClusterContext;
								pPrismLinearSequencerContext->executeNextStep(status);
								return;
							}
						}

						delete pMacResults;
					}
				}

				locationId = L2sysGlobalGetDestLocationId(pPrismLinearSequencerContext, pL2sysGlobalUpdateL2sysGlobalStaticMacMessage);
			} else if (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode() == L2SYS_DELETE_OP) {
				locationId = L2sysGlobalGetDestLocationId(pPrismLinearSequencerContext, pL2sysGlobalUpdateL2sysGlobalStaticMacMessage);
			}

			vector <LocationId> locationIdVector;
			locationIdVector.push_back(locationId);

            trace(TRACE_LEVEL_INFO, string("Send to target location ::") + locationId);
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locationIdVector);
		}

        /* Set Phase 1 message */
        pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);
        /* Send the message to nodes in cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

		trace (TRACE_LEVEL_INFO, "Leaving L2sysUpdateMac SUCCESSSSSSSSSSSSS ........");
        return;
	}

	void
    L2sysGlobalObjectManager::L2sysGlobalProfileConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
		PrismLinearSequencerContext            *seq_ctx_p = NULL;
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
		seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext());

		NSM_PLUG_ASSERT_PTR(seq_ctx_p);

		if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus )
		{
			vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
			UI32 i = 0;

			for (i = 0; i < locations.size(); i++)
			{
				ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(pWaveSendToClusterContext, locations[i]);

				if(WAVE_MESSAGE_SUCCESS != statusPerLocationId)
				{
					trace(TRACE_LEVEL_ERROR, string("L2sysGlobalObjectManager::L2sysGlobalProfileConfigSendToClusterCallback:Response processing: The location: ") + locations[i]);
					trace(TRACE_LEVEL_ERROR, string("L2sysGlobalObjectManager::L2sysGlobalProfileConfigSendToClusterCallback:Returned error: ")  + FrameworkToolKit::localize(statusPerLocationId));
				}
			}
		}
		else
		{
			trace(TRACE_LEVEL_ERROR, string("L2sysGlobalObjectManager::L2sysGlobalProfileConfigSendToClusterCallback:Returned error: ") + FrameworkToolKit::localize(sendToClusterCompletionStatus));
		}

		seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

		delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
		delete pWaveSendToClusterContext;
		return;
    }

    void  L2sysGlobalObjectManager::L2sysGlobalUpdateL2sysGlobalStaticMacMessageHandler( L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            // Validations and Queries should go here
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::validateL2sysUpdateMac),

            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::L2sysUpdateMac),
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::L2sysUpdateMacMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }

    void L2sysGlobalObjectManager::validateL2sysUpdateMac (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {

        trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::validateL2sysUpdateMac called.");

        L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage =
                dynamic_cast<L2sysGlobalUpdateL2sysGlobalStaticMacMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        UI32 opCode = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getOpcode();
        string ifName = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname();
        int ifType = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->m_interface_type;

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if (opCode == L2SYS_SET_OP) {
            if (isMOAlreadyExisting(pPrismLinearSequencerContext, status)) {
                pPrismLinearSequencerContext->executeSuccessStep();
                return;
            }
        }else if (opCode == L2SYS_DELETE_OP) {
            if ( (! isMOAlreadyExisting(pPrismLinearSequencerContext, status)) && (status == WAVE_MESSAGE_SUCCESS) ) {
                pPrismLinearSequencerContext->executeSuccessStep();
                return;
            }
        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            pPrismLinearSequencerContext->executeNextStep(status);
            return;
        }

        status = validateIfName (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->m_interface_type,
                pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->m_ifname);


        if (status == WAVE_MESSAGE_SUCCESS) {
            // Check whether the provided vlanid is really valid.
            int vlanid = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->m_vlanid;
            if (vlanid <= 0 || vlanid > MAX_ALLOWED_VLAN_ID) {
                trace (TRACE_LEVEL_WARN, "L2sysGlobalObjectManager::validateL2sysUpdateMac: Invalid vlanid = " + vlanid);

                status = L2SYS_VLAN_RANGE_ERROR;
            }
        }

        /*
         * Static MAC address can not be configured on an ISL port.
        */
        if ((status == WAVE_MESSAGE_SUCCESS) && (ifType == INTERFACE_TYPE_GIGAETHERNET || ifType == INTERFACE_TYPE_TENGIGAETHERNET || ifType == INTERFACE_TYPE_FORTYGIGAETHERNET) &&
            (NsmUtils::isPhyPortInISLMode(ifName, false) == true)) {
            trace (TRACE_LEVEL_ERROR, string("L2sysGlobalObjectManager::validateL2sysUpdateMac: Returning error as port is an ISL port") + ifName);
            status = WRC_L2SYS_STATIC_MAC_ADDRESS_NOT_ALLOWED_ON_ISL_PORT;
        }

        pPrismLinearSequencerContext->executeNextStep(status);
    }


    /**
     * Checks whether the passed interface ("ifName") is existing or not?
     * "interfaceType" - enum identifying whether the interface is of type - OneGig..., TenGig... or FortyGig...
     * "ifName" - interface to check.
     * return "WAVE_MESSAGE_SUCCESS" if interface found. Otherwise returns an error code.
     */
    ResourceId L2sysGlobalObjectManager::validateIfName(const int interfaceType, const string& ifName) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        if (interfaceType == L2SYS_GIGAETHERNET) {
            if (! NsmUtils::isValidInterface(ifName, IF_TYPE_GI) ) {
                status = WRC_NSM_ERR_PHYINTF_INVALID_ONEGIGE;
            }
        } else if (interfaceType == L2SYS_TENGIGAETHERNET) {
            if (! NsmUtils::isValidInterface(ifName, IF_TYPE_TE) ) {
                status = WRC_NSM_ERR_PHYINTF_INVALID_TENGIGE;
            }
        } else if (interfaceType == L2SYS_FORTYGIGAETHERNET) {
            if (! NsmUtils::isValidInterface(ifName, IF_TYPE_FO)) {
                status = WRC_NSM_ERR_PHYINTF_INVALID_FORTYGIGE;
            }
        } else if (interfaceType == L2SYS_PORT_CHANNEL) {
            // For port channel no validity check is required.
            status = WAVE_MESSAGE_SUCCESS;
        } else {
            // We should never reach here.
            trace(TRACE_LEVEL_ERROR, "L2sysGlobalObjectManager::validateIfName: invalid interface = " + ifName);
            status = L2SYS_INVALID_INTERFACE_TYPE_ERROR;
        }

        return status;
    }


    /**
     * Checks whether the MO is existing or not?
     *
     * return "true" if MO found, "false" otherwise or if an error occurs.
     */
	bool L2sysGlobalObjectManager::isMOAlreadyExisting (PrismLinearSequencerContext *pPrismLinearSequencerContext,
	        ResourceId &status)
	{
        trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::isMOAlreadyExisting called.");

        L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage =
                dynamic_cast<L2sysGlobalUpdateL2sysGlobalStaticMacMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        string mac = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getMac();
        unsigned int forward = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getForward();
        unsigned int type = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getInterface_type();
        string ifname = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getIfname();
        unsigned int vlan = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlan();
        unsigned int vlan_id = pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->getVlanid();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(L2sysGlobalStaticMacManagedObject::getClassName());

        syncQueryCtxt.addAndAttribute (new AttributeString(&mac,"Mac"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE);
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&forward,"forward"));
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&type,"interface_type"));
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifname,"ifname"));
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlan,"vlan"));
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlan_id,"vlanid"));

        UI32 count;
        status = querySynchronouslyForCount(&syncQueryCtxt, count);

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_ERROR, "L2sysGlobalObjectManager::isMOAlreadyExisting: query error.");
            return false;
        }

        if (count > 0) {
            trace (TRACE_LEVEL_INFO, "L2sysGlobalObjectManager::isMOAlreadyExisting: Already exists.");
            return true;
        }

        return false;
    }

    void L2sysGlobalObjectManager::L2sysClearMac(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
	     trace (TRACE_LEVEL_INFO, "Entering L2sysClearMac........");

	  	 L2sysGlobalUpdateL2sysGlobalClearMessage *pL2sysGlobalUpdateL2sysGlobalClearMessage = dynamic_cast<L2sysGlobalUpdateL2sysGlobalClearMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
//		int status = 0;
        WaveSendToClusterContext    *pWaveSendToClusterContext;
		if(pL2sysGlobalUpdateL2sysGlobalClearMessage == NULL) { 
			trace (TRACE_LEVEL_INFO, "pL2sysGlobalUpdateL2sysGlobalClearMessage is NULL; returning.....");
			return;
		}

        /* create clustercontext and initialize with callback func */
        pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&L2sysGlobalObjectManager::L2sysGlobalClearSendToClusterCallback), pPrismLinearSequencerContext);

		if (pL2sysGlobalUpdateL2sysGlobalClearMessage->getOpcode() == L2SYS_DELETE_OP) {

		  trace (TRACE_LEVEL_INFO, "Entering L2sysClearMac DELETE Message........");
		} else {

		trace (TRACE_LEVEL_INFO, "Entering L2sysClearMac CREATE Message........2");
		}


		L2sysUpdateL2sysGlobalClearSPMessage *m = new L2sysUpdateL2sysGlobalClearSPMessage(pL2sysGlobalUpdateL2sysGlobalClearMessage->getRbridgeid(),pL2sysGlobalUpdateL2sysGlobalClearMessage->getIfname(),pL2sysGlobalUpdateL2sysGlobalClearMessage->getMac(),pL2sysGlobalUpdateL2sysGlobalClearMessage->getVlanid(),pL2sysGlobalUpdateL2sysGlobalClearMessage->getOpcode(), pL2sysGlobalUpdateL2sysGlobalClearMessage->getOpcode(), pL2sysGlobalUpdateL2sysGlobalClearMessage->getInterfaceType(), pL2sysGlobalUpdateL2sysGlobalClearMessage->getPortChannelId());

        if(m)
        {
            trace(TRACE_LEVEL_INFO,"Got the Local message 1\n");


        /* Set Phase 1 message */
        pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag (true);
        /* Send the message to all nodes in cluster */
        sendToWaveCluster (pWaveSendToClusterContext);
        }else
        {
            trace(TRACE_LEVEL_INFO,"Got NULL Local message\n");
        }

		trace (TRACE_LEVEL_INFO, "Leaving L2sysUpdateMac SUCCESSSSSSSSSSSSS ........");
        return;
	}

	void
    L2sysGlobalObjectManager::L2sysGlobalClearSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace (TRACE_LEVEL_DEVEL, "L2sysGlobalClearSendToClusterCallback::Entering");
        ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
        PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
        if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message succeeded on all locations");
//      else if(pPrismLinearSequencerContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
//          trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on all locations");
        else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes");
        else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes");
        else
            trace (TRACE_LEVEL_DEVEL, string("getCompletionStatus:: Unknown Error : ") + sendToClusterCompletionStatus);

		LocationId locationid;
		NsmUtils::ClusterGetFirstFailureLocation(pWaveSendToClusterContext, locationid);
		sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationid);
		trace (TRACE_LEVEL_DEVEL, string("getCompletionStatus: ") + sendToClusterCompletionStatus);

		pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
		trace (TRACE_LEVEL_DEVEL, "L2sysGlobalClearSendToClusterCallback::Done");
		delete pWaveSendToClusterContext  -> getPPrismMessageForPhase1();
        delete pWaveSendToClusterContext;
		return;
     }

    void  L2sysGlobalObjectManager::L2sysGlobalUpdateL2sysGlobalClearMessageHandler( L2sysGlobalUpdateL2sysGlobalClearMessage *pL2sysGlobalUpdateL2sysGlobalClearMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::L2sysClearMac),
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&L2sysGlobalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pL2sysGlobalUpdateL2sysGlobalClearMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }

    void L2sysGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
        vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_DEVEL, string ("L2sysGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));
        if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
        {
            UI32 noOfFailedLocations = failedLocationIds.size ();

			startTransaction ();
            for (UI32 i = 0; i < noOfFailedLocations; i++)
            {
				char ifExp[16];
				SI32 mappedId = DcmToolKit::getMappedIdFromLocationId(failedLocationIds[i]);
				WaveManagedObjectSynchronousQueryContextForDeletion *deleteCtxt = new WaveManagedObjectSynchronousQueryContextForDeletion (L2sysGlobalStaticMacManagedObject::getClassName());

				snprintf(ifExp, sizeof(ifExp), "%d/%%", mappedId);

				deleteCtxt->addAndAttribute(new AttributeString(string(ifExp), "ifname"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
				deleteMultipleWaveManagedObjects(deleteCtxt);
            } // for (UI32 i = 0; i < noOfFailedLocations; i++)
			status = commitTransaction ();
        }
        pFailoverAsynchronousContext->setCompletionStatus (status);
        pFailoverAsynchronousContext->callback ();
    }
}

