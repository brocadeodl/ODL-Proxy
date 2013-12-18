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

#include "L2sys/Global/L2sysAgingTimeoutManagedObject.h"
#include "L2sys/Global/L2sysGlobalObjectManager.h"
#include "L2sys/Global/L2sysGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    L2sysAgingTimeoutManagedObject::L2sysAgingTimeoutManagedObject (L2sysGlobalObjectManager *pL2sysGlobalObjectManager)
        : PrismElement  (pL2sysGlobalObjectManager),
        PrismPersistableObject (L2sysAgingTimeoutManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pL2sysGlobalObjectManager),
        DcmManagedObject (pL2sysGlobalObjectManager),
        m_AgingTimeout    (L2SYS_DCM_MAC_AGING_DEFAULT)
    {
    }

    L2sysAgingTimeoutManagedObject::L2sysAgingTimeoutManagedObject (L2sysGlobalObjectManager *pL2sysGlobalObjectManager,const UI32 &AgingTimeout)
        : PrismElement  (pL2sysGlobalObjectManager),
        PrismPersistableObject (L2sysAgingTimeoutManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pL2sysGlobalObjectManager),
        DcmManagedObject (pL2sysGlobalObjectManager),
        m_AgingTimeout    (AgingTimeout)
    {
    }

    L2sysAgingTimeoutManagedObject::~L2sysAgingTimeoutManagedObject ()
    {
    }

    string  L2sysAgingTimeoutManagedObject::getClassName()
    {
        return ("L2sysAgingTimeoutManagedObject");
    }


    Attribute *L2sysAgingTimeoutManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
        Attribute *pAttribute = NULL;
        pAttribute = new AttributeBool (true, "l2sys");
        trace (TRACE_LEVEL_DEBUG,
                string("L2sysAgingTimeoutManagedObject::getAttributeByUserTagUserDefined: Entry "));
        return (pAttribute);
    }

    void  L2sysAgingTimeoutManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_AgingTimeout,"AgingTimeout", mac_address_table_aging_time));
    }

    void  L2sysAgingTimeoutManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_AgingTimeout,"AgingTimeout", mac_address_table_aging_time));
    }

    void  L2sysAgingTimeoutManagedObject::setAgingTimeout(const UI32 &AgingTimeout)
    {
        m_AgingTimeout  =  AgingTimeout;
    }

    UI32  L2sysAgingTimeoutManagedObject::getAgingTimeout() const
    {
        return (m_AgingTimeout);
    }

}
