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

#ifndef FIRMWARESTATUSMESSAGE_H
#define FIRMWARESTATUSMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class FirmwareStatusMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			FirmwareStatusMessage();
			virtual ~FirmwareStatusMessage();
			void setRbridgeIdVector(const vector<UI32> &rbridge_id);
			void setRbridgeId(const UI32 &rbridge_id);
			void setFwdlState(const int &fwdlState);
			void setNumOfEntries(const UI32 &numOfEntries);
			void setIndex(const vector<UI32> &index);
			void setMessageId(const vector<UI32> &messageId);
			void setDateAndTime(const vector<string> &dateAndTime);
			void setMessage(const vector<string> &message);
			void setBladeName(const vector<string> &bladeName);
			vector<UI32> getRbridgeId() const;
			vector<int> getFwdlState() const;
			vector<UI32> getNumOfEntries() const;
			vector<vector<UI32> > getIndex() const;
			vector<vector<UI32> > getMessageId() const;
			vector<vector<string> > getDateAndTime() const;
			vector<vector<string> > getMessage() const;
			vector<vector<string> > getBladeName() const;

		// Now the data members
		private:
		protected:
		public:
			vector<UI32>				m_rbridge_id;
			vector<int>					m_fwdlState;
			vector<UI32>				m_numOfEntries;
			vector<vector<UI32> >		m_index;
			vector<vector<UI32> >		m_messageId;
			vector<vector<string> >		m_dateAndTime;
			vector<vector<string> >		m_message;
			vector<vector<string> >		m_bladeName;
	};
}
#endif		//FIRMWARECLUSTERLOCALMESSAGE_H
