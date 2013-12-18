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

#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/FCAuthLocalAddAuthSecretMessage.h"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    FCAuthLocalAddAuthSecretMessage::FCAuthLocalAddAuthSecretMessage ()
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALADDAUTHSECRET)
    {
    }

    FCAuthLocalAddAuthSecretMessage::FCAuthLocalAddAuthSecretMessage (const string &wwn,const string &peerSecret,const string &localSecret)
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALADDAUTHSECRET),
        m_wwn    (wwn),
        m_peerSecret    (peerSecret),
        m_localSecret    (localSecret)
    {
    }

    FCAuthLocalAddAuthSecretMessage::~FCAuthLocalAddAuthSecretMessage ()
    {
    }

    void  FCAuthLocalAddAuthSecretMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_wwn,"wwn"));
        addSerializableAttribute (new AttributeString(&m_peerSecret,"peerSecret"));
        addSerializableAttribute (new AttributeString(&m_localSecret,"localSecret"));
    }

    void  FCAuthLocalAddAuthSecretMessage::setWwn(const string &wwn)
    {
        m_wwn  =  wwn;
    }

    string  FCAuthLocalAddAuthSecretMessage::getWwn() const
    {
        return (m_wwn);
    }

    void  FCAuthLocalAddAuthSecretMessage::setPeerSecret(const string &peerSecret)
    {
        m_peerSecret  =  peerSecret;
    }

    string  FCAuthLocalAddAuthSecretMessage::getPeerSecret() const
    {
        return (m_peerSecret);
    }

    void  FCAuthLocalAddAuthSecretMessage::setLocalSecret(const string &localSecret)
    {
        m_localSecret  =  localSecret;
    }

    string  FCAuthLocalAddAuthSecretMessage::getLocalSecret() const
    {
        return (m_localSecret);
    }

}
