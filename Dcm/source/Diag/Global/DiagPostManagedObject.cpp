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
 *   Author : sshaik                                                     *
 **************************************************************************/

#include "Diag/Global/DiagPostManagedObject.h"
#include "Diag/Global/DiagObjectManager.h"
#include "Diag/Global/DiagTypes.h"

#include "vcs.h"
//#include "brocade-diag.h"

namespace DcmNs
{

    DiagPostManagedObject::DiagPostManagedObject (DiagObjectManager *pDiagObjectManager)
        : PrismElement  (pDiagObjectManager),
        PrismPersistableObject (DiagPostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pDiagObjectManager),
        DcmManagedObject (pDiagObjectManager)
    {
    }

    DiagPostManagedObject::DiagPostManagedObject (DiagObjectManager *pDiagObjectManager,const bool &enabled)
        : PrismElement  (pDiagObjectManager),
        PrismPersistableObject (DiagPostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pDiagObjectManager),
        DcmManagedObject (pDiagObjectManager),
        m_enabled    (enabled)
    {
    }

    DiagPostManagedObject::~DiagPostManagedObject ()
    {
    }

    string  DiagPostManagedObject::getClassName()
    {
        return ("DiagPostManagedObject");
    }

    void  DiagPostManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enabled,"enabled", diagnostics_enable));
    }

    void  DiagPostManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_enabled,"enabled", diagnostics_enable));
    }

    void  DiagPostManagedObject::setDiagPost(const bool &enabled)
    {
        m_enabled  =  enabled;
    }

    bool  DiagPostManagedObject::getDiagPost() const
    {
        return (m_enabled);
    }

}
