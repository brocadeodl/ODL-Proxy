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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                               *
 **************************************************************************/

#include "Firmware/Global/NodeAutoUpgradeManagedObject.h"
#include "Firmware/Global/NodeAutoUpgradeObjectManager.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

NodeAutoUpgradeManagedObject::NodeAutoUpgradeManagedObject(
		NodeAutoUpgradeObjectManager *pNodeAutoUpgradeObjectManager)
	: PrismElement(pNodeAutoUpgradeObjectManager),
	  PrismPersistableObject(NodeAutoUpgradeManagedObject::getClassName(),
			DcmManagedObject::getClassName()),
	  WaveManagedObject(pNodeAutoUpgradeObjectManager),
	  DcmManagedObject(pNodeAutoUpgradeObjectManager),
	  m_status(false),
	  m_firmwareVersion(""),
	  m_ipaddress(""),
	  m_path(""),
	  m_protocol(1),
	  m_username(""),
	  m_password(""),
	  m_msgVersion(""),
	  m_flags(0)
{
}

NodeAutoUpgradeManagedObject::~NodeAutoUpgradeManagedObject()
{
}

string NodeAutoUpgradeManagedObject::getClassName()
{
	return ("NodeAutoUpgradeManagedObject");
}

void NodeAutoUpgradeManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence();

	addPersistableAttribute (new AttributeBool(&m_status, "status",
			firmware_enable));
	addPersistableAttribute (new AttributeString(&m_firmwareVersion,
			"firmwareVersion"));
	addPersistableAttribute (new AttributeString(&m_ipaddress, "ipaddress",
			firmware_ipaddress));
	addPersistableAttribute (new AttributeString(&m_path, "path",
			firmware_path));
	addPersistableAttribute (new AttributeEnum((UI32 *)(&m_protocol),
			"protocol", firmware_protocol));
	addPersistableAttribute (new AttributeString(&m_username, "username",
			firmware_username));
	addPersistableAttribute (new AttributeString(&m_password, "password",
			firmware_pass));
	addPersistableAttribute (new AttributeString(&m_msgVersion, "msgVersion"));
	addPersistableAttribute (new AttributeUI32(&m_flags, "flags"));
}

void NodeAutoUpgradeManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate();

	addPersistableAttributeForCreate (new AttributeBool(&m_status,
			"status", firmware_enable));
	addPersistableAttributeForCreate (new AttributeString(&m_firmwareVersion,
			"firmwareVersion"));
	addPersistableAttributeForCreate (new AttributeString(&m_ipaddress,
			"ipaddress", firmware_ipaddress));
	addPersistableAttributeForCreate (new AttributeString(&m_path,
			"path", firmware_path));
	addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_protocol),
			"protocol", firmware_protocol));
	addPersistableAttributeForCreate (new AttributeString(&m_username,
			"username", firmware_username));
	addPersistableAttributeForCreate (new AttributeString(&m_password,
			"password", firmware_pass));
	addPersistableAttributeForCreate (new AttributeString(&m_msgVersion,
			"msgVersion"));
	addPersistableAttributeForCreate (new AttributeUI32(&m_flags,
			"flags"));
}

void NodeAutoUpgradeManagedObject::setStatus(const bool &status)
{
	m_status  =  status;
}

bool NodeAutoUpgradeManagedObject::getStatus() const
{
	return (m_status);
}

void NodeAutoUpgradeManagedObject::setFirmwareVersion(const string &firmwareVersion)
{
	m_firmwareVersion  =  firmwareVersion;
}

string NodeAutoUpgradeManagedObject::getFirmwareVersion() const
{
	return (m_firmwareVersion);
}

void NodeAutoUpgradeManagedObject::setIpAddress(const string &ipaddress)
{
	m_ipaddress  =  ipaddress;
}

string NodeAutoUpgradeManagedObject::getIpAddress() const
{
	return (m_ipaddress);
}

void NodeAutoUpgradeManagedObject::setPath(const string &path)
{
	m_path  =  path;
}

string NodeAutoUpgradeManagedObject::getPath() const
{
	return (m_path);
}

void NodeAutoUpgradeManagedObject::setProtocol(const UI32 &protocol)
{
	m_protocol  =  protocol;
}

UI32 NodeAutoUpgradeManagedObject::getProtocol() const
{
	return (m_protocol);
}

void NodeAutoUpgradeManagedObject::setUsername(const string &username)
{
	m_username  =  username;
}

string NodeAutoUpgradeManagedObject::getUsername() const
{
	return (m_username);
}

void NodeAutoUpgradeManagedObject::setPassword(const string &password)
{
	m_password  =  password;
}

string NodeAutoUpgradeManagedObject::getPassword() const
{
	return (m_password);
}

void NodeAutoUpgradeManagedObject::setMsgVersion(const string &msgVersion)
{
	m_msgVersion  =  msgVersion;
}

string NodeAutoUpgradeManagedObject::getMsgVersion() const
{
	return (m_msgVersion);
}

void NodeAutoUpgradeManagedObject::setFlags(const UI32 &flags)
{
	m_flags  =  flags;
}

UI32 NodeAutoUpgradeManagedObject::getFlags() const
{
	return (m_flags);
}

}
