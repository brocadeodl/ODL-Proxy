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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "NTP_Clock/Global/NTPServerManagedObject.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

//#include "ntp.h"
#include "vcs.h"
namespace DcmNs
{

    NTPServerManagedObject::NTPServerManagedObject (NTPClockGlobalObjectManager *pNTPClockGlobalObjectManager)
        : PrismElement  (pNTPClockGlobalObjectManager),
        PrismPersistableObject (NTPServerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNTPClockGlobalObjectManager),
        DcmManagedObject (pNTPClockGlobalObjectManager)
    {
    }

    NTPServerManagedObject::NTPServerManagedObject (NTPClockGlobalObjectManager *pNTPClockGlobalObjectManager,const IpVxAddress &server, const UI32 &Key)
        : PrismElement  (pNTPClockGlobalObjectManager),
        PrismPersistableObject (NTPServerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNTPClockGlobalObjectManager),
        DcmManagedObject (pNTPClockGlobalObjectManager),
        m_server    (server),
        m_Key       (Key)
    {
    }

    NTPServerManagedObject::~NTPServerManagedObject ()
    {
    }

    string  NTPServerManagedObject::getClassName()
    {
        return ("NTPServerManagedObject");
    }

    void  NTPServerManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_index,"index"));
        addPersistableAttribute (new AttributeIpVxAddress(&m_server,"server", ntp_server_ip));
        addPersistableAttribute (new AttributeUI32(&m_Key, "Key", ntp_server_key));
    }

    void  NTPServerManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_index,"index"));
        addPersistableAttributeForCreate  (new AttributeIpVxAddress(&m_server,"server", ntp_server_ip));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Key,"Key", ntp_server_key));
    }

    void NTPServerManagedObject::setupKeys()
    {
		vector<string> key;
		key.push_back ("server");
		setUserDefinedKeyCombination (key);
	}

    void  NTPServerManagedObject::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  NTPServerManagedObject::getIndex() const
    {
        return (m_index);
    }

    void  NTPServerManagedObject::setServer(const IpVxAddress &server)
    {
        m_server  =  server;
    }

    IpVxAddress  NTPServerManagedObject::getServer() const
    {
        return (m_server);
    }

    void NTPServerManagedObject::setKey(const UI32 &Key)
    {
        m_Key = Key;
    }

    UI32 NTPServerManagedObject::getKey() const
    {
        return (m_Key);
    }

}
