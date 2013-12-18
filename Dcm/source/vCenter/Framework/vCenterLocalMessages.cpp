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
 *   Author : sharis                                                     *
 **************************************************************************/

#include "vCenter/Framework/vCenterLocalObjectManager.h"
#include "vCenter/Framework/vCenterLocalMessages.h"
#include "vCenter/Framework/vCenterLocalTypes.h"

namespace DcmNs
{

// ************* vCenterVcenterMessage ************

    vCenterVcenterMessage::vCenterVcenterMessage ()
        : PrismMessage (vCenterLocalObjectManager::getPrismServiceId (), VCENTERVCENTER)
    {
        m_id_flag = false;
        m_ipaddress_flag = false;
        m_username_flag = false;
        m_password_flag = false;
        m_activate		= false;
        m_activate_flag = false;
        m_vc_delete = false;
    }

    vCenterVcenterMessage::vCenterVcenterMessage (const string &id,
        const string &ipaddress,
        const string &username,
        const string &password,
        const bool &activate)
        : PrismMessage (vCenterLocalObjectManager::getPrismServiceId (), VCENTERVCENTER),
        m_id    (id),
        m_ipaddress    (ipaddress),
        m_username    (username),
        m_password    (password),
        m_activate    (activate)
    {
        m_id_flag = false;
        m_ipaddress_flag = false;
        m_username_flag = false;
        m_password_flag = false;
        m_activate		= false;
        m_activate_flag = false;
        m_vc_delete = false;
    }

    vCenterVcenterMessage::~vCenterVcenterMessage ()
    {
    }

    void  vCenterVcenterMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ipaddress,"ipaddress"));
        addSerializableAttribute (new AttributeString(&m_username,"username"));
        addSerializableAttribute (new AttributeString(&m_password,"password"));
        addSerializableAttribute (new AttributeBool(&m_activate,"activate"));
        
        addSerializableAttribute (new AttributeBool(&m_id_flag,"idFlag"));
        addSerializableAttribute (new AttributeBool(&m_ipaddress_flag,"ipaddressFlag"));
        addSerializableAttribute (new AttributeBool(&m_username_flag,"usernameFlag"));
        addSerializableAttribute (new AttributeBool(&m_password_flag,"passwordFlag"));
        addSerializableAttribute (new AttributeBool(&m_activate_flag,"activateFlag"));
        addSerializableAttribute (new AttributeBool(&m_vc_delete,"m_vc_delete"));
    }

    void  vCenterVcenterMessage::setId(const string &id)
    {
        m_id = id;
        m_id_flag = true;
    }

    string  vCenterVcenterMessage::getId() const
    {
        return (m_id);
    }

    bool  vCenterVcenterMessage::getIdFlag() const
    {
        return (m_id_flag);
    }

    void  vCenterVcenterMessage::setIpaddress(const string &ipaddress)
    {
        m_ipaddress = ipaddress;
        m_ipaddress_flag = true;
    }

    string  vCenterVcenterMessage::getIpaddress() const
    {
        return (m_ipaddress);
    }

    bool  vCenterVcenterMessage::getIpaddressFlag() const
    {
        return (m_ipaddress_flag);
    }

    void  vCenterVcenterMessage::setUsername(const string &username)
    {
        m_username = username;
        m_username_flag = true;
    }

    string  vCenterVcenterMessage::getUsername() const
    {
        return (m_username);
    }

    bool  vCenterVcenterMessage::getUsernameFlag() const
    {
        return (m_username_flag);
    }

    void  vCenterVcenterMessage::setPassword(const string &password)
    {
        m_password = password;
        m_password_flag = true;
    }

    string  vCenterVcenterMessage::getPassword() const
    {
        return (m_password);
    }

    bool  vCenterVcenterMessage::getPasswordFlag() const
    {
        return (m_password_flag);
    }

    void  vCenterVcenterMessage::setActivate(const bool &activate)
    {
        m_activate = activate;
        m_activate_flag = true;
    }

    bool  vCenterVcenterMessage::getActivate() const
    {
        return (m_activate);
    }

    bool  vCenterVcenterMessage::getActivateFlag() const
    {
        return (m_activate_flag);
    }
    bool  vCenterVcenterMessage::getDeleteFlag() const
    {
        return (m_vc_delete);
    }
    void  vCenterVcenterMessage::setDeleteFlag(const bool val)
    {
        m_vc_delete = val;
    }
}
