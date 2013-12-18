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
 *   Author : kghanta                                                     *
 **************************************************************************/
 #include "Framework/Attributes/AttributeEnum.h"
#include "AAA/Global/DataRuleManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"



namespace DcmNs
{

    DataRuleManagedObject::DataRuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (DataRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager)
    {
    }

    DataRuleManagedObject::DataRuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const UI32 &index, const string &nameSpace, const string &context, const string &keypath, const string &group, const RBACOperation &ops, const RBACAction &action)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (DataRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager),
        m_index    (index),
        m_namespace    (nameSpace),
        m_context    (context),
        m_keypath    (keypath),
        m_group    (group),
        m_ops    (ops),
        m_action    (action)
    {
    }

    DataRuleManagedObject::~DataRuleManagedObject ()
    {
    }

    string  DataRuleManagedObject::getClassName()
    {
        return ("DataRuleManagedObject");
    }

    void  DataRuleManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
	 }

	void DataRuleManagedObject::setupKeys()
	{
		vector<string> key;
		//key.push_back ("index");
		setUserDefinedKeyCombination (key);
	}

    void  DataRuleManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
	}

    void  DataRuleManagedObject::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  DataRuleManagedObject::getIndex() const
    {
        return (m_index);
    }

	void DataRuleManagedObject::setNamespace(const string &nameSpace) 
	{
		m_namespace = nameSpace;
	}

	string DataRuleManagedObject::getNamespace()  const
	{
		return (m_namespace);
	}

    void  DataRuleManagedObject::setContext(const string &context)
    {
        m_context  =  context;
    }

    string  DataRuleManagedObject::getContext() const
    {
        return (m_context);
    }

	void DataRuleManagedObject::setKeypath(const string &keypath)
	{
		m_keypath = keypath;
	}
	
	string DataRuleManagedObject::getKeypath()  const
	{
		return (m_keypath);
	}

    void DataRuleManagedObject::setGroup(const string &group)
	{
		m_group = group;
	}

    string DataRuleManagedObject::getGroup()  const
	{
		return (m_group);
	}

	void DataRuleManagedObject::setOps(const RBACOperation &ops) {
		m_ops = ops;
	}

 	RBACOperation DataRuleManagedObject::getOps() const{
		return (m_ops);
	}

    void  DataRuleManagedObject::setAction(const RBACAction &action)
    {
        m_action  =  action;
    }

    RBACAction  DataRuleManagedObject::getAction() const
    {
        return (m_action);
    }
}
