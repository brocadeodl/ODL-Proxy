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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "PhysicalInterface/Local/PhysicalInterfaceMessages.h"
#include "PhysicalInterface/Local/PhysicalInterfaceLocalObjectManager.h"
#include "PhysicalInterface/Local/PhysicalInterfaceLocalTypes.h"

namespace DcmNs
{

PhysicalInterfaceCreateMessage::PhysicalInterfaceCreateMessage ()
    : ManagementInterfaceMessage (PhysicalInterfaceLocalObjectManager::getClassName (), PHYSICAL_INTERFACE_LOCAL_CREATE)
{
}

PhysicalInterfaceCreateMessage::PhysicalInterfaceCreateMessage (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description)
    : ManagementInterfaceMessage (PhysicalInterfaceLocalObjectManager::getClassName (), PHYSICAL_INTERFACE_LOCAL_CREATE),
      m_name                     (name),
      m_ifIndex                  (ifIndex),
      m_mtu                      (mtu),
      m_description              (description)
{
}

PhysicalInterfaceCreateMessage::~PhysicalInterfaceCreateMessage ()
{
}

void PhysicalInterfaceCreateMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_name,            "name"));
     addSerializableAttribute (new AttributeUI32   (&m_ifIndex,         "ifIndex"));
     addSerializableAttribute (new AttributeUI32   (&m_mtu,             "mtu"));
     addSerializableAttribute (new AttributeString (&m_description,     "description"));
}

string PhysicalInterfaceCreateMessage::getName () const
{
    return (m_name);
}

UI32 PhysicalInterfaceCreateMessage::getIfIndex () const
{
    return (m_ifIndex);
}

UI32 PhysicalInterfaceCreateMessage::getMtu () const
{
    return (m_mtu);
}

string PhysicalInterfaceCreateMessage::getDescription () const
{
    return (m_description);
}



PhysicalInterfaceUpdateMessage::PhysicalInterfaceUpdateMessage ()
    : ManagementInterfaceMessage (PhysicalInterfaceLocalObjectManager::getClassName (), PHYSICAL_INTERFACE_LOCAL_UPDATE)
{
}

PhysicalInterfaceUpdateMessage::PhysicalInterfaceUpdateMessage (const ObjectId &interfaceObjectId,const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description)
    : ManagementInterfaceMessage (PhysicalInterfaceLocalObjectManager::getClassName (), PHYSICAL_INTERFACE_LOCAL_UPDATE),
      m_interfaceObjectId        (interfaceObjectId),
      m_name                     (name),
      m_ifIndex                  (ifIndex),
      m_mtu                      (mtu),
      m_description              (description)
{
}

PhysicalInterfaceUpdateMessage::~PhysicalInterfaceUpdateMessage ()
{
}

void PhysicalInterfaceUpdateMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_name,        "name"));
     addSerializableAttribute (new AttributeUI32   (&m_ifIndex,     "ifIndex"));
     addSerializableAttribute (new AttributeUI32   (&m_mtu,         "mtu"));
     addSerializableAttribute (new AttributeString (&m_description, "description"));
}

string PhysicalInterfaceUpdateMessage::getName () const
{
    return (m_name);
}

UI32 PhysicalInterfaceUpdateMessage::getIfIndex () const
{
    return (m_ifIndex);
}

UI32 PhysicalInterfaceUpdateMessage::getMtu () const
{
    return (m_mtu);
}

UI32 PhysicalInterfaceUpdateMessage::getCmdCode () const
{
    return (m_cmdCode);
}

void PhysicalInterfaceUpdateMessage::setCmdCode (const UI32 &cmd_code)
{
        m_cmdCode = cmd_code;
}

string PhysicalInterfaceUpdateMessage::getDescription () const
{
    return (m_description);
}

ObjectId PhysicalInterfaceUpdateMessage::getInterfaceObjectId () const
{
        return (m_interfaceObjectId);
}

}
