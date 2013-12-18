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

/* **
 * UdldIntfConfigMessage.cpp
 *
 * Aug 7, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/UdldIntfConfigMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    UdldIntfConfigMessage::UdldIntfConfigMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSM_UDLD_INTF_CONFIG)
        , m_commandCode ( UDLD_UNKNOWN_CMD )
        , m_ifType ( IF_TYPE_INVALID )
        , m_ifName ( "" )
    {
    }

    UdldIntfConfigMessage::UdldIntfConfigMessage (const UI32 opCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), opCode)
        , m_commandCode ( UDLD_UNKNOWN_CMD )
        , m_ifType ( IF_TYPE_INVALID )
        , m_ifName ( "" )
    {
        switch (opCode) {
        case NSM_UDLD_INTF_CONFIG:
        case NSM_UDLD_RESET_ALL:
            break;
        default:
            prismAssert(false, __FILE__, __LINE__);
        }
    }

    UdldIntfConfigMessage::~UdldIntfConfigMessage ()
    {
    }

    void  UdldIntfConfigMessage::setupAttributesForSerialization ()
    {
        PrismMessage::setupAttributesForSerialization();

        addSerializableAttribute(new AttributeEnum((UI32*)(&m_commandCode), "commandCode"));
        addSerializableAttribute(new AttributeEnum((UI32*)(&m_ifType), "ifType"));
        addSerializableAttribute(new AttributeString(&m_ifName, "ifName"));
    }

    void  UdldIntfConfigMessage::setCommandCode (const UdldIfCommand commandCode)
    {
        m_commandCode = commandCode;
    }

    UdldIfCommand  UdldIntfConfigMessage::getCommandCode () const
    {
        return m_commandCode;
    }

    void UdldIntfConfigMessage::setIfType (const InterfaceType ifType)
    {
        m_ifType = ifType;
    }

    InterfaceType UdldIntfConfigMessage::getIfType () const
    {
        return m_ifType;
    }

    void  UdldIntfConfigMessage::setIfName (const string &ifName)
    {
        m_ifName = ifName;
    }

    string  UdldIntfConfigMessage::getIfName () const
    {
        return m_ifName;
    }

}

