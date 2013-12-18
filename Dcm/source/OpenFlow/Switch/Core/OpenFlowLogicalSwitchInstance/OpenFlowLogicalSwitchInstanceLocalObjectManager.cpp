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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchInstance/OpenFlowLogicalSwitchInstanceLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "OpenFlow/Switch/Utils/OpenFlowSwitchUtils.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchInstance/OpenFlowLogicalSwitchInstanceRepository.h"

using namespace WaveNs;

namespace OpenFlowNs
{

OpenFlowLogicalSwitchInstanceLocalObjectManager::OpenFlowLogicalSwitchInstanceLocalObjectManager (const string &openFlowLogicalSwitchName)
    : WaveLocalObjectManagerForUserSpecificTasks (getServiceNamePrefix () + " - " + openFlowLogicalSwitchName),
      m_openFlowLogicalSwitchName                (openFlowLogicalSwitchName)
{
    OpenFlowLogicalSwitchInstanceRepository::addOpenFlowLogicalSwitch (openFlowLogicalSwitchName, this);

    bootStrapSelf ();
}

OpenFlowLogicalSwitchInstanceLocalObjectManager::~OpenFlowLogicalSwitchInstanceLocalObjectManager ()
{
}

OpenFlowLogicalSwitchInstanceLocalObjectManager *OpenFlowLogicalSwitchInstanceLocalObjectManager:: createInstance (const string &openFlowLogicalSwitchName)
{
    OpenFlowLogicalSwitchInstanceLocalObjectManager *pOpenFlowLogicalSwitchInstanceLocalObjectManager = new OpenFlowLogicalSwitchInstanceLocalObjectManager (openFlowLogicalSwitchName);

    WaveNs::prismAssert (NULL != pOpenFlowLogicalSwitchInstanceLocalObjectManager, __FILE__, __LINE__);

    return (pOpenFlowLogicalSwitchInstanceLocalObjectManager);
}

void OpenFlowLogicalSwitchInstanceLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback            ();

    OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject = OpenFlowSwitchUtils::getOpenFlowLogicalSwitchManagedObjectByName (m_openFlowLogicalSwitchName);

    prismAssert (NULL != pOpenFlowLogicalSwitchManagedObject, __FILE__, __LINE__);

    pOpenFlowLogicalSwitchManagedObject->connectToKnownOpenFlowControllers ();
}

}

