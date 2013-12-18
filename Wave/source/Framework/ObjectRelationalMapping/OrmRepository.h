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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ORMREPOSITORY_H
#define ORMREPOSITORY_H

#include "Framework/Types/Types.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/ObjectRelationalMapping/OrmView.h"
#include "Mathematics/Z2/Z2.h" 

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

namespace WaveNs
{

class OrmTable;
class ObjectId;

class OrmRepository
{
    private :
        OrmRepository ();
                     bool                       getValueForCanBeEmpty                         (OrmTable *pOrmTable, const string &fieldName, const string &fieldType);
                     bool                       getValueForDisableValidations                 (OrmTable *pOrmTable, const string &fieldName, const string &fieldType);

        //static       UI32                       getUniqueTableIdForTableName                  (const string &tableName);
                     UI32                       getUniqueTableIdForTableName                  (const string &tableName);
    protected :
    public :
                                               ~OrmRepository                                 ();

        static       OrmRepository             *getInstance                                   ();

                     void                       addTable                                      (OrmTable *pOrmTable);
                     void                       addView                                       (OrmView  *pOrmView);

                     string                     generateSchema                                (const string &waveSchema);
                     void                       generateSqlToPopulateUpgradeTable             (string & sqlForUpgradeTablePopulation,const string & waveSchema);
                     void                       generateSqlToCreateViews                      (string & sqlForCreatingViews);
        static       UI32                       getNextTableId                                ();
                     void                       computeTableSpace                             ();
                     //void                       buildMoSchemaRepository                       (const bool updateTableIdsFromUpgradeMO = 0);
                     void                       buildMoSchemaRepository                       ();
                     void                       buildAttributeTypeToSqlTypeMap                ();
                     void                       buildAttributeSupportedConversionsMap         ();
        static       string                     getSchemaForDerivationsTable                  (const string &tableName, const string &waveSchema);
        static       string                     getSchemaForReferenceTableUniqueConstraint    (const string &tableName, const string &waveSchema);
        static       string                     getSqlToDeleteAllEntriesFromDerivationsTable  (const string &tableName, const string &waveSchema);
        static       string                     getSqlToTruncateFromDerivationsTable          (const string &tableName, const string &waveSchema);
        static       string                     getSqlToDeleteDerivationInstancesTable        (const string &tableName, const string &waveSchema);
        static       string                     getSqlToDropDerivationsTable                  (const string &tableName, const string &waveSchema);
        static       string                     getSqlToDeleteAllEntriesFromAuxilliaryTable   (const string &tableName, const string &waveSchema);
        static       string                     getSqlToTruncateFromAuxilliaryTable           (const string &tableName, const string &waveSchema);
        static       string                     getSqlToRemoveEntriesFromAuxilliaryTable      (const string &tableName, const UI32 classid, const string &waveSchema);
        static       string                     getSqlToRemoveAuxilliaryTableEntriesForToEnd  (const UI32 fromTableId, const string& relation, const UI32 relatedToTableId, const UI32 tableIdTobeRemoved, const string& waveSchema); 
        static       string                     getSqlToDropAuxilliaryTable                   (const string &tableName, const string &waveSchema);
        static       string                     getSqlForDerivedTable                         (const string &tableName, const UI32 &tableId, const string &waveSchema);
        static       OrmTable                  *getMostBaseTable                              (const string &tableName);
        static       ObjectId                   getNewObjectId                                (const string &tableName);
                     void                       getAllSqlsToCountAllEntriesInDatabase         (vector <string> &sqlStrings, const string &waveSchema);
                     void                       getSqlForDelete                               (const ObjectId &objectId, string &sqlForDelete, const string &waveSchema);

                     string                     generateSqlToEmptyOrmDatabase                 (const string &waveSchema);
                     string                     generateSqlToEmptyOrmDatabaseUsingTruncate    (const string &waveSchema);
                     string                     generateSqlToSelectivelyEmptyOrmDatabase      (const string &waveSchema);
                     string                     generateSqlToSelectivelyEmptyOrmDatabaseUsingTruncate      (const string &waveSchema);
                     string                     generateSqlToDropTablesFromOrmDatabase        (const string &waveSchema);
                     string                     generateSqlToDropTableFromOrmDatabase         (const string &waveSchema, const string &tableName);
                     string                     generateSqlToDropSchemaFromOrmDatabase        (const string &waveSchema);
                     string                     generateSqlToTruncateDatabaseSchemaForGivenTables    (const string &waveSchema, vector<string>& tableNames);
                     void                       getSqlToCountNumberOfEntriesInTable           (const string &tableName, const string &waveSchema, string &sqlString);
                     string                     getSqlToQueryAllEntriesFromTable              (const string &tableName, const string &sourceSchema);
                     string                     getSqlToInsertEntriesIntoTable                (const string &tableName, const string &destSchema, const string &values);
                     string                     getSqlToTruncateTable                         (const string &tableName, const string &sourceSchema);
                     string                     getSqlToDeleteAllEntriesFromTable             (const string &tableName, const string &sourceSchema);
        static       string                     getSqlToQueryAllEntriesFromDerivationsTable   (const string &tableName, const string &sourceSchema);
        static       string                     getSqlToInsertEntriesIntoDerivationsTable     (const string &tableName, const string &destSchema, const string &values);
        static       string                     getSqlToQueryAllEntriesFromAuxilliaryTable    (const string &tableName, const string &sourceSchema);
        static       string                     getSqlToInsertEntriesIntoAuxilliaryTable      (const string &tableName, const string &destSchema, const string &values);
        static       string                     getSqlToCopyDerivationsTable                  (const string &tableName, const string &srcSchema, const string &destSchema);
        static       string                     getSqlToCopyTable                             (const string &tableName, const string &srcSchema, const string &destSchema);
        static       string                     getSqlToCopyAuxilliaryTable                   (const string &tableName, const string &srcSchema, const string &destSchema);
        static       string                     getSqlToRenameSchema                          (const string &schemaName, const string &newSchemaName);
        static       string                     getSqlForAllEntriesInTable                    (const string &tableName, const string &waveSchema);

        static       ResourceId                 printOrmDebugShellWrapper                     (UI32 argc, vector<string> argv);
        static       ResourceId                 printOrm                                      (UI32 argc, vector<string> argv, string &ormOutput);

        static       void                       getAllMostBaseTableNames                      (vector<string> &mostBaseTableNames);
        static       void                       updateLatestInstanceIdForTable                (const string &tableName, const UI32 &latestInstanceId);

        static       UI32                       getTableId                                    (const string &tableName);
        static       void                       getAuxilliaryTableNames                       (const string &tableName, vector<string> &auxilliaryTableNames);
        static       void                       getAllAuxilliaryTableNames                    (const string &tableName, vector<string> &auxilliaryTableNames);
        static       void                       getAllAuxilliaryTableNames                    (const string &tableName, vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes);
        static       void                       getAllAuxilliaryTableNames                    (const string &tableName, vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes, const vector<string> &selectedEntities);

        static       string                     getTableNameById                              (const UI32 &tableId);
        static       void                       getSqlForInsertForDerivationsInstances        (const string &tableName, string &sqlForInsertForDerivationsInstnces, const UI64 &instanceId, const string &waveSchema);
        static       void                       getSqlForDeleteForDerivationsInstances        (const string &tableName, string &sqlForDeleteForDerivationsInstnces, const UI64 &instanceId, const string &waveSchema);

        static       OrmTable                  *getTableByName                                (const string &tableName);

        static       void                       addMostBaseClass                              (const string &mostBaseClass);
        static       bool                       isAMostBaseClass                              (const string &mostBaseClass);

        static const string                     getWaveCurrentSchema                          ();
        static const string                     getWaveStartSchema                            ();
                     void                       getSchemaChangeVectors                        (vector<string>   & managedObjectNamesForSchemaChange,vector<string>   & fieldNamesStringsForSchemaChange,vector<string> & fieldNamesTypesForSchemaChange,vector<UI32>   & classIds,vector<UI32> & m_parentTableIds);

        static       bool                       isFieldOfIntegerType                          (const string &tableName, const string &columnName);
        static       void                       getAllCompositionTableName                    (set<string> &allCompositionTableNames);

                     vector<string>             getTableNames                                 (bool checkEmptyNeeded = false);
                     vector<string>             getDerivationsTableNames                      (bool checkEmptyNeeded = false);
                     vector<string>             getAuxilliaryTableNames                       (bool checkEmptyNeeded = false);

                     vector<string>             getTableNamesWithNoEmptyNeeded                ();
                     vector<string>             getDerivationsTableNamesWithNoEmptyNeeded     ();
                     vector<string>             getAuxilliaryTableNamesWithNoEmptyNeeded      ();

        static const map<string, Attribute*> getUserDefinedKeyCombinationWithTypesForTable (const string &tableName, vector<string> &userDefinedKeyCombination);

        static       bool                       isALocalManagedObject                         (const string &tableName);
        //             void                       updateTableId                                 (const vector<pair<string, UI32> > &tableNameIdTuples);
        //             void                       restoreOldTableIds                            ();
        static       void                       getSqlToDeleteAllEntriesInTable               (const string &tableName, string &sqlString);
               const vector<string>  &          getManagedObjectNamesForSchemaChange          () const;
               const vector<string>  &          getFieldNamesStringsForSchemaChange           () const;
               const vector<string>  &          getFieldNamesTypesForSchemaChange             () const;
               const vector<string>  &          getFieldNamesExpandedTypesForSchemaChange     () const;
               const vector<UI32>    &          getClassIdsForSchemaChange                    () const;
               const vector<UI32>    &          getParentTableIds                             () const;
               const vector<bool>    &          getIsALocalManagedObjectInfoForSchemaChange   () const;
               const vector<string>  &          getDerivedFromClassNameForSchemaChange        () const;
               const vector<string>  &          getUserDefinedKeyCombinationsFieldNamesForSchemaChange  () const;
               const vector<string>  &          getUserDefinedKeyCombinationsFieldTypesForSchemaChange  () const;
            ManagedObjectSchemaInfoRepository & getMOSchemaInfoRepository                     ();

        static       void                       getEntityDetails                              (const string &tableName, const string &entityName, bool &isAColumn, bool &isARelation, OrmRelationType &ormRelationType);
        static       void                       getOrmRelationTypeAndOrmRelationUmlType       (const string &tableName, const string &entityName, OrmRelationType &ormRelationType, OrmRelationUmlType &ormRelationUmlType);
        static       bool                       isASimpleObjectIdColumn                       (const string &tableName, const string &entityName);

                     string                     getSqlStringForAttributeTypeString            (const string &attributeTypeString);
                     bool                       isTypeConversionSupported                     (const string &from, const string &to);
//                     ResourceId                 updateTableIdsWithTableIdsReceivedFromPrimary (const vector<string> & managedObjectNamesForSchemaChange,const vector<UI32> & classIds, const vector<UI32> & parentTableIds,vector<string> & oldAuxilliaryTableNames,vector<string> & newAuxilliaryTableNames, vector<string> &parentsForAuxilliaryTables, vector<string> &relatedToTable);


        static const string                     getSchemaUpgradeTableName                     ();

        static       void                       getAllTableNames                              (set<string> &allTableNames);


        static       void                       getRelationNameToAuxilliaryTableNameMapForOneToNAssociations  (const string &tableName, const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap);
        static       void                       getRelationNameToAuxilliaryTableNameMapForOneToNCompositions  (const string &tableName, const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap);
        static       UI32                       getNumberOfCompositionRelationsBetweenTables                  (const string &ownerTableName, const string &childTableName);

        static       bool                       isATableNameInHierarchy                       (const string &inheritanceChildTableName, const string &inheritanceParentTableName);
        static      void                        getUserDefinedKeyCombinationForTable          (const string &managedObjectClassName, vector<string> &userDefinedKeyCombination);
        static       bool                       isManagedClassAView                           (const string &managedClass);
        static       bool                       isManagedClassATable                          (const string &managedClass);

    // Now the data members

    private :
                     vector<OrmTable *>                     m_tables;
                     map<UI32, OrmTable *>                  m_tablesMapById;
                     map<string, OrmTable *>                m_tablesMapByName;
                     map<string, string>                    m_mostBaseClassesMap;
                     map<string, string>                    m_attributeTypeToSqlTypeMap;
                     map<string, map <string, string> >     m_attributeSupportedConversionsMap;

                     ManagedObjectSchemaInfoRepository m_MOUpgradeRepository;

        static const string                  m_waveCurrentSchema;
        static const string                  m_waveStartSchema;

                    //Data members to be used during schema change
                    vector<string>              m_managedObjectNamesForSchemaChange;
                    vector<string>              m_fieldNamesStringsForSchemaChange;
                    vector<string>              m_fieldNamesTypesForSchemaChange;
                    vector<UI32>                m_classIds;
                    vector<UI32>                m_parentTableIds;
                    vector<string>              m_userDefinedKeyCombinationsFieldNamesForSchemaChange;
                    vector<string>              m_userDefinedKeyCombinationsFieldTypesForSchemaChange;
                    vector<string>              m_fieldNamesExpandedTypesForSchemaChange;
                    vector<bool>                m_isALocalManagedObjectInfoForSchemaChange;
                    vector<string>              m_derivedFromClassNameForSchemaChange;

                    Z2                          m_z2;
                    vector<OrmView *>           m_views;
                    map<string, OrmView *>      m_viewsMapByName;
    protected :
    public :
};

}

#endif // ORMREPOSITORY_H
