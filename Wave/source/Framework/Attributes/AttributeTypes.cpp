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

#include "Framework/Attributes/AttributeTypes.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

AttributeType AttributeType::AttributeTypeUI32                   (1,  WAVE_ATTRIBUTE_TYPE_UI32);
AttributeType AttributeType::AttributeTypeWaveMessageType        (1,  WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE);
AttributeType AttributeType::AttributeTypeWaveMessagePriority    (1,  WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY);
AttributeType AttributeType::AttributeTypePrismServiceId         (1,  WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID);
AttributeType AttributeType::AttributeTypeSerializableObjectType (1,  WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE);
AttributeType AttributeType::AttributeTypeLocationId             (1,  WAVE_ATTRIBUTE_TYPE_LOCATIONID);
AttributeType AttributeType::AttributeTypeUI32Vector             (2,  WAVE_ATTRIBUTE_TYPE_UI32_VECTOR);
AttributeType AttributeType::AttributeTypeUI64                   (3,  WAVE_ATTRIBUTE_TYPE_UI64);
AttributeType AttributeType::AttributeTypeUI64Vector             (4,  WAVE_ATTRIBUTE_TYPE_UI64_VECTOR);
AttributeType AttributeType::AttributeTypeSI64                   (5,  WAVE_ATTRIBUTE_TYPE_SI64);
AttributeType AttributeType::AttributeTypeSI64Vector             (6,  WAVE_ATTRIBUTE_TYPE_SI64_VECTOR);
AttributeType AttributeType::AttributeTypeSI32                   (7,  WAVE_ATTRIBUTE_TYPE_SI32);
AttributeType AttributeType::AttributeTypeSI32Vector             (8,  WAVE_ATTRIBUTE_TYPE_SI32_VECTOR);
AttributeType AttributeType::AttributeTypeUI16                   (9,  WAVE_ATTRIBUTE_TYPE_UI16);
AttributeType AttributeType::AttributeTypeUI16Vector             (10, WAVE_ATTRIBUTE_TYPE_UI16_VECTOR);
AttributeType AttributeType::AttributeTypeSI16                   (11, WAVE_ATTRIBUTE_TYPE_SI16);
AttributeType AttributeType::AttributeTypeSI16Vector             (12, WAVE_ATTRIBUTE_TYPE_SI16_VECTOR);
AttributeType AttributeType::AttributeTypeUI8                    (13, WAVE_ATTRIBUTE_TYPE_UI8);
AttributeType AttributeType::AttributeTypeUI8Vector              (14, WAVE_ATTRIBUTE_TYPE_UI8_VECTOR);
AttributeType AttributeType::AttributeTypeSI8                    (15, WAVE_ATTRIBUTE_TYPE_SI8);
AttributeType AttributeType::AttributeTypeSI8Vector              (16, WAVE_ATTRIBUTE_TYPE_SI8_VECTOR);
AttributeType AttributeType::AttributeTypeObjectId               (17, WAVE_ATTRIBUTE_TYPE_OBJECTID);
AttributeType AttributeType::AttributeTypeObjectIdVector         (18, WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR);
AttributeType AttributeType::AttributeTypeComposition            (19, WAVE_ATTRIBUTE_TYPE_COMPOSITION);
AttributeType AttributeType::AttributeTypeCompositionVector      (20, WAVE_ATTRIBUTE_TYPE_COMPOSITION_VECTOR);
AttributeType AttributeType::AttributeTypeBool                   (21, WAVE_ATTRIBUTE_TYPE_BOOL);
AttributeType AttributeType::AttributeTypeBoolVector             (22, WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR);
AttributeType AttributeType::AttributeTypeChar                   (23, WAVE_ATTRIBUTE_TYPE_CHAR);
AttributeType AttributeType::AttributeTypeString                 (24, WAVE_ATTRIBUTE_TYPE_STRING);
AttributeType AttributeType::AttributeTypeStringVector           (25, WAVE_ATTRIBUTE_TYPE_STRING_VECTOR);
AttributeType AttributeType::AttributeTypeWorldWideName          (26, WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME);
AttributeType AttributeType::AttributeTypeWorldWideNameVector    (27, WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR);
AttributeType AttributeType::AttributeTypeMacAddress             (28, WAVE_ATTRIBUTE_TYPE_MACADDRESS);
AttributeType AttributeType::AttributeTypeMacAddressVector       (29, WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR);
AttributeType AttributeType::AttributeTypeIpV4Address            (30, WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS);
AttributeType AttributeType::AttributeTypeIpV4AddressVector      (31, WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR);
AttributeType AttributeType::AttributeTypeIpV6Address            (32, WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS);
AttributeType AttributeType::AttributeTypeIpV6AddressVector      (33, WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR);
AttributeType AttributeType::AttributeTypeEnum			         (34, WAVE_ATTRIBUTE_TYPE_ENUM);
AttributeType AttributeType::AttributeTypeDate                   (35, WAVE_ATTRIBUTE_TYPE_DATE);
AttributeType AttributeType::AttributeTypeDateVector             (36, WAVE_ATTRIBUTE_TYPE_DATE_VECTOR);
AttributeType AttributeType::AttributeTypeTime                   (37, WAVE_ATTRIBUTE_TYPE_TIME);
AttributeType AttributeType::AttributeTypeTimeVector             (38, WAVE_ATTRIBUTE_TYPE_TIME_VECTOR);
AttributeType AttributeType::AttributeTypeDateTime               (39, WAVE_ATTRIBUTE_TYPE_DATETIME);
AttributeType AttributeType::AttributeTypeDateTimeVector         (40, WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR);
AttributeType AttributeType::AttributeTypeBitMap                 (41, WAVE_ATTRIBUTE_TYPE_BITMAP);
AttributeType AttributeType::AttributeTypeDecimal64              (42, WAVE_ATTRIBUTE_TYPE_DECIMAL64);
AttributeType AttributeType::AttributeTypeDecimal64Vector        (43, WAVE_ATTRIBUTE_TYPE_DECIMAL64_VECTOR);
AttributeType AttributeType::AttributeTypeUnion                  (44, WAVE_ATTRIBUTE_TYPE_UNION);
AttributeType AttributeType::AttributeTypeIpV4AddressNetworkMask (45, WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK);
AttributeType AttributeType::AttributeTypeIpV6AddressNetworkMask (46, WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK);
AttributeType AttributeType::AttributeTypeSnmpObjectId           (47, WAVE_ATTRIBUTE_TYPE_SNMPOBJECTID);
AttributeType AttributeType::AttributeTypeUI32Range              (48, WAVE_ATTRIBUTE_TYPE_UI32_RANGE);
AttributeType AttributeType::AttributeTypeIpVxAddress            (49, WAVE_ATTRIBUTE_TYPE_IPVXADDRESS);
AttributeType AttributeType::AttributeTypeIpVxAddressVector      (50, WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR);
AttributeType AttributeType::AttributeTypeLargeObject            (51, WAVE_ATTRIBUTE_TYPE_LARGEOBJECT);
AttributeType AttributeType::AttributeTypeMACRange               (52, WAVE_ATTRIBUTE_TYPE_MAC_RANGE);
AttributeType AttributeType::AttributeTypeUI32UC                 (53, WAVE_ATTRIBUTE_TYPE_UI32_UC);
AttributeType AttributeType::AttributeTypeStringUC               (54, WAVE_ATTRIBUTE_TYPE_STRING_UC);
AttributeType AttributeType::AttributeTypeBoolUC                 (55, WAVE_ATTRIBUTE_TYPE_BOOL_UC);
AttributeType AttributeType::AttributeTypeEnumUC                 (56, WAVE_ATTRIBUTE_TYPE_ENUM_UC);
AttributeType AttributeType::AttributeTypeMacAddress2            (57, WAVE_ATTRIBUTE_TYPE_MACADDRESS2);
AttributeType AttributeType::AttributeTypeMacAddress2Vector      (58, WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR);
AttributeType AttributeType::AttributeTypeResourceId             (59, WAVE_ATTRIBUTE_TYPE_RESOURCEID);
AttributeType AttributeType::AttributeTypeResourceEnum           (59, WAVE_ATTRIBUTE_TYPE_RESOURCEENUM);
AttributeType AttributeType::AttributeTypeResourceIdVector       (60, WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR);
AttributeType AttributeType::AttributeTypeResourceEnumVector     (60, WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR);
AttributeType AttributeType::AttributeTypeIpV4AddressUC          (61, WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC);
AttributeType AttributeType::AttributeTypeUI32VectorUC           (62, WAVE_ATTRIBUTE_TYPE_UI32_VECTOR_UC);
AttributeType AttributeType::AttributeTypeStringVectorUC         (63, WAVE_ATTRIBUTE_TYPE_STRING_UC);
AttributeType AttributeType::AttributeTypeHostUC                 (64, WAVE_ATTRIBUTE_TYPE_HOST_UC);
AttributeType AttributeType::AttributeTypeUI64UC                 (65, WAVE_ATTRIBUTE_TYPE_UI64_UC);
AttributeType AttributeType::AttributeTypeSqlBetween             (66, WAVE_ATTRIBUTE_TYPE_SQL_BETWEEN);
AttributeType AttributeType::AttributeTypeSqlIn                  (67, WAVE_ATTRIBUTE_TYPE_SQL_IN);
AttributeType AttributeType::AttributeTypeStringVectorVector     (68, WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_VECTOR);
AttributeType AttributeType::AttributeTypeSerializableObject     (69, WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT);
AttributeType AttributeType::AttributeTypeUUID                   (70, WAVE_ATTRIBUTE_TYPE_UUID);
AttributeType AttributeType::AttributeTypeSqlExists              (71, WAVE_ATTRIBUTE_TYPE_SQL_EXISTS);
AttributeType AttributeType::AttributeTypeUri                    (72, WAVE_ATTRIBUTE_TYPE_URI);

AttributeType::AttributeType ()
    : m_attributeTypeIndicator (0)
{
}

AttributeType::AttributeType (UI32 attributeTypeIndicator, ResourceId attributeTypeResourceId)
    :   m_attributeTypeIndicator    (attributeTypeIndicator),
        m_attributeTypeResourceId   (attributeTypeResourceId)
{
}

AttributeType::AttributeType (const AttributeType &attributeType)
    :   m_attributeTypeIndicator    (attributeType.m_attributeTypeIndicator),
        m_attributeTypeResourceId   (attributeType.m_attributeTypeResourceId)
{
}

UI32 AttributeType::getAttributeTypeIndicator   () const
{
    return (m_attributeTypeIndicator);
}

ResourceId AttributeType::getAttributeTypeResourceId  () const
{
    return (m_attributeTypeResourceId);
}

AttributeType &AttributeType::operator = (const AttributeType &attributeType)
{
    m_attributeTypeIndicator    = attributeType.m_attributeTypeIndicator;
    m_attributeTypeResourceId   = attributeType.m_attributeTypeResourceId;

    return (*this);
}

bool AttributeType::operator == (const AttributeType &attributeType) const
{
    return ((m_attributeTypeIndicator == attributeType.m_attributeTypeIndicator) &&
            (m_attributeTypeResourceId == attributeType.m_attributeTypeResourceId));
}

bool AttributeType::operator != (const AttributeType &attributeType) const
{
    return ((m_attributeTypeIndicator != attributeType.m_attributeTypeIndicator) ||
            (m_attributeTypeResourceId != attributeType.m_attributeTypeResourceId));
}

}
