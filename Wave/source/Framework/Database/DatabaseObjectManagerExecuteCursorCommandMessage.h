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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sudhakar Jha                                    *
 ***************************************************************************/

#ifndef DATABASEOBJECTMANAGEREXECUTECURSORCOMMANDMESSAGE_H
#define DATABASEOBJECTMANAGEREXECUTECURSORCOMMANDMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

enum e_cursorOpCode
{
	CURSOR_DECLARE,
	CURSOR_FETCH,
	CURSOR_MOVE,
	CURSOR_CLOSE
};


class DatabaseObjectManagerExecuteCursorCfgCommandMessage : public PrismMessage
{
    private :
    protected :
    public :
                        DatabaseObjectManagerExecuteCursorCfgCommandMessage (e_cursorOpCode opCode, string &cursorCommandString, const string &className);
        virtual        ~DatabaseObjectManagerExecuteCursorCfgCommandMessage ();

                string  getCursorCfgCommand                                 () const;
                void    setCursorCfgCommand                                 (string &cursorCommandString);

                string  getClassName                                        () const;
                void    setClassName                                        (const string &className);

    // Now the data members

    private :
        string         m_cursorCommandString;
	    e_cursorOpCode m_commandType;
        string         m_className;

    protected :
    public :
};


class DatabaseObjectManagerExecuteCursorQueryCommandMessage : public PrismMessage
{
	private :
	protected :
	public :
                 DatabaseObjectManagerExecuteCursorQueryCommandMessage (string &queryString);
		virtual ~DatabaseObjectManagerExecuteCursorQueryCommandMessage ();

	// Now the data members

	private :
		string m_cursorQueryString;

	protected :
	public :
};


}

#endif // DATABASEOBJECTMANAGEREXECUTECURSORCOMMANDMESSAGE_H

