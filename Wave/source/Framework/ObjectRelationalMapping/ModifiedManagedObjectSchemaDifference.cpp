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

/**
 *@file ModifiedManagedObjectSchemaDifference.cpp 
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 *                    All rights reserved. 
 * Description: This file defines  the methods in the
 *              class that store the chagnes that 
 *              have been made to an existing table
 *               
 * Author :     Aashish Akhouri 
 * Date :       10/20/2011 
 */

#include "Framework/ObjectRelationalMapping/ModifiedManagedObjectSchemaDifference.h"
#include "Framework/ObjectRelationalMapping/SchemaDifference.h" 
#include "Framework/Utils/TraceUtils.h"
#include <string>


namespace WaveNs
{

ModifiedManagedObjectSchemaDifference::ModifiedManagedObjectSchemaDifference (const string & tableName)
  : m_tableName  (tableName)
{
}

ModifiedManagedObjectSchemaDifference::~ModifiedManagedObjectSchemaDifference ()
{
}

void ModifiedManagedObjectSchemaDifference::addAddedFieldInfo(const pair<string, string> & addedFieldInfo)
{
  m_fieldSchemaDifferences.insertAddition(addedFieldInfo);
}

void  ModifiedManagedObjectSchemaDifference::addRemovedFieldInfo(const pair<string, string> & removedFieldInfo)
{
  m_fieldSchemaDifferences.insertDeletion(removedFieldInfo);
}
void ModifiedManagedObjectSchemaDifference::addChangedFieldInfo(const pair<string, string> & changedFieldInfo)
{
  m_fieldSchemaDifferences.insertChange(changedFieldInfo);
}

const  string &  ModifiedManagedObjectSchemaDifference::getTableName () const
{
  return m_tableName;
}

const  vector<pair<string, string> > &  ModifiedManagedObjectSchemaDifference::getAddedFieldsInfo () const
{
  return (m_fieldSchemaDifferences.getAdditions ());
   
}

const vector<pair<string, string> > &  ModifiedManagedObjectSchemaDifference::getRemovedFieldsInfo () const
{

    return (m_fieldSchemaDifferences.getDeletions ());
}

const vector<pair<string, string> > &  ModifiedManagedObjectSchemaDifference::getChangedFieldsInfo () const
{
  return (m_fieldSchemaDifferences.getChanges ());
}

void ModifiedManagedObjectSchemaDifference::addAddedRelations (const RelationshipInfoFromSchemaDifference & relationshipInfo)
{

  m_addedRelations.push_back(relationshipInfo);   

}

void ModifiedManagedObjectSchemaDifference::addRemovedRelations (const RelationshipInfoFromSchemaDifference & relationshipInfo)
{
  m_removedRelations.push_back(relationshipInfo);
}

void ModifiedManagedObjectSchemaDifference::addChangedRelations (const RelationshipInfoFromSchemaDifference & relationshipInfo)
{
  m_changedRelations.push_back(relationshipInfo);
}

const  vector<RelationshipInfoFromSchemaDifference> &  ModifiedManagedObjectSchemaDifference::getAddedRelations()
{
  return (m_addedRelations);
}

const  vector<RelationshipInfoFromSchemaDifference> & ModifiedManagedObjectSchemaDifference::getRemovedRelations () const
{
  return (m_removedRelations);
}

const  vector<RelationshipInfoFromSchemaDifference> & ModifiedManagedObjectSchemaDifference::getChangedRelations () const
{
  return (m_changedRelations);
}

void ModifiedManagedObjectSchemaDifference::printContentsForDebugging ()
{
    tracePrintf (TRACE_LEVEL_INFO, false, false, "ModifiedManagedObjectSchemaDifference::printContentsForDebugging : Entering for Table Name : %s\n", m_tableName.c_str());  

    // Print all the simple fields 

    vector<pair<string, string> > simpleFieldAdditions = getAddedFieldsInfo ();
    vector<pair<string, string> > simpleFieldDeletions = getRemovedFieldsInfo ();
    vector<pair<string, string> > simpleFieldChanges   = getChangedFieldsInfo ();

    vector<pair<string, string> >::iterator fieldIterator; 

    for (fieldIterator=simpleFieldAdditions.begin (); fieldIterator != simpleFieldAdditions.end(); fieldIterator++)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, " Added (+) Simple Field %s with type %s in table %s\n", (*fieldIterator).first.c_str(), (*fieldIterator).second.c_str(), m_tableName.c_str());        
    }
    
    for (fieldIterator=simpleFieldDeletions.begin (); fieldIterator != simpleFieldDeletions.end(); fieldIterator++)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, " Deleted (-) Simple Field %s with type %s in table %s\n", (*fieldIterator).first.c_str(), (*fieldIterator).second.c_str(), m_tableName.c_str());        
    }
    
    for (fieldIterator=simpleFieldChanges.begin (); fieldIterator != simpleFieldChanges.end(); fieldIterator++)
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, " Changed (~) Simple Field %s with type %s in table %s\n", (*fieldIterator).first.c_str(), (*fieldIterator).second.c_str(), m_tableName.c_str());        
    }


    vector<RelationshipInfoFromSchemaDifference>::iterator rIterator;

    trace (TRACE_LEVEL_INFO, "ModifiedManagedObjectSchemaDifference::printContentsForDebugging : Added Relations are : ");

    for (rIterator=m_addedRelations.begin(); rIterator!=m_addedRelations.end(); rIterator++)
    {    
        (*rIterator).printContentsForDebugging ();
    }
    
    trace (TRACE_LEVEL_INFO, "ModifiedManagedObjectSchemaDifference::printContentsForDebugging : Removed Relations are : ");
    
    for (rIterator=m_removedRelations.begin(); rIterator!=m_removedRelations.end(); rIterator++)
    {    
        (*rIterator).printContentsForDebugging ();
    }
    
    trace (TRACE_LEVEL_INFO, "ModifiedManagedObjectSchemaDifference::printContentsForDebugging : Changed Relations are : ");
    
    for (rIterator=m_changedRelations.begin(); rIterator!=m_changedRelations.end(); rIterator++)
    {    
        (*rIterator).printContentsForDebugging ();
    }
}

}

