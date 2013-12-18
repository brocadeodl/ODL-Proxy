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
 *   Author : Mandar Datar                                                 *
 *   Note   : Copied from Framework/Attributes/AttributeUI32UC.h           *
 ***************************************************************************/

#ifndef ATTRIBUTEUI64UC_H
#define ATTRIBUTEUI64UC_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeUC.h"
#include "Framework/Types/UI64UC.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeUI64UC : public Attribute, public AttributeUC
{
    private :
        virtual bool                    validate                        () const;

    protected :
    public :
                                        AttributeUI64UC                 (const UI64UC &data, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64UC                 (const UI64UC &data, const bool &isNoElement, const bool &defaultFlag, const UI64 &defaultUI64UC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64UC                 (UI64UC *pData, const bool &isNoElement, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64UC                 (UI64UC *pData, const bool &isNoElement, const bool &defaultFlag, const UI64 &defaultUI64UC, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64UC                 (const AttributeUI64UC &attribute);
        virtual                        ~AttributeUI64UC                 ();
                AttributeUI64UC        &operator =                      (const AttributeUI64UC &attribute);

                UI64UC                  getValue                        () const;
                void                    setValue                        (const UI64UC &data);

                bool                    isCurrentValueSameAsDefault     () const;
                UI64                    getDefaultUI64                  () const;
                bool                    getIsDefaultUI64ValidFlag       () const;

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
        virtual void                    getCValue                       (WaveCValue *pCValue);
        virtual bool                    getIsUserConfigured             ();
    // Now the data members

    private :
        UI64UC   *m_pData;
        bool      m_isNoElement;
        UI64      m_defaultUI64;
        bool      m_isDefaultUI64Valid;

    protected :
    public :
};

}

#endif  //ATTRIBUTEUI64UC_H

