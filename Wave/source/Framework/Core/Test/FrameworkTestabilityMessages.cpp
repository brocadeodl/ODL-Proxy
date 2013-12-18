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

#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability2ObjectManager.h"
#include "Framework/Core/Test/FrameworkCoreTestObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability2ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability4ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability5ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestability6ObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include <algorithm>


using namespace std;

namespace WaveNs
{

char pInputString1[]  = "JFeastuhser CShornist is LHoorldy and SGahvoisotr of all";
char pOutputString1[] = "JHeesiuss Cthhreist is the  wSaoyn, tohfe tGroudth and the life";
char pInputString2[]  = "JHeesRues Cihgrnissto Ivse Lroarldl.";
char pInputString3[]  = "PHreaailsoen Heidme Psreiravsees Hailml Pprraaiissee Hainmd fgolroervyer.";

vector<SI64> vectorSI64(10);

static UI32 s_largeInputBuffer1Size = 5 * 1024 * 1024; // 5 MB.
static SI8  s_largeInputBuffer1Char = 'J';

FrameworkTestabilityMessage1::FrameworkTestabilityMessage1 ()
    : PrismMessage (FrameworkTestability1ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE1)
{
}

FrameworkTestabilityMessage1::~FrameworkTestabilityMessage1 ()
{
}

string FrameworkTestabilityMessage1::getMessage ()
{
    return (m_message);
}

void FrameworkTestabilityMessage1::setMessage (string message)
{
    m_message = message;
}

void FrameworkTestabilityMessage1::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_message, "message"));
}

FrameworkTestabilityMessage2::FrameworkTestabilityMessage2 ()
    : PrismMessage (FrameworkTestability1ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE2)
{
}

FrameworkTestabilityMessage2::~FrameworkTestabilityMessage2 ()
{
}

FrameworkTestabilityMessage3::FrameworkTestabilityMessage3 ()
    : PrismMessage (FrameworkTestability1ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE3),
      m_isLargInputBuffer1Set (false),
      m_completionStatusType  (0)
{
}

FrameworkTestabilityMessage3::~FrameworkTestabilityMessage3 ()
{
}

void FrameworkTestabilityMessage3::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isLargInputBuffer1Set, "isLargInputBuffer1Set"));
     addSerializableAttribute (new AttributeUI32 (&m_completionStatusType,  "completionStatusType"));
}

void FrameworkTestabilityMessage3::setupInputBuffer1 ()
{
    addBuffer (0, sizeof (pInputString1), pInputString1, false);
}

void FrameworkTestabilityMessage3::setupOutputBuffer1 ()
{
    addBuffer (1, sizeof (pOutputString1), pOutputString1, false);
}

bool FrameworkTestabilityMessage3::validateInputBuffer1 ()
{
    UI32  bufferSize = 0;
    void *pBuffer    = NULL;

    pBuffer = findBuffer (0, bufferSize);

    prismAssert (NULL != pBuffer, __FILE__, __LINE__);

    if (0 == strcmp (pInputString1, reinterpret_cast<char *> (pBuffer)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool FrameworkTestabilityMessage3::validateOutputBuffer1 ()
{
    UI32  bufferSize = 0;
    void *pBuffer    = NULL;

    pBuffer = findBuffer (1, bufferSize);

    prismAssert (NULL != pBuffer, __FILE__, __LINE__);

    if (0 == strcmp (pOutputString1, reinterpret_cast<char *> (pBuffer)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void FrameworkTestabilityMessage3::setInputLargeBuffer1 ()
{
    SI8  *pTempBuffer = new SI8[s_largeInputBuffer1Size];
    UI32  i           = 0;

    for (i = 0; i < s_largeInputBuffer1Size; i++)
    {
        pTempBuffer[i] = s_largeInputBuffer1Char;
    }

    addBuffer (100, s_largeInputBuffer1Size, pTempBuffer, true);

    m_isLargInputBuffer1Set = true;
}

bool FrameworkTestabilityMessage3::validateInputLargeBuffer1 ()
{
    if (false == m_isLargInputBuffer1Set)
    {
        return (true);
    }

    UI32  bufferSize = 0;
    SI8  *pBuffer    = NULL;
    UI32  i          = 0;

    pBuffer = reinterpret_cast<SI8 *> (findBuffer (100, bufferSize));

    //cout << "Buffer Size : " << bufferSize << endl;

    prismAssert (s_largeInputBuffer1Size == bufferSize, __FILE__, __LINE__);
    prismAssert (NULL != pBuffer, __FILE__, __LINE__);

    for (i = 0; i < s_largeInputBuffer1Size; i++)
    {
        if (s_largeInputBuffer1Char != (pBuffer[i]))
        {
            return (false);
        }
    }

    return (true);
}

void FrameworkTestabilityMessage3::setCompletionStatusType (const UI32 &completionStatusType)
{
    m_completionStatusType = completionStatusType;
}

ResourceId FrameworkTestabilityMessage3::getRequestedCompletionStatus ()
{
    // Get deterministic completion status for message of this type based on the current connected locations (plus self) and the completion status type
    //
    // Completion Status Type:
    // 0 : ALL locations completion status => WAVE_MESSAGE_SUCCESS
    //
    // 1 : ALL locations completion status => WAVE_MESSAGE_ERROR
    //
    // 2 : Modulo 0 indexed locations completion status => WAVE_MESSAGE_ERROR
    //     Modulo 1 indexed locations completion status => WAVE_MESSAGE_SUCCESS
    //
    // 3 : Modulo 0 indexed locations completion status => WAVE_MESSAGE_ERROR
    //     Modulo 1 indexed locations completion status => WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE
    //     Modulo 2 indexed locations completion status => WAVE_MESSAGE_SUCCESS
    //
    // 4 : Modulo 0 indexed locations completion status => WAVE_MESSAGE_ERROR
    //     Modulo 1 indexed locations completion status => WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE
    //
    // 5 : Modulo 0 indexed locations completion status => WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE
    //     Module 1 indexed locations completion status => WAVE_MESSAGE_SUCCESS

    UI32                i                                   = 0;
    UI32                numberOfLocations                   = 0;
    LocationId          locationId                          = FrameworkToolKit::getThisLocationId ();
    vector<LocationId>  allLocations;
    ResourceId          status                              = WAVE_MESSAGE_ERROR;

    allLocations.clear ();

    if (getIsMessageBeingSurrogatedFlag ())
    {
        LocationId locationId1      = 0;
        FrameworkToolKit::getKnownLocations (allLocations);
        LocationBase *pThisLocation = FrameworkToolKit::getPThisLocation ();
        if (NULL != pThisLocation)
        {
            locationId1 = (pThisLocation->getLocationId ());
        }
        allLocations.push_back (locationId1);
    }
    else
    {
        FrameworkToolKit::getConnectedLocations (allLocations);
        allLocations.push_back (locationId);
    }

    sort (allLocations.begin (), allLocations.end ());

    numberOfLocations = static_cast<UI32> (allLocations.size ());


    // Find the index of this location in the connection locations vector

    for (i = 0; i < numberOfLocations; i++)
    {
        if (locationId == allLocations[i])
        {
            break;
        }
    }

    if (numberOfLocations <= i)
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestabilityMessage3::getRequestedCompletionStatus : Location id \"") + locationId + "\" could not be found in allLocations vector");
        prismAssert (false, __FILE__, __LINE__);
    }


    // Determine the completion status based on completionStatusType and index position in the allLocations vector

    if (0 == m_completionStatusType)
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    else if (1 == m_completionStatusType)
    {
        status = WAVE_MESSAGE_ERROR;
    }
    else if (2 == m_completionStatusType)
    {
        if (0 == (i % 2))
        {
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }
    else if (3 == m_completionStatusType)
    {
        if (0 == (i % 3))
        {
            status = WAVE_MESSAGE_ERROR;
        }
        else if (1 == (i % 3))
        {
            status = WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }
    else if (4 == m_completionStatusType)
    {
        if (0 == (i % 2))
        {
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE;
        }
    }
    else if (5 == m_completionStatusType)
    {
        if (0 == (i % 2))
        {
            status = WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE;
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("FrameworkTestabilityMessage3::getRequestedCompletionStatus : CompletionStatusType invalid."));
        prismAssert (false, __FILE__, __LINE__);
    }

    trace (TRACE_LEVEL_DEBUG, string ("FrameworkTestabilityMessage3::getRequestedCompletionStatus : CompletionStatusType : ") + m_completionStatusType + ", LocationId " + allLocations[i] + " => " + FrameworkToolKit::localize (status));

    return (status);
}

FrameworkTestabilityMessage4::FrameworkTestabilityMessage4 ()
    : PrismMessage (FrameworkTestability1ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE4)
{
    m_bool1 = false;                                           // bool                   m_bool1;
    m_bool2 = false;                                           // bool                   m_bool2;
    m_bool3 = false;                                           // bool                   m_bool3;
                                                               // 
    m_char1 = '\0';                                            // char                   m_char1;
    m_char2 = '\0';                                            // char                   m_char2;
    m_char3 = '\0';                                            // char                   m_char3;
                                                               // 
    m_si161 = 0;                                               // SI16                   m_si161;
    m_si162 = 0;                                               // SI16                   m_si162;
    m_si163 = 0;                                               // SI16                   m_si163;
                                                               // 
    m_si321 = 0;                                               // SI32                   m_si321;
    m_si322 = 0;                                               // SI32                   m_si322;
    m_si323 = 0;                                               // SI32                   m_si323;
                                                               // 
    m_si641 = 0;                                               // SI64                   m_si641;
    m_si642 = 0;                                               // SI64                   m_si642;
    m_si643 = 0;                                               // SI64                   m_si643;
                                                               // 
    m_si81 = 0;                                                // SI8                    m_si81;
    m_si82 = 0;                                                // SI8                    m_si82;
    m_si83 = 0;                                                // SI8                    m_si83;
                                                               // 
                                                               // vector<SI64>           m_si64Vector;
                                                               // 
                                                               // string                 m_string1;
                                                               // string                 m_string2;
                                                               // string                 m_string3;
                                                               // 
                                                               // vector<string>         m_stringVector1;
                                                               // vector<string>         m_stringVector2;
                                                               // vector<string>         m_stringVector3;
                                                               // 
    m_ui161 = 0;                                               // UI16                   m_ui161;
    m_ui162 = 0;                                               // UI16                   m_ui162;
    m_ui163 = 0;                                               // UI16                   m_ui163;
                                                               // 
    m_ui321 = 0;                                               // UI32                   m_ui321;
    m_ui322 = 0;                                               // UI32                   m_ui322;
    m_ui323 = 0;                                               // UI32                   m_ui323;
                                                               // 
    m_ui641 = 0;                                               // UI64                   m_ui641;
    m_ui642 = 0;                                               // UI64                   m_ui642;
    m_ui643 = 0;                                               // UI64                   m_ui643;
                                                               // 
    m_ui81 = 0;                                                // UI8                    m_ui81;
    m_ui82 = 0;                                                // UI8                    m_ui82;
    m_ui83 = 0;                                                // UI8                    m_ui83;
                                                               // 
    m_locationId1 = 0;                                         // LocationId             m_locationId1;
    m_locationId2 = 0;                                         // LocationId             m_locationId2;
    m_locationId3 = 0;                                         // LocationId             m_locationId3;
                                                               // 
    m_resourceId1 = WAVE_MESSAGE_ERROR;                        // ResourceId             m_resourceId1;
    m_resourceId2 = WAVE_MESSAGE_ERROR;                        // ResourceId             m_resourceId2;
    m_resourceId3 = WAVE_MESSAGE_ERROR;                        // ResourceId             m_resourceId3;
                                                               // 
    m_prismMessagePriority1 = WAVE_MESSAGE_PRIORITY_NORMAL;    // ResourceId             m_prismMessagePriority1; // WaveMessagePriority
    m_prismMessagePriority2 = WAVE_MESSAGE_PRIORITY_NORMAL;    // ResourceId             m_prismMessagePriority2; // WaveMessagePriority
    m_prismMessagePriority3 = WAVE_MESSAGE_PRIORITY_NORMAL;    // ResourceId             m_prismMessagePriority3; // WaveMessagePriority
                                                               // 
    m_prismMessageType1 = WAVE_MESSAGE_TYPE_REQUEST;           // ResourceId             m_prismMessageType1; // WaveMessageType
    m_prismMessageType2 = WAVE_MESSAGE_TYPE_REQUEST;           // ResourceId             m_prismMessageType2; // WaveMessageType
    m_prismMessageType3 = WAVE_MESSAGE_TYPE_REQUEST;           // ResourceId             m_prismMessageType3; // WaveMessageType
                                                               // 
    m_prismServiceId1 = 0;                                     // PrismServiceId         m_prismServiceId1;
    m_prismServiceId2 = 0;                                     // PrismServiceId         m_prismServiceId2;
    m_prismServiceId3 = 0;                                     // PrismServiceId         m_prismServiceId3;
                                                               // 
    m_serializableObjectType1 = 0;                             // SerializableObjectType m_serializableObjectType1;
    m_serializableObjectType2 = 0;                             // SerializableObjectType m_serializableObjectType2;
    m_serializableObjectType3 = 0;                             // SerializableObjectType m_serializableObjectType3;
                                                               // 
    m_traceClientId1 = 0;                                      // TraceClientId          m_traceClientId1;
    m_traceClientId2 = 0;                                      // TraceClientId          m_traceClientId2;
    m_traceClientId3 = 0;                                      // TraceClientId          m_traceClientId3;
                                                               // 
    m_traceLevel1 = TRACE_LEVEL_UNKNOWN;                       // ResourceId             m_traceLevel1; // TraceLevel
    m_traceLevel2 = TRACE_LEVEL_UNKNOWN;                       // ResourceId             m_traceLevel2; // TraceLevel
    m_traceLevel3 = TRACE_LEVEL_UNKNOWN;                       // ResourceId             m_traceLevel3; // TraceLevel
                                                               // 
                                                               // WorldWideName          m_worldWideName;
                                                               // vector<WorldWideName>  m_worldWideNameVector;
                                                               // 
                                                               // MacAddress             m_macAddress;
                                                               // vector<MacAddress>     m_macAddressVector;
                                                               // 
    m_enum1 = Sunday;                                          // Day                     m_enum1;
    m_enum2 = Sunday;                                          // Day                     m_enum2;
    m_enum3 = Sunday;                                          // Day                     m_enum3;
                                                               // 
                                                               // BitMap                 m_bitMap1;
                                                               // BitMap                 m_bitMap2;
                                                               // BitMap                 m_bitMap3;
                                                               // 
                                                               // Date                   m_date;
                                                               // vector<Date>           m_dateVector;
                                                               // 
                                                               // Time                   m_time;
                                                               // vector<Time>           m_timeVector;
                                                               // 
                                                               // DateTime               m_datetime;
                                                               // vector<DateTime>       m_datetimeVector;
                                                               // 
                                                               // Decimal64              m_decimal64;
                                                               // vector<Decimal64>      m_decimal64Vector;
                                                               // 
                                                               // IpV4AddressNetworkMask m_ipV4NetworkMask;
                                                               // 
                                                               // IpV6AddressNetworkMask m_ipV6NetworkMask;
                                                               // 
                                                               // SnmpObjectId           m_snmpObjectId;
                                                               // 
                                                               // UI32Range              m_UI32Range1;
                                                               // UI32Range              m_UI32Range2;
                                                               // UI32Range              m_UI32Range3;
                                                               // UI32Range              m_UI32Range4;
                                                               // UI32Range              m_UI32Range5;
                                                               // UI32Range              m_UI32Range6;
                                                               // 
                                                               // IpVxAddress            m_ipVxAddress1;
                                                               // IpVxAddress            m_ipVxAddress2;
                                                               // 
                                                               // vector<IpVxAddress>    m_ipVxAddressVector;
                                                               // 
                                                               // LargeObject            m_largeObject1;
                                                               // LargeObject            m_largeObject2;
                                                               // LargeObject            m_largeObject3;
                                                               // 
                                                               // MACRange               m_MacRange1;
                                                               // MACRange               m_MacRange2;
                                                               // MACRange               m_MacRange3;
                                                               // MACRange               m_MacRange4;
                                                               // MACRange               m_MacRange5;
                                                               // MACRange               m_MacRange6;
}

FrameworkTestabilityMessage4::~FrameworkTestabilityMessage4 ()
{
}

void FrameworkTestabilityMessage4::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_bool1, "bool1"));
     addSerializableAttribute (new AttributeBool (&m_bool2, "bool2"));
     addSerializableAttribute (new AttributeBool (&m_bool3, "bool3"));

     addSerializableAttribute (new AttributeChar (&m_char1, "char1"));
     addSerializableAttribute (new AttributeChar (&m_char2, "char2"));
     addSerializableAttribute (new AttributeChar (&m_char3, "char3"));

     addSerializableAttribute (new AttributeSI16 (&m_si161, "si161"));
     addSerializableAttribute (new AttributeSI16 (&m_si162, "si162"));
     addSerializableAttribute (new AttributeSI16 (&m_si163, "si163"));

     addSerializableAttribute (new AttributeSI32 (&m_si321, "si321"));
     addSerializableAttribute (new AttributeSI32 (&m_si322, "si322"));
     addSerializableAttribute (new AttributeSI32 (&m_si323, "si323"));

     addSerializableAttribute (new AttributeSI64 (&m_si641, "si641"));
     addSerializableAttribute (new AttributeSI64 (&m_si642, "si642"));
     addSerializableAttribute (new AttributeSI64 (&m_si643, "si643"));

     addSerializableAttribute (new AttributeSI8 (&m_si81, "si81"));
     addSerializableAttribute (new AttributeSI8 (&m_si82, "si82"));
     addSerializableAttribute (new AttributeSI8 (&m_si83, "si83"));

     addSerializableAttribute (new AttributeSI64Vector (&m_si64Vector, "si64Vector"));

     addSerializableAttribute (new AttributeString (&m_string1, "string1"));
     addSerializableAttribute (new AttributeString (&m_string2, "string2"));
     addSerializableAttribute (new AttributeString (&m_string3, "string3"));

     addSerializableAttribute (new AttributeStringVector (&m_stringVector1, "stringVector1"));
     addSerializableAttribute (new AttributeStringVector (&m_stringVector2, "stringVector2"));
     addSerializableAttribute (new AttributeStringVector (&m_stringVector3, "stringVector3"));

     addSerializableAttribute (new AttributeUI16 (&m_ui161, "ui161"));
     addSerializableAttribute (new AttributeUI16 (&m_ui162, "ui162"));
     addSerializableAttribute (new AttributeUI16 (&m_ui163, "ui163"));

     addSerializableAttribute (new AttributeUI32 (&m_ui321, "ui321"));
     addSerializableAttribute (new AttributeUI32 (&m_ui322, "ui322"));
     addSerializableAttribute (new AttributeUI32 (&m_ui323, "ui323"));

     addSerializableAttribute (new AttributeUI64 (&m_ui641, "ui641"));
     addSerializableAttribute (new AttributeUI64 (&m_ui642, "ui642"));
     addSerializableAttribute (new AttributeUI64 (&m_ui643, "ui643"));

     addSerializableAttribute (new AttributeUI8 (&m_ui81, "ui81"));
     addSerializableAttribute (new AttributeUI8 (&m_ui82, "ui82"));
     addSerializableAttribute (new AttributeUI8 (&m_ui83, "ui83"));

     addSerializableAttribute (new AttributeLocationId (&m_locationId1, "locationId1"));
     addSerializableAttribute (new AttributeLocationId (&m_locationId2, "locationId2"));
     addSerializableAttribute (new AttributeLocationId (&m_locationId3, "locationId3"));

     addSerializableAttribute (new AttributeResourceId (&m_resourceId1, "resourceId1"));
     addSerializableAttribute (new AttributeResourceId (&m_resourceId2, "resourceId2"));
     addSerializableAttribute (new AttributeResourceId (&m_resourceId3, "resourceId3"));

     addSerializableAttribute (new AttributeWaveMessagePriority (&m_prismMessagePriority1, "prismMessagePriority1"));
     addSerializableAttribute (new AttributeWaveMessagePriority (&m_prismMessagePriority2, "prismMessagePriority2"));
     addSerializableAttribute (new AttributeWaveMessagePriority (&m_prismMessagePriority3, "prismMessagePriority3"));

     addSerializableAttribute (new AttributeWaveMessageType (&m_prismMessageType1, "prismMessageType1"));
     addSerializableAttribute (new AttributeWaveMessageType (&m_prismMessageType2, "prismMessageType2"));
     addSerializableAttribute (new AttributeWaveMessageType (&m_prismMessageType3, "prismMessageType3"));

     addSerializableAttribute (new AttributePrismServiceId (&m_prismServiceId1, "prismServiceId1"));
     addSerializableAttribute (new AttributePrismServiceId (&m_prismServiceId2, "prismServiceId2"));
     addSerializableAttribute (new AttributePrismServiceId (&m_prismServiceId3, "prismServiceId3"));

     addSerializableAttribute (new AttributeSerializableObjectType (&m_serializableObjectType1, "serializableObjectType1"));
     addSerializableAttribute (new AttributeSerializableObjectType (&m_serializableObjectType2, "serializableObjectType2"));
     addSerializableAttribute (new AttributeSerializableObjectType (&m_serializableObjectType3, "serializableObjectType3"));

     addSerializableAttribute (new AttributeTraceClientId (&m_traceClientId1, "traceClientId1"));
     addSerializableAttribute (new AttributeTraceClientId (&m_traceClientId2, "traceClientId2"));
     addSerializableAttribute (new AttributeTraceClientId (&m_traceClientId3, "traceClientId3"));

     addSerializableAttribute (new AttributeTraceLevel (&m_traceLevel1, "traceLevel1"));
     addSerializableAttribute (new AttributeTraceLevel (&m_traceLevel2, "traceLevel2"));
     addSerializableAttribute (new AttributeTraceLevel (&m_traceLevel3, "traceLevel3"));

     addSerializableAttribute (new AttributeWorldWideName       (&m_worldWideName,          "worldWideName"));
     addSerializableAttribute (new AttributeWorldWideNameVector (&m_worldWideNameVector,    "worldWideNameVector"));

     addSerializableAttribute (new AttributeMacAddress       (&m_macAddress,        "macAddress"));
     addSerializableAttribute (new AttributeMacAddressVector (&m_macAddressVector,  "macAddressVector"));

    addSerializableAttribute (new AttributeEnum ((UI32 *) (&m_enum1),   "enum1"));
    addSerializableAttribute (new AttributeEnum ((UI32 *) (&m_enum2),   "enum2"));
    addSerializableAttribute (new AttributeEnum ((UI32 *) (&m_enum3),   "enum3"));

     addSerializableAttribute (new AttributeBitMap (&m_bitMap1, "bitMap1"));
     addSerializableAttribute (new AttributeBitMap (&m_bitMap2, "bitMap2"));
     addSerializableAttribute (new AttributeBitMap (&m_bitMap3, "bitMap3"));

     addSerializableAttribute (new AttributeDate       (&m_date,        "date"));
     addSerializableAttribute (new AttributeDateVector (&m_dateVector,  "dateVector"));

     addSerializableAttribute (new AttributeTime       (&m_time,        "time"));
     addSerializableAttribute (new AttributeTimeVector (&m_timeVector,  "timeVector"));

     addSerializableAttribute (new AttributeDateTime       (&m_datetime,        "datetime"));
     addSerializableAttribute (new AttributeDateTimeVector (&m_datetimeVector,  "datetimeVector"));

     addSerializableAttribute (new AttributeDecimal64       (&m_decimal64,      "decimal64"));
     addSerializableAttribute (new AttributeDecimal64Vector (&m_decimal64Vector,"decimal64Vector"));

     addSerializableAttribute (new AttributeIpV4AddressNetworkMask (&m_ipV4NetworkMask, "ipV4NetworkMask"));

     addSerializableAttribute (new AttributeIpV6AddressNetworkMask (&m_ipV6NetworkMask, "ipV6NetworkMask"));

     addSerializableAttribute (new AttributeSnmpObjectId (&m_snmpObjectId, "snmpObjectId"));

     addSerializableAttribute (new AttributeUI32Range    (&m_UI32Range1, "UI32Range1"));
     addSerializableAttribute (new AttributeUI32Range    (&m_UI32Range2, "UI32Range2"));
     addSerializableAttribute (new AttributeUI32Range    (&m_UI32Range3, "UI32Range3"));
     addSerializableAttribute (new AttributeUI32Range    (&m_UI32Range4, "UI32Range4"));
     addSerializableAttribute (new AttributeUI32Range    (&m_UI32Range5, "UI32Range5"));
     addSerializableAttribute (new AttributeUI32Range    (&m_UI32Range6, "UI32Range6"));

     addSerializableAttribute (new AttributeIpVxAddress  (&m_ipVxAddress1, "ipVxAddress1"));
     addSerializableAttribute (new AttributeIpVxAddress  (&m_ipVxAddress2, "ipVxAddress2"));
    
     addSerializableAttribute (new AttributeIpVxAddressVector  (&m_ipVxAddressVector, "ipVxAddressVector"));

 	addSerializableAttribute (new AttributeLargeObject	(&m_largeObject1, "largeObject1"));
 	addSerializableAttribute (new AttributeLargeObject	(&m_largeObject2, "largeObject2"));
 	addSerializableAttribute (new AttributeLargeObject	(&m_largeObject3, "largeObject3"));

     addSerializableAttribute (new AttributeMACRange    (&m_MacRange1, "MacRange1"));
     addSerializableAttribute (new AttributeMACRange    (&m_MacRange2, "MacRange2"));
     addSerializableAttribute (new AttributeMACRange    (&m_MacRange3, "MacRange3"));    
     addSerializableAttribute (new AttributeMACRange    (&m_MacRange4, "MacRange4"));
     addSerializableAttribute (new AttributeMACRange    (&m_MacRange5, "MacRange5"));
     addSerializableAttribute (new AttributeMACRange    (&m_MacRange6, "MacRange6"));
}

void FrameworkTestabilityMessage4::setupBoolInput ()
{
    m_bool1 = true;
    m_bool2 = false;
    m_bool3 = true;
}

bool FrameworkTestabilityMessage4::validateBoolContent ()
{
    if (true != m_bool1)
    {
        return (false);
    }

    if (false != m_bool2)
    {
        return (false);
    }

    if (true != m_bool3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupCharInput ()
{
    m_char1 = '\0';
    m_char2 = 'A';
    m_char3 = '9';
}

bool FrameworkTestabilityMessage4::validateCharContent ()
{
    if ('\0' != m_char1)
    {
        return (false);
    }

    if ('A' != m_char2)
    {
        return (false);
    }

    if ('9' != m_char3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupSI16Input ()
{
    m_si161 = SHRT_MAX;
    m_si162 = 0;
    m_si163 = SHRT_MIN;
}

bool FrameworkTestabilityMessage4::validateSI16Content ()
{
    if (SHRT_MAX != m_si161)
    {
        return (false);
    }

    if (0 != m_si162)
    {
        return (false);
    }

    if (SHRT_MIN != m_si163)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupSI32Input ()
{
    m_si321 = INT_MAX;
    m_si322 = 0;
    m_si323 = INT_MIN;
}

bool FrameworkTestabilityMessage4::validateSI32Content ()
{
    if (INT_MAX != m_si321)
    {
        return (false);
    }

    if (0 != m_si322)
    {
        return (false);
    }

    if (INT_MIN != m_si323)
    {
        return (false);
    }

    return (true);
}
void FrameworkTestabilityMessage4::setupSI64Input ()
{
    m_si641 = WAVE_LLONG_MAX;
    m_si642 = SCHAR_MIN;
    m_si643 = WAVE_LLONG_MIN;
}

bool FrameworkTestabilityMessage4::validateSI64Content ()
{
    if (WAVE_LLONG_MAX != m_si641)
    {
        return (false);
    }

    if (SCHAR_MIN != m_si642)
    {
	return (false);
    }

    if (WAVE_LLONG_MIN != m_si643)
    {
	return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupSI8Input ()
{
    m_si81 = SCHAR_MAX;
    m_si82 = 0;
    m_si83 = SCHAR_MIN;
}

bool FrameworkTestabilityMessage4::validateSI8Content ()
{
    if (SCHAR_MAX != m_si81)
    {
        return (false);
    }

    if (0 != m_si82)
    {
        return (false);
    }

    if (SCHAR_MIN != m_si83)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupSI64VectorInput ()
{
    m_si64Vector = vectorSI64;
}

bool FrameworkTestabilityMessage4::validateSI64VectorContent ()
{
    if (vectorSI64 != m_si64Vector)
    {
	return (false);
    }
    return (true);
}

void FrameworkTestabilityMessage4::setupStringInput ()
{
    m_string1 = pInputString1;
    m_string2 = pInputString2;
    m_string3 = pInputString3;
}

bool FrameworkTestabilityMessage4::validateStringContent ()
{
    if (0 != (strcmp (pInputString1, m_string1.c_str ())))
    {
        return (false);
    }

    if (0 != (strcmp (pInputString2, m_string2.c_str ())))
    {
        return (false);
    }

    if (0 != (strcmp (pInputString3, m_string3.c_str ())))
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupStringVectorInput ()
{
    // setup m_stringVector1

    m_stringVector1.clear ();
    m_stringVector1.push_back (pInputString1);
    m_stringVector1.push_back (pInputString2);
    m_stringVector1.push_back (pInputString3);

    // setup m_stringVector2

    m_stringVector2.clear ();
    m_stringVector2.push_back (pInputString2);

    // setup m_stringVector3

    m_stringVector3.clear ();
}

bool FrameworkTestabilityMessage4::validateStringVectorContent ()
{
    // Validate m_stringVector1

    UI32 numberOfStrings1 = m_stringVector1.size ();

    if (3 != numberOfStrings1)
    {
        return (false);
    }

    if (0 != (strcmp ((m_stringVector1[0]).c_str (), pInputString1)))
    {
        return (false);
    }

    if (0 != (strcmp ((m_stringVector1[1]).c_str (), pInputString2)))
    {
        return (false);
    }
    if (0 != (strcmp ((m_stringVector1[2]).c_str (), pInputString3)))
    {
        return (false);
    }

    // Validate m_stringVector2

    UI32 numberOfStrings2 = m_stringVector2.size ();

    if (1 != numberOfStrings2)
    {
        return (false);
    }

    if (0 != (strcmp ((m_stringVector2[0]).c_str (), pInputString2)))
    {
        return (false);
    }

    // Validate m_stringVector3

    UI32 numberOfStrings3 = m_stringVector3.size ();

    if (0 != numberOfStrings3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupUI16Input ()
{
    m_ui161 = USHRT_MAX;
    m_ui162 = 0;
    m_ui163 = 0;
}

bool FrameworkTestabilityMessage4::validateUI16Content ()
{
    if (USHRT_MAX != m_ui161)
    {
        return (false);
    }

    if (0 != m_ui162)
    {
        return (false);
    }

    if (0 != m_ui163)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupUI32Input ()
{
    m_ui321 = UINT_MAX;
    m_ui322 = 0;
    m_ui323 = 0;
}

bool FrameworkTestabilityMessage4::validateUI32Content ()
{
    if (UINT_MAX != m_ui321)
    {
        return (false);
    }

    if (0 != m_ui322)
    {
        return (false);
    }

    if (0 != m_ui323)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupUI64Input ()
{
    m_ui641 = WAVE_ULLONG_MAX;
    m_ui642 = UINT_MAX;
    m_ui643 = 0;
}

bool FrameworkTestabilityMessage4::validateUI64Content ()
{
    if (WAVE_ULLONG_MAX != m_ui641)
    {
        return (false);
    }

    if (UINT_MAX != m_ui642)
    {
        return (false);
    }

    if (0 != m_ui643)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupUI8Input ()
{
    m_ui81 = UCHAR_MAX;
    m_ui82 = 0;
    m_ui83 = 0;
}

bool FrameworkTestabilityMessage4::validateUI8Content ()
{
    if (UCHAR_MAX != m_ui81)
    {
        return (false);
    }

    if (0 != m_ui82)
    {
        return (false);
    }

    if (0 != m_ui83)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupLocationIdInput ()
{
    m_locationId1 = 0;
    m_locationId2 = 768;
    m_locationId3 = 534;
}

bool FrameworkTestabilityMessage4::validateLocationIdContent ()
{
    if (0 != m_locationId1)
    {
        return (false);
    }

    if (768 != m_locationId2)
    {
        return (false);
    }

    if (534 != m_locationId3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupResourceIdInput ()
{
    m_resourceId1 = WAVE_MESSAGE_SUCCESS;
    m_resourceId2 = WAVE_MESSAGE_ERROR;
    m_resourceId3 = FRAMEWORK_SUCCESS;
}

bool FrameworkTestabilityMessage4::validateResourceIdContent ()
{
    if (WAVE_MESSAGE_SUCCESS != m_resourceId1)
    {
        return (false);
    }

    if (WAVE_MESSAGE_ERROR != m_resourceId2)
    {
        return (false);
    }

    if (FRAMEWORK_SUCCESS != m_resourceId3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupWaveMessagePriorityInput ()
{
    m_prismMessagePriority1 = WAVE_MESSAGE_PRIORITY_NORMAL;
    m_prismMessagePriority2 = WAVE_MESSAGE_PRIORITY_HIGH;
    m_prismMessagePriority3 = WAVE_MESSAGE_PRIORITY_HIGH;
}

bool FrameworkTestabilityMessage4::validateWaveMessagePriorityContent ()
{
    if (WAVE_MESSAGE_PRIORITY_NORMAL != m_prismMessagePriority1)
    {
        return (false);
    }

    if (WAVE_MESSAGE_PRIORITY_HIGH != m_prismMessagePriority2)
    {
        return (false);
    }

    if (WAVE_MESSAGE_PRIORITY_HIGH != m_prismMessagePriority3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupWaveMessageTypeInput ()
{
    m_prismMessageType1 = WAVE_MESSAGE_TYPE_REQUEST;
    m_prismMessageType2 = WAVE_MESSAGE_TYPE_RESPONSE;
    m_prismMessageType3 = WAVE_MESSAGE_TYPE_REQUEST;
}

bool FrameworkTestabilityMessage4::validateWaveMessageTypeContent ()
{
    if (WAVE_MESSAGE_TYPE_REQUEST != m_prismMessageType1)
    {
        return (false);
    }

    if (WAVE_MESSAGE_TYPE_RESPONSE != m_prismMessageType2)
    {
        return (false);
    }

    if (WAVE_MESSAGE_TYPE_REQUEST != m_prismMessageType3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupPrismServiceIdInput ()
{
    m_prismServiceId1 = FrameworkCoreTestObjectManager::getPrismServiceId ();
    m_prismServiceId2 = FrameworkTestability1ObjectManager::getPrismServiceId ();
    m_prismServiceId3 = FrameworkTestability2ObjectManager::getPrismServiceId ();
}

bool FrameworkTestabilityMessage4::validatePrismServiceIdContent ()
{
    if ((FrameworkCoreTestObjectManager::getPrismServiceId ()) != m_prismServiceId1)
    {
        return (false);
    }

    if ((FrameworkTestability1ObjectManager::getPrismServiceId ()) != m_prismServiceId2)
    {
        return (false);
    }

    if ((FrameworkTestability2ObjectManager::getPrismServiceId ()) != m_prismServiceId3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupSerializableObjectTypeInput ()
{
    m_serializableObjectType1 = 0;
    m_serializableObjectType2 = 0;
    m_serializableObjectType3 = 0;
}

bool FrameworkTestabilityMessage4::validateSerializableObjectTypeContent ()
{
    if (0 != m_serializableObjectType1)
    {
        return (false);
    }

    if (0 != m_serializableObjectType2)
    {
        return (false);
    }

    if (0 != m_serializableObjectType3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupTraceClientIdInput ()
{
    m_traceClientId1 = 1;
    m_traceClientId2 = 7;
    m_traceClientId3 = 77;
}

bool FrameworkTestabilityMessage4::validateTraceClientIdContent ()
{
    if (1 != m_traceClientId1)
    {
        return (false);
    }

    if (7 != m_traceClientId2)
    {
        return (false);
    }

    if (77 != m_traceClientId3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupTraceLevelInput ()
{
    m_traceLevel1 = TRACE_LEVEL_UNKNOWN;
    m_traceLevel2 = TRACE_LEVEL_FATAL;
    m_traceLevel3 = TRACE_LEVEL_INFO;
}

bool FrameworkTestabilityMessage4::validateTraceLevelContent ()
{
    if (TRACE_LEVEL_UNKNOWN != m_traceLevel1)
    {
        return (false);
    }

    if (TRACE_LEVEL_FATAL != m_traceLevel2)
    {
        return (false);
    }

    if (TRACE_LEVEL_INFO != m_traceLevel3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setWorldWideNameContent ()
{
    m_worldWideName = WorldWideName ("11:22:33:44:55:66:77:88");
}

bool FrameworkTestabilityMessage4::validateWorldWideNameContent ()
{
    const WorldWideName tempWorldWideName ("11:22:33:44:55:66:77:88");

    if (tempWorldWideName != m_worldWideName)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setWorldWideNameVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            string tempString = string ("11:22:33:44:55:66:77:") + i + string ("") + j;
            m_worldWideNameVector.push_back (WorldWideName (tempString));
        }
    }
}

bool FrameworkTestabilityMessage4::validateWorldWideNameVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            const string        tempString        = string ("11:22:33:44:55:66:77:") + i + string ("") + j;
            const WorldWideName tempWorldWideName   (tempString);

            if (tempWorldWideName != m_worldWideNameVector[i * 10 + j])
            {
                return (false);
            }
        }
    }

    return (true);
}

void FrameworkTestabilityMessage4::setMacAddressContent ()
{
    m_macAddress = MacAddress ("11:22:33:44:55:66");
}

bool FrameworkTestabilityMessage4::validateMacAddressContent ()
{
    const MacAddress tempMacAddress ("11:22:33:44:55:66");

    if (tempMacAddress != m_macAddress)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setMacAddressVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            string tempString = string ("11:22:33:44:55:") + i + string ("") + j;
            m_macAddressVector.push_back (MacAddress (tempString));
        }
    }
}

bool FrameworkTestabilityMessage4::validateMacAddressVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            const string     tempString     = string ("11:22:33:44:55:") + i + string ("") + j;
            const MacAddress tempMacAddress   (tempString);

            if (tempMacAddress != m_macAddressVector[i * 10 + j])
            {
                return (false);
            }
        }
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupEnumInput ()
{
    m_enum1 = Monday;
    m_enum2 = Wednesday;
    m_enum3 = Saturday;
}

bool FrameworkTestabilityMessage4::validateEnumContent ()
{
    if (Monday != m_enum1)
    {
	return (false);
    }

    if (Wednesday != m_enum2)
    {
        return (false);
    }

    if (Saturday != m_enum3)
    {
        return (false);
    }

    return (true);

}

void FrameworkTestabilityMessage4::setBitMapContent ()
{
    m_bitMap1.fromString("ab02cd34376987fedc3ba2736dfe2a87c37bf39074683bc375acf37289463dea");
    m_bitMap2.fromString("10ac384756afdbc07fd28bc90fa836500bc8740fedba34267dbca0953ef02bc010354cbe3", 289);
    m_bitMap3.fromString("ab02cd3bc07fd2fe2a870fedba3f372bc90fa836500953ef0bc8740fedba2cd34376987fedc3ba8740fedba34267db84756afdbc07fd372bc91fa836511953ef");
}

bool FrameworkTestabilityMessage4::validateBitMapContent ()
{
    BitMap tempBitMap1;
    BitMap tempBitMap2;
    BitMap tempBitMap3;

    tempBitMap1.fromString("ab02cd34376987fedc3ba2736dfe2a87c37bf39074683bc375acf37289463dea");
    tempBitMap2.fromString("10ac384756afdbc07fd28bc90fa836500bc8740fedba34267dbca0953ef02bc010354cbe3", 289);
    tempBitMap3.fromString("ab02cd3bc07fd2fe2a870fedba3f372bc90fa836500953ef0bc8740fedba2cd34376987fedc3ba8740fedba34267db84756afdbc07fd372bc91fa836511953ef");

    if (m_bitMap1 != tempBitMap1)
    {
        return (false);
    }

    if (m_bitMap2 != tempBitMap2)
    {
        return (false);
    }

    if (m_bitMap3 != tempBitMap3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setDateContent ()
{
    //trace (TRACE_LEVEL_INFO, "Setting DateContent");
    m_date = Date ("2010-04-21");
}

bool FrameworkTestabilityMessage4::validateDateContent ()
{
    const Date tempDate ("2010-04-21");

    if (tempDate != m_date)
    {
        trace (TRACE_LEVEL_ERROR, string("Validation of DateContent failed. Expected & actual values are:") + tempDate.toString() + string(" ") + m_date.toString());
        return (false);
    }

    //trace (TRACE_LEVEL_INFO, "Validated DateContent successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setDateVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;

    //trace (TRACE_LEVEL_INFO, "Setting DateVectorContent");
    for (i = 0; i < 3; i++)
    {
        for (j = 1; j < 10; j++)
        {
            string tempString = string("2010-04-") + i + string ("") + j;
            m_dateVector.push_back (Date (tempString));
        }
    }
}

bool FrameworkTestabilityMessage4::validateDateVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;
    UI32 k = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 1; j < 10; j++)
        {
            const string    tempString = string("2010-04-") + i + string ("") + j;
            const Date      tempDate (tempString);

            if (tempDate != m_dateVector[k])
            {
                trace (TRACE_LEVEL_ERROR, string("Validation of DateVectorContent failed. Expected & actual values are:") + tempDate.toString() + string(" ") + m_dateVector[k].toString());
                return (false);
            }
            k++;
        }
    }

    //trace (TRACE_LEVEL_INFO, "Validated DateVectorContent successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setTimeContent ()
{
    //trace (TRACE_LEVEL_INFO, "Setting TimeContent");
    // HH:MM:SS.Micro<+|->TimezoneHours:TimezoneMinutes
    m_time = Time ("18:39:22.000228");
}

bool FrameworkTestabilityMessage4::validateTimeContent ()
{
    const Time tempTime ("18:39:22.000228");

    if (tempTime != m_time)
    {
        trace (TRACE_LEVEL_ERROR, string("Validation of TimeContent failed. Expected & actual values are: ") + tempTime.toString() + string(" ") + m_time.toString());
        return (false);
    }

    //trace (TRACE_LEVEL_INFO, "Validated TimeContent successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setTimeVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;

    //trace (TRACE_LEVEL_INFO, "Setting TimeVectorContent");
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 10; j++)
        {
            string tempString = string("18:") + i + string ("") + j + string (":22.000228");
            m_timeVector.push_back (Time (tempString));
        }
    }
}

bool FrameworkTestabilityMessage4::validateTimeVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;
    UI32 k = 0;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 10; j++)
        {
            const string    tempString = string("18:") + i + string ("") + j + string (":22.000228");
            const Time      tempTime (tempString);

            if (tempTime != m_timeVector[k])
            {
                trace (TRACE_LEVEL_FATAL, string("Validation of TimeVectorContent failed. Expected & actual values are: ") + tempTime.toString() + string(" ") + m_timeVector[k].toString());
                return (false);
            }
            k++;
        }
    }

    //trace (TRACE_LEVEL_INFO, "Validated TimeVectorContent successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setDateTimeContent ()
{
    //trace (TRACE_LEVEL_INFO, "Setting DateTimeContent");
    // YYYY-MM-DD HH:MM:SS.Micro<+|->TimezoneHours:TimezoneMinutes
    m_datetime = DateTime ("2010-04-10 18:39:22.000333");
}

bool FrameworkTestabilityMessage4::validateDateTimeContent ()
{
    const DateTime tempDateTime ("2010-04-10 18:39:22.000333");

    if (tempDateTime != m_datetime)
    {
        trace (TRACE_LEVEL_FATAL, string("Validation of DateTimeContent failed. Expected & actual values are: ") + tempDateTime.toString() + string(" ") + m_datetime.toString());
        return (false);
    }

    //trace (TRACE_LEVEL_INFO, "Validated DateTimeContent successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setDateTimeVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 10; j++)
        {
            string tempString = string("2010-04-10 18:") + i + string ("") + j + string (":22.000121");
            //trace (TRACE_LEVEL_INFO, string("FrameworkTestabilityMessage4::setDateTimeVectorContent:") + tempString + string(" i:") + i + string(" j:") + j);
            m_datetimeVector.push_back (DateTime (tempString));
        }
    }
}

bool FrameworkTestabilityMessage4::validateDateTimeVectorContent ()
{
    UI32 i = 0;
    UI32 j = 0;
    UI32 k = 0;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 10; j++)
        {
            const string    tempString = string("2010-04-10 18:") + i + string ("") + j + string (":22.000121");
            //trace (TRACE_LEVEL_INFO, string("FrameworkTestabilityMessage4::validateDateTimeVectorContent:") + tempString + string(" i:") + i + string(" j:") + j);
            const DateTime  tempDateTime (tempString);

            if (tempDateTime != m_datetimeVector[k])
            {
                trace (TRACE_LEVEL_FATAL, string("Validation of DateTimeVectorContent failed. Expected & actual values are: ") + tempDateTime.toString() + string(" ") + m_datetimeVector[k].toString());
                return (false);
            }
            k++;
        }
    }

    //trace (TRACE_LEVEL_INFO, "Validated DateTimeVectorContent successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setDecimal64Content ()
{
    //trace (TRACE_LEVEL_INFO, "Setting Decimal64Content");
    m_decimal64 = Decimal64 ("23523.98");
}

bool FrameworkTestabilityMessage4::validateDecimal64Content ()
{
    const Decimal64 tempDecimal64 ("23523.98");

    if (tempDecimal64 != m_decimal64)
    {
        trace (TRACE_LEVEL_ERROR, string("Validation of Decimal64Content failed. Expected & actual values are:") + tempDecimal64.toString() + string(" ") + m_decimal64.toString());
        return (false);
    }

    //trace (TRACE_LEVEL_INFO, "Validated Decimal64Content successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setDecimal64VectorContent ()
{
    UI32 i = 0;

    //trace (TRACE_LEVEL_INFO, "Setting Decimal64VectorContent");
    for (i = 0; i < 10; i++)
    {
        string tempString = string("23523.98") + i;
        m_decimal64Vector.push_back (Decimal64 (tempString));
    }
}

bool FrameworkTestabilityMessage4::validateDecimal64VectorContent ()
{
    UI32 i = 0;

    for (i = 0; i < 10; i++)
    {
        const string    tempString = string("23523.98") + i;
        const Decimal64 tempDecimal64 (tempString);

        if (tempDecimal64 != m_decimal64Vector[i])
        {
            trace (TRACE_LEVEL_ERROR, string("Validation of Decimal64VectorContent failed. Expected & actual values are:") + tempDecimal64.toString() + string(" ") + m_decimal64Vector[i].toString());
            return (false);
        }
    }

    //trace (TRACE_LEVEL_INFO, "Validated Decimal64VectorContent successfully");
    return (true);
}

void FrameworkTestabilityMessage4::setIpV4AddressNetworkMask ()
{
    m_ipV4NetworkMask.fromString("192.168.1.10/24");
}

bool FrameworkTestabilityMessage4::validateIpV4AddressNetworkMask ()
{
    IpV4AddressNetworkMask tempIpV4AddressNetworkMask ("192.168.1.10/24");

    if (m_ipV4NetworkMask != tempIpV4AddressNetworkMask)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setIpV6AddressNetworkMask ()
{
    m_ipV6NetworkMask.fromString("2001:DB8::1428:57A8/125");
}

bool FrameworkTestabilityMessage4::validateIpV6AddressNetworkMask ()
{
    IpV6AddressNetworkMask tempIpV6AddressNetworkMask ("2001:DB8::1428:57A8/125");

    if (m_ipV6NetworkMask != tempIpV6AddressNetworkMask)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setSnmpObjectId ()
{
    m_snmpObjectId.fromString("1.22.3.45.3.125");
}

bool FrameworkTestabilityMessage4::validateSnmpObjectId ()
{
    SnmpObjectId tempSnmpObjectId ("1.22.3.45.3.125");

    if (m_snmpObjectId !=  tempSnmpObjectId)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setUI32Range ()
{
    string rangeString1 = "1,2-5,8,9-12";
    string rangeString2 = "10-1000,1200,1202-1300,1350-1400,1450-10000,100000-100005";
    string rangeString3 = "1,4,7,9,10,12-43,10000-11001,12345-98765,1234567-7654321,9876543-9999887";
    string rangeString4 = "1";
    string rangeString5 = "1-10000000";
    string rangeString6 = "1,2,3";

    if (true == m_UI32Range1.isAValidString (rangeString1))
    {
        m_UI32Range1.fromString (rangeString1);
    }

    if (true == m_UI32Range2.isAValidString (rangeString2))
    {
        m_UI32Range2.fromString (rangeString2);
    }

    if (true == m_UI32Range3.isAValidString (rangeString3))
    {
        m_UI32Range3.fromString (rangeString3);
    }

    if (true == m_UI32Range4.isAValidString (rangeString4))
    {
        m_UI32Range4.fromString (rangeString4);
    }

    if (true == m_UI32Range5.isAValidString (rangeString5))
    {
        m_UI32Range5.fromString (rangeString5);
    }

    if (true == m_UI32Range6.isAValidString (rangeString6))
    {
        m_UI32Range6.fromString (rangeString6);
    }
}

void FrameworkTestabilityMessage4::setMacRange ()
{
    string rangeString1 = "1001.2200.030a,1001.2200.0321-33";
    string rangeString2 = "1001.2200.1010-20,1200.1200.1200,1202.1202.12A1-C5";
    string rangeString3 = "0000.3333.2222,1111.4444.5555,6666.0000.44AA-CC";
    string rangeString4 = "1000.0000.0001";
    string rangeString5 = "0000.1111.ABCD-EF,1000.0001.bbcc-dd";
    string rangeString6 = "1111.2222.3333,5555.6666.7777,AAAA.BBBB.CCCC";

    if (true == m_MacRange1.isAValidString (rangeString1))
    {
        m_MacRange1.fromString (rangeString1);
    }
    
    if (true == m_MacRange2.isAValidString (rangeString2))
    {   
        m_MacRange2.fromString (rangeString2);
    }

    if (true == m_MacRange3.isAValidString (rangeString3))
    {   
        m_MacRange3.fromString (rangeString3);
    }
    
    if (true == m_MacRange4.isAValidString (rangeString4))
    {   
        m_MacRange4.fromString (rangeString4);
    }

    if (true == m_MacRange5.isAValidString (rangeString5))
    {   
        m_MacRange5.fromString (rangeString5);
    }

    if (true == m_MacRange6.isAValidString (rangeString6))
    {   
        m_MacRange6.fromString (rangeString6);
    }
}


bool FrameworkTestabilityMessage4::validateUI32Range ()
{
    UI32Range rangeString1 ("1,2-5,8,9-12");
    UI32Range rangeString2 ("10-1000,1200,1202-1300,1350-1400,1450-10000,100000-100005");
    UI32Range rangeString3 ("1,4,7,9,10,12-43,10000-11001,12345-98765,1234567-7654321,9876543-9999887");
    UI32Range rangeString4 ("1");
    UI32Range rangeString5 ("1-10000000");
    UI32Range rangeString6 ("1,2,3");

    if (m_UI32Range1 != rangeString1)
    {
        return (false);
    }

    if (m_UI32Range2 != rangeString2)
    {
        return (false);
    }

    if (m_UI32Range3 != rangeString3)
    {
        return (false);
    }

    if (m_UI32Range4 != rangeString4)
    {
        return (false);
    }

    if (m_UI32Range5 != rangeString5)
    {
        return (false);
    }

    if (m_UI32Range6 != rangeString6)
    {
        return (false);
    }

    return (true);
}

bool FrameworkTestabilityMessage4::validateMacRange ()
{
    MACRange rangeString1 ("1001.2200.030a,1001.2200.0321-33");
    MACRange rangeString2 ("1001.2200.1010-20,1200.1200.1200,1202.1202.12A1-C5");
    MACRange rangeString3 ("0000.3333.2222,1111.4444.5555,6666.0000.44AA-CC");
    MACRange rangeString4 ("1000.0000.0001");
    MACRange rangeString5 ("0000.1111.ABCD-EF,1000.0001.bbcc-dd");
    MACRange rangeString6 ("1111.2222.3333,5555.6666.7777,AAAA.BBBB.CCCC");

    if (m_MacRange1 != rangeString1)
    {
        return (false);
    }

    if (m_MacRange2 != rangeString2)
    {   
        return (false);
    }
    
    if (m_MacRange3 != rangeString3)
    {   
        return (false);
    }

    if (m_MacRange4 != rangeString4)
    {   
        return (false);
    }

    if (m_MacRange5 != rangeString5)
    {   
        return (false);
    }

    if (m_MacRange6 != rangeString6)
    {   
        return (false);
    }
    
    return (true);
}


void FrameworkTestabilityMessage4::setIpVxAddress ()
{
    string ipVxString1 = "127.0.0.2";
    string ipVxString2 = "0:0:0:0:0:0:0:2";

    m_ipVxAddress1.fromString(ipVxString1);
    m_ipVxAddress2.fromString(ipVxString2);
}

bool FrameworkTestabilityMessage4::validateIpVxAddress ()
{
    string ipVxString1 = "127.0.0.2";
    string ipVxString2 = "0:0:0:0:0:0:0:2";
    
    IpVxAddress tempIpVxAddress1, tempIpVxAddress2;
    
    tempIpVxAddress1.fromString(ipVxString1);
    tempIpVxAddress2.fromString(ipVxString2);

    if ((tempIpVxAddress1 != m_ipVxAddress1) || (tempIpVxAddress2 != m_ipVxAddress2))
    {
        return false;
    }

    return true;
}

void FrameworkTestabilityMessage4::setIpVxAddressVector ()
{
    string ipVxString1 = "127.0.0.2";
    string ipVxString2 = "0:0:0:0:0:0:0:2";
    string ipVxString3 = "127.0.0.3";
    string ipVxString4 = "0:0:0:0:0:0:0:3";

    IpVxAddress tempIpVxAddress1(ipVxString1);
    IpVxAddress tempIpVxAddress2(ipVxString2);
    IpVxAddress tempIpVxAddress3(ipVxString3);
    IpVxAddress tempIpVxAddress4(ipVxString4);
    m_ipVxAddressVector.push_back (tempIpVxAddress1);
    m_ipVxAddressVector.push_back (tempIpVxAddress2);
    m_ipVxAddressVector.push_back (tempIpVxAddress3);
    m_ipVxAddressVector.push_back (tempIpVxAddress4);

}

bool FrameworkTestabilityMessage4::validateIpVxAddressVector ()
{
    string ipVxString1 = "127.0.0.2";
    string ipVxString2 = "0:0:0:0:0:0:0:2";
    string ipVxString3 = "127.0.0.3";
    string ipVxString4 = "0:0:0:0:0:0:0:3";

    IpVxAddress tempIpVxAddress1(ipVxString1);
    IpVxAddress tempIpVxAddress2(ipVxString2);
    IpVxAddress tempIpVxAddress3(ipVxString3);
    IpVxAddress tempIpVxAddress4(ipVxString4);

    if (m_ipVxAddressVector.size() != 4)
    {
        return false;
    }
    
    if ((m_ipVxAddressVector[0] != tempIpVxAddress1) || 
            (m_ipVxAddressVector[1] != tempIpVxAddress2) ||
            (m_ipVxAddressVector[2] != tempIpVxAddress3) || 
            (m_ipVxAddressVector[3] != tempIpVxAddress4))
    {
        return false;
    }
            
    return true;
}

void FrameworkTestabilityMessage4::setLargeObjectContent ()
{
    m_largeObject1.fromString("ab02cd34376987fedc3ba2736dfe2a87c37bf39074683bc375acf37289463dea");
    m_largeObject2.fromString("10ac384756afdbc07fd28bc90fa836500bc8740fedba34267dbca0953ef02bc010354cbe3", 32);
    m_largeObject3.fromString("ab02cd3bc07fd2fe2a870fedba3f372bc90fa836500953ef0bc8740fedba2cd34376987fedc3ba8740fedba34267db84756afdbc07fd372bc91fa836511953ef");
}

bool FrameworkTestabilityMessage4::validateLargeObjectContent ()
{
    LargeObject tempLargeObject1;
    LargeObject tempLargeObject2;
    LargeObject tempLargeObject3;

    tempLargeObject1.fromString("ab02cd34376987fedc3ba2736dfe2a87c37bf39074683bc375acf37289463dea");
    tempLargeObject2.fromString("10ac384756afdbc07fd28bc90fa836500bc8740fedba34267dbca0953ef02bc010354cbe3", 32);
    tempLargeObject3.fromString("ab02cd3bc07fd2fe2a870fedba3f372bc90fa836500953ef0bc8740fedba2cd34376987fedc3ba8740fedba34267db84756afdbc07fd372bc91fa836511953ef");

    if (m_largeObject1 != tempLargeObject1)
    {
        return (false);
    }

    if (m_largeObject2 != tempLargeObject2)
    {
        return (false);
    }

    if (m_largeObject3 != tempLargeObject3)
    {
        return (false);
    }

    return (true);
}

void FrameworkTestabilityMessage4::setupInput ()
{
    setupBoolInput                   ();
    setupCharInput                   ();
    setupSI16Input                   ();
    setupSI32Input                   ();
    setupSI64Input                   ();
    setupSI8Input                    ();
    setupSI64VectorInput             ();
    setupStringInput                 ();
    setupStringVectorInput           ();
    setupUI16Input                   ();
    setupUI32Input                   ();
    setupUI64Input                   ();
    setupUI8Input                    ();
    setupLocationIdInput             ();
    setupResourceIdInput             ();
    setupWaveMessagePriorityInput    ();
    setupWaveMessageTypeInput        ();
    setupPrismServiceIdInput         ();
    setupSerializableObjectTypeInput ();
    setupTraceClientIdInput          ();
    setupTraceLevelInput             ();
    setWorldWideNameContent          ();
    setWorldWideNameVectorContent    ();
    setMacAddressContent             ();
    setMacAddressVectorContent       ();
    setupEnumInput                   ();
    setBitMapContent                 ();
    setDateContent                   ();
    setDateVectorContent             ();
    setTimeContent                   ();
    setTimeVectorContent             ();
    setDateTimeContent               ();
    setDateTimeVectorContent         ();
    setDecimal64Content              ();
    setDecimal64VectorContent        ();
    setIpV4AddressNetworkMask        ();
    setIpV6AddressNetworkMask        ();
    setSnmpObjectId                  ();
    setUI32Range                     ();
    setIpVxAddress                   ();
    setIpVxAddressVector             ();
    setLargeObjectContent            ();
    setMacRange                     ();
}

bool FrameworkTestabilityMessage4::validateContent ()
{
    if (true != (validateBoolContent ()))
    {
        return (false);
    }

    if (true != (validateCharContent ()))
    {
        return (false);
    }

    if (true != (validateSI16Content ()))
    {
        return (false);
    }

    if (true != (validateSI32Content ()))
    {
        return (false);
    }

    if (true != (validateSI64Content ()))
    {
	return (false);
    }

    if (true != (validateSI8Content ()))
    {
        return (false);
    }

    if (true != (validateSI64VectorContent ()))
    {
	return (false);
    }

    if (true != (validateStringContent ()))
    {
        return (false);
    }

    if (true != (validateStringVectorContent ()))
    {
        return (false);
    }

    if (true != (validateUI16Content ()))
    {
        return (false);
    }

    if (true != (validateUI32Content ()))
    {
        return (false);
    }

    if (true != (validateUI64Content ()))
    {
	return (false);
    }

    if (true != (validateUI8Content ()))
    {
        return (false);
    }

    if (true != (validateLocationIdContent ()))
    {
        return (false);
    }

    if (true != (validateResourceIdContent ()))
    {
        return (false);
    }

    if (true != (validateWaveMessagePriorityContent ()))
    {
        return (false);
    }

    if (true != (validateWaveMessageTypeContent ()))
    {
        return (false);
    }

    if (true != (validatePrismServiceIdContent ()))
    {
        return (false);
    }

    if (true != (validateSerializableObjectTypeContent ()))
    {
        return (false);
    }

    if (true != (validateTraceClientIdContent ()))
    {
        return (false);
    }

    if (true != (validateTraceLevelContent ()))
    {
        return (false);
    }

    if (true != (validateWorldWideNameContent ()))
    {
        return (false);
    }

    if (true != (validateWorldWideNameVectorContent ()))
    {
        return (false);
    }

    if (true != (validateMacAddressContent ()))
    {
        return (false);
    }

    if (true != (validateMacAddressVectorContent ()))
    {
        return (false);
    }

    if (true != (validateEnumContent ()))
    {
	return (false);
    }

    if (true != (validateBitMapContent ()))
    {
        return (false);
    }
    if (true != (validateDateContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate Date test failed");
        return (false);
    }

    if (true != (validateDateVectorContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate DateVector test failed");
        return (false);
    }

    if (true != (validateTimeContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate Time test failed");
        return (false);
    }

    if (true != (validateTimeVectorContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate TimeVector test failed");
        return (false);
    }

    if (true != (validateDateTimeContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate DateTime test failed");
        return (false);
    }

    if (true != (validateDateTimeVectorContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate DateTimeVector test failed");
        return (false);
    }

    if (true != (validateDecimal64Content ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate Decimal64 test failed");
        return (false);
    }

    if (true != (validateDecimal64VectorContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate Decimal64Vector test failed");
        return (false);
    }

    if (true != (validateIpV4AddressNetworkMask ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate IpV4AddressNetworkMask test failed");
        return (false);
    }

    if (true != (validateIpV6AddressNetworkMask ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate IpV6AddressNetworkMask test failed");
        return (false);
    }

    if (true != (validateSnmpObjectId ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate SnmpObjectId test failed");
        return (false);
    }

    if (true != (validateUI32Range ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate UI32Range test failed");
        return (false);
    }
    
    if (true != (validateIpVxAddress ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate IpVxAddress test failed");
        return (false);
    }
    
    if (true != (validateIpVxAddressVector ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate IpVxAddressVector test failed");
        return (false);
    }

    if (true != (validateLargeObjectContent ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate LargeObject test failed");
        return (false);
    }

    if (true != (validateMacRange ()))
    {
        trace (TRACE_LEVEL_FATAL, "Validate MACRange test failed");
        return (false);
    }

    return (true);
}

FrameworkTestabilityMessage5::FrameworkTestabilityMessage5 ()
    : PrismMessage (FrameworkTestability1ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE5)
{
}

FrameworkTestabilityMessage5::~FrameworkTestabilityMessage5 ()
{
}

FrameworkTestabilityMessage6::FrameworkTestabilityMessage6 ()
    : PrismMessage (FrameworkTestability1ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE6)
{
}

FrameworkTestabilityMessage6::~FrameworkTestabilityMessage6 ()
{
}

FrameworkTestabilityMessage7::FrameworkTestabilityMessage7 ()
    : PrismMessage (FrameworkTestability1ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE7)
{
}

FrameworkTestabilityMessage7::~FrameworkTestabilityMessage7 ()
{
}

FrameworkTestabilityMessage8::FrameworkTestabilityMessage8 ()
    : PrismMessage (FrameworkTestability4ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE8),
      m_numberOfObjectsToBeCreated (0),
      m_pageSize (0)
{
}

FrameworkTestabilityMessage8::~FrameworkTestabilityMessage8 ()
{
}

UI32 FrameworkTestabilityMessage8::getNumberOfObjectsToBeCreated () const
{
    return (m_numberOfObjectsToBeCreated);
}

void FrameworkTestabilityMessage8::setNumberOfObjectsToBeCreated (const UI32 &numberOfObjectsToBeCreated)
{
    m_numberOfObjectsToBeCreated = numberOfObjectsToBeCreated;
}

UI32  FrameworkTestabilityMessage8::getPageSize()
{
   return m_pageSize;
}

void  FrameworkTestabilityMessage8::setPageSize(UI32 pageSize)
{
   m_pageSize = pageSize;
}

FrameworkTestabilityMessage9::FrameworkTestabilityMessage9 ()
    : PrismMessage (FrameworkTestability4ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE9)
{
}

FrameworkTestabilityMessage9::~FrameworkTestabilityMessage9 ()
{
}

FrameworkTestabilityMessage10::FrameworkTestabilityMessage10 ()
    : PrismMessage (FrameworkTestability5ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE10)
{
}

FrameworkTestabilityMessage10::~FrameworkTestabilityMessage10 ()
{
}

FrameworkTestabilityMessage11::FrameworkTestabilityMessage11()
  : PrismMessage (FrameworkTestability6ObjectManager::getPrismServiceId (), FRAMEWORK_TESTABILITY_MESSAGE11)
{

}

FrameworkTestabilityMessage11::~FrameworkTestabilityMessage11()
{

}
void FrameworkTestabilityMessage11::setNumberOfObjectsToBeCreated (const UI32 &numberOfObjectsToBeCreated)
{
	 m_numberOfObjectsToBeCreated = numberOfObjectsToBeCreated;
}

UI32 FrameworkTestabilityMessage11::getNumberOfObjectsToBeCreated() const
{
	 return (m_numberOfObjectsToBeCreated);
}

}
