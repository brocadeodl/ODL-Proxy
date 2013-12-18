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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMCREATEPHYINTFMESSAGE_H
#define NSMCREATEPHYINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmCreatePhyIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmCreatePhyIntfMessage ();
            NsmCreatePhyIntfMessage (const string &ifName,const UI64 &swIfIndex,const UI32 &mtu,const string &desc);
            virtual    ~NsmCreatePhyIntfMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setSwIfIndex(const UI64 &swIfIndex);
            UI64 getSwIfIndex()  const;
            void setMtu(const UI32 &mtu);
            UI32 getMtu()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setPortrole(const bool &Portrole);
            bool getPortrole() const;
	    void setIslCapability(const UI32 &islCapability);
	    UI32 getIslCapability() const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI64  m_swIfIndex;
            UI32  m_mtu;
            string  m_desc;
            bool m_portrole;
	    UI32 m_islCapability;
    };
}
#endif                                            //NSMCREATEPHYINTFMESSAGE_H
