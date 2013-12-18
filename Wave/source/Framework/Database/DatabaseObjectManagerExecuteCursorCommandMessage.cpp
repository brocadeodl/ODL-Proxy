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
*   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.			*
*   All rights reserved.													*
*   Author : Sudhakar Jha									*
***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerExecuteCursorCommandMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"

namespace WaveNs
{

DatabaseObjectManagerExecuteCursorCfgCommandMessage::DatabaseObjectManagerExecuteCursorCfgCommandMessage (e_cursorOpCode opCode, string &cursorCommandString, const string &className)
	: PrismMessage          (DatabaseObjectManagerExecuteQueryMessage::getDatabaseObjectManagerServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_CURSOR_CFG_COMMAND),
      m_cursorCommandString	(cursorCommandString),
      m_commandType	        (opCode),
      m_className           (className)
{
}

DatabaseObjectManagerExecuteCursorCfgCommandMessage::~DatabaseObjectManagerExecuteCursorCfgCommandMessage ()
{

}

string DatabaseObjectManagerExecuteCursorCfgCommandMessage::getCursorCfgCommand () const
{
	return (m_cursorCommandString);
}

void   DatabaseObjectManagerExecuteCursorCfgCommandMessage::setCursorCfgCommand (string
&cursorCommandString)
{
	m_cursorCommandString = cursorCommandString;
}


string DatabaseObjectManagerExecuteCursorCfgCommandMessage::getClassName () const
{
    return (m_className);
}

void DatabaseObjectManagerExecuteCursorCfgCommandMessage::setClassName (const string &className)
{
    m_className = className;
}

DatabaseObjectManagerExecuteCursorQueryCommandMessage::DatabaseObjectManagerExecuteCursorQueryCommandMessage 
(string &queryString)
: PrismMessage (DatabaseObjectManagerExecuteQueryMessage::getDatabaseObjectManagerServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_CURSOR_QUERY_COMMAND)
{
	m_cursorQueryString = queryString;
}

DatabaseObjectManagerExecuteCursorQueryCommandMessage::~DatabaseObjectManagerExecuteCursorQueryCommandMessage ()
{

	
}



}
