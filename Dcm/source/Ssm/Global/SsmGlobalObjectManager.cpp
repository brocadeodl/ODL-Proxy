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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/MacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalAddMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalRemMacAclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocRuleToPolicyMessage.h"
#include "Ssm/Global/SsmGlobalAssocPolicyToInterfaceMessage.h"
#include "Ssm/Global/SsmMacAclWorker.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Ssm/Global/SsmIpAclWorker.h"
#include "Ssm/Global/SsmIp6AclWorker.h"
#include "Ssm/Global/MacAclConfigurationHandler.h"
#include "Ssm/Global/MacAclRuleConfigurationHandler.h"
#include "Ssm/Global/ExtMacAclRuleConfigurationHandler.h"
#include "Ssm/Global/StdIpAclRuleConfigurationHandler.h"
#include "Ssm/Global/ExtIpAclRuleConfigurationHandler.h"
#include "DcmResourceIdEnums.h"
#include "Utils/DceClusterUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Ssm/Global/SsmPlugUtils.h"
namespace DcmNs
{

    SsmGlobalObjectManager::SsmGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pSsmMacAclWorker = new SsmMacAclWorker (this);
        prismAssert (NULL != m_pSsmMacAclWorker, __FILE__, __LINE__);

        m_pSsmIpAclWorker = new SsmIpAclWorker (this);
        prismAssert (NULL != m_pSsmIpAclWorker, __FILE__, __LINE__);

        m_pSsmIp6AclWorker = new SsmIp6AclWorker (this);
        prismAssert (NULL != m_pSsmIp6AclWorker, __FILE__, __LINE__);
        
        m_pMacAclConfigurationHandler = new MacAclConfigurationHandler (this);
        prismAssert (NULL != m_pMacAclConfigurationHandler, __FILE__, __LINE__);
        
        m_pMacAclRuleConfigurationHandler = new MacAclRuleConfigurationHandler (this);
        prismAssert (NULL != m_pMacAclRuleConfigurationHandler, __FILE__, __LINE__);

        m_pExtMacAclRuleConfigurationHandler = new ExtMacAclRuleConfigurationHandler (this);
        prismAssert (NULL != m_pExtMacAclRuleConfigurationHandler, __FILE__, __LINE__);

        m_pStdIpAclRuleConfigurationHandler = new StdIpAclRuleConfigurationHandler (this);
        prismAssert (NULL != m_pStdIpAclRuleConfigurationHandler, __FILE__, __LINE__);

        m_pExtIpAclRuleConfigurationHandler = new ExtIpAclRuleConfigurationHandler (this);
        prismAssert (NULL != m_pExtIpAclRuleConfigurationHandler, __FILE__, __LINE__);
	}

    SsmGlobalObjectManager::~SsmGlobalObjectManager ()
    {
    }

    SsmGlobalObjectManager  *SsmGlobalObjectManager::getInstance()
    {
        static SsmGlobalObjectManager *pSsmGlobalObjectManager = new SsmGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pSsmGlobalObjectManager, __FILE__, __LINE__);

        return (pSsmGlobalObjectManager);
    }

    PrismServiceId  SsmGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SsmGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SSMGLOBALADDMACACLPOLICY :
                pPrismMessage = new SsmGlobalAddMacAclPolicyMessage ();
                break;
            case SSMGLOBALREMMACACLPOLICY :
                pPrismMessage = new SsmGlobalRemMacAclPolicyMessage ();
                break;
            case SSMGLOBALASSOCRULETOPOLICY :
                pPrismMessage = new SsmGlobalAssocRuleToPolicyMessage ();
                break;
            case SSMGLOBALASSOCPOLICYTOINTERFACE :
                pPrismMessage = new SsmGlobalAssocPolicyToInterfaceMessage ();
                break;
			default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SsmGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((MacAclRuleManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MacAclRuleManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "SsmGlobalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	void SsmGlobalObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        SSM_PLUG_DBG_LVL("Cluster Collect function called.");

        data_p = dce_cluster_validation_collect_init(SSM_PLUGIN_GLOBAL_VERSION);
        if (data_p == NULL)
        {
            SSM_PLUG_ERR_LVL("Failed to init ValidationData");
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

	void SsmGlobalObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;
		
        SSM_PLUG_DBG_LVL("Cluster Validate function called.");
        pMessage->getValidationDetails(payload_p, len);
        
		SSM_PLUG_DBG_LVL_S(string("Received Validation Data of LEN=") + len);

        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);
		
        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS)
        {
            SSM_PLUG_ERR_LVL_S(string("Failed to parse ValidationData Err=") +
            FrameworkToolKit::localize(status));
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        SSM_PLUG_DBG_LVL_S(string("Recieved version = ") + version);

        if (version != SSM_PLUGIN_GLOBAL_VERSION)
        {
		    SSM_PLUG_ERR_LVL_S(string("Version mismatch, My-Version=") + SSM_PLUGIN_GLOBAL_VERSION + string(" Rcvd-Version=") + version);
			pPrismAsynchronousContext->setCompletionStatus(SSM_CLUSTER_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }
		pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pPrismAsynchronousContext->callback ();
		return;
    }

    void  SsmGlobalObjectManager::SsmGlobalAddMacAclPolicyMessageHandler( SsmGlobalAddMacAclPolicyMessage *pSsmGlobalAddMacAclPolicyMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSsmGlobalAddMacAclPolicyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  SsmGlobalObjectManager::SsmGlobalAssocPolicyToInterfaceMessageHandler( SsmGlobalAssocPolicyToInterfaceMessage *pSsmGlobalAssocPolicyToInterfaceMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSsmGlobalAssocPolicyToInterfaceMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  SsmGlobalObjectManager::SsmGlobalAssocRuleToPolicyMessageHandler( SsmGlobalAssocRuleToPolicyMessage *pSsmGlobalAssocRuleToPolicyMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSsmGlobalAssocRuleToPolicyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  SsmGlobalObjectManager::SsmGlobalRemMacAclPolicyMessageHandler( SsmGlobalRemMacAclPolicyMessage *pSsmGlobalRemMacAclPolicyMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SsmGlobalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSsmGlobalRemMacAclPolicyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
	
}

