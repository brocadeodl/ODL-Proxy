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

#ifndef ATTRIBUTEMANAGEDOBJECTONETOONERELATIONBASE_H
#define ATTRIBUTEMANAGEDOBJECTONETOONERELATIONBASE_H


#include "Framework/Attributes/Attribute.h"
#if 0
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "WaveResourceIdEnums.h"
#endif

namespace WaveNs
{

class AttributeManagedObjectOneToOneRelationBase : public Attribute 
{
    private :
    protected :
                                                                       AttributeManagedObjectOneToOneRelationBase           (AttributeType &attributeType, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational, const bool &canBeEmpty = false);
                                                                       AttributeManagedObjectOneToOneRelationBase           (const AttributeManagedObjectOneToOneRelationBase &attribute);
                AttributeManagedObjectOneToOneRelationBase            &operator =                                           (const AttributeManagedObjectOneToOneRelationBase &attribute);

    public :
        virtual void                                                   setCanBeEmpty                                        (const bool canBeEmpty);
        virtual bool                                                   getCanBeEmpty                                        () const;

        virtual void                                                   setRelatedObjectId                                   (const ObjectId &relatedObjectId);
        virtual ObjectId                                               getRelatedObjectId                                   () const;
        virtual void                                                   loadRelatedObjectIdFromPostgresQueryResult           (PGresult *pResult, const UI32 &row);
                void                                                   popOneToOneCompositionsFromResult                    (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap);
        virtual void                                                   storeComposedObjectPointer                           (WaveManagedObject *pWaveChildManagedObject);

    // Now the data members

    private :
        bool          m_canBeEmpty;
        ObjectId      m_relatedObjectId;
};

}

#endif // ATTRIBUTEMANAGEDOBJECTVECTORCOMPOSITIONTEMPLATEBASE_H

