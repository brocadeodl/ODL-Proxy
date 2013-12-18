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

#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmView.h"
#include "Framework/Attributes/AttributeTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"
#include "Framework/ObjectModel/WaveManagedView.h"
#include "Framework/ObjectModel/WaveManagedObjectOperation.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/Attributes/AttributeUUID.h"


namespace WaveNs
{

static bool                            s_isObjectRelationalMappingEnabled                     = false;

map<string, map<UI32, string> >        PrismPersistableObject::m_globalUserTagToNameMap;
map<string, map<string, UI32> >        PrismPersistableObject::m_globalNameToUserTagMap;
map<string, map<UI32, AttributeType> > PrismPersistableObject::m_globalUserTagToTypeMap;
PrismMutex                             PrismPersistableObject::m_globalUserTagToNameMapMutex;

PrismPersistableObject::PrismPersistableObject (const string &objectClassName, const string &objectDerivedFromClassName)
    : m_objectClassName            (objectClassName),
      m_objectDerivedFromClassName (objectDerivedFromClassName),
      m_isPreparedForPersistence   (false),
      m_isPreparedForCreate        (false),
      m_emptyNeededOnPersistentBoot (false),
      m_keyString                   (""),
      m_ownerKeyString              (""),
      m_emptyNeededOnPersistentBootWithDefault (false),
      m_isPrismView                 (false),
      m_viewName                    ("")
{
    // In normal cases we will obtain a new object id for every new Managed Object.  However, if we are simply querying the data from a database
    // and are creating a managed object from the query results then we must not obtain a new object id since it will be set from the queried data anyway.
    // Otherwise we will be wasting a new object id.  If there are multiple queries then we will waste multiple object ids.
    // To decide whether to obtain a new objectid, we test if this being bexecuted on DatabaseObjectManager's thread.  If it is DatabaseObjectManager's thread
    // then we will not obtain new object id.

    if (true == (DatabaseObjectManager::getIsDatabaseEnabled ()))
    {
        if ((PrismThread::getSelf ()) != (PrismThread::getPrismThreadForServiceId (DatabaseObjectManager::getPrismServiceId ()))->getId ())
        {
            if (true == (getIsObjectRelationalMappingEnabled ()))
            {
                ObjectId tempObjectId = OrmRepository::getNewObjectId (m_objectClassName);
                m_objectId = tempObjectId;
            }
        }
    }
}

PrismPersistableObject::PrismPersistableObject (const string &viewName)
    : m_objectClassName            (""),
      m_objectDerivedFromClassName (""),
      m_isPreparedForPersistence   (false),
      m_isPreparedForCreate        (false),
      m_emptyNeededOnPersistentBoot (false),
      m_keyString                   (""),
      m_ownerKeyString              (""),
      m_emptyNeededOnPersistentBootWithDefault (false),
      m_isPrismView                 (true),
      m_viewName                    (viewName)
{
}

PrismPersistableObject::~PrismPersistableObject ()
{
}

string PrismPersistableObject::getClassName ()
{
    return ("PrismPersistableObject");
}

void PrismPersistableObject::setIsObjectRelationalMappingEnabled (const bool &isObjectRelationalMappingEnabled)
{
    s_isObjectRelationalMappingEnabled = isObjectRelationalMappingEnabled;
}

bool PrismPersistableObject::getIsObjectRelationalMappingEnabled ()
{
    return (s_isObjectRelationalMappingEnabled);
}

void PrismPersistableObject::addPersistableAttribute (Attribute *pAttribute)
{
    m_persistableAttributes.addAttribute (pAttribute);

	UI32          attributeUserTag = pAttribute->getAttributeUserTag ();
    string        attributeName    = pAttribute->getAttributeName    ();
    AttributeType attributeType    = pAttribute->getAttributeType    ();

	if (attributeUserTag != 0)
	{
		m_attributeUserTags.insert (m_attributeUserTags.end (), attributeUserTag);

        addGlobalUserTagToNameCombination (m_objectClassName, attributeUserTag, attributeName, attributeType);
	}

	// trace (TRACE_LEVEL_DEBUG, string ("PrismPersistableObject::addPersistableAttribute:") + pAttribute->getAttributeName() + ',' + attributeUserTag);
}

void PrismPersistableObject::addPersistableAttributeForCreate (Attribute *pAttribute)
{
    m_persistableAttributesForCreate.addAttribute (pAttribute);
}

void PrismPersistableObject::setupAttributesForPersistence ()
{
    addPersistableAttribute (new AttributeObjectId (&m_objectId,                       "objectId"));
    addPersistableAttribute (new AttributeUUID     (&m_objectUuid,                     "objectUuid"));
    addPersistableAttribute (new AttributeString   (&m_keyString,                      "keyString"));
    addPersistableAttribute (new AttributeString   (&m_ownerKeyString,                 "ownerKeyString"));
    addPersistableAttribute (new AttributeString   (&m_userDefinedKeyCombinationValue, "userDefinedKeyCombinationValue"));
}

void PrismPersistableObject::setupAttributesForCreate ()
{
    AttributeObjectId *pAttributeObjectId                       = new AttributeObjectId (&m_objectId,                       "objectId");
    AttributeUUID     *pAttributeUuid                           = new AttributeUUID     (&m_objectUuid,                     "objectUuid");
    AttributeString   *pAttributeKeyString                      = new AttributeString   (&m_keyString,                      "keyString");
    AttributeString   *pAttributeOwnerKeyString                 = new AttributeString   (&m_ownerKeyString,                 "ownerKeyString");
    AttributeString   *pAttributeUserDefinedKeyCombinationValue = new AttributeString   (&m_userDefinedKeyCombinationValue, "userDefinedKeyCombinationValue");

    pAttributeObjectId->setIsPrimary (true);

    addPersistableAttributeForCreate (pAttributeObjectId);
    addPersistableAttributeForCreate (pAttributeUuid);
    addPersistableAttributeForCreate (pAttributeKeyString);
    addPersistableAttributeForCreate (pAttributeOwnerKeyString);
    addPersistableAttributeForCreate (pAttributeUserDefinedKeyCombinationValue);
}

void PrismPersistableObject::prepareForSerialization ()
{
    if (false == m_isPreparedForPersistence)
    {
        setupAttributesForPersistence ();
        m_isPreparedForPersistence = true;
    }
}

void PrismPersistableObject::prepareForCreate ()
{
    if (false == m_isPreparedForCreate)
    {
        setupAttributesForCreate ();
        m_isPreparedForCreate = true;
    }
}

string PrismPersistableObject::getObjectClassName () const
{
    return (m_objectClassName);
}

string PrismPersistableObject::getObjectDerivedFromClassName ()
{
    return (m_objectDerivedFromClassName);
}

string PrismPersistableObject::getSqlForCreate (const string &waveSchema)
{
    string sqlStringForCreate;

    sqlStringForCreate  = "CREATE TABLE ";
    sqlStringForCreate += waveSchema + "." + m_objectClassName;
    sqlStringForCreate += "\n(\n";
    sqlStringForCreate += m_persistableAttributes.getSqlForCreate () + "\n";
    sqlStringForCreate += ");";

    return (sqlStringForCreate);
}

void PrismPersistableObject::getSqlForInsert (string &sqlStringForInsert, const string &waveSchema)
{
    string sqlStringForPreValues;
    string sqlStringForValues;
    string sqlStringForInsert2;

    prepareForSerialization ();

    m_persistableAttributes.getSqlForInsert (sqlStringForPreValues, sqlStringForValues, sqlStringForInsert2);

    sqlStringForInsert += "INSERT INTO ";
    sqlStringForInsert += waveSchema + "." + m_objectClassName + " (";
    sqlStringForInsert += sqlStringForPreValues;
    sqlStringForInsert += ") VALUES (";
    sqlStringForInsert += sqlStringForValues;
    sqlStringForInsert += ");";

    OrmRepository::getSqlForInsertForDerivationsInstances (m_objectClassName, sqlStringForInsert, m_objectId.getInstanceId (), waveSchema);

    sqlStringForInsert += sqlStringForInsert2;
}

void PrismPersistableObject::getSqlForUpdate (string &sqlStringForUpdate, const string &waveSchema)
{
          string sqlStringForUpdate2;
    const UI32   numberOfAttributesToBeUpdated = m_attributesToBeUpdated.size ();

    prepareForSerialization ();

    sqlStringForUpdate += "UPDATE ";
    sqlStringForUpdate += waveSchema + "." + m_objectClassName;
    sqlStringForUpdate += " SET ";

    if (0 == numberOfAttributesToBeUpdated)
    {
        m_persistableAttributes.getSqlForUpdate (sqlStringForUpdate, sqlStringForUpdate2);
    }
    else
    {
        m_persistableAttributes.getSqlForUpdate (sqlStringForUpdate, sqlStringForUpdate2, m_attributesToBeUpdated);
    }

    if (ObjectId::NullObjectId != getObjectId ())
    {
        /* 
         * This indicates generating WHERE clause for an empty MO.
         * It is used in updating Multiple WMO using query context.
         * The WHERE condition is generated in the WaveManagedObjectQueryContextForUpdate class
         */
        sqlStringForUpdate += string (" WHERE objectIdClassId = ") + m_objectId.getClassId () + string (" AND objectIdInstanceId = ") + m_objectId.getInstanceId () + ";";
        sqlStringForUpdate += sqlStringForUpdate2;
    }
}

void PrismPersistableObject::setupOrm ()
{
    OrmRepository              *pOrmRepository                      = OrmRepository::getInstance ();
    OrmTable                   *pOrmTable                           = NULL;
    OrmView                    *pOrmView                            = NULL;
    UI32                        numberOfEntriesInKeyCombination     = 0;
    UI32                        i                                   = 0;
    map<string, Attribute*>     userDefinedKeyCombinationWithTypes;
    bool                        isALocalManagedObject               = false;
    bool                        isAManagedView                      = false;

    // Determine if the object class is a local managed object and constuct ORM Table accordingly
    if (NULL != (dynamic_cast<WaveManagedView *> (this)))
    {
        //trace (TRACE_LEVEL_INFO, "PrismPersistableObject::setupOrm : " + m_viewName + " -> isAManagedView");
        isAManagedView= true;
    }
    else
    {
        //trace (TRACE_LEVEL_INFO, "PrismPersistableObject::setupOrm : " + m_objectClassName + " -> isNotAManagedView");
        isAManagedView = false;
    }

    if (isAManagedView != m_isPrismView)
    {
        trace (TRACE_LEVEL_FATAL, "PrismPersistableObject::setupOrm : Classes derived from WaveManagedView should set isPrismView to true in PrismPersistebleObject constructor.");
        prismAssert (false, __FILE__, __LINE__);
    }
    
    if (false == isAManagedView)
    {
        // Determine if the object class is a local managed object and constuct ORM Table accordingly
        if (NULL != (dynamic_cast<WaveLocalManagedObjectBase *> (this)))
        {
            //trace (TRACE_LEVEL_INFO, "PrismPersistableObject::setupOrm : " + m_objectClassName + " -> LocalManagedObject");
            isALocalManagedObject = true;
        }
        else
        {
            //trace (TRACE_LEVEL_INFO, "PrismPersistableObject::setupOrm : " + m_objectClassName + " -> GlobalManagedObject");
            isALocalManagedObject = false;
        }

        pOrmTable = new OrmTable (m_objectClassName, m_objectDerivedFromClassName, isALocalManagedObject);

        if (NULL == pOrmTable)
        {
            trace (TRACE_LEVEL_FATAL, "PrismPersistableObject::setupOrm : Failed to allocate a new ORM Table.");
            prismAssert (false, __FILE__, __LINE__);
        }

        // Next Call the setupKeys Virtual Function.

        setupKeys ();

        // Prepare for general Serialization at run time and prepate for Schema Creation  phase.

        prepareForSerialization ();
        prepareForCreate        ();

        //m_persistableAttributes.computeAttributesByName ();

        pOrmTable->setEmptyNeededOnPersistentBoot (m_emptyNeededOnPersistentBoot);
        pOrmTable->setEmptyNeededOnPersistentBootWithDefault (m_emptyNeededOnPersistentBootWithDefault);

        m_persistableAttributesForCreate.setupOrm (pOrmTable);

        // Check for user defined key combination if any

        numberOfEntriesInKeyCombination = m_userDefinedKeyCombination.size ();

        for (i = 0; i < numberOfEntriesInKeyCombination; i++)
        {
            Attribute *pAttribute = m_persistableAttributes.getAttribute (m_userDefinedKeyCombination[i]);

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            userDefinedKeyCombinationWithTypes[pAttribute->getAttributeName ()] = pAttribute->clone ();
        }

        pOrmTable->setUserDefinedKeyCombinationWithTypes (userDefinedKeyCombinationWithTypes, m_userDefinedKeyCombination);

        pOrmRepository->addTable (pOrmTable);

        for (i=0; i < m_persistableAttributesForCreate.getSize (); i++)
        {
            pOrmTable->addManagedObjectAttributes (m_persistableAttributesForCreate.getAttributeAt (i));
        }
    }
    else
    {
        pOrmView = new OrmView (m_viewName);

        if (NULL == pOrmView)
        {
            trace (TRACE_LEVEL_FATAL, "PrismPersistableObject::setupOrm : Failed to allocate a new ORM View.");
            prismAssert (false, __FILE__, __LINE__);
        }

        string viewDefinition = getSqlForCreateView ();
        
        if (true == viewDefinition.empty ())
        {
            viewDefinition = computeSqlForCreateView ();

            trace (TRACE_LEVEL_INFO, "PrismPersistableObject::setupOrm : viewDefinition " + viewDefinition);
        }
            
        pOrmView->setSqlForCreateView (viewDefinition);

        // Prepare for general Serialization at run time and prepate for Schema Creation  phase.

        prepareForSerialization ();
        prepareForCreate        ();

        //m_persistableAttributes.computeAttributesByName ();

        m_persistableAttributesForCreate.setupOrm (pOrmView);

        pOrmRepository->addView (pOrmView);
    }
}

string PrismPersistableObject::getSqlForCreateView ()
{
    return "";
}

void PrismPersistableObject::getSetContextForCreateView (WaveManagedObjectSynchronousQueryContextForSetOperation &viewContext)
{
}

string PrismPersistableObject::computeSqlForCreateView ()
{
    /*WaveManagedObjectSynchronousQueryContextForSetOperation *pViewContext = new WaveManagedObjectSynchronousQueryContextForSetOperation ();

    prismAssert (NULL != pViewContext, __FILE__, __LINE__);

    getSetContextForCreateView (pViewContext);

    string  sqlForCreateView  = "CREATE OR REPLACE VIEW ";
            sqlForCreateView += OrmRepository::getWaveCurrentSchema () + "." + m_viewName + " AS ";
            sqlForCreateView += pViewContext->getSql ();

    delete pViewContext;
    pViewContext = NULL;*/

    WaveManagedObjectSynchronousQueryContextForSetOperation viewContext;

    getSetContextForCreateView (viewContext);

    string  sqlForCreateView  = "CREATE OR REPLACE VIEW ";
            sqlForCreateView += OrmRepository::getWaveCurrentSchema () + "." + m_viewName + " AS ";
            sqlForCreateView += viewContext.getSql();


    return (sqlForCreateView);
}

ObjectId PrismPersistableObject::getObjectId () const
{
    return (m_objectId);
}

Uuid PrismPersistableObject::getObjectUuid () const
{
    return (m_objectUuid);
}

void PrismPersistableObject::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager, const bool loadOneToManyRelationships, const bool loadCompositions)
{
    prepareForSerialization ();

    m_persistableAttributes.loadFromPostgresQueryResult (pResult, row, schema, selectFields, pWaveObjectManager, loadOneToManyRelationships, loadCompositions);
}

void PrismPersistableObject::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager, const bool loadCompositions)
{
    prepareForSerialization ();

    // By now the object must have been obtained properly (through the class to loadFromPostgresQueryResult).  So obtain it and pass it on to Attributes.  1-* association related attributes will use it.

    ObjectId tempObjectId = getObjectId ();

    m_persistableAttributes.loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, tempObjectId, schema, selectFields, pWaveObjectManager, loadCompositions);
}

vector<string> PrismPersistableObject::getUserDefinedKeyCombination () const
{
    return (m_userDefinedKeyCombination);
}

string PrismPersistableObject::getUserDefinedKeyCombinationValue () const
{
    return (m_userDefinedKeyCombinationValue);
}

void PrismPersistableObject::setUserDefinedKeyCombination (const vector<string> &userDefinedKeyCombination)
{
    m_userDefinedKeyCombination = userDefinedKeyCombination;
}

Attribute *PrismPersistableObject::getAttributeByName (const string &attributeName)
{
    return (m_persistableAttributes.getAttribute (attributeName));
}

Attribute *PrismPersistableObject::getAttributeByUserTag (const UI32 &attributeUserTag)
{
    return (m_persistableAttributes.getAttributeByUserTag (attributeUserTag));
}

Attribute *PrismPersistableObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
{
    // This function can be used to return user defined attribute if getAttributeByUserTag returns NULL
    // in some case
    return (NULL);
}

void PrismPersistableObject::setUserTagForAttribute (const string &attributeName, const UI32 &attributeUserTag)
{
    m_persistableAttributes.setUserTagForAttribute (attributeName, attributeUserTag);

    Attribute *pAttribute = getAttributeByName (attributeName);

    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    AttributeType attributeType = pAttribute->getAttributeType    ();

    if (attributeUserTag != 0)
    {
        m_attributeUserTags.insert (m_attributeUserTags.end (), attributeUserTag);

        addGlobalUserTagToNameCombination (m_objectClassName, attributeUserTag, attributeName, attributeType);
    }
}

void PrismPersistableObject::updateOrmRelations ()
{
    prepareForSerialization ();

    m_persistableAttributes.updateOrmRelations (this);

    // only compute the keystring if it has not been set previously
    //if ("" == m_keyString)
    //{
        computeKeyString ();
    //}
}

void PrismPersistableObject::setupKeys ()
{
}

WaveManagedObject *PrismPersistableObject::getComposedManagedObject (const ObjectId &childObjectId)
{
    return (m_persistableAttributes.getComposedManagedObject(childObjectId));
}

UI32 PrismPersistableObject::getCase (const UI32 &attributeUserTag)
{
    trace (TRACE_LEVEL_ERROR, "PrismPersistableObject::getCase: Derived Class implementation must override this function");

    prismAssert (false, __FILE__, __LINE__);

    return 0;
}

vector<UI32> PrismPersistableObject::getAttributeUserTags () const
{
	return (m_attributeUserTags);
}

void PrismPersistableObject::setEmptyNeededOnPersistentBoot (bool emptyNeeded)
{
    m_emptyNeededOnPersistentBoot = emptyNeeded;
}

void PrismPersistableObject::getShowDump (string &showDump, const WaveManagedObjectShowType &showType)
{
    m_persistableAttributes.getShowDump (showDump, showType);
}

void PrismPersistableObject::getRawShowDump (string &showDump, const WaveManagedObjectShowType &showType)
{
    m_persistableAttributes.getRawShowDump (showDump, showType);
}

void PrismPersistableObject::addGlobalUserTagToNameCombination   (const string &className, const UI32 &userTag, const string &name, const AttributeType &attributeType)
{
    m_globalUserTagToNameMapMutex.lock ();

    m_globalUserTagToNameMap[className][userTag] = name;
    m_globalUserTagToTypeMap[className][userTag] = attributeType;
    m_globalNameToUserTagMap[className][name]    = userTag;

    m_globalUserTagToNameMapMutex.unlock ();
}

string PrismPersistableObject::getNameFromUserTag (const string &className, const UI32 &userTag)
{
    string name;

    m_globalUserTagToNameMapMutex.lock ();

    name = m_globalUserTagToNameMap[className][userTag];

    m_globalUserTagToNameMapMutex.unlock ();

    return (name);
}

UI32 PrismPersistableObject::getUserTagFromName (const string &className, const string &attributeName)
{
    UI32 userTag = 0;

    m_globalUserTagToNameMapMutex.lock ();

    map<string, map<string, UI32> >::iterator firstMapIterator;

    firstMapIterator = m_globalNameToUserTagMap.find (className);

    if (firstMapIterator != m_globalNameToUserTagMap.end ())
    {
        map<string, UI32>::iterator secondMapIterator;

        secondMapIterator = (firstMapIterator->second).find (attributeName);

        if (secondMapIterator != (firstMapIterator->second).end ())
        {
            userTag = secondMapIterator->second;
        }
    }

    m_globalUserTagToNameMapMutex.unlock ();

    return (userTag);
}

AttributeType PrismPersistableObject::getTypeFromUserTag (const string &className, const UI32 &userTag)
{
    AttributeType attributeType;

    m_globalUserTagToNameMapMutex.lock ();

    attributeType = m_globalUserTagToTypeMap[className][userTag];

    m_globalUserTagToNameMapMutex.unlock ();

    return (attributeType);
}

bool PrismPersistableObject::isHierarchyDeletableForOperation (const WaveManagedObjectOperation &operation)
{
    return (m_persistableAttributes.isDeletableForOperation (operation));
}

void PrismPersistableObject::getAttributeNamesForHtmlTable (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType)
{
    m_persistableAttributes.getAttributeNamesForHtmlTable (attributeNamesForHtmlTable, showType);
}

void PrismPersistableObject::getHtmlTableHeaderNames (vector<string> &htmlTableHeaderNames, const WaveManagedObjectShowType &showType)
{
    vector<string> attributeNamesForHtmlTable;

    getAttributeNamesForHtmlTable (attributeNamesForHtmlTable, showType);

    m_persistableAttributes.getHtmlTableHeaderNames (attributeNamesForHtmlTable, htmlTableHeaderNames, showType);
}

void PrismPersistableObject::getHtmlTableHeaderNamesString (string &htmlTableHeaderNamesString, const WaveManagedObjectShowType &showType)
{
    vector<string> htmlTableHeaderNames;

    getHtmlTableHeaderNames (htmlTableHeaderNames, showType);

    m_persistableAttributes.getHtmlTableHeaderNamesString (htmlTableHeaderNames, htmlTableHeaderNamesString, showType);
}

void PrismPersistableObject::getHtmlTableRowData (vector<string> &htmlTableRowData, const WaveManagedObjectShowType &showType)
{
    vector<string> attributeNamesForHtmlTable;

    getAttributeNamesForHtmlTable (attributeNamesForHtmlTable, showType);

    m_persistableAttributes.getHtmlTableRowData (attributeNamesForHtmlTable, htmlTableRowData, showType);
}

void PrismPersistableObject::getHtmlTableRowDataString (string &htmlTableRowDataString, const WaveManagedObjectShowType &showType)
{
    vector<string> attributeNamesForHtmlTable;

    getAttributeNamesForHtmlTable (attributeNamesForHtmlTable, showType);

    m_persistableAttributes.getHtmlTableRowDataString (attributeNamesForHtmlTable, htmlTableRowDataString, showType);
}

void PrismPersistableObject::getManagedObjectClassNameForRest (string &managedObjectClassNameForRest) const
{
    managedObjectClassNameForRest = m_objectClassName;
}

void PrismPersistableObject::getAttributeNamesForRest (vector<string> &attributeNamesForRest, const WaveManagedObjectShowType &showType)
{
    getAttributeNamesForHtmlTable (attributeNamesForRest, showType);
}

void PrismPersistableObject::getRestHeaderNames (vector<string> &restHeaderNames, const WaveManagedObjectShowType &showType)
{
    getHtmlTableHeaderNames (restHeaderNames, showType);
}

void PrismPersistableObject::getRestRowData (string &restRowData, const WaveManagedObjectShowType &showType)
{
    vector<string> attributeNamesForRest;
    vector<string> restHeaderNames;
    UI32           numberOfAttributeNamesForRest  = 0;
    UI32           numberOfRestHeaderNames        = 0;
    string         managedObjectClassNameForRest;

    getAttributeNamesForRest (attributeNamesForRest, showType);
    getRestHeaderNames       (restHeaderNames,       showType);

    numberOfAttributeNamesForRest = attributeNamesForRest.size ();
    numberOfRestHeaderNames       = restHeaderNames.size       ();

    prismAssert (numberOfAttributeNamesForRest == numberOfRestHeaderNames, __FILE__, __LINE__);

    getManagedObjectClassNameForRest (managedObjectClassNameForRest);

    m_persistableAttributes.getRestRowData (attributeNamesForRest, restHeaderNames, managedObjectClassNameForRest, restRowData, showType);
}

void PrismPersistableObject::getRestRowData (string &restRowData, const vector<string> &attributeNamesForRestDefinedByUser)
{
    vector<string>      attributeNamesForRest;
    vector<string>      restHeaderNames;
    UI32                numberOfAttributeNamesForRest              = 0;
    UI32                numberOfRestHeaderNames                    = 0;
    string              managedObjectClassNameForRest;
    map<string, string> restHeaderNamesByAttributeNames;
    UI32                i                                          = 0;
    UI32                numberOfAttributeNamesForRestDefinedByUser = attributeNamesForRestDefinedByUser.size ();
    vector<string>      restHeaderNamesDefinedByUser;

    getAttributeNamesForRest (attributeNamesForRest);
    getRestHeaderNames       (restHeaderNames);

    numberOfAttributeNamesForRest = attributeNamesForRest.size ();
    numberOfRestHeaderNames       = restHeaderNames.size       ();

    prismAssert (numberOfAttributeNamesForRest == numberOfRestHeaderNames, __FILE__, __LINE__);

    getManagedObjectClassNameForRest (managedObjectClassNameForRest);

    for (i = 0; i < numberOfAttributeNamesForRest; i++)
    {
        restHeaderNamesByAttributeNames[attributeNamesForRest[i]] = restHeaderNames[i];
    }

    for (i = 0; i < numberOfAttributeNamesForRestDefinedByUser; i++)
    {
        restHeaderNamesDefinedByUser.push_back (restHeaderNamesByAttributeNames[attributeNamesForRestDefinedByUser[i]]);
    }

    m_persistableAttributes.getRestRowData (attributeNamesForRestDefinedByUser, restHeaderNamesDefinedByUser, managedObjectClassNameForRest, restRowData);
}

void PrismPersistableObject::getAttributeNamesForJson (vector<string> &attributeNamesForJson, const WaveManagedObjectShowType &showType)
{
    m_persistableAttributes.getAttributeNamesForJson (attributeNamesForJson, showType);
}

void PrismPersistableObject::getJsonNames (vector<string> &jsonNames, const WaveManagedObjectShowType &showType)
{
    vector<string> attributeNamesForJson;

    getAttributeNamesForJson (attributeNamesForJson, showType);

    m_persistableAttributes.getJsonNames (attributeNamesForJson, jsonNames);
}

void PrismPersistableObject::getManagedObjectClassNameForJson (string &managedObjectClassNameForJson) const
{
    managedObjectClassNameForJson = m_objectClassName;
}

void PrismPersistableObject::getJsonObjectData (string &jsonObjectData, const WaveManagedObjectShowType &showType)
{
    vector<string> attributeNamesForJson;
    vector<string> jsonNames;
    UI32           numberOfAttributeNamesForJson = 0;
    UI32           numberOfJsonNames             = 0;
    string         managedObjectClassNameForJson;

    getAttributeNamesForJson (attributeNamesForJson, showType);
    getJsonNames             (jsonNames,             showType);

    numberOfAttributeNamesForJson = attributeNamesForJson.size ();
    numberOfJsonNames             = jsonNames.size             ();

    prismAssert (numberOfAttributeNamesForJson == numberOfJsonNames, __FILE__, __LINE__);

    getManagedObjectClassNameForJson (managedObjectClassNameForJson);

    m_persistableAttributes.getJsonObjectData (attributeNamesForJson, jsonNames, managedObjectClassNameForJson, jsonObjectData);
}

void PrismPersistableObject::getJsonObjectData (string &jsonObjectData, const vector<string> &attributeNamesForJsonDefinedByUser)
{
    vector<string> jsonNames;
    UI32           numberOfAttributeNamesForJson = 0;
    UI32           numberOfJsonNames             = 0;
    string         managedObjectClassNameForJson;

    m_persistableAttributes.getJsonNames (attributeNamesForJsonDefinedByUser, jsonNames);

    numberOfAttributeNamesForJson = attributeNamesForJsonDefinedByUser.size ();
    numberOfJsonNames             = jsonNames.size                          ();

    prismAssert (numberOfAttributeNamesForJson == numberOfJsonNames, __FILE__, __LINE__);

    getManagedObjectClassNameForJson (managedObjectClassNameForJson);

    m_persistableAttributes.getJsonObjectData (attributeNamesForJsonDefinedByUser, jsonNames, managedObjectClassNameForJson, jsonObjectData);
}

void PrismPersistableObject::computeKeyString ()
{
    vector<string>  keyNames;
    UI32            i         = 0;
    OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (m_objectClassName, keyNames);

    m_keyString = m_objectClassName;

    m_userDefinedKeyCombinationValue = "";

    if (0 != keyNames.size ())
    {
        // Iterate through keys
        vector <string>::iterator keyElement;
        for (keyElement = keyNames.begin (); keyElement != keyNames.end (); keyElement++)
        {
            Attribute *attr = getAttributeByName(*keyElement);

            if (NULL != attr)
            {
                string keyValue;
                attr->toString (keyValue);
                m_keyString += "*" + keyValue;

                if (0 != i)
                {
                    m_userDefinedKeyCombinationValue += " " + keyValue;
                }
                else
                {
                    m_userDefinedKeyCombinationValue += keyValue;
                }
            }
            else
            {
                trace (TRACE_LEVEL_DEBUG, string ("PrismPersistableObject::computeKeyString: no attribute for keyName ") + *keyElement + " in MO " + m_objectClassName);
            }

            i++;
        }
    }

    if ("" != m_ownerKeyString)
    {
        m_keyString = m_ownerKeyString + "." + m_keyString;
    }

    trace (TRACE_LEVEL_DEBUG, string ("PrismPersistableObject::computeKeyString:" + m_keyString) + " for MO " + m_objectClassName);
}

string PrismPersistableObject::getKeyString () const
{
    return m_keyString;
}

void PrismPersistableObject::prependOwnerKeyString (const string &keyString)
{
    m_ownerKeyString = keyString;
    computeKeyString ();
    trace (TRACE_LEVEL_DEBUG, string ("PrismPersistableObject::prependOwnerKeyString:" + m_keyString) + " for MO " + m_objectClassName);
}

void PrismPersistableObject::updateKeyString ()
{
    m_persistableAttributes.updateKeyString (this);
}

vector<string> PrismPersistableObject::getAttributesToBeUpdated () const
{
    return (m_attributesToBeUpdated);
}

void PrismPersistableObject::setAttributesToBeUpdated (const vector<string> &attributesToBeUpdated)
{
    if (ObjectId::NullObjectId != getObjectId ())
    {
        m_attributesToBeUpdated = attributesToBeUpdated;

        if (0 != (m_attributesToBeUpdated.size ()))
        {
            m_attributesToBeUpdated.insert (m_attributesToBeUpdated.begin (), "lastModifiedTimeStamp");
            m_attributesToBeUpdated.insert (m_attributesToBeUpdated.begin (), "objectId");
        }
    }
    else
    {
        /* 
         * This indicates setting attributes for an empty MO.
         * It is used in updating Multiple WMO using query context.
         * Limitations: Bulk Update doesn't support updating of these attributes:
         *      1. AttributeTypeObjectIdVector
         *      2. AttributeTypeComposition 
         *      3. AttributeTypeCompositionVector 
         */
         
        UI32 numberOfAttributesToBeUpdated  = attributesToBeUpdated.size ();
        UI32 i                              = 0;

        for (i = 0; i < numberOfAttributesToBeUpdated; i++)
        {
            AttributeType attributeTypeToBeUpdated = (getAttributeByName (attributesToBeUpdated[i]))->getAttributeType ();
            if (attributeTypeToBeUpdated == AttributeType::AttributeTypeObjectIdVector ||
                attributeTypeToBeUpdated == AttributeType::AttributeTypeComposition ||
                attributeTypeToBeUpdated == AttributeType::AttributeTypeCompositionVector)
            {
                trace (TRACE_LEVEL_ERROR, "PrismPersistableObject::addAttributeToBeUpdated: Attribute type cannot be ObjectIdVector, Composition or CompositionVector for InMemory Empty Managed Objects");
                prismAssert (false , __FILE__, __LINE__);
            }

        }
        m_attributesToBeUpdated = attributesToBeUpdated;

        if (0 != (m_attributesToBeUpdated.size ()))
        {
            m_attributesToBeUpdated.insert (m_attributesToBeUpdated.begin (), "lastModifiedTimeStamp");
        }
    }
}

void PrismPersistableObject::addAttributeToBeUpdated (const string &attributeName)
{
    prismAssert ("" != attributeName, __FILE__, __LINE__);

    if (ObjectId::NullObjectId != getObjectId ())
    {
        if (0 == (m_attributesToBeUpdated.size ()))
        {
            m_attributesToBeUpdated.insert (m_attributesToBeUpdated.begin (), "lastModifiedTimeStamp");
            m_attributesToBeUpdated.insert (m_attributesToBeUpdated.begin (), "objectId");
        }

        m_attributesToBeUpdated.push_back (attributeName);
    }
    else 
    {
        /* 
         * This indicates setting attributes for an empty MO.
         * It is used in updating Multiple WMO using query context.
         * Limitations: Bulk Update doesn't support updating of these attributes:
         *      1. AttributeTypeObjectIdVector
         *      2. AttributeTypeComposition 
         *      3. AttributeTypeCompositionVector 
         */

        AttributeType attributeTypeToBeUpdated = (getAttributeByName (attributeName))->getAttributeType ();
        if (attributeTypeToBeUpdated == AttributeType::AttributeTypeObjectIdVector ||
            attributeTypeToBeUpdated == AttributeType::AttributeTypeComposition ||
            attributeTypeToBeUpdated == AttributeType::AttributeTypeCompositionVector)
        {
            trace (TRACE_LEVEL_ERROR, "PrismPersistableObject::addAttributeToBeUpdated: Attribute type cannot be ObjectIdVector, Composition or CompositionVector for InMemory Empty Managed Objects");
            prismAssert (false , __FILE__, __LINE__);
        }

        if (0 == (m_attributesToBeUpdated.size ()))
        {
            m_attributesToBeUpdated.insert (m_attributesToBeUpdated.begin (), "lastModifiedTimeStamp");
        }

        m_attributesToBeUpdated.push_back (attributeName);
    }
}

void PrismPersistableObject::clearAttributesToBeUpdated ()
{
    m_attributesToBeUpdated.clear ();
}

void PrismPersistableObject::setDisableValidations (const string &attributeName, const bool disableValidations)
{
    Attribute *pAttribute = m_persistableAttributesForCreate.getAttribute (attributeName);

    prismAssert ((AttributeType::AttributeTypeCompositionVector == pAttribute->getAttributeType ()), __FILE__, __LINE__);

    AttributeManagedObjectVectorCompositionTemplateBase *pAttributeManagedObjectVectorCompositionTemplateBase = dynamic_cast<AttributeManagedObjectVectorCompositionTemplateBase *> (pAttribute);

    prismAssert (NULL != pAttributeManagedObjectVectorCompositionTemplateBase, __FILE__, __LINE__);

    
    pAttributeManagedObjectVectorCompositionTemplateBase->setDisableValidations (disableValidations);
    
}

void PrismPersistableObject::setEmptyNeededOnPersistentBootWithDefault (const bool emptyNeededWithDefault)
{
    m_emptyNeededOnPersistentBootWithDefault = emptyNeededWithDefault;
}

void PrismPersistableObject::loadFromPostgresQueryResult2 (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager)
{
    prepareForSerialization ();

    m_persistableAttributes.loadFromPostgresQueryResult2 (pResult, row, schema, selectFields, pWaveObjectManager);
}

void PrismPersistableObject::getOidsOfOneToOneCompositions (vector<ObjectId> &vectorOfCompositionOids)
{
    m_persistableAttributes.getOidsOfOneToOneCompositions (vectorOfCompositionOids);
}

void PrismPersistableObject::popOneToOneCompositionsFromResults (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap, const vector<string> &selectFieldsInManagedObject)
{
    m_persistableAttributes.popOneToOneCompositionsFromResults (oidTopManagedObjectMap, selectFieldsInManagedObject);
}

void PrismPersistableObject::storeRelatedObjectIdVectorForAOneToNAssociation (const string &relationName, const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds)
{
    m_persistableAttributes.storeRelatedObjectIdVectorForAOneToNAssociation (relationName, parentObjectId, vectorOfRelatedObjectIds);
}

void PrismPersistableObject::storeRelatedObjectVectorForAOneToNComposition (const string &relationName, const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects)
{
    m_persistableAttributes.storeRelatedObjectVectorForAOneToNComposition (relationName, parentObjectId, vectorOfRelatedObjects);
}

}
