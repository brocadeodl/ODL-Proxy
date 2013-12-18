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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef ATTRIBUTESNMPOBJECTID_H
#define ATTRIBUTESNMPOBJECTID_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Types/SnmpObjectId.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeSnmpObjectId : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeSnmpObjectId           (const SnmpObjectId &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSnmpObjectId           (const SnmpObjectId &data, const SnmpObjectId &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSnmpObjectId           (SnmpObjectId *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSnmpObjectId           (SnmpObjectId *pData, const SnmpObjectId &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSnmpObjectId           (const AttributeSnmpObjectId &attribute);
        virtual                        ~AttributeSnmpObjectId           ();
                AttributeSnmpObjectId  &operator =                      (const AttributeSnmpObjectId &attribute);
                SnmpObjectId            getValue                        () const;
                void                    getAllValues                    (UI32  *oid, SI32 *length);
                void                    setValue                        (const SnmpObjectId &data);
                bool                    isCurrentValueSameAsDefault     () const;
                SnmpObjectId            getDefaultValue                 () const;
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
        virtual void                    getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        SnmpObjectId *m_pData;
        SnmpObjectId  m_defaultData;
        bool          m_isDefaultDataValid;

    protected :
    public :
};

}

#endif  //ATTRIBUTESNMPOBJECTID_H



