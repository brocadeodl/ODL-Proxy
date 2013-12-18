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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha													   *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Ssm/QosClientClearMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"


namespace DcmNs
{

    QosClientClearMessage::QosClientClearMessage ()
        : DcmManagementInterfaceMessage (QOSCLIENTCLEAR)
    {
    }

    QosClientClearMessage::QosClientClearMessage (const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (QOSCLIENTCLEAR),
        m_cmdCode (cmdCode)
    {
    }

    QosClientClearMessage::~QosClientClearMessage ()
    {
    }

    void  QosClientClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_ifTypeStr,"ifTypeStr"));
        addSerializableAttribute (new AttributeSI32(&m_ifType,"ifType"));
    }

    void  QosClientClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  QosClientClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void QosClientClearMessage::setIfName(const string &ifName)
	{
		m_ifName = ifName;
	}

	string QosClientClearMessage::getIfName() const 
	{
		return m_ifName;
	}

    void QosClientClearMessage::setIfType(const SI32 &IfType)
	{
		m_ifType = IfType;
    }

	SI32 QosClientClearMessage::getIfType() const
	{
		return m_ifType;
	}	
	
	void QosClientClearMessage::setIfTypeStr(const string &ifTypeStr)
	{
		m_ifTypeStr = ifTypeStr;
	}

	string QosClientClearMessage::getIfTypeStr() const
	{
		return m_ifTypeStr;
	}	

    const void *QosClientClearMessage::getCStructureForInputs ()
    {
        qos_clear_counters_msg_t *msg = (qos_clear_counters_msg_ *)malloc (sizeof (struct qos_clear_counters_msg_));
        msg->cmd_code = m_cmdCode;
		msg->if_type = m_ifType;
		strncpy(msg->if_name, m_ifName.c_str(), MAX_STR);
		msg->if_name[MAX_STR] = '\0';
		strncpy(msg->if_type_str, m_ifTypeStr.c_str(), MAX_STR);
		msg->if_type_str[MAX_STR] = '\0';
        return msg;
    }

	void 
    QosClientClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
