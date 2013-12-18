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
#include "Framework/Utils/StringUtils.h"
#include "ClientInterface/OSPF/OSPFRouterConfigMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "ClientInterface/OSPF/OSPFPermitDenyRedistributeMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "OSPF/Local/OSPFRouterConfigLocalMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    OSPFRouterConfigMessage::OSPFRouterConfigMessage ()
        : DcmManagementInterfaceMessage (OSPFROUTERCONFIG)
    {
			m_opCode = 0; 
			m_cmdCode = 0; 
			m_vrf = DEFAULT_VRF_NAME;
			m_managedObjectId = OSPF_INVALID_MANAGED_OBJECT;
			m_referenceBandwidth = 0;
			m_refBwUseActivePorts = false;
			m_databaseOverflowInterval = 0;
			m_defInfoOrigAlways = false;
			m_defInfoOrigMetric = 0;
			m_defInfoOrigMetricType = (OSPFDefInfoOrigmetricType)0;
			m_defInfoOrigRouteMap = string("");
			m_defInfoOrigCmd = 0;
			m_defaultMetric = 0;
			m_defaultPassiveInterface = false;
			m_distanceExternal = 0;
			m_distanceInterArea = 0;
			m_distanceIntraArea = 0;
			m_distListRouteMap = string("");
			m_domainId.fromString(string("0.0.0.0"));
			m_domainTag = 0;
			m_extLSDBLimit = 0;
			m_gracefulRestartHelper = false;
			m_gracefulRestartNotifyTime = 0;
			m_gracefulRestartRestartTime = 0;
			m_maxMetricAllLSA = false;
			m_neighborAddress.fromString(string("0.0.0.0"));
			m_isNSSATranslator = false;
			m_extMetricType = OSPF_METRIC_NONE;
			m_redistributeConnectedRouteMap = string("");
			m_redistributeBgpRouteMap = string("");
			m_redistributeStaticRouteMap = string("");
			m_rfc1583Compatibility = false;
			m_rfc1587Compatibility = false;
			m_timerLsaGroupPacingInterval = 0;
			m_timerInitDelay = 0;
			m_timerHoldTime = 0;
			m_timerMaxHoldTime = 0;
			m_summaryAddress.fromString(string("0.0.0.0"));
			m_summaryMask.fromString(string("0.0.0.0"));
			m_replayCmdCodeBitmap = 0;
			m_cmdType = 0;
			m_maxMetricLinkType = 0;
			m_maxMetricOnStartupTime = 0;
			m_extMaxMetric = OSPF_DEFAULT_MAX_METRIC_EXT_METRIC ;
			m_summMaxMetric = OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC ;
			m_isAllVrfMaxMetricSet = false;
			m_isMaxMetricOnStartupWaitForBgp = false;
			m_extMaxMetricLsa = false;
			m_summMaxMetricLsa = false;

    }

    OSPFRouterConfigMessage::~OSPFRouterConfigMessage ()
    {
    }

    void  OSPFRouterConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
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
		addSerializableAttribute (new AttributeUI64(&m_replayCmdCodeBitmap, "replayCmdCodeBitmap"));
		addSerializableAttribute (new AttributeIpV4AddressVector(&m_replayNeighborList, "replayNeighborList"));
		addSerializableAttribute (new AttributeIpV4AddressVector(&m_replaySummaryAddressList, "replaySummaryAddressList"));
		addSerializableAttribute (new AttributeIpV4AddressVector(&m_replaySummaryMaskList, "replaySummaryMaskList"));
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
    }

    void  OSPFRouterConfigMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  OSPFRouterConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFRouterConfigMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  OSPFRouterConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	OSPFManagedObjectId OSPFRouterConfigMessage::getManagedObjectId() const
	{
		return (m_managedObjectId);
	}

    const void *OSPFRouterConfigMessage::getCStructureForInputs ()
    {
		routerOSPFConfigMsg_t *pInput = NULL;
		unsigned int num_neighbors = m_replayNeighborList.size();
		unsigned int num_summary_addresses = m_replaySummaryAddressList.size();

		if (m_cmdCode == OSPF_ROUTER_CMD_CONFIG_REPLAY) {
			UI32 extraBytes = sizeof(OSPFRouterConfigReplayMsg_t) +
								(num_neighbors + num_summary_addresses*2)*OSPF_IPV4_ADDR_STR_SIZE;
			pInput = (routerOSPFConfigMsg_t*)calloc(1, sizeof(routerOSPFConfigMsg_t) + extraBytes);
		} else {
			pInput = new routerOSPFConfigMsg_t;
		}

        pInput->opCode = m_opCode;
        pInput->cmdCode = m_cmdCode;
		pInput->m_referenceBandwidth = m_referenceBandwidth;
		pInput->m_refBwUseActivePorts = m_refBwUseActivePorts;
		pInput->m_databaseOverflowInterval = m_databaseOverflowInterval;
		pInput->m_defInfoOrigAlways = m_defInfoOrigAlways;
		pInput->m_defInfoOrigMetric = m_defInfoOrigMetric;
		pInput->m_defInfoOrigMetricType = m_defInfoOrigMetricType;
		if(m_defInfoOrigRouteMap != string(""))
			strcpy(pInput->m_defInfoOrigRouteMap,m_defInfoOrigRouteMap.c_str());
		else
			pInput->m_defInfoOrigRouteMap[0] = '\0';
		pInput->m_defaultMetric = m_defaultMetric;
		pInput->m_defaultPassiveInterface = m_defaultPassiveInterface;
		pInput->m_distanceExternal = m_distanceExternal;
		pInput->m_distanceInterArea = m_distanceInterArea;
		pInput->m_distanceIntraArea = m_distanceIntraArea;
		if(m_distListRouteMap != string(""))
			strcpy(pInput->m_distListRouteMap ,m_distListRouteMap.c_str());
		else
			pInput->m_distListRouteMap[0] = '\0';
		if(m_domainId.toString() != string("0.0.0.0"))
			strcpy(pInput->m_domainId ,(m_domainId.toString()).c_str());
		else
			pInput->m_domainId[0] = '\0';
		pInput->m_domainTag = m_domainTag;
		pInput->m_extLSDBLimit = m_extLSDBLimit;
		pInput->m_gracefulRestartNotifyTime = m_gracefulRestartNotifyTime;
		pInput->m_gracefulRestartRestartTime = m_gracefulRestartRestartTime;
		pInput->m_maxMetricAllLSA = m_maxMetricAllLSA;
		strcpy(pInput->m_neighborAddress ,(m_neighborAddress.toString()).c_str());
		pInput->m_isNSSATranslator = m_isNSSATranslator;
		pInput->m_extMetricType = m_extMetricType;
		if(m_redistributeConnectedRouteMap != string(""))
			strcpy(pInput->m_redistributeConnectedRouteMap ,m_redistributeConnectedRouteMap.c_str());
		else
			pInput->m_redistributeConnectedRouteMap[0] = '\0';
		if(m_redistributeBgpRouteMap != string(""))
			strcpy(pInput->m_redistributeBgpRouteMap ,m_redistributeBgpRouteMap.c_str());
		else
			pInput->m_redistributeBgpRouteMap[0] = '\0';
		if(m_redistributeStaticRouteMap != string(""))
			strcpy(pInput->m_redistributeStaticRouteMap ,m_redistributeStaticRouteMap.c_str());
		else
			pInput->m_redistributeStaticRouteMap[0] = '\0';
		pInput->m_rfc1583Compatibility = m_rfc1583Compatibility;
		pInput->m_rfc1587Compatibility = m_rfc1587Compatibility;
		pInput->m_timerLsaGroupPacingInterval = m_timerLsaGroupPacingInterval;
		pInput->m_timerInitDelay = m_timerInitDelay;
		pInput->m_timerHoldTime = m_timerHoldTime;
		pInput->m_timerMaxHoldTime = m_timerMaxHoldTime;
		strcpy(pInput->m_summaryAddress ,(m_summaryAddress.toString()).c_str());
		strcpy(pInput->m_summaryMask ,(m_summaryMask.toString()).c_str());
		strcpy(pInput->m_vrf ,m_vrf.c_str());
		pInput->m_cmdType = m_cmdType;
		pInput->m_maxMetricLinkType = m_maxMetricLinkType;
		pInput->m_maxMetricOnStartupTime = m_maxMetricOnStartupTime;
		pInput->m_extMaxMetric = m_extMaxMetric;             
		pInput->m_summMaxMetric = m_summMaxMetric; 
		pInput->m_extMaxMetricLsa = m_extMaxMetricLsa;             
		pInput->m_summMaxMetricLsa = m_summMaxMetricLsa; 
		pInput->m_isAllVrfMaxMetricSet = m_isAllVrfMaxMetricSet;

		if (m_cmdCode == OSPF_ROUTER_CMD_CONFIG_REPLAY) {
			OSPFRouterConfigReplayMsg_t *replay = (OSPFRouterConfigReplayMsg_t*)pInput->config_replay;
			replay->num_neighbors = num_neighbors;
			replay->num_summary_addresses = num_summary_addresses;
			replay->m_replayCmdCodeBitmap = m_replayCmdCodeBitmap;
			char *buf = replay->buf;

			for (UI32 idx = 0; idx < num_neighbors; idx++) {
				strncpy(buf, (m_replayNeighborList[idx].toString()).c_str(), OSPF_IPV4_ADDR_STR_SIZE);
				buf += OSPF_IPV4_ADDR_STR_SIZE;
			}

			for (UI32 idx = 0; idx < num_summary_addresses; idx ++) {
				strncpy(buf, (m_replaySummaryAddressList[idx].toString()).c_str(), OSPF_IPV4_ADDR_STR_SIZE);
				buf += OSPF_IPV4_ADDR_STR_SIZE;
				strncpy(buf, (m_replaySummaryMaskList[idx].toString()).c_str(), OSPF_IPV4_ADDR_STR_SIZE);
				buf += OSPF_IPV4_ADDR_STR_SIZE;
			}
		}

        return (pInput);
    }

    void OSPFRouterConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

	void OSPFRouterConfigMessage::setReferenceBandwidth(const UI32  &referenceBandwidth)
	{
		m_referenceBandwidth = referenceBandwidth;
	}

	UI32  OSPFRouterConfigMessage::getReferenceBandwidth() const
	{
		return (m_referenceBandwidth);
	}

	void OSPFRouterConfigMessage::setRefBwUseActivePorts(const bool  &refBwUseActivePorts)
	{
		m_refBwUseActivePorts = refBwUseActivePorts;
	}

	bool  OSPFRouterConfigMessage::getRefBwUseActivePorts() const
	{
		return (m_refBwUseActivePorts);
	}

	void OSPFRouterConfigMessage::setDatabaseOverflowInterval(const UI32  &databaseOverflowInterval)
	{
		m_databaseOverflowInterval = databaseOverflowInterval;
	}

	UI32  OSPFRouterConfigMessage::getDatabaseOverflowInterval() const
	{
		return (m_databaseOverflowInterval);
	}

	void OSPFRouterConfigMessage::setDefInfoOrigAlways(const bool  &defInfoOrigAlways)
	{
		m_defInfoOrigAlways = defInfoOrigAlways;
	}

	bool  OSPFRouterConfigMessage::getDefInfoOrigAlways() const
	{
		return (m_defInfoOrigAlways);
	}

	void OSPFRouterConfigMessage::setDefInfoOrigMetric(const UI32  &defInfoOrigMetric)
	{
		m_defInfoOrigMetric = defInfoOrigMetric;
	}

	UI32  OSPFRouterConfigMessage::getDefInfoOrigMetric() const
	{
		return (m_defInfoOrigMetric);
	}

	void OSPFRouterConfigMessage::setDefInfoOrigMetricType(const OSPFDefInfoOrigmetricType  &defInfoOrigMetricType)
	{
		m_defInfoOrigMetricType = defInfoOrigMetricType;
	}

	OSPFDefInfoOrigmetricType  OSPFRouterConfigMessage::getDefInfoOrigMetricType() const
	{
		return (m_defInfoOrigMetricType);
	}

	void OSPFRouterConfigMessage::setDefInfoOrigRouteMap(const string &defInfoOrigRouteMap)
	{
		m_defInfoOrigRouteMap = defInfoOrigRouteMap;
	}

	string OSPFRouterConfigMessage::getDefInfoOrigRouteMap() const
	{
		return (m_defInfoOrigRouteMap);
	}

	UI32 OSPFRouterConfigMessage::getDefInfoOrigCmd() const
	{
		return (m_defInfoOrigCmd);
	}

	void OSPFRouterConfigMessage::setDefInfoOrigCmd(const UI32 &cmd)
	{
		m_defInfoOrigCmd = cmd;
	}
	
	void OSPFRouterConfigMessage::setDefaultPassiveInterface(const bool  &defaultPassiveInterface)
	{
		m_defaultPassiveInterface = defaultPassiveInterface;
	}

	bool  OSPFRouterConfigMessage::getDefaultPassiveInterface() const
	{
		return (m_defaultPassiveInterface);
	}
	void OSPFRouterConfigMessage::setDefaultMetric(const UI32  &defaultMetric)
	{
		m_defaultMetric = defaultMetric;
	}

	UI32  OSPFRouterConfigMessage::getDefaultMetric() const
	{
		return (m_defaultMetric);
	}

	void OSPFRouterConfigMessage::setDistanceExternal(const UI32  &distanceExternal)
	{
		m_distanceExternal = distanceExternal;
	}

	UI32  OSPFRouterConfigMessage::getDistanceExternal() const
	{
		return (m_distanceExternal);
	}

	void OSPFRouterConfigMessage::setDistanceInterArea(const UI32  &distanceInterArea)
	{
		m_distanceInterArea = distanceInterArea;
	}

	UI32  OSPFRouterConfigMessage::getDistanceInterArea() const
	{
		return (m_distanceInterArea);
	}

	void OSPFRouterConfigMessage::setDistanceIntraArea(const UI32  &distanceIntraArea)
	{
		m_distanceIntraArea = distanceIntraArea;
	}

	UI32  OSPFRouterConfigMessage::getDistanceIntraArea() const
	{
		return (m_distanceIntraArea);
	}

	void OSPFRouterConfigMessage::setDistListRouteMap(const string &distListRouteMap)
	{
		m_distListRouteMap = distListRouteMap;
	}

	string OSPFRouterConfigMessage::getDistListRouteMap() const
	{
		return (m_distListRouteMap);
	}

	void OSPFRouterConfigMessage::setDomainId(const IpV4Address  &domainId)
	{
		m_domainId = domainId;
	}

	IpV4Address  OSPFRouterConfigMessage::getDomainId() const
	{
		return (m_domainId);
	}

	void OSPFRouterConfigMessage::setDomainTag(const UI32  &domainTag)
	{
		m_domainTag = domainTag;
	}

	UI32  OSPFRouterConfigMessage::getDomainTag() const
	{
		return (m_domainTag);
	}

	void OSPFRouterConfigMessage::setExtLSDBLimit(const UI32  &extLSDBLimit)
	{
		m_extLSDBLimit = extLSDBLimit;
	}

	UI32  OSPFRouterConfigMessage::getExtLSDBLimit() const
	{
		return (m_extLSDBLimit);
	}

	void OSPFRouterConfigMessage::setGracefulRestartHelper(const bool  &gracefulRestartHelper)
	{
		m_gracefulRestartHelper = gracefulRestartHelper;
	}

	bool  OSPFRouterConfigMessage::getGracefulRestartHelper() const
	{
		return (m_gracefulRestartHelper);
	}

	void OSPFRouterConfigMessage::setGracefulRestartNotifyTime(const UI32  &gracefulRestartNotifyTime)
	{
		m_gracefulRestartNotifyTime = gracefulRestartNotifyTime;
	}

	UI32  OSPFRouterConfigMessage::getGracefulRestartNotifyTime() const
	{
		return (m_gracefulRestartNotifyTime);
	}

	void OSPFRouterConfigMessage::setGracefulRestartRestartTime(const UI32  &gracefulRestartRestartTime)
	{
		m_gracefulRestartRestartTime = gracefulRestartRestartTime;
	}

	UI32  OSPFRouterConfigMessage::getGracefulRestartRestartTime() const
	{
		return (m_gracefulRestartRestartTime);
	}

	void OSPFRouterConfigMessage::setMaxMetricAllLSA(const bool &maxMetricAllLSA)
	{
		m_maxMetricAllLSA = maxMetricAllLSA;
	}

	bool OSPFRouterConfigMessage::getMaxMetricAllLSA() const
	{
		return (m_maxMetricAllLSA);
	}

	void OSPFRouterConfigMessage::setNeighborAddress(const IpV4Address  &neighborAddress)
	{
		m_neighborAddress = neighborAddress;
	}

	IpV4Address  OSPFRouterConfigMessage::getNeighborAddress() const
	{
		return (m_neighborAddress);
	}

	void OSPFRouterConfigMessage::setIsNSSATranslator(const bool &isNSSATranslator)
	{
		m_isNSSATranslator = isNSSATranslator;
	}

	bool OSPFRouterConfigMessage::getIsNSSATranslator() const
	{
		return (m_isNSSATranslator);
	}

	OSPFDefInfoOrigmetricType OSPFRouterConfigMessage::getExtMetricType() const
	{
		return (m_extMetricType);
	}

	void OSPFRouterConfigMessage::setExtMetricType(const OSPFDefInfoOrigmetricType &extMetricType)
	{
		m_extMetricType = extMetricType;
	}

	void OSPFRouterConfigMessage::setRedistributeConnectedRouteMap(const string  &redistributeConnectedRouteMap)
	{
		m_redistributeConnectedRouteMap = redistributeConnectedRouteMap;
	}

	string  OSPFRouterConfigMessage::getRedistributeConnectedRouteMap() const
	{
		return (m_redistributeConnectedRouteMap);
	}

	void OSPFRouterConfigMessage::setRedistributeBgpRouteMap(const string  &redistributeBgpRouteMap)
	{
		m_redistributeBgpRouteMap = redistributeBgpRouteMap;
	}

	string  OSPFRouterConfigMessage::getRedistributeBgpRouteMap() const
	{
		return (m_redistributeBgpRouteMap);
	}

	void OSPFRouterConfigMessage::setRedistributeStaticRouteMap(const string  &redistributeStaticRouteMap)
	{
		m_redistributeStaticRouteMap = redistributeStaticRouteMap;
	}

	string  OSPFRouterConfigMessage::getRedistributeStaticRouteMap() const
	{
		return (m_redistributeStaticRouteMap);
	}

	void OSPFRouterConfigMessage::setRfc1583Compatibility(const bool &rfc1583Compatibility)
	{
		m_rfc1583Compatibility = rfc1583Compatibility;
	}

	bool OSPFRouterConfigMessage::getRfc1583Compatibility() const
	{
		return (m_rfc1583Compatibility);
	}

	void OSPFRouterConfigMessage::setRfc1587Compatibility(const bool &rfc1587Compatibility)
	{
		m_rfc1587Compatibility = rfc1587Compatibility;
	}

	bool OSPFRouterConfigMessage::getRfc1587Compatibility() const
	{
		return (m_rfc1587Compatibility);
	}

	void OSPFRouterConfigMessage::setLsaGroupPacingInterval(const UI32 &timerLsaGroupPacingInterval)
	{
		m_timerLsaGroupPacingInterval = timerLsaGroupPacingInterval;
	}

	UI32 OSPFRouterConfigMessage::getLsaGroupPacingInterval() const
	{
		return (m_timerLsaGroupPacingInterval);
	}

    void OSPFRouterConfigMessage::setTimerInitDelay(const UI32 &timerInitDelay)
    {
        m_timerInitDelay = timerInitDelay;
    }

    UI32 OSPFRouterConfigMessage::getTimerInitDelay() const
    {
        return (m_timerInitDelay);
    }

    void OSPFRouterConfigMessage::setTimerHoldTime(const UI32 &timerHoldTime)
    {
        m_timerHoldTime = timerHoldTime;
    }

    UI32 OSPFRouterConfigMessage::getTimerHoldTime() const
    {
        return (m_timerHoldTime);
    }

    void OSPFRouterConfigMessage::setTimerMaxHoldTime(const UI32 &timerMaxHoldTime)
    {
        m_timerMaxHoldTime = timerMaxHoldTime;
    }

    UI32 OSPFRouterConfigMessage::getTimerMaxHoldTime() const
    {
        return (m_timerMaxHoldTime);
    }

	void OSPFRouterConfigMessage::setSummaryAddress(const IpV4Address  &summaryAddress)
	{
		m_summaryAddress = summaryAddress;
	}

	IpV4Address  OSPFRouterConfigMessage::getSummaryAddress() const
	{
		return (m_summaryAddress);
	}

	void OSPFRouterConfigMessage::setSummaryMask(const IpV4Address  &summaryMask)
	{
		m_summaryMask = summaryMask;
	}

	IpV4Address  OSPFRouterConfigMessage::getSummaryMask() const
	{
		return (m_summaryMask);
	}

	UI64 OSPFRouterConfigMessage::getReplayCmdCodeBitmap() const
	{
		return (m_replayCmdCodeBitmap);
	}

	void OSPFRouterConfigMessage::setReplayCmdCodeBitmap(const OSPFRouterConfigCmdCode &cmdCode)
	{
		m_replayCmdCodeBitmap |= (1LL << (UI32)cmdCode);
	}

	void OSPFRouterConfigMessage::setVRF(const string &vrf)
	{
		m_vrf = vrf;
	}

	string OSPFRouterConfigMessage::getVRF() const
	{
		return (m_vrf);
	}

	UI32 OSPFRouterConfigMessage::getCmdType() const
	{
		return ( m_cmdType);
	}

	void OSPFRouterConfigMessage::setCmdType(const UI32& cmdType)
	{
		m_cmdType = cmdType;
	}

	UI32 OSPFRouterConfigMessage::getMaxMetricLinkType() const
	{
		return (m_maxMetricLinkType);
	}

	void OSPFRouterConfigMessage::setMaxMetricLinkType(const UI32& linkType)
	{
		m_maxMetricLinkType = linkType;
	}

	UI32 OSPFRouterConfigMessage::getMaxMetricOnStartupTime() const
	{
		return (m_maxMetricOnStartupTime);
	}

	void OSPFRouterConfigMessage::setMaxMetricOnStartupTime(const UI32& time )
	{
		m_maxMetricOnStartupTime = time;
	}

	UI32 OSPFRouterConfigMessage::getExtMaxMetric() const
	{
		return(m_extMaxMetric);
	}

	void OSPFRouterConfigMessage::setExtMaxMetric(const UI32& extMaxMetric )
	{
		m_extMaxMetric = extMaxMetric;
	}

	UI32 OSPFRouterConfigMessage::getSummMaxMetric() const
	{
		return (m_summMaxMetric);
	}

	void OSPFRouterConfigMessage::setSummMaxMetric(const UI32& summMaxMetric )
	{
		m_summMaxMetric = summMaxMetric;
	}

	 void OSPFRouterConfigMessage::setIsAllVrfSet(const bool& isAllVrf)
	{
		 m_isAllVrfMaxMetricSet = isAllVrf;
	}

	bool OSPFRouterConfigMessage:: getIsMaxMetricOnStartupWaitForBgp() const
        {
                return (m_isMaxMetricOnStartupWaitForBgp);
        }

        void OSPFRouterConfigMessage::setIsMaxMetricOnStartupWaitForBgp(const bool& waitForBgp)
        {

                m_isMaxMetricOnStartupWaitForBgp = waitForBgp;
        }



	bool OSPFRouterConfigMessage::getExtMaxMetricLsa() const
	{
		return(m_extMaxMetricLsa);
	}

	void OSPFRouterConfigMessage::setExtMaxMetricLsa(const bool& extMaxMetricLsa )
	{
		m_extMaxMetricLsa = extMaxMetricLsa;
	}

	bool OSPFRouterConfigMessage::getSummMaxMetricLsa() const
	{
		return (m_summMaxMetricLsa);
	}

	void OSPFRouterConfigMessage::setSummMaxMetricLsa(const bool& summMaxMetricLsa )
	{
		m_summMaxMetricLsa = summMaxMetricLsa;
	}

	void OSPFRouterConfigMessage::dumpMessage() const
	{
	
		trace (TRACE_LEVEL_DEVEL, string("OSPFRouterConfigMessage.cpp : dumpMessage()") );
		trace (TRACE_LEVEL_DEVEL, string("m_opCode : ") + m_opCode);
		trace (TRACE_LEVEL_DEVEL, string("m_cmdCode : ") + m_cmdCode);
		trace (TRACE_LEVEL_DEVEL, string("m_vrf : ") + m_vrf);
		trace (TRACE_LEVEL_DEVEL, string("m_managedObjectId : ") + m_managedObjectId);
		trace (TRACE_LEVEL_DEVEL, string("m_referenceBandwidth : ") + m_referenceBandwidth);
		trace (TRACE_LEVEL_DEVEL, string("m_refBwUseActivePorts : ") + m_refBwUseActivePorts);
		trace (TRACE_LEVEL_DEVEL, string("m_databaseOverflowInterval : ") + m_databaseOverflowInterval);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigAlways : ") + m_defInfoOrigAlways);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigMetric : ") + m_defInfoOrigMetric);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigMetricType : ") + m_defInfoOrigMetricType);
		trace (TRACE_LEVEL_DEVEL, string("m_defInfoOrigRouteMap : ") + m_defInfoOrigRouteMap);
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
		trace (TRACE_LEVEL_DEVEL, string("m_replayCmdCodeBitmap : ") + m_replayCmdCodeBitmap);
		trace (TRACE_LEVEL_DEVEL, string("m_cmdType : ") + m_cmdType);
		trace (TRACE_LEVEL_DEVEL, string("m_maxMetricLinkType : ") + m_maxMetricLinkType);
		trace (TRACE_LEVEL_DEVEL, string("m_MaxMetricOnStartupTime : ") + m_maxMetricOnStartupTime);
		trace (TRACE_LEVEL_DEVEL, string("m_extMaxmetric : ") +m_extMaxMetric );
		trace (TRACE_LEVEL_DEVEL, string("m_summMaxMetric: ") +m_summMaxMetric );
		trace (TRACE_LEVEL_DEVEL, string("m_isAllVrfMaxMetricSet : ") +m_isAllVrfMaxMetricSet );
		trace (TRACE_LEVEL_DEVEL, string("m_isMaxMetricOnStartupWaitForBgp : ") +m_isMaxMetricOnStartupWaitForBgp );
		trace (TRACE_LEVEL_DEVEL, string("m_extMaxmetricLsa : ") +m_extMaxMetricLsa );
		trace (TRACE_LEVEL_DEVEL, string("m_summMaxMetricLsa: ") +m_summMaxMetricLsa );
	}

	void OSPFRouterConfigMessage::copyFromLocalMessage(OSPFRouterConfigLocalMessage *pMsg)
	{
		m_opCode = pMsg->m_opCode;
		m_cmdCode = pMsg->m_cmdCode;
		m_managedObjectId = pMsg->m_managedObjectId;
		m_vrf = pMsg->m_vrf;
		m_referenceBandwidth = pMsg->m_referenceBandwidth;
		m_refBwUseActivePorts = pMsg->m_refBwUseActivePorts;
		m_databaseOverflowInterval = pMsg->m_databaseOverflowInterval;
		m_defInfoOrigAlways = pMsg->m_defInfoOrigAlways;
		m_defInfoOrigMetric = pMsg->m_defInfoOrigMetric;
		m_defInfoOrigMetricType = pMsg->m_defInfoOrigMetricType;
		m_defInfoOrigRouteMap = pMsg->m_defInfoOrigRouteMap;
		m_defaultMetric = pMsg->m_defaultMetric;
		m_defaultPassiveInterface = pMsg->m_defaultPassiveInterface;
		m_distanceExternal = pMsg->m_distanceExternal;
		m_distanceInterArea = pMsg->m_distanceInterArea;
		m_distanceIntraArea = pMsg->m_distanceIntraArea;
		m_distListRouteMap = pMsg->m_distListRouteMap;
		m_domainId = pMsg->m_domainId;
		m_domainTag = pMsg->m_domainTag;
		m_extLSDBLimit = pMsg->m_extLSDBLimit;
		m_gracefulRestartHelper = pMsg->m_gracefulRestartHelper;
		m_gracefulRestartNotifyTime = pMsg->m_gracefulRestartNotifyTime;
		m_gracefulRestartRestartTime = pMsg->m_gracefulRestartRestartTime;
		m_maxMetricAllLSA = pMsg->m_maxMetricAllLSA;
		m_neighborAddress = pMsg->m_neighborAddress;
		m_isNSSATranslator = pMsg->m_isNSSATranslator;
		m_extMetricType = pMsg->m_extMetricType;
		m_redistributeConnectedRouteMap = pMsg->m_redistributeConnectedRouteMap;
		m_redistributeBgpRouteMap = pMsg->m_redistributeBgpRouteMap;
		m_redistributeStaticRouteMap = pMsg->m_redistributeStaticRouteMap;
		m_rfc1583Compatibility = pMsg->m_rfc1583Compatibility;
		m_rfc1587Compatibility = pMsg->m_rfc1587Compatibility;
		m_timerLsaGroupPacingInterval = pMsg->m_timerLsaGroupPacingInterval;
		m_timerInitDelay = pMsg->m_timerInitDelay;
		m_timerHoldTime = pMsg->m_timerHoldTime;
		m_timerMaxHoldTime = pMsg->m_timerMaxHoldTime;
		m_summaryAddress = pMsg->m_summaryAddress;
		m_summaryMask = pMsg->m_summaryMask;
		m_cmdType = pMsg->m_cmdType;
		m_maxMetricLinkType = pMsg->m_maxMetricLinkType;
		m_maxMetricOnStartupTime = pMsg->m_maxMetricOnStartupTime;
		m_extMaxMetric = pMsg->m_extMaxMetric;
		m_summMaxMetric = pMsg->m_summMaxMetric;
		m_isAllVrfMaxMetricSet = pMsg->m_isAllVrfMaxMetricSet;
		m_isMaxMetricOnStartupWaitForBgp  = pMsg->m_isMaxMetricOnStartupWaitForBgp;
		m_extMaxMetricLsa = pMsg->m_extMaxMetricLsa;
		m_summMaxMetricLsa = pMsg->m_summMaxMetricLsa;
        dumpMessage();
	}
}
