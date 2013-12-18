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

#include "OSPF/Local/OSPFConfigLocalManagedObject.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFAreaLocalManagedObject.h"
#include "OSPF/Local/OSPFDistanceLocalManagedObject.h"
#include "OSPF/Local/OSPFNeighborIPAddressLocalManagedObject.h"
#include "OSPF/Local/OSPFPermitRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFDenyRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFSummaryAddressLocalManagedObject.h"
#include "OSPF/Local/OSPFRouterConfigLocalMessage.h"
#include "ClientInterface/OSPF/OSPFRouterConfigMessage.h"
#include "ClientInterface/OSPF/OSPFPermitDenyRedistributeMessage.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"
#include "OSPF/Local/OSPFTypes.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFConfigLocalManagedObject::OSPFConfigLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
		{
			m_status = true;
			m_domainId.fromString("0.0.0.0");
			m_domainTag = 0;
			m_extLSDBLimit = 0;
			m_defaultMetric = 10;
			m_defaultPassiveInterface = false;
			m_extMetricType = OSPF_TYPE2;
			m_ReferenceBandwidth = 0;
			m_useActivePorts = false;
			m_databaseOverflowInterval = 0;
			m_defInfoOrig = false;
			m_always = false;
			m_defInfoOrigMetric = 0;
			m_defInfoOrigMetricType = OSPF_METRIC_NONE;
			m_defInfoOrigRouteMap = string("");
			m_isNSSATranslator = true;
			m_isNSSATranslatorPresence = false;
			m_redistributeStatic = false;
			m_redistributeConnected = false;
			m_redistributeBgp = false;
			m_redistributeStaticMapName.setIsUserConfigured (false);
			m_redistributeConnectedMapName.setIsUserConfigured (false);
			m_redistributeBgpMapName.setIsUserConfigured (false);
			m_rfc1583Compatibility = true;
			m_rfc1583CompatibilityPresence = false;
			m_rfc1587Compatibility = false;
			m_timerLsaGroupPacingInterval = 240;
			//m_timerInitDelay = 0;
            m_timerInitDelay.setIsUserConfigured (false);
			//m_timerHoldTime = 0;
            m_timerHoldTime.setIsUserConfigured (false);
			//m_timerMaxHoldTime = 0;
            m_timerMaxHoldTime.setIsUserConfigured (false);
			//m_routeMap = string("");
			m_isRouteMapIncoming = false;
			m_isMaxMetricAllLSA = false;
			m_gresEnabled = false;
			m_gracefulRestartHelper = false;
			m_gracefulRestartNotifyTime = 0;
			m_gracefulRestartRestartTime = 0;
            m_log_adjacency = 0;
            m_log_dr_only = 0;
            m_log_all = 0;
            m_log_bad_packet = 0;
            m_log_checksum = 0;
            m_log_database = 0;
            m_log_retransmit = 0;
            m_vrf = DEFAULT_VRF_NAME;
            m_vrfLiteCapability = false;
		m_maxMetricOnStartUpTime = 0;
		m_extMaxMetricLsa = false;
		m_summMaxMetricLsa = false;    
		m_routerLsa = false;    
		m_extMaxMetric = OSPF_DEFAULT_MAX_METRIC_EXT_METRIC;
		m_summMaxMetric = OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC;
	        m_isAllVrfMaxMetricSet = false;
		m_isMaxMetricOnStartupWaitForBgp = false;
                m_linkAll = false;
		m_linkPtp = false;
		m_linkStub = false;
                m_linkTransit = false;
                m_isOnStartupMaxMetricAllLsa = false;
                m_onStartupSummMaxMetricLsa = false;
                m_onStartupExtMaxMetricLsa = false;
                m_onStartupSummMaxMetric = OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC;
                m_onStartupExtMaxMetric = OSPF_DEFAULT_MAX_METRIC_EXT_METRIC;
                m_onStartupLinkAll = false;
                m_onStartupLinkPtp = false;
                m_onStartupLinkStub = false;
                m_onStartupLinkTransit = false;
                m_maxMetricOnStartup = false;
	}
#if 0
    
    OSFConfigLocalManagedObject::OSPFConfigLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const bool &status,const string &vrfName,const IpV4Address &domainId,const UI32 &domainTag,const UI32 &extLSDBLimit,const UI32 &defaultMetric,const OSPFDefInfoOrigmetricType &extMetricType,const UI32 &ReferenceBandwidth,const bool &useActivePorts,const UI32 &databaseOverflowInterval,const bool &defInfoOrig,const bool &always,const UI32 &defInfoOrigMetric,const OSPFDefInfoOrigmetricType &defInfoOrigMetricType,const string &defInfoOrigRouteMap, const bool &isNSSATranslator,const bool &redistributeStatic,const bool &redistributeConnected,const bool &redistributeBgp,const string &redistributeStaticMapName,const string &redistributeConnectedMapName,const string &redistributeBgpMapName,const bool &rfc1583Compatibility,const bool &rfc1587Compatibility,const UI32 &timerLsaGroupPacingInterval,const UI32 &timerInitDelay,const UI32 &timerHoldTime,const UI32 &timerMaxHoldTime,const string &routeMap,const bool &isRouteMapIncoming,const bool &isMaxMetricAllLSA,const bool &gresEnabled,const bool &gracefulRestartHelper,const UI32 &gracefulRestartNotifyTime,const UI32 &gracefulRestartRestartTime,const bool &log_adjacency,const bool &log_dr_only,const bool &log_all,const bool &log_bad_packet,const bool &log_checksum,const bool &log_database,const bool &log_retransmit,const vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> > &area,const vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> > &distance,const vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> > &neighborList,const vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> > &permitRedistribute,const vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> > &denyRedistribute,const vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> > &summary)
    OSPFConfigLocalManagedObject::OSPFConfigLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const bool &status,const string &vrfName,const IpV4Address &domainId,const UI32 &domainTag,const UI32 &extLSDBLimit,const UI32 &defaultMetric,const bool &defaultPassiveInterface,const OSPFDefInfoOrigmetricType &extMetricType,const UI32 &ReferenceBandwidth,const bool &useActivePorts,const UI32 &databaseOverflowInterval,const bool &defInfoOrig,const bool &always,const UI32 &defInfoOrigMetric,const OSPFDefInfoOrigmetricType &defInfoOrigMetricType,const string &defInfoOrigRouteMap, const bool &isNSSATranslator,const bool &redistributeStatic,const bool &redistributeConnected,const bool &redistributeBgp,const string &redistributeStaticMapName,const string &redistributeConnectedMapName,const string &redistributeBgpMapName,const bool &rfc1583Compatibility,const bool &rfc1587Compatibility,const UI32 &timerLsaGroupPacingInterval,const UI32 &timerInitDelay,const UI32 &timerHoldTime,const UI32 &timerMaxHoldTime,const string &routeMap,const bool &isRouteMapIncoming,const bool &isMaxMetricAllLSA,const bool &gresEnabled,const bool &gracefulRestartHelper,const UI32 &gracefulRestartNotifyTime,const UI32 &gracefulRestartRestartTime,const bool &log_adjacency,const bool &log_dr_only,const bool &log_all,const bool &log_bad_packet,const bool &log_checksum,const bool &log_database,const bool &log_retransmit,const vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> > &area,const vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> > &distance,const vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> > &neighborList,const vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> > &permitRedistribute,const vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> > &denyRedistribute,const vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> > &summary)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_status    (status),
        m_vrf		(vrfName),
        m_domainId    (domainId),
        m_domainTag    (domainTag),
        m_extLSDBLimit    (extLSDBLimit),
        m_defaultMetric    (defaultMetric),
        m_defaultPassiveInterface    (defaultPassiveInterface),
        m_extMetricType    (extMetricType),
        m_ReferenceBandwidth    (ReferenceBandwidth),
        m_useActivePorts    (useActivePorts),
        m_databaseOverflowInterval    (databaseOverflowInterval),
       m_defInfoOrig    (defInfoOrig),
        m_always    (always),
        m_defInfoOrigMetric    (defInfoOrigMetric),
        m_defInfoOrigMetricType    (defInfoOrigMetricType),
        m_defInfoOrigRouteMap    (defInfoOrigRouteMap),
        m_isNSSATranslator    (isNSSATranslator),
        m_redistributeStatic    (redistributeStatic),
        m_redistributeConnected    (redistributeConnected),
        m_redistributeBgp    (redistributeBgp),
        m_redistributeStaticMapName    (redistributeStaticMapName),
        m_redistributeConnectedMapName    (redistributeConnectedMapName),
        m_redistributeBgpMapName    (redistributeBgpMapName),
        m_rfc1583Compatibility    (rfc1583Compatibility),
        m_rfc1587Compatibility    (rfc1587Compatibility),
		m_timerLsaGroupPacingInterval(timerLsaGroupPacingInterval),
		m_timerInitDelay(timerInitDelay),
		m_timerHoldTime(timerHoldTime),
		m_timerMaxHoldTime(timerMaxHoldTime),
        m_routeMap    (routeMap),
        m_isRouteMapIncoming    (isRouteMapIncoming),
        m_isMaxMetricAllLSA (isMaxMetricAllLSA),
        m_gresEnabled (gresEnabled),
        m_gracefulRestartHelper    (gracefulRestartHelper),
        m_gracefulRestartNotifyTime    (gracefulRestartNotifyTime),
        m_gracefulRestartRestartTime    (gracefulRestartRestartTime),
        m_log_adjacency (log_adjacency),
        m_log_dr_only (log_dr_only),
        m_log_all (log_all),
        m_log_bad_packet (log_bad_packet),
        m_log_checksum (log_checksum),
        m_log_database (log_database),
        m_log_retransmit (log_retransmit),
        m_area    (area),
        m_distance    (distance),
        m_neighborList    (neighborList),
        m_permitRedistribute    (permitRedistribute),
        m_denyRedistribute    (denyRedistribute),
        m_summary    (summary)
    {
    	m_vrfLiteCapability = false;
	m_maxMetricOnStartupTime = 0;
	m_extMaxMetric = OSPF_DEFAULT_MAX_METRIC_EXT_METRIC;
	m_summMaxMetric = OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC;
	m_isAllVrfMaxMetricSet = false;
	m_isMaxMetricOnStartupWaitForBgp = false;
	m_maxMetricLinkType = 0;
    }
#endif
    OSPFConfigLocalManagedObject::~OSPFConfigLocalManagedObject ()
    {
    }

    string  OSPFConfigLocalManagedObject::getClassName()
    {
        return ("OSPFConfigLocalManagedObject");
    }

    void  OSPFConfigLocalManagedObject::setupAttributesForPersistence()
    {
		UI32 default_metric_type = OSPF_TYPE2;
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_status,"status", ospf_ospf));
        addPersistableAttribute (new AttributeString(&m_vrf,"vrf" ,ospf_vrf));
        //addPersistableAttribute (new AttributeIpV4Address(&m_domainId,IpV4Address(string("0.0.0.0")),"domainId", ospf_domain_id));
        //addPersistableAttribute (new AttributeUI32(&m_domainTag,0,"domainTag", ospf_domain_tag));
        addPersistableAttribute (new AttributeUI32(&m_extLSDBLimit,0,"extLSDBLimit", ospf_external_lsdb_limit));
        addPersistableAttribute (new AttributeUI32(&m_defaultMetric,0,"defaultMetric", ospf_default_metric));
        addPersistableAttribute (new AttributeBool(&m_defaultPassiveInterface,"defaultPassiveInterface", ospf_default_passive_interface));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_extMetricType,default_metric_type,"extMetricType" ,ospf_ospf_metric_type));
        addPersistableAttribute (new AttributeUI32(&m_ReferenceBandwidth,0,"ReferenceBandwidth", ospf_ref_bandwidth));
        addPersistableAttribute (new AttributeBool(&m_useActivePorts,"useActivePorts", ospf_use_active_ports));
        addPersistableAttribute (new AttributeUI32(&m_databaseOverflowInterval,0,"databaseOverflowInterval", ospf_database_overflow_interval));
        addPersistableAttribute (new AttributeBool(&m_defInfoOrig,"defInfoOrig", ospf_default_information_originate));
        addPersistableAttribute (new AttributeBool(&m_always,"always", ospf_always));
        addPersistableAttribute (new AttributeUI32(&m_defInfoOrigMetric,0,"defInfoOrigMetric", ospf_def_orig_metric));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_defInfoOrigMetricType,0,"defInfoOrigMetricType" ,ospf_def_orig_metric_type));
        addPersistableAttribute (new AttributeString(&m_defInfoOrigRouteMap,"defInfoOrigRouteMap" ,ospf_def_orig_route_map));
		addPersistableAttribute (new AttributeBool(&m_isNSSATranslatorPresence,"isNSSATranslator", ospf_nssa_translator));
        addPersistableAttribute (new AttributeBool(&m_isNSSATranslator,"isNSSATranslatorFlag", ospf_nssa_translator_flag));
        addPersistableAttribute (new AttributeBool(&m_redistributeStatic,"redistributeStatic", ospf_static));
        addPersistableAttribute (new AttributeBool(&m_redistributeConnected,"redistributeConnected", ospf_connected));
        addPersistableAttribute (new AttributeBool(&m_redistributeBgp,"redistributeBgp", ospf_bgp));
		addPersistableAttribute (new AttributeStringUC(&m_redistributeStaticMapName,true,"redistributeStaticMapName"));
		setUserTagForAttribute ("redistributeStaticMapName", ospf_static_route_map);
		addPersistableAttribute (new AttributeStringUC(&m_redistributeConnectedMapName,true,"redistributeConnectedMapName"));
		setUserTagForAttribute("redistributeConnectedMapName", ospf_connected_route_map);
		addPersistableAttribute (new AttributeStringUC(&m_redistributeBgpMapName,true,"redistributeBgpMapName"));
		setUserTagForAttribute("redistributeBgpMapName", ospf_bgp_route_map);
        addPersistableAttribute (new AttributeBool(&m_rfc1583CompatibilityPresence,"rfc1583Compatibility", ospf_rfc1583_compatibility));
        addPersistableAttribute (new AttributeBool(&m_rfc1583Compatibility,"rfc1583Compatibilityflag", ospf_rfc1583_compatibility_flag));
        //addPersistableAttribute (new AttributeBool(&m_rfc1587Compatibility,"rfc1587Compatibility", ospf_rfc1587_compatibility));
        addPersistableAttribute (new AttributeUI32(&m_timerLsaGroupPacingInterval,240,"timerLsaGroupPacingInterval", ospf_lsa_group_pacing));
        addPersistableAttribute (new AttributeUI32UC(&m_timerInitDelay,true,"timerInitDelay"));
        setUserTagForAttribute ("timerInitDelay",ospf_init_delay);
        addPersistableAttribute (new AttributeUI32UC(&m_timerHoldTime,true,"timerHoldTime"));
        setUserTagForAttribute ("timerHoldTime", ospf_hold_time);
        addPersistableAttribute (new AttributeUI32UC(&m_timerMaxHoldTime,true,"timerMaxHoldTime"));
        setUserTagForAttribute ("timerMaxHoldTime", ospf_max_hold_time);
        //addPersistableAttribute (new AttributeString(&m_routeMap,1,string(""),"routeMap", ospf_route_map));
        addPersistableAttribute (new AttributeStringUC(&m_routeMap,true,"routeMap"));
        setUserTagForAttribute ("routeMap", ospf_route_map);
        addPersistableAttribute (new AttributeBool(&m_isRouteMapIncoming,"isRouteMapIncoming", ospf_in));
        addPersistableAttribute (new AttributeBool(&m_routerLsa,"routerLsa", ospf_router_lsa));
        addPersistableAttribute (new AttributeBool(&m_isMaxMetricAllLSA,"isMaxMetricAllLSA", ospf_all_lsas));
#if 0        
        addPersistableAttribute (new AttributeBool(&m_gresEnabled,"gresEnabled", ospf_graceful_restart));
        addPersistableAttribute (new AttributeBool(&m_gracefulRestartHelper,"gracefulRestartHelper", ospf_helper_disable));
        addPersistableAttribute (new AttributeUI32(&m_gracefulRestartNotifyTime,0,"gracefulRestartNotifyTime", ospf_notify_time));
        addPersistableAttribute (new AttributeUI32(&m_gracefulRestartRestartTime,0,"gracefulRestartRestartTime", ospf_restart_time));
#endif        
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFAreaLocalManagedObject>(&m_area,"area", getClassName (), getObjectId (), OSPFAreaLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFDistanceLocalManagedObject>(&m_distance,"distance", getClassName (), getObjectId (), OSPFDistanceLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFNeighborIPAddressLocalManagedObject>(&m_neighborList,"neighborList", getClassName (), getObjectId (), OSPFNeighborIPAddressLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFPermitRedistributeLocalManagedObject>(&m_permitRedistribute,"permitRedistribute", getClassName (), getObjectId (), OSPFPermitRedistributeLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFDenyRedistributeLocalManagedObject>(&m_denyRedistribute,"denyRedistribute", getClassName (), getObjectId (), OSPFDenyRedistributeLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFSummaryAddressLocalManagedObject>(&m_summary,"summary", getClassName (), getObjectId (), OSPFSummaryAddressLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeBool(&m_log_adjacency,"log_adjacency", ospf_adjacency));
        addPersistableAttribute (new AttributeBool(&m_log_dr_only,"log_dr_only", ospf_dr_only));
        addPersistableAttribute (new AttributeBool(&m_log_all,"log_all", ospf_log_all));
        addPersistableAttribute (new AttributeBool(&m_log_bad_packet,"log_bad_packet", ospf_bad_packet));
        addPersistableAttribute (new AttributeBool(&m_log_checksum,"log_checksum", ospf_log_checksum));
        addPersistableAttribute (new AttributeBool(&m_log_database,"log_database", ospf_log_database));
        addPersistableAttribute (new AttributeBool(&m_log_retransmit,"log_retransmit", ospf_log_retransmit));
        addPersistableAttribute (new AttributeBool(&m_vrfLiteCapability,"vrf_lite_capability", ospf_vrf_lite_capability));
        addPersistableAttribute (new AttributeUI32(&m_maxMetricOnStartUpTime,0,"maxMetricOnStartUpTime",ospf_time));
        addPersistableAttribute (new AttributeBool(&m_extMaxMetricLsa, "extMaxMetricLsa",ospf_external_lsa));
        addPersistableAttribute (new AttributeBool(&m_summMaxMetricLsa, "summMaxMetricLsa",ospf_summary_lsa));
        addPersistableAttribute (new AttributeUI32(&m_extMaxMetric, "extMaxMetric",ospf_external_lsa_val));
        addPersistableAttribute (new AttributeUI32(&m_summMaxMetric, "summMaxMetric",ospf_summary_lsa_val));
        addPersistableAttribute (new AttributeBool(&m_isAllVrfMaxMetricSet, "isAllVrfMaxMetric",ospf_all_vrfs));
        addPersistableAttribute (new AttributeBool(&m_isMaxMetricOnStartupWaitForBgp, "isOnStartUpWaitForBgp",ospf_wait_for_bgp));
        //addPersistableAttribute (new AttributeEnum((UI32*)&m_maxMetricLinkType,0,"maxMetricLinkType",ospf_link));
        addPersistableAttribute (new AttributeBool(&m_linkAll,"linkAll", ospf_all));
        addPersistableAttribute (new AttributeBool(&m_linkPtp,"linkPtp", ospf_ptp));
        addPersistableAttribute (new AttributeBool(&m_linkStub,"linkStub", ospf_stub));
        addPersistableAttribute (new AttributeBool(&m_linkTransit,"linkTransit", ospf_transit));

        //addPersistableAttribute (new AttributeBool(&m_isOnStartupMaxMetricSetAllVrf,"isOnStartupMaxMetricAllVrf", ospf_all_vrfs_onstartup));
        addPersistableAttribute (new AttributeBool(&m_isOnStartupMaxMetricAllLsa,"isOnStartupMaxMetricAllLSA", ospf_all_lsas_onstartup));
        addPersistableAttribute (new AttributeBool(&m_onStartupSummMaxMetricLsa, "onStartupSummMaxMetricLsa",ospf_summary_lsa_onstartup));
        addPersistableAttribute (new AttributeBool(&m_onStartupExtMaxMetricLsa, "onStartupExtMaxMetricLsa",ospf_external_lsa_onstartup));
        addPersistableAttribute (new AttributeUI32(&m_onStartupSummMaxMetric, "onStartupSummMaxMetric",ospf_summary_lsa_val_onstartup));
        addPersistableAttribute (new AttributeUI32(&m_onStartupExtMaxMetric, "onStartupExtMaxMetric",ospf_external_lsa_val_onstartup));
        addPersistableAttribute (new AttributeBool(&m_onStartupLinkAll,"onStartupLinkAll", ospf_all_link_onstartup));
        addPersistableAttribute (new AttributeBool(&m_onStartupLinkPtp,"onStartupLinkPtp", ospf_ptp_link_onstartup));
        addPersistableAttribute (new AttributeBool(&m_onStartupLinkStub,"onStartupLinkStub", ospf_stub_link_onstartup));
        addPersistableAttribute (new AttributeBool(&m_onStartupLinkTransit,"onStartupLinkTransit", ospf_transit_link_onstartup));
        addPersistableAttribute (new AttributeBool(&m_maxMetricOnStartup,"maxMetricOnStartup"));
    }

    void  OSPFConfigLocalManagedObject::setupAttributesForCreate()
    {
		UI32 default_metric_type = OSPF_TYPE2;
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_status,"status", ospf_ospf));
        addPersistableAttributeForCreate  (new AttributeString(&m_vrf,"vrf" ,ospf_vrf));
        //addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_domainId,IpV4Address(string("0.0.0.0")),"domainId", ospf_domain_id));
        //addPersistableAttributeForCreate  (new AttributeUI32(&m_domainTag,0,"domainTag", ospf_domain_tag));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_extLSDBLimit,0,"extLSDBLimit", ospf_external_lsdb_limit));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_defaultMetric,0,"defaultMetric", ospf_default_metric));
        addPersistableAttributeForCreate  (new AttributeBool(&m_defaultPassiveInterface,"defaultPassiveInterface", ospf_default_passive_interface));
        addPersistableAttributeForCreate  (new AttributeEnum ((UI32 *)&m_extMetricType,default_metric_type,"extMetricType" ,ospf_ospf_metric_type));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_ReferenceBandwidth,0,"ReferenceBandwidth", ospf_ref_bandwidth));
        addPersistableAttributeForCreate  (new AttributeBool(&m_useActivePorts,"useActivePorts", ospf_use_active_ports));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_databaseOverflowInterval,0,"databaseOverflowInterval", ospf_database_overflow_interval));
        addPersistableAttributeForCreate  (new AttributeBool(&m_defInfoOrig,"defInfoOrig", ospf_default_information_originate));
        addPersistableAttributeForCreate  (new AttributeBool(&m_always,"always", ospf_always));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_defInfoOrigMetric,0,"defInfoOrigMetric", ospf_def_orig_metric));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_defInfoOrigMetricType,0,"defInfoOrigMetricType" ,ospf_def_orig_metric_type));
        addPersistableAttributeForCreate  (new AttributeString(&m_defInfoOrigRouteMap,"defInfoOrigRouteMap" ,ospf_def_orig_route_map));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isNSSATranslatorPresence,"isNSSATranslator", ospf_nssa_translator));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isNSSATranslator,"isNSSATranslatorFlag", ospf_nssa_translator_flag));
        addPersistableAttributeForCreate (new AttributeBool(&m_redistributeStatic,"redistributeStatic", ospf_static));
        addPersistableAttributeForCreate (new AttributeBool(&m_redistributeConnected,"redistributeConnected", ospf_connected));
        addPersistableAttributeForCreate (new AttributeBool(&m_redistributeBgp,"redistributeBgp", ospf_bgp));
		addPersistableAttributeForCreate (new AttributeStringUC(&m_redistributeStaticMapName,true,"redistributeStaticMapName"));
		addPersistableAttributeForCreate (new AttributeStringUC(&m_redistributeConnectedMapName,true,"redistributeConnectedMapName"));
		addPersistableAttributeForCreate (new AttributeStringUC(&m_redistributeBgpMapName,true,"redistributeBgpMapName"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_rfc1583CompatibilityPresence,"rfc1583Compatibility", ospf_rfc1583_compatibility));
        addPersistableAttributeForCreate  (new AttributeBool(&m_rfc1583Compatibility,"rfc1583Compatibilityflag", ospf_rfc1583_compatibility_flag));
        //addPersistableAttributeForCreate  (new AttributeBool(&m_rfc1587Compatibility,"rfc1587Compatibility", ospf_rfc1587_compatibility));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_timerLsaGroupPacingInterval,240,"timerLsaGroupPacingInterval", ospf_lsa_group_pacing));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_timerInitDelay,true,"timerInitDelay"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_timerHoldTime,true,"timerHoldTime"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_timerMaxHoldTime,true,"timerMaxHoldTime"));
        //addPersistableAttributeForCreate  (new AttributeString(&m_routeMap,1,string(""),"routeMap", ospf_route_map));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_routeMap,true,"routeMap"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isRouteMapIncoming,"isRouteMapIncoming", ospf_in));
        addPersistableAttributeForCreate  (new AttributeBool(&m_routerLsa,"routerLsa", ospf_router_lsa));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isMaxMetricAllLSA,"isMaxMetricAllLSA", ospf_all_lsas));
#if 0        
        addPersistableAttributeForCreate  (new AttributeBool(&m_gresEnabled,"gresEnabled", ospf_graceful_restart));
        addPersistableAttributeForCreate  (new AttributeBool(&m_gracefulRestartHelper,"gracefulRestartHelper", ospf_helper_disable));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_gracefulRestartNotifyTime,0,"gracefulRestartNotifyTime", ospf_notify_time));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_gracefulRestartRestartTime,0,"gracefulRestartRestartTime", ospf_restart_time));
#endif        
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFAreaLocalManagedObject>(&m_area,"area", getClassName (), getObjectId (), OSPFAreaLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFDistanceLocalManagedObject>(&m_distance,"distance", getClassName (), getObjectId (), OSPFDistanceLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFNeighborIPAddressLocalManagedObject>(&m_neighborList,"neighborList", getClassName (), getObjectId (), OSPFNeighborIPAddressLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFPermitRedistributeLocalManagedObject>(&m_permitRedistribute,"permitRedistribute", getClassName (), getObjectId (), OSPFPermitRedistributeLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFDenyRedistributeLocalManagedObject>(&m_denyRedistribute,"denyRedistribute", getClassName (), getObjectId (), OSPFDenyRedistributeLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFSummaryAddressLocalManagedObject>(&m_summary,"summary", getClassName (), getObjectId (), OSPFSummaryAddressLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate (new AttributeBool(&m_log_adjacency,"log_adjacency", ospf_adjacency));
        addPersistableAttributeForCreate (new AttributeBool(&m_log_dr_only,"log_dr_only", ospf_dr_only));
        addPersistableAttributeForCreate (new AttributeBool(&m_log_all,"log_all", ospf_log_all));
        addPersistableAttributeForCreate (new AttributeBool(&m_log_bad_packet,"log_bad_packet", ospf_bad_packet));
        addPersistableAttributeForCreate (new AttributeBool(&m_log_checksum,"log_checksum", ospf_log_checksum));
        addPersistableAttributeForCreate (new AttributeBool(&m_log_database,"log_database", ospf_log_database));
        addPersistableAttributeForCreate (new AttributeBool(&m_log_retransmit,"log_retransmit", ospf_log_retransmit));
        addPersistableAttributeForCreate (new AttributeBool(&m_vrfLiteCapability,"vrf_lite_capability", ospf_vrf_lite_capability));
        addPersistableAttributeForCreate (new AttributeUI32(&m_maxMetricOnStartUpTime, "maxMetricOnStartUpTime",ospf_time));
        addPersistableAttributeForCreate (new AttributeBool(&m_extMaxMetricLsa, "extMaxMetricLsa",ospf_external_lsa));
        addPersistableAttributeForCreate (new AttributeBool(&m_summMaxMetricLsa, "summMaxMetricLsa",ospf_summary_lsa));
        addPersistableAttributeForCreate (new AttributeUI32(&m_extMaxMetric, "extMaxMetric",ospf_external_lsa_val));
        addPersistableAttributeForCreate (new AttributeUI32(&m_summMaxMetric, "summMaxMetric",ospf_summary_lsa_val));
        addPersistableAttributeForCreate (new AttributeBool(&m_isAllVrfMaxMetricSet, "isAllVrfMaxMetric",ospf_all_vrfs));
        addPersistableAttributeForCreate (new AttributeBool(&m_isMaxMetricOnStartupWaitForBgp, "isOnStartUpWaitForBgp",ospf_wait_for_bgp));
        //addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_maxMetricLinkType, "maxMetricLinkType",ospf_link));
        addPersistableAttributeForCreate (new AttributeBool(&m_linkAll,"linkAll", ospf_all));
        addPersistableAttributeForCreate (new AttributeBool(&m_linkPtp,"linkPtp", ospf_ptp));
        addPersistableAttributeForCreate (new AttributeBool(&m_linkStub,"linkStub", ospf_stub));
        addPersistableAttributeForCreate (new AttributeBool(&m_linkTransit,"linkTransit", ospf_transit));


        //addPersistableAttributeForCreate (new AttributeBool(&m_isOnStartupMaxMetricSetAllVrf,"isOnStartupMaxMetricAllVrf", ospf_all_vrfs_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_isOnStartupMaxMetricAllLsa,"isOnStartupMaxMetricAllLSA", ospf_all_lsas_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_onStartupSummMaxMetricLsa, "onStartupSummMaxMetricLsa",ospf_summary_lsa_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_onStartupExtMaxMetricLsa, "onStartupExtMaxMetricLsa",ospf_external_lsa_onstartup));
        addPersistableAttributeForCreate (new AttributeUI32(&m_onStartupSummMaxMetric, "onStartupSummMaxMetric",ospf_summary_lsa_val_onstartup));
        addPersistableAttributeForCreate (new AttributeUI32(&m_onStartupExtMaxMetric, "onStartupExtMaxMetric",ospf_external_lsa_val_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_onStartupLinkAll,"onStartupLinkAll", ospf_all_link_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_onStartupLinkPtp,"onStartupLinkPtp", ospf_ptp_link_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_onStartupLinkStub,"onStartupLinkStub", ospf_stub_link_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_onStartupLinkTransit,"onStartupLinkTransit", ospf_transit_link_onstartup));
        addPersistableAttributeForCreate (new AttributeBool(&m_maxMetricOnStartup,"maxMetricOnStartup"));
    }

    void OSPFConfigLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("vrf");
        setUserDefinedKeyCombination (keyName);
    }

    void  OSPFConfigLocalManagedObject::setStatus(const bool &status)
    {
        m_status  =  status;
    }

    bool OSPFConfigLocalManagedObject::getStatus() const
    {
        return (m_status);
    }

    void  OSPFConfigLocalManagedObject::setDomainId(const IpV4Address &domainId)
    {
        m_domainId  =  domainId;
    }

    IpV4Address  OSPFConfigLocalManagedObject::getDomainId() const
    {
        return (m_domainId);
    }

    void  OSPFConfigLocalManagedObject::setDomainTag(const UI32 &domainTag)
    {
        m_domainTag  =  domainTag;
    }

    UI32  OSPFConfigLocalManagedObject::getDomainTag() const
    {
        return (m_domainTag);
    }

    void  OSPFConfigLocalManagedObject::setExtLSDBLimit(const UI32 &extLSDBLimit)
    {
        m_extLSDBLimit  =  extLSDBLimit;
    }

    UI32  OSPFConfigLocalManagedObject::getExtLSDBLimit() const
    {
        return (m_extLSDBLimit);
    }
	
    void  OSPFConfigLocalManagedObject::setDefaultPassiveInterface(const bool &defaultPassiveInterface)
    {
        m_defaultPassiveInterface  =  defaultPassiveInterface;
    }

    bool  OSPFConfigLocalManagedObject::getDefaultPassiveInterface() const
    {
        return (m_defaultPassiveInterface);
    }
    void  OSPFConfigLocalManagedObject::setDefaultMetric(const UI32 &defaultMetric)
    {
        m_defaultMetric  =  defaultMetric;
    }

    UI32  OSPFConfigLocalManagedObject::getDefaultMetric() const
    {
        return (m_defaultMetric);
    }

    void  OSPFConfigLocalManagedObject::setExtMetricType(const OSPFDefInfoOrigmetricType &extMetricType)
    {
        m_extMetricType  =  extMetricType;
    }

    OSPFDefInfoOrigmetricType OSPFConfigLocalManagedObject::getExtMetricType() const
    {
        return (m_extMetricType);
    }

    void  OSPFConfigLocalManagedObject::setReferenceBandwidth(const UI32 &ReferenceBandwidth)
    {
        m_ReferenceBandwidth  =  ReferenceBandwidth;
    }

    UI32  OSPFConfigLocalManagedObject::getReferenceBandwidth() const
    {
        return (m_ReferenceBandwidth);
    }

    void  OSPFConfigLocalManagedObject::setUseActivePorts(const bool &useActivePorts)
    {
        m_useActivePorts  =  useActivePorts;
    }

    bool  OSPFConfigLocalManagedObject::getUseActivePorts() const
    {
        return (m_useActivePorts);
    }

    void  OSPFConfigLocalManagedObject::setDatabaseOverflowInterval(const UI32 &databaseOverflowInterval)
    {
        m_databaseOverflowInterval  =  databaseOverflowInterval;
    }

    UI32  OSPFConfigLocalManagedObject::getDatabaseOverflowInterval() const
    {
        return (m_databaseOverflowInterval);
    }

    void  OSPFConfigLocalManagedObject::setDefInfoOrig(const bool &defInfoOrig)
    {
        m_defInfoOrig  =  defInfoOrig;
    }

    bool  OSPFConfigLocalManagedObject::getDefInfoOrig() const
    {
        return (m_defInfoOrig);
    }

    void  OSPFConfigLocalManagedObject::setAlways(const bool &always)
    {
        m_always  =  always;
    }

    bool  OSPFConfigLocalManagedObject::getAlways() const
    {
        return (m_always);
    }

    void  OSPFConfigLocalManagedObject::setDefInfoOrigMetric(const UI32 &defInfoOrigMetric)
    {
        m_defInfoOrigMetric  =  defInfoOrigMetric;
    }

    UI32  OSPFConfigLocalManagedObject::getDefInfoOrigMetric() const
    {
        return (m_defInfoOrigMetric);
    }

    void  OSPFConfigLocalManagedObject::setDefInfoOrigMetricType(const OSPFDefInfoOrigmetricType &defInfoOrigMetricType)
    {
        m_defInfoOrigMetricType  =  defInfoOrigMetricType;
    }

    OSPFDefInfoOrigmetricType OSPFConfigLocalManagedObject::getDefInfoOrigMetricType() const
    {
        return (m_defInfoOrigMetricType);
    }

    void  OSPFConfigLocalManagedObject::setDefInfoOrigRouteMap(const string &defInfoOrigRouteMap)
    {
        m_defInfoOrigRouteMap  =  defInfoOrigRouteMap;
    }

    void  OSPFConfigLocalManagedObject::setVRF(const string &vrfName)
    {
        m_vrf  =  vrfName;
    }

    string OSPFConfigLocalManagedObject::getVRF() const
    {
        return (m_vrf);
    }

    string OSPFConfigLocalManagedObject::getDefInfoOrigRouteMap() const
    {
        return (m_defInfoOrigRouteMap);
    }

    void  OSPFConfigLocalManagedObject::setIsNSSATranslator(const bool &isNSSATranslator)
    {
        m_isNSSATranslator  =  isNSSATranslator;
		if (m_isNSSATranslator) {
			m_isNSSATranslatorPresence = false;
		} else {
			m_isNSSATranslatorPresence = true;
		}
    }

    bool  OSPFConfigLocalManagedObject::getIsNSSATranslator() const
    {
        return (m_isNSSATranslator);
    }

    void  OSPFConfigLocalManagedObject::setRedistributeStatic(const bool &redistributeStatic)
    {
        m_redistributeStatic  =  redistributeStatic;
    }

    bool  OSPFConfigLocalManagedObject::getRedistributeStatic() const
    {
        return (m_redistributeStatic);
    }

    void  OSPFConfigLocalManagedObject::setRedistributeConnected(const bool &redistributeConnected)
    {
        m_redistributeConnected  =  redistributeConnected;
    }

    bool  OSPFConfigLocalManagedObject::getRedistributeConnected() const
    {
        return (m_redistributeConnected);
    }

    void  OSPFConfigLocalManagedObject::setRedistributeBgp(const bool &redistributeBgp)
    {
        m_redistributeBgp  =  redistributeBgp;
    }

    bool  OSPFConfigLocalManagedObject::getRedistributeBgp() const
    {
        return (m_redistributeBgp);
    }

    void  OSPFConfigLocalManagedObject::setRedistributeStaticMapName(const string &redistributeStaticMapName, const bool &set)
    {
		m_redistributeStaticMapName.setStringValue(redistributeStaticMapName);
		m_redistributeStaticMapName.setIsUserConfigured(set);
    }

    string  OSPFConfigLocalManagedObject::getRedistributeStaticMapName() const
    {
		return (m_redistributeStaticMapName.getStringValue());
    }

    void  OSPFConfigLocalManagedObject::setRedistributeConnectedMapName(const string &redistributeConnectedMapName, const bool &set)
    {
		m_redistributeConnectedMapName.setStringValue(redistributeConnectedMapName);
		m_redistributeConnectedMapName.setIsUserConfigured(set);
    }

    string  OSPFConfigLocalManagedObject::getRedistributeConnectedMapName() const
    {
		return (m_redistributeConnectedMapName.getStringValue());
    }

    void  OSPFConfigLocalManagedObject::setRedistributeBgpMapName(const string &redistributeBgpMapName, const bool &set)
    {
		m_redistributeBgpMapName.setStringValue(redistributeBgpMapName);
		m_redistributeBgpMapName.setIsUserConfigured(set);
    }

    string  OSPFConfigLocalManagedObject::getRedistributeBgpMapName() const
    {
		return (m_redistributeBgpMapName.getStringValue());
    }

    void  OSPFConfigLocalManagedObject::setRfc1583Compatibility(const bool &rfc1583Compatibility)
    {
        m_rfc1583Compatibility = rfc1583Compatibility;
        if(rfc1583Compatibility)
            m_rfc1583CompatibilityPresence = false;
        else
            m_rfc1583CompatibilityPresence = true;
    }

    bool  OSPFConfigLocalManagedObject::getRfc1583Compatibility() const
    {
        return (m_rfc1583Compatibility);
    }

    void  OSPFConfigLocalManagedObject::setRfc1587Compatibility(const bool &rfc1587Compatibility)
    {
        m_rfc1587Compatibility  =  rfc1587Compatibility;
    }

    bool  OSPFConfigLocalManagedObject::getRfc1587Compatibility() const
    {
        return (m_rfc1587Compatibility);
    }

	UI32 OSPFConfigLocalManagedObject::getLsaGroupPacingInterval() const
	{
		return (m_timerLsaGroupPacingInterval);
	}

	void OSPFConfigLocalManagedObject::setLsaGroupPacingInterval(const UI32 &timerLsaGroupPacingInterval)
	{
		m_timerLsaGroupPacingInterval = timerLsaGroupPacingInterval;
	}

	void OSPFConfigLocalManagedObject::setTimerInitDelay(const UI32 &timerInitDelay, const bool &set)
	{
		//m_timerInitDelay = timerInitDelay;
        m_timerInitDelay.setUI32Value(timerInitDelay);
        m_timerInitDelay.setIsUserConfigured(set);
	}

	UI32 OSPFConfigLocalManagedObject::getTimerInitDelay() const
	{
		return (m_timerInitDelay.getUI32Value());
	}

	bool OSPFConfigLocalManagedObject::getTimerInitDelayIsConfigured() const
	{
		return (m_timerInitDelay.getIsUserConfigured());
	}

	void OSPFConfigLocalManagedObject::setTimerHoldTime(const UI32 &timerHoldTime, const bool &set)
	{
		//m_timerHoldTime = timerHoldTime;
        m_timerHoldTime.setUI32Value(timerHoldTime);
        m_timerHoldTime.setIsUserConfigured(set);
	}

	UI32 OSPFConfigLocalManagedObject::getTimerHoldTime() const
	{
		return (m_timerHoldTime.getUI32Value());
	}

	bool OSPFConfigLocalManagedObject::getTimerHoldTimeIsConfigured() const
	{
		return (m_timerHoldTime.getIsUserConfigured());
	}

	void OSPFConfigLocalManagedObject::setTimerMaxHoldTime(const UI32 &timerMaxHoldTime, const bool &set)
	{
		//m_timerMaxHoldTime = timerMaxHoldTime;
        m_timerMaxHoldTime.setUI32Value(timerMaxHoldTime);
        m_timerMaxHoldTime.setIsUserConfigured(set);
	}

	UI32 OSPFConfigLocalManagedObject::getTimerMaxHoldTime() const
	{
		return (m_timerMaxHoldTime.getUI32Value());
	}

	bool OSPFConfigLocalManagedObject::getTimerMaxHoldTimeIsConfigured() const
	{
		return (m_timerMaxHoldTime.getIsUserConfigured());
	}

    void  OSPFConfigLocalManagedObject::setRouteMap(const string &routeMap, const bool &set)
    {
        //m_routeMap  =  routeMap;
        m_routeMap.setStringValue(routeMap);
        m_routeMap.setIsUserConfigured(set);
    }

    string  OSPFConfigLocalManagedObject::getRouteMap() const
    {
        return (m_routeMap.getStringValue());
    }

    void OSPFConfigLocalManagedObject::setIsRouteMapIncoming(const bool &routeMapIncoming)
    {
        m_isRouteMapIncoming = routeMapIncoming;
    }


    bool OSPFConfigLocalManagedObject::getIsRouteMapIncoming() const
    {
        return (m_isRouteMapIncoming);
    }
    void OSPFConfigLocalManagedObject::setIsMaxMetricAllLSA(const bool &isMaxMetricAllLSA)
    {
        m_isMaxMetricAllLSA = isMaxMetricAllLSA;
    }

    bool OSPFConfigLocalManagedObject::getIsMaxMetricAllLSA() const
    {
        return (m_isMaxMetricAllLSA);
    }

    void OSPFConfigLocalManagedObject::setGresEnabled(const bool &gresEnabled)
    {
        m_gresEnabled = gresEnabled;
    }

    bool OSPFConfigLocalManagedObject::getGresEnabled() const
    {
        return (m_gresEnabled);
    }

    void  OSPFConfigLocalManagedObject::setGracefulRestartHelper(const bool &gracefulRestartHelper)
    {
        m_gracefulRestartHelper  =  gracefulRestartHelper;
    }

    bool  OSPFConfigLocalManagedObject::getGracefulRestartHelper() const
    {
        return (m_gracefulRestartHelper);
    }

    void  OSPFConfigLocalManagedObject::setGracefulRestartNotifyTime(const UI32 &gracefulRestartNotifyTime)
    {
        m_gracefulRestartNotifyTime  =  gracefulRestartNotifyTime;
    }

    UI32  OSPFConfigLocalManagedObject::getGracefulRestartNotifyTime() const
    {
        return (m_gracefulRestartNotifyTime);
    }

    void  OSPFConfigLocalManagedObject::setGracefulRestartRestartTime(const UI32 &gracefulRestartRestartTime)
    {
        m_gracefulRestartRestartTime  =  gracefulRestartRestartTime;
    }

    UI32  OSPFConfigLocalManagedObject::getGracefulRestartRestartTime() const
    {
        return (m_gracefulRestartRestartTime);
    }

    bool OSPFConfigLocalManagedObject::getLogAdjacency() const
    {
        return(m_log_adjacency);
    }

    void OSPFConfigLocalManagedObject::setLogAdjacency(const bool &log_adjacency)
    {
        m_log_adjacency = log_adjacency;
    }

    bool OSPFConfigLocalManagedObject::getLogDROnly() const
    {
        return(m_log_dr_only);
    }

    void OSPFConfigLocalManagedObject::setLogDROnly(const bool &log_dr_only)
    {
        m_log_dr_only = log_dr_only;
    }

    bool OSPFConfigLocalManagedObject::getLogAll() const
    {
        return(m_log_all);
    }

    void OSPFConfigLocalManagedObject::setLogAll(const bool &log_all)
    {
        m_log_all = log_all;
    }

    bool OSPFConfigLocalManagedObject::getLogBadPacket() const
    {
        return(m_log_bad_packet);
    }

    void OSPFConfigLocalManagedObject::setLogBadPacket(const bool &log_bad_packet)
    {
        m_log_bad_packet = log_bad_packet;
    }

    bool OSPFConfigLocalManagedObject::getLogChecksum() const
    {
        return(m_log_checksum);
    }

    void OSPFConfigLocalManagedObject::setLogChecksum(const bool &log_checksum)
    {
        m_log_checksum = log_checksum;
    }

    bool OSPFConfigLocalManagedObject::getLogDatabase() const
    {
        return(m_log_database);
    }

    void OSPFConfigLocalManagedObject::setLogDatabase(const bool &log_database)
    {
        m_log_database = log_database;
    }

    bool OSPFConfigLocalManagedObject::getLogRetransmit() const
    {
        return(m_log_retransmit);
    }

    void OSPFConfigLocalManagedObject::setLogRetransmit(const bool &log_retransmit)
    {
        m_log_retransmit = log_retransmit;
    }

    bool OSPFConfigLocalManagedObject::getVrfLiteCapability() const
    {
        return(m_vrfLiteCapability);
    }

    void OSPFConfigLocalManagedObject::setVrfLiteCapability(const bool &vrfLiteCapability)
    {
    	m_vrfLiteCapability = vrfLiteCapability;
    }

    void  OSPFConfigLocalManagedObject::setArea(const vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> > &area)
    {
        m_area  =  area;
    }

    vector<WaveManagedObjectPointer<OSPFAreaLocalManagedObject> >  OSPFConfigLocalManagedObject::getArea() const
    {
        return (m_area);
    }

    bool OSPFConfigLocalManagedObject::getRouterLsa() const
    {
       return (m_routerLsa); 
    }

    void OSPFConfigLocalManagedObject::setRouterLsa(const bool& routerLsa )
    {
        m_routerLsa = routerLsa;
    }

    bool OSPFConfigLocalManagedObject::getMaxMetricOnStartup() const
    {
         return (m_maxMetricOnStartup);
    }

    void OSPFConfigLocalManagedObject::setMaxMetricOnStartup(const bool& maxMetricOnStartup )
    {
           m_maxMetricOnStartup = maxMetricOnStartup;
    }

    bool OSPFConfigLocalManagedObject::getAreaLocalManagedObject(string areaId) 
    {
        for (UI32 i = 0; i < m_area.size(); i++) {
            if ( (m_area[i])->getAreaId() == areaId )
                return 1;
        }        
            
        trace (TRACE_LEVEL_DEBUG, "getAreaLocalManagedObject: 0");
        return 0;
    }
    
    void  OSPFConfigLocalManagedObject::setAreaLocalMO(OSPFAreaLocalManagedObject *areaMO)
    {
        WaveManagedObjectPointer<OSPFAreaLocalManagedObject> areaLocalMO (areaMO);
        m_area.push_back(areaLocalMO);
    }

    UI32 OSPFConfigLocalManagedObject::getAreaCount()
    {
        return m_area.size();
    }

    void OSPFConfigLocalManagedObject::deleteAreaLocalMO(OSPFAreaLocalManagedObject *areaMO)
    {
        trace (TRACE_LEVEL_DEBUG, "Entering OSPFConfigLocalManagedObject::deleteAreaLocalMO");
        for (UI32 i = 0; i < m_area.size(); i++) {
            if ((m_area[i])->getAreaId() == areaMO->getAreaId()) {
                m_area.erase(m_area.begin() + i);
            }
        }
    }

    void  OSPFConfigLocalManagedObject::setDistance(const vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> > &distance)
    {
        m_distance  =  distance;
    }

    vector<WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> >  OSPFConfigLocalManagedObject::getDistance() const
    {
        return (m_distance);
    }

    void  OSPFConfigLocalManagedObject::setNeighborList(const vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> > &neighborList)
    {
        m_neighborList  =  neighborList;
    }

    vector<WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> >  OSPFConfigLocalManagedObject::getNeighborList() const
    {
        return (m_neighborList);
    }

    void  OSPFConfigLocalManagedObject::setPermitRedistribute(const vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> > &permitRedistribute)
    {
        m_permitRedistribute  =  permitRedistribute;
    }

    vector<WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> >  OSPFConfigLocalManagedObject::getPermitRedistribute() const
    {
        return (m_permitRedistribute);
    }

    void  OSPFConfigLocalManagedObject::setDenyRedistribute(const vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> > &denyRedistribute)
    {
        m_denyRedistribute  =  denyRedistribute;
    }

    vector<WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> >  OSPFConfigLocalManagedObject::getDenyRedistribute() const
    {
        return (m_denyRedistribute);
    }

    void  OSPFConfigLocalManagedObject::setSummary(const vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> > &summary)
    {
        m_summary  =  summary;
    }

    vector<WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> >  OSPFConfigLocalManagedObject::getSummary() const
    {
        return (m_summary);
    }

	void OSPFConfigLocalManagedObject::addDistanceManagedObject(OSPFDistanceLocalManagedObject *pOSPFDistanceMO)
	{
		trace(TRACE_LEVEL_DEVEL, "OSPFConfigLocalManagedObject::addDistanceManagedObject updating");
		WaveManagedObjectPointer<OSPFDistanceLocalManagedObject> OSPFDistance (pOSPFDistanceMO);
		m_distance.push_back(OSPFDistance);
	}

	void OSPFConfigLocalManagedObject::deleteDistanceManagedObject(const OSPFRouteType &routeType)
	{
		trace (TRACE_LEVEL_DEBUG, "Entering OSPFConfigLocalManagedObject::deleteDistanceManagedObject");
		for (UI32 i = 0; i < m_distance.size(); i++) {
			if ((m_distance[i])->getDistanceType() == routeType) {
				m_distance.erase (m_distance.begin() + i);
			}
		}
	}

	void OSPFConfigLocalManagedObject::addSummaryAddressManagedObject(OSPFSummaryAddressLocalManagedObject *pOSPFSummaryAddressMO)
	{
		WaveManagedObjectPointer<OSPFSummaryAddressLocalManagedObject> OSPFSummaryAddress (pOSPFSummaryAddressMO);
		m_summary.push_back(OSPFSummaryAddress);
	}

	void OSPFConfigLocalManagedObject::deleteSummaryAddressManagedObject(const IpV4Address &address, const IpV4Address &mask)
	{
		trace (TRACE_LEVEL_DEBUG, "Entering OSPFConfigLocalManagedObject::deleteSummaryAddressManagedObject");
		for (UI32 i = 0; i < m_summary.size(); i++) {
			if (((m_summary[i])->getSummaryAddress() == address) && ((m_summary[i])->getSummaryMask() == mask)) {
				m_summary.erase (m_summary.begin() + i);
			}
		}
	}

	void OSPFConfigLocalManagedObject::addNeighborIPAddressManagedObject(OSPFNeighborIPAddressLocalManagedObject *pOSPFNeighborIPAddressMO)
	{
		WaveManagedObjectPointer<OSPFNeighborIPAddressLocalManagedObject> OSPFNeighborIPAddress(pOSPFNeighborIPAddressMO);
		m_neighborList.push_back(OSPFNeighborIPAddress);
	}

	void OSPFConfigLocalManagedObject::deleteNeighborIPAddressManagedObject(const IpV4Address &address)
	{
		trace (TRACE_LEVEL_DEBUG, "Entering OSPFConfigLocalManagedObject::deleteNeighborIPAddressManagedObject");
		for (UI32 i = 0; i < m_neighborList.size(); i++) {
			if ((m_neighborList[i])->getNeighborAddress() == address) {
				m_neighborList.erase(m_neighborList.begin() + i);
			}
		}
	}

	void OSPFConfigLocalManagedObject::addPermitRedistributeManagedObject(OSPFPermitRedistributeLocalManagedObject *pOSPFPermitRedistributeMO)
	{
		WaveManagedObjectPointer<OSPFPermitRedistributeLocalManagedObject> OSPFPermitRedistribute(pOSPFPermitRedistributeMO);
		m_permitRedistribute.push_back(OSPFPermitRedistribute);
	}

	void OSPFConfigLocalManagedObject::addDenyRedistributeManagedObject(OSPFDenyRedistributeLocalManagedObject *pOSPFDenyRedistributeMO)
	{
		WaveManagedObjectPointer<OSPFDenyRedistributeLocalManagedObject> OSPFDenyRedistribute(pOSPFDenyRedistributeMO);
		m_denyRedistribute.push_back(OSPFDenyRedistribute);
	}
    void OSPFConfigLocalManagedObject::setIsLinkAll(const bool &linkAll)
    {
        m_linkAll = linkAll;
    }

    bool OSPFConfigLocalManagedObject::getIsLinkAll() const
    {
        return (m_linkAll);
    }
    void OSPFConfigLocalManagedObject::setIsLinkPtp(const bool &linkPtp)
    {
        m_linkPtp = linkPtp;
    }

    bool OSPFConfigLocalManagedObject::getIsLinkPtp() const
    {
        return (m_linkPtp);
    }
    void OSPFConfigLocalManagedObject::setIsLinkStub(const bool &linkStub)
    {
        m_linkStub = linkStub;
    }

    bool OSPFConfigLocalManagedObject::getIsLinkStub() const
    {
        return (m_linkStub);
    }
    void OSPFConfigLocalManagedObject::setIsLinkTransit(const bool &linkTransit/*,const bool &isUserConfigured*/)
    {
          m_linkTransit = linkTransit;
    }

    bool OSPFConfigLocalManagedObject::getIsLinkTransit() const
    {
        return (m_linkTransit);
    }

	void OSPFConfigLocalManagedObject::deletePermitDenyManagedObject(const UI8 &index)
	{
		UI32 i = 0;
		trace (TRACE_LEVEL_DEBUG, "Entering OSPFConfigLocalManagedObject::deletePermitDenyManagedObject");

		for (i = 0; i < m_permitRedistribute.size(); i++) {
			if ((m_permitRedistribute[i])->getIndex() == index) {
				m_permitRedistribute.erase(m_permitRedistribute.begin() + i);
			}
		}

		for (i = 0; i < m_denyRedistribute.size(); i++) {
			if ((m_denyRedistribute[i])->getIndex() == index) {
				m_denyRedistribute.erase(m_denyRedistribute.begin() + i);
			}
		}
	}

	void OSPFConfigLocalManagedObject::updateMOFromLocalMessage(OSPFRouterConfigLocalMessage *pMsg)
	{
                 bool onStartupCmd = false;
		setVRF(pMsg->getVRF());
		setUserSpecifiedPartitionName(pMsg->getPartitionName());
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage: VRF Name: ") + pMsg->getVRF());
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage: Partition Name: ") + pMsg->getPartitionName());
		switch (pMsg->getOpCode())
		{
			case OSPF_CREATE:
			{
				setStatus(true);
			}
			case OSPF_SET_ELEM:
			{
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_REF_BW_VAL:
						setReferenceBandwidth(pMsg->getReferenceBandwidth());
						break;
					case OSPF_ROUTER_CMD_REF_BW_USE_ACTIVE_PORTS:
						setUseActivePorts(pMsg->getRefBwUseActivePorts());
						break;
					case OSPF_ROUTER_CMD_DB_OVERFLOW_INTERVAL:
						setDatabaseOverflowInterval(pMsg->getDatabaseOverflowInterval());
						break;
					case OSPF_ROUTER_CMD_DEF_INFO_ORIGINATE:
						setDefInfoOrig(true);

						if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_ALWAYS) {
							setAlways(pMsg->getDefInfoOrigAlways());
						}
						if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_METRIC) {
							setDefInfoOrigMetric(pMsg->getDefInfoOrigMetric());
						}
						if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_METRIC_TYPE) {
							setDefInfoOrigMetricType(pMsg->getDefInfoOrigMetricType());
						}
						if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_ROUTE_MAP) {
							setDefInfoOrigRouteMap(pMsg->getDefInfoOrigRouteMap());
						}
						break;
					case OSPF_ROUTER_CMD_DEFAULT_METRIC:
						setDefaultMetric(pMsg->getDefaultMetric());
						break;
					case OSPF_ROUTER_CMD_DEFAULT_PASSIVE_INTF:
						setDefaultPassiveInterface(pMsg->getDefaultPassiveInterface());
						break;
					case OSPF_ROUTER_CMD_DISTRIBUTE_LIST_ROUTE_MAP:
						setRouteMap(pMsg->getDistListRouteMap(), true);
						setIsRouteMapIncoming(true);
						break;
					case OSPF_ROUTER_CMD_DOMAIN_ID:
						setDomainId(pMsg->getDomainId());
						break;
					case OSPF_ROUTER_CMD_DOMAIN_TAG:
						setDomainTag(pMsg->getDomainTag());
						break;
					case OSPF_ROUTER_CMD_EXTERNAL_LSDB_LIMIT:
						setExtLSDBLimit(pMsg->getExtLSDBLimit());
						break;
					case OSPF_ROUTER_CMD_GRES:
						setGresEnabled(true);
						break;
					case OSPF_ROUTER_CMD_GRES_HELPER_DISABLE:
						setGracefulRestartHelper(pMsg->getGracefulRestartHelper());
						setGresEnabled(true);
						break;
					case OSPF_ROUTER_CMD_GRES_NOTIFY_TIME:
						setGracefulRestartNotifyTime(pMsg->getGracefulRestartNotifyTime());
						setGresEnabled(true);
						break;
					case OSPF_ROUTER_CMD_GRES_RESTART_TIME:
						setGracefulRestartRestartTime(pMsg->getGracefulRestartRestartTime());
						setGresEnabled(true);
						break;
					case OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:
	                                 	trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: ") );
                                                if((!m_routerLsa)/*||(pMsg->getCmdType()==0)*/){
                                                  
                                                    if(!pMsg->getOnStartup())
                                                    {
                                                        if(!(pMsg->getCmdType()&OSPF_SET_MAX_METRIC_LINK_TYPE))
                                                             setIsLinkTransit(true);
                                                    }
                                                    else 
                                                    {
                                                        setMaxMetricOnStartup(true);     
                                                        if(!(pMsg->getCmdType()&OSPF_SET_MAX_METRIC_LINK_TYPE))
                                                            setIsOnStartupLinkTransit(true);
                                                    }
                                                                
						setRouterLsa(true);
                                                }

                                                else
                                                {
                                                  if((!m_maxMetricOnStartup)&&(pMsg->getOnStartup()))
                                                  {
                                                               trace(TRACE_LEVEL_INFO,"Configuring max metric onsatrtup" );
                                                      if(!(pMsg->getCmdType()& OSPF_SET_MAX_METRIC_LINK_TYPE))
                                                          {
                                                               trace(TRACE_LEVEL_INFO,string("link not set in msg")+pMsg->getCmdType());
                                                          setIsOnStartupLinkTransit(true/*,true*/);
                                                          }
                                                      setMaxMetricOnStartup(true);     
                                                  }
                                                  else if ((m_maxMetricOnStartup)&&(pMsg->getOnStartup())&&(!(pMsg->getCmdType()&OSPF_SET_MAX_METRIC_LINK_TYPE))&&
                                                       (!m_onStartupLinkPtp)&&(!m_onStartupLinkStub)&&(!m_onStartupLinkTransit))
                                                          setIsOnStartupLinkTransit(true/*,true*/);
                                                  else if ((m_routerLsa)&&(!pMsg->getOnStartup())&&(!(pMsg->getCmdType()&OSPF_SET_MAX_METRIC_LINK_TYPE))&&
                                                       (!m_linkPtp)&&(!m_linkStub)&&(!m_linkTransit))
                                                           setIsLinkTransit(true/*,true*/);
                                                }
                                                if(pMsg->getOnStartup()) {
                                                     onStartupCmd = true; 
                                                      if((pMsg->getCmdType() !=OSPF_SET_MAX_METRIC_STARTUP_TIME)&&((pMsg->getCmdType())&OSPF_SET_MAX_METRIC_STARTUP_TIME))
                                                      {              
                                                             trace(TRACE_LEVEL_INFO,string("time with another option"));                                     
                                                           if(pMsg->getMaxMetricOnStartupTime())
                                                          {
                                                               if(!getMaxMetricOnStartupTime())
                                                                {
							setMaxMetricOnStartupTime(pMsg->getMaxMetricOnStartupTime());
							setMaxMetricOnStartupWaitForBgp(false);

                                                                }
                                                          }
                                                       }
                                                      if((pMsg->getCmdType() !=OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP)&&((pMsg->getCmdType())&OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP))
                                                      {
                                                             trace(TRACE_LEVEL_INFO,string("waitforbgp  with another option"));                                     
                                                          if((pMsg->getIsMaxMetricOnStartupWaitForBgp())&&(!getMaxMetricOnStartupWaitForBgp()))
                                                          {
                                                             if(pMsg->getVRF() == DEFAULT_VRF_NAME)
                                                             {
						     	         setMaxMetricOnStartupWaitForBgp( pMsg->getIsMaxMetricOnStartupWaitForBgp() );
                                                                 setMaxMetricOnStartupTime(0);
                                                             }
                                                          }
                                                      }
                                                }
						switch(pMsg->getCmdType())
						{
							case OSPF_SET_MAX_METRIC_STARTUP_TIME:
							setMaxMetricOnStartupTime(pMsg->getMaxMetricOnStartupTime());
							setMaxMetricOnStartupWaitForBgp(false);
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -1")+pMsg->getMaxMetricOnStartupTime() );
							break;
							case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP:
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -2")+ pMsg->getIsMaxMetricOnStartupWaitForBgp()+string("vrfNAme") +pMsg->getVRF());
                                                        if(pMsg->getVRF() == DEFAULT_VRF_NAME)
                                                        {
							    setMaxMetricOnStartupWaitForBgp( pMsg->getIsMaxMetricOnStartupWaitForBgp() );
                                                            setMaxMetricOnStartupTime(0);
                                                        }
							break;
							case OSPF_SET_MAX_METRIC_LINK_TYPE:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_LINK_TYPE:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_LINK_TYPE:
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -3") +pMsg->getMaxMetricLinkType());
								if(pMsg->getLinkAll()) {
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -All1") +pMsg->getMaxMetricLinkType());
                                                                      if(!onStartupCmd) {
                                                                        setIsLinkPtp(true);
                                                                        setIsLinkStub(true);
                                                                        setIsLinkTransit(true/*,true*/);
                                                                    }
                                                                    else {
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -All2") +pMsg->getMaxMetricLinkType());
                                                                        setIsOnStartupLinkPtp(true);
                                                                        setIsOnStartupLinkStub(true);
                                                                        setIsOnStartupLinkTransit(true/*,true*/);

                                                                    }

                                                                }
                                                                if(pMsg->getLinkPtp()) {
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -ptp1") +pMsg->getMaxMetricLinkType());
                                                                      if(!onStartupCmd)
                                                                      {
                                                                          setIsLinkPtp(true); 
                                                                      }
                                                                      else
                                                                      { 
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -ptp2") +pMsg->getMaxMetricLinkType());
                                                                       setIsOnStartupLinkPtp(true);
                                                                      }

                                                                }
                                                                if(pMsg->getLinkStub()) {
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -stub1") +pMsg->getMaxMetricLinkType());
                                                                      if(!onStartupCmd) {
                                                                          setIsLinkStub(true);
                                                                      }
                                                                      else { 
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -stub22") +pMsg->getMaxMetricLinkType());
                                                                          setIsOnStartupLinkStub(true);
                                                                       }

                                                                }
                                                                if(pMsg->getLinkTransit())
                                                                {
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -transit1") +pMsg->getMaxMetricLinkType());
                                                                      if(!onStartupCmd)
                                                                      {
                                                                           setIsLinkTransit(true/*,true*/);
                                                                      }
                                                                      else { 
		trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -transit2") +pMsg->getMaxMetricLinkType());
                                                                        setIsOnStartupLinkTransit(true/*,true*/);
                                                                      }
                                                                }
							break;
							case OSPF_SET_MAX_METRIC_EXTERNAL_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_EXTERNAL_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_EXTERNAL_LSA:
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -4")+pMsg->getExtMaxMetric() );
                                                              
                                                        if(!onStartupCmd) {
								setExtMaxMetric(pMsg->getExtMaxMetric()); 
							        setExtMaxMetricLsa(pMsg->getExtMaxMetricLsa()); 
                                                        }
                                                        else {
								setOnStartupExtMaxMetric(pMsg->getExtMaxMetric()); 
							        setOnStartupExtMaxMetricLsa(pMsg->getExtMaxMetricLsa()); 

                                                        }
							break;
							case OSPF_SET_MAX_METRIC_SUMMARY_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -5")+pMsg->getSummMaxMetric() );
                                                        if(!onStartupCmd) {
							    setSummMaxMetric(pMsg->getSummMaxMetric());
							    setSummMaxMetricLsa(pMsg->getSummMaxMetricLsa());
                                                        }
                                                        else
						        {
							    setOnStartupSummMaxMetric(pMsg->getSummMaxMetric());
							    setOnStartupSummMaxMetricLsa(pMsg->getSummMaxMetricLsa());
							}
							break;
							case OSPF_SET_MAX_METRIC_EXTERNAL_LSA|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
							case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_EXTERNAL_LSA|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
							case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_EXTERNAL_LSA|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -6") );
                                                        if(!onStartupCmd) {
							setExtMaxMetric(pMsg->getExtMaxMetric()); 
							setSummMaxMetric(pMsg->getSummMaxMetric());
							setExtMaxMetricLsa(pMsg->getExtMaxMetricLsa()); 
							setSummMaxMetricLsa(pMsg->getSummMaxMetricLsa());
                                                       }
                                                       else {
							setOnStartupExtMaxMetric(pMsg->getExtMaxMetric()); 
							setOnStartupSummMaxMetric(pMsg->getSummMaxMetric());
							setOnStartupExtMaxMetricLsa(pMsg->getExtMaxMetricLsa()); 
							setOnStartupSummMaxMetricLsa(pMsg->getSummMaxMetricLsa());


                                                       }
						break;



						}
						break;
					case OSPF_ROUTER_CMD_EXTERNAL_METRIC_TYPE:
						setExtMetricType(pMsg->getExtMetricType());
						break;
					case OSPF_ROUTER_CMD_NSSA_TRANSLATOR:
						setIsNSSATranslator(pMsg->getIsNSSATranslator());
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED:
						setRedistributeConnected(true);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED_MAP:
						setRedistributeConnected(true);
						setRedistributeConnectedMapName(pMsg->getRedistributeConnectedRouteMap(),true);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_BGP:
						setRedistributeBgp(true);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_BGP_MAP:
						setRedistributeBgp(true);
						setRedistributeBgpMapName(pMsg->getRedistributeBgpRouteMap(),true);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC:
						setRedistributeStatic(true);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC_MAP:
						setRedistributeStatic(true);
						setRedistributeStaticMapName(pMsg->getRedistributeStaticRouteMap(),true);
						break;
					case OSPF_ROUTER_CMD_RFC1583_COMPAT:
    					setRfc1583Compatibility(pMsg->getRfc1583Compatibility());
						break;
					case OSPF_ROUTER_CMD_RFC1587_COMPAT:
						setRfc1587Compatibility(pMsg->getRfc1587Compatibility());
						break;
					case OSPF_ROUTER_CMD_TIMER_THROTTLE:
						setTimerInitDelay(pMsg->getTimerInitDelay(), true);
						setTimerHoldTime(pMsg->getTimerHoldTime(), true);
						setTimerMaxHoldTime(pMsg->getTimerMaxHoldTime(), true);
                        if((!(getTimerInitDelay())) && (!(getTimerHoldTime())) && (!(getTimerMaxHoldTime())))
                        {
                            setTimerInitDelay(0, false);
                            setTimerHoldTime(0, false);
                            setTimerMaxHoldTime(0, false);
                        }
						break;
					case OSPF_ROUTER_CMD_TIMER_LSA_PACING:
						setLsaGroupPacingInterval(pMsg->getLsaGroupPacingInterval());
						break;
					case OSPF_ROUTER_CMD_LOG_ADJACENCY:
						setLogAdjacency(true);
						if(getLogAdjacency() && getLogBadPacket() && getLogChecksum() && 
						   getLogDatabase() && getLogRetransmit())
						{
							setLogAdjacency(false);
							setLogDROnly(false);
							setLogBadPacket(false);
							setLogChecksum(false);
							setLogDatabase(false);
							setLogRetransmit(false);
							setLogAll(true);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_DR_ONLY:
						setLogAdjacency(true);
						setLogDROnly(true);
						if(getLogAdjacency() && getLogBadPacket() && getLogChecksum() && 
						   getLogDatabase() && getLogRetransmit())
						{
							setLogAdjacency(false);
							setLogDROnly(false);
							setLogBadPacket(false);
							setLogChecksum(false);
							setLogDatabase(false);
							setLogRetransmit(false);
							setLogAll(true);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_ALL:
						setLogAll(true);
						setLogAdjacency(false);
						setLogDROnly(false);
						setLogBadPacket(false);
						setLogChecksum(false);
						setLogDatabase(false);
						setLogRetransmit(false);
						break;
					case OSPF_ROUTER_CMD_LOG_BAD_PACKET:
						setLogBadPacket(true);
						if(getLogAdjacency() && getLogBadPacket() && getLogChecksum() && 
						   getLogDatabase() && getLogRetransmit())
						{
							setLogAdjacency(false);
							setLogDROnly(false);
							setLogBadPacket(false);
							setLogChecksum(false);
							setLogDatabase(false);
							setLogRetransmit(false);
							setLogAll(true);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_CHECKSUM:
						setLogBadPacket(true);
						setLogChecksum(true);
						if(getLogAdjacency() && getLogBadPacket() && getLogChecksum() && 
						   getLogDatabase() && getLogRetransmit())
						{
							setLogAdjacency(false);
							setLogDROnly(false);
							setLogBadPacket(false);
							setLogChecksum(false);
							setLogDatabase(false);
							setLogRetransmit(false);
							setLogAll(true);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_DATABASE:
						setLogDatabase(true);
						if(getLogAdjacency() && getLogBadPacket() && getLogChecksum() && 
						   getLogDatabase() && getLogRetransmit())
						{
							setLogAdjacency(false);
							setLogDROnly(false);
							setLogBadPacket(false);
							setLogChecksum(false);
							setLogDatabase(false);
							setLogRetransmit(false);
							setLogAll(true);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_RETRANSMIT:
						setLogRetransmit(true);
						if(getLogAdjacency() && getLogBadPacket() && getLogChecksum() && 
						   getLogDatabase() && getLogRetransmit())
						{
							setLogAdjacency(false);
							setLogDROnly(false);
							setLogBadPacket(false);
							setLogChecksum(false);
							setLogDatabase(false);
							setLogRetransmit(false);
							setLogAll(true);
						}
						break;
					case OSPF_ROUTER_CMD_VRF_LITE_CAPABILITY:
						trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage: Set vrf-lite cap "));
						setVrfLiteCapability(true);
						break;
				}
			}
				break;

			case OSPF_REMOVE:
			{
				IpV4Address addr;
				addr.fromString("0.0.0.0");

				trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage: REMOVE Cmd: ") + pMsg->getCmdCode());
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_REF_BW_VAL:
				trace (TRACE_LEVEL_INFO, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage: REMOVE Cmd:1 ") );
						setReferenceBandwidth(0);
						setUseActivePorts(false);
						break;
					case OSPF_ROUTER_CMD_REF_BW_USE_ACTIVE_PORTS:
				trace (TRACE_LEVEL_INFO, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage: REMOVE Cmd:2 ") );
						setUseActivePorts(false);
						break;
					case OSPF_ROUTER_CMD_DB_OVERFLOW_INTERVAL:
						setDatabaseOverflowInterval(0);
						break;
					case OSPF_ROUTER_CMD_DEF_INFO_ORIGINATE:
						if (pMsg->getDefInfoOrigCmd() == 0) {
							setDefInfoOrig(false);
							setAlways(false);
							setDefInfoOrigMetricType(OSPF_METRIC_NONE);
							setDefInfoOrigMetric(0);
							setDefInfoOrigRouteMap("");
						} else {

							if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_ALWAYS) {
								setAlways(false);
							}

							if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_METRIC) {
								setDefInfoOrigMetric(0);
							}

							if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_METRIC_TYPE) {
								setDefInfoOrigMetricType(OSPF_METRIC_NONE);
							}

							if (pMsg->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_ROUTE_MAP) {
								setDefInfoOrigRouteMap("");
							}
						}
						
						break;
					case OSPF_ROUTER_CMD_DEFAULT_METRIC:
						setDefaultMetric(0);
						break;
					case OSPF_ROUTER_CMD_DEFAULT_PASSIVE_INTF:
						setDefaultPassiveInterface(false);
						break;
					case OSPF_ROUTER_CMD_DISTRIBUTE_LIST_ROUTE_MAP:
						setRouteMap("", false);
						setIsRouteMapIncoming(false);
						break;
					case OSPF_ROUTER_CMD_DOMAIN_ID:
						setDomainId(addr);
						break;
					case OSPF_ROUTER_CMD_DOMAIN_TAG:
						setDomainTag(0);
						break;
					case OSPF_ROUTER_CMD_EXTERNAL_LSDB_LIMIT:
						setExtLSDBLimit(0);
						break;
					case OSPF_ROUTER_CMD_GRES:
						setGresEnabled(false);
						break;
					case OSPF_ROUTER_CMD_GRES_HELPER_DISABLE:
						setGracefulRestartHelper(false);
						break;
					case OSPF_ROUTER_CMD_GRES_NOTIFY_TIME:
						setGracefulRestartNotifyTime(0);
						break;
					case OSPF_ROUTER_CMD_GRES_RESTART_TIME:
						setGracefulRestartRestartTime(0);
						break;
					case OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:
                                                if(pMsg->getOnStartup()) {
                                                     onStartupCmd = true; 
                                                }
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: ") );
						switch(pMsg->getCmdType())
						{
							case OSPF_SET_MAX_METRIC_STARTUP_TIME:
                                                        setMaxMetricOnStartup(false);
							setMaxMetricOnStartupTime(0);
                                                        setIsOnStartupLinkAll(false);
                                                        setIsOnStartupLinkPtp(false);
                                                        setIsOnStartupLinkStub(false);
                                                        setIsOnStartupLinkTransit(false/*,false*/);
							setOnStartupSummMaxMetricLsa(false);
							setOnStartupExtMaxMetricLsa(false);
							setOnStartupExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC); 
							setOnStartupSummMaxMetric(OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC);
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -1")+pMsg->getMaxMetricOnStartupTime() );
							break;
							case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP:
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -2")+ pMsg->getIsMaxMetricOnStartupWaitForBgp());
                                                        setMaxMetricOnStartup(false);
							setMaxMetricOnStartupWaitForBgp(false);
                                                        setIsOnStartupLinkAll(false);
                                                        setIsOnStartupLinkPtp(false);
                                                        setIsOnStartupLinkStub(false);
                                                        setIsOnStartupLinkTransit(false/*,false*/);
							setOnStartupSummMaxMetricLsa(false);
							setOnStartupExtMaxMetricLsa(false);
							setOnStartupExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC); 
							setOnStartupSummMaxMetric(OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC);
							break;
							case OSPF_SET_MAX_METRIC_LINK_TYPE:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_LINK_TYPE:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_LINK_TYPE:
		trace (TRACE_LEVEL_DEBUG, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -3") +pMsg->getMaxMetricLinkType());

								switch(pMsg->getMaxMetricLinkType()) {
                                                                   case OSPF_MAX_METRIC_LINK_TYPE_ALL:
                                                        if(!onStartupCmd) {
                                                                        setIsLinkAll(false);
                                                                        setIsLinkPtp(false);
                                                                        setIsLinkStub(false);
                                                                        setIsLinkTransit(false/*,false*/);
                                                                        }
                                                                        else
                                                                        {
                                                                        setIsOnStartupLinkAll(false);
                                                                        setIsOnStartupLinkPtp(false);
                                                                        setIsOnStartupLinkStub(false);
                                                                        setIsOnStartupLinkTransit(false/*,false*/);

									}
                                                                         break;
                                                                   case OSPF_MAX_METRIC_LINK_TYPE_PTP:
                                                                        if(!onStartupCmd) {
                                                                           setIsLinkPtp(false);
                                                                        }
                                                                        else
                                                                           setIsOnStartupLinkPtp(false);
                                                                         break;
                                                                   case OSPF_MAX_METRIC_LINK_TYPE_STUB:
                                                                      if(!onStartupCmd) {
                                                                               setIsLinkStub(false);
                                                                      }
                                                                      else 
                                                                               setIsOnStartupLinkStub(false);

                                                                         break;
                                                                   case OSPF_MAX_METRIC_LINK_TYPE_TRANSIT:
                                                                       if(!onStartupCmd) {
                                                                           setIsLinkTransit(false/*,false*/);
                                                                          }
                                                                          else
                                                                           setIsOnStartupLinkTransit(false/*,false*/);
                                                                               
                                                                         break;
								}
							break;
							case OSPF_SET_MAX_METRIC_EXTERNAL_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_EXTERNAL_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_EXTERNAL_LSA:

		trace (TRACE_LEVEL_INFO, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -4")+pMsg->getExtMaxMetric() );
                                                        if(!onStartupCmd) {
							setExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC); 
							setExtMaxMetricLsa(false);
                                                        }
                                                        else {
							setOnStartupExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC); 
							setOnStartupExtMaxMetricLsa(false);
                                                        } 
							break;
							case OSPF_SET_MAX_METRIC_SUMMARY_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
		trace (TRACE_LEVEL_INFO, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -5")+pMsg->getSummMaxMetric() );
                                                        if(!onStartupCmd) {
							setSummMaxMetric(pMsg->getSummMaxMetric());
							setSummMaxMetricLsa(false);
                                                        }
                                                        else {
							setOnStartupSummMaxMetric(pMsg->getSummMaxMetric());
							setOnStartupSummMaxMetricLsa(false);

							}
							break;
							case OSPF_SET_MAX_METRIC_EXTERNAL_LSA|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_TIME|OSPF_SET_MAX_METRIC_EXTERNAL_LSA|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
                                                        case OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|OSPF_SET_MAX_METRIC_EXTERNAL_LSA|OSPF_SET_MAX_METRIC_SUMMARY_LSA:
		trace (TRACE_LEVEL_INFO, string("OSPFConfigLocalManagedObject::updateMOFromLocalMessage OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:: -6") );
                                                        if(!onStartupCmd) {
                                                    
							setExtMaxMetricLsa(false); 
							setSummMaxMetricLsa(false);
							setExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC); 
							setSummMaxMetric(OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC);
                                                        }
                                                        else {
							setOnStartupExtMaxMetricLsa(false); 
							setOnStartupSummMaxMetricLsa(false);
							setOnStartupExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC); 
							setOnStartupSummMaxMetric(OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC);
                                                        }

                                                        
						break;


							default://no router lsa
                                                        
                                                        if(!getMaxMetricOnStartup())
							  setRouterLsa(false);
                                                          setExtMaxMetricLsa(false);
                                                        setSummMaxMetricLsa(false);
                                                        setExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC);
                                                        setSummMaxMetric(OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC);
                                                        setIsLinkAll(false);
                                                        setIsLinkPtp(false);
                                                        setIsLinkStub(false);
                                                        setIsLinkTransit(false/*,false*/);
					      }
                                               if(onStartupCmd)
                                               {
                                                 if((!getOnStartupExtMaxMetricLsa())&&(!getOnStartupSummMaxMetricLsa())&&
                                                  (!getIsOnStartupLinkPtp())&&(!getIsOnStartupLinkStub())&&(!getIsOnStartupLinkTransit()))
                                                 {
                                                    if(getMaxMetricOnStartupTime())
                                                    {
                                                        setMaxMetricOnStartupTime(0);
                                                        setMaxMetricOnStartup(false);
                                                    }
                                                    else if (getMaxMetricOnStartupWaitForBgp())
                                                    {
                                                        setMaxMetricOnStartupWaitForBgp(false);
                                                        setMaxMetricOnStartup(false);
                                                    }
                                                  if((!getExtMaxMetricLsa())&&(!getSummMaxMetricLsa())&&
                                                    (!getIsLinkPtp())&&(!getIsLinkStub())&&(!getIsLinkTransit()))
                                                  {
							  setRouterLsa(false);

                                                  }
                                                 }
                                               }
                                               else if(!getMaxMetricOnStartup()) 
                                               {
                                                  if((!getExtMaxMetricLsa())&&(!getSummMaxMetricLsa())&&
                                                    (!getIsLinkPtp())&&(!getIsLinkStub())&&(!getIsLinkTransit()))
                                                  {
							  setRouterLsa(false);

                                                  }
                                               }
	                                  
						break;
				
                         
					case OSPF_ROUTER_CMD_EXTERNAL_METRIC_TYPE:
						setExtMetricType(OSPF_TYPE2);
						break;
					case OSPF_ROUTER_CMD_NSSA_TRANSLATOR:
						setIsNSSATranslator(false);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED:
						setRedistributeConnected(false);
						setRedistributeConnectedMapName("",false);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED_MAP:
						setRedistributeConnectedMapName("",false);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_BGP:
						setRedistributeBgp(false);
						setRedistributeBgpMapName("",false);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_BGP_MAP:
						setRedistributeBgpMapName("",false);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC:
						setRedistributeStatic(false);
						setRedistributeStaticMapName("",false);
						break;
					case OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC_MAP:
						setRedistributeStaticMapName("",false);
						break;
					case OSPF_ROUTER_CMD_RFC1583_COMPAT:
						setRfc1583Compatibility(false);
						break;
					case OSPF_ROUTER_CMD_RFC1587_COMPAT:
						setRfc1587Compatibility(false);
						break;
					case OSPF_ROUTER_CMD_TIMER_THROTTLE:
						setTimerInitDelay(0, false);
						setTimerHoldTime(0, false);
						setTimerMaxHoldTime(0, false);
						break;
					case OSPF_ROUTER_CMD_TIMER_LSA_PACING:
						setLsaGroupPacingInterval(OSPF_DCMD_DEFAULT_LSA_GROUP_PACING_INTERVAL);
						break;
					case OSPF_ROUTER_CMD_LOG_ADJACENCY:
					case OSPF_ROUTER_CMD_LOG_DR_ONLY:
						setLogAdjacency(0);
						setLogDROnly(0);
						if(getLogAll())
						{
							setLogBadPacket(1);
							setLogDatabase(1);
							setLogRetransmit(1);
							setLogAll(0);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_ALL:
						setLogAdjacency(0);
						setLogDROnly(0);
						setLogBadPacket(0);
						setLogChecksum(0);
						setLogDatabase(0);
						setLogRetransmit(0);
						setLogAll(0);
						break;
					case OSPF_ROUTER_CMD_LOG_BAD_PACKET:
					case OSPF_ROUTER_CMD_LOG_CHECKSUM:
						setLogBadPacket(0);
						setLogChecksum(0);
						if(getLogAll())
						{
							setLogAdjacency(1);
							setLogDatabase(1);
							setLogRetransmit(1);
							setLogAll(0);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_DATABASE:
						setLogDatabase(0);
						if(getLogAll())
						{
							setLogAdjacency(1);
							setLogBadPacket(1);
							setLogRetransmit(1);
							setLogAll(0);
						}
						break;
					case OSPF_ROUTER_CMD_LOG_RETRANSMIT:
						setLogRetransmit(0);
						if(getLogAll())
						{
							setLogAdjacency(1);
							setLogBadPacket(1);
							setLogDatabase(1);
							setLogAll(0);
						}
						break;
					case OSPF_ROUTER_CMD_VRF_LITE_CAPABILITY:
						setVrfLiteCapability(false);
						break;
				}
			}
		}

	}

	void OSPFConfigLocalManagedObject::updateClientMessageForReplay(OSPFRouterConfigMessage *pMsg)
	{
		trace (TRACE_LEVEL_DEBUG, "OSPFConfigLocalManagedObject::updateClientMessageForReplay ..");

		pMsg->setOpCode(OSPF_CREATE);
		pMsg->setCmdCode(OSPF_ROUTER_CMD_CONFIG_REPLAY);
		pMsg->setVRF(m_vrf);
		for (UI32 idx = 0; idx <= OSPF_ROUTER_CMD_GLOBAL_CONFIG; idx ++) {
			switch (idx) {
				case OSPF_ROUTER_CMD_GLOBAL_CONFIG:
					pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_GLOBAL_CONFIG);
					break;
				case OSPF_ROUTER_CMD_REF_BW_VAL:
					if (m_ReferenceBandwidth) {
						pMsg->setReferenceBandwidth(m_ReferenceBandwidth);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REF_BW_VAL);
					}
					break;
				case OSPF_ROUTER_CMD_REF_BW_USE_ACTIVE_PORTS:
					if (m_useActivePorts) {
						pMsg->setRefBwUseActivePorts(m_useActivePorts);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REF_BW_USE_ACTIVE_PORTS);
					}
					break;
				case OSPF_ROUTER_CMD_DB_OVERFLOW_INTERVAL:
					if (m_databaseOverflowInterval) {
						pMsg->setDatabaseOverflowInterval(m_databaseOverflowInterval);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DB_OVERFLOW_INTERVAL);
					}
					break;
				case OSPF_ROUTER_CMD_DEF_INFO_ORIGINATE:
					if (getDefInfoOrig()) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DEF_INFO_ORIGINATE);
						UI32 defInfoOrigCmd = 0;

						if (getAlways()) {
							pMsg->setDefInfoOrigAlways(m_always);
							defInfoOrigCmd |= OSPF_CONFIG_DEF_INFO_ORIG_ALWAYS;
						}

						if (m_defInfoOrigMetric) {
							pMsg->setDefInfoOrigMetric(m_defInfoOrigMetric);
							defInfoOrigCmd |= OSPF_CONFIG_DEF_INFO_ORIG_METRIC;
						}

                        if (m_defInfoOrigMetricType != OSPF_METRIC_NONE) {
							pMsg->setDefInfoOrigMetricType(m_defInfoOrigMetricType);
							defInfoOrigCmd |= OSPF_CONFIG_DEF_INFO_ORIG_METRIC_TYPE;
						}

						if (m_defInfoOrigRouteMap != "") {
							pMsg->setDefInfoOrigRouteMap(m_defInfoOrigRouteMap);
							defInfoOrigCmd |= OSPF_CONFIG_DEF_INFO_ORIG_ROUTE_MAP;
						}
						pMsg->setDefInfoOrigCmd(defInfoOrigCmd);
					}
					break;
				case OSPF_ROUTER_CMD_DEFAULT_METRIC:
					if (m_defaultMetric) {
						pMsg->setDefaultMetric(m_defaultMetric);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DEFAULT_METRIC);
					}
					break;
				case OSPF_ROUTER_CMD_DEFAULT_PASSIVE_INTF:
					if (m_defaultPassiveInterface) {
						pMsg->setDefaultPassiveInterface(m_defaultPassiveInterface);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DEFAULT_PASSIVE_INTF);
					}
					break;
				case OSPF_ROUTER_CMD_DISTRIBUTE_LIST_ROUTE_MAP:
					if (m_routeMap.getIsUserConfigured()) {
						pMsg->setDistListRouteMap(m_routeMap.getStringValue());
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DISTRIBUTE_LIST_ROUTE_MAP);
					}
					break;
				case OSPF_ROUTER_CMD_DOMAIN_ID:
					if (m_domainId.toString() != string("0.0.0.0")) {
						pMsg->setDomainId(m_domainId);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DOMAIN_ID);
					}
					break;
				case OSPF_ROUTER_CMD_DOMAIN_TAG:
					if (m_domainTag) {
						pMsg->setDomainTag(m_domainTag);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DOMAIN_TAG);
					}
					break;
				case OSPF_ROUTER_CMD_EXTERNAL_LSDB_LIMIT:
					if (m_extLSDBLimit) {
						pMsg->setExtLSDBLimit(m_extLSDBLimit);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_EXTERNAL_LSDB_LIMIT);
					}
					break;
				case OSPF_ROUTER_CMD_GRES:
					if (m_gresEnabled) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_GRES);
					}
					break;
				case OSPF_ROUTER_CMD_GRES_HELPER_DISABLE:
					if (m_gracefulRestartHelper) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_GRES_HELPER_DISABLE);
						pMsg->setGracefulRestartHelper(true);
					}
					break;
				case OSPF_ROUTER_CMD_GRES_NOTIFY_TIME:
					if (m_gracefulRestartNotifyTime) {
						pMsg->setGracefulRestartNotifyTime(m_gracefulRestartNotifyTime);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_GRES_NOTIFY_TIME);
					}
					break;
				case OSPF_ROUTER_CMD_GRES_RESTART_TIME:
					if (m_gracefulRestartRestartTime) {
						pMsg->setGracefulRestartRestartTime(m_gracefulRestartRestartTime);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_GRES_RESTART_TIME);
					}
					break;
				case OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA:
                                      if(m_maxMetricOnStartup) {
					pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA);
					if(m_onStartupSummMaxMetricLsa)
					{
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_SUMMARY_LSA);
						pMsg->setSummMaxMetric(m_onStartupSummMaxMetric);
						pMsg->setSummMaxMetricLsa(m_onStartupSummMaxMetricLsa);
					}
					if(m_onStartupExtMaxMetricLsa)
					{
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_EXTERNAL_LSA);
						pMsg->setExtMaxMetric(m_onStartupExtMaxMetric);
						pMsg->setExtMaxMetricLsa(m_onStartupExtMaxMetricLsa);
					}
					if(m_maxMetricOnStartUpTime)
					{
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_STARTUP_TIME);
						pMsg->setMaxMetricOnStartupTime(m_maxMetricOnStartUpTime); 
					}
					if(m_isMaxMetricOnStartupWaitForBgp)
					{
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP);
						pMsg->setIsMaxMetricOnStartupWaitForBgp(m_isMaxMetricOnStartupWaitForBgp);
					}
                                        if (/*((m_linkPtp) && (m_linkStub) && (m_linkTransit))||*/((m_onStartupLinkPtp) && (m_onStartupLinkStub) && (m_onStartupLinkTransit))){
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
						//pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_PTP|OSPF_MAX_METRIC_LINK_TYPE_STUB|OSPF_MAX_METRIC_LINK_TYPE_TRANSIT);
						pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_ALL);

                                        }	
                                        else if (/*((m_linkPtp) && (m_linkStub) && (!m_linkTransit)) ||*/((m_onStartupLinkPtp) && (m_onStartupLinkStub) && (!m_onStartupLinkTransit))){
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
						pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_PTP|OSPF_MAX_METRIC_LINK_TYPE_STUB);         
                             		}	
                                        else if (/*((m_linkPtp) && (!m_linkStub) && (!m_linkTransit)) ||*/((m_onStartupLinkPtp) && (!m_onStartupLinkStub) && (!m_onStartupLinkTransit))){
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
						pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_PTP);         
                             		}	
                                        else if (/*((!m_linkPtp) && (m_linkStub) && (m_linkTransit))||*/((!m_onStartupLinkPtp) && (m_onStartupLinkStub) && (m_onStartupLinkTransit))){
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
						pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_TRANSIT|OSPF_MAX_METRIC_LINK_TYPE_STUB);         
                             		}	
                                        else if (/*((!m_linkPtp) && (m_linkStub) && (!m_linkTransit))||*/((!m_onStartupLinkPtp) && (m_onStartupLinkStub) && (!m_onStartupLinkTransit))){
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
						pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_STUB);         
                             		}	
                                        else if (/*((!m_linkPtp) && (!m_linkStub) && (m_linkTransit))||*/((!m_onStartupLinkPtp) && (!m_onStartupLinkStub) && (m_onStartupLinkTransit))){
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
						pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_TRANSIT);         
                             		}	
                                        else if (/*((m_linkPtp) && (!m_linkStub) && (m_linkTransit))||*/((m_onStartupLinkPtp) && (!m_onStartupLinkStub) && (m_onStartupLinkTransit))){
						pMsg->setCmdType(pMsg->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
						pMsg->setMaxMetricLinkType (OSPF_MAX_METRIC_LINK_TYPE_PTP|OSPF_MAX_METRIC_LINK_TYPE_TRANSIT);         
                             		}	
                                      }
					break;
				case OSPF_ROUTER_CMD_EXTERNAL_METRIC_TYPE:
					if (m_extMetricType != OSPF_METRIC_NONE) {
						pMsg->setExtMetricType(m_extMetricType);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_EXTERNAL_METRIC_TYPE);
					}
					break;
				case OSPF_ROUTER_CMD_NSSA_TRANSLATOR:
					if (!m_isNSSATranslator) {
						pMsg->setIsNSSATranslator(false);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_NSSA_TRANSLATOR);
					}
					break;
				case OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED:
				case OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED_MAP:
					if (m_redistributeConnected) {
						if (m_redistributeConnectedMapName.getIsUserConfigured()) {
							pMsg->setRedistributeConnectedRouteMap(m_redistributeConnectedMapName.getStringValue());
							pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED_MAP);
						} else {
							pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED);
						}
					}
					break;
				case OSPF_ROUTER_CMD_REDISTRIBUTION_BGP:
				case OSPF_ROUTER_CMD_REDISTRIBUTION_BGP_MAP:
					if (m_redistributeBgp) {
						if (m_redistributeBgpMapName.getIsUserConfigured()) {
							pMsg->setRedistributeBgpRouteMap(m_redistributeBgpMapName.getStringValue());
							pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REDISTRIBUTION_BGP_MAP);
						} else {
							pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REDISTRIBUTION_BGP);
						}
					}
					break;
				case OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC:
				case OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC_MAP:
					if (m_redistributeStatic) {
						if (m_redistributeStaticMapName.getIsUserConfigured()) {
							pMsg->setRedistributeStaticRouteMap(m_redistributeStaticMapName.getStringValue());
							pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC_MAP);
						} else {
							pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC);
						}
					}
					break;
				case OSPF_ROUTER_CMD_RFC1583_COMPAT:
					if (!m_rfc1583Compatibility) { 
						pMsg->setRfc1583Compatibility(false);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_RFC1583_COMPAT);
					}
					break;
				case OSPF_ROUTER_CMD_RFC1587_COMPAT:
					if (m_rfc1587Compatibility) {
						pMsg->setRfc1587Compatibility(true);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_RFC1587_COMPAT);
					}
					break;
				case OSPF_ROUTER_CMD_TIMER_THROTTLE:
					if (getTimerInitDelayIsConfigured() || getTimerHoldTimeIsConfigured() || getTimerMaxHoldTimeIsConfigured()) {
						pMsg->setTimerInitDelay(getTimerInitDelay());
						pMsg->setTimerHoldTime(getTimerHoldTime());
						pMsg->setTimerMaxHoldTime(getTimerMaxHoldTime());
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_TIMER_THROTTLE);
					}
					break;
				case OSPF_ROUTER_CMD_TIMER_LSA_PACING:
					if (m_timerLsaGroupPacingInterval) {
						pMsg->setLsaGroupPacingInterval(m_timerLsaGroupPacingInterval);
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_TIMER_LSA_PACING);
					}
					break;
				case OSPF_ROUTER_CMD_LOG_ADJACENCY:
					if (m_log_adjacency) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_LOG_ADJACENCY);
					}
					break;
				case OSPF_ROUTER_CMD_LOG_DR_ONLY:
					if (m_log_dr_only) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_LOG_DR_ONLY);
					}
					break;
				case OSPF_ROUTER_CMD_LOG_ALL:
					if (m_log_all) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_LOG_ALL);
					}
					break;
				case OSPF_ROUTER_CMD_LOG_BAD_PACKET:
					if (m_log_bad_packet) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_LOG_BAD_PACKET);
					}
					break;
				case OSPF_ROUTER_CMD_LOG_CHECKSUM:
					if (m_log_checksum) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_LOG_CHECKSUM);
					}
					break;
				case OSPF_ROUTER_CMD_LOG_DATABASE:
					if (m_log_database) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_LOG_DATABASE);
					}
					break;
				case OSPF_ROUTER_CMD_LOG_RETRANSMIT:
					if (m_log_retransmit) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_LOG_RETRANSMIT);
					}
					break;
				case OSPF_ROUTER_CMD_VRF_LITE_CAPABILITY:
					if (m_vrfLiteCapability) {
						pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_VRF_LITE_CAPABILITY);
					}
					break;
				default:
					break;
			}
		}

		/*
		 * Fill up values from distance MO's
		 */
		for (UI32 i = 0; i < m_distance.size(); i++) {
			switch((m_distance[i])->getDistanceType())
			{
				case OSPF_ROUTE_TYPE_EXTERNAL:
					pMsg->setDistanceExternal((m_distance[i])->getDistanceValue());
					pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DISTANCE_EXTERNAL);
					break;
				case OSPF_ROUTE_TYPE_INTER_AREA:
					pMsg->setDistanceInterArea((m_distance[i])->getDistanceValue());
					pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DISTANCE_INTER_AREA);
					break;
				case OSPF_ROUTE_TYPE_INTRA_AREA:
					pMsg->setDistanceIntraArea((m_distance[i])->getDistanceValue());
					pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_DISTANCE_INTRA_AREA);
					break;
				default:
					break;
			}
		}

		/*
		 * Fill up values from Neighbor MO's
		 */
		for (UI32 i = 0; i < m_neighborList.size(); i++) {
			pMsg->m_replayNeighborList.push_back((m_neighborList[i])->getNeighborAddress());
			pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_NEIGHBOR_ADDRESS);
		}

		/*
		 * Fill up values from SummaryAddress MO's
		 */
		for (UI32 i = 0; i < m_summary.size(); i++) {
			pMsg->m_replaySummaryAddressList.push_back((m_summary[i])->getSummaryAddress());
			pMsg->m_replaySummaryMaskList.push_back((m_summary[i])->getSummaryMask());
			pMsg->setReplayCmdCodeBitmap(OSPF_ROUTER_CMD_SUMMARY_ADDRESS);
		}
	}
	/*UI32 OSPFConfigLocalManagedObject::getMaxMetricLinkType() const
	{
		return (m_maxMetricLinkType);
	}

	void OSPFConfigLocalManagedObject::setMaxMetricLinkType(const UI32& linkType)
	{
		m_maxMetricLinkType = linkType;
	}*/

	UI32 OSPFConfigLocalManagedObject::getMaxMetricOnStartupTime() const
	{
		return(m_maxMetricOnStartUpTime);
	}

	void OSPFConfigLocalManagedObject::setMaxMetricOnStartupTime(const UI32& time )
	{
		m_maxMetricOnStartUpTime = time;
	}

	bool OSPFConfigLocalManagedObject::getMaxMetricOnStartupWaitForBgp() const
	{
		return(m_isMaxMetricOnStartupWaitForBgp);
	}

	void OSPFConfigLocalManagedObject::setMaxMetricOnStartupWaitForBgp(const bool& waitforbgp )
	{
		m_isMaxMetricOnStartupWaitForBgp = waitforbgp;

	}

	UI32 OSPFConfigLocalManagedObject::getExtMaxMetric() const
	{
		return(m_extMaxMetric);
	}

	void OSPFConfigLocalManagedObject::setExtMaxMetric(const UI32& extMaxMetric )
	{
		m_extMaxMetric = extMaxMetric;
	}

	UI32 OSPFConfigLocalManagedObject::getSummMaxMetric() const
	{
		return(m_summMaxMetric); 
	}
 
	void OSPFConfigLocalManagedObject::setSummMaxMetric(const UI32& summMaxMetric )
	{
		m_summMaxMetric = summMaxMetric;
	}

	bool OSPFConfigLocalManagedObject::getExtMaxMetricLsa() const
	{
		return(m_extMaxMetricLsa);
	}

	void OSPFConfigLocalManagedObject::setExtMaxMetricLsa(const bool& extMaxMetricLsa )
	{
		m_extMaxMetricLsa = extMaxMetricLsa;
	}

	bool OSPFConfigLocalManagedObject::getSummMaxMetricLsa() const
	{
		return(m_summMaxMetricLsa); 
	}
 
	void OSPFConfigLocalManagedObject::setSummMaxMetricLsa(const bool& summMaxMetricLsa )
	{
		m_summMaxMetricLsa = summMaxMetricLsa;
        }

	bool OSPFConfigLocalManagedObject::getIsAllVrfMaxMetricSet() const
	{
		return (m_isAllVrfMaxMetricSet);
	}
 
	void OSPFConfigLocalManagedObject::setIsAllVrfMaxMetricSet(const bool& isAllVrf)
	{
		m_isAllVrfMaxMetricSet = isAllVrf;
	}

	void OSPFConfigLocalManagedObject::setIsOnStartupMaxMetricAllLSA(const bool &isMaxMetricAllLSA)
	{
		m_isOnStartupMaxMetricAllLsa = isMaxMetricAllLSA;
	}

	bool OSPFConfigLocalManagedObject::getIsOnStartupMaxMetricAllLSA() const
	{
		return (m_isOnStartupMaxMetricAllLsa);
	}

	UI32 OSPFConfigLocalManagedObject::getOnStartupExtMaxMetric() const
	{
		return(m_onStartupExtMaxMetric);
	}

	void OSPFConfigLocalManagedObject::setOnStartupExtMaxMetric(const UI32& extMaxMetric )
	{
		m_onStartupExtMaxMetric = extMaxMetric;
	}

	UI32 OSPFConfigLocalManagedObject::getOnStartupSummMaxMetric() const
	{
		return(m_onStartupSummMaxMetric); 
	}
 
	void OSPFConfigLocalManagedObject::setOnStartupSummMaxMetric(const UI32& summMaxMetric )
	{
		m_onStartupSummMaxMetric = summMaxMetric;
	}

	bool OSPFConfigLocalManagedObject::getOnStartupExtMaxMetricLsa() const
	{
		return(m_onStartupExtMaxMetricLsa);
	}

	void OSPFConfigLocalManagedObject::setOnStartupExtMaxMetricLsa(const bool& extMaxMetricLsa )
	{
		m_onStartupExtMaxMetricLsa = extMaxMetricLsa;
	}

	bool OSPFConfigLocalManagedObject::getOnStartupSummMaxMetricLsa() const
	{
		return(m_onStartupSummMaxMetricLsa); 
	}
 
	void OSPFConfigLocalManagedObject::setOnStartupSummMaxMetricLsa(const bool& summMaxMetricLsa )
	{
		m_onStartupSummMaxMetricLsa = summMaxMetricLsa;
        }
#if 0
	bool OSPFConfigLocalManagedObject::getIsOnStartupAllVrfMaxMetricSet() const
	{
		return (m_isOnStartupMaxMetricSetAllVrf);
	}
 
	void OSPFConfigLocalManagedObject::setIsOnStartupAllVrfMaxMetricSet(const bool& isAllVrf)
	{
		m_isOnStartupMaxMetricSetAllVrf = isAllVrf;
	}
#endif
    void OSPFConfigLocalManagedObject::setIsOnStartupLinkAll(const bool &linkAll)
    {
        m_onStartupLinkAll = linkAll;
    }

    bool OSPFConfigLocalManagedObject::getIsOnStartupLinkAll() const
    {
        return (m_onStartupLinkAll);
    }
    void OSPFConfigLocalManagedObject::setIsOnStartupLinkPtp(const bool &linkPtp)
    {
        m_onStartupLinkPtp = linkPtp;
    }

    bool OSPFConfigLocalManagedObject::getIsOnStartupLinkPtp() const
    {
        return (m_onStartupLinkPtp);
    }
    void OSPFConfigLocalManagedObject::setIsOnStartupLinkStub(const bool &linkStub)
    {
        m_onStartupLinkStub = linkStub;
    }

    bool OSPFConfigLocalManagedObject::getIsOnStartupLinkStub() const
    {
        return (m_onStartupLinkStub);
    }
    void OSPFConfigLocalManagedObject::setIsOnStartupLinkTransit(const bool &linkTransit/*, const bool &isUserConfigured*/)
    {
        m_onStartupLinkTransit = linkTransit; 
    }

    bool OSPFConfigLocalManagedObject::getIsOnStartupLinkTransit() const
    {
        return (m_onStartupLinkTransit);
    }



string OSPFConfigLocalManagedObject::customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
    {
        string displayString = "ospf";

        if("default-vrf" != m_vrf)
        {
            displayString = displayString + " vrf " + m_vrf;
        }

 //       displayString = displayString + "\n";

        return displayString;
    }

}
