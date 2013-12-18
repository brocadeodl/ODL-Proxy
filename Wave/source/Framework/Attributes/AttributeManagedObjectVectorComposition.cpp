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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectRelationalMapping/OrmColumn.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/ObjectRelationalMapping/OrmAssociation.h"
#include "Framework/ObjectRelationalMapping/OrmAggregation.h"
#include "Framework/ObjectRelationalMapping/OrmComposition.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/ObjectModel/WaveManagedObjectOperation.h"

#include <vector>

using namespace std;

namespace WaveNs
{

template <class T> AttributeManagedObjectVectorComposition<T>::AttributeManagedObjectVectorComposition (const vector <WaveManagedObjectPointer<T> > &data, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectVectorCompositionTemplateBase         (AttributeType::AttributeTypeCompositionVector, attributeName, attributeUserTag, isOperational),
      m_parent                                                    (parent),
      m_parentObjectId                                            (parentObjectId),
      m_composedWith                                              (composedWith),
      m_isDefaultDataValid                                        (false)
{
     m_pData = new vector <WaveManagedObjectPointer<T> >;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

template <class T> AttributeManagedObjectVectorComposition<T>::AttributeManagedObjectVectorComposition (const vector <WaveManagedObjectPointer<T> > &data, const vector <WaveManagedObjectPointer<T> > &defaultData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectVectorCompositionTemplateBase (AttributeType::AttributeTypeCompositionVector, attributeName, attributeUserTag, isOperational),
      m_parent             (parent),
      m_parentObjectId     (parentObjectId),
      m_composedWith       (composedWith),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (true)
{
     m_pData = new vector <WaveManagedObjectPointer<T> >;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

template <class T> AttributeManagedObjectVectorComposition<T>::AttributeManagedObjectVectorComposition (vector <WaveManagedObjectPointer<T> > *pData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectVectorCompositionTemplateBase        (AttributeType::AttributeTypeCompositionVector, attributeName, attributeUserTag, isOperational),
      m_parent                                                   (parent),
      m_parentObjectId                                           (parentObjectId),
      m_composedWith                                             (composedWith),
      m_isDefaultDataValid                                       (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

template <class T> AttributeManagedObjectVectorComposition<T>::AttributeManagedObjectVectorComposition (vector <WaveManagedObjectPointer<T> > *pData, const vector <WaveManagedObjectPointer<T> > &defaultData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectVectorCompositionTemplateBase (AttributeType::AttributeTypeCompositionVector, attributeName, attributeUserTag, isOperational),
      m_parent             (parent),
      m_parentObjectId     (parentObjectId),
      m_composedWith       (composedWith),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

template <class T> AttributeManagedObjectVectorComposition<T>::AttributeManagedObjectVectorComposition (const AttributeManagedObjectVectorComposition &attribute)
    : AttributeManagedObjectVectorCompositionTemplateBase (attribute)
{
    m_parent             = attribute.m_parent;
    m_parentObjectId     = attribute.m_parentObjectId;
    m_composedWith       = attribute.m_composedWith;

    m_pData              = new vector <WaveManagedObjectPointer<T> >;
    *m_pData             = attribute.getValue ();

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

template <class T> AttributeManagedObjectVectorComposition<T>::~AttributeManagedObjectVectorComposition ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

template <class T> AttributeManagedObjectVectorComposition<T> &AttributeManagedObjectVectorComposition<T>::operator = (const AttributeManagedObjectVectorComposition &attribute)
{
    AttributeManagedObjectVectorCompositionTemplateBase::operator = (attribute);

    m_parent               = attribute.m_parent;
    m_parentObjectId       = attribute.m_parentObjectId;
    m_composedWith         = attribute.m_composedWith;
    m_compositionTableName = attribute.m_compositionTableName;

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

template <class T> vector <WaveManagedObjectPointer<T> > AttributeManagedObjectVectorComposition<T>::getValue () const
{
    vector <WaveManagedObjectPointer<T> > temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeManagedObjectVectorComposition::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

template <class T> void AttributeManagedObjectVectorComposition<T>::setValue (const vector <WaveManagedObjectPointer<T> > &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeManagedObjectVectorComposition::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

template <class T> bool AttributeManagedObjectVectorComposition<T>::validate () const
{
    prismAssert (AttributeType::AttributeTypeCompositionVector == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeCompositionVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

template <class T> string AttributeManagedObjectVectorComposition<T>::getSqlType ()
{
    return ("varchar");
}

template <class T> void AttributeManagedObjectVectorComposition<T>::setupOrm (OrmTable *pOrmTable)
{
    OrmComposition *pOrmComposition = new OrmComposition (getAttributeName (), m_composedWith, ORM_RELATION_TYPE_ONE_TO_MANY);

    pOrmComposition->setDisableValidations (getDisableValidations ());

    pOrmTable->addRelation (pOrmComposition);
}

template <class T> string AttributeManagedObjectVectorComposition<T>::getSqlForCreate ()
{
    string sqlForCreate2;
    UI32   parentTableId    = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId = OrmRepository::getTableId (m_composedWith);
    string schema           = OrmRepository::getWaveCurrentSchema ();

    m_compositionTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    sqlForCreate2 += string ("CREATE TABLE ") + m_compositionTableName + "\n";
    sqlForCreate2 += "(\n";
    sqlForCreate2 += "    ownerClassId integer REFERENCES " + schema + "." + m_parent + " (objectIdClassId) DEFERRABLE INITIALLY DEFERRED,\n";
    sqlForCreate2 += "    ownerInstanceId bigint REFERENCES " + schema + "." + m_parent + " (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED,\n";
    sqlForCreate2 += "    relatedToClassId integer REFERENCES " + schema + "." + m_composedWith + " (objectIdClassId) DEFERRABLE INITIALLY DEFERRED,\n";
    sqlForCreate2 += "    relatedToInstanceId bigint REFERENCES " + schema + "." + m_composedWith + " (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED\n";
    sqlForCreate2 += ");";

    return (sqlForCreate2);
}

template <class T> void AttributeManagedObjectVectorComposition<T>::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    UI32   numberOfObjectIdsToInsert = m_pData->size ();
    UI32   i                         = 0;
    UI32   parentTableId             = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId          = OrmRepository::getTableId (m_composedWith);

    m_compositionTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    string   insertPrefix              = string ("INSERT INTO ") + OrmRepository::getWaveCurrentSchema () + "." + m_compositionTableName + string (" VALUES (") + m_parentObjectId.getClassId () + string (", ") + m_parentObjectId.getInstanceId () + string (", ");
    string   insertPostfix             = ");\n";
    ObjectId tempObjectId;

    for (i = 0; i < numberOfObjectIdsToInsert; i++)
    {
        tempObjectId = ((*m_pData)[i])->getObjectId ();

        sqlForInsert2 += insertPrefix + tempObjectId.getClassId () + string (", ") + tempObjectId.getInstanceId () + insertPostfix;
    }
}

template <class T> void AttributeManagedObjectVectorComposition<T>::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string sqlForPreValues;
    string sqlForInsert;
    UI32   parentTableId    = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId = OrmRepository::getTableId (m_composedWith);

    m_compositionTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    sqlForUpdate2 += "DELETE FROM " + OrmRepository::getWaveCurrentSchema () + "." + m_compositionTableName + string (" WHERE ownerClassId = ") + m_parentObjectId.getClassId () + string (" AND ownerInstanceId = ") + m_parentObjectId.getInstanceId () + string (";");
    getSqlForInsert (sqlForPreValues, sqlForInsert, sqlForUpdate2, isFirst);
}

template <class T> void AttributeManagedObjectVectorComposition<T>::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    trace (TRACE_LEVEL_FATAL, "AttributeManagedObjectVectorComposition<T>::getSqlForSelect : AttributeObjectIdVectorAssociation must not be added via addAndAttribute/addOrAttribute member functions.  It msut be added via addAttributeManagedObjectVectorComposition.");
    prismAssert (false, __FILE__, __LINE__);
}

template <class T> void AttributeManagedObjectVectorComposition<T>::getSqlForSelect2 (string &associationTableName, string &whereClause)
{
    UI32     parentTableId             = OrmRepository::getTableId (m_parent);
    UI32     relatedToTableId          = OrmRepository::getTableId (m_composedWith);
    ObjectId tempObjectId;
    UI32     i                         = 0;
    UI32     numberOfObjectIdsToSelect = m_pData->size ();

    m_compositionTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    associationTableName = m_compositionTableName;

    for (i = 0; i < numberOfObjectIdsToSelect; i++)
    {
        tempObjectId = ((*m_pData)[i])->getObjectId ();

        whereClause += string ("( relatedToClassId = ") + tempObjectId.getClassId () + string (" AND relatedToInstanceId = ") + tempObjectId.getInstanceId () + string (")");

        if (i != (numberOfObjectIdsToSelect - 1))
        {
            whereClause += " OR ";
        }
    }
}

template <class T> void AttributeManagedObjectVectorComposition<T>::toString (string &valueString)
{
    UI32 numberOfObjectIds = m_pData->size ();
    UI32 i                 = 0;
        char tempBuffer[64] = {0};

    valueString = "";

    for (i = 0; i < numberOfObjectIds; i++)
    {
        snprintf (tempBuffer, 64, "#%u*%llu", (((*m_pData)[i])->getObjectId ()).getClassId (), ((((*m_pData)[i])->getObjectId ()).getInstanceId ()));

        valueString += tempBuffer;
    }
}

template <class T> void AttributeManagedObjectVectorComposition<T>::fromString (const string &valueString)
{
    vector<ObjectId> tempObjectVector;

    ULI startFromIndex = 0;
    ULI firstIndex     = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        UI32 classId    = 0;
        UI64 instanceId = 0;

        sscanf (valueString.c_str () + firstIndex + 1, "%u*%llu", &classId, &instanceId);
        tempObjectVector.push_back (ObjectId (classId, instanceId));

        startFromIndex = firstIndex + 1;
    }

    // FIXME : Now load the Managed Objects from DB and add them in m_pData.
}

template <class T> void *AttributeManagedObjectVectorComposition<T>::getPData ()
{
    return (m_pData);
}

template <class T> void AttributeManagedObjectVectorComposition<T>::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector <WaveManagedObjectPointer<T> > *> (pData));
}

template <class T> void AttributeManagedObjectVectorComposition<T>::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
}

template <class T> void AttributeManagedObjectVectorComposition<T>::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    UI32   parentTableId    = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId = OrmRepository::getTableId (m_composedWith);

    // This is a special case.  Set the parent ObjectId explicitly.  In normal cases, this would have been set during the call to prepareForPersistence.  However,
    // in case of queried objects the object id for parent at that time will be null object id.

    m_parentObjectId = parentObjectId;

    m_compositionTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    PGresult *pResult = auxilliaryResultsMap[m_compositionTableName];

    prismAssert (NULL != pResult, __FILE__, __LINE__);

    UI32                         numberOfResults     = PQntuples (pResult);
    UI32                         i                   = 0;
    UI32                         columnForClassId    = PQfnumber (pResult, "relatedToClassId");
    UI32                         columnForInstanceId = PQfnumber (pResult, "relatedToInstanceId");
    UI32                         classId             = 0;
    UI64                         instanceId          = 0;
    char                        *pValueString        = NULL;
    WaveManagedObject           *pWaveManagedObject  = NULL;
    T                           *pT                  = NULL;
    ObjectId                     tempObjectId;
    vector<ObjectId>             tempObjectIds;
    vector<WaveManagedObject *>  waveManagedObjects;
    bool                         isClassIdSame       = true;
    UI32                         initializeClassid   = 0;

    for (i = 0; i < numberOfResults; i++)
    {
        pValueString = PQgetvalue (pResult, i, columnForClassId);
        classId      = strtoul (pValueString, NULL, 10);

        if (0 == i)
        {
            initializeClassid = classId;
        }

        if (classId != initializeClassid)
        {
            isClassIdSame = false;
            break;
        }
    }

    if (true == isClassIdSame)
    {
        if (0 < numberOfResults)
        {
            for (i = 0; i < numberOfResults; i++)
            {
                pValueString = PQgetvalue (pResult, i, columnForClassId);
                classId      = strtoul (pValueString, NULL, 10);
                pValueString = PQgetvalue (pResult, i, columnForInstanceId);
                instanceId   = strtoull (pValueString, NULL, 10);

                tempObjectId = ObjectId (classId, instanceId);

                tempObjectIds.push_back (tempObjectId);
            }

            prismAssert (0 < (tempObjectIds.size ()), __FILE__, __LINE__);

            string childTableName = OrmRepository::getTableNameById ((tempObjectIds[0]).getClassId ());

            //waveManagedObjects = DatabaseObjectManager::loadWaveManagedObjectsFromDatabaseWrapper (tempObjectIds, schema, pWaveObjectManager);
            waveManagedObjects = DatabaseObjectManager::loadWaveManagedObjectsFromDatabaseWrapper (childTableName, parentObjectId, schema, pWaveObjectManager);

            for (i = 0; i < numberOfResults; i++)
            {
                pWaveManagedObject = waveManagedObjects[i];

                if (NULL != pWaveManagedObject)
                {
                    pT = dynamic_cast<T *> (pWaveManagedObject);

                    prismAssert (NULL != pT, __FILE__, __LINE__);

                    WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (pT);

                    (*m_pData).push_back (tempWaveManagedObjectPointer);
                }
            }
        }
    }
    else
    {
        for (i = 0; i < numberOfResults; i++)
        {
            pValueString = PQgetvalue (pResult, i, columnForClassId);
            classId      = strtoul (pValueString, NULL, 10);
            pValueString = PQgetvalue (pResult, i, columnForInstanceId);
            instanceId   = strtoull (pValueString, NULL, 10);

            tempObjectId = ObjectId (classId, instanceId);

            pWaveManagedObject = DatabaseObjectManager::loadWaveManagedObjectFromDatabaseWrapper (tempObjectId, schema, pWaveObjectManager);

            if (NULL != pWaveManagedObject)
            {
                pT = dynamic_cast<T *> (pWaveManagedObject);

                prismAssert (NULL != pT, __FILE__, __LINE__);

                WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (pT);

                (*m_pData).push_back (tempWaveManagedObjectPointer);
            }
        }
    }
}

template <class T> Attribute *AttributeManagedObjectVectorComposition<T>::clone ()
{
    AttributeManagedObjectVectorComposition *pAttributeManagedObjectVectorComposition = new AttributeManagedObjectVectorComposition (*this);
    
    return (pAttributeManagedObjectVectorComposition);
}

template <class T> void AttributeManagedObjectVectorComposition<T>::updateOrmRelations (const PrismPersistableObject *pPrismPersistableObject)
{
    UI32 numberOfObjectsInComposition = m_pData->size ();
    UI32 i                            = 0;

    for (i = 0; i < numberOfObjectsInComposition; i++)
    {
        ((*m_pData)[i])->setOwnerManagedObjectId (pPrismPersistableObject->getObjectId ());
    }
}

template<class T> WaveManagedObject *AttributeManagedObjectVectorComposition<T>::getComposedManagedObject (const ObjectId &childObjectId)
{
    trace (TRACE_LEVEL_DEVEL, "AttributeManagedObjectVectorComposition::getComposedManagedObject: Function invoked");

    UI32 compositionVectorSize = (*m_pData).size();

    for (UI32 i=0; i <= compositionVectorSize; i++)
        {
            if (childObjectId == ((*m_pData)[i])->getObjectId ())
            {
            // This means we have found the managed object
                return ( ((*m_pData)[i]).operator->());
            }
        }
        return NULL;
}

template<class T> vector<WaveManagedObject *>  *AttributeManagedObjectVectorComposition<T>::getComposedManagedObject ( )
{
    if ( NULL == m_pData )
    {
        trace (TRACE_LEVEL_FATAL, string("AttributeManagedObjectVectorComposition::getComposedManagedObject: NULL pointer"));    
        prismAssert (false, __FILE__, __LINE__);        
    }

    vector<WaveManagedObject *> *pResults = new vector<WaveManagedObject *> ;
    
    UI32 compositionVectorSize = (*m_pData).size();

    for (UI32 i=0; i < compositionVectorSize; i++)
    {   
       (*pResults).push_back( ((*m_pData)[i]).operator->() );
    }
        
    return ( pResults );
}

template <class T> bool AttributeManagedObjectVectorComposition<T>::isDeletableForOperation (const WaveManagedObjectOperation &operation)
{
    UI32 compositionVectorSize = (*m_pData).size();

    for (UI32 i=0; i < compositionVectorSize; i++)
    {
        if (false == (((*m_pData)[i])->isHierarchyDeletableForOperation (operation)))
        {
            return (false);
            }
        }
    return (true);
}

template <class T> void AttributeManagedObjectVectorComposition<T>::updateKeyString (const PrismPersistableObject *pPrismPersistableObject)
{
    UI32 numberOfObjectsInComposition = m_pData->size ();
    UI32 i                            = 0;

    for (i = 0; i < numberOfObjectsInComposition; i++)
    {
        ((*m_pData)[i])->prependOwnerKeyString (pPrismPersistableObject->getKeyString ());
    }
}

template <class T> void AttributeManagedObjectVectorComposition<T>::getRestRowData (string &restRowData)
{
    UI32 numberOfObjectsInComposition = m_pData->size ();
    UI32 i                            = 0;

    for (i = 0; i < numberOfObjectsInComposition; i++)
    {
        string tempRestRowData;

        ((*m_pData)[i])->getRestRowData (tempRestRowData);
        restRowData += tempRestRowData;
    }
}

template<class T> bool AttributeManagedObjectVectorComposition<T>::isCurrentValueSameAsDefault () const
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeManagedObjectVectorComposition::isCurrentValueSameAsDefault : This method is not supported"));
    prismAssert (false, __FILE__, __LINE__);
    return (false);
}

template <class T> bool AttributeManagedObjectVectorComposition<T>::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

template <class T> vector <WaveManagedObjectPointer<T> > AttributeManagedObjectVectorComposition<T>::getDefaultData() const
{
    return (m_defaultData);
}

template <class T> void AttributeManagedObjectVectorComposition<T>::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        (*m_pData).clear();
    }
}

template <class T> void AttributeManagedObjectVectorComposition<T>::getCValue (WaveCValue *pCValue)
{
    prismAssert (false, __FILE__, __LINE__); 
}

template <class T> void AttributeManagedObjectVectorComposition<T>::storeRelatedObjectVector (const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects)
{
    m_parentObjectId = parentObjectId;

    UI32 numberOfObjects = vectorOfRelatedObjects.size ();

    for (UI32 i = 0; i < numberOfObjects; i++)
    {
        T *pT = dynamic_cast<T *> (vectorOfRelatedObjects[i]);

        prismAssert (NULL != pT, __FILE__, __LINE__);

        WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (pT);

        (*m_pData).push_back (tempWaveManagedObjectPointer);
    }
}

}
