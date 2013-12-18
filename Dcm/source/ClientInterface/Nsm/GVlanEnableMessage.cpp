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
 ****************************************************************************/

#include "ClientInterface/Nsm/GVlanEnableMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

	GVlanEnableMessage::GVlanEnableMessage ()
	: DcmManagementInterfaceMessage (GVLAN_ENABLE_MESSAGE)
	{
	}

	GVlanEnableMessage::~GVlanEnableMessage ()
	{
	}

	void GVlanEnableMessage::setupAttributesForSerialization()
	{
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeBool(&m_enable, "enable"));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_cmdCode, "cmdCode"));
		addSerializableAttribute (new AttributeBool(&m_undo, "undo"));
	}

	void GVlanEnableMessage::setEnable(const bool &enable)
	{
		m_enable = enable;
	}

	bool GVlanEnableMessage::getEnable() const
	{
		return m_enable;
	}

	void GVlanEnableMessage::setCmdCode(const gvlan_enable_stage_t &cmdCode)
	{
		m_cmdCode = cmdCode;
	}

	gvlan_enable_stage_t GVlanEnableMessage::getCmdCode() const
	{
		return m_cmdCode;
	}

	void GVlanEnableMessage::setUndo(const bool &undo)
	{
		m_undo = undo;
	}

	bool GVlanEnableMessage::getUndo() const
	{
		return m_undo;
	}


	const void *GVlanEnableMessage::getCStructureForInputs ()
	{
	  	gvlan_enable_input_msg_t *pInput = (gvlan_enable_input_msg_t *)calloc(1, sizeof(gvlan_enable_input_msg_t));
		pInput->enable = m_enable;
		pInput->stage = m_cmdCode;
		pInput->undo = m_undo;

		return pInput;
	}

	void GVlanEnableMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}

}


