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

#ifndef ATTRIBUTEENUMUC_H
#define ATTRIBUTEENUMUC_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeUC.h"
#include "Framework/Types/UI32UC.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeEnumUC : public Attribute, public AttributeUC
{
    private :
        virtual bool                validate                        () const;

    protected :
    public :
                                    AttributeEnumUC                 (const EnumUC &data, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeEnumUC                 (const EnumUC &data, const bool &isNoElement, const bool &defaultFlag, const UI32 &defaultEnumUC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeEnumUC                 (EnumUC *pData, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeEnumUC                 (EnumUC *pData, const bool &isNoElement, const bool &defaultFlag, const UI32 &defaultEnumUC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeEnumUC                 (const AttributeEnumUC &attribute);
        virtual                    ~AttributeEnumUC                 ();
                AttributeEnumUC    &operator =                      (const AttributeEnumUC &attribute);

                EnumUC              getValue                        () const;
                void                setValue                        (const EnumUC &data);
                
                bool                isCurrentValueSameAsDefault     () const;
                UI32                getDefaultEnum                  () const;
                bool                getIsDefaultEnumValidFlag       () const;

                bool                getIsNoElement                  () const;

        virtual string              getSqlType                      ();
        virtual void                setupOrm                        (OrmTable *pOrmTable);
        virtual string              getSqlForCreate                 ();
        virtual void                getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                toString                        (string &valueString);
        virtual void                fromString                      (const string &valueString);
        virtual ResourceId          loadFromPlainString             (const string &valueString);
        virtual void               *getPData                        ();
        virtual void                setValue                        (const void *pData);
        virtual Attribute          *clone                           ();
        static  map<string, string> getSupportedConversions         ();
        virtual void                setDefaultValue                 ();
		virtual void 		        getCValue                       (WaveCValue *pCValue);
        virtual bool                getIsUserConfigured             ();
    // Now the data members

    private :
        EnumUC   *m_pData;
        bool      m_isNoElement;
        UI32      m_pDefaultEnum;
        bool      m_IsDefaultEnumValid;

    protected :
    public :
};

}

#endif  //ATTRIBUTEENUMUC_H

