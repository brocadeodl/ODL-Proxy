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
 *   Author : skrastog                                                     *
 **************************************************************************/

#include "Nsm/Local/PortSecOuiLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

#include "brocade-interface.h"

namespace DcmNs
{

    PortSecOuiLocalManagedObject::PortSecOuiLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PortSecOuiLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    PortSecOuiLocalManagedObject::PortSecOuiLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PortSecOuiLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
    }

    PortSecOuiLocalManagedObject::PortSecOuiLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const string &oui)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PortSecOuiLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_oui    (oui)
    {
    }

    PortSecOuiLocalManagedObject::PortSecOuiLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const string &oui)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PortSecOuiLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager),
        m_oui    (oui)
    {
    }

    PortSecOuiLocalManagedObject::~PortSecOuiLocalManagedObject ()
    {
    }

    string  PortSecOuiLocalManagedObject::getClassName()
    {
        return ("PortSecOuiLocalManagedObject");
    }

    void  PortSecOuiLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_oui, "oui"));
        addPersistableAttribute (new AttributeString(&m_ifName, "ifName"));
		setUserTagForAttribute("oui", brocade_interface_oui);
    }

    void  PortSecOuiLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_oui, "oui", brocade_interface_oui));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName, "ifName"));
		vector<string > keyName;
		keyName.push_back("oui");
		setUserDefinedKeyCombination (keyName);
    }

    void PortSecOuiLocalManagedObject::setOui(const string &oui)
    {
        m_oui = oui;
    }

    string PortSecOuiLocalManagedObject::getOui() const
    {
        return m_oui;
    }

    void PortSecOuiLocalManagedObject::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    string PortSecOuiLocalManagedObject::getIfName() const
    {
        return m_ifName;
    }
}
