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
 *   Author : Raymond Lai                                                                  *
 ***************************************************************************/

#ifndef ATTRIBUTELARGEOBJECT_H
#define ATTRIBUTELARGEOBJECT_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Types/LargeObject.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeLargeObject : public Attribute
{
    private :
        virtual bool                    validate                        () const;

    protected :
    public :
                                        AttributeLargeObject            (const LargeObject &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeLargeObject            (const LargeObject &data, const LargeObject &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeLargeObject            (LargeObject *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeLargeObject            (LargeObject *pData, const LargeObject &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeLargeObject            (const AttributeLargeObject &attribute);
        virtual                        ~AttributeLargeObject            ();
                AttributeLargeObject   &operator =                      (const AttributeLargeObject &attribute);
                LargeObject             getValue                        () const;
                void                    setValue                        (const LargeObject &data);
                bool                    isCurrentValueSameAsDefault     () const;
                LargeObject             getDefaultValue                 () const;
                bool                    getIsDefaultDataValidFlag       () const;
        virtual string                  getSqlType                      ();
        virtual void                    setupOrm                        (OrmTable *pOrmTable);
        virtual string                  getSqlForCreate                 ();
        virtual void                    getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                    getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                    getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                    toString                        (string &valueString);
        virtual void                    fromString                      (const string &valueString);
        virtual void                   *getPData                        ();
        virtual void                    setValue                        (const void *pData);
        virtual Attribute              *clone                           ();
        static  map<string, string>     getSupportedConversions         ();
        virtual void                    setDefaultValue                 ();
        virtual void                    getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        LargeObject *m_pData;
        LargeObject  m_defaultData;
        bool         m_isDefaultDataValid;

    protected :
    public :
};

}
#endif // ATTRIBUTELARGEOBJECT_H
