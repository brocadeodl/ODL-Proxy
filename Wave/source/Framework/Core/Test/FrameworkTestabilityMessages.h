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

#ifndef FRAMEWORKTESTABILITYMESSAGES_H
#define FRAMEWORKTESTABILITYMESSAGES_H

#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/MacAddress.h"
#include "Framework/Types/Date.h"
#include "Framework/Types/Time.h"
#include "Framework/Types/DateTime.h"
#include "Framework/Types/Decimal64.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Core/Test/FrameworkTestabilityTypes.h"
#include "Framework/Trace/TraceTypes.h"
#include "Framework/Attributes/AttributeSI64.h"
#include "Framework/Attributes/AttributeSI64Vector.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Framework/Attributes/AttributeDate.h"
#include "Framework/Attributes/AttributeTime.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Attributes/AttributeDecimal64.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeSnmpObjectId.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/Attributes/AttributeLargeObject.h"
#include "Framework/Attributes/AttributeMACRange.h"
#include <string>
#include <vector>

namespace WaveNs
{

class FrameworkTestabilityMessage1 : public PrismMessage
{
    private :
    protected :
        void setupAttributesForSerialization ();

    public :
                        FrameworkTestabilityMessage1 ();
        virtual        ~FrameworkTestabilityMessage1 ();
                string  getMessage                   ();
                void    setMessage                   (string message);

    // Now the data members

    private :
        string m_message;

    protected :
    public :
};

class FrameworkTestabilityMessage2 : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkTestabilityMessage2 ();
        virtual ~FrameworkTestabilityMessage2 ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityMessage3 : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                       FrameworkTestabilityMessage3 ();
        virtual       ~FrameworkTestabilityMessage3 ();
                void   setupInputBuffer1            ();
                void   setupOutputBuffer1           ();
                bool   validateInputBuffer1         ();
                bool   validateOutputBuffer1        ();
                void   setInputLargeBuffer1         ();
                bool   validateInputLargeBuffer1    ();

                void        setCompletionStatusType      (const UI32 &completionStatusType);
                ResourceId  getRequestedCompletionStatus ();

    // Now the data members

    private :
        bool m_isLargInputBuffer1Set;
        UI32 m_completionStatusType;

    protected :
    public :
};

class FrameworkTestabilityMessage4 : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                      FrameworkTestabilityMessage4          ();
        virtual      ~FrameworkTestabilityMessage4          ();
                void  setupBoolInput                        ();
                bool  validateBoolContent                   ();
                void  setupCharInput                        ();
                bool  validateCharContent                   ();
                void  setupSI16Input                        ();
                bool  validateSI16Content                   ();
                void  setupSI32Input                        ();
                bool  validateSI32Content                   ();
		void  setupSI64Input                        ();
		bool  validateSI64Content                   ();
                void  setupSI8Input                         ();
                bool  validateSI8Content                    ();
		void  setupSI64VectorInput                  ();
		bool  validateSI64VectorContent             ();
                void  setupStringInput                      ();
                bool  validateStringContent                 ();
                void  setupStringVectorInput                ();
                bool  validateStringVectorContent           ();
                void  setupUI16Input                        ();
                bool  validateUI16Content                   ();
                void  setupUI32Input                        ();
                bool  validateUI32Content                   ();
		void  setupUI64Input                        ();
		bool  validateUI64Content                   ();
                void  setupUI8Input                         ();
                bool  validateUI8Content                    ();
                void  setupLocationIdInput                  ();
                bool  validateLocationIdContent             ();
                void  setupResourceIdInput                  ();
                bool  validateResourceIdContent             ();
                void  setupWaveMessagePriorityInput         ();
                bool  validateWaveMessagePriorityContent    ();
                void  setupWaveMessageTypeInput             ();
                bool  validateWaveMessageTypeContent        ();
                void  setupPrismServiceIdInput              ();
                bool  validatePrismServiceIdContent         ();
                void  setupSerializableObjectTypeInput      ();
                bool  validateSerializableObjectTypeContent ();
                void  setupTraceClientIdInput               ();
                bool  validateTraceClientIdContent          ();
                void  setupTraceLevelInput                  ();
                bool  validateTraceLevelContent             ();
                void  setWorldWideNameContent               ();
                bool  validateWorldWideNameContent          ();
                void  setWorldWideNameVectorContent         ();
                bool  validateWorldWideNameVectorContent    ();
                void  setMacAddressContent                  ();
                bool  validateMacAddressContent             ();
                void  setMacAddressVectorContent            ();
                bool  validateMacAddressVectorContent       ();
		        void  setupEnumInput			            ();
		        bool  validateEnumContent		            ();
                void  setDateContent                        ();
                bool  validateDateContent                   ();
                void  setDateVectorContent                  ();
                bool  validateDateVectorContent             ();
                void  setTimeContent                        ();
                bool  validateTimeContent                   ();
                void  setTimeVectorContent                  ();
                bool  validateTimeVectorContent             ();
                void  setDateTimeContent                    ();
                bool  validateDateTimeContent               ();
                void  setDateTimeVectorContent              ();
                bool  validateDateTimeVectorContent         ();
		        void  setBitMapContent                      ();
                bool  validateBitMapContent                 ();
                void  setDecimal64Content                   ();
                bool  validateDecimal64Content              ();
                void  setDecimal64VectorContent             ();
                bool  validateDecimal64VectorContent        ();
                void  setIpV4AddressNetworkMask             ();
                bool  validateIpV4AddressNetworkMask        ();
                void  setIpV6AddressNetworkMask             ();
                bool  validateIpV6AddressNetworkMask        ();
                void  setSnmpObjectId                       ();
                bool  validateSnmpObjectId                  ();
                void  setUI32Range                          ();
                bool  validateUI32Range                     ();
                void  setIpVxAddress                        ();
                bool  validateIpVxAddress                   ();
                void  setIpVxAddressVector                  ();
                bool  validateIpVxAddressVector             ();
                void  setLargeObjectContent                 ();
                bool  validateLargeObjectContent            ();

                void  setupInput                            ();
                bool  validateContent                       ();

                void  setMacRange                          ();
                bool  validateMacRange                     ();

    // Now the data members

    private :
        bool                   m_bool1;
        bool                   m_bool2;
        bool                   m_bool3;

        char                   m_char1;
        char                   m_char2;
        char                   m_char3;

        SI16                   m_si161;
        SI16                   m_si162;
        SI16                   m_si163;

        SI32                   m_si321;
        SI32                   m_si322;
        SI32                   m_si323;

	SI64                   m_si641;
	SI64                   m_si642;
	SI64                   m_si643;

        SI8                    m_si81;
        SI8                    m_si82;
        SI8                    m_si83;

	vector<SI64>           m_si64Vector;

        string                 m_string1;
        string                 m_string2;
        string                 m_string3;

        vector<string>         m_stringVector1;
        vector<string>         m_stringVector2;
        vector<string>         m_stringVector3;

        UI16                   m_ui161;
        UI16                   m_ui162;
        UI16                   m_ui163;

        UI32                   m_ui321;
        UI32                   m_ui322;
        UI32                   m_ui323;

	UI64                   m_ui641;
	UI64                   m_ui642;
	UI64                   m_ui643;

        UI8                    m_ui81;
        UI8                    m_ui82;
        UI8                    m_ui83;

        LocationId             m_locationId1;
        LocationId             m_locationId2;
        LocationId             m_locationId3;

        ResourceId             m_resourceId1;
        ResourceId             m_resourceId2;
        ResourceId             m_resourceId3;

        ResourceId               m_prismMessagePriority1; // WaveMessagePriority
        ResourceId               m_prismMessagePriority2; // WaveMessagePriority
        ResourceId               m_prismMessagePriority3; // WaveMessagePriority

        ResourceId               m_prismMessageType1; // WaveMessageType
        ResourceId               m_prismMessageType2; // WaveMessageType
        ResourceId               m_prismMessageType3; // WaveMessageType

        PrismServiceId           m_prismServiceId1;
        PrismServiceId           m_prismServiceId2;
        PrismServiceId           m_prismServiceId3;

        SerializableObjectType m_serializableObjectType1;
        SerializableObjectType m_serializableObjectType2;
        SerializableObjectType m_serializableObjectType3;

        TraceClientId          m_traceClientId1;
        TraceClientId          m_traceClientId2;
        TraceClientId          m_traceClientId3;

        ResourceId             m_traceLevel1; // TraceLevel
        ResourceId             m_traceLevel2; // TraceLevel
        ResourceId             m_traceLevel3; // TraceLevel

        WorldWideName          m_worldWideName;
        vector<WorldWideName>  m_worldWideNameVector;

        MacAddress             m_macAddress;
        vector<MacAddress>     m_macAddressVector;

	    Day		               m_enum1;
	    Day		               m_enum2;
	    Day		               m_enum3;

        BitMap                 m_bitMap1;
        BitMap                 m_bitMap2;
        BitMap                 m_bitMap3;

        Date                   m_date;
        vector<Date>           m_dateVector;

        Time                   m_time;
        vector<Time>           m_timeVector;

        DateTime               m_datetime;
        vector<DateTime>       m_datetimeVector;

        Decimal64              m_decimal64;
        vector<Decimal64>      m_decimal64Vector;

        IpV4AddressNetworkMask m_ipV4NetworkMask;

        IpV6AddressNetworkMask m_ipV6NetworkMask;
        
        SnmpObjectId           m_snmpObjectId;

        UI32Range              m_UI32Range1;
        UI32Range              m_UI32Range2;
        UI32Range              m_UI32Range3;
        UI32Range              m_UI32Range4;
        UI32Range              m_UI32Range5;
        UI32Range              m_UI32Range6;

        IpVxAddress            m_ipVxAddress1;
        IpVxAddress            m_ipVxAddress2;
        
        vector<IpVxAddress>    m_ipVxAddressVector;
    
        LargeObject            m_largeObject1;
        LargeObject            m_largeObject2;
        LargeObject            m_largeObject3;
    
        MACRange              m_MacRange1;
        MACRange              m_MacRange2;
        MACRange              m_MacRange3;
        MACRange              m_MacRange4;
        MACRange              m_MacRange5;
        MACRange              m_MacRange6;
    
    protected :
    public :
};

class FrameworkTestabilityMessage5 : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkTestabilityMessage5 ();
        virtual ~FrameworkTestabilityMessage5 ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityMessage6 : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkTestabilityMessage6 ();
        virtual ~FrameworkTestabilityMessage6 ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityMessage7 : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkTestabilityMessage7 ();
        virtual ~FrameworkTestabilityMessage7 ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityMessage8 : public PrismMessage
{
    private :
    protected :
    public :
                      FrameworkTestabilityMessage8  ();
        virtual      ~FrameworkTestabilityMessage8  ();

                UI32  getNumberOfObjectsToBeCreated () const;
                void  setNumberOfObjectsToBeCreated (const UI32 &numberOfObjectsToBeCreated);
				UI32  getPageSize();
				void  setPageSize(UI32 pageSize);
    // Now the data members

    private :
        UI32 m_numberOfObjectsToBeCreated;
        UI32 m_pageSize;

    protected :
    public :
};

class FrameworkTestabilityMessage9 : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkTestabilityMessage9 ();
        virtual ~FrameworkTestabilityMessage9 ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityMessage10 : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkTestabilityMessage10 ();
        virtual ~FrameworkTestabilityMessage10 ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityMessage11 : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkTestabilityMessage11 ();
        virtual ~FrameworkTestabilityMessage11 ();
               UI32  getNumberOfObjectsToBeCreated () const;
               void  setNumberOfObjectsToBeCreated (const UI32 &numberOfObjectsToBeCreated);

    // Now the data members

    private :
		UI32 m_numberOfObjectsToBeCreated;
    protected :
    public :
};

}

#endif // FRAMEWORKTESTABILITYMESSAGES_H
