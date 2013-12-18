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

#include "Framework/Types/HostUC.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include <arpa/inet.h>

namespace WaveNs
{

HostUC::HostUC ()
    : m_domainName (""), 
      m_ipV4Address (""),
      m_ipV6Address (""),
      m_hostType (WAVE_UNKNOWN_HOSTTYPE),
      m_pIsUserConfigured (false)
{
}

HostUC::HostUC (const string &domainName)
    : m_domainName (domainName), 
      m_ipV4Address (""),
      m_ipV6Address (""),
      m_hostType (WAVE_DOMAINNAME),
      m_pIsUserConfigured (false)
{
}

HostUC::HostUC (const string &domainName, const bool &isUserConfigured)
    : m_domainName (domainName),
      m_ipV4Address (""),
      m_ipV6Address (""),
      m_hostType (WAVE_DOMAINNAME),
      m_pIsUserConfigured (isUserConfigured)
{
}

HostUC::HostUC (const IpV4Address &ipV4Address)
    : m_domainName (""), 
      m_ipV4Address (ipV4Address), 
      m_ipV6Address (""),
      m_hostType (WAVE_IPV4ADDRESS),
      m_pIsUserConfigured (false)
{
}

HostUC::HostUC (const IpV4Address &ipV4Address, const bool &isUserConfigured)
    : m_domainName (""),
      m_ipV4Address (ipV4Address),
      m_ipV6Address (""),
      m_hostType (WAVE_IPV4ADDRESS),
      m_pIsUserConfigured (isUserConfigured)
{
}

HostUC::HostUC (const IpV6Address &ipV6Address)
    : m_domainName (""),
      m_ipV4Address (""),
      m_ipV6Address (ipV6Address), 
      m_hostType (WAVE_IPV6ADDRESS),
      m_pIsUserConfigured (false)
{
}

HostUC::HostUC (const IpV6Address &ipV6Address, const bool &isUserConfigured)
    : m_domainName (""),
      m_ipV4Address (""),
      m_ipV6Address (ipV6Address),
      m_hostType (WAVE_IPV6ADDRESS),
      m_pIsUserConfigured (isUserConfigured)
{
}

HostUC::HostUC (const HostUC &hostUC)
{
    m_hostType = hostUC.m_hostType;
    m_domainName = hostUC.m_domainName;
    m_ipV4Address = hostUC.m_ipV4Address;
    m_ipV6Address = hostUC.m_ipV6Address;
    m_pIsUserConfigured = hostUC.m_pIsUserConfigured;	
}

HostUC::~HostUC ()
{
}

void HostUC::setHostValue (const string &pDomainName)
{
    m_hostType = WAVE_DOMAINNAME;
    m_domainName = pDomainName;
}

void HostUC::setHostValue (const IpV4Address &ipV4Address)
{
    m_hostType = WAVE_IPV4ADDRESS;
    m_ipV4Address = ipV4Address;
}

void HostUC::setHostValue (const IpV6Address &ipV6Address)
{
    m_hostType = WAVE_IPV6ADDRESS;
    m_ipV6Address = ipV6Address;
}

string HostUC::getHostValue () const
{
    if (m_hostType == WAVE_DOMAINNAME)
    {
        return (m_domainName);
    }
    else if (m_hostType == WAVE_IPV4ADDRESS)
    {
        return (m_ipV4Address.toString ());
    }
    else if (m_hostType == WAVE_IPV6ADDRESS)
    {
        return (m_ipV6Address.toString ());
    }
    else 
    {
        return (string(""));
    }
}

HostType HostUC::getHostType() const
{
    return (m_hostType);
}

bool HostUC::isValidIpV4Address (const string &ipV4Address)
{
    UI32 i1, i2, i3, i4;

    if (sscanf (ipV4Address.c_str (), "%u.%u.%u.%u", &i1, &i2, &i3, &i4) != 4) 
    {
        return (false);
    }

    if ((i1 > 255) || (i2 > 255) || (i3 > 255) || (i4 > 255)) 
    {
        return (false);
    }

    return (true);
}

bool HostUC::isValidIpV6Address (const string &ipV6Address)
{
    UI32    colonCount = 0;
    size_t  position = 0;

    position = ipV6Address.find (":");
    
    while (position != string::npos)
    {
        colonCount++;
        position = ipV6Address.find (":", position + 1);
    }

    if(colonCount < 2)
    {
        return (false);
    }

    return (true);
}

UI32 HostUC::getAddressFamily ()
{
    if (WAVE_IPV4ADDRESS == m_hostType)
    {
        return (AF_INET);
    }
    else if (WAVE_IPV6ADDRESS == m_hostType)
    {
        return (AF_INET6);
    }
    else
    {
        return (0);
    }
}

void HostUC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool HostUC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

void HostUC::toString (string &valueString) const
{
    char buffer[80] = {0};

    // Write the isUserConfigured flag and hostType
    snprintf (buffer, 80, "%u#%u#", (m_pIsUserConfigured ? 1 : 0), (UI32) (m_hostType));
    valueString = string (buffer);

    // Append the host value
    if (WAVE_DOMAINNAME == m_hostType)
    {
        valueString += m_domainName;
    }
    else if (WAVE_IPV4ADDRESS == m_hostType)
    {
        valueString += m_ipV4Address.toString ();
    }
    else if (WAVE_IPV6ADDRESS == m_hostType)
    {
        valueString += m_ipV6Address.toString ();
    }
}

void HostUC::getPlainString (string &valueString) const
{
    if ("" != m_domainName)
    {
        valueString = m_domainName;
    }
    else if ("" != m_ipV4Address.toString ())
    {
        valueString = m_ipV4Address.toString ();
    }
    else if ("" != m_ipV6Address.toString ())
    {
        valueString = m_ipV6Address.toString ();
    }
}

void HostUC::fromString (const string &hostAsString)
{
    string tempString = hostAsString;
    vector<string> token;
    UI32 numberOfToken = 0;
    UI32 tempIsUserConfigured;

    tokenize (tempString, token, '#');

    numberOfToken = token.size ();

    prismAssert (3 >= numberOfToken, __FILE__, __LINE__);

    if (numberOfToken > 0)
    {
        tempIsUserConfigured =  strtoul (token[0].c_str (), NULL, 10);
        m_pIsUserConfigured  =  tempIsUserConfigured ? true : false;
    }

    if (numberOfToken > 1)
    {
        m_hostType = HostType (strtoul (token[1].c_str (), NULL, 10));
    }

    // Clear the old data
    m_domainName = "";
    m_ipV4Address.fromString("");
    m_ipV6Address.fromString("");

    if (numberOfToken >= 3) 
    {
        switch (m_hostType)
        {
            case WAVE_DOMAINNAME:
                m_domainName = token[2];
                break;
            case WAVE_IPV4ADDRESS:
                m_ipV4Address = token[2];
                break;
            case WAVE_IPV6ADDRESS:
                m_ipV6Address = token[2];
                break;
            default:
                m_hostType = WAVE_UNKNOWN_HOSTTYPE;
                break;
        }
    }
}


ResourceId HostUC::loadFromPlainString (const string &pHostUCInPlainStringFormat)
{
    m_domainName = "";
    m_ipV4Address.fromString("");
    m_ipV6Address.fromString("");
    ResourceId status = WAVE_MESSAGE_ERROR;

    unsigned char buf[sizeof(struct in6_addr)];

    if (inet_pton (AF_INET, pHostUCInPlainStringFormat.c_str(), &buf) == 1)
    {
        m_hostType = WAVE_IPV4ADDRESS;
        status = m_ipV4Address.loadFromPlainString (pHostUCInPlainStringFormat);
    }
    else if (inet_pton (AF_INET6, pHostUCInPlainStringFormat.c_str(), &buf) == 1)
    {
        m_hostType = WAVE_IPV6ADDRESS;
        status = m_ipV6Address.loadFromPlainString (pHostUCInPlainStringFormat);
    }
    else 
    {
        m_hostType = WAVE_DOMAINNAME;
        m_domainName = pHostUCInPlainStringFormat;
        status = WAVE_MESSAGE_SUCCESS;
    }

    m_pIsUserConfigured = true;
       
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string ("HostUC::loadFromPlainString : Improper format of HostUc in string"));
    }

    return status;
}

bool HostUC::operator == (const HostUC &pHostUC) const
{
    if ((m_hostType == pHostUC.m_hostType) &&
        (m_domainName == pHostUC.m_domainName) &&
        (m_ipV4Address == pHostUC.m_ipV4Address) &&
        (m_ipV6Address == pHostUC.m_ipV6Address) &&
        (m_pIsUserConfigured == pHostUC.m_pIsUserConfigured))
    {
        return (true);
    }

    return (false);
}

bool HostUC::operator != (const HostUC &pHostUC) const
{
    return (! (operator == (pHostUC)));
}

HostUC &HostUC::operator = (const HostUC &pHostUC)
{
    m_hostType = pHostUC.m_hostType;
    m_domainName = pHostUC.m_domainName;
    m_ipV4Address = pHostUC.m_ipV4Address;
    m_ipV6Address = pHostUC.m_ipV6Address;
    m_pIsUserConfigured = pHostUC.m_pIsUserConfigured;

    return (*this);
}

}
