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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Types/IpV6Address.h"
#include "Framework/Utils/TraceUtils.h"
#include <sys/socket.h>

namespace WaveNs
{

IpV6Address::IpV6Address ()
    : IpAddress ("0:0:0:0:0:0:0:1")
{
}

IpV6Address::IpV6Address (const string &ipV6Address)
    : IpAddress (ipV6Address)
{
}

IpV6Address::IpV6Address (const IpV6Address &ipV6Address)
    : IpAddress (ipV6Address)
{
}

IpV6Address::~IpV6Address ()
{
}

UI32 IpV6Address::getAddressFamily ()
{
    return (AF_INET6);
}

ResourceId IpV6Address::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    unsigned char buf[sizeof(struct in6_addr)];

    if (inet_pton(AF_INET6, valueString.c_str(), &buf) == 1)
    {
        IpAddress::loadFromPlainString (valueString);
        status = WAVE_MESSAGE_SUCCESS;
    }

    if( WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("IpV6Address::loadFromPlainString : Improper format of IpV6Address in input string"));
    }

    return status;
}
}
