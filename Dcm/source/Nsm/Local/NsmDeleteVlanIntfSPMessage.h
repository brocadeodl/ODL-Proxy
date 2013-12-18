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

#ifndef NSMDELETEVLANINTFSPMESSAGE_H
#define NSMDELETEVLANINTFSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmDeleteVlanIntfSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmDeleteVlanIntfSPMessage ();
            NsmDeleteVlanIntfSPMessage (const UI32 &vlanId,const UI32 &vlanIfIndex, const ObjectId &vlanObjectId);
            virtual    ~NsmDeleteVlanIntfSPMessage ();
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;
            void setVlanIfIndex(const UI32 &vlanIfIndex);
            UI32 getVlanIfIndex()  const;
            void setVlanObjectID(const ObjectId &vlanObjectId);
            ObjectId getVlanObjectID()  const;
            bool isDBUpdateRequest() const;
            void setDBUpdateRequest(const bool &updateDB);

// Now the data members

        private:
        protected:
        public:
            UI32  m_vlanId;
            UI32  m_vlanIfIndex;
			ObjectId m_vlanObjectId;
            bool isDBUpdateReq;
    };
}
#endif                                            //NSMDELETEVLANINTFSPMESSAGE_H
