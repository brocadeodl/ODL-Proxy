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

#include "RAS/Local/RASLocalObjectManager.h"
#include "RAS/Local/RASSupportMessage.h"
#include "RAS/Local/RASTypes.h"
//#include "Framework/Utils/TraceUtils.h"
//#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeEnum.h"
//#include "Framework/Attributes/AttributeIpVxAddress.h"

#include "vcs.h"
//#include "raslog/raslog.h"
//#include "brocade-RAS-show.h"

namespace DcmNs
{

	//////////////////////////////////////////////////////////////
	//  RASSupportMessage Implementation  - BEGIN               //
	
	RASSupportMessage::RASSupportMessage ()
		: PrismMessage (SSLocalObjectManager::getPrismServiceId (), SUPPORTSAVE),
		m_cliStart(""), m_cliEnd(""), m_cliError(""),
		m_cliOutput(false), m_netconf(false), m_sync(true)
		{
			// trace (TRACE_LEVEL_DEBUG, string (" RASSupportMessage::RASSupportMessage()"));
		}

	RASSupportMessage::RASSupportMessage (const ObjectId &SupportOID, const bool &data)
		: PrismMessage (SSLocalObjectManager::getPrismServiceId (), SUPPORTSAVE),
		m_ObjectId(SupportOID), m_cliStart(""), m_cliEnd(""), m_cliError(""),
		m_cliOutput(false), m_netconf(false), m_sync(true)
		{
			// trace (TRACE_LEVEL_DEBUG, string (" RASSupportMessage::RASSupportMessage()"));
		}

	RASSupportMessage::~RASSupportMessage ()
	{
	}

	// Prepare message for transmission over the network
	// any data which referenced by remote node need addSerializableAttribute here
	void  RASSupportMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
 		addSerializableAttribute (new AttributeObjectId (       &m_ObjectId,    "ObjectId"));
		addSerializableAttribute (new AttributeString   (       &m_data,        "data"));
 		addSerializableAttribute (new AttributeBool     (       &m_boolData,    "boolData"));
		addSerializableAttribute (new AttributeEnum     ((UI32*)(&m_enumData),  "enumData"));
 		addSerializableAttribute (new AttributeUI32     (       &m_userWyserTag, "userWyserTag"));
		addSerializableAttribute (new AttributeString   (       &m_cmd,         "cmd"));
 		addSerializableAttribute (new AttributeUI32     (       &m_mappedId,    "mappedId"));
 		addSerializableAttribute (new AttributeBool     (       &m_cliOutput,   "cliOutput"));
 		addSerializableAttribute (new AttributeBool     (       &m_netconf,     "netconf"));
 		addSerializableAttribute (new AttributeBool     (       &m_sync,        "sync"));
	}

	ObjectId RASSupportMessage::getObjectId () const
	{
		return (m_ObjectId);
	}

	string  RASSupportMessage::getData() const
	{
		return (m_data);
	}

	void  RASSupportMessage::setData(const string &Data)
	{
		m_data = Data;
	}

	bool  RASSupportMessage::getBoolData() const
	{
		return (m_boolData);
	}

	void  RASSupportMessage::setBoolData(const bool &boolData)
	{
		m_boolData = boolData;
	}

	Severity  RASSupportMessage::getEnumData() const
	{
		return (m_enumData);
	}

	void  RASSupportMessage::setEnumData(const Severity &enumData)
	{
		m_enumData = enumData;
	}

	UI32 RASSupportMessage::getWyserUserTag() const
	{
		return (m_userWyserTag);
	}
	void  RASSupportMessage::setWyserUserTag(const  UI32 &userWyserTag)
	{
		m_userWyserTag = userWyserTag;
	}

	string  RASSupportMessage::getCmd() const
	{
		return (m_cmd);
	}

	void  RASSupportMessage::setCmd(const string &cmd)
	{
		m_cmd = cmd;
	}

	UI32 RASSupportMessage::getMappedId() const
	{
		return (m_mappedId);
	}

	void  RASSupportMessage::setMappedId(const  UI32 &mappedId)
	{
		m_mappedId = mappedId;
	}

	string RASSupportMessage::getCLIStart() const
	{
		return (m_cliStart);
	}

	void  RASSupportMessage::setCLIStart(const  string &cliStart)
	{
		m_cliStart = cliStart;
	}

	string RASSupportMessage::getCLIEnd() const
	{
		return (m_cliEnd);
	}

	void  RASSupportMessage::setCLIEnd(const  string &cliEnd)
	{
		m_cliEnd = cliEnd;
	}

	string RASSupportMessage::getCLIError() const
	{
		return (m_cliError);
	}

	void  RASSupportMessage::setCLIError(const  string &cliError)
	{
		m_cliError = cliError;
	}

	bool  RASSupportMessage::getCLIOutput() const
	{
		return (m_cliOutput);
	}

	void  RASSupportMessage::setCLIOutput(const bool &boolData)
	{
		m_cliOutput = boolData;
	}

	bool  RASSupportMessage::getNetConf() const
	{
		return (m_netconf);
	}

	void  RASSupportMessage::setNetConf(const bool &boolData)
	{
		m_netconf = boolData;
	}

	bool RASSupportMessage::getSync() const
	{
		return (m_sync);
	}

	void RASSupportMessage::setSync(const bool &boolData)
	{
		m_sync = boolData;
	}
	void   RASSupportMessage::setCLITimeout(const UI32 &CLITimeout)
	{ 
        m_cliTimeout = CLITimeout;
	}
	void   RASSupportMessage::setOrigMappedId(const  UI32 &mappedId)
	{ 
    	m_origMappedId = mappedId;
	}
}

