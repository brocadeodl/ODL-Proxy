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

#ifndef NSMVLANINTFMESSAGE_H
#define NSMVLANINTFMESSAGE_H

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmVlanIntfMessage : public NsmVlanProvisionMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmVlanIntfMessage ();
            NsmVlanIntfMessage (const UI32 &bulkcmdcode);
            NsmVlanIntfMessage (const UI32 &vlanId,const string &vlanName,const UI32 &vlanIfIndex,const string &desc,const bool &flag,const UI32 &cmdCode, const UI8 &pvlanType);
            virtual    ~NsmVlanIntfMessage ();
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
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setBulkCmdCode(const UI32 &cmdCode);
            UI32 getBulkCmdCode() const;
			void setPvlanType(const UI8 &pvlanType);
			UI8 getPvlanType()  const;
			void setPvlanSecAssocRange (const UI32Range &pVlanSecAssocRange);
			UI32Range getPvlanSecAssocRange ()  const;
            void setRemoteSpanEnabled(const bool &vlanRemoteSpanEnable);
            bool getRemoteSpanEnabled()  const;
        const virtual void *getCStructureForInputs();
        bool isReplayInProgress() const;
        void setReplayInProgress(bool isReplayInProgress);


// Now the data members

        private:
        protected:
        public:
            UI32 m_vlanId;
            string  m_vlanName;
            UI32  m_vlanIfIndex;
            string  m_desc;
            bool  m_flag;
            UI32  m_cmdCode;
            UI32  m_bulkCmdCode;
            UI8  m_pvlanType;
            UI32Range	m_pVlanSecAssocRange;
            bool  m_remoteSpanEnabled;
            bool m_replayInProgress;
    };
}
#endif                                            //NSMVLANINTFMESSAGE_H
