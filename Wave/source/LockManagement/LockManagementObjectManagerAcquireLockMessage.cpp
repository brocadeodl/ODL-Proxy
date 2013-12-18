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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "LockManagement/LockManagementObjectManagerTypes.h"
#include "LockManagement/LockManagementObjectManagerAcquireLockMessage.h"
#include "LockManagement/LockManagementObjectManager.h"

namespace WaveNs
{

LockManagementObjectManagerAcquireLockMessage::LockManagementObjectManagerAcquireLockMessage ()
    : ManagementInterfaceMessage (LockManagementObjectManager::getClassName (), LOCK_MANAGEMENT_OBJECT_MANAGER_ACQUIRE_LOCK)
{
}

LockManagementObjectManagerAcquireLockMessage::LockManagementObjectManagerAcquireLockMessage (const string &serviceString)
        : ManagementInterfaceMessage (LockManagementObjectManager::getClassName (), LOCK_MANAGEMENT_OBJECT_MANAGER_ACQUIRE_LOCK),
          m_serviceString (serviceString)
{
}

LockManagementObjectManagerAcquireLockMessage::~LockManagementObjectManagerAcquireLockMessage ()
{
}

void LockManagementObjectManagerAcquireLockMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_serviceString, "serviceString"));
}

const string& LockManagementObjectManagerAcquireLockMessage::getServiceString ()
{
    return m_serviceString;
}

}
