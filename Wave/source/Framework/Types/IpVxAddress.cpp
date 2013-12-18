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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                            *
 ***************************************************************************/

#include "Framework/Types/IpVxAddress.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include <sys/socket.h>

namespace WaveNs
{

IpVxAddress::IpVxAddress ()
    : IpAddress ()
{
    //Use the default type as IPV4, as the base class will set ip as "127.0.0.1"
    setIpType (WAVE_IPV4);
}

IpVxAddress::IpVxAddress (const string &ipVxAddress)
    : IpAddress (ipVxAddress),
      m_addressType (WAVE_INVALID)
{
    if ("" != ipVxAddress)
    {
        setIpType (determineIpType (ipVxAddress));
    }
}

IpVxAddress::IpVxAddress (const IpVxAddress &ipVxAddress)
    : IpAddress (),
      m_addressType (WAVE_INVALID)
{
    IpAddress::operator = (ipVxAddress);
    string ipString = ipVxAddress.toString ();

    if ("" != ipString)
    {
        setIpType (determineIpType (ipString));
    }
}

IpVxAddress::~IpVxAddress ()
{
}

UI32 IpVxAddress::getAddressFamily ()
{
    if (WAVE_IPV4 == m_addressType)
    {
        return (AF_INET);
    }
    else
    {
        return (AF_INET6);
    }
}

bool IpVxAddress::operator == (const IpVxAddress &ipVxAddress) const
{
    return ((m_addressType == ipVxAddress.m_addressType) && (this->toString () == ipVxAddress.toString ()));
}

bool IpVxAddress::operator != (const IpVxAddress &ipVxAddress) const
{
    return (!(operator == (ipVxAddress)));
}

IpVxAddress &IpVxAddress::operator = (const IpVxAddress &ipVxAddress)
{
    IpAddress::operator = (ipVxAddress);
    string ipString = ipVxAddress.toString ();

    if ("" != ipString)
    {
        setIpType (determineIpType (ipString));
    }

    return (*this);
}

bool IpVxAddress::isValidIpV4Address (const string &ipVxAddress)
{
    UI32 i1, i2, i3, i4;

    if (sscanf (ipVxAddress.c_str (), "%u.%u.%u.%u", &i1, &i2, &i3, &i4) != 4) 
    {
        return (false);
    }

    if ((i1 > 255) || (i2 > 255) || (i3 > 255) || (i4 > 255)) 
    {
        return (false);
    }

    return (true);
}

bool IpVxAddress::isValidIpV6Address (const string &ipVxAddress)
{
    UI32    colonCount = 0;
    size_t  position = 0;

    position = ipVxAddress.find (":");
    
    while (position != string::npos)
    {
        colonCount++;
        position = ipVxAddress.find (":", position + 1);
    }

    if(colonCount < 2)
    {
        return (false);
    }

    return (true);
}

void IpVxAddress::setIpType (const IpType ipType)
{
    if (WAVE_INVALID == ipType)
    {
        trace (TRACE_LEVEL_DEBUG, string ("IpVxAddress::setIpType Invalid IpType, If IpAddress string is empty this is possible"));
//        prismAssert (false, __FILE__, __LINE__);
    } 

    m_addressType = ipType;

}

IpType IpVxAddress::getIpType () const
{
    return (m_addressType);
}

IpType IpVxAddress::determineIpType (const string &ipVxAddress)
{
    if ("" != ipVxAddress)
    {
        if (isValidIpV4Address (ipVxAddress))
        {
            return (WAVE_IPV4);
        }
        else if (isValidIpV6Address (ipVxAddress))
        {
            return (WAVE_IPV6);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("IpVxAddress::IpVxAddress: Invalid IpVxAddress : ") + ipVxAddress);
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    return (WAVE_INVALID);
    
}

void IpVxAddress::fromString (const string &ipVxAddress)
{
    IpAddress :: fromString (ipVxAddress);
    setIpType  (determineIpType (ipVxAddress));
}

ResourceId IpVxAddress::loadFromPlainString (const string &ipVxAddress)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    status = IpAddress::loadFromPlainString (ipVxAddress);

    if ( WAVE_MESSAGE_SUCCESS == status )
    {   
        if (isValidIpV4Address (ipVxAddress))
        {   
            setIpType (WAVE_IPV4);
        }
        else if (isValidIpV6Address (ipVxAddress))
        {   
            setIpType (WAVE_IPV6);
        }
    }
    else
    {   
        trace (TRACE_LEVEL_ERROR, string("IpVxAddress::loadFromPlainString : Improper format of IpVxAddress in input string"));
        setIpType (WAVE_INVALID);
    }

    return status;
}

}

