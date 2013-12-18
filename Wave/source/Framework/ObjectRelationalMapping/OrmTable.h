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

#ifndef ORMTABLE_H
#define ORMTABLE_H

#include "Framework/ObjectRelationalMapping/OrmColumn.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Attributes/AttributeTypes.h"

#include <vector>
#include <string>
#include <set>

using namespace std;

namespace WaveNs
{

class OrmTable
{
    private :
        void                        addFieldNameToUpgradeString        (const string & attributeName);
        void                        addFieldSqlTypeToUpgradeString     (const string & attributeSqlType);
        void                        addDbFieldSqlTypeToUpgradeString   (const string & attributeSqlType);

    protected :
    public :
                                    OrmTable                               (const string &name, const string &derivedFromClassName, const bool isALocalManagedObject);
                                    OrmTable                               ();
        virtual                    ~OrmTable                               ();

        string                      getName                                () const;
        string                      getDerivedFromClassName                () const;

        bool                        isAKnownColumn                         (const string &columnName) const;
        bool                        isAKnownColumnInHierarchy              (const string &columnName) const;
        bool                        isAnIntegerColumn                      (const string &columnName);
        void                        addColumn                              (OrmColumn *pOrmColumn);
        bool                        isAKnownRelation                       (const string &relationName) const;
        bool                        isAKnownRelationInHierarchy            (const string &relationName, OrmRelationType &ormRelationType) const;
        void                        addRelation                            (OrmRelation *pOrmRelation);

        string                      getSqlForCreate                        (const string &schema) const;
        string                      getSqlForCreate2                       (const string &schema) const;
        string                      getSqlForCreate2                       (const string &schema, const string &relationName);

        string                      getSqlForRemoveColumn                  (const string &schema, const string& columnName);
        string                      getSqlForRemoveTables                  (const string &schema, const string& moName);
        string                      getSqlToDropDerivationTables           (const string &schema, const string& moName);
        void                        removeOrmColumnFromTable               (const string &columnName);
        void                        getSqlToCountNumberOfEntriesInTable    (const string &schema, string &sqlString);


        UI32                        getTableId                             () const;
        void                        setTableId                             (const UI32 &tableId);
/*
        UI32                        getOldTableId                          () const;
        void                        setOldTableId                          (const UI32 oldTableId);
*/

        OrmTable                   *getPParentTable                        () const;
        void                        setPParentTable                        (OrmTable *pParentTable);

        void                        addDerivation                          (OrmTable *pOrmTable);

        void                        getReferences                          (vector<string> &references);
        void                        getAllDerivedTableIds                  (vector<OrmTable *> &derivedTableIds);

        void                        lock                                   ();
        void                        unlock                                 ();

        UI32                        getNextInstanceId                      ();
        void                        setNextInstanceId                      (const UI32 &nextInstanceId);
        UI32                        getCurrentNextInstanceId               ();

        OrmTable                   *getPMostBaseTable                      ();
        void                        setPMostBaseTable                      (OrmTable *pMostBaseTable);

        string                      getSqlToDeleteAllEntries               (const string &schema) const;
        string                      getSqlToSelectivelyDeleteEntries       (const string &schema) const;
        string                      getSqlToTruncate                       (const string &schema) const;
        string                      getSqlToQueryAllEntries                (const string &sourceSchema) const;
        string                      getSqlToInsertEntries                  (const string &destSchema, const string &values) const;
        string                      getSqlToCopy                           (const string &srcSchema, const string &destSchema) const;
        string                      getSqlToDrop                           (const string &schema) const;

        string                      getSqlTypeForAField                    (const string& fieldName);
 
        void                        getAuxilliaryTableNames                (vector<string> &auxilliaryTableNames) const;
        void                        getAuxilliaryTableNamesAppended        (vector<string> &auxilliaryTableNames) const;
        void                        getAuxilliaryTableDetailsAppended      (vector<string> &auxilliaryTableNames, vector<string> &parentTableNames, vector<string> &relatedToTableNames) const;
        void                        getAllAuxilliaryTableNames             (vector<string> &auxilliaryTableNames) const;
        void                        getAllAuxilliaryTableNames             (vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes) const;
        void                        getAllAuxilliaryTableNames             (vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes, const vector<string> &selectedEntities) const;

        void                        getCompositionFieldNamesInHierarchy    (vector<string> &compositionFieldNames) const;

        UI32                        getNumberOfOneToManyRelations          () const;

        void                        computeDerivationsInstancesTables      ();
        void                        addDerivationsInstancesTable           (const string &derivationsInstancesTable);
        void                        printDerivationsInstancesTables        (const string &prefix, string &outputString);

        void                        getSqlForInsertForDerivationsInstances (string &sqlForInsertForDerivationsInstnces, const UI64 &instanceId, const string &schema);
        void                        getSqlForDeleteForDerivationsInstances (string &sqlForDeleteForDerivationsInstnces, const UI64 &instanceId, const string &schema);

        void                        setUserDefinedKeyCombinationWithTypes  (const map<string, Attribute*> &userDefinedKeyCombinationWithTypes, const vector<string> &userDefinedKeyCombination);
        map<string, Attribute*>     getUserDefinedKeyCombinationWithTypes  (vector<string> &userDefinedKeyCombination) const;

        void                        setEmptyNeededOnPersistentBoot         (const bool &emptyNeededOnPersistentBoot);
        bool                        getEmptyNeededOnPersistentBoot         () const;

        void                        validateRelations                      (OrmTable *pOrmTableToValidate);
        string                      getSqlToSelectivelyDeleteDerivationsInstances (const string &schema);
        string                      getSqlToSelectivelyTruncateFromDerivationsInstances (const string &schema);
        string                      getSqlToSelectivelyDeleteDerivationsInstances (const string &schema, const string &whereClause);

        void                        getDetailsForRelationships             (vector<string> &relatedToTables, vector<OrmRelationType> &relationTypes, vector<OrmRelationUmlType> &relationUmlTypes);
        void                        getDetailsForRelationshipsInHierarchy  (vector<OrmTable *> &relatedFromTables, vector<string> &relatedToTables, vector<OrmRelationType> &relationTypes, vector<OrmRelationUmlType> &relationUmlTypes, vector<string> &relationNames);
        bool                        getIsALocalManagedObject               () const;


        const vector<string> &      getFieldNamesForUpgrade                () const;
        const vector<string> &      getFieldTypesForUpgrade                () const;
        const vector<string> &      getDbFieldTypesForUpgrade              () const;
        const vector<string>        getExtendedFieldTypesForUpgrade        ();
        
        const vector<string>        getUserDefinedFieldNamesForUpgrade     ();
        const vector<string>        getUserDefinedFieldTypesForUpgrade     ();
        
        void                        setUpgradeStrings                      ();
        void                        addManagedObjectAttributes             (Attribute *attribute);
        Attribute*                  getAttributeForFieldNameInManagedObject (const string &fieldName);
        void                        cleanupManagedObjectAttributes         ();

        void                        getEntityDetails                       (const string &entityName, bool &isAColumn, bool &isARelation, OrmRelationType &ormRelationType);
        void                        getOrmRelationTypeAndOrmRelationUmlType(const string &entityName, OrmRelationType &ormRelationType, OrmRelationUmlType &ormRelationUmlType);
        bool                        isASimpleObjectIdColumn                (const string &entityName);
        void                        getDeivedTablesAndRelations            (vector<OrmTable *> &derivedTables, vector<OrmRelation *> &relationColumn);

        string                      getAlterSqlForOneToOneRelationship     (const string &schema,const string & relationName) const;

        bool                        getValueForCanBeEmpty                   (const string &fieldName);
        bool                        getValueForDisableValidations           (const string &fieldName);

        void                        setEmptyNeededOnPersistentBootWithDefault               (const bool &emptyNeededOnPersistentBootWithDefault);
        bool                        getEmptyNeededOnPersistentBootWithDefault               () const;

        string                      getAlterSqlToAddNotNullForOneToOneRelationship          (const string &schema,const string & relationName) const;
        string                      getAlterSqlToDropNotNullForOneToOneRelationship         (const string &schema,const string & relationName) const;
        string                      getAlterSqlToDisableValidationForOneToManyRelationship  (const string &schema, const string & relationName) const;
        string                      getAlterSqlToEnableValidationForOneToManyRelationship   (const string &schema, const string & relationName) const;

        void                        getRelationNameToAuxilliaryTableNameMapForOneToNAssociations  (const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap) const;
        void                        getRelationNameToAuxilliaryTableNameMapForOneToNCompositions  (const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap) const;
        void                        getRelationNameToAuxilliaryTableNameMapForOneToNRelationType  (const OrmRelationUmlType &ormRelationUmlType, const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap) const;
        void                        getAllTableNamesInHierarchy                                   (set<string> &tableNames);
        void                        getCompositionRelationTargetTableNamesFromSet                 (const set<string> &setOfTables, vector<string> &childTableNames);

        bool                        isATableNameInHierarchy                                 (const string &tableName) const;
        void                        getUserDefinedKeyCombination                            (vector<string> &userDefinedKeyCombination) const;
        void                        getAllEntityNamesInHierarchy                            (vector<string> &entityNamesInHierarchy);
        void                        getSqlForATableToUpdateUDKCRelatedColumns               (vector<string>& composedChildren, string& sqlForCurrentSchemaForUDKC, string& sqlForStartupSchemaForUDKC);
        void                        computeKeyStringAndUDKCValueExpression                  (string& keyStringExpression, string& userDefinedKeyCombinationExpression);
        void                        getSqlToDropUserDefinedKeyUniqueConstraint              (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
        void                        getSqlToAddUserDefinedKeyUniqueConstraint               (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);

    // Now the data members


    private :
        string                      m_name;
        string                      m_derivedFromClassName;
        bool                        m_isALocalManagedObject;
        vector<OrmColumn *>         m_columns;
        map<string, string>         m_columnNames;
        vector<OrmRelation *>       m_relations;
        map<string, OrmRelation *>  m_relationsByNames;
        vector<OrmEntity *>         m_ormEntities;
        OrmTable                   *m_pParentTable;
        UI32                        m_tableId;
        UI32                        m_oldTableId;

        vector<OrmTable *>          m_derivations;

        PrismMutex                  m_mutex;
        UI32                        m_nextInstanceId;
        OrmTable                   *m_pMostBaseTable;

        vector<string>              m_derivationsInstancesTables;

        map<string, Attribute*>     m_userDefinedKeyCombinationWithTypes;
        vector<string>              m_userDefinedKeyCombination;
        bool                        m_emptyNeededOnPersistentBoot;
        vector<string>              m_fieldNamesForUpgrade;
        vector<string>              m_fieldTypesForUpgrade;
        vector<string>              m_fieldDbTypesForUpgrade;
        map<string, Attribute*>     m_managedObjectAttributes;
        bool                        m_emptyNeededOnPersistentBootWithDefault;

    protected :
    public :
};

}

#endif // ORMTABLE_H
