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
 *   Author : jrodrigu                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "SwitchConfig/Local/SwitchConfigLocalObjectManager.h"
#include "SwitchConfig/Local/SwitchConfigCfgChassisStateMessage.h"
#include "SwitchConfig/Local/SwitchConfigTypes.h"

//#include "DcmCore/DcmToolKit.h"
#include "switch/switch.h"

namespace DcmNs
{

    SwitchConfigLocalObjectManager::SwitchConfigLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (SWITCHCONFIGCFGCHASSISSTATE, reinterpret_cast<PrismMessageHandler> (&SwitchConfigLocalObjectManager::SwitchConfigCfgChassisStateMessageHandler));
    }

    SwitchConfigLocalObjectManager::~SwitchConfigLocalObjectManager ()
    {
    }

    SwitchConfigLocalObjectManager  *SwitchConfigLocalObjectManager::getInstance()
    {
        static SwitchConfigLocalObjectManager *pSwitchConfigLocalObjectManager = new SwitchConfigLocalObjectManager ();

        WaveNs::prismAssert (NULL != pSwitchConfigLocalObjectManager, __FILE__, __LINE__);

        return (pSwitchConfigLocalObjectManager);
    }

    string  SwitchConfigLocalObjectManager::getClassName()
    {
        return ("SwitchConfig");
    }

    PrismServiceId  SwitchConfigLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SwitchConfigLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SWITCHCONFIGCFGCHASSISSTATE :
                pPrismMessage = new SwitchConfigCfgChassisStateMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SwitchConfigLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

/***********************************************
 enable/disable chassis Handler & API
***********************************************/
	ResourceId SwitchConfigLocalObjectManager::cfgChassisState(PrismSynchronousLinearSequencerContext *pSwitchConfigCfgChassisStateMessageContext)
	{
		int rc;
		SwitchConfigCfgChassisStateMessage *pSwitchConfigCfgChassisStateMessage =
		 reinterpret_cast<SwitchConfigCfgChassisStateMessage *> (pSwitchConfigCfgChassisStateMessageContext->getPPrismMessage ());
		trace (TRACE_LEVEL_INFO, "************* SwitchConfig Plugin: cfg_local_switch_state- entering...");

		bool enableChassis;
		bool beacon;

		beacon = pSwitchConfigCfgChassisStateMessage->getBeacon();
		enableChassis = pSwitchConfigCfgChassisStateMessage->getChassisEnableFlag();
		if (beacon) {
			if (enableChassis) {
				trace (TRACE_LEVEL_INFO, "************* SwitchConfig Plugin: calling chassisBeaconSet(1) API...");
				chassisBeaconSet(1);
			} else {
				trace (TRACE_LEVEL_INFO, "************* SwitchConfig Plugin: calling chassisBeaconSet(0) API...");
				chassisBeaconSet(0);
			}
		} else {
			if (enableChassis) {
				rc = chassisenable();
				pSwitchConfigCfgChassisStateMessage->setResult(rc);	
				trace(TRACE_LEVEL_INFO, string("switchconfig enable rc ") + rc);
			} else {
				rc = chassisdisable();
				pSwitchConfigCfgChassisStateMessage->setResult(rc);
				trace(TRACE_LEVEL_INFO, string("switchconfig disable rc ") + rc); 
			}
		}

		return (WAVE_MESSAGE_SUCCESS);
	}

    void  SwitchConfigLocalObjectManager::SwitchConfigCfgChassisStateMessageHandler( SwitchConfigCfgChassisStateMessage *pSwitchConfigCfgChassisStateMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SwitchConfigLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SwitchConfigLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SwitchConfigLocalObjectManager::cfgChassisState),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SwitchConfigLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SwitchConfigLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSwitchConfigCfgChassisStateMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}
