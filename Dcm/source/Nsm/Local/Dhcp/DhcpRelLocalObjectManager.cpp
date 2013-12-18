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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "ClientInterface/Dhcp/DhcpRelIntfClientConfigMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Dhcp/DhcpRelInterfaceLocalManagedObject.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalObjectManager.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmResourceIds.h"
#include "Utils/DceClusterUtils.h"

namespace DcmNs
{

    DhcpRelLocalObjectManager::DhcpRelLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
		m_pDhcpLocalShowClrWorker = new DhcpRelLocalShowClrWorker (this);
        prismAssert (NULL != m_pDhcpLocalShowClrWorker, __FILE__, __LINE__);		
    }

    DhcpRelLocalObjectManager::~DhcpRelLocalObjectManager ()
    {
    }

    DhcpRelLocalObjectManager *DhcpRelLocalObjectManager::getInstance()
    {
        static DhcpRelLocalObjectManager *pDhcpRelLocalObjectManager = new DhcpRelLocalObjectManager ();

        WaveNs::prismAssert (NULL != pDhcpRelLocalObjectManager, __FILE__, __LINE__);

        return (pDhcpRelLocalObjectManager);
    }

    string  DhcpRelLocalObjectManager::getClassName()
    {
        return ("Dhcp");
    }

    PrismServiceId  DhcpRelLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *DhcpRelLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DhcpRelLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void DhcpRelLocalObjectManager::replayDhcpRelVeIntfConfig(LocationId locId)
    {
        int size, size2;
        vector<NsmIntfSviManagedObject *> sviMOs;
        const NsmL3ReadyManagedObject* l3ReadyMO = NULL;
        ResourceId status;

        tracePrintf(TRACE_LEVEL_DEBUG, "%s:%d ", __PRETTY_FUNCTION__, __LINE__); 

        // First read the Ve intf MOs from the dB
        NsmL3Util::NodeVlanIfGetMOs(sviMOs, locId);
        size = sviMOs.size();
        if (!size) {
            tracePrintf(TRACE_LEVEL_DEBUG, "%s:%d No SVI MOs found. Returning.", __PRETTY_FUNCTION__, __LINE__); 
            return;
        }

        for (int i = 0; i < size; i++) {
            if ((l3ReadyMO = (sviMOs[i]->getL3ReadyMO())) == NULL)
            {
	        trace(TRACE_LEVEL_DEBUG,string("dhcpRel-L3ReadyMo NULL continuing") );
                delete sviMOs[i]; 
                continue;
            } 
            vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> > dhcpRelIntfMOs  = l3ReadyMO->getDhcpRelIntfConf();
            size2 = dhcpRelIntfMOs.size();
	    	trace(TRACE_LEVEL_DEBUG,string("dhcpRelIntfMOs-size2")+size2 );
            for (int j = 0; j < size2; j++) {
                DhcpRelIntfClientConfigMessage *m2 = new DhcpRelIntfClientConfigMessage();

	        	trace(TRACE_LEVEL_DEBUG,string("dhcpRelIntf: inside for-loop") );
	        	string ifName = dhcpRelIntfMOs[j]->getIfName();
	        	trace(TRACE_LEVEL_DEBUG,string("ifname") +ifName);
	        	trace(TRACE_LEVEL_DEBUG,string("ifType") +dhcpRelIntfMOs[j]->getIfType() );
	        	trace(TRACE_LEVEL_DEBUG,string("vlanId") +dhcpRelIntfMOs[j]->getVlanId() );

                m2->setIfName(ifName);
                m2->setIfType(dhcpRelIntfMOs[j]->getIfType());
                m2->setVlanValue(dhcpRelIntfMOs[j]->getVlanId());
                m2->setServerAddress(dhcpRelIntfMOs[j]->getServerAddress());
                status = sendSynchronouslyToWaveClient("iphelpd",  m2);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__);
                } else {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                                m2->getCompletionStatus());
                }
                delete m2;
            } /* end of inner for-loop */
            delete sviMOs[i];
        } /* end of outer for-loop */
    }
    
    void DhcpRelLocalObjectManager::slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
    {
        pWaveAsynchronousContextForSlotFailover->
            setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForSlotFailover->callback();
    }

	void DhcpRelLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string passName = pWaveAsynchronousContextForPostBootPhase->getPassName();
		LocationId locId = FrameworkToolKit::getThisLocationId();

        tracePrintf(TRACE_LEVEL_INFO, "%s:%d Entered with pass:%s", __PRETTY_FUNCTION__, __LINE__, passName.c_str());
        // Check for Layer3 license before sending to backend
        if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
            tracePrintf(TRACE_LEVEL_INFO, "DhcpRelLocalObjectManager::postboot. LAYER_3_LICENSE not present.", __PRETTY_FUNCTION__, __LINE__);
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

        if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE5") == 0) {
            tracePrintf(TRACE_LEVEL_INFO, "%s:%d DCM_POSTBOOT_GLOBAL_STAGE_5", __PRETTY_FUNCTION__, __LINE__); 
            replayDhcpRelVeIntfConfig(locId);
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

void DhcpRelLocalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
        {
                dce_cluster_validation_data_t *data_p;
                ResourceId status = WAVE_MESSAGE_SUCCESS;

                trace(TRACE_LEVEL_DEBUG, "DhcpRelLocalObjectManager::clusterCreateCollectValidationData:Cluster Collect function called.");
                data_p = dce_cluster_validation_collect_init(DHCPREL_PLUGIN_LOCAL_VERSION);
                if (data_p == NULL)
                {
                        trace(TRACE_LEVEL_ERROR, "DhcpRelLocalObjectManager::clusterCreateCollectValidationData:Failed to init ValidationData");
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

        void DhcpRelLocalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
        {
                void                            *payload_p;
                dce_cluster_validation_data_t   *data_p;
                UI32                            len;
                ResourceId                      status = WAVE_MESSAGE_SUCCESS;
                uint32_t                        version = 0, num_records = 0;
                uint64_t                        cap_bits = 0;

                trace(TRACE_LEVEL_DEBUG, "DhcpRelLocalObjectManager::clusterCreateValidate:Cluster Validate function called.");
                pMessage->getValidationDetails(payload_p, len);
                trace(TRACE_LEVEL_DEBUG,string("DhcpRelLocalObjectManager::clusterCreateValidate:Received Validation Data of LEN=") + len);
                data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

                status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
                if (status != DCE_CLUSTER_UTIL_SUCCESS)
                {
                        trace(TRACE_LEVEL_ERROR, string("DhcpRelLocalObjectManager::clusterCreateValidate:Failed to parse ValidationData Err=") + FrameworkToolKit::localize(status));
                        pPrismAsynchronousContext->setCompletionStatus(status);
                        pPrismAsynchronousContext->callback ();
                        return;
                }
                trace(TRACE_LEVEL_DEBUG, string("DhcpRelLocalObjectManager::clusterCreateValidate:Recieved version = ") + version);
                if (version != DHCPREL_PLUGIN_LOCAL_VERSION)
                {
                        trace(TRACE_LEVEL_ERROR, string("DhcpRelLocalObjectManager::clusterCreateValidate:Version mismatch, My-Version=") + DHCPREL_PLUGIN_LOCAL_VERSION + string(" Rcvd-Version=") + version);
                        pPrismAsynchronousContext->setCompletionStatus(NSM_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
                        pPrismAsynchronousContext->callback ();
                        return;
                }

                pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                pPrismAsynchronousContext->callback ();
                return;
        }

	void DhcpRelLocalObjectManager::replayDhcpRelPhyIntfConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        int  size2;
        const NsmL3ReadyManagedObject* l3ReadyMO ;
        ResourceId status;

        tracePrintf(TRACE_LEVEL_INFO, "%s:%d ", __PRETTY_FUNCTION__, __LINE__); 


        /* l3ReadyMO returned by getL3ReadyMO() is a const ptr. So cast it
         * to non-constant else call to getDhcpRelIntfConf() method doesn't
         * compile. */
        if ((l3ReadyMO = (pPhyIntfLocalManagedObject->getL3ReadyMO())) == NULL)
            return;


        vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> > dhcpRelMOs  = l3ReadyMO->getDhcpRelIntfConf();
        size2 = dhcpRelMOs.size();
        for (int j = 0; j < size2; j++) {
            DhcpRelIntfClientConfigMessage *m2 = new DhcpRelIntfClientConfigMessage();

            /* Pass 2 tuple name to backend if we are in VCS mode */
        	string ifname = "";
        	string intfName = dhcpRelMOs[j]->getIfName();
        	if(NsmUtils::isValidThreeTuple(intfName)) {
           		NsmUtils::getTwoTupleIfName(intfName, ifname);
        	}
        	else {
            	ifname = intfName;
        	}

            m2->setIfName(ifname);
            m2->setIfType(dhcpRelMOs[j]->getIfType());
            m2->setVlanValue(dhcpRelMOs[j]->getVlanId());
            m2->setServerAddress(dhcpRelMOs[j]->getServerAddress());
            status = sendSynchronouslyToWaveClient("iphelpd",  m2);
            if (status == WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_INFO, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__); 
            } else {
                tracePrintf(TRACE_LEVEL_INFO, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                            m2->getCompletionStatus()); 
            }
            delete m2;
        } /* end of for-loop */
    }
	
}

