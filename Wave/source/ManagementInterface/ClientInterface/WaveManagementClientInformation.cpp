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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/WaveManagementClientInformation.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Version/PrismVersion.h"

namespace WaveNs
{

WaveManagementClientInformation::WaveManagementClientInformation ()
    : SerializableObject       (),
      m_waveClientName         (""),
      m_ipAddress              (""),
      m_port                   (0),
      m_userInterfaceServiceId (0),
      m_serverUniqueIdentifierFromClientPerspective (0),
      m_messageVersion         ("")
{
}

WaveManagementClientInformation::WaveManagementClientInformation (const string &waveClientName, const string &waveClientNameWithoutExtension, const string &ipAddress, const UI32 &port, const PrismServiceId &userInterfaceServiceId, const string &version)
    : SerializableObject                            (),
      m_waveClientName                              (waveClientName),
      m_waveClientNameWithoutExtension              (waveClientNameWithoutExtension),
      m_ipAddress                                   (ipAddress),
      m_port                                        (port),
      m_userInterfaceServiceId                      (userInterfaceServiceId),
      m_serverUniqueIdentifierFromClientPerspective (0),
      m_messageVersion                              (version)
{
}

WaveManagementClientInformation::~WaveManagementClientInformation ()
{
}

void WaveManagementClientInformation::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString         (&m_waveClientName,                               "waveClientName"));
    addSerializableAttribute (new AttributeString         (&m_waveClientNameWithoutExtension,               "waveClientNameWithoutExtension"));
    addSerializableAttribute (new AttributeString         (&m_ipAddress,                                    "ipAddress"));
    addSerializableAttribute (new AttributeUI32           (&m_port,                                         "port"));
    addSerializableAttribute (new AttributePrismServiceId (&m_userInterfaceServiceId,                       "userInterfaceServiceId"));
    addSerializableAttribute (new AttributeString         (&m_serverIpAddressFromClientPerspective,         "serverIpAddressFromClientPerspective"));
    addSerializableAttribute (new AttributeUI32           (&m_serverUniqueIdentifierFromClientPerspective,  "serverUniqueIdentifierFromClientPerspective"));
    addSerializableAttribute (new AttributeStringVector   (&m_reservedFields,                               "reservedFields"));
    addSerializableAttribute (new AttributeString         (&m_messageVersion,                               "messageVersion"));
}

string WaveManagementClientInformation::getWaveClientName () const
{
    return (m_waveClientName);
}

void WaveManagementClientInformation::setWaveClientName (const string &waveClientName)
{
    m_waveClientName = waveClientName;
}

string WaveManagementClientInformation::getWaveClientNameWithoutExtension () const
{
    return (m_waveClientNameWithoutExtension);
}

void WaveManagementClientInformation::setWaveClientNameWithoutExtension (const string &waveClientNameWithoutExtension)
{
    m_waveClientNameWithoutExtension = waveClientNameWithoutExtension;
}

string WaveManagementClientInformation::getIpAddress () const
{
    return (m_ipAddress);
}

void WaveManagementClientInformation::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

UI32 WaveManagementClientInformation::getPort () const
{
    return (m_port);
}

void WaveManagementClientInformation::setPort (const UI32 &port)
{
    m_port = port;
}


PrismServiceId  WaveManagementClientInformation::getUserInterfaceServiceId () const
{
    return (m_userInterfaceServiceId);
}


void WaveManagementClientInformation::setUserInterfaceServiceId (const PrismServiceId &userInterfaceServiceId)
{
    m_userInterfaceServiceId = userInterfaceServiceId;
}

string WaveManagementClientInformation::getServerIpAddressFromClientPerspective () const
{
    return (m_serverIpAddressFromClientPerspective);
}

void WaveManagementClientInformation::setServerIpAddressFromClientPerspective (const string &serverIpAddressFromClientPerspective)
{
    m_serverIpAddressFromClientPerspective = serverIpAddressFromClientPerspective;
}

UI32 WaveManagementClientInformation::getServerUniqueIdentifier () const
{
    return (m_serverUniqueIdentifierFromClientPerspective);
}

void WaveManagementClientInformation::setServerUniqueIdentifier (const UI32 &uniqueIdentifier)
{
   m_serverUniqueIdentifierFromClientPerspective = uniqueIdentifier;
}

void WaveManagementClientInformation::setMessageVersion (const string &version)
{
    m_messageVersion = version;
}

string WaveManagementClientInformation::getMessageVersion () const
{
    return (m_messageVersion);
}

}
