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
 *   Author : Amulya Makam                                                 *
 **************************************************************************/

#ifndef NSMLOCALVLAGCONFIGMESSAGE_H
#define NSMLOCALVLAGCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmLocalVlagConfigMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NsmLocalVlagConfigMessage ();
			NsmLocalVlagConfigMessage (const UI32 &poId,const UI32 &mode,
				const UI32 &cmdCode);
			virtual    ~NsmLocalVlagConfigMessage ();
			void setPoId(const UI32 &poId);
			UI32 getPoId()  const;
			void setLoadBalanceMode(const UI32 &mode);
			UI32 getLoadBalanceMode()  const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode()  const;

			// Now the data members

		private:
		protected:
		public:
			UI32  m_poId;
			UI32  m_mode;
			UI32  m_cmdCode;
	};
}
#endif                                            //NSMLOCALVLAGCONFIGMESSAGE_H

