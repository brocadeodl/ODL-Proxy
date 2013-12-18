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
 *   Author : Himanshu Varshney                                    *
 ***************************************************************************/

#ifndef ATTRIBUTESI64VECTOR_H
#define ATTRIBUTESI64VECTOR_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeVector.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeSI64Vector : public Attribute, public AttributeVector
{
    private :
        virtual bool                    validate                        () const;

    protected :
    public :
                                        AttributeSI64Vector             (const vector<SI64> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI64Vector             (const vector<SI64> &data, const vector<SI64> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI64Vector             (vector<SI64> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI64Vector             (vector<SI64> *pData, const vector<SI64> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI64Vector             (const AttributeSI64Vector &attribute);
        virtual                        ~AttributeSI64Vector             ();
                AttributeSI64Vector    &operator =                      (const AttributeSI64Vector &attribute);
                vector<SI64>            getValue                        () const;
                void                    setValue                        (const vector<SI64> &data);
                vector<SI64>            getDefaultValue                 () const;
                bool                    isCurrentValueSameAsDefault     () const;
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
        virtual void                    addAttributeToVector            (Attribute *attribute);
        virtual void                    deleteAttributeFromVector       (Attribute *attribute);
    // Now the data members

    private :
        vector<SI64> *m_pData;
        vector<SI64>  m_defaultData;
        bool          m_isDefaultDataValid;
    protected :
    public :
};

}

#endif // ATTRIBUTESI64VECTOR_H

