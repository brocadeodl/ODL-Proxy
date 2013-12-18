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

#ifndef ATTRIBUTEUI32RANGE_H
#define ATTRIBUTEUI32RANGE_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Types/UI32Range.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeUI32Range : public Attribute
{
    private :
        virtual bool                    validate                        () const;

    protected :
    public :
                                        AttributeUI32Range              (const UI32Range &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);

                                        AttributeUI32Range              (const UI32Range &data, const UI32Range &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);

                                        AttributeUI32Range              (UI32Range *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);

                                        AttributeUI32Range              (UI32Range *pData, const UI32Range &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);

                                        AttributeUI32Range              (const AttributeUI32Range &attribute);
        virtual                        ~AttributeUI32Range              ();
                AttributeUI32Range     &operator =                      (const AttributeUI32Range &attribute);
                UI32Range               getValue                        () const;
                void                    setValue                        (const UI32Range &data);
        virtual string                  getSqlType                      ();
        virtual void                    setupOrm                        (OrmTable *pOrmTable);
        virtual string                  getSqlForCreate                 ();
        virtual void                    getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                    getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                    getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                    getSqlForSelectUsingRange       (string &sqlForSelect);
        virtual void                    toString                        (string &valueString);
        virtual void                    fromString                      (const string &valueString);
        virtual ResourceId              loadFromPlainString             (const string &valueString);
        virtual void                   *getPData                        ();
        virtual void                    setValue                        (const void *pData);
        virtual Attribute              *clone                           ();
        static  map<string, string>     getSupportedConversions         ();
        
                bool                    isCurrentValueSameAsDefault     () const;    
            UI32Range                   getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       (void) const;
        virtual void                    setDefaultValue                 ();
        virtual void                    getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        UI32Range *m_pData;
        UI32Range  m_defaultData;
        bool       m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

}

#endif  //ATTRIBUTESNMPOBJECTID_H



