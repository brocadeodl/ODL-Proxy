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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Kausik Sridharabalan                                         *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddFlowWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "OpenFlow/Switch/Core/CreateOpenFlowLogicalSwitchContext.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddFlowMessage.h"
//#include "SlmClientMessaging/FtmAddFlowClientManagementInterfaceMessage.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "DcmResourceIdEnums.h"

using namespace DcmNs;
using namespace DcmNs;

namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerAddFlowWorker::OpenFlowSwitchObjectManagerAddFlowWorker (OpenFlowSwitchObjectManager *pOpenFlowSwitchObjectManager)
    : WaveWorker (pOpenFlowSwitchObjectManager)
{
    // There is no need to for any Managed Object here. We dont persist anything in this case

//    OpenFlowLogicalSwitchManagedObject openFlowLogicalSwitchManagedObject (pOpenFlowSwitchObjectManager);

//    openFlowLogicalSwitchManagedObject.setupOrm ();

//    addManagedClass (OpenFlowLogicalSwitchManagedObject::getClassName (), this);

    addOperationMap (OPEN_FLOW_OBJECT_MANAGER_ADD_FLOW_MESSAGE, reinterpret_cast<PrismMessageHandler> (&OpenFlowSwitchObjectManagerAddFlowWorker::OpenFlowSwitchObjectManagerAddFlowMessageHandler));

}

OpenFlowSwitchObjectManagerAddFlowWorker::~OpenFlowSwitchObjectManagerAddFlowWorker ()
{
}

PrismMessage *OpenFlowSwitchObjectManagerAddFlowWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case OPEN_FLOW_OBJECT_MANAGER_ADD_FLOW_MESSAGE :
            pPrismMessage = new OpenFlowSwitchObjectManagerAddFlowMessage;
            break;

        default :
            pPrismMessage = NULL;
            break;
    }

    return (pPrismMessage);
}

void OpenFlowSwitchObjectManagerAddFlowWorker::OpenFlowSwitchObjectManagerAddFlowMessageHandler (OpenFlowSwitchObjectManagerAddFlowMessage *pOpenFlowSwitchObjectManagerAddFlowMessage)
{
    trace (TRACE_LEVEL_INFO, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::testFtmMessageHandler : Entering ...");

#if 0
    // Create new FTM client message and send to client

    FtmAddFlowClientManagementInterfaceMessage *pFtmAddFlowClientMessage = new FtmAddFlowClientManagementInterfaceMessage(FTM_ADD_FLOW);


    pFtmAddFlowClientMessage->setFtmAddFlowIngressIfIndex (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowIngressIfIndex ());
    pFtmAddFlowClientMessage->setFtmAddFlowEgressIfIndex (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowEgressIfIndex ());
    pFtmAddFlowClientMessage->setFtmAddFlowSrcIpV4Address (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowSrcIpV4Address ());
    pFtmAddFlowClientMessage->setFtmAddFlowSrcIpV4AddressMask (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowSrcIpV4AddressMask ());
    pFtmAddFlowClientMessage->setFtmAddFlowDstIpV4Address (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowDstIpV4Address ());
    pFtmAddFlowClientMessage->setFtmAddFlowDstIpV4AddressMask (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowDstIpV4AddressMask ());
    pFtmAddFlowClientMessage->setFtmAddFlowAppType (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowAppType ());
    pFtmAddFlowClientMessage->setFtmAddFlowSrcPort (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowSrcPort ());
    pFtmAddFlowClientMessage->setFtmAddFlowDstPort (pOpenFlowSwitchObjectManagerAddFlowMessage->getOpenFlowDstPort ());


    ResourceId status = sendSynchronouslyToWaveClient ("ftm", pFtmAddFlowClientMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "OpenFlowSwitchObjectManagerAddFlowMessageHandler send failed : Status" + FrameworkToolKit::localize (status));
    }
    else
    {
        status = pFtmAddFlowClientMessage->getCompletionStatus ();
    }

    delete pFtmAddFlowClientMessage;
#endif

    pOpenFlowSwitchObjectManagerAddFlowMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pOpenFlowSwitchObjectManagerAddFlowMessage);
}

}

