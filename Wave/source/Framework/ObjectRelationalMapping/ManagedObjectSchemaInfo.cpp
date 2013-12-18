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
#include "Framework/ObjectRelationalMapping/RelationReferenceCountPointer.cpp"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfo.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include <algorithm>

namespace WaveNs
{

/**
 * Name:  ManagedObjectSchemaInfo
 * Description: Constructor
 * @param pWaveObjectManager
 */
ManagedObjectSchemaInfo::ManagedObjectSchemaInfo (const string &name, UI32 tableId, UI32 parentTableId)
    :m_name         (name),
    m_tableId       (tableId),
    m_parentTableId (parentTableId),
    m_numberOf1to1RelationFields (0),
    m_numberOf1tonRelationFields (0),
    m_userDefinedKeyCombinationString (""),
    m_derivedFromClassName (""),
    m_isALocalManagedObject (true)
{
}

/**
 * Name: ~ManagedObjectSchemaInfo
 * Description: Destructor
 */
ManagedObjectSchemaInfo::~ManagedObjectSchemaInfo ()
{
}

const string & ManagedObjectSchemaInfo::getName () const
{
    return (m_name);
}

UI32 ManagedObjectSchemaInfo::getTableId () const
{
    return (m_tableId);
}

UI32 ManagedObjectSchemaInfo::getParentTableId () const
{
    return (m_parentTableId);
}

size_t ManagedObjectSchemaInfo::getNumberOfFields () const
{
    return (m_simpleFieldTypeMap.size () + m_relationFieldTypeMap.size ());
}

size_t ManagedObjectSchemaInfo::getNumberOfSimpleFields () const
{
    return (m_simpleFieldTypeMap.size ());
}

size_t ManagedObjectSchemaInfo::getNumberOfRelationFields () const
{
    return (m_relationFieldTypeMap.size ());
}

const map<string, string> & ManagedObjectSchemaInfo::getRelationFields () const
{
    return (m_relationFieldTypeMap);
}

const map<string, string> & ManagedObjectSchemaInfo::getSimpleFields () const
{
    return (m_simpleFieldTypeMap);
}

size_t ManagedObjectSchemaInfo::getNumberOf1to1RelationFields () const
{
    return (m_numberOf1to1RelationFields);
}

size_t ManagedObjectSchemaInfo::getNumberOf1tonRelationFields () const
{
    return (m_numberOf1tonRelationFields);
}

void ManagedObjectSchemaInfo::addFieldTypeTuple (const string &fieldName, const string &fieldType)
{
    map<string,string> *fieldmap = &m_relationFieldTypeMap;

    // Check if a field is relation. Relation have a type like "[a|c|g]-[1|m]-<MO name>"
    // This is a simple test for hyphens in type indicating a field is a relation
    if ((fieldType.length () < 4) || (fieldType[1] != '-' && fieldType[3] != '-'))
    {
        fieldmap = &m_simpleFieldTypeMap;
    }

    // Update 1to1 or 1ton counters
    if ((fieldType[1] == '-' && fieldType[3] == '-'))
    {
        if (fieldType[2] == '1')
        {
            m_numberOf1to1RelationFields++;
        }
        else if (fieldType[2] == 'm')
        {
            //m_numberOf1tonRelationFields++;
        }
    }

    map<string,string>::const_iterator iter = fieldmap->find (fieldName);
    if (iter != fieldmap->end ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::addFieldTypeTuple : Field '" + fieldName + "' already exists in fieldTypeMap for " + getName ());
        prismAssert (false, __FILE__, __LINE__);
    }

    (*fieldmap)[fieldName] = fieldType;
}
        
void ManagedObjectSchemaInfo::addUserDefinedFieldTypeTuple (const string &userDefinedFieldName, const string &userDefinedFieldType)
{
    map<string, string>::const_iterator iter = m_userDefinedKeyCombinationsMap.find (userDefinedFieldName);
        
    if (iter != m_userDefinedKeyCombinationsMap.end ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::addUserDefinedFieldTypeTuple : User Defined Field '" + userDefinedFieldName + "' already exists in userDefinedKeyCombinationsMap for " + getName ());
        prismAssert (false, __FILE__, __LINE__);
    }

    m_userDefinedKeyCombinationsMap[userDefinedFieldName] = userDefinedFieldType;
    m_userDefinedKeyCombinationString += userDefinedFieldName;
}

bool ManagedObjectSchemaInfo::getIsALocalManagedObject () const
{
    return (m_isALocalManagedObject);
}

void ManagedObjectSchemaInfo::setIsALocalManagedObject (const bool isALocalMO)
{
    m_isALocalManagedObject = isALocalMO;
}

string ManagedObjectSchemaInfo::getDerivedFromClassName () const
{
    return (m_derivedFromClassName);
}

void ManagedObjectSchemaInfo::setDerivedFromClassName (const string derivedFromTable)
{
    m_derivedFromClassName = derivedFromTable;
}

bool ManagedObjectSchemaInfo::operator== (const ManagedObjectSchemaInfo &rhs) const
{
    return ((m_tableId == rhs.getTableId ()) &&
        (m_parentTableId == rhs.getParentTableId ()) &&
        (m_name == rhs.getName ()) &&
        (m_simpleFieldTypeMap == rhs.m_simpleFieldTypeMap) &&
        (m_relationFieldTypeMap == rhs.m_relationFieldTypeMap) && 
        (m_isALocalManagedObject == rhs.m_isALocalManagedObject) && 
        (m_derivedFromClassName == rhs.m_derivedFromClassName) && 
        (m_userDefinedKeyCombinationsMap == rhs.m_userDefinedKeyCombinationsMap)); // && 
//        (m_userDefinedKeyCombinationString == rhs.m_userDefinedKeyCombinationString));
}

bool ManagedObjectSchemaInfo::operator!= (const ManagedObjectSchemaInfo &rhs) const
{
    return (!operator== (rhs));
}

ResourceId ManagedObjectSchemaInfo::parseAndCompareRelationships (string lhsRelationTypeFull, string rhsRelationTypeFull) const
{
    trace (TRACE_LEVEL_DEVEL, "ManagedObjectSchemaInfo::parseAndCompareRelationships: Entering: lhsRelationType = " + lhsRelationTypeFull + " : rhsRelationType = " + rhsRelationTypeFull);
    vector<string> lhsTokens, rhsTokens;
    tokenize (lhsRelationTypeFull, lhsTokens, '-');
    tokenize (rhsRelationTypeFull, rhsTokens, '-');
    ResourceId status = FRAMEWORK_SUCCESS;

    if (lhsTokens[0] != rhsTokens[0])
    {   
        trace (TRACE_LEVEL_DEVEL, "ManagedObjectSchemaInfo::parseAndCompareRelationships: Change in relation UML type from " + rhsTokens[0] + " to " + lhsTokens[0]);
    }

    if (lhsTokens[1] != rhsTokens[1])
    {   
        trace (TRACE_LEVEL_DEVEL, "ManagedObjectSchemaInfo::parseAndCompareRelationships: Change in relation type from " + rhsTokens[1] + " to " + lhsTokens[1]);
    }

    vector<string> lhsTkns, rhsTkns;
    tokenize (lhsTokens[2],lhsTkns,'|');
    tokenize (rhsTokens[2],rhsTkns,'|');

    if (lhsTkns[0] != rhsTkns[0])
    {   
        trace (TRACE_LEVEL_DEVEL, "ManagedObjectSchemaInfo::parseAndCompareRelationships: Change in relatedToMO from " + rhsTkns[0] + " to " + lhsTkns[0]);
    }

    if (lhsTkns[2] != rhsTkns[2])
    {   
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::parseAndCompareRelationships: Change in disableValidation from " + rhsTkns[2] + " to " + lhsTkns[2]);
        status = FRAMEWORK_STATUS_RELATION_FIELD_CHANGE_NOT_SUPPORTED;
    }

    if (lhsTkns[3] != rhsTkns[3])
    {   
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::parseAndCompareRelationships: Change in canBeEmpty from " + rhsTkns[3] + " to " + lhsTkns[3]);
        status = FRAMEWORK_STATUS_RELATION_FIELD_CHANGE_NOT_SUPPORTED;
    }

    return status;
}

        
auto_ptr<RelationFieldSchemaDifferences> ManagedObjectSchemaInfo::getRelationFieldObjectDifferences (const ManagedObjectSchemaInfo &rhs, ResourceId & status) const
{
    auto_ptr<RelationFieldSchemaDifferences> pDifference;

    map<string, string> temp;
    map<string, string>::const_iterator temp_iter;

    if (getName () != rhs.getName ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getRelationFieldObjectDifferences : Attempt to compare managed objects with different names- " + getName () + " and " + rhs.getName ());
        status = FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS;
        return (pDifference);
    }

    if (getTableId () != rhs.getTableId ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getRelationFieldObjectDifferences : Attempt to compare managed object" + getName () + " with different table Id- " + getTableId () + " and " + rhs.getTableId ());
        //status = FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS;
        status = FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE;
        return (pDifference);
    }

    pDifference.reset (new RelationFieldSchemaDifferences ());

    // Compute the added Relationships by doing the set_difference 'this-RHS'
    set_difference (m_relationFieldTypeMap.begin (), m_relationFieldTypeMap.end (), rhs.m_relationFieldTypeMap.begin (), rhs.m_relationFieldTypeMap.end (),
                    inserter (temp, temp.end ()), m_relationFieldTypeMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        RelationshipInfoFromSchemaDifference addedRelationshipInfo((*temp_iter).first, (*temp_iter).second);
        pDifference->insertAddition (addedRelationshipInfo);
    }
    temp.clear ();

    // Compute the removed Relationships by doing the set_difference 'RHS-this'
    set_difference (rhs.m_relationFieldTypeMap.begin (), rhs.m_relationFieldTypeMap.end (), m_relationFieldTypeMap.begin (), m_relationFieldTypeMap.end (),
                    inserter (temp, temp.end ()), m_relationFieldTypeMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        RelationshipInfoFromSchemaDifference removedRelationshipInfo((*temp_iter).first, (*temp_iter).second);
        pDifference->insertDeletion (removedRelationshipInfo);
    }
    temp.clear ();

    // Compute the changed Relationships by doing the set_intersection 'RHS^this' and
    set_intersection (rhs.m_relationFieldTypeMap.begin (), rhs.m_relationFieldTypeMap.end (), m_relationFieldTypeMap.begin (), m_relationFieldTypeMap.end (),
                    inserter (temp, temp.end ()), m_relationFieldTypeMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        map<string, string>::const_iterator lhs_iter = m_relationFieldTypeMap.find (temp_iter->first);
        map<string, string>::const_iterator rhs_iter = rhs.m_relationFieldTypeMap.find (temp_iter->first);
        prismAssert (lhs_iter != m_relationFieldTypeMap.end () && rhs_iter != rhs.m_relationFieldTypeMap.end (), __FILE__, __LINE__);
        if (lhs_iter->second != rhs_iter->second)
        {
            /*
                Treat relationType, relationUMLType, relatedToMO changes as add and remove of the relation
                Treat canBeEmpty, disableValidation as modified relation.
            */
            trace (TRACE_LEVEL_DEVEL, "RelationFieldTypeMaps :lhs=" + lhs_iter->second + ",rhs=" + rhs_iter->second);

            RelationshipInfoFromSchemaDifference lhs_relationshipInfo((*lhs_iter).first, (*lhs_iter).second);
            RelationshipInfoFromSchemaDifference rhs_relationshipInfo((*rhs_iter).first, (*rhs_iter).second);

            if (lhs_relationshipInfo.getRelationType () != rhs_relationshipInfo.getRelationType () ||
                lhs_relationshipInfo.getRelationUmlType () != rhs_relationshipInfo.getRelationUmlType () ||
                lhs_relationshipInfo.getRelatedToTable () != rhs_relationshipInfo.getRelatedToTable ())
            {
                pDifference->insertAddition (lhs_relationshipInfo);
                pDifference->insertDeletion (rhs_relationshipInfo);
                continue;
            }

            if (lhs_relationshipInfo.getDisableValidations () != rhs_relationshipInfo.getDisableValidations ())
            {
                lhs_relationshipInfo.setDisableValidationChanged ();
            }

            if (lhs_relationshipInfo.getCanBeEmpty () != rhs_relationshipInfo.getCanBeEmpty ())
            {
                lhs_relationshipInfo.setCanBeEmptyChanged ();
            }
            pDifference->insertChange (lhs_relationshipInfo);
        }
    }
    temp.clear ();
    

    if (false == pDifference->hasDifferences ())
    {
        status = FRAMEWORK_STATUS_NO_CHANGES;
    }

    return (pDifference);
}

ResourceId ManagedObjectSchemaInfo::getInheritanceDiffStatus (const ManagedObjectSchemaInfo &rhs) const
{
    if (getName () != rhs.getName ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getInheritanceDiffStatus : Attempt to compare managed objects with different names- " + getName () + " and " + rhs.getName ());
        return (FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS);
    }

    if (getTableId () != rhs.getTableId ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getInheritanceDiffStatus : Attempt to compare managed object" + getName () + " with different table Id- " + getTableId () + " and " + rhs.getTableId ());
        return (FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE);
    }

    if (getParentTableId () != rhs.getParentTableId ())
    {
        trace (TRACE_LEVEL_ERROR, "DB Conversion : The Parent Table for ManagedObject " + getName () + " is changed from " + rhs.getParentTableId () + " to " + getParentTableId () + ". This type of conversion is not supported."); 
        return (FRAMEWORK_STATUS_PARENT_TABLE_CHANGE_NOT_SUPPORTED);
    }
    else if ( 0 != (getDerivedFromClassName ()).compare(rhs.getDerivedFromClassName ()))
    {
        trace (TRACE_LEVEL_WARN, "ManagedObjectSchemaInfo::getFieldObjectDifferences : Changed the derivedFromClassName for a MO [" + getName () + "]. This is currently ignored.");
        return (FRAMEWORK_STATUS_DERIVED_FROM_TABLE_CHANGED);
    }
    else
    {
        return FRAMEWORK_STATUS_NO_CHANGES;
    }
}

auto_ptr<FieldSchemaDifferences> ManagedObjectSchemaInfo::getUserDefinedKeyDifferences (const ManagedObjectSchemaInfo &rhs, ResourceId & status) const
{
    auto_ptr<FieldSchemaDifferences> pDifference;

    map<string, string> temp;
    map<string, string>::const_iterator temp_iter;

    if (getName () != rhs.getName ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getUserDefinedKeyDifferences : Attempt to compare managed objects with different names- " + getName () + " and " + rhs.getName ());
        status = FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS;
        return (pDifference);
    }

    if (getTableId () != rhs.getTableId ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getUserDefinedKeyDifferences : Attempt to compare managed object" + getName () + " with different table Id- " + getTableId () + " and " + rhs.getTableId ());
        //status = FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS;
        status = FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE;
        return (pDifference);
    }

    pDifference.reset (new FieldSchemaDifferences ());
    
    // Compute the added simpleFields by doing the set_difference 'this-RHS'
    set_difference (m_userDefinedKeyCombinationsMap.begin (), m_userDefinedKeyCombinationsMap.end (), rhs.m_userDefinedKeyCombinationsMap.begin (), rhs.m_userDefinedKeyCombinationsMap.end (),
                    inserter (temp, temp.end ()), m_userDefinedKeyCombinationsMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, "DB Conversion : Adding a user defined key combination field - Name : %s, Type : %s\n", temp_iter->first.c_str(), temp_iter->second.c_str());
        pDifference->insertAddition (*temp_iter);
//        status = FRAMEWORK_STATUS_USER_DEFINED_KEY_COMBINATION_CHANGES_NOT_SUPPORTED;
    }
    temp.clear ();

    // Compute the removed simpleFields by doing the set_difference 'RHS-this'
    set_difference (rhs.m_userDefinedKeyCombinationsMap.begin (), rhs.m_userDefinedKeyCombinationsMap.end (), m_userDefinedKeyCombinationsMap.begin (), m_userDefinedKeyCombinationsMap.end (),
                    inserter (temp, temp.end ()), m_userDefinedKeyCombinationsMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, "DB Conversion : Deleting a user defined combination field - Name : %s, Type : %s\n", temp_iter->first.c_str(), temp_iter->second.c_str());
        pDifference->insertDeletion (*temp_iter);
//        status = FRAMEWORK_STATUS_USER_DEFINED_KEY_COMBINATION_CHANGES_NOT_SUPPORTED;
    }
    temp.clear ();
    
    // Compute the changed simpleFields by doing the set_intersection 'RHS^this' and
    //OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    set_intersection (rhs.m_userDefinedKeyCombinationsMap.begin (), rhs.m_userDefinedKeyCombinationsMap.end (), m_userDefinedKeyCombinationsMap.begin (), m_userDefinedKeyCombinationsMap.end (),
                    inserter (temp, temp.end ()), m_userDefinedKeyCombinationsMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        map<string, string>::const_iterator lhs_iter = m_userDefinedKeyCombinationsMap.find (temp_iter->first);
        map<string, string>::const_iterator rhs_iter = rhs.m_userDefinedKeyCombinationsMap.find (temp_iter->first);
        prismAssert (lhs_iter != m_userDefinedKeyCombinationsMap.end () && rhs_iter != rhs.m_userDefinedKeyCombinationsMap.end (), __FILE__, __LINE__);
        
        if (lhs_iter->second != rhs_iter->second)
        {
//            status = FRAMEWORK_STATUS_USER_DEFINED_KEY_COMBINATION_CHANGES_NOT_SUPPORTED;
            tracePrintf (TRACE_LEVEL_INFO, false, false, "DB Conversion : Changing a user defined key combination field - Name : %s, Changed from : %s to to : %s\n", temp_iter->first.c_str(), rhs_iter->second.c_str(),lhs_iter->second.c_str());

//            if (true == pOrmRepository->isTypeConversionSupported (rhs_iter->second.substr (0, rhs_iter->second.find ("|")), rhs_iter->second.substr (0, rhs_iter->second.find ("|"))))
//            {
                pDifference->insertChange (*lhs_iter);
                trace (TRACE_LEVEL_INFO, "DB Conversion : The user defined key combination field type change is supported ....."); 
//            }
/*            else
            {
                //status = FRAMEWORK_STATUS_FIELD_CHANGE_NOT_SUPPORTED;
                pDifference.reset (new FieldSchemaDifferences ());
                trace (TRACE_LEVEL_INFO, "DB Conversion : The user defined key combination field type change is not supported ......"); 
                break;
            }
*/
        }
    }
    
    temp.clear ();

    if ((false == pDifference->hasDifferences ()) && (status == FRAMEWORK_SUCCESS))
    {
        status = FRAMEWORK_STATUS_NO_CHANGES;
    }
    
    return (pDifference);
}

auto_ptr<FieldSchemaDifferences> ManagedObjectSchemaInfo::getFieldObjectDifferences (const ManagedObjectSchemaInfo &rhs, ResourceId & status) const
{
    auto_ptr<FieldSchemaDifferences> pDifference;

    map<string, string> temp;
    map<string, string>::const_iterator temp_iter;

    if (getName () != rhs.getName ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getFieldObjectDifferences : Attempt to compare managed objects with different names- " + getName () + " and " + rhs.getName ());
        status = FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS;
        return (pDifference);
    }

    if (getTableId () != rhs.getTableId ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getFieldObjectDifferences : Attempt to compare managed object" + getName () + " with different table Id- " + getTableId () + " and " + rhs.getTableId ());
        //status = FRAMEWORK_STATUS_INCONSISTENT_MANAGED_OBJECTS;
        status = FRAMEWORK_STATUS_UNHANDLED_TABLEID_CHANGE;
        return (pDifference);
    }

    if (getIsALocalManagedObject () != rhs.getIsALocalManagedObject ())
    {
        trace (TRACE_LEVEL_ERROR, "ManagedObjectSchemaInfo::getFieldObjectDifferences : Changed the MO [" + getName () + "] from a local/global to a global/local MO. This is not supported");
        status = FRAMEWORK_STATUS_LOCAL_GLOBAL_CHANGE_NOT_SUPPORTED;
        return (pDifference);
    }

        
    pDifference.reset (new FieldSchemaDifferences ());

    // Compute the added simpleFields by doing the set_difference 'this-RHS'
    set_difference (m_simpleFieldTypeMap.begin (), m_simpleFieldTypeMap.end (), rhs.m_simpleFieldTypeMap.begin (), rhs.m_simpleFieldTypeMap.end (),
                    inserter (temp, temp.end ()), m_simpleFieldTypeMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, "DB Conversion : Adding a simple field - Name : %s, Type : %s\n", temp_iter->first.c_str(), temp_iter->second.c_str());
        pDifference->insertAddition (*temp_iter);
    }
    temp.clear ();

    // Compute the removed simpleFields by doing the set_difference 'RHS-this'
    set_difference (rhs.m_simpleFieldTypeMap.begin (), rhs.m_simpleFieldTypeMap.end (), m_simpleFieldTypeMap.begin (), m_simpleFieldTypeMap.end (),
                    inserter (temp, temp.end ()), m_simpleFieldTypeMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, "DB Conversion : Deleting a simple field - Name : %s, Type : %s\n", temp_iter->first.c_str(), temp_iter->second.c_str());
        pDifference->insertDeletion (*temp_iter);
    }
    temp.clear ();

    // Compute the changed simpleFields by doing the set_intersection 'RHS^this' and
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    set_intersection (rhs.m_simpleFieldTypeMap.begin (), rhs.m_simpleFieldTypeMap.end (), m_simpleFieldTypeMap.begin (), m_simpleFieldTypeMap.end (),
                    inserter (temp, temp.end ()), m_simpleFieldTypeMap.value_comp ());
    for (temp_iter = temp.begin (); temp_iter != temp.end (); ++temp_iter)
    {
        map<string, string>::const_iterator lhs_iter = m_simpleFieldTypeMap.find (temp_iter->first);
        map<string, string>::const_iterator rhs_iter = rhs.m_simpleFieldTypeMap.find (temp_iter->first);
        prismAssert (lhs_iter != m_simpleFieldTypeMap.end () && rhs_iter != rhs.m_simpleFieldTypeMap.end (), __FILE__, __LINE__);
        if (lhs_iter->second != rhs_iter->second)
        {
            string rhsWaveAttributeType, lhsWaveAttributeType;
            string rhsSqlDataType, lhsSqlDataType;

            ManagedObjectSchemaInfo::getDataTypesFromExtendedFieldType (rhs_iter->second, rhsWaveAttributeType, rhsSqlDataType);
            ManagedObjectSchemaInfo::getDataTypesFromExtendedFieldType (lhs_iter->second, lhsWaveAttributeType, lhsSqlDataType);

            if (lhsWaveAttributeType != rhsWaveAttributeType)
            {
                tracePrintf (TRACE_LEVEL_INFO, false, false, "DB Conversion : Changing simple field '%s' WaveAttribute Type from : %s to %s\n", temp_iter->first.c_str(), rhsWaveAttributeType.c_str(), lhsWaveAttributeType.c_str());

                if (true == pOrmRepository->isTypeConversionSupported (rhs_iter->second.substr (0, rhs_iter->second.find ("|")), lhs_iter->second.substr (0, lhs_iter->second.find ("|"))))
                {
                    pDifference->insertChange (*lhs_iter);
                    trace (TRACE_LEVEL_INFO, "DB Conversion : The change is supported ....."); 
                }
                else
                {
                    status = FRAMEWORK_STATUS_FIELD_CHANGE_NOT_SUPPORTED;
                    pDifference.reset (new FieldSchemaDifferences ());
                    trace (TRACE_LEVEL_INFO, "DB Conversion : The change is not supported ......"); 
                    break;
                }
            }
            else if (lhsSqlDataType != rhsSqlDataType)
            {
                tracePrintf (TRACE_LEVEL_WARN, false, false, "DB Conversion : Changing simple field '%s' SQL Data Type from : %s to %s.\n  We depend on Postgres implicit type conversion. This conversion may/may not succeed while applying SQL to change this datatype. It all depends on the datatype change and also on the data present currently in the column.\n", temp_iter->first.c_str(), rhsSqlDataType.c_str(), lhsSqlDataType.c_str());
                pDifference->insertChange (*lhs_iter);
            }
        }
    }
    
    temp.clear ();

    if ((false == pDifference->hasDifferences ()) && (status == FRAMEWORK_SUCCESS))
    {
        status = FRAMEWORK_STATUS_NO_CHANGES;
    }
    
    return (pDifference);
}

void ManagedObjectSchemaInfo::getDataTypesFromExtendedFieldType (const string &extendedFieldType, string &attributeType, string &sqlDataType)
{
    vector<string> tokens;
    tokenize(extendedFieldType, tokens, '|');

    attributeType = tokens[0];
    sqlDataType   = tokens[1];
}

string ManagedObjectSchemaInfo::getSQLForTableUpdate (const string &schema, const vector<pair<string, string> > &addedFields, const vector<pair<string, string> > &removedFields, const vector<pair<string, string> > &changedFields) const
{
    string sqlcommand;

        const string altercommand = "ALTER TABLE " + schema + "." + getName ();
        vector<pair<string, string> >::const_iterator iter;

    for (iter = removedFields.begin (); iter != removedFields.end (); ++iter)
        {
            sqlcommand += altercommand + " DROP COLUMN " + (*iter).first + ";\n";
        }
//        OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    for (iter = addedFields.begin (); iter != addedFields.end (); ++iter)
        {
            /*
             * The type in the map is the ExtendedType of the format WAVE_ATTRIBUTE_TYPE_OBJECTID|integer|0|0,
             * So, parse the second field and get the SQL type.
             */
            string sqlDataType;
            string attributeType;
            ManagedObjectSchemaInfo::getDataTypesFromExtendedFieldType (iter->second, attributeType, sqlDataType);
            tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfo::getSQLForTableUpdate : Sql String : '%s' Extended String : '%s'\n", sqlDataType.c_str (), (iter->second).c_str ());
            if (sqlDataType != "")
            {
                sqlcommand += altercommand + " ADD COLUMN " + iter->first + " " + sqlDataType + ";\n";
            }
            else
            {
                tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfo::getSQLForTableUpdate : Sql String returned from OrmRepository should never be a NULL string when Adding a Column. Type : '%s' Attribute String : '%s'\n", (iter->first).c_str (), (iter->second).c_str ());
                prismAssert (false, __FILE__, __LINE__);
            }
        }

    for (iter = changedFields.begin (); iter != changedFields.end (); ++iter)
        {
            /*
             * The type in the map is the ExtendedType of the format WAVE_ATTRIBUTE_TYPE_OBJECTID|integer|0|0,
             * So, parse the second field and get the SQL type.
             */
            // Pritee : Should we not check if the column has any entries already before modifying the name/type?
            string sqlDataType;
            string attributeType;
            ManagedObjectSchemaInfo::getDataTypesFromExtendedFieldType (iter->second, attributeType, sqlDataType);
            tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfo::getSQLForTableUpdate : Sql String : '%s' Extended String : '%s'\n", sqlDataType.c_str (), (iter->second).c_str ());
            if (sqlDataType != "")
            {
                sqlcommand += altercommand + " ALTER COLUMN " + iter->first + " TYPE " + sqlDataType + ";\n";
            }
            else 
            {
                tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfo::getSQLForTableUpdate : Sql String returned from OrmRepository should never be a NULL string. Type : '%s' Attribute String : '%s'\n", (iter->first).c_str (), (iter->second).c_str ());
                prismAssert (false, __FILE__, __LINE__);
            }
        }

    return (sqlcommand);
}

void ManagedObjectSchemaInfo::createReferenceCountToSchemaInfoObject (const string & toObjectName, ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository)
{
      ManagedObjectSchemaInfo* pToManagedObjectSchemaInfo = pManagedObjectSchemaInfoRepository->findSchemaInfoObject(toObjectName);
      RelationReferenceCountPointer<ManagedObjectSchemaInfo>*   pRelationReferenceCountPointer = new RelationReferenceCountPointer<ManagedObjectSchemaInfo>(pToManagedObjectSchemaInfo);
      m_relationReferenceCountPointers.push_back(pRelationReferenceCountPointer);  
}

void ManagedObjectSchemaInfo::updateFromEndForRelatedToTable (ManagedObjectSchemaInfo* relatedToTable, const string& relation, const string& relationType)
{
    string relationtype = (relationType=="m")?relation:"ORM_RELATION_TYPE_ONE_TO_ONE";

    relatedToTable->updateRelationFromTable (relationtype, this);

    tracePrintf (TRACE_LEVEL_DEBUG, true, false, "ManagedObjectSchemaInfo::updateFromEndForRelatedToTable: adding [%s]:[%s] as relation for [%s]", getName().c_str(), relationType.c_str(), (relatedToTable->getName()).c_str());
    
}

void ManagedObjectSchemaInfo::updateRelationFromTable (const string& relation, ManagedObjectSchemaInfo* fromMoSchema)
{
    m_relationsFromTable[relation] = fromMoSchema; 
}

void ManagedObjectSchemaInfo::removeFromEndForRelatedToTable (ManagedObjectSchemaInfo* relatedToTable, const string& relation)
{
    relatedToTable->removeRelationFromTable (relation);

    tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfo::removeFromEndForRelatedToTable: removed [%s]:[%s] relation from [%s]", getName ().c_str(), relation.c_str(), (relatedToTable->getName ()).c_str());
}

void ManagedObjectSchemaInfo::removeRelationFromTable (const string& relation)
{
    map <string, ManagedObjectSchemaInfo*>::const_iterator relationsIter = m_relationsFromTable.find (relation);
    map <string, ManagedObjectSchemaInfo*>::const_iterator relationsIterEnd = m_relationsFromTable.end  ();
    if (relationsIter != relationsIterEnd)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfo::removeRelationFromTable: relation [%s]", relation.c_str());
        m_relationsFromTable.erase (relation);
    }
}

void ManagedObjectSchemaInfo::populateRelationReferencePointers (ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository)
{
    tracePrintf (TRACE_LEVEL_DEBUG, true,false,"ManagedObjectSchemaInfo::populateRelationReferencePointers: ManagedObjectName: %s,", getName ().c_str());

    map<string, string>::const_iterator temp_iter = m_relationFieldTypeMap.begin ();

    for(;temp_iter!=m_relationFieldTypeMap.end ();++temp_iter) 
    {
	    string fieldType = (temp_iter->second);
    	tracePrintf (TRACE_LEVEL_DEBUG, true,false,"ManagedObjectSchemaInfo::populateRelationReferencePointers: fieldType: %s,", fieldType.c_str());

	    vector<string> tokens;
    	tokenize (fieldType,tokens,'-');
        StringUtils::stripFromFirstOccuranceOf (tokens[2],'|');
	    string toObjectName = tokens[2];
    
    	ManagedObjectSchemaInfo* toObject = pManagedObjectSchemaInfoRepository->findSchemaInfoObject(toObjectName);
	    tracePrintf (TRACE_LEVEL_DEBUG, true,false,"ManagedObjectSchemaInfo::populateRelationReferencePointers: toObjectName %s,", toObjectName.c_str());

    	prismAssert(NULL!=toObject,__FILE__,__LINE__);
	    tracePrintf (TRACE_LEVEL_DEBUG, true,false,"ManagedObjectSchemaInfo::populateRelationReferencePointers: toObjectReferenceCount Before %d,", toObject->getReferenceCount());


        RelationReferenceCountPointer<ManagedObjectSchemaInfo>*   pRelationReferenceCountPointer = new RelationReferenceCountPointer<ManagedObjectSchemaInfo>(toObject);
    	tracePrintf (TRACE_LEVEL_DEBUG, true,false,"ManagedObjectSchemaInfo::populateRelationReferencePointers: toObjectReferenceCount After %d,", toObject->getReferenceCount());
        updateFromEndForRelatedToTable (toObject, temp_iter->first, tokens[1]);

	    m_relationReferenceCountPointers.push_back(pRelationReferenceCountPointer); 

	    if(tokens[1]=="m") 
    	{
	        ++m_numberOf1tonRelationFields;
	    }
    }

}

UI32 ManagedObjectSchemaInfo::getCountOfRelationshipReferecePointers ()
{
    return (m_relationReferenceCountPointers.size ());
}

void ManagedObjectSchemaInfo::displayRelationshipReferenceCountInfo ()
{
    tracePrintf (TRACE_LEVEL_DEBUG, true,false,"ManagedObjectSchemaInfo::displayRelationshipReferenceCountInfo: ManagedObjectName: %s, ReferenceCount: %d", getName ().c_str(), getReferenceCount ());

    for (UI32 i =0; i< m_relationReferenceCountPointers.size (); ++i) 
    {
	RelationReferenceCountPointer<ManagedObjectSchemaInfo> *pRelationReferenceCountPointer = m_relationReferenceCountPointers[i];
	string toObjectName = (*pRelationReferenceCountPointer)->getName();

	tracePrintf (TRACE_LEVEL_DEBUG, true,false,"ManagedObjectSchemaInfo::displayRelationshipReferenceCountInfo: ManagedObjectName: %s, RelatedTo: %s", getName ().c_str(), toObjectName.c_str());
    }

}



void ManagedObjectSchemaInfo::processRelationsAddedInNewSchema(const vector<RelationshipInfoFromSchemaDifference> & relationshipsInfoVector, ManagedObjectSchemaInfoRepository & dbManagedObjectSchemaInfoRepository,  string & sqlForDerivationTablesForCurrentSchema,string & sqlForDerivationTablesForStartSchema,string & sqlForAuxilliaryTablesForCurrentSchema,string &  sqlForAuxilliaryTablesForStartSchema)
{

 
  vector<RelationshipInfoFromSchemaDifference>::const_iterator relationsIterator = relationshipsInfoVector.begin ();
  for(;relationsIterator != relationshipsInfoVector.end (); ++relationsIterator) 
  {

      string relationName = relationsIterator->getRelationName ();
      string relatedToTableName = relationsIterator->getRelatedToTable ();
      //OrmRelationUmlType  relationUmlType = relationsIterator->getRelationUmlType ();
      OrmRelationType     relationType = relationsIterator->getRelationType ();

      ManagedObjectSchemaInfo* toObject = dbManagedObjectSchemaInfoRepository.findSchemaInfoObject(relatedToTableName);

	  prismAssert (NULL != toObject, __FILE__, __LINE__);
	  
      RelationReferenceCountPointer<ManagedObjectSchemaInfo>*   pRelationReferenceCountPointer = new RelationReferenceCountPointer<ManagedObjectSchemaInfo>(toObject);

      m_relationReferenceCountPointers.push_back(pRelationReferenceCountPointer); 

        tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfo::processRelationsAddedInNewSchema : Entering for Relation in Managed Object %s -- Relation Name : %s, RelatedToTable : %s, RelationType : %d, FromObject's NumberOf1toN : %d, FromObject's ReferenceCount : %d, ToObject's NumberOf1toN : %d, ToObject's Reference Count : %d\n\n", m_name.c_str(), relationName.c_str (), relatedToTableName.c_str (), relationType, getNumberOf1tonRelationFields (), getReferenceCount(), toObject->getNumberOf1tonRelationFields (), toObject->getReferenceCount());



      if(ORM_RELATION_TYPE_ONE_TO_ONE==relationType) 
      {
        //For a 1:1 referenceCount pointer nothing is required on the from side but on the to side 
        //if the referenceCount pointer vector has no 1:n Relationship Reference Count pointer then generate sql for creating 
        //Derivations and Derivations instances table on the to side.
	        if((0 == toObject->getNumberOf1tonRelationFields ()) && (1 == toObject->getReferenceCount()))
	        {
	            sqlForDerivationTablesForCurrentSchema+= OrmRepository::getSchemaForDerivationsTable(relatedToTableName,OrmRepository::getWaveCurrentSchema()); 
	            sqlForDerivationTablesForStartSchema+= OrmRepository::getSchemaForDerivationsTable(relatedToTableName,OrmRepository::getWaveStartSchema()); 
        	}

      }       
      else if(ORM_RELATION_TYPE_ONE_TO_MANY==relationType) 
      {
	        //    ---For a 1:n referenceCount pointer: 
            //    -- On the from side if the rc is 0 and there are no 1:n refrecnce count pointers then generate sql for creating 
            //       derivations and derivations instances on the from side
   	        if((0 == getNumberOf1tonRelationFields ()) && (0 == getReferenceCount()))
	        {
	            sqlForDerivationTablesForCurrentSchema+= OrmRepository::getSchemaForDerivationsTable(m_name,OrmRepository::getWaveCurrentSchema()); 
	            sqlForDerivationTablesForStartSchema+= OrmRepository::getSchemaForDerivationsTable(m_name,OrmRepository::getWaveStartSchema()); 
	        }    

            //    -- On the to side if the rc is 0 and there are no 1:n refrecnce count pointers then generate sql for creating 
            //                          derivations and derivations instances on the to side
   	        if((0 == toObject->getNumberOf1tonRelationFields ()) && (1 == toObject->getReferenceCount()))
	        {
	            sqlForDerivationTablesForCurrentSchema+= OrmRepository::getSchemaForDerivationsTable(relatedToTableName,OrmRepository::getWaveCurrentSchema()); 
	            sqlForDerivationTablesForStartSchema+= OrmRepository::getSchemaForDerivationsTable(relatedToTableName,OrmRepository::getWaveStartSchema()); 

	        }        

            //-- generate SQL for auxiliary table corresponding to each relationship
	        OrmTable* pOrmTable = OrmRepository::getTableByName(m_name);
	        prismAssert (NULL!=pOrmTable, __FILE__,__LINE__);
	        sqlForAuxilliaryTablesForCurrentSchema+=  pOrmTable->getSqlForCreate2 (OrmRepository::getWaveCurrentSchema(), relationName);
	        sqlForAuxilliaryTablesForStartSchema+=  pOrmTable->getSqlForCreate2 (OrmRepository::getWaveStartSchema(), relationName);

	        ++m_numberOf1tonRelationFields;
      }
     
  }

}

// This function call is made in the context of DBSCHEMAINFOREPOSITORY 
ResourceId ManagedObjectSchemaInfo::getSqlForRemovedRelationship (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, const string & relationName, const string & relatedToTable, OrmRelationUmlType relationUmlType, OrmRelationType relationType, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS;



    // "from" side means the current ManagedObjectSchemaInfo object side of the relationship
    // "to" side means the ManagedObjectSchemaInfo object to which the relationship is pointing to in the "DB Schema Repository"
    // If 1 to 1
    if (relationType == ORM_RELATION_TYPE_ONE_TO_ONE)
    {
        /* 1. Generate Sql for removing the classid, instanceid columns
              of this relationship in the "from" side by calling getSqlForRemovingClassIdInstanceIdColumns
           2. Get the "to" side's ManagedObjectSchemaInfo using findManagedObjectSchemaInfo
           3. Remove the "to" ManagedObjectSchemaInfo from m_relationReferenceCountPointers
           4. Call the "to" ManagedObjectSchemaInfo's getSqlForRemovingDerivationAndDerivationInstances
         */
       
        // 1
        getSqlForRemovingClassIdInstanceIdColumns (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, relationName);

        // 2 
        ManagedObjectSchemaInfo *pRelatedToManagedObject = dbSchemaInfoRepository.findSchemaInfoObject (relatedToTable);

        if (NULL == pRelatedToManagedObject)
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "ManagedObjectSchemaInfo::getSqlForRemovedRelationship: DbSchemaRepository missing the MO at to side of changed relation field. relatedToTable: %s", relatedToTable.c_str()); 
            return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
        }
                
        tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfo::getSqlForRemovedRelationship : Entering for Relation in Managed Object %s -- Relation Name : %s, RelatedToTable : %s, RelationType : %d, FromObject's NumberOf1toN : %d, FromObject's ReferenceCount : %d, ToObject's NumberOf1toN : %d, ToObject's ReferenceCount : %d\n\n", m_name.c_str(), relationName.c_str (), relatedToTable.c_str (), relationType, getNumberOf1tonRelationFields (), getReferenceCount(), pRelatedToManagedObject->getNumberOf1tonRelationFields (), pRelatedToManagedObject->getReferenceCount());
        
        // 3
        vector<RelationReferenceCountPointer<ManagedObjectSchemaInfo> *>::iterator relIterator;

        for (relIterator=m_relationReferenceCountPointers.begin(); relIterator!=m_relationReferenceCountPointers.end(); relIterator++)
        {
        	RelationReferenceCountPointer<ManagedObjectSchemaInfo> *pRelationReferenceCountPointer = (*relIterator);
	        string toObjectName = (*pRelationReferenceCountPointer)->getName();

            // If the "toObjectName" is same as relatedToTable then remove the "to" side
            if (toObjectName == relatedToTable)
            {
                delete pRelationReferenceCountPointer;
                m_relationReferenceCountPointers.erase (relIterator); 
                break;
            }
        }

        // 4
        pRelatedToManagedObject->getSqlForRemovingDerivationAndDerivationInstances (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);
    }
    // Else if 1 to n
    else if (relationType == ORM_RELATION_TYPE_ONE_TO_MANY)
    {
        /* 1. Generate Sql to remove the auxillary table using getSqlForRemovingAuxillaryTable
         * 2. Decrement the number of 1 to n relationships 
         * 3. Call getSqlForRemovingDerivationAndDerivationInstances and see if D & DI have to 
         *    removed. If yes, generate the sql for it
         * 4. Get the "to" side's ManagedObjectSchemaInfo using findManagedObjectSchemaInfo
         * 5. Remove the "to" ManagedObjectSchemaInfo from m_relationReferenceCountPointers
         * 6. Remove entry from m_relationsFromTable of relatedToTable where its key is this relationName.
         * 7. Call the "to" ManagedObjectSchemaInfo's getSqlForRemovingDerivationAndDerivationInstances
         */
        
        // 1
        getSqlForRemovingAuxillaryTable (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema, relationName, relatedToTable, dbSchemaInfoRepository);

        // 2
        m_numberOf1tonRelationFields--;

        // 3
        getSqlForRemovingDerivationAndDerivationInstances (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);

        // 4 
        ManagedObjectSchemaInfo *pRelatedToManagedObject = dbSchemaInfoRepository.findSchemaInfoObject (relatedToTable);

        if (NULL == pRelatedToManagedObject)
        {
            return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
        }

        tracePrintf (TRACE_LEVEL_INFO, false, false, "ManagedObjectSchemaInfo::getSqlForRemovedRelationship : Entering for Relation in Managed Object %s -- Relation Name : %s, RelatedToTable : %s, RelationType : %d, FromObject's NumberOf1toN : %d, FromObject's ReferenceCount : %d, ToObject's NumberOf1toN : %d, ToObject's ReferenceCount : %d\n\n", m_name.c_str(), relationName.c_str (), relatedToTable.c_str (), relationType, getNumberOf1tonRelationFields (), getReferenceCount(), pRelatedToManagedObject->getNumberOf1tonRelationFields (), pRelatedToManagedObject->getReferenceCount());
        // 5 
        vector<RelationReferenceCountPointer<ManagedObjectSchemaInfo> *>::iterator relIterator;

        for (relIterator=m_relationReferenceCountPointers.begin(); relIterator!=m_relationReferenceCountPointers.end(); relIterator++)
        {
        	RelationReferenceCountPointer<ManagedObjectSchemaInfo> *pRelationReferenceCountPointer = (*relIterator);
	        string toObjectName = (*pRelationReferenceCountPointer)->getName();

            // If the "toObjectName" is same as relatedToTable then remove the "to" side
            if (toObjectName == relatedToTable)
            {
                delete pRelationReferenceCountPointer;
                m_relationReferenceCountPointers.erase (relIterator);
                break;
            }
        }

        // 6
        removeFromEndForRelatedToTable (pRelatedToManagedObject, relationName);

        // 7
        pRelatedToManagedObject->getSqlForRemovingDerivationAndDerivationInstances (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);
    }

    return (status);
}

ResourceId ManagedObjectSchemaInfo::getSqlForRemovingClassIdInstanceIdColumns (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, const string & relationName)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    // Create the Sql for Removing the ClassId and InstanceId Columns from the table related to the "relationName"

    string altercommandforstart    = string("ALTER TABLE ") + string("wavestart.") + getName ();
    string altercommandforcurrent  = string("ALTER TABLE ") + string("wavecurrent.") + getName ();
    
    string removeClassId           = " DROP COLUMN " + relationName + "classid" + ";\n";
    string removeInstanceId        = " DROP COLUMN " + relationName + "instanceid" + ";\n";

    schemaUpdateSqlForStartSchema += altercommandforstart + removeClassId;
    schemaUpdateSqlForStartSchema += altercommandforstart + removeInstanceId;
    
    schemaUpdateSqlForCurrentSchema += altercommandforcurrent + removeClassId;
    schemaUpdateSqlForCurrentSchema += altercommandforcurrent + removeInstanceId;

    return (status);
}

ResourceId ManagedObjectSchemaInfo::getSqlForRemovingDerivationAndDerivationInstances (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    /*  1. Check to see if Reference Count (the number of MO's pointing to current MO) is 0 
           AND the number of 1:n relationships is 0
     *  2. If YES, get the Sql for Removing the Derivation and Derivation Instances from the table 
     *  3. If NO, the sql will not be generated
     */
    
    if ((0 == getReferenceCount()) && (0 == m_numberOf1tonRelationFields))
    {
        // Sql for removing Derivation and Derivation Instances -- Check about pkey, unique
        
        OrmRepository *pOrmRepository = OrmRepository::getInstance ();
        schemaUpdateSqlForStartSchema += pOrmRepository->getSqlToDropDerivationsTable (getName(), "wavestart");
        schemaUpdateSqlForCurrentSchema += pOrmRepository->getSqlToDropDerivationsTable (getName(), "wavecurrent");
    }

    return (status);
}

ResourceId ManagedObjectSchemaInfo::getSqlForRemovingAuxillaryTable (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema, const string &relationName, const string & relatedToTable, ManagedObjectSchemaInfoRepository & dbSchemaInfoRepository)
{
    ResourceId status = FRAMEWORK_SUCCESS;
    
    ManagedObjectSchemaInfo *pRelatedToManagedObject = dbSchemaInfoRepository.findSchemaInfoObject (relatedToTable);

    if (NULL == pRelatedToManagedObject)
    {
        return (FRAMEWORK_STATUS_CONVERSION_FAILURE);
    }

    // Check if there is a function for generating sql for removing auxillary tables. If yes, then use it otherwise create a new function and put the below sql generation in it

    string auxillaryTableNameToBeRemoved = string ("t") + getTableId () + relationName + string("t") + pRelatedToManagedObject->getTableId();

    schemaUpdateSqlForStartSchema += "DROP TABLE wavestart." + auxillaryTableNameToBeRemoved + ";\n";
    schemaUpdateSqlForCurrentSchema += "DROP TABLE wavecurrent." + auxillaryTableNameToBeRemoved + ";\n";

    return (status);
}

map<string, ManagedObjectSchemaInfo*>& ManagedObjectSchemaInfo::getRelationFromTables ()
{
    return (m_relationsFromTable);
}

void ManagedObjectSchemaInfo::getSqlToRemoveAuxilliaryTableEntriesForToEndTables  (string& schemaUpdateSqlForCurrentSchema, string& schemaUpdateSqlForStartSchema, const UI32 tableIdToBeRemoved)
{
    // 1. Get vector of Tables which are pointing to this table.
    // 2. For each of these tables, check number of 1:n relations. 
    // 3. If the realtion is 1:n and pointed to this table, get the relation (field) name for getting the auxilliary table name.
    // 4. get the auxilliary tablename from relation name 
    // 5. call sql to remove the tableId passed as an arguement from the calculated auxilliary table.

    //1.
    map <string, ManagedObjectSchemaInfo*>::const_iterator relationsIter = m_relationsFromTable.begin ();
    for (;relationsIter != m_relationsFromTable.end (); ++relationsIter)
    {
    //2.     
        if ("ORM_RELATION_TYPE_ONE_TO_ONE" != relationsIter->first)
        {
            ManagedObjectSchemaInfo* pRelationFromTable = relationsIter->second;

            tracePrintf(TRACE_LEVEL_INFO, true, false, "ManagedObjectSchemaInfo::getSqlToRemoveAuxilliaryTableEntriesForToEndTables: [%s] found 1:N relationwith [%s]", pRelationFromTable->getName().c_str(), getName().c_str()); 
         
    //3.        
            schemaUpdateSqlForCurrentSchema += OrmRepository::getSqlToRemoveAuxilliaryTableEntriesForToEnd (pRelationFromTable->getTableId (), relationsIter->first, getTableId(), tableIdToBeRemoved, OrmRepository::getWaveCurrentSchema ());
            schemaUpdateSqlForStartSchema += OrmRepository::getSqlToRemoveAuxilliaryTableEntriesForToEnd (pRelationFromTable->getTableId (), relationsIter->first, getTableId(), tableIdToBeRemoved, OrmRepository::getWaveStartSchema ());
        }
    }
    
}

LeafManagedObjectSchemaInfo::LeafManagedObjectSchemaInfo (const string &name, UI32 tableId, UI32 parentTableId)
    :ManagedObjectSchemaInfo (name, tableId, parentTableId)
{
}

LeafManagedObjectSchemaInfo::~LeafManagedObjectSchemaInfo ()
{
}

void LeafManagedObjectSchemaInfo::addSchemaInfoObject (ManagedObjectSchemaInfo *child)
{
    trace (TRACE_LEVEL_FATAL, "LeafManagedObjectSchemaInfo :: addSchemaInfoObject : Adding child node " + child->getName () + " to a leaf object " + getName () + " is not allowed");
    prismAssert (false, __FILE__, __LINE__);
}

void LeafManagedObjectSchemaInfo::removeInfoObject (ManagedObjectSchemaInfo *child)
{
    trace (TRACE_LEVEL_FATAL, "LeafManagedObjectSchemaInfo :: removeInfoObject : Can not remove child node " + child->getName () + " from a leaf object " + getName ());
    prismAssert (false, __FILE__, __LINE__);
}

vector<ManagedObjectSchemaInfo *> *LeafManagedObjectSchemaInfo::getChildSchemaInfoObjects () 
{
    trace (TRACE_LEVEL_INFO, "LeafManagedObjectSchemaInfo::getChildSchemaInfoObjects : this is a leaf object. Hence, no child");
    return (NULL);
}

CompositeManagedObjectSchemaInfo::CompositeManagedObjectSchemaInfo (const string &name, UI32 tableId, UI32 parentTableId)
    :ManagedObjectSchemaInfo (name, tableId, parentTableId)
{
}

CompositeManagedObjectSchemaInfo::~CompositeManagedObjectSchemaInfo ()
{
}

void CompositeManagedObjectSchemaInfo::addSchemaInfoObject (ManagedObjectSchemaInfo *child)
{
    m_children.push_back (child);
}

void  CompositeManagedObjectSchemaInfo::getClassIdsOfTheEntireSubTree (vector<UI32> & classIdsOfAllNodesInTheSubTree) const
{

   //Do a breadth first search
   vector <ManagedObjectSchemaInfo *>::const_iterator schemaInfoIterator = m_children.begin () ;
   for(;schemaInfoIterator != m_children.end (); ++schemaInfoIterator) 
   {
       classIdsOfAllNodesInTheSubTree.push_back ((*schemaInfoIterator)->getTableId ());
   }

   schemaInfoIterator = m_children.begin () ;
   for(;schemaInfoIterator != m_children.end (); ++schemaInfoIterator) 
   {
       ManagedObjectSchemaInfo* pManagedObjectSchemaInfo = *schemaInfoIterator;
       CompositeManagedObjectSchemaInfo* pCompositeManagedObjectSchemaInfo = dynamic_cast<CompositeManagedObjectSchemaInfo* > (pManagedObjectSchemaInfo);
       prismAssert (NULL!= pCompositeManagedObjectSchemaInfo,__FILE__, __LINE__);
       pCompositeManagedObjectSchemaInfo->getClassIdsOfTheEntireSubTree (classIdsOfAllNodesInTheSubTree); 
   }

}

void CompositeManagedObjectSchemaInfo::generateSqlToRepopulateDerivationsTable (const string & waveSchema, string & sqlToRepopulateDerivationsTable)
{
  trace (TRACE_LEVEL_DEVEL, "CompositeManagedObjectSchemaInfo::generateSqlToRepopulateDerivationsTable :Enter..");

  if( (getReferenceCount () > 0) || (getNumberOf1tonRelationFields () >0)) 
  {
     tracePrintf (TRACE_LEVEL_DEVEL,true,false, "CompositeManagedObjectSchemaInfo::generateSqlToRepopulateDerivationsTable: Mo Name %s :",getName().c_str ());
     vector<UI32>  classIdsOfAllNodesInTheSubTree;
     classIdsOfAllNodesInTheSubTree.push_back(getTableId ());
     getClassIdsOfTheEntireSubTree (classIdsOfAllNodesInTheSubTree);



     //generate the sql to Clear the derivations table
     string sqlToClearDerivationsTable = "DELETE FROM " + waveSchema + "." + getName () + "Derivations;";
     sqlToRepopulateDerivationsTable+= sqlToClearDerivationsTable;
     //generate the SQL to populate the derivations table with the tableIds collected

     for(UI32 i = 0; i<classIdsOfAllNodesInTheSubTree.size (); ++i ) 
     {
	 tracePrintf (TRACE_LEVEL_DEVEL,false, true, "CompositeManagedObjectSchemaInfo::generateSqlToRepopulateDerivationsTable:ClassId of the child :%d",classIdsOfAllNodesInTheSubTree[i]);
	 sqlToRepopulateDerivationsTable+=OrmRepository::getSqlForDerivedTable (getName (),classIdsOfAllNodesInTheSubTree[i],waveSchema) +"\n";
     }
     
  }

}


void CompositeManagedObjectSchemaInfo::repopulateDerivationsInstancesTable (const string & waveSchema, string & sqlToRepopulateDerivationsInstancesTable)
{

#if 0
    if( (getReferenceCount () > 0) || (getNumberOf1tonRelationFields ()>0)) 
    {
      vector<UI32>  classIdsOfAllNodesInTheSubTree;
      classIdsOfAllNodesInTheSubTree.push_back(getTableId ());
      getClassIdsOfTheEntireSubTree (classIdsOfAllNodesInTheSubTree);

     
      sqlToRepopulateDerivationsInstancesTable+=getSqlToDeleteAllEntriesOfDerivationsTable (waveSchema);

      for(UI32 i = 0; i<classIdsOfAllNodesInTheSubTree.size (); ++i ) 
      {
	 sqlToRepopulateDerivationsInstancesTable+=OrmRepository::getSqlForDerivedTable (getName (),classIdsOfAllNodesInTheSubTree[i],waveSchema) +"\n";
      }
    }
#endif

}


vector<ManagedObjectSchemaInfo *> *CompositeManagedObjectSchemaInfo::getChildSchemaInfoObjects () 
{
    return (&m_children);
}

void CompositeManagedObjectSchemaInfo::removeInfoObject (ManagedObjectSchemaInfo *child)
{
    for (vector <ManagedObjectSchemaInfo *>::iterator iter = m_children.begin (); iter != m_children.end (); ++iter)
    {
        if (child->getTableId () == (*iter)->getTableId())
        {
            m_children.erase (iter);      
            break;
        }
    }
}

/*
void ManagedObjectSchemaInfo::getSqlToDropUserDefinedKeyUniqueConstraint (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    if (false == m_userDefinedKeyCombinationsMap.empty ())
    {
        //trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getSqlToDropUserDefinedKeyUniqueConstraint : for [ " + m_name + "]");
        schemaUpdateSqlForCurrentSchema+= string ("ALTER TABLE ") + OrmRepository::getWaveCurrentSchema() + string (".") + m_name + string (" DROP CONSTRAINT IF EXISTS ") + m_name + string("_unique;\n"); 
        schemaUpdateSqlForStartSchema+= string ("ALTER TABLE ") + OrmRepository::getWaveStartSchema() + string (".") + m_name + string (" DROP CONSTRAINT IF EXISTS ") + m_name + string("_unique;\n"); 
    }

}

void ManagedObjectSchemaInfo::getSqlToAddUserDefinedKeyUniqueConstraint (string & schemaUpdateSqlForCurrentSchema, string & schemaUpdateSqlForStartSchema)
{
    UI32                    numberOfUserDefinedKeyCombination = m_userDefinedKeyCombinationsMap.size ();
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
        OrmTable        *pTable     = OrmRepository::getTableByName (m_name);
        vector<string>  udkcInOrder = pTable->getUserDefinedFieldNamesForUpgrade ();
        UI32            keyNumber   = 0; 

        //getSqlToDropUserDefinedKeyUniqueConstraint (schemaUpdateSqlForCurrentSchema, schemaUpdateSqlForStartSchema);
        
        for (;keyNumber < udkcInOrder.size (); keyNumber++)
        {
            if (keyNumber != 0)
            {
                sqlForUniqueConstraint  += ", ";
            }

            string userDefinedKeyField = udkcInOrder [keyNumber];
            map<string, string>::const_iterator iter = m_userDefinedKeyCombinationsMap.find(userDefinedKeyField);

            prismAssert (m_userDefinedKeyCombinationsMap.end () != iter, __FILE__, __LINE__);

            string userDefinedKeyType = iter->second;

            if (userDefinedKeyType == (FrameworkToolKit::localizeToSourceCodeEnum ((AttributeType::AttributeTypeObjectId).getAttributeTypeResourceId())))
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
*/

string ManagedObjectSchemaInfo::getUserDefinedKeyCombinationString ()
{
    return (m_userDefinedKeyCombinationString);
}

void ManagedObjectSchemaInfo::getCompositionParentsForATable (ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository, vector<string>& compositionParentTables)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : tableName = [" + m_name + "]");

    // 1. check if it has any composition relation poiting to it.
    // 2. check for all inherited parents and if any poiting relation to them.
    // 3. collect all composition parent tables.

    if (m_relationsFromTable.size ())
    {
        map<string, ManagedObjectSchemaInfo*>::iterator iter;
        for (iter = m_relationsFromTable.begin (); iter != m_relationsFromTable.end (); iter++)
        {
            ManagedObjectSchemaInfo*        relatationFromTable     = iter->second;
            string                          relationName            = iter->first;
            map<string, string>             relationFieldTypeMap    = relatationFromTable->getRelationFields ();
            map<string, string>::iterator   relIter;
            string                          fieldType;
                
 
            if ("ORM_RELATION_TYPE_ONE_TO_ONE" != relationName)
            {
                relIter = relationFieldTypeMap.find (relationName);

                if (relationFieldTypeMap.end () != relIter)
                {
                    fieldType = relIter->second;
                }
            
                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : fieldType1 : " + fieldType);
                vector<string> tokens;
                tokenize (fieldType,tokens,'-');

                trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : tokens1: " + tokens[0] + ", " + tokens[1] + ", " + tokens[2]);
                if (0 == tokens[0].compare ("c"))
                {
                    compositionParentTables.push_back (relatationFromTable->getName ());
                    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : table1 [" + relatationFromTable->getName () + "] pushed as a composition parent");
                }
            }
            else
            {
                for (relIter = relationFieldTypeMap.begin (); relIter != relationFieldTypeMap.end (); ++relIter)
                {
                    fieldType = relIter->second;
                    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : fieldType2 : " + fieldType);

                    vector<string> tokens;
                    tokenize (fieldType,tokens,'-');
                    StringUtils::stripFromFirstOccuranceOf (tokens[2],'|');
                    string toObjectName = tokens[2];

                    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : tokens2: " + tokens[0] + ", " + tokens[1] + ", " + tokens[2]);
                    if ((0 == toObjectName.compare (m_name)) && (0 == tokens[1].compare ("1")))
                    {
                        if (0 == tokens[0].compare ("c"))
                        {
                            compositionParentTables.push_back (relatationFromTable->getName ());
                            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : table2 [" + relatationFromTable->getName () + "] pushed as a composition parent");
                        }
                    }
                }
            }
        }
    }
    /*else
    {*/
        if (0 != m_parentTableId)
        {
            ManagedObjectSchemaInfo* inheritanceParent = pManagedObjectSchemaInfoRepository->findSchemaInfoObject (m_parentTableId);
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionParentsForATable : inherited parent table : ["+ inheritanceParent->getName () + "]");
            inheritanceParent->getCompositionParentsForATable (pManagedObjectSchemaInfoRepository, compositionParentTables);
        }
    //}
    
    //return (compositionParentTables.size ());
}

void ManagedObjectSchemaInfo::getCompositionChildrenForATable (ManagedObjectSchemaInfoRepository* pManagedObjectSchemaInfoRepository, vector<string>& compositionChildren)
{
    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionChildrenForATable : table : " + m_name);
    map<string, string>::iterator relIter;

    for (relIter = m_relationFieldTypeMap.begin (); relIter != m_relationFieldTypeMap.end (); ++relIter)
    {
        string fieldType = relIter->second;

        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionChildrenForATable : fieldType : " + fieldType);

        vector<string> tokens;
        tokenize (fieldType,tokens,'-');

        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionChildrenForATable : tokens: " + tokens[0] + ", " + tokens[1] + ", " + tokens[2]);

        if (0 == tokens[0].compare ("c"))
        {
            StringUtils::stripFromFirstOccuranceOf (tokens[2],'|');
            string toObjectName = tokens[2];

            compositionChildren.push_back (toObjectName);
            trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionChildrenForATable : pushed table : ["+ toObjectName + "] as inherited composed child ");

            // push all its derived classes also as the composition children.
            ManagedObjectSchemaInfo* pComposedChildSchemaInfoObject = pManagedObjectSchemaInfoRepository->findSchemaInfoObject (toObjectName);
    
            prismAssert (NULL != pComposedChildSchemaInfoObject, __FILE__, __LINE__);

            vector<ManagedObjectSchemaInfo *> *pInheritedChildren = pComposedChildSchemaInfoObject->getChildSchemaInfoObjects ();

            if (pInheritedChildren != NULL)
            {   
                for (vector<ManagedObjectSchemaInfo *>::iterator iter1 = (*pInheritedChildren).begin (); iter1 != (*pInheritedChildren).end (); ++iter1)
                {   
                    string inheritedChildName = (*iter1)->getName ();    
                    compositionChildren.push_back (inheritedChildName);
                    trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionChildrenForATable : pushed table : ["+ inheritedChildName + "] as inherited composed child ");
                }
            }
        }
    }

    if (0 != m_parentTableId)
    {
        ManagedObjectSchemaInfo* inheritanceParent = pManagedObjectSchemaInfoRepository->findSchemaInfoObject (m_parentTableId);
        trace (TRACE_LEVEL_INFO, "ManagedObjectSchemaInfo::getCompositionChildrenForATable : inherited parent table : ["+ inheritanceParent->getName () + "]");

        inheritanceParent->getCompositionChildrenForATable (pManagedObjectSchemaInfoRepository, compositionChildren);
    }

    //return (compositionChildren.size ());
}
        
}    
