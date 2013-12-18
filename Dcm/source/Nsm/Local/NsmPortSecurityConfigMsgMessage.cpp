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
 *   Author : skrastog                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmPortSecurityConfigMsgMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmPortSecurityConfigMsgMessage::NsmPortSecurityConfigMsgMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMPORTSECURITYCONFIGMSG)
    {
		m_sticky = false;
    }

    NsmPortSecurityConfigMsgMessage::NsmPortSecurityConfigMsgMessage (const UI8 &opCode,const UI32 &numAllowedMacs,const UI8 &action,const string &mac)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMPORTSECURITYCONFIGMSG),
        m_opCode    (opCode),
        m_numAllowedMacs    (numAllowedMacs),
        m_action    (action),
        m_mac    (mac)
    {
		m_sticky = false;
    }

    NsmPortSecurityConfigMsgMessage::~NsmPortSecurityConfigMsgMessage ()
    {
    }

    void  NsmPortSecurityConfigMsgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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

    void  NsmPortSecurityConfigMsgMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmPortSecurityConfigMsgMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  NsmPortSecurityConfigMsgMessage::setNumAllowedMacs(const UI32 &numAllowedMacs)
    {
        m_numAllowedMacs  =  numAllowedMacs;
    }

    UI32  NsmPortSecurityConfigMsgMessage::getNumAllowedMacs() const
    {
        return (m_numAllowedMacs);
    }

    void  NsmPortSecurityConfigMsgMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmPortSecurityConfigMsgMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmPortSecurityConfigMsgMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  NsmPortSecurityConfigMsgMessage::getAction() const
    {
        return (m_action);
    }

    void NsmPortSecurityConfigMsgMessage::setMac(const string &mac)
    {
        m_mac  =  mac;
    }

    string NsmPortSecurityConfigMsgMessage::getMac() const
    {
        return m_mac;
    }

    void NsmPortSecurityConfigMsgMessage::setOui(const string &oui)
    {
        m_oui  =  oui;
    }

    string NsmPortSecurityConfigMsgMessage::getOui() const
    {
        return m_oui;
    }

	void  NsmPortSecurityConfigMsgMessage::setIfName(const string &ifName)
	{
		m_ifName  =  ifName;
	}

	string  NsmPortSecurityConfigMsgMessage::getIfName() const
	{
		return (m_ifName);
	}

	void  NsmPortSecurityConfigMsgMessage::setIfType(const InterfaceType &ifType)
	{
		m_ifType  =  ifType;
	}

	InterfaceType  NsmPortSecurityConfigMsgMessage::getIfType() const
	{
		return m_ifType;
	}

	void  NsmPortSecurityConfigMsgMessage::setSticky(const bool &sticky)
	{
		m_sticky  =  sticky;
	}

	bool  NsmPortSecurityConfigMsgMessage::getSticky() const
	{
		return m_sticky;
	}

    void  NsmPortSecurityConfigMsgMessage::setShutdownTime(const UI32 &time)
    {
        m_shutdownTime  =  time;
    }

    UI32  NsmPortSecurityConfigMsgMessage::getShutdownTime() const
    {
        return (m_shutdownTime);
    }
}
