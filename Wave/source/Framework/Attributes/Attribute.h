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

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Framework/Attributes/AttributeTypes.h"
#include "Framework/Types/Types.h"
#include "Framework/Types/WaveSetCValue.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "WaveResourceIdEnums.h"

#include <string>
#include <vector>
#include <map>

//#include <xercesc/dom/DOM.hpp>
//using namespace XERCES_CPP_NAMESPACE;

#include "libpq-fe.h"

using namespace std;

namespace WaveNs
{

class OrmTable;
class WaveManagedObject;
class PrismPersistableObject;
class WaveObjectManager;
class WaveManagedObjectOperation;

class Attribute
{
    private :
    protected :
                                   Attribute                             (AttributeType &attributeType);
                                   Attribute                             (AttributeType &attributeType, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational);
                                   Attribute                             (const Attribute &attribute);
                Attribute         &operator =                            (const Attribute &attribute);
                void               setIsMemoryOwnedByAttribute           (bool isMemoryOwnedByAttribute);
                bool               getIsMemoryOwnedByAttribute           ();
                void               setAttributeType                      (const AttributeType &attributeType);
        virtual bool               validate                              () const = 0;
        virtual bool               isConditionOperatorSupported          (AttributeConditionOperator attributeConditionOperator); 

    public :
        virtual                   ~Attribute                             ();
                UI32               getAttributeTag                       ();
                void               setAttributeTag                       (const UI32 &attributeTag);
                AttributeType      getAttributeType                      () const;
                string             getAttributeName                      () const;
                void               setAttributeName                      (const string &attributeName);
                UI32               getAttributeUserTag                   () const;
                void               setAttributeUserTag                   (const UI32 &attributeUserTag);
                bool               getIsOperational                      () const;
        virtual string             getSqlType                            ()                                                                                           = 0;
                string             getAttributeTypeString                () const;
        virtual void               setupOrm                              (OrmTable *pOrmTable)                                                                        = 0;
        virtual string             getSqlForCreate                       ()                                                                                           = 0;
        virtual void               getSqlForInsert                       (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false) = 0;
        virtual void               getSqlForUpdate                       (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false)                          = 0;
        virtual void               getSqlForSelect                       (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) = 0;
        virtual void               toString                              (string &valueString)                                                                        = 0;
//                void               serializeTo                         (DOMDocument *pDomDocument);
        virtual void               fromString                            (const string &valueString)                                                                  = 0;
        virtual void               getPlainString                        (string &valueString);
        virtual ResourceId         loadFromPlainString                   (const string &valueString);
        virtual void              *getPData                              ()                                                                                           = 0;
        virtual void               setValue                              (const void *pData)                                                                          = 0;

                bool               getIsPrimary                          ();
                void               setIsPrimary                          (const bool &isPrimary);

        virtual void               loadFromPostgresQueryResult           (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        virtual void               loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);

        virtual Attribute         *clone                                 ()                                                                                           = 0;

        virtual void               updateOrmRelations                    (const PrismPersistableObject *pPrismPersistableObject);
        virtual WaveManagedObject *getComposedManagedObject              (const ObjectId &childObjectId);
        virtual vector<WaveManagedObject *> *getComposedManagedObject    ( );
        virtual bool               isDeletableForOperation               (const WaveManagedObjectOperation &operation);
        virtual void               updateKeyString                       (const PrismPersistableObject *pPrismPersistableObject);

        virtual void               getRestRowData                        (string &restRowData);
        virtual bool               isCurrentValueSameAsDefault           () const = 0;
        virtual void               setDefaultValue                       () = 0;
        virtual void               getCValue                             (WaveCValue *pCValue) = 0;
        virtual void               toEscapedString                       (string &valueString);
        static  Attribute         *getAttributeFromAttributeType         (const ResourceId attributeType);

        virtual void               toJsonString                          (string &jsonString);
    // Now the data members

    private :
        AttributeType               m_attributeType;
        string                      m_attributeName;
        UI32                        m_attributeTag;
        bool                        m_isMemoryOwnedByAttribute;

        bool                        m_isPrimary;

        UI32                        m_attributeUserTag;

        bool                        m_isOperational;

    protected :
    public :
};

}

#endif // ATTRIBUTE_H
