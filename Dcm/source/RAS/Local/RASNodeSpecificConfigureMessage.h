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
 *   Author : bkesanam                                                     *
 **************************************************************************/

#ifndef RASNODESPECIFICCONFIGUREMESSAGE_H
#define RASNODESPECIFICCONFIGUREMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "RASTypes.h"
#include "ClientInterface/RAS/RASApp.h"
#include "ClientInterface/RAS/RASClient.h"

#include <raslog/raslogd.h>
#include <raslog/raslogm.h>
#include <utils/support.h>

using namespace WaveNs;

namespace DcmNs
{

/////////////////////////////////////////////////
//  RASNodeSpecificConfigureMessage definition //
/////////////////////////////////////////////////

class RASNodeSpecificConfigureMessage : public PrismMessage
{
	private:
		void setupAttributesForSerialization();
	protected:
	public:
		RASNodeSpecificConfigureMessage ();
		RASNodeSpecificConfigureMessage (const ObjectId &LoggingOId, const string &data, UI32 &userWyserTag);

		virtual    ~RASNodeSpecificConfigureMessage ();
		ObjectId  getObjectId() const;
		void setWyserUserTag(const UI32 &userWyserTag);
		UI32 getWyserUserTag() const;
		void setData(const string &Data);
		string getData() const;
		void setCmd(const string &cmd);
		string getCmd() const;
		void setMappedId(const UI32 &mappedId);
		UI32 getMappedId() const;
		string getCLIStart() const;
		void setCLIStart(const string &msg);
		string getCLIEnd() const;
		void setCLIEnd(const string &msg);
		string getCLIError() const;
		void setCLIError(const string &msg);
		bool getCLIOutput() const;
		void setCLIOutput(const bool &CLIOutput);

	// Now the data members

	private:
		ObjectId m_ObjectId;
		UI32    m_mappedId;
		UI32    m_userWyserTag;
		string  m_data;
		string  m_cmd;
		string  m_cliStart;
		string  m_cliEnd;
		string  m_cliError;
		bool   m_cliOutput;
	protected:
	public:

};

} // namespace DcmNs

#endif
