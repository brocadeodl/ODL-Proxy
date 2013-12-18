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

#include "Framework/ObjectRelationalMapping/RelationshipEntry.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Types/DateTime.h"

namespace WaveNs
{

RelationshipEntry::RelationshipEntry (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
    : m_parentClassName (parentClassName),
      m_childClassName  (childClassName),
      m_relationshipName (relationshipName),
      m_parentObjectId  (parentObjectId),
      m_childObjectId   (childObjectId)
{
    OrmRepository::getOrmRelationTypeAndOrmRelationUmlType (parentClassName, relationshipName, m_ormRelationType, m_ormRelationUmlType);
}

RelationshipEntry::~RelationshipEntry ()
{
}

void RelationshipEntry::getSqlForInsertIntoParent (string &sql)
{
    if (ORM_RELATION_TYPE_ONE_TO_MANY == getRelationType ())
    {
        string relationshipTableName = getRelationshipTableName ();

        sql += string ("INSERT INTO ") + OrmRepository::getWaveCurrentSchema () + "." + relationshipTableName + string (" VALUES (") + m_parentObjectId.getClassId () + string (", ") + m_parentObjectId.getInstanceId () + string (", ") + m_childObjectId.getClassId () + string (", ") + m_childObjectId.getInstanceId () + string (");\n");
        sql += string ("UPDATE ")      + OrmRepository::getWaveCurrentSchema () + "." + m_parentClassName    + string (" SET lastModifiedTimeStamp = '") + (DateTime ()).toString () + string ("' WHERE objectIdClassId = ") + m_parentObjectId.getClassId () + string (" AND objectIdInstanceId = ") + m_parentObjectId.getInstanceId () + string (";\n");
    }
    else
    {
        sql += string ("UPDATE ") + OrmRepository::getWaveCurrentSchema () + "." + m_parentClassName + string (" SET ") + m_relationshipName + "ClassId =" + m_childObjectId.getClassId () + string (", ") + m_relationshipName + string ("InstanceId =") + m_childObjectId.getInstanceId () + string (", lastModifiedTimeStamp = '") + (DateTime ()).toString () + string ("' WHERE ObjectIdClassId=") + m_parentObjectId.getClassId () + string (" AND ObjectIdInstanceId = ") + m_parentObjectId.getInstanceId () + string (";\n");
    }
    
    return;
}

void RelationshipEntry::getSqlForDeleteFromParent (string &sql)
{
    if (ORM_RELATION_TYPE_ONE_TO_MANY == getRelationType ())
    {
        string relationshipTableName = getRelationshipTableName ();

        sql += string ("DELETE FROM ") + OrmRepository::getWaveCurrentSchema () + "." + relationshipTableName + string (" WHERE ownerClassId = ") + m_parentObjectId.getClassId () + string (" AND ownerInstanceId = ") + m_parentObjectId.getInstanceId () + string (" AND relatedToClassId = ") + m_childObjectId.getClassId () + string (" AND relatedToInstanceId = ") + m_childObjectId.getInstanceId () + string (";\n");
        sql += string ("UPDATE ")      + OrmRepository::getWaveCurrentSchema () + "." + m_parentClassName    + string (" SET lastModifiedTimeStamp = '") + (DateTime ()).toString () + string ("' WHERE objectIdClassId = ") + m_parentObjectId.getClassId () + string (" AND objectIdInstanceId = ") + m_parentObjectId.getInstanceId () + string (";\n");
    }
    else
    {
        sql += string ("UPDATE ") + OrmRepository::getWaveCurrentSchema () + "." + m_parentClassName + string (" SET ") + m_relationshipName + "ClassId =NULL" + string (", ") + m_relationshipName + string ("InstanceId =NULL") + string (", lastModifiedTimeStamp = '") + (DateTime ()).toString () + string ("' WHERE ObjectIdClassId=") + m_parentObjectId.getClassId () + string (" AND ObjectIdInstanceId = ") + m_parentObjectId.getInstanceId () + string (";\n");
    }
    

}

const string & RelationshipEntry::getParentClassName () const
{
    return (m_parentClassName);
}

const string & RelationshipEntry::getChildClassName () const
{
    return (m_childClassName);
}

const string & RelationshipEntry::getRelationshipName () const
{
    return (m_relationshipName);
}

OrmRelationType RelationshipEntry::getRelationType () const
{
    return (m_ormRelationType);
}

OrmRelationUmlType RelationshipEntry::getRelationUmlType () const
{
    return (m_ormRelationUmlType);
}

string RelationshipEntry::getRelationshipTableName () const
{
    UI32   parentTableId        = OrmRepository::getTableId (m_parentClassName);
    UI32   relatedToTableId     = OrmRepository::getTableId (m_childClassName);
    string relationshipTableName = string ("t") + parentTableId + m_relationshipName + string ("t") + relatedToTableId;

    return (relationshipTableName);
}

}
