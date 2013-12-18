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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#ifndef DHCPRELCLIENTSGINTFLOCALCONFIGMESSAGE_H
#define DHCPRELCLIENTSGINTFLOCALCONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h" 

using namespace WaveNs;

namespace DcmNs
{

class DhcpRelIntfClientConfigMessage : public DcmManagementInterfaceMessage
{
	// Class methods
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        DhcpRelIntfClientConfigMessage();
        DhcpRelIntfClientConfigMessage (const IpV4Address &serverAddress, const string &ifName, const UI32 &ifType, const UI32 &vlanValue, const bool &negation);
        virtual    ~DhcpRelIntfClientConfigMessage ();
        void setServerAddress(const IpV4Address &serverAddress);
        IpV4Address getServerAddress()  const;
        void setIfName(const string &ifName);
        string getIfName()  const;
        void setIfType(const UI32 &ifType);
        UI32 getIfType()  const;
        void setVlanValue(const UI32 &vlanValue);
        UI32 getVlanValue()  const;
        void setNegation(const bool &negation);
        bool getNegation()  const;
        virtual const void *getCStructureForInputs();
        virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);

	// Class data members
    private:
    protected:
    public:
        IpV4Address  m_serverAddress;
        string  m_ifName;
        UI32  m_ifType;
        UI32  m_vlanValue;
        bool  m_negation;
};
}
#endif                                        //DHCPRELCLIENTSGINTFLOCALCONFIGMESSAGE_H
