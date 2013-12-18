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

/**
 *@file RelationshipInfoFromSchemaDifference.h 
 * Copyright (C) 2011 Brocade Communications Systems,Inc.
 * All rights reserved. 
 * Description: This file declares the class that stores the info regarding a relation
 *              It will be used in the context of storing the added/removed/changed relationships
 *              for each table.
 *              
 *               
 * Author :     Aashish Akhouri 
 * Date :       10/20/2011 
 */

#ifndef RELATIONSHIPINFOFROMSCHEMADIFFRENCE_H
#define RELATIONSHIPINFOFROMSCHEMADIFFRENCE_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include <vector>

namespace WaveNs
{

class RelationshipInfoFromSchemaDifference
{
    public:

                                                        RelationshipInfoFromSchemaDifference    (const string & relationName, const string & relatedToTable, OrmRelationUmlType relationUmlType, OrmRelationType relationType, bool disableValidations = false, bool canBeEmpty = false);
                                                        RelationshipInfoFromSchemaDifference    (const RelationshipInfoFromSchemaDifference & relationshipInfo);
                                                        RelationshipInfoFromSchemaDifference    (string relationName, string relationFieldType);
        const   RelationshipInfoFromSchemaDifference &  operator=                               (const RelationshipInfoFromSchemaDifference & relationshipInfo);
                                                       ~RelationshipInfoFromSchemaDifference    ();

        const   string &                                getRelationName                         () const;
                OrmRelationUmlType                      getRelationUmlType                      () const;
                OrmRelationType                         getRelationType                         () const;
        const   string &                                getRelatedToTable                       () const;
                bool                                    getDisableValidations                   () const;
                bool                                    getCanBeEmpty                           () const;
                UI8                                     getChangedRelationshipAttributes        () const;
                bool                                    isChangedRelationship                   () const;
                void                                    setCanBeEmptyChanged                    ();
                bool                                    isCanBeEmptyChanged                     () const;
                void                                    setDisableValidationChanged             ();
                bool                                    isDisableValidationChanged              () const;

        void                                            printContentsForDebugging               ();

    private:
                string                                  m_relationName;
                string                                  m_relatedToTable;
                OrmRelationUmlType 	                    m_relationUmlType; 
                OrmRelationType                         m_relationType;
                bool                                    m_disableValidations;
                bool                                    m_canBeEmpty;
                UI8                                     m_changedRelationshipAttributes;
};

}

#endif//RELATIONSHIPINFOFROMSCHEMADIFFRENCE_H
