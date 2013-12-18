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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef OSPFROUTERCONFIGLOCALMESSAGE_H
#define OSPFROUTERCONFIGLOCALMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFRouterConfigLocalMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFRouterConfigLocalMessage ();
            OSPFRouterConfigLocalMessage (const OSPFManagedObjectId &objectId);
            virtual    ~OSPFRouterConfigLocalMessage ();

            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;

			OSPFManagedObjectId getManagedObjectId() const;

			UI32  getReferenceBandwidth() const;
			void setReferenceBandwidth(const UI32  &referenceBandwidth);

			bool  getRefBwUseActivePorts() const;
			void setRefBwUseActivePorts(const bool  &refBwUseActivePorts);

			UI32  getDatabaseOverflowInterval() const;
			void setDatabaseOverflowInterval(const UI32  &databaseOverflowInterval);

			bool  getDefInfoOrigAlways() const;
			void setDefInfoOrigAlways(const bool  &defInfoOrigAlways);

			UI32  getDefInfoOrigMetric() const;
			void setDefInfoOrigMetric(const UI32  &defInfoOrigMetric);

			OSPFDefInfoOrigmetricType  getDefInfoOrigMetricType() const;
			void setDefInfoOrigMetricType(const OSPFDefInfoOrigmetricType  &defInfoOrigMetricType);

			string getDefInfoOrigRouteMap() const;
			void setDefInfoOrigRouteMap(const string &defInfoOrigRouteMap);

			UI32 getDefInfoOrigCmd() const;
			void setDefInfoOrigCmd(const UI32 &cmd);

			UI32  getDefaultMetric() const;
			void setDefaultMetric(const UI32  &defaultMetric);
			
			bool  getDefaultPassiveInterface() const;
			void setDefaultPassiveInterface(const bool  &defaultPassiveInterface);

			UI32  getDistanceExternal() const;
			void setDistanceExternal(const UI32  &distanceExternal);

			UI32  getDistanceInterArea() const;
			void setDistanceInterArea(const UI32  &distanceInterArea);

			UI32  getDistanceIntraArea() const;
			void setDistanceIntraArea(const UI32  &distanceIntraArea);

			string getDistListRouteMap() const;
			void setDistListRouteMap(const string &distListRouteMap);

			IpV4Address  getDomainId() const;
			void setDomainId(const IpV4Address  &domainId);

			UI32  getDomainTag() const;
			void setDomainTag(const UI32  &domainTag);

			UI32  getExtLSDBLimit() const;
			void setExtLSDBLimit(const UI32  &extLSDBLimit);

			bool  getGracefulRestartHelper() const;
			void setGracefulRestartHelper(const bool  &gracefulRestartHelper);

			UI32  getGracefulRestartNotifyTime() const;
			void setGracefulRestartNotifyTime(const UI32  &gracefulRestartNotifyTime);

			UI32  getGracefulRestartRestartTime() const;
			void setGracefulRestartRestartTime(const UI32  &gracefulRestartRestartTime);

			bool getMaxMetricAllLSA() const;
			void setMaxMetricAllLSA(const bool &maxMetricAllLSA);

			IpV4Address  getNeighborAddress() const;
			void setNeighborAddress(const IpV4Address  &neighborAddress);

			bool getIsNSSATranslator() const;
			void setIsNSSATranslator(const bool &isNSSATranslator);

			OSPFDefInfoOrigmetricType getExtMetricType() const;
			void setExtMetricType(const OSPFDefInfoOrigmetricType &extMetricType);

			string  getRedistributeConnectedRouteMap() const;
			void setRedistributeConnectedRouteMap(const string  &redistributeConnectedRouteMap);

			string  getRedistributeBgpRouteMap() const;
			void setRedistributeBgpRouteMap(const string  &redistributeBgpRouteMap);

			string  getRedistributeStaticRouteMap() const;
			void setRedistributeStaticRouteMap(const string  &redistributeStaticRouteMap);

			bool getRfc1583Compatibility() const;
			void setRfc1583Compatibility(const bool &rfc1583Compatibility);

			bool getRfc1587Compatibility() const;
			void setRfc1587Compatibility(const bool &rfc1587Compatibility);

			UI32 getLsaGroupPacingInterval() const;
			void setLsaGroupPacingInterval(const UI32 &timerLsaGroupPacingInterval);

			UI32 getTimerInitDelay() const;
			void setTimerInitDelay(const UI32 &timerInitDelay);

			UI32 getTimerHoldTime() const;
			void setTimerHoldTime(const UI32 &timerHoldTime);

			UI32 getTimerMaxHoldTime() const;
			void setTimerMaxHoldTime(const UI32 &timerMaxHoldTime);

			IpV4Address  getSummaryAddress() const;
			void setSummaryAddress(const IpV4Address  &summaryAddress);

			IpV4Address  getSummaryMask() const;
			void setSummaryMask(const IpV4Address  &summaryMask);

			string  getVRF() const;
			void setVRF(const string  &vrfName);

			UI32 getCmdType() const;
			void setCmdType(const UI32& cmdType);
	
			UI32 getMaxMetricLinkType() const;
			void setMaxMetricLinkType(const UI32& linkType);

			UI32 getMaxMetricOnStartupTime() const;
			void setMaxMetricOnStartupTime(const UI32& time );

			UI32 getExtMaxMetric() const;
			void setExtMaxMetric(const UI32& extMaxMetric );

			UI32 getSummMaxMetric() const;
			void setSummMaxMetric(const UI32& summMaxMetric );
			
			bool getExtMaxMetricLsa() const;
			void setExtMaxMetricLsa(const bool& extMaxMetricLsa );

			bool getSummMaxMetricLsa() const;
			void setSummMaxMetricLsa(const bool& summMaxMetricLsa );

			bool getIsAllVrfSet() const;
			void setIsAllVrfSet(const bool& isAllVrf); 
			bool getIsMaxMetricOnStartupWaitForBgp() const;
                        void setIsMaxMetricOnStartupWaitForBgp(const bool& waitForBgp);

			void dumpMessage() const;
			bool getLinkAll() const;
			void setLinkAll(const bool  &linkAll);
			bool getLinkPtp() const;
			void setLinkPtp(const  bool  &linkPtp);
			bool getLinkStub() const;
			void setLinkStub(const bool  &linkStub);
			bool getLinkTransit() const;
			void setLinkTransit(const bool  &linkTransit);
                        void  setOnStartup (const bool &onstartup);
                        bool  getOnStartup() const;
// Now the data members

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;

			OSPFManagedObjectId m_managedObjectId;

			string m_vrf;

			/* auto-cost reference-bandwidth [<uint32> | use-active-ports] */
            UI32  m_referenceBandwidth;
			bool  m_refBwUseActivePorts;

			/* database-overflow-interval */
            UI32  m_databaseOverflowInterval;

			/* default-info-originate [always | metric | metric-type | route-map] */
			bool  m_defInfoOrigAlways;
            UI32  m_defInfoOrigMetric;
            OSPFDefInfoOrigmetricType  m_defInfoOrigMetricType;
			string m_defInfoOrigRouteMap;
			UI32  m_defInfoOrigCmd;

			/* default-metric <uint32> */
            UI32  m_defaultMetric;
            bool  m_defaultPassiveInterface;

			/* distance [external | inter-area | intra-area ] */
            UI32  m_distanceExternal;
            UI32  m_distanceInterArea;
            UI32  m_distanceIntraArea;

			/* distribute-list <name> in */
			string m_distListRouteMap;

			/* domain-id <a.b.c.d> */
            IpV4Address  m_domainId;

			/* domain-tag <uint32> */
            UI32  m_domainTag;

			/* external-lsdb-limit <uint32> */
            UI32  m_extLSDBLimit;

			/* graceful-restart [helper-disable | notify-time | restart-time] */
            bool  m_gracefulRestartHelper;
            UI32  m_gracefulRestartNotifyTime;
            UI32  m_gracefulRestartRestartTime;

			/* max-metric router-lsa all-lsas */
			bool  m_maxMetricAllLSA;

			/* neighbor <a.b.c.d> */
            IpV4Address  m_neighborAddress;

			/* metric-type <type1 | type2> */
			OSPFDefInfoOrigmetricType m_extMetricType;

			/* nssa-translator */
			bool m_isNSSATranslator;

			/* redistribution [connected | bgp | static ] */
            string  m_redistributeConnectedRouteMap;
            string  m_redistributeBgpRouteMap;
            string  m_redistributeStaticRouteMap;

			/* rfc1583-compatibility */
			bool m_rfc1583Compatibility;

			/* rfc1587-compatibility */
			bool m_rfc1587Compatibility;
			UI32 m_timerLsaGroupPacingInterval;
			UI32 m_timerInitDelay;
			UI32 m_timerHoldTime;
			UI32 m_timerMaxHoldTime;

			/* summary-address <a.b.c.d> <mask> */
            IpV4Address  m_summaryAddress;
            IpV4Address  m_summaryMask;
		UI32 m_cmdType;
		UI32 m_maxMetricLinkType;
		UI32 m_maxMetricOnStartupTime;
		UI32 m_extMaxMetric;         
		UI32 m_summMaxMetric;
		bool m_extMaxMetricLsa;         
		bool m_summMaxMetricLsa;
		bool m_isAllVrfMaxMetricSet;   
		bool m_isMaxMetricOnStartupWaitForBgp;
                bool m_linkAll;
                bool m_linkPtp;
                bool m_linkStub;
                bool m_linkTransit;
                bool m_onStartup;
    };
}
#endif                                            //OSPFROUTERCONFIGLOCALMESSAGE_H
