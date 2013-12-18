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

#include "Framework/Core/Wave.h"
#include "DcmCore/Nsp.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/Test/VcsNodeLocalMessagingTestObjectManager.h"
#include "VcsFabric/Local/Test/VcsNodeLocalEventTestObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"

using namespace WaveNs;
using namespace DcmNs;

namespace NspNs
{

void Nsp::initialize ()
{
    // Setup DCM specific Most ORM Base Classes

    OrmRepository::addMostBaseClass (DcmManagedObject::getClassName ());
    OrmRepository::addMostBaseClass (DcmLocalManagedObject::getClassName ());

    // register a function with persistence object manager, which tells whether Management Cluster is enabled.
    VcsFabricLocalObjectManager::registerClusterEnabledCheckFunctionWithPersistence ();

    // Now add the Dcm specific services to Framework.
    Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (VcsFabObjectManager::getInstance));
    Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (VcsFabricLocalObjectManager::getInstance));
    Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (VcsNodeFabosInterfaceObjectManager::getInstance));
    Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VcsNodeLocalMessagingTestObjectManager::getInstance));
    Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VcsNodeLocalEventTestObjectManager::getInstance));

    Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NetworkConfigurationObjectManager::getInstance));
    Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NetworkConfigurationLocalObjectManager::getInstance));
}

}
