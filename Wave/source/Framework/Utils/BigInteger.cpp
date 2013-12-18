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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/BigInteger.h"

namespace WaveNs
{

BigInteger::BigInteger (const SI64 &bigIntegerValue)
    : m_bigIntegerValue (bigIntegerValue)
{
}

BigInteger::~BigInteger ()
{
}

SI64 BigInteger::getIntegerValue () const
{
    return (m_bigIntegerValue);
}

void BigInteger::setIntegerValue (const SI64 &bigIntegerValue)
{
    m_bigIntegerValue = bigIntegerValue;
}

string BigInteger::toString () const
{
    char tempBuffer[256] = {0};

    snprintf (tempBuffer, 256, "%lld", m_bigIntegerValue);

    return (string (tempBuffer));
}

string BigInteger::toHexString () const
{
    char tempBuffer[256] = {0};

    snprintf (tempBuffer, 256, "%llX", m_bigIntegerValue);

    return (string (tempBuffer));
}

}
