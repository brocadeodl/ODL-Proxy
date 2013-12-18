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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "Sflow/Global/SflowGlobalWorker.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceClusterUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCStatus.h"


namespace DcmNs
{

    SflowGlobalObjectManager::SflowGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pSflowGlobalWorker = new SflowGlobalWorker (this);
        prismAssert (NULL != m_pSflowGlobalWorker, __FILE__, __LINE__);
    }

    SflowGlobalObjectManager::~SflowGlobalObjectManager ()
    {
    }

    SflowGlobalObjectManager  *SflowGlobalObjectManager::getInstance()
    {
        static SflowGlobalObjectManager *pSflowGlobalObjectManager = new SflowGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pSflowGlobalObjectManager, __FILE__, __LINE__);

        return (pSflowGlobalObjectManager);
    }

    string  SflowGlobalObjectManager::getClassName()
    {
        return ("SflowGlobal");
    }

    PrismServiceId  SflowGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SflowGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SflowGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	void SflowGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
	{
		dce_cluster_validation_data_t   *data_p;
		ResourceId                      status = WRC_WAVE_MESSAGE_SUCCESS;

		NSM_PLUG_DBG("Cluster Collect function called.");

		data_p = dce_cluster_validation_collect_init(SFLOW_PLUGIN_GLOBAL_VERSION);
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
	
	void SflowGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
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

		if (version != SFLOW_PLUGIN_GLOBAL_VERSION) 
		{
			NSM_PLUG_DBG_ERR_S(string("Version mismatch, My-Version=") + SFLOW_PLUGIN_GLOBAL_VERSION + string(" Rcvd-Version=") + version);
			pPrismAsynchronousContext->setCompletionStatus(WRC_FCOE_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
			pPrismAsynchronousContext->callback ();
			return;
		}
        pPrismAsynchronousContext->setCompletionStatus (WRC_WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
	}


}
