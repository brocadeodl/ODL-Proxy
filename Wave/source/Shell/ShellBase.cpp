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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Shell/ShellBase.h"
#include "Shell/ShellObjectManager.h"

namespace WaveNs
{

ShellBase::ShellBase (WaveObjectManager *pWaveObjectManager, string shellName)
          :WaveWorker (pWaveObjectManager), m_shellName (shellName)
{
}

ShellBase::~ShellBase ()
{
}

void ShellBase::addShellCommandHandler (const ShellCommandHandler &shellCommandHandler)
{
    m_shellCommandHandlers.push_back (shellCommandHandler);

    m_waveLineEditor.addValidCommand (shellCommandHandler.getAction ());
}

void ShellBase::getArguments (string *cmdString, vector<string> &argv, UI32 *argc)
{
    ULI        strLen;
    ULI        index       = 0;
    ULI        nextIndex   = 0;
    ULI        position    = 0;

   strLen  = cmdString->size();
   while ((index < strLen) && (string::npos != index))
    {
               // Skip any extra spaces.
        while (' ' == (*cmdString)[index])
        {
            index++;
        }

        if ((index < strLen) && (string::npos != index))
        {
            nextIndex      = cmdString->find (" ", index);
            argv.push_back (cmdString->substr(index, string::npos == nextIndex? nextIndex: nextIndex - index));
            position++;
            index = nextIndex;
        }
    }
    *argc   = position;
}

void ShellBase::shellExecuteHandler ()
{
    string                          cmdString;
    vector<string>                  argv;
    UI32                            argc;
    vector<ShellCommandHandler>     *shellCmd;
    UI32                            index;

    trace (TRACE_LEVEL_DEVEL, "ShellObjectManager::initialize : Entering ...");

    while(1)
    {
        cout << m_shellName << ">";

        if ((ShellObjectManager::getInstance ())->m_inputSource.is_open ())
        {
            getline((ShellObjectManager::getInstance ())->m_inputSource, cmdString);
            if ((ShellObjectManager::getInstance ())->m_inputSource.eof ())
            {
                (ShellObjectManager::getInstance ())->m_inputSource.close ();
                //getline(cin, cmdString);
                cmdString = m_waveLineEditor.getUserInputLine (m_shellName);
            }
        }
        else
        {
            //getline (cin, cmdString);
            cmdString = m_waveLineEditor.getUserInputLine (m_shellName);

            if (true == (cin.eof ()))
            {
                cout << endl;
                exit (0);
            }
        }

        shellCmd = &m_shellCommandHandlers;

        argv.clear ();

        getArguments (&cmdString, argv, &argc);

        if(0 == argc)
        {
            continue;
        }

        for (index = 0; index < (*shellCmd).size(); index++)
        {
            if(argv[0].length () >= (*shellCmd)[index].getMinCmdLength ())
            {
                string currCmd    =  (*shellCmd)[index].getAction ();
                string partialCmd =  currCmd.substr (0, argv[0].length () );
                if (partialCmd == argv[0])
                {
                    if ((*shellCmd)[0].getMinArgc () > argc)
                    {
                        if (NULL != (*shellCmd)[index].getUsageFunction ())
                        {
                            (*((*shellCmd)[index].getUsageFunction () )) ();
                        }
                    }
                    else
                    {
                        if (NULL != (*shellCmd)[index].getCmdFunction ())
                        {
                            if ( (*((*shellCmd)[index].getCmdFunction () )) (argc, argv) == SHELL_EXIT)
                            {
                                return;
                            }
                        }
                    }
                break;
                }
            }
        }
        if ((*shellCmd).size() == index)
        {
            cout << "Bad Command" << endl;
        }
    }
}


ShellCommandHandler::ShellCommandHandler (string action, UI32 minArgc, ShellCmdFunction cmdFunction, UI32 minCmdLength, ShellUsageFunction usageFunction)
                    :m_action (action), m_minArgc (minArgc), m_cmdFunction (cmdFunction), m_minCmdLength (minCmdLength), m_usageFunction (usageFunction)
{

}

ShellCommandHandler::ShellCommandHandler ()
    :m_action ("END"), m_minArgc (1), m_cmdFunction (NULL), m_minCmdLength (1), m_usageFunction (NULL)
{

}

ShellCommandHandler::~ShellCommandHandler ()
{
}

string ShellCommandHandler::getAction () const
{
    return m_action;
}

UI32 ShellCommandHandler::getMinArgc () const
{
    return m_minArgc;
}

ShellCmdFunction ShellCommandHandler::getCmdFunction () const
{
    return m_cmdFunction;
}

UI32 ShellCommandHandler::getMinCmdLength () const
{
    return m_minCmdLength;
}

ShellUsageFunction ShellCommandHandler::getUsageFunction () const
{
    return m_usageFunction;
}


ShellCommandHandler &ShellCommandHandler::operator = (const ShellCommandHandler &rhs)
{
    m_action        =   rhs.m_action;
    m_minArgc       =   rhs.m_minArgc;
    m_cmdFunction   =   rhs.m_cmdFunction;
    m_minCmdLength  =   rhs.m_minCmdLength;
    m_usageFunction =   rhs.m_usageFunction;

    return *this;
}
}
