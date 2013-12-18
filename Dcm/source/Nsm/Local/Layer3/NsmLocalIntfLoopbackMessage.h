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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Justin Huang                                                 *
 **************************************************************************/

#ifndef NSMLOCALINTFLOOPBACKMESSAGE_H
#define NSMLOCALINTFLOOPBACKMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
//#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"

//#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalIntfLoopbackMessage : public PrismMessage
    {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NsmLocalIntfLoopbackMessage ();
			NsmLocalIntfLoopbackMessage (UI32 id);
			~NsmLocalIntfLoopbackMessage ();

            void setOpCode(const UI32 &op);
            UI32 getOpCode() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

            void setRbridge(const string &name);
            string getRbridge() const;
            void setIfId(const UI32 &id);
            UI32 getIfId() const;
            void setIfName(const string &name);
            string getIfName() const;
			void setStatus(const bool &flag);
			bool getStatus() const;
            void setIpv4Addr(const string &addr);
            string getIpv4Addr() const;
            void setVrfName(const string &name);
            string getVrfName() const;

			// Now the data members
		private:
		protected:
		public:
            UI32 m_cmdCode;
            UI32 m_opCode;

            string m_rbridge;
            UI32 m_ifId;
            string m_ifName;
			bool  m_status;
            string m_ipv4Addr;
            string m_vrfName;

	};
}
#endif //NSMLOCALINTFLOOPBACKMESSAGE_H

