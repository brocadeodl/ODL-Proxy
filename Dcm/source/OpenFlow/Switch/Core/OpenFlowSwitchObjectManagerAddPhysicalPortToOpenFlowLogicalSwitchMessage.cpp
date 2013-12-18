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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddPhysicalPortToOpenFlowLogicalSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"

namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerAddPhysicalPortToOpenFlowLogicalSwitchMessage::OpenFlowSwitchObjectManagerAddPhysicalPortToOpenFlowLogicalSwitchMessage ()
    : ManagementInterfaceMessage (OpenFlowSwitchObjectManager::getServiceName (), OPENFLOW_OBJECT_MANAGER_ADD_PHYSICAL_PORT_TO_OPENFLOW_LOGICAL_SWITCH)
{
}

OpenFlowSwitchObjectManagerAddPhysicalPortToOpenFlowLogicalSwitchMessage::~OpenFlowSwitchObjectManagerAddPhysicalPortToOpenFlowLogicalSwitchMessage ()
{
}

void OpenFlowSwitchObjectManagerAddPhysicalPortToOpenFlowLogicalSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

}

