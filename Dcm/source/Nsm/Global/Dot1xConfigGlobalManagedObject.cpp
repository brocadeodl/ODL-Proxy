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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Nsm/Global/Dot1xConfigGlobalManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    Dot1xConfigGlobalManagedObject::Dot1xConfigGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (Dot1xConfigGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_Enable = false;
    }

    Dot1xConfigGlobalManagedObject::Dot1xConfigGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const bool &Enable)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (Dot1xConfigGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_Enable    (Enable)
    {
    }

    Dot1xConfigGlobalManagedObject::~Dot1xConfigGlobalManagedObject ()
    {
    }

    string  Dot1xConfigGlobalManagedObject::getClassName()
    {
        return ("Dot1xConfigGlobalManagedObject");
    }

    void  Dot1xConfigGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_Enable,"Enable"));
    }

    void  Dot1xConfigGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_Enable,"Enable"));
    }

    void  Dot1xConfigGlobalManagedObject::setEnable(const bool &Enable)
    {
        m_Enable  =  Enable;
    }

    bool  Dot1xConfigGlobalManagedObject::getEnable() const
    {
        return (m_Enable);
    }

}
