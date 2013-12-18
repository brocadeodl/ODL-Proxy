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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef AAAGLOBALGETLOCKEDUSERSMESSAGE_H
#define AAAGLOBALGETLOCKEDUSERSMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class AAAGlobalGetLockedUsersMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			AAAGlobalGetLockedUsersMessage ();
			AAAGlobalGetLockedUsersMessage (const vector<string> &userName,const vector<UI32> &switchid);
			virtual    ~AAAGlobalGetLockedUsersMessage ();
			UI32 getNumVectors() const;
			void clearNumVectors();
			void setLockedUser(const string &userName, const UI32 &switchid);
			bool getLockedUser(const UI32 &index, string &userName, UI32 &switchid);

// Now the data members

		private:
		protected:
		public:
			vector<string>  m_vUserName;
			vector<UI32>  m_vSwitchid;
	};
}
#endif     //AAAGLOBALGETLOCKEDUSERSMESSAGE_H
