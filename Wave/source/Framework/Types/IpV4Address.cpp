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

#include "Framework/Types/IpV4Address.h"
#include "Framework/Utils/TraceUtils.h"
#include <sys/socket.h>

namespace WaveNs
{

IpV4Address::IpV4Address ()
    : IpAddress ("127.0.0.1")
{
}

IpV4Address::IpV4Address (const string &ipV4Address)
    : IpAddress (ipV4Address)
{
}

IpV4Address::IpV4Address (const IpV4Address &ipV4Address)
    : IpAddress (ipV4Address)
{
}

IpV4Address::~IpV4Address ()
{
}

UI32 IpV4Address::getAddressFamily ()
{
    return (AF_INET);
}

ResourceId IpV4Address::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    unsigned char buf[sizeof(struct in6_addr)];

    if (inet_pton(AF_INET, valueString.c_str(), &buf) == 1)
    {
        IpAddress::loadFromPlainString (valueString);
        status = WAVE_MESSAGE_SUCCESS;
    }

    if( WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("IpV4Address::loadFromPlainString : Improper format of IpV4Address in input string"));
    }

    return status;
}

}
