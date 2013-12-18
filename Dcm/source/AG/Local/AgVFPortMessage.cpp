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
 *   Author : D. Chung
 **************************************************************************/

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgVFPortMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgVFPortMessage::AgVFPortMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_VFPORT_MESSAGE)
    {
    }

    AgVFPortMessage::AgVFPortMessage (const UI32 messageType)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_VFPORT_MESSAGE),
        m_messageType(messageType)
    {
    }

    AgVFPortMessage::~AgVFPortMessage ()
    {
    }

    void  AgVFPortMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_messageType, "messageType"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vfportsByPort, "vfportsByPort"));
        addSerializableAttribute (new AttributeStringVector(&m_vfportsByIfName, "vfportsByIfName"));
    }
	
    void  AgVFPortMessage::setMessageType(const UI32 messageType)
    {
        m_messageType =  messageType;
    }

    UI32  AgVFPortMessage::getMessageType() const
    {
        return (m_messageType);
	}

	void  AgVFPortMessage::setVFPortsByPort(const vector<UI32> &portList)
    {
        m_vfportsByPort = portList;
    }

	vector<UI32>  AgVFPortMessage::getVFPortsByPort() const
    {
        return (m_vfportsByPort);
	}

	void AgVFPortMessage::setVFPortsByIfName(const vector<string> &ifList)
    {
        m_vfportsByIfName = ifList;
    }

    vector<string> AgVFPortMessage::getVFPortsByIfName() const
    {
        return (m_vfportsByIfName);
    }
}
