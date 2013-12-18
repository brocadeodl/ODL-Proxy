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

#ifndef NSMADDPHYINTFTOPOINTFMESSAGE_H
#define NSMADDPHYINTFTOPOINTFMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmAddPhyIntfToPoIntfMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmAddPhyIntfToPoIntfMessage ();
            NsmAddPhyIntfToPoIntfMessage (const string &ifName,const UI64 &swIfIndex,const string &poName,const UI32 &poIndex,const UI32 &mode,const UI32 &type,const string &poLacpSystemId, const UI32 &poLacpSystemIdDomainId, const bool &replayPoConfigs, const UI32 &cmdCode);
            virtual    ~NsmAddPhyIntfToPoIntfMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setSwIfIndex(const UI64 &swIfIndex);
            UI64 getSwIfIndex()  const;
            void setPoName(const string &poName);
            string getPoName()  const;
            void setPoIndex(const UI32 &poIndex);
            UI32 getPoIndex()  const;
            void setPoMode(const UI32 &mode);
            UI32 getPoMode()  const;
            void setPoType(const UI32 &type);
            UI32 getPoType()  const;
            void setPoLacpSystemId(const string &poLacpSystemId);
            string getPoLacpSystemId()  const;
            void setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId);
            UI32 getPoLacpSystemIdDomainId()  const;
			void setReplayPoConfigs(const bool &replayPoConfigs);
			bool getReplayPoConfigs () const;
			void setIfType(const UI32 &ifType);
            UI32 getIfType() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
			/* TR 370798 - ELD */
			void setPrio(const UI32 &prio);
			UI32 getPrio()  const;
			void setVlanidList(const vector<UI32> &vlanidList);
			vector<UI32> getVlanidList()  const;
			/* TR 370798 - ELD */


// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI64  m_swIfIndex;
            string  m_poName;
            UI32  m_poIndex;
            UI32  m_poMode;
            UI32  m_poType;
            UI32  m_cmdCode;
            string m_poLacpSystemId;
            UI32 m_poLacpSystemIdDomainId;
		    bool  m_replayPoConfigs;
		    UI32 m_ifType;
			/* TR 370798 - ELD */
			UI32  m_prio;
			vector<UI32>  m_vlanidList;
			UI32Range     m_vlanIdRange;
			/* TR 370798 - ELD */
    };
}
#endif                                            //NSMADDPHYINTFTOPOINTFMESSAGE_H
