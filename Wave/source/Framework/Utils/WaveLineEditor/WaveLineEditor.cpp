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

#include "Framework/Utils/WaveLineEditor/WaveLineEditor.h"

#include <sys/ioctl.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace std;

namespace WaveNs
{

WaveLineEditor::WaveLineEditor ()
{
}

WaveLineEditor::WaveLineEditor (const vector<string> &validCommands)
    : m_validCommands (validCommands)
{
}

WaveLineEditor::~WaveLineEditor ()
{
}

UI32 WaveLineEditor::getNumberOfCharactersAvailableToRead ()
{
    unsigned int numberOfCharactersAvailableToRead = 0;

    usleep (10000);
    ioctl(0, FIONREAD, &numberOfCharactersAvailableToRead);

    return numberOfCharactersAvailableToRead;
}

string WaveLineEditor::getUserInputLine (const string &linePrompt)
{
    UI32           i                                 = 0;
    UI32           j                                 = 0;
    SI32           c                                 ='\0';
    SI32           c1                                ='\0';
    SI32           c2                                ='\0';
    SI32           c3                                ='\0';
    UI32           numberOfCharactersAvailableToRead = 0;
    string         command;
    string         currentlyTypingCommand;
    UI32           globalIndex                       = m_commandHistory.size ();
    UI32           currentCommandSize                = 0;
    UI32           currentPosition                   = command.size ();
    UI32           commandPromptSize                 = linePrompt.size ();
    vector<string> matchingCommands;

    system("stty raw -echo isig");

    printf ("%c%s>", 13, linePrompt.c_str ());
    fflush (stdout);

    while (1)
    {
        if (0 != (numberOfCharactersAvailableToRead = (getNumberOfCharactersAvailableToRead ())))
        {
            if (3 == numberOfCharactersAvailableToRead)
            {
                c1 = getchar ();
                c2 = getchar ();
                c3 = getchar ();

                if ((27 == c1) && (91 == c2) && (65 == c3))
                {
                    if (0 < globalIndex)
                    {
                        currentCommandSize = command.size ();

                        printf ("%c", 13);

                        for (i = 0; i < (currentCommandSize + commandPromptSize + 1); i++)
                        {
                            printf (" ");
                        }

                        globalIndex--;
                        command = m_commandHistory[globalIndex];
                        currentPosition = command.size ();
                        printf ("%c%s>%s", 13, linePrompt.c_str (), command.c_str ());
                        fflush (stdout);
                    }
                }

                if ((27 == c1) && (91 == c2) && (66 == c3))
                {
                    if (globalIndex < (m_commandHistory.size ()))
                    {
                        currentCommandSize = command.size ();

                        printf ("%c", 13);

                        for (i = 0; i < (currentCommandSize + commandPromptSize + 1); i++)
                        {
                            printf (" ");
                        }

                        if (globalIndex < ((m_commandHistory.size ()) - 1))
                        {
                            globalIndex++;
                            command = m_commandHistory[globalIndex];
                        }
                        else
                        {
                            globalIndex++;
                            command = currentlyTypingCommand;
                        }

                        currentPosition = command.size ();

                        printf ("%c%s>%s", 13, linePrompt.c_str (), command.c_str ());
                        fflush (stdout);
                    }
                }

                if ((27 == c1) && (91 == c2) && (67 == c3))
                {
                    if (currentPosition < (command.size ()))
                    {
                        currentPosition++;
                        printf ("%c", command[currentPosition - 1]);
                        fflush (stdout);
                    }
                }

                if ((27 == c1) && (91 == c2) && (68 == c3))
                {
                    if (0 < currentPosition)
                    {
                        currentPosition--;
                        printf ("\b");
                        fflush (stdout);
                    }
                }

                continue;
            }

            for (i = 0; i < numberOfCharactersAvailableToRead; i++)
            {
                c = getchar();

                if (4 == c)
                {
                    system("stty cooked echo");
                    exit (0);
                }

                if ((0 != (isalnum (c))) || (' ' == c) || ('_' == c) || ('-' == c) || (':' == c) || ('.' == c) || ('=' == c) || ('/' == c) || ('<' == c) || ('>' == c) || ('{' == c) || ('}' == c) || ('[' == c) || (']' == c) || ('"' == c) || (',' == c) || ('#' == c) || ('?' == c) || ('@' == c))
                {
                    command.insert (currentPosition, 1, c);
                    currentPosition++;

                    printf("%c%s>%s", 13, linePrompt.c_str (), command.c_str ());

                    for (j = 0; j < ((command.size ()) - currentPosition); j++)
                    {
                        printf ("\b");
                    }

                    fflush (stdout);
                }
                else if ('\t' == c)
                {
                    if ("" == command)
                    {
                        for (j = 0; j < m_validCommands.size (); j++)
                        {
                            printf ("\n\r  %s", (m_validCommands[j]).c_str ());
                        }

                        printf ("\n\r");
                    }
                    else
                    {
                        matchingCommands.clear ();

                        for (j = 0; j < m_validCommands.size (); j++)
                        {
                            if (0 == (m_validCommands[j].find (command)))
                            {
                                matchingCommands.push_back (m_validCommands[j]);
                            }
                        }

                        if (1 == (matchingCommands.size ()))
                        {
                            command = matchingCommands[0];

                            currentPosition = command.size ();
                        }
                        else
                        {
                            for (j = 0; j < matchingCommands.size (); j++)
                            {
                                printf ("\n\r  %s", (matchingCommands[j]).c_str ());
                            }

                            printf ("\n\r");
                        }
                    }

                    printf("%c%s>%s", 13, linePrompt.c_str (), command.c_str ());

                    for (j = 0; j < ((command.size ()) - currentPosition); j++)
                    {
                        printf ("\b");
                    }

                    fflush (stdout);
                }
                else if ('?' == c)
                {
                    if ("" == command)
                    {
                        for (j = 0; j < m_validCommands.size (); j++)
                        {
                            printf ("\n\r  %s", (m_validCommands[j]).c_str ());
                        }

                        printf ("\n\r");
                    }
                    else
                    {
                        matchingCommands.clear ();

                        for (j = 0; j < m_validCommands.size (); j++)
                        {
                            if (0 == (m_validCommands[j].find (command)))
                            {
                                matchingCommands.push_back (m_validCommands[j]);
                            }
                        }

                        for (j = 0; j < matchingCommands.size (); j++)
                        {
                            printf ("\n\r  %s", (matchingCommands[j]).c_str ());
                        }

                        printf ("\n\r");
                    }

                    printf("%c%s>%s", 13, linePrompt.c_str (), command.c_str ());

                    for (j = 0; j < ((command.size ()) - currentPosition); j++)
                    {
                        printf ("\b");
                    }

                    fflush (stdout);
                }
                else
                {
                    if ((8 == c) || (127 == c))
                    {
                        if (0 < currentPosition)
                        {
                            printf ("%c", 13);

                            for (j = 0; j < (command.size () + commandPromptSize + 1); j++)
                            {
                                printf (" ");
                            }

                            command = command.erase (currentPosition - 1, 1);

                            currentPosition--;

                            printf("%c%s>%s", 13, linePrompt.c_str (), command.c_str ());

                            for (j = 0; j < ((command.size ()) - currentPosition); j++)
                            {
                                printf ("\b");
                            }

                            fflush (stdout);
                        }
                    }
                }
            }

            if (c == 13)
            {
                printf ("\n\r");
                fflush (stdout);

                if (0 != (command.size ()))
                {
                    m_commandHistory.push_back (command);
                }

                system("stty cooked echo");
                return (command);
            }

            currentlyTypingCommand = command;
        }
    }

    system("stty cooked echo");

    return (string (""));
}

void WaveLineEditor::addValidCommand (const string &validCommand)
{
    m_validCommands.push_back (validCommand);
}

}
