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

#include "Framework/Attributes/AttributeManagedObjectOneToOneRelationBase.h"
#include <vector>

using namespace std;

namespace WaveNs
{
AttributeManagedObjectOneToOneRelationBase::AttributeManagedObjectOneToOneRelationBase (AttributeType &attributeType, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational, const bool &canBeEmpty)
    : Attribute (attributeType, attributeName, attributeUserTag, isOperational),
      m_canBeEmpty  (canBeEmpty)
{
}

AttributeManagedObjectOneToOneRelationBase::AttributeManagedObjectOneToOneRelationBase (const AttributeManagedObjectOneToOneRelationBase &attributeManagedObjectOneToOneRelationBase)
    : Attribute (attributeManagedObjectOneToOneRelationBase)
{
    m_canBeEmpty = attributeManagedObjectOneToOneRelationBase.m_canBeEmpty;
}

AttributeManagedObjectOneToOneRelationBase &AttributeManagedObjectOneToOneRelationBase::operator = (const AttributeManagedObjectOneToOneRelationBase &attributeManagedObjectOneToOneRelationBase)
{
    Attribute::operator = (attributeManagedObjectOneToOneRelationBase);

    m_canBeEmpty = attributeManagedObjectOneToOneRelationBase.m_canBeEmpty;

    return (*this);
}

bool AttributeManagedObjectOneToOneRelationBase::getCanBeEmpty () const
{
    return m_canBeEmpty;
}

void AttributeManagedObjectOneToOneRelationBase::setCanBeEmpty (const bool canBeEmpty)
{
    m_canBeEmpty = canBeEmpty;
}

void AttributeManagedObjectOneToOneRelationBase::setRelatedObjectId (const ObjectId &relatedObjectId)
{
    m_relatedObjectId = relatedObjectId;
}

ObjectId AttributeManagedObjectOneToOneRelationBase::getRelatedObjectId () const
{
    return m_relatedObjectId;
}

void AttributeManagedObjectOneToOneRelationBase::loadRelatedObjectIdFromPostgresQueryResult (PGresult *pResult, const UI32 &row)
{
    string  attributeName    = getAttributeName ();
    SI32    classIdColumn    = PQfnumber (pResult, ((getAttributeName ()) + "ClassId").c_str ());
    SI32    instanceIdColumn = PQfnumber (pResult, ((getAttributeName ()) + "InstanceId").c_str ());

    if ((-1 != classIdColumn) && (-1 != instanceIdColumn))
    {
        const   char   *pclassIdValue    = PQgetvalue (pResult, row, classIdColumn);
                UI32    classId          = strtoul  (pclassIdValue, NULL, 10);
        const   char   *pInstanceIdValue = PQgetvalue (pResult, row, instanceIdColumn);
                UI64    instanceId       = strtoull (pInstanceIdValue, NULL, 10);

        m_relatedObjectId = ObjectId (classId, instanceId);
    }
}

void AttributeManagedObjectOneToOneRelationBase::popOneToOneCompositionsFromResult (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap)
{
    ObjectId relatedObjectId = getRelatedObjectId ();

    if (ObjectId::NullObjectId != relatedObjectId)
    {
        map<ObjectId, WaveManagedObject*>::iterator element2            = oidTopManagedObjectMap.find (relatedObjectId);
        map<ObjectId, WaveManagedObject*>::iterator limitingElement2    = oidTopManagedObjectMap.end ();

        if (limitingElement2 != element2)
        {
            WaveManagedObject *pChildWaveManagedObject = element2->second;

            storeComposedObjectPointer (pChildWaveManagedObject);

            oidTopManagedObjectMap.erase (element2);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        storeComposedObjectPointer (NULL);
    }
}

void AttributeManagedObjectOneToOneRelationBase::storeComposedObjectPointer (WaveManagedObject *pWaveChildManagedObject)
{
    WaveNs::prismAssert (false, __FILE__, __LINE__);
}

}
