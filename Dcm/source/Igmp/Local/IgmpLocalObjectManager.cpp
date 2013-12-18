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
 *   Author : agidwani                                                     *
 **************************************************************************/


#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "ClientInterface/Igmps/IgmpsUtils.h"
#include "ClientInterface/Igmp/IgmpMessageDef.h"
#include "ClientInterface/Igmp/IgmpClientPhyIntfLocalConfigMessage.h"
#include "ClientInterface/Igmp/IgmpClientPoSviIntfLocalConfigMessage.h"
#include "ClientInterface/Igmp/IgmpClientSgIntfLocalConfigMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Igmp/IgmpIntfConfManagedObject.h"
#include "Nsm/Local/Igmp/IgmpPhyIntfLocalConfigMessage.h"
#include "Nsm/Local/Igmp/IgmpPoSviIntfLocalConfigMessage.h"

#include "Igmp/Local/IgmpLocalObjectManager.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmResourceIds.h"
#include "Utils/DceClusterUtils.h"

namespace DcmNs
{

    IgmpLocalObjectManager::IgmpLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        // Do nothing since we don't have any config that's not under an interface.
    }

    IgmpLocalObjectManager::~IgmpLocalObjectManager ()
    {
    }

    IgmpLocalObjectManager  *IgmpLocalObjectManager::getInstance()
    {
        static IgmpLocalObjectManager *pIgmpLocalObjectManager = new IgmpLocalObjectManager ();

        WaveNs::prismAssert (NULL != pIgmpLocalObjectManager, __FILE__, __LINE__);

        return (pIgmpLocalObjectManager);
    }

    string  IgmpLocalObjectManager::getClassName()
    {
        return ("Igmp");
    }

    PrismServiceId  IgmpLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *IgmpLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IgmpLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	igmp_intf_type IgmpLocalObjectManager::convertNsmIntfTypeToIgmpIntfType(InterfaceType intfType)
    {
        switch(intfType) {
            case IF_TYPE_GI:
                return INTERFACE_ONEGIG_IGMP;
                break;
            case IF_TYPE_TE:
                return INTERFACE_TENGIG_IGMP;
                break;
            case IF_TYPE_FO:
                return INTERFACE_FORTYGIG_IGMP;
                break;
            default:
                return INTERFACE_INVALID_IGMP;
                break;
        }
    }

    void IgmpLocalObjectManager::replayIgmpSviConfig(LocationId locId)
    {
        int size, size2;
        vector<NsmIntfSviManagedObject *> sviMOs;
        const NsmL3ReadyManagedObject* l3ReadyMO = NULL;
        IgmpIntfConfManagedObject* igmpIntfMO = NULL;
        ResourceId status;
        tracePrintf(TRACE_LEVEL_DEBUG, "%s:%d ", __PRETTY_FUNCTION__, __LINE__); 

        // First read the SVI MOs from the dB
        NsmL3Util::NodeVlanIfGetMOs(sviMOs, locId);
        size = sviMOs.size();
        if (!size) {
            tracePrintf(TRACE_LEVEL_DEBUG, "%s:%d No SVI MOs found. Returning.", __PRETTY_FUNCTION__, __LINE__); 
            return;
        }

        for (int i = 0; i < size; i++) {
            /* Now create a message for the backend for each IGMP IntfMO 
             * within the sviMO.
             */
            /* l3ReadyMO returned by getL3ReadyMO() is a const ptr. So cast it
             * to non-constant else call to getIgmpIntfConf() method doesn't
             * compile. */

            if((l3ReadyMO = (sviMOs[i]->getL3ReadyMO())) == NULL) 
                continue;

            if((igmpIntfMO = ((NsmL3ReadyManagedObject *)l3ReadyMO)->getIgmpIntfConf())==NULL)
                continue;

            IgmpClientPoSviIntfLocalConfigMessage *m = new IgmpClientPoSviIntfLocalConfigMessage();
            tracePrintf(TRACE_LEVEL_DEBUG, "%d:%d:%d:%d SVI MOs igmp config found" ,igmpIntfMO->getLmqi(),igmpIntfMO->getQmrt(),igmpIntfMO->getImmediateLeave(),igmpIntfMO->getQueryInterval() ); 
            m->setLmqi(igmpIntfMO->getLmqi());
            m->setQmrt(igmpIntfMO->getQmrt());
            m->setImmediateLeave(igmpIntfMO->getImmediateLeave());
            m->setQueryInterval(igmpIntfMO->getQueryInterval());

	    string ifName = toString(sviMOs[i]->getIfId());
	    trace(TRACE_LEVEL_DEBUG,string("ifname") +ifName);
	    trace(TRACE_LEVEL_DEBUG,string("ifname1") + sviMOs[i]->getIfId());
            m->setIfName(ifName);
            m->setIfType(INTERFACE_VLAN_IGMP);
            m->setOpCode(IGMP_LMQI | IGMP_QMRT | IGMP_QUERY_INTERVAL | IGMP_IMMEDIATE_LEAVE);
            m->setNegation(0);
            tracePrintf(TRACE_LEVEL_DEBUG, "%d:%d:%d:%d SVI MOs igmp config Send" ,m->getLmqi(),m->getQmrt(),m->getImmediateLeave(),m->getQueryInterval() ); 
            status = sendSynchronouslyToWaveClient("IGMP",  m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_DEBUG, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__); 
                tracePrintf(TRACE_LEVEL_DEBUG, "%s:%d Completion Status  %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                            m->getCompletionStatus()); 
            } else {
                tracePrintf(TRACE_LEVEL_DEBUG, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                            m->getCompletionStatus()); 
            }
            delete m;
        }

        for (int i = 0; i < size; i++) {
            if ((l3ReadyMO = (sviMOs[i]->getL3ReadyMO())) == NULL)
            {
	        trace(TRACE_LEVEL_DEBUG,string("sg-L3ReadyMo NULL continuing") );
                delete sviMOs[i]; 
                continue;
            } 
            vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > igmpSgMOs  = l3ReadyMO->getIgmpSgIntfConf();
            size2 = igmpSgMOs.size();
	    trace(TRACE_LEVEL_DEBUG,string("sg-size2")+size2 );
            for (int j = 0; j < size2; j++) {
                IgmpClientSgIntfLocalConfigMessage *m2 = new IgmpClientSgIntfLocalConfigMessage();

	        trace(TRACE_LEVEL_DEBUG,string("sg-igmpSgMo inside for") );
	        string ifName = igmpSgMOs[j]->getIfName();
	        trace(TRACE_LEVEL_DEBUG,string("sgifname") +ifName);
	        trace(TRACE_LEVEL_DEBUG,string("sgifname1") +igmpSgMOs[j]->getIfName() );
	        trace(TRACE_LEVEL_DEBUG,string("sgifType") +igmpSgMOs[j]->getIfType() );

                m2->setIfName(ifName);
                m2->setIfType(igmpSgMOs[j]->getIfType());
                m2->setGroupAddress(igmpSgMOs[j]->getGroupAddress());
                status = sendSynchronouslyToWaveClient("IGMP",  m2);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__);
                } else {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                                m2->getCompletionStatus());
                }
                delete m2;
            }
            delete sviMOs[i];
        }
    }
    
    void IgmpLocalObjectManager::replayIgmpPoConfig(LocationId locId)
    {
        int size, size2, mapId = 0;
        vector<NsmNodePoManagedObject *> poMOs;
        const NsmL3ReadyManagedObject *l3ReadyMO;
        IgmpIntfConfManagedObject *igmpIntfMO;
        ResourceId status;

        tracePrintf(TRACE_LEVEL_INFO, "%s:%d ", __PRETTY_FUNCTION__, __LINE__); 

        // First read the SVI MOs from the dB
        NsmL3Util::NodePoIfGetMOs(poMOs, mapId);
        size = poMOs.size();
        if (!size) {
            tracePrintf(TRACE_LEVEL_INFO, "%s:%d No Po MOs found. Returning.", __PRETTY_FUNCTION__, __LINE__); 
            return;
        }

        for (int i = 0; i < size; i++) {
            /* Now create a message for the backend for each IGMP IntfMO 
             * within the poMO.
             */
            /* l3ReadyMO returned by getL3ReadyMO() is a const ptr. So cast it
             * to non-constant else call to getIgmpIntfConf() method doesn't
             * compile. */
            if ((l3ReadyMO = (poMOs[i]->getL3ReadyMO())) == NULL
                || (igmpIntfMO = ((NsmL3ReadyManagedObject *)l3ReadyMO)->getIgmpIntfConf()) == NULL)
                continue;
            IgmpClientPoSviIntfLocalConfigMessage *m = new IgmpClientPoSviIntfLocalConfigMessage();
            m->setLmqi(igmpIntfMO->getLmqi());
            m->setQmrt(igmpIntfMO->getQmrt());
            m->setImmediateLeave(igmpIntfMO->getImmediateLeave());
            m->setQueryInterval(igmpIntfMO->getQueryInterval());
            /* Pass 2 tuple name to backend if we are in VCS mode */
			string ifname;
			string intfName = poMOs[i]->getIfName();
            if(NsmUtils::isValidThreeTuple(intfName)) {
                NsmUtils::getTwoTupleIfName(intfName, ifname);
            }
            else {
                ifname = intfName;
            }

            m->setIfName(ifname);
            m->setIfType(INTERFACE_PORT_CHANNEL_IGMP);
            m->setOpCode(IGMP_LMQI | IGMP_QMRT | IGMP_QUERY_INTERVAL | IGMP_IMMEDIATE_LEAVE);
            m->setNegation(0);
            status = sendSynchronouslyToWaveClient("IGMP",  m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_INFO, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__); 
            } else {
                tracePrintf(TRACE_LEVEL_INFO, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                            m->getCompletionStatus()); 
            }
            delete m;
        }

        for (int i = 0; i < size; i++) {
            if ((l3ReadyMO = (poMOs[i]->getL3ReadyMO())) == NULL)
            {
                delete poMOs[i]; 
                continue;
            }
            vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > igmpSgMOs  = l3ReadyMO->getIgmpSgIntfConf();
            size2 = igmpSgMOs.size();
            for (int j = 0; j < size2; j++) {
                IgmpClientSgIntfLocalConfigMessage *m2 = new IgmpClientSgIntfLocalConfigMessage();
                /* Pass 2 tuple name to backend if we are in VCS mode */
            	string ifname;
            	string intfName = igmpSgMOs[j]->getIfName();
            	if(NsmUtils::isValidThreeTuple(intfName)) {
                	NsmUtils::getTwoTupleIfName(intfName, ifname);
            	}
            	else {
                	ifname = intfName;
            	}

                m2->setIfName(ifname);
                m2->setIfType(igmpSgMOs[j]->getIfType());
                m2->setGroupAddress(igmpSgMOs[j]->getGroupAddress());
                status = sendSynchronouslyToWaveClient("IGMP",  m2);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__); 
                } else {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                                m2->getCompletionStatus()); 
                }
                delete m2;
            }
            delete poMOs[i];
        }
    }

    void IgmpLocalObjectManager::slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
    {
        pWaveAsynchronousContextForSlotFailover->
            setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForSlotFailover->callback();
    }

	void IgmpLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string passName = pWaveAsynchronousContextForPostBootPhase->getPassName();
		LocationId locId = FrameworkToolKit::getThisLocationId();

        tracePrintf(TRACE_LEVEL_INFO, "%s:%d Entered with pass:%s", __PRETTY_FUNCTION__, __LINE__, passName.c_str());
        // Check for Layer3 license before sending to backend
        if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
            tracePrintf(TRACE_LEVEL_INFO, "IgmpLocalObjectManager::postboot. LAYER_3_LICENSE not present.", __PRETTY_FUNCTION__, __LINE__);
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

        if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE4") == 0) {
            tracePrintf(TRACE_LEVEL_INFO, "%s:%d DCM_POSTBOOT_GLOBAL_STAGE_4", __PRETTY_FUNCTION__, __LINE__); 
            replayIgmpSviConfig(locId);
            replayIgmpPoConfig(locId);
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

void IgmpLocalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
        {
                dce_cluster_validation_data_t *data_p;
                ResourceId status = WAVE_MESSAGE_SUCCESS;

                trace(TRACE_LEVEL_DEBUG, "OPSFLocalObjectManager::clusterCreateCollectValidationData:Cluster Collect function called.");
                data_p = dce_cluster_validation_collect_init(IGMP_PLUGIN_LOCAL_VERSION);
                if (data_p == NULL)
                {
                        trace(TRACE_LEVEL_ERROR, "IgmpLocalObjectManager::clusterCreateCollectValidationData:Failed to init ValidationData");
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

        void IgmpLocalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
        {
                void                            *payload_p;
                dce_cluster_validation_data_t   *data_p;
                UI32                            len;
                ResourceId                      status = WAVE_MESSAGE_SUCCESS;
                uint32_t                        version = 0, num_records = 0;
                uint64_t                        cap_bits = 0;

                trace(TRACE_LEVEL_DEBUG, "IgmpLocalObjectManager::clusterCreateValidate:Cluster Validate function called.");
                pMessage->getValidationDetails(payload_p, len);
                trace(TRACE_LEVEL_DEBUG,string("IgmpLocalObjectManager::clusterCreateValidate:Received Validation Data of LEN=") + len);
                data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

                status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
                if (status != DCE_CLUSTER_UTIL_SUCCESS)
                {
                        trace(TRACE_LEVEL_ERROR, string("IgmpLocalObjectManager::clusterCreateValidate:Failed to parse ValidationData Err=") + FrameworkToolKit::localize(status));
                        pPrismAsynchronousContext->setCompletionStatus(status);
                        pPrismAsynchronousContext->callback ();
                        return;
                }
                trace(TRACE_LEVEL_DEBUG, string("IgmpLocalObjectManager::clusterCreateValidate:Recieved version = ") + version);
                if (version != IGMP_PLUGIN_LOCAL_VERSION)
                {
                        trace(TRACE_LEVEL_ERROR, string("IgmpLocalObjectManager::clusterCreateValidate:Version mismatch, My-Version=") + IGMP_PLUGIN_LOCAL_VERSION + string(" Rcvd-Version=") + version);
                        pPrismAsynchronousContext->setCompletionStatus(NSM_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
                        pPrismAsynchronousContext->callback ();
                        return;
                }

                pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                pPrismAsynchronousContext->callback ();
                return;
        }

	void IgmpLocalObjectManager::replayIgmpPhyConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
    {
        int  size2;
        const NsmL3ReadyManagedObject* l3ReadyMO ;
        IgmpIntfConfManagedObject* igmpIntfMO;
        ResourceId status;

        tracePrintf(TRACE_LEVEL_INFO, "%s:%d ", __PRETTY_FUNCTION__, __LINE__); 


            /* l3ReadyMO returned by getL3ReadyMO() is a const ptr. So cast it
             * to non-constant else call to getIgmpIntfConf() method doesn't
             * compile. */
            if ((l3ReadyMO = (pPhyIntfLocalManagedObject->getL3ReadyMO())) == NULL
                || (igmpIntfMO = ((NsmL3ReadyManagedObject *)l3ReadyMO)->getIgmpIntfConf()) == NULL)
                return;
            IgmpClientPhyIntfLocalConfigMessage *m = new IgmpClientPhyIntfLocalConfigMessage();
            m->setLmqi(igmpIntfMO->getLmqi());
            m->setQmrt(igmpIntfMO->getQmrt());
            m->setImmediateLeave(igmpIntfMO->getImmediateLeave());
            m->setQueryInterval(igmpIntfMO->getQueryInterval());

			/* Pass 2 tuple name to backend if we are in VCS mode */
            string ifname;
            string intfName = pPhyIntfLocalManagedObject->getIfName();
            if(NsmUtils::isValidThreeTuple(intfName)) {
                NsmUtils::getTwoTupleIfName(intfName, ifname);
            }
            else {
                ifname = intfName;
            }

            m->setIfName(ifname);
            m->setIfType(convertNsmIntfTypeToIgmpIntfType(pPhyIntfLocalManagedObject->getPhyType()));
            m->setOpCode(IGMP_LMQI | IGMP_QMRT | IGMP_QUERY_INTERVAL | IGMP_IMMEDIATE_LEAVE);
            m->setNegation(0);
            status = sendSynchronouslyToWaveClient("IGMP",  m);
            if (status == WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_INFO, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__); 
            } else {
                tracePrintf(TRACE_LEVEL_INFO, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                            m->getCompletionStatus()); 
            }
            delete m;

            vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > igmpSgMOs  = l3ReadyMO->getIgmpSgIntfConf();
            size2 = igmpSgMOs.size();
            for (int j = 0; j < size2; j++) {
                IgmpClientSgIntfLocalConfigMessage *m2 = new IgmpClientSgIntfLocalConfigMessage();

                /* Pass 2 tuple name to backend if we are in VCS mode */
            	intfName = igmpSgMOs[j]->getIfName();
            	if(NsmUtils::isValidThreeTuple(intfName)) {
               		NsmUtils::getTwoTupleIfName(intfName, ifname);
            	}
            	else {
                	ifname = intfName;
            	}

                m2->setIfName(ifname);
                m2->setIfType(igmpSgMOs[j]->getIfType());
                m2->setGroupAddress(igmpSgMOs[j]->getGroupAddress());
                status = sendSynchronouslyToWaveClient("IGMP",  m2);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Successfully sent msg. ", __PRETTY_FUNCTION__, __LINE__); 
                } else {
                    tracePrintf(TRACE_LEVEL_INFO, "%s:%d Error %d sending msg. ", __PRETTY_FUNCTION__, __LINE__,
                                m2->getCompletionStatus()); 
                }
                delete m2;
            }
    }
	
}

