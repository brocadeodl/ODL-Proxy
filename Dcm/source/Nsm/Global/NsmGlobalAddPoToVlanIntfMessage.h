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

#ifndef NSMGLOBALADDPOTOVLANINTFMESSAGE_H
#define NSMGLOBALADDPOTOVLANINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalAddPoToVlanIntfMessage : virtual public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalAddPoToVlanIntfMessage ();
            NsmGlobalAddPoToVlanIntfMessage (const UI64 &poIfIndex,const string &vlanName,const UI32 &tagStatus);
            virtual    ~NsmGlobalAddPoToVlanIntfMessage ();
            void setPoIfIndex(const UI64 &poIfIndex);
            UI64 getPoIfIndex()  const;
            void setVlanName(const string &vlanName);
            string getVlanName()  const;
            void setTagStatus(const UI32 &tagStatus);
            UI32 getTagStatus()  const;

// Now the data members

        private:
        protected:
        public:
            UI64  m_poIfIndex;
            string  m_vlanName;
            UI32  m_tagStatus;
    };
}
#endif                                            //NSMGLOBALADDPOTOVLANINTFMESSAGE_H
