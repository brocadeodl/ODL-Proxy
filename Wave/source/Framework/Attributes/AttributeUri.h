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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ATTRIBUTEURI_H
#define ATTRIBUTEURI_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Types/Uri.h"

#include <map>

using namespace std;

namespace WaveNs
{

class OrmTable;

class AttributeUri : public Attribute
{
    private :
        virtual bool                validate                        () const;
        virtual bool                isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                     AttributeUri                (const Uri &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeUri                (const Uri &data, const bool &defaultFlag, const Uri &defaultUri, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeUri                (Uri *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeUri                (Uri *pData, const bool &defaultFlag, const Uri &defaultUri, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeUri                (const AttributeUri &attribute);
        virtual                     ~AttributeUri                ();
                AttributeUri        &operator =                  (const AttributeUri &attribute);
                Uri                  getValue                    () const;
                void                 setValue                    (const Uri &data);
                bool                 isCurrentValueSameAsDefault () const;
                Uri                  getDefaultUri               () const;
                bool                 getIsDefaultUriValidFlag    () const;
        virtual string               getSqlType                  ();
        virtual void                 setupOrm                    (OrmTable *pOrmTable);
        virtual string               getSqlForCreate             ();
        virtual void                 getSqlForInsert             (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                 getSqlForUpdate             (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                 getSqlForSelect             (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                 toString                    (string &valueString);
        virtual void                 fromString                  (const string &valueString);
        virtual void                *getPData                    ();
        virtual void                 setValue                    (const void *pData);
        virtual Attribute           *clone                       ();
        static  map<string, string>  getSupportedConversions     ();
        virtual void                 setDefaultValue             ();
                void                 toEscapedString             (string &valueString);
        virtual void                 getCValue                   (WaveCValue *pCValue);

    // Now the data members

    private :
        Uri  *m_pData;
        Uri   m_defaultUri;
        bool  m_isDefaultUriValid;

    protected :
    public :
};

}

#endif // ATTRIBUTEURI_H

