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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowControllerManagedObject.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"

using namespace WaveNs;

namespace OpenFlowNs
{

OpenFlowControllerManagedObject::OpenFlowControllerManagedObject (WaveObjectManager *pWaveObjectManager)
: PrismElement           (pWaveObjectManager),
  PrismPersistableObject (OpenFlowControllerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
  WaveManagedObject      (pWaveObjectManager),
  DcmManagedObject       (pWaveObjectManager),
  m_port                 (0),
  m_localPort            (0),
  m_protocol             (OPEN_FLOW_CHANNEL_CONNECTION_PROTOCOL_TCP)
{
}

OpenFlowControllerManagedObject::~OpenFlowControllerManagedObject ()
{
}

string OpenFlowControllerManagedObject::getClassName ()
{
    return ("OpenFlowControllerManagedObject");
}

void OpenFlowControllerManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString                 (         &m_id,             "id"));
    addPersistableAttribute (new AttributeResourceId             ((UI32 *) &m_role,           "role"));
    addPersistableAttribute (new AttributeIpV4AddressNetworkMask (         &m_ipPrefix,       "ipPrefix"));
    addPersistableAttribute (new AttributeUI32                   (         &m_port,           "port"));
    addPersistableAttribute (new AttributeIpV4Address            (         &m_localIpAddress, "localIpAddress"));
    addPersistableAttribute (new AttributeUI32                   (         &m_localPort,      "localPort"));
    addPersistableAttribute (new AttributeResourceId             ((UI32 *) &m_protocol,       "protocol"));
}

void OpenFlowControllerManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString                 (         &m_id,             "id"));
    addPersistableAttributeForCreate (new AttributeResourceId             ((UI32 *) &m_role,           "role"));
    addPersistableAttributeForCreate (new AttributeIpV4AddressNetworkMask (         &m_ipPrefix,       "ipPrefix"));
    addPersistableAttributeForCreate (new AttributeUI32                   (         &m_port,           "port"));
    addPersistableAttributeForCreate (new AttributeIpV4Address            (         &m_localIpAddress, "localIpAddress"));
    addPersistableAttributeForCreate (new AttributeUI32                   (         &m_localPort,      "localPort"));
    addPersistableAttributeForCreate (new AttributeResourceId             ((UI32 *) &m_protocol,       "protocol"));
}

string OpenFlowControllerManagedObject::getId () const
{
    return (m_id);
}

void OpenFlowControllerManagedObject::setId (const string &id)
{
    m_id = id;
}

OpenFlowControllerRole OpenFlowControllerManagedObject::getRole () const
{
    return (m_role);
}

IpV4AddressNetworkMask OpenFlowControllerManagedObject::getIpPrefix () const
{
    return (m_ipPrefix);
}

void OpenFlowControllerManagedObject::setIpPrefix (const IpV4AddressNetworkMask &ipPrefix)
{
    m_ipPrefix = ipPrefix;
}

UI32 OpenFlowControllerManagedObject::getPort () const
{
    return (m_port);
}

void OpenFlowControllerManagedObject::setPort (const UI32 &port)
{
    m_port = port;
}

IpV4Address OpenFlowControllerManagedObject::getLocalIpAddress () const
{
    return (m_localIpAddress);
}

void OpenFlowControllerManagedObject::setLocalIpAddress (const IpV4Address &localIpAddress)
{
    m_localIpAddress = localIpAddress;
}

UI32 OpenFlowControllerManagedObject::getLocalPort () const
{
    return (m_localPort);
}

void OpenFlowControllerManagedObject::setLocalPort (const UI32 &localPort)
{
    m_localPort = localPort;
}

OpenFlowChannelConnectionProtocol OpenFlowControllerManagedObject::getProtocol () const
{
    return (m_protocol);
}

void OpenFlowControllerManagedObject::setProtocol (const OpenFlowChannelConnectionProtocol &protocol)
{
    m_protocol = protocol;
}

}
