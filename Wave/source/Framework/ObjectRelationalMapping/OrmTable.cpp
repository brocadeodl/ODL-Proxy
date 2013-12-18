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

#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/ObjectRelationalMapping/OrmComposition.h"
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/Attributes.h" 
#include "Framework/Attributes/AttributeManagedObjectVectorCompositionTemplateBase.h" 
#include "Framework/Attributes/AttributeManagedObjectOneToOneRelationBase.h" 

namespace WaveNs
{

OrmTable::OrmTable (const string &name, const string &derivedFromClassName, const bool isALocalManagedObject)
    : m_name                 (name),
      m_derivedFromClassName (derivedFromClassName),
      m_isALocalManagedObject (isALocalManagedObject),
      m_pParentTable         (NULL),
      m_tableId              (0),
      m_oldTableId           (0),
      m_nextInstanceId       (0),
      m_pMostBaseTable       (NULL),
      m_emptyNeededOnPersistentBoot (false),
      m_emptyNeededOnPersistentBootWithDefault (false)

{
    if (m_name == m_derivedFromClassName)
    {
        trace (TRACE_LEVEL_FATAL, "OrmTable::OrmTable : A table cannot be derived from the same table : " + name);
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
}

/* Using this Constructor in OrmView */
OrmTable::OrmTable ()
    : m_name                 (""),
      m_derivedFromClassName (""),
      m_isALocalManagedObject (false),
      m_pParentTable         (NULL),
      m_tableId              (0),
      m_oldTableId           (0),
      m_nextInstanceId       (0),
      m_pMostBaseTable       (NULL),
      m_emptyNeededOnPersistentBoot (false),
      m_emptyNeededOnPersistentBootWithDefault (false)

{
}

OrmTable::~OrmTable ()
{
    UI32 numberOfColumns   = m_columns.size ();
    UI32 numberOfRelations = m_relations.size ();
    UI32 i                 = 0;

    for (i = 0; i < numberOfColumns; i++)
    {
        delete m_columns[i];
    }

    m_columns.clear ();

    for (i = 0; i < numberOfRelations; i++)
    {
        delete m_relations[i];
    }

    m_relations.clear ();
}

string OrmTable::getName () const
{
    return (m_name);
}

bool OrmTable::isAKnownColumn (const string &columnName) const
{
    map<string, string>::const_iterator element    = m_columnNames.find (columnName);
    map<string, string>::const_iterator endElement = m_columnNames.end ();

    if (endElement == element)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool OrmTable::isAKnownColumnInHierarchy (const string &columnName) const
{
    map<string, string>::const_iterator element    = m_columnNames.find (columnName);
    map<string, string>::const_iterator endElement = m_columnNames.end ();

    if (endElement == element)
    {
        if (NULL != m_pParentTable)
        {
            return (m_pParentTable->isAKnownColumnInHierarchy (columnName));
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (true);
    }
}

bool OrmTable::isAnIntegerColumn (const string &columnName)
{
    UI32 columnVectorSize = m_columns.size ();

    for (UI32 i = 0; i < columnVectorSize; i++)
    {
        if (columnName == m_columns[i]->getName ())
        {
            string sqlType  = m_columns[i]->getSqlType ();

            if (("integer" == sqlType) || ( "bigint" == sqlType))
            {
                return (true);
            }

            break;
        }
    }

    return (false);
}

void OrmTable::addColumn (OrmColumn *pOrmColumn)
{
    if (false == isAKnownColumn (pOrmColumn->getName ()))
    {
        m_ormEntities.push_back (pOrmColumn);
        m_columns.push_back (pOrmColumn);
        m_columnNames[pOrmColumn->getName ()] = pOrmColumn->getName ();
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "OrmTable::addColumn : Duplicate Column Names are not allowed.  Column Name : \"" + pOrmColumn->getName () + "\"");
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool OrmTable::isAKnownRelation (const string &relationName) const
{
    map<string, OrmRelation *>::const_iterator element    = m_relationsByNames.find (relationName);
    map<string, OrmRelation *>::const_iterator endElement = m_relationsByNames.end ();

    if (endElement == element)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool OrmTable::isAKnownRelationInHierarchy (const string &relationName, OrmRelationType &ormRelationType) const
{
    map<string, OrmRelation *>::const_iterator element    = m_relationsByNames.find (relationName);
    map<string, OrmRelation *>::const_iterator endElement = m_relationsByNames.end ();

    if (endElement == element)
    {
        if (NULL != m_pParentTable)
        {
            return (m_pParentTable->isAKnownRelationInHierarchy (relationName, ormRelationType));
        }
        else
        {
            return (false);
        }
    }
    else
    {
        ormRelationType = (element->second)->getRelationType ();

        return (true);
    }
}

void OrmTable::addRelation (OrmRelation *pOrmRelation)
{
    if (false == isAKnownRelation (pOrmRelation->getName ()))
    {
        m_ormEntities.push_back (pOrmRelation);
        m_relations.push_back (pOrmRelation);
        m_relationsByNames[pOrmRelation->getName ()] = pOrmRelation;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "OrmTable::addRelation : Duplicate Relation Names are not allowed.  Relation Name : \"" + pOrmRelation->getName () + "\"");
        prismAssert (false, __FILE__, __LINE__);
    }
}

string OrmTable::getSqlForCreate (const string &schema) const
{
    UI32   numberOfOrmEntities   = m_ormEntities.size ();
    UI32   i                     = 0;
    string sqlForCreate;
    string tempSql;
    bool   entitySeparatorNeeded = false;
    bool   isThereAnySimpleColumnInThisTable = false;

    sqlForCreate += "CREATE TABLE " + schema + "." + m_name + "\n";
    sqlForCreate += "(\n";

    for (i = 0; i < numberOfOrmEntities; i++)
    {
        tempSql = (m_ormEntities[i])->getSqlForCreate (schema);

        if (false == (tempSql.empty ()))
        {
            if (true == entitySeparatorNeeded)
            {
                sqlForCreate += ",\n";
            }
            else
            {
                entitySeparatorNeeded = true;
            }

            sqlForCreate += tempSql;

            isThereAnySimpleColumnInThisTable = true;
        }
    }

    UI32                    numberOfUserDefinedKeyCombination = m_userDefinedKeyCombination.size ();
    string                  sqlForUniqueConstraint;
    bool                    isTableNameFoundInAllCompostionTableSet = false;
    set<string>             allCompostionTableNames;
    set<string>::iterator   it;

    OrmRepository::getAllCompositionTableName (allCompostionTableNames);

    it = allCompostionTableNames.find (m_name);

    if (it != allCompostionTableNames.end ())
    {
        isTableNameFoundInAllCompostionTableSet = true;
    }

    if ((0 < numberOfUserDefinedKeyCombination) && (true != isTableNameFoundInAllCompostionTableSet))
    {
        for (UI32 keyNumber = 0; keyNumber < numberOfUserDefinedKeyCombination; keyNumber ++)
        {
            if (keyNumber != 0)
            {
                sqlForUniqueConstraint  += ", ";
            }

            string userDefinedKeyField = m_userDefinedKeyCombination [keyNumber];

            Attribute *pAttribute = (m_userDefinedKeyCombinationWithTypes.find (userDefinedKeyField))->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (AttributeType::AttributeTypeObjectId == pAttribute->getAttributeType ())
            {
                sqlForUniqueConstraint += userDefinedKeyField + "ClassId, " + userDefinedKeyField + "InstanceId";
            }
            else
            {
                sqlForUniqueConstraint += userDefinedKeyField;
            }
        }

        // Add ownerpartitionManagedObjectId to unique key constraints.

        string ownerPartitionMOObjectIdFieldName = "ownerPartitionManagedObjectId";

        sqlForUniqueConstraint +=  ", " + ownerPartitionMOObjectIdFieldName + "ClassId, " + ownerPartitionMOObjectIdFieldName + "InstanceId";

        if (true == m_isALocalManagedObject)
        {
            string ownerWaveNodeObjectIdFieldName = "ownerWaveNodeObjectId";

            sqlForUniqueConstraint +=  ", " + ownerWaveNodeObjectIdFieldName + "ClassId, " + ownerWaveNodeObjectIdFieldName + "InstanceId";
        }

        if (true == isThereAnySimpleColumnInThisTable)
        {
            sqlForCreate += ",\n ";
        }


        sqlForCreate += "CONSTRAINT " + m_name + "_unique UNIQUE ("+ sqlForUniqueConstraint + ")";
    }

    sqlForCreate += "\n)";

    if (("" != m_derivedFromClassName) && (false == (OrmRepository::isAMostBaseClass (m_derivedFromClassName))))
    {
        sqlForCreate += " INHERITS (" + schema + "." + m_derivedFromClassName + ")";
    }

    sqlForCreate += ";\n";

    return (sqlForCreate);
#if 0
    UI32   numberOfColumns             = m_columns.size ();
    UI32   numberOfRelations             = m_relations.size ();
    UI32   i                             = 0;
    UI32   numberOfOneToOneRelations     = 0;
    UI32   tempNumberOfOneToOneRelations = 0;

    string sqlForCreate;

    sqlForCreate += "CREATE TABLE " + schema + "." + m_name + "\n";
    sqlForCreate += "(\n";

    for (i = 0; i < numberOfColumns; i++)
    {
        sqlForCreate += (m_columns[i])->getSqlForCreate ();

        if ((numberOfColumns - 1) != i)
        {
            sqlForCreate += ",\n";
        }
    }

    numberOfOneToOneRelations = 0;

    for (i = 0; i < numberOfRelations; i++)
    {
        if (ORM_RELATION_TYPE_ONE_TO_ONE == ((m_relations[i])->getRelationType ()))
        {
            numberOfOneToOneRelations++;
        }
    }

    if (0 < numberOfOneToOneRelations)
    {
        sqlForCreate += ",\n";
    }

    for (i = 0; i < numberOfRelations; i++)
    {
        sqlForCreate += (m_relations[i])->getSqlForCreate ();

        if (ORM_RELATION_TYPE_ONE_TO_ONE == ((m_relations[i])->getRelationType ()))
        {
            tempNumberOfOneToOneRelations++;
        }

        if (tempNumberOfOneToOneRelations < numberOfOneToOneRelations)
        {
            sqlForCreate += ",\n";
        }
    }

    sqlForCreate += "\n)";

    if (("" != m_derivedFromClassName) && (false == (OrmRepository::isAMostBaseClass (m_derivedFromClassName))))
    {
        sqlForCreate += " INHERITS (" + schema + "." + m_derivedFromClassName + ")";
    }

    sqlForCreate += ";\n";

    return (sqlForCreate);
#endif
}

string OrmTable::getSqlForCreate2 (const string &schema) const
{
    UI32   numberOfRelations = m_relations.size ();
    UI32   i                 = 0;

    string name              = getName ();
    string sqlForCreate2;

    for (i = 0; i < numberOfRelations; i++)
    {
        sqlForCreate2 += (m_relations[i])->getSqlForCreate2 (name, schema) + "\n";
    }

    return (sqlForCreate2);
}

string OrmTable::getSqlForCreate2 (const string &schema, const string &relationName)
{
    OrmRelation* pOrmRelation = m_relationsByNames[relationName];
    prismAssert(NULL != pOrmRelation, __FILE__, __LINE__);
    string name              = getName ();
    string sqlForCreatingAuxilliaryTable = pOrmRelation->getSqlForCreate2(name, schema);
    return sqlForCreatingAuxilliaryTable;
}


UI32 OrmTable::getTableId () const
{
    return (m_tableId);
}

void OrmTable::setTableId (const UI32 &tableId)
{
    m_tableId = tableId;
}
/*
UI32 OrmTable::getOldTableId () const
{
    return (m_oldTableId);
}

void OrmTable::setOldTableId (const UI32 oldTableId)
{
    m_oldTableId = oldTableId;
}
*/
string OrmTable::getDerivedFromClassName () const
{
    return (m_derivedFromClassName);
}

OrmTable *OrmTable::getPParentTable () const
{
    return (m_pParentTable);
}

void OrmTable::setPParentTable (OrmTable *pParentTable)
{
    m_pParentTable = pParentTable;

    pParentTable->addDerivation (this);
}

void OrmTable::addDerivation (OrmTable *pOrmTable)
{
    m_derivations.push_back (pOrmTable);
}

void OrmTable::getReferences (vector<string> &references)
{
    UI32   numberOfRelations = m_relations.size ();
    UI32   i                 = 0;

    for (i = 0; i < numberOfRelations; i++)
    {
        string relatedTo          = m_relations[i]->getRelatedTo ();
        UI32   j                  = 0;
        UI32   numberOfReferences = references.size ();
        bool   isAlreadyPresent   = false;

        for (j = 0; j < numberOfReferences; j++)
        {
            if (relatedTo == references[j])
            {
                isAlreadyPresent = true;
                break;
            }
        }

        if (false == isAlreadyPresent)
        {
            references.push_back (relatedTo);
        }
    }

    UI32 numberOfOneToManyRelations = getNumberOfOneToManyRelations ();

    if (0 < numberOfOneToManyRelations)
    {
        string relatedTo          = m_name;
        UI32   j                  = 0;
        UI32   numberOfReferences = references.size ();
        bool   isAlreadyPresent   = false;

        for (j = 0; j < numberOfReferences; j++)
        {
            if (relatedTo == references[j])
            {
                isAlreadyPresent = true;
                break;
            }
        }

        if (false == isAlreadyPresent)
        {
            references.push_back (relatedTo);
        }
    }
}

void OrmTable::getAllDerivedTableIds (vector<OrmTable *> &derivedTableIds)
{
    UI32 numberOfDerivations = m_derivations.size ();
    UI32 i                   = 0;

    // Recursively obtain all the derived table ids for all of the immediate derivations.

    for (i = 0; i < numberOfDerivations; i++)
    {
        derivedTableIds.push_back (m_derivations[i]);
        m_derivations[i]->getAllDerivedTableIds (derivedTableIds);
    }
}

void OrmTable::lock ()
{
    m_mutex.lock ();
}

void OrmTable::unlock ()
{
    m_mutex.unlock ();
}

UI32 OrmTable::getNextInstanceId ()
{
    UI32 nextInstanceId = 0;

    lock ();

    m_nextInstanceId++;
    nextInstanceId = m_nextInstanceId;

    unlock ();

    return (nextInstanceId);
}

void OrmTable::setNextInstanceId (const UI32 &nextInstanceId)
{
    lock ();

    m_nextInstanceId = nextInstanceId;

    unlock ();
}

UI32 OrmTable::getCurrentNextInstanceId ()
{
    UI32 nextInstanceId = 0;

    lock ();

    nextInstanceId = m_nextInstanceId;

    unlock ();

    return (nextInstanceId);
}

OrmTable *OrmTable::getPMostBaseTable ()
{
    return (m_pMostBaseTable);
}

void OrmTable::setPMostBaseTable (OrmTable *pMostBaseTable)
{
    m_pMostBaseTable = pMostBaseTable;
}

void OrmTable::getSqlToCountNumberOfEntriesInTable  (const string &schema, string &sqlString)
{
    sqlString = "select count (*) from " + schema + "." + m_name + ";";
}

string OrmTable::getSqlToDeleteAllEntries (const string &schema) const
{
    return ("DELETE FROM " + schema + "." + m_name + ";");
}

string OrmTable::getSqlToSelectivelyDeleteEntries (const string &schema) const
{
    return (string ("DELETE FROM ") + schema + string (".") + m_name + string (" WHERE objectIdClassId = ") + m_tableId + string (";"));
}

string OrmTable::getSqlToTruncate (const string &schema) const
{
    return ("TRUNCATE " + schema + "." + m_name + ";");
}

string OrmTable::getSqlToQueryAllEntries (const string &sourceSchema) const
{
    return (string ("SELECT * FROM ") + sourceSchema + string (".") + m_name + string (" WHERE objectIdClassId = ") + m_tableId + string (";"));
}

string OrmTable::getSqlToInsertEntries (const string &destSchema, const string &values) const
{
    return ("INSERT INTO " + destSchema + "." + m_name + " VALUES (" + values + ");");
}

string OrmTable::getSqlToCopy (const string &srcSchema, const string &destSchema) const
{
    return ("INSERT INTO " + destSchema + "." + m_name + " SELECT * FROM ONLY " + srcSchema + "." + m_name + ";");
}

string OrmTable::getSqlToDrop (const string &schema) const
{
    return ("DROP TABLE " + schema + "." + m_name + ";");
}

void OrmTable::getAuxilliaryTableNames (vector<string> &auxilliaryTableNames) const
{
    UI32         numberOfRelations    = m_relations.size ();
    UI32         i                    = 0;
    string       auxilliaryTableName;
    OrmRelation *pOrmRelation         = NULL;

    auxilliaryTableNames.clear ();

    for (i = 0; i < numberOfRelations; i++)
    {
        pOrmRelation = m_relations[i];

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        auxilliaryTableName = pOrmRelation->getAuxilliaryTableName (m_name);

        if ("" != auxilliaryTableName)
        {
            auxilliaryTableNames.push_back (auxilliaryTableName);
        }
    }
}

void OrmTable::getAuxilliaryTableDetailsAppended (vector<string> &auxilliaryTableNames, vector<string> &parentTableNames, vector<string> &relatedToTableNames) const
{
    UI32         numberOfRelations    = m_relations.size ();
    UI32         i                    = 0;
    string       auxilliaryTableName;
    OrmRelation *pOrmRelation         = NULL;

    for (i = 0; i < numberOfRelations; i++)
    {   
        pOrmRelation = m_relations[i];

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        auxilliaryTableName = pOrmRelation->getAuxilliaryTableName (m_name);

        if ("" != auxilliaryTableName)
        {
            auxilliaryTableNames.push_back (auxilliaryTableName);
            parentTableNames.push_back     (m_name);
            relatedToTableNames.push_back  (pOrmRelation->getRelatedTo());
        }
    }
}

void OrmTable::getAuxilliaryTableNamesAppended (vector<string> &auxilliaryTableNames) const
{
    UI32         numberOfRelations    = m_relations.size ();
    UI32         i                    = 0;
    string       auxilliaryTableName;
    OrmRelation *pOrmRelation         = NULL;

    for (i = 0; i < numberOfRelations; i++)
    {
        pOrmRelation = m_relations[i];

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        auxilliaryTableName = pOrmRelation->getAuxilliaryTableName (m_name);

        if ("" != auxilliaryTableName)
        {
            auxilliaryTableNames.push_back (auxilliaryTableName);
        }
    }
}

void OrmTable::getAllAuxilliaryTableNames (vector<string> &auxilliaryTableNames) const
{
    UI32         numberOfRelations    = m_relations.size ();
    UI32         i                    = 0;
    string       auxilliaryTableName;
    OrmRelation *pOrmRelation         = NULL;

    for (i = 0; i < numberOfRelations; i++)
    {
        pOrmRelation = m_relations[i];

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        auxilliaryTableName = pOrmRelation->getAuxilliaryTableName (m_name);

        if ("" != auxilliaryTableName)
        {
            auxilliaryTableNames.push_back (auxilliaryTableName);
        }
    }

    if (NULL != m_pParentTable)
    {
        m_pParentTable->getAllAuxilliaryTableNames (auxilliaryTableNames);
    }
}

void OrmTable::getAllAuxilliaryTableNames (vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes) const
{
    UI32                numberOfRelations       = m_relations.size ();
    UI32                i                       = 0;
    string              auxilliaryTableName;
    OrmRelationUmlType  auxilliaryTableUmlType;
    OrmRelation        *pOrmRelation            = NULL;

    for (i = 0; i < numberOfRelations; i++)
    {
        pOrmRelation = m_relations[i];

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        auxilliaryTableName = pOrmRelation->getAuxilliaryTableName (m_name);
        auxilliaryTableUmlType = pOrmRelation->getRelationUmlType ();

        if ("" != auxilliaryTableName)
        {
            auxilliaryTableNames.push_back (auxilliaryTableName);
            auxilliaryTableUmlTypes.push_back (auxilliaryTableUmlType);
        }
    }

    if (NULL != m_pParentTable)
    {
        m_pParentTable->getAllAuxilliaryTableNames (auxilliaryTableNames, auxilliaryTableUmlTypes);
    }

}

void OrmTable::getAllAuxilliaryTableNames (vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes, const vector<string> &selectedEntities) const
{
    UI32                numberOfSelectedEntities = selectedEntities.size ();
    UI32                i                        = 0;
    string              auxilliaryTableName;
    OrmRelationUmlType  auxilliaryTableUmlType;
    OrmRelation        *pOrmRelation             = NULL;
    string              selectedEntity;

    if (0 == numberOfSelectedEntities)
    {
        getAllAuxilliaryTableNames (auxilliaryTableNames, auxilliaryTableUmlTypes);
    }
    else
    {
        for (i = 0; i < numberOfSelectedEntities; i++)
        {
            selectedEntity = selectedEntities[i];

            if (true == (isAKnownRelation (selectedEntity)))
            {
                map<string, OrmRelation *>::const_iterator element = m_relationsByNames.find (selectedEntity);

                pOrmRelation = element->second;

                prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

                auxilliaryTableName = pOrmRelation->getAuxilliaryTableName (m_name);
                auxilliaryTableUmlType = pOrmRelation->getRelationUmlType ();

                if ("" != auxilliaryTableName)
                {
                    auxilliaryTableNames.push_back (auxilliaryTableName);
                    auxilliaryTableUmlTypes.push_back (auxilliaryTableUmlType);
                }
            }
        }

        if (NULL != m_pParentTable)
        {
            m_pParentTable->getAllAuxilliaryTableNames (auxilliaryTableNames, auxilliaryTableUmlTypes, selectedEntities);
        }
    }
}

void OrmTable::getCompositionFieldNamesInHierarchy (vector<string> &compositionFieldNames) const
{
    UI32         numberOfRelations    = m_relations.size ();
    UI32         i                    = 0;
    string       compositionFieldName;
    OrmRelation *pOrmRelation         = NULL;

    for (i = 0; i < numberOfRelations; i++)
    {
        pOrmRelation = m_relations[i];
    
        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        if (ORM_RELATION_UML_TYPE_COMPOSITION == pOrmRelation->getRelationUmlType ())
        {
            compositionFieldName = pOrmRelation->getName ();

            if ("" != compositionFieldName)
            {
                compositionFieldNames.push_back (compositionFieldName);
            }
        }
    }

    if (NULL != m_pParentTable)
    {
        m_pParentTable->getCompositionFieldNamesInHierarchy (compositionFieldNames);
    }
}

UI32 OrmTable::getNumberOfOneToManyRelations () const
{
    UI32 numberOfRelations          = m_relations.size ();
    UI32 i                          = 0;
    UI32 numberOfOneToManyRelations = 0;

    trace (TRACE_LEVEL_DEBUG, string ("OrmTable::getNumberOfOneToManyRelations : ") + getName () + string (" :"));

    for (i = 0; i < numberOfRelations; i++)
    {
        if (ORM_RELATION_TYPE_ONE_TO_MANY == ((m_relations[i])->getRelationType ()))
        {
            trace (TRACE_LEVEL_DEBUG, string ("OrmTable::getNumberOfOneToManyRelations :     ") + (m_relations[i])->getName ());
            numberOfOneToManyRelations++;
        }
    }

    return (numberOfOneToManyRelations);
}

void OrmTable::addDerivationsInstancesTable (const string &derivationsInstancesTable)
{
    UI32 numberOfDerivationsInstancesTables = m_derivationsInstancesTables.size ();
    UI32 i                                  = 0;
    bool found                              = false;

    for (i = 0; i < numberOfDerivationsInstancesTables; i++)
    {
        if (derivationsInstancesTable == m_derivationsInstancesTables[i])
        {
            found = true;
            break;
        }
    }

    if (false == found)
    {
        m_derivationsInstancesTables.push_back (derivationsInstancesTable);
    }
}

void OrmTable::computeDerivationsInstancesTables ()
{
    OrmTable *pOrmTable                  = this;
    UI32      numberOfOneToManyRelations = 0;
    string    tableName;


    trace (TRACE_LEVEL_DEVEL, string ("OrmTable::computeDerivationsInstancesTables : Derivations Instances Tables For ") + getName () + string (" :"));

    while (NULL != pOrmTable)
    {
        numberOfOneToManyRelations = pOrmTable->getNumberOfOneToManyRelations ();

        if (0 < numberOfOneToManyRelations)
        {
            tableName = pOrmTable->getName ();

            addDerivationsInstancesTable (tableName);

            trace (TRACE_LEVEL_DEVEL, string ("OrmTable::computeDerivationsInstancesTables :  ") + tableName + string (" (") + numberOfOneToManyRelations + string (")"));
        }

        pOrmTable = pOrmTable->getPParentTable ();
    }

    UI32                numberOfRelations   = m_relations.size ();
    UI32                i                  = 0;
    string            relatedTo;
    OrmTable           *pRelatedToOrmTable  = NULL;
    vector<OrmTable *>  derivedTables;
    UI32                j                  = 0;
    UI32                numberOfDerivations = 0;

    for (i = 0; i < numberOfRelations; i++)
    {
        relatedTo = (m_relations[i])->getRelatedTo ();

        trace (TRACE_LEVEL_DEVEL, string ("OrmTable::computeDerivationsInstancesTables :  RELATED TO : ") + relatedTo);

        pRelatedToOrmTable = OrmRepository::getTableByName (relatedTo);

        prismAssert (NULL != pRelatedToOrmTable, __FILE__, __LINE__);

        pRelatedToOrmTable->addDerivationsInstancesTable (relatedTo);

        trace (TRACE_LEVEL_DEVEL, string ("OrmTable::computeDerivationsInstancesTables :      Adding ") + relatedTo +  string (" to ") + relatedTo);

        derivedTables.clear ();
        pRelatedToOrmTable->getAllDerivedTableIds (derivedTables);
        numberOfDerivations = derivedTables.size ();

        for (j = 0; j < numberOfDerivations; j++)
        {
            (derivedTables[j])->addDerivationsInstancesTable (relatedTo);

            trace (TRACE_LEVEL_DEVEL, string ("OrmTable::computeDerivationsInstancesTables :      Adding ") + relatedTo +  string (" to ") + derivedTables[j]->getName ());
        }
    }
}

void OrmTable::printDerivationsInstancesTables (const string &prefix, string &outputString)
{
    UI32    numberOfDerivationsInstancesTables  = m_derivationsInstancesTables.size ();
    UI32    i                                   = 0;

    for (i = 0; i < numberOfDerivationsInstancesTables; i++)
    {
        outputString += prefix + m_derivationsInstancesTables[i] + "\r\n";
    }
}

void OrmTable::getSqlForInsertForDerivationsInstances (string &sqlForInsertForDerivationsInstnces, const UI64 &instanceId, const string &schema)
{
    UI32 numberOfDerivationsInstancesTables = m_derivationsInstancesTables.size ();
    UI32 i                                  = 0;

    for (i = 0; i < numberOfDerivationsInstancesTables; i++)
    {
        sqlForInsertForDerivationsInstnces += string ("INSERT INTO ") + schema + "." + m_derivationsInstancesTables[i] + string ("DerivationsInstances VALUES (") + instanceId + string (");\n");
    }
}

void OrmTable::getSqlForDeleteForDerivationsInstances (string &sqlForDeleteForDerivationsInstnces, const UI64 &instanceId, const string &schema)
{
    UI32 numberOfDerivationsInstancesTables = m_derivationsInstancesTables.size ();
    UI32 i                                  = 0;

    for (i = 0; i < numberOfDerivationsInstancesTables; i++)
    {
        sqlForDeleteForDerivationsInstnces += string ("DELETE FROM ") + schema + "." + m_derivationsInstancesTables[i] + string ("DerivationsInstances WHERE ObjectIdInstanceId = ") + instanceId + string (";\n");
    }
}

string OrmTable::getSqlToSelectivelyDeleteDerivationsInstances (const string &schema)
{
    string  sqlToSelectivelyDeleteDerivationsInstances;
    UI32    numberOfDerivationsInstancesTables          = m_derivationsInstancesTables.size ();
    UI32    i                                           = 0;

    for (i = 0; i < numberOfDerivationsInstancesTables; i++)
    {
        sqlToSelectivelyDeleteDerivationsInstances += string ("DELETE FROM ") + schema + "." + m_derivationsInstancesTables[i] + string ("DerivationsInstances WHERE ObjectIdInstanceId IN (SELECT ObjectIdInstanceId FROM ") + schema + "." + m_derivationsInstancesTables[i] + " WHERE ObjectIdClassId = " + getTableId () + string (");\n");
    }

    trace (TRACE_LEVEL_DEBUG, string ("OrmTable::getSqlToSelectivelyDeleteDerivationsInstances : ") + sqlToSelectivelyDeleteDerivationsInstances);
    return (sqlToSelectivelyDeleteDerivationsInstances);

}

string OrmTable::getSqlToSelectivelyTruncateFromDerivationsInstances (const string &schema)
{
    string  sqlToSelectivelyDeleteDerivationsInstances;
    UI32    numberOfDerivationsInstancesTables          = m_derivationsInstancesTables.size ();
    UI32    i                                           = 0;

    for (i = 0; i < numberOfDerivationsInstancesTables; i++)
    {
        sqlToSelectivelyDeleteDerivationsInstances += string ("TRUNCATE ") + schema + "." + m_derivationsInstancesTables[i] + string ("DerivationsInstances CASCADE;");
    }

    trace (TRACE_LEVEL_DEBUG, string ("OrmTable::getSqlToSelectivelyDeleteDerivationsInstances : ") + sqlToSelectivelyDeleteDerivationsInstances);
    return (sqlToSelectivelyDeleteDerivationsInstances);

}

string OrmTable::getSqlToSelectivelyDeleteDerivationsInstances (const string &schema, const string &whereClause)
{
    string  sqlToSelectivelyDeleteDerivationsInstances;
    UI32    numberOfDerivationsInstancesTables          = m_derivationsInstancesTables.size ();
    UI32    i                                           = 0;

    for (i = 0; i < numberOfDerivationsInstancesTables; i++)
    {
        sqlToSelectivelyDeleteDerivationsInstances += string ("DELETE FROM ") + schema + "." + m_derivationsInstancesTables[i] + string ("DerivationsInstances WHERE ObjectIdInstanceId IN ") + whereClause + string (";\n");
    }

    trace (TRACE_LEVEL_DEBUG, string ("OrmTable::getSqlToSelectivelyDeleteDerivationsInstances : ") + sqlToSelectivelyDeleteDerivationsInstances);
    return (sqlToSelectivelyDeleteDerivationsInstances);

}

void OrmTable::setUserDefinedKeyCombinationWithTypes (const map<string, Attribute*> &userDefinedKeyCombinationWithTypes, const vector<string> &userDefinedKeyCombination)
{
    m_userDefinedKeyCombinationWithTypes = userDefinedKeyCombinationWithTypes;
    m_userDefinedKeyCombination          = userDefinedKeyCombination;
}

map<string, Attribute*> OrmTable::getUserDefinedKeyCombinationWithTypes (vector<string> &userDefinedKeyCombination) const
{
    userDefinedKeyCombination = m_userDefinedKeyCombination;

    return (m_userDefinedKeyCombinationWithTypes);
}

const vector<string> OrmTable::getUserDefinedFieldNamesForUpgrade () 
{
    return (m_userDefinedKeyCombination);
}

const vector<string> OrmTable::getUserDefinedFieldTypesForUpgrade () 
{
    map<string, Attribute*>::iterator   iter;
    vector<string>                      userDefinedFieldTypes;
    
    for (UI32 keyNo = 0; keyNo < m_userDefinedKeyCombination.size (); keyNo++)
    {
        iter = m_userDefinedKeyCombinationWithTypes.find(m_userDefinedKeyCombination[keyNo]);

        if (m_userDefinedKeyCombinationWithTypes.end () == iter)
        {
            trace (TRACE_LEVEL_ERROR, "OrmTable::getUserDefinedFieldTypesForUpgrade: failed to get an entry for " + m_userDefinedKeyCombination[keyNo] + " in m_userDefinedKeyCombinationWithTypes");
            prismAssert (false, __FILE__, __LINE__);
        }

        Attribute* pAttribute = iter->second;

        if (NULL != pAttribute)
        {
            userDefinedFieldTypes.push_back (FrameworkToolKit::localizeToSourceCodeEnum ((pAttribute->getAttributeType()).getAttributeTypeResourceId()));
        }
        else
        {
            userDefinedFieldTypes.push_back ("NO_TYPE");
        }
    }        
    return (userDefinedFieldTypes);
}

void OrmTable::setEmptyNeededOnPersistentBoot (const bool &emptyNeededOnPersistentBoot)
{
    m_emptyNeededOnPersistentBoot = emptyNeededOnPersistentBoot;
}

bool OrmTable::getEmptyNeededOnPersistentBoot () const
{
    return (m_emptyNeededOnPersistentBoot);
}

void OrmTable::validateRelations (OrmTable *pOrmTableToValidate)
{
    UI32            j;
    vector<string>  references;
    UI32            numberOfReferences;

    pOrmTableToValidate->getReferences (references);
    numberOfReferences = references.size ();

    for (j = 0; j < numberOfReferences; j++)
    {
        // Check if this table is one of the references for the table to validate

        if (getName () == references[j])
        {
            vector<string>  compositionRelations;
            set<string>     relatedTableNames;

            relatedTableNames.insert (getName ());

            pOrmTableToValidate->getCompositionRelationTargetTableNamesFromSet (relatedTableNames, compositionRelations);

           if ((getEmptyNeededOnPersistentBoot () != pOrmTableToValidate->getEmptyNeededOnPersistentBoot ())
                && ((false == pOrmTableToValidate->getEmptyNeededOnPersistentBoot ()) || (false == compositionRelations.empty ())))
           {
               trace (TRACE_LEVEL_ERROR, string ("OrmTable::validateRelations : emptyNeeded flag in ") + pOrmTableToValidate->getName () + " should be same as " + getName () + ".  Please use setEmptyNeededOnPersistentBoot to set the flag." );

               prismAssert (false, __FILE__, __LINE__);
           }

           if ((getEmptyNeededOnPersistentBootWithDefault () != pOrmTableToValidate->getEmptyNeededOnPersistentBootWithDefault ())
               && ((false == pOrmTableToValidate->getEmptyNeededOnPersistentBootWithDefault ()) || (false == compositionRelations.empty ())))
           {
               trace (TRACE_LEVEL_ERROR, string ("OrmTable::validateRelations : emptyNeededWithDefault flag in ") + pOrmTableToValidate->getName () + " should be same as " + getName () + ".  Please use setEmptyNeededOnPersistentBootWithDefault to set the flag." );

               prismAssert (false, __FILE__, __LINE__);
           }

           // Next check all derivations of table to validate also have the same value

           UI32                 d;
           vector<OrmTable *>   derivedTableIds;
           UI32                 numberOfDerivations;

           pOrmTableToValidate->getAllDerivedTableIds (derivedTableIds);

           numberOfDerivations = derivedTableIds.size ();

           for (d = 0; d < numberOfDerivations; d++)
           {
            if (getEmptyNeededOnPersistentBoot () != derivedTableIds[d]->getEmptyNeededOnPersistentBoot ())
            {
               trace (TRACE_LEVEL_ERROR, string ("OrmTable::validateRelations : emptyNeeded flag in ") + derivedTableIds[d]->getName () + " should be same as " + getName () + ". Please use setEmptyNeededOnPersistentBoot to set the flag." );

               prismAssert (false, __FILE__, __LINE__);

            }

               if (getEmptyNeededOnPersistentBootWithDefault () != derivedTableIds[d]->getEmptyNeededOnPersistentBootWithDefault ())
               {
                  trace (TRACE_LEVEL_ERROR, string ("OrmTable::validateRelations : emptyNeededWithDefault flag in ") + derivedTableIds[d]->getName () + " should be same as " + getName () + ". Please use setEmptyNeededOnPersistentBootWithDefault to set the flag." );

                  prismAssert (false, __FILE__, __LINE__);
               }
           }

           break;
        }
    }
}

void OrmTable::getDetailsForRelationships (vector<string> &relatedToTables, vector<OrmRelationType> &relationTypes, vector<OrmRelationUmlType> &relationUmlTypes)
{
    UI32         numberOfRelations    = m_relations.size ();
    UI32         i                    = 0;
    string       auxilliaryTableName;
    OrmRelation *pOrmRelation         = NULL;

    for (i = 0; i < numberOfRelations; i++)
    {
        pOrmRelation = m_relations[i];

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        relatedToTables.push_back (pOrmRelation->getRelatedTo ());
        relationTypes.push_back (pOrmRelation->getRelationType ());
        relationUmlTypes.push_back (pOrmRelation->getRelationUmlType ());
    }
}

void OrmTable::getDetailsForRelationshipsInHierarchy (vector<OrmTable *> &relatedFromTables, vector<string> &relatedToTables, vector<OrmRelationType> &relationTypes, vector<OrmRelationUmlType> &relationUmlTypes, vector<string> &relationNames)
{
    UI32         numberOfRelations    = m_relations.size ();
    UI32         i                    = 0;
    string       auxilliaryTableName;
    OrmRelation *pOrmRelation         = NULL;

    for (i = 0; i < numberOfRelations; i++)
    {
        pOrmRelation = m_relations[i];

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        relatedFromTables.push_back (this);
        relatedToTables.push_back (pOrmRelation->getRelatedTo ());
        relationTypes.push_back (pOrmRelation->getRelationType ());
        relationUmlTypes.push_back (pOrmRelation->getRelationUmlType ());
        relationNames.push_back (pOrmRelation->getName ());
    }

    if (NULL != m_pParentTable)
    {
        m_pParentTable->getDetailsForRelationshipsInHierarchy (relatedFromTables, relatedToTables, relationTypes, relationUmlTypes, relationNames);
    }
}

bool OrmTable::getIsALocalManagedObject () const
{
    return (m_isALocalManagedObject);
}


void    OrmTable::addFieldNameToUpgradeString (const string & attributeName)
{
      m_fieldNamesForUpgrade.push_back (attributeName);
}

void    OrmTable::addFieldSqlTypeToUpgradeString (const string & attributeSqlType)
{
      m_fieldTypesForUpgrade.push_back (attributeSqlType);
}

void    OrmTable::addDbFieldSqlTypeToUpgradeString (const string & attributeSqlType)
{
      m_fieldDbTypesForUpgrade.push_back (attributeSqlType);
}

const vector<string> & OrmTable::getFieldNamesForUpgrade () const
{
    return (m_fieldNamesForUpgrade);
}

const vector<string> & OrmTable::getFieldTypesForUpgrade () const
{
   return (m_fieldTypesForUpgrade);
}

const vector<string> & OrmTable::getDbFieldTypesForUpgrade () const
{
   return (m_fieldDbTypesForUpgrade);
}

void OrmTable::setUpgradeStrings ()
{
    UI32   numberOfEntities      = m_ormEntities.size();

    for (UI32 i = 0; i < numberOfEntities; ++i)
    {
        OrmEntity *entity = m_ormEntities[i];
        addFieldNameToUpgradeString (entity->getName ());
        addFieldSqlTypeToUpgradeString (entity->getTypeForUpgradeMO ());

        // Dynamic cast the OrmEntity to a OrmColumn .. 
        // If not null then get the sql type otherwise its a relations and thus the type should be "r"
        OrmColumn *column = dynamic_cast<OrmColumn *> (entity);
        if (NULL != column)
        {
            addDbFieldSqlTypeToUpgradeString (column->getSqlType ());
        }
        else 
        {
            addDbFieldSqlTypeToUpgradeString ("r");
        }
    }
}

void OrmTable::getAllEntityNamesInHierarchy (vector<string> &entityNamesInHierarchy)
{
    if (NULL != m_pParentTable)
    {
        m_pParentTable->getAllEntityNamesInHierarchy (entityNamesInHierarchy);
    }

    UI32   numberOfEntities = m_ormEntities.size ();
    UI32   i                 = 0;

    for (i = 0; i < numberOfEntities; i++)
    {
        entityNamesInHierarchy.push_back (m_ormEntities[i]->getName ());
    }

}

void OrmTable::addManagedObjectAttributes (Attribute *pAttribute)
{
    Attribute *attr = pAttribute->clone ();

    m_managedObjectAttributes[attr->getAttributeName ()] = attr;
}

Attribute* OrmTable::getAttributeForFieldNameInManagedObject (const string &fieldName)
{
    map<string, Attribute *>::iterator temp = m_managedObjectAttributes.find (fieldName);

    if (temp != m_managedObjectAttributes.end ())
    {
        return (temp->second);
    }
    else
    {
        return (NULL);
    }
}

void OrmTable::cleanupManagedObjectAttributes ()
{
    m_managedObjectAttributes.clear ();
}

void OrmTable::getEntityDetails (const string &entityName, bool &isAColumn, bool &isARelation, OrmRelationType &ormRelationType)
{
    isAColumn = isAKnownColumnInHierarchy   (entityName);

    if (false == isAColumn)
    {
        isARelation = isAKnownRelationInHierarchy (entityName, ormRelationType);
    }
    else
    {
        isARelation = false;
    }
}

bool OrmTable::isASimpleObjectIdColumn (const string &entityName)
{
    const string entityNameClassId    = entityName + "ClassId";

    if (true == (isAKnownColumnInHierarchy (entityNameClassId)))
    {
        const string entityNameInstanceId = entityName + "InstanceId";

        if (true == (isAKnownColumnInHierarchy (entityNameInstanceId)))
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}


void OrmTable::removeOrmColumnFromTable(const std::string& columnName)
{
    UI32 i;
    UI32 bitmap=0;
    OrmColumn *pOrmColumn = NULL;
    for ( i=0;  ((bitmap !=  3 ) && (i < m_ormEntities.size())); i++ )
    {
        if (( (bitmap & 1)  == 0 ) && (m_ormEntities[i]->getName() == columnName ))
        {
            bitmap |= 1;
            m_ormEntities.erase(m_ormEntities.begin() + i);
        }

        if ( ( (bitmap & 2)  == 0 ) && (i < m_columns.size()) && (m_columns[i]->getName() == columnName ))
        {
            pOrmColumn = m_columns[i];
            m_columns.erase(m_columns.begin() + i);
            bitmap |= 2;
        }
    }
    if(pOrmColumn)
    {
        delete pOrmColumn;

    }

    map<string,string>::iterator element = m_columnNames.find(columnName);
    map<string,string>::iterator endElement = m_columnNames.end();

    if ( element != endElement )
    {
        m_columnNames.erase(element);
    }

}


string OrmTable::getSqlForRemoveColumn(const std::string& schema, const std::string& columnName)
{

    string sqlForRemoveColumn = "";

    for ( UI32 i=0 ; i <m_derivations.size(); i++ )
    {
        sqlForRemoveColumn = m_derivations[i]->getSqlForRemoveColumn(schema, columnName) + sqlForRemoveColumn;
    }
    string          auxilliaryTableName;
    OrmRelation     *pOrmRelation            = NULL;

    if (true == (isAKnownRelation (columnName)))
    {
        map<string, OrmRelation *>::const_iterator element = m_relationsByNames.find (columnName);

        pOrmRelation = element->second;

        prismAssert (NULL != pOrmRelation, __FILE__, __LINE__);

        auxilliaryTableName = pOrmRelation->getAuxilliaryTableName (m_name);
        string relatedToClass = pOrmRelation->getRelatedTo();
        OrmRepository *pOrmRepository = OrmRepository::getInstance();
        OrmTable *pRelatedOrmTable = pOrmRepository->getTableByName(relatedToClass);
        UI32 relatedTableId = pRelatedOrmTable->getTableId ();
        string relatedConstraintName = string ("t") + getTableId () + columnName + string ("t") + relatedTableId ;

        if ( ORM_RELATION_TYPE_ONE_TO_MANY == pOrmRelation->getRelationType())
        {

            if ("" != auxilliaryTableName)
            {
                sqlForRemoveColumn += "DROP TABLE " + schema + "." + auxilliaryTableName + " CASCADE;\n";

            }
        } else// if ( ORM_RELATION_TYPE_ONE_TO_ONE  == pOrmRelation->getRelationType())
        {
            sqlForRemoveColumn += "ALTER TABLE " + schema + "." + m_name + "  DROP COLUMN IF EXISTS " + columnName +  "classid" + " CASCADE ;\n";
            sqlForRemoveColumn += "ALTER TABLE " + schema + "." + m_name + "  DROP COLUMN IF EXISTS " + columnName +  "instanceid" + " CASCADE ;\n";
        }
        if ( "" != relatedToClass )
        {
            sqlForRemoveColumn += "DELETE FROM "+ schema +"." + relatedToClass + string(" WHERE ownermanagedobjectidclassid=")+  getTableId () + ";\n";
            sqlForRemoveColumn +=  string ("DELETE FROM ") + schema + "." + relatedToClass + string ("derivations WHERE objectidclassid=")+  getTableId () + string (";\n");
        }
    }

    map<string, string>::iterator element   = m_columnNames.find(columnName);
    map<string, string>::iterator endElement =  m_columnNames.end();
    if ( element != endElement )
    {
        sqlForRemoveColumn += "ALTER TABLE " + schema + "." + m_name + "  DROP COLUMN  IF EXISTS " + columnName + " CASCADE;\n";
        trace (TRACE_LEVEL_INFO, "OrmTable::getSqlForRemoveColumn Schema:" + schema + " Column:" + columnName + " Table:" + m_name + " SQL: " +sqlForRemoveColumn );
    }
    return  sqlForRemoveColumn;
}


/* Delete References from all derivations table */
string OrmTable::getSqlToDropDerivationTables(const std::string& schema, const std::string& tableName)
{
    string sqlForDrop = "";
    trace (TRACE_LEVEL_INFO, "OrmTable::getSqlToDropDerivationTables Schema:" +  schema +  " Table:" + tableName );
    SI32           i                     = 0;
    vector<string> references;
    vector<string> auxilliaryTableNames;
    UI32           numberOfReferences   = 0;
    OrmRepository *pOrmRepository = OrmRepository::getInstance();

    // Get the unique destinations in reference statements (all types of foreign key constraints destinations and 1->* foreign key sources as well)

    getReferences (references);

    numberOfReferences = references.size ();

    // First drop Auxilliary tables (used for 1-* UML associations/compositions).
    getAuxilliaryTableNames (auxilliaryTableNames);

    UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
    UI32 j                      = 0;

    for (j = 0; j < numberOfAuxilliaryTables; j++)
    {
        sqlForDrop += pOrmRepository->getSqlToDropAuxilliaryTable (auxilliaryTableNames[j], schema) + "\n";
    }

    sqlForDrop += getSqlToDrop (schema) + " \n";
    // Then drop the derivation tables at last.
    for (i = 0; i < ((SI32) numberOfReferences); i++)
    {
        if ( references[i] ==  tableName )
        {
            sqlForDrop += "DROP TABLE IF EXISTS " + schema + "." + tableName + "derivations CASCADE;\n";
            sqlForDrop += "DROP TABLE IF EXISTS " + schema + "." + tableName + "derivationsinstances CASCADE;\n";

        }
		else 
        {
            sqlForDrop += "DELETE FROM "+ schema +"." + references[i] + (" WHERE ownermanagedobjectidclassid=")+  getTableId () + ";\n";
        } 
    }
    trace (TRACE_LEVEL_INFO, "Reference OrmTable::getSqlToDropDerivationTables Schema:" + schema +  " Table:" + tableName + sqlForDrop );

    return (sqlForDrop);
    
    

}

void OrmTable::getDeivedTablesAndRelations(vector<OrmTable *> &derivedTables, vector<OrmRelation *> &relationColumn)
{
    derivedTables = m_derivations;
    relationColumn = m_relations;
}

string OrmTable::getSqlForRemoveTables(const std::string& schema, const std::string& tableName)
{
    string sqlForDrop = "";
    trace (TRACE_LEVEL_INFO, "OrmTable::getSqlForRemoveTables Schema:" + schema +  " Table:" + m_name);

    sqlForDrop += getSqlToDropDerivationTables(schema, tableName);
    
    OrmTable *pParent = this;
    /* Delete References from Parent where owner class id is from table to be removed*/
    while ( (pParent = pParent->getPParentTable()))
    {
        vector<string> auxilliaryTableNames;
        pParent->getAuxilliaryTableNames (auxilliaryTableNames);

        UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
        UI32 j                      = 0;

        for (j = 0; j < numberOfAuxilliaryTables; j++)
        {
            sqlForDrop += string ("DELETE FROM ") + schema + "." + auxilliaryTableNames[j] + string (" WHERE ownerclassid=") + getTableId () + string (";\n");
        }
        sqlForDrop += string ("DELETE FROM ") + schema + "." + pParent->getName() + string (" WHERE ownermanagedobjectidclassid=")+  getTableId () + string (";\n");
        UI32 numberOfDerivationsInstancesTables = m_derivationsInstancesTables.size ();
        UI32 i                                  = 0;
        for (i = 0; i < numberOfDerivationsInstancesTables; i++)
        {
            if (m_derivationsInstancesTables[i] == pParent->getName())
            {
                sqlForDrop += string ("DELETE FROM ") + schema + "." + m_derivationsInstancesTables[i] + string ("derivations WHERE objectidclassid=")+  getTableId () + string (";\n");
            }
        }
    }
    return  sqlForDrop;

}

string  OrmTable::getAlterSqlForOneToOneRelationship(const string &schema,const string & relationName) const
{
   string sqlForOneToOneRelationship = ""; 
   map<string,OrmRelation*>::const_iterator relationIterator = m_relationsByNames.find(relationName);

   if(relationIterator!=m_relationsByNames.end())
   {
       OrmRelation* pRelation = relationIterator->second;
       sqlForOneToOneRelationship += pRelation->getAlterSqlForOneToOneRelationship (schema,m_name);
   }

   return sqlForOneToOneRelationship;

}

string  OrmTable::getAlterSqlToDisableValidationForOneToManyRelationship (const string &schema, const string & relationName) const
{
   string name = getName ();
   string sqlForOneToManyRelationship = ""; 
   map<string,OrmRelation*>::const_iterator relationIterator = m_relationsByNames.find(relationName);

   if(relationIterator!=m_relationsByNames.end())
   {
       OrmRelation* pRelation = relationIterator->second;
       if (ORM_RELATION_UML_TYPE_COMPOSITION == pRelation->getRelationUmlType ())
       {
            // Dynamic cast the OrmRelation to a OrmComposition .. 
            // If not null then get the sql for disable/enable validation
            OrmComposition *pComposition = dynamic_cast<OrmComposition *> (pRelation);
            if (NULL != pComposition)
            {
                sqlForOneToManyRelationship += pComposition->getSqlForDisableValidation (name, schema);
            }
       }
   }

   return sqlForOneToManyRelationship;

}

string  OrmTable::getAlterSqlToEnableValidationForOneToManyRelationship (const string &schema, const string & relationName) const
{
   string name = getName ();
   string sqlForOneToManyRelationship = ""; 
   map<string,OrmRelation*>::const_iterator relationIterator = m_relationsByNames.find(relationName);

   if(relationIterator!=m_relationsByNames.end())
   {
       OrmRelation* pRelation = relationIterator->second;
       if (ORM_RELATION_UML_TYPE_COMPOSITION == pRelation->getRelationUmlType ())
       {
            // Dynamic cast the OrmRelation to a OrmComposition .. 
            // If not null then get the sql for disable/enable validation
            OrmComposition *pComposition = dynamic_cast<OrmComposition *> (pRelation);
            if (NULL != pComposition)
            {
                sqlForOneToManyRelationship += pComposition->getSqlForEnableValidation (name, schema);
            }
       }
   }

   return sqlForOneToManyRelationship;

}
string  OrmTable::getAlterSqlToAddNotNullForOneToOneRelationship(const string &schema,const string & relationName) const
{
   string sqlForOneToOneRelationship = ""; 
   map<string,OrmRelation*>::const_iterator relationIterator = m_relationsByNames.find(relationName);

   if(relationIterator!=m_relationsByNames.end())
   {
       OrmRelation* pRelation = relationIterator->second;
       sqlForOneToOneRelationship += pRelation->getAlterSqlToAddNotNullForOneToOneRelationship (schema,m_name);
   }

   return sqlForOneToOneRelationship;

}

string  OrmTable::getAlterSqlToDropNotNullForOneToOneRelationship(const string &schema,const string & relationName) const
{
   string sqlForOneToOneRelationship = ""; 
   map<string,OrmRelation*>::const_iterator relationIterator = m_relationsByNames.find(relationName);

   if(relationIterator!=m_relationsByNames.end())
   {
       OrmRelation* pRelation = relationIterator->second;
       sqlForOneToOneRelationship += pRelation->getAlterSqlToDropNotNullForOneToOneRelationship (schema,m_name);
   }

   return sqlForOneToOneRelationship;

}

void OrmTable::getOrmRelationTypeAndOrmRelationUmlType (const string &entityName, OrmRelationType &ormRelationType, OrmRelationUmlType &ormRelationUmlType)
{
    bool isAColumn   = isAKnownColumnInHierarchy   (entityName);
    
    isAColumn = isAKnownColumnInHierarchy   (entityName);

    if (false == isAColumn)
    {
        map<string, OrmRelation *>::const_iterator element    = m_relationsByNames.find (entityName);
        map<string, OrmRelation *>::const_iterator endElement = m_relationsByNames.end ();

        if (endElement == element)
        {
            if (NULL != m_pParentTable)
            {
                return (m_pParentTable->getOrmRelationTypeAndOrmRelationUmlType (entityName, ormRelationType, ormRelationUmlType));
            }
            else
            {
                WaveNs::prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            ormRelationType     = (element->second)->getRelationType ();
            ormRelationUmlType  = (element->second)->getRelationUmlType ();
        }
    }
    else
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);        
    }
}

bool OrmTable::getValueForDisableValidations (const string &fieldName)
{
    Attribute *pAttribute = getAttributeForFieldNameInManagedObject (fieldName);
    if (AttributeType::AttributeTypeCompositionVector == pAttribute->getAttributeType ())
    {
        AttributeManagedObjectVectorCompositionTemplateBase *pAttributeManagedObjectVectorCompositionTemplateBase = dynamic_cast<AttributeManagedObjectVectorCompositionTemplateBase *> (pAttribute);

        prismAssert (NULL != pAttributeManagedObjectVectorCompositionTemplateBase, __FILE__, __LINE__);

        return (pAttributeManagedObjectVectorCompositionTemplateBase->getDisableValidations ());
    }
    
    return (false);
}

bool OrmTable::getValueForCanBeEmpty (const string &fieldName)
{
    Attribute *pAttribute = getAttributeForFieldNameInManagedObject (fieldName);

    if (AttributeType::AttributeTypeComposition == pAttribute->getAttributeType ())
    {
        AttributeManagedObjectOneToOneRelationBase *pAttributeManagedObjectOneToOneRelationBase = dynamic_cast<AttributeManagedObjectOneToOneRelationBase *> (pAttribute);
        prismAssert (NULL != pAttributeManagedObjectOneToOneRelationBase, __FILE__, __LINE__);
        return (pAttributeManagedObjectOneToOneRelationBase->getCanBeEmpty ());
    }   
    else if (AttributeType::AttributeTypeObjectId == pAttribute->getAttributeType ())
    {
        AttributeObjectIdAssociation *pAttributeObjectIdAssociation = dynamic_cast<AttributeObjectIdAssociation *> (pAttribute);
        prismAssert (NULL != pAttributeObjectIdAssociation, __FILE__, __LINE__);
        return (pAttributeObjectIdAssociation->getCanBeEmpty ());
    }

    return (false);
}

void OrmTable::setEmptyNeededOnPersistentBootWithDefault (const bool &emptyNeededOnPersistentBootWithDefault)
{
    m_emptyNeededOnPersistentBootWithDefault = emptyNeededOnPersistentBootWithDefault;
}

bool OrmTable::getEmptyNeededOnPersistentBootWithDefault () const
{
    return (m_emptyNeededOnPersistentBootWithDefault);
}

const vector<string> OrmTable::getExtendedFieldTypesForUpgrade () 
{
    vector<string> extendedFieldTypes;
    const vector<string>& fields = m_fieldNamesForUpgrade;
    const vector<string>& types  = m_fieldTypesForUpgrade;
    const vector<string>& dbtypes = m_fieldDbTypesForUpgrade;

    for (size_t idx = 0; idx < fields.size (); ++idx)
    {
        // For each field, get the value of "disableValidations" and "canBeEmpty" by calling helper functions
        const string fieldName = fields[idx];
        const string fieldType = types[idx];
        bool  disableValidations = false;
        bool  canBeEmpty         = false;

        if ((fieldType[1] == '-' && fieldType[3] == '-'))
        {   
            if (fieldType[2] == 'm')
            {  
                disableValidations = getValueForDisableValidations (fieldName); 
            }
            
            if (fieldType[2] == '1')
            {
                canBeEmpty = getValueForCanBeEmpty (fieldName);
            }
        }

        string dataType = types[idx] + "|" + dbtypes[idx] + "|" + disableValidations + "|" + canBeEmpty;

        extendedFieldTypes.push_back (dataType);
        trace (TRACE_LEVEL_DEBUG, "OrmTable::getExtendedFieldTypesForUpgrade: field:" +fieldName+ ", dataType = " + dataType); 
    }
    return (extendedFieldTypes);
}

void OrmTable::getRelationNameToAuxilliaryTableNameMapForOneToNAssociations (const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap) const
{
    getRelationNameToAuxilliaryTableNameMapForOneToNRelationType (ORM_RELATION_UML_TYPE_ASSOCIATION, selectFieldsInManagedObject, relationNameToAuxilliaryTableNamesMap);
}

void OrmTable::getRelationNameToAuxilliaryTableNameMapForOneToNCompositions (const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap) const
{
    getRelationNameToAuxilliaryTableNameMapForOneToNRelationType (ORM_RELATION_UML_TYPE_COMPOSITION, selectFieldsInManagedObject, relationNameToAuxilliaryTableNamesMap);
}

void OrmTable::getRelationNameToAuxilliaryTableNameMapForOneToNRelationType (const OrmRelationUmlType &ormRelationUmlType, const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap) const
{
    UI32 numberOfSelectFields = selectFieldsInManagedObject.size ();

    if (0 == numberOfSelectFields)
    {
        map<string, OrmRelation *>::const_iterator itr        = m_relationsByNames.begin ();
        map<string, OrmRelation *>::const_iterator endElement = m_relationsByNames.end ();

        for (; endElement != itr; itr++)
        {
            OrmRelationType     ormRelationTypeForCurrentRelation       = (itr->second)->getRelationType ();
            OrmRelationUmlType  ormRelationUmlTypeForCurrentRelation    = (itr->second)->getRelationUmlType ();

            if (ORM_RELATION_TYPE_ONE_TO_MANY == ormRelationTypeForCurrentRelation)
            {
                if (ormRelationUmlType == ormRelationUmlTypeForCurrentRelation)
                {
                    string auxilliaryTableName = (itr->second)->getAuxilliaryTableName (m_name);
                    prismAssert (false == auxilliaryTableName.empty (), __FILE__, __LINE__);

                    relationNameToAuxilliaryTableNamesMap[(itr->second)->getName ()] = auxilliaryTableName;
                }
            }
        }
    }
    else
    {
        for (UI32 i = 0; i < numberOfSelectFields; i++)
        {
            map<string, OrmRelation *>::const_iterator itr        = m_relationsByNames.find (selectFieldsInManagedObject[i]);
            map<string, OrmRelation *>::const_iterator endElement = m_relationsByNames.end ();

            if (endElement == itr)
            {
                continue;
            }

            OrmRelationType     ormRelationTypeForCurrentRelation       = (itr->second)->getRelationType ();
            OrmRelationUmlType  ormRelationUmlTypeForCurrentRelation    = (itr->second)->getRelationUmlType ();

            if (ORM_RELATION_TYPE_ONE_TO_MANY == ormRelationTypeForCurrentRelation)
            {
                if (ormRelationUmlType == ormRelationUmlTypeForCurrentRelation)
                {
                    string auxilliaryTableName = (itr->second)->getAuxilliaryTableName (m_name);
                    prismAssert (false == auxilliaryTableName.empty (), __FILE__, __LINE__);

                    relationNameToAuxilliaryTableNamesMap[(itr->second)->getName ()] = auxilliaryTableName;
                }
            }
        }
    }

    if (NULL != m_pParentTable)
    {
        m_pParentTable->getRelationNameToAuxilliaryTableNameMapForOneToNRelationType (ormRelationUmlType, selectFieldsInManagedObject, relationNameToAuxilliaryTableNamesMap);
    }
}

void OrmTable::getAllTableNamesInHierarchy (set<string> &tableNames)
{
    if (NULL != m_pParentTable)
    {
        m_pParentTable->getAllTableNamesInHierarchy (tableNames);
    }

    tableNames.insert (m_name);
}

void OrmTable::getCompositionRelationTargetTableNamesFromSet (const set<string> &setOfTables, vector<string> &childTableNames)
{
    if (NULL != m_pParentTable)
    {
        m_pParentTable->getCompositionRelationTargetTableNamesFromSet (setOfTables, childTableNames);
    }

    map<string, OrmRelation *>::const_iterator itr        = m_relationsByNames.begin ();
    map<string, OrmRelation *>::const_iterator endElement = m_relationsByNames.end ();

    for (; endElement != itr; itr++)
    {
        OrmRelation        *pOrmRelation                            = itr->second;
        OrmRelationUmlType  ormRelationUmlTypeForCurrentRelation    = pOrmRelation->getRelationUmlType ();

        if (ORM_RELATION_UML_TYPE_COMPOSITION == ormRelationUmlTypeForCurrentRelation)
        {
            string                      relatedTableName    = pOrmRelation->getRelatedTo ();

            set<string>::const_iterator itr2                = setOfTables.find (relatedTableName);
            set<string>::const_iterator end2                = setOfTables.end (); // Can be moved outside the loop.

            if (end2 != itr2)
            {
                childTableNames.push_back (relatedTableName);
            }
        }
    }
}

bool OrmTable::isATableNameInHierarchy (const string &tableName) const
{
    if (m_name == tableName)
    {
        return true;
    }

    bool isInHierarchy = false;

    if (NULL != m_pParentTable)
    {
        isInHierarchy = m_pParentTable->isATableNameInHierarchy (tableName);
    }

    return isInHierarchy;
}

void OrmTable::getUserDefinedKeyCombination (vector<string> &userDefinedKeyCombination) const
{
    userDefinedKeyCombination = m_userDefinedKeyCombination;
}

void OrmTable::computeKeyStringAndUDKCValueExpression (string& keyStringExpression, string& userDefinedKeyCombinationExpression)
{
    UI32 numberOfKeys = m_userDefinedKeyCombination.size (); 

    // Compute keyString and userDefinedKeyCombinationValue expressions which will update all the rows in a table.
    userDefinedKeyCombinationExpression = "";
    keyStringExpression                 = string("\'") + m_name + string("\'"); 

    for (UI32 keyNumber = 0; keyNumber < numberOfKeys; keyNumber++)
    {
        string      userDefinedKeyField = m_userDefinedKeyCombination [keyNumber];
        Attribute   *pAttribute         = (m_userDefinedKeyCombinationWithTypes.find (userDefinedKeyField))->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        keyStringExpression += "||'*'||";

        if (0 != keyNumber)
        {
            userDefinedKeyCombinationExpression += "||' '||";
        }

        if (AttributeType::AttributeTypeObjectId == pAttribute->getAttributeType ())
        {
            userDefinedKeyCombinationExpression += userDefinedKeyField + "ClassId || '*' ||" + userDefinedKeyField + "InstanceId";
            keyStringExpression                 += userDefinedKeyField + "ClassId || '*' ||" + userDefinedKeyField + "InstanceId";
        }
        else
        {
            userDefinedKeyCombinationExpression += userDefinedKeyField;
            keyStringExpression                 += userDefinedKeyField;
        }
    }
}

void OrmTable::getSqlForATableToUpdateUDKCRelatedColumns (vector<string>& compositionChildrenForATable, string& sqlForCurrentSchema, string& sqlForStartupSchema)
{
    string                          userDefinedKeyCombinationExpression;
    string                          keyStringExpression;
    string                          ownerKeyStringExpression;

    // 1. for a table -
    trace (TRACE_LEVEL_INFO, "OrmTable::getSqlForATableToUpdateUDKCRelatedColumns: table [" + m_name + "]");

    computeKeyStringAndUDKCValueExpression (keyStringExpression, userDefinedKeyCombinationExpression);

    sqlForCurrentSchema += string ("UPDATE ONLY ")+OrmRepository::getWaveCurrentSchema() + string(".") + m_name + string (" SET keystring = ") + keyStringExpression 
                        + string (" WHERE ownermanagedobjectidclassid = 0;\n");
    sqlForStartupSchema += string ("UPDATE ONLY ")+OrmRepository::getWaveStartSchema() + string(".") + m_name + string (" SET keystring = ") + keyStringExpression 
                        + string (" WHERE ownermanagedobjectidclassid = 0;\n");

    if (0 != m_userDefinedKeyCombination.size ())
    {
        sqlForCurrentSchema += string ("UPDATE ONLY ")+OrmRepository::getWaveCurrentSchema() + string(".") + m_name + string (" SET userdefinedkeycombinationvalue = ") 
                            + userDefinedKeyCombinationExpression + string (" WHERE ownermanagedobjectidclassid = 0;\n");
        sqlForStartupSchema += string ("UPDATE ONLY ")+OrmRepository::getWaveStartSchema() + string(".") + m_name + string (" SET userdefinedkeycombinationvalue = ") 
                            + userDefinedKeyCombinationExpression + string (" WHERE ownermanagedobjectidclassid = 0;\n");
    }
   
    // 2. for composed children - 
    for (UI32 i = 0; i < compositionChildrenForATable.size (); i++)
    {
        string composedChildTableName   = compositionChildrenForATable[i];

        trace (TRACE_LEVEL_INFO, "OrmTable::getSqlForATableToUpdateUDKCRelatedColumns: composed child table [" + composedChildTableName + "]"); 

        userDefinedKeyCombinationExpression = "";
        keyStringExpression                 = "";
    
        OrmTable* pChildTable = OrmRepository::getTableByName (composedChildTableName);

        prismAssert (NULL != pChildTable, __FILE__, __LINE__);
        
        pChildTable->computeKeyStringAndUDKCValueExpression (keyStringExpression, userDefinedKeyCombinationExpression);

        //1. update ownerKeyString first

        sqlForCurrentSchema += string ("UPDATE ONLY ") + OrmRepository::getWaveCurrentSchema() + string(".") + composedChildTableName 
                            + string (" AS o SET ownerkeystring=(select keystring from ") + OrmRepository::getWaveCurrentSchema() + "." + m_name + " where objectidclassid = " 
                            + getTableId () + " AND objectidinstanceid = o.ownermanagedobjectidinstanceid) where o.ownermanagedobjectidclassid = " + getTableId () + ";\n";

        sqlForStartupSchema += string ("UPDATE ONLY ") + OrmRepository::getWaveStartSchema() + string(".") + composedChildTableName 
                            + string (" AS o SET ownerkeystring=(select keystring from ") + OrmRepository::getWaveStartSchema() + "." + m_name + " where objectidclassid = " 
                            + getTableId () + " AND objectidinstanceid = o.ownermanagedobjectidinstanceid) where o.ownermanagedobjectidclassid = " + getTableId () + ";\n";

        //2. Now update keyString and userDefinedKeyCombinationValue columns.

        sqlForCurrentSchema += string ("UPDATE ONLY ") + OrmRepository::getWaveCurrentSchema() + string(".") + composedChildTableName 
                            + string (" SET keystring = ownerkeystring || '.' || ") + keyStringExpression + string (" where ownermanagedobjectidclassid = ") + getTableId () + ";\n";
        sqlForStartupSchema += string ("UPDATE ONLY ") + OrmRepository::getWaveStartSchema() + string(".") + composedChildTableName 
                            + string (" SET keystring = ownerkeystring || '.' || ") + keyStringExpression + string (" where ownermanagedobjectidclassid = ") + getTableId () + ";\n";

        
        vector<string> keys;
        pChildTable->getUserDefinedKeyCombination (keys);

        if (0 != keys.size())
        {
            sqlForCurrentSchema += string ("UPDATE ONLY ") + OrmRepository::getWaveCurrentSchema() + string(".") + composedChildTableName + string (" SET userdefinedkeycombinationvalue = ") 
                                + userDefinedKeyCombinationExpression + string (" where ownermanagedobjectidclassid = ") + getTableId () + string (";\n");
            sqlForStartupSchema += string ("UPDATE ONLY ") + OrmRepository::getWaveStartSchema() + string(".") + composedChildTableName + string (" SET userdefinedkeycombinationvalue = ") 
                                + userDefinedKeyCombinationExpression + string (" where ownermanagedobjectidclassid = ") + getTableId () + string (";\n"); 
        }
    }
                    
}

void OrmTable::getSqlToDropUserDefinedKeyUniqueConstraint (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    if (false == m_userDefinedKeyCombination.empty ())
    {
        schemaUpdateSqlForCurrentSchema += string ("ALTER TABLE ") + OrmRepository::getWaveCurrentSchema() + string (".") + m_name + string (" DROP CONSTRAINT IF EXISTS ") 
                                        + m_name + string("_unique;\n"); 
        schemaUpdateSqlForStartSchema   += string ("ALTER TABLE ") + OrmRepository::getWaveStartSchema() + string (".") + m_name + string (" DROP CONSTRAINT IF EXISTS ") 
                                        + m_name + string("_unique;\n"); 
    }

}

void OrmTable::getSqlToAddUserDefinedKeyUniqueConstraint (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    UI32                    numberOfUserDefinedKeyCombination = m_userDefinedKeyCombination.size ();
    string                  sqlForUniqueConstraint;
    bool                    isTableNameFoundInAllCompostionTableSet = false;
    set<string>             allCompostionTableNames;
    set<string>::iterator   it;

    OrmRepository::getAllCompositionTableName (allCompostionTableNames);

    it = allCompostionTableNames.find (m_name);

    if (it != allCompostionTableNames.end ())
    {
        isTableNameFoundInAllCompostionTableSet = true;
    }

    if ((0 < numberOfUserDefinedKeyCombination) && (true != isTableNameFoundInAllCompostionTableSet))
    {
        //OrmTable        *pTable     = OrmRepository::getTableByName (m_name);
        //vector<string>  udkcInOrder = pTable->getUserDefinedFieldNamesForUpgrade ();
        vector<string>          udkcInOrder; // = getUserDefinedFieldNamesForUpgrade ();
        map<string, Attribute*> udkcKeyWithTypes;

        udkcKeyWithTypes = getUserDefinedKeyCombinationWithTypes (udkcInOrder);

        UI32            keyNumber   = 0; 

        for (;keyNumber < udkcInOrder.size (); keyNumber++)
        {
            if (keyNumber != 0)
            {
                sqlForUniqueConstraint  += ", ";
            }

            string      userDefinedKeyField = udkcInOrder [keyNumber];
            //map<string, string>::const_iterator iter = m_userDefinedKeyCombinationsMap.find(userDefinedKeyField);

            //prismAssert (m_userDefinedKeyCombinationsMap.end () != iter, __FILE__, __LINE__);

            //string userDefinedKeyType = iter->second;

            Attribute*  udkcFieldAttribute  = udkcKeyWithTypes[userDefinedKeyField];

            prismAssert (NULL != udkcFieldAttribute, __FILE__, __LINE__);

            //if (userDefinedKeyType == (FrameworkToolKit::localizeToSourceCodeEnum ((AttributeType::AttributeTypeObjectId).getAttributeTypeResourceId())))
            if (AttributeType::AttributeTypeObjectId == udkcFieldAttribute->getAttributeType())
            {
                sqlForUniqueConstraint += userDefinedKeyField + "ClassId, " + userDefinedKeyField + "InstanceId";
            }
            else
            {
                sqlForUniqueConstraint += userDefinedKeyField;
            }
        }
        
        // Add ownerpartitionManagedObjectId to unique key constraints.
        
        string ownerPartitionMOObjectIdFieldName = "ownerPartitionManagedObjectId";
        
        sqlForUniqueConstraint +=  ", " + ownerPartitionMOObjectIdFieldName + "ClassId, " + ownerPartitionMOObjectIdFieldName + "InstanceId";
        
        if (true == m_isALocalManagedObject)
        {   
            string ownerWaveNodeObjectIdFieldName = "ownerWaveNodeObjectId";
            
            sqlForUniqueConstraint +=  ", " + ownerWaveNodeObjectIdFieldName + "ClassId, " + ownerWaveNodeObjectIdFieldName + "InstanceId";
        }
        
        schemaUpdateSqlForCurrentSchema += string("ALTER TABLE ") + OrmRepository::getWaveCurrentSchema() + string(".") + m_name + string (" ADD CONSTRAINT ") + m_name + string("_unique UNIQUE (")+ sqlForUniqueConstraint + ");\n";
        schemaUpdateSqlForStartSchema   += string("ALTER TABLE ") + OrmRepository::getWaveStartSchema() + string(".") + m_name + string (" ADD CONSTRAINT ") + m_name + string("_unique UNIQUE (")+ sqlForUniqueConstraint + ");\n";
    }
}

string OrmTable::getSqlTypeForAField (const string& fieldName)
{
    UI32 numOfEntities = m_ormEntities.size ();

    for (UI32 i = 0; i < numOfEntities; i++)
    {
        OrmEntity* entity = m_ormEntities[i];

        if (0 == fieldName.compare (entity->getName ()))
        {
            OrmColumn *column = dynamic_cast<OrmColumn *> (entity);
            if (NULL != column)
            {   
                return (column->getSqlType ());
            }
            else
            {
                trace (TRACE_LEVEL_WARN, "OrmTable::getSqlTypeForAField: [" + fieldName + "] is not a column but a relation in a table.");
                return ("");
            }
        }
    }

    trace (TRACE_LEVEL_WARN, "OrmTable::getSqlTypeForAField: could not find column [" + fieldName + "] in a table");

    return ("");
}
        
            
}

