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

#ifndef MACGROUPCONFIGLOCALMESSAGE_H
#define MACGROUPCONFIGLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
	class MacGroupConfigLocalMessage : public PrismMessage
	{
		private :
			void setupAttributesForSerialization ();

		protected :
		public :
			MacGroupConfigLocalMessage ();
			virtual ~MacGroupConfigLocalMessage ();
			UI32 getCmdCode() const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getMacGroupId() const;
			void setMacGroupId(const UI32 &macGroupId);
			void setMac(const string &mac);
			string getMac() const;
			void setMask(const string &mask);
			string getMask() const;
		private:


		// Now the data members


		private :
			UI32 m_cmdCode;
			UI32 m_macGroupId;
			string m_mac;
			string m_mask;

		protected :
		public :
	};
}
#endif		//MACGROUPCONFIGLOCALMESSAGE_H


