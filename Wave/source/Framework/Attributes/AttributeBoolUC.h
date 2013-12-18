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
 *   Author : Himanshu                                                     *
 ***************************************************************************/

#ifndef ATTRIBUTEBOOLUC_H
#define ATTRIBUTEBOOLUC_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeUC.h"
#include "Framework/Types/BoolUC.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeBoolUC : public Attribute, public AttributeUC
{
    private :
        virtual bool                    validate ()                     const;

    protected :
    public :
                                        AttributeBoolUC                 (const BoolUC &data, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeBoolUC                 (const BoolUC &data, const bool &isNoElement, const string &defaultFlag, const string &defaultBoolUC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeBoolUC                 (BoolUC *pData, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeBoolUC                 (BoolUC *pData, const bool &isNoElement, const string &defaultFlag, const string &defaultBoolUC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeBoolUC                 (const AttributeBoolUC &attribute);
        virtual                        ~AttributeBoolUC                 ();
                AttributeBoolUC        &operator =                      (const AttributeBoolUC &attribute);

                BoolUC                  getValue                        () const;
                void                    setValue                        (const BoolUC &data);
                        
                bool                    isCurrentValueSameAsDefault     () const;
                bool                    getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       () const;
                bool                    getIsNoElement                  () const;

        virtual string                  getSqlType                      ();
        virtual void                    setupOrm                        (OrmTable *pOrmTable);
        virtual string                  getSqlForCreate                 ();
        virtual void                    getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                    getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                    getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                    toString                        (string &valueString);
        virtual void                    fromString                      (const string &valueString);
        virtual void                    getPlainString                  (string &valueString);
        virtual ResourceId              loadFromPlainString             (const string &valueString);
        virtual void                   *getPData                        ();
        virtual void                    setValue                        (const void *pData);
        virtual Attribute              *clone                           ();
        static  map<string, string>     getSupportedConversions         ();
        virtual void                    setDefaultValue                 ();
                void                    setDefaultDataForBoolUC         ( const string &defaultFlag, const string &defaultBoolUC );
        virtual void                    getCValue                       (WaveCValue *pCValue);
        virtual bool                    getIsUserConfigured             ();     
    private :
        BoolUC *m_pData;
        bool    m_isNoElement;
        bool    m_defaultData;
        bool    m_isDefaultDataValid;

    protected :
    public :
};

}

#endif  //ATTRIBUTEBOOLUC_H

