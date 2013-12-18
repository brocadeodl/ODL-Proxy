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

#ifndef MANAGEDOBJECTSCHEMAINFOREPOSITORY_H
#define MANAGEDOBJECTSCHEMAINFOREPOSITORY_H

#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfo.h"
#include "Framework/ObjectRelationalMapping/SchemaDifference.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaDifferenceInfo.h"
#include "Framework/ObjectRelationalMapping/ModifiedManagedObjectSchemaDifference.h"
#include "Framework/ObjectRelationalMapping/AddedRemovedManagedObjectSchemaDifference.h"

#include <vector>
#include <map>
#include <memory>

using namespace std;

namespace WaveNs
{

typedef SchemaDifference<ManagedObjectSchemaInfo *> ManagedObjectSchemaDifferences;

class ManagedObjectSchemaInfoRepository
{
    private :
        ResourceId                                  applyDatabaseSchemaDifferences                  (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
        ResourceId                                  applySqlToDatabase                              (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
        bool                                        checkIfTableHasDerivationInstanceTable          (ManagedObjectSchemaInfo* pInfoObject) const; 
        string                                      generateSqlToDeleteDerivationInstanceIdsFromGivenTable    (const UI32 tableId, const vector<UI64>& instanceIds, const string &table, const string &waveSchema);
        void                                        dropAllDatabaseViewsForUpgrade                  ();
        void                                        createAllDatabaseViewsForUpgrade                ();

    protected :
    public :
                                                    ManagedObjectSchemaInfoRepository   ();
                                                   ~ManagedObjectSchemaInfoRepository   ();
        ResourceId                                  computeDatabaseSchemaDifferences    (const ManagedObjectSchemaInfoRepository &rhs);
        void                                        addSchemaInfoObject                 (ManagedObjectSchemaInfo *upgradeInfo);
        ResourceId                                  removeSchemaInfoObjectAndGetSql     (ManagedObjectSchemaInfo *schemaInfo, string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository, bool isInternalCall = false);
        ResourceId                                  removeManagedObjectSql              (ManagedObjectSchemaInfo *pRemovedManagedObject, string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        bool                                        removeUpgradeInfoObject             (ManagedObjectSchemaInfo *upgradeInfo);
        ManagedObjectSchemaInfo *                   findSchemaInfoObject                (UI32 tableId) const;
        ManagedObjectSchemaInfo *                   findSchemaInfoObject                (const string &tableName) const;
        map<string, ModifiedManagedObjectSchemaDifference> getModifiedTablesSchemaDifference ();
        UI32                                        getNumberOfInfoObjectsInRepository  () const;
        void                                        dumpToDOT                           (const string &graphName) const;
        void                                        clear                               ();

        bool                                        operator==                          (const ManagedObjectSchemaInfoRepository &rhs) const;
        bool                                        operator!=                          (const ManagedObjectSchemaInfoRepository &rhs) const;

        auto_ptr<ManagedObjectSchemaDifferences>    getManagedObjectDifferences         (const ManagedObjectSchemaInfoRepository &rhs);
	    ResourceId                                  handleDatabaseSchemaDifferences     (ManagedObjectSchemaInfoRepository &rhs);

        void                                        addMOToSchemaDifference             (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, bool isNewManagedObject);

        void                                        addMOFieldToSchemaDifference        (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, const string& fieldName, const string& fieldType);

        ManagedObjectSchemaDifferenceInfo *         findMOInSchemaDifferenceInfo        (const string &managedObjectName);
        
        void                                        applyFieldDifferences               (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, const FieldSchemaDifferences *differences);

        void                                        cleanupSchemaDifferenceInfo         ();

        void                                        populateSchemaInfoObjectsWithRelationshipPointers       ();

        void                                        displayRelationshipReferenceCountInfo                   ();

        ModifiedManagedObjectSchemaDifference*      getModifiedManagedObjectSchemaDifference                (const string& moName);
        void                                        addModifiedTableToSchemaDifference                      (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, ManagedObjectSchemaInfo *pRhsManagedObjectSchemaInfo, const FieldSchemaDifferences *differences, const RelationFieldSchemaDifferences *relationFieldDiff);
        void                                        addAddedTableToSchemaDifference                         (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo);
        void                                        addRemovedTableToSchemaDifference                       (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo);
        bool                                        isTableRemovedFromSchema                                (const string& managedObjectName);
        vector<UI32>                                getClassIdsOfAllNodesInTheSubTree                       (const string & nameOfManagedObjectAtSubTreeRoot);                                        
        ResourceId                                  generateSqlForSchemaDifferences                         (string & schemaUpdateSqlForCurrentSchema, string & schameUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        ResourceId                                  getSqlForRemovedManagedObjectsAndRelationships          (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        ResourceId                                  getSqlForRemovedManagedObjects                          (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        ResourceId                                  getSqlForRelationshipsInRemovedManagedObjects           (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        ResourceId                                  getSqlForRemovedRelationshipsInModifiedManagedObjects   (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        ResourceId                                  getSqlForSimpleFieldsInModifiedManagedObjects           (string & schemaUpdateSqlForCurrentSchema, string & schameUpdateSqlForStartSchema);
        ResourceId                                  getSqlToRemoveManagedObject                             (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfo *pRemovedManagedObject, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        ResourceId                                  getSqlToRemoveMoEntriesFromAuxillaryAndDerivationsTables (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, string &tableName, const UI32 &tableId, ManagedObjectSchemaInfoRepository & dbRepository);
        ResourceId                                  getSqlToRemoveEntriesFromAuxillaryTables                (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, string tableName, const UI32 tableId, ManagedObjectSchemaInfoRepository & dbRepository);
        void                                        checkIfTableHasAuxillaryAndDerivationsTables            (const ManagedObjectSchemaInfo* pInfoObject, bool &hasAuxillary, bool &hasDerivations);
                     void                           printDifferenceContentsForDebugging                     ();

        void                                        addSchemaInfoObjectFromComputedDifferences              (ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);

	void                                            generateSqlsForAddedRelationShipsInModifiedObjects      (ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository,string & sqlForDerivationTablesForCurrentSchema,string & sqlForDerivationTablesForStartSchema, string & alterSqlForOneToOneRelationShipForCurrentSchema, string &  alterSqlForOneToOneRelationShipForStartSchema,string & sqlForAuxilliaryTablesForCurrentSchema, string & sqlForAuxilliaryTablesForStartSchema);

        void                                        generateSqlsForRelationShipsInAddedObjects              (ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository,string & sqlForDerivationTablesForCurrentSchema,string & sqlForDerivationTablesForStartSchema,string & sqlForAuxilliaryTablesForCurrentSchema, string & sqlForAuxilliaryTablesForStartSchema);
        

	void                                            generateSqlForCreationOfAddedTables                     (string & sqlForCurrentSchema, string & sqlForStartSchema);
    void                                            generateSqlForRepopulatingEntiresOfDerivationsTable     (string & sqlToRepopulateDerivationsTableInCurrentSchema, string & sqlToRepopulateDerivationsTableInStartSchema);
    void                                            generateSqlsForModifiedRelationShipsInModifiedObjects   (string & updateCurrentSchemaSql, string & updateStartupSchemaSql);

	void                                            processAdditionOfNewMOsAndRelationships                 (string & updateCurrentSchemaSql,string  & updateStartupSchemaSql,ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);



                     void                           printMORepositoryForDebugging                           (string &results);
                     void                           getAllInstanceIds                                       (const string tableName, string &result);
                     ResourceId                     setupHierarchialOrder                                   (vector<string>& addedOrRemovedManagedObjects, bool isAddCase, const ManagedObjectSchemaInfoRepository& dbSchemaInfoRepository);
                     void                           addToAHeirarchyList                                     (const ManagedObjectSchemaInfo* moSchemaInfoObject, map<string, AddedRemovedManagedObjectSchemaDifference>& addedOrRemovedTablesSchemaDifference, vector<string>& addedOrRemovedManagedObjects, bool isAddCase, const ManagedObjectSchemaInfoRepository& dbSchemaInfoRepository);


        static       ResourceId                     printMoRepositoryDebugShellWrapper                      (UI32 argc, vector<string> argv);
        static       ResourceId                     removeSchemaObjectDebugShellWrapper                     (UI32 argc, vector<string> argv);
                     
                     ResourceId                     getSqlForInsertDrvInstancesEntriesForTable              (const string &table, string &sql);
                     ResourceId                     getSqlToRemoveInstanceIdsFromDerivationInstancesTables  (string& schemaUpdateSqlForCurrentSchema, string& schemaUpdateSqlForStartSchema, const ManagedObjectSchemaInfoRepository &dbRepository, const string &tableName, const UI32 tableId, vector<UI64> &instanceIds);
                     ResourceId                     getSqlToDropUserDefinedKeyUniqueConstraint              (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
                     void                           getSqlToAddUserDefinedKeyUniqueConstraint               (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
                     bool                           getIfUserDefinedKeyCombinationChanged                   ();
                     void                           getTablesInOrderForUDKCHandling                         (vector<string>& tablesForUDKCHandlingInOrder, map<string, vector<string> >& compositionsToHandle); 
                     void                           getSqlToUpdateUserDefinedKeyCombinationForUpgrade       (string& sqlForCurrentSchemaForUDKC, string& sqlForStartupSchemaForUDKC);              
                     void                           getSqlToUpdateUDKCRelatedColumns                        (vector<string>& tablesForUDKCHandlingInOrder, map<string, vector<string> >& compositionsToHandle, string& sqlForCurrentSchemaForUDKC, string& sqlForStartupSchemaForUDKC);

    // Now the data members
    private :
        bool                                                m_userDefinedKeyCombinationChanged;
        vector<ManagedObjectSchemaInfo *>                   m_schemaInfoObjects;
        map<string, ManagedObjectSchemaInfo *>              m_schemaInfoByNameMap;
        map<UI32, ManagedObjectSchemaInfo *>                m_schemaInfoByIdMap;
        map<UI32, vector<ManagedObjectSchemaInfo *> >       m_tempInfoObjectStore;
        vector<UI32>                                        m_tempInfoObjectIdStoreForRemove;
        map <string, ManagedObjectSchemaDifferenceInfo *>   m_schemaDifferenceInfo;

        map<string, ModifiedManagedObjectSchemaDifference>      m_modifiedTablesSchemaDifference;  
        map<string, AddedRemovedManagedObjectSchemaDifference>  m_addedTablesSchemaDifference;
        map<string, AddedRemovedManagedObjectSchemaDifference>  m_removedTablesSchemaDifference;
    protected :
    public :
};
}
#endif //MANAGEDOBJECTSCHEMAINFOREPOSITORY_H

