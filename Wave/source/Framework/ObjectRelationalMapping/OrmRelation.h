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

#ifndef ORMRELATION_H
#define ORMRELATION_H

#include "Framework/ObjectRelationalMapping/OrmEntity.h"

#include <string>

using namespace std;

namespace WaveNs
{

typedef enum
{
    ORM_RELATION_TYPE_ONE_TO_ONE,
    ORM_RELATION_TYPE_ONE_TO_MANY
} OrmRelationType;

typedef enum
{
    ORM_RELATION_UML_TYPE_ASSOCIATION,
    ORM_RELATION_UML_TYPE_AGGREGATION,
    ORM_RELATION_UML_TYPE_COMPOSITION
} OrmRelationUmlType;


class OrmRelation : public OrmEntity
{
    private :
    protected :
    public :
                                    OrmRelation                    (const string &name, const string &relatedTo, const OrmRelationType &relationType);
        virtual                    ~OrmRelation                    ();

                string              getRelatedTo           () const;
                OrmRelationType     getRelationType        () const;
        virtual string              getSqlForCreate        (const string &schema) const = 0;
        virtual string              getSqlForCreate2       (const string &parentName, const string &schema) const = 0;

	
        virtual string              getAuxilliaryTableName (const string &parentName) const;
	virtual	string		    getTypeForUpgradeMO	   () const;
        virtual OrmRelationUmlType  getRelationUmlType     () const = 0;
        virtual string              getAlterSqlForOneToOneRelationship              (const string &schema,const string & tableName) const;
        virtual string              getAlterSqlToAddNotNullForOneToOneRelationship  (const string &schema,const string & tableName) const;
        virtual string              getAlterSqlToDropNotNullForOneToOneRelationship (const string &schema,const string & tableName) const;
    // Now the data members

    private :
        string          m_relatedTo;
        OrmRelationType m_relationType;

    protected :
    public :
};

}

#endif // ORMRELATION_H
