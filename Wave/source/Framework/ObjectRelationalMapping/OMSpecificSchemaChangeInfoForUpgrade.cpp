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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Pritee Kadu                            					   *
 ***************************************************************************/

#include "Framework/ObjectRelationalMapping/OMSpecificSchemaChangeInfoForUpgrade.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfo.h"
#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

using namespace std;

namespace WaveNs
{

OMSpecificSchemaChangeInfoForUpgrade::OMSpecificSchemaChangeInfoForUpgrade ()
{
}

OMSpecificSchemaChangeInfoForUpgrade::~OMSpecificSchemaChangeInfoForUpgrade ()
{
}

void OMSpecificSchemaChangeInfoForUpgrade::setObjectManagerName (const string& objectManagerName)
{
    m_name = objectManagerName;
}

string OMSpecificSchemaChangeInfoForUpgrade::getObjectManagerName () const
{
    return (m_name);
}

void OMSpecificSchemaChangeInfoForUpgrade::setAddedManagedObjectNames (const vector<string>& managedObjectNames)
{
    m_addedManagedObjectNames = managedObjectNames;
}

vector<string> OMSpecificSchemaChangeInfoForUpgrade::getAddedManagedObjectNames () const
{
    return (m_addedManagedObjectNames);
}

void OMSpecificSchemaChangeInfoForUpgrade::setRemovedManagedObjectNames (const vector<string>& managedObjectNames)
{
    m_removedManagedObjectNames = managedObjectNames;
}

vector<string> OMSpecificSchemaChangeInfoForUpgrade::getRemovedManagedObjectNames () const
{
    return (m_removedManagedObjectNames);
}

void OMSpecificSchemaChangeInfoForUpgrade::setModifiedManagedObjectInfo (const map<string, ModifiedManagedObjectSchemaDifference*>& modifiedManagedObjects)
{
    m_modifiedManagedObjectSchemaInfo = modifiedManagedObjects;
}

map<string, ModifiedManagedObjectSchemaDifference*>& OMSpecificSchemaChangeInfoForUpgrade::getModifiedManagedObjectInfo ()
{
    return (m_modifiedManagedObjectSchemaInfo);
}

/*
vector<string> OMSpecificSchemaChangeInfoForUpgrade::getModifiedManagedObjectNames () const
{
    
}
*/
ModifiedManagedObjectSchemaDifference* OMSpecificSchemaChangeInfoForUpgrade::getDifferenceSchemaInfoForMo (const string& moName) const
{
    map <string, ModifiedManagedObjectSchemaDifference*>::const_iterator iter = m_modifiedManagedObjectSchemaInfo.find (moName);

    if (m_modifiedManagedObjectSchemaInfo.end () == iter)
    {
        trace (TRACE_LEVEL_WARN, "OMSpecificSchemaChangeInfoForUpgrade::getDifferenceInfoForMo: [" + moName + "] is not a modified managedObject. Please, check the same.");
        prismAssert (false, __FILE__, __LINE__);
    }
        
    return (iter->second);    
}

}
