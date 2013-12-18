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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"
#include "Qos/Global/CeeMapManagedObject.h"

#include "vcs.h"
namespace DcmNs
{

    FcoeMapManagedObject::FcoeMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager)
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager)
    {
    }

    FcoeMapManagedObject::FcoeMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager,const string &map_name,const ObjectId &cee_map,const vector<ObjectId> &fabric_map_list)
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager),
        m_map_name    (map_name),
        m_cee_map    (cee_map),
        m_fabric_map_list    (fabric_map_list)
    {
    }

    FcoeMapManagedObject::~FcoeMapManagedObject ()
    {
    }

    string  FcoeMapManagedObject::getClassName()
    {
        return ("FcoeMapManagedObject");
    }

    void  FcoeMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_map_name,"map_name"));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cee_map,"cee_map", CeeMapManagedObject::getClassName (), true));
        addPersistableAttribute (new AttributeObjectIdVectorAssociation(&m_fabric_map_list,"fabric_map_list", getClassName (), getObjectId (), FcoeFabricMapManagedObject::getClassName ()));

        setUserTagForAttribute("map_name", fcoe_fcoe_map_name);
        setUserTagForAttribute("cee_map", fcoe_fcoe_map_cee_map_leaf);
        setUserTagForAttribute("fabric_map_list", fcoe_fcoe_map_fabric_map_name);
    }

    void  FcoeMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_map_name,"map_name"));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cee_map,"cee_map", CeeMapManagedObject::getClassName (), true));
        addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_fabric_map_list,"fabric_map_list", getClassName (), getObjectId (), FcoeFabricMapManagedObject::getClassName ()));

        vector<string > keyName;
        keyName.push_back ("map_name");
        setUserDefinedKeyCombination (keyName);

    }

    void  FcoeMapManagedObject::setMap_name(const string &map_name)
    {
        m_map_name  =  map_name;
    }

    string  FcoeMapManagedObject::getMap_name() const
    {
        return (m_map_name);
    }

    void  FcoeMapManagedObject::setCee_map(const ObjectId &cee_map)
    {
        m_cee_map  =  cee_map;
    }

    ObjectId  FcoeMapManagedObject::getCee_map() const
    {
        return (m_cee_map);
    }

    void FcoeMapManagedObject::clearCeeMap ()
    {
        m_cee_map = ObjectId::NullObjectId;
    }

    void  FcoeMapManagedObject::setFabric_map_list(const vector<ObjectId> &fabric_map_list)
    {
        m_fabric_map_list  =  fabric_map_list;
    }

    vector<ObjectId>  FcoeMapManagedObject::getFabric_map_list() const
    {
        return (m_fabric_map_list);
    }

    void FcoeMapManagedObject::addFabricMap (const ObjectId &object_id)
    {
        m_fabric_map_list.push_back(object_id);
    }

    void FcoeMapManagedObject::remFabricMap (const ObjectId &object_id)
    {
        // Only for VIRGO, since we have only one Fab Map.
        // Need to fix this once we have multiple fab-maps in Mercury.
        // TODO:
        m_fabric_map_list.pop_back();
    }

}
