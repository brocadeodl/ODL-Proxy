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

#ifndef MANAGEDOBJECTSCHEMAINFO_H
#define MANAGEDOBJECTSCHEMAINFO_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectRelationalMapping/SchemaDifference.h"
#include "Framework/ObjectRelationalMapping/RelationReferenceCount.h"
#include "Framework/ObjectRelationalMapping/RelationReferenceCountPointer.h"
#include "Framework/ObjectRelationalMapping/RelationshipInfoFromSchemaDifference.h"
//#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"

#include <string>
#include <map>
#include <set>
#include <memory>

namespace WaveNs
{

typedef SchemaDifference<pair<string, string> > FieldSchemaDifferences;

class ManagedObjectSchemaInfoRepository;
class RelationshipInfoFromSchemaDifference;

typedef SchemaDifference<RelationshipInfoFromSchemaDifference> RelationFieldSchemaDifferences;
class ManagedObjectSchemaInfo : public RelationReferenceCount
{
    private :
    protected :
    public :
                        ManagedObjectSchemaInfo         (const string &name, UI32 tableId, UI32 parentTableId);
        virtual        ~ManagedObjectSchemaInfo         ();
        const string &  getName                         () const;
        UI32            getTableId                      () const;
        UI32            getParentTableId                () const;
        size_t          getNumberOfFields               () const;
        size_t          getNumberOfSimpleFields         () const;
        size_t          getNumberOfRelationFields       () const;
        size_t          getNumberOf1to1RelationFields   () const;
        size_t          getNumberOf1tonRelationFields   () const;

        const map<string, string> & getRelationFields   () const;
        const map<string, string> & getSimpleFields     () const;

        virtual void    addSchemaInfoObject             (ManagedObjectSchemaInfo *child) = 0;
        virtual vector<ManagedObjectSchemaInfo *>*      getChildSchemaInfoObjects () = 0;
        virtual void    removeInfoObject                (ManagedObjectSchemaInfo *child) = 0;
        void            addFieldTypeTuple               (const string &fieldName, const string &fieldType);
        void            addUserDefinedFieldTypeTuple    (const string &userDefinedFieldName, const string &userDefinedFieldType);

        bool            operator==                      (const ManagedObjectSchemaInfo &rhs) const;
        bool            operator!=                      (const ManagedObjectSchemaInfo &rhs) const;



        auto_ptr<FieldSchemaDifferences>    getFieldObjectDifferences                           (const ManagedObjectSchemaInfo &rhs, ResourceId & status) const;
        auto_ptr<RelationFieldSchemaDifferences>    getRelationFieldObjectDifferences           (const ManagedObjectSchemaInfo &rhs, ResourceId & status) const;
        ResourceId                          parseAndCompareRelationships                        (string lhsRelationTypeFull, string rhsRelationTypeFull) const;
        auto_ptr<FieldSchemaDifferences>    getUserDefinedKeyDifferences                        (const ManagedObjectSchemaInfo &rhs, ResourceId & status) const;
        ResourceId                          getInheritanceDiffStatus                            (const ManagedObjectSchemaInfo &rhs) const;
        string                              getSQLForTableUpdate                                (const string &schema, const vector<pair<string, string> > &addedFields, const vector<pair<string, string> > &removedFields, const vector<pair<string, string> > &changedFields) const;
        ResourceId                          getSqlForRemovedRelationship                        (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, const string & relationName, const string & relatedToTable, OrmRelationUmlType relationUmlType, OrmRelationType relationType, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);

        ResourceId                          getSqlForRemovingClassIdInstanceIdColumns           (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, const string & relationName);
        ResourceId                          getSqlForRemovedManagedObject                       (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
        ResourceId                          getSqlForRemovingDerivationAndDerivationInstances   (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
        ResourceId                          getSqlForRemovingAuxillaryTable                     (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, const string &relationName, const string & relatedToTable, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository);
        void                                createReferenceCountToSchemaInfoObject              (const string & toManagedObjectName, ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository);
        void                                populateRelationReferencePointers                   (ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository);
	    void                                displayRelationshipReferenceCountInfo               ();
        UI32                                getCountOfRelationshipReferecePointers              ();
	    string                              generateAlterSqlForOneToOneRelationship             (const string & toObjectName);

	    void                                processRelationsAddedInNewSchema                    (const vector<RelationshipInfoFromSchemaDifference> & relationshipsInfoVector, ManagedObjectSchemaInfoRepository & dbManagedObjectSchemaInfoRepository,  string & sqlForDerivationTablesForCurrentSchema,string & sqlForDerivationTablesForStartSchema,string & sqlForAuxilliaryTablesForCurrentSchema,string &  sqlForAuxilliaryTablesForStartSchema);

        void                                getSqlToRemoveAuxilliaryTableEntriesForToEndTables  (string& schemaUpdateSqlForCurrentSchema, string& schemaUpdateSqlForStartSchema, const UI32 tableId);
        void                                updateRelationFromTable                             (const string& realtion, ManagedObjectSchemaInfo* fromMoSchema);
        void                                updateFromEndForRelatedToTable                      (ManagedObjectSchemaInfo* relatedToTable, const string& relation, const string& relationType);
        void                                removeRelationFromTable                             (const string& realtion);
        void                                removeFromEndForRelatedToTable                      (ManagedObjectSchemaInfo* relatedToTable, const string& relation);
        map<string, ManagedObjectSchemaInfo*>& getRelationFromTables                            (); 
        
        bool                                getIsALocalManagedObject                            () const;
        void                                setIsALocalManagedObject                            (bool isALocalMO);

        string                              getDerivedFromClassName                             () const;
        void                                setDerivedFromClassName                             (const string derivedFromTable);
        static void                         getDataTypesFromExtendedFieldType                   (const string &extendedFieldType, string &attributeType, string &sqlDataType);
        /*void                                getSqlToDropUserDefinedKeyUniqueConstraint          (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);
        void                                getSqlToAddUserDefinedKeyUniqueConstraint           (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema);*/
        string                              getUserDefinedKeyCombinationString                  ();
        void                                getCompositionParentsForATable                      (ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository, vector<string>& compositionParentTableName);
        void                                getCompositionChildrenForATable                     (ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository, vector<string>& compositionChildren);
    // Now the data members

    private :
        string                                  m_name;
        UI32                                    m_tableId;
        UI32                                    m_parentTableId;
        UI32                                    m_numberOf1to1RelationFields;
        UI32                                    m_numberOf1tonRelationFields;
        map<string, string>                     m_simpleFieldTypeMap;   //map <fieldname, fieldtype>
        map<string, string>                     m_relationFieldTypeMap; //map <fieldname, fieldtype>
        map<string, ManagedObjectSchemaInfo*>   m_relationsFromTable;   //map <relation, relationFromMO>  - relation = fieldName for 1:n relation OR ORM_RELATION_TYPE_ONE_TO_ONE
        map<string, string>                     m_userDefinedKeyCombinationsMap; // map <usdkfieldname, usdkfieldtype>
        string                                  m_userDefinedKeyCombinationString;
        string                                  m_derivedFromClassName;
        bool                                    m_isALocalManagedObject;
	    vector<RelationReferenceCountPointer<ManagedObjectSchemaInfo> *> m_relationReferenceCountPointers;        

    protected :
    public :
};

class LeafManagedObjectSchemaInfo: public ManagedObjectSchemaInfo
{
    private :
    protected :
    public :
                        LeafManagedObjectSchemaInfo     (const string &name, UI32 tableId, UI32 parentTableId);
        virtual        ~LeafManagedObjectSchemaInfo     ();
        virtual void    addSchemaInfoObject             (ManagedObjectSchemaInfo *child);
        virtual void    removeInfoObject                (ManagedObjectSchemaInfo *child);
        virtual vector<ManagedObjectSchemaInfo *>*      getChildSchemaInfoObjects           ();

    // Now the data members

    private :
    protected :
    public :
};

class CompositeManagedObjectSchemaInfo: public ManagedObjectSchemaInfo
{
    private :
    protected :
    public :
                        CompositeManagedObjectSchemaInfo    (const string &name, UI32 tableId, UI32 parentTableId);
        virtual        ~CompositeManagedObjectSchemaInfo    ();
        virtual void    addSchemaInfoObject                 (ManagedObjectSchemaInfo *child);
        virtual void    removeInfoObject                    (ManagedObjectSchemaInfo *child);
        virtual vector<ManagedObjectSchemaInfo *>*          getChildSchemaInfoObjects           ();
        void            getClassIdsOfTheEntireSubTree           (vector<UI32> & classIdsOfAllNodesInTheSubTree) const;
        void            generateSqlToRepopulateDerivationsTable (const string & waveSchema, string & sqlToRepopulateDerivationsTable);
        string          getSqlToDeleteAllEntriesOfDerivationsTable (const string & waveSchema);

        void            repopulateDerivationsInstancesTable (const string & waveSchema, string & sqlToRepopulateDerivationsInstancesTable);



    // Now the data members

    private :
        vector <ManagedObjectSchemaInfo *>       m_children;
    protected :
    public :
};

}
#endif //MANAGEDOBJECTSCHEMAINFO_H

