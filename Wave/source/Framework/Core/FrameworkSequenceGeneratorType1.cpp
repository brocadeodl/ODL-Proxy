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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/FrameworkSequenceGeneratorType1.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/Test/FrameworkCoreTestObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability2ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability3ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability4ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability5ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability6ObjectManager.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager2.h"
#include "Framework/Messaging/Remote/Test/FrameworkRemoteMessagingTestObjectManager.h"
#include "Shell/ShellObjectManager.h"
#include "License/LicenseObjectManager.h"
#include "License/Test/LicenseTestObjectManager.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Trace/Test/TraceTestObjectManager.h"
#include "Regression/RegressionTestObjectManager.h"
#include "App/ClusterInterfaceObjectManager.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/LocalClusterConfigObjectManager.h"
#include "Cluster/Local/HeartBeat/HeartBeatObjectManager.h"
#include "Cluster/Test/ClusterTestObjectManager.h"
#include "Framework/Timer/TimerObjectManager.h"
#include "Framework/Timer/TimerSignalObjectManager.h"
#include "Framework/Timer/Test/TimerTestObjectManager.h"
#include "SoftwareManagement/SoftwareManagementObjectManager.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "Framework/Messaging/Remote/InterLocationMessageTransportObjectManager.h"
#include "Framework/Messaging/Remote/InterLocationMessageReceiverObjectManager.h"
#include "ManagementInterface/ManagementInterfaceReceiverObjectManager.h"
#include "Framework/Persistence/Test/PersistenceTestObjectManager.h"
#include "LockManagement/LockManagementObjectManager.h"

namespace WaveNs
{

FrameworkSequenceGeneratorType1::FrameworkSequenceGeneratorType1 ()
    : FrameworkSequenceGenerator ()
{
    // Initialize

    m_prismServiceIdsToInitializeDuringPrePhase.push_back (TraceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitializeDuringPrePhase.push_back (TimerSignalObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitializeDuringPrePhase.push_back (TimerObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitializeDuringPrePhase.push_back (DatabaseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitializeDuringPrePhase.push_back (PersistenceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitializeDuringPrePhase.push_back (InterLocationMessageReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitializeDuringPrePhase.push_back (InterLocationMessageTransportObjectManager::getPrismServiceId ());

    m_prismServiceIdsToInitialize.push_back (SoftwareManagementObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (ManagementInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (ManagementInterfaceReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (LocalClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (CentralClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (HeartBeatObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (ClusterInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (LockManagementObjectManager::getPrismServiceId ());

    m_prismServiceIdsToInitialize.push_back (TraceTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkCoreTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkTestability1ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkTestability2ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkTestability3ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkTestability4ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkTestability5ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkTestability6ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkLocalMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (TimerTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (ClusterTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (PersistenceTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToInitialize.push_back (RegressionTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInitialize.push_back (ShellObjectManager::getPrismServiceId ());

    // Enable

    m_prismServiceIdsToEnableDuringPrePhase.push_back (TraceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnableDuringPrePhase.push_back (TimerSignalObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnableDuringPrePhase.push_back (TimerObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnableDuringPrePhase.push_back (DatabaseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnableDuringPrePhase.push_back (PersistenceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnableDuringPrePhase.push_back (InterLocationMessageReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnableDuringPrePhase.push_back (InterLocationMessageTransportObjectManager::getPrismServiceId ());

    m_prismServiceIdsToEnable.push_back (SoftwareManagementObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (ManagementInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (ManagementInterfaceReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (LocalClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (CentralClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (HeartBeatObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (ClusterInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (LockManagementObjectManager::getPrismServiceId ());

    m_prismServiceIdsToEnable.push_back (TraceTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkCoreTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkTestability1ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkTestability2ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkTestability3ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkTestability4ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkTestability5ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkTestability6ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkLocalMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (TimerTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (ClusterTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (PersistenceTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToEnable.push_back (RegressionTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToEnable.push_back (ShellObjectManager::getPrismServiceId ());

    // Install

    m_prismServiceIdsToInstallDuringPrePhase.push_back (TraceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstallDuringPrePhase.push_back (TimerSignalObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstallDuringPrePhase.push_back (TimerObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstallDuringPrePhase.push_back (DatabaseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstallDuringPrePhase.push_back (PersistenceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstallDuringPrePhase.push_back (InterLocationMessageReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstallDuringPrePhase.push_back (InterLocationMessageTransportObjectManager::getPrismServiceId ());

    m_prismServiceIdsToInstall.push_back (SoftwareManagementObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (ManagementInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (ManagementInterfaceReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (LocalClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (CentralClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (HeartBeatObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (ClusterInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (LockManagementObjectManager::getPrismServiceId ());

    m_prismServiceIdsToInstall.push_back (TraceTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkCoreTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkTestability1ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkTestability2ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkTestability3ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkTestability4ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkTestability5ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkTestability6ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkLocalMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (TimerTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (ClusterTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (PersistenceTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToInstall.push_back (RegressionTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToInstall.push_back (ShellObjectManager::getPrismServiceId ());

    // Boot

    m_prismServiceIdsToBootDuringPrePhase.push_back (TraceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBootDuringPrePhase.push_back (TimerSignalObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBootDuringPrePhase.push_back (TimerObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBootDuringPrePhase.push_back (DatabaseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBootDuringPrePhase.push_back (PersistenceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBootDuringPrePhase.push_back (InterLocationMessageReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBootDuringPrePhase.push_back (InterLocationMessageTransportObjectManager::getPrismServiceId ());

    m_prismServiceIdsToBoot.push_back (SoftwareManagementObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (ManagementInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (ManagementInterfaceReceiverObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (LocalClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (CentralClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (HeartBeatObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (ClusterInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (LockManagementObjectManager::getPrismServiceId ());

    m_prismServiceIdsToBoot.push_back (TraceTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkCoreTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkTestability1ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkTestability2ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkTestability3ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkTestability4ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkTestability5ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkTestability6ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkLocalMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (TimerTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (ClusterTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (PersistenceTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToBoot.push_back (RegressionTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToBoot.push_back (ShellObjectManager::getPrismServiceId ());

    // Shutdown

    m_prismServiceIdsToShutdown.push_back (ShellObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (RegressionTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToShutdown.push_back (LocalClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (CentralClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (HeartBeatObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (ClusterInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (LockManagementObjectManager::getPrismServiceId ());

    m_prismServiceIdsToShutdown.push_back (TraceTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkCoreTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkTestability1ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkTestability2ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkTestability3ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkTestability4ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkTestability5ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkTestability6ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkLocalMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (TimerTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (ClusterTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (PersistenceTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToShutdown.push_back (ManagementInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (InterLocationMessageTransportObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (RegressionTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (TimerObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (TimerSignalObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (SoftwareManagementObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (PersistenceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (DatabaseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToShutdown.push_back (TraceObjectManager::getPrismServiceId ());

    // Disable

    m_prismServiceIdsToDisable.push_back (ShellObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (RegressionTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToDisable.push_back (LocalClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (CentralClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (HeartBeatObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (ClusterInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (LockManagementObjectManager::getPrismServiceId ());

    m_prismServiceIdsToDisable.push_back (TraceTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkCoreTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkTestability1ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkTestability2ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkTestability3ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkTestability4ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkTestability5ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkTestability6ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkLocalMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (TimerTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (ClusterTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (PersistenceTestObjectManager::getPrismServiceId ());

    //m_prismServiceIdsToDisable.push_back (PrismFrameworkObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (ManagementInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (InterLocationMessageTransportObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (RegressionTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (TimerObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (TimerSignalObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (SoftwareManagementObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (PersistenceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (DatabaseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToDisable.push_back (TraceObjectManager::getPrismServiceId ());

    // Failover

    m_prismServiceIdsToFailover.push_back (TraceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (TimerObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (DatabaseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (PersistenceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (InterLocationMessageTransportObjectManager::getPrismServiceId ());

    m_prismServiceIdsToFailover.push_back (LicenseObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (SoftwareManagementObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (ManagementInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (LocalClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (CentralClusterConfigObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (HeartBeatObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (ClusterInterfaceObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (LockManagementObjectManager::getPrismServiceId ());

    m_prismServiceIdsToFailover.push_back (LicenseTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (TraceTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkCoreTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkTestability1ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkTestability2ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkTestability3ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkTestability4ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkTestability5ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkTestability6ObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkLocalMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkLocalMessagingTestObjectManager2::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (FrameworkRemoteMessagingTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (TimerTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (ClusterTestObjectManager::getPrismServiceId ());
    m_prismServiceIdsToFailover.push_back (PersistenceTestObjectManager::getPrismServiceId ());

    m_prismServiceIdsToFailover.push_back (RegressionTestObjectManager::getPrismServiceId ());
}

FrameworkSequenceGeneratorType1::~FrameworkSequenceGeneratorType1 ()
{
}

}
