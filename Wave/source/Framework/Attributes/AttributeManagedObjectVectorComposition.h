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

#ifndef ATTRIBUTEMANAGEDOBJECTVECTORCOMPOSITION_H
#define ATTRIBUTEMANAGEDOBJECTVECTORCOMPOSITION_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeManagedObjectVectorCompositionTemplateBase.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class WaveManagedObject;
class PrismPersistableObject;
class WaveObjectManager;
class WaveManagedObjectOperation;

template<class T> class AttributeManagedObjectVectorComposition : public AttributeManagedObjectVectorCompositionTemplateBase
{
    private :
        virtual bool                                     validate                                () const;

    protected :
    public :
                                                         AttributeManagedObjectVectorComposition (const vector <WaveManagedObjectPointer<T> > &data, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                         AttributeManagedObjectVectorComposition (const vector <WaveManagedObjectPointer<T> > &data, const vector <WaveManagedObjectPointer<T> > &defaultData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                         AttributeManagedObjectVectorComposition (vector <WaveManagedObjectPointer<T> > *pData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                         AttributeManagedObjectVectorComposition (vector <WaveManagedObjectPointer<T> > *pData, const vector <WaveManagedObjectPointer<T> > &defaultData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &composedWith, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                         AttributeManagedObjectVectorComposition (const AttributeManagedObjectVectorComposition &attribute);
        virtual                                         ~AttributeManagedObjectVectorComposition ();
                AttributeManagedObjectVectorComposition &operator =                              (const AttributeManagedObjectVectorComposition &attribute);
                vector <WaveManagedObjectPointer<T> >    getValue                                () const;
                void                                     setValue                                (const vector <WaveManagedObjectPointer<T> > &data);
                bool                                     isCurrentValueSameAsDefault             () const;
                bool                                     getIsDefaultDataValidFlag               () const;
                vector <WaveManagedObjectPointer<T> >    getDefaultData                          () const;
        virtual string                                   getSqlType                              ();
        virtual void                                     setupOrm                                (OrmTable *pOrmTable);
        virtual string                                   getSqlForCreate                         ();
        virtual void                                     getSqlForInsert                         (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                                     getSqlForUpdate                         (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                                     getSqlForSelect                         (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
                void                                     getSqlForSelect2                        (string &associationTableName, string &whereClause);
        virtual void                                     toString                                (string &valueString);
        virtual void                                     fromString                              (const string &valueString);
        virtual void                                    *getPData                                ();
        virtual void                                     setValue                                (const void *pData);
        virtual Attribute                               *clone                                   ();
        virtual void                                     loadFromPostgresQueryResult             (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        virtual void                                     loadFromPostgresAuxilliaryQueryResult   (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);

        virtual void                                     updateOrmRelations                      (const PrismPersistableObject *pPrismPersistableObject);
        virtual WaveManagedObject                       *getComposedManagedObject                (const ObjectId &childObjectId);
        virtual vector<WaveManagedObject *>             *getComposedManagedObject                ( );
        virtual bool                                     isDeletableForOperation                 (const WaveManagedObjectOperation &operation);
        virtual void                                     updateKeyString                         (const PrismPersistableObject *pPrismPersistableObject);
        virtual void                                     getRestRowData                          (string &restRowData);
        virtual void                                     setDefaultValue                         ();
        virtual void                                     getCValue                               (WaveCValue *pCValue);

        virtual void                                     storeRelatedObjectVector                (const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects);

    // Now the data members

    private :
    protected :
        vector <WaveManagedObjectPointer<T> > *m_pData;
        string                                 m_parent;
        ObjectId                               m_parentObjectId;
        string                                 m_composedWith;
        string                                 m_compositionTableName;
        vector <WaveManagedObjectPointer<T> >  m_defaultData;
        bool                                   m_isDefaultDataValid;
    public :
};

}

#endif // ATTRIBUTEMANAGEDOBJECTVECTORCOMPOSITION_H
