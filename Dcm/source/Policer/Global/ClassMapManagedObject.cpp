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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "Policer/Global/ClassMapManagedObject.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/PolicerGlobalTypes.h"
#include "vcs.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"

namespace DcmNs
{

    ClassMapManagedObject::ClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (ClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager)
    {
    	m_filterCriteria = FB_POLICY_RULE_MATCH_INVALID;
    }

    ClassMapManagedObject::ClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager,const string &classMapName, const UI32 &filterCriteria, const string &accessListName)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (ClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager),
        m_classMapName    (classMapName),
    	m_filterCriteria (filterCriteria),
    	m_accessListName (accessListName)
    {
    }

    ClassMapManagedObject::~ClassMapManagedObject ()
    {
    }

    string  ClassMapManagedObject::getClassName()
    {
        return ("ClassMapManagedObject");
    }

    void  ClassMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_classMapName,"classMapName"));
		setUserTagForAttribute("classMapName", policer_name);
		addPersistableAttribute  (new AttributeUI32(&m_filterCriteria,FB_POLICY_RULE_MATCH_INVALID, "filterCriteria"));
		addPersistableAttribute (new AttributeString(&m_accessListName,true,"","accessListName",policer_access_group_name));
    }

    void  ClassMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_classMapName,"classMapName",policer_name));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_filterCriteria, FB_POLICY_RULE_MATCH_INVALID, "filterCriteria"));
        addPersistableAttributeForCreate  (new AttributeString(&m_accessListName,"accessListName",policer_access_group_name));
		vector<string > keyName;
        keyName.push_back ("classMapName");
        setUserDefinedKeyCombination (keyName);

    }

    void  ClassMapManagedObject::setClassMapName(const string &classMapName)
    {
        m_classMapName  =  classMapName;
    }

    string  ClassMapManagedObject::getClassMapName() const
    {
        return (m_classMapName);
    }

    void ClassMapManagedObject::setFilterCriteria(const UI32 &filterCriteria) {
    	m_filterCriteria = filterCriteria;
    }

    UI32 ClassMapManagedObject::getFilterCriteria() const {
    	return m_filterCriteria;
    }

    void ClassMapManagedObject::setAccessListName(const string &accessListObjectName) {
    	m_accessListName = accessListObjectName;
    }

    string ClassMapManagedObject::getAccessListName() const {
    	return m_accessListName;
    }

}
