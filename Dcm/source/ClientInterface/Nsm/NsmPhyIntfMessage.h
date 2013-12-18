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

#ifndef NSMPHYINTFMESSAGE_H
#define NSMPHYINTFMESSAGE_H

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmPhyIntfMessage : public NsmVlanProvisionMessage
    {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NsmPhyIntfMessage ();
			NsmPhyIntfMessage (const string &ifName,const UI64 &swIfIndex,const UI32 &mtu, const bool &openflow,const string &desc,const bool &flag,const bool &swModeFlag,const UI32 &swMode,const bool &isAllowedVlanAll,const UI32 &cmdCode,const LacpTimeout &lacpTimeout, const UI32 &lacpPortPriority, const LineSpeed &lineSpeed,const UI32 &nativeVlanId,const string &poName, const bool &enable, const UI32 &pollingInterval, const UI32 &sampleRate, const bool &isPortProfiled, const UI32 &msgId, const UI32 &exception, const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll);
			virtual    ~NsmPhyIntfMessage ();
			void setIfName(const string &ifName);
			string getIfName()  const;
			void setPoName(const string &poName);
			string getPoName()  const;
			void setSwIfIndex(const UI64 &swIfIndex);
			UI64 getSwIfIndex()  const;
			void setMtu(const UI32 &mtu);
			UI32 getMtu()  const;
            void setOpenflow(const bool &openflow);
            bool getOpenflow() const;
			void setDesc(const string &desc);
			string getDesc()  const;
			void setFlag(const bool &flag);
			bool getFlag()  const;
            void setVepa(const bool &flag);
            bool getVepa()  const;
			void setSwMode(const UI32 &swMode);
			UI32 getSwMode()  const;
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
			UI32Range getPvlanRange()  const;
			void setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId);
			UI32 getPvlanHostPrimVlanId()  const;
			void setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId);
			UI32 getPvlanHostSecVlanId()  const;
			void setLacpTimeout(const LacpTimeout &lacpTimeout);
			LacpTimeout getLacpTimeout()  const;
			void setLacpPortPriority(const UI32 &LacpPortPriority);
			UI32 getLacpPortPriority()  const;
			void setLacpDefaultUp(const bool &lacpDefaultUp);
			bool getLacpDefaultUp()  const;
			void setLineSpeed(const LineSpeed &lineSpeed);
			LineSpeed getLineSpeed()  const;
			void setNativevlanDisable(const bool &nativevlanDisable);
			bool getNativevlanDisable()  const;
			void setNativeVlanId(const UI32 &nativeVlanId);
            UI32 getNativeVlanId()  const;
			void setVlanRange(const UI32Range &vlanRange);
			UI32Range getVlanRange()  const;
			void setEnable(const bool &enable);
			bool getEnable()  const;
			void setPollingInterval(const UI32 &pollingInterval);
			UI32 getPollingInterval()  const;
			void setSampleRate(const UI32 &sampleRate);
            UI32 getSampleRate()  const;
            void setPoMode(const UI32 &mode);
            UI32 getPoMode()  const;
            void setPoType(const UI32 &type);
            UI32 getPoType()  const;

			void setIsPortProfiled(const bool &isPortProfiled, const UI32 type);
			void setIsPortProfiled(const bool &isPortProfiled);
			bool getIsPortProfiled()  const;
            void setPortProfileName(const string &portProfileName);
            void setPortProfileDomainName(const string &portProfileDomainName);
            string getPortProfileName() const;
            string getPortProfileDomainName() const;

			void setMsgId(const UI32 &msgId);
			UI32 getMsgId()  const;
            void setException(const UI32 &exception);
            UI32 getException()  const;

            virtual const void *getCStructureForInputs    ();

            void setBulkCmdCode(const UI32 &cmdCode);
            UI32 getBulkCmdCode() const;


            void  setPortChannelName(const string &poName,const UI32 &mode, const UI32 &type);
            void  setVlanRange(const UI32Range &vlanRange,const UI32 type );
			// Now the data members

            void setPoLacpSystemId(const string &poLacpSystemId);
            string getPoLacpSystemId()  const;
            void setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId);
            UI32 getPoLacpSystemIdDomainId()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
            void setEld(const bool &eld);
            bool getEld()  const;
            void setEldPrioVal(const UI32 &eldPrio);
            UI32 getEldPrioVal()  const;
            void setEldVlan(const UI32 &eldVlan);
            UI32 getEldVlan()  const;
            void setRaGuard(const bool &raGuard);
            bool getRaGuard() const;
		void setNbrDiscoveryDisable(const bool &discoveryFlag);
            void setMac(const string &mac);
            string getMac() const;
            void setCtagId(const UI32 &ctagId);
            UI32 getCtagId()  const;
            void setMacGroupId(const UI32 &macGroupId);
            UI32 getMacGroupId()  const;
			
		private:
		protected:
		public:
			string  m_ifName;
			UI64  m_swIfIndex;
			UI32  m_mtu;
            bool  m_openflow;
			string  m_desc;
			bool  m_flag;
			bool  m_vepa;
			bool  m_swModeFlag;
			bool  m_eld;
			UI32  m_eld_prio_val;
			UI32  m_eld_vlan;
			UI32  m_swMode;
			bool  m_isAllowedVlanAll;
			UI32Range  m_vlanRange;
			UI32  m_cmdCode;
            UI32  m_bulkCmdCode;
            UI32  m_ifType;
			UI32  m_pVlanNativeVlanId;
			bool  m_pVlanIsAllowedVlanAll;
			UI32Range  m_pVlanRange;
            UI32  m_pVlanHostPrimVlanId;
            UI32  m_pVlanHostSecVlanId;
			LacpTimeout  m_lacpTimeout;
			UI32 m_lacpPortPriority;
			bool m_lacpDefaultUp;
			LineSpeed  m_lineSpeed;
            bool  m_nativevlanDisable;
			UI32 m_nativeVlanId;
			string m_poName;
			bool m_enable;
			UI32 m_pollingInterval;
			UI32 m_sampleRate;
            bool m_isPortProfiled;	/* APPM */
            string m_portProfileName;
            string m_portProfileDomainName;
			UI32 m_msgId;
			UI32 m_exception;
            UI32  m_poMode;
            UI32  m_poType;
            string m_poLacpSystemId;
            UI32 m_poLacpSystemIdDomainId;
			bool m_raGuard; //router-advertisement-guard
			string m_mac;
			UI32 m_ctagId;
			UI32 m_macGroupId;
	};
}
#endif                                            //NSMPHYINTFMESSAGE_H
