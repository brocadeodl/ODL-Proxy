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

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "OSPF/Local/OSPFRouterConfigLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    OSPFRouterConfigLocalMessage::OSPFRouterConfigLocalMessage ()
        : PrismMessage (OSPFLocalObjectManager::getPrismServiceId (),OSPFROUTERCONFIGLOCAL)
		{
			m_opCode = 0; 
			m_cmdCode = 0; 
			m_managedObjectId = OSPF_INVALID_MANAGED_OBJECT;
			m_referenceBandwidth = 0;
			m_refBwUseActivePorts = false;
			m_databaseOverflowInterval = 0;
			m_defInfoOrigAlways = false;
			m_defInfoOrigMetric = 0;
			m_defInfoOrigMetricType = (OSPFDefInfoOrigmetricType)0;
			m_defInfoOrigRouteMap = "";
			m_defInfoOrigCmd = 0;
			m_defaultMetric = 0;
			m_defaultPassiveInterface = false;
			m_distanceExternal = 0;
			m_distanceInterArea = 0;
			m_distanceIntraArea = 0;
			m_distListRouteMap = "";
			m_domainId.fromString("0.0.0.0");
			m_domainTag = 0;
			m_extLSDBLimit = 0;
			m_gracefulRestartHelper = false;
			m_gracefulRestartNotifyTime = 0;
			m_gracefulRestartRestartTime = 0;
			m_maxMetricAllLSA = false;
			m_neighborAddress.fromString("0.0.0.0");
			m_isNSSATranslator = false;
			m_extMetricType = OSPF_METRIC_NONE;
			m_redistributeConnectedRouteMap = "";
			m_redistributeBgpRouteMap = "";
			m_redistributeStaticRouteMap = "";
			m_rfc1583Compatibility = false;
			m_rfc1587Compatibility = false;
			m_timerLsaGroupPacingInterval = 0;
			m_timerInitDelay = OSPF_TIMER_SPF_INVALID;
			m_timerHoldTime = OSPF_TIMER_SPF_INVALID;
			m_timerMaxHoldTime = OSPF_TIMER_SPF_INVALID;
			m_summaryAddress.fromString("0.0.0.0");
			m_summaryMask.fromString("0.0.0.0");
			m_vrf = DEFAULT_VRF_NAME;
			m_cmdType = 0;
			m_maxMetricLinkType = 0;
			m_maxMetricOnStartupTime = 0;
			m_extMaxMetric = OSPF_DEFAULT_MAX_METRIC_EXT_METRIC;
			m_summMaxMetric = OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC;
			m_isAllVrfMaxMetricSet = false;
			m_isMaxMetricOnStartupWaitForBgp = false;
			m_extMaxMetric = OSPF_DEFAULT_MAX_METRIC_EXT_METRIC;
			m_summMaxMetric = OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC;
			m_extMaxMetricLsa = false;
			m_summMaxMetricLsa = false;
                        m_linkAll = false;
                        m_linkPtp = false;
                        m_linkStub = false;
                        m_linkTransit = false;
                        m_onStartup = false;
    }

    OSPFRouterConfigLocalMessage::OSPFRouterConfigLocalMessage (const OSPFManagedObjectId &managedObjectId)
        : PrismMessage (OSPFLocalObjectManager::getPrismServiceId (),OSPFROUTERCONFIGLOCAL)
	{
			m_opCode = 0; 
			m_cmdCode = 0; 
			m_managedObjectId = managedObjectId;
			m_referenceBandwidth = 0;
			m_refBwUseActivePorts = false;
			m_databaseOverflowInterval = 0;
			m_defInfoOrigAlways = false;
			m_defInfoOrigMetric = 0;
			m_defInfoOrigMetricType = (OSPFDefInfoOrigmetricType)0;
			m_defInfoOrigRouteMap = "";
			m_defInfoOrigCmd = 0;
			m_defaultMetric = 0;
			m_defaultPassiveInterface = false;
			m_distanceExternal = 0;
			m_distanceInterArea = 0;
			m_distanceIntraArea = 0;
			m_distListRouteMap = "";
			m_domainId.fromString("0.0.0.0");
			m_domainTag = 0;
			m_extLSDBLimit = 0;
			m_gracefulRestartHelper = false;
			m_gracefulRestartNotifyTime = 0;
			m_gracefulRestartRestartTime = 0;
			m_maxMetricAllLSA = false;
			m_neighborAddress.fromString("0.0.0.0");
			m_isNSSATranslator = false;
			m_extMetricType = OSPF_METRIC_NONE;
			m_redistributeConnectedRouteMap = "";
			m_redistributeBgpRouteMap = "";
			m_redistributeStaticRouteMap = "";
			m_rfc1583Compatibility = false;
			m_rfc1587Compatibility = false;
			m_timerLsaGroupPacingInterval = 0;
			m_timerInitDelay = OSPF_TIMER_SPF_INVALID;
			m_timerHoldTime = OSPF_TIMER_SPF_INVALID;
			m_timerMaxHoldTime = OSPF_TIMER_SPF_INVALID;
			m_summaryAddress.fromString("0.0.0.0");
			m_summaryMask.fromString("0.0.0.0");
			m_vrf = DEFAULT_VRF_NAME;
			m_cmdType = 0;
			m_maxMetricLinkType = 0;
			m_maxMetricOnStartupTime = 0;
			m_extMaxMetric = OSPF_DEFAULT_MAX_METRIC_EXT_METRIC ;
			m_summMaxMetric = OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC ;
			m_isAllVrfMaxMetricSet = false;
			m_isMaxMetricOnStartupWaitForBgp = false;
			m_summMaxMetricLsa = false;
                        m_extMaxMetricLsa = false; 
                        m_linkAll = false;
                        m_linkPtp = false;
                        m_linkStub = false;
                        m_linkTransit = false;
                        m_onStartup = false;
    }

    OSPFRouterConfigLocalMessage::~OSPFRouterConfigLocalMessage ()
    {
    }

    void  OSPFRouterConfigLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));

		addSerializableAttribute (new AttributeEnum((UI32*)&m_managedObjectId, "managedObjectId"));

		addSerializableAttribute (new AttributeUI32(&m_referenceBandwidth, "referenceBandwidth"));
		addSerializableAttribute (new AttributeBool(&m_refBwUseActivePorts, "refBwUseActivePorts"));
		addSerializableAttribute (new AttributeUI32(&m_databaseOverflowInterval, "databaseOverflowInterval"));
		addSerializableAttribute (new AttributeBool(&m_defInfoOrigAlways, "defInfoOrigAlways"));
		addSerializableAttribute (new AttributeUI32(&m_defInfoOrigMetric, "defInfoOrigMetric"));
		addSerializableAttribute (new AttributeEnum((UI32*)&m_defInfoOrigMetricType, "defInfoOrigMetricType"));
		addSerializableAttribute (new AttributeString(&m_defInfoOrigRouteMap, "defInfoOrigRouteMap"));
		addSerializableAttribute (new AttributeUI32(&m_defInfoOrigCmd, "defInfoOrigCmd"));
		addSerializableAttribute (new AttributeUI32(&m_defaultMetric, "defaultMetric"));
		addSerializableAttribute (new AttributeBool(&m_defaultPassiveInterface, "defaultPassiveInterface"));
		addSerializableAttribute (new AttributeUI32(&m_distanceExternal, "distanceExternal"));
		addSerializableAttribute (new AttributeUI32(&m_distanceInterArea, "distanceInterArea"));
		addSerializableAttribute (new AttributeUI32(&m_distanceIntraArea, "distanceIntraArea"));
		addSerializableAttribute (new AttributeString(&m_distListRouteMap, "distListRouteMap"));
		addSerializableAttribute (new AttributeIpV4Address(&m_domainId, "domainId"));
		addSerializableAttribute (new AttributeUI32(&m_domainTag, "domainTag"));
		addSerializableAttribute (new AttributeUI32(&m_extLSDBLimit, "extLSDBLimit"));
		addSerializableAttribute (new AttributeBool(&m_gracefulRestartHelper, "gracefulRestartHelper"));
		addSerializableAttribute (new AttributeUI32(&m_gracefulRestartNotifyTime, "gracefulRestartNotifyTime"));
		addSerializableAttribute (new AttributeUI32(&m_gracefulRestartRestartTime, "gracefulRestartRestartTime"));
		addSerializableAttribute (new AttributeBool(&m_maxMetricAllLSA, "maxMetricAllLSA"));
		addSerializableAttribute (new AttributeIpV4Address(&m_neighborAddress, "neighborAddress"));
		addSerializableAttribute (new AttributeBool(&m_isNSSATranslator, "isNSSATranslator"));
		addSerializableAttribute (new AttributeEnum((UI32*)&m_extMetricType, "extMetricType"));
		addSerializableAttribute (new AttributeString(&m_redistributeConnectedRouteMap, "redistributeConnectedRouteMap"));
		addSerializableAttribute (new AttributeString(&m_redistributeBgpRouteMap, "redistributeBgpRouteMap"));
		addSerializableAttribute (new AttributeString(&m_redistributeStaticRouteMap, "redistributeStaticRouteMap"));
		addSerializableAttribute (new AttributeBool(&m_rfc1583Compatibility, "rfc1583Compatibility"));
		addSerializableAttribute (new AttributeBool(&m_rfc1587Compatibility, "rfc1587Compatibility"));
		addSerializableAttribute (new AttributeUI32(&m_timerLsaGroupPacingInterval, "timerLsaGroupPacingInterval"));
		addSerializableAttribute (new AttributeUI32(&m_timerInitDelay, "timerInitDelay"));
		addSerializableAttribute (new AttributeUI32(&m_timerHoldTime, "timerHoldTime"));
		addSerializableAttribute (new AttributeUI32(&m_timerMaxHoldTime, "timerMaxHoldTime"));
		addSerializableAttribute (new AttributeIpV4Address(&m_summaryAddress, "summaryAddress"));
		addSerializableAttribute (new AttributeIpV4Address(&m_summaryMask, "summaryMask"));
		addSerializableAttribute (new AttributeString(&m_vrf, "vrf"));
		addSerializableAttribute (new AttributeUI32(&m_cmdType, "cmdType"));
		addSerializableAttribute (new AttributeUI32(&m_maxMetricLinkType,  "maxMetricLinkType"));
		addSerializableAttribute (new AttributeUI32(&m_maxMetricOnStartupTime, "maxMetricOnStartUp"));
		addSerializableAttribute (new AttributeUI32(&m_extMaxMetric, "extMaxMetric"));
		addSerializableAttribute (new AttributeUI32(&m_summMaxMetric, "summMaxMetric"));
		addSerializableAttribute (new AttributeBool(&m_isAllVrfMaxMetricSet, "isAllVrfMaxMetric"));
		addSerializableAttribute (new AttributeBool(&m_isMaxMetricOnStartupWaitForBgp, "isOnStartUpWaitForBgp"));
		addSerializableAttribute (new AttributeBool(&m_extMaxMetricLsa, "extMaxMetricLsa"));
		addSerializableAttribute (new AttributeBool(&m_summMaxMetricLsa, "summMaxMetricLsa"));
		addSerializableAttribute (new AttributeBool(&m_linkAll, "linkAll"));
		addSerializableAttribute (new AttributeBool(&m_linkPtp, "linkPtp"));
		addSerializableAttribute (new AttributeBool(&m_linkStub, "linkStub"));
		addSerializableAttribute (new AttributeBool(&m_linkTransit, "linkTransit"));
		addSerializableAttribute (new AttributeBool(&m_onStartup, "onStartup"));

    }

    void  OSPFRouterConfigLocalMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  OSPFRouterConfigLocalMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFRouterConfigLocalMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  OSPFRouterConfigLocalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	OSPFManagedObjectId OSPFRouterConfigLocalMessage::getManagedObjectId() const
	{
		return (m_managedObjectId);
	}

	void OSPFRouterConfigLocalMessage::setReferenceBandwidth(const UI32  &referenceBandwidth)
	{
		m_referenceBandwidth = referenceBandwidth;
	}

	UI32  OSPFRouterConfigLocalMessage::getReferenceBandwidth() const
	{
		return (m_referenceBandwidth);
	}

	void OSPFRouterConfigLocalMessage::setRefBwUseActivePorts(const bool  &refBwUseActivePorts)
	{
		m_refBwUseActivePorts = refBwUseActivePorts;
	}

	bool  OSPFRouterConfigLocalMessage::getRefBwUseActivePorts() const
	{
		return (m_refBwUseActivePorts);
	}

	void OSPFRouterConfigLocalMessage::setDatabaseOverflowInterval(const UI32  &databaseOverflowInterval)
	{
		m_databaseOverflowInterval = databaseOverflowInterval;
	}

	UI32  OSPFRouterConfigLocalMessage::getDatabaseOverflowInterval() const
	{
		return (m_databaseOverflowInterval);
	}

	void OSPFRouterConfigLocalMessage::setDefInfoOrigAlways(const bool  &defInfoOrigAlways)
	{
		m_defInfoOrigAlways = defInfoOrigAlways;
	}

	bool  OSPFRouterConfigLocalMessage::getDefInfoOrigAlways() const
	{
		return (m_defInfoOrigAlways);
	}

	void OSPFRouterConfigLocalMessage::setDefInfoOrigMetric(const UI32  &defInfoOrigMetric)
	{
		m_defInfoOrigMetric = defInfoOrigMetric;
	}

	UI32  OSPFRouterConfigLocalMessage::getDefInfoOrigMetric() const
	{
		return (m_defInfoOrigMetric);
	}

	void OSPFRouterConfigLocalMessage::setDefInfoOrigMetricType(const OSPFDefInfoOrigmetricType  &defInfoOrigMetricType)
	{
		m_defInfoOrigMetricType = defInfoOrigMetricType;
	}

	OSPFDefInfoOrigmetricType  OSPFRouterConfigLocalMessage::getDefInfoOrigMetricType() const
	{
		return (m_defInfoOrigMetricType);
	}

	void OSPFRouterConfigLocalMessage::setDefInfoOrigRouteMap(const string &defInfoOrigRouteMap)
	{
		m_defInfoOrigRouteMap = defInfoOrigRouteMap;
	}

	string OSPFRouterConfigLocalMessage::getDefInfoOrigRouteMap() const
	{
		return (m_defInfoOrigRouteMap);
	}

	UI32 OSPFRouterConfigLocalMessage::getDefInfoOrigCmd() const
	{
		return (m_defInfoOrigCmd);
	}

	void OSPFRouterConfigLocalMessage::setDefInfoOrigCmd(const UI32 &cmd)
	{
		m_defInfoOrigCmd = cmd;
	}
	
	void OSPFRouterConfigLocalMessage::setDefaultPassiveInterface(const bool  &defaultPassiveInterface)
	{
		m_defaultPassiveInterface = defaultPassiveInterface;
	}

	bool  OSPFRouterConfigLocalMessage::getDefaultPassiveInterface() const
	{
		return (m_defaultPassiveInterface);
	}

	void OSPFRouterConfigLocalMessage::setDefaultMetric(const UI32  &defaultMetric)
	{
		m_defaultMetric = defaultMetric;
	}

	UI32  OSPFRouterConfigLocalMessage::getDefaultMetric() const
	{
		return (m_defaultMetric);
	}

	void OSPFRouterConfigLocalMessage::setDistanceExternal(const UI32  &distanceExternal)
	{
		m_distanceExternal = distanceExternal;
	}

	UI32  OSPFRouterConfigLocalMessage::getDistanceExternal() const
	{
		return (m_distanceExternal);
	}

	void OSPFRouterConfigLocalMessage::setDistanceInterArea(const UI32  &distanceInterArea)
	{
		m_distanceInterArea = distanceInterArea;
	}

	UI32  OSPFRouterConfigLocalMessage::getDistanceInterArea() const
	{
		return (m_distanceInterArea);
	}

	void OSPFRouterConfigLocalMessage::setDistanceIntraArea(const UI32  &distanceIntraArea)
	{
		m_distanceIntraArea = distanceIntraArea;
	}

	UI32  OSPFRouterConfigLocalMessage::getDistanceIntraArea() const
	{
		return (m_distanceIntraArea);
	}

	void OSPFRouterConfigLocalMessage::setDistListRouteMap(const string &distListRouteMap)
	{
		m_distListRouteMap = distListRouteMap;
	}

	string OSPFRouterConfigLocalMessage::getDistListRouteMap() const
	{
		return (m_distListRouteMap);
	}

	void OSPFRouterConfigLocalMessage::setDomainId(const IpV4Address  &domainId)
	{
		m_domainId = domainId;
	}

	IpV4Address  OSPFRouterConfigLocalMessage::getDomainId() const
	{
		return (m_domainId);
	}

	void OSPFRouterConfigLocalMessage::setDomainTag(const UI32  &domainTag)
	{
		m_domainTag = domainTag;
	}

	UI32  OSPFRouterConfigLocalMessage::getDomainTag() const
	{
		return (m_domainTag);
	}

	void OSPFRouterConfigLocalMessage::setExtLSDBLimit(const UI32  &extLSDBLimit)
	{
		m_extLSDBLimit = extLSDBLimit;
	}

	UI32  OSPFRouterConfigLocalMessage::getExtLSDBLimit() const
	{
		return (m_extLSDBLimit);
	}

	void OSPFRouterConfigLocalMessage::setGracefulRestartHelper(const bool  &gracefulRestartHelper)
	{
		m_gracefulRestartHelper = gracefulRestartHelper;
	}

	bool  OSPFRouterConfigLocalMessage::getGracefulRestartHelper() const
	{
		return (m_gracefulRestartHelper);
	}

	void OSPFRouterConfigLocalMessage::setGracefulRestartNotifyTime(const UI32  &gracefulRestartNotifyTime)
	{
		m_gracefulRestartNotifyTime = gracefulRestartNotifyTime;
	}

	UI32  OSPFRouterConfigLocalMessage::getGracefulRestartNotifyTime() const
	{
		return (m_gracefulRestartNotifyTime);
	}

	void OSPFRouterConfigLocalMessage::setGracefulRestartRestartTime(const UI32  &gracefulRestartRestartTime)
	{
		m_gracefulRestartRestartTime = gracefulRestartRestartTime;
	}

	UI32  OSPFRouterConfigLocalMessage::getGracefulRestartRestartTime() const
	{
		return (m_gracefulRestartRestartTime);
	}

	void OSPFRouterConfigLocalMessage::setMaxMetricAllLSA(const bool &maxMetricAllLSA)
	{
		m_maxMetricAllLSA = maxMetricAllLSA;
	}

	bool OSPFRouterConfigLocalMessage::getMaxMetricAllLSA() const
	{
		return (m_maxMetricAllLSA);
	}

	void OSPFRouterConfigLocalMessage::setNeighborAddress(const IpV4Address  &neighborAddress)
	{
		m_neighborAddress = neighborAddress;
	}

	IpV4Address  OSPFRouterConfigLocalMessage::getNeighborAddress() const
	{
		return (m_neighborAddress);
	}

	void OSPFRouterConfigLocalMessage::setIsNSSATranslator(const bool &isNSSATranslator)
	{
		m_isNSSATranslator = isNSSATranslator;
	}

	bool OSPFRouterConfigLocalMessage::getIsNSSATranslator() const
	{
		return (m_isNSSATranslator);
	}


	OSPFDefInfoOrigmetricType OSPFRouterConfigLocalMessage::getExtMetricType() const
	{
		return (m_extMetricType);
	}
	void OSPFRouterConfigLocalMessage::setExtMetricType(const OSPFDefInfoOrigmetricType &extMetricType)
	{
		m_extMetricType = extMetricType;
	}

	void OSPFRouterConfigLocalMessage::setRedistributeConnectedRouteMap(const string  &redistributeConnectedRouteMap)
	{
		m_redistributeConnectedRouteMap = redistributeConnectedRouteMap;
	}

	string  OSPFRouterConfigLocalMessage::getRedistributeConnectedRouteMap() const
	{
		return (m_redistributeConnectedRouteMap);
	}

	void OSPFRouterConfigLocalMessage::setRedistributeBgpRouteMap(const string  &redistributeBgpRouteMap)
	{
		m_redistributeBgpRouteMap = redistributeBgpRouteMap;
	}

	string  OSPFRouterConfigLocalMessage::getRedistributeBgpRouteMap() const
	{
		return (m_redistributeBgpRouteMap);
	}

	void OSPFRouterConfigLocalMessage::setRedistributeStaticRouteMap(const string  &redistributeStaticRouteMap)
	{
		m_redistributeStaticRouteMap = redistributeStaticRouteMap;
	}

	string  OSPFRouterConfigLocalMessage::getRedistributeStaticRouteMap() const
	{
		return (m_redistributeStaticRouteMap);
	}

	void OSPFRouterConfigLocalMessage::setRfc1583Compatibility(const bool &rfc1583Compatibility)
	{
		m_rfc1583Compatibility = rfc1583Compatibility;
	}

	bool OSPFRouterConfigLocalMessage::getRfc1583Compatibility() const
	{
		return (m_rfc1583Compatibility);
	}

	void OSPFRouterConfigLocalMessage::setRfc1587Compatibility(const bool &rfc1587Compatibility)
	{
		m_rfc1587Compatibility = rfc1587Compatibility;
	}

	bool OSPFRouterConfigLocalMessage::getRfc1587Compatibility() const
	{
		return (m_rfc1587Compatibility);
	}

	void OSPFRouterConfigLocalMessage::setLsaGroupPacingInterval(const UI32 &timerLsaGroupPacingInterval)
	{
		m_timerLsaGroupPacingInterval = timerLsaGroupPacingInterval;
	}

	UI32 OSPFRouterConfigLocalMessage::getLsaGroupPacingInterval() const
	{
		return (m_timerLsaGroupPacingInterval);
	}

	void OSPFRouterConfigLocalMessage::setTimerInitDelay(const UI32 &timerInitDelay)
	{
		m_timerInitDelay = timerInitDelay;
	}

	UI32 OSPFRouterConfigLocalMessage::getTimerInitDelay() const
	{
		return (m_timerInitDelay);
	}

	void OSPFRouterConfigLocalMessage::setTimerHoldTime(const UI32 &timerHoldTime)
	{
		m_timerHoldTime = timerHoldTime;
	}

	UI32 OSPFRouterConfigLocalMessage::getTimerHoldTime() const
	{
		return (m_timerHoldTime);
	}

	void OSPFRouterConfigLocalMessage::setTimerMaxHoldTime(const UI32 &timerMaxHoldTime)
	{
		m_timerMaxHoldTime = timerMaxHoldTime;
	}

	UI32 OSPFRouterConfigLocalMessage::getTimerMaxHoldTime() const
	{
		return (m_timerMaxHoldTime);
	}

	void OSPFRouterConfigLocalMessage::setSummaryAddress(const IpV4Address  &summaryAddress)
	{
		m_summaryAddress = summaryAddress;
	}

	IpV4Address  OSPFRouterConfigLocalMessage::getSummaryAddress() const
	{
		return (m_summaryAddress);
	}

	void OSPFRouterConfigLocalMessage::setSummaryMask(const IpV4Address  &summaryMask)
	{
		m_summaryMask = summaryMask;
	}

	IpV4Address  OSPFRouterConfigLocalMessage::getSummaryMask() const
	{
		return (m_summaryMask);
	}

	void OSPFRouterConfigLocalMessage::setVRF(const string &vrfName)
	{
		m_vrf = vrfName;
	}

	string OSPFRouterConfigLocalMessage::getVRF() const
	{
		return (m_vrf);
	}

	UI32 OSPFRouterConfigLocalMessage::getCmdType() const
	{
		return ( m_cmdType);
	}

	void OSPFRouterConfigLocalMessage::setCmdType(const UI32& cmdType)
	{
		m_cmdType = cmdType;
	}

	UI32 OSPFRouterConfigLocalMessage::getMaxMetricLinkType() const 
	{
		return (m_maxMetricLinkType);
	}

	void OSPFRouterConfigLocalMessage::setMaxMetricLinkType(const UI32& linkType)
	{
		m_maxMetricLinkType = linkType;
	}

	UI32 OSPFRouterConfigLocalMessage::getMaxMetricOnStartupTime() const
	{
		return (m_maxMetricOnStartupTime);
	}

	void OSPFRouterConfigLocalMessage::setMaxMetricOnStartupTime(const UI32& time )
	{
		m_maxMetricOnStartupTime = time;
	}

	UI32 OSPFRouterConfigLocalMessage::getExtMaxMetric() const
	{
		return(m_extMaxMetric);
	}

	void OSPFRouterConfigLocalMessage::setExtMaxMetric(const UI32& extMaxMetric )
	{
		m_extMaxMetric = extMaxMetric;
	}

	UI32 OSPFRouterConfigLocalMessage::getSummMaxMetric() const
	{
		return (m_summMaxMetric);
	}

	void OSPFRouterConfigLocalMessage::setSummMaxMetric(const UI32& summMaxMetric )
	{
		m_summMaxMetric = summMaxMetric;
	}

	bool OSPFRouterConfigLocalMessage::getIsAllVrfSet() const
	{
		return (m_isAllVrfMaxMetricSet);
	}

	void OSPFRouterConfigLocalMessage::setIsAllVrfSet(const bool& isAllVrf)
	{
		m_isAllVrfMaxMetricSet = isAllVrf;
	}

	bool OSPFRouterConfigLocalMessage:: getIsMaxMetricOnStartupWaitForBgp() const
	{
		return (m_isMaxMetricOnStartupWaitForBgp);	
	}

	void OSPFRouterConfigLocalMessage::setIsMaxMetricOnStartupWaitForBgp(const bool& waitForBgp)
	{
		
		m_isMaxMetricOnStartupWaitForBgp = waitForBgp;
	}

	bool OSPFRouterConfigLocalMessage::getExtMaxMetricLsa() const
	{
		return(m_extMaxMetricLsa);
	}
	void OSPFRouterConfigLocalMessage::setExtMaxMetricLsa(const bool& extMaxMetricLsa )
	{
		m_extMaxMetricLsa = extMaxMetricLsa;
	}

	bool OSPFRouterConfigLocalMessage::getSummMaxMetricLsa() const
	{
		return (m_summMaxMetricLsa);
	}

	void OSPFRouterConfigLocalMessage::setSummMaxMetricLsa(const bool& summMaxMetricLsa )
	{
		m_summMaxMetricLsa = summMaxMetricLsa;
	}



bool OSPFRouterConfigLocalMessage::getLinkAll() const
{
    return(m_linkAll);
}
void OSPFRouterConfigLocalMessage::setLinkAll( const bool  &linkAll)
{
      m_linkAll = linkAll;
}
bool OSPFRouterConfigLocalMessage::getLinkPtp() const
{
    return(m_linkPtp);
}

void OSPFRouterConfigLocalMessage::setLinkPtp( const bool  &linkPtp)
{
      m_linkPtp = linkPtp;

}
bool OSPFRouterConfigLocalMessage::getLinkStub() const
{
    return(m_linkStub);


}

void OSPFRouterConfigLocalMessage::setLinkStub(const  bool  &linkStub)
{

      m_linkStub = linkStub;
}
 
bool OSPFRouterConfigLocalMessage::getLinkTransit() const
{
    return(m_linkTransit);

}
void OSPFRouterConfigLocalMessage::setLinkTransit(const  bool  &linkTransit)
{
      m_linkTransit = linkTransit;

}

bool OSPFRouterConfigLocalMessage::getOnStartup() const
{
    return(m_onStartup);

}
void OSPFRouterConfigLocalMessage::setOnStartup(const  bool  &onStartup)
{
      m_onStartup = onStartup;

}

	void OSPFRouterConfigLocalMessage::dumpMessage() const
	{
		trace (TRACE_LEVEL_DEVEL, string("m_opCode : ") + m_opCode);
		trace (TRACE_LEVEL_DEVEL, string("m_cmdCode : ") + m_cmdCode);
		trace (TRACE_LEVEL_DEVEL, string("m_managedObjectId : ") + m_managedObjectId);
		trace (TRACE_LEVEL_DEVEL, string("m_vrf : ") + m_vrf);
		trace (TRACE_LEVEL_DEVEL, string("m_referenceBandwidth : ") + m_referenceBandwidth);
		trace (TRACE_LEVEL_DEVEL, string("m_refBwUseActivePorts : ") + m_refBwUseActivePorts);
		trace (TRACE_LEVEL_DEVEL, string("m_databaseOverflowInterval : ") + m_databaseOverflowInterval);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigAlways : ") + m_defInfoOrigAlways);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigMetric : ") + m_defInfoOrigMetric);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigMetricType : ") + m_defInfoOrigMetricType);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigRouteMap : ") + m_defInfoOrigRouteMap);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigCmd : ") + m_defInfoOrigCmd);
		trace (TRACE_LEVEL_DEVEL, string("m_defaultMetric : ") + m_defaultMetric);
		trace (TRACE_LEVEL_DEVEL, string("m_defaultPassiveInterface : ") + m_defaultPassiveInterface);
		trace (TRACE_LEVEL_DEVEL, string("m_distanceExternal : ") + m_distanceExternal);
		trace (TRACE_LEVEL_DEVEL, string("m_distanceInterArea : ") + m_distanceInterArea);
		trace (TRACE_LEVEL_DEVEL, string("m_distanceIntraArea : ") + m_distanceIntraArea);
		trace (TRACE_LEVEL_DEVEL, string("m_distListRouteMap : ") + m_distListRouteMap);
		trace (TRACE_LEVEL_DEVEL, string("m_domainId : ") + m_domainId.toString());
		trace (TRACE_LEVEL_DEVEL, string("m_domainTag : ") + m_domainTag);
		trace (TRACE_LEVEL_DEVEL, string("m_extLSDBLimit : ") + m_extLSDBLimit);
		trace (TRACE_LEVEL_DEVEL, string("m_gracefulRestartHelper : ") + m_gracefulRestartHelper);
		trace (TRACE_LEVEL_DEVEL, string("m_gracefulRestartNotifyTime : ") + m_gracefulRestartNotifyTime);
		trace (TRACE_LEVEL_DEVEL, string("m_gracefulRestartRestartTime : ") + m_gracefulRestartRestartTime);
		trace (TRACE_LEVEL_DEVEL, string("m_maxMetricAllLSA : ") + m_maxMetricAllLSA);
		trace (TRACE_LEVEL_DEVEL, string("m_neighborAddress : ") + m_neighborAddress.toString());
		trace (TRACE_LEVEL_DEVEL, string("m_isNSSATranslator : ") + m_isNSSATranslator);
		trace (TRACE_LEVEL_DEVEL, string("m_extMetricType : ") + m_extMetricType);
		trace (TRACE_LEVEL_DEVEL, string("m_redistributeConnectedRouteMap : ") + m_redistributeConnectedRouteMap);
		trace (TRACE_LEVEL_DEVEL, string("m_redistributeBgpRouteMap : ") + m_redistributeBgpRouteMap);
		trace (TRACE_LEVEL_DEVEL, string("m_redistributeStaticRouteMap : ") + m_redistributeStaticRouteMap);
		trace (TRACE_LEVEL_DEVEL, string("m_rfc1583Compatibility : ") + m_rfc1583Compatibility);
		trace (TRACE_LEVEL_DEVEL, string("m_rfc1587Compatibility : ") + m_rfc1587Compatibility);
		trace (TRACE_LEVEL_DEVEL, string("m_timerLsaGroupPacingInterval : ") + m_timerLsaGroupPacingInterval);
		trace (TRACE_LEVEL_DEVEL, string("m_timerInitDelay : ") + m_timerInitDelay);
		trace (TRACE_LEVEL_DEVEL, string("m_timerHoldTime : ") + m_timerHoldTime);
		trace (TRACE_LEVEL_DEVEL, string("m_timerMaxHoldTime : ") + m_timerMaxHoldTime);
		trace (TRACE_LEVEL_DEVEL, string("m_summaryAddress : ") + m_summaryAddress.toString());
		trace (TRACE_LEVEL_DEVEL, string("m_summaryMask : ") + m_summaryMask.toString());
		trace (TRACE_LEVEL_DEVEL, string("m_cmdType : ") + m_cmdType);
		trace (TRACE_LEVEL_DEVEL, string("m_maxMetricLinkType : ") + m_maxMetricLinkType);
		trace (TRACE_LEVEL_DEVEL, string("m_MaxMetricOnStartupTime : ") + m_maxMetricOnStartupTime);
		trace (TRACE_LEVEL_DEVEL, string("m_extMaxmetric : ") +m_extMaxMetric );
		trace (TRACE_LEVEL_DEVEL, string(" m_summMaxMetric: ") +m_summMaxMetric );
		trace (TRACE_LEVEL_DEVEL, string("m_extMaxmetricLsa : ") +m_extMaxMetricLsa );
		trace (TRACE_LEVEL_DEVEL, string(" m_summMaxMetricLsa: ") +m_summMaxMetricLsa );
		trace (TRACE_LEVEL_DEVEL, string("m_isAllVrfMaxMetricSet : ") +m_isAllVrfMaxMetricSet );
		trace (TRACE_LEVEL_DEVEL, string("m_isMaxMetricOnStartupWaitForBgp : ") +m_isMaxMetricOnStartupWaitForBgp );
		trace (TRACE_LEVEL_DEVEL, string("m_linkAll : ") +m_linkAll );
		trace (TRACE_LEVEL_DEVEL, string("m_linkPtp : ") +m_linkPtp );
		trace (TRACE_LEVEL_DEVEL, string("m_linkStub : ") +m_linkStub );
		trace (TRACE_LEVEL_DEVEL, string("m_linkTransit : ") +m_linkTransit );
	}

}
