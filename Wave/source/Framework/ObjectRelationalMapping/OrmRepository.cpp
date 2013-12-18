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
#include "Framework/Types/Types.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeSI64.h"
#include "Framework/Attributes/AttributeSI64Vector.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeDate.h"
#include "Framework/Attributes/AttributeTime.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Framework/Attributes/AttributeDecimal64.h"
#include "Framework/Attributes/AttributeMACRange.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeSnmpObjectId.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/Attributes/AttributeLargeObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"
#include "Framework/Attributes/AttributeUI32VectorUC.h"
#include "Framework/Attributes/AttributeStringVectorUC.h"
#include "Framework/Attributes/AttributeHostUC.h"
#include "Framework/Attributes/AttributeUI64UC.h"
#include "Framework/Attributes/AttributeSqlBetween.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include "Framework/Attributes/AttributeUUID.h"
#include "Framework/Attributes/AttributeUri.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"

#include <algorithm>

namespace WaveNs
{

const string OrmRepository::m_waveCurrentSchema = "WaveCurrent";
const string OrmRepository::m_waveStartSchema = "WaveStart";

static set<string> s_allCompostionTableNames;

OrmRepository::OrmRepository ()
{

    //trace (TRACE_LEVEL_INFO, "OrmRepository::OrmRepository : Calling build functions in OrmRepository Constructor ");
    buildAttributeTypeToSqlTypeMap ();
    buildAttributeSupportedConversionsMap ();

    MultiByteWord tempPolynomial;
    tempPolynomial.setPolynomialDegree (30);
    tempPolynomial[0] = 0x30185CE3;

    m_z2.setCurrentIrreduciblePolynomial (tempPolynomial);
}

OrmRepository::~OrmRepository ()
{
    UI32 numberOfTables = m_tables.size ();
    UI32 numberOfViews  = m_views.size ();
    UI32 i              = 0;

    for (i = 0; i < numberOfTables; i++)
    {
        delete m_tables[i];
    }

    m_tables.clear ();

    for (i = 0; i < numberOfViews; i++)
    {
        delete m_views[i];
    }

}

OrmRepository *OrmRepository::getInstance ()
{
    static OrmRepository *pOrmRepository = new OrmRepository ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    return (pOrmRepository);
}

bool OrmRepository::isManagedClassAView (const string &managedClass)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    map<string, OrmView *>::iterator element1    = pOrmRepository->m_viewsMapByName.find (managedClass);
    map<string, OrmView *>::iterator element1End = pOrmRepository->m_viewsMapByName.end ();

    if (element1End != element1)
    {
        return true;
    }
    return false;
}

bool OrmRepository::isManagedClassATable (const string &managedClass)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    map<string, OrmTable *>::iterator  element1      = pOrmRepository->m_tablesMapByName.find (managedClass);
    map<string, OrmTable *>::iterator  element1End   = pOrmRepository->m_tablesMapByName.end ();

    if (element1End != element1)
    {
        return true;
    }
    return false;
}

void OrmRepository::addView (OrmView *pOrmView)
{
    PrismMutex s_addViewMutex;

    s_addViewMutex.lock ();

    //UI32   nextTableId = getNextTableId ();
    string viewName   = pOrmView->getName ();

    map<string, OrmView *>::iterator element1    = m_viewsMapByName.find (viewName);
    map<string, OrmView *>::iterator element1End = m_viewsMapByName.end ();

    if (element1End != element1)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::addView : Duplicate View Entry for " + viewName);
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    map<string, OrmTable *>::iterator element2    = m_tablesMapByName.find (viewName);
    map<string, OrmTable *>::iterator element2End = m_tablesMapByName.end ();

    if (element2End != element2)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::addView : Table Entry found for " + viewName + ". Tables and view names should be unique");
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_views.push_back (pOrmView);
    m_viewsMapByName[viewName] = pOrmView;

    s_addViewMutex.unlock ();
}

void OrmRepository::addTable (OrmTable *pOrmTable)
{
    PrismMutex s_addTableMutex;

    s_addTableMutex.lock ();

    //UI32   nextTableId = getNextTableId ();
    string tableName   = pOrmTable->getName ();
    UI32   nextTableId = getUniqueTableIdForTableName (tableName);

    pOrmTable->setTableId    (nextTableId);
    //pOrmTable->setOldTableId (nextTableId);

    map<string, OrmTable *>::iterator element1    = m_tablesMapByName.find (tableName);
    map<string, OrmTable *>::iterator element1End = m_tablesMapByName.end ();

    if (element1End != element1)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::addTable : Duplicate Table Entry for " + tableName);
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    map<UI32, OrmTable *>::iterator element2    = m_tablesMapById.find (nextTableId);
    map<UI32, OrmTable *>::iterator element2End = m_tablesMapById.end ();

    if (element2End != element2)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::addTable : Duplicate Table Id Entry for " + tableName + ", Table ID : " + nextTableId);
        trace (TRACE_LEVEL_FATAL, "OrmRepository::addTable : Please try using different table-name even though it is not repeated: " + nextTableId);
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    map<string, OrmView *>::iterator element3    = m_viewsMapByName.find (tableName);
    map<string, OrmView *>::iterator element3End = m_viewsMapByName.end ();

    if (element3End != element3)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::addTable : View Entry found for " + tableName + ". Tables and view names should be unique");
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_tables.push_back (pOrmTable);
    m_tablesMapById[nextTableId] = pOrmTable;
    m_tablesMapByName[tableName] = pOrmTable;

    s_addTableMutex.unlock ();
}

void  OrmRepository::generateSqlToCreateViews (string & sqlForCreatingViews)
{
    UI32           numberOfViews = m_views.size();

    for (UI32 i = 0; i < numberOfViews; i++)
    {
        sqlForCreatingViews += (m_views[i])->getSqlForCreateView ();
    }

    trace (TRACE_LEVEL_DEBUG, string ("SQL to create view : ") + sqlForCreatingViews);
}

void  OrmRepository::generateSqlToPopulateUpgradeTable(string & sqlForUpgradeTablePopulation,const string & waveSchema)
{
    UI32 numberOfTables = m_managedObjectNamesForSchemaChange.size();

    // 1. iterate through all the objects in the schemaChange vectors maintained by OrmRepository.
    // 2. For each object, tokenize the fields like field names, types, extended types etc. to form the vector
    // 3. initialize these to AttributeStringVector as need to be pushed to the database. 
    // 4. form the sql using toEscapedString.
    for (UI32 i = 0; i < numberOfTables; i++)
    {
        string tableName                    = m_managedObjectNamesForSchemaChange[i];
        string fieldNamesString             = m_fieldNamesStringsForSchemaChange[i];
        string fieldTypesString             = m_fieldNamesTypesForSchemaChange[i];
        UI32   tableClassId                 = m_classIds[i];
        UI32   parentTableId                = m_parentTableIds[i];
        string userDefinedFieldNamesString  = m_userDefinedKeyCombinationsFieldNamesForSchemaChange[i];
        string userDefinedFieldTypesString  = m_userDefinedKeyCombinationsFieldTypesForSchemaChange[i];
        string extendedFiledTypesString     = m_fieldNamesExpandedTypesForSchemaChange[i];
        string derivedFromClassName         = m_derivedFromClassNameForSchemaChange [i];
        bool   isALocalMo                   = m_isALocalManagedObjectInfoForSchemaChange[i];
        string localManagedObjectInfo       = (true == isALocalMo) ? "true" : "false";

        tracePrintf (TRACE_LEVEL_DEBUG, true, false, "OrmRepository::generateSqlToPopulateUpgradeTable: table:%s, classid: %u, parentClassid:%u localManagedObjectInfo:%s, derivedFromClassName:%s", tableName.c_str(), tableClassId, parentTableId, localManagedObjectInfo.c_str(), derivedFromClassName.c_str());

        vector<string> fieldNames;
        vector<string> fieldTypes;
        vector<string> userDefinedFieldNames;
        vector<string> userDefinedFieldTypes;
        vector<string> extendedFieldTypes;

        tokenize (fieldNamesString, fieldNames, ':');
        tokenize (fieldTypesString, fieldTypes, ':');
        tokenize (userDefinedFieldNamesString, userDefinedFieldNames, ':');
        tokenize (userDefinedFieldTypesString, userDefinedFieldTypes, ':');
        tokenize (extendedFiledTypesString, extendedFieldTypes, ':');
        
        AttributeStringVector fieldNameAttr (fieldNames);
        AttributeStringVector fieldTypeAttr (fieldTypes);
        AttributeStringVector userDefinedFieldNameAttr (userDefinedFieldNames);
        AttributeStringVector userDefinedFieldTypeAttr (userDefinedFieldTypes);
        AttributeStringVector extendedFieldTypeattr (extendedFieldTypes);

        fieldNameAttr.toEscapedString (fieldNamesString);
        fieldTypeAttr.toEscapedString (fieldTypesString);
        userDefinedFieldNameAttr.toEscapedString (userDefinedFieldNamesString);
        userDefinedFieldTypeAttr.toEscapedString (userDefinedFieldTypesString);
        extendedFieldTypeattr.toEscapedString (extendedFiledTypesString);

        sqlForUpgradeTablePopulation += "INSERT INTO " + waveSchema + "." + getSchemaUpgradeTableName ();
        sqlForUpgradeTablePopulation += string(" (objectidinstanceid,classname,fieldnames,fieldtypes,classid,parentclassid,expandedfieldtypes,userdefinedfieldnames,userdefinedfieldtypes,isalocalmanagedobject, derivedfromclassname) VALUES (" );
        sqlForUpgradeTablePopulation += i + string(",");
        sqlForUpgradeTablePopulation += string("'") + tableName + string("',");
        sqlForUpgradeTablePopulation += string("'") + fieldNamesString + string("',");
        sqlForUpgradeTablePopulation += string("'") + fieldTypesString + string("',");
        sqlForUpgradeTablePopulation += tableClassId + string(",");
        sqlForUpgradeTablePopulation += parentTableId + string (",");
        sqlForUpgradeTablePopulation += string("'") + extendedFiledTypesString + string("',");
        sqlForUpgradeTablePopulation += string("'") + userDefinedFieldNamesString + string("',");
        sqlForUpgradeTablePopulation += string("'") + userDefinedFieldTypesString + string("',"); 
        sqlForUpgradeTablePopulation += string("'") + localManagedObjectInfo + string("',"); 
        sqlForUpgradeTablePopulation += string("'") + derivedFromClassName + string("');");
    }

    trace (TRACE_LEVEL_DEBUG, string ("SQL to populate upgradeMO: ") + sqlForUpgradeTablePopulation);
}

string OrmRepository::generateSchema (const string &waveSchema)
{
    UI32           numberOfTables     = m_tables.size ();
    UI32           i                  = 0;
    string         sqlForCreate;
    vector<string> references;
    UI32           numberOfReferences = 0;

    // Get the unique destinations in reference statements (foreign key constraint destinations)

    for (i = 0; i < numberOfTables; i++)
    {
        m_tables[i]->getReferences (references);
    }

    numberOfReferences = references.size ();

    trace (TRACE_LEVEL_INFO, string ("Total Number of unique REFERNCES destinations  : ") + numberOfReferences);

    sqlForCreate = "CREATE SCHEMA " + waveSchema + ";\n";

    // Generate SQL for tables representing the derivations for those references.

    for (i = 0; i < numberOfReferences; i++)
    {
        sqlForCreate += getSchemaForDerivationsTable (references[i], waveSchema) + "\n";
    }

    // Generate Sql for all the tables.

    for (i = 0; i < numberOfTables; i++)
    {
        sqlForCreate += (m_tables[i])->getSqlForCreate (waveSchema);
    }

    // Generate SQL for tables being refered for unique constraint on objectIdInstanceId.

    for (i = 0; i < numberOfReferences; i++)
    {
        sqlForCreate += getSchemaForReferenceTableUniqueConstraint (references[i], waveSchema) + "\n";
    }

    for (i = 0; i < numberOfTables; i++)
    {
        sqlForCreate += (m_tables[i])->getSqlForCreate2 (waveSchema);
    }

    // Generate SQL to populate all the derivations tables for the references.

    for (i = 0; i < numberOfReferences; i++)
    {
        UI32                numberOfDerivations = 0;
        UI32                j                   = 0;
        vector<OrmTable *>  derivedTables;
        OrmTable           *pTable              = NULL;
        string              referenceName;

        referenceName = references[i];
        pTable        = m_tablesMapByName[referenceName];

        if (NULL == pTable)
        {
            trace (TRACE_LEVEL_FATAL, "OrmRepository::generateSchema : Trying to get derived tables for non existant table : " + references[i]);
            prismAssert (NULL != pTable, __FILE__, __LINE__);
        }
        else
        {
            pTable->getAllDerivedTableIds (derivedTables);

            numberOfDerivations = derivedTables.size ();

            // Add self first and then add all the derivations.

            sqlForCreate += getSqlForDerivedTable (pTable->getName (), pTable->getTableId (), waveSchema) + "\n";

            for (j = 0; j < numberOfDerivations; j++)
            {
                sqlForCreate += getSqlForDerivedTable (pTable->getName (), derivedTables[j]->getTableId (), waveSchema) + "\n";
            }
        }
    }

    string sqlForUpgradeTablePopulation;
    generateSqlToPopulateUpgradeTable(sqlForUpgradeTablePopulation, waveSchema);
    sqlForCreate += sqlForUpgradeTablePopulation;

    string sqlForCreatingViews;
    generateSqlToCreateViews (sqlForCreatingViews);
    sqlForCreate += sqlForCreatingViews;

    return (sqlForCreate);
}

UI32 OrmRepository::getUniqueTableIdForTableName (const string &tableName)
{
    MultiByteWord fingerPrintValue;

    m_z2.computeFingerPrint (fingerPrintValue, tableName);
    
    string tableIdString = fingerPrintValue.toHexString();

    UI32 uniqueTableId = strtoul (tableIdString.c_str(), NULL, 16);

    if ((0 == uniqueTableId) || (0 > (SI32)uniqueTableId))
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::getUniqueTableIdForTableName : table-id returned is a greater than required range or strtoul has failed. uniqueTableId returned : " + uniqueTableId);
        prismAssert (false, __FILE__, __LINE__);
    }

    trace (TRACE_LEVEL_DEBUG, "OrmRepository::getUniqueTableIdForTableName : className \'" + tableName + "\' uniqueTableId :" + uniqueTableId);
    return uniqueTableId;    
}

UI32 OrmRepository::getNextTableId ()
{
    static PrismMutex s_mutex;
    static UI32       s_nextTableId = 1;

           UI32       tempTableId   = 0;

    s_mutex.lock ();

    tempTableId = s_nextTableId;
    s_nextTableId++;

    s_mutex.unlock ();

    return (tempTableId);
}

bool OrmRepository::getValueForDisableValidations (OrmTable *pOrmTable, const string &fieldName, const string &fieldType)
{
    /* Check if fieldType is a 1-n relationship
     * If true then get the Attribute* from pOrmTable and get the value of disableValidations and return
     */
    if ((fieldType[1] == '-' && fieldType[3] == '-'))
    {
        if (fieldType[2] == 'm')
        {
            return (pOrmTable->getValueForDisableValidations (fieldName));
        }
    }

    return (false);
}

bool OrmRepository::getValueForCanBeEmpty (OrmTable *pOrmTable, const string &fieldName, const string &fieldType)
{
    /* Check if fieldType is a 1-1 relationship
     * If true then get the Attribute* from pOrmTable and get the value of canBeEmpty and return
     */

    if ((fieldType[1] == '-' && fieldType[3] == '-'))
    {
        if (fieldType[2] == '1')
        {
            return (pOrmTable->getValueForCanBeEmpty (fieldName));
        }
    }

    return (false);
}

//void OrmRepository::buildMoSchemaRepository (const bool updateTableIdsFromUpgradeMO)
void OrmRepository::buildMoSchemaRepository ()
{
/*
    // if updateTableIdsFromUpgradeMO=true, then update the table-ids from the Upgrade MO.
    if (updateTableIdsFromUpgradeMO)
    {
        trace (TRACE_LEVEL_INFO, "OrmRepository::buildMoSchemaRepository: updating table-ids for Orm form Upgrade MO.");
        DatabaseStandaloneTransaction dbStandaloneTransaction;
        vector<pair<string, UI32> >   tableIds;          
    
        UI32 numOfTablesFromUpgradeMO = dbStandaloneTransaction.getTableIdsFromUpgradeMO (tableIds);

        if (numOfTablesFromUpgradeMO != m_managedObjectNamesForSchemaChange.size())
        {
            tracePrintf (TRACE_LEVEL_INFO, true, false, "OrmRepository::buildMoSchemaRepository: #tables in Orm does not match with that in UpgradeMO in database. db : %u, orm : %u", numOfTablesFromUpgradeMO, m_managedObjectNamesForSchemaChange.size());
            //prismAssert (false, __FILE__, __LINE__);
        }
        updateTableId (tableIds);
    }
*/
    
    UI32      numberOfTables  = m_tables.size ();
    ManagedObjectSchemaInfoRepository &moUpgradeRepository = getMOSchemaInfoRepository ();

    m_managedObjectNamesForSchemaChange.clear ();
    m_classIds.clear();
    m_parentTableIds.clear();
    m_fieldNamesStringsForSchemaChange.clear();
    m_fieldNamesTypesForSchemaChange.clear();
    m_userDefinedKeyCombinationsFieldNamesForSchemaChange.clear ();
    m_userDefinedKeyCombinationsFieldTypesForSchemaChange.clear ();
    m_fieldNamesExpandedTypesForSchemaChange.clear ();
    m_isALocalManagedObjectInfoForSchemaChange.clear ();
    m_derivedFromClassNameForSchemaChange.clear ();

    for (UI32 i = 0; i < numberOfTables; i++)
    {
        OrmTable *pOrmTable = m_tables[i];
        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        UI32 parentTableId = (pOrmTable->getPParentTable()) ? (pOrmTable->getPParentTable ()->getTableId ()) : 0;
        string strFieldNames, strFieldTypes, strFieldExpandedTypes, dataType;
        bool disableValidations, canBeEmpty;

        const vector<string>& fields = pOrmTable->getFieldNamesForUpgrade ();
        const vector<string>& types  = pOrmTable->getFieldTypesForUpgrade ();
        const vector<string>& dbtypes = pOrmTable->getDbFieldTypesForUpgrade ();

        ManagedObjectSchemaInfo *pSchemaInfoObj = new CompositeManagedObjectSchemaInfo (pOrmTable->getName (), pOrmTable->getTableId (), parentTableId);

        pSchemaInfoObj->setIsALocalManagedObject(pOrmTable->getIsALocalManagedObject());
        pSchemaInfoObj->setDerivedFromClassName (pOrmTable->getDerivedFromClassName());

        prismAssert (fields.size () == types.size (), __FILE__, __LINE__);
        prismAssert (fields.size () == dbtypes.size (), __FILE__, __LINE__);

        for (size_t idx = 0; idx < fields.size (); ++idx)
        {
            // For each field, get the value of "disableValidations" and "canBeEmpty" by calling helper functions
            disableValidations   = getValueForDisableValidations (pOrmTable, fields[idx], types[idx]);
            canBeEmpty           = getValueForCanBeEmpty         (pOrmTable, fields[idx], types[idx]);
            
            dataType = types[idx] + "|" + dbtypes[idx] + "|" + disableValidations + "|" + canBeEmpty;
            
            pSchemaInfoObj->addFieldTypeTuple (fields[idx], dataType);

            strFieldNames += fields[idx] + ":";
            strFieldTypes += types[idx] + ":";
            strFieldExpandedTypes += dataType + ":";
        }
        
        // For User Defined Key Combinations 
        string strUserDefinedFieldNames, strUserDefinedFieldTypes;
        const vector<string> & userDefinedFields = pOrmTable->getUserDefinedFieldNamesForUpgrade (); 
        const vector<string> & userDefinedTypes  = pOrmTable->getUserDefinedFieldTypesForUpgrade (); 
        
        prismAssert (userDefinedFields.size () == userDefinedTypes.size (), __FILE__, __LINE__);
        for (size_t uidx = 0; uidx < userDefinedFields.size (); ++uidx)
        {
            pSchemaInfoObj->addUserDefinedFieldTypeTuple (userDefinedFields[uidx], userDefinedTypes[uidx]);

            strUserDefinedFieldNames += userDefinedFields[uidx] + ":";
            strUserDefinedFieldTypes += userDefinedTypes[uidx] + ":";
        }
        
        trace (TRACE_LEVEL_DEVEL, "OrmRepository::buildMoSchemaRepository : Added class \'" + pOrmTable->getName () + "\' parent id " + parentTableId + " to MO upgrade repository with " + fields.size () + " fields");

        m_managedObjectNamesForSchemaChange.push_back (m_tables[i]->getName ());
        m_classIds.push_back (m_tables[i]->getTableId());
        m_parentTableIds.push_back (parentTableId);
        m_fieldNamesStringsForSchemaChange.push_back (strFieldNames);
        m_fieldNamesTypesForSchemaChange.push_back (strFieldTypes);
        m_userDefinedKeyCombinationsFieldNamesForSchemaChange.push_back (strUserDefinedFieldNames);
        m_userDefinedKeyCombinationsFieldTypesForSchemaChange.push_back (strUserDefinedFieldTypes);
        m_fieldNamesExpandedTypesForSchemaChange.push_back (strFieldExpandedTypes);
        m_isALocalManagedObjectInfoForSchemaChange.push_back (pOrmTable->getIsALocalManagedObject());
        m_derivedFromClassNameForSchemaChange.push_back (pOrmTable->getDerivedFromClassName());

        moUpgradeRepository.addSchemaInfoObject (pSchemaInfoObj);
    }

    moUpgradeRepository.populateSchemaInfoObjectsWithRelationshipPointers ();

}

void OrmRepository::computeTableSpace ()
{
    UI32      numberOfTables  = m_tables.size ();
    UI32      i               = 0;
    UI32      j               = 0;
    OrmTable *pOrmTable       = NULL;
    OrmTable *pOrmParentTable = NULL;
    string    derivedFrom;

    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        pOrmTable->setUpgradeStrings ();
        derivedFrom = pOrmTable->getDerivedFromClassName ();

        if (("" != derivedFrom) && (false == (OrmRepository::isAMostBaseClass (derivedFrom))))
        {
            pOrmParentTable = m_tablesMapByName[derivedFrom];

            if (NULL == pOrmParentTable)
            {
                trace (TRACE_LEVEL_FATAL, "OrmRepository::computeTableSpace : Could not find parent table " + derivedFrom + " for table " + pOrmTable->getName ());
                prismAssert (false, __FILE__, __LINE__);
            }

            pOrmTable->setPParentTable (pOrmParentTable);
        }

        trace (TRACE_LEVEL_DEVEL, string ("") + pOrmTable->getTableId () + string (" : ") + pOrmTable->getName () + string (" -> ") + pOrmTable->getDerivedFromClassName ());
    }

    // We must do the following ONLY after the above is completed.

    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        pOrmTable->setPMostBaseTable (getMostBaseTable (pOrmTable->getName ()));
    }

    // For each table compute the derivationsinstances tables required

    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        pOrmTable->computeDerivationsInstancesTables ();
    }

    // For each table validate the UML relations

    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        for (j = 0; j < numberOfTables; j++)
        {
            if (pOrmTable->getName () != (m_tables[j])->getName ())
            {
                pOrmTable->validateRelations (m_tables[j]);
            }
        }
    }
    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        vector<string>             relatedToTables;
        vector<OrmRelationType>    relationTypes;
        vector<OrmRelationUmlType> relationUmlTypes;

        pOrmTable->getDetailsForRelationships (relatedToTables, relationTypes, relationUmlTypes);

        UI32 numberOfRelations = relatedToTables.size ();

        for (UI32 k = 0; k < numberOfRelations; k++)
        {
             if (ORM_RELATION_UML_TYPE_COMPOSITION == relationUmlTypes[k])
             {
                s_allCompostionTableNames.insert (relatedToTables [k]);
             }
        }
    }

    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        vector<string> vectorOfEntityNamesInHierarhy;
        pOrmTable->getAllEntityNamesInHierarchy (vectorOfEntityNamesInHierarhy);
        
        set<string>    setOfEntityNamesInHierarhy (vectorOfEntityNamesInHierarhy.begin (), vectorOfEntityNamesInHierarhy.end ());

        if (vectorOfEntityNamesInHierarhy.size () != setOfEntityNamesInHierarhy.size ())
        {
            trace (TRACE_LEVEL_FATAL, "OrmRepository::computeTableSpace: The OrmTable " + pOrmTable->getName () + " has duplicate columns in its hierarchy. The column names are: ");
            UI32 entityNameIndex = 0;
            for (entityNameIndex = 0 ; entityNameIndex < vectorOfEntityNamesInHierarhy.size (); entityNameIndex++)
            {
                trace (TRACE_LEVEL_INFO, "OrmRepository::computeTableSpace: columnName : " + vectorOfEntityNamesInHierarhy[entityNameIndex]);
            }
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void OrmRepository::getSqlToCountNumberOfEntriesInTable  (const string &tableName, const string &waveSchema, string &sqlString)
{
    sqlString  = "select count (*) from " + waveSchema + "." + tableName + ";";
}

string OrmRepository::getSqlToRenameSchema (const string &schemaName, const string &newSchemaName)
{
    return ("ALTER SCHEMA " + schemaName + " RENAME TO " + newSchemaName + ";");
}

string OrmRepository::getSchemaForDerivationsTable (const string &tableName, const string &waveSchema)
{
    string schemaForDerivationsTable =  "CREATE TABLE " + waveSchema + "." + tableName + "Derivations\n(\n    objectIdClassId integer PRIMARY KEY\n);";

    schemaForDerivationsTable += "CREATE TABLE " + waveSchema + "." + tableName + "DerivationsInstances\n(\n   objectIdInstanceId bigint PRIMARY KEY\n);";

    return (schemaForDerivationsTable);
}

string OrmRepository::getSchemaForReferenceTableUniqueConstraint (const string &tableName, const string &waveSchema)
{
    //return ("ALTER TABLE " + waveSchema + "." + tableName + " ADD CONSTRAINT " + tableName + "UniqueObjectIdInstanceId UNIQUE (ObjectIdInstanceId);");
    return ("");
}

string OrmRepository::getSqlToDeleteAllEntriesFromDerivationsTable (const string &tableName, const string &waveSchema)
{
    return ("DELETE FROM " + waveSchema + "." + tableName + "Derivations; DELETE FROM " + waveSchema + "." + tableName + "DerivationsInstances;");
}

string OrmRepository::getSqlToTruncateFromDerivationsTable (const string &tableName, const string &waveSchema)
{
    return ("TRUNCATE " + waveSchema + "." + tableName + "Derivations CASCADE; TRUNCATE " + waveSchema + "." + tableName + "DerivationsInstances CASCADE;");
}

string OrmRepository::getSqlToDeleteDerivationInstancesTable (const string &tableName, const string &waveSchema)
{
    return ("DELETE FROM " + waveSchema + "." + tableName + "DerivationsInstances;");
}

string OrmRepository::getSqlToQueryAllEntriesFromDerivationsTable (const string &tableName, const string &sourceSchema)
{
    return ("SELECT * FROM " + sourceSchema + "." + tableName + "DerivationsInstances;");
}

string OrmRepository::getSqlToInsertEntriesIntoDerivationsTable (const string &tableName, const string &destSchema, const string &values)
{
    return ("INSERT INTO " + destSchema + "." + tableName + "DerivationsInstances VALUES (" + values + ");");
}

string OrmRepository::getSqlToCopyDerivationsTable (const string &tableName, const string &srcSchema, const string &destSchema)
{
    return ("INSERT INTO " + destSchema + "." + tableName + "Derivations SELECT * FROM " + srcSchema + "." + tableName + "Derivations; INSERT INTO " + destSchema + "." + tableName + "DerivationsInstances SELECT * FROM " + srcSchema + "." + tableName + "DerivationsInstances;");
}

string OrmRepository::getSqlToTruncateTable (const string &tableName, const string &sourceSchema)
{
    return (getTableByName(tableName)->getSqlToTruncate(sourceSchema));
}

string OrmRepository::getSqlToDeleteAllEntriesFromTable (const string &tableName, const string &sourceSchema)
{
    return (getTableByName(tableName)->getSqlToDeleteAllEntries(sourceSchema));
}

string OrmRepository::getSqlToQueryAllEntriesFromTable (const string &tableName, const string &sourceSchema)
{
    return (getTableByName(tableName)->getSqlToQueryAllEntries(sourceSchema));
}

string OrmRepository::getSqlToCopyTable (const string &tableName, const string &srcSchema, const string &destSchema)
{
    return (getTableByName(tableName)->getSqlToCopy (srcSchema, destSchema));
}

string OrmRepository::getSqlToInsertEntriesIntoTable (const string &tableName, const string &destSchema, const string &values)
{
    return (getTableByName(tableName)->getSqlToInsertEntries (destSchema, values));
}

string OrmRepository::getSqlToDropDerivationsTable (const string &tableName, const string &waveSchema)
{
    return ("DROP TABLE " + waveSchema + "." + tableName + "Derivations; DROP TABLE " + waveSchema + "." + tableName + "DerivationsInstances;");
}

string OrmRepository::getSqlToDeleteAllEntriesFromAuxilliaryTable (const string &tableName, const string &waveSchema)
{
    return ("DELETE FROM " + waveSchema + "." + tableName + ";");
}

string OrmRepository::getSqlToRemoveEntriesFromAuxilliaryTable (const string &tableName, const UI32 classid, const string &waveSchema)
{
    return ("DELETE FROM " + waveSchema + "." + tableName + " WHERE ownerclassid='" + classid + "';");
}

string OrmRepository::getSqlToTruncateFromAuxilliaryTable (const string &tableName, const string &waveSchema)
{
    return ("TRUNCATE " + waveSchema + "." + tableName + ";");
}

string OrmRepository::getSqlToQueryAllEntriesFromAuxilliaryTable (const string &tableName, const string &sourceSchema)
{
    return ("SELECT * FROM " + sourceSchema + "." + tableName + ";");
}

string OrmRepository::getSqlToInsertEntriesIntoAuxilliaryTable (const string &tableName, const string &destSchema, const string &values)
{
    return ("INSERT INTO " + destSchema + "." + tableName + " VALUES (" + values + ");");
}

string OrmRepository::getSqlToCopyAuxilliaryTable (const string &tableName, const string &srcSchema, const string &destSchema)
{
    return ("INSERT INTO " + destSchema + "." + tableName + " SELECT * FROM " + srcSchema + "." + tableName + ";");
}

string OrmRepository::getSqlToDropAuxilliaryTable (const string &tableName, const string &waveSchema)
{
    return ("DROP TABLE " + waveSchema + "." + tableName + ";");
}

string OrmRepository::getSqlForDerivedTable (const string &tableName, const UI32 &tableId, const string &waveSchema)
{
    return (string ("INSERT INTO ") + waveSchema + "." + tableName + string ("Derivations VALUES (") + tableId + string (");"));
}

string OrmRepository::getSqlForAllEntriesInTable (const string &tableName, const string &waveSchema)
{
    return (string ("SELECT * FROM ") + waveSchema + "." + tableName + string (";"));
}

OrmTable *OrmRepository::getMostBaseTable (const string &tableName)
{
    OrmRepository                     *pOrmRepositoy = getInstance ();
    map<string, OrmTable *>::iterator  element1      = pOrmRepositoy->m_tablesMapByName.find (tableName);
    map<string, OrmTable *>::iterator  element1End   = pOrmRepositoy->m_tablesMapByName.end ();
    OrmTable                          *pTempOrmTable = NULL;

    if (element1End == element1)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::getMostBaseTable : No Table Entry for " + tableName);
        prismAssert (false, __FILE__, __LINE__);
        return (NULL);
    }
    else
    {
        pTempOrmTable = element1->second;

        while (NULL != (pTempOrmTable->getPParentTable ()))
        {
            pTempOrmTable = pTempOrmTable->getPParentTable ();
        }
    }

    return (pTempOrmTable);
}

ObjectId OrmRepository::getNewObjectId (const string &tableName)
{
    OrmRepository                     *pOrmRepositoy = getInstance ();
    map<string, OrmTable *>::iterator  element1      = pOrmRepositoy->m_tablesMapByName.find (tableName);
    map<string, OrmTable *>::iterator  element1End   = pOrmRepositoy->m_tablesMapByName.end ();

    if (element1End == element1)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::getNewObjectId : No Table Entry for " + tableName);
        prismAssert (false, __FILE__, __LINE__);
        return (ObjectId (0, 0));
    }
    else
    {
        OrmTable *pOrmTable = element1->second;
        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        OrmTable *pMostBaseOrmTable = pOrmTable->getPMostBaseTable ();
        prismAssert (NULL != pMostBaseOrmTable, __FILE__, __LINE__);

        return (ObjectId (pOrmTable->getTableId (), (((UI64) (((FrameworkToolKit::getThisLocationId ()) + (FrameworkToolKit::getHaId ()))) << 32) | pMostBaseOrmTable->getNextInstanceId ())));
    }
}

void OrmRepository::getSqlForDelete (const ObjectId &objectId, string &sqlForDelete, const string &waveSchema)
{
    UI32            tableId                  = objectId.getClassId ();
    UI64            instanceId               = objectId.getInstanceId ();
    OrmTable       *pTempOrmTable            = m_tablesMapById[tableId];
    vector<string>  auxilliaryTableNames;
    UI32            numberOfAuxilliaryTables = 0;
    UI32            i                        = 0;
    string          tableIdString            = string ("") + tableId;
    string          instanceIdString         = string ("") + instanceId;

    prismAssert (NULL != pTempOrmTable, __FILE__, __LINE__);

    pTempOrmTable->getAllAuxilliaryTableNames (auxilliaryTableNames);

    numberOfAuxilliaryTables = auxilliaryTableNames.size ();

    for (i = 0; i < numberOfAuxilliaryTables; i++)
    {
        sqlForDelete += "DELETE FROM " + waveSchema + "." + auxilliaryTableNames[i] + " WHERE ownerClassId = " + tableIdString + " AND ownerInstanceId = " + instanceIdString + ";";
    }

    getSqlForDeleteForDerivationsInstances (pTempOrmTable->getName (), sqlForDelete, instanceId, waveSchema);

    sqlForDelete += "DELETE FROM " + waveSchema + "." + pTempOrmTable->getName () + " WHERE objectIdClassId = " + tableIdString + " AND objectIdInstanceId = " + instanceIdString + ";";
}

ResourceId OrmRepository::printOrmDebugShellWrapper (UI32 argc, vector<string> argv)
{
    string ormOutput = "";

    ResourceId status = printOrm (argc, argv, ormOutput);

    trace (TRACE_LEVEL_INFO, string ("OrmRepository::printOrmDebugShellWrapper : Object Relational Mapping:") + ormOutput);
    
    return (status);
}

ResourceId OrmRepository::printOrm (UI32 argc, vector<string> argv, string &ormOutput)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    // Orm print function variables
    UI32      numberOfTables                = pOrmRepository->m_tables.size ();
    UI32      i                             = 0;
    OrmTable *pOrmTable                     = NULL;
    OrmTable *pOrmParentTable               = NULL;
    OrmTable *pMostBaseTable                = NULL;
    string    isALocalManagedObjectString   = "false";

  
    // Argument parsing variables
    UI32                numberOfArguments               = argv.size ();
    bool                isDotFormatEnabled              = false;
    bool                skipArg                         = true;
    UI32                numberOfFilteredManagedObjects  = 0;
    vector<string>      filteredManagedObjectsVector;
    map<string, string> filteredManagedObjectsMap;

    for (i = 0; i < numberOfArguments; i++)
    {
        if ("--filter" == argv[i] || "-f" == argv[i])
        {
            if ((i + 2) > numberOfArguments)
            {
                trace (TRACE_LEVEL_ERROR, "OrmRepository::printOrm : Filtering requires a list of comma delmited managed objects.");
                return (1);
            }

            tokenize (argv[i + 1], filteredManagedObjectsVector, ',');
            
            numberOfFilteredManagedObjects  = filteredManagedObjectsVector.size ();
            if (0 < numberOfFilteredManagedObjects)
            {
                // Build Filtered Map

                for (UI32 j = 0; j < numberOfFilteredManagedObjects; j++)
                {
                    filteredManagedObjectsMap[filteredManagedObjectsVector[j]] = filteredManagedObjectsVector[j];
                }
            }

            skipArg = true;
        }
        else if ("--dot" == argv[i] || "-d" ==  argv[i])
        {
            isDotFormatEnabled = true;
            skipArg = false;
        }
        else
        {
            skipArg = false;
        }

        if (true == skipArg)
        {
            i++;
        }
    }

    ormOutput += "\r\n";

    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = pOrmRepository->m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);
        
        // Print ORM table information when no filtering is given or only if when a specified filtering matches

        if ((0 == numberOfFilteredManagedObjects) || (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())))
        {
            pOrmParentTable = pOrmTable->getPParentTable ();

            pMostBaseTable = pOrmTable->getPMostBaseTable ();

            prismAssert (NULL != pMostBaseTable, __FILE__, __LINE__);

            if (true == pOrmTable->getIsALocalManagedObject ())
            {
                isALocalManagedObjectString = "true";
            }
            else
            {
                isALocalManagedObjectString = "false";
            }

            ormOutput += string ("") + pOrmTable->getTableId () + string (" : ") + pOrmTable->getName () + "\r\n";
            ormOutput += string ("    Derived From Class          : ") + pOrmTable->getDerivedFromClassName () + "\r\n";

            if (NULL != pOrmParentTable)
            {
                ormOutput += string ("    Derived From Class (Link)   : ") + pOrmParentTable->getName () + "\r\n";
            }
            else
            {
                ormOutput += string ("    Derived From Class (Link)   : NULL") + "\r\n";
            }

            ormOutput += string ("    Most ORM Base Class Name    : ") + pMostBaseTable->getName () + "\r\n";
            ormOutput += string ("    Current Next Instance Id    : ") + pMostBaseTable->getCurrentNextInstanceId () + "\r\n";
            ormOutput += string ("    Is A Local Managed Object   : ") + isALocalManagedObjectString + "\r\n";
            ormOutput += string ("    Derivation Instances Tables : ") + "\r\n";

            pOrmTable->printDerivationsInstancesTables ("        ", ormOutput);
        }
    }

    if (true == isDotFormatEnabled)
    {
        ormOutput += string ("digraph WaveManagedObjectClassDiagram") + "\r\n";
        ormOutput += string ("{") + "\r\n";
        ormOutput += string ("    node [style=filled,color=\".7 .3 1.0\"];") + "\r\n";
        ormOutput += string ("    rankdir=LR;") + "\r\n";

        ormOutput += string ("    WaveLocalManagedObject -> WaveManagedObject;") + "\r\n";
        ormOutput += string ("    DcmManagedObject -> WaveManagedObject;") + "\r\n";
        ormOutput += string ("    DcmLocalManagedObject -> WaveLocalManagedObject;") + "\r\n";

        for (i = 0; i < numberOfTables; i++)
        {
            pOrmTable = pOrmRepository->m_tables[i];

            prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

            // Print ORM table information when no filtering is given or only if when a specified filtering matches

            if ((0 == numberOfFilteredManagedObjects) || 
                (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())) ||
                (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getDerivedFromClassName ())) )
            {
                ormOutput += string ("    edge [style=solid,dir=forward,color=black,arrowhead=normal];") + "\r\n";
                ormOutput += string ("    ") + pOrmTable->getName () + " ->  " + pOrmTable->getDerivedFromClassName () + "\r\n";
            }

            vector<string>             relatedToTables;
            vector<OrmRelationType>    relationTypes;
            vector<OrmRelationUmlType> relationUmlTypes;

            pOrmTable->getDetailsForRelationships (relatedToTables, relationTypes, relationUmlTypes);

            UI32 numberOfRelations = relatedToTables.size ();
            UI32 j                 = 0;

            for (j = 0; j < numberOfRelations; j++)
            {
                if (ORM_RELATION_UML_TYPE_ASSOCIATION == relationUmlTypes[j])
                {
                    if (ORM_RELATION_TYPE_ONE_TO_ONE == relationTypes[j])
                    {
                        if ((0 == numberOfFilteredManagedObjects) || 
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())) ||
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (relatedToTables[j])) )
                        {
                            ormOutput += string ("        edge [style=dotted,dir=forward,color=blue,arrowhead=normal];") + "\r\n";
                            ormOutput += string ("        ") + pOrmTable->getName () + " -> " + relatedToTables[j] + ";" + "\r\n";
                        }
                    }
                    else
                    {
                        if ((0 == numberOfFilteredManagedObjects) || 
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())) ||
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (relatedToTables[j])) )
                        {
                            ormOutput += string ("        edge [style=solid,dir=forward,color=blue,arrowhead=normal];") + "\r\n";
                            ormOutput += string ("        ") + pOrmTable->getName () + " -> " + relatedToTables[j] + ";" + "\r\n";
                        }
                    }
                }
                else if (ORM_RELATION_UML_TYPE_AGGREGATION == relationUmlTypes[j])
                {
                    if (ORM_RELATION_TYPE_ONE_TO_ONE == relationTypes[j])
                    {
                        if ((0 == numberOfFilteredManagedObjects) || 
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())) ||
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (relatedToTables[j])) )
                        {
                            ormOutput += string ("        edge [style=dotted,dir=forward,color=green,arrowhead=odiamond];") + "\r\n";
                            ormOutput += string ("        ") + relatedToTables[j] + " -> " + pOrmTable->getName () + ";" + "\r\n";
                        }
                    }
                    else
                    {
                        if ((0 == numberOfFilteredManagedObjects) || 
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())) ||
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (relatedToTables[j])) )
                        {
                            ormOutput += string ("        edge [style=solid,dir=forward,color=green,arrowhead=odiamond];") + "\r\n";
                            ormOutput += string ("        ") + relatedToTables[j] + " -> " + pOrmTable->getName () + ";" + "\r\n";
                        }
                    }
                }
                else if (ORM_RELATION_UML_TYPE_COMPOSITION == relationUmlTypes[j])
                {
                    if (ORM_RELATION_TYPE_ONE_TO_ONE == relationTypes[j])
                    {
                        if ((0 == numberOfFilteredManagedObjects) || 
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())) ||
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (relatedToTables[j])) )
                        {
                            ormOutput += string ("        edge [style=dotted,dir=forward,color=red,arrowhead=diamond];") + "\r\n";
                            ormOutput += string ("        ") + relatedToTables[j] + " -> " + pOrmTable->getName () + ";" + "\r\n";
                        }
                    }
                    else
                    {
                        if ((0 == numberOfFilteredManagedObjects) || 
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (pOrmTable->getName ())) ||
                            (filteredManagedObjectsMap.end () != filteredManagedObjectsMap.find (relatedToTables[j])) )
                        {
                            ormOutput += string ("        edge [style=solid,dir=forward,color=red,arrowhead=diamond];") + "\r\n";
                            ormOutput += string ("        ") + relatedToTables[j] + " -> " + pOrmTable->getName () + ";" + "\r\n";
                        }
                    }
                }
            }
        }

        ormOutput += string ("}") + "\r\n";
    }

    return (0);
}

void OrmRepository::getAllMostBaseTableNames (vector<string> &mostBaseTableNames)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    UI32      numberOfTables  = pOrmRepository->m_tables.size ();
    UI32      i               = 0;
    OrmTable *pOrmTable       = NULL;
    OrmTable *pOrmParentTable = NULL;

    trace (TRACE_LEVEL_INFO, "");

    for (i = 0; i < numberOfTables; i++)
    {
        pOrmTable = pOrmRepository->m_tables[i];

        prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

        pOrmParentTable = pOrmTable->getPParentTable ();

        if (NULL == pOrmParentTable)
        {
            mostBaseTableNames.push_back (pOrmTable->getName ());
        }
    }
}

void OrmRepository::updateLatestInstanceIdForTable (const string &tableName, const UI32 &latestInstanceId)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->setNextInstanceId (latestInstanceId);
}

string OrmRepository::generateSqlToEmptyOrmDatabase (const string &waveSchema)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    string         sqlForDelete;
    vector<string> references;
    vector<string> auxilliaryTableNames;
    UI32           numberOfReferences    = 0;

    // Get the unique destinations in reference statements (all types of foreign key constraints destinations and 1->* foreign key sources as well)

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        m_tables[i]->getReferences (references);
    }

    numberOfReferences = references.size ();

    // Generate Sql for all the tables.  The SQL must be generated in the reverese order to satisfy the dependencies;

    // First delete all entries from Auxilliary tables (used for 1-* UML associations/compositions).

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        auxilliaryTableNames.clear ();
        (m_tables[i])->getAuxilliaryTableNames (auxilliaryTableNames);

        UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
        UI32 j                        = 0;

        for (j = 0; j < numberOfAuxilliaryTables; j++)
        {
            sqlForDelete += getSqlToDeleteAllEntriesFromAuxilliaryTable (auxilliaryTableNames[j], waveSchema) + "\n";
        }
    }

    // Then delete the entries from all regular tables (it must be done in the reverese order to satisfy the dependencies)

    for (i = numberOfTables - 1; i >= 0; i--)
    {
        sqlForDelete += (m_tables[i])->getSqlToDeleteAllEntries (waveSchema) + " \n";
    }

    // Then delete from the derivation tables at last.

    for (i = 0; i < ((SI32) numberOfReferences); i++)
    {
        sqlForDelete += getSqlToDeleteAllEntriesFromDerivationsTable (references[i], waveSchema) + "\n";
    }

    return (sqlForDelete);
}

void OrmRepository::getAllSqlsToCountAllEntriesInDatabase (vector <string> &sqlStrings, const string &waveSchema)
{
    UI32           numberOfTables       = m_tables.size ();
    SI32           i                    = 0;
    vector<string> references;
    UI32           numberOfReferences   = 0;
    vector<string> auxilliaryTableNames;


    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        m_tables[i]->getReferences (references);
    }

    numberOfReferences = references.size ();

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        auxilliaryTableNames.clear ();

        (m_tables[i])->getAuxilliaryTableNames (auxilliaryTableNames);

        UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
        UI32 j                        = 0;

        for (j = 0; j < numberOfAuxilliaryTables; j++)
        {
            string countSqlString;
    
            getSqlToCountNumberOfEntriesInTable (auxilliaryTableNames[j], waveSchema, countSqlString);

            sqlStrings.push_back (countSqlString);
        }        
    }

    for (i = numberOfTables - 1; i >= 0; i--)
    {
        string countSqlString;

        (m_tables[i])->getSqlToCountNumberOfEntriesInTable (waveSchema, countSqlString);
        
        sqlStrings.push_back (countSqlString);
    }


    for (i = 0; i < ((SI32) numberOfReferences); i++)
    {
        string countSqlString;

        getSqlToCountNumberOfEntriesInTable (references[i], waveSchema, countSqlString);

        sqlStrings.push_back (countSqlString);
    }
}


string OrmRepository::generateSqlToEmptyOrmDatabaseUsingTruncate (const string &waveSchema)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    string         sqlForDelete;
    vector<string> references;
    vector<string> auxilliaryTableNames;
    UI32           numberOfReferences    = 0;

    // Get the unique destinations in reference statements (all types of foreign key constraints destinations and 1->* foreign key sources as well)

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        m_tables[i]->getReferences (references);
    }

    numberOfReferences = references.size ();

    // Generate Sql for all the tables.  The SQL must be generated in the reverese order to satisfy the dependencies;

    // First delete all entries from Auxilliary tables (used for 1-* UML associations/compositions).

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        auxilliaryTableNames.clear ();
        (m_tables[i])->getAuxilliaryTableNames (auxilliaryTableNames);

        UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
        UI32 j                        = 0;

        for (j = 0; j < numberOfAuxilliaryTables; j++)
        {
            sqlForDelete += getSqlToTruncateFromAuxilliaryTable (auxilliaryTableNames[j], waveSchema) + "\n";
        }
    }

    // Then delete the entries from all regular tables (it must be done in the reverese order to satisfy the dependencies)

    for (i = numberOfTables - 1; i >= 0; i--)
    {
        sqlForDelete += (m_tables[i])->getSqlToTruncate (waveSchema) + " \n";
    }

    // Then delete from the derivation tables at last.

    for (i = 0; i < ((SI32) numberOfReferences); i++)
    {
        sqlForDelete += getSqlToTruncateFromDerivationsTable (references[i], waveSchema) + "\n";
    }

    return (sqlForDelete);
}

string OrmRepository::generateSqlToTruncateDatabaseSchemaForGivenTables (const string &waveSchema, vector<string>& tableNames)
{
    trace (TRACE_LEVEL_INFO, "OrmRepository::generateSqlToTruncateDatabaseSchemaForGivenTables: entering...");

    string  sqlForTruncate;
    UI32    numOfTables     = tableNames.size ();

    for (UI32 i = 0; i < numOfTables; i++)
    {
        sqlForTruncate += string ("TRUNCATE " + waveSchema + "." + tableNames[i] + " CASCADE;");
    }

    return (sqlForTruncate);
}
        
string OrmRepository::generateSqlToSelectivelyEmptyOrmDatabase (const string &waveSchema)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    string         sqlForDelete;
    vector<string> auxilliaryTableNames;

    // Generate Sql for all the tables.  The SQL must be generated in the reverese order to satisfy the dependencies;


    // Delete from the derivation instances tables at First.

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        if (true == m_tables[i]->getEmptyNeededOnPersistentBootWithDefault ())
        {
            sqlForDelete += (m_tables[i])->getSqlToSelectivelyDeleteDerivationsInstances (waveSchema) + "\n";
        }
    }

    // Next delete all entries from Auxilliary tables (used for 1-* UML associations/compositions).

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        if (true == m_tables[i]->getEmptyNeededOnPersistentBootWithDefault ())
        {
            auxilliaryTableNames.clear ();
            (m_tables[i])->getAuxilliaryTableNames (auxilliaryTableNames);

            UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
            UI32 j                        = 0;

            for (j = 0; j < numberOfAuxilliaryTables; j++)
            {
                sqlForDelete += getSqlToTruncateFromAuxilliaryTable (auxilliaryTableNames[j], waveSchema) + "\n";
            }
        }
    }

    // Then delete the entries from all regular tables (it must be done in the reverese order to satisfy the dependencies)

    for (i = numberOfTables - 1; i >= 0; i--)
    {
        if (true == m_tables[i]->getEmptyNeededOnPersistentBootWithDefault ())
        {
            sqlForDelete += (m_tables[i])->getSqlToSelectivelyDeleteEntries (waveSchema) + " \n";
        }
    }

    trace (TRACE_LEVEL_DEBUG, "OrmRepository::generateSqlToSelectivelyEmptyOrmDatabase : " + sqlForDelete);

    return (sqlForDelete);
}

string OrmRepository::generateSqlToSelectivelyEmptyOrmDatabaseUsingTruncate (const string &waveSchema)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    string         sqlForDelete;
    vector<string> auxilliaryTableNames;

    // Generate Sql for all the tables.  The SQL must be generated in the reverese order to satisfy the dependencies;



    // Delete all entries from Auxilliary tables (used for 1-* UML associations/compositions).

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        if (true == m_tables[i]->getEmptyNeededOnPersistentBootWithDefault ())
        {
            auxilliaryTableNames.clear ();
            (m_tables[i])->getAuxilliaryTableNames (auxilliaryTableNames);

            UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
            UI32 j                        = 0;

            for (j = 0; j < numberOfAuxilliaryTables; j++)
            {
                sqlForDelete += getSqlToTruncateFromAuxilliaryTable (auxilliaryTableNames[j], waveSchema) + "\n";
            }
        }
    }

    // Then delete the entries from all regular tables (it must be done in the reverese order to satisfy the dependencies)

    for (i = numberOfTables - 1; i >= 0; i--)
    {
        if (true == m_tables[i]->getEmptyNeededOnPersistentBootWithDefault ())
        {
            sqlForDelete += (m_tables[i])->getSqlToTruncate (waveSchema) + " \n";
            //sqlForDelete += (m_tables[i])->getSqlToSelectivelyDeleteEntries (waveSchema) + " \n";
        }
    }

    // Then Delete from the derivation instances tables.

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        if (true == m_tables[i]->getEmptyNeededOnPersistentBootWithDefault ())
        {
            sqlForDelete += (m_tables[i])->getSqlToSelectivelyTruncateFromDerivationsInstances (waveSchema) + "\n";
            //sqlForDelete += (m_tables[i])->getSqlToSelectivelyDeleteDerivationsInstances (waveSchema) + "\n";
        }
    }

    trace (TRACE_LEVEL_DEBUG, "OrmRepository::generateSqlToSelectivelyEmptyOrmDatabaseUsingTruncate : " + sqlForDelete);

    return (sqlForDelete);
}

string OrmRepository::generateSqlToDropTableFromOrmDatabase (const string &waveSchema, const string &tableName)
{
    SI32           i                     = 0;
    string         sqlForDrop;
    vector<string> references;
    vector<string> auxilliaryTableNames;
    UI32           numberOfReferences    = 0;
    OrmTable       *ormTable             = getTableByName (tableName);

    // Get the unique destinations in reference statements (all types of foreign key constraints destinations and 1->* foreign key sources as well)

    (ormTable)->getReferences (references);

    numberOfReferences = references.size ();

    // First drop Auxilliary tables (used for 1-* UML associations/compositions).
    (ormTable)->getAuxilliaryTableNames (auxilliaryTableNames);

    UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
    UI32 j                        = 0;

    for (j = 0; j < numberOfAuxilliaryTables; j++)
    {
        //sqlForDrop += getSqlToDropAuxilliaryTable (auxilliaryTableNames[j], waveSchema) + "\n";
        sqlForDrop += getSqlToDeleteAllEntriesFromAuxilliaryTable (auxilliaryTableNames[j], waveSchema) + "\n";
    }

    // Then drop all regular table
    //sqlForDrop += (ormTable)->getSqlToDrop (waveSchema) + " \n";
    sqlForDrop += (ormTable)->getSqlToDeleteAllEntries (waveSchema) + " \n";

    // Then drop the derivation tables at last.
    for (i = 0; i < ((SI32) numberOfReferences); i++)
    {
        //sqlForDrop += getSqlToDropDerivationsTable (references[i], waveSchema) + "\n";
        sqlForDrop += getSqlToDeleteAllEntriesFromDerivationsTable (references[i], waveSchema) + "\n";
    }

    return (sqlForDrop);
}

string OrmRepository::generateSqlToDropTablesFromOrmDatabase (const string &waveSchema)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    string         sqlForDrop;
    vector<string> references;
    vector<string> auxilliaryTableNames;
    UI32           numberOfReferences    = 0;

    // Get the unique destinations in reference statements (all types of foreign key constraints destinations and 1->* foreign key sources as well)

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        m_tables[i]->getReferences (references);
    }

    numberOfReferences = references.size ();

    // Generate Sql for all the tables to drop them.  The SQL must be generated in the reverese order to satisfy the dependencies;

    // First drop Auxilliary tables (used for 1-* UML associations/compositions).

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        auxilliaryTableNames.clear ();
        (m_tables[i])->getAuxilliaryTableNames (auxilliaryTableNames);

        UI32 numberOfAuxilliaryTables = auxilliaryTableNames.size ();
        UI32 j                        = 0;

        for (j = 0; j < numberOfAuxilliaryTables; j++)
        {
            sqlForDrop += getSqlToDropAuxilliaryTable (auxilliaryTableNames[j], waveSchema) + "\n";
        }
    }

    // Then drop all regular tables (it must be done in the reverese order to satisfy the dependencies)

    for (i = numberOfTables - 1; i >= 0; i--)
    {
        sqlForDrop += (m_tables[i])->getSqlToDrop (waveSchema) + " \n";
    }

    // Then drop the derivation tables at last.

    for (i = 0; i < ((SI32) numberOfReferences); i++)
    {
        sqlForDrop += getSqlToDropDerivationsTable (references[i], waveSchema) + "\n";
    }

    return (sqlForDrop);
}

string OrmRepository::generateSqlToDropSchemaFromOrmDatabase (const string &waveSchema)
{
    string sqlToDropSchema;

    sqlToDropSchema = "DROP SCHEMA " + waveSchema + " CASCADE;";

    return (sqlToDropSchema);
}

vector<string> OrmRepository::getTableNames (bool checkEmptyNeeded)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    vector<string> tableNames;

    if (true == checkEmptyNeeded)
    {
        for (i = 0; i < ((SI32) numberOfTables); i++)
        {
            if (true == m_tables[i]->getEmptyNeededOnPersistentBoot ()) 
            {
                tableNames.push_back (m_tables[i]->getName ());
            }
            else
            {
                trace (TRACE_LEVEL_DEVEL, "OrmRepository::getNumberOfTables : skipping " + m_tables[i]->getName ());
            }
        }
    }
    else
    {
        for (i = 0; i < ((SI32) numberOfTables); i++)
        {
            tableNames.push_back (m_tables[i]->getName ());
        }
    }

    return (tableNames);
}

vector<string> OrmRepository::getDerivationsTableNames (bool checkEmptyNeeded)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    vector<string> references;

    if (true == checkEmptyNeeded)
    {
        for (i = 0; i < ((SI32) numberOfTables); i++)
        {
            if (true == m_tables[i]->getEmptyNeededOnPersistentBoot ())
            {
                m_tables[i]->getReferences (references);
            }
            else
            {
                trace (TRACE_LEVEL_DEVEL, "OrmRepository::getDerivationsTableNames : skipping " + m_tables[i]->getName ());
            }
        }
    }
    else
    {
        for (i = 0; i < ((SI32) numberOfTables); i++)
        {
            m_tables[i]->getReferences (references);
        }
    }

    return (references);
}

vector<string> OrmRepository::getAuxilliaryTableNames (bool checkEmptyNeeded)
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    vector<string> auxilliaryTableNames;

    auxilliaryTableNames.clear ();

    if (true == checkEmptyNeeded)
    {
        for (i = 0; i < ((SI32) numberOfTables); i++)
        {
            if (true == m_tables[i]->getEmptyNeededOnPersistentBoot ())
            {
                (m_tables[i])->getAuxilliaryTableNamesAppended (auxilliaryTableNames);
            }
            else
            {
                trace (TRACE_LEVEL_DEVEL, "OrmRepository::getAuxilliaryTableNames : skipping " + m_tables[i]->getName ());
            }
        }
    }
    else
    {
        for (i = 0; i < ((SI32) numberOfTables); i++)
        {
            (m_tables[i])->getAuxilliaryTableNamesAppended (auxilliaryTableNames);
        }
    }

    return (auxilliaryTableNames);
}

vector<string> OrmRepository::getTableNamesWithNoEmptyNeeded ()
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    vector<string> tableNames;

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        if (false == m_tables[i]->getEmptyNeededOnPersistentBoot ())
        {
            tableNames.push_back (m_tables[i]->getName ());
        }
    }

    return (tableNames);
}

vector<string> OrmRepository::getDerivationsTableNamesWithNoEmptyNeeded ()
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    vector<string> references;

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        if (false == m_tables[i]->getEmptyNeededOnPersistentBoot ())
        {
            m_tables[i]->getReferences (references);
        }
    }

    return (references);
}

vector<string> OrmRepository::getAuxilliaryTableNamesWithNoEmptyNeeded ()
{
    UI32           numberOfTables        = m_tables.size ();
    SI32           i                     = 0;
    vector<string> auxilliaryTableNames;

    auxilliaryTableNames.clear ();

    for (i = 0; i < ((SI32) numberOfTables); i++)
    {
        if (false == m_tables[i]->getEmptyNeededOnPersistentBoot ())
        {
            (m_tables[i])->getAuxilliaryTableNamesAppended (auxilliaryTableNames);
        }
    }

    return (auxilliaryTableNames);
}

UI32 OrmRepository::getTableId (const string &tableName)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    UI32 tableId = 0;

    if (NULL != pOrmTable)
    {
        tableId = pOrmTable->getTableId ();
    }

    return (tableId);
}

void OrmRepository::getAuxilliaryTableNames (const string &tableName, vector<string> &auxilliaryTableNames)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getAuxilliaryTableNames (auxilliaryTableNames);
}

void OrmRepository::getAllAuxilliaryTableNames (const string &tableName, vector<string> &auxilliaryTableNames)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getAllAuxilliaryTableNames (auxilliaryTableNames);
}

void OrmRepository::getAllAuxilliaryTableNames (const string &tableName, vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getAllAuxilliaryTableNames (auxilliaryTableNames, auxilliaryTableUmlTypes);
}

void OrmRepository::getAllAuxilliaryTableNames (const string &tableName, vector<string> &auxilliaryTableNames, vector<OrmRelationUmlType> &auxilliaryTableUmlTypes, const vector<string> &selectedEntities)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getAllAuxilliaryTableNames (auxilliaryTableNames, auxilliaryTableUmlTypes, selectedEntities);
}

string OrmRepository::getTableNameById (const UI32 &tableId)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapById[tableId];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable->getName ());
}

void OrmRepository::getSqlForInsertForDerivationsInstances (const string &tableName, string &sqlForInsertForDerivationsInstnces, const UI64 &instanceId, const string &waveSchema)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable->getSqlForInsertForDerivationsInstances (sqlForInsertForDerivationsInstnces, instanceId, waveSchema));
}

void OrmRepository::getSqlForDeleteForDerivationsInstances (const string &tableName, string &sqlForDeleteForDerivationsInstnces, const UI64 &instanceId, const string &waveSchema)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable->getSqlForDeleteForDerivationsInstances (sqlForDeleteForDerivationsInstnces, instanceId, waveSchema));
}

OrmTable *OrmRepository::getTableByName (const string &tableName)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable);
}

void OrmRepository::addMostBaseClass (const string &mostBaseClass)
{
    OrmRepository *pOrmRepository = getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    map<string, string>::iterator element    = (pOrmRepository->m_mostBaseClassesMap).find (mostBaseClass);
    map<string, string>::iterator endElement = (pOrmRepository->m_mostBaseClassesMap).end  ();

    if (endElement != element)
    {
        trace (TRACE_LEVEL_FATAL, "OrmRepository::addMostBaseClass : Duplicate Addition of Most Base Class : \'" +  mostBaseClass + "\'");
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        (pOrmRepository->m_mostBaseClassesMap)[mostBaseClass] = mostBaseClass;
    }
}

bool OrmRepository::isAMostBaseClass (const string &mostBaseClass)
{
    OrmRepository *pOrmRepository = getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    map<string, string>::iterator element    = (pOrmRepository->m_mostBaseClassesMap).find (mostBaseClass);
    map<string, string>::iterator endElement = (pOrmRepository->m_mostBaseClassesMap).end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

const string OrmRepository::getWaveCurrentSchema ()
{
    return (m_waveCurrentSchema);
}

const string OrmRepository::getWaveStartSchema ()
{
    return (m_waveStartSchema);
}

const map<string, Attribute*> OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (const string &tableName, vector<string> &userDefinedKeyCombination)
{
    OrmTable *pOrmTable = getTableByName (tableName);

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable->getUserDefinedKeyCombinationWithTypes (userDefinedKeyCombination));
}

bool OrmRepository::isALocalManagedObject (const string &tableName)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable->getIsALocalManagedObject ());
}



/*
void OrmRepository::restoreOldTableIds ()
{
    vector <OrmTable *> tablesCopy = m_tables;
    m_tablesMapById.clear ();

    vector<OrmTable *>::iterator iter;

    for (iter=tablesCopy.begin (); iter < tablesCopy.end (); ++iter)
    {
        OrmTable *table = *iter;

        UI32 oldTableId = table->getOldTableId ();

        table->setTableId (oldTableId);

        m_tablesMapById[oldTableId] = table;
    }
    trace (TRACE_LEVEL_INFO, "OrmRepository::restoreOldTableIds: completed");
}
*/
/*
void OrmRepository::updateTableId (const vector<pair<string, UI32> > &tableNameIdTuples)
{
    vector <OrmTable *> tablesCopy = m_tables;
    m_tablesMapById.clear ();

    for (vector<pair<string, UI32> >::const_iterator iter = tableNameIdTuples.begin (); iter < tableNameIdTuples.end (); ++iter)
    {
        const string &tableName  = (*iter).first;
        const UI32    newTableId = (*iter).second;

        OrmTable *table = m_tablesMapByName[tableName];

        // This table got deleted in the new schema
        if (table == NULL)
        {
            continue;
        }

        trace (TRACE_LEVEL_DEVEL, "OrmRepository::updateTableId : Updating table \'" + tableName + "\' to table id :" + newTableId + ", old table Id :" + table->getTableId ());

        table->setOldTableId (table->getTableId ());
        table->setTableId (newTableId);
        m_tablesMapById[newTableId] = table;
        vector <OrmTable *>::iterator tableIter = find (tablesCopy.begin (), tablesCopy.end (), table);
        if (tableIter != tablesCopy.end ())
        {
            tablesCopy.erase (tableIter);
        }
    } //end_for

    // What remains in the tablesCopy vector are new tables, assign them (unique) tableIds
    UI32 runCounter = 1;
    for (vector<OrmTable *>::iterator iter = tablesCopy.begin (); iter < tablesCopy.end ();
          ++iter)
    {

        OrmTable *table = *iter;

        // detect holes in tableIds and assign those to new tables
        for (map<UI32, OrmTable *>::iterator iter2 = m_tablesMapById.lower_bound(runCounter);
              (iter2 != m_tablesMapById.end ()) && ((*iter2).first == runCounter); ++iter2)
        {
            ++runCounter;
        }

        trace (TRACE_LEVEL_INFO, "OrmRepository::updateTableId : Table \':" + table->getName () + "\' now assigned id " + runCounter);
        table->setOldTableId (table->getTableId ());
        table->setTableId(runCounter);
        m_tablesMapById[runCounter] = table;

        ++runCounter;
    } // end_for
}
*/
const vector<string>  &   OrmRepository::getManagedObjectNamesForSchemaChange () const
{
    return m_managedObjectNamesForSchemaChange;
}

const vector<string> &   OrmRepository:: getFieldNamesStringsForSchemaChange () const
{
    return m_fieldNamesStringsForSchemaChange;
}

const vector<string> &   OrmRepository::getFieldNamesTypesForSchemaChange () const
{
    return m_fieldNamesTypesForSchemaChange;
}
               
const vector<string>  &  OrmRepository::getFieldNamesExpandedTypesForSchemaChange () const
{
    return m_fieldNamesExpandedTypesForSchemaChange;
}

const vector<UI32>    &   OrmRepository:: getClassIdsForSchemaChange () const
{
    return m_classIds;
}

const vector<UI32>    &   OrmRepository::getParentTableIds () const
{
    return m_parentTableIds;
}

const vector<bool>  &   OrmRepository::getIsALocalManagedObjectInfoForSchemaChange  () const
{
    return m_isALocalManagedObjectInfoForSchemaChange;
}

const vector<string>  &   OrmRepository::getDerivedFromClassNameForSchemaChange  () const
{
    return m_derivedFromClassNameForSchemaChange;
}

const vector<string>  &   OrmRepository::getUserDefinedKeyCombinationsFieldNamesForSchemaChange  () const
{
    return m_userDefinedKeyCombinationsFieldNamesForSchemaChange;
}

const vector<string>  &   OrmRepository::getUserDefinedKeyCombinationsFieldTypesForSchemaChange  () const
{
    return m_userDefinedKeyCombinationsFieldTypesForSchemaChange;
}

ManagedObjectSchemaInfoRepository & OrmRepository::getMOSchemaInfoRepository()
{
    return (m_MOUpgradeRepository);
}

bool OrmRepository::isFieldOfIntegerType (const string &tableName, const string &columnName)
{
    OrmTable  *pTableName = getTableByName (tableName);    
    
    prismAssert (NULL != pTableName, __FILE__, __LINE__);

    return (pTableName->isAnIntegerColumn (columnName));
}

void OrmRepository::getSqlToDeleteAllEntriesInTable (const string &tableName, string &sqlString)
{
    OrmTable       *pOrmTable                = getTableByName (tableName);
    vector<string>  auxilliaryTableNames;
    UI32            numberOfauxilliaryTables = 0;

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getAllAuxilliaryTableNames (auxilliaryTableNames);

    numberOfauxilliaryTables = auxilliaryTableNames.size ();    

    for (UI32 i = 0; i < numberOfauxilliaryTables; i++)
    {
        sqlString += "DELETE FROM " + OrmRepository::m_waveCurrentSchema + "." + auxilliaryTableNames [i] + ";\n";
    }    

    sqlString += "DELETE FROM " + OrmRepository::m_waveCurrentSchema + "." + tableName + ";\n";
    sqlString += "DELETE FROM " + OrmRepository::m_waveCurrentSchema + "." + tableName + "DerivationsInstances;\n";

}

void OrmRepository::getAllCompositionTableName (set<string> &allCompositionTableNames)
{
    allCompositionTableNames = s_allCompostionTableNames;
}

void OrmRepository::getEntityDetails (const string &tableName, const string &entityName, bool &isAColumn, bool &isARelation, OrmRelationType &ormRelationType)
{
    OrmTable *pOrmTable = getTableByName (tableName);

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getEntityDetails (entityName, isAColumn, isARelation, ormRelationType);
}

void OrmRepository::getOrmRelationTypeAndOrmRelationUmlType (const string &tableName, const string &entityName, OrmRelationType &ormRelationType, OrmRelationUmlType &ormRelationUmlType)
{
    OrmTable *pOrmTable = getTableByName (tableName);

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getOrmRelationTypeAndOrmRelationUmlType (entityName, ormRelationType, ormRelationUmlType);    
}

bool OrmRepository::isASimpleObjectIdColumn (const string &tableName, const string &entityName)
{
    OrmTable *pOrmTable = getTableByName (tableName);

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable->isASimpleObjectIdColumn (entityName));
}

                     
string OrmRepository::getSqlStringForAttributeTypeString (const string &attributeTypeString)
{
    map<string, string>::iterator strIter;

    strIter = m_attributeTypeToSqlTypeMap.find (attributeTypeString);

    if (strIter != m_attributeTypeToSqlTypeMap.end ())
    {
        return (strIter->second);
    }
    else 
    {
        tracePrintf (TRACE_LEVEL_ERROR, false, false, "OrmRepository::getSqlStringForAttributeTypeString : Not able to get Sql String for Attribute String '%s'\n", attributeTypeString.c_str());
    }

    return ("");
}
                     
bool OrmRepository::isTypeConversionSupported (const string &from, const string &to)
{
    map<string, map<string, string> >::iterator mapStringToMapStringIter = m_attributeSupportedConversionsMap.find (from);

    if (mapStringToMapStringIter != m_attributeSupportedConversionsMap.end ())
    {
        map<string, string> supportedConversionsMap = mapStringToMapStringIter->second; 

        map<string, string>::iterator mapStringToStringIter = supportedConversionsMap.find (to);

        if (mapStringToStringIter != supportedConversionsMap.end ())
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
        tracePrintf (TRACE_LEVEL_ERROR, false, false, "OrmRepository::isTypeConversionSupported : Not able to find Attribute String '%s' in supportedConversionsMap \n", to.c_str());
    }

    return (false);
}

void OrmRepository::buildAttributeTypeToSqlTypeMap ()
{
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI32"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI32_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI64"] = "bigint";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI64_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI64"] = "bigint";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI64_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI32"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI32_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI16"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI16_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI16"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI16_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI8"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI8_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI8"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SI8_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_OBJECTID"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_COMPOSITION"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_COMPOSITION_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_BOOL"] = "boolean";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_CHAR"] = "char (1)";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_STRING"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_ENUM"] = "integer";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_DATE"] = "date";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_DATE_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_TIME"] = "time without time zone";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_TIME_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_DATETIME"] = "timestamp without time zone";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_BITMAP"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_DECIMAL64"] = "numeric";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_DECIMAL64_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UNION"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SNMPOBJECTID"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI32_RANGE"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_LARGEOBJECT"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_MAC_RANGE"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI32_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_STRING_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_BOOL_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_ENUM_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS2"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_RESOURCEID"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI32_VECTOR_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_HOST_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UI64_UC"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SQL_BETWEEN"] = "";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_SQL_IN"] = "";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_UUID"] = "varchar";
    m_attributeTypeToSqlTypeMap["WAVE_ATTRIBUTE_TYPE_URI"] = "varchar";
}
                     
void OrmRepository::buildAttributeSupportedConversionsMap ()
{
    // UI32
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI32"] = AttributeUI32::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = AttributeUI32::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = AttributeUI32::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = AttributeUI32::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = AttributeUI32::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = AttributeUI32::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI32_VECTOR"] = AttributeUI32Vector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI64"] = AttributeUI64::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI64_VECTOR"] = AttributeUI64Vector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI64"] = AttributeSI64::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI64_VECTOR"] = AttributeSI64Vector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI32"] = AttributeSI32::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI32_VECTOR"] = AttributeSI32Vector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI16"] = AttributeUI16::getSupportedConversions ();

    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI16"] = AttributeSI16::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI8"] = AttributeUI8::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI8"] = AttributeSI8::getSupportedConversions ();

    //m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI16_VECTOR"] = AttributeUI16Vector::getSupportedConversions ();
    //m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI16_VECTOR"] = AttributeSI16Vector::getSupportedConversions ();
    //m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI8_VECTOR"] = AttributeUI8Vector::getSupportedConversions ();
    //m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SI8_VECTOR"] = AttributeSI8Vector::getSupportedConversions ();

    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_OBJECTID"] = AttributeObjectId::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR"] = AttributeObjectIdVector::getSupportedConversions ();
    //m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_COMPOSITION"] = AttributeManagedObjectComposition::getSupportedConversions ();
    //m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_COMPOSITION_VECTOR"] = AttributeManagedObjectVectorComposition::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_BOOL"] = AttributeBool::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR"] = AttributeBoolVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_CHAR"] = AttributeChar::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_STRING"] = AttributeString::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = AttributeStringVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME"] = AttributeWorldWideName::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = AttributeWorldWideNameVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS"] = AttributeMacAddress::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = AttributeMacAddressVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = AttributeIpV4Address::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = AttributeIpV4AddressVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = AttributeIpV6Address::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = AttributeIpV6AddressVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_ENUM"] = AttributeEnum::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_DATE"] = AttributeDate::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_DATE_VECTOR"] = AttributeDateVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_TIME"] = AttributeTime::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_TIME_VECTOR"] = AttributeTimeVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_DATETIME"] = AttributeDateTime::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR"] = AttributeDateTimeVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_BITMAP"] = AttributeBitMap::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_DECIMAL64"] = AttributeDecimal64::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_DECIMAL64_VECTOR"] = AttributeDecimal64Vector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UNION"] = AttributeUnion::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"] = AttributeIpV4AddressNetworkMask::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK"] = AttributeIpV6AddressNetworkMask::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SNMPOBJECTID"] = AttributeSnmpObjectId::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI32_RANGE"] = AttributeUI32Range::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = AttributeIpVxAddress::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = AttributeIpVxAddressVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_LARGEOBJECT"] = AttributeLargeObject::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_MAC_RANGE"] = AttributeMACRange::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI32_UC"] = AttributeUI32UC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_STRING_UC"] = AttributeStringUC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_BOOL_UC"] = AttributeBoolUC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_ENUM_UC"] = AttributeEnumUC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS2"] = AttributeMacAddress2::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = AttributeMacAddress2Vector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_RESOURCEID"] = AttributeResourceId::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM"] = AttributeResourceId::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR"] = AttributeResourceIdVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR"] = AttributeResourceIdVector::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC"] = AttributeIpV4AddressUC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI32_VECTOR_UC"] = AttributeUI32VectorUC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC"] = AttributeStringVectorUC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_HOST_UC"] = AttributeHostUC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UI64_UC"] = AttributeUI64UC::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SQL_BETWEEN"] = AttributeSqlBetween::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_SQL_IN"] = AttributeSqlIn::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_UUID"] = AttributeUUID::getSupportedConversions ();
    m_attributeSupportedConversionsMap["WAVE_ATTRIBUTE_TYPE_URI"] = AttributeUri::getSupportedConversions ();

} 

string OrmRepository::getSqlToRemoveAuxilliaryTableEntriesForToEnd (const UI32 fromTableId, const string& relation, const UI32 relatedToTableId, const UI32 tableIdTobeRemoved, const string& waveSchema)
{
    string auxillaryTableName = string ("t") + fromTableId + relation + string ("t") + relatedToTableId;
    tracePrintf (TRACE_LEVEL_INFO, true, false, "OrmRepository::getSqlToRemoveAuxilliaryTableEntriesForToEnd : auxilliaryTable = [%s]", auxillaryTableName.c_str());

    return (string ("DELETE FROM ") + waveSchema + string(".") + auxillaryTableName + string (" WHERE relatedtoclassid = ") + tableIdTobeRemoved + string (";"));
} 

void OrmRepository::getSchemaChangeVectors (vector<string>   & managedObjectNamesForSchemaChange,vector<string>   & fieldNamesStringsForSchemaChange,vector<string> & fieldNamesTypesForSchemaChange,vector<UI32>   & classIds,vector<UI32> & parentTableIds)
{
    managedObjectNamesForSchemaChange   = getManagedObjectNamesForSchemaChange ();
    fieldNamesStringsForSchemaChange    = getFieldNamesStringsForSchemaChange ();
    fieldNamesTypesForSchemaChange      = getFieldNamesExpandedTypesForSchemaChange ();
    classIds                            = getClassIdsForSchemaChange ();
    parentTableIds                      = getParentTableIds ();
    //fieldNamesTypesForSchemaChange =   ((getInstance ())->getFieldNamesTypesForSchemaChange ());
}

/*
ResourceId OrmRepository::updateTableIdsWithTableIdsReceivedFromPrimary (const vector<string> & managedObjectNamesForSchemaChange,const vector<UI32> & classIds, const vector<UI32> & parentTableIds,vector<string> & oldAuxilliaryTableNames,vector<string> & newAuxilliaryTableNames, vector<string> &parentsForAuxilliaryTables, vector<string> &relatedToTables)
{

    vector<pair<string, UI32> > tableClassIdTuples;
    UI32                        numElemsInPrimarySchema = managedObjectNamesForSchemaChange.size();

    //Get all auxilliary table names for all OrmTables
    for (UI32 i = 0; i<numElemsInPrimarySchema; ++i) 
    {
        vector<UI32> parentsForOldAuxilliaryTables;
        vector<UI32> relatedToForOldAuxilliaryTables;
        map<string, OrmTable *>::const_iterator mapIter =  m_tablesMapByName.find (managedObjectNamesForSchemaChange[i]);

        if (mapIter != m_tablesMapByName.end())
        {   
            OrmTable* pOrmTable = mapIter->second;
            pOrmTable->getAuxilliaryTableNamesAppended (oldAuxilliaryTableNames);
            tableClassIdTuples.push_back (pair<string,UI32> (managedObjectNamesForSchemaChange[i], classIds[i]));
        }   
        else
        {
            tracePrintf (TRACE_LEVEL_FATAL, "OrmRepository::updateTableIdsWithTableIdsReceivedFromPrimary: [%s] doesnt exists in Schema.", managedObjectNamesForSchemaChange[i].c_str());
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    updateTableId (tableClassIdTuples);

    // Now calling the auxilliary table APIs will get us the names of the AuxilliaryTables with new classIds
    for(UI32 i = 0; i<numElemsInPrimarySchema; ++i) 
    {
        map<string, OrmTable *>::const_iterator mapIter =  m_tablesMapByName.find (managedObjectNamesForSchemaChange[i]);

        if(mapIter != m_tablesMapByName.end())
        {   
	        OrmTable* pOrmTable = mapIter->second;
	        pOrmTable->getAuxilliaryTableDetailsAppended(newAuxilliaryTableNames, parentsForAuxilliaryTables, relatedToTables);
        }   
    }

    return FRAMEWORK_SUCCESS;
}
*/

const string OrmRepository::getSchemaUpgradeTableName ()                                                                                                                         
{                                                                                                                                                                                
    return ("MoSchemaInfoManagedObject");                                                                                                                                        
}

void OrmRepository::getAllTableNames (set<string> &allTableNames)
{
    OrmRepository *pOrmRepository = getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    UI32 numberOfTables = pOrmRepository->m_tables.size ();
    UI32 i              = 0;

    for (i = 0; i < numberOfTables; i++)
    {
        allTableNames.insert (pOrmRepository->m_tables[i]->getName ());
    }
}

void OrmRepository::getRelationNameToAuxilliaryTableNameMapForOneToNAssociations (const string &tableName, const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getRelationNameToAuxilliaryTableNameMapForOneToNAssociations (selectFieldsInManagedObject, relationNameToAuxilliaryTableNamesMap);
}

void OrmRepository::getRelationNameToAuxilliaryTableNameMapForOneToNCompositions (const string &tableName, const vector<string> &selectFieldsInManagedObject, map<string, string> &relationNameToAuxilliaryTableNamesMap)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();

    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->m_tablesMapByName[tableName];

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getRelationNameToAuxilliaryTableNameMapForOneToNCompositions (selectFieldsInManagedObject, relationNameToAuxilliaryTableNamesMap);
}

UI32 OrmRepository::getNumberOfCompositionRelationsBetweenTables (const string &ownerTableName, const string &childTableName)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);


    OrmTable *pChildOrmTable = pOrmRepository->m_tablesMapByName[childTableName];
    prismAssert (NULL != pChildOrmTable, __FILE__, __LINE__);

    set<string> tableNamesInHierarchy;
    pChildOrmTable->getAllTableNamesInHierarchy (tableNamesInHierarchy);


    OrmTable *pOwnerOrmTable = pOrmRepository->m_tablesMapByName[ownerTableName];
    prismAssert (NULL != pOwnerOrmTable, __FILE__, __LINE__);

    vector<string> childTableNames;
    pOwnerOrmTable->getCompositionRelationTargetTableNamesFromSet (tableNamesInHierarchy, childTableNames);

    UI32 numberOfCompositionRelations = childTableNames.size ();

    return numberOfCompositionRelations;
}

bool OrmRepository::isATableNameInHierarchy (const string &inheritanceChildTableName, const string &inheritanceParentTableName)
{
    bool isInHierarchy = false;

    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable  *pOrmTable = pOrmRepository->getTableByName (inheritanceChildTableName);
    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    isInHierarchy = pOrmTable->isATableNameInHierarchy (inheritanceParentTableName);

    return isInHierarchy;
}

void OrmRepository::getUserDefinedKeyCombinationForTable (const string &managedObjectClassName, vector<string> &userDefinedKeyCombination)
{
    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    prismAssert (NULL != pOrmRepository, __FILE__, __LINE__);

    OrmTable *pOrmTable = pOrmRepository->getTableByName (managedObjectClassName);
    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    pOrmTable->getUserDefinedKeyCombination (userDefinedKeyCombination);
}

}
