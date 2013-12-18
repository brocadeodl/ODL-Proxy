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

#include "Interface/Association/ValInterfaceConfigureVlanMessage.h"
#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/ValInterfaceTypes.h"

namespace DcmNs
{

ValInterfaceConfigureVlanMessage:: ValInterfaceConfigureVlanMessage ()
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_ASSOC_VLAN),
      m_id               (0)
{
}

ValInterfaceConfigureVlanMessage:: ValInterfaceConfigureVlanMessage (const ObjectId &interfaceChildObjectId, const UI32 &id)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_ASSOC_VLAN),
      m_interfaceChildObjectId   (interfaceChildObjectId),
      m_id                       (id)
{
}

ValInterfaceConfigureVlanMessage:: ValInterfaceConfigureVlanMessage (const ObjectId &interfaceChildObjectId, vector<UI32> &list)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_ASSOC_VLAN),
      m_interfaceChildObjectId   (interfaceChildObjectId),
      m_vlanIdList               (list)
{
}

ValInterfaceConfigureVlanMessage:: ValInterfaceConfigureVlanMessage (const ObjectId &interfaceChildObjectId, vector<UI32> &list, string fromString)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_ASSOC_VLAN),
      m_interfaceChildObjectId   (interfaceChildObjectId),
      m_vlanIdList2              (list)
{
}

ValInterfaceConfigureVlanMessage::~ValInterfaceConfigureVlanMessage ()
{
}

void ValInterfaceConfigureVlanMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeObjectId (&m_interfaceChildObjectId,    "interfaceChildObjectId"));
     addSerializableAttribute (new AttributeUI32     (&m_id,                        "id"));
     addSerializableAttribute (new AttributeUI32Vector (&m_vlanIdList,              "vlanIdList"));
     addSerializableAttribute (new AttributeUI32Vector (&m_vlanIdList2,             "vlanIdList2"));
}

ObjectId ValInterfaceConfigureVlanMessage::getInterfaceChildObjectId () const
{
    return (m_interfaceChildObjectId);
}

UI32 ValInterfaceConfigureVlanMessage::getId () const
{
    return (m_id);
}

void ValInterfaceConfigureVlanMessage::setInterfaceChildObjectId (const ObjectId &interfaceObjectId)
{
    m_interfaceChildObjectId = interfaceObjectId;
}

void ValInterfaceConfigureVlanMessage::setId (const UI32 &vlanId)
{
    m_id = vlanId;
}

vector<UI32> ValInterfaceConfigureVlanMessage::getVlanIdVector () const
{
    return (m_vlanIdList);
}

void ValInterfaceConfigureVlanMessage::setVlanIdVector (vector<UI32> &list)
{
    m_vlanIdList = list;
}

vector<UI32> ValInterfaceConfigureVlanMessage::getVlanIdVector2 () const
{
    return (m_vlanIdList2);
}

void ValInterfaceConfigureVlanMessage::setVlanIdVector2 (vector<UI32> &list)
{
    m_vlanIdList2 = list;
}

}
