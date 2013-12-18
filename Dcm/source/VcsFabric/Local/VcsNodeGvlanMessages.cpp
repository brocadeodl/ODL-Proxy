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
 *   Author : Mithun BS                                                    *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeGvlanMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeIsGvlanCapableMessage::VcsNodeIsGvlanCapableMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_GVLAN_CAPABLE)
{
    m_isGvlanCapable = false;
}

VcsNodeIsGvlanCapableMessage::~VcsNodeIsGvlanCapableMessage ()
{
}

void VcsNodeIsGvlanCapableMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isGvlanCapable, "isGvlanCapable"));
}

bool VcsNodeIsGvlanCapableMessage::isGvlanCapable () const
{
    return (m_isGvlanCapable);
}

void VcsNodeIsGvlanCapableMessage::setGvlanCapable (bool gvlanCapable)
{
    m_isGvlanCapable = gvlanCapable;
}

VcsNodeIsGvlanEnabledMessage::VcsNodeIsGvlanEnabledMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_GVLAN_ENABLED)
{
    m_isGvlanEnabled = false;
}

VcsNodeIsGvlanEnabledMessage::~VcsNodeIsGvlanEnabledMessage ()
{
}

void VcsNodeIsGvlanEnabledMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isGvlanEnabled, "isGvlanEnabled"));
}

bool VcsNodeIsGvlanEnabledMessage::isGvlanEnabled () const
{
    return (m_isGvlanEnabled);
}

void VcsNodeIsGvlanEnabledMessage::setGvlanEnabled (bool gvlanEnabled)
{
    m_isGvlanEnabled = gvlanEnabled;
}

}
