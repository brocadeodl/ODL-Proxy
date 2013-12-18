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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeCopyDefaultStartMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeCopyDefaultStartTriggerMessage::VcsNodeCopyDefaultStartTriggerMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_COPY_DEFAULT_START_TRIGGER)
{
    m_vcsMode = 0;
}

VcsNodeCopyDefaultStartTriggerMessage::VcsNodeCopyDefaultStartTriggerMessage (const UI32 &vcsMode)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_COPY_DEFAULT_START_TRIGGER),
      m_vcsMode                    (vcsMode)
{
}

VcsNodeCopyDefaultStartTriggerMessage::~VcsNodeCopyDefaultStartTriggerMessage ()
{
}

void VcsNodeCopyDefaultStartTriggerMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_vcsMode, "vcsMode"));
}

UI32 VcsNodeCopyDefaultStartTriggerMessage::getVcsMode () const
{
    return (m_vcsMode);
}

}
