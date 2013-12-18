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
 *   Author : jleelakr													   *
 **************************************************************************/

#ifndef RASSUPPORTMESSAGE_H
#define RASSUPPORTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "RASTypes.h"
#include "RAS/Global/RASObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

//////////////////////////////////////
//  RASSupportMessage definition    //
//////////////////////////////////////

class RASSupportMessage : public PrismMessage
{
	private:
		void setupAttributesForSerialization();
	protected:
	public:
		RASSupportMessage ();
		RASSupportMessage (const ObjectId &SupportOId, const bool &data);

		virtual    ~RASSupportMessage ();
		ObjectId  getObjectId() const;
		void setData(const string &Data);
		string getData() const;
		void setEnumData(const Severity &enumData);
		Severity getEnumData() const;
		void setBoolData(const bool &Data);
		bool getBoolData() const;
		void setWyserUserTag(const UI32 &userWyserTag);
		UI32 getWyserUserTag() const;
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
		bool  getNetConf() const;
		void  setNetConf(const bool &);
		bool getSync() const;
		void setSync(const bool &boolData);
		void setOrigMappedId(const UI32 &mappedId);
		void setCLITimeout(const UI32 &CLITimeout);
					
		// Now the data members

	private:
		ObjectId m_ObjectId;
		UI32    m_mappedId;
		UI32    m_userWyserTag;
		Severity    m_enumData;
		string  m_data;
		bool    m_boolData;
		string  m_cmd;
		string  m_cliStart;
		string  m_cliEnd;
		string  m_cliError;
		bool   m_cliOutput;

		bool    m_netconf;
		bool m_sync;
		UI32    m_cliTimeout;
		UI32   m_origMappedId;

	protected:
	public:

};
} // namespace DcmNs

#endif



