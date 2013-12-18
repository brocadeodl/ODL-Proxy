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

#include "AAA/Global/AAAGlobalChangeRuleMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

    AAAGlobalChangeRuleMessage::AAAGlobalChangeRuleMessage ()
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGERULE)
    {
    }

    AAAGlobalChangeRuleMessage::AAAGlobalChangeRuleMessage (const UI32 &index, const vector<string> &command, const vector<string> &keypath, const string &context, const SI32 &commandEnum, const string &instanceCommand, const RBACCommandType &commandType, const string &role,const RBACOperation &operation,const RBACAction &action)
        : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGERULE),
        m_index    (index),
        m_command    (command),
        m_keypath    (keypath),
        m_context    (context),
        m_commandEnum    (commandEnum),
        m_instanceCommand    (instanceCommand),
        m_commandType    (commandType),
        m_role    (role),
        m_operation    (operation),
        m_action    (action)
    {
    }

    AAAGlobalChangeRuleMessage::~AAAGlobalChangeRuleMessage ()
    {
    }

    void  AAAGlobalChangeRuleMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_index,"index"));
        addSerializableAttribute (new AttributeStringVector(&m_command,"command"));
        addSerializableAttribute (new AttributeStringVector(&m_keypath,"keypath"));
        addSerializableAttribute (new AttributeString(&m_context,"context"));
        addSerializableAttribute (new AttributeSI32(&m_commandEnum,"commandEnum"));
        addSerializableAttribute (new AttributeString(&m_instanceCommand, "interface"));
        addSerializableAttribute (new AttributeUI32((UI32*)&m_commandType,"commandType"));
        addSerializableAttribute (new AttributeString(&m_role,"role"));
        addSerializableAttribute (new AttributeUI32((UI32*)&m_operation,"operation"));
        addSerializableAttribute (new AttributeUI32((UI32*)&m_action,"action"));
    }

    void  AAAGlobalChangeRuleMessage::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  AAAGlobalChangeRuleMessage::getIndex() const
    {
        return (m_index);
    }

	void AAAGlobalChangeRuleMessage::setKeypath(const vector<string> &keypath)
	{
        m_keypath  =  keypath;
	}

	vector<string> AAAGlobalChangeRuleMessage::getKeypath()  const
	{
			return (m_keypath);
	}

    void  AAAGlobalChangeRuleMessage::setCommand(const vector<string> &command)
    {
        m_command  =  command;
    }

    vector<string>  AAAGlobalChangeRuleMessage::getCommand() const
    {
			return (m_command);
	}

    void  AAAGlobalChangeRuleMessage::setContext(const string &context)
    {
        m_context  =  context;
    }

    string  AAAGlobalChangeRuleMessage::getContext() const
    {
        return (m_context);
    }

    void  AAAGlobalChangeRuleMessage::setCommandEnum(const SI32 &commandEnum)
    {
        m_commandEnum  =  commandEnum;
    }

    SI32  AAAGlobalChangeRuleMessage::getCommandEnum() const
    {
        return (m_commandEnum);
    }

    void  AAAGlobalChangeRuleMessage::setInstanceCommand(const string &instanceCommand)
    {
        m_instanceCommand  =  instanceCommand;
    }

    string  AAAGlobalChangeRuleMessage::getInstanceCommand() const
    {
        return (m_instanceCommand);
    }

    void  AAAGlobalChangeRuleMessage::setCommandType(const RBACCommandType &commandType)
    {
        m_commandType  =  commandType;
    }

    RBACCommandType  AAAGlobalChangeRuleMessage::getCommandType() const
    {
        return (m_commandType);
    }

    void  AAAGlobalChangeRuleMessage::setRole(const string &role)
    {
        m_role  =  role;
    }

    string  AAAGlobalChangeRuleMessage::getRole() const
    {
        return (m_role);
    }

    void  AAAGlobalChangeRuleMessage::setOperation(const RBACOperation &operation)
    {
        m_operation  =  operation;
    }

    RBACOperation  AAAGlobalChangeRuleMessage::getOperation() const
    {
        return (m_operation);
    }

    void  AAAGlobalChangeRuleMessage::setAction(const RBACAction &action)
    {
        m_action  =  action;
    }

    RBACAction  AAAGlobalChangeRuleMessage::getAction() const
    {
        return (m_action);
    }
}
