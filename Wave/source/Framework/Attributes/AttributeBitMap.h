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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef ATTRIBUTEBITMAP_H
#define ATTRIBUTEBITMAP_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Types/BitMap.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeBitMap : public Attribute
{
    private :
        virtual bool                validate                        () const;

    protected :
    public :
                                    AttributeBitMap                 (const BitMap &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false, const bool force32bitPresentation = false);
                                    AttributeBitMap                 (const BitMap &data, const BitMap &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false, const bool force32bitPresentation = false);
                                    AttributeBitMap                 (BitMap *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false, const bool force32bitPresentation = false);
                                    AttributeBitMap                 (BitMap *pData, const BitMap &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false, const bool force32bitPresentation = false);
                                    AttributeBitMap                 (const AttributeBitMap &attribute);
        virtual                    ~AttributeBitMap                 ();
                AttributeBitMap    &operator =                      (const AttributeBitMap &attribute);
                BitMap              getValue                        () const;
                void                setValue                        (const BitMap &data);
                bool                isCurrentValueSameAsDefault     () const;
                bool                getIsDefaultBitMapValidFlag     () const;
                BitMap              getDefaultBitMap                () const;
                bool                getForce32BitRepresentationFlag () const;
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
        virtual void                getCValue                       (WaveCValue *pCValue);

                void                populateNameValueBitMap         (string name, UI32 bitPosition); 
                void                getBitNameFromBitPosition       (string &name, UI32 &bitPosition); 
                void                getBitPositionFromBitName       (string &name, UI32 &bitPosition); 
    // Now the data members
    private :
        BitMap *m_pData;
        BitMap  m_defaultBitMap;
        bool    m_isDefaultBitMapValid;
        bool    m_force32BitRepresentation;

        map <string, UI32>      m_bitNameToBitPositionMap;
        map <UI32, string>      m_bitPositionToBitNameMap;

    protected :
    public :
};

}

#endif //ATTRIBUTEBITMAP_H
