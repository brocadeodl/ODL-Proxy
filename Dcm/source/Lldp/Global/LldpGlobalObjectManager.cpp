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
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalConfWorker.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "Lldp/Common/LldpPluginDebug.h"
#include "Lldp/Global/CdpGlobalConfWorker.h"

#include "Utils/DceClusterUtils.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveLocalManagedObject.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"

#include "DcmResourceIdEnums.h"

#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    LldpGlobalObjectManager::LldpGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pLldpGlobalConfWorker = new LldpGlobalConfWorker (this);
        prismAssert (NULL != m_pLldpGlobalConfWorker, __FILE__, __LINE__);

        m_pCdpGlobalConfWorker = new CdpGlobalConfWorker (this);
        prismAssert (NULL != m_pCdpGlobalConfWorker, __FILE__, __LINE__);
    }

    LldpGlobalObjectManager::~LldpGlobalObjectManager ()
    {
    }

    LldpGlobalObjectManager  *LldpGlobalObjectManager::getInstance()
    {
        static LldpGlobalObjectManager *pLldpGlobalObjectManager = new LldpGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pLldpGlobalObjectManager, __FILE__, __LINE__);

        return (pLldpGlobalObjectManager);
    }

    string  LldpGlobalObjectManager::getClassName()
    {
        return ("LldpGlobal");
    }

    PrismServiceId  LldpGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *LldpGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LldpGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void
    LldpGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, 
                                                                 PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        
        LLDP_PLUG_DBG("Cluster Collect function called.");

        data_p = dce_cluster_validation_collect_init(LLDP_PLUGIN_GLOBAL_VERSION);
        if (data_p == NULL) {
            LLDP_PLUG_DBG_ERR("Failed to init ValidationData");
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
    LldpGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage,
                                                    PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;
        
        LLDP_PLUG_DBG("Cluster Validate function called.");

        pMessage->getValidationDetails(payload_p, len);

        LLDP_PLUG_DBG_S(string("Received Validation Data of LEN=") + len);

        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            LLDP_PLUG_DBG_ERR(string("Failed to parse ValidationData Err=") +
                                FrameworkToolKit::localize(status)); 
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        LLDP_PLUG_DBG_S(string("Recieved version = ") + version);
        
        if (version != LLDP_PLUGIN_GLOBAL_VERSION) {
            LLDP_PLUG_DBG_ERR(string("Version mismatch, My-Version=") + LLDP_PLUGIN_GLOBAL_VERSION +
                                string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(WRC_LLDP_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }

}
