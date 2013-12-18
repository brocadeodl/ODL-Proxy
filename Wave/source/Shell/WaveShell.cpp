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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Shell/WaveShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

WaveShell::WaveShell (const string &shellName)
    : m_shellName (shellName)
{
    addCommandfunction ("help", reinterpret_cast<WaveShellCommandFunction> (&WaveShell::overAllHelp), "Prints the over all help for this shell.");
}

WaveShell::~WaveShell ()
{
}

string WaveShell::getShellName () const
{
    return (m_shellName);
}

bool WaveShell::isAKnownCommand (const string &command)
{
    map<string, WaveShell *>::iterator              subShellElement    = m_subShells.find (command);
    map<string, WaveShell *>::iterator              subShellEndElement = m_subShells.end ();
    map<string, WaveShellCommandFunction>::iterator commandElement     = m_commandFunctions.find (command);
    map<string, WaveShellCommandFunction>::iterator commandEndElement  = m_commandFunctions.end ();

    if ((subShellEndElement != subShellElement) || (commandEndElement != commandElement))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void WaveShell::addCommandfunction (const string &command, WaveShellCommandFunction commandFunction, const string &commandBriefHelp, WaveShellCommandHelpFunction commandHelpFunction, WaveShell *pCommandFunctionOwner)
{
    bool knownCommand = isAKnownCommand (command);

    if (NULL == pCommandFunctionOwner)
    {
        pCommandFunctionOwner = this;
    }

    if (false == knownCommand)
    {
        m_commandFunctions[command]                 = commandFunction;
        m_commandFunctionBriefDescriptions[command] = commandBriefHelp;
        m_commandHelpFunctions[command]             = commandHelpFunction;
        m_commandFunctionOwners[command]            = pCommandFunctionOwner;

        m_waveLineEditor.addValidCommand (command);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveShell::addSubShell (WaveShell *pSubshell)
{
    bool knownCommand = isAKnownCommand (pSubshell->getShellName ());

    if (false == knownCommand)
    {
        m_subShells[pSubshell->getShellName ()] = pSubshell;

        m_waveLineEditor.addValidCommand (pSubshell->getShellName ());
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveShell::execute (const vector<string> &arguments)
{
           UI32           numberOfArguments     = arguments.size ();
           string         inputCommand;
           vector<string> inputArguments;
           UI32           numberOfInputCommands = 0;
    static bool           manualInput           = false;

    while (true)
    {
        if (0 == numberOfArguments)
        {
            inputCommand = m_waveLineEditor.getUserInputLine (getShellName ());

            manualInput = true;

            tokenize (inputCommand, inputArguments);
        }
        else
        {
            inputArguments = arguments;
        }

        numberOfInputCommands = inputArguments.size ();

        if (cin.eof ())
        {
            cout << endl;
            exit (EXIT_SUCCESS);
        }

        if (0 != numberOfInputCommands)
        {
            if ("quit" == inputArguments[0])
            {
                return;
            }

            runCommand (inputArguments, !(manualInput));
            numberOfArguments = 0;
        }
    }
}

void WaveShell::runCommand (const vector<string> &arguments, const bool &exitWhenDone)
{
    string                                           command               = arguments[0];
    map<string, WaveShell *>::iterator               subShellElement       = m_subShells.find (command);
    map<string, WaveShell *>::iterator               subShellEndElement    = m_subShells.end ();
    map<string, WaveShellCommandFunction>::iterator  commandElement        = m_commandFunctions.find (command);
    map<string, WaveShellCommandFunction>::iterator  commandEndElement     = m_commandFunctions.end ();
    vector<string>                                   arguments1            = arguments;
    ResourceId                                       status                = WAVE_MESSAGE_SUCCESS;
    WaveShellCommandFunction                         commandFunction       = NULL;
    WaveShell                                       *pCommandFunctionOwner = NULL;

    arguments1.erase (arguments1.begin ());

    if (subShellEndElement != subShellElement)
    {
        (subShellElement->second)->execute (arguments1);
    }
    else if (commandEndElement != commandElement)
    {
        commandFunction       = commandElement->second;
        pCommandFunctionOwner = m_commandFunctionOwners[command];

        status = (pCommandFunctionOwner->*commandFunction) (arguments1);

        if (true == exitWhenDone)
        {
            if (WAVE_MESSAGE_SUCCESS == status)
            {
                exit (EXIT_SUCCESS);
            }
            else
            {
                exit (EXIT_FAILURE);
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, command + " : Unknown Command.");
    }
}

void WaveShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        A Command Shell For Wave Based Application.");
}

void WaveShell::overAllHelp (const vector<string> &arguments)
{
    if (1 >  arguments.size ())
    {
        map<string, WaveShell *>::iterator              subShellElement    = m_subShells.begin ();
        map<string, WaveShell *>::iterator              subShellEndElement = m_subShells.end ();
        map<string, WaveShellCommandFunction>::iterator commandElement     = m_commandFunctions.begin ();
        map<string, WaveShellCommandFunction>::iterator commandEndElement  = m_commandFunctions.end ();

        while (subShellEndElement != subShellElement)
        {
            tracePrintf (TRACE_LEVEL_SUCCESS, true, true, "   %s", (subShellElement->first).c_str ());

            (subShellElement->second)->briefHelp ();

            tracePrintf (TRACE_LEVEL_INFO, true, true, "");

            subShellElement++;
        }

        while (commandEndElement != commandElement)
        {
            tracePrintf (TRACE_LEVEL_SUCCESS, false, true, "   %s", (commandElement->first).c_str ());
            tracePrintf (TRACE_LEVEL_INFO,    true,  true, " - %s", (m_commandFunctionBriefDescriptions[commandElement->first]).c_str ());
            commandElement++;
        }
    }
    else
    {
        help (arguments[0]);
    }
}

void WaveShell::help (const string &command)
{
    map<string, WaveShell *>::iterator               subShellElement       = m_subShells.find (command);
    map<string, WaveShell *>::iterator               subShellEndElement    = m_subShells.end ();
    map<string, WaveShellCommandFunction>::iterator  commandElement        = m_commandFunctions.find (command);
    map<string, WaveShellCommandFunction>::iterator  commandEndElement     = m_commandFunctions.end ();
    WaveShellCommandHelpFunction                     commandHelpFunction   = NULL;
    WaveShell                                       *pCommandFunctionOwner = NULL;

    if (subShellEndElement != subShellElement)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, true, "Help For ");
        tracePrintf (TRACE_LEVEL_SUCCESS, false, true, "%s", command.c_str ());
        tracePrintf (TRACE_LEVEL_INFO, true, true, " Shell : ");

        (subShellElement->second)->overAllHelp (vector<string> ());
    }
    else if (commandEndElement != commandElement)
    {
        tracePrintf (TRACE_LEVEL_INFO,    false, true, "Help For ");
        tracePrintf (TRACE_LEVEL_SUCCESS, false, true, "%s", command.c_str ());
        tracePrintf (TRACE_LEVEL_INFO,    true,  true, " :");

        commandHelpFunction   = m_commandHelpFunctions[command];
        pCommandFunctionOwner = m_commandFunctionOwners[command];

        if (NULL != commandHelpFunction)
        {
            (pCommandFunctionOwner->*commandHelpFunction) ();
        }
    }else
    {
        trace (TRACE_LEVEL_ERROR, command + " : Unknown Command.");
    }
}

}
