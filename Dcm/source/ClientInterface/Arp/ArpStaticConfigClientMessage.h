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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#ifndef ARPSTATICCONFIGCLIENTMESSAGE_H
#define ARPSTATICCONFIGCLIENTMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpStaticConfigClientMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ArpStaticConfigClientMessage ();
            ArpStaticConfigClientMessage (const IpV4Address &IpAddress,const string &MacAdress,const UI32 &InterfaceType,const UI32 &PortChannelValue,const string &GigabitValue,const string &TenGigabitValue,const string &FortyGigabitValue,const UI32 &Vlanvalue,const bool &opCode);
            virtual    ~ArpStaticConfigClientMessage ();

            void setIpAddress(const IpV4Address &IpAddress);
            IpV4Address getIpAddress()  const;
            void setMacAdress(const string &MacAdress);
            string getMacAdress()  const;
            void setInterfaceType(const UI32 &InterfaceType);
            UI32 getInterfaceType()  const;
            void setPortChannelValue(const UI32 &PortChannelValue);
            UI32 getPortChannelValue()  const;
            void setGigabitValue(const string &GigabitValue);
            string getGigabitValue()  const;
            void setTenGigabitValue(const string &TenGigabitValue);
            string getTenGigabitValue()  const;
            void setFortyGigabitValue(const string &FortyGigabitValue);
            string getFortyGigabitValue()  const;
            void setVlanValue(const UI32 &VlanValue);
            UI32 getVlanValue()  const;
            void setOpCode(const bool &opCode);
            bool getOpCode()  const;

            string getTwoTouple (string ifName);

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            IpV4Address  m_IpAddress;
            string  m_MacAdress;
            UI32  m_InterfaceType;
			UI32 m_PortChannelValue;
            string  m_GigabitValue;
            string  m_TenGigabitValue;
            string  m_FortyGigabitValue;
			UI32    m_VlanValue;
            bool  m_opCode;
    };
}
#endif                                            //ARPSTATICCONFIGCLIENTMESSAGE_H

