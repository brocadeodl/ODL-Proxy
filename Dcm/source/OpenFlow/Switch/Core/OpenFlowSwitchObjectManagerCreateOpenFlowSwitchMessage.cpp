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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"

namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage::OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage ()
    : ManagementInterfaceMessage (OpenFlowSwitchObjectManager::getServiceName (), OPENFLOW_OBJECT_MANAGER_CREATE_OPENFLOW_SWITCH)
{
}

OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage::~OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage ()
{
}

void OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_openFlowSwitchName, "openFlowSwitchName"));
    addSerializableAttribute (new AttributeUri    (&m_openFlowSwitchUri,  "openFlowSwitchUri" ));
}

string OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage::getOpenFlowSwitchName () const
{
    return (m_openFlowSwitchName);
}

void OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage::setOpenFlowSwitchName (const string &openFlowSwitchName)
{
    m_openFlowSwitchName = openFlowSwitchName;
}

Uri OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage::getOpenFlowSwitchUri () const
{
    return (m_openFlowSwitchUri);
}

void OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage::setOpenFlowSwitchUri (const Uri &openFlowSwitchUri)
{
    m_openFlowSwitchUri = openFlowSwitchUri;
}

}
