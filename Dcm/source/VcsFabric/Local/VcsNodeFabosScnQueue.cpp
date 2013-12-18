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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                            *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeFabosScnQueue.h"
namespace DcmNs
{

VcsNodeFabosScnQueueObject::VcsNodeFabosScnQueueObject ()
{
    m_domainId = 0;
    m_principalDomainId = 0;
    m_scnObjectId = 0;
    m_scnType = 0;
}

VcsNodeFabosScnQueueObject::~VcsNodeFabosScnQueueObject ()
{
}


UI32 VcsNodeFabosScnQueueObject::getDomainId             () const
{
	return m_domainId;
}

void VcsNodeFabosScnQueueObject::setDomainId     (const UI32 &domainId)
{
	m_domainId = domainId;
}

UI32 VcsNodeFabosScnQueueObject::getPrincipalDomainId	() const
{
	return m_principalDomainId;
}

void VcsNodeFabosScnQueueObject::setPrincipalDomainId (const UI32 &principalDomainId)
{
	m_principalDomainId = principalDomainId;
}

UI32 VcsNodeFabosScnQueueObject::getScnType   () const
{
	return m_scnType;
}

void VcsNodeFabosScnQueueObject::setScnType (const UI32 &scnType)
{
	m_scnType = scnType;
}

UI32 VcsNodeFabosScnQueueObject::getScnObjectId   () const
{
	return m_scnObjectId;
}

void VcsNodeFabosScnQueueObject::setScnObjectId (const UI32 &scnObjectId)
{
	m_scnObjectId = scnObjectId;
}


}


