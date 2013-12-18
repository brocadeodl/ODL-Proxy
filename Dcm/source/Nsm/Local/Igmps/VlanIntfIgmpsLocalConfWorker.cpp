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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Igmps/VlanIntfIgmpsLocalConfWorker.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsVlanLocalConfigMessage.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsSgVlanLocalConfigMessage.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsMrtrVlanLocalConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateMrtrVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateSgVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsUtils.h"
#include "Utils/ProvisioningUtil.h"

namespace DcmNs
{

    VlanIntfIgmpsLocalConfWorker::VlanIntfIgmpsLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        addOperationMap (NSMUPDATEIGMPSVLANLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&VlanIntfIgmpsLocalConfWorker::NsmUpdateIgmpsVlanLocalConfigMessageHandler));
        addOperationMap (NSMUPDATEIGMPSSGVLANLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&VlanIntfIgmpsLocalConfWorker::NsmUpdateIgmpsSgVlanLocalConfigMessageHandler));
        addOperationMap (NSMUPDATEIGMPSMRTRVLANLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&VlanIntfIgmpsLocalConfWorker::NsmUpdateIgmpsMrtrVlanLocalConfigMessageHandler));
    }

    VlanIntfIgmpsLocalConfWorker::~VlanIntfIgmpsLocalConfWorker ()
    {
    }

    PrismMessage  *VlanIntfIgmpsLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMUPDATEIGMPSVLANLOCALCONFIG :
                pPrismMessage = new NsmUpdateIgmpsVlanLocalConfigMessage ();
                break;
            case NSMUPDATEIGMPSSGVLANLOCALCONFIG :
                pPrismMessage = new NsmUpdateIgmpsSgVlanLocalConfigMessage ();
                break;
            case NSMUPDATEIGMPSMRTRVLANLOCALCONFIG :
                pPrismMessage = new NsmUpdateIgmpsMrtrVlanLocalConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *VlanIntfIgmpsLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId VlanIntfIgmpsLocalConfWorker::updateIgmpsMrtrVlanLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmUpdateIgmpsMrtrVlanLocalConfigMessage *pNsmUpdateIgmpsMrtrVlanLocalConfigMessage = dynamic_cast<NsmUpdateIgmpsMrtrVlanLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        int status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "VlanIntfIgmpsLocalConfWorker::updateIgmpsMrtrVlanLocalConfigStep: Entered....");

        if (!ProvisioningUtil::isVlanProvisioned(pNsmUpdateIgmpsMrtrVlanLocalConfigMessage->getVlanId())) {
            trace(TRACE_LEVEL_INFO,string("VlanIntfIgmpsLocalConfWorker::updateIgmpsMrtrVlanLocalConfigStep VLAN is not provisioned...  "));
            return status;
        }

        IgmpsClientUpdateMrtrVlanConfigMessage *m = new IgmpsClientUpdateMrtrVlanConfigMessage ();
        m->setVlanId(pNsmUpdateIgmpsMrtrVlanLocalConfigMessage->getVlanId());
        m->setIfType(pNsmUpdateIgmpsMrtrVlanLocalConfigMessage->getIfType());
        m->setIfName(pNsmUpdateIgmpsMrtrVlanLocalConfigMessage->getIfName());
        m->setNegation(pNsmUpdateIgmpsMrtrVlanLocalConfigMessage->getNegation());
	
        status = sendSynchronouslyToWaveClient ("IGMP", m);
        ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (pNsmUpdateIgmpsMrtrVlanLocalConfigMessage, m, status);
        delete m;
        return retStatus;
    }

    void  VlanIntfIgmpsLocalConfWorker::NsmUpdateIgmpsMrtrVlanLocalConfigMessageHandler( NsmUpdateIgmpsMrtrVlanLocalConfigMessage *pNsmUpdateIgmpsMrtrVlanLocalConfigMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::updateIgmpsMrtrVlanLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdateIgmpsMrtrVlanLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute();
    }

    ResourceId VlanIntfIgmpsLocalConfWorker::updateIgmpsSgVlanLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {
        NsmUpdateIgmpsSgVlanLocalConfigMessage *pNsmUpdateIgmpsSgVlanLocalConfigMessage = dynamic_cast<NsmUpdateIgmpsSgVlanLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        int status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "VlanIntfIgmpsLocalConfWorker::updateIgmpsSgVlanLocalConfigStep: Entered....");

        if (!ProvisioningUtil::isVlanProvisioned(pNsmUpdateIgmpsSgVlanLocalConfigMessage->getVlanId())) {
            trace(TRACE_LEVEL_INFO,string("VlanIntfIgmpsLocalConfWorker::updateIgmpsSgVlanLocalConfigStep VLAN is not provisioned...  "));
            return status;
        }

        IgmpsClientUpdateSgVlanConfigMessage *m = new IgmpsClientUpdateSgVlanConfigMessage ();
        m->setVlanId(pNsmUpdateIgmpsSgVlanLocalConfigMessage->getVlanId());
        m->setGroupAddress(pNsmUpdateIgmpsSgVlanLocalConfigMessage->getGroupAddress());
        m->setIfType(pNsmUpdateIgmpsSgVlanLocalConfigMessage->getIfType());
        m->setIfName(pNsmUpdateIgmpsSgVlanLocalConfigMessage->getIfName());
        m->setNegation(pNsmUpdateIgmpsSgVlanLocalConfigMessage->getNegation());
	
        status = sendSynchronouslyToWaveClient ("IGMP", m);
        ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (pNsmUpdateIgmpsSgVlanLocalConfigMessage, m, status);
        delete m;
        return retStatus;
    }

    void  VlanIntfIgmpsLocalConfWorker::NsmUpdateIgmpsSgVlanLocalConfigMessageHandler( NsmUpdateIgmpsSgVlanLocalConfigMessage *pNsmUpdateIgmpsSgVlanLocalConfigMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::updateIgmpsSgVlanLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdateIgmpsSgVlanLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId VlanIntfIgmpsLocalConfWorker::updateIgmpsVlanLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmUpdateIgmpsVlanLocalConfigMessage *pNsmUpdateIgmpsVlanLocalConfigMessage = dynamic_cast<NsmUpdateIgmpsVlanLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        int status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "VlanIntfIgmpsLocalConfWorker::updateIgmpsVlanLocalConfigStep: Entered....");

        if (!ProvisioningUtil::isVlanProvisioned(pNsmUpdateIgmpsVlanLocalConfigMessage->getVlanId())) {
            trace(TRACE_LEVEL_INFO,string("VlanIntfIgmpsLocalConfWorker::updateIgmpsVlanLocalConfigStep VLAN is not provisioned...  "));
            return status;
        }

        IgmpsClientUpdateVlanConfigMessage *m = new IgmpsClientUpdateVlanConfigMessage ();

		m->setLmqi(pNsmUpdateIgmpsVlanLocalConfigMessage->getLmqi());
		m->setQmrt(pNsmUpdateIgmpsVlanLocalConfigMessage->getQmrt());
		m->setEnable(pNsmUpdateIgmpsVlanLocalConfigMessage->getEnable());
		m->setFastLeave(pNsmUpdateIgmpsVlanLocalConfigMessage->getFastLeave());
		m->setQueryInterval(pNsmUpdateIgmpsVlanLocalConfigMessage->getQueryInterval());
		m->setQuerierEnable(pNsmUpdateIgmpsVlanLocalConfigMessage->getQuerierEnable());
		m->setMrouterTimeout(pNsmUpdateIgmpsVlanLocalConfigMessage->getMrouterTimeout());
		m->setVlanId(pNsmUpdateIgmpsVlanLocalConfigMessage->getVlanId());
		m->setVlanIfName(pNsmUpdateIgmpsVlanLocalConfigMessage->getVlanIfName());
		m->setVlanIfIndex(pNsmUpdateIgmpsVlanLocalConfigMessage->getVlanIfIndex());
		m->setOpCode(pNsmUpdateIgmpsVlanLocalConfigMessage->getOpCode());

        status = sendSynchronouslyToWaveClient ("IGMP", m);
        ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (pNsmUpdateIgmpsVlanLocalConfigMessage, m, status);
        delete m;
        return retStatus;
    }

    void  VlanIntfIgmpsLocalConfWorker::NsmUpdateIgmpsVlanLocalConfigMessageHandler( NsmUpdateIgmpsVlanLocalConfigMessage *pNsmUpdateIgmpsVlanLocalConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::updateIgmpsVlanLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VlanIntfIgmpsLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdateIgmpsVlanLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}
