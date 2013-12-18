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

#include "Nsm/Local/FcoeWorker/FcoeportAttrLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/FcoeWorker/NsmFcoeWorkerTypes.h"

#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "vcs.h"

namespace DcmNs
{

    FcoeportAttrLocalManagedObject::FcoeportAttrLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (FcoeportAttrLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    FcoeportAttrLocalManagedObject::FcoeportAttrLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const ObjectId &map)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (FcoeportAttrLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_map    (map)
    {
    }

    FcoeportAttrLocalManagedObject::~FcoeportAttrLocalManagedObject ()
    {
    }

    string  FcoeportAttrLocalManagedObject::getClassName()
    {
        return ("FcoeportAttrLocalManagedObject");
    }

    void  FcoeportAttrLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_map,"map", FcoeMapManagedObject::getClassName (), false));

        setUserTagForAttribute("map", fcoe_fcoeport_map);
    }

    void  FcoeportAttrLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_map,"map", FcoeMapManagedObject::getClassName (), false));
    }

    void  FcoeportAttrLocalManagedObject::setMap(const ObjectId &map)
    {
        m_map = map;
    }

    ObjectId  FcoeportAttrLocalManagedObject::getMap() const
    {
        return (m_map);
    }
}
