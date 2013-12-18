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
 *   Author : skrastog
 **************************************************************************/

#include "ClientInterface/L2sys/L2sysPortSecShowMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "L2sys/Local/L2sysTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    L2sysPortSecShowMessage::L2sysPortSecShowMessage ()
        : DcmManagementInterfaceMessage (L2SYS_PORT_SECURITY_SHOW_MSG),
		m_cmdCode(0),
		m_ifName(""),
		m_ifType(IF_TYPE_INVALID),
		m_lastIfName(""),
		m_lastType(""),
		m_lastMac(""),
		m_lastVlanId(0),
		m_isLast(0),
		m_isFirstRequest(0)
    {
    }

    L2sysPortSecShowMessage::L2sysPortSecShowMessage (const UI32 &cmdCode, const string &ifName, const UI32 &ifType)
        : DcmManagementInterfaceMessage (L2SYS_PORT_SECURITY_SHOW_MSG),
		m_cmdCode(cmdCode),
		m_ifName(ifName),
		m_ifType(ifType)
	{
		m_lastIfName = "";
		m_lastType = "";
		m_lastMac = "";
		m_lastVlanId = 0;
		m_isLast = 0;
		m_isFirstRequest = 0;
	}

    L2sysPortSecShowMessage::~L2sysPortSecShowMessage ()
    {
    }

    void  L2sysPortSecShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeString(&m_lastIfName,"lastIfName"));
        addSerializableAttribute (new AttributeString(&m_lastType,"lastType"));
		addSerializableAttribute (new AttributeString(&m_lastMac,"lastMac"));
        addSerializableAttribute (new AttributeUI32(&m_lastVlanId,"lastVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_isLast,"isLast"));
        addSerializableAttribute (new AttributeUI32(&m_isFirstRequest,"isFirstRequest"));
    }


    void  L2sysPortSecShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L2sysPortSecShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  L2sysPortSecShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  L2sysPortSecShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  L2sysPortSecShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  L2sysPortSecShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  L2sysPortSecShowMessage::setIsFirstRequest(const UI32 &isFirstRequest)
    {
        m_isFirstRequest = isFirstRequest;
    }

    UI32  L2sysPortSecShowMessage::getIsFirstRequest() const
    {
        return (m_isFirstRequest);
    }

    void L2sysPortSecShowMessage::setLastPortSecurityAddressRecord(const l2sys_show_mac_output_msg_t *lastRecord)
    {
        m_lastIfName = lastRecord->if_name;
        m_lastMac = lastRecord->Mac;
        m_lastType = lastRecord->type;
        m_lastVlanId = lastRecord->vlan_id;
        m_isLast = lastRecord->is_last;
    }

    void L2sysPortSecShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *L2sysPortSecShowMessage::getCStructureForInputs ()
    {
        L2sysShow_PortSecurity_InputMsg_t *pInput = new L2sysShow_PortSecurity_InputMsg_t;


		memset(pInput, 0, sizeof(L2sysShow_PortSecurity_InputMsg_t));

		if (m_ifType == IF_TYPE_PO)
            snprintf(pInput->if_name, 64, "po%s", m_ifName.c_str());
        else if (m_ifType != IF_TYPE_INVALID)
            GENERATE_SHORT_IF_NAME_FOR_BACK_END(m_ifType, m_ifName.c_str(), pInput->if_name, 64);

        pInput->interface_type = m_ifType;
        pInput->cmdCode     = m_cmdCode;
        //copy the information of the last Mac Record
        pInput->is_first_request = m_isFirstRequest;
        strncpy(pInput->last_record.if_name, m_lastIfName.c_str(), 256);
        strncpy(pInput->last_record.Mac, m_lastMac.c_str(), 16);
        strncpy(pInput->last_record.type, m_lastType.c_str(), 64);
        pInput->last_record.vlan_id = m_lastVlanId;
        pInput->last_record.is_last = m_isLast;
        return (pInput);
    }

	void L2sysPortSecShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
    	l2sys_show_msg_t *msg = ((l2sys_show_msg_t *)pOutputCStructure);
        if(msg == NULL)
			return;

		int num_records = msg->num_records;

		switch (msg->record_type) {
		  case L2SYSSHOW_PORT_SECURITY:
        		addBuffer (L2SYSSHOW_PORT_SECURITY, (sizeof(l2sys_show_msg_t)+ 
                            ((num_records - 1) * sizeof (l2sys_port_security_output_msg_t))), 
                           pOutputCStructure, false);
				break;
		  case L2SYSSHOW_PORT_SECURITY_ADDRESSES:
				addBuffer (L2SYSSHOW_PORT_SECURITY_ADDRESSES,
					(sizeof(l2sys_show_msg_t)+ ((num_records - 1) * sizeof (l2sys_show_mac_output_msg_t))),
					pOutputCStructure, false);
				break;
		  case L2SYSSHOW_PORT_SECURITY_INTERFACE:
        		addBuffer (L2SYSSHOW_PORT_SECURITY_INTERFACE, sizeof(l2sys_show_msg_t), pOutputCStructure, false);
				break;
		  case L2SYSSHOW_PORT_SECURITY_OUI:
        		addBuffer (L2SYSSHOW_PORT_SECURITY_OUI, sizeof(l2sys_show_msg_t), pOutputCStructure, false);
				break;
		  default :
			break;
		}
	}
}
