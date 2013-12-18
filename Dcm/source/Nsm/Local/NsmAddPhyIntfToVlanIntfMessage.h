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

#ifndef NSMADDPHYINTFTOVLANINTFMESSAGE_H
#define NSMADDPHYINTFTOVLANINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmAddPhyIntfToVlanIntfMessage : virtual public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmAddPhyIntfToVlanIntfMessage ();
            NsmAddPhyIntfToVlanIntfMessage (const UI64 &swIfIndex,const string &vlanName,const UI32 &tagStatus);
            virtual    ~NsmAddPhyIntfToVlanIntfMessage ();
            void setSwIfIndex(const UI64 &swIfIndex);
            UI64 getSwIfIndex()  const;
            void setVlanName(const string &vlanName);
            string getVlanName()  const;
            void setTagStatus(const UI32 &tagStatus);
            UI32 getTagStatus()  const;

// Now the data members

        private:
        protected:
        public:
            UI64  m_swIfIndex;
            string  m_vlanName;
            UI32  m_tagStatus;
    };
}
#endif                                            //NSMADDPHYINTFTOVLANINTFMESSAGE_H
