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

#ifndef INTERFACEDATATYPECONFIGUREINTERFACEDATATYPEMESSAGE_H
#define INTERFACEDATATYPECONFIGUREINTERFACEDATATYPEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Framework/Attributes/AttributeDate.h"
#include "Framework/Attributes/AttributeTime.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Attributes/AttributeDecimal64.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeSnmpObjectId.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Attributes/AttributeMACRange.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"
#include "Framework/Attributes/AttributeUI32VectorUC.h"
#include "Framework/Attributes/AttributeStringVectorUC.h"
#include "Framework/Attributes/AttributeHostUC.h"
#include "Framework/Attributes/AttributeUI64UC.h"
#include "Framework/Attributes/AttributeUUID.h"

using namespace WaveNs;

namespace DcmNs
{

    class InterfaceDataTypeConfigureInterfaceDataTypeMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            InterfaceDataTypeConfigureInterfaceDataTypeMessage ();
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const BitMap &bitMap);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Date &date);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Time &time);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const DateTime &datetime);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Decimal64 &decimal64);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const SI32 &val);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const string &val);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const IpV4AddressNetworkMask &ipV4NetworkMask);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const IpV6AddressNetworkMask &ipV6NetworkMask);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const SnmpObjectId &snmpObjectId);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI32Range &rangeIntegers, const UI16 flag = 1);
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const MACRange &rangeMac);	
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI8 &UI8Default);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI16 &UI16Default);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI64 &UI64Default);
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const StringUC &stringUC);
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI32UC   &ui32UC);	
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const BoolUC   &boolUC);	
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const EnumUC   &enumUC, const bool &isEnum);
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const IpV4AddressUC   &ipV4AddressUC);	
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI32VectorUC   &ui32VectorUC);	
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const StringVectorUC   &stringVectorUC);	
    	    InterfaceDataTypeConfigureInterfaceDataTypeMessage (const HostUC &hostUC);	
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const UI64UC &ui64UC);
            InterfaceDataTypeConfigureInterfaceDataTypeMessage (const Uuid &uuid);

            virtual    ~InterfaceDataTypeConfigureInterfaceDataTypeMessage ();

            void setBitMap(const BitMap &bitMap);
            BitMap getBitMap()  const;

            void setDate(const Date &date);
            Date getDate()  const;

            void setTime(const Time &time);
            Time getTime()  const;

            void setDateTime(const DateTime &datetime);
            DateTime getDateTime()  const;

            void setDecimal64(const Decimal64 &decimal64);
            Decimal64 getDecimal64()  const;

            void setIpV4AddressNetworkMask (const IpV4AddressNetworkMask &ipV4NetworkMask);
            IpV4AddressNetworkMask getIpV4AddressNetworkMask () const;

            void setIpV6AddressNetworkMask (const IpV6AddressNetworkMask &ipV6NetworkMask);
            IpV6AddressNetworkMask getIpV6AddressNetworkMask () const;

            void setSnmpObjectId (const SnmpObjectId &snmpObjectId);
            SnmpObjectId getSnmpObjectId () const;

            void setUI32Range (const UI32Range &rangeIntegers);
            UI32Range   getUI32Range () const;

    	    void setUI32Range2 (const UI32Range &rangeIntegers);
            UI32Range   getUI32Range2 () const;

	        void setMacRange (const MACRange &rangeMac);
            MACRange   getMacRange () const;

            void        setUI8Default       (const UI8 &UI8Default);
            UI8         getUI8Default       () const;

            void        setUI16Default      (const UI16 &UI16Default);
            UI16        getUI16Default      () const;

            void        setUI64Default      (const UI64 &UI64Default);
            UI64        getUI64Default      () const;

            // define a get & set for each data type supported inside union
            void        setUtypeSi32(const SI32 &val);
            SI32        getUtypeSi32()  const;
            void        setUtypeString(const string &val);
            string      getUtypeString()  const;
            void        setUtype(const WaveUnion &val);
            WaveUnion   getUtype()  const;

            void        setStringUC (const StringUC &stringUC);
            StringUC    getStringUC () const;
            void        setUI32UC   (const UI32UC &ui32UC);
            UI32UC      getUI32UC   () const;
            void        setBoolUC   (const BoolUC &boolUC);
            BoolUC      getBoolUC   () const;
            void        setEnumUC   (const EnumUC &enumUC);
            EnumUC      getEnumUC   () const;

            void          setIpV4AddressUC (const IpV4AddressUC &ipV4AddressUC);
            IpV4AddressUC getIpV4AddressUC () const;

            void           setUI32VectorUC (const UI32VectorUC &ui32VectorUC);
            UI32VectorUC   getUI32VectorUC () const;
            void           setStringVectorUC (const StringVectorUC &stringVectorUC);
            StringVectorUC getStringVectorUC () const;

            void        setHostUC   (const HostUC &hostUC);
            HostUC      getHostUC   () const;

            void        setUI64UC   (const UI64UC &ui64UC);
            UI64UC      getUI64UC   () const;

            void        setUuid   (const Uuid &uuid);
            Uuid        getUuid   () const;

// Now the data members

        private:
        protected:
        public:
            BitMap                  m_bitMap;
            Date                    m_date;
            Time                    m_time;
            DateTime                m_datetime;
            Decimal64               m_decimal64;
            WaveUnion               m_utype;
            SI32                    m_utypeSi32; // this data type is part of m_utype union & is not exposed directly
            string                  m_utypeString; // this data type is part of m_utype union & is not exposed directly
            IpV4AddressNetworkMask  m_IpV4AddressNetworkMask;
            IpV6AddressNetworkMask  m_IpV6AddressNetworkMask;
            SnmpObjectId            m_SnmpObjectId;
            UI32Range               m_UI32Range;
            UI32Range               m_UI32Range2;
            MACRange                m_MacRange;
            UI8                     m_UI8Default;
            UI16                    m_UI16Default;
            UI64                    m_UI64Default;
            StringUC                m_stringUC;
            UI32UC                  m_ui32UC;
            BoolUC                  m_boolUC;
            EnumUC                  m_enumUC;
            IpV4AddressUC           m_ipV4AddressUC;
            UI32VectorUC            m_ui32VectorUC;
            StringVectorUC          m_stringVectorUC;
            HostUC                  m_hostUC;
            UI64UC                  m_ui64UC;
            Uuid                    m_uuid;
    };
}
#endif                                            //INTERFACEDATATYPECONFIGUREINTERFACEPRIORITYMESSAGE_H
