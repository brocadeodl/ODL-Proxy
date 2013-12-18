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

#ifndef NODEAUTOUPGRADEMANAGEDOBJECT_H
#define NODEAUTOUPGRADEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/SerializableObject.h"
#include "Firmware/Global/FirmwareTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class NodeAutoUpgradeObjectManager;
class NodeAutoUpgradeManagedObject : virtual public DcmManagedObject
{
	private:
	protected:
		virtual void setupAttributesForPersistence();
		virtual void setupAttributesForCreate();
	public:
		NodeAutoUpgradeManagedObject(NodeAutoUpgradeObjectManager *pNodeAutoUpgradeObjectManager);
		virtual ~NodeAutoUpgradeManagedObject ();
		static string getClassName();
		void setStatus(const bool &status);
		bool getStatus() const;
		void setFirmwareVersion(const string &firmwareVersion);
		string getFirmwareVersion() const;
		void setIpAddress(const string &ipaddress);
		string getIpAddress() const;
		void setPath(const string &path);
		string getPath() const;
		void setProtocol(const UI32 &protocol);
		UI32 getProtocol() const;
		void setUsername(const string &username);
		string getUsername() const;
		void setPassword(const string &password);
		string getPassword() const;
		void setMsgVersion(const string &msgVersion);
		string getMsgVersion() const;
		void setFlags(const UI32 &flags);
		UI32 getFlags() const;

	// Now the data members

	private:
	protected:
	public:
		bool		m_status;
		string		m_firmwareVersion;
		string		m_ipaddress;
		string		m_path;
		UI32		m_protocol;
		string		m_username;
		string		m_password;
		string		m_msgVersion;
		UI32		m_flags;
};
}
#endif		//NODEAUTOUPGRADEMANAGEDOBJECT_H
