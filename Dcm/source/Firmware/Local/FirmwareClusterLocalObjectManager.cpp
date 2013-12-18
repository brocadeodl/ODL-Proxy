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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "errno.h"

#include "Firmware/Local/FirmwareClusterLocalObjectManager.h"
#include "Firmware/Local/FirmwareClusterLocalMessage.h"
#include "Firmware/Local/FirmwareClusterTypes.h"
#include "Firmware/Local/FirmwareClusterLocalThread.h"


namespace DcmNs
{

    FirmwareClusterLocalObjectManager::FirmwareClusterLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (FIRMWARECLUSTERLOCAL, reinterpret_cast<PrismMessageHandler> (&FirmwareClusterLocalObjectManager::FirmwareClusterLocalMessageHandler));
        addOperationMap (NETCONFSTATUS, reinterpret_cast<PrismMessageHandler> (&FirmwareClusterLocalObjectManager::NetconfStatusMessageHandler));
    }

    FirmwareClusterLocalObjectManager::~FirmwareClusterLocalObjectManager ()
    {
    }

    FirmwareClusterLocalObjectManager  *FirmwareClusterLocalObjectManager::getInstance()
    {
        static FirmwareClusterLocalObjectManager *pFirmwareClusterLocalObjectManager = new FirmwareClusterLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFirmwareClusterLocalObjectManager, __FILE__, __LINE__);

        return (pFirmwareClusterLocalObjectManager);
    }

    string  FirmwareClusterLocalObjectManager::getClassName()
    {
        return ("FirmwareCluster");
    }

    PrismServiceId  FirmwareClusterLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FirmwareClusterLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FIRMWARECLUSTERLOCAL :
                pPrismMessage = new FirmwareClusterLocalMessage();
                break;
            case NETCONFSTATUS :
                pPrismMessage = new NetconfStatusMessage();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FirmwareClusterLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  FirmwareClusterLocalObjectManager::FirmwareClusterLocalMessageHandler(FirmwareClusterLocalMessage *pFirmwareClusterLocalMessage)
	{

		NetconfStatusMessage *pNetconfStatusMessage = NULL;

		FirmwareClusterLocalThread *pFirmwareClusterLocalThread = new FirmwareClusterLocalThread(m_pWaveObjectManager, pFirmwareClusterLocalMessage, pNetconfStatusMessage, FIRMWARECLUSTERLOCAL);
		WaveThreadStatus threadStatus = pFirmwareClusterLocalThread->run();

		if (WAVE_THREAD_SUCCESS != threadStatus) {
			trace(TRACE_LEVEL_ERROR, "FirmwareClusterLocalOM::FirmwareCluster"
				"LocalMessageHandler: Local thread failed with status: " +
				FrameworkToolKit::localize(threadStatus));
		} else {
			trace(TRACE_LEVEL_SUCCESS, "FirmwareClusterLocalOM::FirmwareClust"
				"erLocalMessageHandler: Local thread started successfully");
		}

	}

    void  FirmwareClusterLocalObjectManager::NetconfStatusMessageHandler(NetconfStatusMessage *pNetconfStatusMessage)
	{

		FirmwareClusterLocalMessage *pFirmwareClusterLocalMessage = NULL;

		FirmwareClusterLocalThread *pFirmwareClusterLocalThread = new FirmwareClusterLocalThread(m_pWaveObjectManager, pFirmwareClusterLocalMessage, pNetconfStatusMessage, NETCONFSTATUS);
		WaveThreadStatus threadStatus = pFirmwareClusterLocalThread->run();

		if (WAVE_THREAD_SUCCESS != threadStatus) {
			trace(TRACE_LEVEL_ERROR, "FirmwareClusterLocalOM::NetconfStatus"
				"MessageHandler: Local thread failed with status: " +
				FrameworkToolKit::localize(threadStatus));
		} else {
			trace(TRACE_LEVEL_SUCCESS, "FirmwareClusterLocalOM::NetconfStatus"
				"MessageHandler: Local thread started successfully");
		}

	}

}
