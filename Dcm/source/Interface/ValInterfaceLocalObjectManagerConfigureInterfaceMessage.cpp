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

#include "Interface/ValInterfaceLocalObjectManagerConfigureInterfaceMessage.h"
#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/ValInterfaceTypes.h"

namespace DcmNs
{

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage ()
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
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
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol) PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2              (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const string &demoString, const IpV4Address &ipAddress, const IpV4Address &ipAddressdef, const SI8 &mtu8, const SI16  &mtu16, const SI32  &mtu32, const SI32  &mtu32def, const vector<SI32> &mtu32Vector, const UI8  &mtu8u, const UI16  &mtu16u, const UI32  &mtu32u, const UI32  &mtu32udef, const UI64  &mtu64u, const bool &isEthernet, const IpV6Address &ip6Address, const MacAddress &macAddress, const WorldWideName &worldWideName, const bool &shutdown, const Protocol &protocol, const ErrorLevel &errorLevel, const vector<IpV4Address> &ipV4Vector, const vector<IpV6Address> &ipV6Vector, const IpVxAddress &ipVxAddress, const SI32  &interfaceTuple, const MacAddress2 &mac2)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (demoString),
      m_ipAddress                (ipAddress),
      m_ipAddressDef             (ipAddressdef),
      m_mtu8                     (mtu8),
      m_mtu16                    (mtu16),
      m_mtu32                    (mtu32),
      m_mtu32Def                 (mtu32def),
      m_mtu32Vector              (mtu32Vector),
      m_mtu8u                    (mtu8u),
      m_mtu16u                   (mtu16u),
      m_mtu32u                   (mtu32u),
      m_mtu32udef                (mtu32udef),
      m_mtu64u                   (mtu64u),
      m_isEthernet               (isEthernet),
      m_ip6Address               (ip6Address),
      m_macAddress             	 (macAddress),
      m_wwn                      (worldWideName),
      m_shutdown                 (shutdown),
      m_protocol                 (protocol),
      m_errorLevel               (errorLevel),
      m_ipV4Vector               (ipV4Vector),
      m_ipV6Vector               (ipV6Vector),
      m_ipVxAddress              (ipVxAddress),
      m_interfaceTuple           (interfaceTuple),
      m_macAddress2            	 (mac2)
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const string &demoString)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (demoString),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (0),
      m_mtu32Def                 (0),
      m_mtu32Vector              (0),
      m_mtu8u                    (0),
      m_mtu16u                   (0),
      m_mtu32u                   (0),
      m_mtu32udef                (0),
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const IpV4Address &ipAddress, const bool isDefault)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (""),
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
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
    if (true == isDefault)
    {
        m_ipAddress = IpV4Address("");
        m_ipAddressDef = ipAddress;
    }
    else
    {
        m_ipAddress = ipAddress;
        m_ipAddressDef = IpV4Address("");
    }
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const SI8 &mtu8)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (mtu8),
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
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const SI16 &mtu16)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (mtu16),
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
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const SI32 &mtu32, const UI16 flag, const bool isDefault)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (0),
      m_mtu32Vector              (0),
      m_mtu8u                    (0),
      m_mtu16u                   (0),
      m_mtu32u                   (0),
      m_mtu32udef                (0),
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
    if ( 1 == flag )
    {
        if (0 == isDefault)
        {
            m_mtu32Def       = 0;
            m_mtu32          = mtu32;
        }
        else
        {
            m_mtu32Def       = mtu32;
            m_mtu32          = 0;
        }   
        m_interfaceTuple = 0;
    }
    else
    {
        // If flag is not having default value of 1, it means that,
        // this function call is not for setting m_mtu32,
        // but it is for setting m_interfaceTuple.
        m_mtu32          = 0;
        m_mtu32Def       = 0;
        m_interfaceTuple = mtu32;
    }
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const vector<SI32> &mtu32Vector)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (0),
      m_mtu32                    (0),
      m_mtu32Def                 (0),
      m_mtu32Vector              (mtu32Vector),
      m_mtu8u                    (0),
      m_mtu16u                   (0),
      m_mtu32u                   (0),
      m_mtu32udef                (0),
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI8 &mtu8u)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (0),
      m_mtu32                    (0),
      m_mtu32Def                 (0),
      m_mtu32Vector              (0),
      m_mtu8u                    (mtu8u),
      m_mtu16u                   (0),
      m_mtu32u                   (0),
      m_mtu32udef                (0),
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}
ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI16 &mtu16u)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
      m_string                   (""),
      m_ipAddress                (""),
      m_ipAddressDef             (""),
      m_mtu8                     (0),
      m_mtu16                    (0),
      m_mtu32                    (0),
      m_mtu32Def                 (0),
      m_mtu32Vector              (0),
      m_mtu8u                    (0),
      m_mtu16u                   (mtu16u),
      m_mtu32u                   (0),
      m_mtu32udef                (0),
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}
ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI32 &mtu32u, const bool &isDefault)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_mtu64u                   (0),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
      if (true == isDefault)
      {
        m_mtu32udef = mtu32u;
        m_mtu32u = 0;
      }
      else 
	  {
        m_mtu32u = mtu32u;
        m_mtu32udef = 0;
      }	
}
ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI64 &mtu64u)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_mtu64u                   (mtu64u),
      m_isEthernet               (true),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const bool &isEthernet)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_isEthernet               (isEthernet),
      m_ip6Address               (""),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const IpV6Address &ip6Address)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_ip6Address               (ip6Address),
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}
ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const MacAddress &macAddress)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress               (macAddress),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}
ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const WorldWideName &worldWideName)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (worldWideName),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const Protocol &protocol)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 (protocol),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const ErrorLevel &errorLevel)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress             	 (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               (errorLevel),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const vector<IpV4Address> &ipV4Vector)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress               (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (ipV4Vector),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const vector<IpV6Address> &ipV6Vector)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress               (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (ipV6Vector),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const IpVxAddress &ipVxAddress)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress               (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (ipVxAddress),
      m_interfaceTuple           (0),
      m_macAddress2            	 (MacAddress2("0000.0000.0000"))
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage:: ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const MacAddress2 &macAddress2)
    : ManagementInterfaceMessage (ValInterfaceLocalObjectManager::getClassName (), VAL_INTERFACE_CONFIGURE_INTERFACE),
      m_interfaceObjectId        (interfaceObjectId),
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
      m_macAddress               (MacAddress("00:00:00:00:00:00")),
      m_wwn                      (WorldWideName("00:00:00:00:00:00:00:00")),
      m_shutdown                 (true),
      m_protocol                 ((Protocol)PROTO_MAX),
      m_errorLevel               ((ErrorLevel) DCM_ERRORLEVEL_MAX),
      m_ipV4Vector               (0),
      m_ipV6Vector               (0),
      m_ipVxAddress              (""),
      m_interfaceTuple           (0),
      m_macAddress2              (macAddress2)
{
}

ValInterfaceLocalObjectManagerConfigureInterfaceMessage::~ValInterfaceLocalObjectManagerConfigureInterfaceMessage ()
{
}

void ValInterfaceLocalObjectManagerConfigureInterfaceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeObjectId        (&m_interfaceObjectId,      "interfaceObjectId"));
     addSerializableAttribute (new AttributeString          (&m_string,                 "string"));
     addSerializableAttribute (new AttributeIpV4Address     (&m_ipAddress,              "ipAddress"));
     addSerializableAttribute (new AttributeIpV4Address     (&m_ipAddressDef,           "ipAddressDef"));
     addSerializableAttribute (new AttributeSI8             (&m_mtu8,                   "mtu8"));
     addSerializableAttribute (new AttributeSI16            (&m_mtu16,                  "mtu16"));
     addSerializableAttribute (new AttributeSI32            (&m_mtu32,                  "mtu32"));
     addSerializableAttribute (new AttributeSI32            (&m_mtu32Def,               "mtu32Def"));
     addSerializableAttribute (new AttributeSI32Vector      (&m_mtu32Vector,            "mtu32Vector"));
     addSerializableAttribute (new AttributeUI8             (&m_mtu8u,                  "mtu8u"));
     addSerializableAttribute (new AttributeUI16            (&m_mtu16u,                 "mtu16u"));
     addSerializableAttribute (new AttributeUI32            (&m_mtu32u,                 "mtu32u"));
     addSerializableAttribute (new AttributeUI32            (&m_mtu32udef,              "mtu32udef"));
     addSerializableAttribute (new AttributeUI64            (&m_mtu64u,                 "mtu64u"));
     addSerializableAttribute (new AttributeBool            (&m_isEthernet,             "isEthernet"));
     addSerializableAttribute (new AttributeIpV6Address     (&m_ip6Address,             "ip6Address"));
     addSerializableAttribute (new AttributeMacAddress      (&m_macAddress,             "macAddress"));
     addSerializableAttribute (new AttributeWorldWideName   (&m_wwn,                    "wwn"));
     addSerializableAttribute (new AttributeBool            (&m_shutdown,               "shutdown"));
    addSerializableAttribute  (new AttributeEnum            ((UI32 *) (&m_protocol),    "protocol"));
    addSerializableAttribute  (new AttributeEnum            ((UI32 *) (&m_errorLevel),  "errorLevel"));
     addSerializableAttribute (new AttributeIpV4AddressVector(&m_ipV4Vector,            "ipV4Vector"));               
     addSerializableAttribute (new AttributeIpV6AddressVector(&m_ipV6Vector,            "ipV6Vector"));
     addSerializableAttribute (new AttributeIpVxAddress     (&m_ipVxAddress,            "ipVxAddress"));
     addSerializableAttribute (new AttributeSI32            (&m_interfaceTuple,         "interfaceTuple"));
     addSerializableAttribute (new AttributeMacAddress2     (&m_macAddress2,            "macAddress2"));
}

ObjectId ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getInterfaceObjectId () const
{
    return (m_interfaceObjectId);
}

string ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getString () const
{
    return (m_string);
}

IpV4Address ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getIpAddress () const
{
    return (m_ipAddress);
}

IpV4Address ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getIpAddressDef () const
{     
    return (m_ipAddressDef);
}

SI8 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu8 () const
{
    return (m_mtu8);
}

SI16 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu16 () const
{
    return (m_mtu16);
}

SI32 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu32 () const
{
    return (m_mtu32);
}

SI32 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu32Def () const
{
    return (m_mtu32Def);
}

vector<SI32> ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu32Vector () const
{
    return (m_mtu32Vector);
}

UI8 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu8u () const
{
    return (m_mtu8u);
}
UI16 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu16u () const
{
    return (m_mtu16u);
}
UI32 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu32u () const
{
    return (m_mtu32u);
}
UI64 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu64u () const
{
    return (m_mtu64u);
}

bool ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getIsEthernet () const
{
    return (m_isEthernet);
}

IpV6Address ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getIp6Address () const
{
    return (m_ip6Address);
}

MacAddress ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMacAddress () const
{
    return (m_macAddress);
}

WorldWideName ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getWorldWideName () const
{
    return (m_wwn);
}

bool ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getShutdown () const
{
    return (m_shutdown);
}

Protocol ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getEnum () const
{
    return (m_protocol);
}

ErrorLevel ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getErrorLevelEnum () const
{
    return (m_errorLevel);
}

vector<IpV4Address> ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getIpV4Vector () const
{
    return (m_ipV4Vector);
}

vector<IpV6Address> ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getIpV6Vector () const
{
    return (m_ipV6Vector);
}

IpVxAddress ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getIpVxAddress () const
{
    return (m_ipVxAddress);
}

UI32 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMtu32udef () const
{
    return (m_mtu32udef);
}

SI32 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getInterfaceTuple () const
{
    return (m_interfaceTuple);
}

MacAddress2 ValInterfaceLocalObjectManagerConfigureInterfaceMessage::getMacAddress2 () const
{
    return (m_macAddress2);
}
}
