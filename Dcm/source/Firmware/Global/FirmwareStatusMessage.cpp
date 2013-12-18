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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#include "Firmware/Global/FirmwareStatusObjectManager.h"
#include "Firmware/Global/FirmwareStatusMessage.h"
#include "Firmware/Global/FirmwareTypes.h"
#include "Framework/Attributes/AttributeStringVectorVector.h"
#include "Framework/Attributes/AttributeUI32VectorVector.h"

namespace DcmNs
{

    FirmwareStatusMessage::FirmwareStatusMessage()
        : PrismMessage (FirmwareStatusObjectManager::getPrismServiceId (), FIRMWARENETCONFSTATUS)
    {
    }

    FirmwareStatusMessage::~FirmwareStatusMessage()
    {
    }

	void  FirmwareStatusMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization();
 		addSerializableAttribute (new AttributeUI32Vector           (&m_rbridge_id,     "rbridge_id"));
 		addSerializableAttribute (new AttributeSI32Vector           (&m_fwdlState,      "fwdlState"));
 		addSerializableAttribute (new AttributeUI32Vector           (&m_numOfEntries,   "numOfEntries"));
 		addSerializableAttribute (new AttributeUI32VectorVector     (&m_index,          "index"));
 		addSerializableAttribute (new AttributeUI32VectorVector     (&m_messageId,      "messageId"));
 		addSerializableAttribute (new AttributeStringVectorVector   (&m_dateAndTime,    "dateAndTime"));
 		addSerializableAttribute (new AttributeStringVectorVector   (&m_message,        "message"));
 		addSerializableAttribute (new AttributeStringVectorVector   (&m_bladeName,      "bladeName"));
	}

	void FirmwareStatusMessage::setRbridgeIdVector(const vector<UI32> &rbridge_id)
	{
		m_rbridge_id = rbridge_id;
	}

	void FirmwareStatusMessage::setRbridgeId(const UI32 &rbridge_id)
	{
		m_rbridge_id.push_back(rbridge_id);
	}

	void FirmwareStatusMessage::setFwdlState(const int &fwdlState)
	{
		m_fwdlState.push_back(fwdlState);
	}

	void FirmwareStatusMessage::setNumOfEntries(const UI32 &numOfEntries)
	{
		m_numOfEntries.push_back(numOfEntries);
    }

    void FirmwareStatusMessage::setIndex(const vector<UI32> &index)
    {
        m_index.push_back(index);
    }

    void FirmwareStatusMessage::setMessageId(const vector<UI32> &messageId)
    {
        m_messageId.push_back(messageId);
    }

    void FirmwareStatusMessage::setDateAndTime(const vector<string> &dateAndTime)
    {
        m_dateAndTime.push_back(dateAndTime);
    }

    void FirmwareStatusMessage::setMessage(const vector<string> &message)
    {
        m_message.push_back(message);
    }

    void FirmwareStatusMessage::setBladeName(const vector<string> &bladeName)
    {
        m_bladeName.push_back(bladeName);
    }

    vector<UI32> FirmwareStatusMessage::getRbridgeId() const
    {
        return (m_rbridge_id);
    }

    vector<int> FirmwareStatusMessage::getFwdlState() const
    {
        return (m_fwdlState);
    }

    vector<UI32> FirmwareStatusMessage::getNumOfEntries() const
    {
        return (m_numOfEntries);
    }

    vector<vector<UI32> > FirmwareStatusMessage::getIndex() const
    {
        return (m_index);
    }

    vector<vector<UI32> > FirmwareStatusMessage::getMessageId() const
    {
        return (m_messageId);
    }

    vector<vector<string> > FirmwareStatusMessage::getDateAndTime() const
    {
        return (m_dateAndTime);
    }

    vector<vector<string> > FirmwareStatusMessage::getMessage() const
    {
        return (m_message);
    }

    vector<vector<string> > FirmwareStatusMessage::getBladeName() const
    {
        return (m_bladeName);
    }
}
