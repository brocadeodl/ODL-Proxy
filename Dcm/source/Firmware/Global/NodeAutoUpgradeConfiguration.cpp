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
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "NodeAutoUpgradeConfiguration.h"
#include "Framework/Utils/FrameworkToolKit.h"

using namespace std;

namespace WaveNs
{

NodeAutoUpgradeConfiguration::NodeAutoUpgradeConfiguration()
	: SerializableObject(),
	  m_status (false),
	  m_firmwareVersion (""),
	  m_host (""),
	  m_path (""),
	  m_protocol (0),
	  m_username (""),
	  m_password(""),
	  m_msgVersion(""),
	  m_flags(0)
{
}

NodeAutoUpgradeConfiguration::~NodeAutoUpgradeConfiguration()
{
}

void NodeAutoUpgradeConfiguration::setupAttributesForSerialization()
{
	SerializableObject::setupAttributesForSerialization();

 	addSerializableAttribute (new AttributeBool     (&m_status,         "status"));
 	addSerializableAttribute (new AttributeString   (&m_firmwareVersion,"firmwareVersion"));
 	addSerializableAttribute (new AttributeString   (&m_host,           "host"));
 	addSerializableAttribute (new AttributeString   (&m_path,           "path"));
 	addSerializableAttribute (new AttributeUI32     (&m_protocol,       "protocol"));
 	addSerializableAttribute (new AttributeString   (&m_username,       "username"));
 	addSerializableAttribute (new AttributeString   (&m_password,       "password"));
 	addSerializableAttribute (new AttributeString   (&m_msgVersion,     "msgVersion"));
 	addSerializableAttribute (new AttributeUI32     (&m_flags,          "flags"));
}

void NodeAutoUpgradeConfiguration::setStatus(const bool &status)
{
	m_status  =  status;
}

bool NodeAutoUpgradeConfiguration::getStatus() const
{
	return (m_status);
}

void NodeAutoUpgradeConfiguration::setFirmwareVersion(const string &firmwareVersion)
{
	m_firmwareVersion  =  firmwareVersion;
}

string NodeAutoUpgradeConfiguration::getFirmwareVersion() const
{
	return (m_firmwareVersion);
}

void NodeAutoUpgradeConfiguration::setHost(const string &host)
{
	m_host  =  host;
}

string NodeAutoUpgradeConfiguration::getHost() const
{
	return (m_host);
}

void NodeAutoUpgradeConfiguration::setPath(const string &path)
{
	m_path  =  path;
}

string NodeAutoUpgradeConfiguration::getPath() const
{
	return (m_path);
}

void NodeAutoUpgradeConfiguration::setProtocol(const UI32 &protocol)
{
	m_protocol  =  protocol;
}

UI32 NodeAutoUpgradeConfiguration::getProtocol() const
{
	return (m_protocol);
}

void NodeAutoUpgradeConfiguration::setUsername(const string &username)
{
	m_username  =  username;
}

string NodeAutoUpgradeConfiguration::getUsername() const
{
	return (m_username);
}

void NodeAutoUpgradeConfiguration::setPassword(const string &password)
{
	m_password  =  password;
}

string NodeAutoUpgradeConfiguration::getPassword() const
{
	return (m_password);
}

void NodeAutoUpgradeConfiguration::setMsgVersion(const string &msgVersion)
{
	m_msgVersion  =  msgVersion;
}

string NodeAutoUpgradeConfiguration::getMsgVersion() const
{
	return (m_msgVersion);
}

void NodeAutoUpgradeConfiguration::setFlags(const UI32 &flags)
{
	m_flags  =  flags;
}

UI32 NodeAutoUpgradeConfiguration::getFlags() const
{
	return (m_flags);
}

}
