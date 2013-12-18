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

#include "VcsFabric/Local/VcsNodeVcsIdMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeSetVcsIdMessage::VcsNodeSetVcsIdMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_VCSID)
{
    m_vcsId = 0;
}

VcsNodeSetVcsIdMessage::VcsNodeSetVcsIdMessage (const UI32 &vcsId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_VCSID),
      m_vcsId                    (vcsId)
{
}

VcsNodeSetVcsIdMessage::~VcsNodeSetVcsIdMessage ()
{
}

void VcsNodeSetVcsIdMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_vcsId, "vcsId"));
}

UI32 VcsNodeSetVcsIdMessage::getVcsId () const
{
    return (m_vcsId);
}

/************************************ 
* GET VCS_ID
*************************************/

VcsNodeGetVcsIdMessage::VcsNodeGetVcsIdMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_VCSID)
{
    m_vcsId = 0;
}

VcsNodeGetVcsIdMessage::~VcsNodeGetVcsIdMessage ()
{
}

void VcsNodeGetVcsIdMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_vcsId, "vcsId"));
}

void VcsNodeGetVcsIdMessage::setVcsId (const UI32 &vcsId)
{
    m_vcsId = vcsId;
}

UI32 VcsNodeGetVcsIdMessage::getVcsId () const
{
    return (m_vcsId);
}

}
