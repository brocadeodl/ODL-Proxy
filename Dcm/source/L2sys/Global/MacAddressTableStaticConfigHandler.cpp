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

#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "L2sys/Global/MacAddressTableStaticConfigHandler.h"

#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "DcmResourceIdEnums.h"

#include "ClientInterface/L2sys/L2sysUtils.h"


#define TOKEN_INTERFACE_TYPE "interface_type"
#define TOKEN_IFNAME "ifname"
#define TOKEN_FORWARD "forward"
#define TOKEN_VLAN "vlan"
#define TOKEN_VLANID "vlanid"

#define UNKNOWN_STRING ""

namespace DcmNs
{


MacAddressTableStaticConfigHandler::MacAddressTableStaticConfigHandler  (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "static-mac-wysereaconfiguration", false),
      logger (getTraceClientId(), "MacAddressTableStaticConfigHandler::")
{
}
    
MacAddressTableStaticConfigHandler::~MacAddressTableStaticConfigHandler  ()
{
}

ManagementInterfaceMessage* MacAddressTableStaticConfigHandler::createConfig (
        WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    AttributeModel attributeModel = getAttributeModel(pWaveConfigurationSegmentCreateContext);

    // Check if there was any error in creating the attribute model.
    if (attributeModel._status != WAVE_MESSAGE_SUCCESS) {
        L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pMessage = new L2sysGlobalUpdateL2sysGlobalStaticMacMessage();
        pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(attributeModel._status);

        return (pMessage);
    }

    int opCode = L2SYS_SET_OP;

    unsigned int portChannelId = ( (attributeModel.interfaceType == L2SYS_PORT_CHANNEL)
            ? ((UI32) strtoul(attributeModel.ifName.c_str(), NULL, 0)) : 0 );

    L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pMessage = new L2sysGlobalUpdateL2sysGlobalStaticMacMessage(
            attributeModel.rBridge, attributeModel.macAddress, attributeModel.forward,
            attributeModel.interfaceType, attributeModel.ifName, attributeModel.vlan, attributeModel.vlanid,
            opCode, portChannelId);
    pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(WAVE_MESSAGE_SUCCESS);

    return (pMessage);
}

ManagementInterfaceMessage* MacAddressTableStaticConfigHandler::deleteConfig (
		WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
    AttributeModel attributeModel = getAttributeModel(pWaveConfigurationSegmentDeleteContext);

    // Check if there was any error in creating the attribute model.
    if (attributeModel._status != WAVE_MESSAGE_SUCCESS) {
        L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pMessage = new L2sysGlobalUpdateL2sysGlobalStaticMacMessage();
        pWaveConfigurationSegmentDeleteContext->setConfigHandlerStatus(attributeModel._status);

        return (pMessage);
    }

    int opCode = L2SYS_DELETE_OP;

    unsigned int portChannelId = ( (attributeModel.interfaceType == L2SYS_PORT_CHANNEL)
            ? ((UI32) strtoul(attributeModel.ifName.c_str(), NULL, 0)) : 0 );

    L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pMessage = new L2sysGlobalUpdateL2sysGlobalStaticMacMessage(
            attributeModel.rBridge, attributeModel.macAddress, attributeModel.forward,
            attributeModel.interfaceType, attributeModel.ifName, attributeModel.vlan, attributeModel.vlanid,
            opCode, portChannelId);
    pWaveConfigurationSegmentDeleteContext->setConfigHandlerStatus(WAVE_MESSAGE_SUCCESS);

    return (pMessage);
}

MacAddressTableStaticConfigHandler::AttributeModel MacAddressTableStaticConfigHandler::getAttributeModel (
        WaveConfigurationSegmentContext *pWaveConfigurationSegmentContext)
{
    vector<UI32> attributeUserTags = pWaveConfigurationSegmentContext->getAttributeUserTags ();
    vector<string> attributeNames  = pWaveConfigurationSegmentContext->getAttributeNames ();
    vector<string> attributeValues = pWaveConfigurationSegmentContext->getAttributeValues ();
	char macAddr[20];

    if (logger.isDebugEnabled()) {
        logger.debug ("getAttributeModel: attributeUserTags = ", attributeUserTags);
        logger.debug ("getAttributeModel: attributeNames = ", attributeNames);
        logger.debug ("getAttributeModel: attributeValues = ", attributeValues);
    }

    AttributeModel attributeModel;
    attributeModel._status = WAVE_MESSAGE_SUCCESS;

    if (attributeValues.size() == 0) {
        trace(TRACE_LEVEL_ERROR, "getAttributeModel: Empty attributeValues.");

        attributeModel._status = L2SYS_EMPTY_COMMAND_ERROR;
        return attributeModel;
    }

    // Whole command is passed as value for in each of the attributeValues elements, like
    // "1111.1111.1111 forward fortygigabitethernet 3/4 vlan 4".
    // Just pick the first value and tokenize the same to get individual values.
    string wholeCommand = attributeValues[0];

    vector<string> cmdTokens;
    StringUtils::tokenize(wholeCommand, cmdTokens);

    if (cmdTokens.size() < 6) {
        trace(TRACE_LEVEL_ERROR, string("getAttributeModel: Invalid command size = ") + cmdTokens.size()
                + ", command = " + wholeCommand);

        attributeModel._status = L2SYS_INCOMPLETE_COMMAND_ERROR;
        return attributeModel;
    }

	strcpy(macAddr, cmdTokens[0].c_str());
	for (unsigned int i = 0; i < strlen(macAddr); i++) {
		macAddr[i] = tolower(macAddr[i]);
	}
    attributeModel.macAddress = string(macAddr);

    attributeModel.forward = atoi(cmdTokens[1].c_str());
    attributeModel.forward = 1; // TODO: Remove this statement.

    string interfaceTypeStr = cmdTokens[2];
    if (interfaceTypeStr == "gigabitethernet") {
        attributeModel.interfaceType = L2SYS_GIGAETHERNET;
    } else if (interfaceTypeStr == "tengigabitethernet") {
        attributeModel.interfaceType = L2SYS_TENGIGAETHERNET;
    } else if (interfaceTypeStr == "fortygigabitethernet") {
        attributeModel.interfaceType = L2SYS_FORTYGIGAETHERNET;
    } else if (interfaceTypeStr == "port-channel") {
        attributeModel.interfaceType = L2SYS_PORT_CHANNEL;
    } else {
        // We should never reach here.
        trace(TRACE_LEVEL_ERROR, "getAttributeModel: Invalid interface Type = " + interfaceTypeStr);

        attributeModel._status = L2SYS_INVALID_INTERFACE_TYPE_ERROR;
        return attributeModel;
    }

    attributeModel.ifName = cmdTokens[3];
    attributeModel.rBridge = L2sysUtils::tell_rbrid (attributeModel.ifName.c_str());

    attributeModel.vlan = atoi(cmdTokens[4].c_str());
    attributeModel.vlan = 1;    // TODO: Remove this statement.

    attributeModel.vlanid = atoi(cmdTokens[5].c_str());

    trace(TRACE_LEVEL_DEBUG, string("getAttributeModel: attributeModel._status final = ") + attributeModel._status);

    return attributeModel;
}

// TODO: We should remove this function all together. And call FrameworkToolKit::localize directly from the
// wyser-write-framework.
string MacAddressTableStaticConfigHandler::getErrorString(ManagementInterfaceMessage* pMessage) {
    if (pMessage == NULL) {
        trace(TRACE_LEVEL_ERROR, string("getErrorString: pMessage is NULL"));
        return "";
    }

    return FrameworkToolKit::localize(pMessage->getCompletionStatus());
}

void MacAddressTableStaticConfigHandler::trace (TraceLevel traceLevel, const string &stringToTrace) {
    WaveConfigurationSegmentWorker::trace(traceLevel, "MacAddressTableStaticConfigHandler::" + stringToTrace);
}

}
