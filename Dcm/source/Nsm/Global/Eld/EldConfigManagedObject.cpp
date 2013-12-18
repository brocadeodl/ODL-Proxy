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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#include "Nsm/Global/Eld/EldConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    EldConfigManagedObject::EldConfigManagedObject (NsmGlobalObjectManager 
                                                    *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (EldConfigManagedObject::getClassName (), 
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_dot1xenable = false;
		m_nativevlanDisable = true;
		m_systemPriority = 32768;
		m_rxlim = 1;
		m_hello = 1000;
		m_shutdown = 0;
    }

    EldConfigManagedObject::EldConfigManagedObject (NsmGlobalObjectManager 
                        *pNsmGlobalObjectManager,const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (EldConfigManagedObject::getClassName (), 
        DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_systemPriority    (systemPriority),
        m_dot1xenable    (dot1xenable),
        m_nativevlanDisable    (nativevlanDisable)
    {
		m_rxlim = 1;
		m_hello = 1000;
		m_shutdown = 0;
    }

    EldConfigManagedObject::~EldConfigManagedObject ()
    {
    }

    string  EldConfigManagedObject::getClassName()
    {
        return ("EldConfigManagedObject");
    }

    void  EldConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_rxlim, "rxlim", eld_pdu_rx_limit));
        addPersistableAttribute (new AttributeUI32(&m_hello, "hello", eld_hello_interval));
        addPersistableAttribute (new AttributeUI32(&m_shutdown, "shutdown", eld_shutdown_time));
    }

    void  EldConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_rxlim, "rxlim", eld_pdu_rx_limit));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_hello, "hello", eld_hello_interval));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_shutdown, "shutdown", eld_shutdown_time));
    }

    void  EldConfigManagedObject::setRxlim(const UI32 &rxlim)
    {
        m_rxlim  =  rxlim;
    }

    UI32  EldConfigManagedObject::getRxlim() const
    {
        return (m_rxlim);
    }

    void  EldConfigManagedObject::setHello(const UI32 &hello)
    {
        m_hello  =  hello;
    }

    UI32  EldConfigManagedObject::getHello() const
    {
        return (m_hello);
    }

    void  EldConfigManagedObject::setShutdown(const UI32 &shutdown)
    {
        m_shutdown  =  shutdown;
    }

    UI32  EldConfigManagedObject::getShutdown() const
    {
        return (m_shutdown);
    }
}
