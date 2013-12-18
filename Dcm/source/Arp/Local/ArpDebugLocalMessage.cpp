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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#include "Arp/Local/ArpLocalObjectManager.h"
#include "Arp/Local/ArpDebugLocalMessage.h"
#include "Arp/Local/ArpTypes.h"

namespace DcmNs
{

    ArpDebugLocalMessage::ArpDebugLocalMessage ()
        : ManagementInterfaceMessage (ArpLocalObjectManager::getClassName (),ARPDEBUGLOCAL)
    {
    }

    ArpDebugLocalMessage::ArpDebugLocalMessage (const IpV4Address &IpAddress)
        : ManagementInterfaceMessage (ArpLocalObjectManager::getClassName (),ARPDEBUGLOCAL),
        m_IpAddress    (IpAddress)
    {
    }

    ArpDebugLocalMessage::~ArpDebugLocalMessage ()
    {
    }

    void  ArpDebugLocalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4Address(&m_IpAddress,"IpAddress"));
    }

    void  ArpDebugLocalMessage::setIpAddress(const IpV4Address &IpAddress)
    {
        m_IpAddress  =  IpAddress;
    }

    IpV4Address  ArpDebugLocalMessage::getIpAddress() const
    {
        return (m_IpAddress);
    }
    void  ArpDebugLocalMessage::setNoFlag(const UI32 &NoFlag)
    {
        m_NoFlag  =  NoFlag;
    }

    UI32 ArpDebugLocalMessage::getNoFlag() const
    {
        return (m_NoFlag);
    }

}
