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

#include "Framework/Attributes/Attribute.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
//#include <xercesc/parsers/XercesDOMParser.hpp>
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectRelationalMapping/OrmColumn.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/ObjectRelationalMapping/OrmAssociation.h"
#include "Framework/ObjectRelationalMapping/OrmAggregation.h"
#include "Framework/ObjectRelationalMapping/OrmComposition.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectOperation.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeDate.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Attributes/AttributeDecimal64.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeHostUC.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/Attributes/AttributeLargeObject.h"
#include "Framework/Attributes/AttributeMACRange.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Attributes/AttributeSI64.h"
#include "Framework/Attributes/AttributeSI64Vector.h"
#include "Framework/Attributes/AttributeSnmpObjectId.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeStringVectorUC.h"
#include "Framework/Attributes/AttributeTime.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeUI32VectorUC.h"
#include "Framework/Attributes/AttributeUI64UC.h"

namespace WaveNs
{

Attribute::Attribute (AttributeType &attributeType)
    : m_attributeType    (attributeType),
      m_attributeTag     (0),
      m_isMemoryOwnedByAttribute (false),
      m_isPrimary        (false),
      m_attributeUserTag (0),
      m_isOperational (false)
{
}

Attribute::Attribute (AttributeType &attributeType, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : m_attributeType    (attributeType),
      m_attributeName    (attributeName),
      m_attributeTag     (0),
      m_isMemoryOwnedByAttribute (false),
      m_isPrimary        (false),
      m_attributeUserTag (attributeUserTag),
      m_isOperational    (isOperational)
{
}

Attribute::Attribute (const Attribute &attribute)
    : m_attributeType (attribute.m_attributeType)
{
    m_attributeName    = attribute.m_attributeName;
    m_attributeTag     = attribute.m_attributeTag;
    m_isPrimary        = attribute.m_isPrimary;
    m_attributeUserTag = attribute.m_attributeUserTag;
    m_isOperational        = attribute.m_isOperational;
    m_isMemoryOwnedByAttribute = attribute.m_isMemoryOwnedByAttribute;

}

Attribute &Attribute::operator = (const Attribute &attribute)
{
    m_attributeType    = attribute.m_attributeType;
    m_attributeName    = attribute.m_attributeName;
    m_attributeTag     = attribute.m_attributeTag;
    m_isPrimary        = attribute.m_isPrimary;
    m_attributeUserTag = attribute.m_attributeUserTag;
    m_isMemoryOwnedByAttribute = attribute.m_isMemoryOwnedByAttribute;
    m_isOperational        = attribute.m_isOperational;

    return (*this);
}

Attribute::~Attribute ()
{
}

void Attribute::setIsMemoryOwnedByAttribute (bool isMemoryOwnedByAttribute)
{
    m_isMemoryOwnedByAttribute = isMemoryOwnedByAttribute;
}

bool Attribute::getIsMemoryOwnedByAttribute ()
{
    return (m_isMemoryOwnedByAttribute);
}

AttributeType Attribute::getAttributeType () const
{
    return (m_attributeType);
}

string Attribute::getAttributeTypeString () const
{
    return (FrameworkToolKit::localizeToSourceCodeEnum (m_attributeType.getAttributeTypeResourceId ()));
}

void Attribute::setAttributeType (const AttributeType &attributeType)
{
    m_attributeType = attributeType;
}

UI32 Attribute::getAttributeTag ()
{
    return (m_attributeTag);
}

void Attribute::setAttributeTag (const UI32 &attributeTag)
{
    m_attributeTag = attributeTag;
}

string Attribute::getAttributeName () const
{
    return (m_attributeName);
}

void Attribute::setAttributeName (const string &attributeName) 
{
    m_attributeName = attributeName;
}

UI32 Attribute::getAttributeUserTag () const
{
    return (m_attributeUserTag);
}

void Attribute::setAttributeUserTag (const UI32 &attributeUserTag)
{
    m_attributeUserTag = attributeUserTag;
}

bool Attribute::getIsOperational () const
{
    return (m_isOperational);
}

// Temporarily disabled all Xerces related code.
#if 0
void Attribute::serializeTo (DOMDocument *pDomDocument)
{
    string tempString;

    toString (tempString);

    prismAssert (NULL != pDomDocument, __FILE__, __LINE__);

    DOMElement *pRootElement = pDomDocument->getDocumentElement ();

    prismAssert (NULL != pRootElement, __FILE__, __LINE__);

    char buffer[64];

    sprintf (buffer, "A%lu", (long unsigned int) m_attributeTag);

    XMLCh *pCurrentElementName = XMLString::transcode (buffer);

    DOMElement *pCurrentElement = pDomDocument->createElement (pCurrentElementName);

    prismAssert (NULL != pCurrentElement, __FILE__,__LINE__);

    pRootElement->appendChild (pCurrentElement);

    XMLCh *pCurrentTextNodeName = XMLString::transcode (tempString.c_str ());

    DOMText *pCurrentTextNode = pDomDocument->createTextNode (pCurrentTextNodeName);

    prismAssert (NULL != pCurrentTextNode, __FILE__, __LINE__);

    pCurrentElement->appendChild (pCurrentTextNode);

    XMLString::release (&pCurrentElementName);
    XMLString::release (&pCurrentTextNodeName);
}
#endif

bool Attribute::getIsPrimary ()
{
    return (m_isPrimary);
}

void Attribute::setIsPrimary (const bool &isPrimary)
{
    m_isPrimary = isPrimary;
}

void Attribute::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    SI32 column = PQfnumber (pResult, (getAttributeName()).c_str ());

    if (-1 != column)
    {
        char *pValue = PQgetvalue (pResult, row, column);

        fromString (string (pValue));
    }
}

void Attribute::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
}

void Attribute::updateOrmRelations (const PrismPersistableObject *pPrismPersistableObject)
{
}

WaveManagedObject *Attribute::getComposedManagedObject (const ObjectId &childObjectId)
{
    trace (TRACE_LEVEL_FATAL, "This function should not be called for this Attribute Object");

    prismAssert (false, __FILE__, __LINE__);

	return NULL;
}

vector<WaveManagedObject *>  *Attribute::getComposedManagedObject ()
{
    trace (TRACE_LEVEL_FATAL, "This function should not be called for this Attribute Object");

    prismAssert (false, __FILE__, __LINE__);

    return NULL;
}

bool Attribute::isDeletableForOperation (const WaveManagedObjectOperation &operation)
{
	return true;
}

void Attribute::updateKeyString (const PrismPersistableObject *pPrismPersistableObject)
{
}

void Attribute::getRestRowData (string &restRowData)
{
    toString (restRowData);
}

bool Attribute::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
{
    bool    isSupported   = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL == attributeConditionOperator || WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

Attribute* Attribute::getAttributeFromAttributeType (const ResourceId attributeType)
{

    Attribute *pAttribute = NULL;

    switch (attributeType)
    {
        case WAVE_ATTRIBUTE_TYPE_UI32:
            {
                UI32 temp = 0;
                pAttribute = new AttributeUI32 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI32_VECTOR:
            {
                vector <UI32> temp;
                pAttribute = new AttributeUI32Vector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI64:
            {
                UI64 temp = 0;
                pAttribute = new AttributeUI64 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI64_VECTOR:
            {
                vector <UI64> temp;
                pAttribute = new AttributeUI64Vector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SI32:
            {
                SI32 temp = 0;
                pAttribute = new AttributeSI32 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SI32_VECTOR:
            {
                vector <SI32> temp;
                pAttribute = new AttributeSI32Vector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SI64:
            {
                SI64 temp = 0;
                pAttribute = new AttributeSI64 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SI64_VECTOR:
            {
                vector <SI64> temp;
                pAttribute = new AttributeSI64Vector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI16:
            {
                UI16 temp = 0;
                pAttribute = new AttributeUI16 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SI16:
            {
                SI16 temp = 0;
                pAttribute = new AttributeSI16 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI8:
            {
                UI8 temp = '\0';
                pAttribute = new AttributeUI8 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SI8:
            {
                SI8 temp = 0;
                pAttribute = new AttributeSI8 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_OBJECTID:
            {
                ObjectId tempObjectId;
                pAttribute = new AttributeObjectId (tempObjectId);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR:
            {
                vector <ObjectId> temp;
                pAttribute = new AttributeObjectIdVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_BOOL:
            {
                pAttribute = new AttributeBool (false);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR:
            {
                vector <bool> temp;
                pAttribute = new AttributeBoolVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_CHAR:
            {
                pAttribute = new AttributeChar ('\0');
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_STRING:
            {
                pAttribute = new AttributeString ("");
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_STRING_VECTOR:
            {
                vector <string> temp;
                pAttribute = new AttributeStringVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME:
            {
                WorldWideName tempWWN;
                pAttribute = new AttributeWorldWideName (tempWWN);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR:
            {
                vector <WorldWideName> temp;
                pAttribute = new AttributeWorldWideNameVector (temp);
            }
		    break;
        case WAVE_ATTRIBUTE_TYPE_MACADDRESS:
            {
                MacAddress tempMAC;
                pAttribute = new AttributeMacAddress (tempMAC);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR:
            {
                vector <MacAddress> temp;
                pAttribute = new AttributeMacAddressVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS:
            {
                IpV4Address temp;
                pAttribute = new AttributeIpV4Address (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR:
            {
                vector <IpV4Address> temp;
                pAttribute = new AttributeIpV4AddressVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS:
            {
                IpV6Address temp;
                pAttribute = new AttributeIpV6Address (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR:
            {
                vector <IpV6Address> temp;
                pAttribute = new AttributeIpV6AddressVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_ENUM:
            {
                UI32 temp = 0;
                pAttribute = new AttributeEnum (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_DATE:
            {
                Date temp;
                pAttribute = new AttributeDate (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_DATE_VECTOR:
            {
                vector <Date> temp;
                pAttribute = new AttributeDateVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_TIME:
            {
                Time temp;
                pAttribute = new AttributeTime (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_TIME_VECTOR:
            {
                vector <Time> temp;
                pAttribute = new AttributeTimeVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_DATETIME:
            {
                DateTime temp;
                pAttribute = new AttributeDateTime (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR:
            {
                vector <DateTime> temp;
                pAttribute = new AttributeDateTimeVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_BITMAP:
            {
                BitMap temp;
                pAttribute = new AttributeBitMap (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_DECIMAL64:
            {
                Decimal64 temp;
                pAttribute = new AttributeDecimal64 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_DECIMAL64_VECTOR:
            {
                vector <Decimal64> temp;
                pAttribute = new AttributeDecimal64Vector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UNION:
            {
                pAttribute = new AttributeUnion ("");
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK:
            {
                IpV4AddressNetworkMask temp;
                pAttribute = new AttributeIpV4AddressNetworkMask (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK:
            {
                IpV6AddressNetworkMask temp;
                pAttribute = new AttributeIpV6AddressNetworkMask (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SNMPOBJECTID:
            {
                SnmpObjectId temp;
                pAttribute = new AttributeSnmpObjectId (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI32_RANGE:
            {
                UI32Range temp;
                pAttribute = new AttributeUI32Range (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPVXADDRESS:
            {
                IpVxAddress temp;
                pAttribute = new AttributeIpVxAddress (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR:
            {
                vector <IpVxAddress> temp;
                pAttribute = new AttributeIpVxAddressVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_LARGEOBJECT:
            {
                LargeObject temp;
                pAttribute = new AttributeLargeObject (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_MAC_RANGE:
            {
                MACRange temp;
                pAttribute = new AttributeMACRange (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI32_UC:
            {
                UI32UC temp;
                pAttribute = new AttributeUI32UC (temp, true);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_STRING_UC:
            {
                StringUC temp;
                pAttribute = new AttributeStringUC (temp, true);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_BOOL_UC:
            {
                BoolUC temp;
                pAttribute = new AttributeBoolUC (temp, true);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_ENUM_UC:
            {
                EnumUC temp;
                pAttribute = new AttributeEnumUC (temp, true);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_MACADDRESS2:
            {
                MacAddress2 temp;
                pAttribute = new AttributeMacAddress2 (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR:
            {
                vector <MacAddress2> temp;
                pAttribute = new AttributeMacAddress2Vector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_RESOURCEID:
        case WAVE_ATTRIBUTE_TYPE_RESOURCEENUM:
            {
                UI32 temp = 0;
                pAttribute = new AttributeResourceId (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR:
            {
                vector <UI32> temp;
                pAttribute = new AttributeResourceIdVector (temp);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC:
            {
                IpV4AddressUC temp;
                pAttribute = new AttributeIpV4AddressUC (temp, true);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI32_VECTOR_UC:
            {
                UI32VectorUC temp;
                pAttribute = new AttributeUI32VectorUC (temp, true);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_HOST_UC:
            {
                HostUC temp;
                pAttribute = new AttributeHostUC (temp, true);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI64_UC:
            {
                UI64UC temp;
                pAttribute = new AttributeUI64UC (temp, true);
            }
            break;
        default:
            {
                trace (TRACE_LEVEL_FATAL, string ("Attribute::getAttributeFromAttributeType: Unsupported attributeType = ") + attributeType);
                prismAssert (false, __FILE__, __LINE__);
            }
    }

    return (pAttribute);    
}

void Attribute::toEscapedString (string &valueString)
{
    toString (valueString);
}


void Attribute::getPlainString (string &valueString)
{
    toString (valueString);
}

ResourceId Attribute::loadFromPlainString (const string &valueString)
{
    fromString (valueString);

    return WAVE_MESSAGE_SUCCESS;
}

void Attribute::toJsonString (string &jsonString)
{
    toString (jsonString);
}

}
