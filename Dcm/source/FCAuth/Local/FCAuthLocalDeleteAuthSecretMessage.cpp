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
#include "FCAuth/Local/FCAuthLocalDeleteAuthSecretMessage.h"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    FCAuthLocalDeleteAuthSecretMessage::FCAuthLocalDeleteAuthSecretMessage ()
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALDELETEAUTHSECRET)
    {
    }

    FCAuthLocalDeleteAuthSecretMessage::FCAuthLocalDeleteAuthSecretMessage (const string &wwn)
        : PrismMessage (FCAuthLocalObjectManager::getPrismServiceId (), FCAUTHLOCALDELETEAUTHSECRET),
        m_wwn    (wwn)
    {
    }

    FCAuthLocalDeleteAuthSecretMessage::~FCAuthLocalDeleteAuthSecretMessage ()
    {
    }

    void  FCAuthLocalDeleteAuthSecretMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_wwn,"wwn"));
    }

    void  FCAuthLocalDeleteAuthSecretMessage::setWwn(const string &wwn)
    {
        m_wwn  =  wwn;
    }

    string  FCAuthLocalDeleteAuthSecretMessage::getWwn() const
    {
        return (m_wwn);
    }

}
