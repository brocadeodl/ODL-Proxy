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
#include "Framework/Attributes/AttributeEnum.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/SecServicesLocalConfigMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

    TelnetServerLocalConfigMessage::TelnetServerLocalConfigMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), TELNETSERVERLOCALCONFIG)
    {
    }

    TelnetServerLocalConfigMessage::TelnetServerLocalConfigMessage (const bool &disableFlag)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), TELNETSERVERLOCALCONFIG),
        m_disableFlag    (disableFlag)
    {
    }

    TelnetServerLocalConfigMessage::~TelnetServerLocalConfigMessage ()
    {
    }

    void  TelnetServerLocalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_disableFlag, "disableFlag"));
    }

    void  TelnetServerLocalConfigMessage::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  TelnetServerLocalConfigMessage::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerLocalConfigMessage::SSHServerLocalConfigMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLOCALCONFIG)
    {
    }

    SSHServerLocalConfigMessage::SSHServerLocalConfigMessage (const bool &disableFlag)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLOCALCONFIG),
        m_disableFlag  (disableFlag)
    {
    }

    void  SSHServerLocalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_disableFlag, "disableFlag"));
    }

    void  SSHServerLocalConfigMessage::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  SSHServerLocalConfigMessage::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerLocalConfigMessage::~SSHServerLocalConfigMessage ()
    {
    }

    SSHServerListLocalAddMessage::SSHServerListLocalAddMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLISTLOCALADD)
    {
    }

    SSHServerListLocalAddMessage::SSHServerListLocalAddMessage (const SSHLocalKeyExchangeProtocol &protocol)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLISTLOCALADD),
        m_protocol  (protocol)
    {
    }

    void  SSHServerListLocalAddMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_protocol), "protocol"));
    }

    void  SSHServerListLocalAddMessage::setKeyExchangeProtocol(const SSHLocalKeyExchangeProtocol &protocol)
    {
        m_protocol  =  protocol;
    }

    SSHLocalKeyExchangeProtocol SSHServerListLocalAddMessage::getKeyExchangeProtocol() const
    {
        return (m_protocol);
    }

    SSHServerListLocalAddMessage::~SSHServerListLocalAddMessage ()
    {
    }

    SSHServerListLocalDeleteMessage::SSHServerListLocalDeleteMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLISTLOCALDELETE)
    {
    }

    SSHServerListLocalDeleteMessage::SSHServerListLocalDeleteMessage (const SSHLocalKeyExchangeProtocol &protocol)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLISTLOCALDELETE),
        m_protocol  (protocol)
    {
    }

    void  SSHServerListLocalDeleteMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_protocol), "protocol"));
    }

    void  SSHServerListLocalDeleteMessage::setKeyExchangeProtocol(const SSHLocalKeyExchangeProtocol &protocol)
    {
        m_protocol  =  protocol;
    }

    SSHLocalKeyExchangeProtocol SSHServerListLocalDeleteMessage::getKeyExchangeProtocol() const
    {
        return (m_protocol);
    }

    SSHServerListLocalDeleteMessage::~SSHServerListLocalDeleteMessage ()
    {
    }
}
