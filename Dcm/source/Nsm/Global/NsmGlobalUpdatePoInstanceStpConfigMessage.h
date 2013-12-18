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
 *   Author : sudheend                                                     *
 **************************************************************************/

#ifndef NSMGLOBALUPDATEPOINSTANCESTPCONFIGMESSAGE_H
#define NSMGLOBALUPDATEPOINSTANCESTPCONFIGMESSAGE_H

#include "Nsm/Common/NsmPortStpConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalUpdatePoInstanceStpConfigMessage : public NsmPortStpConfigMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalUpdatePoInstanceStpConfigMessage ();
            NsmGlobalUpdatePoInstanceStpConfigMessage (const string &ifName,const UI8 &instanceId,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode);
            virtual    ~NsmGlobalUpdatePoInstanceStpConfigMessage ();
            void setIfName(const string &ifName);
            void setInstanceId(const UI8 &instanceId);
            UI8 getInstanceId()  const;

	    void printMsg();
// Now the data members

        private:
        protected:
        public:
            UI8  m_instanceId;

    };
}
#endif                                            //NSMGLOBALUPDATEPOINSTANCESTPCONFIGMESSAGE_H
