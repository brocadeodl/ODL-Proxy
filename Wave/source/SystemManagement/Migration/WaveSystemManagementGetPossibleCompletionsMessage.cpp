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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "SystemManagement/Migration/WaveSystemManagementGetPossibleCompletionsMessage.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/WaveSystemManagementTypes.h"

namespace WaveNs
{

WaveSystemManagementGetPossibleCompletionsMessage::WaveSystemManagementGetPossibleCompletionsMessage ()
    : ManagementInterfaceMessage (WaveSystemManagementObjectManager::getClassName (), WAVE_SYSTEM_MANAGEMENT_GET_POSSIBLE_COMPLETIONS_MESSAGE)
{
}

WaveSystemManagementGetPossibleCompletionsMessage::~WaveSystemManagementGetPossibleCompletionsMessage ()
{
}

void WaveSystemManagementGetPossibleCompletionsMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

void WaveSystemManagementGetPossibleCompletionsMessage::addCompletionNameAndInfoOneByOne (const string &completionName, const string &completionInfo)
{
    m_completionNameAndInfo.push_back (make_pair(completionName, completionInfo));
}

void WaveSystemManagementGetPossibleCompletionsMessage::addCompletionNameByVector (const vector<pair <string, string> > &completionNameAndInfo)
{
    m_completionNameAndInfo = completionNameAndInfo;
}

vector<pair<string, string> > WaveSystemManagementGetPossibleCompletionsMessage::getCompletionNameAndInfo () const
{
    return (m_completionNameAndInfo);
}

string WaveSystemManagementGetPossibleCompletionsMessage::getCommandName () const
{
    return (m_commandName);
}

void WaveSystemManagementGetPossibleCompletionsMessage::setCommandName (const string &commandName)
{
    m_commandName = commandName;
}

string WaveSystemManagementGetPossibleCompletionsMessage::getToken () const
{
    return (m_token);
}

void WaveSystemManagementGetPossibleCompletionsMessage::setToken (const string &token)
{
    m_token = token;
}
}
