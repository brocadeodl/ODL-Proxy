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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sumank                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmPortSecurityGlobalConfigMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmPortSecurityGlobalConfigMessage::NsmPortSecurityGlobalConfigMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMPORTSECURITYGLOBALCONFIGMSG)
    {
    }

    NsmPortSecurityGlobalConfigMessage::NsmPortSecurityGlobalConfigMessage (const UI8 &opCode,const UI32 &numAllowedMacs,const UI8 &action,const string &mac)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMPORTSECURITYGLOBALCONFIGMSG),
        m_opCode    (opCode),
        m_numAllowedMacs    (numAllowedMacs),
        m_action    (action),
        m_mac    (mac)
    {
    }

    NsmPortSecurityGlobalConfigMessage::~NsmPortSecurityGlobalConfigMessage ()
    {
    }

    void  NsmPortSecurityGlobalConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeUI32(&m_numAllowedMacs,"numAllowedMacs"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_action,"action"));
        addSerializableAttribute (new AttributeString(&m_mac,"mac"));
        addSerializableAttribute (new AttributeString(&m_oui,"oui"));
        addSerializableAttribute (new AttributeBool(&m_sticky,"sticky"));
		addSerializableAttribute (new AttributeUI32(&m_shutdownTime,"shutdownTime"));
    }

    void  NsmPortSecurityGlobalConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmPortSecurityGlobalConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  NsmPortSecurityGlobalConfigMessage::setNumAllowedMacs(const UI32 &numAllowedMacs)
    {
        m_numAllowedMacs  =  numAllowedMacs;
    }

    UI32  NsmPortSecurityGlobalConfigMessage::getNumAllowedMacs() const
    {
        return (m_numAllowedMacs);
    }

    void  NsmPortSecurityGlobalConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmPortSecurityGlobalConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmPortSecurityGlobalConfigMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  NsmPortSecurityGlobalConfigMessage::getAction() const
    {
        return (m_action);
    }

    void NsmPortSecurityGlobalConfigMessage::setMac(const string &mac)
    {
        m_mac  =  mac;
    }

    string NsmPortSecurityGlobalConfigMessage::getMac() const
    {
        return m_mac;
    }

    void NsmPortSecurityGlobalConfigMessage::setOui(const string &oui)
    {
        m_oui  =  oui;
    }

    string NsmPortSecurityGlobalConfigMessage::getOui() const
    {
        return m_oui;
    }

	void  NsmPortSecurityGlobalConfigMessage::setIfName(const string &ifName)
	{
		m_ifName  =  ifName;
	}

	string  NsmPortSecurityGlobalConfigMessage::getIfName() const
	{
		return (m_ifName);
	}

	void  NsmPortSecurityGlobalConfigMessage::setIfType(const InterfaceType &ifType)
	{
		m_ifType  =  ifType;
	}

	InterfaceType  NsmPortSecurityGlobalConfigMessage::getIfType() const
	{
		return m_ifType;
	}

	void  NsmPortSecurityGlobalConfigMessage::setSticky(const bool &sticky)
	{
		m_sticky  =  sticky;
	}

	bool  NsmPortSecurityGlobalConfigMessage::getSticky() const
	{
		return m_sticky;
	}

    void  NsmPortSecurityGlobalConfigMessage::setShutdownTime(const UI32 &time)
    {
        m_shutdownTime  =  time;
    }

    UI32  NsmPortSecurityGlobalConfigMessage::getShutdownTime() const
    {
        return (m_shutdownTime);
    }
}

