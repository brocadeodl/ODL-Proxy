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

#include "Framework/ObjectRelationalMapping/OrmAssociation.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

namespace WaveNs
{

OrmAssociation::OrmAssociation (const string &name, const string &relatedTo, const OrmRelationType &relationType, const bool &canBeEmpty)
    : OrmRelation  (name, relatedTo, relationType),
      m_canBeEmpty (canBeEmpty)
{
}

OrmAssociation::~OrmAssociation ()
{
}

string OrmAssociation::getSqlForCreate (const string &schema) const
{
    string sqlForCreate;

    switch (getRelationType ())
    {
        case ORM_RELATION_TYPE_ONE_TO_ONE :
            if (true == m_canBeEmpty)
            {
                sqlForCreate += "    " + getName () + "ClassId integer REFERENCES "   +  schema + "." + getRelatedTo () + "Derivations (objectIdClassId) DEFERRABLE INITIALLY DEFERRED,\n";
                sqlForCreate += "    " + getName () + "InstanceId bigint REFERENCES " +  schema + "." + getRelatedTo () + "DerivationsInstances (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED";
            }
            else
            {
                sqlForCreate += "    " + getName () + "ClassId integer REFERENCES "   +  schema + "." + getRelatedTo () + "Derivations (objectIdClassId) DEFERRABLE INITIALLY DEFERRED NOT NULL,\n";
                sqlForCreate += "    " + getName () + "InstanceId bigint REFERENCES " +  schema + "." + getRelatedTo () + "DerivationsInstances (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED NOT NULL";
            }

            break;

        case ORM_RELATION_TYPE_ONE_TO_MANY :
            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (sqlForCreate);
}

string OrmAssociation::getAlterSqlToDropNotNullForOneToOneRelationship (const string &schema,const string & tableName) const
{

    string sqlForAlter;

    switch (getRelationType ())
    {
        case ORM_RELATION_TYPE_ONE_TO_ONE :
            if (true == m_canBeEmpty)
            {
		        sqlForAlter += "ALTER TABLE " + schema + "." + tableName +" ALTER "+  getName () +"ClassId DROP NOT NULL;\n";
		        sqlForAlter += "ALTER TABLE " + schema + "." + tableName +" ALTER "+  getName () +"InstanceId DROP NOT NULL;\n";
            }

            break;

        case ORM_RELATION_TYPE_ONE_TO_MANY :
            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (sqlForAlter);

}

string OrmAssociation::getAlterSqlToAddNotNullForOneToOneRelationship (const string &schema,const string & tableName) const
{

    string sqlForAlter;

    switch (getRelationType ())
    {
        case ORM_RELATION_TYPE_ONE_TO_ONE :
            if (true != m_canBeEmpty)
            {
		        sqlForAlter += "ALTER TABLE " + schema + "." + tableName +" ALTER "+  getName () +"ClassId SET NOT NULL;\n";
		        sqlForAlter += "ALTER TABLE " + schema + "." + tableName +" ALTER "+  getName () +"InstanceId SET NOT NULL;\n";
            }

            break;

        case ORM_RELATION_TYPE_ONE_TO_MANY :
            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (sqlForAlter);

}

string OrmAssociation::getAlterSqlForOneToOneRelationship (const string &schema,const string & tableName) const
{

    string sqlForAlter;

    switch (getRelationType ())
    {
        case ORM_RELATION_TYPE_ONE_TO_ONE :
            /*
             * NOT NULL for the foreign keys is not applied here.
             * Once the upgrade functions are called for all OMs, NOT NULL should be added by calling getAlterSqlToAddNotNullForOneToOneRelationship
             * Here, we no need to distinguish if it can be empty or not(m_canBeEmpty)
             */
            sqlForAlter += "ALTER TABLE " +schema+"."+tableName +" ADD COLUMN " + getName () + "ClassId integer;\n";
            sqlForAlter += "ALTER TABLE " +schema+"."+tableName +" ADD FOREIGN KEY (" +  getName () +"ClassId) REFERENCES " + schema+ "."+getRelatedTo () + "Derivations (objectIdClassId) DEFERRABLE INITIALLY DEFERRED;\n";

            sqlForAlter += "ALTER TABLE " +schema+"."+tableName +" ADD COLUMN " + getName () + "InstanceId bigint;\n";
            sqlForAlter += "ALTER TABLE " +schema+"."+tableName +" ADD FOREIGN KEY (" +  getName () +"InstanceId) REFERENCES " +schema+ "."+ getRelatedTo () + "DerivationsInstances (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED;\n";
#if 0
            sqlForAlter += "ALTER TABLE " +schema+"."+tableName +" ADD COLUMN " + getName () + "ClassId integer REFERENCES "   +  schema + "." + getRelatedTo () + "Derivations (objectIdClassId) DEFERRABLE INITIALLY DEFERRED NOT NULL,\n";
            sqlForAlter +=  "ALTER TABLE " +schema+"."+tableName +" ADD COLUMN " + getName () + "InstanceId bigint REFERENCES " +  schema + "." + getRelatedTo () + "DerivationsInstances (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED NOT NULL";
#endif
            break;

        case ORM_RELATION_TYPE_ONE_TO_MANY :
            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (sqlForAlter);

}
string OrmAssociation::getSqlForCreate2 (const string &parentName, const string &schema) const
{
    string sqlForCreate2;
    string relatedTo        = getRelatedTo ();
    UI32   parentTableId    = OrmRepository::getTableId (parentName);
    UI32   relatedToTableId = OrmRepository::getTableId (relatedTo);

    switch (getRelationType ())
    {
        case ORM_RELATION_TYPE_ONE_TO_ONE :
            break;

        case ORM_RELATION_TYPE_ONE_TO_MANY :
            sqlForCreate2 += string ("CREATE TABLE ") + schema + string (".t") + parentTableId + getName () + string ("t") + relatedToTableId + "\n";
            sqlForCreate2 += "(\n";
            sqlForCreate2 += "    ownerClassId integer CONSTRAINT " + getOwnerClassFkeyConstraintName (parentName) + " REFERENCES " + schema + "." + parentName + "Derivations (objectIdClassId) DEFERRABLE INITIALLY DEFERRED,\n";
            sqlForCreate2 += "    ownerInstanceId bigint CONSTRAINT " + getOwnerInstanceFkeyConstraintName (parentName) + " REFERENCES " + schema + "." + parentName + "DerivationsInstances (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED,\n";
            sqlForCreate2 += "    relatedToClassId integer CONSTRAINT " + getRelatedClassFkeyConstraintName (parentName) + " REFERENCES " + schema + "." + relatedTo + "Derivations (objectIdClassId) DEFERRABLE INITIALLY DEFERRED,\n";
            sqlForCreate2 += "    relatedToInstanceId bigint CONSTRAINT " + getRelatedInstanceFkeyConstraintName (parentName) + " REFERENCES " + schema + "." + relatedTo + "DerivationsInstances (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED\n";
            sqlForCreate2 += ");";

            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (sqlForCreate2);
}

string OrmAssociation::getOwnerClassFkeyConstraintName (const string &parentName) const
{
    return getAuxilliaryTableName (parentName) + "_ownerClassId_fkey ";
}

string OrmAssociation::getOwnerInstanceFkeyConstraintName (const string &parentName) const
{
    return getAuxilliaryTableName (parentName) + "_ownerInstanceId_fkey ";
}

string OrmAssociation::getRelatedClassFkeyConstraintName (const string &parentName) const
{
    return getAuxilliaryTableName (parentName) + "_relatedToClassId_fkey ";
}

string OrmAssociation::getRelatedInstanceFkeyConstraintName (const string &parentName) const
{
    return getAuxilliaryTableName (parentName) + "_relatedToInstanceId_fkey ";
}

string OrmAssociation::getAuxilliaryTableName (const string &parentName) const
{
    string auxilliaryTableName;
    string relatedTo            = getRelatedTo ();
    UI32   parentTableId        = OrmRepository::getTableId (parentName);
    UI32   relatedToTableId     = OrmRepository::getTableId (relatedTo);

    switch (getRelationType ())
    {
        case ORM_RELATION_TYPE_ONE_TO_ONE :
            break;

        case ORM_RELATION_TYPE_ONE_TO_MANY :
            auxilliaryTableName = string ("t") + parentTableId + getName () + string ("t") + relatedToTableId;
            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (auxilliaryTableName);
}

OrmRelationUmlType OrmAssociation::getRelationUmlType () const
{
    return (ORM_RELATION_UML_TYPE_ASSOCIATION);
}


}
