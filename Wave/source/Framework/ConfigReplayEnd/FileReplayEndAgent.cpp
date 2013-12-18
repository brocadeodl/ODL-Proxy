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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Framework/ConfigReplayEnd/FileReplayEndAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/ConfigReplayEnd/FileReplayEndAgentContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

FileReplayEndAgent::FileReplayEndAgent (WaveObjectManager *pWaveObjectManager) 
    : WaveWorker (pWaveObjectManager) 
{
}

FileReplayEndAgent::~FileReplayEndAgent ()
{
}

ResourceId FileReplayEndAgent::execute ()
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileReplayEndAgent::getListOfEnabledServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileReplayEndAgent::sendFileReplayEndStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileReplayEndAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileReplayEndAgent::prismSynchronousLinearSequencerFailedStep)
    };

    FileReplayEndAgentContext *pFileReplayEndAgentContext = new FileReplayEndAgentContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pFileReplayEndAgentContext->execute ();

    return (status);
}

ResourceId FileReplayEndAgent::getListOfEnabledServicesStep (FileReplayEndAgentContext *pFileReplayEndAgentContext)
{
    vector<PrismServiceId> &enabledServices = pFileReplayEndAgentContext->getEnabledServices ();

    WaveObjectManager::getListOfEnabledServices (enabledServices);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId FileReplayEndAgent::sendFileReplayEndStep (FileReplayEndAgentContext *pFileReplayEndAgentContext)
{
    vector<PrismServiceId> &serviceIdsToSendFileReplayEnd = pFileReplayEndAgentContext->getEnabledServices ();
    UI32                    i                    = 0;
    UI32                    numberOfServices     = 0;

    numberOfServices = serviceIdsToSendFileReplayEnd.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (requiresFileReplayEndNotification (serviceIdsToSendFileReplayEnd[i])))
        {
            continue;
        }

        PrismFileReplayEndObjectManagerMessage *prismFileReplayEndObjectManagerMessage = new PrismFileReplayEndObjectManagerMessage (serviceIdsToSendFileReplayEnd[i]);

        ResourceId status = sendSynchronously (prismFileReplayEndObjectManagerMessage);

        delete prismFileReplayEndObjectManagerMessage;

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFileReplayEndAgent::sendFileReplayEndStep: Could not send File Replay End to a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToSendFileReplayEnd[i]));
            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Successfully sent FileReplayEnd " + FrameworkToolKit::getServiceNameById (serviceIdsToSendFileReplayEnd[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

bool FileReplayEndAgent::requiresFileReplayEndNotification(const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId               ()) == prismServiceId) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

}
