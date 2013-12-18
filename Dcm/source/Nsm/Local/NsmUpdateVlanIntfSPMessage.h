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

#ifndef NSMUPDATEVLANINTFSPMESSAGE_H
#define NSMUPDATEVLANINTFSPMESSAGE_H

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdateVlanIntfSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmUpdateVlanIntfSPMessage ();
            NsmUpdateVlanIntfSPMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc,const bool &flag,const UI32 &mtu,const UI32 &cmdCode,const pvlanTypeEnum &pvlanType);
            virtual    ~NsmUpdateVlanIntfSPMessage ();
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;
            void setVlanName(const string &vlanName);
            string getVlanName()  const;
            void setVlanIfIndex(const UI32 &vlanIfIndex);
            UI32 getVlanIfIndex()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setFlag(const bool &flag);
            bool getFlag()  const;
            void setPvlanType(const pvlanTypeEnum &pvlanType);
            pvlanTypeEnum getPvlanType()  const;
			void setPvlanSecAssocRange (const UI32Range &pVlanSecAssocRange);
			UI32Range getPvlanSecAssocRange ()  const;
            void setMtu(const UI32 &mtu);
            UI32 getMtu()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setRemoteSpanEnabled(const bool &vlanRemoteSpanEnable);
            bool getRemoteSpanEnabled()  const;
            bool isReplayInProgress() const;
            void setReplayInProgress(const bool &isReplayInProgress);

// Now the data members

        private:
        protected:
        public:
            UI32  m_vlanId;
            string  m_vlanName;
            UI32  m_vlanIfIndex;
            string  m_desc;
            bool  m_flag;
            pvlanTypeEnum m_pvlanType;
            UI32Range	m_pVlanSecAssocRange;
            UI32 m_mtu;
            UI32  m_cmdCode;
            bool  m_remoteSpanEnabled;
            bool m_replayInProgress;
    };
}
#endif                                            //NSMUPDATEVLANINTFSPMESSAGE_H
