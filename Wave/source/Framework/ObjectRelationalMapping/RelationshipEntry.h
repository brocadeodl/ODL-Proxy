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

#ifndef RELATIONSHIPENTRY_H
#define RELATIONSHIPENTRY_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"

#include <string>

using namespace std;

namespace WaveNs
{

class RelationshipEntry
{
    private :
    protected :
    public :
                                    RelationshipEntry           (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual                    ~RelationshipEntry           ();

                void                getSqlForInsertIntoParent   (string &sql);
                void                getSqlForDeleteFromParent   (string &sql);

        const   string             &getParentClassName          () const;
        const   string             &getChildClassName           () const;
        const   string             &getRelationshipName         () const;
                string              getRelationshipTableName    () const;
                OrmRelationType     getRelationType             () const;
                OrmRelationUmlType  getRelationUmlType          () const;
        // Now the data members

    private :
        string              m_parentClassName;
        string              m_childClassName;
        string              m_relationshipName;
        ObjectId            m_parentObjectId;
        ObjectId            m_childObjectId;
        OrmRelationType     m_ormRelationType;
        OrmRelationUmlType  m_ormRelationUmlType; 

    protected :
    public :
};

}

#endif // RELATIONSHIPENTRY_H
