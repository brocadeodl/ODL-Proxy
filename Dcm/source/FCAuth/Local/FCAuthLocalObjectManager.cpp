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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

#include "FCAuth/Local/AuthProtocolLocalManagedObject.h"
#include "FCAuth/Local/AuthPolicyLocalManagedObject.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/LocalSecPolicyWorker.h"
#include "FCAuth/Local/LocalAuthSecretWorker.h"
#include "FCAuth/Local/LocalAuthWorker.h"
#include "FCAuth/Local/FCAuthTypes.h"
#include "FCAuth/Global/FCAuthGlobalObjectManager.h"

#define AUTH_PLUGIN_FSS_STAGE10 10
#include "DcmResourceIdEnums.h"
#include "fabos/license.h"

namespace DcmNs
{

    FCAuthLocalObjectManager::FCAuthLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
		associateWithVirtualWaveObjectManager(FCAuthGlobalObjectManager::getInstance ());
        m_pLocalSecPolicyWorker = new LocalSecPolicyWorker (this);
        prismAssert (NULL != m_pLocalSecPolicyWorker, __FILE__, __LINE__);
        m_pLocalAuthSecretWorker = new LocalAuthSecretWorker (this);
        prismAssert (NULL != m_pLocalAuthSecretWorker, __FILE__, __LINE__);
        m_pLocalAuthWorker = new LocalAuthWorker (this);
        prismAssert (NULL != m_pLocalAuthWorker, __FILE__, __LINE__);
        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
    }

    FCAuthLocalObjectManager::~FCAuthLocalObjectManager ()
    {
    }

    FCAuthLocalObjectManager  *FCAuthLocalObjectManager::getInstance()
    {
        static FCAuthLocalObjectManager *pFCAuthLocalObjectManager = new FCAuthLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFCAuthLocalObjectManager, __FILE__, __LINE__);

        return (pFCAuthLocalObjectManager);
    }

    string  FCAuthLocalObjectManager::getClassName()
    {
        return ("FCAuth");
    }

    PrismServiceId  FCAuthLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FCAuthLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FCAuthLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void FCAuthLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int          swModel = get_pod_SWBD();
        trace (TRACE_LEVEL_INFO, "FCAuthLocalObjectManager::install()");

        if ((SWBD_ELARA2F != swModel) && (SWBD_CALLISTOF != swModel)) {
            trace (TRACE_LEVEL_INFO, "FCAuthLocalObjectManager::install: Command not supported in this platform");
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

        ResourceId bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

        /* Node is out-of-the-box or is rebooting after net-install is performed */
        if ((bootReason == WAVE_BOOT_FIRST_TIME_BOOT) ||
            (bootReason == WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT))
        {

            trace(TRACE_LEVEL_INFO, string("FCAuth local 'install' Service called with reason - ") + FrameworkToolKit::localize(bootReason));

            startTransaction ();

            /* Install auth protocol managed object */
            AuthProtocolLocalManagedObject *pAuthProtocolLocalManagedObj = new AuthProtocolLocalManagedObject(this);

            /* Install auth policy managed object */
            AuthPolicyLocalManagedObject *pAuthPolicyLocalManagedObj = new AuthPolicyLocalManagedObject(this);

            /* Commit to the Wave Database */
            status = commitTransaction ();
            if (FRAMEWORK_SUCCESS == status) {
                trace (TRACE_LEVEL_INFO, "Installed Auth Protocol and Policy Managed Objects");
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                trace (TRACE_LEVEL_FATAL, "Failed to install Auth Protocol and Policy Managed Objects");
                prismAssert (false, __FILE__, __LINE__);
            }

            delete pAuthProtocolLocalManagedObj;
            delete pAuthPolicyLocalManagedObj;
        }

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    void FCAuthLocalObjectManager::externalStateSynchronization (
             WaveAsynchronousContextForExternalStateSynchronization
             *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        if ( (pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == AUTH_PLUGIN_FSS_STAGE10) &&
            (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) )
        {
            trace (TRACE_LEVEL_INFO, "FcAuthLocalObjectManager::externalStateSynchronization Entering..");
            int ret = m_pLocalAuthWorker->fwBackendOpAuthUtil();
            if (0 != ret)
            {
                trace (TRACE_LEVEL_INFO, string("FCAuthLocalObjectManager::externalStateSynchronization fwBackendOpAuthUtil failed"));
            }
        }
        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();
    }



}
