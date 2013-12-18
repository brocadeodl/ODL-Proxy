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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman					   *
 ***************************************************************************/

#include "Framework/ObjectRelationalMapping/DatabaseSchema.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfo.h"
#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Database/DatabaseObjectManagerGetDBSchemaInfoMessage.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

using namespace std;

namespace WaveNs
{

DatabaseSchema::DatabaseSchema ()
    : SerializableObject ()
{
}

DatabaseSchema::~DatabaseSchema ()
{

}

void DatabaseSchema::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeStringVector     (&m_managedObjectNames,                   "managedObjectNames"));
    addSerializableAttribute (new AttributeStringVector     (&m_fieldNamesStrings,                    "fieldNamesStrings"));
    addSerializableAttribute (new AttributeStringVector     (&m_fieldNamesTypes,                      "fieldNamesTypes"));
    addSerializableAttribute (new AttributeUI32Vector       (&m_classIds,                             "classIds"));
    addSerializableAttribute (new AttributeUI32Vector       (&m_parentTableIds,                       "parentTableIds"));
    addSerializableAttribute (new AttributeStringVector     (&m_userDefinedKeyCombinationsFieldNames, "userDefinedKeyCombinationsFieldNames"));
    addSerializableAttribute (new AttributeStringVector     (&m_userDefinedKeyCombinationsFieldTypes, "userDefinedKeyCombinationsFieldTypes"));
    addSerializableAttribute (new AttributeStringVector     (&m_fieldNamesExpandedTypes,              "fieldNamesExpandedTypes"));
    addSerializableAttribute (new AttributeBoolVector       (&m_isLocalManagedObjectInfo,             "isLocalManagedObjectInfo"));
    addSerializableAttribute (new AttributeStringVector     (&m_derivedFromClassNames,                "derivedFromClassNames"));
}

void DatabaseSchema::setupAttributesForSerializationInAttributeOrderFormat ()
{
    SerializableObject::setupAttributesForSerializationInAttributeOrderFormat ();

    addAttributeNameForOrderToNameMapping ("managedObjectNames");
    addAttributeNameForOrderToNameMapping ("fieldNamesStrings");
    addAttributeNameForOrderToNameMapping ("fieldNamesTypes");
    addAttributeNameForOrderToNameMapping ("classIds");
    addAttributeNameForOrderToNameMapping ("parentTableIds");
    addAttributeNameForOrderToNameMapping ("userDefinedKeyCombinationsFieldNames");
    addAttributeNameForOrderToNameMapping ("userDefinedKeyCombinationsFieldTypes");
    addAttributeNameForOrderToNameMapping ("fieldNamesExpandedTypes");
    addAttributeNameForOrderToNameMapping ("isLocalManagedObjectInfo");
    addAttributeNameForOrderToNameMapping ("derivedFromClassNames");
}


/* This function is a static function and can be called from anywhere
 * 
 * The Schema can be obtained from 2 different places,
 * 
 * 1. The MoSchemaInfoManagedObject table in the database. 
 * 2. From the .cfg file using the PrismFrameworkConfiguration 
 *    for backward compatibility since Hydra's MoSchemaInfoManagedObject 
 *    table doesnt contain all the fields required in the schema.
 *
 * AS A FIRST IMPLEMENTATION FOR 'HA' purposes, the implementation takes 
 * as granted that this function is called only after the Lyra firmware boots
 * which will mean that the MoSchemaInfoManagedObject has all the info. 
 *
 *
 */

ResourceId DatabaseSchema::getDatabaseSchema (DatabaseSchema &databaseSchema)
{
    ResourceId status  = FRAMEWORK_SUCCESS;   

    trace (TRACE_LEVEL_INFO, "DatabaseSchema::getDatabaseSchema : Reading from Upgrade Mo from Database.");

    // First send message to DBOM and then if the error is WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED, then try to read upgrade MO thru DbStandaloneTransaction.

    trace (TRACE_LEVEL_INFO, "DatabaseSchema::getDatabaseSchema : Sending message to DBOM to read from upgrade Mo.");
    DatabaseObjectManagerGetDBSchemaInfoMessage message;
    status = WaveObjectManagerToolKit::sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        trace (TRACE_LEVEL_ERROR, "DatabaseSchema::getDatabaseSchema : Could not send message to DatabaseObjectManager to read UpgradeMO. Status : " + FrameworkToolKit::localize (status));
        if (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED == status)
        {
            trace (TRACE_LEVEL_INFO, "DatabaseSchema::getDatabaseSchema : As DatabaseObjectManager is not enabled, try reading Upgrade MO from same OM");
            DatabaseStandaloneTransaction     databaseObject;

            if (0 == databaseObject.getSchemaChangeInfo (databaseSchema))
            {   
                trace (TRACE_LEVEL_ERROR, "DatabaseSchema::getDatabaseSchema : Upgrade Mo has zero entries. DB must be emptied. Erroring out Schema conversion and wii go to First Time Boot.");
                //prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "DatabaseSchema::getDatabaseSchema : successfully read upgradeMO contents using DatabaseStandaloneTransaction.");
                status = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {   
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_ERROR, "DatabaseSchema::getDatabaseSchema :: failed to read upgradeMO. Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {   
            trace (TRACE_LEVEL_INFO, "DatabaseSchema::getDatabaseSchema : successfully read upgradeMO contents.");
            message.getDatabaseSchema (databaseSchema);
        }
    }

    return ((status == WAVE_MESSAGE_SUCCESS)?FRAMEWORK_SUCCESS:FRAMEWORK_ERROR);
}

bool DatabaseSchema::operator== (const DatabaseSchema &rhsDbSchema)
{
    return ( (m_managedObjectNames == rhsDbSchema.getManagedObjectNames ()) &&    
            (m_fieldNamesStrings == rhsDbSchema.getFieldNamesStrings ()) &&    
            (m_fieldNamesTypes == rhsDbSchema.getFieldNamesTypes ()) &&    
            (m_classIds == rhsDbSchema.getClassIds ()) &&    
            (m_parentTableIds == rhsDbSchema.getParentTableIds ()) &&    
            (m_userDefinedKeyCombinationsFieldNames ==
             rhsDbSchema.getUserDefinedKeyCombinationsFieldNames ()) &&    
            (m_userDefinedKeyCombinationsFieldTypes ==
             rhsDbSchema.getUserDefinedKeyCombinationsFieldTypes ()) &&    
            (m_fieldNamesExpandedTypes ==
             rhsDbSchema.getFieldNamesExpandedTypes ()) &&
            (m_isLocalManagedObjectInfo == rhsDbSchema.getIsLocalManagedObjectInfo()) &&
            (m_derivedFromClassNames  == rhsDbSchema.getDerivedFromClassNames ()) );
}


/*
 * In this function there are 2 inputs
 * 1. The calling Object is the starting schema.
 * 2. The 'rhsDbSchema' is the resulting schema.
 *    In the case that this API is used in HA (DB sync from active to standby), 
 *    the calling object should be Active's DB schema and rhsDbSchema should be the StandBy's DB schema. 
 *
 */
bool DatabaseSchema::isConvertibleTo (const DatabaseSchema &rhsDbSchema)
{
    // Convert both Database Schemas into ManagedObjectInfoSchemaRepository and call computeDatabaseSchemaDifferences
    ResourceId status = FRAMEWORK_SUCCESS;
    ManagedObjectSchemaInfoRepository rhsMoSchemaInfoRepository, lhsMoSchemaInfoRepository;

    rhsDbSchema.convertDatabaseSchemaToManagedObjectSchemaInfoRepository (rhsMoSchemaInfoRepository);
    rhsMoSchemaInfoRepository.populateSchemaInfoObjectsWithRelationshipPointers ();

    convertDatabaseSchemaToManagedObjectSchemaInfoRepository (lhsMoSchemaInfoRepository);
    lhsMoSchemaInfoRepository.populateSchemaInfoObjectsWithRelationshipPointers ();

    status = rhsMoSchemaInfoRepository.computeDatabaseSchemaDifferences (lhsMoSchemaInfoRepository);
    if (FRAMEWORK_SUCCESS == status)
    { 
        return true;
    }

    return false;

}

void DatabaseSchema::convertDatabaseSchemaToManagedObjectSchemaInfoRepository (ManagedObjectSchemaInfoRepository &dBSchemaRepository) const
{
    vector<string>  managedObjectNames                      = getManagedObjectNames ();
    vector<string>  managedObjectFieldNames                 = getFieldNamesStrings ();
    vector<string>  managedObjectFieldTypes                 = getFieldNamesTypes ();
    vector<UI32>    classIds                                = getClassIds ();
    vector<UI32>    parentClassIds                          = getParentTableIds ();
    vector<string>  userDefinedKeyCombinationsFieldNames    = getUserDefinedKeyCombinationsFieldNames ();
    vector<string>  userDefinedKeyCombinationsFieldTypes    = getUserDefinedKeyCombinationsFieldTypes ();
    vector<string>  managedObjectFieldExpandedTypesStrings  = getFieldNamesExpandedTypes ();
    vector<bool>    isLocalManagedObjectInfo                = getIsLocalManagedObjectInfo ();
    vector<string>  derivedFromClassNames                   = getDerivedFromClassNames ();


    vector<string>  fieldTypesStrings;
    const UI32      numTables = managedObjectNames.size ();

    for (UI32 i = 0; i < numTables; ++i)
    {
        ManagedObjectSchemaInfo *pSchemaInfoObj = new CompositeManagedObjectSchemaInfo (managedObjectNames[i], classIds[i], parentClassIds[i]);

        if (0 != isLocalManagedObjectInfo.size () && 0 != derivedFromClassNames.size())
        {    
            pSchemaInfoObj->setIsALocalManagedObject (isLocalManagedObjectInfo[i]);
            pSchemaInfoObj->setDerivedFromClassName (derivedFromClassNames[i]);
        }

        vector<string> fieldNames;
        vector<string> fieldTypes;
        StringUtils::tokenizeStringWithCount (managedObjectFieldNames[i], fieldNames);

        if (!managedObjectFieldExpandedTypesStrings.size ())
        {   
            fieldTypesStrings = managedObjectFieldTypes;
        }
        else
        {
            fieldTypesStrings = managedObjectFieldExpandedTypesStrings;
        }

        StringUtils::tokenizeStringWithCount (fieldTypesStrings[i], fieldTypes);

        for (size_t j = 0; j < fieldNames.size (); ++j)
        {
            tracePrintf (TRACE_LEVEL_DEBUG, true, false, "fieldNames = %s, fieldTypes = %s", fieldNames[j].c_str (), fieldTypes[j].c_str ());
            pSchemaInfoObj->addFieldTypeTuple (fieldNames[j], fieldTypes[j]);
        }

        if (userDefinedKeyCombinationsFieldNames.size () > 0)
        {   
            vector<string> userDefinedFieldNames;
            vector<string> userDefinedFieldTypes;

            StringUtils::tokenizeStringWithCount (userDefinedKeyCombinationsFieldNames[i], userDefinedFieldNames);
            StringUtils::tokenizeStringWithCount (userDefinedKeyCombinationsFieldTypes[i], userDefinedFieldTypes);

            for (size_t k = 0; k < userDefinedFieldNames.size (); ++k)
            {
                pSchemaInfoObj->addUserDefinedFieldTypeTuple (userDefinedFieldNames[k], userDefinedFieldTypes[k]);
            }
        }

        dBSchemaRepository.addSchemaInfoObject (pSchemaInfoObj);
//          tableClassIdTuples.push_back (pair<string, UI32>(managedObjectNames[i], classIds[i]));                                                                                 
    }
}

void  DatabaseSchema::setManagedObjectNames   (const vector<string> managedObjectNames)
{
    m_managedObjectNames = managedObjectNames;
}

vector<string>  DatabaseSchema::getManagedObjectNames   () const
{
    return (m_managedObjectNames);
}

void DatabaseSchema::setFieldNamesStrings    (const vector<string> fieldNamesStrings)
{
    m_fieldNamesStrings = fieldNamesStrings;
}
    
vector<string>  DatabaseSchema::getFieldNamesStrings    () const
{
    return (m_fieldNamesStrings);
}

void DatabaseSchema::setFieldNamesTypes      (const vector<string> fieldNamesTypes)
{
    m_fieldNamesTypes  = fieldNamesTypes; 
}

vector<string>  DatabaseSchema::getFieldNamesTypes      () const
{
    return (m_fieldNamesTypes);
}
                
void            DatabaseSchema::setClassIds             (const vector<UI32> classIds)
{
    m_classIds = classIds;
}

vector<UI32>    DatabaseSchema::getClassIds             () const
{
    return (m_classIds);
}

void            DatabaseSchema::setParentTableIds       (const vector<UI32> parentTableIds)
{
    m_parentTableIds = parentTableIds;
}

vector<UI32>    DatabaseSchema::getParentTableIds       () const
{
    return (m_parentTableIds);
}
                
void DatabaseSchema::setUserDefinedKeyCombinationsFieldNames (const vector<string> userDefinedKeyCombinationsFieldNames)
{
    m_userDefinedKeyCombinationsFieldNames = userDefinedKeyCombinationsFieldNames;
}

vector<string>  DatabaseSchema::getUserDefinedKeyCombinationsFieldNames () const
{
    return (m_userDefinedKeyCombinationsFieldNames);
}

void DatabaseSchema::setUserDefinedKeyCombinationsFieldTypes (const vector<string> userDefinedKeyCombinationsFieldTypes)
{
    m_userDefinedKeyCombinationsFieldTypes = userDefinedKeyCombinationsFieldTypes;
}

vector<string>  DatabaseSchema::getUserDefinedKeyCombinationsFieldTypes () const
{
    return (m_userDefinedKeyCombinationsFieldTypes); 
}
                
void DatabaseSchema::setFieldNamesExpandedTypes  (const vector<string> fieldNamesExpandedTypes)
{
    m_fieldNamesExpandedTypes = fieldNamesExpandedTypes; 
}

vector<string>  DatabaseSchema::getFieldNamesExpandedTypes  () const
{
    return (m_fieldNamesExpandedTypes);
}

void DatabaseSchema::setDerivedFromClassNames (const vector<string> derivedFromClassNames)
{
    m_derivedFromClassNames = derivedFromClassNames; 
}

vector<string>  DatabaseSchema::getDerivedFromClassNames  () const
{
    return (m_derivedFromClassNames);
}

void DatabaseSchema::setIsLocalManagedObjectInfo (const vector<bool> isLocalManagedObjects)
{
    m_isLocalManagedObjectInfo = isLocalManagedObjects; 
}

vector<bool>  DatabaseSchema::getIsLocalManagedObjectInfo  () const
{
    return (m_isLocalManagedObjectInfo);
}

}
