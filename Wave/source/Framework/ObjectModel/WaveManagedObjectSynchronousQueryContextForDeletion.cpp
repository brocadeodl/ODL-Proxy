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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

WaveManagedObjectSynchronousQueryContextForDeletion::WaveManagedObjectSynchronousQueryContextForDeletion (const string &classToQueryFor)
    : WaveManagedObjectSynchronousQueryContext (classToQueryFor)
{
}

WaveManagedObjectSynchronousQueryContextForDeletion::~WaveManagedObjectSynchronousQueryContextForDeletion ()
{
}

UI32 WaveManagedObjectSynchronousQueryContextForDeletion::getPageSizeForQueryResults ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForDeletion::getPageSizeForQueryResults : This method should never be invoked for this deletion context.");   
    prismAssert (false, __FILE__, __LINE__);

    return (0);
}

void WaveManagedObjectSynchronousQueryContextForDeletion::setPageSizeForQueryResults (UI32 limitForQueryResults)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForDeletion::setPageSizeForQueryResults : This method should never be invoked for this deletion context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForDeletion::setPageQueryContext (WaveObjectManager* callingOm)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForDeletion::setPageQueryContext : This method should never be invoked for this deletion context.");   
    prismAssert (false, __FILE__, __LINE__);
}

bool WaveManagedObjectSynchronousQueryContextForDeletion::isPageQueryContextSet ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForDeletion::isPageQueryContextSet : This method should never be invoked for this deletion context.");   
    prismAssert (false, __FILE__, __LINE__);

    return (false);
}

string WaveManagedObjectSynchronousQueryContextForDeletion::getPageQuerySql ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForDeletion::getPageQuerySql : This method should never be invoked for this deletion context.");   
    prismAssert (false, __FILE__, __LINE__);

    return ("");
}

void WaveManagedObjectSynchronousQueryContextForDeletion::moveOffsetOnSuccessfulQuery ()
{
    // Do nothing here. since this is called as part of the querySynchronously (QueryContext) 
}

string WaveManagedObjectSynchronousQueryContextForDeletion::getSqlWithPagingEnabled (WaveObjectManager* callingOm)
{
    // We only want to return the non-paged SQL here.  This is also called as part of the querySynchronously (QueryContext).

    return (getSql ());
}

void WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForDelete (string &sql)
{
    UI32     i = 0;
    OrmTable *pTable = OrmRepository::getTableByName (getClassToQueryFor ());
    prismAssert (NULL != pTable, __FILE__, __LINE__);

    vector<OrmTable *> derivedTables;
    pTable->getAllDerivedTableIds (derivedTables);
    derivedTables.push_back (pTable);

    string          whereClause;
    updateAssociationTableAndWhereClause (whereClause);

    for (i = 0; i < derivedTables.size() ; i++)
    {
        prismAssert (NULL != derivedTables[i], __FILE__, __LINE__);
        
        string          instanceIdSql;
        instanceIdSql = string (" (SELECT ObjectIdInstanceId FROM ") + OrmRepository::getWaveCurrentSchema ()+ "." + derivedTables[i]->getName () + whereClause + ")";

        getSqlForDeleteFromDerivationAndAuxilliaryTables (sql, instanceIdSql, derivedTables[i]);

    }
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForDelete: Bulk delete sql statement:" + sql);

}

void WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForDeleteFromDerivationAndAuxilliaryTables (string &sql, string instanceIdSql, OrmTable *pTable)
{
    vector<string>  auxilliaryTableNames;
    UI32            numberOfAuxilliaryTables = 0;
    UI32            numberOfRelations = 0;
    UI32            i                        = 0;
    string          sqlForDelete;
    string          tableIdString            = string ("") + pTable->getTableId ();

    vector<OrmTable *>         relatedFromTables;
    vector<string>             relatedToTables;
    vector<OrmRelationType>    relationTypes;
    vector<OrmRelationUmlType> relationUmlTypes;
    vector<string>             relationNames;
    OrmTable *pComposedTable;

    pTable->getDetailsForRelationshipsInHierarchy (relatedFromTables, relatedToTables, relationTypes, relationUmlTypes, relationNames);
    numberOfRelations = relatedToTables.size ();

    for (i = 0; i < numberOfRelations; i++)
    {
        if (ORM_RELATION_UML_TYPE_COMPOSITION == relationUmlTypes[i])
        {
            string instanceIdSql2;
            if (ORM_RELATION_TYPE_ONE_TO_ONE == relationTypes[i])
            {
                // add Composed field information in instanceIdSql.
                instanceIdSql2 = string (" (SELECT " + relationNames[i] + "InstanceId" + " FROM ") + OrmRepository::getWaveCurrentSchema () + "." + pTable->getName () + " WHERE ObjectIdInstanceId IN " + instanceIdSql + ")";
            }
            else
            {
                // add Auxiliary table information in instanceIdSql.
                UI32   parentTableId        = OrmRepository::getTableId (relatedFromTables[i]->getName ());
                UI32   relatedToTableId     = OrmRepository::getTableId (relatedToTables[i]);
                string auxilliaryTableName  = string ("t") + parentTableId + relationNames[i] + string ("t") + relatedToTableId;
                instanceIdSql2 = string (" (SELECT relatedToInstanceId FROM ") + OrmRepository::getWaveCurrentSchema () + "." + auxilliaryTableName + " WHERE ownerClassId = " + tableIdString + " AND ownerInstanceId IN " + instanceIdSql + ")";
            }

            pComposedTable = OrmRepository::getTableByName (relatedToTables[i]);

            /*
                The 'To-end-of-a-relation' can be the composed Table or any of its inherited children.
                So Try deleting from all child tables of composed table.
            */
            vector<OrmTable *> composedTableChildren;
            pComposedTable->getAllDerivedTableIds (composedTableChildren); 
            UI32 composedChildrenCount = composedTableChildren.size ();
            for (UI32 j = 0; j < composedChildrenCount; j++)
            {
                getSqlForDeleteFromDerivationAndAuxilliaryTables (sql, instanceIdSql2, composedTableChildren[j]);
            }

            getSqlForDeleteFromDerivationAndAuxilliaryTables (sql, instanceIdSql2, pComposedTable);
        }
    }

    pTable->getAllAuxilliaryTableNames (auxilliaryTableNames);
    numberOfAuxilliaryTables = auxilliaryTableNames.size ();

    for (i = 0; i < numberOfAuxilliaryTables; i++)
    {
        sqlForDelete += "DELETE FROM " + OrmRepository::getWaveCurrentSchema ()+ "." + auxilliaryTableNames[i] + " WHERE ownerClassId = " + tableIdString + " AND ownerInstanceId IN ";
        sqlForDelete += instanceIdSql + string (";\n");
    }

    sqlForDelete += pTable->getSqlToSelectivelyDeleteDerivationsInstances (OrmRepository::getWaveCurrentSchema (), instanceIdSql);

    sqlForDelete += "DELETE FROM " + OrmRepository::getWaveCurrentSchema ()+ "." + pTable->getName () + " WHERE ObjectIdClassId = " + tableIdString + " AND ObjectIdInstanceId IN ";
    sqlForDelete += instanceIdSql + string (";\n");

    sql += sqlForDelete;
}

void WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForDeletingCompositions (string &sql,
        set<string> compositions)
{
    UI32     i = 0;
    OrmTable *pTable = OrmRepository::getTableByName (getClassToQueryFor ());
    prismAssert (NULL != pTable, __FILE__, __LINE__);

    vector<OrmTable *> derivedTables;
    pTable->getAllDerivedTableIds (derivedTables);
    derivedTables.push_back (pTable);

    for (i = 0; i < derivedTables.size() ; i++)
    {
        prismAssert (NULL != derivedTables[i], __FILE__, __LINE__);
        
        getSqlForDeletingCompositionsPerTable (sql, compositions, derivedTables[i]);

    }
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForDeletingCompositions: Bulk delete sql statement:" + sql);
}

void WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForDeletingCompositionsPerTable (string &sql,
        set<string> compositions, OrmTable *pTable)
{
    string                      whereClause;
    string                      instanceIdSql;
    UI32                        numberOfRelations = 0;
    string                      sqlForDelete;

    vector<OrmTable *>          relatedFromTables;
    vector<string>              relatedToTables;
    vector<OrmRelationType>     relationTypes;
    vector<OrmRelationUmlType>  relationUmlTypes;
    vector<string>              relationNames;
    OrmTable                   *pComposedTable;
    vector<string>  oneToOneCompositions;

    updateAssociationTableAndWhereClause (whereClause);
    instanceIdSql = string (" (SELECT ObjectIdInstanceId FROM ") + OrmRepository::getWaveCurrentSchema ()+ "." + pTable->getName () + whereClause + ")";

    pTable->getDetailsForRelationshipsInHierarchy (relatedFromTables, relatedToTables, relationTypes, relationUmlTypes, relationNames);
    numberOfRelations = relatedToTables.size ();

    for (UI32 i = 0; i < numberOfRelations; i++)
    {
        // We should check the "compositions" set to identify the compositions to be delete.
        // If "compositions" set is empty, then proceed with deleting all compositions.
        if (! compositions.empty () && compositions.find (relationNames[i]) == compositions.end ()) {
            continue;
        }

        if (ORM_RELATION_UML_TYPE_COMPOSITION == relationUmlTypes[i])
        {
            string instanceIdSql2  = "";
            string sqlForDeletingMyAuxTableEntries = "";

            if (ORM_RELATION_TYPE_ONE_TO_ONE == relationTypes[i])
            {
                // SQL for obtaining the set of values for the composition-field, for the MO's matching the passed
                // query context.
                instanceIdSql2 = string (" (SELECT " + relationNames[i] + "InstanceId" + " FROM ") + OrmRepository::getWaveCurrentSchema () + "." + pTable->getName () + whereClause + ")";

                oneToOneCompositions.push_back(relationNames[i]);
            }
            else
            {
                // add Auxiliary table information in instanceIdSql.
                UI32   parentTableId        = OrmRepository::getTableId (relatedFromTables[i]->getName ());
                UI32   relatedToTableId     = OrmRepository::getTableId (relatedToTables[i]);
                string auxilliaryTableName  = string ("t") + parentTableId + relationNames[i] + string ("t") + relatedToTableId;
                instanceIdSql2   = string (" (SELECT relatedToInstanceId FROM ") + OrmRepository::getWaveCurrentSchema () + "." + auxilliaryTableName + " WHERE ownerClassId = " + pTable->getTableId () + " AND ownerInstanceId IN " + instanceIdSql + ")";

                // Generate SQL to delete the auxiliary table entries for the above composition.
                sqlForDeletingMyAuxTableEntries += " DELETE FROM " + OrmRepository::getWaveCurrentSchema ()+ "." + auxilliaryTableName
                        + " WHERE ownerClassId = " + pTable->getTableId () + " AND ownerInstanceId IN "
                        + instanceIdSql + string (";\n");
            }

            pComposedTable = OrmRepository::getTableByName (relatedToTables[i]);


            // The 'To-end-of-a-relation' can be the composed Table or any of its inherited children.
            // So Try deleting from all child tables of composed table.
            vector<OrmTable *> composedTableChildren;
            pComposedTable->getAllDerivedTableIds (composedTableChildren);
            UI32 composedChildrenCount = composedTableChildren.size ();
            for (UI32 j = 0; j < composedChildrenCount; j++)
            {
                getSqlForDeleteFromDerivationAndAuxilliaryTables (sql, instanceIdSql2, composedTableChildren[j]);
            }

            getSqlForDeleteFromDerivationAndAuxilliaryTables (sql, instanceIdSql2, pComposedTable);

            sql += sqlForDeletingMyAuxTableEntries;
        }
    }

    // Update lastModifiedTimeStamp to current time for the MO's, whose composition is deleted.
    // Also, for one-to-one compositions, update the composition field with NULL if the attribute can be empty.
    // Note, this API must not called with a composition which cannot be empty.

    string updateAttributesClause = " lastModifiedTimeStamp = '" + (DateTime ()).toString () + "'";

    UI32 oneToOneCompositionsNumber  = oneToOneCompositions.size();
    for(UI32 i = 0; i < oneToOneCompositionsNumber; i++) {
        updateAttributesClause += ", " + oneToOneCompositions[i] + "InstanceId = NULL";
        updateAttributesClause += ", " + oneToOneCompositions[i] + "ClassId = NULL ";
    }

    sqlForDelete += " UPDATE " + OrmRepository::getWaveCurrentSchema () + "." + pTable->getName ()
            + " SET " + updateAttributesClause + whereClause + ";\n";

    sql += sqlForDelete;

    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForDelete: Bulk delete sql statement:" + sql);

}

void WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForPartialDeleteFromVectorCompositions (string &sql, string &parentClassName, string &compositionName)
{
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForPartialDeleteFromVectorCompositions: Entering ...:");

    string   childClassName = getClassToQueryFor ();
    OrmTable *pChildTable = OrmRepository::getTableByName (getClassToQueryFor ());
    prismAssert (NULL != pChildTable, __FILE__, __LINE__);

    string          whereClause;
    string          instanceIdSql;

    updateAssociationTableAndWhereClause (whereClause);
    instanceIdSql = string (" (SELECT ObjectIdInstanceId FROM ") + OrmRepository::getWaveCurrentSchema ()+ "." + childClassName + whereClause + ")";

    UI32   parentTableId        = OrmRepository::getTableId (parentClassName);
    UI32   relatedToTableId     = OrmRepository::getTableId (childClassName);
    string parentTableIdString  = string ("") + parentTableId;
    string childTableIdString   = string ("") + relatedToTableId;
    string auxilliaryTableName  = string ("t") + parentTableId + compositionName + string ("t") + relatedToTableId;

    string updateAttributesClause = " lastModifiedTimeStamp = '" + (DateTime ()).toString () + "'";
    string updateOnParentMos = " WHERE objectIdClassId = " + parentTableIdString + " AND objectIdInstanceId IN (SELECT DISTINCT ownerInstanceId FROM " + OrmRepository::getWaveCurrentSchema ()+ "." + auxilliaryTableName + " WHERE relatedToClassId = " + childTableIdString + " AND relatedToInstanceId IN ";
    updateOnParentMos += instanceIdSql + string (")");

    sql += " UPDATE " + OrmRepository::getWaveCurrentSchema () + "." + parentClassName
            + " SET " + updateAttributesClause + updateOnParentMos + ";\n";

    sql += "DELETE FROM " + OrmRepository::getWaveCurrentSchema ()+ "." + auxilliaryTableName + " WHERE relatedToClassId = " + childTableIdString + " AND relatedToInstanceId IN ";
    sql += instanceIdSql + string (";\n");

    getSqlForDeleteFromDerivationAndAuxilliaryTables (sql, instanceIdSql, pChildTable);

    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForDeletion::getSqlForPartialDeleteFromVectorCompositions: Bulk delete sql statement:" + sql);
}

}
