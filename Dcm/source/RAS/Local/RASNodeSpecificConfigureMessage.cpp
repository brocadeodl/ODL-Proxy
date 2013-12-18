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
 *   Author : chenj                                                     *
 **************************************************************************/

#include "RAS/Local/RASNodeSpecificLocalObjectManager.h"
#include "RAS/Local/RASNodeSpecificConfigureMessage.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "RAS/Local/RASLocalObjectManager.h"
#include "RAS/Local/RASTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/AttributeEnum.h"

#include "vcs.h"
#include "raslog/raslog.h"

namespace DcmNs
{

////////////////////////////////////////////////////////////////////////////
//  RASNodeSpecificConfigureMessage Implementation  - BEGIN               //

// Every MSG O will be use in different inquiry
RASNodeSpecificConfigureMessage::RASNodeSpecificConfigureMessage ()
	: PrismMessage (RASLocalObjectManager::getPrismServiceId (), RAS_NODE_SPECIFIC_LOGGING),
	m_cmd(""),
	m_cliStart(""), m_cliEnd(""), m_cliError(""),
	m_cliOutput(true)
{
	trace (TRACE_LEVEL_INFO, string (" RASNodeSpecificConfigureMessage::RASNodeSpecificConfigureMessage()"));
}

RASNodeSpecificConfigureMessage::~RASNodeSpecificConfigureMessage ()
{
}
// config RAS string data type
RASNodeSpecificConfigureMessage::RASNodeSpecificConfigureMessage (const ObjectId &LoggingOID, const string &data, UI32 &userWyserTag)
	: PrismMessage (RASLocalObjectManager::getPrismServiceId (), RAS_NODE_SPECIFIC_LOGGING),
	m_ObjectId(LoggingOID), m_userWyserTag(userWyserTag), m_data(data),
	m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(true)
{
	trace (TRACE_LEVEL_INFO, string (" RASNodeSpecificConfigureMessage::RASNodeSpecificConfigureMessage()"));
}

// Prepare message for transmission over the network
// any data which referenced by remote node need addSerializableAttribute here
void  RASNodeSpecificConfigureMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
 	addSerializableAttribute (new AttributeObjectId (&m_ObjectId,       "ObjectId"));
 	addSerializableAttribute (new AttributeUI32     (&m_userWyserTag,    "userWyserTag"));
	addSerializableAttribute (new AttributeString   (&m_cmd,            "cmd"));
 	addSerializableAttribute (new AttributeUI32     (&m_mappedId,       "mappedId"));
 	addSerializableAttribute (new AttributeBool     (&m_cliOutput,      "cliOutput"));
}

ObjectId RASNodeSpecificConfigureMessage::getObjectId () const
{
	return (m_ObjectId);
}

string  RASNodeSpecificConfigureMessage::getData() const
{
	return (m_data);
}

void  RASNodeSpecificConfigureMessage::setData(const string &data)
{
	m_data = data;
}


UI32 RASNodeSpecificConfigureMessage::getWyserUserTag() const
{
	return (m_userWyserTag);
}
void  RASNodeSpecificConfigureMessage::setWyserUserTag(const  UI32 &userWyserTag)
{
	m_userWyserTag = userWyserTag;
}
string  RASNodeSpecificConfigureMessage::getCmd() const
{
	return (m_cmd);
}
void  RASNodeSpecificConfigureMessage::setCmd(const string &cmd)
{
	m_cmd = cmd;
}

UI32 RASNodeSpecificConfigureMessage::getMappedId() const
{
	return (m_mappedId);
}
void  RASNodeSpecificConfigureMessage::setMappedId(const  UI32 &mappedId)
{
	m_mappedId = mappedId;
}

string RASNodeSpecificConfigureMessage::getCLIStart() const
{
	return (m_cliStart);
}
void  RASNodeSpecificConfigureMessage::setCLIStart(const  string &cliStart)
{
	m_cliStart = cliStart;
}
string RASNodeSpecificConfigureMessage::getCLIEnd() const
{
	return (m_cliEnd);
}
void  RASNodeSpecificConfigureMessage::setCLIEnd(const  string &cliEnd)
{
	m_cliEnd = cliEnd;
}
string RASNodeSpecificConfigureMessage::getCLIError() const
{
	return (m_cliError);
}
void  RASNodeSpecificConfigureMessage::setCLIError(const  string &cliError)
{
	m_cliError = cliError;
}
bool  RASNodeSpecificConfigureMessage::getCLIOutput() const
{
	return (m_cliOutput);
}
void  RASNodeSpecificConfigureMessage::setCLIOutput(const bool &boolData)
{
	m_cliOutput = boolData;
}


//  RASNodeSpecificConfigureMessage Implementation  - END    //
//////////////////////////////////////////////////////////////

} // namespace DcmNs
