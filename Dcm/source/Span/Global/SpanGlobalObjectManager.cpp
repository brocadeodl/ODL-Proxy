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

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Span/Global/SpanGlobalObjectManager.h"
#include "Span/Global/SpanGlobalWorker.h"
#include "Span/Global/SpanGlobalSpanGlobalConfigMessage.h"
#include "Span/Global/SpanGlobalConfigManagedObject.h"
#include "Span/Global/SpanGlobalTypes.h"
#include "Span/Local/SpanLocalSpanLocalConfigMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "ClientInterface/Span/SpanMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceClusterUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmCStatus.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Span/NsmLocaldestinationMirrorportMessage.h"

namespace DcmNs
{
	SpanGlobalObjectManager::SpanGlobalObjectManager ()
		 : WaveObjectManager  (getClassName ())
	{
	     m_pSpanGlobalWorker = new SpanGlobalWorker (this);
	     prismAssert (NULL != m_pSpanGlobalWorker, __FILE__, __LINE__);
	}

	SpanGlobalObjectManager::~SpanGlobalObjectManager ()
	{
	}
	
    SpanGlobalObjectManager  *SpanGlobalObjectManager::getInstance()
	{
 	    static SpanGlobalObjectManager *pSpanGlobalObjectManager = new SpanGlobalObjectManager ();
        WaveNs::prismAssert (NULL != pSpanGlobalObjectManager, __FILE__, __LINE__);
        return (pSpanGlobalObjectManager);
    }

	string  SpanGlobalObjectManager::getClassName()
	{
	    return ("SpanGlobal");
	}

	PrismServiceId  SpanGlobalObjectManager::getPrismServiceId()
	{
	    return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *SpanGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
	    PrismMessage *pPrismMessage = NULL;
        switch (operationCode)
 	    {
            default :
                pPrismMessage = NULL;
        }
        return (pPrismMessage);
    }

	WaveManagedObject  *SpanGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
	    return NULL;
	}

    ResourceId SpanGlobalObjectManager::SpanGlobalCleanupForLocation (UI32 &location_id,FailoverAsynchronousContext *failoverContext)
    {
        UI32 mapped_id, mappedId_srcIfc,mappedId_destIfc = 0;
        UI32 mappedId_backEndToBeUpdated = 0;
		char db_source_ifName[MAX_TENGIGA_PORT_NUM_LEN];
		char db_dest_ifName[MAX_TENGIGA_PORT_NUM_LEN];
        mapped_id = DcmToolKit::getMappedIdFromLocationId(location_id);
        NSM_PLUG_DBG_S(string("Found Mapped ID = ") + mapped_id);
        
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SpanGlobalConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        vector<WaveManagedObject *>::iterator it;
        SpanGlobalConfigManagedObject  *loc_mo_p;

        startTransaction();

        for (it = pResults->begin(); it != pResults->end(); it++) 
		{
            loc_mo_p = dynamic_cast<SpanGlobalConfigManagedObject *> (*it);
			strncpy(db_source_ifName, (loc_mo_p->getSource_ifName()).c_str(), MAX_TENGIGA_PORT_NUM_LEN);
			strncpy(db_dest_ifName, (loc_mo_p->getDest_ifName()).c_str(), MAX_TENGIGA_PORT_NUM_LEN);
			mappedId_srcIfc = NsmUtils::getMappedIdFromIfName(db_source_ifName);
			mappedId_destIfc = NsmUtils::getMappedIdFromIfName(db_dest_ifName);
			NSM_PLUG_DBG_S(string("Found Mapped ID from source if_Name = ") + mappedId_srcIfc);
			NSM_PLUG_DBG_S(string("Found Mapped ID from destination if_Name = ") + mappedId_destIfc);
			if (mappedId_srcIfc == mapped_id || mappedId_destIfc == mapped_id)
			{
				//In case of Span over VCS, where the source and destination interfaces of the session belong to two different nodes in the cluster;
				//If the source node fails and moves away from the cluster, the 'show command' will show the source as 'None' and the configured destination interface.
				//Similarly if the destination node leaves the cluster and source node stays, then again the show command output won't be proper
				//In both of these scenarios, the 'running config' is updated properly and the session source and destination config will be removed.
				//So we have to try to cleanup the backend of source node (if destination leaves the cluster and source stays) or destination node (if source node leaves and destination stays)
				//If both source and destination interfaces belong to the same node, then the backend cleanup is not required because the node is no longer part of the cluster.
				NSM_PLUG_DBG_S(string("Found Mapped ID match = ") + mapped_id);
				if(mappedId_srcIfc == mappedId_destIfc) {
					NSM_PLUG_DBG_S(string("No need to update the backend because the session config is completely removed. ") );
				}
				else {
					if(mappedId_srcIfc == mapped_id) {
						//We need to cleanup the backend of destination interface node
						//mappedId_backEndToBeUpdated = mappedId_destIfc;
					}
					else if(mappedId_destIfc == mapped_id){
						//We need to cleanup the backend of source interface node
						//mappedId_backEndToBeUpdated = mappedId_srcIfc;
					}
				}

		        if(mappedId_backEndToBeUpdated != 0) {
		        	NSM_PLUG_DBG_S(string("Need to cleanup the backend ") + mappedId_backEndToBeUpdated);
		        	//UI32 backEndLocationId = DcmToolKit::getLocationIdFromMappedId(mappedId_backEndToBeUpdated);

		        	//Cleanup the db the destination interface entry
		        	NsmLocaldestinationMirrorportMessage *pNsmLocaldestinationMirrorportMessage = new NsmLocaldestinationMirrorportMessage();
		        	pNsmLocaldestinationMirrorportMessage->setOpcode(SPAN_DEL_MIRRORED_PORT);
		        	pNsmLocaldestinationMirrorportMessage->setDestifName(loc_mo_p->getDest_ifName());
					ResourceId status = sendSynchronously (pNsmLocaldestinationMirrorportMessage);
					if (status == WAVE_MESSAGE_SUCCESS &&
						pNsmLocaldestinationMirrorportMessage->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
						NSM_PLUG_DBG_S(string("Successfully cleaned up the destination interface"));
					} else {
						NSM_PLUG_DBG_S(string("Some failure occurred during cleaning up the destination config"));
						if (status == WAVE_MESSAGE_SUCCESS) {
							status = pNsmLocaldestinationMirrorportMessage->getCompletionStatus();
							NSM_PLUG_DBG_S(string("Completion Status during cleaning up the destination config:")+status);
						}
					}
					delete pNsmLocaldestinationMirrorportMessage;


					SpanLocalSpanLocalConfigMessage *m = new SpanLocalSpanLocalConfigMessage();
		        	string str_twoTupleSrcIfName = "",str_twoTupleDestIfName = "";
                    SI32 srcMappedId = 0;
                    SI32 dstMappedId = 0;
					 if (NsmUtils::isValidThreeTuple(loc_mo_p->getSource_ifName())) {
					   srcMappedId = NsmUtils::getMappedIdFromIfName(loc_mo_p->getSource_ifName());
					 }
					 if (NsmUtils::isValidThreeTuple(loc_mo_p->getDest_ifName())) {
					   dstMappedId = NsmUtils::getMappedIdFromIfName(loc_mo_p->getDest_ifName());
					 }

		        	// send msg to back-end
		        	m->setSession_id(loc_mo_p->getSession_id());
		        	m->setOpCode(SPAN_MSG_NO_SET);
		        	m->setMessageType(SPAN_LOCAL_MSG_CONFIG);
		        	NsmUtils::getTwoTupleIfName(loc_mo_p->getSource_ifName(), str_twoTupleSrcIfName);
		        	m->setSource_ifName(str_twoTupleSrcIfName);
		        	NsmUtils::getTwoTupleIfName(loc_mo_p->getDest_ifName(), str_twoTupleDestIfName);
		        	m->setDest_ifName(str_twoTupleDestIfName);
		        	m->setDirection(loc_mo_p->getDirection());
		        	m->setSrcethernet(loc_mo_p->getSource_tengiga());
		        	m->setDestethernet(loc_mo_p->getDest_tengiga());
		        	m->setTrillSA(srcMappedId);
		        	m->setTrillDA(dstMappedId);


		        	WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
		    			(&SpanGlobalObjectManager::spanFailoverSendToClusterCallback), failoverContext);
		    		prismAssert ((send_ctx_p) != NULL, __FILE__, __LINE__);
		    		send_ctx_p->setPPrismMessageForPhase1(m);

		    		NSM_PLUG_DBG_S(string("Sending the cleanup message to the wave cluster "));
		    		sendToWaveCluster (send_ctx_p);
		        }

				loc_mo_p->setSource(ENUM_DEFAULT_VALUE);
				loc_mo_p->setSource_tengiga(ENUM_DEFAULT_VALUE);
				loc_mo_p->setSource_ifName("None");
				loc_mo_p->setDestination(ENUM_DEFAULT_VALUE);
				loc_mo_p->setDest_tengiga(ENUM_DEFAULT_VALUE);
				loc_mo_p->setDest_ifName("None");
				loc_mo_p->setDirection(ENUM_DEFAULT_VALUE);
				loc_mo_p->setDestVlanId(0);
				updateWaveManagedObject (loc_mo_p);
				NSM_PLUG_DBG_S(string("Set the port mirroring session to default values for the session=")+loc_mo_p->getSession_id());
			}
        }
        commitTransaction();
		
		if(pResults)
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
        return WAVE_MESSAGE_SUCCESS;
    }

    void SpanGlobalObjectManager::spanFailoverSendToClusterCallback (WaveSendToClusterContext *ctx_p) {
    	NSM_PLUG_DBG_S(string("Entering"));
        ResourceId   sendToClusterCompletionStatus = ctx_p->getCompletionStatus ();
        NSM_PLUG_DBG_S(string("Status:")+FrameworkToolKit::localize (sendToClusterCompletionStatus));
        delete (ctx_p->getPPrismMessageForPhase1 ());
        delete ctx_p;
    }

    ResourceId SpanGlobalObjectManager::SpanGlobalHandleFailoverForLocation (UI32 &location_id,FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        ResourceId  status;
        status = SpanGlobalObjectManager::SpanGlobalCleanupForLocation(location_id,pFailoverAsynchronousContext);
        return status;
    }

    void SpanGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
        vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

        vector<LocationId>::iterator  it;

        NSM_PLUG_DBG_S(string("Rcvd Failover with Reason=") + FrameworkToolKit::localize(failoverReason));

        if (failoverReason == FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED) 
		{
            NSM_PLUG_DBG("Uncontrolled failover rcvd.. Returning");
            pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pFailoverAsynchronousContext->callback ();
            return;
        }
		
        for (it = failedLocationIds.begin(); it != failedLocationIds.end(); it++) 
		{
            NSM_PLUG_DBG_S(string("Failed Location = ") + *it);
            SpanGlobalObjectManager::SpanGlobalHandleFailoverForLocation(*it,pFailoverAsynchronousContext);
        }

        pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pFailoverAsynchronousContext->callback ();
    }

	void SpanGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
	{
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WRC_WAVE_MESSAGE_SUCCESS;

        NSM_PLUG_DBG("Cluster Collect function called.");

        data_p = dce_cluster_validation_collect_init(SPAN_PLUGIN_GLOBAL_VERSION);
        if (data_p == NULL)
        {
            NSM_PLUG_DBG_ERR("Failed to init ValidationData");
            pPrismAsynchronousContext->setCompletionStatus (WRC_DCE_CLUSTER_UTIL_ALLOC_FAILED);
            pPrismAsynchronousContext->callback ();
            return;
        }

        status = dce_cluster_validation_collect_done(pMessage, data_p);
        if (status != WRC_DCE_CLUSTER_UTIL_SUCCESS)
        {
            pPrismAsynchronousContext->setCompletionStatus (status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WRC_WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;

	}

	void SpanGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
	{
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WRC_WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;

        NSM_PLUG_DBG("Cluster Validate function called.");
        pMessage->getValidationDetails(payload_p, len);

        NSM_PLUG_DBG_S(string("Received Validation Data of LEN=") + len);

        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != WRC_DCE_CLUSTER_UTIL_SUCCESS)
        {
            NSM_PLUG_DBG_ERR_S(string("Failed to parse ValidationData Err=") +
            FrameworkToolKit::localize(status));
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        NSM_PLUG_DBG_S(string("Recieved version = ") + version);

        if (version != SPAN_PLUGIN_GLOBAL_VERSION)
        {
            NSM_PLUG_DBG_ERR_S(string("Version mismatch, My-Version=") + SPAN_PLUGIN_GLOBAL_VERSION + string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(WRC_NSM_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }
        pPrismAsynchronousContext->setCompletionStatus (WRC_WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;

    }
}

			
			
