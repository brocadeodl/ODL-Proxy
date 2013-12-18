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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Framework/Core/VersionInformation.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

using namespace std;

namespace WaveNs
{

VersionInformation::VersionInformation ()
    : SerializableObject (),
      m_firmwareVersion (""),
      m_haVersion (0)
{
}

VersionInformation::~VersionInformation ()
{
}

void VersionInformation::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_firmwareVersion,  "firmwareVersion"));
    addSerializableAttribute (new AttributeUI32   (&m_haVersion,        "haVersion"));
}

void VersionInformation::display () const
{
    trace (TRACE_LEVEL_INFO, string ("___________________________________________________"));

    trace (TRACE_LEVEL_INFO, string ("Firmware Version   : ") + m_firmwareVersion);
    trace (TRACE_LEVEL_INFO, string ("HA Version         : ") + m_haVersion);

    trace (TRACE_LEVEL_INFO, string ("___________________________________________________"));
}

string VersionInformation::getFirmwareVersion () const
{
    return (m_firmwareVersion);
}

void VersionInformation::setFirmwareVersion (const string &firmwareVersion)
{
    m_firmwareVersion = firmwareVersion;
}

UI32 VersionInformation::getHaVersion () const
{
    return (m_haVersion);
}

void VersionInformation::setHaVersion (const UI32 &haVersion)
{
    m_haVersion = haVersion;
}

}
