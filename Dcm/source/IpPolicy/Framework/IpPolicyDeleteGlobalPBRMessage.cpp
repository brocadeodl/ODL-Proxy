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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Debojyoti                                                     *
 **************************************************************************/

#include "IpPolicy/Framework/IpPolicyDeleteGlobalPBRMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    IpPolicyDeleteGlobalPBRMessage::IpPolicyDeleteGlobalPBRMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),IPPOLICYDELETEGLOBALPBR)
    {
    }

    IpPolicyDeleteGlobalPBRMessage::IpPolicyDeleteGlobalPBRMessage (const string &ifName, UI32 ifType)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),IPPOLICYDELETEGLOBALPBR),
        m_ifName    (ifName),
        m_ifType    (ifType)
    {
    }

    IpPolicyDeleteGlobalPBRMessage::~IpPolicyDeleteGlobalPBRMessage ()
    {
    }

    void  IpPolicyDeleteGlobalPBRMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    string IpPolicyDeleteGlobalPBRMessage::getIfName ()
    {
        return (m_ifName);
    }

    UI32 IpPolicyDeleteGlobalPBRMessage::getIfType ()
    {
        return (m_ifType);
    }

}

