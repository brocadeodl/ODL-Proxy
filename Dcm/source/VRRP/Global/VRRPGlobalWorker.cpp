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

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "VRRP/Global/VRRPGlobalObjectManager.h"
#include "VRRP/Global/VRRPGlobalWorker.h"
#include "VRRP/Global/VRRPGlobalUpdateVRRPGlobalConfigMessage.h"
#include "VRRP/Global/VRRPGlobalTypes.h"
#include "VRRP/Local/VRRPUpdateVRRPConfigMessage.h"

namespace DcmNs
{

    VRRPGlobalWorker::VRRPGlobalWorker ( VRRPGlobalObjectManager *pVRRPGlobalObjectManager)
        : WaveWorker  (pVRRPGlobalObjectManager)
    {
        addOperationMap (VRRPGLOBALUPDATEVRRPGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&VRRPGlobalWorker::VRRPGlobalUpdateVRRPGlobalConfigMessageHandler));
    }

    VRRPGlobalWorker::~VRRPGlobalWorker ()
    {
    }

    PrismMessage  *VRRPGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case VRRPGLOBALUPDATEVRRPGLOBALCONFIG :
                pPrismMessage = new VRRPGlobalUpdateVRRPGlobalConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *VRRPGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId VRRPGlobalWorker::createVRRPGlobalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        VRRPGlobalUpdateVRRPGlobalConfigMessage *pVRRPGlobalUpdateVRRPGlobalConfigMessage = dynamic_cast<VRRPGlobalUpdateVRRPGlobalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "VRRPGlobalWorker::createVRRPGlobalConfigStep Entered....");
        
        int status;
        VRRPUpdateVRRPConfigMessage *m = new VRRPUpdateVRRPConfigMessage();
        
        m->setConfigDisable(pVRRPGlobalUpdateVRRPGlobalConfigMessage->getConfigDisable());
        m->setVrrpEnable(pVRRPGlobalUpdateVRRPGlobalConfigMessage->getVrrpEnable());
        m->setVrrpeEnable(pVRRPGlobalUpdateVRRPGlobalConfigMessage->getVrrpeEnable());
        
        status = sendSynchronously(m);
        
        pVRRPGlobalUpdateVRRPGlobalConfigMessage->setCompletionStatus(m->getCompletionStatus());

        if (status == WAVE_MESSAGE_SUCCESS)
            if (pVRRPGlobalUpdateVRRPGlobalConfigMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                status = pVRRPGlobalUpdateVRRPGlobalConfigMessage->getCompletionStatus();

		if (m)
			delete (m);

        return status;
    }

    void  VRRPGlobalWorker::VRRPGlobalUpdateVRRPGlobalConfigMessageHandler( VRRPGlobalUpdateVRRPGlobalConfigMessage *pVRRPGlobalUpdateVRRPGlobalConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalWorker::createVRRPGlobalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pVRRPGlobalUpdateVRRPGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}
