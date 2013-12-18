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

#include "SystemManagement/WaveSystemManagementGatewayMessage.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/WaveSystemManagementTypes.h"

namespace WaveNs
{

WaveSystemManagementGatewayMessage::WaveSystemManagementGatewayMessage ()
    : ManagementInterfaceMessage (WaveSystemManagementObjectManager::getClassName (), WAVE_SYSTEM_MANAGEMENT_GATEWAY_MESSAGE)
{
}

WaveSystemManagementGatewayMessage::~WaveSystemManagementGatewayMessage ()
{
}

void WaveSystemManagementGatewayMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_writeInputArguments,      "writeInputArguments"));
    addSerializableAttribute (new AttributeString (&m_writeSubMode,             "writeSubMode"));
    addSerializableAttribute (new AttributeString (&m_errorString,              "errorString"));
    addSerializableAttribute (new AttributeBool   (&m_isWyserEaCompatibilityMode, "isWyserEaCompatibilityMode"));
}

string WaveSystemManagementGatewayMessage::getWriteInputArguments ()
{
    return (m_writeInputArguments);
}
        
void WaveSystemManagementGatewayMessage::setWriteInputArguments (string writeInputArguments)
{
    m_writeInputArguments = writeInputArguments;
}

string WaveSystemManagementGatewayMessage::getWriteSubMode ()
{
    return (m_writeSubMode);
}
        
void WaveSystemManagementGatewayMessage::setWriteSubMode (string writeSubMode)
{
    m_writeSubMode = writeSubMode;
}

string WaveSystemManagementGatewayMessage::getCommandErrorString () const
{
    return (m_errorString);
}

void WaveSystemManagementGatewayMessage::setCommandErrorString (const string &errorString)
{
	m_errorString = errorString;
}

bool WaveSystemManagementGatewayMessage::getIsWyserEaCompatibilityMode() const
{
    return (m_isWyserEaCompatibilityMode);
}

void WaveSystemManagementGatewayMessage::setIsWyserEaCompatibilityMode (const bool &isWyserEaCompatibilityMode)
{
	m_isWyserEaCompatibilityMode = isWyserEaCompatibilityMode;
}


}
