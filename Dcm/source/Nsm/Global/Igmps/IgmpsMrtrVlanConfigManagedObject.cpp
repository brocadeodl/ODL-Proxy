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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Nsm/Global/Igmps/IgmpsMrtrVlanConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    IgmpsMrtrVlanConfigManagedObject::IgmpsMrtrVlanConfigManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (IgmpsMrtrVlanConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_ifType = 0;
    }

    IgmpsMrtrVlanConfigManagedObject::IgmpsMrtrVlanConfigManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &ifType,const string &ifName)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (IgmpsMrtrVlanConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_ifType    (ifType),
        m_ifName    (ifName)
    {
    }

    IgmpsMrtrVlanConfigManagedObject::~IgmpsMrtrVlanConfigManagedObject ()
    {
    }

    string  IgmpsMrtrVlanConfigManagedObject::getClassName()
    {
        return ("IgmpsMrtrVlanConfigManagedObject");
    }

    void  IgmpsMrtrVlanConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32*) &m_ifType,"ifType"));
        addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));
        
        setUserTagForAttribute("ifType", igmp_snooping_if_type);
        setUserTagForAttribute("ifName", igmp_snooping_value);
    }

    void  IgmpsMrtrVlanConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_ifType,"ifType"));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));

        vector<string > keyName;
        keyName.push_back ("ifType");
        keyName.push_back ("ifName");
        setUserDefinedKeyCombination (keyName);
    }

    void  IgmpsMrtrVlanConfigManagedObject::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  IgmpsMrtrVlanConfigManagedObject::getIfType() const
    {
        return (m_ifType);
    }

    void  IgmpsMrtrVlanConfigManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsMrtrVlanConfigManagedObject::getIfName() const
    {
        return (m_ifName);
    }

}
