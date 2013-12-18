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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#include "VcsFabric/Global/VcsAddSwitchMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"


namespace DcmNs
{
VcsAddSwitchMessage::VcsAddSwitchMessage ()
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_ADD_SWITCH)
{
    m_senderId = 0;
}

VcsAddSwitchMessage::VcsAddSwitchMessage (const UI32 senderId)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_ADD_SWITCH),
    m_senderId          (senderId)
{
}

VcsAddSwitchMessage::~VcsAddSwitchMessage ()
{
}

void VcsAddSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_senderId,              "senderId"));
     addSerializableAttribute (new AttributeUI32Vector (&m_failedDomainIds, "failedDomainIds"));
}

void VcsAddSwitchMessage::setSenderId (const UI32 &senderId)
{
    m_senderId = senderId;
}

UI32 VcsAddSwitchMessage::getSenderId ()
{
    return m_senderId;
}

void VcsAddSwitchMessage::addToFailedDomainIdVector (const UI32 &domainId)
{
    m_failedDomainIds.push_back (domainId);
}

vector<UI32> VcsAddSwitchMessage::getFailedDomainIdVector () const
{
    return (m_failedDomainIds);
}

vector<VcsFabricLocalSwitchInfoObject> VcsAddSwitchMessage::getRemoteSwitchInfoObjects () const
{
    return (m_vcsFabricLocalSwitchInfoObjects);
}

void VcsAddSwitchMessage::setRemoteSwitchInfoObjects (const vector<VcsFabricLocalSwitchInfoObject> &infoObjects)
{
    UI32 size = infoObjects.size ();

    for (UI32 i = 0; i < size; i++)
    {
        m_vcsFabricLocalSwitchInfoObjects.push_back (infoObjects[i]);
    }
}

}

