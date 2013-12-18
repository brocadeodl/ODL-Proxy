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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Types/IpAddressNetworkMask.h"
#include "Framework/Utils/AssertUtils.h"
#include "string"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

void IpAddressNetworkMask::parseNetworkMask (string &ipAddressNetworkMask)
{
    UI32             count = 0;
    vector <string>  tokenNetworkMask;

    tokenize (ipAddressNetworkMask, tokenNetworkMask, '/');   

    count = tokenNetworkMask.size ();
    
    if (count == 1)
    {
        m_len = 32; //if ther is only 1 token, that means complete ipAddress is network ipaddress & ther is no subnet.
    }
    else if (count == 2)
    {
        m_len = (UI8) strtoul (tokenNetworkMask[1].c_str (), NULL, 10);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    m_ipAddress = tokenNetworkMask[0];
    
}

IpAddressNetworkMask::IpAddressNetworkMask ()
    : m_ipAddress ("255.0.0.0"),
      m_len ((UI8) 8)
{
}

IpAddressNetworkMask::IpAddressNetworkMask (const string &ipAddressNetworkMask)
{
    string tempString = ipAddressNetworkMask;

    parseNetworkMask (tempString);
}

IpAddressNetworkMask::IpAddressNetworkMask (const IpAddressNetworkMask &ipAddressNetworkMask)
{
    m_ipAddress = ipAddressNetworkMask.m_ipAddress;
    m_len       = ipAddressNetworkMask.m_len;
}

IpAddressNetworkMask::~IpAddressNetworkMask ()
{
}

string IpAddressNetworkMask::toString () const
{
    char   tempBuf[256] = {0};
    string tempString;

    snprintf(tempBuf, 256, "%s/%u", m_ipAddress.c_str(), m_len);
    tempString = tempBuf;

    return (tempString);
}

void IpAddressNetworkMask::fromString (const string &ipAddressNetworkMask)
{
    string tempString = ipAddressNetworkMask;

    parseNetworkMask (tempString);
}


void IpAddressNetworkMask::getPlainString (string &valueString) const  
{
    valueString = toString ();
}

ResourceId IpAddressNetworkMask::loadFromPlainString (const string &ipAddressNetworkMask)
{
    string           tempString = ipAddressNetworkMask;
    UI32             count      = 0;
    vector <string>  tokenNetworkMask;
    ResourceId       status     = WAVE_MESSAGE_ERROR;

    tokenize (tempString, tokenNetworkMask, '/');   

    count = tokenNetworkMask.size ();
    
    if (count == 1)
    {
        m_len = 32; //if ther is only 1 token, that means complete ipAddress is network ipaddress & ther is no subnet.
    }
    else if (count == 2)
    {
        char* firstInvalidCharacterPosition = NULL;

        m_len = (UI8) strtoul (tokenNetworkMask[1].c_str (), &(firstInvalidCharacterPosition), 10);
        
        if ( *firstInvalidCharacterPosition == '\0' )
        {   
            m_ipAddress = tokenNetworkMask[0];
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {   
            trace (TRACE_LEVEL_ERROR, string("IpAddressNetworkMask::loadFromPlainString : Improper format of IpAddressNetworkMask in string"));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("IpAddressNetworkMask::loadFromPlainString : Improper format of IpaddressNetworkMask in string"));
    }

    return status;
}

string IpAddressNetworkMask::getIpAddress () const
{
    return (m_ipAddress);
}

UI8 IpAddressNetworkMask::getLength () const
{
    return (m_len);
}

void IpAddressNetworkMask::setIpAddress (const string &ipAddress) 
{
    m_ipAddress = ipAddress;
}

void  IpAddressNetworkMask::setLength (const UI8 &length) 
{
    m_len = length;
}

bool IpAddressNetworkMask::operator == (const IpAddressNetworkMask &ipAddressNetworkMask) const
{
    if (m_ipAddress != ipAddressNetworkMask.m_ipAddress)
    {
        return (false);
    }

    if (m_len != ipAddressNetworkMask.m_len)
    {
        return (false);
    }

    return (true);
}

bool IpAddressNetworkMask::operator != (const IpAddressNetworkMask &ipAddressNetworkMask) const
{
    return (! (operator == (ipAddressNetworkMask)));
}

IpAddressNetworkMask &IpAddressNetworkMask::operator = (const IpAddressNetworkMask &ipAddressNetworkMask)
{
    m_ipAddress = ipAddressNetworkMask.m_ipAddress;
    m_len       = ipAddressNetworkMask.m_len;

    return (*this);
}

}

