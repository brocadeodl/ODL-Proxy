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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "ClientInterface/Nsm/PortSecurityClientMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "L2sys/Global/L2sysGlobalTypes.h"

namespace DcmNs
{

    PortSecurityClientMessage::PortSecurityClientMessage ()
        : DcmManagementInterfaceMessage (L2SYS_PORT_SECURITY_MSG)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    PortSecurityClientMessage::PortSecurityClientMessage (const string &ifName, const UI32 &numAllowedMacs, const UI32 &action, const string &mac, const UI32 &opCode)
        : DcmManagementInterfaceMessage (L2SYS_PORT_SECURITY_MSG),
        m_ifName    (ifName),
        m_numAllowedMacs    (numAllowedMacs),
        m_action    (action),
        m_allowedMac    (mac),
		m_opCode		(opCode)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    PortSecurityClientMessage::~PortSecurityClientMessage ()
    {
    }

    void PortSecurityClientMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
		addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeUI32(&m_numAllowedMacs, "numAllowedMacs"));
        addSerializableAttribute (new AttributeUI32(&m_action, "action"));
        addSerializableAttribute (new AttributeString(&m_allowedMac, "allowedMac"));
        addSerializableAttribute (new AttributeString(&m_oui, "oui"));
        addSerializableAttribute (new AttributeBool(&m_sticky, "sticky"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId, "vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_shutdownTime, "shutdownTime"));
    }

    void PortSecurityClientMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string PortSecurityClientMessage::getIfName() const
    {
        return m_ifName;
    }

    void PortSecurityClientMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

   	UI32 PortSecurityClientMessage::getIfType() const
    {
        return m_ifType;
    }

    void PortSecurityClientMessage::setNumAllowedMacs(const UI32 &numAllowedMacs)
    {
        m_numAllowedMacs  =  numAllowedMacs;
    }

    UI32 PortSecurityClientMessage::getNumAllowedMacs() const
    {
        return m_numAllowedMacs;
    }

    void PortSecurityClientMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32 PortSecurityClientMessage::getAction() const
    {
        return m_action;
    }

    void PortSecurityClientMessage::setShutdownTime(const UI32 &time)
    {
        m_shutdownTime  =  time;
    }

    UI32 PortSecurityClientMessage::getShutdownTime() const
    {
        return m_shutdownTime;
    }

    void PortSecurityClientMessage::setAllowedMac(const string &mac)
    {
        m_allowedMac  =  mac;
    }

    string PortSecurityClientMessage::getAllowedMac() const
    {
        return m_allowedMac;
    }

    void PortSecurityClientMessage::setOui(const string &oui)
    {
        m_oui  =  oui;
    }

    string PortSecurityClientMessage::getOui() const
    {
        return m_oui;
    }

	void PortSecurityClientMessage::setOpCode(const UI32 &opCode)
	{
		m_opCode = opCode;
	}

	UI32 PortSecurityClientMessage::getOpCode() const
	{
		return m_opCode;
	}

	void PortSecurityClientMessage::setVlanId(const UI32 &vlanId)
	{
		m_vlanId = vlanId;
	}

	UI32 PortSecurityClientMessage::getVlanId() const
	{
		return m_vlanId;
	}

	void PortSecurityClientMessage::setSticky(const bool &sticky)
	{
		m_sticky = sticky;
	}

	bool PortSecurityClientMessage::getSticky() const
	{
		return m_sticky;
	}

	void PortSecurityClientMessage::getStickyMacRecordsOutputBuffer(UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer = findBuffer(bufferNum, size);
	}

	const void *PortSecurityClientMessage::getCStructureForInputs ()
	{
		PortSecurityMsg_t *pInput = new PortSecurityMsg_t;

		bzero(pInput, sizeof(PortSecurityMsg_t));

		if (m_ifType == IF_TYPE_PO) {
			snprintf(pInput->ifName, 64, "po%s", m_ifName.c_str());
		} else if (m_ifType != IF_TYPE_INVALID) {
			GENERATE_SHORT_IF_NAME_FOR_BACK_END(m_ifType, m_ifName.c_str(), pInput->ifName, 64);
		}

		pInput->cmdCode = m_opCode;

		if (m_opCode == PORT_SECURITY_MAC || m_opCode == PORT_SECURITY_MAC_REMOVE) {
			strncpy(pInput->mac, m_allowedMac.c_str(), 20);
			pInput->vlanId = m_vlanId;
		}

		if (m_opCode == PORT_SECURITY_OUI || m_opCode == PORT_SECURITY_OUI_REMOVE) {
			strncpy(pInput->mac, m_oui.c_str(), 20);
		}

		if (m_opCode == PORT_SECURITY_MAC_MAX)
			pInput->numAllowedMacs = m_numAllowedMacs;

		if (m_opCode == PORT_SECURITY_ACTION)
			pInput->action = m_action;

		if (m_opCode == PORT_SECURITY_STICKY)
			pInput->sticky = 1;

		if (m_opCode == PORT_SECURITY_SHUTDOWN_TIME)
			pInput->shutdownTime = m_shutdownTime;

		if (m_opCode == PORT_SECURITY_CONFIG_REPLAY) {
			pInput->numAllowedMacs = m_numAllowedMacs;
			pInput->action = m_action;
			pInput->shutdownTime = m_shutdownTime;
		}

		return pInput;
	}

	void PortSecurityClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		PortSecurityStickyOutputMsg_t *msg = (PortSecurityStickyOutputMsg_t *)pOutputCStructure;
		unsigned int num_records = 0;

		if (msg == NULL)
			return;

		num_records = msg->num_records;
		addBuffer(STICKY_MAC_CREATE_BULK, (sizeof(PortSecurityStickyOutputMsg_t) + ((num_records - 1) * sizeof(PortSecurityStickyMacRecord_t))),
                   pOutputCStructure, false);
	}
}
