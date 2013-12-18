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

#include "ManagementInterface/WaveManagementServerInformation.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

WaveManagementServerInformation::WaveManagementServerInformation ()
    : SerializableObject                            (),
      m_ipAddress                                   (""),
      m_port                                        (0),
      m_serverUniqueIdentifierFromServerPerspective (0),
      m_messageVersion                              ("")
{
    FrameworkToolKit::getServices (m_serviceNames, m_serviceIds);

    trace (TRACE_LEVEL_DEBUG, string ("WaveManagementServerInformation::WaveManagementServerInformation : Number Of Services : ") + m_serviceNames.size ());
}

WaveManagementServerInformation::WaveManagementServerInformation (const string &ipAddress, const UI32 &port)
    : SerializableObject                            (),
      m_ipAddress                                   (ipAddress),
      m_port                                        (port),
      m_serverUniqueIdentifierFromServerPerspective (0),
      m_messageVersion                              ("")   
{
    FrameworkToolKit::getServices (m_serviceNames, m_serviceIds);

    trace (TRACE_LEVEL_DEBUG, string ("WaveManagementServerInformation::WaveManagementServerInformation : Number Of Services : ") + m_serviceNames.size ());
}

WaveManagementServerInformation::~WaveManagementServerInformation ()
{
}

void WaveManagementServerInformation::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString               (&m_ipAddress,                                  "ipAddress"));
    addSerializableAttribute (new AttributeUI32                 (&m_port,                                       "port"));
    addSerializableAttribute (new AttributeStringVector         (&m_serviceNames,                               "serviceNames"));
    addSerializableAttribute (new AttributePrismServiceIdVector (&m_serviceIds,                                 "serviceIds"));
    addSerializableAttribute (new AttributeUI32                 (&m_serverUniqueIdentifierFromServerPerspective,"serverUniqueIdentifierFromServerPerspective"));
    addSerializableAttribute (new AttributeStringVector         (&m_reservedFields,                             "reservedFields"));
    addSerializableAttribute (new AttributeString               (&m_messageVersion,                             "messageVersion"));
}

string WaveManagementServerInformation::getIpAddress () const
{
    return (m_ipAddress);
}

void WaveManagementServerInformation::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

UI32 WaveManagementServerInformation::getPort () const
{
    return (m_port);
}

void WaveManagementServerInformation::setPort (const UI32 &port)
{
    m_port = port;
}

UI32 WaveManagementServerInformation::getNumberOfServices () const
{
    return (m_serviceNames.size ());
}

string WaveManagementServerInformation::getServiceNameAtIndex (const UI32 &index) const
{
    UI32 numberOfServices = getNumberOfServices ();

    prismAssert (index < numberOfServices, __FILE__, __LINE__);

    return (m_serviceNames[index]);
}

UI32 WaveManagementServerInformation::getServiceIdAtIndex (const UI32 &index) const
{
    UI32 numberOfServices = getNumberOfServices ();

    prismAssert (index < numberOfServices, __FILE__, __LINE__);

    return (m_serviceIds[index]);
}

vector<string> WaveManagementServerInformation::getServiceNames () const
{
    return (m_serviceNames);
}

vector<UI32> WaveManagementServerInformation::getServiceIds () const
{
    return (m_serviceIds);
}

UI32 WaveManagementServerInformation::getServerUniqueIdentifierFromServerPerspective ()
{
    return (m_serverUniqueIdentifierFromServerPerspective);
}


void WaveManagementServerInformation::setServerUniqueIdentifierFromServerPerspective (const UI32 &serverUniqueIdentifierFromServerPerspective)
{
    m_serverUniqueIdentifierFromServerPerspective  = serverUniqueIdentifierFromServerPerspective;
}

string WaveManagementServerInformation::getMessageVersion () const
{
    return (m_messageVersion);
}

void WaveManagementServerInformation::setMessageVersion (const string &version)
{
    m_messageVersion = version;
}

}
