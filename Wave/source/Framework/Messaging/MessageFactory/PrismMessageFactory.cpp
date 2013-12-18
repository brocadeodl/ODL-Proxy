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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/MessageFactory/PrismMessageFactory.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

namespace WaveNs
{

PrismMessage *PrismMessageFactory::getMessageInstance (const PrismServiceId &serviceId, const UI32 &opcode)
{
    WaveObjectManager *pWaveObjectManager = getWaveObjectManagerForMessage (serviceId, opcode);
    PrismMessage       *pPrismMessage       = NULL;

    if (NULL != pWaveObjectManager)
    {
        pPrismMessage = pWaveObjectManager->createMessageInstanceWrapper (opcode);
    }

    if (NULL == pPrismMessage)
    {
        trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getMessageInstance : Could not create message instance for  Service : " + FrameworkToolKit::getServiceNameById (serviceId) + ", Opcode = " + opcode + ".");
        trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getMessageInstance : Have you implemented the createMessageInstance on the object manager yet???.");
        trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getMessageInstance : Also, Please make sure that the opcode has been registered using addOperationMap on the corresponoding Object Manager.");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismMessage);
}

WaveObjectManager *PrismMessageFactory::getWaveObjectManagerForMessage (const PrismServiceId &serviceId, const UI32 &opcode)
{
    PrismThread        *pPrismThread        = NULL;
    WaveObjectManager *pWaveObjectManager = NULL;

    pPrismThread = PrismThread::getPrismThreadForServiceId (serviceId);

    if (NULL != pPrismThread)
    {
        pWaveObjectManager = pPrismThread->getWaveObjectManagerForOperationCode (opcode);
    }

    return (pWaveObjectManager);
}

WaveObjectManager *PrismMessageFactory::getWaveObjectManagerForEvent (const PrismServiceId &serviceId, const UI32 &eventOpcode)
{
    PrismThread        *pPrismThread        = NULL;
    WaveObjectManager *pWaveObjectManager = NULL;

    pPrismThread = PrismThread::getPrismThreadForServiceId (serviceId);

    if (NULL != pPrismThread)
    {
        pWaveObjectManager = pPrismThread->getWaveObjectManagerForEventOperationCode (eventOpcode);
    }

    return (pWaveObjectManager);
}

WaveObjectManager *PrismMessageFactory::getWaveObjectManagerForEventListener (const LocationId &locationId, const PrismServiceId &serviceId, const UI32 &eventOpcode)
{
    PrismThread        *pPrismThread        = NULL;
    WaveObjectManager *pWaveObjectManager = NULL;

    pPrismThread = PrismThread::getPrismThreadForServiceId (serviceId);

    if (NULL != pPrismThread)
    {
        pWaveObjectManager = pPrismThread->getWaveObjectManagerForEventOperationCodeForListening (locationId, serviceId, eventOpcode);
    }

    return (pWaveObjectManager);
}

PrismMessage *PrismMessageFactory::getManagementInterfaceMessageInstance (const string &waveClientName, const PrismServiceId &serviceId, const UI32 &opcode)
{
    if (WAVE_MGMT_INTF_ROLE_CLIENT == (FrameworkToolKit::getManagementInterfaceRole ()))
    {
        return (getMessageInstance (serviceId, opcode));
    }

    ManagementInterfaceMessageInstantiatorForClient  managementInterfaceMessageInstantiatorForClient = WaveUserInterfaceObjectManager::getManagementInterfaceMessageInstantiatorForClient (waveClientName);
    PrismMessage                                    *pPrismMessage                                   = NULL;

    if (NULL != managementInterfaceMessageInstantiatorForClient)
    {
        pPrismMessage = (*managementInterfaceMessageInstantiatorForClient) (opcode);

        if (NULL == pPrismMessage)
        {
            trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getManagementInterfaceMessageInstance : Management Interface Message Instantiator for client \'" + waveClientName + "\' Returned NULL.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            ManagementInterfaceMessage *pManagementInterfaceMessage = dynamic_cast<ManagementInterfaceMessage *> (pPrismMessage);

            if (NULL == pManagementInterfaceMessage)
            {
                trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getManagementInterfaceMessageInstance : Only Management Interface Derivations can be used in messaging with Wave Clients.");
                prismAssert (false, __FILE__, __LINE__);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "PrismMessageFactory::getManagementInterfaceMessageInstance : Management Interface Message Instantiator has not bee set for client \'" + waveClientName + "\'.");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pPrismMessage);
}

}
