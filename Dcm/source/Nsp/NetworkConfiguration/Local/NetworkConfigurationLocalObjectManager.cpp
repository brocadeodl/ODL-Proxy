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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationCreateNetworkLocalWorker.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationDeleteNetworkLocalWorker.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationAttachVirtualInterfaceLocalWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "DcmCore/DcmToolKit.h"

namespace NspNs
{

NetworkConfigurationLocalObjectManager::NetworkConfigurationLocalObjectManager ()
    : WaveLocalObjectManager            (getClassName ()),
      m_pValClientSynchronousConnection (NULL)
{
    m_pNetworkConfigurationCreateNetworkLocalWorker = new NetworkConfigurationCreateNetworkLocalWorker (this);

    prismAssert (NULL != m_pNetworkConfigurationCreateNetworkLocalWorker, __FILE__, __LINE__);

    m_pNetworkConfigurationDeleteNetworkLocalWorker = new NetworkConfigurationDeleteNetworkLocalWorker (this);

    prismAssert (NULL != m_pNetworkConfigurationDeleteNetworkLocalWorker, __FILE__, __LINE__);

    m_pNetworkConfigurationAttachVirtualInterfaceLocalWorker = new NetworkConfigurationAttachVirtualInterfaceLocalWorker (this);

    prismAssert (NULL != m_pNetworkConfigurationAttachVirtualInterfaceLocalWorker, __FILE__, __LINE__);
}

NetworkConfigurationLocalObjectManager::~NetworkConfigurationLocalObjectManager ()
{
    if (NULL != m_pNetworkConfigurationCreateNetworkLocalWorker)
    {
        delete m_pNetworkConfigurationCreateNetworkLocalWorker;
    }

    if (NULL != m_pNetworkConfigurationDeleteNetworkLocalWorker)
    {
        delete m_pNetworkConfigurationDeleteNetworkLocalWorker;
    }

    if (NULL != m_pNetworkConfigurationAttachVirtualInterfaceLocalWorker)
    {
        delete m_pNetworkConfigurationAttachVirtualInterfaceLocalWorker;
    }
}

string NetworkConfigurationLocalObjectManager::getClassName()
{
    return ("Network Service Provider Local");
}

NetworkConfigurationLocalObjectManager *NetworkConfigurationLocalObjectManager::getInstance ()
{
    static NetworkConfigurationLocalObjectManager *pNetworkConfigurationLocalObjectManager = NULL;

    if (NULL == pNetworkConfigurationLocalObjectManager)
    {
        pNetworkConfigurationLocalObjectManager = new NetworkConfigurationLocalObjectManager ();

        WaveNs::prismAssert (NULL != pNetworkConfigurationLocalObjectManager, __FILE__, __LINE__);
    }

    return (pNetworkConfigurationLocalObjectManager);
}

PrismServiceId NetworkConfigurationLocalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}


void NetworkConfigurationLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    if (NULL == m_pValClientSynchronousConnection)
    {
        if (DCM_CCM == DcmToolKit::getDistributionMode ())
        {
            m_pValClientSynchronousConnection = new ValClientSynchronousConnection ();

            prismAssert (NULL != m_pValClientSynchronousConnection, __FILE__, __LINE__);
        }
        else if (CCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            m_pValClientSynchronousConnection = new ValClientSynchronousConnection ("Nsp", "127.0.0.1", 9110);

            prismAssert (NULL != m_pValClientSynchronousConnection, __FILE__, __LINE__);
        }
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

ValClientSynchronousConnection *NetworkConfigurationLocalObjectManager::getPValClientSynchronousConnection ()
{
    NetworkConfigurationLocalObjectManager *pNetworkConfigurationLocalObjectManager = getInstance ();

    WaveNs::prismAssert (NULL != pNetworkConfigurationLocalObjectManager, __FILE__, __LINE__);

    return (pNetworkConfigurationLocalObjectManager->m_pValClientSynchronousConnection);
}

}

