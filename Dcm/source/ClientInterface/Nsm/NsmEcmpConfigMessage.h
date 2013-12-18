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

#ifndef NSMECMPCONFIGMESSAGE_H
#define NSMECMPCONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmEcmpConfigMessage : public DcmManagementInterfaceMessage
	{
		private:
			virtual void setupAttributesForSerialization();
		protected:
		public:
			NsmEcmpConfigMessage ();
			NsmEcmpConfigMessage (const UI32 &loadBalanceMode ,const UI32 &cmdCode,const UI32 &hashSwap);
			virtual    ~NsmEcmpConfigMessage ();
			void setLoadBalanceMode(const UI32 &loadBalanceMode);
			UI32 getLoadBalanceMode()  const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode() const;
			void setHashSwap(const UI32 &hashSwap);
			UI32 getHashSwap() const;
			virtual const void *getCStructureForInputs    ();
			virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

			// Now the data members
		private:
		protected:
		public:
			UI32  m_loadBalanceMode;
			UI32  m_cmdCode;
			UI32  m_hashSwap;
	};
}
#endif                                            //NSMECMPCONFIGMESSAGE_H

