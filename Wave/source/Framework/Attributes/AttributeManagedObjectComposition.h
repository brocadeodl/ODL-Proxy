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

#ifndef ATTRIBUTEMANAGEDOBJECTCOMPOSITION_H
#define ATTRIBUTEMANAGEDOBJECTCOMPOSITION_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "WaveResourceIdEnums.h"
#include "Framework/Attributes/AttributeManagedObjectOneToOneRelationBase.h"

namespace WaveNs
{

class WaveManagedObject;
class PrismPersistableObject;
class WaveObjectManager;
class WaveManagedObjectOperation;

template<class T> class AttributeManagedObjectComposition : public AttributeManagedObjectOneToOneRelationBase
{
    private :
        virtual bool                                  validate                          () const;
        virtual bool                                  isConditionOperatorSupported      (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                                      AttributeManagedObjectComposition (const WaveManagedObjectPointer<T> &data, const string &attributeName , const string &composedWith, const bool &canBeEmpty = false, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                      AttributeManagedObjectComposition (const WaveManagedObjectPointer<T> &data, const WaveManagedObjectPointer<T> &defaultData, const string &attributeName , const string &composedWith, const bool &canBeEmpty = false, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                      AttributeManagedObjectComposition (WaveManagedObjectPointer<T> *pData, const string &attributeName, const string &composedWith, const bool &canBeEmpty = false, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                      AttributeManagedObjectComposition (WaveManagedObjectPointer<T> *pData, const WaveManagedObjectPointer<T> &defaultData, const string &attributeName, const string &composedWith, const bool &canBeEmpty = false, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                      AttributeManagedObjectComposition (const AttributeManagedObjectComposition<T> &attribute);
        virtual                                      ~AttributeManagedObjectComposition ();
                AttributeManagedObjectComposition<T> &operator =                        (const AttributeManagedObjectComposition<T> &attribute);
                WaveManagedObjectPointer<T>           getValue                          () const;
                void                                  setValue                          (const WaveManagedObjectPointer<T> &data);
                bool                                  isCurrentValueSameAsDefault       () const;
                WaveManagedObjectPointer<T>           getDefaultData                    () const;
                bool                                  getIsDefaultDataValidFlag         () const;
        virtual string                                getSqlType                        ();
        virtual void                                  setupOrm                          (OrmTable *pOrmTable);
        virtual string                                getSqlForCreate                   ();
        virtual void                                  getSqlForInsert                   (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                                  getSqlForUpdate                   (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                                  getSqlForSelect                   (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                                  toString                          (string &valueString);
        virtual void                                  fromString                        (const string &valueString);
        virtual void                                 *getPData                          ();
        virtual void                                  setValue                          (const void *pData);
        virtual Attribute                            *clone                             ();

        virtual void                                  loadFromPostgresQueryResult       (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);

        virtual void                                  updateOrmRelations                (const PrismPersistableObject *pPrismPersistableObject);
        virtual WaveManagedObject                    *getComposedManagedObject          (const ObjectId &childObjectId);
        virtual vector<WaveManagedObject *>          *getComposedManagedObject          ();
        virtual bool                                  isDeletableForOperation           (const WaveManagedObjectOperation &operation);
        virtual void                                  updateKeyString                   (const PrismPersistableObject *pPrismPersistableObject);
        virtual void                                  setDefaultValue                   ();
        virtual void                                  getCValue                         (WaveCValue *pCValue);

        virtual void                                  storeComposedObjectPointer        (WaveManagedObject *pWaveChildManagedObject);

    // Now the data members

    private :
    protected :
        WaveManagedObjectPointer<T> *m_pData;
        string                       m_composedWith;
        WaveManagedObjectPointer<T>  m_defaultData;
        bool                         m_isDefaultDataValid;

    public :
};

}

#endif // ATTRIBUTEMANAGEDOBJECTCOMPOSITION_H
