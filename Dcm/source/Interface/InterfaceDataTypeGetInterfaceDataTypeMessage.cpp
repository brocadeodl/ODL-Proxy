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
 *   Author : Himanshu Varshney                                                     *
 **************************************************************************/

#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/InterfaceDataTypeGetInterfaceDataTypeMessage.h"
#include "Interface/InterfaceDatatypeTypes.h"

namespace DcmNs
{

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage ()
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const BitMap &bitMap)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_bitMap    (bitMap)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const Date &date)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_date    (date)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const Time &time)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_time    (time)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const DateTime &datetime)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_datetime    (datetime)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const Decimal64 &decimal64)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_decimal64    (decimal64)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const SI32 &val)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_utypeSi32    (val)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const string &val)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_utypeString    (val)
    {
    }
    
    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const IpV4AddressNetworkMask &ipV4NetworkMask)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_IpV4AddressNetworkMask (ipV4NetworkMask)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const IpV6AddressNetworkMask &ipV6NetworkMask)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_IpV6AddressNetworkMask (ipV6NetworkMask)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const SnmpObjectId &snmpObjectId)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_SnmpObjectId (snmpObjectId)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const UI32Range &rangeIntegers, const UI16 flag )
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE)
    {
         if  ( 1 == flag )
	 {
            m_UI32Range = rangeIntegers;
	 }   
	 else if ( 2 ==  flag )
	 {
	    m_UI32Range2 = rangeIntegers;
	 }   
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const MACRange &rangeMac)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_MacRange (rangeMac)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const UI8 &UI8Default)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_UI8Default (UI8Default)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const UI16 &UI16Default)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_UI16Default (UI16Default)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const UI64 &UI64Default)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_UI64Default (UI64Default)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const StringUC &stringUC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_stringUC (stringUC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const UI32UC &ui32UC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_ui32UC (ui32UC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const BoolUC &boolUC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_boolUC (boolUC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const EnumUC &enumUC, const bool &isEnum)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_enumUC (enumUC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const IpV4AddressUC &ipV4AddressUC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_ipV4AddressUC (ipV4AddressUC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const StringVectorUC &stringVectorUC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_stringVectorUC (stringVectorUC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const UI32VectorUC &ui32VectorUC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_ui32VectorUC (ui32VectorUC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const HostUC &hostUC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_hostUC (hostUC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const UI64UC &ui64UC)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_ui64UC (ui64UC)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::InterfaceDataTypeGetInterfaceDataTypeMessage (const Uuid &uuid)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPEGETINTERFACEDATATYPE),
        m_uuid (uuid)
    {
    }

    InterfaceDataTypeGetInterfaceDataTypeMessage::~InterfaceDataTypeGetInterfaceDataTypeMessage ()
    {
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBitMap(&m_bitMap,"bitmap"));
        addSerializableAttribute (new AttributeDate(&m_date,"date"));
        addSerializableAttribute (new AttributeTime(&m_time,"time"));
        addSerializableAttribute (new AttributeDateTime(&m_datetime,"datetime"));
        addSerializableAttribute (new AttributeDecimal64(&m_decimal64,"decimal64"));
        addSerializableAttribute (new AttributeUnion(&m_utype,"utype"));
        addSerializableAttribute (new AttributeSI32(&m_utypeSi32,"utypeSi32"));
        addSerializableAttribute (new AttributeString(&m_utypeString,"utypeString"));
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_IpV4AddressNetworkMask,"ipV4NetworkMask"));
        addSerializableAttribute (new AttributeIpV6AddressNetworkMask(&m_IpV6AddressNetworkMask,"ipV6NetworkMask"));
        addSerializableAttribute (new AttributeSnmpObjectId(&m_SnmpObjectId,"snmpObjectId"));
        addSerializableAttribute (new AttributeUI32Range(&m_UI32Range,"rangeintegers"));
    	addSerializableAttribute (new AttributeUI32Range(&m_UI32Range2,"rangeintegers2"));
        addSerializableAttribute (new AttributeMACRange(&m_MacRange,"rangeMac"));
        addSerializableAttribute (new AttributeUI8(&m_UI8Default, "ui8default"));
        addSerializableAttribute (new AttributeUI16(&m_UI16Default, "ui16default"));
        addSerializableAttribute (new AttributeUI64(&m_UI64Default, "ui64default"));
        addSerializableAttribute (new AttributeStringUC(&m_stringUC,false, "stringuc"));
        addSerializableAttribute (new AttributeUI32UC(&m_ui32UC, false,"ui32uc"));
        addSerializableAttribute (new AttributeBoolUC(&m_boolUC, false,"booluc"));
        addSerializableAttribute (new AttributeEnumUC(&m_enumUC, false,"enumuc"));
        addSerializableAttribute (new AttributeIpV4AddressUC(&m_ipV4AddressUC, false, "ipv4addressuc"));
        addSerializableAttribute (new AttributeStringVectorUC(&m_stringVectorUC, false, "stringvectoruc"));
        addSerializableAttribute (new AttributeUI32VectorUC(&m_ui32VectorUC,false,"ui32vectoruc"));
        addSerializableAttribute (new AttributeHostUC(&m_hostUC, false, "hostuc"));
        addSerializableAttribute (new AttributeUI64UC(&m_ui64UC, false, "ui64uc"));
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setBitMap(const BitMap &bitMap)
    {
        m_bitMap  =  bitMap;
    }

    BitMap  InterfaceDataTypeGetInterfaceDataTypeMessage::getBitMap() const
    {
        return (m_bitMap);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setDate(const Date &date)
    {
        m_date  =  date;
    }

    Date  InterfaceDataTypeGetInterfaceDataTypeMessage::getDate() const
    {
        return (m_date);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setTime(const Time &time)
    {
        m_time  =  time;
    }

    Time  InterfaceDataTypeGetInterfaceDataTypeMessage::getTime() const
    {
        return (m_time);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setDateTime(const DateTime &datetime)
    {
        m_datetime  =  datetime;
    }

    DateTime  InterfaceDataTypeGetInterfaceDataTypeMessage::getDateTime() const
    {
        return (m_datetime);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setDecimal64(const Decimal64 &decimal64)
    {
        m_decimal64  =  decimal64;
    }

    Decimal64  InterfaceDataTypeGetInterfaceDataTypeMessage::getDecimal64() const
    {
        return (m_decimal64);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setUtypeSi32(const SI32 &val)
    {
        m_utypeSi32  =  val;
    }

    SI32  InterfaceDataTypeGetInterfaceDataTypeMessage::getUtypeSi32() const
    {
        return (m_utypeSi32);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setUtypeString(const string &val)
    {
        m_utypeString  =  val;
    }

    string  InterfaceDataTypeGetInterfaceDataTypeMessage::getUtypeString() const
    {
        return (m_utypeString);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setUtype(const WaveUnion &val)
    {
        m_utype  =  val;
    }

    WaveUnion  InterfaceDataTypeGetInterfaceDataTypeMessage::getUtype() const
    {
        return (m_utype);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setIpV4AddressNetworkMask (const IpV4AddressNetworkMask &ipV4NetworkMask)
    {
        m_IpV4AddressNetworkMask = ipV4NetworkMask;
    }

    IpV4AddressNetworkMask InterfaceDataTypeGetInterfaceDataTypeMessage::getIpV4AddressNetworkMask () const
    {
        return (m_IpV4AddressNetworkMask);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setIpV6AddressNetworkMask (const IpV6AddressNetworkMask &ipV6NetworkMask)
    {
        m_IpV6AddressNetworkMask = ipV6NetworkMask;
    }

    IpV6AddressNetworkMask InterfaceDataTypeGetInterfaceDataTypeMessage::getIpV6AddressNetworkMask () const
    {
        return (m_IpV6AddressNetworkMask);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setSnmpObjectId (const SnmpObjectId &snmpObjectId)
    {
        m_SnmpObjectId = snmpObjectId;
    }

    SnmpObjectId InterfaceDataTypeGetInterfaceDataTypeMessage::getSnmpObjectId () const
    {
        return (m_SnmpObjectId);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI32Range (const UI32Range &rangeIntegers)
    {
        m_UI32Range = rangeIntegers;
    }

    UI32Range InterfaceDataTypeGetInterfaceDataTypeMessage::getUI32Range () const
    {
        return (m_UI32Range);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI32Range2 (const UI32Range &rangeIntegers)
    {   
        m_UI32Range2 = rangeIntegers;
    }

    UI32Range InterfaceDataTypeGetInterfaceDataTypeMessage::getUI32Range2 () const
    {   
        return (m_UI32Range2);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setMacRange (const MACRange &rangeMac)
    {   
        m_MacRange = rangeMac;
    }

    MACRange InterfaceDataTypeGetInterfaceDataTypeMessage::getMacRange () const
    {   
        return (m_MacRange);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI8Default (const UI8 &UI8Default)
    {
        m_UI8Default= UI8Default;
    }

    UI8 InterfaceDataTypeGetInterfaceDataTypeMessage::getUI8Default () const
    {
        return (m_UI8Default);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI16Default (const UI16 &UI16Default)
    {
        m_UI16Default= UI16Default;
    }

    UI16 InterfaceDataTypeGetInterfaceDataTypeMessage::getUI16Default () const
    {
        return (m_UI16Default);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI64Default (const UI64 &UI64Default)
    {
        m_UI64Default= UI64Default;
    }

    UI64 InterfaceDataTypeGetInterfaceDataTypeMessage::getUI64Default () const
    {
        return (m_UI64Default);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setStringUC (const StringUC &stringUC)
    {
        m_stringUC = stringUC;
    }

    StringUC InterfaceDataTypeGetInterfaceDataTypeMessage::getStringUC () const
    {
        return (m_stringUC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI32UC (const UI32UC &ui32UC)
    {
        m_ui32UC = ui32UC;
    }

    UI32UC InterfaceDataTypeGetInterfaceDataTypeMessage::getUI32UC () const
    {
        return (m_ui32UC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setBoolUC (const BoolUC &boolUC)
    {
        m_boolUC = boolUC;
    }

    BoolUC InterfaceDataTypeGetInterfaceDataTypeMessage::getBoolUC () const
    {
        return (m_boolUC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setEnumUC (const EnumUC &enumUC)
    {
        m_enumUC = enumUC;
    }

    EnumUC InterfaceDataTypeGetInterfaceDataTypeMessage::getEnumUC () const
    {
        return (m_enumUC);
    }

    void  InterfaceDataTypeGetInterfaceDataTypeMessage::setIpV4AddressUC (const IpV4AddressUC &ipV4AddressUC)
    {
        m_ipV4AddressUC = ipV4AddressUC;
    }

    IpV4AddressUC InterfaceDataTypeGetInterfaceDataTypeMessage::getIpV4AddressUC () const
    {
        return (m_ipV4AddressUC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setStringVectorUC (const StringVectorUC &stringVectorUC)
    {
        m_stringVectorUC = stringVectorUC;
    }

    StringVectorUC InterfaceDataTypeGetInterfaceDataTypeMessage::getStringVectorUC () const
    {
        return (m_stringVectorUC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI32VectorUC (const UI32VectorUC &ui32VectorUC)
    {
        m_ui32VectorUC = ui32VectorUC;
    }

    UI32VectorUC InterfaceDataTypeGetInterfaceDataTypeMessage::getUI32VectorUC () const
    {
        return (m_ui32VectorUC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setHostUC (const HostUC &hostUC)
    {
        m_hostUC = hostUC;
    }

    HostUC InterfaceDataTypeGetInterfaceDataTypeMessage::getHostUC () const
    {
        return (m_hostUC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUI64UC (const UI64UC &ui64UC)
    {
        m_ui64UC = ui64UC;
    }

    UI64UC InterfaceDataTypeGetInterfaceDataTypeMessage::getUI64UC () const
    {
        return (m_ui64UC);
    }

    void InterfaceDataTypeGetInterfaceDataTypeMessage::setUuid (const Uuid &uuid)
    {
        m_uuid = uuid;
    }

    Uuid InterfaceDataTypeGetInterfaceDataTypeMessage::getUuid () const
    {
        return (m_uuid);
    }
}

