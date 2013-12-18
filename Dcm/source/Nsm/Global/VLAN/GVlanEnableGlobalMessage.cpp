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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : vsatyana														*
 ***************************************************************************/

#include "Nsm/Global/VLAN/GVlanEnableGlobalMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

GVlanEnableGlobalMessage::GVlanEnableGlobalMessage ()
	: ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), GVLAN_ENABLE_GLOBAL_MESSAGE)
{
}

GVlanEnableGlobalMessage::~GVlanEnableGlobalMessage ()
{
}

void GVlanEnableGlobalMessage::setupAttributesForSerialization ()
{
	ManagementInterfaceMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeBool(&m_enable, "enable"));
	addSerializableAttribute (new AttributeBool(&m_undo, "undo"));
	addSerializableAttribute (new AttributeEnum((UI32 *)&m_failedStage, "failedStage"));
}

void GVlanEnableGlobalMessage::setEnable(const bool &enable)
{
	m_enable = enable;
}

bool GVlanEnableGlobalMessage::getEnable() const
{
	return m_enable;
}

void GVlanEnableGlobalMessage::setUndo(const bool &undo)
{
	m_undo = undo;
}

bool GVlanEnableGlobalMessage::getUndo() const
{
	return m_undo;
}

void GVlanEnableGlobalMessage::setFailedStage(const gvlan_enable_stage_t &failedStage)
{
	m_failedStage = failedStage;
}

gvlan_enable_stage_t GVlanEnableGlobalMessage::getFailedStage() const
{
	return m_failedStage;
}

}

