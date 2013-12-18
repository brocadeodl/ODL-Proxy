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
 *   Author : vdharwad                                                     *
 **************************************************************************/
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "DcmResourceIdEnums.h"


#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalWorker.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

#include "Framework/Utils/FrameworkToolKit.h"

#include "Fcoe/Common/FcoePluginDebug.h"
#include "Fcoe/Common/FcoePluginTypes.h"
//#include "Fcoe/Common/FcoePluginUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveLocalManagedObject.h"

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"

#include "DcmCore/DcmToolKit.h"

#include "Fcoe/Local/FcoeIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeETIntfLocalManagedObject.h"

#include "Utils/DceClusterUtils.h"

namespace DcmNs
{

    FcoeGlobalObjectManager::FcoeGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pFcoeGlobalWorker = new FcoeGlobalWorker (this);
        prismAssert (NULL != m_pFcoeGlobalWorker, __FILE__, __LINE__);
    }

    FcoeGlobalObjectManager::~FcoeGlobalObjectManager ()
    {
    }

    FcoeGlobalObjectManager  *FcoeGlobalObjectManager::getInstance()
    {
        static FcoeGlobalObjectManager *pFcoeGlobalObjectManager = new FcoeGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pFcoeGlobalObjectManager, __FILE__, __LINE__);

        return (pFcoeGlobalObjectManager);
    }

    string  FcoeGlobalObjectManager::getClassName()
    {
        return ("FcoeGlobal");
    }

    PrismServiceId  FcoeGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FcoeGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FcoeGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId
    FcoeGlobalObjectManager::FcoeGlobalCleanupInterfacesForLocationGeneric (UI32 &location_id,
                                                                            const string &className)
    {
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId(location_id,
                                                                                                   className);
        vector<WaveManagedObject *>::iterator it;
        WaveManagedObject  *loc_mo_p;

        FCOE_PLUG_DBG_S(string("Found ") + pResults->size() + className +
                        string("interfaces for LocationId = ") + location_id);
        startTransaction();
        
        for (it = pResults->begin();
             it != pResults->end();
             it++) {

            loc_mo_p = (*it);

            delete (loc_mo_p);
        }
        commitTransaction();

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId
    FcoeGlobalObjectManager::FcoeGlobalHandleFailoverForLocation (UI32 &location_id)
    {
        ResourceId  status;
        
        status = FcoeGlobalObjectManager::FcoeGlobalCleanupInterfacesForLocationGeneric(location_id,
                                                                                        FcoeIntfLocalManagedObject::getClassName());
        status = FcoeGlobalObjectManager::FcoeGlobalCleanupInterfacesForLocationGeneric(location_id,
                                                                                        FcoeETIntfLocalManagedObject::getClassName());
        return status;
    }

    void
    FcoeGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
        vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

        vector<LocationId>::iterator    it;
        
        FCOE_PLUG_DBG_S(string("Rcvd Failover with Reason=") + FrameworkToolKit::localize(failoverReason));

        if (failoverReason == FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED) {
            FCOE_PLUG_DBG("Uncontrolled failover rcvd.. Returning");
            pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pFailoverAsynchronousContext->callback ();
            return;
        }
        
        for (it = failedLocationIds.begin();
             it != failedLocationIds.end();
             it++) {
            FCOE_PLUG_DBG_S(string("Failed Location = ") + *it);

            FcoeGlobalObjectManager::FcoeGlobalHandleFailoverForLocation(*it);
        }


        pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pFailoverAsynchronousContext->callback ();
    }

    void
    FcoeGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, 
                                                                 PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        
        FCOE_PLUG_DBG("Cluster Collect function called.");

        data_p = dce_cluster_validation_collect_init(FCOE_PLUGIN_GLOBAL_VERSION);
        if (data_p == NULL) {
            FCOE_PLUG_DBG_ERR("Failed to init ValidationData");
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
    FcoeGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage,
                                                    PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;
        
        FCOE_PLUG_DBG("Cluster Validate function called.");

        pMessage->getValidationDetails(payload_p, len);

        FCOE_PLUG_DBG_S(string("Received Validation Data of LEN=") + len);

        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            FCOE_PLUG_DBG_ERR_S(string("Failed to parse ValidationData Err=") +
                                FrameworkToolKit::localize(status)); 
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        FCOE_PLUG_DBG_S(string("Recieved version = ") + version);
        
        if (version != FCOE_PLUGIN_GLOBAL_VERSION) {
            FCOE_PLUG_DBG_ERR_S(string("Version mismatch, My-Version=") + FCOE_PLUGIN_GLOBAL_VERSION +
                                string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(FCOE_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }
    
}
