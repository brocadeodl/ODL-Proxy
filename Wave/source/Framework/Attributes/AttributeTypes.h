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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ATTRIBUTETYPES_H
#define ATTRIBUTETYPES_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class AttributeType
{
    private :
    protected :
                                AttributeType               (UI32 attributeTypeIndictator, ResourceId attributeTypeResourceId);

    public :
                                AttributeType               ();
                                AttributeType               (const AttributeType &attributeType);

                UI32            getAttributeTypeIndicator   () const;
                ResourceId      getAttributeTypeResourceId  () const;

                AttributeType  &operator =                  (const AttributeType &attributeType);

                bool            operator ==                 (const AttributeType &attributeType) const;
                bool            operator !=                 (const AttributeType &attributeType) const;

    // Now the data types

    private :
        UI32        m_attributeTypeIndicator;
        ResourceId  m_attributeTypeResourceId;

    protected :
    public :
        static AttributeType AttributeTypeUI32;
        static AttributeType AttributeTypeWaveMessageType;
        static AttributeType AttributeTypeWaveMessagePriority;
        static AttributeType AttributeTypePrismServiceId;
        static AttributeType AttributeTypeResourceId;
        static AttributeType AttributeTypeResourceEnum;
        static AttributeType AttributeTypeSerializableObjectType;
        static AttributeType AttributeTypeLocationId;
        static AttributeType AttributeTypeUI32Vector;
        static AttributeType AttributeTypeUI64;
        static AttributeType AttributeTypeUI64Vector;
        static AttributeType AttributeTypeSI32;
        static AttributeType AttributeTypeSI32Vector;
    	static AttributeType AttributeTypeSI64;
	    static AttributeType AttributeTypeSI64Vector;
        static AttributeType AttributeTypeUI16;
        static AttributeType AttributeTypeUI16Vector;
        static AttributeType AttributeTypeSI16;
        static AttributeType AttributeTypeSI16Vector;
        static AttributeType AttributeTypeUI8;
        static AttributeType AttributeTypeUI8Vector;
        static AttributeType AttributeTypeSI8;
        static AttributeType AttributeTypeSI8Vector;
        static AttributeType AttributeTypeObjectId;
        static AttributeType AttributeTypeObjectIdVector;
        static AttributeType AttributeTypeComposition;
        static AttributeType AttributeTypeCompositionVector;
        static AttributeType AttributeTypeBool;
        static AttributeType AttributeTypeBoolVector;
        static AttributeType AttributeTypeChar;
        static AttributeType AttributeTypeString;
        static AttributeType AttributeTypeStringVector;
        static AttributeType AttributeTypeWorldWideName;
        static AttributeType AttributeTypeWorldWideNameVector;
        static AttributeType AttributeTypeMacAddress;
        static AttributeType AttributeTypeMacAddressVector;
        static AttributeType AttributeTypeIpV4Address;
        static AttributeType AttributeTypeIpV4AddressVector;
        static AttributeType AttributeTypeIpV6Address;
        static AttributeType AttributeTypeIpV6AddressVector;
	    static AttributeType AttributeTypeEnum;
	    static AttributeType AttributeTypeBitMap;

        static AttributeType AttributeTypeDate;
        static AttributeType AttributeTypeDateVector;
        static AttributeType AttributeTypeTime;
        static AttributeType AttributeTypeTimeVector;
        static AttributeType AttributeTypeDateTime;
        static AttributeType AttributeTypeDateTimeVector;
        static AttributeType AttributeTypeDecimal64;
        static AttributeType AttributeTypeDecimal64Vector;
        static AttributeType AttributeTypeUnion;

        static AttributeType AttributeTypeIpV4AddressNetworkMask;
        static AttributeType AttributeTypeIpV6AddressNetworkMask;
        static AttributeType AttributeTypeSnmpObjectId;
        static AttributeType AttributeTypeUI32Range;
        static AttributeType AttributeTypeIpVxAddress;
        static AttributeType AttributeTypeIpVxAddressVector;

		static AttributeType AttributeTypeLargeObject;

        static AttributeType AttributeTypeMACRange;

        static AttributeType AttributeTypeUI32UC;
        static AttributeType AttributeTypeStringUC;
        static AttributeType AttributeTypeBoolUC;
        static AttributeType AttributeTypeEnumUC;


        static AttributeType AttributeTypeMacAddress2;
        static AttributeType AttributeTypeMacAddress2Vector;

        static AttributeType AttributeTypeResourceIdVector;
        static AttributeType AttributeTypeResourceEnumVector;


        static AttributeType AttributeTypeIpV4AddressUC;
        static AttributeType AttributeTypeUI32VectorUC;
        static AttributeType AttributeTypeStringVectorUC;

        static AttributeType AttributeTypeHostUC;

        static AttributeType AttributeTypeUI64UC;

        static AttributeType AttributeTypeSqlBetween;
        static AttributeType AttributeTypeSqlIn;

        static AttributeType AttributeTypeStringVectorVector;

        static AttributeType AttributeTypeSerializableObject;

        static AttributeType AttributeTypeUUID;

        static AttributeType AttributeTypeSqlExists;

        static AttributeType AttributeTypeUri;

        // The Start Value for Attribute Type Indicator that can be used out side of Prism Code base

        static const UI32 AttributeTypeIndicatorStartValueForPublicUsage = 0x1 << 16;
};

}

#endif // ATTRIBUTETYPES_H
