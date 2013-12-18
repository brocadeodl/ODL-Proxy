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
 *   Author : nbharat                                                     *
 **************************************************************************/

#include "LicenseService/Local/LicenseServiceLocalObjectManager.h"
#include "LicenseService/Local/LicenseServiceFileAddMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServiceFileAddMessage::LicenseServiceFileAddMessage ()
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEFILEADD)
    {
    }

    LicenseServiceFileAddMessage::LicenseServiceFileAddMessage (const string &proto,const string &user,const string &host,const string &password,const string &srcFilePath,const string &msgStatus)
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEFILEADD),
        m_proto    (proto),
        m_user    (user),
        m_host    (host),
        m_password    (password),
        m_srcFilePath    (srcFilePath),
        m_msgStatus    (msgStatus)
    {
    }

    LicenseServiceFileAddMessage::~LicenseServiceFileAddMessage ()
    {
    }

    void  LicenseServiceFileAddMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_proto,"proto"));
        addSerializableAttribute (new AttributeString(&m_user,"user"));
        addSerializableAttribute (new AttributeString(&m_host,"host"));
        addSerializableAttribute (new AttributeString(&m_password,"password"));
        addSerializableAttribute (new AttributeString(&m_srcFilePath,"srcFilePath"));
        addSerializableAttribute (new AttributeString(&m_msgStatus,"msgStatus"));
    }

    void  LicenseServiceFileAddMessage::setProto(const string &proto)
    {
        m_proto  =  proto;
    }

    string  LicenseServiceFileAddMessage::getProto() const
    {
        return (m_proto);
    }

    void  LicenseServiceFileAddMessage::setUser(const string &user)
    {
        m_user  =  user;
    }

    string  LicenseServiceFileAddMessage::getUser() const
    {
        return (m_user);
    }

    void  LicenseServiceFileAddMessage::setHost(const string &host)
    {
        m_host  =  host;
    }

    string  LicenseServiceFileAddMessage::getHost() const
    {
        return (m_host);
    }

    void  LicenseServiceFileAddMessage::setPassword(const string &password)
    {
        m_password  =  password;
    }

    string  LicenseServiceFileAddMessage::getPassword() const
    {
        return (m_password);
    }

    void  LicenseServiceFileAddMessage::setSrcFilePath(const string &srcFilePath)
    {
        m_srcFilePath  =  srcFilePath;
    }

    string  LicenseServiceFileAddMessage::getSrcFilePath() const
    {
        return (m_srcFilePath);
    }

    void  LicenseServiceFileAddMessage::setMsgStatus(const string &msgStatus)
    {
        m_msgStatus  =  msgStatus;
    }

    string  LicenseServiceFileAddMessage::getMsgStatus() const
    {
        return (m_msgStatus);
    }

}
