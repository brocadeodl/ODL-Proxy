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

#ifndef FIRMWARECLUSTERMESSAGE_H
#define FIRMWARECLUSTERMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FirmwareClusterMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FirmwareClusterMessage ();
            FirmwareClusterMessage (const UI32 &state);
			FirmwareClusterMessage (const vector<UI32> &rbridgeIds,
				const UI32 &flag);
			FirmwareClusterMessage (const vector<UI32> &rbridgeIds,
				const string &user, const string &password,
				const string &host, const string &path,
				const string &proto, const UI32 &flag);
            virtual    ~FirmwareClusterMessage ();
			void setState(const UI32 &state);
			void setMsgVersion(const UI32 &msg_version);
			void setRbridgeIds(const vector<UI32> &rbridgeIds);
			void setUser(const string &user);
			void setPassword(const string &password);
			void setHost(const string &host);
			void setPath(const string &path);
			void setProto(const string &proto);
			void setFlag(const UI32 &flag);
			void setClusterNodesRet(const vector<int> &cluster_nodes_ret);
			void setClusterNodeMesg(const vector<string> &mesg);
			void setClusterFwdlStatus(const vector<int> &cluster_fwdl_status);
			void setFirmwareVersion(const string &firmwareVersion);
			UI32 getState() const;
			UI32 getMsgVersion() const;
			vector<UI32> getRbridgeIds() const;
			vector<int> getClusterNodesRet() const;
			vector<int> getClusterFwdlStatus() const;
			string getUser() const;
			string getPassword() const;
			string getHost() const;
			string getPath() const;
			string getProto() const;
			UI32 getFlag() const;
			vector<string> getClusterNodeMesg() const;
			vector<string> getFirmwareVersion() const;

		// Now the data members
        private:
        protected:
        public:
			UI32			m_msg_version;
			UI32			m_state;
			vector<UI32>	m_rbridgeIds;
			string			m_user;
			string			m_password;
			string			m_host;
			string			m_path;
			string			m_proto;
			UI32			m_flag;
			vector<int>		m_cluster_nodes_ret;
			vector<string>	m_mesg;
			vector<int>		m_cluster_fwdl_status;
			vector<string>	m_firmwareVersion;
    };
}
#endif		//FIRMWARECLUSTERMESSAGE_H
