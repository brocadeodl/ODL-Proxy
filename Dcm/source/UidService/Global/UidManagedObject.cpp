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
 *   Author : sholla                                                     *
 **************************************************************************/

#include "UidService/Global/UidManagedObject.h"
#include "UidService/Global/UidServiceObjectManager.h"
#include "UidService/Global/UidServiceTypes.h"

namespace DcmNs
{

    UidManagedObject::UidManagedObject (UidServiceObjectManager *pUidServiceObjectManager)
        : PrismElement  (pUidServiceObjectManager),
        PrismPersistableObject (UidManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject   (pUidServiceObjectManager),
        DcmManagedObject    (pUidServiceObjectManager),
        m_idType            (IDTYPE_INVALID_ID),
        m_id                (0),
        m_active            (false)
    {
    }

    UidManagedObject::UidManagedObject (UidServiceObjectManager *pUidServiceObjectManager,const string &entityName,const UI32 &idType,const UI32 &id,const bool &active)
        : PrismElement  (pUidServiceObjectManager),
        PrismPersistableObject (UidManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pUidServiceObjectManager),
        DcmManagedObject (pUidServiceObjectManager),
        m_entityName    (entityName),
        m_idType    (idType),
        m_id    (id),
        m_active    (active)
    {
    }

    UidManagedObject::~UidManagedObject ()
    {
    }

    string  UidManagedObject::getClassName()
    {
        return ("UidManagedObject");
    }

    void  UidManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_entityName,"entityName"));
        addPersistableAttribute (new AttributeUI32(&m_idType,"idType"));
        addPersistableAttribute (new AttributeUI32(&m_id,"id"));
        addPersistableAttribute (new AttributeBool(&m_active,"active"));
    }

    void  UidManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_entityName,"entityName"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_idType,"idType"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_id,"id"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_active,"active"));
    }

    void  UidManagedObject::setEntityName(const string &entityName)
    {
        m_entityName  =  entityName;
    }

    string  UidManagedObject::getEntityName() const
    {
        return (m_entityName);
    }

    void  UidManagedObject::setIdType(const UI32 &idType)
    {
        m_idType  =  idType;
    }

    UI32  UidManagedObject::getIdType() const
    {
        return (m_idType);
    }

    void  UidManagedObject::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  UidManagedObject::getId() const
    {
        return (m_id);
    }

    void  UidManagedObject::setActive(const bool &active)
    {
        m_active  =  active;
    }

    bool  UidManagedObject::getActive() const
    {
        return (m_active);
    }

}
