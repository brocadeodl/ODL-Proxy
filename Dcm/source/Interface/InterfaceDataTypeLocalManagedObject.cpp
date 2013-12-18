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
 **************************************************************************/

#include "Interface/InterfaceDataTypeLocalManagedObject.h"
#include "Interface/InterfaceClassLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "InterfaceDataType.h"

namespace DcmNs
{

    InterfaceDataTypeLocalManagedObject::InterfaceDataTypeLocalManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (InterfaceDataTypeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager)
    {
    }

    InterfaceDataTypeLocalManagedObject::~InterfaceDataTypeLocalManagedObject ()
    {
    }

    string  InterfaceDataTypeLocalManagedObject::getClassName()
    {
        return ("InterfaceDataTypeLocalManagedObject");
    }

    void  InterfaceDataTypeLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBitMap(&m_bitMap,"bitmap",interfacedatatype_bitmap, false, true));
        addPersistableAttribute (new AttributeDate(&m_date,"date",interfacedatatype_date));
        addPersistableAttribute (new AttributeTime(&m_time,"time",interfacedatatype_time));
        addPersistableAttribute (new AttributeDateTime(&m_datetime,"datetime",interfacedatatype_datetime));
        addPersistableAttribute (new AttributeDecimal64(&m_decimal64,"decimal64",interfacedatatype_decimal64));
        addPersistableAttribute (new AttributeIpV4AddressNetworkMask (&m_IpV4AddressNetworkMask, "ipv4prefix", interfacedatatype_ipv4prefix));
        addPersistableAttribute (new AttributeIpV6AddressNetworkMask (&m_IpV6AddressNetworkMask, "ipv6prefix", interfacedatatype_ipv6prefix));
        addPersistableAttribute (new AttributeSnmpObjectId (&m_SnmpObjectId, "snmpobjectid", interfacedatatype_snmpobjectid));
        addPersistableAttribute (new AttributeUnion(&m_utype,"utype",interfacedatatype_utype));
        addPersistableAttribute (new AttributeSI32(&m_utypeSi32,"utypeSi32")); // no xml tag for this internal union var
        addPersistableAttribute (new AttributeString(&m_utypeString,"utypeString")); // no xml tag for this internal union var
        addPersistableAttribute (new AttributeUI32Range(&m_UI32Range, "rangeintegers", interfacedatatype_rangeintegers));

        UI32Range  defaultData = (UI32Range) ("0");
	    addPersistableAttribute (new AttributeUI32Range(&m_UI32Range2,defaultData, "rangeintegers2", interfacedatatype_rangeintegers2));
        
        MACRange  mac_DefaultData = (MACRange) ("0000.0000.0000");
        addPersistableAttribute (new AttributeMACRange(&m_MacRange,mac_DefaultData, "rangemac", interfacedatatype_rangemac));

        UI8  UI8DefaultData  = (UI8) 8;
        addPersistableAttribute (new AttributeUI8 (&m_UI8Default, UI8DefaultData, "ui8default", interfacedatatype_ui8default));
        UI16 UI16DefaultData = (UI16) 16;
        addPersistableAttribute (new AttributeUI16 (&m_UI16Default, UI16DefaultData, "ui16default", interfacedatatype_ui16default));
        UI64 UI64DefaultData = (UI64) 64;
        addPersistableAttribute (new AttributeUI64 (&m_UI64Default, UI64DefaultData, "ui64default", interfacedatatype_ui64default));

        addPersistableAttribute (new AttributeStringUC(&m_stringUC, true, true, "d-e-f-a-u-l-t", "stringuc", interfacedatatype_stringuc));
        addPersistableAttribute (new AttributeUI32UC(&m_ui32UC, true, true, 99, "ui32uc", interfacedatatype_ui32uc));
        addPersistableAttribute (new AttributeBoolUC(&m_boolUC, false, "booluc", interfacedatatype_booluc));
        addPersistableAttribute (new AttributeEnumUC(&m_enumUC, true, true, 999, "enumuc", interfacedatatype_enumuc));

        IpV4Address ipv4address("127.0.0.1");
        addPersistableAttribute (new AttributeIpV4AddressUC(&m_ipV4AddressUC, true, true, ipv4address, "ipv4addressuc", interfacedatatype_ipv4addressuc));

        vector<UI32> defaultUI32Vec;
        defaultUI32Vec.push_back(99);
        addPersistableAttribute (new AttributeUI32VectorUC(&m_ui32VectorUC, true, true, defaultUI32Vec, "ui32vectoruc", interfacedatatype_ui32vectoruc));

        vector<string> defaultStringVec;
        defaultStringVec.push_back("d-e-f-a-u-l-t");
        addPersistableAttribute (new AttributeStringVectorUC(&m_stringVectorUC, true, true, defaultStringVec, "stringvectoruc", interfacedatatype_stringvectoruc));

        HostUC defaultHostUC("d-e-f-a-u-l-t");
        addPersistableAttribute (new AttributeHostUC(&m_hostUC, true, true, defaultHostUC, "hostuc", interfacedatatype_hostuc));

        addPersistableAttribute (new AttributeUI64UC(&m_ui64UC, true, true, 64, "ui64uc", interfacedatatype_ui64uc));

        Uuid defaultUuid;
        defaultUuid.clear();
        addPersistableAttribute (new AttributeUUID(&m_uuid, defaultUuid, "uuid", interfacedatatype_uuid));
    }

    void  InterfaceDataTypeLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBitMap(&m_bitMap,"bitmap",interfacedatatype_bitmap,false,true));
        addPersistableAttributeForCreate  (new AttributeDate(&m_date,"date",interfacedatatype_date));
        addPersistableAttributeForCreate  (new AttributeTime(&m_time,"time",interfacedatatype_time));
        addPersistableAttributeForCreate  (new AttributeDateTime(&m_datetime,"datetime",interfacedatatype_datetime));
        addPersistableAttributeForCreate  (new AttributeDecimal64(&m_decimal64,"decimal64",interfacedatatype_decimal64));
        addPersistableAttributeForCreate  (new AttributeIpV4AddressNetworkMask (&m_IpV4AddressNetworkMask, "ipv4prefix", interfacedatatype_ipv4prefix));
        addPersistableAttributeForCreate  (new AttributeIpV6AddressNetworkMask (&m_IpV6AddressNetworkMask, "ipv6prefix", interfacedatatype_ipv6prefix));
        addPersistableAttributeForCreate  (new AttributeSnmpObjectId (&m_SnmpObjectId, "snmpobjectid", interfacedatatype_snmpobjectid));
        addPersistableAttributeForCreate  (new AttributeUI32Range (&m_UI32Range, "rangeintegers", interfacedatatype_rangeintegers));

    	UI32Range  defaultData = (UI32Range) ("0");
        addPersistableAttributeForCreate  (new AttributeUI32Range(&m_UI32Range2,defaultData, "rangeintegers2", interfacedatatype_rangeintegers2));
			
        addPersistableAttributeForCreate  (new AttributeUnion(&m_utype,"utype",interfacedatatype_utype));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_utypeSi32,"utypeSi32")); // no xml tag for this internal union var
        addPersistableAttributeForCreate  (new AttributeString(&m_utypeString,"utypeString")); // no xml tag for this internal union var
        
        MACRange mac_DefaultData = (MACRange) ("0000.0000.0000");
        addPersistableAttributeForCreate  (new AttributeMACRange(&m_MacRange,mac_DefaultData, "rangemac", interfacedatatype_rangemac)); 

        UI8  UI8DefaultData  = (UI8) 8;
        addPersistableAttributeForCreate  (new AttributeUI8 (&m_UI8Default, UI8DefaultData, "ui8default", interfacedatatype_ui8default));
        UI16 UI16DefaultData = (UI16) 16;
        addPersistableAttributeForCreate  (new AttributeUI16 (&m_UI16Default, UI16DefaultData, "ui16default", interfacedatatype_ui16default));
        UI64 UI64DefaultData = (UI64) 64;
        addPersistableAttributeForCreate  (new AttributeUI64 (&m_UI64Default, UI64DefaultData, "ui64default", interfacedatatype_ui64default));

        addPersistableAttributeForCreate  (new AttributeStringUC(&m_stringUC, true, true, "d-e-f-a-u-l-t", "stringuc", interfacedatatype_stringuc));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_ui32UC, true,true, 99, "ui32uc", interfacedatatype_ui32uc));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_boolUC, false, "booluc", interfacedatatype_booluc));
        addPersistableAttributeForCreate  (new AttributeEnumUC(&m_enumUC, true, true, 999, "enumuc", interfacedatatype_enumuc));

        IpV4Address ipv4address("127.0.0.1");
        addPersistableAttributeForCreate (new AttributeIpV4AddressUC(&m_ipV4AddressUC, true, true, ipv4address, "ipv4addressuc", interfacedatatype_ipv4addressuc));

        vector<UI32> defaultUI32Vec;         
        defaultUI32Vec.push_back(99);
        addPersistableAttributeForCreate (new AttributeUI32VectorUC(&m_ui32VectorUC, true, true, defaultUI32Vec, "ui32vectoruc", interfacedatatype_ui32vectoruc));

        vector<string> defaultStringVec;
        defaultStringVec.push_back("d-e-f-a-u-l-t");
        addPersistableAttributeForCreate (new AttributeStringVectorUC(&m_stringVectorUC, true, true, defaultStringVec, "stringvectoruc", interfacedatatype_stringvectoruc));

        HostUC defaultHostUC("d-e-f-a-u-l-t");
        addPersistableAttributeForCreate  (new AttributeHostUC(&m_hostUC, true, true, defaultHostUC, "hostuc", interfacedatatype_hostuc));

        addPersistableAttributeForCreate  (new AttributeUI64UC(&m_ui64UC, true,true, 64, "ui64uc", interfacedatatype_ui64uc));

        Uuid defaultUuid;
        defaultUuid.clear();
        addPersistableAttributeForCreate  (new AttributeUUID(&m_uuid, defaultUuid, "uuid", interfacedatatype_uuid));
    }

    void  InterfaceDataTypeLocalManagedObject::setBitMap(const BitMap &bitMap)
    {
        m_bitMap  =  bitMap;
    }

    BitMap  InterfaceDataTypeLocalManagedObject::getBitMap() const
    {
        return (m_bitMap);
    }

    void  InterfaceDataTypeLocalManagedObject::setDate(const Date &date)
    {
        m_date  =  date;
    }

    Date  InterfaceDataTypeLocalManagedObject::getDate() const
    {
        return (m_date);
    }

    void  InterfaceDataTypeLocalManagedObject::setTime(const Time &time)
    {
        m_time  =  time;
    }

    Time  InterfaceDataTypeLocalManagedObject::getTime() const
    {
        return (m_time);
    }

    void  InterfaceDataTypeLocalManagedObject::setDateTime(const DateTime &datetime)
    {
        m_datetime  =  datetime;
    }

    DateTime  InterfaceDataTypeLocalManagedObject::getDateTime() const
    {
        return (m_datetime);
    }

    void  InterfaceDataTypeLocalManagedObject::setDecimal64(const Decimal64 &decimal64)
    {
        m_decimal64  =  decimal64;
    }

    Decimal64  InterfaceDataTypeLocalManagedObject::getDecimal64() const
    {
        return (m_decimal64);
    }

    void  InterfaceDataTypeLocalManagedObject::setUtypeSi32(const SI32 &val)
    {
        m_utypeSi32  =  val;
        m_utype = "utypeSi32";
    }

    SI32  InterfaceDataTypeLocalManagedObject::getUtypeSi32() const
    {
        return (m_utypeSi32);
    }

    void  InterfaceDataTypeLocalManagedObject::setUtypeString(const string &val)
    {
        m_utypeString  =  val;
        m_utype = "utypeString";
    }

    string  InterfaceDataTypeLocalManagedObject::getUtypeString() const
    {
        return (m_utypeString);
    }

    void  InterfaceDataTypeLocalManagedObject::setUtype(const WaveUnion &val)
    {
        m_utype  =  val;
    }

    WaveUnion  InterfaceDataTypeLocalManagedObject::getUtype() const
    {
        return (m_utype);
    }

    void  InterfaceDataTypeLocalManagedObject::setIpV4AddressNetworkMask (const IpV4AddressNetworkMask &ipV4NetworkMask)
    {
        m_IpV4AddressNetworkMask = ipV4NetworkMask;
    }

    IpV4AddressNetworkMask InterfaceDataTypeLocalManagedObject::getIpV4AddressNetworkMask () const
    {
        return (m_IpV4AddressNetworkMask);
    }

    void  InterfaceDataTypeLocalManagedObject::setIpV6AddressNetworkMask (const IpV6AddressNetworkMask &ipV6NetworkMask)
    {
        m_IpV6AddressNetworkMask = ipV6NetworkMask;
    }

    IpV6AddressNetworkMask InterfaceDataTypeLocalManagedObject::getIpV6AddressNetworkMask () const
    {
        return (m_IpV6AddressNetworkMask);
    }

    void  InterfaceDataTypeLocalManagedObject::setSnmpObjectId (const SnmpObjectId &snmpObjectId)
    {
        m_SnmpObjectId = snmpObjectId;
    }

    SnmpObjectId InterfaceDataTypeLocalManagedObject::getSnmpObjectId () const
    {
        return (m_SnmpObjectId);
    }

    void InterfaceDataTypeLocalManagedObject::setUI32Range (const UI32Range &rangeIntegers)
    {
        m_UI32Range = rangeIntegers;
    }

    UI32Range InterfaceDataTypeLocalManagedObject::getUI32Range () const
    {
        return (m_UI32Range);
    }

    void InterfaceDataTypeLocalManagedObject::setUI32Range2 (const UI32Range &rangeIntegers)
    {   
        m_UI32Range2 = rangeIntegers;
    }

    UI32Range InterfaceDataTypeLocalManagedObject::getUI32Range2 () const
    {   
        return (m_UI32Range2);
    }

    void InterfaceDataTypeLocalManagedObject::setMacRange (const MACRange &rangeMac)
    {   
        m_MacRange = rangeMac;
    }

    MACRange InterfaceDataTypeLocalManagedObject::getMacRange () const
    {   
        return (m_MacRange);
    }

    void InterfaceDataTypeLocalManagedObject::setUI8Default (const UI8 &UI8Default)
    {
        m_UI8Default = UI8Default;
    }

    UI8 InterfaceDataTypeLocalManagedObject::getUI8Default () const
    {
        return (m_UI8Default);
    }

    void InterfaceDataTypeLocalManagedObject::setUI16Default (const UI16 &UI16Default)
    {
        m_UI16Default = UI16Default;
    }

    UI16 InterfaceDataTypeLocalManagedObject::getUI16Default () const
    {
        return (m_UI16Default);
    }

    void InterfaceDataTypeLocalManagedObject::setUI64Default (const UI64 &UI64Default)
    {
        m_UI64Default = UI64Default;
    }

    UI64 InterfaceDataTypeLocalManagedObject::getUI64Default () const
    {
        return (m_UI64Default);
    }

    void InterfaceDataTypeLocalManagedObject::setStringUC (const StringUC &stringUC)
    {
        m_stringUC = stringUC;
    }

    StringUC InterfaceDataTypeLocalManagedObject::getStringUC () const
    {
        return (m_stringUC);
    }

    void InterfaceDataTypeLocalManagedObject::setUI32UC (const UI32UC &ui32UC)
    {
        m_ui32UC = ui32UC;
    }

    UI32UC InterfaceDataTypeLocalManagedObject::getUI32UC () const
    {
        return (m_ui32UC);
    }

    void InterfaceDataTypeLocalManagedObject::setBoolUC (const BoolUC &boolUC)
    {
        m_boolUC = boolUC;
    }

    BoolUC InterfaceDataTypeLocalManagedObject::getBoolUC () const
    {
        return (m_boolUC);
    }

    void InterfaceDataTypeLocalManagedObject::setEnumUC (const EnumUC &enumUC)
    {
        m_enumUC = enumUC;
    }

    EnumUC InterfaceDataTypeLocalManagedObject::getEnumUC () const
    {
        return (m_enumUC);
    }

    void InterfaceDataTypeLocalManagedObject::setIpV4AddressUC (const IpV4AddressUC &ipV4AddressUC)
    {
        m_ipV4AddressUC = ipV4AddressUC;
    }

    IpV4AddressUC InterfaceDataTypeLocalManagedObject::getIpV4AddressUC () const
    {
        return (m_ipV4AddressUC);
    }

    void InterfaceDataTypeLocalManagedObject::setUI32VectorUC (const UI32VectorUC &ui32VectorUC)
    {
        m_ui32VectorUC = ui32VectorUC;
    }

    UI32VectorUC InterfaceDataTypeLocalManagedObject::getUI32VectorUC () const
    {
        return (m_ui32VectorUC);
    }

    void InterfaceDataTypeLocalManagedObject::setStringVectorUC (const StringVectorUC &stringVectorUC)
    {
        m_stringVectorUC = stringVectorUC;
    }

    StringVectorUC InterfaceDataTypeLocalManagedObject::getStringVectorUC () const
    {
        return (m_stringVectorUC);
    }

    void InterfaceDataTypeLocalManagedObject::setHostUC (const HostUC &hostUC)
    {
        m_hostUC = hostUC;
    }

    HostUC InterfaceDataTypeLocalManagedObject::getHostUC () const
    {
        return (m_hostUC);
    }

    void InterfaceDataTypeLocalManagedObject::setUI64UC (const UI64UC &ui64UC)
    {
        m_ui64UC = ui64UC;
    }

    UI64UC InterfaceDataTypeLocalManagedObject::getUI64UC () const
    {
        return (m_ui64UC);
    }

    void InterfaceDataTypeLocalManagedObject::setUuid (const Uuid &uuid)
    {
        m_uuid = uuid;
    }

    Uuid InterfaceDataTypeLocalManagedObject::getUuid () const
    {
        return (m_uuid);
    }
}
