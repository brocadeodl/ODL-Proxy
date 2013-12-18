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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodePostBootMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeTriggerPostBootMessage::VcsNodeTriggerPostBootMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_TRIGGER_POSTBOOT)
{
    m_checkNotifyPhase2CompletedBool = false;
}

VcsNodeTriggerPostBootMessage::VcsNodeTriggerPostBootMessage (const bool &checkNotifyPhase2CompletedBool)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_TRIGGER_POSTBOOT),
      m_checkNotifyPhase2CompletedBool (checkNotifyPhase2CompletedBool)
{
}

VcsNodeTriggerPostBootMessage::~VcsNodeTriggerPostBootMessage ()
{
}

void VcsNodeTriggerPostBootMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_checkNotifyPhase2CompletedBool, "checkNotifyPhase2CompletedBool"));
}

void VcsNodeTriggerPostBootMessage::setCheckNotifyPhase2CompletedBool (const bool &checkNotifyPhase2CompletedBool)
{
    m_checkNotifyPhase2CompletedBool = checkNotifyPhase2CompletedBool;
}

bool VcsNodeTriggerPostBootMessage::getCheckNotifyPhase2CompletedBool () const
{
    return (m_checkNotifyPhase2CompletedBool);
}

}
