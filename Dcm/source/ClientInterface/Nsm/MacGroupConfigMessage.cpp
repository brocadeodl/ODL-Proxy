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
 ****************************************************************************/

#include "ClientInterface/Nsm/MacGroupConfigMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

	MacGroupConfigMessage::MacGroupConfigMessage ()
		: DcmManagementInterfaceMessage (MAC_GROUP_CONFIG_MESSAGE)
	{
	}

	MacGroupConfigMessage::~MacGroupConfigMessage ()
	{
	}

	void MacGroupConfigMessage::setupAttributesForSerialization()
	{
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_macGroupId,"macGroupId"));
		addSerializableAttribute (new AttributeStringVector(&m_mac, "mac"));
		addSerializableAttribute (new AttributeStringVector(&m_mask, "mask"));
	}

    void MacGroupConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode = cmdCode;
    }

    UI32 MacGroupConfigMessage::getCmdCode() const
    {
        return m_cmdCode;
    }

    void MacGroupConfigMessage::setMacGroupId(const UI32 &macGroupId)
    {
        m_macGroupId = macGroupId;
    }

    UI32 MacGroupConfigMessage::getMacGroupId() const
    {
        return m_macGroupId;
    }

	void MacGroupConfigMessage::setMac(const vector<string> &mac)
	{
		m_mac = mac;
	}

	vector<string> MacGroupConfigMessage::getMac() const
	{
		return m_mac;
	}

	void MacGroupConfigMessage::setMask(const vector<string> &mask)
	{
		m_mask = mask;
	}

	vector<string> MacGroupConfigMessage::getMask() const
	{
		return m_mask;
	}

	void MacGroupConfigMessage::setMacEntry(const string &mac, const string &mask)
	{
		m_mac.push_back(mac);
		m_mask.push_back(mask);
	}

	const void *MacGroupConfigMessage::getCStructureForInputs ()
	{
		UI32 numMacRecords = m_mac.size();
		MacGroupInputMsg_t *pInput = NULL;

		if (numMacRecords < 1)
		{
			pInput = (MacGroupInputMsg_t *)calloc(1, sizeof(MacGroupInputMsg_t));
			pInput->cmdCode = m_cmdCode;
			pInput->macGroupId = m_macGroupId;
			pInput->numMacRecords = 0;
		}
		else
		{
			pInput = (MacGroupInputMsg_t *)calloc(1, (sizeof(MacGroupInputMsg_t) + (numMacRecords - 1) * sizeof(MacGroupRecord_t)));
			pInput->cmdCode = m_cmdCode;
			pInput->macGroupId = m_macGroupId;
			pInput->numMacRecords = numMacRecords;

			for(UI32 recIdx = 0; recIdx < numMacRecords; recIdx++)
			{
				MacGroupRecord_t *rec = &(pInput->macGroupRecord[recIdx]);
				sscanf(m_mac[recIdx].c_str(), "%4hx.%4hx.%4hx", (unsigned short *)&(rec->address[0]), (unsigned short *)&(rec->address[2]), (unsigned short *)&(rec->address[4]));
				sscanf(m_mask[recIdx].c_str(), "%4hx.%4hx.%4hx", (unsigned short *)&(rec->mask[0]), (unsigned short *)&(rec->mask[2]), (unsigned short *)&(rec->mask[4]));
			}
		}

		return (pInput);
	}

	void MacGroupConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}

}


