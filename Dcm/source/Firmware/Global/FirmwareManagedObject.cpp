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
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#include "Firmware/Global/FirmwareManagedObject.h"
#include "Firmware/Global/FirmwareObjectManager.h"
#include "Firmware/Global/FirmwareTypes.h"

namespace DcmNs
{

    FirmwareManagedObject::FirmwareManagedObject (FirmwareObjectManager *pFirmwareObjectManager)
        : PrismElement  (pFirmwareObjectManager),
        PrismPersistableObject (FirmwareManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFirmwareObjectManager),
        DcmManagedObject (pFirmwareObjectManager)
    {
    }

    FirmwareManagedObject::FirmwareManagedObject (FirmwareObjectManager *pFirmwareObjectManager, const UI32 &rbridge_id, const UI32 &state)
        : PrismElement  (pFirmwareObjectManager),
        PrismPersistableObject (FirmwareManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFirmwareObjectManager),
        DcmManagedObject (pFirmwareObjectManager),
		m_rbridge_id(rbridge_id),
		m_state	(state)
    {
    }

    FirmwareManagedObject::~FirmwareManagedObject ()
    {
    }

    string  FirmwareManagedObject::getClassName()
    {
        return ("FirmwareManagedObject");
    }

    void  FirmwareManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_rbridge_id,"rbridge_id"));
        addPersistableAttribute (new AttributeUI32(&m_state,"state"));
        addPersistableAttribute (new AttributeUI32(&m_cli_state,"cli_state"));
    }

    void  FirmwareManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI32(&m_rbridge_id,
			"rbridge_id"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_state, "state"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cli_state, "cli_state"));
    	/*vector<string > keyName;
		keyName.push_back ("name");
        setUserDefinedKeyCombination (keyName);     */

	}

    void FirmwareManagedObject::setRbridge_Id(const UI32 &rbridge_id)
    {
        m_rbridge_id  =  rbridge_id;
    }

    UI32 FirmwareManagedObject::getRbridge_Id() const
    {
        return (m_rbridge_id);
    }

    void FirmwareManagedObject::setState(const UI32 &state)
    {
        m_state  =  state;
    }

    UI32 FirmwareManagedObject::getState() const
    {
        return (m_state);
    }

    void FirmwareManagedObject::setCliState(const UI32 &state)
    {
        m_cli_state  =  state;
    }

    UI32 FirmwareManagedObject::getCliState() const
    {
        return (m_cli_state);
    }

}
