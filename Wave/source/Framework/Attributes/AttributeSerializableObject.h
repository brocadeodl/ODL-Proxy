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
 *   Copyright (C) 2008-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef ATTRIBUTESERIALIZABLEOBJECT_H
#define ATTRIBUTESERIALIZABLEOBJECT_H 

#include "Framework/Attributes/Attribute.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

template<class T> class AttributeSerializableObject : public Attribute
{
    private:
        virtual bool                                   validate                          () const;
    protected:
    public:
                                                       AttributeSerializableObject (const T &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                       AttributeSerializableObject (T *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                       AttributeSerializableObject (const AttributeSerializableObject<T> &attribute);
        virtual                                       ~AttributeSerializableObject ();
                AttributeSerializableObject<T>        &operator =                  (const AttributeSerializableObject<T> &attribute);
                T                                      getValue                    () const;
                void                                   setValue                    (const T &data);
        virtual string                                 getSqlType                  ();
        virtual void                                   setupOrm                    (OrmTable *pOrmTable);
        virtual string                                 getSqlForCreate             ();
        virtual void                                   getSqlForInsert             (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                                   getSqlForUpdate             (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                                   getSqlForSelect             (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                                   toString                    (string &valueString);
        virtual void                                   fromString                  (const string &valueString);
        virtual void                                  *getPData                    ();
        virtual void                                   setValue                    (const void *pData);
        virtual Attribute                             *clone                       ();
        virtual void                                   setDefaultValue             ();
        virtual void                                   getCValue                   (WaveCValue *pCValue);
                bool                                   isCurrentValueSameAsDefault () const; 
    // data members
    private:
                T   *m_pData;
    protected:
    public:
};

}
#endif //ATTRIBUTESERIALIZABLEOBJECT_H
