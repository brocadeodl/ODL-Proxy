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

#ifndef NSMDELETEPHYINTFTOPOINTFMESSAGE_H
#define NSMDELETEPHYINTFTOPOINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmDeletePhyIntfToPoIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmDeletePhyIntfToPoIntfMessage ();
            NsmDeletePhyIntfToPoIntfMessage (const string &ifName,const UI64 &swIfIndex,const string &poName,const UI32 &poIndex);
            virtual    ~NsmDeletePhyIntfToPoIntfMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setSwIfIndex(const UI64 &swIfIndex);
            UI64 getSwIfIndex()  const;
            void setPoName(const string &poName);
            string getPoName()  const;
            void setPoIndex(const UI32 &poIndex);
            UI32 getPoIndex()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType() const;

// Now the data members

        private:
        protected:
        public:
            string m_ifName;
            UI64  m_swIfIndex;
            string  m_poName;
            UI32  m_poIndex;
            UI32 m_ifType;
    };
}
#endif                                            //NSMDELETEPHYINTFTOPOINTFMESSAGE_H
