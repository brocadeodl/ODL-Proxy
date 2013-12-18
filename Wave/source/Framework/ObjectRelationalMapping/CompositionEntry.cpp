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

#include "Framework/ObjectRelationalMapping/CompositionEntry.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Types/DateTime.h"

namespace WaveNs
{

CompositionEntry::CompositionEntry (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
    : m_parentClassName (parentClassName),
      m_childClassName  (childCalssName),
      m_compositionName (compositionName),
      m_parentObjectId  (parentObjectId),
      m_childObjectId   (childObjectId)
{
}

CompositionEntry::~CompositionEntry ()
{
}

void CompositionEntry::getSqlForInsertIntoParent (string &sql)
{
    string compositionTableName = getCompositionTableName ();

    sql += string ("INSERT INTO ") + OrmRepository::getWaveCurrentSchema () + "." + compositionTableName + string (" VALUES (") + m_parentObjectId.getClassId () + string (", ") + m_parentObjectId.getInstanceId () + string (", ") + m_childObjectId.getClassId () + string (", ") + m_childObjectId.getInstanceId () + string (");\n");
    sql += string ("UPDATE ")      + OrmRepository::getWaveCurrentSchema () + "." + m_parentClassName    + string (" SET lastModifiedTimeStamp = '") + (DateTime ()).toString () + string ("' WHERE objectIdClassId = ") + m_parentObjectId.getClassId () + string (" AND objectIdInstanceId = ") + m_parentObjectId.getInstanceId () + string (";\n");

    return;
}

void CompositionEntry::getSqlForDeleteFromParent (string& sql)
{
    string compositionTableName = getCompositionTableName ();

    sql += string ("DELETE FROM ") + OrmRepository::getWaveCurrentSchema () + "." + compositionTableName + string (" WHERE ownerClassId = ") + m_parentObjectId.getClassId () + string (" AND ownerInstanceId = ") + m_parentObjectId.getInstanceId () + string (" AND relatedToClassId = ") + m_childObjectId.getClassId () + string (" AND relatedToInstanceId = ") + m_childObjectId.getInstanceId () + string (";\n");
    sql += string ("UPDATE ")      + OrmRepository::getWaveCurrentSchema () + "." + m_parentClassName    + string (" SET lastModifiedTimeStamp = '") + (DateTime ()).toString () + string ("' WHERE objectIdClassId = ") + m_parentObjectId.getClassId () + string (" AND objectIdInstanceId = ") + m_parentObjectId.getInstanceId () + string (";\n");
}

const string & CompositionEntry::getParentClassName () const
{
    return (m_parentClassName);
}

const string & CompositionEntry::getChildClassName () const
{
    return (m_childClassName);
}

const string & CompositionEntry::getCompositionName () const
{
    return (m_compositionName);
}

string CompositionEntry::getCompositionTableName () const
{
    UI32   parentTableId        = OrmRepository::getTableId (m_parentClassName);
    UI32   relatedToTableId     = OrmRepository::getTableId (m_childClassName);
    string compositionTableName = string ("t") + parentTableId + m_compositionName + string ("t") + relatedToTableId;

    return (compositionTableName);
}

AssociationEntry::AssociationEntry (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
    : CompositionEntry (parentClassName, childCalssName, associationName, parentObjectId, childObjectId)
{
}

string AssociationEntry::getAssociationTableName () const
{
    return (getCompositionTableName ());
}

}
