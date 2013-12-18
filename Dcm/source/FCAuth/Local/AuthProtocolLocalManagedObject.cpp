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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "FCAuth/Local/AuthProtocolLocalManagedObject.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/FCAuthTypes.h"

#include "brocade-fc-auth.h"

namespace DcmNs
{

    AuthProtocolLocalManagedObject::AuthProtocolLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (AuthProtocolLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager)
    {
        m_ProtocolAuthType = dhchap;
        m_ProtocolAuthGroup = "*";
        m_ProtocolAuthHash = all;
    }

    AuthProtocolLocalManagedObject::AuthProtocolLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const AuthProtocolType &ProtocolAuthType,const AuthProtocolGroup &ProtocolAuthGroup,const AuthProtocolHash &ProtocolAuthHash)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (AuthProtocolLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager),
        m_ProtocolAuthType    (ProtocolAuthType),
        m_ProtocolAuthGroup    (ProtocolAuthGroup),
        m_ProtocolAuthHash    (ProtocolAuthHash)
    {
    }

    AuthProtocolLocalManagedObject::~AuthProtocolLocalManagedObject ()
    {
    }

    string  AuthProtocolLocalManagedObject::getClassName()
    {
        return ("AuthProtocolLocalManagedObject");
    }

    void  AuthProtocolLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_ProtocolAuthType,"ProtocolAuthType",brcd_fc_auth_auth_type));
        addPersistableAttribute (new AttributeString(&m_ProtocolAuthGroup,"ProtocolAuthGroup",brcd_fc_auth_group));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_ProtocolAuthHash,"ProtocolAuthHash",brcd_fc_auth_hash));
    }

    void  AuthProtocolLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_ProtocolAuthType,"ProtocolAuthType",brcd_fc_auth_auth_type));
        addPersistableAttributeForCreate  (new AttributeString(&m_ProtocolAuthGroup,"ProtocolAuthGroup",brcd_fc_auth_group));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_ProtocolAuthHash,"ProtocolAuthHash",brcd_fc_auth_hash));
    }

    void  AuthProtocolLocalManagedObject::setProtocolAuthType(const AuthProtocolType &ProtocolAuthType)
    {
        m_ProtocolAuthType  =  ProtocolAuthType;
    }

    AuthProtocolType  AuthProtocolLocalManagedObject::getProtocolAuthType() const
    {
        return (m_ProtocolAuthType);
    }

    void  AuthProtocolLocalManagedObject::setProtocolAuthGroup(const AuthProtocolGroup &ProtocolAuthGroup)
    {
        m_ProtocolAuthGroup  =  ProtocolAuthGroup;
    }

    AuthProtocolGroup  AuthProtocolLocalManagedObject::getProtocolAuthGroup() const
    {
        return (m_ProtocolAuthGroup);
    }

    void  AuthProtocolLocalManagedObject::setProtocolAuthHash(const AuthProtocolHash &ProtocolAuthHash)
    {
        m_ProtocolAuthHash  =  ProtocolAuthHash;
    }

    AuthProtocolHash  AuthProtocolLocalManagedObject::getProtocolAuthHash() const
    {
        return (m_ProtocolAuthHash);
    }

}
