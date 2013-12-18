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
  *    Author : Ramya Rangarajan										   *
   **************************************************************************/

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/SecLocalConfigMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"

namespace DcmNs
{
	SecLocalConfigMessage::SecLocalConfigMessage() 
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SECURITYLOCALCONFIG)
	{
	}
	SecLocalConfigMessage::SecLocalConfigMessage(const string &applyvalue, const bool &pause)
		: PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SECURITYLOCALCONFIG),
		m_applyvalue  (applyvalue),
		m_pause (pause)
		{
		}
	SecLocalConfigMessage::~SecLocalConfigMessage ()
	{
	}

	void SecLocalConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_applyvalue, "applyvalue"));
		addSerializableAttribute (new AttributeBool(&m_pause,"pause"));
	}
	void SecLocalConfigMessage::setApplyValue(const string &applyvalue)
	{
		m_applyvalue = applyvalue;
	}
	string SecLocalConfigMessage::getApplyValue() const
	{
		return (m_applyvalue);
	}
	void SecLocalConfigMessage::setPause(const bool &pause)
	{
		m_pause = pause;
	}
	bool SecLocalConfigMessage::getPause() const
	{
		return(m_pause);
	}
}

