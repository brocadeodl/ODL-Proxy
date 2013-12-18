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
 *   Copyright (C) 2005 - 2012 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Chaitanya Gangwar                                            *
 ***************************************************************************/

#ifndef ATTRIBUTEUI32VECTORVECTOR_H
#define ATTRIBUTEUI32VECTORVECTOR_H

#include "Framework/Attributes/Attribute.h"

namespace WaveNs
{

class AttributeUI32VectorVector : public Attribute
{
    private :
        virtual bool validate                     () const;
        virtual bool isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                             AttributeUI32VectorVector (const vector<vector<UI32> > &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeUI32VectorVector (const vector<vector<UI32> > &data, const vector<vector<UI32> > &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeUI32VectorVector (vector<vector<UI32> > *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeUI32VectorVector (vector<vector<UI32> > *pData, const vector<vector<UI32> > &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeUI32VectorVector (const AttributeUI32VectorVector &attribute);
        virtual                             ~AttributeUI32VectorVector ();
                AttributeUI32VectorVector &operator =                 (const AttributeUI32VectorVector &attribute);
                vector<vector<UI32> >        getValue                    () const;
                void                         setValue                    (const vector<vector<UI32> > &m_data);
                bool                         isCurrentValueSameAsDefault () const;
                vector<vector<UI32> >        getDefaultData              () const;
                bool                         getIsDefaultDataValidFlag   () const;
        virtual string                       getSqlType                  ();
        virtual void                         setupOrm                    (OrmTable *pOrmTable);
        virtual string                       getSqlForCreate             ();
        virtual void                         getSqlForInsert             (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                         getSqlForUpdate             (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                         getSqlForSelect             (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                         toString                    (string &valueString);
        virtual void                         fromString                  (const string &valueString);
        virtual void                        *getPData                    ();
        virtual void                         setValue                    (const void *pData);
        virtual Attribute                   *clone                       ();
        static  map<string, string>          getSupportedConversions     ();
        virtual void                         setDefaultValue             ();
        virtual void                         getCValue                   (WaveCValue *pCValue);

    // Now the data members

    private :
        vector<vector<UI32> > *m_pData;
        vector<vector<UI32> >  m_defaultData;
        bool                   m_isDefaultDataValid;

    protected :
    public :
};

}

#endif // ATTRIBUTESTRINGVECTORVECTOR_H
