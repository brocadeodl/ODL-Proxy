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
 *   Author : Balaji. M                                                    *
 **************************************************************************/

#if 0
#include "Framework/Attributes/AttributeEnum.h"
#include "AAA/Global/SecServicesGlobalManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-sec-services.h"

namespace DcmNs
{

    TelnetServerGlobalManagedObject::TelnetServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (TelnetServerGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager)
    {
    }

    TelnetServerGlobalManagedObject::TelnetServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const bool &disableFlag)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (TelnetServerGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager),
        m_disableFlag    (disableFlag)
    {
    }

    TelnetServerGlobalManagedObject::~TelnetServerGlobalManagedObject ()
    {
    }

    string  TelnetServerGlobalManagedObject::getClassName()
    {
        return ("TelnetServerGlobalManagedObject");
    }

    void  TelnetServerGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  TelnetServerGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  TelnetServerGlobalManagedObject::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  TelnetServerGlobalManagedObject::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerGlobalManagedObject::SSHServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (SSHServerGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager)
    {
    }

    SSHServerGlobalManagedObject::SSHServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,
            const bool &disableFlag)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (SSHServerGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager),
        m_disableFlag    (disableFlag)
    {
    }

    string  SSHServerGlobalManagedObject::getClassName()
    {
        return ("SSHServerGlobalManagedObject");
    }

    void  SSHServerGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  SSHServerGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  SSHServerGlobalManagedObject::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  SSHServerGlobalManagedObject::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerGlobalManagedObject::~SSHServerGlobalManagedObject ()
    {
    }
    
    SSHServerListManagedObject::SSHServerListManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (SSHServerListManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager)
    {
    }

    SSHServerListManagedObject::SSHServerListManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,
            const SSHKeyExchangeProtocol &protocol)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (SSHServerListManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager),
        m_protocol   (protocol)
    {
    }

    SSHServerListManagedObject::~SSHServerListManagedObject ()
    {
    }

    string  SSHServerListManagedObject::getClassName()
    {
        return ("SSHServerListManagedObject");
    }

    void  SSHServerListManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum ((UI32 *)(&m_protocol), "protocol", brcd_sec_services_protocol));
    }

    void  SSHServerListManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_protocol), "protocol", brcd_sec_services_protocol));
        vector<string> keyName;
        keyName.push_back ("protocol");
        setUserDefinedKeyCombination (keyName);
    }

    void  SSHServerListManagedObject::setKeyExchangeProtocol(const SSHKeyExchangeProtocol &protocol)
    {
        m_protocol  =  protocol;
    }

    SSHKeyExchangeProtocol SSHServerListManagedObject::getKeyExchangeProtocol() const
    {
        return (m_protocol);
    }
}
#endif
