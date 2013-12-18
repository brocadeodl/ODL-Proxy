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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/Composition/ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage.h"
#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/ValInterfaceTypes.h"

namespace DcmNs
{

ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage ()
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE_CHILDREN),
      m_isAllowed               (true)
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage (const ObjectId &interfaceChildObjectId, const bool &isAllowed)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE_CHILDREN),
      m_interfaceChildObjectId        (interfaceChildObjectId),
      m_isAllowed               (isAllowed)
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage::~ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage ()
{
}

void ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeObjectId (&m_interfaceChildObjectId, "interfaceChildObjectId"));
     addSerializableAttribute (new AttributeBool     (&m_isAllowed,              "isAllowed"));
}

ObjectId ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage::getInterfaceChildObjectId () const
{
    return (m_interfaceChildObjectId);
}

bool ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage::getIsAllowed () const
{
    return (m_isAllowed);
}

}
