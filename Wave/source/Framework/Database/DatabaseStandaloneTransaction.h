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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Mayur Mahajan                                                *
 ***************************************************************************/

#ifndef DATABASESTANDALONETRANSACTION_H
#define DATABASESTANDALONETRANSACTION_H

#include "Framework/Types/Types.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/ObjectRelationalMapping/DatabaseSchema.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"

#include <string>

using namespace std;

namespace WaveNs
{

class DatabaseStandaloneTransaction
{
    private :
               bool         executeStep                  (const string &sql);

               void         bootDatabaseStep                ();
               PGconn       *getDatabaseConnection       ();
               ResourceId   getTableNamesForWaveSchema      (vector<string>& tableNames);

    protected :
    public :
                            DatabaseStandaloneTransaction   ();
                           ~DatabaseStandaloneTransaction   ();
               bool         execute                         (const string &sql);
               bool         getObjectInstanceIdsForTable    (const string &tableName, vector<UI64>& objectInstanceIds);
               bool         checkIfTableHasEntriesInDb      (const string &tableName, const string &schema, bool &exists);                                                       
               UI32         getSchemaChangeInfo             (DatabaseSchema &databaseSchema);
//               void         populateSchemaUpgradeTableInDb  ();  
//               UI32         getTableIdsFromUpgradeMO        (vector<pair<string, UI32> > &tableIdsFromUpgradeMO);
               void         establishDbConnection           ();
               void         shutdownDatabase                ();
               ResourceId   rollbackDatabase                (); 
               ResourceId   rollbackConfigFile              (PrismFrameworkConfiguration& prismFrameworkConfiguration);
               ResourceId   dropAllDBViews                  ();

    // Now the data members

    private :
            bool            m_needConnectionTeardown;
            UI8             m_bootCount;
	static  PrismMutex      s_standaloneTransactionMutex;
    protected :
    public :
};

}

#endif // DATABASESTANDALONETRANSACTION_H
