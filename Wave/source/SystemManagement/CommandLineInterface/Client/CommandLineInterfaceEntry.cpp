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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SystemManagement/CommandLineInterface/Client/CommandLineInterfaceEntry.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "SystemManagement/CommandLineInterface/Client/WaveCommandLineInterface.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Modeling/YANG/ObjectModel/YangList.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include <sys/ioctl.h>
#include <cstdio>
#include <algorithm>

namespace WaveNs
{

CommandLineInterfaceEntry::CommandLineInterfaceEntry ()
    : m_commandHistoryCurrentIndex        (1),
      m_isKeyProcessingRequired           (false),
      m_pParentCommandLinteInterfaceEntry (NULL)
{
}

CommandLineInterfaceEntry::CommandLineInterfaceEntry (const string &name)
    : m_name                              (name),
      m_commandHistoryCurrentIndex        (1),
      m_isKeyProcessingRequired           (false),
      m_pParentCommandLinteInterfaceEntry (NULL),
      m_cliTargetNodeName                 (name)
{
}

CommandLineInterfaceEntry::~CommandLineInterfaceEntry ()
{
    UI32 numberOfChildEntries = m_childEntries.size ();
    UI32 i                    = 0;

    for (i = 0; i < numberOfChildEntries; i++)
    {
        delete m_childEntries[i];
    }
}

string CommandLineInterfaceEntry::getName () const
{
    return (m_name);
}

bool CommandLineInterfaceEntry::isAKnownChildEntry (const string &name) const
{
    map<string, CommandLineInterfaceEntry *>::const_iterator element    = m_childEntriesMap.find (name);
    map<string, CommandLineInterfaceEntry *>::const_iterator endElement = m_childEntriesMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void CommandLineInterfaceEntry::addChildEntryIfDoesNotExist (CommandLineInterfaceEntry *pChildCommandLineinterfaceEntry)
{
    prismAssert (NULL != pChildCommandLineinterfaceEntry, __FILE__, __LINE__);

    const string childEntryName = pChildCommandLineinterfaceEntry->getName ();
    const bool   isKnown        = isAKnownChildEntry (childEntryName);

    if (false == isKnown)
    {
        m_childEntries.push_back (pChildCommandLineinterfaceEntry);
        m_childEntriesMap[childEntryName] = pChildCommandLineinterfaceEntry;

        pChildCommandLineinterfaceEntry->setPParentCommandLineInterfaceEntry (this);

        // check if the the element is of type leaf and it does not contain the anotation 
        addTovalidCommands (pChildCommandLineinterfaceEntry , childEntryName);      
    }
}

void CommandLineInterfaceEntry::addChildEntryIfDoesNotExist (const string &childCommandLineInterfaceEntryName)
{
    const bool isKnown = isAKnownChildEntry (childCommandLineInterfaceEntryName);

    if (false == isKnown)
    {
        CommandLineInterfaceEntry *pChildCommandLineinterfaceEntry = new CommandLineInterfaceEntry (childCommandLineInterfaceEntryName);

        prismAssert (NULL != pChildCommandLineinterfaceEntry, __FILE__, __LINE__);

        

        m_childEntries.push_back (pChildCommandLineinterfaceEntry);
        m_childEntriesMap[childCommandLineInterfaceEntryName] = pChildCommandLineinterfaceEntry;

        pChildCommandLineinterfaceEntry->setPParentCommandLineInterfaceEntry (this);
        
        // check if the the element is of type leaf and it does not contain the anotation 
        addTovalidCommands ( pChildCommandLineinterfaceEntry , childCommandLineInterfaceEntryName);      
    }
}


void CommandLineInterfaceEntry::addTovalidCommands (CommandLineInterfaceEntry *pChildCommandLineinterfaceEntry , const string &childCommandLineInterfaceEntryName)
{

   bool addToValidCommands = true;

   vector<YangElement *> pYangElementVector = pChildCommandLineinterfaceEntry->getYangElements();

   for(UI32 i = 0 ; i < pYangElementVector.size() ; i++)
   {   

       YangElement * pYangElement = pYangElementVector[i];

       YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (pYangElement) ;

       if ( NULL != pYangLeaf && (pYangLeaf->getCliSuppressShowMatch() || pYangLeaf->getCliSuppressShowConfPath() ))
       {
           addToValidCommands = false;
           break;
       }
   }

   if(addToValidCommands)
   {   
       m_validCommands.push_back (childCommandLineInterfaceEntryName);
   }
}

void CommandLineInterfaceEntry::addCommandLine (const string &commandLine)
{
    vector<string> commandLineTokens;

    StringUtils::tokenize (commandLine, commandLineTokens, ' ');

    UI32                        numberOfCommandLineTokens  = commandLineTokens.size ();
    UI32                        i                          = 0;
    CommandLineInterfaceEntry * pCommandLineInterfaceEntry = this;

    for (i = 0; i < numberOfCommandLineTokens; i++)
    {
        string commandLineToken = commandLineTokens[i];

        pCommandLineInterfaceEntry->addChildEntryIfDoesNotExist (commandLineToken);

        pCommandLineInterfaceEntry = pCommandLineInterfaceEntry->m_childEntriesMap[commandLineToken];

        prismAssert (NULL != pCommandLineInterfaceEntry, __FILE__, __LINE__);
    }
}

void CommandLineInterfaceEntry::addCommandLines (const vector<string> &commandLines)
{
    UI32 numberOfCommandLines = commandLines.size ();
    UI32 i                    = 0;

    for (i = 0; i < numberOfCommandLines; i++)
    {
        addCommandLine (commandLines[i]);
    }
}

CommandLineInterfaceEntry *CommandLineInterfaceEntry::getCommandLineInterfaceEntry (const string &commandLine , vector<string> &keyVector)
{
    if ("" == commandLine)
    {
        return (this);
    }

    vector<string> commandLineTokens;

    StringUtils::tokenize (commandLine, commandLineTokens, ' ');

    UI32                       numberOfCommandLineTokens     = commandLineTokens.size ();
    UI32                       i                             = 0;
    CommandLineInterfaceEntry *pCommandLineInterfaceEntry    = this;
    bool                       isKeyProcessingRequired       = false;
    UI32                       numberOfKeyCombinationEntries = 0;
    vector<string>             keyCombination;

    for (i = 0; i < numberOfCommandLineTokens; i++)
    {
        string commandLineToken = commandLineTokens[i];

        if (true == (pCommandLineInterfaceEntry->isAKnownChildEntry (commandLineToken)))
        {
            pCommandLineInterfaceEntry = pCommandLineInterfaceEntry->m_childEntriesMap[commandLineToken];

            prismAssert (NULL != pCommandLineInterfaceEntry, __FILE__, __LINE__);

            isKeyProcessingRequired = pCommandLineInterfaceEntry->getIsKeyProcessingRequired ();

            if (true == isKeyProcessingRequired)
            {
                keyCombination = pCommandLineInterfaceEntry->getKeyCombination ();

                numberOfKeyCombinationEntries = keyCombination.size ();

                // This is to identify how may keys are there and put them in keyVector 
                // This logic does not handle multiple key support as of now 
                // TODO:: Add logic to handle multiple key support 
                while ((numberOfCommandLineTokens - i) > numberOfKeyCombinationEntries && numberOfKeyCombinationEntries > 0)
                {
                    i++;
                    numberOfKeyCombinationEntries--;
                    keyVector.push_back(commandLineTokens[i]);
                }

                continue;
            }
        }
        else
        {
            tracePrintf(TRACE_LEVEL_INFO ," CommandLineInterfaceEntry::getCommandLineInterfaceEntry  returning NULL for unknown command [ %s ] ",commandLineToken.c_str() );
            return (NULL);
        }
    }

    return (pCommandLineInterfaceEntry);
}


void CommandLineInterfaceEntry::getInputDetailsForCommandLine (const string &commandLine, bool &areKeysCompletelyEntered, UI32 &numberOfKeysEntered)
{
    if ("" == commandLine)
    {
        areKeysCompletelyEntered = true;
        numberOfKeysEntered      = 0;
    }

    vector<string> commandLineTokens;

    StringUtils::tokenize (commandLine, commandLineTokens, ' ');

    UI32                       numberOfCommandLineTokens     = commandLineTokens.size ();
    UI32                       i                             = 0;
    CommandLineInterfaceEntry *pCommandLineInterfaceEntry    = WaveCommandLineInterface::getPCurrentModeCommandLineInterfaceEntry ();
    bool                       isKeyProcessingRequired       = false;
    UI32                       numberOfKeyCombinationEntries = 0;
    vector<string>             keyCombination;
    UI32                       previousI;

    for (i = 0; i < numberOfCommandLineTokens; i++)
    {
        string commandLineToken = commandLineTokens[i];

        if (true == (pCommandLineInterfaceEntry->isAKnownChildEntry (commandLineToken)))
        {
            pCommandLineInterfaceEntry = pCommandLineInterfaceEntry->m_childEntriesMap[commandLineToken];

            prismAssert (NULL != pCommandLineInterfaceEntry, __FILE__, __LINE__);

            isKeyProcessingRequired = pCommandLineInterfaceEntry->getIsKeyProcessingRequired ();

            if (true == isKeyProcessingRequired)
            {
                keyCombination = pCommandLineInterfaceEntry->getKeyCombination ();

                numberOfKeyCombinationEntries = keyCombination.size ();

                previousI  = i;
                i         += numberOfKeyCombinationEntries;

                if (i >= numberOfCommandLineTokens)
                {
                    areKeysCompletelyEntered = false;
                    numberOfKeysEntered      = numberOfCommandLineTokens - previousI - 1;
                }
                else
                {
                    areKeysCompletelyEntered = true;
                    numberOfKeysEntered      = numberOfKeyCombinationEntries;
                }
            }
        }
    }
}

void CommandLineInterfaceEntry::removeKeysFromCommandLine (string &commandLine)
{
    vector<string> commandLineTokens;
    tracePrintf(TRACE_LEVEL_INFO,"CommandLineInterfaceEntry::removeKeysFromCommandLine old  commnad line  is  :: [ %s ] ", commandLine.c_str());

    StringUtils::tokenize (commandLine, commandLineTokens, ' ');

    UI32                       numberOfCommandLineTokens     = commandLineTokens.size ();
    UI32                       i                             = 0;
    CommandLineInterfaceEntry *pCommandLineInterfaceEntry    = WaveCommandLineInterface::getPCurrentModeCommandLineInterfaceEntry ();
    bool                       isKeyProcessingRequired       = false;
    UI32                       numberOfKeyCombinationEntries = 0;
    vector<string>             keyCombination;
    string                     commandLineWithoutKeys;

    for (i = 0; i < numberOfCommandLineTokens; i++)
    {
        string commandLineToken = commandLineTokens[i];

        tracePrintf(TRACE_LEVEL_DEBUG," CommandLineInterfaceEntry::removeKeysFromCommandLine  the commnad line  tokes are :: [ %s ]", commandLineToken.c_str());

        if (true == (pCommandLineInterfaceEntry->isAKnownChildEntry (commandLineToken)))
        {
            pCommandLineInterfaceEntry = pCommandLineInterfaceEntry->m_childEntriesMap[commandLineToken];

            prismAssert (NULL != pCommandLineInterfaceEntry, __FILE__, __LINE__);

            isKeyProcessingRequired = pCommandLineInterfaceEntry->getIsKeyProcessingRequired ();

            if (true == isKeyProcessingRequired)
            {
                keyCombination = pCommandLineInterfaceEntry->getKeyCombination ();

                numberOfKeyCombinationEntries = keyCombination.size ();
            
                i += numberOfKeyCombinationEntries;

                if (i >= numberOfCommandLineTokens)
                {
                    break;
                }
            }
        }

        if ("" != commandLineWithoutKeys)
        {
            commandLineWithoutKeys += " " + commandLineToken;
        }
        else
        {
            commandLineWithoutKeys += commandLineToken;
        }
        
    }

    commandLine = commandLineWithoutKeys;
}


bool CommandLineInterfaceEntry::isAFullCommand () const
{
    return (false);
}

string CommandLineInterfaceEntry::getModePrompt () const
{
    return (m_modePrompt);
}

void CommandLineInterfaceEntry::setModePrompt (const string &modePrompt)
{
    m_modePrompt = modePrompt;
}

UI32 CommandLineInterfaceEntry::getNumberOfCharactersAvailableToRead ()
{
    unsigned int numberOfCharactersAvailableToRead = 0;

    usleep (10000);
    ioctl(0, FIONREAD, &numberOfCharactersAvailableToRead);

    return numberOfCharactersAvailableToRead;
}

void CommandLineInterfaceEntry::printCommandLinePrompt (const string &prompt, const string &commandFromPreviousLevel, const string &command) const
{
    if (("" != commandFromPreviousLevel) || ("" != command))
    {
        if ("" != commandFromPreviousLevel)
        {
            printf ("%c%s# %s %s", 13, prompt.c_str (), commandFromPreviousLevel.c_str (), command.c_str ());
        }
        else
        {
            printf ("%c%s# %s", 13, prompt.c_str (), command.c_str ());
        }
    }
    else
    {
        printf ("%c%s# ", 13, prompt.c_str ());
    }
}

void CommandLineInterfaceEntry::acceptcommand (const string &prompt, string &commandFromPreviousLevel, string &command, WaveCommandLineInterface *pWaveCommandLineInterface)
{
    UI32                      i                                 = 0;
    UI32                      j                                 = 0;
    SI32                      c                                 ='\0';
    SI32                      c1                                ='\0';
    SI32                      c2                                ='\0';
    SI32                      c3                                ='\0';
    UI32                      numberOfCharactersAvailableToRead = 0;
    UI32                      currentCommandSize                = 0;
    UI32                      currentPosition                   = command.size ();
    UI32                      currentPositionForPreviousLevel   = commandFromPreviousLevel.size () + 1;
    UI32                      commandPromptSize                 = prompt.size () + commandFromPreviousLevel.size () + 1;
    vector<string>            matchingCommands;
    string                    currentMatchAtThisLevel;

    prismAssert (NULL != pWaveCommandLineInterface, __FILE__, __LINE__);

    vector<string> &commandHistory         = pWaveCommandLineInterface->getCommandHistory ();
    UI32           &globalIndex            = pWaveCommandLineInterface->getCommandHistoryCurrentIndex ();
    string         &currentlyTypingCommand = pWaveCommandLineInterface->getCurrentlyTypingCommand ();

    printCommandLinePrompt (prompt, commandFromPreviousLevel, command);

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

                if ((27 == c1) && (91 == c2) && (65 == c3)) // UP Arrow
                {
                    if (0 < globalIndex)
                    {
                        currentCommandSize = command.size ();

                        printf ("%c", 13);

                        for (i = 0; i < (currentCommandSize + commandPromptSize + 2); i++)
                        {
                            printf (" ");
                        }

                        globalIndex--;
                        command = commandHistory[globalIndex];
                        currentPosition = command.size ();
                        //printCommandLinePrompt (prompt, commandFromPreviousLevel, command);
                        //printCommandLinePrompt (prompt, command, "");
                        //fflush (stdout);

                        string firstPart;
                        string secondPart;

                        StringUtils::splitBasedOnLastOccuranceOf (command, ' ', firstPart, secondPart);

                        commandFromPreviousLevel = firstPart;
                        command                  = secondPart;

                        if ("" == command)
                        {
                            command                  = commandFromPreviousLevel;
                            commandFromPreviousLevel = "";
                        }

                        CommandLineInterfaceEntry *pCommandLineInterfaceEntry = pWaveCommandLineInterface->getCommandLineInterfaceEntryForShowRunningConfiguration (commandFromPreviousLevel);

                        if (NULL == pCommandLineInterfaceEntry)
                        {
                            pCommandLineInterfaceEntry = this;
                        }

                        pCommandLineInterfaceEntry->acceptcommand (prompt, commandFromPreviousLevel, command, pWaveCommandLineInterface);

                        return;
                    }
                }

                if ((27 == c1) && (91 == c2) && (66 == c3)) // DOWN Arrow
                {
                    if (globalIndex <= (commandHistory.size ()))
                    {
                        currentCommandSize = command.size ();

                        printf ("%c", 13);

                        for (i = 0; i < (currentCommandSize + commandPromptSize + 2); i++)
                        {
                            printf (" ");
                        }

                        if (globalIndex < ((commandHistory.size ()) - 1))
                        {
                            globalIndex++;
                            command = commandHistory[globalIndex];
                        }
                        else
                        {
                            command = currentlyTypingCommand;
                        }

                        currentPosition = command.size ();

                        //printCommandLinePrompt (prompt, commandFromPreviousLevel, command);
                        //printCommandLinePrompt (prompt, command, "");
                        //fflush (stdout);

                        string firstPart;
                        string secondPart;

                        StringUtils::splitBasedOnLastOccuranceOf (command, ' ', firstPart, secondPart);

                        commandFromPreviousLevel = firstPart;
                        command                  = secondPart;

                        if ("" == command)
                        {
                            command                  = commandFromPreviousLevel;
                            commandFromPreviousLevel = "";
                        }

                        CommandLineInterfaceEntry *pCommandLineInterfaceEntry = pWaveCommandLineInterface->getCommandLineInterfaceEntryForShowRunningConfiguration (commandFromPreviousLevel);

                        if (NULL == pCommandLineInterfaceEntry)
                        {
                            pCommandLineInterfaceEntry = this;
                        }

                        pCommandLineInterfaceEntry->acceptcommand (prompt, commandFromPreviousLevel, command, pWaveCommandLineInterface);

                        return;
                    }
                }

                if ((27 == c1) && (91 == c2) && (67 == c3)) // RIGHT Arrow
                {
                    if (currentPositionForPreviousLevel == (commandFromPreviousLevel.size () + 1))
                    {
                        if (currentPosition < (command.size ()))
                        {
                            currentPosition++;
                            printf ("%c", command[currentPosition - 1]);
                            fflush (stdout);
                        }
                    }
                    else
                    {
                        currentPositionForPreviousLevel++;

                        if (currentPositionForPreviousLevel <= (commandFromPreviousLevel.size ()))
                        {
                            printf ("%c", commandFromPreviousLevel[currentPositionForPreviousLevel - 1]);
                        }
                        else
                        {
                            printf (" ");
                        }

                        fflush (stdout);
                    }
                }

                if ((27 == c1) && (91 == c2) && (68 == c3)) // LEFT Arrow
                {
                    if (0 < currentPosition)
                    {
                        currentPosition--;
                        printf ("\b");
                        fflush (stdout);
                    }
                    else
                    {
                        if (0 < currentPositionForPreviousLevel)
                        {
                            currentPositionForPreviousLevel--;
                            printf ("\b");
                            fflush (stdout);
                        }
                    }
                }

                continue;
            }

            for (i = 0; i < numberOfCharactersAvailableToRead; i++)
            {
                c = getchar();

                if (4 == c)
                {
                    exit (0);
                }

                if ((0 != (isalnum (c))) || ('_' == c) || ('-' == c) || (':' == c) || ('.' == c) || ('=' == c) || ('/' == c) || ('<' == c) || ('>' == c) || ('{' == c) || ('}' == c) || ('[' == c) || (']' == c) || ('"' == c) || (',' == c))
                {
                    if (currentPositionForPreviousLevel == (commandFromPreviousLevel.size () + 1))
                    {
                        command.insert (currentPosition, 1, c);
                        currentPosition++;
                    }
                    else
                    {
                        commandFromPreviousLevel.insert (currentPositionForPreviousLevel, 1, c);
                        currentPositionForPreviousLevel++;
                    }

                    printCommandLinePrompt (prompt, commandFromPreviousLevel, command);

                    for (j = 0; j < ((command.size ()) - currentPosition); j++)
                    {
                        printf ("\b");
                    }

                    for (j = 0; j < ((commandFromPreviousLevel.size ()) + 1 - currentPositionForPreviousLevel); j++)
                    {
                        printf ("\b");
                    }

                    fflush (stdout);
                }
                else if ('\t' == c)
                {
                    if (("" == command) && (1 < (m_validCommands.size ())))
                    {
                        if (false == m_isKeyProcessingRequired)
                        {
                            displayPossibleCompletions (commandFromPreviousLevel, m_validCommands, false);
                        }
                        else
                        {
                            bool areKeysCompletelyEntered = false;
                            UI32 numberOfKeysEntered      = 0;

                            getInputDetailsForCommandLine (commandFromPreviousLevel, areKeysCompletelyEntered, numberOfKeysEntered);

                            if (false == areKeysCompletelyEntered)
                            {
                                vector<string> possibleCompletions;

                                possibleCompletions.push_back (m_keyCombination[numberOfKeysEntered]);

                                displayPossibleCompletions (commandFromPreviousLevel, possibleCompletions, false);
                            }
                            else
                            {
                                displayPossibleCompletions (commandFromPreviousLevel, m_validcommandsWithoutKeyCombination, true);
                            }
                        }
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
                            currentMatchAtThisLevel = matchingCommands[0];

                            if ("" == commandFromPreviousLevel)
                            {
                                commandFromPreviousLevel += currentMatchAtThisLevel;
                            }
                            else
                            {
                                commandFromPreviousLevel += " " + currentMatchAtThisLevel;
                            }

                            command = "";

                            if (true == (isAKnownChildEntry (currentMatchAtThisLevel)))
                            {
                                m_childEntriesMap[currentMatchAtThisLevel]->acceptcommand (prompt, commandFromPreviousLevel, command, pWaveCommandLineInterface);

                                currentPosition = command.size ();

                                return;
                            }
                        }
                        else
                        {
                            string commonStringAtBeginning;

                            StringUtils::getCommonStringAtBeginning (matchingCommands, commonStringAtBeginning);

                            displayPossibleCompletions (commandFromPreviousLevel, matchingCommands, false);

                            command = commonStringAtBeginning;

                            currentPosition = command.size ();
                        }
                    }

                    printCommandLinePrompt (prompt, commandFromPreviousLevel, command);

                    for (j = 0; j < ((command.size ()) - currentPosition); j++)
                    {
                        printf ("\b");
                    }

                    for (j = 0; j < ((commandFromPreviousLevel.size ()) + 1 - currentPositionForPreviousLevel); j++)
                    {
                        printf ("\b");
                    }

                    fflush (stdout);
                }
                else if ('?' == c)
                {
                    if ("" == command)
                    {
                        if (false == m_isKeyProcessingRequired)
                        {
                            displayPossibleCompletions (commandFromPreviousLevel, m_validCommands, true);
                        }
                        else
                        {
                            bool areKeysCompletelyEntered = false;
                            UI32 numberOfKeysEntered      = 0;

                            getInputDetailsForCommandLine (commandFromPreviousLevel, areKeysCompletelyEntered, numberOfKeysEntered);

                            if (false == areKeysCompletelyEntered)
                            {
                                vector<string> possibleCompletions;

                                possibleCompletions.push_back (m_keyCombination[numberOfKeysEntered]);

                                displayPossibleCompletions (commandFromPreviousLevel, possibleCompletions, true);
                            }
                            else
                            {
                                displayPossibleCompletions (commandFromPreviousLevel, m_validcommandsWithoutKeyCombination, true);
                            }
                        }

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

                        displayPossibleCompletions (commandFromPreviousLevel, matchingCommands, false);
                    }

                    printCommandLinePrompt (prompt, commandFromPreviousLevel, command);

                    for (j = 0; j < ((command.size ()) - currentPosition); j++)
                    {
                        printf ("\b");
                    }

                    for (j = 0; j < ((commandFromPreviousLevel.size ()) + 1 - currentPositionForPreviousLevel); j++)
                    {
                        printf ("\b");
                    }

                    fflush (stdout);
                }
                else if (' ' == c)
                {
                    if (currentPositionForPreviousLevel == (commandFromPreviousLevel.size () + 1))
                    {
                        CommandLineInterfaceEntry *pCommandLineInterfaceEntry = pWaveCommandLineInterface->getCommandLineInterfaceEntryForShowRunningConfiguration (commandFromPreviousLevel + " " + command);

                        if (NULL != pCommandLineInterfaceEntry)
                        {
                            if ("" == commandFromPreviousLevel)
                            {
                                commandFromPreviousLevel += command;
                            }
                            else
                            {
                                commandFromPreviousLevel += " " + command;
                            }

                            command = "";

                            pCommandLineInterfaceEntry->acceptcommand (prompt, commandFromPreviousLevel, command, pWaveCommandLineInterface);

                            return;
                        }
                        else
                        {
                            command.insert (currentPosition, 1, c);
                            currentPosition++;

                            printCommandLinePrompt (prompt, commandFromPreviousLevel, command);

                            for (j = 0; j < ((command.size ()) - currentPosition); j++)
                            {
                                printf ("\b");
                            }
                            for (j = 0; j < ((commandFromPreviousLevel.size ()) + 1 - currentPositionForPreviousLevel); j++)
                            {
                                printf ("\b");
                            }

                            fflush (stdout);
                        }
                    }
                    else
                    {
                        commandFromPreviousLevel.insert (currentPositionForPreviousLevel, 1, c);
                        currentPositionForPreviousLevel++;

                        printCommandLinePrompt (prompt, commandFromPreviousLevel, command);

                        for (j = 0; j < ((command.size ()) - currentPosition); j++)
                        {
                            printf ("\b");
                        }

                        for (j = 0; j < ((commandFromPreviousLevel.size ()) + 1 - currentPositionForPreviousLevel); j++)
                        {
                            printf ("\b");
                        }

                        fflush (stdout);
                    }
                }
                else
                {
                    if ((8 == c) || (127 == c))
                    {
                        if (0 < currentPosition)
                        {
                            printf ("%c", 13);

                            for (j = 0; j < (command.size () + commandPromptSize + 2); j++)
                            {
                                printf (" ");
                            }

                            command = command.erase (currentPosition - 1, 1);

                            currentPosition--;

                            printCommandLinePrompt (prompt, commandFromPreviousLevel, command);

                            for (j = 0; j < ((command.size ()) - currentPosition); j++)
                            {
                                printf ("\b");
                            }

                            for (j = 0; j < ((commandFromPreviousLevel.size ()) + 1 - currentPositionForPreviousLevel); j++)
                            {
                                printf ("\b");
                            }

                            fflush (stdout);
                        }
                        else
                        {
                            string firstPart;
                            string secondPart;

                            StringUtils::splitBasedOnLastOccuranceOf (commandFromPreviousLevel, ' ', firstPart, secondPart);

                            commandFromPreviousLevel = firstPart;
                            command                  = secondPart;

                            CommandLineInterfaceEntry *pCommandLineInterfaceEntry = pWaveCommandLineInterface->getCommandLineInterfaceEntryForShowRunningConfiguration (commandFromPreviousLevel);

                            if (NULL != pCommandLineInterfaceEntry)
                            {
                                pCommandLineInterfaceEntry->acceptcommand (prompt, commandFromPreviousLevel, command, pWaveCommandLineInterface);

                                return;
                            }
                        }
                    }
                }
            }

            if (c == 13)
            {
                printf ("\n\r");
                fflush (stdout);

                if ("" == commandFromPreviousLevel)
                {
                    commandFromPreviousLevel += command;
                }
                else
                {
                    commandFromPreviousLevel += " " + command;
                }

                return;
            }

            currentlyTypingCommand = command;
        }
    }

    return;
}

void CommandLineInterfaceEntry::sortValidCommands ()
{
    std::sort (m_validCommands.begin (), m_validCommands.end ());

    if (true == m_isKeyProcessingRequired)
    {
        computeValidCommandsWithoutKeyCombination ();
    }

    UI32 numberOfValidCommands = m_validCommands.size ();
    UI32 i                     = 0;

    for (i = 0; i < numberOfValidCommands; i++)
    {
        string                     validCommand               = m_validCommands[i];
        CommandLineInterfaceEntry *pCommandLineInterfaceEntry = m_childEntriesMap[validCommand];

        prismAssert (NULL != pCommandLineInterfaceEntry, __FILE__, __LINE__);

        pCommandLineInterfaceEntry->sortValidCommands ();
    }
}

vector<YangElement *> CommandLineInterfaceEntry::getYangElementsByTargetNodeName (const string &targetNodeName)
{
    return (WaveCommandLineInterface::getYangElementsByTargetNodeName (targetNodeName));
}

vector<YangElement *> CommandLineInterfaceEntry::getYangElements ()
{
    return (m_yangElements);
}

void CommandLineInterfaceEntry::displayPossibleCompletions (const string &commandFromPreviousLevel, const vector<string> &possibleCompletions, const bool &displayDescription)
{
           UI32                   j                               = 0;
           UI32                   k                               = 0;
           UI32                   maximumCommandLength            = 0;
           UI32                   commandLength                   = 0;
           UI32                   numberOfSpacesForThisCommand    = 0;
           vector<YangElement *>  yangElements;
           string                 prunedCommandFromPreviousLevel;
           string                 prunedCommand;
           string                 infoString;
           YangElement           *pTempYangElement                = NULL;
           UI32                   numberOfPossibleCompletions     = possibleCompletions.size ();
    static vector<string>         auxiliaryCompletions;
    static vector<string>         auxiliaryCompletionsInfo;
    static UI32                   numberOfAuxiliaryCompletions    = 0;


    if (0 == (auxiliaryCompletions.size ()))
    {
        auxiliaryCompletions.push_back ("|");
        auxiliaryCompletionsInfo.push_back ("Output modifiers");

        auxiliaryCompletions.push_back ("<cr>");
        auxiliaryCompletionsInfo.push_back ("");

        prismAssert ((auxiliaryCompletions.size ()) == (auxiliaryCompletionsInfo.size ()), __FILE__, __LINE__);

        numberOfAuxiliaryCompletions = auxiliaryCompletions.size ();
    }

    for (j = 0; j < numberOfPossibleCompletions; j++)
    {
        commandLength = possibleCompletions[j].size ();

        if (commandLength > maximumCommandLength)
        {
            maximumCommandLength = commandLength;
        }
    }

    for (j = 0; j < numberOfAuxiliaryCompletions; j++)
    {
        commandLength = auxiliaryCompletions[j].size ();

        if (commandLength > maximumCommandLength)
        {
            maximumCommandLength = commandLength;
        }
    }

    maximumCommandLength += 2;

    prunedCommandFromPreviousLevel = commandFromPreviousLevel;


    removeKeysFromCommandLine (prunedCommandFromPreviousLevel);
    

    StringUtils::replaceAllInstancesOfInputStringWith (prunedCommandFromPreviousLevel, "show running-config ", "");
    StringUtils::replaceAllInstancesOfInputStringWith (prunedCommandFromPreviousLevel, "show running-config", "");

    if (true == displayDescription)
    {
        yangElements = getYangElementsByTargetNodeName (prunedCommandFromPreviousLevel);

        if (0 < (yangElements.size ()))
        {
            pTempYangElement = yangElements[0];

            prismAssert (NULL != pTempYangElement, __FILE__, __LINE__);

            infoString = pTempYangElement->getInfo ();

            StringUtils::stripFromFirstOccuranceOf (infoString, '\n');

            printf ("\r\nDescription : %s ", infoString.c_str ());
        }
    }

    if ((0 != numberOfPossibleCompletions) || (true == displayDescription))
    {
        printf ("\r\nPossible completions :");
    }

    for (j = 0; j < numberOfPossibleCompletions; j++)
    {
        commandLength = possibleCompletions[j].size ();

        printf ("\n\r  %s", (possibleCompletions[j]).c_str ());

        numberOfSpacesForThisCommand = maximumCommandLength - commandLength;

        for (k = 0; k < numberOfSpacesForThisCommand; k++)
        {
            printf (" ");
        }

        if ("" != prunedCommandFromPreviousLevel)
        {
            prunedCommand = prunedCommandFromPreviousLevel + " " + possibleCompletions[j];
        }
        else
        {
            prunedCommand = possibleCompletions[j];
        }

        yangElements = getYangElementsByTargetNodeName (prunedCommand);

        if (0 < (yangElements.size ()))
        {
            pTempYangElement = yangElements[0];

            prismAssert (NULL != pTempYangElement, __FILE__, __LINE__);

            infoString = pTempYangElement->getInfo ();

            StringUtils::stripFromFirstOccuranceOf (infoString, '\n');

            printf (" %s", infoString.c_str ());
        }
    }


    if (true == displayDescription)
    {
        for (j = 0; j < numberOfAuxiliaryCompletions; j++)
        {
            commandLength = auxiliaryCompletions[j].size ();

            printf ("\n\r  %s", (auxiliaryCompletions[j]).c_str ());

            numberOfSpacesForThisCommand = maximumCommandLength - commandLength;

            for (k = 0; k < numberOfSpacesForThisCommand; k++)
            {
                printf (" ");
            }

            printf (" %s", auxiliaryCompletionsInfo[j].c_str ());
        }
    }

    printf ("\n\r");
}

void CommandLineInterfaceEntry::addToCommandHistory (const string &command)
{
    if ("" != command)
    {
        m_commandHistory.push_back (command);
    }
}

vector<string> &CommandLineInterfaceEntry::getCommandHistory ()
{
    return (m_commandHistory);
}

UI32 &CommandLineInterfaceEntry::getCommandHistoryCurrentIndex ()
{
    return (m_commandHistoryCurrentIndex);
}

void CommandLineInterfaceEntry::resetCommandHistoryCurrentIndex ()
{
    m_commandHistoryCurrentIndex = m_commandHistory.size ();
}

string &CommandLineInterfaceEntry::getCurrentlyTypingCommand ()
{
    return (m_currentlyTypingCommand);
}

void CommandLineInterfaceEntry::resetCurrentlyTypingCommand ()
{
    m_currentlyTypingCommand = "";
}

bool CommandLineInterfaceEntry::getIsKeyProcessingRequired () const
{
    return (m_isKeyProcessingRequired);
}

void CommandLineInterfaceEntry::setIsKeyProcessingRequired (const bool &isKeyProcessingRequired)
{
    m_isKeyProcessingRequired = isKeyProcessingRequired;
}

vector<string> CommandLineInterfaceEntry::getKeyCombination () const
{
    return (m_keyCombination);
}

void CommandLineInterfaceEntry::setKeyCombination (const vector<string> &keyCombination)
{
    m_keyCombination = keyCombination;
}

CommandLineInterfaceEntry *CommandLineInterfaceEntry::getPParentCommandLineInterfaceEntry ()
{
    return (m_pParentCommandLinteInterfaceEntry);
}

void CommandLineInterfaceEntry::setPParentCommandLineInterfaceEntry (CommandLineInterfaceEntry *pParentCommandLineInterfaceEntry)
{
    prismAssert (NULL == m_pParentCommandLinteInterfaceEntry, __FILE__, __LINE__);

    m_pParentCommandLinteInterfaceEntry = pParentCommandLineInterfaceEntry;

    string parentCliTargetNodeName = m_pParentCommandLinteInterfaceEntry->getCliTargetNodeName ();

    if ("" != parentCliTargetNodeName)
    {
        m_cliTargetNodeName = parentCliTargetNodeName + " " + m_name;
    }
    else
    {
        m_cliTargetNodeName = m_name;
    }

    m_yangElements = getYangElementsByTargetNodeName (m_cliTargetNodeName);

    UI32   numberOfYangElements     = m_yangElements.size ();
    UI32   i                        = 0;
    bool   areAllElementsOfSameType = true;
    string yangName;

    if (0 < numberOfYangElements)
    {
        YangElement *pYangElement = m_yangElements[0];

        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        yangName = pYangElement->getYangName ();
    }

    if (1 < numberOfYangElements)
    {
        //cout << "Multiple Yang Elements for : " << m_cliTargetNodeName << " : " << numberOfYangElements << "\r\n";
    }

    for (i = 0; i < numberOfYangElements; i++)
    {
        YangElement *pYangElement = m_yangElements[i];

        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        if (yangName != (pYangElement->getYangName ()))
        {
            areAllElementsOfSameType = false;
        }

        YangList *pYangList = dynamic_cast<YangList *> (pYangElement);

        if (NULL != pYangList)
        {
            m_isKeyProcessingRequired = true;
            //break;
        }
    }

    // TODO : sagar : Put the following logic in checking yang integrity.

    if (false == areAllElementsOfSameType)
    {
        cout << "All Elements are NOT Same for : " << m_cliTargetNodeName << " : " << numberOfYangElements << "\r\n";

        for (i = 0; i < numberOfYangElements; i++)
        {
            YangElement *pYangElement = m_yangElements[i];

            prismAssert (NULL != pYangElement, __FILE__, __LINE__);

            cout << "    " << pYangElement->getYangName () << ":" << pYangElement->getName () << "\r\n";
        }

        trace (TRACE_LEVEL_INFO, "CommandLineInterfaceEntry::setPParentCommandLineInterfaceEntry : Multiple YANG Elements YANG Names do not match for : " + m_cliTargetNodeName);

//        prismAssert (false, __FILE__, __LINE__);
    }
//    else
//    {
        if (0 < numberOfYangElements)
        {
            YangElement *pYangElement = m_yangElements[0];

            prismAssert (NULL != pYangElement, __FILE__, __LINE__);

            YangList *pYangList = dynamic_cast<YangList *> (pYangElement);

            if (NULL != pYangList)
            {
                m_keyCombination = pYangList->getKeyCombination ();
            }
        }
//    }
}

string CommandLineInterfaceEntry::getCliTargetNodeName () const
{
    return (m_cliTargetNodeName);
}

void CommandLineInterfaceEntry::setCliTargetNodeName (const string &cliTargetNodeName)
{
    m_cliTargetNodeName = cliTargetNodeName;
}

void CommandLineInterfaceEntry::computeValidCommandsWithoutKeyCombination ()
{
    UI32                     numberOfKeyCombinationEntries = m_keyCombination.size ();
    UI32                     i                             = 0;
    vector<string>::iterator keyCombinationElement;

    m_validcommandsWithoutKeyCombination = m_validCommands;

    for (i = 0; i < numberOfKeyCombinationEntries; i++)
    {
        keyCombinationElement = std::find (m_validcommandsWithoutKeyCombination.begin (), m_validcommandsWithoutKeyCombination.end (), m_keyCombination[i]);

        if ((m_validCommands.end ()) != (keyCombinationElement))
        {
            m_validcommandsWithoutKeyCombination.erase (keyCombinationElement);
        }
    }
}

void CommandLineInterfaceEntry::getValidCommands (vector<string> &allPossibleCompletion ,vector<string> &keyVector , string &token)
{
    UI32   numberOfYangElements     = m_yangElements.size ();
    string yangName;
    bool found = false;
    bool noMatchCompletion = false;
    bool suppressShowConfPath = false;

    for (UI32 i = 0; i < numberOfYangElements; i++)
    {
        YangElement *pYangElement = m_yangElements[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        suppressShowConfPath = pYangElement->getCliSuppressShowConfPath (); 

        if (!suppressShowConfPath)
        {
            YangList *pYangList = dynamic_cast<YangList *> (pYangElement);
            if (NULL != pYangList && (!pYangList->getCliNoKeyCompletion () || pYangList->getCliNoMatchCompletion ())  ) 
            {
                noMatchCompletion = pYangList->getCliNoMatchCompletion ();
                // Get MO Name from ConfigurationSegmentName of yangelement 
                string managedObjectClassName = WaveConfigurationSegmentMap::getManagedObjectClassNameByConfigurationSegmentName (pYangElement->getConfigurationSegmentName ());
                
                vector<string> ormUserKeyCombinationForClassName;

                OrmRepository::getUserDefinedKeyCombinationForTable (managedObjectClassName, ormUserKeyCombinationForClassName);
                
                WaveManagedObjectSynchronousQueryContext syncQueryCtxt (managedObjectClassName);
                
                for (UI32 keyCombo=0; keyCombo < ormUserKeyCombinationForClassName.size (); keyCombo++)
                {
                    tracePrintf (TRACE_LEVEL_INFO, false, false, "getValidCommands: %s", ormUserKeyCombinationForClassName[keyCombo].c_str ());
                    syncQueryCtxt.addSelectField (ormUserKeyCombinationForClassName[keyCombo]);
                }

                //Querry DB for the MO
                vector<WaveManagedObject *> *pResult = WaveObjectManagerToolKit::querySynchronously (&syncQueryCtxt);
                // get the keys of the list 
                vector<string> keyCombination = pYangList->getKeyCombination ();

                for (UI32 j = 0 ; j < keyCombination.size() && !found ; j++)
                {
                    YangElement *pYangListkey =  pYangList->getUniqueYangElementByName (keyCombination[j]);
                    UI32 userTag = pYangListkey->getUserTagValue ();    

                    for (UI32 k = 0 ; k < pResult->size () && !found; k++)
                    {
                        Attribute* atter = (*pResult)[k]->getAttributeByUserTag (userTag);
                        string value ;
                        atter->toString (value);

                        if (!noMatchCompletion)
                        {
                            allPossibleCompletion.push_back (value);
                        }    

                        if (std::find(keyVector.begin (), keyVector.end (), value) != keyVector.end ()  || (noMatchCompletion && (token == value)))
                        {
                            allPossibleCompletion.clear ();
                            found = true;
                            noMatchCompletion = false;
                        }
                    }    
                }     
                
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResult);
            }
        }
    }
    // if suppressShowConfPath is false and noMatchCompletion is false then only put the valid commands in possible completion
    for (UI32 i = 0 ; (i < m_validCommands.size() ) && (!noMatchCompletion || found) && (!suppressShowConfPath)  ; i++)
    {
        allPossibleCompletion.push_back(m_validCommands[i]);
    }
}

}
