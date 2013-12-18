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

VcsNodeSetIsReadyMessage::VcsNodeSetIsReadyMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_ISREADY)
{
    m_isReady = false;
}

VcsNodeSetIsReadyMessage::VcsNodeSetIsReadyMessage (const bool &isReady)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_ISREADY),
      m_isReady              (isReady)
{
}

VcsNodeSetIsReadyMessage::~VcsNodeSetIsReadyMessage ()
{
}

void VcsNodeSetIsReadyMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isReady, "isReady"));
}

void VcsNodeSetIsReadyMessage::setIsReady (const bool &isReady)
{
    m_isReady = isReady;
}

bool VcsNodeSetIsReadyMessage::getIsReady () const
{
    return (m_isReady);
}

/************************************ 
* GET ISPRINCIPAL 
*************************************/

VcsNodeGetIsReadyMessage::VcsNodeGetIsReadyMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_ISREADY)
{
    m_isReady = false;
}

VcsNodeGetIsReadyMessage::~VcsNodeGetIsReadyMessage ()
{
}

void VcsNodeGetIsReadyMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isReady, "isReady"));
}

void VcsNodeGetIsReadyMessage::setIsReady (const bool &isReady)
{
    m_isReady = isReady;
}

bool VcsNodeGetIsReadyMessage::getIsReady () const
{
    return (m_isReady);
}

}
