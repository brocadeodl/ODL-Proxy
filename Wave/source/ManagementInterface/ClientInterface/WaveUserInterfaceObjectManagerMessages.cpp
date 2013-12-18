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
 *   Author : acshetty                                                     *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManagerTypes.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManagerMessages.h"

namespace WaveNs
{

WaveUpdateClientStatusMessage::WaveUpdateClientStatusMessage()
: ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), WAVE_UPDATE_CLIENT_STATUS),
  m_terminalMonitorStatus (false)
{
}

WaveUpdateClientStatusMessage::WaveUpdateClientStatusMessage( const bool terminalMonitorStatus)
: ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), WAVE_UPDATE_CLIENT_STATUS),
  m_terminalMonitorStatus (terminalMonitorStatus)
{
}

WaveUpdateClientStatusMessage::~WaveUpdateClientStatusMessage ()
{
}

void WaveUpdateClientStatusMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_terminalMonitorStatus, "terminalMonitorStatus"));
}

bool WaveUpdateClientStatusMessage::getTerminalMonitorStatus () const
{
    return (m_terminalMonitorStatus);
}

void WaveUpdateClientStatusMessage::setTerminalMonitorStatus (const bool &terminalMonitorStatus)
{
    m_terminalMonitorStatus = terminalMonitorStatus;
}

}
