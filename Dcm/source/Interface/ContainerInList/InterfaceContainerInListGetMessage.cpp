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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/ContainerInList/InterfaceContainerInListGetMessage.h"
#include "Interface/ContainerInList/InterfaceContainerInListTypes.h"

namespace DcmNs
{

InterfaceContainerInListGetMessage::InterfaceContainerInListGetMessage ()
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTGET)
{
}

InterfaceContainerInListGetMessage::InterfaceContainerInListGetMessage (const IpV4Address &ip, const WorldWideName &wwn, const SI32 &mtu32, const UI32 &configTypeChoice, const SI32 &fcFabricId, const SI32 &ethIpAddress)
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTGET),
	m_ip(ip),
	m_wwn(wwn),
	m_mtu32(mtu32),
    m_configTypeChoice (configTypeChoice),
    m_fcFabricId(fcFabricId),
    m_ethIpAddress(ethIpAddress)
{
}

InterfaceContainerInListGetMessage::~InterfaceContainerInListGetMessage ()
{
}

void InterfaceContainerInListGetMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeIpV4Address(&m_ip,"ip"));
	addSerializableAttribute (new AttributeWorldWideName(&m_wwn,"wwn"));
	addSerializableAttribute (new AttributeSI32(&m_mtu32,"mtu32"));
	addSerializableAttribute (new AttributeUI32(&m_configTypeChoice,"configTypeChoice"));
	addSerializableAttribute (new AttributeSI32(&m_fcFabricId,"fcFabricId"));
	addSerializableAttribute (new AttributeSI32(&m_ethIpAddress,"ethIpAddress"));
}

void InterfaceContainerInListGetMessage::setIp(const IpV4Address &ip)
{
	m_ip = ip;
}

IpV4Address InterfaceContainerInListGetMessage::getIp() const
{
	return (m_ip);
}

void InterfaceContainerInListGetMessage::setWwn(const WorldWideName &wwn)
{
	m_wwn = wwn;
}

WorldWideName InterfaceContainerInListGetMessage::getWwn() const
{
	return (m_wwn);
}

void InterfaceContainerInListGetMessage::setMtu32(const SI32 &mtu32)
{
	m_mtu32 = mtu32;
}

SI32 InterfaceContainerInListGetMessage::getMtu32() const
{
	return (m_mtu32);
}

UI32 InterfaceContainerInListGetMessage::getConfigTypeChoice () const
{
    return (m_configTypeChoice);
}

void InterfaceContainerInListGetMessage::setConfigTypeChoice (const UI32 &configTypeChoice)
{
    m_configTypeChoice = configTypeChoice;
}

SI32 InterfaceContainerInListGetMessage::getFcFabricId () const
{
    return (m_fcFabricId);
}

void InterfaceContainerInListGetMessage::setFcFabricId (const SI32 &fcFabricId)
{
    m_fcFabricId = fcFabricId;
}
                
}
