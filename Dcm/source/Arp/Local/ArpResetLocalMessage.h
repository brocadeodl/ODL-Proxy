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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#ifndef ARPRESETLOCALMESSAGE_H
#define ARPRESETLOCALMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpResetLocalMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ArpResetLocalMessage ();
            ArpResetLocalMessage (const IpV4Address &IpAddress,const string &MacAdress,const UI32 &InterfaceType,const string &InterfaceValue,const UI32 &SlotNumber,const UI32 &CommandCode);
            virtual    ~ArpResetLocalMessage ();
            void setIpAddress(const IpV4Address &IpAddress);
            IpV4Address getIpAddress()  const;
            void setMacAdress(const string &MacAdress);
            string getMacAdress()  const;
            void setInterfaceType(const UI32 &InterfaceType);
            UI32 getInterfaceType()  const;
            void setInterfaceValue(const string &InterfaceValue);
            string getInterfaceValue()  const;
            void setSlotNumber(const UI32 &SlotNumber);
            UI32 getSlotNumber()  const;
            void setCommandCode(const UI32 &CommandCode);
            UI32 getCommandCode()  const;

// Now the data members

        private:
        protected:
        public:
            IpV4Address  m_IpAddress;
            string  m_MacAdress;
            UI32  m_InterfaceType;
            string  m_InterfaceValue;
            UI32  m_SlotNumber;
            UI32  m_CommandCode;
    };
}
#endif                                            //ARPRESETLOCALMESSAGE_H
