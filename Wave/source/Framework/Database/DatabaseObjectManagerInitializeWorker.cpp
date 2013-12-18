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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerInitializeWorker.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

DatabaseObjectManagerInitializeWorker::DatabaseObjectManagerInitializeWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
}

DatabaseObjectManagerInitializeWorker::~DatabaseObjectManagerInitializeWorker ()
{
}

void DatabaseObjectManagerInitializeWorker::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInitializeWorker::initializeCreateObjectRelationalMappingStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInitializeWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInitializeWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void DatabaseObjectManagerInitializeWorker::initializeCreateObjectRelationalMappingStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void DatabaseObjectManagerInitializeWorker::zeroize (WaveAsynchronousContextForShutDownPhases *pPrismAsynchronousContext)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInitializeWorker::zeroizeDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInitializeWorker::zeroizeDatabaseBackupFilesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInitializeWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInitializeWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPrismAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void DatabaseObjectManagerInitializeWorker::zeroizeDatabaseStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

    string          dbDirPath         = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + DatabaseObjectManager::getDatabaseDirectory () ;
    vector<string>  commandOutput;
    string          output = "";

    string shutdownPgSQLCmd =   string("pg_ctl -D ") + dbDirPath + string(" -m immediate stop");
    
    trace(TRACE_LEVEL_INFO,"DatabaseObjectManagerInitializeWorker::zeroizeDatabaseStep:" + shutdownPgSQLCmd);
    FrameworkToolKit::systemCommandOutput (shutdownPgSQLCmd, commandOutput);
    for(UI32 i = 0 ; i< commandOutput.size(); i++){
       output.append(commandOutput[i]);
    } 
    trace(TRACE_LEVEL_INFO,"DatabaseObjectManagerInitializeWorker::zeroizeDatabaseStep: " + output);

    // Delete postgresql log file.
    string databaseLogFile = DatabaseObjectManager::getDatabaseLogFileName ();
    bool removeOnClear = true;
    FrameworkToolKit::secureClearFile(databaseLogFile, removeOnClear);
    FrameworkToolKit::secureClearFile("/mnt" + databaseLogFile, removeOnClear);
    trace (TRACE_LEVEL_INFO, string ("PrismFrameworkObjectManager::zeroizeDatabaseStep: cleared and deleted databaseLogFiles, [") + databaseLogFile + string ("]"));

    trace (TRACE_LEVEL_INFO,"PrismFrameworkObjectManager::zeroizeDatabaseStep:Deleting database files");
    FrameworkToolKit::secureDeleteDir(dbDirPath);

    trace (TRACE_LEVEL_INFO,"PrismFrameworkObjectManager::zeroizeDatabaseStep:Deleting database files from secondary partition");
    FrameworkToolKit::secureDeleteDir("/mnt" + dbDirPath );

    trace (TRACE_LEVEL_INFO,"PrismFrameworkObjectManager::zeroizeDatabaseStep:Database Cleanup Complete");

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}
	
// The database backup files should be cleaned up once the file is restored using pg_restore.
// Cleaning them here just for precaution.
void DatabaseObjectManagerInitializeWorker::zeroizeDatabaseBackupFilesStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{   
    string dbBackupFile = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName ());
    FrameworkToolKit::secureClearFile(dbBackupFile, true);
    FrameworkToolKit::secureClearFile("/mnt" + dbBackupFile, true);

    dbBackupFile.clear ();
    dbBackupFile = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ()); 
    FrameworkToolKit::secureClearFile(dbBackupFile, true);
    FrameworkToolKit::secureClearFile("/mnt" + dbBackupFile, true);

    dbBackupFile.clear ();
    dbBackupFile = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName3 ());
    FrameworkToolKit::secureClearFile(dbBackupFile, true);
    FrameworkToolKit::secureClearFile("/mnt" + dbBackupFile, true);

    trace (TRACE_LEVEL_INFO,"PrismFrameworkObjectManager::zeroizeDatabaseBackupFilesStep:Database Backup files Cleanup Complete");
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
