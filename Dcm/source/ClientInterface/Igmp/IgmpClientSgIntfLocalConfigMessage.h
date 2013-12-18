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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Abhay Gidwani                                                     *
 **************************************************************************/

#ifndef IGMPCLIENTSGINTFLOCALCONFIGMESSAGE_H
#define IGMPCLIENTSGINTFLOCALCONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h" 

using namespace WaveNs;

namespace DcmNs
{

class IgmpClientSgIntfLocalConfigMessage : public DcmManagementInterfaceMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        IgmpClientSgIntfLocalConfigMessage();
        IgmpClientSgIntfLocalConfigMessage (const IpV4Address &groupAddress, const string &ifName, const UI32 &ifType, const bool &negation);
        virtual    ~IgmpClientSgIntfLocalConfigMessage ();
        void setGroupAddress(const IpV4Address &groupAddress);
        IpV4Address getGroupAddress()  const;
        void setIfName(const string &ifName);
        string getIfName()  const;
        void setIfType(const UI32 &opCode);
        UI32 getIfType()  const;
        void setNegation(const bool &negation);
        bool getNegation()  const;
        virtual const void *getCStructureForInputs();
        virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);

// Now the data members

    private:
    protected:
    public:
        IpV4Address  m_groupAddress;
        string  m_ifName;
        UI32  m_ifType;
        bool  m_negation;
};
}
#endif                                        //IGMPCLIENTSGINTFLOCALCONFIGMESSAGE_H
