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
 *@file AddedRemovedManagedObjectSchemaDifference.h 
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 *                    All rights reserved. 
 * Description: This file declares the information regarding the tables added to
 *              the new schema and tables removed from the old schema
 *              
 *               
 * Author :     Aashish Akhouri 
 * Date :       10/20/2011 
 */

#ifndef ADDEDREMOVEDMANAGEDOBJECTSCHEMADIFFRENCE_H
#define ADDEDREMOVEDMANAGEDOBJECTSCHEMADIFFRENCE_H

#include <string>
#include "Framework/Types/Types.h"
#include "Framework/ObjectRelationalMapping/RelationshipInfoFromSchemaDifference.h"

namespace WaveNs
{

class AddedRemovedManagedObjectSchemaDifference
{
    private:
    protected:
    public:

                                                        AddedRemovedManagedObjectSchemaDifference   (const string & tableName);
                                                       ~AddedRemovedManagedObjectSchemaDifference   ();

    void                                                addRelations                                (const RelationshipInfoFromSchemaDifference &  relationshipInfo);
    const vector<RelationshipInfoFromSchemaDifference>& getRelations                                () const;

    const string &                                      getTableName () const;
    void                                                printContentsForDebugging                   ();

    private:
        string                                          mTableName;
        vector<RelationshipInfoFromSchemaDifference>    mRelations;
    protected:
    public:

};

}

#endif//ADDEDREMOVEDMANAGEDOBJECTSCHEMADIFFRENCE_H
