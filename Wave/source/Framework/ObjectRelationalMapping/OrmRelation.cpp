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

#include "Framework/ObjectRelationalMapping/OrmRelation.h"

namespace WaveNs
{

OrmRelation::OrmRelation (const string &name, const string &relatedTo, const OrmRelationType &relationType)
    : OrmEntity (name),
      m_relatedTo    (relatedTo),
      m_relationType (relationType)
{
}

OrmRelation::~OrmRelation ()
{
}

string OrmRelation::getRelatedTo () const
{
    return (m_relatedTo);
}

OrmRelationType OrmRelation::getRelationType () const
{
    return (m_relationType);
}

string OrmRelation::getTypeForUpgradeMO () const
{
	string upgradeType;

	switch (getRelationUmlType()) {
	    case ORM_RELATION_UML_TYPE_ASSOCIATION:
		    upgradeType = "a-";
		    break;
	    case ORM_RELATION_UML_TYPE_AGGREGATION:
		    upgradeType = "g-";
		    break;
	    case ORM_RELATION_UML_TYPE_COMPOSITION:
		    upgradeType = "c-";
	}
	
	if (getRelationType () == ORM_RELATION_TYPE_ONE_TO_MANY)
		upgradeType += "m-";
	else
		upgradeType += "1-";

	upgradeType += m_relatedTo;

	return (upgradeType);
}

string OrmRelation::getAuxilliaryTableName (const string &parentName) const
{
    return ("");
}

string OrmRelation::getAlterSqlForOneToOneRelationship(const string &schema,const string & tableName) const
{
    return ("");
}

string OrmRelation::getAlterSqlToAddNotNullForOneToOneRelationship (const string &schema,const string & tableName) const
{
    return ("");
}

string OrmRelation::getAlterSqlToDropNotNullForOneToOneRelationship (const string &schema,const string & tableName) const
{
    return ("");
}

}
