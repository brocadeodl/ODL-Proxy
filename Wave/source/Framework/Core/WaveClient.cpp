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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/WaveClient.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorBase.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "App/AppInterfaceObjectManager.h"
#include "App/AppObjectManager.h"
#include "Shell/ShellObjectManager.h"
#include "Framework/Timer/TimerObjectManager.h"
#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/ServiceInterface/ApplicationService.h"
#include "Framework/ServiceInterface/ApplicationLocalService.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Framework/Profiling/WaveProfiler.h"

#include <time.h>
#include <stdlib.h>

//Commenting it our for now.  We need to find way to enable it on Solaris.
//extern int daemon (int doNotChangeToRootDir, int doNotCloseFileDescriptors);

static string s_waveClientTraceFileDirectory         ("");
static string s_waveClientConfigurationFileDirectory ("");
static string s_waveClientProfileFileDirectory       ("");

namespace WaveNs
{

vector<NativePrismServiceInstantiator>         WaveClient::m_nativePrismServiceInstantiators;
vector<bool>                                   WaveClient::m_nativePrismServiceInstantiatorIsForNormalPhase;
vector<NativeMultiplePrismServiceInstantiator> WaveClient::m_nativeMultiplePrismServiceInstantiators;
UpdateClientStatusFunctionPtr                  WaveClient::m_updateClientStatusFunctionPtr = NULL;
PrismMutex                                     WaveClient::m_updateClientStatusFunctionPtrMutex;  

void WaveClient::initialize (const WaveClientConfiguration &waveClientConfiguration)
{
    // Initialize Random Generator

    srand (time (NULL));

    // Set the Management Interface Role

    FrameworkToolKit::setManagementInterfaceRole (WAVE_MGMT_INTF_ROLE_CLIENT);

    // Set if the database is to be enabled.

    DatabaseObjectManager::setIsDatabaseEnabled (waveClientConfiguration.getIsDatabaseEnabled());
    DatabaseObjectManager::setDatabasePort (waveClientConfiguration.getDatabasePort());

    // First record the current working directory as the directory in which the process was started

    FrameworkToolKit::initialize ();

    // set the WaveClient Framework configuration file.

    PrismFrameworkObjectManager::setConfigurationFile ((getConfigurationFileDirectory ()) + "/" + waveClientConfiguration.getApplicationCompactName() + ".cfg");
    PrismFrameworkObjectManager::setGlobalConfigurationFile ((getConfigurationFileDirectory ()) + "/" + waveClientConfiguration.getApplicationCompactName() + ".global.cfg");

    // Set the trace file name first

    TraceObjectManager::setTraceFileName ((getTraceFileDirectory ()) + "/" + waveClientConfiguration.getApplicationCompactName() + ".trc");

    // Set the profile file namespace

    WaveProfiler::setProfileFileName ((getProfileFileDirectory ()) + "/" + waveClientConfiguration.getApplicationCompactName() + ".wpd");

    // Set if logging to syslog is required

    TraceObjectManager::setIsSysLogRequired (waveClientConfiguration.getIsSysLogRequired ());

    // Record if the process is running as a daemon.

    FrameworkToolKit::setIsRunningAsADaemon (false);

    // PrismFrameworkObjectManager must be the first one to be created.

    PrismFrameworkObjectManager::getInstance ();

    // WARNING! WARNING! WARNING!
    // DO NOT CHANGE ABOVE THIS LINE

    // Below this one the services are registered in reverse order.  The last one will be on top of the list.

    if (true == waveClientConfiguration.getNeedShell ())
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ShellObjectManager::getInstance));

        FrameworkToolKit::registerDebugShellEntries ();
        WaveSourceGeneratorBase::registerDebugShellEntries ();
        WaveClientReceiverObjectManager::registerDebugShellEntries ();
    }

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveUserInterfaceObjectManager::getInstance));
    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveClientTransportObjectManager::getInstance));
    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveClientReceiverObjectManager::getInstance));

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TimerObjectManager::getInstance));
    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TimerSignalObjectManager::getInstance));

    // Instantiate Native WaveClient Services here

    instantiateNativePrismServices ();

    // Start the ObjectManager s corresponding to Application Services

    ApplicationService::createNewApplicationServices ();

    // start the ObjectManager s corresponding to Application Local Services

    ApplicationLocalService::createNewApplicationLocalServices ();

    // Starting Application specfic Object managers.

    AppObjectManager::createAppObjectMangers ();
}

string WaveClient::getTraceFileDirectory ()
{
    static PrismMutex traceFileDirectoryMutex;

    traceFileDirectoryMutex.lock ();

    if ("" != s_waveClientTraceFileDirectory)
    {
        traceFileDirectoryMutex.unlock ();
        return (s_waveClientTraceFileDirectory);
    }
    else
    {
        s_waveClientTraceFileDirectory = FrameworkToolKit::getProcessInitialWorkingDirectory ();

        if ("" == s_waveClientTraceFileDirectory)
        {
            s_waveClientTraceFileDirectory = "/tmp";
        }
    }

    traceFileDirectoryMutex.unlock ();

    return (s_waveClientTraceFileDirectory);
}

string WaveClient::getConfigurationFileDirectory ()
{
    static PrismMutex configurationFileDirectoryMutex;

    configurationFileDirectoryMutex.lock ();

    if ("" != s_waveClientConfigurationFileDirectory)
    {

    configurationFileDirectoryMutex.unlock ();
        return (s_waveClientConfigurationFileDirectory);
    }
    else
    {
        s_waveClientConfigurationFileDirectory = FrameworkToolKit::getProcessInitialWorkingDirectory ();

        if ("" == s_waveClientConfigurationFileDirectory)
        {
            s_waveClientConfigurationFileDirectory = "/tmp";
        }
    }

    configurationFileDirectoryMutex.unlock ();

    return (s_waveClientConfigurationFileDirectory);
}

string WaveClient::getProfileFileDirectory ()
{
    static PrismMutex profileFileDirectoryMutex;

    profileFileDirectoryMutex.lock ();

    if ("" != s_waveClientProfileFileDirectory)
    {
        profileFileDirectoryMutex.unlock ();
        return (s_waveClientProfileFileDirectory);
    }
    else
    {
        s_waveClientProfileFileDirectory = FrameworkToolKit::getProcessInitialWorkingDirectory ();

        if ("" == s_waveClientProfileFileDirectory)
        {
            s_waveClientProfileFileDirectory = "/tmp";
        }
    }

    profileFileDirectoryMutex.unlock ();

    return (s_waveClientProfileFileDirectory);
}

//This is used to register the funcptr invoked by updateclientstatus 
void WaveClient::registerUpdateClientStatusFunction ( UpdateClientStatusFunctionPtr  pUpdateClientStatusFunctionPtr  )
{
    m_updateClientStatusFunctionPtrMutex.lock();
    m_updateClientStatusFunctionPtr = pUpdateClientStatusFunctionPtr;
    m_updateClientStatusFunctionPtrMutex.unlock();
}

// This function will be called by WaveUserInterfaceOM to update status received from Server
// Inturn the client registered funcptr is invoked passing the message
SI32 WaveClient::updateClientStatusFunction( ManagementInterfaceMessage *pManagementInterfaceMessage  )
{
    SI32 status = 0;
    if ( m_updateClientStatusFunctionPtr != NULL  )
    {
        m_updateClientStatusFunctionPtrMutex.lock();
        status = m_updateClientStatusFunctionPtr ( pManagementInterfaceMessage );
        m_updateClientStatusFunctionPtrMutex.unlock();
    }
    return (status);
}

void WaveClient::registerNativeService (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase)
{
    m_nativePrismServiceInstantiators.push_back                (pNativePrismServiceInstantiator);
    m_nativePrismServiceInstantiatorIsForNormalPhase.push_back (isForNormalPhase);
}

void WaveClient::registerNativeServiceInternal (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase)
{
    m_nativePrismServiceInstantiators.insert                (m_nativePrismServiceInstantiators.begin (), pNativePrismServiceInstantiator);
    m_nativePrismServiceInstantiatorIsForNormalPhase.insert (m_nativePrismServiceInstantiatorIsForNormalPhase.begin (), isForNormalPhase);
}

void WaveClient::registerNativeService (NativeMultiplePrismServiceInstantiator pNativeMultiplePrismServiceInstantiator)
{
    m_nativeMultiplePrismServiceInstantiators.push_back (pNativeMultiplePrismServiceInstantiator);
}

void WaveClient::instantiateNativePrismServices ()
{
    FrameworkSequenceGenerator &frameworkSequenceGenerator                       = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();
    UI32                        numberOfNativePrismServciesToInstantiate         = m_nativePrismServiceInstantiators.size ();
    UI32                        numberOfNativeMultiplePrismServciesToInstantiate = m_nativeMultiplePrismServiceInstantiators.size ();
    UI32                        i                                                = 0;
    UI32                        j                                                = 0;
    WaveObjectManager         *pWaveObjectManager                              = NULL;

    for (i = 0; i < numberOfNativePrismServciesToInstantiate; i++)
    {
        pWaveObjectManager = (*(m_nativePrismServiceInstantiators[i])) ();

        prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

        frameworkSequenceGenerator.addPrismServiceIdToAll (pWaveObjectManager->getServiceId (), m_nativePrismServiceInstantiatorIsForNormalPhase[i]);
    }

    for (i = 0; i < numberOfNativeMultiplePrismServciesToInstantiate; i++)
    {
        vector<WaveObjectManager *> objectManagers         = (*(m_nativeMultiplePrismServiceInstantiators[i])) ();
        UI32                         numberOfObjectManagers = objectManagers.size ();

        for (j = 0; j < numberOfObjectManagers; j++)
        {
            pWaveObjectManager = objectManagers[j];

            prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

            frameworkSequenceGenerator.addPrismServiceIdToAll (pWaveObjectManager->getServiceId ());
        }
    }

    trace (TRACE_LEVEL_DEBUG, string ("Instantiated ") + numberOfNativePrismServciesToInstantiate + " Native WaveClient Services");
}

}

extern "C" void setWaveClientTraceFileDirectory (const char *pPrismTraceFileDirectory)
{
    s_waveClientTraceFileDirectory = pPrismTraceFileDirectory;
}

extern "C" void setWaveClientConfigurationFileDirectory (const char *pPrismConfigurationFileDirectory)
{
    s_waveClientConfigurationFileDirectory = pPrismConfigurationFileDirectory;
}
