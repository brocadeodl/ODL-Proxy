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
 *   Author : Kumar Ghanta                                                 *
 **************************************************************************/

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Local/SecServicesLocalShowMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

    TelnetServerLocalShowMessage::TelnetServerLocalShowMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), TELNETSERVERLOCALSHOW)
    {
    }

    TelnetServerLocalShowMessage::TelnetServerLocalShowMessage (const bool &disableFlag)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), TELNETSERVERLOCALSHOW),
    	m_disableFlag(disableFlag)
    {
    }

    void TelnetServerLocalShowMessage::setupAttributesForSerialization()
    {
	    PrismMessage::setupAttributesForSerialization ();
	    addSerializableAttribute (new AttributeBool(&m_disableFlag, "disableFlag"));
    }

    void TelnetServerLocalShowMessage::setDisableFlag (const bool &disableFlag)
    {
	    m_disableFlag = disableFlag;
    }

    bool TelnetServerLocalShowMessage::getDisableFlag ()
    {
	    return m_disableFlag;
    }

    TelnetServerLocalShowMessage::~TelnetServerLocalShowMessage ()
    {
    }

    SSHServerLocalShowMessage::SSHServerLocalShowMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLOCALSHOW)
    {
    }

    SSHServerLocalShowMessage::SSHServerLocalShowMessage (const bool &disableFlag)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLOCALSHOW),
    	m_disableFlag(disableFlag)
    {
    }

    void SSHServerLocalShowMessage::setupAttributesForSerialization()
    {
	    PrismMessage::setupAttributesForSerialization ();
	    addSerializableAttribute (new AttributeBool(&m_disableFlag, "disableFlag"));
    }

    void SSHServerLocalShowMessage::setDisableFlag (const bool &disableFlag)
    {
	    m_disableFlag = disableFlag;
    }

    bool SSHServerLocalShowMessage::getDisableFlag ()
    {
	    return m_disableFlag;
    }

    SSHServerLocalShowMessage::~SSHServerLocalShowMessage ()
    {
    }

    SSHServerListLocalShowMessage::SSHServerListLocalShowMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLISTLOCALSHOW)
    {
    }

    SSHServerListLocalShowMessage::SSHServerListLocalShowMessage (const SSHLocalKeyExchangeProtocol &protocol)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), SSHSERVERLISTLOCALSHOW),
    	m_protocol  (protocol)
    {
    }

    void SSHServerListLocalShowMessage::setupAttributesForSerialization()
    {
	    PrismMessage::setupAttributesForSerialization ();
	    addSerializableAttribute (new AttributeEnum((UI32 *)(&m_protocol), "protocol"));
    }


    void SSHServerListLocalShowMessage::setKeyExchangeProtocol (const SSHLocalKeyExchangeProtocol &protocol)
    {
	    m_protocol = protocol;
    }

    SSHLocalKeyExchangeProtocol SSHServerListLocalShowMessage::getKeyExchangeProtocol ()
    {
	    return m_protocol;
    }

    SSHServerListLocalShowMessage::~SSHServerListLocalShowMessage ()
    {
    }

}
