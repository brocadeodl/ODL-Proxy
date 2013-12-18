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

#ifndef MULTIDATABASEOBJECTMANAGER_H
#define MULTIDATABASEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

namespace WaveNs
{

class WaveAsynchronousContextForBootPhases;
class DatabaseObjectManagerExecuteQueryWorker;
class DatabaseObjectManagerExecuteCursorCommandWorker;
class MultiDatabaseObjectManagerSetupServersWorker;

class MultiDatabaseObjectManager : public WaveLocalObjectManager
{

    private :
                     MultiDatabaseObjectManager ();
        virtual void boot                       (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
        virtual                             ~MultiDatabaseObjectManager                ();

        static  MultiDatabaseObjectManager  *getInstance                               ();
        static  PrismServiceId               getPrismServiceId                         ();
        static  string                       getClassName                              ();

                WaveManagedObject           *loadWaveManagedObjectFromDatabase         (const ObjectId         &waveManagedObjectObjectId,  const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
                vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabase        (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
                vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabase        (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        static  WaveManagedObject           *loadWaveManagedObjectFromDatabaseWrapper  (const ObjectId         &waveManagedObjectObjectId,  const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        static  vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabaseWrapper (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        static  vector<WaveManagedObject *>  loadWaveManagedObjectsFromDatabaseWrapper (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);


    // Now the data members

    private :
        DatabaseObjectManagerExecuteQueryWorker         *m_pDatabaseObjectManagerExecuteQueryWorker;
        DatabaseObjectManagerExecuteCursorCommandWorker *m_pDatabaseObjectManagerExecuteCursorCommandWorker;
        MultiDatabaseObjectManagerSetupServersWorker    *m_pMultiDatabaseObjectManagerSetupServersWorker;

    protected :
    public :

        friend class DatabaseObjectManager;
};

}

#endif // MULTIDATABASEOBJECTMANAGER_H
