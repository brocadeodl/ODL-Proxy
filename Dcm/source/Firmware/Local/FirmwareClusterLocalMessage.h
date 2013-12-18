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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#ifndef FIRMWARECLUSTERLOCALMESSAGE_H
#define FIRMWARECLUSTERLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FirmwareClusterLocalMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
			FirmwareClusterLocalMessage ();
			FirmwareClusterLocalMessage (const UI32 &state);
			FirmwareClusterLocalMessage (const UI32 &state,
				const UI32 &rbridge_id);
			FirmwareClusterLocalMessage (const UI32 &state, const string &user,
				const string &password,	const string &host,const string &path,
				const string &proto, const UI32 &flag, const UI32 &rbridge_id);
            virtual    ~FirmwareClusterLocalMessage ();
			void setState(const UI32 &state);
			void setUser(const string &user);
			void setPassword(const string &password);
			void setHost(const string &host);
			void setPath(const string &path);
			void setProto(const string &proto);
			void setFlag(const UI32 &flag);
			void setRbridgeId(const UI32 &rbridge_id);
			void setLocalNodeRet(const int &local_node_ret);
			void setLocalFwdlStatus(const int &local_fwdl_status);
			void setLocalNodeMesg(const string &mesg);
			void setFirmwareVersion(const string &firmwareVersion);
			UI32 getState() const;
			string getUser() const;
			string getPassword() const;
			string getHost() const;
			string getPath() const;
			string getProto() const;
			UI32 getFlag() const;
			UI32 getRbridgeId() const;
			int getLocalNodeRet() const;
			int getLocalFwdlStatus() const;
			string getLocalNodeMesg() const;
			string getFirmwareVersion() const;

		// Now the data members
        private:
        protected:
        public:
			UI32		m_state;
			string		m_user;
			string		m_password;
			string		m_host;
			string		m_path;
			string		m_proto;
			UI32		m_flag;
			UI32		m_rbridge_id;
			int			m_local_node_ret;
			string		m_mesg;
			int			m_local_fwdl_status;
			string		m_firmwareVersion;
    };

	class NetconfStatusMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NetconfStatusMessage();
			virtual ~NetconfStatusMessage();
			void setRbridgeId(const UI32 &rbridge_id);
			void setFwdlState(const int &fwdlState);
			void setNumOfEntries(const UI32 &numOfEntries);
			void setIndex(const vector<UI32> &index);
			void setMessageId(const vector<UI32> &messageId);
			void setDateAndTime(const vector<string> &dateAndTime);
			void setMessage(const vector<string> &message);
			void setBladeName(const vector<string> &bladeName);
			UI32 getRbridgeId() const;
			int getFwdlState() const;
			UI32 getNumOfEntries() const;
			vector<UI32> getIndex() const;
			vector<UI32> getMessageId() const;
			vector<string> getDateAndTime() const;
			vector<string> getMessage() const;
			vector<string> getBladeName() const;

		// Now the data members
		private:
		protected:
		public:
			UI32				m_rbridge_id;
			int					m_fwdlState;
			UI32				m_numOfEntries;
			vector<UI32>		m_index;
			vector<UI32>		m_messageId;
			vector<string>		m_dateAndTime;
			vector<string>		m_message;
			vector<string>		m_bladeName;
	};
}
#endif		//FIRMWARECLUSTERLOCALMESSAGE_H
