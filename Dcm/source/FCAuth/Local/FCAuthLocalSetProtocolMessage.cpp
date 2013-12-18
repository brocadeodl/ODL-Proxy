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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/FCAuthLocalSetProtocolMessage.h"
#include "FCAuth/Local/FCAuthTypes.h"

#include "brocade-fc-auth.h"

namespace DcmNs
{

    FCAuthLocalSetProtocolMessage::FCAuthLocalSetProtocolMessage ()
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALSETPROTOCOL)
    {
        m_AuthType = dhchap;
        m_AuthGroup = "";
        m_AuthHash = FCAUTH_PROTOCOLHASHTYPE_MAX;
    }

    FCAuthLocalSetProtocolMessage::FCAuthLocalSetProtocolMessage (const AuthProtocolType &AuthType,const AuthProtocolGroup &AuthGroup,const AuthProtocolHash &AuthHash)
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALSETPROTOCOL),
        m_AuthType    (AuthType),
        m_AuthGroup    (AuthGroup),
        m_AuthHash    (AuthHash)
    {
    }

    FCAuthLocalSetProtocolMessage::~FCAuthLocalSetProtocolMessage ()
    {
    }

    void  FCAuthLocalSetProtocolMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_AuthType,"AuthType"));
        addSerializableAttribute (new AttributeString(&m_AuthGroup,"AuthGroup"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_AuthHash,"AuthHash"));
    }

    void  FCAuthLocalSetProtocolMessage::setAuthType(const AuthProtocolType &AuthType)
    {
        m_AuthType  =  AuthType;
    }

    AuthProtocolType  FCAuthLocalSetProtocolMessage::getAuthType() const
    {
        return (m_AuthType);
    }

    void  FCAuthLocalSetProtocolMessage::setAuthGroup(const AuthProtocolGroup &AuthGroup)
    {
        m_AuthGroup  =  AuthGroup;
    }

    AuthProtocolGroup  FCAuthLocalSetProtocolMessage::getAuthGroup() const
    {
        return (m_AuthGroup);
    }

    void  FCAuthLocalSetProtocolMessage::setAuthHash(const AuthProtocolHash &AuthHash)
    {
        m_AuthHash  =  AuthHash;
    }

    AuthProtocolHash  FCAuthLocalSetProtocolMessage::getAuthHash() const
    {
        return (m_AuthHash);
    }

}
