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

#ifndef COMMANDLINEINTERFACEENTRY_H
#define COMMANDLINEINTERFACEENTRY_H

#include "Framework/Types/Types.h"

#include <map>

namespace WaveNs
{

class YangElement;
class WaveCommandLineInterface;

class CommandLineInterfaceEntry
{
    private :
    protected :
    public :
                                   CommandLineInterfaceEntry            ();
                                   CommandLineInterfaceEntry            (const string &name);
                                  ~CommandLineInterfaceEntry            ();

        string                     getName                              () const;

        bool                       isAKnownChildEntry                   (const string &name) const;

        void                       addChildEntryIfDoesNotExist          (CommandLineInterfaceEntry *pChildCommandLineinterfaceEntry);
        void                       addChildEntryIfDoesNotExist          (const string &childCommandLineInterfaceEntryName);

        void                       addCommandLine                       (const string &commandLine);
        void                       addCommandLines                      (const vector<string> &commandLines);
        CommandLineInterfaceEntry *getCommandLineInterfaceEntry         (const string &commandLine , vector<string> &keyVector);
        void                       getInputDetailsForCommandLine        (const string &commandLine, bool &areKeysCompletelyEntered, UI32 &numberOfKeysEntered);
        void                       removeKeysFromCommandLine            (string &commandLine);

        bool                       isAFullCommand                       () const;

        string                     getModePrompt                        () const;
        void                       setModePrompt                        (const string &modePrompt);

        UI32                       getNumberOfCharactersAvailableToRead ();
        void                       printCommandLinePrompt               (const string &prompt, const string &commandFromPreviousLevel, const string &command) const;
        void                       acceptcommand                        (const string &prompt, string &commandFromPreviousLevel, string &command, WaveCommandLineInterface *pWaveCommandLineInterface);

        void                       sortValidCommands                    ();

        vector<YangElement *>      getYangElementsByTargetNodeName      (const string &targetNodeName);
        void                       displayPossibleCompletions           (const string &commandFromPreviousLevel, const vector<string> &possibleCompletions, const bool &displayDescription);

        void                       addToCommandHistory                  (const string &command);
        vector<string>            &getCommandHistory                    ();
        UI32                      &getCommandHistoryCurrentIndex        ();
        void                       resetCommandHistoryCurrentIndex      ();
        string                    &getCurrentlyTypingCommand            ();
        void                       resetCurrentlyTypingCommand          ();

        bool                       getIsKeyProcessingRequired           () const;
        void                       setIsKeyProcessingRequired           (const bool &isKeyProcessingRequired);
        vector<string>             getKeyCombination                    () const;
        void                       setKeyCombination                    (const vector<string> &keyCombination);
        CommandLineInterfaceEntry *getPParentCommandLineInterfaceEntry  ();
        void                       setPParentCommandLineInterfaceEntry  (CommandLineInterfaceEntry *pParentCommandLineInterfaceEntry);
        string                     getCliTargetNodeName                 () const;
        void                       setCliTargetNodeName                 (const string &cliTargetNodeName);
        void                       computeValidCommandsWithoutKeyCombination ();
        void                       getValidCommands                     (vector<string> &allPossibleCompletion, vector<string> &keyValue , string &token);
        vector<YangElement *>      getYangElements                      ();

    private:
        void                       addTovalidCommands                   ( CommandLineInterfaceEntry *pChildCommandLineinterfaceEntry ,  const string &childCommandLineInterfaceEntryName);

    // Now the data members

    private :
        string                                    m_name;
        vector<CommandLineInterfaceEntry *>       m_childEntries;
        map <string, CommandLineInterfaceEntry *> m_childEntriesMap;
        vector<string>                            m_commandHistory;
        string                                    m_modePrompt;
        vector<string>                            m_validCommands;
        UI32                                      m_commandHistoryCurrentIndex;
        string                                    m_currentlyTypingCommand;
        bool                                      m_isKeyProcessingRequired;
        vector<string>                            m_keyCombination;
        vector<YangElement *>                     m_yangElements;
        CommandLineInterfaceEntry                *m_pParentCommandLinteInterfaceEntry;
        string                                    m_cliTargetNodeName;
        vector<string>                            m_validcommandsWithoutKeyCombination;

    protected :
    public :
};

}

#endif // COMMANDLINEINTERFACEENTRY_H
