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
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#include "VcsFabric/Global/VcsFabGetDomainListMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"


namespace DcmNs
{
VcsFabGetDomainListMessage::VcsFabGetDomainListMessage ()
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_GET_DOMAIN_LIST)
{
    m_domainList.clear ();
    m_ipAddresses.clear ();
    m_ports.clear ();
}

VcsFabGetDomainListMessage::VcsFabGetDomainListMessage (const vector<UI32> &domainList, const vector<string> &ipAddresses, const vector<UI32> &ports)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_GET_DOMAIN_LIST),
	m_domainList         (domainList),
    m_ipAddresses   (ipAddresses),
    m_ports         (ports)
{
}

VcsFabGetDomainListMessage::~VcsFabGetDomainListMessage ()
{
}

void VcsFabGetDomainListMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

 	addSerializableAttribute (new AttributeUI32Vector   (&m_domainList,     "domainList"));
 	addSerializableAttribute (new AttributeUI32Vector   (&m_ports,          "ports"));
 	addSerializableAttribute (new AttributeStringVector (&m_ipAddresses,    "ipAddresses"));
}

vector<UI32> VcsFabGetDomainListMessage::getDomainList ()
{
    return m_domainList;
}

vector<UI32> VcsFabGetDomainListMessage::getPorts ()
{
    return m_ports;
}
vector<string> VcsFabGetDomainListMessage::getIpAddresses ()
{
    return m_ipAddresses;
}

}

