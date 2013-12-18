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
 *   Author : Vidyasagara Reddy Guntaka, Jayanth Venkataraman              *
 ***************************************************************************/

#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Version/PrismVersion.h"
#include "Framework/Utils/AssertUtils.h"
#include "SoftwareManagement/SoftwareManagementVersionWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "SystemManagement/SystemManagementToolKit.h"
#include "SystemManagement/WaveSystemManagementDisplayConfigurationWorker.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "SystemManagement/WaveSystemManagementGetYangUserInterfaceWorker.h"
#include "SystemManagement/WaveSystemManagementYangInterfacesInitializeWorker.h"
#include "SystemManagement/WaveSystemManagementGateway.h"
#include "SystemManagement/WaveSystemManagementTypes.h"
#include "SystemManagement/Migration/WaveSystemManagementGetPossibleCompletionsWorker.h"
#include "SystemManagement/Configuration/CliConfigurationFile.h"
#include "Shell/ShellDebug.h"

namespace WaveNs
{

WaveSystemManagementObjectManager::WaveSystemManagementObjectManager ()
    : WaveLocalObjectManager (WaveSystemManagementObjectManager::getClassName ())
{
    m_pWaveSystemManagementDisplayConfigurationWorker = new WaveSystemManagementDisplayConfigurationWorker (this);

    prismAssert (NULL != m_pWaveSystemManagementDisplayConfigurationWorker, __FILE__, __LINE__);

    m_pWaveSystemManagementGetYangUserInterfaceWorker = new WaveSystemManagementGetYangUserInterfaceWorker (this);

    prismAssert (NULL != m_pWaveSystemManagementGetYangUserInterfaceWorker, __FILE__, __LINE__);

    m_pWaveSystemManagementYangInterfacesInitializeWorker = new WaveSystemManagementYangInterfacesInitializeWorker (this);

    prismAssert (NULL != m_pWaveSystemManagementYangInterfacesInitializeWorker, __FILE__, __LINE__);

    m_pWaveSystemManagementGetPossibleCompletionsWorker = new WaveSystemManagementGetPossibleCompletionsWorker (this);

    prismAssert (NULL != m_pWaveSystemManagementGetPossibleCompletionsWorker, __FILE__, __LINE__);

    m_pWaveSystemManagementGatewayWorker = new WaveSystemManagementGatewayWorker (this);

    prismAssert (NULL != m_pWaveSystemManagementGatewayWorker, __FILE__, __LINE__);

    addDebugFunction ((ShellCmdFunction) (&CliConfigurationFile::debugLoad), "debugLoadCliFile");
}

WaveSystemManagementObjectManager::~WaveSystemManagementObjectManager ()
{
    if (NULL != m_pWaveSystemManagementDisplayConfigurationWorker)
    {
        delete m_pWaveSystemManagementDisplayConfigurationWorker;
    }

    if (NULL != m_pWaveSystemManagementGetYangUserInterfaceWorker)
    {
        delete m_pWaveSystemManagementGetYangUserInterfaceWorker;
    }

    if (NULL != m_pWaveSystemManagementYangInterfacesInitializeWorker)
    {
        delete m_pWaveSystemManagementYangInterfacesInitializeWorker;
    }

    if (NULL != m_pWaveSystemManagementGetPossibleCompletionsWorker)
    {
        delete m_pWaveSystemManagementGetPossibleCompletionsWorker;
    }

    if (NULL != m_pWaveSystemManagementGatewayWorker)
    {
        delete m_pWaveSystemManagementGatewayWorker;
    }
}

WaveSystemManagementObjectManager *WaveSystemManagementObjectManager::getInstance ()
{
    static WaveSystemManagementObjectManager *pSoftwareManagementObjectManager = NULL;

    if (NULL == pSoftwareManagementObjectManager)
    {
        pSoftwareManagementObjectManager = new WaveSystemManagementObjectManager ();
        WaveNs::prismAssert (NULL != pSoftwareManagementObjectManager, __FILE__, __LINE__);
    }

    return (pSoftwareManagementObjectManager);
}

PrismServiceId WaveSystemManagementObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string WaveSystemManagementObjectManager::getClassName ()
{
    return ("System Management");
}

YangModuleCollection *WaveSystemManagementObjectManager::getYangModuleCollection ()
{
    WaveSystemManagementObjectManager *pWaveSystemManagementObjectManager = WaveSystemManagementObjectManager::getInstance ();

    YangModuleCollection *retYangModuleCollection = pWaveSystemManagementObjectManager->m_pWaveSystemManagementYangInterfacesInitializeWorker->getYangModuleCollection ();

    return (retYangModuleCollection);
}

YangUserInterface *WaveSystemManagementObjectManager::getYangUserInterface ()
{
    WaveSystemManagementObjectManager *pWaveSystemManagementObjectManager = WaveSystemManagementObjectManager::getInstance ();

    return (pWaveSystemManagementObjectManager->m_pWaveSystemManagementYangInterfacesInitializeWorker->getYangUserInterface ());
}

YangUserInterface *WaveSystemManagementObjectManager::getClonedYangUserInterface ()
{
    WaveSystemManagementObjectManager *pWaveSystemManagementObjectManager = WaveSystemManagementObjectManager::getInstance ();

    YangElement *pYangElement = pWaveSystemManagementObjectManager->m_pWaveSystemManagementYangInterfacesInitializeWorker->getClonedYangUserInterface();

    WaveNs::prismAssert (NULL != pYangElement, __FILE__, __LINE__);

    YangUserInterface *pYangUserInterface = dynamic_cast<YangUserInterface *> (pYangElement);

    WaveNs::prismAssert (NULL != pYangUserInterface, __FILE__, __LINE__);

    return (pYangUserInterface);
}

void WaveSystemManagementObjectManager::getYinForYangUserInterface (string &yinForYangUserInterface)
{
    WaveSystemManagementObjectManager *pWaveSystemManagementObjectManager = WaveSystemManagementObjectManager::getInstance ();

    yinForYangUserInterface = pWaveSystemManagementObjectManager->m_pWaveSystemManagementYangInterfacesInitializeWorker->getYinForYangUserInterface ();
}

}
