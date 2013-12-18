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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeVector.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/MacAddress.h"
#include "Framework/Types/MacAddress2.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV6Address.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeUI32 : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeUI32                   (const UI32 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI32                   (const UI32 &data, const UI32 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0,const bool &isOperational = false);
                                AttributeUI32                   (UI32 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI32                   (UI32 *pData, const UI32 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI32                   (const AttributeUI32 &attribute);
        virtual                ~AttributeUI32                   ();
                AttributeUI32  &operator =                      (const AttributeUI32 &attribute);
                UI32            getValue                        () const;
                void            setValue                        (const UI32 &data);
                bool            isCurrentValueSameAsDefault     () const;
                UI32            getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       (void) const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        UI32 *m_pData;
        UI32  m_defaultData;
        bool  m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

typedef AttributeUI32       AttributePrismServiceId;
typedef AttributeUI32       AttributeSerializableObjectType;
typedef AttributeUI32       AttributeLocationId;
typedef AttributeUI32       AttributeTraceClientId;

class AttributeUI32Vector : public Attribute, public AttributeVector
{
    private :
        virtual bool                    validate                        () const;

    protected :
    public :
                                        AttributeUI32Vector             (const vector<UI32> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI32Vector             (const vector<UI32> &data, const vector<UI32> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI32Vector             (vector<UI32> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI32Vector             (vector<UI32> *pData, const vector<UI32> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI32Vector             (const AttributeUI32Vector &attribute);
        virtual                        ~AttributeUI32Vector             ();
                AttributeUI32Vector    &operator =                      (const AttributeUI32Vector &attribute);
                vector<UI32>            getValue                        () const;
                void                    setValue                        (const vector<UI32> &data);
                bool                    isCurrentValueSameAsDefault     () const;
                vector<UI32>            getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       (void) const;
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
        virtual void                    addAttributeToVector            (Attribute *attribute);
        virtual void                    deleteAttributeFromVector       (Attribute *attribute);
    // Now the data members

    private :

        vector<UI32> *m_pData;
        vector<UI32>  m_defaultData;
        bool          m_isDefaultDataValid;
    protected :
    public :
};

typedef AttributeUI32Vector       AttributePrismServiceIdVector;
typedef AttributeUI32Vector       AttributeSerializableObjectTypeVector;
typedef AttributeUI32Vector       AttributeLocationIdVector;
typedef AttributeUI32Vector       AttributeTraceClientIdVector;

class AttributeUI64 : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeUI64                   (const UI64 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI64                   (const UI64 &data, const UI64 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI64                   (UI64 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI64                   (UI64 *pData, const UI64 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI64                   (const AttributeUI64 &attribute);
        virtual                ~AttributeUI64                   ();
                AttributeUI64  &operator =                      (const AttributeUI64 &attribute);
                UI64            getValue                        () const;
                void            setValue                        (const UI64 &data);
                bool            isCurrentValueSameAsDefault     () const;
                UI64            getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       (void) const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        UI64 *m_pData;
        UI64  m_defaultData;
        bool  m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

class AttributeUI64Vector : public Attribute, public AttributeVector
{
    private :
        virtual bool                    validate                        () const;

    protected :
    public :
                                        AttributeUI64Vector             (const vector<UI64> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64Vector             (const vector<UI64> &data, const vector<UI64> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64Vector             (vector<UI64> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64Vector             (vector<UI64> *pData, const vector<UI64> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeUI64Vector             (const AttributeUI64Vector &attribute);
        virtual                        ~AttributeUI64Vector             ();
                AttributeUI64Vector    &operator =                      (const AttributeUI64Vector &attribute);
                vector<UI64>            getValue                        () const;
                void                    setValue                        (const vector<UI64> &data);
                bool                    isCurrentValueSameAsDefault     () const;
                vector<UI64>            getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       () const;
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
        virtual void                    getCValue                       (WaveCValue *pCValue );
        virtual void                    addAttributeToVector            (Attribute *attribute);
        virtual void                    deleteAttributeFromVector       (Attribute *attribute);
    // Now the data members

    private :
        vector<UI64> *m_pData;
        vector<UI64>  m_defaultData;
        bool          m_isDefaultDataValid;
    protected :
    public :
};

class AttributeSI32 : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeSI32                   (const SI32 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI32                   (const SI32 &data, const SI32 &defData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI32                   (SI32 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI32                   (SI32 *pData, const SI32 &defData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI32                   (const AttributeSI32 &attribute);
        virtual                ~AttributeSI32                   ();
                AttributeSI32  &operator =                      (const AttributeSI32 &attribute);
                SI32            getValue                        () const;
                void            setValue                        (const SI32 &data);
                bool            isCurrentValueSameAsDefault     () const;
                SI32            getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       (void) const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        SI32 *m_pData;
        SI32  m_defaultData;
        bool  m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

class AttributeSI32Vector : public Attribute, public AttributeVector
{
    private :
        virtual bool                    validate                        () const;

    protected :
    public :
                                        AttributeSI32Vector             (const vector<SI32> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI32Vector             (const vector<SI32> &data, const vector<SI32> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI32Vector             (vector<SI32> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI32Vector             (vector<SI32> *pData, const vector<SI32> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeSI32Vector             (const AttributeSI32Vector &attribute);
        virtual                        ~AttributeSI32Vector             ();
                AttributeSI32Vector    &operator =                      (const AttributeSI32Vector &attribute);
                vector<SI32>            getValue                        () const;
                void                    setValue                        (const vector<SI32> &data);
                bool                    isCurrentValueSameAsDefault     () const;
                vector<SI32>            getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       () const;
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
        virtual void                    getCValue                       (WaveCValue *pCValue );
        virtual void                    addAttributeToVector            (Attribute *attribute);
        virtual void                    deleteAttributeFromVector       (Attribute *attribute);
        virtual void                    getPlainString                  (string &valueString);
    // Now the data members

    private :
        vector<SI32> *m_pData;
        vector<SI32>  m_defaultData;
        bool          m_isDefaultDataValid;
    protected :
    public :
};

class AttributeUI16 : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeUI16                   (const UI16 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI16                   (const UI16 &data, const UI16 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI16                   (UI16 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI16                   (UI16 *pData, const UI16 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI16                   (const AttributeUI16 &attribute);
        virtual                ~AttributeUI16                   ();
                AttributeUI16  &operator =                      (const AttributeUI16 &attribute);
                UI16            getValue                        () const;
                void            setValue                        (const UI16 &data);
                bool            isCurrentValueSameAsDefault     () const;
                UI16            getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       (void) const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        UI16 *m_pData;
        UI16  m_defaultData;
        bool  m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

class AttributeSI16 : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeSI16                   (const SI16 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI16                   (const SI16 &data, const SI16 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI16                   (SI16 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI16                   (SI16 *pData, const SI16 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI16                   (const AttributeSI16 &attribute);
        virtual                ~AttributeSI16                   ();
                AttributeSI16  &operator =                      (const AttributeSI16 &attribute);
                SI16            getValue                        () const;
                void            setValue                        (const SI16 &data);
                bool            isCurrentValueSameAsDefault     () const;
                SI16            getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       () const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        SI16 *m_pData;
        SI16  m_defaultData;
        bool  m_isDefaultDataValid;
    protected :
    public :
};

class AttributeUI8 : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeUI8                    (const UI8 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI8                    (const UI8 &data, const UI8 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI8                    (UI8 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI8                    (UI8 *pData, const UI8 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUI8                    (const AttributeUI8 &attribute);
        virtual                ~AttributeUI8                    ();
                AttributeUI8   &operator =                      (const AttributeUI8 &attribute);
                UI8             getValue                        () const;
                void            setValue                        (const UI8 &data);
                bool            isCurrentValueSameAsDefault     () const;
                UI8             getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       (void) const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        UI8 *m_pData;
        UI8  m_defaultData;
        bool m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

class AttributeSI8 : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeSI8                    (const SI8 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI8                    (const SI8 &data, const SI8 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI8                    (SI8 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI8                    (SI8 *pData, const SI8 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeSI8                    (const AttributeSI8 &attribute);
        virtual                ~AttributeSI8                    ();
                AttributeSI8   &operator =                      (const AttributeSI8 &attribute);
                SI8             getValue                        () const;
                void            setValue                        (const SI8 &data);
                bool            isCurrentValueSameAsDefault     () const;
                SI8             getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       () const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        SI8 *m_pData;
        SI8  m_defaultData;
        bool m_isDefaultDataValid;

    protected :
    public :
};

class AttributeBool : public Attribute
{
    private :
        virtual bool           validate        () const;

    protected :
    public :
                               AttributeBool   (const bool &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                               AttributeBool   (const bool &data, const bool &defaultFlag, const string &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false); 
                               AttributeBool   (bool *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                               AttributeBool   (bool *pData, const bool &defaultFlag, const string &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false); 
                               AttributeBool   (const AttributeBool &attribute);
        virtual               ~AttributeBool   ();
                AttributeBool &operator =      (const AttributeBool &attribute);
                bool           getValue        () const;
                void           setValue        (const bool &data);
                bool           isCurrentValueSameAsDefault      () const;
                bool           getDefaultData  () const;
                bool           getIsDefaultDataValidFlag        () const;                    
        virtual string         getSqlType      ();
        virtual void           setupOrm        (OrmTable *pOrmTable);
        virtual string         getSqlForCreate ();
        virtual void           getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void           getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void           getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void           toString        (string &valueString);
        virtual void           fromString      (const string &valueString);
        virtual ResourceId     loadFromPlainString             (const string &valueString);
        virtual void          *getPData        ();
        virtual void           setValue        (const void *pData);
        virtual Attribute     *clone           ();
        static  map<string, string>     getSupportedConversions         ();
        virtual void           setDefaultValue ();
                void           setDefaultDataForBool ( const bool &defaultFlag, const string &defaultData );     
        virtual void           getCValue       (WaveCValue *pCValue);
    // Now the data members

    private :
        bool     *m_pData;
        bool      m_defaultData;
        bool      m_isDefaultDataValid;
    protected :
    public :
};

class AttributeBoolVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                 validate                  () const;

    protected :
    public :
                                     AttributeBoolVector       (const vector<bool> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeBoolVector       (const vector<bool> &data, const vector<bool> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeBoolVector       (vector<bool> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeBoolVector       (vector<bool> *pData, const vector<bool> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                     AttributeBoolVector       (const AttributeBoolVector &attribute);
        virtual                     ~AttributeBoolVector       ();
                AttributeBoolVector &operator =                (const AttributeBoolVector &attribute);
                vector<bool>         getValue                  () const;
                void                 setValue                  (const vector<bool> &data);
                bool                 isCurrentValueSameAsDefault    () const;
                vector<bool>         getDefaultData            () const;
                bool                 getIsDefaultDataValidFlag () const;
        virtual string               getSqlType                ();
        virtual void                 setupOrm                  (OrmTable *pOrmTable);
        virtual string               getSqlForCreate           ();
        virtual void                 getSqlForInsert           (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                 getSqlForUpdate           (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                 getSqlForSelect           (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                 toString                  (string &valueString);
        virtual void                 fromString                (const string &valueString);
        virtual void                *getPData                  ();
        virtual void                 setValue                  (const void *pData);
        virtual Attribute           *clone                     ();
        virtual void                 setDefaultValue           ();
        static  map<string, string>     getSupportedConversions         ();
        virtual void                 getCValue                 (WaveCValue *pCValue);
        virtual void                 addAttributeToVector      (Attribute *attribute);
        virtual void                 deleteAttributeFromVector (Attribute *attribute);
    // Now the data members

    private :
        vector<bool> *m_pData;
        vector<bool>  m_defaultData;
        bool          m_isDefaultDataValid;
    protected :
    public :
};

class AttributeString : public Attribute
{
    private :
        virtual bool                validate                        () const;
        virtual bool                isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                    AttributeString                 (const string &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeString                 (const string &data, const bool &defaultFlag, const string &defaultString, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeString                 (string *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeString                 (string *pData, const bool &defaultFlag, const string &defaultString, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeString                 (const AttributeString &attribute);
        virtual                    ~AttributeString                 ();
                AttributeString    &operator =                      (const AttributeString &attribute);
                string              getValue                        () const;
                void                setValue                        (const string &data);
                bool                isCurrentValueSameAsDefault     () const;
                string              getDefaultString                () const;
                bool                getIsDefaultStringValidFlag     () const;
        virtual string              getSqlType                      ();
        virtual void                setupOrm                        (OrmTable *pOrmTable);
        virtual string              getSqlForCreate                 ();
        virtual void                getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                toString                        (string &valueString);
        virtual void                fromString                      (const string &valueString);
        virtual void               *getPData                        ();
        virtual void                setValue                        (const void *pData);
        virtual Attribute          *clone                           ();
        static  map<string, string>     getSupportedConversions     ();
        virtual void                setDefaultValue                 ();
                void                toEscapedString                 (string &valueString);
        virtual void                getCValue                       (WaveCValue *pCValue);

    // Now the data members

    private :
        string *m_pData;
        string  m_defaultString;
        bool    m_isDefaultStringValid;

    protected :
    public :
};

class AttributeUnion : public Attribute
{
    private :
        virtual bool            validate                  () const;

    protected :
    public :
                                AttributeUnion            (const string &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUnion            (const string &data, const bool &defaultFlag, const string &defaultUnion, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUnion            (string *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUnion            (string *pData, const bool &defaultFlag, const string &defaultUnion, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeUnion            (const AttributeUnion &attribute);
        virtual                ~AttributeUnion            ();
                AttributeUnion &operator =                (const AttributeUnion &attribute);
                string          getValue                  () const;
                void            setValue                  (const string &data);
                bool            isCurrentValueSameAsDefault     () const;
                string          getDefaultData            () const;
                bool            getIsDefaultDataValidFlag () const;
        virtual string          getSqlType                ();
        virtual void            setupOrm                  (OrmTable *pOrmTable);
        virtual string          getSqlForCreate           ();
        virtual void            getSqlForInsert           (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate           (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect           (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                  (string &valueString);
        virtual void            fromString                (const string &valueString);
        virtual void           *getPData                  ();
        virtual void            setValue                  (const void *pData);
        virtual Attribute      *clone                     ();
        virtual void            setDefaultValue           ();

                void            toEscapedString           (string &valueString);
        static  map<string, string>     getSupportedConversions         ();
        virtual void            getCValue                       (WaveCValue *pCValue);

    // Now the data members

    private :
        string *m_pData;
        string  m_defaultData;
        bool    m_isDefaultDataValid;
    protected :
    public :
};

class AttributeChar : public Attribute
{
    private :
        virtual bool            validate                        () const;
        virtual bool            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                AttributeChar                   (const char &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeChar                   (const char &data, const char &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeChar                   (char *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeChar                   (char *pData, const char &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                AttributeChar                   (const AttributeChar &attribute);
        virtual                ~AttributeChar                   ();
                AttributeChar  &operator =                      (const AttributeChar &attribute);
                char            getValue                        () const;
                void            setValue                        (const char &data);
                bool            isCurrentValueSameAsDefault     () const;
                char            getDefaultData                  () const;
                bool            getIsDefaultDataValidFlag       () const;
        virtual string          getSqlType                      ();
        virtual void            setupOrm                        (OrmTable *pOrmTable);
        virtual string          getSqlForCreate                 ();
        virtual void            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void            toString                        (string &valueString);
        virtual void            fromString                      (const string &valueString);
        virtual ResourceId      loadFromPlainString             (const string &valueString);
        virtual void           *getPData                        ();
        virtual void            setValue                        (const void *pData);
        virtual Attribute      *clone                           ();
        static  map<string, string>     getSupportedConversions ();
        virtual void            setDefaultValue                 ();
        virtual void            getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        char *m_pData;
        char  m_defaultData;
        bool  m_isDefaultDataValid;

    protected :
    public :
};

class AttributeStringVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeStringVector           (const vector<string> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeStringVector           (const vector<string> &data, const vector<string> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeStringVector           (vector<string> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeStringVector           (vector<string> *pData, const vector<string> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeStringVector           (const AttributeStringVector &attribute);
        virtual                        ~AttributeStringVector           ();
                AttributeStringVector  &operator =                      (const AttributeStringVector &attribute);
                vector<string>          getValue                        () const;
                void                    setValue                        (const vector<string> &m_data);
                bool                    isCurrentValueSameAsDefault     () const;
                vector<string>          getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       () const;
        virtual string                  getSqlType                      ();
        virtual void                    setupOrm                        (OrmTable *pOrmTable);
        virtual string                  getSqlForCreate                 ();
        virtual void                    getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                    getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                    getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                    toString                        (string &valueString);
        virtual void                    fromString                      (const string &valueString);
        virtual void                    getPlainString                  (string &valueString);
        virtual void                   *getPData                        ();
        virtual void                    setValue                        (const void *pData);
        virtual Attribute              *clone                           ();
        static  map<string, string>     getSupportedConversions         ();
        virtual void                    setDefaultValue                 ();
                void                    toEscapedString                 (string &valueString);
        virtual void                    getCValue                       (WaveCValue *pCValue);
        virtual void                    addAttributeToVector            (Attribute *attribute);
        virtual void                    deleteAttributeFromVector       (Attribute *attribute);

    // Now the data members

    private :
        vector<string> *m_pData;
        vector<string>  m_defaultData;
        bool            m_isDefaultDataValid;
    protected :
    public :
};

class AttributeObjectId : public Attribute
{
    private :
        virtual bool                validate                        () const;

    protected :
    public :
                                    AttributeObjectId               (const ObjectId &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeObjectId               (const ObjectId &data, const ObjectId &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeObjectId               (ObjectId *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeObjectId               (ObjectId *pData, const ObjectId &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                    AttributeObjectId               (const AttributeObjectId &attribute);
        virtual                    ~AttributeObjectId               ();
                AttributeObjectId  &operator =                      (const AttributeObjectId &attribute);
                ObjectId            getValue                        () const;
                void                setValue                        (const ObjectId &data);
                bool                isCurrentValueSameAsDefault     () const;
                ObjectId            getDefaultData                  () const;
                bool                getIsDefaultDataValidFlag       () const;
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
        static  map<string, string>     getSupportedConversions     ();
        virtual void                setDefaultValue                 ();
        virtual void                loadFromPostgresQueryResult     (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        virtual void                getCValue                       (WaveCValue *pCValue);

    // Now the data members

    private :
    protected :
        ObjectId *m_pData;
        ObjectId  m_defaultData;
        bool      m_isDefaultDataValid;

    public :
};

class AttributeObjectIdAssociation : public AttributeObjectId
{
    private :
        virtual bool                            isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                                AttributeObjectIdAssociation    (const ObjectId &data, const string &attributeName, const string &associatedTo, const bool &canBeEmpty = false, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                AttributeObjectIdAssociation    (ObjectId *pData, const string &attributeName, const string &associatedTo, const bool &canBeEmpty = false, const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                AttributeObjectIdAssociation    (const AttributeObjectIdAssociation &attribute);
        virtual                                ~AttributeObjectIdAssociation    ();
                AttributeObjectIdAssociation   &operator =                      (const AttributeObjectIdAssociation &attribute);
        virtual string                          getSqlType                      ();
        virtual void                            setupOrm                        (OrmTable *pOrmTable);
        virtual string                          getSqlForCreate                 ();
        virtual void                            getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                            getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                            getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual Attribute                      *clone                           ();
        virtual string                          getAssociatedTo                 () const;
        virtual bool                            getCanBeEmpty                   () const;
        virtual void                            getCValue                       (WaveCValue *pCValue);

    // Now the data members

    private :
        string m_associatedTo;
        bool   m_canBeEmpty;

    protected :
    public :
};

typedef AttributeObjectIdAssociation AttributeObjectIdAggregation;

class AttributeObjectIdVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                     validate                    () const;

    protected :
    public :
                                         AttributeObjectIdVector     (const vector<ObjectId> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                         AttributeObjectIdVector     (const vector<ObjectId> &data, const vector<ObjectId> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                         AttributeObjectIdVector     (vector<ObjectId> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                         AttributeObjectIdVector     (vector<ObjectId> *pData, const vector<ObjectId> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                         AttributeObjectIdVector     (const AttributeObjectIdVector &attribute);
        virtual                         ~AttributeObjectIdVector     ();
                AttributeObjectIdVector &operator =                  (const AttributeObjectIdVector &attribute);
                vector<ObjectId>         getValue                    () const;
                void                     setValue                    (const vector<ObjectId> &data);
                bool                     isCurrentValueSameAsDefault () const;
                vector<ObjectId>         getDefaultData              () const;
                bool                     getIsDefaultDataValidFlag   () const;
        virtual string                   getSqlType                  ();
        virtual void                     setupOrm                    (OrmTable *pOrmTable);
        virtual string                   getSqlForCreate             ();
        virtual void                     getSqlForInsert             (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                     getSqlForUpdate             (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                     getSqlForSelect             (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                     toString                    (string &valueString);
        virtual void                     fromString                  (const string &valueString);
        virtual void                    *getPData                    ();
        virtual void                     setValue                    (const void *pData);
        virtual Attribute               *clone                       ();
        static  map<string, string>     getSupportedConversions      ();
        virtual void                     setDefaultValue             ();
        virtual void                     getCValue                   (WaveCValue *pCValue);
        virtual void                     addAttributeToVector        (Attribute *attribute);
        virtual void                     deleteAttributeFromVector   (Attribute *attribute);
    // Now the data members

    private :
    protected :
        vector<ObjectId> *m_pData;
        vector<ObjectId>  m_defaultData;
        bool              m_isDefaultDataValid;

    public :
};

class AttributeObjectIdVectorAssociation : public AttributeObjectIdVector
{
    private :
    protected :
    public :
                                                    AttributeObjectIdVectorAssociation    (const vector<ObjectId> &data, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &associatedTo, const UI32 &attributeUserTag = 0, const bool &isOperational = false, const bool &outputAsString = false);
                                                    AttributeObjectIdVectorAssociation    (vector<ObjectId> *pData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &associatedTo, const UI32 &attributeUserTag = 0, const bool &isOperational = false, const bool &outputAsString = false);
                                                    AttributeObjectIdVectorAssociation    (const AttributeObjectIdVectorAssociation &attribute);
        virtual                                    ~AttributeObjectIdVectorAssociation    ();
                AttributeObjectIdVectorAssociation &operator =                            (const AttributeObjectIdVectorAssociation &attribute);
        virtual string                              getSqlType                            ();
        virtual void                                setupOrm                              (OrmTable *pOrmTable);
        virtual string                              getSqlForCreate                       ();
        virtual void                                getSqlForInsert                       (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                                getSqlForUpdate                       (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                                getSqlForSelect                       (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
                void                                getSqlForSelect2                      (string &associationTableName, string &whereClause);
                bool                                getOutputAsString                     (void) const;

        virtual void                                loadFromPostgresQueryResult           (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);
        virtual void                                loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager = NULL);

        virtual Attribute                          *clone                                 ();
        virtual void                                getCValue                             (WaveCValue *pCValue);
                string                              getAssociatedTo                       () const;

        virtual void                                storeRelatedObjectIdVector            (const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds);

    // Now the data members

    private :
        string   m_parent;
        ObjectId m_parentObjectId;
        string   m_associatedTo;
        string   m_associationTableName;
        bool     m_outputAsString;

    protected :
    public :
};

typedef AttributeObjectIdVectorAssociation AttributeObjectIdVectorAggregation;

class AttributeWorldWideName : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeWorldWideName          (const WorldWideName &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeWorldWideName          (const WorldWideName &data, const WorldWideName &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeWorldWideName          (WorldWideName *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeWorldWideName          (WorldWideName *pData, const WorldWideName &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeWorldWideName          (const AttributeWorldWideName &attribute);
        virtual                        ~AttributeWorldWideName          ();
                AttributeWorldWideName &operator =                      (const AttributeWorldWideName &attribute);
                WorldWideName           getValue                        () const;
                bool                    isCurrentValueSameAsDefault     () const;
                WorldWideName           getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       () const;
                void                    setValue                        (const WorldWideName &data);
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
        WorldWideName *m_pData;
        WorldWideName  m_defaultData;
        bool           m_isDefaultDataValid;

    protected :
    public :
};

class AttributeWorldWideNameVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                          validate                     () const;

    protected :
    public :
                                              AttributeWorldWideNameVector (const vector<WorldWideName> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeWorldWideNameVector (const vector<WorldWideName> &data, const vector<WorldWideName> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeWorldWideNameVector (vector<WorldWideName> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeWorldWideNameVector (vector<WorldWideName> *pData, const vector<WorldWideName> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                              AttributeWorldWideNameVector (const AttributeWorldWideNameVector &attribute);
        virtual                              ~AttributeWorldWideNameVector ();
                AttributeWorldWideNameVector &operator =                   (const AttributeWorldWideNameVector &attribute);
                vector<WorldWideName>         getValue                     () const;
                void                          setValue                     (const vector<WorldWideName> &data);
                bool                          isCurrentValueSameAsDefault  () const;
                vector<WorldWideName>         getDefaultData               () const;
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
        vector<WorldWideName> *m_pData;
        vector<WorldWideName>  m_defaultData;
        bool                   m_isDefaultDataValid;

    protected :
    public :
};

class AttributeMacAddress : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeMacAddress             (const MacAddress &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress             (const MacAddress &data, const MacAddress &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress             (MacAddress *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress             (MacAddress *pData, const MacAddress &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress             (const AttributeMacAddress &attribute);
        virtual                        ~AttributeMacAddress             ();
                AttributeMacAddress    &operator =                      (const AttributeMacAddress &attribute);
                MacAddress              getValue                        () const;
                void                    setValue                        (const MacAddress &data);
                bool                    isCurrentValueSameAsDefault     () const;
                MacAddress              getDefaultData                  () const;
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
        MacAddress *m_pData;
        MacAddress  m_defaultData;
        bool        m_isDefaultDataValid;
    protected :
    public :
};

class AttributeMacAddressVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                        validate                    () const;

    protected :
    public :
                                            AttributeMacAddressVector   (const vector<MacAddress> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddressVector   (const vector<MacAddress> &data, const vector<MacAddress> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddressVector   (vector<MacAddress> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddressVector   (vector<MacAddress> *pData, const vector<MacAddress> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddressVector   (const AttributeMacAddressVector &attribute);
        virtual                            ~AttributeMacAddressVector   ();
                AttributeMacAddressVector  &operator =                  (const AttributeMacAddressVector &attribute);
                vector<MacAddress>          getValue                    () const;
                void                        setValue                    (const vector<MacAddress> &data);
                bool                        isCurrentValueSameAsDefault () const;
                vector<MacAddress>          getDefaultData              () const;
                bool                        getIsDefaultDataValidFlag   () const;
        virtual string                      getSqlType                  ();
        virtual void                        setupOrm                    (OrmTable *pOrmTable);
        virtual string                      getSqlForCreate             ();
        virtual void                        getSqlForInsert             (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                        getSqlForUpdate             (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                        getSqlForSelect             (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                        toString                    (string &valueString);
        virtual void                        fromString                  (const string &valueString);
        virtual void                       *getPData                    ();
        virtual void                        setValue                    (const void *pData);
        virtual Attribute                  *clone                       ();
        static  map<string, string>         getSupportedConversions     ();
        virtual void                        setDefaultValue             ();
        virtual void                        getCValue                   (WaveCValue *pCValue);
        virtual void                        addAttributeToVector        (Attribute *attribute);
        virtual void                        deleteAttributeFromVector   (Attribute *attribute);
    // Now the data members

    private :
        vector<MacAddress> *m_pData;
        vector<MacAddress>  m_defaultData;
        bool                m_isDefaultDataValid;

    protected :
    public :
};

class AttributeMacAddress2 : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeMacAddress2            (const MacAddress2 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress2            (const MacAddress2 &data, const MacAddress2 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress2            (MacAddress2 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress2            (MacAddress2 *pData, const MacAddress2 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeMacAddress2            (const AttributeMacAddress2 &attribute);
        virtual                        ~AttributeMacAddress2            ();
                AttributeMacAddress2   &operator =                      (const AttributeMacAddress2 &attribute);
                MacAddress2             getValue                        () const;
                void                    setValue                        (const MacAddress2 &data);
                bool                    isCurrentValueSameAsDefault     () const;
                MacAddress2             getDefaultData                  () const;
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
        MacAddress2 *m_pData;
        MacAddress2  m_defaultData;
        bool         m_isDefaultDataValid;

    protected :
    public :
};

class AttributeMacAddress2Vector : public Attribute, public AttributeVector
{
    private :
        virtual bool                        validate                    () const;

    protected :
    public :
                                            AttributeMacAddress2Vector  (const vector<MacAddress2> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddress2Vector  (const vector<MacAddress2> &data, const vector<MacAddress2> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddress2Vector  (vector<MacAddress2> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddress2Vector  (vector<MacAddress2> *pData, const vector<MacAddress2> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeMacAddress2Vector  (const AttributeMacAddress2Vector &attribute);
        virtual                            ~AttributeMacAddress2Vector  ();
                AttributeMacAddress2Vector &operator =                  (const AttributeMacAddress2Vector &attribute);
                vector<MacAddress2>         getValue                    () const;
                void                        setValue                    (const vector<MacAddress2> &data);
                bool                        isCurrentValueSameAsDefault () const;
                vector<MacAddress2>         getDefaultData              () const;
                bool                        getIsDefaultDataValidFlag   () const;
        virtual string                      getSqlType                  ();
        virtual void                        setupOrm                    (OrmTable *pOrmTable);
        virtual string                      getSqlForCreate             ();
        virtual void                        getSqlForInsert             (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                        getSqlForUpdate             (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                        getSqlForSelect             (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                        toString                    (string &valueString);
        virtual void                        fromString                  (const string &valueString);
        virtual void                       *getPData                    ();
        virtual void                        setValue                    (const void *pData);
        virtual Attribute                  *clone                       ();
        static  map<string, string>         getSupportedConversions     ();
        virtual void                        setDefaultValue             ();
        virtual void                        getCValue                   (WaveCValue *pCValue);
        virtual void                        addAttributeToVector        (Attribute *attribute);
        virtual void                        deleteAttributeFromVector   (Attribute *attribute);
    // Now the data members

    private :
        vector<MacAddress2> *m_pData;
        vector<MacAddress2> m_defaultData;
        bool                m_isDefaultDataValid;

    protected :
    public :
};

class AttributeIpV4Address : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeIpV4Address            (const IpV4Address &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV4Address            (const IpV4Address &data, const IpV4Address &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV4Address            (IpV4Address *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV4Address            (IpV4Address *pData, const IpV4Address &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV4Address            (const AttributeIpV4Address &attribute);
        virtual                        ~AttributeIpV4Address            ();
                AttributeIpV4Address   &operator =                      (const AttributeIpV4Address &attribute);
                IpV4Address             getValue                        () const;
                void                    setValue                        (const IpV4Address &data);
                bool                    isCurrentValueSameAsDefault     () const;    
                IpV4Address             getDefaultData                  () const;
                bool                    getIsDefaultDataValidFlag       (void) const;
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
        IpV4Address *m_pData;
        IpV4Address  m_defaultData;
        bool         m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

class AttributeIpV4AddressVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                        validate                   () const;

    protected :
    public :
                                            AttributeIpV4AddressVector (const vector<IpV4Address> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV4AddressVector (const vector<IpV4Address> &data, const vector<IpV4Address> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV4AddressVector (vector<IpV4Address> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV4AddressVector (vector<IpV4Address> *pData, const vector<IpV4Address> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV4AddressVector (const AttributeIpV4AddressVector &attribute);
        virtual                            ~AttributeIpV4AddressVector ();
                AttributeIpV4AddressVector &operator =                 (const AttributeIpV4AddressVector &attribute);
                vector<IpV4Address>         getValue                   () const;
                void                        setValue                   (const vector<IpV4Address> &data);
                bool                        isCurrentValueSameAsDefault() const;
                vector<IpV4Address>         getDefaultData             () const;
                bool                        getIsDefaultDataValidFlag  () const;
        virtual string                      getSqlType                 ();
        virtual void                        setupOrm                   (OrmTable *pOrmTable);
        virtual string                      getSqlForCreate            ();
        virtual void                        getSqlForInsert            (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                        getSqlForUpdate            (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                        getSqlForSelect            (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                        toString                   (string &valueString);
        virtual void                        fromString                 (const string &valueString);
        virtual void                       *getPData                   ();
        virtual void                        setValue                   (const void *pData);
        virtual Attribute                  *clone                      ();
        static  map<string, string>         getSupportedConversions    ();
        virtual void                        setDefaultValue            ();
        virtual void                        getCValue                  (WaveCValue *pCValue);
        virtual void                        addAttributeToVector       (Attribute *attribute);
        virtual void                        deleteAttributeFromVector  (Attribute *attribute);
        virtual void                        getPlainString             (string &valueString);
    // Now the data members

    private :
        vector<IpV4Address> *m_pData;
        vector<IpV4Address>  m_defaultData;
        bool                 m_isDefaultDataValid;

    protected :
    public :
};

class AttributeIpV6Address : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeIpV6Address            (const IpV6Address &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV6Address            (const IpV6Address &data, const IpV6Address &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV6Address            (IpV6Address *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV6Address            (IpV6Address *pData, const IpV6Address &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpV6Address            (const AttributeIpV6Address &attribute);
        virtual                        ~AttributeIpV6Address            ();
                AttributeIpV6Address   &operator =                      (const AttributeIpV6Address &attribute);
                IpV6Address             getValue                        () const;
                void                    setValue                        (const IpV6Address &data);
                bool                    isCurrentValueSameAsDefault     () const;    
                IpV6Address             getDefaultData                  () const;
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
        IpV6Address *m_pData;
        IpV6Address m_defaultData;
        bool        m_isDefaultDataValid;
    protected :
    public :
};

class AttributeIpV6AddressVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                        validate                   () const;

    protected :
    public :
                                            AttributeIpV6AddressVector (const vector<IpV6Address> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV6AddressVector (const vector<IpV6Address> &data, const vector<IpV6Address> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV6AddressVector (vector<IpV6Address> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV6AddressVector (vector<IpV6Address> *pData, const vector<IpV6Address> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpV6AddressVector (const AttributeIpV6AddressVector &attribute);
        virtual                            ~AttributeIpV6AddressVector ();
                AttributeIpV6AddressVector &operator =                 (const AttributeIpV6AddressVector &attribute);
                vector<IpV6Address>         getValue                   () const;
                void                        setValue                   (const vector<IpV6Address> &data);
                bool                        isCurrentValueSameAsDefault() const;
                vector<IpV6Address>         getDefaultData             () const;
                bool                        getIsDefaultDataValidFlag  () const;
        virtual string                      getSqlType                 ();
        virtual void                        setupOrm                   (OrmTable *pOrmTable);
        virtual string                      getSqlForCreate            ();
        virtual void                        getSqlForInsert            (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                        getSqlForUpdate            (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                        getSqlForSelect            (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                        toString                   (string &valueString);
        virtual void                        fromString                 (const string &valueString);
        virtual void                       *getPData                   ();
        virtual void                        setValue                   (const void *pData);
        virtual Attribute                  *clone                      ();
        static  map<string, string>         getSupportedConversions    ();
        virtual void                        setDefaultValue            ();
        virtual void                        getCValue                  (WaveCValue *pCValue);
        virtual void                        addAttributeToVector       (Attribute *attribute);
        virtual void                        deleteAttributeFromVector  (Attribute *attribute);
        virtual void                        getPlainString             (string &valueString);
    // Now the data members

    private :
        vector<IpV6Address> *m_pData;
        vector<IpV6Address>  m_defaultData;
        bool                 m_isDefaultDataValid;

    protected :
    public :
};

}

#endif // ATTRIBUTES_H
