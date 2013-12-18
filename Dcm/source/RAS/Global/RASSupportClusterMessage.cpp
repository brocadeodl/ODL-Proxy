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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : jleelakr 
 **************************************************************************/

#include "RAS/Global/RASObjectManager.h"
#include "RAS/Global/RASSupportClusterMessage.h"
#include "RAS/Global/RASObjectManagerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    //////////////////////////////////////////////////////////////
    //  RASSupportClusterMessage Implementation  - BEGIN               //

    RASSupportClusterMessage::RASSupportClusterMessage ()
        : ManagementInterfaceMessage (SSObjectManager::getClassName (), RAS_COPY_SUPPORT),
        m_cliStart(""), m_cliEnd(""), m_cliError(""),
        m_cliOutput(false), m_netconf(false), m_sync(true), m_rbridgeIds(0)
        {
            trace (TRACE_LEVEL_DEBUG, string (" RASSupportClusterMessage::RASSupportClusterMessage()"));
        }

    RASSupportClusterMessage::~RASSupportClusterMessage ()
    {
    }

	void  RASSupportClusterMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString   (       &m_data,        "data"));
        addSerializableAttribute (new AttributeBool     (       &m_boolData,    "boolData"));
        addSerializableAttribute (new AttributeEnum     ((UI32*)(&m_enumData),  "enumData"));
        addSerializableAttribute (new AttributeUI32     (       &m_userWyserTag, "userWyserTag"));
        addSerializableAttribute (new AttributeString   (       &m_cmd,         "cmd"));
        addSerializableAttribute (new AttributeUI32     (       &m_mappedId,    "mappedId"));
        addSerializableAttribute (new AttributeBool     (       &m_cliOutput,   "cliOutput"));
        addSerializableAttribute (new AttributeBool     (       &m_netconf,     "netconf"));
        addSerializableAttribute (new AttributeBool     (       &m_sync,        "sync"));
		addSerializableAttribute (new AttributeUI32Vector	(		&m_rbridgeIds,	"rbridgeIds"));
    }

    string  RASSupportClusterMessage::getData() const
    {
        return (m_data);
    }

    void  RASSupportClusterMessage::setData(const string &Data)
    {
        m_data = Data;
    }

    bool  RASSupportClusterMessage::getBoolData() const
    {
        return (m_boolData);
    }

    void  RASSupportClusterMessage::setBoolData(const bool &boolData)
    {
        m_boolData = boolData;
    }

    Severity  RASSupportClusterMessage::getEnumData() const
    {
        return (m_enumData);
    }

    void  RASSupportClusterMessage::setEnumData(const Severity &enumData)
    {
        m_enumData = enumData;
    }

	UI32 RASSupportClusterMessage::getWyserUserTag() const
    {
        return (m_userWyserTag);
    }
    void  RASSupportClusterMessage::setWyserUserTag(const  UI32 &userWyserTag)
    {
        m_userWyserTag = userWyserTag;
    }

    string  RASSupportClusterMessage::getCmd() const
    {
        return (m_cmd);
    }

    void  RASSupportClusterMessage::setCmd(const string &cmd)
    {
        m_cmd = cmd;
    }

    UI32 RASSupportClusterMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void  RASSupportClusterMessage::setMappedId(const  UI32 &mappedId)
    {
        m_mappedId = mappedId;
    }

    string RASSupportClusterMessage::getCLIStart() const
    {
        return (m_cliStart);
    }

	void  RASSupportClusterMessage::setCLIStart(const  string &cliStart)
    {
        m_cliStart = cliStart;
    }

    string RASSupportClusterMessage::getCLIEnd() const
    {
        return (m_cliEnd);
    }

    void  RASSupportClusterMessage::setCLIEnd(const  string &cliEnd)
    {
        m_cliEnd = cliEnd;
    }

    string RASSupportClusterMessage::getCLIError() const
    {
        return (m_cliError);
    }

    void  RASSupportClusterMessage::setCLIError(const  string &cliError)
    {
        m_cliError = cliError;
    }

    bool  RASSupportClusterMessage::getCLIOutput() const
    {
        return (m_cliOutput);
    }

    void  RASSupportClusterMessage::setCLIOutput(const bool &boolData)
    {
        m_cliOutput = boolData;
    }

	bool  RASSupportClusterMessage::getNetConf() const
    {
        return (m_netconf);
    }

    void  RASSupportClusterMessage::setNetConf(const bool &boolData)
    {
        m_netconf = boolData;
    }

    bool RASSupportClusterMessage::getSync() const
    {
        return (m_sync);
    }

    void RASSupportClusterMessage::setSync(const bool &boolData)
    {
        m_sync = boolData;
    }

    void   RASSupportClusterMessage::setCLITimeout(const UI32 &CLITimeout)
    {
        m_cliTimeout = CLITimeout;
    }

	UI32  RASSupportClusterMessage::getCLITimeout() const
	{
		return (m_cliTimeout);
	}

    void   RASSupportClusterMessage::setOrigMappedId(const  UI32 &mappedId)
    {
        m_origMappedId = mappedId;
    }

	UI32  RASSupportClusterMessage::getOrigMappedId() const
	{
	return (m_origMappedId);
	}

	void RASSupportClusterMessage::setRbridgeIds(const vector<UI32> &rbridgeIds)
	{
		m_rbridgeIds  =  rbridgeIds;
	}

	vector<UI32> RASSupportClusterMessage::getRbridgeIds() const
	{
		return (m_rbridgeIds);
	}
}

