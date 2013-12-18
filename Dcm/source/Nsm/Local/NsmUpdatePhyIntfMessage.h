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

#ifndef NSMUPDATEPHYINTFMESSAGE_H
#define NSMUPDATEPHYINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdatePhyIntfMessage : public ManagementInterfaceMessage
    {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NsmUpdatePhyIntfMessage ();
			NsmUpdatePhyIntfMessage (const string &ifName,const UI64 &swIfIndex,const UI32 &mtu,const bool &openflow,const string &desc,const bool &flag,const swModeEnum &swMode,const swModeEnum &mode,const bool &isAllowedVlanAll,const UI32 &cmdCodae,const LacpTimeout &lacpTimeout, const UI32 &lacpPortPriority, const LineSpeed &lineSpeed,const UI32 &nativeVlanId,const bool &enable, const UI32 &pollingInterval, const UI32 &sampleRate, const bool &isPortProfiled, const UI32 &msgId, const UI32 &exception, const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll);
			virtual    ~NsmUpdatePhyIntfMessage ();
			void setIfName(const string &ifName);
			string getIfName()  const;
			void setSwIfIndex(const UI64 &swIfIndex);
			UI64 getSwIfIndex()  const;
			void setMtu(const UI32 &mtu);
			UI32 getMtu()  const;
			void setOpenflow(const bool &openflow);
			bool getOpenflow()  const;
			void setDesc(const string &desc);
			string getDesc()  const;
			void setFlag(const bool &flag);
			bool getFlag()  const;
			void setActive(const bool &flag);
			bool getActive()  const;
			void setVepa(const bool &flag);
			bool getVepa()  const;
			void setEld(const bool &eld);
			bool getEld()  const;
			void setEldPrioVal(const UI32 &eldPrio);
			UI32 getEldPrioVal()  const;
			void setEldVlan(const UI32 &eldVlan);
			UI32 getEldVlan()  const;
			void setSwMode(const swModeEnum &swMode);
			swModeEnum getSwMode()  const;
			void setMode(const swModeEnum &mode);
			swModeEnum getMode()  const;
			void setIsAllowedVlanAll(const bool &isAllowedVlanAll);
			bool getIsAllowedVlanAll()  const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode()  const;
			void setSwModeFlag(const bool &swModeFlag);
			bool getSwModeFlag()  const;
			void setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId);
			UI32 getPvlanNativeVlanId()  const;
			void setPvlanIsAllowedVlanAll (const bool &pVlanIsAllowedVlanAll);
			bool getPvlanIsAllowedVlanAll()  const;
			void setPvlanRange (const UI32Range &pVlanRange);
			UI32Range getPvlanRange ()  const;
			void setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId);
			UI32 getPvlanHostPrimVlanId()  const;
			void setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId);
			UI32 getPvlanHostSecVlanId()  const;
			void setLacpTimeout(const LacpTimeout &lacpTimeout);
			LacpTimeout getLacpTimeout()  const;
			void setLacpPortPriority(const UI32 &lacpportPriority);
			UI32 getLacpPortPriority () const; 
			void setLacpDefaultUp(const bool &lacpDefaultUp);
			bool getLacpDefaultUp()  const;
			void setLineSpeed(const LineSpeed &lineSpeed);
			LineSpeed getLineSpeed()  const;
            void setNativevlanDisable(const bool &nativevlanDisable);
            bool getNativevlanDisable()  const;
			void setNativeVlanId(const UI32 &nativeVlanId);
			UI32 getNativeVlanId() const;
			void setEnable(const bool &enable);
			bool getEnable()  const;
			void setPollingInterval(const UI32 &pollingInterval);
			UI32 getPollingInterval() const;
			void setSampleRate(const UI32 &sampleRate);
			UI32 getSampleRate() const;
			void setVlanRange(const UI32Range &vlanRange);
			UI32Range getVlanRange()  const;

            void setIsPortProfiled(const bool &flag);
            bool getIsPortProfiled()  const;
            void setPortProfileName(const string &portProfileName);
            string getPortProfileName() const;
            void setPortProfileDomainName(const string &portProfileDomainName);
            string getPortProfileDomainName() const;

			void setMsgId(const UI32 &msgId);
			UI32 getMsgId()  const;
            void setException(const UI32 &exception);
            UI32 getException()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
			void setVlanidList(const vector<UI32> &vlanidList);
			vector<UI32> getVlanidList()  const;
			void setVlanIdRange(const UI32Range &vlanIdRange);
			UI32Range getVlanIdRange()  const;
			void setVlanid(const vector<ObjectId> &vlanid);
			vector<ObjectId> getVlanid()  const;
			void setRaGuard(const bool &raGuard);
			bool getRaGuard() const;
			// Now the data members

		private:
		protected:
		public:
			string  m_ifName;
			UI64  m_swIfIndex;
			UI32  m_mtu;
			bool  m_openflow;
			string  m_desc;
			bool  m_flag;
			bool  m_active;
			bool  m_vepa;
			swModeEnum  m_swMode;
			swModeEnum  m_mode;
			bool  m_isAllowedVlanAll;
			UI32Range  m_vlanRange;
			UI32  m_cmdCode;
            UI32  m_ifType;
			bool  m_swModeFlag;
			UI32  m_pVlanNativeVlanId;
			bool  m_pVlanIsAllowedVlanAll;
			UI32Range  m_pVlanRange;
            UI32  m_pVlanHostPrimVlanId;
            UI32  m_pVlanHostSecVlanId;
			LacpTimeout  m_lacpTimeout;
			UI32 m_lacpPortPriority;
			bool m_lacpDefaultUp;
			LineSpeed  m_lineSpeed;
            bool m_nativevlanDisable;
			UI32 m_nativeVlanId;
			bool m_enable;
			UI32 m_pollingInterval;
			UI32 m_sampleRate;
            bool m_isPortProfiled; /* APPM */
            string m_portProfileName;/* APPM */
            string m_portProfileDomainName;/* APPM */
			UI32 m_msgId;
			UI32 m_exception;
			bool  m_eld;			/* ELD */
			UI32  m_eld_prio_val;			/* ELD */
			UI32  m_eld_vlan;			/* ELD */
			vector<ObjectId>  m_vlanid;
			vector<UI32>  m_vlanidList;
			UI32Range     m_vlanIdRange;
			bool m_raGuard; //routing-advertisement-guard
	};
}
#endif                                            //NSMUPDATEPHYINTFMESSAGE_H
