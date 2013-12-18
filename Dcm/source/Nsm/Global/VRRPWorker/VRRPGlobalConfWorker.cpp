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
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/VRRPWorker/VRRPGlobalConfWorker.h"
#include "Nsm/Global/VRRPWorker/NsmGlobalUpdateVRRPIntfConfGlobalMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/VRRPWorker/NsmUpdateVRRPLocalConfigPhyIntfMessage.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    VRRPGlobalConfWorker::VRRPGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        addOperationMap (NSMGLOBALUPDATEVRRPINTFCONFGLOBAL, reinterpret_cast<PrismMessageHandler> (&VRRPGlobalConfWorker::NsmGlobalUpdateVRRPIntfConfGlobalMessageHandler));
    }

    VRRPGlobalConfWorker::~VRRPGlobalConfWorker ()
    {
    }

    PrismMessage  *VRRPGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMGLOBALUPDATEVRRPINTFCONFGLOBAL :
                pPrismMessage = new NsmGlobalUpdateVRRPIntfConfGlobalMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *VRRPGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId VRRPGlobalConfWorker::createVRRPGlobalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, "VRRPGlobalConfWorker::createVRRPGlobalConfigSte: Entered....");

        NsmGlobalUpdateVRRPIntfConfGlobalMessage *pNsmGlobalUpdateVRRPIntfConfGlobalMessage = dynamic_cast<NsmGlobalUpdateVRRPIntfConfGlobalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        int status;
        NsmUpdateVRRPLocalConfigPhyIntfMessage *m = new NsmUpdateVRRPLocalConfigPhyIntfMessage();

        m->setIfName(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getIfName());
        m->setIfType(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getIfType());
        m->setVrid(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getVrid());
        m->setVirtual_ip(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getVirtual_ip());
        m->setTrack_ifType(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getTrack_ifType());
        m->setTrack_ifName(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getTrack_ifName());
        m->setTrack_prio(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getTrack_prio());
        m->setAdvt_int(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getAdvt_int());
        m->setPrio(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getPrio());
        m->setEnable(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getEnable());
        m->setPreempt(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getPreempt());
        m->setHold_time(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getHold_time());
        m->setDescription(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getDesc());
        m->setAdvt_backup(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getAdvt_backup());
        m->setGarp_timer(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getGarp_timer());
        m->setBackup_advt_int(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getBackup_advt_int());
        m->setVrrp_type(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getVrrp_type());
        m->setChoice(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getChoice());

        status = sendSynchronously (m);
        pNsmGlobalUpdateVRRPIntfConfGlobalMessage->setCompletionStatus(m->getCompletionStatus());

        if(status == WAVE_MESSAGE_SUCCESS)
            if(pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                status = pNsmGlobalUpdateVRRPIntfConfGlobalMessage->getCompletionStatus();

		if (m)
			delete m;

        return status;
    }

    void  VRRPGlobalConfWorker::NsmGlobalUpdateVRRPIntfConfGlobalMessageHandler( NsmGlobalUpdateVRRPIntfConfGlobalMessage *pNsmGlobalUpdateVRRPIntfConfGlobalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalConfWorker::createVRRPGlobalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPGlobalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmGlobalUpdateVRRPIntfConfGlobalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}
