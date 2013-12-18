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

#include "VcsFabric/Local/VcsNodeIsHasmLaunchedMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeSetIsHasmLaunchedMessage::VcsNodeSetIsHasmLaunchedMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_IS_HASM_LAUNCHED)
{
    m_isHasmLaunched = false;
}

VcsNodeSetIsHasmLaunchedMessage::VcsNodeSetIsHasmLaunchedMessage (const bool &isHasmLaunched)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_IS_HASM_LAUNCHED),
      m_isHasmLaunched              (isHasmLaunched)
{
}

VcsNodeSetIsHasmLaunchedMessage::~VcsNodeSetIsHasmLaunchedMessage ()
{
}

void VcsNodeSetIsHasmLaunchedMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isHasmLaunched, "isHasmLaunched"));
}

void VcsNodeSetIsHasmLaunchedMessage::setIsHasmLaunched (const bool &isHasmLaunched)
{
    m_isHasmLaunched = isHasmLaunched;
}

bool VcsNodeSetIsHasmLaunchedMessage::getIsHasmLaunched () const
{
    return (m_isHasmLaunched);
}

/************************************ 
* GET  IsHasmLaunched
*************************************/

VcsNodeGetIsHasmLaunchedMessage::VcsNodeGetIsHasmLaunchedMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_IS_HASM_LAUNCHED)
{
    m_isHasmLaunched = false;
}

VcsNodeGetIsHasmLaunchedMessage::~VcsNodeGetIsHasmLaunchedMessage ()
{
}

void VcsNodeGetIsHasmLaunchedMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isHasmLaunched, "isHasmLaunched"));
}

void VcsNodeGetIsHasmLaunchedMessage::setIsHasmLaunched (const bool &isHasmLaunched)
{
    m_isHasmLaunched = isHasmLaunched;
}

bool VcsNodeGetIsHasmLaunchedMessage::getIsHasmLaunched () const
{
    return (m_isHasmLaunched);
}

}
