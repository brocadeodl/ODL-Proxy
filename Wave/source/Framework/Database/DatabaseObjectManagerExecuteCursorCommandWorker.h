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

#ifndef DATABASEOBJECTMANAGEREXECUTECURSORWORKER_H
#define DATABASEOBJECTMANAGEREXECUTECURSORWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "libpq-fe.h"

namespace WaveNs
{

class DatabaseObjectManagerExecuteCursorCfgCommandMessage;
class DatabaseObjectManagerExecuteCursorQueryCommandMessage;

class DatabaseObjectManagerExecuteCursorCommandWorker : public WaveWorker
{
    private :
        PGconn *getDatabaseServerConnection (const string &className);

    protected :
    public :
                      DatabaseObjectManagerExecuteCursorCommandWorker (WaveObjectManager *pWaveObjectManager, bool isMultiDatabaseMode = false);
        virtual      ~DatabaseObjectManagerExecuteCursorCommandWorker ();

                void  executeCursorCfgCommandMessageHandler           (DatabaseObjectManagerExecuteCursorCfgCommandMessage *pDatabaseObjectManagerExecuteCursorCfgCommandMessage);
                void  executeCursorCfgCommandHandler                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
	
                void  executeCursorQueryCommandMessageHandler         (DatabaseObjectManagerExecuteCursorQueryCommandMessage *pDatabaseObjectManagerExecuteCursorQueryCommandMessage);

    // Now the data members

    private :
        bool m_isMultiDatabaseMode;

    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGERCURSORWORKER_H






