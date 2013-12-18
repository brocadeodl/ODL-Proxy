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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "Nsm/Global/LacpConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    LacpConfigManagedObject::LacpConfigManagedObject (NsmGlobalObjectManager 
                                                    *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (LacpConfigManagedObject::getClassName (), 
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_dot1xenable = false;
		m_nativevlanDisable = true;
		m_systemPriority = 32768;
		m_dot1xtesttimeout = 10;
    }

    LacpConfigManagedObject::LacpConfigManagedObject (NsmGlobalObjectManager 
                        *pNsmGlobalObjectManager,const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (LacpConfigManagedObject::getClassName (), 
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_systemPriority    (systemPriority),
        m_dot1xenable    (dot1xenable),
        m_nativevlanDisable    (nativevlanDisable)
    {
    }

    LacpConfigManagedObject::LacpConfigManagedObject (NsmGlobalObjectManager 
                        *pNsmGlobalObjectManager,const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable, const UI32 dot1xtesttimeout)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (LacpConfigManagedObject::getClassName (), 
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_systemPriority    (systemPriority),
        m_dot1xenable    (dot1xenable),
        m_nativevlanDisable    (nativevlanDisable),
        m_dot1xtesttimeout    (dot1xtesttimeout)
    {
    }

    LacpConfigManagedObject::~LacpConfigManagedObject ()
    {
    }

    string  LacpConfigManagedObject::getClassName()
    {
        return ("LacpConfigManagedObject");
    }

    void  LacpConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_systemPriority,
                                                            "systemPriority"));
        addPersistableAttribute (new AttributeBool(&m_dot1xenable,
                                                            "dot1xenable"));
        addPersistableAttribute (new AttributeBool(&m_nativevlanDisable,
                                                            "nativevlanDisable", brocade_vlan_native));
        addPersistableAttribute (new AttributeUI32(&m_dot1xtesttimeout,
                                                            "dot1xtesttimeout"));
		setUserTagForAttribute ("systemPriority", 
                                                lacp_system_priority);
		setUserTagForAttribute ("dot1xenable", dot1x_enable);
		setUserTagForAttribute ("dot1xtesttimeout", dot1x_timeout);
    }

    void  LacpConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_systemPriority,
                                                            "systemPriority"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_dot1xenable,
                                                            "dot1xenable"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_nativevlanDisable,
                                                            "nativevlanDisable"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_dot1xtesttimeout,
                                                            "dot1xtesttimeout"));
		
#if 0
		vector<string > keyName;
        keyName.push_back ("systemPriority");
        setUserDefinedKeyCombination (keyName);
#endif
    }

    void  LacpConfigManagedObject::setSystemPriority(const UI32 &systemPriority)
    {
        m_systemPriority  =  systemPriority;
    }

    UI32  LacpConfigManagedObject::getSystemPriority() const
    {
        return (m_systemPriority);
    }

    void  LacpConfigManagedObject::setDot1xenable(const bool &dot1xenable)
    {
        m_dot1xenable  =  dot1xenable;
    }

    bool  LacpConfigManagedObject::getDot1xenable() const
    {
        return (m_dot1xenable);
    }
    
	void  LacpConfigManagedObject::setNativevlanDisable(const bool &nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  LacpConfigManagedObject::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }

    void  LacpConfigManagedObject::setDot1xtesttimeout(const UI32 &dot1xtesttimeout)
    {
        m_dot1xtesttimeout  =  dot1xtesttimeout;
    }

    UI32  LacpConfigManagedObject::getDot1xtesttimeout() const
    {
        return (m_dot1xtesttimeout);
    }
    
}
