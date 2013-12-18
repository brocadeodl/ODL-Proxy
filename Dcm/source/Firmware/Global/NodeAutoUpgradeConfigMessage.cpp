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
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#include "Firmware/Global/NodeAutoUpgradeConfigMessage.h"
#include "Firmware/Global/NodeAutoUpgradeObjectManager.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

	NodeAutoUpgradeConfigMessage::NodeAutoUpgradeConfigMessage()
		: PrismMessage(NodeAutoUpgradeObjectManager::getPrismServiceId(),
			NODEAUTOUPGRADECONFIG)
	{
	}

	NodeAutoUpgradeConfigMessage::NodeAutoUpgradeConfigMessage(
		const UI32 &status, const string &firmwareVersion,
		const string &host, const string &path,
		const UI32 &protocol, const string &username,
		const string &password, const UI32 &flags)
		: PrismMessage(NodeAutoUpgradeObjectManager::getPrismServiceId(),
			NODEAUTOUPGRADECONFIG),
		m_status(status),
		m_firmwareVersion(firmwareVersion),
		m_host(host),
		m_path(path),
		m_protocol(protocol),
		m_username(username),
		m_password(password),
		m_flags(flags)
	{
	}

	NodeAutoUpgradeConfigMessage::~NodeAutoUpgradeConfigMessage()
	{
	}

	void NodeAutoUpgradeConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization();
		addSerializableAttribute(new AttributeUI32(&m_status, "status"));
		addSerializableAttribute(new AttributeString(&m_firmwareVersion,
				"firmwareVersion"));
		addSerializableAttribute(new AttributeString(&m_host, "host"));
		addSerializableAttribute(new AttributeString(&m_path, "path"));
		addSerializableAttribute(new AttributeUI32(&m_protocol, "protocol"));
		addSerializableAttribute(new AttributeString(&m_username, "username"));
		addSerializableAttribute(new AttributeString(&m_password, "password"));
		addSerializableAttribute(new AttributeUI32(&m_flags, "flags"));
	}

	void NodeAutoUpgradeConfigMessage::setStatus(const UI32 &status)
	{
		m_status  =  status;
    }

	UI32 NodeAutoUpgradeConfigMessage::getStatus() const
	{
		return (m_status);
	}

	void NodeAutoUpgradeConfigMessage::setFirmwareVersion(const string &firmwareVersion)
	{
		m_firmwareVersion = firmwareVersion;
	}

	string NodeAutoUpgradeConfigMessage::getFirmwareVersion() const
	{
		return (m_firmwareVersion);
	}

	void NodeAutoUpgradeConfigMessage::setHost(const string &host)
	{
		m_host  =  host;
	}

	string NodeAutoUpgradeConfigMessage::getHost() const
	{
		return (m_host);
	}

	void NodeAutoUpgradeConfigMessage::setPath(const string &path)
	{
		m_path  =  path;
	}

	string NodeAutoUpgradeConfigMessage::getPath() const
	{
		return (m_path);
	}

	void NodeAutoUpgradeConfigMessage::setProtocol(const UI32 &protocol)
	{
		m_protocol  =  protocol;
	}

	UI32 NodeAutoUpgradeConfigMessage::getProtocol() const
	{
		return (m_protocol);
	}

	void NodeAutoUpgradeConfigMessage::setUsername(const string &username)
	{
		m_username  =  username;
	}

	string NodeAutoUpgradeConfigMessage::getUsername() const
	{
		return (m_username);
	}

	void NodeAutoUpgradeConfigMessage::setPassword(const string &password)
	{
		m_password  =  password;
	}

	string NodeAutoUpgradeConfigMessage::getPassword() const
	{
		return (m_password);
	}

	void NodeAutoUpgradeConfigMessage::setFlags(const UI32 &flags)
	{
		m_flags  =  flags;
	}

	UI32 NodeAutoUpgradeConfigMessage::getFlags() const
	{
		return (m_flags);
	}

}
