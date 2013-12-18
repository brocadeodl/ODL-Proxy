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

#ifndef ATTRIBUTETIME_H
#define ATTRIBUTETIME_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeVector.h"
#include "Framework/Types/Time.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeTime : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeTime                   (const Time &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeTime                   (const Time &data, const Time &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeTime                   (Time *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeTime                   (Time *pData, const Time &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeTime                   (const AttributeTime &attribute);
        virtual                        ~AttributeTime                   ();
                AttributeTime          &operator =                      (const AttributeTime &attribute);
                Time                    getValue                        () const;
                void                    getAllValues                    (UI8 *hour, UI8 *min, UI8 *sec, UI32 *micro, SI8 *timezonehours, SI8 *timezoneminutes);
                void                    setValue                        (const Time &data);
                bool                    isCurrentValueSameAsDefault     () const;
                Time                    getDefaultValue                 () const;
                bool                    getIsDefaultValueValidFlag      () const;
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
        virtual void                    getPlainString                  (string &plainString);

    // Now the data members

    private :
        Time *m_pData;
        Time  m_defaultData;
        bool  m_isDefaultDataValid;

    protected :
    public :
};

class AttributeTimeVector : public Attribute, public AttributeVector
{
    private :
        virtual bool validate () const;

    protected :
    public :
                                              AttributeTimeVector          (const vector<Time> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeTimeVector          (const vector<Time> &data, const vector<Time> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeTimeVector          (vector<Time> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeTimeVector          (vector<Time> *pData, const vector<Time> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeTimeVector          (const AttributeTimeVector &attribute);
        virtual                              ~AttributeTimeVector          ();
                AttributeTimeVector &operator =                            (const AttributeTimeVector &attribute);
                vector<Time>                  getValue                     () const;
                void                          setValue                     (const vector<Time> &data);
                bool                          isCurrentValueSameAsDefault  () const;
                vector<Time>                  getDefaultValue              () const;
                bool                          getIsDefaultDataValidFlag    () const;
        virtual string                        getSqlType                   ();
        virtual void                          setupOrm                     (OrmTable *pOrmTable);
        virtual string                        getSqlForCreate              ();
        virtual void                          getSqlForInsert              (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                          getSqlForUpdate              (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                          getSqlForSelect              (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                          toString                     (string &valueString);
        virtual void                          fromString                   (const string &valueString);
        virtual void                         *getPData                     ();
        virtual void                          setValue                     (const void *pData);
        virtual Attribute                    *clone                        ();
        static  map<string, string>           getSupportedConversions      ();
        virtual void                          setDefaultValue              ();
        virtual void                          getCValue                    (WaveCValue *pCValue);
        virtual void                          addAttributeToVector         (Attribute *attribute);
        virtual void                          deleteAttributeFromVector    (Attribute *attribute);
    // Now the data members

    private :
        vector<Time> *m_pData;
        vector<Time>  m_defaultData;
        bool          m_isDefaultDataValid;

    protected :
    public :
};

}

#endif // ATTRIBUTETIME_H
