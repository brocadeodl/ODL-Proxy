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

#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManagerInitializeWorker.h"
#include "Framework/Database/DatabaseObjectManagerInstallWorker.h"
#include "Framework/Database/DatabaseObjectManagerBootWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCursorCommandWorker.h"
#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableWorker.h"
#include "Framework/Database/DatabaseObjectManagerBackupWorker.h"
#include "Framework/Database/DatabaseObjectManagerEmptyWorker.h"
#include "Framework/Database/DatabaseObjectManagerRestoreWorker.h"
#include "Framework/Database/DatabaseObjectManagerCalculateDbCksumWorker.h"
#include "Framework/Database/DatabaseObjectManagerCleanPreparedTransactionsWorker.h"
#include "Framework/Database/DatabaseObjectManagerShutdownWorker.h"
#include "Framework/Database/DatabaseObjectManagerGetDbAccessDetailsWorker.h"
#include "Framework/ObjectModel/ObjectFactory/WaveManagedObjectFactory.h"
#include "Framework/Database/DatabaseObjectManagerCreateInMemoryManagedObjectMessage.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Shell/ShellDebug.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForDebugInformation.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/Wave.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"
#include "Framework/Database/MultiDatabaseObjectManager.h"
#include "Framework/Database/MultiDatabaseToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/DistributedLog/DistributedLogEntryManagedObject.h"

#include <stdlib.h>
#include <fstream>
#include <algorithm>

namespace WaveNs
{

static bool       s_isDatabaseEnabled;
static PrismMutex s_isDatabaseEnabledMutex;
static SI32       s_databasePort;
static PrismMutex s_databasePortMutex;
static string     s_databaseName                    = "WaveDatabase";
static PrismMutex s_databaseNameMutex;
static string     s_databaseLogFileName             = "pg_ctl";
static PrismMutex s_databaseLogFileNameMutex;
static bool       s_isDatabaseLogEnabled;
static PrismMutex s_isDatabaseLogEnabledMutex;
static string     s_databaseSharedBuffers           = "128MB";
static PrismMutex s_databaseSharedBuffersMutex;
static bool       s_isDatabaseConnected             = true;
static bool       s_isDatabaseShuttingDownOnReboot  = false;
static PrismMutex s_isDatabaseShuttingDownOnRebootMutex;

const string DatabaseObjectManager::m_databaseDirectory                 = "WaveDatabase";
const string DatabaseObjectManager::m_databaseInstallCompletionFileName = ".__WaveDatabaseInstallCompleted__";

const  UI32   NUM_OF_CORRUPTION_IDENTIFIERS                                    = 8; 
static string s_databaseCorruptionIdentifiers [NUM_OF_CORRUPTION_IDENTIFIERS]  = {"invalid page header", "could not read block", "database system is starting up", "could not connect to server: No such file or directory", "could not open file", "could not seek to end of file", "is not a valid data directory", "bad file descriptor"};

const  string DatabaseObjectManager::m_databaseCorruptIdentifierFileName       = ".WaveDatabaseFoundCorrupted";

static UI32         s_databaseEmptyType      = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION;
static PrismMutex   s_databaseEmptyTypeMutex;

static UI32         s_databaseEmptyTypeAutoDetectionThresholdValue      = 75000;
static PrismMutex   s_databaseEmptyTypeAutoDetectionThresholdValueMutex;

DatabaseCommitClientSubscriptionFunction   DatabaseObjectManager::m_pDatabaseCommitClientSubscriptionFunction   = NULL;
PrismMutex                                 DatabaseObjectManager::m_DatabaseCommitClientSubscriptionFunctionMutex;

void DatabaseObjectManager::setIsDatabaseEnabled (const bool &isDatabaseEnabled)
{
    s_isDatabaseEnabledMutex.lock ();
    s_isDatabaseEnabled = isDatabaseEnabled;
    s_isDatabaseEnabledMutex.unlock ();
}

bool DatabaseObjectManager::getIsDatabaseEnabled ()
{
    bool isDatabaseEnabled = false;

    s_isDatabaseEnabledMutex.lock ();
    isDatabaseEnabled = s_isDatabaseEnabled;
    s_isDatabaseEnabledMutex.unlock ();

    return (isDatabaseEnabled);
}

void DatabaseObjectManager::setIsDatabaseConnected (bool isDatabaseConnected)
{
    s_isDatabaseConnected = isDatabaseConnected;
}

bool DatabaseObjectManager::getIsDatabaseConnected ()
{
    return (s_isDatabaseConnected);
}

void DatabaseObjectManager::setDatabasePort (const SI32 &databasePort)
{
    s_databasePortMutex.lock ();
    s_databasePort = databasePort;
    s_databasePortMutex.unlock ();
}

SI32 DatabaseObjectManager::getDatabasePort ()
{
    SI32 databasePort = 0;

    s_databasePortMutex.lock ();
    databasePort = s_databasePort;
    s_databasePortMutex.unlock ();

    return (databasePort);
}

UI32 DatabaseObjectManager::getEmptyDatabaseType ()
{
    UI32 emptyDatabaseType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION;

    s_databaseEmptyTypeMutex.lock ();
    emptyDatabaseType = s_databaseEmptyType;
    s_databaseEmptyTypeMutex.unlock ();

    return emptyDatabaseType;
}

void DatabaseObjectManager::setEmptyDatabaseType (const string &emptyType)
{
    UI32 emptyDatabaseType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION;

    if ("delete" == emptyType)
    {
        emptyDatabaseType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE;
    }
    else if ("truncate" == emptyType)
    {
        emptyDatabaseType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_TRUNCATE;
    }
    else
    {
        emptyDatabaseType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION;
    }

    s_databaseEmptyTypeMutex.lock ();
    s_databaseEmptyType = emptyDatabaseType;
    s_databaseEmptyTypeMutex.unlock ();
}

UI32 DatabaseObjectManager::getEmptyDatabaseAutoDetectionThresholdValue ()
{
    UI32 thresholdValue = 75000;

    s_databaseEmptyTypeAutoDetectionThresholdValueMutex.lock ();
    thresholdValue = s_databaseEmptyTypeAutoDetectionThresholdValue;
    s_databaseEmptyTypeAutoDetectionThresholdValueMutex.unlock ();

    return thresholdValue;
}

void DatabaseObjectManager::setEmptyDatabaseAutoDetectionThresholdValue (const UI32 &thresholdValue)
{
    s_databaseEmptyTypeAutoDetectionThresholdValueMutex.lock ();
    s_databaseEmptyTypeAutoDetectionThresholdValue = thresholdValue;
    s_databaseEmptyTypeAutoDetectionThresholdValueMutex.unlock ();
}

string DatabaseObjectManager::getDatabaseName ()
{
    string databaseName;

    s_databaseNameMutex.lock ();
    databaseName = s_databaseName;
    s_databaseNameMutex.unlock ();

    return (databaseName);
}

string DatabaseObjectManager::getDatabaseDirectory ()
{
    return (m_databaseDirectory);
}

void DatabaseObjectManager::setDatabaseLogFileName (const string &databaseLogFileName)
{
    s_databaseLogFileNameMutex.lock ();
    s_databaseLogFileName = databaseLogFileName;
    s_databaseLogFileNameMutex.unlock ();
}

string DatabaseObjectManager::getDatabaseLogFileName ()
{
    string databaseLogFileName;

    s_databaseLogFileNameMutex.lock ();
    databaseLogFileName = s_databaseLogFileName;
    s_databaseLogFileNameMutex.unlock ();
    
    return (databaseLogFileName);
}

void DatabaseObjectManager::setDatabaseSharedBuffers (const string &databaseSharedBuffers)
{
    s_databaseSharedBuffersMutex.lock ();
    s_databaseSharedBuffers = databaseSharedBuffers;
    s_databaseSharedBuffersMutex.unlock ();
}

string DatabaseObjectManager::getDatabaseSharedBuffers ()
{
    string databaseSharedBuffers;

    s_databaseSharedBuffersMutex.lock ();
    databaseSharedBuffers = s_databaseSharedBuffers;
    s_databaseSharedBuffersMutex.unlock ();

    return (databaseSharedBuffers);
}

string DatabaseObjectManager::getDatabaseLogStringForPgctl ()
{
    string databaseLogString = "";

    if (true == getIsDatabaseLogEnabled ())
    {
        databaseLogString = " -l " + getDatabaseLogFileName ();
    }
    else
    {
        databaseLogString = "";
    }
    
    return (databaseLogString);
}

void DatabaseObjectManager::setIsDatabaseLogEnabled (const bool isDatabaseLogEnabled)
{
    s_isDatabaseLogEnabledMutex.lock ();
    s_isDatabaseLogEnabled = isDatabaseLogEnabled;
    s_isDatabaseLogEnabledMutex.unlock ();
}

bool DatabaseObjectManager::getIsDatabaseLogEnabled ()
{
    bool isDatabaseLogEnabled = false;

    s_isDatabaseLogEnabledMutex.lock ();
    isDatabaseLogEnabled = s_isDatabaseLogEnabled;
    s_isDatabaseLogEnabledMutex.unlock ();

    return (isDatabaseLogEnabled);
}

string DatabaseObjectManager::getWaveDatabaseInstallCompletionFileName ()
{
    return (m_databaseInstallCompletionFileName);
}

DatabaseObjectManager::DatabaseObjectManager ()
    : WaveLocalObjectManager (getClassName ())
{
    DatabaseObjectManagerExecuteQueryMessage::setDatabaseObjectManagerServiceId (getServiceId ());

    m_pDatabaseObjectManagerInitializeWorker = new DatabaseObjectManagerInitializeWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerInitializeWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerInstallWorker = new DatabaseObjectManagerInstallWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerInstallWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerBootWorker = new DatabaseObjectManagerBootWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerBootWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerExecuteTransactionWorker = new DatabaseObjectManagerExecuteTransactionWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerExecuteTransactionWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerExecuteQueryWorker = new DatabaseObjectManagerExecuteQueryWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerExecuteQueryWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerExecuteCopySchemaWorker = new DatabaseObjectManagerExecuteCopySchemaWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerExecuteCopySchemaWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerExecuteCursorCommandWorker = new DatabaseObjectManagerExecuteCursorCommandWorker(this);

    prismAssert (NULL != m_pDatabaseObjectManagerExecuteCursorCommandWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerGetLatestInstanceIdForTableWorker = new DatabaseObjectManagerGetLatestInstanceIdForTableWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerGetLatestInstanceIdForTableWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerBackupWorker = new DatabaseObjectManagerBackupWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerBackupWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerEmptyWorker = new DatabaseObjectManagerEmptyWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerEmptyWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerRestoreWorker = new DatabaseObjectManagerRestoreWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerRestoreWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerCleanPreparedTransactionsWorker = new DatabaseObjectManagerCleanPreparedTransactionsWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerCleanPreparedTransactionsWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerCalculateDbCksumWorker = new DatabaseObjectManagerCalculateDbCksumWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerCleanPreparedTransactionsWorker, __FILE__, __LINE__);
    
    m_pDatabaseObjectManagerShutdownWorker = new DatabaseObjectManagerShutdownWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerShutdownWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerGetDbAccessDetailsWorker = new DatabaseObjectManagerGetDbAccessDetailsWorker (this);

    prismAssert (NULL != m_pDatabaseObjectManagerGetDbAccessDetailsWorker, __FILE__, __LINE__);

    addOperationMap (DATABASE_OBJECT_MANAGER_CREATE_INMEMORY_MO, reinterpret_cast<PrismMessageHandler> (&DatabaseObjectManager::createInMemoryManagedObjectHandler));

    addDebugFunction ((ShellCmdFunction) (&DatabaseObjectManager::debugPsql), "psql");

}

WaveObjectManager *DatabaseObjectManager::getInstance ()
{
    static WaveObjectManager          *pWaveObjectManager          = NULL;
    static DatabaseObjectManager      *pDatabaseObjectManager      = NULL;
    static MultiDatabaseObjectManager *pMultiDatabaseObjectManager = NULL;

    if (true == (MultiDatabaseToolKit::getIsMultiModeEnabled ()))
    {
        if (NULL == pMultiDatabaseObjectManager)
        {
            pMultiDatabaseObjectManager = new MultiDatabaseObjectManager ();
            WaveNs::prismAssert (NULL != pMultiDatabaseObjectManager, __FILE__, __LINE__);
        }

        pWaveObjectManager = pMultiDatabaseObjectManager;
    }
    else
    {
        if (NULL == pDatabaseObjectManager)
        {
            pDatabaseObjectManager = new DatabaseObjectManager ();
            WaveNs::prismAssert (NULL != pDatabaseObjectManager, __FILE__, __LINE__);
        }

        pWaveObjectManager = pDatabaseObjectManager;
    }

    return (pWaveObjectManager);
}

DatabaseObjectManager::~DatabaseObjectManager ()
{
    if (NULL != m_pDatabaseObjectManagerInitializeWorker)
    {
        delete m_pDatabaseObjectManagerInitializeWorker;
        m_pDatabaseObjectManagerInitializeWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerInstallWorker)
    {
        delete m_pDatabaseObjectManagerInstallWorker;
        m_pDatabaseObjectManagerInstallWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerBootWorker)
    {
        delete m_pDatabaseObjectManagerBootWorker;
        m_pDatabaseObjectManagerBootWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerExecuteTransactionWorker)
    {
        delete m_pDatabaseObjectManagerExecuteTransactionWorker;
        m_pDatabaseObjectManagerExecuteTransactionWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerExecuteQueryWorker)
    {
        delete m_pDatabaseObjectManagerExecuteQueryWorker;
        m_pDatabaseObjectManagerExecuteQueryWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerExecuteCopySchemaWorker)
    {
        delete m_pDatabaseObjectManagerExecuteCopySchemaWorker;
        m_pDatabaseObjectManagerExecuteCopySchemaWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerExecuteCursorCommandWorker)
    {   
        delete m_pDatabaseObjectManagerExecuteCursorCommandWorker;
        m_pDatabaseObjectManagerExecuteCursorCommandWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerGetLatestInstanceIdForTableWorker)
    {
        delete m_pDatabaseObjectManagerGetLatestInstanceIdForTableWorker;
        m_pDatabaseObjectManagerGetLatestInstanceIdForTableWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerBackupWorker)
    {
        delete m_pDatabaseObjectManagerBackupWorker;
        m_pDatabaseObjectManagerBackupWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerEmptyWorker)
    {
        delete m_pDatabaseObjectManagerEmptyWorker;
        m_pDatabaseObjectManagerEmptyWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerRestoreWorker)
    {
        delete m_pDatabaseObjectManagerRestoreWorker;
        m_pDatabaseObjectManagerRestoreWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerCleanPreparedTransactionsWorker)
    {
        delete m_pDatabaseObjectManagerCleanPreparedTransactionsWorker;
    }
    
    if (NULL != m_pDatabaseObjectManagerCalculateDbCksumWorker)
    {
        delete m_pDatabaseObjectManagerCalculateDbCksumWorker;
        m_pDatabaseObjectManagerCalculateDbCksumWorker = NULL;
    }
    
    if (NULL != m_pDatabaseObjectManagerShutdownWorker)
    {
        delete m_pDatabaseObjectManagerShutdownWorker;
        m_pDatabaseObjectManagerShutdownWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerGetDbAccessDetailsWorker)
    {
        delete m_pDatabaseObjectManagerGetDbAccessDetailsWorker;
        m_pDatabaseObjectManagerGetDbAccessDetailsWorker = NULL;
    }
}

PrismServiceId DatabaseObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string DatabaseObjectManager::getClassName ()
{
    return ("Wave Database");
}

void DatabaseObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    // We do not have to do any thing here.  All the work is done in the install for m_pDatabaseObjectManagerInstallWorker.

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void DatabaseObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    // We do not have to do any thing here.  All the work is done in the boot for m_pDatabaseObjectManagerBootWorker.

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void DatabaseObjectManager::debugPsql (UI32 argc, vector<string> argv)
{
    string commandString = string ("psql -p ") + (DatabaseObjectManager::getDatabasePort ()) + string (" ") + DatabaseObjectManager::getDatabaseName ();

    system (commandString.c_str ());
}

WaveManagedObject *DatabaseObjectManager::loadWaveManagedObjectFromDatabase (const ObjectId &waveManagedObjectObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return (m_pDatabaseObjectManagerExecuteQueryWorker->loadWaveManagedObjectFromDatabase (waveManagedObjectObjectId, schema, pWaveObjectManager));
}

WaveManagedObject *DatabaseObjectManager::loadWaveManagedObjectFromDatabaseWrapper (const ObjectId &waveManagedObjectObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    if (true == (MultiDatabaseToolKit::getIsMultiModeEnabled ()))
    {
        return ((MultiDatabaseObjectManager::getInstance ())->loadWaveManagedObjectFromDatabase (waveManagedObjectObjectId, schema, pWaveObjectManager));
    }
    else
    {
        DatabaseObjectManager *pDatabaseObjectManager = dynamic_cast<DatabaseObjectManager *>(DatabaseObjectManager::getInstance ());

        WaveNs::prismAssert (NULL != pDatabaseObjectManager, __FILE__, __LINE__);

        return (pDatabaseObjectManager->loadWaveManagedObjectFromDatabase (waveManagedObjectObjectId, schema, pWaveObjectManager));
    }
}

vector<WaveManagedObject *> DatabaseObjectManager::loadWaveManagedObjectsFromDatabase (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return (m_pDatabaseObjectManagerExecuteQueryWorker->loadWaveManagedObjectsFromDatabase (waveManagedObjectObjectIds, schema, pWaveObjectManager));
}

vector<WaveManagedObject *> DatabaseObjectManager::loadWaveManagedObjectsFromDatabaseWrapper (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    if (true == (MultiDatabaseToolKit::getIsMultiModeEnabled ()))
    {
        return ((MultiDatabaseObjectManager::getInstance ())->loadWaveManagedObjectsFromDatabase (waveManagedObjectObjectIds, schema, pWaveObjectManager));
    }
    else
    {
        DatabaseObjectManager *pDatabaseObjectManager = dynamic_cast<DatabaseObjectManager *>(DatabaseObjectManager::getInstance ());

        WaveNs::prismAssert (NULL != pDatabaseObjectManager, __FILE__, __LINE__);

        return (pDatabaseObjectManager)->loadWaveManagedObjectsFromDatabase (waveManagedObjectObjectIds, schema, pWaveObjectManager);
    }
}

vector<WaveManagedObject *> DatabaseObjectManager::loadWaveManagedObjectsFromDatabase (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return (m_pDatabaseObjectManagerExecuteQueryWorker->loadWaveManagedObjectsFromDatabase (childClassName, parentObjectId, schema, pWaveObjectManager));
}

vector<WaveManagedObject *> DatabaseObjectManager::loadWaveManagedObjectsFromDatabaseWrapper (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    if (true == (MultiDatabaseToolKit::getIsMultiModeEnabled ()))
    {
        return ((MultiDatabaseObjectManager::getInstance ())->loadWaveManagedObjectsFromDatabase (childClassName, parentObjectId, schema, pWaveObjectManager));
    }
    else
    {
        DatabaseObjectManager *pDatabaseObjectManager = dynamic_cast<DatabaseObjectManager *>(DatabaseObjectManager::getInstance ());

        WaveNs::prismAssert (NULL != pDatabaseObjectManager, __FILE__, __LINE__);

        return (pDatabaseObjectManager->loadWaveManagedObjectsFromDatabase (childClassName, parentObjectId, schema, pWaveObjectManager));
    }
}

void DatabaseObjectManager::executeDatabaseCommitClientSubscriptionFunction (const bool &commitHappenedFlag)
{
    m_DatabaseCommitClientSubscriptionFunctionMutex.lock ();

    if (NULL != m_pDatabaseCommitClientSubscriptionFunction)
    {
        (*m_pDatabaseCommitClientSubscriptionFunction) (commitHappenedFlag);
    }

    m_DatabaseCommitClientSubscriptionFunctionMutex.unlock ();
}

void DatabaseObjectManager::setDatabaseCommitClientSubscriptionFunction (DatabaseCommitClientSubscriptionFunction databaseCommitClientSubscriptionFunction)
{
    m_DatabaseCommitClientSubscriptionFunctionMutex.lock ();

    m_pDatabaseCommitClientSubscriptionFunction = databaseCommitClientSubscriptionFunction;

    m_DatabaseCommitClientSubscriptionFunctionMutex.unlock ();
}

bool DatabaseObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
{
    switch (operationCode)
    {
        case DATABASE_OBJECT_MANAGER_EXECUTE_QUERY:
        case WAVE_OBJECT_MANAGER_FIPS_ZEROIZE:
        case DATABASE_OBJECT_MANAGER_STOP:
            return (true);
            break;
        default :
            return (WaveObjectManager::isOperationAllowedBeforeEnabling (operationCode));
            break;
    }
}

ResourceId DatabaseObjectManager::createNewWaveDatabase ()
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManager::createNewWaveDatabase : Creating Wave Database...");

    const string        completionFilePath   = m_databaseDirectory + "/" + m_databaseInstallCompletionFileName;
          ifstream      completionFile       (completionFilePath.c_str (), ios::in);
          SI32          returnStatus         = -1; 
          ResourceId    status               = WAVE_MESSAGE_SUCCESS;  
    
    if (true == (completionFile.fail ()))
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "DatabaseObjectManager::createNewWaveDatabase : Database was never installed. Creating a fresh database."); 
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "DatabaseObjectManager::createNewWaveDatabase : Database is already installed. Stopping the database before creating fresh database.");

        string commandString = string ("pg_ctl -D ") + m_databaseDirectory + string (" -o \" -p ") + getDatabasePort () + "\" -m immediate stop > /dev/null";
        returnStatus = system (commandString.c_str());

        if (0 != returnStatus)
        {
            WaveNs::trace (TRACE_LEVEL_WARN, string("DatabaseObjectManager::createNewWaveDatabase : failed to stop the database. status:") + returnStatus);
        }
        completionFile.close ();
    }

    // Clean-up the old WaveDatabase
    string tmpDatabaseDirectory = m_databaseDirectory + ".old";
    returnStatus = system ((string ("/bin/mv ") + m_databaseDirectory + string(" ") + tmpDatabaseDirectory).c_str());

    if (0 != returnStatus)
    {
        WaveNs::trace (TRACE_LEVEL_WARN, string("DatabaseObjectManager::createNewWaveDatabase : failed to rename the file. status:") + returnStatus);
    }

    returnStatus = system ((string ("/bin/rm -rf ") + tmpDatabaseDirectory).c_str ());

    if (0 != returnStatus)
    {
        WaveNs::trace (TRACE_LEVEL_WARN, string("DatabaseObjectManager::createNewWaveDatabase : failed to delete the WaveDatabase. status:") + returnStatus);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_DEBUG, string("DatabaseObjectManager::createNewWaveDatabase : successfully deleted ") + tmpDatabaseDirectory);
    }
    
    // Initialize a Database
    returnStatus = system ((string ("initdb -A trust -D ") + m_databaseDirectory + string (" > /dev/null")).c_str ());

    if (0 != returnStatus)
    {
        WaveNs::trace (TRACE_LEVEL_WARN, string("DatabaseObjectManager::createNewWaveDatabase : failed to initialize Database. status:") + returnStatus);
        status = WAVE_MESSAGE_ERROR_INIT_DATABASE_FAILED;
    }

    prismSleep (20);
    // start the database
    WaveNs::trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManager::createNewWaveDatabase : Booting the database to create Prism Database...");


    UI8     retries       = 30;
    SI32    cmdStatus     = -1;  
    string  commandString = string ("pg_ctl -D ") + DatabaseObjectManager::getDatabaseDirectory () + DatabaseObjectManager::getDatabaseLogStringForPgctl () + string (" -w -o \" -F --shared_buffers=") + DatabaseObjectManager::getDatabaseSharedBuffers () + string (" --max_stack_depth=7680kB --checkpoint_segments=1 --max_prepared_transactions='8'  --max_locks_per_transaction='200' --log_line_prefix='%t %a [%e] ' -p ") + DatabaseObjectManager::getDatabasePort () + "\" start > /dev/null";

    for (UI8 i = 0; i < retries; i++)
    {
        vector<string>  output;
                        cmdStatus = FrameworkToolKit::systemCommandOutput (commandString.c_str (), output);

        if (0 == cmdStatus)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, "DatabaseObjectManager::createNewWaveDatabase : Done");
            break;
        }
 
        WaveNs::trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::createNewWaveDatabase : failed to start postgres server. -");
        if (0 < output.size())
        {
            WaveNs::tracePrintf (TRACE_LEVEL_WARN, false, true, output[0].c_str());
        }
        i++;
        prismSleep (4);
    }

    if (0 != cmdStatus)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_WARN, true, false, string("DatabaseObjectManager::createNewWaveDatabase : failed to start postgres server after [%s] retries also. Can't move forward. Asserting.").c_str(), retries);
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    prismSleep (10);

    // create a database    
    WaveNs::trace (TRACE_LEVEL_DEBUG, "DatabaseObjectManager::createNewWaveDatabase : Creating Prism Database...");

    commandString.clear ();    
    commandString = string ("createdb ") + getDatabaseName () + " -p " + getDatabasePort ();
    returnStatus = system (commandString.c_str ());

    if (0 != returnStatus)
    {   
        WaveNs::trace (TRACE_LEVEL_WARN, string("DatabaseObjectManager::createNewWaveDatabase : failed to create Database. Status : ") + returnStatus);
        status = WAVE_MESSAGE_ERROR_CREATE_DATABASE_FAILED;
    }

    // create a database install completion file
    ofstream completionfileNew;
    completionfileNew.open (completionFilePath.c_str ());
    completionfileNew.close ();
    completionfileNew.clear ();

    return (status);
}

string DatabaseObjectManager::getDBCorruptIdentifierFileName ()
{
    return (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + m_databaseCorruptIdentifierFileName);    
}

bool DatabaseObjectManager::isErrorDueToCorruption (const string& errorString)
{
    string lowercaseError = errorString;

    transform(lowercaseError.begin(), lowercaseError.end(), lowercaseError.begin(), StringUtils::getLowerCase);

    for (UI32 i = 0; i < NUM_OF_CORRUPTION_IDENTIFIERS; i++)
    {
        if (string::npos != lowercaseError.find (s_databaseCorruptionIdentifiers[i]))
        {
            return (true);
        }
    }
            
    return (false);
}

void DatabaseObjectManager::handleDatabaseCorruption ()
{
    // 1. Add a RAS log (if possible) that db corruption is encountered and system will undergo reboot for recovery.
    // 2. clean up the existing db directory WaveDatabase And recreate the same. Deleting the file getWaveDatabaseInstallCompletionFileName() 
    //    will ensure that db is deleted and recreated during boot install phase.
    // 3. Delete *.cfg and *.cfg.bk file so that the node will come up with FIRST TIME BOOT  next time and will take care of config replay.
    // 4. touch a corruption identifier file. This will help in identifying that the last reboot was because of db corruption recovery.
    // 5. reboot

    // 1.
    WaveNs::trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleDatabaseCorruption: Encountered database corruption. Triggered Auto recovery.");

    // 2.
    vector<string>  output;
    SI32            cmdStatus = 0;

    const string completionFilePath = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + getDatabaseDirectory() + "/" + getWaveDatabaseInstallCompletionFileName ();

    WaveNs::trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleDatabaseCorruption: deleting file " + completionFilePath);

    cmdStatus = FrameworkToolKit::systemCommandOutput ((string ("/bin/rm -rf ") + completionFilePath).c_str(), output);

    if ( cmdStatus != 0 )
    {   
        WaveNs::trace (TRACE_LEVEL_ERROR, string("DatabaseObjectManager::handleDatabaseCorruption: cmd to deleted file ")+ completionFilePath + " failed with error message : " + output[0]);
    }

    // 3.
    const string prismConfigurationfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
    const string prismConfigurationBackupfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationBackupFileName ();

    WaveNs::trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleDatabaseCorruption: deleting files " + prismConfigurationfileName + " and " + prismConfigurationBackupfileName);

    cmdStatus = 0;
    output.clear();

    cmdStatus = FrameworkToolKit::systemCommandOutput ((string ("/bin/rm -rf ") + prismConfigurationfileName + string (" ") + prismConfigurationBackupfileName).c_str(), output);

    if ( cmdStatus != 0 )
    {        
        WaveNs::trace (TRACE_LEVEL_ERROR, string("DatabaseObjectManager::handleDatabaseCorruption: cmd to delete files ")+ prismConfigurationfileName + string (", ") + prismConfigurationBackupfileName + string(" failed with error message : ") + output[0]);
    }

    //4. 
    const string dbCorruptIdentifierFile = getDBCorruptIdentifierFileName ();

    WaveNs::trace (TRACE_LEVEL_INFO, "DatabaseObjectManager::handleDatabaseCorruption: generating file " + dbCorruptIdentifierFile);

    cmdStatus = 0;
    output.clear();

    cmdStatus = FrameworkToolKit::systemCommandOutput ((string ("/bin/touch ") + dbCorruptIdentifierFile).c_str(), output);

    if (cmdStatus != 0)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string("DatabaseObjectManager::handleDatabaseCorruption: cmd to generate file ") + dbCorruptIdentifierFile + "failed with error message : " + output[0]);
    }

//    WaveNs::trace (TRACE_LEVEL_INFO, "DatabaseObjectManager::handleDatabaseCorruption : Beginning System Call SYNC");
//    sync ();
//    WaveNs::trace (TRACE_LEVEL_INFO, "DatabaseObjectManager::handleDatabaseCorruption : Ending System Call SYNC");

    // 5. 
    Wave::logOperationStatus (REBOOT_FOR_DB_CORRUPTION_AUTO_RECOVERY);
    prismSleep(10);

    WaveNs::trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleDatabaseCorruption: System going down for auto db recovery.");

    FrameworkToolKit::systemCommandOutput ("/fabos/sbin/reboot -s -r AutoRecoveryForDBCorruption", output);
    prismSleep(20);
}

void DatabaseObjectManager::handleIfDBCorruption (string &databaseErrorMessage, PGresult *pResult)
{
    if (false == FrameworkToolKit::getHandleDBCorruptionFlag ())
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "DatabaseObjectManager::handleIfDBCorruption: DB Corruption auto-recovery is disabled.");
        return;
    }

    bool databaseCorrupted          = false;
    if (true == databaseErrorMessage.empty())
    {
        if (pResult)
        {
            databaseErrorMessage     = string(PQresultErrorMessage (pResult));
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "DatabaseObjectManager::handleIfDBCorruption: ErrorMessage is empty and PGresult is missing.");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }

    bool isDBErrorDueToCorruption   = isErrorDueToCorruption (databaseErrorMessage);
    if (true == isDBErrorDueToCorruption)
    {   
        databaseCorrupted = true;
    }
    else
    {
        string cmd = "tail -5 " + DatabaseObjectManager::getDatabaseLogFileName () + " | grep -i warning";

        vector<string> cmdOutput;
        FrameworkToolKit::systemCommandOutput (cmd, cmdOutput);

        for (UI32 i = 0; i < cmdOutput.size (); i++)
        {
            string warningMessage = cmdOutput[i];
            WaveNs::trace (TRACE_LEVEL_WARN, "DatabaseConnection::handleIfDBCorruption : cmdOutput = " + warningMessage);
            if (false == warningMessage.empty())
            {
                bool isWarningForCorruption = isErrorDueToCorruption (warningMessage);
                WaveNs::tracePrintf (TRACE_LEVEL_INFO, true, false, "DatabaseConnection::handleIfDBCorruption : isWarningForCorruption : ", isWarningForCorruption);
                if (isWarningForCorruption)
                {
                    databaseCorrupted = true;
                    break;
                }
            }
        }
    }

    if (pResult)
    {
        char* returnState = PQresultErrorField (pResult, PG_DIAG_SQLSTATE);
        if (NULL != returnState)
        {
            WaveNs::trace (TRACE_LEVEL_WARN, string("DatabaseConnection::handleIfDBCorruption : SQLSTATE :\n") + returnState);
            /* Treat database as corrupted if the returned SQLSTATE is 'XX*' */
            /*
                Postgres Documentation says 
                Class XX  - Internal Error
                XX000     - internal_error
                XX001     - data_corrupted
                XX002     - index_corrupted
            */
            if (strlen(returnState) >= 2 && returnState[0] == 'X' && returnState[1] == 'X')
            {
                databaseCorrupted = true;
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_DEBUG, string("DatabaseConnection::handleIfDBCorruption : No SQLSTATE provided by postgres."));
        }
    }

    if (databaseCorrupted)
    {

        WaveNs::trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleIfDBCorruption: failure in operation is due to database corruption. Handling auto recovery of the database.");
        handleDatabaseCorruption ();
    }
    else
    {
        WaveNs::tracePrintf (TRACE_LEVEL_INFO, true, false, "DatabaseObjectManager::handleIfDBCorruption: failure is not because of database corruption. ErrorMessage : \n%s", databaseErrorMessage.c_str());
    }
}
        
bool DatabaseObjectManager::getIsDatabaseShuttingDownOnReboot ()
{
    bool isDatabaseShuttingDownOnReboot = false;

    s_isDatabaseShuttingDownOnRebootMutex.lock ();
    isDatabaseShuttingDownOnReboot = s_isDatabaseShuttingDownOnReboot;
    s_isDatabaseShuttingDownOnRebootMutex.unlock ();

    return (isDatabaseShuttingDownOnReboot);
}


void  DatabaseObjectManager::setIsDatabaseShuttingDownOnReboot (const bool &isDatabaseShuttingDownOnReboot)
{
    s_isDatabaseShuttingDownOnRebootMutex.lock ();
    s_isDatabaseShuttingDownOnReboot = isDatabaseShuttingDownOnReboot;
    s_isDatabaseShuttingDownOnRebootMutex.unlock ();
}

void DatabaseObjectManager::goOnInfiniteLoopBeforeReboot ()
{
    UI32 numberOfIterations = 0;

    while (1) 
    {
        if (numberOfIterations < 300)
        {
            ++numberOfIterations;
            prismSleep (1);
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, "DatabaseObjectManager::goOnInfiniteLoopBeforeReboot : 5 minutes after reload/reboot command has been given and system hasnt rebooted. So Dcmd is forcefully asserting thus causing the system to reboot");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void DatabaseObjectManager::createInMemoryManagedObjectHandler (DatabaseObjectManagerCreateInMemoryManagedObjectMessage *pDatabaseObjectManagerCreateInMemoryManagedObjectMessage)
{
    PrismServiceId    prismServiceId;
    string            className           = pDatabaseObjectManagerCreateInMemoryManagedObjectMessage->getClassName ();
    WaveManagedObject *pWaveManagedObject = NULL;
             
    trace (TRACE_LEVEL_DEVEL, "DatabaseObjectManager::createInMemoryManagedObjectHandler");
                  
    WaveObjectManager *pWaveObjectManager = WaveObjectManager::getOwnerForManagedClass (className);
    prismServiceId = pWaveObjectManager->getServiceId();
                           
    pWaveManagedObject = WaveManagedObjectFactory::getWaveManagedObjectInstance (prismServiceId, className);
    prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

    pWaveManagedObject->setPCurrentOwnerWaveObjectManager (pDatabaseObjectManagerCreateInMemoryManagedObjectMessage->getPWaveObjectManager ());
                                    
    pDatabaseObjectManagerCreateInMemoryManagedObjectMessage->setInMemoryMO (pWaveManagedObject);
                                         
    pDatabaseObjectManagerCreateInMemoryManagedObjectMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                                              
    reply (pDatabaseObjectManagerCreateInMemoryManagedObjectMessage);
}

UI32 DatabaseObjectManager::getTotalNumberOfEntriesInAllTableFromDatabase (string &waveschema)
{
    vector<string> sqlStrings;
    UI32           totalEntriesInDatabase = 0; 
    UI32           numberOfSqls           = 0;
    OrmRepository *pOrmRepository         = OrmRepository::getInstance ();

    pOrmRepository->getAllSqlsToCountAllEntriesInDatabase (sqlStrings, waveschema);

    numberOfSqls = sqlStrings.size ();
    
    for (UI32 i = 0; i < numberOfSqls; i++) 
    {    
        UI32 count;

        m_pDatabaseObjectManagerExecuteQueryWorker->getCountFromDatabaseTableForAGivenSql (sqlStrings [i], count);

        totalEntriesInDatabase += count;
    }    

    return (totalEntriesInDatabase);
}

/*
 * This method takes the schemas where empty has to be done.
 * Also, it takes user configured empty type
 * Based on the count of DB entries, the method will return the appropriate DB empty type.
 */
UI32 DatabaseObjectManager::determineDatabaseEmptyType (set<string> schemasToBeEmptied, UI32 emptyTypeConfigured)
{
    DatabaseObjectManager  *pDatabaseObjectManager          = dynamic_cast<DatabaseObjectManager *> (DatabaseObjectManager::getInstance ());
    UI32                    emptyType                       = emptyTypeConfigured;
    string                  waveSchema                      = OrmRepository::getWaveCurrentSchema ();
    UI32                    numberOfDistributedLogEntries   = 0;

    if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_NOT_DEFINED == emptyType)
    {
        emptyType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION;
    }

    if (DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION == emptyType)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManager::determineDatabaseEmptyType : Using Auto Detection"));

        UI32 numberOfEntriesThresholdValue = DatabaseObjectManager::getEmptyDatabaseAutoDetectionThresholdValue ();

        UI32 numberOfEntriesInDB = 0;

        if (schemasToBeEmptied.empty ())
        {
            OrmRepository *pOrmRepository                   = OrmRepository::getInstance ();
            string         sqlToCountDistributedLogEntries;
            UI32           count                            = 0;   

            pOrmRepository->getSqlToCountNumberOfEntriesInTable (DistributedLogEntryManagedObject::getClassName (), waveSchema, sqlToCountDistributedLogEntries);

            (pDatabaseObjectManager->m_pDatabaseObjectManagerExecuteQueryWorker)->getCountFromDatabaseTableForAGivenSql (sqlToCountDistributedLogEntries, count);

            numberOfDistributedLogEntries += count;
 
            numberOfEntriesInDB = pDatabaseObjectManager->getTotalNumberOfEntriesInAllTableFromDatabase (waveSchema);
        }
        else
        {
            set<string>::iterator it;
            
            for (it = schemasToBeEmptied.begin (); it != schemasToBeEmptied.end (); it++)
            {
                waveSchema = *it;

                OrmRepository *pOrmRepository                   = OrmRepository::getInstance ();
                string         sqlToCountDistributedLogEntries;
                UI32           count                            = 0;   

                pOrmRepository->getSqlToCountNumberOfEntriesInTable (DistributedLogEntryManagedObject::getClassName (), waveSchema, sqlToCountDistributedLogEntries);

                (pDatabaseObjectManager->m_pDatabaseObjectManagerExecuteQueryWorker)->getCountFromDatabaseTableForAGivenSql (sqlToCountDistributedLogEntries, count);

                numberOfDistributedLogEntries += count;

                numberOfEntriesInDB += pDatabaseObjectManager->getTotalNumberOfEntriesInAllTableFromDatabase (waveSchema);
            }
        }

        WaveNs::trace (TRACE_LEVEL_INFO, string ("DatabaseObjectManager::determineDatabaseEmptyType : Current Threshold Value is ") + numberOfEntriesThresholdValue + string (" number of entries in Database are ") + numberOfEntriesInDB + string (" number of distributed log entry objects ") + numberOfDistributedLogEntries);

        if ((numberOfEntriesInDB - numberOfDistributedLogEntries) >= numberOfEntriesThresholdValue)
        {
            emptyType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_TRUNCATE;
        } 
        else
        {
            emptyType = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE;
        }
    }

    return emptyType;
}

}
