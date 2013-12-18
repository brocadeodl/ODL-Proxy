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
 *@file AddedRemovedManagedObjectSchemaDifference.cpp 
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 *                    All rights reserved. 
 * Description: This file declares the information regarding the tables added to
 *              the new schema and tables removed from the old schema
 *              
 *               
 * Author :     Aashish Akhouri 
 * Date :       10/20/2011 
 */


#include "Framework/Types/Types.h"
#include "Framework/Utils/TraceUtils.h"
#include <vector>
#include "Framework/ObjectRelationalMapping/RelationshipInfoFromSchemaDifference.h"
#include "Framework/ObjectRelationalMapping/AddedRemovedManagedObjectSchemaDifference.h"

namespace WaveNs
{


AddedRemovedManagedObjectSchemaDifference::AddedRemovedManagedObjectSchemaDifference (const string & tableName)
  :mTableName(tableName)
{
}
AddedRemovedManagedObjectSchemaDifference::~AddedRemovedManagedObjectSchemaDifference ()
{
}


void   AddedRemovedManagedObjectSchemaDifference::addRelations(const RelationshipInfoFromSchemaDifference  &  relationshipInfo)
{
  mRelations.push_back(relationshipInfo);
} 


const vector<RelationshipInfoFromSchemaDifference> &  AddedRemovedManagedObjectSchemaDifference::getRelations() const
{
  return mRelations;
}

const string & AddedRemovedManagedObjectSchemaDifference::getTableName () const
{
    return mTableName;

}
void AddedRemovedManagedObjectSchemaDifference::printContentsForDebugging ()
{
    tracePrintf (TRACE_LEVEL_INFO, false, false, "AddedRemovedManagedObjectSchemaDifference::printContentsForDebugging : Entering for Table Name : %s\n", mTableName.c_str());  
    
    vector<RelationshipInfoFromSchemaDifference>::iterator rIterator;

    for (rIterator=mRelations.begin(); rIterator!=mRelations.end(); rIterator++)
    {    
        (*rIterator).printContentsForDebugging ();
    }
    
}

}


