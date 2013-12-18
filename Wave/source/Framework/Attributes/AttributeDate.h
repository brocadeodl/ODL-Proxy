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
 *   Author : Himanshu Varshney, Amit Agrawal                              *
 ***************************************************************************/

#ifndef ATTRIBUTEDATE_H
#define ATTRIBUTEDATE_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeVector.h"
#include "Framework/Types/Date.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeDate : public Attribute 
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeDate                   (const Date &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDate                   (const Date &data, const Date &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDate                   (Date *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDate                   (Date *pData, const Date &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDate                   (const AttributeDate &attribute);
        virtual                        ~AttributeDate                   ();
                AttributeDate          &operator =                      (const AttributeDate &attribute);
                Date                    getValue                        () const;
                void                    getAllValues                    (SI16 *year, UI8 *month, UI8 *day, SI8 *timezone, SI8 *timezone_minutes);
                void                    setValue                        (const Date &data);
                bool                    isCurrentValueSameAsDefault     () const;
                Date                    getDefaultValue                 () const;
                bool                    getIsDefaultDataValidFlag       () const;
        virtual string                  getSqlType                      ();
        virtual void                    setupOrm                        (OrmTable *pOrmTable);
        virtual string                  getSqlForCreate                 ();
        virtual void                    getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                    getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                    getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                    toString                        (string &valueString);
        virtual void                    fromString                      (const string &valueString);
        virtual ResourceId              loadFromPlainString             (const string &valueString);
        virtual void                   *getPData                        ();
        virtual void                    setValue                        (const void *pData);
        virtual Attribute              *clone                           ();
        static  map<string, string>     getSupportedConversions         ();
        virtual void                    setDefaultValue                 ();
		virtual void 		            getCValue                       (WaveCValue *pCValue);
        virtual void                    getPlainString                  (string &plainString);

    // Now the data members

    private :
        Date *m_pData;
        Date  m_defaultData;
        bool  m_isDefaultDataValid;
    protected :
    public :
};

class AttributeDateVector : public Attribute, public AttributeVector 
{
    private :
        virtual bool validate () const;

    protected :
    public :
                                              AttributeDateVector (const vector<Date> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateVector (const vector<Date> &data, const vector<Date> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateVector (vector<Date> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateVector (vector<Date> *pData, const vector<Date> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateVector (const AttributeDateVector &attribute);
        virtual                              ~AttributeDateVector ();
                AttributeDateVector &operator =                   (const AttributeDateVector &attribute);
                vector<Date>         getValue                     () const;
                void                 setValue                     (const vector<Date> &data);
                bool                 isCurrentValueSameAsDefault  () const;
                vector<Date>         getDefaultValue              () const;
                bool                 getIsDefaultDataValidFlag    () const;
        virtual string               getSqlType                   ();
        virtual void                 setupOrm                     (OrmTable *pOrmTable);
        virtual string               getSqlForCreate              ();
        virtual void                 getSqlForInsert              (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                 getSqlForUpdate              (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                 getSqlForSelect              (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                 toString                     (string &valueString);
        virtual void                 fromString                   (const string &valueString);
        virtual void                *getPData                     ();
        virtual void                 setValue                     (const void *pData);
        virtual Attribute           *clone                        ();
        virtual void                 setDefaultValue              ();
        static  map<string, string>  getSupportedConversions      ();
		virtual void 		         getCValue                    (WaveCValue *pCValue);
        virtual void                 addAttributeToVector         (Attribute *attribute);
        virtual void                 deleteAttributeFromVector    (Attribute *attribute);
    // Now the data members

    private :
        vector<Date> *m_pData;
        vector<Date>  m_defaultData;
        bool          m_isDefaultDataValid;

    protected :
    public :
};

}

#endif // ATTRIBUTEDATE_H
