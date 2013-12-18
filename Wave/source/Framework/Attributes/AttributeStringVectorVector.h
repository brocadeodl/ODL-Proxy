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
 *   Copyright (C) 2005 - 2011 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ATTRIBUTESTRINGVECTORVECTOR_H
#define ATTRIBUTESTRINGVECTORVECTOR_H

#include "Framework/Attributes/Attribute.h"

namespace WaveNs
{

class AttributeStringVectorVector : public Attribute
{
    private :
        virtual bool validate                     () const;
        virtual bool isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                             AttributeStringVectorVector (const vector<vector<string> > &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeStringVectorVector (const vector<vector<string> > &data, const vector<vector<string> > &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeStringVectorVector (vector<vector<string> > *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeStringVectorVector (vector<vector<string> > *pData, const vector<vector<string> > &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                             AttributeStringVectorVector (const AttributeStringVectorVector &attribute);
        virtual                             ~AttributeStringVectorVector ();
                AttributeStringVectorVector &operator =                 (const AttributeStringVectorVector &attribute);
                vector<vector<string> >      getValue                    () const;
                void                         setValue                    (const vector<vector<string> > &m_data);
                bool                         isCurrentValueSameAsDefault () const;
                vector<vector<string> >      getDefaultData              () const;
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
                void                         toEscapedString             (string &valueString);
        virtual void                         getCValue                   (WaveCValue *pCValue);

    // Now the data members

    private :
        vector<vector<string> > *m_pData;
        vector<vector<string> >  m_defaultData;
        bool                     m_isDefaultDataValid;

    protected :
    public :
};

}

#endif // ATTRIBUTESTRINGVECTORVECTOR_H
