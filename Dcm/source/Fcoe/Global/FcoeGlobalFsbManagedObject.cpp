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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jaideep Singh Saluja                                         *
 **************************************************************************/

#include "Fcoe/Global/FcoeGlobalFsbManagedObject.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{   

    FcoeGlobalFsbManagedObject::FcoeGlobalFsbManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager)
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeGlobalFsbManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager)
    {
    }

    FcoeGlobalFsbManagedObject::FcoeGlobalFsbManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager,const bool &enable)   
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeGlobalFsbManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager),
        m_enable    (enable)
    {
    }

    FcoeGlobalFsbManagedObject::~FcoeGlobalFsbManagedObject ()
    {
    }

    string  FcoeGlobalFsbManagedObject::getClassName()
    {
        return ("FcoeGlobalFsbManagedObject");
    }

    void  FcoeGlobalFsbManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enable,"enable"));

        setUserTagForAttribute("enable", fcoe_fcoe_fsb_enable);
    }

    void  FcoeGlobalFsbManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enable"));
    }

    void  FcoeGlobalFsbManagedObject::setMode(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  FcoeGlobalFsbManagedObject::getMode() const
    {
        return (m_enable);
    }

}
