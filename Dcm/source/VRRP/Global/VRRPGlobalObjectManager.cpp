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
 *   Author : krao                                                     *
 **************************************************************************/

#include "VRRP/Global/VRRPGlobalObjectManager.h"
#include "VRRP/Global/VRRPGlobalWorker.h"
#include "VRRP/Global/VRRPGlobalTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

    VRRPGlobalObjectManager::VRRPGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pVRRPGlobalWorker = new VRRPGlobalWorker (this);
        prismAssert (NULL != m_pVRRPGlobalWorker, __FILE__, __LINE__);
    }

    VRRPGlobalObjectManager::~VRRPGlobalObjectManager ()
    {
    }

    VRRPGlobalObjectManager  *VRRPGlobalObjectManager::getInstance()
    {
        static VRRPGlobalObjectManager *pVRRPGlobalObjectManager = new VRRPGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pVRRPGlobalObjectManager, __FILE__, __LINE__);

        return (pVRRPGlobalObjectManager);
    }

    string  VRRPGlobalObjectManager::getClassName()
    {
        return ("VRRPGlobal");
    }

    PrismServiceId  VRRPGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *VRRPGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *VRRPGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void VRRPGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t *data_p;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_INFO, "VRRPGlobalObjectManager::clusterCreateCollectValidationData:Cluster Collect function called.");
        data_p = dce_cluster_validation_collect_init(VRRP_PLUGIN_GLOBAL_VERSION);
        if (data_p == NULL)
        {
            trace(TRACE_LEVEL_ERROR, "VRRPGlobalObjectManager::clusterCreateCollectValidationData:Failed to init ValidationData");
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

    void VRRPGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;

        trace(TRACE_LEVEL_INFO, "VRRPGlobalObjectManager::clusterCreateValidate:Cluster Validate function called.");
        pMessage->getValidationDetails(payload_p, len);
        trace(TRACE_LEVEL_INFO,string("VRRPGlobalObjectManager::clusterCreateValidate:Received Validation Data of LEN=") + len);
        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS)
        {
            trace(TRACE_LEVEL_ERROR, string("VRRPGlobalObjectManager::clusterCreateValidate:Failed to parse ValidationData Err=") + FrameworkToolKit::localize(status));
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }
        trace(TRACE_LEVEL_INFO, string("VRRPGlobalObjectManager::clusterCreateValidate:Recieved version = ") + version);
        if (version != VRRP_PLUGIN_GLOBAL_VERSION)
	   	{
            trace(TRACE_LEVEL_ERROR, string("VRRPGlobalObjectManager::clusterCreateValidate:Version mismatch, My-Version=") + VRRP_PLUGIN_GLOBAL_VERSION + string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(NSM_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }


}