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

#include "Firmware/Global/FirmwareClusterMessage.h"
#include "Firmware/Global/FirmwareObjectManager.h"
#include "Firmware/Global/FirmwareTypes.h"

namespace DcmNs
{

	FirmwareClusterMessage::FirmwareClusterMessage()
		: PrismMessage(FirmwareObjectManager::getPrismServiceId(), FIRMWARECLUSTER)
	{
	}

	FirmwareClusterMessage::FirmwareClusterMessage(const UI32 &state)
		: PrismMessage(FirmwareObjectManager::getPrismServiceId(), FIRMWARECLUSTER),
		m_state			(state)
	{
	}

	/*
	 * Use the same 2 arg constructor for Firmware activate and Firmware recover.
	*/
	FirmwareClusterMessage::FirmwareClusterMessage (const vector<UI32> &rbridgeIds,
		const UI32 &flag)
		: PrismMessage (FirmwareObjectManager::getPrismServiceId (), FIRMWAREACTIVATE),
		m_rbridgeIds	(rbridgeIds),
		m_flag			(flag)
	{
	}

	FirmwareClusterMessage::FirmwareClusterMessage(const vector<UI32> &rbridgeIds,
		const string &user,	const string &password, const string &host,
		const string &path,	const string &proto, const UI32 &flag)
        : PrismMessage(FirmwareObjectManager::getPrismServiceId(), FIRMWARECLUSTER),
		m_rbridgeIds	(rbridgeIds),
		m_user			(user),
		m_password		(password),
		m_host			(host),
		m_path			(path),
		m_proto			(proto),
		m_flag			(flag)
	{
	}

	FirmwareClusterMessage::~FirmwareClusterMessage()
	{
	}

	void FirmwareClusterMessage::setupAttributesForSerialization()
	{
        PrismMessage::setupAttributesForSerialization();
 		addSerializableAttribute (new AttributeUI32             (&m_state,              "state"));
 		addSerializableAttribute (new AttributeUI32             (&m_msg_version,        "msg_version"));
 		addSerializableAttribute (new AttributeUI32Vector       (&m_rbridgeIds,         "rbridgeIds"));
 		addSerializableAttribute (new AttributeString           (&m_user,               "user"));
 		addSerializableAttribute (new AttributeString           (&m_password,           "password"));
 		addSerializableAttribute (new AttributeString           (&m_host,               "host"));
 		addSerializableAttribute (new AttributeString           (&m_path,               "path"));
 		addSerializableAttribute (new AttributeString           (&m_proto,              "proto"));
 		addSerializableAttribute (new AttributeUI32             (&m_flag,               "flag"));
 		addSerializableAttribute (new AttributeSI32Vector       (&m_cluster_nodes_ret,  "cluster_nodes_ret"));
 		addSerializableAttribute (new AttributeSI32Vector       (&m_cluster_fwdl_status,"cluster_fwdl_status"));
 		addSerializableAttribute (new AttributeStringVector     (&m_mesg,               "mesg"));
		addSerializableAttribute (new AttributeStringVector     (&m_firmwareVersion,    "firmwareVersion"));
    }

	void FirmwareClusterMessage::setState(const UI32 &state)
	{
		m_state  =  state;
	}

	void FirmwareClusterMessage::setMsgVersion(const UI32 &msg_version)
	{
		m_msg_version  =  msg_version;
	}

	void FirmwareClusterMessage::setRbridgeIds(const vector<UI32> &rbridgeIds)
	{
		m_rbridgeIds  =  rbridgeIds;
	}

	void FirmwareClusterMessage::setUser(const string &user)
	{
		m_user  =  user;
	}

	void FirmwareClusterMessage::setPassword(const string &password)
	{
		m_password  =  password;
	}

	void FirmwareClusterMessage::setHost(const string &host)
	{
		m_host  =  host;
	}

	void FirmwareClusterMessage::setPath(const string &path)
	{
		m_path  =  path;
	}

	void FirmwareClusterMessage::setProto(const string &proto)
	{
		m_proto  =  proto;
	}

	void FirmwareClusterMessage::setFlag(const UI32 &flag)
	{
		m_flag  =  flag;
	}

	void FirmwareClusterMessage::setClusterNodesRet(const vector<int> &cluster_nodes_ret)
	{
		m_cluster_nodes_ret = cluster_nodes_ret;
	}

	void FirmwareClusterMessage::setClusterNodeMesg(const vector<string> &mesg)
	{
		m_mesg = mesg;
	}

	void FirmwareClusterMessage::setClusterFwdlStatus(const vector<int> &cluster_fwdl_status)
	{
		m_cluster_fwdl_status = cluster_fwdl_status;
	}

	void FirmwareClusterMessage::setFirmwareVersion(const string &firmwareVersion)
	{
		m_firmwareVersion.push_back(firmwareVersion);
	}

	UI32 FirmwareClusterMessage::getState() const
	{
		return (m_state);
	}

	UI32 FirmwareClusterMessage::getMsgVersion() const
	{
		return (m_msg_version);
	}

	vector<UI32> FirmwareClusterMessage::getRbridgeIds() const
	{
		return (m_rbridgeIds);
	}

	string FirmwareClusterMessage::getUser() const
	{
		return (m_user);
	}

	string FirmwareClusterMessage::getPassword() const
	{
		return (m_password);
	}

	string FirmwareClusterMessage::getHost() const
	{
		return (m_host);
	}

	string FirmwareClusterMessage::getPath() const
	{
		return (m_path);
	}

	string FirmwareClusterMessage::getProto() const
	{
		return (m_proto);
	}

	UI32 FirmwareClusterMessage::getFlag() const
	{
		return (m_flag);
	}

	vector<int> FirmwareClusterMessage::getClusterNodesRet() const
	{
		return (m_cluster_nodes_ret);
	}

	vector<string> FirmwareClusterMessage::getClusterNodeMesg() const
	{
		return (m_mesg);
	}

	vector<int> FirmwareClusterMessage::getClusterFwdlStatus() const
	{
		return (m_cluster_fwdl_status);
	}

	vector<string> FirmwareClusterMessage::getFirmwareVersion() const
	{
		return (m_firmwareVersion);
	}
}
