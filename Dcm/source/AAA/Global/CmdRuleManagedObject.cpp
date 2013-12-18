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
#include "AAA/Global/CmdRuleManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"


namespace DcmNs
{

    CmdRuleManagedObject::CmdRuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (CmdRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager)
    {
    }

    CmdRuleManagedObject::CmdRuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const UI32 &index,const string &context,const string &command, const string &group, const RBACOperation &ops, const RBACAction &action)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (CmdRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager),
        m_index    (index),
        m_context    (context),
        m_command    (command),
        m_group    (group),
        m_ops    (ops),
        m_action    (action)
    {
    }

    CmdRuleManagedObject::~CmdRuleManagedObject ()
    {
    }

    string  CmdRuleManagedObject::getClassName()
    {
        return ("CmdRuleManagedObject");
    }

    void  CmdRuleManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
	 }

	void CmdRuleManagedObject::setupKeys()
	{
		vector<string> key;
		//key.push_back ("index1");
		setUserDefinedKeyCombination (key);
	}

    void  CmdRuleManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
	}

    void  CmdRuleManagedObject::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  CmdRuleManagedObject::getIndex() const
    {
        return (m_index);
    }

    void  CmdRuleManagedObject::setContext(const string &context)
    {
        m_context  =  context;
    }

    string  CmdRuleManagedObject::getContext() const
    {
        return (m_context);
    }

    void  CmdRuleManagedObject::setCommand(const string &command)
    {
        m_command  =  command;
    }

    string  CmdRuleManagedObject::getCommand() const
    {
        return (m_command);
    }

    void CmdRuleManagedObject::setGroup(const string &group) {
		m_group = group;
	}

    string CmdRuleManagedObject::getGroup()  const {
		return (m_group);
	}

	void CmdRuleManagedObject::setOps(const RBACOperation &ops) {
		m_ops = ops;
	}

 	RBACOperation CmdRuleManagedObject::getOps() const{
		return (m_ops);
	}

    void  CmdRuleManagedObject::setAction(const RBACAction &action)
    {
        m_action  =  action;
    }

    RBACAction  CmdRuleManagedObject::getAction() const
    {
        return (m_action);
    }
}
