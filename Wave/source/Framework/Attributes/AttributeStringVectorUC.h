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
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef ATTRIBUTESTRINGVECTORUC_H
#define ATTRIBUTESTRINGVECTORUC_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeUC.h"
#include "Framework/Types/StringVectorUC.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeStringVectorUC : public Attribute, public AttributeUC
{
    private :
        virtual bool                        validate                        () const;

    protected :
    public :
                                            AttributeStringVectorUC         (const StringVectorUC &data, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeStringVectorUC         (const StringVectorUC &data, const bool &isNoElement, const bool &defaultFlag, const vector<string> &defaultStringVectorUC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeStringVectorUC         (StringVectorUC *pData, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeStringVectorUC         (StringVectorUC *pData, const bool &isNoElement, const bool &defaultFlag, const vector<string> &defaultStringVectorUC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeStringVectorUC         (const AttributeStringVectorUC &attribute);
        virtual                            ~AttributeStringVectorUC         ();
                AttributeStringVectorUC    &operator =                      (const AttributeStringVectorUC &attribute);

                StringVectorUC              getValue                        () const;
                void                        setValue                        (const StringVectorUC &data);

                bool                        isCurrentValueSameAsDefault     () const;
                vector<string>              getDefaultStringVector          () const;
                bool                        getIsDefaultStringValidFlag     () const;

                bool                        getIsNoElement                  () const;

        virtual string                      getSqlType                      ();
        virtual void                        setupOrm                        (OrmTable *pOrmTable);
        virtual string                      getSqlForCreate                 ();
        virtual void                        getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                        getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                        getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                        toString                        (string &valueString);
        virtual void                        fromString                      (const string &valueString);
        virtual void                       *getPData                        ();
        virtual void                        setValue                        (const void *pData);
        virtual Attribute                  *clone                           ();
        static  map<string, string>         getSupportedConversions         ();
        virtual void                        setDefaultValue                 ();
        virtual void                        getCValue                       (WaveCValue *pCValue);
        virtual bool                        getIsUserConfigured             ();
        virtual void                        getPlainString                  (string &valueString);

    // Now the data members

    private :
        StringVectorUC *m_pData;
        bool            m_isNoElement;
        vector<string>  m_defaultStringVector;
        bool            m_isDefaultStringValid;

    protected :
    public :
};

}

#endif  //ATTRIBUTESTRINGVECTORUC_H

