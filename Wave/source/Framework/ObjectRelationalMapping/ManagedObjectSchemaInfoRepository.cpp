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

#include "Framework/Types/Types.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/ObjectRelationalMapping/RelationshipInfoFromSchemaDifference.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Database/DatabaseObjectManagerDropAllDBViewsMessage.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace WaveNs
{

/**
 * Name:  ManagedObjectSchemaInfoRepository
 * Description: Constructor
 * @param None
 */
ManagedObjectSchemaInfoRepository::ManagedObjectSchemaInfoRepository ()
    : m_userDefinedKeyCombinationChanged (false)
{
}

/**
 * Name: ~ManagedObjectSchemaInfoRepository
 * Description: Destructor
 */
ManagedObjectSchemaInfoRepository::~ManagedObjectSchemaInfoRepository ()
{
    clear ();
}

void ManagedObjectSchemaInfoRepository::clear ()
{
    for (map<UI32, ManagedObjectSchemaInfo *>::iterator iter = m_schemaInfoByIdMap.begin ();
          iter != m_schemaInfoByIdMap.end (); ++iter)
    {
        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "ManagedObjectSchemaInfoRepository::clear: cleaning: [%s] : tableId: %u", ((iter->second)->getName ()).c_str(), iter->first);
        delete (iter->second);
    }

    m_schemaInfoByIdMap.clear ();
    m_schemaInfoByNameMap.clear ();
    m_tempInfoObjectStore.clear ();
    m_schemaInfoObjects.clear ();

    cleanupSchemaDifferenceInfo ();
}

UI32 ManagedObjectSchemaInfoRepository::getNumberOfInfoObjectsInRepository () const
{
    return (m_schemaInfoByIdMap.size ());
}

map<string, ModifiedManagedObjectSchemaDifference> ManagedObjectSchemaInfoRepository::getModifiedTablesSchemaDifference ()
{
    return m_modifiedTablesSchemaDifference;
}

void ManagedObjectSchemaInfoRepository::addSchemaInfoObject (ManagedObjectSchemaInfo *schemaInfo)
{
    UI32 parentTableId = schemaInfo->getParentTableId ();

    if (parentTableId == 0)
    {
        m_schemaInfoObjects.push_back (schemaInfo);
    }
    else
    {
        map<UI32, ManagedObjectSchemaInfo *>::iterator iter = m_schemaInfoByIdMap.find (parentTableId);
        if (iter != m_schemaInfoByIdMap.end ())
        {
            ManagedObjectSchemaInfo *parentTable = iter->second;
            parentTable->addSchemaInfoObject (schemaInfo);
        }
        else
        {
            // tempInfo object map is indexed on _parent_ tableId
            m_tempInfoObjectStore[parentTableId].push_back (schemaInfo);
        }
    } //end_if

    // Check if any objects in m_tempInfoObjectStore have this as
    // it's parent. If so, move it
    map<UI32, vector<ManagedObjectSchemaInfo *> >::iterator iter = m_tempInfoObjectStore.find (schemaInfo->getTableId ());
    if (iter != m_tempInfoObjectStore.end ()) {
        vector <ManagedObjectSchemaInfo *> schemaInfoVec = (*iter).second;
    for (vector <ManagedObjectSchemaInfo *>::iterator iter2 = schemaInfoVec.begin ();
         iter2 != schemaInfoVec.end (); ++iter2)
    {
        schemaInfo->addSchemaInfoObject (*iter2);
    }
    schemaInfoVec.clear ();
        m_tempInfoObjectStore.erase (iter);
    }

    m_schemaInfoByNameMap[schemaInfo->getName ()] = schemaInfo;
    m_schemaInfoByIdMap[schemaInfo->getTableId ()] = schemaInfo;
}

ResourceId ManagedObjectSchemaInfoRepository::removeManagedObjectSql (ManagedObjectSchemaInfo *pRemovedManagedObject, string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::removeManagedObjectSql  : entering...");
   
    // 1. Get instanceIds for the table and call the cleanup function.
    vector<UI64> objectInstanceIds;
    DatabaseStandaloneTransaction dbStandaloneTransaction;
    
    bool success = dbStandaloneTransaction.getObjectInstanceIdsForTable (pRemovedManagedObject->getName (), objectInstanceIds);
    if (false == success)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::removeManagedObjectSql : failed to get the derivationsInstanceIds for table " + pRemovedManagedObject->getName ());
        return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
    }

    // 2 
    schemaUpdateSqlForCurrentSchema += "DROP TABLE " + OrmRepository::getWaveCurrentSchema () + "." + pRemovedManagedObject->getName () + ";" + " \n";
    schemaUpdateSqlForStartSchema   += "DROP TABLE " + OrmRepository::getWaveStartSchema ()   + "." + pRemovedManagedObject->getName () + ";" + " \n";

    // 3 
    ResourceId status = getSqlToRemoveInstanceIdsFromDerivationInstancesTables (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository, pRemovedManagedObject->getName (), pRemovedManagedObject->getTableId(), objectInstanceIds);
    if (FRAMEWORK_SUCCESS != status)
    {
        // Add a trace here
        return (status);
    }
    
    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::removeSchemaInfoObjectAndGetSql (ManagedObjectSchemaInfo *schemaInfo, string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository, bool isInternalCall)
{
    trace (TRACE_LEVEL_DEBUG, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject : entering...");

    // 1. calculate number of children

    UI32    numberOfChildren                     = 0;
    vector<ManagedObjectSchemaInfo *> *pChildren = schemaInfo->getChildSchemaInfoObjects ();
    if (pChildren != NULL)
    {
        numberOfChildren = pChildren->size ();
    }

    string  myMOName = schemaInfo->getName ();
    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject : %s : number of children = %u", myMOName.c_str(), numberOfChildren);
    for (vector<ManagedObjectSchemaInfo *>::iterator iter1 = (*pChildren).begin (); iter1 != (*pChildren).end (); ++iter1)
    {
        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "childTableId : %u", (*iter1)->getTableId ());
    }

    UI32 myTableId = schemaInfo->getTableId ();
    if (0 == numberOfChildren)
    {
        // 1. Get Sql for removing managed object, derivation and derivation instances
        ResourceId status = removeManagedObjectSql (schemaInfo, schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository);
        if (FRAMEWORK_SUCCESS != status)
        {
            // Add a trace here 
            return (status);
        }
        
        // 2. remove schemaInfo from the vector and maps maintaining it.
        UI32    myParentTableId = schemaInfo->getParentTableId ();
        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject : %s ::myParentTableId : %u", myMOName.c_str(), myParentTableId);

        if (myParentTableId)
        {
            map<UI32, ManagedObjectSchemaInfo *>::iterator iter2 = m_schemaInfoByIdMap.find (myParentTableId);
            if (iter2 != m_schemaInfoByIdMap.end ())
            {
                ManagedObjectSchemaInfo *myParentMOSchemaObject = iter2->second;
                tracePrintf (TRACE_LEVEL_DEBUG, true, false, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject :%s :: parentName : %s", myMOName.c_str(), myParentMOSchemaObject->getName ().c_str());
                myParentMOSchemaObject->removeInfoObject (schemaInfo);
            }
        }
        //else
        //{
            for (vector <ManagedObjectSchemaInfo *>::iterator iter1 = m_schemaInfoObjects.begin (); iter1 != m_schemaInfoObjects.end(); ++iter1)
            {
                if ((*iter1)->getTableId () == myTableId)
                {
                    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject :cleaning m_schemaInfoObjects for %s", myMOName.c_str());
                    m_schemaInfoObjects.erase (iter1);
                    break;
                }
            }
        //}
        tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject :cleaning for [%s], tableId = %u, name = %s", myMOName.c_str(), schemaInfo->getTableId (), (schemaInfo->getName ()).c_str());
        
        UI32 entriesErased = m_schemaInfoByNameMap.erase (schemaInfo->getName ());
        tracePrintf (TRACE_LEVEL_INFO, true, false, "removeSchemaInfoObject: m_schemaInfoByNameMap.erase returned %u", entriesErased);
        entriesErased = m_schemaInfoByIdMap.erase (schemaInfo->getTableId ());    
        tracePrintf (TRACE_LEVEL_INFO, true, false, "removeSchemaInfoObject: m_schemaInfoByIdMap.erase returned %u", entriesErased);

        // Remove myself from the temp store in case schemaInfo Id is present.
        for (vector <UI32>::iterator iter2 = m_tempInfoObjectIdStoreForRemove.begin (); iter2 != m_tempInfoObjectIdStoreForRemove.end (); ++iter2)
        {
            if (myTableId == *iter2)
            {   
                m_tempInfoObjectIdStoreForRemove.erase (iter2);
                break;
            }
        }

        // 4. Check if remove for Parent of a schemaObject was called before. 
        for (vector <UI32>::iterator iter2 = m_tempInfoObjectIdStoreForRemove.begin (); iter2 != m_tempInfoObjectIdStoreForRemove.end (); iter2++)
        {
            if (myParentTableId == *iter2)
            {
                trace (TRACE_LEVEL_DEBUG, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject : parent was called for remove. trying to remove it now.");
                ManagedObjectSchemaInfo* parentInfoObject = findSchemaInfoObject (myParentTableId);
                removeSchemaInfoObjectAndGetSql (parentInfoObject, schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository, true);
                break;
            }
        }
        
        delete (schemaInfo);
    }
    else
    {
        bool existsAlready = false;
        for (vector <UI32>::iterator iter2 = m_tempInfoObjectIdStoreForRemove.begin (); iter2 != m_tempInfoObjectIdStoreForRemove.end (); ++iter2)
        {
            if (myTableId == *iter2) 
            {
                if (!isInternalCall)
                {
                    tracePrintf (TRACE_LEVEL_FATAL, true, false, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject: called more than once for \"%s\"", myMOName.c_str());
                    prismAssert (false, __FILE__, __LINE__);
                }
                existsAlready = true;
                break;
            }
        }
        if (!existsAlready)
        {
            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "ManagedObjectSchemaInfoRepository::removeSchemaInfoObject : pushing %s", (schemaInfo->getName ()).c_str ());
            m_tempInfoObjectIdStoreForRemove.push_back (myTableId);
        }
    }

    return (FRAMEWORK_SUCCESS);
}

bool ManagedObjectSchemaInfoRepository::removeUpgradeInfoObject (ManagedObjectSchemaInfo *schemaInfo)
{
    // TODO
    return (false);
}

ManagedObjectSchemaInfo *ManagedObjectSchemaInfoRepository::findSchemaInfoObject (UI32 tableId) const
{
    map<UI32, ManagedObjectSchemaInfo *>::const_iterator iter = m_schemaInfoByIdMap.find (tableId);
    if (iter != m_schemaInfoByIdMap.end ())
    {
        return (iter->second);
    }

    return (NULL);
}

ManagedObjectSchemaInfo *ManagedObjectSchemaInfoRepository::findSchemaInfoObject (const string &tableName) const
{
    map<string, ManagedObjectSchemaInfo *>::const_iterator iter = m_schemaInfoByNameMap.find (tableName);
    if (iter != m_schemaInfoByNameMap.end ())
    {
        return (iter->second);
    }

    return (NULL);
}

bool ManagedObjectSchemaInfoRepository::operator== (const ManagedObjectSchemaInfoRepository &rhs) const
{
    if (m_schemaInfoByIdMap.size () != rhs.m_schemaInfoByIdMap.size ())
    {
        trace (TRACE_LEVEL_INFO, string("ManagedObjectSchemaInfoRepository::operator== : Not equal. Table sizes don't match. LHS = ") + m_schemaInfoByIdMap.size () + ", RHS = " + rhs.m_schemaInfoByIdMap.size ());
        return (false);
    }

    for (map<UI32, ManagedObjectSchemaInfo *>::const_iterator iter1 = m_schemaInfoByIdMap.begin ();
          iter1 != m_schemaInfoByIdMap.end (); ++iter1)
    {
        UI32 tableId = (*iter1).first;
        const ManagedObjectSchemaInfo *mo1 = (*iter1).second;
        prismAssert (mo1 != NULL, __FILE__, __LINE__);

        map<UI32, ManagedObjectSchemaInfo *>::const_iterator iter2 = rhs.m_schemaInfoByIdMap.find (tableId);
        if (iter2 == rhs.m_schemaInfoByIdMap.end ())
        {
            return (false);
        }

        const ManagedObjectSchemaInfo *mo2 = (*iter2).second;
        if (*mo1 != *mo2)
        {
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::operator== : Not equal. Table " + mo1->getName () + " doesn't match");
            return (false);
        }
    }

    return (true);
}

bool ManagedObjectSchemaInfoRepository::operator!= (const ManagedObjectSchemaInfoRepository &rhs) const
{
    return (!operator== (rhs));
}

void ManagedObjectSchemaInfoRepository::dumpToDOT(const string &graphName) const
{
    ofstream graphFile;
    string fullFileName = FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + graphName + ".gv";
    graphFile.open (fullFileName.c_str ());
    graphFile << "digraph " << graphName << " {\n";

    for (map<UI32, ManagedObjectSchemaInfo *>::const_iterator iter = m_schemaInfoByIdMap.begin ();
          iter != m_schemaInfoByIdMap.end (); ++iter)
    {
        ManagedObjectSchemaInfo *object = (*iter).second;
        graphFile << "\t " << object->getTableId () << " [label=\"" << object->getName () << "\", shape=box];\n";
    }

    for (map<UI32, ManagedObjectSchemaInfo *>::const_iterator iter = m_schemaInfoByIdMap.begin ();
          iter != m_schemaInfoByIdMap.end (); ++iter)
    {
        ManagedObjectSchemaInfo *object = (*iter).second;
        if (object->getParentTableId () != 0)
        {
            graphFile << "\t " << object->getTableId () << " -> " << object->getParentTableId () << " [arrowhead=empty];\n";
        }
    }

    graphFile << "}\n" << endl;
    graphFile.close ();
}

auto_ptr<ManagedObjectSchemaDifferences> ManagedObjectSchemaInfoRepository::getManagedObjectDifferences (const ManagedObjectSchemaInfoRepository &rhs)
{
    auto_ptr<ManagedObjectSchemaDifferences> pDifference (new ManagedObjectSchemaDifferences ());

    map<string, ManagedObjectSchemaInfo *> temp;
    map<string, ManagedObjectSchemaInfo *>::const_iterator temp_iter;

    // Compute the added ManagedObjects by doing the set_difference 'this-RHS'
    set_difference (m_schemaInfoByNameMap.begin (), m_schemaInfoByNameMap.end (), rhs.m_schemaInfoByNameMap.begin (), rhs.m_schemaInfoByNameMap.end (),
                    inserter (temp, temp.end ()), m_schemaInfoByNameMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        pDifference->insertAddition (temp_iter->second);
    }
    temp.clear ();

    // Compute the removed ManagedObjects by doing the set_difference 'RHS-this'
    set_difference (rhs.m_schemaInfoByNameMap.begin (), rhs.m_schemaInfoByNameMap.end (), m_schemaInfoByNameMap.begin (), m_schemaInfoByNameMap.end (),
                    inserter (temp, temp.end ()), m_schemaInfoByNameMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        pDifference->insertDeletion (temp_iter->second);
    }
    temp.clear ();

    // Compute the changed ManagedObjects by doing the set_intersection 'RHS^this' and
    set_intersection (rhs.m_schemaInfoByNameMap.begin (), rhs.m_schemaInfoByNameMap.end (), m_schemaInfoByNameMap.begin (), m_schemaInfoByNameMap.end (),
                    inserter (temp, temp.end ()), m_schemaInfoByNameMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        // We cannot dereference the iterator here since we need to compare the ManagedObjects
        // on the LHS and RHS before adding to the changed set
        map<string, ManagedObjectSchemaInfo *>::const_iterator lhs_iter = m_schemaInfoByNameMap.find (temp_iter->first);
        map<string, ManagedObjectSchemaInfo *>::const_iterator rhs_iter = rhs.m_schemaInfoByNameMap.find (temp_iter->first);
        prismAssert (lhs_iter != m_schemaInfoByNameMap.end () && rhs_iter != rhs.m_schemaInfoByNameMap.end (), __FILE__, __LINE__);

        if (*(lhs_iter->second) != *(rhs_iter->second))
        {
            pDifference->insertChange (lhs_iter->second);
        }
    }
    temp.clear ();

    return (pDifference);
}

ResourceId ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences (const ManagedObjectSchemaInfoRepository &rhs)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    /*
     * Clean up all the difference vectors.
     * In case of HA join/BIST runs, these vectors are populated multiple times before a restart of DCM.
     */
    cleanupSchemaDifferenceInfo();
    m_userDefinedKeyCombinationChanged = false;

    auto_ptr<ManagedObjectSchemaDifferences> differences = getManagedObjectDifferences(rhs);
    if (differences->hasDifferences () == false)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : No schema changes detected");
        return (WAVE_SCHEMA_UPGRADE_NOT_REQUIRED);
    }

    // Update this flag everytime DB conversion is called.
    // Because if DB conversion is triggered by BIST, we have to check for the existence of file disableSchemaConversion.
    FrameworkToolKit::updateSchemaConversionDisabledByUser ();

    const vector<ManagedObjectSchemaInfo *> &additionsVec = differences->getAdditions ();
    const vector<ManagedObjectSchemaInfo *> &removalsVec = differences->getDeletions ();
    const vector<ManagedObjectSchemaInfo *> &changesVec = differences->getChanges ();

    trace (TRACE_LEVEL_INFO, string("ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Additions: ") + additionsVec.size ());
    
    for (vector<ManagedObjectSchemaInfo *>::const_iterator iter = additionsVec.begin ();
          iter != additionsVec.end (); ++iter)
    {
        ManagedObjectSchemaInfo *object = *iter;
        
        addMOToSchemaDifference (object, true); 

        addAddedTableToSchemaDifference (object);
        
        trace (TRACE_LEVEL_INFO, "+ Managed object '" + object->getName () + "', table Id " + object->getTableId ());

    }

    trace (TRACE_LEVEL_INFO, string ("ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Deletions: ") + removalsVec.size ());

    for (vector<ManagedObjectSchemaInfo *>::const_iterator iter = removalsVec.begin ();
          iter != removalsVec.end (); ++iter)
    {
        ManagedObjectSchemaInfo *object = *iter;

        addRemovedTableToSchemaDifference (object);

        trace (TRACE_LEVEL_INFO, "- Managed object '" + object->getName () + "', table Id " + object->getTableId ());
    }

    trace (TRACE_LEVEL_INFO, string("ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Changes: ") + changesVec.size ());

    for (vector<ManagedObjectSchemaInfo *>::const_iterator iter = changesVec.begin ();
          iter != changesVec.end (); ++iter)
    {
        ManagedObjectSchemaInfo *pLhsObject = *iter;
        ManagedObjectSchemaInfo *pRhsObject = rhs.findSchemaInfoObject (pLhsObject->getName ());

	    if (pRhsObject == NULL)
    	{
	        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Cannot find MO " + pLhsObject->getName ());
	        prismAssert (false, __FILE__, __LINE__);
    	}

        tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Finding changes for MO : %s \n",pLhsObject->getName ().c_str() );

        ResourceId fieldDiffStatus                  = FRAMEWORK_SUCCESS;
        ResourceId relationFieldDiffStatus          = FRAMEWORK_SUCCESS;
        ResourceId userDefinedKeyFieldDiffStatus    = FRAMEWORK_SUCCESS;
        ResourceId inheritanceDiffStatus            = FRAMEWORK_SUCCESS;

        auto_ptr<FieldSchemaDifferences>            fieldDiff               = pLhsObject->getFieldObjectDifferences (*pRhsObject, fieldDiffStatus);
        auto_ptr<RelationFieldSchemaDifferences>    relationFieldDiff       = pLhsObject->getRelationFieldObjectDifferences (*pRhsObject, relationFieldDiffStatus);
        auto_ptr<FieldSchemaDifferences>            userDefinedKeyFieldDiff = pLhsObject->getUserDefinedKeyDifferences (*pRhsObject, userDefinedKeyFieldDiffStatus);
        inheritanceDiffStatus = pLhsObject->getInheritanceDiffStatus (*pRhsObject);

        trace (TRACE_LEVEL_INFO, "FieldDiff ... : '" + FrameworkToolKit::localize(fieldDiffStatus) + "'");
        trace (TRACE_LEVEL_INFO, "Relation FieldDiff ... : '" + FrameworkToolKit::localize(relationFieldDiffStatus) + "'");
        trace (TRACE_LEVEL_INFO, "User Defined Key FieldDiff ... : '" + FrameworkToolKit::localize(userDefinedKeyFieldDiffStatus) + "'");
        trace (TRACE_LEVEL_INFO, "Inheritance Diff... : '" + FrameworkToolKit::localize(inheritanceDiffStatus) + "'");

        if ((fieldDiffStatus == FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS)
                || (relationFieldDiffStatus == FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS)
                || (userDefinedKeyFieldDiffStatus == FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS)
                || (inheritanceDiffStatus == FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS))
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Inconsistency detected while comparing managed object '" + pLhsObject->getName () + "'. ");
            prismAssert (false, __FILE__, __LINE__);
        }
        else if ((fieldDiffStatus == FRAMEWORK_STATUS_NO_CHANGES)
                    && (relationFieldDiffStatus == FRAMEWORK_STATUS_NO_CHANGES)
                    && (userDefinedKeyFieldDiffStatus == FRAMEWORK_STATUS_NO_CHANGES)
                    && (inheritanceDiffStatus == FRAMEWORK_STATUS_NO_CHANGES))
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : No differences found in '" + pLhsObject->getName () + "' even though changes were detected. ");
            prismAssert (false, __FILE__, __LINE__);
        }
        else if (fieldDiffStatus == FRAMEWORK_STATUS_LOCAL_GLOBAL_CHANGE_NOT_SUPPORTED)
        {   
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Change from local/global to global/local change for " + pLhsObject->getName () + "' not supported. Please check failure. ");  
            status = fieldDiffStatus;
        }
        else if (fieldDiffStatus == FRAMEWORK_STATUS_FIELD_CHANGE_NOT_SUPPORTED)
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Simple field changes in '" + pLhsObject->getName () + "' not supported. Please check failure. ");
            status = fieldDiffStatus;
        }
        else if (relationFieldDiffStatus == FRAMEWORK_STATUS_RELATION_FIELD_CHANGE_NOT_SUPPORTED)
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Relation field changes in '" + pLhsObject->getName () + "' not supported. Please check failure. ");
            status = relationFieldDiffStatus;
        }
        else if (userDefinedKeyFieldDiffStatus == FRAMEWORK_STATUS_USER_DEFINED_KEY_COMBINATION_CHANGES_NOT_SUPPORTED)
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : User Defined Key Combination Field Changes in '" + pLhsObject->getName () + "'. This is not supported. Please check the failure. ");
            status = userDefinedKeyFieldDiffStatus;
        }
        else if (inheritanceDiffStatus == FRAMEWORK_STATUS_PARENT_TABLE_CHANGE_NOT_SUPPORTED)
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Inheritance Parent of '" + pLhsObject->getName () + "' is changed. This is not supported. Please check the failure. ");
            status = inheritanceDiffStatus;
        }
        else if (FRAMEWORK_STATUS_DERIVED_FROM_TABLE_CHANGED == inheritanceDiffStatus)
        {
            trace (TRACE_LEVEL_WARN, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Derived From class name for '" + pLhsObject->getName () + "' is changed. This is currently ignored. Can be used in future cases if required. Currenlty, only detection logic is added.");
        }
        else if ((FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE == fieldDiffStatus) || (FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE == relationFieldDiffStatus) || (FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE == userDefinedKeyFieldDiffStatus))
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Table-id change detected which is not currently supported for . '" + pLhsObject->getName () + "'. ");
            status = FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE;
        }
        else
        {
            addMOToSchemaDifference (pLhsObject, false); 

            addModifiedTableToSchemaDifference (pLhsObject, pRhsObject, fieldDiff.get(), relationFieldDiff.get ());

            if (FrameworkToolKit::isSchemaConversionDisabledByUser ())
            {
                if (relationFieldDiffStatus == FRAMEWORK_SUCCESS)
                {
                    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Relationship differences found between Schemas and at this point conversion doesnt support it. So, going back to first time boot. ");
                    status = FRAMEWORK_STATUS_UNHANDLED_RELATIONS;    
                }
                else 
                {
                    applyFieldDifferences (pLhsObject, fieldDiff.get());
                }
            }
            else
            {
                applyFieldDifferences (pLhsObject, fieldDiff.get());
            }
        }

        if ((FRAMEWORK_SUCCESS == status) && (((NULL != relationFieldDiff.get ()) && (relationFieldDiff.get ())->hasDifferences ()) 
                                                  || ((NULL != userDefinedKeyFieldDiff.get ()) && (userDefinedKeyFieldDiff.get ())->hasDifferences ()))) 
        {
            m_userDefinedKeyCombinationChanged = true;
        }

    }

    if ((FRAMEWORK_SUCCESS == status) && ((additionsVec.size () > 0) || (removalsVec.size () > 0)))
    {
        m_userDefinedKeyCombinationChanged = true; 
    }

    if (FrameworkToolKit::isSchemaConversionDisabledByUser ())
    {
        if ((additionsVec.size () > 0) || (removalsVec.size () > 0))
        {
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : FRAMEWORK_STATUS_UNHANDLED_MANAGEDOBJECTS due to additions ");
	        status = FRAMEWORK_STATUS_UNHANDLED_MANAGEDOBJECTS;
        }
    }

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Framework cannot convert DB during upgrade/downgrade for the above schema changes.");
        //prismAssert (false, __FILE__, __LINE__);
        FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------");
        FrameworkToolKit::consolePrint ("Schema changes detected are not supported by schema conversion logic. Please, check.");
        FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------");
    }

    /*
     * To support capricorn to leo downgrade, we should not have any deletions of MO/field, modifications of field
     * This check has to be removed by the end of capricorn.
     */
    if (removalsVec.size () > 0)
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Deleteion of MOs are not supported.");
        //prismAssert (false, __FILE__, __LINE__);
        FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------------------");
        FrameworkToolKit::consolePrint ("Schema changes detected 'Deletion of following MOs' which will break Capricorn to Leo downgrade. Please, take care.");

        for (vector<ManagedObjectSchemaInfo *>::const_iterator iter = removalsVec.begin (); iter != removalsVec.end (); ++iter)
        {
            ManagedObjectSchemaInfo *object = *iter;
            trace (TRACE_LEVEL_INFO, "\n- Managed object '" + object->getName ()); 
        }

        FrameworkToolKit::consolePrint ("--------------------------------------------------------------------------------------------------------------------");
    }
        
    map<string, ModifiedManagedObjectSchemaDifference>::iterator mIterator;

    for (mIterator=m_modifiedTablesSchemaDifference.begin();mIterator!=m_modifiedTablesSchemaDifference.end();mIterator++)
    {
        if (0 != (mIterator->second).getRemovedFieldsInfo ().size())    // || 0 != (mIterator->second).getChangedFieldsInfo ().size())
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Deletion of simple fields are not supported.");
            //prismAssert (false, __FILE__, __LINE__);
            FrameworkToolKit::consolePrint ("--------------------Unsupported schema change detected : 'Deletion of simple field/s' for :---------------------");
            FrameworkToolKit::consolePrint (mIterator->first.c_str());
        }
        if (0 != (mIterator->second).getRemovedRelations ().size() || 0 != (mIterator->second).getChangedRelations ().size())
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::computeDatabaseSchemaDifferences : Deletion or Modification of relation fields are not supported.");
            //prismAssert (false, __FILE__, __LINE__);
            FrameworkToolKit::consolePrint ("--------------------Unsupported schema change detected : 'Deletion or Modification of relation field/s' for :---------------");
            FrameworkToolKit::consolePrint (mIterator->first.c_str());
        }

    }

    return (status);

}


ResourceId ManagedObjectSchemaInfoRepository::applyDatabaseSchemaDifferences (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applyDatabaseSchemaDifferences: Enter.. ");

    ResourceId status = applySqlToDatabase (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applyDatabaseSchemaDifferences: failed to apply schema differences to Database.");
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applyDatabaseSchemaDifferences : successfully converted the database.");
    }

    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::applySqlToDatabase (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applySqlToDatabase: Enter.. ");

    string schemaUpdateSql = schemaUpdateSqlForCurrentSchema + schemaUpdateSqlForStartSchema;

    if (schemaUpdateSql.empty () == false)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applySqlToDatabase: sending a message to DatabaseOM.");

        DatabaseObjectManagerExecuteTransactionMessage message (schemaUpdateSql);
        status = WaveObjectManagerToolKit::sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_WARN, "ManagedObjectSchemaInfoRepository::applySqlToDatabase: Could not send message DatabaseOM. Status : " + FrameworkToolKit::localize (status));
        
            if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == status)
            {
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applySqlToDatabase: As DatabaseOM is not enabled, using DatabaseStandaloneTransaction.");
                
                DatabaseStandaloneTransaction dbStandaloneTransaction;
                if (!dbStandaloneTransaction.execute (schemaUpdateSql))
                {
                    trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::applySqlToDatabase : Error in applying SQL to database");
                    status = FRAMEWORK_STATUS_CONVERSION_FAILURE;
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applySqlToDatabase : successfully applied sql to the database.");
                    status = FRAMEWORK_SUCCESS;
                }
            }
        }
        else
        {   
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {   
                trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::applyDatabaseSchemaDifference: failed to apply SQL to database by DBOM. Status : " + FrameworkToolKit::localize (status));
                status = FRAMEWORK_STATUS_CONVERSION_FAILURE;
            }
            else
            {   
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::applyDatabaseSchemaDifference: successfully applied sql to the database using DBOM.");
                status = FRAMEWORK_SUCCESS;
            }
        }
    }

    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::getSqlForSimpleFieldsInModifiedManagedObjects (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    /* 1. Loop over the vector of Modified Managed Objects 
     * 2. For each Modified MO, findSchemaInfoObject
     * 3. Call getSqlForTableUpdate for both WaveCurrent and WaveStart schemas
     */

    map<string, ModifiedManagedObjectSchemaDifference>::const_iterator modifiedIterator;

    for (modifiedIterator=m_modifiedTablesSchemaDifference.begin (); modifiedIterator!=m_modifiedTablesSchemaDifference.end ();modifiedIterator++)
    {
        ManagedObjectSchemaInfo *pModifiedManagedObject = findSchemaInfoObject (modifiedIterator->first);
         
        if (NULL == pModifiedManagedObject)
        {
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlForSimpleFieldsInModifiedManagedObjects : Simple Field Differences not found");
            return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
        }

        schemaUpdateSqlForCurrentSchema += pModifiedManagedObject->getSQLForTableUpdate (OrmRepository::getWaveCurrentSchema (), (modifiedIterator->second).getAddedFieldsInfo (), (modifiedIterator->second).getRemovedFieldsInfo (), (modifiedIterator->second).getChangedFieldsInfo ());
        schemaUpdateSqlForStartSchema += pModifiedManagedObject->getSQLForTableUpdate (OrmRepository::getWaveStartSchema (), (modifiedIterator->second).getAddedFieldsInfo (), (modifiedIterator->second).getRemovedFieldsInfo (), (modifiedIterator->second).getChangedFieldsInfo ());

    }
        
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlForSimpleFieldsInModifiedManagedObjects : Field update SQL For Current Schema " + schemaUpdateSqlForCurrentSchema);
    
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlForSimpleFieldsInModifiedManagedObjects : Field update SQL For Start Schema" + schemaUpdateSqlForStartSchema);

    return (FRAMEWORK_SUCCESS);
}

ResourceId ManagedObjectSchemaInfoRepository::getSqlForRemovedRelationshipsInModifiedManagedObjects (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    /* 1. Loop over the vector of Modified Managed Objects 
     * 2. For each Modified MO, findSchemaInfoObject
     * 3. For each Removed Relationship in each Modified MO, get the Sql 
     * 
     */
    
    map<string, ModifiedManagedObjectSchemaDifference>::const_iterator modifiedIterator;

    for (modifiedIterator=m_modifiedTablesSchemaDifference.begin (); modifiedIterator!=m_modifiedTablesSchemaDifference.end ();modifiedIterator++)
    {
        ManagedObjectSchemaInfo *pModifiedManagedObject = dbSchemaInfoRepository.findSchemaInfoObject (modifiedIterator->first);
         
        if (NULL == pModifiedManagedObject)
        {
            return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
        }
        
        tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfoRepository::getSqlForRemovedRelationshipsInModifiedManagedObjects: handling table:%s", (pModifiedManagedObject->getName()).c_str());

        vector<RelationshipInfoFromSchemaDifference> removedRelationDiff =  (modifiedIterator->second).getRemovedRelations ();
        vector<RelationshipInfoFromSchemaDifference>::iterator relationIterator; 

        for (relationIterator=removedRelationDiff.begin(); relationIterator!=removedRelationDiff.end();relationIterator++)
        {
            status = pModifiedManagedObject->getSqlForRemovedRelationship (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, (*relationIterator).getRelationName(), (*relationIterator).getRelatedToTable(), (*relationIterator).getRelationUmlType(), (*relationIterator).getRelationType(), dbSchemaInfoRepository);
        }
    }

    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::getSqlForRelationshipsInRemovedManagedObjects (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    /* 1. Loop over the vector of Removed Managed Objects 
     * 2. For each Removed MO, findSchemaInfoObject
     * 3. Call getSqlForRemoveRelationships for each Removed MO 
     */
    
    map<string, AddedRemovedManagedObjectSchemaDifference>::const_iterator removedIterator;

    for (removedIterator=m_removedTablesSchemaDifference.begin (); removedIterator!=m_removedTablesSchemaDifference.end ();removedIterator++)
    {
        ManagedObjectSchemaInfo *pRemovedManagedObject = dbSchemaInfoRepository.findSchemaInfoObject (removedIterator->first);
         
        if (NULL == pRemovedManagedObject)
        {
            return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
        }
        
        vector<RelationshipInfoFromSchemaDifference> relationDiff =  (removedIterator->second).getRelations ();
        vector<RelationshipInfoFromSchemaDifference>::iterator relationIterator; 

        for (relationIterator=relationDiff.begin(); relationIterator!=relationDiff.end(); relationIterator++)
        {
            status = pRemovedManagedObject->getSqlForRemovedRelationship (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, (*relationIterator).getRelationName(), (*relationIterator).getRelatedToTable(), (*relationIterator).getRelationUmlType(), (*relationIterator).getRelationType(), dbSchemaInfoRepository);
        }
    }
    
    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::getSqlToRemoveManagedObject (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfo *pRemovedManagedObject, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{

    ResourceId status = FRAMEWORK_SUCCESS;
    
    /* There are 2 cases here:      * 1. RemovedManagedObject has a relationship and is at 'from' End.
      * 2. RemovedManagedObject has a relationship and is at 'to' End.
      * #1. For 'from" end RemovedManagedObject, updating the relationship is already being taken care in last step.
      * #2. For 'to' end RemovedManagedObject, need to identify if the relationship from the tables pointing ot it has taken care or not.
      *     Need to check if the modifiedRelations/RemovedRelations are taken care or not. 
     */

    
    /* 
     * 0. Check no relations are pointing to or is being pointed by this ManagedObjectSchemaInfo object. This will take care of the above two cases.
     * 1. Remove the entries of this MO from the Auxillary tables in its hierarchy
     * 2. Find the class id and the vector of instance id's
     * 3. Generate sql for Removing the managed object
     * 4. Call getSqlToRemoveInstanceIdsFromDerivationInstancesTables to the get the 
     *    sql to remove the class id and instance id's from the derivation, derivation 
     *    instances and auxilliary tables of the current table and all its parents .. 
     *    ... Call the helper function
     * 5. Remove the managed object from the DB tree using removeSchemaInfoObject
     */ 
        
    // 0  
    if ((0 < pRemovedManagedObject->getCountOfRelationshipReferecePointers ()) || (0 < pRemovedManagedObject->getReferenceCount ()))
    {
        tracePrintf (TRACE_LEVEL_FATAL, true, false, "ManagedObjectSchemaInfo::getSqlToRemoveManagedObject : few relationships still pointed by or are pointing to \"%s\"", pRemovedManagedObject->getName().c_str());
        prismAssert (false, __FILE__, __LINE__);
    }

    // 1 
    status = getSqlToRemoveEntriesFromAuxillaryTables (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, pRemovedManagedObject->getName (), pRemovedManagedObject->getTableId (), dbSchemaInfoRepository);
    if (FRAMEWORK_SUCCESS != status)
    {
        // Add a trace here
        return (status);
    }
    
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository:::getSqlToRemoveManagedObject sql for AuxilliaryTable are :\n" + schemaUpdateSqlForCurrentSchema + "\n" + schemaUpdateSqlForStartSchema);
#if 0
    // 2. Get instanceIds for a table and call the cleanup function.
    vector<UI64> objectInstanceIds;
    DatabaseStandaloneTransaction dbStandaloneTransaction;
    
    bool success = dbStandaloneTransaction.getObjectInstanceIdsForTable (pRemovedManagedObject->getName (), objectInstanceIds);
    if (false == success)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToRemoveManagedObject : failed to get the derivationsInstanceIds for table " + pRemovedManagedObject->getName ());
        return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
    }

    // 3 
    schemaUpdateSqlForCurrentSchema += "DROP TABLE " + OrmRepository::getWaveCurrentSchema () + "." + pRemovedManagedObject->getName () + ";" + " \n";
    schemaUpdateSqlForStartSchema   += "DROP TABLE " + OrmRepository::getWaveStartSchema ()   + "." + pRemovedManagedObject->getName () + ";" + " \n";

    // 4
    status = getSqlToRemoveInstanceIdsFromDerivationInstancesTables (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository, pRemovedManagedObject->getName (), pRemovedManagedObject->getTableId(), objectInstanceIds);
    if (FRAMEWORK_SUCCESS != status)
    {
        // Add a trace here
        return (status);
    }
#endif    
    // 5
    status = dbSchemaInfoRepository.removeSchemaInfoObjectAndGetSql (pRemovedManagedObject, schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository);
    if (FRAMEWORK_SUCCESS != status)
    {
        // Add a trace here
        return (status);
    }
    
    return (status);

}

ResourceId ManagedObjectSchemaInfoRepository::getSqlToRemoveEntriesFromAuxillaryTables (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, string tableName, const UI32 tableId, ManagedObjectSchemaInfoRepository & dbRepository)
{
    // 1. Get the parent MOSchemaInfo object. 
    // 2. check if table has auxillary tables. or check number of 1:N relations pointed by parent class. Remove class-id of MO from all the auxillary tables
    // 3. Check if parent table has any 1:N relations pointing to it. If yes, get the from table/s and remove the class-id of MO from those tables.

    // 1.
    ManagedObjectSchemaInfo *schemaInfoObject = dbRepository.findSchemaInfoObject (tableName);
    prismAssert (NULL != schemaInfoObject, __FILE__, __LINE__);

    UI32 parent = schemaInfoObject->getParentTableId ();                                                                                                     
    if (0 != parent)
    {
        ManagedObjectSchemaInfo *parentInfoObject = dbRepository.findSchemaInfoObject (parent);
        string                  parentClassName   = parentInfoObject->getName ();

        if (parentInfoObject->getNumberOf1tonRelationFields ())
        {
            vector<string> auxillaryTables;
            OrmRepository::getAuxilliaryTableNames (parentClassName, auxillaryTables);
            
            for (vector<string>::const_iterator iter = auxillaryTables.begin (); iter!=auxillaryTables.end(); ++iter)
            {
                schemaUpdateSqlForCurrentSchema += OrmRepository::getSqlToRemoveEntriesFromAuxilliaryTable (*iter, tableId, OrmRepository::getWaveCurrentSchema ());
                schemaUpdateSqlForStartSchema += OrmRepository::getSqlToRemoveEntriesFromAuxilliaryTable (*iter, tableId, OrmRepository::getWaveStartSchema ());
            }
        }

        if (parentInfoObject->getReferenceCount ())
        {
            tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfoRepository::getSqlToRemoveEntriesFromAuxillaryTables: [%s] has a relation from other tables.");
            parentInfoObject->getSqlToRemoveAuxilliaryTableEntriesForToEndTables (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, tableId);     
        }
        trace (TRACE_LEVEL_DEBUG, "ManagedObjectSchemaInfoRepository::getSqlToRemoveEntriesFromAuxillaryTables: sql are :\n" + schemaUpdateSqlForCurrentSchema + "\n" + schemaUpdateSqlForStartSchema);

        getSqlToRemoveEntriesFromAuxillaryTables (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, parentClassName, tableId, dbRepository);        
    } 
    else 
    {
        tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfoRepository::getSqlToRemoveEntriesFromAuxillaryTables: [%s] is most base class. Hence, returning", tableName.c_str());
    }


    return (FRAMEWORK_SUCCESS);

}

void ManagedObjectSchemaInfoRepository::addToAHeirarchyList (const ManagedObjectSchemaInfo* moSchemaInfoObject, map<string, AddedRemovedManagedObjectSchemaDifference>& addedOrRemovedTablesSchemaDifference, vector<string>& addedOrRemovedManagedObjects, bool isAddCase, const ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    string                      tableName       = moSchemaInfoObject->getName ();
    UI32                        parentTableId   = moSchemaInfoObject->getParentTableId ();
    ManagedObjectSchemaInfo*    parentTable     = NULL;


    if (0 != parentTableId)
    {
        if (isAddCase)
        {
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::addToAHeirarchyList: this is a add MO case.");
            parentTable = findSchemaInfoObject (parentTableId);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::addToAHeirarchyList: this is a remove MO case.");
            parentTable = dbSchemaInfoRepository.findSchemaInfoObject (parentTableId);
        }

        if (NULL != parentTable)
        {
            map<string, AddedRemovedManagedObjectSchemaDifference>::iterator iter = addedOrRemovedTablesSchemaDifference.find (parentTable->getName ());

            if (addedOrRemovedTablesSchemaDifference.end () != iter)
            {
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::addToAHeirarchyList: parent [" + parentTable->getName () + "] exists in list.");

                addToAHeirarchyList (parentTable, addedOrRemovedTablesSchemaDifference, addedOrRemovedManagedObjects, isAddCase, dbSchemaInfoRepository);
            }
        }    
    }
    
    addedOrRemovedManagedObjects.push_back (moSchemaInfoObject->getName ());

    map<string, AddedRemovedManagedObjectSchemaDifference>::iterator it = addedOrRemovedTablesSchemaDifference.find (tableName);

    addedOrRemovedTablesSchemaDifference.erase (it);

    return;
}
    
//void ManagedObjectSchemaInfoRepository::setupHierarchialOrder (map<string, AddedRemovedManagedObjectSchemaDifference> addedOrRemovedTablesSchemaDifference, vector<string>& addedOrRemovedManagedObjects, bool isAddCase, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
ResourceId ManagedObjectSchemaInfoRepository::setupHierarchialOrder (vector<string>& addedOrRemovedManagedObjects, bool isAddCase, const ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    map<string, AddedRemovedManagedObjectSchemaDifference> addedOrRemovedTablesSchemaDifference = (isAddCase)?(m_addedTablesSchemaDifference):(m_removedTablesSchemaDifference);
    UI32                                                   noOfMOsInList                        = addedOrRemovedTablesSchemaDifference.size ();
        
    for (UI32 i = 0; i < noOfMOsInList; i++)
    {
        if (0 < addedOrRemovedTablesSchemaDifference.size ())
        {
            string moName = (addedOrRemovedTablesSchemaDifference.begin ())->first;
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::setupHierarchialOrder : moName [" + moName + "]");

            ManagedObjectSchemaInfo* moSchemaInfoObject;

            if (isAddCase)
            {
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::setupHierarchialOrder: this is a add MO case.");
                moSchemaInfoObject = findSchemaInfoObject (moName);
            }
            else
            {    
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::setupHierarchialOrder: this is a remove MO case.");
                moSchemaInfoObject = dbSchemaInfoRepository.findSchemaInfoObject (moName);
            }

            if (NULL == moSchemaInfoObject)
            {
                return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
            }

            addToAHeirarchyList (moSchemaInfoObject, addedOrRemovedTablesSchemaDifference, addedOrRemovedManagedObjects, isAddCase, dbSchemaInfoRepository);
        }
        
    }

    return (FRAMEWORK_SUCCESS);
}

ResourceId ManagedObjectSchemaInfoRepository::getSqlForRemovedManagedObjects (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS;
     
    /* 1. Loop over the vector of Removed Managed Objects 
       2. For each Removed MO, findSchemaInfoObject
       3. Call getSqlForRemoveManagedObject for each Removed MO
     */

    vector<string> removedManagedObjectsInHierarchialOrder;

    status = setupHierarchialOrder (removedManagedObjectsInHierarchialOrder, false, dbSchemaInfoRepository);

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::getSqlForRemovedManagedObjects: failed to setupHierarchialOrder for removed MOs.");
        return (status);
    }

    /*map<string, AddedRemovedManagedObjectSchemaDifference>::const_iterator removedIterator;

    for (removedIterator=m_removedTablesSchemaDifference.begin (); removedIterator!=m_removedTablesSchemaDifference.end ();removedIterator++)*/

    vector<string>::reverse_iterator rit;

    for (rit = removedManagedObjectsInHierarchialOrder.rbegin (); rit != removedManagedObjectsInHierarchialOrder.rend (); ++rit)
    {
        //ManagedObjectSchemaInfo *pRemovedManagedObject = dbSchemaInfoRepository.findSchemaInfoObject (removedIterator->first);
        ManagedObjectSchemaInfo *pRemovedManagedObject = dbSchemaInfoRepository.findSchemaInfoObject (*rit);
         
        if (NULL == pRemovedManagedObject)
        {
            return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
        }
        
        status = getSqlToRemoveManagedObject (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, pRemovedManagedObject, dbSchemaInfoRepository);
    }

    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::getSqlForRemovedManagedObjectsAndRelationships (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    // Get the sql for all removed relationships in Modified Managed Objects 
    status = getSqlForRemovedRelationshipsInModifiedManagedObjects (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository);
    if (FRAMEWORK_SUCCESS != status)
    {
        // Add trace here 
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlForRemovedManagedObjectsAndRelationships : Failure in getSqlForRemovedRelationshipsInModifiedManagedObjects");
        return (status);
    }

    // Get the sql for all removed relationships in Removed Managed Objects 
    status = getSqlForRelationshipsInRemovedManagedObjects (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository);
    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlForRemovedManagedObjectsAndRelationships : Failure in getSqlForRelationshipsInRemovedManagedObjects");
        return (status);
    }

    // Get the sql to remove all Removed Managed Objects 
    status = getSqlForRemovedManagedObjects (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository);
    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlForRemovedManagedObjectsAndRelationships : Failure in getSqlForRemovedManagedObjects");
        return (status);
    }

    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::generateSqlForSchemaDifferences (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS;
    
    // Get the sql for all the simple fields in Modified Managed Objects 
    status = getSqlForSimpleFieldsInModifiedManagedObjects (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);
    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::generateSqlForSchemaDifferences : getSqlForSimpleFieldsInModifiedManagedObjects returning Failure...");
        return (status);
    }

    if (false == FrameworkToolKit::isSchemaConversionDisabledByUser ())
    {
        // Get the sql for all the Removed Managed Objects and Relationships
        status = getSqlForRemovedManagedObjectsAndRelationships (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository);
        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::generateSqlForSchemaDifferences : getSqlForRemovedManagedObjectsAndRelationships  returning Failure...");
            return (status);
        }
    
        generateSqlsForModifiedRelationShipsInModifiedObjects (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);

        // Get the sql for all Added Managed Objects ... 
        processAdditionOfNewMOsAndRelationships(schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbSchemaInfoRepository);
    }

    trace (TRACE_LEVEL_DEVEL, "ManagedObjectSchemaInfoRepository::generateSqlForSchemaDifferences : sql is " + schemaUpdateSqlForCurrentSchema);
    return (status);
}

ResourceId ManagedObjectSchemaInfoRepository::handleDatabaseSchemaDifferences (ManagedObjectSchemaInfoRepository &rhs)
{
    trace (TRACE_LEVEL_DEBUG, "ManagedObjectSchemaInfoRepository::handleDatabaseSchemaDifferences : entering...");
    
    //ResourceId status = FRAMEWORK_STATUS_UNHANDLED_MANAGEDOBJECTS;
    ResourceId status = FRAMEWORK_SUCCESS;

    string schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema;
    
    // drop all db views irrespective of DB conversion status.
    dropAllDatabaseViewsForUpgrade ();

    status = computeDatabaseSchemaDifferences (rhs);

    if (FRAMEWORK_SUCCESS == status)
    {
        /*
            To support downgrade from Capricorn to Leo, this check will be removed. 
            In Capricorn we should make sure that there are no schema changes(like deletions of columns or MOs/modification of relations)
            so that it can be downgraded to Leo without any need for plugin upgrade/downgrade(virutal) function 
        */
        if (-1 == FrameworkToolKit::getFirmwareDownloadCase())
        {
            trace (TRACE_LEVEL_WARN, "ManagedObjectSchemaInfoRepository::handleDatabaseSchemaDifferences : the new firmware has schema changes that is supported by db conversion but it's a firmware downgrade case. Plugin's upgrade function/virtual function will not be able to handle this downgrade.");
            //trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::handleDatabaseSchemaDifferences : the new firmware has schema changes that can be supported by db conversion but it's a firmware downgrade case.");
            //return (FRAMEWORK_STATUS_SCHEMA_UPGRADE_FOR_FW_DOWNGRADE_NOT_SUPPORTED);
        }

        printDifferenceContentsForDebugging ();
    
        status = generateSqlForSchemaDifferences (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, rhs);
    
        if (FRAMEWORK_SUCCESS == status)
        {
            status = applyDatabaseSchemaDifferences (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema); 


            if ((FRAMEWORK_SUCCESS == status) && (true == m_userDefinedKeyCombinationChanged))
            {
                // Drop unique key constraint for userDefinedKeyCombination if necessary.
                schemaUpdateSqlForCurrentSchema = "";
                schemaUpdateSqlForStartSchema   = "";                
                getSqlToDropUserDefinedKeyUniqueConstraint (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);

                status = applyDatabaseSchemaDifferences (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema); 
            }


            if ((FRAMEWORK_SUCCESS == status) && (true == m_userDefinedKeyCombinationChanged))
            {
                // Drop unique key constraint for userDefinedKeyCombination if necessary.
                schemaUpdateSqlForCurrentSchema = "";
                schemaUpdateSqlForStartSchema   = "";                
                getSqlToDropUserDefinedKeyUniqueConstraint (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);

                status = applySqlToDatabase (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema); 
            }
        }
    }

    // apply sql for view generation irrespective of upgrade status.
    createAllDatabaseViewsForUpgrade ();

    return (status);
}

void ManagedObjectSchemaInfoRepository::addMOToSchemaDifference (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, bool isNewManagedObject)
{
    ManagedObjectSchemaDifferenceInfo *pManagedObjectSchemaDifferenceInfo = new ManagedObjectSchemaDifferenceInfo (pManagedObjectSchemaInfo->getName (), isNewManagedObject); 

    m_schemaDifferenceInfo[pManagedObjectSchemaInfo->getName ()] = pManagedObjectSchemaDifferenceInfo;
}

ManagedObjectSchemaDifferenceInfo *ManagedObjectSchemaInfoRepository::findMOInSchemaDifferenceInfo (const string &managedObjectName)
{
    map<string, ManagedObjectSchemaDifferenceInfo *>::iterator moSchemaDiffInfoIterator = m_schemaDifferenceInfo.find (managedObjectName);

    if (moSchemaDiffInfoIterator != m_schemaDifferenceInfo.end())
    {
        return (moSchemaDiffInfoIterator->second);
    }

    return (NULL);
}

void ManagedObjectSchemaInfoRepository::addMOFieldToSchemaDifference (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, const string &fieldName, const string &fieldType)
{
     OrmRepository *pOrmRepository = OrmRepository::getInstance ();

     ManagedObjectSchemaDifferenceInfo *pManagedObjectSchemaDifferenceInfo = m_schemaDifferenceInfo [pManagedObjectSchemaInfo->getName ()];
     
     Field *pField = new Field (fieldName, fieldType, pManagedObjectSchemaInfo->getName ());

     OrmTable *pOrmTable = pOrmRepository->getTableByName (pManagedObjectSchemaInfo->getName ());

     Attribute *pAttribute = pOrmTable->getAttributeForFieldNameInManagedObject (fieldName);
     pField->setAttribute (pAttribute->clone ());
     
     pManagedObjectSchemaDifferenceInfo->addAddedField (pField);
}

void ManagedObjectSchemaInfoRepository::applyFieldDifferences (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, const FieldSchemaDifferences *differences)
{
    tracePrintf (TRACE_LEVEL_INFO, false, false, "OrmRepository::applyFieldDifferences : Entering for MO : %s\n", pManagedObjectSchemaInfo->getName ().c_str());

    if (differences->hasDifferences ())
    {
        vector<pair<string, string> >::const_iterator iter;

        const vector<pair<string, string> >& fieldremovals = differences->getDeletions ();
        for (iter = fieldremovals.begin (); iter != fieldremovals.end (); ++iter)
        {
            // Dont do anything for removed fields 
        }

        vector<pair<string, string> > fieldadditions = differences->getAdditions ();
        for (iter = fieldadditions.begin (); iter != fieldadditions.end (); ++iter)
        {
            tracePrintf (TRACE_LEVEL_INFO, false, false, "Adding Field : %s, Type: %s\n", (*iter).first.c_str(), (*iter).second.c_str());

            string sqlDataType;
            string attributeType;
            string fieldName = iter->first;

            ManagedObjectSchemaInfo::getDataTypesFromExtendedFieldType (iter->second, attributeType, sqlDataType);
            if (FrameworkToolKit::localizeToSourceCodeEnum (WAVE_ATTRIBUTE_TYPE_OBJECTID) == attributeType)
            {
                /* If the attributeType is OBJECTID, we need to map two fieldNames(ClassId and InstanceId) to a single Attribute */
                /* So, if the fieldName not ends with "ClassId", ignore that field and do not add that field to Schema Difference */
                string classId = (iter->first).substr((iter->first).length() - 7);
                fieldName = (iter->first).substr(0, (iter->first).length() - 7);
                if ("ClassId" != classId)
                {
                    /* In this case the value of classId will be tanceId */
                    continue;
                }
            }

            addMOFieldToSchemaDifference (pManagedObjectSchemaInfo, fieldName, (*iter).second);
        }

        vector<pair<string, string> > fieldchanges = differences->getChanges ();
        for (iter = fieldchanges.begin (); iter != fieldchanges.end (); ++iter)
        {
            tracePrintf (TRACE_LEVEL_INFO, false, false, "Changing Field : %s, Type: %s\n", (*iter).first.c_str(), (*iter).second.c_str());

            string sqlDataType;
            string attributeType;
            string fieldName = iter->first;

            ManagedObjectSchemaInfo::getDataTypesFromExtendedFieldType (iter->second, attributeType, sqlDataType);
            if (FrameworkToolKit::localizeToSourceCodeEnum (WAVE_ATTRIBUTE_TYPE_OBJECTID) == attributeType)
            {
                /* If the attributeType is OBJECTID, we need to map two fieldNames(ClassId and InstanceId) to a single Attribute */
                /* So, if the fieldName not ends with "ClassId", ignore that field and do not add that field to Schema Difference */

                string classId = (iter->first).substr((iter->first).length() - 7);
                fieldName = (iter->first).substr(0, (iter->first).length() - 7);
                if ("ClassId" != classId)
                {
                    /* In this case the value of classId will be tanceId */
                    continue;
                }
            }

//            addMOFieldToSchemaDifference (pManagedObjectSchemaInfo, fieldName, (*iter).second);
        }
    }
    
}

void ManagedObjectSchemaInfoRepository::cleanupSchemaDifferenceInfo ()
{
    map <string, ManagedObjectSchemaDifferenceInfo *>::iterator schemaDifferenceInfoIterator;

    for (schemaDifferenceInfoIterator=m_schemaDifferenceInfo.begin(); schemaDifferenceInfoIterator!=m_schemaDifferenceInfo.end(); schemaDifferenceInfoIterator++)
    {
        delete (schemaDifferenceInfoIterator->second);
    }
    
    m_schemaDifferenceInfo.clear ();
    m_modifiedTablesSchemaDifference.clear ();
    m_addedTablesSchemaDifference.clear ();
    m_removedTablesSchemaDifference.clear ();
}


void ManagedObjectSchemaInfoRepository::populateSchemaInfoObjectsWithRelationshipPointers ()
{
    map<string, ManagedObjectSchemaInfo *>::const_iterator iter = m_schemaInfoByNameMap.begin();

    for(;iter!=m_schemaInfoByNameMap.end ();++iter) 
    {
      ManagedObjectSchemaInfo * pManagedObjectSchemaInfo = iter->second;
      pManagedObjectSchemaInfo->populateRelationReferencePointers(this);
    }

    displayRelationshipReferenceCountInfo ();

}

void ManagedObjectSchemaInfoRepository::displayRelationshipReferenceCountInfo ()
{

    map<string, ManagedObjectSchemaInfo *>::const_iterator iter = m_schemaInfoByNameMap.begin();
    for(;iter!=m_schemaInfoByNameMap.end ();++iter) 
    {
      ManagedObjectSchemaInfo * pManagedObjectSchemaInfo = iter->second;
      pManagedObjectSchemaInfo->displayRelationshipReferenceCountInfo();
    }

}

ModifiedManagedObjectSchemaDifference* ManagedObjectSchemaInfoRepository::getModifiedManagedObjectSchemaDifference (const string& moName)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getModifiedManagedObjectSchemaDifference: called for [" + moName + "]");
    map<string, ModifiedManagedObjectSchemaDifference>::iterator iter = m_modifiedTablesSchemaDifference.find (moName);

    if (m_modifiedTablesSchemaDifference.end() == iter)
    {
        trace (TRACE_LEVEL_FATAL, "ManagedObjectSchemaInfoRepository::getModifiedManagedObjectSchemaDifference: could not find schemaDifference object for mo [" + moName + "]");
        prismAssert (false, __FILE__, __LINE__);
    }

    return &(iter->second);  
}
        
void ManagedObjectSchemaInfoRepository::addModifiedTableToSchemaDifference  (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo, ManagedObjectSchemaInfo *pRhsManagedObjectSchemaInfo, const FieldSchemaDifferences *differences, const RelationFieldSchemaDifferences *relationFieldDiff)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::addModifiedTableToSchemaDifference  : Entering ");
    
    ModifiedManagedObjectSchemaDifference modifiedManagedObjectSchemaDifference (pManagedObjectSchemaInfo->getName ());
    
    unsigned int i = 0;

    vector <pair <string, string> > fieldAdditions = differences->getAdditions (); 
    
    for (i = 0; i < fieldAdditions.size (); i++)
    {
        modifiedManagedObjectSchemaDifference.addAddedFieldInfo (fieldAdditions[i]);
    }
    
    vector <pair <string, string> > fieldRemovals = differences->getDeletions (); 
    
    for (i = 0; i < fieldRemovals.size (); i++)
    {
        modifiedManagedObjectSchemaDifference.addRemovedFieldInfo (fieldRemovals[i]);
    }
    
    vector <pair <string, string> > fieldChanges = differences->getChanges (); 
    
    for (i = 0; i < fieldChanges.size (); i++)
    {
        modifiedManagedObjectSchemaDifference.addChangedFieldInfo (fieldChanges[i]);
    }
    
    if (relationFieldDiff->hasDifferences ())
    {
        vector <RelationshipInfoFromSchemaDifference> relationFieldAdditions = relationFieldDiff->getAdditions (); 
        for (i = 0; i < relationFieldAdditions.size (); i++)
        {
            modifiedManagedObjectSchemaDifference.addAddedRelations (relationFieldAdditions[i]);
        }
        
        vector <RelationshipInfoFromSchemaDifference> relationFieldDeletions = relationFieldDiff->getDeletions (); 
        for (i = 0; i < relationFieldDeletions.size (); i++)
        {
            modifiedManagedObjectSchemaDifference.addRemovedRelations (relationFieldDeletions[i]);
        }
        
        vector <RelationshipInfoFromSchemaDifference> relationFieldChanges = relationFieldDiff->getChanges (); 
        for (i = 0; i < relationFieldChanges.size (); i++)
        {
            modifiedManagedObjectSchemaDifference.addChangedRelations (relationFieldChanges[i]);
        }
    }
    
    m_modifiedTablesSchemaDifference.insert ( pair<string, ModifiedManagedObjectSchemaDifference>(pManagedObjectSchemaInfo->getName (), modifiedManagedObjectSchemaDifference) );
}
     
void ManagedObjectSchemaInfoRepository::addAddedTableToSchemaDifference (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::addAddedTableToSchemaDifference : Entering ");

    AddedRemovedManagedObjectSchemaDifference addedRemovedManagedObjectSchemaDifference (pManagedObjectSchemaInfo->getName ());

    map<string, string> relationFieldsTypeMap = pManagedObjectSchemaInfo->getRelationFields (); 

    map<string, string>::iterator relationFieldsTypeMapIterator;
    
    for (relationFieldsTypeMapIterator = relationFieldsTypeMap.begin (); relationFieldsTypeMapIterator != relationFieldsTypeMap.end (); relationFieldsTypeMapIterator++)
    {
        RelationshipInfoFromSchemaDifference relation ((*relationFieldsTypeMapIterator).first, (*relationFieldsTypeMapIterator).second);
        addedRemovedManagedObjectSchemaDifference.addRelations (relation);
    }

    m_addedTablesSchemaDifference.insert ( pair<string, AddedRemovedManagedObjectSchemaDifference>(pManagedObjectSchemaInfo->getName (), addedRemovedManagedObjectSchemaDifference) );
}       

bool ManagedObjectSchemaInfoRepository::isTableRemovedFromSchema (const string& managedObjectName)
{
    map<string, AddedRemovedManagedObjectSchemaDifference>::iterator iter = m_removedTablesSchemaDifference.find (managedObjectName);

    return (m_removedTablesSchemaDifference.end () != iter)?(true):(false);
}

void ManagedObjectSchemaInfoRepository::addRemovedTableToSchemaDifference   (ManagedObjectSchemaInfo *pManagedObjectSchemaInfo)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::addRemovedTableToSchemaDifference : Entering ");

    AddedRemovedManagedObjectSchemaDifference addedRemovedManagedObjectSchemaDifference (pManagedObjectSchemaInfo->getName ());

    map<string, string> relationFieldsTypeMap = pManagedObjectSchemaInfo->getRelationFields (); 

    map<string, string>::iterator relationFieldsTypeMapIterator;
    
    for (relationFieldsTypeMapIterator = relationFieldsTypeMap.begin (); relationFieldsTypeMapIterator != relationFieldsTypeMap.end (); relationFieldsTypeMapIterator++)
    {
        RelationshipInfoFromSchemaDifference relation ((*relationFieldsTypeMapIterator).first, (*relationFieldsTypeMapIterator).second);
        addedRemovedManagedObjectSchemaDifference.addRelations (relation);
    }

    m_removedTablesSchemaDifference.insert ( pair<string, AddedRemovedManagedObjectSchemaDifference>(pManagedObjectSchemaInfo->getName (), addedRemovedManagedObjectSchemaDifference) );
}

void ManagedObjectSchemaInfoRepository::printDifferenceContentsForDebugging ()
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::printDifferenceContentsForDebugging : Printing all Differences");
    
    map<string, ModifiedManagedObjectSchemaDifference>::iterator mIterator;
    
    tracePrintf (TRACE_LEVEL_INFO, false, false, "Number of Modified Managed Objects : %d\n", m_modifiedTablesSchemaDifference.size ());
    
    for (mIterator=m_modifiedTablesSchemaDifference.begin();mIterator!=m_modifiedTablesSchemaDifference.end();mIterator++)
    {
        (mIterator->second).printContentsForDebugging ();
    }
    
    map<string, AddedRemovedManagedObjectSchemaDifference>::iterator addRemoveIterator;

    tracePrintf (TRACE_LEVEL_INFO, false, false, "Number of Added Managed Objects : %d\n", m_addedTablesSchemaDifference.size ());
    
    for (addRemoveIterator=m_addedTablesSchemaDifference.begin();addRemoveIterator!=m_addedTablesSchemaDifference.end();addRemoveIterator++)
    {
        (addRemoveIterator->second).printContentsForDebugging ();
    }
    
    tracePrintf (TRACE_LEVEL_INFO, false, false, "Number of Removed Managed Objects : %d\n", m_removedTablesSchemaDifference.size ());
    
    for (addRemoveIterator=m_removedTablesSchemaDifference.begin();addRemoveIterator!=m_removedTablesSchemaDifference.end();addRemoveIterator++)
    {
        (addRemoveIterator->second).printContentsForDebugging ();
    }
}


ResourceId ManagedObjectSchemaInfoRepository::getSqlForInsertDrvInstancesEntriesForTable (const string &tableName, string &sql)
{
    trace (TRACE_LEVEL_DEBUG, "ManagedObjectSchemaInfoRepository::getSqlForInsertDrvInstancesEntriesForTable: entering..\n");

    string drvtInstancesTable   = tableName + string("DerivationsInstances");
    string waveSchema           = OrmRepository::getWaveCurrentSchema ();
    bool   tableHasEntries      = false;
            
    DatabaseStandaloneTransaction dbStandaloneTransaction;

    tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfoRepository::getSqlForInsertDrvInstancesEntriesForTable: checking if table %s exists in db", drvtInstancesTable.c_str());
    if (dbStandaloneTransaction.checkIfTableHasEntriesInDb (drvtInstancesTable, waveSchema, tableHasEntries))
    {
        if (tableHasEntries)
        {
            tracePrintf (TRACE_LEVEL_FATAL, true, false, "ManagedObjectSchemaInfoRepository::getSqlForInsertDrvInstancesEntriesForTable: table [%s] has non zero entries already.", drvtInstancesTable.c_str()); 
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "getSqlToPopulateAllDrvtionInstancesTables: table doesn't exist in db for %s", tableName.c_str());

            sql = "\n" + OrmRepository::getSqlToDeleteDerivationInstancesTable (tableName, waveSchema);

            vector <UI64> objectInstanceIds;
            dbStandaloneTransaction.getObjectInstanceIdsForTable (tableName, objectInstanceIds);
            tracePrintf (TRACE_LEVEL_INFO, true, false, "getSqlToPopulateAllDrvtionInstancesTables: objectInstanceIds.size () = %u", objectInstanceIds.size());
            for (vector<UI64>::const_iterator iIter = objectInstanceIds.begin (); iIter!=objectInstanceIds.end (); ++iIter)
            {
                string instanceId     = string ("") + *iIter;
                string insertSql      = "";
                insertSql = OrmRepository::getSqlToInsertEntriesIntoDerivationsTable (tableName, waveSchema, instanceId);
                sql += "\n" + insertSql;
            }
            tracePrintf (TRACE_LEVEL_INFO, true, false, "getSqlToPopulateAllDrvtionInstancesTables: sql in WAVE_OBJECT_MANAGER_UPDATE_TABLE : %s\n", sql.c_str());
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string("ManagedObjectSchemaInfoRepository::getSqlToPopulateAllDrvtionInstancesTables : failed to check if derivationInstancesTable exists in db") + tableName);
        prismAssert (false, __FILE__, __LINE__);
    }

    return WAVE_MESSAGE_SUCCESS;
}

void ManagedObjectSchemaInfoRepository::printMORepositoryForDebugging (string &result)
{
     result += "\r\n";
     result += string (" Index\t\tTableId\t\tparentTableId\t#relationsTo\t#relationsFrom\tMO Name\r\n");
     UI32 index = 0;
    
    for (map<string, ManagedObjectSchemaInfo *>::iterator iter = m_schemaInfoByNameMap.begin (); iter != m_schemaInfoByNameMap.end (); iter++)
    {   
        index++;
        ManagedObjectSchemaInfo *moSchemaInfo = (iter->second);

        result += string (" ") + index 
                + "\t\t" + moSchemaInfo->getTableId ()
                + "\t\t" + moSchemaInfo->getParentTableId ()
                + "\t\t" + moSchemaInfo->getCountOfRelationshipReferecePointers ()
                + "\t\t" + moSchemaInfo->getReferenceCount()
                + "\t\t" + (moSchemaInfo->getName ()).c_str() + "\r\n";
    }
    
}

void ManagedObjectSchemaInfoRepository::getAllInstanceIds (const string tableName, string &result)
{
     
    vector<UI64> objectInstanceIds; 
    DatabaseStandaloneTransaction dbStandaloneTransaction;                                                                                                  
    bool status = dbStandaloneTransaction.getObjectInstanceIdsForTable (tableName, objectInstanceIds);

    string instanceIds = "";                                                                                                                                                                                                                                                                                         
    if (status)   
    {
        for (vector<UI64>::iterator iter1 = objectInstanceIds.begin (); iter1 != objectInstanceIds.end (); iter1++)
        {
            instanceIds = instanceIds + "|" + (*iter1);
        }
    }
    result = instanceIds;
}

ResourceId ManagedObjectSchemaInfoRepository::printMoRepositoryDebugShellWrapper (UI32 argc, vector<string> argv)
{               

    OrmRepository *orm                                = OrmRepository::getInstance ();
    ManagedObjectSchemaInfoRepository &moRepository   = orm->getMOSchemaInfoRepository ();

    string results = "";
    moRepository.printMORepositoryForDebugging (results);
    trace (TRACE_LEVEL_INFO, "results:\n" + results);
  
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId ManagedObjectSchemaInfoRepository::removeSchemaObjectDebugShellWrapper (UI32 argc, vector<string> argv)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    UI32 numberOfArguments      = argv.size ();
    if (numberOfArguments <= 1 || numberOfArguments > 2)
    {   
        trace (TRACE_LEVEL_WARN, "help : removeschemaobject <schemaMOName>");
        return (status);
    }

    string schemaMOName         = argv[1];

    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::removeSchemaObjectDebugShellWrapper: called for " + schemaMOName);

    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);
                                                                        
    ManagedObjectSchemaInfoRepository &moRepository  = pOrmRepository->getMOSchemaInfoRepository ();
    ManagedObjectSchemaInfo *pSchemaMO               = moRepository.findSchemaInfoObject (schemaMOName);

    if (NULL == pSchemaMO)
    {   
        trace (TRACE_LEVEL_ERROR, "OrmRepository::removeSchemaObjectDebugShellWrapper: " + schemaMOName + " does not exists in repository");
        return (status);
    }
    
    string schemaUpdateTempString;
    status = moRepository.removeSchemaInfoObjectAndGetSql (pSchemaMO , schemaUpdateTempString, schemaUpdateTempString , moRepository);
    if (FRAMEWORK_SUCCESS != status)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else 
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    return (status);
}

ResourceId  ManagedObjectSchemaInfoRepository::getSqlToRemoveInstanceIdsFromDerivationInstancesTables   (string& schemaUpdateSqlForCurrentSchema, string& schemaUpdateSqlForStartSchema, const ManagedObjectSchemaInfoRepository &dbRepository, const string &tableName, const UI32 tableId, vector<UI64> &instanceIds)
{
    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "getSqlToRemoveInstanceIdsFromDerivationInstancesTables: called for MO %s", tableName.c_str());

    ManagedObjectSchemaInfo *schemaInfoObject = dbRepository.findSchemaInfoObject (tableName);
    prismAssert (NULL != schemaInfoObject, __FILE__, __LINE__);

    UI32 parent = schemaInfoObject->getParentTableId ();

    if (0 != parent)
    {
        ManagedObjectSchemaInfo *parentInfoObject = dbRepository.findSchemaInfoObject (parent);
        string                  parentClassName   = parentInfoObject->getName ();

        bool hasDerivationInstanceTable = dbRepository.checkIfTableHasDerivationInstanceTable (parentInfoObject);
        if (hasDerivationInstanceTable)
        {
            map<string, AddedRemovedManagedObjectSchemaDifference>::iterator iter = m_removedTablesSchemaDifference.find (parentClassName);
            if (m_removedTablesSchemaDifference.end () != iter)
            {
                tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfoRepository::getSqlToRemoveInstanceIdsFromDerivationInstancesTables : [%s] table is already a member for removal. No need to delete the instances entries.", parentClassName.c_str());
            }
            else
            {
                string tmpSqlCurrent = generateSqlToDeleteDerivationInstanceIdsFromGivenTable (tableId, instanceIds, parentClassName, OrmRepository::getWaveCurrentSchema ());
                string tmpSqlStart   = generateSqlToDeleteDerivationInstanceIdsFromGivenTable (tableId, instanceIds, parentClassName, OrmRepository::getWaveStartSchema ());
                schemaUpdateSqlForCurrentSchema += "\n" + tmpSqlCurrent;
                schemaUpdateSqlForStartSchema   += "\n" + tmpSqlStart;
            }
        }

        getSqlToRemoveInstanceIdsFromDerivationInstancesTables (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, dbRepository, parentClassName, tableId, instanceIds);

    } 
    else 
    {
        tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfoRepository::getSqlToRemoveInstanceIdsFromDerivationInstancesTables: [%s] is most base class. Hence, returning", tableName.c_str());
    }

    return (FRAMEWORK_SUCCESS); 
}

bool ManagedObjectSchemaInfoRepository::checkIfTableHasDerivationInstanceTable (ManagedObjectSchemaInfo* pInfoObject) const
{
    bool hasDerivationInstancesTable = false;

    if (0 != pInfoObject->getReferenceCount ())
    {
        hasDerivationInstancesTable = true;
    }
    else if (0 != pInfoObject->getCountOfRelationshipReferecePointers ())
    {
        map<string, string> relationFieldsTypeMap = pInfoObject->getRelationFields ();

        for (map<string, string>::iterator tmpIter = relationFieldsTypeMap.begin (); tmpIter != relationFieldsTypeMap.end (); tmpIter++)
        {   
            RelationshipInfoFromSchemaDifference relation ((*tmpIter).first, (*tmpIter).second);
            if (ORM_RELATION_TYPE_ONE_TO_MANY == relation.getRelationType ())
            {
                hasDerivationInstancesTable = true;
                break;
            }
        }
    }

    return (hasDerivationInstancesTable);
}

string ManagedObjectSchemaInfoRepository::generateSqlToDeleteDerivationInstanceIdsFromGivenTable (const UI32 tableId, const vector<UI64>& instanceIds, const string &table, const string &waveSchema)
{
    string sqlToDeleteInstanceIdsFromATable = "\n";
    sqlToDeleteInstanceIdsFromATable += string ("DELETE FROM ") + waveSchema + string (".") + table + string ("Derivations WHERE objectidclassid = ") + tableId + string (";");
    for (vector<UI64>::const_iterator iter = instanceIds.begin(); iter != instanceIds.end (); ++iter)
    {   
        sqlToDeleteInstanceIdsFromATable += string ("\n") + string ("DELETE FROM ") + waveSchema + string (".") + table + string ("DerivationsInstances WHERE objectidinstanceid = ") + *iter + string(";");
    }
    return (sqlToDeleteInstanceIdsFromATable);
}


void ManagedObjectSchemaInfoRepository::addSchemaInfoObjectFromComputedDifferences (ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
      map<string, AddedRemovedManagedObjectSchemaDifference>::iterator addRemoveIterator;

      tracePrintf (TRACE_LEVEL_INFO, false, false, "Number of Added Managed Objects : %d\n", m_addedTablesSchemaDifference.size ());

      for (addRemoveIterator=m_addedTablesSchemaDifference.begin();addRemoveIterator!=m_addedTablesSchemaDifference.end();addRemoveIterator++)
      {
	  string addedManagedObjectName = addRemoveIterator->first;
	  OrmTable* pOrmTable = OrmRepository::getTableByName(addedManagedObjectName);

	  UI32 parentTableId = (pOrmTable->getPParentTable()) ? (pOrmTable->getPParentTable ()->getTableId ()) : 0;
	  string strFieldNames, strFieldTypes;
	  const vector<string>& fields = pOrmTable->getFieldNamesForUpgrade ();
	  const vector<string>& types  = pOrmTable->getFieldTypesForUpgrade ();

	  ManagedObjectSchemaInfo *pSchemaInfoObj = new CompositeManagedObjectSchemaInfo (pOrmTable->getName (), pOrmTable->getTableId (), parentTableId);

	  prismAssert (fields.size () == types.size (), __FILE__, __LINE__);
	  for (size_t idx = 0; idx < fields.size (); ++idx)
	  {
	      pSchemaInfoObj->addFieldTypeTuple (fields[idx], types[idx]);
	  }


	  dbSchemaInfoRepository.addSchemaInfoObject(pSchemaInfoObj);

	  tracePrintf (TRACE_LEVEL_INFO, false, false, "Table : %s has been added to the new schema\n",addedManagedObjectName.c_str() );
      }


}

void ManagedObjectSchemaInfoRepository::generateSqlsForModifiedRelationShipsInModifiedObjects (string & updateCurrentSchemaSql, string & updateStartupSchemaSql)
{
    map<string, ModifiedManagedObjectSchemaDifference>::iterator mIterator;
    
    tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfoRepository::generateSqlsForModifiedRelationShipsInModifiedObjects: Number of Modified Managed Objects : %d\n", m_modifiedTablesSchemaDifference.size ());
    
    for (mIterator = m_modifiedTablesSchemaDifference.begin(); mIterator != m_modifiedTablesSchemaDifference.end(); mIterator++)
    {
        //(mIterator->second).printContentsForDebugging ();

        string modifiedManagedObjectName = mIterator->first;

        //Iterate through the vector of all changed relationships 
        vector<RelationshipInfoFromSchemaDifference> changedRelationsVector = (mIterator->second).getChangedRelations ();
        vector<RelationshipInfoFromSchemaDifference>::const_iterator changedRelationsIterator = changedRelationsVector.begin();

	    //---generate Alter statements to add NOT NULL constraints for 1:1 relationships 
        for(;changedRelationsIterator != changedRelationsVector.end(); ++changedRelationsIterator) 
        {
            if (changedRelationsIterator->isCanBeEmptyChanged ())
            {
                if (changedRelationsIterator->getCanBeEmpty ())
                {
                    OrmTable* pOrmTable = OrmRepository::getTableByName (modifiedManagedObjectName);
                    updateCurrentSchemaSql += pOrmTable->getAlterSqlToDropNotNullForOneToOneRelationship (OrmRepository::getWaveCurrentSchema() ,changedRelationsIterator->getRelationName());
                    updateStartupSchemaSql += pOrmTable->getAlterSqlToDropNotNullForOneToOneRelationship (OrmRepository::getWaveStartSchema(), changedRelationsIterator->getRelationName());
                }
                else
                {
                    /* If canBeEmpty is false, we add NOT NULL after calling Upgrade function */
                }
            }

            if (changedRelationsIterator->isDisableValidationChanged ())
            {
                if (changedRelationsIterator->getDisableValidations ())
                {
                    OrmTable* pOrmTable = OrmRepository::getTableByName (modifiedManagedObjectName);
                    updateCurrentSchemaSql += pOrmTable->getAlterSqlToDisableValidationForOneToManyRelationship (OrmRepository::getWaveCurrentSchema() ,changedRelationsIterator->getRelationName());
                    updateStartupSchemaSql += pOrmTable->getAlterSqlToDisableValidationForOneToManyRelationship (OrmRepository::getWaveStartSchema(), changedRelationsIterator->getRelationName());
                }
                else
                {
                    /* If Validations are enabled, we add Foriegn Key Constraints after calling Upgrade function */
                }
            }
        }
    }
}

void ManagedObjectSchemaInfoRepository::generateSqlsForAddedRelationShipsInModifiedObjects (ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository,string & sqlForDerivationTablesForCurrentSchema,string & sqlForDerivationTablesForStartSchema, string & alterSqlForOneToOneRelationShipForCurrentSchema, string &  alterSqlForOneToOneRelationShipForStartSchema,string & sqlForAuxilliaryTablesForCurrentSchema, string & sqlForAuxilliaryTablesForStartSchema)
{
    map<string, ModifiedManagedObjectSchemaDifference>::iterator mIterator;
    
    tracePrintf (TRACE_LEVEL_INFO, false, false, "Number of Modified Managed Objects : %d\n", m_modifiedTablesSchemaDifference.size ());
    
    for (mIterator=m_modifiedTablesSchemaDifference.begin();mIterator!=m_modifiedTablesSchemaDifference.end();++mIterator)
    {

        string modifiedManagedObjectName = mIterator->first;
        ManagedObjectSchemaInfo* pFromManagedObjectSchemaInfo = dbSchemaInfoRepository.findSchemaInfoObject (modifiedManagedObjectName);

        //Iterate through the vector of all added relationships 
        vector<RelationshipInfoFromSchemaDifference> addedRelationsVector = (mIterator->second).getAddedRelations ();

        vector<RelationshipInfoFromSchemaDifference>::const_iterator relationsIterator = addedRelationsVector.begin();

	//---generate Alter statements  for creating all 1:1 relationships added to modified objects
        for(;relationsIterator != addedRelationsVector.end(); ++relationsIterator) 
         {
                if(ORM_RELATION_TYPE_ONE_TO_ONE == relationsIterator->getRelationType ()) 
                 {
                    OrmTable* pOrmTable = OrmRepository::getTableByName (modifiedManagedObjectName);
                    alterSqlForOneToOneRelationShipForCurrentSchema+= pOrmTable->getAlterSqlForOneToOneRelationship (OrmRepository::getWaveCurrentSchema() ,relationsIterator->getRelationName());
		    alterSqlForOneToOneRelationShipForStartSchema+= pOrmTable->getAlterSqlForOneToOneRelationship (OrmRepository::getWaveStartSchema(), relationsIterator->getRelationName());

                 }
         }
        

       //populate the referenceCount pointer corresponding to each relationship over all modified object
        prismAssert (NULL!= pFromManagedObjectSchemaInfo,__FILE__, __LINE__);
        pFromManagedObjectSchemaInfo->processRelationsAddedInNewSchema (addedRelationsVector,dbSchemaInfoRepository,sqlForDerivationTablesForCurrentSchema,sqlForDerivationTablesForStartSchema,sqlForAuxilliaryTablesForCurrentSchema, sqlForAuxilliaryTablesForStartSchema);
    }

}

void ManagedObjectSchemaInfoRepository::generateSqlsForRelationShipsInAddedObjects (ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository,string & sqlForDerivationTablesForCurrentSchema,string & sqlForDerivationTablesForStartSchema,string & sqlForAuxilliaryTablesForCurrentSchema, string & sqlForAuxilliaryTablesForStartSchema)
{


    tracePrintf (TRACE_LEVEL_INFO, false, false, "Number of Added Managed Objects : %d\n", m_addedTablesSchemaDifference.size ());
    map<string, AddedRemovedManagedObjectSchemaDifference>::iterator addRemoveIterator;
    for (addRemoveIterator=m_addedTablesSchemaDifference.begin();addRemoveIterator!=m_addedTablesSchemaDifference.end();++addRemoveIterator)
    {
        string addedManagedObjectName = addRemoveIterator->first;

        ManagedObjectSchemaInfo* pFromManagedObjectSchemaInfo = dbSchemaInfoRepository.findSchemaInfoObject (addedManagedObjectName);

        prismAssert (NULL!= pFromManagedObjectSchemaInfo,__FILE__, __LINE__);

        vector<RelationshipInfoFromSchemaDifference> newRelationsInAddedMOs = (addRemoveIterator->second).getRelations ();
        if(newRelationsInAddedMOs.size ()>0)
         {
            pFromManagedObjectSchemaInfo->processRelationsAddedInNewSchema (newRelationsInAddedMOs, dbSchemaInfoRepository,sqlForDerivationTablesForCurrentSchema,sqlForDerivationTablesForStartSchema,sqlForAuxilliaryTablesForCurrentSchema, sqlForAuxilliaryTablesForStartSchema);
         }
         else
        {
            trace(TRACE_LEVEL_INFO,"ManagedObjectSchemaInfoRepository::processAdditionOfNewMOsAndRelationshipAddition: No New Relation found in the MO");
        }
    }

}


void  ManagedObjectSchemaInfoRepository::generateSqlForCreationOfAddedTables (string & sqlForCurrentSchema, string & sqlForStartSchema)
{
    ManagedObjectSchemaInfoRepository   dbSchemaInfoRepository;                 // this need not be filled.
    vector<string>                      addedMoInHierarchialOrder;
 
    ResourceId status = setupHierarchialOrder (addedMoInHierarchialOrder, true, dbSchemaInfoRepository);
    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::generateSqlForCreationOfAddedTables: failed to setupHierarchialOrder for added MOs.");
        return;
    }
    
/*    map<string, AddedRemovedManagedObjectSchemaDifference>::iterator addRemoveIterator;
    for (addRemoveIterator=m_addedTablesSchemaDifference.begin();addRemoveIterator!=m_addedTablesSchemaDifference.end();addRemoveIterator++)*/

    for (vector<string>::iterator addRemoveIterator = addedMoInHierarchialOrder.begin (); addRemoveIterator != addedMoInHierarchialOrder.end (); ++addRemoveIterator) 
    {
        //string addedManagedObjectName = addRemoveIterator->first;
        string addedManagedObjectName = *addRemoveIterator;
        OrmTable* pOrmTable = OrmRepository::getTableByName(addedManagedObjectName);        
        string sqlForCreateTableInCurrentSchema = pOrmTable->getSqlForCreate(OrmRepository::getWaveCurrentSchema());
	    string sqlForCreateTableInStartSchema = pOrmTable->getSqlForCreate(OrmRepository::getWaveStartSchema());
        tracePrintf (TRACE_LEVEL_INFO, false, false, "Sql to add table  %s:sqlForCreateTableInCurrentSchema: %s , sqlForCreateTableInStartSchema %s\n",addedManagedObjectName.c_str(), sqlForCreateTableInCurrentSchema.c_str(), sqlForCreateTableInStartSchema.c_str());

	    sqlForCurrentSchema+=sqlForCreateTableInCurrentSchema;
	    sqlForStartSchema+=sqlForCreateTableInStartSchema;
    }

}

void ManagedObjectSchemaInfoRepository::generateSqlForRepopulatingEntiresOfDerivationsTable (string & sqlToRepopulateDerivationsTableInCurrentSchema, string & sqlToRepopulateDerivationsTableInStartSchema)
{
    //   --- Recompute the sql for populating the entries of each derivations table 
    map<string, ManagedObjectSchemaInfo *>::const_iterator      schemaInfoIterator = m_schemaInfoByNameMap.begin ();

    for (;schemaInfoIterator!=m_schemaInfoByNameMap.end();++schemaInfoIterator)
    {

        ManagedObjectSchemaInfo* pManagedObjectSchemaInfo = schemaInfoIterator->second;

        prismAssert (NULL!= pManagedObjectSchemaInfo,__FILE__, __LINE__);

        CompositeManagedObjectSchemaInfo* pCompositeManagedObjectSchemaInfo = dynamic_cast<CompositeManagedObjectSchemaInfo *> (pManagedObjectSchemaInfo);
        prismAssert (NULL != pManagedObjectSchemaInfo, __FILE__, __LINE__);

        string sqlInCurrentSchema;
        string sqlInStartSchema;

        pCompositeManagedObjectSchemaInfo->generateSqlToRepopulateDerivationsTable(OrmRepository::getWaveCurrentSchema(),sqlInCurrentSchema);
        pCompositeManagedObjectSchemaInfo->generateSqlToRepopulateDerivationsTable(OrmRepository::getWaveStartSchema(),sqlInStartSchema);       
        sqlToRepopulateDerivationsTableInCurrentSchema+=sqlInCurrentSchema;
        sqlToRepopulateDerivationsTableInStartSchema+=sqlInStartSchema;  
    }  


}

void ManagedObjectSchemaInfoRepository::processAdditionOfNewMOsAndRelationships (string & updateCurrentSchemaSql,string  & updateStartupSchemaSql,ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
     //Iterate through list of ManagedObjects to be added: For each MO to be added
     //    ---Create CompositeMO 
     //    ---Add it to the tree    
        
    addSchemaInfoObjectFromComputedDifferences (dbSchemaInfoRepository);
          
    string sqlForDerivationTablesForCurrentSchema,sqlForDerivationTablesForStartSchema ;
    string alterSqlForOneToOneRelationShipForCurrentSchema, alterSqlForOneToOneRelationShipForStartSchema;
    string sqlForAuxilliaryTablesForCurrentSchema,sqlForAuxilliaryTablesForStartSchema ;

    generateSqlsForAddedRelationShipsInModifiedObjects (dbSchemaInfoRepository,sqlForDerivationTablesForCurrentSchema,sqlForDerivationTablesForStartSchema,alterSqlForOneToOneRelationShipForCurrentSchema,alterSqlForOneToOneRelationShipForStartSchema,sqlForAuxilliaryTablesForCurrentSchema,sqlForAuxilliaryTablesForStartSchema);
    

    generateSqlsForRelationShipsInAddedObjects (dbSchemaInfoRepository,sqlForDerivationTablesForCurrentSchema,sqlForDerivationTablesForStartSchema,sqlForAuxilliaryTablesForCurrentSchema,sqlForAuxilliaryTablesForStartSchema);

    updateCurrentSchemaSql+=sqlForDerivationTablesForCurrentSchema + alterSqlForOneToOneRelationShipForCurrentSchema;
    updateStartupSchemaSql+=sqlForDerivationTablesForStartSchema + alterSqlForOneToOneRelationShipForStartSchema;

    string sqlForCreateTableInCurrentSchema; 
    string sqlForCreateTableInStartSchema; 
    //Sql for table creation
    generateSqlForCreationOfAddedTables (sqlForCreateTableInCurrentSchema,sqlForCreateTableInStartSchema);

    //Update the Sql with sql for  New Table Creation
    updateCurrentSchemaSql+= sqlForCreateTableInCurrentSchema;
    updateStartupSchemaSql+= sqlForCreateTableInStartSchema;   
    //Sql For Auxilliary tables
    updateCurrentSchemaSql+= sqlForAuxilliaryTablesForCurrentSchema;
    updateStartupSchemaSql+= sqlForAuxilliaryTablesForStartSchema;    


    string sqlToRepopulateDerivationsTableInCurrentSchema, sqlToRepopulateDerivationsTableInStartSchema;
    generateSqlForRepopulatingEntiresOfDerivationsTable (sqlToRepopulateDerivationsTableInCurrentSchema,sqlToRepopulateDerivationsTableInStartSchema);
    updateCurrentSchemaSql+= sqlToRepopulateDerivationsTableInCurrentSchema;
    updateStartupSchemaSql+= sqlToRepopulateDerivationsTableInStartSchema; 


  //   --- Iterate through the vector of all the relationships that are being added.
  // 
  //       Examine the from and to end of each 1:n and to end of each 1:1 relationship. If the object at the examined end is a modified 
  //       object, delete all entries of its derivations instances table and recompute the sql for inserting  its derivatiosn instances entries.


}

ResourceId ManagedObjectSchemaInfoRepository::getSqlToDropUserDefinedKeyUniqueConstraint (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    map<string, ManagedObjectSchemaInfo *>::iterator    iter; 
    string                                              sqlToDropUserDefinedKeyCombinationConstraintForCurrentSchema;
    string                                              sqlToDropUserDefinedKeyCombinationConstraintForStartSchema;

    for (iter = m_schemaInfoByNameMap.begin (); iter != m_schemaInfoByNameMap.end (); iter++)
    {
        //ManagedObjectSchemaInfo* schemaInfoObject = iter->second;

        //schemaInfoObject->getSqlToDropUserDefinedKeyUniqueConstraint (sqlToDropUserDefinedKeyCombinationConstraintForCurrentSchema, sqlToDropUserDefinedKeyCombinationConstraintForStartSchema);

        string tableName = iter->first;

        OrmTable *pOrmTable = OrmRepository::getTableByName (tableName);

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        pOrmTable->getSqlToDropUserDefinedKeyUniqueConstraint (sqlToDropUserDefinedKeyCombinationConstraintForCurrentSchema, sqlToDropUserDefinedKeyCombinationConstraintForStartSchema);
    }

    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToDropUserDefinedKeyUniqueConstraint: sqlToDropUserDefinedKeyCombinationConstraintForCurrentSchema - " + sqlToDropUserDefinedKeyCombinationConstraintForCurrentSchema);

    schemaUpdateSqlForCurrentSchema+=sqlToDropUserDefinedKeyCombinationConstraintForCurrentSchema;
    schemaUpdateSqlForStartSchema  +=sqlToDropUserDefinedKeyCombinationConstraintForStartSchema;

    return FRAMEWORK_SUCCESS;
}

void ManagedObjectSchemaInfoRepository::getSqlToAddUserDefinedKeyUniqueConstraint (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    map<string, ManagedObjectSchemaInfo *>::iterator    iter; 
    string                                              sqlToAddUserDefinedKeyCombinationConstraintForCurrentSchema;
    string                                              sqlToAddUserDefinedKeyCombinationConstraintForStartupSchema;

    for (iter = m_schemaInfoByNameMap.begin (); iter != m_schemaInfoByNameMap.end (); iter++)    
    {    
        /*ManagedObjectSchemaInfo* schemaInfoObject = iter->second;

        prismAssert (NULL != schemaInfoObject, __FILE__, __LINE__);

        schemaInfoObject->getSqlToAddUserDefinedKeyUniqueConstraint (sqlToAddUserDefinedKeyCombinationConstraintForCurrentSchema, sqlToAddUserDefinedKeyCombinationConstraintForStartupSchema);*/

        string tableName = iter->first;

        OrmTable *pOrmTable = OrmRepository::getTableByName (tableName);

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        pOrmTable->getSqlToAddUserDefinedKeyUniqueConstraint (sqlToAddUserDefinedKeyCombinationConstraintForCurrentSchema, sqlToAddUserDefinedKeyCombinationConstraintForStartupSchema);
    }
 
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToAddUserDefinedKeyUniqueConstraint:sqlToAddUserDefinedKeyCombinationConstraintForCurrentSchema - \n" + sqlToAddUserDefinedKeyCombinationConstraintForCurrentSchema);

    schemaUpdateSqlForCurrentSchema += string("BEGIN;\n") + sqlToAddUserDefinedKeyCombinationConstraintForCurrentSchema + string("COMMIT;\n");
    schemaUpdateSqlForStartSchema   += string("BEGIN;\n") + sqlToAddUserDefinedKeyCombinationConstraintForStartupSchema + string("COMMIT;\n"); 
}

bool ManagedObjectSchemaInfoRepository::getIfUserDefinedKeyCombinationChanged ()
{
    return m_userDefinedKeyCombinationChanged;
}

void ManagedObjectSchemaInfoRepository::getTablesInOrderForUDKCHandling (vector<string>& tablesForUDKCHandlingInOrder, map<string, vector<string> >& compositionsToHandle)
{
    vector<string> tablesWithCompositionParentAndNoCompositionChild;
    vector<string> tablesWithCompositionParentAndCompositionChildren;

    map<string, vector<string> > tablesCompositionChildren;
    map<string, vector<string> > tablesCompositionParents;

    map<string, string>          pushedTables;
    map<string, bool>            pushLaterTable;
    map<string, string>          tablesWithNoUserDefinedKeyCombination;

    tablesForUDKCHandlingInOrder.clear ();

    map<string, ManagedObjectSchemaInfo *>::const_iterator tableIter;

    for (tableIter = m_schemaInfoByNameMap.begin (); tableIter != m_schemaInfoByNameMap.end (); tableIter++)
    {
        string                      tableName               = tableIter->first;
        ManagedObjectSchemaInfo*    pSchemaInfoObject       = tableIter->second;
        bool                        isCompositionChild      = false;
        bool                        hasCompositionChildren  = false;
        vector<string>              compositionChildren;
        vector<string>              compositionParents;

        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getTablesInOrderForUDKCHandling : tableName - " + tableName);
        if (true == (pSchemaInfoObject->getUserDefinedKeyCombinationString()).empty())
        {
            tablesWithNoUserDefinedKeyCombination[tableName] = tableName;
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getTablesInOrderForUDKCHandling : -- has no unique Key defined by User.");
        }
        else
        {
            //1. getCompositionParentsForATable
            pSchemaInfoObject->getCompositionParentsForATable (this, compositionParents);
            isCompositionChild = compositionParents.size ();

            //2. check for all composition children
            pSchemaInfoObject->getCompositionChildrenForATable (this, compositionChildren);
            hasCompositionChildren = compositionChildren.size();

            if (false == isCompositionChild && false == hasCompositionChildren)
            {
                tablesForUDKCHandlingInOrder.push_back (tableName);
                pushedTables[tableName] = tableName;
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedValuesForUpgrade 0 : [" + tableName + "] pushed to the order vector.");
            }
            else if (false == isCompositionChild && true == hasCompositionChildren)
            {
                tablesCompositionChildren[tableName] = compositionChildren;
                tablesForUDKCHandlingInOrder.push_back (tableName);
                pushedTables[tableName] = tableName;
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedValuesForUpgrade 1 : [" + tableName + "] pushed to the order vector.");
            }
            else if (true == isCompositionChild && true == hasCompositionChildren)
            {
                tablesWithCompositionParentAndCompositionChildren.push_back (tableName);
                tablesCompositionParents[tableName]     = compositionParents;
                tablesCompositionChildren[tableName]    = compositionChildren;
            }
            else if (true == isCompositionChild && false == hasCompositionChildren)
            {
                tablesWithCompositionParentAndNoCompositionChild.push_back (tableName);
                tablesCompositionParents[tableName]     = compositionParents;
            }
        }
    }

    // 2. push tables having composition parents and composition children.
    for (UI32 j = 0; j < tablesWithCompositionParentAndCompositionChildren.size (); j++)
    {
        // confirm all parents are already pushed in the vector.
        string          tableName = tablesWithCompositionParentAndCompositionChildren[j];
        vector<string>  parents;
        bool            parentNotPushed = false;

        map<string, vector<string> >::iterator iter = tablesCompositionParents.find (tableName);

        if (tablesCompositionParents.end () != iter)
        {
            parents = iter->second;
        }

        for (UI32 k = 0; k < parents.size (); k++)
        {
            string parentName = parents[k];

            if ((pushedTables.end () == pushedTables.find (parentName)) && (tablesWithNoUserDefinedKeyCombination.end () == tablesWithNoUserDefinedKeyCombination.find (parentName)))
            {
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getTablesInOrderForUDKCHandling: parentTable [" + parentName + "] not yet pushed. Cant push table [" + tableName + "]");
                parentNotPushed = true;
                break;
            }
        }

        if (parentNotPushed)
        {
            pushLaterTable[tableName] = true;
        }
        else
        {
            tablesForUDKCHandlingInOrder.push_back (tableName);
            pushedTables[tableName] = tableName;
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedValuesForUpgrade 2: [" + tableName + "] pushed to the order vector.");
        }
    }

    // 3. push tables which were not visited before their composition parents.
    while (pushLaterTable.size ())
    {
        map<string, bool>::iterator iter;

        for (iter = pushLaterTable.begin (); iter != pushLaterTable.end (); )  //iter++)
        {
            string          tableName = iter->first;
            vector<string>  parents;
            bool            parentNotPushed = false;
    
            map<string,vector<string> >::iterator parentIter = tablesCompositionParents.find (tableName);

            if (tablesCompositionParents.end () != parentIter)
            {   
                parents = parentIter->second;
            }

            for (UI32 k = 0; k < parents.size (); k++)
            {   
                string parentName = parents[k];

                if (pushedTables.end () == pushedTables.find (parentName) && (tablesWithNoUserDefinedKeyCombination.end () == tablesWithNoUserDefinedKeyCombination.find (parentName)))
                {   
                    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getTablesInOrderForUDKCHandling: parentTable [" + parentName + "] not yet pushed. Cant push table [" + tableName + "]");
                    parentNotPushed = true;
                    break;
                }
            }
            
            if (false == parentNotPushed)
            {   
                tablesForUDKCHandlingInOrder.push_back (tableName);
                pushedTables[tableName] = tableName;
                map<string, bool>::iterator tmpIter = iter;
                iter++;
                pushLaterTable.erase (tmpIter);
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedValuesForUpgrade 3: [" + tableName + "] pushed to the order vector.");
            } 
            else
            {
                iter++;
            }
        }
        tracePrintf (TRACE_LEVEL_INFO, true, true, "ManagedObjectSchemaInfoRepository::getTablesInOrderForUDKCHandling: pushLaterTable.size  : %d", pushLaterTable.size ());
    }
        
    compositionsToHandle = tablesCompositionChildren;
}

void ManagedObjectSchemaInfoRepository::getSqlToUpdateUserDefinedKeyCombinationForUpgrade (string& sqlForCurrentSchemaForUDKC, string& sqlForStartupSchemaForUDKC)
{
    vector<string>                  tablesForUDKCHandlingInOrder;
    map<string, vector<string> >    compositionsToHandle;

    // 1. get all tables having user defined key combination and their composition children in correct order.
    getTablesInOrderForUDKCHandling (tablesForUDKCHandlingInOrder, compositionsToHandle);

    // debugging info -
    TraceLevel traceLevel = TRACE_LEVEL_INFO;
    trace (traceLevel, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUserDefinedKeyCombinationForUpgrade: output - ");

    for (UI32 i = 0; i < tablesForUDKCHandlingInOrder.size (); i++)
    {
        tracePrintf (traceLevel, true, true, "i : %d \ttable : %s", i, tablesForUDKCHandlingInOrder[i].c_str());
        map<string, vector<string> >::iterator iter = compositionsToHandle.find (tablesForUDKCHandlingInOrder[i]);

        if (compositionsToHandle.end () != iter)
        {
            vector<string> compositionChildren = iter->second;

            for (UI32 j = 0; j < compositionChildren.size (); j++)
            {
                tracePrintf (traceLevel, true, true, "child : %d, \ttable : %s", j, compositionChildren[j].c_str());
            }
        }
        else
        {
            trace (traceLevel, "has no conposition child.");
        }
    }

    //2. get the sql to update all columns respective to user defined key combination.

    getSqlToUpdateUDKCRelatedColumns (tablesForUDKCHandlingInOrder, compositionsToHandle, sqlForCurrentSchemaForUDKC, sqlForStartupSchemaForUDKC);
}

void ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedColumns (vector<string>& tablesForUDKCHandlingInOrder, map<string, vector<string> >& compositionsToHandle, string& sqlForCurrentSchemaForUDKC, string& sqlForStartupSchemaForUDKC)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedColumns: entering...");

    for (UI32 i = 0; i < tablesForUDKCHandlingInOrder.size (); i++)
    {
        vector<string>  compositionChildrenForATable; 
        string          tableName                       = tablesForUDKCHandlingInOrder[i];

        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedColumns: tableName [" + tableName + "]"); 

        map<string, vector<string> >::const_iterator iter = compositionsToHandle.find (tableName);

        if (compositionsToHandle.end () != iter)
        {
            compositionChildrenForATable = iter->second;
        }    

        OrmTable* pTable = OrmRepository::getTableByName (tableName);

        prismAssert (NULL != pTable, __FILE__, __LINE__);

        pTable->getSqlForATableToUpdateUDKCRelatedColumns (compositionChildrenForATable, sqlForCurrentSchemaForUDKC, sqlForStartupSchemaForUDKC);
    }

    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::getSqlToUpdateUDKCRelatedColumns: sqlForCurrentSchemaForUDKC - \n" + sqlForCurrentSchemaForUDKC);
}

void ManagedObjectSchemaInfoRepository::dropAllDatabaseViewsForUpgrade ()
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::dropAllDatabaseViewsForUpgrade : entering....");

    DatabaseObjectManagerDropAllDBViewsMessage message;
    ResourceId status = WaveObjectManagerToolKit::sendSynchronously (&message);
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == status)
        {
            DatabaseStandaloneTransaction dbStandaloneTransaction;
            ResourceId status1 = dbStandaloneTransaction.dropAllDBViews ();
    
            if (status1 != FRAMEWORK_SUCCESS)
            {
                trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::dropAllDatabaseViewsForUpgrade : Error in dropping ManagedViews uisng StandaloneTransaction.");
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::dropAllDatabaseViewsForUpgrade : successfully deleted all ManagedViews.");
            }
        }
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfoRepository::dropAllDatabaseViewsForUpgrade :: failed to delete views from the database. Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::dropAllDatabaseViewsForUpgrade : successfully deleted all the views from the database.");
        }
    }
}

void ManagedObjectSchemaInfoRepository::createAllDatabaseViewsForUpgrade ()
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::createAllDatabaseViewsForUpgrade: entering...");

    string schemaUpdateSqlForCurrentSchema = "";
    string schemaUpdateSqlForStartSchema   = "";                
    (OrmRepository::getInstance ())->generateSqlToCreateViews (schemaUpdateSqlForCurrentSchema);

    ResourceId status1 = applySqlToDatabase (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);

    trace (TRACE_LEVEL_DEBUG, "ManagedObjectSchemaInfoRepository::createAllDatabaseViewsForUpgrade : sqlToCreateViews: " + schemaUpdateSqlForCurrentSchema);
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfoRepository::createAllDatabaseViewsForUpgrade : status for adding ManagedViews : " + FrameworkToolKit::localize(status1)); 
}
}
