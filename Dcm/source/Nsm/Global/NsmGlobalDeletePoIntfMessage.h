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

#ifndef NSMGLOBALDELETEPOINTFMESSAGE_H
#define NSMGLOBALDELETEPOINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalDeletePoIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalDeletePoIntfMessage ();
            NsmGlobalDeletePoIntfMessage (const string &poId,const UI32 &poIfIndex);
            virtual    ~NsmGlobalDeletePoIntfMessage ();
            void setPoId(const string &poId);
            string getPoId()  const;
            void setPoIfIndex(const UI32 &poIfIndex);
            UI32 getPoIfIndex()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_poId;
            UI32  m_poIfIndex;
    };
}
#endif                                            //NSMGLOBALDELETEPOINTFMESSAGE_H