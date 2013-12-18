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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkObjectManagerServiceControlWorker.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"

namespace WaveNs
{

PrismFrameworkObjectManagerServiceControlWorker::PrismFrameworkObjectManagerServiceControlWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_SERVICE_CONTROL_LIST_MESSAGE,     reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerServiceControlWorker::serviceControlListMessageHandler));
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_SERVICE_SET_CPU_AFFINITY_MESSAGE, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerServiceControlWorker::serviceSetCpuAffinityMessageHandler));
}

PrismFrameworkObjectManagerServiceControlWorker::~PrismFrameworkObjectManagerServiceControlWorker ()
{
}

PrismMessage *PrismFrameworkObjectManagerServiceControlWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case FRAMEWORK_OBJECT_MANAGER_SERVICE_CONTROL_LIST_MESSAGE :
            pPrismMessage = new FrameworkObjectManagerServiceControlListMessage;
            break;

        case FRAMEWORK_OBJECT_MANAGER_SERVICE_SET_CPU_AFFINITY_MESSAGE :
            pPrismMessage = new FrameworkObjectManagerServiceSetCpuAffinityMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void PrismFrameworkObjectManagerServiceControlWorker::serviceControlListMessageHandler (FrameworkObjectManagerServiceControlListMessage *pFrameworkObjectManagerServiceControlListMessage)
{
    vector<PrismServiceId> prismServiceIds;
    UI32                   numberOfPrismServices;
    UI32                   i;
    PrismServiceId         serviceId              = 0;
    string                 serviceName;
    bool                   isEnabled              = false;
    bool                   isLocal                = false;
    vector<UI32>           cpuAffinityVector;
    string                 cpuAffinity;
    UI32                   numberOfCpus           = 0;
    UI32                   j                      = 0;

    PrismThread::getListOfServiceIds (prismServiceIds);
    numberOfPrismServices = prismServiceIds.size ();

    for (i = 0; i < numberOfPrismServices; i++)
    {
        serviceId         = prismServiceIds[i];
        serviceName       = FrameworkToolKit::getServiceNameById (serviceId);
        isEnabled         = WaveObjectManager::isServiceEnabled (serviceId);
        isLocal           = FrameworkToolKit::isALocalService (serviceId);
        cpuAffinityVector = PrismThread::getCpuAffinityVectorForServiceId (serviceId);

        cpuAffinity  = "";
        numberOfCpus = cpuAffinityVector.size ();

        for (j = 0; j < numberOfCpus; j++)
        {
            cpuAffinity = cpuAffinity + cpuAffinityVector[j] + string (":");
        }

        pFrameworkObjectManagerServiceControlListMessage->addAService (serviceId, serviceName, isEnabled, isLocal, cpuAffinity);
    }

    pFrameworkObjectManagerServiceControlListMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pFrameworkObjectManagerServiceControlListMessage);
}

void PrismFrameworkObjectManagerServiceControlWorker::serviceSetCpuAffinityMessageHandler (FrameworkObjectManagerServiceSetCpuAffinityMessage *pFrameworkObjectManagerServiceSetCpuAffinityMessage)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManagerServiceControlWorker::serviceSetCpuAffinityMessageHandler : Entering ...");

          PrismServiceId                           prismServiceId                          = pFrameworkObjectManagerServiceSetCpuAffinityMessage->getPrismServiceId ();
          vector<UI32>                             cpuAffinityVector                       = pFrameworkObjectManagerServiceSetCpuAffinityMessage->getCpuAffinityVector ();
          PrismSetCpuAffinityObjectManagerMessage  prismSetCpuAffinityObjectManagerMessage   (prismServiceId);
          ResourceId                               status                                  = WAVE_MESSAGE_SUCCESS;
    const PrismServiceId                           thisServiceId                           = getServiceId ();

    if (thisServiceId != prismServiceId)
    {
        prismSetCpuAffinityObjectManagerMessage.setCpuAffinityVector (cpuAffinityVector);

        status = sendSynchronously (&prismSetCpuAffinityObjectManagerMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = prismSetCpuAffinityObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerServiceControlWorker::serviceSetCpuAffinityMessageHandler : Failed to set Cpu Affinity for service \'" + FrameworkToolKit::getServiceNameById (prismServiceId) + "\'.  Status : " + FrameworkToolKit::localize (status));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PrismFrameworkObjectManagerServiceControlWorker::serviceSetCpuAffinityMessageHandler : Failed to send message.  Status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        setCpuAffinity (cpuAffinityVector);
    }

    pFrameworkObjectManagerServiceSetCpuAffinityMessage->setCompletionStatus (status);
    reply (pFrameworkObjectManagerServiceSetCpuAffinityMessage);
}

}
