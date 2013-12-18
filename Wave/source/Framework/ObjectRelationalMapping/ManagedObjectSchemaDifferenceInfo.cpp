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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaDifferenceInfo.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

/* Definition of class ManagedObjectSchemaDifferenceInfo */ 
ManagedObjectSchemaDifferenceInfo::ManagedObjectSchemaDifferenceInfo (const string& managedObjectName, bool isNewManagedObject)
    :   m_managedObjectName                     (managedObjectName),
        m_isNewManagedObject                    (isNewManagedObject),
        m_isManagedObjectInstancePresentInDb    (false)
{
}


ManagedObjectSchemaDifferenceInfo::~ManagedObjectSchemaDifferenceInfo ()
{
    map<string, Field *>::iterator addedFieldsIterator;

    for (addedFieldsIterator=m_addedFields.begin(); addedFieldsIterator!= m_addedFields.end(); addedFieldsIterator++)
    {
        delete (addedFieldsIterator->second); 
    }

    m_addedFields.clear ();
}

const string& ManagedObjectSchemaDifferenceInfo::getManagedObjectName () const
{
    return (m_managedObjectName);
}

bool ManagedObjectSchemaDifferenceInfo::getIsManagedObjectInstancePresentInDb () const
{
    return (m_isManagedObjectInstancePresentInDb);
}

bool ManagedObjectSchemaDifferenceInfo::getIsNewManagedObject               () const
{
    return (m_isNewManagedObject);
}

void ManagedObjectSchemaDifferenceInfo::setIsManagedObjectInstancePresentInDb (bool isManagedObjectInstancePresentInDb) 
{
    m_isManagedObjectInstancePresentInDb = isManagedObjectInstancePresentInDb;
}

void ManagedObjectSchemaDifferenceInfo::addAddedField (Field *pField)
{
    m_addedFields[pField->getFieldName()]= pField;    
}
        
UI32 ManagedObjectSchemaDifferenceInfo::getNumberOfAddedFields () const
{
    return (m_addedFields.size());
}

void ManagedObjectSchemaDifferenceInfo::getSqlForAddedFields (string &sqlForUpdate) 
{
    map<string, Field *>::iterator fieldIterator; 
    string sqlForUpdate2;
    bool isFirst = false;

    for (fieldIterator=m_addedFields.begin(); fieldIterator!=m_addedFields.end();fieldIterator++)
    {
        fieldIterator->second->getSqlForField (sqlForUpdate, sqlForUpdate2, isFirst);

        /*if (isFirst == true)
        {
            isFirst = false;
        }*/
    }
}

/* Defintion of class Field */

Field::Field (const string& fieldName, const string& fieldType, const string& parentManagedObjectName)
    :   m_fieldName                 (fieldName),
        m_fieldType                 (fieldType),
        m_parentManagedObjectName   (parentManagedObjectName),
        m_pAttribute                (NULL)
{
}

Field::~Field ()
{
    if (NULL != m_pAttribute)
    {
        delete (m_pAttribute);
    }
}

const string & Field::getFieldName () const
{
    return (m_fieldName);
}

const string & Field::getFieldType () const
{
    return (m_fieldType);
}

const string & Field::getParentManagedObjectName () const
{
    return (m_parentManagedObjectName);
}

Attribute* Field::getAttribute ()
{
    return (m_pAttribute);
}
void Field::setAttribute (Attribute *pAttribute)
{
    m_pAttribute = pAttribute;
}

void Field::getSqlForField (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    if (m_pAttribute != NULL)
    {
        m_pAttribute->setDefaultValue ();
        m_pAttribute->getSqlForUpdate (sqlForUpdate, sqlForUpdate2, isFirst); 
    }
}

}
