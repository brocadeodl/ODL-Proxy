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
 *   Author : suw                                                     *
 **************************************************************************/
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/User/IpPolicyLocalWorkers.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "IpPolicy/Framework/PBRShowMessage.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmResourceIds.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    IpPolicyLocalObjectManager::IpPolicyLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pIpPolicyLocalWorker = new IpPolicyLocalWorker (this);
        prismAssert (NULL != m_pIpPolicyLocalWorker, __FILE__, __LINE__);
    }

    IpPolicyLocalObjectManager::~IpPolicyLocalObjectManager ()
    {
    }

    IpPolicyLocalObjectManager  *IpPolicyLocalObjectManager::getInstance()
    {
        static IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager = new IpPolicyLocalObjectManager ();

        WaveNs::prismAssert (NULL != pIpPolicyLocalObjectManager, __FILE__, __LINE__);

        return (pIpPolicyLocalObjectManager);
    }

    string  IpPolicyLocalObjectManager::getClassName()
    {
        return ("IpPolicyLocal");
    }

    PrismServiceId  IpPolicyLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *IpPolicyLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
		  	case PBR_SHOW_ROUTEMAP :
			  	pPrismMessage = new PBRShowMessage ();
				break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IpPolicyLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void IpPolicyLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {

        string name = pWaveAsynchronousContextForPostBootPhase->getPassName();

        trace(TRACE_LEVEL_INFO, "Ip Policy Postboot Entered with pass: " + name);
        // Check for Layer3 license before sending to backend
        if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
            trace(TRACE_LEVEL_DEBUG, "IpPolicyLocalObjectManager::postboot. LAYER_3_LICENSE not present.");
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE5") {
		  		m_pIpPolicyLocalWorker->restoreRouteMapMO();
				m_pIpPolicyLocalWorker->ConfigReplayPbrGlobalIntf(IF_TYPE_VL);
				m_pIpPolicyLocalWorker->ConfigReplayPbrGlobalIntf(IF_TYPE_PO);
                m_pIpPolicyLocalWorker->restoreIpPrefixMO();
                m_pIpPolicyLocalWorker->restoreAsPathAccessListMO();
                m_pIpPolicyLocalWorker->restoreIpCommunityStdAccessListMO();
                m_pIpPolicyLocalWorker->restoreIpCommunityExtAccessListMO();
                m_pIpPolicyLocalWorker->restoreFilterChangeUpdateDelayMO();
        }
        else if (name == "DCM_POSTBOOT_SLOT_STAGE2") {
			 m_pIpPolicyLocalWorker->restoreRouteMapMO(pWaveAsynchronousContextForPostBootPhase->getSlotNum());
			 m_pIpPolicyLocalWorker->restorePBRMO(pWaveAsynchronousContextForPostBootPhase->getSlotNum());
        }
        else {
            //
        }

        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
        return;
    }

}
