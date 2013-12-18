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

 /**************************************************************************
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Pradeep Yelamanti                                            *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeSetModeChangeParametersMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"


namespace DcmNs
{

VcsNodeSetModeChangeParametersMessage::VcsNodeSetModeChangeParametersMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_FABRIC_SET_MODE_CHANGE_PARAMETERS),
    m_vcsId (0), m_vcsMode ("SA"), m_rbridgeId (0), m_rebootSw (true)
{
}

VcsNodeSetModeChangeParametersMessage::VcsNodeSetModeChangeParametersMessage (const UI32 &vcsId, const string &vcsMode, const UI32 &rbridgeId, const bool &rebootSw)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_FABRIC_SET_MODE_CHANGE_PARAMETERS),
    m_vcsId (vcsId), m_vcsMode (vcsMode), m_rbridgeId (rbridgeId), m_rebootSw (rebootSw)
{
}

VcsNodeSetModeChangeParametersMessage::~VcsNodeSetModeChangeParametersMessage ()
{
}

void VcsNodeSetModeChangeParametersMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32   (&m_vcsId,    "vcsId"));
    addSerializableAttribute (new AttributeString (&m_vcsMode,  "vcsMode"));
    addSerializableAttribute (new AttributeUI32   (&m_rbridgeId,"rbridgeId"));
    addSerializableAttribute (new AttributeBool   (&m_rebootSw, "rebootSw"));
}

UI32 VcsNodeSetModeChangeParametersMessage::getVcsId () const
{
    return (m_vcsId);
}

void VcsNodeSetModeChangeParametersMessage::setVcsId (const UI32 &vcsId)
{
    m_vcsId = vcsId;
}

string VcsNodeSetModeChangeParametersMessage::getVcsMode () const
{
    return (m_vcsMode);
}

void VcsNodeSetModeChangeParametersMessage::setVcsMode (const string &vcsMode)
{
    m_vcsMode = vcsMode;
}

UI32 VcsNodeSetModeChangeParametersMessage::getRbridgeId () const
{
    return (m_rbridgeId);
}

void VcsNodeSetModeChangeParametersMessage::setRbridgeId (const UI32 &rbridgeId)
{
    m_rbridgeId = rbridgeId;
}

bool VcsNodeSetModeChangeParametersMessage::getRebootSw () const
{
    return (m_rebootSw);
}

void VcsNodeSetModeChangeParametersMessage::setRebootSw (const bool &rebootSw)
{
    m_rebootSw = rebootSw;
}

}

