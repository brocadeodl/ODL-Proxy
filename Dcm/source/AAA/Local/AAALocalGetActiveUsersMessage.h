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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef AAALOCALGETACTIVEUSERSMESSAGE_H
#define AAALOCALGETACTIVEUSERSMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeDateTime.h"

using namespace WaveNs;

namespace DcmNs
{

	class AAALocalGetActiveUsersMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			AAALocalGetActiveUsersMessage ();
			AAALocalGetActiveUsersMessage (const vector<string> &userName,
				const vector<string> &hostName, const vector<UI32> &sessionId,
				const vector<string> &ipAddr, const vector<string> &deviceName,
				const vector<UI32> &pid,const vector<DateTime> &timeOfLog,
				const vector<string> &role);
			virtual    ~AAALocalGetActiveUsersMessage ();

			UI32 getNumVectors() const;
			void clearNumVectors();
			void setActiveUser(const string &userName, const string &hostName,
				const UI32 &sessionId, const string &ipAddr,
				const string &deviceName, const UI32 &pid,
				const DateTime &timeOfLog, const string &role);
			bool getActiveUser(const UI32 &index, string &userName,
				string &hostName, UI32 &sessionId, string &ipAddr,
				string &deviceName, UI32 &pid, DateTime &timeOfLog, string &role);

// Now the data members

		private:
		protected:
		public:
			vector<string>  m_vUserName;
			vector<string>  m_vHostName;
			vector<UI32>  m_vSessionId;
			vector<string> m_vIpAddr;
			vector<string>  m_vDeviceName;
			vector<UI32>  m_vPid;
			vector<DateTime> m_vTimeOfLog;
			vector<string>  m_vRole;
	};
}
#endif       //AAALOCALGETACTIVEUSERSMESSAGE_H
