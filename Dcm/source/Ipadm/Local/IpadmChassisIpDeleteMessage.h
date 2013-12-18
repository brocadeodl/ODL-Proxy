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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.		  *
 *   All rights reserved.												  *
 *   Author : cshah													 *
 **************************************************************************/

#ifndef IPADMCHASSISIPDELETEVIPMESSAGE_H
#define IPADMCHASSISIPDELETEVIPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class IpadmChassisIpDeleteMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			IpadmChassisIpDeleteMessage ();
			IpadmChassisIpDeleteMessage (const UI32 &VirtualIpTag, const IpadmAddressType &IpAddrType);
			virtual	~IpadmChassisIpDeleteMessage ();
			void setVirtualIpTag(const UI32 &VirtualIpTag);
			UI32 getVirtualIpTag()  const;
                        void setIpAddrFlag(const IpadmAddressType &IpAddrFlag);
                        IpadmAddressType getIpAddrFlag() const;

// Now the data members

		private:
		protected:
		public:
			UI32  m_VirtualIpTag;
			IpadmAddressType m_AddrType;
	};
}
#endif											//IPADMCHASSISIPDELETEVIPMESSAGE_H
