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

#include "Firmware/Local/FirmwareClusterLocalObjectManager.h"
#include "Firmware/Local/FirmwareClusterLocalMessage.h"
#include "Firmware/Local/FirmwareClusterTypes.h"

namespace DcmNs
{

    FirmwareClusterLocalMessage::FirmwareClusterLocalMessage ()
        : PrismMessage (FirmwareClusterLocalObjectManager::getPrismServiceId (), FIRMWARECLUSTERLOCAL)
    {
    }

    FirmwareClusterLocalMessage::FirmwareClusterLocalMessage (const UI32 &state,
		const UI32 &rbridge_id)
        : PrismMessage (FirmwareClusterLocalObjectManager::getPrismServiceId (), FIRMWARECLUSTERLOCAL),
        m_state			(state),
		m_rbridge_id    (rbridge_id)
    {
    }

    FirmwareClusterLocalMessage::FirmwareClusterLocalMessage (const UI32 &state,
		const string &user,	const string &password, const string &host,
		const string &path,	const string &proto, const UI32 &flag,
		const UI32 &rbridge_id)
        : PrismMessage (FirmwareClusterLocalObjectManager::getPrismServiceId (), FIRMWARECLUSTERLOCAL),
		m_state			(state),
		m_user    		(user),
		m_password		(password),
		m_host			(host),
		m_path			(path),
		m_proto			(proto),
		m_flag			(flag),
		m_rbridge_id	(rbridge_id)
    {
    }

    FirmwareClusterLocalMessage::~FirmwareClusterLocalMessage ()
    {
    }

    void  FirmwareClusterLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
 		addSerializableAttribute (new AttributeUI32     (&m_state,              "state"));
 		addSerializableAttribute (new AttributeString   (&m_user,               "user"));
 		addSerializableAttribute (new AttributeString   (&m_password,           "password"));
 		addSerializableAttribute (new AttributeString   (&m_host,               "host"));
 		addSerializableAttribute (new AttributeString   (&m_path,               "path"));
 		addSerializableAttribute (new AttributeString   (&m_proto,              "proto"));
 		addSerializableAttribute (new AttributeUI32     (&m_flag,               "flag"));
 		addSerializableAttribute (new AttributeUI32     (&m_rbridge_id,         "rbridge_id"));
 		addSerializableAttribute (new AttributeSI32     (&m_local_node_ret,     "local_node_ret"));
 		addSerializableAttribute (new AttributeSI32     (&m_local_fwdl_status,  "local_fwdl_status"));
 		addSerializableAttribute (new AttributeString   (&m_mesg,               "mesg"));
		addSerializableAttribute (new AttributeString   (&m_firmwareVersion,    "firmwareVersion"));
    }

    void  FirmwareClusterLocalMessage::setState(const UI32 &state)
    {
        m_state  =  state;
    }

    void  FirmwareClusterLocalMessage::setUser(const string &user)
    {
        m_user  =  user;
    }

    void  FirmwareClusterLocalMessage::setPassword(const string &password)
    {
        m_password  =  password;
    }

    void  FirmwareClusterLocalMessage::setHost(const string &host)
    {
        m_host  =  host;
    }

    void  FirmwareClusterLocalMessage::setPath(const string &path)
    {
        m_path  =  path;
    }

    void  FirmwareClusterLocalMessage::setProto(const string &proto)
    {
        m_proto  =  proto;
    }

    void  FirmwareClusterLocalMessage::setFlag(const UI32 &flag)
    {
        m_flag  =  flag;
    }

    void  FirmwareClusterLocalMessage::setRbridgeId(const UI32 &rbridge_id)
    {
        m_rbridge_id  =  rbridge_id;
    }

	void FirmwareClusterLocalMessage::setLocalNodeRet(const int &local_node_ret)
	{
		m_local_node_ret = local_node_ret;
	}

	void FirmwareClusterLocalMessage::setLocalFwdlStatus(const int &local_fwdl_status)
	{
		m_local_fwdl_status = local_fwdl_status;
	}

	void FirmwareClusterLocalMessage::setLocalNodeMesg(const string &mesg)
	{
		m_mesg = mesg;
	}

	void FirmwareClusterLocalMessage::setFirmwareVersion(const string &firmwareVersion)
	{
		m_firmwareVersion = firmwareVersion;
	}

    UI32  FirmwareClusterLocalMessage::getState() const
    {
        return (m_state);
    }

    string  FirmwareClusterLocalMessage::getUser() const
    {
        return (m_user);
    }

    string  FirmwareClusterLocalMessage::getPassword() const
    {
        return (m_password);
    }

    string  FirmwareClusterLocalMessage::getHost() const
    {
        return (m_host);
    }

    string  FirmwareClusterLocalMessage::getPath() const
    {
        return (m_path);
    }

    string  FirmwareClusterLocalMessage::getProto() const
    {
        return (m_proto);
    }

    UI32  FirmwareClusterLocalMessage::getFlag() const
    {
        return (m_flag);
    }

    UI32  FirmwareClusterLocalMessage::getRbridgeId() const
    {
        return (m_rbridge_id);
    }

	int FirmwareClusterLocalMessage::getLocalNodeRet() const
	{
		return(m_local_node_ret);
	}

	int FirmwareClusterLocalMessage::getLocalFwdlStatus() const
	{
		return(m_local_fwdl_status);
	}

	string FirmwareClusterLocalMessage::getLocalNodeMesg() const
	{
		return (m_mesg);
	}

	string FirmwareClusterLocalMessage::getFirmwareVersion() const
	{
		return (m_firmwareVersion);
	}


    NetconfStatusMessage::NetconfStatusMessage ()
        : PrismMessage (FirmwareClusterLocalObjectManager::getPrismServiceId (), NETCONFSTATUS)
    {
    }

    NetconfStatusMessage::~NetconfStatusMessage ()
    {
    }

    void  NetconfStatusMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
 		addSerializableAttribute (new AttributeUI32         (&m_rbridge_id,     "rbridge_id"));
 		addSerializableAttribute (new AttributeSI32         (&m_fwdlState,      "fwdlState"));
 		addSerializableAttribute (new AttributeUI32         (&m_numOfEntries,   "numOfEntries"));
 		addSerializableAttribute (new AttributeUI32Vector   (&m_index,          "index"));
 		addSerializableAttribute (new AttributeUI32Vector   (&m_messageId,      "messageId"));
 		addSerializableAttribute (new AttributeStringVector (&m_dateAndTime,    "dateAndTime"));
 		addSerializableAttribute (new AttributeStringVector (&m_message,        "message"));
 		addSerializableAttribute (new AttributeStringVector (&m_bladeName,      "bladeName"));
    }

    void  NetconfStatusMessage::setRbridgeId(const UI32 &rbridge_id)
    {
        m_rbridge_id  =  rbridge_id;
    }

    void  NetconfStatusMessage::setFwdlState(const int &fwdlState)
    {
        m_fwdlState  =  fwdlState;
    }

    void  NetconfStatusMessage::setNumOfEntries(const UI32 &numOfEntries)
    {
        m_numOfEntries  =  numOfEntries;
    }

    void  NetconfStatusMessage::setIndex(const vector<UI32> &index)
    {
        m_index  =  index;
    }

    void  NetconfStatusMessage::setMessageId(const vector<UI32> &messageId)
    {
        m_messageId  =  messageId;
    }

    void  NetconfStatusMessage::setDateAndTime(const vector<string> &dateAndTime)
    {
        m_dateAndTime  =  dateAndTime;
    }

    void  NetconfStatusMessage::setMessage(const vector<string> &message)
    {
        m_message  =  message;
    }

    void  NetconfStatusMessage::setBladeName(const vector<string> &bladeName)
    {
        m_bladeName  =  bladeName;
    }

    UI32  NetconfStatusMessage::getRbridgeId() const
    {
        return (m_rbridge_id);
    }

    int  NetconfStatusMessage::getFwdlState() const
    {
        return (m_fwdlState);
    }

    UI32  NetconfStatusMessage::getNumOfEntries() const
    {
        return (m_numOfEntries);
    }

    vector<UI32>  NetconfStatusMessage::getIndex() const
    {
        return (m_index);
    }

    vector<UI32>  NetconfStatusMessage::getMessageId() const
    {
        return (m_messageId);
    }

    vector<string>  NetconfStatusMessage::getDateAndTime() const
    {
        return (m_dateAndTime);
    }

    vector<string>  NetconfStatusMessage::getMessage() const
    {
        return (m_message);
    }

    vector<string>  NetconfStatusMessage::getBladeName() const
    {
        return (m_bladeName);
    }
}
