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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Sajad Bashir Qadri                                           *
 ***************************************************************************/

#include <sstream>

#include "Arp/Local/ArpStaticConfigLocalMessage.h"
#include "Arp/Local/ArpStaticConfigHandler.h"

#include "ClientInterface/Arp/ArpMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "DcmResourceIdEnums.h"

#define TOKEN_DOMAIN_ID     "domainId"
#define TOKEN_VRF_NAME      "vrfName"

#define TOKEN_IP_ADDRESS    "ipAddress"
#define TOKEN_MAC_ADDRESS   "macAddress"
#define TOKEN_IF_TYPE       "ifType"
#define TOKEN_IF_NAME       "ifName"

#define TOKEN_COUNT_CREATE 4
#define TOKEN_COUNT_DELETE 1

#define GIGABIT_ETHERNET_ENUM_STRING "GigabitEthernet"
#define TEN_GIGABIT_ETHERNET_ENUM_STRING "TenGigabitEthernet"
#define FORTY_GIGABIT_ETHERNET_ENUM_STRING "FortyGigabitEthernet"
#define VLAN_ENUM_STRING "Ve"


#define UNKNOWN_IF_TYPE -1

namespace DcmNs
{

ArpStaticConfigHandler::ArpStaticConfigHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "ArpStaticConfigWyserEaConfiguration", false),
      logger (getTraceClientId(), "ArpStaticConfigHandler::")
{
}
    
ArpStaticConfigHandler::~ArpStaticConfigHandler  ()
{
}

ManagementInterfaceMessage* ArpStaticConfigHandler::createConfig (
        WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    trace(TRACE_LEVEL_DEVEL, "createConfig(...) called.");

    ArpStaticConfigLocalMessage *pMessage = new ArpStaticConfigLocalMessage();

    map<string, string> attributeMap = getAttributeMap(pWaveConfigurationSegmentCreateContext);
    if (attributeMap.size() < TOKEN_COUNT_CREATE) {
        tracePrintf( TRACE_LEVEL_ERROR, "ArpStaticConfigHandler::createConfig: Invalid attribute count: %d.", attributeMap.size());

        pMessage->setCompletionStatus(ARP_INVALID_COMMAND_ERROR);
        return pMessage;
    }

    pMessage->setIpAddress(attributeMap[TOKEN_IP_ADDRESS]);
    pMessage->setMacAdress(attributeMap[TOKEN_MAC_ADDRESS]);

    // TODO: Probably, we should set the rbridge-id & vrf name, generically in some parent class.
    if ((attributeMap[TOKEN_VRF_NAME] != "") && (attributeMap[TOKEN_DOMAIN_ID] != "")) {
        pMessage->setPartitionName(attributeMap[TOKEN_VRF_NAME]);
        pMessage->setPartitionLocationIdForPropagation(atoi(attributeMap[TOKEN_DOMAIN_ID].c_str()));

        pMessage->setIsPartitionContextPropagated (true);
    }
    else if (attributeMap[TOKEN_VRF_NAME] != "") {
        // We should never reach here. We should never got a vrf name, but not domain id (rbridge-id).
        trace (TRACE_LEVEL_ERROR, "createConfig: VRF name passed but no domainId (rbridge-id)");
        pMessage->setCompletionStatus(ARP_INVALID_RBRIDGE_ERROR);
        return pMessage;
    }

    if (attributeMap[TOKEN_DOMAIN_ID] != "") {
        pWaveConfigurationSegmentCreateContext->setLocationId(DcmToolKit::getLocationIdFromRbridgeId(
                atoi(attributeMap[TOKEN_DOMAIN_ID].c_str())));
    }

    int status = setInterface(pMessage, attributeMap);
    if (status != WAVE_MESSAGE_SUCCESS) {
        pMessage->setCompletionStatus(status);
        return pMessage;
    }

    // Marks this message as a create-message.
    pMessage->setOpCode(false);

    pMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

    return (pMessage);
}

ManagementInterfaceMessage* ArpStaticConfigHandler::deleteConfig (
        WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
    trace(TRACE_LEVEL_DEVEL, "deleteConfig(...) called.");

    ArpStaticConfigLocalMessage *pMessage = new ArpStaticConfigLocalMessage();

    map<string, string> attributeMap = getAttributeMap(pWaveConfigurationSegmentDeleteContext);
    if (attributeMap.size() < TOKEN_COUNT_DELETE) {
        tracePrintf( TRACE_LEVEL_ERROR, "ArpStaticConfigHandler::deleteConfig: Invalid attribute count: %d.", attributeMap.size());

        pMessage->setCompletionStatus(ARP_INVALID_COMMAND_ERROR);
        return pMessage;
    }

    pMessage->setIpAddress(attributeMap[TOKEN_IP_ADDRESS]);

    // TODO: Probably, we should set the rbridge-id & vrf name, generically in some parent class.
    if ((attributeMap[TOKEN_VRF_NAME] != "") && (attributeMap[TOKEN_DOMAIN_ID] != "")) {
        pMessage->setPartitionName(attributeMap[TOKEN_VRF_NAME]);
        pMessage->setPartitionLocationIdForPropagation(atoi(attributeMap[TOKEN_DOMAIN_ID].c_str()));

        pMessage->setIsPartitionContextPropagated (true);
    }
    else if (attributeMap[TOKEN_VRF_NAME] != "") {
        // We should never reach here. We should never got a vrf name, but not domain id (rbridge-id).
        trace (TRACE_LEVEL_ERROR, "deleteConfig: VRF name passed but no domainId (rbridge-id)");
        pMessage->setCompletionStatus(ARP_INVALID_RBRIDGE_ERROR);
        return pMessage;
    }

    if (attributeMap[TOKEN_DOMAIN_ID] != "") {
        pWaveConfigurationSegmentDeleteContext->setLocationId(DcmToolKit::getLocationIdFromRbridgeId(
                atoi(attributeMap[TOKEN_DOMAIN_ID].c_str())));
    }

    // Marks this message as delete-message.
    pMessage->setOpCode(true);

    pMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

    return (pMessage);
}

map<string, string> ArpStaticConfigHandler::getAttributeMap (
        WaveConfigurationSegmentContext *pWaveConfigurationSegmentContext)
{

    vector<UI32> attributeUserTags = pWaveConfigurationSegmentContext->getAttributeUserTags ();
    vector<string> attributeNames  = pWaveConfigurationSegmentContext->getAttributeNames ();
    vector<string> attributeValues = pWaveConfigurationSegmentContext->getAttributeValues ();

    if (logger.isDebugEnabled())
    {
        logger.debug ("getAttributeMap: attributeUserTags = ", attributeUserTags);
        logger.debug ("getAttributeMap: attributeNames = ", attributeNames);
        logger.debug ("getAttributeMap: attributeValues = ", attributeValues);
    }

    /**
     * Attributes are passed like below (if executed outside):
     *
     * attributeUserTags = [1335235926, 0, 0, 0, ]
     * attributeNames = [IpAddress, 1111.1111.1111, interface, TenGigabitEthernet, 0/22, ]
     * attributeValues = [11.11.11.11, 1, 1, 1, 1, ]
     *
     * Or like below (if executed inside r-bridge):
     * attributeUserTags = [23403572, 1335235926, 0, 0, 0, ]
     * attributeNames = [domainId, IpAddress, 1234.1234.1234, interface, TenGigabitEthernet, 0/23, ]
     * attributeValues = [1, 11.11.11.11, 1, 1, 1, 1, ]
     *
     * Or like below (if executed inside vrf, as "no" command):
     * attributeUserTags = [23403572, 2140916904, 904026399, 1335235926]
     * attributeNames = [domainId, vrfName, maxRoutes, IpAddress]
     * attributeValues = [1, abcd, 3600, 1.2.3.4]
     */

    map<string, string> attributeMap;

    if (attributeNames.size() == 0 || attributeValues.size() == 0)
    {
        trace(TRACE_LEVEL_ERROR, "getAttributeModel: Incomplete command.");

        return attributeMap;
    }

    // Find the "domainId" item (if present), in the passed attributes.
    UI32 attributesCount = attributeNames.size();
    UI32 i = 0;
    for (; i < attributesCount; i++)
    {
        if (attributeNames[i] == "domainId")
        {
            attributeMap[TOKEN_DOMAIN_ID] = attributeValues[i];
            break;
        }
    }

    // Find the "vrfName" item (if present), in the passed attributes.
    for (i = 0; i < attributesCount; i++)
    {
        if (attributeNames[i] == "vrfName")
        {
            attributeMap[TOKEN_VRF_NAME] = attributeValues[i];
            break;
        }
    }

    // Find the location of "IpAddress" item.
    for (i = 0; i < attributesCount; i++)
    {
        if (attributeNames[i] == "IpAddress")
        {
            break;
        }
    }

    attributeMap[TOKEN_IP_ADDRESS] = attributeValues[i + 0];

    // If there are no more attributes after "IpAddress", then this may be a "no" command,
    // getting only key attribute (i.e., ip-address token).
    if ((i + 1) >= attributesCount)
    {
        logger.debug(string("getAttributeModel: Probably a \"no\" command. attribute count = "
                + attributeNames.size()));

        return (attributeMap);
    }

    attributeMap[TOKEN_MAC_ADDRESS] = attributeNames[i + 1];
    attributeMap[TOKEN_IF_TYPE] = attributeNames[i + 3];
    attributeMap[TOKEN_IF_NAME] = attributeNames[i + 4];

    logger.debug ("attributeMap = ", attributeMap);

    return (attributeMap);
}

int ArpStaticConfigHandler::setInterface(ArpStaticConfigLocalMessage *pMessage, map<string, string> attributeMap) {
    string ifType = attributeMap[TOKEN_IF_TYPE];
    string ifName = attributeMap[TOKEN_IF_NAME];

    if (ifType == GIGABIT_ETHERNET_ENUM_STRING) {
        pMessage->setInterfaceType(CONFIG_ARP_GIGABIT);
        pMessage->setGigabitValue(ifName);
    }
    else if (ifType == TEN_GIGABIT_ETHERNET_ENUM_STRING) {
        pMessage->setInterfaceType(CONFIG_ARP_TENGIGABIT);
        pMessage->setTenGigabitValue(ifName);
    }
    else if (ifType == FORTY_GIGABIT_ETHERNET_ENUM_STRING) {
        pMessage->setInterfaceType(CONFIG_ARP_FORTYGIGABIT);
        pMessage->setFortyGigabitValue(ifName);
    }
    else if (ifType == VLAN_ENUM_STRING) {
        pMessage->setInterfaceType(CONFIG_ARP_VLAN);
        pMessage->setVlanValue(atoi(ifName.c_str()));
    }
    else {
        // We should never reach here.
        trace (TRACE_LEVEL_ERROR, string ("getInterfaceType: Unknown Interface Type: ") + ifType);
        return ARP_INVALID_IF_TYPE_ERROR;
    }

    return WAVE_MESSAGE_SUCCESS;
}

void ArpStaticConfigHandler::trace (TraceLevel traceLevel, const string &stringToTrace) {
    WaveConfigurationSegmentWorker::trace(traceLevel, "ArpStaticConfigHandler::" + stringToTrace);
}

}
