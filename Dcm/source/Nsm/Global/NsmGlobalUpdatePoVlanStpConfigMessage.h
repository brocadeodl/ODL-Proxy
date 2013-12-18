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
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef NSMGLOBALUPDATEPOVLANSTPCONFIGMESSAGE_H
#define NSMGLOBALUPDATEPOVLANSTPCONFIGMESSAGE_H

#include "Nsm/Common/NsmPortStpConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalUpdatePoVlanStpConfigMessage : public NsmPortStpConfigMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalUpdatePoVlanStpConfigMessage ();
            NsmGlobalUpdatePoVlanStpConfigMessage (const string &ifName,const UI32 &vid,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode);
            virtual    ~NsmGlobalUpdatePoVlanStpConfigMessage ();
            void setIfName(const string &ifName);
            void setVid(const UI32 &vid);
            UI32 getVid()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_vid;

    };
}
#endif                                            //NSMGLOBALUPDATEPOVLANSTPCONFIGMESSAGE_H
