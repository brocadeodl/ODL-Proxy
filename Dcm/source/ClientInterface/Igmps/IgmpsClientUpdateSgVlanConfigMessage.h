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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef IGMPSCLIENTUPDATESGVLANCONFIGMESSAGE_H
#define IGMPSCLIENTUPDATESGVLANCONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h" 

using namespace WaveNs;

namespace DcmNs
{

    class IgmpsClientUpdateSgVlanConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IgmpsClientUpdateSgVlanConfigMessage ();
            IgmpsClientUpdateSgVlanConfigMessage (const IpV4Address &groupAddress,const UI32 &interface,const UI32 &ifType,const string &ifName,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const bool &negation);
            virtual    ~IgmpsClientUpdateSgVlanConfigMessage ();
            void setGroupAddress(const IpV4Address &groupAddress);
            IpV4Address getGroupAddress()  const;
            void setInterface(const UI32 &interface);
            UI32 getInterface()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;
            void setVlanIfName(const string &vlanIfName);
            string getVlanIfName()  const;
            void setVlanIfIndex(const UI64 &vlanIfIndex);
            UI64 getVlanIfIndex()  const;
            void setNegation(const bool &negation);
            bool getNegation()  const;
            virtual const void *getCStructureForInputs();
            virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            IpV4Address  m_groupAddress;
            UI32  m_interface;
            UI32  m_ifType;
            string  m_ifName;
            UI32  m_vlanId;
            string  m_vlanIfName;
            UI64  m_vlanIfIndex;
            bool  m_negation;
    };
}
#endif                                            //IGMPSCLIENTUPDATESGVLANCONFIGMESSAGE_H
