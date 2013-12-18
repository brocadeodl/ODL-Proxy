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
 *   Author : lhu                                                     *
 **************************************************************************/

#include "Firmware/Local/FirmwareOpLocalObjectManager.h"
#include "Firmware/Local/FirmwareOpActionMessage.h"
#include "Firmware/Local/FirmwareOpTypes.h"

namespace DcmNs
{

	//////////////////////////////////////////////////////////////
	//  FirmwareOpActionMessage Definition                      //
	//////////////////////////////////////////////////////////////
	FirmwareOpActionMessage::FirmwareOpActionMessage ()
		: PrismMessage (FirmwareOpLocalObjectManager::getPrismServiceId (), FIRMWAREOPACTION)
	{
	}

	FirmwareOpActionMessage::FirmwareOpActionMessage (const string &command)
		: PrismMessage (FirmwareOpLocalObjectManager::getPrismServiceId (), FIRMWAREOPACTION),
		m_command    (command)
	{
	}

	FirmwareOpActionMessage::~FirmwareOpActionMessage ()
	{
	}

	void  FirmwareOpActionMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_command,"command"));
		addSerializableAttribute (new AttributeString(&m_result,"result"));
	}

	void  FirmwareOpActionMessage::setCommand(const string &command)
	{
		m_command  =  command;
	}

	string  FirmwareOpActionMessage::getCommand() const
	{
		return (m_command);
	}

	void  FirmwareOpActionMessage::setResult(const string &result)
	{
		m_result  =  result; 
	}

	string  FirmwareOpActionMessage::getResult() const
	{
		return (m_result);
	}

	void  FirmwareOpActionMessage::setReturnCode(const int code)
	{
		m_returncode = code;
	}

	int	FirmwareOpActionMessage::getReturnCode() const
	{
		return (m_returncode);
	}

	//////////////////////////////////////////////////////////////
	//  NetconfFirmwareDownloadMessage Definition               //
	//////////////////////////////////////////////////////////////
	NetconfFirmwareDownloadMessage::NetconfFirmwareDownloadMessage ()
		: PrismMessage (FirmwareOpLocalObjectManager::getPrismServiceId (), NETCONF_FWDL)
	{
	}

	NetconfFirmwareDownloadMessage::NetconfFirmwareDownloadMessage (const string &protocol, const string &user, const string &password, const string &host, const string &file, bool &reboot)
		: PrismMessage (FirmwareOpLocalObjectManager::getPrismServiceId (), NETCONF_FWDL), m_protocol(protocol), m_user(user), m_password(password), m_host(host), m_file(file), m_reboot(reboot)
	{
	}

	NetconfFirmwareDownloadMessage::~NetconfFirmwareDownloadMessage ()
	{
	}

	void  NetconfFirmwareDownloadMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
 		addSerializableAttribute (new AttributeString (&m_protocol,     "protocol"));
 		addSerializableAttribute (new AttributeString (&m_user,         "user"));
 		addSerializableAttribute (new AttributeString (&m_password,     "password"));
 		addSerializableAttribute (new AttributeString (&m_host,         "host"));
 		addSerializableAttribute (new AttributeString (&m_file,         "file"));
 		addSerializableAttribute (new AttributeString (&m_errmsg,       "errmsg"));
 		addSerializableAttribute (new AttributeBool   (&m_reboot,       "reboot"));
 		addSerializableAttribute (new AttributeUI32   (&m_option,       "option"));
	}


	//////////////////////////////////////////////////////////////
	//  FirmwareOpAutoUpgradeMessage Definition                      //
	//////////////////////////////////////////////////////////////
	FirmwareOpAutoUpgradeMessage::FirmwareOpAutoUpgradeMessage()
		: PrismMessage(FirmwareOpLocalObjectManager::getPrismServiceId(), NODEAUTOUPGRADE)
	{
	}

	FirmwareOpAutoUpgradeMessage::FirmwareOpAutoUpgradeMessage(
					NodeAutoUpgradeConfiguration *pUpgradeConfigData)
		: PrismMessage(FirmwareOpLocalObjectManager::getPrismServiceId(), NODEAUTOUPGRADE), m_pUpgradeConfigData(pUpgradeConfigData)
	{
	}

	FirmwareOpAutoUpgradeMessage::~FirmwareOpAutoUpgradeMessage()
	{
	}

	void  FirmwareOpAutoUpgradeMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization();
		addSerializableAttribute (new AttributeUI32(&m_status,"status"));
	}

	NodeAutoUpgradeConfiguration *FirmwareOpAutoUpgradeMessage::getUpgradeConfigData()
	{
		return (m_pUpgradeConfigData);
	}

	void FirmwareOpAutoUpgradeMessage::setStatus(const UI32 &status)
	{
		m_status = status;
	}

	UI32 FirmwareOpAutoUpgradeMessage::getStatus() const
	{
		return(m_status);
	}

}
