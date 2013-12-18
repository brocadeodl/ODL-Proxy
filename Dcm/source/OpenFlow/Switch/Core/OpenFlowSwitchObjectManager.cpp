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

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddFlowWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker.h"

namespace OpenFlowNs
{

OpenFlowSwitchObjectManager::OpenFlowSwitchObjectManager ()
    : WaveObjectManager (getServiceName ())
{
    m_pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker = new OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker (this);

    prismAssert (NULL != m_pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker, __FILE__, __LINE__);

    m_pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker = new OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker (this);

    prismAssert (NULL != m_pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker, __FILE__, __LINE__);

    m_pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker = new OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker (this);

    prismAssert (NULL != m_pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker, __FILE__, __LINE__);

    m_pOpenFlowSwitchObjectManagerAddFlowWorker = new OpenFlowSwitchObjectManagerAddFlowWorker (this);

    prismAssert (NULL != m_pOpenFlowSwitchObjectManagerAddFlowWorker, __FILE__, __LINE__);

    m_pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker = new OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker (this);

    prismAssert (NULL != m_pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker, __FILE__, __LINE__);
}

OpenFlowSwitchObjectManager::~OpenFlowSwitchObjectManager ()
{
    if (NULL != m_pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker)
    {
        delete m_pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker;
    }

    if (NULL != m_pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker)
    {
        delete m_pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker;
    }

    if (NULL != m_pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker)
    {
        delete m_pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker;
    }

    if (NULL != m_pOpenFlowSwitchObjectManagerAddFlowWorker)
    {
        delete m_pOpenFlowSwitchObjectManagerAddFlowWorker;
    }

    if (NULL != m_pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker)
    {
        delete m_pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker;
    }
}

OpenFlowSwitchObjectManager *OpenFlowSwitchObjectManager:: getInstance ()
{
    static OpenFlowSwitchObjectManager *pOpenFlowSwitchObjectManager = new OpenFlowSwitchObjectManager ();

    WaveNs::prismAssert (NULL != pOpenFlowSwitchObjectManager, __FILE__, __LINE__);

    return (pOpenFlowSwitchObjectManager);
}

PrismServiceId OpenFlowSwitchObjectManager:: getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

}

