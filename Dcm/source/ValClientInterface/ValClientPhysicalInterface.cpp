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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ValClientInterface/ValClientPhysicalInterface.h"

namespace DcmNs
{

ValClientPhysicalInterface::ValClientPhysicalInterface ()
{
}

ValClientPhysicalInterface::~ValClientPhysicalInterface ()
{
}

string ValClientPhysicalInterface::getName () const
{
    return (m_name);
}

void ValClientPhysicalInterface::setName (const string &name)
{
    m_name = name;
}


string ValClientPhysicalInterface::getIpAddress () const
{
    return (m_ipAddress);
}

void ValClientPhysicalInterface::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

UI32 ValClientPhysicalInterface::getMtu () const
{
    return (m_mtu);
}

void ValClientPhysicalInterface::setMtu (const UI32 &mtu)
{
    m_mtu = mtu;
}

}
