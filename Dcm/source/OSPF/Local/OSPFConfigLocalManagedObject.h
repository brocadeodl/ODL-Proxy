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

#ifndef OSPFCONFIGLOCALMANAGEDOBJECT_H
#define OSPFCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
//#include "Framework/Attributes/AttributeBoolUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalObjectManager;
    class OSPFAreaLocalManagedObject;
    class OSPFDistanceLocalManagedObject;
    class OSPFNeighborIPAddressLocalManagedObject;
    class OSPFPermitRedistributeLocalManagedObject;
    class OSPFDenyRedistributeLocalManagedObject;
    class OSPFSummaryAddressLocalManagedObject;
	class OSPFRouterConfigLocalMessage;
	class OSPFRouterConfigMessage;
    class OSPFConfigLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            OSPFConfigLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager);
            OSPFConfigLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const bool &status,const string &vrfName,const IpV4Address &domainId,const UI32 &domainTag,const UI32 &extLSDBLimit,const UI32 &defaultMetric,const bool &defaultPassiveInterface,const OSPFDefInfoOrigmetricType &extMetricType,const UI32 &ReferenceBandwidth,const bool &useActivePorts,const UI32 &databaseOverflowInterval,const bool &defInfoOrig,const bool &always,const UI32 &defInfoOrigMetric,const OSPFDefInfoOrigmetricType &defInfoOrigMetricType,const string &defInfoOrigRouteMap,const bool &isNSSATranslator,const bool &redistributeStatic,const bool &redistributeConnected,const bool &redistributeBgp,const string &redistributeStaticMapName,const string &redistributeConnectedMapName,const string &redistributeBgpMapName,const bool &rfc1583Compatibility,const bool &rfc1587Compatibility,const UI32 &timerLsaGroupPacingInterval, const UI32 &timerInitDelay,const UI32 &timerHoldTime,const UI32 &timerMaxHoldTime,const string &routeMap,const bool &is_routeMapIncoming,const bool &isMaxMetricAllLSA,const bool &gresEnabled,const bool &gracefulRestartHelper,const UI32 &gracefulRestartNotifyTime,const UI32 &gracefulRestartRestartTime,const bool &log_adjacency,const bool &log_dr_only,const bool &log_all,const bool &log_bad_packet,const bool &log_checksum,const bool &log_database,const bool &log_retransmit,const vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> > &area,const vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> > &distance,const vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> > &neighborList,const vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> > &permitRedistribute,const vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> > &denyRedistribute,const vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> > &summary);
            virtual    ~OSPFConfigLocalManagedObject ();
            static string getClassName();
            void setStatus(const bool &status);
            bool getStatus()  const;
            void setVRF(const string &vrfName);
            string getVRF()  const;
            void setDomainId(const IpV4Address &domainId);
            IpV4Address getDomainId()  const;
            void setDomainTag(const UI32 &domainTag);
            UI32 getDomainTag()  const;
            void setExtLSDBLimit(const UI32 &extLSDBLimit);
            UI32 getExtLSDBLimit()  const;
            void setDefaultPassiveInterface(const bool &defaultPassiveInterface);
            bool getDefaultPassiveInterface()  const;
            void setDefaultMetric(const UI32 &defaultMetric);
            UI32 getDefaultMetric()  const;
            void setExtMetricType(const OSPFDefInfoOrigmetricType &extMetricType);
            OSPFDefInfoOrigmetricType getExtMetricType()  const;
            void setReferenceBandwidth(const UI32 &ReferenceBandwidth);
            UI32 getReferenceBandwidth()  const;
            void setUseActivePorts(const bool &useActivePorts);
            bool getUseActivePorts()  const;
            void setDatabaseOverflowInterval(const UI32 &databaseOverflowInterval);
            UI32 getDatabaseOverflowInterval()  const;
            void setDefInfoOrig(const bool &defInfoOrig);
            bool getDefInfoOrig()  const;
            void setAlways(const bool &always);
            bool getAlways()  const;
            void setDefInfoOrigMetric(const UI32 &defInfoOrigMetric);
            UI32 getDefInfoOrigMetric()  const;
            void setDefInfoOrigMetricType(const OSPFDefInfoOrigmetricType &defInfoOrigMetricType);
            OSPFDefInfoOrigmetricType getDefInfoOrigMetricType()  const;
            void setDefInfoOrigRouteMap(const string &defInfoOrigRouteMap);
            string getDefInfoOrigRouteMap()  const;
            void setIsNSSATranslator(const bool &isNSSATranslator);
            bool getIsNSSATranslator()  const;
            void setRedistributeStatic(const bool &redistributeStatic);
            bool getRedistributeStatic()  const;
            void setRedistributeConnected(const bool &redistributeConnected);
            bool getRedistributeConnected()  const;
            void setRedistributeBgp(const bool &redistributeBgp);
            bool getRedistributeBgp()  const;
            void setRedistributeStaticMapName(const string &redistributeStaticMapName, const bool &set);
            string getRedistributeStaticMapName()  const;
            void setRedistributeConnectedMapName(const string &redistributeConnectedMapName, const bool &set);
            string getRedistributeConnectedMapName()  const;
            void setRedistributeBgpMapName(const string &redistributeBgpMapName, const bool &set);
            string getRedistributeBgpMapName()  const;
            void setRfc1583Compatibility(const bool &rfc1583Compatibility);
            bool getRfc1583Compatibility()  const;
            void setRfc1587Compatibility(const bool &rfc1587Compatibility);
            bool getRfc1587Compatibility()  const;
			UI32 getLsaGroupPacingInterval() const;
			void setLsaGroupPacingInterval(const UI32 &timerLsaGroupPacingInterval);
			UI32 getTimerInitDelay() const;
			bool getTimerInitDelayIsConfigured() const;
			void setTimerInitDelay(const UI32 &timerInitDelay, const bool &set);
			UI32 getTimerHoldTime() const;
			bool getTimerHoldTimeIsConfigured() const;
			void setTimerHoldTime(const UI32 &timerHoldTime, const bool &set);
			UI32 getTimerMaxHoldTime() const;
			bool getTimerMaxHoldTimeIsConfigured() const;
			void setTimerMaxHoldTime(const UI32 &timerMaxHoldTime, const bool &set);
            void setRouteMap(const string &routeMap, const bool &set);
            string getRouteMap()  const;
            void setIsRouteMapIncoming(const bool &routeMapIncoming);
            bool getIsRouteMapIncoming() const;
            void setIsMaxMetricAllLSA(const bool &isMaxMetricAllLSA);
            bool getIsMaxMetricAllLSA() const;
            void setGresEnabled(const bool &gresEnabled);
            bool getGresEnabled() const;
            void setGracefulRestartHelper(const bool &gracefulRestartHelper);
            bool getGracefulRestartHelper()  const;
            void setGracefulRestartNotifyTime(const UI32 &gracefulRestartNotifyTime);
            UI32 getGracefulRestartNotifyTime()  const;
            void setGracefulRestartRestartTime(const UI32 &gracefulRestartRestartTime);
            UI32 getGracefulRestartRestartTime()  const;
			bool getLogAdjacency() const;
			void setLogAdjacency(const bool &log_adjacency);
			bool getLogDROnly() const;
			void setLogDROnly(const bool &log_dr_only);
			bool getLogAll() const;
			void setLogAll(const bool &log_all);
			bool getLogBadPacket() const;
			void setLogBadPacket(const bool &log_bad_packet);
			bool getLogChecksum() const;
			void setLogChecksum(const bool &log_checksum);
			bool getLogDatabase() const;
			void setLogDatabase(const bool &log_database);
			bool getLogRetransmit() const;
			void setLogRetransmit(const bool &log_retransmit);
            void setArea(const vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> > &area);
			void setAreaLocalMO(OSPFAreaLocalManagedObject *area);
			UI32 getAreaCount();
			void deleteAreaLocalMO(OSPFAreaLocalManagedObject *area);
			bool getVrfLiteCapability() const;
			void setVrfLiteCapability(const bool &vrfLiteCapability);

			void updateMOFromLocalMessage(OSPFRouterConfigLocalMessage *pMsg);
			void updateClientMessageForReplay(OSPFRouterConfigMessage *pMsg);

            vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> > getArea()  const;
            void setDistance(const vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> > &distance);
            vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> > getDistance()  const;
            void setNeighborList(const vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> > &neighborList);
            vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> > getNeighborList()  const;
            void setPermitRedistribute(const vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> > &permitRedistribute);
            vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> > getPermitRedistribute()  const;
            void setDenyRedistribute(const vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> > &denyRedistribute);
            vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> > getDenyRedistribute()  const;
            void setSummary(const vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> > &summary);
            vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> > getSummary()  const;

			void addDistanceManagedObject(OSPFDistanceLocalManagedObject *pOSPFDistanceMO);
			void deleteDistanceManagedObject(const OSPFRouteType &routeType);
			void addSummaryAddressManagedObject(OSPFSummaryAddressLocalManagedObject *pOSPFSummaryAddressMO);
			void deleteSummaryAddressManagedObject(const IpV4Address &address, const IpV4Address &mask);
			void addNeighborIPAddressManagedObject(OSPFNeighborIPAddressLocalManagedObject *pOSPFNeighborIPAddressMO);
			void deleteNeighborIPAddressManagedObject(const IpV4Address &address);
			void addPermitRedistributeManagedObject(OSPFPermitRedistributeLocalManagedObject *pOSPFPermitMO);
			void addDenyRedistributeManagedObject(OSPFDenyRedistributeLocalManagedObject *pOSPFDenyMO);
			void deletePermitDenyManagedObject(const UI8 &index);
            bool getAreaLocalManagedObject(string areaId);
    virtual string  customConfigurationDisplay(WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext);

            UI32 getMaxMetricLinkType() const;
            void setMaxMetricLinkType(const UI32& linkType);

            UI32 getMaxMetricOnStartupTime() const;
            void setMaxMetricOnStartupTime(const UI32& time );

            bool getMaxMetricOnStartupWaitForBgp() const;
            void setMaxMetricOnStartupWaitForBgp(const bool& waitforbgp );

            UI32 getExtMaxMetric() const;
            void setExtMaxMetric(const UI32& extMaxMetric );

             UI32 getSummMaxMetric() const;
             void setSummMaxMetric(const UI32& summMaxMetric );
            
            bool getExtMaxMetricLsa() const;
            void setExtMaxMetricLsa(const bool& extMaxMetricLsa );

            bool getSummMaxMetricLsa() const;
            void setSummMaxMetricLsa(const bool& summMaxMetricLsa );

            bool getRouterLsa() const;
            void setRouterLsa(const bool& routerLsa );

            bool getIsLinkAll() const;
            void setIsLinkAll(const bool& linkAll );

            bool getIsLinkPtp() const;
            void setIsLinkPtp(const bool& linkPtp );

            bool getIsLinkStub() const;
            void setIsLinkStub(const bool& linkStub );

            bool getIsLinkTransit() const;
            void setIsLinkTransit(const bool& linkTransit );
             bool getIsAllVrfMaxMetricSet() const;
             void setIsAllVrfMaxMetricSet(const bool& isAllVrf);

            bool getIsOnStartupLinkAll() const;
            void setIsOnStartupLinkAll(const bool& linkAll );

            bool getIsOnStartupLinkPtp() const;
            void setIsOnStartupLinkPtp(const bool& linkPtp );

            bool getIsOnStartupLinkStub() const;
            void setIsOnStartupLinkStub(const bool& linkStub );

            bool getIsOnStartupLinkTransit() const;
            void setIsOnStartupLinkTransit(const bool& linkTransit);
             //bool getIsOnStartupAllVrfMaxMetricSet() const;
            // void setIsOnStartupAllVrfMaxMetricSet(const bool& isAllVrf);


            UI32 getOnStartupExtMaxMetric() const;
            void setOnStartupExtMaxMetric(const UI32& extMaxMetric );

             UI32 getOnStartupSummMaxMetric() const;
             void setOnStartupSummMaxMetric(const UI32& summMaxMetric );
            
            bool getOnStartupExtMaxMetricLsa() const;
            void setOnStartupExtMaxMetricLsa(const bool& extMaxMetricLsa );

            bool getOnStartupSummMaxMetricLsa() const;
            void setOnStartupSummMaxMetricLsa(const bool& summMaxMetricLsa );

            void setIsOnStartupMaxMetricAllLSA(const bool &isMaxMetricAllLSA);
            bool getIsOnStartupMaxMetricAllLSA() const;

            bool getMaxMetricOnStartup() const;
            void setMaxMetricOnStartup(const bool& maxMetricOnStartup );
              
// Now the data members

        private:
        protected:
        public:
            bool  m_status;
            string m_vrf;
            IpV4Address  m_domainId;
            UI32  m_domainTag;
            UI32  m_extLSDBLimit;
            UI32  m_defaultMetric;
            bool  m_defaultPassiveInterface;
            OSPFDefInfoOrigmetricType  m_extMetricType;
            UI32  m_ReferenceBandwidth;
            bool  m_useActivePorts;
            UI32  m_databaseOverflowInterval;
            bool  m_defInfoOrig;
            bool  m_always;
            UI32  m_defInfoOrigMetric;
            OSPFDefInfoOrigmetricType  m_defInfoOrigMetricType;
            string  m_defInfoOrigRouteMap;
            bool  m_isNSSATranslator;
			bool  m_isNSSATranslatorPresence;
            bool  m_redistributeStatic;
            bool  m_redistributeConnected;
            bool  m_redistributeBgp;
            StringUC  m_redistributeStaticMapName;
            StringUC  m_redistributeConnectedMapName;
            StringUC  m_redistributeBgpMapName;
            bool  m_rfc1583Compatibility;
            bool m_rfc1583CompatibilityPresence;
            bool  m_rfc1587Compatibility;
            UI32 m_timerLsaGroupPacingInterval;
            UI32UC m_timerInitDelay;
            UI32UC m_timerHoldTime;
            UI32UC m_timerMaxHoldTime;
            StringUC  m_routeMap;
            bool  m_isRouteMapIncoming;
            bool  m_isMaxMetricAllLSA;
            bool  m_gresEnabled;
            bool  m_gracefulRestartHelper;
            UI32  m_gracefulRestartNotifyTime;
            UI32  m_gracefulRestartRestartTime;
			bool m_log_adjacency;
			bool m_log_dr_only;
			bool m_log_all;
			bool m_log_bad_packet;
			bool m_log_checksum;
			bool m_log_database;
			bool m_log_retransmit;
			bool  m_vrfLiteCapability;
            vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> >  m_area;
            vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> >  m_distance;
            vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> >  m_neighborList;
            vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> >  m_permitRedistribute;
            vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> >  m_denyRedistribute;
            vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> >  m_summary;
                       bool m_isAllVrfMaxMetricSet;
                       bool m_summMaxMetricLsa;
                       bool m_extMaxMetricLsa;
                       bool m_routerLsa;
                       UI32 m_summMaxMetric;
                       UI32 m_extMaxMetric;
                       UI32 m_maxMetricOnStartUpTime;
                       bool m_isMaxMetricOnStartupWaitForBgp;
                       bool m_linkAll;
                       bool m_linkPtp;
                       bool m_linkStub;
                       bool m_linkTransit;
                       bool m_isOnStartupMaxMetricAllLsa;
                       bool m_onStartupSummMaxMetricLsa;
                       bool m_onStartupExtMaxMetricLsa;
                       UI32 m_onStartupSummMaxMetric;
                       UI32 m_onStartupExtMaxMetric;
                       bool m_onStartupLinkAll;
                       bool m_onStartupLinkPtp;
                       bool m_onStartupLinkStub;
                      bool  m_onStartupLinkTransit;  
                      bool  m_maxMetricOnStartup;   

    };
}
#endif                                            //OSPFCONFIGLOCALMANAGEDOBJECT_H
