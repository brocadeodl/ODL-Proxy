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

#ifndef ATTRIBUTEIPV4ADDRESSNETWORKMASK_H
#define ATTRIBUTEIPV4ADDRESSNETWORKMASK_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

class AttributeIpV4AddressNetworkMask : public Attribute
{
    private :
        virtual bool                                validate                        () const;
        virtual bool                                isConditionOperatorSupported    (AttributeConditionOperator attributeConditionOperator);

    protected :
    public :
                                                    AttributeIpV4AddressNetworkMask (const IpV4AddressNetworkMask &data, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                    AttributeIpV4AddressNetworkMask (const IpV4AddressNetworkMask &data, const IpV4AddressNetworkMask &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                    AttributeIpV4AddressNetworkMask (IpV4AddressNetworkMask *pData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                    AttributeIpV4AddressNetworkMask (IpV4AddressNetworkMask *pData, const IpV4AddressNetworkMask &defaultData, const string &attributeName = "", const UI32 &attributeUserTag = 0, const bool &isOperational = false);
                                                    AttributeIpV4AddressNetworkMask (const AttributeIpV4AddressNetworkMask &attribute);
        virtual                                    ~AttributeIpV4AddressNetworkMask ();
                AttributeIpV4AddressNetworkMask    &operator =                      (const AttributeIpV4AddressNetworkMask &attribute);
                IpV4AddressNetworkMask              getValue                        () const;
                void                                getAllValues                    (string &NetworkMask, UI8 *length);
                void                                setValue                        (const IpV4AddressNetworkMask &data);
                bool                                isCurrentValueSameAsDefault     () const;
                IpV4AddressNetworkMask              getDefaultValue                 () const;
                bool                                getIsDefaultDataValidFlag       () const;
        virtual string                              getSqlType                      ();
        virtual void                                setupOrm                        (OrmTable *pOrmTable);
        virtual string                              getSqlForCreate                 ();
        virtual void                                getSqlForInsert                 (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst = false);
        virtual void                                getSqlForUpdate                 (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst = false);
        virtual void                                getSqlForSelect                 (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        virtual void                                toString                        (string &valueString);
        virtual void                                fromString                      (const string &valueString);
        virtual ResourceId                          loadFromPlainString             (const string &valueString);
        virtual void                               *getPData                        ();
        virtual void                                setValue                        (const void *pData);
        virtual Attribute                          *clone                           ();
        static  map<string, string>                 getSupportedConversions         ();
        virtual void                                setDefaultValue                 ();
        virtual void                                getCValue                       (WaveCValue *pCValue);
    // Now the data members

    private :
        IpV4AddressNetworkMask *m_pData;
        IpV4AddressNetworkMask  m_defaultData;
        bool                    m_isDefaultDataValid;

    protected :
    public :
};

}

#endif // ATTRIBUTEIPV4ADDRESSNETWORKMASK_H
