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

#include "vCenter/Framework/vCenterManagedObjects.h"
#include "vCenter/Framework/vCenterObjectManager.h"
#include "vCenter/Framework/vCenterTypes.h"
#include "vCenter/User/vCenterDcmDef.h"

#include "vcs.h"
/*#include "brocade-vswitch.h"*/

namespace DcmNs
{

// ************* VcenterManagedObject ************

    VcenterManagedObject::VcenterManagedObject (vCenterObjectManager *pvCenterObjectManager)
        : PrismElement  (pvCenterObjectManager),
        PrismPersistableObject (VcenterManagedObject::getClassName (),
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pvCenterObjectManager),
        DcmManagedObject (pvCenterObjectManager)
    {
		m_activate = false;
		m_interval = DISCOVER_INTERVAL_DEFAULT_MIN;
        m_ignore_value = DISCOVER_IGNORE_COUNT;
        m_always  = false;

    }

    VcenterManagedObject::VcenterManagedObject (vCenterObjectManager *pvCenterObjectManager,const string &id,
        const string &ipaddress,
        const string &username,
        const string &password,
        const bool &activate)
        : PrismElement  (pvCenterObjectManager),
        PrismPersistableObject (VcenterManagedObject::getClassName (),
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pvCenterObjectManager),
        DcmManagedObject (pvCenterObjectManager),
        m_id    (id),
        m_ipaddress    (ipaddress),
        m_username    (username),
        m_password    (password),
        m_activate    (activate)
    {
		m_activate = false;
		m_interval = DISCOVER_INTERVAL_DEFAULT_MIN;
        m_ignore_value = DISCOVER_IGNORE_COUNT;
        m_always  = false;

    }

    VcenterManagedObject::~VcenterManagedObject ()
    {
    }

    string  VcenterManagedObject::getClassName()
    {
        return ("VcenterManagedObject");
    }

    void  VcenterManagedObject::setupAttributesForPersistence()
    {
		UI32 intervalDefault = DISCOVER_INTERVAL_DEFAULT_MIN;
		UI32 ignoreDefault = DISCOVER_IGNORE_COUNT;
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_id,"id", vswitch_id));
        addPersistableAttribute (new AttributeString(&m_ipaddress,"ipaddress", vswitch_url));
        addPersistableAttribute (new AttributeString(&m_username,"username", vswitch_username));
        addPersistableAttribute (new AttributeString(&m_password,"password", vswitch_password));
        addPersistableAttribute (new AttributeBool(&m_activate,"activate", vswitch_activate));
        addPersistableAttribute (new AttributeUI32(&m_interval, intervalDefault,"interval", vswitch_interval));
        addPersistableAttribute (new AttributeUI32(&m_ignore_value,ignoreDefault,"ignoreValue", vswitch_ignore_value));
        addPersistableAttribute (new AttributeBool(&m_always, "always", vswitch_always));

    }

    void  VcenterManagedObject::setupAttributesForCreate()
    {
		UI32 intervalDefault = DISCOVER_INTERVAL_DEFAULT_MIN;
		UI32 ignoreDefault = DISCOVER_IGNORE_COUNT;
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_id,"id", vswitch_id));
        addPersistableAttributeForCreate  (new AttributeString(&m_ipaddress,"ipaddress", vswitch_url));
        addPersistableAttributeForCreate  (new AttributeString(&m_username,"username", vswitch_username));
        addPersistableAttributeForCreate  (new AttributeString(&m_password,"password", vswitch_password));
        addPersistableAttributeForCreate  (new AttributeBool(&m_activate,"activate", vswitch_activate));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_interval, intervalDefault,"interval", vswitch_interval));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_ignore_value,ignoreDefault,"ignoreValue", vswitch_ignore_value));
        addPersistableAttributeForCreate  (new AttributeBool(&m_always,"always", vswitch_always));
    
        vector<string > keyName;
        keyName.push_back ("id");
        setUserDefinedKeyCombination (keyName);
}

    void  VcenterManagedObject::setId(const string &id)
    {
        m_id = id;
    }

    string  VcenterManagedObject::getId() const
    {
        return (m_id);
    }

    void  VcenterManagedObject::setIpaddress(const string &ipaddress)
    {
        m_ipaddress = ipaddress;
    }

    string  VcenterManagedObject::getIpaddress() const
    {
        return (m_ipaddress);
    }

    void  VcenterManagedObject::setUsername(const string &username)
    {
        m_username = username;
    }

    string  VcenterManagedObject::getUsername() const
    {
        return (m_username);
    }

    void  VcenterManagedObject::setPassword(const string &password)
    {
        m_password = password;
    }

    string  VcenterManagedObject::getPassword() const
    {
        return (m_password);
    }

    void  VcenterManagedObject::setActivate(const bool &activate)
    {
        m_activate = activate;
    }

    bool  VcenterManagedObject::getActivate() const
    {
        return (m_activate);
    }

    void  VcenterManagedObject::setInterval(const UI32 &interval)
    {
        m_interval = interval;
    }

    UI32  VcenterManagedObject::getInterval() const
    {
        return (m_interval);
    }

   void  VcenterManagedObject::setIgnoreValue(const UI32 &ignoreValue)
    {
        m_ignore_value = ignoreValue;
    }

    UI32  VcenterManagedObject::getIgnoreValue() const
    {
        return (m_ignore_value);
    }

    void  VcenterManagedObject::setAlways(const bool &always)
    {
        m_always = always;
    }

    bool  VcenterManagedObject::getAlways() const
    {
        return (m_always);
    }

}
