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

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Nsm/Global/FcoeWorker/FcoeportAttrGlobalManagedObject.h"
#include "vcs.h"

namespace DcmNs
{

    FcoeportAttrGlobalManagedObject::FcoeportAttrGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (FcoeportAttrGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
    }

    FcoeportAttrGlobalManagedObject::FcoeportAttrGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const ObjectId &map)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (FcoeportAttrGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_map    (map)
    {
    }

    FcoeportAttrGlobalManagedObject::~FcoeportAttrGlobalManagedObject ()
    {
    }

    string  FcoeportAttrGlobalManagedObject::getClassName()
    {
        return ("FcoeportAttrGlobalManagedObject");
    }

    void  FcoeportAttrGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_map,"map", FcoeMapManagedObject::getClassName (), true));

        setUserTagForAttribute("map", fcoe_fcoeport_map);
    }

    void  FcoeportAttrGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_map,"map", FcoeMapManagedObject::getClassName (), true));
    }

    void  FcoeportAttrGlobalManagedObject::setMap(const ObjectId &map)
    {
        m_map = map;
    }

    ObjectId  FcoeportAttrGlobalManagedObject::getMap() const
    {
        return (m_map);
    }
}
