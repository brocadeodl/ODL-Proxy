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

#include "Framework/Attributes/AttributeEnum.h"
#include "AAA/Local/SecServicesLocalManagedObject.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"

#include "brocade-sec-services.h"

namespace DcmNs
{

    TelnetServerLocalManagedObject::TelnetServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager)
        : PrismElement  (pAAALocalLocalObjectManager),
        PrismPersistableObject (TelnetServerLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalLocalObjectManager),
		DcmManagedObject (pAAALocalLocalObjectManager),
		WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalLocalObjectManager)
    {
    }

    TelnetServerLocalManagedObject::TelnetServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager,const bool &disableFlag)
        : PrismElement  (pAAALocalLocalObjectManager),
        PrismPersistableObject (TelnetServerLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalLocalObjectManager),
		DcmManagedObject (pAAALocalLocalObjectManager),
		WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalLocalObjectManager),
        m_disableFlag    (disableFlag)
    {
    }

    TelnetServerLocalManagedObject::~TelnetServerLocalManagedObject ()
    {
    }

    string  TelnetServerLocalManagedObject::getClassName()
    {
        return ("TelnetServerLocalManagedObject");
    }

    void  TelnetServerLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  TelnetServerLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  TelnetServerLocalManagedObject::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  TelnetServerLocalManagedObject::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerLocalManagedObject::SSHServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager)
        : PrismElement  (pAAALocalLocalObjectManager),
        PrismPersistableObject (SSHServerLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalLocalObjectManager),
		DcmManagedObject (pAAALocalLocalObjectManager),
		WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalLocalObjectManager)
    {
    }

    SSHServerLocalManagedObject::SSHServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager,
            const bool &disableFlag)
        : PrismElement  (pAAALocalLocalObjectManager),
        PrismPersistableObject (SSHServerLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalLocalObjectManager),
		DcmManagedObject (pAAALocalLocalObjectManager),
		WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalLocalObjectManager),
        m_disableFlag    (disableFlag)
    {
    }

    string  SSHServerLocalManagedObject::getClassName()
    {
        return ("SSHServerLocalManagedObject");
    }

    void  SSHServerLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  SSHServerLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_disableFlag, "disableFlag", brcd_sec_services_shutdown));
    }

    void  SSHServerLocalManagedObject::setDisableFlag(const bool &disableFlag)
    {
        m_disableFlag  =  disableFlag;
    }

    bool  SSHServerLocalManagedObject::getDisableFlag() const
    {
        return (m_disableFlag);
    }

    SSHServerLocalManagedObject::~SSHServerLocalManagedObject ()
    {
    }
    
    SSHServerListLocalManagedObject::SSHServerListLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager)
        : PrismElement  (pAAALocalLocalObjectManager),
        PrismPersistableObject (SSHServerListLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalLocalObjectManager),
		DcmManagedObject (pAAALocalLocalObjectManager),
		WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalLocalObjectManager)
    {
    }

    SSHServerListLocalManagedObject::SSHServerListLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager,
            const SSHLocalKeyExchangeProtocol &protocol)
        : PrismElement  (pAAALocalLocalObjectManager),
        PrismPersistableObject (SSHServerListLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalLocalObjectManager),
		DcmManagedObject (pAAALocalLocalObjectManager),
		WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalLocalObjectManager),
        m_protocol   (protocol)
    {
    }

    SSHServerListLocalManagedObject::~SSHServerListLocalManagedObject ()
    {
    }

    string  SSHServerListLocalManagedObject::getClassName()
    {
        return ("SSHServerListLocalManagedObject");
    }

    void  SSHServerListLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum ((UI32 *)(&m_protocol), "protocol", brcd_sec_services_protocol));
    }

    void  SSHServerListLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_protocol), "protocol", brcd_sec_services_protocol));
        vector<string> keyName;
        keyName.push_back ("protocol");
        setUserDefinedKeyCombination (keyName);
    }

    void  SSHServerListLocalManagedObject::setKeyExchangeProtocol(const SSHLocalKeyExchangeProtocol &protocol)
    {
        m_protocol  =  protocol;
    }

    SSHLocalKeyExchangeProtocol SSHServerListLocalManagedObject::getKeyExchangeProtocol() const
    {
        return (m_protocol);
    }
}
