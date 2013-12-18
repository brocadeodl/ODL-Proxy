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

#ifndef WAVEMESSAGEBROKERCLIENT_H
#define WAVEMESSAGEBROKERCLIENT_H

#include "Framework/Types/Types.h"

#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "License/LicenseObjectManager.h"
#include "License/Test/LicenseTestObjectManager.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Trace/Test/TraceTestObjectManager.h"
#include "Regression/RegressionTestObjectManager.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "Framework/Messaging/Remote/InterLocationMessageReceiverObjectManager.h"
#include "Framework/Messaging/Remote/InterLocationMessageTransportObjectManager.h"
#include "Shell/ShellObjectManager.h"
#include "Framework/Core/Test/FrameworkCoreTestObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager2.h"
#include "Framework/Messaging/Remote/Test/FrameworkRemoteMessagingTestObjectManager.h"
#include "Framework/Messaging/Remote/Test/FrameworkRemoteMessagingTestObjectManager2.h"
#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability2ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability3ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability4ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability5ObjectManager.h"
#include "Framework/Timer/TimerObjectManager.h"
#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Timer/Test/TimerTestObjectManager.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/LocalClusterConfigObjectManager.h"
#include "Cluster/Local/HeartBeat/HeartBeatObjectManager.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "App/AppInterfaceObjectManager.h"
#include "App/AppObjectManager.h"
#include "App/ClusterInterfaceObjectManager.h"
#include "SoftwareManagement/SoftwareManagementObjectManager.h"
#include "Cluster/Test/ClusterTestObjectManager.h"
#include "Framework/ServiceInterface/ApplicationService.h"
#include "Framework/ServiceInterface/ApplicationLocalService.h"
#include "Framework/Core/WaveMainConfiguration.h"
#include "File/Global/FileObjectManager.h"
#include "File/Local/FileLocalObjectManager.h"
#include "File/Local/Test/FileLocalMessagingTestObjectManager.h"
#include "LockManagement/LockManagementObjectManager.h"


#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

typedef        WaveObjectManager *  (* NativePrismServiceInstantiator) ();
typedef vector<WaveObjectManager *> (* NativeMultiplePrismServiceInstantiator) ();
typedef        ResourceId           (* PersistencePostBootCheck) ();
typedef        bool                 (* ClusterEnabledCheck) ();
typedef        void                 (* LogOperationStatusFunction) (ResourceId);
typedef        bool                 (* IsWaveClusteringEnabled)();
typedef        void                 (* GetLinecardReadySlotIdBitMapFunction) ( BitMap &, UI32 &);

class WaveMessageBrokerClient
{
    private :
    protected :
    public :
        static void         initialize                                (const WaveMainConfiguration &waveMainConfiguration);
        static string       getTraceFileDirectory                     ();
        static string       getConfigurationFileDirectory             ();
        static string       getProfileFileDirectory                   ();

        static void         registerNativeService                     (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase = true, const ResourceId &serviceMode = WAVE_SERVICE_ACTIVE);
        static void         registerApplicationSpecificNativeServices (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase = true, const ResourceId &serviceMode = WAVE_SERVICE_ACTIVE);
        static void         registerNativeServiceInternal             (NativePrismServiceInstantiator pNativePrismServiceInstantiator, const bool &isForNormalPhase = true, const ResourceId &serviceMode = WAVE_SERVICE_ACTIVE);
        static void         registerNativeService                     (NativeMultiplePrismServiceInstantiator pNativeMultiplePrismServiceInstantiator);
        static void         registerPersistencePostBootCheck          (PersistencePostBootCheck pPersistencePostBootCheck);
        static void         registerClusterEnabledCheck               (ClusterEnabledCheck pClusterEnabledCheck);
        static void         registerLogOperationStatusFunction        (LogOperationStatusFunction pLogOperationStatusFunction);
        static void         registerIsWaveClusteringEnabledFunction   (IsWaveClusteringEnabled pIsWaveClusteringEnabled);
        static void         registerGetLinecardReadySlotIdBitMapFunction ( GetLinecardReadySlotIdBitMapFunction  pGetLinecardReadySlotIdBitMapFunction);

        static void         enableAllFeatures                         ();
        static void         enablePersistence                         ();
        static void         disablePersistence                        ();
        static void         enableClustering                          ();
        static void         disableClustering                         ();
        static void         enableBuiltInSelfTest                     ();
        static void         disableBuiltInSelfTest                    ();
        static void         enableShell                               ();
        static void         disableShell                              ();
        static void         enableManagementInterface                 ();
        static void         disableManagementInterface                ();
        static void         enableLicense                             ();
        static void         disableLicense                            ();
        static void         enableFileService                         ();
        static void         disableFileService                        ();
        static void         enableSoftwareManagement                  ();
        static void         disableSoftwareManagement                 ();
        static void         enableTimer                               ();
        static void         disableTimer                              ();
        static void         enableTrace                               ();
        static void         disableTrace                              ();
        static void         enableDistributedLog                      ();
        static void         disableDistributedLog                     ();
        static void         enableHaPeer                              ();
        static void         disableHaPeer                             ();
        static void         enableSystemManagement                    ();
        static void         disableSystemManagement                   ();

        static ResourceId   persistencePostBootCheck              ();
        static bool         clusterEnabledCheck                   ();
        static bool         isWaveClusteringEnabled               ();
        static string       getWaveUserClientPath                 ();
        static void         setWaveUserClientPath                 (const string &path);
        static string       getWaveUserClientParams               ();
        static void         setWaveUserClientParams               (const string &params);
        static void         logOperationStatus                    (UI32 status);
        static void         getLinecardReadySlotIdBitMap          ( BitMap &linecardReadySlotIdBitMap, UI32 &maxLinecards );

        static void         instantiateNativePrismServices ();
        static void         waveExit ();
    // Now the data members

    private :
        static vector<NativePrismServiceInstantiator>         m_nativePrismServiceInstantiators;
        static vector<bool>                                   m_nativePrismServiceInstantiatorIsForNormalPhase;
        static vector<NativeMultiplePrismServiceInstantiator> m_nativeMultiplePrismServiceInstantiators;
        static vector<NativePrismServiceInstantiator>         m_nativeApplicationSpecificPrismServiceInstantiators;
        static vector<ResourceId>                             m_nativePrismServiceInstantiationMode;
        static PersistencePostBootCheck                       m_persistencePostBootCheck;
        static LogOperationStatusFunction                     m_logOperationStatusFunction;
        static PrismMutex                                     m_logOperationStatusMutex;
        static ClusterEnabledCheck                            m_clusterEnabledCheck;
        static PrismMutex                                     m_clusterEnabledCheckMutex;
        static IsWaveClusteringEnabled                        m_isWaveClusteringEnabled;
        static PrismMutex                                     m_isWaveClusteringEnabledMutex;
        static GetLinecardReadySlotIdBitMapFunction           m_linecardReadySlotIdBitMapFunction;
        static PrismMutex                                     m_linecardReadySlotIdBitMapFunctionMutex;
        static bool                                           m_enableFileServiceSupport;
        static bool                                           m_enablePersistenceSupport;
        static bool                                           m_enableClusteringSupport;
        static bool                                           m_enableBuiltInSelfTestSupport;
        static bool                                           m_enableLicensingSupport;
        static bool                                           m_enableSoftwareManagementSupport;
        static bool                                           m_enableTraceSupport;
        static bool                                           m_enableDistributedLogSupport;
        static bool                                           m_enableShellSupport;
        static bool                                           m_enableManagementInterfaceSupport;
        static bool                                           m_enableTimerSupport;
        static bool                                           m_enableHaPeerSupport;
        static bool                                           m_enableSystemManagementSupport;
        static string                                         m_waveUserClientPath;
        static string                                         m_waveUserClientParams;

    protected :
    public :
};

}

#endif // WAVEMESSAGEBROKERCLIENT_H
