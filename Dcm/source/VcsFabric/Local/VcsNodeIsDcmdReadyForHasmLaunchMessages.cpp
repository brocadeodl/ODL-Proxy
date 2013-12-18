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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeIsDcmdReadyForHasmLaunchMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeSetIsDcmdReadyForHasmLaunchMessage::VcsNodeSetIsDcmdReadyForHasmLaunchMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_IS_DCMD_READY_FOR_HASM_LAUNCH)
{
    m_isDcmdReadyForHasmLaunch = false;
}

VcsNodeSetIsDcmdReadyForHasmLaunchMessage::VcsNodeSetIsDcmdReadyForHasmLaunchMessage (const bool &isDcmdReadyForHasmLaunch)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_IS_DCMD_READY_FOR_HASM_LAUNCH),
      m_isDcmdReadyForHasmLaunch              (isDcmdReadyForHasmLaunch)
{
}

VcsNodeSetIsDcmdReadyForHasmLaunchMessage::~VcsNodeSetIsDcmdReadyForHasmLaunchMessage ()
{
}

void VcsNodeSetIsDcmdReadyForHasmLaunchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isDcmdReadyForHasmLaunch, "isDcmdReadyForHasmLaunch"));
}

void VcsNodeSetIsDcmdReadyForHasmLaunchMessage::setIsDcmdReadyForHasmLaunch (const bool &isDcmdReadyForHasmLaunch)
{
    m_isDcmdReadyForHasmLaunch = isDcmdReadyForHasmLaunch;
}

bool VcsNodeSetIsDcmdReadyForHasmLaunchMessage::getIsDcmdReadyForHasmLaunch () const
{
    return (m_isDcmdReadyForHasmLaunch);
}

/************************************ 
* GET  IsDcmdReadyForHasmLaunch
*************************************/

VcsNodeGetIsDcmdReadyForHasmLaunchMessage::VcsNodeGetIsDcmdReadyForHasmLaunchMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_IS_DCMD_READY_FOR_HASM_LAUNCH)
{
    m_isDcmdReadyForHasmLaunch = false;
}

VcsNodeGetIsDcmdReadyForHasmLaunchMessage::~VcsNodeGetIsDcmdReadyForHasmLaunchMessage ()
{
}

void VcsNodeGetIsDcmdReadyForHasmLaunchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isDcmdReadyForHasmLaunch, "isDcmdReadyForHasmLaunch"));
}

void VcsNodeGetIsDcmdReadyForHasmLaunchMessage::setIsDcmdReadyForHasmLaunch (const bool &isDcmdReadyForHasmLaunch)
{
    m_isDcmdReadyForHasmLaunch = isDcmdReadyForHasmLaunch;
}

bool VcsNodeGetIsDcmdReadyForHasmLaunchMessage::getIsDcmdReadyForHasmLaunch () const
{
    return (m_isDcmdReadyForHasmLaunch);
}

}
