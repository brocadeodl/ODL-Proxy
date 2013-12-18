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
#include "FCAuth/Local/FCAuthLocalShowAuthSecretMessage.h"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    FCAuthLocalShowAuthSecretMessage::FCAuthLocalShowAuthSecretMessage ()
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALSHOWAUTHSECRET)
    {
        m_vWwn.clear();
    }

    FCAuthLocalShowAuthSecretMessage::FCAuthLocalShowAuthSecretMessage (const vector<string> &wwn)
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALSHOWAUTHSECRET),
        m_vWwn    (wwn)
    {
    }

    FCAuthLocalShowAuthSecretMessage::~FCAuthLocalShowAuthSecretMessage ()
    {
    }

    void  FCAuthLocalShowAuthSecretMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeStringVector(&m_vWwn,"vWwn"));
    }

    UI32  FCAuthLocalShowAuthSecretMessage::getNumVectors() const
    {
        return (m_vWwn.size());
    }

    void  FCAuthLocalShowAuthSecretMessage::clearNumVectors()
    {
        m_vWwn.clear();
    }

    void  FCAuthLocalShowAuthSecretMessage::setWwn(const string &wwn)
    {
        m_vWwn.push_back(wwn);
    }

    bool  FCAuthLocalShowAuthSecretMessage::getWwn(const UI32 &index,
        string &wwn)
    {
        if (index >= m_vWwn.size())
        {
            return false;
        }

        wwn = m_vWwn[index];

        return true;
    }

}
