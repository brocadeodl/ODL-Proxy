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

#include "VcsFabric/Global/VcsRejoinSwitchMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"


namespace DcmNs
{
VcsRejoinSwitchMessage::VcsRejoinSwitchMessage ()
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_REJOIN_SWITCH),
    m_portId                 (0)
{
}
VcsRejoinSwitchMessage::VcsRejoinSwitchMessage (const vector<UI32> &domainIds)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_REJOIN_SWITCH),
	m_domainIds               (domainIds),
    m_portId                  (0)
{
}

VcsRejoinSwitchMessage::VcsRejoinSwitchMessage (const string &ipAddress, const UI32 portId)
	: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_REJOIN_SWITCH),
	m_ipAddress                (ipAddress),
    m_portId                   (portId)
{
}

VcsRejoinSwitchMessage::~VcsRejoinSwitchMessage ()
{
}

void VcsRejoinSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32Vector (&m_domainIds,    "domainIds"));
 	addSerializableAttribute (new AttributeString (&m_ipAddress,        "ipAddress"));
    addSerializableAttribute (new AttributeUI32 (&m_portId,             "portId"));
}

vector<UI32> VcsRejoinSwitchMessage::getDomainIds ()
{
	return m_domainIds;
}

void VcsRejoinSwitchMessage::setDomainIds (const vector<UI32> &domainIds)
{
	m_domainIds = domainIds;
}

UI32 VcsRejoinSwitchMessage::getPortId ()
{
	return m_portId;
}
void VcsRejoinSwitchMessage::setPortId (const UI32 &portId)
{
	m_portId = portId;
}
string VcsRejoinSwitchMessage::getIpAddress ()
{
	return m_ipAddress;
}

void VcsRejoinSwitchMessage::setIpAddress (const string &ipAddress)
{
	m_ipAddress = ipAddress;
}

}

