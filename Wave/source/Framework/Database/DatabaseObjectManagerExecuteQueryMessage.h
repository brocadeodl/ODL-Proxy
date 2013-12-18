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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DATABASEOBJECTMANAGEREXECUTEQUERYMESSAGE_H
#define DATABASEOBJECTMANAGEREXECUTEQUERYMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "libpq-fe.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class WaveObjectManager;

class DatabaseObjectManagerExecuteQueryMessage : public PrismMessage
{
    private :
    protected :
    public :
                                             DatabaseObjectManagerExecuteQueryMessage (const string &className, WaveObjectManager *pWaveObjectManager, const string &sql, const string &schema);
        virtual                             ~DatabaseObjectManagerExecuteQueryMessage ();

                string                       getClassName                             () const;
                WaveObjectManager           *getPWaveObjectManager                    () const;
                string                       getSql                                   () const;
                string                       getSchema                                () const;
                bool                         getLoadOneToManyRelationships            () const;
                void                         setLoadOneToManyRelationships            (const bool &loadOneToManyRelationships);
                bool                         getLoadCompositions                      () const;
                void                         setLoadCompositions                      (const bool loadCompositions);
                vector<string>               getSelectFields                          () const;
                void                         setSelectFields                          (const vector<string> &selectFields);
                vector<string>               getSelectFieldsInManagedObject           () const;
                void                         setSelectFieldsInManagedObject           (const vector<string> &selectFieldsInManagedObject);
                void                         setPResults                              (vector<WaveManagedObject *> *pResults);
                vector<WaveManagedObject *> *getPResults                              () const;

        static  void                         setDatabaseObjectManagerServiceId        (const PrismServiceId &databaseObjectManagerServiceId);
        static  PrismServiceId               getDatabaseObjectManagerServiceId        ();

    // Now the data members

    private :
               string                       m_className;
               WaveObjectManager           *m_pWaveObjectManager;
               string                       m_sql;
               string                       m_schema;
               vector<string>               m_selectFields;
               vector<string>               m_selectFieldsInManagedObject;
               bool                         m_loadOneToManyRelationships;
               bool                         m_loadCompositions;
               vector<WaveManagedObject *> *m_pResults;

        static PrismServiceId               m_databaseObjectManagerServiceId;

    protected :
    public :
};


class DatabaseObjectManagerDebugChangeQuerySettingsMessage : public ManagementInterfaceMessage
{
    private:
        void    setupAttributesForSerialization                         ();

    public:
                DatabaseObjectManagerDebugChangeQuerySettingsMessage    (const bool &useRecursiveQuery, const UI32 &batchSize);
                DatabaseObjectManagerDebugChangeQuerySettingsMessage    ();
               ~DatabaseObjectManagerDebugChangeQuerySettingsMessage    ();

        bool    getUseRecursiveQuery                                    () const;
        UI32    getBatchSize                                            () const;

    // Now the data members

    private :

        bool    m_useRecursiveQuery;
        UI32    m_batchSize;
};

}

#endif // DATABASEOBJECTMANAGEREXECUTEQUERYMESSAGE_H
