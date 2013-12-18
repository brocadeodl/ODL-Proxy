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
 *   Author : Venkat														*
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/VLAN/GVlanEnableLocalMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

GVlanEnableLocalMessage::GVlanEnableLocalMessage ()
  : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), GVLAN_ENABLE_LOCAL_MESSAGE)
{
}
   
void GVlanEnableLocalMessage::setupAttributesForSerialization ()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeBool(&m_enable, "enable"));
	addSerializableAttribute (new AttributeBool(&m_undo, "undo"));
	addSerializableAttribute (new AttributeEnum((UI32 *)&m_cmdCode, "cmdCode"));
}

GVlanEnableLocalMessage::~GVlanEnableLocalMessage ()
{
}

bool GVlanEnableLocalMessage::getEnable () const
{
	return m_enable;
}

void GVlanEnableLocalMessage::setEnable (const bool &enable)
{
	m_enable = enable;
}

bool GVlanEnableLocalMessage::getUndo () const
{
	return m_undo;
}

void GVlanEnableLocalMessage::setUndo (const bool &undo)
{
	m_undo = undo;
}

gvlan_enable_stage_t GVlanEnableLocalMessage::getCmdCode () const
{
	return m_cmdCode;
}

void GVlanEnableLocalMessage::setCmdCode (const gvlan_enable_stage_t &cmdCode)
{
	m_cmdCode = cmdCode;
}

}


