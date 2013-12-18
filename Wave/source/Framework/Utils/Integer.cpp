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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/Integer.h"

namespace WaveNs
{

Integer::Integer (const SI32 &integerValue)
    : m_integerValue (integerValue)
{
}

Integer::~Integer ()
{
}

SI32 Integer::getIntegerValue () const
{
    return (m_integerValue);
}

void Integer::setIntegerValue (const SI32 &integerValue)
{
    m_integerValue = integerValue;
}

string Integer::toString () const
{
    char tempBuffer[32] = {0};

    snprintf (tempBuffer, 32, "%d", m_integerValue);

    return (string (tempBuffer));
}

string Integer::toHexString () const
{
    char tempBuffer[32] = {0};

    snprintf (tempBuffer, 32, "%X", m_integerValue);

    return (string (tempBuffer));
}

}
