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

#include "APPM/Global/APPMVlanClassifierManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
//#include "vcs.h"
#include "brocade-port-profile.h"
namespace DcmNs
{

    APPMVlanClassifierManagedObject::APPMVlanClassifierManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (APPMVlanClassifierManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
		m_vlanclassifieroperation (0)
    {
    }

    APPMVlanClassifierManagedObject::APPMVlanClassifierManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const UI32 &grpId,const UI32 &vlanclassifieroperation,const UI32 &vlanId)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (APPMVlanClassifierManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        m_grpId    (grpId),
        m_vlanclassifieroperation    (vlanclassifieroperation),
        m_vlanId    (vlanId)
    {
    }

    APPMVlanClassifierManagedObject::~APPMVlanClassifierManagedObject ()
    {
    }

    string  APPMVlanClassifierManagedObject::getClassName()
    {
        return ("APPMVlanClassifierManagedObject");
    }

    void  APPMVlanClassifierManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_grpId,"grpId"));
        addPersistableAttribute (new AttributeEnum((UI32 *) (&m_vlanclassifieroperation),"vlanclassifieroperation"));
        addPersistableAttribute (new AttributeUI32(&m_vlanId,"vlanId",appm_vlan));
#if 0
		setUserTagForAttribute("grpId", appm_groupid);
		setUserTagForAttribute("dummy", appm_dummy);
		setUserTagForAttribute("vlanId", appm_vlan);
#endif
    }

    void  APPMVlanClassifierManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_grpId,"grpId"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *) (&m_vlanclassifieroperation),"vlanclassifieroperation"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vlanId,"vlanId",appm_vlan));
    }

    void APPMVlanClassifierManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("grpId");
        keyName.push_back("vlanclassifieroperation");
        keyName.push_back("vlanId");
        setUserDefinedKeyCombination (keyName);
    }

    void  APPMVlanClassifierManagedObject::setGrpId(const UI32 &grpId)
    {
        m_grpId  =  grpId;
    }

    UI32  APPMVlanClassifierManagedObject::getGrpId() const
    {
        return (m_grpId);
    }

    void  APPMVlanClassifierManagedObject::setVlanclassifierOperation(const UI32 &vlanclassifieroperation)
    {
        m_vlanclassifieroperation  =  vlanclassifieroperation;
    }

    UI32  APPMVlanClassifierManagedObject::getVlanclassifierOperation() const
    {
        return (m_vlanclassifieroperation);
    }

    void  APPMVlanClassifierManagedObject::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  APPMVlanClassifierManagedObject::getVlanId() const
    {
        return (m_vlanId);
    }

}
