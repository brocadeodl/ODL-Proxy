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
#include "Interface/InterfaceDataTypeConfigureInterfaceDataTypeMessage.h"
#include "Interface/InterfaceDatatypeTypes.h"

namespace DcmNs
{

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage ()
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE)
    {
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const BitMap &bitMap)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE), 
        m_bitMap  (bitMap)
    {
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_decimal64 = Decimal64("0.00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }
    
    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Date &date)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE), 
        m_date  (date)
    {
        m_bitMap = BitMap(0);
        m_time = Time("00:00:00.000000");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_decimal64 = Decimal64("0.00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
      	m_UI32Range2 = UI32Range("0");
    	m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }
    
    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Time &time)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE), 
        m_time  (time)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_decimal64 = Decimal64("0.00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
       	m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }
    
    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const DateTime &datetime)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE), 
        m_datetime  (datetime)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }
    
    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Decimal64 &decimal64)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE), 
        m_decimal64  (decimal64)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }
    
    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const SI32 &val)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE), 
        m_utype ("utypeSi32"),
        m_utypeSi32  (val)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_decimal64 = Decimal64("0.00");
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }
    
    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const string &val)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE), 
        m_utype ("utypeString"),
        m_utypeString  (val)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_decimal64 = Decimal64("0.00");
        m_utypeSi32 = (0);
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const IpV4AddressNetworkMask &ipV4NetworkMask)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_IpV4AddressNetworkMask (ipV4NetworkMask)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }
   
    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const IpV6AddressNetworkMask &ipV6NetworkMask)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_IpV6AddressNetworkMask (ipV6NetworkMask)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

     InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const SnmpObjectId &snmpObjectId)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_SnmpObjectId (snmpObjectId)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI32Range &rangeIntegers, const UI16 flag)
        : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE)
    {
	    if  ( 1 == flag )
            {
	       m_UI32Range = rangeIntegers;
	       m_UI32Range2 = UI32Range("0");
	    }
	    else if ( 2 ==  flag )
	    {
	     m_UI32Range = UI32Range("0");
	     m_UI32Range2 = rangeIntegers;
	    }
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
    	m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const MACRange &rangeMac)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
	    m_MacRange (rangeMac)
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI8 &UI8Default)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
	    m_UI8Default (UI8Default)
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI16 &UI16Default)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
	    m_UI16Default (UI16Default)
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
        m_UI8Default  = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI64 &UI64Default)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
	    m_UI64Default (UI64Default)
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const StringUC &stringUC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
	    m_stringUC (stringUC)
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
    	m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_ui32UC     = UI32UC (0); 
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI32UC &ui32UC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_ui32UC (ui32UC)
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
    	m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const BoolUC &boolUC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_boolUC (boolUC) 
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
    	m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const EnumUC &enumUC, const bool &isEnum)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_enumUC (enumUC) 
    {
		m_bitMap = BitMap(0);
		m_date = Date("0000-00-00");
		m_time = Time("00:00:00.000000");
		m_decimal64 = Decimal64("0.00");
		m_datetime = DateTime("0000-00-00 00:00:00");
		m_utype= ("");
		m_utypeSi32 = (0);
		m_utypeString = ("");
		m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
		m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
		m_UI32Range = UI32Range("0");
		m_UI32Range2 = UI32Range("0");
    	m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0); 
        m_boolUC     = BoolUC (false);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const IpV4AddressUC   &ipV4AddressUC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_ipV4AddressUC (ipV4AddressUC)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC (ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI32VectorUC   &ui32VectorUC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),         m_ui32VectorUC (ui32VectorUC)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const StringVectorUC   &stringVectorUC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),         m_stringVectorUC (stringVectorUC)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC(ui32VectorUC);
        m_ui64UC         = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const HostUC &hostUC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),         
        m_hostUC (hostUC)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC(ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_ui64UC         = UI64UC (0);
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI64UC &ui64UC)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_ui64UC (ui64UC)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC(ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_uuid.clear();
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Uuid &uuid)
       : PrismMessage (ValInterfaceLocalObjectManager::getPrismServiceId (), INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE),
        m_uuid (uuid)
    {
        m_bitMap = BitMap(0);
        m_date = Date("0000-00-00");
        m_time = Time("00:00:00.000000");
        m_decimal64 = Decimal64("0.00");
        m_datetime = DateTime("0000-00-00 00:00:00");
        m_utype= ("");
        m_utypeSi32 = (0);
        m_utypeString = ("");
        m_IpV4AddressNetworkMask = IpV4AddressNetworkMask("0.0.0.0/0");
        m_IpV6AddressNetworkMask = IpV6AddressNetworkMask("0:0:0:0:0:0:0:0/0");
        m_SnmpObjectId           = SnmpObjectId("0");
        m_UI32Range = UI32Range("0");
        m_UI32Range2 = UI32Range("0");
        m_MacRange = MACRange("0000.0000.0000");
        m_UI8Default  = (0);
        m_UI16Default = (0);
        m_UI64Default = (0);
        m_stringUC   = StringUC ("");
        m_ui32UC     = UI32UC (0);
        m_boolUC     = BoolUC (false);
        m_enumUC     = EnumUC (999);
        IpV4Address ipV4Address("0.0.0.0");
        m_ipV4AddressUC = IpV4AddressUC (ipV4Address);
        vector<UI32> ui32VectorUC;
        ui32VectorUC.clear ();
        m_ui32VectorUC = UI32VectorUC(ui32VectorUC);
        vector<string> stringVectorUC;
        stringVectorUC.clear ();
        m_stringVectorUC = StringVectorUC (stringVectorUC);
        m_hostUC = HostUC ("");
        m_ui64UC         = UI64UC (0);
    }

    InterfaceDataTypeConfigureInterfaceDataTypeMessage::~InterfaceDataTypeConfigureInterfaceDataTypeMessage ()
    {
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setupAttributesForSerialization()
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
	    addSerializableAttribute (new AttributeUI32Range(&m_UI32Range2,"rangeintegers2"));
        addSerializableAttribute (new AttributeMACRange(&m_MacRange,"rangeMac"));
        addSerializableAttribute (new AttributeUI8(&m_UI8Default, "ui8default"));
        addSerializableAttribute (new AttributeUI16(&m_UI16Default, "ui16default"));
        addSerializableAttribute (new AttributeUI64(&m_UI64Default, "ui64default"));
        addSerializableAttribute (new AttributeStringUC(&m_stringUC, false, "stringuc"));
        addSerializableAttribute (new AttributeUI32UC(&m_ui32UC, false, "ui32uc"));
        addSerializableAttribute (new AttributeBoolUC(&m_boolUC, false, "booluc"));
        addSerializableAttribute (new AttributeEnumUC(&m_enumUC, false, "enumuc"));
        addSerializableAttribute (new AttributeIpV4AddressUC(&m_ipV4AddressUC,false,"ipv4addressuc"));
        addSerializableAttribute (new AttributeStringVectorUC(&m_stringVectorUC, false, "stringvectoruc"));
        addSerializableAttribute (new AttributeUI32VectorUC(&m_ui32VectorUC, false, "ui32vectoruc"));
        addSerializableAttribute (new AttributeHostUC(&m_hostUC, false, "hostuc"));
        addSerializableAttribute (new AttributeUI64UC(&m_ui64UC, false, "ui64uc"));
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setBitMap(const BitMap &bitMap)
    {
        m_bitMap  =  bitMap;
    }

    BitMap  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getBitMap() const
    {
        return (m_bitMap);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setDate(const Date &date)
    {
        m_date  =  date;
    }

    Date  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getDate() const
    {
        return (m_date);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setTime(const Time &time)
    {
        m_time  =  time;
    }

    Time  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getTime() const
    {
        return (m_time);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setDateTime(const DateTime &datetime)
    {
        m_datetime  =  datetime;
    }

    DateTime  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getDateTime() const
    {
        return (m_datetime);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setDecimal64(const Decimal64 &decimal64)
    {
        m_decimal64  =  decimal64;
    }

    Decimal64  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getDecimal64() const
    {
        return (m_decimal64);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUtypeSi32(const SI32 &val)
    {
        m_utypeSi32  =  val;
    }

    SI32  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUtypeSi32() const
    {
        return (m_utypeSi32);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUtypeString(const string &val)
    {
        m_utypeString  =  val;
    }

    string  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUtypeString() const
    {
        return (m_utypeString);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUtype(const WaveUnion &val)
    {
        m_utype  =  val;
    }

    WaveUnion  InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUtype() const
    {
        return (m_utype);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setIpV4AddressNetworkMask (const IpV4AddressNetworkMask &ipV4NetworkMask)
    {
        m_IpV4AddressNetworkMask = ipV4NetworkMask;
    }

    IpV4AddressNetworkMask InterfaceDataTypeConfigureInterfaceDataTypeMessage::getIpV4AddressNetworkMask () const
    {
        return (m_IpV4AddressNetworkMask);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setIpV6AddressNetworkMask (const IpV6AddressNetworkMask &ipV6NetworkMask)
    {
        m_IpV6AddressNetworkMask = ipV6NetworkMask;
    }

    IpV6AddressNetworkMask InterfaceDataTypeConfigureInterfaceDataTypeMessage::getIpV6AddressNetworkMask () const
    {
        return (m_IpV6AddressNetworkMask);
    }

    void  InterfaceDataTypeConfigureInterfaceDataTypeMessage::setSnmpObjectId (const SnmpObjectId &snmpObjectId)
    {
        m_SnmpObjectId = snmpObjectId;
    }

    SnmpObjectId InterfaceDataTypeConfigureInterfaceDataTypeMessage::getSnmpObjectId () const
    {
        return (m_SnmpObjectId);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI32Range (const UI32Range &rangeIntegers)
    {
        m_UI32Range = rangeIntegers;
    }

    UI32Range InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI32Range () const
    {
        return (m_UI32Range);
    }
    
    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI32Range2 (const UI32Range &rangeIntegers)
    {   
        m_UI32Range2 = rangeIntegers;
    }

    UI32Range InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI32Range2 () const
    {   
        return (m_UI32Range2);
    }
    
    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setMacRange (const MACRange &rangeMac)
    {   
        m_MacRange = rangeMac;
    }

    MACRange InterfaceDataTypeConfigureInterfaceDataTypeMessage::getMacRange () const
    {   
        return (m_MacRange);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI8Default (const UI8 &UI8Default)
    {
        m_UI8Default = UI8Default;
    }

    UI8 InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI8Default () const
    {
        return (m_UI8Default);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI16Default (const UI16 &UI16Default)
    {
        m_UI16Default = UI16Default;
    }

    UI16 InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI16Default () const
    {
        return (m_UI16Default);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI64Default (const UI64 &UI64Default)
    {
        m_UI64Default = UI64Default;
    }

    UI64 InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI64Default () const
    {
        return (m_UI64Default);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setStringUC (const StringUC &stringUC)
    {
        m_stringUC = stringUC;
    }

    StringUC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getStringUC () const
    {
        return (m_stringUC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI32UC (const UI32UC &ui32UC)
    {
        m_ui32UC = ui32UC;
    }

    UI32UC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI32UC () const
    {
        return (m_ui32UC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setBoolUC (const BoolUC &boolUC)
    {
        m_boolUC = boolUC;
    }

    BoolUC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getBoolUC () const
    {
        return (m_boolUC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setEnumUC (const EnumUC &enumUC)
    {
        m_enumUC = enumUC;
    }

    EnumUC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getEnumUC () const
    {
        return (m_enumUC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setIpV4AddressUC (const IpV4AddressUC &ipV4AddressUC)
    {
        m_ipV4AddressUC = ipV4AddressUC;
    }

    IpV4AddressUC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getIpV4AddressUC () const
    {
        return (m_ipV4AddressUC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setStringVectorUC (const StringVectorUC &stringVectorUC)
    {
        m_stringVectorUC = stringVectorUC;
    }

    StringVectorUC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getStringVectorUC () const
    {
        return (m_stringVectorUC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI32VectorUC (const UI32VectorUC &ui32VectorUC)
    {
        m_ui32VectorUC = ui32VectorUC;
    }

    UI32VectorUC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI32VectorUC () const
    {
        return (m_ui32VectorUC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setHostUC (const HostUC &hostUC)
    {
        m_hostUC = hostUC;
    }

    HostUC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getHostUC () const
    {
        return (m_hostUC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUI64UC (const UI64UC &ui64UC)
    {
        m_ui64UC = ui64UC;
    }

    UI64UC InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUI64UC () const
    {
        return (m_ui64UC);
    }

    void InterfaceDataTypeConfigureInterfaceDataTypeMessage::setUuid (const Uuid &uuid)
    {
        m_uuid = uuid;
    }

    Uuid InterfaceDataTypeConfigureInterfaceDataTypeMessage::getUuid () const
    {
        return (m_uuid);
    }
}
