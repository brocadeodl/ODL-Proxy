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
 *   Author : Mayur Mahajan                                                *
 ***************************************************************************/

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Persistence/MoSchemaInfoManagedObject.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"

namespace WaveNs
{

/** 
 * Name:  MoSchemaInfoManagedObject
 * Description: Constructor 
 * @param pWaveObjectManager
 */
MoSchemaInfoManagedObject::MoSchemaInfoManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject    (MoSchemaInfoManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject         (pWaveObjectManager),
      m_classId                  (0),
      m_parentClassId            (0)
{
}

/**
 * Name: ~MoSchemaInfoManagedObject
 * Description: Destructor
 */
MoSchemaInfoManagedObject::~MoSchemaInfoManagedObject ()
{
}

/** 
 * Accessor Functions 
 */
const string &  MoSchemaInfoManagedObject::getManagedObjectName () const
{
    return m_managedObjectName;
}


const vector<string> & MoSchemaInfoManagedObject::getManagedObjectFieldNames () const
{
    return m_fieldNames;
}



const vector<string> & MoSchemaInfoManagedObject::getManagedObjectFieldTypes () const
{
    return m_fieldTypes;
}


UI32 MoSchemaInfoManagedObject::getManagedObjectClassId () const
{
    return m_classId;
}


UI32 MoSchemaInfoManagedObject::getManagedObjectParentClassId () const
{
    return m_parentClassId;
}

string MoSchemaInfoManagedObject::getClassName ()
{
    return ("MoSchemaInfoManagedObject");
}

const vector<string> & MoSchemaInfoManagedObject::getManagedObjectExpandedFieldTypes () const
{
    return m_expandedFieldTypes;
}

const vector<string> & MoSchemaInfoManagedObject::getManagedObjectUserDefinedFieldTypes () const
{
    return m_userDefinedFieldTypes;
}

const vector<string> & MoSchemaInfoManagedObject::getManagedObjectUserDefinedFieldNames () const
{
    return m_userDefinedFieldNames;
}

bool MoSchemaInfoManagedObject::getIsALocalManagedObject () const
{
    return (m_isALocalManagedObject);
}

const string& MoSchemaInfoManagedObject::getDerivedFromClassName () const
{
    return (m_derivedFromClassName);
} 

/** 
 * Name: setupAttributesForPersistence
 * Description:
 * @return:     none 
 */
void MoSchemaInfoManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString        (&m_managedObjectName, "className"));
    addPersistableAttribute (new AttributeStringVector  (&m_fieldNames, "fieldNames"));
    addPersistableAttribute (new AttributeStringVector  (&m_fieldTypes, "fieldTypes"));
    addPersistableAttribute (new AttributeUI32          (&m_classId, "classId"));
    addPersistableAttribute (new AttributeUI32          (&m_parentClassId, "parentClassId"));
    addPersistableAttribute (new AttributeStringVector  (&m_expandedFieldTypes, "expandedFieldTypes"));
    addPersistableAttribute (new AttributeStringVector  (&m_userDefinedFieldNames, "userDefinedFieldNames"));
    addPersistableAttribute (new AttributeStringVector  (&m_userDefinedFieldTypes, "userDefinedFieldTypes"));
    addPersistableAttribute (new AttributeBool          (&m_isALocalManagedObject, "isALocalManagedObject"));
    addPersistableAttribute (new AttributeString        (&m_derivedFromClassName, "derivedFromClassName"));
}

/** 
 * Name: setupAttributesForCreate 
 * Description: 
 * @return:     none 
 */
void MoSchemaInfoManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString       (&m_managedObjectName, "className"));
    addPersistableAttributeForCreate (new AttributeStringVector (&m_fieldNames, "fieldNames"));
    addPersistableAttributeForCreate (new AttributeStringVector (&m_fieldTypes, "fieldTypes"));
    addPersistableAttributeForCreate (new AttributeUI32         (&m_classId, "classId"));
    addPersistableAttributeForCreate (new AttributeUI32         (&m_parentClassId, "parentClassId"));
    addPersistableAttributeForCreate (new AttributeStringVector  (&m_expandedFieldTypes, "expandedFieldTypes"));
    addPersistableAttributeForCreate (new AttributeStringVector  (&m_userDefinedFieldNames, "userDefinedFieldNames"));
    addPersistableAttributeForCreate (new AttributeStringVector  (&m_userDefinedFieldTypes, "userDefinedFieldTypes"));
    addPersistableAttributeForCreate (new AttributeBool          (&m_isALocalManagedObject, "isALocalManagedObject"));
    addPersistableAttributeForCreate (new AttributeString        (&m_derivedFromClassName, "derivedFromClassName"));
}

}
