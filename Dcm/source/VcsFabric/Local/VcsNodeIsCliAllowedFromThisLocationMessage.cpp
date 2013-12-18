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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Apurva Singh                                               *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeIsCliAllowedFromThisLocationMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"


namespace DcmNs
{

VcsNodeIsCliAllowedFromThisLocationMessage::VcsNodeIsCliAllowedFromThisLocationMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_CLI_ALLOWED_FROM_THIS_LOCATION),
    m_isCliAllowedFromThisLocation (false)
{
}

VcsNodeIsCliAllowedFromThisLocationMessage::VcsNodeIsCliAllowedFromThisLocationMessage (bool isCliAllowedFromThisLocation)
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_CLI_ALLOWED_FROM_THIS_LOCATION),
    m_isCliAllowedFromThisLocation (isCliAllowedFromThisLocation)
{
}

VcsNodeIsCliAllowedFromThisLocationMessage::~VcsNodeIsCliAllowedFromThisLocationMessage ()
{
}

void VcsNodeIsCliAllowedFromThisLocationMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isCliAllowedFromThisLocation, "isCliAllowedFromThisLocation"));
}

bool VcsNodeIsCliAllowedFromThisLocationMessage::getIsCliAllowedFromThisLocation () const
{
    return (m_isCliAllowedFromThisLocation);
}

void VcsNodeIsCliAllowedFromThisLocationMessage::setIsCliAllowedFromThisLocation (bool isCliAllowedFromThisLocation)
{
    m_isCliAllowedFromThisLocation = isCliAllowedFromThisLocation;
}

}

                    
