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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Sajad Bashir Qadri                                           *
 ***************************************************************************/

#include <sstream>
#include <cstdlib>

#include "L2sys/Global/L2sysGlobalUpdateL2sysAgingTimeoutMessage.h"
#include "L2sys/Global/L2sysGlobalTypes.h"
#include "L2sys/Global/MacAddressTableAgingTimeConfigHandler.h"

#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "DcmCore/DcmToolKit.h"

#include "DcmResourceIdEnums.h"

#define UNKNOWN -1

#define L2SYS_SA_MIN_MAC_AGING 10

#define TOKEN_AGING_TIME "aging_time"

namespace DcmNs
{


MacAddressTableAgingTimeConfigHandler::MacAddressTableAgingTimeConfigHandler (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationSegmentWorker (pWaveObjectManager, "mac-aging-wysereaconfiguration", false)
{
}
    
MacAddressTableAgingTimeConfigHandler::~MacAddressTableAgingTimeConfigHandler ()
{
}

ResourceId MacAddressTableAgingTimeConfigHandler::validateAgingTime(
        int agingTime) {
    // Check whether the provided aging-times is really valid.
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    if (agingTime > L2SYS_VCS_MAX_MAC_AGING) {
        status = WRC_L2SYS_INVALID_VCS_MAC_AGING;
    } else if (agingTime != 0) {
        if ((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()
                && agingTime < L2SYS_VCS_MIN_MAC_AGING)
                || (WAVE_PERSISTENCE_CLUSTER_ENABLED
                        != DcmToolKit::isVcsEnabled()
                        && agingTime < L2SYS_SA_MIN_MAC_AGING)) {
            status = WRC_L2SYS_INVALID_VCS_MAC_AGING;
        }
    }

    return status;
}

ManagementInterfaceMessage *MacAddressTableAgingTimeConfigHandler::createConfig (
        WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    L2sysGlobalUpdateL2sysAgingTimeoutMessage *pMessage = new  L2sysGlobalUpdateL2sysAgingTimeoutMessage ();

    AttributeModel attributeModel = getAttributeModel(pWaveConfigurationSegmentCreateContext);

    // Check whether "aging_time" attribute is present or not. If not throw error immediately.
    if (attributeModel.agingTime == UNKNOWN) {
        pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(L2SYS_INCOMPLETE_COMMAND_ERROR);

        return (pMessage);
    }

    // Check whether the provided aging-times is really valid.
    ResourceId status = validateAgingTime(attributeModel.agingTime);
    if ( status != WAVE_MESSAGE_SUCCESS) {
        trace (TRACE_LEVEL_ERROR, "createConfig: Invalid agingTime = " + attributeModel.agingTime);

        pWaveConfigurationSegmentCreateContext->setConfigHandlerStatus(status);

        return (pMessage);
    }

    pMessage->setAgingTimeout(attributeModel.agingTime);

    return (pMessage);
}

ManagementInterfaceMessage *MacAddressTableAgingTimeConfigHandler::deleteConfig (
        WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
    // TODO: Check whether the currently set aging time is really what the user passed. Throw error otherwise.
    // But, we can do this check in the plug-in itself, instead of doing it here.

    L2sysGlobalUpdateL2sysAgingTimeoutMessage *pMessage = new  L2sysGlobalUpdateL2sysAgingTimeoutMessage (
            L2SYS_DCM_MAC_AGING_DEFAULT);

    return (pMessage);
}

MacAddressTableAgingTimeConfigHandler::AttributeModel MacAddressTableAgingTimeConfigHandler::getAttributeModel (
        WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    vector<UI32> attributeUserTags = pWaveConfigurationSegmentCreateContext->getAttributeUserTags ();
    vector<string> attributeNames  = pWaveConfigurationSegmentCreateContext->getAttributeNames ();
    vector<string> attributeValues = pWaveConfigurationSegmentCreateContext->getAttributeValues ();

    printVector("getAttributeModel: attributeUserTags = [", attributeUserTags);
    printSVector("getAttributeModel: attributeNames = [", attributeNames);
    printSVector("getAttributeModel: attributeValues = [", attributeValues);

    AttributeModel attributeModel;

    if (attributeNames.size() == 0) {   // TODO: We should be using attributeValues here, instead of attributeNames
        attributeModel.agingTime = UNKNOWN;
    }

    // Whole command is passed as value for in each of the attributeValues elements
    // Just pick the first value and tokenize the same to get individual values.
    string wholeCommand = attributeNames[0];    // TODO: We should be using attributeValues here, instead of attributeNames

    vector<string> cmdTokens;
    StringUtils::tokenize(wholeCommand, cmdTokens);

    if (cmdTokens.size() == 0) {
       attributeModel.agingTime = UNKNOWN;
       return attributeModel;
    }

    attributeModel.agingTime = atoi(cmdTokens[0].c_str());

    return attributeModel;
}

void MacAddressTableAgingTimeConfigHandler::trace (TraceLevel traceLevel, const string &stringToTrace) {
    WaveConfigurationSegmentWorker::trace(traceLevel, "MacAddressTableAgingTimeConfigHandler::" + stringToTrace);
}

void MacAddressTableAgingTimeConfigHandler::printVector(string label, vector<UI32>& inputVector) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    ss << label << "[";
    for (vector<UI32>::iterator it = inputVector.begin();
            it != inputVector.end(); ++it) {
        ss << *it << ", ";
    }
    ss << "]\n";
    trace (TRACE_LEVEL_DEBUG, ss.str());
}

void MacAddressTableAgingTimeConfigHandler::printSVector(string label,   vector<string>& inputVector) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    ss << label << "[";
    for (vector<string>::iterator it = inputVector.begin();
            it != inputVector.end(); ++it) {
        ss << *it << ", ";
    }
    ss << "]\n";
    trace (TRACE_LEVEL_DEBUG, ss.str());
}


}
