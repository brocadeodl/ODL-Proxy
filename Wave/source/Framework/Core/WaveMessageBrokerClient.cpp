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

#include "Framework/Core/WaveMessageBrokerClient.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorBase.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Cluster/MultiPartition/Global/MultiPartitionObjectManager.h"
#include "Cluster/MultiPartition/Local/MultiPartitionLocalObjectManager.h"
#include "Framework/DistributedLog/DistributedLogObjectManager.h"
#include "Framework/DistributedLog/Test/DistributedLogTestObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Persistence/Test/PersistenceTestObjectManager.h"
#include "File/Local/Test/FileLocalMessagingTestObjectManager.h"
#include "ManagementInterface/ManagementInterfaceReceiverObjectManager.h"
#include "Framework/Profiling/WaveProfiler.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObject.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "Framework/Core/ApplicationSpecificServices.h"
#include "Framework/ObjectModel/ReservedWaveLocalObjectManager.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugGlobalObjectManager.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugLocalObjectManager.h"
#include "Framework/Utils/TimeZoneToolKit.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "Framework/Messaging/HaPeer/HaPeerMessageReceiverObjectManager.h"
#include "Framework/Messaging/HaPeer/HaPeerMessageTransportObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability6ObjectManager.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/SystemManagementToolKit.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceReceiverObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerClientReceiverObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerConnectionObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/Test/WaveMessagingBrokerClientTestObjectManager.h"
#include "Regression/RegressionTestObjectManager.h"

#include <time.h>
#include <stdlib.h>
#include <sys/resource.h>

//Commenting it our for now.  We need to find way to enable it on Solaris.
//extern int daemon (int doNotChangeToRootDir, int doNotCloseFileDescriptors);

static string s_waveTraceFileDirectory         ("");
static string s_waveConfigurationFileDirectory ("");
static string s_waveProfileFileDirectory       ("");

namespace WaveNs
{

vector<NativePrismServiceInstantiator>         WaveMessageBrokerClient::m_nativePrismServiceInstantiators;
vector<bool>                                   WaveMessageBrokerClient::m_nativePrismServiceInstantiatorIsForNormalPhase;
vector<NativeMultiplePrismServiceInstantiator> WaveMessageBrokerClient::m_nativeMultiplePrismServiceInstantiators;
vector<NativePrismServiceInstantiator>         WaveMessageBrokerClient::m_nativeApplicationSpecificPrismServiceInstantiators;
vector<ResourceId>                             WaveMessageBrokerClient::m_nativePrismServiceInstantiationMode;
bool                                           WaveMessageBrokerClient::m_enablePersistenceSupport;
bool                                           WaveMessageBrokerClient::m_enableClusteringSupport;
bool                                           WaveMessageBrokerClient::m_enableBuiltInSelfTestSupport;
bool                                           WaveMessageBrokerClient::m_enableShellSupport                                  = true;
bool                                           WaveMessageBrokerClient::m_enableManagementInterfaceSupport;
bool                                           WaveMessageBrokerClient::m_enableLicensingSupport;
bool                                           WaveMessageBrokerClient::m_enableSoftwareManagementSupport;
bool                                           WaveMessageBrokerClient::m_enableTimerSupport;
bool                                           WaveMessageBrokerClient::m_enableHaPeerSupport;
bool                                           WaveMessageBrokerClient::m_enableDistributedLogSupport;
bool                                           WaveMessageBrokerClient::m_enableTraceSupport;
bool                                           WaveMessageBrokerClient::m_enableFileServiceSupport;
bool                                           WaveMessageBrokerClient::m_enableSystemManagementSupport;
PersistencePostBootCheck                       WaveMessageBrokerClient::m_persistencePostBootCheck;
PrismMutex                                     WaveMessageBrokerClient::m_clusterEnabledCheckMutex;
ClusterEnabledCheck                            WaveMessageBrokerClient::m_clusterEnabledCheck  = NULL;
string                                         WaveMessageBrokerClient::m_waveUserClientPath;
string                                         WaveMessageBrokerClient::m_waveUserClientParams;
LogOperationStatusFunction                     WaveMessageBrokerClient::m_logOperationStatusFunction = NULL;
PrismMutex                                     WaveMessageBrokerClient::m_logOperationStatusMutex;
IsWaveClusteringEnabled                        WaveMessageBrokerClient::m_isWaveClusteringEnabled = NULL;
PrismMutex                                     WaveMessageBrokerClient::m_isWaveClusteringEnabledMutex;
GetLinecardReadySlotIdBitMapFunction           WaveMessageBrokerClient::m_linecardReadySlotIdBitMapFunction = NULL;
PrismMutex                                     WaveMessageBrokerClient::m_linecardReadySlotIdBitMapFunctionMutex;

void WaveMessageBrokerClient::enableAllFeatures ()
{
    enablePersistence         ();
    enableClustering          ();
    enableBuiltInSelfTest     ();
    enableShell               ();
    enableManagementInterface ();
    enableLicense             ();
    enableFileService         ();
    enableSoftwareManagement  ();
    enableTimer               ();
    enableTrace               ();
    enableDistributedLog      ();
    enableHaPeer              ();
    enableSystemManagement    ();
}

void WaveMessageBrokerClient::enablePersistence ()
{
    m_enablePersistenceSupport = true;
}

void WaveMessageBrokerClient::disablePersistence ()
{
    m_enablePersistenceSupport = false;
}

void WaveMessageBrokerClient::enableClustering ()
{
    m_enableClusteringSupport = true;
}

void WaveMessageBrokerClient::disableClustering ()
{
    m_enableClusteringSupport = false;
}

void WaveMessageBrokerClient::enableBuiltInSelfTest ()
{
    m_enableBuiltInSelfTestSupport = true;
}

void WaveMessageBrokerClient::disableBuiltInSelfTest ()
{
    m_enableBuiltInSelfTestSupport = false;
}

void WaveMessageBrokerClient::enableShell ()
{
    m_enableShellSupport = true;
}

void WaveMessageBrokerClient::disableShell ()
{
    m_enableShellSupport = false;
}

void WaveMessageBrokerClient::enableManagementInterface ()
{
    m_enableManagementInterfaceSupport = true;
}

void WaveMessageBrokerClient::disableManagementInterface ()
{
    m_enableManagementInterfaceSupport = false;
}

void WaveMessageBrokerClient::enableLicense ()
{
    m_enableLicensingSupport = true;
}

void WaveMessageBrokerClient::disableLicense ()
{
    m_enableLicensingSupport = false;
}

void WaveMessageBrokerClient::enableSoftwareManagement ()
{
    m_enableSoftwareManagementSupport = true;
}

void WaveMessageBrokerClient::disableSoftwareManagement ()
{
    m_enableSoftwareManagementSupport = false;
}

void WaveMessageBrokerClient::enableTimer ()
{
    m_enableTimerSupport = true;
}

void WaveMessageBrokerClient::disableTimer ()
{
    m_enableTimerSupport = false;
}

void WaveMessageBrokerClient::enableTrace ()
{
    m_enableTraceSupport = true;
}

void WaveMessageBrokerClient::disableTrace ()
{
    m_enableTraceSupport = false;
}

void WaveMessageBrokerClient::enableFileService ()
{
    m_enableFileServiceSupport = true;
}

void WaveMessageBrokerClient::disableFileService ()
{
    m_enableFileServiceSupport = false;
}

void WaveMessageBrokerClient::enableDistributedLog ()
{
    m_enableDistributedLogSupport = true;
}

void WaveMessageBrokerClient::disableDistributedLog ()
{
    m_enableDistributedLogSupport = false;
}

void WaveMessageBrokerClient::enableHaPeer ()
{
    m_enableHaPeerSupport = true;
}

void WaveMessageBrokerClient::disableHaPeer ()
{
    m_enableHaPeerSupport = false;
}

void WaveMessageBrokerClient::enableSystemManagement ()
{
    m_enableSystemManagementSupport = true;
}

void WaveMessageBrokerClient::disableSystemManagement ()
{
    m_enableSystemManagementSupport = false;
}

void WaveMessageBrokerClient::initialize (const WaveMainConfiguration &waveMainConfiguration)
{
    // First change to user specified directory if any

    SI32 rlimitStatus = 0;
    rlimit rlim;

    rlim.rlim_cur = 1048576 * 8;   // 8 MB
    rlim.rlim_max = 1048576 * 8;

    rlimitStatus = setrlimit (RLIMIT_STACK, &rlim);

    if (0 != rlimitStatus)
    {
        trace (TRACE_LEVEL_WARN, "WaveMessageBrokerClient::initialize not able to set rlimit");
    }

    if ("" != (waveMainConfiguration.getChangeDirectoryTo ()))
    {
        SI32 chDirStatus = chdir ((waveMainConfiguration.getChangeDirectoryTo ()).c_str ());

        if (0 != chDirStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveMessageBrokerClient::initialize : Could not Change Directory to " + waveMainConfiguration.getChangeDirectoryTo ());
            trace (TRACE_LEVEL_ERROR, "WaveMessageBrokerClient::initialize : Reason listed below :");
            perror ("WAVE");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    // Initialize Random Generator

    srand (time (NULL));

    // Set the Management Interface Role

    FrameworkToolKit::setManagementInterfaceRole (WAVE_MGMT_INTF_ROLE_SERVER);

    // First record the current working directory as the directory in which the process was started

    FrameworkToolKit::initialize ();

    //Initialize time zone information

    TimeZoneToolKit::initialize ();

    // Setup the System Management;

    SystemManagementToolKit::setYinPaths          (waveMainConfiguration.getYinPaths ());
    SystemManagementToolKit::setWyserTagsFilePath (waveMainConfiguration.getWyserTagsFilePath ());

    // set the WaveMessageBrokerClient Framework configuration file.

    PrismFrameworkObjectManager::setConfigurationFile ((getConfigurationFileDirectory ()) + "/" + waveMainConfiguration.getApplicationCompactName () + ".cfg");
    PrismFrameworkObjectManager::setGlobalConfigurationFile ((getConfigurationFileDirectory ()) + "/" + waveMainConfiguration.getApplicationCompactName () + ".global.cfg");

#ifdef _X86_COMPILE_
    PrismFrameworkObjectManager::setLockFileForConfigurationFile ((getConfigurationFileDirectory ()) + "/" + waveMainConfiguration.getApplicationCompactName () + ".cfg.tmp");
#else
    PrismFrameworkObjectManager::setLockFileForConfigurationFile ("/tmp/" + waveMainConfiguration.getApplicationCompactName () + ".cfg.tmp");
#endif

    // Set the trace file name first

    TraceObjectManager::setTraceFileName ((getTraceFileDirectory ()) + "/" + waveMainConfiguration.getApplicationCompactName () + ".trc");

    // Set the profile file namespace

    WaveProfiler::setProfileFileName ((getProfileFileDirectory ()) + "/" + waveMainConfiguration.getApplicationCompactName () + ".wpd");

    // Set if logging to syslog is required

    TraceObjectManager::setIsSysLogRequired (waveMainConfiguration.getIsSysLogRequired ());


    // Run the process in daemon mode if required.

    if (true == (waveMainConfiguration.getIsADaemon ()))
    {
        daemon (1, 0);
        FrameworkToolKit::initializeConsolePrintFunction ();
    }

    // Record if the process is running as a daemon.

    FrameworkToolKit::setIsRunningAsADaemon (waveMainConfiguration.getIsADaemon ());

    // Add ORM Repository Most Base Classes information from WaveMessageBrokerClient.

    OrmRepository::addMostBaseClass (PrismPersistableObject::getClassName ());
    OrmRepository::addMostBaseClass (WaveManagedObject::getClassName      ());
    OrmRepository::addMostBaseClass (WaveLocalManagedObject::getClassName ());

    // PrismFrameworkObjectManager must be the first one to be created.

    PrismFrameworkObjectManager::getInstance ();


    if (true == m_enableShellSupport)
    {
        // ShellObjectManager must be the next one so that all can register their shells and related shell handlers.

        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ShellObjectManager::getInstance));
    }

    FrameworkToolKit::registerDebugShellEntries ();
    WaveSourceGeneratorBase::registerDebugShellEntries ();
    InterLocationMessageReceiverObjectManager::registerDebugShellEntries ();
    ManagementInterfaceReceiverObjectManager::registerDebugShellEntries ();
    // ManagementInterfaceObjectManager::registerDebugShellEntries ();

    // WARNING! WARNING! WARNING!
    // DO NOT CHANGE ABOVE THIS LINE

    // FrameworkToolKit must be initialized before we call any trace.
    // Trace functionality uses FrameworkToolKit for different directory paths.
    trace(TRACE_LEVEL_PERF_START, "WaveMessageBrokerClient Bootup.");

    // Below this one the services are registered in reverse order.  The last one will be on top of the list.

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveMessagingBrokerClientTestObjectManager::getInstance));
    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (RegressionTestObjectManager::getInstance));

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveMessagingBrokerClientReceiverObjectManager::getInstance));
    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveMessagingBrokerConnectionObjectManager::getInstance));

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ManagementInterfaceReceiverObjectManager::getInstance));
    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ManagementInterfaceObjectManager::getInstance));

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TraceObjectManager::getInstance));

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ReservedWaveLocalObjectManager::getInstance), false);

    // WARNING! WARNING! WARNING!
    // Always keep the Timer Object Manager as the last registered internal native service.
    // All services will cancel their timers with the Timer Object Manager during shutdown so this service must be the last to shutdown.

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TimerObjectManager::getInstance), false);
    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TimerSignalObjectManager::getInstance), false);

    // Instantiate Native WaveMessageBrokerClient Services here

    instantiateNativePrismServices ();

    // Start the ObjectManager s corresponding to Application Services

    ApplicationService::createNewApplicationServices ();

    // start the ObjectManager s corresponding to Application Local Services

    ApplicationLocalService::createNewApplicationLocalServices ();

    // Starting Application specific Object managers.

    AppObjectManager::createAppObjectMangers ();
}

string WaveMessageBrokerClient::getTraceFileDirectory ()
{
    static PrismMutex traceFileDirectoryMutex;

    traceFileDirectoryMutex.lock ();

    if ("" != s_waveTraceFileDirectory)
    {
        traceFileDirectoryMutex.unlock ();
        return (s_waveTraceFileDirectory);
    }
    else
    {
        s_waveTraceFileDirectory = FrameworkToolKit::getProcessInitialWorkingDirectory ();

        if ("" == s_waveTraceFileDirectory)
        {
            s_waveTraceFileDirectory = "/tmp";
        }
    }

    traceFileDirectoryMutex.unlock ();

    return (s_waveTraceFileDirectory);
}

string WaveMessageBrokerClient::getConfigurationFileDirectory ()
{
    static PrismMutex configurationFileDirectoryMutex;

    configurationFileDirectoryMutex.lock ();

    if ("" != s_waveConfigurationFileDirectory)
    {
        configurationFileDirectoryMutex.unlock ();
        return (s_waveConfigurationFileDirectory);
    }
    else
    {
        s_waveConfigurationFileDirectory = FrameworkToolKit::getProcessInitialWorkingDirectory ();

        if ("" == s_waveConfigurationFileDirectory)
        {
            s_waveConfigurationFileDirectory = "/tmp";
        }
    }

    configurationFileDirectoryMutex.unlock ();

    return (s_waveConfigurationFileDirectory);
}

string WaveMessageBrokerClient::getProfileFileDirectory ()
{
    static PrismMutex profileFileDirectoryMutex;

    profileFileDirectoryMutex.lock ();

    if ("" != s_waveProfileFileDirectory)
    {
        profileFileDirectoryMutex.unlock ();
        return (s_waveProfileFileDirectory);
    }
    else
    {
        s_waveProfileFileDirectory = FrameworkToolKit::getProcessInitialWorkingDirectory ();

        if ("" == s_waveProfileFileDirectory)
        {
            s_waveProfileFileDirectory = "/tmp";
        }
    }

    profileFileDirectoryMutex.unlock ();

    return (s_waveProfileFileDirectory);
}

void WaveMessageBrokerClient::registerNativeService (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase, const ResourceId &serviceMode)
{
    m_nativePrismServiceInstantiators.push_back                (pNativePrismServiceInstantiator);
    m_nativePrismServiceInstantiatorIsForNormalPhase.push_back (isForNormalPhase);
    m_nativePrismServiceInstantiationMode.push_back            (serviceMode);
}

void WaveMessageBrokerClient::registerApplicationSpecificNativeServices (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase, const ResourceId &serviceMode)
{
    m_nativeApplicationSpecificPrismServiceInstantiators.push_back (pNativePrismServiceInstantiator);

    registerNativeService (pNativePrismServiceInstantiator, isForNormalPhase, serviceMode);
}

void WaveMessageBrokerClient::registerNativeServiceInternal (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase, const ResourceId &serviceMode)
{
    m_nativePrismServiceInstantiators.insert                (m_nativePrismServiceInstantiators.begin (), pNativePrismServiceInstantiator);
    m_nativePrismServiceInstantiatorIsForNormalPhase.insert (m_nativePrismServiceInstantiatorIsForNormalPhase.begin (), isForNormalPhase);
    m_nativePrismServiceInstantiationMode.insert            (m_nativePrismServiceInstantiationMode.begin (), serviceMode);
}

void WaveMessageBrokerClient::registerNativeService (NativeMultiplePrismServiceInstantiator pNativeMultiplePrismServiceInstantiator)
{
    m_nativeMultiplePrismServiceInstantiators.push_back (pNativeMultiplePrismServiceInstantiator);
}

void WaveMessageBrokerClient::registerPersistencePostBootCheck (PersistencePostBootCheck pPersistencePostBootCheck)
{
    m_persistencePostBootCheck = pPersistencePostBootCheck;
}

ResourceId WaveMessageBrokerClient::persistencePostBootCheck ()
{
    return ((m_persistencePostBootCheck ()));
}

void WaveMessageBrokerClient::registerClusterEnabledCheck (ClusterEnabledCheck pClusterEnabledCheck)
{
    m_clusterEnabledCheckMutex.lock ();

    m_clusterEnabledCheck = pClusterEnabledCheck;

    m_clusterEnabledCheckMutex.unlock ();
}

bool WaveMessageBrokerClient::clusterEnabledCheck ()
{
    bool clusterEnabled = false;

    m_clusterEnabledCheckMutex.lock ();

    if (NULL != m_clusterEnabledCheck)
    {
        clusterEnabled = m_clusterEnabledCheck ();
    }

    m_clusterEnabledCheckMutex.unlock ();

    return clusterEnabled;
}

void WaveMessageBrokerClient::registerIsWaveClusteringEnabledFunction (IsWaveClusteringEnabled pIsWaveClusteringEnabled)
{
    m_isWaveClusteringEnabledMutex.lock ();
    m_isWaveClusteringEnabled = pIsWaveClusteringEnabled ;
    m_isWaveClusteringEnabledMutex.unlock ();
}

bool WaveMessageBrokerClient::isWaveClusteringEnabled ()
{
    bool mode = false;
    m_isWaveClusteringEnabledMutex.lock ();
    mode = m_isWaveClusteringEnabled();
    m_isWaveClusteringEnabledMutex.unlock ();

    return mode;
}

void WaveMessageBrokerClient::registerLogOperationStatusFunction (LogOperationStatusFunction pLogOperationStatusFunction)
{
    m_logOperationStatusMutex.lock ();

    m_logOperationStatusFunction = pLogOperationStatusFunction;

    m_logOperationStatusMutex.unlock ();
}

void WaveMessageBrokerClient::logOperationStatus (ResourceId status)
{
    trace (TRACE_LEVEL_DEBUG, string("WaveMessageBrokerClient::logOperationStatus : status = ") + status);

    m_logOperationStatusMutex.lock ();
    m_logOperationStatusFunction (status);
    m_logOperationStatusMutex.unlock ();
}

void WaveMessageBrokerClient::registerGetLinecardReadySlotIdBitMapFunction ( GetLinecardReadySlotIdBitMapFunction plinecardReadySlotIdBitMapFunction )
{
    m_linecardReadySlotIdBitMapFunctionMutex.lock();

    m_linecardReadySlotIdBitMapFunction = plinecardReadySlotIdBitMapFunction;

    m_linecardReadySlotIdBitMapFunctionMutex.unlock();
}

void WaveMessageBrokerClient::getLinecardReadySlotIdBitMap ( BitMap &linecardReadySlotIdBitMap , UI32 &maxLinecards )
{
    m_linecardReadySlotIdBitMapFunctionMutex.lock();

    if ( NULL != m_linecardReadySlotIdBitMapFunction )
    {
        m_linecardReadySlotIdBitMapFunction ( linecardReadySlotIdBitMap, maxLinecards );
    }

    m_linecardReadySlotIdBitMapFunctionMutex.unlock();
}

void WaveMessageBrokerClient::setWaveUserClientPath (const string &path)
{
    m_waveUserClientPath = path;
}

string WaveMessageBrokerClient::getWaveUserClientPath ()
{
    return (m_waveUserClientPath);
}

void WaveMessageBrokerClient::setWaveUserClientParams (const string &params)
{
    m_waveUserClientParams = params;
}

string WaveMessageBrokerClient::getWaveUserClientParams ()
{
    return (m_waveUserClientParams);
}

void WaveMessageBrokerClient::instantiateNativePrismServices ()
{
    FrameworkSequenceGenerator &frameworkSequenceGenerator                       = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();
    UI32                        numberOfNativePrismServciesToInstantiate         = m_nativePrismServiceInstantiators.size ();
    UI32                        numberOfNativeMultiplePrismServciesToInstantiate = m_nativeMultiplePrismServiceInstantiators.size ();
    UI32                        numberOfNativeApplicationSpecificPrismServices   = m_nativeApplicationSpecificPrismServiceInstantiators.size ();
    UI32                        i                                                = 0;
    UI32                        j                                                = 0;
    WaveObjectManager         *pWaveObjectManager                              = NULL;

    for (i = 0; i < numberOfNativePrismServciesToInstantiate; i++)
    {

        WaveObjectManager::m_waveServiceLaunchMode = (WaveServiceMode) (m_nativePrismServiceInstantiationMode[i]);

        pWaveObjectManager = (*(m_nativePrismServiceInstantiators[i])) ();

        prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

        if (WAVE_SERVICE_ACTIVE == m_nativePrismServiceInstantiationMode[i])
        {
            frameworkSequenceGenerator.addPrismServiceIdToAll (pWaveObjectManager->getServiceId (), m_nativePrismServiceInstantiatorIsForNormalPhase[i]);

        }
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

    for (i = 0; i < numberOfNativeApplicationSpecificPrismServices; i++)
    {

        pWaveObjectManager = (*(m_nativeApplicationSpecificPrismServiceInstantiators[i])) ();

        ApplicationSpecificServices *pApplicationSpecificServices = ApplicationSpecificServices::getInstance ();

        pApplicationSpecificServices->setPrismServiceId (pWaveObjectManager->getServiceId ());

        trace (TRACE_LEVEL_DEBUG, string ("ApplicationSpecificPrismServices service ID ") + pWaveObjectManager->getServiceId ());
    }

    trace (TRACE_LEVEL_INFO, string ("Instantiated ") + numberOfNativePrismServciesToInstantiate + " Native WaveMessageBrokerClient Services");
}

void WaveMessageBrokerClient::waveExit ()
{
#if 0
    map<PrismThreadId, PrismThreadId> mapOfCurrentPrismThreads;

    PrismFrameworkObjectManager::getAllPrismThreads (mapOfCurrentPrismThreads);

    map<PrismThreadId, PrismThreadId>::iterator  threadIterator;

    for (threadIterator = mapOfCurrentPrismThreads.begin (); threadIterator != mapOfCurrentPrismThreads.end (); threadIterator++)
    {
        pthread_cancel (threadIterator->first);
    }

    for (threadIterator = mapOfCurrentPrismThreads.begin (); threadIterator != mapOfCurrentPrismThreads.end (); threadIterator++)
    {
        void *pContext = NULL;

        pthread_join (threadIterator->first, &pContext);
    }
#endif
    PrismFrameworkObjectManager::deleteAllPrismThreads ();

    exit (0);
}

}
