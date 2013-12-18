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

#include "vCenter/Framework/vCenterMessages.h"
#include "vCenter/Framework/vCenterObjectManager.h"
#include "vCenter/Framework/vCenterTypes.h"

namespace DcmNs
{

// ************* vCenterCreateVcenterMessage ************

    vCenterCreateVcenterMessage::vCenterCreateVcenterMessage ()
        : PrismMessage (vCenterObjectManager::getPrismServiceId (), VCENTERCREATEVCENTER)
    {
        m_id_flag = false;
        m_ipaddress_flag = false;
        m_username_flag = false;
        m_password_flag = false;
        m_activate		= false;
        m_activate_flag = false;
    }

    vCenterCreateVcenterMessage::vCenterCreateVcenterMessage (const string &id,
        const string &ipaddress,
        const string &username,
        const string &password,
        const bool &activate)
        : PrismMessage (vCenterObjectManager::getPrismServiceId (), VCENTERCREATEVCENTER),
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
    }

    vCenterCreateVcenterMessage::~vCenterCreateVcenterMessage ()
    {
    }

    void  vCenterCreateVcenterMessage::setupAttributesForSerialization()
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
    }

    void  vCenterCreateVcenterMessage::setId(const string &id)
    {
        m_id = id;
        m_id_flag = true;
    }

    string  vCenterCreateVcenterMessage::getId() const
    {
        return (m_id);
    }

    bool  vCenterCreateVcenterMessage::getIdFlag() const
    {
        return (m_id_flag);
    }

    void  vCenterCreateVcenterMessage::setIpaddress(const string &ipaddress)
    {
        m_ipaddress = ipaddress;
        m_ipaddress_flag = true;
    }

    string  vCenterCreateVcenterMessage::getIpaddress() const
    {
        return (m_ipaddress);
    }

    bool  vCenterCreateVcenterMessage::getIpaddressFlag() const
    {
        return (m_ipaddress_flag);
    }

    void  vCenterCreateVcenterMessage::setUsername(const string &username)
    {
        m_username = username;
        m_username_flag = true;
    }

    string  vCenterCreateVcenterMessage::getUsername() const
    {
        return (m_username);
    }

    bool  vCenterCreateVcenterMessage::getUsernameFlag() const
    {
        return (m_username_flag);
    }

    void  vCenterCreateVcenterMessage::setPassword(const string &password)
    {
        m_password = password;
        m_password_flag = true;
    }

    string  vCenterCreateVcenterMessage::getPassword() const
    {
        return (m_password);
    }

    bool  vCenterCreateVcenterMessage::getPasswordFlag() const
    {
        return (m_password_flag);
    }

    void  vCenterCreateVcenterMessage::setActivate(const bool &activate)
    {
        m_activate = activate;
        m_activate_flag = true;
    }

    bool  vCenterCreateVcenterMessage::getActivate() const
    {
        return (m_activate);
    }

    bool  vCenterCreateVcenterMessage::getActivateFlag() const
    {
        return (m_activate_flag);
    }


// ************* vCenterUpdateVcenterMessage ************

    vCenterUpdateVcenterMessage::vCenterUpdateVcenterMessage ()
        : PrismMessage (vCenterObjectManager::getPrismServiceId (), VCENTERUPDATEVCENTER)
    {
        m_id_flag = false;
        m_ipaddress_flag = false;
        m_username_flag = false;
        m_password_flag = false;
        m_activate		= false;
        m_activate_flag = false;
        m_interval		= 3;
        m_interval_flag = false;
        m_ignore_value  = 0;
        m_ignore_value_flag = false;
        m_always        = false;
        m_always_flag   = false;

    }

    vCenterUpdateVcenterMessage::vCenterUpdateVcenterMessage (const string &id,
        const string &ipaddress,
        const string &username,
        const string &password,
        const bool &activate)
        : PrismMessage (vCenterObjectManager::getPrismServiceId (), VCENTERUPDATEVCENTER),
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
        m_interval		= 3;
        m_interval_flag = false;
        m_ignore_value  = 0;
        m_ignore_value_flag = false;
        m_always        = false;
        m_always_flag   = false;

    }

    vCenterUpdateVcenterMessage::~vCenterUpdateVcenterMessage ()
    {
    }

    void  vCenterUpdateVcenterMessage::setupAttributesForSerialization()
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
        addSerializableAttribute (new AttributeUI32(&m_interval,"interval"));
        addSerializableAttribute (new AttributeBool(&m_interval_flag,"intervalFlag"));
        addSerializableAttribute (new AttributeUI32(&m_ignore_value,"ignoreValue"));
        addSerializableAttribute (new AttributeBool(&m_ignore_value_flag,"ignoreValueFlag"));
        addSerializableAttribute (new AttributeBool(&m_always,"always"));
        addSerializableAttribute (new AttributeBool(&m_always_flag,"alwaysFlag"));

    }

    void  vCenterUpdateVcenterMessage::setId(const string &id)
    {
        m_id = id;
        m_id_flag = true;
    }

    string  vCenterUpdateVcenterMessage::getId() const
    {
        return (m_id);
    }

    bool  vCenterUpdateVcenterMessage::getIdFlag() const
    {
        return (m_id_flag);
    }

    void  vCenterUpdateVcenterMessage::setIpaddress(const string &ipaddress)
    {
        m_ipaddress = ipaddress;
        m_ipaddress_flag = true;
    }

    string  vCenterUpdateVcenterMessage::getIpaddress() const
    {
        return (m_ipaddress);
    }

    bool  vCenterUpdateVcenterMessage::getIpaddressFlag() const
    {
        return (m_ipaddress_flag);
    }

    void  vCenterUpdateVcenterMessage::setUsername(const string &username)
    {
        m_username = username;
        m_username_flag = true;
    }

    string  vCenterUpdateVcenterMessage::getUsername() const
    {
        return (m_username);
    }

    bool  vCenterUpdateVcenterMessage::getUsernameFlag() const
    {
        return (m_username_flag);
    }

    void  vCenterUpdateVcenterMessage::setPassword(const string &password)
    {
        m_password = password;
        m_password_flag = true;
    }

    string  vCenterUpdateVcenterMessage::getPassword() const
    {
        return (m_password);
    }

    bool  vCenterUpdateVcenterMessage::getPasswordFlag() const
    {
        return (m_password_flag);
    }

    void  vCenterUpdateVcenterMessage::setActivate(const bool &activate)
    {
        m_activate = activate;
        m_activate_flag = true;
    }

    bool  vCenterUpdateVcenterMessage::getActivate() const
    {
        return (m_activate);
    }

    bool  vCenterUpdateVcenterMessage::getActivateFlag() const
    {
        return (m_activate_flag);
    }

    void  vCenterUpdateVcenterMessage::setInterval(const UI32 &interval)
    {
        m_interval = interval;
        m_interval_flag = true;
    }

    UI32  vCenterUpdateVcenterMessage::getInterval() const
    {
        return (m_interval);
    }

    bool  vCenterUpdateVcenterMessage::getIntervalFlag() const
    {
        return (m_interval_flag);
    }

    UI32  vCenterUpdateVcenterMessage::getIgnoreValue() const
    {
        return (m_ignore_value);
    }

    bool  vCenterUpdateVcenterMessage::getIgnoreValueFlag() const
    {
        return (m_ignore_value_flag);
    }

    void  vCenterUpdateVcenterMessage::setIgnoreValue(const UI32 &ignoreValue)
    {
        m_ignore_value = ignoreValue;
        m_ignore_value_flag = true;
    }

    void vCenterUpdateVcenterMessage::setAlways (const bool &always)
    {
        m_always = always;
        m_always_flag = true;
    }

    bool  vCenterUpdateVcenterMessage::getAlways() const
    {
        return (m_always);
    }

    bool  vCenterUpdateVcenterMessage::getAlwaysFlag() const
    {
        return (m_always_flag);
    }


// ************* vCenterDeleteVcenterMessage ************

    vCenterDeleteVcenterMessage::vCenterDeleteVcenterMessage ()
        : PrismMessage (vCenterObjectManager::getPrismServiceId (), VCENTERDELETEVCENTER)
    {
        m_id_flag = false;
    }

    vCenterDeleteVcenterMessage::vCenterDeleteVcenterMessage (const string &id)
        : PrismMessage (vCenterObjectManager::getPrismServiceId (), VCENTERDELETEVCENTER),
        m_id    (id)
    {
        m_id_flag = false;
    }

    vCenterDeleteVcenterMessage::~vCenterDeleteVcenterMessage ()
    {
    }

    void  vCenterDeleteVcenterMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeBool(&m_id_flag,"idFlag"));
    }

    void  vCenterDeleteVcenterMessage::setId(const string &id)
    {
        m_id = id;
        m_id_flag = true;
    }

    string  vCenterDeleteVcenterMessage::getId() const
    {
        return (m_id);
    }

    bool  vCenterDeleteVcenterMessage::getIdFlag() const
    {
        return (m_id_flag);
    }

}
