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
 *   Author : lhu                                                     *
 **************************************************************************/

#ifndef FIRMWAREOPACTIONMESSAGE_H
#define FIRMWAREOPACTIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Firmware/Global/NodeAutoUpgradeConfiguration.h"

using namespace WaveNs;

namespace DcmNs
{

	//////////////////////////////////////////////////////////////
	//  FirmwareOpActionMessage Declaration                     //
	//////////////////////////////////////////////////////////////
	class FirmwareOpActionMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			FirmwareOpActionMessage ();
			FirmwareOpActionMessage (const string &command);
			virtual    ~FirmwareOpActionMessage ();
			void setCommand(const string &command);
			string getCommand()  const;
			void setResult(const string &result);
			string getResult()  const;
			void setReturnCode(const int code);
			int getReturnCode()  const;

// Now the data members

		private:
		protected:
		public:
			string	m_command;
			string	m_result;
			int		m_returncode;
    };

	//////////////////////////////////////////////////////////////
	//  NetconfFirmwareDownloadMessage Declaration              //
	//////////////////////////////////////////////////////////////
	class NetconfFirmwareDownloadMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NetconfFirmwareDownloadMessage();
			NetconfFirmwareDownloadMessage(const string &protocol,
				const string &user, const string &password, const string &host,
				const string &file, bool &reboot);
			virtual    ~NetconfFirmwareDownloadMessage();

		private:
		protected:
		public:
			string          m_protocol;
			string          m_user;
			string          m_password;
			string          m_host;
			string          m_file;
			bool            m_reboot;
			string          m_errmsg;
			UI32			m_option;
	};

	//////////////////////////////////////////////////////////////
	//  FirmwareOpAutoUpgradeMessage Declaration                //
	//////////////////////////////////////////////////////////////
	class FirmwareOpAutoUpgradeMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			FirmwareOpAutoUpgradeMessage();
			FirmwareOpAutoUpgradeMessage(NodeAutoUpgradeConfiguration *NodeAutoUpgradeConfigData);
			virtual ~FirmwareOpAutoUpgradeMessage();
			NodeAutoUpgradeConfiguration *getUpgradeConfigData();
			void setStatus(const UI32 &status);
			UI32 getStatus() const;

// Now the data members

		private:
		protected:
		public:
			NodeAutoUpgradeConfiguration *m_pUpgradeConfigData;
			UI32	m_status;
	};
}
#endif		//FIRMWAREOPACTIONMESSAGE_H
