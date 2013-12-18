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
 *   Author : nsong, sholla                                                *
 **************************************************************************/

#ifndef NSMPORTSTPCONFIGMESSAGE_H
#define NSMPORTSTPCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs
{

	class NsmPortStpConfigMessage : public ManagementInterfaceMessage
	{
		public:
			NsmPortStpConfigMessage(const string &targetSvc, UI32 opCode);
			NsmPortStpConfigMessage(const string &targetSvc, UI32 opCode, UI32 cmdCode, const InterfaceType &ifType, const string &ifName, UI32 cost=0, UI32 priority=0);
			virtual ~NsmPortStpConfigMessage();

			virtual void setInterfaceInfo(const InterfaceType &ifType, const string &ifName);
			virtual void setPriority(const UI32 &priority);
                        virtual void setCost(const UI32 &cost);
                        virtual void setCmdCode(const UI32 &cmdCode);
                        virtual InterfaceType getIfType() const;
                        virtual string getIfName() const;
			virtual	UI32 getCost() const;
			virtual	UI32 getPriority() const;
			virtual	UI32 getCmdCode() const;
                        virtual bool getIsNoCmd() const;
                        virtual void setIsNoCmd(const bool &isNoCmd);

		protected:
			virtual void setupAttributesForSerialization();


		public:
			UI32            m_cmdCode;
			InterfaceType   m_ifType;
			string          m_ifName;
			UI32            m_cost;
			UI32            m_priority;
                        bool            m_isNoCmd;

	};

}

#endif	// NSMPORTSTPCONFIGMESSAGE_H

