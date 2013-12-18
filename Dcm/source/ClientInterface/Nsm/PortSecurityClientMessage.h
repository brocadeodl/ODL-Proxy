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
 *   Author : skrastog                                                     *
 **************************************************************************/

#ifndef PORTSECURITYCLIENTMESSAGE_H
#define PORTSECURITYCLIENTMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortSecurityClientMessage : public DcmManagementInterfaceMessage
    {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			PortSecurityClientMessage ();
			PortSecurityClientMessage (const string &ifName, const UI32 &numAllowedMacs, const UI32 &action, const string &mac, const UI32 &opCode);
			virtual    ~PortSecurityClientMessage ();
			void setIfName(const string &ifName);
			string getIfName()  const;
			void setIfType(const UI32 &ifType);
			UI32 getIfType()  const;
			void setNumAllowedMacs(const UI32 &numAllowedMacs);
			UI32 getNumAllowedMacs()  const;
			void setAction(const UI32 &action);
			UI32 getAction()  const;
			void setAllowedMac(const string &mac);
			string getAllowedMac()  const;
			void setOui(const string &oui);
			string getOui()  const;
			void setOpCode(const UI32 &opCode);
			UI32 getOpCode() const;
			void setSticky(const bool &sticky);
			bool getSticky() const;
			void getStickyMacRecordsOutputBuffer(UI32 bufferNum, UI32 &size, void *&pBuffer);
			const void* getCStructureForInputs();
			void loadOutputsFromCStructure (const void *pOutputCStructure);
			void setShutdownTime(const UI32 &time);
			UI32 getShutdownTime() const;
			void setVlanId(const UI32 &vlanId);
			UI32 getVlanId() const;
			// Now the data members

		private:
		protected:
		public:
			string  m_ifName;
			UI32 m_ifType;
			UI32  m_numAllowedMacs;
			UI32  m_action;
			string  m_allowedMac;
			string  m_oui;
			UI32 m_opCode;
			bool m_sticky;
			UI32 m_shutdownTime;
			UI32 m_vlanId;
	};
}
#endif                                            //PORTSECURITYCLIENTMESSAGE_H
