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
#include "Interface/ContainerInList/InterfaceContainerInListConfigureMessage.h"
#include "Interface/ContainerInList/InterfaceContainerInListTypes.h"

namespace DcmNs
{

InterfaceContainerInListConfigureMessage::InterfaceContainerInListConfigureMessage () 
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTCONFIGURE),
	m_ip	(""),
	m_wwn   (WorldWideName("00:00:00:00:00:00:00:00")),
	m_mtu32	(0),
    m_configTypeChoice (NO_CHOICE),
    m_fcFabricId(0),
    m_ethIpAddress(0)
{
}

InterfaceContainerInListConfigureMessage::InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId) 
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTCONFIGURE),
	m_interfaceObjectId(interfaceObjectId),
	m_ip	(""),
	m_wwn   (WorldWideName("00:00:00:00:00:00:00:00")),
	m_mtu32	(0),
    m_configTypeChoice (NO_CHOICE),
    m_fcFabricId(0),
    m_ethIpAddress(0)
{
}

InterfaceContainerInListConfigureMessage::InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const IpV4Address &ip, const WorldWideName &wwn, const SI32 &mtu32, UI32 &configTypeChoice, SI32 &fcFabricId, SI32 &ethIpAddress)
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTCONFIGURE),
	m_interfaceObjectId(interfaceObjectId),
	m_ip	(ip),
	m_wwn	(wwn),
	m_mtu32	(mtu32),
    m_configTypeChoice (configTypeChoice),
    m_fcFabricId(fcFabricId),
    m_ethIpAddress(ethIpAddress)
{
}

InterfaceContainerInListConfigureMessage::InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const IpV4Address &ip)
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTCONFIGURE),
	m_interfaceObjectId(interfaceObjectId),
	m_ip	(ip),
	m_wwn   (WorldWideName("00:00:00:00:00:00:00:00")),
	m_mtu32 (0),
    m_configTypeChoice (NO_CHOICE),
    m_fcFabricId(0),
    m_ethIpAddress(0)
{
}

InterfaceContainerInListConfigureMessage::InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const WorldWideName &wwn)
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTCONFIGURE),
	m_interfaceObjectId(interfaceObjectId),
	m_ip	(""),
	m_wwn	(wwn),
	m_mtu32 (0),
    m_configTypeChoice (NO_CHOICE),
    m_fcFabricId(0),
    m_ethIpAddress(0)
{
}

InterfaceContainerInListConfigureMessage::InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const SI32 &mtu32)
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTCONFIGURE),
	m_interfaceObjectId(interfaceObjectId),
	m_ip    (""),
	m_wwn	(WorldWideName("00:00:00:00:00:00:00:00")),
	m_mtu32(mtu32),
    m_configTypeChoice (NO_CHOICE),
    m_fcFabricId(0),
    m_ethIpAddress(0)
{
}

InterfaceContainerInListConfigureMessage::InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const UI32 &configTypeChoice, const SI32 &fcFabricId, const SI32 &ethIpAddress)
	: PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACECONTAINERINLISTCONFIGURE),
	m_interfaceObjectId(interfaceObjectId),
	m_ip    (""),
	m_wwn	(WorldWideName("00:00:00:00:00:00:00:00")),
	m_mtu32(0),
    m_configTypeChoice (configTypeChoice),
    m_fcFabricId(fcFabricId),
    m_ethIpAddress(ethIpAddress)
{
}

InterfaceContainerInListConfigureMessage::~InterfaceContainerInListConfigureMessage ()
{
}

ObjectId InterfaceContainerInListConfigureMessage::getInterfaceObjectId() const
{
	return(m_interfaceObjectId);
}

void InterfaceContainerInListConfigureMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();

	addSerializableAttribute (new AttributeObjectId(&m_interfaceObjectId,"interfaceObjectId"));
	addSerializableAttribute (new AttributeIpV4Address(&m_ip,"ip"));
	addSerializableAttribute (new AttributeWorldWideName(&m_wwn,"wwn"));
	addSerializableAttribute (new AttributeSI32(&m_mtu32,"mtu32"));
	addSerializableAttribute (new AttributeUI32(&m_configTypeChoice,"configTypeChoice"));
	addSerializableAttribute (new AttributeSI32(&m_fcFabricId,"fcFabricId"));
	addSerializableAttribute (new AttributeSI32(&m_ethIpAddress,"ethIpAddress"));
}

void InterfaceContainerInListConfigureMessage::setIp(const IpV4Address &ip)
{
	m_ip = ip;
}

IpV4Address InterfaceContainerInListConfigureMessage::getIp() const
{
	return (m_ip);
}

void InterfaceContainerInListConfigureMessage::setWwn(const WorldWideName &wwn)
{
	m_wwn = wwn;
}

WorldWideName InterfaceContainerInListConfigureMessage::getWwn() const
{
	return (m_wwn);
}

void InterfaceContainerInListConfigureMessage::setMtu32(const SI32 &mtu32)
{
	m_mtu32 = mtu32;
}

SI32 InterfaceContainerInListConfigureMessage::getMtu32() const
{
	return (m_mtu32);
}

UI32 InterfaceContainerInListConfigureMessage::getConfigTypeChoice () const
{
    return (m_configTypeChoice);
}

void InterfaceContainerInListConfigureMessage::setConfigTypeChoice (const UI32 &configTypeChoice)
{
    m_configTypeChoice = configTypeChoice;
}

SI32 InterfaceContainerInListConfigureMessage::getFcFabricId () const
{
    return (m_fcFabricId);
}

void InterfaceContainerInListConfigureMessage::setFcFabricId (const SI32 &fcFabricId)
{
    m_fcFabricId = fcFabricId;
}
                
SI32 InterfaceContainerInListConfigureMessage::getEthIpAddress () const
{
    return (m_ethIpAddress);
}

void InterfaceContainerInListConfigureMessage::setEthIpAddress (const SI32 &ethIpAddress)
{
    m_ethIpAddress = ethIpAddress;
}

}
