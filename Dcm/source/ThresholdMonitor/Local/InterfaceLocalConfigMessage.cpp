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
  *    Copyright (C) 2005-2011 Brocade Communications Systems, Inc.        *
  *    All rights reserved.                                                *
  *    Author : Priya Ahuja												   *
   **************************************************************************/

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/InterfaceLocalConfigMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"

namespace DcmNs
{
	InterfaceLocalConfigMessage::InterfaceLocalConfigMessage() 
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), INTERFACELOCALCONFIG)
	{
	}
	InterfaceLocalConfigMessage::InterfaceLocalConfigMessage(const string &applyvalue, const bool &pause)
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), INTERFACELOCALCONFIG),
		m_applyvalue  (applyvalue),
		m_pause    (pause)
		{
		}
	InterfaceLocalConfigMessage::~InterfaceLocalConfigMessage ()
	{
	}

	void InterfaceLocalConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_applyvalue, "applyvalue"));
		addSerializableAttribute (new AttributeBool(&m_pause,"pause"));
	}
	void InterfaceLocalConfigMessage::setApplyValue(const string &applyvalue)
	{
		m_applyvalue = applyvalue;
	}
	string InterfaceLocalConfigMessage::getApplyValue() const
	{
		return (m_applyvalue);
	}
	void InterfaceLocalConfigMessage::setPause(const bool &pause)
	{
		m_pause = pause;
	}
	bool InterfaceLocalConfigMessage::getPause() const
	{
	        return (m_pause);
	}
}

