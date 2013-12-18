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

#ifndef ATTRIBUTEDATETIME_H
#define ATTRIBUTEDATETIME_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeVector.h"
#include "Framework/Types/DateTime.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeDateTime : public Attribute 
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeDateTime               (const DateTime &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDateTime               (const DateTime &data, const DateTime &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDateTime               (DateTime *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDateTime               (DateTime *pData, const DateTime &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeDateTime               (const AttributeDateTime &attribute);
        virtual                        ~AttributeDateTime               ();
                AttributeDateTime       &operator =                     (const AttributeDateTime &attribute);
                DateTime                getValue                        () const;
                bool                    isCurrentValueSameAsDefault     () const;
                DateTime                getDefaultValue                 () const;
                bool                    getIsDefaultDataValidFlag       () const;
                void                    getAllValues                    (SI16 *year, UI8 *month, UI8 *day, UI8 *hour, UI8 *min, UI8 *sec, UI32 *micro, SI8 *timezone, SI8 *timezone_minutes);
                void                    setValue                        (const DateTime &data);
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
        virtual void                    getCValue                       (WaveCValue *pCValue);
        virtual void                    getPlainString                  (string &plainString);

    // Now the data members

    private :
        DateTime *m_pData;
        DateTime m_defaultData;
        bool     m_isDefaultDataValid;
    protected :
    public :
};

class AttributeDateTimeVector : public Attribute, public AttributeVector
{
    private :
        virtual bool validate () const;

    protected :
    public :
                                              AttributeDateTimeVector      (const vector<DateTime> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateTimeVector      (const vector<DateTime> &data, const vector<DateTime> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateTimeVector      (vector<DateTime> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateTimeVector      (vector<DateTime> *pData, const vector<DateTime> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeDateTimeVector      (const AttributeDateTimeVector &attribute);
        virtual                              ~AttributeDateTimeVector      ();
                AttributeDateTimeVector       &operator =                  (const AttributeDateTimeVector &attribute);
                vector<DateTime>              getValue                     () const;
                void                          setValue                     (const vector<DateTime> &data);
                bool                          isCurrentValueSameAsDefault  () const;
                vector<DateTime>              getDefaultValue              () const;
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
        vector<DateTime> *m_pData;
        vector<DateTime>  m_defaultData;
        bool              m_isDefaultDataValid;

    protected :
    public :
};

}

#endif // ATTRIBUTEDATETIME_H
