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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerInstallWorker.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Database/DatabaseConnection.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace WaveNs
{

bool DatabaseObjectManagerInstallWorker::m_fullInstallRequried = true;

DatabaseObjectManagerInstallWorker::DatabaseObjectManagerInstallWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
}

DatabaseObjectManagerInstallWorker::~DatabaseObjectManagerInstallWorker ()
{
}

void DatabaseObjectManagerInstallWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    // In case of persistent boot with default we do not need to install database
    if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)
    {
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
        return;
    }

    trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManagerInstallWorker::install : Entering ...");

    LocationRole thisLocationRole = FrameworkToolKit::getThisLocationRole ();

    if ((LOCATION_PRIMARY != thisLocationRole) && (LOCATION_STAND_ALONE != thisLocationRole))
    {
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
        return;
    }

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInstallWorker::installValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInstallWorker::installDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInstallWorker::installBootDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInstallWorker::installCreatePrismDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInstallWorker::installShutdownDatabaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInstallWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DatabaseObjectManagerInstallWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    m_fullInstallRequried = true;

    pPrismLinearSequencerContext->start ();
}

void DatabaseObjectManagerInstallWorker::installValidateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void DatabaseObjectManagerInstallWorker::installDatabaseStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManagerInstallWorker::installDatabaseStep : No support for Persistent Store will be provided.");
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManagerInstallWorker::installDatabaseStep : Creating repository for persistence ...");

#if 0
    const string   databaseDirectory                 = DatabaseObjectManager::getDatabaseDirectory ();
    const string   databaseInstallCompletionFileName = DatabaseObjectManager::getWaveDatabaseInstallCompletionFileName ();
    const string   completionFilePath                = databaseDirectory + "/" + databaseInstallCompletionFileName;
          ifstream completionFile                      (completionFilePath.c_str (), ios::in);

    if (true == (completionFile.fail ()))
    {
#endif
        //trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installDatabaseStep : Database was never installed.  Initializing Database.");
        trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installDatabaseStep : Initializing Database.");

        system ((string ("rm -rf ") + DatabaseObjectManager::getDatabaseDirectory ()).c_str ());

        system ("rm -rf *.cfg *.pgd");

        // Indicate that initdb output starts below this
        string echoStr = string ("echo \"Executing the initdb command\" >> ") + DatabaseObjectManager::getDatabaseLogFileName ();
        system(echoStr.c_str ());

        string command = string ("initdb -A trust -D ") + DatabaseObjectManager::getDatabaseDirectory () + string (" >> ") + DatabaseObjectManager::getDatabaseLogFileName () + string (" 2>&1");

        // Log the command as well
        echoStr = string("echo \"") + command + string(" \" >> ") + DatabaseObjectManager::getDatabaseLogFileName ();
        system(echoStr.c_str ());

        // Execute the command
        int ret = system(command.c_str ());

        // Log the return status
        trace (TRACE_LEVEL_INFO, string("DatabaseObjectManagerInstallWorker::installDatabaseStep : Initializing Database Completed. Ret:") + ret);

//        PersistenceObjectManager::setSchemaDropRequiredDuringBoot (false);

        m_fullInstallRequried = true;
#if 0
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installDatabaseStep : Database was already installed.  Reusing the already initialized Database.");

        system ((string ("pg_ctl -D ") + DatabaseObjectManager::getDatabaseDirectory () + string (" stop")).c_str ());

        system ("rm -rf *.cfg *.pgd");

        completionFile.close ();

        trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installDatabaseStep : Preparation for Reusing Database Completed.");

        PersistenceObjectManager::setSchemaDropRequiredDuringBoot (true);

        m_fullInstallRequried = false;
    }
#endif
    trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManagerInstallWorker::installDatabaseStep : Done.");

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void DatabaseObjectManagerInstallWorker::installBootDatabaseStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    if (false == m_fullInstallRequried)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installBootDatabaseStep : Booting the database to create Prism Database...");

    // Indicate that pg_ctl command output starts below this
    string echoStr = string ("echo \"Executing the pg_ctl start command\" >> ") + DatabaseObjectManager::getDatabaseLogFileName ();
    system(echoStr.c_str ());

    UI8     retries       = 10;
    SI32    status        = -1;
    string  commandString = string ("pg_ctl -D ") + DatabaseObjectManager::getDatabaseDirectory () + DatabaseObjectManager::getDatabaseLogStringForPgctl () + string (" -w -t 120 -o \"--log_line_prefix='%t %a [%e] ' -p ") + DatabaseObjectManager::getDatabasePort () + "\" start >> " + DatabaseObjectManager::getDatabaseLogFileName () + string (" 2>&1");

    // Log the command as well
    echoStr = string("echo \"") + commandString + string(" \" >> ") + DatabaseObjectManager::getDatabaseLogFileName ();
    system(echoStr.c_str ());

    // Execute the command
    for (UI8 i = 0; i < retries; i++)
    {
        vector<string>  output;
                        status = FrameworkToolKit::systemCommandOutput (commandString.c_str (), output);

        if (0 == status)
        {
            trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installBootDatabaseStep : Done");
            break;
        }

        /* 
         * Sometimes pg_ctl command returns non-zero but postgres has actually started.
         * So, lets try connecting to DB and confirm if we need to retry 'pg_ctl start' command.
         */

        prismSleep (4);

        bool                isConnectedToDatabse = false;
        DatabaseConnection *pDatabaseConnection  = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());

        prismAssert (NULL != pDatabaseConnection, __FILE__, __LINE__);

        if (NULL != (pDatabaseConnection->getPConnection ()))
        {
            trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerInstallWorker::installBootDatabaseStep : Already connected to Wave Database. This should not happen as it is just started.");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            isConnectedToDatabse = pDatabaseConnection->connect (120);

            if (false == isConnectedToDatabse)
            {
                trace (TRACE_LEVEL_ERROR, "DatabaseObjectManagerInstallWorker::installBootDatabaseStep : Could not connect to database.");
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installBootDatabaseStep : Connection succeeded even if pg_ctl errored out. Treating postgres has started successfully.");
                DatabaseConnection::deleteInstance ();
                status = 0;
                break;
            }
        }


        trace (TRACE_LEVEL_WARN, "DatabaseObjectManagerInstallWorker::installBootDatabaseStep : failed to start postgres server. -");
        if (0 < output.size())
        {
            tracePrintf (TRACE_LEVEL_WARN, false, true, output[0].c_str());
        }
        prismSleep (4);
    }

    if (0 != status)
    {
        tracePrintf (TRACE_LEVEL_WARN, true, false, string("DatabaseObjectManagerInstallWorker::installBootDatabaseStep : failed to start postgres server after [%s] retries also. Can't move forward. Asserting.").c_str(), retries);
        prismAssert (false, __FILE__, __LINE__);
    }

    // Log the return status
    trace (TRACE_LEVEL_INFO, string("DatabaseObjectManagerInstallWorker::installBootDatabaseStep : ret:") + status);

    prismSleep (20);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void DatabaseObjectManagerInstallWorker::installCreatePrismDatabaseStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    if (false == m_fullInstallRequried)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    const string   databaseDirectory                 = DatabaseObjectManager::getDatabaseDirectory ();
    const string   databaseInstallCompletionFileName = DatabaseObjectManager::getWaveDatabaseInstallCompletionFileName ();
    const string   completionFilePath                = databaseDirectory + "/" + databaseInstallCompletionFileName;
          ofstream completionfileNew;

    trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installCreatePrismDatabaseStep : Creating Prism Database...");

    string commandString = string ("createdb ") + DatabaseObjectManager::getDatabaseName () + " -p " + DatabaseObjectManager::getDatabasePort ();
    system (commandString.c_str ());

    completionfileNew.open (completionFilePath.c_str ());
    completionfileNew.close ();
    completionfileNew.clear ();

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void DatabaseObjectManagerInstallWorker::installShutdownDatabaseStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    if (false == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    if (false == m_fullInstallRequried)
    {
        pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    trace (TRACE_LEVEL_INFO, "DatabaseObjectManagerInstallWorker::installShutdownDatabaseStep : Shutting down the database to create Prism Database...");

    string commandString = string ("pg_ctl -D ") + DatabaseObjectManager::getDatabaseDirectory () + string (" -o \" -p ") + DatabaseObjectManager::getDatabasePort () + "\" stop > /dev/null";
    system (commandString.c_str ());

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
