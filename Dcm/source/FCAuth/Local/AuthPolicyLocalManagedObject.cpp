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
#include "FCAuth/Local/AuthPolicyLocalManagedObject.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/FCAuthTypes.h"

#include "brocade-fc-auth.h"
namespace DcmNs
{

    AuthPolicyLocalManagedObject::AuthPolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (AuthPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager)
    {
        m_EnforceType = passive;
    }

    AuthPolicyLocalManagedObject::AuthPolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const AuthEnforceType &EnforceType)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (AuthPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager),
        m_EnforceType    (EnforceType)
    {
    }

    AuthPolicyLocalManagedObject::~AuthPolicyLocalManagedObject ()
    {
    }

    string  AuthPolicyLocalManagedObject::getClassName()
    {
        return ("AuthPolicyLocalManagedObject");
    }

    void  AuthPolicyLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_EnforceType,"EnforceType",brcd_fc_auth_switch));
    }

    void  AuthPolicyLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_EnforceType,"EnforceType",brcd_fc_auth_switch));
    }

    void  AuthPolicyLocalManagedObject::setEnforceType(const AuthEnforceType &EnforceType)
    {
        m_EnforceType  =  EnforceType;
    }

    AuthEnforceType  AuthPolicyLocalManagedObject::getEnforceType() const
    {
        return (m_EnforceType);
    }

}
