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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"

namespace DcmNs
{

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsCopyRunningToStartupMessage------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

VcsCopyRunningToStartupMessage::VcsCopyRunningToStartupMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_COPY_RUNNING_TO_STARTUP_MESSAGE)
{
}
    
VcsCopyRunningToStartupMessage::~VcsCopyRunningToStartupMessage ()
{
}

void VcsCopyRunningToStartupMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsCopyDefaultToStartupMessage------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

VcsCopyDefaultToStartupMessage::VcsCopyDefaultToStartupMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_COPY_DEFAULT_TO_STARTUP_MESSAGE)
{
}

VcsCopyDefaultToStartupMessage::~VcsCopyDefaultToStartupMessage ()
{
}

void VcsCopyDefaultToStartupMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsCopyToStartupMessage-------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

VcsCopyToStartupMessage::VcsCopyToStartupMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_COPY_TO_STARTUP_MESSAGE),
      m_path ("")
{
}
    
VcsCopyToStartupMessage::VcsCopyToStartupMessage (const string &path)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_COPY_TO_STARTUP_MESSAGE),
      m_path (path)
{
}
    
VcsCopyToStartupMessage::~VcsCopyToStartupMessage ()
{
}

void VcsCopyToStartupMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_path, "path"));
}

void VcsCopyToStartupMessage::setPath (const string &path)
{
    m_path = path;
}

string VcsCopyToStartupMessage::getPath () const
{
    return (m_path);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------VcsCopyToStartupAllowedMessage------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

VcsCopyToStartupAllowedMessage::VcsCopyToStartupAllowedMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_COPY_TO_STARTUP_ALLOWED_MESSAGE),
      m_isAllowed (false)
{
}
    
VcsCopyToStartupAllowedMessage::~VcsCopyToStartupAllowedMessage ()
{
}

void VcsCopyToStartupAllowedMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isAllowed, "isAllowed"));
}

void VcsCopyToStartupAllowedMessage::setIsAllowed (const bool isAllowed)
{
    m_isAllowed = isAllowed;
}

bool VcsCopyToStartupAllowedMessage::getIsAllowed () const
{
    return (m_isAllowed);
}

}

