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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVESYSTEMMANAGEMENTGETPOSSIBLECOMPLETIONSWORKER_H
#define WAVESYSTEMMANAGEMENTGETPOSSIBLECOMPLETIONSWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "SystemManagement/CommandLineInterface/Client/CommandLineInterfaceEntry.h"

namespace WaveNs
{

class WaveSystemManagementGetPossibleCompletionsMessage; 

class WaveSystemManagementGetPossibleCompletionsWorker : public WaveWorker
{
    private :
        virtual PrismMessage *createMessageInstance (const UI32 &operationCode);

    protected :
    public :
                            WaveSystemManagementGetPossibleCompletionsWorker    (WaveObjectManager *pWaveObjectManager);
        virtual            ~WaveSystemManagementGetPossibleCompletionsWorker    ();
                void        getPossibleCompletionsMessageHandler                (WaveSystemManagementGetPossibleCompletionsMessage *pWaveSystemManagementGetPossibleCompletionsMessage); 
                void        initializeCommandLineInterfaces ();

    // Now the Data Members

    private :
    protected :
    public :
        CommandLineInterfaceEntry  *m_pTopLevelCommandLineinterfaceEntry;
        CommandLineInterfaceEntry  *m_pCommandLineInterfaceEntryForShowRunningConfig;
        CommandLineInterfaceEntry  *m_pCommandLineInterfaceEntryForShow;
        YangUserInterface          *m_pYangUserInterface;
        bool                        m_isInitializationDone;
};

}

#endif // WAVESYSTEMMANAGEMENTGETPOSSIBLECOMPLETIONSWORKER_H 
