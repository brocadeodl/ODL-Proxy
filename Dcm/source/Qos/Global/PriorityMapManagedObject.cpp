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
 *   Author : pbalacha                                                     *
 **************************************************************************/
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Global/QosGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    PriorityMapManagedObject::PriorityMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (PriorityMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    PriorityMapManagedObject::PriorityMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const ObjectId &cos0ToPriorityGrouMap,const ObjectId &cos1ToPriorityGrouMap,const ObjectId &cos2ToPriorityGrouMap,const ObjectId &cos3ToPriorityGrouMap,const ObjectId &cos4ToPriorityGrouMap,const ObjectId &cos5ToPriorityGrouMap,const ObjectId &cos6ToPriorityGrouMap,const ObjectId &cos7ToPriorityGrouMap)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (PriorityMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_cos0ToPriorityGrouMap    (cos0ToPriorityGrouMap),
        m_cos1ToPriorityGrouMap    (cos1ToPriorityGrouMap),
        m_cos2ToPriorityGrouMap    (cos2ToPriorityGrouMap),
        m_cos3ToPriorityGrouMap    (cos3ToPriorityGrouMap),
        m_cos4ToPriorityGrouMap    (cos4ToPriorityGrouMap),
        m_cos5ToPriorityGrouMap    (cos5ToPriorityGrouMap),
        m_cos6ToPriorityGrouMap    (cos6ToPriorityGrouMap),
        m_cos7ToPriorityGrouMap    (cos7ToPriorityGrouMap)
    {
    }

    PriorityMapManagedObject::~PriorityMapManagedObject ()
    {
    }

    string  PriorityMapManagedObject::getClassName()
    {
        return ("PriorityMapManagedObject");
    }

    void  PriorityMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos0ToPriorityGrouMap,"cos0ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos1ToPriorityGrouMap,"cos1ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos2ToPriorityGrouMap,"cos2ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos3ToPriorityGrouMap,"cos3ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos4ToPriorityGrouMap,"cos4ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos5ToPriorityGrouMap,"cos5ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos6ToPriorityGrouMap,"cos6ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cos7ToPriorityGrouMap,"cos7ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));

        setUserTagForAttribute("cos0ToPriorityGrouMap", brocade_cee_map_map_cos0_pgid);
        setUserTagForAttribute("cos1ToPriorityGrouMap", brocade_cee_map_map_cos1_pgid);
        setUserTagForAttribute("cos2ToPriorityGrouMap", brocade_cee_map_map_cos2_pgid);
        setUserTagForAttribute("cos3ToPriorityGrouMap", brocade_cee_map_map_cos3_pgid);
        setUserTagForAttribute("cos4ToPriorityGrouMap", brocade_cee_map_map_cos4_pgid);
        setUserTagForAttribute("cos5ToPriorityGrouMap", brocade_cee_map_map_cos5_pgid);
        setUserTagForAttribute("cos6ToPriorityGrouMap", brocade_cee_map_map_cos6_pgid);
        setUserTagForAttribute("cos7ToPriorityGrouMap", brocade_cee_map_map_cos7_pgid);
        setUserTagForAttribute("ceeMapName", brocade_cee_map_cee_map);
    }

    void  PriorityMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos0ToPriorityGrouMap,"cos0ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos1ToPriorityGrouMap,"cos1ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos2ToPriorityGrouMap,"cos2ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos3ToPriorityGrouMap,"cos3ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos4ToPriorityGrouMap,"cos4ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos5ToPriorityGrouMap,"cos5ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos6ToPriorityGrouMap,"cos6ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cos7ToPriorityGrouMap,"cos7ToPriorityGrouMap", PriorityGroupManagedObject::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeString(&m_ceeMapName,"ceeMapName"));
    }

    void  PriorityMapManagedObject::setCos0ToPriorityGrouMap(const ObjectId &cos0ToPriorityGrouMap)
    {
        m_cos0ToPriorityGrouMap  =  cos0ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos0ToPriorityGrouMap() const
    {
        return (m_cos0ToPriorityGrouMap);
    }

    void  PriorityMapManagedObject::setCos1ToPriorityGrouMap(const ObjectId &cos1ToPriorityGrouMap)
    {
        m_cos1ToPriorityGrouMap  =  cos1ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos1ToPriorityGrouMap() const
    {
        return (m_cos1ToPriorityGrouMap);
    }

    void  PriorityMapManagedObject::setCos2ToPriorityGrouMap(const ObjectId &cos2ToPriorityGrouMap)
    {
        m_cos2ToPriorityGrouMap  =  cos2ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos2ToPriorityGrouMap() const
    {
        return (m_cos2ToPriorityGrouMap);
    }

    void  PriorityMapManagedObject::setCos3ToPriorityGrouMap(const ObjectId &cos3ToPriorityGrouMap)
    {
        m_cos3ToPriorityGrouMap  =  cos3ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos3ToPriorityGrouMap() const
    {
        return (m_cos3ToPriorityGrouMap);
    }

    void  PriorityMapManagedObject::setCos4ToPriorityGrouMap(const ObjectId &cos4ToPriorityGrouMap)
    {
        m_cos4ToPriorityGrouMap  =  cos4ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos4ToPriorityGrouMap() const
    {
        return (m_cos4ToPriorityGrouMap);
    }

    void  PriorityMapManagedObject::setCos5ToPriorityGrouMap(const ObjectId &cos5ToPriorityGrouMap)
    {
        m_cos5ToPriorityGrouMap  =  cos5ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos5ToPriorityGrouMap() const
    {
        return (m_cos5ToPriorityGrouMap);
    }

    void  PriorityMapManagedObject::setCos6ToPriorityGrouMap(const ObjectId &cos6ToPriorityGrouMap)
    {
        m_cos6ToPriorityGrouMap  =  cos6ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos6ToPriorityGrouMap() const
    {
        return (m_cos6ToPriorityGrouMap);
    }

    void  PriorityMapManagedObject::setCos7ToPriorityGrouMap(const ObjectId &cos7ToPriorityGrouMap)
    {
        m_cos7ToPriorityGrouMap  =  cos7ToPriorityGrouMap;
    }

    ObjectId  PriorityMapManagedObject::getCos7ToPriorityGrouMap() const
    {
        return (m_cos7ToPriorityGrouMap);
    }
    void  PriorityMapManagedObject::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName = ceeMapName;
    }

    string  PriorityMapManagedObject::getCeeMapName() const
    {
        return (m_ceeMapName);
    }
}
