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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Mstp/MstpClearMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    MstpClearMessage::MstpClearMessage ()
        : DcmManagementInterfaceMessage (MSTPCLEAR)
    {
        m_ifType    =   IF_TYPE_INVALID;
        m_cmdCode     =   0; 
    }

    MstpClearMessage::MstpClearMessage (const string &ifName,const UI32 &cmdCode,
        const UI32 &ifType) : DcmManagementInterfaceMessage (MSTPCLEAR),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode),m_ifType  (ifType)
    {
    }

    MstpClearMessage::~MstpClearMessage ()
    {
    }

    void  MstpClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    }

    void  MstpClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  MstpClearMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  MstpClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpClearMessage::setIfType(const UI32 &ifType)
    {
        m_ifType= ifType ;
    }

    UI32  MstpClearMessage::getIfType() const
    {
        return (m_ifType);
    }
    const void *MstpClearMessage::getCStructureForInputs ()
    {
        mstp_msg_clear_t  *msg = new mstp_msg_clear_t;
		msg->cmd = m_cmdCode;
		msg->ifType= m_ifType;
		strncpy((char *)(msg->name), (const char*)(m_ifName.data()), MSTP_DCM_IF_NAME_LEN);
        return msg;
    }

	void 
    MstpClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}
}
