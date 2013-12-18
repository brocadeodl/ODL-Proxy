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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : Venkat														*
 **************************************************************************/

#ifndef SWITCHPORTVLANCONFIGLOCALMESSAGE_H
#define SWITCHPORTVLANCONFIGLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
	class SwitchportVlanConfigLocalMessage : public PrismMessage
	{
		private :
			void setupAttributesForSerialization ();

		protected :
		public :
			SwitchportVlanConfigLocalMessage ();
			virtual ~SwitchportVlanConfigLocalMessage ();
			UI32 getCmdCode() const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getVlanId() const;
			void setVlanId(const UI32 &vlanId);
			UI32 getCtagId() const;
			void setCtagId(const UI32 &ctagId);
			string getMac() const;
			void setMac(const string &mac);
			string getIfName() const;
			void setIfName(const string &ifName);
			UI32 getMacGroupId() const;
			void setMacGroupId(const UI32 &macGroupId);
		private:


		// Now the data members


		private :
			UI32 m_cmdCode;
			UI32 m_vlanId;
			UI32 m_ctagId;
			string m_mac;
			string m_ifName;
			UI32 m_macGroupId;

		protected :
		public :
	};
}
#endif //SWITCHPORTVLANCONFIGLOCALMESSAGE_H

