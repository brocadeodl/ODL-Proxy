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
#include "VRRP/Local/VRRPLocalObjectManager.h"
#include "VRRP/Local/VRRPLocalConfigWorker.h"
#include "VRRP/Local/VRRPUpdateVRRPConfigMessage.h"
#include "VRRP/Local/VRRPTypes.h"
#include "ClientInterface/VRRP/VRRPGlobalConfigSPMessage.h"
#include "VRRP/Local/VRRPGlobalConfigManagedObject.h"
#include "Nsm/Local/VRRPWorker/NsmUpdateVRRPLocalConfigPhyIntfMessage.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmResourceIds.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "Utils/Capabilities.h"

namespace DcmNs
{

    VRRPLocalConfigWorker::VRRPLocalConfigWorker ( VRRPLocalObjectManager *pVRRPLocalObjectManager)
        : WaveWorker  (pVRRPLocalObjectManager)
    {
        VRRPGlobalConfigManagedObject VRRPGlobalConfigManagedObject (pVRRPLocalObjectManager);
        VRRPGlobalConfigManagedObject.setupOrm ();
        addManagedClass (VRRPGlobalConfigManagedObject::getClassName (), this);
        addOperationMap (VRRPUPDATEVRRPCONFIG, reinterpret_cast<PrismMessageHandler> (&VRRPLocalConfigWorker::VRRPUpdateVRRPConfigMessageHandler));
    }

    VRRPLocalConfigWorker::~VRRPLocalConfigWorker ()
    {
    }

    PrismMessage  *VRRPLocalConfigWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case VRRPUPDATEVRRPCONFIG :
                pPrismMessage = new VRRPUpdateVRRPConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *VRRPLocalConfigWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
        if ((VRRPGlobalConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VRRPGlobalConfigManagedObject(dynamic_cast<VRRPLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VRRPLocalConfigWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId VRRPLocalConfigWorker::createVRRPLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        VRRPUpdateVRRPConfigMessage *pVRRPUpdateVRRPConfigMessage = dynamic_cast<VRRPUpdateVRRPConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfigWorker::createVRRPLocalConfigStep Entered....");
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();

        ResourceId status;
        VRRPGlobalConfigSPMessage *m = new VRRPGlobalConfigSPMessage();

        m->setConfigDisable(pVRRPUpdateVRRPConfigMessage->getConfigDisable());
        m->setVrrpEnable(pVRRPUpdateVRRPConfigMessage->getVrrpEnable());
        m->setVrrpeEnable(pVRRPUpdateVRRPConfigMessage->getVrrpeEnable());

        status = sendSynchronouslyToWaveClient ("vrrp", m);

        if (status == WAVE_MESSAGE_SUCCESS) {
            if ((status = m->getCompletionStatus()) != WAVE_MESSAGE_SUCCESS) {
            	trace(TRACE_LEVEL_ERROR, "VRRPLocalConfigWorker::createVRRPLocalConfigStep: Failure in communication with client: " + FrameworkToolKit::localize (status));
            } else {
                status = m->getClientStatus();
            }
        }

        pVRRPUpdateVRRPConfigMessage->setCompletionStatus(status);

		if (m)
			delete (m);
        
		return status;
    }

    ResourceId VRRPLocalConfigWorker::updateVRRPInterfaceConfig(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        VRRPUpdateVRRPConfigMessage *pVRRPUpdateVRRPConfigMessage = dynamic_cast<VRRPUpdateVRRPConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = new NsmUpdateVRRPLocalConfigPhyIntfMessage();

		if (pVRRPUpdateVRRPConfigMessage->getConfigDisable()) {
			if (pVRRPUpdateVRRPConfigMessage->getVrrpEnable()) {
				pNsmUpdateVRRPLocalConfigPhyIntfMessage->setChoice(VRRP_GLOBAL_CMD_DISABLE);
			} else if (pVRRPUpdateVRRPConfigMessage->getVrrpeEnable()) {
				pNsmUpdateVRRPLocalConfigPhyIntfMessage->setChoice(VRRPE_GLOBAL_CMD_DISABLE);
			}

        	if ((status = sendSynchronously(pNsmUpdateVRRPLocalConfigPhyIntfMessage)) == WAVE_MESSAGE_SUCCESS)
        	{
            	trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfigWorker::updateVRRPInterfaceConfig global config message sent successfully");
            	status = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getCompletionStatus();
        	}
		}

        pVRRPUpdateVRRPConfigMessage->setCompletionStatus(status);
        delete (pNsmUpdateVRRPLocalConfigPhyIntfMessage);
        return (status);
    }

    ResourceId VRRPLocalConfigWorker::createVRRPGlobalConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int createFlag = 1;
        //int status = 0;
        LocationId locationId = FrameworkToolKit::getThisLocationId ();

        trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfigWorker::createVRRPGlobalConfigMOStep Entered....");
        VRRPGlobalConfigManagedObject *pVRRPGlobalConfigManagedObject = NULL;
        VRRPUpdateVRRPConfigMessage *pVRRPUpdateVRRPConfigMessage = dynamic_cast<VRRPUpdateVRRPConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, VRRPGlobalConfigManagedObject::getClassName());

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "numberOfResults = 0");
                createFlag = 1;
                //status = WAVE_MESSAGE_SUCCESS;
            } else if (1 == numberOfResults) {
                pVRRPGlobalConfigManagedObject = dynamic_cast<VRRPGlobalConfigManagedObject*>((*pResults)[0]);
                trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
                createFlag = 0;
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
                //status = WAVE_MESSAGE_ERROR;
            }
            pResults->clear ();
            delete pResults;
        }
        //querySynchronouslyLocalManagedObjectsForLocationId cannot be used inside a transaction. So, Using start and commit 
        //transactions instead of transaction steps.
        startTransaction();
        if (createFlag == 0) {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
            updateWaveManagedObject (pVRRPGlobalConfigManagedObject);
            prismAssert (NULL != pVRRPGlobalConfigManagedObject, __FILE__, __LINE__);
        } else {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
            pVRRPGlobalConfigManagedObject = new VRRPGlobalConfigManagedObject(dynamic_cast<VRRPLocalObjectManager *>(getPWaveObjectManager()));
            prismAssert (NULL != pVRRPGlobalConfigManagedObject, __FILE__, __LINE__);
        }

		if (pVRRPUpdateVRRPConfigMessage->getConfigDisable() == false) {
			if (pVRRPUpdateVRRPConfigMessage->getVrrpEnable() == true) {
				pVRRPGlobalConfigManagedObject->setVrrpEnable(true);
				if (Capabilities::instance()->isCapabilityPresent(VRRP_CAPABILITY_MULTI_MODE)) {
					pVRRPGlobalConfigManagedObject->setVrrpeEnable(true);
				}
			}
			
			if (pVRRPUpdateVRRPConfigMessage->getVrrpeEnable() == true) {
				pVRRPGlobalConfigManagedObject->setVrrpeEnable(true);
			}
		} else {
			if (pVRRPUpdateVRRPConfigMessage->getVrrpEnable()) {
				pVRRPGlobalConfigManagedObject->setVrrpEnable(false);
			}
			if (pVRRPUpdateVRRPConfigMessage->getVrrpeEnable()) {
				pVRRPGlobalConfigManagedObject->setVrrpeEnable(false);
			}
		}


        commitTransaction();
        if (pVRRPGlobalConfigManagedObject) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pVRRPGlobalConfigManagedObject);
        }
        return pVRRPUpdateVRRPConfigMessage->getCompletionStatus ();
    }

    void  VRRPLocalConfigWorker::VRRPUpdateVRRPConfigMessageHandler( VRRPUpdateVRRPConfigMessage *pVRRPUpdateVRRPConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfigWorker::createVRRPLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfigWorker::updateVRRPInterfaceConfig),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfigWorker::createVRRPGlobalConfigMOStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pVRRPUpdateVRRPConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}
