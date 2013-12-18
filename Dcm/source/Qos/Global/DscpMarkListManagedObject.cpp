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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nbellari                                                     *
 **************************************************************************/

#include "Qos/Global/DscpMarkListManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    DscpMarkListManagedObject::DscpMarkListManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (DscpMarkListManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    DscpMarkListManagedObject::DscpMarkListManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &markList,const SI32 &toValue)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (DscpMarkListManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_markList    (markList),
        m_toValue    (toValue)
    {
    }

    DscpMarkListManagedObject::~DscpMarkListManagedObject ()
    {
    }

    string  DscpMarkListManagedObject::getClassName()
    {
        return ("DscpMarkListManagedObject");
    }

    void  DscpMarkListManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_markList,"markList"));
        addPersistableAttribute (new AttributeSI32(&m_toValue,"toValue"));

		setUserTagForAttribute("markList", qos_dscp_in_values);
		setUserTagForAttribute("toValue", qos_to);
    }

    void  DscpMarkListManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_markList,"markList"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_toValue,"toValue"));

		vector<string> keyName;
		keyName.push_back("markList");
		setUserDefinedKeyCombination (keyName);
    }

    void  DscpMarkListManagedObject::setMarkList(const string &markList)
    {
        m_markList  =  markList;
    }

    string  DscpMarkListManagedObject::getMarkList() const
    {
        return (m_markList);
    }

    void  DscpMarkListManagedObject::setToValue(const SI32 &toValue)
    {
        m_toValue  =  toValue;
    }

    SI32  DscpMarkListManagedObject::getToValue() const
    {
        return (m_toValue);
    }

}
