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

#include "Framework/Core/Wave.h"
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

vector<NativePrismServiceInstantiator>         Wave::m_nativePrismServiceInstantiators;
vector<bool>                                   Wave::m_nativePrismServiceInstantiatorIsForNormalPhase;
vector<NativeMultiplePrismServiceInstantiator> Wave::m_nativeMultiplePrismServiceInstantiators;
vector<NativePrismServiceInstantiator>         Wave::m_nativeApplicationSpecificPrismServiceInstantiators;
vector<ResourceId>                             Wave::m_nativePrismServiceInstantiationMode;
bool                                           Wave::m_enablePersistenceSupport;
bool                                           Wave::m_enableClusteringSupport;
bool                                           Wave::m_enableBuiltInSelfTestSupport;
bool                                           Wave::m_enableShellSupport;
bool                                           Wave::m_enableManagementInterfaceSupport;
bool                                           Wave::m_enableLicensingSupport;
bool                                           Wave::m_enableSoftwareManagementSupport;
bool                                           Wave::m_enableTimerSupport;
bool                                           Wave::m_enableHaPeerSupport;
bool                                           Wave::m_enableDistributedLogSupport;
bool                                           Wave::m_enableTraceSupport;
bool                                           Wave::m_enableFileServiceSupport;
bool                                           Wave::m_enableSystemManagementSupport;
PersistencePostBootCheck                       Wave::m_persistencePostBootCheck;
PrismMutex                                     Wave::m_clusterEnabledCheckMutex;
ClusterEnabledCheck                            Wave::m_clusterEnabledCheck  = NULL;
string                                         Wave::m_waveUserClientPath;
string                                         Wave::m_waveUserClientParams;
LogOperationStatusFunction                     Wave::m_logOperationStatusFunction = NULL;
PrismMutex                                     Wave::m_logOperationStatusMutex;
IsWaveClusteringEnabled                        Wave::m_isWaveClusteringEnabled = NULL;
PrismMutex                                     Wave::m_isWaveClusteringEnabledMutex;
GetLinecardReadySlotIdBitMapFunction           Wave::m_linecardReadySlotIdBitMapFunction = NULL;
PrismMutex                                     Wave::m_linecardReadySlotIdBitMapFunctionMutex;

void Wave::enableAllFeatures ()
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

void Wave::enablePersistence ()
{
    m_enablePersistenceSupport = true;
}

void Wave::disablePersistence ()
{
    m_enablePersistenceSupport = false;
}

void Wave::enableClustering ()
{
    m_enableClusteringSupport = true;
}

void Wave::disableClustering ()
{
    m_enableClusteringSupport = false;
}

void Wave::enableBuiltInSelfTest ()
{
    m_enableBuiltInSelfTestSupport = true;
}

void Wave::disableBuiltInSelfTest ()
{
    m_enableBuiltInSelfTestSupport = false;
}

void Wave::enableShell ()
{
    m_enableShellSupport = true;
}

void Wave::disableShell ()
{
    m_enableShellSupport = false;
}

void Wave::enableManagementInterface ()
{
    m_enableManagementInterfaceSupport = true;
}

void Wave::disableManagementInterface ()
{
    m_enableManagementInterfaceSupport = false;
}

void Wave::enableLicense ()
{
    m_enableLicensingSupport = true;
}

void Wave::disableLicense ()
{
    m_enableLicensingSupport = false;
}

void Wave::enableSoftwareManagement ()
{
    m_enableSoftwareManagementSupport = true;
}

void Wave::disableSoftwareManagement ()
{
    m_enableSoftwareManagementSupport = false;
}

void Wave::enableTimer ()
{
    m_enableTimerSupport = true;
}

void Wave::disableTimer ()
{
    m_enableTimerSupport = false;
}

void Wave::enableTrace ()
{
    m_enableTraceSupport = true;
}

void Wave::disableTrace ()
{
    m_enableTraceSupport = false;
}

void Wave::enableFileService ()
{
    m_enableFileServiceSupport = true;
}

void Wave::disableFileService ()
{
    m_enableFileServiceSupport = false;
}

void Wave::enableDistributedLog ()
{
    m_enableDistributedLogSupport = true;
}

void Wave::disableDistributedLog ()
{
    m_enableDistributedLogSupport = false;
}

void Wave::enableHaPeer ()
{
    m_enableHaPeerSupport = true;
}

void Wave::disableHaPeer ()
{
    m_enableHaPeerSupport = false;
}

void Wave::enableSystemManagement ()
{
    m_enableSystemManagementSupport = true;
}

void Wave::disableSystemManagement ()
{
    m_enableSystemManagementSupport = false;
}

void Wave::initialize (const WaveMainConfiguration &waveMainConfiguration)
{
    // First change to user specified directory if any

    SI32 rlimitStatus = 0;
    rlimit rlim;

    rlim.rlim_cur = 1048576 * 8;   // 8 MB
    rlim.rlim_max = 1048576 * 8;

    rlimitStatus = setrlimit (RLIMIT_STACK, &rlim);

    if (0 != rlimitStatus)
    {
        trace (TRACE_LEVEL_WARN, "Wave::initialize not able to set rlimit");
    }

    if ("" != (waveMainConfiguration.getChangeDirectoryTo ()))
    {
        SI32 chDirStatus = chdir ((waveMainConfiguration.getChangeDirectoryTo ()).c_str ());

        if (0 != chDirStatus)
        {
            trace (TRACE_LEVEL_ERROR, "Wave::initialize : Could not Change Directory to " + waveMainConfiguration.getChangeDirectoryTo ());
            trace (TRACE_LEVEL_ERROR, "Wave::initialize : Reason listed below :");
            perror ("WAVE");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    // Initialize Random Generator

    srand (time (NULL));

    // Set the Management Interface Role

    FrameworkToolKit::setManagementInterfaceRole (WAVE_MGMT_INTF_ROLE_SERVER);

    // Set if the database is to be enabled.

    DatabaseObjectManager::setIsDatabaseEnabled (waveMainConfiguration.getIsDatabaseEnabled ());
    DatabaseObjectManager::setDatabasePort (waveMainConfiguration.getDatabasePort ());

    // First record the current working directory as the directory in which the process was started

    FrameworkToolKit::initialize ();

    //Initialize time zone information

    TimeZoneToolKit::initialize ();

    // Setup the System Management;

    SystemManagementToolKit::setYinPaths          (waveMainConfiguration.getYinPaths ());
    SystemManagementToolKit::setWyserTagsFilePath (waveMainConfiguration.getWyserTagsFilePath ());

    // set the Wave Framework configuration file.

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

    // Set database logging information and if logging is required.

    DatabaseObjectManager::setDatabaseLogFileName                      (getTraceFileDirectory () + "/" + waveMainConfiguration.getApplicationCompactName () + "." + waveMainConfiguration.getDatabaseLogFileName () + ".log");
    DatabaseObjectManager::setIsDatabaseLogEnabled                     (waveMainConfiguration.getIsDatabaseLogEnabled ());
    DatabaseObjectManager::setEmptyDatabaseType                        (waveMainConfiguration.getDatabaseEmptyType ());
    DatabaseObjectManager::setEmptyDatabaseAutoDetectionThresholdValue (waveMainConfiguration.getDatabaseEmptyTypeAutoDetectionThresholdValue ());
    // Set database runtime parameters

    DatabaseObjectManager::setDatabaseSharedBuffers (waveMainConfiguration.getDatabaseSharedBuffers ());

    // Run the process in daemon mode if required.

    if (true == (waveMainConfiguration.getIsADaemon ()))
    {
        daemon (1, 0);
        FrameworkToolKit::initializeConsolePrintFunction ();
    }

    // Record if the process is running as a daemon.

    FrameworkToolKit::setIsRunningAsADaemon (waveMainConfiguration.getIsADaemon ());

    // Add ORM Repository Most Base Classes information from Wave.

    OrmRepository::addMostBaseClass (PrismPersistableObject::getClassName ());
    OrmRepository::addMostBaseClass (WaveManagedObject::getClassName      ());
    OrmRepository::addMostBaseClass (WaveLocalManagedObject::getClassName ());

    // PrismFrameworkObjectManager must be the first one to be created.

    PrismFrameworkObjectManager::getInstance ();

    // ShellObjectManager must be the next one so that all can register their shells and related shell handlers.

    if (true == m_enableShellSupport)
    {
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
    trace(TRACE_LEVEL_PERF_START, "Wave Bootup.");

    // Below this one the services are registered in reverse order.  The last one will be on top of the list.

    if (true == m_enableBuiltInSelfTestSupport)
    {
		registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkTestability6ObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (DistributedLogTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FileLocalMessagingTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (PersistenceTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ClusterTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TimerTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TraceTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (LicenseTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkRemoteMessagingTestObjectManager2::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkRemoteMessagingTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkLocalMessagingTestObjectManager2::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkLocalMessagingTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkTestability5ObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkTestability4ObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkTestability3ObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkTestability2ObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkTestability1ObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FrameworkCoreTestObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (RegressionTestObjectManager::getInstance));
    }

    if (true == m_enableSystemManagementSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (CommandLineInterfaceReceiverObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveSystemManagementObjectManager::getInstance));
    }

    if (true == m_enablePersistenceSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (MultiPartitionObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (MultiPartitionLocalObjectManager::getInstance));
    }

    if (true == m_enableClusteringSupport)
    {
		registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (DistributedDebugGlobalObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (DistributedDebugLocalObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (LockManagementObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (InterLocationMessageReceiverObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (InterLocationMessageTransportObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (CentralClusterConfigObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ClusterLocalObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (LocalClusterConfigObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ClusterInterfaceObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (HeartBeatObjectManager::getInstance));
    }

    if (true == m_enableManagementInterfaceSupport)
    {

        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (HttpInterfaceReceiverObjectManager::getInstance));

        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ManagementInterfaceReceiverObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ManagementInterfaceObjectManager::getInstance));

        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveUserInterfaceObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveClientTransportObjectManager::getInstance));
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (WaveClientReceiverObjectManager::getInstance));
    }

    if (true == m_enableFileServiceSupport)
    {
         registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FileObjectManager::getInstance),false);
         registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (FileLocalObjectManager::getInstance),false);
    }

    if (true == m_enableHaPeerSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (HaPeerMessageTransportObjectManager::getInstance),false);
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (HaPeerMessageReceiverObjectManager::getInstance),false);
    }

    if (true == m_enableTraceSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TraceObjectManager::getInstance));
    }

    if (true == m_enableSoftwareManagementSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (SoftwareManagementObjectManager::getInstance));
    }

    if (true == m_enableLicensingSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (LicenseObjectManager::getInstance));
    }

    if (true == m_enableDistributedLogSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (DistributedLogObjectManager::getInstance));
    }

    if (true == m_enablePersistenceSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (PersistenceObjectManager::getInstance), false);
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (PersistenceLocalObjectManager::getInstance), false);
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (DatabaseObjectManager::getInstance), false);
    }

    registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (ReservedWaveLocalObjectManager::getInstance), false);

    // WARNING! WARNING! WARNING!
    // Always keep the Timer Object Manager as the last registered internal native service.
    // All services will cancel their timers with the Timer Object Manager during shutdown so this service must be the last to shutdown.

    if (true == m_enableTimerSupport)
    {
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TimerObjectManager::getInstance), false);
        registerNativeServiceInternal (reinterpret_cast<NativePrismServiceInstantiator> (TimerSignalObjectManager::getInstance), false);
    }

    // Instantiate Native Wave Services here

    instantiateNativePrismServices ();

    // Start the ObjectManager s corresponding to Application Services

    ApplicationService::createNewApplicationServices ();

    // start the ObjectManager s corresponding to Application Local Services

    ApplicationLocalService::createNewApplicationLocalServices ();

    // Starting Application specfic Object managers.

    AppObjectManager::createAppObjectMangers ();
}

string Wave::getTraceFileDirectory ()
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

string Wave::getConfigurationFileDirectory ()
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

string Wave::getProfileFileDirectory ()
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

void Wave::registerNativeService (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase, const ResourceId &serviceMode)
{
    m_nativePrismServiceInstantiators.push_back                (pNativePrismServiceInstantiator);
    m_nativePrismServiceInstantiatorIsForNormalPhase.push_back (isForNormalPhase);
    m_nativePrismServiceInstantiationMode.push_back            (serviceMode);
}

void Wave::registerApplicationSpecificNativeServices (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase, const ResourceId &serviceMode)
{
    m_nativeApplicationSpecificPrismServiceInstantiators.push_back (pNativePrismServiceInstantiator);

    registerNativeService (pNativePrismServiceInstantiator, isForNormalPhase, serviceMode);
}

void Wave::registerNativeServiceInternal (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase, const ResourceId &serviceMode)
{
    m_nativePrismServiceInstantiators.insert                (m_nativePrismServiceInstantiators.begin (), pNativePrismServiceInstantiator);
    m_nativePrismServiceInstantiatorIsForNormalPhase.insert (m_nativePrismServiceInstantiatorIsForNormalPhase.begin (), isForNormalPhase);
    m_nativePrismServiceInstantiationMode.insert            (m_nativePrismServiceInstantiationMode.begin (), serviceMode);
}

void Wave::registerNativeService (NativeMultiplePrismServiceInstantiator pNativeMultiplePrismServiceInstantiator)
{
    m_nativeMultiplePrismServiceInstantiators.push_back (pNativeMultiplePrismServiceInstantiator);
}

void Wave::registerPersistencePostBootCheck (PersistencePostBootCheck pPersistencePostBootCheck)
{
    m_persistencePostBootCheck = pPersistencePostBootCheck;
}

ResourceId Wave::persistencePostBootCheck ()
{
    return ((m_persistencePostBootCheck ()));
}

void Wave::registerClusterEnabledCheck (ClusterEnabledCheck pClusterEnabledCheck)
{
    m_clusterEnabledCheckMutex.lock ();

    m_clusterEnabledCheck = pClusterEnabledCheck;

    m_clusterEnabledCheckMutex.unlock ();
}

bool Wave::clusterEnabledCheck ()
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

void Wave::registerIsWaveClusteringEnabledFunction (IsWaveClusteringEnabled pIsWaveClusteringEnabled)
{
    m_isWaveClusteringEnabledMutex.lock ();
    m_isWaveClusteringEnabled = pIsWaveClusteringEnabled ;
    m_isWaveClusteringEnabledMutex.unlock ();
}

bool Wave::isWaveClusteringEnabled ()
{
    bool mode = false;
    m_isWaveClusteringEnabledMutex.lock ();
    mode = m_isWaveClusteringEnabled();
    m_isWaveClusteringEnabledMutex.unlock ();

    return mode;
}

void Wave::registerLogOperationStatusFunction (LogOperationStatusFunction pLogOperationStatusFunction)
{
    m_logOperationStatusMutex.lock ();

    m_logOperationStatusFunction = pLogOperationStatusFunction;

    m_logOperationStatusMutex.unlock ();
}

void Wave::logOperationStatus (ResourceId status)
{
    trace (TRACE_LEVEL_DEBUG, string("Wave::logOperationStatus : status = ") + status);

    m_logOperationStatusMutex.lock ();
    m_logOperationStatusFunction (status);
    m_logOperationStatusMutex.unlock ();
}

void Wave::registerGetLinecardReadySlotIdBitMapFunction ( GetLinecardReadySlotIdBitMapFunction plinecardReadySlotIdBitMapFunction )
{
    m_linecardReadySlotIdBitMapFunctionMutex.lock();

    m_linecardReadySlotIdBitMapFunction = plinecardReadySlotIdBitMapFunction;

    m_linecardReadySlotIdBitMapFunctionMutex.unlock();
}

void Wave::getLinecardReadySlotIdBitMap ( BitMap &linecardReadySlotIdBitMap , UI32 &maxLinecards )
{
    m_linecardReadySlotIdBitMapFunctionMutex.lock();

    if ( NULL != m_linecardReadySlotIdBitMapFunction )
    {
        m_linecardReadySlotIdBitMapFunction ( linecardReadySlotIdBitMap, maxLinecards );
    }

    m_linecardReadySlotIdBitMapFunctionMutex.unlock();    
}

void Wave::setWaveUserClientPath (const string &path)
{
    m_waveUserClientPath = path;
}

string Wave::getWaveUserClientPath ()
{
    return (m_waveUserClientPath);
}

void Wave::setWaveUserClientParams (const string &params)
{
    m_waveUserClientParams = params;
}

string Wave::getWaveUserClientParams ()
{
    return (m_waveUserClientParams);
}

void Wave::instantiateNativePrismServices ()
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

    trace (TRACE_LEVEL_INFO, string ("Instantiated ") + numberOfNativePrismServciesToInstantiate + " Native Wave Services");
}

void Wave::waveExit ()
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

extern "C" void setPrismTraceFileDirectory (const char *pPrismTraceFileDirectory)
{
    s_waveTraceFileDirectory = pPrismTraceFileDirectory;
}

extern "C" void setPrismConfigurationFileDirectory (const char *pPrismConfigurationFileDirectory)
{
    s_waveConfigurationFileDirectory = pPrismConfigurationFileDirectory;
}
