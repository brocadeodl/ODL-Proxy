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

#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
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

namespace WaveNs
{

template<class T> AttributeManagedObjectComposition<T>::AttributeManagedObjectComposition (const WaveManagedObjectPointer<T> &data, const string &attributeName, const string &composedWith, const bool &canBeEmpty, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectOneToOneRelationBase (AttributeType::AttributeTypeComposition, attributeName, attributeUserTag, isOperational, canBeEmpty),
      m_composedWith (composedWith)
{
    m_pData              = new WaveManagedObjectPointer<T>;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

template<class T> AttributeManagedObjectComposition<T>::AttributeManagedObjectComposition (const WaveManagedObjectPointer<T> &data, const WaveManagedObjectPointer<T> &defaultData, const string &attributeName, const string &composedWith, const bool &canBeEmpty, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectOneToOneRelationBase (AttributeType::AttributeTypeComposition, attributeName, attributeUserTag, isOperational, canBeEmpty),
      m_composedWith (composedWith)
{
    m_pData              = new WaveManagedObjectPointer<T>;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

template<class T> AttributeManagedObjectComposition<T>::AttributeManagedObjectComposition (WaveManagedObjectPointer<T> *pData, const string &attributeName, const string &composedWith, const bool &canBeEmpty, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectOneToOneRelationBase (AttributeType::AttributeTypeComposition, attributeName, attributeUserTag, isOperational, canBeEmpty),
      m_composedWith (composedWith)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

template<class T> AttributeManagedObjectComposition<T>::AttributeManagedObjectComposition (WaveManagedObjectPointer<T> *pData, const WaveManagedObjectPointer<T> &defaultData, const string &attributeName, const string &composedWith, const bool &canBeEmpty, const UI32 &attributeUserTag, const bool &isOperational)
    : AttributeManagedObjectOneToOneRelationBase (AttributeType::AttributeTypeComposition, attributeName, attributeUserTag, isOperational, canBeEmpty),
      m_composedWith (composedWith)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (false);
}

template<class T> AttributeManagedObjectComposition<T>::AttributeManagedObjectComposition (const AttributeManagedObjectComposition<T> &attribute)
    : AttributeManagedObjectOneToOneRelationBase (attribute)
{
    m_pData = new WaveManagedObjectPointer<T>;
    *m_pData = attribute.getValue ();

    setIsMemoryOwnedByAttribute (true);

    m_composedWith       = attribute.m_composedWith;

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

}

template<class T> AttributeManagedObjectComposition<T>::~AttributeManagedObjectComposition ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

template<class T> AttributeManagedObjectComposition<T> &AttributeManagedObjectComposition<T>::operator = (const AttributeManagedObjectComposition<T> &attribute)
{
    AttributeManagedObjectOneToOneRelationBase::operator = (attribute);

    setValue (attribute.getValue ());

    m_composedWith       = attribute.m_composedWith;
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

template<class T> WaveManagedObjectPointer<T> AttributeManagedObjectComposition<T>::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeManagedObjectComposition::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (WaveManagedObjectPointer<T> ());
    }
}

template<class T> void AttributeManagedObjectComposition<T>::setValue (const WaveManagedObjectPointer<T> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeManagedObjectComposition::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

template<class T> bool AttributeManagedObjectComposition<T>::validate () const
{
    prismAssert (AttributeType::AttributeTypeComposition == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeComposition == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

template<class T> bool AttributeManagedObjectComposition<T>::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

template<class T> string AttributeManagedObjectComposition<T>::getSqlType ()
{
    return ("integer");
}

template<class T> void AttributeManagedObjectComposition<T>::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addRelation (new OrmComposition (getAttributeName (), m_composedWith, ORM_RELATION_TYPE_ONE_TO_ONE, getCanBeEmpty ()));
}

template<class T> string AttributeManagedObjectComposition<T>::getSqlForCreate ()
{
    string attributeName = getAttributeName ();

    if (true == getCanBeEmpty ())
    {
        return (attributeName + "ClassId integer, " + attributeName + "InstanceId bigint");
    }
    else
    {
        return (attributeName + "ClassId integer NOT NULL, " + attributeName + "InstanceId bigint NOT NULL");
    }
}

template<class T> void AttributeManagedObjectComposition<T>::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    T *pT = (*m_pData).operator -> ();

    if (NULL != pT)
    {
        if (true == getCanBeEmpty ())
        {
            if (ObjectId::NullObjectId != (*m_pData)->getObjectId ())
            {
                sqlForInsert += string (",") + ((*m_pData)->getObjectId ()).getClassId () + string (",") + ((*m_pData)->getObjectId ()).getInstanceId ();

                sqlForPreValues += string (",") + getAttributeName () + "ClassId," + getAttributeName () + "InstanceId";
            }
        }
        else
        {
            sqlForInsert += string (",") + ((*m_pData)->getObjectId ()).getClassId () + string (",") + ((*m_pData)->getObjectId ()).getInstanceId ();

            sqlForPreValues += string (",") + getAttributeName () + "ClassId," + getAttributeName () + "InstanceId";
        }
    }
}

template<class T> void AttributeManagedObjectComposition<T>::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    T *pT = (*m_pData).operator -> ();

    if (NULL != pT)
    {
        if (true == getCanBeEmpty ())
        {
            if (ObjectId::NullObjectId != (*m_pData)->getObjectId ())
            {
                sqlForUpdate += "," + getAttributeName () + string ("ClassId = ") + (((*m_pData)->getObjectId ()).getClassId ()) + ", " + (getAttributeName ()) + string ("InstanceId = ") + (((*m_pData)->getObjectId ()).getInstanceId ());
            }
            else
            {
                sqlForUpdate += "," + getAttributeName () + string ("ClassId = NULL, ") + (getAttributeName ()) + string ("InstanceId = NULL");
            }
        }
        else
        {
            sqlForUpdate += "," + getAttributeName () + string ("ClassId = ") + (((*m_pData)->getObjectId ()).getClassId ()) + ", " + (getAttributeName ()) + string ("InstanceId = ") + (((*m_pData)->getObjectId ()).getInstanceId ());
        }
    }
    else
    {
        sqlForUpdate += "," + getAttributeName () + string ("ClassId = NULL, ") + (getAttributeName ()) + string ("InstanceId = NULL");
    }
}

template<class T> void AttributeManagedObjectComposition<T>::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeManagedObjectComposition<T>::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    T *pT = (*m_pData).operator -> ();

    if (NULL != pT)
    {
        sqlForSelect += getAttributeName () + string ("ClassId = ") + (((*m_pData)->getObjectId ()).getClassId ()) + " AND " + (getAttributeName ()) + string ("InstanceId = ") + (((*m_pData)->getObjectId ()).getInstanceId ());
    }
    else
    {
        sqlForSelect += getAttributeName () + string ("ClassId = NULL AND ") + (getAttributeName ()) + string ("InstanceId = NULL");
    }
}

template<class T> void AttributeManagedObjectComposition<T>::toString (string &valueString)
{
    T *pT = (*m_pData).operator -> ();

    if (NULL != pT)
    {
        char buffer[64] = {0};

        snprintf (buffer, 64, "%u*%llu", ((*m_pData)->getObjectId ()).getClassId (), ((*m_pData)->getObjectId ()).getInstanceId ());

        valueString = string (buffer);
    }
    else
    {
        valueString = "0*0";
    }
}

template<class T> void AttributeManagedObjectComposition<T>::fromString (const string &valueString)
{
    WaveManagedObjectPointer<T> tempWaveManagedObjectPointer;

    if ("0*0" != valueString)
    {
        ObjectId tempObjectId;
        ULI      tempIndex;

        tempObjectId.setClassId (strtoul (valueString.c_str (), NULL, 10));

        tempIndex = valueString.find ("*", 0);

        prismAssert (string::npos != tempIndex, __FILE__, __LINE__);

        if (string::npos != tempIndex)
        {
            tempObjectId.setInstanceId (strtoull (valueString.c_str () + tempIndex + 1, NULL, 10));
        }

        // FIXME: Fill the temp before setting.
    }

    setValue (tempWaveManagedObjectPointer);
}

template<class T> void *AttributeManagedObjectComposition<T>::getPData ()
{
    return (m_pData);
}

template<class T> void AttributeManagedObjectComposition<T>::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const WaveManagedObjectPointer<T> *> (pData));
}

template<class T> Attribute *AttributeManagedObjectComposition<T>::clone ()
{
    AttributeManagedObjectComposition *pAttributeManagedObjectComposition = new AttributeManagedObjectComposition (*this);

    return (pAttributeManagedObjectComposition);
}

template<class T> void AttributeManagedObjectComposition<T>::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    string attributeName    = getAttributeName ();
    SI32   classIdColumn    = PQfnumber (pResult, (attributeName + "ClassId").c_str ());
    SI32   instanceIdColumn = PQfnumber (pResult, (attributeName + "InstanceId").c_str ());

    if ((-1 != classIdColumn) && (-1 != instanceIdColumn))
    {
        char   *pclassIdValue    = PQgetvalue (pResult, row, classIdColumn);
        char   *pInstanceIdValue = PQgetvalue (pResult, row, instanceIdColumn);
        UI32    classId          = strtoul  (pclassIdValue, NULL, 10);
        UI64    instanceId       = strtoull (pInstanceIdValue, NULL, 10);

        ObjectId tempObjectId (classId, instanceId);

        if (ObjectId::NullObjectId != tempObjectId)
        {
            WaveManagedObject *pWaveManagedObject = DatabaseObjectManager::loadWaveManagedObjectFromDatabaseWrapper (tempObjectId, schema, pWaveObjectManager);

            if (NULL != pWaveManagedObject)
            {
                T *pT = dynamic_cast<T *> (pWaveManagedObject);

                prismAssert (NULL != pT, __FILE__, __LINE__);

                WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (pT);

                *m_pData = tempWaveManagedObjectPointer;
            }
            else
            {
                WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (NULL);

                *m_pData = tempWaveManagedObjectPointer;
            }
        }
        else
        {
            WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (NULL);

            *m_pData = tempWaveManagedObjectPointer;
        }
    }
    else
    {
        WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (NULL);

        *m_pData = tempWaveManagedObjectPointer;
    }
}

template<class T> void AttributeManagedObjectComposition<T>::updateOrmRelations (const PrismPersistableObject *pPrismPersistableObject)
{
    T *pT = (*m_pData).operator -> ();

    if (NULL != pT)
    {
        (*m_pData)->setOwnerManagedObjectId (pPrismPersistableObject->getObjectId ());
    }
}

template<class T> WaveManagedObject *AttributeManagedObjectComposition<T>::getComposedManagedObject (const ObjectId &childObjectId)
{

    trace (TRACE_LEVEL_DEVEL, "AttributeManagedObjectComposition<T>::getComposedManagedObject: Function invoked");

        if (childObjectId == (*m_pData)->getObjectId ())
        {
        // This means we have found the managed object
            return ( (*m_pData).operator->());
        }
        return NULL;
}

template<class T> vector<WaveManagedObject *>  *AttributeManagedObjectComposition<T>::getComposedManagedObject ( )
{
    trace (TRACE_LEVEL_DEVEL, "AttributeManagedObjectComposition<T>::getComposedManagedObject: Function invoked without ObjectId");

    vector<WaveManagedObject *> *pResults = new vector<WaveManagedObject *>;

    (*pResults).push_back ( (*m_pData).operator->() );

    return ( pResults );
}

template<class T> bool AttributeManagedObjectComposition<T>::isDeletableForOperation (const WaveManagedObjectOperation &operation)
{
    return ((*m_pData)->isHierarchyDeletableForOperation (operation));
}

template<class T> void AttributeManagedObjectComposition<T>::updateKeyString (const PrismPersistableObject *pPrismPersistableObject)
{
    T *pT = (*m_pData).operator -> ();

    if (NULL != pT)
    {
        (*m_pData)->prependOwnerKeyString (pPrismPersistableObject->getKeyString ());
    }
}

template<class T> bool AttributeManagedObjectComposition<T>::isCurrentValueSameAsDefault () const
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeManagedObjectComposition::isCurrentValueSameAsDefault : This method is not supported"));
    prismAssert (false, __FILE__, __LINE__);
    return (false);
}

template<class T> bool AttributeManagedObjectComposition<T>::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

template<class T> WaveManagedObjectPointer<T> AttributeManagedObjectComposition<T>::getDefaultData() const
{
    return (m_defaultData);
}

template<class T> void AttributeManagedObjectComposition<T>::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        WaveManagedObjectPointer<T> tempData;
        *m_pData = tempData;
    }
}

template<class T> void AttributeManagedObjectComposition<T>::getCValue (WaveCValue *pCValue)
{
    prismAssert (false, __FILE__, __LINE__);
}

template<class T> void AttributeManagedObjectComposition<T>::storeComposedObjectPointer (WaveManagedObject *pWaveChildManagedObject)
{
    T *pT = NULL;

    if (NULL != pWaveChildManagedObject)
    {
        pT = dynamic_cast<T *> (pWaveChildManagedObject);

        prismAssert (NULL != pT, __FILE__, __LINE__);
    }

    WaveManagedObjectPointer<T> tempWaveManagedObjectPointer (pT);

    *m_pData = tempWaveManagedObjectPointer;
}

}
