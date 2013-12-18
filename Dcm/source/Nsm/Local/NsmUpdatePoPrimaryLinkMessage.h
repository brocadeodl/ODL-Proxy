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

#ifndef NSMUPDATEPOPRIMARYLINK_H
#define NSMUPDATEPOPRIMARYLINK_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdatePoPrimaryLinkMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmUpdatePoPrimaryLinkMessage ();
            NsmUpdatePoPrimaryLinkMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const UI32 &mtu,const string &desc,const bool &flag,const bool &swModeFlag,const UI32 &swMode,const bool &isAllowedVlanAll,const UI32 &poMode,const UI32 &poType,const UI32 &poMinLinks,const LineSpeed &poAllowedSpeed,const UI32 &poLoadBalanceMode, const UI32 &poPrimaryLinkDomainId, const UI32 &poPartnerSystemPriority, const UI32 &poPartnerKey, const string &poPartnerSystem,const string &poLacpSystemId, const UI32 &cmdCode);
            virtual    ~NsmUpdatePoPrimaryLinkMessage ();
            void setPoId(const string &poId);
            string getPoId()  const;
            void setPoName(const string &poName);
            string getPoName()  const;
            void setPoIfIndex(const UI32 &poIfIndex);
            UI32 getPoIfIndex()  const;
            void setMtu(const UI32 &mtu);
            UI32 getMtu()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setFlag(const bool &flag);
            bool getFlag()  const;
            void setSwMode(const UI32 &swMode);
            UI32 getSwMode()  const;
            void setMode(const UI32 &mode);
            UI32 getMode()  const;
            void setIsAllowedVlanAll(const bool &isAllowedVlanAll);
            bool getIsAllowedVlanAll()  const;
            void setPoMode(const UI32 &poMode);
            UI32 getPoMode()  const;
            void setPoType(const UI32 &poType);
            UI32 getPoType()  const;
			void setPoMinLinks(const UI32 &poMinLinks);
			UI32 getPoMinLinks()  const;
			void setPoAllowedSpeed(const LineSpeed &poAllowedSpeed);
			LineSpeed getPoAllowedSpeed() const;
			void setPoLoadBalanceMode(const UI32 &poLoadBalanceMode);
			UI32 getPoLoadBalanceMode()  const;
            void setPoPrimaryLinkDomainId(const UI32 &poPrimaryLinkDomainId);
            UI32 getPoPrimaryLinkDomainId()  const;
            void setPoPartnerSystemPriority(const UI32 &poPartnerSystemPriority);
            UI32 getPoPartnerSystemPriority()  const;
            void setPoPartnerKey(const UI32 &poPartnerKey);
            UI32 getPoPartnerKey()  const;
            void setPoPartnerSystem(const string &poPartnerSystem);
            string getPoPartnerSystem()  const;
            void setPoLacpSystemId(const string &poLacpSystemId);
            string getPoLacpSystemId()  const;
            void setReplyCode(const UI32 &replyCode);
            UI32 getReplyCode()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setSwModeFlag(const bool &swModeFlag);
            bool getSwModeFlag()  const;
			void setNativeVlanId(const UI32 &nativeVlanId);
			UI32 getNativeVlanId() const;

// Now the data members

        private:
        protected:
        public:
            string  m_poId;
            string  m_poName;
            UI32  m_poIfIndex;
            UI32  m_mtu;
            string  m_desc;
            bool  m_flag;
            UI32  m_mode;
            bool  m_swModeFlag;
            UI32  m_swMode;
            bool  m_isAllowedVlanAll;
            UI32  m_poMode;
            UI32  m_poType;
            UI32  m_poMinLinks;
			LineSpeed m_poAllowedSpeed;
            UI32  m_poLoadBalanceMode;
            UI32  m_poPrimaryLinkDomainId;
            UI32  m_poPartnerSystemPriority;
            UI32  m_poPartnerKey;
            string  m_poPartnerSystem;
            string  m_poLacpSystemId;
            UI32  m_cmdCode;
            UI32  m_replyCode;
	   UI32 m_nativeVlanId;
    };
}
#endif                                            //NSMUPDATEPOPRIMARYLINK_H
