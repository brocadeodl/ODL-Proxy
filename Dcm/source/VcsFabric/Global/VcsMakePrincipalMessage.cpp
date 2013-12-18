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

#include "VcsFabric/Global/VcsMakePrincipalMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"


namespace DcmNs
{
VcsMakePrincipalMessage::VcsMakePrincipalMessage ()
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_MAKE_PRINCIPAL),
	m_domainId                 (0)
{

}

VcsMakePrincipalMessage::VcsMakePrincipalMessage (const UI32 domainId)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_MAKE_PRINCIPAL),
	m_domainId            (domainId)
{
}

void VcsMakePrincipalMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_domainId, "domainId"));
}

VcsMakePrincipalMessage::~VcsMakePrincipalMessage ()
{
}

UI32 VcsMakePrincipalMessage::GetSwitchDomainId              ()
{
	return m_domainId;
}

void VcsMakePrincipalMessage::SetSwitchDomainId              (const UI32 domainId)
{
    m_domainId = domainId;
}

}

