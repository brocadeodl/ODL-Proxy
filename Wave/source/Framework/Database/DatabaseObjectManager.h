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

#ifndef DATABASEOBJECTMANAGER_H
#define DATABASEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Utils/PrismMutex.h"
#include "libpq-fe.h"

namespace WaveNs
{

typedef void (*DatabaseCommitClientSubscriptionFunction)              (const bool &commitHappenedFlag);

class DatabaseObjectManagerInitializeWorker;
class DatabaseObjectManagerInstallWorker;
class DatabaseObjectManagerBootWorker;
class DatabaseObjectManagerExecuteTransactionWorker;
class DatabaseObjectManagerExecuteQueryWorker;
class DatabaseObjectManagerExecuteCopySchemaWorker;
class DatabaseObjectManagerExecuteCursorCommandWorker;
class DatabaseObjectManagerGetLatestInstanceIdForTableWorker;
class DatabaseObjectManagerBackupWorker;
class DatabaseObjectManagerEmptyWorker;
class DatabaseObjectManagerRestoreWorker;
class DatabaseObjectManagerCleanPreparedTransactionsWorker;
class DatabaseObjectManagerCalculateDbCksumWorker;
class DatabaseObjectManagerShutdownWorker;
class WaveAsynchronousContextForBootPhases;
class WaveManagedObject;
class WaveObjectManager;
class DatabaseObjectManagerCreateInMemoryManagedObjectMessage;
class DatabaseObjectManagerGetDbAccessDetailsWorker;

class DatabaseObjectManager : public WaveLocalObjectManager
{
    private :
                                             DatabaseObjectManager              ();
        virtual void                         install                            (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                         boot                               (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);


        static  void                         setIsDatabaseLogEnabled            (const bool isDatabaseLogEnabled);
        static  void                         setDatabasePort                    (const SI32 &databasePort);
        static  void                         setDatabaseLogFileName             (const string &databaseLogFileName);
        static  void                         setDatabaseSharedBuffers           (const string &databaseSharedBuffers);

        static  void                         debugPsql                          (UI32 argc, vector<string> argv);

                bool                         isOperationAllowedBeforeEnabling   (const UI32 &operationCode);

                WaveManagedObject           *loadWaveManagedObjectFromDatabase  (const ObjectId         &waveManagedObjectObjectId,  const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
                vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabase (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
                vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabase (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        static  bool                         isErrorDueToCorruption             (const string& errorString);            
        static  void                         handleDatabaseCorruption           ();

    protected :
    public :
        virtual                            ~DatabaseObjectManager                     ();
        static WaveObjectManager           *getInstance                               ();
        static PrismServiceId               getPrismServiceId                         ();
        static string                       getClassName                              ();
        static bool                         getIsDatabaseEnabled                      ();
        static void                         setIsDatabaseEnabled                      (const bool &isDatabaseEnabled);
        static bool                         getIsDatabaseLogEnabled                   ();
        static SI32                         getDatabasePort                           ();
        static string                       getDatabaseName                           ();
        static string                       getDatabaseDirectory                      ();
        static string                       getDatabaseLogFileName                    ();
        static string                       getDatabaseLogStringForPgctl              ();
        static string                       getDatabaseSharedBuffers                  ();
        static string                       getWaveDatabaseInstallCompletionFileName  ();
        static string                       getDBCorruptIdentifierFileName            ();

        static PrismThreadId                getPrismThreadId                          ();

        static bool                         getIsDatabaseConnected                    ();
        static void                         setIsDatabaseConnected                    (bool isDatabaseConnected);
        static WaveManagedObject           *loadWaveManagedObjectFromDatabaseWrapper  (const ObjectId         &waveManagedObjectObjectId,  const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        static vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabaseWrapper (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        static vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabaseWrapper (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        static void                         executeDatabaseCommitClientSubscriptionFunction (const bool &commitHappenedFlag);
        static void                         setDatabaseCommitClientSubscriptionFunction (DatabaseCommitClientSubscriptionFunction databaseCommitClientSubscriptionFunction);
        static ResourceId                   createNewWaveDatabase                       ();
        static void                         handleIfDBCorruption                        (string& errorString, PGresult *pResult = NULL);
        
        static bool                         getIsDatabaseShuttingDownOnReboot           ();
        static void                         setIsDatabaseShuttingDownOnReboot           (const bool &isDatabaseShuttingDownOnReboot);

        static void                         goOnInfiniteLoopBeforeReboot                ();
               void                         createInMemoryManagedObjectHandler          (DatabaseObjectManagerCreateInMemoryManagedObjectMessage *pDatabaseObjectManagerCreateInMemoryManagedObjectMessage); 

        static UI32                         getEmptyDatabaseType                        ();
        static void                         setEmptyDatabaseType                        (const string &emptyType);

        static UI32                         getEmptyDatabaseAutoDetectionThresholdValue ();
        static void                         setEmptyDatabaseAutoDetectionThresholdValue (const UI32 &thresholdValue);

               UI32                         getTotalNumberOfEntriesInAllTableFromDatabase (string &waveschema);
        static UI32                         determineDatabaseEmptyType                    (set<string> schemasToBeEmptied, UI32 emptyTypeConfigured);
    // Now the data members

    private :
                     DatabaseObjectManagerInitializeWorker                  *m_pDatabaseObjectManagerInitializeWorker;
                     DatabaseObjectManagerInstallWorker                     *m_pDatabaseObjectManagerInstallWorker;
                     DatabaseObjectManagerBootWorker                        *m_pDatabaseObjectManagerBootWorker;
                     DatabaseObjectManagerExecuteTransactionWorker          *m_pDatabaseObjectManagerExecuteTransactionWorker;
                     DatabaseObjectManagerExecuteQueryWorker                *m_pDatabaseObjectManagerExecuteQueryWorker;
                     DatabaseObjectManagerExecuteCopySchemaWorker           *m_pDatabaseObjectManagerExecuteCopySchemaWorker;
                     DatabaseObjectManagerExecuteCursorCommandWorker        *m_pDatabaseObjectManagerExecuteCursorCommandWorker;
                     DatabaseObjectManagerGetLatestInstanceIdForTableWorker *m_pDatabaseObjectManagerGetLatestInstanceIdForTableWorker;
                     DatabaseObjectManagerBackupWorker                      *m_pDatabaseObjectManagerBackupWorker;
                     DatabaseObjectManagerEmptyWorker                       *m_pDatabaseObjectManagerEmptyWorker;
                     DatabaseObjectManagerRestoreWorker                     *m_pDatabaseObjectManagerRestoreWorker;
                     DatabaseObjectManagerCleanPreparedTransactionsWorker   *m_pDatabaseObjectManagerCleanPreparedTransactionsWorker;
                     DatabaseObjectManagerCalculateDbCksumWorker            *m_pDatabaseObjectManagerCalculateDbCksumWorker;
                     DatabaseObjectManagerShutdownWorker                    *m_pDatabaseObjectManagerShutdownWorker;
                     DatabaseObjectManagerGetDbAccessDetailsWorker          *m_pDatabaseObjectManagerGetDbAccessDetailsWorker;

        static const string                                                  m_databaseDirectory;
        static const string                                                  m_databaseLogFilename;
        static const string                                                  m_databaseInstallCompletionFileName;
        static       DatabaseCommitClientSubscriptionFunction                m_pDatabaseCommitClientSubscriptionFunction;
        static       PrismMutex                                              m_DatabaseCommitClientSubscriptionFunctionMutex;
        static const string                                                  m_databaseCorruptIdentifierFileName;

    protected :
    public :

    friend class Wave;
    friend class WaveClient;
    friend class WaveSystemManagement;
};

}

#endif // DATABASEOBJECTMANAGER_H
