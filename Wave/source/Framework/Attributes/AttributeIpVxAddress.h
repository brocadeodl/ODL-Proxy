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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#ifndef ATTRIBUTES_IP_VX_ADDRESS_H
#define ATTRIBUTES_IP_VX_ADDRESS_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeVector.h"
#include "Framework/Types/IpVxAddress.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeIpVxAddress : public Attribute
{
    private :
        virtual bool                    validate                        () const;
        virtual bool                    isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                        AttributeIpVxAddress            (const IpVxAddress &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpVxAddress            (const IpVxAddress &data, const IpVxAddress &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpVxAddress            (IpVxAddress *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpVxAddress            (IpVxAddress *pData, const IpVxAddress &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                        AttributeIpVxAddress            (const AttributeIpVxAddress &attribute);
        virtual                        ~AttributeIpVxAddress            ();
                AttributeIpVxAddress   &operator =                      (const AttributeIpVxAddress &attribute);
                IpVxAddress             getValue                        () const;
                void                    setValue                        (const IpVxAddress &data);
                bool                    isCurrentValueSameAsDefault     () const;
                bool                    getIsDefaultDataValidFlag       () const;
                IpVxAddress             getDefaultValue                 () const;
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
        IpVxAddress *m_pData;
        IpVxAddress  m_defaultData;
        bool         m_isDefaultDataValid;

    protected :
    public :
};

class AttributeIpVxAddressVector : public Attribute, public AttributeVector
{
    private :
        virtual bool validate () const;

    protected :
    public :
                                            AttributeIpVxAddressVector (const vector<IpVxAddress> &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpVxAddressVector (const vector<IpVxAddress> &data, const vector<IpVxAddress> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpVxAddressVector (vector<IpVxAddress> *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpVxAddressVector (vector<IpVxAddress> *pData, const vector<IpVxAddress> &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                            AttributeIpVxAddressVector (const AttributeIpVxAddressVector &attribute);
        virtual                            ~AttributeIpVxAddressVector ();
                AttributeIpVxAddressVector &operator =                 (const AttributeIpVxAddressVector &attribute);
                vector<IpVxAddress>         getValue                   () const;
                void                        setValue                   (const vector<IpVxAddress> &data);
                bool                        isCurrentValueSameAsDefault() const;
                vector<IpVxAddress>         getDefaultValue            () const;
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
    // Now the data members

    private :
        vector<IpVxAddress> *m_pData;
        vector<IpVxAddress>  m_defaultData;
        bool                 m_isDefaultDataValid;
    protected :
    public :
};


}
#endif //ATTRIBUTES_IP_VX_ADDRESS_H
