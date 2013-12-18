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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/ValInterfaceLocalObjectManagerGetInterfaceMessage.h"
#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/ValInterfaceTypes.h"

namespace DcmNs
{

ValInterfaceLocalObjectManagerGetInterfaceMessage:: ValInterfaceLocalObjectManagerGetInterfaceMessage ()
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_GET_INTERFACE),
      m_name                     (""),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (0),
      m_mtu32                    (0),
      m_mtu32Def                 (0),
      m_mtu32Vector              (0),
      m_mtu8u                    (0),
      m_mtu16u                   (0),
      m_mtu32u                   (0),
      m_mtu32udef                (0),
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress               ("00:11:BB:22:CC:33"),
      m_macAddress2              ("001E.0BBF.47D4"),
      m_wwn                      (""),
      m_shutdown                 (true),
      m_protocol                 (Protocol (0)),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0)
{
}

ValInterfaceLocalObjectManagerGetInterfaceMessage:: ValInterfaceLocalObjectManagerGetInterfaceMessage (const string &name)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_GET_INTERFACE),
      m_name                     (name),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (0),
      m_mtu32                    (0),
      m_mtu32Def                 (0),
      m_mtu32Vector              (0),
      m_mtu8u                    (0),
      m_mtu16u                   (0),
      m_mtu32u                   (0),
      m_mtu32udef                (0),
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress               (""),
      m_macAddress2              (""),
      m_wwn                      (""),
      m_shutdown                 (true),
      m_protocol                 (Protocol (0)),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0)
{
}

ValInterfaceLocalObjectManagerGetInterfaceMessage::~ValInterfaceLocalObjectManagerGetInterfaceMessage ()
{
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString              (&m_name,           "name"));
     addSerializableAttribute (new AttributeString              (&m_string,         "string"));
     addSerializableAttribute (new AttributeIpV4Address         (&m_ipAddress,      "ipAddress"));
     addSerializableAttribute (new AttributeIpV4Address         (&m_ipAddressDef,   "ipAddressDef"));
     addSerializableAttribute (new AttributeSI8                 (&m_mtu8,           "mtu8"));
     addSerializableAttribute (new AttributeSI16                (&m_mtu16,          "mtu16"));
     addSerializableAttribute (new AttributeSI32                (&m_mtu32,          "mtu32"));
     addSerializableAttribute (new AttributeSI32                (&m_mtu32Def,       "mtu32Def"));
     addSerializableAttribute (new AttributeSI32Vector          (&m_mtu32Vector,    "mtu32Vector"));
     addSerializableAttribute (new AttributeUI8                 (&m_mtu8u,          "mtu8u"));
     addSerializableAttribute (new AttributeUI16                (&m_mtu16u,         "mtu16u"));
     addSerializableAttribute (new AttributeUI32                (&m_mtu32u,         "mtu32u"));
     addSerializableAttribute (new AttributeUI32                (&m_mtu32udef,      "mtu32udef"));
     addSerializableAttribute (new AttributeUI64                (&m_mtu64u,         "mtu64u"));
     addSerializableAttribute (new AttributeBool                (&m_isEthernet,     "isEthernet"));
     addSerializableAttribute (new AttributeIpV6Address         (&m_ip6Address,     "ip6Address"));
     addSerializableAttribute (new AttributeMacAddress          (&m_macAddress,     "macAddress"));
     addSerializableAttribute (new AttributeMacAddress2         (&m_macAddress2,    "macAddress2"));
     addSerializableAttribute (new AttributeWorldWideName       (&m_wwn,            "wwn"));
     addSerializableAttribute (new AttributeBool                (&m_shutdown,       "shutdown"));
    addSerializableAttribute  (new AttributeEnum                ((UI32 *) (&m_protocol),  "protocol"));
    addSerializableAttribute  (new AttributeEnum                ((UI32 *) (&m_errorLevel),"errorLevel"));
     addSerializableAttribute (new AttributeIpV4AddressVector   (&m_ipV4Vector,      "ipV4Vector"));
     addSerializableAttribute (new AttributeIpV6AddressVector   (&m_ipV6Vector,      "ipV6Vector"));
     addSerializableAttribute (new AttributeIpVxAddress         (&m_ipVxAddress,     "ipVxAddress"));
     addSerializableAttribute (new AttributeSI32                (&m_interfaceTuple,  "interfaceTuple"));
}

string ValInterfaceLocalObjectManagerGetInterfaceMessage::getName () const
{
    return (m_name);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setName (const string &name)
{
    m_name = name;
}

string ValInterfaceLocalObjectManagerGetInterfaceMessage::getString () const
{
    return (m_string);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setString (const string &demoString)
{
    m_string = demoString;
}

IpV4Address ValInterfaceLocalObjectManagerGetInterfaceMessage::getIpAddress () const
{
    return (m_ipAddress);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setIpAddress (const IpV4Address &ipAddress)
{
        m_ipAddress = ipAddress;
}

IpV4Address ValInterfaceLocalObjectManagerGetInterfaceMessage::getIpAddressDef () const
{
    return (m_ipAddressDef);
}   

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setIpAddressDef (const IpV4Address &ipAddress) 
{
        m_ipAddressDef = ipAddress;
} 

SI8 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu8 () const
{
    return (m_mtu8);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu8 (const SI8 &mtu8)
{
    m_mtu8 = mtu8;
}

SI16 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu16 () const
{
    return (m_mtu16);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu16 (const SI16 &mtu16)
{
    m_mtu16 = mtu16;
}

SI32 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu32 () const
{
    return (m_mtu32);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu32 (const SI32 &mtu32)
{
        m_mtu32 = mtu32;
}

SI32 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu32Def () const
{
    return (m_mtu32Def);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu32Def (const SI32 &mtu32) 
{
        m_mtu32Def = mtu32;
}

vector<SI32> ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu32Vector () const
{
    return (m_mtu32Vector);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu32Vector (const vector<SI32> &mtu32Vector)
{
    m_mtu32Vector = mtu32Vector;
}

UI8 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu8u () const
{
    return (m_mtu8u);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu8u (const UI8 &mtu8u)
{
    m_mtu8u = mtu8u;
}

UI16 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu16u () const
{
    return (m_mtu16u);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu16u (const UI16 &mtu16u)
{
    m_mtu16u = mtu16u;
}

UI32 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu32u () const
{
    return (m_mtu32u);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu32u (const UI32 &mtu32u)
{
    m_mtu32u = mtu32u;
}

UI64 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu64u () const
{
    return (m_mtu64u);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu64u (const UI64 &mtu64u)
{
    m_mtu64u = mtu64u;
}


bool ValInterfaceLocalObjectManagerGetInterfaceMessage::getIsEthernet () const
{
    return (m_isEthernet);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setIsEthernet (const bool &isEthernet)
{
    m_isEthernet = isEthernet;
}

IpV6Address ValInterfaceLocalObjectManagerGetInterfaceMessage::getIp6Address () const
{
    return (m_ip6Address);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setIp6Address (const IpV6Address &ip6Address)
{
    m_ip6Address = ip6Address;
}

MacAddress ValInterfaceLocalObjectManagerGetInterfaceMessage::getMacAddress () const
{
    return (m_macAddress);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMacAddress (const MacAddress &macAddress)
{
    m_macAddress = macAddress;
}

MacAddress2 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMacAddress2 () const
{
    return (m_macAddress2);
}   

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMacAddress2 (const MacAddress2 &macAddress)
{
    m_macAddress2 = macAddress;
}

WorldWideName ValInterfaceLocalObjectManagerGetInterfaceMessage::getWorldWideName () const
{
    return (m_wwn);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setWorldWideName (const WorldWideName &worldWideName)
{
    m_wwn = worldWideName;
}

bool ValInterfaceLocalObjectManagerGetInterfaceMessage::getShutdown () const
{
    return (m_shutdown);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setShutdown (const bool &shutdown)
{
    m_shutdown = shutdown;
}

Protocol ValInterfaceLocalObjectManagerGetInterfaceMessage::getEnum () const
{
    return (m_protocol);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setEnum (const Protocol &protocol)
{
    m_protocol = protocol;
}

ErrorLevel ValInterfaceLocalObjectManagerGetInterfaceMessage::getErrorLevelEnum () const
{
    return (m_errorLevel);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setErrorLevelEnum (const ErrorLevel &errorLevel)
{
    m_errorLevel = errorLevel;
}

vector<IpV4Address> ValInterfaceLocalObjectManagerGetInterfaceMessage::getIpV4Vector () const
{
    return (m_ipV4Vector);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setIpV4Vector (const vector<IpV4Address> &ipV4Vector)
{
    m_ipV4Vector = ipV4Vector;
}

vector<IpV6Address> ValInterfaceLocalObjectManagerGetInterfaceMessage::getIpV6Vector () const
{
    return (m_ipV6Vector);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setIpV6Vector (const vector<IpV6Address> &ipV6Vector)
{
    m_ipV6Vector = ipV6Vector;
}

IpVxAddress ValInterfaceLocalObjectManagerGetInterfaceMessage::getIpVxAddress () const
{
    return (m_ipVxAddress);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setIpVxAddress (const IpVxAddress &ipVxAddress)
{
    m_ipVxAddress = ipVxAddress;
}

UI32 ValInterfaceLocalObjectManagerGetInterfaceMessage::getMtu32udef () const
{
    return (m_mtu32udef);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setMtu32udef (const UI32 &mtu32udef)
{
    m_mtu32udef = mtu32udef;
}

SI32 ValInterfaceLocalObjectManagerGetInterfaceMessage::getInterfaceTuple () const
{
    return (m_interfaceTuple);
}

void ValInterfaceLocalObjectManagerGetInterfaceMessage::setInterfaceTuple (const SI32 &interfaceTuple)
{
    m_interfaceTuple = interfaceTuple;
}

}

