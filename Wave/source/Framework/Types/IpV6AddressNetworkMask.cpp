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

#include "Framework/Types/IpV6AddressNetworkMask.h"
#include "Framework/Utils/TraceUtils.h"
#include <sys/socket.h>
#include <arpa/inet.h>

namespace WaveNs
{

IpV6AddressNetworkMask::IpV6AddressNetworkMask ()
    : IpAddressNetworkMask ("255:0:0:0:0:0:0:0/8")
{
}

IpV6AddressNetworkMask::IpV6AddressNetworkMask (const string &ipV6AddressNetworkMask)
    : IpAddressNetworkMask (ipV6AddressNetworkMask)
{
}

IpV6AddressNetworkMask::IpV6AddressNetworkMask (const IpV6AddressNetworkMask &ipV6AddressNetworkMask)
    : IpAddressNetworkMask (ipV6AddressNetworkMask)
{
}

IpV6AddressNetworkMask::~IpV6AddressNetworkMask ()
{
}

UI32 IpV6AddressNetworkMask::getAddressFamily ()
{
    return (AF_INET6);
}

ResourceId IpV6AddressNetworkMask::loadFromPlainString (const string &ipV6AddressNetworkMask)
{
    string           tempString = ipV6AddressNetworkMask;
    UI32             count      = 0;
    vector <string>  tokenNetworkMask;
    ResourceId       status     = WAVE_MESSAGE_ERROR;

    tokenize (tempString, tokenNetworkMask, '/');   

    count = tokenNetworkMask.size ();

    if (count == 1)
    {
        setLength ( (UI8) 32);  /* For Ipv6 its not clear when prifix is not present what does it means when some one starts using it 
                                   revisit of this logic is required. currentely keeping it as it is in the IpAddressNetworkMask.*/

        unsigned char buf[sizeof(struct in6_addr)];

        if (inet_pton(AF_INET6, tokenNetworkMask[0].c_str(), &buf) == 1)
        {
            setIpAddress (tokenNetworkMask[0]);
            status = WAVE_MESSAGE_SUCCESS;
        }    
    }
    else if (count == 2)
    {
        char* firstInvalidCharacterPosition = NULL;

        setLength( (UI8) strtoul (tokenNetworkMask[1].c_str (), &(firstInvalidCharacterPosition), 10) );

        if ( *firstInvalidCharacterPosition == '\0' )
        {  
            unsigned char buf[sizeof(struct in6_addr)];

            if (inet_pton(AF_INET6, tokenNetworkMask[0].c_str(), &buf) == 1)
            {
                setIpAddress (tokenNetworkMask[0]);
                status = WAVE_MESSAGE_SUCCESS;
            }    
        }
    }

    if( WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("IpV6AddressNetworkMask::loadFromPlainString : Improper format of IpV6addressNetworkMask in string"));
    }

    return status;
}
}

