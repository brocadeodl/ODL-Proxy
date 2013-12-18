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
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Framework/Types/IpV4AddressUC.h"
#include "Framework/Utils/AssertUtils.h"

#include <sys/socket.h>

namespace WaveNs
{

IpV4AddressUC::IpV4AddressUC ()
    : m_pIpV4Address("127.0.0.1"), m_pIsUserConfigured(false)
{
}

IpV4AddressUC::IpV4AddressUC (const IpV4Address &ipV4Address)
    : m_pIpV4Address(ipV4Address), m_pIsUserConfigured(false)
{
}

IpV4AddressUC::IpV4AddressUC (const IpV4Address &ipV4Address, const bool &isUserConfigured)
    : m_pIpV4Address(ipV4Address), m_pIsUserConfigured(isUserConfigured)
{
}

IpV4AddressUC::IpV4AddressUC (const IpV4AddressUC &ipV4AddressUC)
{
    m_pIpV4Address      = ipV4AddressUC.m_pIpV4Address;
    m_pIsUserConfigured = ipV4AddressUC.m_pIsUserConfigured;
}

IpV4AddressUC::~IpV4AddressUC ()
{
}

UI32 IpV4AddressUC::getAddressFamily ()
{
    return m_pIpV4Address.getAddressFamily();
}

void IpV4AddressUC::setIpV4AddressValue (const IpV4Address &pIpV4Address)
{
    m_pIpV4Address = pIpV4Address;
}

IpV4Address IpV4AddressUC::getIpV4AddressValue () const
{
    return (m_pIpV4Address);
}

void IpV4AddressUC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool IpV4AddressUC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

string IpV4AddressUC::toString ()
{
    char   buffer[256] = {0};
    string tempString;
    UI32   stringLength = m_pIpV4Address.toString().size();

    snprintf (buffer, 256, "%u#%s:%u", stringLength, m_pIpV4Address.toString().c_str(), (m_pIsUserConfigured ? 1 : 0));

    tempString = buffer;

    return (tempString);
}

void IpV4AddressUC::getPlainString (string &valueString) const
{
    valueString = m_pIpV4Address.toString();
}


void IpV4AddressUC::fromString (const string &pIpV4AddressUCInStringFormat)
{
    UI32   stringLength;
    char   buffer[256];
    string tempString;
    UI32   tempIsUserConfigured;

    sscanf (pIpV4AddressUCInStringFormat.c_str(), "%u#%s", &stringLength, buffer);

    tempString = buffer;

    if (stringLength < tempString.size())
    {
        string tempStringForIsConfigured = tempString.substr((stringLength + 1));

        prismAssert (1 == tempStringForIsConfigured.size(), __FILE__, __LINE__);

        tempIsUserConfigured = strtoul(tempStringForIsConfigured.c_str (), NULL, 10);
        m_pIsUserConfigured  = tempIsUserConfigured ? true : false;
    }

    m_pIpV4Address.fromString(tempString.substr(0, stringLength));
}


ResourceId IpV4AddressUC::loadFromPlainString (const string &pIpV4AddressUCInPlainStringFormat)
{
    m_pIsUserConfigured  = true;

    return (m_pIpV4Address.loadFromPlainString (pIpV4AddressUCInPlainStringFormat));
}


bool IpV4AddressUC::operator == (const IpV4AddressUC &pIpV4AddressUC)  const 
{
    if ((m_pIpV4Address != pIpV4AddressUC.m_pIpV4Address) || (m_pIsUserConfigured != pIpV4AddressUC.m_pIsUserConfigured))
    {
        return (false);
    }

    return (true);
}

bool IpV4AddressUC::operator != (const IpV4AddressUC &pIpV4AddressUC)  const 
{
    return (! (operator == (pIpV4AddressUC)));
}

IpV4AddressUC & IpV4AddressUC::operator  = (const IpV4AddressUC &pIpV4AddressUC)
{
    m_pIpV4Address      = pIpV4AddressUC.m_pIpV4Address;
    m_pIsUserConfigured = pIpV4AddressUC.m_pIsUserConfigured;

    return (*this);
}

}
