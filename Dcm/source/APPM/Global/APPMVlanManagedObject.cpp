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

#include "APPM/Global/APPMVlanManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    APPMVlanManagedObject::APPMVlanManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (APPMVlanManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager)
    {
    }

    APPMVlanManagedObject::APPMVlanManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const UI32 &vlanId,const UI32 &modeTag)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (APPMVlanManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        m_vlanId    (vlanId),
        m_modeTag    (modeTag)
    {
    }

    APPMVlanManagedObject::~APPMVlanManagedObject ()
    {
    }

    string  APPMVlanManagedObject::getClassName()
    {
        return ("APPMVlanManagedObject");
    }

    void  APPMVlanManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addPersistableAttribute (new AttributeUI32(&m_modeTag,"modeTag"));
    }

    void  APPMVlanManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vlanId,"vlanId"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_modeTag,"modeTag"));
    }

    void  APPMVlanManagedObject::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  APPMVlanManagedObject::getVlanId() const
    {
        return (m_vlanId);
    }

    void  APPMVlanManagedObject::setModeTag(const UI32 &modeTag)
    {
        m_modeTag  =  modeTag;
    }

    UI32  APPMVlanManagedObject::getModeTag() const
    {
        return (m_modeTag);
    }

}
