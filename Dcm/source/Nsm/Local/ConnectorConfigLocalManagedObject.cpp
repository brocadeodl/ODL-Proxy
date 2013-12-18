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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Venkat                                                       *
 **************************************************************************/

#include "Nsm/Local/ConnectorConfigLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"

namespace DcmNs
{

    ConnectorConfigLocalManagedObject::ConnectorConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (ConnectorConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_name = "";
		m_breakout = false;
    }

    ConnectorConfigLocalManagedObject::ConnectorConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager, const string &name, const bool &breakout)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (ConnectorConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_name (name),
		m_breakout (breakout)
    {
    }

    ConnectorConfigLocalManagedObject::~ConnectorConfigLocalManagedObject ()
    {
    }

    string  ConnectorConfigLocalManagedObject::getClassName()
    {
        return ("ConnectorConfigLocalManagedObject");
    }

    void  ConnectorConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_name, "connectorName", hardware_name));
        addPersistableAttribute (new AttributeBool(&m_breakout, "breakout", hardware_breakout));
    }

    void  ConnectorConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_name, "connectorName", hardware_name));
        addPersistableAttributeForCreate  (new AttributeBool(&m_breakout, "breakout", hardware_breakout));
    }

	void ConnectorConfigLocalManagedObject::setupKeys()
	{
        vector<string> keyName;
		keyName.push_back ("connectorName");
		setUserDefinedKeyCombination (keyName);
	}

    void  ConnectorConfigLocalManagedObject::setName(const string &name)
    {
        m_name  =  name;
    }

    string  ConnectorConfigLocalManagedObject::getName() const
    {
        return (m_name);
    }

    void  ConnectorConfigLocalManagedObject::setBreakout(const bool &breakout)
    {
        m_breakout  =  breakout;
    }

    bool  ConnectorConfigLocalManagedObject::getBreakout() const
    {
        return (m_breakout);
    }

}
