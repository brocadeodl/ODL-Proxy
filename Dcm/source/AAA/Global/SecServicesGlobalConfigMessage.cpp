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
 *   Author : Balaji. M                                                    *
 **************************************************************************/

#if 0
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/SecServicesGlobalConfigMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    TelnetServerGlobalConfigMessage::TelnetServerGlobalConfigMessage ()
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), TELNETSERVERGLOBALCONFIG)
    {
    }

    TelnetServerGlobalConfigMessage::TelnetServerGlobalConfigMessage (const bool &disableFlag)
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), TELNETSERVERGLOBALCONFIG),
        m_disableFlag    (disableFlag)
    {
    }

    TelnetServerGlobalConfigMessage::~TelnetServerGlobalConfigMessage ()
    {
    }

    void  TelnetServerGlobalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_disableFlag, "disableFlag"));
    }

    void  TelnetServerGlobalConfigMessage::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  TelnetServerGlobalConfigMessage::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerGlobalConfigMessage::SSHServerGlobalConfigMessage ()
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), SSHSERVERGLOBALCONFIG)
    {
    }

    SSHServerGlobalConfigMessage::SSHServerGlobalConfigMessage (const bool &disableFlag)
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), SSHSERVERGLOBALCONFIG),
        m_disableFlag    (disableFlag)
    {
    }

    void  SSHServerGlobalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_disableFlag, "disableFlag"));
    }

    void  SSHServerGlobalConfigMessage::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  SSHServerGlobalConfigMessage::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerGlobalConfigMessage::~SSHServerGlobalConfigMessage ()
    {
    }

    SSHServerListGlobalAddMessage::SSHServerListGlobalAddMessage ()
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), SSHSERVERLISTGLOBALADD)
    {
    }

    SSHServerListGlobalAddMessage::SSHServerListGlobalAddMessage (const SSHKeyExchangeProtocol &protocol)
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), SSHSERVERLISTGLOBALADD),
        m_protocol    (protocol)
    {
    }

    void  SSHServerListGlobalAddMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_protocol), "protocol"));
    }

    void  SSHServerListGlobalAddMessage::setKeyExchangeProtocol(const SSHKeyExchangeProtocol &protocol)
    {
        m_protocol = protocol;
    }

    SSHKeyExchangeProtocol SSHServerListGlobalAddMessage::getKeyExchangeProtocol() const
    {
        return (m_protocol);
    }

    SSHServerListGlobalAddMessage::~SSHServerListGlobalAddMessage ()
    {
    }

    SSHServerListGlobalDeleteMessage::SSHServerListGlobalDeleteMessage ()
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), SSHSERVERLISTGLOBALDELETE)
    {
    }

    SSHServerListGlobalDeleteMessage::SSHServerListGlobalDeleteMessage (const SSHKeyExchangeProtocol &protocol)
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), SSHSERVERLISTGLOBALDELETE),
        m_protocol    (protocol)
    {
    }

    void  SSHServerListGlobalDeleteMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_protocol), "protocol"));
    }

    void  SSHServerListGlobalDeleteMessage::setKeyExchangeProtocol(const SSHKeyExchangeProtocol &protocol)
    {
        m_protocol = protocol;
    }

    SSHKeyExchangeProtocol SSHServerListGlobalDeleteMessage::getKeyExchangeProtocol() const
    {
        return (m_protocol);
    }

    SSHServerListGlobalDeleteMessage::~SSHServerListGlobalDeleteMessage ()
    {
    }
}
#endif
