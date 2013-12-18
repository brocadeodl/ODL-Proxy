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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef NTPCLOCKGLOBALGETSERVERMESSAGE_H
#define NTPCLOCKGLOBALGETSERVERMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class NTPClockGlobalGetServerMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NTPClockGlobalGetServerMessage ();
            NTPClockGlobalGetServerMessage (const IpVxAddress &ActiveServer,const UI32 &MappedId);
            NTPClockGlobalGetServerMessage (const vector<IpVxAddress> &ActiveServer, const vector<UI32> &MappedId);
            virtual    ~NTPClockGlobalGetServerMessage ();
            void setActiveServer(const IpVxAddress &ActiveServer);
            IpVxAddress getActiveServer()  const;
            void setMappedId(const UI32 &MappedId);
            UI32 getMappedId()  const;

            UI32 getNumVectors() const;
            void clearNumVectors();
            void setVectorActiveServer(const IpVxAddress &ActiveServer, const UI32 &MappedId);
            bool getVectorActiveServer(const UI32 &index, IpVxAddress &ActiveServer, UI32 &MappedId);

// Now the data members

        private:
        protected:
        public:
            IpVxAddress  m_ActiveServer;
            UI32  m_MappedId;
            UI32  m_NumVectors;	
            vector<IpVxAddress>  m_vActiveServer;
            vector<UI32>  m_vMappedId;
    };
}
#endif                                            //NTPCLOCKGLOBALGETSERVERMESSAGE_H
