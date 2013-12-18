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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : bperuma                                                     *
 **************************************************************************/

#include "NTP_Clock/Global/NTPKeyManagedObject.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    NTPKeyManagedObject::NTPKeyManagedObject (NTPClockGlobalObjectManager *pNTPClockGlobalObjectManager)
        : PrismElement  (pNTPClockGlobalObjectManager),
        PrismPersistableObject (NTPKeyManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNTPClockGlobalObjectManager),
        DcmManagedObject (pNTPClockGlobalObjectManager)
    {
    }

    NTPKeyManagedObject::NTPKeyManagedObject (NTPClockGlobalObjectManager *pNTPClockGlobalObjectManager,const UI32 &Key_ID,const string &Key_Value)
        : PrismElement  (pNTPClockGlobalObjectManager),
        PrismPersistableObject (NTPKeyManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNTPClockGlobalObjectManager),
        DcmManagedObject (pNTPClockGlobalObjectManager),
        m_Key_ID    (Key_ID),
        m_Key_Value    (Key_Value)
    {
    }

    NTPKeyManagedObject::~NTPKeyManagedObject ()
    {
    }

    string  NTPKeyManagedObject::getClassName()
    {
        return ("NTPKeyManagedObject");
    }

    void  NTPKeyManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_index,"index"));
        addPersistableAttribute (new AttributeUI32(&m_Key_ID,"Key_ID", ntp_server_keyid));
        addPersistableAttribute (new AttributeString(&m_Key_Value,"Key_Value", ntp_server_md5));
    }

    void  NTPKeyManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeUI32(&m_index,"index"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Key_ID,"Key_ID", ntp_server_keyid));
        addPersistableAttributeForCreate  (new AttributeString(&m_Key_Value,"Key_Value", ntp_server_md5));
    }

	void NTPKeyManagedObject::setupKeys()
    {   
        vector<string> key;
        key.push_back ("Key_ID");
        setUserDefinedKeyCombination (key);
    }

	void  NTPKeyManagedObject::setIndex(const UI32 &index)
    {   
        m_index  =  index;
    }   

    UI32  NTPKeyManagedObject::getIndex() const
    {
        return (m_index);
    }

    void  NTPKeyManagedObject::setKey_ID(const UI32 &Key_ID)
    {
        m_Key_ID  =  Key_ID;
    }

    UI32  NTPKeyManagedObject::getKey_ID() const
    {
        return (m_Key_ID);
    }

    void  NTPKeyManagedObject::setKey_Value(const string &Key_Value)
    {
        m_Key_Value  =  Key_Value;
    }

    string  NTPKeyManagedObject::getKey_Value() const
    {
        return (m_Key_Value);
    }

}
