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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "APPM/Global/AppmEnableManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    AppmEnableManagedObject::AppmEnableManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (AppmEnableManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager)
    {
    }

    AppmEnableManagedObject::AppmEnableManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const bool &status)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (AppmEnableManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        m_status    (status)
    {
    }

    AppmEnableManagedObject::~AppmEnableManagedObject ()
    {
    }

    string  AppmEnableManagedObject::getClassName()
    {
        return ("AppmEnableManagedObject");
    }

    void  AppmEnableManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_status,"status"));
    }

    void  AppmEnableManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_status,"status"));
    }

    void  AppmEnableManagedObject::setStatus(const bool &status)
    {
        m_status  =  status;
    }

    bool  AppmEnableManagedObject::getStatus() const
    {
        return (m_status);
    }

}
