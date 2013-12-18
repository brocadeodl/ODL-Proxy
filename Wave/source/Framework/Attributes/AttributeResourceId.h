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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#ifndef ATTRIBUTERESOURCEID_H
#define ATTRIBUTERESOURCEID_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeVector.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeResourceId : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeResourceId             (const UI32 &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeResourceId             (const UI32 &data, const UI32 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0,const bool &isOperational = false);
                                        AttributeResourceId             (UI32 *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeResourceId             (UI32 *pData, const UI32 &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeResourceId             (const AttributeResourceId &attribute);
        virtual                        ~AttributeResourceId             ();
                AttributeResourceId    &operator =                      (const AttributeResourceId &attribute);
                UI32                    getValue                        () const;
                void                    setValue                        (const UI32 &data);
                bool                    isCurrentValueSameAsDefault     () const;
                UI32                    getDefaultData                  () const;

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
                void                    toEscapedString                 (string &valueString);
        virtual void                    setDefaultValue                 ();
        virtual void                    getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        UI32 *m_pData;
        UI32  m_defaultData;
        bool  m_isDefaultDataValid; // if this is true, then default value is valid

    protected :
    public :
};

typedef AttributeResourceId AttributeResourceEnum;
typedef AttributeResourceId AttributeWaveMessageType;
typedef AttributeResourceId AttributeWaveMessagePriority;
typedef AttributeResourceId AttributeTraceLevel;

class AttributeResourceIdVector : public Attribute, public AttributeVector
{
    private :
        virtual bool                        validate                              () const;

    protected :
    public :
                                            AttributeResourceIdVector             (const vector<UI32> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeResourceIdVector             (const vector<UI32> &data, const vector<UI32> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeResourceIdVector             (vector<UI32> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeResourceIdVector             (vector<UI32> *pData, const vector<UI32> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeResourceIdVector             (const AttributeResourceIdVector &attribute);
        virtual                            ~AttributeResourceIdVector             ();
                AttributeResourceIdVector  &operator =                            (const AttributeResourceIdVector &attribute);
                vector<UI32>                getValue                              () const;
                void                        setValue                              (const vector<UI32> &data);
                bool                        isCurrentValueSameAsDefault           () const;
                bool                        getIsDefaultResourceIdVectorValidFlag () const;
                vector<UI32>                getDefaultResourceIdVector            () const;
        virtual string                      getSqlType                            ();
        virtual void                        setupOrm                              (OrmTable *pOrmTable);
        virtual string                      getSqlForCreate                       ();
        virtual void                        getSqlForInsert                       (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                        getSqlForUpdate                       (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                        getSqlForSelect                       (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                        toString                              (string &valueString);
        virtual void                        fromString                            (const string &valueString);
        virtual void                       *getPData                              ();
        virtual void                        setValue                              (const void *pData);
        virtual Attribute                  *clone                                 ();
                void                        toEscapedString                       (string &valueString);
        virtual void                        setDefaultValue                       ();
        static  map<string, string>         getSupportedConversions               ();
        virtual void                        getCValue                             (WaveCValue *pCValue);
        virtual void                        addAttributeToVector                  (Attribute *attribute);
        virtual void                        deleteAttributeFromVector             (Attribute *attribute);
    // Now the data members

    private :

        vector<UI32> *m_pData;
        vector<UI32>  m_defaultResourceIdVector;
        bool          m_isDefaultResourceIdVectorValid;
    protected :
    public :
};

typedef AttributeResourceIdVector AttributeResourceEnumVector;
typedef AttributeResourceIdVector AttributeWaveMessageTypeVector;
typedef AttributeResourceIdVector AttributeWaveMessagePriorityVector;
typedef AttributeResourceIdVector AttributeTraceLevelVector;

}

#endif //ATTRIBUTERESOURCEID_H
