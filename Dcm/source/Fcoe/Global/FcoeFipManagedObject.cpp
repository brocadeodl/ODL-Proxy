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

#include "Fcoe/Global/FcoeFipManagedObject.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    FcoeFipManagedObject::FcoeFipManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager)
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeFipManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager)
    {
    }

    FcoeFipManagedObject::FcoeFipManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager,const SI32 &fka_intvl,const bool &timeout)
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeFipManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager),
        m_fka_intvl    (fka_intvl),
        m_timeout    (timeout)
    {
    }

    FcoeFipManagedObject::~FcoeFipManagedObject ()
    {
    }

    string  FcoeFipManagedObject::getClassName()
    {
        return ("FcoeFipManagedObject");
    }

    void  FcoeFipManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_fka_intvl,"fka_intvl"));
        addPersistableAttribute (new AttributeBool(&m_timeout,"timeout"));

        setUserTagForAttribute("fka_intvl", fcoe_fcoe_fip_advertisement_interval);
        setUserTagForAttribute("timeout", fcoe_fcoe_fip_keep_alive_timeout);
    }

    void  FcoeFipManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_fka_intvl,"fka_intvl"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_timeout,"timeout"));
    }

    void  FcoeFipManagedObject::setFka_intvl(const SI32 &fka_intvl)
    {
        m_fka_intvl  =  fka_intvl;
    }

    SI32  FcoeFipManagedObject::getFka_intvl() const
    {
        return (m_fka_intvl);
    }

    void  FcoeFipManagedObject::setTimeout(const bool &timeout)
    {
        m_timeout  =  timeout;
    }

    bool  FcoeFipManagedObject::getTimeout() const
    {
        return (m_timeout);
    }

}
